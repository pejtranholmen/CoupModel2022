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
		Irrigation* p_Atm;

	private:
		function<void(vector<double>, vector<double>, vector<double>*)> f_ConvectiveFlux;
		function<void(int, vector<double>, vector<double>, vector<double>, vector<double>, vector<double>*)> f_DiffusionFlux;
		function<void(int, vector<double>, vector<double>, vector<double>, vector<double>, vector<double>, vector<double>*)> f_DiffusionAirSoilFlux;
		function<double(double, double, double)> f_delta_conc;
		function<void(vector<double>, vector<double>, vector<double>*)> f_ConvectiveCOSFlux;
		function<void(int, vector<double>, vector<double>, vector<double>, vector<double>, vector<double>*)> f_DiffusionCOSFlux;
		function<void(int, vector<double>, vector<double>, vector<double>, vector<double>, vector<double>, vector<double>*)> f_DiffusionAirSoilCOSFlux;

};
