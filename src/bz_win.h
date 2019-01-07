#ifndef SNAKE_BZ_WIN_H_
#define SNAKE_BZ_WIN_H_

#define _WIN32_WINNT 0x0501
#include <windows.h>

inline void cls()
{
    system("cls");
}

inline void pause(bool echo = true)
{
    if (echo)
        system("pause");
    else
        system("pause >nul");
}

inline void hide_cursor()
{
    const CONSOLE_CURSOR_INFO cursor{100, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

inline void show_cursor(DWORD size = 100)
{
    const CONSOLE_CURSOR_INFO cursor{size, 1};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

inline void utf8_fix()
{
    system("chcp 65001 >nul");
}

inline bool has_pressed(int vkey)
{
    return (GetAsyncKeyState(vkey) & 0x8000) ? 1 : 0;
}

inline void set_window_status(const char *title)
{
    HWND hWnd = GetConsoleWindow();
    RECT rc;
    GetWindowRect(hWnd, &rc);
    SetWindowLongPtr(hWnd,
                     GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX);
    SetWindowTextA(hWnd, title);
}

#endif
