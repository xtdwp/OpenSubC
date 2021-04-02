#include <CrossMomentumEquation.h>
#include <calculations.h>
#include <geometry.h>
#include <AxialMomentumEquation.h>

namespace opensubc {
	namespace calculation {
		 Eigen::SparseVector<double> v, vk, U, Uk; //分别为vij、vkj、Uij、Ukj
		 Eigen::SparseMatrix<double> CrossMomentumA; //横向动量方程系数矩阵 
		 Eigen::SparseVector<double> CrossMomentumB;//常数向量
	}
}

void opensubc::initCrossMomentumEquation()//初始化横向动量方程
{
	using namespace opensubc::calculation;
	//定义向量长度
	v.resize(numOfChannelData);
	U.resize(numOfChannelData);
	vk.resize(numOfGapData);
	Uk.resize(numOfGapData);
	CrossMomentumA.resize(numOfGapData, numOfGapData);
	CrossMomentumB.resize(numOfGapData);
}
void opensubc::calculateCrossMomentumVectors()//计算vij、vkj、Uij、Ukj等向量
{
	using namespace opensubc::calculation;
	using namespace opensubc::geometry;
	//将所有向量置零
	v.setZero();
	U.setZero();
	vk.setZero();
	Uk.setZero();
	//计算vij和Uij的值（在所有的出口网格内mij必须大于0）
	for (size_t i = 0; i < numOfChannelData; ++i)
	{
		if (!checkInletInterval(i))//判断是不是入口虚拟网格，如果不是则进行填写
		{
			if (m.coeffRef(i) > 0)
				v.insert(i) = 1 / rho.coeffRef(i);
			else
				v.insert(i) = 1 / rho.coeffRef(i + (long long)1);
			U.insert(i) = v.coeffRef(i) * m.coeffRef(i) / channels[i / (numOfBlocks + (long long)1)].A;
		}
	}
	//计算vkj和Ukj的值
	for (size_t i = 0; i < numOfGapData; ++i)
	{
		if (!checkInletInterval(i))//判断是不是入口虚拟网格，如果不是则进行填写
		{
			unsigned gapid = i / (numOfBlocks + (long long)1);//得到对应的gapid
			if (!checkBoundaryGap(gapid))//判断是不是边界通道，如果不是则进行填写
			{
				double vk_value = 0;
				double Uk_value = 0;
				for (auto& channelId : gaps[gapid].channelIds)//遍历gap中连接的所有的channel编号
				{
					vk_value += v.coeffRef(channelId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1));//将连接的所有channel的对应的vij求和
					Uk_value += U.coeffRef(channelId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1));//将连接的所有channel的对应的Uij求和
				}
				vk_value /= gaps[gapid].channelIds.size();//取平均
				Uk_value /= gaps[gapid].channelIds.size();//取平均
				vk.insert(i) = vk_value;
				Uk.insert(i) = Uk_value;
			}
		}
	}	
}
void opensubc::calculateCrossMomentumMatrix()//计算横向动量方程系数矩阵与常数向量
{
	using namespace opensubc::calculation;
	using namespace opensubc::geometry;
	CrossMomentumA.setZero();//置零
	CrossMomentumB.setZero();
	for (size_t row = 0; row < numOfGapData; ++row)//对每一行进行遍历
	{
		if (!checkInletInterval(row))//判断是不是入口虚拟网格，如果不是则进行填写
		{
			unsigned gapid = row / (numOfBlocks + (long long)1);
			if (!checkBoundaryGap(gapid))//判断是不是边界通道，如果不是则进行填写
			{    
				//计算横向动量方程系数矩阵
				double coeffkj = length / numOfBlocks / tStep + KG * vk.coeffRef(row) * w.coeffRef(row) * length / numOfBlocks / gaps[gapid].l / gaps[gapid].s / 2;//wkj的系数
				if (Uk.coeffRef(row) > 0)
					coeffkj += Uk.coeffRef(row);
				else
					CrossMomentumA.insert(row, row +(long long) 1) = Uk.coeffRef(row);
				if (Uk.coeffRef(row - (long long)1) < 0)
					coeffkj -= Uk.coeffRef(row - (long long)1);
				else
					CrossMomentumA.insert(row, row - (long long)1) = -Uk.coeffRef(row - (long long)1);
				CrossMomentumA.insert(row, row) = coeffkj;
				//计算常数向量
				CrossMomentumB.insert(row) = gaps[gapid].l / gaps[gapid].s * length / numOfBlocks * Pk.coeffRef(row - (long long)1) + length / numOfBlocks / tStep * wn.coeffRef(row);

			}
		}
	}

}
bool opensubc::checkInletInterval(int i)//判断是不是入口处网格，，是则返回真
{
	using namespace opensubc::calculation;
	if (i % (numOfBlocks + (long long)1) == 0)
	{
		return true;
	}
	else
		return false;
}
bool opensubc::checkBoundaryGap(int gapid)//判断是不是boundarygap，是则返回真
{
	using namespace opensubc::geometry;
	if (gaps[gapid].channelIds.size() < 2)
	{
		return true;
	}
	else
		return false;
}