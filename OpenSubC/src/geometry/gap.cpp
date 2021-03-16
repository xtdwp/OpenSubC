#include "gap.h"
#include <geometry.h>
#include <fuelRod.h>

unsigned opensubc::gap::num = 0;

opensubc::gap::gap()//边界上的间隙
{
    //创建间隙id
    id = num++;
    //记录边界信息
    boundaryType = BoundaryType::NoBoundary;//完全边界的间隙不需要这些信息，随便赋值即可
}

opensubc::gap::gap(const fuelRod& rod0, const fuelRod& rod1)//根据燃料棒构建间隙（两个燃料棒）
{
    //创建间隙id
    id = num++;
    //存储燃料棒id
    rodIds.push_back(rod0.id);
    rodIds.push_back(rod1.id);
    //记录边界信息
    boundaryType = BoundaryType::NoBoundary;
    //计算间隙宽度,需要判断燃料棒相对方位
    sk = abs(rod0.x - rod1.x) - rod0.r - rod1.r;
    if (sk < 0)
        sk = abs(rod0.y - rod1.y) - rod0.r - rod1.r;
}

opensubc::gap::gap(const fuelRod& rod, BoundaryType _boundaryType)//根据燃料棒与间隙宽度构建间隙（一个燃料棒与边界）
{
    //创建间隙id
    id = num++;
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

unsigned opensubc::gap::getOtherChannelId(unsigned channelId)//根据给定的通道id给出该gap连接的另一个通道id
{

}