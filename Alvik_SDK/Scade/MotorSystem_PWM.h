/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Command: swan_cg.exe config.json -student
*************************************************************$ */
#ifndef SWAN_MotorSystem_PWM_H_
#define SWAN_MotorSystem_PWM_H_

#include "swan_types.h"

typedef struct Ctx_MotorSystem_PWM {
  swan_float32 mem1;
  swan_int32 mem2;
} outC_MotorSystem_PWM;

/* PWM::MotorSystem */
extern void MotorSystem_PWM(
  /* ref_speed */swan_float32 ref_speed,
  /* curr_speed */swan_float32 curr_speed,
  /* pwm_out */swan_bool * restrict pwm_out,
  outC_MotorSystem_PWM * restrict outC);

#ifndef SWAN_NO_EXTERN_CALL_TO_RESET
extern void MotorSystem_reset_PWM(outC_MotorSystem_PWM * restrict outC);
#endif /* SWAN_NO_EXTERN_CALL_TO_RESET */

#ifndef SWAN_USER_DEFINED_INIT
extern void MotorSystem_init_PWM(outC_MotorSystem_PWM * restrict outC);
#endif /* SWAN_USER_DEFINED_INIT */



#endif /* SWAN_MotorSystem_PWM_H_ */
/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** MotorSystem_PWM.h
*************************************************************$ */
