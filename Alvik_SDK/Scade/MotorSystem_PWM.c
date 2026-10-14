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
  swan_float32 x2;

  x1 = ref_speed - curr_speed;
  outC->mem1 = outC->mem1 + T_CYCLE_PWM * x1;
  x2 = x1 * kp_PWM + outC->mem1 * ki_PWM;
  if (x2 > swan_lit_float32(1.0)) {
    x2 = swan_lit_float32(1.0);
  }
  else if (x2 < swan_lit_float32(0.0)) {
    x2 = swan_lit_float32(0.0);
  }
  *pwm_out = outC->mem2 < (swan_int32) (x2 * (swan_float32) swan_lit_int32(100));
  if (outC->mem2 >= swan_lit_int32(99)) {
    outC->mem2 = swan_lit_int32(0);
  }
  else {
    outC->mem2 = outC->mem2 + swan_lit_int32(1);
  }
}

#ifndef SWAN_USER_DEFINED_INIT
void MotorSystem_init_PWM(outC_MotorSystem_PWM * restrict outC)
{
  outC->mem1 = swan_lit_float32(0.0);
  outC->mem2 = swan_lit_int32(0);
}
#endif /* SWAN_USER_DEFINED_INIT */


#ifndef SWAN_NO_EXTERN_CALL_TO_RESET
void MotorSystem_reset_PWM(outC_MotorSystem_PWM * restrict outC)
{
  outC->mem1 = swan_lit_float32(0.0);
  outC->mem2 = swan_lit_int32(0);
}
#endif /* SWAN_NO_EXTERN_CALL_TO_RESET */




/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** MotorSystem_PWM.c
*************************************************************$ */
