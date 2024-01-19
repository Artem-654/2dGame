#include <iostream>
#include <curses.h>
#include <string>
#include <vector>
#include <Windows.h>
#include "Class.h"
using namespace std;
vector<vector<Map_block*>> GAME::MAP_BLOCKS;
vector<vector<string>> GAME::MAP_SCREEN;
int GAME::SCREEN_POSY = 0;
int GAME::SCREEN_POSX = 0;
GAME::GAME()
{
    MAP_BLOCKS.resize(GLOBAL_SIZEY);
    for (int i = 0; i < GLOBAL_SIZEY; ++i)
    {
        MAP_BLOCKS[i].resize(GLOBAL_SIZEX);
        for (int j = 0; j < GLOBAL_SIZEX; j++)
            MAP_BLOCKS[i][j] = nullptr;
    }
    MAP_SCREEN.resize(SCREEN_SIZEY);
    for (int i = 0; i < SCREEN_SIZEY; ++i)
    {
        MAP_SCREEN[i].resize(SCREEN_SIZEX);
        for (int j = 0; j < SCREEN_SIZEX; j++)
            MAP_SCREEN[i][j] = "  ";
    }
    init_color(1, 0, 768, 0);
    init_pair(1, 1, COLOR_BLACK);
    init_color(2, 300, 300, 300);
    init_pair(2, 2, COLOR_BLACK);
    init_color(3, 768, 400, 400);
    init_pair(3, 3, COLOR_BLACK);
    init_color(4, 0, 368, 0);
    init_pair(4, 4, COLOR_BLACK);
    srand(time(0));
    int playerY = rand() % GLOBAL_SIZEY, playerX = rand() % GLOBAL_SIZEX;
    //playerY = 0, playerX = 0;
    MAP_BLOCKS[playerY][playerX] = new Map_block(playerY, playerX);
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
    for(int i = 0,indexi =SCREEN_POSY - (SCREEN_SIZEY / 2); i< SCREEN_SIZEY; indexi++,i++)
    {
        for (int j = 0, indexj =SCREEN_POSX - (SCREEN_SIZEY / 2); j < SCREEN_SIZEX; indexj++,j++)
        {
            if (indexi < 0 || indexj < 0 || indexi >= GLOBAL_SIZEY || indexj >= GLOBAL_SIZEX)
            {
                //printw("  ");
                MAP_SCREEN[i][j] = "  ";
                continue;
            }
            //if (MAP_SCREEN[i][j] == MAP_BLOCKS[indexi][indexj]->Get_model())
            //    continue;
            MAP_SCREEN[i][j] = MAP_BLOCKS[indexi][indexj]->Get_model();
            //printw("%s", MAP_SCREEN[i][j].c_str());
        }
        //printw("\n");
    }
    for (int i = 0,iy = SCREEN_POSY - (SCREEN_SIZEY / 2);i< SCREEN_SIZEY;i++,iy++)
    {
        for (int j = 0,ix = SCREEN_POSX - (SCREEN_SIZEX / 2); j < SCREEN_SIZEX; j++,ix++)
        {
            if (iy < 0 || ix < 0 || iy >= GLOBAL_SIZEY || ix >= GLOBAL_SIZEX)
            {
                printw("%s", MAP_SCREEN[i][j].c_str());
            }
            else
            {
                attron(COLOR_PAIR(MAP_BLOCKS[iy][ix]->GetColorpair()));
                printw("%s", MAP_SCREEN[i][j].c_str());
                attroff(COLOR_PAIR(MAP_BLOCKS[iy][ix]->GetColorpair()));
            }
        }
        printw("\n");
    }

    //for (int i = 0; i < SCREEN_SIZEY; i++)
    //{
    //    for (int j = 0; j < SCREEN_SIZEX; j++)
    //    {
    //       printw("%s", MAP_SCREEN[i][j].c_str());
    //    }
    //    printw("\n");
    //}
    refresh();
}

