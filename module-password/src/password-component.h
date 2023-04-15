#ifndef PASSWORD_COMPONENT_H
#define PASSWORD_COMPONENT_H

#include "password-slice.h"
#include <Arduino.h>

class PasswordComponent
{
private:
  const char* answer;
  PasswordSlice slice1;
  PasswordSlice slice2;
  PasswordSlice slice3;
  PasswordSlice slice4;

public:
  PasswordComponent(const char **args);
  void mix();
  bool eval(char buf[4]);
  const char* getAnswer();
};
#endif