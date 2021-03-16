#pragma once
#include <vector>

namespace opensubc {

	//依赖的类的声明
	enum class BoundaryType;
	class fuelRod;

	//间隙定义
	class gap {
	private:
		static unsigned num;
	public:
		unsigned id;
		BoundaryType boundaryType;
		std::vector<unsigned> rodIds;
		std::vector<unsigned> channelIds;
		double lk = 0, sk = 0;//间隙几何参数

		gap();//边界上的间隙
		gap(const fuelRod& rod0, const fuelRod& rod1);//根据燃料棒构建间隙（两个燃料棒）
		gap(const fuelRod& rod, BoundaryType _boundaryType);//根据燃料棒与间隙宽度构建间隙（一个燃料棒与边界）
		unsigned getOtherChannelId(unsigned channelId);//根据给定的通道id给出该gap连接的另一个通道id
	};
}

