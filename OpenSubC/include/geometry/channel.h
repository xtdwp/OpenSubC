#pragma once
#include <vector>

namespace opensubc {

	//依赖的类的声明
	enum class BoundaryType;

	//通道定义
	class channel
	{
	public:
		unsigned id;//id为该子通道在数组中的位置
		std::vector<unsigned> gapIds;//间隙id
		std::vector<unsigned> rodIds;//燃料棒id
		std::vector<double> circleLength;//各燃料棒在通道内的浸润周长
		std::vector<BoundaryType> boundaryTypes;//通道是否在边界的信息
		double A;//流体通道横截面积

		channel() = default;
		channel(unsigned _id, const std::vector<unsigned>& _rodIds, const std::vector<BoundaryType> _boundaryTypes);//给定一定数量燃料棒和边界信息构建单通道
		void setChannel();//在初始化之后，根据燃料棒位置分布构造子通道几何（目前只支持最简单的矩形情形）
		void setCornerChannel();//构建角通道
		void setEdgeChannel();//构建边通道
		void setOrdinaryChannel();//构建普通的通道
		~channel();
	};
}


