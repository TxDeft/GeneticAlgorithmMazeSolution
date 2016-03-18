#pragma once
#include "common.h"
#include "CPosition.h"
#include "CMazeMap.h"
using namespace std;

//没有完全仿生,而是模拟机器工作的场景
//8个机器人在工作,每次达到目的地以后,返回到达所用步数,并且将最好的两个基因(不是四个是因为任务过程中追求效率)
//可写两种情况,两格基因库或者四格基因库比较效率,先写两格基因库的
//基因是一个结构体数组,里面记录着一定步数的坐标和在这个坐标选择的下一步方向【基因容量是否增大待定】！！这样不好，基因影响行为的时候对基因的遍历复杂度不为1
//基因即为一个short int 的二维数组数组的长宽即位地图大小
//每个机器人完成任务(找到出口后),立刻回家,①比较判断是否更新最优基因库,②让两个最优基因库"杂交"并更新自己的基因库.
//!!!!!!!!!!!!上一行所述方法可以防止自然界中纯种优良性状经几代杂交消失的情况,但是会比较依赖于第一代中最优路径的结果,所以变异应该是较为普遍的.


//变异有两种途径,一种是在杂交的时候可能出现基因的变异,另外一种途径是在行走过程中可能会不按基因规定行走


//不设置回溯,被卡住的基因是不好的应该抛弃!!!
/*
0	1	2

3	*	4

5	6	7


*/

class CGene
{
public:
	CGene();
	~CGene();
	CGene &operator=(CGene &CGene);	//深拷贝
	bool operator > (CGene CGene){return Step>CGene.Step;}
	bool operator <	(CGene CGene){return Step<CGene.Step;}


	int GetGeneValue(int h,int w){return g[h][w];}
	void SetGeneValue(int h,int w,int v){g[h][w] = v;}

	int GetGeneH(){return height;}
	int GetGeneW(){return width;}
	int GetStep(){return Step;}
	void SetGeneH(int h){height = h;}
	void SetGeneW(int w){width = w;}

	void SetStep(int s){Step = s;}

	void InitGene()
	{
		g = new int*[height];
		for(int h=0;h<height;h++)
		{
			g[h] = new int[width];
			for(int w=0;w<width;w++)
			{
				g[h][w] = rand()%8;
			}
		}
	}
	void InitGene(int i)
	{
		g = new int*[height];
		for(int h=0;h<height;h++)
		{
			g[h] = new int[width];
			for(int w=0;w<width;w++)
			{
				g[h][w] = i;
			}
		}
	} 

	void ResetAll();
protected:
	int height,width;
	int Step;
	int **g;

};


class CGeneBank			//基因库类
{
public:
	CGeneBank()
	{
		
	}
	void GeneComeIn(CGene g_new);
	void GeneHybridize();
	CGene &GetHGene(){return g_hybridize;}
	CGene &GetBGene(){return g_best;}
	CGene &GetSGene(){return g_second;}

protected:
	CGene g_best;
	CGene g_second;
	CGene g_hybridize;	//杂交基因
};



class CCreature
{
public:
	CCreature();
	~CCreature();
	CCreature(CCreature &c)
	{
		mem_height = c.mem_height;
		mem_width = c.mem_width;
		PassMap = c.PassMap;
		CrePos = c.CrePos;
		CreGen = c.CreGen;
		mem = new int*[mem_height];
		for(int h=0;h<mem_height;h++)
		{
			mem[h] = new int[mem_width];
			for(int w=0;w<mem_width;w++)
			{
				mem[h][w] = c.mem[h][w];
			}
		}

		
	}
	void RestAll();
	void InitMem()
	{
		mem = new int*[mem_height];
		for(int h=0;h<mem_height;h++)
		{
			mem[h] = new int[mem_width];
			for(int w=0;w<mem_width;w++)
			{
				mem[h][w] = 0;
			}
		}
	}
	int GetMemValue(CPosition p){return mem[p.GetH()][p.GetW()];}
	void SetMemValue(CPosition p,int v){mem[p.GetH()][p.GetW()] = v;}

	void SetMemH(int h){mem_height = h;}
	int GetMemH(){return mem_height;}

	void SetMemW(int w){mem_width = w;}
	int GetMemW(){return mem_width;}

	int GetPass(){return PassMap;}

	void SetCrePos(int h,int w){CrePos.Set(h,w);}
	CPosition GetCrePos(){return CrePos;}

	CGene &GetGene(){return CreGen;}	//深拷贝是这样 这届retuen还是传个引用进来用重写的"="?
	void SetGene(CGene gg){CreGen = gg;}

	bool GetToDes(){return getDestination;}

	void Move(CMazeMap &map,CGeneBank &gbank,int &num);

	void MoveTo(CMazeMap &map,CPosition p);


protected:
	int **mem,mem_height,mem_width;
	int PassMap;
	CPosition CrePos;
	CGene CreGen;

	bool getDestination;


	float innovateProbability;

};