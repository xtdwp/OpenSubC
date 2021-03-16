#pragma once
#include <vector>

namespace opensubc {

	//�������������
	enum class BoundaryType;

	//ͨ������
	class channel
	{
	private:
		static unsigned num;
	public:
		unsigned id;
		std::vector<unsigned> gapIds;//��϶id
		std::vector<unsigned> rodIds;//ȼ�ϰ�id
		std::vector<double> circleLength;//��ȼ�ϰ���ͨ���ڵĽ����ܳ�
		std::vector<BoundaryType> boundaryTypes;//ͨ���Ƿ��ڱ߽����Ϣ
		double A;//����ͨ��������

		channel() = default;
		channel(unsigned _id, const std::vector<unsigned>& _rodIds);//����һ������ȼ�ϰ�������ͨ��
		void setChannel();//�ڳ�ʼ��֮�󣬸���ȼ�ϰ�λ�÷ֲ�������ͨ�����Σ�Ŀǰֻ֧����򵥵ľ������Σ�
		void setCornerChannel();//������ͨ��
		void setEdgeChannel();//������ͨ��
		void setOrdinaryChannel();//������ͨ��ͨ��
		~channel();
	};
}


