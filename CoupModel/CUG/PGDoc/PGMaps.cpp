#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include "PGMaps.h"



PGMaps::PGMaps()
{
	DefineUnitMap();
	vector<UNIT_TYPES> koll = GetUnitOptions(elements::WATER);
}


PGMaps::~PGMaps()
{
}
void PGMaps::DefineUnitMap()
{
	SetUnitMap(elements::WATER,UNIT_TYPES::WATERFLOW_UNIT);
	SetUnitMap(elements::WATER, UNIT_TYPES::WATERMASS_UNIT);
	SetUnitMap(elements::WATER, UNIT_TYPES::VOLPERCENTAGE_UNIT);
	SetUnitMap(elements::WATER, UNIT_TYPES::PRESSUREHEAD_UNIT);
	SetUnitMap(elements::HEAT, UNIT_TYPES::HEATFLOW_UNIT);
	SetUnitMap(elements::HEAT, UNIT_TYPES::HEATSOURCE_UNIT);
	SetUnitMap(elements::HEAT, UNIT_TYPES::HEATSTORE_UNIT);
	SetUnitMap(elements::HEAT, UNIT_TYPES::HEATCOND_UNIT);
	SetUnitMap(elements::NITROGEN, UNIT_TYPES::MASS_UNIT);
	SetUnitMap(elements::NITROGEN, UNIT_TYPES::MASSFLOW_UNIT);
	SetUnitMap(elements::CARBON, UNIT_TYPES::MASS_UNIT);
	SetUnitMap(elements::CARBON, UNIT_TYPES::MASSFLOW_UNIT);
	SetUnitMap(elements::RADIATION, UNIT_TYPES::HEATFLOW_UNIT);
	SetUnitMap(elements::SNOW, UNIT_TYPES::WATERMASS_UNIT);
	SetUnitMap(elements::SNOW,UNIT_TYPES::WATERFLOW_UNIT);
	SetUnitMap(elements::SNOW, UNIT_TYPES::LENGTH_UNIT);
	SetUnitMap(elements::SALT, UNIT_TYPES::MASS_UNIT);
	SetUnitMap(elements::SALT, UNIT_TYPES::MASSFLOW_UNIT);
	SetUnitMap(elements::SALT, UNIT_TYPES::mMASS_UNIT);
	SetUnitMap(elements::SALT, UNIT_TYPES::mMASSFLOW_UNIT);
	SetUnitMap(elements::VAPOUR, UNIT_TYPES::PRESSURE_UNIT );
	SetUnitMap(elements::VAPOUR, UNIT_TYPES::PERCENTAGE );
	SetUnitMap(elements::TEMPERATURE,UNIT_TYPES::TEMP_UNIT);
	SetUnitMap(elements::PRECIPITATION,UNIT_TYPES::WATERFLOW_UNIT);
	SetUnitMap(elements::AIR, UNIT_TYPES::LENGTH_PER_TIME_UNIT );
	SetUnitMap(elements::CLIMATE, UNIT_TYPES::MASSFLOW_UNIT);
	SetUnitMap(elements::CLIMATE,UNIT_TYPES::TEMP_UNIT);
	SetUnitMap(elements::CLIMATE, UNIT_TYPES::LENGTH_PER_TIME_UNIT);
	SetUnitMap(elements::CLIMATE, UNIT_TYPES::HEATFLOW_UNIT);
	SetUnitMap(elements::CLIMATE, UNIT_TYPES::HEATFLOW_UNIT);
	SetUnitMap(elements::CLIMATE, UNIT_TYPES::PERCENTAGE);
	SetUnitMap(elements::HUMIDITY, UNIT_TYPES::PERCENTAGE);
	SetUnitMap(elements::GLOBALRADIATION, UNIT_TYPES::HEATFLOW_UNIT);
	SetUnitMap(elements::LONGWAVEINCOMINGRADIATION, UNIT_TYPES::HEATFLOW_UNIT);
	SetUnitMap(elements::WIND, UNIT_TYPES::LENGTH_PER_TIME_UNIT);
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