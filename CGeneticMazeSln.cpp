#include"CGeneticMazeSln.h"
using namespace std;
void CGeneticMazeSln::ResetAll()
{
	slnMap.ResetAll();
	// 清除AntColony的辅助结构
	
	//while(!antsInside.empty()) antsInside.pop();
	//while(!antsBreedRoom.empty()) antsBreedRoom.pop();
	round=0;
	sumStep = 0;		//所有生物共走过多少步
	findOutTimes = 0;	//所有生物共找到出口多少次
}
void CGeneticMazeSln::Start(int refInterval)
{
	refreshInterval=refInterval;
	gbank.GetBGene().SetGeneH(slnMap.GetHeight());
	gbank.GetBGene().SetGeneW(slnMap.GetWidth());
	gbank.GetHGene().SetGeneH(slnMap.GetHeight());
	gbank.GetHGene().SetGeneW(slnMap.GetWidth());
	gbank.GetSGene().SetGeneH(slnMap.GetHeight());
	gbank.GetSGene().SetGeneW(slnMap.GetWidth());
	gbank.GetBGene().InitGene(rand()%8);
	gbank.GetBGene().SetStep(9999);
	gbank.GetSGene().InitGene(rand()%8);
	gbank.GetSGene().SetStep(9999);
	gbank.GetHGene().InitGene(rand()%8);
	gbank.GetHGene().SetStep(9999);
	for(int i=0;i<8;i++)
	{
		cre[i].SetCrePos(slnMap.GetInPos().GetH(),slnMap.GetInPos().GetW());
		cre[i].SetMemH(slnMap.GetHeight());
		cre[i].SetMemW(slnMap.GetWidth());

		cre[i].InitMem();
		cre[i].GetGene().SetGeneH(slnMap.GetHeight());
		cre[i].GetGene().SetGeneW(slnMap.GetWidth());

		cre[i].GetGene().InitGene(i);



	}
}

bool CGeneticMazeSln::Run(int refInterval/*=100*/)
{
	if(!HasMap()) return false;
	Start(refInterval);

	while(true)
	{
		 Update();
	}
	return true;
}

void CGeneticMazeSln::GetInput()
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

void CGeneticMazeSln::Update()
{
	round++;  // 上来就是Round-1,没有Round-0

	GetInput();
	
	for(unsigned int i=0;i<8;i++)//@
	{
		cre[i].Move(slnMap,gbank,findOutTimes);
		
	}
	system("cls");
	Draw();
	Sleep(refreshInterval);
	// getch();
}


void CGeneticMazeSln::Draw()  //画出当前的slnMap
{
	if(slnMap.IsEmpty()){cout<<"Error:No Map Value."<<endl;}
	else
	{
		cout<<"W-Speed Up;S-Speed Down;X-Pause;   Refresh Interval: "<<refreshInterval<<"ms"<<endl;
		cout<<"Rounds: "<<round<<" Find the exit for "<<findOutTimes<<" times. The length of current shortest path:"<<gbank.GetBGene().GetStep()<<endl;
		
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
					case MAPVALUE_IN:	cout<<"☆";break;
					case MAPVALUE_OUT:	cout<<"★";break;
					// 【G】【IPT】mapValue是int的，从1000开始作为信息素为0的取值即可；前面留着，按char用
					default:
						int v=slnMap.GetValue(CPosition(h,w));
						bool isAnt=false;
						unsigned int i=0;
						for(;i<8;)//@
						{
							if(cre[i].GetCrePos()==CPosition(h,w))
							{
								isAnt=true;
								break;
							}
							i++;
						}
						if(isAnt)
						{
							
							 cout<<i<<" ";
						}
						else if(v>1000 && v<1100)
						{
							v=(v-1000)%100;
							if(v>=10) cout<<" ";
							else cout<<" "<<" ";
						}
						//else cout<<"mx";
						break;
				}
			}
			cout<<endl;
		}
		cout<<endl;
	}
}