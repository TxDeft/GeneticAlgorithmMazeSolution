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
	void Draw();  //画出当前的slnMap

protected:
	CCreature cre[8];
	CGeneBank gbank;
	int round;
	int sumStep;		//所有生物共走过多少步
	int findOutTimes;	//所有生物共找到出口多少次
};
