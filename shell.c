#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "app_common.h"
#include "shell.h"
#include "shell_if_uart.h"
#include "misc.h"
#include "pwm.h"
#include "adc_reader.h"
#include "gpio.h"
#include "event_dispatcher.h"
#include "event_list.h"

#include "heater.h"
#include "settings.h"

////////////////////////////////////////////////////////////////////////////////
//
// private definitions
//
////////////////////////////////////////////////////////////////////////////////

// avr specific
#define FSTR(s)   PSTR(s)
//#define shell_printf_P                  shell_printf
static void shell_printf_P(ShellIntf* intf, const char* fmt, ...) __attribute__((format(gnu_printf, 2, 3)));

#define SHELL_MAX_COLUMNS_PER_LINE      80
#define SHELL_COMMAND_MAX_ARGS          4

typedef void (*shell_command_handler)(ShellIntf* intf, int argc, const char** argv);

typedef struct
{
  const char*           command;
  shell_command_handler handler;
} ShellCommand;

////////////////////////////////////////////////////////////////////////////////
//
// private prototypes
//
////////////////////////////////////////////////////////////////////////////////
static void shell_command_help(ShellIntf* intf, int argc, const char** argv);
static void shell_command_version(ShellIntf* intf, int argc, const char** argv);
static void shell_command_uptime(ShellIntf* intf, int argc, const char** argv);

static void shell_command_pwm(ShellIntf* intf, int argc, const char** argv);
static void shell_command_adc(ShellIntf* intf, int argc, const char** argv);
static void shell_command_gpio_out(ShellIntf* intf, int argc, const char** argv);
static void shell_command_gpio_in(ShellIntf* intf, int argc, const char** argv);

static void shell_command_start(ShellIntf* intf, int argc, const char** argv);
static void shell_command_stop(ShellIntf* intf, int argc, const char** argv);
static void shell_command_status(ShellIntf* intf, int argc, const char** argv);

static void shell_command_glow(ShellIntf* intf, int argc, const char** argv);
static void shell_command_oil(ShellIntf* intf, int argc, const char** argv);
static void shell_command_fan(ShellIntf* intf, int argc, const char** argv);
static void shell_command_set_oil_pump_freq(ShellIntf* intf, int argc, const char** argv);
static void shell_command_set_fan_power(ShellIntf* intf, int argc, const char** argv);
static void shell_command_settings(ShellIntf* intf, int argc, const char** argv);
static void shell_command_mod(ShellIntf* intf, int argc, const char** argv);
static void shell_command_save(ShellIntf* intf, int argc, const char** argv);
static void shell_command_reset(ShellIntf* intf, int argc, const char** argv);
static void shell_command_step(ShellIntf* intf, int argc, const char** argv);
static void shell_command_set(ShellIntf* intf, int argc, const char** argv);

////////////////////////////////////////////////////////////////////////////////
//
// common utilities
//
////////////////////////////////////////////////////////////////////////////////
static inline int
fcompare(float a, float b)
{
  //
  // returns -1 when a < b
  //          1 when a > b
  //          0 when equal
  //
#define FLT_EPSILON 0.000001f

  float d = a - b;

  if (d >= -FLT_EPSILON && d <= FLT_EPSILON)
  {
    return 0;
  }

  if(d < -FLT_EPSILON)
  {
    // a is less than b
    return -1;
  }

  // a is bigger than b
  return 1;
}

static inline void
__float_to_int_1dec(float f, uint8_t* i, uint8_t* d)
{
  uint8_t   freq10x = (uint8_t)(f * 10);
  *i = (uint8_t)(freq10x / 10);
  *d = (uint8_t)(freq10x - (*i * 10));
}

////////////////////////////////////////////////////////////////////////////////
//
// private variables
//
////////////////////////////////////////////////////////////////////////////////
static char                   _print_buffer[SHELL_MAX_COLUMNS_PER_LINE + 1];

static LIST_HEAD(_shell_intf_list);

