#include <Arduino.h>
#include <Wire.h>
#include <PCM.h>
#include <chat.h>
#include "clock.h"
#include "sounds.h"
#include "serial-number.h"
#include "lcd-helpers.h"

#define STRIKE1 49
#define STRIKE2 51
#define STRIKE3 53
#define CTRL_LED 52
#define ALT_LED 50
#define ETP_LED 48
#define SPEAKER_PIN 10
#define AUDIO_GND 11
#define VCC 13

Chat chat(ChatSource::Main);
ChatMessage msg;

Clock *clock;

int duration;
short strikes = 0;

short numModulesToWin = 3;
short numModulesDisarmed = 0;
bool disarmedModules[13] = { false, false, false, false, false, false, false, false, false, false, false, false, false };
bool noFailMode = false;
bool indicatorCtrl = 0;
bool indicatorAlt = 0;
bool indicatorEtp = 0;

SerialNumber serialNumbers[32] = {
  { "249X7N27N2", 0                                     },
  { "29V7NV6J62", 0                                     },
  { "PNKPSC4774", 0                                     },
  { "1972S686R4", 0                                     },
  { "CDCM2B4AM6", 0                                     },
  { "1XKFQ859I6", 0                                     },
  { "K8F6287346", 0                                     },
  { "CW65F4DTX6", 0                                     },
  
  { "1RONXM4NE8", SN_CONTAINS_VOWEL                     },
  { "VS0B42D7I4", SN_CONTAINS_VOWEL                     },
  { "SS4YU75BY8", SN_CONTAINS_VOWEL                     },
  { "7EV9164428", SN_CONTAINS_VOWEL                     },
  { "7VR2D228A2", SN_CONTAINS_VOWEL                     },
  { "FEV91662L8", SN_CONTAINS_VOWEL                     },
  { "45PQ4E81Q2", SN_CONTAINS_VOWEL                     },
  { "50NFJGDI44", SN_CONTAINS_VOWEL                     },

  { "MCJ11757X5",                     SN_LAST_DIGIT_ODD },
  { "231JR0M4N7",                     SN_LAST_DIGIT_ODD },
  { "10T4C0S4M3",                     SN_LAST_DIGIT_ODD },
  { "7H7TL5ZN85",                     SN_LAST_DIGIT_ODD },
  { "V92NGTP629",                     SN_LAST_DIGIT_ODD },
  { "55QX395ES7",                     SN_LAST_DIGIT_ODD },
  { "FR39CZ6299",                     SN_LAST_DIGIT_ODD },
  { "SR43WK6PR1",                     SN_LAST_DIGIT_ODD },

  { "MN1U297F71", SN_CONTAINS_VOWEL | SN_LAST_DIGIT_ODD },
  { "2C3P061A93", SN_CONTAINS_VOWEL | SN_LAST_DIGIT_ODD },
  { "D24ETH4NP5", SN_CONTAINS_VOWEL | SN_LAST_DIGIT_ODD },
  { "N9JZI7H3I3", SN_CONTAINS_VOWEL | SN_LAST_DIGIT_ODD },
  { "NQA38EZ307", SN_CONTAINS_VOWEL | SN_LAST_DIGIT_ODD },
  { "H71AKRRF49", SN_CONTAINS_VOWEL | SN_LAST_DIGIT_ODD },
  { "EW8J992OC7", SN_CONTAINS_VOWEL | SN_LAST_DIGIT_ODD },
  { "4N9WYRA9P3", SN_CONTAINS_VOWEL | SN_LAST_DIGIT_ODD }

};

SerialNumber *serialNumber;

void detonate();
void disarm();
void setDifficulty(short d);
void publishTimeChanges(TimeChanges tc);

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting...");

  Serial2.begin(9600);

  chat.begin();

  pinMode(STRIKE1, OUTPUT);
  pinMode(STRIKE2, OUTPUT);
  pinMode(STRIKE3, OUTPUT);

  pinMode(CTRL_LED, OUTPUT);
  pinMode(ALT_LED, OUTPUT);
  pinMode(ETP_LED, OUTPUT);

  // extra ground pin
  pinMode(AUDIO_GND, OUTPUT);
  digitalWrite(AUDIO_GND, LOW);

  // extra vcc pin
  pinMode(VCC, OUTPUT);
  digitalWrite(VCC, HIGH);

  digitalWrite(STRIKE1, LOW);
  digitalWrite(STRIKE2, LOW);
  digitalWrite(STRIKE3, LOW);

  randomSeed(analogRead(A5));

  // it seems to provide better randomization
  // when you get a couple out of the way
  random(10);
  random(100);
  random(1000);
  random(10000);

  serialNumber = &serialNumbers[random(32)];
  serialNumber->randomizeIndicators();

  clearLCD();
  selectLineOneLCD();
  printLCD("Configuring...");
  selectLineTwoLCD();
  
  for (int i = 0; i < 128; i++)
  {
    if (i == 0 || i == 42 || i == 84)
    {
      tone(SPEAKER_PIN, 523, 150);
    }
    
    if (i % 8 == 0)
    {
      writeLCD(255); // all pixels character for the loading bar
    }
    delay(30);

    if (chat.receive(&msg))
    {
      if (msg.message == MessageType::Difficulty)
      {
        setDifficulty(msg.data);

        tone_blocking(1000, 50);
        tone_blocking(1200, 50);
        tone_blocking(1400, 50);
      }
    }
  }

  if (!duration)
  {
    setDifficulty(DIFFICULTY_EASY);
  }

  clearLCD();
  selectLineOneLCD();
  printLCD("Serial No");
  selectLineTwoLCD();
  printLCD(serialNumber->number);

  digitalWrite(CTRL_LED, serialNumber->ctrl);
  digitalWrite(ALT_LED, serialNumber->alt);
  digitalWrite(ETP_LED, serialNumber->etp);

  tone_blocking(695, 500);
  clock->start();
  chat.send(MessageType::Reset, (uint8_t)serialNumber->flags);

  Serial.print("Flags: ");
  Serial.println(serialNumber->flags, BIN);
}


