#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include "PGMaps.h"



PGMaps::PGMaps()
{
	DefineUnitMap();
	vector<UNIT_TYPES> koll = GetUnitOptions(WATER);
}


PGMaps::~PGMaps()
{
}
void PGMaps::DefineUnitMap()
{
	SetUnitMap(WATER,UNIT_TYPES::WATERFLOW_UNIT);
	SetUnitMap(WATER, UNIT_TYPES::WATERMASS_UNIT);
	SetUnitMap(WATER, UNIT_TYPES::WATERCONTENT_UNIT);
	SetUnitMap(WATER, UNIT_TYPES::PRESSUREHEAD_UNIT);
	SetUnitMap(HEAT, UNIT_TYPES::HEATFLOW_UNIT);
	SetUnitMap(HEAT, UNIT_TYPES::HEATSOURCE_UNIT);
	SetUnitMap(HEAT, UNIT_TYPES::HEATSTORE_UNIT);
	SetUnitMap(HEAT, UNIT_TYPES::HEATCOND_UNIT);
	SetUnitMap(NITROGEN, UNIT_TYPES::MASS_UNIT);
	SetUnitMap(NITROGEN, UNIT_TYPES::MASSFLOW_UNIT);
	SetUnitMap(CARBON, UNIT_TYPES::MASS_UNIT);
	SetUnitMap(CARBON, UNIT_TYPES::MASSFLOW_UNIT);
	SetUnitMap(RADIATION, UNIT_TYPES::HEATFLOW_UNIT);
	SetUnitMap(SNOW, UNIT_TYPES::WATERMASS_UNIT);
	SetUnitMap(SNOW,UNIT_TYPES::WATERFLOW_UNIT);
	SetUnitMap(SNOW, UNIT_TYPES::LENGTH_UNIT);
	SetUnitMap(SALT, UNIT_TYPES::MASS_UNIT);
	SetUnitMap(SALT, UNIT_TYPES::MASSFLOW_UNIT);
	SetUnitMap(SALT, UNIT_TYPES::mMASS_UNIT);
	SetUnitMap(SALT, UNIT_TYPES::mMASSFLOW_UNIT);
	SetUnitMap(VAPOUR, UNIT_TYPES::PRESSURE_UNIT );
	SetUnitMap(VAPOUR, UNIT_TYPES::PERCENTAGE );
	SetUnitMap(TEMPERATURE,UNIT_TYPES::TEMP_UNIT);
	SetUnitMap(PRECIPITATION,UNIT_TYPES::WATERFLOW_UNIT);
	SetUnitMap(AIR, UNIT_TYPES::LENGTH_PER_TIME_UNIT );
	SetUnitMap(CLIMATE, UNIT_TYPES::MASSFLOW_UNIT);
	SetUnitMap(CLIMATE,UNIT_TYPES::TEMP_UNIT);
	SetUnitMap(CLIMATE, UNIT_TYPES::LENGTH_PER_TIME_UNIT);
	SetUnitMap(CLIMATE, UNIT_TYPES::HEATFLOW_UNIT);
	SetUnitMap(CLIMATE, UNIT_TYPES::HEATFLOW_UNIT);
	SetUnitMap(CLIMATE, UNIT_TYPES::PERCENTAGE);
	SetUnitMap(HUMIDITY, UNIT_TYPES::PERCENTAGE);
	SetUnitMap(GLOBALRADIATION, UNIT_TYPES::HEATFLOW_UNIT);
	SetUnitMap(LONGWAVEINCOMINGRADIATION, UNIT_TYPES::HEATFLOW_UNIT);
	SetUnitMap(WIND, UNIT_TYPES::LENGTH_PER_TIME_UNIT);
}
vector<string> PGMaps::GetUnitOptionsNames(elements key)
{
	vector<string> outvector;

	auto itstart = m_StringMapUnits.lower_bound(key);
	auto itabove = m_StringMapUnits.upper_bound(key);
	if (itstart != m_StringMapUnits.end()) {
		for (auto it = itstart; it != itabove; ++it) {
			outvector.push_back((*it).second);
		}
	}
	return outvector;

}
vector<UNIT_TYPES> PGMaps::GetUnitOptions(elements key)
{
	vector<UNIT_TYPES> outvector;

	auto itstart = m_Units.lower_bound(key);
	auto itabove = m_Units.upper_bound(key);
	if (itstart != m_Units.end()) {
		for (auto it = itstart; it != itabove; ++it) {
			outvector.push_back((*it).second);
		}
	}
	return outvector;

}