static const ShellCommand const _commands[] = 
{
  {
    "help",
    shell_command_help,
  },
  {
    "version",
    shell_command_version,
  },
  {
    "uptime",
    shell_command_uptime,
  },
  {
    "pwm",
    shell_command_pwm,
  },
  {
    "adc",
    shell_command_adc,
  },
  {
    "gpio_out",
    shell_command_gpio_out,
  },
  {
    "gpio_in",
    shell_command_gpio_in,
  },
  {
    "start",
    shell_command_start,
  },
  {
    "stop",
    shell_command_stop,
  },
  {
    "status",
    shell_command_status,
  },
  {
    "glow",
    shell_command_glow,
  },
  {
    "oil",
    shell_command_oil,
  },
  {
    "fan",
    shell_command_fan,
  },
  {
    "pump_freq",
    shell_command_set_oil_pump_freq,
  },
  {
    "fan_power",
    shell_command_set_fan_power,
  },
  {
    "settings",
    shell_command_settings,
  },
  {
    "mod",
    shell_command_mod,
  },
  {
    "save",
    shell_command_save,
  },
  {
    "reset",
    shell_command_reset,
  },
  {
    "step",
    shell_command_step,
  },
  {
    "set",
    shell_command_set,
  },
};

static const char* 
heater_state_desc[] = 
{
  "off",
  "glowing for start",
  "priming",
  "running",
  "glowing for stop",
  "cooling",
};

static const char*
on_off_str[] =
{
  "off",
  "on",
};

////////////////////////////////////////////////////////////////////////////////
//
// shell utilities
//
////////////////////////////////////////////////////////////////////////////////
static inline void
shell_prompt(ShellIntf* intf)
{
  shell_printf_P(intf, FSTR("\r\nHeater> "));
}

////////////////////////////////////////////////////////////////////////////////
//
// shell command handlers
//
////////////////////////////////////////////////////////////////////////////////

static void
shell_command_help(ShellIntf* intf, int argc, const char** argv)
{
  shell_printf_P(intf, FSTR("\r\n"));

  // I know.
  // to reduce sram usage
  //
  shell_printf_P(intf, FSTR("help                : show this command\r\n"));
  shell_printf_P(intf, FSTR("version             : show version\r\n"));
  shell_printf_P(intf, FSTR("uptime              : show uptime\r\n"));
  shell_printf_P(intf, FSTR("pwm                 : control pwm\r\n"));
  shell_printf_P(intf, FSTR("adc                 : show adc values\r\n"));
  shell_printf_P(intf, FSTR("gpio_out            : control gpio out\r\n"));
  shell_printf_P(intf, FSTR("gpio_in             : show gpio in state\r\n"));
  shell_printf_P(intf, FSTR("start               : start heater\r\n"));
  shell_printf_P(intf, FSTR("stop                : stop heater\r\n"));
  shell_printf_P(intf, FSTR("status              : show heater status\r\n"));
  shell_printf_P(intf, FSTR("glow                : control glow plug\r\n"));
  shell_printf_P(intf, FSTR("oil                 : control oil pump\r\n"));
  shell_printf_P(intf, FSTR("fan                 : control fan\r\n"));
  shell_printf_P(intf, FSTR("pump_freq           : set oil pump frequency\r\n"));
  shell_printf_P(intf, FSTR("fan_power           : set fan power\r\n"));
  shell_printf_P(intf, FSTR("settings            : show settings\r\n"));
  shell_printf_P(intf, FSTR("mod                 : change settings\r\n"));
  shell_printf_P(intf, FSTR("save                : save settings\r\n"));
  shell_printf_P(intf, FSTR("reset               : reset settings to default\r\n"));
  shell_printf_P(intf, FSTR("step                : modify step\r\n"));
  shell_printf_P(intf, FSTR("set                 : set current step\r\n"));
}

static void
shell_command_version(ShellIntf* intf, int argc, const char** argv)
{
  shell_printf_P(intf, FSTR("\r\n"));
  shell_printf_P(intf, FSTR("Heater V0.2a\r\n"));
}

static void
shell_command_uptime(ShellIntf* intf, int argc, const char** argv)
{
  shell_printf_P(intf, FSTR("\r\n"));
  shell_printf_P(intf, FSTR("%ld seconds\r\n"), __uptime);
}

