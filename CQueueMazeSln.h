#pragma once
#include "common.h"
#include "CBaseMazeSln.h"

typedef struct QNode
{
	int data_x;
	int data_y;
	QNode *prier,*next;
}*QueuePtr;

struct LinkQueue
{
	QueuePtr front,rear; // 队头、队尾指针
};
class CQueuekMazePath
{
public:
	CQueuekMazePath();
	~CQueuekMazePath();
	void Destroy();
	void InitQueue(int e,int r);
	void EnQueue(int e,int r,QueuePtr M);
	bool DeQueue(int &e,int &r,QueuePtr M);
	bool MeQueue(int &e,int &r,QueuePtr M);

	LinkQueue	Q;


};



//******************************【下面是CQueueMazeSln】******************************

class CQueueMazeSln:public CBaseMazeSln
{
public:
	void ResetAll();

	bool Run(int refInterval=100);
	void Draw();  //画出当前的slnMap
	void GetInput();


	int round;
	CQueuekMazePath path;
};