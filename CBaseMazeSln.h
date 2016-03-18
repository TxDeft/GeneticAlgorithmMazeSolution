#pragma once
#include "common.h"
#include "CMazeMap.h"
using namespace std;

class CBaseMazeSln
{
public:
	virtual void ResetAll()=0;

	virtual bool HasMap();
	virtual bool SetMap(CMazeMap& map);
	virtual bool Run(int refInterval=100)=0;
	virtual void Draw()=0;  //画出当前的slnMap

protected:
	CMazeMap slnMap;   //在这个属于自己的记忆地图上标识路径、当前位置啥的
	int refreshInterval;
};

