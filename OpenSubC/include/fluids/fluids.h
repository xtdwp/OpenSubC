#pragma once

namespace opensubc {
	class CO2 {
	public:
		static double h(double p, double t);   //��ѹǿ(MPa)���¶�(K)����ֵ(kJ/kg)
		static double t(double p, double h);  //��ѹǿ����ֵ���¶�
		static double rho(double p, double h); //��ѹǿ����ֵ���ܶȣ�kg/m3��
		static double u(double rho, double t); //���ܶȡ��¶���ճ�ȣ�uPa*s��
		static double k(double rho, double t);//���ܶ��¶����ȵ��ʣ�mW/m/K��
		static double cp(double h, double p);//����ֵ��ѹǿ�㶨ѹ�����ݣ�kJ/kg��
	};
}

