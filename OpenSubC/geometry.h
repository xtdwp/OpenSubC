#pragma once
#include <vector>

namespace opensubc {
	/**********燃料棒、间隙与通道定义**********/
//燃料棒
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

	//间隙
	class gap {
	private:
		static unsigned num;
	public:
		unsigned id;
		unsigned rodId[2];
		int channelId[2];
		double lk, sk;

		gap() = default;
		gap(unsigned rodId0, unsigned rodId1);
		int getOtherChannelId(unsigned _channelId);//获取gap除了输入的通道id之外连接的另一个通道的id
		static int checkGapExistence(unsigned rodId0, unsigned rodId1);
	};

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


	namespace geometry {
		extern double boundaryHeight, boundaryWidth;
		extern std::vector<fuelRod> rods;
		extern std::vector<gap> gaps;
		extern std::vector<channel> channels;
	}

	void initialize_geometry();
	//void read_geometry_xml();
	void finalize_geometry();
}
