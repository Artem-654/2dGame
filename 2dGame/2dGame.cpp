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
    char buffer[256]{""};
    short size1, size2;
    printw("Cols =");
    getstr(buffer);
    if (buffer[0] != '\0')
    {
        string a1(buffer);
        size1 = stoi(a1);
    }
    else
    {
        size1 = 0;
    }
    printw("\nLines =");
    getstr(buffer);
    if (buffer[0] != '\0')
    {
        string a2(buffer);
        size2 = stoi(a2);
    }
    else
    {
        size2 = 0;
    }
    clear();
    GAME game(size1,size2);
    Player player;
    player.Spawn(game);
    curs_set(0);//no cursor blinking
    noecho();
    nodelay(stdscr, TRUE);
    game.Update(game);
    game.ShowScreen();
    //int key;

    do
    {
        //SetWindowText(hwnd, L"2dGame");
        //key = getch();
        //player.Move(getch(), game);
        if (player.Move(getch(), game) == 2)
        {
            game.addEntitie(game);
        }
        game.Update(game);
        game.ShowMap();
    } while (true);

    endwin();//end
    return 0;
}