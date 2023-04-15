#include "password-slice.h"
#include <Arduino.h>

PasswordSlice::PasswordSlice(char ans, const char *noise, short btnPin)
{
  answer = ans;
  index = 0;
  size = strlen(noise) + 1; // add an extra spot for the correct answer
  list = (char *)malloc(size + 1);  // add an extra spot for the null
  
  list[0] = ans;
  strcpy(&list[1], noise);

  btn = Button(btnPin);
}

void PasswordSlice::mix()
{
  // shuffle the character list
  for (short i = 0; i < size - 1; i++) {
    short j = random() % size;
    char temp = list[j];
    list[j] = list[i];
    list[i] = temp;
  }
}
void PasswordSlice::next() 
{
  index++;
  if (index >= size) {
    index = 0;
  }
}
void PasswordSlice::prev()
{
  index--;
  if (index < 0) {
    index = size - 1;
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
  return list[index];
}

PasswordSlice::~PasswordSlice()
{
  free(list);
}