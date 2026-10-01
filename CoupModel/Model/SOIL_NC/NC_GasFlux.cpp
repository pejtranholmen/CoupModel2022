// ============================================================================
// NC_GasFlux - merged K implementation, STEP 1
//
// STEP 1 scope:
//   * K version is the functional baseline.
//   * Ini() / IniFunctors() retain the K transport/functor implementation.
//   * Carbon-bearing atmospheric gas mass concentrations use the CoupModel
//     convention g C and named Fix conversion constants:
//         Fix::CH4_TO_C, Fix::CO2_TO_C, Fix::COS_TO_C.
//   * Remaining process methods are intentionally unchanged from K in this step.
//
// Subsequent steps:
//   2. GeneralUpdates() / O2-CO2 diffusion cleanup
//   3. SoilOxygenCarbonGas_Fluxes()
//   4. MethaneGas_Fluxes()
//   5. COSGas_Fluxes()
// ============================================================================

#include "NC_GasFlux.h"
#include "../SOIL/SoilModel.h"
#include "../PLANT/PlantModel.h"
#include "../SOIL/Soil_HeatPump.h"
#include "./Functors/NC_GasFluxFunctors.h"
#include "../PLANT_NC/PlantOrgModel.h"
#include "../Atmosphere/AtmModel.h"

						   
NC_GasFlux::NC_GasFlux(void)
{
	ModuleName="Gas processes";
	ModuleNo=32;
	ModuleConditions="";
}

bool NC_GasFlux::Ini()	{
	NC_Organic::Ini();
	SoilModel* p_Soil=(SoilModel*)m_pModelInfo->GetModelPtr("Soil Model");
	Soil_HeatPump *pSoilHeat=&p_Soil->m_HeatPump;
	AtmModel* p_AtmModel = (AtmModel*)m_pModelInfo->GetModelPtr("Atm Model");
	double* pSoilEvap = &p_AtmModel->m_Irrigation.RaSoilSurf;
	double Rasurf = *pSoilEvap;
	Irrigation* pMet = (Irrigation*)p_Atm;

	T_Step = *pT_Step;

	DCH4_Tortuosity_Water = pow(10., Diff_CH4LogScale);
	DCH4_Tortuosity_Air = pow(10., Diff_CH4AirLogScale);
	DCOS_Tortuosity_Water = pow(10., Diff_COSLogScale);
	DCOS_Tortuosity_Air = pow(10., Diff_COSAirLogScale);



	f_DiffGas_CH4 = SoilAirDiffusion(GasElement::CH4, 0.66);
	f_DiffGas_O2 = SoilAirDiffusion(GasElement::OXYGEN, 0.66);


	f_DiffWater_CH4 = SoilWaterDiffusion(GasElement::CH4, ArrheniusActWater, ArrheniusTemp0Water, DCH4_Tortuosity_Water);

	f_DiffWater_O2 = SoilWaterDiffusion(GasElement::OXYGEN, ArrheniusActWater, ArrheniusTemp0Water, DO2_Tortuosity);
	GasDiffusionSoilCH4Flux_= GasDiffusionSoilFlux(GasElement::CH4, ArrheniusActWater, ArrheniusTemp0Water, DCH4_Tortuosity_Water, DCH4_Tortuosity_Air);
	GasDiffusionSoilCOSFlux_= GasDiffusionSoilFlux(GasElement::COS, ArrheniusActWater, ArrheniusTemp0Water, DCOS_Tortuosity_Water, DCOS_Tortuosity_Air);

	f_ConvectiveFlux = GasConvectiveSoilFlux();
	f_ConvectiveCOSFlux = GasConvectiveSoilFlux();

	NC_GasFlux::IniFunctors();
//	Use Parameters
//Use Hydraulic, Only	: M_Saturation
		for(size_t i=0; i<Init_Oxygen_Conc.size(); i++) {		
			Oxygen_Conc[i]=Init_Oxygen_Conc[i];
			CO2_Conc[i]=Init_CO2_Conc[i]; //wenxin
			CO2_FluxSoil[i] = 0.0; //wenxin
			Air_Porosity[i]=max(0.01, p_Soil_WaterF->M_Saturation[i]-p_Soil_WaterF->ThetaTotal[i]);
			
			if (!m_pModelInfo->StartStateValue) {
				double z_layer = Air_Porosity[i]/Fix::PERCENT_FRAC * p_Soil_WaterF->AThicknessLayers[i]; // Unit [m]
				Oxygen[i] = z_layer*f_OxygenDensityFrom_Volume_Temp(Oxygen_Conc[i], pSoilHeat->Temp[i]);	// g/m2																												
				CO2Soil[i] =z_layer*f_CarbonDioOxideDensityFrom_Volume_Temp(CO2_Conc[i],pSoilHeat->Temp[i]); //g/m2
			}     
		}

		CH4_AccBubbles=CH4_AccPlant=CH4_AccEmissionSoil=CH4_AccEmissionPlant=CH4_AccProduction=CH4_AccOxidation=0.;
		COS_AccEmission = COS_AccProduction = COS_AccOxic = COS_AccAnoxic = COS_AccSoilSink = 0.;

		OxygenMassConcAtm = f_OxygenDensityFrom_Volume_Temp(O2_CONC_Atm, pSoilHeat->Temp.front());      //g/m-3
		MethaneMassConcAtm = Fix::CH4_TO_C * f_MethaneDensityFrom_Volume_Temp(CH4_CONC_Atm, pSoilHeat->Temp.front());
		CO2MassConcAtm = Fix::CO2_TO_C * f_CarbonDioOxideDensityFrom_Volume_Temp(CO2_CONC_Atm,pSoilHeat->Temp.front());
		COSMassConcAtm = Fix::COS_TO_C * f_COSDensityFrom_Volume_Temp(COS_CONC_Atm, pSoilHeat->Temp.front());
		
		An_VolFraction.assign(p_Soil_WaterF->NL,0.0001); //need to update
		COS_TotSoilUptake = 0.;

		if(Methane_Model_Sw(Methane_Model )> Methane_Model_Sw::off) {
		    
			for (size_t i = 0; i < CH4.size(); i++) {
				CH4[i] = CH4_InitConc * p_Soil_WaterF->WaterStorage[i] * 0.001; // scaling from mg/m2 to g/m2.
			}
			if (LitterPools_Sw(LitterPools) == LitterPools_Sw::two) {
				if (MethaneProductionModel_Sw(MethaneProductionModel) == MethaneProductionModel_Sw::Temp_Function) {
					vector<double> zero_phimpact;
					f_MethaneProdVector_TwoLitter = MethaneProduction(CH4_RateCoefRatio, CH4_ProdQ10, CH4_ProdTref,CH4_ProdTbase, RateCoefLitter1, RateCoefLitter2, RateCoefHumus, zero_phimpact);
					f_MethaneProdVector_ByLitterIndex = MethaneProduction(CH4_RateCoefRatio, CH4_ProdQ10, CH4_ProdTref, CH4_ProdTbase, RateCoefLitter1, RateCoefLitter2, RateCoefHumus, zero_phimpact);
				}
				else {
					f_MethaneProdVector_TwoLitter = MethaneProduction(CH4_RateCoefRatio, CH4_ProdQ10, CH4_ProdTref, CH4_ProdTbase, RateCoefLitter1, RateCoefLitter2, RateCoefHumus, pH_Value);
					f_MethaneProdVector_ByLitterIndex = MethaneProduction(CH4_RateCoefRatio, CH4_ProdQ10, CH4_ProdTref, CH4_ProdTbase, RateCoefLitter1, RateCoefLitter2, RateCoefHumus, pH_Value);
				}
				
			}
			else {
				if (MethaneProductionModel_Sw(MethaneProductionModel) == MethaneProductionModel_Sw::Temp_Function) {
					vector<double> zero_phimpact;
					f_MethaneProdVector_OneLitter = MethaneProduction(CH4_RateCoefRatio, CH4_ProdQ10, CH4_ProdTref, CH4_ProdTbase, RateCoefLitter1, RateCoefHumus, zero_phimpact);
					f_MethaneProdVector_ByLitterIndex = MethaneProduction(CH4_RateCoefRatio, CH4_ProdQ10, CH4_ProdTref, CH4_ProdTbase, RateCoefLitter1, RateCoefHumus, zero_phimpact);
				}
				else {
					f_MethaneProdVector_OneLitter = MethaneProduction(CH4_RateCoefRatio, CH4_ProdQ10, CH4_ProdTref, CH4_ProdTbase, RateCoefLitter1, RateCoefHumus, pH_Value);
					f_MethaneProdVector_ByLitterIndex = MethaneProduction(CH4_RateCoefRatio, CH4_ProdQ10, CH4_ProdTref, CH4_ProdTbase, RateCoefLitter1, RateCoefHumus, pH_Value);
				}

			}
		    CH4_InitStorage=sum(CH4);
		    CH4_DiffTotStorage=sum(CH4)-CH4_InitStorage	;
		}

		if (COS_SoilModel_Sw(COS_SoilModel) == COS_SoilModel_Sw::GasModel) {
			for (size_t i = 0; i < OCS.size(); i++) {
				OCS[i] = COS_InitConc * p_Soil_WaterF->WaterStorage[i]; // ug/m2.
			}
			COS_InitStorage = sum(OCS);
			COS_DiffTotStorage = sum(OCS) - COS_InitStorage;
		}
		

		if(N2O_YearlyFluxes_Sw(N2O_YearlyFluxes)> N2O_YearlyFluxes_Sw::off) {
				DailyValuesN2O.resize(365);
		        DailyValuesN2O.assign(365,0.0);
		        DailyValuesNO.resize(365);
		        DailyValuesNO.assign(365,0.0);
		}

		OxygenGradients.resize(p_Soil_WaterF->NL);
		
		DO2_SoilCoef.resize(p_Soil_WaterF->NL);
		DCO2_SoilCoef.resize(p_Soil_WaterF->NL);  //wenxin
		layer_sat_lag.resize(p_Soil_WaterF->NL);

		DO2_ExCoefSoil.resize(p_Soil_WaterF->NL);
		
		DenitGrowth_NO3.resize(p_Soil_WaterF->NL);
		DenitGrowth_NO2.resize(p_Soil_WaterF->NL);
		DenitGrowth_NO.resize(p_Soil_WaterF->NL);
		DenitGrowth_N2O.resize(p_Soil_WaterF->NL);
		DenitUptakeMin.resize(p_Soil_WaterF->NL);
		DenitUptakeOrg.resize(p_Soil_WaterF->NL);
		DenitUptakeFactor.resize(p_Soil_WaterF->NL);
		
		YearShift=false;
		FullYear=false;			



	//p_Soil_WaterF->WaterE
	return true;
}
bool NC_GasFlux::IniFunctors() {
	NC_Organic::IniFunctors();

	if (Methane_Model_Sw(Methane_Model) >= Methane_Model_Sw::SimpleWithStaticTempControl) {
		f_MethaneOxidationTempResponse = MethaneOxidationTempResponse(CH4_OxidQ10, CH4_OxidTref, CH4_OxidTbase);
		f_MethaneOxidation = Get_D1_Functor(MyFunc::METHANE_OXIDATION);
		f_MethaneOxidationRegulationCH4 = Michaelis_Menten(CH4_HalfRateOxConstant);
		if(WaterDiffusionTempResponse_Sw(WaterDiffusionTempResponse) == WaterDiffusionTempResponse_Sw::Arrhenius)
			f_WaterDiffusionTempFunction = Arrhenius(ArrheniusActWater, ArrheniusTemp0Water);
		
		f_MethaneOxidationRegulationCH4_O2 = Double_Michaelis_Menten(CH4_HalfRateOxConstant, Oxygen_HalfRateOxConstant);
		f_PlantRootRespRegulation = Get_D1_Functor(MyFunc::METHANE_PLANTOXIDATION_BY_ROOTRESPIRATION);
		f_MethaneProduction_TwoLitter = MethaneProduction(CH4_RateCoefRatio, CH4_ProdQ10, CH4_ProdTref, CH4_ProdTbase, RateCoefLitter1, RateCoefLitter2, RateCoefHumus);
		f_MethaneProduction_OneLitter = MethaneProduction(CH4_RateCoefRatio, CH4_ProdQ10, CH4_ProdTref, CH4_ProdTbase, RateCoefLitter1, RateCoefHumus);
		f_AirDensityTempFunction = AirDensityTempFunction();
		
		if (Dissolved_OxygenGas_Sw(Dissolved_OxygenGas) == Dissolved_OxygenGas_Sw::Explicit) {
			f_GasWaterConcCH4 = HenryLawGasWaterConc(CH4_HStandard, CH4_HTempSens, H_TRef);
			f_GasWaterConcO2 = HenryLawGasWaterConc(O2_HStandard, O2_HTempSens, H_TRef);
			f_GasWaterConcCO2 = HenryLawGasWaterConc(CO2_HStandard, CO2_HTempSens, H_TRef);
		}
		else {
			f_MethaneVolatility = Get_D2_Functor(MyFunc::METHANE_VOLATIVITY);
		}

		if (Ebullition_Sw(Ebullition) == Ebullition_Sw::SolubilityBased)
			f_Ebullition = Get_D3_Functor(MyFunc::METHANE_EBULLITION);
		else if (Ebullition_Sw(Ebullition) == Ebullition_Sw::PressureBased)
			//f_EbullitionPress = EbullitionPressure(BubbleFrac, RatioPressThreshold);;
			f_EbullitionPress = EbullitionPressure(CH4_BubbleRate);
		
		f_PlantEmissionbyWaterUptake = MethaneEmissionByTranspiration(CH4_LimitWaterUptake, CH4_WaterPowerCoef, CH4_WaterPlantCoef);
		
		if(Methane_Aerenchyma_Emission_Sw(Methane_Aerenchyma_Emission)== Methane_Aerenchyma_Emission_Sw::on){
		    f_PlantEmissionbyAerenchyma = MethaneEmissionByAerenchyma(MethaneMassConcAtm, AereScale_CH4); 
			f_OxygenDiffusionbyAerenchyma = OxygenDiffusionByAerenchyma(OxygenMassConcAtm, AereScale_O2); 
			}
	
		if (Methane_SoilBoundary_Flux_Sw(Methane_SoilBoundary_Flux) == Methane_SoilBoundary_Flux_Sw::Fixed_Air_Resistance)
			f_MethaneBoundaryLayerFlux = MethaneBoundaryLayerFlux(CH4_CONC_Atm, CH4_ResistanceAir, CH4_ResistanceAirLogScale);
		else
			f_MethaneDynResBoundaryLayerFlux = MethaneBoundaryLayerFlux(CH4_CONC_Atm, CH4_ResistanceAirLogScale);
		
		if (Methane_SoilBoundary_Flux_Sw(Methane_SoilBoundary_Flux) == Methane_SoilBoundary_Flux_Sw::Dynamic_Surface_Pool)
			f_MethaneResistanceSoilSurface = SurfaceResistanceCH4();

		f_MethaneOxidationInPlant = Get_D1_Functor(MyFunc::METHANE_PLANTOXIDATION_OF_WATERUPTAKE);
	}
	if (COS_Model_Sw(COS_Model) > COS_Model_Sw::Off) {
		if (COS_PlantModel_Sw(COS_PlantModel) == COS_PlantModel_Sw::Tranpiration_Base)
			f_COS_Plant = COS_PlantScaleFlux(COS_TranspirationRatio);
		else if (COS_PlantModel_Sw(COS_PlantModel) == COS_PlantModel_Sw::Experimental) {
			f_COS_Plant = COS_PlantScaleFlux(COS_PhotoSynthesisRatio);
			f_COS_PlantTransp = COS_PlantScaleFlux(COS_TranspirationRatio);
			f_COS_PlantTranspPot = COS_PlantScaleFlux(COS_TranspirationPotRatio);
			f_COS_PlantTranspPotMax = COS_PlantScaleFlux(COS_TranspirationPotMaxRatio);
			f_COS_CanopyResist = COS_PlantResistFlux(COS_CanopyResistCoef, COS_HalfCondCanopy);
			f_COS_AirResist = COS_PlantResistFlux(COS_AirResistCoef, COS_HalfCondAir);
			f_COS_RadInShort = COS_PlantRadFlux(COS_RadScaleCoef, COS_RadHalfCoef);
		}
		else if (COS_PlantModel_Sw(COS_PlantModel) == COS_PlantModel_Sw::PhotoSynthesis_Base)
			f_COS_Plant = COS_PlantScaleFlux(COS_PhotoSynthesisRatio);

		if (COS_SoilModel_Sw(COS_SoilModel) == COS_SoilModel_Sw::Heteotrophic_Respiration_Base)
			f_COS_Soil = COS_SoilScaleFlux(COS_SoilRespiratonRatio);
		else if (COS_SoilModel_Sw(COS_SoilModel) == COS_SoilModel_Sw::GasModel) {
			f_GasWaterConcCOS = HenryLawCOSVolatility(Henry_alpha, Henry_beta);
			f_COSProdTempResponse = Q10(COS_ProdQ10, COS_ProdTref, COS_ProdTbase);
			f_COSUptRegulation = COS_Michaelis_Menten(COS_HalfRateUptConstant);
			f_LitterCOSUptThetaResponse = LitterThetaFunc(Theta_factor);
			f_COSOxicRegulation = COS_Michaelis_Menten(COS_HalfRateUptConstant);
			f_COSOxicTempResponse = EnzymeActivity(G_cat, H_eq, T_eq);
			f_COSAnoxicTempResponse = Q10(COS_ProdQ10, COS_ProdTref, COS_ProdTbase);
			f_COSOxicThetaResponse = RayleighBell(Theta_opt);
			f_COSDynResBoundaryLayerFlux = COSBoundaryLayerFlux(COS_CONC_Atm, COS_ResistanceAirLogScale);
		}
		else if (COS_SoilModel_Sw(COS_SoilModel) == COS_SoilModel_Sw::Ogee)
			f_COSSoilOgee = SoilCOSFluxOgee(COSProdAlpha,COSProdBeta,pKw,fCA,DHa,DHd,DSd,Z_max,COS_ProdTref);
	}

	f_AnaerobicFraction = Get_D1_Functor(MyFunc::ANAEROBIC_BALLON);
	f_AnaerobicExchangeEfficiency = Get_D1_Functor(MyFunc::EXCHANGE_EFFICIENY_ANAEROBIC);
	f_DenitrifierAnaerobicFracResponse= Get_D1_Functor(MyFunc::DENIT_ANAEROBIC_FRAC_RESPONSE);
	f_OxygenDiffusionCoef = Get_D2_Functor(MyFunc::DiFFUSION_SOIL_OXYGEN);
	f_CarbonDiOxideDiffusionCoef = Get_D2_Functor(MyFunc::DiFFUSION_SOIL_CO2);
	Ps* pP = (Ps*)m_pModelMap->GetPsPointer("AltSimPosition");
	f_OxygenDensityFrom_Volume_Temp = GasDensityTempFunction(GasElement::OXYGEN, pP->GetValue());
	f_CarbonDioOxideDensityFrom_Volume_Temp = GasDensityTempFunction(GasElement::CO2, pP->GetValue());
	f_MethaneDensityFrom_Volume_Temp = GasDensityTempFunction(GasElement::CH4, pP->GetValue());
	f_MethaneAirConcFrom_water_Temp = GasDensityTempFunction(GasElement::CH4, pP->GetValue());
	f_OxygenVolumeFrom_Density_Temp = GasVolumeTempFunction(GasElement::OXYGEN, pP->GetValue());
	f_CarbonDioOxideVolumeFrom_Density_Temp = GasVolumeTempFunction(GasElement::CO2, pP->GetValue());
	f_COSDensityFrom_Volume_Temp = GasDensityTempFunction(GasElement::COS, pP->GetValue());
	
	f_delta_conc = ChangeConcentration();

	f_NitrogenOxideProduction_Temp = Get_D1_Functor(MyFunc::NITRI_NO_PROD_TEMP_RESPONSE);
	f_NO_Production_Moist = Get_D1_Functor(MyFunc::NITRI_NO_PROD_MOIST_RESPONSE);
	f_N2O_Production_Moist = Get_D1_Functor(MyFunc::NITRI_N2O_PROD_MOIST_RESPONSE);
	f_NO_Production_PH = Get_D1_Functor(MyFunc::NITRI_NO_PROD_PH_RESPONSE);
	f_NO_Production = Get_D1_Functor(MyFunc::NITRI_NO_PROD_RESPONSE);
	f_N2O_Production = Get_D1_Functor(MyFunc::NITRI_N2O_PROD_RESPONSE);

	return true;
}
bool NC_GasFlux::Dyn()
{
	NC_Organic::Dyn();
	Flux();
	NC_Mineral::Integ();
	Integ();
	return true;
}

