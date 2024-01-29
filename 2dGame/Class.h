#pragma once
#include <vector>
#include <string>
using namespace std;
class Entitie;
class Map_block;
class Screen_cell;
class GAME
{
    static vector<vector<vector<vector<Map_block*>>>> MAP_BLOCKS;
    static vector<vector<Entitie*>> MAP_BLOCKS_BUFFER;
    static vector<vector<Screen_cell*>> MAP_SCREEN;
    static const int RENDER_SIZE = 50;
    static const int CHUNK_SIZEX = 1000;
    static const int CHUNK_SIZEY = 1000;
    static const int GLOBAL_SIZEX = 1000;
    static const int GLOBAL_SIZEY = 1000;
    static const int SCREEN_SIZEX = 50;
    static const int SCREEN_SIZEY = 50;
    static int SCREEN_POSY;
    static int SCREEN_POSX;
    static int SCREEN_CHUNKPOSY;
    static int SCREEN_CHUNKPOSX;
    static bool UpChunkGenerated;
    static bool LeftChunkGenerated;
    static bool RightChunkGenerated;
    static bool DownChunkGenerated;
public:
    GAME();
    ~GAME();
    static void ShowScreen();
    static void Update();
    static void SetEntitie(int chunkY, int chunkX, int Y, int X,Entitie*);
    static void SetEntitieInBuffer(int chunkY, int chunkX, int Y, int X, Entitie*);
    static void SetSCREENposY(int Y);
    static void SetSCREENposX(int X);
    static void SetSCREENchunkposY(int chunkposY);
    static void SetSCREENchunkposX(int chunkposX);
    static void SetFromplayerScreenPos();
    static void SetDefaultValues(int i ,int j);
    static void SetFromBufferToMap();
    static void GenerateChunk(int ChunkY, int ChunkX);
    static int GetRENDER_SIZE();
    static int GetGlobal_sizeY();
    static int GetGlobal_sizeX();
    static int GetCHUNK_sizeY();
    static int GetCHUNK_sizeX();
    static Entitie* GetEntitie_ptr(int chunkY, int chunkX, int Y, int X);
    static Map_block* GetMap_block_ptr(int chunkY, int chunkX, int Y, int X);
    static bool CheckEntitieInBuffer(int chunkY, int chunkX, int Y, int X);
    static bool Checkblock_ptr(int chunkY, int chunkX, int Y, int X);
};
class Screen_cell
{
    string str;
    int color;
public:
    Screen_cell();
    string Get_str();
    int Get_color();
    void Set_str(string str);
    void Set_color(int color);
};
class Map_block
{
protected:
    string block_model,entitie_model;
    Entitie* Entitie_ptr = nullptr;
    int chunkposY,chunkposX, posY, posX,color;
    bool transperent, roof, CanWalkThêough;
public:
    Map_block(int chunkY, int chunkX, int Y, int X);
    ~Map_block();
    void Update();
    void UpdateEntitie_model();
    int Get_posY() ;
    int Get_posX() ;
    string Get_model() const;
    void Set_Entitie(Entitie*);
    void SetposEntitie(int chunkY, int chunkX, int Y,int X);
    bool IsEmpty();
    bool Get_CanWalkThêough();
    //int Move(int result);
    int GetColor();
    Entitie* GetEntitieptr();
};

class StoneWall : public Map_block
{
public:
    StoneWall(int chunkY, int chunkX, int Y, int X);
};

class Entitie
{
protected:
    bool TurnedRight = true,player = false;
    int chunkposY, chunkposX, posY, posX,color;
    string Entities_model;
public:
    Entitie(int chunkY, int chunkX ,int Y,int X);
    virtual int Move(int result);
    virtual void Update();
    virtual void Action();
    string GetModel();
    void SetposY(int Y);
    void SetposX(int X);
    int GetposY();
    int GetposX();
    void SetchunkposY(int Y);
    void SetchunkposX(int X);
    int GetchunkposY();
    int GetchunkposX();
    int GetColor();
};
class Player : public Entitie
{
    static int ScreenPosY, ScreenPosX,ScreenChunkPosY, ScreenChunkPosX;
public:
    Player(int chunkY, int chunkX, int Y, int X);
    void Update() override;
    int Move(int result) override;
    void Action() override;
    static int GetPlrScrPosY();
    static int GetPlrScrPosX();
    static int GetPlrScrChunkPosY();
    static int GetPlrScrChunkPosX();
};
class Mob : public Entitie
{
public:
    Mob(int chunkY, int chunkX, int Y,int X);
};