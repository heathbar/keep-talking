#ifndef KEYPAD_H
#define KEYPAD_H

// Internal wiring of the keypad is multiplexed
// 
// PIN 2 --+-+-+
// PIN 7 --+-+-+
// PIN 6 --+-+-+
// PIN 4 --+-+-+
//         | | |
// PIN     3 1 5
// 

enum State {
  UNCHANGED,
  PRESSED,
  RELEASED
};


class Keypad {

  private:
    bool state[12];
    bool last_state[12];
    long last_debounce_time[12];
    short KP1;
    short KP2;
    short KP3;
    short KP4;
    short KP5;
    short KP6;
    short KP7;
    void read(bool *values);

  public:
    Keypad(short pin1, short pin2, short pin3, short pin4, short pin5, short pin6, short pin7);
    
    void refresh(State *states);

};

#endif