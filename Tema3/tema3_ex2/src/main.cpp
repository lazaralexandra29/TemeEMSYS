#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

void initUART() 
{
  UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
  UBRR0L = (uint8_t)(UBRR_VALUE);
  UCSR0B = (1 << TXEN0); 
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uartTransmit(char c) 
{
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = c;
}

void uartSendString(const char* str) 
{
  while (*str) 
  {
    uartTransmit(*str++);
  }
}

void initADC() 
{
  ADMUX = (1 << REFS0); 
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
}

uint16_t readADC() 
{
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));
  return ADC;
}

void sendVoltageMessage(uint16_t adcValue) 
{
  if (adcValue < 307) 
  {
    uartSendString("Voltage is LOW\r\n");
  } 
  else if (adcValue < 716) 
  {
    uartSendString("Voltage is NORMAL\r\n");
  } 
  else 
  {
    uartSendString("Voltage is HIGH\r\n");
  }
}

int main() 
{
  initUART();
  initADC();

  while (1) 
  {
    uint16_t adcValue = readADC();
    sendVoltageMessage(adcValue);
    _delay_ms(1000);
  }

  return 0;
}