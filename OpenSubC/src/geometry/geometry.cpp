#include <iostream>

#include "initialize.h"
#include "constants.h"
#include "geometry.h"
#include "tinystr.h"
#include "tinyxml.h"

double opensubc::geometry::boundaryHeight, opensubc::geometry::boundaryWidth;
std::vector<opensubc::fuelRod> opensubc::geometry::rods;
std::vector<opensubc::gap> opensubc::geometry::gaps;
std::vector<opensubc::channel> opensubc::geometry::channels;

void opensubc::initialize_geometry()
{
    using namespace opensubc::geometry;

    //从整个xml文件中找到几何部分
    TiXmlNode* geometryData = inp.FirstChild("input")->FirstChild("GeometricParameter");

    //读取边界宽度
    boundaryHeight = atof(geometryData->FirstChildElement("HBOUND")->GetText());
    boundaryWidth = atof(geometryData->FirstChildElement("WBOUND")->GetText());

    //遍历燃料棒数据并构建燃料棒对象
    for (TiXmlNode* rodData = geometryData->IterateChildren("ROD", nullptr); rodData; rodData = geometryData->IterateChildren("ROD", rodData))
    {
        unsigned id = atoi(rodData->FirstChildElement("ID")->GetText());
        double x = atof(rodData->FirstChildElement("X")->GetText());
        double y = atof(rodData->FirstChildElement("Y")->GetText());
        double r = atof(rodData->FirstChildElement("R")->GetText());
        rods.push_back(fuelRod(id, x, y, r));
    }

    //遍历通道信息并构建通道对象与间隙对象（间隙在构造通道时自动构造）
    for (TiXmlNode* channelData = geometryData->IterateChildren("CHANL", nullptr); channelData; channelData = geometryData->IterateChildren("CHNAL", channelData))
    {
        unsigned id = atoi(channelData->FirstChildElement("ID")->GetText());
        std::vector<unsigned> rodIds;
        TiXmlNode* rodId = nullptr;
        while (rodId = channelData->IterateChildren("RODID", rodId))
            rodIds.push_back(atoi(rodId->ToElement()->GetText()));
        channels.push_back(channel(id, rodIds));
    }
}

void opensubc::finalize_geometry()
{

}