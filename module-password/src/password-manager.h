#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H

#include <Arduino.h>
#include "password-slice.h"

class PasswordManager
{
  private:
    bool wait;
    char answer[5];

    PasswordSlice slice1;
    PasswordSlice slice2;
    PasswordSlice slice3;
    PasswordSlice slice4;

  public:
    PasswordManager();
    void load(short num, bool alt);
    void mix();
    bool eval(char buf[4]);
    const char* getAnswer();
};

#endif