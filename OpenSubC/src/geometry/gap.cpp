#include "gap.h"
#include <geometry.h>
#include <fuelRod.h>

opensubc::gap::gap()//�߽��ϵļ�϶
{
    //��¼�߽���Ϣ
    boundaryType = BoundaryType::NoBoundary;//��ȫ�߽�ļ�϶����Ҫ��Щ��Ϣ����㸳ֵ����
}



opensubc::gap::gap(const fuelRod& rod0, const fuelRod& rod1)//����ȼ�ϰ�������϶������ȼ�ϰ���
{
    //�洢ȼ�ϰ�id
    rodIds.push_back(rod0.id);
    rodIds.push_back(rod1.id);
    //��¼�߽���Ϣ
    boundaryType = BoundaryType::NoBoundary;
    //�����϶���,��Ҫ�ж�ȼ�ϰ���Է�λ
    sk = sqrt(pow(rod0.x - rod1.x, 2) + pow(rod0.y - rod1.y, 2)) - rod0.r - rod1.r;
}

opensubc::gap::gap(const fuelRod& rod, BoundaryType _boundaryType)//����ȼ�ϰ����϶��ȹ�����϶��һ��ȼ�ϰ���߽磩
{
    //�洢ȼ�ϰ�id
    rodIds.push_back(rod.id);
    //��¼�߽���Ϣ
    boundaryType = _boundaryType;
    //���ü�϶���
    if (boundaryType == BoundaryType::PositiveX || boundaryType == BoundaryType::NegativeX)
        sk = geometry::boundaryWidth;
    if (boundaryType == BoundaryType::PositiveY || boundaryType == BoundaryType::NegativeY)
        sk = geometry::boundaryHeight;
}

bool opensubc::gap::getOtherChannelId(unsigned channelId, unsigned& otherChannelId)//���ݸ�����ͨ��id������gap���ӵ���һ��ͨ��id
{
    if (channelIds.size() != 2)
        return false;
    if (channelId == channelIds[0])
        otherChannelId = channelIds[1];
    else if (channelId == channelIds[1])
        otherChannelId = channelIds[0];
    else
        return false;
    return true;
}