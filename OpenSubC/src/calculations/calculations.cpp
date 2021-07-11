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

        /**********ȫ�����ݴ洢(���������)**********/
        Eigen::VectorXd P, h, T, rho, m, w, wTurbulence, k, q,f,K,Tw,u,cp;//�������Ϊ����Ħ��ѹ���;ֲ�ѹ��ϵ�������¡�ճ�ȡ���ѹ������
        Eigen::VectorXd hn, rhon, mn, wn;//�����ϸ�ʱ�䲽���ļ�����
        Eigen::VectorXd mk, wk;//�����ϸ�ʱ�䲽�ļ�����,���ڴ��ɳڼ���

        /**********ȫ�ּ�������**********/
        double length;//ͨ������
        int numOfBlocks;//����ֶ���
        int numOfChannelData, numOfGapData, numOfRodData;//ͨ�����������ܳ���,gap���������ܳ�����ȼ�ϰ����������ܳ���
        double pInitial, TInitial, GInitial;//������̼��ڳ�ʼ����
        double tStep, tEnd;//ʱ�䲽���������ʱ��
        double GT;//�ȵ��ʼ�������
        double KG;//��������ѹ������
        double fT;//����������
        double theta;//ͨ������ֱ����ļнǣ��Ƕ�ֵ��
        double rQ;//��������ʾȼ�ϰ��������ѱ书��ֱ�ӽ�����ȴ���ı�����
        double d1 = 0.00001, d2 = 0.00001;//w��m�Ĵ��ɳ�����
    }
}

void opensubc::initialize_calculation()
{
    read_calculation_xml();
    initVectors();//��ʼ������������
    initEnergyEquation();//��ʼ�����������������
    initAxialMomentumEquation();//��ʼ�������������������
    initCrossMomentumEquation();//��ʼ�������������������
    initMassEquation();//��ʼ�������غ㷽���������
}

void opensubc::read_calculation_xml()
{
    using namespace opensubc::geometry;
    using namespace opensubc::calculation;

    //������xml�ļ����ҵ����в�������
    TiXmlNode* operatingData = inp.FirstChild("input")->FirstChild("OperatingConditions");

    //��ȡ���в���
    TInitial = atof(operatingData->FirstChildElement("TIN")->GetText()) ;
    GInitial = atof(operatingData->FirstChildElement("GIN")->GetText()) ;
    pInitial = atof(operatingData->FirstChildElement("PIN")->GetText())*1000000;
    tStep = atof(operatingData->FirstChildElement("TSTEP")->GetText());
    tEnd = atof(operatingData->FirstChildElement("TEND")->GetText());
    //��ȡ�������õ��ĳ���
    TiXmlNode* calculationData = inp.FirstChild("input")->FirstChild("Calculation");
    GT = atof(calculationData->FirstChildElement("GT")->GetText());
    KG = atof(calculationData->FirstChildElement("KG")->GetText());
    fT = atof(calculationData->FirstChildElement("fT")->GetText());
    theta = atof(calculationData->FirstChildElement("theta")->GetText());
    rQ = atof(calculationData->FirstChildElement("rQ")->GetText());

    //������xml�ļ����ҵ����β���
    TiXmlNode* geometryData = inp.FirstChild("input")->FirstChild("GeometricParameter");
    length = atof(geometryData->FirstChildElement("LCHANL")->GetText()) / 1000;
    numOfBlocks = atoi(geometryData->FirstChildElement("NDX")->GetText());
    numOfChannelData = channels.size() * ((numOfBlocks + (long long)1));
    numOfGapData=gaps.size() * ((numOfBlocks + (long long)1));
    numOfRodData=rods.size()* ((numOfBlocks + (long long)1));

    //����ȼ�ϰ����ݲ���ȡ�����ܶ�
    TiXmlNode* rodData = nullptr;
    unsigned rodid = 0;
    q.resize(numOfRodData);
    q.setZero();//�Ȱ�q��������
    while (rodData = geometryData->IterateChildren("ROD", rodData))
    {
        double Q = atof(rodData->FirstChildElement("Q")->GetText()) * 1000;
        for (unsigned int i = rodid* (numOfBlocks + (long long)1); i <( (rodid+(long long)1)*(numOfBlocks + (long long)1)); ++i)
        {
            if (!checkInletInterval(i))//�ж��ǲ����������������������������д
            {
                q(i) = Q;
            }
            
        }
        rodid++;
    }
}

