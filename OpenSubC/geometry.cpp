#include "geometry.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "initialize.h"
#include "constants.h"
#include <iostream>
#include <vector>

double opensubc::geometry::boundaryHeight, opensubc::geometry::boundaryWidth;
std::vector<opensubc::fuelRod> opensubc::geometry::rods;
std::vector<opensubc::gap> opensubc::geometry::gaps;
std::vector<opensubc::channel> opensubc::geometry::channels;

unsigned opensubc::fuelRod::num = 0;
unsigned opensubc::gap::num = 0;
unsigned opensubc::channel::num = 0;

//��������λ����뾶���г�ʼ��
opensubc::fuelRod::fuelRod(unsigned _id, double _x, double _y, double _r):id(_id), x(_x), y(_y),r(_r)
{
	++num;
}

opensubc::fuelRod::~fuelRod()
{

}


opensubc::gap::gap(unsigned rodId0, unsigned rodId1)
{
    id = num++;
	rodId[0] = rodId0;
	rodId[1] = rodId1;
}

int opensubc::gap::checkGapExistence(unsigned rodId0, unsigned rodId1)
{
    for (int i = 0; i < geometry::gaps.size(); ++i)
    {
        gap& gap = geometry::gaps[i];
        if ((gap.rodId[0] == rodId0 && gap.rodId[1] == rodId1) || (gap.rodId[0] == rodId1 && gap.rodId[1] == rodId0))
            return i;
    }
    return -1;
}

opensubc::channel::channel(unsigned _id, const std::vector<unsigned>& _rodIds)//����һ������ȼ�ϰ�������ͨ��
{
    ++num;
    id = _id;
    rodIds = _rodIds;
    circleLength.resize(rodIds.size());
    setChannel();
}

void opensubc::channel::setChannel()//�ڳ�ʼ��֮�󣬸���ȼ�ϰ�λ�÷ֲ�������ͨ�����Σ�Ŀǰֻ֧����򵥵ľ������Σ�
{
    //ÿ��ȼ�ϰ������Ӹ�ͨ��id��Ϣ
    for (int i = 0; i < 4; ++i)
        geometry::rods[rodIds[i]].channelIds.push_back(id);

    double r = geometry::rods[rodIds[0]].r;
    unsigned crossId;//����һ��ȼ�ϰ��Խ��ߵ�ȼ�ϰ��������е�λ��
    //�������������ܳ�
    for (int i = 1; i < 4; ++i)
    {
        double dx = abs(geometry::rods[rodIds[0]].x - geometry::rods[rodIds[i]].x);
        double dy = abs(geometry::rods[rodIds[0]].y - geometry::rods[rodIds[i]].y);
        if (dx > r && dy > r)
        {
            A = dx * dy;
            crossId = i;
        }
        circleLength[i] = geometry::rods[rodIds[i]].r * PI * 0.5;
    }

    //����gap
    for (int i = 1; i < 4; ++i)
    {
        if (i == crossId)
            continue;
        int gapId = gap::checkGapExistence(rodIds[0], rodIds[i]);
        if (gapId == -1)
        {
            geometry::gaps.push_back(gap(rodIds[0], rodIds[i]));
            geometry::gaps[geometry::gaps.size() - 1].channelId[0] = id;
            geometry::rods[rodIds[0]].channelIds.push_back(geometry::gaps.size() - 1);
            geometry::rods[rodIds[i]].channelIds.push_back(geometry::gaps.size() - 1);
        }
        else
        {
            geometry::gaps[gapId].channelId[1] = id;
        }

        gapId = gap::checkGapExistence(rodIds[crossId], rodIds[i]);
        if (gapId == -1)
        {
            geometry::gaps.push_back(gap(rodIds[crossId], rodIds[i]));
            geometry::gaps[geometry::gaps.size() - 1].channelId[0] = id;
            geometry::rods[rodIds[crossId]].channelIds.push_back(geometry::gaps.size() - 1);
            geometry::rods[rodIds[i]].channelIds.push_back(geometry::gaps.size() - 1);
        }
        else
        {
            geometry::gaps[gapId].channelId[1] = id;
        }

    }
}

opensubc::channel::~channel()
{

}

void opensubc::initialize_geometry()
{
    TiXmlHandle hDoc(&inp);         // hDoc��&inpָ��Ķ���
    TiXmlElement* p0Elem;            // ָ��Ԫ�ص�ָ��
    p0Elem = hDoc.FirstChildElement().Element(); //���ڵ��ָ��
    TiXmlHandle hRoot(p0Elem);       // hRoot�Ǹ��ڵ�
    TiXmlElement* HBOUNDElem = hRoot.FirstChild("GeometricParameter").FirstChild("HBOUND").Element();
    TiXmlElement* WBOUNDElem = hRoot.FirstChild("GeometricParameter").FirstChild("WBOUND").Element();
    geometry::boundaryHeight= atof(HBOUNDElem->GetText()); 
    geometry::boundaryWidth= atof(WBOUNDElem->GetText());
    std::cout << geometry::boundaryHeight << "  " << geometry::boundaryWidth << std::endl;
    TiXmlElement* RODElem = hRoot.FirstChild("GeometricParameter").FirstChild("ROD").Element(); //��ǰָ���˵�һ��ROD�ڵ�
    unsigned idtemp;
    double xtemp, ytemp,rtemp;
    for (RODElem; RODElem; RODElem = RODElem->NextSiblingElement()) //ȼ�ϰ�����
    {
        TiXmlHandle ROD(RODElem); 
        TiXmlElement* XElem = ROD.FirstChild("X").Element();
        TiXmlElement* YElem = ROD.FirstChild("Y").Element();
        TiXmlElement* RRODElem = ROD.FirstChild("RROD").Element();
        RODElem->QueryUnsignedAttribute("id", &idtemp);
        xtemp = atof(XElem->GetText());
        ytemp = atof(YElem->GetText());
        rtemp = atof(RRODElem->GetText());
        fuelRod rodtemp(idtemp, xtemp, ytemp, rtemp);
        std::cout << idtemp << "  " << xtemp << "  " << ytemp << "  " << rtemp << std::endl;
        geometry::rods.push_back(rodtemp);
    }
    TiXmlElement* CHANLElem = hRoot.FirstChild("GeometricParameter").FirstChild("CHANL").Element();
    for (CHANLElem; CHANLElem; CHANLElem = CHANLElem->NextSiblingElement())   //ͨ������
    {
        TiXmlHandle CHANL(CHANLElem);
        TiXmlElement* RODIDElem = CHANL.FirstChild("RODID").Element();
        RODIDElem->QueryUnsignedAttribute("id", &idtemp);
        unsigned count=0;
        std::vector <unsigned> RODID;//��ͨ�����ӵ�ȼ�ϰ���id������
        for (RODIDElem; RODIDElem; RODIDElem = RODIDElem->NextSiblingElement())
        {  
            RODID.push_back(atoi(RODIDElem->GetText()));
            count++;
        }
        //unsigned* rodIdstemp=&RODID[0];        //ֱ�Ӵ�vector��Ԫ�ص�����
        for (auto id : RODID)
            std::cout << id << " ";
        std::cout << std::endl;
        channel channeltemp(count, RODID);//�������id
        geometry::channels.push_back(channeltemp);
    }
    //gap����δд��ͨ��֮�����ӹ�ϵ��Ҫ�����뿨������
}

void opensubc::finalize_geometry()
{

}