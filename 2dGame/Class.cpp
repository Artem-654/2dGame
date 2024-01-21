#include <iostream>
#include <curses.h>
#include <string>
#include <vector>
#include <Windows.h>
#include "Class.h"
using namespace std;
vector<vector<vector<vector<Map_block*>>>> GAME::MAP_BLOCKS;
vector<vector<Screen_cell*>> GAME::MAP_SCREEN;
int GAME::SCREEN_POSY;
int GAME::SCREEN_POSX;
int GAME::SCREEN_CHUNKPOSY;
int GAME::SCREEN_CHUNKPOSX;
bool GAME::UpChunkGenerated = false;
bool GAME::LeftChunkGenerated = false;
bool GAME::RightChunkGenerated = false;
bool GAME::DownChunkGenerated = false;
int Player::ScreenPosY, Player::ScreenPosX, Player::ScreenChunkPosY, Player::ScreenChunkPosX;
GAME::GAME()
{
    MAP_BLOCKS.resize(CHUNK_SIZEY);
    for (int i = 0; i < CHUNK_SIZEY; ++i)
    {
        MAP_BLOCKS[i].resize(CHUNK_SIZEX);
        //for (int j = 0; j < CHUNK_SIZEX; j++)
        //    MAP_BLOCKS[i][j] = nullptr;
    }
    MAP_SCREEN.resize(SCREEN_SIZEY);
    for (int i = 0; i < SCREEN_SIZEY; ++i)
    {
        MAP_SCREEN[i].resize(SCREEN_SIZEX);
        for (int j = 0; j < SCREEN_SIZEX; j++)
             MAP_SCREEN[i][j] = new Screen_cell();
    }
    //init color (index, forground, background)

    init_pair(2, 82, 136); // grass

    init_pair(3, 248, 136); // wall

    init_pair(4, 14, 136); // player

    init_pair(5, 46, 136); // mob

    init_pair(1, 0, 39);//39
    bkgd(COLOR_PAIR(1));

    int chunkplayerY = rand()% CHUNK_SIZEY, chunkplayerX = rand() % CHUNK_SIZEX, playerY = rand() % GLOBAL_SIZEY, playerX = rand() % GLOBAL_SIZEX;
    //chunkplayerY = 1, chunkplayerX = 1, playerY = 199, playerX = 199;
    for (int i = -1;i<2;i++)
        for (int j = -1 ;j<2;j++)
        {
            if (((chunkplayerY + i) == (CHUNK_SIZEY)) || ((chunkplayerY + i) == -1) || ((chunkplayerX + j) == -1) || ((chunkplayerX + j) == CHUNK_SIZEX))
            {
                continue;
            }
            MAP_BLOCKS[chunkplayerY + i][chunkplayerX + j].resize(GLOBAL_SIZEY);
            for (int k = 0; k < GLOBAL_SIZEY; k++)
                MAP_BLOCKS[chunkplayerY + i][chunkplayerX + j][k].resize(GLOBAL_SIZEX);
        }
    MAP_BLOCKS[chunkplayerY][chunkplayerX][playerY][playerX] = new Map_block(chunkplayerY, chunkplayerX,playerY, playerX);

    MAP_BLOCKS[chunkplayerY][chunkplayerX][playerY][playerX]->Set_Entitie(new Player(chunkplayerY, chunkplayerX,playerY, playerX));
    SetFromplayerScreenPos();
}

GAME::~GAME()
{
    for (int iy = 0; iy < GLOBAL_SIZEY; iy++)
        for (int jx = 0; jx < GLOBAL_SIZEX; jx++)
            for (int i = 0; i < GLOBAL_SIZEY; i++)
                for (int j = 0; j < GLOBAL_SIZEX; j++)
                    delete MAP_BLOCKS[iy][jx][i][j];
    for (int i = 0; i < SCREEN_SIZEY; i++)
        for (int j = 0; j < SCREEN_SIZEX; j++)
            delete MAP_SCREEN[i][j];
}

