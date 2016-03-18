#include "CAnt.h"

CAnt::CAnt()
{
	pheromoneReleaseRate = PHEROMONE_RELEASE_RATE;
	pheromoneReleaseRateCarryingFood=PHEROMONE_RELEASE_RATE_CARRYINGFOOD;
	innovateProbability = INNOVATE_PROBABILITY;
	innovateDuration=INNOVATE_DURATION;
	innovateDurationLeft=0;
	senseRadius = SENSE_RADIUS;

	isCarryingFood=false;
	awayTime=0;
	
}

CAnt::~CAnt()
{
	memory.~CAntMemory();
}

//CAnt::CAnt(const CAnt &a):memory(a.memory)
//{
//	pheromoneReleaseRate=a.pheromoneReleaseRate;
//  pheromoneReleaseRateCarryingFood=a.pheromoneReleaseRateCarryingFood;
//	innovateProbability=a.innovateProbability;
//	innovateDuration=a.innovateDuration;
//	innovateDurationLeft=a.innovateDurationLeft;
//	senseRadius=a.senseRadius;
//
//	cntPos=a.cntPos;
//	//memory=a.memory;  // 调用重构的赋值深拷贝！
//	
//
//	isCarryingFood=a.isCarryingFood;
//	awayTime=a.awayTime;
//}

void CAnt::Move(CMazeMap &map)   // 弄8方向的吧！
{
	memory.Memorize(cntPos);
	
	if(IsDead()) return;
	if(IsCarryingFood() && map.IsAroundInPos(cntPos,1)) {MoveTo(map.GetInPos(),map);return;}
	awayTime++;

	if(innovateDurationLeft>0) innovateDurationLeft--;
	else
	{
		int p1=rand()%100+1;
		int p2=int(innovateProbability*100);
		if(p1<=p2)
		{
			innovateDurationLeft=innovateDuration;
		}
	}
		
	if(!IsCarryingFood() && innovateDurationLeft>0)  //创新走法
	{
		//int dir4[4][2]={{1,0},{0,1},{-1,0},{0,-1}};  // 顺序:下右上左
		int dir8[8][2]={{1,-1},{1,1},{-1,1},{-1,-1},{1,0},{0,1},{-1,0},{0,-1}};  //右上，右下，左下，左上；右，下，左，上
		int r;
		CPosition t;
		int attemptTimes=0;
		do
		{
			r=rand()%8;
			t.Set(cntPos.GetH()+dir8[r][0],cntPos.GetW()+dir8[r][1]);
			if(++attemptTimes>ANT_ATTEMPT_TIMES_LIMIT) return;
		}while(memory.IsInBuffer(t,ANT_MEMORY_BUFFER_RADIUS) || map.IsInPos(t) || map.GetValue(t)==MAPVALUE_WALL);
		MoveTo(t,map);
	}
	else  // 跟随信息素
	{
		int h1=cntPos.GetH()-(int)(senseRadius+1);
		h1=(h1<0)? 0:h1;
		int h2=cntPos.GetH()+(int)(senseRadius+1);
		h2=(h2>=map.GetHeight())? map.GetHeight()-1:h2;
		int w1=cntPos.GetW()-(int)(senseRadius+1);
		w1=(w1<0)? 0:w1;
		int w2=cntPos.GetW()+(int)(senseRadius+1);
		w2=(w2>=map.GetWidth())? map.GetWidth()-1:w2;	


		SPheromoneInfo phInfo[9];  //信息素的值 和 方位
		/*
		0 1 2
		3 4 5
		6 7 8
		*/
		for(int i=0;i<9;i++) phInfo[i].ph=0;
		phInfo[4].ph=-13;
		phInfo[0].hdir=-1;phInfo[0].wdir=-1;
		phInfo[1].hdir=-1;phInfo[1].wdir=0;
		phInfo[2].hdir=-1;phInfo[2].wdir=1;
		phInfo[3].hdir=0;phInfo[3].wdir=-1;
		phInfo[4].hdir=0;phInfo[4].wdir=0;
		phInfo[5].hdir=0;phInfo[5].wdir=1;
		phInfo[6].hdir=1;phInfo[6].wdir=-1;
		phInfo[7].hdir=1;phInfo[7].wdir=0;
		phInfo[8].hdir=1;phInfo[8].wdir=1;

		for(int h=h1;h<=h2;h++)
		{
			for(int w=w1;w<=w2;w++)
			{
				if(memory.IsInBuffer(CPosition(h,w),0)) continue;  // 防止被自己的信息素误导以及回头走->在记忆中的点的信息素不统计

				int value=map.GetValue(CPosition(h,w));
				if(value==MAPVALUE_IN)
				{
					if(IsCarryingFood()) value=PHEROMONE_MAX_VALUE;
					else value=-PHEROMONE_MAX_VALUE;
				}
				else if(value==MAPVALUE_OUT)
				{
					if(!IsCarryingFood()) value=PHEROMONE_MAX_VALUE;
					else value=-PHEROMONE_MAX_VALUE;
				}
				else if(value<1000) continue;
				else value-=1000;

				if(h<=cntPos.GetH() && w<=cntPos.GetW())
				{
					phInfo[0].ph+=(double)value+(double(rand()%2))*PHEROMONE_RELEASE_RATE/100.0; //给每个点恩信息素一个扰动，解决信息素等量时的选择问题
				}
				if(h<=cntPos.GetH() && w>=cntPos.GetW())   // 这里不是else if 是因为这样可以保证 中线两边各用一次，公平！
				{
					phInfo[2].ph+=(double)value+(double(rand()%2))*PHEROMONE_RELEASE_RATE/100.0;
				}
				if(h>=cntPos.GetH() && w<=cntPos.GetW())
				{
					phInfo[6].ph+=(double)value+(double(rand()%2))*PHEROMONE_RELEASE_RATE/100.0;
				}
				if(h>=cntPos.GetH() && w>=cntPos.GetW())
				{
					phInfo[8].ph+=(double)value+(double(rand()%2))*PHEROMONE_RELEASE_RATE/100.0;
				}
			}
		}
		phInfo[1].ph=(phInfo[0].ph+phInfo[2].ph)/2.0;
		phInfo[3].ph=(phInfo[0].ph+phInfo[6].ph)/2.0;
		phInfo[5].ph=(phInfo[2].ph+phInfo[8].ph)/2.0;
		phInfo[7].ph=(phInfo[6].ph+phInfo[8].ph)/2.0;


		//【下面是快速排序】
		Tools_QuickOrder<SPheromoneInfo>(phInfo,9);

		CPosition t;
		bool movable=false; // 如果被记忆缓冲区卡住
		for(int i=8;i>1;i--)
		{
			t.Set(cntPos.GetH()+phInfo[i].hdir,cntPos.GetW()+phInfo[i].wdir);
			if( (map.IsInPos(t) && IsCarryingFood())
			|| (!map.IsInPos(t) && !memory.IsInBuffer(t,ANT_MEMORY_BUFFER_RADIUS) && map.GetValue(t)!=MAPVALUE_WALL && (!IsCarryingFood() || !map.IsOutPos(t)) ) )
			{
				MoveTo(t,map);
				movable=true;
				break;
			}
		}
		if(!movable)  // 和上面的随机代码不一样，这里去掉了缓冲区的限制。即只要不是墙随机能走就走。
		{
			int dir8[8][2]={{1,-1},{1,1},{-1,1},{-1,-1},{1,0},{0,1},{-1,0},{0,-1}};  //右上，右下，左下，左上；右，下，左，上
			int r;
			CPosition t;
			int attemptTimes=0;
			do
			{
				r=rand()%8;
				t.Set(cntPos.GetH()+dir8[r][0],cntPos.GetW()+dir8[r][1]);
				if(++attemptTimes>ANT_ATTEMPT_TIMES_LIMIT) return;
			}while((!IsCarryingFood() && map.IsInPos(t)) || map.GetValue(t)==MAPVALUE_WALL);
			MoveTo(t,map);
		}
	}
}

