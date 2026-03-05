#pragma once
#include "../ModelTypes/SimB.h"
#include <string>
#include <string_view>
#include <vector>
#include <array>	


struct RunStatusOfDocumentInfo
{
	bool StartMode;
	int ViewStatus;
	bool Running;
	bool MultiRunning;
	bool ReadyToSimulate;
	bool NoSingleRunSimFile;

};
class Sim;
class SimB;

 

class CommonModelInfo 
{ 
public:
	CommonModelInfo();
	~CommonModelInfo();
	size_t GetTypeIndex(std::string_view str);
	size_t GetGroupIndex(std::string_view str);

	std::string_view GetGroupCategoryName (size_t index) { if (index<size(m_GroupCategoryNames)) return m_GroupCategoryNames[index]; return "All Modules"; };
	std::string_view GetGroupName(size_t index) noexcept {if(index<size(m_GroupNames)) return m_GroupNames[index];return "";};
	std::string_view GetFysProcName(fysprocess fys_proc) noexcept { auto index = static_cast<size_t>(fys_proc); if(index < m_FysProcessNames.size()) return m_FysProcessNames[index]; else return "";};
	std::string_view GetBioProcName(bioprocess bioproc) noexcept { auto index = static_cast<size_t>(bioproc); if (index < m_BioProcessNames.size()) return m_BioProcessNames[index]; else return "";};
	std::string_view ElementName(elements elem) noexcept;

	void trim_xml(std::string& str);
	void SetCommandLineMode(bool value)  noexcept {m_CommandInfo=value;};
	bool GetCommandLineMode()  noexcept {return m_CommandInfo;};
	bool IsRunning()  noexcept {return m_RunInfo.Running;};
	void SetRunning(bool value)  noexcept {m_RunInfo.Running=value;};
	bool IsMultiRunning()  noexcept {	return m_RunInfo.MultiRunning;	};
	void SetMultiRunning(bool value)  noexcept {	m_RunInfo.MultiRunning=value;};
	bool IsStartMode()  noexcept {return m_RunInfo.StartMode;};
	void SetStartMode(bool value)  noexcept {m_RunInfo.StartMode=value;};
	bool IsReady()  noexcept {return m_RunInfo.ReadyToSimulate;};
	void SetReady(bool value)  noexcept {m_RunInfo.ReadyToSimulate=value;};
	void SetRunDoc(Sim* pSim)  noexcept {m_pRunDoc=pSim;};
	int GetViewStatus()  noexcept {return m_RunInfo.ViewStatus;};
	void SetViewStatus(int value)  noexcept {m_RunInfo.ViewStatus=value;};
	bool GetNoSingleRun()  noexcept {return m_RunInfo.NoSingleRunSimFile;};
	void SetNoSingleRun(bool value) noexcept {m_RunInfo.NoSingleRunSimFile=value;};
	std::string getOrigName(std::string txt) {
		auto it = changed_names.find(txt);
		if (it != changed_names.end())
			return (*it).second;
		else
			return txt;
	}
	void insert_ChangedNames(std::vector<std::pair<std::string, std::string>>);


	std::vector<std::string_view> GetSortedListofElements();


	std::vector<std::string_view> GetSortedListofPhysProc();


	std::vector<std::string_view> GetSortedListofBiolProc();



	std::vector<std::string_view> GetSortedListofGroupNames() { return m_GroupNamesSorted; };

	std::string_view GetGroupName(GROUPNAME group) noexcept;
	std::string_view GetGroupCatName(GROUPCATNAME group) noexcept;
	int GetGroupCategory(std::string_view group) noexcept;
	int GetGroupCategoryNo(size_t groupno) noexcept;
	std::string_view GetVarType(size_t) noexcept;

	std::string_view GetGroupCategoryFromGroup(GROUPNAME group) noexcept {
		GROUPCATNAME cat = GROUPCATNAME::ALL;
		auto it = m_GroupMap.find(group);
		if (it != m_GroupMap.end())
			cat= it->second;		
		return GetGroupCatName(cat);
	};


	std::vector<std::string_view> GetGroupNames() {
		std::vector<std::string_view> out;
		for (std::string_view name : m_GroupNames) {
			out.push_back(name);
		}
		return out;
	}
	size_t GetNumberOfElements()  noexcept { return m_ElementSorted.size(); };
	std::string GetStringOfElement(elements elem);
	elements GetElementFromString(std::string str);
	fysprocess GetPhysProcFromString(std::string str);
	bioprocess GetBioProcFromString(std::string str);

