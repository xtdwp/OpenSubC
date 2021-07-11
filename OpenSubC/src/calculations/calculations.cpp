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
#include <fluids.h>
#include <output.h>
#include <Eigen/Dense>
#include <Eigen/SparseQR>

namespace opensubc {

    namespace calculation {

        /**********全局数据存储(向量与矩阵)**********/
        Eigen::VectorXd P, h, T, rho, m, w, wTurbulence, k, q,f,K,Tw,u,cp;//最后三项为轴向摩擦压降和局部压降系数、壁温、粘度、定压比热容
        Eigen::VectorXd hn, rhon, mn, wn;//储存上个时间步长的计算结果
        Eigen::VectorXd mk, wk;//储存上个时间步的计算结果,用于次松弛计算

        /**********全局计算设置**********/
        double length;//通道长度
        int numOfBlocks;//轴向分段数
        int numOfChannelData, numOfGapData, numOfRodData;//通道数据向量总长度,gap数据向量总长度与燃料棒数据向量总长度
        double pInitial, TInitial, GInitial;//二氧化碳入口初始条件
        double tStep, tEnd;//时间步长与计算总时长
        double GT;//热导率几何因子
        double KG;//横向流动压降因子
        double fT;//横向动量因子
        double theta;//通道与竖直方向的夹角（角度值）
        double rQ;//常数，表示燃料棒产生的裂变功率直接进入冷却剂的比例；
        double d1 = 0.00001, d2 = 0.00001;//w和m的次松弛因子
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
    using namespace opensubc::geometry;
    using namespace opensubc::calculation;

    //从整个xml文件中找到运行参数部分
    TiXmlNode* operatingData = inp.FirstChild("input")->FirstChild("OperatingConditions");

    //读取运行参数
    TInitial = atof(operatingData->FirstChildElement("TIN")->GetText()) ;
    GInitial = atof(operatingData->FirstChildElement("GIN")->GetText()) ;
    pInitial = atof(operatingData->FirstChildElement("PIN")->GetText())*1000000;
    tStep = atof(operatingData->FirstChildElement("TSTEP")->GetText());
    tEnd = atof(operatingData->FirstChildElement("TEND")->GetText());
    //读取计算中用到的常数
    TiXmlNode* calculationData = inp.FirstChild("input")->FirstChild("Calculation");
    GT = atof(calculationData->FirstChildElement("GT")->GetText());
    KG = atof(calculationData->FirstChildElement("KG")->GetText());
    fT = atof(calculationData->FirstChildElement("fT")->GetText());
    theta = atof(calculationData->FirstChildElement("theta")->GetText());
    rQ = atof(calculationData->FirstChildElement("rQ")->GetText());

    //从整个xml文件中找到几何部分
    TiXmlNode* geometryData = inp.FirstChild("input")->FirstChild("GeometricParameter");
    length = atof(geometryData->FirstChildElement("LCHANL")->GetText()) / 1000;
    numOfBlocks = atoi(geometryData->FirstChildElement("NDX")->GetText());
    numOfChannelData = channels.size() * ((numOfBlocks + (long long)1));
    numOfGapData=gaps.size() * ((numOfBlocks + (long long)1));
    numOfRodData=rods.size()* ((numOfBlocks + (long long)1));

