#ifndef __FAN_DEF_H__
#define __FAN_DEF_H__ 

#include "app_common.h"
#include "pwm.h"

#define FAN_DEFAULT_POWER     55

typedef struct
{
  bool            on;
  pwm_channel_t   pwm_chnl;
  uint8_t         pwr;
} fan_t;

extern void fan_init(fan_t* fan, pwm_channel_t chnl);
extern void fan_start(fan_t* fan);
extern void fan_stop(fan_t* fan);
extern void fan_set_power(fan_t* fan, uint8_t pwr);

#endif /* !__FAN_DEF_H__ */
