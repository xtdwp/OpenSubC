#include "fuelRod.h"

unsigned opensubc::fuelRod::num = 0;

//给定坐标位置与半径进行初始化
opensubc::fuelRod::fuelRod(unsigned _id, double _x, double _y, double _r) :id(_id), x(_x), y(_y), r(_r)
{
	++num;
}

opensubc::fuelRod::~fuelRod()
{

}