#pragma once
#include "common.h"
#include "CBaseMazeSln.h"
#include "CCreature.h"

class CGeneticMazeSln:public CBaseMazeSln
{
public:

	void ResetAll();
	bool Run(int refInterval=100);
	void Start(int refInterval);
	void Update();void GetInput();
	void Draw();  //������ǰ��slnMap

protected:
	CCreature cre[8];
	CGeneBank gbank;
	int round;
	int sumStep;		//�������ﹲ�߹����ٲ�
	int findOutTimes;	//�������ﹲ�ҵ����ڶ��ٴ�
};
