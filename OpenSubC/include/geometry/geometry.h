#pragma once
#include <vector>
#include <fuelRod.h>
#include <gap.h>
#include <channel.h>

namespace opensubc {
	/**********燃料棒、间隙与通道定义**********/

	class fuelRod;//燃料棒的声明
	class gap;//间隙的声明
	class channel;//通道的声明


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
