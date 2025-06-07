#pragma once
#include "PG.h"
class PGCalc :
	public CPG
{
public:
	PGCalc(void);
	~PGCalc(void);
	std::vector<size_t> GetYearStartIndex();
	std::vector<size_t> GetDayStartIndex();
};

