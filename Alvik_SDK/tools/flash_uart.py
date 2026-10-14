#!/usr/bin/env python3
"""Flash the STM32F411 through the Nano ESP32 UART bridge (ST bootloader, AN3155).

The bridge (Arduino_Alvik/examples/bridge_firmware_updater) holds BOOT0 high and
resets the STM32 into its system bootloader when the ESP32 starts. It flushes its
UART every time the host changes the line coding, and STM32_Programmer_CLI changes
it several times while connecting, so its replies get lost. This script opens the
port once, waits for the bridge to settle, then talks to the bootloader directly.

BOOT0 stays high, so the program is started with the Go command rather than a reset.
To flash again, reset the Nano ESP32 so the bridge puts the STM32 back in the bootloader.
"""
import argparse
import sys
import time

import serial

ACK, NACK = 0x79, 0x1F
FLASH_BASE = 0x08000000
# STM32F411xC flash sectors: 4 x 16 KB, 1 x 64 KB, 1 x 128 KB
SECTOR_SIZES = [0x4000] * 4 + [0x10000, 0x20000]


def xor(data):
    cs = 0
    for b in data:
        cs ^= b
    return cs


class Bootloader:
    def __init__(self, port, baud):
        self.s = serial.Serial(port, baud, parity=serial.PARITY_EVEN, timeout=1)
        time.sleep(0.5)  # let the bridge apply the line coding before talking
        self.s.reset_input_buffer()

    def ack(self, timeout=1.0):
        self.s.timeout = timeout
        r = self.s.read(1)
        self.s.timeout = 1
        if not r:
            raise IOError("no reply from bootloader")
        if r[0] != ACK:
            raise IOError(f"bootloader answered 0x{r[0]:02x} instead of ACK")

    def sync(self):
        # A fresh bootloader ACKs 0x7F. One already synced, or left mid-command by an
        # earlier tool, NACKs after a few bytes; either reply means we are aligned.
        self.s.timeout = 0.1
        for _ in range(300):
            self.s.write(b"\x7f")
            r = self.s.read(1)
            if r and r[0] in (ACK, NACK):
                break
        else:
            raise IOError("bootloader not responding: is the Alvik switched on? "
                          "Reset the Nano ESP32 and try again")
        self.s.timeout = 1
        time.sleep(0.05)
        self.s.reset_input_buffer()

    def cmd(self, code):
        self.s.write(bytes([code, code ^ 0xFF]))
        self.ack()

    def send_addr(self, addr):
        a = addr.to_bytes(4, "big")
        self.s.write(a + bytes([xor(a)]))
        self.ack()

    def get_id(self):
        self.cmd(0x02)
        n = self.s.read(1)[0]
        pid = self.s.read(n + 1)
        self.ack()
        return int.from_bytes(pid, "big")

    def erase_sectors(self, sectors):
        self.cmd(0x44)
        payload = (len(sectors) - 1).to_bytes(2, "big")
        payload += b"".join(s.to_bytes(2, "big") for s in sectors)
        self.s.write(payload + bytes([xor(payload)]))
        self.ack(timeout=30)

    def write(self, addr, chunk):
        chunk += b"\xff" * (-len(chunk) % 4)
        self.cmd(0x31)
        self.send_addr(addr)
        payload = bytes([len(chunk) - 1]) + chunk
        self.s.write(payload + bytes([xor(payload)]))
        self.ack(timeout=2)

    def read(self, addr, n):
        self.cmd(0x11)
        self.send_addr(addr)
        self.s.write(bytes([n - 1, (n - 1) ^ 0xFF]))
        self.ack()
        return self.s.read(n)

    def go(self, addr):
        self.cmd(0x21)
        self.send_addr(addr)


def sectors_for(size):
    sectors, end = [], 0
    for i, sz in enumerate(SECTOR_SIZES):
        if end >= size:
            break
        sectors.append(i)
        end += sz
    if end < size:
        raise ValueError(f"image is {size} bytes, larger than the 256 KB flash")
    return sectors


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("image", help="raw .bin image, written at 0x08000000")
    ap.add_argument("-p", "--port", default="/dev/ttyACM0")
    ap.add_argument("-b", "--baud", type=int, default=115200)
    ap.add_argument("--no-verify", action="store_true")
    ap.add_argument("--no-go", action="store_true", help="stay in the bootloader")
    args = ap.parse_args()

    image = open(args.image, "rb").read()
    bl = Bootloader(args.port, args.baud)
    bl.sync()
    print(f"Connected, chip ID 0x{bl.get_id():03x}")

    sectors = sectors_for(len(image))
    print(f"Erasing sectors {sectors[0]}-{sectors[-1]}...")
    bl.erase_sectors(sectors)

    for off in range(0, len(image), 256):
        bl.write(FLASH_BASE + off, image[off:off + 256])
        print(f"\rWriting {min(off + 256, len(image))}/{len(image)} bytes", end="", flush=True)
    print()

    if not args.no_verify:
        for off in range(0, len(image), 256):
            chunk = image[off:off + 256]
            if bl.read(FLASH_BASE + off, len(chunk)) != chunk:
                sys.exit(f"Verify failed at 0x{FLASH_BASE + off:08x}")
        print("Verified")

    if not args.no_go:
        bl.go(FLASH_BASE)
        print("Started program at 0x08000000")


if __name__ == "__main__":
    try:
        main()
    except (IOError, ValueError) as e:
        sys.exit(f"Error: {e}")
