#include <Arduino.h>
#include "patient-chat.h"

#define MESSAGE_HEADER 30

PatientChat::PatientChat(ChatSource id, short rtsPin, short ctsPin): Chat(id)
{
  rts = rtsPin;
  cts = ctsPin;
  pinMode(rts, OUTPUT);
  pinMode(cts, INPUT);
}

void PatientChat::write(uint8_t *buf, int len)
{
  digitalWrite(rts, HIGH);
  
  short count = 0;
  while(digitalRead(cts) == LOW && count < 100)
  {
    count++;
    delay(1);
  }

  if (count < 100)
  {
    Chat::write(buf, len);
  }
  
  digitalWrite(rts, LOW);
}

