#include "calculations.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "capi.h"
#include "initialize.h"
#include <vector>

/**********ȫ�����ݴ洢**********/
        //ͨ������������
opensubc::StateValue** opensubc::calculation::stateValue;
double** opensubc::calculation::v;
//��϶��������
opensubc::MixValue** opensubc::calculation::mixValue;
//ȼ�ϰ������湦��
double** opensubc::calculation::q;

/**********ȫ�����ݴ洢(���������)**********/
Eigen::VectorXd P, h, T, rho, m, w, wk;
Eigen::MatrixXd W[31];
Eigen::SparseMatrix<double> SW[31];

/**********ȫ�ּ�������**********/
double opensubc::calculation::length;//ͨ������
int opensubc::calculation::numOfBlocks;//����ֶ���
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
    for (RODElem; RODElem; RODElem = RODElem->NextSiblingElement())//�������е�ȼ�ϰ���Ҫ�����뿨��id˳����д��
    {
        TiXmlHandle ROD(RODElem);
        TiXmlElement* QElem = ROD.FirstChild("Q").Element();
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