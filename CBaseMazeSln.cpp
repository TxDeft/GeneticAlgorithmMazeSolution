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
		slnMap=map;  //CMazeMap里面已经写了真拷贝的重载
		return true;
	}
}

