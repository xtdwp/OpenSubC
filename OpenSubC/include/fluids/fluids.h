#pragma once

namespace opensubc {
	class CO2 {
	public:
		static double h(double p, double t);   //由压强(MPa)、温度(K)算焓值(kJ/kg)
		static double t(double p, double h);  //由压强、焓值算温度
		static double rho(double p, double h); //由压强、焓值算密度（kg/m3）
		static double u(double rho, double t); //由密度、温度算粘度（uPa*s）
		static double k(double rho, double t);//由密度温度算热导率（mW/m/K）
		static double cp(double h, double p);//由焓值、压强算定压比热容（kJ/kg）
	};
}

