/*
    Wheel speed control by the SCADE One model PWM::MotorSystem.
*/

#ifndef __SCADE_CONTROL_H__
#define __SCADE_CONTROL_H__

#include "alvik.h"


void scade_control_init(void);
void scade_control_set_rpm(motor_id_t motor, float rpm);
float scade_control_get_target(motor_id_t motor);

// One step of the model for each wheel; pass to timer_start()
void scade_control_step(void);


#endif
