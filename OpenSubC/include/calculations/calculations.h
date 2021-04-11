#ifndef OPENSUBC_SIMULATION_H
#define OPENSUBC_SIMULATION_H

#include <vector>
#include "Eigen/Dense"
#include "Eigen/Eigen"

namespace opensubc {

	namespace calculation {

		/**********ȫ�����ݴ洢(���������)**********/
		extern Eigen::SparseVector<double> P, h, T, rho, m, w, wTurbulence, k, q,f,K;//�������Ϊ����Ħ��ѹ���;ֲ�ѹ��ϵ��
		extern Eigen::SparseVector<double> Pn, hn, Tn, rhon, mn, wn, wTurbulencen, kn, qn;//�����ϸ�ʱ�䲽���ļ�����

		/**********ȫ�ּ�������**********/
		extern double length;//ͨ������
		extern int numOfBlocks;//����ֶ���
		extern int numOfChannelData, numOfGapData, numOfRodData;//ͨ�����������ܳ���,gap���������ܳ�����ȼ�ϰ����������ܳ���
		extern double pInitial, TInitial, vInitial;//������̼��ڳ�ʼ����
		extern double tStep, tEnd;//ʱ�䲽���������ʱ��
		extern double GT;//�ȵ��ʼ�������
		extern double KG;//��������ѹ������
		extern double fT;//����������
		extern double theta;//ͨ������ֱ����ļн�(�Ƕ�ֵ)
	}

	/**********************���ܺ���**********************/

	void initialize_calculation();
	void read_calculation_xml();
	void finalize_calculation();

	void initVectors();//��ʼ������������

	//void initAxialMomentumEquation();//��ʼ�������������������
	//void initCrossMomentumEquation();//��ʼ�������������������
	//void initMassEquation();//��ʼ�������غ㷽���������
} // namespace opensubc

#endif // OPENSUBC_SIMULATION_H
