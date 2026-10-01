#pragma once
#include "./NC_Organic.h"
#include "./Functors/NC_GasFluxFunctors.h"

class NC_GasFlux_Functions : public NC_Organic
{
public:
    NC_GasFlux_Functions(void);
    ~NC_GasFlux_Functions(void);

    bool Def();
    bool FunctionDef();

    // =====================================================================
    // Model switches / options
    // =====================================================================
    size_t Trace_Gas_Emissions, Oxygen_Model, Methane_Model,
        Methane_Plant_Oxidation, Methane_Soil_Oxidation,
        Methane_Plant_Emission, Methane_SoilBoundary_Flux,
        Methane_Aerenchyma_Emission, Ebullition,
        MethaneConvectionSoilFlux, Methane_OxygenStress;

    size_t WaterDiffusionTempResponse;
    size_t Dissolved_OxygenGas, MethaneProductionModel;
    size_t COS_Model, COS_PlantModel, COS_SoilModel, COS_LitterModel;
    size_t SIF_Model;
    size_t N2O_YearlyFluxes; // CO2_SoilModel moved to AbioticResponses_Functions

    // =====================================================================
    // O2 / CO2 state, fluxes and transport parameters
    // =====================================================================
    vector<double> Oxygen, Oxygen_Flux, Oxygen_Consumption;
    vector<double> CO2Soil, CO2_FluxSoil, CO2_Production;
    vector<double> Init_Oxygen_Conc, Oxygen_Conc, Air_Porosity, Init_CO2_Conc;
    vector<double> Oxygen_Conc_Water, CO2_Conc_Water;
    vector<double> OxygenGradients, DO2_SoilCoef, DO2_ExCoefSoil, DCO2_SoilCoef;

    double DO2_Tortuosity, AnBal_ShapeCoef, IntDiffRedFrac,
        IntDiffRedFracBase, Oxygen_Flux_Infil;
    double DCO2_Tortuosity;
    double DCO2_PowerCoefSoilDiff, DCO2_AirThreshold, DCO2Min_Soil;

    double OxygenMassConcAtm, CO2MassConcAtm;
    double O2_CONC_Atm, O2_CONC_Rain, CO2_CONC_Atm;
    double CO2_Flux_Out;

    double ArrheniusActWater, ArrheniusTemp0Water;

    // =====================================================================
    // Methane: state and layer variables
    // =====================================================================
    vector<double> CH4_Prod, CH4_Oxidation, CH4_SoilFlux, CH4;
    vector<double> RootFrac, CH4_OxidationPlantSoil;
    vector<double> CH4_Ebullition, CH4_PlantWaterUptake,
        CH4_Conc_Water, CH4_PlantTransport;
    vector<double> CH4_Demand, O2_Demand, CH4_Aere, O2_Aere,
        Aere_temp, O2_Oxidation, OxygenStress, MethaneStress;
    vector<double> CH4_OxidationSoilBacteria, CH4_OxidationPlantRoot,
        MethaneBubbleFunc, CH4_OxidationInPlant;
    vector<double> CH4_AirSoilFlux, CH4_CONC_AIR, O2_CONC_AIR,
        CO2_CONC_AIR;

    // =====================================================================
    // Methane: process parameters
    // =====================================================================
    double CH4_RateCoefRatio;
    double CH4_ProdQ10, CH4_ProdTref, CH4_ProdTbase;
    double CH4_OxidQ10, CH4_OxidTref, CH4_OxidTbase;
    double Redoxlag_vertical;

    double CH4_OxidationReactionRate, CH4_HalfRateOxConstant,
        Oxygen_HalfRateOxConstant, CH4_PlantOxidationRate,
        CH4_Water_Diff_Coef, CH4_Snow_Diff_Coef;
    double CH4_EbullMax, CH4_BubbleRateConstant, CH4_BubbleRate;
    double CH4_PlantOxidationEff, CH4_WaterPlantCoef, CH4_ResistanceAir;
    double CH4_PlantOxShape, CH4_LimitWaterUptake, CH4_Air_Diff_Coef;

    double CH4_HStandard, CH4_HTempSens;
    double O2_HStandard, O2_HTempSens;
    double CO2_HStandard, CO2_HTempSens;
    double H_TRef, CH4_WaterPowerCoef, CH4_InitConc, CH4_SurfaceEmission;

    double CH4_CONC_Atm;
    double MethaneMassConcAtm;

    // Aerenchyma
    double AereScale_CH4, AereScale_O2, D_CH4_Aere, D_O2_Aere;
    vector<double> NPPyr, Porosity_Tiller, NPP_Ratio, Rob;

    // Diffusion / resistance scaling
    double CH4_ResistanceAirLogScale;
    double Diff_CH4AirLogScale, Diff_CO2AirLogScale, Diff_O2AirLogScale,
        OxidUnSat_LogScale;
    double Diff_CH4LogScale, Diff_O2LogScale;

