#pragma once
#include <Eigen/Eigen>

namespace opensubc {

	namespace calculation {
		extern Eigen::SparseVector<double> AX, AXt; //（AXt，即AiXj / dt）
		extern Eigen::SparseMatrix<double> CC;//通道连接矩阵
		extern std::vector<Eigen::SparseMatrix<double>> CT, CM;//湍流交混连接矩阵与横向质量流量连接矩阵
		extern Eigen::SparseMatrix<double> CQ;//计算燃料棒q的的连接矩阵
		extern Eigen::SparseMatrix<double> CST;//计算通道间导热的连接矩阵
		extern Eigen::SparseMatrix<double> energyM, energyMCrossT, energyMCrossM;//系数矩阵,湍流交混部分系数矩阵,横向质量流量部分系数矩阵
		extern Eigen::SparseVector<double> energyB;//常数向量
	}

	void initEnergyEquation();//初始化能量方程所需矩阵
	void initGeometryVector();//初始化几何矩阵（AXt，即AiXj/dt）
	void initCC();//初始化通道连接矩阵
	void initCT();//初始化湍流交混连接矩阵
	void initCM();//初始化横向质量流量连接矩阵
	void initCQ();//初始化计算燃料棒q的的连接矩阵
	void initCST();//初始化通道导热连接矩阵

	void calculateEnergyMatrix();//计算能量方程系数矩阵与常数向量
}