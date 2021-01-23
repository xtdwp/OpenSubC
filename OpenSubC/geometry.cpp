#include "geometry.h"

unsigned opensubc::fuelRod::num = 0;
unsigned opensubc::gap::num = 0;
unsigned opensubc::channel::num = 0;

//给定坐标位置与半径进行初始化
opensubc::fuelRod::fuelRod(unsigned _id, double _x, double _y, double _r):id(_id), x(_x), y(_y),r(_r)
{
	++num;
}

opensubc::fuelRod::~fuelRod()
{

}


opensubc::gap::gap(unsigned rodId0, unsigned rodId1, unsigned channelId0, unsigned channelId1)
{
	id = num++;
	rodId[0] = rodId0;
	rodId[1] = rodId1;
	channelId[0] = channelId0;
	channelId[1] = channelId1;
}

opensubc::channel::channel(int numOfRods, std::vector<unsigned> rodIds)//给定一定数量燃料棒构建单通道
{

}

opensubc::channel::~channel()
{

}

void opensubc::initialize_geometry()
{

}

void opensubc::finalize_geometry()
{

}