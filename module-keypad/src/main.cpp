#include <Arduino.h>
#include <Tone.h>
#include <chat.h>
#include <serial-number-flags.h>
#include "keypad.h"
#include "number-sequence.h"
#include "morse-player.h"


#define KP1 10
#define KP2 9
#define KP3 8
#define KP4 7
#define KP5 6
#define KP6 5
#define KP7 4

#define STATUS_RED 2
#define STATUS_GRN 3

#define STAGE1_RED A0
#define STAGE1_GRN A1
#define STAGE2_RED A2
#define STAGE2_GRN A3
#define STAGE3_RED A4
#define STAGE3_GRN A5

#define INITIAL 0
#define WIN 4

Chat chat(ChatSource::Keypad);
ChatMessage msg;

Tone tone1;
Tone tone2;

Keypad keypad(KP1, KP2, KP3, KP4, KP5, KP6, KP7);
NumberSequence pin_seq;
MorsePlayer mp(&tone1);


const int DTMF_freq1[] = { 1336, 1209, 1336, 1477, 1209, 1336, 1477, 1209, 1336, 1477, 1209, 1633 };
const int DTMF_freq2[] = {  941,  697,  697,  697,  770,  770,  770,  852,  852,  852,  941,  941 };

String morses[] = 
{
  "... --- ...",
  "-... ... --- -...",
  ".--. .. ",
  ".... . .-.. .--.",
  "-.-. --.- -..",
  "-.-. ....-",
  "- -. -",
  ".-- .. .-. .",
  ".-. .--- .--.",
  "--- .... -. ---",
  ".... .--- .--.",
  "- ..-. - ..",
  ".-. - ..-. --",
  "-... --- --- -..."
};

short pin_codes[][5] = 
{
  { 8, 0, 8, 3, 4 },
  { 3, 4, 1, 8, 6 },
  { 3, 1, 4, 1, 5 },
  { 4, 1, 1, 3, 7 },
  { 1, 9, 1, 1, 6 },
  { 4, 6, 8, 3, 1 },
  { 7, 3, 1, 9, 2 },
  { 6, 3, 8, 6, 1 },
  { 3, 2, 7, 1, 7 },
  { 6, 4, 8, 8, 1 },
  { 5, 2, 0, 8, 6 },
  { 2, 1, 6, 8, 3 },
  { 4, 0, 4, 8, 6 },
  { 1, 9, 3, 7, 4 },
};

short stage = INITIAL;
bool playing = false;

bool disarmed = false;
bool detonated = false;

void reset(bool even, bool ctrl);
void playDTMF(uint8_t number);
void turnLEDsRed();


void setup() {
  chat.begin();
  tone1.begin(11);
  tone2.begin(12);

  pinMode(STATUS_GRN, OUTPUT);
  pinMode(STATUS_RED, OUTPUT);
  pinMode(STAGE1_GRN, OUTPUT);
  pinMode(STAGE1_RED, OUTPUT);
  pinMode(STAGE2_GRN, OUTPUT);
  pinMode(STAGE2_RED, OUTPUT);
  pinMode(STAGE3_GRN, OUTPUT);
  pinMode(STAGE3_RED, OUTPUT);

  randomSeed(analogRead(A6));

  // Stuff kept starting out not random without this mess...
  random(10);
  random(100);
  random(1000);
  random(10000);

  reset(false, false);
}


void loop() {
  
  if (chat.receive(&msg))
  {
    switch (msg.message)
    {
      case MessageType::Reset:
        bool even = !(msg.data & SN_LAST_DIGIT_ODD);
        bool ctrl = msg.data & IND_CTRL;
        reset(even, ctrl);
        break;
      case MessageType::Detonate:
        detonated = true;
        turnLEDsRed();
        break;
    }
  }

  if (!disarmed && !detonated)
  {
    State states[12];
    keypad.refresh(states);

    for (short i = 0; i < 12; i++)
    {
      if (states[i] == PRESSED)
      {
        tone1.stop();
        tone2.stop();
        playing = false;

        playDTMF(i);

        KeyPressedOutcome response = pin_seq.keyPressed(i);

        if (response == INCORRECT)
        {
          chat.send(MessageType::Strike);
          playing = true;
          turnLEDsRed();
          delay(150);
        }
        else if (response == CORRECT)
        {
          stage++;
          playing = true;

          int r = random(14);
          mp.setPhrase(morses[r]);
          pin_seq.terminated(5, pin_codes[r]);
        }
      }
      else if (states[i] == RELEASED)
      {
        tone1.stop();
        tone2.stop();
      }
    }

    if (stage == 1)
    {
      digitalWrite(STAGE1_RED, LOW);
      digitalWrite(STAGE2_RED, LOW);
      digitalWrite(STAGE3_RED, LOW);
    }
    else if (stage == 2)
    {
      digitalWrite(STAGE1_GRN, HIGH);
      digitalWrite(STAGE1_RED, LOW);
      digitalWrite(STAGE2_RED, LOW);
      digitalWrite(STAGE3_RED, LOW);
    }
    else if (stage == 3)
    {
      digitalWrite(STAGE1_GRN, HIGH);
      digitalWrite(STAGE2_GRN, HIGH);
      digitalWrite(STAGE1_RED, LOW);
      digitalWrite(STAGE2_RED, LOW);
      digitalWrite(STAGE3_RED, LOW);
    }
    if (stage == WIN)
    {
      chat.send(MessageType::Disarm);
      digitalWrite(STATUS_GRN, HIGH);
      digitalWrite(STATUS_RED, LOW);
      digitalWrite(STAGE1_GRN, HIGH);
      digitalWrite(STAGE2_GRN, HIGH);
      digitalWrite(STAGE3_GRN, HIGH);
      digitalWrite(STAGE1_RED, LOW);
      digitalWrite(STAGE2_RED, LOW);
      digitalWrite(STAGE3_RED, LOW);
      disarmed = true;
      delay(250);

      tone1.stop();
      tone2.stop();
    }
    else if (playing)
    {
      mp.tick();
    }
  }
}

void reset(bool even, bool ctrl)
{
  digitalWrite(STATUS_GRN, LOW);
  digitalWrite(STATUS_RED, HIGH);
  digitalWrite(STAGE1_GRN, LOW);
  digitalWrite(STAGE1_RED, LOW);
  digitalWrite(STAGE2_GRN, LOW);
  digitalWrite(STAGE2_RED, LOW);
  digitalWrite(STAGE3_GRN, LOW);
  digitalWrite(STAGE3_RED, LOW);

  stage = INITIAL;
  disarmed = false;
  detonated = false;

  if (even)
  {
    short digits[2] = { 6, 2 };
    pin_seq.fixed(2, digits);
  }
  else if (ctrl)
  {
    short digits[2] = { 5, 7 };
    pin_seq.fixed(2, digits);
  }
  else
  {
    short digits[2] = { 1, 1 };
    pin_seq.fixed(2, digits);
  }
}

void playDTMF(uint8_t number)
{
  tone1.play(DTMF_freq1[number]);
  tone2.play(DTMF_freq2[number]);
}

void turnLEDsRed()
{
  digitalWrite(STAGE1_GRN, LOW);
  digitalWrite(STAGE1_RED, HIGH);
  digitalWrite(STAGE2_GRN, LOW);
  digitalWrite(STAGE2_RED, HIGH);
  digitalWrite(STAGE3_GRN, LOW);
  digitalWrite(STAGE3_RED, HIGH);
}