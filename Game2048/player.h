#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <ctime>
#include <vector>

#include "chessboard.h"
#include "cheat.h"

using namespace std;

const int BONUSSCORE = 1;
const int UNDOMAX = 3;

class Cheat;

class Roundinf{
public:
    int score;
    char dir;
    Roundinf& operator=(Roundinf &r);
};

class PlayerEventLogger{
public:
    virtual void GetScore(time_t time, char dir, int score) = 0;
    virtual void GetBonus(time_t time, double usetime) = 0;
};

class Player{
    int score;
    string p_name; 
    vector<PlayerEventLogger*> observers;
    char AskForOrder();
    void HandleCheat(string &parameter);
public:
    static clock_t lastMoveClock;
    static bool Log;
    static bool Bonus;
    static bool Multi;
    int undoChances;
    Player();
    Player(const string Name);
    Player(const Player &p);
    const char* GetName();
    int GetScore();
    int AddScore(int add, char dir);
    void GetBonus();
    Roundinf& PlayARound(Chessboard &c);
    void addObserver(PlayerEventLogger* observer);
    void removeObserver(PlayerEventLogger* observer);
};
#endif