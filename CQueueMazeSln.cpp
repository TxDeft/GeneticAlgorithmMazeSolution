#include"CQueueMazeSln.h"

void CQueueMazeSln::ResetAll()
{
	slnMap.ResetAll();
	path.Destroy();
	round = 0;
}



bool CQueueMazeSln::Run(int refInterval/*=100*/)
{
	if(!HasMap()) return false;

	refreshInterval=refInterval;


	int x = slnMap.GetInPos().GetW();
	int y = slnMap.GetInPos().GetH();


	path.InitQueue(x,y);
	QueuePtr M;//广度优先的上一层节点信息


	int (*dir)[2]; //指向数组的指针！
	int dir4[4][2]={{1,0},{0,1},{-1,0},{0,-1}};  // 顺序:下右上左
	int dir8[8][2]={{1,1},{1,0},{0,1},{1,-1},{-1,1},{-1,0},{0,-1},{-1,-1}};  // 顺序:右下，下，右，左下，右上，上，左，左上
	if(slnMap.GetDirectionMode()==4){dir=dir4;}
	else {dir=dir8;}

	bool isFinished=false;
	bool findAccess=false;
	bool B;
	char v;
	int count = 0;



	system("cls");
	Draw();
	Sleep(refreshInterval);
	
		

		do
		{
			GetInput();
			M=path.Q.front;
			v=slnMap.GetValue(CPosition(y,x-1));
			if(x!=0 &&v!=MAPVALUE_WALL &&v!=MAPVALUE_USED && v!= MAPVALUE_IN)//上
			{
				if(v!=MAPVALUE_OUT)
				{
					slnMap.SetValue(CPosition(y,x-1),MAPVALUE_USED);
					path.EnQueue(x-1,y,M);
					system("cls");
					round++;
					Draw();
					
					Sleep(refreshInterval);
				}
				else
				{
					refreshInterval = 100;
					findAccess=true;
					break;
				}
			}
			//-----------------------------------------------------
			v=slnMap.GetValue(CPosition(y+1,x-1));
			if(x!=0&&y!= slnMap.GetHeight()  &&v!=MAPVALUE_WALL &&v!=MAPVALUE_USED && v!= MAPVALUE_IN)//上
			{
				if(v!=MAPVALUE_OUT)
				{
					slnMap.SetValue(CPosition(y+1,x-1),MAPVALUE_USED);
					path.EnQueue(x-1,y+1,M);
					system("cls");
					round++;
					Draw();
					
					Sleep(refreshInterval);
				}
				else
				{
					refreshInterval = 100;
					findAccess=true;
					break;
				}
			}
			//-----------------------------------------------------
			v=slnMap.GetValue(CPosition(y+1,x));
			if(y!= slnMap.GetHeight() &&v!=MAPVALUE_WALL &&v!=MAPVALUE_USED && v!= MAPVALUE_IN)//上
			{
				if(v!=MAPVALUE_OUT)
				{
					slnMap.SetValue(CPosition(y+1,x),MAPVALUE_USED);
					path.EnQueue(x,y+1,M);
					system("cls");
					round++;
					Draw();
					
					Sleep(refreshInterval);
				}
				else
				{
					refreshInterval = 100;
					findAccess=true;
					break;
				}
			}
			//------------------------------------------------------
			v=slnMap.GetValue(CPosition(y+1,x+1));
			if(x!=slnMap.GetWidth()&&y!= slnMap.GetHeight()  &&v!=MAPVALUE_WALL &&v!=MAPVALUE_USED && v!= MAPVALUE_IN)//上
			{
				if(v!=MAPVALUE_OUT)
				{
					slnMap.SetValue(CPosition(y+1,x+1),MAPVALUE_USED);
					path.EnQueue(x+1,y+1,M);
					system("cls");
					round++;
					Draw();
					
					Sleep(refreshInterval);
				}
				else
				{
					refreshInterval = 100;
					findAccess=true;
					break;
				}
			}
			//-----------------------------------------------------
			v=slnMap.GetValue(CPosition(y,x+1));
			if(x!=slnMap.GetWidth() &&v!=MAPVALUE_WALL &&v!=MAPVALUE_USED && v!= MAPVALUE_IN)//上
			{
				if(v!=MAPVALUE_OUT)
				{
					slnMap.SetValue(CPosition(y,x+1),MAPVALUE_USED);
					path.EnQueue(x+1,y,M);
					system("cls");
					round++;
					Draw();
					Sleep(refreshInterval);
				}
				else
				{
					refreshInterval = 100;
					findAccess=true;
					break;
				}
			}
			//------------------------------------------------------
			v=slnMap.GetValue(CPosition(y-1,x+1));
			if(x!=slnMap.GetWidth()&&y!= 0  &&v!=MAPVALUE_WALL &&v!=MAPVALUE_USED && v!= MAPVALUE_IN)//上
			{
				if(v!=MAPVALUE_OUT)
				{
					slnMap.SetValue(CPosition(y-1,x+1),MAPVALUE_USED);
					path.EnQueue(x+1,y-1,M);
					system("cls");
					round++;
					Draw();
					
					Sleep(refreshInterval);
				}
				else
				{
					refreshInterval = 100;
					findAccess=true;
					break;
				}
			}
			//------------------------------------------------------
			
			v=slnMap.GetValue(CPosition(y-1,x));
			if(y!=0 &&v!=MAPVALUE_WALL &&v!=MAPVALUE_USED && v!= MAPVALUE_IN)//上
			{
				if(v!=MAPVALUE_OUT)
				{
					slnMap.SetValue(CPosition(y-1,x),MAPVALUE_USED);
					path.EnQueue(x,y-1,M);
					system("cls");
					round++;
					Draw();
					Sleep(refreshInterval);
				}
				else
				{
					refreshInterval = 100;
					findAccess=true;
					break;
				}
			}
			//------------------------------------------------------
			v=slnMap.GetValue(CPosition(y-1,x-1));
			if(x!=0&&y!= 0  &&v!=MAPVALUE_WALL &&v!=MAPVALUE_USED && v!= MAPVALUE_IN)//上
			{
				if(v!=MAPVALUE_OUT)
				{
					slnMap.SetValue(CPosition(y-1,x-1),MAPVALUE_USED);
					path.EnQueue(x-1,y-1,M);
					system("cls");
					round++;
					Draw();
					
					Sleep(refreshInterval);
				}
				else
				{
					refreshInterval = 100;
					findAccess=true;
					break;
				}
			}
			B=path.DeQueue(x,y,M);
		}while(B==1);



		
	

	if(findAccess)
	{

		if(B==1)
		{
			while(path.MeQueue(x,y,M)==1)
			{
				count++;
				if(slnMap.GetValue(CPosition(y,x))!=MAPVALUE_OUT)
				{
					slnMap.SetValue(CPosition(y,x),MAPVALUE_INPATH);
					system("cls");
					Draw();
				}
				M=M->prier;
			}
		}
		cout<<"A path has been found, the length of the shortest path is "<<count<<endl<<endl;
	}
	else
	{
		cout<<"Sorry，no path can be found."<<endl<<endl;
		return false;
	}

}








