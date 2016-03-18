#pragma once
#include "common.h"
#include "CBaseMazeSln.h"

//******************************��������MazePathNode_S��CStackMazePath��******************************
struct MazePathNode_S
{
	MazePathNode_S(int hh=0,int ww=0,int kk=0){Set(hh,ww,kk);}
	void Set(int hh,int ww,int kk){h=hh;w=ww;k=kk;}
	int h;
	int w;
	int k;  // ��¼�������ĸ�������
};

class CStackMazePath
{
public:
	CStackMazePath();
	~CStackMazePath();
	int Init(int length);
	void Destroy();
	int EnStack(MazePathNode_S& ein);   // ���������ô�������ֻ�Ǵ��ÿ�һ�㣬�ڶ�ս�ڲ�����Ҫ��ֵһ�����ݵ�
	int DeStack(MazePathNode_S& eout);	// �ѳ�ջ�Ķ����ŵ�e��
	int DeStack();	// ������ջ
	int GetTopValue(MazePathNode_S& eout);	// ��ȡջ��Ԫ��

	int GetCntLength(){return cntLength;}
	int GetMaxSize(){return MaxSize;}

	
	
protected:
	MazePathNode_S *data;    //���ڰ�ȫ�Ƕ���Щ������Ӧ��Ū��ͨ���ӿڷ��ʣ�or�ֶ�ֻ������
	int top;  // ջ����ЧԪ��
	int cntLength;
	int MaxSize;

	
};

//******************************��������CStackMazeSln��******************************

class CStackMazeSln:public CBaseMazeSln
{
public:
	void ResetAll();

	bool Run(int refInterval=100);
	void Draw();  //������ǰ��slnMap
	void GetInput();

protected:
	int round;
	CStackMazePath path;
};




