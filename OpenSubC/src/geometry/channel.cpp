#include "channel.h"
#include <geometry.h>
#include <constants.h>
#include <iostream>
#include <fuelRod.h>
#include <gap.h>

opensubc::channel::channel(unsigned _id, const std::vector<unsigned>& _rodIds, const std::vector<BoundaryType> _boundaryTypes)//给定一定数量燃料棒和边界信息构建单通道
    :id(_id), rodIds(_rodIds), boundaryTypes(_boundaryTypes)
{
    circleLength.resize(rodIds.size());
    setChannel();
}

void opensubc::channel::setChannel()//在初始化之后，根据燃料棒位置分布构造子通道几何（目前只支持最简单的矩形情形）
{
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
    x = rod.x + boundaryWidth * 0.5;
    y = rod.y + boundaryHeight * 0.5;
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

    //计算子通道中心点坐标与横截面积及浸润周长
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

    //计算子通道中心点坐标与横截面积及浸润周长
    for (auto& rodId : rodIds)
    {
        x += rods[rodId].x;
        y += rods[rodId].y;
    }
    x /= 4;
    y /= 4;
    A = 4 * abs(x - rods[rodIds[0]].x) * (y - rods[rodIds[1]].y);

    //计算燃料棒在该通道内浸润周长
    for (auto& rodId : rodIds)
        circleLength.push_back(rods[rodId].r * PI * 0.5);

    //创建四个普通gap
    for (size_t i = 0; i < 3; ++i)
        addOrdinaryGap(rods[rodIds[i]], rods[rodIds[i + 1]]);
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
    //id设置为数组中的位置
    gaps.end()->id = gaps.size() - 1;

    //为该gap添加该子通道的id
    gaps.end()->channelIds.push_back(id);

    //为该子通道添加新增加的gap的id
    gaps.end()->channelIds.push_back(id);
}

opensubc::channel::~channel()
{

}