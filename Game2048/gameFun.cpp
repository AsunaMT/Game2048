#include <iostream>
#include "gameFun.h"
using namespace std;

GameEngine::GameEngine(){
	cheat = Cheat::GetCheat();
	game = NULL;	rounds = 0;
	fileTest = false;
	Player virtualPlayer;
	players.push_back(virtualPlayer);
}

void GameEngine::WelcomeScreen(){
	printf("\n——————————————————\n\n");
	printf("    *----* *----* *    *  *----*\n");
	printf("         | |    | |    |  |    |\n");
	printf("    *----* |    | *----*  *----*\n");
	printf("    |      |    |      |  |    |\n");
	printf("    *----* *----*      *  *----*\n\n");
	printf("————Welcome to the game!————\n\n");
}

int GameEngine::ChooseMode(){
	if(fileTest) return -1;
	WelcomeScreen();
	printf("Please choose the mode of the game:\n\n");
	printf("     #Single player---1\n     #Two-player---2\n\n");
	printf("Please input your choice (1 or 2) :\n");
	char buff[20];
	fgets(buff, 20, stdin);
	return (buff[1] == '\n')? atoi(&buff[0]) : 0; 
}

void GameEngine::LoadChessboard(Chessboard *c){
	game = c;
}


void GameEngine::RunARound(int &i){
	PlayerScoreLog log1(&players[i]);
	PlayerBonusLog bonus1(&players[i]);
	for(int j = 0; j < players.size(); j ++){
		printf("The score of %s is: %d\n", players[j].GetName(), players[j].GetScore());
	}
	printf("\nThe operator of the next round is: %s\n\n", players[i].GetName());
	Roundinf infpacket; 
	auto* comR = new RoundCommand(&players[i], game);
	commands["Round"] = comR;
	commands["Round"]->Execute();
	infpacket = comR->GetRoundinf();
	auto* comS = new ScoreCommand(&players[i], infpacket.score, infpacket.dir);
	commands["Score"] = comS;
	commands["Score"]->Execute();
	game->GenerateNum();
	game->PrintMap();
	if(players[i].undoChances > 0) IfUndo(i);
	commands.erase("Score");	commands.erase("Round");
	delete comR;	delete comS;
}

void GameEngine::IfUndo(int &i){
	int &chances = players[i].undoChances;
	printf("You have %d chance(s) to undo, do you want to do it?\n", chances);
	printf("Yes---1/No---0\n");
	int order;	bool end;
	do{
		scanf("%d", &order);	fflush(stdin);
		switch (order){
		case 1:
			if(Player::Multi == false){
				for(; chances > 0; ){
					commands["Score"]->Undo();
					commands["Round"]->Undo();
					game->PrintMap();
					chances --;	rounds --;
					if(chances > 0 && rounds > 0){
						printf("You stil have %d chance(s) to undo, do you want to continue undoing?\n", chances);
						printf("Yes---1/No---0\n");
					}
					else break;
					do{
						scanf("%d", &order);	fflush(stdin);
					}while(order != 1 && order != 0);
					if(order == 0) break;
				}
			}
			else{
				commands["Score"]->Undo();
				commands["Round"]->Undo();
				game->PrintMap();
				i --;	chances --;	rounds --;
			}
			end = false;	break;
		case 0:
			end = false;	break;
		default:
			printf("Please enter again!\n");
			end = true;		break;
		}
	}while(end);
}

void GameEngine::SinglePlayerMode(){
	Player p1;
	players.push_back(p1);
	game->PrintMap();
	int result, i = 0;
    do{
		rounds ++;
		RunARound(i);
		result = game->EndJudge();
	}while(result / 2);
	SinglePlayerModeOver(result);
}
void GameEngine::TwoPlayerMode(){
	Player::Multi = true;
	Player p1("Player1"), p2("Player2");
	players.push_back(p1);	players.push_back(p2);
	game->PrintMap();
	int result, i = 0;
    do{
		rounds ++;
		RunARound(i);
		result = game->EndJudge();
		i = (i + 1) % 2;
	}while(result / 2);
	TwoPlayerModeOver();
}

void GameEngine::SinglePlayerModeOver(int result){
	printf("————Game Over!————\n");
	printf("Your final score is: %d\n", players[0].GetScore());
	if(result == 1) printf("Congratulations！You successed!\n");
	else printf("Unfortunately！Please try again and you will make progress!\n");
}
void GameEngine::TwoPlayerModeOver(){
	printf("————Game Over!————\n");
	printf("The final score of %s is: %d\n", players[0].GetName(), players[0].GetScore());
	printf("The final score of %s is: %d\n\n", players[1].GetName(), players[1].GetScore());
	if(players[0].GetScore() == players[1].GetScore()){
		printf("What a coincidence!The game ends in a tie!\n\n");
	}
	else
		printf("%s is the WINNER!\nCongratulations!\n\n"
		, players[0].GetScore() > players[1].GetScore() ? players[0].GetName() : players[1].GetName());
}

void GameEngine::DealParameter(int argc, char **argv, int &size, int &end){
	string in, out, buff;
	for(int i = 0; i < argc; i++){
		if(argv[i][0] == '-'){
			char parameter = argv[i][1];
			switch(parameter){
				case 't':
					end = 64;
					break;
				case 's':
					size = atoi(argv[i + 1]);
					while(size < 2 || size > 5){
						printf("The dimension you set is illegal!\nPlease set again!\n");
						scanf("%d", &size);	fflush(stdin);
					}
					break;
				case 'i':
					in = argv[i + 1];
					if(!out.empty()) fileTest = true;
					break;
				case 'o':
					out = argv[i + 1];
					if(!in.empty()) fileTest = true;
					break;
				case 'l':
					buff = argv[i];
					if(buff.substr(1) == "log")	Player::Log = true;	
					break;
				case 'p':
					Player::Bonus = true;	break;
				default:
					printf("You entered an invalid parameter!\n");
			}
		}
	}
	if(fileTest) FileTestMode(in, out);
	players.clear();
}

void GameEngine::FileTestMode(const string &in, const string &out){
	FILE *fp1 = fopen(in.c_str(), "r");
	int size; 
	fscanf(fp1, "%d", &size);
	Chessboard game(size);
	char dir = game.LoadFile(fp1);
	FILE *fp2 = fopen(out.c_str(), "w");
	game.OutputFile(fp2, dir);
}