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
	virtual void Draw()=0;  //������ǰ��slnMap

protected:
	CMazeMap slnMap;   //����������Լ��ļ����ͼ�ϱ�ʶ·������ǰλ��ɶ��
	int refreshInterval;
};

