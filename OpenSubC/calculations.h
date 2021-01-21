#ifndef OPENSUBC_SIMULATION_H
#define OPENSUBC_SIMULATION_H

#include <vector>

namespace opensubc {

	struct StateValue;
	struct MixValue;

	namespace calculation {
		/**********全局数据存储**********/
		//通道控制体数据
		StateValue** stateValue;
		double** v;
		//间隙搅混数据
		MixValue** mixValue;
		//燃料棒发热面功率
		double** q;

		/**********全局计算设置**********/
		double length;//通道长度
		int numOfBlocks;//轴向分段数
		double pInitial, TInitial, vInitial;//二氧化碳入口初始条件
		double tStep, tEnd;//时间步长与计算总时长
	}

	/**********************功能函数**********************/

	void initialize_calculation();
	//void read_calculation_xml();
	void finalize_calculation();
} // namespace opensubc

/*********数据结构体定义*********/
//状态量
struct opensubc::StateValue
{
	double p;//压强
	double T;//温度
	double density;//密度
	double h;//焓
	double u;//粘性系数
};

//搅混参数
struct opensubc::MixValue
{
	int direction;//从小编号流入大编号为正，反之为负
	double wCross;//横向搅混质量流量
	double wTurbulence;//湍流搅混质量流量
};

#endif // OPENSUBC_SIMULATION_H
