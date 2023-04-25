#include <Arduino.h>
#include "keypad.h"

#define DEBOUNCE_TIME 35

Keypad::Keypad(short pin1, short pin2, short pin3, short pin4, short pin5, short pin6, short pin7)
{
  KP1 = pin1;
  KP2 = pin2;
  KP3 = pin3;
  KP4 = pin4;
  KP5 = pin5;
  KP6 = pin6;
  KP7 = pin7;

  for (short i = 0; i < 7; i++)
  {
    state[i] = false;
    last_state[i] = false;
    last_debounce_time[i] = 0;
  }

  pinMode(KP1, INPUT);
  pinMode(KP2, INPUT_PULLUP);
  pinMode(KP3, INPUT);
  pinMode(KP4, INPUT_PULLUP);
  pinMode(KP5, INPUT);
  pinMode(KP6, INPUT_PULLUP);
  pinMode(KP7, INPUT_PULLUP);
}

void Keypad::refresh(State *states)
{
  for (short i = 0; i < 12; i++)
  {
    states[i] = UNCHANGED;
  }

  bool readings[12];
  read(readings);
  

  for (short i = 0; i < 12; i++)
  {
    if (readings[i] != last_state[i])
    {
      last_debounce_time[i] = millis();
    }
     
    if ((millis() - last_debounce_time[i]) > DEBOUNCE_TIME) {
      if (readings[i] != state[i])
      {
        state[i] = readings[i];

        if (state[i] == HIGH)
        {
          states[i] = PRESSED;
        }
        else
        {
          states[i] = RELEASED;
        }
      }
    }
    last_state[i] = readings[i];
  }
}

void Keypad::read(bool *values)
{
    for (short i = 0; i < 12; i++)
    {
      values[i] = false;
    }
    
    pinMode(KP1, INPUT);
    pinMode(KP3, OUTPUT);
    pinMode(KP5, INPUT);
    digitalWrite(KP3, LOW);

    if (digitalRead(KP2) == LOW)
    {
      values[1] = true;
    }
    else if (digitalRead(KP7) == LOW)
    {
      values[4] = true;
    }
    else if (digitalRead(KP6) == LOW)
    {
      values[7] = true;
    }
    else if (digitalRead(KP4) == LOW)
    {
      values[10] = true;
    }

    pinMode(KP3, INPUT);
    pinMode(KP1, OUTPUT);  
    digitalWrite(KP1, LOW);


    if (digitalRead(KP2) == LOW)
    {
      values[2] = true;
    }
    else if (digitalRead(KP7) == LOW)
    {
      values[5] = true;
    }
    else if (digitalRead(KP6) == LOW)
    {
      values[8] = true;
    }
    else if (digitalRead(KP4) == LOW)
    {
      values[0] = true;
    }

    pinMode(KP1, INPUT);
    pinMode(KP5, OUTPUT);  
    digitalWrite(KP5, LOW);


    if (digitalRead(KP2) == LOW)
    {
      values[3] = true;
    }
    else if (digitalRead(KP7) == LOW)
    {
      values[6] = true;
    }
    else if (digitalRead(KP6) == LOW)
    {
      values[9] = true;
    }
    else if (digitalRead(KP4) == LOW)
    {
      values[11] = true;
    }
}