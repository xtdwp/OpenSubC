#ifndef _InputData_H
#define _InputData_H

#include<vector>

class InputData {
public:
	InputData(const char* inputPath);
	const char* input;
	float TIN, GIN, POUT, QTAB, DROD, DPX, LHEAT, DOG, DIG, DP, SCRUD, SOX, KG, KCRUD, KOX, HGP;
	int NGRID;
	std::vector<float> ZGRID;
	std::vector<float> KGRID;

	bool readXml();
	void coutXml();
};

#endif

