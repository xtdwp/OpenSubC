#ifndef OPENSUBC_SIMULATION_H
#define OPENSUBC_SIMULATION_H

#include <vector>
#include "Eigen//Dense"

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

		/**********ȫ�ּ�������**********/
		extern double length;//ͨ������
		extern int numOfBlocks;//����ֶ���
		extern double pInitial, TInitial, vInitial;//������̼��ڳ�ʼ����
		extern double tStep, tEnd;//ʱ�䲽���������ʱ��
	}

	/**********************���ܺ���**********************/

	void initialize_calculation();
	//void read_calculation_xml();
	void finalize_calculation();
} // namespace opensubc

#endif // OPENSUBC_SIMULATION_H
