#ifndef PLAYERCOMMAND_H
#define PLAYERCOMMAND_H
#include <deque>

#include "player.h"

class PlayerCommand{
public:
    virtual void Execute() = 0;
    virtual void Undo() = 0;
};

class ScoreCommand : public PlayerCommand{
    Player *player;
    int score;
    char dir;
    static deque<int> history;
    static const string ctype;
public:
    ScoreCommand(Player *p, int s, char d);
    void Execute();
    void Undo();
};

class RoundCommand : public PlayerCommand{
    Player *player;
    Chessboard *map;
    static deque<Chessboard> history;
    Roundinf infpacket;
    static const string ctype;
public:
    RoundCommand(Player *p, Chessboard *m);
    void Execute();
    void Undo();
    Roundinf& GetRoundinf();
};
#endif