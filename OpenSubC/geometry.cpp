#include "geometry.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "initialize.h"
#include <vector>

opensubc::fuelRod::fuelRod(unsigned _id, double _x, double _y, double _r)//给定坐标位置与半径进行初始化
{

}

opensubc::fuelRod::~fuelRod()
{

}

opensubc::gap::gap(unsigned rodId0, unsigned rodId2, unsigned channelId0, unsigned channelId1)
{

}

opensubc::channel::channel(int numOfRods, unsigned* rodIds)//给定一定数量燃料棒构建单通道
{

}

opensubc::channel::~channel()
{

}

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
    TiXmlElement* RODElem = hRoot.FirstChild("GeometricParameter").FirstChild("ROD").Element(); //当前指向了第一个ROD节点
    unsigned idtemp;
    double xtemp, ytemp,rtemp;
    for (RODElem; RODElem; RODElem = RODElem->NextSiblingElement()) //燃料棒部分
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
        geometry::rods.push_back(rodtemp);
    }
    TiXmlElement* CHANLElem = hRoot.FirstChild("GeometricParameter").FirstChild("CHANL").Element();
    for (CHANLElem; CHANLElem; CHANLElem = CHANLElem->NextSiblingElement())   //通道部分
    {
        TiXmlHandle CHANL(CHANLElem);
        TiXmlElement* RODIDElem = CHANL.FirstChild("RODID").Element();
        RODIDElem->QueryUnsignedAttribute("id", &idtemp);
        unsigned count=0;
        std::vector <unsigned> RODID;//子通道连接的燃料棒的id的数组
        for (RODIDElem; RODIDElem; RODIDElem = RODIDElem->NextSiblingElement())
        {  
            RODID.push_back(atoi(RODIDElem->GetText()));
            count++;
        }
        unsigned* rodIdstemp=&RODID[0];        //直接传vector首元素的引用
        channel channeltemp(count,rodIdstemp);//建议添加id
        geometry::channels.push_back(channeltemp);
    }
    //gap部分未写，通道之间连接关系需要在输入卡内完善
}

void opensubc::finalize_geometry()
{

}