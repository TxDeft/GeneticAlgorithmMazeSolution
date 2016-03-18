#include "common.h"
#include "CMazeMap.h"
#include "CStackMazeSln.h"
#include "CQueueMazeSln.h"
#include "CGeneticMazeSln.h"

using namespace std;


void main()
{
	srand((unsigned int)time(0));  //设置随机种子，整个程序只设置一次就行！用到main开头吧！不然会出现连续随机出一样的

	CMazeMap map;  //【纵轴是x轴，横轴是y轴；】
	CStackMazeSln stackMazeSln;
	CQueueMazeSln queueMazeSln;
	CGeneticMazeSln geneticMazeSln;
	while(true)
	{
		char control='\0';
		do
		{
			cout<<"Please choose a maze file 1~3(input 0 to exit)：";
			cin>>control;
		}while(control>'3' || control<'0');
	
		if(control=='0'){break;} //退出
		else if(control=='1')
		{
			char *m="maps/map1.txt";
			map.LoadMap(m,10,10,8);
			cout<<"Load with 8 directions "<<m<<"："<<endl;
		}
		else if(control=='2')
		{
			char *m="maps/map2.txt";
			map.LoadMap(m,20,47,8);
			cout<<"Load with 8 directions "<<m<<"："<<endl;
		}
		else if(control=='3')
		{
			char *m="maps/map3.txt";
			map.LoadMap(m,15,33,8);
			cout<<"Load with 8 directions "<<m<<"："<<endl;
		}

		do
		{
			cout<<endl
				<<"Please choose a solution for the maze："<<endl
				<<"1.Depth-first traversal (Stack)"<<endl
				<<"2.Breadth-first traversal (Queue)"<<endl
				<<"3.Genetic Algorithm"<<endl;
			cin>>control;
		}while(control!='1' && control!='2' && control!='3');
		
		system("cls");
		if(control=='1')
		{
			stackMazeSln.SetMap(map);
			stackMazeSln.Run(REFRESH_INTERVAL);
		}
		else if(control=='2')
		{
			queueMazeSln.SetMap(map);
			queueMazeSln.Run(REFRESH_INTERVAL);
			
		}
		else if(control=='3')
		{
			geneticMazeSln.SetMap(map);
			geneticMazeSln.Run(REFRESH_INTERVAL);
			
		}

		system("pause");
	}
}
