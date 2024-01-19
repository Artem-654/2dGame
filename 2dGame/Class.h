#pragma once
#include <vector>
#include <string>
using namespace std;
class Entitie;
class Map_block;
class GAME
{
    static vector<vector<Map_block*>> MAP_BLOCKS;
    static vector<vector<string>> MAP_SCREEN;
    static const int RENDER_SIZE = 101;
    static const int GLOBAL_SIZEX = 10000;//10 000 is 100 km^2
    static const int GLOBAL_SIZEY = 10000;
    static const int SCREEN_SIZEX = 51;
    static const int SCREEN_SIZEY = 51;
    static int SCREEN_POSY;
    static int SCREEN_POSX;
public:
    GAME();
    ~GAME();
    static void ShowScreen();
    static void Update();
    static void SetEntitie(int Y, int X,Entitie*);
    static void SetposEntitie(int posY, int posX,int Y, int X);
    static void SetSCREENposY(int Y);
    static void SetSCREENposX(int X);
    static int GetXSize();
    static int GetYSize();
    static int GetposY(int Y, int X);
    static int GetposX(int Y, int X);
    static int PosibleMove(int result, int Y, int X);
};
class Map_block
{
protected:
    string block_model,entitie_model;
    Entitie* Entitie_ptr = nullptr;
    int posY, posX,colorpair;
    bool transperent, roof, CanWalkThêough;
public:
    Map_block(int Y, int X);
    ~Map_block();
    void Update();
    void UpdateEntitie_model();
    int Get_posY() ;
    int Get_posX() ;
    string Get_model() const;
    void Set_Entitie(Entitie*);
    void SetposEntitie(int Y,int X);
    bool IsEmpty();
    bool Get_CanWalkThêough();
    int Move(int move);
    int GetColorpair();
};
class StoneWall : public Map_block
{
public:
    StoneWall(int Y, int X);
};
class Entitie
{
protected:
    int posY = 0, posX = 0, oldposY = 0, oldposX = 0, damage = 30, health = 100, colorpair;
    string Entities_model;
public:
    Entitie(int Y,int X);
    virtual ~Entitie();
    virtual int Move();
    virtual void Update();
    string GetModel();
    void SetposY(int Y);
    void SetposX(int X);
    void addpos(int Y,int X);
    int GetposY();
    int GetposX();
    int GetColorPair();
};
class Player : public Entitie
{
public:
    Player(int Y, int X);
    void Update() override;
    int Move() override;
};
class Mob : public Entitie
{
public:
    Mob(int Y,int X);
};