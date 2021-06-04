#pragma once
#include <Eigen/Eigen>

namespace opensubc {

	namespace calculation {
		extern Eigen::SparseMatrix<double> EnergyA;
		extern Eigen::VectorXd EnergyB;//能量方程系数矩阵与常数向量
	}

	void initEnergyEquation();//初始化能量方程
	void calculateEnergyMatrix();//计算能量方程系数矩阵与常数向量
}