void NC_GasFlux::Flux()
{
	SoilModel* p_Soil=(SoilModel*)m_pModelInfo->GetModelPtr("Soil Model");
	Soil_HeatPump *pSoilHeat=&p_Soil->m_HeatPump;

	AtmModel* p_AtmModel = (AtmModel*)m_pModelInfo->GetModelPtr("Atm Model");
	double *ra = &p_AtmModel->m_Irrigation.RaSoilSurf;
	double RaSurf = max(0.0001, *ra);
	Irrigation* pMet = (Irrigation*)p_Atm;

	if (Trace_Gas_Emissions_Sw(Trace_Gas_Emissions) > Trace_Gas_Emissions_Sw::off) {
		GeneralUpdates();
		SoilOxygenCarbonGas_Fluxes();
		for (size_t i = 0; i < Oxygen_Conc.size(); i++) {
			if (i >= p_Soil_WaterF->LayerOfSatLevel)
				An_VolFraction[i] = 1.;
			else
				An_VolFraction[i] = f_AnaerobicFraction(Oxygen_Conc[i] / Fix::PERCENT_FRAC);
		}
		NitrousGas_Fluxes();
		MethaneGas_Fluxes();

		if (N2O_YearlyFluxes > 0) {

			if (YearShift && m_pModelInfo->JDayNum == 1) {
				SortDArray(&DailyValuesNO[0], 365);
				SortDArray(&DailyValuesN2O[0], 365);
				if (FullYear) {
					NO_10Percentil = DailyValuesNO[36];
					NO_50Percentil = DailyValuesNO[182];
					NO_90Percentil = DailyValuesNO[329];
					NO_Ymean = sum(DailyValuesNO) / 365.;
					N2O_10Percentil = DailyValuesN2O[36];
					N2O_50Percentil = DailyValuesN2O[182];
					N2O_90Percentil = DailyValuesN2O[329];
					N2O_Ymean = sum(DailyValuesN2O) / 365.;
				}
				else
					FullYear = true;


				DailyValuesNO.assign(365, 0.0);
				DailyValuesN2O.assign(365, 0.0);
				YearShift = false;
			}

			if (m_pModelInfo->JDayNum == 365) {
				YearShift = true;
			}
			if (m_pModelInfo->JDayNum >= 1 && m_pModelInfo->JDayNum <= 365) {
				DailyValuesN2O[int(m_pModelInfo->JDayNum) - 1] += N2O_Rate * T_Step;
				DailyValuesNO[int(m_pModelInfo->JDayNum) - 1] += NO_Rate * T_Step;
			}
		}
	}
	if (COS_Model_Sw(COS_Model) > COS_Model_Sw::Off) {
		COSGas_Fluxes();
	}
}
void NC_GasFlux::GeneralUpdates() {
	SoilModel* p_Soil = (SoilModel*)m_pModelInfo->GetModelPtr("Soil Model");
	Soil_HeatPump* pSoilHeat = &p_Soil->m_HeatPump;

	double	Mean_Saturation, Mean_Air_Porosity, Mean_Temp;
	double OxidationRatio;

	for (size_t i = 0; i < Oxygen_Consumption.size(); i++) {
		double tot_CO2_Consumption = p_NC_Plant->CRootsAtmLayer[i] + p_NC_Plant->CFungiAtmLayer[i] + p_NC_Plant->CCoarseRootsAtmLayer[i] +
			O_RespRate[_L1][i] + O_RespRate[_L2][i] + O_RespRate[_H][i] + O_RespRate[_MIC][i] + O_RespRate[_F][i];
		Oxygen_Consumption[i] = Fix::MOL_RATIO_O2_C * tot_CO2_Consumption;   // g O2/m2/day
		CO2_Production[i] = tot_CO2_Consumption; // gC/m2/day
		Air_Porosity[i] = max(0.01, p_Soil_WaterF->M_Saturation[i] - p_Soil_WaterF->ThetaTotal[i]);  //%
	}
	
	DO2_SoilCoef.front() = f_OxygenDiffusionCoef(Air_Porosity.front(), pSoilHeat->Temp.front()); // m2/day
	DCO2_SoilCoef.front() = f_CarbonDiOxideDiffusionCoef(Air_Porosity.front(), pSoilHeat->Temp.front()); // m2/day

	for (size_t i = 1; i < Air_Porosity.size() - 1; i++) {
		Mean_Saturation = (p_Soil_WaterF->M_Saturation[i - 1] + p_Soil_WaterF->M_Saturation[i]) / 2.;
		Mean_Air_Porosity = (Air_Porosity[i - 1] + Air_Porosity[i]) / 2.;  // Mean value using is vol % 
		Mean_Temp = (pSoilHeat->Temp[i - 1] + pSoilHeat->Temp[i]) / 2.;
		if (i >= p_Soil_WaterF->LayerOfSatLevel) {
			DO2_SoilCoef[i] = pow(10.,Diff_O2AirLogScale) * f_OxygenDiffusionCoef(Mean_Air_Porosity, Mean_Temp); // m2/day
			DCO2_SoilCoef[i] = pow(10.,Diff_CO2AirLogScale) * f_CarbonDiOxideDiffusionCoef(Mean_Air_Porosity, Mean_Temp);
		}
		else {
			DO2_SoilCoef[i] = f_OxygenDiffusionCoef(Mean_Air_Porosity, Mean_Temp); // m2/day
			DCO2_SoilCoef[i] = f_CarbonDiOxideDiffusionCoef(Mean_Air_Porosity, Mean_Temp);
		}

		for (size_t i = 0; i < p_Soil_WaterF->M_Saturation.size(); i++) { // we consider the situation when soil layer is below water table but not saturated, the lagged redox effect
			if (p_Soil_WaterF->Theta[i] < p_Soil_WaterF->M_Saturation[i]) {
				if (i >= p_Soil_WaterF->LayerOfSatLevel && Redoxlag_vertical > 0.)
					layer_sat_lag[i] = layer_sat_lag[i] * exp(-T_Step / Redoxlag_vertical) + (1. - exp(-T_Step / Redoxlag_vertical));
				else if (Redoxlag_vertical > 0.)
					layer_sat_lag[i] = layer_sat_lag[i] * exp(-T_Step / Redoxlag_vertical);
				else if (i >= p_Soil_WaterF->LayerOfSatLevel)
					layer_sat_lag[i] = 1.;
				else
					layer_sat_lag[i] = 0.;
			}
			else
				layer_sat_lag[i] = 1.;
		}
		
	}
	// Added bottom layer values Dec 1 PEJ
	DO2_SoilCoef.back() = pow(10.,Diff_O2AirLogScale) * f_OxygenDiffusionCoef(Air_Porosity.back(), pSoilHeat->Temp.back());
	DCO2_SoilCoef.back() = pow(10., Diff_CO2AirLogScale) * f_CarbonDiOxideDiffusionCoef(Air_Porosity.back(), pSoilHeat->Temp.back());

}
void NC_GasFlux::SoilOxygenCarbonGas_Fluxes() {
	SoilModel* p_Soil = (SoilModel*)m_pModelInfo->GetModelPtr("Soil Model");
	Soil_HeatPump* pSoilHeat = &p_Soil->m_HeatPump;
	AtmModel* p_AtmModel = (AtmModel*)m_pModelInfo->GetModelPtr("Atm Model");
	Irrigation* pMet = &p_AtmModel->m_Irrigation;
	
	if (Oxygen_Model_Sw(Oxygen_Model) == Oxygen_Model_Sw::Steady_state) {
		// Accumulation of oxygen consumption from the bottom of soil profile to get vertical oxygen flux
		double moleRatio_O2 = Fix::MOL_RATIO_O2_AIR / Fix::PERCENT_FRAC;
		Oxygen_Flux.back() = 0.;
		OxygenMassConcAtm = f_OxygenDensityFrom_Volume_Temp(O2_CONC_Atm, pSoilHeat->Temp.front());
		double DiffCoef_O2;

		for (int i = Oxygen_Flux.size() - 2; i >= 0; i--) {
			if (Dissolved_OxygenGas_Sw(Dissolved_OxygenGas) == Dissolved_OxygenGas_Sw::Implicit) {
				Oxygen_Conc_Water[i] = Fix::KILO * Oxygen[i] / p_Soil_WaterF->WaterStorage[i];  // mg/L or g/m3
				O2_CONC_AIR[i] = f_MethaneVolatility(Oxygen_Conc_Water[i], p_NC_Plant->NDrivTemperature[i]);
				O2_CONC_AIR[i] = O2_CONC_AIR[i] / moleRatio_O2 / f_AirDensityTempFunction(p_NC_Plant->NDrivTemperature[i]); // vol%-g
			}
			else {
				Oxygen_Conc_Water[i] = Oxygen[i] / p_Soil_WaterF->AThicknessLayers[i] / (p_Soil_WaterF->Theta[i] / 100. 
					+ Air_Porosity[i] / 100. / f_GasWaterConcO2(p_NC_Plant->NDrivTemperature[i])); //g/m3-w

				O2_CONC_AIR[i] = Oxygen[i] / p_Soil_WaterF->AThicknessLayers[i] / f_GasWaterConcO2(p_NC_Plant->NDrivTemperature[i]);
				O2_CONC_AIR[i] = O2_CONC_AIR[i] / moleRatio_O2 / f_AirDensityTempFunction(p_NC_Plant->NDrivTemperature[i]);// g/m3-g to vol%-g

			}

			Oxygen_Flux[i] = Oxygen_Flux[i + 1] + Oxygen_Consumption[i+1]; // g/m2/day in air 
			
			if (Methane_Aerenchyma_Emission_Sw(Methane_Aerenchyma_Emission) == Methane_Aerenchyma_Emission_Sw::on) {

				if (i >= p_Soil_WaterF->LayerOfSatLevel) {
					double* ra = &p_AtmModel->m_Irrigation.RaSoilSurf;
					double RaAir = max(0.0001, *ra);
					double scalecoef = pow(10, Diff_O2AirLogScale);
					DiffCoef_O2 = scalecoef*f_DiffGas_O2(Air_Porosity[i], pSoilHeat->Temp[i]);

					Aere_temp.assign(p_Soil_WaterF->NL, 0.);
					for (size_t IP = 0; IP < size_t(((WaterUptake*)pPlant)->NumPlants);IP++) {
						DEPTH_type out;
						out.NumLay = 0;
						out = p_Soil_WaterF->DepthDist(((WaterUptake*)pPlant)->RootDistribution, 1., ((WaterUptake*)pPlant)->RootDepth[IP], ((WaterUptake*)pPlant)->RFracLow);
						out = ((WaterUptake*)pPlant)->f_WaterUptakeDistribution(1., ((WaterUptake*)pPlant)->RootDepth[IP]);
						auto RootFrac = out.Dist;
						if (p_NC_Plant->NDrivTemperature[i] > 0. && p_Soil_WaterF->Theta[i] >= p_Soil_WaterF->M_Saturation[i]) {
							double conc_mass = f_OxygenDensityFrom_Volume_Temp(O2_CONC_AIR[i], p_NC_Plant->NDrivTemperature[i]); // g/m3-g
							Aere_temp[i] += f_OxygenDiffusionbyAerenchyma(conc_mass, p_NC_Plant->NDrivTemperature[i], RootFrac[i], p_Soil_WaterF->DepthLayers[i], RaAir,
								((WaterUptake*)pPlant)->LAI[IP], NPPyr[IP], Porosity_Tiller[IP], NPP_Ratio[IP], Rob[IP], DiffCoef_O2);
						}

					}
					O2_Aere[i] = max(0., Aere_temp[i]);
				}
				else
					O2_Aere[i] = 0.;

				Oxygen_Flux[i] = Oxygen_Flux[i] + O2_Aere[i];
			}
			else
				O2_Aere[i] = 0.;
		}
		
		double Oxygen_rainfall = pMet->PrecCorr/ Fix::KILO * O2_CONC_Rain; // g/m2/d, Oxygen flux with rainfall
		Oxygen_Flux_Infil = Oxygen_Flux.front() + Oxygen_Consumption.front() + Oxygen_rainfall;

		// Oxygen concentration in soil profile by stepwise change of concentrations (dc)
		const double deltamassconc = f_delta_conc(Oxygen_Flux_Infil, p_Soil_WaterF->AThicknessLayers.front() / 2., DO2_SoilCoef.front());//g/m3-g, f_OxygenDensityFrom_Volume_Temp(Init_Oxygen_Conc[0], pSoilHeat->Temp.front())); //vol %
		const double deltavolconc = f_OxygenVolumeFrom_Density_Temp(deltamassconc, pSoilHeat->Temp.front()); //vol %
		Oxygen_Conc.front() = O2_CONC_Atm - deltavolconc; //vol %
		Oxygen.front() = f_OxygenDensityFrom_Volume_Temp(Oxygen_Conc.front(), pSoilHeat->Temp.front()) * p_Soil_WaterF->AThicknessLayers.front() * Air_Porosity.front() / 100.; // g/m2

		for (size_t i = 1; i < Air_Porosity.size(); i++) {
			if (Oxygen_Conc[i - 1] <= 0) {
				Oxygen_Conc[i - 1] = 0.;
			}
			
			const double deltamassconc = f_delta_conc(Oxygen_Flux[i-1], p_Soil_WaterF->DistanceMidpLayers[i], DO2_SoilCoef[i]);//g/m3, f_OxygenDensityFrom_Volume_Temp(Oxygen_Conc[i - 1], pSoilHeat->Temp.front())); //vol %
			const double deltavolconc = f_OxygenVolumeFrom_Density_Temp(deltamassconc, pSoilHeat->Temp[i]); //vol %
			Oxygen_Conc[i] = Oxygen_Conc[i - 1] - deltavolconc; //vol%, dc is the concentration change for layer [i-1]
			Oxygen_Conc[i] = max(0., Oxygen_Conc[i]);
			Oxygen[i] = f_OxygenDensityFrom_Volume_Temp(Oxygen_Conc[i], pSoilHeat->Temp[i]) * p_Soil_WaterF->AThicknessLayers[i] * Air_Porosity[i] / 100.; // g/m2
			Oxygen[i] = max(0., Oxygen[i]);
		}

	}
	else {
		//oxygen dynamic

		OutFlow(Oxygen, Oxygen_Consumption);
		for (size_t i = 0; i < Oxygen_Conc.size(); i++) {

			double z_layer = Air_Porosity[i] / Fix::PERCENT_FRAC * p_Soil_WaterF->AThicknessLayers[i]; // Unit [m]
			//Oxygen_Conc[i] = Oxygen[i] / (z_layer * f_OxygenDensityFrom_Volume_Temp(100., pSoilHeat->Temp[i]));
			Oxygen_Conc[i] = f_OxygenVolumeFrom_Density_Temp(Oxygen[i] / z_layer, pSoilHeat->Temp[i]); // vol%
		}

		OxygenMassConcAtm = f_OxygenDensityFrom_Volume_Temp(O2_CONC_Atm, pSoilHeat->Temp[0]);
		Oxygen_Flux_Infil = DO2_SoilCoef.front() * (OxygenMassConcAtm - f_OxygenDensityFrom_Volume_Temp(Oxygen_Conc.front(),pSoilHeat->Temp.front())) / p_Soil_WaterF->AThicknessLayers.front() / 2.;
		//Do i=2, NL,1
		for (size_t i = 1; i < Oxygen_Flux.size(); i++)
			Oxygen_Flux[i - 1] = DO2_SoilCoef[i] * f_OxygenDensityFrom_Volume_Temp(Oxygen_Conc[i - 1] - Oxygen_Conc[i],pSoilHeat->Temp[i-1]) / p_Soil_WaterF->DistanceMidpLayers[i - 1];

		InFlow(Oxygen.front(), Oxygen_Flux_Infil);
		IntFlow(Oxygen, Oxygen_Flux);

	}

	//added by Wenxin

	if (CO2_SoilModel_Sw(CO2_SoilModel) == CO2_SoilModel_Sw::Steady_state) {
		CO2_FluxSoil.back() = 0.;
		double moleRatio_CO2 = Fix::MOL_RATIO_CO2_AIR / Fix::PPM_FRAC;
		CO2MassConcAtm = 12./44. * f_CarbonDioOxideDensityFrom_Volume_Temp(CO2_CONC_Atm, pSoilHeat->Temp.front());
		
		for (int i = CO2_FluxSoil.size() - 2; i >= 0; i--) {
			if (Dissolved_OxygenGas_Sw(Dissolved_OxygenGas) == Dissolved_OxygenGas_Sw::Implicit) {
				CO2_Conc_Water[i] = Fix::KILO * CO2Soil[i] / p_Soil_WaterF->WaterStorage[i];  // mg/L or g/m3, 1mg/L-w is 1 ppm-w
				CO2_CONC_AIR[i] = f_MethaneVolatility(CO2_Conc_Water[i], p_NC_Plant->NDrivTemperature[i]);
				CO2_CONC_AIR[i] = CO2_CONC_AIR[i] * 44./12./ moleRatio_CO2 / f_AirDensityTempFunction(p_NC_Plant->NDrivTemperature[i]); // co2 concentration in ppm
			}
			else {

				CO2_Conc_Water[i] = CO2Soil[i] / p_Soil_WaterF->AThicknessLayers[i] / (p_Soil_WaterF->Theta[i] / 100. 
					+ Air_Porosity[i] / 100. / f_GasWaterConcCO2(p_NC_Plant->NDrivTemperature[i])); //g/m3-w, ppm-w

				CO2_CONC_AIR[i] = CO2Soil[i] / p_Soil_WaterF->AThicknessLayers[i] / f_GasWaterConcCO2(p_NC_Plant->NDrivTemperature[i]);
				CO2_CONC_AIR[i] = CO2_CONC_AIR[i] * 44. / 12. / moleRatio_CO2 / f_AirDensityTempFunction(p_NC_Plant->NDrivTemperature[i]); // ppm-g

			}

			CO2_FluxSoil[i] = CO2_FluxSoil[i + 1] + CO2_Production[i];
		}

		CO2_Flux_Out = CO2_FluxSoil.front() + CO2_Production.front();
		double dc = f_delta_conc(CO2_Flux_Out, p_Soil_WaterF->AThicknessLayers.front() / 2., DCO2_SoilCoef.front());//g/m3, 12. / 44. * f_CarbonDioOxideDensityFrom_Volume_Temp(CO2_CONC_Atm, pSoilHeat->Temp.front())); // PPM
		CO2_Conc.front() = CO2_CONC_Atm + f_CarbonDioOxideVolumeFrom_Density_Temp(44./12.* dc, pSoilHeat->Temp.front()); // ppm
		CO2Soil.front() = 12./44.*f_CarbonDioOxideDensityFrom_Volume_Temp(CO2_Conc.front(), pSoilHeat->Temp.front()) * p_Soil_WaterF->AThicknessLayers.front() * Air_Porosity.front() / 100.;  // g/m2

		for (size_t i = 1; i < Air_Porosity.size(); i++) {
			if (CO2_Conc[i-1] <= 0.) {
				CO2_Conc[i-1] = 0.;
			}

			dc = f_delta_conc(CO2_FluxSoil[i - 1], p_Soil_WaterF->DistanceMidpLayers[i], DCO2_SoilCoef[i]);//g/m3, 12. / 44. * f_CarbonDioOxideDensityFrom_Volume_Temp(CO2_Conc[i], pSoilHeat->Temp.front())); // PPM

			CO2_Conc[i] = CO2_Conc[i - 1] + 44. / 12. * f_CarbonDioOxideVolumeFrom_Density_Temp(dc, pSoilHeat->Temp[i]); // ppm
			//CO2_Conc[i] = max(0.0, min(1.E5, CO2_Conc[i]));				//Per PPM
			CO2Soil[i] = 12. / 44. * f_CarbonDioOxideDensityFrom_Volume_Temp(CO2_Conc[i], pSoilHeat->Temp[i]) * p_Soil_WaterF->AThicknessLayers[i] * Air_Porosity[i] / 100.; //Per volume of CO2 of soil
			CO2Soil[i] = max(0., CO2Soil[i]);
		}

	}
	else if (CO2_SoilModel_Sw(CO2_SoilModel) == CO2_SoilModel_Sw::Dynamic) {

		OutFlow(CO2Soil, CO2_Production);

		for (size_t i = 0; i < CO2_Conc.size(); i++) {
			double z_layer = Air_Porosity[i] / Fix::PERCENT_FRAC * p_Soil_WaterF->AThicknessLayers[i]; // Unit [m]
			//CO2_Conc[i] = CO2Soil[i] * Fix::PPM_FRAC / (z_layer * 12./44.*f_CarbonDioOxideDensityFrom_Volume_Temp(Fix::PPM_FRAC, pSoilHeat->Temp[i]));
			CO2_Conc[i] = 44./12.*f_CarbonDioOxideVolumeFrom_Density_Temp(CO2Soil[i] / z_layer, pSoilHeat->Temp[i]);
		}
		CO2MassConcAtm = 12./44.*f_CarbonDioOxideDensityFrom_Volume_Temp(CO2_CONC_Atm, pSoilHeat->Temp.front());
		CO2_Flux_Out = DCO2_SoilCoef.front() * (CO2MassConcAtm - 12./44.*f_CarbonDioOxideDensityFrom_Volume_Temp(CO2_Conc.front(),pSoilHeat->Temp.front())) / p_Soil_WaterF->AThicknessLayers.front() / 2.;

		for (size_t i = 1; i < CO2_FluxSoil.size(); i++)
			CO2_FluxSoil[i - 1] = DCO2_SoilCoef[i] * 12./44.*f_CarbonDioOxideDensityFrom_Volume_Temp(CO2_Conc[i - 1] - CO2_Conc[i],pSoilHeat->Temp[i-1]) / p_Soil_WaterF->DistanceMidpLayers[i - 1];

		InFlow(CO2Soil.front(), CO2_Flux_Out);
		IntFlow(CO2Soil, CO2_FluxSoil);
	}

}
void NC_GasFlux::NitrousGas_Fluxes() {
	SoilModel* p_Soil = (SoilModel*)m_pModelInfo->GetModelPtr("Soil Model");
	Soil_HeatPump* pSoilHeat = &p_Soil->m_HeatPump;
	AtmModel* p_AtmModel = (AtmModel*)m_pModelInfo->GetModelPtr("Atm Model");

	if (Trace_Gas_Emissions_Sw(Trace_Gas_Emissions) == Trace_Gas_Emissions_Sw::direct_loos) {
		NO_Rate = N2O_Rate = N2_Rate = 0.;
		for (size_t i = 0; i < NitrificationRate.size(); i++) {
			double relsat = p_NC_Plant->NDrivTheta[i] / p_Soil_WaterF->M_Saturation[i];

			NO_NitriFlux[i] = f_NO_Production(NitrificationRate[i]) * f_NO_Production_Moist(relsat) * f_NitrogenOxideProduction_Temp(p_NC_Plant->NDrivTemperature[i]) * f_NO_Production_PH(pH_Value[i]);
			N2O_NitriFlux[i] = f_N2O_Production(NitrificationRate[i]) * f_N2O_Production_Moist(relsat) * f_NitrogenOxideProduction_Temp(p_NC_Plant->NDrivTemperature[i]);

			N2O_Rate = N2O_Rate + N2O_NitriFlux[i];
			NO_Rate = NO_Rate + NO_NitriFlux[i];

			//  Exchange with anaerobic sites

			DO2_ExCoefSoil[i] = f_AnaerobicExchangeEfficiency(relsat) * DO2_SoilCoef[i];

			NO_Exchange[i] = DO2_ExCoefSoil[i] * An_NO[i];

			if (NO_Exchange[i] > 0.)
				RatioNO[i] = min(1., max(0., An_NO[i] / ((NO_Exchange[i] + An_Cons_NO[i]) * T_Step)));
			else
				RatioNO[i] = 0.;

			N2O_Exchange[i] = DO2_ExCoefSoil[i] * An_N2O[i];
			if (N2O_Exchange[i] > 0.)
				RatioN2O[i] = min(1., max(0., An_N2O[i] / ((N2O_Exchange[i] + An_Cons_N2O[i]) * T_Step)));
			else
				RatioN2O[i] = 0.;

			N2_Exchange[i] = min(DO2_ExCoefSoil[i] * An_N2[i], max(0., An_N2[i] / T_Step));
		}

		bool test = DenitrifierUpdate();

		for (size_t i = 0; i < NO_Exchange.size(); i++) {
			NO_Rate += NO_Exchange[i];
			N2O_Rate += N2O_Exchange[i];
			N2_Rate += N2_Exchange[i];
		}

		An_Tot_NO3 = sum(An_NO3);
		An_Tot_NO2 = sum(An_NO2);
		An_Tot_NO = sum(An_NO);
		An_Tot_N2O = sum(An_N2O);
		An_Tot_N2 = sum(An_N2);

		An_TotCons_NO3 = sum(An_Cons_NO3);
		An_TotCons_NO2 = sum(An_Cons_NO2);
		An_TotCons_NO = sum(An_Cons_NO);
		An_TotCons_N2O = sum(An_Cons_N2O);

		An_TotProd_NO2 = sum(An_Prod_NO2);
		An_TotProd_NO = sum(An_Prod_NO);
		An_TotProd_N2O = sum(An_Prod_N2O);
		An_TotProd_N2 = sum(An_Prod_N2);

		N_Tot_AnarobicGasStorage = An_Tot_NO2 + An_Tot_NO + An_Tot_N2O + An_Tot_N2;

		N_Tot_DenitMicrobeStorage = sum(Denitrifier_Biomass);

		N_Tot_DenitDeathRate = sum(Denitrifier_Death);
		N_Tot_DenitGrowthRate = sum(Denitrifier_Growth);

		NTotDenitrification = NO_Rate + N2O_Rate + N2_Rate;

		NO_TotExchange = sum(NO_Exchange);
		N2O_TotExchange = sum(N2O_Exchange);
		N2_TotExchange = sum(N2_Exchange);

		OutFlow(An_NO, NO_Exchange);
		OutFlow(An_N2O, N2O_Exchange);
		OutFlow(An_N2, N2_Exchange);

		OutFlow(SoilNitrate, NO_NitriFlux);
		OutFlow(SoilNitrate, N2O_NitriFlux);
	}
	else if (Trace_Gas_Emissions_Sw(Trace_Gas_Emissions) == Trace_Gas_Emissions_Sw::Vertical_fluxes_in_soil) {

		//  Flux at soil surface
		NO_Rate = DO2_SoilCoef.front() * NO.front() / p_Soil_WaterF->AThicknessLayers.front() / 2.;
		N2O_Rate = DO2_SoilCoef.front() * N2O.front() / p_Soil_WaterF->AThicknessLayers.front() / 2.;
		N2_Rate = DO2_SoilCoef.front() * N2.front() / p_Soil_WaterF->AThicknessLayers.front() / 2.;
		NTotDenitrification = NO_Rate + N2O_Rate + N2_Rate;

		for (size_t i = 0; i < NO_NitriFlux.size(); i++) {
			//	Production by Nitrification
			double relsat = p_NC_Plant->NDrivTheta[i] / p_Soil_WaterF->M_Saturation[i];

			NO_NitriFlux[i] = f_NO_Production(NitrificationRate[i]) * f_NO_Production_Moist(relsat) * f_NitrogenOxideProduction_Temp(p_NC_Plant->NDrivTemperature[i]) * f_NO_Production_PH(pH_Value[i]);
			N2O_NitriFlux[i] = f_N2O_Production(NitrificationRate[i]) * f_N2O_Production_Moist(relsat) * f_NitrogenOxideProduction_Temp(p_NC_Plant->NDrivTemperature[i]);

			DO2_ExCoefSoil[i] = f_AnaerobicExchangeEfficiency(relsat) * DO2_SoilCoef[i];

			NO_Exchange[i] = -DO2_ExCoefSoil[i] * (NO[i] - An_NO[i]);
			N2O_Exchange[i] = -DO2_ExCoefSoil[i] * (N2O[i] - An_N2O[i]);
			N2_Exchange[i] = -DO2_ExCoefSoil[i] * (N2[i] - An_N2[i]);

			if (NO_Exchange[i] > 0.) {
				RatioNO[i] = min(1., max(0., An_NO[i] / ((NO_Exchange[i] + An_Cons_NO[i]) * T_Step)));
				NO_Exchange[i] = RatioNO[i] * NO_Exchange[i];
				An_Cons_NO[i] = RatioNO[i] * An_Cons_NO[i];
				An_Prod_N2O[i] = RatioNO[i] * An_Prod_N2O[i];
			}
			else {
				NO_Exchange[i] = 0.0;
				An_Cons_NO[i] = 0.0;
				An_Prod_N2O[i] = 0.0;
			}

			if (N2O_Exchange[i] > 0.) {
				RatioN2O[i] = min(1., max(0., An_N2O[i] / ((N2O_Exchange[i] + An_Cons_N2O[i]) * T_Step)));
				N2O_Exchange[i] = RatioN2O[i] * N2O_Exchange[i];
				An_Cons_N2O[i] = RatioN2O[i] * An_Cons_N2O[i];
				An_Prod_N2[i] = RatioN2O[i] * An_Prod_N2[i];
			}
			else {
				N2O_Exchange[i] = 0.;
				An_Cons_N2O[i] = 0.;
				An_Prod_N2[i] = 0.;
			}

			if (i >= 1) {
				//  Vertical Flux between Layers
				NO_SoilFlux[i - 1] = -DO2_SoilCoef[i] * (NO[i] - NO[i - 1]) / p_Soil_WaterF->DistanceMidpLayers[i];
				N2O_SoilFlux[i - 1] = -DO2_SoilCoef[i] * (N2O[i] - N2O[i - 1]) / p_Soil_WaterF->DistanceMidpLayers[i];
				N2_SoilFlux[i - 1] = -DO2_SoilCoef[i] * (N2[i] - N2[i - 1]) / p_Soil_WaterF->DistanceMidpLayers[i];
			}

		}

		bool test = DenitrifierUpdate();

		for (size_t i = 0; i < NO_Exchange.size(); i++) {
			NO_Rate += NO_Exchange[i];
			N2O_Rate += N2O_Exchange[i];
			N2_Rate += N2_Exchange[i];
		}

		An_Tot_NO3 = sum(An_NO3);
		An_Tot_NO2 = sum(An_NO2);
		An_Tot_NO = sum(An_NO);
		An_Tot_N2O = sum(An_N2O);
		An_Tot_N2 = sum(An_N2);

		An_TotCons_NO3 = sum(An_Cons_NO3);
		An_TotCons_NO2 = sum(An_Cons_NO2);
		An_TotCons_NO = sum(An_Cons_NO);
		An_TotCons_N2O = sum(An_Cons_N2O);

		An_TotProd_NO2 = sum(An_Prod_NO2);
		An_TotProd_NO = sum(An_Prod_NO);
		An_TotProd_N2O = sum(An_Prod_N2O);
		An_TotProd_N2 = sum(An_Prod_N2);

		N_Tot_AnarobicGasStorage = An_Tot_NO2 + An_Tot_NO + An_Tot_N2O + An_Tot_N2;

		N_Tot_DenitMicrobeStorage = sum(Denitrifier_Biomass);

		OutFlow(NO.front(), NO_Rate);
		OutFlow(N2O.front(), N2O_Rate);
		OutFlow(N2.front(), N2_Rate);

		InFlow(NO, NO_NitriFlux);
		InFlow(N2O, N2O_NitriFlux);
		OutFlow(SoilNitrate, NO_NitriFlux);
		OutFlow(SoilNitrate, N2O_NitriFlux);

		IntFlow(NO, NO_SoilFlux);
		IntFlow(N2O, N2O_SoilFlux);
		IntFlow(N2, N2_SoilFlux);

		InFlow(NO, NO_Exchange);
		InFlow(N2O, N2O_Exchange);
		InFlow(N2, N2_Exchange);

		OutFlow(An_NO, NO_Exchange);
		OutFlow(An_N2O, N2O_Exchange);
		OutFlow(An_N2, N2_Exchange);

	}
	if (Trace_Gas_Emissions_Sw(Trace_Gas_Emissions) > Trace_Gas_Emissions_Sw::off) {
		if (DenitrifierGrowth == 1)
			OutFlow(O_State[_DIS][_N_], Denitrifier_Growth);
		else if (DenitrifierGrowth == 2) {
			if (DenitUptakeMin.front() >= 0.)
				OutFlow(SoilAmmonium, DenitUptakeMin);
			else
				DenitUptakeMin.front() = 0.;

			OutFlow(O_State[_DIS][_N_], DenitUptakeOrg);
		}

	}

}
void NC_GasFlux::MethaneGas_Fluxes() {
	SoilModel* p_Soil = (SoilModel*)m_pModelInfo->GetModelPtr("Soil Model");
	Soil_HeatPump* pSoilHeat = &p_Soil->m_HeatPump;
	AtmModel* p_AtmModel = (AtmModel*)m_pModelInfo->GetModelPtr("Atm Model");
	//RootFrac.assign(CH4.size(), 0.);
	//RootFrac.assign(p_Soil_WaterF->NL, 0.);    
	
	if (Methane_Model_Sw(Methane_Model) > Methane_Model_Sw::off) {
		CH4_TotSoilStorage = sum(CH4);
		CH4_DiffTotStorage = CH4_TotSoilStorage - CH4_InitStorage;
		double loss_check;
		double SaturationScale;
		double DiffCoef_CH4;
		CH4_OxidationPlantSoil.assign(CH4.size(), 0.);
		//double CH4_Ebullition_total; // to store the total ebullition for adding to certain layer

		MethaneMassConcAtm = 12./16. * f_MethaneDensityFrom_Volume_Temp(CH4_CONC_Atm, pSoilHeat->Temp.front()); // g Methane/m3 --> g C/m3

		if (LitterPools_Sw(LitterPools) == LitterPools_Sw::two) {
			if (MethaneProductionModel_Sw(MethaneProductionModel) == MethaneProductionModel_Sw::Temp_Function)
				CH4_Prod = f_MethaneProdVector_TwoLitter(O_State[_L1][_C_], O_State[_L2][_C_], O_State[_H][_C_], p_NC_Plant->NDrivTemperature, An_VolFraction, CH4_Prod);
			else
				CH4_Prod = f_MethaneProdVector_TwoLitter(O_State[_L1][_C_], O_State[_L2][_C_], O_State[_H][_C_], p_NC_Plant->NDrivTemperature, An_VolFraction, CH4_Prod);
			auto Litter2Only = f_MethaneProdVector_ByLitterIndex(O_State[_L1][_C_], O_State[_L2][_C_], O_State[_H][_C_], p_NC_Plant->NDrivTemperature, An_VolFraction, _L2);
			OutFlow(O_State[_L2][_C_], Litter2Only);
		}
		else
			CH4_Prod = f_MethaneProdVector_OneLitter(O_State[_L1][_C_],  O_State[_H][_C_], p_NC_Plant->NDrivTemperature, An_VolFraction, CH4_Prod);
		
		auto Litter1Only = f_MethaneProdVector_ByLitterIndex(O_State[_L1][_C_], O_State[_L2][_C_], O_State[_H][_C_], p_NC_Plant->NDrivTemperature, An_VolFraction, _L1);
		OutFlow(O_State[_L1][_C_], Litter1Only);
		auto HumusOnly = f_MethaneProdVector_ByLitterIndex(O_State[_L1][_C_], O_State[_L2][_C_], O_State[_H][_C_], p_NC_Plant->NDrivTemperature, An_VolFraction, 2);
		OutFlow(O_State[_H][_C_], HumusOnly);

		if (MethaneProductionModel_Sw(MethaneProductionModel) == MethaneProductionModel_Sw::Temp_pH_Redox_Function) {
			for (size_t i = 0; i < CH4.size(); i++) {
				CH4_Prod[i] = CH4_Prod[i] * layer_sat_lag[i];
			}
		}

		InFlow(CH4, CH4_Prod);

		for (size_t i = 0; i < CH4.size(); i++) { // Loop over all soil layes
			if (Dissolved_OxygenGas_Sw(Dissolved_OxygenGas) == Dissolved_OxygenGas_Sw::Implicit) {
				CH4_Conc_Water[i] = Fix::KILO * CH4[i] / p_Soil_WaterF->WaterStorage[i];  // mg/L or g/m3
				CH4_CONC_AIR[i] = f_MethaneVolatility(CH4_Conc_Water[i], p_NC_Plant->NDrivTemperature[i]);
			}
			else { // Explicit by Mousong considering both Methane and Oxygen 

				CH4_Conc_Water[i] = CH4[i] / p_Soil_WaterF->AThicknessLayers[i] / (p_Soil_WaterF->Theta[i] / 100.
					+ Air_Porosity[i] / 100. / f_GasWaterConcCH4(p_NC_Plant->NDrivTemperature[i])); //g/m3-w
				//Oxygen_Conc_Water[i] = Oxygen[i] / p_Soil_WaterF->AThicknessLayers[i] / (p_Soil_WaterF->Theta[i] / 100. 
				//	+ Air_Porosity[i] / 100. / f_GasWaterConcO2(p_NC_Plant->NDrivTemperature[i])); //g/m3-w

				CH4_CONC_AIR[i] = CH4_Conc_Water[i] / f_GasWaterConcCH4(p_NC_Plant->NDrivTemperature[i]);
				//O2_CONC_AIR[i] = Oxygen[i] / p_Soil_WaterF->AThicknessLayers[i] / f_GasWaterConcO2(p_NC_Plant->NDrivTemperature[i]);
			}
		}

		for (size_t i = 0; i < CH4.size(); i++) { // 2nd Loop over all soil layes
			if (i < p_Soil_WaterF->LayerOfSatLevel)
				SaturationScale = pow(10.,OxidUnSat_LogScale);
			else
				SaturationScale = 1.;

			if (Methane_Soil_Oxidation_Sw(Methane_Soil_Oxidation) == Methane_Soil_Oxidation_Sw::on_orignal)
				if (p_NC_Plant->NDrivTemperature[i] > 0.)
					CH4_OxidationSoilBacteria[i] = SaturationScale * f_MethaneOxidation(CH4[i]) *
					f_MethaneOxidationRegulationCH4(CH4_Conc_Water[i], SaturationScale) * (1 - An_VolFraction[i]) *f_MethaneOxidationTempResponse(p_NC_Plant->NDrivTemperature[i]);
				else
					CH4_OxidationSoilBacteria[i] = 0.;

			else
				if (p_NC_Plant->NDrivTemperature[i] > 0.)
					CH4_OxidationSoilBacteria[i] = SaturationScale * f_MethaneOxidation(CH4[i]) *
					f_MethaneOxidationRegulationCH4_O2(CH4_Conc_Water[i], Oxygen_Conc_Water[i], SaturationScale) *f_MethaneOxidationTempResponse(p_NC_Plant->NDrivTemperature[i]);
				else
					CH4_OxidationSoilBacteria[i] = 0.;	
			
			CH4_OxidationSoilBacteria[i] = max(CH4_OxidationSoilBacteria[i], 0.);

			if (Methane_Plant_Oxidation_Sw(Methane_Plant_Oxidation) > Methane_Plant_Oxidation_Sw::off) {
				double RespRate = p_NC_Plant->CRootsAtmLayer[i] + p_NC_Plant->CCoarseRootsAtmLayer[i];
				CH4_OxidationPlantRoot[i] = f_PlantRootRespRegulation(RespRate) *f_MethaneOxidationTempResponse(p_NC_Plant->NDrivTemperature[i]);
			}
			else
				CH4_OxidationPlantRoot[i] = 0.;

			CH4_OxidationPlantSoil[i] = CH4_OxidationSoilBacteria[i] + CH4_OxidationPlantRoot[i];

			if (p_NC_Plant->NDrivTemperature[i] > 0. && i >= p_Soil_WaterF->LayerOfSatLevel) {
				
				if (Ebullition_Sw(Ebullition) == Ebullition_Sw::SolubilityBased)
					CH4_Ebullition[i] = f_Ebullition(CH4_Conc_Water[i], p_Soil_WaterF->WaterStorage[i] / Fix::KILO, p_NC_Plant->NDrivTemperature[i]);
				else if (Ebullition_Sw(Ebullition) == Ebullition_Sw::PressureBased) {
					double pressure = p_NC_Plant->P_Surface + Fix::DENSW * Fix::GRAVCONST * (p_Soil_WaterF->DepthLayers[i] + p_Soil_WaterF->SatLev);
					double pondingdepth = p_AtmModel->m_Irrigation.SurfacePool / Fix::KILO; // Ponding depth, mm-->m
					double pondfrac = p_AtmModel->m_Irrigation.SpoolCover; // Surface pool cover, fraction
					if (pondfrac > 0.)
						pressure += Fix::DENSW * Fix::GRAVCONST * pondingdepth / pondfrac;

					//double vgc = CH4_CONC_AIR[i] /Fix::MOL_C * Fix::RGAS * (p_NC_Plant->NDrivTemperature[i] + Fix::ABS_ZERO) / pressure;  // partial pressure of CH4 in bubbles
					double ppress_ch4 = CH4_CONC_AIR[i] / Fix::MOL_C * Fix::RGAS * (p_NC_Plant->NDrivTemperature[i] + Fix::ABS_ZERO);
					double conc_mass_o2 = f_OxygenDensityFrom_Volume_Temp(O2_CONC_AIR[i], p_NC_Plant->NDrivTemperature[i]);
					double ppress_o2 = conc_mass_o2 /Fix::MOL_O2 * Fix::RGAS * (p_NC_Plant->NDrivTemperature[i] + Fix::ABS_ZERO);
					double conc_mass_co2 = 12./44.*f_CarbonDioOxideDensityFrom_Volume_Temp(CO2_CONC_AIR[i], p_NC_Plant->NDrivTemperature[i]);
					double ppress_co2 = conc_mass_co2 /Fix::MOL_C * Fix::RGAS * (p_NC_Plant->NDrivTemperature[i] + Fix::ABS_ZERO);
					double ppress_n2 = 0.40 * pressure;
					double f_ss = -(pressure - (ppress_ch4 + ppress_o2 + ppress_co2 + ppress_n2)) / (ppress_ch4 + ppress_o2 + ppress_co2 + ppress_n2);
					//CH4_Ebullition[i] = f_EbullitionPress(CH4[i], vgc) * p_Soil_WaterF->AThicknessLayers[i] / T_Step;
					CH4_Ebullition[i] = f_EbullitionPress(CH4_Conc_Water[i], p_Soil_WaterF->WaterStorage[i] / Fix::KILO, f_ss);
				}

			}	
			else
				CH4_Ebullition[i] = 0.;

			////Plants effect, add the aerenchyma transport for considering paddy rice and arctic plants which have aerenchyma, mousong
			// if only one plant is considered, the plantwaterupatake and aerenchyma parameters are specific for the plant, if more than one plant considered, the 
			// parameters are for the ecosystem level, therefore need to be generalized, as we did not iterate over plants here.

			if (Methane_Plant_Emission_Sw(Methane_Plant_Emission) > Methane_Plant_Emission_Sw::off){
                CH4_PlantWaterUptake[i] = f_PlantEmissionbyWaterUptake(((WaterUptake*)pPlant)->WUptakeRate[i], CH4_Conc_Water[i]);
				if(Methane_Aerenchyma_Emission_Sw(Methane_Aerenchyma_Emission) > Methane_Aerenchyma_Emission_Sw::off){
					if (i >= p_Soil_WaterF->LayerOfSatLevel) {
						Aere_temp.assign(p_Soil_WaterF->NL, 0.);
						double* ra = &p_AtmModel->m_Irrigation.RaSoilSurf;
						double RaSurf = max(0.0001, *ra);
						double scalecoef = pow(10, Diff_CH4AirLogScale);
						DiffCoef_CH4 = scalecoef*f_DiffGas_CH4(Air_Porosity[i], p_NC_Plant->NDrivTemperature[i]);

						for (size_t IP = 0; IP < size_t(((WaterUptake*)pPlant)->NumPlants);IP++) {
							DEPTH_type out;
							out.NumLay = 0;
							out = p_Soil_WaterF->DepthDist(((WaterUptake*)pPlant)->RootDistribution, 1., ((WaterUptake*)pPlant)->RootDepth[IP], ((WaterUptake*)pPlant)->RFracLow);
							out = ((WaterUptake*)pPlant)->f_WaterUptakeDistribution(1., ((WaterUptake*)pPlant)->RootDepth[IP]);
							auto RootFrac = out.Dist;
							if (p_NC_Plant->NDrivTemperature[i] > 0. && p_Soil_WaterF->Theta[i] >= p_Soil_WaterF->M_Saturation[i]) {
								Aere_temp[i] += f_PlantEmissionbyAerenchyma(CH4_CONC_AIR[i], p_NC_Plant->NDrivTemperature[i],
									RootFrac[i], p_Soil_WaterF->DepthLayers[i], RaSurf, ((WaterUptake*)pPlant)->LAI[IP], NPPyr[IP], Porosity_Tiller[IP],
									NPP_Ratio[IP], Rob[IP], DiffCoef_CH4) * p_Soil_WaterF->AThicknessLayers[i];
							}
							else
								Aere_temp[i] = 0.;
						}
						CH4_Aere[i] = max(0., Aere_temp[i]);
					}
					else
						CH4_Aere[i] = 0.;
					
					CH4_PlantTransport[i] = CH4_PlantWaterUptake[i] + CH4_Aere[i];
					CH4_OxidationInPlant[i] = f_MethaneOxidationInPlant(CH4_PlantWaterUptake[i]) * f_MethaneOxidationTempResponse(p_NC_Plant->NDrivTemperature[i])
						+ f_MethaneOxidationInPlant(CH4_Aere[i]) * f_MethaneOxidationTempResponse(p_NC_Plant->NDrivTemperature[i]);
				}
				else {
					CH4_Aere[i] = 0.;
					CH4_PlantTransport[i] = CH4_PlantWaterUptake[i];
					CH4_OxidationInPlant[i] = f_MethaneOxidationInPlant(CH4_PlantWaterUptake[i]) * f_MethaneOxidationTempResponse(p_NC_Plant->NDrivTemperature[i]);
				}
			}	
			else {
				CH4_PlantWaterUptake[i] = 0.;
			}

			CH4_Oxidation[i] = CH4_OxidationPlantSoil[i] + CH4_OxidationInPlant[i];

			if (Methane_OxygenStress_Sw(Methane_OxygenStress) > Methane_OxygenStress_Sw::off) {
				O2_Oxidation[i] = CH4_Oxidation[i] * 2. * Fix::MOL_RATIO_O2_C;
				O2_Demand[i] = Oxygen_Consumption[i] + O2_Oxidation[i];
				CH4_Demand[i] = CH4_Oxidation[i] + CH4_Ebullition[i] + CH4_PlantTransport[i] - CH4_OxidationInPlant[i];

				if (O2_Demand[i] > Oxygen[i] / T_Step + O2_Aere[i] && O2_Demand[i] > 0.) {
					OxygenStress[i] = min((Oxygen[i] / T_Step + O2_Aere[i]) / O2_Demand[i], 1.);
				}
				else
					OxygenStress[i] = 1.;

				if (CH4_Demand[i] > 0.) {
					MethaneStress[i] = min((CH4[i] / T_Step) / CH4_Demand[i], 1.);
				}
				else
					MethaneStress[i] = 1.;

				if (OxygenStress[i] < 1. || MethaneStress[i] < 1.) {
					if (MethaneStress[i] < OxygenStress[i]) {
						// recalculate Oxygen limitation
						if (OxygenStress[i] < 1.) {
							O2_Demand[i] = Oxygen_Consumption[i];

							if (O2_Demand[i] > 0.) {
								OxygenStress[i] = (Oxygen[i] / T_Step + O2_Aere[i] -
									MethaneStress[i] * 2. * Fix::MOL_RATIO_O2_C * CH4_Oxidation[i]) / O2_Demand[i];
								OxygenStress[i] = min(OxygenStress[i], 1.);
							}
							else
								OxygenStress[i] = 1.;
						}
						// reset oxidation
						CH4_Oxidation[i] = CH4_Oxidation[i] * MethaneStress[i];
						CH4_OxidationSoilBacteria[i] = MethaneStress[i] * CH4_OxidationSoilBacteria[i];
						CH4_OxidationPlantRoot[i] = MethaneStress[i] * CH4_OxidationPlantRoot[i];
						CH4_OxidationPlantSoil[i] = MethaneStress[i] * CH4_OxidationPlantSoil[i];
						CH4_OxidationInPlant[i] = MethaneStress[i] * CH4_OxidationInPlant[i];
						O2_Oxidation[i] = O2_Oxidation[i] * MethaneStress[i];

					}
					else {
						// recalculate Methane limitation
						if (MethaneStress[i] < 1.) {
							CH4_Demand[i] = CH4_PlantTransport[i] + CH4_Ebullition[i];

							if (CH4_Demand[i] > 0.) {
								MethaneStress[i] = (CH4[i] / T_Step -
									OxygenStress[i] * CH4_Oxidation[i]) / CH4_Demand[i];
								MethaneStress[i] = min(MethaneStress[i], 1.);
							}
							else
								MethaneStress[i] = 1.;
						}
						// reset oxidation
						CH4_Oxidation[i] = CH4_Oxidation[i] * OxygenStress[i];
						CH4_OxidationSoilBacteria[i] = OxygenStress[i] * CH4_OxidationSoilBacteria[i];
						CH4_OxidationPlantRoot[i] = OxygenStress[i] * CH4_OxidationPlantRoot[i];
						CH4_OxidationPlantSoil[i] = OxygenStress[i] * CH4_OxidationPlantSoil[i];
						CH4_OxidationInPlant[i] = OxygenStress[i] * CH4_OxidationInPlant[i];
						O2_Oxidation[i] = O2_Oxidation[i] * OxygenStress[i];

					}
				}
				// reset non-methanotroph demands

				CH4_PlantWaterUptake[i] = CH4_PlantWaterUptake[i] * MethaneStress[i];
				CH4_Ebullition[i] = CH4_Ebullition[i] * MethaneStress[i];
				Oxygen_Consumption[i] = Oxygen_Consumption[i] * OxygenStress[i];

				if (Methane_Aerenchyma_Emission_Sw(Methane_Aerenchyma_Emission) > Methane_Aerenchyma_Emission_Sw::off) {
					CH4_Aere[i] = CH4_Aere[i] * MethaneStress[i];
					CH4_PlantTransport[i] = CH4_PlantWaterUptake[i] + CH4_Aere[i];
				}
				else
					CH4_PlantTransport[i] = CH4_PlantWaterUptake[i];
				
				// below is the adjustment of the fluxes from PEJ
				loss_check = CH4_OxidationPlantSoil[i] + CH4_Ebullition[i] + CH4_PlantTransport[i];

				//if (Methane_Aerenchyma_Emission_Sw(Methane_Aerenchyma_Emission) > Methane_Aerenchyma_Emission_Sw::off)	loss_check += CH4_Aere[i];

				if (loss_check > CH4[i] / T_Step && CH4[i] >= 0.) {
					double lossmaxratio = CH4[i] / T_Step / loss_check * 0.5;
					CH4_Oxidation[i] = lossmaxratio * CH4_Oxidation[i];
					CH4_OxidationSoilBacteria[i] = lossmaxratio * CH4_OxidationSoilBacteria[i];
					CH4_OxidationPlantRoot[i] = lossmaxratio * CH4_OxidationPlantRoot[i];
					CH4_OxidationPlantSoil[i] = lossmaxratio * CH4_OxidationPlantSoil[i];
					CH4_OxidationInPlant[i] = lossmaxratio * CH4_OxidationInPlant[i];
					CH4_Ebullition[i] = lossmaxratio * CH4_Ebullition[i];
					CH4_PlantWaterUptake[i] = lossmaxratio * CH4_PlantWaterUptake[i];
					if (Methane_Aerenchyma_Emission_Sw(Methane_Aerenchyma_Emission) > Methane_Aerenchyma_Emission_Sw::off)
						CH4_Aere[i] = lossmaxratio * CH4_Aere[i];
				}
				else if (CH4[i] < 0.) {
					CH4[i] = 0.;
					CH4_Oxidation[i] = 0.;
					CH4_OxidationSoilBacteria[i] = 0.;
					CH4_OxidationPlantRoot[i] = 0.;
					CH4_OxidationPlantSoil[i] = 0.;
					CH4_OxidationInPlant[i] = 0.;
					CH4_Ebullition[i] = 0.;
					CH4_Aere[i] = 0.;
					CH4_PlantWaterUptake[i] = 0.;
				}
				if (Methane_Aerenchyma_Emission_Sw(Methane_Aerenchyma_Emission) > Methane_Aerenchyma_Emission_Sw::off)
					CH4_PlantTransport[i] = CH4_PlantWaterUptake[i] + CH4_Aere[i];
				else
					CH4_PlantTransport[i] = CH4_PlantWaterUptake[i];
			}

			else {
				// below is the adjustment of the fluxes from PEJ
				loss_check = CH4_OxidationPlantSoil[i] + CH4_Ebullition[i] + CH4_PlantTransport[i];

				//if (Methane_Aerenchyma_Emission_Sw(Methane_Aerenchyma_Emission) > Methane_Aerenchyma_Emission_Sw::off)	loss_check += CH4_Aere[i];

				if (loss_check > CH4[i] / T_Step && CH4[i] >= 0.) {
					double lossmaxratio = CH4[i] / T_Step / loss_check * 0.5;
					CH4_Oxidation[i] = lossmaxratio * CH4_Oxidation[i];
					CH4_OxidationSoilBacteria[i] = lossmaxratio * CH4_OxidationSoilBacteria[i];
					CH4_OxidationPlantRoot[i] = lossmaxratio * CH4_OxidationPlantRoot[i];
					CH4_OxidationPlantSoil[i] = lossmaxratio * CH4_OxidationPlantSoil[i];
					CH4_OxidationInPlant[i] = lossmaxratio * CH4_OxidationInPlant[i];
					CH4_Ebullition[i] = lossmaxratio * CH4_Ebullition[i];
					CH4_PlantWaterUptake[i] = lossmaxratio * CH4_PlantWaterUptake[i];
					if (Methane_Aerenchyma_Emission_Sw(Methane_Aerenchyma_Emission) > Methane_Aerenchyma_Emission_Sw::off)
						CH4_Aere[i] = lossmaxratio * CH4_Aere[i];
				}
				else if (CH4[i] < 0.) {
					CH4[i] = 0.;
					CH4_Oxidation[i] = 0.;
					CH4_OxidationSoilBacteria[i] = 0.;
					CH4_OxidationPlantRoot[i] = 0.;
					CH4_OxidationPlantSoil[i] = 0.;
					CH4_OxidationInPlant[i] = 0.;
					CH4_Ebullition[i] = 0.;
					CH4_Aere[i] = 0.;
					CH4_PlantWaterUptake[i] = 0.;
				}
				if (Methane_Aerenchyma_Emission_Sw(Methane_Aerenchyma_Emission) > Methane_Aerenchyma_Emission_Sw::off)
					CH4_PlantTransport[i] = CH4_PlantWaterUptake[i] + CH4_Aere[i];
				else
					CH4_PlantTransport[i] = CH4_PlantWaterUptake[i];
			}
			
		}

		OutFlowZeroControl(CH4, CH4_OxidationPlantSoil);

		OutFlowZeroControl(CH4, CH4_Ebullition);

		OutFlowZeroControl(CH4, CH4_PlantTransport);
		
		GasDiffusionSoilCH4Flux_.WaterFlux(CH4_Conc_Water, p_Soil_WaterF->M_Saturation, pSoilHeat->Temp, p_Soil_WaterF->DistanceMidpLayers, &CH4_SoilFlux);

		if (MethaneConvectionSoilFlux_Sw(MethaneConvectionSoilFlux) == MethaneConvectionSoilFlux_Sw::on)
			f_ConvectiveFlux(CH4_Conc_Water, p_Soil_WaterF->WaterFlow, &CH4_SoilFlux);

		GasDiffusionSoilCH4Flux_.AirFlux(CH4_CONC_AIR, Air_Porosity, pSoilHeat->Temp, p_Soil_WaterF->DistanceMidpLayers, &CH4_AirSoilFlux);
		//Uppermost layer

		if (Methane_SoilBoundary_Flux_Sw(Methane_SoilBoundary_Flux) == Methane_SoilBoundary_Flux_Sw::Fixed_Air_Resistance)
			CH4_SurfaceEmission = f_MethaneBoundaryLayerFlux(CH4_CONC_AIR[0], pSoilHeat->Temp.front()); // CH4 concentration needs to be g/m3-air
		else if (Methane_SoilBoundary_Flux_Sw(Methane_SoilBoundary_Flux) == Methane_SoilBoundary_Flux_Sw::Dynamic_Air_Resistance)
			CH4_SurfaceEmission = f_MethaneDynResBoundaryLayerFlux(CH4_CONC_AIR[0], pSoilHeat->Temp.front(), p_AtmModel->m_Irrigation.RaSoilSurf);
		else if (Methane_SoilBoundary_Flux_Sw(Methane_SoilBoundary_Flux) == Methane_SoilBoundary_Flux_Sw::Dynamic_Surface_Pool) {
			double snowheight = p_AtmModel->m_Irrigation.HSnow / Fix::KILO; // Snow height, mm-->m
			double pondingdepth = p_AtmModel->m_Irrigation.SurfacePool / Fix::KILO; // Ponding depth, mm-->m
			double pondfrac = p_AtmModel->m_Irrigation.SpoolCover; // Surface pool cover, fraction
			double soilfrac = p_AtmModel->m_Irrigation.FracBare; // Soil cover, fraction
			double scalecoefgas = pow(10, Diff_CH4AirLogScale);
			double DiffusSnow = scalecoefgas*f_DiffGas_CH4(Air_Porosity.front(), pSoilHeat->Temp.front());
			double scalecoefwater = pow(10, Diff_CH4LogScale);
			double DiffusPond = scalecoefwater*f_DiffWater_CH4(100., pSoilHeat->Temp.front());
			double RaSurf_CH4 = f_MethaneResistanceSoilSurface(pondingdepth, snowheight, pondfrac, soilfrac, p_AtmModel->m_Irrigation.RaSoilSurf, DiffusSnow, DiffusPond); // s/m
			CH4_SurfaceEmission = f_MethaneDynResBoundaryLayerFlux(CH4_CONC_AIR[0], pSoilHeat->Temp.front(), RaSurf_CH4);
		}
		
		OutFlowZeroControl(CH4.front(), CH4_SurfaceEmission);
        
		IntFlowZeroControl(CH4, CH4_SoilFlux);
		IntFlowZeroControl(CH4, CH4_AirSoilFlux); // Added  pej Jan 2023 the Diffusive flow in air

		// Summation of fluxes to total fluxes

		CH4_TotSoilProductionFlux = sum(CH4_Prod);
		CH4_TotEbullition = sum(CH4_Ebullition);
		CH4_TotSoilFlux = sum(CH4_Ebullition) + CH4_SurfaceEmission;
		CH4_TotPlantUptake = sum(CH4_PlantTransport);

		CH4_TotSoilOxidation = sum(CH4_OxidationSoilBacteria);

		CH4_TotPlantOxidation = sum(CH4_OxidationInPlant) + sum(CH4_OxidationPlantRoot);
		CH4_TotPlantEmission = CH4_TotPlantUptake - sum(CH4_OxidationInPlant);

		CH4_TotSysFlux = CH4_TotSoilFlux + CH4_TotPlantEmission;

		// Integration of flows to acc variables

		InFlow(CH4_AccBubbles, sum(CH4_Ebullition));
		InFlow(CH4_AccPlant, CH4_TotPlantUptake);
		InFlow(CH4_AccEmissionSoil, CH4_TotSoilFlux);
		InFlow(CH4_AccEmissionPlant, CH4_TotPlantEmission);
		InFlow(CH4_AccProduction, CH4_TotSoilProductionFlux);
		InFlow(CH4_AccOxidation, sum(CH4_Oxidation));
		//InFlow(CH4_AccOxidationPlantSoil, sum(CH4_OxidationPlantSoil));

		CH4_MassBalanceCheck = CH4_AccProduction - CH4_AccEmissionSoil - CH4_DiffTotStorage - CH4_AccOxidation - CH4_AccEmissionPlant;
	}

}
void NC_GasFlux::COSGas_Fluxes() {

	PlantOrgModel* pPlantOrg = (PlantOrgModel*)m_pModelInfo->GetModelPtr("Plant Org Model");
	NC_Plant* pNCPlant;
	pNCPlant = &pPlantOrg->m_NC_Plant;

	PlantModel* pPlant = (PlantModel*)m_pModelInfo->GetModelPtr("Plant Model");
	WaterUptake* p_Plant = &pPlant->m_WaterUptake;

	AtmModel* pAtm = (AtmModel*)m_pModelInfo->GetModelPtr("Atm Model");
	Irrigation* p_Irrig = &pAtm->m_Irrigation;

	SoilModel* p_Soil = (SoilModel*)m_pModelInfo->GetModelPtr("Soil Model");
	Soil_HeatPump* pSoilHeat = &p_Soil->m_HeatPump;

	//AtmModel* p_AtmModel = (AtmModel*)m_pModelInfo->GetModelPtr("Atm Model");
	if (COS_Model_Sw(COS_Model) >= COS_Model_Sw::Off) {
		if (COS_PlantModel_Sw(COS_PlantModel) == COS_PlantModel_Sw::Experimental) {
			COS_TotPlantUptake = f_COS_Plant(sum(pNCPlant->AtmNewMobile));
			COS_Option1 = f_COS_PlantTransp(p_Plant->Transpiration_All);
			COS_Option2 = f_COS_PlantTranspPot(p_Plant->MC_PotTransp[0]);
			COS_Option3 = f_COS_PlantTranspPotMax(p_Plant->MC_PotTranspMax[0]);
			COS_Option4 = f_COS_CanopyResist(p_Plant->MC_ResSurfVar[0]);
			COS_Option5 = f_COS_AirResist(p_Plant->MC_ResAir[0]);
			COS_Option6 = f_COS_RadInShort(p_Irrig->RadInShort);
		}
		else if (COS_PlantModel_Sw(COS_PlantModel) == COS_PlantModel_Sw::PhotoSynthesis_Base)
			COS_TotPlantUptake = f_COS_Plant(sum(pNCPlant->AtmNewMobile));
		else if (COS_PlantModel_Sw(COS_PlantModel) == COS_PlantModel_Sw::Tranpiration_Base)
			COS_TotPlantUptake = f_COS_Plant(p_Plant->Transpiration_All);
		else if (COS_PlantModel_Sw(COS_PlantModel) == COS_PlantModel_Sw::Stomata_Base)
			COS_TotPlantUptake = sum(pNCPlant->COSAtmNewMobile);

		if (COS_SoilModel_Sw(COS_SoilModel) == COS_SoilModel_Sw::Heteotrophic_Respiration_Base) {
			COS_TotSoilUptake = f_COS_Soil(CTotSoilRespRate);
			//COS_EcoSystemFlux = COS_TotPlantUptake + COS_TotSoilUptake;
		}
		else if (COS_SoilModel_Sw(COS_SoilModel) == COS_SoilModel_Sw::Ogee) {
			COS_TotSoilUptake = f_COSSoilOgee(pSoilHeat->Temp[0], p_Soil_WaterF->M_Saturation[1], 
				p_Soil_WaterF->Theta[0], pH_Value[0], p_NC_Plant->P_Surface, p_NC_Plant->coscas); // pmol/m2/s
			COS_TotSoilUptake = COS_TotSoilUptake * 1.e-6 * Fix::MOL_C * Fix::SECPERDAY; // ug/m2/day, here positive for uptake
		}
		else if (COS_SoilModel_Sw(COS_SoilModel) == COS_SoilModel_Sw::GasModel) {
			COS_TotSoilStorage = sum(OCS); // unit ug/m2, due to the ppt conc for COS
			COS_DiffTotStorage = COS_TotSoilStorage - COS_InitStorage;
			double loss_check;
			//double Anavol;

			COSMassConcAtm = 12. / 60. * 1.E6 * f_COSDensityFrom_Volume_Temp(COS_CONC_Atm, pSoilHeat->Temp.front()); // g COS/m3 --> ug C/m3

			for (size_t i = 0; i < OCS.size(); i++) { // Loop over all soil layes
				COS_Conc_Water[i] = OCS[i] / p_Soil_WaterF->AThicknessLayers[i] / (p_Soil_WaterF->Theta[i] / 100. + 
					Air_Porosity[i] / 100. / f_GasWaterConcCOS(p_NC_Plant->NDrivTemperature[i])); //ug/m3-w

				COS_CONC_AIR[i] = COS_Conc_Water[i] / f_GasWaterConcCOS(p_NC_Plant->NDrivTemperature[i]); // ug C/m3

				//Anavol = p_Soil_WaterF->Theta[i]/ max(p_Soil_WaterF->M_Saturation[i],10.);
				//Anavol = min(Anavol, 1.);

				COS_Prod[i] = COSProd_max * p_Soil_WaterF->AThicknessLayers[i] *
					f_COSProdTempResponse(p_NC_Plant->NDrivTemperature[i]); // ug/m2/d

				InFlow(OCS[i], COS_Prod[i]);
				//OCS[i] = OCS[i] + COS_Prod[i];
				if (COS_LitterModel_Sw(COS_LitterModel) > COS_LitterModel_Sw::Off) {
					COS_Litter = LitterCOSProd_max * f_COSProdTempResponse(p_NC_Plant->NDrivTemperature.front()) -
						LitterCOSUpt_max * 1.e6 * f_COSUptRegulation(COS_Conc_Water.front()) * f_LitterCOSUptThetaResponse(p_Soil_WaterF->Theta.front()); // ug/m3/d
					COS_Litter = COS_Litter * Litter_Thickness; // ug/m2/d
					//OCS.front() = OCS.front() + COS_Litter * T_Step; // unit needs check
				}
				else
					COS_Litter = 0.;

				InFlow(OCS.front(), COS_Litter);

				if (Oxic_SoilType == 1.) {
					if (p_NC_Plant->NDrivTemperature[i] > 0.) {
						COS_Oxic[i] = COSOxic_max * f_COSOxicRegulation(COS_Conc_Water[i]) * f_COSOxicTempResponse(p_NC_Plant->NDrivTemperature[i]) *
							f_COSOxicThetaResponse(p_Soil_WaterF->Theta[i]); // g/m3/d
						COS_Oxic[i] = COS_Oxic[i] * 1.E6 * p_Soil_WaterF->AThicknessLayers[i]; // ug/m2/d
					}
					else COS_Oxic[i] = 0.;
					COS_Anoxic[i] = 0.;
				}
				else {
					if (p_NC_Plant->NDrivTemperature[i] > 0.) {
						COS_Anoxic[i] = COS_AnoxicRef * f_COSAnoxicTempResponse(p_NC_Plant->NDrivTemperature[i]); // ug/m2/d
					}
					else COS_Anoxic[i] = 0.;
					COS_Oxic[i] = 0.;
				}

				COS_NetSink[i] = COS_Oxic[i] - COS_Anoxic[i];

				loss_check = COS_NetSink[i];

				if (loss_check > OCS[i] / T_Step && OCS[i] > 0.) {
					double lossmaxratio = OCS[i] / T_Step / loss_check * 0.5;
					COS_NetSink[i] = lossmaxratio * COS_NetSink[i];
					COS_Oxic[i] = lossmaxratio * COS_Oxic[i];
					COS_Anoxic[i] = lossmaxratio * COS_Anoxic[i];
				}
				else if (OCS[i] <= 0.) {
					OCS[i] = 0.;
					COS_NetSink[i] = 0.;
					COS_Oxic[i] = 0.;
					COS_Anoxic[i] = 0.;
				}
			}
			
			OutFlowZeroControl(OCS, COS_NetSink);
			
			GasDiffusionSoilCOSFlux_.WaterFlux(COS_Conc_Water, p_Soil_WaterF->ThetaTotal, pSoilHeat->Temp, p_Soil_WaterF->DistanceMidpLayers, &COS_SoilFlux);// positive for upward
			f_ConvectiveCOSFlux(COS_Conc_Water, p_Soil_WaterF->WaterFlow, &COS_SoilFlux);
			GasDiffusionSoilCOSFlux_.AirFlux( COS_CONC_AIR, Air_Porosity, pSoilHeat->Temp, p_Soil_WaterF->DistanceMidpLayers, &COS_AirSoilFlux);
			// positive for upward

			//Uppermost layer

			COS_SurfaceFlux = f_COSDynResBoundaryLayerFlux(COS_CONC_AIR[0], pSoilHeat->Temp.front(), pAtm->m_Irrigation.RaSoilSurf); // positive for emission, negative for uptake

			//InFlow(OCS.front(), COS_Litter);

			OutFlowZeroControl(OCS.front(), COS_SurfaceFlux);

			// Summation of fluxes to total fluxes

			COS_TotSoilProductionFlux = sum(COS_Prod);
			COS_TotSoilUptake = -COS_SurfaceFlux;
			COS_TotSoilSink = sum(COS_NetSink);

			COS_TotSoilOxic = sum(COS_Oxic);
			COS_TotSoilAnoxic = sum(COS_Anoxic);

			// Integration of flows to acc variables

			InFlow(COS_AccEmission, COS_SurfaceFlux);
			InFlow(COS_AccProduction, COS_TotSoilProductionFlux);
			InFlow(COS_AccProduction, COS_Litter);
			InFlow(COS_AccOxic, COS_TotSoilOxic);
			InFlow(COS_AccAnoxic, COS_TotSoilAnoxic);
			InFlow(COS_AccSoilSink, COS_TotSoilSink);

			IntFlowZeroControl(OCS, COS_SoilFlux);
			IntFlowZeroControl(OCS, COS_AirSoilFlux); // Added  pej Jan 2023 the Diffusive flow in air

			COS_MassBalanceCheck = COS_AccProduction - COS_AccEmission - COS_DiffTotStorage - COS_AccOxic + COS_AccAnoxic;

		}

		COS_EcoSystemFlux = COS_TotSoilUptake + COS_TotPlantUptake;
	}
}

void NC_GasFlux::Integ()
{
}

bool NC_GasFlux::End()
{
return true;
}
NC_GasFlux::~NC_GasFlux(void)
{
}
