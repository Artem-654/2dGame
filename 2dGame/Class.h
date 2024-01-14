#pragma once
#include <vector>
using namespace std;
class Mob;
class GAME
{
protected:
    vector<vector<string>> LOCAL_MAP;
    vector<vector<string>> ENTITIES_MAP;
    int GLOBAL_SIZEX = 95;
    int GLOBAL_SIZEY = 51;
    vector<vector<string>> SCREEN;
    vector<Mob*> MOBS;
public:
    GAME(short size1, short size2);
    ~GAME();
    void ShowScreen();
    void ShowMap();
    void Update(GAME&);
    int GetXSize() const;
    int GetYSize() const;
    string GetEntities(int Y, int X);
    void SetEntities(int posY, int posX, string model);
    void MoveXEntities(int posY, int posX, int oldposX, string model);
    void MoveYEntities(int posY, int posX, int oldposY, string model);
    void addEntitie(GAME& game);
};
class Entities
{
protected:
    short posY = 0, posX = 0, oldposY = 0, oldposX = 0, damage = 30, health = 100;
    string Entities_model;
public:
    void Move(GAME&);

    void Spawn(GAME&);
};
class Player : public Entities
{
public:
    int Move(int key, GAME&);
    Player();
};
class Mob : public Entities
{
    //static int _id;
    //int id;

public:
    Mob(GAME&game);
    //void Spawn(GAME&);
};