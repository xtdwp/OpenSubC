#pragma once
#include <vector>

namespace opensubc {
	//通道
	class channel
	{
	private:
		static unsigned num;
	public:
		unsigned id;
		std::vector<unsigned> gapIds;
		std::vector<unsigned> rodIds;
		std::vector<double> circleLength;//各燃料棒在通道内的浸润周长
		double A;//流体通道横截面积

		channel() = default;
		channel(unsigned _id, const std::vector<unsigned>& _rodIds);//给定一定数量燃料棒构建单通道
		void setChannel();//在初始化之后，根据燃料棒位置分布构造子通道几何（目前只支持最简单的矩形情形）
		~channel();
	};
}


