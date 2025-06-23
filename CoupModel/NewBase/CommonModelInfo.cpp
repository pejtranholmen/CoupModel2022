#include "../std.h"
#include "./CommonModelInfo.h"
#include "../ModelTypes/SimB.h"
using namespace std;
CommonModelInfo::CommonModelInfo() {

	m_GroupMap.clear();
	m_GroupMap.emplace(GROUPNAME::HIDDEN, GROUPCATNAME::ALL);
	m_GroupMap.emplace(GROUPNAME::TECHNICAL, GROUPCATNAME::MODEL_FACIL);
	m_GroupMap.emplace(GROUPNAME::MODEL_STRUC, GROUPCATNAME::MODEL_STRUC);
	m_GroupMap.emplace(GROUPNAME::NUMERICAL, GROUPCATNAME::MODEL_FACIL);
	m_GroupMap.emplace(GROUPNAME::SOIL_PROFILE, GROUPCATNAME::MODEL_FACIL);
	m_GroupMap.emplace(GROUPNAME::SOIL_HYDRAULIC, GROUPCATNAME::SOIL_WATER);
	m_GroupMap.emplace(GROUPNAME::SOIL_THERMAL, GROUPCATNAME::SOIL_HEAT);
	m_GroupMap.emplace(GROUPNAME::METEOROLOGICAL_DATA, GROUPCATNAME::ATM);
	m_GroupMap.emplace(GROUPNAME::IRRIGATION, GROUPCATNAME::PLANT);
	m_GroupMap.emplace(GROUPNAME::PLANT, GROUPCATNAME::PLANT);
	m_GroupMap.emplace(GROUPNAME::INTERCEPTION, GROUPCATNAME::PLANT);
	m_GroupMap.emplace(GROUPNAME::POTENTIAL_TRANSP, GROUPCATNAME::PLANT);
	m_GroupMap.emplace(GROUPNAME::RADIATION, GROUPCATNAME::ATM);
	m_GroupMap.emplace(GROUPNAME::SOIL_EVAPORATION, GROUPCATNAME::ATM);
	m_GroupMap.emplace(GROUPNAME::SNOWPACK, GROUPCATNAME::ATM);
	m_GroupMap.emplace(GROUPNAME::SOIL_FROST, GROUPCATNAME::SOIL_HEAT);
	m_GroupMap.emplace(GROUPNAME::SOIL_HEAT_FLOW, GROUPCATNAME::SOIL_HEAT);
	m_GroupMap.emplace(GROUPNAME::SOIL_HEAT_PUMP, GROUPCATNAME::SOIL_HEAT);
	m_GroupMap.emplace(GROUPNAME::SOIL_WATER_FLOW, GROUPCATNAME::SOIL_WATER);

	m_GroupMap.emplace(GROUPNAME::SURFACE_WATER, GROUPCATNAME::SOIL_WATER);
	m_GroupMap.emplace(GROUPNAME::WATER_UPTAKE, GROUPCATNAME::PLANT);
	m_GroupMap.emplace(GROUPNAME::DRAINAGE_DEEP_PERC, GROUPCATNAME::SOIL_WATER);
	m_GroupMap.emplace(GROUPNAME::SALT_TRACER, GROUPCATNAME::SOIL_WATER);
	m_GroupMap.emplace(GROUPNAME::ADDITION_VAR, GROUPCATNAME::MODEL_STRUC);
	m_GroupMap.emplace(GROUPNAME::PLANT_GROWTH, GROUPCATNAME::NITROGEN_CARBON_ABOVE);

	m_GroupMap.emplace(GROUPNAME::SOIL_ORGANIC, GROUPCATNAME::NITROGEN_CARBON_BELOW);
	m_GroupMap.emplace(GROUPNAME::SOIL_MINERAL, GROUPCATNAME::NITROGEN_CARBON_BELOW);
	m_GroupMap.emplace(GROUPNAME::ABIOTIC_DRIVING, GROUPCATNAME::NITROGEN_CARBON_BELOW);
	m_GroupMap.emplace(GROUPNAME::COMMON_ABIOTIC_RESPONSE, GROUPCATNAME::MODEL_STRUC);
	m_GroupMap.emplace(GROUPNAME::EXTERNAL_N_INPUT, GROUPCATNAME::NITROGEN_CARBON_ABOVE);
	m_GroupMap.emplace(GROUPNAME::SOIL_MANAGEMENT, GROUPCATNAME::NITROGEN_CARBON_BELOW);


	m_GroupMap.emplace(GROUPNAME::ADDITIONAL_BIOTIC, GROUPCATNAME::MODEL_STRUC);
	m_GroupMap.emplace(GROUPNAME::GAS_PROCESSES, GROUPCATNAME::NITROGEN_CARBON_BELOW);
	m_GroupMap.emplace(GROUPNAME::MINTEQ, GROUPCATNAME::NITROGEN_CARBON_BELOW);
	m_GroupMap.emplace(GROUPNAME::HBV, GROUPCATNAME::SOIL_WATER);
	m_GroupMap.emplace(GROUPNAME::VALIDATION, GROUPCATNAME::ALL);
	m_GroupMap.emplace(GROUPNAME::SOIL_MINERAL_P, GROUPCATNAME::NITROGEN_CARBON_BELOW);


	m_GroupMap.emplace(GROUPNAME::EXTERNAL_P_INPUT, GROUPCATNAME::NITROGEN_CARBON_ABOVE);


		m_RunInfo.StartMode=true;
		m_RunInfo.ReadyToSimulate=true;
		m_RunInfo.Running=false;
		m_RunInfo.MultiRunning=false;
		m_RunInfo.ViewStatus=0;
		
		std::map <std::string_view, elements> SortedListElementNames;

		for(size_t count = 0; count < size(ElementNames); ++count) {
			SortedListElementNames.insert(std::make_pair(ElementNames[count], elements(count)));
			m_ElementNamesMap.insert(std::make_pair(elements(count), std::string(ElementNames[count])));
		}
		m_NamesElementMap = SortedListElementNames;
		for (auto it = SortedListElementNames.begin(); it != SortedListElementNames.end(); ++it) {
			m_ElementSorted.push_back((*it).first);
		}


		std::map <std::string_view, fysprocess> SortedListPhysProcNames;
	

		for(size_t count=0; count<size(m_FysProcessNames); count++) {
			SortedListPhysProcNames.insert(std::make_pair(m_FysProcessNames[count], fysprocess(count)));
			m_PhysProcNamesMap.insert(std::make_pair(fysprocess(count), std::string(m_FysProcessNames[count])));
		}
		for(auto it = SortedListPhysProcNames.begin(); it != SortedListPhysProcNames.end(); ++it) {
			m_FysProcessNamesSorted.push_back((*it).first);
		}
		map <string, bioprocess> SortedBiolProcessNames;
		for(size_t count = 0; count < size(m_BioProcessNames); count++) {
			SortedBiolProcessNames.insert(std::make_pair(m_BioProcessNames[count], bioprocess(count)));
			m_BioProcNamesMap.insert(std::make_pair(bioprocess(count), std::string(m_BioProcessNames[count])));
		}
		for(auto it = SortedBiolProcessNames.begin(); it != SortedBiolProcessNames.end(); ++it) {
			m_BioProcessNamesSorted.push_back((*it).first);
		}


	

		m_StringBasedUnits.clear();

		auto koll = size(ElementNames);

		for(size_t count=0; count<size(m_UnitTypeNames); count++) {
        // Fix for the reported error by ensuring the correct type is used for the map value
	        m_StringBasedUnits.insert(std::make_pair(m_UnitTypeNames[count], static_cast<UNIT_TYPES>(count)));
		}
		for(size_t count=0; count<size(m_GroupNames); count++) {
			m_GroupNoMap[m_GroupNames[count]]=count;
		}
		for (size_t count = 0; count < static_cast<size_t>(GROUPCATNAME::COUNT); count++) {
			auto group_cat = static_cast<GROUPCATNAME>(count);
			auto index = static_cast<size_t>(group_cat);
			m_GroupCategoryNo[index] = m_GroupCategoryNames[count];
		}
		for (size_t count = 0; count < size(m_TypeNames); count++) {
			m_ModelTypeNoMap[m_TypeNames[count]] = count;
		}

}
CommonModelInfo::~CommonModelInfo() {

}
size_t CommonModelInfo::GetTypeIndex(std::string_view str)
{
	auto it = m_ModelTypeNoMap.find(str);
	if (it != m_ModelTypeNoMap.end()) {
		return static_cast<size_t>((*it).second);
	}
	else {
		return static_cast<size_t>(MODELTYPENAME::COUNT);
	}
}
;
size_t CommonModelInfo::GetGroupIndex(std::string_view str) {
	auto it = m_GroupNoMap.find(str);
	if (it != m_GroupNoMap.end()) {
		return static_cast<size_t>((*it).second);
	} else {
		return static_cast<size_t>(GROUPNAME::COUNT);
	}

};
std::string_view CommonModelInfo::ElementName(elements elem) noexcept { if (elem < elements::COUNT) { auto index = static_cast<size_t>(elem);  return ElementNames[index]; } else return "unknow"; };

