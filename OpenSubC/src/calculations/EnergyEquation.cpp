#include <EnergyEquation.h>
#include <calculations.h>
#include <geometry.h>
#include <CrossMomentumEquation.h>
#include "constants.h"


namespace opensubc {

	namespace calculation {
		 Eigen::SparseMatrix<double> EnergyA;
		 Eigen::VectorXd EnergyB;//��������ϵ�������볣������
	}
}

void opensubc::initEnergyEquation()//��ʼ����������
{
	using namespace opensubc::calculation;
	//������������
	EnergyA.resize(numOfChannelData, numOfChannelData);
	EnergyB.resize(numOfChannelData);
}
void opensubc::calculateEnergyMatrix()//������������ϵ�������볣������
{
	using namespace opensubc::calculation;
	using namespace opensubc::geometry;
	//��������������
	EnergyA.setZero();
	EnergyB.setZero();
	for (size_t i = 0; i < numOfChannelData; ++i)
	{
		if (!checkInletInterval(i))//�ж��ǲ����������������������������д
		{
			unsigned channelid = i / (numOfBlocks + (long long)1);//�õ���Ӧ��channelid
			double Ck = 0;//��ʾ�Խ���Ԫ�������϶��ص������
			double ew = 0;//��ʾeik*wkj�����
			double Ct = 0;//��ʾ��������������ͨ�����ȴ�����
			for (auto& gapId : channels[channelid].gapIds)//������ͨ�����ӵ�����gap
			{
				unsigned connectedChannelId;
				if (gaps[gapId].getOtherChannelId(channelid, connectedChannelId))//�����gapȷʵ��������һ����ͨ��
				{
					unsigned gapindex = gapId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1);//��Ӧ��gap�Ŀ�������gap�����е�����
					ew = w.coeffRef(gapindex) * (channelid < connectedChannelId ? 1 : -1);
					Ck += wTurbulence.coeffRef(gapindex) - (ew > 0 ? 0 : 1) * ew;
					/*std::cout << wTurbulence.coeffRef(gapindex) << std::endl;*/
					EnergyA.insert(i, (connectedChannelId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1))) = -(length / numOfBlocks) * (wTurbulence.coeffRef(gapindex) - (ew > 0 ? 0 : 1) * ew);
					Ct -= (length / numOfBlocks) * GT * (k.coeffRef(i) + k.coeffRef(connectedChannelId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1))) / 2 / gaps[gapId].l * gaps[gapId].s * (T.coeffRef(i) - T.coeffRef(connectedChannelId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1)));
				}
			}
			EnergyA.insert(i, i) = channels[channelid].A * (length / numOfBlocks) / tStep * rhon.coeffRef(i) - (m.coeffRef(i) > 0 ? 0 : 1) * m.coeffRef(i) + (m.coeffRef(i - 1) > 0 ? 1 : 0) * m.coeffRef(i - 1) + (length / numOfBlocks) * Ck;
			if (i < numOfChannelData - (long long)1)
				EnergyA.insert(i, i + 1) = (m.coeffRef(i) > 0 ? 0 : 1) * m.coeffRef(i);
			EnergyA.insert(i, i - 1) = -(m.coeffRef(i - 1) > 0 ? 1 : 0) * m.coeffRef(i - 1);
			double Cq = 0;//��ʾȼ�ϰ����������
			for (auto& rodId : channels[channelid].rodIds)//������ͨ�����ӵ�����rod
			{
				Cq += (length / numOfBlocks) * rods[rodId].r * PI / 2 * q.coeffRef(rodId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1)) * (1 + rQ);
			}
			EnergyB(i) = channels[channelid].A * (length / numOfBlocks) / tStep * rhon.coeffRef(i) * hn.coeffRef(i) + Cq + Ct;
			/*std::cout << "q  " << Cq << "  " << Ct << std::endl;
			system("pause");*/
		}
		//����ڴ���h����
		else
		{
			EnergyA.insert(i, i) = 1;
			EnergyB(i) = h.coeffRef(i);
		}
	}
}
