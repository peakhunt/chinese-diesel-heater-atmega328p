#include <math.h>
#include "ntc50.h"

#define LPF_Beta      0.025         // 0 < B < 1
#define THERMISTOR_NOMINAL          50000       // 50K
#define TEMPERTURE_NOMINAL          25          // 25 C
#define BCOEFFICIENT                3950        // b3950 thermistor

static inline void
ntc50_calc_temp(ntc50_t* ntc50)
{
#if 0
  const float c1 = 1.009249522e-03,
              c2 = 2.378405444e-04,
              c3 = 2.019202697e-07;
  float logR = logR = log(ntc50->r1);
  float T;

  T = (1.0 / (c1 + c2*logR + c3*logR*logR*logR));
  T = T - 273.15;
  T = (T * 9.0)/ 5.0 + 32.0; 

  //
  // pass T through low pass filter
  //
  ntc50->temp = ntc50->temp - (LPF_Beta * (ntc50->temp - T));
#else
  float steinhart;

  steinhart = ntc50->r1 / THERMISTOR_NOMINAL;
  steinhart = log(steinhart);
  steinhart /= BCOEFFICIENT;
  steinhart += 1.0 / (TEMPERTURE_NOMINAL + 273.15);
  steinhart = 1.0 / steinhart;
  steinhart -= 273.15;

  ntc50->temp = ntc50->temp - (LPF_Beta * (ntc50->temp - steinhart));
#endif
}

static void
ntc50_adc_listener(adc_channel_t ch, adcsample_t sample, void* arg)
{
  ntc50_t* ntc50 = (ntc50_t*)arg;
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
}

void
ntc50_init(ntc50_t* ntc50, uint8_t vs, float r2, adc_channel_t chnl)
{
  ntc50->temp     = 0.0f;
  ntc50->vs       = vs;
  ntc50->r1       = 0.0f;
  ntc50->r2       = r2;
  ntc50->chnl     = chnl;

  adc_reader_listen(chnl, ntc50_adc_listener, (void*)ntc50);
}