void CQueueMazeSln::Draw() //画出当前的slnMap
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






void CQueueMazeSln::GetInput()
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


//******************************【下面是QNode和CQueuekMazePath】******************************
CQueuekMazePath::CQueuekMazePath()
{
	Q.front = Q.rear = new QNode;
	Q.front->next = NULL;
}
CQueuekMazePath::~CQueuekMazePath()
{
	Destroy();
}

 void CQueuekMazePath::InitQueue(int e,int r)
 { // 构造一个空队列Q
   if(!(Q.front=Q.rear=(QueuePtr)malloc(sizeof(QNode))))
     exit(OVERFLOW);
   Q.front->data_x=e;
   Q.front->data_y=r;
   Q.rear->prier=Q.front->next=NULL;
   
 }

 void CQueuekMazePath::EnQueue(int e,int r,QueuePtr M)
 { // 插入元素e为Q的新的队尾元素
   QueuePtr p;

   if(!(p=(QueuePtr)malloc(sizeof(QNode)))) // 存储分配失败
     exit(OVERFLOW);
   p->data_x=e;
   p->data_y=r;
   p->next=NULL;
   
   p->prier=M;

   Q.rear->next=p;
   Q.rear=Q.rear->next;
 }



 bool CQueuekMazePath::DeQueue(int &e,int &r,QueuePtr M)
 { // 若队列不空，删除Q的队头元素，用e返回其值，并返回OK，否则返回ERROR
   QueuePtr p;
   if(M==Q.rear)
     return ERROR;
   p=Q.front->next;
   e=p->data_x;
   r=p->data_y;
   Q.front=Q.front->next;

//   free(p);
   return true;
 }


 bool CQueuekMazePath::MeQueue(int &e,int &r,QueuePtr M)
 { //查看队头元素

   if(M==NULL)
     return ERROR;
   
   e=M->data_x;
   r=M->data_y;

   return true;
 }


 void CQueuekMazePath::Destroy()
{
	while(Q.front)
	{
		Q.rear = Q.front->next;
		delete Q.front;
		Q.front = Q.rear;
	}
}