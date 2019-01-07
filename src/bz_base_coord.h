#ifndef SNAKE_BZ_BASE_COORD_H_
#define SNAKE_BZ_BASE_COORD_H_

#define _WIN32_WINNT 0x0501
#include <windows.h>

class base_coord : public COORD
{
  public:
    base_coord() { X = 0, Y = 0; }

    base_coord(short x, short y) { X = x, Y = y; }

    bool operator==(const base_coord &that) const { return X == that.X && Y == that.Y; }

    bool operator<(const base_coord &that) const { return X * 1000 + Y < that.X * 1000 + that.Y; }
};

#endif
