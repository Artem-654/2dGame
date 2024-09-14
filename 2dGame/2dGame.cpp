#include <curses.h>
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <chrono>
#include "Class.h"
using namespace std;
int main()
{
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_MAXIMIZE);
    srand(time(0));
    initscr();
    start_color();
    curs_set(0);
    noecho();
    nodelay(stdscr, TRUE);

    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();
    chrono::steady_clock::time_point frameStart = chrono::steady_clock::now();

    GAME game;
    do
    {
        game.update();
        game.setFromBufferToMap();
        game.setFromPlayerScreenPos();
        game.showScreen();
        game.countFPS(startTime, frameStart);
    } while (true);
    endwin();
    return 0;
}
    //do
    //{
    //    printw("%d\n",getch());
    //} while (true);