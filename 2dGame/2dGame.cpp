#include <curses.h>
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
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
    GAME game;
    do
    {
        game.Update();
        game.SetFromplayerScreenPos();
        game.ShowScreen();
    } while (true);
    endwin();
    return 0;
}
    //do
    //{
    //    printw("%d\n",getch());
    //} while (true);