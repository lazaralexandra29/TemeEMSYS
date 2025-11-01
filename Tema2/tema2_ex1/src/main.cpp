#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000UL

constexpr uint8_t BUTTON1_PIN = PD2;
constexpr uint8_t LED0_PIN    = PD4;
constexpr uint8_t LED1_PIN    = PD5;

void pinToggle(volatile uint8_t* port, uint8_t pin) 
{
  *port ^= (1 << pin);
}

void setPin(volatile uint8_t* port, uint8_t pin) 
{
  *port |= (1 << pin);
}

void resetPin(volatile uint8_t* port, uint8_t pin) 
{
  *port &= ~(1 << pin);
}

void initDevices() 
{
  DDRD |= (1 << LED0_PIN) | (1 << LED1_PIN);

  DDRD &= ~(1 << BUTTON1_PIN);      
  PORTD |= (1 << BUTTON1_PIN);     

  EICRA |= (1 << ISC00);            
  EIMSK |= (1 << INT0);           

  sei(); 
}

ISR(INT0_vect) 
{
  if ((PIND & (1 << BUTTON1_PIN)) == 0) 
  {
    setPin(&PORTD, LED1_PIN);
  } 
  else 
  {
    resetPin(&PORTD, LED1_PIN);
  }
}

int main() 
{
  initDevices();

  while (1) 
  {
    pinToggle(&PORTD, LED0_PIN);
    _delay_ms(3000);
  }

  return 0;
}