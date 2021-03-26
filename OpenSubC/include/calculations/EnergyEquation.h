#pragma once
#include <Eigen/Eigen>

namespace opensubc {

	namespace calculation {
		extern Eigen::SparseVector<double> AX, AXt; //��AXt����AiXj / dt��
		extern Eigen::SparseMatrix<double> CC;//ͨ�����Ӿ���
		extern std::vector<Eigen::SparseMatrix<double>> CT, CM;//�����������Ӿ�������������������Ӿ���
		extern Eigen::SparseMatrix<double> CQ;//����ȼ�ϰ�q�ĵ����Ӿ���
		extern Eigen::SparseMatrix<double> CST;//����ͨ���䵼�ȵ����Ӿ���
		extern Eigen::SparseMatrix<double> energyM, energyMCrossT, energyMCrossM;//ϵ������,�������첿��ϵ������,����������������ϵ������
		extern Eigen::SparseVector<double> energyB;//��������
	}

	void initEnergyEquation();//��ʼ�����������������
	void initGeometryVector();//��ʼ�����ξ���AXt����AiXj/dt��
	void initCC();//��ʼ��ͨ�����Ӿ���
	void initCT();//��ʼ�������������Ӿ���
	void initCM();//��ʼ�����������������Ӿ���
	void initCQ();//��ʼ������ȼ�ϰ�q�ĵ����Ӿ���
	void initCST();//��ʼ��ͨ���������Ӿ���

	void calculateEnergyMatrix();//������������ϵ�������볣������
}