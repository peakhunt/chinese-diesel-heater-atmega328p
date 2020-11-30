#include "heater.h"
#include "gpio.h"
#include "pwm.h"
#include "mainloop_timer.h"
#include "settings.h"

////////////////////////////////////////////////////////////////////////////////
//
// internals
//
////////////////////////////////////////////////////////////////////////////////
typedef enum
{
  heater_event_start_request = 0,
  heater_event_stop_request,
  heater_event_heating_glow_plug_complete_for_start,
  heater_event_oil_pump_priming_complete,
  heater_event_heating_glow_plug_complete_for_stop,
  heater_event_cooling_down_complete,
} heater_event_t;

static void heater_event(heater_event_t ev);

static heater_t         _heater;
static SoftTimerElem    _tmr;

////////////////////////////////////////////////////////////////////////////////
//
// heater state machine
//
////////////////////////////////////////////////////////////////////////////////
static void
heater_change_state(heater_state_t ns)
{
  _heater.state = ns;
}

static void
heater_timer_callback(SoftTimerElem* te)
{
  switch(_heater.state)
  {
  case heater_state_heating_glow_plug_for_start:
    heater_event(heater_event_heating_glow_plug_complete_for_start);
    break;

  case heater_state_oil_pump_priming:
    heater_event(heater_event_oil_pump_priming_complete);
    break;

  case heater_state_heating_glow_plug_for_stop:
    heater_event(heater_event_heating_glow_plug_complete_for_stop);
    break;

  case heater_state_cooling_down:
    heater_event(heater_event_cooling_down_complete);
    break;

  default:
    break;
  }
}

static void
handle_heater_event_state_off(heater_event_t ev)
{
  switch(ev)
  {
  case heater_event_start_request:
    fan_start(&_heater.fan);
    fan_set_power(&_heater.fan, HEATER_STARTUP_FAN_POWER);
    glow_plug_on(&_heater.glow_plug);

    mainloop_timer_schedule(&_tmr, HEATER_HEATING_GLOW_PLUG_BEFORE_OIL_PUMP_PRIMING);

    heater_change_state(heater_state_heating_glow_plug_for_start);
    break;

  default:
    break;
  }
}

static void
handle_heater_event_state_heating_glow_plug_for_start(heater_event_t ev)
{
  switch(ev)
  {
  case heater_event_heating_glow_plug_complete_for_start:
    oil_pump_on(&_heater.oil_pump);

    mainloop_timer_schedule(&_tmr, HEATER_OIL_PUMP_PRIMING_BEFORE_GLOW_PLUG_TURN_OFF);

    heater_change_state(heater_state_oil_pump_priming);
    break;

  default:
    break;
  }
}

static void
handle_heater_event_state_oil_pump_priming(heater_event_t ev)
{
  switch(ev)
  {
  case heater_event_oil_pump_priming_complete:
    glow_plug_off(&_heater.glow_plug);
    fan_set_power(&_heater.fan, FAN_DEFAULT_POWER);

    heater_change_state(heater_state_running);
    break;

  default:
    break;
  }
}

static void
handle_heater_event_state_running(heater_event_t ev)
{
  switch(ev)
  {
  case heater_event_stop_request:
    fan_set_power(&_heater.fan, HEATER_STOP_FAN_POWER);
    oil_pump_off(&_heater.oil_pump);
    glow_plug_on(&_heater.glow_plug);

    mainloop_timer_schedule(&_tmr, HEATER_HEATING_GLOW_PLUG_BEFORE_COOLING_DOWN);

    heater_change_state(heater_state_heating_glow_plug_for_stop);
    break;

  default:
    break;
  }
}

static void
handle_heater_event_state_heating_glow_plug_for_stop(heater_event_t ev)
{
  switch(ev)
  {
  case heater_event_heating_glow_plug_complete_for_stop:
    glow_plug_off(&_heater.glow_plug);

    mainloop_timer_schedule(&_tmr, HEATER_COOLING_DOWN_AFTER_GLOW_PLUG_TURN_OFF);

    heater_change_state(heater_state_cooling_down);
    break;

  default:
    break;
  }
}

static void
handle_heater_event_state_cooling_down(heater_event_t ev)
{
  switch(ev)
  {
  case heater_event_cooling_down_complete:
    fan_stop(&_heater.fan);

    heater_change_state(heater_state_off);
    break;

  default:
    break;
  }
}

static void
heater_event(heater_event_t ev)
{
  switch(_heater.state)
  {
  case heater_state_off:
    handle_heater_event_state_off(ev);
    break;

  case heater_state_heating_glow_plug_for_start:
    handle_heater_event_state_heating_glow_plug_for_start(ev);
    break;

  case heater_state_oil_pump_priming:
    handle_heater_event_state_oil_pump_priming(ev);
    break;

  case heater_state_running:
    handle_heater_event_state_running(ev);
    break;

  case heater_state_heating_glow_plug_for_stop:
    handle_heater_event_state_heating_glow_plug_for_stop(ev);
    break;

  case heater_state_cooling_down:
    handle_heater_event_state_cooling_down(ev);
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// heater state machine
//
////////////////////////////////////////////////////////////////////////////////
void
heater_init(void)
{
  settings_init();

  _heater.state = heater_state_off;

  glow_plug_init(&_heater.glow_plug, gpio_out_pin_pd2);
  oil_pump_init(&_heater.oil_pump, gpio_out_pin_pd3);
  fan_init(&_heater.fan, pwm_channel_0);

  soft_timer_init_elem(&_tmr);
  _tmr.cb = heater_timer_callback;
}

void
heater_start(void)
{
  heater_event(heater_event_start_request);
}

void
heater_stop(void)
{
  heater_event(heater_event_stop_request);
}

const heater_t*
heater_get(void)
{
  return &_heater;
}

void
heater_glow_plug_on(void)
{
  glow_plug_on(&_heater.glow_plug);
}

void
heater_glow_plug_off(void)
{
  glow_plug_off(&_heater.glow_plug);
}

void
heater_oil_pump_on(void)
{
  oil_pump_on(&_heater.oil_pump);
}

void
heater_oil_pump_off(void)
{
  oil_pump_off(&_heater.oil_pump);
}

void
heater_oil_pump_freq(float freq)
{
  oil_pump_change_freq(&_heater.oil_pump, freq);
}

void
heater_fan_on(void)
{
  fan_start(&_heater.fan);
}

void
heater_fan_off(void)
{
  fan_stop(&_heater.fan);
}

void
heater_fan_power(uint8_t pwr)
{
  fan_set_power(&_heater.fan, pwr);
}