#include <MassEquation.h>
#include <calculations.h>
#include <geometry.h>
#include <CrossMomentumEquation.h>


namespace opensubc {

	namespace calculation {
		 Eigen::SparseMatrix<double> MassA;
		 Eigen::VectorXd MassB;//质量方程系数矩阵与常数向量
	}
}

void opensubc::initMassEquation()//初始化质量方程
{
	using namespace opensubc::calculation;
	//定义向量长度
	MassA.resize(numOfChannelData, numOfChannelData);
	MassB.resize(numOfChannelData);
}
void opensubc::calculateMassMatrix()//计算质量方程系数矩阵与常数向量
{
	using namespace opensubc::calculation;
	using namespace opensubc::geometry;
	//将所有向量置零
	MassA.setZero();
	MassB.setZero();
	for (size_t i = 0; i < numOfChannelData; ++i)
	{
		if (!checkInletInterval(i))//判断是不是入口虚拟网格，如果不是则进行填写
		{
			unsigned channelid = i / (numOfBlocks + (long long)1);//得到对应的channelid
			MassA.insert(i, i) = 1;
			MassA.insert(i, i-1) = -1;
			double ew = 0;//表示eik*wkj的求和
			for (auto& gapId : channels[channelid].gapIds)//遍历子通道连接的所有gap
			{
				unsigned connectedChannelId;
				if (gaps[gapId].getOtherChannelId(channelid, connectedChannelId))//如果该gap确实连接了另一个子通道
				{
					unsigned gapindex = gapId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1);//对应的gap的控制体在gap向量中的索引
					ew += w.coeffRef(gapindex) * (channelid < connectedChannelId ? 1 : -1);
				}
			}
			MassB(i) = -channels[channelid].A * length / numOfBlocks / tStep * (rho.coeffRef(i) - rhon.coeffRef(i)) - length / numOfBlocks * ew;
		}
		else
		{
			MassA.insert(i, i) = 1;
			MassB(i) = m.coeffRef(i);
		}
	}


}