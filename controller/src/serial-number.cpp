#include <Arduino.h>
#include <serial-number-flags.h>
#include "serial-number.h"

SerialNumber::SerialNumber(const char *num, short flgs)
{
  number = num;
  flags = flgs;
}


void SerialNumber::randomizeIndicators()
{
  short ind = random(8);
  flags |= ind << 2;

  ctrl = (flags >> 2) & 0x1;
  alt = (flags >> 3) & 0x1;
  etp = (flags >> 4) & 0x1;
}
