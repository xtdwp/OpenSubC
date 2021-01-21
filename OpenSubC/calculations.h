#ifndef OPENSUBC_SIMULATION_H
#define OPENSUBC_SIMULATION_H

#include <vector>

namespace opensubc {

	struct StateValue;
	struct MixValue;

	namespace calculation {
		/**********ȫ�����ݴ洢**********/
		//ͨ������������
		StateValue** stateValue;
		double** v;
		//��϶��������
		MixValue** mixValue;
		//ȼ�ϰ������湦��
		double** q;

		/**********ȫ�ּ�������**********/
		double length;//ͨ������
		int numOfBlocks;//����ֶ���
		double pInitial, TInitial, vInitial;//������̼��ڳ�ʼ����
		double tStep, tEnd;//ʱ�䲽���������ʱ��
	}

	/**********************���ܺ���**********************/

	void initialize_calculation();
	//void read_calculation_xml();
	void finalize_calculation();
} // namespace opensubc

/*********���ݽṹ�嶨��*********/
//״̬��
struct opensubc::StateValue
{
	double p;//ѹǿ
	double T;//�¶�
	double density;//�ܶ�
	double h;//��
	double u;//ճ��ϵ��
};

//�������
struct opensubc::MixValue
{
	int direction;//��С����������Ϊ������֮Ϊ��
	double wCross;//���������������
	double wTurbulence;//����������������
};

#endif // OPENSUBC_SIMULATION_H
