#pragma once
#include <vector>

namespace opensubc {

	//�������������
	enum class BoundaryType;
	class fuelRod;

	//��϶����
	class gap {
	private:
		static unsigned num;
	public:
		unsigned id;
		BoundaryType boundaryType;
		std::vector<unsigned> rodIds;
		std::vector<unsigned> channelIds;
		double lk = 0, sk = 0;//��϶���β���

		gap();//�߽��ϵļ�϶
		gap(const fuelRod& rod0, const fuelRod& rod1);//����ȼ�ϰ�������϶������ȼ�ϰ���
		gap(const fuelRod& rod, BoundaryType _boundaryType);//����ȼ�ϰ����϶��ȹ�����϶��һ��ȼ�ϰ���߽磩
		unsigned getOtherChannelId(unsigned channelId);//���ݸ�����ͨ��id������gap���ӵ���һ��ͨ��id
	};
}

