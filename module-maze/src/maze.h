#ifndef MAZE_H
#define MAZE_H

#include <NeoPixelBus.h>

const int brightness = 5;

enum Direction
{
  N = 1 << 0,
  S = 1 << 1,
  E = 1 << 2,
  W = 1 << 3
};

struct Element
{
  short x;
  short y;
  RgbColor color;
};

struct Maze {
  Element marker1;
  Element marker2;
  short walls[64];

  short getWalls(short x, short y)
  {
    return walls[x + y * 8];
  }

  bool is_move_allowed(Element from, Direction dir)
  {
    short walls = getWalls(from.x, from.y);

    if (walls & dir)
    {
      return false;
    }
    else
    {
      return true;
    }
  }

};

bool are_same_location(Element e1, Element e2)
{
  return e1.x == e2.x && e1.y == e2.y;
}


Maze mazes[6] = {

  // Maze 1
  {
    { 1, 0, RgbColor(0, brightness, 0)},
    { 0, 5, RgbColor(0, brightness, 0)},
    {
      N + W,
      N,
      N,
      N + S + E,
      N + W,
      N + E,
      N + W,
      N + E,
      W + E,
      W + E,
      W,
      N,
      S + E,
      W + S + E,
      W + E,
      W + E,
      W + E,
      W + E,
      W + S + E,
      W,
      N + S,
      N,
      S + E,
      W + E,
      W + S + E,
      W + S,
      N + E,
      W + S,
      N + S + E,
      W,
      N + E,
      W + E,
      N + W,
      N + S + E,
      W + S,
      N + S,
      N + E,
      W + E,
      W + E,
      W + E,
      W + S,
      N + S,
      N + S,
      N + E,
      W + S,
      E,
      W + E,
      W + E,
      N + W + E,
      N + W,
      N + E,
      W + E,
      N + W,
      S + E,
      W + E,
      W + E,
      W + S,
      S + E,
      W + S,
      S,
      S,
      N + S,
      S + E,
      W + S + E
    }
  },

  // Maze 2
  {
    { 3, 1, RgbColor(0, brightness, 0)},
    { 2, 7, RgbColor(0, brightness, 0)},
    {
      N + W,
      N + S + E,
      N + W,
      N + E,
      N + W + S,
      N + S,
      N + S,
      N + E,

      W + S,
      N + E,
      W + E,
      W + S + E,
      N + W,
      N + S,
      N + S,
      E,
      
      N + W,
      S + E,
      W,
      N + S,
      S + E,
      N + W + S,
      N,
      E,
      
      W,
      N + S,
      S,
      N,
      N,
      N,
      E,
      W + S + E,
      
      W,
      N + E,
      N + W + E,
      W + S + E,
      W + S + E,
      W + E,
      W + S,
      N + E,
      
      W + E,
      W + S,
      S,
      N + S,
      N,
      E,
      N + W + S,
      S + E,
      
      W + S,
      N + E,
      N + W + S,
      N + S,
      S + E,
      W + S,
      N + S,
      N + E,
      
      N + W + S,
      S,
      N + S,
      N + S,
      N + S,
      N + S,
      N + S,
      S + E
    }
  },


  // Maze 3
  {
    { 5, 3, RgbColor(0, brightness, 0)},
    { 7, 5, RgbColor(0, brightness, 0)},
    {
      N + W,
      N + E,
      N + W,
      N + S,
      N + E,
      N + W,
      N + E,
      N + W + E,

      W + E,
      W + S,
      S,
      N + E,
      W + S + E,
      W + E,
      W + E,
      W + E,

      W + S + E,
      N + W,
      N + E,
      W + E,
      N + W,
      S + E,
      W + E,
      W + E,
      
      N + W,
      E,
      W + S,
      0,
      S + E,
      N + W + E,
      W + E,
      W + E,
      
      W + E,
      W + S + E,
      N + W,
      E,
      N + W,
      E,
      W,
      E,
      
      W + S,
      N + E,
      W + E,
      W + E,
      W + E,
      W + S,
      E,
      W + S + E,
      
      N + W + E,
      W + S,
      E,
      W + S,
      S + E,
      N + W + S,
      S,
      N + E,
      
      W + S,
      N + S,
      S,
      N + S,
      N + S,
      N + S,
      N + S,
      S + E
    }
  },

  // Maze 4
  {
    { 7, 1, RgbColor(0, brightness, 0)},
    { 1, 3, RgbColor(0, brightness, 0)},
    {
      N + W,
      N + S,
      N + S,
      N + S,
      N,
      N + S + E,
      N + W,
      N + E,

      W,
      N + S,
      W + S,
      N + E,
      W + S,
      N,
      E,
      W + E,
      
      W + S,
      N + S,
      N + E,
      W + S,
      N + S,
      E,
      W + E,
      W + E,
      
      N + W,
      N + S + E,
      W,
      N + S,
      N + S,
      E,
      W + S,
      E,
      
      W + E,
      N + W,
      S,
      N,
      N,
      S,
      N + S + E,
      W + E,
      
      W + S,
      E,
      N + W,
      S + E,
      W + S,
      N + S,
      N,
      E,
      
      N + W,
      S + E,
      W + S,
      N + E,
      N + W + S,
      N,
      S + E,
      W + E,
      
      W + S + E,
      N + W + S,
      N + S,
      S,
      N + S,
      S + E,
      N + W + S,
      S + E
    }
  },

  // Maze 5
  {
    { 1, 4, RgbColor(0, brightness, 0)},
    { 6, 4, RgbColor(0, brightness, 0)},
    {
      N + W,
      N,
      N + S,
      N + E,
      N + W,
      N + E,
      N + W,
      N + E,

      W + S + E,
      W + E,
      N + W,
      S + E,
      W + E,
      W + S,
      S + E,
      W + E,
      
      N + W,
      E,
      W + E,
      N + W,
      S,
      N + S,
      N + E,
      W + E,
      
      W + E,
      W + E,
      W + E,
      W + E,
      N + W + E,
      N + W,
      S + E,
      W + E,
      
      W + E,
      W + S + E,
      W,
      S,
      E,
      W + E,
      N + W + E,
      W + E,
      
      W + S,
      N,
      E,
      N + W + S,
      S,
      E,
      W + S,
      E,
      
      N + W + S,
      S + E,
      W + E,
      N + W,
      N + S,
      E,
      N + W,
      E,
      
      N + W + S,
      N + S,
      S,
      S + E,
      N + W + S,
      S,
      S,
      S + E
    }
  },

  // Maze 6
  {
    { 6, 2, RgbColor(0, brightness, 0)},
    { 3, 6, RgbColor(0, brightness, 0)},
    {
      N + W + E,
      N + W + E,
      N,
      N + S,
      N + S,
      N + E,
      N + W + S,
      N + W,

      W + E,
      W + S,
      E,
      N + W + S,
      N + S,
      0,
      N + S,
      E,
      
      W,
      N + S,
      S + E,
      N + W,
      N + S,
      S,
      N + E,
      W + E,
      
      W,
      N,
      N + S,
      S,
      N + S,
      N + E,
      W + S + E,
      W + E,
      
      W + S + E,
      W + E,
      N + W,
      N + S,
      N + E,
      W,
      N + S,
      E,
      
      N + W,
      S,
      S,
      N + S + E,
      W + E,
      W + E,
      N + W,
      S + E,
      
      W + E,
      N + W,
      N + S,
      N + S + E,
      W + S,
      S + E,
      W + S,
      N + E,
      
      W + S,
      S,
      N + S,
      N + S,
      N + S,
      N + S,
      N + S,
      S + E
    }
  }
};

#endif