#ifndef ColorButton_h
#define ColorButton_h

#include "Arduino.h"

class ColorButton
{
public:
  ColorButton(char id, short ledPin, short btnPin, int tone);
  char id;
  short ledPin;
  short btnPin;
  short btnState;
  bool isPressed;
  bool isReleased;
  void turnOn();
  void turnOnMuted();
  void turnOff();
  void refresh();
  void enableSound();

private:
  short lastState;
  long lastDebounceTime;
  int toneHertz;
  bool isOn;
  bool soundEnabled;
};

#endif