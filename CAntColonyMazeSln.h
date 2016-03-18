#pragma once
#include "common.h"
#include "CBaseMazeSln.h"
#include "CAnt.h"

//******************************【下面是CAntColonyMazeSln】******************************
class CAntColonyMazeSln:public CBaseMazeSln
{
public:
	void ResetAll();

	bool Run(int refInterval=100);

	void Start(int refInterval);
	void Update();

	void GetInput();
	void SendOut();
	void SendIn();
	void PhDecrease();
	void KillAnt();
	void Draw();  //画出当前的slnMap

protected:
	vector<CAnt> antsOutside;
	//queue<CAnt> antsInside;
	//queue<CAnt> antsBreedRoom;

	int round;
	int foodNum;
	int deadNum;
};


/*
流程：
0、初始化蚁窝及其中所有的蚂蚁
1、确定要出窝的蚂蚁数
2、将它们出队，记忆力清空，随机放在蚁窝周围某个位置
3、处理vector中的全部蚂蚁
4、等待下一次出窝
5、将回来的蚂蚁，放入交配区等待；当交配区有两只的时候。根据窝内蚂蚁数量判断需要交配否，不需要的话，将先回来那只入队。
6、(queue是动态的，蚁群会不断变大，不会满的)


变量：
在窝蚁群队列    （如果蚁群数量过少，每个蚂蚁带食物回家的时候会等待另一只回家的蚂蚁，然后二者交配下，三个一起入队）
				（理解好：适者生存，不需要评定蚂蚁的功勋值什么的。强迫出去，能活着回来的就是好样的）
在外蚁群vector  （出窝和回窝的时候，在vector和queue间换）

蚁群初始大小
当前窝内蚂蚁数
每次出的只数  （可以随机而来）
出蚂蚁的间隔  （可以在这次出的时候，随机出下一次的间隔）
当前回合
食物收集量

*/