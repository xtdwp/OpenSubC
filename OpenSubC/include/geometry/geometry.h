#pragma once
#include <vector>
#include <fuelRod.h>
#include <gap.h>
#include <channel.h>

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
		extern std::vector<fuelRod> rods;//�洢����ȼ�ϰ�������
		extern std::vector<gap> gaps;//�洢���м�϶������
		extern std::vector<channel> channels;//�洢������ͨ��������

		//���뿨������ȼ�ϰ�����ͨ����ID,���ǳ�����ʵ��ʹ�õ�IDΪȼ�ϰ�����ͨ�����������е����
		//Ϊ��������IDϵͳת��,��������ת������,ת������洢˳����ȼ�ϰ����鼰��ͨ��������ͬ,�洢����Ϊ���뿨�����ID
		extern std::vector<unsigned> rodIdConverter, channelIdConverter;
	}

	void initialize_geometry();
	//void read_geometry_xml();
	void finalize_geometry();

	bool checkGapExistence(const fuelRod& rod, BoundaryType boundaryType, unsigned& gapId);//���Ҹ�gap�Ƿ����
	bool checkGapExistence(const fuelRod& rod0, const fuelRod& rod1, unsigned& gapId); //���Ҹ�gap�Ƿ����

}
