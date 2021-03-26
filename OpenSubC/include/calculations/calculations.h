#ifndef OPENSUBC_SIMULATION_H
#define OPENSUBC_SIMULATION_H

#include <vector>
#include "Eigen/Dense"
#include "Eigen/Eigen"

namespace opensubc {

	namespace calculation {

		/**********全局数据存储(向量与矩阵)**********/
		extern Eigen::SparseVector<double> P, h, T, rho, m, w, wTurbulence, k, q;

		/**********全局计算设置**********/
		extern double length;//通道长度
		extern int numOfBlocks;//轴向分段数
		extern int numOfChannelData, numOfGapData, numOfRodData;//通道数据向量总长度,gap数据向量总长度与燃料棒数据向量总长度
		extern double pInitial, TInitial, vInitial;//二氧化碳入口初始条件
		extern double tStep, tEnd;//时间步长与计算总时长
		extern double GT;//热导率几何因子
	}

	/**********************功能函数**********************/

	void initialize_calculation();
	void read_calculation_xml();
	void finalize_calculation();

	void initVectors();//初始化各变量向量

	void initAxialMomentumEquation();//初始化轴向动量方程所需矩阵
	void initCrossMomentumEquation();//初始化横向动量方程所需矩阵
	void initMassEquation();//初始化质量守恒方程所需矩阵
} // namespace opensubc

#endif // OPENSUBC_SIMULATION_H
