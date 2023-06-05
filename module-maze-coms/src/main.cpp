#include <Arduino.h>
#include <SPI.h>
#include <chat.h>

#define I2C_ADDR 2

Chat chat(ChatSource::Maze);
ChatMessage msg;

ChatMessage queue[8];
short write_index = 0;
short read_index = 0;
int messages_received = 0;
int messages_sent = 0;

volatile short transmit_index = 0;
volatile byte command = 0;

void setup()
{
  chat.begin();
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCK, INPUT);
  pinMode(SS, INPUT);

  SPCR |= _BV(SPE);

  SPI.attachInterrupt();
}


ISR (SPI_STC_vect)
{
  byte c = SPDR;

  if (command == 0)
  {
    command = c;
  }

  if (command == 'm') // send messages
  {
    if (messages_received <= messages_sent)
    {
      command = 0;
      SPDR = 0;
    }
    else if (transmit_index == 0)
    {
      SPDR = queue[read_index].sender;
      transmit_index++;
    }
    else if (transmit_index == 1)
    {
      SPDR = (uint8_t)queue[read_index].message;
      transmit_index++;
    }
    else
    {
      SPDR = queue[read_index].data;
      command = 0;
      transmit_index = 0;
      read_index++;
      if (read_index == 16)
      {
        read_index = 0;
      }
      messages_sent++;
    }
  }
  else if (command == 's') // strike
  {
    chat.send(MessageType::Strike);
    command = 0;
  }
  else if (command == 'd') // disarm
  {
    chat.send(MessageType::Disarm);
    command = 0;
  }
}


void loop()
{
  if (chat.receive(&msg))
  {
    switch (msg.message)
    {
      case MessageType::Reset:
      case MessageType::Detonate:
      case MessageType::Win:        

      
        queue[write_index].sender = msg.sender;
        queue[write_index].message = msg.message;
        queue[write_index].data = msg.data;

        write_index++;
        if (write_index == 16)
        {
          write_index = 0;
        }
        messages_received++;
        break;
    }
  }

  delay(1);
}
