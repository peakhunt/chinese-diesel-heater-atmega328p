#include <avr/io.h>
#include <avr/interrupt.h>
#include "gpio.h"
#include "event_dispatcher.h"
#include "event_list.h"
#include "soft_timer.h"
#include "mainloop_timer.h"

#define PIN_CHANGE_DEBOUNCE_TIME          10    // 10ms
typedef struct
{
  gpio_in_pin_t           pin_name;
  gpio_input_state_t      state;
  SoftTimerElem           debounce_tmr;
  uint8_t                 pin;
  uint8_t                 event;
} gpio_input_t;

#define GPIO_SET(port, bv, val)       \
  if(val)                             \
  {                                   \
    port |= (bv);                     \
  }                                   \
  else                                \
  {                                   \
    port &= ~(bv);                    \
  }

static gpio_input_t   _inputs[GPIO_MAX_INPUT] =
{
  {
    .pin_name = gpio_in_pin_pd5,
    .pin = PD5,
    .event = DISPATCH_EVENT_GPIO_IN_0,
  },
  {
    .pin_name = gpio_in_pin_pd6,
    .pin = PD6,
    .event = DISPATCH_EVENT_GPIO_IN_1,
  },
  {
    .pin_name = gpio_in_pin_pd7,
    .pin = PD7,
    .event = DISPATCH_EVENT_GPIO_IN_2,
  },
};

ISR(PCINT2_vect)
{
  event_set(1 << DISPATCH_EVENT_PIN_CHANGE);
}

static void
handle_pin_change(uint32_t event)
{
  gpio_input_t*   input;
  uint8_t         pv;

  for(uint8_t i = 0; i < GPIO_MAX_INPUT; i++)
  {
    input = &_inputs[i];

    pv = (PIND & _BV(input->pin)) == 0 ? 0 : 1;

    switch(input->state)
    {
    case gpio_input_state_low:
      if(pv)
      {
        // goes high
        input->state = gpio_input_state_low_to_high;
        mainloop_timer_schedule(&input->debounce_tmr, PIN_CHANGE_DEBOUNCE_TIME);
      }
      break;

    case gpio_input_state_low_to_high:
      if(pv == 0)
      {
        // goes low again
        input->state = gpio_input_state_low;
        mainloop_timer_cancel(&input->debounce_tmr);
      }
      break;

    case gpio_input_state_high:
      if(pv == 0)
      {
        // goes low
        input->state = gpio_input_state_high_to_low;
        mainloop_timer_schedule(&input->debounce_tmr, PIN_CHANGE_DEBOUNCE_TIME);
      }
      break;

    case gpio_input_state_high_to_low:
      if(pv)
      {
        // goes high again
        input->state = gpio_input_state_high;
        mainloop_timer_cancel(&input->debounce_tmr);
      }
      break;
    }
  }
}

static void
debounce_tmr_callback(SoftTimerElem* te)
{
  gpio_input_t*   input = (gpio_input_t*)te->priv;

  if(input->state == gpio_input_state_low_to_high)
  {
    input->state = gpio_input_state_high;
    event_set(1 << input->event);
  }
  else if(input->state == gpio_input_state_high_to_low)
  {
    input->state = gpio_input_state_low;
    event_set(1 << input->event);
  }
}

static void
init_input_pins(void)
{
  for(uint8_t i = 0; i < GPIO_MAX_INPUT; i++)
  {
    _inputs[i].state = (PIND & _BV(_inputs[i].pin)) == 0 ? gpio_input_state_low : gpio_input_state_high;

    soft_timer_init_elem(&_inputs[i].debounce_tmr);
    _inputs[i].debounce_tmr.cb = debounce_tmr_callback;
    _inputs[i].debounce_tmr.priv = &_inputs[i];
  }
}

void
gpio_init(void)
{
  // out
  DDRD |= (_BV(PD2) | _BV(PD3) | _BV(PD4));

  event_register_handler(handle_pin_change, DISPATCH_EVENT_PIN_CHANGE);

  // in. to make sure
  DDRD &= ~(_BV(PD5) | _BV(PD6) | _BV(PD7));

  init_input_pins();

  // PD5 - PCINT21
  // PD6 - PCINT22
  // PD7 - PCINT23
  PCMSK2 |= (_BV(PCINT21) | _BV(PCINT22) | _BV(PCINT23));

  // enable pin change interrupt
  PCICR |= _BV(PCIE2);
}

void
gpio_set(gpio_out_pin_t pin, bool v)
{
  switch(pin)
  {
  case gpio_out_pin_pd2:
    GPIO_SET(PORTD, _BV(PD2), v);
    break;

  case gpio_out_pin_pd3:
    GPIO_SET(PORTD, _BV(PD3), v);
    break;

  case gpio_out_pin_pd4:
    GPIO_SET(PORTD, _BV(PD4), v);
    break;

  default:
    return;
  }
}

bool
gpio_get(gpio_in_pin_t pin)
{
  switch(_inputs[pin].state)
  {
  case gpio_input_state_low:
  case gpio_input_state_low_to_high:
    return false;
  default:
    break;
  }
  return true;
}

gpio_input_state_t
gpio_get_state(gpio_in_pin_t pin)
{
  return _inputs[pin].state;
}
