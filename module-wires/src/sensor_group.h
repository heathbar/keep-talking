#ifndef SENSOR_GROUP_H
#define SENSOR_GROUP_H

#include "sensor.h"

enum SensorGroupEvent
{
  None,
  Disarmed,
  Striken
};

class SensorGroup {
  public:
    SensorGroup();
    SensorGroupEvent update();
    void init(bool sn_last_digit_odd);

  private:
    Sensor *sensors;
    Sensor *solution;
    short countWires(Wire w);
    bool isLastWire(Wire w);
    Sensor *findNthWire(short n);
    Sensor *findLastWire();
    Sensor *findLastWire(Wire w);
};
#endif