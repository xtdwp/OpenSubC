#pragma once
#include <vector>
#include <string>

namespace opensubc {

	//依赖的类的声明
	enum class BoundaryType;
	class fuelRod;

	//间隙定义
	class gap {
	public:
		unsigned id = 0;//id为该间隙在数组中的位置
		BoundaryType boundaryType;
		std::vector<unsigned> rodIds;
		std::vector<unsigned> channelIds;
		double lk = 0, sk = 0;//间隙几何参数

		gap();//边界上的间隙
		gap(const fuelRod& rod0, const fuelRod& rod1);//根据燃料棒构建间隙（两个燃料棒）
		gap(const fuelRod& rod, BoundaryType _boundaryType);//根据燃料棒与间隙宽度构建间隙（一个燃料棒与边界）
		bool getOtherChannelId(unsigned channelId, unsigned& otherChannelId);//根据给定的通道id给出该gap连接的另一个通道id

		std::string toString()const;//将对象成string
	};
}

