#include "Arduino.h"
#include "RgbButton.h"
#include <chat.h>

#define debounceTime 25

RgbButton::RgbButton(short red, short green, short blue, short btn, Chat *c)
{
  pinMode(btn, INPUT_PULLUP);
  led = {red, green, blue, 0, 0, 0};
  btnPin = btn;
  btnState = true;
  lastState = true;
  lastDebounceTime = 0;
  lastPressedTime = 0;
  lastReleasedTime = 0;
  pressedCount = 0;
  releaseTime = -1;
  chat = c;
}

void RgbButton::setAnimation(short index)
{
  animation = animations[index];
  solution = animation.solution;
}

RgbButtonEvent RgbButton::refresh()
{
  this->isPressed = false;
  this->isReleased = false;
  long now = millis();

  int reading = digitalRead(this->btnPin);
  if (reading != this->lastState)
  {
    this->lastDebounceTime = now;
  }

  if ((now - this->lastDebounceTime) > debounceTime)
  {
    if (reading != this->btnState)
    {
      this->btnState = reading;

      if (this->btnState == LOW)
      {
        this->isPressed = true;
        lastPressedTime = now;
      }
      else
      {
        this->isReleased = true;
        lastReleasedTime = now;
      }
    }
  }
  this->lastState = reading;

  if (solved)
  {
    setColor(Color::black);
    return RgbButtonEvent::none;
  }
  else if (isPressed)
  {
    return handlePressed();
  }
  else if (isReleased)
  {
    return handleReleased();
  }
  else if (animation.solution == Solution::ReleaseOnTime)
  {
    if (btnState == false)
    {
      // Don't animate when executing the hold & release on time
      return RgbButtonEvent::none;
    }

  }

  //  Run animation
  switch (animation.type)
  {
  case AnimationType::flash1x:
    flash1x(now);
    break;
  case AnimationType::flash2x:
    flash1x(now);
    break;
  case AnimationType::alternate:
    alternate(now);
    break;
  case AnimationType::solid:
    solid(now);
    break;
  }
  return RgbButtonEvent::none;
}

void RgbButton::flash1x(long now)
{
  short progress = now % animation.duration;
  if (progress < (animation.duration / 4))
  {
    setColor(animation.color1);
  }
  else
  {
    setColor(Color::black);
  }
}


void RgbButton::flash2x(long now)
{
  int eighth_time = animation.duration / 8;
  int progress = now % animation.duration;

  if (progress < eighth_time)
  {
    setColor(animation.color1);
  }
  else if (progress < 2 * eighth_time)
  {
    setColor(Color::black);
  }
  else if (progress < 3 * eighth_time)
  {
    setColor(animation.color1);
  }
  else
  {
    setColor(Color::black);
  }
}

void RgbButton::alternate(long now)
{
  short progress = now % animation.duration;
  if (progress < (animation.duration / 2))
  {
    setColor(animation.color1);
  }
  else
  {
    setColor(animation.color2);
  }
}

void RgbButton::solid(long now)
{
  setColor(animation.color1, 4095);
}

void RgbButton::setColor(Color c, int value)
{
  switch (c)
  {
  case Color::black:
    led.redVal = 0;
    led.grnVal = 0;
    led.bluVal = 0;
    break;
  case Color::red:
    led.redVal = value;
    led.grnVal = 0;
    led.bluVal = 0;
    break;
  case Color::green:
    led.redVal = 0;
    led.grnVal = value;
    led.bluVal = 0;
    break;
  case Color::blue:
    led.redVal = 0;
    led.grnVal = 0;
    led.bluVal = value;
    break;
  case Color::cyan:
    led.redVal = 0;
    led.grnVal = value / 4;
    led.bluVal = value / 2;
    break;
  case Color::magenta:
    led.redVal = value / 4;
    led.grnVal = 0;
    led.bluVal = value / 2;
    break;
  case Color::yellow:
    led.redVal = value / 4;
    led.grnVal = value / 2;
    led.bluVal = 0;
    break;
  case Color::white:
    led.redVal = value / 5;
    led.grnVal = value / 2;
    led.bluVal = value;
    break;
  }

  tlc->set(led.redPin, led.redVal);
  tlc->set(led.grnPin, led.grnVal);
  tlc->set(led.bluPin, led.bluVal);
}

