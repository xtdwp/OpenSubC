#pragma once

namespace opensubc {
	class fuelRod;
	class gap;
	class channel;
}

/**********燃料棒、间隙与通道定义**********/
//燃料棒
class opensubc::fuelRod {
private:
	static int num;
public:
	unsigned id;
	double x, y;
	double r;
	unsigned* channelIds;

	fuelRod() = default;
	fuelRod(unsigned _id, double _x, double _y, double _r);//给定坐标位置与半径进行初始化
	~fuelRod();
};

//间隙
class opensubc::gap {
private:
	static int num;
public:
	unsigned id;
	unsigned rodId[2];
	unsigned channelId[2];

	gap() = default;
	gap(unsigned rodId0, unsigned rodId2, unsigned channelId0, unsigned channelId1);
};

//通道
class opensubc::channel
{
private:
	static int num;
public:
	unsigned id;
	unsigned* gapIds;
	unsigned* rodIds;
	double* circleLength;//各燃料棒在通道内的浸润周长

	channel() = default;
	channel(int numOfRods, unsigned* rodIds);//给定一定数量燃料棒构建单通道
	~channel();
};

