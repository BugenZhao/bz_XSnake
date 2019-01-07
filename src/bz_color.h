#ifndef SNAKE_BZ_COLOR_H_
#define SNAKE_BZ_COLOR_H_

#define _WIN32_WINNT 0x0501
#include <windows.h>

enum Color
{
    black = 0x0,
    red = 0x4,
    green = 0x2,
    blue = 0x1,
    yellow = red | green,
    cyan = green | blue,
    magenta = blue | red,
    white = red | green | blue,
};

int last_color = 0;

inline void set_color(Color color, bool intensity = true, bool background = false)
{
    int c = (color | (intensity * 8)) * (background ? 16 : 1);
    if (c == last_color)
        return;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)c);
    last_color = c;
}

inline void set_color(Color fore_color, Color back_color)
{
    int c = (fore_color | 0) | ((back_color | 8) * 16);
    if (c == last_color)
        return;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)c);
    last_color = c;
}

inline void set_color(Color fore_color, Color back_color, bool fore_intensity, bool back_intensity)
{
    int c = (fore_color | (fore_intensity * 8)) | ((back_color | (back_intensity * 8)) * 16);
    if (c == last_color)
        return;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)c);
    last_color = c;
}

inline void set_color(short c)
{
    if (c == last_color)
        return;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)c);
    last_color = c;
}

#endif
