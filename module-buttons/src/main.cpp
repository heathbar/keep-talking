
#include <Arduino.h>
#include "Tlc5940.h"
#include "RgbButton.h"
#include <chat.h>

#define STATUS_GRN 6
#define STATUS_GND 5
#define STATUS_RED 4

Chat chat(ChatSource::Buttons);
ChatMessage msg;


RgbButton buttons[5] = {
  {8, 7,  6,   A1, &chat},
  {11, 10, 9,  A2, &chat},
  {14, 13, 12, A3, &chat},
  {5, 4,  3,   A0, &chat},
  {2, 1,  0,   12, &chat}
};

bool solves[5] = { false, false, false, false, false };
bool disarmed = false;
bool detonated = true;

void setUniqueRandomAnimations();
void setTime(short index, short time);


void setup()
{
  Tlc.init();
  chat.begin();

  pinMode(STATUS_GRN, OUTPUT);
  pinMode(STATUS_RED, OUTPUT);
  pinMode(STATUS_GND, OUTPUT);

  digitalWrite(STATUS_GRN, HIGH);
  digitalWrite(STATUS_GND, HIGH);
  digitalWrite(STATUS_RED, LOW);
}

void loop()
{
  if (chat.receive(&msg))
  {
    switch (msg.message)
    {
      case MessageType::Reset:
        detonated = false;
        disarmed = false;
        
        digitalWrite(STATUS_GRN, HIGH);
        digitalWrite(STATUS_GND, HIGH);
        digitalWrite(STATUS_RED, LOW);
        
        setUniqueRandomAnimations();
        break;
      case MessageType::Detonate:
        detonated = true;
        buttons[0].setColor(Color::red);
        buttons[1].setColor(Color::red);
        buttons[2].setColor(Color::red);
        buttons[3].setColor(Color::red);
        buttons[4].setColor(Color::red);
        break;
      case MessageType::Time1:
        setTime(0, msg.data);
        break;
      case MessageType::Time2:
        setTime(1, msg.data);
        break;
      case MessageType::Time3:
        setTime(2, msg.data);
        break;
      case MessageType::Time4:
        setTime(3, msg.data);
        break;
    }
  }


  if (!disarmed && !detonated)
  {
    RgbButtonEvent status[5] =
    {
      buttons[0].refresh(),
      buttons[1].refresh(),
      buttons[2].refresh(),
      buttons[3].refresh(),
      buttons[4].refresh()
    };

    for (short i = 0; i < 5; i++)
    {
      if (status[i] == RgbButtonEvent::stricken)
      {
        chat.send(MessageType::Strike);
      }
      else if (status[i] == RgbButtonEvent::solved)
      {
        solves[i] = true;
      }
    }

    if ((solves[0] || buttons[0].solution == Solution::Skip) &&
        (solves[1] || buttons[1].solution == Solution::Skip) &&
        (solves[2] || buttons[2].solution == Solution::Skip) &&
        (solves[3] || buttons[3].solution == Solution::Skip) &&
        (solves[4] || buttons[4].solution == Solution::Skip))
    {
      chat.send(MessageType::Disarm);

      buttons[0].setColor(Color::black);
      buttons[1].setColor(Color::black);
      buttons[2].setColor(Color::black);
      buttons[3].setColor(Color::black);
      buttons[4].setColor(Color::black);

      digitalWrite(STATUS_GRN, LOW);
      digitalWrite(STATUS_GND, HIGH);
      digitalWrite(STATUS_RED, HIGH);
      
      disarmed = true;
    }
  }
  Tlc.update();
}




void setUniqueRandomAnimations()
{
  const short MAX = 20;
  
  randomSeed(analogRead(A5));
  
  short uniqueRandomNumbers[5];
 
  // Populate array with unique random numbers 
  for (short i = 0; i < 5; i++)
  {
    bool duplicate = false;
    do {
      duplicate = false;
      uniqueRandomNumbers[i] = random(MAX);
      for (short j = 0; j < i; j++) {
        if (uniqueRandomNumbers[i] == uniqueRandomNumbers[j])
        {
          duplicate = true;
        }
      }
    } while (duplicate);
  }

  buttons[0].setAnimation(uniqueRandomNumbers[0]);
  buttons[1].setAnimation(uniqueRandomNumbers[1]);
  buttons[2].setAnimation(uniqueRandomNumbers[2]);
  buttons[3].setAnimation(uniqueRandomNumbers[3]);
  buttons[4].setAnimation(uniqueRandomNumbers[4]);
}

void setTime(short index, short time)
{
  buttons[0].setDigit(index, time);
  buttons[1].setDigit(index, time);
  buttons[2].setDigit(index, time);
  buttons[3].setDigit(index, time);
  buttons[4].setDigit(index, time);
}