#include <Arduino.h>
#include "chat.h"

// Uncomment to adjust serial output format
// #define USB_DEBUG 1

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
  uint8_t buf[3];
  buf[0] = me;
  buf[1] = (short)message;
  buf[2] = data;

  serial->write(buf, 3);

  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

    buf[0] += 48;
    buf[1] += 48;
    buf[2] += 48;

    Serial.print("->");
    Serial.write(buf, 3);
    Serial.println();

  #endif
  
}

bool Chat::receive(ChatMessage* message)
{
  if (serial->available() < 3)
  {
    return false;
  }

  uint8_t buf[3];
  serial->readBytes(buf, 3);

  message->sender = buf[0];
  message->message = (MessageType)buf[1];
  message->data = buf[2];

  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    uint8_t print_buf[3];
    print_buf[0] = buf[0] + 48;
    print_buf[1] = buf[1] + 48;
    print_buf[2] = buf[2] + 48;

    Serial.print("<-");
    Serial.write(print_buf, 3);
    Serial.println();
  #endif


  // If it's a debug message, sent by me, drain the bytes from the queue
  if (message->message == MessageType::Debug)
  {
    // forward the initial message
    serial->write(buf, 3);

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
  serial->write(buf, 3);

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
  serial->write(buffer, length);

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
  send(MessageType::Debug, s.length());
  Serial.print(s);
}