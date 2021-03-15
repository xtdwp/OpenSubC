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
		//ͨ���䵼������
		extern double GT;

		/**********ȫ�����ݴ洢(���������)**********/
		extern Eigen::SparseMatrix<double> P, h, T, rho, m, w, wTurbulence, k;
		extern Eigen::SparseMatrix<double> AXt; //��AXt����AiXj / dt��
		extern Eigen::SparseMatrix<double> L, H;//������ž�����������ž���
		extern Eigen::SparseMatrix<double> energyD,energyC;//�����������������ͨ�����Ӿ���
		extern std::vector<Eigen::SparseMatrix<double>> energyE[2];//ͨ����gap����������������Ӿ��󣨰�������
		extern std::vector<Eigen::SparseMatrix<double>> energyCs;//������ͨ���䵼��ʱ�����Ӿ���

		/**********ȫ�ּ�������**********/
		extern double length;//ͨ������
		extern int numOfBlocks;//����ֶ���
		extern int numOfChannelData, numOfGapData;//ͨ�����������ܳ�����gap���������ܳ���
		extern double pInitial, TInitial, vInitial;//������̼��ڳ�ʼ����
		extern double tStep, tEnd;//ʱ�䲽���������ʱ��
	}

	/**********************���ܺ���**********************/

	void initialize_calculation();
	void read_calculation_xml();
	void finalize_calculation();

	void initVectors();//��ʼ������������

	void initEnergyEquation();//��ʼ�����������������
	void initGeometryMatrix();//��ʼ�����ξ���AXt����AiXj/dt��
	void initDirectionMatrix();//��ʼ������ת������
	void initConnectMatrix();//��ʼ�����Ӿ���
	void initCrossDirectionMatrix();//��ʼ��������������ʸ����e������
	void initHeatConductMatrix();//��ʼ����ͨ�����ȴ�������
	void initLowerHigherColumnMatrix();//��ʼ��������ž�����������ž���

	void initAxialMomentumEquation();//��ʼ�������������������
	void initCrossMomentumEquation();//��ʼ�������������������
	void initMassEquation();//��ʼ�������غ㷽���������
} // namespace opensubc

#endif // OPENSUBC_SIMULATION_H