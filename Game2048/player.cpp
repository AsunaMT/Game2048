#include <iostream>
#include <algorithm>
#include "player.h"
using namespace std;

bool Player::Bonus = false;
bool Player::Log = false;
bool Player::Multi = false;
clock_t Player::lastMoveClock = clock() - (clock_t)1000;

Player::Player(){
	score = 0;
	observers.clear();
	p_name.resize(100);
	p_name = "DefaultUser";
    undoChances = UNDOMAX;
}
Player::Player(const string Name){
	p_name.resize(100); 
	printf("Please input %s's name:\n", Name.c_str());
	getline(cin, p_name); 
	fflush(stdin);
	score = 0;
	undoChances = UNDOMAX;
}
Player::Player(const Player &p){
	p_name.assign(p.p_name);
	score = p.score;
	undoChances = p.undoChances;
}

const char* Player::GetName(){
	return p_name.c_str() ;
}

int Player::GetScore(){
	return score;
}
	
int Player::AddScore(int add, char dir){
	score += add;
	time_t nowTime = time(NULL);
	for(auto observer:observers){
		observer->GetScore(nowTime, dir, add);
	}
	return score;
}

void Player::GetBonus(){
	clock_t nowClock = clock();
	time_t nowTime = time(NULL);
	double dt = (double)((nowClock - lastMoveClock) / ((clock_t)1));
	if(Bonus){
		if(dt < 999.0){
			score += BONUSSCORE;
			if(Log)
			for(auto observer:observers){
				observer->GetBonus(nowTime, dt);
			}
		}
	}	
}

Roundinf& Player::PlayARound(Chessboard &game){
	static Roundinf packet;
	bool hasMove = false;
	int score = 0;	char dir;
	auto *cheat = Cheat::GetCheat();
	if(Multi){
		bool entercheat = (cheat->GetUser() != this) ? true : false;
		string direction = "wasz";
		if(!entercheat)   direction.assign(game.MoveJudge());
		if(!entercheat && direction.length() == 1){
			dir = direction[0];
			cheat->CheatTakeEffect(dir);
			game.GenerateMap(dir, score);
			hasMove = true;
		}
	}
	if(!hasMove)
		do{
			dir = AskForOrder();
		}while(game.GenerateMap(dir, score));
	packet.dir = dir;	packet.score = score;
	return packet;
}

char Player::AskForOrder(){
	printf("Please input the command:\n"); 
	char order, cr, buff[20];
	int end = 1;
	while(end){
		fgets(buff, 20, stdin);
		order = buff[0]; cr = buff[1];
		switch(order){
			case 'w':	case 'a':	case 's':	case 'z':
				if(cr == '\n') end = 0;
				else printf("Nullity!Please enter again!\n");
				break;
			case 'c':
				if(cr == ' '){
					string parameter;
					parameter.resize(100);
					parameter.assign(buff, 2, string::npos);
					HandleCheat(parameter);
				}
				else printf("Nullity!Please enter again!\n");	
				break;
			default:
				printf("Nullity!Please enter again!\n");	break;
			}
		fflush(stdin);
	}	
	return order;
}

void Player::HandleCheat(string &parameter){
	auto *cheat = Cheat::GetCheat();
	if(!Multi) printf("You can't use this order in this mode!\n"); 
	else{
		if(cheat->CanCheat()){
			cheat->UseCheat(this, parameter);
			printf("Command successed!Please go on entering.\n");
		}
		else{
			printf("You have used this order!\n");
		}
	}
}

void Player::addObserver(PlayerEventLogger* observer){
        observers.push_back(observer);
}
void Player::removeObserver(PlayerEventLogger* observer){
    observers.erase(
    find(observers.begin(),observers.end(),observer)
    );
}

Roundinf& Roundinf::operator=(Roundinf &r){
	score = r.score;
	dir = r.dir;
	return *this;
}