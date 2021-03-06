﻿/*
 * event_dispatcher.c
 *
 * Created: 12/8/2016 2:35:56 PM
 *  Author: hkim
 */ 

#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdint.h>
#include "app_common.h"
#include "event_dispatcher.h"

#define EVENT_MAX_EVENTS                32

static volatile uint32_t            _events;
static event_handler                _event_handlers[EVENT_MAX_EVENTS];

static void
event_dispatcher_event_dispatch(void)
{
  uint32_t    evt;
  uint32_t    i;

  if(_events == 0)
  {
    return;
  }

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    evt = _events;
    _events = 0;
  }

  if(evt == 0)
  {
    return;
  }

  for(i = 0; i < EVENT_MAX_EVENTS; i++)
  {
    if(evt & 0x01)
    {
      if(_event_handlers[i] != NULL)
      {
        _event_handlers[i](i);
      }
    }
    evt >>= 1;
  }
}

void
event_dispatcher_init(void)
{
  uint8_t   i;
  
  _events = 0;
  for(i = 0; i < EVENT_MAX_EVENTS; i++)
  {
    _event_handlers[i] = NULL;
  }
}

void
event_set(uint32_t bits)
{
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    _events |= bits;
  }

  return;
}

void
event_register_handler(event_handler handler, uint32_t event)
{
  _event_handlers[event] = handler;
}

void
event_dispatcher_dispatch(void)
{
  event_dispatcher_event_dispatch();
}
