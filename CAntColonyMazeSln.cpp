#include "CAntColonyMazeSln.h"
using namespace std;

//******************************��������CAntColonyMazeSln��******************************
void CAntColonyMazeSln::ResetAll()
{
	slnMap.ResetAll();
	// ���AntColony�ĸ����ṹ
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
	round++;  // ��������Round-1,û��Round-0

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
		// ��������������Ӧ������Start��ʱ�� ��queue���ʼ�� �������ϡ�  Ȼ�������Ǵ�queue����vector���á�������
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
	for(vector<CAnt>::iterator it=antsOutside.begin();it!=antsOutside.end();)  //��G��vector�ĵ�������
	{
		if((*it).IsCarryingFood() && slnMap.IsAroundInPos((*it).GetPos(),1))  // ˵��.�����ȼ���*��
		{
			// ������������vector�ŵ���queue��������
			//��antsInside.�������ɾ����
			it=antsOutside.erase(it);  // ʲôXX������ʧЧ����
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
					if(slnMap.IsAroundInPos(CPosition(h,w),3))  // �Ҹ�������Ϣ��˥����  ��֤���ҵ���
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
	if(round%50==0)  // 50�غ�һ��ɱһ��  ��ֹ��ʳ��һ����Ȧ
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

void CAntColonyMazeSln::Draw()  //������ǰ��slnMap
{
	if(slnMap.IsEmpty()){cout<<"Error:No Map Value."<<endl;}
	else
	{
		cout<<"W-����;S-����;X-��ͣ;   ��ǰˢ�¼��:"<<refreshInterval<<"ms"<<endl;
		cout<<"��ǰ�غ���:"<<round<<"    ����������:"<<antsOutside.size()<<"    ����������:"<<deadNum<<"    �ռ�ʳ����:"<<foodNum<<endl;
		
		if(refreshInterval<=0) return;

		for(int h=0;h<slnMap.GetHeight();h++)
		{
			for(int w=0;w<slnMap.GetWidth();w++)
			{
				switch(slnMap.GetValue(CPosition(h,w)))
				{
					// ������������
					case MAPVALUE_WALL:	cout<<"��";break;
					case 1000:
					case MAPVALUE_WAY:	cout<<"  ";break;
					case MAPVALUE_IN:	cout<<"��";break;
					case MAPVALUE_OUT:	cout<<"ʳ";break;
					// ��G����IPT��mapValue��int�ģ���1000��ʼ��Ϊ��Ϣ��Ϊ0��ȡֵ���ɣ�ǰ�����ţ���char��
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
							if(antsOutside[i].IsDead()) cout<<"��";
							else if(!antsOutside[i].IsCarryingFood()) cout<<"��";
							else cout<<"��";
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