#pragma once
#include <Eigen/Eigen>

namespace opensubc {

	namespace calculation {
		extern Eigen::VectorXd DPx,R,Uk1,Pk; //�ֱ�Ϊѹ���ݶ�dP/dX������Rij������Ukj������ͨ�����ѹ��Pkj
	}

	void initAxialMomentumEquation();//��ʼ������������
	void calculateAxialMomentumVectors();//����Rij��Ukj������
	void calculateAxialMomentumEquation();//����ѹ���ݶȺ�����ͨ����ѹ��
}