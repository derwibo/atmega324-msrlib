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
  uint8_t i;

  adc0.enabled = 0;
  for(i=0; i<8; i++)
  {
    adc0.result[i] = 0;
    adc0.mux[i] = i;
  }
  adc0.curchn = 0;

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
        adc0.enabled |= (1 << pin);
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
        adc0.enabled &= ~(1 << pin);
        DIDR0 &= ~(1 << pin);
        break;
      default:
        break;
    }
  }
}

void adc_set_mux(uint8_t channel, uint8_t mux)
{
  if(channel < 8)
  {
    adc0.mux[channel] = mux & 0b11111;
  }
}

void adc_start()
{
  adc0.curchn = 0;  
  while(adc0.curchn < 8)
  {
    if(adc0.enabled & (1 << adc0.curchn))
    {
      // Start the first conversion 
      ADMUX  = adc0.refsrc | adc0.mux[adc0.curchn];
      ADMUX  = adc0.refsrc;
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
  uint16_t result = ADC;
  adc0.result[adc0.curchn] = result;
  do{
    adc0.curchn++;
    if(adc0.enabled & (1 << adc0.curchn))
    {
      // Start the next conversion 
      ADMUX  = adc0.refsrc | adc0.mux[adc0.curchn];
      ADCSRA |= (1 << ADSC);
      break;
    }
  } while(adc0.curchn < 8);
}


