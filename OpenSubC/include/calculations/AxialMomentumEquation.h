#pragma once
#include <Eigen/Eigen>

namespace opensubc {

	namespace calculation {
		extern Eigen::SparseVector<double> DPx,R,Uk1,Pk; //�ֱ�Ϊѹ���ݶ�dP/dX������Rij������Ukj������ͨ�����ѹ��Pkj
	}

	void initAxialMomentumEquation();//��ʼ������������
	void calculateAxialMomentumVectors();//����Rij��Ukj������
	void calculateAxialMomentumEquation(Eigen::SparseVector<double>&DPx, Eigen::SparseVector<double>&Pk);//����ѹ���ݶȺ�����ͨ����ѹ��
}