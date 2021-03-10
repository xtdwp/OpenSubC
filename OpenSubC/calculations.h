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
		//通道间导热因子
		extern double GT;

		/**********全局数据存储(向量与矩阵)**********/
		extern Eigen::SparseMatrix<double> P, h, T, rho, m, w, wTurbulence, k;
		extern Eigen::SparseMatrix<double> AXt; //（AXt，即AiXj / dt）
		extern Eigen::SparseMatrix<double> L, H;//降列序号矩阵与升列序号矩阵
		extern Eigen::SparseMatrix<double> energyD,energyC;//轴向流量方向矩阵与通道连接矩阵
		extern std::vector<Eigen::SparseMatrix<double>> energyE[2];//通道与gap计算横向流量的连接矩阵（包括方向）
		extern std::vector<Eigen::SparseMatrix<double>> energyCs;//计算子通道间导热时的连接矩阵

		/**********全局计算设置**********/
		extern double length;//通道长度
		extern int numOfBlocks;//轴向分段数
		extern int numOfChannelData, numOfGapData;//通道数据向量总长度与gap数据向量总长度
		extern double pInitial, TInitial, vInitial;//二氧化碳入口初始条件
		extern double tStep, tEnd;//时间步长与计算总时长
	}

	/**********************功能函数**********************/

	void initialize_calculation();
	void read_calculation_xml();
	void finalize_calculation();

	void initVectors();//初始化各变量向量

	void initEnergyEquation();//初始化能量方程所需矩阵
	void initGeometryMatrix();//初始化几何矩阵（AXt，即AiXj/dt）
	void initDirectionMatrix();//初始化方向转换矩阵
	void initConnectMatrix();//初始化连接矩阵
	void initCrossDirectionMatrix();//初始化横向流量方向矢量（e）矩阵
	void initHeatConductMatrix();//初始化子通道间热传导矩阵
	void initLowerHigherColumnMatrix();//初始化降列序号矩阵与升列序号矩阵

	void initAxialMomentumEquation();//初始化轴向动量方程所需矩阵
	void initCrossMomentumEquation();//初始化横向动量方程所需矩阵
	void initMassEquation();//初始化质量守恒方程所需矩阵
} // namespace opensubc

#endif // OPENSUBC_SIMULATION_H
