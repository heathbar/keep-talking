#include "password-manager.h"
#include "password-slice.h"
#include "passwords.h"

PasswordManager::PasswordManager()
{
  wait = true;
}

void PasswordManager::load(short num, bool alt)
{
  char noise[6];
  int offset = num * 5;
  const char * const * db = alt ? db2 : db1;

  strcpy_P(answer, (char*)pgm_read_word(&(db[offset + 0])));

  strcpy_P(noise, (char*)pgm_read_word(&(db[offset + 1])));
  slice1.init(answer[0], noise, 3);
  
  strcpy_P(noise, (char*)pgm_read_word(&(db[offset + 2])));
  slice2.init(answer[1], noise, 4);
  
  strcpy_P(noise, (char*)pgm_read_word(&(db[offset + 3])));
  slice3.init(answer[2], noise, 5);
  
  strcpy_P(noise, (char*)pgm_read_word(&(db[offset + 4])));
  slice4.init(answer[3], noise, 6);
}


void PasswordManager::mix()
{
  slice1.mix();
  slice2.mix();
  slice3.mix();
  slice4.mix();

  wait = false;
}

bool PasswordManager::eval(char *buf)
{
  if (wait)
  {
    buf[0] = 'W';
    buf[1] = 'A';
    buf[2] = 'I';
    buf[3] = 'T';
    buf[4] = '\0';  
    return false;
  }

  buf[0] = slice1.eval();
  buf[1] = slice2.eval();
  buf[2] = slice3.eval();
  buf[3] = slice4.eval();
  buf[4] = '\0';

  short correct_letters = 0;

  for (short i = 0; i < 4; i++)
  {
    if (buf[i] == answer[i])
    {
      correct_letters++;
    }
  }
  if (correct_letters > 1)
  {
    return true;
  }
  return false;
}

const char* PasswordManager::getAnswer()
{
  return answer;
}
