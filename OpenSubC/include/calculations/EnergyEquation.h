#pragma once
#include <Eigen/Eigen>

namespace opensubc {

	namespace calculation {
		extern Eigen::SparseMatrix<double> EnergyA;
		extern Eigen::VectorXd EnergyB;//��������ϵ�������볣������
	}

	void initEnergyEquation();//��ʼ����������
	void calculateEnergyMatrix();//������������ϵ�������볣������
}