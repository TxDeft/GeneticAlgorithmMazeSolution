#pragma once
#include "common.h"
#include "CBaseMazeSln.h"

//******************************【下面是MazePathNode_S和CStackMazePath】******************************
struct MazePathNode_S
{
	MazePathNode_S(int hh=0,int ww=0,int kk=0){Set(hh,ww,kk);}
	void Set(int hh,int ww,int kk){h=hh;w=ww;k=kk;}
	int h;
	int w;
	int k;  // 记录搜索到哪个方向了
};

class CStackMazePath
{
public:
	CStackMazePath();
	~CStackMazePath();
	int Init(int length);
	void Destroy();
	int EnStack(MazePathNode_S& ein);   // 这里用引用传进来，只是传得快一点，在对战内部还是要赋值一遍数据的
	int DeStack(MazePathNode_S& eout);	// 把出栈的东西放到e中
	int DeStack();	// 单纯出栈
	int GetTopValue(MazePathNode_S& eout);	// 获取栈顶元素

	int GetCntLength(){return cntLength;}
	int GetMaxSize(){return MaxSize;}

	
	
protected:
	MazePathNode_S *data;    //出于安全角度这些东西都应该弄成通过接口访问，or字段只读访问
	int top;  // 栈顶有效元素
	int cntLength;
	int MaxSize;

	
};

//******************************【下面是CStackMazeSln】******************************

class CStackMazeSln:public CBaseMazeSln
{
public:
	void ResetAll();

	bool Run(int refInterval=100);
	void Draw();  //画出当前的slnMap
	void GetInput();

protected:
	int round;
	CStackMazePath path;
};




