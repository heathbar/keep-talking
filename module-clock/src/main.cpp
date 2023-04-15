#include <Arduino.h>
#include "button.h"
#include <NeoPixelBus.h>
#include <chat.h>
#include <serial-number-flags.h>

#define BRIGHTNESS 15
#define DATA_PIN 3
#define PIXEL_COUNT 12

#define BTN_GND 10
#define BTN_PIN 11

#define STATUS_GND 7
#define STATUS_RED 8
#define STATUS_GRN 9

#define POTS_GND A0
#define POTS_VCC 2

#define MODE_READ 0
#define MODE_SET 1
#define MODE_ENT 2

Chat chat(ChatSource::Clock);
ChatMessage msg;

short mode = MODE_READ;
short puzzle;
bool detonated = false;
bool disarmed = false;
bool vowel = false;
short strikes = 0;
short clock_minutes = -1;

Button button(BTN_PIN);

typedef NeoGrbwFeature MyPixelColorFeature;

const short init_times[][3] = {
  { 0,  3,  4 }, // 5:20
  { 8,  1,  5 }, // 6:10
  { 4,  2,  10}, // 11:15
  { 10, 6,  9 }, // 10:35
  { 6,  4,  7 }, // 8:25
  { 11, 5,  1 }, // 2:30
  { 5,  9 , 2 }, // 3:50
  { 9 , 8,  11}, // 12:45
  { 1,  10, 8 }, // 9:55
  { 3,  11, 6 }, // 7:00
  { 5,  7,  0 }, // 1:40
  { 7,  0,  3 }, // 4:05

  { 5,  2,  4 }, // 5:15
  { 9,  6,  5 }, // 6:35
  { 1,  3,  10}, // 11:20
  { 3,  1,  9 }, // 10:10
  { 5,  9,  7 }, // 8:50
  { 7,  8,  1 }, // 2:45
  { 0,  4 , 2 }, // 3:25
  { 8,  5,  11}, // 12:30
  { 4,  7,  8 }, // 9:40
  { 10, 0,  6 }, // 7:05
  { 6,  10, 0 }, // 1:55
  { 11, 11, 3 }  // 4:00
};

struct Light {
  short prevDigit;
  short digit;
  short color;
};

struct Light redLight = { 0, 1, 1 };
struct Light grnLight = { 1, 2, 2 };
struct Light bluLight = { 2, 3, 3 };

struct Light *light1 = &redLight;
struct Light *light2 = &grnLight;
struct Light *light3 = &bluLight;


RgbColor colors[4] = {
  RgbColor(0),
  RgbColor(BRIGHTNESS, 0, 0),
  RgbColor(0, BRIGHTNESS, 0),
  RgbColor(0, 0, BRIGHTNESS)
};

short submission[3] = { -1, -1, -1 };

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PIXEL_COUNT, DATA_PIN);

void strike();

