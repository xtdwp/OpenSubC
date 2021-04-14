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

        /**********ȫ�����ݴ洢(���������)**********/
        Eigen::SparseVector<double> P, h, T, rho, m, w, wTurbulence, k, q,f,K;//�������Ϊ����Ħ��ѹ���;ֲ�ѹ��ϵ��
        Eigen::SparseVector<double> Pn, hn, Tn, rhon, mn, wn, wTurbulencen, kn, qn;//�����ϸ�ʱ�䲽���ļ�����

        /**********ȫ�ּ�������**********/
        double length;//ͨ������
        int numOfBlocks;//����ֶ���
        int numOfChannelData, numOfGapData, numOfRodData;//ͨ�����������ܳ���,gap���������ܳ�����ȼ�ϰ����������ܳ���
        double pInitial, TInitial, vInitial;//������̼��ڳ�ʼ����
        double tStep, tEnd;//ʱ�䲽���������ʱ��
        double GT;//�ȵ��ʼ�������
        double KG;//��������ѹ������
        double fT;//����������
        double theta;//ͨ������ֱ����ļнǣ��Ƕ�ֵ��
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
    //calculation::q = &q0[0];
}

void opensubc::initVectors()//��ʼ������������
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