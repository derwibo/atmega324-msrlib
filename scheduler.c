/* scheduler.c

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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stddef.h>
#include "scheduler.h"
#include "at324hw.h"

systemtime_t systime;

task_t tasktable[MAX_NR_TASKS];

uint16_t systime_get()
{
  return systime.ticks;
}

void systime_reset()
{
  cli();
  systime.ticks = 0;
  sei();
}

void scheduler_init(void)
{
  int i;

  systime.ticks = 0;

  for(i=0; i<MAX_NR_TASKS; i++)
  {
    tasktable[i].state = 0;
    tasktable[i].rate = 0;
    tasktable[i].counter = 0;
    tasktable[i].func = NULL;
  }  

  tcnt1_init_1ms();
}

void scheduler_run(void)
{
  int8_t i;

  for(i=0; i<MAX_NR_TASKS; i++)
  {
    if(tasktable[i].func != NULL && tasktable[i].state == TASK_RUNNING)
    {
      if(tasktable[i].counter <= 0)
      {
        cli();
        tasktable[i].counter += tasktable[i].rate;
        sei();
        tasktable[i].func();
      }
    }
  }
}

int8_t task_create(void (*func)(void), uint16_t rate, uint8_t state)
{
  int8_t i;

  for(i=0; i<MAX_NR_TASKS; i++)
  {
    if(tasktable[i].func == NULL)
    {
      tasktable[i].state = TASK_STOPPED;
      tasktable[i].func = func;
      tasktable[i].rate = (rate > 0 ? rate : 1);
      tasktable[i].counter = 0;
      tasktable[i].state = state;
      return i;
    }
  }
  return -1;
}

void task_start(uint8_t tasknr)
{
  if(tasknr < MAX_NR_TASKS && tasktable[tasknr].func != NULL && tasktable[tasknr].state != TASK_RUNNING)
  {
    tasktable[tasknr].counter = 0;
    tasktable[tasknr].state = TASK_RUNNING;   
  }
}

void task_stop(uint8_t tasknr)
{
  if(tasknr < MAX_NR_TASKS && tasktable[tasknr].state == TASK_RUNNING)
  {
    tasktable[tasknr].state = TASK_STOPPED;
  }
}

ISR(TIMER1_COMPA_vect)
{
  int i;
  systime.ticks++;
  
  for(i=0; i<MAX_NR_TASKS; i++)
  {
    tasktable[i].counter--;
  }  
}

ISR(TIMER1_OVF_vect)
{
  // timer_signal = 1;
}

