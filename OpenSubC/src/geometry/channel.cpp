#include "channel.h"
#include <geometry.h>
#include <constants.h>
#include <iostream>
#include <fuelRod.h>
#include <gap.h>

opensubc::channel::channel(unsigned _id, const std::vector<unsigned>& _rodIds, const std::vector<BoundaryType> _boundaryTypes)//给定一定数量燃料棒和边界信息构建单通道
    :id(_id), rodIds(_rodIds), boundaryTypes(_boundaryTypes)
{
    originalId = opensubc::geometry::channelIdConverter[id];
    setChannel();
}

void opensubc::channel::setChannel()//在初始化之后，根据燃料棒位置分布构造子通道几何（目前只支持最简单的矩形情形）
{
    std::cout << rodIds.size() << std::endl;
    if (rodIds.size() == 1)
        setCornerChannel();
    else if (rodIds.size() == 2)
        setEdgeChannel();
    else
        setOrdinaryChannel();
}

void opensubc::channel::setCornerChannel()//构建角通道
{
    using namespace opensubc::geometry;

    fuelRod& rod = rods[rodIds[0]];
    rod.channelIds.push_back(id);//燃料棒增加该通道id

    //计算子通道中心点坐标与横截面积
    switch (boundaryTypes[0])
    {
    case BoundaryType::PositiveX:
        x = rod.x + boundaryWidth * 0.5;
        break;
    case BoundaryType::NegativeX:
        x = rod.x - boundaryWidth * 0.5;
        break;
    case BoundaryType::PositiveY:
        y = rod.y + boundaryHeight * 0.5;
        break;
    case BoundaryType::NegativeY:
        y = rod.y - boundaryHeight * 0.5;
        break;
    default:
        break;
    }
    switch (boundaryTypes[1])
    {
    case BoundaryType::PositiveX:
        x = rod.x + boundaryWidth * 0.5;
        break;
    case BoundaryType::NegativeX:
        x = rod.x - boundaryWidth * 0.5;
        break;
    case BoundaryType::PositiveY:
        y = rod.y + boundaryHeight * 0.5;
        break;
    case BoundaryType::NegativeY:
        y = rod.y - boundaryHeight * 0.5;
        break;
    default:
        break;
    }
    A = boundaryHeight * boundaryWidth;

    //计算燃料棒在该通道内浸润周长
    circleLength.push_back(rod.r * PI * 0.5);

    //创建边界上的两个gap
    addBoundaryGap();
    addBoundaryGap();

    //创建跟燃料棒连接的两个gap(需要判断该间隙是否已经创建)
    addSingleRodGap(rod, boundaryTypes[0]);
    addSingleRodGap(rod, boundaryTypes[1]);
}

void opensubc::channel::setEdgeChannel()//构建边通道
{
    using namespace opensubc::geometry;

    //各燃料棒添加该通道id
    for (auto& rodId : rodIds)
        rods[rodId].channelIds.push_back(id);

    //计算子通道中心点坐标与横截面积及浸润周长
    if (boundaryTypes[0] == BoundaryType::PositiveX || boundaryTypes[0] == BoundaryType::NegativeX)
    {
        if (boundaryTypes[0] == BoundaryType::PositiveX)
            x = rods[rodIds[0]].x + boundaryWidth * 0.5;
        else
            x = rods[rodIds[0]].x - boundaryWidth * 0.5;
        y = (rods[rodIds[0]].y + rods[rodIds[1]].y) * 0.5;
        A = abs(rods[rodIds[0]].y - rods[rodIds[1]].y) * boundaryWidth;
    }
    else if (boundaryTypes[0] == BoundaryType::PositiveY || boundaryTypes[0] == BoundaryType::NegativeY)
    {
        x = (rods[rodIds[0]].x + rods[rodIds[1]].x) * 0.5;
        if (boundaryTypes[0] == BoundaryType::PositiveY)
            y = rods[rodIds[0]].y + boundaryHeight * 0.5;
        else
            y = rods[rodIds[0]].y - boundaryHeight * 0.5;
        A = abs(rods[rodIds[0]].x - rods[rodIds[1]].x) * boundaryHeight;
    }

    //计算燃料棒在该通道内浸润周长
    for (auto& rodId : rodIds)
        circleLength.push_back(rods[rodId].r * PI * 0.5);

    //创建一个边界上的gap
    addBoundaryGap();

    //创建两个单燃料棒gap
    addSingleRodGap(rods[rodIds[0]], boundaryTypes[0]);
    addSingleRodGap(rods[rodIds[1]], boundaryTypes[0]);

    //创建一个普通gap
    addOrdinaryGap(rods[rodIds[0]], rods[rodIds[1]]);
}

