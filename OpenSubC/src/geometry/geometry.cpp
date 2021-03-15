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

    //������xml�ļ����ҵ����β���
    TiXmlNode* geometryData = inp.FirstChild("input")->FirstChild("GeometricParameter");

    //��ȡ�߽���
    boundaryHeight = atof(geometryData->FirstChildElement("HBOUND")->GetText());
    boundaryWidth = atof(geometryData->FirstChildElement("WBOUND")->GetText());

    //����ȼ�ϰ����ݲ�����ȼ�ϰ�����
    for (TiXmlNode* rodData = geometryData->IterateChildren("ROD", nullptr); rodData; rodData = geometryData->IterateChildren("ROD", rodData))
    {
        unsigned id = atoi(rodData->FirstChildElement("ID")->GetText());
        double x = atof(rodData->FirstChildElement("X")->GetText());
        double y = atof(rodData->FirstChildElement("Y")->GetText());
        double r = atof(rodData->FirstChildElement("R")->GetText());
        rods.push_back(fuelRod(id, x, y, r));
    }

    //����ͨ����Ϣ������ͨ���������϶���󣨼�϶�ڹ���ͨ��ʱ�Զ����죩
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