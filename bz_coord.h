
#ifndef SNAKE_BZ_COORD_H_
#define SNAKE_BZ_COORD_H_

#include <algorithm>
#include <conio.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <stdexcept>
#include <thread>
#include <vector>
#include <windows.h>
#include "bz_base_coord.h"

class bz_coord : public base_coord
{
  public:
    enum Arrow
    {
        up,
        down,
        left,
        right
    };

  private:
    static constexpr short max_coord = std::numeric_limits<short>::max();
    short min_X, min_Y, max_X, max_Y;

  public:
    bz_coord() { base_coord(), min_X = min_Y = 0, max_X = max_Y = max_coord; }

    bz_coord(short x, short y, short min_x = 0, short min_y = 0,
             short max_x = max_coord, short max_y = max_coord);

  private:
    std::map<int, Arrow> arrow =
        {{72, up},
         {80, down},
         {75, left},
         {77, right}};

    bool check(short, short);

  private:
    short dxs[4] = {0, 0, -1, 1};
    short dys[4] = {-1, 1, 0, 0};

  public:
    void set_dx(short);

    void set_dy(short);

    inline void refresh_pos();

    short x() { return X; }

    short y() { return Y; }

    bool set_x(short);

    bool set_y(short);

    inline bool move_to(short, short);

    inline bool move_to(const base_coord &);

    bool move(short, short);

    bool move(const bz_coord &);

    bool move(Arrow);

    bool arrow_move();

    void arrow_move_continuous(int, bool = false);

    inline void fill_in(const std::string &);

    inline void fill_in(const char &);

    inline void fill_in(const char *&);

    inline void fill_in(const wchar_t *);
};

bz_coord::bz_coord(short x, short y, short min_x, short min_y,
                   short max_x, short max_y)
{
    if (max_x <= min_x || max_y <= min_y)
        throw std::range_error("");
    min_X = min_x, min_Y = min_y;
    max_X = max_x, max_Y = max_y;
    set_x(x), set_y(y);
    refresh_pos();
}

bool bz_coord::check(short x, short y)
{
    return x >= min_X && y >= min_Y &&
           x < max_X && y < max_Y;
}

void bz_coord::set_dx(short dx)
{
    dxs[2] = -dx, dxs[3] = dx;
}

void bz_coord::set_dy(short dy)
{
    dys[0] = -dy, dys[1] = dy;
}

void bz_coord::refresh_pos()
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), *this);
}

bool bz_coord::set_x(short x)
{
    if (check(x, min_Y))
        X = x;
    else
        return false;
    return true;
}

bool bz_coord::set_y(short y)
{
    if (check(min_X, y))
        Y = y;
    else
        return false;
    return true;
}

bool bz_coord::move_to(short x, short y)
{
    if (check(x, y))
        X = x, Y = y;
    else
        return false;
    refresh_pos();
    return true;
}

bool bz_coord::move_to(const base_coord &bc)
{
    return move_to(bc.X, bc.Y);
}

bool bz_coord::move(short dx, short dy)
{
    if (check(X + dx, Y + dy))
        X += dx, Y += dy;
    else
        return false;
    refresh_pos();
    return true;
}

bool bz_coord::move(const bz_coord &mv_c)
{
    if (check(X + mv_c.X, Y + mv_c.Y))
        X += mv_c.X, Y += mv_c.Y;
    else
        return false;
    refresh_pos();
    return true;
}

bool bz_coord::move(Arrow a)
{
    if (check(X + dxs[a], Y + dys[a]))
        X += dxs[a], Y += dys[a];
    else
        return false;
    refresh_pos();
    return true;
}

bool bz_coord::arrow_move()
{
    int key = 0;
    while (key != 224)
        key = getch();
    while (arrow.count(key) == 0)
        key = getch();
    return move(arrow[key]);
}

void bz_coord::arrow_move_continuous(int termi_key, bool attach_224)
{
    int key = 0;
    while (true)
    {
        key = getch();
        if (!attach_224 && key == termi_key)
            break;
        if (key != 224)
            continue;
        key = getch();
        if (attach_224 && key == termi_key)
            break;
        move(arrow[key]);
    }
}

void bz_coord::fill_in(const std::string &s)
{
    std::cout << s;
    refresh_pos();
}

void bz_coord::fill_in(const char &c)
{
    printf("%c", c);
    refresh_pos();
}

void bz_coord::fill_in(const char *&s)
{
    printf(s);
    refresh_pos();
}

void bz_coord::fill_in(const wchar_t *s)
{
    wprintf(s);
    refresh_pos();
}

#endif //SNAKE_BZ_COORD_H_
