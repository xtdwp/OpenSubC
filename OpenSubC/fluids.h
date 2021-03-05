#pragma once

namespace opensubc {
	class CO2 {
	public:
		static double h(double p, double t);   //由压强(MPa)、温度(K)算焓值(kJ/kg)
		static double t(double p, double h);  //由压强、焓值算温度
		static double rho(double p, double h); //由压强、焓值算密度（kg/m3）
		static double u(double rho, double t); //由密度、温度算粘度（uPa/s）
	};
}

