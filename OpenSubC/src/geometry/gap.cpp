#include "gap.h"
#include <geometry.h>
#include <fuelRod.h>

opensubc::gap::gap()//边界上的间隙
{
    //记录边界信息
    boundaryType = BoundaryType::NoBoundary;//完全边界的间隙不需要这些信息，随便赋值即可
}



opensubc::gap::gap(const fuelRod& rod0, const fuelRod& rod1)//根据燃料棒构建间隙（两个燃料棒）
{
    //存储燃料棒id
    rodIds.push_back(rod0.id);
    rodIds.push_back(rod1.id);
    //记录边界信息
    boundaryType = BoundaryType::NoBoundary;
    //计算间隙宽度,需要判断燃料棒相对方位
    sk = sqrt(pow(rod0.x - rod1.x, 2) + pow(rod0.y - rod1.y, 2)) - rod0.r - rod1.r;
}

opensubc::gap::gap(const fuelRod& rod, BoundaryType _boundaryType)//根据燃料棒与间隙宽度构建间隙（一个燃料棒与边界）
{
    //存储燃料棒id
    rodIds.push_back(rod.id);
    //记录边界信息
    boundaryType = _boundaryType;
    //设置间隙宽度
    if (boundaryType == BoundaryType::PositiveX || boundaryType == BoundaryType::NegativeX)
        sk = geometry::boundaryWidth;
    if (boundaryType == BoundaryType::PositiveY || boundaryType == BoundaryType::NegativeY)
        sk = geometry::boundaryHeight;
}

bool opensubc::gap::getOtherChannelId(unsigned channelId, unsigned& otherChannelId)//根据给定的通道id给出该gap连接的另一个通道id
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