void setup() {
  pinMode(BTN_GND, OUTPUT);
  digitalWrite(BTN_GND, LOW);
  
  pinMode(STATUS_GND, OUTPUT);
  pinMode(STATUS_RED, OUTPUT);
  pinMode(STATUS_GRN, OUTPUT);
  
  digitalWrite(STATUS_GND, LOW);
  digitalWrite(STATUS_RED, HIGH);
  digitalWrite(STATUS_GRN, LOW);

  pinMode(POTS_GND, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(POTS_VCC, OUTPUT);

  digitalWrite(POTS_GND, LOW);
  digitalWrite(POTS_VCC, HIGH);
 
  chat.begin();
  strip.Begin();

  // generate the question
  randomSeed(analogRead(A5));
  // puzzle = random(24);
  puzzle = 0;
  const short *q = init_times[puzzle];

  strip.SetPixelColor(q[0], colors[1]);
  strip.SetPixelColor(q[1], colors[2]);
  strip.SetPixelColor(q[2], colors[3]);
}


void loop() {
  if (chat.receive(&msg))
  {
    if (msg.message == MessageType::Reset)
    {
      detonated = false;
      disarmed = false;
      mode = MODE_READ;
      puzzle = random(24);
      vowel = msg.data & SN_CONTAINS_VOWEL;

      digitalWrite(STATUS_RED, HIGH);
      digitalWrite(STATUS_GRN, LOW);

      strip.ClearTo(colors[0]);
    }
    else if (msg.message == MessageType::Time2)
    {
      clock_minutes = msg.data;
    }
    else if (msg.message == MessageType::Strike)
    {
      strikes++;
    }
    else if (msg.message == MessageType::Detonate)
    {
      detonated = true;
      strip.ClearTo(colors[1]);
    }
  }

  if (!detonated && !disarmed)
  {
    if (button.isReleased())
    {
      if (mode == MODE_READ)
      {
        mode = MODE_SET;
      }
      else if (mode == MODE_SET)
      {
        mode = MODE_READ;
      }
      strip.ClearTo(colors[0]);
    }
    else if (mode == MODE_SET && button.isLongPressed())
    {
      mode = MODE_ENT;
    }

    if (mode == MODE_READ)
    {
      const short *q = init_times[puzzle];
      strip.SetPixelColor(q[0], colors[1]);
      strip.SetPixelColor(q[1], colors[2]);
      strip.SetPixelColor(q[2], colors[3]);
    }
    else if (mode == MODE_SET)
    {
      strip.ClearTo(colors[0]);

      if (millis() % 800 < 500)
      {
        int p1 = analogRead(A1);
        redLight.digit = map(p1, 0, 1010, 0, PIXEL_COUNT-1);

        int p2 = analogRead(A2);
        grnLight.digit = map(p2, 0, 1010, 0, PIXEL_COUNT-1);

        int p3 = analogRead(A3);
        bluLight.digit = map(p3, 0, 1023, 0, PIXEL_COUNT-1);

        submission[0] = redLight.digit;
        submission[1] = grnLight.digit;
        submission[2] = bluLight.digit;

        strip.SetPixelColor(redLight.digit, colors[redLight.color]);
        strip.SetPixelColor(grnLight.digit, colors[grnLight.color]);
        strip.SetPixelColor(bluLight.digit, colors[bluLight.color]);

        if (light2->digit != light2->prevDigit)
        {
          struct Light *temp = light1;
          light1 = light2;
          light2 = temp;
        }
        if (light3->digit != light3->prevDigit)
        {
          struct Light *temp = light1;
          light1 = light3;
          light3 = light2;
          light2 = temp;
        }

        strip.SetPixelColor(light1->digit, colors[light1->color]); 
        
        light1->prevDigit = light1->digit;
      }
    }
    else if (mode == MODE_ENT)
    {
      const short initialBlue = init_times[puzzle][2] + 1;  // plus one because clocks are not zero-based like arrays
      const short submitBlue = (submission[2] + 1) % 12;    // plus one because clocks are not zero-based like arrays

      const short initialGreen = init_times[puzzle][1] + 1; // plus one because clocks are not zero-based like arrays
      const short submitGreen = (submission[1] + 1) % 12;   // plus one because clocks are not zero-based like arrays

      const short initialRed = init_times[puzzle][0] + 1;   // plus one because clocks are not zero-based like arrays
      const short submitRed = (submission[0] + 1) % 12;     // plus one because clocks are not zero-based like arrays

      if (initialBlue < 5)
      {
        if (submitBlue != initialBlue * 2)
        {
          return strike();
        }
      }
      else if (initialBlue % 2 == 1 && vowel)
      {
        if (submitBlue != (initialBlue + 9) % 12)
        {
          return strike();
        }
      }
      else if (initialBlue % 2 == 1 && !vowel)
      {
        if (submitBlue != (initialBlue + 2) % 12)
        {
          return strike();
        }
      }
      else if (submitBlue != (initialBlue + 6) % 12)
      {
        return strike();
      }


      if (initialGreen > initialRed)
      {
        if (submitGreen != (initialRed + 11) % 12)
        {
          return strike();
        }
      }
      else if (submitGreen != initialGreen + 2 * strikes)
      {
        return strike();
      }

      if (strikes > 0)
      {
        if (submitRed != initialRed)
        {
          return strike();
        }
      }
      else if (clock_minutes <= 2)
      {
        if (submitRed != 3)
        {
          return strike();
        }
      }
      else
      {
        if (submitRed != 9)
        {
          return strike();
        }
      }

      // better to go green or the new time?
      strip.ClearTo(colors[2]);
      strip.Show();
      disarmed = true;
      
      chat.send(MessageType::Disarm);

      digitalWrite(STATUS_RED, LOW);
      digitalWrite(STATUS_GRN, HIGH);
    }
  }
 
  strip.Show();

}

void strike()
{
  mode = MODE_READ;
  chat.send(MessageType::Strike);

  // is it a bad idea to block?
  while (!button.isReleased())
  {
    delay(5);
  };
}