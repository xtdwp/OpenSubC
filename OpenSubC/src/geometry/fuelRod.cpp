#include "fuelRod.h"

unsigned opensubc::fuelRod::num = 0;

//��������λ����뾶���г�ʼ��
opensubc::fuelRod::fuelRod(unsigned _id, double _x, double _y, double _r) :id(_id), x(_x), y(_y), r(_r)
{
	++num;
}

opensubc::fuelRod::~fuelRod()
{

}