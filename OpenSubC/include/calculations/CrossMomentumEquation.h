#pragma once
#include <Eigen/Eigen>

namespace opensubc {

	namespace calculation {
		extern Eigen::SparseVector<double> v, vk,U,Uk; //分别为vij、vkj、Uij、Ukj
		extern Eigen::SparseMatrix<double> CrossMomentumA;  
		extern Eigen::VectorXd CrossMomentumB;//横向动量方程系数矩阵与常数向量
		extern Eigen::SparseMatrix<double> W4, W5, W4_inverse;
		extern Eigen::VectorXd B2;
	}

	void initCrossMomentumEquation();//初始化横向动量方程
	void calculateCrossMomentumVectors();//计算vij、vkj、Uij、Ukj等向量
	void calculateCrossMomentumMatrix();//计算横向动量方程系数矩阵与常数向量
	bool checkInletInterval(int);//判断是不是入口处网格，是则返回真
	bool checkBoundaryGap(int);//判断是不是boundarygap，是则返回真

	void calculateCrossMatrixs();//计算W4, W5, W4_inverse和B2
}