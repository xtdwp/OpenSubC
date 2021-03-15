#pragma once
#include <vector>
#include <fuelRod.h>
#include <gap.h>
#include <channel.h>

namespace opensubc {
	/**********ȼ�ϰ�����϶��ͨ������**********/

	class fuelRod;//ȼ�ϰ�������
	class gap;//��϶������
	class channel;//ͨ��������


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
