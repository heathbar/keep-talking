#include <Arduino.h>
#include "blue-display.h"
#include "white-display.h"
#include "button.h"
#include <chat.h>

#define SW_PIN 2

#define SLIDER_GND A3
#define SLIDER_PIN A2

#define DATA_PIN 10
#define CLOCK_PIN 12
#define LATCH_PIN 11

#define DIGIT1_PIN 9
#define DIGIT2_PIN 8
#define DIGIT3_PIN 7
#define DIGIT4_PIN 6
short digit_pins[] = { DIGIT1_PIN, DIGIT2_PIN, DIGIT3_PIN, DIGIT4_PIN };

Chat chat(ChatSource::Memory);
ChatMessage msg;

short stage = 1;
short selectedDigit = 1;

struct Memory {
  byte position[5];
  byte label[5];
};
Memory memory;

BlueDisplay blue;
WhiteDisplay white;
Button btn(SW_PIN);

bool disarmed = false;
bool detonated = false;
bool won = false;


void reset();
byte readSelected();
void display();
bool check(short stage, byte blue, short choice);
void shiftOut(byte myDataOut);

void setup() {
  chat.begin();

  pinMode(DIGIT1_PIN, INPUT_PULLUP);
  pinMode(DIGIT2_PIN, INPUT_PULLUP);
  pinMode(DIGIT3_PIN, INPUT_PULLUP);
  pinMode(DIGIT4_PIN, INPUT_PULLUP);

  pinMode(SW_PIN, INPUT_PULLUP);
  pinMode(SLIDER_GND, OUTPUT);
  pinMode(SLIDER_PIN, INPUT);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  digitalWrite(SLIDER_GND, LOW);

  randomSeed(analogRead(A4));

  reset();
}

void loop() {
  if (chat.receive(&msg))
  {
    switch (msg.message)
    {
      case MessageType::Reset:
        reset();
        break;
      case MessageType::Win:
        won = true;
        break;
      case MessageType::Detonate:
        detonated = true;
        break;
    }
  }

  if (!disarmed && !detonated && !won)
  {
    selectedDigit = readSelected();

    if (btn.refresh() != ButtonState::NONE)
    {
      if (check(stage, blue.value(), selectedDigit))
      {
        // Record the choice for later
        memory.position[stage] = selectedDigit;
        memory.label[stage] = white.label(selectedDigit);

        stage++;

        if (stage == 6)
        {
          disarmed = true;
          white.clear();
          blue.clear();
          return;
        }
        else
        {
          blue.next();
          white.shuffle();
        }
      }
      else
      {
        reset();
      }
    }
  }
  display();
}

void reset()
{
  disarmed = false;
  detonated = false;
  won = false;

  stage = 1;

  blue.reset();
  white.reset();
}

byte readSelected()
{
    int s = analogRead(SLIDER_PIN);

    if (s < 150)
    {
      return 0;
    }
    else if (s < 500)
    {
      return 1;
    }
    else if (s < 850)
    {
      return 2;
    }
    else
    {
      return 3;
    }
}


void display()
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(LATCH_PIN, LOW);

    // Stage LEDs
    byte leds = disarmed ? 2 : 4;
    for (short j = 1; j <= stage; j++)
    {
      if (j != stage || millis() % 600 > 300)
      {
        leds += (1 << (j+2));
      }
    }
    shiftOut(leds);
    
    // Blue digit
    shiftOut(blue.display());

    // Options Digits
    if (i == selectedDigit && millis() % 400 > 200)
    {
      shiftOut(0);
    }
    else
    {
      shiftOut(white.display(i));
    }

    digitalWrite(LATCH_PIN, HIGH);
    pinMode(digit_pins[i], OUTPUT);
    digitalWrite(digit_pins[i], LOW);
    delay(1);
    pinMode(digit_pins[i], INPUT_PULLUP);
  }
}

bool check(short stage, byte blue, short choice)
{
  Serial.println(String("Blue = ") + blue);
  Serial.println(String("choice = ") + choice);
  

  switch (stage)
  {
    case 1:
      switch (blue)
      {
        case 1:
          return choice == 1;
        case 2:
          return choice == 1;
        case 3:
          return choice == 2;
        case 4:
          return choice == 3;
      }
      break;
    case 2:
      switch (blue)
      {
        case 1:
          return choice == white.find(4);
        case 2:
          return choice == memory.position[1];
        case 3:
          return choice == 0;
        case 4:
          return choice == memory.position[1];
      }
      break;
    case 3:
      switch (blue)
      {
        case 1:
          return choice == white.find(memory.label[2]);
        case 2:
          return choice == white.find(memory.label[1]);
        case 3:
          return choice == 2;
        case 4:
          return choice == white.find(4);
      }
      break;
    case 4:
      switch (blue)
      {
        case 1:
          return choice == memory.position[1];
        case 2:
          return choice == 0;
        case 3:
          return choice == memory.position[2];
        case 4:
          return choice == memory.position[2];
      }
      break;
    case 5:
      Serial.println("---");
      switch (blue)
      {
        case 1:
          return choice == white.find(memory.label[1]);
        case 2:
          return choice == white.find(memory.label[2]);
        case 3:
          return choice == white.find(memory.label[4]);
        case 4:
          return choice == white.find(memory.label[3]);
      }
      break;
  }
}


void shiftOut(byte myDataOut)
{
  // This shifts 8 bits out MSB first,
  // on the rising edge of the clock,
  // clock idles low
  // internal function setup
  int i = 0;
  int pinState;

  // clear everything out just in case to
  // prepare shift register for bit shifting
  digitalWrite(DATA_PIN, 0);
  digitalWrite(CLOCK_PIN, 0);

  // for each bit in the byte myDataOut;
  // NOTICE THAT WE ARE COUNTING DOWN in our for loop
  // This means that %00000001 or "1" will go through such
  // that it will be pin Q0 that lights.
  for (i = 7; i >= 0; i--)
  {
    digitalWrite(CLOCK_PIN, 0);

    // if the value passed to myDataOut and a bitmask result
    //  true then... so if we are at i=6 and our value is
    //  %11010100 it would the code compares it to %01000000
    //  and proceeds to set pinState to 1.
    if (myDataOut & (1 << i))
    {
      pinState = 1;
    }
    else
    {
      pinState = 0;
    }

    // Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(DATA_PIN, pinState);

    // register shifts bits on upstroke of clock pin
    digitalWrite(CLOCK_PIN, 1);

    // zero the data pin after shift to prevent bleed through
    digitalWrite(DATA_PIN, 0);
  }

  // stop shifting
  digitalWrite(CLOCK_PIN, 0);
}