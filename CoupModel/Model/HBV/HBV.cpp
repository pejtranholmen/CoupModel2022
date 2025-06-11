#include "HBV.h"
#include "Functor/HBV_Functor.h"
#include "../Atmosphere/AtmModel.h"
#include "../PLANT/PlantModel.h"

void HBV::setHBV(ModelInfo* pInfo, ModelMap* pMap, Structure* pStruc)
{
	ModuleName = "HBV";
	ModuleNo = 34;
	ModuleConditions = "";
	m_pModelInfo = pInfo; m_pModelMap = pMap; m_pModelStructure = pStruc;
	pT_Step = &m_pModelInfo->T_Step;
	T_Step = *pT_Step;
}

HBV::HBV(void)
{

	ModuleName="HBV";
	ModuleNo=34;
	ModuleConditions="HBV Soil Module >0";
}
bool HBV::Def()
{

	Ps* pPs;
	Func *pFunc;
	string funcname;
	FunctorProp functorprop;
	vector<Ps*> ps_vec;
	vector<P*> p_vec;
	ps_vec.clear();
	ModuleNo = 34;
	PLOT_PARAM plotpar;
	plotpar.Num_Of_Curves = 1;
	plotpar.Num_Of_Points = 300;



	ModuleConditions = "HBV Soil Module >0";
	AtmModel* pAtm = (AtmModel*)m_pModelInfo->GetModelPtr("Atm Model");
	m_pMet = &pAtm->m_Irrigation;
	PlantModel* pPlant = (PlantModel*)m_pModelInfo->GetModelPtr("Plant Model");
	m_pPlant = &pPlant->m_WaterUptake;

	pPs = new Ps(&Initial_SoilStorage, datatype::SINGLE, elements::WATER, fysprocess::INITF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs, "Initial Soil Storage", 30., "mm", "", 0., 300.);
	pPs = new Ps(&Initial_PeakStorage, datatype::SINGLE, elements::WATER, fysprocess::INITF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs, "Initial Peak Storage", 30., "mm", "", 0., 300.);
	pPs = new Ps(&Initial_BaseStorage, datatype::SINGLE, elements::WATER, fysprocess::INITF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs, "Initial Base Storage", 30., "mm", "", 0., 300.);


	pPs = new Ps(&FieldCapacity, datatype::SINGLE, elements::WATER, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
	ps_vec.push_back(pPs);
	Define(pPs, "FieldCapacity", 50., "mm", "", 10., 400.);
	pPs = new Ps(&CriticalUptakeFrac, datatype::SINGLE, elements::WATER, fysprocess::WATERUPTAKE, bioprocess::NOBPROC, userlevel::NORMAL);
	ps_vec.push_back(pPs);
	Define(pPs, "Critical Uptake Frac", .45, "mm", "", 0.1, 1.0);

	functorprop.FuncName = MyFunc::HBV_ACTUALEVAPORATION; functorprop.Arg = MyArgType::D2;
	functorprop.FunctorInitOption = std::string::npos;
	funcname = "Actual Evaporation Function";
	plotpar.Num_Of_Curves = 1;
	plotpar.Num_Of_Points = 300;
	plotpar.Num_Of_X = 2;
	plotpar.X_Min = 0;
	plotpar.X_Max = 100;
	plotpar.X_Label = "Soil Water Storage (mm)";
	plotpar.Y_Label = "Evaporation Rate (mm/day)";
	plotpar.LogXScale = false;
	plotpar.LogYScale = false;
	plotpar.Num_Of_Opt = 1;
	plotpar.X2_Vec = { 6. };
	plotpar.LegendsVec = { "6 mm/day" };



	pFunc = new Func( datatype::SINGLEPLANT, elements::ATMOSPHERE, fysprocess::POTTRANSPIRATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname, ps_vec ,plotpar);
	ps_vec.erase(ps_vec.begin() + 1);



	pPs = new Ps(&BetaCoef, datatype::SINGLE, elements::WATER, fysprocess::INFILTRATION, bioprocess::NOBPROC, userlevel::NORMAL);	ps_vec.push_back(pPs);
	Define(pPs, "BetaCoef", 1., UNIT_TYPES::NO_UNIT, "", 1., 6.);

	functorprop.FuncName = MyFunc::HBV_INFILTRATION; functorprop.Arg = MyArgType::D3;
	funcname = "Soil Infiltration Function";

	plotpar.Num_Of_Curves = 1;
	plotpar.Num_Of_Points = 300;
	plotpar.Num_Of_X = 3;
	plotpar.X_Min = 0;
	plotpar.X_Max = 100;
	plotpar.X_Label = "Soil Water Storage (mm)";
	plotpar.Y_Label = "Relative Infiltration Degree (-)";
	plotpar.Num_Of_Opt = 1;
	plotpar.X2_Vec = { 1. };
	plotpar.X3_Vec = { 0. };
	plotpar.LegendsVec = { "" };

	pFunc = new Func( datatype::SINGLEPLANT, elements::ATMOSPHERE, fysprocess::POTTRANSPIRATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname, ps_vec, plotpar); ps_vec.clear();


	pPs = new Ps(&DisCoefk1, datatype::SINGLE, elements::WATER, fysprocess::RUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);	ps_vec.push_back(pPs);
	Define(pPs, "Discharge K1", .16, "1/day", "", 0.1, .3);
	pPs = new Ps(&DisCoefAlfa, datatype::SINGLE, elements::WATER, fysprocess::RUNOFF, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs, "Discharge Alfa", 0., "mm", "", 0., 1.);
	pPs = new Ps(&T_Step, datatype::SINGLE, elements::WATER, fysprocess::RUNOFF, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs, "HBV TStep", 1., "mm", "", 0.001, 1.);

	functorprop.FuncName = MyFunc::HBV_PEAK_RUNOFF; functorprop.Arg = MyArgType::D1;
	funcname = "Peak Discharge Function";

	plotpar.Num_Of_X = 1;
	plotpar.X_Min = 0;
	plotpar.X_Max = 100;
	plotpar.Y_Label = "Flow Rate (mm/day)";
	plotpar.X_Label = "Peak Storage (mm)";
	plotpar.LogXScale = false;
	plotpar.LogYScale = false;
	plotpar.Num_Of_Opt = 0.;
	plotpar.LegendsVec = { "" };

	pFunc = new Func( datatype::SINGLE, elements::WATER, fysprocess::DRAINAGE, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname, ps_vec, plotpar);
	ps_vec.clear();

	pPs = new Ps(&DisCoefk2, datatype::SINGLE, elements::WATER, fysprocess::RUNOFF, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs, "Discharge K2", 0.016, "mm", "", 0.001, 0.1);

	functorprop.FuncName = MyFunc::HBV_BASE_RUNOFF; functorprop.Arg = MyArgType::D1;
	funcname = "Base Discharge Function";
	plotpar.X_Label = "Base Storage (mm)";
	pFunc = new Func( datatype::SINGLE, elements::WATER, fysprocess::DRAINAGE, bioprocess::NOBPROC, userlevel::NORMAL);



	Define(pFunc, functorprop, funcname, ps_vec, plotpar); ps_vec.clear();

	ps_vec.clear();
	pPs = new Ps(&MaxPerc, datatype::SINGLE, elements::WATER, fysprocess::PERCOLATION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs, "PMaxPerc", 0.5,UNIT_TYPES::WATERFLOW_UNIT, "", 0.1, 0.6);
	functorprop.FuncName = MyFunc::HBV_GROUNDWATER_REDIST; functorprop.Arg = MyArgType::D1;
	functorprop.FunctorInitOption = std::string::npos;
	funcname = "HBV GroundWater Redistribution Function";
	plotpar.Num_Of_Curves = 1;
	plotpar.Num_Of_Points = 300;
	plotpar.Num_Of_X = 1;
	plotpar.X_Min = 0;
	plotpar.X_Max = 100;
	plotpar.X_Label = "Flow to GroundWater (mm/day)";
	plotpar.Y_Label = "Flow to LowerBase GroundWater (mm/day)";
	plotpar.LogXScale = false;
	plotpar.LogYScale = false;
	plotpar.Num_Of_Opt = 1;


	pFunc = new Func( datatype::SINGLE, elements::WATER, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname, ps_vec, plotpar); ps_vec.clear();



	pPs = new Ps(&Min_GWLev, datatype::SINGLE, elements::WATER, fysprocess::DRAINAGE, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs, "Min_GwLev", -2.5, "m", "", -100, -0.1);
	pPs = new Ps(&Max_GWLev, datatype::SINGLE, elements::WATER, fysprocess::DRAINAGE, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs, "Max_GwLev", 0., "m", "", 0., 0.1);

	pPs = new Ps(&Sens_GWLev, datatype::SINGLE, elements::WATER, fysprocess::DRAINAGE, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs, "Sens_GwLev", 2, UNIT_TYPES::NO_UNIT, "", 1., 100.);
	pPs = new Ps(&PowerCoef_GWLev, datatype::SINGLE, elements::WATER, fysprocess::DRAINAGE, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs, "PowerCoef_GwLev", 1., UNIT_TYPES::NO_UNIT, "", 1., 100.);

	functorprop.FuncName = MyFunc::HBV_GROUNDWATERLEVEL; functorprop.Arg = MyArgType::D1;
	functorprop.FunctorInitOption = std::string::npos;
	funcname = "HBV GroundWater Level Function";
	pFunc = new Func( datatype::SINGLE, elements::WATER, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname, ps_vec, plotpar); ps_vec.clear();





	Xs* pXs;
	pXs = new Xs(&SoilStorage, datatype::SINGLE, elements::WATER, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pXs, "Soil Storage", 0., "mm");
	pXs = new Xs(&PeakStorage, datatype::SINGLE, elements::WATER, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pXs, "Peak GW Storage", 0., "mm");
	pXs = new Xs(&BaseStorage, datatype::SINGLE, elements::WATER, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pXs, "Base GW Storage", 0., "mm");

	Ts* pTs;
	pTs = new Ts(&HBV_Input_Soil, datatype::SINGLE, elements::WATER, fysprocess::INFILTRATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pTs, "HBV Input SOIL", 0.0,UNIT_TYPES::WATERFLOW_UNIT);
	pTs = new Ts(&HBV_Input_GW, datatype::SINGLE, elements::WATER, fysprocess::REDISTRIBUTION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pTs, "HBV Input GW", 0.0,UNIT_TYPES::WATERFLOW_UNIT);
	pTs = new Ts(&HBV_GW_Redist, datatype::SINGLE, elements::WATER, fysprocess::REDISTRIBUTION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pTs, "HBV GW ReDist", 0.0,UNIT_TYPES::WATERFLOW_UNIT);
	pTs = new Ts(&Q_Base, datatype::SINGLE, elements::WATER, fysprocess::RUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pTs, "Base Flow", 0.0,UNIT_TYPES::WATERFLOW_UNIT);
	pTs = new Ts(&Q_Peak, datatype::SINGLE, elements::WATER, fysprocess::RUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pTs, "Peak Flow", 0.0,UNIT_TYPES::WATERFLOW_UNIT);
	Gs *pGs = new Gs(&HBV_GWLev, datatype::SINGLE, elements::WATER, fysprocess::RUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pGs, "HBV GroundWaterLevel", 0.0,UNIT_TYPES::WATERFLOW_UNIT);
	pTs = new Ts(&Q, datatype::SINGLE, elements::WATER, fysprocess::RUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pTs, "HBV RunOff", 0.0,UNIT_TYPES::WATERFLOW_UNIT);
	pTs = new Ts(&HBV_Evaporation, datatype::SINGLE, elements::WATER, fysprocess::EVAPOTRANSPIRATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pTs, "HBV Evap", 0.0,UNIT_TYPES::WATERFLOW_UNIT, "");

	return true;
}

bool HBV::Ini()	{
	f_ActEvaporation = Get_D2_Functor(MyFunc::HBV_ACTUALEVAPORATION);

	f_Infiltration = Get_D3_Functor(MyFunc::HBV_INFILTRATION);
	f_GroundWaterDistribution = Get_D1_Functor(MyFunc::HBV_GROUNDWATER_REDIST);
	f_PeakRunOff = Get_D1_Functor(MyFunc::HBV_PEAK_RUNOFF);
	f_BaseRunOff = Get_D1_Functor(MyFunc::HBV_BASE_RUNOFF);
	f_GroundWaterLevel = Get_D1_Functor(MyFunc::HBV_GROUNDWATERLEVEL);

	if(!m_pModelInfo->StartStateValue) {
		SoilStorage=Initial_SoilStorage;
		PeakStorage=Initial_PeakStorage;
		BaseStorage=Initial_BaseStorage;
	}  
	T_Step=1.;
	HBV_GWLev=f_GroundWaterLevel(PeakStorage+BaseStorage);

	return true;
}
bool HBV::Dyn()
{
	Flux();
	Integ();
	return true;
}

void HBV::Flux()
{
	// Definition of Dynamic inputs from linked Modules
	auto ThroughFallFunc = [&]() {
		if (m_pMet != nullptr) {
			if (m_pMet->WTotSnow > 1.E-8 || m_pMet->SnowOutFlow > 0)
				return 	m_pMet->SnowOutFlow;
			else
				return m_pPlant->ThroughFall;
		}
		else
			return 0.;
	};

	HBV_Input=ThroughFallFunc();
	double PotEvap = 0;	if (m_pPlant != nullptr) PotEvap= m_pPlant->PotWaterUptake;

//  Actual evaporation and input to the soil
	HBV_Evaporation = f_ActEvaporation( SoilStorage, PotEvap);

	HBV_Input_Soil = f_Infiltration( SoilStorage, HBV_Input, HBV_Evaporation);


	HBV_Input_GW=HBV_Input-HBV_Input_Soil; // Input to GroundWater

	HBV_GW_Redist = f_GroundWaterDistribution(PeakStorage); // Redistribution between Ground Water Compartments

	InFlow(PeakStorage, HBV_Input_GW); // Update of PeakStorage prior estimation of RunOff 
	InFlow(BaseStorage, HBV_GW_Redist);  // Update of BaseStorage prior estimation of RunOff

	Q_Peak =f_PeakRunOff(PeakStorage);  // Runoff from Peakstorage
	Q_Base = f_BaseRunOff(BaseStorage);  // Runoff from Basestorage

	HBV_GWLev = f_GroundWaterLevel(PeakStorage + BaseStorage); // GroundWater Level
	Q=Q_Peak+Q_Base; // Total RunOff
}
void HBV::Integ()
{
	InFlow(SoilStorage, HBV_Input_Soil);
	OutFlow(SoilStorage, HBV_Evaporation);

	OutFlow(BaseStorage, Q_Base);
	OutFlow(PeakStorage, Q_Peak);
	
	OutFlow(PeakStorage,HBV_GW_Redist);
}

bool HBV::End()
{
return true;
}
HBV::~HBV(void)
{
}