    //遍历燃料棒数据并读取热流密度
    TiXmlNode* rodData = nullptr;
    unsigned rodid = 0;
    q.resize(numOfRodData);
    q.setZero();//先把q向量置零
    while (rodData = geometryData->IterateChildren("ROD", rodData))
    {
        double Q = atof(rodData->FirstChildElement("Q")->GetText()) * 1000;
        for (unsigned int i = rodid* (numOfBlocks + (long long)1); i <( (rodid+(long long)1)*(numOfBlocks + (long long)1)); ++i)
        {
            if (!checkInletInterval(i))//判断是不是入口虚拟网格，如果不是则进行填写
            {
                q(i) = Q;
            }
            
        }
        rodid++;
    }
}

void opensubc::initVectors()//初始化各变量向量
{
    using namespace opensubc::calculation;
    P.resize(numOfChannelData); h.resize(numOfChannelData); T.resize(numOfChannelData); rho.resize(numOfChannelData); m.resize(numOfChannelData); w.resize(numOfGapData); wTurbulence.resize(numOfGapData); k.resize(numOfChannelData); f.resize(numOfChannelData); K.resize(numOfChannelData); Tw.resize(numOfChannelData); u.resize(numOfChannelData); cp.resize(numOfChannelData);
    hn.resize(numOfChannelData); rhon.resize(numOfChannelData); mn.resize(numOfChannelData); wn.resize(numOfGapData);
    mk.resize(numOfChannelData);wk.resize(numOfGapData);
}

void opensubc::calculate_Tw_f()//计算壁温和摩擦因子
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;
    for (size_t i = 0; i < numOfChannelData; ++i)
    {
        if (!checkInletInterval(i))//判断是不是入口虚拟网格，如果不是则进行填写
        {
            unsigned channelid= i / (numOfBlocks + (long long)1);
            double Re = m.coeffRef(i) * channels[channelid].Dh / channels[channelid].A / u.coeffRef(i);//雷诺数
            double Pr = u.coeffRef(i) * cp.coeffRef(i) / k.coeffRef(i);
            double Tw = T.coeffRef(i) + 200;//壁温迭代的初值
            double Tw_old;//Tw_old表示上一次迭代的壁温
            double rhow, hw, uw;//壁温处的密度、焓值、粘度
            do
            {
                Tw_old = Tw;
                 hw= opensubc::CO2::h((P.coeffRef(i) + P.coeffRef(i - 1)) / 2 / 1000000, Tw) * 1000;//同一压力下，壁温下的焓值
                 rhow= opensubc::CO2::rho((P.coeffRef(i) + P.coeffRef(i - 1)) / 2 / 1000000, hw / 1000);//同一压力下，壁温下的密度
                double q0 = 0;//计算相连的燃料棒的平均热流密度
                for (auto& rodId : channels[channelid].rodIds)//遍历子通道连接的所有rod
                {
                    q0 += q.coeffRef(rodId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1));
                }
                q0 /= channels[channelid].rodIds.size();//取平均
                Tw = T.coeffRef(i) + q0 / (k.coeffRef(i) / channels[channelid].Dh * 0.0069 * pow(Re, 0.9) * pow(Pr, 0.66) * pow(rhow / rho.coeffRef(i), 0.43) * (1 + 2.4 / ((i % (numOfBlocks + (long long)1) * length / numOfBlocks - length / numOfBlocks / 2) / channels[channelid].Dh)) * (1 + 0.9120 * pow(Re, -0.1) * pow(Pr, 0.4) * (1 - 2.0043 * exp(-channels[channelid].Dh / (2 * rods[channels[channelid].rodIds[0]].r)))));

            }while (abs(Tw_old - Tw) > 0.01);//收敛条件
            /*std::cout << Tw << std::endl;
            system("pause");*/
            //计算摩擦因子f
            uw= opensubc::CO2::u(rhow, Tw) / 1000000;
            hw = opensubc::CO2::h((P.coeffRef(i) + P.coeffRef(i - 1)) / 2 / 1000000, Tw) * 1000;//同一压力下，壁温下的焓值
            rhow = opensubc::CO2::rho((P.coeffRef(i) + P.coeffRef(i - 1)) / 2 / 1000000, hw / 1000);//同一压力下，壁温下的密度
            f(i) = 1 / pow((1.82 * log10(Re) - 1.64), 2) * 0.84 * pow(Pr, 0.18) * pow(uw / u.coeffRef(i), 0.76) * pow(rhow / rho.coeffRef(i), 0.24);
            calculation::Tw(i) = Tw;
        }
    }
}


void opensubc::calculate_wTurbulence()//计算湍流交混速率w’
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;
    for (size_t i = 0; i < numOfGapData; ++i)
    {
        if (!checkInletInterval(i))//判断是不是入口虚拟网格，如果不是则进行填写
        {
            unsigned gapid = i / (numOfBlocks + (long long)1);
            if (!checkBoundaryGap(gapid))//判断是不是边界通道，如果不是则进行填写
            {
                unsigned channelindex0 = gaps[gapid].channelIds[0] * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1);//gap连接的两个子通道的索引
                unsigned channelindex1 = gaps[gapid].channelIds[1] * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1);
                //计算两个子通道的雷诺数
                double Re0 = m.coeffRef(channelindex0) * channels[gaps[gapid].channelIds[0]].Dh / channels[gaps[gapid].channelIds[0]].A / u.coeffRef(channelindex0);//计算雷诺数
                double Re1 = m.coeffRef(channelindex1) * channels[gaps[gapid].channelIds[1]].Dh / channels[gaps[gapid].channelIds[1]].A / u.coeffRef(channelindex1);
                //计算湍流交混速率
                wTurbulence(i) = 0.05136 * pow((channels[gaps[gapid].channelIds[0]].Dh + channels[gaps[gapid].channelIds[1]].Dh) / 2 / gaps[gapid].s, 0.236) * pow((Re0 + Re1) / 2, -0.073) * gaps[gapid].s * (m.coeffRef(channelindex0) / channels[gaps[gapid].channelIds[0]].A + m.coeffRef(channelindex1) / channels[gaps[gapid].channelIds[1]].A) / 2;
            }
        }

    }

}


