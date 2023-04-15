#include <Arduino.h>
#include <chat.h>
#include <button.h>

#define VCC 2
#define DATA_PIN 3  // Pin connected to DS    of 74HC595
#define CLOCK_PIN 5 // Pin connected to SH_CP of 74HC595
#define LATCH_PIN 4 // Pin connected to ST_CP of 74HC595

#define COUNTER_PIN_001 10
#define COUNTER_PIN_010 11
#define COUNTER_PIN_100 12

#define STATUS_RED 13
#define STATUS_GRN A0

Button button_blk(A1);
Button button_gry(A2);
Button button_wht(A3);

Chat chat(ChatSource::BinaryCounter);
ChatMessage msg;

bool disarmed = false;
bool detonated = false;

unsigned int counter = 0;
unsigned int last_counter = 0;
long last_debounce = millis();

int solutions[] = { 519, 111, 771, 250, 037, 724, 433, 367, 989, 678, 823, 116, 413, 248, 565, 382, 699, 805, 485, 206 };
int solution;

short counter_map[] = { 0, 9, 8, 7, 6, 5, 1, 2, 3, 4 };

unsigned int refresh(unsigned int led1, unsigned int led2);
void shiftOut(byte myDataOut);
void disarm();

void reset()
{
  digitalWrite(STATUS_RED, HIGH);
  digitalWrite(STATUS_GRN, LOW);

  solution = solutions[random(20)];
  counter = refresh(solution, counter);
  delay(30); // bypass debounce
  counter = refresh(solution, counter);
  
  while (solution == counter)
  {
    solution = solutions[random(20)];
  }
}

void setup()
{
  chat.begin();

  randomSeed(analogRead(5));

  pinMode(VCC, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  pinMode(COUNTER_PIN_001, INPUT);
  pinMode(COUNTER_PIN_010, INPUT);
  pinMode(COUNTER_PIN_100, INPUT);

  pinMode(STATUS_RED, OUTPUT);
  pinMode(STATUS_GRN, OUTPUT);

  digitalWrite(VCC, HIGH);

  reset();
}

void loop()
{

  if (chat.receive(&msg)) 
  {
    if (msg.message == MessageType::Reset)
    {
      reset();
    }
    else if (msg.message == MessageType::Detonate)
    {
      detonated = true;
    }
  }

  if (!detonated && !disarmed)
  {
    counter = refresh(solution, counter);
    delay(10);

    if (button_blk.isPressed())
    {
      if (
        (solution == 699 && counter == 695) ||
        (solution == 805 && counter == 813) ||
        (solution == 485 && counter == 501) ||
        (solution == 206 && counter == 204)
        )
      {
        disarm();
      }
      else
      {
        chat.send(MessageType::Strike);
      }
    }

    if (button_gry.isPressed())
    {
      if (counter % 2 == 1 && solution == counter)
      {
        disarm();
      }
      else
      {
        chat.send(MessageType::Strike);
      }
    }

    if (button_wht.isPressed())
    {
      if (counter % 2 == 0 && solution == counter)
      {
        disarm();
      }
      else
      {
        chat.send(MessageType::Strike);
      }
    }
  }

}


void disarm()
{
  disarmed = true;
  chat.send(MessageType::Disarm);
  digitalWrite(STATUS_GRN, HIGH);
  digitalWrite(STATUS_RED, LOW);
}

unsigned int reverse(unsigned int num)
{
    unsigned int count = (16 - 1); 
    unsigned int tmp = num;         //  Assign num to the tmp 
	     
    num >>= 1; // shift num because LSB already assigned to tmp
    
    while(num)
    {
       tmp <<= 1;  //shift the tmp because alread have the LSB of num 	      
       tmp |= num & 1; // putting the set bits of num
       num >>= 1;
       count--;
    }
    tmp <<= count; //when num become zero shift tmp from the remaining counts
    return tmp;
}

// void prntBits(unsigned int b)
// {
//   for (int i = 7; i >= 0; i--)
//     Serial.print(bitRead(b, i));
// }

unsigned int refresh(unsigned int led1, unsigned int led2)
{
  unsigned int led1_reversed = reverse(led1);

  byte b0 = led2 >> 4;
  byte b1;
  byte b1m = led2 << 4; // MSB 4 bits
  byte b2;
  byte b2l = led1_reversed >> 14; // LSB 2 bits
  byte b3 = led1_reversed >> 6;

  unsigned int value = 0;

  for (short i = 0; i < 10; i++) {
    digitalWrite(LATCH_PIN, 0);

    if (i < 6)
    {
      b1 = b1m;
      b2 = b2l + (1 << (2 + i));
    }
    else
    {
      b1 = b1m + (1 << (i - 6));
      b2 = b2l;
    }

    shiftOut(b0);
    shiftOut(b1);
    shiftOut(b2);
    shiftOut(b3);

    digitalWrite(LATCH_PIN, 1);

    // let things settle
    delay(1);

    int v001 = digitalRead(COUNTER_PIN_001);
    int v010 = digitalRead(COUNTER_PIN_010);
    int v100 = digitalRead(COUNTER_PIN_100);

    if (v001) {
      value += counter_map[i];
    }

    if (v010) {
      value += counter_map[i] * 10;
    }

    if (v100) {
      value += counter_map[i] * 100;
    }
  }

  if (value != last_counter) {
    last_debounce = millis();
  }

  if ((millis() - last_debounce) > 25 && value != counter)
  {
    counter = value;
  }
  last_counter = value;

  return counter;
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