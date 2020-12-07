#include "adc_reader.h"
#include "event_dispatcher.h"
#include "event_list.h"
#include "soft_timer.h"
#include "mainloop_timer.h"

#define ADC_DELAY_VALUE       50       // 100ms

static adcsample_t            _samples[ADC_MAX_CHANNELS];
static volatile adcsample_t   _curr_sample;
static uint8_t                _curr_ch;
static SoftTimerElem          _delay_tmr;

static void
adc_read_callback(adc_channels_num_t ch, adcsample_t sample)
{
  _curr_sample = sample;
  event_set(1 << DISPATCH_EVENT_ADC_COMPLETE);
}

static void
adc_read_complete(uint32_t event)
{
  _samples[_curr_ch] = _curr_sample;

  _curr_ch++;
  if(_curr_ch >= ADC_MAX_CHANNELS)
  {
    _curr_ch = 0;
  }

  // invoke timer
  mainloop_timer_schedule(&_delay_tmr, ADC_DELAY_VALUE);
}

static void
adc_delay_tmr_callback(SoftTimerElem* te)
{
  adc_start_conversion(_curr_ch, adc_read_callback);
}

void
adc_reader_init(void)
{
  soft_timer_init_elem(&_delay_tmr);
  _delay_tmr.cb = adc_delay_tmr_callback;

  event_register_handler(adc_read_complete, DISPATCH_EVENT_ADC_COMPLETE);
  adc_init(ADCRef_AVCC, ADCPrescaler_DIV_128);

  _samples[0] =
  _samples[1] =
  _samples[2] = 
  _samples[3] = 0;

  _curr_sample = 0;
  _curr_ch = 0;

  adc_start_conversion(_curr_ch, adc_read_callback);
}

adcsample_t
adc_get(adc_channel_t ch)
{
  return _samples[ch];
}

float
adc_get_volt(adc_channel_t ch)
{
  float v;
  adcsample_t   adc;

  adc = adc_get(ch);

  v = 5.0f * adc / 1023.0f;
  return v;
}
