#pragma once
#include <vector>

namespace opensubc {
	class fuelRod {
	private:
		static unsigned num;
	public:
		unsigned id;
		double x, y;
		double r;
		std::vector<unsigned> channelIds;
		std::vector<unsigned> gapIds;

		fuelRod() = default;
		fuelRod(unsigned _id, double _x, double _y, double _r);//给定坐标位置与半径进行初始化
		~fuelRod();
	};
}