void opensubc::initVectors()//��ʼ������������
{
    using namespace opensubc::calculation;
    P.resize(numOfChannelData); h.resize(numOfChannelData); T.resize(numOfChannelData); rho.resize(numOfChannelData); m.resize(numOfChannelData); w.resize(numOfGapData); wTurbulence.resize(numOfGapData); k.resize(numOfChannelData); f.resize(numOfChannelData); K.resize(numOfChannelData); Tw.resize(numOfChannelData); u.resize(numOfChannelData); cp.resize(numOfChannelData);
    hn.resize(numOfChannelData); rhon.resize(numOfChannelData); mn.resize(numOfChannelData); wn.resize(numOfGapData);
    mk.resize(numOfChannelData);wk.resize(numOfGapData);
}

void opensubc::calculate_Tw_f()//������º�Ħ������
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;
    for (size_t i = 0; i < numOfChannelData; ++i)
    {
        if (!checkInletInterval(i))//�ж��ǲ����������������������������д
        {
            unsigned channelid= i / (numOfBlocks + (long long)1);
            double Re = m.coeffRef(i) * channels[channelid].Dh / channels[channelid].A / u.coeffRef(i);//��ŵ��
            double Pr = u.coeffRef(i) * cp.coeffRef(i) / k.coeffRef(i);
            double Tw = T.coeffRef(i) + 200;//���µ����ĳ�ֵ
            double Tw_old;//Tw_old��ʾ��һ�ε����ı���
            double rhow, hw, uw;//���´����ܶȡ���ֵ��ճ��
            do
            {
                Tw_old = Tw;
                 hw= opensubc::CO2::h((P.coeffRef(i) + P.coeffRef(i - 1)) / 2 / 1000000, Tw) * 1000;//ͬһѹ���£������µ���ֵ
                 rhow= opensubc::CO2::rho((P.coeffRef(i) + P.coeffRef(i - 1)) / 2 / 1000000, hw / 1000);//ͬһѹ���£������µ��ܶ�
                double q0 = 0;//����������ȼ�ϰ���ƽ�������ܶ�
                for (auto& rodId : channels[channelid].rodIds)//������ͨ�����ӵ�����rod
                {
                    q0 += q.coeffRef(rodId * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1));
                }
                q0 /= channels[channelid].rodIds.size();//ȡƽ��
                Tw = T.coeffRef(i) + q0 / (k.coeffRef(i) / channels[channelid].Dh * 0.0069 * pow(Re, 0.9) * pow(Pr, 0.66) * pow(rhow / rho.coeffRef(i), 0.43) * (1 + 2.4 / ((i % (numOfBlocks + (long long)1) * length / numOfBlocks - length / numOfBlocks / 2) / channels[channelid].Dh)) * (1 + 0.9120 * pow(Re, -0.1) * pow(Pr, 0.4) * (1 - 2.0043 * exp(-channels[channelid].Dh / (2 * rods[channels[channelid].rodIds[0]].r)))));

            }while (abs(Tw_old - Tw) > 0.01);//��������
            /*std::cout << Tw << std::endl;
            system("pause");*/
            //����Ħ������f
            uw= opensubc::CO2::u(rhow, Tw) / 1000000;
            hw = opensubc::CO2::h((P.coeffRef(i) + P.coeffRef(i - 1)) / 2 / 1000000, Tw) * 1000;//ͬһѹ���£������µ���ֵ
            rhow = opensubc::CO2::rho((P.coeffRef(i) + P.coeffRef(i - 1)) / 2 / 1000000, hw / 1000);//ͬһѹ���£������µ��ܶ�
            f(i) = 1 / pow((1.82 * log10(Re) - 1.64), 2) * 0.84 * pow(Pr, 0.18) * pow(uw / u.coeffRef(i), 0.76) * pow(rhow / rho.coeffRef(i), 0.24);
            calculation::Tw(i) = Tw;
        }
    }
}


void opensubc::calculate_wTurbulence()//����������������w��
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;
    for (size_t i = 0; i < numOfGapData; ++i)
    {
        if (!checkInletInterval(i))//�ж��ǲ����������������������������д
        {
            unsigned gapid = i / (numOfBlocks + (long long)1);
            if (!checkBoundaryGap(gapid))//�ж��ǲ��Ǳ߽�ͨ������������������д
            {
                unsigned channelindex0 = gaps[gapid].channelIds[0] * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1);//gap���ӵ�������ͨ��������
                unsigned channelindex1 = gaps[gapid].channelIds[1] * (numOfBlocks + (long long)1) + i % (numOfBlocks + (long long)1);
                //����������ͨ������ŵ��
                double Re0 = m.coeffRef(channelindex0) * channels[gaps[gapid].channelIds[0]].Dh / channels[gaps[gapid].channelIds[0]].A / u.coeffRef(channelindex0);//������ŵ��
                double Re1 = m.coeffRef(channelindex1) * channels[gaps[gapid].channelIds[1]].Dh / channels[gaps[gapid].channelIds[1]].A / u.coeffRef(channelindex1);
                //����������������
                wTurbulence(i) = 0.05136 * pow((channels[gaps[gapid].channelIds[0]].Dh + channels[gaps[gapid].channelIds[1]].Dh) / 2 / gaps[gapid].s, 0.236) * pow((Re0 + Re1) / 2, -0.073) * gaps[gapid].s * (m.coeffRef(channelindex0) / channels[gaps[gapid].channelIds[0]].A + m.coeffRef(channelindex1) / channels[gaps[gapid].channelIds[1]].A) / 2;
            }
        }

    }

}


