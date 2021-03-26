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
    s = sqrt(pow(rod0.x - rod1.x, 2) + pow(rod0.y - rod1.y, 2)) - rod0.r - rod1.r;
}

opensubc::gap::gap(const fuelRod& rod, BoundaryType _boundaryType)//根据燃料棒与间隙宽度构建间隙（一个燃料棒与边界）
{
    //存储燃料棒id
    rodIds.push_back(rod.id);
    //记录边界信息
    boundaryType = _boundaryType;
    //设置间隙宽度
    if (boundaryType == BoundaryType::PositiveX || boundaryType == BoundaryType::NegativeX)
        s = geometry::boundaryWidth;
    if (boundaryType == BoundaryType::PositiveY || boundaryType == BoundaryType::NegativeY)
        s = geometry::boundaryHeight;
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

std::string opensubc::gap::toString()const
{
    //打印开头
    std::string outputString;
    for (int i = 0; i < 10; ++i)
        outputString += "-";
    outputString += "gap";
    for (int i = 0; i < 10; ++i)
        outputString += "-";
    outputString += "\n";

    //打印id及边界信息
    outputString += "id: " + std::to_string(id) + "\n";
    outputString += "boundaryType: " + std::to_string(static_cast<int>(boundaryType)) + "\n";

    //打印燃料棒的id
    outputString += "rodIds: ";
    for (auto& rodId : rodIds)
        outputString += std::to_string(rodId) + ", ";
    outputString.erase(outputString.size() - 2);
    outputString += "\n";

    //打印子通道id
    outputString += "channelIds: ";
    for (auto& channelId : channelIds)
        outputString += std::to_string(channelId) + ", ";
    outputString.erase(outputString.size() - 2);
    outputString += "\n";

    //打印结尾
    for (int i = 0; i < 23; ++i)
        outputString += "-";
    outputString += "\n";

    return outputString;
}