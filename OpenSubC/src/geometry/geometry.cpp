#include <iostream>
#include <algorithm>

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

        std::vector<unsigned> rodIdConverter, channelIdConverter;
    }
}

void opensubc::initialize_geometry()
{
    using namespace opensubc::geometry;

    //������xml�ļ����ҵ����β���
    TiXmlNode* geometryData = inp.FirstChild("input")->FirstChild("GeometricParameter");

    //��ȡ�߽���
    boundaryHeight = atof(geometryData->FirstChildElement("HBOUND")->GetText());
    boundaryWidth = atof(geometryData->FirstChildElement("WBOUND")->GetText());

    //����ȼ�ϰ����ݲ�����ȼ�ϰ�����
    TiXmlNode* rodData = nullptr;
    while (rodData = geometryData->IterateChildren("ROD", rodData))
    {
        //�洢���뿨�����id
        unsigned id = atoi(rodData->FirstChildElement("ID")->GetText());
        rodIdConverter.push_back(id);

        //��ȡȼ�ϰ�������Ϣ
        double x = atof(rodData->FirstChildElement("X")->GetText());
        double y = atof(rodData->FirstChildElement("Y")->GetText());
        double r = atof(rodData->FirstChildElement("R")->GetText());

        //����ȼ�ϰ�����������
        rods.push_back(fuelRod(rodIdConverter.size() - 1, x, y, r));
    }

    //����ͨ����Ϣ������ͨ���������϶���󣨼�϶�ڹ���ͨ��ʱ�Զ����죩
    TiXmlNode* channelData = nullptr;
    while (channelData = geometryData->IterateChildren("CHANL", channelData))
    {
        //�洢���뿨�����id
        unsigned id = atoi(channelData->FirstChildElement("ID")->GetText());
        channelIdConverter.push_back(id);

        std::cout << "id: " << id << std::endl;

        //��ȡ���뿨��������ɸ���ͨ����ȼ�ϰ����
        std::vector<unsigned> rodIds;
        TiXmlNode* rodId = nullptr;
        while (rodId = channelData->IterateChildren("RODID", rodId))
            rodIds.push_back(atoi(rodId->ToElement()->GetText()));

        //�����뿨�����ȼ�ϰ�idת��Ϊʵ��ʹ�õ�ȼ�ϰ�id
        for (auto& rodId : rodIds)
            rodId = std::find(rodIdConverter.begin(), rodIdConverter.end(), rodId) - rodIdConverter.begin();

        //��ȡ���뿨�����ı߽���Ϣ
        std::vector<BoundaryType> boundaryTypes;
        TiXmlNode* boundaryType = nullptr;
        while (boundaryType = channelData->IterateChildren("BOUNDARYTYPE", boundaryType))
            boundaryTypes.push_back(static_cast<BoundaryType>(atoi(boundaryType->ToElement()->GetText())));

        //������ͨ������������
        channels.push_back(channel(channelIdConverter.size() - 1, rodIds, boundaryTypes));
    }
}

void opensubc::finalize_geometry()
{

}

bool opensubc::checkGapExistence(const fuelRod& rod, BoundaryType boundaryType, unsigned& gapId)//���Ҹ�gap�Ƿ����
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

bool opensubc::checkGapExistence(const fuelRod& rod0, const fuelRod& rod1, unsigned& gapId)//���Ҹ�gap�Ƿ����
{
    using namespace opensubc::geometry;
    for (auto& gap : gaps)
        if (gap.rodIds.size() == 2 && (
            (gap.rodIds[0] == rod0.id && gap.rodIds[1] == rod1.id)|| 
            (gap.rodIds[1] == rod0.id && gap.rodIds[0] == rod1.id)))
        {
            gapId = gap.id;
            return true;
        }
    return false;
}