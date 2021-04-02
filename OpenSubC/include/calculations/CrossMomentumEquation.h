#pragma once
#include <Eigen/Eigen>

namespace opensubc {

	namespace calculation {
		extern Eigen::SparseVector<double> v, vk,U,Uk; //分别为vij、vkj、Uij、Ukj
		extern Eigen::SparseMatrix<double> CrossMomentumA;  
		extern Eigen::SparseVector<double> CrossMomentumB;//横向动量方程系数矩阵与常数向量
	}

	void initCrossMomentumEquation();//初始化横向动量方程
	void calculateCrossMomentumVectors();//计算vij、vkj、Uij、Ukj等向量
	void calculateCrossMomentumMatrix();//计算横向动量方程系数矩阵与常数向量
	bool checkInletInterval(int);//判断是不是入口处网格，是则返回真
	bool checkBoundaryGap(int);//判断是不是boundarygap，是则返回真
}