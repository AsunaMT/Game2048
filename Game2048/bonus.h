#ifndef BONUS_H
#define BONUS_H
#include <string>
#include "player.h"

class PlayerBonusLog : public PlayerEventLogger{
    Player* player;
public:
    PlayerBonusLog(Player* p);
    ~PlayerBonusLog();
    virtual void GetScore(time_t time, char dir, int score);
    virtual void GetBonus(time_t time, double usetime);
};
#endif