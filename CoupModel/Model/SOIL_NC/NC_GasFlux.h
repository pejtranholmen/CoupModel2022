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
		void GeneralUpdates();
		void SoilOxygenCarbonGas_Fluxes();

		void NitrousGas_Fluxes();
		void MethaneGas_Fluxes();
		void COSGas_Fluxes();

		void Integ();
		bool End();

	private:
		function<void(vector<double>, std::vector<double>, std::vector<double> *)> f_DiffusionFlux, f_ConvectiveFlux;
		function<void(vector<double>, std::vector<double>, std::vector<double>, std::vector<double>, std::vector<double>*)> f_DiffusionAirSoilFlux;
		function<double(double, double, double , double c)> f_delta_conc;


};
