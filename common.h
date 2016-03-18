#pragma once

/*
==========坐标系规定==========
左上角0,0
【纵轴为h;】
【横轴为w;】
都是h优 先然后是w
*/


const int REFRESH_INTERVAL=500;

//-------------------生物相关----------------------
const float INNOVATE_PROBABILITY=0.2f;


//==========地图==========

const int MAPVALUE_WALL = '1';
const int MAPVALUE_WAY = ' ';
const int MAPVALUE_IN = 'i';
const int MAPVALUE_OUT = 'o';

const int MAPVALUE_INPATH = 'p';
const int MAPVALUE_USED = 'u';



//#include "graphics.h"
#include <ctime>
#include <fstream>
#include <conio.h>
#include <vector>
#include <windows.h>
#include <iostream>



