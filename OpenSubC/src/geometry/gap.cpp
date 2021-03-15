#include "gap.h"
#include <geometry.h>

unsigned opensubc::gap::num = 0;

opensubc::gap::gap(unsigned rodId0, unsigned rodId1)
{
    id = num++;
    rodId[0] = rodId0;
    rodId[1] = rodId1;
    channelId[0] = channelId[1] = -1;
}

int opensubc::gap::getOtherChannelId(unsigned _channelId)//��ȡgap���������ͨ��id֮�����ӵ���һ��ͨ����id
{
    return _channelId == channelId[0] ? channelId[1] : channelId[0];
}

int opensubc::gap::checkGapExistence(unsigned rodId0, unsigned rodId1)
{
    for (int i = 0; i < geometry::gaps.size(); ++i)
    {
        gap& gap = geometry::gaps[i];
        if ((gap.rodId[0] == rodId0 && gap.rodId[1] == rodId1) || (gap.rodId[0] == rodId1 && gap.rodId[1] == rodId0))
            return i;
    }
    return -1;
}