#include "cheat.h"
using namespace std;

Cheat* Cheat::instance = new Cheat();
Cheat* Cheat::GetCheat(){
	return instance;
}

Cheat::Cheat(){
	usage = 1;
}

bool Cheat::CanCheat(){
	return (usage > 0) ? true : false;
}
void Cheat::UseCheat(Player* const p, const string &parameter){
	usage--;
	user = p;
	selfparameter.resize(100);
	selfparameter.assign(parameter);
}

Player* Cheat::GetUser(){
	return user;
}

void Cheat::CheatTakeEffect(const char &dir){
	printf("%sIf you agree, please enter %c.\n", selfparameter.c_str(), dir);
	char order, cr, buff[20];
	while(true){
		fgets(buff, 20, stdin); fflush(stdin);
		order = buff[0]; cr = buff[1];
		if(order == dir && cr == '\n'){
			selfparameter.clear(); break;
		} 
		else printf("Nullity!Please enter again!\n");
	}	
}