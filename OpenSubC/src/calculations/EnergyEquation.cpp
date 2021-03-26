#include <EnergyEquation.h>
#include <calculations.h>
#include <geometry.h>

namespace opensubc {
	namespace calculation {
		Eigen::SparseVector<double> AX, AXt; //��AXt����AiXj / dt��
		Eigen::SparseMatrix<double> CC;//ͨ�����Ӿ���
		std::vector<Eigen::SparseMatrix<double>> CT, CM;//�����������Ӿ�������������������Ӿ���
		Eigen::SparseMatrix<double> CQ;//����ȼ�ϰ�q�ĵ����Ӿ���
		Eigen::SparseMatrix<double> CST;//����ͨ���䵼�ȵ����Ӿ���
	}
}

void opensubc::initEnergyEquation()//��ʼ�����������������
{
    initGeometryVector();
    initCC();
    initCT();
    initCM();
    initCQ();
    initCST();
}

void opensubc::initGeometryVector()//��ʼ�����ξ���AXt����AiXj/dt��
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;

    //������������
    AX.resize(numOfChannelData);
    AXt.resize(numOfChannelData);

    //������ֵ
    for (int i = 0; i < numOfChannelData; ++i)
        AX.insert(i) = channels[i / (numOfBlocks + 1)].A * length / numOfBlocks;
    AXt = AX / tStep;
}

void opensubc::initCC()//��ʼ��ͨ�����Ӿ���
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;

    //��������С
    CC.resize(numOfChannelData, numOfChannelData);

    //����ֵ
    for (int channelId = 0; channelId < channels.size(); ++channelId)//������ͨ��
    {
        channel& channel = channels[channelId];
            for (auto& gapId : channel.gapIds)//������ͨ�����ӵ�����gap
            {
                unsigned connectedChannelId;
                if (gaps[gapId].getOtherChannelId(channelId, connectedChannelId))//�����gapȷʵ��������һ����ͨ��
                {
                    for (int i = 0; i < numOfBlocks + 1; ++i)//����ÿ����ͨ����������
                    {
                        //������ͨ����������
                        int row = connectedChannelId * (numOfBlocks + 1) + i;
                        int col = channelId * (numOfBlocks + 1) + i;
                        //����ֵ
                        CC.insert(row, col) = -1;
                    }
                }
            }
    }
}

void opensubc::initCT()//��ʼ�������������Ӿ���
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;

    //������������С
    CT.resize(numOfChannelData);
    //������������ڸ������С
    for (auto& matrix : CT)
        matrix.resize(numOfChannelData, numOfGapData);

    //������ֵ
    for (int channelId = 0; channelId < channels.size(); ++channelId)//������ͨ��
    {
        channel& channel = channels[channelId];
        for (auto& gapId : channel.gapIds)//������ͨ�����ӵ�����gap
        {
            unsigned connectedChannelId;
            if (gaps[gapId].getOtherChannelId(channelId, connectedChannelId))//�����gapȷʵ��������һ����ͨ��
            {
                for (int i = 0; i < numOfBlocks + 1; ++i)//����ÿ����ͨ����������
                {
                    //������ͨ����������
                    int index = channelId * (numOfBlocks + 1) + i;
                    int row = connectedChannelId * (numOfBlocks + 1) + i;
                    int col = gapId * (numOfBlocks + 1) + i;
                    //����ֵ
                    CT[index].insert(row, col) = -length / numOfBlocks;
                    CT[index].insert(index, col) = length / numOfBlocks;
                }
            }
        }
    }

}

void opensubc::initCM()//��ʼ�����������������Ӿ���
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;

    //������������С
    CM.resize(numOfChannelData);
    //��������ڸ������С
    for (auto& matrix : CM)
        matrix.resize(numOfChannelData, numOfGapData);

    //������ֵ
    for (int channelId = 0; channelId < channels.size(); ++channelId)//������ͨ��
    {
        channel& channel = channels[channelId];
        for (auto& gapId : channel.gapIds)//������ͨ�����ӵ�����gap
        {
            unsigned connectedChannelId;
            if (gaps[gapId].getOtherChannelId(channelId, connectedChannelId))//�����gapȷʵ��������һ����ͨ��
            {
                for (int i = 0; i < numOfBlocks + 1; ++i)//����ÿ����ͨ����������
                {
                    //������ͨ����������
                    int index = channelId * (numOfBlocks + 1) + i;
                    int row = connectedChannelId * (numOfBlocks + 1) + i;
                    int col = gapId * (numOfBlocks + 1) + i;
                    //���㷽��ʸ��
                    int e = channelId < connectedChannelId ? 1 : -1;
                    //����ֵ
                    CM[index].insert(row, col) = length / numOfBlocks * e;
                }
            }
        }
    }
}

void opensubc::initCQ()//��ʼ������ȼ�ϰ�q�ĵ����Ӿ���
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;

    //��������С
    CQ.resize(numOfChannelData, numOfRodData);

    //����ֵ
    for (int channelId = 0; channelId < channels.size(); ++channelId)//������ͨ��
    {
        channel& channel = channels[channelId];
        for (auto& rodId : channel.rodIds)//������ͨ�����ӵ�����ȼ�ϰ�
        {
                for (int i = 0; i < numOfBlocks + 1; ++i)//����ÿ����ͨ����������
                {
                    //������ͨ����������
                    int row = channelId * (numOfBlocks + 1) + i;
                    int col = rodId * (numOfBlocks + 1) + i;
                    //����ֵ
                    CQ.insert(row, col) = length / numOfBlocks * channel.circleLength[rodId];
                }
        }
    }
}

void opensubc::initCST()//��ʼ��ͨ���������Ӿ���
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;

    //��������С
    CST.resize(numOfChannelData, numOfChannelData);
    
    //����ֵ
    for (int channelId = 0; channelId < channels.size(); ++channelId)//������ͨ��
    {
        channel& channel = channels[channelId];
        for (auto& gapId : channel.gapIds)//������ͨ�����ӵ�����gap
        {
            unsigned connectedChannelId;
            if (gaps[gapId].getOtherChannelId(channelId, connectedChannelId))//�����gapȷʵ��������һ����ͨ��
            {
                for (int i = 0; i < numOfBlocks + 1; ++i)//����ÿ����ͨ����������
                {
                    //������ͨ����������
                    int row = channelId * (numOfBlocks + 1) + i;
                    int col = connectedChannelId * (numOfBlocks + 1) + i;
                    //����ֵ
                    CST.insert(row, col) = length / numOfBlocks * GT * gaps[gapId].s / gaps[gapId].l;
                }
            }
        }
    }

}

void opensubc::calculateEnergyMatrix()//������������ϵ�������볣������
{

}
