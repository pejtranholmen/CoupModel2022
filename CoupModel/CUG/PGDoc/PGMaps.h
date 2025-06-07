#pragma once

#include "..\..\ModelTypes\SimB.h"
#include "..\..\NewBase\Doc.h"
#include "..\..\Model\Units.h"



class PGMaps 	
{
public:
	PGMaps();
	~PGMaps();

	void SetUnitMap(elements key, UNIT_TYPES typ) {
		
		m_StringMapUnits.insert(std::pair<elements, std::string>(key, string(m.GetUnitString(typ))));
		m_Units.insert(std::pair< elements, UNIT_TYPES>(key, typ));
	};

	std::vector<std::string> GetUnitOptionsNames(enum elements);
	std::vector<UNIT_TYPES> GetUnitOptions(enum elements);
private:
	Units m;
	multimap<elements, std::string> m_StringMapUnits;
	multimap<elements, UNIT_TYPES> m_Units;
	void DefineUnitMap();

};