void CAnt::MoveTo(CPosition p,CMazeMap &map)
{
	cntPos=p;
	if(!map.IsOutPos(p))
	{
		int rate=(IsCarryingFood())? pheromoneReleaseRateCarryingFood:pheromoneReleaseRate;
		rate*=(double)(AWAY_TIME_LIMIT-awayTime)/AWAY_TIME_LIMIT;// 体力剩余越多撒的信息素越高
		//int rate=AWAY_TIME_LIMIT-awayTime;  
		int value=map.GetValue(p); 
		value=(value==MAPVALUE_WAY)? 1000+rate:value+rate;
		value=(value<PHEROMONE_MAX_VALUE)? value:PHEROMONE_MAX_VALUE;
		map.SetValue(p,value);
	}
	else
	{
		memory.FlushMem(p);
		isCarryingFood=true;
		awayTime=-AWAY_TIME_LIMIT;
	}
}



/*
Ant行动流程
（出蚁窝，记忆力被清空）
1、如果没有携带食物，根据不听话几率，如果中了，则朝不是记忆中已有位置的随机一个方向走；且如果没有带食物不能回家，重新随机一个
2、如果没中，检测周围一定范围内信息素；
3、找出信息素高且不是自己记忆中已有位置的的方向；且如果没有带食物不能回家
4、找到食物，记忆清除为只保留当前所在点，如果携带了食物则不会根据不听话几率随机，会直接找家。
*/