void GAME::Update()
{   
    int chunkindexY,chunkindexX,indexY, indexX;
    for (int i = 0, indexi = SCREEN_POSY - (RENDER_SIZE / 2); i < RENDER_SIZE; indexi++, i++)
    {
        for (int j = 0, indexj = SCREEN_POSX - (RENDER_SIZE / 2); j < RENDER_SIZE; indexj++, j++)
        {
            chunkindexY = 0, chunkindexX = 0, indexY = 0, indexX = 0;
            if (indexi < 0)
            {
                if (!SCREEN_CHUNKPOSY)
                    continue;
                if (!UpChunkGenerated)
                {
                    if (SCREEN_CHUNKPOSX != 0)
                    {
                        GenerateChunk(SCREEN_CHUNKPOSY - 1, SCREEN_CHUNKPOSX - 1);
                    }
                    if (SCREEN_CHUNKPOSX != (CHUNK_SIZEX - 1))
                    {
                        GenerateChunk(SCREEN_CHUNKPOSY - 1, SCREEN_CHUNKPOSX + 1);
                    }
                    GenerateChunk(SCREEN_CHUNKPOSY - 1, SCREEN_CHUNKPOSX);
                    UpChunkGenerated = true;
                }
                chunkindexY = -1, indexY = GLOBAL_SIZEY;
            }
            if (indexj < 0)
            {
                if (!SCREEN_CHUNKPOSX)
                    continue;
                if (!LeftChunkGenerated)
                {
                    if (SCREEN_CHUNKPOSY != 0)
                    {
                        GenerateChunk(SCREEN_CHUNKPOSY - 1, SCREEN_CHUNKPOSX - 1);
                    }
                    if (SCREEN_CHUNKPOSY != (CHUNK_SIZEX - 1))
                    {
                        GenerateChunk(SCREEN_CHUNKPOSY + 1, SCREEN_CHUNKPOSX - 1);
                    }
                    GenerateChunk(SCREEN_CHUNKPOSY, SCREEN_CHUNKPOSX - 1);
                    LeftChunkGenerated = true;
                }
                chunkindexX = -1, indexX = GLOBAL_SIZEX;
            }
            if (indexi >= GLOBAL_SIZEY)
            {
                if (SCREEN_CHUNKPOSY == CHUNK_SIZEY)
                    continue;
                if (!DownChunkGenerated)
                {
                    if (SCREEN_CHUNKPOSX != 0)
                    {
                        GenerateChunk(SCREEN_CHUNKPOSY + 1, SCREEN_CHUNKPOSX - 1);
                    }
                    if (SCREEN_CHUNKPOSX != (CHUNK_SIZEX - 1))
                    {
                        GenerateChunk(SCREEN_CHUNKPOSY + 1, SCREEN_CHUNKPOSX + 1);
                    }
                    GenerateChunk(SCREEN_CHUNKPOSY + 1, SCREEN_CHUNKPOSX);
                    DownChunkGenerated = true;
                }
                chunkindexY = +1,  indexY = -GLOBAL_SIZEY;
            }
            if (indexj >= GLOBAL_SIZEX)
            {
                if (SCREEN_CHUNKPOSX == CHUNK_SIZEX)
                    continue;
                if (!RightChunkGenerated)
                {
                    if (SCREEN_CHUNKPOSY != 0)
                    {
                        GenerateChunk(SCREEN_CHUNKPOSY - 1, SCREEN_CHUNKPOSX + 1);
                    }
                    if (SCREEN_CHUNKPOSY != (CHUNK_SIZEX - 1))
                    {
                        GenerateChunk(SCREEN_CHUNKPOSY + 1, SCREEN_CHUNKPOSX + 1);
                    }
                    GenerateChunk(SCREEN_CHUNKPOSY, SCREEN_CHUNKPOSX + 1);
                    RightChunkGenerated = true;
                }
                 chunkindexX = +1, indexX = -GLOBAL_SIZEX;
            }
            if (((indexi + indexY) < 0) || ((indexj + indexX) < 0) || ((indexi + indexY) < 0) >= GLOBAL_SIZEY || ((indexj + indexX) < 0) >= GLOBAL_SIZEX)
                continue;
            if (MAP_BLOCKS[SCREEN_CHUNKPOSY + chunkindexY][SCREEN_CHUNKPOSX + chunkindexX][indexi + indexY][indexj + indexX] == nullptr)
            {
                if ((rand() % 100) == 1)
                {
                    MAP_BLOCKS[SCREEN_CHUNKPOSY + chunkindexY][SCREEN_CHUNKPOSX + chunkindexX][indexi + indexY][indexj + indexX] = new StoneWall(SCREEN_CHUNKPOSY + chunkindexY,SCREEN_CHUNKPOSX + chunkindexX,indexi + indexY,indexj + indexX);
                }
                else
                {
                    MAP_BLOCKS[SCREEN_CHUNKPOSY + chunkindexY][SCREEN_CHUNKPOSX + chunkindexX][indexi + indexY][indexj + indexX] = new Map_block(SCREEN_CHUNKPOSY + chunkindexY, SCREEN_CHUNKPOSX + chunkindexX, indexi + indexY, indexj + indexX);
                    if(rand()% 100 == 1)
                       SetEntitie(SCREEN_CHUNKPOSY + chunkindexY, SCREEN_CHUNKPOSX + chunkindexX, indexi + indexY, indexj + indexX, new Mob(SCREEN_CHUNKPOSY + chunkindexY, SCREEN_CHUNKPOSX + chunkindexX, indexi + indexY, indexj + indexX));
                }
            }
            MAP_BLOCKS[SCREEN_CHUNKPOSY + chunkindexY][SCREEN_CHUNKPOSX + chunkindexX][indexi + indexY][indexj + indexX]->Update();
        }
    }
}

