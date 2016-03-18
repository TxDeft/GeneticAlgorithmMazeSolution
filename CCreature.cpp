#include"CCreature.h"
using namespace std;
void CGene::ResetAll()
{
	
	//height = width = 0;
	Step = 0;
}

CGene::CGene()
{
	g = NULL;
	ResetAll();
}
CGene::~CGene()
{
	ResetAll();
}

CGene &CGene::operator=(CGene &CGene)
{
	ResetAll();
	height = CGene.height;
	width = CGene.width;
	Step = CGene.Step;

	 g = new int*[height];
	 for (int h = 0; h < height; h++)
    {
        g[h] = new int[width];
        for (int w = 0; w < width; w++)
        {
            g[h][w] = CGene.g[h][w];
        }
    }
    return *this;
}

//----------------------CGeneBank-----------------------------
void CGeneBank::GeneComeIn(CGene g_new)
{
	if(g_new<g_best)	//步数越少越好,只留好的基因
	{
		g_best = g_new;
	}
	else if(g_new<g_second)
	{
		g_second = g_new;
	}
}
void CGeneBank::GeneHybridize()		//基因杂交	10%发生突变 50%遗传自最好基因 40%遗传自次好基因
{
	int hh = g_hybridize.GetGeneH();
	int ww = g_hybridize.GetGeneW();
	g_hybridize.SetStep((g_best.GetStep()+g_second.GetStep())/2);
	for(int h=0;h<hh;h++)
	{
		for(int w=0;w<ww;w++)
		{
			bool innovateHappen = false;
			int p1=rand()%100+1;
			int p2=int(INNOVATE_PROBABILITY*100);
			if(p1<=p2)
			{
				int r=rand()%8;
				g_hybridize.SetGeneValue(h,w,r);
			}
			else if(p1<=60)
			{
				g_hybridize.SetGeneValue(h,w,g_best.GetGeneValue(h,w));
			}
			else
			{
				g_hybridize.SetGeneValue(h,w,g_second.GetGeneValue(h,w));
			}
		}
	}

}






//-----------------CCreature----------------------
CCreature::CCreature()
{
	mem = NULL;
	PassMap = 0;
	getDestination = false;
	innovateProbability = INNOVATE_PROBABILITY;
}
CCreature::~CCreature()
{
	RestAll();
}
void CCreature::RestAll()
{
	for(int h=0;h<mem_height;h++){
		for(int w=0;w<mem_width;w++)
		{
			mem[h][w]=0;
		}
	}
	//mem_height=mem_width=0;
	PassMap = 0;
	CrePos.Set(0,0);
	//CreGen.ResetAll();

	getDestination = false;
}


void CCreature::Move(CMazeMap &map,CGeneBank &gbank,int &num)
{
	int dir8[8][2]={{1,-1},{1,1},{-1,1},{-1,-1},{1,0},{0,1},{-1,0},{0,-1}};  //右上，右下，左下，左上；右，下，左，上
	if(GetToDes())
	{
		num++;
		GetGene().SetStep(GetPass());
		gbank.GeneComeIn(GetGene());
		RestAll();
		gbank.GeneHybridize();
		SetGene(gbank.GetHGene());
		MoveTo(map,map.GetInPos());
		return ;
	}
	bool innovateHappen = false;
	int p1=rand()%100+1;
	int p2=int(innovateProbability*100);
	if(p1<=p2)
	{
		innovateHappen = true;
	}
	if(innovateHappen)  //单纯的变异走没有意义,在路上有一定几率改变基因更合理
	{
		int r = rand()%8;
		GetGene().SetGeneValue(CrePos.GetH(),CrePos.GetW(),r);

	}
	int dead_count=0;	
	int r = (GetGene().GetGeneValue(CrePos.GetH(),CrePos.GetW())-1+8)%8;
	CPosition t;
	do
	{
		dead_count++;
		r = (r+1)%8;
		t.Set(CrePos.GetH()+dir8[r][0],CrePos.GetW()+dir8[r][1]);
		if(dead_count>=10)
		{
			RestAll();

			if(gbank.GetBGene().GetStep()<9999&&gbank.GetSGene().GetStep()<9999)	//开始的杂交很重要,所以一定要保证有意义
			{
				gbank.GeneHybridize();
				SetGene(gbank.GetHGene());
			}
			MoveTo(map,map.GetInPos());
			return;
		}
	}
	while(GetMemValue(t)==1||map.IsInPos(t) || map.GetValue(t)==MAPVALUE_WALL);
	dead_count=0;
	MoveTo(map,t);


}


void CCreature::MoveTo(CMazeMap &map,CPosition p)
{
	CrePos=p;
	PassMap++;
	if(!map.IsOutPos(p))
	{
		if(!map.IsInPos(p))
		{
			SetMemValue(p,1);
			map.SetValue(p,1002);
		}
	}
	else
	{
		getDestination = true;
	}
}