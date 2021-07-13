#include <calculations.h>
#include <geometry.h>
#include <AxialMomentumEquation.h>
#include <CrossMomentumEquation.h>
#include <constants.h>

namespace opensubc {

	namespace calculation {
		Eigen::VectorXd DPx, R, Uk1, Pk; //�ֱ�Ϊѹ���ݶ�dP/dX������Rij������Ukj������ͨ�����ѹ��Pkj
		Eigen::SparseMatrix<double> W1, W2, W3;
		Eigen::VectorXd B1;
	}
}

void opensubc::initAxialMomentumEquation()//��ʼ������������
{
	using namespace opensubc::calculation;
	//������������
	DPx.resize(numOfChannelData);
	R.resize(numOfChannelData);
	Uk1.resize(numOfGapData);
	Pk.resize(numOfGapData);	
	Pk.setZero();

	W1.resize(numOfChannelData, numOfChannelData);
	W2.resize(numOfChannelData, numOfGapData);
	W3.resize(numOfChannelData, numOfGapData);

	B1.resize(numOfChannelData);
}
void opensubc::calculateAxialMomentumVectors()//����Rij��Ukj������
{
	using namespace opensubc::calculation;
	using namespace opensubc::geometry;
	//��������������
	R.setZero();
	Uk1.setZero();
	//����Rij����
	for (int i = 0; i < numOfChannelData; ++i)
	{
		if (!checkInletInterval(i))//�ж��ǲ����������������������������д
		{
			unsigned channelid = i / (numOfBlocks + (long long)1);//�õ���Ӧ��channelid
			R(i) = ((f.coeffRef(i) / rho.coeffRef(i) / channels[channelid].Dh +K.coeffRef(i)*v.coeffRef(i)/ (length / numOfBlocks)) / 2+(v.coeffRef(i)- v.coeffRef(i-(long long)1))/ (length / numOfBlocks)) / pow(channels[channelid].A, 2);
			/*std::cout << "R  " << R(i) << "  " << f.coeffRef(i) << "  " << rho.coeffRef(i) << "  " << K.coeffRef(i) << "  " << v.coeffRef(i) << std::endl;
			system("pause");*/
		}
	}
	//����Ukj����
	for (int i = 0; i < numOfGapData; ++i)
	{
		if (!checkInletInterval(i))//�ж��ǲ����������������������������д
		{
			unsigned gapid = i / (numOfBlocks + (long long)1);//�õ���Ӧ��gapid
			if (!checkBoundaryGap(gapid))//�ж��ǲ��Ǳ߽�ͨ������������������д
			{
				if (w.coeffRef(i) > 0)//�ж�wkj�Ĵ�С�������㣬��ȡСͨ����channelIds[0]����Uij������ȡ��ͨ����Uij
					Uk1(i) = U.coeffRef(gaps[gapid].channelIds[0] * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1));
				else
					Uk1(i) = U.coeffRef(gaps[gapid].channelIds[1] * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1));
			}
		}
	}
	
}
void opensubc::calculateAxialMomentumEquation()//����ѹ���ݶȺ�����ͨ����ѹ��
{
	using namespace opensubc::calculation;
	using namespace opensubc::geometry;
	DPx.setZero();
	Pk.setZero();
	//����ѹ���ݶ�DPx����
	for (int i = 0; i < numOfChannelData; ++i)
	{
		if (!checkInletInterval(i))//�ж��ǲ����������������������������д
		{
			unsigned channelid = i / (numOfBlocks + (long long)1);//�õ���Ӧ��channelid
			double Ck0=0, Ck1=0;//�ֱ��ʾ����϶�йص��������
			double A = channels[channelid].A;
			for (auto& gapId : channels[channelid].gapIds)//������ͨ�����ӵ�����gap
			{
				unsigned connectedChannelId;
				if (gaps[gapId].getOtherChannelId(channelid, connectedChannelId))//�����gapȷʵ��������һ����ͨ��
				{
					unsigned gapindex = gapId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1);//��Ӧ��gap�Ŀ�������gap�����е�����
					Ck0 += (w.coeffRef(gapindex) * (channelid < connectedChannelId ? 1 : -1) * Uk1.coeffRef(gapindex) + fT * wTurbulence.coeffRef(gapindex) * (U.coeffRef(i) - U.coeffRef(connectedChannelId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1))))/ A;
					Ck1 += (w.coeffRef(gapindex) * (channelid < connectedChannelId ? 1 : -1)) / A;
				}
			}
			DPx(i) = (-R.coeffRef(i)) * pow(m.coeffRef(i - (long long)1), 2) - g * rho.coeffRef(i) * cos(theta * PI / 180) + (mn.coeffRef(i) - m.coeffRef(i - (long long)1)) / A / tStep - Ck0 + (2*U.coeffRef(i) + (length / numOfBlocks) / tStep + R.coeffRef(i) * A * (length / numOfBlocks) * (m.coeffRef(i) + m.coeffRef(i - (long long)1))) * ((rho.coeffRef(i) - rhon.coeffRef(i)) / tStep + Ck1);
			//std::cout << DPx(i) << std::endl;
			//system("pause");
		}
	}
	//��������ͨ����ѹ��Pkj����
	for (int i = numOfGapData - 1; i >= 0; --i)
	{
		unsigned gapid = i / (numOfBlocks + (long long)1);//�õ���Ӧ��gapid
		if (!checkBoundaryGap(gapid))//�ж��ǲ��Ǳ߽�ͨ������������������д
		{
			//if (checkInletInterval(i))//�ж��ǲ��������������,����ǣ�����Ϊ����ͨ��ѹ��Ϊ0
			//{
			//	Pk(i) = 0;
			//}
			if (i % (numOfBlocks + (long long)1) == numOfBlocks)//�ж��ǲ��ǳ�������,����ǣ�����Ϊ����ͨ��ѹ��Ϊ0
			{
				Pk(i) = 0;
			}
			else
			{
				Pk(i) = Pk.coeffRef(i + (long long)1) - (length / numOfBlocks) * (DPx.coeffRef(gaps[gapid].channelIds[0] * (numOfBlocks + (long long)1) + (i + 1) % (numOfBlocks + (long long)1)) - DPx.coeffRef(gaps[gapid].channelIds[1] * (numOfBlocks + (long long)1) + (i + 1) % (numOfBlocks + (long long)1)))*(gaps[gapid].channelIds[0] < gaps[gapid].channelIds[1] ? 1 : -1);
			}
		}
		
	}
	
}

