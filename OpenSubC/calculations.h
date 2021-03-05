#ifndef OPENSUBC_SIMULATION_H
#define OPENSUBC_SIMULATION_H

#include <vector>
#include "Eigen/Dense"
#include "Eigen/Eigen"

namespace opensubc {

	/*********数据结构体定义*********/
//状态量
	struct StateValue
	{
		double p;//压强
		double T;//温度
		double density;//密度
		double h;//焓
		double u;//粘性系数
	};

	//搅混参数
	struct MixValue
	{
		int direction;//从小编号流入大编号为正，反之为负
		double wCross;//横向搅混质量流量
		double wTurbulence;//湍流搅混质量流量
	};

	namespace calculation {
		/**********全局数据存储**********/
		//通道控制体数据
		extern StateValue** stateValue;
		extern double** v;
		//间隙搅混数据
		extern MixValue** mixValue;
		//燃料棒发热面功率
		extern double** q;

		/**********全局数据存储(向量与矩阵)**********/
		extern Eigen::SparseMatrix<double> P, h, T, rho, m, w, wTurbulence;
		extern Eigen::SparseMatrix<double> energyD[3];
		extern std::vector<Eigen::SparseMatrix<double>> energyC;

		/**********全局计算设置**********/
		extern double length;//通道长度
		extern int numOfBlocks;//轴向分段数
		extern double pInitial, TInitial, vInitial;//二氧化碳入口初始条件
		extern double tStep, tEnd;//时间步长与计算总时长
	}

	/**********************功能函数**********************/

	void initialize_calculation();
	void read_calculation_xml();
	void finalize_calculation();

	void initVectors();//初始化各变量向量

	void initEnergyEquation();//初始化能量方程所需矩阵
	void initDirectionMatrix();//初始化方向转换矩阵
	void initConnectMatrix();//初始化连接矩阵

	void initAxialMomentumEquation();//初始化轴向动量方程所需矩阵
	void initCrossMomentumEquation();//初始化横向动量方程所需矩阵
	void initMassEquation();//初始化质量守恒方程所需矩阵
} // namespace opensubc

#endif // OPENSUBC_SIMULATION_H
