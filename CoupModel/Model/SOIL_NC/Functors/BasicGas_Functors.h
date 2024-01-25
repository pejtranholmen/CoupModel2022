#pragma once
#include "../../../std.h"
enum class Trace_Gas_Emissions_Sw { off, direct_loos, Vertical_fluxes_in_soil };
enum class CO2_SoilModel_Sw { off, Steady_state, Dynamic };
enum class Oxygen_Model_Sw { Steady_state, Dynamic };

enum class GasElement { OXYGEN, NO, CO2, CH4 };

class RelativePressureHeightFunction {
public:
	RelativePressureHeightFunction() {
		coef = 2.25577E-5, powercoef = 5.25588;
	}
	double operator() (double height) {
		return pow(1 - coef * height, powercoef);
	}
private:
	double coef, powercoef;
};


class AirDensityTempFunction {	// Returns KG/M3 prior 2024 - changed to g/m3  
public:
	AirDensityTempFunction(double reftemp_C, double height) {  RelativePressureHeightFunction f; density = f(height)*Fix::DENSA*Fix::KILO; reftemp = reftemp_C + Fix::ABS_ZERO; }
	AirDensityTempFunction(double height) { RelativePressureHeightFunction f; density = f(height)*Fix::DENSA*Fix::KILO; reftemp = 293.15; }
	AirDensityTempFunction() {  density = Fix::DENSA*Fix::KILO; reftemp = 293.15; }
	double operator() (double temp) { return density * reftemp / (Fix::ABS_ZERO + temp); }
private:
	double density, reftemp;
};
class GasDensityTempFunction {	// Return g/M3
public:
	GasDensityTempFunction(GasElement element, double height) {
		f_AirDensityTempFunction = AirDensityTempFunction(height);
		switch (element) {
		case GasElement::OXYGEN:
			moleRatio = Fix::MOL_RATIO_O2_AIR/Fix::PERCENT_FRAC;
			break;
		case GasElement::NO:
			moleRatio = Fix::MOL_RATIO_NO_AIR/Fix::PPM_FRAC;
			break;
		case GasElement::CO2:
			moleRatio = Fix::MOL_RATIO_CO2_AIR/Fix::PPM_FRAC;
			break;
		case GasElement::CH4:
			moleRatio = Fix::MOL_RATIO_CH4_AIR/Fix::PPM_FRAC;
			break;
		}
	}

	double operator() (double gas_volume, double temp) {
		return gas_volume * moleRatio * f_AirDensityTempFunction(temp); // Note the gas_volume have adjusted units
	}
private:
	double moleRatio;
	function<double(double)> f_AirDensityTempFunction;
};


class TempDiffusionFunc {
public:
	TempDiffusionFunc() { abstemp = 273.15, powercoef = 1.75; }
	double operator() (double Temp) {
		return pow((Temp + abstemp) / abstemp, powercoef);
	}
private:
	double abstemp, powercoef;
};

class SoilAirDiffusion {
public:
	SoilAirDiffusion(double dcoef, double tourtous) noexcept : D_Coef(dcoef), tourtousf(tourtous) {
		f_Temp = TempDiffusionFunc();
		D_MinValue = D_Coef / 1.E4;	f_Temp = TempDiffusionFunc();	Simple = true;
	}
	SoilAirDiffusion(GasElement element, double tourtous) noexcept : tourtousf(tourtous) {
		switch (element) {
		case GasElement::OXYGEN:
		case GasElement::NO:
			D_Coef = Fix::DNOT_O2;
			break;
		case GasElement::CO2:
			D_Coef = Fix::DNOT_CO2;
			break;
		case GasElement::CH4:
			D_Coef = Fix::DNOT_CH4;
			break;
		}
		f_Temp = TempDiffusionFunc();
		D_Coef *= 864.;
		D_MinValue = D_Coef / 1.E4;
		Simple = true;
	}
	SoilAirDiffusion(GasElement element, double tourtous, double PowerCoef, double Threshold, double MinValue) noexcept :
		tourtousf(tourtous), D_PowerCoef(PowerCoef), D_AirThreshold(Threshold), D_MinValue(MinValue) {
		switch (element) {
		case GasElement::OXYGEN:
		case GasElement::NO:
			D_Coef = Fix::DNOT_O2;
			break;
		case GasElement::CO2:
			D_Coef = Fix::DNOT_CO2;
			break;
		case GasElement::CH4:
			D_Coef = Fix::DNOT_CH4;
			break;
		}
		D_Coef *= 864.;	 // Note this to convert from m2/s to m2/day, considering the Air Percentage instead of Air Fraction
						 // 864 instead of 86400	
		D_MinValue = D_Coef / 1.E4;
		f_Temp = TempDiffusionFunc();
		Simple = false;
	}

	double operator() (double AirPerCentFraction) {
		if (Simple) return SimpleDiffusion(AirPerCentFraction);
		else return ComplexDiffusion(AirPerCentFraction);

	}
	double operator() (double AirPerCentFraction, double Temp) {
		if (Simple) return SimpleDiffusion(AirPerCentFraction) * f_Temp(Temp);
		else return ComplexDiffusion(AirPerCentFraction) * f_Temp(Temp);
	}
private:
	double SimpleDiffusion(double AirPerCentFraction) {
		return max(D_MinValue, tourtousf * AirPerCentFraction * D_Coef);
	}
	double ComplexDiffusion(double AirPerCentFraction) {
		if (AirPerCentFraction >= D_AirThreshold)
			return D_MinValue + pow((AirPerCentFraction - D_AirThreshold), D_PowerCoef) * tourtousf * D_Coef;
		else
			return D_MinValue;
	}


	bool Simple;
	double D_Coef, tourtousf;
	double D_MinValue, D_AirThreshold, D_PowerCoef;
	function<double(double)> f_Temp;
};
class ChangeConcentration {
	public:
		ChangeConcentration() {};
		double operator() (double flux, double dz, double Diff, double conc) {
		return flux * dz / (Diff * conc);
	}
};