void GAME::Update()
{
    for (int i = 0, indexi = SCREEN_POSY - (RENDER_SIZE / 2); i < RENDER_SIZE; indexi++, i++)
    {
        for (int j = 0, indexj = SCREEN_POSX - (RENDER_SIZE / 2); j < RENDER_SIZE; indexj++, j++)
        {
            if (indexi < 0 || indexj < 0 || indexi >= GLOBAL_SIZEY || indexj >= GLOBAL_SIZEX)
            {
                continue;
            }
            if (MAP_BLOCKS[indexi][indexj] == nullptr)
            {
                if ((rand() % 10) == 1)
                {
                    MAP_BLOCKS[indexi][indexj] = new StoneWall(indexi, indexj);
                }
                else
                {
                    MAP_BLOCKS[indexi][indexj] = new Map_block(indexi, indexj);
                    if(rand()% 100 == 1)
                        SetEntitie(indexi, indexj, new Mob(indexi, indexj));
                }
            }
            MAP_BLOCKS[indexi][indexj]->Update();
        }
    }
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
        if (MAP_BLOCKS[Y - 1][X] == nullptr)
            break;
        if (!MAP_BLOCKS[Y - 1][X]->IsEmpty())
            break;
        if (!MAP_BLOCKS[Y - 1][X]->Get_CanWalkThêough())
            break;
        return 1;
    case 2: //a -X
        if (X == 0)
            break;
        if (MAP_BLOCKS[Y][X - 1] == nullptr)
            break;
        if (!MAP_BLOCKS[Y][X - 1]->IsEmpty())
            break;
        if (!MAP_BLOCKS[Y][X - 1]->Get_CanWalkThêough())
            break;
        return 2;
    case 3: //s +Y
        if (Y == GLOBAL_SIZEY - 1)
            break;
        if (MAP_BLOCKS[Y + 1][X] == nullptr)
            break;
        if (!MAP_BLOCKS[Y + 1][X]->IsEmpty())
            break;
        if (!MAP_BLOCKS[Y + 1][X]->Get_CanWalkThêough())
            break;
        return 3;
    case 4: //d +X
        if (X == GLOBAL_SIZEX - 1)
            break;
        if (MAP_BLOCKS[Y][X + 1] == nullptr)
            break;
        if (!MAP_BLOCKS[Y][X + 1]->IsEmpty())
            break;
        if (!MAP_BLOCKS[Y][X + 1]->Get_CanWalkThêough())
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
    colorpair = 1;
    block_model = ". ";
    entitie_model = ". ";
    CanWalkThêough = true;
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
   return entitie_model;
}

void Map_block::Update()
{
    if (Entitie_ptr != nullptr)
    {
        Entitie_ptr->Update();
        Map_block::Move(GAME::PosibleMove(Entitie_ptr->Move(), Map_block::Get_posY(), Map_block::Get_posX()));
    }
}
void Map_block::UpdateEntitie_model()
{
    if (Entitie_ptr != nullptr)
        entitie_model = Entitie_ptr->GetModel();
}
bool Map_block::IsEmpty()
{
    return this->Entitie_ptr == nullptr;
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
bool Map_block::Get_CanWalkThêough()
{
    return CanWalkThêough;
}
int Map_block::GetColorpair()
{
    if (Entitie_ptr == nullptr)
        return colorpair;
    else
        return Entitie_ptr->GetColorPair();
}
//Entitie* Map_block::Get_Entitie() 
//{
//    return Entitie_ptr;
//}
void Map_block::Set_Entitie(Entitie* entitie)
{
    Entitie_ptr = entitie;

    if (Entitie_ptr != nullptr) {
        entitie_model = Entitie_ptr->GetModel();
    }
    else {
        entitie_model = block_model;
    }

}

void Map_block::SetposEntitie(int Y, int X)
{
    Entitie_ptr->addpos(Y,X);
}

Entitie::Entitie(int Y, int X)
{
    posY = Y;
    posX = X;
}

Entitie::~Entitie()
{

}

int Entitie::Move()
{
    if (rand()%100 == 1)
    {
        return (rand() % 4) + 1;
    }
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
int Entitie::GetColorPair()
{
    return colorpair;
}
Player::Player(int Y,int X) : Entitie(Y, X)
{
    colorpair = 3;
    Entities_model = "@ ";
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
StoneWall::StoneWall(int Y, int X) : Map_block(Y, X)
{
    block_model = "##";
    entitie_model = "##";
    CanWalkThêough = false;
    colorpair = 2;
}
Mob::Mob(int Y, int X) : Entitie(Y,X)
{
    colorpair = 4;
    Entities_model = "g ";
}