static void
shell_command_pwm(ShellIntf* intf, int argc, const char** argv)
{
  pwm_channel_t chnl;
  uint8_t       pct;

  shell_printf_P(intf, FSTR("\r\n"));

  if(argc != 3)
  {
    shell_printf_P(intf, FSTR("invalid %s <chnl> <percent>\r\n"), argv[0]);
    return;
  }

  chnl = (pwm_channel_t)atoi(argv[1]);
  pct = atoi(argv[2]);

  if(chnl < 0 || chnl >= PWM_MAX_CHANNEL)
  {
    shell_printf_P(intf, FSTR("Invalid channel %d\r\n"), chnl);
    return;
  }

  if(pct < 0 || pct > 100)
  {
    shell_printf_P(intf, FSTR("Invalid percent %d\r\n"), pct);
    return;
  }

  pwm_control(chnl, pct);

  shell_printf_P(intf, FSTR("set pwm channel %d to %d percent\r\n"), chnl, pct);
}

static void
shell_command_adc(ShellIntf* intf, int argc, const char** argv)
{
  shell_printf_P(intf, FSTR("\r\n"));

  for(uint8_t i = 0; i < ADC_MAX_CHANNELS; i++)
  {
    shell_printf_P(intf, FSTR("ADC CH %d - %d\r\n"), i, adc_get(i));
  }
}

static void
shell_command_gpio_out(ShellIntf* intf, int argc, const char** argv)
{
  uint8_t pin, v;

  shell_printf_P(intf, FSTR("\r\n"));

  if(argc != 3)
  {
    shell_printf_P(intf, FSTR("invalid %s <pin> <value>\r\n"), argv[0]);
    return;
  }

  pin = atoi(argv[1]);
  v = atoi(argv[2]);
  v = v != 0 ? 1 : 0;

  if(pin >= GPIO_MAX_OUTPUT)
  {
    shell_printf_P(intf, FSTR("Invalid pin %d\r\n"), pin);
    return;
  }

  gpio_set((gpio_out_pin_t)pin, v);

  shell_printf_P(intf, FSTR("set gpio out %d to %d\r\n"), pin, v);
}

static void
shell_command_gpio_in(ShellIntf* intf, int argc, const char** argv)
{
  static const char* pin_states[] =
  {
    "low",
    "low to high",
    "high",
    "high to low",
  };

  shell_printf_P(intf, FSTR("\r\n"));

  for(uint8_t i = 0; i < GPIO_MAX_INPUT; i++)
  {
    shell_printf_P(intf, FSTR("gpio in %d - %d, %s\r\n"),
      i,
      gpio_get((gpio_in_pin_t)i),
      pin_states[gpio_get_state((gpio_in_pin_t)i)]
    );
  }
}

static void
shell_command_start(ShellIntf* intf, int argc, const char** argv)
{
  heater_start();
}

static void
shell_command_stop(ShellIntf* intf, int argc, const char** argv)
{
  heater_stop();
}

static void
shell_command_status(ShellIntf* intf, int argc, const char** argv)
{
  const heater_t* heater = heater_get();
  uint8_t   freq_int, freq_dec;
  static const char* motor_state_str[] = 
  {
    "not rotating",
    "starting",
    "rotating",
  };

  __float_to_int_1dec(heater->oil_pump.freq, &freq_int, &freq_dec);

  shell_printf_P(intf, FSTR("\r\n"));

  shell_printf_P(intf, FSTR("state : %s\r\n"), heater_state_desc[heater->state]);
  shell_printf_P(intf, FSTR("pump  : %s, freq %d.%d Hz\r\n"),
      on_off_str[heater->oil_pump.on],
      freq_int, freq_dec);
  shell_printf_P(intf, FSTR("glow  : %s\r\n"), on_off_str[heater->glow_plug.on]);
  shell_printf_P(intf, FSTR("fan   : %s, %s, power %d%%\r\n"),
      on_off_str[heater->fan.on],
      motor_state_str[heater->fan.motor_state],
      heater->fan.pwr);
  shell_printf_P(intf, FSTR("step  : %d\r\n"), heater->step);

  {
    int i_part, d_part;

    i_part = (int)heater->outlet_temp.temp;
    d_part = abs((heater->outlet_temp.temp - i_part) * 10);

    shell_printf_P(intf, FSTR("outlet : %d.%d C\r\n"),
        i_part, d_part);

    i_part = (int)heater->room_temp.temp;
    d_part = abs((heater->room_temp.temp - i_part) * 10);
    shell_printf_P(intf, FSTR("room   : %d.%d C\r\n"),
        i_part, d_part);
  }
}

