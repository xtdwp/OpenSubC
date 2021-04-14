#include <MassEquation.h>
#include <calculations.h>
#include <geometry.h>
#include <CrossMomentumEquation.h>


namespace opensubc {

	namespace calculation {
		extern Eigen::SparseMatrix<double> MassA;
		extern Eigen::SparseVector<double> MassB;//��������ϵ�������볣������
	}
}

void opensubc::initMassEquation()//��ʼ����������
{
	using namespace opensubc::calculation;
	//������������
	MassA.resize(numOfGapData, numOfGapData);
	MassB.resize(numOfGapData);
}
void opensubc::calculateMassMatrix()//������������ϵ�������볣������
{
	using namespace opensubc::calculation;
	using namespace opensubc::geometry;
	//��������������
	MassA.setZero();
	MassB.setZero();
	for (size_t i = 0; i < numOfChannelData; ++i)
	{
		if (!checkInletInterval(i))//�ж��ǲ����������������������������д
		{
			unsigned channelid = i / (numOfBlocks + (long long)1);//�õ���Ӧ��channelid
			MassA.insert(i, i) = 1;
			MassA.insert(i, i-1) = -1;
			double ew = 0;//��ʾeik*wkj�����
			for (auto& gapId : channels[channelid].gapIds)//������ͨ�����ӵ�����gap
			{
				unsigned connectedChannelId;
				if (gaps[gapId].getOtherChannelId(channelid, connectedChannelId))//�����gapȷʵ��������һ����ͨ��
				{
					unsigned gapindex = gapId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1);//��Ӧ��gap�Ŀ�������gap�����е�����
					ew += w.coeffRef(gapindex) * (channelid < connectedChannelId ? 1 : -1);
				}
			}
			MassB.insert(i) = -channels[channelid].A * length / numOfBlocks / tStep * (rho.coeffRef(i) - rhon.coeffRef(i)) - length / numOfBlocks * ew;
		}
	}


}