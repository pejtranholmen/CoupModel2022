#pragma once
#include "../../../std.h"
#include "../../Basic_Functors.h"
enum class Methane_Model_Sw { off, SimpleWithStaticTempControl, WithTempControl};
enum class MethaneProductionModel_Sw{Temp_Function, Temp_pH_Function};
enum class Dissolved_OxygenGas_Sw {Implicit, Explicit};
enum class Methane_Plant_Oxidation_Sw { off, on };
enum class Methane_Soil_Oxidation_Sw { off, on_orignal, New };
enum class Methane_Plant_Emission_Sw { off, on };
enum class Ebullition_Sw {Off,SolubilityBased, PressureBased};
enum class Methane_SoilBoundary_Flux_Sw{ Fixed_Air_Resistance,Dynamic_Air_Resistance, Dynamic_Surface_Pool};
enum class Methane_Aerenchyma_Emission_Sw{ off, on};
enum class MethaneConvectionSoilFlux_Sw { off, on };

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

class EbullitionPressure {
public:
	EbullitionPressure(double bubble_frac, double thresholdpress) noexcept:
	frac(bubble_frac), Threshold(thresholdpress) {}

	double operator() (double conc_ch4, double partialpress) {
		return (partialpress - Threshold * frac) * conc_ch4;
	}
private:
	double frac, Threshold;
};

//class BubbleGrowth {
//public:
//	BubbleGrowth(double a, double b, double c) noexcept :
//		a(a), b(b), c(c) {

//	}

//	double operator() (double aa, double bb, double cc) {
//		Qebu = 4. * Fix::PI * radius * Dw_coef * Nbub / Vw * (conc_water - f * Henry * P / (R * T));
//		Vmax = Vmaxfrac * Vw;
//		Vb = Vb + Qebu
//		return cc;
//	}
//private:
//	double a, b, c;
//};

class Michaelis_Menten {
public:
	Michaelis_Menten(double k_ch4) noexcept :
		k_ch4(k_ch4) {}

	double operator() (double conc_ch4) {
		return (conc_ch4 / (k_ch4 + conc_ch4));
	}
private:
	double k_ch4;
};

class Double_Michaelis_Menten {
public:
	Double_Michaelis_Menten(double k_ch4, double k_o2) noexcept :
		k_ch4(k_ch4), k_o2(k_o2) {
		
	}

	double operator() (double conc_ch4, double conc_o2) {
		return (conc_ch4 / (k_ch4 + conc_ch4)) * (conc_o2 / (k_o2 + conc_o2));
	}
private:
	double k_ch4, k_o2;
};

class HenryLawMethaneVolatility {
public:
	HenryLawMethaneVolatility(double Henry_cp, double HenryCoefTemp, double RefTemp) noexcept :
		cp(Henry_cp), ctemp(HenryCoefTemp), reftemp(RefTemp)
	{		// Converts from mass/mass to mass per volume assuming the denisity of water to 10**6 g/m3
			// MASS_CH4_WATER as input is mg/L = or g/m3 
			// Output unit = g/m3 has the same unit as input unit

		cp = Henry_cp * Fix::KILO * Fix::RGAS_L_ATM_K; // To get from PPM to 1 Litre and  dm3   

	}
	double operator() (double Mass_CH4_Water, double Temp) noexcept {
		double H = cp * exp(-ctemp * (1 / (Temp + Fix::ABS_ZERO) - 1 / (reftemp + Fix::ABS_ZERO))) * (Temp + Fix::ABS_ZERO);
		return Mass_CH4_Water / H;
	};


private:
	double cp, ctemp, reftemp;

};

class HenryLawGasWaterConc {
public:
	HenryLawGasWaterConc(double Henry_cp, double HenryCoefTemp, double RefTemp ) noexcept:
	cp(Henry_cp), ctemp(HenryCoefTemp), reftemp(RefTemp)
	{		// Converts from mass/mass to mass per volume assuming the denisity of water to 10**6 g/m3
			// MASS_CH4_WATER as input is mg/L = or g/m3 
			// Output unit = g/m3 has the same unit as input unit
		cp = 1. / Henry_cp; // 769 L.atm/mol
		ctemp = -HenryCoefTemp; // 1700 K
		//cp	= Henry_cp*Fix::KILO*Fix::RGAS_L_ATM_K ; // To get from PPM to 1 Litre and dm3   

	}
	double operator() (double Temp) noexcept {
		double kh_inv = exp(-ctemp * (1 / (Temp + Fix::ABS_ZERO) - 1 / (reftemp + Fix::ABS_ZERO)) + log(cp));
		double kh_cc = (Temp + Fix::ABS_ZERO) / kh_inv * Fix::RGAS_L_ATM_K; // m3-g / m3-w
		return kh_cc;//Mass_CH4 / (Water_Vol + Air_Vol/kh_cc); // g/m3-w
	};
																															

private:
	double cp, ctemp,reftemp;

};

