#include <iostream>
#include <curses.h>
#include <string>
#include <vector>
#include <Windows.h>
#include "Class.h"
using namespace std;
vector<vector<vector<vector<Map_block*>>>> GAME::MAP_BLOCKS;
vector<vector<Entitie*>> GAME::MAP_BLOCKS_BUFFER;
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
    MAP_BLOCKS_BUFFER.resize(RENDER_SIZE+4);
    for (int i = 0; i < RENDER_SIZE+4; ++i)
    {
        MAP_BLOCKS_BUFFER[i].resize(RENDER_SIZE+4);
        for (int j = 0; j < RENDER_SIZE+4; j++)
            MAP_BLOCKS_BUFFER[i][j] = nullptr;
    }
    //init color (index, forground, background)

    init_pair(2, 82, 100); // grass

    init_pair(3, 248, 100); // wall

    init_pair(4, 14, 100); // player

    init_pair(5, 46, 100); // mob

    init_pair(1, 0, 150);//39
    bkgd(COLOR_PAIR(1));

    int chunkplayerY = rand()% CHUNK_SIZEY, chunkplayerX = rand() % CHUNK_SIZEX, playerY = rand() % GLOBAL_SIZEY, playerX = rand() % GLOBAL_SIZEX;
    chunkplayerY = 1, chunkplayerX = 0, playerY = 50, playerX = 0;
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
                if ((rand() % 100) == -1)
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
    //move(0, 51*2);
    //for (int i = 0; i < 45; i++)
    //{
    //    if (i < 10)
    //    {
    //        printw("0%d", i);
    //    }
    //    else
    //    {
    //        printw("%d", i);
    //    }
    //}
    printw("\n");
    move(1,0);

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
                    SetDefaultValues(i,j);
                    continue;
                }

                chunkindexY = -1, indexY = GLOBAL_SIZEY;
            }
            if (indexj < 0)
            {
                if (!SCREEN_CHUNKPOSX)
                {
                    SetDefaultValues(i, j);
                    continue;
                }
                chunkindexX = -1, indexX = GLOBAL_SIZEX;
            }
            if (indexi >= GLOBAL_SIZEY)
            {
                if (SCREEN_CHUNKPOSY == CHUNK_SIZEY)
                {
                    SetDefaultValues(i, j);
                    continue;
                }
                chunkindexY = +1, indexY = -GLOBAL_SIZEY;
            }
            if (indexj >= GLOBAL_SIZEX)
            {
                if (SCREEN_CHUNKPOSX == CHUNK_SIZEX)
                {
                    SetDefaultValues(i, j);
                    continue;
                }
                chunkindexX = +1, indexX = -GLOBAL_SIZEX;
            }
            MAP_SCREEN[i][j]->Set_str(MAP_BLOCKS[SCREEN_CHUNKPOSY + chunkindexY][SCREEN_CHUNKPOSX + chunkindexX][indexi + indexY][indexj + indexX]->Get_model());
            MAP_SCREEN[i][j]->Set_color(MAP_BLOCKS[SCREEN_CHUNKPOSY + chunkindexY][SCREEN_CHUNKPOSX + chunkindexX][indexi + indexY][indexj + indexX]->GetColor());
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
    mvprintw(3, SCREEN_SIZEX * 2, "| Y = %d, chunk = %d", SCREEN_POSY, SCREEN_CHUNKPOSY);
    mvprintw(5, SCREEN_SIZEX * 2, "| X = %d, chunk = %d", SCREEN_POSX, SCREEN_CHUNKPOSX);
    refresh();
}


