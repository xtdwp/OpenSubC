#ifndef OPENSUBC_SIMULATION_H
#define OPENSUBC_SIMULATION_H

#include <vector>
#include "Eigen/Dense"
#include "Eigen/Eigen"

namespace opensubc {

	/*********���ݽṹ�嶨��*********/
//״̬��
	struct StateValue
	{
		double p;//ѹǿ
		double T;//�¶�
		double density;//�ܶ�
		double h;//��
		double u;//ճ��ϵ��
	};

	//�������
	struct MixValue
	{
		int direction;//��С����������Ϊ������֮Ϊ��
		double wCross;//���������������
		double wTurbulence;//����������������
	};

	namespace calculation {
		/**********ȫ�����ݴ洢**********/
		//ͨ������������
		extern StateValue** stateValue;
		extern double** v;
		//��϶��������
		extern MixValue** mixValue;
		//ȼ�ϰ������湦��
		extern double** q;

		/**********ȫ�����ݴ洢(���������)**********/
		extern Eigen::VectorXd P, h, T, rho, m, w, wk;
		extern Eigen::MatrixXd W[31];
		extern Eigen::SparseMatrix<double> SW[31];

		/**********ȫ�ּ�������**********/
		extern double length;//ͨ������
		extern int numOfBlocks;//����ֶ���
		extern double pInitial, TInitial, vInitial;//������̼��ڳ�ʼ����
		extern double tStep, tEnd;//ʱ�䲽���������ʱ��
	}

	/**********************���ܺ���**********************/

	void initialize_calculation();
	void read_calculation_xml();
	void finalize_calculation();

	void initVectors();//��ʼ������������
	void initEnergyEquation();//��ʼ�����������������
	void initAxialMomentumEquation();//��ʼ�������������������
	void initCrossMomentumEquation();//��ʼ�������������������
	void initMassEquation();//��ʼ�������غ㷽���������
} // namespace opensubc

#endif // OPENSUBC_SIMULATION_H
