/* at324hw.c

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

#include "at324hw.h"

inline void at324_prescaler_init()
{
#ifdef FOSC_8MHZ
/* Change the clock prescaling to 1 independent of the CLKDIV fuse */

  CLKPR = 0x80;
  CLKPR = 0x00;

  __asm__ __volatile__ ("nop");
  __asm__ __volatile__ ("nop");
  __asm__ __volatile__ ("nop");

#endif
}

void at324_init()
{

}


void tcnt1_init_1ms()
{
  TCCR1A = 0b00000000;
#ifdef FOSC_8MHZ
  TCCR1B = 0b00001011; // enable the timer with prescaler 64 (for 8 Mhz system clock)
#else
  TCCR1B = 0b00001010; // enable the timer with prescaler 8 (for 1 Mhz system clock)
#endif
  OCR1A  = 125;      // Set the TOP Value so that we will get 1 ms rate

  TIMSK1 |= (1 << TOIE1 | 1 << OCIE1A);
}

void tcnt2_init_122hz_pwm()
{
  TCCR2A = 0b00000011;  // Fast PWM Mode
#ifdef FOSC_8MHZ
  TCCR2B = 0b00000110;  // prescaler 256 (for 8 Mhz system clock)
#else
  TCCR2B = 0b00000011;  // prescaler 32 (for 1 Mhz system clock)
#endif
}

