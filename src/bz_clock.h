#ifndef SNAKE_BZ_CLOCK_H_
#define SNAKE_BZ_CLOCK_H_

#include <iomanip>
#include <iostream>

class Clock
{
    int m, s;

  public:
    Clock() : m(0), s(0) {}

    Clock(int secs) : m(secs / 60), s(secs % 60) {}

    void set_time(int secs)
    {
        m = secs / 60, s = secs % 60;
    }

    void print()
    {
        std::cout << std::setfill('0') << std::setw(2) << m << ':'
                  << std::setfill('0') << std::setw(2) << s;
    }

    void clear()
    {
        m = 0, s = 0;
    }

    void increment()
    {
        ++s;
        if (s == 60)
            s = 0, ++m;
    }
};

#endif
