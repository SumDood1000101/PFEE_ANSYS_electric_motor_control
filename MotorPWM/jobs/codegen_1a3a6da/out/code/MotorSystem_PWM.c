/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Command: swan_cg.exe config.json -student
*************************************************************$ */
#include "swan_sensors.h"
#include "swan_consts.h"
#include "MotorSystem_PWM.h"

/* PWM::MotorSystem */
void MotorSystem_PWM(
  /* ref_speed */swan_float32 ref_speed,
  /* curr_speed */swan_float32 curr_speed,
  /* pwm_out */swan_bool * restrict pwm_out,
  outC_MotorSystem_PWM * restrict outC)
{
  swan_float32 x1;

  /* #0: */
  PIController_PWM(
    ref_speed,
    curr_speed,
    swan_lit_float32(0.5),
    swan_lit_float32(0.5),
    swan_lit_float32(0.0),
    swan_lit_float32(1.0),
    &x1,
    &outC->Ctx_PIController1);
  /* #1: */
  PWMGenerator_PWM(swan_lit_int32(100), x1, pwm_out, &outC->Ctx_PWMGenerator1);
}

#ifndef SWAN_USER_DEFINED_INIT
void MotorSystem_init_PWM(outC_MotorSystem_PWM * restrict outC)
{
  /* #0: */ PIController_init_PWM(&outC->Ctx_PIController1);
  /* #1: */ PWMGenerator_init_PWM(&outC->Ctx_PWMGenerator1);
}
#endif /* SWAN_USER_DEFINED_INIT */


#ifndef SWAN_NO_EXTERN_CALL_TO_RESET
void MotorSystem_reset_PWM(outC_MotorSystem_PWM * restrict outC)
{
  /* #0: */ PIController_reset_PWM(&outC->Ctx_PIController1);
  /* #1: */ PWMGenerator_reset_PWM(&outC->Ctx_PWMGenerator1);
}
#endif /* SWAN_NO_EXTERN_CALL_TO_RESET */




/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** MotorSystem_PWM.c
*************************************************************$ */