    // =====================================================================
    // Methane: accumulated and integrated outputs
    // =====================================================================
    double CH4_AccBubbles, CH4_AccPlant;
    double CH4_AccEmissionSoil, CH4_AccEmissionPlant;
    double CH4_DiffTotStorage, CH4_MassBalanceCheck;
    double CH4_AccProduction, CH4_AccOxidation, CH4_AccOxidationPlantSoil;

    double CH4_TotPlantUptake, CH4_TotPlantEmission,
        CH4_TotPlantOxidation, CH4_TotSoilOxidation;
    double CH4_TotEbullition;
    double CH4_TotSoilFlux, CH4_TotSysFlux;
    double CH4_TotSoilProductionFlux, CH4_TotSoilStorage;
    double CH4_InitStorage;

    // =====================================================================
    // COS / OCS: state, parameters and outputs
    // =====================================================================
    vector<double> OCS, COS_SoilFlux;
    vector<double> COS_Prod, COS_Oxic, COS_Anoxic, COS_NetSink;
    vector<double> COS_Conc_Water;
    vector<double> COS_CONC_AIR, COS_AirSoilFlux;

    double COS_Litter, COS_SurfaceFlux, Litter_Thickness;
    double COS_InitConc, COS_CONC_Atm, COSMassConcAtm;
    double Oxic_SoilType;

    double COS_TotSoilStorage, COSProd_max, LitterCOSProd_max,
        LitterCOSUpt_max, COSOxic_max, COS_AnoxicRef;
    double COS_TotSoilProductionFlux, COS_TotSoilSink,
        COS_TotSoilOxic, COS_TotSoilAnoxic;
    double COS_AccEmission, COS_AccProduction, COS_AccOxic,
        COS_AccAnoxic, COS_AccSoilSink, COS_MassBalanceCheck;
    double COS_DiffTotStorage;
    double COS_InitStorage;

    // COS plant / soil parameters
    double COS_TranspirationRatio, COS_PhotoSynthesisRatio;
    double COS_TranspirationPotMaxRatio, COS_TranspirationPotRatio;
    double COS_AirResistCoef, COS_CanopyResistCoef,
        COS_RadHalfCoef, COS_RadScaleCoef;
    double COS_HalfCondCanopy, COS_HalfCondAir;
    double COS_SoilRespiratonRatio;
    double COS_TotPlantUptake, COS_EcoSystemFlux; // COS_TotSoilUptake removed in K
    double COS_Option1, COS_Option2, COS_Option3,
        COS_Option4, COS_Option5, COS_Option6;

    double Henry_alpha, Henry_beta, COS_ProdQ10, COS_ProdTref,
        COS_ProdTbase, COS_HalfRateUptConstant, Theta_factor;
    double G_cat, H_eq, T_eq, Theta_opt, COS_ResistanceAirLogScale;
    double COSProdAlpha, COSProdBeta, pKw, fCA, DHa, DHd, DSd, Z_max;

    double Diff_COSLogScale, Diff_COSAirLogScale;

    // =====================================================================
    // Nitrogen gases / denitrification
    // =====================================================================
    double NO_Rate, N2O_Rate, N2_Rate;
    double MaxFracN2O, MaxFracNO;
    double N2O_TempMax, N2O_TempOpt, N2O_TempShape,
        N2O_RelSatCrit, N2O_RelSatForm;
    double NO_RelSatCrit, NO_RelSatForm;
    double NO_pH_Coef;
    double Denit_Act_PowerCoef;

    double N2O_10Percentil, N2O_50Percentil, N2O_90Percentil, N2O_Ymean;
    double NO_10Percentil, NO_50Percentil, NO_90Percentil, NO_Ymean;

    vector<double> pH_Value;
    vector<double> layer_sat_lag;
    vector<P*> mTab_Ox, mTab_Ph, mTab_Aere;

    vector<double> NO, N2O, N2;
    vector<double> NO_SoilFlux, N2O_SoilFlux, N2_SoilFlux,
        NO_NitriFlux, N2O_NitriFlux;
    vector<double> NO_Exchange, N2O_Exchange, N2_Exchange;
    double NO_TotExchange, N2O_TotExchange, N2_TotExchange;

    vector<double> DenitGrowth_NO3, DenitGrowth_NO2, DenitGrowth_NO,
        DenitGrowth_N2O, DenitUptakeFactor;
    vector<double> DailyValuesNO, DailyValuesN2O;
    vector<double> DenitUptakeMin, DenitUptakeOrg;

    // =====================================================================
    // COS function bindings
    // =====================================================================
    function<double(double)> f_COS_Plant, f_COS_PlantTransp,
        f_COS_PlantTranspPotMax, f_COS_PlantTranspPot, f_COS_Soil;
    function<double(double)> f_COS_CanopyResist, f_COS_AirResist,
        f_COS_RadInShort;
    function<double(double, double, double, double, double, double)> f_COSSoilOgee;

    function<double(double)> f_COSProdTempResponse, f_COSUptRegulation,
        f_LitterCOSUptThetaResponse, f_COSOxicRegulation;
    function<double(double)> f_COSOxicTempResponse, f_COSAnoxicTempResponse,
        f_COSOxicThetaResponse;
    function<double(double, double, double)> f_COSDynResBoundaryLayerFlux;

