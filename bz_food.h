#ifndef SNAKE_BZ_FOOD_H_
#define SNAKE_BZ_FOOD_H_

#include "bz_base_coord.h"
#include "bz_color.h"

class Food
{
  public:
    base_coord coord;
    Color color;

    Food(short _x, short _y, Color _color) : coord(_x, _y), color(_color) {}

    bool operator==(const Food &that) const { return coord == that.coord; }

    bool operator<(const Food &that) const { return coord < that.coord; }
};

#endif
