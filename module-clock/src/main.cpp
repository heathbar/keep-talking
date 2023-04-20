#include <Arduino.h>
#include <NeoPixelBus.h>
#include <chat.h>
#include <serial-number-flags.h>
#include "hand.h"
#include "button.h"
#include "encoder.h"
#include "light.h"

#define BRIGHTNESS 15
#define PIXEL_COUNT 12

#define LED_VCC_PIN 10
#define LED_GND_PIN 11
#define LED_DATA_PIN 12

#define BTN_HR A0
#define BTN_MN A1
#define BTN_SC A2
#define BTN_SB A3

#define ENC_0_CLK 4
#define ENC_0_DAT 5
#define ENC_1_CLK 6
#define ENC_1_DAT 7
#define ENC_2_CLK 8
#define ENC_2_DAT 9

#define STATUS_RED 2
#define STATUS_GRN 3

Chat chat(ChatSource::Clock);
ChatMessage msg;

short puzzle;
bool detonated = false;
bool disarmed = false;
bool vowel = false;
bool ctrl = false;
bool alt = false;
bool etp = false;

short strikes = 0;
short clock_minutes = -1;

RgbColor colors[4] = {
  RgbColor(0),
  RgbColor(BRIGHTNESS, 0, 0),
  RgbColor(0, BRIGHTNESS, 0),
  RgbColor(0, 0, BRIGHTNESS)
};

Hand secondHand(BTN_SC, ENC_0_CLK, ENC_0_DAT, colors[1], 2);
Hand minuteHand(BTN_MN, ENC_1_CLK, ENC_1_DAT, colors[2], 1);
Hand hourHand(BTN_HR, ENC_2_CLK, ENC_2_DAT, colors[3], 0);

Hand *hands[] = { &secondHand, &minuteHand, &hourHand };

Button submitButton(BTN_SB);

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



short submission[3] = { -1, -1, -1 };

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PIXEL_COUNT, LED_DATA_PIN);

void strike();
void promote(Light * light);

void setup() {

// temporary
  pinMode(LED_GND_PIN, OUTPUT);
  pinMode(LED_VCC_PIN, OUTPUT);
  digitalWrite(LED_GND_PIN, LOW);
  digitalWrite(LED_VCC_PIN, HIGH);

  pinMode(STATUS_RED, OUTPUT);
  pinMode(STATUS_GRN, OUTPUT);
  
  digitalWrite(STATUS_RED, HIGH);
  digitalWrite(STATUS_GRN, LOW);
  
  chat.begin();
  strip.Begin();

  // generate the question
  randomSeed(analogRead(A5));
  puzzle = random(24);

  secondHand.setTime(init_times[puzzle][0]);
  minuteHand.setTime(init_times[puzzle][1]);
  hourHand.setTime(init_times[puzzle][2]);
}


void loop() {

  if (chat.receive(&msg))
  {
    if (msg.message == MessageType::Reset)
    {
      detonated = false;
      disarmed = false;
      puzzle = random(24);
      vowel = msg.data & SN_CONTAINS_VOWEL;
      ctrl = msg.data & IND_CTRL;
      alt = msg.data & IND_ALT;
      etp = msg.data & IND_ETP;

      digitalWrite(STATUS_RED, HIGH);
      digitalWrite(STATUS_GRN, LOW);

      strip.ClearTo(colors[0]);

      secondHand.setTime(init_times[puzzle][0]);
      minuteHand.setTime(init_times[puzzle][1]);
      hourHand.setTime(init_times[puzzle][2]);

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
    strip.ClearTo(colors[0]);

    hands[0]->update();
    hands[1]->update();
    hands[2]->update();

    // Do some re-ordering to provide a better experience when two lights occupy the same position
    if (hands[1]->hasChanged())
    {
      struct Hand *temp = hands[0];
      hands[0] = hands[1];
      hands[1] = temp;
    }
    if (hands[2]->hasChanged())
    {
      struct Hand *temp = hands[0];
      hands[0] = hands[2];
      hands[2] = hands[1];
      hands[1] = temp;
    }

    strip.SetPixelColor(hands[2]->getValue(), hands[2]->getColor());
    strip.SetPixelColor(hands[1]->getValue(), hands[1]->getColor());
    strip.SetPixelColor(hands[0]->getValue(), hands[0]->getColor());
    
    if (submitButton.isReleased())
    {
      const short initialBlue = init_times[puzzle][2] + 1;  // plus one because clocks are not zero-based like arrays
      const short submitBlue = (hourHand.getValue() + 1) % 12;    // plus one because clocks are not zero-based like arrays

      const short initialGreen = init_times[puzzle][1] + 1; // plus one because clocks are not zero-based like arrays
      const short submitGreen = (minuteHand.getValue() + 1) % 12;   // plus one because clocks are not zero-based like arrays

      const short initialRed = init_times[puzzle][0] + 1;   // plus one because clocks are not zero-based like arrays
      const short submitRed = (secondHand.getValue() + 1) % 12;     // plus one because clocks are not zero-based like arrays
     
      //  HOUR HAND

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

      // MINUTE HAND

      if (ctrl)
      {
        if (submitGreen != 5)
        {
          return strike();
        }
      }
      else if (etp)
      {
        if (submitGreen != (initialGreen + 3) % 12)
        {
          return strike();
        }
      }
      else if (initialGreen > 3 && initialGreen < 9)
      {
        if (submitGreen != 11)
        {
          return strike();
        }
      }
      else if (submitGreen != initialGreen + 2 * strikes)
      {
        return strike();
      }

      // SECOND HAND

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
      else if (alt)
      {
        if (submitRed != 2)
        {
          return strike();
        }
      }
      else if (submitRed != 9)
      {
        return strike();
      }
    
      disarmed = true;
      chat.send(MessageType::Disarm);

      digitalWrite(STATUS_RED, LOW);
      digitalWrite(STATUS_GRN, HIGH);

      for (short i = 0; i < 3; i++)
      {
        delay(150);
        strip.ClearTo(colors[2]);
        strip.Show();
 
        delay(150);
        strip.ClearTo(colors[0]);
        strip.Show();
      }
    }
    else if (submitButton.isLongPressed())
    {
      // reset to original time
      secondHand.setTime(init_times[puzzle][0]);
      minuteHand.setTime(init_times[puzzle][1]);
      hourHand.setTime(init_times[puzzle][2]);
    }
  }
 
  strip.Show();
}

void strike()
{
  chat.send(MessageType::Strike);
}
