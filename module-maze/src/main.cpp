#include <Arduino.h>
#include <NeoPixelBus.h>
#include <SPI.h>
#include <chat.h>
#include <button.h>
#include "maze.h"

#define VCC 2
#define GND 8
#define STATUS_RED A0
#define STATUS_GRN A1

#define PIXEL_PIN 3
#define BRIGHTNESS 5

#define COMS_ADDR 2  // address of the I2C slave that handles coms for us

const uint16_t PixelCount = 64;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PIXEL_PIN);

RgbColor red(BRIGHTNESS, 0, 0);
RgbColor green(0, BRIGHTNESS, 0);
RgbColor blue(0, 0, BRIGHTNESS);
RgbColor purple(BRIGHTNESS / 2, 0, BRIGHTNESS / 2);

RgbColor black(0);

Button left(4);
Button up(5);
Button down(6);
Button right(7);

Element player = { -1, -1, blue };
Element goal   = { -2, -2, red };

Maze maze;

bool disarmed = false;
bool detonated = false;
bool won = false;

void reset();
void draw(Element e);
void flash(RgbColor c);
void sendMessage(char c);

void setup()
{

  Serial.begin(19200);

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  digitalWrite(SS,HIGH);

  pinMode(VCC, OUTPUT);
  pinMode(GND, OUTPUT);
  
  pinMode(STATUS_RED, OUTPUT);
  pinMode(STATUS_GRN, OUTPUT);

  digitalWrite(VCC, HIGH);
  digitalWrite(GND, LOW);

  digitalWrite(STATUS_GRN, LOW);
  digitalWrite(STATUS_RED, HIGH);

  strip.Begin();
  strip.Show();

  randomSeed(analogRead(A5));

  // reset();
}

void loop()
{
  strip.ClearTo(black);

  // ask for messages
  ChatMessage m;
  digitalWrite(SS, LOW);
  int emm = SPI.transfer('m');
  delay(50);
  m.sender = SPI.transfer(0);
  delay(4);
  m.message = (MessageType)SPI.transfer(0);
  delay(4);
  m.data = SPI.transfer(0);

  digitalWrite(SS, HIGH);
  
  if (m.sender != 0 || (uint8_t)m.message != 0 || m.data !=0)
  {  
    Serial.println(String("emm    : ") + String(emm));
    Serial.println(String("sender : ") + String(m.sender));
    Serial.println(String("message: ") + String((byte)m.message));
    Serial.println(String("data   : ") + String(m.data));

    switch (m.message)
    {
      case MessageType::Reset:
        reset();
        break;
      case MessageType::Detonate:
        for (int i = 0; i < 10; i++) {
          flash(red);
          delay(50);
          detonated = true;
        }
        break;

      case MessageType::Win:
        won = true;
        break;
    }
  }

  if (!disarmed && !detonated && !won)
  {
    draw(maze.marker1);
    draw(maze.marker2);

    if (up.isPressed())
    {
      if (maze.is_move_allowed(player, N))
      {
        player.y -= 1;
      }
      else
      {
        sendMessage('s');
        flash(red);
      }
    }

    if (left.isPressed())
    {
      if (maze.is_move_allowed(player, W))
      {
        player.x -= 1;
      }
      else
      {
        sendMessage('s');
        flash(red);
      }
    }

    if (down.isPressed())
    {
      if (maze.is_move_allowed(player, S))
      {
        player.y += 1;
      }
      else
      {
        sendMessage('s');
        flash(red);
      }
    }

    if (right.isPressed())
    {
      if (maze.is_move_allowed(player, E))
      {
        player.x += 1;
      }
      else
      {
        sendMessage('s');
        flash(red);
      }
    }

    draw(goal);
    draw(player);

    if (are_same_location(goal, player))
    {
      sendMessage('d');

      digitalWrite(STATUS_GRN, HIGH);
      digitalWrite(STATUS_RED, LOW);

      disarmed = true;

      for (int i = 10; i >= 0; i--)
      {
        strip.ClearTo(RgbColor(0, i, 0));
        strip.Show();
        delay(65);
      }

    }
  }

  strip.Show();
}

void reset()
{
  maze = mazes[random(6)];

  do {
    player.x = random(0, 7);
    player.y = random(0, 4);
  } while(are_same_location(player, maze.marker1) || are_same_location(player, maze.marker2));

  do {
    goal.x = random(0, 7);
    goal.y = random(4, 8);
  } while(are_same_location(goal, maze.marker1) || are_same_location(goal, maze.marker2));

  detonated = false;
  disarmed = false;
  won = false;

  digitalWrite(STATUS_GRN, LOW);
  digitalWrite(STATUS_RED, HIGH);
}

/**
 * This method maps from an x,y coordinate system (or maybe better to think of 
 * it as rows & columns) to the snakey coordinate system built into the LED grid.
*/
void draw(Element e)
{
  short i;

  if (e.x < 0 || e.x > 7 || e.y < 0 || e.y > 7)
  {
    return;
  }
  
  if (e.y % 2 == 0)
  {
    i = 63 - (e.y * 8 + e.x);
  }
  else
  {
    i = 63 - (e.y * 8 + 7 - e.x);
  }

  strip.SetPixelColor(i, e.color);
}

void flash(RgbColor c)
{
  strip.ClearTo(c);
  strip.Show();
  delay(50);
  strip.ClearTo(black);
  strip.Show();
}

void sendMessage(char c)
{
  digitalWrite(SS, LOW);
  SPI.transfer(c);
  digitalWrite(SS, HIGH);
}