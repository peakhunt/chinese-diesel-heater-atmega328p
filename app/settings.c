#include <avr/eeprom.h>
#include "settings.h"
#include "heater.h"
#include "glow_plug.h"
#include "oil_pump.h"

#define SETTINGS_MAGIC          0x1122334f
#define SETTINGS_EEPROM_ADDRESS 128

static settings_t   _settings;

void
settings_init(void)
{
  eeprom_read_block((void*)&_settings, (const void*)SETTINGS_EEPROM_ADDRESS, sizeof(settings_t));

  if(_settings.magic != SETTINGS_MAGIC)
  {
    // invalid settings
    // reset everything to default
    settings_reset();
  }
}

void
settings_reset(void)
{
  _settings.magic                             = SETTINGS_MAGIC;
  _settings.glow_plug_on_duration_for_start   = HEATER_HEATING_GLOW_PLUG_BEFORE_OIL_PUMP_PRIMING;
  _settings.oil_pump_priming_duration         = HEATER_OIL_PUMP_PRIMING_BEFORE_GLOW_PLUG_TURN_OFF;
  _settings.glow_plug_on_duration_for_stop    = HEATER_HEATING_GLOW_PLUG_BEFORE_COOLING_DOWN;
  _settings.cooling_down_period               = HEATER_COOLING_DOWN_AFTER_GLOW_PLUG_TURN_OFF;

  _settings.startup_fan_power                 = HEATER_STARTUP_FAN_POWER;
  _settings.stop_fan_power                    = HEATER_STOP_FAN_POWER;

  _settings.glow_plug_pwm_freq                = GLOW_PLUG_PWM_FREQ;
  _settings.glow_plug_pwm_duty                = GLOW_PLUG_PWM_DUTY;

  _settings.oil_pump_startup_freq             = OIL_PUMP_DEF_FREQ;
  _settings.oil_pump_pulse_length             = OIL_PUMP_PULSE_LENGTH;

  for(uint8_t i = 0; i < MAX_OIL_PUMP_FAN_STEPS; i++)
  {
    _settings.steps[i].pump_freq = 1.0f + 0.4f * i;
    _settings.steps[i].fan_pwr = 30 + 10 * i;
  }

  settings_update();
}

void
settings_update(void)
{
  eeprom_update_block((void*)&_settings, (void*)SETTINGS_EEPROM_ADDRESS, sizeof(settings_t));
}

settings_t*
settings_get(void)
{
  return &_settings;
}
