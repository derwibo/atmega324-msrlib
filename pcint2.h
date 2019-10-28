/* pcint2.h

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

#ifndef PCINT2_H
#define PCINT2_H

#define PWM2A 1
#define PWM2B 2

struct pcint2_dev
{
  uint8_t rmask;
  uint8_t fmask;
  volatile uint8_t prev;
  volatile uint8_t cnt[8];
};

typedef struct pcint2_dev pcint2_dev_t;

void pcint2_init();

void pcint2_enable(uint8_t channel, uint8_t raising, uint8_t falling);
void pcint2_disable(uint8_t channel);

inline uint8_t pcint2_peek(uint8_t channel);
inline uint8_t pcint2_get(uint8_t channel);

#endif /* PCINT2_H */

