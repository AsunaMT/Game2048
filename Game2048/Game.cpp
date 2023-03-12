#include<iostream> 
#include "gameFun.h"
using namespace std;

int main(int argc, char **argv){
	GameEngine GAME;
	int end = MAXEND, size = ORIGINSIZE;
	GAME.DealParameter(argc, argv, size, end);
	Chessboard game(size, end);
	GAME.LoadChessboard(&game);
	for(bool i = true; i; ){
		i = false;
		switch(GAME.ChooseMode()){
			case 1:
				GAME.SinglePlayerMode(); break;
			case 2:
				GAME.TwoPlayerMode(); break;
			case -1:
				break;
			default:
				printf("Invalid input, please try again!\n");
				i = true;	break;
		}
	}
	return 0;
}