static void
shell_command_glow(ShellIntf* intf, int argc, const char** argv)
{
  shell_printf_P(intf, FSTR("\r\n"));

  if(argc != 2)
  {
    shell_printf_P(intf, FSTR("invalid %s on|off\r\n"), argv[0]);
    return;
  }

  if(strcmp(argv[1], "on") == 0)
  {
    heater_glow_plug_on();
  }
  else if(strcmp(argv[1], "off") == 0)
  {
    heater_glow_plug_off();
  }
  else
  {
    shell_printf_P(intf, FSTR("invalid %s on|off\r\n"), argv[0]);
    return;
  }
  shell_printf_P(intf, FSTR("turned %s glow plug\r\n"), argv[1]);
}

static void
shell_command_oil(ShellIntf* intf, int argc, const char** argv)
{
  shell_printf_P(intf, FSTR("\r\n"));

  if(argc != 2)
  {
    shell_printf_P(intf, FSTR("invalid %s on|off\r\n"), argv[0]);
    return;
  }

  if(strcmp(argv[1], "on") == 0)
  {
    heater_oil_pump_on();
  }
  else if(strcmp(argv[1], "off") == 0)
  {
    heater_oil_pump_off();
  }
  else
  {
    shell_printf_P(intf, FSTR("invalid %s on|off\r\n"), argv[0]);
    return;
  }
  shell_printf_P(intf, FSTR("turned %s oil pump\r\n"), argv[1]);
}

static void
shell_command_fan(ShellIntf* intf, int argc, const char** argv)
{
  shell_printf_P(intf, FSTR("\r\n"));

  if(argc != 2)
  {
    shell_printf_P(intf, FSTR("invalid %s on|off\r\n"), argv[0]);
    return;
  }

  if(strcmp(argv[1], "on") == 0)
  {
    heater_fan_on();
  }
  else if(strcmp(argv[1], "off") == 0)
  {
    heater_fan_off();
  }
  else
  {
    shell_printf_P(intf, FSTR("invalid %s on|off\r\n"), argv[0]);
    return;
  }
  shell_printf_P(intf, FSTR("turned %s fan\r\n"), argv[1]);
}

static void
shell_command_set_oil_pump_freq(ShellIntf* intf, int argc, const char** argv)
{
  float freq;

  shell_printf_P(intf, FSTR("\r\n"));

  if(argc != 2)
  {
    shell_printf_P(intf, FSTR("invalid %s <freq>\r\n"), argv[0]);
    return;
  }

  freq = atof(argv[1]);

  if(fcompare(freq, OIL_PUMP_MIN_FREQ) < 0 || fcompare(freq, OIL_PUMP_MAX_FREQ) > 0)
  {
    shell_printf_P(intf, FSTR("Invalid frequency\r\n"));
    return;
  }

  heater_oil_pump_freq(freq);
}

static void
shell_command_set_fan_power(ShellIntf* intf, int argc, const char** argv)
{
  uint8_t pwr;

  shell_printf_P(intf, FSTR("\r\n"));

  if(argc != 2)
  {
    shell_printf_P(intf, FSTR("invalid %s <pwr>\r\n"), argv[0]);
    return;
  }

  pwr = atoi(argv[1]);

  if(pwr < 0 || pwr > 100)
  {
    shell_printf_P(intf, FSTR("invalid: should be between 0 and 100\r\n"));
    return;
  }

  heater_fan_power(pwr);
  shell_printf_P(intf, FSTR("set power to %d\r\n"), pwr);
}