    // =====================================================================
    // Methane function bindings
    // =====================================================================
    function<double(double)> f_MethaneOxidation,
        f_MethaneOxidationRegulation, f_PlantRootRespRegulation;
    function<double(double, double)> f_MethaneOxidationRegulationCH4,
        f_MethaneVolatility;
    function<double(double, double, double)> f_MethaneOxidationRegulationCH4_O2;
    function<double(double)> f_MethaneOxidationTempResponse;

    function<double(double, double, double)> f_EbullitionPress;
    function<double(double, double, double)> f_Ebullition;

    function<double(double, double, double)> f_MethaneProduction_OneLitter;
    function<double(double, double, double, double)> f_MethaneProduction_TwoLitter;
    function<vector<double>(vector<double>, vector<double>, vector<double>,
        vector<double>, vector<double>, vector<double>)> f_MethaneProdVector_TwoLitter;
    function<vector<double>(vector<double>, vector<double>, vector<double>,
        vector<double>, vector<double>)> f_MethaneProdVector_OneLitter;
    function<vector<double>(vector<double>, vector<double>, vector<double>,
        vector<double>, vector<double>, size_t OrgIndex)> f_MethaneProdVector_ByLitterIndex;

    function<double(double, double)> f_PlantEmissionbyWaterUptake;
    function<double(double, double)> f_MethaneBoundaryLayerFlux;
    function<double(double, double, double)> f_MethaneDynResBoundaryLayerFlux;
    function<double(double)> f_MethaneOxidationInPlant;
    function<double(double, double, double, double, double, double, double)>
        f_MethaneResistanceSoilSurface;
    function<double(double, double, double, double, double, double,
        double, double, double, double, double)>
        f_PlantEmissionbyAerenchyma, f_OxygenDiffusionbyAerenchyma;
    function<double(double)> f_AnaerobicFraction;

    // =====================================================================
    // Gas transport / concentration conversion function bindings
    // =====================================================================

    function<void(vector<double>, vector<double>, vector<double>, vector<double>, vector<double>*)> f_DiffusionCH4_Flux;
    function<void(vector<double>, vector<double>, vector<double>, vector<double>, vector<double>*)> f_DiffusionCOS_Flux;
    function<double(double)> f_AirDensityTempFunction;

    function<double(double)> f_GasWaterConcCH4, f_GasWaterConcO2,
        f_GasWaterConcCO2, f_GasWaterConcCOS;
	function<double(double)> f_WaterDiffusionTempFunction, f_GasDiffusionTempFunction;
    function<double(double, double)> f_OxygenDiffusionCoef;
    function<double(double, double)> f_CarbonDiOxideDiffusionCoef;

    function<double(double, double)> f_DiffWater_CH4, f_DiffWater_O2;
    function<double(double, double)> f_DiffGas_CH4, f_DiffGas_O2;

    function<double(double, double)> f_OxygenDensityFrom_Volume_Temp,
        f_OxygenVolumeFrom_Density_Temp;
    function<double(double, double)> f_CarbonDioOxideDensityFrom_Volume_Temp,
        f_CarbonDioOxideVolumeFrom_Density_Temp,
        f_MethaneDensityFrom_Volume_Temp;
    function<double(double, double)> f_COSDensityFrom_Volume_Temp;
    function<double(double, double, double)> f_MethaneAirConcFrom_water_Temp;

    function<void(vector<double>, vector<double>, vector<double>*)> f_ConvectiveFlux;
    function<void(vector<double>, vector<double>, vector<double>*)> f_ConvectiveCOSFlux;

	function<void(int, vector<double>, vector<double>, vector<double>, vector<double>, vector<double>, vector<double>*)> f_DiffusionAirSoilFlux;
	function<double(double, double, double)> f_delta_conc;



    // =====================================================================
    // Nitrogen / denitrification function bindings
    // =====================================================================
    function<double(double)> f_NitrogenOxideProduction_Temp;
    function<double(double)> f_NO_Production_Moist;
    function<double(double)> f_N2O_Production_Moist;
    function<double(double)> f_NO_Production_PH;
    function<double(double)> f_NO_Production;
    function<double(double)> f_N2O_Production;
    function<double(double)> f_AnaerobicExchangeEfficiency;
    function<double(double)> f_DenitrifierAnaerobicFracResponse;

    // =====================================================================
    // Miscellaneous state
    // =====================================================================
    bool YearShift, FullYear;

    bool DenitrifierUpdate();
    GasDiffusionSoilFlux GasDiffusionSoilCH4Flux_;
    GasDiffusionSoilFlux GasDiffusionSoilCOSFlux_;

    double DCH4_Tortuosity_Water = pow(10., Diff_CH4LogScale);
    double DCH4_Tortuosity_Air = pow(10., Diff_CH4AirLogScale);
    double DCOS_Tortuosity_Water = pow(10., Diff_COSLogScale);
    double DCOS_Tortuosity_Air = pow(10., Diff_COSAirLogScale);

};
