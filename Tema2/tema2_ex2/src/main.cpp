#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000UL

constexpr uint8_t BUTTON_OFF_PIN = PD2; 
constexpr uint8_t BUTTON_ON_PIN  = PD3; 

constexpr uint8_t LED0_PIN = PD4;
constexpr uint8_t LED1_PIN = PD5;
constexpr uint8_t LED2_PIN = PD6;
constexpr uint8_t LED3_PIN = PD7;

void turnOnLEDs() 
{
  PORTD |= (1 << LED0_PIN) | (1 << LED1_PIN) | (1 << LED2_PIN) | (1 << LED3_PIN);
}

void turnOffLEDs() 
{
  PORTD &= ~((1 << LED0_PIN) | (1 << LED1_PIN) | (1 << LED2_PIN) | (1 << LED3_PIN));
}

void initDevices() 
{
  DDRD |= (1 << LED0_PIN) | (1 << LED1_PIN) | (1 << LED2_PIN) | (1 << LED3_PIN);

  DDRD &= ~((1 << BUTTON_OFF_PIN) | (1 << BUTTON_ON_PIN));
  PORTD |= (1 << BUTTON_OFF_PIN) | (1 << BUTTON_ON_PIN);

  EICRA |= (1 << ISC00) | (1 << ISC10); 
  EIMSK |= (1 << INT0) | (1 << INT1);   

  sei(); 

  turnOnLEDs();
}

ISR(INT0_vect) 
{
  if ((PIND & (1 << BUTTON_OFF_PIN)) == 0) 
  {
    turnOffLEDs();
  }
}

ISR(INT1_vect) 
{
  if ((PIND & (1 << BUTTON_ON_PIN)) == 0) 
  {
    turnOnLEDs();
  }
}

int main() 
{
  initDevices();

  while (1) 
  {
  }

  return 0;
}