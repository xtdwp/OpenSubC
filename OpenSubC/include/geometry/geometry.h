#pragma once
#include <vector>
#include <fuelRod.h>
#include <gap.h>
#include <channel.h>

namespace opensubc {

	/**********类声明与定义**********/

	class fuelRod;//燃料棒的声明
	class gap;//间隙的声明
	class channel;//通道的声明
	enum class BoundaryType
	{
		NoBoundary, PositiveX, NegativeX, PositiveY, NegativeY
	};

	/**********变量声明**********/
	namespace geometry {
		extern double boundaryHeight, boundaryWidth;//最边缘燃料棒中心到边界的距离，y为Height，x为Width
		extern std::vector<fuelRod> rods;//存储所有燃料棒的数组
		extern std::vector<gap> gaps;//存储所有间隙的数组
		extern std::vector<channel> channels;//存储所有子通道的数组

		//输入卡定义了燃料棒与子通道的ID,但是程序内实际使用的ID为燃料棒或子通道在其数组中的序号
		//为方便两套ID系统转换,创建两个转换数组,转换数组存储顺序与燃料棒数组及子通道数组相同,存储内容为输入卡定义的ID
		extern std::vector<unsigned> rodIdConverter, channelIdConverter;
	}

	void initialize_geometry();
	//void read_geometry_xml();
	void finalize_geometry();

	bool checkGapExistence(const fuelRod& rod, BoundaryType boundaryType, unsigned& gapId);//查找该gap是否存在
	bool checkGapExistence(const fuelRod& rod0, const fuelRod& rod1, unsigned& gapId); //查找该gap是否存在

}
