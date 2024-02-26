#include <iostream>
#include <curses.h>
#include <string>
#include <vector>
#include <Windows.h>
#include "Class.h"
using namespace std;
vector<vector<vector<vector<Map_block*>>>> GAME::MAP_BLOCKS;
vector<vector<Entity*>> GAME::MAP_BLOCKS_BUFFER;
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
    short bgrcolor = 100;//100,64

    init_pair(2, 82, bgrcolor); // grass

    init_pair(3, 248, bgrcolor); // wall

    init_pair(4, 14, bgrcolor); // player

    init_pair(5, 46, bgrcolor); // mob

    init_pair(6, 124, bgrcolor);// angry mob

    init_pair(1, 0, 150);//150
    bkgd(COLOR_PAIR(1));

    int chunkplayerY = rand()% CHUNK_SIZEY, chunkplayerX = rand() % CHUNK_SIZEX, playerY = rand() % GLOBAL_SIZEY, playerX = rand() % GLOBAL_SIZEX;
    chunkplayerY = 0, chunkplayerX = 0, playerY = 0, playerX = 0;
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

    MAP_BLOCKS[chunkplayerY][chunkplayerX][playerY][playerX]->Set_Entity(new Player(chunkplayerY, chunkplayerX,playerY, playerX));
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
                       SetEntity(SCREEN_CHUNKPOSY + chunkindexY, SCREEN_CHUNKPOSX + chunkindexX, indexi + indexY, indexj + indexX, new Mob(SCREEN_CHUNKPOSY + chunkindexY, SCREEN_CHUNKPOSX + chunkindexX, indexi + indexY, indexj + indexX));
                    else
                        if(rand() % 1000 == 1)
                            SetEntity(SCREEN_CHUNKPOSY + chunkindexY, SCREEN_CHUNKPOSX + chunkindexX, indexi + indexY, indexj + indexX, new AngryMob(SCREEN_CHUNKPOSY + chunkindexY, SCREEN_CHUNKPOSX + chunkindexX, indexi + indexY, indexj + indexX));
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
    mvprintw(3, SCREEN_SIZEX * 2, " Y = %d, chunk = %d", SCREEN_POSY, SCREEN_CHUNKPOSY);
    mvprintw(5, SCREEN_SIZEX * 2, " X = %d, chunk = %d", SCREEN_POSX, SCREEN_CHUNKPOSX);
    //mvprintw(7, SCREEN_SIZEX * 2, " health = %d, damage = %d", Player::GetPlrHealth(), Player::GetPlrDamage());
    refresh();
}


