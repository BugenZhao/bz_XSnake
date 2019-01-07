#ifndef SNAKE_BZ_SNAKE_H_
#define SNAKE_BZ_SNAKE_H_

#include "bz_base_coord.h"
#include "bz_color.h"

class Snake
{
  public:
    base_coord coord;
    Snake *next = nullptr;
    Color color;

    Snake(short _x, short _y) : coord(_x, _y), color(black) {}

    Snake(short _x, short _y, Color _color) : coord(_x, _y), color(_color) {}

    Snake(base_coord c) : coord(c), color(black) {}

    Snake(base_coord c, Color _color) : coord(c), color(_color) {}
};
#endif
