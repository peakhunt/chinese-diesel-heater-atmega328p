#ifndef __SETTINGS_DEF_H__
#define __SETTINGS_DEF_H__

#include "app_common.h"

typedef struct
{
  uint32_t        magic;

  uint32_t        glow_plug_on_duration_for_start;
  uint32_t        oil_pump_priming_duration;
  uint32_t        glow_plug_on_duration_for_stop;
  uint32_t        cooling_down_period;

  uint8_t         startup_fan_power;
  uint8_t         stop_fan_power;

  uint8_t         glow_plug_pwm_freq;
  uint8_t         glow_plug_pwm_duty;

  float           oil_pump_freq;
  uint8_t         oil_pump_pulse_length;

  uint8_t         fan_default_power;
} settings_t;

extern void settings_init(void);
extern void settings_reset(void);
extern void settings_update(void);
extern settings_t* settings_get(void);

#endif /* !__SETTINGS_DEF_H__ */
