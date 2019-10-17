/* Testprogramm
   Ausgang PD7 wird in Abhaengigkeit von PC0 geschaltet
   Ausgang PD6 wird in Abhaengigkeit von PC1 geschaltet
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#include "at324hw.h"
#include "scheduler.h"
#include "usart.h"
#include "adc.h"
#include "pwm.h"

#define BAUDRATE 9600

struct uart_msg
{
  uint8_t id;
  uint8_t nb;
  uint8_t data[8];
};

typedef struct uart_msg uart_msg_t;

uart_msg_t datamsg;

unsigned char pwm2a;
unsigned char pwm2b;

int8_t usart0_transmit_msg(uart_msg_t* msg);

void transmit_func(void)
{
  int8_t i;
  uint16_t adcresult;

  datamsg.id = 0x12;
  datamsg.nb = 8;

  for(i=0 ; i<8 ; i++)
  {
    datamsg.data[i] = 0;
  }

  datamsg.data[0] = PINC;

  datamsg.data[1] = 0;

//  datamsg.data[2] = pwm2a;
//  datamsg.data[3] = pwm2b;
  datamsg.data[1] = adc0.mux[0];
  datamsg.data[2] = adc0.mux[1];
  datamsg.data[3] = adc0.mux[2];

  adcresult = adc_get(PINA0);
  datamsg.data[4] = (uint8_t)((adcresult >> 8) & 0xFF);
  datamsg.data[5] = (uint8_t)((adcresult) & 0xFF);

  adcresult = adc_get(PINA1);
  datamsg.data[6] = (uint8_t)((adcresult >> 8) & 0xFF);
  datamsg.data[7] = (uint8_t)((adcresult) & 0xFF);

  usart0_transmit_msg(&datamsg);
}

void blink_func(void)
{
  if(PORTD & (1<<PORTD5))
  {
    PORTD &= ~(1<<PORTD5);
  }
  else
  {
    PORTD |= (1<<PORTD5);
  }
}

void input_func(void)
{
  pwm2a = 0;
  pwm2b = 0;

  if(PINC & (1 << PINC6))
  {
    pwm2b = 63;
  }

  if(PINC & (1 << PINC5))
  {
    pwm2b = 127;
  }

  if(PINC & (1 << PINC4))
  {
    pwm2b = 191;
  }

  if(PINC & (1 << PINC3))
  {
    pwm2b = 255;
  }

  pwm_set(PWM2A, pwm2a);
  pwm_set(PWM2B, pwm2b);
}

int main(void)
{
  uint8_t i;
//  at324_prescaler_init();

  CLKPR = 0x80;
  CLKPR = 0x00;

  __asm__ __volatile__ ("nop");
  __asm__ __volatile__ ("nop");
  __asm__ __volatile__ ("nop");

  at324_init();

/* CONFIGURE I/O PINS, all unused Port pins as input with pull up */

  DDRA  = 0b00000000;  // PA0 - PA3 : Analogeingaenge
//  PORTA = 0b11111111;
  PORTA = 0b00000000;  

  DDRB  = 0b00000000;
  PORTB = 0b11111111;

  DDRC  = 0b00000000;  // Digitale Eingaenge
  PORTC = 0b00000000;  

  DDRD  = 0b11100010;
  PORTD = 0b00011100;

  DIDR0 = 0b11111111;

  scheduler_init();
  usart0_init(BAUDRATE);

  adc_init(ADC_REF_AVCC);
  adc_enable_input(PINA0);
  adc_enable_input(PINA1);

  pwm_init();

  task_create(adc_start, 100, TASK_RUNNING);
  task_create(transmit_func, 1000, TASK_RUNNING);
  task_create(blink_func, 500, TASK_RUNNING);
  task_create(input_func, 10, TASK_RUNNING);

  sei();

  while(1)
  {
    scheduler_run();
  }

  return 0;
}


inline int8_t usart0_transmit_msg(uart_msg_t* msg)
{
  uint8_t* dataptr = (uint8_t*)msg;
  int8_t nb = msg->nb + 2;
  if(usart0_transmit(dataptr, nb) > 0)
  {
    return 1;
  }
  return 0;
}


ISR(TIMER0_OVF_vect)
{

}



