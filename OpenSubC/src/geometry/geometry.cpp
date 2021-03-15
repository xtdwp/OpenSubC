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
    TiXmlHandle hDoc(&inp);         // hDoc是&inp指向的对象
    TiXmlElement* p0Elem;            // 指向元素的指针
    p0Elem = hDoc.FirstChildElement().Element(); //根节点的指针
    TiXmlHandle hRoot(p0Elem);       // hRoot是根节点
    TiXmlElement* HBOUNDElem = hRoot.FirstChild("GeometricParameter").FirstChild("HBOUND").Element();
    TiXmlElement* WBOUNDElem = hRoot.FirstChild("GeometricParameter").FirstChild("WBOUND").Element();
    geometry::boundaryHeight= atof(HBOUNDElem->GetText()); 
    geometry::boundaryWidth= atof(WBOUNDElem->GetText());
    /*std::cout << geometry::boundaryHeight << "  " << geometry::boundaryWidth << std::endl;*/
    TiXmlElement* RODElem = hRoot.FirstChild("GeometricParameter").FirstChild("ROD").Element(); //当前指向了第一个ROD节点
    unsigned idtemp;
    double xtemp, ytemp,rtemp;
    /*int n = 0;*/
    for (RODElem; RODElem; RODElem = RODElem->NextSiblingElement()) //燃料棒部分
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
    for (CHANLElem; CHANLElem; CHANLElem = CHANLElem->NextSiblingElement())   //通道部分
    {
        TiXmlHandle CHANL(CHANLElem);
        TiXmlElement* RODIDElem = CHANL.FirstChild("RODID").Element();
        /*std::cout << "here!" << std::endl;
        if (RODElem == NULL)
            std::cout << "NULL!" << std::endl;*/
        RODIDElem->QueryUnsignedAttribute("id", &idtemp);
        /*std::cout << "here!" << std::endl;*/
        unsigned count=0;
        std::vector <unsigned> RODID;//子通道连接的燃料棒的id的数组
        for (RODIDElem; RODIDElem; RODIDElem = RODIDElem->NextSiblingElement())
        {  
            RODID.push_back(atoi(RODIDElem->GetText()));
            count++;
        }
        //unsigned* rodIdstemp=&RODID[0];        //直接传vector首元素的引用
        /*for (auto id : RODID)
            std::cout << id << " ";
        std::cout << std::endl;*/
        channel channeltemp(count, RODID);//建议添加id
        geometry::channels.push_back(channeltemp);
    }
    //gap部分未写，通道之间连接关系需要在输入卡内完善
}

void opensubc::finalize_geometry()
{

}