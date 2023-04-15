#ifndef SERIAL_NUMBER_FLAGS_H
#define SERIAL_NUMBER_FLAGS_H

enum SerialNumberFlags
{
  SN_CONTAINS_VOWEL = 0x1,
  SN_LAST_DIGIT_ODD = 0x2,
  IND_CTRL = 0x04,
  IND_ALT = 0x08,
  IND_ETP = 0x16
};

#endif
