#ifndef BUTTON_H
#define BUTTON_H

class Button {
  private:
    short pin;
    bool state;
    bool lastState;
    long lastDebounceTime;

  public:
    Button();
    Button(short pin);
    bool isPressed();

};
#endif