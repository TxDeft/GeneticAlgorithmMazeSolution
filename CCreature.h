#pragma once
#include "common.h"
#include "CPosition.h"
#include "CMazeMap.h"
using namespace std;

//û����ȫ����,����ģ����������ĳ���
//8���������ڹ���,ÿ�δﵽĿ�ĵ��Ժ�,���ص������ò���,���ҽ���õ���������(�����ĸ�����Ϊ���������׷��Ч��)
//��д�������,������������ĸ�����Ƚ�Ч��,��д���������
//������һ���ṹ������,�����¼��һ����������������������ѡ�����һ�����򡾻��������Ƿ���������������������ã�����Ӱ����Ϊ��ʱ��Ի���ı������ӶȲ�Ϊ1
//����Ϊһ��short int �Ķ�ά��������ĳ���λ��ͼ��С
//ÿ���������������(�ҵ����ں�),���̻ؼ�,�ٱȽ��ж��Ƿ�������Ż����,�����������Ż����"�ӽ�"�������Լ��Ļ����.
//!!!!!!!!!!!!��һ�������������Է�ֹ��Ȼ���д���������״�������ӽ���ʧ�����,���ǻ�Ƚ������ڵ�һ��������·���Ľ��,���Ա���Ӧ���ǽ�Ϊ�ձ��.


//����������;��,һ�������ӽ���ʱ����ܳ��ֻ���ı���,����һ��;���������߹����п��ܻ᲻������涨����


//�����û���,����ס�Ļ����ǲ��õ�Ӧ������!!!
/*
0	1	2

3	*	4

5	6	7


*/

class CGene
{
public:
	CGene();
	~CGene();
	CGene &operator=(CGene &CGene);	//���
	bool operator > (CGene CGene){return Step>CGene.Step;}
	bool operator <	(CGene CGene){return Step<CGene.Step;}


	int GetGeneValue(int h,int w){return g[h][w];}
	void SetGeneValue(int h,int w,int v){g[h][w] = v;}

	int GetGeneH(){return height;}
	int GetGeneW(){return width;}
	int GetStep(){return Step;}
	void SetGeneH(int h){height = h;}
	void SetGeneW(int w){width = w;}

	void SetStep(int s){Step = s;}

	void InitGene()
	{
		g = new int*[height];
		for(int h=0;h<height;h++)
		{
			g[h] = new int[width];
			for(int w=0;w<width;w++)
			{
				g[h][w] = rand()%8;
			}
		}
	}
	void InitGene(int i)
	{
		g = new int*[height];
		for(int h=0;h<height;h++)
		{
			g[h] = new int[width];
			for(int w=0;w<width;w++)
			{
				g[h][w] = i;
			}
		}
	} 

	void ResetAll();
protected:
	int height,width;
	int Step;
	int **g;

};


class CGeneBank			//�������
{
public:
	CGeneBank()
	{
		
	}
	void GeneComeIn(CGene g_new);
	void GeneHybridize();
	CGene &GetHGene(){return g_hybridize;}
	CGene &GetBGene(){return g_best;}
	CGene &GetSGene(){return g_second;}

protected:
	CGene g_best;
	CGene g_second;
	CGene g_hybridize;	//�ӽ�����
};



class CCreature
{
public:
	CCreature();
	~CCreature();
	CCreature(CCreature &c)
	{
		mem_height = c.mem_height;
		mem_width = c.mem_width;
		PassMap = c.PassMap;
		CrePos = c.CrePos;
		CreGen = c.CreGen;
		mem = new int*[mem_height];
		for(int h=0;h<mem_height;h++)
		{
			mem[h] = new int[mem_width];
			for(int w=0;w<mem_width;w++)
			{
				mem[h][w] = c.mem[h][w];
			}
		}

		
	}
	void RestAll();
	void InitMem()
	{
		mem = new int*[mem_height];
		for(int h=0;h<mem_height;h++)
		{
			mem[h] = new int[mem_width];
			for(int w=0;w<mem_width;w++)
			{
				mem[h][w] = 0;
			}
		}
	}
	int GetMemValue(CPosition p){return mem[p.GetH()][p.GetW()];}
	void SetMemValue(CPosition p,int v){mem[p.GetH()][p.GetW()] = v;}

	void SetMemH(int h){mem_height = h;}
	int GetMemH(){return mem_height;}

	void SetMemW(int w){mem_width = w;}
	int GetMemW(){return mem_width;}

	int GetPass(){return PassMap;}

	void SetCrePos(int h,int w){CrePos.Set(h,w);}
	CPosition GetCrePos(){return CrePos;}

	CGene &GetGene(){return CreGen;}	//��������� ���retuen���Ǵ������ý�������д��"="?
	void SetGene(CGene gg){CreGen = gg;}

	bool GetToDes(){return getDestination;}

	void Move(CMazeMap &map,CGeneBank &gbank,int &num);

	void MoveTo(CMazeMap &map,CPosition p);


protected:
	int **mem,mem_height,mem_width;
	int PassMap;
	CPosition CrePos;
	CGene CreGen;

	bool getDestination;


	float innovateProbability;

};