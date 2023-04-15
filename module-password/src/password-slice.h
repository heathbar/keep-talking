#ifndef PASSWORD_SLICE_H
#define PASSWORD_SLICE_H

#include <Arduino.h>
#include <button.h>

class PasswordSlice {
  private:
      char answer;
      
      short index;
      short size;
      Button btn;

  public:
    char* list;
    PasswordSlice(char answer, const char *noise, short btnPin);
    void mix();
    void next();
    void prev();
    char eval();
    ~PasswordSlice();
};
#endif