/* pwm.h

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

#ifndef PWM_H
#define PWM_H

#define PWM2A 1
#define PWM2B 2

struct pwm_dev
{

};

typedef struct pwm_dev pwm_dev_t;

void pwm_init();

void pwm_set(uint8_t channel, uint8_t value);

#endif /* PWM_H */

