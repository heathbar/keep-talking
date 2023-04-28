#include <Arduino.h>
#include <Wire.h>
#include <PCM.h>
#include <Adafruit_SSD1306.h>
#include <chat.h>
#include "clock.h"
#include "sounds.h"
#include "serial-number.h"

#define STRIKE1 49
#define STRIKE2 51
#define STRIKE3 53
#define CTRL_LED 52
#define ALT_LED 50
#define ETP_LED 48
#define SPEAKER_PIN 10
#define AUDIO_GND 11
#define OLED_VCC 13

Chat chat(ChatSource::Main);
ChatMessage msg;

Clock *clock;
Adafruit_SSD1306 oled = Adafruit_SSD1306(128, 32, &Wire);

int duration;
short strikes = 0;

short moduleCount = 6;
short modulesDisarmed = 0;
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
  pinMode(OLED_VCC, OUTPUT);
  digitalWrite(OLED_VCC, HIGH);

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

  serialNumber = &serialNumbers[random(12)];
  serialNumber->randomizeIndicators();

  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  oled.clearDisplay();

  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0,0);
  oled.println("Configuring Bomb...");
  
  for (int i = 0; i < 128; i++)
  {
    if (i == 0 || i == 42 || i == 84)
    {
      tone(SPEAKER_PIN, 523, 150);
    }
    if (i % 10 == 0 || i % 10 == 1) {
      continue;
    }
    oled.drawLine(i, 16, i, 64, SSD1306_WHITE);
    oled.display();
    delay(15);

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

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0,0);
  oled.println("Serial Number");
  oled.println();

  oled.setTextSize(2);
  oled.setTextColor(SSD1306_WHITE);
  oled.println(serialNumber->number);
  oled.display();

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
      modulesDisarmed++;
   
      // Play happy tone
      tone_blocking(800, 65);
      tone_blocking(1000, 65);
      tone_blocking(1300, 100);
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

  if (modulesDisarmed == moduleCount)
  {
    disarm();
  }
}

void detonate()
{
  if (noFailMode)
  {
    oled.clearDisplay();
    oled.setCursor(0, 8);
    oled.setTextSize(2);
    oled.println("OVERTIME!");
    oled.display();
  }
  else
  {
    chat.send(MessageType::Detonate);

    oled.clearDisplay();
    oled.setCursor(0, 8);
    oled.setTextSize(2);
    oled.println("OH NOES!!!");
    oled.display();

    delay(500);
    tone_blocking(800, 1000);
    delay(500);
    
    oled.clearDisplay();
    oled.setCursor(0, 8);
    oled.setTextSize(2);
    oled.println("BOOM!!!");
    oled.display();

    startPlayback(SND_EXPLOSION, sizeof(SND_EXPLOSION));

    delay(1500);
    oled.clearDisplay();

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
  oled.fillRect(0, 0, 128, 8, SSD1306_BLACK);
  oled.display();
  oled.setCursor(0, 0);

  switch(d)
  {
    case DIFFICULTY_NO_FAIL: 
      duration = 300; // FIVE MINUTES
      moduleCount = 3;
      noFailMode = true;
      oled.println("No Fail Mode...");
      Serial.println("No Fail Mode");
      break;
    case DIFFICULTY_EASY:
      duration = 300; // FIVE MINUTES
      moduleCount = 3;
      noFailMode = false;
      oled.println("Easy Difficulty...");
      Serial.println("Easy Difficulty");
      break;
    case DIFFICULTY_NORMAL:
      duration = 300; // FIVE MINUTES
      moduleCount = 6;
      noFailMode = false;
      oled.println("Normal Difficulty...");
      Serial.println("Normal Difficulty");
      break;
    case DIFFICULTY_HEROIC:
      duration = 360; // EIGHT MINUTES
      moduleCount = 9;
      noFailMode = false;
      oled.println("Heroic Difficulty...");
      Serial.println("Heroic Difficulty");
      break;
    case DIFFICULTY_LEGENDARY:
      duration = 360; // SIX MINUTES
      moduleCount = 9;
      noFailMode = false;
      oled.println("Legendary Difficulty...");
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
