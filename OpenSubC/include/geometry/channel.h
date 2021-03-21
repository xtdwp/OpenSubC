#pragma once
#include <vector>
#include <string>

namespace opensubc {

	//依赖的类的声明
	class fuelRod;
	enum class BoundaryType;

	//通道定义
	class channel
	{
	public:
		unsigned id;//id为该子通道在数组中的位置
		unsigned originalId;//输入卡定义的该通道id
		std::vector<unsigned> gapIds;//间隙id
		std::vector<unsigned> rodIds;//燃料棒id
		std::vector<double> circleLength;//各燃料棒在通道内的浸润周长
		std::vector<BoundaryType> boundaryTypes;//通道是否在边界的信息
		double x, y, A;//子通道中心点坐标与横截面积

		channel() = default;
		channel(unsigned _id, const std::vector<unsigned>& _rodIds, const std::vector<BoundaryType> _boundaryTypes);//给定一定数量燃料棒和边界信息构建单通道
		void setChannel();//在初始化之后，根据燃料棒位置分布构造子通道几何（目前只支持最简单的矩形情形）
		void setCornerChannel();//构建角通道
		void setEdgeChannel();//构建边通道
		void setOrdinaryChannel();//构建普通的通道
		void addBoundaryGap();//为该子通道添加一个位于边界的gap
		void addSingleRodGap(const fuelRod& rod, BoundaryType boundaryType);//为该子通道添加一个单燃料棒的gap
		void addOrdinaryGap(const fuelRod& rod0, const fuelRod& rod1);//为该子通道添加一个普通的两个燃料棒之间的gap
		void addExistsGap(unsigned gapId);//为该子通道添加一个已经存在的gap
		void setIdOfLastGap();//给gap数组中的最后一个gap增加id相关信息（最后一个说明刚刚创建,需要配套使用）
		~channel();

		std::string toString()const;//将对象打印成string
	};
}


