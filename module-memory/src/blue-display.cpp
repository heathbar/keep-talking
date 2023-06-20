#include <Arduino.h>
#include "blue-display.h"

const byte BlueDisplay::map[] = { 4, 62, 130, 10, 56, 72, 64, 30, 0, 24, 255 };

BlueDisplay::BlueDisplay()
{
    reset();
}

void BlueDisplay::reset()
{
    next();
}

byte BlueDisplay::display()
{
    return BlueDisplay::map[number];
}

byte BlueDisplay::value()
{
    return number;
}

void BlueDisplay::next()
{
    number = random(1, 5);
}

void BlueDisplay::clear()
{
    number = 10;
}