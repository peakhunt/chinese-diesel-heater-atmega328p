#include <stdlib.h>
#include <avr/interrupt.h>
#include "event_dispatcher.h"
#include "event_list.h"

#define TIMER_TICK_COUNT_FOR_1MS        250

ISR(TIMER0_COMPA_vect)
{
  event_set(1 << DISPATCH_EVENT_TIMER_TICK);
}

void
sys_timer_init(void)
{
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  // set TC0 registers to generate 1ms system tick
  //
  // the core is running with 16 MHz Clock.
  // So to generate 1ms tick with 8 bit TC value,
  //
  // 1ms  = 16000000 Hz / 1000 = 16000 Hz
  // with 64 pre-scaler,
  // 1ms  = 16000 Hz / 64 = 250, which is good enough for us
  //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  TCCR0A  = (1 << WGM01) | (0 << WGM00) |                /* CTC mode.        */
            (0 << COM0A1) | (0 << COM0A0) |              /* OC0A disabled.   */
            (0 << COM0B1) | (0 << COM0B0);               /* OC0B disabled.   */
  TCCR0B  = (0 << WGM02) | _BV(CS01) | _BV(CS00);        /* CTC mode., 64 prescale  */
  OCR0A   = TIMER_TICK_COUNT_FOR_1MS - 1;
  TCNT0   = 0;                                           /* Reset counter.   */
  TIFR0   = (1 << OCF0A);                                /* Reset pending.   */
  TIMSK0  = (1 << OCIE0A);                               /* IRQ on compare.  */
}
