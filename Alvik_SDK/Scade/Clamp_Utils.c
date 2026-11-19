/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Command: swan_cg.exe config.json -student
*************************************************************$ */
#include "swan_sensors.h"
#include "swan_consts.h"
#include "Clamp_Utils.h"

/* Utils::Clamp */
swan_float32 Clamp_Utils(
  /* val */swan_float32 val,
  /* max */swan_float32 max,
  /* min */swan_float32 min)
{
  /* out */
  swan_float32 out;

  if (val > max) {
    out = max;
  }
  else if (val < min) {
    out = min;
  }
  else {
    out = val;
  }
  return out;
}



/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Clamp_Utils.c
*************************************************************$ */
