#pragma once
#include <vector>

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
		extern std::vector<fuelRod> rods;
		extern std::vector<gap> gaps;
		extern std::vector<channel> channels;
	}

	void initialize_geometry();
	//void read_geometry_xml();
	void finalize_geometry();

	bool checkGapExistence(const fuelRod& rod, BoundaryType boundaryType, unsigned& gapId);//查找该gap是否存在
	bool checkGapExistence(const fuelRod& rod0, const fuelRod& rod1, unsigned& gapId); //查找该gap是否存在

}
