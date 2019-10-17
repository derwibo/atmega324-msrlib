/* scheduler.h

    Copyright (C) 2014 Michael Martens

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

#ifndef SCHEDULER_H
#define SCHEDULER_H

#define MAX_NR_TASKS 16

#define TASK_STOPPED 0
#define TASK_RUNNING 1

struct systemtime
{
  volatile uint16_t ticks;
};

typedef struct systemtime systemtime_t;

struct task
{
  uint8_t state;
  uint16_t rate;
  volatile int16_t counter;
  void (*func)(void);
};

typedef struct task task_t;

void scheduler_init(void);
void scheduler_run(void);

int8_t task_create(void (*func)(void), uint16_t rate, uint8_t state);
void task_start(uint8_t tasknr);
void task_stop(uint8_t tasknr);

uint16_t systime_get();
void systime_reset();

#endif /* SCHEDULER_H */

