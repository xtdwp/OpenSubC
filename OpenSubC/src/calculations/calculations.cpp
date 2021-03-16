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

/**********ȫ�����ݴ洢**********/
        //ͨ������������
opensubc::StateValue** opensubc::calculation::stateValue;
double** opensubc::calculation::v;
//��϶��������
opensubc::MixValue** opensubc::calculation::mixValue;
//ȼ�ϰ������湦��
double** opensubc::calculation::q;
//ͨ���䵼������
double opensubc::calculation::GT;

/**********ȫ�����ݴ洢(���������)**********/
Eigen::SparseMatrix<double> opensubc::calculation::P, opensubc::calculation::h, opensubc::calculation::T, opensubc::calculation::rho, opensubc::calculation::m, opensubc::calculation::w, opensubc::calculation::wTurbulence;
Eigen::SparseMatrix<double> opensubc::calculation::AXt; //��AXt����AiXj / dt��
Eigen::SparseMatrix<double> opensubc::calculation::L, opensubc::calculation::H;   //����ž���
Eigen::SparseMatrix<double> opensubc::calculation::energyD, opensubc::calculation::energyC;
std::vector<Eigen::SparseMatrix<double>> opensubc::calculation::energyE[2];
std::vector<Eigen::SparseMatrix<double>> opensubc::calculation::energyCs;

/**********ȫ�ּ�������**********/
double opensubc::calculation::length;//ͨ������
int opensubc::calculation::numOfBlocks;//����ֶ���
int opensubc::calculation::numOfChannelData, opensubc::calculation::numOfGapData;//ͨ�����������ܳ�����gap���������ܳ���
double opensubc::calculation::pInitial, opensubc::calculation::TInitial, opensubc::calculation::vInitial;//������̼��ڳ�ʼ����
double opensubc::calculation::tStep, opensubc::calculation::tEnd;//ʱ�䲽���������ʱ��

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
    TiXmlHandle hDoc(&inp);         // hDoc��&inpָ��Ķ���
    TiXmlElement* p0Elem;            // ָ��Ԫ�ص�ָ��
    p0Elem = hDoc.FirstChildElement().Element(); //���ڵ��ָ��
    TiXmlHandle hRoot(p0Elem);       // hRoot�Ǹ��ڵ�
    TiXmlElement* LCHANLElem = hRoot.FirstChild("GeometricParameter").FirstChild("LCHANL").Element(); //��ǰָ����LCHANL�ڵ�
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
    std::vector<std::vector<double>> q1(NROD,std::vector<double>(calculation::numOfBlocks));//������ȹ��ʵ�vector��ά����
    double Q;
    std::vector<double*>q0;        //���һ��ָ���vector����
    int i=0;
    std::cout << "before loop" << std::endl;
    for (RODElem; RODElem; RODElem = RODElem->NextSiblingElement())//�������е�ȼ�ϰ���Ҫ�����뿨��id˳����д��
    {
        TiXmlHandle ROD(RODElem);
        TiXmlElement* QElem = ROD.FirstChild("Q").Element();
        if (QElem == NULL)
        {
            std::cout << "input end" << std::endl;
            break;
        }
        Q= atof(QElem->GetText());
        for (int j = 0; j < calculation::numOfBlocks; j++)//���벻ͬ����ֶ������ȹ���
        {
            q1[i][j] = Q;
        }
        q0.push_back(&q1[i][0]);          
        i++;
    }
    calculation::q = &q0[0];
}

void opensubc::initVectors()//��ʼ������������
{

}

void opensubc::initEnergyEquation()
{

}

void opensubc::initDirectionMatrix()//��ʼ������ת������
{
    int n = calculation::numOfChannelData;
    typedef Eigen::Triplet<double> T;
    std::vector<T> tripletList0;           //������Ԫ��
    int v_ij ;
    for (int i = 0; i < n; i++) { //�����У���ֵ
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
    calculation::energyD.setFromTriplets(tripletList0.begin(), tripletList0.end());     //����Ԫ����䵽����
}

void opensubc::initGeometryMatrix()//��ʼ�����ξ���AXt����AiXj/dt��
{
    calculation::AXt.resize(calculation::numOfChannelData, calculation::numOfChannelData);
    for (auto& channel : geometry::channels)
    {
        for (size_t i = 1; i <= calculation::numOfBlocks; ++i)
            calculation::AXt.insert(channel.id * (calculation::numOfBlocks + (long long)1) + i, channel.id * (calculation::numOfBlocks + (long long)1) + i) = channel.A * calculation::length / calculation::numOfBlocks / calculation::tStep;
    }
}

void opensubc::initConnectMatrix()//��ʼ�����Ӿ���
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

void opensubc::initCrossDirectionMatrix()//��ʼ��������������ʸ����e������
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

void opensubc::initHeatConductMatrix()//��ʼ����ͨ�����ȴ�������
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

void opensubc::initLowerHigherColumnMatrix()//��ʼ��������ž���
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