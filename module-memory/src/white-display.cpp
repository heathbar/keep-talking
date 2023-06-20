#include <Arduino.h>
#include "white-display.h"

const byte WhiteDisplay::map[] = { 238, 40, 182, 188, 120, 220, 222, 168, 254, 252, 0 };

WhiteDisplay::WhiteDisplay()
{
    reset();
}

void WhiteDisplay::reset()
{
    numbers[0] = 1;
    numbers[1] = 2;
    numbers[2] = 3;
    numbers[3] = 4;
    shuffle();
}

byte WhiteDisplay::display(byte i)
{
    return WhiteDisplay::map[numbers[i]];
}

void WhiteDisplay::shuffle()
{
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
        if (numbers[i] == num) {
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
    numbers[0] = 10;
    numbers[1] = 10;
    numbers[2] = 10;
    numbers[3] = 10;
}