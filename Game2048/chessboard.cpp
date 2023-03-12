#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include "chessboard.h"
using namespace std;

Chessboard::Chessboard(const Chessboard &c):Size(c.Size){
	Size = c.Size;
	ROW = c.ROW; COL = c.COL;
	EndCondition = c.EndCondition;
	int *MAP = new int[Size * Size];
	map = new int *[Size];
	for(int i = 0; i < Size; i++){
		map[i] = MAP + i * Size;
	}
	for(int i = 0; i < Size; i++){
		for(int j = 0; j < Size; j++){
			map[i][j] = c.map[i][j];
		}
	}
}
Chessboard::Chessboard(int s, int end):Size(s){
	ROW = COL = s;
	int *MAP = new int[Size * Size];
	map = new int *[Size];
	for(int i = 0; i < Size; i++){
		map[i] = MAP + i * Size;
	}
    EndCondition = end;
	memset(&map[0][0], 0, Size * Size * sizeof(int));
	GenerateNum();	GenerateNum();
}
Chessboard::~Chessboard(){
	delete [] map;
}

Chessboard& Chessboard::operator=(Chessboard &c){
	for(int i = 0; i < ROW; i++){
		for(int j = 0; j < COL; j++){
			map[i][j] = c.map[i][j];
		}
	}
	return *this;
}

void Chessboard::PrintMap(){
	for(int i = 0; i < ROW; i++){
		for(int j = 0; j < COL; j++){
			printf("+-----");
		}
		printf("+\n");
		for(int j = 0; j < COL; j++){
			if(map[i][j] != 0) printf("|%-5d", map[i][j]);
			else printf("|     ");
		}
		printf("|\n");
	}
	for(int j = 0; j < COL; j++){
		printf("+-----");
	}
	printf("+\n");
}

int Chessboard::GenerateMap(char order, int &score){
	int row = 0, col = 0, count = 0;
	HandleOrder(row, col, order);
	int dir = row + col;
	if(row == 0) Transpose();
	Translation(dir, count);
	score += Merge(dir, count);
	if(count == 0){
		if(row == 0) Transpose();
		return 1;
	}
	Translation(dir, count);
	if(row == 0) Transpose();
	return 0;
}

void Chessboard::GenerateNum(int num, int i, int j){
	if(j < 0){
		int k = 0;
		srand(time(NULL));
		do{
			i = rand()%ROW; j = rand()%COL;
			k++;
		}while(map[i][j] > 0 && k <= ROW * COL);
	}
	map[i][j] = num;
}

void Chessboard::HandleOrder(int &row, int &col, char order){
	switch(order){
		case 'w':	row = 0; col = 1;	break;
		case 'z':	row = 0; col = -1;	break;
		case 'a':	row = 1; col = 0;	break;
		case 's':	row = -1; col = 0;	break;
	}
}

void Chessboard::Transpose(){
	if(ROW != COL) exchange(ROW, COL);
	for(int i = 0; i < Size; i++){
		for(int j = i + 1; j < Size; j++){
			exchange(map[i][j], map[j][i]);
		}
	}
}

bool Chessboard::CirculationEnd(int j, int end){
	if(end > 0)		return j < end ? true : false;
	else			return j > end ? true : false;
}

void Chessboard::Translation(int dir, int &count){
	int start = dir > 0 ? 0 : (COL - 1), end = COL - 1 - start;
	for(int i = 0; i < ROW; i++){
		for(int j = start; CirculationEnd(j, end); j += dir){
			if(map[i][j] == 0){
				int k = j + dir, tra = 0;
				while(CirculationEnd(k, end + dir)){
					if(map[i][k]){
						count = 1;
						tra = 1; break;
					}
					else k += dir;
				}
				if(tra) { map[i][j] = map[i][k]; map[i][k] = 0; }
				else break;
			}
		}
	}
}

int Chessboard::Merge(int dir, int &count){
	int score = 0;
	int start = dir > 0 ? 0 : (COL - 1), end = COL - 1 - start;
	for(int i = 0; i < ROW; i++){
		for(int j = start; CirculationEnd(j, end); j += dir){
			if((map[i][j] != 0) && (map[i][j] == map[i][j + dir])){
				score += map[i][j] *= 2; 
				map[i][j + dir] = 0; count = 1;
				j += dir;
			}
		}
	}
	return score;
}

string Chessboard::MoveJudge(){
	string all = "wasz";	int j = 0, buf;
	for(int k = 0; k < all.length() && j < all.length(); k++){
		Chessboard buff(*this);
		if(buff.GenerateMap(all[k], buf)){
			all.erase(j, 1); k--;
		}
		else j++;
	}
	return all;
}

int Chessboard::EndJudge(){
	for(int i = 0; i < ROW; i++){
		for(int j = 0; j < COL; j++){
			if(map[i][j] >= EndCondition) return 1;// Sucessed
		}
	}
	if(MoveJudge().length() == 0) return 0;	// Fail
	else return 2;			// Countinue
}

char Chessboard::LoadFile(FILE *fp){
	for(int i = 0; i < ROW; i++){
		for(int j = 0; j < COL; j++){
			fscanf(fp, "%d", &map[i][j]);
		}
	}
	char dir, cr;	int score = 0;
	fscanf(fp, "%c", &cr);
	fscanf(fp, "%c", &dir);
	fclose(fp);
	return dir;
}

void Chessboard::OutputFile(FILE *fp, char dir){
	string canMove = MoveJudge();
	int score = 0; 
	GenerateMap(dir, score);
	fprintf(fp, "%d ", canMove.length());
	int i, j;
	for(i = 0; i< canMove.length() - 1; i++){
		fprintf(fp, "%c ", canMove[i]);
	}
	fprintf(fp, "%c\n", canMove[i]);
	for(i = 0; i < ROW; i++){
		for(j = 0; j < COL - 1; j++){
			fprintf(fp, "%d ", map[i][j]);
		}
		fprintf(fp, "%d\n", map[i][j]);
	}
	srand(time(NULL));
	do{
		i = rand()%ROW; j = rand()%COL;
	}while(map[i][j] > 0);
	fprintf(fp, "2\n%d %d\n%d", i, j, score);
	fclose(fp);
}

void exchange(int &a, int &b){
	a = a + b;
	b = a - b;
	a = a - b;
}