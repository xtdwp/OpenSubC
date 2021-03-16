#include "gap.h"
#include <geometry.h>
#include <fuelRod.h>

unsigned opensubc::gap::num = 0;

opensubc::gap::gap()//�߽��ϵļ�϶
{
    //������϶id
    id = num++;
    //��¼�߽���Ϣ
    boundaryType = BoundaryType::NoBoundary;//��ȫ�߽�ļ�϶����Ҫ��Щ��Ϣ����㸳ֵ����
}

opensubc::gap::gap(const fuelRod& rod0, const fuelRod& rod1)//����ȼ�ϰ�������϶������ȼ�ϰ���
{
    //������϶id
    id = num++;
    //�洢ȼ�ϰ�id
    rodIds.push_back(rod0.id);
    rodIds.push_back(rod1.id);
    //��¼�߽���Ϣ
    boundaryType = BoundaryType::NoBoundary;
    //�����϶���,��Ҫ�ж�ȼ�ϰ���Է�λ
    sk = abs(rod0.x - rod1.x) - rod0.r - rod1.r;
    if (sk < 0)
        sk = abs(rod0.y - rod1.y) - rod0.r - rod1.r;
}

opensubc::gap::gap(const fuelRod& rod, BoundaryType _boundaryType)//����ȼ�ϰ����϶��ȹ�����϶��һ��ȼ�ϰ���߽磩
{
    //������϶id
    id = num++;
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

unsigned opensubc::gap::getOtherChannelId(unsigned channelId)//���ݸ�����ͨ��id������gap���ӵ���һ��ͨ��id
{

}