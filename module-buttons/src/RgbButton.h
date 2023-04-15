#ifndef RgbButton_h
#define RgbButton_h

#include "Arduino.h"
#include "Tlc5940.h"
#include <chat.h>

enum Color
{
  black,
  red,
  green,
  blue,
  cyan,
  magenta,
  yellow,
  white
};

enum AnimationType
{
  flash1x,
  flash2x,
  alternate,
  solid
};

enum RgbButtonEvent
{
  none,
  stricken,
  solved
};

enum Solution
{
  PressOnGreen,
  PressTwice,
  PressThrice,
  HoldTwoSeconds,
  HoldThreeSeconds,
  ReleaseOnTime,
  Skip
};

struct Animation
{
  AnimationType type;
  Color color1;
  Color color2;
  int duration;
  Solution solution;
};

struct RgbLed
{
  short redPin;
  short grnPin;
  short bluPin;
  int redVal;
  int grnVal;
  int bluVal;
};

class RgbButton
{
  public:
    RgbButton(short red, short green, short blue, short btn, Chat *c);
    Chat *chat;
    bool isPressed;
    bool isReleased;
    Solution solution;
    void setAnimation(short index);
    RgbButtonEvent refresh();
    void setColor(Color c, int value = 4095);
    void setDigit(short index, uint8_t time);

  private:
    const static struct Animation animations[];
    static Tlc5940 *tlc;

    short r;
    short g;
    short b;
    short btnPin;
    short btnState;
    short lastState;
    long lastDebounceTime;
    long lastPressedTime;
    long lastReleasedTime;
    RgbLed led;
    Animation animation;
    bool solved;          // whether or not the solution for the button has been met
    short pressedCount;   // Number of times button was pressed when Solution is PressXTimes
    Color releaseColor;   // The color to change to when Solution is ReleaseOnTime
    uint8_t releaseTime;  // The time needed to release when Solution is ReleaseOnTime
    bool digitsSet;
    uint8_t time[4];

    void flash1x(long now);
    void flash2x(long now);
    void alternate(long now);
    void solid(long now);
    RgbButtonEvent handlePressed();
    RgbButtonEvent handleReleased();
    
};

#endif