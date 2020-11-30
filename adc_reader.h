#ifndef __ADC_READER_DEF_H__
#define __ADC_READER_DEF_H__

#include "app_common.h"
#include "adc.h"

typedef enum
{
  adc_channel_0,
  adc_channel_1,
  adc_channel_2,
  adc_channel_3,
} adc_channel_t;

#define ADC_MAX_CHANNELS    (adc_channel_3 + 1)

extern void adc_reader_init(void);
extern adcsample_t adc_get(adc_channel_t ch);

#endif /* !__ADC_READER_DEF_H__ */
