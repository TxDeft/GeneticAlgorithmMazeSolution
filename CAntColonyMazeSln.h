#pragma once
#include "common.h"
#include "CBaseMazeSln.h"
#include "CAnt.h"

//******************************��������CAntColonyMazeSln��******************************
class CAntColonyMazeSln:public CBaseMazeSln
{
public:
	void ResetAll();

	bool Run(int refInterval=100);

	void Start(int refInterval);
	void Update();

	void GetInput();
	void SendOut();
	void SendIn();
	void PhDecrease();
	void KillAnt();
	void Draw();  //������ǰ��slnMap

protected:
	vector<CAnt> antsOutside;
	//queue<CAnt> antsInside;
	//queue<CAnt> antsBreedRoom;

	int round;
	int foodNum;
	int deadNum;
};


/*
���̣�
0����ʼ�����Ѽ��������е�����
1��ȷ��Ҫ���ѵ�������
2�������ǳ��ӣ���������գ��������������Χĳ��λ��
3������vector�е�ȫ������
4���ȴ���һ�γ���
5�������������ϣ����뽻�����ȴ���������������ֻ��ʱ�򡣸����������������ж���Ҫ����񣬲���Ҫ�Ļ������Ȼ�����ֻ��ӡ�
6��(queue�Ƕ�̬�ģ���Ⱥ�᲻�ϱ�󣬲�������)


������
������Ⱥ����    �������Ⱥ�������٣�ÿ�����ϴ�ʳ��ؼҵ�ʱ���ȴ���һֻ�ؼҵ����ϣ�Ȼ����߽����£�����һ����ӣ�
				�����ã��������棬����Ҫ�������ϵĹ�ѫֵʲô�ġ�ǿ�ȳ�ȥ���ܻ��Ż����ľ��Ǻ����ģ�
������Ⱥvector  �����Ѻͻ��ѵ�ʱ����vector��queue�任��

��Ⱥ��ʼ��С
��ǰ����������
ÿ�γ���ֻ��  ���������������
�����ϵļ��  ����������γ���ʱ���������һ�εļ����
��ǰ�غ�
ʳ���ռ���

*/