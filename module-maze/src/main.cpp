#include <Arduino.h>
#include <NeoPixelBus.h>
#include <chat.h>
#include <button.h>
#include "maze.h"

#define STATUS_RED A0
#define STATUS_GRN A1

Chat chat(ChatSource::Maze);
ChatMessage msg;

const uint16_t PixelCount = 64;
const uint8_t PixelPin = 2;

#define colorSaturation 5

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor purple(colorSaturation / 2, 0, colorSaturation / 2);

RgbColor black(0);

Button left(4);
Button up(5);
Button down(6);
Button right(7);

Element player = { 0, 0, blue };
Element goal   = { 0, 0, red };

Maze maze;

short winIndex = 10;

bool disarmed = false;
bool detonated = false;

void reset();
void draw(Element e);
void flash(RgbColor c);

void setup()
{
  chat.begin();

  randomSeed(analogRead(A5));
  reset();

  strip.Begin();
  strip.Show();
}

void loop()
{
  strip.ClearTo(black);

  if (chat.receive(&msg))
  {
    switch (msg.message)
    {
      case MessageType::Reset:
        reset();
        break;
      case MessageType::Detonate:
        for (int i = 0; i < 10; i++) {
          flash(red);
          delay(50);
        }
        break;
    }
  }

  if (!disarmed && !detonated)
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
        chat.send(MessageType::Strike);
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
        chat.send(MessageType::Strike);
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
        chat.send(MessageType::Strike);
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
        chat.send(MessageType::Strike);
        flash(red);
      }
    }

    draw(goal);
    draw(player);

    if (are_same_location(goal, player))
    {
      chat.send(MessageType::Disarm);

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
