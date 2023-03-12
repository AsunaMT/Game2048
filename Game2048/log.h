#ifndef LOG_H
#define LOG_H
#include <string>

#include "player.h"

class PlayerScoreLog : public PlayerEventLogger{
    Player* player;
public:
    PlayerScoreLog(Player* p);
    ~PlayerScoreLog();
    virtual void GetScore(time_t time, char dir, int score);
    virtual void GetBonus(time_t time, double usetime);
};
#endif