class MethaneEmissionByAerenchyma {
public:
	MethaneEmissionByAerenchyma(double atm_conc, double aere_scale, double diffus_aere) noexcept :
	atm_conc(atm_conc), // mg/L or g/m3
	aere_scale(aere_scale),
	diffuscoef(diffus_aere) {
		diffus_aere * 1.E-4 * Fix::SECPERDAY;
	} // cm2/s to m2/day

	double operator()(double ch4_conc, double TempSurface, double rootfr, double z, double ra, double lai, double npp, double porostill, double nppratio,double rob) noexcept {
		double m_tiller = npp * nppratio * lai; // set lai as 4.0
		double n_tiller = m_tiller / 0.22;
		double scalecoef = pow(10., aere_scale);
		double area_tiller = scalecoef * n_tiller * porostill * Fix::PI * 2.9E-3 * 2.9E-3;  // M2/M2
		double aerecond = area_tiller * rootfr * diffuscoef / (z * rob);  // m/day
		ra = 1. / aerecond + ra/Fix::SECPERDAY; // day/m
		double flux = (ch4_conc - atm_conc) / ra; // g/m2/day
		if (TempSurface < 0.)
			return max(0., flux);
		else
			return flux;
	}

private:
	double atm_conc;
	double aere_scale;
	double diffus_aere;
	double diffuscoef;
	std::function<double(double)> f_AirDensity;
};

class OxygenDiffusionByAerenchyma {
public:
	OxygenDiffusionByAerenchyma(double atm_conc, double aere_scale, double diffus_aere) noexcept :
		atm_conc(atm_conc), // mg/L or g/m3
		aere_scale(aere_scale),
		diffuscoef(diffus_aere) {
		diffus_aere * 1.E-4 * Fix::SECPERDAY;
	} // cm2/s to m2/day

	double operator()(double o2_conc, double TempSurface, double rootfr, double z, double ra, double lai, double npp, double porostill, double nppratio, double rob) noexcept {
		double m_tiller = npp * nppratio * lai; // lai set as 4.0
		double n_tiller = m_tiller / 0.22;
		double scalecoef = pow(10., aere_scale);
		double area_tiller = scalecoef * n_tiller * porostill * Fix::PI * 2.9E-3 * 2.9E-3;  // M2/M2
		double aerecond = area_tiller * rootfr * diffuscoef / (z * rob);  // m/day
		ra = 1. / aerecond + ra/Fix::SECPERDAY; // day/m
		double flux = (atm_conc - o2_conc) / ra; // g/m2/day
		if (TempSurface < 0.)
			return max(0., flux);
		else
			return flux;
	}

private:
	double atm_conc;
	double aere_scale;
	double diffus_aere;
	double diffuscoef;
};


class MethaneBoundaryLayerFlux {
public:
	MethaneBoundaryLayerFlux(double ref_conc, double ResistanceAir) noexcept : atm_conc(ref_conc), ra(ResistanceAir) { // This is the instantiation for fixed air resistance
		atm_conc = ref_conc * Fix::MOL_RATIO_CH4_AIR/Fix::PPM_FRAC ; ra /= Fix::SECPERDAY; scalecoef =1.;
		f_AirDensity = AirDensityTempFunction();
	};   // corrected from ra is scaled to convert MassConsAtm and ConSurface from mg/L to g/m3
	MethaneBoundaryLayerFlux(double ref_conc, double ResistanceAir, double LogScale) noexcept : atm_conc(ref_conc), ra(ResistanceAir) { //This is the instantiation for dynamics scaling of surface flux
		atm_conc = ref_conc * Fix::MOL_RATIO_CH4_AIR/Fix::PPM_FRAC; ra /= Fix::SECPERDAY; scalecoef = pow(10., LogScale);
		f_AirDensity = AirDensityTempFunction();
	};