	//std::string_view GetUnitString(UNIT_TYPES) noexcept;
	//std::string_view GetUnitString(UNIT_TYPES key, bool withparanthesis) noexcept;


	size_t GetNumUnits()  noexcept { return m_TypeBasedUnits.size(); };
	std::string_view GetUnitString(UNIT_TYPES key, bool withparanthesis = false) {
		auto it = m_TypeBasedUnits.find(key);
		if (it != m_TypeBasedUnits.end()) {
			if (withparanthesis) {
				return it->second;//" + it->second + ")";
			}
			else {
				return it->second;
			}
		}
		else {
			return "";
		}
	}
	UNIT_TYPES GetUnitType(std::string key) {
		auto it = m_StringBasedUnits.find(key);
		if (it != m_StringBasedUnits.end())
			return (*it).second;
		else
			return UNIT_TYPES::NO_UNIT;
	};

	std::vector<std::string> GetSelectedStringBasedUnits(std::vector<UNIT_TYPES> key) {
		std::vector<std::string> out;
		for_each(key.begin(), key.end(), [&](UNIT_TYPES& key) {
			auto it = m_TypeBasedUnits.find(key);
			std::string str = "";
			str = std::string(it->second);
			out.push_back(str);
			});
		return out;
	};

	Sim *GetRunDoc()  noexcept {return m_pRunDoc;};
	std::vector<SimB*> SortSimB_ByGroup(std::vector<SimB*> v);
	std::vector<SimB*> SortSimB_ByCategory(std::vector<SimB*> v);
	std::map<std::string, std::string> changed_names;


private:
	static constexpr std::array<std::string_view, static_cast<size_t>(elements::COUNT)> ElementNames = {
		"Water", "Heat", "Carbon", "Nitrogen", "Phosphorus", "Radiation",
		"Snow", "Salt", "Methane", "Vapour", "General", "Temperature", 
		"Water Potential","Soil", "Atmosphere", "Air", "Precipitation", "Climate",
		"Organic","Dissolved Carbon", "Dissolved Nitrogen", "Dissolved Phosphorus", "Mineral N", "Mineral P", 
		"Ammonium", "Nitrate","Phosphate", "Mycorrhize", "Denitrifiers", "Nitrifiers",
		"Anarobic gas", "Oxygen", "Manure",	"Plant Shape", "Distance", "CO2 SOIL", 
		"Time", "Area", "Plant Stress", "Wind Speed",		"Global Radiation", "LongWave Radiation",
		"Humidity", "GreenHouseGas", "Plant Biomass", "Nitric Oxide", "Nitrous Oxide","Carbon Oxide Sulphur"
	};

	static constexpr std::array<std::string_view, static_cast<size_t>(fysprocess::COUNT) > m_FysProcessNames = { "Root Water Uptake", "Evapotranspiration", "Transpiration",
	"Soil Evaporation", "Potential Transpiration", "Interception", "Turbulence", "Surface Runoff", "Aerodynamic Resistance",
	"Snow Melting", "Redistribution", "Storage", "Transport", "Infiltration", "Drainage", "Percolation",
	"Runoff", "biotic process", "Initial Flow", "Shrinking", "Physical Shape", "Heat Extraction",
	"Heat Pipe", "Heat Source", "Lower Boundary", "Upper Boundary", "Water Source", "Salt Source",
	"Enerby Balance Partitioning", "Heat Conduction", "Heat Convection", "Technical Regulation",
	"Emission", "Adsorption", "Reflection", "Mass Balance","Measurement Sensor","Geographic Position","Frost Bondary","Hysteresis",
	"Irrigation","Budget","Geometric","Scattering","Soil Freezing","Diffusion" };

	static constexpr std::array<std::string_view, static_cast<size_t>(bioprocess::COUNT)> m_BioProcessNames = { "Decomposition", "Mineralisation", "Nitrification",
		"Denitrification", "Respiration", "Production", "Oxidation", "Re Allocation", "Phenology", "Soil Gas Dynamics",
		"Photosynthesis", "Litterfall", "abiotic process", "Initial NC Conditions", "Initial P Conditions",
		"Forcing Biological Conditions", "Organic Transformation", "Balancing Ratio", "Concentration",
		"Organic Plant Uptake", "Plant Nitrogen Uptake", "Plant Phosphorus Uptake", "Sorption", "Harvest",
		"Grazing", "Mycon Nitrogen Uptake", "Soil mixing", "Emergence", "Plant Management",
		"Nitrogen Fixation","Living","Non Living","Growth","WoodEating","Weathering","Methane Oxidation","Methane Production",
		"Methane Transport","Gas Equilibrium" };


