#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <string>
#include <ctime>

using namespace std;

const int MAXEND = 2048;
const int ORIGINSIZE = 4;
const int GRIDNUM = 2;

class Chessboard{
private:
    int Size;
    int ROW;
    int COL;
    int **map;
    int EndCondition;
    void HandleOrder(int &row, int &col, char order);   
    bool CirculationEnd(int j, int end); 
    void Transpose();   //转置棋盘
    void Translation(int dir, int &count);  //朝某一方向平移到底
    int Merge(int dir, int &count); //进行一次合并，返回这一步的得分
public:
    Chessboard(const Chessboard &c);
    Chessboard(int s = ORIGINSIZE, int end = MAXEND);
    ~Chessboard();
    Chessboard& operator=(Chessboard &c);
    void PrintMap();   
    void GenerateNum(int num = GRIDNUM, int i = -1, int j = -1);     //可指定生成一个数，默认随机生成一个2
    int GenerateMap(char order, int &score);    //进行一整步操作
    string MoveJudge(); //返回可移动的所有方向
    int EndJudge();
    char LoadFile(FILE *fp);
    void OutputFile(FILE *fp, char dir);
};
void exchange(int &a, int &b);
#endif