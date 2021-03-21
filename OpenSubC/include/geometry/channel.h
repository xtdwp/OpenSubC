#pragma once
#include <vector>
#include <string>

namespace opensubc {

	//�������������
	class fuelRod;
	enum class BoundaryType;

	//ͨ������
	class channel
	{
	public:
		unsigned id;//idΪ����ͨ���������е�λ��
		unsigned originalId;//���뿨����ĸ�ͨ��id
		std::vector<unsigned> gapIds;//��϶id
		std::vector<unsigned> rodIds;//ȼ�ϰ�id
		std::vector<double> circleLength;//��ȼ�ϰ���ͨ���ڵĽ����ܳ�
		std::vector<BoundaryType> boundaryTypes;//ͨ���Ƿ��ڱ߽����Ϣ
		double x, y, A;//��ͨ�����ĵ������������

		channel() = default;
		channel(unsigned _id, const std::vector<unsigned>& _rodIds, const std::vector<BoundaryType> _boundaryTypes);//����һ������ȼ�ϰ��ͱ߽���Ϣ������ͨ��
		void setChannel();//�ڳ�ʼ��֮�󣬸���ȼ�ϰ�λ�÷ֲ�������ͨ�����Σ�Ŀǰֻ֧����򵥵ľ������Σ�
		void setCornerChannel();//������ͨ��
		void setEdgeChannel();//������ͨ��
		void setOrdinaryChannel();//������ͨ��ͨ��
		void addBoundaryGap();//Ϊ����ͨ�����һ��λ�ڱ߽��gap
		void addSingleRodGap(const fuelRod& rod, BoundaryType boundaryType);//Ϊ����ͨ�����һ����ȼ�ϰ���gap
		void addOrdinaryGap(const fuelRod& rod0, const fuelRod& rod1);//Ϊ����ͨ�����һ����ͨ������ȼ�ϰ�֮���gap
		void addExistsGap(unsigned gapId);//Ϊ����ͨ�����һ���Ѿ����ڵ�gap
		void setIdOfLastGap();//��gap�����е����һ��gap����id�����Ϣ�����һ��˵���ոմ���,��Ҫ����ʹ�ã�
		~channel();

		std::string toString()const;//�������ӡ��string
	};
}


