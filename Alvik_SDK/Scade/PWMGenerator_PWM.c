/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Command: swan_cg.exe config.json -student
*************************************************************$ */
#include "swan_sensors.h"
#include "swan_consts.h"
#include "PWMGenerator_PWM.h"

/* PWM::PWMGenerator */
void PWMGenerator_PWM(
  /* period */swan_int32 period,
  /* duty_cycle */swan_float32 duty_cycle,
  /* pwm_out */swan_bool * restrict pwm_out,
  outC_PWMGenerator_PWM * restrict outC)
{
  *pwm_out = outC->mem1 < (swan_int32) (duty_cycle * (swan_float32) period);
  if (outC->mem1 >= period - swan_lit_int32(1)) {
    outC->mem1 = swan_lit_int32(0);
  }
  else {
    outC->mem1 = outC->mem1 + swan_lit_int32(1);
  }
}

#ifndef SWAN_USER_DEFINED_INIT
void PWMGenerator_init_PWM(outC_PWMGenerator_PWM * restrict outC)
{
  outC->mem1 = swan_lit_int32(0);
}
#endif /* SWAN_USER_DEFINED_INIT */


#ifndef SWAN_NO_EXTERN_CALL_TO_RESET
void PWMGenerator_reset_PWM(outC_PWMGenerator_PWM * restrict outC)
{
  outC->mem1 = swan_lit_int32(0);
}
#endif /* SWAN_NO_EXTERN_CALL_TO_RESET */




/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** PWMGenerator_PWM.c
*************************************************************$ */
