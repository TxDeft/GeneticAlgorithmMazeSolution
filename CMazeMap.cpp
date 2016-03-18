#include"CMazeMap.h"
using namespace std;



CMazeMap::CMazeMap()
{
	value=NULL;  // 【G】有指针的类，构造函数的最重要任务就是把指针赋值为NULL，之后判断方便。没有动态开空间的指针，delete的话会出错！
	ResetAll();
}

CMazeMap::CMazeMap(CMazeMap &map)
{
	ResetAll();

    height = map.height;
    width = map.width;
	directionMode = map.directionMode;
	inPos = map.inPos;
	outPos = map.outPos;

    value = new int*[height];
    for (int h = 0; h < height; h++)
    {
        value[h] = new int[width];
        for (int w = 0; w < width; w++)
        {
            value[h][w] = map.value[h][w];
        }
    }
}

CMazeMap& CMazeMap::operator=(CMazeMap &map)
{
	ResetAll();

    height = map.height;
    width = map.width;
	directionMode = map.directionMode;
	inPos = map.inPos;
	outPos = map.outPos;

    value = new int*[height];
    for (int h = 0; h < height; h++)
    {
        value[h] = new int[width];
        for (int w = 0; w < width; w++)
        {
            value[h][w] = map.value[h][w];
        }
    }
    return *this;
}

CMazeMap::~CMazeMap()
{
	ResetAll();
}

void CMazeMap::ResetAll()
{
	if(value!=NULL)
	{
		for (int h = 0; h < height; h++) delete[]value[h];
		delete[]value;
		value=NULL;
	}
	height=width=0;
	directionMode=0;
	inPos.Set(0,0);
	outPos.clear();
}



int CMazeMap::LoadMap(char* path_fileName,int hh,int ww,int directionMode)  // 地图文件只存地图的东西，不存参数，参数自己指定
{
	ResetAll();
	height=hh;
	width=ww;
	this->directionMode=directionMode;
	ifstream fin(path_fileName,ios::in);

	int h,w;
	value = new int*[height];
	for (h = 0; h < height; h++)
	{
		value[h] = new int[width];
		for (w = 0; w < width; w++)
		{
			char c=fin.get();
			value[h][w]=c;
			if(value[h][w]==MAPVALUE_IN){inPos.Set(h,w);}
			if(value[h][w]==MAPVALUE_OUT){outPos.push_back(CPosition(h,w));}
		}
		fin.get();
	}
	fin.close();
	return 1;
}


int CMazeMap::GetValue(CPosition p)
{
	if(p.GetH()>=0 && p.GetH()<height && p.GetW()>=0 && p.GetW()<width)
	{
		return value[p.GetH()][p.GetW()];
	}
	else return -13;
}

int CMazeMap::SetValue(CPosition p,int v)
{
	if(p.GetH()>=0 && p.GetH()<height && p.GetW()>=0 && p.GetW()<width
	&& value[p.GetH()][p.GetW()]!=1   // 不是墙
	&& !IsInPos(p) && !IsOutPos(p)) // 不是入口和出口
	{
		value[p.GetH()][p.GetW()]=v;
		return 1;
	}
	else return 0;
}


bool CMazeMap::IsInPos(CPosition p)
{
	return p==inPos;
}

bool CMazeMap::IsAroundInPos(CPosition p,int r)
{
	for(int dh=-r;dh<=r;dh++)
	{
		for(int dw=-r;dw<=r;dw++)
		{
			if(IsInPos(CPosition(p.GetH()+dh,p.GetW()+dw))) return true;
		}
	}
	return false;
}

bool CMazeMap::IsOutPos(CPosition p)
{
	for(unsigned int i=0;i<outPos.size();i++)
	{
		if(p==outPos[i]) return true;
	}
	return false;
}