void opensubc::calculate()
{
    using namespace opensubc::calculation;
    using namespace opensubc::geometry;
    //�������������������
    double hInitial = opensubc::CO2::h(pInitial / 1000000, TInitial) * 1000;
    double rhoInitial = opensubc::CO2::rho(pInitial / 1000000, hInitial / 1000);
    double kInitial = opensubc::CO2::k(rhoInitial, TInitial) / 1000;
    double uInitial = opensubc::CO2::u(rhoInitial, TInitial) / 1000000;
    double cpInitial = opensubc::CO2::cp(hInitial / 1000, pInitial / 1000000) * 1000;
    //������������Ը�ֵ����ʼʱ�̵�������ͨ��������
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
    K.setZero(); w.setZero(); wTurbulence.setZero(); Tw.setZero(); f.setZero();//������ֲ�ѹ�������������¡�Ħ����������
    calculate_Tw_f();//�����ʼʱ�̱��¡�Ħ������
    output(0);//�����ʼʱ�̵Ľ��
     //��ÿ��ʱ�䲽���½��м��㲢���
    for (double t = tStep; t <= tEnd; t += tStep)
    {
        
        hn = h; rhon = rho; mn = m; wn = w;
        using namespace Eigen;
        VectorXd m_t,P_t;//�����ϴε�������������
        m_t.resize(numOfChannelData);
        P_t.resize(numOfChannelData);
        double m_max,P_max;//�洢����������ѹ�����ϴε����ͱ��μ��������ƫ������ֵ
        int n = 0;
        do
        {
            wk = w;
            mk = m;
            std::cout << n++ << std::endl;
            //system("pause");
            m_t = m;//����һ������������ѹ������������m_t��P_t
            P_t = P;
            m_max = 0; P_max = 0;
            //����Cholesky�ֽ�õ������غ㷽�����Է�����Ľ�
            calculateEnergyMatrix();
            /*std::cout << EnergyA << std::endl;
            std::cout << EnergyB << std::endl << std::endl;*/
            SparseLU<SparseMatrix < double >> cholEnergy(EnergyA);  // ִ��A�� Cholesky�ֽ�
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

            //��ѹ���ݶȼ���ѹǿ���Լ�������������
            for (size_t i = 0; i < numOfChannelData; ++i)
            {
                if (!checkInletInterval(i))//�ж��ǲ����������������������������д
                {
                    P(i) = P(i - 1) + DPx(i) * length / numOfBlocks;
                    rho(i) = opensubc::CO2::rho((P.coeffRef(i)+ P.coeffRef(i-1))/2 / 1000000, h.coeffRef(i)/ 1000);
                    T(i) = opensubc::CO2::t((P.coeffRef(i) + P.coeffRef(i - 1)) / 2 / 1000000, h.coeffRef(i) / 1000);
                    /*std::cout << h(i) << " " << P(i) << " " <<  T(i) << std::endl;
                    system("pause");*/
                    k(i) = opensubc::CO2::k(rho.coeffRef(i), T.coeffRef(i)) / 1000;
                    u(i) = opensubc::CO2::u(rho.coeffRef(i), T.coeffRef(i)) / 1000000;
                    cp(i)= opensubc::CO2::cp(h.coeffRef(i) / 1000, (P.coeffRef(i) + P.coeffRef(i - 1)) / 2 / 1000000) * 1000;
                    m_max = m_max>abs((m.coeffRef(i) - m_t.coeffRef(i)) / m_t.coeffRef(i))? m_max: abs((m.coeffRef(i) - m_t.coeffRef(i)) / m_t.coeffRef(i));//���������������ϴε�����������
                    P_max = P_max > abs((P.coeffRef(i) - P_t.coeffRef(i)) / P_t.coeffRef(i)) ? P_max : abs((P.coeffRef(i) - P_t.coeffRef(i)) / P_t.coeffRef(i));
                }
            }
            output(t);
            //system("pause");
            calculate_wTurbulence();//����������������w��
            calculate_Tw_f();//���㱾�ε����ı��º�Ħ������f
        }while (((m_max > 1e-8)||(P_max>1e-8))&&(n<20));//��������
        //������ʱ�䲽���Ľ�����
        output(t);
        system("pause");
    }
}


void opensubc::finalize_calculation()
{

}