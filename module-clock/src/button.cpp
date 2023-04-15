#include "Arduino.h"
#include "button.h"

#define DEBOUNCE_TIME 25

Button::Button(short p)
{
  pin = p;
  state = true;
  lastState = false;
  lastDebounceTime = 0;
  pinMode(pin, INPUT_PULLUP);
}

void Button::refresh()
{
  _isPressed = false;
  _isReleased = false;

  bool reading = digitalRead(pin);
  if (reading != lastState)
  {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > DEBOUNCE_TIME)
  {
    if (reading != state)
    {
      state = reading;

      if (state == LOW)
      {
        lastState = reading;
        lastPressedTime = millis();
        _isPressed = true;
      }
      else
      {
        lastState = reading;
        _isReleased = true;
      }
    }
  }
  lastState = reading;
}


bool Button::isPressed()
{
  refresh();
  return _isPressed;
}

bool Button::isReleased()
{
  refresh();
  return _isReleased;
}

bool Button::isLongPressed()
{
  refresh();
  return state == false && millis() - lastPressedTime > 1500;
}
