#include <Arduino.h>
#include "MusicNotes.h"
#include "ColorButton.h"
#include <chat.h>
#include <serial-number-flags.h>


Chat chat(ChatSource::SimonSays);
ChatMessage msg;

#define MODE_SPEAKING 0
#define MODE_LISTENING 1
#define MODE_PAUSING 2

#define STATUS_RED 2
#define STATUS_GRN 3
#define GND 11

ColorButton red = ColorButton('r', A0, 7, NOTE_C4);
ColorButton blu = ColorButton('b', A1, 6, NOTE_D4);
ColorButton yel = ColorButton('y', A2, 5, NOTE_E4);
ColorButton grn = ColorButton('g', A3, 4, NOTE_F4);

ColorButton *options[] = {&red, &blu, &yel, &grn};
ColorButton *lightSeq[4];
ColorButton *buttonSeq[4];

bool disarmed = false;
bool detonated = true;

bool vowel = false;
short strikes = 0;
short strikesSent = 0;
short stage = 0;
short index = 0;
short state = 0;
long counter = 0;
long epoch = millis();

void setDifficulty();
ColorButton *mapLightToButton(ColorButton *light);
void turnOnSolo(ColorButton *btn);
bool isCorrectButtonPressed();
void strike();

void setup()
{
  randomSeed(analogRead(5));
  lightSeq[0] = options[random(4)];
  lightSeq[1] = options[random(4)];
  lightSeq[2] = options[random(4)];
  lightSeq[3] = options[random(4)];

  red.turnOff();
  blu.turnOff();
  yel.turnOff();
  grn.turnOff();

  pinMode(STATUS_RED, OUTPUT);
  pinMode(STATUS_GRN, OUTPUT);
  pinMode(GND, OUTPUT);

  digitalWrite(STATUS_RED, HIGH);
  digitalWrite(STATUS_GRN, LOW);
  digitalWrite(GND, LOW);

  chat.begin();

  setDifficulty();
}

void loop()
{
  if (chat.receive(&msg)) 
  {
    if (msg.message == MessageType::Reset)
    {
      // Reset
      strikes = 0;
      disarmed = false;
      detonated = false;
      digitalWrite(STATUS_RED, HIGH);
      digitalWrite(STATUS_GRN, LOW);
      state = MODE_SPEAKING;
      stage = index = 0;
      counter = epoch = millis();
      vowel = msg.data & SN_CONTAINS_VOWEL;

      lightSeq[0] = options[random(4)];
      lightSeq[1] = options[random(4)];
      lightSeq[2] = options[random(4)];
      lightSeq[3] = options[random(4)];

      red.turnOff();
      blu.turnOff();
      yel.turnOff();
      grn.turnOff();
    }
    else if (msg.message == MessageType::Strike)
    {          
      strikes++;
    }
    else if (msg.message == MessageType::Detonate)
    {
      red.turnOff();
      yel.turnOff();
      grn.turnOff();
      blu.turnOff();
      detonated = true;
    }
  }

  if (!disarmed && !detonated)
  {
    counter = millis();

    red.refresh();
    blu.refresh();
    yel.refresh();
    grn.refresh();

    bool anyButtonPressed = false;

    if (red.isPressed) {
      turnOnSolo(&red);
      anyButtonPressed = true;
    }
    if (blu.isPressed) {
      turnOnSolo(&blu);
      anyButtonPressed = true;
    }
    if (yel.isPressed) {
      turnOnSolo(&yel);
      anyButtonPressed = true;
    }
    if (grn.isPressed) {
      turnOnSolo(&grn);
      anyButtonPressed = true;
    }

    if (anyButtonPressed) {
      
      state = MODE_LISTENING;

      if (isCorrectButtonPressed()) {
        if (index < stage) {
          index++;
        } else {
          stage++;
          index = 0;
          state = MODE_PAUSING;
          counter = epoch = millis();

          // On the last stage, skip the pause
          if (stage == 4) {
            state = MODE_SPEAKING;
          }
        }
      } else {
        strike();
        state = MODE_SPEAKING;
      }
    }

    if (red.isReleased) {
      red.turnOff();
    }

    if (blu.isReleased) {
      blu.turnOff();
    }

    if (yel.isReleased) {
      yel.turnOff();
    }

    if (grn.isReleased) {
      grn.turnOff();
    }

    if (state == MODE_PAUSING) {
      if (counter > epoch + 500) {
        red.turnOff();
        blu.turnOff();
        yel.turnOff();
        grn.turnOff();
      }
      if (counter > epoch + 1000) {
        state = MODE_SPEAKING;
        counter = epoch = millis();
      }
    }

    if (state == MODE_SPEAKING) {
      if (stage == 0) {

        if (counter < epoch + 250) {
          lightSeq[0]->turnOnMuted();
        } else {
          lightSeq[0]->turnOff();
        }

        if (counter > epoch + 2000) {
          epoch = millis();
        }
      }

      if (stage == 1) {
        if (counter < epoch + 250) {
          lightSeq[0]->turnOn();
        } else if (counter < epoch + 500) {
          lightSeq[0]->turnOff();
        } else if (counter < epoch + 750) {
          lightSeq[1]->turnOn();
        } else {
          lightSeq[1]->turnOff();
        }

        if (counter > epoch + 3500) {
          epoch = millis();
        }
      }

      if (stage == 2) {
        if (counter < epoch + 250) {
          lightSeq[0]->turnOn();
        } else if (counter < epoch + 500) {
          lightSeq[0]->turnOff();
        } else if (counter < epoch + 750) {
          lightSeq[1]->turnOn();
        } else if (counter < epoch + 1000) {
          lightSeq[1]->turnOff();
        } else if (counter < epoch + 1250) {
          lightSeq[2]->turnOn();
        } else {
          lightSeq[2]->turnOff();
        }

        if (counter > epoch + 4000) {
          epoch = millis();
        }
      }

      if (stage == 3) {
        if (counter < epoch + 250) {
          lightSeq[0]->turnOn();
        } else if (counter < epoch + 500) {
          lightSeq[0]->turnOff();
        } else if (counter < epoch + 750) {
          lightSeq[1]->turnOn();
        } else if (counter < epoch + 1000) {
          lightSeq[1]->turnOff();
        } else if (counter < epoch + 1250) {
          lightSeq[2]->turnOn();
        } else if (counter < epoch + 1500) {
          lightSeq[2]->turnOff();
        } else if (counter < epoch + 1750) {
          lightSeq[3]->turnOn();
        } else {
          lightSeq[3]->turnOff();
        }

        if (counter > epoch + 4500) {
          epoch = millis();
        }
      }

      if (stage == 4) {

        if (counter > epoch + 250) {
          chat.send(MessageType::Disarm);

          digitalWrite(STATUS_RED, LOW);
          digitalWrite(STATUS_GRN, HIGH);

          red.turnOff();
          blu.turnOff();
          yel.turnOff();
          grn.turnOff();

          disarmed = true;
        }
      }
    }
  }
}

