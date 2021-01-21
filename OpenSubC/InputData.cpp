#include "InputData.h"
#include "tinystr.h"
#include "tinyxml.h"
#include <iostream>
#include<vector>
#include<string>
#include<stdlib.h>


InputData::InputData(const char* inputPath):input (inputPath)
{
	TIN = 0; GIN = 0; POUT = 0; QTAB = 0; DROD = 0; DPX = 0; LHEAT = 0; DOG = 0;
	DIG = 0; DP = 0; SCRUD = 0; SOX = 0; KG = 0; KCRUD = 0; KOX = 0; HGP = 0; NGRID = 0;
	
};
bool InputData::readXml()
{
	float temp1, temp2;
	TiXmlDocument doc;
	if (!doc.LoadFile(input)) return false;
	TiXmlHandle hDoc(&doc);         // hDoc��&docָ��Ķ���
	TiXmlElement* p0Elem;            // ָ��Ԫ�ص�ָ��
	p0Elem = hDoc.FirstChildElement().Element(); //���ڵ��ָ��
	TiXmlHandle hRoot(p0Elem);       // hRoot�Ǹ��ڵ�
	TiXmlElement* p1Elem = hRoot.FirstChild("OperatingConditions").FirstChild("TIN").Element(); // ָ����Tin�ڵ�
	TIN = atof(p1Elem->GetText());
	p1Elem = hRoot.FirstChild("OperatingConditions").FirstChild("GIN").Element();
	GIN = atof(p1Elem->GetText());
	p1Elem = hRoot.FirstChild("OperatingConditions").FirstChild("POUT").Element();
	POUT = atof(p1Elem->GetText());
	p1Elem = hRoot.FirstChild("OperatingConditions").FirstChild("QTAB").Element();
	QTAB = atof(p1Elem->GetText());
	p1Elem = hRoot.FirstChild("GeometricParameter").FirstChild("DROD").Element();
	DROD = atof(p1Elem->GetText());
	p1Elem = hRoot.FirstChild("GeometricParameter").FirstChild("DPX").Element();
	DPX = atof(p1Elem->GetText());
	p1Elem = hRoot.FirstChild("GeometricParameter").FirstChild("LHEAT").Element();
	LHEAT = atof(p1Elem->GetText());
	p1Elem = hRoot.FirstChild("FuelHeatingModels").FirstChild("DOG").Element();
	DOG = atof(p1Elem->GetText());
	p1Elem = hRoot.FirstChild("FuelHeatingModels").FirstChild("DIG").Element();
	DIG = atof(p1Elem->GetText());
	p1Elem = hRoot.FirstChild("FuelHeatingModels").FirstChild("DP").Element();
	DP = atof(p1Elem->GetText());
	p1Elem = hRoot.FirstChild("FuelHeatingModels").FirstChild("SCRUD").Element();
	SCRUD = atof(p1Elem->GetText());
	p1Elem = hRoot.FirstChild("FuelHeatingModels").FirstChild("SOX").Element();
	SOX = atof(p1Elem->GetText());
	p1Elem = hRoot.FirstChild("FuelHeatingModels").FirstChild("KG").Element();
	KG = atof(p1Elem->GetText());
	p1Elem = hRoot.FirstChild("FuelHeatingModels").FirstChild("KCRUD").Element();
	KCRUD = atof(p1Elem->GetText());
	p1Elem = hRoot.FirstChild("FuelHeatingModels").FirstChild("KOX").Element();
	KOX = atof(p1Elem->GetText());
	p1Elem = hRoot.FirstChild("FuelHeatingModels").FirstChild("HGP").Element();
	HGP = atof(p1Elem->GetText());
	p1Elem = hRoot.FirstChild("GeometricParameter").FirstChild("NGRID").Element();
	NGRID = atoi(p1Elem->GetText());
	TiXmlElement* GRIDElem = hRoot.FirstChild("GeometricParameter").FirstChild("GRID").Element();
	for (GRIDElem; GRIDElem; GRIDElem = GRIDElem->NextSiblingElement()) {
		TiXmlHandle GRID(GRIDElem);
		TiXmlElement* ZGRIDElem = GRID.FirstChild("ZGRID").Element();
		temp1 = atof(ZGRIDElem->GetText());
		ZGRID.push_back(temp1);
		TiXmlElement* KGRIDElem = GRID.FirstChild("KGRID").Element();
		temp2 = atof(KGRIDElem->GetText());
		KGRID.push_back(temp2);
	}
	return true;
}
	void InputData::coutXml()
	{
		using namespace std;
		cout << "OperatingConditions" << endl << "Tin=" << TIN << "  GIN=" << GIN << "  POUT=" << POUT << "  QTAB=" << QTAB << endl
			<< "GeometricParameter" << endl << "DROD=" << DROD << "  DPX=" << DPX << "  LHEAT=" << LHEAT<<endl<<"NGRID="<<NGRID<<endl;
		for (size_t i = 0; i<ZGRID.size(); i++) {
			cout << "ZGRID=" << ZGRID[i] << "  KGRID=" << KGRID[i] << endl;
		}
		cout << "FuelHeatingModels" << endl << "DOG=" << DOG << "  DIG=" << DIG << "  DP=" << DP << "  SCRUD=" << SCRUD << endl <<
			"SOX=" << SOX << "  KG=" << KG << "  KCRUD=" << KCRUD << "  KOX=" << KOX << "  HGP=" << HGP;
	}

