/*
    Minimal hardware layer for the Alvik carrier (STM32F411, HAL):
    switch a motor on/off, read a wheel's speed, run a function at a fixed rate.
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


#endif