void opensubc::calculateAxialMatrixs()//����W1, W2, W3��B1
{
	using namespace opensubc::calculation;
	using namespace opensubc::geometry;
	for (int row = 0; row < numOfChannelData; ++row)
	{
		if (row % (numOfBlocks + (long long)1) != numOfBlocks)//������ǳ��ڴ�
		{
			W1.insert(row, row + (long long)1) = numOfBlocks / length;
			W1.insert(row, row) = -numOfBlocks / length;

			double Ck0, Ck1;
			unsigned channelid = row / (numOfBlocks + (long long)1);//�õ���Ӧ��channelid
			double A = channels[channelid].A;
			Ck1 = (2 * U.coeffRef(row + (long long)1) + (length / numOfBlocks) / tStep + R.coeffRef(row + (long long)1) * A * (length / numOfBlocks) * (m.coeffRef(row + (long long)1) + m.coeffRef(row)));
			for (auto& gapId : channels[channelid].gapIds)//������ͨ�����ӵ�����gap
			{
				unsigned connectedChannelId;
				if (gaps[gapId].getOtherChannelId(channelid, connectedChannelId))//�����gapȷʵ��������һ����ͨ��
				{
					unsigned gapindex = gapId * (numOfBlocks + (long long)1) + (row + (long long)1) % (numOfBlocks + (long long)1);//��Ӧ��gap�Ŀ�������gap�����е�����
					W2.insert(row, gapindex) = -w.coeffRef(gapindex) * (channelid < connectedChannelId ? 1 : -1) * Uk1.coeffRef(gapindex) / A;
					W3.insert(row, gapindex) = Ck1 * (w.coeffRef(gapindex) * (channelid < connectedChannelId ? 1 : -1)) / A;
					Ck0 += fT * wTurbulence.coeffRef(gapindex) * (U.coeffRef(row + (long long)1) - U.coeffRef(connectedChannelId * (numOfBlocks + (long long)1) + (row + 1) % (numOfBlocks + (long long)1))) / A;
				}
			}
			B1(row) = (-R.coeffRef(row + (long long)1)) * pow(m.coeffRef(row), 2) - g * rho.coeffRef(row + (long long)1) * cos(theta * PI / 180) + (mn.coeffRef(row + (long long)1) - m.coeffRef(row)) / A / tStep - Ck0 + Ck1 * (rho.coeffRef(row + (long long)1) - rhon.coeffRef(row + (long long)1)) / tStep;
		}
		else
		{
			W1.insert(row, row) = 1;
		}
	}
}