void GAME::ShowScreen()
{
    move(0,0);

    int chunkindexY, chunkindexX, indexY, indexX;
    for (int i = 0, indexi = SCREEN_POSY - (SCREEN_SIZEY / 2); i < SCREEN_SIZEY; indexi++, i++)
    {
        for (int j = 0, indexj = SCREEN_POSX - (SCREEN_SIZEX / 2); j < SCREEN_SIZEX; indexj++, j++)
        {
            chunkindexY = 0, chunkindexX = 0, indexY = 0, indexX = 0;
            if (indexi < 0)
            {
                if (!SCREEN_CHUNKPOSY)
                {
                    MAP_SCREEN[i][j]->Set_str("  ");
                    MAP_SCREEN[i][j]->Set_color(0);
                    continue;
                }

                chunkindexY = -1, indexY = GLOBAL_SIZEY;
            }
            if (indexj < 0)
            {
                if (!SCREEN_CHUNKPOSX)
                {
                    MAP_SCREEN[i][j]->Set_str("  ");
                    MAP_SCREEN[i][j]->Set_color(0);
                    continue;
                }
                chunkindexX = -1, indexX = GLOBAL_SIZEX;
            }
            if (indexi >= GLOBAL_SIZEY)
            {
                if (SCREEN_CHUNKPOSY == CHUNK_SIZEY)
                {
                    MAP_SCREEN[i][j]->Set_str("  ");
                    MAP_SCREEN[i][j]->Set_color(0);
                    continue;
                }
                chunkindexY = +1, indexY = -GLOBAL_SIZEY;
            }
            if (indexj >= GLOBAL_SIZEX)
            {
                if (SCREEN_CHUNKPOSX == CHUNK_SIZEX)
                {
                    MAP_SCREEN[i][j]->Set_str("  ");
                    MAP_SCREEN[i][j]->Set_color(0);
                    continue;
                }
                chunkindexX = +1, indexX = -GLOBAL_SIZEX;
            }
            MAP_SCREEN[i][j]->Set_str(MAP_BLOCKS[SCREEN_CHUNKPOSY + chunkindexY][SCREEN_CHUNKPOSX + chunkindexX][indexi + indexY][indexj + indexX]->Get_model());
            MAP_SCREEN[i][j]->Set_color(MAP_BLOCKS[SCREEN_CHUNKPOSY + chunkindexY][SCREEN_CHUNKPOSX + chunkindexX][indexi + indexY][indexj + indexX]->GetColorpair());
        }
    }

    for (int i = 0; i < SCREEN_SIZEY; i++)
    {
        for (int j = 0; j < SCREEN_SIZEX; j++)
        {
            attron(COLOR_PAIR(MAP_SCREEN[i][j]->Get_color()));
            printw("%s", MAP_SCREEN[i][j]->Get_str().c_str());
            attroff(COLOR_PAIR(MAP_SCREEN[i][j]->Get_color()));
        }
        printw("\n");
    }
    mvprintw(3, SCREEN_SIZEX * 2, "| Y position = %d, chunk position = %d", SCREEN_POSY, SCREEN_CHUNKPOSY);
    mvprintw(5, SCREEN_SIZEX * 2, "| X position = %d, chunk position = %d", SCREEN_POSX, SCREEN_CHUNKPOSX);
    mvprintw(7, SCREEN_SIZEX * 2, "| SCREEN Up = %d, Down = %d", UpChunkGenerated, DownChunkGenerated);
    mvprintw(9, SCREEN_SIZEX * 2, "| SCREEN Left = %d, Right = %d", LeftChunkGenerated, RightChunkGenerated);
    refresh();
}