void GAME::SetEntity(int chunkY, int chunkX, int Y, int X, Entity* Entity)
{
    MAP_BLOCKS[chunkY][chunkX][Y][X]->Set_Entity(Entity);
}
void GAME::SetEntityHealth(int chunkY, int chunkX, int Y, int X, int damage)
{
    MAP_BLOCKS[chunkY][chunkX][Y][X]->Set_EntityHealth(damage);
}
void GAME::SetEntityInBuffer(int chunkY, int chunkX, int Y, int X, Entity* Entity)
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
    int bufferY = (Y - SCREEN_POSY) + ((RENDER_SIZE+2) / 2) - chunkindexY, bufferX = (X - SCREEN_POSX) + ((RENDER_SIZE+2) / 2) - chunkindexX;
    MAP_BLOCKS_BUFFER[bufferY][bufferX] = Entity;
    Entity->SetchunkposY(chunkY);
    Entity->SetchunkposX(chunkX);
    Entity->SetposY(Y);
    Entity->SetposX(X);
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
    for (int i = 0 ; i < RENDER_SIZE+2; i++)
    {
        for (int j = 0; j < RENDER_SIZE+2;j++)
        {
            if (MAP_BLOCKS_BUFFER[i][j] != nullptr)
            {
                MAP_BLOCKS[MAP_BLOCKS_BUFFER[i][j]->GetchunkposY()][MAP_BLOCKS_BUFFER[i][j]->GetchunkposX()][MAP_BLOCKS_BUFFER[i][j]->GetposY()][MAP_BLOCKS_BUFFER[i][j]->GetposX()]->Set_Entity(MAP_BLOCKS_BUFFER[i][j]);
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
Entity* GAME::GetEntity_ptr(int chunkY, int chunkX, int Y, int X)
{
    return MAP_BLOCKS[chunkY][chunkX][Y][X]->GetEntityptr();
}
Map_block* GAME::GetMap_block_ptr(int chunkY, int chunkX, int Y, int X)
{
    return MAP_BLOCKS[chunkY][chunkX][Y][X];
}
bool GAME::CheckEntityInBuffer(int chunkY, int chunkX, int Y, int X)
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
    if (GAME::CheckEntityInBuffer(chunkY, chunkX, Y, X))
        return true;
    return false;
}
bool GAME::IsPlayer(int chunkY, int chunkX, int Y, int X)
{
    return MAP_BLOCKS[chunkY][chunkX][Y][X]->IsPlayer();
}
int GAME::Checkattackblock_ptr(int chunkY, int chunkX, int Y, int X)
{
    if (GAME::GetMap_block_ptr(chunkY, chunkX, Y, X) == nullptr)
        return 0;
    if (GAME::GetMap_block_ptr(chunkY, chunkX, Y, X)->IsEmpty())
        return 0;
    /*if (!GAME::IsPlayer(chunkY, chunkX, Y, X))
        return 0;*/
    if (GAME::CheckEntityInBuffer(chunkY, chunkX, Y, X))
        return 0;
    return 1;
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
    CanWalkThêough = true;
}

Map_block::~Map_block()
{
    delete Entity_ptr;
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
    if (Entity_ptr != nullptr) {
        return Entity_ptr->GetModel();
    }
    else {
        return block_model;
    }
}
void Map_block::Set_EntityHealth(int damage)
{
    Entity_ptr->SetHealth(damage);
}
void Map_block::Update()
{
    if (Entity_ptr != nullptr)
    {
        if (Entity_ptr->Update())
            return;
        Entity_ptr->Action();
    }
}
bool Map_block::IsEmpty()
{
    return this->Entity_ptr == nullptr;
}
bool Map_block::Get_CanWalkThêough()
{
    return CanWalkThêough;
}
bool Map_block::IsPlayer()
{
    return Entity_ptr->GetIsPlayer();
}
int Map_block::GetColor()
{
    if (Entity_ptr == nullptr)
        return color;
    else
        return Entity_ptr->GetColor();
}
void Map_block::Set_Entity(Entity* Entity)
{
    Entity_ptr = Entity;
}

void Map_block::SetposEntity(int chunkY, int chunkX,int Y, int X)
{
    Entity_ptr->SetchunkposY(chunkY);
    Entity_ptr->SetchunkposX(chunkX);
    Entity_ptr->SetposY(Y);
    Entity_ptr->SetposX(X);
}

Entity* Map_block::GetEntityptr()
{
    return Entity_ptr;
}

StoneWall::StoneWall(int chunkY, int chunkX, int Y, int X) : Map_block( chunkY, chunkX, Y, X)
{
    block_model = "##";
    CanWalkThêough = false;
    color = 3;
}
Entity::Entity(int chunkY, int chunkX, int Y, int X)
{
    color = 0;
    health = 90;
    damage = 10;
    walkingdelay = 0;
    standartdelay = 100;
    animationcooldown = 100;
    chunkposY = chunkY;
    chunkposX = chunkX;
    posY = Y;
    posX = X;
}

int Entity::Move(int result)
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
            copyposY = GAME::GetGlobal_sizeY();
        } 
        if (Player::GetPlrScrChunkPosY() != chunkposY)
            chunkindex = GAME::GetGlobal_sizeY()+1;
        if (Player::GetPlrScrPosY() -(constposY - chunkindex) == GAME::GetRENDER_SIZE() / 2)
            break;
        if (GAME::Checkblock_ptr(copychunkposY, copychunkposX, copyposY - 1, copyposX))
            break;
        walkingdelay = standartdelay;
        GAME::SetEntityInBuffer(copychunkposY, copychunkposX, copyposY - 1, copyposX, GAME::GetEntity_ptr(chunkposY, chunkposX, posY, posX));
        GAME::SetEntity(constchunkposY, constchunkposX, constposY, constposX, nullptr);
        return 0;
    case 2://a
        if (TurnedRight)
        {
            swap(Entitys_model[0], Entitys_model[1]);
            swap(constEntitymodel[0], constEntitymodel[1]);
            swap(AttackEntitymodel[0], AttackEntitymodel[1]);
            TurnedRight = false;
        }
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
        walkingdelay = standartdelay;
        GAME::SetEntityInBuffer(copychunkposY, copychunkposX, copyposY, copyposX - 1, GAME::GetEntity_ptr(chunkposY, chunkposX, posY, posX));
        GAME::SetEntity(constchunkposY, constchunkposX, constposY, constposX, nullptr);
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
        walkingdelay = standartdelay;
        GAME::SetEntityInBuffer(copychunkposY, copychunkposX, copyposY + 1, copyposX, GAME::GetEntity_ptr(chunkposY, chunkposX, posY, posX));
        GAME::SetEntity(constchunkposY, constchunkposX, constposY, constposX, nullptr);
        return 0;
    case 4://d
        if (!TurnedRight)
        {
            swap(Entitys_model[0], Entitys_model[1]);
            swap(constEntitymodel[0], constEntitymodel[1]);
            swap(AttackEntitymodel[0], AttackEntitymodel[1]);
            TurnedRight = true;
        }
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
        walkingdelay = standartdelay;
        GAME::SetEntityInBuffer(copychunkposY, copychunkposX, copyposY, copyposX + 1, GAME::GetEntity_ptr(chunkposY, chunkposX, posY, posX));
        GAME::SetEntity(constchunkposY, constchunkposX, constposY, constposX, nullptr);
        return 0;
    case 5:
        if (copyposY == 0)
        {
            if (chunkposY != 0)
            {
                copychunkposY--;
                copyposY = GAME::GetGlobal_sizeY();
                Punch(copychunkposY, copychunkposX, copyposY - 1, copyposX, damage);
                copychunkposY = constchunkposY;
                copyposY = constposY;
            }
        }
        else
        {
            Punch(copychunkposY, copychunkposX, copyposY - 1, copyposX, damage);
        }
        if (copyposX == 0)
        {
            if (chunkposX != 0)
            {
                copychunkposX--;
                copyposX = GAME::GetGlobal_sizeX();
                Punch(copychunkposY, copychunkposX, copyposY, copyposX - 1, damage);
                copychunkposX = constchunkposX;
                copyposX = constposX;
            }
        }
        else
        {
            Punch(copychunkposY, copychunkposX, copyposY, copyposX - 1, damage);
        }
        if (copyposY == (GAME::GetGlobal_sizeY() - 1))
        {
            if (chunkposY != GAME::GetCHUNK_sizeY() - 1)
            {
                copychunkposY++;
                copyposY = -1;
                Punch(copychunkposY, copychunkposX, copyposY + 1, copyposX, damage);
                copychunkposY = constchunkposY;
                copyposY = constposY;
            }
        }
        else
        {
            Punch(copychunkposY, copychunkposX, copyposY + 1, copyposX, damage);
        }
        if (copyposX == (GAME::GetGlobal_sizeX() - 1))
        {
            if (chunkposX != GAME::GetCHUNK_sizeX() - 1)
            {
                copychunkposX++;
                copyposX = -1;
                Punch(copychunkposY, copychunkposX, copyposY, copyposX + 1, damage);
                copychunkposX = constchunkposX;
                copyposX = constposX;
            }
        }
        else
        {
            Punch(copychunkposY, copychunkposX, copyposY, copyposX + 1, damage);
        }
        animationcooldown = 200;
        walkingdelay = standartdelay;
        return 1;
    }

    return 0;
}
bool Entity::Update()
{
    if (health <= 0)
    {
        killthisEntity();
        return true;
    }
    if (animationcooldown == 0)
        Entitys_model = constEntitymodel;
    else
        animationcooldown--;
    if(walkingdelay)
        walkingdelay--;
    return false;
}
void Entity::Action()
{
    if(!walkingdelay&&(!(rand()%1000)))
        if (Move(PathFinding()))//PathFinding()
        {
            Entitys_model = AttackEntitymodel;
        }
}
string Entity::GetModel()
{
    return Entitys_model;
}
void Entity::SetposY(int Y)
{
    posY = Y;
}
void Entity::SetposX(int X)
{
    posX = X;
}
int Entity::GetposY()
{
    return posY;
}
int Entity::GetposX()
{
    return posX;
}
void Entity::SetchunkposY(int Y)
{
    chunkposY = Y;
}
void Entity::SetchunkposX(int X)
{
    chunkposX = X;
}
void Entity::SetHealth(int damage)
{
    health -= damage;
}
void Entity::Punch(int chunkY, int chunkX, int Y, int X, int damage)
{
    switch (GAME::Checkattackblock_ptr(chunkY, chunkX, Y, X))
    {
    case 0:
        break;
    case 1:
        GAME::SetEntityHealth(chunkY, chunkX, Y, X, damage);
        break;
    }

}
void Entity::killthisEntity()
{
    GAME::SetEntity(chunkposY, chunkposX, posY, posX, nullptr);
    delete this;
}
int Entity::GetchunkposY()
{
    return chunkposY;
}
int Entity::GetchunkposX()
{
    return chunkposX;
}
int Entity::GetColor()
{
    return color;
}
bool Entity::GetIsPlayer()
{
    return Isplayer;
}
int Entity::PathFinding()
{
    return (rand() % 5) + 1;
}
Player::Player(int chunkY, int chunkX, int Y,int X) : Entity( chunkY,  chunkX, Y, X)
{
    color = 4;
    damage = 50;
    health = 100;
    standartdelay = 0;
    Isplayer = true;
    Entitys_model = "@!";
    constEntitymodel = "@!";
    AttackEntitymodel = "@-";
    ScreenPosY = Y;
    ScreenPosX = X;
    ScreenChunkPosY = chunkY;
    ScreenChunkPosX = chunkX;
}
void Player::killthisEntity()
{
    GAME::SetEntity(chunkposY, chunkposX, posY, posX, nullptr);
    delete this;
    int chunkplayerY = 0, chunkplayerX = 0, playerY = 0, playerX = 0;
    if(GAME::GetEntity_ptr(chunkplayerY, chunkplayerX, playerY, playerX) != nullptr)
    {
        Punch(chunkplayerY, chunkplayerX, playerY, playerX, 1000000);
    }
    GAME::SetEntity(chunkplayerY, chunkplayerX, playerY , playerX,new Player(chunkplayerY, chunkplayerX, playerY, playerX));
}
//void Player::Update()
//{
//
//}
void Player::Action()
{
    if (walkingdelay)
        return;
    switch (getch())
    {
    case 119:
        Move(1);
        break;
    case 97:
        Move(2);
        if (TurnedRight)
        {
            swap(Entitys_model[0], Entitys_model[1]);
            swap(constEntitymodel[0], constEntitymodel[1]);
            swap(AttackEntitymodel[0], AttackEntitymodel[1]);
            TurnedRight = false;
        }
        break;
    case 115:
        Move(3);
        break;
    case 100:
        Move(4);
        if (!TurnedRight)
        {
            swap(Entitys_model[0], Entitys_model[1]);
            swap(constEntitymodel[0], constEntitymodel[1]);
            swap(AttackEntitymodel[0], AttackEntitymodel[1]);
            TurnedRight = true;
        }
        break;
    case 102:
        if (Move(5))
        {
            Entitys_model = AttackEntitymodel;
        }
        break;
    }
    ScreenPosY = posY;
    ScreenPosX = posX;
    ScreenChunkPosY = chunkposY;
    ScreenChunkPosX = chunkposX;
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
//int Player::GetPlrDamage()
//{
//    return damage;
//}
//int Player::GetPlrHealth()
//{
//    return health;
//}
Mob::Mob(int chunkY, int chunkX, int Y, int X) : Entity( chunkY,  chunkX, Y,X)
{
    color = 5;
    Entitys_model = "g ";
    constEntitymodel = "g ";
    AttackEntitymodel = "g-";
}
AngryMob::AngryMob(int chunkY, int chunkX, int Y, int X) : Entity(chunkY, chunkX, Y, X)
{
    color = 6;
    damage = 30;
    Entitys_model = "G ";
    constEntitymodel = "G ";
    AttackEntitymodel = "G-";
}
int AngryMob::PathFinding()
{
    int chunkindexY = 0, chunkindexX = 0, distanceY,distanceX;
    if (Player::GetPlrScrChunkPosY() != chunkposY)
    {
        if (Player::GetPlrScrChunkPosY() < chunkposY)
        {
            chunkindexY = GAME::GetGlobal_sizeY();
        }
        else
        {
            chunkindexY = -GAME::GetGlobal_sizeY();
        }
    }
    if (Player::GetPlrScrChunkPosX() != chunkposX)
    {
        if (Player::GetPlrScrChunkPosX() < chunkposX)
        {
            chunkindexX = GAME::GetGlobal_sizeX();
        }
        else
        {
            chunkindexX = -GAME::GetGlobal_sizeX();
        }
    }
    distanceY = Player::GetPlrScrPosY() - posY - chunkindexY;
    distanceX = Player::GetPlrScrPosX() - posX - chunkindexX;
    if ((abs(distanceY) + abs(distanceX))<2)
        return 5;
    if (abs(distanceY) < abs(distanceX))
    {
        if (distanceX == abs(distanceX))
        {
            return 4;
        }
        else
        {
            return 2;
        }
    }
    else
    {
        if (distanceY == abs(distanceY))
        {
            return 3;
        }
        else
        {
            return 1;
        }
    }
}