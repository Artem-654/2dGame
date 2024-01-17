#include <iostream>
#include <curses.h>
#include <string>
#include <vector>
#include <Windows.h>
#include "Class.h"
using namespace std;
vector<vector<Map_block*>> GAME::MAP_BLOCKS;
int GAME::SCREEN_POSY = 0;
int GAME::SCREEN_POSX = 0;
GAME::GAME()
{
    MAP_BLOCKS.resize(GLOBAL_SIZEY);
    for (int i = 0; i < GLOBAL_SIZEY; ++i)
        MAP_BLOCKS[i].resize(GLOBAL_SIZEX);
    /*SCREEN.resize(SCREEN_SIZEY);
    for (int i = 0; i < SCREEN_SIZEY; ++i)
        SCREEN[i].resize(SCREEN_SIZEX);*/
    
    for (int i = 0; i < GLOBAL_SIZEY; i++)
        for (int j = 0; j < GLOBAL_SIZEX; j++)
            MAP_BLOCKS[i][j] = nullptr;

    for (int i = 0; i < GLOBAL_SIZEY; i++)
        for (int j = 0; j < GLOBAL_SIZEX; j++)
            MAP_BLOCKS[i][j] = new Map_block(i,j);
    int playerY = rand() % GLOBAL_SIZEY, playerX = rand() % GLOBAL_SIZEX;
    playerY = 0, playerX = 0;
    MAP_BLOCKS[playerY][playerX]->Set_Entitie(new Player(playerY, playerX));
}

GAME::~GAME()
{
    for (int i = 0; i < GLOBAL_SIZEY; i++)
        for (int j = 0; j < GLOBAL_SIZEX; j++)
            delete MAP_BLOCKS[i][j];
}

void GAME::ShowScreen()
{
    move(0,0);
    for(int i = 0,indexi = (- (SCREEN_SIZEY / 2)) + SCREEN_POSY; i< SCREEN_SIZEY; indexi++,i++)
    {
        for (int j = 0, indexj = (-(SCREEN_SIZEY / 2)) + SCREEN_POSX; j < SCREEN_SIZEX; indexj++,j++)
        {
            if (indexi < 0 || indexj < 0 || indexi >= GLOBAL_SIZEY || indexj >= GLOBAL_SIZEX)
            {
                printw("  ");
                continue;
            }
            printw("%s", MAP_BLOCKS[indexi][indexj]->Get_model().c_str());
        }
        printw("\n");
    }
    refresh();
}

void GAME::Update()
{
    for (int i = 0; i < GLOBAL_SIZEY; i++)
        for (int j = 0; j < GLOBAL_SIZEX; j++)
            MAP_BLOCKS[i][j]->Update();
}

void GAME::SetEntitie(int Y, int X, Entitie* entitie)
{
    MAP_BLOCKS[Y][X]->Set_Entitie(entitie);
}

void GAME::SetposEntitie(int posY, int posX, int Y, int X)
{
    MAP_BLOCKS[posY][posX]->SetposEntitie(Y, X);
}

void GAME::SetSCREENposY(int Y)
{
    SCREEN_POSY = Y;
}

void GAME::SetSCREENposX(int X)
{
    SCREEN_POSX = X;
}

int GAME::GetXSize() 
{
    return SCREEN_SIZEX;
}
int GAME::GetYSize() 
{
    return SCREEN_SIZEY;
}

int GAME::GetposY(int Y, int X)
{
    return MAP_BLOCKS[Y][X]->Get_posY();
}

int GAME::GetposX(int Y, int X)
{
    return MAP_BLOCKS[Y][X]->Get_posX();
}


int GAME::PosibleMove(int result,int Y,int X)
{
    switch (result)
    {
    case 1: //w -Y
        if (Y == 0)
            break;
        if (!MAP_BLOCKS[Y - 1][X]->IsEmpty())
            break;
        return 1;
    case 2: //a -X
        if (X == 0)
            break;
        if (!MAP_BLOCKS[Y][X - 1]->IsEmpty())
            break;
        return 2;
    case 3: //s +Y
        if (Y == GLOBAL_SIZEY - 1)
            break;
        if (!MAP_BLOCKS[Y + 1][X]->IsEmpty())
            break;
        return 3;
    case 4: //d +X
        if (X == GLOBAL_SIZEX - 1)
            break;
        if (!MAP_BLOCKS[Y][X + 1]->IsEmpty())
            break;
        return 4;
    //case 5: //f
    }
    return 0;
}

Map_block::Map_block(int Y,int X)
{
    posY = Y;
    posX = X;
}

