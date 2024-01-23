#pragma once
#include "../../../std.h"
#include "../../Basic_Functors.h"
enum class Methane_Model_Sw { off, SimpleWithStaticTempControl, WithTempControl};
enum class Methane_Plant_Oxidation_Sw { off, on };
enum class Methane_Soil_Oxidation_Sw { off, on_orignal, New };
enum class Methane_Plant_Emission_Sw { off, on };
enum class Methane_SoilBoundary_Flux_Sw{ Fixed_Air_Resistance,Dynamic_Air_Resistance};
enum class N2O_YearlyFluxes_Sw { off, on };



class Ebullition {
public:
	Ebullition (double ratecoef, double thresholdconc) noexcept:
	ratecoef(ratecoef), Threshold(thresholdconc) {
		f_temp = ExpDecline_Response( 0.03);
		// This is an approximate estimated based on 
		//https://www.engineeringtoolbox.com/gases-solubility-water-d_1148.html#gsc.tab=0
	}

	double operator() (double waterconc,double temp) {
		return RateCoefFunc(waterconc, temp);
	};
	double operator() (double waterconc, double wateramount, double temp) {
		return RateCoefFunc(waterconc, temp) * wateramount;
	};
private:
	double RateCoefFunc(double waterconc, double temp) {
		if(temp < 0.) return 0.;
		return ratecoef *max(waterconc - (1.-f_temp(temp))*Threshold, 0.);
	};
	double ratecoef, Threshold;
	function<double(double)> f_temp;
	
};
class HenryLawMethaneVolatility {
public:
	HenryLawMethaneVolatility(double Henry_cp, double HenryCoefTemp, double RefTemp ) noexcept 	:
	cp(Henry_cp), ctemp(HenryCoefTemp), reftemp(RefTemp)
	{		// Converts from mass/mass to mass per volume assuming the denisity of water to 10**6 g/m3
			//	changing the unit from g/m3 to g/liter 2008-10-10
		cp = Henry_cp *1000.; 
	}
	double operator() (double WaterConc, double Temp) noexcept {
		return WaterConc / (cp * exp(-ctemp * (1 / (Temp + 273.15) - 1 / (reftemp + 273.15))) * (Temp + 273.15) / 12.2);
	};
																															

private:
	double cp, ctemp, reftemp;

};
class MethaneBoundaryLayerFlux {
public:
	MethaneBoundaryLayerFlux(double ref_conc, double ResistanceAir) noexcept : ref_conc(ref_conc), ra(ResistanceAir) { // This is the instantiation for fixed air resistance
		ra /= Fix::SECPERDAY;};
	MethaneBoundaryLayerFlux(double ref_conc, double ResistanceAir, double LogScale) noexcept : ref_conc(ref_conc), ra(ResistanceAir) { //This is the instantiantion for dynamics scaling of surface flux
		ra /= Fix::SECPERDAY; scalecoef = pow(10., LogScale); };

	double operator() (double ConcSurface) noexcept {
		return ( ConcSurface- ref_conc) / ra;
	};
	double operator() (double ConcSurface, double TempSurface) { // This is the default fixed ra operator
		double MethaneMassConcAtm = ref_conc * Fix::MOL_RATIO_CH4_AIR * AirDensityTempFunc(TempSurface);
		double flux= (ConcSurface - MethaneMassConcAtm) / ra;
		if (TempSurface < 0.)
			return max(0., flux);
		else
			return flux;
	};
	double operator() (double ConcSurface, double TempSurface, double AirResistance) { //This is the operator for dynamics scaling of surface flux
		double MethaneMassConcAtm = ref_conc * Fix::MOL_RATIO_CH4_AIR * AirDensityTempFunc(TempSurface);
		double flux = (ConcSurface - MethaneMassConcAtm) / (AirResistance/Fix::SECPERDAY*scalecoef);
		if (TempSurface < 0.)
			return max(0., flux);
		else
			return flux;
	};



private:
	double ra, ref_conc, scalecoef;

};




