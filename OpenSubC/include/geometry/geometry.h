#pragma once
#include <vector>

namespace opensubc {

	/**********�������붨��**********/

	class fuelRod;//ȼ�ϰ�������
	class gap;//��϶������
	class channel;//ͨ��������
	enum class BoundaryType
	{
		NoBoundary, PositiveX, NegativeX, PositiveY, NegativeY
	};

	/**********��������**********/
	namespace geometry {
		extern double boundaryHeight, boundaryWidth;//���Եȼ�ϰ����ĵ��߽�ľ��룬yΪHeight��xΪWidth
		extern std::vector<fuelRod> rods;
		extern std::vector<gap> gaps;
		extern std::vector<channel> channels;
	}

	void initialize_geometry();
	//void read_geometry_xml();
	void finalize_geometry();

	bool checkGapExistence(const fuelRod& rod, BoundaryType boundaryType, unsigned& gapId);//���Ҹ�gap�Ƿ����
	bool checkGapExistence(const fuelRod& rod0, const fuelRod& rod1, unsigned& gapId); //���Ҹ�gap�Ƿ����

}
