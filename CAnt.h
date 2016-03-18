#pragma once
#include "common.h"
#include "CPosition.h"
#include "CMazeMap.h"
using namespace std;


class CAntMemory   //����ָ�� ��Ҫ�������������
{
public:
	CAntMemory(){/*mem=NULL*/;Clear();}
	~CAntMemory(){Clear();}
	CAntMemory(const CAntMemory &m)
	{
		//mem=NULL;  //���͹��캯��һ�� �ز����٣��Ȱ�ָ�븴��Ϊ NULL��
		Clear();
		size = m.size;
		cnt = m.cnt;
		//mem = new CPosition[size];
		for (int i = 0; i < size; i++)
		{
			mem[i]=m.mem[i];
		}
	}
	CAntMemory& CAntMemory::operator=(const CAntMemory &m)  // ��ֵ���������治��Ҫ��mem=NULL �Ѿ��������
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
	}  // ����ÿ�յ�һ����ʱ��Ϊ�õ���丳ֵ

	bool IsInBuffer(CPosition tryPos,int r=0)
	{
		if(/*mem!=NULL*/ true)
		{
			int rr;
			int work=cnt;   
			for(int count=0;count<size;count++,work=(--work+size)%size) // �ǻ��ݼ��䣬�Ǽ�������
			{
				bool hasRepeatedPos=false;
				for(int k=0;k<size;k++)
				{
					if(work!=k%size && mem[work]==mem[k%size])  hasRepeatedPos=true;
				}
				if(hasRepeatedPos) rr=0;    // ����Ӧ��FlushMem��ʱ��	
				else if(count<ANT_MEMORY_CONE_BUFFER_NUM){rr=(count/2<r)? count/2:r;}
				else rr=r;							// ׶�ε����߼���(ǰ������r=0��Ȼ��ÿ��������r+1)
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
	int cnt;   //��ǰ��ļ��䣻   ����ÿ�յ�һ����ʱ��Ϊ�õ���丳ֵ
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
		//memory=a.memory;  // �����ع��ĸ�ֵ�����
	

		isCarryingFood=a.isCarryingFood;
		awayTime=a.awayTime;
	}
	
	void WalkOut(CPosition p){cntPos=p;memory.InitSize(ANT_MEMORY_SIZE);memory.FlushMem(p);isCarryingFood=false;awayTime=0;}
	CPosition GetPos(){return cntPos;}
	CAntMemory& GetMemory(){return memory;}
	void Move(CMazeMap &map);    
	void MoveTo(CPosition p,CMazeMap &map);
	// ����ֻ�ܿ����Լ����ƶ�������ʳ��Ļ��ƶ������ڣ���ʱ����Ⱥ����ѭ���£����ڶ��ڵĽӽ����ͺ��ˣ�  û�д���ʳ��Ķ�������˼������
	// �൱�� ��Ⱥ���Ǹ�ͳһ�����������ϵ��࣬Ҳ���������������
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
Ant�ж�����
�������ѣ�����������գ�
1�����û��Я��ʳ����ݲ��������ʣ�������ˣ��򳯲��Ǽ���������λ�õ����һ�������ߣ������û�д�ʳ�ﲻ�ܻؼң��������һ��
2�����û�У������Χһ����Χ����Ϣ�أ�
3���ҳ���Ϣ�ظ��Ҳ����Լ�����������λ�õĵķ��������û�д�ʳ�ﲻ�ܻؼ�
4���ҵ�ʳ��������Ϊֻ������ǰ���ڵ㣬���Я����ʳ���򲻻���ݲ����������������ֱ���Ҽҡ�


������
��Ϣ������ ����Ūһ����֮��ĳ�[2]  һ���ǲ���ʳ��ģ�һ���Ǵ�ʳ��ģ���ʳ��ĸߣ�
�Ƿ�Я��ʳ��
�йٷ�Χ
���Լ���  ��   �Լ�дһ�����ζ��У�ֻ��Ҫ�ж�ͷ��ʶ����һ��ȫ�����÷�������һ����ӷ�������ӵĻ�����������ͷλ����һ��λ�ø���д�뼴�ɡ�
����������  ������Ϊ�̶�ֵ��֮��θ�Ϊ����Χ��Ϣ��Ũ���йأ�Ũ�ȸ߾ͼ���С��Ũ�ȵ;ͺܴ��ʲ�������
��ǰλ��
����ʱ��    
*/