	double operator() (double ConcSurface) noexcept {
		return ( ConcSurface- atm_conc) / ra;
	};
	double operator() (double ConcSurface, double TempSurface) { // This is the default fixed ra operator
		double MassConcAtm = atm_conc * f_AirDensity(TempSurface);  // g Methane/m3
		double flux = (ConcSurface - MassConcAtm) / (ra*scalecoef);
		if (TempSurface < 0.) // No possible uptake of Methane during freezing conditions
			return max(0., flux);
		else
			return flux;
	};
	double operator() (double ConcSurface, double TempSurface, double AirResistance) { //This is the operator for dynamics scaling of surface flux
		f_AirDensity = AirDensityTempFunction();																				// Input units are (mg/L, C, s/m)
		double MassConcAtm = atm_conc * f_AirDensity(TempSurface);
		double flux = (ConcSurface - MassConcAtm) / (scalecoef*AirResistance/Fix::SECPERDAY);
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
		OrgRateCoef.push_back(clitter1); OrgRateCoef.push_back(clitter2); OrgRateCoef.push_back(chumus);
		f_Temp_Response = Q10(q10_prod, 20., 5.);
	}
	MethaneProduction(double CH4_RateCoef, double q10_prod, double RateCoefLitter1, double RateCoefHumus) noexcept
	{
		clitter1 = CH4_RateCoef * RateCoefLitter1; clitter2 = 0.; chumus = CH4_RateCoef * RateCoefHumus;
		OrgRateCoef.push_back(clitter1); OrgRateCoef.push_back(clitter2); OrgRateCoef.push_back(chumus);
		f_Temp_Response = Q10(q10_prod, 20., 5.);
	}
	MethaneProduction(double CH4_RateCoef, double q10_prod, double RateCoefLitter1, double RateCoefLitter2, double RateCoefHumus, vector<double> pHValues) noexcept
	{
		clitter1 = CH4_RateCoef * RateCoefLitter1; clitter2 = CH4_RateCoef * RateCoefLitter2; chumus = CH4_RateCoef * RateCoefHumus;
		OrgRateCoef.push_back(clitter1); OrgRateCoef.push_back(clitter2); OrgRateCoef.push_back(chumus);

		f_Temp_Response = Q10(q10_prod, 20., 5.);
		if (pHValues.size() > 0) {
			for(size_t i=0; i<pHValues.size(); i++)
				pH_Influence.push_back(pow(10., -0.2235 * pHValues[i] * pHValues[i] + 2.7727 * pHValues[i] - 8.6));
		}
	}
	MethaneProduction(double CH4_RateCoef,  double q10_prod, double RateCoefLitter1,  double RateCoefHumus, vector<double> pHValues) noexcept
	{
		clitter1 = CH4_RateCoef * RateCoefLitter1; clitter2 = 0.; chumus = CH4_RateCoef * RateCoefHumus;
		OrgRateCoef.push_back(clitter1); OrgRateCoef.push_back(clitter2); OrgRateCoef.push_back(chumus);
		f_Temp_Response = Q10(q10_prod, 20., 5.);
		if (pHValues.size() > 0) {
			for (size_t i = 0; i < pHValues.size(); i++)
				pH_Influence.push_back(pow(10., -0.2235 * pHValues[i] * pHValues[i] + 2.7727 * pHValues[i] - 8.6));
		}
	}

