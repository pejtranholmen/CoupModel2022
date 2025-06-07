#include "../std.h"
#include "./CommonModelInfo.h"
#include "../ModelTypes/SimB.h"
using namespace std;
CommonModelInfo::CommonModelInfo() {





	for (size_t i = 0; i < std::size(m_TypeNames); ++i) {
		m_TypeNameToIndex[m_TypeNames[i]] = i;
	}
	static constexpr std::string_view m_ElementNames[] = { "Water", "Heat", "Carbon", "Nitrogen", "Phosphorus", "Radiation",
		"Snow", "Salt", "Methane", "Vapour", "General", "Temperature", "Water Potential",
		"Soil", "Atmosphere", "Air", "Precipitation", "Climate", "Organic",
		"Dissolved Carbon", "Dissolved Nitrogen", "Dissolved Phosphorus", "Mineral N", "Mineral P", "Ammonium", "Nitrate",
		"Phosphate", "Mycorrhize", "Denitrifiers", "Nitrofiers", "Anarobic gas", "Oxygen", "Manure",
		 "Plant Shape", "Distance", "CO2 SOIL", "Time", "Area", "Plant Stress", "Wind Speed",
		"Global Radiation", "LongWave Radiation", "Humidity", "GreenHouseGas", "Plant Biomass", "Nitric Oxide", "Nitrous Oxide" };

	
	static constexpr std::string_view m_FysProcessNames[] = { "Root Water Uptake", "Evapotranspiration", "Transpiration",
		"Soil Evaporation", "Potential Transpiration", "Interception", "Turbulence", "Surface Runoff", "Aerodynamic Resistance",
		"Snow Melting", "Redistribution", "Storage", "Transport", "Infiltration", "Drainage", "Percolation",
		"Runoff", "biotic process", "Initial Flow", "Shrinking", "Physical Shape", "Heat Extraction",
		"Heat Pipe", "Heat Source", "Lower Boundary", "Upper Boundary", "Water Source", "Salt Source",
		"Enerby Balance Partitioning", "Heat Conduction", "Heat Convection", "Technical Regulation",
		"Emission", "Adsorption", "Reflection", "Mass Balance","Measurement Sensor","Geographic Position","Frost Bondary","Hysteresis",
		"Irrigation","Budget","Geometric","Scattering","Soil Freezing","Diffusion" };

	static constexpr std::string_view m_BioProcessNames[] = { "Decomposition", "Mineralisation", "Nitrification",
		"Denitrification", "Respiration", "Production", "Oxidation", "Re Allocation", "Phenology", "Soil Gas Dynamics",
		"Photosynthesis", "Litterfall", "abiotic process", "Initial NC Conditions", "Initial P Conditions",
		"Forcing Biological Conditions", "Organic Transformation", "Balancing Ratio", "Concentration",
		"Organic Plant Uptake", "Plant Nitrogen Uptake", "Plant Phosphorus Uptake", "Sorption", "Harvest",
		"Grazing", "Mycon Nitrogen Uptake", "Soil mixing", "Emergence", "Plant Management",
		"Nitrogen Fixation","Living","Non Living","Growth","WoodEating","Weathering","Methane Oxidation","Methane Production",
		"Methane Transport","Gas Equilibrium" };


	static constexpr std::string_view m_GroupNames[] = { "Hidden", "Technical", "Model Structure", "Numerical", "Soil Profile",
		"Soil Hydraulic", "Soil Thermal", "Meteorological Data", "Irrigation", "Plant", "Interception",
		"Potential Transpiration", "Radiation properties", "Soil evaporation", "SnowPack", "Soil frost",
		"Soil heat flows", "Soil heat pump", "Soil water flows", "Surface water", "Water uptake",
		"Drainage and deep percolation", "Salt Tracer", "Additional Variables", "Plant Growth",
		"Soil organic processes", "Soil mineral N processes", "Abiotic driving variables",
		"Common abiotic responses","External N inputs","Soil Management","Additional Biotic Variables",
		"Gas processes","Minteq","HBV","Validation","Soil mineral P Processes","External P Inputs" };

	static constexpr std::string_view m_GroupCategoryNames[] = { "All Modules", "Atmospheric Related Processes",
		"Plant Water Processes", "Soil Heat Processes", "Soil Water Processes", "Nitrogen and Carbon",
		"Nitrogen and Carbon - Below Ground", "Model Structure", "Model Facilities" };


	m_GroupCategoryNo.insert(std:: pair<size_t, size_t>(0, 0));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(1,8));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(2,7));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(3,8));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(4,7));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(5,4));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(6,3));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(7,1));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(8,2));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(9,2));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(10,2));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(11,2));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(12,1));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(13,1));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(14,1));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(15,3));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(16,3));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(17,3));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(18,4));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(19,4));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(20,2));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(21,4));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(22,2));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(23,7));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(24,5));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(25,6));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(26,6));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(27,6));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(28,7));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(29,5));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(30,6));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(31,7));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(32,6));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(33,6));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(34,4));
	m_GroupCategoryNo.insert(pair<size_t,size_t>(35,0));
	m_GroupCategoryNo.insert(pair<size_t, size_t>(36, 6));
	m_GroupCategoryNo.insert(pair<size_t, size_t>(37, 5));


		m_GroupCategory.insert(pair<string,int>("Technical",8));
		m_GroupCategory.insert(pair<string,int>("Model Structure",7));
		m_GroupCategory.insert(pair<string,int>("Numerical",8));
		m_GroupCategory.insert(pair<string,int>("Soil Profile",7));
		m_GroupCategory.insert(pair<string,int>("Soil Hydraulic",4));
		m_GroupCategory.insert(pair<string,int>("Soil Thermal",3));
		m_GroupCategory.insert(pair<string,int>("Meteorological Data",1));

		m_GroupCategory.insert(pair<string,int>("Irrigation" ,4));
		m_GroupCategory.insert(pair<string,int>( "Plant",2));
		m_GroupCategory.insert(pair<string,int>( "Interception",2));
		m_GroupCategory.insert(pair<string,int>("Potential Transpiration" ,2));
		m_GroupCategory.insert(pair<string,int>("Radiation properties" ,1));
		m_GroupCategory.insert(pair<string,int>("Soil evaporation" ,1));
		m_GroupCategory.insert(pair<string,int>( "SnowPack",1));

		m_GroupCategory.insert(pair<string,int>( "Soil frost",3));
		m_GroupCategory.insert(pair<string,int>("Soil heat flows" ,3));
		m_GroupCategory.insert(pair<string,int>("Soil heat pump" ,3));
		m_GroupCategory.insert(pair<string,int>("Soil water flows" ,4));
		m_GroupCategory.insert(pair<string,int>( "Surface water",4));
		m_GroupCategory.insert(pair<string,int>("Water uptake" ,2));

		m_GroupCategory.insert(pair<string,int>("Drainage and deep percolation" ,4));
		m_GroupCategory.insert(pair<string,int>("Salt Tracer" ,4));
		m_GroupCategory.insert(pair<string,int>("Additional Variables" ,7));
		m_GroupCategory.insert(pair<string,int>( "Plant Growth",5));
		m_GroupCategory.insert(pair<string,int>("Soil organic processes",6));
		m_GroupCategory.insert(pair<string, int>("Soil mineral P processes",6));
		m_GroupCategory.insert(pair<string,int>("Soil mineral N processes",6));
		m_GroupCategory.insert(pair<string,int>( "Abiotic driving variables",7));
		m_GroupCategory.insert(pair<string,int>("Common abiotic responses" ,7));
		m_GroupCategory.insert(pair<string,int>( "External N inputs" ,5));
		m_GroupCategory.insert(pair<string, int>("External P inputs", 5));
		m_GroupCategory.insert(pair<string,int>( "Soil Management" ,6));
		m_GroupCategory.insert(pair<string,int>( "Additional Biotic Variables" ,7));
		m_GroupCategory.insert(pair<string,int>( "Gas processes" ,6));
		m_GroupCategory.insert(pair<string,int>( "Minteq" ,6));
		m_GroupCategory.insert(pair<string,int>( "HBV" ,4));
		m_GroupCategory.insert(pair<string,int>( "Validation" ,0));
		m_RunInfo.StartMode=true;
		m_RunInfo.ReadyToSimulate=true;
		m_RunInfo.Running=false;
		m_RunInfo.MultiRunning=false;
		m_RunInfo.ViewStatus=0;
		
		std::map <std::string_view, elements> SortedListElementNames;

		for(size_t count = 0; count < size(m_ElementNames); ++count) {
			SortedListElementNames.insert(std::make_pair(m_ElementNames[count], elements(count)));
			m_ElementNamesMap.insert(std::make_pair(elements(count), std::string(m_ElementNames[count])));
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

}
CommonModelInfo::~CommonModelInfo() {

}

size_t CommonModelInfo::GetTypeIndex(string str) {
	auto it=m_TypeMap.find(str);
	if(it!=m_TypeMap.end())
		return it->second;
	else
		return std::string::npos;
}
size_t CommonModelInfo::GetGroupIndex(string str) {
	auto it=m_GroupMap.find(str);
	if(it!=m_GroupMap.end())
		return it->second;
	else
		return std::string::npos;
}
int CommonModelInfo::GetGroupCategory(string str) {
	auto it=m_GroupCategory.find(str);
	if(it!=m_GroupCategory.end())
		return it->second;
	else
		return std::string::npos;
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

