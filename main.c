#include <avr/io.h>
#include <avr/interrupt.h>

#include "mainloop_timer.h"
#include "event_dispatcher.h"
#include "sys_timer.h"
#include "misc.h"
#include "shell.h"
#include "pwm.h"
#include "gpio.h"
#include "adc_reader.h"

#include "heater.h"

int
main(void)
{
  // just to make sure, disable interrupt
  cli();

  event_dispatcher_init();
  mainloop_timer_init();

  pwm_init();
  gpio_init();
  misc_init();
  shell_init();
  sys_timer_init();

  adc_reader_init();

  heater_init();

  sei();

  shell_start();

  while(1)
  {
    event_dispatcher_dispatch();
  }
  return 0;
}
