#include "NC_GasFlux_Functions.h"
#include "../PLANT_NC/PlantOrgModel.h"
#include "../SOIL/SoilModel.h"
#include "../SOIL/Soil_Frost.h"
#include "../PLANT/PlantModel.h"
#include "../PLANT/Salt.h"


NC_GasFlux_Functions::NC_GasFlux_Functions(void)
{


}
NC_GasFlux_Functions::~NC_GasFlux_Functions(void)
{
}


bool NC_GasFlux_Functions::Def()
{
	PLOT_PARAM plotpar;
	plotpar.Num_Of_Curves = 1;
	plotpar.Num_Of_Points = 300;
	plotpar.Num_Of_X = 1;
	plotpar.X_Min = 0;
	plotpar.X_Max = 10;
	plotpar.X_Label = "Independent(g/m2)";
	plotpar.Y_Label = "Dependent (g/m2/day)";
	plotpar.LogXScale = false;
	plotpar.LogYScale = false;
	plotpar.Num_Of_Opt = 1;
	
	Func* pFunc;
	string funcname;
	FunctorProp functorprop;

	vector<Ps*> ps_vec;

	NC_Organic::Def();

	ModuleNo=32;
	SoilModel* p_Soil=(SoilModel*)m_pModelInfo->GetModelPtr("Soil Model");
	Soil_Frost* pSoil=&p_Soil->m_SoilFrost;
	NE *pNL=m_pModelMap->GetNEPointer("Number of Layers");
	PlantModel* pPlant=(PlantModel*)m_pModelInfo->GetModelPtr("Plant Model");

	Salt* p_Plant=&pPlant->m_WaterUptake;
	NE *pNumPlants=m_pModelMap->GetNEPointer("Number of Plants");
	Sw* pSw; Ps *pPs; P *pP; X *pX; T *pT; G *pG; Xs *pXs; Ts *pTs; Gs *pGs;

	pSw= new Sw(&Trace_Gas_Emissions , datatype::SIMPROFILE, elements::AIR, fysprocess::NOFPROC, bioprocess::INITB, userlevel::NORMAL);
		Define(pSw,"Trace Gas Emissions",0,"off|Direct Loss|Vertical fluxes in soil","Nitrogen and Carbon > 0", 3101);
	pSw= new Sw(&CO2_SoilModel, datatype::SIMPROFILE, elements::AIR, fysprocess::NOFPROC, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pSw, "CO2 SoilModel",0,"Off|Steady state|Dynamic","Trace Gas Emissions >0");   //wenxin Added off by pej
	
	
	pSw= new Sw(&Oxygen_Model, datatype::SIMPROFILE, elements::AIR, fysprocess::NOFPROC, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pSw, "Oxygen Model",0,"Steady state|Dynamic","Trace Gas Emissions >0");

	pSw = new Sw(&COS_Model, datatype::SIMPROFILE, elements::AIR, fysprocess::NOFPROC, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
	Define(pSw, "COS Model", 0, "Off|Only Plant|Both Plant and Soil", "Nitrogen and Carbon > 0");
	
	pSw = new Sw(&COS_PlantModel, datatype::SIMPROFILE, elements::AIR, fysprocess::NOFPROC, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
	Define(pSw, "COS PlantModel", 0, "Off|Tranpiration Base|PhotoSynthesis Base|Experimental", "Nitrogen and Carbon > 0|COS Model>0");

	pSw = new Sw(&COS_SoilModel, datatype::SIMPROFILE, elements::AIR, fysprocess::NOFPROC, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
	Define(pSw, "COS SoilModel", 0, "Off|Heteorotrophic Respiration Base", "Nitrogen and Carbon > 0|COS Model>1");

	pGs = new Gs(&COS_TotPlantUptake, datatype::ALLPLANTS, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	Define(pGs, "COS PlantUptake", 0.0,UNIT_TYPES::MIKRO_MASS_FLOW_UNIT, "Nitrogen and Carbon > 0|COS Model>0");
	pGs = new Gs(&COS_TotSoilUptake, datatype::TOTPROFILE, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	Define(pGs, "COS SoilUptake", 0.0,UNIT_TYPES::MIKRO_MASS_FLOW_UNIT, "Nitrogen and Carbon > 0|COS Model>1");
	pGs = new Gs(&COS_EcoSystemFlux, datatype::TOTPROFILE, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	Define(pGs, "COS EcoSystemFlux", 0.0,UNIT_TYPES::MIKRO_MASS_FLOW_UNIT, "Nitrogen and Carbon > 0|COS Model>1");

	pGs = new Gs(&COS_Option1, datatype::TOTPROFILE, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	Define(pGs, "COS Opt1 Transp", 0.0,UNIT_TYPES::MIKRO_MASS_FLOW_UNIT, "Nitrogen and Carbon > 0|COS Model>1");

	pGs = new Gs(&COS_Option2, datatype::TOTPROFILE, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	Define(pGs, "COS Opt2 PotTransp", 0.0,UNIT_TYPES::MIKRO_MASS_FLOW_UNIT, "Nitrogen and Carbon > 0|COS Model>1");
	pGs = new Gs(&COS_Option3, datatype::TOTPROFILE, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	Define(pGs, "COS Opt3 MaxPotTransp", 0.0,UNIT_TYPES::MIKRO_MASS_FLOW_UNIT, "Nitrogen and Carbon > 0|COS Model>1");


	pGs = new Gs(&COS_Option4, datatype::TOTPROFILE, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	Define(pGs, "COS Opt4 CanRes", 0.0,UNIT_TYPES::MIKRO_MASS_FLOW_UNIT, "Nitrogen and Carbon > 0|COS Model>1");

	pGs = new Gs(&COS_Option5, datatype::TOTPROFILE, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	Define(pGs, "COS Opt5 AirRes", 0.0,UNIT_TYPES::MIKRO_MASS_FLOW_UNIT, "Nitrogen and Carbon > 0|COS Model>1");
	pGs = new Gs(&COS_Option6, datatype::TOTPROFILE, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	Define(pGs, "COS Opt6 GlobalRad", 0.0,UNIT_TYPES::MIKRO_MASS_FLOW_UNIT, "Nitrogen and Carbon > 0|COS Model>1");


	pSw= new Sw(&Methane_Model, datatype::SIMPROFILE, elements::AIR, fysprocess::NOFPROC, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
	    Define(pSw, "Methane Model",0,"off|Simplified|Detailed width temp control","Trace Gas Emissions >0");
	pSw = new Sw(&MethaneProductionModel, datatype::SIMPROFILE, elements::AIR, fysprocess::NOFPROC, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pSw, "Methane Production Model", 0, "Temp_Control|Temp_pH_Control", "Trace Gas Emissions >0");
	pSw = new Sw(&Dissolved_OxygenGas, datatype::SIMPROFILE, elements::AIR, fysprocess::NOFPROC, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pSw, "Dissolved oxygen gas", 0, "Implicit|Explicit", "Trace Gas Emissions >0");
	pSw= new Sw(&Methane_Plant_Oxidation , datatype::SIMPROFILE, elements::AIR, fysprocess::NOFPROC, bioprocess::METHANE_OXIDATION, userlevel::NORMAL);
	    Define(pSw,"Methane oxidation by plants",0,"off|on","Nitrogen and Carbon > 0|Methane Model>0");
	pSw = new Sw(&Methane_Soil_Oxidation, datatype::SIMPROFILE, elements::AIR, fysprocess::NOFPROC, bioprocess::METHANE_OXIDATION, userlevel::NORMAL);
		Define(pSw, "Methane Soil oxidation", 1, "off|on original|New", "Nitrogen and Carbon > 0|Methane Model>0");
	pSw= new Sw(&Methane_Plant_Emission , datatype::SIMPROFILE, elements::AIR, fysprocess::NOFPROC, bioprocess::METHANE_PRODUCTION, userlevel::NORMAL);
	    Define(pSw,"Methane emission by plants",0,"off|on","Nitrogen and Carbon > 0|Methane Model>0");
	pSw= new Sw(&Methane_Aerenchyma_Emission , datatype::SIMPROFILE, elements::AIR, fysprocess::NOFPROC, bioprocess::METHANE_PRODUCTION, userlevel::NORMAL);
	    Define(pSw,"Methane emission by aerenchyma",0,"off|on","Nitrogen and Carbon > 0|Methane Model>0");
	pSw = new Sw(&Methane_SoilBoundary_Flux, datatype::SIMPROFILE, elements::AIR, fysprocess::NOFPROC, bioprocess::METHANE_PRODUCTION, userlevel::NORMAL);
		Define(pSw, "Methane SoilBoundary", 0, "Fixed Air Resistance|Dynamic Air Resistance|Dynamic and SurfacePool", "Nitrogen and Carbon > 0|Methane Model>0");
	pSw = new Sw(&MethaneConvectionSoilFlux, datatype::SIMPROFILE, elements::AIR, fysprocess::NOFPROC, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pSw, "Methane Convection SoilFlux", 0, "off|on", "Nitrogen and Carbon > 0|Trace Gas Emissions >0|Methane Model>0");
	pSw = new Sw(&Ebullition, datatype::SIMPROFILE, elements::AIR, fysprocess::NOFPROC, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pSw, "Methane Ebullition", 1, "off|Solubility based| Pressure based", "Trace Gas Emissions >0|Methane Model>0");
	pSw= new Sw(&N2O_YearlyFluxes , datatype::SIMPROFILE, elements::AIR, fysprocess::NOFPROC, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
	    Define(pSw,"N2O Yearly Flux Statistics",0,"off|on","Nitrogen and Carbon > 0|Trace Gas Emissions >0");

	vector<double> initv;
	pX= new X(&CH4,pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
	initv.assign(pSoil->NL,0.);
    Define(pX, pNL  , "Methane content",initv,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0");


////State variables added by Marco
	pXs= new Xs(&CH4_AccBubbles, datatype::ACCUMULATOR, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_PRODUCTION, userlevel::NORMAL);
		Define(pXs,   "Acc CH4 Bubble Emission",0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon >= 1|Methane Model>0");
	pXs= new Xs(&CH4_AccPlant, datatype::ACCUMULATOR, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_PRODUCTION, userlevel::NORMAL);
		Define(pXs,   "Acc CH4 Plant Emission",0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon >= 1|Methane Model>0") ;
	pXs= new Xs(&CH4_AccEmission, datatype::ACCUMULATOR, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
	    Define(pXs,  "Acc CH4 Emission",0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon >= 1|Methane Model>0") ;
	pXs= new Xs(&CH4_DiffTotStorage, datatype::ACCUMULATOR, elements::METHANE, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
	    Define(pXs,   "Diff CH4 Storage",0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon >= 1|Methane Model>0") ;
	pXs= new Xs(&CH4_MassBalanceCheck, datatype::ACCUMULATOR, elements::METHANE, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
	    Define(pXs,   "CH4 MassbalanceCheck",0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon >= 1|Methane Model>0") ;
	pXs= new Xs(& CH4_AccProduction, datatype::ACCUMULATOR, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_PRODUCTION, userlevel::NORMAL);
	    Define(pXs,  "Acc CH4 Production",0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon >= 1|Methane Model>0") ;
	pXs= new Xs(&CH4_AccOxidation, datatype::ACCUMULATOR, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL);
	    Define(pXs,   "Acc CH4 Oxidation",0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon >= 1|Methane Model>0") ;
	initv.assign(pSoil->NL,0.);
	pT= new T(&CH4_Prod ,pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_PRODUCTION, userlevel::NORMAL); 
		Define(pT, pNL  , "CH4 Source flux",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0");
	pT= new T(&CH4_Oxidation,pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL); 
	    Define(pT, pNL  , "CH4 Sink flux",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0");
	pT= new T(&CH4_SoilFlux,pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL); 
		Define(pT, pNL  , "CH4 soil water flux",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0");

	pT = new T(&O2_Aere, pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::TRANSPORT, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
		Define(pT, pNL, "Oxygen Flux Aerenchyma", initv, UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0 |Methane Model>0|Methane emission by aerenchyma>0");
	pT = new T(&CH4_Aere, pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::TRANSPORT, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
		Define(pT, pNL, "Methane Flux Aerenchyma", initv, UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0 |Methane Model>0|Methane emission by aerenchyma>0");
	pT = new T(&O2_Demand, pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::TRANSPORT, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
		Define(pT, pNL, "Oxygen Demand", initv, UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0 |Methane Model>0|Dissolved oxygen gas>0");
	pT = new T(&CH4_Demand, pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::TRANSPORT, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
		Define(pT, pNL, "Methane Demand", initv, UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0 |Methane Model>0|Dissolved oxygen gas>0");
	pT = new T(&O2_Oxidation, pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::TRANSPORT, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
		Define(pT, pNL, "Oxygen Oxidation", initv, UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0 |Methane Model>0|Dissolved oxygen gas>0");
	pT = new T(&OxygenStress, pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::TRANSPORT, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
		Define(pT, pNL, "Oxygen Stress", initv, "-", "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0 |Methane Model>0|Dissolved oxygen gas>0");
	pT = new T(&MethaneStress, pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::TRANSPORT, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
		Define(pT, pNL, "Methane Stress", initv, "-", "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0 |Methane Model>0|Dissolved oxygen gas>0");
	;
////Vectors added by Marco
	pT= new T(&CH4_OxidationSoilBacteria ,pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL); 
		Define(pT, pNL  , "CH4 aerobic oxidation",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0");
	pT= new T(&CH4_OxidationPlantRoot ,pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL); 	
		Define(pT, pNL  , "CH4 plant oxidation",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0");
 ;
	pT= new T(&CH4_Ebullition ,pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::EMISSION, bioprocess::METHANE_PRODUCTION, userlevel::NORMAL); 
		Define(pT, pNL  , "CH4 ebullition rate",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0");
	pT= new T(&CH4_PlantWaterUptake ,pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL); 
		Define(pT, pNL  , "CH4 plant uptake",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0");
	pT = new T(&CH4_PlantTransport, pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL);
		Define(pT, pNL, "CH4 plant transport", initv, UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0|Methane emission by aerenchyma>0");
	pTs= new Ts(&CH4_SurfaceEmission, datatype::SOILSURFACE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
	    Define(pTs,   "CH4 surface Emission",0.,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon >= 1|Methane Model>0");
	pT= new T(&CH4_AirSoilFlux ,pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
	    Define(pT, pNL  , "CH4 soil air flux",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0");

  ;
// CH4 Parameters;
  functorprop.FuncName = MyFunc::METHANE_PRODUCTION_FROM_ONE_LITTER; functorprop.Arg = MyArgType::D3;
  functorprop.FunctorInitOption = std::string::npos;
  funcname = "Anaerobic Methane Production Function";
  plotpar.X_Label = "Amount Litter1 (g/m2)";
  plotpar.X_Min = 0;
  plotpar.X_Max = 10.;
  plotpar.Y_Label = "Potential Methane Production (";
  std::string unit = string(p_ModelInfo->GetUnitString(UNIT_TYPES::mMASSFLOW_UNIT)) ; plotpar.Y_Label += unit;
  plotpar.Num_Of_Curves = 3.;
  plotpar.Num_Of_X = 3;
  plotpar.Num_Of_Opt = 1;
  plotpar.X2_Vec = { 0.,0., 0. };
  plotpar.X3_Vec = { 0.,5.,10. };
  plotpar.Y_ScalingCoef = 3;
  plotpar.LegendsVec = { "Only Litter1 at 0C","Only Litter1 at 5C", "Only Litter1 at 10C" };


	pPs= new Ps(&CH4_RateCoefRatio, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_PRODUCTION, userlevel::NORMAL); 
	    Define(pPs,  "CH4 RateCoefRatio",.003, UNIT_TYPES::RATECOEF_UNIT,"Nitrogen and Carbon >=1|Methane Model>0",0.,50.); ps_vec.push_back(pPs);
	pPs = new Ps(&CH4_ProductionQ10, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_PRODUCTION, userlevel::NORMAL);
		Define(pPs, "CH4 ProductionQ10", 3., UNIT_TYPES::NO_UNIT, "Nitrogen and Carbon >=1|Methane Model>1", 1., 5.); ps_vec.push_back(pPs);
    pFunc = new Func( datatype::TOTPROFILE, elements::METHANE, fysprocess::NOFPROC, bioprocess::METHANE_PRODUCTION, userlevel::NORMAL);
	ps_vec.push_back((Ps*)m_pModelMap->GetPsPointer("RateCoefLitter1"));
	ps_vec.push_back((Ps*)m_pModelMap->GetPsPointer("RateCoefLitter2"));
	ps_vec.push_back((Ps*)m_pModelMap->GetPsPointer("RateCoefHumus"));

	Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();

;
functorprop.FuncName = MyFunc::METHANE_PLANTOXIDATION_OF_WATERUPTAKE; functorprop.Arg = MyArgType::D1;
functorprop.FunctorInitOption = std::string::npos;
funcname = "Oxidation of Methane from Water Uptake Function";
plotpar.X_Label = "Methane uptake by waterflow ";
unit = p_ModelInfo->GetUnitString(UNIT_TYPES::MIKRO_MASS_FLOW_UNIT, true);
plotpar.X_Label += unit;
plotpar.X_Min = 0;
plotpar.X_Max = 1.E-4;
plotpar.Y_Label = "Plant Oxidation (";
plotpar.Y_Label += unit;
plotpar.Num_Of_X = 1;
plotpar.Num_Of_Opt = 0;
plotpar.X2_Vec = { };
plotpar.LegendsVec = { "" };
plotpar.X_ScalingCoef = 6;
plotpar.Y_ScalingCoef = 6;

	pPs= new Ps(&CH4_PlantOxidationEff , datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL); 
	    Define(pPs,  "CH4 PlantOxidationEff",1.0, UNIT_TYPES::NO_UNIT,"Nitrogen and Carbon >=1|Methane Model>0|PlanType>0",0.,1.);	 ps_vec.push_back(pPs);

	pFunc = new Func( datatype::TOTPROFILE, elements::METHANE, fysprocess::NOFPROC, bioprocess::METHANE_OXIDATION, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();
	
	ps_vec.clear();
	functorprop.FuncName = MyFunc::METHANE_OXIDATION; functorprop.Arg = MyArgType::D1;
	functorprop.FunctorInitOption = std::string::npos;
	funcname = "Maximum Oxidation Rate Function";
	plotpar.X_Label = "Amount of Methane (g/m2)";
	plotpar.X_Min = 0;
	plotpar.X_Max = 1.;
	plotpar.Y_Label = "Oxidation Rate (g/m2/day)";
	plotpar.Num_Of_X = 1;
	plotpar.Num_Of_Opt = 0;
	plotpar.X2_Vec = { };
	plotpar.LegendsVec = {""};
	plotpar.X_ScalingCoef = 0;
	plotpar.Y_ScalingCoef = 0;
		
	pPs= new Ps(&CH4_OxidationReactionRate, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL); 
	Define(pPs, "CH4 AerobicOxRate", 0.1, UNIT_TYPES::RATECOEF_UNIT, "Nitrogen and Carbon >=1|Methane Model>0", 0., 50., 0., 1.); ps_vec.push_back(pPs);
	
	pFunc = new Func( datatype::TOTPROFILE, elements::METHANE, fysprocess::NOFPROC, bioprocess::METHANE_OXIDATION, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();


	functorprop.FuncName = MyFunc::METHANE_OXIDATION_REGULATION_BY_CH4; functorprop.Arg = MyArgType::D1;
	functorprop.FunctorInitOption = std::string::npos;
	funcname = "Oxidation Regulation by CH4 concentration";
	plotpar.X_Label = "Methane Water Conc(";
	unit = p_ModelInfo->GetUnitString(UNIT_TYPES::mMASS_LITER_UNIT); unit += ")"; plotpar.X_Label += unit;
	plotpar.X_Min = 0;
	plotpar.X_Max = 100;
	plotpar.X_ScalingCoef = 0;
	plotpar.Y_Label = "Methane Oxidation(-)";

	pPs= new Ps(&CH4_HalfRateOxConstant, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL); 
	Define(pPs,   "CH4 HalfRateOxConstant",40, UNIT_TYPES::mMASS_LITER_UNIT,"Nitrogen and Carbon >=1|Methane Model>0",0.,100.);	ps_vec.push_back(pPs);
	

	pFunc = new Func( datatype::TOTPROFILE, elements::METHANE, fysprocess::NOFPROC, bioprocess::METHANE_OXIDATION, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();

	pPs = new Ps(&Oxygen_HalfRateOxConstant, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL);
	Define(pPs, "Oxygen HalfRateOxConstant", 160, UNIT_TYPES::mMASS_LITER_UNIT, "Nitrogen and Carbon >=1|Methane Model>0", 0., 300.);//	ps_vec.push_back(pPs);

	functorprop.FuncName = MyFunc::METHANE_EBULLITION; functorprop.Arg = MyArgType::D2;
		funcname = "Methane Ebullition Rate Production";
		plotpar.X_Label = "Methane Water Concentration ("; unit = p_ModelInfo->GetUnitString(UNIT_TYPES::mMASS_LITER_UNIT); unit += ")"; plotpar.X_Label += unit;
		plotpar.X_Min = 0;
		plotpar.X_Max = 400;
		plotpar.Num_Of_Curves = 3;
		plotpar.Num_Of_Opt = 3;
		plotpar.X_ScalingCoef = 0;
		plotpar.Y_ScalingCoef = 0;
		plotpar.X2_Vec = { 0.,10., 20. };
		plotpar.Y_Label = "Methane Ebullition (";  unit = p_ModelInfo->GetUnitString(UNIT_TYPES::mMASSFLOW_UNIT); unit += ")"; plotpar.Y_Label += unit;
		plotpar.LegendsVec.clear();
		string lstr = string(p_ModelInfo->GetUnitString(UNIT_TYPES::TEMP_UNIT));
		plotpar.LegendsVec.push_back("0 " + lstr);  plotpar.LegendsVec.push_back("10 " + lstr); plotpar.LegendsVec.push_back("20 " + lstr);
	
	pPs = new Ps(&CH4_BubbleRateConstant, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
		Define(pPs, "CH4 BubbleRateConstant", 2.8, "-", "Nitrogen and Carbon >=1|Methane Model>0|Methane Ebullition=1", 0., 4.);	ps_vec.push_back(pPs);
	pPs= new Ps(&CH4_EbullMax, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL); 
		Define(pPs, "CH4 EbullThreshold", 20. , UNIT_TYPES::mMASS_LITER_UNIT, "Nitrogen and Carbon >=1|Methane Model>0|Methane Ebullition=1", 0.1, 1E3);	ps_vec.push_back(pPs);
	
	pFunc = new Func( datatype::TOTPROFILE, elements::METHANE, fysprocess::NOFPROC, bioprocess::METHANE_OXIDATION, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();
	
		


;

    pPs = new Ps(&BubbleFrac, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
        Define(pPs, "CH4 Frac in Bubble", 0.57, UNIT_TYPES::RATECOEF_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0|Methane Ebullition=2", 1.E-02, 1.);
	pPs = new Ps(&RatioPressThreshold, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
		Define(pPs, "CH4 Frac in Bubble", 0.15, UNIT_TYPES::RATECOEF_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0||Methane Ebullition=2", 1.E-02, .2);


functorprop.FuncName = MyFunc::METHANE_PLANTOXIDATION_REGULATION_BY_ROOTRESPIRATION; functorprop.Arg = MyArgType::D1;
functorprop.FunctorInitOption = std::string::npos;
funcname = "Plant Oxidation Regulation by root respiration";
plotpar.X_Label = "Root Respiration Rate (gC/m3/day)";
plotpar.X_Min = 0;
plotpar.X_Max = 1.;
plotpar.Num_Of_Curves = 1;
plotpar.X_ScalingCoef = 0;
plotpar.Y_ScalingCoef = 0;
plotpar.Y_Label = "Relative Rate (-)";
plotpar.LegendsVec.clear();
	pPs = new Ps(&CH4_PlantOxidationRate, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL);
	Define(pPs, "CH4 PlantOxidationRate", 4.2E-05, UNIT_TYPES::MASS_RATIO_UNIT, "Nitrogen and Carbon >=1|Methane Model>0|PlantType>0", 0., 10., 0., 1.E-3); ps_vec.push_back(pPs);


	
	pFunc = new Func( datatype::TOTPROFILE, elements::METHANE, fysprocess::NOFPROC, bioprocess::METHANE_OXIDATION, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();

	functorprop.FuncName = MyFunc::DIFFUSION_FLUX_FUNCTION; functorprop.Arg = MyArgType::D1;
	functorprop.FunctorInitOption = std::string::npos;
	funcname = "Methane Diffusion Flux Function in water";
	plotpar.X_Label = "Concentration gradient (gC/m4)";
	plotpar.X_Min = 0;
	plotpar.X_Max = 200.;
	plotpar.Num_Of_Curves = 1;
	plotpar.X_ScalingCoef = 0;
	plotpar.Y_ScalingCoef = 3;
	plotpar.Y_Label = "Flux Rate (mg/m2/day)";
	plotpar.LegendsVec.push_back("Note \nSolubility around 2 g/m3 \n200 is the max gradient\n at 0.1 m distance");


	pPs = new Ps(&CH4_Water_Diff_Coef, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
		Define(pPs, "CH4 WaterDiffCoef", 1.3E-9, UNIT_TYPES::AREA_PER_TIME_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0", 1.E-9, 1.E-8);
		ps_vec.push_back(pPs);
	pFunc = new Func( datatype::TOTPROFILE, elements::METHANE, fysprocess::NOFPROC, bioprocess::METHANE_OXIDATION, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();



	ps_vec.push_back((Ps*)m_pModelMap->GetPsPointer("CH4 PlantOxidationRate"));
	functorprop.FuncName = MyFunc::METHANE_PLANTOXIDATION_BY_ROOTRESPIRATION; functorprop.Arg = MyArgType::D1;
		functorprop.FunctorInitOption = std::string::npos;
		funcname = "Plant Oxidation Regulation by root respiration";
		plotpar.X_Label = "Root Respiration Rate (gC/m3/day)";
		plotpar.X_Min = 0;
		plotpar.X_Max = 1.;
		plotpar.Y_ScalingCoef = 3;
		plotpar.Y_Label = "Methane_Oxidation Rate (mg/m3/day)";

		pPs = new Ps(&CH4_PlantOxShape, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL);
		Define(pPs, "CH4 PlantOxShapeCoef", 34., UNIT_TYPES::INVERSE_SQUARED_FLUX_PER_VOLUME, "Nitrogen and Carbon >=1|Methane Model>0|PlantType>0", 0., 200.);  ps_vec.push_back(pPs);


		
	
	pFunc = new Func( datatype::TOTPROFILE, elements::METHANE, fysprocess::NOFPROC, bioprocess::METHANE_OXIDATION, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();




	
	functorprop.FuncName = MyFunc::METHANE_VOLATIVITY; functorprop.Arg = MyArgType::D2;
	functorprop.FunctorInitOption = std::string::npos;
	funcname = "Methane Volatility Function";
	plotpar.X_Label = "Water Concentration ";
	auto m_volume = p_ModelInfo->GetUnitString(UNIT_TYPES::mMASS_LITER_UNIT,true);

	plotpar.X_Label +=m_volume; ;
	plotpar.Y_Label = "Boundary Air Concentration ";
	//plotpar.Y_Label += m_volume;
	plotpar.Y_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::mikro_MASS_LITER_UNIT, true);
	plotpar.X_Min = 0;
	plotpar.X_Max = 20;
	plotpar.Num_Of_Curves = 3;
	plotpar.Num_Of_X = 2;
	plotpar.Num_Of_Opt = 3;
	plotpar.X_ScalingCoef = 0;
	plotpar.Y_ScalingCoef = 3.;
	plotpar.X2_Vec = { 0, 10.,20. };
	plotpar.LegendsVec.clear();
	lstr = p_ModelInfo->GetUnitString(UNIT_TYPES::TEMP_UNIT);
	plotpar.LegendsVec.push_back("0 " + lstr);  plotpar.LegendsVec.push_back("10 " + lstr); plotpar.LegendsVec.push_back("20 " + lstr);

	
		
	pPs= new Ps(&CH4_HStandard, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::SOIL_GAS_EQUILIBRIUM, userlevel::NORMAL);
	    Define(pPs,   "CH4 HenryStdCoef_cp",0.0014,"Mol/atm","Nitrogen and Carbon >=1|Methane Model>0",0.001,0.1);	 ps_vec.push_back(pPs);
	pPs= new Ps(&CH4_HTempSens, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::SOIL_GAS_EQUILIBRIUM, userlevel::NORMAL);
	    Define(pPs,   "CH4 HenryCoefTempSens",-1600.,"K","Nitrogen and Carbon >=1|Methane Model>0",-10000.,10000.);	 ps_vec.push_back(pPs);

	pPs= new Ps(&H_TRef, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::SOIL_GAS_EQUILIBRIUM, userlevel::NORMAL);
	    Define(pPs,   "H_RefTemp",25., UNIT_TYPES::TEMP_UNIT,"Nitrogen and Carbon >=1|Methane Model>0",-50.,50.); 	 ps_vec.push_back(pPs);


    
	pFunc = new Func( datatype::TOTPROFILE, elements::METHANE, fysprocess::NOFPROC, bioprocess::SOIL_GAS_EQUILIBRIUM, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();

	pPs = new Ps(&O2_HStandard, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::SOIL_GAS_EQUILIBRIUM, userlevel::NORMAL);
		Define(pPs, "O2 HenryStdCoef_cp", 0.0013, "Mol/atm", "Nitrogen and Carbon >=1|Methane Model>0|Dissolved oxygen gas>0", 0.001, 0.1); //	 ps_vec.push_back(pPs);
	pPs = new Ps(&O2_HTempSens, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::SOIL_GAS_EQUILIBRIUM, userlevel::NORMAL);
		Define(pPs, "O2 HenryCoefTempSens", -1500., "K", "Nitrogen and Carbon >=1|Methane Model>0|Dissolved oxygen gas>0", -10000., 10000.); //	 ps_vec.push_back(pPs);
		//pPs = new Ps(&O2_HConst, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::SOIL_GAS_EQUILIBRIUM, userlevel::NORMAL);
		//	Define(pPs, "O2 HenryConst", 769.23, "L.atm/mol", "Nitrogen and Carbon >=1|Methane Model>0", 10., 1000.);	 ps_vec.push_back(pPs);

	
	functorprop.FuncName = MyFunc::METHANE_TRANSPIRATION_EFFICIENCY; functorprop.Arg = MyArgType::D1;
	functorprop.FunctorInitOption = std::string::npos;
	funcname = "Transpiration Methane Emission Function";
		plotpar.X_Label = "Water Uptake Rate (mm/day)";
		plotpar.Y_Label = "Methane Emission Efficiency ()";
		plotpar.X_Min = 0;
		plotpar.X_Max = 8;
		plotpar.X_ScalingCoef = 0;
		plotpar.Y_ScalingCoef = 0;

		plotpar.Num_Of_Curves = 1;

		plotpar.Num_Of_X = 1;
		plotpar.Num_Of_Opt =0;
		plotpar.X2_Vec = { };
		plotpar.LegendsVec = {""};



	pPs= new Ps(&CH4_LimitWaterUptake, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL); 
	    Define(pPs,   "CH4 LimitWaterUptake",4.,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon >=1|Methane Model>0|PlantType>0",0.,10.); ps_vec.push_back(pPs);
	pPs= new Ps(&CH4_WaterPowerCoef, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL); 
	    Define(pPs,   "CH4 Water Power Coef",7.,UNIT_TYPES::NO_UNIT,"Nitrogen and Carbon >=1|Methane Model>0|PlantType>0",0,10.); ps_vec.push_back(pPs);
	pPs= new Ps(&CH4_WaterPlantCoef, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL); 
	    Define(pPs,  "CH4 Water Plant Coef",.66, UNIT_TYPES::MASS_RATIO_UNIT,"Nitrogen and Carbon >=1|Methane Model>0|PlantType>0",0.,1.); ps_vec.push_back(pPs);  //Changed default from 66 % to .66	  March 2 2020

	pFunc = new Func( datatype::TOTPROFILE, elements::METHANE, fysprocess::NOFPROC, bioprocess::METHANE_PRODUCTION, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();



	functorprop.FuncName = MyFunc::METHANE_BOUNDARYLAYER_FLUX; functorprop.Arg = MyArgType::D2;
	functorprop.FunctorInitOption = std::string::npos;
		funcname = "Methane Soil Surface Flux";
		plotpar.X_Label = "Air Surface Concentration (";
		unit = p_ModelInfo->GetUnitString(UNIT_TYPES::mMASS_LITER_UNIT); unit += ")"; plotpar.X_Label += unit;
		plotpar.Y_Label = "Methane Flux (";	 unit = p_ModelInfo->GetUnitString(UNIT_TYPES::mMASSFLOW_UNIT); unit += ")"; plotpar.Y_Label += unit;
		plotpar.X_Min = 0.;
		plotpar.X_Max = 30.0;
		plotpar.Num_Of_Curves = 3;
		plotpar.Num_Of_X = 2;
		plotpar.Num_Of_Opt = 3;
		plotpar.X2_Vec = { 0, 10.,20. };
		plotpar.LegendsVec = {};
		plotpar.X_ScalingCoef = 0;
		plotpar.Y_ScalingCoef = 0;
		plotpar.LegendsVec.clear();
		lstr = p_ModelInfo->GetUnitString(UNIT_TYPES::TEMP_UNIT);
		plotpar.LegendsVec.push_back("0 " + lstr);  plotpar.LegendsVec.push_back("10 " + lstr); plotpar.LegendsVec.push_back("20 " + lstr);


	pPs = new Ps(&CH4_CONC_Atm, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
		Define(pPs, "CH4 Air Conc", 1.8, UNIT_TYPES::PPM_CONC_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0", 0., 10.);	ps_vec.push_back(pPs);
	pPs = new Ps(&CH4_ResistanceAir, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
		Define(pPs, "CH4 Resistance Air", 30., "-", "Nitrogen and Carbon >=1|Methane Model>0|Methane SoilBoundary=0", 1., 500.); ps_vec.push_back(pPs);

	
	pFunc = new Func( datatype::TOTPROFILE, elements::METHANE, fysprocess::NOFPROC, bioprocess::METHANE_PRODUCTION, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();

	pPs = new Ps(&CH4_ResistanceAirLogScale, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
		Define(pPs, "CH4 AirResistance LogScale", 0., "-", "Nitrogen and Carbon >=1|Methane Model>0", -2., 4.);  // Changed to allway make use of possible scaling of air resistance

   pPs = new Ps(&O2_CONC_Atm, datatype::SIMPROFILE, elements::OXYGEN, fysprocess::STORAGE, bioprocess::INITB, userlevel::NORMAL);
		Define(pPs, "O2 Air Conc", 20.95, UNIT_TYPES::VOLPERCENTAGE_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0", 0., 100.); 

	pPs = new Ps(&CO2_CONC_Atm, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::INITB, userlevel::NORMAL);
		Define(pPs, "CO2 Air Conc", 30., UNIT_TYPES::PPM_CONC_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Soil CO2Model>0", 2.E-04, 7.E-04);
		 //wenxin
	pPs= new Ps(&CH4_InitConc, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
	    Define(pPs,   "CH4 init Conc",20.,UNIT_TYPES::mMASS_LITER_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0",1.E-02,5.E3);

    //pPs= new Ps(&NPPyr, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
	//    Define(pPs,   "Annual NPP",600.,"gC/m2/yr", "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0",0.,5.E3);
	//pPs= new Ps(&Porosity_Tiller, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
	//    Define(pPs,   "Porosity of tiller",0.3,"-", "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0",0.,1.);
	//pPs= new Ps(&NPP_Ratio, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
	//    Define(pPs,   "NPP Ratio",0.5,"-", "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0",0.,1.);
	pPs= new Ps(&AereScale, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
	    Define(pPs,   "Aerenchyma LogScale",0.,"-", "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0|Methane emission by aerenchyma>0",-5.,5.);
	//pPs= new Ps(&Rob, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
	//    Define(pPs,   "Ratio Root Len. to Depth",3.,"-", "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0",0.,20.);
	pPs= new Ps(&D_CH4_Aere, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
	    Define(pPs,   "Diffusivity CH4 through Aerenchyma",0.1875,"cm2/s", "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0|Methane emission by aerenchyma>0",0.,1.);
	pPs= new Ps(&D_O2_Aere, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
	    Define(pPs,   "Diffusivity O2 through Aerenchyma",0.1759,"cm2/s", "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0|Methane emission by aerenchyma>0",0.,1.);
    //pPs= new Ps(&LAI_Aere, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
	//    Define(pPs,   "Aerenchyma LAI",4.,"cm2/cm2", "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1|Methane Model>0",0.,10.);

	pPs = new Ps(&COS_TranspirationRatio, datatype::ALLPLANTS, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	  Define(pPs, "COS Transpiration Ratio", 15., UNIT_TYPES::MIKRO_MASS_RATIO, "Nitrogen and Carbon >= 1|COS PlantModel >= 1", 1.E-03, 1.E3);
	pPs = new Ps(&COS_TranspirationPotRatio, datatype::ALLPLANTS, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	  Define(pPs, "COS Pot Transp Ratio", 10., UNIT_TYPES::MIKRO_MASS_RATIO, "Nitrogen and Carbon >= 1|COS PlantModel >= 1", 1.E-03, 1.E3);
	pPs = new Ps(&COS_TranspirationPotMaxRatio, datatype::ALLPLANTS, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	  Define(pPs, "COS Pot MaxTransp Ratio", 4., UNIT_TYPES::MIKRO_MASS_RATIO, "Nitrogen and Carbon >= 1|COS PlantModel >= 1", 1.E-03, 1.E3);
  
  pPs = new Ps(&COS_CanopyResistCoef, datatype::ALLPLANTS, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	  Define(pPs, "COS CanopyResistCoef", 8.E4, UNIT_TYPES::MIKRO_MASS_RATIO, "Nitrogen and Carbon >= 1|COS PlantModel >= 1", 1.E-03, 1.E6);
  pPs = new Ps(&COS_AirResistCoef, datatype::ALLPLANTS, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	  Define(pPs, "COS AirResistCoef", 4800., UNIT_TYPES::MIKRO_MASS_RATIO, "Nitrogen and Carbon >= 1|COS PlantModel >= 1", 1.E-03, 1.E3);
  pPs = new Ps(&COS_HalfCondCanopy, datatype::ALLPLANTS, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	  Define(pPs, "COS HalfCondCanopy", 30., UNIT_TYPES::WIND_SPEED_UNIT, "Nitrogen and Carbon >= 1|COS PlantModel >= 1", 1., 1.E3);
  pPs = new Ps(&COS_HalfCondAir, datatype::ALLPLANTS, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	  Define(pPs, "COS HalfCondAir", 10., UNIT_TYPES::WIND_SPEED_UNIT, "Nitrogen and Carbon >= 1|COS PlantModel >= 1", 1., 1.E3);

	pPs = new Ps(&COS_RadHalfCoef, datatype::ALLPLANTS, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	  Define(pPs, "COS RadHalfCoef", 5.E7,UNIT_TYPES::HEATFLOW_UNIT , "Nitrogen and Carbon >= 1|COS PlantModel >= 1", 1.E3, 1.E9);
  pPs = new Ps(&COS_RadScaleCoef, datatype::ALLPLANTS, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	  Define(pPs, "COS RadScaleCoef", 100.,UNIT_TYPES::MIKRO_MASS_FLOW_UNIT, "Nitrogen and Carbon >= 1|COS PlantModel >= 1", 1., 1.E3);

	  
	  pPs = new Ps(&COS_PhotoSynthesisRatio, datatype::ALLPLANTS, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	  Define(pPs, "COS Photosynthesis Ratio", 3., UNIT_TYPES::MIKRO_MASS_RATIO, "Nitrogen and Carbon >= 1|COS PlantModel == 2", 1.E-03, 1.E3);
	pPs = new Ps(&COS_SoilRespiratonRatio, datatype::TOTPROFILE, elements::COS, fysprocess::NOFPROC, bioprocess::COS_TRANSPORT, userlevel::NORMAL);
	  Define(pPs, "COS Soil Respiration Ratio", 1.6, UNIT_TYPES::MIKRO_MASS_RATIO, "Nitrogen and Carbon >= 1|COS Model >= 1", 1.E-03, 1.E3);
;
;	initv.assign(pSoil->NL,0.);
	pG= new G(&CH4_CONC_AIR,pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL); 
	    Define(pG, pNL,   "CH4 air concentration",initv,UNIT_TYPES::mMASS_LITER_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0");
;
    //initv.assign(pSoil->NL, 0.);
    //pG = new G(&CH4_Conc, pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL);
    //    Define(pG, pNL, "CH4 concentration", initv, UNIT_TYPES::PPM_CONC_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0");
//;
	pGs = new Gs(&CH4_TotSoilStorage, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_PRODUCTION, userlevel::NORMAL);
	Define(pGs, "CH4 total soil Storage", 0.0, UNIT_TYPES::MASSFLOW_UNIT, "Methane Model>0");

	pGs = new Gs(&CH4_TotSoilProductionFlux, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_PRODUCTION, userlevel::NORMAL);
	Define(pGs, "CH4 total soil Production", 0.0, UNIT_TYPES::MASSFLOW_UNIT, "Methane Model>0");
	pGs= new Gs(&CH4_TotSoilFlux, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL); 
	    Define(pGs,     "CH4 total soil flux ", 0.0,UNIT_TYPES::MASSFLOW_UNIT, "Methane Model>0")  ;
	pGs= new Gs(&CH4_TotPlantUptake, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL); 
	    Define(pGs,     "CH4 total Plant Uptake", 0.0,UNIT_TYPES::MASSFLOW_UNIT, "Methane Model>0")  ;
	pGs= new Gs(&CH4_TotPlantEmission, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_PRODUCTION, userlevel::NORMAL); 
	    Define(pGs,    "CH4 total Plant Emission", 0.0,UNIT_TYPES::MASSFLOW_UNIT, "Methane Model>0")  ;
	pGs= new Gs(&CH4_TotPlantOxidation, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL); 
	    Define(pGs,     "CH4 total Plant Oxidation", 0.0,UNIT_TYPES::MASSFLOW_UNIT, "Methane Model>0")  ;
	pGs = new Gs(&CH4_TotSoilOxidation, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_OXIDATION, userlevel::NORMAL);
		Define(pGs, "CH4 total Soil Oxidation", 0.0,UNIT_TYPES::MASSFLOW_UNIT, "Methane Model>0");

		
		;
	pGs= new Gs(&N2O_10Percentil, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
	    Define(pGs,     "N2O 10 Percentil",0.0,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon >= 1|Trace Gas Emissions > 0|N2O Yearly Flux Statistics>0");
	pGs= new Gs(&N2O_50Percentil, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
	    Define(pGs,     "N2O 50 Percentil",0.0,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon >= 1|Trace Gas Emissions > 0|N2O Yearly Flux Statistics>0");
	pGs= new Gs(&N2O_90Percentil, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
	    Define(pGs,     "N2O 90 Percentil",0.0,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon >= 1|Trace Gas Emissions > 0|N2O Yearly Flux Statistics>0");
	pGs= new Gs(&N2O_Ymean, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
	    Define(pGs,     "N2O Yearly mean",0.0,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon >= 1|Trace Gas Emissions > 0|N2O Yearly Flux Statistics>0");
    ;
	pGs= new Gs(&NO_10Percentil, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
	    Define(pGs,     "NO 10 Percentil",0.0,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon >= 1|Trace Gas Emissions > 0|N2O Yearly Flux Statistics>0");
	pGs= new Gs(&NO_50Percentil, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
	    Define(pGs,     "NO 50 Percentil",0.0,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon >= 1|Trace Gas Emissions > 0|N2O Yearly Flux Statistics>0");
	pGs= new Gs(&NO_90Percentil, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
	    Define(pGs,     "NO 90 Percentil",0.0,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon >= 1|Trace Gas Emissions > 0|N2O Yearly Flux Statistics>0");
	pGs= new Gs(& NO_Ymean, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
	    Define(pGs,    "NO Yearly mean",0.0,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon >= 1|Trace Gas Emissions > 0|N2O Yearly Flux Statistics>0");
;
 //;
   	pGs= new Gs(&CH4_TotSysFlux, datatype::TOTPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL); 
	Define(pGs,     "Methane ecosystem flux", 0.0,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0|Methane Model>0")  ;
;

	pG= new G(&CH4_Conc_Water,pNL, datatype::SIMPROFILE, elements::METHANE, fysprocess::STORAGE, bioprocess::METHANE_TRANSPORT, userlevel::NORMAL); 
	initv.assign(pSoil->NL,0.);
    Define(pG, pNL ,"CH4 water conc",initv,UNIT_TYPES::mMASS_LITER_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0");

; initv.assign(pSoil->NL, .1);
	pX = new X(&CO2Soil, pNL, datatype::SIMPROFILE, elements::SOILCO2, fysprocess::STORAGE, bioprocess::RESPIRATION, userlevel::NORMAL);
		Define(pX, pNL, "CO2Soil", initv, UNIT_TYPES::GAS_MASS_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0|CO2 SoilModel>0");
		//Wenxin SOILCO2 don't showup
;	initv.assign(pSoil->NL,20.);
	pX= new X(&Oxygen,pNL, datatype::SIMPROFILE, elements::OXYGEN, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pX, pNL  , "Oxygen",initv,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0");
	pX= new X(&NO,pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pX, pNL  , "Nitric Oxide",initv,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 1");
	pX= new X(&N2O,pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pX, pNL  , "Nitrous Oxide",initv,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 1");
	pX= new X(&N2 ,pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pX, pNL  , "Nitrogen gas",initv,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 1");
;	initv.assign(pSoil->NL,0.);
	pT= new T(& NO_SoilFlux ,pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::TRANSPORT, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pT, pNL , "Nitric oxide Soil Flux",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 1");
	pT= new T(& N2O_SoilFlux,pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::TRANSPORT, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pT, pNL , "Nitrous oxide Soil Flux",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 1");
	pT= new T(&N2_SoilFlux ,pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::TRANSPORT, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pT, pNL  , "Nitrogen Soil Flux",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 1");
;
	pT= new T(&NO_Exchange ,pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::TRANSPORT, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pT, pNL  , "Nitric oxide internal exchange",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1");
	pT= new T(&N2O_Exchange ,pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::TRANSPORT, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pT, pNL  , "Nitrous oxide internal exchange",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1");
	pT= new T(&N2_Exchange ,pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::TRANSPORT, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pT, pNL  , "Nitrogen internal exchange",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1");
;
  	pGs= new Gs(&NO_TotExchange , datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pGs, "N Tot NO internal exchange", 0.,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1");
  	pGs= new Gs(&N2O_TotExchange, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pGs,     "N Tot N2O internal exchange", 0.,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1");
  	pGs= new Gs(&N2_TotExchange, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pGs,     "N Tot N2 internal exchange", 0.,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1");
;
	initv.assign(pSoil->NL,0.);
	pT= new T(&NO_NitriFlux, pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::TRANSPORT, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pT, pNL   ,"Nitric oxide (Nitrification)",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1");
	pT= new T(&N2O_NitriFlux ,pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::TRANSPORT, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pT, pNL  , "Nitrous oxide (Nitrification)",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions >= 1");
;
	pT= new T(&CO2_FluxSoil ,pNL, datatype::SIMPROFILE, elements::SOILCO2, fysprocess::TRANSPORT, bioprocess::RESPIRATION, userlevel::NORMAL); 
		Define(pT, pNL  , "CO2_FluxSoil",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0");


	pT= new T(&Oxygen_Flux ,pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::TRANSPORT, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pT, pNL  , "Oxygen Flux",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0");
	pT= new T(&Oxygen_Consumption ,pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::TRANSPORT, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pT, pNL  , "Oxygen Consumption",initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0");
	
	pT = new T(&CO2_Production, pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::TRANSPORT, bioprocess::OXIDATION, userlevel::NORMAL);
		Define(pT, pNL, "CO2 production", initv,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0|CO2 SoilModel>0");
	
	pTs= new Ts(&Oxygen_Flux_Infil , datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pTs, "Oxygen Flux Infil", 0.,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0");

	pTs = new Ts(&CO2_Flux_Out, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL);
		Define(pTs, "CO2 Flux Out", 0.,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0|CO2 SoilModel>0");
;
	pTs= new Ts(&N2O_Rate , datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pTs, "N2O emission rate", 0.,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0");
  	pTs= new Ts(&NO_Rate , datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pTs,  "NO emission rate", 0.,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0");
  	pTs= new Ts(&N2_Rate, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pTs,   "N2 emission rate", 0.,UNIT_TYPES::MASSFLOW_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0");
;
;
// CH4 model above;
; initv.assign(pSoil->NL, .0004);
	pG= new G(&CO2_Conc ,pNL, datatype::SIMPROFILE, elements::SOILCO2, fysprocess::TRANSPORT, bioprocess::RESPIRATION, userlevel::NORMAL); 
		Define(pG, pNL   , "CO2 Concentration",initv, UNIT_TYPES::PPM_CONC_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0");
		//wenxin ppm = volume ratio?
	initv.assign(pSoil->NL, 20.);
	pG= new G(&Oxygen_Conc ,pNL, datatype::SIMPROFILE, elements::AIR, fysprocess::TRANSPORT, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pG, pNL   , "Oxygen Concentration",initv,UNIT_TYPES::VOLPERCENTAGE_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0");
	pG= new G(&Air_Porosity ,pNL, datatype::SIMPROFILE, elements::AIR, fysprocess::TRANSPORT, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pG, pNL   , "Air Porosity",initv,"%", "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0");
		initv.assign(pSoil->NL, .0);
	pG = new G(&DCO2_SoilCoef, pNL, datatype::SIMPROFILE, elements::AIR, fysprocess::TRANSPORT, bioprocess::OXIDATION, userlevel::NORMAL);
		Define(pG, pNL, "CO2 diffusion coefficient", initv, UNIT_TYPES::AREA_PER_TIME_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0");
		
	initv.assign(pSoil->NL,0.);
	pG= new G(&An_VolFraction ,pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::TRANSPORT, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pG, pNL  , "An_VolFraction",initv,"-", "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0");

	functorprop.FuncName = MyFunc::ANAEROBIC_BALLON; functorprop.Arg = MyArgType::D1;
		functorprop.FunctorInitOption = std::string::npos;
		funcname = "Anaerobic Ballon Function";
		plotpar.X_Label = "Oxygen content ";
		unit = p_ModelInfo->GetUnitString(UNIT_TYPES::mMASS_LITER_UNIT,true);
		plotpar.X_Label += unit;
		plotpar.X_Min = 0;
		plotpar.X_Max = 0.20;
		plotpar.Y_Label = "Fraction of Anaerobic Volume ";
		unit = p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT,true);
		plotpar.Y_Label += unit;
		plotpar.Num_Of_X = 1;
		plotpar.Num_Of_Opt = 0;
		plotpar.X2_Vec = { };
		plotpar.LegendsVec = {};
		plotpar.X_ScalingCoef = 0;
		plotpar.Y_ScalingCoef = 0;

	pPs= new Ps(&AnBal_ShapeCoef , datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pPs, "AnBal_ShapeCoef", 100., "-", "Trace Gas Emissions > 0", 0., 500.);  ps_vec.push_back(pPs);
	pFunc = new Func( datatype::TOTPROFILE, elements::METHANE, fysprocess::NOFPROC, bioprocess::METHANE_PRODUCTION, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();

		functorprop.FuncName = MyFunc::DiFFUSION_SOIL_OXYGEN; functorprop.Arg = MyArgType::D1;
		functorprop.FunctorInitOption = std::string::npos;
		funcname = "Oxygen Soil Air Diffusion Function";
		plotpar.Y_Label = "Soil Diffusion ";
		unit = p_ModelInfo->GetUnitString(UNIT_TYPES::AREA_PER_TIME_UNIT,true);
		plotpar.Y_Label += unit;
		plotpar.X_Min = 0;
		plotpar.X_Max = 100;
		plotpar.X_Label = "Percentage of total Volume ";
		unit = p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT,true);
		plotpar.X_Label += unit;
		plotpar.Num_Of_X = 1;
		plotpar.Num_Of_Opt = 0;
		plotpar.X2_Vec = { };
		plotpar.LegendsVec = {};
		plotpar.X_ScalingCoef = 0;
		plotpar.Y_ScalingCoef = 0;


		pPs = new Ps(&DO2_Tortuosity, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); ps_vec.push_back(pPs);
		Define(pPs,   "D O2 Tortuosity", 0.66, "-","Trace Gas Emissions > 0",0.3,1.0);

		pFunc = new Func( datatype::TOTPROFILE, elements::OXYGEN, fysprocess::DIFFUSION, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();
		functorprop.Arg = MyArgType::D2;
		pFunc = new Func( datatype::TOTPROFILE, elements::OXYGEN, fysprocess::DIFFUSION, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();

		functorprop.FuncName = MyFunc::DiFFUSION_SOIL_CO2; functorprop.Arg = MyArgType::D1;
		functorprop.FunctorInitOption = std::string::npos;
		funcname = "CO2 Soil Air Diffusion Function";
		plotpar.Y_Label = "Soil Diffusion ";
		unit = p_ModelInfo->GetUnitString(UNIT_TYPES::AREA_PER_TIME_UNIT,true);
		plotpar.Y_Label += unit;
		plotpar.X_Min = 0;
		plotpar.X_Max = 100.;
		plotpar.X_Label = "Fraction of total Volume ";
		unit = p_ModelInfo->GetUnitString(UNIT_TYPES::VOLPERCENTAGE_UNIT,true);
		plotpar.X_Label += unit;
		plotpar.Y_ScalingCoef = 0;

	pPs= new Ps(&DCO2_Tortuosity, datatype::SIMPROFILE, elements::SOILCO2, fysprocess::TRANSPORT, bioprocess::RESPIRATION, userlevel::NORMAL); 
		Define(pPs,   "D CO2 Tortuosity", 0.66, "-","Trace Gas Emissions > 0",0.3,1.0);	ps_vec.push_back(pPs);
	pPs = new Ps(&DCO2_PowerCoefSoilDiff, datatype::SIMPROFILE, elements::SOILCO2, fysprocess::TRANSPORT, bioprocess::RESPIRATION, userlevel::NORMAL);
		Define(pPs, "D CO2 PowerCoef", 1., "-", "Trace Gas Emissions > 0", 0.01, 100.0); ps_vec.push_back(pPs);
	pPs = new Ps(&DCO2_AirThreshold, datatype::SIMPROFILE, elements::SOILCO2, fysprocess::TRANSPORT, bioprocess::RESPIRATION, userlevel::NORMAL);
		Define(pPs, "D CO2 AirThreshold", 1., "vol %", "Trace Gas Emissions > 0", 0.01, 100.0); ps_vec.push_back(pPs);
	pPs = new Ps(&DCO2Min_Soil, datatype::SIMPROFILE, elements::SOILCO2, fysprocess::TRANSPORT, bioprocess::RESPIRATION, userlevel::NORMAL);
		Define(pPs, "D CO2 Min Value Soil", 1.E-9, UNIT_TYPES::AREA_PER_TIME_UNIT, "Trace Gas Emissions > 0", 1.E-10, 1.E-6); ps_vec.push_back(pPs);

	pFunc = new Func( datatype::TOTPROFILE, elements::OXYGEN, fysprocess::DIFFUSION, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();
		functorprop.Arg = MyArgType::D2;
	pFunc = new Func( datatype::TOTPROFILE, elements::OXYGEN, fysprocess::DIFFUSION, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();




	initv.assign(pSoil->NL,20.);

	functorprop.FuncName = MyFunc::NITRI_N2O_PROD_RESPONSE; functorprop.Arg = MyArgType::D1;
	functorprop.FunctorInitOption = std::string::npos;
	funcname = "Nitrous Oxide Production from Nitrifiers";
	plotpar.Y_Label = "Nitrous Oxide Production ";
	unit = p_ModelInfo->GetUnitString(UNIT_TYPES::MIKRO_MASS_FLOW_UNIT,true);
	plotpar.Y_Label += unit;
	plotpar.X_Min = 0;
	plotpar.X_Max = 1E-3;
	plotpar.X_Label = "Nitrification rate";
	unit = p_ModelInfo->GetUnitString(UNIT_TYPES::mMASSFLOW_UNIT,true);
	plotpar.X_Label += unit;
	plotpar.Y_ScalingCoef = 6;
	plotpar.X_ScalingCoef = 3;

;
	pPs= new Ps(&MaxFracN2O, datatype::SIMPROFILE, elements::SOILGAS_N2O, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
	Define(pPs, "MaxFracN2O", 0.0006, "-", "Trace Gas Emissions > 0", 0., 0.1); ps_vec.push_back(pPs);
	pFunc = new Func( datatype::TOTPROFILE, elements::OXYGEN, fysprocess::DIFFUSION, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();

	functorprop.FuncName = MyFunc::NITRI_NO_PROD_RESPONSE;
	funcname = "Nitric Oxide Production from Nitrifiers";
	plotpar.Y_Label = "Nitric Oxide Production ";
	unit = p_ModelInfo->GetUnitString(UNIT_TYPES::MIKRO_MASS_FLOW_UNIT,true);
	plotpar.Y_Label += unit;
	
	pPs= new Ps(&MaxFracNO, datatype::SIMPROFILE, elements::SOILGAS_NO, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pPs,   "MaxFracNO", 0.004, "-","Trace Gas Emissions > 0",0., 0.5); ps_vec.push_back(pPs);
	pFunc = new Func( datatype::TOTPROFILE, elements::OXYGEN, fysprocess::DIFFUSION, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();
;


functorprop.FuncName = MyFunc::NITRI_NO_PROD_TEMP_RESPONSE; functorprop.Arg = MyArgType::D1;
functorprop.FunctorInitOption = std::string::npos;
funcname = "N oxide Production From Nitrification Temp Response";
plotpar.Y_Label = "Relative Response ";
unit = p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT,true);
plotpar.Y_Label += unit;
plotpar.X_Min = -5.;
plotpar.X_Max = 35.;
plotpar.X_Label = "Temperature ";
unit = p_ModelInfo->GetUnitString(UNIT_TYPES::TEMP_UNIT,true);
plotpar.X_Label += unit;
plotpar.Y_ScalingCoef = 0;
plotpar.X_ScalingCoef = 0;


	pPs= new Ps(&N2O_TempMax, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
	Define(pPs, "N2O TempMax", 33.5, UNIT_TYPES::TEMP_UNIT, "Trace Gas Emissions > 0", 10., 50.); ps_vec.push_back(pPs);
	pPs= new Ps(&N2O_TempOpt, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pPs,   "N2O TempOpt", 23.5,UNIT_TYPES::TEMP_UNIT,"Trace Gas Emissions > 0",10., 50.); ps_vec.push_back(pPs);
	pPs= new Ps(&N2O_TempShape, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pPs,   "N2O TempShape", 1.5, "-","Trace Gas Emissions > 0",0.001, 5.); ps_vec.push_back(pPs);

	pFunc = new Func( datatype::TOTPROFILE, elements::OXYGEN, fysprocess::DIFFUSION, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();

;
functorprop.FuncName = MyFunc::NITRI_N2O_PROD_MOIST_RESPONSE; functorprop.Arg = MyArgType::D1;
functorprop.FunctorInitOption = std::string::npos;
funcname = "N20 Production From Nitrification Moisture Response";
plotpar.Y_Label = "Relative Response ";
unit = p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT,true);
plotpar.Y_Label += unit;
plotpar.X_Min = 0;
plotpar.X_Max = 1;
plotpar.X_Label = "Relative Saturation ";
unit = p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT,true);
plotpar.X_Label += unit;
plotpar.Y_ScalingCoef = 0;
	pPs= new Ps(&N2O_RelSatCrit, datatype::SIMPROFILE, elements::SOILGAS_N2O, fysprocess::STORAGE, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL); 
		Define(pPs,  "N2O RelSatCrit", 0.55, "-","Trace Gas Emissions > 0",0.1, 1.); ps_vec.push_back(pPs);
	pPs= new Ps(&N2O_RelSatForm, datatype::SIMPROFILE, elements::SOILGAS_N2O, fysprocess::STORAGE, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL); 
		Define(pPs,   "N2O RelSatForm", 0.24, "-","Trace Gas Emissions > 0",0.001, 1.);	ps_vec.push_back(pPs);
	
	pFunc = new Func( datatype::TOTPROFILE, elements::OXYGEN, fysprocess::DIFFUSION, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();
;
functorprop.FuncName = MyFunc::NITRI_NO_PROD_MOIST_RESPONSE; functorprop.Arg = MyArgType::D1;
funcname = "N0 Production From Nitrification Moisture Response";
	pPs= new Ps(&NO_RelSatCrit, datatype::SIMPROFILE, elements::SOILGAS_NO, fysprocess::STORAGE, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pPs,  "NO RelSatCrit", 0.45, "-","Trace Gas Emissions > 0",0.1, 1.);	ps_vec.push_back(pPs);
	pPs= new Ps(&NO_RelSatForm , datatype::SIMPROFILE, elements::SOILGAS_NO, fysprocess::STORAGE, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pPs,  "NO RelSatForm", 0.024, "-","Trace Gas Emissions > 0",0.001, 1.); ps_vec.push_back(pPs);
	pFunc = new Func( datatype::TOTPROFILE, elements::SOILGAS_NO, fysprocess::DIFFUSION, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();

		functorprop.FuncName = MyFunc::NITRI_NO_PROD_PH_RESPONSE; functorprop.Arg = MyArgType::D1;
		functorprop.FunctorInitOption = std::string::npos;
		funcname = "N0 Production From Nitrification PH Response";
		plotpar.Y_Label = "Relative Response ";
		unit = p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT,true);
		plotpar.Y_Label += unit;
		plotpar.X_Min = 3;
		plotpar.X_Max = 8;
		plotpar.X_Label = "Soil PH ";
		unit = p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT,true);
		plotpar.X_Label += unit;
		plotpar.Y_ScalingCoef = 0;



	pPs = new Ps(&NO_pH_Coef, datatype::SIMPROFILE, elements::SOILGAS_NO, fysprocess::STORAGE, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);	ps_vec.push_back(pPs);
		Define(pPs, "NO pHCoef", 4.7, "-", "Trace Gas Emissions > 0", 2., 10.);

	pFunc = new Func( datatype::TOTPROFILE, elements::SOILGAS_NO, fysprocess::DIFFUSION, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();

		functorprop.FuncName = MyFunc::DENIT_ANAEROBIC_FRAC_RESPONSE; functorprop.Arg = MyArgType::D1;
		functorprop.FunctorInitOption = std::string::npos;
		funcname = "Denitrifiers Response to Anaerobic fraction";
		plotpar.Y_Label = "Relative Response ";
		unit = p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT,true);
		plotpar.Y_Label += unit;
		plotpar.X_Min = 0;
		plotpar.X_Max = 1;
		plotpar.X_Label = "Aerobic Volume Frac ";
		unit = p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT,true);
		plotpar.X_Label += unit;
		plotpar.Y_ScalingCoef = 0;
	pPs= new Ps(&Denit_Act_PowerCoef, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
	Define(pPs, "Denit Act PowerCoef", 1., "-", "Trace Gas Emissions > 0", 1., 5.); ps_vec.push_back(pPs);
	
	pFunc = new Func( datatype::TOTPROFILE, elements::ANAROBIC_GAS, fysprocess::DIFFUSION, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();

;
functorprop.FuncName = MyFunc::EXCHANGE_EFFICIENY_ANAEROBIC; functorprop.Arg = MyArgType::D1;
functorprop.FunctorInitOption = std::string::npos;
funcname = "Diffusion Exchange efficiency from Anaerobic to Aerobic";
plotpar.Y_Label = "Relative Response ";
unit = p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT,true);
plotpar.Y_Label += unit;
plotpar.X_Min = 0;
plotpar.X_Max = 1.;
plotpar.X_Label = "Relative anaerobic fraction ";
unit = p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT,true);
plotpar.X_Label += unit;
plotpar.Y_ScalingCoef = 0;
	pPs= new Ps(&IntDiffRedFrac, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pPs,   "IntDiffRedFrac", 0.01, "-","Trace Gas Emissions > 0",0.1E-8, 1.);  ps_vec.push_back(pPs);
	pPs= new Ps(&IntDiffRedFracBase, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pPs,   "IntDiffRedFracBase", 0.0001, "-","Trace Gas Emissions > 0",0.0, 1.);	ps_vec.push_back(pPs);
	pFunc = new Func( datatype::TOTPROFILE, elements::ANAROBIC_GAS, fysprocess::DIFFUSION, bioprocess::SOIL_GAS_DYNAMICS, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname, ps_vec, plotpar);	ps_vec.clear();
;
	Tab *pTab;

		pP=m_pModelMap->GetPPointer("mUpper Depth");
		mTab_Ox.push_back(pP);mTab_Ph.push_back(pP);
		pP=m_pModelMap->GetPPointer("mLower Depth");
		mTab_Ox.push_back(pP);mTab_Ph.push_back(pP);
	initv.assign(pSoil->NL, 20.9);
	pP= new P(&Init_Oxygen_Conc,pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::TRANSPORT, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pP, pNL,"Init Oxygen Conc", initv,UNIT_TYPES::VOLPERCENTAGE_UNIT,"Nitrogen and Carbon >= 1|Trace Gas Emissions > 0",0.,30.);
		mTab_Ox.push_back(pP);
;	initv.assign(pSoil->NL, 5000);
	pP = new P(&Init_CO2_Conc, pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::TRANSPORT, bioprocess::OXIDATION, userlevel::NORMAL);
		Define(pP, pNL, "Init CO2 Conc", initv, UNIT_TYPES::PPM_CONC_UNIT, "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0", 0., 600.);
		mTab_Ox.push_back(pP); //wenxin

	pTab= new Tab(&mTab_Ox, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::INITF, bioprocess::NOBPROC, userlevel::NORMAL); 
		Define(pTab, "Initial Gas Concentrations",3);
		initv.assign(pSoil->NL,5.);

	pP= new P(&pH_Value, pNL, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::STORAGE, bioprocess::OXIDATION, userlevel::NORMAL); 
		Define(pP, pNL,"pH value",initv,"-","Nitrogen and Carbon > 0|Trace Gas Emissions > 0",1.,10.);
		mTab_Ph.push_back(pP);

	pTab= new Tab(&mTab_Ph, datatype::SIMPROFILE, elements::ANAROBIC_GAS, fysprocess::INITF, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pTab,"pH Soil Profile",3);

	initv.assign(p_Plant->NumPlants, 600.);
	pP = new P(&NPPyr, pNumPlants, datatype::PLANT, elements::PLANT_SHAPE, fysprocess::INTERCEPTION, bioprocess::NOBPROC, userlevel::NORMAL);
	mTab_Aere.push_back(pP);
	Define(pP, pNumPlants, "Annual NPP", initv, "gC/m2/yr", "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0", 0., 5.E3);
	
	initv.assign(p_Plant->NumPlants, 0.3);
	pP = new P(&Porosity_Tiller, pNumPlants, datatype::PLANT, elements::PLANT_SHAPE, fysprocess::INTERCEPTION, bioprocess::NOBPROC, userlevel::NORMAL);
	mTab_Aere.push_back(pP);
	Define(pP, pNumPlants, "Porosity of Tiller", initv, "-", "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0", 0., 1.);	 
	
	initv.assign(p_Plant->NumPlants, 0.5);
	pP = new P(&NPP_Ratio, pNumPlants, datatype::PLANT, elements::PLANT_SHAPE, fysprocess::INTERCEPTION, bioprocess::NOBPROC, userlevel::NORMAL);
	mTab_Aere.push_back(pP);
    Define(pP, pNumPlants, "NPP Ratio", initv, "-", "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0", 0., 1.); 

	initv.assign(p_Plant->NumPlants, 3.);
	pP = new P(&Rob, pNumPlants, datatype::PLANT, elements::PLANT_SHAPE, fysprocess::INTERCEPTION, bioprocess::NOBPROC, userlevel::NORMAL);    
	mTab_Aere.push_back(pP);
	Define(pP, pNumPlants, "Ratio Root len. to Depth", initv, "-", "Nitrogen and Carbon >= 1|Trace Gas Emissions > 0", 0., 20.);
		
	//Tab* pTab;
	pTab = new Tab(&mTab_Aere, datatype::PLANT, elements::PLANT_SHAPE, fysprocess::INTERCEPTION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pTab, "Aerenchyma Transport", 1);


return FunctionDef();
}
bool NC_GasFlux_Functions::FunctionDef() {


	return true;
}

 /**
double NC_GasFlux_Functions::N2O_TempFunctionL(size_t Layer) {
	
	return N2O_TempFunction(p_NC_Plant->NDrivTemperature[Layer]);
}

double NC_GasFlux_Functions::N2O_TempFunction(double Value) {

	return pow((N2O_TempMax-Value)/(N2O_TempMax-N2O_TempOpt),N2O_TempShape)*
					  exp(N2O_TempShape*(Value-N2O_TempOpt)/(N2O_TempMax-N2O_TempOpt));
}

double NC_GasFlux_Functions::N2O_MoistureFunctionL(size_t Layer) {

	double RelSat;
	RelSat=p_NC_Plant->NDrivTheta[Layer]/p_Soil_WaterF->M_Saturation[Layer];
	return N2O_MoistureFunction(RelSat);
}

double NC_GasFlux_Functions::N2O_MoistureFunction(double Value) {
	return 1-1/(1+exp((Value-N2O_RelSatCrit)/N2O_RelSatForm));
}


double NC_GasFlux_Functions::NO_MoistureFunctionL(size_t Layer) {

	double RelSat;
	RelSat=p_NC_Plant->NDrivTheta[Layer]/p_Soil_WaterF->M_Saturation[Layer];

	return NO_MoistureFunction(RelSat);
}

double NC_GasFlux_Functions::NO_MoistureFunction(double Value) {
	
	return 1-1/(1+exp((Value-NO_RelSatCrit)/NO_RelSatForm));
}


double NC_GasFlux_Functions::NO_pHFunctionL(size_t Layer) {
	

	return NO_pHFunction(pH_Value[Layer]);
}

double NC_GasFlux_Functions::NO_pHFunction(double Value) {
	
	return min(1.,max(0.05, NO_pH_Coef-Value));
}


double NC_GasFlux_Functions::ExCoefFunctionL(size_t Layer) {
	return An_VolFraction[Layer]*(1-An_VolFraction[Layer])*DO2_SoilCoef[Layer]*IntDiffRedFrac+IntDiffRedFracBase*DO2_SoilCoef[Layer];
}


double NC_GasFlux_Functions::ExCoefFunction(double AnVolFraction, double AirPorosity) {
	//Use GenFunctions, only	:D_Oxygen

	//Real AnVolFraction, AirPorosity

	return	D_oxygen(DO2_Tortuosity, AirPorosity, 20.)*AnVolFraction*(1.-AnVolFraction)*IntDiffRedFrac*86.4E6 
	+IntDiffRedFracBase*D_oxygen(DO2_Tortuosity, AirPorosity, 20.)*86.4E6;
}		 */
bool NC_GasFlux_Functions::DenitrifierUpdate() {


	
    N_Tot_DenitGrowth_NO3=N_Tot_DenitGrowth_NO2=N_Tot_DenitGrowth_NO=N_Tot_DenitGrowth_N2O=0;
    
	for(size_t i=0; i<An_Cons_NO3.size(); i++) {
 
		if(An_Cons_NO3[i]>0) {
		        RatioNO3[i]=min(1., max(0.,  An_NO3[i]/(An_Cons_NO3[i]*T_Step) ));
			    An_Cons_NO3[i] = RatioNO3[i]*An_Cons_NO3[i];
		}
        else {
                RatioNO3[i]=1.;
            	An_Cons_NO3[i] = 0.			    		    ;
		}    			
			
		if(An_Cons_NO2[i]>0.) {
		        RatioNO2[i]=min(1., max(0.,  An_NO2[i]/(An_Cons_NO2[i]*T_Step) ));
    		    An_Cons_NO2[i] = RatioNO2[i]*An_Cons_NO2[i]	;
		}
        else {
                RatioNO2[i]=1.;
            	An_Cons_NO2[i] = 0.	        ;	    				
		}    	
			
		if(NO_Exchange[i]>0.) {	
				NO_Exchange[i]=RatioNO[i]*NO_Exchange[i];
			    An_Cons_NO[i]=RatioNO[i]*An_Cons_NO[i]			;
		}
		else {
				NO_Exchange[i]=0.0;
			    An_Cons_NO[i]=0.0;
	    }		    

		if(N2O_Exchange[i]>0.) {					
			    N2O_Exchange[i]=RatioN2O[i]*N2O_Exchange[i];
			    An_Cons_N2O[i]=RatioN2O[i]*An_Cons_N2O[i];
		}
        else {
			    N2O_Exchange[i]=0.;
			    An_Cons_N2O[i]=0.;
	    }	
        
        if(Denitrifier_Biomass[i]>=0.&&Denitrifier_Biomass[i]<10000.) ;
        
        
        else
            Denitrifier_Biomass[i]=0.00;
        
        if(RatioNO3[i]>=0.&&RatioNO2[i]>=0.&&RatioNO[i]>=0&&RatioN2O[i]>=0.) {
       
            DenitGrowth_NO3[i]    =  DMic_Growth_NO3[i]*RatioNO3[i]*Denitrifier_Activity[i]*Denitrifier_Biomass[i];
            DenitGrowth_NO2[i]   =   DMic_Growth_NO2[i]*RatioNO2[i]*Denitrifier_Activity[i]*Denitrifier_Biomass[i];
            DenitGrowth_NO[i]    =   DMic_Growth_NO[i]*RatioNO[i]*Denitrifier_Activity[i]*Denitrifier_Biomass[i];
            DenitGrowth_N2O[i]   =   DMic_Growth_N2O[i]*RatioN2O[i]*DenitriInhibitNO3L(i)*Denitrifier_Activity[i]*Denitrifier_Biomass[i];
		}
        else 
            DenitGrowth_NO3[i]=0.;

		Denitrifier_Growth[i]   =DenitGrowth_NO3[i] +DenitGrowth_NO2[i]+DenitGrowth_NO[i]+DenitGrowth_N2O[i];
	    Denitrifier_Death[i]    =DMic_DeathRateCoef*max(Denitrifier_Biomass[i]-DMic_DormancyThreshold/p_Soil_WaterF->AThicknessLayers[i],0.)+
	                                DMic_DeathCDormancy*Denitrifier_Biomass[i];
	                                
	    
	    
	    if(Denitrifier_Growth[i]>0.) {
	    
	        if(DenitrifierGrowth==0) {
	            double ThresholdUptake=NUptMaxAvailFrac*SoilAmmonium[i]/T_Step;
	            DenitUptakeFactor[i]=max(0., min(1.,ThresholdUptake/Denitrifier_Growth[i]));
	            Denitrifier_Growth[i]   =Denitrifier_Growth[i]*DenitUptakeFactor[i];
			}
	        else if(DenitrifierGrowth==1) {
	            double ThresholdUptake=NUptMaxAvailFrac*O_State[_DIS][_N_][i]/T_Step;
	            DenitUptakeFactor[i]=max(0., min(1.,ThresholdUptake/Denitrifier_Growth[i]));
	            Denitrifier_Growth[i]   =Denitrifier_Growth[i]*DenitUptakeFactor[i];
			}
	        else {
                double ThresholdUptakeMin=NUptMaxAvailFrac*SoilAmmonium[i]/T_Step;
                double ThresholdUptakeOrg=NUptMaxAvailFrac*O_State[_DIS][_N_][i]/T_Step            ;
                double ThresholdUptake=NUptMaxAvailFrac*(O_State[_DIS][_N_][i]+SoilAmmonium[i])/T_Step     ;
                DenitUptakeFactor[i]=max(0., min(1.,ThresholdUptake/Denitrifier_Growth[i]));
                Denitrifier_Growth[i]=DenitUptakeFactor[i]*Denitrifier_Growth[i]       ;
	            DenitUptakeMin[i]=ThresholdUptakeMin/(ThresholdUptakeMin+ThresholdUptakeOrg)*Denitrifier_Growth[i];
	            DenitUptakeOrg[i]=Denitrifier_Growth[i]-DenitUptakeMin[i]	           	         ;
	        }  
		}
        else {
            DenitUptakeMin[i]=0.;
            DenitUptakeOrg[i]=0.;
            DenitUptakeFactor[i]=0.;
	    }
	    
	    
	    if(DenitUptakeFactor[i]<1.) {
	        DenitGrowth_NO3[i]    =  DenitUptakeFactor[i]*DenitGrowth_NO3[i];
            DenitGrowth_NO2[i]   =  DenitUptakeFactor[i]*DenitGrowth_NO2[i];
            DenitGrowth_NO[i]    =  DenitUptakeFactor[i]*DenitGrowth_NO[i];
            DenitGrowth_N2O[i]   =  DenitUptakeFactor[i]*DenitGrowth_N2O[i];
           	An_Cons_NO3[i]		 = DenitUptakeFactor[i]*An_Cons_NO3[i];
		    An_Cons_NO2[i]		 = DenitUptakeFactor[i]*An_Cons_NO2[i];
 		    An_Cons_NO[i]		 = DenitUptakeFactor[i]*An_Cons_NO[i];
            An_Cons_N2O[i]	     = DenitUptakeFactor[i]*An_Cons_N2O[i];
         }
 		 An_Prod_NO2[i]	=An_Cons_NO3[i];
 		 An_Prod_NO[i]	=An_Cons_NO2[i]	;
	     An_Prod_N2O[i]  =An_Cons_NO[i];
	     An_Prod_N2[i]   =An_Cons_N2O[i];
		 if(T_Step==0.) 	T_Step = *pT_Step;
	
	    Denitrifier_ActivityRate[i]=DeniActivityRateCoef*(DMic_TempFunctionL(i)*DMic_pH_ResponseL(i)*DMic_N_ResponseL(i)*pow(An_VolFraction[i],Denit_Act_PowerCoef)-Denitrifier_Activity[i])/T_Step;
	
	}
	if (An_Cons_NO3[0] == 0.) {
		An_Cons_NO3[0] = 1.E-7;
	}
	
	N_Tot_DenitGrowth_NO3   =  sum(DenitGrowth_NO3);
    N_Tot_DenitGrowth_NO2   =  sum(DenitGrowth_NO2);
    N_Tot_DenitGrowth_NO    =  sum(DenitGrowth_NO);
    N_Tot_DenitGrowth_N2O   =  sum(DenitGrowth_N2O);
	return true;
	   
}


