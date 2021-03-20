#include "channel.h"
#include <geometry.h>
#include <constants.h>
#include <iostream>
#include <fuelRod.h>

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
    A = boundaryHeight * boundaryWidth;//计算通道横截面积
    circleLength.push_back(rod.r * PI * 0.5);//计算燃料棒在该通道内浸润周长

    //创建边界上的两个gap

}

void opensubc::channel::setEdgeChannel()//构建边通道
{

}

void opensubc::channel::setOrdinaryChannel()//构建普通的通道
{

}

opensubc::channel::~channel()
{

}