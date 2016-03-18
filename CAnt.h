#pragma once
#include "common.h"
#include "CPosition.h"
#include "CMazeMap.h"
using namespace std;


class CAntMemory   //【有指针 需要深拷贝！！！！】
{
public:
	CAntMemory(){/*mem=NULL*/;Clear();}
	~CAntMemory(){Clear();}
	CAntMemory(const CAntMemory &m)
	{
		//mem=NULL;  //【和构造函数一样 必不可少！先把指针复制为 NULL】
		Clear();
		size = m.size;
		cnt = m.cnt;
		//mem = new CPosition[size];
		for (int i = 0; i < size; i++)
		{
			mem[i]=m.mem[i];
		}
	}
	CAntMemory& CAntMemory::operator=(const CAntMemory &m)  // 赋值的重载里面不需要把mem=NULL 已经构造过了
	{
		Clear();
		size = m.size;
		cnt = m.cnt;
		//mem = new CPosition[size];
		for (int i = 0; i < size; i++)
		{
			mem[i]=m.mem[i];
		}
		return *this;
	}

	void InitSize(int s){Clear();/*mem=new CPosition[s];*/size=s;}
	void FlushMem(CPosition p)
	{
		if(/*mem!=NULL*/ true)
		{
			for(int i=0;i<size;i++) mem[i]=p;
		}
	}
	void Clear(){size=0;cnt=0;/*if(mem!=NULL){delete mem;}*/}
	
	void Memorize(CPosition p)
	{
		if(/*mem!=NULL*/ true)
		{
			mem[++cnt%size]=p;
		}
	}  // 规则：每刚到一个点时，为该点记忆赋值

	bool IsInBuffer(CPosition tryPos,int r=0)
	{
		if(/*mem!=NULL*/ true)
		{
			int rr;
			int work=cnt;   
			for(int count=0;count<size;count++,work=(--work+size)%size) // 是回溯记忆，是减法！！
			{
				bool hasRepeatedPos=false;
				for(int k=0;k<size;k++)
				{
					if(work!=k%size && mem[work]==mem[k%size])  hasRepeatedPos=true;
				}
				if(hasRepeatedPos) rr=0;    // 用来应对FlushMem的时候	
				else if(count<ANT_MEMORY_CONE_BUFFER_NUM){rr=(count/2<r)? count/2:r;}
				else rr=r;							// 锥形的已走记忆(前两个点r=0，然后每隔两个点r+1)
				int w1=mem[work].GetW()-rr;
				int w2=mem[work].GetW()+rr;
				int h1=mem[work].GetH()-rr;
				int h2=mem[work].GetH()+rr;
				for(int h=h1;h<=h2;h++)
				{
					for(int w=w1;w<=w2;w++){if(tryPos==CPosition(h,w)) return true;}
				}
			}
			return false;
		}
		return false;
	}

	int GetSize(){return size;}
	


protected:
	int size;
	int cnt;   //当前点的记忆；   规则：每刚到一个点时，为该点记忆赋值
	CPosition mem[ANT_MEMORY_SIZE];//CPosition *mem;
};

struct SPheromoneInfo
{
	double ph;
	int hdir;
	int wdir;

	void operator=(SPheromoneInfo p){ph=p.ph;hdir=p.hdir;wdir=p.wdir;}
	bool operator>=(SPheromoneInfo p){return ph>=p.ph;}
	bool operator<=(SPheromoneInfo p){return ph<=p.ph;}
};

class CAnt
{
public:
	CAnt();
	~CAnt();
	CAnt(const CAnt &a):memory(a.memory)
	{
		pheromoneReleaseRate=a.pheromoneReleaseRate;
		pheromoneReleaseRateCarryingFood=a.pheromoneReleaseRateCarryingFood;
		innovateProbability=a.innovateProbability;
		innovateDuration=a.innovateDuration;
		innovateDurationLeft=a.innovateDurationLeft;
		senseRadius=a.senseRadius;

		cntPos=a.cntPos;
		//memory=a.memory;  // 调用重构的赋值深拷贝！
	

		isCarryingFood=a.isCarryingFood;
		awayTime=a.awayTime;
	}
	
	void WalkOut(CPosition p){cntPos=p;memory.InitSize(ANT_MEMORY_SIZE);memory.FlushMem(p);isCarryingFood=false;awayTime=0;}
	CPosition GetPos(){return cntPos;}
	CAntMemory& GetMemory(){return memory;}
	void Move(CMazeMap &map);    
	void MoveTo(CPosition p,CMazeMap &map);
	// 蚂蚁只能控制自己的移动，带回食物的会移动到洞口，到时候蚁群控制循环下，把在洞口的接进来就好了；  没有带回食物的都不好意思回蚁窝
	// 相当于 蚁群类是个统一处理所有蚂蚁的类，也遍历管理蚁窝外的
	bool IsDead(){return awayTime>AWAY_TIME_LIMIT;}
	bool IsCarryingFood(){return isCarryingFood;}

protected:
	int pheromoneReleaseRate,pheromoneReleaseRateCarryingFood;
	float innovateProbability;
	int innovateDuration;
	int innovateDurationLeft;
	float senseRadius;

	CPosition cntPos;
	CAntMemory memory;

	bool isCarryingFood;
	int awayTime;
};




/*
Ant行动流程
（出蚁窝，记忆力被清空）
1、如果没有携带食物，根据不听话几率，如果中了，则朝不是记忆中已有位置的随机一个方向走；且如果没有带食物不能回家，重新随机一个
2、如果没中，检测周围一定范围内信息素；
3、找出信息素高且不是自己记忆中已有位置的的方向；且如果没有带食物不能回家
4、找到食物，记忆清除为只保留当前所在点，如果携带了食物则不会根据不听话几率随机，会直接找家。


变量：
信息素撒速 （先弄一个，之后改成[2]  一个是不带食物的，一个是带食物的；带食物的高）
是否携带食物
感官范围
大脑记忆  ：   自己写一个环形队列，只需要有对头标识；有一个全部重置方法；有一个入队方法，入队的话，就是往对头位置下一个位置覆盖写入即可。
不听话几率  （先设为固定值，之后课改为跟周围信息素浓度有关，浓度高就几率小；浓度低就很大几率不听话）
当前位置
离窝时间    
*/