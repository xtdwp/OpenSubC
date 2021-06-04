#pragma once
#include <Eigen/Eigen>

namespace opensubc {

	namespace calculation {
		extern Eigen::SparseMatrix<double> MassA;
		extern Eigen::VectorXd MassB;//��������ϵ�������볣������
	}

	void initMassEquation();//��ʼ����������
	void calculateMassMatrix();//������������ϵ�������볣������
}
