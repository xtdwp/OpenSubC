#include <iostream>

#include "initialize.h"
#include "constants.h"
#include "geometry.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "fuelRod.h"
#include "gap.h"
#include "channel.h"

namespace opensubc {
    namespace geometry {
        double boundaryHeight, boundaryWidth;
        std::vector<opensubc::fuelRod> rods;
        std::vector<opensubc::gap> gaps;
        std::vector<opensubc::channel> channels;
    }
}

void opensubc::initialize_geometry()
{
    using namespace opensubc::geometry;

    //从整个xml文件中找到几何部分
    TiXmlNode* geometryData = inp.FirstChild("input")->FirstChild("GeometricParameter");

    //读取边界宽度
    boundaryHeight = atof(geometryData->FirstChildElement("HBOUND")->GetText());
    boundaryWidth = atof(geometryData->FirstChildElement("WBOUND")->GetText());

    //遍历燃料棒数据并构建燃料棒对象
    TiXmlNode* rodData = nullptr;
    while (rodData = geometryData->IterateChildren("ROD", rodData))
    {
        unsigned id = atoi(rodData->FirstChildElement("ID")->GetText());
        double x = atof(rodData->FirstChildElement("X")->GetText());
        double y = atof(rodData->FirstChildElement("Y")->GetText());
        double r = atof(rodData->FirstChildElement("R")->GetText());
        rods.push_back(fuelRod(id, x, y, r));
    }

    //遍历通道信息并构建通道对象与间隙对象（间隙在构造通道时自动构造）
    TiXmlNode* channelData = nullptr;
    while (channelData = geometryData->IterateChildren("CHNAL", channelData))
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

bool opensubc::checkGapExistence(const fuelRod& rod, BoundaryType boundaryType, unsigned& gapId)//查找该gap是否存在
{
    using namespace opensubc::geometry;
    for (auto& gap : gaps)
        if (gap.rodIds.size() == 1 && gap.rodIds[0] == rod.id && gap.boundaryType == boundaryType)
        {
            gapId = gap.id;
            return true;
        }
    return false;
}

bool opensubc::checkGapExistence(const fuelRod& rod0, const fuelRod& rod1, unsigned& gapId)//查找该gap是否存在
{
    using namespace opensubc::geometry;
    for (auto& gap : gaps)
        if (gap.rodIds.size() == 2 && (
            (gap.rodIds[0] == rod0.id && gap.rodIds[1] == rod1.id)|| 
            (gap.rodIds[0] == rod1.id && gap.rodIds[1] == rod0.id)))
        {
            gapId = gap.id;
            return true;
        }
    return false;
}