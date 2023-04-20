#ifndef HAND_H
#define HAND_H

#include <NeoPixelBus.h>
#include "button.h"
#include "encoder.h"
#include "light.h"

struct HandState
{
  bool mode;
  short value;
};

class Hand
{
  private:
    Button button;
    Encoder encoder;
    bool mode;
    short value;
    RgbColor color;
    bool changed;

  public:
    Hand(short btnPin, short clockPin, short dataPin, RgbColor c, short initialValue);
    HandState update();
    bool hasChanged();
    short getValue();
    RgbColor getColor();
    void setTime(short t);
};

#endif
