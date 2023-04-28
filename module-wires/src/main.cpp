#include <Arduino.h>
#include <patient-chat.h>
#include <serial-number-flags.h>
#include "sensor_group.h"
#include "wire.h"

#define STATUS_GND 11
#define STATUS_RED 12
#define STATUS_GRN 13

// The next module in the chain is Maze which uses NeoPixelBus which notoriously drops 
// packets. So we have implemented a RTS/CTS system called PatientChat to make sure 
// Maze is ready to receive before we send any messages.
#define RTS 2
#define CTS 3

bool disarmed = false;
bool detonated = false;
bool last_digit_odd = false;

SensorGroup sensors;

PatientChat chat(ChatSource::Wires, RTS, CTS);
ChatMessage msg;


void setup()
{
  chat.begin();

  pinMode(STATUS_GND, OUTPUT);
  pinMode(STATUS_RED, OUTPUT);
  pinMode(STATUS_GRN, OUTPUT);
  
  digitalWrite(STATUS_GND, LOW);
  digitalWrite(STATUS_RED, HIGH);
  digitalWrite(STATUS_GRN, LOW);

  sensors.init(true);
}

void loop()
{

  if (chat.receive(&msg))
  {
    if (msg.message == MessageType::Reset)
    {
      sensors.init(msg.data & SN_LAST_DIGIT_ODD);
      detonated = false;
      disarmed = false;

      digitalWrite(STATUS_RED, HIGH);
      digitalWrite(STATUS_GRN, LOW);
    }
    else if (msg.message == MessageType::Detonate)
    {
      detonated = true;
    }
  }

  if (!disarmed && !detonated)
  {
    SensorGroupEvent event = sensors.update();

    if (event == SensorGroupEvent::Disarmed)
    {
      disarmed = true;
      
      digitalWrite(STATUS_RED, LOW);
      digitalWrite(STATUS_GRN, HIGH);
      
      chat.send(MessageType::Disarm);
    }
    else if (event == SensorGroupEvent::Striken)
    {
      chat.send(MessageType::Strike);
    }
  }
}

