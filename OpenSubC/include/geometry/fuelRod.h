#pragma once
#include <vector>
#include <string>
#include <iostream>

namespace opensubc {
	class fuelRod {
	public:
		unsigned id;//idΪ��ȼ�ϰ��������е�λ��
		unsigned originalId;//���뿨����ĸ�ͨ��id
		double x, y;
		double r;
		std::vector<unsigned> channelIds;
		std::vector<unsigned> gapIds;

		fuelRod() = default;
		fuelRod(unsigned _id, double _x, double _y, double _r);//��������λ����뾶���г�ʼ��
		~fuelRod();

		std::string toString()const;//�������ӡ��string
	};
}


