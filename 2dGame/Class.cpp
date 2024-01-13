#include <iostream>
#include <curses.h>
#include <string>
#include <vector>
#include <Windows.h>
#include "Class.h"
using namespace std;

GAME::GAME(short size1,short size2)
{
    GLOBAL_SIZEY = size2;
    GLOBAL_SIZEX = size1;
    LOCAL_MAP.resize(GLOBAL_SIZEY);
    for (int i = 0; i < GLOBAL_SIZEY; ++i) {
        LOCAL_MAP[i].resize(GLOBAL_SIZEX);
        for (int j = 0; j < GLOBAL_SIZEX; ++j) {
            LOCAL_MAP[i][j] = ". ";
        }
    }
    SCREEN.resize(GLOBAL_SIZEY);
    for (int i = 0; i < GLOBAL_SIZEY; ++i) {
        SCREEN[i].resize(GLOBAL_SIZEX);
    }
    ENTITIES_MAP.resize(GLOBAL_SIZEY);
    for (int i = 0; i < GLOBAL_SIZEY; ++i) {
        ENTITIES_MAP[i].resize(GLOBAL_SIZEX);
    }
}

GAME::~GAME()
{
    for (Mob* mob : MOBS)
        delete mob;
}

void GAME::ShowScreen()
{
    for (int i = 0; i < GLOBAL_SIZEX + 2; i++)
    {
        printw("**");
    }
    printw("\n");
    for (int i = 0; i < GLOBAL_SIZEY; i++)
    {
        printw("**");
        for (int j = 0; j < GLOBAL_SIZEX; j++)
        {
            printw("%s",SCREEN[i][j].c_str());
        }
        printw("**\n");
    }
    for (int i = 0; i < GLOBAL_SIZEX + 2; i++)
        printw("**");
    
    refresh();
}

void GAME::ShowMap()
{
    move(1,2);
    for (int i = 0; i < GLOBAL_SIZEY; i++)
    {
        move(i + 1,2);
        for (int j = 0; j < GLOBAL_SIZEX; j++)
        {
            printw("%s", SCREEN[i][j].c_str());
        }
        printw("**\n");
    }
    refresh();
}

void GAME::Update(GAME&game)
{
    for (Mob* mob : MOBS)
        mob->Move(game);
    for (int i = 0; i < GLOBAL_SIZEY; i++)
    {
        for (int j = 0; j < GLOBAL_SIZEX; j++)
        {
            SCREEN[i][j] = LOCAL_MAP[i][j];
        }
    }
    for (int i = 0; i < GLOBAL_SIZEY; i++)
    {
        for (int j = 0; j < GLOBAL_SIZEX; j++)
        {
            if (ENTITIES_MAP[i][j].empty())
                continue;
            SCREEN[i][j] = ENTITIES_MAP[i][j];
        }
    }
}

int GAME::GetXSize() const
{
    return GLOBAL_SIZEX;
}
int GAME::GetYSize() const
{
    return GLOBAL_SIZEY;
}

string GAME::GetEntities(int Y, int X)
{
    return ENTITIES_MAP[Y][X];
}

void GAME::SetEntities(int posY, int posX, string model)
{
    ENTITIES_MAP[posY][posX] = model;
}

void GAME::MoveXEntities(int posY, int posX, int oldposX)
{
    swap(ENTITIES_MAP[posY][posX], ENTITIES_MAP[posY][oldposX]);
}
void GAME::MoveYEntities(int posY, int posX, int oldposY)
{
    swap(ENTITIES_MAP[posY][posX], ENTITIES_MAP[oldposY][posX]);
}

void GAME::addEntitie(GAME& game)
{
    MOBS.push_back(new Mob(game));
}

void Entities::Move(GAME &game)
{
    napms(10);
    switch (rand() % 4)
    {
    case 0:
        oldposX = posX;
        posX--;
        if (posX == -1) posX++;
        game.MoveXEntities(posY, posX, oldposX);
        break;
    case 1:
        oldposY = posY;
        posY--;
        if (posY == -1) posY++;
        game.MoveYEntities(posY, posX, oldposY);
        break;
    case 2:
        oldposX = posX;
        posX++;
        if (posX == game.GetXSize()) posX--;
        game.MoveXEntities(posY, posX, oldposX);
        break;
    case 3:
        oldposY = posY;
        posY++;
        if (posY == game.GetYSize()) posY--;
        game.MoveYEntities(posY, posX, oldposY);
        break;
    }
}

void Entities::Spawn(GAME &game)
{
    posY = (rand() % game.GetYSize());
    posX = (rand() % game.GetXSize());
    game.SetEntities(posY,posX,Entities_model);
    
}
int Player::Move(int key, GAME&game)
{
    switch (key)
    {
    case 119://w
        if (posY == 0)
            return 0;
        oldposY = posY;
        posY--;
        game.MoveYEntities(posY, posX, oldposY);
        return 1;
    case 97://a
        if (posX == 0)
            return 0;
        oldposX = posX;
        posX--;
        game.MoveXEntities(posY, posX, oldposX);
        return 1;
    case 115://s
        if (posY == game.GetYSize() - 1)
            return 0;
        oldposY = posY;
        posY++;
        game.MoveYEntities(posY, posX, oldposY);
        return 1;
    case 100://d
        if (posX == game.GetXSize() - 1)
            return 0;
        oldposX = posX;
        posX++;
        game.MoveXEntities(posY, posX, oldposX);
        return 1;
    case 102:
        return 2;
    }
    return 0;
}
Player::Player()
{
    Entities_model = "@ ";
}
int Mob::_id = 0;
Mob::Mob(GAME&game)
{
    id = _id;
    _id++;
    Entities_model = "g ";
    health = 50;
    damage = 10;
    this->Spawn(game);
}

void Mob::Spawn(GAME&game)
{
    posY = (rand() % game.GetYSize());
    posX = (rand() % game.GetXSize());
    game.SetEntities(posY, posX, Entities_model);
}
