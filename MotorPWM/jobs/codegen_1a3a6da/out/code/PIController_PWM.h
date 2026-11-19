/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Command: swan_cg.exe config.json -student
*************************************************************$ */
#ifndef SWAN_PIController_PWM_H_
#define SWAN_PIController_PWM_H_

#include "Integral_PWM.h"

typedef struct Ctx_PIController_PWM {
  swan_bool mem1;
  outC_Integral_PWM /* #13: */ Ctx_Integral1;
} outC_PIController_PWM;

/* PWM::PIController */
extern void PIController_PWM(
  /* ref */swan_float32 ref,
  /* measured */swan_float32 measured,
  /* kp */swan_float32 kp,
  /* ki */swan_float32 ki,
  /* out_min */swan_float32 out_min,
  /* out_max */swan_float32 out_max,
  /* out */swan_float32 * restrict out,
  outC_PIController_PWM * restrict outC);

#ifndef SWAN_NO_EXTERN_CALL_TO_RESET
extern void PIController_reset_PWM(outC_PIController_PWM * restrict outC);
#endif /* SWAN_NO_EXTERN_CALL_TO_RESET */

#ifndef SWAN_USER_DEFINED_INIT
extern void PIController_init_PWM(outC_PIController_PWM * restrict outC);
#endif /* SWAN_USER_DEFINED_INIT */



#endif /* SWAN_PIController_PWM_H_ */
/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** PIController_PWM.h
*************************************************************$ */
