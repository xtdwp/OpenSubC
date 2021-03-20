#include "channel.h"
#include <geometry.h>
#include <constants.h>
#include <iostream>
#include <fuelRod.h>
#include <gap.h>

opensubc::channel::channel(unsigned _id, const std::vector<unsigned>& _rodIds, const std::vector<BoundaryType> _boundaryTypes)//����һ������ȼ�ϰ��ͱ߽���Ϣ������ͨ��
    :id(_id), rodIds(_rodIds), boundaryTypes(_boundaryTypes)
{
    circleLength.resize(rodIds.size());
    setChannel();
}

void opensubc::channel::setChannel()//�ڳ�ʼ��֮�󣬸���ȼ�ϰ�λ�÷ֲ�������ͨ�����Σ�Ŀǰֻ֧����򵥵ľ������Σ�
{
    ////ÿ��ȼ�ϰ������Ӹ�ͨ��id��Ϣ
    //for (int i = 0; i < 4; ++i)
    //{
    //    geometry::rods[rodIds[i]].channelIds.push_back(id);
    //    circleLength[i] = geometry::rods[rodIds[i]].r * PI * 0.5;
    //}

    //double r = geometry::rods[rodIds[0]].r;
    //unsigned crossId;//����һ��ȼ�ϰ��Խ��ߵ�ȼ�ϰ��������е�λ��
    ////�������������ܳ�
    //for (int i = 1; i < 4; ++i)
    //{
    //    double dx = abs(geometry::rods[rodIds[0]].x - geometry::rods[rodIds[i]].x);
    //    double dy = abs(geometry::rods[rodIds[0]].y - geometry::rods[rodIds[i]].y);
    //    if (dx > r && dy > r)
    //    {
    //        A = dx * dy;
    //        crossId = i;
    //    }
    //}
    //A -= PI * r * r;
    ////����gap
    //for (int i = 1; i < 4; ++i)
    //{
    //    if (i == crossId)
    //        continue;
    //    int gapId = gap::checkGapExistence(rodIds[0], rodIds[i]);
    //    if (gapId == -1)
    //    {
    //        geometry::gaps.push_back(gap(rodIds[0], rodIds[i]));
    //        geometry::gaps[geometry::gaps.size() - 1].channelIds[0] = id;
    //        geometry::rods[rodIds[0]].gapIds.push_back(geometry::gaps.size() - 1);
    //        geometry::rods[rodIds[i]].gapIds.push_back(geometry::gaps.size() - 1);
    //    }
    //    else
    //    {
    //        geometry::gaps[gapId].channelId[1] = id;
    //    }

    //    gapId = gap::checkGapExistence(rodIds[crossId], rodIds[i]);
    //    if (gapId == -1)
    //    {
    //        geometry::gaps.push_back(gap(rodIds[crossId], rodIds[i]));
    //        geometry::gaps[geometry::gaps.size() - 1].channelId[0] = id;
    //        geometry::rods[rodIds[crossId]].gapIds.push_back(geometry::gaps.size() - 1);
    //        geometry::rods[rodIds[i]].gapIds.push_back(geometry::gaps.size() - 1);
    //    }
    //    else
    //    {
    //        geometry::gaps[gapId].channelId[1] = id;
    //    }

    //}
}

void opensubc::channel::setCornerChannel()//������ͨ��
{
    using namespace opensubc::geometry;

    fuelRod& rod = rods[rodIds[0]];
    rod.channelIds.push_back(id);//ȼ�ϰ����Ӹ�ͨ��id

    //������ͨ�����ĵ������������
    x = rod.x + boundaryWidth * 0.5;
    y = rod.y + boundaryHeight * 0.5;
    A = boundaryHeight * boundaryWidth;

    //����ȼ�ϰ��ڸ�ͨ���ڽ����ܳ�
    circleLength.push_back(rod.r * PI * 0.5);

    //�����߽��ϵ�����gap
    addBoundaryGap();
    addBoundaryGap();

    //������ȼ�ϰ����ӵ�����gap(��Ҫ�жϸü�϶�Ƿ��Ѿ�����)
    addSingleRodGap(rod, boundaryTypes[0]);
    addSingleRodGap(rod, boundaryTypes[1]);
}

void opensubc::channel::setEdgeChannel()//������ͨ��
{
    using namespace opensubc::geometry;


}

void opensubc::channel::setOrdinaryChannel()//������ͨ��ͨ��
{

}

void opensubc::channel::addBoundaryGap()//Ϊ����ͨ�����һ��λ�ڱ߽��gap
{
    using namespace opensubc::geometry;

    //�ڼ�϶����������һ����gap
    gaps.push_back(gap());
    //����id�����ֵ
    setIdOfLastGap();
}

void opensubc::channel::addSingleRodGap(const fuelRod& rod, BoundaryType boundaryType)//Ϊ����ͨ�����һ����ȼ�ϰ���gap
{
    using namespace opensubc::geometry;
    //��Ҫ�ж�gap�Ƿ��Ѿ�������
    unsigned gapId;
    if (checkGapExistence(rod, boundaryType, gapId))//����Ѿ�������
        addExistsGap(gapId);
    else//���δ������
    {
        //����gap����������
        gaps.push_back(gap(rod, boundaryType));
        //����id�����ֵ
        setIdOfLastGap();
    }
}

void opensubc::channel::addOrdinaryGap(const fuelRod& rod0, const fuelRod& rod1)//Ϊ����ͨ�����һ����ͨ������ȼ�ϰ�֮���gap
{
    using namespace opensubc::geometry;
    //��Ҫ�ж�gap�Ƿ��Ѿ�������
    unsigned gapId;
    if (checkGapExistence(rod0, rod1, gapId))//����Ѿ�������
        addExistsGap(gapId);
    else//���δ������
    {
        //����gap����������
        gaps.push_back(gap(rod0, rod1));
        //����id�����ֵ
        setIdOfLastGap();
    }
}

void opensubc::channel::addExistsGap(unsigned gapId)//Ϊ����ͨ�����һ���Ѿ����ڵ�gap
{
    using namespace opensubc::geometry;
    gap& gap = gaps[gapId];

    //����gap�ļ�϶����lk
    channel& channel = channels[gap.channelIds[0]];
    gap.lk = sqrt(pow(channel.x - x, 2) + pow(channel.y - y, 2));

    //��϶���Ӹ���ͨ����id
    gap.channelIds.push_back(id);

    //����ͨ�����Ӽ�϶id
    gapIds.push_back(gapId);
}

void opensubc::channel::setIdOfLastGap()//��gap�����е����һ��gap����id�����Ϣ�����һ��˵���ոմ���,��Ҫ����ʹ�ã�
{
    using namespace opensubc::geometry;
    //id����Ϊ�����е�λ��
    gaps.end()->id = gaps.size() - 1;

    //Ϊ��gap��Ӹ���ͨ����id
    gaps.end()->channelIds.push_back(id);

    //Ϊ����ͨ����������ӵ�gap��id
    gaps.end()->channelIds.push_back(id);
}

opensubc::channel::~channel()
{

}