void GAME::SetEntitie(int chunkY, int chunkX, int Y, int X, Entitie* entitie)
{
    MAP_BLOCKS[chunkY][chunkX][Y][X]->Set_Entitie(entitie);
    MAP_BLOCKS[chunkY][chunkX][Y][X]->SetposEntitie(chunkY, chunkX, Y, X);
}

void GAME::SetSCREENposY(int Y)
{
    SCREEN_POSY = Y;
}

void GAME::SetSCREENposX(int X)
{
    SCREEN_POSX = X;
}
void GAME::SetSCREENchunkposY(int chunkposY)
{
    if (SCREEN_CHUNKPOSY != chunkposY)
    {
        int tempChunkpos = SCREEN_CHUNKPOSY;
        SCREEN_CHUNKPOSY = chunkposY;
        if (tempChunkpos < chunkposY)
        {
            DownChunkGenerated = false;
            return;
        }
        if (tempChunkpos > chunkposY)
            UpChunkGenerated = false;
    }
}
void GAME::SetSCREENchunkposX(int chunkposX)
{
    if (SCREEN_CHUNKPOSX != chunkposX)
    {
        int tempChunkpos = SCREEN_CHUNKPOSX;
        SCREEN_CHUNKPOSX = chunkposX;
        if (tempChunkpos < chunkposX)
        {
            RightChunkGenerated = false;
            return;
        }
        if (tempChunkpos > chunkposX)
        {
            LeftChunkGenerated = false;
        }
    }
}
void GAME::SetFromplayerScreenPos()
{
    GAME::SetSCREENposY(Player::GetPlrScrPosY());
    GAME::SetSCREENposX(Player::GetPlrScrPosX());
    GAME::SetSCREENchunkposY(Player::GetPlrScrChunkPosY());
    GAME::SetSCREENchunkposX(Player::GetPlrScrChunkPosX());
}
void GAME::GenerateChunk(int ChunkY, int ChunkX)
{
    MAP_BLOCKS[ChunkY][ChunkX].resize(GLOBAL_SIZEY);
    for (int i = 0; i < GLOBAL_SIZEY; i++)
        MAP_BLOCKS[ChunkY][ChunkX][i].resize(GLOBAL_SIZEX);
}
//int GAME::GetSCREENPosY()
//{
//    return SCREEN_POSY;
//}
//int GAME::GetSCREENPosX() 
//{
//    return SCREEN_POSX;
//}
//int GAME::GetSCREENChunkPosY()
//{
//    return SCREEN_CHUNKPOSY;
//}
//int GAME::GetSCREENChunkPosX()
//{
//    return SCREEN_CHUNKPOSX;
//}
//int GAME::GetblockposY(int chunkY, int chunkX, int Y, int X)
//{
//    return MAP_BLOCKS[chunkY][chunkX][Y][X]->Get_posY();
//}
//
//int GAME::GetblockposX(int chunkY, int chunkX, int Y, int X)
//{
//    return MAP_BLOCKS[chunkY][chunkX][Y][X]->Get_posX();
//}

int GAME::GetGlobal_sizeY()
{
    return GLOBAL_SIZEY;
}
int GAME::GetGlobal_sizeX()
{
    return GLOBAL_SIZEX;
}

