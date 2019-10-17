/* adc.h

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

#ifndef ADC_H
#define ADC_H

#define ADC_REF_256_INT 1
#define ADC_REF_AVCC 2

struct adc_dev
{
  uint8_t enabled;
  uint8_t refsrc;
  uint8_t curchn;
  uint16_t result[8];
  uint8_t mux[8];
};

typedef struct adc_dev adc_dev_t;

void adc_init(uint8_t refsrc);

void adc_enable_input(uint8_t pin);
void adc_disable_input(uint8_t pin);
void adc_set_input(uint8_t pin, uint8_t state);
void adc_set_mux(uint8_t channel, uint8_t mux);

void adc_start();
void adc_stop();
uint16_t adc_get(uint8_t index);

#endif /* ADC_H */

