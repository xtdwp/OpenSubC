#pragma once

namespace opensubc {
	class fuelRod;
	class gap;
	class channel;
}

/**********ȼ�ϰ�����϶��ͨ������**********/
//ȼ�ϰ�
class opensubc::fuelRod {
private:
	static int num;
public:
	unsigned id;
	double x, y;
	double r;
	unsigned* channelIds;

	fuelRod() = default;
	fuelRod(unsigned _id, double _x, double _y, double _r);//��������λ����뾶���г�ʼ��
	~fuelRod();
};

//��϶
class opensubc::gap {
private:
	static int num;
public:
	unsigned id;
	unsigned rodId[2];
	unsigned channelId[2];

	gap() = default;
	gap(unsigned rodId0, unsigned rodId2, unsigned channelId0, unsigned channelId1);
};

//ͨ��
class opensubc::channel
{
private:
	static int num;
public:
	unsigned id;
	unsigned* gapIds;
	unsigned* rodIds;
	double* circleLength;//��ȼ�ϰ���ͨ���ڵĽ����ܳ�

	channel() = default;
	channel(int numOfRods, unsigned* rodIds);//����һ������ȼ�ϰ�������ͨ��
	~channel();
};

