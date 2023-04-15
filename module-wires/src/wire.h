#ifndef WIRE_H
#define WIRE_H

// 3.3kΩ = 90   Yellow
// 10kΩ  = 214  Black
// 22kΩ  = 367  White
// 47kΩ  = 552  Blue
// 100kΩ = 724  Red
// 200kΩ = 843  Green (unused, but here for posterity)

struct Wire
{
  char id;
  int min_val;
  int max_val;
};

class Wires
{
  public:
    static Wire yellow;
    static Wire black;
    static Wire white;
    static Wire blue;
    static Wire red;
    static Wire open;

    static Wire list[6];
};

#endif