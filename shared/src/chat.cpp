#include <Arduino.h>
#include "chat.h"

#define MESSAGE_HEADER 30

Chat::Chat(ChatSource id)
{
  me = (short)id;
  pinMode(1, INPUT);
  pinMode(0, OUTPUT);

  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    serial = &Serial1;
  #else
    serial = &Serial;
  #endif
}

void Chat::begin()
{
  serial->begin(19200);
}

void Chat::send(MessageType message, uint8_t data)
{
  uint8_t buf[4] = { MESSAGE_HEADER, me, (short)message, data };
  write(buf, 4);

  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

    // Convert to printable characters
    uint8_t print_buf[4] = {
      buf[0] + 94,
      buf[1] + 48,
      buf[2] + 48,
      buf[3] + 48
    };

    Serial.print("->");
    Serial.write(print_buf, 4);
    Serial.println();

  #endif
  
}

bool Chat::receive(ChatMessage* message)
{
  if (serial->available() < 4)
  {
    return false;
  }

  uint8_t buf[4];
  serial->readBytes(buf, 4);

  if (buf[0] != MESSAGE_HEADER)
  {
    debug("RECOVER!\n" + String(buf[0]) + String(buf[1]) + String(buf[2]) + String(buf[3]));
    if (!recover(buf))
    {
      return false;
    }
  }

  message->sender = buf[1];
  message->message = (MessageType)buf[2];
  message->data = buf[3];

  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    uint8_t print_buf[4] = {
      buf[0] + 94,
      buf[1] + 48,
      buf[2] + 48,
      buf[3] + 48
    };

    Serial.print("<-");
    Serial.write(print_buf, 4);
    Serial.println();
  #endif


  // If it's a debug message, sent by me, drain the bytes from the queue
  if (message->message == MessageType::Debug)
  {
    // forward the initial message
    write(buf, 4);

    // process the extra data
    return handleDebugMessage(message);
  }

  else if (message->sender == me)
  {
    return false;
  }

  else if (message->sender < 0 || message->sender >= (short)ChatSource::LAST_VALUE 
    || (short)message->message < 0 || (short)message->message >= (short)MessageType::LAST_VALUE)
  {
    // bad message, clear the buffer
    
    #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    Serial.println("*** BUFFER ISSUE ***");
    #endif

    while (serial->available() >= 1)
    {
      serial->readBytes(buf, 1);
    }
    return false;
  }

  // forward message
  write(buf, 4);

  return true;  
}

bool Chat::handleDebugMessage(ChatMessage *message)
{
  const short length = message->data;
  char buffer[length+1];
  serial->readBytes(buffer, length);
  
  if (message->sender == me)
  {
    return false;
  }

  // forward the debug data
  write(buffer, length);

  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

  // terminate the string so it prints nicely
  buffer[length] = NULL;

  Serial.print(message->sender);
  Serial.print(": ");
  Serial.println(buffer);

  #endif

  return false;
}

void Chat::debug(String s)
{
  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  
  Serial.print(s);
  return;

  #endif

  send(MessageType::Debug, s.length());
  Serial.print(s);
}

bool Chat::recover(uint8_t *buf)
{
  while (buf[0] != MESSAGE_HEADER && serial->available())
  {
    buf[0] = buf[1];
    buf[1] = buf[2];
    buf[2] = buf[3];
    buf[3] = serial->read();
  }

  return buf[0] == MESSAGE_HEADER;
}

void Chat::write(uint8_t *buf, int len)
{
  serial->write(buf, len);
}