class MethaneProduction {
public:
	MethaneProduction(double CH4_RateCoef, double q10_prod, double RateCoefLitter1, double RateCoefLitter2, double RateCoefHumus) noexcept
	{
		clitter1 = CH4_RateCoef * RateCoefLitter1; clitter2 = CH4_RateCoef * RateCoefLitter2; chumus = CH4_RateCoef * RateCoefHumus;
		f_Temp_Response = Q10(q10_prod, 20., 5.);
	}
	MethaneProduction(double CH4_RateCoef,  double q10_prod, double RateCoefLitter1,  double RateCoefHumus) noexcept
	{
		clitter1 = CH4_RateCoef * RateCoefLitter1; clitter2 = 0.; chumus = CH4_RateCoef * RateCoefHumus;
		f_Temp_Response = Q10(q10_prod, 20., 5.);
	}

	double operator() (double Litter1, double Litter2, double Humus, double temp) {
		if (temp < 0.) return 0.;

		return f_Temp_Response(temp)*(clitter1 * Litter1 + clitter2 * Litter2 + chumus * Humus);
	}
	double operator() (double Litter1, double Humus, double temp) {
		return f_Temp_Response(temp)*(clitter1 * Litter1 + chumus * Humus);
	}


private:
	double clitter1, clitter2, chumus;
	function <double(double)> f_Temp_Response;

};
class MethaneEmissionByTranspiration {
public:
	MethaneEmissionByTranspiration(double CH4_LimitWaterUptake, double CH4_WaterPowerCoef, double CH4_WaterPlantCoef) :
		fluxthreshold(CH4_LimitWaterUptake), powercoef(CH4_WaterPowerCoef), plantcoef(CH4_WaterPlantCoef) {	}

	double operator() (double WaterUptakeRate, double Methane_Conc) {
		return WaterUptakeRate * Methane_Conc * PlantEffFunc(WaterUptakeRate);
	}
	double operator() (double WaterUptakeRate) {
		return PlantEffFunc(WaterUptakeRate);
	}

private:
	double PlantEffFunc(double UptakeRate) {
		double out;
		if (UptakeRate <= fluxthreshold)
			out = 1;
		else
			out = pow(fluxthreshold / UptakeRate, powercoef);
		return plantcoef * out;
	}

	double fluxthreshold, powercoef, plantcoef;

};
class MethanePlantOxidation {
public:
	MethanePlantOxidation(double CH4_PlantOxidationRate, double CH4_PlantOxChapeCoef) noexcept : coef(CH4_PlantOxidationRate) {
		f_RootRespRegulation = ExpSquaredDecline_Response(CH4_PlantOxChapeCoef);
	};
	double operator() (double RespirationRate) {
		return coef * f_RootRespRegulation(RespirationRate);
	};


private:
	function <double(double)> f_RootRespRegulation;
	double coef;

};
class GasDiffusionSoilFlux {
	// The DiffusionCoef as input is m2/s and converted to m2/day and
	// also scaled to have flux in g/m2/day instead of mg/m2/day.
	// Note that concentration in water is in mg/dm3
public:
	GasDiffusionSoilFlux(double MethaneDiffCoefWater) {
		diffcoef_m2_day = MethaneDiffCoefWater * 86.4;
	};
	void operator() (vector<double> &WaterConc, vector<double> &MidDistance, vector<double> *flux) {
		for (size_t i = 1; i < flux->size(); i++) {
			flux->at(i - 1) = diffcoef_m2_day * (WaterConc[i] - WaterConc[i - 1]) / MidDistance[i];
		}
	}
private:
	double diffcoef_m2_day;

};
class GasConvectiveSoilFlux {
// The convective flux is added to the flux of diffusion
public:
	GasConvectiveSoilFlux() {scalingcoef = 0.001;};
	void operator() (vector<double> &WaterConc, vector<double> &WaterFlow, vector<double> *CH4_SoilFlux) {
		for (size_t i = 1; i < CH4_SoilFlux->size(); i++) {
			if(WaterFlow[i-1]>0.)
				CH4_SoilFlux->at(i - 1) = CH4_SoilFlux->at(i - 1) + WaterFlow[i - 1] * WaterConc[i - 1] *scalingcoef; // Scaling from mg/L to g/L
			else
				CH4_SoilFlux->at(i - 1) = CH4_SoilFlux->at(i - 1) + WaterFlow[i] * WaterConc[i] * scalingcoef;
		}
	}
private:
	double scalingcoef;

};
