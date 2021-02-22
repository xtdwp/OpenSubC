#include "calculations.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "capi.h"
#include "initialize.h"
#include <vector>

void opensubc::initialize_calculation()
{
    TiXmlHandle hDoc(&inp);         // hDoc是&inp指向的对象
    TiXmlElement* p0Elem;            // 指向元素的指针
    p0Elem = hDoc.FirstChildElement().Element(); //根节点的指针
    TiXmlHandle hRoot(p0Elem);       // hRoot是根节点
    TiXmlElement* LCHANLElem = hRoot.FirstChild("GeometricParameter").FirstChild("LCHANL").Element(); //当前指向了LCHANL节点
    calculation::length= atof(LCHANLElem->GetText());
    TiXmlElement* NDXElem = hRoot.FirstChild("GeometricParameter").FirstChild("NDX").Element(); 
    calculation::numOfBlocks = atoi(NDXElem->GetText());
    TiXmlElement* TINElem = hRoot.FirstChild("OperatingConditions").FirstChild("TIN").Element();
    calculation::TInitial = atof(TINElem->GetText());
    TiXmlElement* VINElem = hRoot.FirstChild("OperatingConditions").FirstChild("VIN").Element();
    calculation::vInitial = atof(VINElem->GetText());
    TiXmlElement* PINElem = hRoot.FirstChild("OperatingConditions").FirstChild("PIN").Element();
    calculation::pInitial = atof(PINElem->GetText());
    TiXmlElement* TSTEPElem = hRoot.FirstChild("OperatingConditions").FirstChild("TSTEP").Element();
    calculation::tStep = atof(TSTEPElem->GetText());
    TiXmlElement* TENDElem = hRoot.FirstChild("OperatingConditions").FirstChild("TEND").Element();
    calculation::tEnd = atof(TENDElem->GetText());
    TiXmlElement* RODElem = hRoot.FirstChild("GeometricParameter").FirstChild("ROD").Element();
    TiXmlElement* NRODElem = hRoot.FirstChild("GeometricParameter").FirstChild("NROD").Element();
    unsigned NROD= atoi(NRODElem->GetText());
    std::vector<std::vector<double>> q1(NROD,std::vector<double>(calculation::numOfBlocks));//存放面热功率的vector二维数组
    double Q;
    std::vector<double*>q0;        //存放一级指针的vector数组
    int i=0;
    for (RODElem; RODElem; RODElem = RODElem->NextSiblingElement())//遍历所有的燃料棒（要求输入卡按id顺序填写）
    {
        TiXmlHandle ROD(RODElem);
        TiXmlElement* QElem = ROD.FirstChild("Q").Element();
        Q= atof(QElem->GetText());
        for (int j = 0; j < calculation::numOfBlocks; j++)//输入不同轴向分段数的热功率
        {
            q1[i][j] = Q;
        }
        q0.push_back(&q1[i][0]);          
        i++;
    }
    calculation::q = &q0[0];
}

void opensubc::finalize_calculation()
{

}
double opensubc::h(double p, double t) {
    double a[5] = { -1.42101,8.72986,-19.462,11.08749,-0.97826 };
    double b[5] = { 9.67303,-48.26339,104.01501,-59.74097,5.57283 };
    double c[5] = { -18.02611,97.33641,-206.0090,119.21196,-11.64132 };
    double d[5] = { 15.97775,-85.65223,179.22737,-104.40354,10.5892 };
    double e[5] = { -5.19521,27.8447,-57.80601,33.86865,-3.54565 };
    double z, x, y;
    //DataElement temp;
    x = t / 780; y = p / 20; z = 0;
    for (int i = 0; i < 5; i++) {
        z += (a[i] + b[i] * x + c[i] * x * x + d[i] * x * x * x + e[i] * x * x * x * x) * pow(y, i);
    }
    z *= 990;
    return z;
}
double opensubc::t(double p, double h) {
    double a[5] = { -0.87811,4.19127,1.11324,-6.78545,3.22732 };
    double b[5] = { 3.58799,-15.32662,-14.17288,41.41008,-18.53626 };
    double c[5] = { -2.79791,19.82254,41.9763,-91.03666,38.97091 };
    double d[5] = { 1.14141,-9.87656,-47.0195,86.16367,-35.64303 };
    double e[5] = { -0.06412,1.20086,18.13742,-29.79073,11.99363 };
    double z, x, y;
    x = h / 990; y = p / 20; z = 0;
    for (int i = 0; i < 5; i++) {
        z += (a[i] + b[i] * x + c[i] * x * x + d[i] * x * x * x + e[i] * x * x * x * x) * pow(y, i);
    }
    z *= 780;
    return z;
}
double opensubc::rho(double p, double h) {
    double a[6] = { 5.51141,-31.90696,116.86343,-84.53118,-26.86934,29.46078 };
    double b[6] = { -30.68877,177.99507,-597.69806,275.28642,388.4933,-254.14657 };
    double c[6] = { 66.27682,-367.93354,1138.11163,-24.97488,-1538.7021,812.59139 };
    double d[6] = { -69.14811,351.83442,-963.95423,-807.41087,2628.70703,-1232.66205 };
    double e[6] = { 34.62849,-149.98417,317.08842,1018.0708,-2067.58415,899.00831 };
    double f[6] = { -6.57949,20.26986,-10.35475,-376.60235,616.10422,-254.29788 };
    double z, x, y;
    x = h / 990; y = p / 20; z = 0;
    for (int i = 0; i < 6; i++) {
        z += (a[i] + b[i] * x + c[i] * x * x + d[i] * x * x * x + e[i] * x * x * x * x + f[i] * x * x * x * x * x) * pow(y, i);
    }
    z *= 480;
    return z;
}
double opensubc::u(double rho, double t) {
    double a[4] = { -0.07622,1.34854,-0.54067,0.12929 };
    double b[4] = { 0.23975,-0.6812,0.81125,-0.32631 };
    double c[4] = { 0.04682,1.31995,-1.62115,0.69507 };
    double d[4] = { 0.287,-1.11705,1.53517,-0.74812 };
    double z, x, y;
    x = rho / 480; y = t / 780; z = 0;
    for (int i = 0; i < 4; i++) {
        z += (a[i] + b[i] * x + c[i] * x * x + d[i] * x * x * x) * pow(y, i);
    }
    z *= 40;
    return z;
}