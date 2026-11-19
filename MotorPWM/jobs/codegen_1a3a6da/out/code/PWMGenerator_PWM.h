/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Command: swan_cg.exe config.json -student
*************************************************************$ */
#ifndef SWAN_PWMGenerator_PWM_H_
#define SWAN_PWMGenerator_PWM_H_

#include "swan_types.h"

typedef struct Ctx_PWMGenerator_PWM { swan_int32 mem1; } outC_PWMGenerator_PWM;

/* PWM::PWMGenerator */
extern void PWMGenerator_PWM(
  /* period */swan_int32 period,
  /* duty_cycle */swan_float32 duty_cycle,
  /* pwm_out */swan_bool * restrict pwm_out,
  outC_PWMGenerator_PWM * restrict outC);

#ifndef SWAN_NO_EXTERN_CALL_TO_RESET
extern void PWMGenerator_reset_PWM(outC_PWMGenerator_PWM * restrict outC);
#endif /* SWAN_NO_EXTERN_CALL_TO_RESET */

#ifndef SWAN_USER_DEFINED_INIT
extern void PWMGenerator_init_PWM(outC_PWMGenerator_PWM * restrict outC);
#endif /* SWAN_USER_DEFINED_INIT */



#endif /* SWAN_PWMGenerator_PWM_H_ */
/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** PWMGenerator_PWM.h
*************************************************************$ */
