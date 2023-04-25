#include <Arduino.h>
#include "number-sequence.h"


NumberSequence::NumberSequence()
{
  length = 0;
  index = 0;
}

void NumberSequence::fixed(short len, short *seq)
{
  mode = FIXED_LENGTH;
  length = len;
  assign(seq);
}

void NumberSequence::terminated(short len, short *seq)
{
  mode = TERMINATED;
  length = len;
  assign(seq);
}

KeyPressedOutcome NumberSequence::keyPressed(short k)
{
  if (mode == FIXED_LENGTH)
  {
    return keyPressed_fixed(k);
  }
  else
  {
    return keyPressed_terminated(k);
  }
}

KeyPressedOutcome NumberSequence::keyPressed_fixed(short k)
{
  if (k == INIT_KEY)
  {
    index = 0;
    return INCOMPLETE;
  }

  input[index] = k;
  index++;

  if (index == length)
  {
    index = 0;
    return validate() ? CORRECT : INCORRECT;
  }
  return INCOMPLETE;
}

KeyPressedOutcome NumberSequence::keyPressed_terminated(short k)
{
  if (k == SUBMIT_KEY)
  {
    return validate() ? CORRECT : INCORRECT;
  }
  
  input[0] = input[1];
  input[1] = input[2];
  input[2] = input[3];
  input[3] = input[4];
  input[4] = k;
  return INCOMPLETE;
}


bool NumberSequence::validate()
{
  for (short i = 0; i < length; i++)
  {
    if (sequence[i] != input[i])
    {
      return false;
    }
  }
  return true;
}

void NumberSequence::assign(short *src)
{
  for (short i = 0; i < length; i++)
  {
    sequence[i] = src[i];
  }
}