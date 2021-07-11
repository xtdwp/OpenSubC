#include <EnergyEquation.h>
#include <calculations.h>
#include <geometry.h>
#include <CrossMomentumEquation.h>
#include "constants.h"


namespace opensubc {

	namespace calculation {
		 Eigen::SparseMatrix<double> EnergyA;
		 Eigen::VectorXd EnergyB;//能量方程系数矩阵与常数向量
	}
}

void opensubc::initEnergyEquation()//初始化质量方程
{
	using namespace opensubc::calculation;
	//定义向量长度
	EnergyA.resize(numOfChannelData, numOfChannelData);
	EnergyB.resize(numOfChannelData);
}
void opensubc::calculateEnergyMatrix()//计算能量方程系数矩阵与常数向量
{
	using namespace opensubc::calculation;
	using namespace opensubc::geometry;
	//将所有向量置零
	EnergyA.setZero();
	EnergyB.setZero();
	for (size_t i = 0; i < numOfChannelData; ++i)
	{
		if (!checkInletInterval(i))//判断是不是入口虚拟网格，如果不是则进行填写
		{
			unsigned channelid = i / (numOfBlocks + (long long)1);//得到对应的channelid
			double Ck = 0;//表示对角线元素中与间隙相关的求和项
			double ew = 0;//表示eik*wkj的求和
			double Ct = 0;//表示常数项中相邻子通道间热传导项
			for (auto& gapId : channels[channelid].gapIds)//遍历子通道连接的所有gap
			{
				unsigned connectedChannelId;
				if (gaps[gapId].getOtherChannelId(channelid, connectedChannelId))//如果该gap确实连接了另一个子通道
				{
					unsigned gapindex = gapId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1);//对应的gap的控制体在gap向量中的索引
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
			double Cq = 0;//表示燃料棒导热求和项
			for (auto& rodId : channels[channelid].rodIds)//遍历子通道连接的所有rod
			{
				Cq += (length / numOfBlocks) * rods[rodId].r * PI / 2 * q.coeffRef(rodId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1)) * (1 + rQ);
			}
			EnergyB(i) = channels[channelid].A * (length / numOfBlocks) / tStep * rhon.coeffRef(i) * hn.coeffRef(i) + Cq + Ct;
			/*std::cout << "q  " << Cq << "  " << Ct << std::endl;
			system("pause");*/
		}
		//让入口处的h不变
		else
		{
			EnergyA.insert(i, i) = 1;
			EnergyB(i) = h.coeffRef(i);
		}
	}
}
