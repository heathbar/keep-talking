#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "term.h"


enum KeyPressedOutcome
{
  INCORRECT,
  INCOMPLETE,
  CORRECT
};

class NumberSequence
{
  private:
    short sequence[5];
    short length;
    short index;
    short mode = 0;
    short input[5];

    static const short SUBMIT_KEY = 11;
    static const short INIT_KEY = 10;
    static const short FIXED_LENGTH = 1;
    static const short TERMINATED = 2;

    bool validate();
    void push(short k);
    void assign(short *src);
    KeyPressedOutcome keyPressed_fixed(short k);
    KeyPressedOutcome keyPressed_terminated(short k);
    

  public:
    NumberSequence();
    void fixed(short len, short *seq);
    void terminated(short len, short *seq);
    KeyPressedOutcome keyPressed(short k);
};

#endif