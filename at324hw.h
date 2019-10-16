/* at324hw.h

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

#ifndef AT324HW_H
#define AT324HW_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#define FOSC_8MHZ

#ifdef FOSC_8MHZ
#define FOSC 8000000
#else
#define FOSC 1000000
#endif

inline void at324_prescaler_init();

void at324_init();
void tcnt1_init_1ms();
void tcnt2_init_122hz_pwm();

#endif /* AT324HW_H */