void loop()
{
  TimeChanges tc = clock->refresh();
  if (clock->isExpired())
  {
    detonate();
  }
  publishTimeChanges(tc);

  
  if (chat.receive(&msg))
  {    
    if (msg.message == MessageType::Disarm)
    {
      // Due to reliability issues, disarm messages will be sent multiple times. 
      // We track that here so that each module is counted only once
      if (!disarmedModules[msg.sender])
      {
        disarmedModules[msg.sender] = true;
        numModulesDisarmed++;

        // Play happy tone
        tone_blocking(800, 65);
        tone_blocking(1000, 65);
        tone_blocking(1300, 100);
      }
   
    }
    else if (msg.message == MessageType::Strike)
    {
      strikes++;
      clock->strike();

      switch(strikes)
      {
        case 1:
          digitalWrite(STRIKE1, HIGH);
          digitalWrite(STRIKE2, LOW);
          digitalWrite(STRIKE3, LOW);

          tone_blocking(1200, 65);
          delay(20);
          tone_blocking(1200, 65);
          delay(20);
          tone_blocking(1200, 85);
          delay(15);
          break;
        case 2:
          digitalWrite(STRIKE1, HIGH);
          digitalWrite(STRIKE2, HIGH);
          digitalWrite(STRIKE3, LOW);

          tone_blocking(1200, 65);
          delay(20);
          tone_blocking(1200, 65);
          delay(20);
          tone_blocking(1200, 85);
          delay(15);
          break;
        case 3:
          digitalWrite(STRIKE1, HIGH);
          digitalWrite(STRIKE2, HIGH);
          digitalWrite(STRIKE3, HIGH);
          detonate();
      }

    }
  }

  if (numModulesDisarmed == numModulesToWin)
  {
    disarm();
  }
}

void detonate()
{
  if (noFailMode)
  {
    clearLCD();
    selectLineOneLCD();
    printLCD("OVERTIME!");
  }
  else
  {
    chat.send(MessageType::Detonate);

    clearLCD();
    selectLineOneLCD();
    printLCD("OH NOES!!!");

    delay(500);
    tone_blocking(800, 1000);
    delay(500);

    selectLineTwoLCD();
    printLCD("BOOM!!!");

    startPlayback(SND_EXPLOSION, sizeof(SND_EXPLOSION));

    delay(1500);
    clearLCD();

    // one last refresh to clear out any ms
    // this causes an extra "tick" which seems to not be necessary
    // clock->refresh();

    while(1);
  }

}

void disarm()
{
  chat.send(MessageType::Win);
  levelClear();
  while(1);
}


void setDifficulty(short d)
{
  clearLCD();
  selectLineOneLCD();

  switch(d)
  {
    case DIFFICULTY_NO_FAIL: 
      duration = 300; // FIVE MINUTES
      numModulesToWin = 3;
      noFailMode = true;
      printLCD("No Fail Mode...");
      Serial.println("No Fail Mode");
      selectLineTwoLCD();
      break;
    case DIFFICULTY_EASY:
      duration = 300; // FIVE MINUTES
      numModulesToWin = 3;
      noFailMode = false;
      printLCD("Easy Mode");
      selectLineTwoLCD();
      Serial.println("Easy Difficulty");
      break;
    case DIFFICULTY_NORMAL:
      duration = 300; // FIVE MINUTES
      numModulesToWin = 6;
      noFailMode = false;
      printLCD("Normal Mode");
      selectLineTwoLCD();
      Serial.println("Normal Difficulty");
      break;
    case DIFFICULTY_HEROIC:
      duration = 480; // EIGHT MINUTES
      numModulesToWin = 9;
      noFailMode = false;
      printLCD("Heroic Mode");
      selectLineTwoLCD();
      Serial.println("Heroic Difficulty");
      break;
    case DIFFICULTY_LEGENDARY:
      duration = 360; // SIX MINUTES
      numModulesToWin = 9;
      noFailMode = false;
      printLCD("Legendary Mode");
      selectLineTwoLCD();
      Serial.println("Legendary Difficulty");
      break;
  }

  clock = new Clock(duration);
}

void publishTimeChanges(TimeChanges tc)
{
  for (short i = 0; i < 4; i++)
  {
    if (tc.digits[i] >= 0)
    {
      chat.send(timeMessages[i], tc.digits[i]);
    }
  }
}
