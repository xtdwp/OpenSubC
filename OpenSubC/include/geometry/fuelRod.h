#pragma once
#include <vector>
#include <string>
#include <iostream>

namespace opensubc {
	class fuelRod {
	public:
		unsigned id;//id为该燃料棒在数组中的位置
		unsigned originalId;//输入卡定义的该通道id
		double x, y;
		double r;
		std::vector<unsigned> channelIds;
		std::vector<unsigned> gapIds;

		fuelRod() = default;
		fuelRod(unsigned _id, double _x, double _y, double _r);//给定坐标位置与半径进行初始化
		~fuelRod();

		std::string toString()const;//将对象打印成string
	};
}


