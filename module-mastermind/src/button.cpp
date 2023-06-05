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

ButtonState Button::refresh()
{
  this->isPressed = false;
  this->isReleased = false;

  bool reading = digitalRead(this->pin);
  if (reading != this->lastState)
  {
    this->lastDebounceTime = millis();
  }
  
  this->lastState = reading;

  if ((millis() - this->lastDebounceTime) > debounceTime)
  {
    if (reading != this->state)
    {
      this->state = reading;

      if (this->state == LOW)
      {
        return PRESSED;
      }
      else
      {
        return RELEASED;
      }
    }
  }
  
  return NONE;
}