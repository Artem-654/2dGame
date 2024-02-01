#pragma once
#include <vector>
#include <string>
using namespace std;
class Entity;
class Map_block;
class Screen_cell;
class GAME
{
    static vector<vector<vector<vector<Map_block*>>>> MAP_BLOCKS;
    static vector<vector<Entity*>> MAP_BLOCKS_BUFFER;
    static vector<vector<Screen_cell*>> MAP_SCREEN;
    static const int RENDER_SIZE = 52;
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
    static void SetEntity(int chunkY, int chunkX, int Y, int X,Entity*);
    static void SetEntityInBuffer(int chunkY, int chunkX, int Y, int X, Entity*);
    static void SetSCREENposY(int Y);
    static void SetSCREENposX(int X);
    static void SetSCREENchunkposY(int chunkposY);
    static void SetSCREENchunkposX(int chunkposX);
    static void SetFromplayerScreenPos();
    static void SetDefaultValues(int i ,int j);
    static void SetFromBufferToMap();
    static void SetEntityHealth(int chunkY, int chunkX, int Y, int X,int damage);
    static void SetEntityHealthInBuffer(int chunkY, int chunkX, int Y, int X, int damage);
    static void GenerateChunk(int ChunkY, int ChunkX);
    static int GetRENDER_SIZE();
    static int GetGlobal_sizeY();
    static int GetGlobal_sizeX();
    static int GetCHUNK_sizeY();
    static int GetCHUNK_sizeX();
    static Entity* GetEntity_ptr(int chunkY, int chunkX, int Y, int X);
    static Map_block* GetMap_block_ptr(int chunkY, int chunkX, int Y, int X);
    static bool CheckEntityInBuffer(int chunkY, int chunkX, int Y, int X);
    static bool Checkblock_ptr(int chunkY, int chunkX, int Y, int X);
    static bool IsPlayer(int chunkY, int chunkX, int Y, int X);
    static int Checkattackblock_ptr(int chunkY, int chunkX, int Y, int X);
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
    string block_model;
    Entity* Entity_ptr = nullptr;
    int chunkposY,chunkposX, posY, posX,color;
    bool transperent, roof, CanWalkThêough;
public:
    Map_block(int chunkY, int chunkX, int Y, int X);
    ~Map_block();
    void Update();
    //void UpdateEntity_model();
    int Get_posY() ;
    int Get_posX() ;
    string Get_model() const;
    void Set_Entity(Entity*);
    void SetposEntity(int chunkY, int chunkX, int Y,int X);
    void Set_EntityHealth(int damage);
    bool IsEmpty();
    bool Get_CanWalkThêough();
    bool IsPlayer();
    //int Move(int result);
    int GetColor();
    Entity* GetEntityptr();
};

class StoneWall : public Map_block
{
public:
    StoneWall(int chunkY, int chunkX, int Y, int X);
};

class Entity
{
protected:
    bool TurnedRight = true,Isplayer = false;
    int chunkposY, chunkposX, posY, posX,color,damage,health,animationcooldown,walkingdelay,standartdelay;
    string Entitys_model, constEntitymodel, AttackEntitymodel;
public:
    Entity(int chunkY, int chunkX ,int Y,int X);
    virtual int Move(int result);
    bool Update();
    virtual void Action();
    string GetModel();
    void SetposY(int Y);
    void SetposX(int X);
    int GetposY();
    int GetposX();
    void SetchunkposY(int Y);
    void SetchunkposX(int X);
    void SetHealth(int damage);
    void Punch(int chunkY, int chunkX, int Y, int X,int damage);
    virtual void killthisEntity();
    int GetchunkposY();
    int GetchunkposX();
    int GetColor();
    bool GetIsPlayer();
    virtual int PathFinding();
};
class Player : public Entity
{
    static int ScreenPosY, ScreenPosX, ScreenChunkPosY, ScreenChunkPosX;
public:
    Player(int chunkY, int chunkX, int Y, int X);
    //void Update() override;
    void killthisEntity() override;
    int Move(int result) override;
    void Action() override;
    static int GetPlrScrPosY();
    static int GetPlrScrPosX();
    static int GetPlrScrChunkPosY();
    static int GetPlrScrChunkPosX();
    //static int GetPlrDamage();
    //static int GetPlrHealth();
};
class Mob : public Entity
{
public:
    Mob(int chunkY, int chunkX, int Y,int X);
};
class AngryMob : public Entity
{
public:
    AngryMob(int chunkY, int chunkX, int Y, int X);
    int PathFinding() override;
};