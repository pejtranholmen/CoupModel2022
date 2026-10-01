#pragma once
#include "../../../std.h"
#include "./../../Basic_Functors.h"
#include "./BasicGas_Functors.h"

#include "./NOxide_Functors.h"
#include "./Methane_Functors.h"		
enum class COS_Model_Sw { Off, Plant_Soil};
enum class COS_PlantModel_Sw { Off, Tranpiration_Base, PhotoSynthesis_Base, Experimental, Stomata_Base};
enum class COS_SoilModel_Sw { Off, Heteotrophic_Respiration_Base, GasModel, Ogee};
enum class COS_LitterModel_Sw {Off,On};

class FromAnaerobicToAerobicDiffusion {
public:
	FromAnaerobicToAerobicDiffusion(double DiffRedFrac, double DiffRedFracBase) :a(DiffRedFrac), b(DiffRedFracBase) {}
	double operator () (double anaerobic_frac) {
		return anaerobic_frac * max(0., (1 - anaerobic_frac)) * a + b;
}
private:
	double a, b;
};
class DenitrifiersAnaerobicMoistureResponse {
public:
	DenitrifiersAnaerobicMoistureResponse(double Denit_Act_PowerCoef): powercoef(Denit_Act_PowerCoef)  {}
	double operator() (double anaerobicfrac) {
		return pow(anaerobicfrac, powercoef);
}
private:
	double powercoef;
};
class COS_PlantScaleFlux {
	public:
	COS_PlantScaleFlux(double Cos_Ratio) : Cos_Ratio(Cos_Ratio) {}

	double operator() (double FluxBase) {
		return 	FluxBase * Cos_Ratio;
	}
	private:
		double Cos_Ratio;
};
class COS_SoilScaleFlux {
public:
	COS_SoilScaleFlux(double Cos_Ratio) : Cos_Ratio(Cos_Ratio) {}

	double operator() (double FluxBase) {
		return 	FluxBase * Cos_Ratio;

	}
private:
	double Cos_Ratio;
};

class COS_PlantResistFlux {
public:
	COS_PlantResistFlux(double Coef, double HalfRate) :coef(Coef), rate(HalfRate) {}

	double operator() (double FluxBase) {
		return coef * (1. / (FluxBase * rate + 1.));
	}
private:
	double coef, rate;
};

class COS_PlantRadFlux {
public:
	COS_PlantRadFlux(double Coef, double HalfRate) :coef(Coef), rate(HalfRate) {}

	double operator() (double FluxBase) {
		return coef * FluxBase / (FluxBase + rate);
	}
private:
	double coef, rate;
};

class HenryLawCOSVolatility {
public:
	HenryLawCOSVolatility(double Henry_alpha, double Henry_beta):a(Henry_alpha),b(Henry_beta) {}
	double operator() (double Temp) noexcept {
		double kh_cc = (Temp + Fix::ABS_ZERO) * exp(a + b/(Temp + Fix::ABS_ZERO));
		return kh_cc;
	};

private:
	double a,b;

};

class LitterThetaFunc {
public:
	LitterThetaFunc(double Theta_factor): kl(Theta_factor){}
	double operator() (double Theta) noexcept {
		return sinh(kl*Theta/100.);
	}
private:
	double kl;
};

class EnzymeActivity {
public:
	EnzymeActivity(double G_cat, double H_eq, double T_eq): Gcat(G_cat),Heq(H_eq),Teq(T_eq) {}
	double operator() (double Temp) noexcept {
		double TempResponse = (Temp + Fix::ABS_ZERO) * exp(-Gcat / (Fix::RGAS * (Temp + Fix::ABS_ZERO))) / 
			(1 + exp((-Heq / Fix::RGAS) * (1 / (Temp + Fix::ABS_ZERO) - 1 / Teq))); // K
		double At = 180.28 * exp((-4.e-4) * Gcat); // normalization factor
		return min(1., 1./At * TempResponse);
	}
private:
	double Gcat, Heq, Teq;
};

class RayleighBell {
public:
	RayleighBell(double Theta_opt):wopt(Theta_opt) {}
	double operator() (double Theta) noexcept {
		double ThetaResponse = Theta/100. / (wopt * wopt) * exp(-(Theta/100. / wopt) * (Theta/100. / wopt));
		double Aw = 0.4579 * pow(wopt,-0.937); // normalization factor
		return min(1.,1./Aw * ThetaResponse);
	}
private:
	double wopt;
};

class COS_Michaelis_Menten {
public:
	COS_Michaelis_Menten(double k_cos) noexcept :
		k_cos(k_cos) {}

	double operator() (double conc_cos) {
		return (conc_cos / (k_cos + conc_cos));
	}
private:
	double k_cos;
};

