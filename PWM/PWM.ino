// MotorSystem.ino
#include <Arduino_Alvik.h>

extern "C"
{
  #include "swan_config.h"
  #include "swan_types.h"
  #include "swan_sensors.h"
  #include "swan_consts.h"
  #include "MotorSystem_PWM.h"
}

swan_float32 kp_PWM = 0.8f;
swan_float32 ki_PWM = 0.1f;

Arduino_Alvik alvik;

outC_MotorSystem_PWM leftCtx;
outC_MotorSystem_PWM rightCtx;
swan_bool left_pwm_out  = false;
swan_bool right_pwm_out = false;

swan_float32 ref_speed = 0.5f;

void setup() {
  Serial.begin(115200);

  while (alvik.begin() != 0) {
    Serial.println("Alvik not ready, retrying...");
    delay(500);
  }

  MotorSystem_init_PWM(&leftCtx);
  MotorSystem_init_PWM(&rightCtx);
}

void loop() {
  float left_pct  = 0.0f;
  float right_pct = 0.0f;
  alvik.get_wheels_speed(left_pct, right_pct, PERCENTAGE);
  
  swan_float32 curr_left  = (swan_float32)(left_pct  / 100.0f);
  swan_float32 curr_right = (swan_float32)(right_pct / 100.0f);

  MotorSystem_PWM(ref_speed, curr_left,  &left_pwm_out,  &leftCtx);
  MotorSystem_PWM(ref_speed, curr_right, &right_pwm_out, &rightCtx);

  float drive_pct = ref_speed * 100.0f;
  alvik.left_wheel.set_speed( left_pwm_out  ? drive_pct : 0.0f, PERCENTAGE);
  alvik.right_wheel.set_speed(right_pwm_out ? drive_pct : 0.0f, PERCENTAGE);

  Serial.print("L_pwm:"); Serial.print(left_pwm_out);
  Serial.print("  R_pwm:"); Serial.print(right_pwm_out);
  Serial.print("  L_integ:"); Serial.print(leftCtx.mem1);
  Serial.print("  R_integ:"); Serial.println(rightCtx.mem1);

  // 50ms matches T_CYCLE_PWM = 0.05 in swan_consts.h
  delay(50);
}