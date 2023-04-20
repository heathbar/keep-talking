#include <Arduino.h>
#include "encoder.h"

Encoder::Encoder(short clockPin, short dataPin): clockPin(clockPin), dataPin(dataPin)
{
  pinMode(clockPin, INPUT_PULLUP);
  pinMode(dataPin, INPUT_PULLUP);
}

short Encoder::read()
{
  short change = 0;
  int value = digitalRead(clockPin);
  if (last == LOW && value == HIGH)
  {
    if (digitalRead(dataPin) == LOW)
    {
      change--;
    }
    else
    {
      change++;
    }
  }
  last = value;
  return change;
}