class COSBoundaryLayerFlux {
public:
	COSBoundaryLayerFlux(double ref_conc, double LogScale) noexcept : atm_conc(ref_conc) { // This is the instantiation for dynamic air resistance
		atm_conc = ref_conc * Fix::MOL_RATIO_COS_AIR / Fix::PPT_FRAC; scalecoef = pow(10., LogScale);
		f_AirDensity = AirDensityTempFunction();
	};   // corrected from ra is scaled to convert MassConsAtm and ConSurface from mg/L to g/m3
	
	double operator() (double ConcSurface, double TempSurface, double AirResistance) { //This is the operator for dynamics scaling of surface flux
		f_AirDensity = AirDensityTempFunction();																				// Input units are (mg/L, C, s/m)
		double MassConcAtm = 12. / 60. * 1.E6 * atm_conc * f_AirDensity(TempSurface);
		double flux = (ConcSurface - MassConcAtm) / (scalecoef * AirResistance / Fix::SECPERDAY);
		if (TempSurface <= 0.)
			return max(0., flux);
		else
			return flux;
	};

private:
	double ra, atm_conc, scalecoef;
	function<double(double)> f_AirDensity;

};

class SoilCOSFluxOgee {
public:
	SoilCOSFluxOgee(double alpha, double beta, double pKw, double fca, double DHa, double DHd, double DSd, double zp, double T_ref):
		a(alpha), b(beta), pKw(pKw), fca(fca), DHa(DHa), DHd(DHd), DSd(DSd), zp(zp), T_ref(T_ref) {}
	double operator() (double Ts, double poros, double Theta, double pH, double pressure, double coscas) noexcept {
		double T_s = Ts + Fix::ABS_ZERO; // K
		double Tref = T_ref + Fix::ABS_ZERO; // K
		double theta_w = Theta / 100.;
		double prod = 1.0e-12 * a * exp(b * (T_s - 273.15)); // mol/m3/s
		double inv_T_s = 1.0 / T_s; // 1/K
		double kuncat = 2.15e-5 * exp(-10450.0 * (inv_T_s - 1.0 / Tref)) +
			12.7 * pow(10, (-pKw + pH)) * exp(-6040.0 * (inv_T_s - 1.0 / Tref)); // s-1
		double Rconst = Fix::RGAS; // J/mol/K
		double D0l25 = 1.94e-9; // m2/s
		//double Rconst = 8.3145; // J/(mol K) or m3*Pa/K/mol
		//double DHa = 40.0e3; // J/mol
		//double DHd = 200.0e3; // J/mol
		//double DSd = 660.0; // J/(mol K)
		//double zp = 1.0; // m

		double phi = poros/100.; // porosity
		double ea = max(0.01, phi - theta_w); // theta_w for soil water content

		// Diffusivity, page 2224 Ogee
		double D0a = 1.27e-5 * pow(T_s / 298.15, 1.5); // T_s in K, m2/s
		double D0lT0 = D0l25 / pow((298.15 / 216.0 - 1.0), 2.0); // m2/s
		double D0l = D0lT0 * pow((T_s / 216.0 - 1.0), 2.0); // m2/s

		// Tortuosities, torttype = "Mol03r"
		double ta = (0.2 * pow(ea / phi, 2) + 0.004) / phi;
		double tl = pow(theta_w, 7.0 / 3.0) / pow(phi, 2.0);

		double Deffa = D0a * ta * ea; // m2/s
		double Deffl = D0l * tl * theta_w; // m2/s

		// xCA
		double xCA = exp(-DHa / Rconst * inv_T_s) / (1.0 + exp(-DHd / Rconst * inv_T_s + DSd / Rconst));
		double xCAref = exp(-DHa / (Rconst * 293.0)) / (1.0 + exp(-DHd / (Rconst * 293.0) + DSd / Rconst));

		// Kh, Henry's law constant, p 2223 Ogee
		double Kh = 0.021e-2 * exp(24900.0 * (inv_T_s - 1.0 / 298.15) / Rconst);

		// B, related to Henry's law constant, p 2223 Ogee
		double B = Kh * Rconst * T_s;

		double D = Deffa + Deffl * B; // m2/s
		double mV = Rconst * T_s / pressure; // m3/mol
		double Ca = coscas * 1.0e-12 / mV; // mol/m3

		double k = fca * kuncat * xCA / xCAref; // 1/s

		double z1 = sqrt(D / (k * B * theta_w)); // m
		// Soil COS flux
		double COS_grnd = sqrt(k * B * theta_w * D) * (Ca - (pow(z1, 2) * prod / D) * (1.0 - exp(-zp / z1))); // positive for uptake
		//    mol/m2/s     m/s                           mol/m3

		return COS_grnd * 1.0e12; // pmol/m2/s
	}
private:
	double a, b, pKw, fca, DHa, DHd, DSd, zp, T_ref;
};


