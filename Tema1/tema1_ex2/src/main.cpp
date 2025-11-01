#include <avr/io.h>
#include <util/delay.h>

constexpr uint8_t START_MASK = 0b00000011; 
constexpr uint8_t MAX_MASK   = 0b11000000; 

void initLEDs() 
{
  DDRC = 0xFF;     
  PORTC = 0x00;   
}

void setLEDs(uint8_t mask) 
{
  PORTC = mask;
}

void shiftRight() 
{
  uint8_t mask = START_MASK;
  while (mask <= MAX_MASK) 
  {
    setLEDs(mask);
    _delay_ms(500);
    mask <<= 1;
  }
}

void shiftLeft() 
{
  uint8_t mask = MAX_MASK;
  while (mask >= START_MASK) 
  {
    setLEDs(mask);
    _delay_ms(500);
    mask >>= 1;
  }
}

int main() 
{
  initLEDs();

  while (1) 
  {
    shiftRight();
    shiftLeft();
  }

  return 0;
}