static void
shell_command_settings(ShellIntf* intf, int argc, const char** argv)
{
  settings_t* s = settings_get();
  uint8_t   freq_int, freq_dec;

  shell_printf_P(intf, FSTR("\r\n"));

  shell_printf_P(intf, FSTR("0. glow plug on duration for start %ld sec\r\n"), s->glow_plug_on_duration_for_start / 1000);
  shell_printf_P(intf, FSTR("1. oil pump priming duratuin       %ld sec\r\n"), s->oil_pump_priming_duration / 1000);
  shell_printf_P(intf, FSTR("2. glow plug on duration for stop  %ld sec\r\n"), s->glow_plug_on_duration_for_stop / 1000);
  shell_printf_P(intf, FSTR("3. cooling down period             %ld sec\r\n"), s->cooling_down_period / 1000);
  shell_printf_P(intf, FSTR("4. start-up fan power              %d %%\r\n"), s->startup_fan_power);
  shell_printf_P(intf, FSTR("5. stop fan power                  %d %%\r\n"), s->stop_fan_power);
  shell_printf_P(intf, FSTR("6. glow plug PWM frequency         %d Hz\r\n"), s->glow_plug_pwm_freq);
  shell_printf_P(intf, FSTR("7. glow plug PWM duty              %d %%\r\n"), s->glow_plug_pwm_duty);

  __float_to_int_1dec(s->oil_pump_startup_freq, &freq_int, &freq_dec);
  shell_printf_P(intf, FSTR("8. oil pump startup frequency      %d.%d Hz\r\n"), freq_int, freq_dec);
  shell_printf_P(intf, FSTR("9. oil pump pulse length           %d ms\r\n"), s->oil_pump_pulse_length);

  shell_printf_P(intf, FSTR("\r\n"));
  for(uint8_t i = 0; i < MAX_OIL_PUMP_FAN_STEPS; i++)
  {
    __float_to_int_1dec(s->steps[i].pump_freq, &freq_int, &freq_dec);

    shell_printf_P(intf, FSTR("step %d, oil pump freq %d.%d Hz, Fan %d%%\r\n"), 
          i,
          freq_int, freq_dec,
          s->steps[i].fan_pwr);
  }
}

static void
shell_command_mod(ShellIntf* intf, int argc, const char** argv)
{
  uint8_t       num;
  uint32_t      iv;
  float         fv;
  settings_t*   s = settings_get();

  shell_printf_P(intf, FSTR("\r\n"));

  if(argc != 3)
  {
    shell_printf_P(intf, FSTR("invalid %s <setting number> <value>\r\n"), argv[0]);
    return;
  }

  num = atoi(argv[1]);
  if(num < 0 || num > 9)
  {
    shell_printf_P(intf, FSTR("invalid: setting number should be between 0 and 10\r\n"));
    return;
  }

  switch(num)
  {
  case 0: // glow plug on duration for start
    iv = atol(argv[2]);
    if(iv < 30 || iv > 150)
    {
      shell_printf_P(intf, FSTR("invalid: value should be between  30 and 150\r\n"));
      return;
    }
    s->glow_plug_on_duration_for_start = iv * 1000;
    break;

  case 1: // oil pump priming duration
    iv = atol(argv[2]);
    if(iv < 5 || iv > 60)
    {
      shell_printf_P(intf, FSTR("invalid: value should be between  5 and 60\r\n"));
      return;
    }
    s->oil_pump_priming_duration = iv * 1000;
    break;

  case 2: // glow plug on duration for stop
    iv = atol(argv[2]);
    if(iv < 20 || iv > 60)
    {
      shell_printf_P(intf, FSTR("invalid: value should be between  20 and 60\r\n"));
      return;
    }
    s->glow_plug_on_duration_for_start = iv * 1000;
    break;

  case 3: // cooling down period
    iv = atol(argv[2]);
    if(iv < 60 || iv > 180)
    {
      shell_printf_P(intf, FSTR("invalid: value should be between  60 and 180\r\n"));
      return;
    }
    s->cooling_down_period = iv * 1000;
    break;

  case 4: // start up fan power
    iv = atol(argv[2]);
    if(iv < 10 || iv > 180)
    {
      shell_printf_P(intf, FSTR("invalid: value should be between  10 and 180\r\n"));
      return;
    }
    s->startup_fan_power = iv;
    break;

  case 5: // stop fan power
    iv = atol(argv[2]);
    if(iv < 10 || iv > 180)
    {
      shell_printf_P(intf, FSTR("invalid: value should be between  10 and 180\r\n"));
      return;
    }
    s->stop_fan_power = iv;
    break;

  case 6: // glow plug PWM frequency
    iv = atol(argv[2]);
    if(iv < 3 || iv > 20)
    {
      shell_printf_P(intf, FSTR("invalid: value should be between 2 and 20\r\n"));
      return;
    }
    s->glow_plug_pwm_freq = iv;
    break;

  case 7: // glow plug PWM duty
    iv = atol(argv[2]);
    if(iv < 5 || iv > 95)
    {
      shell_printf_P(intf, FSTR("invalid: value should be between 5 and 95\r\n"));
      return;
    }
    s->glow_plug_pwm_duty = iv;
    break;

  case 8: // oil pump frequency
    fv = atof(argv[2]);
    if(fcompare(fv, OIL_PUMP_MIN_FREQ) < 0 || fcompare(fv, OIL_PUMP_MAX_FREQ) > 0)
    {
      shell_printf_P(intf, FSTR("invalid: value should be between 0.8 and 5.0\r\n"));
      return;
    }
    s->oil_pump_startup_freq = fv;
    break;

  case 9: // oil pump pulse length
    iv = atol(argv[2]);
    if(iv < 10 || iv > 100)
    {
      shell_printf_P(intf, FSTR("invalid: value should be between  10 and 100\r\n"));
      return;
    }
    s->oil_pump_pulse_length = iv;
    break;

  default:
    return;
  }

  shell_printf_P(intf, FSTR("done changing setting #%d. Be sure to save\r\n"), num);
}

