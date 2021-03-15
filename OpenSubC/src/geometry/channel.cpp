#include "channel.h"
#include <geometry.h>
#include <constants.h>

unsigned opensubc::channel::num = 0;

opensubc::channel::channel(unsigned _id, const std::vector<unsigned>& _rodIds)//����һ������ȼ�ϰ�������ͨ��
{
    ++num;
    id = _id;
    rodIds = _rodIds;
    circleLength.resize(rodIds.size());
    setChannel();
}

void opensubc::channel::setChannel()//�ڳ�ʼ��֮�󣬸���ȼ�ϰ�λ�÷ֲ�������ͨ�����Σ�Ŀǰֻ֧����򵥵ľ������Σ�
{
    //ÿ��ȼ�ϰ������Ӹ�ͨ��id��Ϣ
    for (int i = 0; i < 4; ++i)
    {
        geometry::rods[rodIds[i]].channelIds.push_back(id);
        circleLength[i] = geometry::rods[rodIds[i]].r * PI * 0.5;
    }

    double r = geometry::rods[rodIds[0]].r;
    unsigned crossId;//����һ��ȼ�ϰ��Խ��ߵ�ȼ�ϰ��������е�λ��
    //�������������ܳ�
    for (int i = 1; i < 4; ++i)
    {
        double dx = abs(geometry::rods[rodIds[0]].x - geometry::rods[rodIds[i]].x);
        double dy = abs(geometry::rods[rodIds[0]].y - geometry::rods[rodIds[i]].y);
        if (dx > r && dy > r)
        {
            A = dx * dy;
            crossId = i;
        }
    }
    A -= PI * r * r;
    //����gap
    for (int i = 1; i < 4; ++i)
    {
        if (i == crossId)
            continue;
        int gapId = gap::checkGapExistence(rodIds[0], rodIds[i]);
        if (gapId == -1)
        {
            geometry::gaps.push_back(gap(rodIds[0], rodIds[i]));
            geometry::gaps[geometry::gaps.size() - 1].channelId[0] = id;
            geometry::rods[rodIds[0]].gapIds.push_back(geometry::gaps.size() - 1);
            geometry::rods[rodIds[i]].gapIds.push_back(geometry::gaps.size() - 1);
        }
        else
        {
            geometry::gaps[gapId].channelId[1] = id;
        }

        gapId = gap::checkGapExistence(rodIds[crossId], rodIds[i]);
        if (gapId == -1)
        {
            geometry::gaps.push_back(gap(rodIds[crossId], rodIds[i]));
            geometry::gaps[geometry::gaps.size() - 1].channelId[0] = id;
            geometry::rods[rodIds[crossId]].gapIds.push_back(geometry::gaps.size() - 1);
            geometry::rods[rodIds[i]].gapIds.push_back(geometry::gaps.size() - 1);
        }
        else
        {
            geometry::gaps[gapId].channelId[1] = id;
        }

    }
}

opensubc::channel::~channel()
{

}