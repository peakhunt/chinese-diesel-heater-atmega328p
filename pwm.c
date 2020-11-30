#include <stdint.h>
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "pwm.h"

//
// PWM clock is running at 16 Mhz using no clock divider out of 16 MHz system clock.
//
// 15.625 KHz is the target PWM frequency and we wanna be able to control the duty cycle from 0 to 100%
//

////////////////////////////////////////////////////////////////////////////////
//
// private definitions
//
////////////////////////////////////////////////////////////////////////////////
#define PWM_CLOCK                 16000000                        // 2 Mhz
#define PWM_TARGET_FREQUENCY      15625                           // 15.625Kz Khz
#define PWM_TICKS_FOR_FREQUENCY   (1024 - 1)                      // 16 Mhz / 15.625 Khz - 1
#define PWM_PCT_TO_TICKS(pct)     ((uint16_t)((float)pct * PWM_TICKS_FOR_FREQUENCY / 100.0f))


////////////////////////////////////////////////////////////////////////////////
//
// pwm configuration 
//
////////////////////////////////////////////////////////////////////////////////
static void
pwm_init_config(void)
{
  //
  // reset counter
  //
  TCNT1   = 0;

  // initialize each channel to 0% duty cycle
  OCR1A   = PWM_PCT_TO_TICKS(0);
  OCR1B   = PWM_PCT_TO_TICKS(0);

  // initialize TOP value so period becomes 2.5 ms
  ICR1    = PWM_PCT_TO_TICKS(100);

  // setup TC mode and start counting by providing clock.
  // TCCR1A/TCCR1B
  //
  // Compare Output Mode for Fast PWM with ICR1 as TOP
  //
  // Clear OC1A/OC1B on Compare Match, set OC1A/OC1B at BOTTOM (non-inverting mode)
  // COM1A1 : 1
  // COM1A0 : 0
  // COM1B1 : 1
  // COM1B0 : 0
  //
  //            WGM13  WGM12   WGM11   WGM10
  //Mode 14 :     1     1       1         0
  //
  // ICNC1  : Don't care
  // ICES1  : Don't care
  //
  // Clock Divider :  no Divider
  //    CS12    CS11    CS10
  //      0       0       1
  //

  // for zero duty cycle
  // TCCR1A    = (_BV(COM1A1) | _BV(COM1B1) | _BV(WGM11));
  TCCR1A    = _BV(WGM11);
  TCCR1B    = (_BV(WGM13)  | _BV(WGM12)  |  _BV(CS10));

}

////////////////////////////////////////////////////////////////////////////////
//
// public interfaces 
//
////////////////////////////////////////////////////////////////////////////////
void
pwm_init(void)
{
  pwm_init_config();

  // PB1/PB2 are OC1x output
  //
  DDRB |= (_BV(PB1) | _BV(PB2));
}

void
pwm_control(pwm_channel_t chnl, uint8_t pct)
{
  uint16_t    ticks;

  ticks = PWM_PCT_TO_TICKS(pct);

  switch(chnl)
  {
  case pwm_channel_0:
    OCR1A   = ticks;
    if(ticks == 0)
    {
      TCCR1A &= ~_BV(COM1A1);
    }
    else
    {
      TCCR1A |= _BV(COM1A1);
    }
    break;

  case pwm_channel_1:
    OCR1B   = ticks;
    if(ticks == 0)
    {
      TCCR1A &= ~_BV(COM1B1);
    }
    else
    {
      TCCR1A |= _BV(COM1B1);
    }
    break;
  }
}
