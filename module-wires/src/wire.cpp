#include "wire.h"

Wire Wires::yellow = { 'Y', 80,  140 };
Wire Wires::black  = { 'K', 225, 285 };
Wire Wires::white  = { 'W', 395, 460 };
Wire Wires::blue   = { 'B', 590, 650 };
Wire Wires::red    = { 'R', 750, 810 };
Wire Wires::open   = { 'O', 999, 1024 };

Wire Wires::list[6] = {
  Wires::yellow,
  Wires::black,
  Wires::white,
  Wires::blue,
  Wires::red,
  Wires::open
};
