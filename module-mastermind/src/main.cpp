#include <Arduino.h>
#include <chat.h>
#include "button.h"

#define DATA_PIN 12  // Pin connected to DS    of 74HC595
#define CLOCK_PIN 10 // Pin connected to SH_CP of 74HC595
#define LATCH_PIN 11 // Pin connected to ST_CP of 74HC595

#define MATRIX_IN_1 2
#define MATRIX_IN_2 3
#define MATRIX_IN_3 4
#define MATRIX_IN_4 5
#define MATRIX_IN_5 6
#define MATRIX_IN_6 7

#define BTN_TEST A0
#define GND 13
#define STATUS_GND A1
#define STATUS_RED A2
#define STATUS_GRN A3

Chat chat(ChatSource::Mastermind);
ChatMessage msg;

bool disarmed = false;
bool detonated = false;
bool won = false;

Button test_btn(BTN_TEST);

short solution[4] = { 0, 0, 0, 0 };

int leds[][4] = {

  //     1 = { 0, 0, R, 0 }
  //     2 = { B, 0, 0, 0 }
  //     4 = { G, 0, 0, 0 }
  //     8 = { R, 0, 0, 0 }
  //    16 = { 0, 0, B, 0 }
  //    32 = { 0, 0, G, 0 }
  //    64 = { 0, B, 0, 0 }
  //   128 = { 0, G, 0, 0 }
  //   256 = { 0, R, 0, 0 }
  //   512 = { 0, 0, 0, B }
  //  1024 = { 0, 0, 0, G }
  //  2048 = { 0, 0, 0, R }

  { 1120, 2114, 2084, 1192 }, // OBGG BBOR GOGR RGGG
  { 776,  1058, 2113, 1160 }, // RROB BOGG OBRR RGOG
  { 792,  1124, 612,  2307 }, // RRBB GBGG GBGB BRRR
  { 2306, 600,  592,  1172 }, // BROR RBBB OBBB GGBG
  { 1282, 2053, 610,  2208 }, // BROG GORR BBGB OGGR
  { 1300, 616,  2049, 1091 }, // GRBG RBGB OORR BBRG
  { 1090, 2324, 578,  768  }, // BBOG GRBR BBOB OROB
  { 1312, 2312, 800,  579  }, // ORGG RROR ORGB BBRB
  { 521,  2130, 1314, 2181 }, // RORB BBBR BRGG GGRR
  { 2210, 2338, 2152, 1044 }, // BGGR BRGR RBGR GOBG
  { 2178, 1283, 1029, 2117 }, // BGOR BRRG GORG GBRR
  { 1152, 676,  577,  1156 }, // OGOG GGGB OBRB GGOG
  { 2305, 1304, 2068, 581  }, // ORRR RRBG GOBR GBRB
  { 2053, 784,  656,  520  }, // GORR ORBB OGBB ROOB
  { 1096, 2216, 1040, 641  }, // RBOG RGGR OOBG OGRB
};

short misdirection = 0;

void readButtonState(short state[4]);
void flashLeds(int leds, int num);
void setLeds(int leds);
void shiftOut(byte myDataOut);
void disarm();
bool isValidGuess(short state[4]);
short validate(short state[4]);


void reset()
{
  disarmed = false;
  detonated = false;
  won = false;

  digitalWrite(STATUS_RED, HIGH);
  digitalWrite(STATUS_GRN, LOW);

  solution[0] = 1 << random(6);
  solution[1] = 1 << random(6);
  solution[2] = 1 << random(6);
  solution[3] = 1 << random(6);

}

void setup()
{
  chat.begin();

  int seed1 = analogRead(A5);
  int seed2 = analogRead(A6);
  randomSeed(seed1 + seed2);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  pinMode(STATUS_GND, OUTPUT);
  pinMode(STATUS_RED, OUTPUT);
  pinMode(STATUS_GRN, OUTPUT);

  pinMode(MATRIX_IN_1, INPUT_PULLUP);
  pinMode(MATRIX_IN_2, INPUT_PULLUP);
  pinMode(MATRIX_IN_3, INPUT_PULLUP);
  pinMode(MATRIX_IN_4, INPUT_PULLUP);
  pinMode(MATRIX_IN_5, INPUT_PULLUP);
  pinMode(MATRIX_IN_6, INPUT_PULLUP);

  pinMode(GND, OUTPUT);
  pinMode(BTN_TEST, INPUT_PULLUP);

  digitalWrite(GND, LOW);
  digitalWrite(STATUS_GND, LOW);
  digitalWrite(STATUS_RED, HIGH);
  digitalWrite(STATUS_GRN, LOW);

  reset();
}

