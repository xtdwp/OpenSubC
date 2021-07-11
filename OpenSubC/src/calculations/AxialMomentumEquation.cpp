#include <calculations.h>
#include <geometry.h>
#include <AxialMomentumEquation.h>
#include <CrossMomentumEquation.h>
#include <constants.h>

namespace opensubc {

	namespace calculation {
		Eigen::VectorXd DPx, R, Uk1, Pk; //分别为压力梯度dP/dX、变量Rij、变量Ukj、相邻通道间的压差Pkj
	}
}

void opensubc::initAxialMomentumEquation()//初始化轴向动量方程
{
	using namespace opensubc::calculation;
	//定义向量长度
	DPx.resize(numOfChannelData);
	R.resize(numOfChannelData);
	Uk1.resize(numOfGapData);
	Pk.resize(numOfGapData);	
	Pk.setZero();
}
void opensubc::calculateAxialMomentumVectors()//计算Rij、Ukj等向量
{
	using namespace opensubc::calculation;
	using namespace opensubc::geometry;
	//将所有向量置零
	R.setZero();
	Uk1.setZero();
	//计算Rij向量
	for (int i = 0; i < numOfChannelData; ++i)
	{
		if (!checkInletInterval(i))//判断是不是入口虚拟网格，如果不是则进行填写
		{
			unsigned channelid = i / (numOfBlocks + (long long)1);//得到对应的channelid
			R(i) = ((f.coeffRef(i) / rho.coeffRef(i) / channels[channelid].Dh +K.coeffRef(i)*v.coeffRef(i)/ (length / numOfBlocks)) / 2+(v.coeffRef(i)- v.coeffRef(i-(long long)1))/ (length / numOfBlocks)) / pow(channels[channelid].A, 2);
			/*std::cout << "R  " << R(i) << "  " << f.coeffRef(i) << "  " << rho.coeffRef(i) << "  " << K.coeffRef(i) << "  " << v.coeffRef(i) << std::endl;
			system("pause");*/
		}
	}
	//计算Ukj向量
	for (int i = 0; i < numOfGapData; ++i)
	{
		if (!checkInletInterval(i))//判断是不是入口虚拟网格，如果不是则进行填写
		{
			unsigned gapid = i / (numOfBlocks + (long long)1);//得到对应的gapid
			if (!checkBoundaryGap(gapid))//判断是不是边界通道，如果不是则进行填写
			{
				if (w.coeffRef(i) > 0)//判断wkj的大小，大于零，则取小通道（channelIds[0]）的Uij，否则取大通道的Uij
					Uk1(i) = U.coeffRef(gaps[gapid].channelIds[0] * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1));
				else
					Uk1(i) = U.coeffRef(gaps[gapid].channelIds[1] * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1));
			}
		}
	}
	
}
void opensubc::calculateAxialMomentumEquation()//计算压力梯度和相邻通道的压差
{
	using namespace opensubc::calculation;
	using namespace opensubc::geometry;
	DPx.setZero();
	Pk.setZero();
	//计算压力梯度DPx向量
	for (int i = 0; i < numOfChannelData; ++i)
	{
		if (!checkInletInterval(i))//判断是不是入口虚拟网格，如果不是则进行填写
		{
			unsigned channelid = i / (numOfBlocks + (long long)1);//得到对应的channelid
			double Ck0=0, Ck1=0;//分别表示跟间隙有关的那两项常数
			double A = channels[channelid].A;
			for (auto& gapId : channels[channelid].gapIds)//遍历子通道连接的所有gap
			{
				unsigned connectedChannelId;
				if (gaps[gapId].getOtherChannelId(channelid, connectedChannelId))//如果该gap确实连接了另一个子通道
				{
					unsigned gapindex = gapId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1);//对应的gap的控制体在gap向量中的索引
					Ck0 += (w.coeffRef(gapindex) * (channelid < connectedChannelId ? 1 : -1) * Uk1.coeffRef(gapindex) + fT * wTurbulence.coeffRef(gapindex) * (U.coeffRef(i) - U.coeffRef(connectedChannelId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1))))/ A;
					Ck1 += (w.coeffRef(gapindex) * (channelid < connectedChannelId ? 1 : -1)) / A;
				}
			}
			DPx(i) = (-R.coeffRef(i)) * pow(m.coeffRef(i - (long long)1), 2) - g * rho.coeffRef(i) * cos(theta * PI / 180) + (mn.coeffRef(i) - m.coeffRef(i - (long long)1)) / A / tStep - Ck0 + (2*U.coeffRef(i) + (length / numOfBlocks) / tStep + R.coeffRef(i) * A * (length / numOfBlocks) * (m.coeffRef(i) + m.coeffRef(i - (long long)1))) * ((rho.coeffRef(i) - rhon.coeffRef(i)) / tStep + Ck1);
			DPx(i) /= g;
			std::cout << DPx(i) << " " << (-R.coeffRef(i)) * pow(m.coeffRef(i - (long long)1), 2) 
				<< " " 
				<< -g * rho.coeffRef(i) * cos(theta * PI / 180)
				<< " "
				<< (mn.coeffRef(i) - m.coeffRef(i - (long long)1)) / A / tStep
				<< " "
				<< -Ck0
				<< " "
				<< (2 * U.coeffRef(i) + (length / numOfBlocks) / tStep + R.coeffRef(i) * A * (length / numOfBlocks) * (m.coeffRef(i) + m.coeffRef(i - (long long)1)))
				<< " "
				<< (rho.coeffRef(i) - rhon.coeffRef(i)) / tStep
				<< " "
				<< Ck1
				<< std::endl;
			//system("pause");
		}
	}
	//计算相邻通道的压差Pkj向量
	for (int i = 0; i < numOfGapData; ++i)
	{
		unsigned gapid = i / (numOfBlocks + (long long)1);//得到对应的gapid
		if (!checkBoundaryGap(gapid))//判断是不是边界通道，如果不是则进行填写
		{
			if (checkInletInterval(i))//判断是不是入口虚拟网格,如果是，则认为相邻通道压差为0
			{
				Pk(i) = 0;
			}
			else
			{
				Pk(i) = Pk.coeffRef(i - (long long)1) + (length / numOfBlocks) * (DPx.coeffRef(gaps[gapid].channelIds[0] * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1)) - DPx.coeffRef(gaps[gapid].channelIds[1] * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1)));
			}
		}
		
	}
	
}
