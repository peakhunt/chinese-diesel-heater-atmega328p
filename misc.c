#include <avr/io.h>
#include "soft_timer.h"
#include "mainloop_timer.h"
#include "misc.h"

#define BLINK_INTERVAL      50
#define ONE_SEC_INTERVAL    1000

uint32_t __uptime;

static SoftTimerElem  _blink_timer;
static SoftTimerElem  _1sec_timer;

static void
blink_callback(SoftTimerElem* te)
{
  PORTB = PORTB ^ _BV(PB5);
  mainloop_timer_schedule(&_blink_timer, BLINK_INTERVAL);
}

static void
onesec_callback(SoftTimerElem* te)
{
  __uptime++;
  mainloop_timer_schedule(&_1sec_timer, ONE_SEC_INTERVAL);
}

void
misc_init(void)
{
  __uptime = 0;

  // init blink timer
  soft_timer_init_elem(&_blink_timer);
  _blink_timer.cb    = blink_callback;
  mainloop_timer_schedule(&_blink_timer, BLINK_INTERVAL);

  // init one sec timer
  soft_timer_init_elem(&_1sec_timer);
  _1sec_timer.cb    = onesec_callback;
  mainloop_timer_schedule(&_1sec_timer, ONE_SEC_INTERVAL);

  // init LED pin
  DDRB |= _BV(PB5);
}
