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
	SetUnitMap(WATER, WATERFLOW_UNIT);
	SetUnitMap(WATER, WATERMASS_UNIT);
	SetUnitMap(WATER, WATERCONTENT_UNIT);
	SetUnitMap(WATER, PRESSUREHEAD_UNIT);
	SetUnitMap(HEAT, HEATFLOW_UNIT);
	SetUnitMap(HEAT, HEATSOURCE_UNIT);
	SetUnitMap(HEAT, HEATSTORE_UNIT);
	SetUnitMap(HEAT, HEATCOND_UNIT);
	SetUnitMap(NITROGEN, MASS_UNIT);
	SetUnitMap(NITROGEN, MASSFLOW_UNIT);
	SetUnitMap(CARBON, MASS_UNIT);
	SetUnitMap(CARBON, MASSFLOW_UNIT);
	SetUnitMap(RADIATION, HEATFLOW_UNIT);
	SetUnitMap(SNOW, WATERMASS_UNIT);
	SetUnitMap(SNOW, WATERFLOW_UNIT);
	SetUnitMap(SNOW, LENGTH_UNIT);
	SetUnitMap(SALT, MASS_UNIT);
	SetUnitMap(SALT, MASSFLOW_UNIT);
	SetUnitMap(SALT, mMASS_UNIT);
	SetUnitMap(SALT, mMASSFLOW_UNIT);
	SetUnitMap(VAPOUR,PRESSURE_UNIT );
	SetUnitMap(VAPOUR,PERCENTAGE );
	SetUnitMap(TEMPERATURE,TEMP_UNIT);
	SetUnitMap(PRECIPITATION,WATERFLOW_UNIT);
	SetUnitMap(AIR,LENGTH_PER_TIME_UNIT );
	SetUnitMap(CLIMATE, MASSFLOW_UNIT);
	SetUnitMap(CLIMATE, TEMP_UNIT);
	SetUnitMap(CLIMATE, LENGTH_PER_TIME_UNIT);
	SetUnitMap(CLIMATE, HEATFLOW_UNIT);
	SetUnitMap(CLIMATE, HEATFLOW_UNIT);
	SetUnitMap(CLIMATE, PERCENTAGE);
	SetUnitMap(HUMIDITY, PERCENTAGE);
	SetUnitMap(GLOBALRADIATION, HEATFLOW_UNIT);
	SetUnitMap(LONGWAVEINCOMINGRADIATION, HEATFLOW_UNIT);
	SetUnitMap(WIND, LENGTH_PER_TIME_UNIT);
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