	double operator() (double Litter1, double Litter2, double Humus, double temp) {
		if (temp < 0.) return 0.;

		return f_Temp_Response(temp)*(clitter1 * Litter1 + clitter2 * Litter2 + chumus * Humus);
	}
	double operator() (double Litter1, double Humus, double temp) {
		return f_Temp_Response(temp)*(clitter1 * Litter1 + chumus * Humus);
	}
	vector<double> operator() (const vector<double>& Litter1, const vector<double>& Humus, const vector<double>& temp, const vector<double>& AnaFrac, vector<double>& Prod) {
		for (size_t i = 0; i < Prod.size(); i++) {
			Prod[i] = f_Temp_Response(temp[i]) * (clitter1 * Litter1[i] + chumus * Humus[i])*AnaFrac[i];
			if (pH_Influence.size() == Prod.size())
				Prod[i] *= pH_Influence[i];
		}
		return Prod;
	}
	vector<double> operator() (const vector<double> &Litter1, const vector<double> &Litter2, const vector<double> &Humus, const vector<double> &temp, const vector<double>& AnaFrac, vector<double> &Prod) {
		for (size_t i = 0; i < Prod.size(); i++) {
			Prod[i] = f_Temp_Response(temp[i]) * (clitter1 * Litter1[i] + clitter2 * Litter2[i] + chumus * Humus[i])*AnaFrac[i];
			if (pH_Influence.size() == Prod.size())
				Prod[i] *= pH_Influence[i];
		}
			
		return Prod;
	}
	vector<double> operator() (const vector<double>& Litter1, const vector<double>& Litter2, const vector<double>& Humus, const vector<double>& temp, const vector<double>& AnaFrac, size_t OrgIndex) {
		vector<double> out;
	
		for (size_t i = 0; i < Litter1.size(); i++) {
			out.push_back(f_Temp_Response(temp[i])*AnaFrac[i]);
			if (OrgIndex == 0) 
				out[i] *= OrgRateCoef[OrgIndex] * Litter1[i];
			else if (OrgIndex == 1 && OrgRateCoef.size() == 2) 
				out[i] *= OrgRateCoef[OrgIndex] * Humus[i];
			else
				out[i] *= OrgRateCoef[OrgIndex] * Litter2[i];
			if (pH_Influence.size() >i)
				out[i] *= pH_Influence[i];
		}
		return out;
	}


private:
	double clitter1, clitter2, chumus;
	vector<double> OrgRateCoef;
	vector<double> pH_Influence;
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
	// mg/l = 1E-3 g/(1E-3 m3) = g/m3
public:
	GasDiffusionSoilFlux(double MethaneDiffCoefWater) {
		diffcoef_m2_day = MethaneDiffCoefWater * 86400.; //.E - 6;
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
			double meanaircontent = (AirContent[i - 1] + AirContent[i]) / 2.;//*1.E-6;
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

class SurfaceResistanceCH4 {
public:
	SurfaceResistanceCH4(double CH4_Water_Diff_Coef, double RaSurf) :
		CH4_Water_Diff_Coef(CH4_Water_Diff_Coef),
		RaSurf(RaSurf)
	{}

	double operator() (double pondingdepth, double snowheight, double pondfrac, double soilfrac, double ra) {

		if (pondingdepth > 0.0) { // surface pool exists
			CH4_Resistance_Ponding = pondingdepth / CH4_Water_Diff_Coef; // s/m
			if (snowheight > 0.0) {
				// snow on surface pool
				CH4_Resistance_Snow = snowheight / CH4_Water_Diff_Coef; // s/m
				RaSurf1 = 1.0 / (1.0 / CH4_Resistance_Ponding + 1.0 / CH4_Resistance_Snow + 1.0 / RaSurf); // s/m
			}
			else {
				RaSurf2 = 1.0 / (1.0 / CH4_Resistance_Ponding + 1.0 / RaSurf); // only surface pool, s/m
			}
		}
		else {
			if (snowheight > 0.0) {
				// snow on surface pool
				CH4_Resistance_Snow = snowheight / CH4_Water_Diff_Coef; // only snow on surface, s/m
				RaSurf3 = 1.0 / (1.0 / CH4_Resistance_Snow + 1.0 / RaSurf); // s/m
			}
			else {
				RaSurf4 = 1.0 / RaSurf; // only bare soil, s/m
			}
		}

		double frac1 = min(pondfrac, 1.0 - soilfrac);
		double frac2 = max(pondfrac - min(pondfrac, 1.0 - soilfrac), 0.0001);
		double frac3 = max(1.0 - soilfrac - min(pondfrac, 1.0 - soilfrac), 0.0001);
		double frac4 = max(1.0 - frac1 - frac2 - frac3, 0.0001);

		return 1./(frac1 /RaSurf1 + frac2 / RaSurf2 + frac3 / RaSurf3 + frac4 / RaSurf4); // s/m
	}

private:
    double RaSurf1, RaSurf2, RaSurf3, RaSurf4;
	double CH4_Resistance_Ponding;
	double CH4_Resistance_Snow;
	double CH4_Water_Diff_Coef;
	double RaSurf;
};