void opensubc::channel::setOrdinaryChannel()//构建普通的通道
{
    using namespace opensubc::geometry;

    //各燃料棒添加该通道id
    for (auto& rodId : rodIds)
        rods[rodId].channelIds.push_back(id);

    //计算子通道中心点坐标与横截面积及浸润周长
    for (auto& rodId : rodIds)
    {
        x += rods[rodId].x;
        y += rods[rodId].y;
    }
    x /= 4;
    y /= 4;
    A = 4 * abs(x - rods[rodIds[0]].x) * (y - rods[rodIds[0]].y);

    //计算燃料棒在该通道内浸润周长
    for (auto& rodId : rodIds)
        circleLength.push_back(rods[rodId].r * PI * 0.5);

    std::cout << "here!" << std::endl;

    //创建四个普通gap
    fuelRod& rod0 = rods[rodIds[0]];//参与创建该子通道的第一个燃料棒
    unsigned diagonalRodId;//与第一个燃料棒对角的燃料棒序号（不是燃料棒id）
    //创建第一个燃料棒参与的两个gap
    for (int i = 1; i < 4; ++i)
    {
        fuelRod& rod = rods[rodIds[i]];
        if (abs(rod0.x - rod.x) > rod0.r && abs(rod0.y - rod.y) > rod0.r)//如果对角,记录序号
            diagonalRodId = i;
        else//非对角,创建gap
            addOrdinaryGap(rod0, rod);
    }
    //创建第一个燃料棒对角燃料棒参与的两个gap
    for (int i = 1; i < 4; ++i)
        if (i != diagonalRodId)
            addOrdinaryGap(rods[rodIds[diagonalRodId]], rods[rodIds[i]]);
}

void opensubc::channel::addBoundaryGap()//为该子通道添加一个位于边界的gap
{
    using namespace opensubc::geometry;

    //在间隙数组中增加一个空gap
    gaps.push_back(gap());
    //设置id相关数值
    setIdOfLastGap();
}

void opensubc::channel::addSingleRodGap(const fuelRod& rod, BoundaryType boundaryType)//为该子通道添加一个单燃料棒的gap
{
    using namespace opensubc::geometry;
    //需要判断gap是否已经被创建
    unsigned gapId;
    if (checkGapExistence(rod, boundaryType, gapId))//如果已经被创建
        addExistsGap(gapId);
    else//如果未被创建
    {
        //创建gap并存入数组
        gaps.push_back(gap(rod, boundaryType));
        //设置id相关数值
        setIdOfLastGap();
    }
}

void opensubc::channel::addOrdinaryGap(const fuelRod& rod0, const fuelRod& rod1)//为该子通道添加一个普通的两个燃料棒之间的gap
{
    using namespace opensubc::geometry;
    //需要判断gap是否已经被创建
    unsigned gapId;
    if (checkGapExistence(rod0, rod1, gapId))//如果已经被创建
        addExistsGap(gapId);
    else//如果未被创建
    {
        //创建gap并存入数组
        gaps.push_back(gap(rod0, rod1));
        //设置id相关数值
        setIdOfLastGap();
    }
}

void opensubc::channel::addExistsGap(unsigned gapId)//为该子通道添加一个已经存在的gap
{
    using namespace opensubc::geometry;
    gap& gap = gaps[gapId];

    //计算gap的间隙参数lk
    channel& channel = channels[gap.channelIds[0]];
    gap.lk = sqrt(pow(channel.x - x, 2) + pow(channel.y - y, 2));

    //间隙增加该子通道的id
    gap.channelIds.push_back(id);

    //该子通道增加间隙id
    gapIds.push_back(gapId);
}

void opensubc::channel::setIdOfLastGap()//给gap数组中的最后一个gap增加id相关信息（最后一个说明刚刚创建,需要配套使用）
{
    using namespace opensubc::geometry;
    gap& gap = gaps[gaps.size() - 1];

    //id设置为数组中的位置
    gap.id = gaps.size() - 1;

    //为该gap添加该子通道的id
    gap.channelIds.push_back(id);

    //为该子通道添加新增加的gap的id
    gapIds.push_back(gap.id);

    //为gap关联的燃料棒添加该gap的id
    for (auto& rodId : gap.rodIds)
        rods[rodId].gapIds.push_back(gap.id);
}

opensubc::channel::~channel()
{

}

std::string opensubc::channel::toString()const
{
    //打印开头
    std::string outputString;
    for (int i = 0; i < 10; ++i)
        outputString += "-";
    outputString += "channel";
    for (int i = 0; i < 10; ++i)
        outputString += "-";
    outputString += "\n";

    //打印id及几何信息
    outputString += "id: " + std::to_string(id) + ", originalId: " + std::to_string(originalId) + "\n";
    outputString += "x: " + std::to_string(x) + ", y: " + std::to_string(y) + "\n";
    outputString += "A: " + std::to_string(A) + "\n";

    //打印边界信息
    outputString += "boundaryTypes: ";
    for (auto& boundaryType : boundaryTypes)
        outputString += std::to_string(static_cast<int>(boundaryType)) + ", ";
    outputString.erase(outputString.size() - 2);
    outputString += "\n";

    //打印gap的id
    outputString += "gapIds: ";
    for (auto& gapId : gapIds)
        outputString += std::to_string(gapId) + ", ";
    outputString.erase(outputString.size() - 2);
    outputString += "\n";

    //打印燃料棒的id
    outputString += "rodIds: ";
    for (auto& rodId : rodIds)
        outputString += std::to_string(rodId) + ", ";
    outputString.erase(outputString.size() - 2);
    outputString += "\n";

    //打印燃料棒浸润周长
    outputString += "circleLength: ";
    for (auto& length : circleLength)
        outputString += std::to_string(length) + ", ";
    outputString.erase(outputString.size() - 2);
    outputString += "\n";

    //打印结尾
    for (int i = 0; i < 27; ++i)
        outputString += "-";
    outputString += "\n";

    return outputString;
}