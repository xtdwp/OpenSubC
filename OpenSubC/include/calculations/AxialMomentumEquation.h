#pragma once
#include <Eigen/Eigen>

namespace opensubc {

	namespace calculation {
		extern Eigen::VectorXd DPx,R,Uk1,Pk; //�ֱ�Ϊѹ���ݶ�dP/dX������Rij������Ukj������ͨ�����ѹ��Pkj
		extern Eigen::SparseMatrix<double> W1, W2, W3;
		extern Eigen::VectorXd B1;
	}

	void initAxialMomentumEquation();//��ʼ������������
	void calculateAxialMomentumVectors();//����Rij��Ukj������
	void calculateAxialMomentumEquation();//����ѹ���ݶȺ�����ͨ����ѹ��
	void calculateAxialMatrixs();//����W1, W2, W3��B1
}