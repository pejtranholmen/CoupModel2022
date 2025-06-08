#include "SoilEvaporation_Functions.h"
#include "../SOIL/SoilModel.h"
#include "../PLANT/PlantModel.h"
#include "../PLANT_NC/PlantOrgModel.h"
#include "../PLANT_NC/NC_Plant.h"
#include "../PhysConst.h"
SoilEvaporation_Functions::SoilEvaporation_Functions(void)
{
	ModuleNo = 13;
	ModuleConditions = "Evaporation>0";
	Ps *pPs;

}
SoilEvaporation_Functions::~SoilEvaporation_Functions(void)
{
}



bool SoilEvaporation_Functions::Def()
{
	SnowPack::Def();
	
	ModuleNo = 13;
	ModuleConditions = "Evaporation>0";
	Ps *pPs;

	PlantModel* pPlant = (PlantModel*)m_pModelInfo->GetModelPtr("Plant Model");
	mp_Plant = &pPlant->m_WaterUptake;
	PlantOrgModel* pNCPlant = (PlantOrgModel*)m_pModelInfo->GetModelPtr("Plant Org Model");
	NC_Plant *pNC_Plant = &pNCPlant->m_NC_Plant;

	Sw* pSw;
	pSw = new Sw(&SoilEvap, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pSw, "Evaporation Method", 0, "Not Estimated|PM-eq, Rs(1Par)|PM-eq, Rs(3Par)|Iterative Energy Balance|K-function", "Evaporation > 1|WaterEq = 1", 201);
	pSw = new Sw(&SurEbal, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pSw, "Surface Temperature", 0, "Air Temperature|f(PM-equation)|f(E-balance Solution)", "WaterEq = 1|Evaporation Method > 0", 201);
	pSw = new Sw(&StabilitySoil, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pSw, "Stability Correction", 1, "f(Richardson Number)|f(Monin-Obukhov Length)", "WaterEq = 1|Surface Temperature >=2|Evaporation Method > 0");
	pSw = new Sw(&SoilRoughness, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pSw, "SoilRoughness", 0, "IndividualR|CommonR", "WaterEq = 1|Surface Temperature >=2|Evaporation Method > 0");
	pSw = new Sw(&SoilPartitioningArea, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pSw, "SoilPartitioningArea", 0, "No|Based on Dripp Irrig|Based on Drip Irrig and Radiation", "WaterEq = 1|Surface Temperature >=2|Evaporation Method > 0");
	pSw = new Sw(&SurfaceCover, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pSw, "SurfaceCover", 0, "No|Dyn plastic sheet|Permanent plastic sheet", "WaterEq = 1|Surface Temperature >=2|Evaporation Method > 2");
	//    !=====================	
	//    !     PARAMETERS
	//    !=====================
	Func *pFunc;
	string funcname;
	FunctorProp functorprop;
	vector<Ps*> ps_vec;
	ps_vec.clear();

	PLOT_PARAM plotpar;
	plotpar.Num_Of_Curves = 4;
	plotpar.Num_Of_Points = 300;
	plotpar.Num_Of_X = 3;
	plotpar.X_Min = 2;
	plotpar.X_Max = 5;

	plotpar.LogXScale = true;
	plotpar.LogYScale = false;
	plotpar.Num_Of_Opt = 1;
	plotpar.X2_Vec = { 10., 10.,10., 10.};
	plotpar.X3_Vec = { -2.f, -1.f, 0.f,1.f };
	
	plotpar.LegendsVec = { "2 mm deficit", "1 mm deficit", "equilibrium","1 mm surplus" };

	functorprop.FuncName = MyFunc::REL_HUMIDITY_GAS_EQUILIBRIUM; functorprop.Arg = MyArgType::D3;
	funcname = "Gas Liquid Phase Function";

	plotpar.X_Label = "Pressure Head ";	 plotpar.X_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::PRESSUREHEAD_UNIT, true);
	plotpar.Y_Label = "Relative Humidity ";	 plotpar.Y_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT, true);

	pPs = new Ps(&EGPsi, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs, "EquilAdjustPsi", 1., "-", "Evaporation > 0|Evaporation Method = 3|WaterEq = 1", 0., 5., 0., 2.5);


	pFunc = new Func( datatype::SOILSURFACE, elements::ATMOSPHERE, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname,  ps_vec, plotpar); ps_vec.clear(); 
	
	functorprop.FuncName = MyFunc::SOIL_SURFACE_RES_1P; functorprop.Arg = MyArgType::D2;
	funcname = "Soil Surface Resistance 1 par Function";
	
	
	plotpar.X2_Vec = { -2.f, -1.f, 0.f,1.f };
	plotpar.Num_Of_X = 2;
	plotpar.LegendsVec = { "2 mm deficit", "1 mm deficit", "equilibrium","1 mm surplus" };
	plotpar.Y_Label = "Surface Resistance ";	 plotpar.Y_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::AERODYNAMIC_RESISTANCE_UNIT, true);
	pPs = new Ps(&PsiRs_1p, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs, "PsiRs_1p", 200., "", "Evaporation > 1|Evaporation Method = 1|WaterEq = 1", 0., 1.E4, 50., 500.);

	pFunc = new Func( datatype::SOILSURFACE, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname,  ps_vec, plotpar); ps_vec.clear(); 


	functorprop.FuncName = MyFunc::SOIL_SURFACE_RES_3P; functorprop.Arg = MyArgType::D2;
	funcname = "Soil Surface Resistance 3 par Function";
	plotpar.LogYScale = true;
	pPs = new Ps(&PsiRs_3pf1, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
	string unit = "1 /"; unit +=p_ModelInfo->GetUnitString(UNIT_TYPES::PRESSUREHEAD_UNIT);
	Define(pPs, "PsiRs_3pf1", 1.,unit, "Evaporation > 1|Evaporation Method = 2|WaterEq = 1", 0.01, 5., 0.1, 5.);//!LK changed to max 20 instead of 5.
	pPs = new Ps(&PsiRs_3pf2, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
	Define(pPs, "PsiRs_3pf2", 300., UNIT_TYPES::PRESSUREHEAD_UNIT, "Evaporation > 1|Evaporation Method = 2|WaterEq = 1", 1., 1000.);
	pPs = new Ps(&PsiRs_3pf3, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
	Define(pPs, "PsiRs_3pf3", 100., "1/mm", "Evaporation > 1|Evaporation Method = 2|WaterEq = 1", 10., 1000.);

	pFunc = new Func( datatype::SOILSURFACE, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname,  ps_vec, plotpar); ps_vec.clear(); 


	pPs = new Ps(&MaxSoilCondens, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs, "MaxSoilCondens", 2.,UNIT_TYPES::WATERFLOW_UNIT, "Evaporation > 1|Evaporation Method > 0|WaterEq = 1", 0., 1000., 1., 4.);
	pPs = new Ps(&MaxSurfDeficit, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs, "MaxSurfDeficit", -2., "mm", "Evaporation > 1|Evaporation Method > 0|Evaporation Method < 4|WaterEq = 1", -4., 0., -3., -1.);
	pPs = new Ps(&MaxSurfExcess, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs, "MaxSurfExcess", 1., "mm", "Evaporation > 1|Evaporation Method > 0|Evaporation Method < 4|WaterEq = 1", 0., 3., 0.5, 2.);



	functorprop.FuncName = MyFunc::RA_BELOWCANOPYLAI; functorprop.Arg = MyArgType::D1;
	functorprop.FunctorInitOption = std::string::npos;
	funcname = "Below Canopy Aerodynamic Resistance Function";
	plotpar.Num_Of_Curves = 1;
	plotpar.Num_Of_Points = 100;
	plotpar.Num_Of_X = 1;
	plotpar.X_Min = 0;
	plotpar.X_Max = 6;
	plotpar.LogXScale = false;
	plotpar.LogYScale = false;
	plotpar.X_Label = "Leaf Area Index ";	 plotpar.X_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT, true);
	plotpar.Y_Label = "Resistance ";	 plotpar.Y_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::AERODYNAMIC_RESISTANCE_UNIT, true);
	plotpar.LegendsVec = { "" };

	pPs = new Ps(&RaLAI, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::TURBULENCE, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
	Define(pPs, "RaIncreaseWithLAI", 50., "s/m", "PlantType > 1|Evaporation Method > 0|WaterEq = 1", 1., 2000., 20., 100.);

	pFunc = new Func( datatype::ATMBOUNDARY, elements::ATMOSPHERE, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname,  ps_vec, plotpar);

	pPs = new Ps(&WindLessExchangeSoil, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::TURBULENCE, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs, "WindLessExchangeSoil", 0.001, "-", "WaterEq = 1|Surface Temperature >=2|Evaporation Method > 0", 0.0, 1.0);
	pPs = new Ps(&RoughLBareSoilMom, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::TURBULENCE, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs, "RoughLBareSoilMom", 0.001, "m", "Evaporation Method > 0|WaterEq = 1", 0.00000001, 5., 0.00001, .05);

	ps_vec.clear();
	functorprop.FuncName = MyFunc::RA_HEATSOIL_NEUTRAL; functorprop.Arg = MyArgType::D2;
	functorprop.FunctorInitOption = std::string::npos;
	funcname = "Aerodynamic Resistance (Neutral) above bare soil";
	plotpar.Num_Of_Curves = 2;
	plotpar.Num_Of_X = 2;
	plotpar.X_Min = 0;
	plotpar.X_Max = 15.;
	plotpar.LogYScale = true;
	plotpar.Y_Label = "Resistance ";  plotpar.Y_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::AERODYNAMIC_RESISTANCE_UNIT, true);
	plotpar.X_Label = "Wind Speed "; plotpar.X_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::WIND_SPEED_UNIT, true);
	plotpar.X2_Vec = { 2.f, 10.f };
	plotpar.LegendsVec = { "Height 2 m","Height 10 m" };

	ps_vec.push_back(m_pModelMap->GetPsPointer("WindLessExchangeSoil"));
	ps_vec.push_back(m_pModelMap->GetPsPointer("KBMinusOne"));
	ps_vec.push_back(m_pModelMap->GetPsPointer("RoughLBareSoilMom"));

	pFunc = new Func( datatype::ATMBOUNDARY, elements::ATMOSPHERE, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname, ps_vec, plotpar) ;

	functorprop.FuncName = MyFunc::RA_HEATSOIL_ANALYTICAL; functorprop.Arg = MyArgType::D5;
	functorprop.FunctorInitOption = std::string::npos;
	funcname = "Aerodynamic Resistance above soil surface - with plant";
	plotpar.Num_Of_Curves = 3;
	plotpar.Num_Of_X = 5;
	plotpar.X2_Vec = { 2.f, 2.0f, 2.0f };   // Height
	plotpar.X3_Vec = { 0.f, 0.f, 0.f };	  // Diff Temp
	plotpar.X4_Vec = { 10.f, 10.f, 10.f};	   //  Temp
	plotpar.X5_Vec = { 0.5f, 2.f, 5.f };	   // LAI

	plotpar.LegendsVec = { "LAI 0.5","LAI 2","LAI 5" };


	ps_vec.insert(ps_vec.begin(), m_pModelMap->GetPsPointer("RaIncreaseWithLAI"));

	ps_vec.push_back(m_pModelMap->GetPsPointer("StabCoefStableRich"));
	ps_vec.push_back(m_pModelMap->GetPsPointer("StabCoefStableExp"));
	ps_vec.push_back(m_pModelMap->GetPsPointer("StabCoefUnstableRich"));
	ps_vec.push_back(m_pModelMap->GetPsPointer("StabCoefUnstableExp"));




	pFunc = new Func( datatype::SOILSURFACE, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname,  ps_vec, plotpar);

	functorprop.FuncName = MyFunc::RA_HEATSOIL_NUMERICAL; functorprop.Arg = MyArgType::D5;
	functorprop.FunctorInitOption = std::string::npos;
	plotpar.Num_Of_X = 5;
	funcname = "Aerodynamic Resistance above soil surface (Monin-Obukkov) - with plant";
	pFunc = new Func( datatype::SOILSURFACE, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname, ps_vec, plotpar);


	functorprop.FuncName = MyFunc::RA_HEATSOIL_ANALYTICAL; functorprop.Arg = MyArgType::D4;
	functorprop.FunctorInitOption = std::string::npos;
	funcname = "Aerodynamic Resistance above soil surface ";
	plotpar.Num_Of_X = 2;
	plotpar.Num_Of_Curves = 2;
	plotpar.X2_Vec = { 2.f, 10.f };   // Height
	plotpar.X3_Vec = { 0.f, 0.f };	  // Diff Temp
	plotpar.X4_Vec = { 10.f, 10.f };	   //  Temp

	plotpar.LegendsVec = { "Height 2","Height 10" };

	pFunc = new Func( datatype::SOILSURFACE, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname,  ps_vec, plotpar);


	functorprop.FuncName = MyFunc::RA_HEATSOIL_NUMERICAL; functorprop.Arg = MyArgType::D4;
	functorprop.FunctorInitOption = std::string::npos;
	plotpar.Num_Of_X = 4;

	funcname = "Aerodynamic Resistance above soil surface (Monin-Obukkov) ";
	pFunc = new Func( datatype::SOILSURFACE, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname,  ps_vec, plotpar);


	functorprop.FuncName = MyFunc::STABILITY_ANALYTICAL; functorprop.Arg = MyArgType::D4;
	functorprop.FunctorInitOption = std::string::npos; 
		funcname = "Analytical Atmosphere Stability Function";
	pFunc = new Func( datatype::SOILSURFACE, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
	for (size_t i = 0; i < 4; i++) {ps_vec.erase(ps_vec.begin());}
	Define(pFunc, functorprop, funcname,  ps_vec, plotpar);


	ps_vec.clear(); 

	functorprop.FuncName = MyFunc::TEMPDIFF_FROMSENSIBLEFLOW; functorprop.Arg = MyArgType::BLANK;
	funcname = "Temperature Diff as Function of Flux and Resistance";
	pFunc = new Func( datatype::SOILSURFACE, elements::HEAT, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname,  ps_vec);

	functorprop.FuncName = MyFunc::NO_PARFUNC; functorprop.Arg = MyArgType::D5;
	functorprop.PhysFuncName = FUNC_CHOICE::PM;
	funcname = "Penman Monteith Equation - Soil Surface";
	pFunc = new Func( datatype::SOILSURFACE, elements::HEAT, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname,  ps_vec, plotpar);


 	pPs= new Ps(&SoilCoverEvap, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
    Define(pPs,   "SoilCoverEvap",0.,"-","Evaporation > 0|Evaporation Method > 2|WaterEq = 1|SurfaceCover >0",0., 1.);
 	pPs= new Ps(&SoilCoverRunOff, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
    Define(pPs,   "SoilCoverRunOff",0.,"-","Evaporation > 0|Evaporation Method > 2|WaterEq = 1|SurfaceCover >0",0., 1.);

    //!======================	
    //! FLUX VARIABLES    
    //!======================	
 	Ts *pTs= new Ts(&SoilEvaporationFlux, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
 
    Define(pTs,  "SoilEvaporation",0.,UNIT_TYPES::WATERFLOW_UNIT,"Evaporation Method > 0|Evaporation > 1|WaterEq = 1");
    //!======================	
    //! AUXILIARY VARIABLES     
    //!======================
	Gs *pGs= new Gs(&Surfmos, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
	    Define(pGs,   "SurfmoistureBalance", 0.,"mm","Evaporation > 0|Evaporation Method > 0|WaterEq = 1") ;
	pGs= new Gs(&RaSoilSurf, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
		Define(pGs,  "ResAirAboveSoil", 0.,"s/m", "Evaporation > 1|Evaporation Method > 0|WaterEq = 1");
	pGs= new Gs(&RsSoilSurf, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
	    Define(pGs,  "ResSoilSurface", 0.,"s/m", "Evaporation > 1||Evaporation Method > 0|Surface Temperature <2|WaterEq = 1") ;
	pGs= new Gs(&EAvailSurf, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
	    Define(pGs,  "EAvailableSurf", 0.,UNIT_TYPES::HEATFLOW_UNIT,"Evaporation > 0|Surface Temperature > 0|Surface Temperatue <2|WaterEq = 1");
	pGs= new Gs(&SoilLatentF, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
	    Define(pGs,  "SoilLatentFlow", 0.,UNIT_TYPES::HEATFLOW_UNIT, "Evaporation > 0|Surface Temperature > 0|WaterEq = 1");
	pGs= new Gs(&SoilSensF, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
	    Define(pGs,  "SoilSensibelFlow", 0.,UNIT_TYPES::HEATFLOW_UNIT,"Evaporation > 0|Surface Temperature > 0|WaterEq = 1");
	pGs= new Gs(&RadNetBareSoil, datatype::ATMBOUNDARY, elements::RADIATION, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
	    Define(pGs,  "RadNetBareSoil", 0.,UNIT_TYPES::HEATFLOW_UNIT,"Evaporation > 1|Surface Temperature > 1");

    //! DG 080902
	pGs= new Gs(&RadNetShortBareSoil, datatype::ATMBOUNDARY, elements::RADIATION, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
	    Define(pGs,  "RadNetShortBareSoil", 0.,UNIT_TYPES::HEATFLOW_UNIT,"Evaporation > 1|Surface Temperature > 1");
	pGs= new Gs(&RadNetLongBareSoil, datatype::ATMBOUNDARY, elements::RADIATION, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
	    Define(pGs,  "RadNetLongBareSoil", 0.,UNIT_TYPES::HEATFLOW_UNIT,"Evaporation > 1|Surface Temperature > 1");
    //! END DG 080902

	pGs= new Gs(&SoilEbal, datatype::ATMBOUNDARY, elements::HEAT, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
	    Define(pGs,  "EBalanceClosure", 0.,UNIT_TYPES::HEATFLOW_UNIT, "Evaporation > 0|Surface Temperature > 0|WaterEq = 1");
	pGs= new Gs(&VapourPSurf, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
	    Define(pGs,  "VapourPSurf", 0.,"Pa", "Evaporation > 0|Surface Temperature > 0|WaterEq = 1");
	pGs= new Gs(&PotEvapGround, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
	    Define(pGs,   "PotEvapGround", 0.,UNIT_TYPES::WATERFLOW_UNIT, "Evaporation > 1|Evaporation Method > 0|WaterEq = 1");
	pGs= new Gs(&TempBareSoil, datatype::ATMBOUNDARY, elements::TEMPERATURE, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
	    Define(pGs,  "TempBareSoil"  , 10.,UNIT_TYPES::TEMP_UNIT,"Evaporation > 0|HeatEq >= 1");
	
    //!==============================
    //! AUXILIARY VARIABLES (AREA 1)    
    //!==============================

	pTs= new Ts(&SoilEvaporation1, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
	    Define(pTs,  "SoilEvaporation1",0.,UNIT_TYPES::WATERFLOW_UNIT		,"SoilPartitioningArea>0|Evaporation > 0|Surface Temperature > 0|WaterEq = 1");
	pTs= new Ts(&SurfHeatFlow1, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
		Define(pTs,  "SurfHeatFlow1",0.,UNIT_TYPES::HEATFLOW_UNIT,"SoilPartitioningArea>0|Evaporation Method > 0|Evaporation > 1");
 	pGs= new Gs(&Fraction1, datatype::ATMBOUNDARY, elements::ATMOSPHERE, fysprocess::TURBULENCE, bioprocess::NOBPROC, userlevel::NORMAL); 
		Define(pGs,   "Fraction of soil Area1", 0.,"-",			"SoilPartitioningArea>0|Evaporation > 0|Evaporation Method > 0|WaterEq = 1") ;
	pGs= new Gs(&SoilLatentF1, datatype::ATMBOUNDARY, elements::HEAT, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
	    Define(pGs,  "SoilLatentFlow1", 0.,UNIT_TYPES::HEATFLOW_UNIT		,"SoilPartitioningArea>0|Evaporation > 0|Surface Temperature > 0|WaterEq = 1");
	pGs= new Gs(&SoilSensF1, datatype::ATMBOUNDARY, elements::HEAT, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
    Define(pGs,  "SoilSensibelFlow1", 0.,UNIT_TYPES::HEATFLOW_UNIT		,"SoilPartitioningArea>0|Evaporation > 0|Surface Temperature > 0|WaterEq = 1");
	pGs= new Gs(&RadNetBareSoil1, datatype::ATMBOUNDARY, elements::RADIATION, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
    Define(pGs,  "RadNetBareSoil1", 0.,UNIT_TYPES::HEATFLOW_UNIT,"SoilPartitioningArea>0|Evaporation > 1|Surface Temperature > 1");

	pGs= new Gs(&TempBareSoil1, datatype::ATMBOUNDARY, elements::TEMPERATURE, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
    Define(pGs,  "TempBareSoil1"  , 10.,UNIT_TYPES::TEMP_UNIT,"SoilPartitioningArea>0|Evaporation > 0|HeatEq >= 1");
	pGs= new Gs(&SoilEBal1, datatype::ATMBOUNDARY, elements::HEAT, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
    Define(pGs,  "EBalanceClosure1", 0.,UNIT_TYPES::HEATFLOW_UNIT		, "SoilPartitioningArea>0|Evaporation > 0|Surface Temperature > 0|WaterEq = 1");
	pGs= new Gs(&SurfMos1, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
    Define(pGs,   "SurfmoistureBalance1", 0.,"mm"			,"SoilPartitioningArea>0|Evaporation > 0|Evaporation Method > 0|WaterEq = 1") ;
	pGs= new Gs(&RaSoilSurf1, datatype::ATMBOUNDARY, elements::ATMOSPHERE, fysprocess::TURBULENCE, bioprocess::NOBPROC, userlevel::NORMAL); 
    Define(pGs,  "ResAirAboveSoil1", 0.,"s/m"				,"SoilPartitioningArea>0|Evaporation > 1|Evaporation Method > 0|WaterEq = 1");

    //!==============================
    //! AUXILIARY VARIABLES (AREA 2)    
    //!==============================
	pTs= new Ts(&SoilEvaporation2, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
		Define(pTs,  "SoilEvaporation2",0.,UNIT_TYPES::WATERFLOW_UNIT		,"SoilPartitioningArea>0|Evaporation > 0|Surface Temperature > 0|WaterEq = 1");
	pTs= new Ts(&SurfHeatFlow2, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
		Define(pTs,  "SurfHeatFlow2",0.,UNIT_TYPES::HEATFLOW_UNIT,"SoilPartitioningArea>0|Evaporation Method > 0|Evaporation > 1");
	pGs= new Gs(&SoilLatentF2, datatype::ATMBOUNDARY, elements::HEAT, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
		Define(pGs,  "SoilLatentFlow2", 0.,UNIT_TYPES::HEATFLOW_UNIT		,"SoilPartitioningArea>0|Evaporation > 0|Surface Temperature > 0|WaterEq = 1");
  	pGs= new Gs(&SoilSensF2, datatype::ATMBOUNDARY, elements::HEAT, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);   
		Define(pGs,  "SoilSensibelFlow2", 0.,UNIT_TYPES::HEATFLOW_UNIT		,"SoilPartitioningArea>0|Evaporation > 0|Surface Temperature > 0|WaterEq = 1");
	pGs= new Gs(&RadNetBareSoil2, datatype::ATMBOUNDARY, elements::RADIATION, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);    
		Define(pGs,  "RadNetBareSoil2", 0.,UNIT_TYPES::HEATFLOW_UNIT,"SoilPartitioningArea>0|Evaporation > 1|Surface Temperature > 1");
	pGs= new Gs(&TempBareSoil2, datatype::ATMBOUNDARY, elements::TEMPERATURE, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
		Define(pGs,  "TempBareSoil2"  , 10.,UNIT_TYPES::TEMP_UNIT,"SoilPartitioningArea>0|Evaporation > 0|HeatEq >= 1");
	pGs= new Gs(&SoilEBal2, datatype::ATMBOUNDARY, elements::HEAT, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);    
		Define(pGs,  "EBalanceClosure2", 0.,UNIT_TYPES::HEATFLOW_UNIT		, "SoilPartitioningArea>0|Evaporation > 0|Surface Temperature > 0|WaterEq = 1");
	pGs= new Gs(&SurfMos2, datatype::ATMBOUNDARY, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL); 
		Define(pGs,   "SurfmoistureBalance2", 0.,"mm"			,"SoilPartitioningArea>0|Evaporation > 0|Evaporation Method > 0|WaterEq = 1") ;
	pGs= new Gs(&RaSoilSurf2, datatype::ATMBOUNDARY, elements::ATMOSPHERE, fysprocess::TURBULENCE, bioprocess::NOBPROC, userlevel::NORMAL); 
		Define(pGs,  "ResAirAboveSoil2", 0.,"s/m"				,"SoilPartitioningArea>0|Evaporation > 1|Evaporation Method > 0|WaterEq = 1");

	return FunctionDef();
}

bool SoilEvaporation_Functions::FunctionDef() {
	Func *pFunc;
	/*double (SoilEvaporation_Functions::*fpoint)(double);
		fpoint=&SoilEvaporation_Functions::RaBound;
		pFunc= new Func( datatype::ATMBOUNDARY, elements::ATMOSPHERE, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pFunc, m_fpointer0_1.size(),0,this, "Below Canopy Aerodynamic Resistance Function"); m_fpointer0_1.push_back(fpoint);
		double res=pFunc->GetValue(3.);

	double (SoilEvaporation_Functions::*fpoint3)(double, double, double);
		fpoint3=&SoilEvaporation_Functions::RelHumidity;
		pFunc= new Func( datatype::SOILSURFACE, elements::ATMOSPHERE, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pFunc, m_fpointer0_3.size(),2,this, "Gas Liquid Phase Function"); m_fpointer0_3.push_back(fpoint3);
		res=pFunc->GetValue(30.,20.,1.);

	double (SoilEvaporation_Functions::*fpoint2)(double, double);
		fpoint2=&SoilEvaporation_Functions::RsSoil;
		pFunc= new Func( datatype::SOILSURFACE, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pFunc, m_fpointer0_2.size(),1,this, "Soil Surface Resistance 1 par Function"); m_fpointer0_2.push_back(fpoint2);
		res=pFunc->GetValue(1000.,3.);

		fpoint2=&SoilEvaporation_Functions::RsSoil;
		pFunc= new Func( datatype::SOILSURFACE, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pFunc, m_fpointer0_2.size(),1,this, "Soil Surface Resistance 3 par Function"); m_fpointer0_2.push_back(fpoint2);
		res=pFunc->GetValue(1000.,3.);*/

	/*
		fpoint2=&SoilEvaporation_Functions::RAFuncSoil;
		pFunc= new Func( datatype::SOILSURFACE, elements::ATMOSPHERE, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pFunc, m_fpointer0_2.size(),1,this, "Soil Surface Aerodynamic Function"); m_fpointer0_2.push_back(fpoint2);
		res=pFunc->GetValue(3.,20.);

		fpoint=&SoilEvaporation_Functions::RaFuncSoilNeutral;
		pFunc= new Func( datatype::ATMBOUNDARY, elements::ATMOSPHERE, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pFunc, m_fpointer0_1.size(),0,this, "Soil Surface Aerodynamic Resistance Function"); m_fpointer0_1.push_back(fpoint);
		*/

/*		double (SoilEvaporation_Functions::*fpoint4)(double, double, double, double);
		fpoint4=&SoilEvaporation_Functions::StabilityCorrAnalytical;
		pFunc= new Func( datatype::SOILSURFACE, elements::VAPOUR, fysprocess::SOILEVAPORATION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pFunc, m_fpointer0_4.size(),3,this, "Analytical Atmosphere Stability Function"); m_fpointer0_4.push_back(fpoint4);
		res=pFunc->GetValue(10.,3,2.,1.);*/


	return true;
}


