#ifndef SERIAL_NUMBER_H
#define SERIAL_NUMBER_H

#include <serial-number-flags.h>

class SerialNumber
{

  public:
    const char *number;
    short flags;
    bool ctrl;
    bool alt;
    bool etp;

    SerialNumber(const char *num, short flags);
    void randomizeIndicators();
};

#endif