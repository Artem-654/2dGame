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
    static vector<vector<Screen_cell*>> MAP_SCREEN;
    static const int RENDER_SIZE = 101;
    static const int CHUNK_SIZEX = 1000;
    static const int CHUNK_SIZEY = 1000;
    static const int GLOBAL_SIZEX = 1000;
    static const int GLOBAL_SIZEY = 1000;
    static const int SCREEN_SIZEX = 51;
    static const int SCREEN_SIZEY = 51;
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
    static void SetSCREENposY(int Y);
    static void SetSCREENposX(int X);
    static void SetSCREENchunkposY(int chunkposY);
    static void SetSCREENchunkposX(int chunkposX);
    static void SetFromplayerScreenPos();
    static void GenerateChunk(int ChunkY, int ChunkX);
    //static int GetSCREENPosY();
    //static int GetSCREENPosX();
    //static int GetSCREENChunkPosY();
    //static int GetSCREENChunkPosX();
    //static int GetblockposY(int chunkY, int chunkX, int Y, int X);
    //static int GetblockposX(int chunkY, int chunkX, int Y, int X);
    static int GetGlobal_sizeY();
    static int GetGlobal_sizeX();
    static int PosibleMove(int result, int chunkY, int chunkX, int Y, int X);

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
    int chunkposY,chunkposX, posY, posX,colorpair;
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
    int Move(int result);
    int GetColorpair();
};
class StoneWall : public Map_block
{
public:
    StoneWall(int chunkY, int chunkX, int Y, int X);
};
class Entitie
{
protected:
    bool TurnedRight = true;
    int chunkposY, chunkposX, posY, posX,color;
    string Entities_model;
public:
    Entitie(int chunkY, int chunkX ,int Y,int X);
    virtual int Move();
    virtual void Update();
    string GetModel();
    void SetposY(int Y);
    void SetposX(int X);
    int GetposY();
    int GetposX();
    void SetchunkposY(int Y);
    void SetchunkposX(int X);
    int GetchunkposY();
    int GetchunkposX();
    int GetColorPair();
};
class Player : public Entitie
{
    static int ScreenPosY, ScreenPosX,ScreenChunkPosY, ScreenChunkPosX;
public:
    Player(int chunkY, int chunkX, int Y, int X);
    void Update() override;
    int Move() override;
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