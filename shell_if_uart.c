#include <avr/interrupt.h>
#include <avr/io.h>

#include "app_common.h"
#include "shell_if_uart.h"
#include "circ_buffer.h"
#include "event_list.h"
#include "event_dispatcher.h"

#define SHELL_DEFAULT_BAUD_RATE         115200

#define UBRR(b)                         (((F_CPU / b) >> 4) - 1)
#define UBRR2x(b)                       (((F_CPU / b) >> 3) - 1)

////////////////////////////////////////////////////////////////////////////////
//
// private variables
//
////////////////////////////////////////////////////////////////////////////////
static ShellIntf              _shell_uart_if;
static CircBuffer             _rx_cb;
static CircBuffer             _tx_cb;
static volatile uint8_t       _rx_buffer[CLI_RX_BUFFER_LENGTH];
static volatile uint8_t       _tx_buffer[CLI_TX_BUFFER_LENGTH];

////////////////////////////////////////////////////////////////////////////////
//
// utilities
//
////////////////////////////////////////////////////////////////////////////////
static void
shell_if_uart_event_handler(uint32_t event)
{
  shell_handle_rx(&_shell_uart_if);
}

static void
shell_if_uart_enter_critical(CircBuffer* cb)
{
  cli();
}

static void
shell_if_uart_leave_critical(CircBuffer* cb)
{
  sei();
}

static void
shell_if_uart_config_uart(void)
{
  unsigned long baud = SHELL_DEFAULT_BAUD_RATE;
  uint16_t baud_setting = (F_CPU / (8 * baud)) - 1;

#if 0 // not necessary
  // PD0 : RX
  // PD1 : Tx
  DDRD |= _BV(PD1);
  DDRD &= _BV(PD0);
#endif

  // try u2x mode first
  UCSR0A      = _BV(U2X0);

  if (((F_CPU == 16000000UL) && (baud == 57600)) || (baud_setting >4095))
  {
    UCSR0A        = 0;
    baud_setting  = (F_CPU / (16 * baud)) - 1;
  }

  UBRR0H      = baud_setting >> 8;
  UBRR0L      = baud_setting;

  UCSR0C      = _BV(UCSZ00) | _BV(UCSZ01);                  // 8 bit data, no parity, 1 stop-bit
  UCSR0B      = _BV(RXEN0)  | _BV(TXEN0) | _BV(RXCIE0);
}

////////////////////////////////////////////////////////////////////////////////
//
// RX IRQ
//
////////////////////////////////////////////////////////////////////////////////
ISR(USART_RX_vect)
{
  uint8_t       b;

  b = UDR0;

  if(circ_buffer_enqueue(&_rx_cb, &b, 1, true) == false)
  {
    // fucked up. overflow mostly.
    // do something here
  }

  event_set(1 << DISPATCH_EVENT_UART_CLI_RX);
}

ISR(USART_UDRE_vect)
{
  uint8_t     b;

  if(circ_buffer_dequeue(&_tx_cb, &b, 1, true))
  {
    UDR0 = b;
  }
  else
  {
    UCSR0B &= ~_BV(UDRIE0);
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// callbacks for core shell and rx interrupt
//
////////////////////////////////////////////////////////////////////////////////
static uint8_t
shell_if_uart_get_rx_data(ShellIntf* intf, uint8_t* data)
{
  if(circ_buffer_dequeue(&_rx_cb, data, 1, false) == false)
  {
    return false;
  }
  return true;
}

static bool
shell_if_uart_put_tx_data(ShellIntf* intf, uint8_t* data, uint16_t len)
{
  bool ret;

  cli();
  ret = circ_buffer_enqueue(&_tx_cb, data, len, true);
  if(ret)
  {
    UCSR0B |= _BV(UDRIE0);
  }
  sei();

  return ret;
}

////////////////////////////////////////////////////////////////////////////////
//
// public interfaces
//
////////////////////////////////////////////////////////////////////////////////
void
shell_if_uart_init(void)
{
  _shell_uart_if.cmd_buffer_ndx    = 0;
  _shell_uart_if.get_rx_data       = shell_if_uart_get_rx_data;
  _shell_uart_if.put_tx_data       = shell_if_uart_put_tx_data;

  INIT_LIST_HEAD(&_shell_uart_if.lh);

  circ_buffer_init(&_rx_cb, _rx_buffer, CLI_RX_BUFFER_LENGTH,
      shell_if_uart_enter_critical,
      shell_if_uart_leave_critical);

  circ_buffer_init(&_tx_cb, _tx_buffer, CLI_TX_BUFFER_LENGTH,
      shell_if_uart_enter_critical,
      shell_if_uart_leave_critical);

  shell_if_uart_config_uart();

  event_register_handler(shell_if_uart_event_handler, DISPATCH_EVENT_UART_CLI_RX);
  shell_if_register(&_shell_uart_if);
}
