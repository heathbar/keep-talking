#include <Arduino.h>
#include "password-component.h"
#include "password-slice.h"

PasswordComponent::PasswordComponent(const char **args):
  answer(args[0]),
  slice1(args[0][0], args[1], 3),
  slice2(args[0][1], args[2], 4),
  slice3(args[0][2], args[3], 5),
  slice4(args[0][3], args[4], 6)
{
}

void PasswordComponent::mix()
{
  slice1.mix();
  slice2.mix();
  slice3.mix();
  slice4.mix();
}
bool PasswordComponent::eval(char *buf)
{
  buf[0] = slice1.eval();
  buf[1] = slice2.eval();
  buf[2] = slice3.eval();
  buf[3] = slice4.eval();
  buf[4] = '\0';

  if (buf[0] == answer[0] &&
      buf[1] == answer[1] &&
      buf[2] == answer[2] &&
      buf[3] == answer[3])
  {
    return true;
  }
  return false;
}

const char* PasswordComponent::getAnswer()
{
  return answer;
}