std::string_view CommonModelInfo::GetUnitString(UNIT_TYPES u) noexcept { if (u < UNIT_TYPES::COUNT) { return m_UnitTypeNames[static_cast<size_t>(u)]; } else return "unknow"; }
std::string CommonModelInfo::GetUnitString(UNIT_TYPES key, bool with_parenthesis) noexcept {
	if (with_parenthesis) return "(" + string(GetUnitString(key)) + ")";
	else  return string(GetUnitString(key));
}

std::string_view CommonModelInfo::GetGroupName(GROUPNAME group) noexcept {
		if(group< GROUPNAME::COUNT) 
			return m_GroupNames[static_cast<size_t>(group)];
		else
			return "Unknown Group";

};
std::string_view CommonModelInfo::GetGroupCatName(GROUPCATNAME groupcat) noexcept {
	if (groupcat < GROUPCATNAME::COUNT)
		return m_GroupCategoryNames[static_cast<size_t>(groupcat)];
	else
		return "Unknown Groupcat";
};
int CommonModelInfo::GetGroupCategory(string_view group) noexcept {
		int GroupNo = -1;
		auto it = m_GroupNoMap.find(group);
		if (it != m_GroupNoMap.end()) {
			GroupNo=static_cast<int>((*it).second);
		}
		return GroupNo;
}
int CommonModelInfo::GetGroupCategoryNo(size_t group_no) noexcept {
	GROUPNAME group = static_cast<GROUPNAME>(group_no);
	auto it = m_GroupMap.find(group);
	if (it != m_GroupMap.end()) {
		auto cat=static_cast<GROUPCATNAME>(it->second);
		auto cat_no = static_cast<size_t>(cat);
		return cat_no;
	}
	return -1;	
}

