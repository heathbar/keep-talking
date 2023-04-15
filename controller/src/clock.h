#ifndef CLOCK_H
#define CLOCK_H

#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

struct TimeChanges
{
  short digits[4];
};

class Clock
{
  private:
    long length;
    long startTime;
    long remaining;
    short lastDisplay;
    short lastSound;
    short strikes;
    long strike1time = -1;
    long strike2time = -1;
    short digits[4];
    short prevDigits[4];
    Adafruit_7segment matrix;

  public:
    Clock(short seconds);
    void start();
    TimeChanges refresh();
    bool isExpired();
    void showColon(bool flag);
    void strike();
};
#endif