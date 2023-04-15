#include <Arduino.h>
#include "sensor_group.h"

#define NUM_SENSORS 6

SensorGroup::SensorGroup()
{
}

void SensorGroup::init(bool sn_last_digit_odd)
{
  sensors = (Sensor *)malloc(6 * sizeof(Sensor));

  sensors[0] = Sensor(A0);
  sensors[1] = Sensor(A1);
  sensors[2] = Sensor(A2);
  sensors[3] = Sensor(A3);
  sensors[4] = Sensor(A4);
  sensors[5] = Sensor(A5);

  short wire_count = 0;

  for (short i = 0; i < NUM_SENSORS; i++)
  {
    if (sensors[i].hasWire())
    {
      sensors[i].wire_num = wire_count;
      wire_count++;
    }
  }

  switch (wire_count)
  {
    case 3:
      if (countWires(Wires::red) == 0)
      {
        solution = findNthWire(2);
      }
      else if (isLastWire(Wires::white))
      {
        solution = findLastWire();
      }
      else if (countWires(Wires::blue) > 1)
      {
        solution = findLastWire(Wires::blue);
      }
      else
      {
        solution = findLastWire();
      }
      break;
    case 4:
      if (countWires(Wires::red) > 1 && sn_last_digit_odd)
      {
        solution = findLastWire(Wires::red);
      }
      else if (isLastWire(Wires::yellow) && countWires(Wires::red) == 0)
      {
        solution = findNthWire(1);
      }
      else if (countWires(Wires::blue) == 1)
      {
        solution = findNthWire(1);
      }
      else if (countWires(Wires::yellow) > 1)
      {
        solution = findLastWire();
      }
      else
      {
        solution = findNthWire(2);
      }
      break;
    case 5:
      if (isLastWire(Wires::black) && sn_last_digit_odd)
      {
        solution = findNthWire(4);
      }
      else if (countWires(Wires::red) == 1 && countWires(Wires::yellow) > 1)
      {
        solution = findNthWire(1);
      }
      else if (countWires(Wires::black) == 0)
      {
        solution = findNthWire(2);
      }
      else
      {
        solution = findNthWire(1);
      }
      break;
    case 6:
      if (countWires(Wires::yellow) == 0 && sn_last_digit_odd)
      {
        solution = findNthWire(3);
      }
      else if (countWires(Wires::yellow) == 1 && countWires(Wires::white) > 1)
      {
        solution = findNthWire(4);
      }
      else if (countWires(Wires::red) == 0)
      {
        solution = findLastWire();
      }
      else
      {
        solution = findNthWire(4);
      }
      break;
  }
}

SensorGroupEvent SensorGroup::update()
{
  for (short i = 0; i < NUM_SENSORS; i++)
  {
    bool changed = sensors[i].update();

    if (changed) 
    {
      if (!sensors[i].hasWire())
      {
        if (sensors[i].currentState().id == solution->currentState().id)
        {
          return SensorGroupEvent::Disarmed;
        }
        else
        {
          return SensorGroupEvent::Striken;
        }
      }
    }
  }
  return SensorGroupEvent::None;
}


short SensorGroup::countWires(Wire w)
{
  short count = 0;
  for (short i = 0; i < NUM_SENSORS; i++)
  {
    if (sensors[i].initialState().id == w.id)
    {
      count++;
    }
  }
  return count;
}

Sensor *SensorGroup::findLastWire()
{
  for (short i = NUM_SENSORS-1; i >= 0; i--)
  {
    if (sensors[i].hasWire())
    {
      return &sensors[i];
    }
  }
}

Sensor *SensorGroup::findLastWire(Wire w)
{
  for (short i = NUM_SENSORS-1; i >= 0; i--)
  {
    if (sensors[i].currentState().id == w.id)
    {
      return &sensors[i];
    }
  }
}

Sensor *SensorGroup::findNthWire(short n)
{
  short count = 0;

  for (short i = 0; i < NUM_SENSORS; i++)
  {
    if (sensors[i].hasWire())
    {
      count++;
      if (count == n)
      {
        return &sensors[i];
      }
    }
  }
}

bool SensorGroup::isLastWire(Wire w)
{
  return findLastWire()->currentState().id == w.id;
}
