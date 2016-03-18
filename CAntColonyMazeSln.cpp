#include "CAntColonyMazeSln.h"
using namespace std;

//******************************【下面是CAntColonyMazeSln】******************************
void CAntColonyMazeSln::ResetAll()
{
	slnMap.ResetAll();
	// 清除AntColony的辅助结构
	antsOutside.clear();
	//while(!antsInside.empty()) antsInside.pop();
	//while(!antsBreedRoom.empty()) antsBreedRoom.pop();
	round=0;
	foodNum=0;
	deadNum=0;
}


bool CAntColonyMazeSln::Run(int refInterval/*=100*/)
{
	if(!HasMap()) return false;
	Start(refInterval);

	while(true)
	{
		 Update();
	}
	return true;
}


void CAntColonyMazeSln::Start(int refInterval)
{
	refreshInterval=refInterval;
}

void CAntColonyMazeSln::Update()
{
	round++;  // 上来就是Round-1,没有Round-0

	GetInput();
	SendOut();
	SendIn();
	PhDecrease();
	KillAnt();
	for(unsigned int i=0;i<antsOutside.size();i++)
	{
		antsOutside[i].Move(slnMap);
	}
	system("cls");
	Draw();
	Sleep(refreshInterval);
	// getch();
}


void CAntColonyMazeSln::GetInput()
{
	while(kbhit())
	{
		char control=getch();
		int d=100;
		int min=0;
		int max=1000;
		switch(control)
		{
			case 'w':
			case 'W':refreshInterval-=d;if(refreshInterval<=min) refreshInterval=min;break;
			case 's':
			case 'S':refreshInterval+=d;if(refreshInterval>=max) refreshInterval=max;break;
			case 'x':
			case 'X':system("pause");break;
			default:break;
		}
	}
}

void CAntColonyMazeSln::SendOut()
{		
	if(round%SENT_OUT_INTERVAL==1)
	{
		CAnt ant;
		ant.WalkOut(slnMap.GetInPos());
		// 【【【【正经的应该是在Start的时候 往queue里初始化 若干蚂蚁。  然后这里是从queue里往vector里拿】】】】
		if(antsOutside.size()<OUTSIDE_MAX_NUM-SENT_OUT_NUM_ONCE)
		{
			for(int count=0;count<SENT_OUT_NUM_ONCE;count++)
			{
				antsOutside.push_back(ant);
			}
		}
	}
}
void CAntColonyMazeSln::SendIn()
{
	for(vector<CAnt>::iterator it=antsOutside.begin();it!=antsOutside.end();)  //【G】vector的迭代器！
	{
		if((*it).IsCarryingFood() && slnMap.IsAroundInPos((*it).GetPos(),1))  // 说明.的优先级比*高
		{
			// 【【【【从外vector放到内queue】】】】
			//【antsInside.先入队再删除】
			it=antsOutside.erase(it);  // 什么XX迭代器失效问题
			foodNum++;
		}
		else
		{
			++it;
		}
	}
}
void CAntColonyMazeSln::PhDecrease()
{
	if(round%PHEROMONE_DECREASE_INTERVAL==0)
	{
		for(int h=0;h<slnMap.GetHeight();h++)
		{
			for(int w=0;w<slnMap.GetWidth();w++)
			{
				int value=slnMap.GetValue(CPosition(h,w));
				if(value>1000)
				{
					int dv=value-1000;
					dv/=PHEROMONE_DECREASE_RATE;
					if(slnMap.IsAroundInPos(CPosition(h,w),3))  // 家附近的信息素衰减慢  保证能找到家
					{
						dv/=2;
					}
					dv=(dv<1)? 1:dv;
					value-=dv;
					slnMap.SetValue(CPosition(h,w),value);
				}
			}
		}
	}
}
void CAntColonyMazeSln::KillAnt()
{
	if(round%50==0)  // 50回合一起杀一次  防止带食物一起绕圈
	{
		for(vector<CAnt>::iterator it=antsOutside.begin();it!=antsOutside.end();)
		{
			if((*it).IsDead()) 
			{
				it=antsOutside.erase(it); 
				deadNum++;
			}
			else
			{
				++it;
			}
		}
	}
}

void CAntColonyMazeSln::Draw()  //画出当前的slnMap
{
	if(slnMap.IsEmpty()){cout<<"Error:No Map Value."<<endl;}
	else
	{
		cout<<"W-加速;S-减速;X-暂停;   当前刷新间隔:"<<refreshInterval<<"ms"<<endl;
		cout<<"当前回合数:"<<round<<"    窝外蚂蚁数:"<<antsOutside.size()<<"    死亡蚂蚁数:"<<deadNum<<"    收集食物数:"<<foodNum<<endl;
		
		if(refreshInterval<=0) return;

		for(int h=0;h<slnMap.GetHeight();h++)
		{
			for(int w=0;w<slnMap.GetWidth();w++)
			{
				switch(slnMap.GetValue(CPosition(h,w)))
				{
					// ☆★▲△◇◆○●
					case MAPVALUE_WALL:	cout<<"■";break;
					case 1000:
					case MAPVALUE_WAY:	cout<<"  ";break;
					case MAPVALUE_IN:	cout<<"家";break;
					case MAPVALUE_OUT:	cout<<"食";break;
					// 【G】【IPT】mapValue是int的，从1000开始作为信息素为0的取值即可；前面留着，按char用
					default:
						int v=slnMap.GetValue(CPosition(h,w));
						bool isAnt=false;
						unsigned int i=0;
						for(;i<antsOutside.size();)
						{
							if(antsOutside[i].GetPos()==CPosition(h,w))
							{
								isAnt=true;
								break;
							}
							i++;
						}
						if(isAnt)
						{
							if(antsOutside[i].IsDead()) cout<<"死";
							else if(!antsOutside[i].IsCarryingFood()) cout<<"◇";
							else cout<<"◆";
						}
						else if(v>1000 && v<1100)
						{
							v=(v-1000)%100;
							if(v>=10) cout<<v;
							else cout<<" "<<v;
						}
						else cout<<"mx";
						break;
				}
			}
			cout<<endl;
		}
		cout<<endl;
	}
}