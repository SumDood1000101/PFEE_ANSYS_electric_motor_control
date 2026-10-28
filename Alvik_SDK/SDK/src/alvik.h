/*
    Minimal hardware layer for the Alvik carrier (STM32F411, HAL):
    switch a motor on/off, read a wheel's speed, run a function at a fixed rate,
    read the accelerometer and gyroscope.
*/

#ifndef __ALVIK_H__
#define __ALVIK_H__

#include <stdbool.h>
#include "stm32f4xx_hal.h"


// Rate of the timer callback. The tick timer passed to alvik_init() must be
// set to this rate in CubeMX (TIM10: prescaler 99, period 99 at 100 MHz).
#define ALVIK_TICK_HZ           10000u

typedef enum {
    MOTOR_LEFT = 0,
    MOTOR_RIGHT,
    MOTOR_COUNT
} motor_id_t;


// Motors off, encoders running, motor driver enabled, tick timer started.
// The SDK defines HAL_TIM_PeriodElapsedCallback, so do not define it elsewhere.
void alvik_init(TIM_HandleTypeDef * tick_timer);

// on: motor driven forward at full power, off: motor coasts
void motor_set(motor_id_t motor, bool on);

// Wheel speed in rpm, measured from the encoder every 20 ms
float motor_get_rpm(motor_id_t motor);

// Call callback ALVIK_TICK_HZ times per second, from the timer interrupt
void timer_start(void (*callback)(void));


typedef struct {
    float ax, ay, az;       // acceleration, g
    float gx, gy, gz;       // angular velocity, deg/s
} imu_data_t;

// LSM6DSO on I2C1 (PB7/PB8, set up in CubeMX): 104 Hz, +-4 g, +-2000 deg/s.
// Returns false if the chip does not answer with its WHO_AM_I id.
bool imu_init(I2C_HandleTypeDef * i2c);

// Latest accelerometer and gyro sample, with the same axes as the Arduino
// library's getAcceleration*() / getAngularVelocity*().
// Blocking I2C read (~0.4 ms): call it from the main loop, not from the timer callback.
bool imu_read(imu_data_t * data);


#endif
