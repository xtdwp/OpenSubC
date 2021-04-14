#pragma once
#include <Eigen/Eigen>

namespace opensubc {

	namespace calculation {
		extern Eigen::SparseMatrix<double> MassA;
		extern Eigen::SparseVector<double> MassB;//质量方程系数矩阵与常数向量
	}

	void initMassEquation();//初始化质量方程
	void calculateMassMatrix();//计算质量方程系数矩阵与常数向量
}
