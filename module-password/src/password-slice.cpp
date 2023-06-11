#include "password-slice.h"
#include <Arduino.h>

const short SIZE = 6;

void PasswordSlice::init(char ans, char noise[], short btnPin)
{
  answer = ans;
  index = 0;

  chars[0] = ans;
  strncpy(&chars[1], noise, 6);
  
  btn = Button(btnPin);
}

void PasswordSlice::mix()
{
  // shuffle the character chars
  for (short i = 0; i < SIZE - 1; i++) {
    short j = random() % SIZE;
    char temp = chars[j];
    chars[j] = chars[i];
    chars[i] = temp;
  }
}
void PasswordSlice::next() 
{
  index++;
  if (index >= SIZE) {
    index = 0;
  }
}
void PasswordSlice::prev()
{
  index--;
  if (index < 0) {
    index = SIZE - 1;
  }
}

char PasswordSlice::eval()
{
  if (btn.isPressed()) {
    next();
  }
  if (btn.isPressed()) {
    prev();
  }
  return chars[index];
}