	static constexpr std::array<std::string_view, static_cast<size_t>(MODELTYPENAME::COUNT)> m_TypeNames = { "Switches","Parameters","Tables","Table Parameters", "State Variables",
	"Flow Variables","Auxiliary Variables", "Driving Variables", "Files", "NE", "DataBase", "RunInfo", "Validation" };
	
	static constexpr std::array<std::string_view, static_cast<size_t>(GROUPCATNAME::COUNT)>  m_GroupCategoryNames = { "All Modules", "Atmospheric Related Processes",
	"Plant Water Processes", "Soil Heat Processes", "Soil Water Processes", "Nitrogen and Carbon",
	"Nitrogen and Carbon - Below Ground", "Model Structure", "Model Facilities" };

	static constexpr std::array<std::string_view, static_cast<size_t>(GROUPNAME::COUNT)>  m_GroupNames = { "Hidden", "Technical", "Model Structure", "Numerical", "Soil Profile",
	"Soil Hydraulic", "Soil Thermal", "Meteorological Data", "Irrigation", "Plant", "Interception",
	"Potential Transpiration", "Radiation properties", "Soil evaporation", "SnowPack", "Soil frost",
	"Soil heat flows", "Soil heat pump", "Soil water flows", "Surface water", "Water uptake",
	"Drainage and deep percolation", "Salt Tracer", "Additional Variables", "Plant Growth",
	"Soil organic processes", "Soil mineral N processes", "Abiotic driving variables",
	"Common abiotic responses","External N inputs","Soil Management","Additional Biotic Variables",
	"Gas processes","Minteq","HBV","Validation","Soil mineral P Processes","External P Inputs" };


	static constexpr std::array<std::string_view,static_cast<size_t>(UNIT_TYPES::COUNT)> m_UnitTypeNames = {"MJ/m\xB2/d","J/m\xB2/d","J/m\xB2","J/m\xB3","MJ/m\xB3",//5
		"\xB0""C","\xB0""Cd", "W/m/\xB0""C","g/m\xB2","gC/m\xB2","gC/m\xB2/d","gN/m\xB2","gN/m\xB2/d","g/m\xB2/d","m/s",//15
		"mm/d","Vol %","cm water","m\xB2","g/m\xB3", "Kg/m\xB3", "m\xB2/g",//22
		"mg/m\xB2","mg/m\xB2/d","mm","Pa","m","m/s","s/m",//29
		"\xB5mol/m\xB2/d","m\xB2/d","m\xB2/s","cm\xB2/s","gm\xB2","PPM", "yr", "%", "d", "J/mol",//39
		"mg/l","\xB5g/l","-","1/d","d/mm","dm\xB2/g","g/g","m\xB3/m\xB3",//47
		"\xB5g/\xB5g","xB5g/m\xB2/d","\xB5g/d","min","rad","W/m/\xB0""C","1/m\xB2","DOY",//55
		"1/\xB0""C","Vol %"};

	std::map <std::string_view, size_t> m_ModelTypeNoMap, m_GroupNoMap;
	std::map < size_t,std::string_view> m_GroupCategoryNo;



	std::map <GROUPNAME, GROUPCATNAME>  m_GroupMap;
	//std::map <std::string_view, size_t> m_GroupCategory;
	//std::map <size_t, size_t> m_GroupCategoryNo;





	std::vector<std::string_view> m_GroupNamesSorted;
	std::vector<std::string_view> m_ElementSorted;
	std::vector<std::string_view> m_FysProcessNamesSorted, m_BioProcessNamesSorted;
	RunStatusOfDocumentInfo m_RunInfo;
	
	std::map <elements, std::string> m_ElementNamesMap;
	std::map <std::string_view, elements> m_NamesElementMap;

	std::map <fysprocess, std::string> m_PhysProcNamesMap;
	std::map <std::string_view, fysprocess> m_NamesFysProcMap;

	std::map <bioprocess, std::string> m_BioProcNamesMap;
	std::map <std::string_view, bioprocess> m_NamesBioProcMap;

	std::map<UNIT_TYPES, std::string_view> m_TypeBasedUnits;    // Correct the declaration of m_TypeBasedUnits
	std::map<std::string_view, UNIT_TYPES> m_StringBasedUnits;


	bool m_CommandInfo;
	Sim *m_pRunDoc;
};
