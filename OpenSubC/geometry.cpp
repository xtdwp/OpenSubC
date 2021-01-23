#include "geometry.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "initialize.h"
#include <vector>

opensubc::fuelRod::fuelRod(unsigned _id, double _x, double _y, double _r)//��������λ����뾶���г�ʼ��
{

}

opensubc::fuelRod::~fuelRod()
{

}

opensubc::gap::gap(unsigned rodId0, unsigned rodId2, unsigned channelId0, unsigned channelId1)
{

}

opensubc::channel::channel(int numOfRods, unsigned* rodIds)//����һ������ȼ�ϰ�������ͨ��
{

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
        unsigned* rodIdstemp=&RODID[0];        //ֱ�Ӵ�vector��Ԫ�ص�����
        channel channeltemp(count,rodIdstemp);//�������id
        geometry::channels.push_back(channeltemp);
    }
    //gap����δд��ͨ��֮�����ӹ�ϵ��Ҫ�����뿨������
}

void opensubc::finalize_geometry()
{

}