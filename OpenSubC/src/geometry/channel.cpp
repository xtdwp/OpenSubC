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
    if (rodIds.size() == 1)
        setCornerChannel();
    else if (rodIds.size() == 2)
        setEdgeChannel();
    else
        setOrdinaryChannel();
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

    //������ͨ�����ĵ�������������������ܳ�
    if (boundaryTypes[0] == BoundaryType::PositiveX || boundaryTypes[0] == BoundaryType::NegativeX)
    {
        x = rods[rodIds[0]].x + boundaryWidth * 0.5;
        y = (rods[rodIds[0]].y + rods[rodIds[1]].y) * 0.5;
        A = abs(rods[rodIds[0]].y - rods[rodIds[1]].y) * boundaryWidth;
    }
    else if (boundaryTypes[0] == BoundaryType::PositiveY || boundaryTypes[0] == BoundaryType::NegativeY)
    {
        x = (rods[rodIds[0]].x + rods[rodIds[1]].x) * 0.5;
        y = rods[rodIds[0]].y + boundaryHeight * 0.5;
        A = abs(rods[rodIds[0]].x - rods[rodIds[1]].x) * boundaryHeight;
    }

    //����ȼ�ϰ��ڸ�ͨ���ڽ����ܳ�
    for (auto& rodId : rodIds)
        circleLength.push_back(rods[rodId].r * PI * 0.5);

    //����һ���߽��ϵ�gap
    addBoundaryGap();

    //����������ȼ�ϰ�gap
    addSingleRodGap(rods[rodIds[0]], boundaryTypes[0]);
    addSingleRodGap(rods[rodIds[1]], boundaryTypes[0]);

    //����һ����ͨgap
    addOrdinaryGap(rods[rodIds[0]], rods[rodIds[1]]);
}

void opensubc::channel::setOrdinaryChannel()//������ͨ��ͨ��
{
    using namespace opensubc::geometry;

    //������ͨ�����ĵ�������������������ܳ�
    for (auto& rodId : rodIds)
    {
        x += rods[rodId].x;
        y += rods[rodId].y;
    }
    x /= 4;
    y /= 4;
    A = 4 * abs(x - rods[rodIds[0]].x) * (y - rods[rodIds[1]].y);

    //����ȼ�ϰ��ڸ�ͨ���ڽ����ܳ�
    for (auto& rodId : rodIds)
        circleLength.push_back(rods[rodId].r * PI * 0.5);

    //�����ĸ���ͨgap
    for (size_t i = 0; i < 3; ++i)
        addOrdinaryGap(rods[rodIds[i]], rods[rodIds[i + 1]]);
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