static void
shell_command_save(ShellIntf* intf, int argc, const char** argv)
{
  shell_printf_P(intf, FSTR("\r\nsaving changes to EEPROM...\r\n"));
  settings_update();
  shell_printf_P(intf, FSTR("done saving changes to EEPROM...\r\n"));
}

static void
shell_command_reset(ShellIntf* intf, int argc, const char** argv)
{
  shell_printf_P(intf, FSTR("\r\nresettinging settings...\r\n"));
  settings_reset();
  shell_printf_P(intf, FSTR("done resetting settings...\r\n"));
}

static void
shell_command_step(ShellIntf* intf, int argc, const char** argv)
{
  float   freq;
  uint8_t pwr;
  uint8_t step;
  settings_t*   s = settings_get();

  shell_printf_P(intf, FSTR("\r\n"));

  if(argc != 4)
  {
    shell_printf_P(intf, FSTR("invalid %s <step number> <pump freq> <fan pwr>\r\n"), argv[0]);
    return;
  }

  step = atoi(argv[1]);
  if(step < 0 || step >= MAX_OIL_PUMP_FAN_STEPS)
  {
    shell_printf_P(intf, FSTR("invalid step number\r\n"));
    return;
  }

  freq = atof(argv[2]);
  if(fcompare(freq, OIL_PUMP_MIN_FREQ) < 0 || fcompare(freq, OIL_PUMP_MAX_FREQ) > 0)
  {
    shell_printf_P(intf, FSTR("Invalid frequency\r\n"));
    return;
  }

  pwr = atoi(argv[3]);
  if(pwr < 0 || pwr > 100)
  {
    shell_printf_P(intf, FSTR("Invalid power\r\n"));
    return;
  }

  s->steps[step].pump_freq = freq;
  s->steps[step].fan_pwr = pwr;

  shell_printf_P(intf, FSTR("changed settings of step #%d\r\n"), step);
}

static void
shell_command_set(ShellIntf* intf, int argc, const char** argv)
{
  uint8_t step;

  shell_printf_P(intf, FSTR("\r\n"));

  if(argc != 2)
  {
    shell_printf_P(intf, FSTR("invalid %s <step number>\r\n"), argv[0]);
    return;
  }

  step = atoi(argv[1]);
  if(step < 0 || step >= MAX_OIL_PUMP_FAN_STEPS)
  {
    shell_printf_P(intf, FSTR("invalid step number\r\n"));
    return;
  }

  heater_set_step(step);

  shell_printf_P(intf, FSTR("changed current step to %d\r\n"), step);
}