Map_block::~Map_block()
{
    delete Entitie_ptr;
}
int Map_block::Get_posY() 
{
    return posY;
}

int Map_block::Get_posX() 
{
    return posX;
}

string Map_block::Get_model() const
{
    if (entitie_model.empty())
        return block_model;
    else
        return entitie_model;
}

void Map_block::Update()
{
    if (Entitie_ptr != nullptr)
    {
        if (entitie_model.empty())
            entitie_model = Entitie_ptr->GetModel();
        Entitie_ptr->Update();
        Map_block::Move(GAME::PosibleMove(Entitie_ptr->Move(), Map_block::Get_posY(), Map_block::Get_posX()));
    }
    else
    {
        if (!entitie_model.empty())
            entitie_model.erase();
    }
}
void Map_block::UpdateEntitie_model()
{
    if (Entitie_ptr != nullptr)
        entitie_model = Entitie_ptr->GetModel();
}
bool Map_block::IsEmpty()
{
    return Entitie_ptr == nullptr;
}
int Map_block::Move(int move)
{
    switch (move)
    {
    case 1://w
        GAME::SetposEntitie(posY, posX, -1, 0);
        GAME::SetEntitie(posY - 1, posX, Entitie_ptr);
        Map_block::Set_Entitie(nullptr);
        return 1;
    case 2://a
        GAME::SetposEntitie(posY, posX, 0, -1);
        GAME::SetEntitie(posY, posX - 1, Entitie_ptr);
        Map_block::Set_Entitie(nullptr);
        return 1;
    case 3://s
        GAME::SetposEntitie(posY, posX, 1, 0);
        GAME::SetEntitie(posY + 1, posX, Entitie_ptr);
        Map_block::Set_Entitie(nullptr);
        return 1;
    case 4://d
        GAME::SetposEntitie(posY, posX, 0, 1);
        GAME::SetEntitie(posY, posX + 1, Entitie_ptr);
        Map_block::Set_Entitie(nullptr);
        return 1;
    }
    return 0;
}
Entitie* Map_block::Get_Entitie() 
{
    return Entitie_ptr;
}
void Map_block::Set_Entitie(Entitie* entitie)
{
    Entitie_ptr = entitie;
}

void Map_block::SetposEntitie(int Y, int X)
{
    Entitie_ptr->addpos(Y,X);
}

Entitie::Entitie()
{

}

Entitie::~Entitie()
{

}

int Entitie::Move()
{
    return 0;
}
void Entitie::Update()
{

}
string Entitie::GetModel()
{
    return Entities_model;
}
void Entitie::SetposY(int Y)
{
    posY = Y;
}
void Entitie::SetposX(int X)
{
    posX = X;
}
void Entitie::addpos(int Y, int X)
{
    posY += Y;
    posX += X;
}
int Entitie::GetposY()
{
    return posY;
}
int Entitie::GetposX()
{
    return posX;
}
Player::Player(int Y,int X)
{
    Entities_model = "@ ";
    posY = Y;
    posX = X;
    GAME::SetSCREENposY(posY);
    GAME::SetSCREENposX(posX);
}
void Player::Update()
{
    GAME::SetSCREENposY(posY);
    GAME::SetSCREENposX(posX);
}
int Player::Move()
{
    
    switch (getch())
    {
    case 119://w
        return 1;
    case 97://a
        return 2;
    case 115://s
        return 3;
    case 100://d
        return 4;
    case 102: //f - for spawn mobs
        return 5;
    }
    return 0;
}
//Mob::Mob()
//{
//    Entities_model = "g ";
//    //health = 50;
//    //damage = 10;
//}
//
//int Mob::Move(GAME& game)
//{
//    if ((rand() % 100) == 1)
//    {
//        switch (rand() % 4)
//        {
//        case 0:
//            oldposX = posX;
//            posX--;
//            if (posX == -1) posX++;
//            game.MoveXEntities(posY, posX, oldposX, Entities_model);
//            break;
//        case 1:
//            oldposY = posY;
//            posY--;
//            if (posY == -1) posY++;
//            game.MoveYEntities(posY, posX, oldposY, Entities_model);
//            break;
//        case 2:
//            oldposX = posX;
//            posX++;
//            if (posX == game.GetXSize()) posX--;
//            game.MoveXEntities(posY, posX, oldposX, Entities_model);
//            break;
//        case 3:
//            oldposY = posY;
//            posY++;
//            if (posY == game.GetYSize()) posY--;
//            game.MoveYEntities(posY, posX, oldposY, Entities_model);
//            break;
//        }
//    }
//}
