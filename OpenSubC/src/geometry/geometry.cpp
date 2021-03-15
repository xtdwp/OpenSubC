#include "geometry.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "initialize.h"
#include "constants.h"
#include "gap.h"
#include <iostream>
#include <vector>
#include <fuelRod.h>

double opensubc::geometry::boundaryHeight, opensubc::geometry::boundaryWidth;
std::vector<opensubc::fuelRod> opensubc::geometry::rods;
std::vector<opensubc::gap> opensubc::geometry::gaps;
std::vector<opensubc::channel> opensubc::geometry::channels;

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
    /*std::cout << geometry::boundaryHeight << "  " << geometry::boundaryWidth << std::endl;*/
    TiXmlElement* RODElem = hRoot.FirstChild("GeometricParameter").FirstChild("ROD").Element(); //��ǰָ���˵�һ��ROD�ڵ�
    unsigned idtemp;
    double xtemp, ytemp,rtemp;
    /*int n = 0;*/
    for (RODElem; RODElem; RODElem = RODElem->NextSiblingElement()) //ȼ�ϰ�����
    {
        /*std::cout << n << std::endl;*/
        TiXmlHandle ROD(RODElem); 
        TiXmlElement* XElem = ROD.FirstChild("X").Element();
        if (XElem == NULL)
            break;
        TiXmlElement* YElem = ROD.FirstChild("Y").Element();
        TiXmlElement* RRODElem = ROD.FirstChild("RROD").Element();
        RODElem->QueryUnsignedAttribute("id", &idtemp);
        xtemp = atof(XElem->GetText());
        ytemp = atof(YElem->GetText());
        rtemp = atof(RRODElem->GetText());
        fuelRod rodtemp(idtemp, xtemp, ytemp, rtemp);
        /*std::cout << idtemp << "  " << xtemp << "  " << ytemp << "  " << rtemp << std::endl;*/
        geometry::rods.push_back(rodtemp);
        /*std::cout << n++ << std::endl;*/
    }
    /*std::cout << "here!" << std::endl;*/
    TiXmlElement* CHANLElem = hRoot.FirstChild("GeometricParameter").FirstChild("CHANL").Element();
    /*std::cout << "here!" << std::endl;*/
    for (CHANLElem; CHANLElem; CHANLElem = CHANLElem->NextSiblingElement())   //ͨ������
    {
        TiXmlHandle CHANL(CHANLElem);
        TiXmlElement* RODIDElem = CHANL.FirstChild("RODID").Element();
        /*std::cout << "here!" << std::endl;
        if (RODElem == NULL)
            std::cout << "NULL!" << std::endl;*/
        RODIDElem->QueryUnsignedAttribute("id", &idtemp);
        /*std::cout << "here!" << std::endl;*/
        unsigned count=0;
        std::vector <unsigned> RODID;//��ͨ�����ӵ�ȼ�ϰ���id������
        for (RODIDElem; RODIDElem; RODIDElem = RODIDElem->NextSiblingElement())
        {  
            RODID.push_back(atoi(RODIDElem->GetText()));
            count++;
        }
        //unsigned* rodIdstemp=&RODID[0];        //ֱ�Ӵ�vector��Ԫ�ص�����
        /*for (auto id : RODID)
            std::cout << id << " ";
        std::cout << std::endl;*/
        channel channeltemp(count, RODID);//�������id
        geometry::channels.push_back(channeltemp);
    }
    //gap����δд��ͨ��֮�����ӹ�ϵ��Ҫ�����뿨������
}

void opensubc::finalize_geometry()
{

}