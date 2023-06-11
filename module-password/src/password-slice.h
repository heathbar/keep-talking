#ifndef PASSWORD_SLICE_H
#define PASSWORD_SLICE_H

#include <Arduino.h>
#include <button.h>

class PasswordSlice {
  private:
      char answer;
      short index;
      Button btn;

  public:
    char chars[7];
    void init(char answer, char noise[], short btnPin);
    void mix();
    void next();
    void prev();
    char eval();
};
#endif