void setDifficulty()
{
  delay(30);  // wait for debounce
  grn.refresh();
  yel.refresh();
  red.refresh();
  blu.refresh();

  if (grn.isPressed)
  {
    while(!grn.isReleased)
    {
      grn.refresh();
    }
    chat.send(MessageType::Difficulty, DIFFICULTY_NO_FAIL);
  }
  else if (yel.isPressed)
  {
    while(!yel.isReleased)
    {
      yel.refresh();
    }
    chat.send(MessageType::Difficulty, DIFFICULTY_NORMAL);
  }
  else if (red.isPressed)
  {
    while(!red.isReleased)
    {
      red.refresh();
    }
    chat.send(MessageType::Difficulty, DIFFICULTY_HEROIC);
  }
  else if (blu.isPressed)
  {
    while(!blu.isReleased)
    {
      blu.refresh();
    }
    chat.send(MessageType::Difficulty, DIFFICULTY_LEGENDARY);
  }
}

ColorButton *mapLightToButton(ColorButton *light)
{
  if (vowel == true)
  {
    switch (strikes)
    {
    case 0:
      switch (light->id)
      {
      case 'r':
        return &blu;
      case 'b':
        return &red;
      case 'g':
        return &yel;
      case 'y':
        return &grn;
      }
      break;
    case 1:
      switch (light->id)
      {
      case 'r':
        return &yel;
      case 'b':
        return &grn;
      case 'g':
        return &blu;
      case 'y':
        return &red;
      }
      break;
    case 2:
      switch (light->id)
      {
      case 'r':
        return &grn;
      case 'b':
        return &red;
      case 'g':
        return &yel;
      case 'y':
        return &blu;
      }
      break;
    }
  }
  else
  {
    switch (strikes)
    {
    case 0:
      switch (light->id)
      {
      case 'r':
        return &blu;
      case 'b':
        return &yel;
      case 'g':
        return &grn;
      case 'y':
        return &red;
      }
      break;
    case 1:
      switch (light->id)
      {
      case 'r':
        return &red;
      case 'b':
        return &blu;
      case 'g':
        return &yel;
      case 'y':
        return &grn;
      }
      break;
    case 2:
      switch (light->id)
      {
      case 'r':
        return &yel;
      case 'b':
        return &grn;
      case 'g':
        return &blu;
      case 'y':
        return &red;
      }
      break;
    }
  }
  return NULL;
}

void turnOnSolo(ColorButton *btn)
{
  lightSeq[0]->turnOff();
  lightSeq[1]->turnOff();
  lightSeq[2]->turnOff();
  lightSeq[3]->turnOff();
  btn->turnOn();
}

bool isCorrectButtonPressed()
{
  ColorButton *correctButton = mapLightToButton(lightSeq[index]);

  if (correctButton->isPressed)
  {
    return true;
  }
  return false;
}


void strike()
{
  chat.send(MessageType::Strike);
  strikes++;
  index = 0;

  for (short i = 0; i < 5; i++)
  {
    red.turnOn();
    blu.turnOn();
    yel.turnOn();
    grn.turnOn();
    delay(50);
    red.turnOff();
    blu.turnOff();
    yel.turnOff();
    grn.turnOff();
    delay(50);
  }
  counter = epoch = millis();
}
