#ifndef BLUE_DISPLAY_H
#define BLUE_DISPLAY_H

#include "Arduino.h"

class BlueDisplay
{
  public:
    BlueDisplay();
    void reset();
    byte display();
    byte value();
    void next();
    void clear();

  private:
    static const byte map[11];
    byte number;
};

#endif