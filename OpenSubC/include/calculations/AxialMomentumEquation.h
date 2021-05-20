#pragma once
#include <Eigen/Eigen>

namespace opensubc {

	namespace calculation {
		extern Eigen::VectorXd DPx,R,Uk1,Pk; //分别为压力梯度dP/dX、变量Rij、变量Ukj、相邻通道间的压差Pkj
	}

	void initAxialMomentumEquation();//初始化轴向动量方程
	void calculateAxialMomentumVectors();//计算Rij、Ukj等向量
	void calculateAxialMomentumEquation();//计算压力梯度和相邻通道的压差
}