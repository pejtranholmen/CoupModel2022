#pragma once

#include "./NC_GasFlux_Functions.h"
class NC_GasFlux : public NC_GasFlux_Functions
{
	public:
		NC_GasFlux(void);
		~NC_GasFlux(void);
		bool Ini();
		bool IniFunctors();
		bool Dyn();
		void Flux();
		void Integ();
		bool End();

	private:
		function<void(vector<double>, vector<double>, vector<double> *)> f_DiffusionFlux, f_ConvectiveFlux;
		function<double(double, double, double , double c)> f_delta_conc;


};
