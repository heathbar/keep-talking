#ifndef PATIENT_CHAT_H
#define PATIENT_CHAT_H

#include <Arduino.h>
#include "chat.h"

class PatientChat: public Chat
{
  private:
    short rts;
    short cts;
    void write(uint8_t *buf, int len) override;

  public:
    PatientChat(ChatSource id, short rtsPin, short ctsPin);
    
};

#endif
