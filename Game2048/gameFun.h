#ifndef GAMEFUN_H
#define GAMEFUN_H
#include <string>
#include <ctime>
#include <map>

#include "player.h"
#include "log.h"
#include "bonus.h"
#include "cheat.h"
#include "playercommand.h"

using namespace std;

class GameEngine{
    int rounds;
    Chessboard* game;
    bool fileTest;
    Cheat *cheat;
    vector<Player> players;
    map<string, PlayerCommand*> commands;
    void WelcomeScreen();
    void SinglePlayerModeOver(int result);
    void TwoPlayerModeOver();
    void FileTestMode(const string &in, const string &out);
    void RunARound(int &i);
    void IfUndo(int &i);
public:
    GameEngine();
    void LoadChessboard(Chessboard *c);
    void DealParameter(int argc, char **argv, int &size, int &end); 
    int ChooseMode();
    void SinglePlayerMode();
    void TwoPlayerMode();
};
#endif