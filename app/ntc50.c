#include <math.h>
#include "ntc50.h"

static inline void
ntc50_calc_temp(ntc50_t* ntc50)
{
  const float c1 = 1.009249522e-03,
              c2 = 2.378405444e-04,
              c3 = 2.019202697e-07;
  float logR = logR = log(ntc50->r1);
  float T;

  T = (1.0 / (c1 + c2*logR + c3*logR*logR*logR));
  T = T - 273.15;
  T = (T * 9.0)/ 5.0 + 32.0; 

  ntc50->temp = T;
}

static void
ntc50_tmr_callback(SoftTimerElem* te)
{
  ntc50_t* ntc50 = (ntc50_t*)te->priv;
  float         vout;

  vout = adc_get_volt(ntc50->chnl);
  //
  // just to prevent floating point error
  // realistically it never goes down to zero
  //
  if(vout < 0.1f)
  {
    vout = 0.1f;
  }

  //
  // voltage divider
  // Vout = (Vs * r2)/(r1 + r2)
  // 
  //
  // r1 = (Vs * r2)/Vout - r2
  //
  ntc50->r1 = (ntc50->vs * ntc50->r2)/vout - ntc50->r2;

  ntc50_calc_temp(ntc50);
  mainloop_timer_schedule(&ntc50->tmr, 1000);
}

void
ntc50_init(ntc50_t* ntc50, uint8_t vs, float r2, adc_channel_t chnl)
{
  ntc50->temp     = 0.0f;
  ntc50->vs       = vs;
  ntc50->r1       = 0.0f;
  ntc50->r2       = r2;
  ntc50->chnl     = chnl;

  soft_timer_init_elem(&ntc50->tmr);
  ntc50->tmr.cb = ntc50_tmr_callback;
  ntc50->tmr.priv = ntc50;

  mainloop_timer_schedule(&ntc50->tmr, 1000);
}
