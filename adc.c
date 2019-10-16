/* adc.c

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
#include "adc.h"
#include "at324hw.h"

adc_dev_t adc0;

void adc_init(uint8_t refsrc)
{
  switch(refsrc)
  {
    case ADC_REF_256_INT:
      adc0.refsrc = (1 << REFS1)|(1 << REFS0);
      break;
    case ADC_REF_AVCC:
      adc0.refsrc = (1 << REFS0);
      break;
    default:
      adc0.refsrc = (1 << REFS0);
  }
#ifdef FOSC_8MHZ
  ADCSRA = (1 << ADEN)|(1 << ADIE)|(0b110 << ADPS0); // Prescaler 64 gives 125 kHz ADC rate
#else
  ADCSRA = (1 << ADEN)|(1 << ADIE)|(0b11 << ADPS0); // Prescaler 8 gives 125 kHz ADC rate
#endif
  ADCSRB = 0x00;
 
  ADMUX  = adc0.refsrc;
  adc0.curchn = 0;
}

void adc_enable_input(uint8_t pin)
{
  adc_set_input(pin, 1);
}

void adc_disable_input(uint8_t pin)
{
  adc_set_input(pin, 0);
}

void adc_set_input(uint8_t pin, uint8_t state)
{
  if(state == 1)
  {
    switch(pin)
    {
      case PINA0:
      case PINA1:
      case PINA2:
      case PINA3:
      case PINA4:
      case PINA5:
      case PINA6:
      case PINA7:
        adc_set_mux(pin, pin);
        DIDR0 |= (1 << pin);
        break;
      default:
        break;
    }
  }
  else
  {
    switch(pin)
    {
      case PINA0:
      case PINA1:
      case PINA2:
      case PINA3:
      case PINA4:
      case PINA5:
      case PINA6:
      case PINA7:
        adc_set_mux(pin, 0x0);
        DIDR0 &= ~(1 << pin);
        break;
      default:
        break;
    }
  }
}

void adc_set_mux(uint8_t channel, uint8_t mux)
{
  if(channel >= 8) return;
    adc0.mux[channel] = mux;
}

void adc_start()
{
  adc0.curchn = 0;  
  while(adc0.curchn <= 8)
  {
    if(adc0.mux[adc0.curchn] != 0)
    {
      // Start the first conversion 
      ADMUX  = adc0.refsrc | adc0.mux[adc0.curchn];
      ADCSRA |= (1 << ADSC);
      break;
    }
    adc0.curchn++;
  }
}

void adc_stop()
{

}

uint16_t adc_get(uint8_t index)
{
  return adc0.result[index];
}


ISR(ADC_vect)
{
  adc0.result[adc0.curchn] = ADC;
  do{
    adc0.curchn++;
    if(adc0.mux[adc0.curchn] != 0)
    {
      // Start the next conversion 
      ADMUX  = adc0.refsrc | adc0.mux[adc0.curchn];
      ADCSRA |= (1 << ADSC);
      break;
    }
  } while(adc0.curchn <= 8);
}


