/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Command: swan_cg.exe config.json -student
*************************************************************$ */
#include "swan_sensors.h"
#include "swan_consts.h"
#include "Clamp_Utils.h"
#include "SaturationCheck_PWM.h"
#include "PIController_PWM.h"

/* PWM::PIController */
void PIController_PWM(
  /* ref */swan_float32 ref,
  /* measured */swan_float32 measured,
  /* kp */swan_float32 kp,
  /* ki */swan_float32 ki,
  /* out_min */swan_float32 out_min,
  /* out_max */swan_float32 out_max,
  /* out */swan_float32 * restrict out,
  outC_PIController_PWM * restrict outC)
{
  swan_float32 x1;
  swan_float32 x2;

  x2 = ref - measured;
  /* #13: */ Integral_PWM(x2, outC->mem1, &outC->Ctx_Integral1);
  x1 = x2 * kp + outC->Ctx_Integral1.integral * ki;
  *out = /* #20: */ Clamp_Utils(x1, out_max, out_min);
  outC->mem1 = /* #22: */ SaturationCheck_PWM(x1, *out, x2, out_max, out_min);
}

#ifndef SWAN_USER_DEFINED_INIT
void PIController_init_PWM(outC_PIController_PWM * restrict outC)
{
  outC->mem1 = swan_false;
  /* #13: */ Integral_init_PWM(&outC->Ctx_Integral1);
}
#endif /* SWAN_USER_DEFINED_INIT */


#ifndef SWAN_NO_EXTERN_CALL_TO_RESET
void PIController_reset_PWM(outC_PIController_PWM * restrict outC)
{
  outC->mem1 = swan_false;
  /* #13: */ Integral_reset_PWM(&outC->Ctx_Integral1);
}
#endif /* SWAN_NO_EXTERN_CALL_TO_RESET */




/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** PIController_PWM.c
*************************************************************$ */
