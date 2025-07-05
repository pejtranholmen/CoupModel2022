#include "Radiation_Functions.h"
#include "../SOIL/SoilModel.h"
#include "../PLANT/PlantModel.h"
//
#include "../../Simulator/Simulator.h"
#include "../Structure/Structure.h"
#include "./Functors/Radiation_Functors.h"

Radiation_Functions::Radiation_Functions(void)
{
	Tsum_albedo=0;
}
Radiation_Functions::~Radiation_Functions(void)
{
}



bool Radiation_Functions::Def()
{

	Meteorological_Data::Def();

	ModuleNo = 12;
	ModuleConditions = "Radiation>0";

	SoilModel* pSoil = (SoilModel*)m_pModelInfo->GetModelPtr("Soil Model");
	p_Soil_WaterF = &pSoil->m_SoilFrost;
	PlantModel* pPlant = (PlantModel*)m_pModelInfo->GetModelPtr("Plant Model");
	p_Plant = &pPlant->m_WaterUptake;
	//m_pModelStructure = ((Simulator*)m_pModelInfo)->GetSimStructurePointer();


	//    ! Snow surface energy balance
	Sw* pSw;    //! PEJ Introduced RadiationInput to avoid crazy use of PlantType

	pSw = new Sw(&m_pModelStructure->RadiationInput, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);

	Define(pSw, "RadiationInput", 0, "No|Yes", "Develop < -100", 200);

	pSw = new Sw(&NetLongRad, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);

	Define(pSw, "LongWaveBalance", 0, "One formula f(AirTemp)|Two seperate formulas", "RadiationInput  > 0");

	//    ! DG 2003-01-28
	//    !	Define(pSw,  LongRadCanopy,"LongRadCanopy",0, "Beers law|f(TempCanopy)" ,"RadiationInput > 0|LongWaveBalance >= 1|PlantType=3");

	 //   ! DG 2003-08-18: Option names are not very clearifying
	 //   !                   Beers law is used in both cases for the transmissivity,
	 //   !                   but only alternative 1 uses outgoing longwave from Tempcanopy explicitly.
	 //   !                   Therefor implicit/explicit is better than Beer law/f(Tempcanopy).
	pSw = new Sw(&LongRadCanopy, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pSw, "LongRadCanopy", 0, "implicit|explicit f(TempCanopy)", "RadiationInput > 0|LongWaveBalance >= 1|PlantType=3");

	// ! DG 19/6														
	pSw = new Sw(&InLongRad, datatype::SINGLE, elements::RADIATION, fysprocess::EMISSION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pSw, "InLongRad", 0, "Konzelmann et al equation|Satterlunds equation|Brunts equation", "RadiationInput > 0|LongWaveBalance >= 1");
	// ! DG 24/4-99 Switch between old Potential radiation formulation and new from Pluss
	pSw = new Sw(&Turbidity, datatype::SINGLE, elements::RADIATION, fysprocess::ADSORPTION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pSw, "Turbidity", 0, "Constant|Function of solar angle", "RadiationInput > 0|TimeResolution!=1");
	//    ! SWITCHES AND AUXILIARY VARIABLES FOR SNOWMIP2 - MIGHT BE INCLUDED IN OFFICIAL RELEASE LATER
	pSw = new Sw(&SplitGlobal, datatype::SINGLE, elements::RADIATION, fysprocess::ADSORPTION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pSw, "SplitGlobal", 0, "no|yes", "RadiationInput > 0|LongWaveBalance >= 1|LongRadCanopy = 1", 202);// ! Set to 1 if CanopyShade is set to 1
	pSw = new Sw(&CanopyShade, datatype::SINGLE, elements::RADIATION, fysprocess::ADSORPTION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pSw, "CanopyShade", 0, "constant|f(solar elevation)", "RadiationInput > 0|LongWaveBalance >= 1|PlantType=3|LongRadCanopy=1|LongRadCanopy = 1", 202);




	// ! DG 14/12-01 Enable slope correction of input global radiation
 //!   Define(pSw,  SlopeCorrMeasuredGlobal,"SlopeCorrMeasuredGlobal",0, "No|Yes" ,"RadiationInput > 0|TimeResolution!=1");

   // ! DG 19/11-99 only for evaluation of the two functions
   // !	Define(pSw,  DistributeRadFunc,"DistributeRadFunc",0, "Per-Eriks|David" ,"PlantType>=3");
	Func *pFunc;
	string funcname;

	FunctorProp functorprop;
	PLOT_PARAM plotpar;
	plotpar.Num_Of_Curves = 1;
	plotpar.Num_Of_Points = 300;
	plotpar.Num_Of_X = 2;
	plotpar.X_Min = 0;

	plotpar.Y_Label = "Potential Transpiration (mm/day)";
	plotpar.LogXScale = false;
	plotpar.LogYScale = false;
	plotpar.Num_Of_Opt = 1;


	vector<Ps*> ps_vec;
	ps_vec.clear();


	Ps *pPs;
	functorprop.PhysFuncName = FUNC_CHOICE::DAYLENGTH; functorprop.Arg = MyArgType::D1;
	functorprop.FuncName = MyFunc::NO_PARFUNC;
	funcname = "Day Length During Year Function"; functorprop.FunctorInitOption = std::string::npos;

	plotpar.X_Max = 365;
	plotpar.X_Label = "Day Number";
	plotpar.Y_Label = "Day Length "; plotpar.Y_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::TIME_MINUTE_UNIT, true);



	pPs = new Ps(&Latitude, datatype::SINGLE, elements::RADIATION, fysprocess::GEOPOS, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs, "Latitude", 58.5, UNIT_TYPES::NO_UNIT, "RadiationInput  > 0", -80., 80., -75., 75., 6);  ps_vec.push_back(pPs);
	pFunc = new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);

	Define(pFunc, functorprop, funcname, ps_vec,plotpar);




	pFunc = new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	functorprop.PhysFuncName = FUNC_CHOICE::POTRISDAILY; functorprop.Arg = MyArgType::D1;
	funcname = "Potential Global Radiation Function"; functorprop.FunctorInitOption = std::string::npos;
	plotpar.Y_Label = "Global Radiation "; plotpar.Y_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::MEGA_HEATFLOW_UNIT, true);
	plotpar.Y_ScalingCoef = -6;

	Define(pFunc, functorprop, funcname, ps_vec,plotpar);







	pPs = new Ps(&Solar_Time_Adjust, datatype::SINGLE, elements::RADIATION, fysprocess::GEOMETRIC, bioprocess::NOBPROC, userlevel::NORMAL);  ps_vec.push_back(pPs);

	pFunc = new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs, "Solar Time Adjust", 0., "minutes", "RadiationInput > 0", -600., 600., -240., 240.);
	functorprop.PhysFuncName = FUNC_CHOICE::POTRISHOUR; functorprop.Arg = MyArgType::D2;
	functorprop.FuncName = MyFunc::NO_PARFUNC;
	funcname = "Potential Global Radiation Within Day Function"; functorprop.FunctorInitOption = std::string::npos;
	plotpar.Num_Of_Curves = 3;
	plotpar.Num_Of_Points = 100;
	plotpar.Num_Of_X = 2;
	plotpar.X_Min = 0;
	plotpar.X_Max = 24;
	plotpar.X_Label = "Hour of Day";
	plotpar.Y_Label = "Radiation ";  plotpar.Y_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::MEGA_HEATFLOW_UNIT, true);
	plotpar.LogXScale = false;
	plotpar.LogYScale = false;

	plotpar.Num_Of_Opt = 1;
	plotpar.LegendsVec = { "Winter","Spring", "Summer" };
	plotpar.X2_Vec = { 355.f, 81.f,172.f};
	Define(pFunc, functorprop, funcname,  ps_vec, plotpar);



	functorprop.FuncName = MyFunc::ATM_TRANSPARENCY; functorprop.Arg = MyArgType::D1;
	funcname = "Angstroms Short Wave Equation"; functorprop.FunctorInitOption = std::string::npos;
	plotpar.Num_Of_Curves = 1;
	plotpar.Num_Of_X = 1;
	plotpar.X_Min = 0;
	plotpar.X_Max = 1;
	plotpar.Y_ScalingCoef = 0;
	plotpar.Y_Label = "Degree of Radiation ";  plotpar.Y_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT, true);
	plotpar.X_Label = "Degree of Clear sky";   plotpar.X_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT, true);


	ps_vec.clear();
	pPs = new Ps(&RadFracAng1, datatype::SINGLE, elements::RADIATION, fysprocess::SCATTERING, bioprocess::NOBPROC, userlevel::NORMAL);  ps_vec.push_back(pPs);
		Define(pPs, "RadFracAng1", 0.22, UNIT_TYPES::NO_UNIT, "Evaporation > 0|RadiationInput  > 0", 0.15, 0.3, 0.2, 0.25);
	pPs = new Ps(&RadFracAng2, datatype::SINGLE, elements::RADIATION, fysprocess::SCATTERING, bioprocess::NOBPROC, userlevel::NORMAL);  ps_vec.push_back(pPs);
		Define(pPs, "RadFracAng2", 0.50, UNIT_TYPES::NO_UNIT, "Evaporation > 0|RadiationInput  > 0", 0.4, 0.6, 0.45, 0.55);
	
	pFunc = new Func( datatype::ATMBOUNDARY, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname,  ps_vec, plotpar);  ps_vec.clear();


	functorprop.FuncName = MyFunc::BEER_LAW_SINGLE_LEAF; functorprop.Arg = MyArgType::D1;
	funcname = "Beer's Law"; functorprop.FunctorInitOption = std::string::npos;
	plotpar.Num_Of_Curves = 1;
	plotpar.Num_Of_Points = 100;
	plotpar.Num_Of_X = 1;
	plotpar.X_Max = 10.;
	plotpar.Y_Label = "Radiation Adsorbed ";  plotpar.Y_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT, true);
	plotpar.X_Label = "Hour of Day";   plotpar.X_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT, true);
	plotpar.Y_ScalingCoef = 0;

	pPs= new Ps(&RntLAI, datatype::SINGLE, elements::RADIATION, fysprocess::ADSORPTION, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pPs,   "RntLAI", 0.5,UNIT_TYPES::NO_UNIT,"Evaporation > 0|PlantType =2",0.2,1.0,0.3,0.7); ps_vec.push_back(pPs);

	pFunc = new Func( datatype::ATMBOUNDARY, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL); 
	Define(pFunc, functorprop, funcname,  ps_vec, plotpar);  ps_vec.clear();



	NE *pNumPlants;
	pNumPlants = m_pModelMap->GetNEPointer("Number of Plants");
	vector<double> initv;
	P* pP;
	vector<P*> p_vec;

	pP = new P(&RntLAIv, pNumPlants, datatype::PLANT, elements::RADIATION, fysprocess::ADSORPTION, bioprocess::NOBPROC, userlevel::NORMAL); p_vec.push_back(pP);

	
	initv.assign(p_Plant->NumPlants, 0.5);

	Define(pP, pNumPlants, "RntLAIv", initv, UNIT_TYPES::NO_UNIT, "PlantType>=3", 0., 1.);
	mTab_Beer.push_back(pP);

	pFunc = new Func( datatype::ATMBOUNDARY, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);  
	functorprop.FuncName = MyFunc::BEER_LAW_PERLEAF; functorprop.Arg = MyArgType::D1;
	funcname = "Beer's Law multi canopy"; functorprop.FunctorInitOption = std::string::npos;
	

	Define(pFunc, functorprop, funcname,  p_vec, plotpar);  ps_vec.clear();


	pPs = new Ps(&AlbedoDry, datatype::SINGLE, elements::RADIATION, fysprocess::REFLECTION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
    Define(pPs,  "AlbedoDry", 30., "%", "Evaporation > 0|RadiationInput  > 0|PlantType !=1",10.,80.,20., 40.);
	pPs= new Ps(&AlbedoKExp, datatype::SINGLE, elements::RADIATION, fysprocess::REFLECTION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
    Define(pPs,  "AlbedoKExp",1.,UNIT_TYPES::NO_UNIT, "Evaporation > 0|SoilEvap > 0|RadiationInput  > 0|PlantType !=1", 0.1, 4., 0.5, 1.5);
	pPs = new Ps(&AlbedoWet, datatype::SINGLE, elements::RADIATION, fysprocess::REFLECTION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
	Define(pPs, "AlbedoWet", 15., "%", "Evaporation > 0|RadiationInput  > 0|PlantType !=1", 5., 25., 10., 20.);
	plotpar.Num_Of_Curves = 1;
	plotpar.Num_Of_Points = 100;
	plotpar.Num_Of_X = 1;
	plotpar.X_Min = 0;
	plotpar.X_Max = 5;
	plotpar.Y_Label = "Albedo (%)";
	plotpar.X_Label = "Pressure head (cm water)";
	plotpar.LogXScale = true;
	plotpar.LogYScale = false;
	plotpar.Num_Of_Opt = 1;
	functorprop.FuncName = MyFunc::ALBEDO_SOIL; functorprop.Arg = MyArgType::D1;
	funcname = "Bare Soil Albedo Function"; functorprop.FunctorInitOption = std::string::npos;
	pFunc = new Func( datatype::SOILSURFACE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname,  ps_vec,plotpar);  ps_vec.clear();

	NE* pNumAlbSnow;
	pNumAlbSnow = new NE(&NumAlbSnow, 3, 3, 3, 3, 3);
	Define(pNumAlbSnow, "Number of Snow Albedo");
	pPs = new Ps(&AlbSnowCoef_1, datatype::SINGLE, elements::RADIATION, fysprocess::REFLECTION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
	Define(pPs, "AlbSnowCoef_1", 50., UNIT_TYPES::NO_UNIT, "RadiationInput  > 0", 40., 55.);
	pPs = new Ps(&AlbSnowCoef_2, datatype::SINGLE, elements::RADIATION, fysprocess::REFLECTION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
	Define(pPs, "AlbSnowCoef_2", -0.05, UNIT_TYPES::NO_UNIT, "RadiationInput  > 0", -0.1, 0.);
	pPs = new Ps(&AlbSnowCoef_3, datatype::SINGLE, elements::RADIATION, fysprocess::REFLECTION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
	Define(pPs, "AlbSnowCoef_3", -0.1, UNIT_TYPES::NO_UNIT, "RadiationInput  > 0", -0.2, 0.);
	pPs = new Ps(&AlbSnowMin, datatype::SINGLE, elements::RADIATION, fysprocess::REFLECTION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
	Define(pPs, "AlbSnowMin", 40., UNIT_TYPES::NO_UNIT, "Evaporation > 0|SnowPack > 0", 20., 70., 30., 60.);//			! DG 8/7-97



	plotpar.Num_Of_Curves = 1;
	plotpar.Num_Of_Points = 100;
	plotpar.Num_Of_X = 1;
	plotpar.X_Min = 0;
	plotpar.X_Max = 100;
	plotpar.X_Label = "Number of Days";
	plotpar.Y_Label = "Albedo (%)";
	plotpar.LogXScale = false;
	plotpar.LogYScale = false;
	plotpar.Num_Of_Opt = 1;


	functorprop.FuncName = MyFunc::ALBEDO_SNOW_TSUMAIR; functorprop.Arg = MyArgType::D2;
	funcname = "Snow Age Albedo TempSum Function"; functorprop.FunctorInitOption = std::string::npos;
	pFunc = new Func( datatype::SOILSURFACE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname, ps_vec, plotpar);  


	ps_vec.push_back(m_pModelMap->GetPsPointer("Time Step"));

	functorprop.FuncName = MyFunc::ALBEDO_SNOW_TAIR; functorprop.Arg = MyArgType::D2;
	funcname = "Snow Age Albedo Function"; functorprop.FunctorInitOption = std::string::npos;
	pFunc = new Func( datatype::SOILSURFACE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);

	Define(pFunc, functorprop, funcname,  ps_vec,plotpar); 
	
	ps_vec.clear();



	pPs= new Ps(&Albedo, datatype::SINGLE, elements::RADIATION, fysprocess::REFLECTION, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pPs,  "Albedo",25.,"%","Evaporation >0|PlantType =1",5.,50.,8.,40.);


	functorprop.FuncName = MyFunc::RADLONG_OUT_EM; functorprop.Arg = MyArgType::D1;
	funcname = "Outgoing Long Wave Radiation"; functorprop.FunctorInitOption = std::string::npos;

	plotpar.Num_Of_X = 1;
	plotpar.X_Min = -20;
	plotpar.X_Max = 30;
	plotpar.X_Label = "Temperature";  plotpar.X_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::TEMP_UNIT, true);
	plotpar.Y_Label = "Radiation ";  plotpar.Y_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::MEGA_HEATFLOW_UNIT, true);
	plotpar.LogXScale = false;
	plotpar.LogYScale = false;
	plotpar.Y_ScalingCoef = -6;

	plotpar.Num_Of_Opt = 1;
	plotpar.LegendsVec = { ""};
	plotpar.X2_Vec = { };

	pPs= new Ps(&Emissivity, datatype::SINGLE, elements::RADIATION, fysprocess::EMISSION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pPs, "Emissivity", 1., UNIT_TYPES::NO_UNIT, "RadiationInput > 0|LongWaveBalance >= 1", .8, 1., .1, 1.);  ps_vec.push_back(pPs);

	pFunc = new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pFunc, functorprop, funcname,  ps_vec, plotpar);  ps_vec.clear();



	functorprop.FuncName = MyFunc::BRISTOW_FIXEDB; functorprop.Arg = MyArgType::D2;
	funcname = "Atmospheric Transparency from Daily Temp Range"; functorprop.FunctorInitOption = std::string::npos;
	
	pPs= new Ps(&ParB.Bristow_Turb, datatype::SINGLE, elements::RADIATION, fysprocess::SCATTERING, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs, "Bristow_Turb", 0.72, UNIT_TYPES::NO_UNIT, "RadiationInput > 0|CloudInput=4", 0., 1., 0.5, 0.8); 
	pPs= new Ps(&ParB.Bristow_B, datatype::SINGLE, elements::RADIATION, fysprocess::SCATTERING, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs,  "Bristow_B",0.07,UNIT_TYPES::NO_UNIT,"RadiationInput > 0|CloudInput=4",.01,.2);    
	pPs= new Ps(&ParB.Bristow_C, datatype::SINGLE, elements::RADIATION, fysprocess::SCATTERING, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs,  "Bristow_C",1.5,UNIT_TYPES::NO_UNIT,"RadiationInput > 0|CloudInput=4",1.,2.,1.2,1.8);



	pFunc = new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname, ps_vec,plotpar);
    
	pPs= new Ps(&ParB.Thornton_B0, datatype::SINGLE, elements::RADIATION, fysprocess::SCATTERING, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs,  "Thornton B0",0.031,UNIT_TYPES::NO_UNIT,"RadiationInput > 0|CloudInput=4",0.01,.05);    
	pPs= new Ps(&ParB.Thornton_B1, datatype::SINGLE, elements::RADIATION, fysprocess::SCATTERING, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
 	Define(pPs,  "Thornton B1",0.201,UNIT_TYPES::NO_UNIT,"RadiationInput > 0|CloudInput=4",0.1,.3);
	pPs= new Ps(&ParB.Thornton_B2, datatype::SINGLE, elements::RADIATION, fysprocess::SCATTERING, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs,  "Thornton B2",-0.185,UNIT_TYPES::NO_UNIT,"RadiationInput > 0|CloudInput=4",-0.3,-0.1);

	pPs = new Ps(&ParB.DTR_YEAR_MEAN, datatype::SINGLE, elements::RADIATION, fysprocess::SCATTERING, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs, "DTR Year Mean", 15.,UNIT_TYPES::TEMP_UNIT, "RadiationInput > 0|CloudInput=4", 0., 30., 3., 25.);
	pPs = new Ps(&ParB.DTR_YEAR_AMP, datatype::SINGLE, elements::RADIATION, fysprocess::SCATTERING, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs, "DTR Year Amp", 10.,UNIT_TYPES::TEMP_UNIT, "RadiationInput > 0|CloudInput=4", 0., 10., 3., 20.);
	pPs = new Ps(&ParB.DTR_YEAR_SHIFT, datatype::SINGLE, elements::RADIATION, fysprocess::SCATTERING, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs, "DTR Year Shift", 15., "DayNo", "RadiationInput > 0|CloudInput=4", 0., 30., 3., 25.);

	functorprop.FuncName = MyFunc::BRISTOW_SEASONALB; functorprop.Arg = MyArgType::D2;
	funcname = "Atmospheric Transparency from Daily Temp Range with seasonality"; functorprop.FunctorInitOption = std::string::npos;

	pFunc = new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname,  ps_vec); ps_vec.clear();

    
    //! Treated by new snow interception model!!
	pPs= new Ps(&AlbLeafSnowCoef, datatype::SINGLE, elements::RADIATION, fysprocess::REFLECTION, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pPs,  "AlbLeafSnowCoef", 0.5, UNIT_TYPES::NO_UNIT, "Evaporation > 0|RadiationInput  > 0|PlantType !=1|SnowInterception > 0 ",0., 1., 0.,1. );

    //! New Gryning Coefficient
	
	functorprop.FuncName = MyFunc::RAD_RATIO_DIFFUSE; functorprop.Arg = MyArgType::D2;
	funcname = "Ratio of Diffuse Radiation"; functorprop.FunctorInitOption = std::string::npos;
	plotpar.Num_Of_Curves = 1;
	plotpar.Num_Of_X = 1;
	plotpar.X_Min = 0;
	plotpar.X_Max = 1.;
	plotpar.X_Label = "Cloudiness ";  plotpar.X_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT, true);
	plotpar.Y_Label = "Diffuse Fraction ";  plotpar.Y_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT, true);
	plotpar.X2_Vec = { 1. };
	plotpar.LegendsVec = { ""};
	plotpar.Y_ScalingCoef = 0;

	pPs = new Ps(&DiffuseRadCoef, datatype::SINGLE, elements::RADIATION, fysprocess::ADSORPTION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
    Define(pPs,  "DiffuseRadCoef", 0.2, UNIT_TYPES::NO_UNIT, "Evaporation > 0|RadiationInput  > 0|SplitGlobal > 0",0.1, 0.5, 0.1,0.3 );

	
	pFunc = new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname, ps_vec, plotpar);  ps_vec.clear();



	pPs= new Ps(&ViewScalingMax, datatype::SINGLE, elements::RADIATION, fysprocess::ADSORPTION, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pPs,  "ViewScalingMax", 1000., UNIT_TYPES::NO_UNIT, "Evaporation > 0|RadiationInput  > 0|SplitGlobal > 0",100., 10000., 900.,1100. );
	
/*	pPs = new Ps(&CanopyDiameterHeightRatio_Common, datatype::SINGLE, elements::RADIATION, fysprocess::GEOMETRIC, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
    Define(pPs,  "DiameterHeightRatio", 0.2, UNIT_TYPES::NO_UNIT, "Evaporation > 0|RadiationInput  > 0|SplitGlobal > 0",0.001, 10., .05,5. );*/
	

	pPs = new Ps(&CanopyShadeCoef, datatype::SINGLE, elements::RADIATION, fysprocess::ADSORPTION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
	Define(pPs, "CanopyShadeCoef", .01, UNIT_TYPES::NO_UNIT, "Evaporation > 0|RadiationInput  > 0|SplitGlobal > 0", 0., 10., 0.001, 2.);
	
	pPs = new Ps(&CanopyShadeCoef_Base, datatype::SINGLE, elements::RADIATION, fysprocess::ADSORPTION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs, "CanopyShadeCoef_Base", .5, UNIT_TYPES::NO_UNIT, "Evaporation > 0|RadiationInput  > 0|SplitGlobal > 0", 0., 1., 0.001, 1.);


	functorprop.FuncName = MyFunc::CANOPY_SHADE_COVER; functorprop.Arg = MyArgType::D2;
	plotpar.Num_Of_Curves = 2;
	plotpar.Y_Label = "Enhanced Shadowing Factor ";  plotpar.Y_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT, true);
	funcname = "Shadowing Cover for Canopy Heights"; functorprop.FunctorInitOption = std::string::npos;
	plotpar.X2_Vec = { 0.1f, 100.f };
	plotpar.LegendsVec = { "1 m tree","10 m tree"};
	plotpar.X_Label = "Solar Zenit Angle ";  plotpar.X_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::ANGLE_UNIT, true);
	plotpar.X_Min = 0;
	plotpar.X_Max = 1.57;

	pFunc = new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname,  ps_vec, plotpar);  ps_vec.clear();

	NE* pNumBrunt;
	pNumBrunt=new NE(&NumBrunt, 4,4,4,4,4);
	Define(pNumBrunt, "Number of Brunt Coefficients");
	ps_vec.clear();
	initv.resize(NumBrunt);
	initv[0]=0.56;initv[1]=0.00779;initv[2]=0.1;initv[3]=0.9;
	pPs= new Ps(&BruntAirCoef_1, datatype::SINGLE, elements::RADIATION, fysprocess::EMISSION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
	Define(pPs,"BruntAirCoef_1",0.56,UNIT_TYPES::NO_UNIT,"LongWaveBalance=0|RadiationInput  > 0", 0.51, 0.61);
	pPs= new Ps(&BruntAirCoef_2, datatype::SINGLE, elements::RADIATION, fysprocess::EMISSION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
	Define(pPs,"BruntAirCoef_2",0.00779,UNIT_TYPES::NO_UNIT,"LongWaveBalance=0|RadiationInput  > 0", 0.007, 0.009);
	pPs= new Ps(&BruntAirCoef_3, datatype::SINGLE, elements::RADIATION, fysprocess::EMISSION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs,"BruntAirCoef_3",0.1,UNIT_TYPES::NO_UNIT,"LongWaveBalance=0|RadiationInput  > 0", 0.05, 0.15);
	pPs= new Ps(&BruntAirCoef_4, datatype::SINGLE, elements::RADIATION, fysprocess::EMISSION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs,"BruntAirCoef_4",0.9,UNIT_TYPES::NO_UNIT,"LongWaveBalance=0|RadiationInput  > 0", 0.8, 1.);

	functorprop.FuncName = MyFunc::RADNETLONG_SATURATED_AIR; functorprop.Arg = MyArgType::D2;
	funcname = "Net Long Wave Radiation"; functorprop.FunctorInitOption = std::string::npos;

	plotpar.Num_Of_Curves = 2;
	plotpar.Num_Of_Points = 100;
	plotpar.Num_Of_X = 2;
	plotpar.X_Min = -10;
	plotpar.X_Max = 20;
	plotpar.X_Label = "Air Temperature (C)";
	plotpar.Y_Label = "Outgoing Radiation (J/m2/day)";
	plotpar.LogXScale = false;
	plotpar.LogYScale = false;
	plotpar.Num_Of_Opt = 1;
	plotpar.LegendsVec={ "Cloudy","Clear Sky" };
	plotpar.X2_Vec = { 0., 1. };
	functorprop.Set_PlotPar = true;


	pFunc = new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	functorprop.FuncName = MyFunc::RADNETLONG_AIR; functorprop.Arg = MyArgType::D3;
	Define(pFunc, functorprop, funcname,  ps_vec,plotpar);

	functorprop.Set_PlotPar = false;

	ps_vec.clear();


	pPs= new Ps(&KonzelmannCoef_1, datatype::SINGLE, elements::RADIATION, fysprocess::EMISSION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pPs,"KonzelmannCoef_1",0.23,UNIT_TYPES::NO_UNIT,"LongWaveBalance > 0|InLongRad=0", 0.15, 0.31); ps_vec.push_back(pPs); 
	pPs= new Ps(&KonzelmannCoef_2, datatype::SINGLE, elements::RADIATION, fysprocess::EMISSION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pPs,"KonzelmannCoef_2",0.483,UNIT_TYPES::NO_UNIT,"LongWaveBalance > 0|InLongRad=0", 0.4, 0.56); ps_vec.push_back(pPs); 
	pPs= new Ps(&KonzelmannCoef_3, datatype::SINGLE, elements::RADIATION, fysprocess::EMISSION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pPs,"KonzelmannCoef_3",0.963,UNIT_TYPES::NO_UNIT,"LongWaveBalance > 0|InLongRad=0", 0.9, 0.99); ps_vec.push_back(pPs);
	
	functorprop.FuncName = MyFunc::RADLONGIN_KONZELMANN_EMISSIVITY; functorprop.Arg = MyArgType::D3;
	funcname = "Emissivity Long Wave Radiation (Konzelmann)"; functorprop.FunctorInitOption = std::string::npos;
	pFunc = new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	functorprop.Set_PlotPar = false;
	plotpar.Y_Label = "Emissivity atmosphere (-)";
	plotpar.Y_ScalingCoef = 0;
	Define(pFunc, functorprop, funcname,  ps_vec, plotpar);
	pFunc = new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	functorprop.Set_PlotPar = true; functorprop.Arg = MyArgType::D2;
	Define(pFunc, functorprop, funcname, ps_vec, plotpar);


	functorprop.FuncName = MyFunc::RADLONGIN_KONZELMANN; functorprop.Arg = MyArgType::D3;

	funcname = "Incoming Long Wave Radiation (Konzelmann)"; functorprop.FunctorInitOption = std::string::npos;
	pFunc = new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	functorprop.Set_PlotPar = false;
	plotpar.Y_Label = "Incomining Longwave Radiation (J/m2/day)";
	Define(pFunc, functorprop, funcname,  ps_vec,plotpar);	 

	pFunc = new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	functorprop.Set_PlotPar = true; functorprop.Arg = MyArgType::D2;
	Define(pFunc, functorprop, funcname, ps_vec, plotpar);	 ps_vec.clear();


	mTab_SnowAlb.push_back(pP);

	NE *pNumKonz;
	pNumKonz = new NE(&NumKonz,3,3,3,3,3);
	Define(pNumKonz, "Number of Konzelmann");
	pP= new P(&KonzelmannCoef,pNumKonz, datatype::SINGLE, elements::RADIATION, fysprocess::EMISSION, bioprocess::NOBPROC, userlevel::NORMAL);
		initv.resize(3); initv[0]=0.23; initv[1]=0.483; initv[2]=0.963;
		Define(pP,pNumKonz  ,"KonzelmannCoef",initv,UNIT_TYPES::NO_UNIT,"RadiationInput  > 0");




		mTab_Konzel.push_back(pP);
	NE *pNumSatterl;
	pNumSatterl = new NE(&NumSatterl,2,2,2,2,2);
	Define(pNumSatterl, "Number of Satterlund");
	initv.resize(NumSatterl);
	initv[0]=2016; initv[1]=0.3;
	pPs= new Ps(&SatterlundCoef_1, datatype::SINGLE, elements::RADIATION, fysprocess::EMISSION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
		Define(pPs,"SatterlundCoef_1",2016.,UNIT_TYPES::NO_UNIT,"RadiationInput  > 0|InLongRad=1", 2000., 2032.);
	pPs= new Ps(&SatterlundCoef_2, datatype::SINGLE, elements::RADIATION, fysprocess::EMISSION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
		Define(pPs,"SatterlundCoef_2",.3,UNIT_TYPES::NO_UNIT,"RadiationInput  > 0|InLongRad=1", 0.1, 0.7);
		
	functorprop.FuncName = MyFunc::RADLONGIN_SATTERLUND_EMISSIVITY; functorprop.Arg = MyArgType::D3;
	funcname = "Emissivity Long Wave Radiation (Satterlund)"; functorprop.FunctorInitOption = std::string::npos;
	pFunc = new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname, ps_vec);
	functorprop.FuncName = MyFunc::RADLONGIN_SATTERLUND; functorprop.Arg = MyArgType::D3;
	funcname = "Incoming Long Wave Radiation (Satterlund)"; functorprop.FunctorInitOption = std::string::npos;
	pFunc = new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname,  ps_vec);	 ps_vec.clear();

	
    //SatterlundCoef(2)=0.3	 !This is my own cloudcorrection function! DG 9/10-97

	NE *pNumBruntIn;
	pNumBruntIn = new NE(&NumBruntIn,3,3,3,3,3);
	Define(pNumBruntIn, "Number of Brunt In");
	initv.resize(NumBruntIn);
	initv[0]=0.605; initv[1]=0.048;initv[2]=0.3;
	pPs= new Ps(&BruntCoef_1, datatype::SINGLE, elements::RADIATION, fysprocess::EMISSION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
		Define(pPs, "BruntCoef_1",0.605,UNIT_TYPES::NO_UNIT,"RadiationInput  > 0|InLongRad=2",0.5,0.7);
	pPs= new Ps(&BruntCoef_2, datatype::SINGLE, elements::RADIATION, fysprocess::EMISSION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
		Define(pPs, "BruntCoef_2",0.048,UNIT_TYPES::NO_UNIT,"RadiationInput  > 0|InLongRad=2",0.04,0.056);
	pPs= new Ps(&BruntCoef_3, datatype::SINGLE, elements::RADIATION, fysprocess::EMISSION, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
		Define(pPs, "BruntCoef_3",0.3,UNIT_TYPES::NO_UNIT,"RadiationInput  > 0|InLongRad=2",0.2,0.4);
		mTab_BruntInLong.push_back(pP);
	functorprop.FuncName = MyFunc::RADLONGIN_BRUNT_EMISSIVITY; functorprop.Arg = MyArgType::D3;
	funcname = "Emissivity Long Wave Radiation (Brunt)"; functorprop.FunctorInitOption = std::string::npos;
	pFunc = new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname, ps_vec);
	functorprop.FuncName = MyFunc::RADLONGIN_BRUNT; functorprop.Arg = MyArgType::D3;
	funcname = "Incoming Long Wave Radiation (Brunt)"; functorprop.FunctorInitOption = std::string::npos;
	pFunc = new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname,  ps_vec);	 ps_vec.clear();

	Tab *pTab;
	pTab= new Tab(&mTab_Beer, datatype::PLANT, elements::RADIATION, fysprocess::ADSORPTION, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pTab, "Beers law - multiple plants","RntLAIv","Evaporation > 0|PlantType>=3");


	G* pG;
	initv.assign(p_Plant->NumPlants,0.);
	pG = new G(&MC_RntDist,pNumPlants, datatype::PLANT, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);   
    Define(pG,pNumPlants,  "Net radiation Canopy" ,initv,	UNIT_TYPES::NO_UNIT,"PlantType=3");
	pG = new G(&MC_LAIAbove,pNumPlants, datatype::PLANT, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL); 
    Define(pG, pNumPlants,  "LAI Above canopy" ,initv,	UNIT_TYPES::NO_UNIT,"PlantType=3");
	pG = new G(&MC_TempCanopy,pNumPlants, datatype::PLANT, elements::RADIATION, fysprocess::EMISSION, bioprocess::NOBPROC, userlevel::NORMAL); 
    Define(pG, pNumPlants,  "MC Temp Canopy" ,initv,	UNIT_TYPES::NO_UNIT,"PlantType=3");


	Gs *pGs;
	pGs= new Gs(&RadNetGround, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);

    Define(pGs,  "RadNetGround",0.,UNIT_TYPES::HEATFLOW_UNIT,"SoilEvap > 0|PlantType >= 2|Evaporation > 0");
	pGs= new Gs(&RadNetShort, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "RadNetShort",0.,UNIT_TYPES::HEATFLOW_UNIT,"SoilEvap > 0|Evaporation > 0");
	pGs= new Gs(&AlbedoVar, datatype::SINGLE, elements::RADIATION, fysprocess::REFLECTION, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "AlbedoVar",0.,"%","RadiationInput > 0");
	pGs= new Gs(&CanopyFracRad, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "CanopyFracRad",0.,UNIT_TYPES::NO_UNIT,"RadiationInput > 0");
	pGs= new Gs(&RadInLongGround, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "RadInLongGround",0.,UNIT_TYPES::HEATFLOW_UNIT,"SoilEvap > 0|Evaporation > 0|LongRadCanopy = 1");
//    ! AUXILIARY VARIABLES FOR PARTITIONING OF RADIATION AT GROUND SURFACE ON IRRIGATED AND NON-IRRIGATED
	pGs= new Gs(&RadNetGround1, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "RadNetGround1",0.,UNIT_TYPES::HEATFLOW_UNIT,"SoilEvap > 0|PlantType >= 2|Evaporation > 0|SoilPartitioningArea>1");
	pGs= new Gs(&RadNetGround2, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "RadNetGround2",0.,UNIT_TYPES::HEATFLOW_UNIT,"SoilEvap > 0|PlantType >= 2|Evaporation > 0|SoilPartitioningArea>1");

	pGs= new Gs(&CanopyFracRad1, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "CanopyFracRad1",0.,UNIT_TYPES::NO_UNIT,"RadiationInput > 0|SoilPartitioningArea>1");
	pGs= new Gs(&CanopyFracRad2, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "CanopyFracRad2",0.,UNIT_TYPES::NO_UNIT,"RadiationInput > 0|SoilPartitioningArea>1");

	pGs= new Gs(&RadInLongGround1, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "RadInLongGround1",0.,UNIT_TYPES::HEATFLOW_UNIT,"SoilEvap > 0|Evaporation > 0|LongRadCanopy = 1|SoilPartitioningArea>1");
	pGs= new Gs(&RadInLongGround2, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "RadInLongGround2",0.,UNIT_TYPES::HEATFLOW_UNIT,"SoilEvap > 0|Evaporation > 0|LongRadCanopy = 1|SoilPartitioningArea>1");
    
//    ! AUXILIARY NEEDED FOR CALCULATIONS
	pGs= new Gs(&CanopyFracRadDirect, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "CanopyFracRadDirect",0.,UNIT_TYPES::NO_UNIT,"RadiationInput > 0|PlantType > 2|CanopyShade>0|LongRadCanopy = 1");
	pGs= new Gs(&CanopyFracRadDiffuse, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "CanopyFracRadDiffuse",0.,UNIT_TYPES::NO_UNIT,"RadiationInput > 0|PlantType > 2|CanopyShade>0|LongRadCanopy = 1");
	pGs= new Gs(&CanopyFracRadLong, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "CanopyFracRadLong",0.,UNIT_TYPES::NO_UNIT,"RadiationInput > 0|PlantType > 2|CanopyShade>0|LongRadCanopy = 1");
	pGs= new Gs(&RadInDirect, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "RadInDirect",0.,UNIT_TYPES::HEATFLOW_UNIT,"RadiationInput > 0|SplitGlobal>0|LongRadCanopy = 1");
	pGs= new Gs(&RadInDiffuse, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "RadInDiffuse",0.,UNIT_TYPES::HEATFLOW_UNIT,"RadiationInput > 0|SplitGlobal>0|LongRadCanopy = 1");

//    ! AUXILIARY NEEDED FOR SNOWMIP2I
	pGs= new Gs(&RadNetCanopy, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "RadNetCanopy",0.,UNIT_TYPES::HEATFLOW_UNIT,"RadiationInput > 0|PlantType > 0|SoilEvap > 0|Evaporation > 0|CanopyShade>0");
	pGs= new Gs(&RadNetLongCanopy, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "RadNetLongCanopy",0.,UNIT_TYPES::HEATFLOW_UNIT,"RadiationInput > 0|PlantType > 2|SoilEvap > 0|Evaporation > 0|CanopyShade>0");
	pGs= new Gs(&RadNetLongGround, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "RadNetLongGround",0.,UNIT_TYPES::HEATFLOW_UNIT,"RadiationInput > 0|PlantType > 2|SoilEvap > 0|Evaporation > 0|CanopyShade>0");
	pGs= new Gs(&RadLongOutVar, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "RadLongOut",0.,UNIT_TYPES::HEATFLOW_UNIT,"RadiationInput > 0|SoilEvap > 0|Evaporation > 0");
	pGs= new Gs(&RadNetShortCanopy, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "RadNetShortCanopy",0.,UNIT_TYPES::HEATFLOW_UNIT,"RadiationInput > 0|PlantType > 2|SoilEvap > 0|Evaporation > 0|CanopyShade>0");
	pGs= new Gs(&RadInShortGround, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "RadInShortGround",0.,UNIT_TYPES::HEATFLOW_UNIT,"RadiationInput > 0|PlantType > 2|SoilEvap > 0|Evaporation > 0|CanopyShade>0");
	pGs= new Gs(&RadNetShortGround, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "RadNetShortGround",0.,UNIT_TYPES::HEATFLOW_UNIT,"RadiationInput > 0|PlantType > 2|SoilEvap > 0|Evaporation > 0|CanopyShade>0");
	pGs= new Gs(&RadShortOut, datatype::SINGLE, elements::RADIATION, fysprocess::EBAL_PARTITIONING, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "RadShortOut",0.,UNIT_TYPES::HEATFLOW_UNIT,"RadiationInput > 0|SoilEvap > 0|Evaporation > 0");

	pGs= new Gs(&SnowAlbedoVar, datatype::SINGLE, elements::RADIATION, fysprocess::REFLECTION, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "Snow Albedo",0.,UNIT_TYPES::HEATFLOW_UNIT,"SoilEvap > 0|Evaporation > 0|SnowPack >0");
	pGs= new Gs(&SoilAlbedoVar, datatype::SINGLE, elements::RADIATION, fysprocess::REFLECTION, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "Soil Albedo",0.,UNIT_TYPES::HEATFLOW_UNIT,"SoilEvap > 0|Evaporation > 0|SnowPack >0");
	pGs= new Gs(&GroundAlbedoVar, datatype::SINGLE, elements::RADIATION, fysprocess::REFLECTION, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pGs,  "Ground Albedo",0.,UNIT_TYPES::HEATFLOW_UNIT,"SoilEvap > 0|Evaporation > 0|SnowPack >0");


	return FunctionDef();

	//pSw= new Sw(&SnowSurEbal, datatype::SINGLE, elements::SNOW, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
}
bool Radiation_Functions::FunctionDef() {
	Func *pFunc;


	
		f_daylength = Physf(FUNC_CHOICE::DAYLENGTH, Latitude, AltSimPosition, RadFracAng1, RadFracAng2);
		f_potrisday = Physf(FUNC_CHOICE::POTRISDAILY, Latitude, AltSimPosition, RadFracAng1, RadFracAng2);
/*		double (PhysFunctions::*fpointfys1)(double);
		fpointfys1=&PhysFunctions::DayLengthFunction;
		pFunc= new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pFunc, m_fPhyspointer0_1.size(),100,this, "Day Length During Year Function"); m_fPhyspointer0_1.push_back(fpointfys1);
		//res=pFunc->GetValue(1.);


		auto yy = f_daylength(269);
		fpointfys1=&PhysFunctions::PotRisDaily;
		pFunc= new Func( datatype::SINGLE, elements::RADIATION, fysprocess::NOFPROC, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pFunc, m_fPhyspointer0_1.size(),100,this, "Potential Global Radiation Function"); m_fPhyspointer0_1.push_back(fpointfys1);
		//res=pFunc->GetValue(1.);*/




	return true;
}
