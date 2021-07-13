#include <CrossMomentumEquation.h>
#include <calculations.h>
#include <geometry.h>
#include <AxialMomentumEquation.h>
#include <constants.h>

namespace opensubc {
	namespace calculation {
		 Eigen::SparseVector<double> v, vk, U, Uk; //�ֱ�Ϊvij��vkj��Uij��Ukj
		 Eigen::SparseMatrix<double> CrossMomentumA; //����������ϵ������ 
		 Eigen::VectorXd CrossMomentumB;//��������

		 Eigen::SparseMatrix<double> W4, W5, W4_inverse;
		 Eigen::VectorXd B2;
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

	W4.resize(numOfGapData, numOfGapData);
	W5.resize(numOfGapData, numOfChannelData);
	W4_inverse.resize(numOfGapData, numOfGapData);

	B2.resize(numOfGapData);
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
			if (m.coeffRef(i) > 0)
				v.insert(i) = 1 / rho.coeffRef(i);
			else
				v.insert(i) = 1 / rho.coeffRef(i + (long long)1);
			U.insert(i) = v.coeffRef(i) * m.coeffRef(i) / channels[i / (numOfBlocks + (long long)1)].A;

	}
	//����vkj��Ukj��ֵ
	for (size_t i = 0; i < numOfGapData; ++i)
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
				double coeffkj = length / numOfBlocks / tStep + KG * vk.coeffRef(row) * abs(w.coeffRef(row)) * length / numOfBlocks / gaps[gapid].l / gaps[gapid].s / 2;//wkj��ϵ��
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
				CrossMomentumB(row) = gaps[gapid].s / gaps[gapid].l * length / numOfBlocks * (Pk.coeffRef(row - (long long)1) + Pk.coeffRef(row)) / 2 + length / numOfBlocks / tStep * wn.coeffRef(row);
			}
			//�ñ߽�ͨ����w����0
			else
			{
				CrossMomentumA.insert(row, row) = 1;
				CrossMomentumB(row) = 0;
			}
		}
		//����ڴ���w����
		else
		{
			CrossMomentumA.insert(row, row) = 1;
			CrossMomentumB(row) = w.coeffRef(row);
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

void opensubc::calculateCrossMatrixs()//����W4, W5, W4_inverse��B2
{
	using namespace opensubc::calculation;
	using namespace opensubc::geometry;
	for (size_t row = 0; row < numOfGapData; ++row)//��ÿһ�н��б���
	{
		if (!checkInletInterval(row))//�ж��ǲ����������������������������д
		{
			unsigned gapid = row / (numOfBlocks + (long long)1);
			if (!checkBoundaryGap(gapid))//�ж��ǲ��Ǳ߽�ͨ������������������д
			{
				//�������������ϵ������
				double coeffkj = length / numOfBlocks / tStep + KG * vk.coeffRef(row) * abs(w.coeffRef(row)) * length / numOfBlocks / gaps[gapid].l / gaps[gapid].s / 2;//wkj��ϵ��
				if (Uk.coeffRef(row) > 0)
					coeffkj += Uk.coeffRef(row);
				else
					W4.insert(row, row + (long long)1) = Uk.coeffRef(row);
				if (Uk.coeffRef(row - (long long)1) < 0)
					coeffkj -= Uk.coeffRef(row - (long long)1);
				else
					W4.insert(row, row - (long long)1) = -Uk.coeffRef(row - (long long)1);
				W4.insert(row, row) = coeffkj;
				//���㳣������
				B2(row) = length / numOfBlocks / tStep * wn.coeffRef(row);

				unsigned j = row % (numOfBlocks + (long long)1);
				int e = (gaps[gapid].channelIds[0] < gaps[gapid].channelIds[1] ? 1 : -1);

				W5.insert(row, gaps[gapid].channelIds[0] * (numOfBlocks + (long long)1) + j - 1) = e * gaps[gapid].s / gaps[gapid].l * length / numOfBlocks;
				W5.insert(row, gaps[gapid].channelIds[1] * (numOfBlocks + (long long)1) + j - 1) = -e * gaps[gapid].s / gaps[gapid].l * length / numOfBlocks;
			}
			else
			{
				W4.insert(row, row) = 1;
			}
		}
		else
		{
			W4.insert(row, row) = 1;
		}
	}

	Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
	solver.compute(W4);
	Eigen::SparseMatrix<double> I(numOfGapData, numOfGapData);
	I.setIdentity();

	W4_inverse = solver.solve(I);
}