std::string_view CommonModelInfo::GetVarType(size_t index) noexcept
{
	return m_TypeNames[index];
}

void CommonModelInfo::trim_xml(std::string& str)
{
	string in, out;
	in = str;
	auto pos = str.find(" ");
	while (pos != std::string::npos) {
		str.erase(pos, 1);
		pos = str.find(" ");
	}
	pos = str.find(",");
	while (pos != std::string::npos) {
		str.erase(pos, 1);
		pos = str.find(",");
	}
	pos = str.find("(");
	while (pos != std::string::npos) {
		str.erase(pos, 1);
		pos = str.find("(");
	}
	pos = str.find(")");
	while (pos != std::string::npos) {
		str.erase(pos, 1);
		pos = str.find(")");
	}
	pos = str.find("'");
	while (pos != std::string::npos) {
		str.erase(pos, 1);
		pos = str.find("'");
	}
	pos = str.find("&");
	while (pos != std::string::npos) {
		str.erase(pos, 1);
		pos = str.find("&");
	}
	pos = str.find("/");
	while (pos != std::string::npos) {
		str.erase(pos, 1);
		pos = str.find("/");
	}
	out = str;
	if (in != out) {
		changed_names.insert(pair<string, std::string>(out, in));

	}
}
void CommonModelInfo::insert_ChangedNames(std::vector<std::pair<std::string, std::string>> p)
{
	for (auto it = p.begin(); it != p.end(); it++) {
		std::string in = (*it).first;
		std::string out = (*it).second;
		changed_names.insert(pair<string, std::string>(in, out));

	}
}

vector<string_view> CommonModelInfo::GetSortedListofElements()
{
	return m_ElementSorted;
}

vector<string_view> CommonModelInfo::GetSortedListofPhysProc()
{
	return m_FysProcessNamesSorted;
}

vector<string_view> CommonModelInfo::GetSortedListofBiolProc()
{
	return m_BioProcessNamesSorted;
}


string CommonModelInfo::GetStringOfElement(elements elem) {
	auto it = m_ElementNamesMap.find(elem);
	if (it != m_ElementNamesMap.end())
		return (*it).second;
	else
		return "";
}
elements CommonModelInfo::GetElementFromString(string str) {
	auto it = m_NamesElementMap.find(str);
	if (it != m_NamesElementMap.end())
		return (*it).second;
	else
		return elements::GENERAL;
}
fysprocess CommonModelInfo::GetPhysProcFromString(string str) {
	auto it = m_NamesFysProcMap.find(str);
	if (it != m_NamesFysProcMap.end())
		return (*it).second;
	else
		return fysprocess::NOFPROC;
}bioprocess CommonModelInfo::GetBioProcFromString(string str) {
	auto it = m_NamesBioProcMap.find(str);
	if (it != m_NamesBioProcMap.end())
		return (*it).second;
	else
		return bioprocess::NOBPROC;
}
vector<SimB*> CommonModelInfo::SortSimB_ByGroup(vector<SimB*> v_in) {
	vector<SimB*> v_out;
	multimap<string_view, SimB*> SortMap;
	for(size_t i=0; i<v_in.size(); i++) {	
		SortMap.insert(std::pair<std::string_view, SimB*>(v_in[i]->GetGroup(),v_in[i]));
	}

	for(auto it=SortMap.begin(); it!=SortMap.end(); ++it) {
		v_out.push_back((*it).second);
	}
	return v_out;

}
vector<SimB*> CommonModelInfo::SortSimB_ByCategory(vector<SimB*> v_in) {

	vector<SimB*> v_out;
	multimap<int, SimB*> SortMap;
	for(size_t i=0; i<v_in.size(); i++) {	
		SortMap.insert(pair<int, SimB*>(GetGroupCategory(v_in[i]->GetGroup()),v_in[i]));
	}

	for(auto it=SortMap.begin(); it!=SortMap.end(); ++it) {
		v_out.push_back((*it).second);
	}
	return v_out;
}

