/* pcint2.c

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
#include "pcint2.h"

pcint2_dev_t pcint2;

void pcint2_init()
{
  int i;
  pcint2.rmask = 0;
  pcint2.fmask = 0;
  pcint2.prev = 0;
  for(i=0; i<8; i++)
  {
    pcint2.cnt[i] = 0;
  }

  PCICR |= (1 << PCIE2);
}

void pcint2_enable(uint8_t channel, uint8_t raising, uint8_t falling)
{
  uint8_t mask;
  mask = (1 << channel);
  PCMSK2 |= mask;
  if(raising) pcint2.rmask |= mask;
  if(falling) pcint2.fmask |= mask;
}

void pcint2_disable(uint8_t channel)
{
  uint8_t mask;
  mask = ~(1 << channel);
  PCMSK2 &= mask;
  pcint2.rmask &= mask;
  pcint2.fmask &= mask;
}

inline uint8_t pcint2_peek(uint8_t channel)
{
  return pcint2.cnt[channel];
}

inline uint8_t pcint2_get(uint8_t channel)
{
  uint8_t val;
  cli();
  val = pcint2.cnt[channel];
  pcint2.cnt[channel] = 0;
  sei();
  return val;
}

ISR(PCINT2_vect)
{
  uint8_t i;
  uint8_t val;
  uint8_t trig;
  
  val = PINC;
  trig = val ^ pcint2.prev;
  pcint2.prev = val;
  trig = trig & ((val & pcint2.rmask) | (~val & pcint2.fmask));

  if(trig)
  {  
    for(i=0; i<8; i++)
    {
      if(trig & 0x01)
      {
        pcint2.cnt[i]++;
      }
      trig = trig >> 1;
    }
  }  
}

