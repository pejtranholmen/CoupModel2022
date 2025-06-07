#pragma once
#include "../std.h"
#include "PG.H"

struct TIMESTAT
{	int VarSub;
	int VarSubRep;
	int nSub;
	int VarUsed;
	int VarUsedRep;
	float R2;
	float A0;
	float A1;
};
class TimeAggregate
{
public:
	TimeAggregate(void) {;};
	~TimeAggregate(void) {;};
	bool CreateNewAggregatedFile(CPG *pPG, int option, int AggregationStep=1);

	std::string m_DestFileName;
	std::vector<std::string> m_ResultSum;

protected:

	CPG *m_pPGSource;

}

;

