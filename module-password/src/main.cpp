#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_Alphanumeric_Display.h>
#include "password-manager.h"
#include <chat.h>
#include <button.h>
#include <serial-number-flags.h>

const short ledPin = 2;
const short gndPin = 7;
const short btnPin = A3;
const short gndPin2 = 13;
const short armedPin = A0;
const short disarmedPin = A1;

HT16K33 display;

PasswordManager password;
Button submit(btnPin);

Chat chat(ChatSource::Password);
ChatMessage msg;

bool disarmed = false;
bool detonated = false;
bool won = false;

void reset(bool alt);

void setup()
{
  randomSeed(analogRead(A2));
  chat.begin();
  Wire.begin();

  if (display.begin() == false)
  {
    while (1);
  }

  pinMode(ledPin, OUTPUT);
  pinMode(gndPin, OUTPUT);
  pinMode(gndPin2, OUTPUT);
  pinMode(armedPin, OUTPUT);
  pinMode(disarmedPin, OUTPUT);
  pinMode(btnPin, INPUT_PULLUP);
  digitalWrite(ledPin, HIGH);
  digitalWrite(gndPin, LOW);
  digitalWrite(gndPin2, LOW);
  digitalWrite(armedPin, HIGH);
  digitalWrite(disarmedPin, LOW);

  // reset(false);

}

void loop()
{
  if (chat.receive(&msg))
  {

    switch (msg.message)
    {
      case MessageType::Reset:
        bool alt = msg.data & IND_ALT;
        reset(alt);
        break;
    
      case MessageType::Detonate:
        detonated = true;
        display.displayOn();
        display.print("RUN ");
        delay(200);
        display.displayOff();
        delay(200);
        display.displayOn();
        delay(200);
        display.displayOff();
        delay(200);
        display.displayOn();
        delay(200);
        display.displayOff();
        delay(200);
        display.print("BOOM");
        delay(100);
        display.displayOff();
        delay(100);
        display.displayOn();
        delay(100);
        display.displayOff();
        delay(100);
        display.displayOn();
        delay(100);
        display.displayOff();
        break;

      case MessageType::Win:
        won = true;
        display.print("NICE");
        break;
    }
  }

  if (!detonated && !disarmed && !won)
  {
    char pass[5];
    bool correct = password.eval(pass);
    display.print(pass);

    if (submit.isPressed())
    {
      if (correct)
      {
        chat.send(MessageType::Disarm);

        digitalWrite(armedPin, LOW);
        digitalWrite(disarmedPin, HIGH);

        display.displayOn();
        delay(200);
        display.displayOff();
        delay(200);
        display.displayOn();
        delay(200);
        display.displayOff();
        delay(200);
        display.displayOn();
        delay(200);
        display.displayOff();
        delay(200);

        disarmed = true;
      }
      else
      {
        chat.send(MessageType::Strike);
      }
    }
  }  
}


void reset(bool alt)
{
  detonated = false;
  disarmed = false;
  won = false;

  short index = random(30);
  password.load(index, alt);
  password.mix();

  char pass[5];
  while (password.eval(pass))
  {
    password.mix();
  }
}