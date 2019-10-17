/* usart.c

    Copyright (C) 2017 - 2019 Michael Martens

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#include "at324hw.h"
#include "usart.h"

usart_dev_t usart0;

void usart0_init(uint16_t baudrate)
{
  usart0.baudrate = FOSC/16/baudrate - 1;
  usart0.tx_datastart = 0;
  usart0.tx_dataend   = 0;

  usart0.rx_datastart = 0;
  usart0.rx_dataend   = 0;

  /* Set baud rate */
  UBRR0H = (unsigned char)(usart0.baudrate>>8);
  UBRR0L = (unsigned char)usart0.baudrate;
  /* Enable receiver and transmitter */
  UCSR0B = (1<<RXCIE0)|(1<<TXCIE0)|(1<<UDRIE0)|(1<<RXEN0)|(1<<TXEN0);
  /* Set frame format: 8data, 2stop bit */
  UCSR0C = (1<<USBS0)|(0b11<<UCSZ00);
}

int8_t usart0_transmit(uint8_t* data, int8_t nb)
{
  int8_t i;
  if(nb >= (USART_TX_BUFFER_SIZE + usart0.tx_datastart - usart0.tx_dataend))
  {
    return 0;
  }
  for(i=0 ; i<nb ; i++)
  {
    usart0.tx_buffer[((usart0.tx_dataend + i) & USART_TX_BUFFER_MASK)] = data[i];
  }
  cli();
  usart0.tx_dataend += nb;
  if(UCSR0A & (1<<UDRE0))
  {
    UDR0 = usart0.tx_buffer[usart0.tx_datastart & USART_TX_BUFFER_MASK];
    usart0.tx_datastart++;
  }
  sei();
  return nb;
}

int8_t usart0_receive(uint8_t* buf, int8_t nb)
{
  int8_t i;
  int8_t cnt;
  cnt = usart0.rx_dataend - usart0.rx_datastart;
  if(cnt == 0) return 0;
  if(cnt < nb)
    nb = cnt;
  for(i=0; i<nb; i++)
  {
    *buf = usart0.rx_buffer[usart0.rx_datastart & USART_RX_BUFFER_MASK];
    buf++;
    usart0.rx_datastart++;
  }
  return nb;
}

ISR(USART0_RX_vect)
{
  uint8_t c;

  if(UCSR0A & (1<<RXC0))
  {
    c = UDR0;
    if(usart0.rx_dataend - usart0.rx_datastart <= USART_RX_BUFFER_SIZE)
    {
      usart0.rx_dataend++;
      usart0.rx_buffer[usart0.rx_dataend & USART_RX_BUFFER_MASK] = c;
    }
    if(usart0.rx_datastart >= USART_RX_BUFFER_SIZE)
    {
      usart0.rx_datastart = usart0.rx_datastart % USART_RX_BUFFER_SIZE;
      usart0.rx_dataend = usart0.rx_dataend % USART_RX_BUFFER_SIZE;
    }
  }
}

ISR(USART0_UDRE_vect)
{
  if(usart0.tx_datastart != usart0.tx_dataend)
  {
    UDR0 = usart0.tx_buffer[usart0.tx_datastart & USART_TX_BUFFER_MASK];
    usart0.tx_datastart++;

    if(usart0.tx_datastart >= USART_TX_BUFFER_SIZE)
    {
      usart0.tx_datastart = usart0.tx_datastart % USART_TX_BUFFER_SIZE;
      usart0.tx_dataend = usart0.tx_dataend % USART_TX_BUFFER_SIZE;
    }
  }
}

ISR(USART0_TX_vect)
{

}

ISR(USART1_RX_vect)
{

}

ISR(USART1_UDRE_vect)
{

}

ISR(USART1_TX_vect)
{

}


