#include "CBaseMazeSln.h"

bool CBaseMazeSln::HasMap()
{
	return !slnMap.IsEmpty();
}

bool CBaseMazeSln::SetMap(CMazeMap& map)
{
	if(map.IsEmpty()) return false; 
	else
	{
		ResetAll();
		slnMap=map;  //CMazeMap�����Ѿ�д���濽��������
		return true;
	}
}

