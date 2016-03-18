#include "CStackMazeSln.h"
using namespace std;

//******************************【下面是CStackMazeSln】******************************

void CStackMazeSln::ResetAll()
{
	slnMap.ResetAll();
	path.Destroy();
	round = 0;
}

bool CStackMazeSln::Run(int refInterval/*=100*/)
{
	if(!HasMap()) return false;

	refreshInterval=refInterval;
	path.Init(slnMap.GetHeight()*slnMap.GetWidth());

	int (*dir)[2]; //指向数组的指针！
	int dir4[4][2]={{1,0},{0,1},{-1,0},{0,-1}};  // 顺序:下右上左
	int dir8[8][2]={{1,1},{1,0},{0,1},{1,-1},{-1,1},{-1,0},{0,-1},{-1,-1}};  // 顺序:右下，下，右，左下，右上，上，左，左上
	if(slnMap.GetDirectionMode()==4){dir=dir4;}
	else {dir=dir8;}

	int h,w; //用于记录搜索到哪个方向了
	MazePathNode_S cntMpn;

	//int Status=0;
	bool isFinished=false;
	bool findAccess=false;

	

	cntMpn.Set(slnMap.GetInPos().GetH(),slnMap.GetInPos().GetW(),0);

	system("cls");
	Draw();
	Sleep(refreshInterval);
	while(!isFinished)
	{
		GetInput();
		h=cntMpn.h+dir[cntMpn.k][0];
		w=cntMpn.w+dir[cntMpn.k][1];
		char v=slnMap.GetValue(CPosition(h,w));
		if((v==MAPVALUE_INPATH || v==MAPVALUE_WALL || v==MAPVALUE_USED || v==MAPVALUE_IN) // 是已经走过的路或者死路          u表示不是当前路径，但是已经走过的路
		&& cntMpn.k<slnMap.GetDirectionMode()-1)  // 且还有没尝试的方向
		{
			cntMpn.k++;
			//Status=0; // 继续尝试
		}
		else if(v==MAPVALUE_WAY || v==MAPVALUE_OUT) // 是还没走过的路
		{
			path.EnStack(cntMpn); // 记录上一点的坐标及脱离该点的方向数k；（x，y）是尝试点的坐标，不是该点
			slnMap.SetValue(CPosition(cntMpn.h,cntMpn.w),MAPVALUE_INPATH);
			cntMpn.Set(h,w,0);  // 重置搜索方向，该搜下一个点了
			if(v==MAPVALUE_OUT)
			{
				isFinished=true;
				refreshInterval = 100;
				findAccess=true;
			}
			else
			{
				//Status=0;  // 继续搜索
			}

			system("cls");
			
			Draw();
			round++;
			Sleep(refreshInterval);
		}
		else if(cntMpn.k==slnMap.GetDirectionMode()-1)  // 都尝试过了都没通
		{
			// 回溯
			slnMap.SetValue(CPosition(cntMpn.h,cntMpn.w),MAPVALUE_USED);  // 把当前位置设为已经涉足过的
			path.DeStack(cntMpn);
			if(path.GetCntLength()==0)
			{
				isFinished=true;
				refreshInterval = 100;
				findAccess=false;
			}
			else
			{
				cntMpn.k++; // 回溯之后继续搜索下一个方向
				cntMpn.k=(cntMpn.k>slnMap.GetDirectionMode()-1)? slnMap.GetDirectionMode()-1:cntMpn.k;
				
			}

			system("cls");
			
			Draw();
			round++;
			Sleep(refreshInterval);
		}
		
		
		
	}

	if(findAccess)
	{
		cout<<"A path has been found."<<endl<<endl;
		return true;
	}
	else
	{
		cout<<"Sorry，no path can be found."<<endl<<endl;
		return false;
	}
	
}

void CStackMazeSln::Draw() //画出当前的slnMap
{
	if(slnMap.IsEmpty()){cout<<"Error:No Map Value."<<endl;}
	else 
	{
		//cout<<endl;
		cout<<"W-Speed Up;S-Speed Down;X-Pause;   Refresh Interval: "<<refreshInterval<<"ms"<<endl;
		cout<<"☆ is Entrance	★ is Exit	Steps: "<<round<<endl;
		if(refreshInterval!=0)
		{
			for(int h=0;h<slnMap.GetHeight();h++)
			{
				for(int w=0;w<slnMap.GetWidth();w++)
				{
					switch(slnMap.GetValue(CPosition(h,w)))
					{
						// ☆★▲△◇◆○●
						case MAPVALUE_WALL:	cout<<"■";break;
						case MAPVALUE_WAY:	cout<<"  ";break;
						case MAPVALUE_IN:	cout<<"☆";break;
						case MAPVALUE_OUT:	cout<<"★";break;
						case MAPVALUE_INPATH:	cout<<"●";break;
						case MAPVALUE_USED:	cout<<"○";break;  // u是表示踏足过的地方
						default:break;
					}
				}
				cout<<endl;
			}
			cout<<endl;
		}
	}
}



void CStackMazeSln::GetInput()
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





//******************************【下面是MazePathNode_S和CStackMazePath】******************************

CStackMazePath::CStackMazePath()
{
	data=NULL;
}
CStackMazePath::~CStackMazePath()
{
	Destroy();
}
int CStackMazePath::Init(int l)
{
	Destroy();       //代码要经得起折腾！要先清空！
	data = new MazePathNode_S[l];
	if(data==NULL){return 0;}
	else
	{
		top=-1;
		MaxSize=l;
		cntLength=0;
		return 1;
	}
}
void CStackMazePath::Destroy()
{
	if(data!=NULL)           // 要判断下！代码要经得起折腾！
	{
		delete data;
		data = NULL;
	}
}
int CStackMazePath::EnStack(MazePathNode_S& ein)
{
	if(data==NULL){return 0;}//堆栈没有初始化
	if(cntLength==MaxSize){return 0;}
	else
	{
		data[++top]=ein;
		cntLength++;
		return 1;
	}
}
int CStackMazePath::DeStack(MazePathNode_S& eout)
{
	if(data==NULL){return 0;}//堆栈没有初始化
	if(cntLength==0){return 0;}
	else
	{
		eout=data[top];
		top--;
		cntLength--;
		return 1;
	}
}
int CStackMazePath::DeStack()
{
	if(data==NULL){return 0;}//堆栈没有初始化
	if(cntLength==0){return 0;}
	else
	{
		top--;
		cntLength--;
		return 1;
	}
}
int CStackMazePath::GetTopValue(MazePathNode_S& eout)
{
	if(data==NULL){return 0;}//堆栈没有初始化
	if(cntLength==0){return 0;}
	else
	{
		eout=data[top];
		return 1;
	}
}