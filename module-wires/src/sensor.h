#ifndef SENSOR_H
#define SENSOR_H

#include "wire.h"

class Sensor {

  public:
    short wire_num;
    
    Sensor(short pinNumber);
    bool update();
    bool hasWire();
    Wire initialState();
    Wire currentState();

  private:
    short pin;
    Wire initial_state;
    Wire current_state;
    Wire last_state;
    long last_debounce_time;
    

};
#endif