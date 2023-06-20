#ifndef BUTTON_H
#define BUTTON_H


enum ButtonState
{
  NONE,
  PRESSED,
  RELEASED
};

class Button {
  private:
    short pin;
    bool isPressed;
    bool isReleased;
    bool state;
    bool lastState;
    long lastDebounceTime;

  public:
    Button();
    Button(short pin);
    ButtonState refresh();
};
#endif
