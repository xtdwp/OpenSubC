#pragma once
#include <vector>

namespace opensubc {
	class fuelRod;
	class gap;
	class channel;

	namespace geometry {
		double boundaryHeight, boundaryWidth;
		std::vector<fuelRod> rods;
		std::vector<gap> gaps;
		std::vector<channel> channels;
	}

	void initialize_geometry();
	//void read_geometry_xml();
	void finalize_geometry();
}

/**********ȼ�ϰ�����϶��ͨ������**********/
//ȼ�ϰ�
class opensubc::fuelRod {
private:
	static unsigned num;
public:
	unsigned id;
	double x, y;
	double r;
	std::vector<unsigned> channelIds;

	fuelRod() = default;
	fuelRod(unsigned _id, double _x, double _y, double _r);//��������λ����뾶���г�ʼ��
	~fuelRod();
};

//��϶
class opensubc::gap {
private:
	static unsigned num;
public:
	unsigned id;
	unsigned rodId[2];
	unsigned channelId[2];

	gap() = default;
	gap(unsigned rodId0, unsigned rodId1, unsigned channelId0, unsigned channelId1);
};

//ͨ��
class opensubc::channel
{
private:
	static unsigned num;
public:
	unsigned id;
	std::vector<unsigned> gapIds;
	std::vector<unsigned> rodIds;
	std::vector<unsigned> circleLength;//��ȼ�ϰ���ͨ���ڵĽ����ܳ�

	channel() = default;
	channel(int numOfRods, std::vector<unsigned> rodIds);//����һ������ȼ�ϰ�������ͨ��
	~channel();
};

