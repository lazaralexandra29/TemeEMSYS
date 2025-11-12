#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

constexpr uint8_t LED0_PIN = PD4;
constexpr uint8_t LED1_PIN = PD5;

constexpr uint16_t LOW_THRESHOLD    = 307; 
constexpr uint16_t HIGH_THRESHOLD   = 716; 

volatile bool adcEnabled = false;

void initUART() 
{
  UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
  UBRR0L = (uint8_t)(UBRR_VALUE);
  UCSR0B = (1 << RXEN0) | (1 << TXEN0); 
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uartTransmit(char c) 
{
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = c;
}

void uartSendString(const char* str) 
{
  while (*str) uartTransmit(*str++);
}

void uartReceiveLine(char* buffer, uint8_t maxLength) 
{
  uint8_t i = 0;
  while (i < maxLength - 1) 
  {
    while (!(UCSR0A & (1 << RXC0)));
    char c = UDR0;
    if (c == '\n' || c == '\r') break;
    buffer[i++] = c;
  }
  buffer[i] = '\0';
}

void initLEDs() 
{
  DDRD |= (1 << LED0_PIN) | (1 << LED1_PIN);
  PORTD &= ~((1 << LED0_PIN) | (1 << LED1_PIN));
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

void updateLEDs(uint16_t adcValue) 
{
  if (adcValue < LOW_THRESHOLD) 
  {
    PORTD |=  (1 << LED0_PIN);
    PORTD &= ~(1 << LED1_PIN);
    uartSendString("Voltage is LOW\r\n");
  } 
  else if (adcValue < HIGH_THRESHOLD) 
  {
    PORTD &= ~(1 << LED0_PIN);
    PORTD &= ~(1 << LED1_PIN);
    uartSendString("Voltage is NORMAL\r\n");
  } else {
    PORTD |=  (1 << LED1_PIN);
    PORTD &= ~(1 << LED0_PIN);
    uartSendString("Voltage is HIGH\r\n");
  }
}

void processCommand(const char* cmd) 
{
  if (strcmp(cmd, "adc_on") == 0) 
  {
    adcEnabled = true;
    uartSendString("ADC started\r\n");
  } 
  else if (strcmp(cmd, "adc_off") == 0) 
  {
    adcEnabled = false;
    uartSendString("ADC stopped\r\n");
  }
  else if (strcmp(cmd, "adc_status") == 0) 
  {
    uartSendString(adcEnabled ? "ADC is ON\r\n" : "ADC is OFF\r\n");
  } 
  else if (strcmp(cmd, "help") == 0) 
  {
    uartSendString("Available commands:\r\n");
    uartSendString(" - adc_on\r\n");
    uartSendString(" - adc_off\r\n");
    uartSendString(" - adc_status\r\n");
    uartSendString(" - help\r\n");
  } 
  else 
  {
    uartSendString("Unknown command\r\n");
  }
}

int main() 
{
  initUART();
  initADC();
  initLEDs();

  char commandBuffer[32];

  while (1) 
  {
    uartReceiveLine(commandBuffer, sizeof(commandBuffer));
    processCommand(commandBuffer);

    if (adcEnabled) 
    {
      uint16_t adcValue = readADC();
      updateLEDs(adcValue);
      _delay_ms(1000);
    }
  }

  return 0;
}