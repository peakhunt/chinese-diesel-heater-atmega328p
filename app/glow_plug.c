#include "glow_plug.h"

void
glow_plug_init(glow_plug_t* gp, gpio_out_pin_t pin)
{
  gp->on        = false;

  soft_pwm_init(&gp->pwm, pin, GLOW_PLUG_PWM_FREQ, GLOW_PLUG_PWM_DUTY);
}

void
glow_plug_on(glow_plug_t* gp)
{
  if(gp->on == true)
  {
    return;
  }

  gp->on = true;
  soft_pwm_start(&gp->pwm);
}

void
glow_plug_off(glow_plug_t* gp)
{
  if(gp->on == false)
  {
    return;
  }

  gp->on = false;
  soft_pwm_stop(&gp->pwm);
}
