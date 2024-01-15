#include <Arduino.h>
#include "white-display.h"

const byte WhiteDisplay::segments[] = { 1, 2, 4, 8, 16, 32, 64, 128 };
const byte WhiteDisplay::map[] = {238, 40, 182, 188, 120, 220, 222, 168, 254, 252, 0};

WhiteDisplay::WhiteDisplay()
{
  reset();
}

void WhiteDisplay::reset()
{
  animated = false;
  numbers[0] = 1;
  numbers[1] = 2;
  numbers[2] = 3;
  numbers[3] = 4;
  shuffle();
}

byte WhiteDisplay::display(byte i)
{
  if (animated)
  {
    int progress = millis() % 1200;
    if (progress < 100)
    {
      return i == 0 ? segments[7] : i == 3 ? segments[2] : 0;
    }
    else if (progress < 200)
    {
      return i == 1 ? segments[7] : i == 2 ? segments[2] : 0;
    }
    else if (progress < 300)
    {
      return i == 2 ? segments[7] : i == 1 ? segments[2] : 0;
    }
    else if (progress < 400)
    {
      return i == 3 ? segments[7] : i == 0 ? segments[2] : 0;
    }
    else if (progress < 500)
    {
      return i == 3 ? segments[5] : i == 0 ? segments[1] : 0;
    }
    else if (progress < 600)
    {
      return i == 3 ? segments[3] : i == 0 ? segments[6] : 0;
    }
    else if (progress < 700)
    {
      return i == 3 ? segments[2] : i == 0 ? segments[7] : 0;
    }
    else if (progress < 800)
    {
      return i == 2 ? segments[2] : i == 1 ? segments[7]: 0;
    }
    else if (progress < 900)
    {
      return i == 1 ? segments[2] : i == 2 ? segments[7]: 0;
    }
    else if (progress < 1000)
    {
      return i == 0 ? segments[2] : i == 3 ? segments[7]: 0;
    }
    else if (progress < 1100)
    {
      return i == 0 ? segments[1] : i == 3 ? segments[5]: 0;
    }
    else
    {
      return i == 0 ? segments[6] : i == 3 ? segments[3]: 0;
    }
  }
  
  return map[numbers[i]];
}

void WhiteDisplay::shuffle()
{
  animated = false;
  byte temp;
  for (short i = 0; i < 4; i++)
  {
    temp = numbers[i];
    short r = random(0, 4);
    numbers[i] = numbers[r];
    numbers[r] = temp;
  }
}

byte WhiteDisplay::find(byte num)
{
  for (short i = 0; i < 4; i++)
  {
    if (numbers[i] == num)
    {
      return i;
    }
  }
  return -1;
}

byte WhiteDisplay::label(byte num)
{
  return numbers[num];
}

void WhiteDisplay::clear()
{
  animated = false;
  numbers[0] = 10;
  numbers[1] = 10;
  numbers[2] = 10;
  numbers[3] = 10;
}

void WhiteDisplay::animate()
{
  animated = true;
}