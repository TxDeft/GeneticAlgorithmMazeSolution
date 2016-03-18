#pragma once
#include "common.h"
#include "CPosition.h"
using namespace std;


class CMazeMap
{
public:
	CMazeMap();
	CMazeMap(CMazeMap &map);    //深拷贝构造函数；因为之后再做比赛两个程序速度的时候，需要复制出多个地图来
    CMazeMap& operator=(CMazeMap &map);//同为深拷贝构造函数-重载赋值运算符
	~CMazeMap();

	void ResetAll();
	bool IsEmpty(){return value==NULL;}
	
	int LoadMap(char* path_fileName,int hh,int ww,int directionMode);

	int GetHeight(){return height;}
	int GetWidth(){return width;}
	int GetDirectionMode(){return directionMode;}
	CPosition& GetInPos(){return inPos;}		//【Q】返回值是引用的时候，如果直接用返回值做其它东西的参数，而不是将它赋给别的变量，是不是就不行了？
	bool IsInPos(CPosition p);
	bool IsAroundInPos(CPosition p,int r);
	bool IsOutPos(CPosition p);

	int GetValue(CPosition p);
	int SetValue(CPosition p, int v);


protected:
	int height,width;
	int directionMode;  //是四向还是八方向，缺省值0
	CPosition inPos;
	vector<CPosition> outPos;
	int **value;
};
