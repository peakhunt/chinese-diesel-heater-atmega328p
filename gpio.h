#ifndef __GPIO_DEF_H__
#define __GPIO_DEF_H__

#include "app_common.h"

typedef enum
{
  gpio_out_pin_pd2,
  gpio_out_pin_pd3,
  gpio_out_pin_pd4,
} gpio_out_pin_t;
#define GPIO_MAX_OUTPUT   (gpio_out_pin_pd4 + 1)

typedef enum
{
  gpio_in_pin_pd5,
  gpio_in_pin_pd6,
  gpio_in_pin_pd7,
} gpio_in_pin_t;
#define GPIO_MAX_INPUT    (gpio_in_pin_pd7 + 1)

typedef enum
{
  gpio_input_state_low,
  gpio_input_state_low_to_high,
  gpio_input_state_high,
  gpio_input_state_high_to_low,
} gpio_input_state_t;

typedef void (*gpio_listener)(gpio_in_pin_t pin, gpio_input_state_t state, void* arg);

extern void gpio_init(void);
extern void gpio_set(gpio_out_pin_t pin, bool v);
extern bool gpio_get(gpio_in_pin_t pin);
extern void gpio_set_debounce(gpio_in_pin_t pin, bool debounce);
extern void gpio_listen(gpio_in_pin_t pin, gpio_listener listener, void* arg);
extern gpio_input_state_t gpio_get_state(gpio_in_pin_t pin);

#endif /* !__GPIO_DEF_H__ */
