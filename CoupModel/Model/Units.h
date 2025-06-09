#pragma once
#include "../ModelTypes/SimB.h"
#include <string_view> 
#include <map>
#include <vector>




struct MODELUNITS {
    enum class M_UNITS {};
	std::string value;
};

class Units {
public:
	Units();
	~Units();
	size_t GetNumUnits()  noexcept { return m_TypeBasedUnits.size();};
    std::string_view GetUnitString(UNIT_TYPES key, bool withparanthesis=false) {
					auto it = m_TypeBasedUnits.find(key);
					if (it != m_TypeBasedUnits.end()) {
						if (withparanthesis) {
							return it->second;//" + it->second + ")";
						} else {
							return it->second;
						}
					} else {
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

	std::vector<std::string_view> GetAllStringBasedUnits() {
		std::vector<std::string_view> out;
		for(auto it=m_TypeBasedUnits.begin(); it!=m_TypeBasedUnits.end();++it)  {
			out.push_back(it->second);
		};
		return out;
	};
	std::vector<std::string_view> GetSelectedStringBasedUnits(std::vector<UNIT_TYPES> key) {
		std::vector<std::string_view> out;
		for_each(key.begin(), key.end(), [&](UNIT_TYPES& key) {
			auto it = m_TypeBasedUnits.find(key);
			out.push_back((*it).second);
			});
		return out;
	};

private:


	void SetUnitMap(enum UNIT_TYPES type, std::string_view str) {
		m_TypeBasedUnits.insert(std::make_pair(type, str));
		m_StringBasedUnits.insert(std::make_pair(str, type));
	};

	std::map<enum UNIT_TYPES, std::string_view> m_TypeBasedUnits;    // Correct the declaration of m_TypeBasedUnits
	std::map<std::string_view, enum UNIT_TYPES> m_StringBasedUnits;
};



