#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

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

#define SHELL_MAX_COLUMNS_PER_LINE      80
#define SHELL_COMMAND_MAX_ARGS          4

#define VERSION       "Heater V0.2a"

typedef void (*shell_command_handler)(ShellIntf* intf, int argc, const char** argv);

typedef struct
{
  const char*           command;
  const char*           description;
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


////////////////////////////////////////////////////////////////////////////////
//
// private variables
//
////////////////////////////////////////////////////////////////////////////////
const uint8_t                 _welcome[] = "\r\n**** Welcome ****\r\n";
const uint8_t                 _prompt[]  = "\r\nHeater> ";

static char                   _print_buffer[SHELL_MAX_COLUMNS_PER_LINE + 1];

static LIST_HEAD(_shell_intf_list);

static const ShellCommand     _commands[] = 
{
  {
    "help",
    "show this command",
    shell_command_help,
  },
  {
    "version",
    "show version",
    shell_command_version,
  },
  {
    "uptime",
    "show uptime",
    shell_command_uptime,
  },
  {
    "pwm",
    "control pwm",
    shell_command_pwm,
  },
  {
    "adc",
    "show adc values",
    shell_command_adc,
  },
  {
    "gpio_out",
    "control gpio out",
    shell_command_gpio_out,
  },
  {
    "gpio_in",
    "show gpio in state",
    shell_command_gpio_in,
  },
  {
    "start",
    "start heater",
    shell_command_start,
  },
  {
    "stop",
    "stop heater",
    shell_command_stop,
  },
  {
    "status",
    "show heater status",
    shell_command_status,
  },
  {
    "glow",
    "control glow plug",
    shell_command_glow,
  },
  {
    "oil",
    "control oil pump",
    shell_command_oil,
  },
  {
    "fan",
    "control fan",
    shell_command_fan,
  },
  {
    "pump_freq",
    "set oil pump frequency",
    shell_command_set_oil_pump_freq,
  },
  {
    "fan_power",
    "set fan power",
    shell_command_set_fan_power,
  },
  {
    "settings",
    "show settings",
    shell_command_settings,
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

////////////////////////////////////////////////////////////////////////////////
//
// shell utilities
//
////////////////////////////////////////////////////////////////////////////////
static inline void
shell_prompt(ShellIntf* intf)
{
  do
  {
  } while(intf->put_tx_data(intf, (uint8_t*)_prompt, sizeof(_prompt) -1) == false);
}

////////////////////////////////////////////////////////////////////////////////
//
// shell command handlers
//
////////////////////////////////////////////////////////////////////////////////
static inline void
__float_to_int_1dec(float f, uint8_t* i, uint8_t* d)
{
  uint8_t   freq10x = (uint8_t)(f * 10);
  *i = (uint8_t)(freq10x / 10);
  *d = (uint8_t)(freq10x - (*i * 10));
}

static void
shell_command_help(ShellIntf* intf, int argc, const char** argv)
{
  size_t i;

  shell_printf(intf, "\r\n");

  for(i = 0; i < sizeof(_commands)/sizeof(ShellCommand); i++)
  {
    shell_printf(intf, "%-20s: ", _commands[i].command);
    shell_printf(intf, "%s\r\n", _commands[i].description);
  }
}

static void
shell_command_version(ShellIntf* intf, int argc, const char** argv)
{
  shell_printf(intf, "\r\n");
  shell_printf(intf, "%s\r\n", VERSION);
}

static void
shell_command_uptime(ShellIntf* intf, int argc, const char** argv)
{
  shell_printf(intf, "\r\n");
  shell_printf(intf, "%ld seconds\r\n", __uptime);
}

static void
shell_command_pwm(ShellIntf* intf, int argc, const char** argv)
{
  pwm_channel_t chnl;
  uint8_t       pct;

  shell_printf(intf, "\r\n");

  if(argc != 3)
  {
    shell_printf(intf, "invalid %s <chnl> <percent>\r\n", argv[0]);
    return;
  }

  chnl = (pwm_channel_t)atoi(argv[1]);
  pct = atoi(argv[2]);

  if(chnl < 0 || chnl >= PWM_MAX_CHANNEL)
  {
    shell_printf(intf, "Invalid channel %d\r\n", chnl);
    return;
  }

  if(pct < 0 || pct > 100)
  {
    shell_printf(intf, "Invalid percent %d\r\n", pct);
    return;
  }

  pwm_control(chnl, pct);

  shell_printf(intf, "set pwm channel %d to %d percent\r\n", chnl, pct);
}

static void
shell_command_adc(ShellIntf* intf, int argc, const char** argv)
{
  shell_printf(intf, "\r\n");

  for(uint8_t i = 0; i < ADC_MAX_CHANNELS; i++)
  {
    shell_printf(intf, "ADC CH %d - %d\r\n", i, adc_get(i));
  }
}

static void
shell_command_gpio_out(ShellIntf* intf, int argc, const char** argv)
{
  uint8_t pin, v;

  shell_printf(intf, "\r\n");

  if(argc != 3)
  {
    shell_printf(intf, "invalid %s <pin> <value>\r\n", argv[0]);
    return;
  }

  pin = atoi(argv[1]);
  v = atoi(argv[2]);
  v = v != 0 ? 1 : 0;

  if(pin >= GPIO_MAX_OUTPUT)
  {
    shell_printf(intf, "Invalid pin %d\r\n", pin);
    return;
  }

  gpio_set((gpio_out_pin_t)pin, v);

  shell_printf(intf, "set gpio out %d to %d\r\n", pin, v);
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

  shell_printf(intf, "\r\n");

  for(uint8_t i = 0; i < GPIO_MAX_INPUT; i++)
  {
    shell_printf(intf, "gpio in %d - %d, %s\r\n",
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

  __float_to_int_1dec(heater->oil_pump.freq, &freq_int, &freq_dec);

  shell_printf(intf, "\r\n");

  shell_printf(intf, "state : %s\r\n", heater_state_desc[heater->state]);
  shell_printf(intf, "pump  : %s, freq %d.%d Hz\r\n",
      heater->oil_pump.on ? "on" : "off",
      freq_int, freq_dec);
  shell_printf(intf, "glow  : %s\r\n", heater->glow_plug.on ? "on" : "off");
  shell_printf(intf, "fan   : %s, power %d%%\r\n",
      heater->fan.on ? "on" : "off",
      heater->fan.pwr);
}

static void
shell_command_glow(ShellIntf* intf, int argc, const char** argv)
{
  shell_printf(intf, "\r\n");

  if(argc != 2)
  {
    shell_printf(intf, "invalid %s on|off\r\n", argv[0]);
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
    shell_printf(intf, "invalid %s on|off\r\n", argv[0]);
    return;
  }
  shell_printf(intf, "turned %s glow plug\r\n", argv[1]);
}

static void
shell_command_oil(ShellIntf* intf, int argc, const char** argv)
{
  shell_printf(intf, "\r\n");

  if(argc != 2)
  {
    shell_printf(intf, "invalid %s on|off\r\n", argv[0]);
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
    shell_printf(intf, "invalid %s on|off\r\n", argv[0]);
    return;
  }
  shell_printf(intf, "turned %s oil pump\r\n", argv[1]);
}

static void
shell_command_fan(ShellIntf* intf, int argc, const char** argv)
{
  shell_printf(intf, "\r\n");

  if(argc != 2)
  {
    shell_printf(intf, "invalid %s on|off\r\n", argv[0]);
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
    shell_printf(intf, "invalid %s on|off\r\n", argv[0]);
    return;
  }
  shell_printf(intf, "turned %s fan\r\n", argv[1]);
}

static void
shell_command_set_oil_pump_freq(ShellIntf* intf, int argc, const char** argv)
{
  float freq;

  shell_printf(intf, "\r\n");

  if(argc != 2)
  {
    shell_printf(intf, "invalid %s <freq>\r\n", argv[0]);
    return;
  }

  freq = atof(argv[1]);

  if(freq < OIL_PUMP_MIN_FREQ || freq > OIL_PUMP_MAX_FREQ)
  {
    shell_printf(intf, "Invalid frequency\r\n");
    return;
  }

  heater_oil_pump_freq(freq);
}

static void
shell_command_set_fan_power(ShellIntf* intf, int argc, const char** argv)
{
  uint8_t pwr;

  shell_printf(intf, "\r\n");

  if(argc != 2)
  {
    shell_printf(intf, "invalid %s <pwr>\r\n", argv[0]);
    return;
  }

  pwr = atoi(argv[1]);

  if(pwr < 0 || pwr > 100)
  {
    shell_printf(intf, "invalid: should be between 0 and 100\r\n");
    return;
  }

  heater_fan_power(pwr);
  shell_printf(intf, "set power to %d\r\n", pwr);
}

static void
shell_command_settings(ShellIntf* intf, int argc, const char** argv)
{
#if 0
  settings_t* s = settings_get();
  uint8_t   freq_int, freq_dec;

  shell_printf(intf, "\r\n");

  shell_printf(intf, "gplug on1 %ld sec\r\n", s->glow_plug_on_duration_for_start / 1000);
  shell_printf(intf, "opriming  %ld sec\r\n", s->oil_pump_priming_duration / 1000);
  shell_printf(intf, "gplug on2 %ld sec\r\n", s->glow_plug_on_duration_for_stop / 1000);
  shell_printf(intf, "cool down %ld sec\r\n", s->cooling_down_period / 1000);
  shell_printf(intf, "fan pwr1  %d %%\r\n", s->startup_fan_power);
  shell_printf(intf, "fan pwr2  %d %%\r\n", s->stop_fan_power);
  shell_printf(intf, "gplug frq %d Hz\n", s->glow_plug_pwm_freq);
  shell_printf(intf, "gplug dty %d %%\n", s->glow_plug_pwm_duty);

  __float_to_int_1dec(s->glow_plug_pwm_freq, &freq_int, &freq_dec);
  shell_printf(intf, "op freq   %d.%d Hz\n", freq_int, freq_dec);

  shell_printf(intf, "op pulse  %d ms\n", s->oil_pump_pulse_length);
  shell_printf(intf, "fan pwr3  %d %%\n", s->fan_default_power);
#endif
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
      shell_printf(intf, "\r\nError: too many arguments\r\n");
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
      shell_printf(intf, "\r\nExecuting %s\r\n", argv[0]);
      _commands[i].handler(intf, argc, argv);
      return;
    }
  }
  shell_printf(intf, "%s", "\r\nUnknown Command: ");
  shell_printf(intf, "%s", argv[0]);
  shell_printf(intf, "%s", "\r\n");
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
    intf->put_tx_data(intf, (uint8_t*)_welcome, sizeof(_welcome) -1);
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
