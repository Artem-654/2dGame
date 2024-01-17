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
    initscr();//start
    curs_set(0);//no cursor blinking
    noecho();
    nodelay(stdscr, TRUE);
    GAME game;
    do
    {
        game.Update();
        game.ShowScreen();
    } while (true);

    endwin();//end
    return 0;
}
    //do
    //{
    //    printw("%d\n",getch());
    //} while (true);