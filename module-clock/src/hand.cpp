#include <Arduino.h>
#include <NeoPixelBus.h>
#include "hand.h"
#include "button.h"
#include "encoder.h"
#include "light.h"

RgbColor black(0);

Hand::Hand(short btnPin, short clockPin, short dataPin, RgbColor c, short initialValue): button(btnPin), encoder(clockPin, dataPin)
{
  mode = 0;
  value = initialValue;
  color = c;

  // read once to clear out the inital state
  encoder.read();
}

HandState Hand::update()
{
  changed = false;

  if (button.isPressed())
  {
    mode = !mode;
    if (mode == 1)
    {
      changed = true;
    }
  }

  if (mode == 0)
  {
    return { mode, value };
  }
  else
  {
    short prev = value;
    value += encoder.read();

    if (value == -1)
    {
      value = 11;
    }
    else if (value == 12)
    {
      value = 0;
    }

    if (prev != value)
    {
      changed = true;
    }
    
    return { mode, value };
  }
}

bool Hand::hasChanged()
{
  return changed;
}

short Hand::getValue()
{
  return value;
}

RgbColor Hand::getColor()
{
  return (mode == 1 && millis() % 300 > 150) ? black : color;
}

void Hand::setTime(short t)
{
  mode = 0;
  value = t;
}