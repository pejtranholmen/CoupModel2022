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
	size_t GetTypeIndex(std::string str);
	size_t GetGroupIndex(std::string str);
	size_t GetGroupCategoryNo(size_t index) {
		auto it = m_GroupCategoryNo.find(index);
		if (it != m_GroupCategoryNo.end())
			return it->second;
		else
			return std::string::npos;
;};

	std::string GetGroupCategoryNames(size_t index) { if (index<m_GroupCategoryNames.size()) return m_GroupCategoryNames[index]; return "All Modules"; };
	std::string GetGroupNames(size_t index) {if(index<m_GroupNames.size()) return m_GroupNames[index];return "";};
    std::string_view GetVarType(size_t ityp) const{ 
        if (ityp < size(m_TypeNames)) 
            return m_TypeNames[ityp]; 
        return ""; 
    };
	std::string_view GetFysProcName(size_t index) {if(index<m_FysProcessNames.size()) return m_FysProcessNames[index]; else return "";};
	std::string_view GetBioProcName(size_t index) {if(index<m_BioProcessNames.size()) return m_BioProcessNames[index]; else return "";};
	std::string_view ElementName(size_t index) {if(index<m_ElementNames.size()) return m_ElementNames[index]; else return "";};

	int GetGroupCategory(std::string str);
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
	std::map <std::string, size_t> m_TypeMap, m_GroupMap;
	std::map <std::string_view, size_t> m_GroupCategory;
	std::map <size_t, size_t> m_GroupCategoryNo;
	std::vector<std::string>  m_GroupNames, m_GroupCategoryNames;


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