RgbButtonEvent RgbButton::handlePressed()
{
  if (solved)
  {
    // If already solved, another press results in strike
    return RgbButtonEvent::stricken;
  }

  switch (animation.solution)
  {
    case Solution::PressOnGreen:
    {
      if (animation.type == AnimationType::solid)
      {
        solved = true;
        return RgbButtonEvent::solved;
      }

      // in alternating sequences, green is always first
      short progress = millis() % animation.duration;
      if (progress < (animation.duration / 2))
      {
        solved = true;
        return RgbButtonEvent::solved;
      }
      return RgbButtonEvent::stricken;
    }
    case Solution::PressTwice:
    {
      pressedCount++;
      if (pressedCount == 2)
      {
        solved = true;
        return RgbButtonEvent::solved;
      }
      return RgbButtonEvent::none;
    }
    case Solution::PressThrice:
    {
      pressedCount++;
      if (pressedCount == 3)
      {
        solved = true;
        return RgbButtonEvent::solved;
      }
      return RgbButtonEvent::none;
    }
    case Solution::Skip:
    {
      return RgbButtonEvent::stricken;
    }
    case Solution::ReleaseOnTime:
    {
      releaseColor = (Color)random(1, 8);
      switch (releaseColor)
      {
      case Color::black:
      case Color::red:
      case Color::cyan:
      case Color::magenta:
        releaseTime = 5;
        break;
      case Color::green:
        releaseTime = 2;
        break;
      case Color::blue:
        releaseTime = 4;
        break;
      case Color::yellow:
        releaseTime = 8;
        break;
      case Color::white:
        releaseTime = 1;
        break;
      }
      setColor((Color)releaseColor);
      return RgbButtonEvent::none;
    }
    default:
      return RgbButtonEvent::none;
  }
}

RgbButtonEvent RgbButton::handleReleased()
{
  switch (animation.solution)
  {
    case Solution::HoldTwoSeconds:
    {
      if ((lastReleasedTime - lastPressedTime) > 2000)
      {
        solved = true;
        return RgbButtonEvent::solved;
      }
      return RgbButtonEvent::stricken;
    }
    case Solution::HoldThreeSeconds:
    {
      if ((lastReleasedTime - lastPressedTime) > 3000)
      {
        solved = true;
        return RgbButtonEvent::solved;
      }
      return RgbButtonEvent::stricken;
    }
    case Solution::ReleaseOnTime:
    {
      if (time[3] == releaseTime || time[2] == releaseTime || time[1] == releaseTime || time[0] == releaseTime)
      {
        solved = true;
        return RgbButtonEvent::solved;
      }
      else
      {
        chat->debug("strike");
        return RgbButtonEvent::stricken;
      }
    }
    default:
      return RgbButtonEvent::none;
  }
}

void RgbButton::setDigit(short index, uint8_t t)
{
  time[index] = t;
}

const Animation RgbButton::animations[] =
{
  {AnimationType::flash1x,     Color::red,     Color::black,  750,  Solution::PressTwice},
  {AnimationType::flash2x,     Color::green,   Color::black,  2000, Solution::Skip},
  {AnimationType::flash2x,     Color::blue,    Color::black,  1750, Solution::Skip},
  {AnimationType::flash1x,     Color::cyan,    Color::cyan,   2410, Solution::PressTwice},
  {AnimationType::flash1x,     Color::magenta, Color::black,  2000, Solution::ReleaseOnTime},
  {AnimationType::flash2x,     Color::yellow,  Color::black,  1500, Solution::PressTwice},
    
  {AnimationType::alternate, Color::green,   Color::magenta,  1620, Solution::PressOnGreen},
  {AnimationType::alternate, Color::green,   Color::blue,     2120, Solution::PressOnGreen},
  {AnimationType::alternate, Color::green,   Color::cyan,     2120, Solution::PressOnGreen},
  {AnimationType::alternate, Color::yellow,  Color::red,      1080, Solution::PressThrice},
  {AnimationType::alternate, Color::yellow , Color::magenta,  2120, Solution::PressThrice},
  {AnimationType::alternate, Color::yellow,  Color::cyan,     1290, Solution::PressThrice},
  {AnimationType::alternate, Color::red,     Color::cyan,     1290, Solution::HoldTwoSeconds},
  {AnimationType::alternate, Color::blue,    Color::magenta,  1080, Solution::HoldTwoSeconds},
  {AnimationType::alternate, Color::blue,    Color::red,      1620, Solution::HoldTwoSeconds},
  
  {AnimationType::solid,      Color::magenta, Color::magenta, 1000, Solution::ReleaseOnTime},
  {AnimationType::solid,      Color::red,     Color::red,     1000, Solution::ReleaseOnTime},
  {AnimationType::solid,      Color::green,   Color::green,   1000, Solution::PressOnGreen},
  {AnimationType::solid,      Color::cyan,    Color::cyan,    1000, Solution::ReleaseOnTime},
  {AnimationType::solid,      Color::yellow,  Color::yellow,  1000, Solution::ReleaseOnTime}
};
