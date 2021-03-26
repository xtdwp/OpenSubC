#include <EnergyEquation.h>
#include <calculations.h>
#include <geometry.h>

namespace opensubc {
	namespace calculation {
		Eigen::SparseVector<double> AX, AXt; //（AXt，即AiXj / dt）
		Eigen::SparseMatrix<double> CC;//通道连接矩阵
		std::vector<Eigen::SparseMatrix<double>> CT, CM;//湍流交混连接矩阵与横向质量流量连接矩阵
		Eigen::SparseMatrix<double> CQ;//计算燃料棒q的的连接矩阵
		Eigen::SparseMatrix<double> CST;//计算通道间导热的连接矩阵
	}
}

void opensubc::initEnergyEquation()//初始化能量方程所需矩阵
{
    initGeometryVector();
    initCC();
    initCT();
    initCM();
    initCQ();
    initCST();
}

void opensubc::initGeometryVector()//初始化几何矩阵（AXt，即AiXj/dt）
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;

    //定义向量长度
    AX.resize(numOfChannelData);
    AXt.resize(numOfChannelData);

    //向量赋值
    for (int i = 0; i < numOfChannelData; ++i)
        AX.insert(i) = channels[i / (numOfBlocks + 1)].A * length / numOfBlocks;
    AXt = AX / tStep;
}

void opensubc::initCC()//初始化通道连接矩阵
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;

    //定义矩阵大小
    CC.resize(numOfChannelData, numOfChannelData);

    //矩阵赋值
    for (int channelId = 0; channelId < channels.size(); ++channelId)//遍历子通道
    {
        channel& channel = channels[channelId];
            for (auto& gapId : channel.gapIds)//遍历子通道连接的所有gap
            {
                unsigned connectedChannelId;
                if (gaps[gapId].getOtherChannelId(channelId, connectedChannelId))//如果该gap确实连接了另一个子通道
                {
                    for (int i = 0; i < numOfBlocks + 1; ++i)//遍历每个子通道所有网格
                    {
                        //计算子通道网格的序号
                        int row = connectedChannelId * (numOfBlocks + 1) + i;
                        int col = channelId * (numOfBlocks + 1) + i;
                        //矩阵赋值
                        CC.insert(row, col) = -1;
                    }
                }
            }
    }
}

void opensubc::initCT()//初始化湍流交混连接矩阵
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;

    //定义矩阵数组大小
    CT.resize(numOfChannelData);
    //定义矩阵数组内各矩阵大小
    for (auto& matrix : CT)
        matrix.resize(numOfChannelData, numOfGapData);

    //各矩阵赋值
    for (int channelId = 0; channelId < channels.size(); ++channelId)//遍历子通道
    {
        channel& channel = channels[channelId];
        for (auto& gapId : channel.gapIds)//遍历子通道连接的所有gap
        {
            unsigned connectedChannelId;
            if (gaps[gapId].getOtherChannelId(channelId, connectedChannelId))//如果该gap确实连接了另一个子通道
            {
                for (int i = 0; i < numOfBlocks + 1; ++i)//遍历每个子通道所有网格
                {
                    //计算子通道网格的序号
                    int index = channelId * (numOfBlocks + 1) + i;
                    int row = connectedChannelId * (numOfBlocks + 1) + i;
                    int col = gapId * (numOfBlocks + 1) + i;
                    //矩阵赋值
                    CT[index].insert(row, col) = -length / numOfBlocks;
                    CT[index].insert(index, col) = length / numOfBlocks;
                }
            }
        }
    }

}

void opensubc::initCM()//初始化横向质量流量连接矩阵
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;

    //定义矩阵数组大小
    CM.resize(numOfChannelData);
    //定义矩阵内各矩阵大小
    for (auto& matrix : CM)
        matrix.resize(numOfChannelData, numOfGapData);

    //各矩阵赋值
    for (int channelId = 0; channelId < channels.size(); ++channelId)//遍历子通道
    {
        channel& channel = channels[channelId];
        for (auto& gapId : channel.gapIds)//遍历子通道连接的所有gap
        {
            unsigned connectedChannelId;
            if (gaps[gapId].getOtherChannelId(channelId, connectedChannelId))//如果该gap确实连接了另一个子通道
            {
                for (int i = 0; i < numOfBlocks + 1; ++i)//遍历每个子通道所有网格
                {
                    //计算子通道网格的序号
                    int index = channelId * (numOfBlocks + 1) + i;
                    int row = connectedChannelId * (numOfBlocks + 1) + i;
                    int col = gapId * (numOfBlocks + 1) + i;
                    //计算方向矢量
                    int e = channelId < connectedChannelId ? 1 : -1;
                    //矩阵赋值
                    CM[index].insert(row, col) = length / numOfBlocks * e;
                }
            }
        }
    }
}

void opensubc::initCQ()//初始化计算燃料棒q的的连接矩阵
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;

    //定义矩阵大小
    CQ.resize(numOfChannelData, numOfRodData);

    //矩阵赋值
    for (int channelId = 0; channelId < channels.size(); ++channelId)//遍历子通道
    {
        channel& channel = channels[channelId];
        for (auto& rodId : channel.rodIds)//遍历子通道连接的所有燃料棒
        {
                for (int i = 0; i < numOfBlocks + 1; ++i)//遍历每个子通道所有网格
                {
                    //计算子通道网格的序号
                    int row = channelId * (numOfBlocks + 1) + i;
                    int col = rodId * (numOfBlocks + 1) + i;
                    //矩阵赋值
                    CQ.insert(row, col) = length / numOfBlocks * channel.circleLength[rodId];
                }
        }
    }
}

void opensubc::initCST()//初始化通道导热连接矩阵
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;

    //定义矩阵大小
    CST.resize(numOfChannelData, numOfChannelData);
    
    //矩阵赋值
    for (int channelId = 0; channelId < channels.size(); ++channelId)//遍历子通道
    {
        channel& channel = channels[channelId];
        for (auto& gapId : channel.gapIds)//遍历子通道连接的所有gap
        {
            unsigned connectedChannelId;
            if (gaps[gapId].getOtherChannelId(channelId, connectedChannelId))//如果该gap确实连接了另一个子通道
            {
                for (int i = 0; i < numOfBlocks + 1; ++i)//遍历每个子通道所有网格
                {
                    //计算子通道网格的序号
                    int row = channelId * (numOfBlocks + 1) + i;
                    int col = connectedChannelId * (numOfBlocks + 1) + i;
                    //矩阵赋值
                    CST.insert(row, col) = length / numOfBlocks * GT * gaps[gapId].s / gaps[gapId].l;
                }
            }
        }
    }

}

void opensubc::calculateEnergyMatrix()//计算能量方程系数矩阵与常数向量
{

}
