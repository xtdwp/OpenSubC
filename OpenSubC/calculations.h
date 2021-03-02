#ifndef OPENSUBC_SIMULATION_H
#define OPENSUBC_SIMULATION_H

#include <vector>
#include "Eigen//Dense"

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

		/**********全局计算设置**********/
		extern double length;//通道长度
		extern int numOfBlocks;//轴向分段数
		extern double pInitial, TInitial, vInitial;//二氧化碳入口初始条件
		extern double tStep, tEnd;//时间步长与计算总时长
	}

	/**********************功能函数**********************/

	void initialize_calculation();
	//void read_calculation_xml();
	void finalize_calculation();
} // namespace opensubc

#endif // OPENSUBC_SIMULATION_H
