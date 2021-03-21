#pragma once
#include <vector>
#include <string>

namespace opensubc {

	//�������������
	enum class BoundaryType;
	class fuelRod;

	//��϶����
	class gap {
	public:
		unsigned id = 0;//idΪ�ü�϶�������е�λ��
		BoundaryType boundaryType;
		std::vector<unsigned> rodIds;
		std::vector<unsigned> channelIds;
		double lk = 0, sk = 0;//��϶���β���

		gap();//�߽��ϵļ�϶
		gap(const fuelRod& rod0, const fuelRod& rod1);//����ȼ�ϰ�������϶������ȼ�ϰ���
		gap(const fuelRod& rod, BoundaryType _boundaryType);//����ȼ�ϰ����϶��ȹ�����϶��һ��ȼ�ϰ���߽磩
		bool getOtherChannelId(unsigned channelId, unsigned& otherChannelId);//���ݸ�����ͨ��id������gap���ӵ���һ��ͨ��id

		std::string toString()const;//�������string
	};
}

