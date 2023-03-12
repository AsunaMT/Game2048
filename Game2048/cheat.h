#ifndef CHEAT_H
#define CHEAT_H
#include <string>

#include "player.h"

class Player;

class Cheat{
    int usage;
    string selfparameter; 
    Player* user;
    Cheat();
    static Cheat* instance;
public:
    static Cheat* GetCheat();
    bool CanCheat();
    void UseCheat(Player* const otherp, const string &parameter);
    Player* GetUser();
    void CheatTakeEffect(const char &dir);
};
#endif