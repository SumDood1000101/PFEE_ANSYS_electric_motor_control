/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Command: swan_cg.exe config.json -student
*************************************************************$ */
#ifndef SWAN_Integral_PWM_H_
#define SWAN_Integral_PWM_H_

#include "swan_types.h"

typedef struct Ctx_Integral_PWM {
  swan_float32 /* integral */ integral;
} outC_Integral_PWM;

/* PWM::Integral */
extern void Integral_PWM(
  /* epsilon */swan_float32 epsilon,
  /* windup */swan_bool windup,
  outC_Integral_PWM * restrict outC);

#ifndef SWAN_NO_EXTERN_CALL_TO_RESET
extern void Integral_reset_PWM(outC_Integral_PWM * restrict outC);
#endif /* SWAN_NO_EXTERN_CALL_TO_RESET */

#ifndef SWAN_USER_DEFINED_INIT
extern void Integral_init_PWM(outC_Integral_PWM * restrict outC);
#endif /* SWAN_USER_DEFINED_INIT */



#endif /* SWAN_Integral_PWM_H_ */
/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Integral_PWM.h
*************************************************************$ */