void loop()
{

  if (chat.receive(&msg)) 
  {
    switch (msg.message)
    {
      case MessageType::Reset:
        reset();
        break;
    
      case MessageType::Detonate:
        detonated = true;
        flashLeds(2313, 10); // 2313 = all RED
        break;
      case MessageType::Win:
        won = true;
        break;
    }
  }

  if (!detonated && !disarmed && !won)
  {
    ButtonState btn = test_btn.refresh();
    if (btn == PRESSED)
    {
      short state[4];
      readButtonState(state);

      if (!isValidGuess(state))
      {
        chat.send(MessageType::Strike);
      }
      else
      {
        int valid_columns = validate(state);

        if (valid_columns == 15)
        {
          disarm();
          flashLeds(1188, 3); // 1188 = all GREEN
        }
        else
        {
          int lights = leds[valid_columns][misdirection];
          misdirection = (misdirection + random(1, 4)) % 4; 
          setLeds(lights);
        }
      }
    }
    else if (btn == RELEASED)
    {
      setLeds(0);
    }
    delay(1);
  }

}


void disarm()
{
  disarmed = true;
  chat.send(MessageType::Disarm);
  digitalWrite(STATUS_GRN, HIGH);
  digitalWrite(STATUS_RED, LOW);
}

// void prntBits(unsigned int b)
// {
//   for (int i = 7; i >= 0; i--)
//     Serial.print(bitRead(b, i));
//   Serial.print(" ");
// }


void readButtonState(short state[4])
{
  for (short i = 0; i < 4; i++) {
    digitalWrite(LATCH_PIN, LOW);

    byte hi = 0;
    byte lo = 15 - (1 << (3-i));

    // prntBits(hi); 
    // prntBits(lo);
    
    shiftOut(hi);
    shiftOut(lo);

    digitalWrite(LATCH_PIN, HIGH);

    // let things settle
    delay(1);

    int in1 = !digitalRead(MATRIX_IN_1);
    int in2 = !digitalRead(MATRIX_IN_2);
    int in3 = !digitalRead(MATRIX_IN_3);
    int in4 = !digitalRead(MATRIX_IN_4);
    int in5 = !digitalRead(MATRIX_IN_5);
    int in6 = !digitalRead(MATRIX_IN_6);

    unsigned int in = (in1) | (in2 << 1) | (in3 << 2) | (in4 << 3) | (in5 << 4) | (in6 << 5);

    // prntBits(in);

    state[i] = 0;

    if (in1)
    {
      state[i] += 1;
    }
    if (in2)
    {
      state[i] += 2;
    }
    if (in3)
    {
      state[i] += 4;
    }
    if (in4)
    {
      state[i] += 8;
    }
    if (in5)
    {
      state[i] += 16;
    }
    if (in6)
    {
      state[i] += 32;
    }
  }
}


void flashLeds(int leds, int num)
{
  for (int i = 0; i < num; i++)
  {
    setLeds(leds);
    delay(150);
    setLeds(0);
    delay(150);
  }
}

void setLeds(int leds)
{
  digitalWrite(LATCH_PIN, LOW);

    byte hi = leds >> 4;
    byte lo = leds << 4 | 15;

    // prntBits(hi); 
    // prntBits(lo);
    
    shiftOut(hi);
    shiftOut(lo);

    digitalWrite(LATCH_PIN, HIGH);
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

bool isValidGuess(short state[4])
{
  for (short i = 0; i < 4; i++)
  {
    if (state[i] != 1 && 
        state[i] != 2 && 
        state[i] != 4 && 
        state[i] != 8 && 
        state[i] != 16 && 
        state[i] != 32)
    {
      return false;
    }
  }
  return true;
}

short validate(short state[4])
{
  short res = 0;
  if (state[0] == solution[0])
  {
    res += 1;
  }
  if (state[1] == solution[1])
  {
    res += 2;
  }
  if (state[2] == solution[2])
  {
    res += 4;
  }
  if (state[3] == solution[3])
  {
    res += 8;
  }
 
  return res;
}
