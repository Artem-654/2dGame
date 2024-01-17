#pragma once
#include <vector>
#include <string>
using namespace std;
class Entitie;
class Map_block;
class GAME
{
    static vector<vector<Map_block*>> MAP_BLOCKS;
    //static vector<vector<string>> SCREEN;
    static const int GLOBAL_SIZEX = 100;
    static const int GLOBAL_SIZEY = 100;
    static const int SCREEN_SIZEX = 61;
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
    string block_model = ". ",entitie_model;
    Entitie* Entitie_ptr = nullptr;
    int posY, posX;
    bool transperent = true,roof = false;
public:
    Map_block(int Y, int X);
    ~Map_block();
    void Update();
    void UpdateEntitie_model();
    int Get_posY() ;
    int Get_posX() ;
    string Get_model() const;
    Entitie* Get_Entitie() ;
    void Set_Entitie(Entitie*);
    void SetposEntitie(int Y,int X);
    bool IsEmpty();
    int Move(int move);
};
class Entitie
{
protected:
    int posY = 0, posX = 0, oldposY = 0, oldposX = 0, damage = 30, health = 100;
    string Entities_model;
public:
    Entitie();
    virtual ~Entitie();
    virtual int Move();
    virtual void Update();
    string GetModel();
    void SetposY(int Y);
    void SetposX(int X);
    void addpos(int Y,int X);
    int GetposY();
    int GetposX();
};
class Player : public Entitie
{
public:
    Player(int Y, int X);
    void Update() override;
    int Move() override;
};
//class Mob : public Entitie
//{
//public:
//    Mob();
//    int Move(GAME&) override;
//};