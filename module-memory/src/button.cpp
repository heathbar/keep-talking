#include "Arduino.h"
#include "button.h"

#define debounceTime 25

Button::Button()
{
  // this is mostly pointless
}

Button::Button(short p)
{
  pin = p;
  pinMode(pin, INPUT_PULLUP);

  lastDebounceTime = 0;
  state = lastState = digitalRead(pin);
}

ButtonState Button::refresh()
{
  isPressed = false;
  isReleased = false;

  bool reading = digitalRead(pin);
  if (reading != lastState)
  {
    lastDebounceTime = millis();
  }
  
  lastState = reading;

  if ((millis() - lastDebounceTime) > debounceTime)
  {
    if (reading != state)
    {
      state = reading;

      if (state == LOW)
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