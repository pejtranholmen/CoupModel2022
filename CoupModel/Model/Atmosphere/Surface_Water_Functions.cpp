#include "./Surface_Water_Functions.h"
Surface_Water_Functions::Surface_Water_Functions(void)
{
}
Surface_Water_Functions::~Surface_Water_Functions(void)
{
}



bool Surface_Water_Functions::Def()
{

	SoilEvaporation::Def();

	ModuleNo=19;
	ModuleConditions="WaterEq>0";

//    ! Snow surface energy balance
	Sw* pSw;
	pSw= new Sw(&Furrow, datatype::SURFACEPOOL, elements::WATER, fysprocess::INFILTRATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pSw,"Furrow",0,"Off|Irrigation|I.+Precipitation","WaterEq = 1") ; 
	pSw= new Sw(&RunOnInput, datatype::SURFACEPOOL, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pSw,  "RunOnInput",0,"Off|Lateral Input File","WaterEq = 1");  
	pSw= new Sw(&PumpStation, datatype::SURFACEPOOL, elements::WATER, fysprocess::INFILTRATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pSw,"PumpStation",0,"Off|On","WaterEq = 1");  

	Ps* pPs;
	Func *pFunc;
	string funcname;

	FunctorProp functorprop;
	vector<Ps*> ps_vec;
	ps_vec.clear(); 

	PLOT_PARAM plotpar;
	plotpar.Num_Of_Curves = 1;
	plotpar.Num_Of_Points = 100;
	plotpar.Num_Of_X = 1;
	plotpar.X_Min = 0;
	plotpar.X_Max = 50;
	plotpar.X_Label = "Surface Pool "; plotpar.X_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::WATERMASS_UNIT, true);
	plotpar.Y_Label = "Water Flow Rate "; plotpar.Y_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::WATERFLOW_UNIT, true);
	plotpar.LogXScale = false;
	plotpar.LogYScale = false;
	plotpar.Num_Of_Opt = 1;

	functorprop.FuncName = MyFunc::SURFACE_RUNOFF; functorprop.Arg = MyArgType::D1;
	functorprop.FunctorInitOption = std::string::npos;
	funcname = "Surface Pool Runoff Function";

	pPs = new Ps(&SurfCoef, datatype::SURFACEPOOL, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
	Define(pPs,"SurfCoef", 0.8,"-","WaterEq = 1",0., 1000.,0.,10.);
	pPs= new Ps(&SurfPoolMax , datatype::SURFACEPOOL, elements::WATER, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs,  "SurfPoolMax", 0.,"mm","WaterEq = 1",0., 200.); ps_vec.push_back(pPs); 

	pFunc = new Func( datatype::SURFACEPOOL, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname,  ps_vec, plotpar); ps_vec.clear();




	pPs= new Ps(&SurfPoolInit , datatype::SURFACEPOOL, elements::WATER, fysprocess::INITF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs, "SurfPoolInit", 0.,"mm","WaterEq = 1",0., 100000.);

	functorprop.FuncName = MyFunc::SURFACE_WATER_COVER; functorprop.Arg = MyArgType::D1;
	functorprop.FunctorInitOption = std::string::npos;
	funcname = "Surface Pool Cover Function";
	plotpar.X_Label = "Surface Pool "; plotpar.X_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::WATERMASS_UNIT, true);
	plotpar.Y_Label = "Degree of Soil Cover "; plotpar.Y_Label += p_ModelInfo->GetUnitString(UNIT_TYPES::NO_UNIT, true);


	pPs= new Ps(&SPCovMax , datatype::SURFACEPOOL, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
	Define(pPs, "SP Max cover", 1.,"-","WaterEq = 1",0.001, 1.);
	pPs = new Ps(&SPCovTot, datatype::SURFACEPOOL, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs);
	Define(pPs, "SPCoverTotal", 50., "mm", "WaterEq = 1", 0.001, 1000.);
	pPs= new Ps(&SPCovPot, datatype::SURFACEPOOL, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL); ps_vec.push_back(pPs); 
	Define(pPs,  "SPCovPot", 1.,"-","WaterEq = 1",0.001, 10.);
	

	pFunc = new Func( datatype::SURFACEPOOL, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pFunc, functorprop, funcname,  ps_vec, plotpar); ps_vec.clear(); 


	pPs= new Ps(&InfFurrow, datatype::SURFACEPOOL, elements::WATER, fysprocess::INFILTRATION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs, "InfFurrow", 0.,"-","Furrow > 0", 0., 1.);
	pPs= new Ps(&SoilCover , datatype::SURFACEPOOL, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs, "SoilCover", 0., "-","WaterEq = 1", 0., 1.);


	pPs= new Ps(&PumpRate , datatype::WATERPOND, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs,  "PumpRate", 40., "mm/day","WaterEq = 1|PumpStation >0", 0., 1000.);
	pPs= new Ps(&PumpLevelStart , datatype::WATERPOND, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pPs, "PumpLevelStart", 20., "mm","WaterEq = 1|PumpStation >0", 0., 1000.);
	pPs= new Ps(&PumpLevelStop , datatype::WATERPOND, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pPs,  "PumpLevelStop", 2., "mm","WaterEq = 1|PumpStation >0", 0., 10.);
	pPs= new Ps(&PumpPartitioning , datatype::WATERPOND, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
    Define(pPs, "PumpPartitioning", 0.5, "mm","WaterEq = 1|PumpStation >0", 0., 1.);

	Xs *pXs;
	pXs= new Xs(&SurfacePool , datatype::SURFACEPOOL, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);


	Define(pXs, "SurfacePool", 0.,"mm","WaterEq = 1");
	pXs= new Xs(&PumpPool , datatype::WATERPOND, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pXs, "PumpPool", 0.,"mm","WaterEq = 1|PumpStation>0");

	Ts *pTs;
	pTs= new Ts(&Spoolinflow, datatype::SURFACEPOOL, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);

	Define(pTs, "Spoolinflow",0.,UNIT_TYPES::WATERFLOW_UNIT,"WaterEq = 1");
	pTs= new Ts(&SpoolRunoff, datatype::SURFACEPOOL, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pTs,  "SpoolRunoff",0.,UNIT_TYPES::WATERFLOW_UNIT,"WaterEq = 1");
	pTs= new Ts(&FurrowInfil, datatype::SURFACEPOOL, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);	
	Define(pTs, "FurrowInfil",0.,UNIT_TYPES::WATERFLOW_UNIT,"Furrow >= 1");
	pTs= new Ts(&FurrowPrec, datatype::SURFACEPOOL, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pTs, "FurrowPrec",0.,UNIT_TYPES::WATERFLOW_UNIT,"Furrow >= 2");
	
	pTs= new Ts(&PumpInFlow, datatype::WATERPOND, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);	
	Define(pTs, "PumpInFlow",0.,UNIT_TYPES::WATERFLOW_UNIT,"WaterEq = 1|PumpStation>0");
	pTs= new Ts(&PumpOutFlow, datatype::WATERPOND, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);	
	Define(pTs, "PumpOutFlow",0.,UNIT_TYPES::WATERFLOW_UNIT,"WaterEq = 1|PumpStation>0");
	pTs= new Ts(&SpoolDelayRunoff, datatype::WATERPOND, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pTs, "SpoolDelayRunoff",0.,UNIT_TYPES::WATERFLOW_UNIT,"WaterEq = 1|PumpStation>0");

	pTs= new Ts(&SpoolSoilInfil, datatype::WATERPOND, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pTs,"SpoolSoilInfil",0.0,UNIT_TYPES::WATERFLOW_UNIT,"WaterEq = 1");
	pTs= new Ts(&SoilInfil, datatype::WATERPOND, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pTs, "SoilInfil",0.,UNIT_TYPES::WATERFLOW_UNIT,"WaterEq = 1");

	Gs *pGs;
	pGs= new Gs(&SpoolCover, datatype::SURFACEPOOL, elements::WATER, fysprocess::SURFACERUNOFF, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pGs,  "SpoolCover", 0., "-","WaterEq = 1");


	return true;

	//pSw= new Sw(&SnowSurEbal, datatype::SINGLE, elements::SNOW, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);

}