void opensubc::calculate()
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;
    //计算入口条件所有物性
    double hInitial = opensubc::CO2::h(pInitial / 1000000, TInitial) * 1000;
    double rhoInitial = opensubc::CO2::rho(pInitial / 1000000, hInitial / 1000);
    double kInitial = opensubc::CO2::k(rhoInitial, TInitial) / 1000;
    double uInitial = opensubc::CO2::u(rhoInitial, TInitial) / 1000000;
    double cpInitial = opensubc::CO2::cp(hInitial / 1000, pInitial / 1000000) * 1000;
    //将入口条件物性赋值给初始时刻的所有子通道控制体
    for (size_t i = 0; i < numOfChannelData; ++i)
    {
        P(i) = pInitial;
        h(i) = hInitial;
        T(i) = TInitial;
        rho(i) = rhoInitial;
        unsigned channelid = i / (numOfBlocks + (long long)1);
        m(i) = GInitial * channels[channelid].A ;
        k(i) = kInitial;
        u(i) = uInitial;
        cp(i) = cpInitial;
    }
    K.setZero(); w.setZero(); wTurbulence.setZero(); Tw.setZero(); f.setZero();//将轴向局部压降、横流、壁温、摩擦因子置零
    calculate_Tw_f();//计算初始时刻壁温、摩擦因子
    output(0);//输出初始时刻的结果
     //在每个时间步长下进行计算并输出
    for (double t = tStep; t <= tEnd; t += tStep)
    {
        
        hn = h; rhon = rho; mn = m; wn = w;
        using namespace Eigen;
        VectorXd m_t,P_t;//储存上次迭代的质量流量
        m_t.resize(numOfChannelData);
        P_t.resize(numOfChannelData);
        double m_max,P_max;//存储质量流量和压力在上次迭代和本次计算结果相对偏差的最大值
        int n = 0;
        do
        {
            wk = w;
            mk = m;
            std::cout << n++ << std::endl;
            //system("pause");
            m_t = m;//将上一次质量流量和压力计算结果赋给m_t、P_t
            P_t = P;
            m_max = 0; P_max = 0;
            //采用Cholesky分解得到所有守恒方程线性方程组的解
            calculateEnergyMatrix();
            /*std::cout << EnergyA << std::endl;
            std::cout << EnergyB << std::endl << std::endl;*/
            SparseLU<SparseMatrix < double >> cholEnergy(EnergyA);  // 执行A的 Cholesky分解
            h = cholEnergy.solve(EnergyB);
            /*std::cout << h << std::endl;
            system("pause");*/
            calculateCrossMomentumVectors();
            calculateCrossMomentumMatrix();
            SimplicialCholesky<SparseMatrix < double >> cholCrossMomentum0(CrossMomentumA);
            w = cholCrossMomentum0.solve(CrossMomentumB);
            w = (1 - d1) * wk + d1 * w;
            wk = w;
            //std::cout << w << std::endl;
            calculateAxialMomentumVectors();
            calculateAxialMomentumEquation();
            calculateCrossMomentumMatrix();
            SimplicialCholesky<SparseMatrix < double >> cholCrossMomentum1(CrossMomentumA);
            w = cholCrossMomentum1.solve(CrossMomentumB);
            w = (1 - d1) * wk + d1 * w;
            calculateMassMatrix();
            SimplicialCholesky<SparseMatrix < double >> cholMass(MassA);
            m = cholMass.solve(MassB);
            m = (1 - d2) * mk + d2 * m;

            //由压力梯度计算压强，以及其它所有物性
            for (size_t i = 0; i < numOfChannelData; ++i)
            {
                if (!checkInletInterval(i))//判断是不是入口虚拟网格，如果不是则进行填写
                {
                    P(i) = P(i - 1) + DPx(i) * length / numOfBlocks;
                    rho(i) = opensubc::CO2::rho((P.coeffRef(i)+ P.coeffRef(i-1))/2 / 1000000, h.coeffRef(i)/ 1000);
                    T(i) = opensubc::CO2::t((P.coeffRef(i) + P.coeffRef(i - 1)) / 2 / 1000000, h.coeffRef(i) / 1000);
                    /*std::cout << h(i) << " " << P(i) << " " <<  T(i) << std::endl;
                    system("pause");*/
                    k(i) = opensubc::CO2::k(rho.coeffRef(i), T.coeffRef(i)) / 1000;
                    u(i) = opensubc::CO2::u(rho.coeffRef(i), T.coeffRef(i)) / 1000000;
                    cp(i)= opensubc::CO2::cp(h.coeffRef(i) / 1000, (P.coeffRef(i) + P.coeffRef(i - 1)) / 2 / 1000000) * 1000;
                    m_max = m_max>abs((m.coeffRef(i) - m_t.coeffRef(i)) / m_t.coeffRef(i))? m_max: abs((m.coeffRef(i) - m_t.coeffRef(i)) / m_t.coeffRef(i));//计算质量流量与上次迭代的最大误差
                    P_max = P_max > abs((P.coeffRef(i) - P_t.coeffRef(i)) / P_t.coeffRef(i)) ? P_max : abs((P.coeffRef(i) - P_t.coeffRef(i)) / P_t.coeffRef(i));
                }
            }
            output(t);
            //system("pause");
            calculate_wTurbulence();//计算湍流交混速率w’
            calculate_Tw_f();//计算本次迭代的壁温和摩擦因子f
        }while (((m_max > 1e-8)||(P_max>1e-8))&&(n<20));//收敛条件
        //将本次时间步长的结果输出
        output(t);
        system("pause");
    }
}


void opensubc::finalize_calculation()
{

}