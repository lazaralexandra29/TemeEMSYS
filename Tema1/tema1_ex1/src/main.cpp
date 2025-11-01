#include <avr/io.h>
#include <util/delay.h>

constexpr uint8_t LEDS_PORTB_EVEN = (1 << PB0) | (1 << PB2) | (1 << PB4);
constexpr uint8_t LEDS_PORTD_EVEN = (1 << PD4) | (1 << PD6);

constexpr uint8_t LEDS_PORTB_ODD  = (1 << PB1) | (1 << PB3) | (1 << PB5);
constexpr uint8_t LEDS_PORTD_ODD  = (1 << PD5) | (1 << PD7);

void initLEDs() 
{
  DDRB = 0xFF;
  DDRD = 0xFF;
  PORTB = 0x00;
  PORTD = 0x00;
}

void setEvenLEDs() 
{
  PORTB = LEDS_PORTB_EVEN;
  PORTD = LEDS_PORTD_EVEN;
}

void setOddLEDs() 
{
  PORTB = LEDS_PORTB_ODD;
  PORTD = LEDS_PORTD_ODD;
}

int main() 
{
  initLEDs();

  while (1) 
  {
    setEvenLEDs();
    _delay_ms(500);

    setOddLEDs();
    _delay_ms(500);
  }

  return 0;
}