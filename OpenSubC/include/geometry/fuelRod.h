#pragma once
#include <vector>

namespace opensubc {
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
		fuelRod(unsigned _id, double _x, double _y, double _r);//��������λ����뾶���г�ʼ��
		~fuelRod();
	};
}


