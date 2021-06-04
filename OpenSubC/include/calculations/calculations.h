#ifndef OPENSUBC_SIMULATION_H
#define OPENSUBC_SIMULATION_H

#include <vector>
#include "Eigen/Dense"
#include "Eigen/Eigen"

namespace opensubc {

	namespace calculation {

		/**********ȫ�����ݴ洢(���������)**********/
		extern Eigen::VectorXd P, h, T, rho, m, w, wTurbulence, k, q,f,K, Tw,u��cp;//�������Ϊ����Ħ��ѹ���;ֲ�ѹ��ϵ�������¡�ճ�ȡ���ѹ������
		extern Eigen::VectorXd hn, rhon, mn, wn;//�����ϸ�ʱ�䲽���ļ�����

		/**********ȫ�ּ�������**********/
		extern double length;//ͨ������
		extern int numOfBlocks;//����ֶ���
		extern int numOfChannelData, numOfGapData, numOfRodData;//ͨ�����������ܳ���,gap���������ܳ�����ȼ�ϰ����������ܳ���
		extern double pInitial, TInitial, GInitial;//������̼��ڳ�ʼ����
		extern double tStep, tEnd;//ʱ�䲽���������ʱ��
		extern double GT;//�ȵ��ʼ�������
		extern double KG;//��������ѹ������
		extern double fT;//����������
		extern double theta;//ͨ������ֱ����ļн�(�Ƕ�ֵ)
		extern double rQ;//��������ʾȼ�ϰ��������ѱ书��ֱ�ӽ�����ȴ���ı�����
	}

	/**********************���ܺ���**********************/

	void initialize_calculation();
	void calculate();
	void calculate_Tw_f();//������º�Ħ������
	void calculate_wTurbulence();//����������������w��
	void read_calculation_xml();
	void finalize_calculation();

	void initVectors();//��ʼ������������


	//void initAxialMomentumEquation();//��ʼ�������������������
	//void initCrossMomentumEquation();//��ʼ�������������������
	//void initMassEquation();//��ʼ�������غ㷽���������
} // namespace opensubc

#endif // OPENSUBC_SIMULATION_H