int GAME::PosibleMove(int result, int chunkY, int chunkX, int Y,int X)
{
    switch (result)
    {
    case 1: //w -Y
        if (Y == 0)
        {
            if (chunkY == 0)
                break;
            chunkY--;
            Y = GLOBAL_SIZEY;
        }
        if (MAP_BLOCKS[chunkY][chunkX][Y - 1][X] == nullptr)
            break;
        if (!MAP_BLOCKS[chunkY][chunkX][Y - 1][X]->IsEmpty())
            break;
        if (!MAP_BLOCKS[chunkY][chunkX][Y - 1][X]->Get_CanWalkThêough())
            break;
        return 1;
    case 2: //a -X
        if (X == 0)
        {
            if (chunkX == 0)
                break;
            chunkX--;
            X = GLOBAL_SIZEX;
        }
        if (MAP_BLOCKS[chunkY][chunkX][Y][X - 1] == nullptr)
            break;
        if (!MAP_BLOCKS[chunkY][chunkX][Y][X - 1]->IsEmpty())
            break;
        if (!MAP_BLOCKS[chunkY][chunkX][Y][X - 1]->Get_CanWalkThêough())
            break;
        return 2;
    case 3: //s +Y
        if (Y == (GLOBAL_SIZEY - 1))
        {
            if (chunkY == CHUNK_SIZEY)
                break;
            chunkY++;
            Y = -1;
        }
        if (MAP_BLOCKS[chunkY][chunkX][Y + 1][X] == nullptr)
            break;
        if (!MAP_BLOCKS[chunkY][chunkX][Y + 1][X]->IsEmpty())
            break;
        if (!MAP_BLOCKS[chunkY][chunkX][Y + 1][X]->Get_CanWalkThêough())
            break;
        return 3;
    case 4: //d +X
        if (X == (GLOBAL_SIZEX - 1))
        {
            if (chunkX == CHUNK_SIZEX)
                break;
            chunkX++;
            X = -1;
        }
        if (MAP_BLOCKS[chunkY][chunkX][Y][X + 1] == nullptr)
            break;
        if (!MAP_BLOCKS[chunkY][chunkX][Y][X + 1]->IsEmpty())
            break;
        if (!MAP_BLOCKS[chunkY][chunkX][Y][X + 1]->Get_CanWalkThêough())
            break;
        return 4;
    }
    return 0;
}

Screen_cell::Screen_cell()
{
    str = "  ";
    color = 0;
}

