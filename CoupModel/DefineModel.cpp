#include "..\\std.h"
#include "DefineModel.h"
#include "..\Document\Doc.h"
#include "..\SoilData\GetSoilStorage.h"
#include "..\model\HBV\HBV.h"
#include "..\model\Plant\PlantModel.h"
#include "..\model\Plant\WaterUptake.h"
#include "..\model\Soil\SoilModel.h"
#include "..\Model\Atmosphere\AtmModel.h"
#include "..\model\Atmosphere\Irrigation.h"
#include "..\model\Structure\StructureModel.h"
#include "..\model\Structure\Structure.h"
#include "..\Simulator\ModelInfo.h"
#include "..\Model\SOIL_NC\SoilOrgModel.h"
#include "..\Model\PLANT_NC\PlantOrgModel.h"

DefineModel::DefineModel (){

p_ModelInfo = nullptr;
p_ModelMap = nullptr;
p_ModelStructure = nullptr;
p_ModelInfo = new ModelInfo();
p_ModelMap = new ModelMap();
p_ModelStructure = new Structure(p_ModelInfo, p_ModelMap);

SoilModel soilmodel;
AtmModel atmmodel;
PlantModel plantmodel;
StructureModel structuremodel;
PlantOrgModel plantorgmodel;
SoilOrgModel soilorgmodel;

p_ModelInfo->AddModel("Soil Model", &soilmodel);
p_ModelInfo->AddModel("Plant Model", &plantmodel);
p_ModelInfo->AddModel("Atm Model", &atmmodel);
p_ModelInfo->AddModel("Structure Model", &structuremodel);
p_ModelInfo->AddModel("Plant Org Model", &plantorgmodel);
p_ModelInfo->AddModel("Soil Org Model", &soilorgmodel);


auto pHBV = make_unique<HBV>(p_ModelInfo, p_ModelMap, p_ModelStructure);

auto p_Irrig = make_shared<Irrigation>(p_ModelInfo, p_ModelMap, p_ModelStructure);
auto p_SoilFrost = make_unique<Soil_Frost>(p_ModelInfo, p_ModelMap, p_ModelStructure);
auto p_Plant = make_shared<WaterUptake>(p_ModelInfo, p_ModelMap, p_ModelStructure);
auto p_Additional = make_shared<Additional_BioticVariables>(p_ModelInfo, p_ModelMap, p_ModelStructure);
auto p_SoilOrg = make_unique< NC_SoilManagement>(p_ModelInfo, p_ModelMap, p_ModelStructure);
auto p_PlantOrg = make_unique< NC_Plant>(p_ModelInfo, p_ModelMap, p_ModelStructure);
auto p_NC = make_unique<NC_Drivings>(p_ModelInfo, p_ModelMap, p_ModelStructure);

p_SoilFrost->Def();
p_Plant->Def();
p_Irrig->Def();
p_PlantOrg->Def();
p_NC->Def();
p_SoilOrg->Def();
pHBV->Def();
p_ModelStructure->Def();
p_Additional->Def();

};
DefineModel::~DefineModel() {
	auto koll = p_ModelInfo;
	if (koll != nullptr)		delete koll;
	auto koll1 = p_ModelMap;
	if (koll1 != nullptr)		delete koll1;
	auto koll2 = p_ModelStructure;
	if (koll2 != nullptr)		delete koll2;


};