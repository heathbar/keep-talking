#include <Arduino.h>
#include "sensor.h"

#define DEBOUNCE_TIME 25

const short NUM_WIRE_OPTS = 6;

Sensor::Sensor(short pinNumber)
{
  pin = pinNumber;

  pinMode(pin, INPUT_PULLUP);
  int value = analogRead(pin);

  for (short j = 0; j < NUM_WIRE_OPTS; j++)
  {

    if (value > Wires::list[j].min_val && value < Wires::list[j].max_val)
    {
      initial_state = Wires::list[j];
      last_state = Wires::list[j];
      current_state = Wires::list[j];
    }
  }
}

bool Sensor::update()
{
  int value = analogRead(pin);
  Wire next_state;

  for (short j = 0; j < NUM_WIRE_OPTS; j++)
  {
    if (value > Wires::list[j].min_val && value < Wires::list[j].max_val)
    {
      next_state = Wires::list[j];
    }
  }

  if (next_state.id != last_state.id)
  {
    last_debounce_time = millis();
  }
  if ((millis() - last_debounce_time) > DEBOUNCE_TIME)
  {
    if (next_state.id != current_state.id)
    {
      current_state = next_state;
      return true;
    }
  }
  last_state = next_state;
  return false;
}

Wire Sensor::initialState()
{
  return initial_state;
}

Wire Sensor::currentState()
{
  return current_state;
}

bool Sensor::hasWire()
{
  return current_state.id != Wires::open.id;
}