////////////////////////////////////////////////////////////////////////////////
//
// shell core
//
////////////////////////////////////////////////////////////////////////////////
static void
shell_execute_command(ShellIntf* intf, char* cmd)
{
  static const char*    argv[SHELL_COMMAND_MAX_ARGS];
  int                   argc = 0;
  size_t                i;
  char                  *s, *t;

  while((s = strtok_r(argc  == 0 ? cmd : NULL, " \t", &t)) != NULL)
  {
    if(argc >= SHELL_COMMAND_MAX_ARGS)
    {
      shell_printf_P(intf, FSTR("\r\nError: too many arguments\r\n"));
      return;
    }
    argv[argc++] = s;
  }

  if(argc == 0)
  {
    return;
  }

  for(i = 0; i < sizeof(_commands)/sizeof(ShellCommand); i++)
  {
    if(strcmp(_commands[i].command, argv[0]) == 0)
    {
      shell_printf_P(intf, FSTR("\r\nExecuting %s\r\n"), argv[0]);
      _commands[i].handler(intf, argc, argv);
      return;
    }
  }
  shell_printf_P(intf, FSTR("\r\nUnknown Command: "));
  shell_printf_P(intf, FSTR("%s"), argv[0]);
  shell_printf_P(intf, FSTR("\r\n"));
}


void
shell_printf(ShellIntf* intf, const char* fmt, ...)
{
  va_list   args;
  int       len;

  va_start(args, fmt);
  len = vsnprintf(_print_buffer, SHELL_MAX_COLUMNS_PER_LINE, fmt, args);
  va_end(args);

  if(len > CLI_TX_BUFFER_LENGTH)
  {
    return;
  }

  do
  {
  } while(intf->put_tx_data(intf, (uint8_t*)_print_buffer, len) == false);
}

void
shell_printf_P(ShellIntf* intf, const char* fmt, ...)
{
  va_list   args;
  int       len;

  va_start(args, fmt);
  len = vsnprintf_P(_print_buffer, SHELL_MAX_COLUMNS_PER_LINE, fmt, args);
  va_end(args);

  if(len > CLI_TX_BUFFER_LENGTH)
  {
    return;
  }

  do
  {
  } while(intf->put_tx_data(intf, (uint8_t*)_print_buffer, len) == false);
}

static void
shell_handle_heater_event(uint32_t event)
{
#if 0 // still not decided whether to go with this or not
  const heater_t* heater = heater_get();
  ShellIntf* intf;

  list_for_each_entry(intf, &_shell_intf_list, lh)
  {
    shell_printf(intf, "\r\n<<< Heater State Changed to %s\r\n", heater_state_desc[heater->state]);
    shell_prompt(intf);
  }
#endif
}

////////////////////////////////////////////////////////////////////////////////
//
// public interface
//
////////////////////////////////////////////////////////////////////////////////
void
shell_init(void)
{
  shell_if_uart_init();

  event_register_handler(shell_handle_heater_event, DISPATCH_EVENT_HEATER_STATE);
}

void
shell_start(void)
{
  ShellIntf* intf;

  list_for_each_entry(intf, &_shell_intf_list, lh)
  {
    shell_printf_P(intf, FSTR("\r\n**** Welcome ****\r\n"));
    shell_prompt(intf);
  }
}


void
shell_if_register(ShellIntf* intf)
{
  list_add_tail(&intf->lh, &_shell_intf_list);
}

void
shell_handle_rx(ShellIntf* intf)
{
  uint8_t   b;

  while(1)
  {
    if(intf->get_rx_data(intf, &b) == false)
    {
      return;
    }

    if(b != '\r' && intf->cmd_buffer_ndx < SHELL_MAX_COMMAND_LEN)
    {
      if(b == '\b' || b == 0x7f)
      {
        if(intf->cmd_buffer_ndx > 0)
        {
          shell_printf(intf, "%c%c%c", b, 0x20, b);
          intf->cmd_buffer_ndx--;
        }
      }
      else
      {
        shell_printf(intf, "%c", b);
        intf->cmd_buffer[intf->cmd_buffer_ndx++] = b;
      }
    }
    else if(b == '\r')
    {
      intf->cmd_buffer[intf->cmd_buffer_ndx++] = '\0';

      shell_execute_command(intf, (char*)intf->cmd_buffer);

      intf->cmd_buffer_ndx = 0;
      shell_prompt(intf);
    }
  }
}

struct list_head*
shell_get_intf_list(void)
{
  return &_shell_intf_list;
}
