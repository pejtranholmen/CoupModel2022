#pragma once
#include "../../../std.h"
#include "../../Basic_Functors.h"
enum class Methane_Model_Sw { off, SimpleWithStaticTempControl, WithTempControl};
enum class Methane_Plant_Oxidation_Sw { off, on };
enum class Methane_Soil_Oxidation_Sw { off, on_orignal, New };
enum class Methane_Plant_Emission_Sw { off, on };
enum class Methane_SoilBoundary_Flux_Sw{ Fixed_Air_Resistance,Dynamic_Air_Resistance};
enum class N2O_YearlyFluxes_Sw { off, on };
enum class MethaneConvectionSoilFlux_Sw{off, on};



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
			// MASS_CH4_WATER as input is mg/L = or g/m3 
			// Output unit = g/m3 has the same unit as input unit

		cp = Henry_cp*Fix::PPM_FRAC*Fix::RGAS_L_ATM_K ; // To get from PPM to 1 Litre and  dm3   

	}
	double operator() (double Mass_CH4_Water, double Temp) noexcept {
		double H = cp * exp(-ctemp * (1 / (Temp + Fix::ABS_ZERO) - 1 / (reftemp + Fix::ABS_ZERO))) * (Temp + Fix::ABS_ZERO);
		return Mass_CH4_Water / H;
	};
																															

private:
	double cp, ctemp, reftemp;

};
class MethaneBoundaryLayerFlux {
public:
	MethaneBoundaryLayerFlux(double ref_conc, double ResistanceAir) noexcept : atm_conc(ref_conc), ra(ResistanceAir) { // This is the instantiation for fixed air resistance
		atm_conc = ref_conc * Fix::MOL_RATIO_CH4_AIR/Fix::PPM_FRAC; ra /= Fix::SECPERDAY; scalecoef = 3.;
		f_AirDensity = AirDensityTempFunction();
	};   // corrected from ra is scaled to convert MassConsAtm and ConSurface from mg/L to g/m3
	MethaneBoundaryLayerFlux(double ref_conc, double ResistanceAir, double LogScale) noexcept : atm_conc(ref_conc), ra(ResistanceAir) { //This is the instantiation for dynamics scaling of surface flux
		atm_conc = ref_conc * Fix::MOL_RATIO_CH4_AIR/Fix::PPM_FRAC; ra /= Fix::SECPERDAY; 	scalecoef = pow(10., LogScale);
		f_AirDensity = AirDensityTempFunction();
	};

	double operator() (double ConcSurface) noexcept {
		return ( ConcSurface- atm_conc) / ra;
	};
	double operator() (double ConcSurface, double TempSurface) { // This is the default fixed ra operator
		double MassConcAtm =atm_conc * f_AirDensity(TempSurface)/Fix::KILO;  // g Methane/m3
		double flux = (ConcSurface - MassConcAtm) / ra;
		if (TempSurface < 0.) // No possible uptake of Methane during freezing conditions
			return max(0., flux);
		else
			return flux;
	};
	double operator() (double ConcSurface, double TempSurface, double AirResistance) { //This is the operator for dynamics scaling of surface flux
																						// Input units are (mg/L, C, s/m)
		double MassConcAtm = atm_conc * f_AirDensity(TempSurface)/Fix::KILO;
		AirResistance = max(1., AirResistance);
		double flux = (ConcSurface - MassConcAtm) / (AirResistance/Fix::SECPERDAY*scalecoef);
		if (TempSurface < 0.)
			return max(0., flux);
		else
			return flux;
	};



private:
	double ra, atm_conc, scalecoef;
	function<double(double)> f_AirDensity;

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
		double out=0.;
		if (UptakeRate <= fluxthreshold)
			out = 1;
		else
			out = pow(fluxthreshold / UptakeRate, powercoef);
		out *= plantcoef;
		return out;
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
	// Note that concentration in water is in mg/dm3 = g/m3
public:
	GasDiffusionSoilFlux(double MethaneDiffCoefWater) {
		diffcoef_m2_day = MethaneDiffCoefWater * Fix::SECPERDAY;
	};
	GasDiffusionSoilFlux(GasElement element, double tourtous) {
		f_DiffusionAir = SoilAirDiffusion(element, 0.66);
	};
	void operator() (vector<double>& WaterConc, vector<double>& MidDistance, vector<double>* flux) {
		for (size_t i = 1; i < flux->size(); i++) {
			flux->at(i - 1) = -diffcoef_m2_day * (WaterConc[i] - WaterConc[i - 1]) / MidDistance[i];
		}
	};
	void operator() (vector<double>& AirConc, vector<double>& AirContent, vector<double>& Temp, vector<double>& MidDistance, vector<double>* flux) {
		for (size_t i = 1; i < flux->size(); i++) {
			double meanaircontent = (AirContent[i - 1] + AirContent[i]) / 2.;
			flux->at(i - 1) = -f_DiffusionAir(meanaircontent, Temp[i-1])* (AirConc[i] - AirConc[i - 1]) / MidDistance[i];
		}
	};

private:
	double diffcoef_m2_day;
	function<double(double, double)> f_DiffusionAir;
};
class GasConvectiveSoilFlux {
// The convective flux is added to the flux of diffusion
public:
	GasConvectiveSoilFlux() {scalingcoef = 1.E-3;};
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
class DiffusionFlux {
public :
	DiffusionFlux(double DCoef) : d(DCoef) {
		d *= Fix::SECPERDAY;
	};
	double operator() (double grad) { 
		// grad in g/m4 return g/m2/day;
		return d*grad; };
private :
	double d;
};
