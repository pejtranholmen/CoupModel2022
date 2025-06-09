#pragma once

#include "../Model/Units.h"
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
class CommonModelInfo : public Units
{ 
public:
	CommonModelInfo();
	~CommonModelInfo();
	size_t GetTypeIndex(std::string_view str);
	size_t GetGroupIndex(std::string_view str);
	size_t GetGroupCategoryNo(size_t index) {
		auto it = m_GroupCategoryNo.find(index);
		if (it != m_GroupCategoryNo.end())
			return it->second;
		else
			return std::string::npos;
;};

	std::string_view GetGroupCategoryNames(size_t index) { if (index<size(m_GroupCategoryNames)) return m_GroupCategoryNames[index]; return "All Modules"; };
	std::string_view GetGroupNames(size_t index) {if(index<size(m_GroupNames)) return m_GroupNames[index];return "";};
    std::string_view GetVarType(size_t ityp) const{ 
        if (ityp < size(m_TypeNames)) 
            return m_TypeNames[ityp]; 
        return ""; 
    };
	std::string_view GetFysProcName(fysprocess fys_proc) { auto index = static_cast<size_t>(fys_proc); if(index < m_FysProcessNames.size()) return m_FysProcessNames[index]; else return "";};
	std::string_view GetBioProcName(bioprocess bioproc) { auto index = static_cast<size_t>(bioproc); if (index < m_BioProcessNames.size()) return m_BioProcessNames[index]; else return "";	};
	std::string_view ElementName(elements elem) { auto index = static_cast<size_t>(elem); if (index < m_ElementNames.size()) return m_ElementNames[index]; else return ""; };

	int GetGroupCategory(std::string_view str);
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



	Sim *GetRunDoc()  noexcept {return m_pRunDoc;};
	std::vector<SimB*> SortSimB_ByGroup(std::vector<SimB*> v);
	std::vector<SimB*> SortSimB_ByCategory(std::vector<SimB*> v);
	std::map<std::string, std::string> changed_names;


private:
	static constexpr std::string_view m_TypeNames[] = { "Switches","Parameters","Tables","Table Parameters", "State Variables",
	"Flow Variables","Auxiliary Variables", "Driving Variables", "Files", "NE", "DataBase", "RunInfo", "Validation" };
	
	static constexpr std::string_view m_GroupCategoryNames[] = { "All Modules", "Atmospheric Related Processes",
	"Plant Water Processes", "Soil Heat Processes", "Soil Water Processes", "Nitrogen and Carbon",
	"Nitrogen and Carbon - Below Ground", "Model Structure", "Model Facilities" };

	static constexpr std::string_view m_GroupNames[] = { "Hidden", "Technical", "Model Structure", "Numerical", "Soil Profile",
	"Soil Hydraulic", "Soil Thermal", "Meteorological Data", "Irrigation", "Plant", "Interception",
	"Potential Transpiration", "Radiation properties", "Soil evaporation", "SnowPack", "Soil frost",
	"Soil heat flows", "Soil heat pump", "Soil water flows", "Surface water", "Water uptake",
	"Drainage and deep percolation", "Salt Tracer", "Additional Variables", "Plant Growth",
	"Soil organic processes", "Soil mineral N processes", "Abiotic driving variables",
	"Common abiotic responses","External N inputs","Soil Management","Additional Biotic Variables",
	"Gas processes","Minteq","HBV","Validation","Soil mineral P Processes","External P Inputs" };
	
	std::map <std::string_view, size_t> m_TypeMap, m_GroupMap;
	std::map <std::string_view, size_t> m_GroupCategory;
	std::map <size_t, size_t> m_GroupCategoryNo;

	std::map<std::string_view, size_t> m_TypeNameToIndex;




	std::vector<std::string_view> m_GroupNamesSorted;
	std::vector<std::string_view> m_FysProcessNames, m_BioProcessNames, m_ElementNames, m_ElementSorted;
	std::vector<std::string_view> m_FysProcessNamesSorted, m_BioProcessNamesSorted;
	RunStatusOfDocumentInfo m_RunInfo;
	
	std::map <elements, std::string> m_ElementNamesMap;
	std::map <std::string_view, elements> m_NamesElementMap;

	std::map <fysprocess, std::string> m_PhysProcNamesMap;
	std::map <std::string_view, fysprocess> m_NamesFysProcMap;

	std::map <bioprocess, std::string> m_BioProcNamesMap;
	std::map <std::string_view, bioprocess> m_NamesBioProcMap;


	bool m_CommandInfo;
	Sim *m_pRunDoc;
};
