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

/**********全局数据存储**********/
        //通道控制体数据
opensubc::StateValue** opensubc::calculation::stateValue;
double** opensubc::calculation::v;
//间隙搅混数据
opensubc::MixValue** opensubc::calculation::mixValue;
//燃料棒发热面功率
double** opensubc::calculation::q;
//通道间导热因子
double opensubc::calculation::GT;

/**********全局数据存储(向量与矩阵)**********/
Eigen::SparseMatrix<double> opensubc::calculation::P, opensubc::calculation::h, opensubc::calculation::T, opensubc::calculation::rho, opensubc::calculation::m, opensubc::calculation::w, opensubc::calculation::wTurbulence;
Eigen::SparseMatrix<double> opensubc::calculation::AXt; //（AXt，即AiXj / dt）
Eigen::SparseMatrix<double> opensubc::calculation::L, opensubc::calculation::H;   //降编号矩阵
Eigen::SparseMatrix<double> opensubc::calculation::energyD, opensubc::calculation::energyC;
std::vector<Eigen::SparseMatrix<double>> opensubc::calculation::energyE[2];
std::vector<Eigen::SparseMatrix<double>> opensubc::calculation::energyCs;

/**********全局计算设置**********/
double opensubc::calculation::length;//通道长度
int opensubc::calculation::numOfBlocks;//轴向分段数
int opensubc::calculation::numOfChannelData, opensubc::calculation::numOfGapData;//通道数据向量总长度与gap数据向量总长度
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
    int n = calculation::numOfChannelData;
    typedef Eigen::Triplet<double> T;
    std::vector<T> tripletList0;           //创建三元组
    int v_ij ;
    for (int i = 0; i < n; i++) { //遍历行，赋值
        if (calculation::m.coeffRef(i, 0) > 0)
        {
            v_ij = 1;
        }
        else
        {
            v_ij = 0;
        }
        tripletList0.push_back(T(i, i, v_ij));
    };
    calculation::energyD.resize(n, n);
    calculation::energyD.setFromTriplets(tripletList0.begin(), tripletList0.end());     //从三元组填充到矩阵
}

void opensubc::initGeometryMatrix()//初始化几何矩阵（AXt，即AiXj/dt）
{
    calculation::AXt.resize(calculation::numOfChannelData, calculation::numOfChannelData);
    for (auto& channel : geometry::channels)
    {
        for (size_t i = 1; i <= calculation::numOfBlocks; ++i)
            calculation::AXt.insert(channel.id * (calculation::numOfBlocks + (long long)1) + i, channel.id * (calculation::numOfBlocks + (long long)1) + i) = channel.A * calculation::length / calculation::numOfBlocks / calculation::tStep;
    }
}

void opensubc::initConnectMatrix()//初始化连接矩阵
{
    calculation::energyC.resize(calculation::numOfChannelData, calculation::numOfChannelData);
    for (auto& channel : geometry::channels)
    {
        int connectedChannelId;
        for (unsigned gapid : channel.gapIds)
        {
            connectedChannelId = geometry::gaps[gapid].getOtherChannelId(channel.id);
            if (connectedChannelId == -1) continue;
            for (size_t i = 0; i <= calculation::numOfBlocks; ++i)
                calculation::energyC.insert(channel.id * (calculation::numOfBlocks + (long long)1) + i, connectedChannelId * (calculation::numOfBlocks + (long long)1) + i) = 1;
        }
    }
}

