#include "fan.h"

void
fan_init(fan_t* fan, pwm_channel_t chnl)
{
  fan->on = false;
  fan->pwm_chnl = chnl;
  fan->pwr = FAN_DEFAULT_POWER;

  pwm_control(fan->pwm_chnl, 0);
}

void
fan_start(fan_t* fan)
{
  fan->on = true;
  pwm_control(fan->pwm_chnl, fan->pwr);
}

void
fan_stop(fan_t* fan)
{
  fan->on = false;
  pwm_control(fan->pwm_chnl, 0);
}

void
fan_set_power(fan_t* fan, uint8_t pwr)
{
  fan->pwr = pwr;
  if(fan->on)
  {
    pwm_control(fan->pwm_chnl, fan->pwr);
  }
}
