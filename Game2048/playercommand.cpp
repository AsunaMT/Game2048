#include "playercommand.h"
#include <iostream>
using namespace std;

deque<int> ScoreCommand::history = {};
deque<Chessboard> RoundCommand::history = {}; 
const string ScoreCommand::ctype = "Score";
const string RoundCommand::ctype = "Round";

RoundCommand::RoundCommand(Player *p, Chessboard *c):player(p),map(c){ }

void RoundCommand::Execute(){
    history.push_back(*map);
    infpacket = player->PlayARound(*map);
    if(history.size() > UNDOMAX) history.pop_front();
}
void RoundCommand::Undo(){
    *map = history.back();
    history.pop_back();
}

Roundinf& RoundCommand::GetRoundinf(){
    return infpacket;
}


ScoreCommand::ScoreCommand(Player *p, int s, char d):player(p),score(s),dir(d){ }

void ScoreCommand::Execute(){
    int totalPoints = player->GetScore();
    history.push_back(totalPoints);
    player->AddScore(score, dir);
    if(history.size() > UNDOMAX) history.pop_front();
}
void ScoreCommand::Undo(){
    int s = history.back() - player->GetScore();
    if(s < 0) player->AddScore(s, '\0');
    history.pop_back();
}