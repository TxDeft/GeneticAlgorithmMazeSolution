#pragma once
#include "common.h"

class CPosition
{
public:
	CPosition(){h=w=0;}
	CPosition(int nh,int nw){h=nh;w=nw;}
	bool operator==(CPosition &p){return (h==p.h && w==p.w);}
	bool operator!=(CPosition &p){return (h!=p.h || w!=p.w);}
	void operator=(const CPosition &p){h=p.h;w=p.w;}
	void Set(int nh,int nw){h=nh;w=nw;}
	int GetH(){return h;}
	int GetW(){return w;}

protected:   // �ϸ��ձ�����ͨ���ӿڷ��ʵ�ԭ�򣡹��ؾ�
	int h;
	int w;

};