void opensubc::initCrossDirectionMatrix()//初始化横向流量方向矢量（e）矩阵
{
    calculation::energyE[0].resize(calculation::numOfChannelData);
    calculation::energyE[1].resize(calculation::numOfChannelData);
    for (auto& matrix : calculation::energyE[0])
        matrix.resize(calculation::numOfGapData, calculation::numOfChannelData);
    for (auto& matrix : calculation::energyE[1])
        matrix.resize(calculation::numOfGapData, calculation::numOfChannelData);
    for (auto& channel : geometry::channels)
    {
        int connectedChannelId;
        for (unsigned gapid : channel.gapIds)
        {
            connectedChannelId = geometry::gaps[gapid].getOtherChannelId(channel.id);
            if (connectedChannelId == -1) continue;
            for (size_t i = 0; i <= calculation::numOfBlocks; ++i)
            {
                calculation::energyE[0][channel.id * (calculation::numOfBlocks + (long long)1) + i].insert(connectedChannelId * (calculation::numOfBlocks + (long long)1) + i, gapid * (calculation::numOfBlocks + (long long)1) + i) = channel.id > connectedChannelId ? 1 : -1;
                calculation::energyE[1][channel.id * (calculation::numOfBlocks + (long long)1) + i].insert(connectedChannelId * (calculation::numOfBlocks + (long long)1) + i, gapid * (calculation::numOfBlocks + (long long)1) + i) = -1;
                calculation::energyE[1][channel.id * (calculation::numOfBlocks + (long long)1) + i].insert(channel.id * (calculation::numOfBlocks + (long long)1) + i, gapid * (calculation::numOfBlocks + (long long)1) + i) = 1;
            }
        }
    }
}

void opensubc::initHeatConductMatrix()//初始化子通道间热传导矩阵
{
    calculation::energyCs.resize(calculation::numOfChannelData);
    for (auto& matrix : calculation::energyCs)
        matrix.resize(calculation::numOfChannelData, calculation::numOfChannelData);
    for (auto& channel : geometry::channels)
    {
        int connectedChannelId;
        double mainElement = 0;
        for (unsigned gapid : channel.gapIds)
        {
            opensubc::gap& gap = geometry::gaps[gapid];
            connectedChannelId = gap.getOtherChannelId(channel.id);
            if (connectedChannelId == -1) continue;
            mainElement += gap.sk / gap.lk * calculation::GT;
            for (size_t i = 0; i <= calculation::numOfBlocks; ++i)
            {
                calculation::energyCs[channel.id * (calculation::numOfBlocks + (long long)1) + i].insert(connectedChannelId * (calculation::numOfBlocks + (long long)1) + i, channel.id * (calculation::numOfBlocks + (long long)1) + i) = -0.5 * gap.sk / gap.lk * calculation::GT;
                calculation::energyCs[channel.id * (calculation::numOfBlocks + (long long)1) + i].insert(connectedChannelId * (calculation::numOfBlocks + (long long)1) + i, connectedChannelId * (calculation::numOfBlocks + (long long)1) + i) = -0.5 * gap.sk / gap.lk * calculation::GT;
                calculation::energyCs[channel.id * (calculation::numOfBlocks + (long long)1) + i].insert(channel.id * (calculation::numOfBlocks + (long long)1) + i, connectedChannelId * (calculation::numOfBlocks + (long long)1) + i) = 0.5 * gap.sk / gap.lk * calculation::GT;
            }
        }
        for (size_t i = 0; i <= calculation::numOfBlocks; ++i)
            calculation::energyCs[channel.id * (calculation::numOfBlocks + (long long)1) + i].insert(channel.id * (calculation::numOfBlocks + (long long)1) + i, channel.id * (calculation::numOfBlocks + (long long)1) + i) = mainElement * 0.5;
    }
}

void opensubc::initLowerHigherColumnMatrix()//初始化降列序号矩阵
{
    calculation::L.resize(calculation::numOfChannelData, calculation::numOfChannelData);
    calculation::H.resize(calculation::numOfChannelData, calculation::numOfChannelData);
    for (int i = 0; i < calculation::numOfChannelData; ++i)
    {
        calculation::H.insert(static_cast<Eigen::Index>(i) + 1, i) = 1;
        if (i % (calculation::numOfBlocks + 1) != 0)
            calculation::L.insert(static_cast<Eigen::Index>(i) - 1, i) = 1;
    }
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