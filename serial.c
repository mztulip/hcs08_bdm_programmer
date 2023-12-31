#include <stdio.h>
#include <avr/io.h>
#include "serial.h"


void serial_init(void)
{
    //Arduino Atmega2560 uses PE0(RX0) i PE1(TX0)
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // set character data size to 8
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

    //9600, crystal 16Mhz UBRR = 16000000/(16*baudrate)-1
    UBRR0 = 103;
}

uint8_t serial_receive(void)
{
    while(!(UCSR0A & (1<<RXC0)));
    return UDR0;
}

int serial_send( char data, FILE *stream)
{
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = data;
    return 0;
}

void serial_send_buffer(char *buffer, uint8_t len)
{
    for(int i = 0; i< len; i++)
    {
        serial_send(buffer[i], NULL);
    }
}

uint8_t serial_read_line(char *line_buffer)
{
  //Line buffer len=255
  int i = 0;
  while(1)
  {
      char c = serial_receive();
      if(c == '\n')
      {
          return i;
      }
      line_buffer[i] = c;
      i++;
      if(i == 255)
      {
        printf("\n\033[31mLine buffer is full\033[0m");
      }
  }
}