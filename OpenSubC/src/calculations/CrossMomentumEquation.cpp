#include <CrossMomentumEquation.h>
#include <calculations.h>
#include <geometry.h>
#include <AxialMomentumEquation.h>

namespace opensubc {
	namespace calculation {
		 Eigen::SparseVector<double> v, vk, U, Uk; //�ֱ�Ϊvij��vkj��Uij��Ukj
		 Eigen::SparseMatrix<double> CrossMomentumA; //����������ϵ������ 
		 Eigen::SparseVector<double> CrossMomentumB;//��������
	}
}

void opensubc::initCrossMomentumEquation()//��ʼ������������
{
	using namespace opensubc::calculation;
	//������������
	v.resize(numOfChannelData);
	U.resize(numOfChannelData);
	vk.resize(numOfGapData);
	Uk.resize(numOfGapData);
	CrossMomentumA.resize(numOfGapData, numOfGapData);
	CrossMomentumB.resize(numOfGapData);
}
void opensubc::calculateCrossMomentumVectors()//����vij��vkj��Uij��Ukj������
{
	using namespace opensubc::calculation;
	using namespace opensubc::geometry;
	//��������������
	v.setZero();
	U.setZero();
	vk.setZero();
	Uk.setZero();
	//����vij��Uij��ֵ�������еĳ���������mij�������0��
	for (size_t i = 0; i < numOfChannelData; ++i)
	{
		if (!checkInletInterval(i))//�ж��ǲ����������������������������д
		{
			if (m.coeffRef(i) > 0)
				v.insert(i) = 1 / rho.coeffRef(i);
			else
				v.insert(i) = 1 / rho.coeffRef(i + (long long)1);
			U.insert(i) = v.coeffRef(i) * m.coeffRef(i) / channels[i / (numOfBlocks + (long long)1)].A;
		}
	}
	//����vkj��Ukj��ֵ
	for (size_t i = 0; i < numOfGapData; ++i)
	{
		if (!checkInletInterval(i))//�ж��ǲ����������������������������д
		{
			unsigned gapid = i / (numOfBlocks + (long long)1);//�õ���Ӧ��gapid
			if (!checkBoundaryGap(gapid))//�ж��ǲ��Ǳ߽�ͨ������������������д
			{
				double vk_value = 0;
				double Uk_value = 0;
				for (auto& channelId : gaps[gapid].channelIds)//����gap�����ӵ����е�channel���
				{
					vk_value += v.coeffRef(channelId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1));//�����ӵ�����channel�Ķ�Ӧ��vij���
					Uk_value += U.coeffRef(channelId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1));//�����ӵ�����channel�Ķ�Ӧ��Uij���
				}
				vk_value /= gaps[gapid].channelIds.size();//ȡƽ��
				Uk_value /= gaps[gapid].channelIds.size();//ȡƽ��
				vk.insert(i) = vk_value;
				Uk.insert(i) = Uk_value;
			}
		}
	}	
}
void opensubc::calculateCrossMomentumMatrix()//�������������ϵ�������볣������
{
	using namespace opensubc::calculation;
	using namespace opensubc::geometry;
	CrossMomentumA.setZero();//����
	CrossMomentumB.setZero();
	for (size_t row = 0; row < numOfGapData; ++row)//��ÿһ�н��б���
	{
		if (!checkInletInterval(row))//�ж��ǲ����������������������������д
		{
			unsigned gapid = row / (numOfBlocks + (long long)1);
			if (!checkBoundaryGap(gapid))//�ж��ǲ��Ǳ߽�ͨ������������������д
			{    
				//�������������ϵ������
				double coeffkj = length / numOfBlocks / tStep + KG * vk.coeffRef(row) * w.coeffRef(row) * length / numOfBlocks / gaps[gapid].l / gaps[gapid].s / 2;//wkj��ϵ��
				if (Uk.coeffRef(row) > 0)
					coeffkj += Uk.coeffRef(row);
				else
					CrossMomentumA.insert(row, row +(long long) 1) = Uk.coeffRef(row);
				if (Uk.coeffRef(row - (long long)1) < 0)
					coeffkj -= Uk.coeffRef(row - (long long)1);
				else
					CrossMomentumA.insert(row, row - (long long)1) = -Uk.coeffRef(row - (long long)1);
				CrossMomentumA.insert(row, row) = coeffkj;
				//���㳣������
				CrossMomentumB.insert(row) = gaps[gapid].l / gaps[gapid].s * length / numOfBlocks * Pk.coeffRef(row - (long long)1) + length / numOfBlocks / tStep * wn.coeffRef(row);

			}
		}
	}

}
bool opensubc::checkInletInterval(int i)//�ж��ǲ�����ڴ����񣬣����򷵻���
{
	using namespace opensubc::calculation;
	if (i % (numOfBlocks + (long long)1) == 0)
	{
		return true;
	}
	else
		return false;
}
bool opensubc::checkBoundaryGap(int gapid)//�ж��ǲ���boundarygap�����򷵻���
{
	using namespace opensubc::geometry;
	if (gaps[gapid].channelIds.size() < 2)
	{
		return true;
	}
	else
		return false;
}