#pragma once
#include <vector>

namespace opensubc {

	//�������������
	enum class BoundaryType;

	//ͨ������
	class channel
	{
	public:
		unsigned id;//idΪ����ͨ���������е�λ��
		std::vector<unsigned> gapIds;//��϶id
		std::vector<unsigned> rodIds;//ȼ�ϰ�id
		std::vector<double> circleLength;//��ȼ�ϰ���ͨ���ڵĽ����ܳ�
		std::vector<BoundaryType> boundaryTypes;//ͨ���Ƿ��ڱ߽����Ϣ
		double A;//����ͨ��������

		channel() = default;
		channel(unsigned _id, const std::vector<unsigned>& _rodIds, const std::vector<BoundaryType> _boundaryTypes);//����һ������ȼ�ϰ��ͱ߽���Ϣ������ͨ��
		void setChannel();//�ڳ�ʼ��֮�󣬸���ȼ�ϰ�λ�÷ֲ�������ͨ�����Σ�Ŀǰֻ֧����򵥵ľ������Σ�
		void setCornerChannel();//������ͨ��
		void setEdgeChannel();//������ͨ��
		void setOrdinaryChannel();//������ͨ��ͨ��
		~channel();
	};
}


