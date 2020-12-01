#ifndef __HEATER_DEF_H__
#define __HEATER_DEF_H__

#include "glow_plug.h"
#include "oil_pump.h"
#include "fan.h"
#include "settings.h"

#define HEATER_HEATING_GLOW_PLUG_BEFORE_OIL_PUMP_PRIMING        90000     // 90 second
#define HEATER_OIL_PUMP_PRIMING_BEFORE_GLOW_PLUG_TURN_OFF       30000     // 30 second
#define HEATER_HEATING_GLOW_PLUG_BEFORE_COOLING_DOWN            30000     // 30 second
#define HEATER_COOLING_DOWN_AFTER_GLOW_PLUG_TURN_OFF            90000     // 90 second

#define HEATER_STARTUP_FAN_POWER                                65        // 65%
#define HEATER_STOP_FAN_POWER                                   65        // 65%

typedef enum
{
  heater_state_off,
  heater_state_heating_glow_plug_for_start,
  heater_state_oil_pump_priming,
  heater_state_running,
  heater_state_heating_glow_plug_for_stop,
  heater_state_cooling_down,
} heater_state_t;

typedef struct
{
  heater_state_t    state;
  glow_plug_t       glow_plug;
  oil_pump_t        oil_pump;
  fan_t             fan;
  uint8_t           step;
} heater_t;

extern void heater_init(void);
extern void heater_start(void);
extern void heater_stop(void);

extern void heater_glow_plug_on(void);
extern void heater_glow_plug_off(void);
extern void heater_oil_pump_on(void);
extern void heater_oil_pump_off(void);
extern void heater_oil_pump_freq(float freq);
extern void heater_fan_on(void);
extern void heater_fan_off(void);
extern void heater_fan_power(uint8_t pwr);
extern void heater_set_step(uint8_t step);

extern const heater_t* heater_get(void);

#endif /* !__HEATER_DEF_H__ */
