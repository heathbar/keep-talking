#include "clock.h"
#include <PCM.h>
#include "sounds.h"

Clock::Clock(short seconds)
{
  length = 1000 * (long)seconds;
  remaining = length;
  lastDisplay = -1;
  lastSound = -1;
  strikes = 0;
  matrix.begin(0x70);
  matrix.drawColon(true);

  matrix.writeDigitNum(0, 0);
  matrix.writeDigitNum(1, 0);
  matrix.writeDigitNum(3, 0);
  matrix.writeDigitNum(4, 0);
  matrix.writeDisplay();

  startTime = millis();
  refresh();
}

void Clock::start()
{
  startTime = millis();
}

TimeChanges Clock::refresh()
{
  long realElapsed = millis() - startTime;
  long adjustedElapsed = realElapsed;

  if (strikes == 1)
  {
    adjustedElapsed = strike1time + ((realElapsed - strike1time) * 5 / 4);
  }
  else if (strikes == 2)
  {
    adjustedElapsed = strike1time + ((strike2time - strike1time) * 5 / 4) + ((realElapsed - strike2time) * 3 / 2);
  }

  remaining = length - adjustedElapsed;
  int minutes = remaining / 1000 / 60;
  int seconds = remaining / 1000 % 60;

  if (minutes > 0) {
    if (minutes < 10) {
      digits[0] = 0;
      digits[1] = minutes;
    } else {
      digits[0] = minutes / 10;
      digits[1] = minutes % 1;
    }

    if (seconds < 10) {
      digits[2] = 0;
      digits[3] = seconds;
    } else {
      digits[2] = seconds / 10;
      digits[3] = seconds % 10;
    }

    if (seconds != lastDisplay)
    {
      lastDisplay = seconds;

      tick();
      matrix.writeDigitNum(0, digits[0]);
      matrix.writeDigitNum(1, digits[1]);
      matrix.writeDigitNum(3, digits[2]);
      matrix.writeDigitNum(4, digits[3]);
      matrix.writeDisplay();
    }

  } else {
    matrix.writeDigitRaw(2, 0xE); // turn on both colons
    if (seconds < 10) {
      digits[0] = 0;
      digits[1] = seconds;
    } else {
      digits[0] = seconds / 10;
      digits[1] = seconds % 10;
    }

    int thousandths = remaining % 1000;
    int hundredths = thousandths / 10;
    int tenths = hundredths / 10;

    if (hundredths < 10) {
      digits[2] = 0;
      digits[3] = hundredths;
    } else {
      digits[2] = tenths;
      digits[3] = hundredths % 10;
    }

    if (hundredths != lastDisplay)
    {
      lastDisplay = hundredths;

      if (lastSound != tenths && (tenths == 0 || tenths == 5))
      {
        lastSound = tenths;
        tick();
      }
      
      matrix.writeDigitNum(0, digits[0]);
      matrix.writeDigitNum(1, digits[1]);
      matrix.writeDigitNum(3, digits[2]);
      matrix.writeDigitNum(4, digits[3]);
      matrix.writeDisplay();
    }

    // When timer is under 1 minute, the display moves the digits around for effect.
    // Here we remap digits back to original positions before they are sent to the other modules
    digits[3] = digits[1];
    digits[2] = digits[0];
    digits[1] = 0;
    digits[0] = 0;
  }

  TimeChanges tc;
  for (short i = 0; i < 4; i++)
  {
    if (digits[i] != prevDigits[i])
    {
      tc.digits[i] = digits[i];
    }
    else
    {
      tc.digits[i] = -1;
    }
    prevDigits[i] = digits[i];
  }
  return tc;
}

bool Clock::isExpired()
{
  return remaining <= 0;
}

void Clock::showColon(bool flag)
{
  matrix.drawColon(flag);
}

void Clock::strike()
{
  strikes++;
  if (strikes == 1)
  {
    strike1time = millis();
  }
  else if (strikes == 2)
  {
    strike2time = millis();
  }
}
