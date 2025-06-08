#include "Soil_HeatPump_Functions.h"

Soil_HeatPump_Functions::Soil_HeatPump_Functions(void)
{
}
Soil_HeatPump_Functions::~Soil_HeatPump_Functions(void)
{
}
bool Soil_HeatPump_Functions::Def()
{
	Soil_HeatF::Def();
	
	ModuleName="Soil Heat Pump";
	ModuleNo=17;
	ModuleConditions="HeatPump > 0";

	Ts *pTs= new Ts(&HeatPumpFlow, datatype::SINGLE, elements::HEAT, fysprocess::EXTRACTION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pTs, "HeatPumpFlow", 0.,UNIT_TYPES::HEATFLOW_UNIT,"Heat Pump >= 1");
	
	Ps *pPs= new Ps(&HPBase, datatype::SINGLE, elements::HEAT, fysprocess::EXTRACTION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs, "HPBase", 0.,UNIT_TYPES::HEATFLOW_UNIT ,	"Heat Pump = 1",-1.E7,1.E7);
	pPs= new Ps(&HPAmp, datatype::SINGLE, elements::HEAT, fysprocess::EXTRACTION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs,  "HPAmp ", 1.E5,"J/m2/day/C","Heat Pump = 1",0., 1.E6);
	pPs= new Ps(&HPCut, datatype::SINGLE, elements::HEAT, fysprocess::EXTRACTION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs,  "HPCut", -5.,UNIT_TYPES::TEMP_UNIT,			"Heat Pump = 1", -50., 20.);
	pPs= new Ps(&HPLayer, datatype::SINGLE, elements::HEAT, fysprocess::EXTRACTION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs, "HPLayer", 4.,"#",		 "Heat Pump >= 1", 0., 100.);
	pPs= new Ps(&HPMax  , datatype::SINGLE, elements::HEAT, fysprocess::EXTRACTION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs, "HPMax",   -10.,UNIT_TYPES::TEMP_UNIT ,		"Heat Pump = 1",-30., 0.);
	pPs= new Ps(&HPZero  , datatype::SINGLE, elements::HEAT, fysprocess::EXTRACTION, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pPs, "HPZero",   -10.,UNIT_TYPES::TEMP_UNIT ,	"Heat Pump = 1", -100., 10.);

	return true;
}
