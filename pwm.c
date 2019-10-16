/* pwm.c

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
#include "pwm.h"
#include "at324hw.h"

pwm_dev_t pwm0;

void pwm_init()
{
  tcnt2_init_122hz_pwm();
}

void pwm_set(uint8_t channel, uint8_t value)
{
  switch(channel)
  {
    case PWM2A:
      if(value)
      {
        OCR2A = value;
        TCCR2A |= (1 << COM2A1);
      }
      else
      {
        TCCR2A &= ~((1 << COM2A1)|(1<<COM2A0));
      }
      break;
    case PWM2B:
      if(value)
      {
        OCR2B = value;
        TCCR2A |= (1 << COM2B1);
      }
      else
      {
        TCCR2A &= ~((1 << COM2B1)|(1<<COM2B0));
      }
      break;
    default:
      break;
  } 
}


