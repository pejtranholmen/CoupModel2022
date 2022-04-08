#pragma once
#include "chartData.h"

class ChartProp;
class ChartUtil;
class CPG;

struct CHARTINFO
{
	size_t NumVariables;
	size_t NumPriorVariables;
	size_t NumVariableBands;
	size_t NumOfPoints;
	size_t NumOfCommonPoints;
	size_t NumBands;
	size_t NumOfYears;
	size_t NumOfDistPoints;
	size_t WithinDayRepresentation;
	size_t YearRepresentation;
	size_t HourRepresentation;
	size_t NumExtraPointFreq;
	CPG *pPGFirst;
	bool ResidualSource;
	bool MultiPGMode;
	bool MultiValMode;
	bool PGRepitMode;
	size_t DataType;
	bool RepitDataType;
};


class DistributionCurves: public ChartData
{
	public:
	DistributionCurves(void);
	~DistributionCurves(void);
	
	CHARTINFO m_ChartInfo;

	size_t GetAdjustedVariableIndex(size_t org_index);

	protected:
		void Frequence_SetTime();
		void Frequence_EstimateDataDist(size_t icurve, size_t TotNumPrior);
		void Frequence_Set_MinMaxNumberOfStepsDistType(size_t icurve);
		void Frequence_SetNumberOfPoint_LogScale(size_t icurve);


		ChartCug *m_pChartCug;
		ChartProp *m_pProp;
		ChartUtil *m_pUtil;


};

