#include "Additional_BioticVariables_Functions.h"
#include "../PLANT_NC/PlantOrgModel.h"
#include "../PLANT_NC/NC_Plant.h"

Additional_BioticVariables_Functions::Additional_BioticVariables_Functions(void)
{


}
Additional_BioticVariables_Functions::~Additional_BioticVariables_Functions(void)
{
}


bool Additional_BioticVariables_Functions::Def()

{
	Additional_Variables::Def();
	ModuleNo=31;
	PlantOrgModel* pPlantOrg=(PlantOrgModel*)m_pModelInfo->GetModelPtr("Plant Org Model");
	NC_Plant *pNCPlant;
	pNCPlant=&pPlantOrg->m_NC_Plant;
	Xs *pXs; Ts *pTs; Gs *pGs;
	
	pXs= new Xs(&AccFertilization, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,	"AccFertilization",		 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs= new Xs(&AccManure, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "AccNitrogenManure"	   	, 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs = new Xs(&AccMinNDeepPercolation, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "AccMinNDeepPercolation", 0., UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0|DeepPercolationInput>0");
	pXs= new Xs(&AccExternalCarbon, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  "AccExternalCarbon"	   	, 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");

	pXs = new Xs(&Acc_P_Fertilization, datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "Acc_P_Fertilization", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus> 0|N Fertilization >0");
	pXs = new Xs(&Acc_P_Manure, datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "Acc_P_Manure", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus> 0|N Fertilization >0");
	pXs = new Xs(&Acc_P_Weathering, datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "Acc_P_Weathering", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus> 0");

	pXs = new Xs(&pNCPlant->AccTotPlantUptake[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "AccTotNPlantUptake", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0|Growth>1");
	pXs = new Xs(&pNCPlant->AccTotPlantUptake[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "AccTot_P_PlantUptake", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus> 0|Growth>1");



	pXs= new Xs(&pNCPlant->AccAboveLitter[_C_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,   "AccAboveLitterCarbon"	   	, 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs = new Xs(&pNCPlant->AccAboveLitter[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "AccAboveLitterN", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
	pXs = new Xs(&pNCPlant->AccAboveLitter[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "AccAboveLitterP", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus> 0");


	pXs = new Xs(&AccSoilFormation[_C_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "AccCarbonSoilFormation", 0., UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
	pXs = new Xs(&AccSoilFormation[_N_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "AccNitrogenSoilFormation", 0., UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
	pXs = new Xs(&AccSoilFormation[_P_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "Acc_P_SoilFormation", 0., UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0|Phosphorus> 0");
		
		
	pXs= new Xs(&pNCPlant->AccBelowLitter[_C_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,    "AccBelowLitterCarbon"	   	, 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs= new Xs(&pNCPlant->AccBelowLitter[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  	    "AccBelowLitterN"	   	, 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs = new Xs(&pNCPlant->AccBelowLitter[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "AccBelowLitterP", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus> 0");
	
	pXs= new Xs(&pNCPlant->AccPlantLitter[_N_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,   "AccPlantLitterN"	   	, 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs= new Xs(&pNCPlant->AccPlantLitter[_C_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  	    "AccPlantLitterC"	   	, 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");	
	pXs = new Xs(&pNCPlant->AccPlantLitter[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "AccPlantLitterP", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus> 0");
		
		
		
	pXs= new Xs(&AccNInput, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  	"AccNitrogenInput"			, 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs= new Xs(&AccNOutput, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  		"AccNitrogenOutput"			, 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");

	pXs= new Xs(&AccDeposition, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  		"AccDeposition"			, 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs = new Xs(&Acc_P_Deposition, datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "Acc_P_Deposition", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus >0|Nitrogen and Carbon > 0");
	pXs = new Xs(&Acc_P_Mineralization, datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "Acc_P_Mineralization", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus >0|Nitrogen and Carbon > 0");




	pXs= new Xs(&AccNFixation, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  		"AccNFixation"			, 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");

	pXs= new Xs(&pNCPlant->AccHarvest[_N_]	, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  		"AccNitrogenHarvest"  	, 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs = new Xs(&pNCPlant->AccHarvest[_C_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "AccCarbonHarvest", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
	pXs = new Xs(&pNCPlant->AccHarvest[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "Acc_P_Harvest", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus > 0");
	pXs = new Xs(&pNCPlant->AccSeedInputs[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "AccNitrogenSeedInputs", 0., UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
	pXs = new Xs(&pNCPlant->AccSeedInputs[_C_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "AccCarbonSeedInputs", 0., UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
	pXs = new Xs(&pNCPlant->AccSeedInputs[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "Acc_P_SeedInputs", 0., UNIT_TYPES::MASS_UNIT, "Phosphorus > 0");

	pXs= new Xs(&AccDenitrification, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  	"AccDenitrification"	, 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs= new Xs(&AccMinLeaching[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  "AccMinNLeaching"			, 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs = new Xs(&AccMinLeaching[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "AccMin_P_Leaching", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus > 0");
	pXs = new Xs(&AccSurfaceRunOffLeaching[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "Acc_P_SurfaceRunoff", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus >0");

		
	pXs= new Xs(&AccDisLeaching[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  		"AccDisNLeaching"		, 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs= new Xs(&AccDisLeaching[_C_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  		"AccDisCLeaching"		, 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs = new Xs(&AccDisLeaching[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "AccDis_P_Leaching", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus > 0");


	pXs= new Xs(&pNCPlant->TotalDiffPlant[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  		"TotalDiffNPlant"		 , 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs= new Xs(&pNCPlant->TotalDiffPlant[_C_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  		"TotalDiffCPlant"		 , 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs = new Xs(&pNCPlant->TotalDiffPlant[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiff_P_Plant", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus > 0");

	pXs = new Xs(&TotalDiffSoil[_C_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiffCSoil", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
	pXs= new Xs(&TotalDiffSoil[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
	    Define(pXs, "TotalDiffNSoil"		 , 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs = new Xs(&TotalDiffSoil[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiff_P_Soil", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus > 0");
	
	pXs = new Xs(&TotalDiffOrgSoil[_C_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiffOrgCSoil", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
	pXs= new Xs(&TotalDiffOrgSoil[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  	"TotalDiffOrgNSoil"		 , 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs = new Xs(&TotalDiffOrgSoil[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiffOrg_P_Soil", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus > 0");
	
	pXs = new Xs(&TotalDiffHumus[_C_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiffCHumus", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
	pXs= new Xs(&TotalDiffHumus[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  "TotalDiffNHumus"		 , 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs = new Xs(&TotalDiffHumus[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiff_P_Humus", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus > 0");

		pXs = new Xs(&TotalDiffLitter1[_C_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiffCLitter1", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
		pXs = new Xs(&TotalDiffLitter1[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiffNLitter1", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
		pXs = new Xs(&TotalDiffLitter1[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiff_P_Litter1", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus > 0");

		pXs = new Xs(&TotalDiffLitter2[_C_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiffCLitter2", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
		pXs = new Xs(&TotalDiffLitter2[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiffNLitter2", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
		pXs = new Xs(&TotalDiffLitter2[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiff_P_Litter2", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus > 0");

	pXs= new Xs(&TotalDiffMinSoil[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  	"TotalDiffMinNSoil"		 , 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs = new Xs(&TotalDiffMinSoil[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiffMin_P_Soil", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus > 0");

	pXs = new Xs(&TotalDiffSolidMineralPhosphorus, datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiffSolidPMinerals", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus > 0");

	pXs = new Xs(&Soil_BalanceCheck[_C_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "Soil C BalanceCheck", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
	pXs = new Xs(&Soil_BalanceCheck[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "Soil N BalanceCheck", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
	pXs = new Xs(&Soil_BalanceCheck[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "Soil P BalanceCheck", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus > 0");

	pXs = new Xs(&TotalBalanceCheck[_C_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "CarbonBalanceCheck", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
	pXs= new Xs(&TotalBalanceCheck[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  "NitrogenBalanceCheck"	 , 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs = new Xs(&TotalBalanceCheck[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "PhosphorusBalanceCheck", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus > 0");

	pXs = new Xs(&pNCPlant->Plant_BalanceCheck[_C_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "Plant C BalanceCheck", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
	pXs= new Xs(&pNCPlant->Plant_BalanceCheck[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  "Plant N BalanceCheck"	 , 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs = new Xs(&pNCPlant->Plant_BalanceCheck[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "Plant P BalanceCheck", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus> 0");

	pXs = new Xs(&TotalDiff_Eco[_C_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiff C Eco", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
	pXs= new Xs(&TotalDiff_Eco[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
	    Define(pXs,  	"TotalDiff N Eco"		 , 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs = new Xs(&TotalDiff_Eco[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiff P Eco", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus> 0");

		pXs = new Xs(&TotalDiff_Fungi[_C_], datatype::ACCUMULATOR, elements::CARBON, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiff C Fungi", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
		pXs = new Xs(&TotalDiff_Fungi[_N_], datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiff N Fungi", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0");
		pXs = new Xs(&TotalDiff_Fungi[_P_], datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "TotalDiff P Fungi", 0.,UNIT_TYPES::MASS_UNIT, "Phosphorus> 0");




	pTs= new Ts(&NitrogenBalanceRate, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pTs, 	"NitrogenBalanceRate"	 , 0.,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon > 0");
 	pTs= new Ts(&N_SupplyRate, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pTs, 	"N InputRate"	 , 0.,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon > 0");
 	pTs= new Ts(&N_LossesRate, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pTs, 	"N OutputRate"	 , 0.,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon > 0");

	pTs= new Ts(&CarbonBalanceRate, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
 		Define(pTs, 	"CarbonBalanceRate"	 , 0.,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon > 0");
	pTs= new Ts(&CarbonFlux, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
 		Define(pTs, 	"Carbon Flux"	 , 0.,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon > 0");
	pTs= new Ts(&SoilRespOrg, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
 		Define(pTs, 	"Soil Resp (no roots)"	 , 0.,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon > 0");
	pTs= new Ts(&SoilRespTot, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
 		Define(pTs, 	"Soil Respiration", 0.,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon > 0");
	pTs= new Ts(&pNCPlant->TotalPhotoSynt, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
 		Define(pTs, 	"Total PhotoSynt", 0.,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon > 0");
	pTs= new Ts(&TotalRespiration, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
 		Define(pTs, 	"Total Respiration", 0.,UNIT_TYPES::MASSFLOW_UNIT,"Nitrogen and Carbon > 0");
	pXs= new Xs(&pNCPlant->AccPhotosyntheses, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  	"AccPhotosyntheses"		 , 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs= new Xs(&AccTotalRespiration, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  	"AccTotalRespiration"	 , 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs= new Xs(&pNCPlant->AccPlantRespiration, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  	"AccPlantRespiration"	 , 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs= new Xs(&AccSoilRespiration, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  	"AccSoilRespiration"	 , 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs = new Xs(&pNCPlant->AccFungiRespiration, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "AccFungiRespiration", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0|Mycorrhiza Fungal Growth>0");
	pXs = new Xs(&pNCPlant->AccFungiGrowth, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
	Define(pXs, "AccFungiGrowth", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0|Mycorrhiza Fungal Growth>0");
		pXs = new Xs(&pNCPlant->AccFungiLitterFall, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs, "AccFungiLitterFall", 0.,UNIT_TYPES::MASS_UNIT, "Nitrogen and Carbon > 0|Mycorrhiza Fungal Growth>0");
	pXs= new Xs(&AccSoilOrgRespiration, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  	"AccSoilOrgRespiration"	 , 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs= new Xs(&AccCarbonBalance, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  	"AccCarbonBalance"		 , 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");
	pXs= new Xs(&AccNitrogenBalance, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  	"AccNitrogenBalance"		 , 0.,UNIT_TYPES::MASS_UNIT,"Nitrogen and Carbon > 0");

	pXs= new Xs(&AccTotAdsRadiation, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pXs,  	"AccTotAdsRadiation"	 , 0.,"MJ/m2","Nitrogen and Carbon > 0|Growth>1");
	pGs= new Gs(&AccRadEfficiency, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
 		Define(pGs, "AccRadEfficiency",0.0,"MJ/gC","Nitrogen and Carbon >0|Growth >=3");
	pGs= new Gs(&AccWaterEfficiency, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
 		Define(pGs, "AccWaterEfficiency",0.0,"mm/gC","Nitrogen and Carbon >0|Growth >=3");
	pGs= new Gs(&AccNitrogenEfficiency, datatype::ACCUMULATOR, elements::NITROGEN, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
 		Define(pGs, "AccNEfficiency",0.0,"gN/gC","Nitrogen and Carbon >0|Growth >=3");
	pGs = new Gs(&AccPhosphorusEfficiency, datatype::ACCUMULATOR, elements::PHOSPHORUS, fysprocess::STORAGE, bioprocess::NOBPROC, userlevel::NORMAL);
		Define(pGs, "Acc_P_Efficiency", 0.0, "gP/gC", "Phosphorus >0|Growth >=3");


	pGs = new Gs(&GWP_NEE[0], datatype::ATMBOUNDARY, elements::GHG, fysprocess::EMISSION, bioprocess::LIVING, userlevel::NORMAL);
	 Define(pGs, "GWP Eco - CO2", 0.0, "MgCO2e/ha/yr", "Nitrogen and Carbon > 0");
	pGs = new Gs(&GWP_NEE[1], datatype::ATMBOUNDARY, elements::GHG, fysprocess::EMISSION, bioprocess::LIVING, userlevel::NORMAL);
	 Define(pGs, "GWP - N2O", 0.0, "MgCO2e/ha/yr", "Nitrogen and Carbon > 0");
	pGs = new Gs(&GWP_NEE[2], datatype::ATMBOUNDARY, elements::GHG, fysprocess::EMISSION, bioprocess::LIVING, userlevel::NORMAL);
	 Define(pGs, "GWP - CH4", 0.0, "MgCO2e/ha/yr", "Nitrogen and Carbon > 0");
	pGs = new Gs(&GWP_NEE[3], datatype::ATMBOUNDARY, elements::GHG, fysprocess::EMISSION, bioprocess::LIVING, userlevel::NORMAL);
	 Define(pGs, "GWP Eco - All", 0.0, "MgCO2e/ha/yr", "Nitrogen and Carbon > 0");

	pGs = new Gs(&GWP_SOIL[0], datatype::ATMBOUNDARY, elements::GHG, fysprocess::EMISSION, bioprocess::LIVING, userlevel::NORMAL);
	 Define(pGs, "GWP Soil - CO2", 0.0, "MgCO2e/ha/yr", "Nitrogen and Carbon > 0");
	pGs = new Gs(&GWP_SOIL[1], datatype::ATMBOUNDARY, elements::GHG, fysprocess::EMISSION, bioprocess::LIVING, userlevel::NORMAL);
	 Define(pGs, "GWP Soil  - All", 0.0, "MgCO2e/ha/yr", "Nitrogen and Carbon > 0");
	
	 pGs = new Gs(&GWP_PLANT, datatype::ATMBOUNDARY, elements::GHG, fysprocess::EMISSION, bioprocess::LIVING, userlevel::NORMAL);
	 Define(pGs, "GWP Plant - CO2", 0.0, "MgCO2e/ha/yr", "Nitrogen and Carbon > 0");



return true;
}



