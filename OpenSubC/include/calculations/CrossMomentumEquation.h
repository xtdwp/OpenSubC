#pragma once
#include <Eigen/Eigen>

namespace opensubc {

	namespace calculation {
		extern Eigen::SparseVector<double> v, vk,U,Uk; //�ֱ�Ϊvij��vkj��Uij��Ukj
		extern Eigen::SparseMatrix<double> CrossMomentumA;  
		extern Eigen::VectorXd CrossMomentumB;//����������ϵ�������볣������
		extern Eigen::SparseMatrix<double> W4, W5, W4_inverse;
		extern Eigen::VectorXd B2;
	}

	void initCrossMomentumEquation();//��ʼ������������
	void calculateCrossMomentumVectors();//����vij��vkj��Uij��Ukj������
	void calculateCrossMomentumMatrix();//�������������ϵ�������볣������
	bool checkInletInterval(int);//�ж��ǲ�����ڴ��������򷵻���
	bool checkBoundaryGap(int);//�ж��ǲ���boundarygap�����򷵻���

	void calculateCrossMatrixs();//����W4, W5, W4_inverse��B2
}