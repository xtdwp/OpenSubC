#include "calculations.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "capi.h"
#include "initialize.h"
#include "geometry.h"
#include <vector>
#include <iostream>
#include <channel.h>
#include <gap.h>
#include <EnergyEquation.h>
#include <AxialMomentumEquation.h>
#include <CrossMomentumEquation.h>
#include <MassEquation.h>

namespace opensubc {

    namespace calculation {

        /**********全局数据存储(向量与矩阵)**********/
        Eigen::SparseVector<double> P, h, T, rho, m, w, wTurbulence, k, q,f,K;//最后两项为轴向摩擦压降和局部压降系数
        Eigen::SparseVector<double> Pn, hn, Tn, rhon, mn, wn, wTurbulencen, kn, qn;//储存上个时间步长的计算结果

        /**********全局计算设置**********/
        double length;//通道长度
        int numOfBlocks;//轴向分段数
        int numOfChannelData, numOfGapData, numOfRodData;//通道数据向量总长度,gap数据向量总长度与燃料棒数据向量总长度
        double pInitial, TInitial, vInitial;//二氧化碳入口初始条件
        double tStep, tEnd;//时间步长与计算总时长
        double GT;//热导率几何因子
        double KG;//横向流动压降因子
        double fT;//横向动量因子
        double theta;//通道与竖直方向的夹角（角度值）
    }
}

void opensubc::initialize_calculation()
{
    read_calculation_xml();
    initVectors();//初始化各变量向量
    initEnergyEquation();//初始化能量方程所需矩阵
    initAxialMomentumEquation();//初始化轴向动量方程所需矩阵
    initCrossMomentumEquation();//初始化横向动量方程所需矩阵
    initMassEquation();//初始化质量守恒方程所需矩阵
}

void opensubc::read_calculation_xml()
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
    std::cout << "before loop" << std::endl;
    for (RODElem; RODElem; RODElem = RODElem->NextSiblingElement())//遍历所有的燃料棒（要求输入卡按id顺序填写）
    {
        TiXmlHandle ROD(RODElem);
        TiXmlElement* QElem = ROD.FirstChild("Q").Element();
        if (QElem == NULL)
        {
            std::cout << "input end" << std::endl;
            break;
        }
        Q= atof(QElem->GetText());
        for (int j = 0; j < calculation::numOfBlocks; j++)//输入不同轴向分段数的热功率
        {
            q1[i][j] = Q;
        }
        q0.push_back(&q1[i][0]);          
        i++;
    }
    //calculation::q = &q0[0];
}

void opensubc::initVectors()//初始化各变量向量
{

}

//void opensubc::initAxialMomentumEquation()
//{

//}

//void opensubc::initCrossMomentumEquation()
//{

//}

//void opensubc::initMassEquation()
//{

//}

void opensubc::finalize_calculation()
{

}