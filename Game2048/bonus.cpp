#include "bonus.h"
using namespace std;

PlayerBonusLog::PlayerBonusLog(Player* p):player(p){
    p->addObserver(this);
}

PlayerBonusLog::~PlayerBonusLog(){
    player->removeObserver(this);
}


void PlayerBonusLog::GetScore(time_t time, char dir, int score){
    if(score >= 0) player->GetBonus();
    player->lastMoveClock = clock();
}
void PlayerBonusLog::GetBonus(time_t time, double usetime){
    struct tm* t = localtime(&time);
    FILE *logFile = fopen("log.txt", "a");
    fprintf(logFile, "%s %d/%.2d/%.2d %.2d:%.2d:%.2d %.2lf %d\n"
    , player->GetName(), 1900 + t->tm_year, 1 + t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, usetime/1000.0, BONUSSCORE);
    fclose(logFile);
}