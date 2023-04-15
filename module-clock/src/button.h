#ifndef BUTTON_H
#define BUTTON_H

class Button {
  private:
    short pin;
    bool state;
    bool lastState;
    long lastDebounceTime;
    long lastPressedTime;
    bool _isPressed;
    bool _isReleased;

  public:
    Button(short pin);
    void refresh();
    bool isPressed();
    bool isReleased();
    bool isLongPressed();

};
#endif