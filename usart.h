/* usart.h

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

#ifndef USART_H
#define USART_H

#include <inttypes.h>

#define USART_TX_BUFFER_SIZE 0x0F

struct usart_dev
{
  uint16_t baudrate;
  volatile uint8_t status;
  volatile uint8_t tx_buffer[USART_TX_BUFFER_SIZE+1];
  volatile int8_t tx_datastart;
  volatile int8_t tx_dataend;
};

typedef struct usart_dev usart_dev_t;

void usart0_init(uint16_t baudrate);

int8_t usart0_transmit(uint8_t* data, int8_t nb);

#endif /* USART_H */

