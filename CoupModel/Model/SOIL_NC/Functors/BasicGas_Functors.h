#pragma once
#include "../../../std.h"
enum class Trace_Gas_Emissions_Sw { off, direct_loos, Vertical_fluxes_in_soil };
enum class CO2_SoilModel_Sw { off, Steady_state, Dynamic };
enum class Oxygen_Model_Sw { Steady_state, Dynamic };

enum class GasElement { OXYGEN, NO, CO2, CH4, COS };

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
		case GasElement::COS:
			moleRatio = Fix::MOL_RATIO_COS_AIR / Fix::PPT_FRAC; 
			break;
		}
	}
	
	double operator() (double gas_volume, double temp) {
		return gas_volume * moleRatio * f_AirDensityTempFunction(temp); // Note the gas_volume have adjusted units
	}
	double operator() (double mass_m2, double dz, double temp) {
		double massratio_water_air = 1.E6/ f_AirDensityTempFunction(temp);

		return mass_m2 / dz /massratio_water_air/moleRatio;

	}

private:
	double moleRatio;
	function<double(double)> f_AirDensityTempFunction;
};

class GasVolumeTempFunction {	// Return g/M3
public:
	GasVolumeTempFunction(GasElement element, double height) {
		f_AirDensityTempFunction = AirDensityTempFunction(height);
		switch (element) {
		case GasElement::OXYGEN:
			moleRatio = Fix::MOL_RATIO_O2_AIR / Fix::PERCENT_FRAC;
			break;
		case GasElement::NO:
			moleRatio = Fix::MOL_RATIO_NO_AIR / Fix::PPM_FRAC;
			break;
		case GasElement::CO2:
			moleRatio = Fix::MOL_RATIO_CO2_AIR / Fix::PPM_FRAC;
			break;
		case GasElement::CH4:
			moleRatio = Fix::MOL_RATIO_CH4_AIR / Fix::PPM_FRAC;
			break;
		case GasElement::COS:
			moleRatio = Fix::MOL_RATIO_COS_AIR / Fix::PPT_FRAC;
			break;
		}
	}

	double operator() (double conc, double temp) {
		return conc / moleRatio / f_AirDensityTempFunction(temp); // Note the gas_volume have adjusted units
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
			D_Coef = Fix::DNOT_O2;
		case GasElement::NO:
			D_Coef = Fix::DNOT_O2;
			break;
		case GasElement::CO2:
			D_Coef = Fix::DNOT_CO2;
			break;
		case GasElement::CH4:
			D_Coef = Fix::DNOT_CH4;
			break;
		case GasElement::COS:
			D_Coef = Fix::DNOT_COS;
			break;
		}
		f_Temp = TempDiffusionFunc();
		D_Coef *= 86400.;
		D_MinValue = D_Coef / 1.E4;
		Simple = true;
	}
	SoilAirDiffusion(GasElement element, double tourtous, double PowerCoef, double Threshold, double MinValue) noexcept :
		tourtousf(tourtous), D_PowerCoef(PowerCoef), D_AirThreshold(Threshold), D_MinValue(MinValue) {
		switch (element) {
		case GasElement::OXYGEN:
			D_Coef = Fix::DNOT_O2;
			break;
		case GasElement::NO:
			D_Coef = Fix::DNOT_O2;
			break;
		case GasElement::CO2:
			D_Coef = Fix::DNOT_CO2;
			break;
		case GasElement::CH4:
			D_Coef = Fix::DNOT_CH4;
			break;
		case GasElement::COS:
			D_Coef = Fix::DNOT_COS;
			break;
		}
		D_Coef *= 86400.;	 // Note this to convert from m2/s to m2/day, considering the Air Percentage instead of Air Fraction
						 // 864 instead of 86400	
		D_MinValue = D_Coef / 1.E4;
		f_Temp = TempDiffusionFunc();
		Simple = false;
	}

	double operator() (double Saturation,double AirPerCentFraction) {
		if (Simple) return SimpleDiffusion(Saturation,AirPerCentFraction);
		else return ComplexDiffusion(Saturation,AirPerCentFraction);

	}
	double operator() (double Saturation, double AirPerCentFraction, double Temp) {
		if (Simple) return SimpleDiffusion(Saturation,AirPerCentFraction) * f_Temp(Temp);
		else return ComplexDiffusion(Saturation,AirPerCentFraction) * f_Temp(Temp);
	}
private:
	double SimpleDiffusion(double Saturation,double AirPerCentFraction) {
		if (AirPerCentFraction >= 1.)
			return tourtousf * AirPerCentFraction / Saturation * D_Coef;
		else
			return D_MinValue;
			//return max(D_MinValue, tourtousf * pow(AirPerCentFraction/100.,10./3.)/pow(Saturation/100.,2) * D_Coef);
	}
	double ComplexDiffusion(double Saturation, double AirPerCentFraction) {
		if (AirPerCentFraction >= D_AirThreshold)
			return D_MinValue + pow((AirPerCentFraction - D_AirThreshold)/100., D_PowerCoef)/pow(Saturation/100.,2) * tourtousf * D_Coef;
		else
			return D_MinValue;
	}

	bool Simple;
	double D_Coef, tourtousf;
	double D_MinValue, D_AirThreshold, D_PowerCoef;
	function<double(double)> f_Temp;
};

class SoilWaterDiffusion {
public:
	SoilWaterDiffusion(GasElement element) : element(element) {
		switch (element) {
		case GasElement::OXYGEN:
			a = 1.172;
			b = 0.03443;
			c = 0.0005048;
			scale = 1.e-9;
			break;
		case GasElement::CH4:
			a = 0.9798;
			b = 0.02986;
			c = 0.0004381;
			scale = 1.e-9;
			break;
		case GasElement::CO2:
			a = 0.939;
			b = 0.02671;
			c = 0.0004095;
			scale = 1.e-9;
		case GasElement::COS:
			a = 0.939;
			b = 0.02671;
			c = 0.0004095;
			scale = 1.e-9;
		}
	}

	double operator() (double sat,double Temp) {
		return sat*sat*(a + b * Temp + c * Temp * Temp)*scale*1.E-4; // m2/s
	}

private:
	double a, b, c, scale;
	GasElement element;
};

class ChangeConcentration {
	public:
		ChangeConcentration() {};
		double operator() (double flux, double dz, double Diff) {
		//return flux * dz / (Diff * conc);
		return flux * dz / Diff; // return the concentration change in g/m3
	}
};