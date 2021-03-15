#pragma once
#include <vector>

namespace opensubc {
	/**********ȼ�ϰ�����϶��ͨ������**********/
//ȼ�ϰ�
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

	//��϶
	class gap {
	private:
		static unsigned num;
	public:
		unsigned id;
		unsigned rodId[2];
		int channelId[2];
		double lk, sk;

		gap() = default;
		gap(unsigned rodId0, unsigned rodId1);
		int getOtherChannelId(unsigned _channelId);//��ȡgap���������ͨ��id֮�����ӵ���һ��ͨ����id
		static int checkGapExistence(unsigned rodId0, unsigned rodId1);
	};

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


	namespace geometry {
		extern double boundaryHeight, boundaryWidth;
		extern std::vector<fuelRod> rods;
		extern std::vector<gap> gaps;
		extern std::vector<channel> channels;
	}

	void initialize_geometry();
	//void read_geometry_xml();
	void finalize_geometry();
}
