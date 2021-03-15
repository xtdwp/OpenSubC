#pragma once
#include <vector>

namespace opensubc {
	//ͨ��
	class channel
	{
	private:
		static unsigned num;
	public:
		unsigned id;
		std::vector<unsigned> gapIds;
		std::vector<unsigned> rodIds;
		std::vector<double> circleLength;//��ȼ�ϰ���ͨ���ڵĽ����ܳ�
		double A;//����ͨ��������

		channel() = default;
		channel(unsigned _id, const std::vector<unsigned>& _rodIds);//����һ������ȼ�ϰ�������ͨ��
		void setChannel();//�ڳ�ʼ��֮�󣬸���ȼ�ϰ�λ�÷ֲ�������ͨ�����Σ�Ŀǰֻ֧����򵥵ľ������Σ�
		~channel();
	};
}


