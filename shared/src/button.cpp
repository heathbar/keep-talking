#include "Arduino.h"
#include "button.h"

#define debounceTime 25

Button::Button()
{
  // this is mostly pointless
}

Button::Button(short pin)
{
  this->pin = pin;
  this->state = false;
  this->lastState = false;
  this->lastDebounceTime = 0;
  pinMode(pin, INPUT_PULLUP);
}

bool Button::isPressed()
{
  bool reading = digitalRead(this->pin);
  if (reading != this->lastState)
  {
    this->lastDebounceTime = millis();
  }
  if ((millis() - this->lastDebounceTime) > debounceTime)
  {
    if (reading != this->state)
    {
      this->state = reading;

      if (this->state == LOW)
      {
        this->lastState = reading;
        return true;
      }
    }
  }
  this->lastState = reading;
  return false;
}