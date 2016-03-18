#pragma once
#include "common.h"
#include "CPosition.h"
using namespace std;


class CMazeMap
{
public:
	CMazeMap();
	CMazeMap(CMazeMap &map);    //������캯������Ϊ֮�������������������ٶȵ�ʱ����Ҫ���Ƴ������ͼ��
    CMazeMap& operator=(CMazeMap &map);//ͬΪ������캯��-���ظ�ֵ�����
	~CMazeMap();

	void ResetAll();
	bool IsEmpty(){return value==NULL;}
	
	int LoadMap(char* path_fileName,int hh,int ww,int directionMode);

	int GetHeight(){return height;}
	int GetWidth(){return width;}
	int GetDirectionMode(){return directionMode;}
	CPosition& GetInPos(){return inPos;}		//��Q������ֵ�����õ�ʱ�����ֱ���÷���ֵ�����������Ĳ����������ǽ���������ı������ǲ��ǾͲ����ˣ�
	bool IsInPos(CPosition p);
	bool IsAroundInPos(CPosition p,int r);
	bool IsOutPos(CPosition p);

	int GetValue(CPosition p);
	int SetValue(CPosition p, int v);


protected:
	int height,width;
	int directionMode;  //�������ǰ˷���ȱʡֵ0
	CPosition inPos;
	vector<CPosition> outPos;
	int **value;
};
