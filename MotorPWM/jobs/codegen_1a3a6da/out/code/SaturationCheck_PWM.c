/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Command: swan_cg.exe config.json -student
*************************************************************$ */
#include "swan_sensors.h"
#include "swan_consts.h"
#include "SaturationCheck_PWM.h"

/* PWM::SaturationCheck */
swan_bool SaturationCheck_PWM(
  /* out */swan_float32 out,
  /* out_clamped */swan_float32 out_clamped,
  /* epsilon */swan_float32 epsilon,
  /* max */swan_float32 max,
  /* min */swan_float32 min)
{
  /* isSat */
  swan_bool isSat;

  isSat = (out > max || out < min) && (out - out_clamped) * epsilon >
    swan_lit_float32(0.0);
  return isSat;
}



/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** SaturationCheck_PWM.c
*************************************************************$ */
