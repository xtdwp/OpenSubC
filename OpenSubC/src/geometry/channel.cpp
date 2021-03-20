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
    ////每个燃料棒都增加该通道id信息
    //for (int i = 0; i < 4; ++i)
    //{
    //    geometry::rods[rodIds[i]].channelIds.push_back(id);
    //    circleLength[i] = geometry::rods[rodIds[i]].r * PI * 0.5;
    //}

    //double r = geometry::rods[rodIds[0]].r;
    //unsigned crossId;//跟第一个燃料棒对角线的燃料棒在数组中的位置
    ////计算面积与浸润周长
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
    ////构建gap
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


}

void opensubc::channel::setOrdinaryChannel()//构建普通的通道
{

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