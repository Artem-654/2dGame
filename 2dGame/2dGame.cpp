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
    char buffer[256]{""};
    short size1, size2;
    printw("Cols =");
    getstr(buffer);
    if (buffer[0] != '\0')
    {
        size1 = stoi(buffer);
    }
    else
    {
        size1 = 0;
    }
    printw("\nLines =");
    getstr(buffer);
    if (buffer[0] != '\0')
    {
        size2 = stoi(buffer);
    }
    else
    {
        size2 = 0;
    }
    clear();
    GAME game(size1,size2);
    Player player;
    player.Spawn(game);
    noecho();
    nodelay(stdscr, TRUE);
    game.Update(game);
    game.ShowScreen();
    //int key;
    int count = 0;
    do
    {
        //SetWindowText(hwnd, L"2dGame");
        //key = getch();
        //player.Move(getch(), game);
        if (player.Move(getch(), game) == 2)
        {
            game.addEntitie(game);
            count++;
            mvprintw(game.GetYSize()+1,1 , "%d", count);
        }
        game.Update(game);
        game.ShowMap();
    } while (true);

    endwin();//end
    return 0;
}