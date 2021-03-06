#include "calculations.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "capi.h"
#include "initialize.h"
#include <vector>
#include <iostream>

/**********全局数据存储**********/
        //通道控制体数据
opensubc::StateValue** opensubc::calculation::stateValue;
double** opensubc::calculation::v;
//间隙搅混数据
opensubc::MixValue** opensubc::calculation::mixValue;
//燃料棒发热面功率
double** opensubc::calculation::q;

/**********全局数据存储(向量与矩阵)**********/
Eigen::SparseMatrix<double> P, h, T, rho, m, w, wk;
Eigen::SparseMatrix<double> energyD[3];
std::vector<Eigen::SparseMatrix<double>> energyC;

/**********全局计算设置**********/
double opensubc::calculation::length;//通道长度
int opensubc::calculation::numOfBlocks;//轴向分段数
double opensubc::calculation::pInitial, opensubc::calculation::TInitial, opensubc::calculation::vInitial;//二氧化碳入口初始条件
double opensubc::calculation::tStep, opensubc::calculation::tEnd;//时间步长与计算总时长

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
    calculation::q = &q0[0];
}

void opensubc::initVectors()//初始化各变量向量
{

}

void opensubc::initEnergyEquation()
{

}

void opensubc::initDirectionMatrix()//初始化方向转换矩阵
{
    int n = m.rows();
    typedef Eigen::Triplet<double> T;
    std::vector<T> tripletList0;           //创建三元组
    int v_ij ;
    for (int i = 0; i < n; i++) { //遍历行，赋值
        if (m.coeffRef(i, 0) > 0)
        {
            v_ij = 1;
        }
        else
        {
            v_ij = 0;
        }
        tripletList0.push_back(T(i, i, v_ij));
    };
    energyD[0].resize(n, n);         
    energyD[0].setFromTriplets(tripletList0.begin(), tripletList0.end());     //从三元组填充到矩阵


}

void opensubc::initConnectMatrix()//初始化连接矩阵
{

}


void opensubc::initAxialMomentumEquation()
{

}

void opensubc::initCrossMomentumEquation()
{

}

void opensubc::initMassEquation()
{

}

void opensubc::finalize_calculation()
{

}