#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL

constexpr uint8_t LED0_PIN = PD4;
constexpr uint8_t LED1_PIN = PD5;
constexpr uint8_t ADC_CHANNEL = 0; 

void initLEDs() 
{
  DDRD |= (1 << LED0_PIN) | (1 << LED1_PIN);
  PORTD &= ~((1 << LED0_PIN) | (1 << LED1_PIN)); 
}

void initADC() 
{
  ADMUX = (1 << REFS0) | ADC_CHANNEL; 
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
}

uint16_t readADC() 
{
  ADCSRA |= (1 << ADSC);              
  while (ADCSRA & (1 << ADSC));       
  return ADC;
}

void updateLEDs(uint16_t adcValue) 
{
  if (adcValue < 307) 
  {
    PORTD |=  (1 << LED0_PIN);
    PORTD &= ~(1 << LED1_PIN);
  } 
  else 
  {
    PORTD |=  (1 << LED1_PIN);
    PORTD &= ~(1 << LED0_PIN);
  }
}

int main() 
{
  initLEDs();
  initADC();

  while (1) 
  {
    uint16_t adcValue = readADC();
    updateLEDs(adcValue);
    _delay_ms(100); 
  }

  return 0;
}