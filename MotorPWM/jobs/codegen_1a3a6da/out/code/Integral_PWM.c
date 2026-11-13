/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Command: swan_cg.exe config.json -student
*************************************************************$ */
#include "swan_sensors.h"
#include "swan_consts.h"
#include "Integral_PWM.h"

/* PWM::Integral */
void Integral_PWM(
  /* epsilon */swan_float32 epsilon,
  /* windup */swan_bool windup,
  outC_Integral_PWM * restrict outC)
{
  swan_float32 x1;

  if (windup) {
    x1 = swan_lit_float32(0.0);
  }
  else {
    x1 = T_CYCLE_PWM * epsilon;
  }
  outC->integral = outC->integral + x1;
}

#ifndef SWAN_USER_DEFINED_INIT
void Integral_init_PWM(outC_Integral_PWM * restrict outC)
{
  outC->integral = swan_lit_float32(0.0);
}
#endif /* SWAN_USER_DEFINED_INIT */


#ifndef SWAN_NO_EXTERN_CALL_TO_RESET
void Integral_reset_PWM(outC_Integral_PWM * restrict outC)
{
  outC->integral = swan_lit_float32(0.0);
}
#endif /* SWAN_NO_EXTERN_CALL_TO_RESET */




/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Integral_PWM.c
*************************************************************$ */