void GAME::SetEntitie(int chunkY, int chunkX, int Y, int X, Entitie* entitie)
{
    MAP_BLOCKS[chunkY][chunkX][Y][X]->Set_Entitie(entitie);
}
void GAME::SetEntitieInBuffer(int chunkY, int chunkX, int Y, int X, Entitie* entitie)
{
    int chunkindexY = 0, chunkindexX = 0;
    if (chunkY < SCREEN_CHUNKPOSY)
        chunkindexY = GLOBAL_SIZEY;
    if (chunkX < SCREEN_CHUNKPOSX)
        chunkindexX = GLOBAL_SIZEX;
    if (chunkY > SCREEN_CHUNKPOSY)
        chunkindexY = -GLOBAL_SIZEY;
    if (chunkX > SCREEN_CHUNKPOSX)
        chunkindexX = -GLOBAL_SIZEX;
    int bufferY = (Y - SCREEN_POSY) + ((RENDER_SIZE+4) / 2) - chunkindexY, bufferX = (X - SCREEN_POSX) + ((RENDER_SIZE+4) / 2) - chunkindexX;
    MAP_BLOCKS_BUFFER[bufferY][bufferX] = entitie;
    entitie->SetchunkposY(chunkY);
    entitie->SetchunkposX(chunkX);
    entitie->SetposY(Y);
    entitie->SetposX(X);
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
void GAME::SetDefaultValues(int i, int j) {
    MAP_SCREEN[i][j]->Set_str("  ");
    MAP_SCREEN[i][j]->Set_color(0);

}
void GAME::SetFromBufferToMap()
{
    int chunkindexY, chunkindexX, indexY, indexX;
    for (int i = 0 ; i < RENDER_SIZE; i++)
    {
        for (int j = 0; j < RENDER_SIZE;j++)
        {
            if (MAP_BLOCKS_BUFFER[i][j] != nullptr)
            {
                MAP_BLOCKS[MAP_BLOCKS_BUFFER[i][j]->GetchunkposY()][MAP_BLOCKS_BUFFER[i][j]->GetchunkposX()][MAP_BLOCKS_BUFFER[i][j]->GetposY()][MAP_BLOCKS_BUFFER[i][j]->GetposX()]->Set_Entitie(MAP_BLOCKS_BUFFER[i][j]);
                MAP_BLOCKS_BUFFER[i][j] = nullptr;
            }
        }
    }
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
int GAME::GetRENDER_SIZE()
{
    return RENDER_SIZE;
}
int GAME::GetGlobal_sizeY()
{
    return GLOBAL_SIZEY;
}
int GAME::GetGlobal_sizeX()
{
    return GLOBAL_SIZEX;
}
int GAME::GetCHUNK_sizeY()
{
    return CHUNK_SIZEY;
}
int GAME::GetCHUNK_sizeX()
{
    return CHUNK_SIZEX;
}
Entitie* GAME::GetEntitie_ptr(int chunkY, int chunkX, int Y, int X)
{
    return MAP_BLOCKS[chunkY][chunkX][Y][X]->GetEntitieptr();
}
Map_block* GAME::GetMap_block_ptr(int chunkY, int chunkX, int Y, int X)
{
    return MAP_BLOCKS[chunkY][chunkX][Y][X];
}

bool GAME::CheckEntitieInBuffer(int chunkY, int chunkX, int Y, int X)
{
    int chunkindexY = 0, chunkindexX = 0;
    if (chunkY < SCREEN_CHUNKPOSY)
        chunkindexY = GLOBAL_SIZEY;
    if (chunkX < SCREEN_CHUNKPOSX)
        chunkindexX = GLOBAL_SIZEX;
    if (chunkY > SCREEN_CHUNKPOSY)
        chunkindexY = -GLOBAL_SIZEY;
    if (chunkX > SCREEN_CHUNKPOSX)
        chunkindexX = -GLOBAL_SIZEX;
    int bufferY = (Y - SCREEN_POSY) + ((RENDER_SIZE + 2) / 2) - chunkindexY, bufferX = (X - SCREEN_POSX) + ((RENDER_SIZE + 2) / 2) - chunkindexX;
    return MAP_BLOCKS_BUFFER[bufferY][bufferX] != nullptr;
}
bool GAME::Checkblock_ptr(int chunkY, int chunkX, int Y, int X)
{
    if (GAME::GetMap_block_ptr(chunkY, chunkX, Y, X) == nullptr)
        return true;
    if (!GAME::GetMap_block_ptr(chunkY, chunkX, Y, X)->IsEmpty())
        return true;
    if (!GAME::GetMap_block_ptr(chunkY, chunkX, Y, X)->Get_CanWalkThêough())
        return true;
    return false;
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
    color = 2;
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
        //Entitie_ptr->Update();
        Entitie_ptr->Action();
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
//int Map_block::Move(int result)
//{
//    int Y = 1, X = 1, CHY = 0, CHX = 0;
//    switch (result)
//    {
//    case 1://w
//        if (posY == 0)
//        {
//            CHY = -1;
//            Y = -(GAME::GetGlobal_sizeY()-1);
//        }
//        GAME::SetEntitie(chunkposY + CHY, chunkposX, posY - Y, posX, Entitie_ptr);
//        Map_block::Set_Entitie(nullptr);
//        return 1;
//    case 2://a
//        if (posX == 0)
//        {
//            CHX = -1;
//            X = -(GAME::GetGlobal_sizeX() - 1);
//        }
//        GAME::SetEntitie(chunkposY, chunkposX + CHX, posY, posX - X, Entitie_ptr);
//        Map_block::Set_Entitie(nullptr);
//        return 1;
//    case 3://s
//        if (posY == (GAME::GetGlobal_sizeY()-1))
//        {
//            CHY = 1;
//            Y = -(GAME::GetGlobal_sizeY() - 1);
//        }
//        GAME::SetEntitie(chunkposY + CHY, chunkposX, posY + Y, posX, Entitie_ptr);
//        Map_block::Set_Entitie(nullptr);
//        return 1;
//    case 4://d
//        if (posX == (GAME::GetGlobal_sizeX()-1))
//        {
//            CHX = 1;
//            X = -(GAME::GetGlobal_sizeX() - 1);
//        }
//        GAME::SetEntitie(chunkposY, chunkposX + CHX, posY, posX + X, Entitie_ptr);
//        Map_block::Set_Entitie(nullptr);
//        return 1;
//    }
//    return 0;
//}
bool Map_block::Get_CanWalkThêough()
{
    return CanWalkThêough;
}
int Map_block::GetColor()
{
    if (Entitie_ptr == nullptr)
        return color;
    else
        return Entitie_ptr->GetColor();
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

Entitie* Map_block::GetEntitieptr()
{
    return Entitie_ptr;
}

StoneWall::StoneWall(int chunkY, int chunkX, int Y, int X) : Map_block( chunkY, chunkX, Y, X)
{
    block_model = "##";
    entitie_model = "##";
    CanWalkThêough = false;
    color = 3;
}
Entitie::Entitie(int chunkY, int chunkX, int Y, int X)
{
    color = 0;
    chunkposY = chunkY;
    chunkposX = chunkX;
    posY = Y;
    posX = X;
}

int Entitie::Move(int result)
{
    int copyposY = posY, copyposX = posX, copychunkposY = chunkposY, copychunkposX = chunkposX,constposY = posY,constposX = posX,constchunkposY = chunkposY,constchunkposX = chunkposX,chunkindex = 0;
    switch (result)
    {
    case 1://w
        if (copyposY == 0)
        {
            if (chunkposY == 0)
                return 0;
            copychunkposY--;
            copyposY = GAME::GetGlobal_sizeY()+1;
        } 
        if (Player::GetPlrScrChunkPosY() != chunkposY)
            chunkindex = GAME::GetGlobal_sizeY()+1;
        if (Player::GetPlrScrPosY() -(constposY - chunkindex) == GAME::GetRENDER_SIZE() / 2)
            break;
        if (GAME::Checkblock_ptr(copychunkposY, copychunkposX, copyposY - 1, copyposX))
            break;
        if (GAME::CheckEntitieInBuffer(copychunkposY, copychunkposX, copyposY - 1, copyposX))
            return 0;
        GAME::SetEntitieInBuffer(copychunkposY, copychunkposX, copyposY - 1, copyposX, GAME::GetEntitie_ptr(chunkposY, chunkposX, posY, posX));
        GAME::SetEntitie(constchunkposY, constchunkposX, constposY, constposX, nullptr);
        return 0;
    case 2://a
        if (copyposX == 0)
        {
            if (chunkposX == 0)
                return 0;
            copychunkposX--;
            copyposX = GAME::GetGlobal_sizeX();
        }
        if (Player::GetPlrScrChunkPosX() != chunkposX)
            chunkindex = GAME::GetGlobal_sizeX()+1;
        if (Player::GetPlrScrPosX() -(constposX - chunkindex) == GAME::GetRENDER_SIZE() / 2)
            break;
        if (GAME::Checkblock_ptr(copychunkposY, copychunkposX, copyposY, copyposX - 1))
            break;
        if (GAME::CheckEntitieInBuffer(copychunkposY, copychunkposX, copyposY, copyposX - 1))
            return 0; 
        if (TurnedRight)
        {
            swap(Entities_model[0], Entities_model[1]);
            TurnedRight = false;
        }
        GAME::SetEntitieInBuffer(copychunkposY, copychunkposX, copyposY, copyposX - 1, GAME::GetEntitie_ptr(chunkposY, chunkposX, posY, posX));
        GAME::SetEntitie(constchunkposY, constchunkposX, constposY, constposX, nullptr);
        return 0;
    case 3://s
        if (copyposY == (GAME::GetGlobal_sizeY() - 1))
        {
            if (chunkposY == GAME::GetCHUNK_sizeY() - 1)
                return 0;
            copychunkposY++;
            copyposY = -1;
        }
        if (Player::GetPlrScrChunkPosY() != chunkposY)
            chunkindex = GAME::GetGlobal_sizeY()+1;
        if (constposY - (Player::GetPlrScrPosY() - chunkindex) == GAME::GetRENDER_SIZE() / 2)
            break;
        if (GAME::Checkblock_ptr(copychunkposY, copychunkposX, copyposY + 1, copyposX))
            break;
        if (GAME::CheckEntitieInBuffer(copychunkposY, copychunkposX, copyposY + 1, copyposX))
            return 0;
        GAME::SetEntitieInBuffer(copychunkposY, copychunkposX, copyposY + 1, copyposX, GAME::GetEntitie_ptr(chunkposY, chunkposX, posY, posX));
        GAME::SetEntitie(constchunkposY, constchunkposX, constposY, constposX, nullptr);
        return 0;
    case 4://d
        if (copyposX == (GAME::GetGlobal_sizeX() - 1))
        {
            if (chunkposX == GAME::GetCHUNK_sizeX() - 1)
                return 0;
            copychunkposX++;
            copyposX = -1;
        }
        if (Player::GetPlrScrChunkPosX() != chunkposX)
            chunkindex = GAME::GetGlobal_sizeX() + 1;
        if (constposX - (Player::GetPlrScrPosX() - chunkindex) == GAME::GetRENDER_SIZE() / 2)
            break;
        if (GAME::Checkblock_ptr(copychunkposY, copychunkposX, copyposY, copyposX + 1))
            break;
        if (GAME::CheckEntitieInBuffer(copychunkposY, copychunkposX, copyposY, copyposX + 1))
            return 0;
        if (!TurnedRight)
        {
            swap(Entities_model[0], Entities_model[1]);
            TurnedRight = true;
        }
        GAME::SetEntitieInBuffer(copychunkposY, copychunkposX, copyposY, copyposX + 1, GAME::GetEntitie_ptr(chunkposY, chunkposX, posY, posX));
        GAME::SetEntitie(constchunkposY, constchunkposX, constposY, constposX, nullptr);
        return 0;
    }

    return 0;
}
void Entitie::Update()
{

}
void Entitie::Action()
{
    if((rand()%100) == 1)
        Move((rand() % 4) + 1);
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
int Entitie::GetColor()
{
    return color;
}
Player::Player(int chunkY, int chunkX, int Y,int X) : Entitie( chunkY,  chunkX, Y, X)
{
    player = true;
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
int Player::Move(int result) 
{
    int copyposY = posY, copyposX = posX, copychunkposY = chunkposY, copychunkposX = chunkposX, constposY = posY, constposX = posX, constchunkposY = chunkposY, constchunkposX = chunkposX;;
    switch (result)
    {
    case 1://w
        if (copyposY == 0)
        {
            if (chunkposY == 0)
                return 0;
            copychunkposY--;
            copyposY = GAME::GetGlobal_sizeY();
            if (GAME::Checkblock_ptr(copychunkposY, copychunkposX, copyposY - 1, copyposX))
                break;
            ScreenPosY = GAME::GetGlobal_sizeY() - 1;
            ScreenChunkPosY = chunkposY - 1;
        }
        else
        {
            if (GAME::Checkblock_ptr(copychunkposY, copychunkposX, copyposY - 1, copyposX))
                return 0;
            ScreenPosY = posY - 1;
            ScreenChunkPosY = chunkposY;
        }
        GAME::SetEntitieInBuffer(copychunkposY, copychunkposX, copyposY - 1, copyposX, GAME::GetEntitie_ptr(chunkposY, chunkposX, posY, posX));
        GAME::SetEntitie(constchunkposY, constchunkposX, constposY, constposX, nullptr);
        return 0;
    case 2://a
        if (copyposX == 0)
        {
            if (chunkposX == 0)
                return 0;
            copychunkposX--;
            copyposX = GAME::GetGlobal_sizeX();
            if (GAME::Checkblock_ptr(copychunkposY, copychunkposX, copyposY, copyposX - 1))
                return 0;
            ScreenPosX = GAME::GetGlobal_sizeX() - 1;
            ScreenChunkPosX = chunkposX - 1;
        }
        else
        {
            if (GAME::Checkblock_ptr(copychunkposY, copychunkposX, copyposY, copyposX - 1))
                return 0;
            ScreenPosX = posX - 1;
            ScreenChunkPosX = chunkposX;
        }
        if (TurnedRight)
        {
            swap(Entities_model[0], Entities_model[1]);
            TurnedRight = false;
        }
        GAME::SetEntitieInBuffer(copychunkposY, copychunkposX, copyposY, copyposX - 1, GAME::GetEntitie_ptr(chunkposY, chunkposX, posY, posX));
        GAME::SetEntitie(constchunkposY, constchunkposX, constposY, constposX, nullptr);
        return 0;
    case 3://s
        if (copyposY == (GAME::GetGlobal_sizeY() - 1))
        {
            if (chunkposY == GAME::GetCHUNK_sizeY() - 1)
                return 0;
            copychunkposY++;
            copyposY = -1;
            if (GAME::Checkblock_ptr(copychunkposY, copychunkposX, copyposY + 1, copyposX))
                return 0;
            ScreenPosY = 0;
            ScreenChunkPosY = chunkposY + 1;
        }
        else
        {
            if (GAME::Checkblock_ptr(copychunkposY, copychunkposX, copyposY + 1, copyposX))
                return 0;
            ScreenPosY = posY + 1;
            ScreenChunkPosY = chunkposY;
        }
        GAME::SetEntitieInBuffer(copychunkposY, copychunkposX, copyposY + 1, copyposX, GAME::GetEntitie_ptr(chunkposY, chunkposX, posY, posX));
        GAME::SetEntitie(constchunkposY, constchunkposX, constposY, constposX, nullptr);
        return 0;
    case 4://d
        if (copyposX == (GAME::GetGlobal_sizeX() - 1))
        {
            if (chunkposX == GAME::GetCHUNK_sizeX() - 1)
                return 0;
            copychunkposX++;
            copyposX = -1;
            ScreenPosX = 0;
            ScreenChunkPosX = chunkposX + 1;
            if (GAME::Checkblock_ptr(copychunkposY, copychunkposX, copyposY, copyposX + 1))
                return 0;
        }
        else
        {
            if (GAME::Checkblock_ptr(copychunkposY, copychunkposX, copyposY, copyposX + 1))
                return 0;
            ScreenPosX = posX + 1;
            ScreenChunkPosX = chunkposX;
        }
        if (!TurnedRight)
        {
            swap(Entities_model[0], Entities_model[1]);
            TurnedRight = true;
        }
        GAME::SetEntitieInBuffer(copychunkposY, copychunkposX, copyposY, copyposX + 1, GAME::GetEntitie_ptr(chunkposY, chunkposX, posY, posX));
        GAME::SetEntitie(constchunkposY, constchunkposX, constposY, constposX, nullptr);
        return 0;
    }

    return 0;
}
void Player::Action()
{
    switch (getch())
    {
    case 119:
        Move(1);
        break;
    case 97:
        Move(2); 
        break;
    case 115:
        Move(3);
        break;
    case 100:
        Move(4);
        break;
    }
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