string Screen_cell::Get_str()
{
    return str;
}
int Screen_cell::Get_color()
{
    return color;
}
void Screen_cell::Set_str(string str)
{
    this->str = str;
}
void Screen_cell::Set_color(int color)
{
    this->color = color;
}
Map_block::Map_block(int chunkY, int chunkX, int Y,int X)
{
    posY = Y;
    posX = X;
    chunkposY = chunkY;
    chunkposX = chunkX;
    colorpair = 2;
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
        Map_block::Move(Entitie_ptr->Move());
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
int Map_block::Move(int result)
{
    int Y = 1, X = 1, CHY = 0, CHX = 0;
    switch (result)
    {
    case 1://w
        if (posY == 0)
        {
            CHY = -1;
            Y = -(GAME::GetGlobal_sizeY()-1);
        }
        GAME::SetEntitie(chunkposY + CHY, chunkposX, posY - Y, posX, Entitie_ptr);
        Map_block::Set_Entitie(nullptr);
        return 1;
    case 2://a
        if (posX == 0)
        {
            CHX = -1;
            X = -(GAME::GetGlobal_sizeX() - 1);
        }
        GAME::SetEntitie(chunkposY, chunkposX + CHX, posY, posX - X, Entitie_ptr);
        Map_block::Set_Entitie(nullptr);
        return 1;
    case 3://s
        if (posY == (GAME::GetGlobal_sizeY()-1))
        {
            CHY = 1;
            Y = -(GAME::GetGlobal_sizeY() - 1);
        }
        GAME::SetEntitie(chunkposY + CHY, chunkposX, posY + Y, posX, Entitie_ptr);
        Map_block::Set_Entitie(nullptr);
        return 1;
    case 4://d
        if (posX == (GAME::GetGlobal_sizeX()-1))
        {
            CHX = 1;
            X = -(GAME::GetGlobal_sizeX() - 1);
        }
        GAME::SetEntitie(chunkposY, chunkposX + CHX, posY, posX + X, Entitie_ptr);
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

void Map_block::SetposEntitie(int chunkY, int chunkX,int Y, int X)
{
    Entitie_ptr->SetchunkposY(chunkY);
    Entitie_ptr->SetchunkposX(chunkX);
    Entitie_ptr->SetposY(Y);
    Entitie_ptr->SetposX(X);
}

StoneWall::StoneWall(int chunkY, int chunkX, int Y, int X) : Map_block( chunkY, chunkX, Y, X)
{
    block_model = "##";
    entitie_model = "##";
    CanWalkThêough = false;
    colorpair = 3;
}
Entitie::Entitie(int chunkY, int chunkX, int Y, int X)
{
    color = 0;
    chunkposY = chunkY;
    chunkposX = chunkX;
    posY = Y;
    posX = X;
}

int Entitie::Move()
{
    if (rand()%100 == 1)
    {
        int temp = GAME::PosibleMove((rand() % 4) + 1, chunkposY, chunkposX, posY, posX);
        if (temp)
        {
            if ((temp == 1) && TurnedRight)
            {
                swap(Entities_model[0], Entities_model[1]);
                TurnedRight = false;
            }
            if ((temp == 3) && !TurnedRight)
            {
                swap(Entities_model[0], Entities_model[1]);
                TurnedRight = true;
            }
            return temp;
        }
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
int Entitie::GetposY()
{
    return posY;
}
int Entitie::GetposX()
{
    return posX;
}
void Entitie::SetchunkposY(int Y)
{
    chunkposY = Y;
}
void Entitie::SetchunkposX(int X)
{
    chunkposX = X;
}
int Entitie::GetchunkposY()
{
    return chunkposY;
}
int Entitie::GetchunkposX()
{
    return chunkposX;
}
int Entitie::GetColorPair()
{
    return color;
}
Player::Player(int chunkY, int chunkX, int Y,int X) : Entitie( chunkY,  chunkX, Y, X)
{
    color = 4;
    Entities_model = "@ ";
    ScreenPosY = Y;
    ScreenPosX = X;
    ScreenChunkPosY = chunkY;
    ScreenChunkPosX = chunkX;
}
void Player::Update()
{

}
int Player::Move()
{
    
    switch (getch())
    {
    case 119://w
        if (!GAME::PosibleMove(1, chunkposY, chunkposX, posY, posX))
            break;
        if (posY == 0)
        {
            ScreenPosY = GAME::GetGlobal_sizeY() - 1;
            ScreenChunkPosY = chunkposY - 1;
        }
        else
        {
            ScreenPosY = posY - 1;
            ScreenChunkPosY = chunkposY;
        }
        return 1;
    case 97://a
        if (!GAME::PosibleMove(2, chunkposY, chunkposX, posY, posX))
            break;
        if (posX == 0)
        {
            ScreenPosX = GAME::GetGlobal_sizeX() - 1;
            ScreenChunkPosX = chunkposX - 1;
        }
        else
        {
            ScreenPosX = posX - 1;
            ScreenChunkPosX = chunkposX;
        }
        if (TurnedRight)
        {
            swap(Entities_model[0], Entities_model[1]);
            TurnedRight = false;
        }
        return 2;
    case 115://s
        if (!GAME::PosibleMove(3, chunkposY, chunkposX, posY, posX))
            break;
        if (posY == (GAME::GetGlobal_sizeY() - 1))
        {
            ScreenPosY = 0;
            ScreenChunkPosY = chunkposY + 1;
        }
        else
        {
            ScreenPosY = posY + 1;
            ScreenChunkPosY = chunkposY;
        }
        return 3;
    case 100://d
        if (!GAME::PosibleMove(4, chunkposY, chunkposX, posY, posX))
            break;
        if (posX == (GAME::GetGlobal_sizeX() - 1))
        {
            ScreenPosX = 0;
            ScreenChunkPosX = chunkposX + 1;
        }
        else
        {
            ScreenPosX = posX + 1;
            ScreenChunkPosX = chunkposX;
        }
        if (!TurnedRight)
        {
            swap(Entities_model[0], Entities_model[1]);
            TurnedRight = true;
        }
        return 4;
    }
    return 0;
}
int Player::GetPlrScrPosY()
{
    return ScreenPosY;
}
int Player::GetPlrScrPosX()
{
    return ScreenPosX;
}
int Player::GetPlrScrChunkPosY()
{
    return ScreenChunkPosY;
}
int Player::GetPlrScrChunkPosX()
{
    return ScreenChunkPosX;
}
Mob::Mob(int chunkY, int chunkX, int Y, int X) : Entitie( chunkY,  chunkX, Y,X)
{
    color = 5;
    Entities_model = "g ";
}