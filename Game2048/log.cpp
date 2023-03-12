#include "log.h"
using namespace std;

PlayerScoreLog::PlayerScoreLog(Player* p):player(p){
    p->addObserver(this);
}

PlayerScoreLog::~PlayerScoreLog(){
    player->removeObserver(this);
}

void PlayerScoreLog::GetScore(time_t time, char dir, int score){
    if(player->Log)
    if(score != 0){ 
        struct tm* t = localtime(&time);
        FILE *logFile = fopen("log.txt", "a");
        if(score > 0)
            fprintf(logFile, "%s %d/%.2d/%.2d %.2d:%.2d:%.2d %c %d\n"
            , player->GetName(), 1900 + t->tm_year, 1 + t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, dir, score);
        else
            fprintf(logFile, "%s %d/%.2d/%.2d %.2d:%.2d:%.2d %d\n"
            , player->GetName(), 1900 + t->tm_year, 1 + t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, score);
        fclose(logFile);
    }
}

void PlayerScoreLog::GetBonus(time_t time, double usetime){
    ;
}