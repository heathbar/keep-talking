#ifndef WHITE_DISPLAY_H
#define WHITE_DISPLAY_H

#include "Arduino.h"

class WhiteDisplay
{
  public:
    WhiteDisplay();
    void reset();
    byte display(byte i);
    void shuffle();
    byte find(byte num);
    byte label(byte num);
    void clear();
    void animate();

  private:
    static const byte map[11];
    static const byte segments[8];
    byte numbers[4];
    bool animated;

};

#endif