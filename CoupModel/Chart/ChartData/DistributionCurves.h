#pragma once
#include "chartData.h"

class ChartProp;
class ChartUtil;
class CPG;

struct CHARTINFO
{
	size_t NumVariables=0;
	size_t NumPriorVariables=0;
	size_t NumVariableBands=0;
	size_t NumOfPoints=0;
	size_t NumOfCommonPoints=0;
	size_t NumBands=0;
	size_t NumOfYears=0;
	size_t NumOfDistPoints=0;
	size_t WithinDayRepresentation=0;
	size_t YearRepresentation=0;
	size_t HourRepresentation=0;
	size_t NumExtraPointFreq=0;
	CPG *pPGFirst=nullptr;
	bool ResidualSource=false;
	bool MultiPGMode=false;
	bool MultiValMode=false;
	bool PGRepitMode=false;
	size_t DataType=0;
	bool RepitDataType=false;
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


		ChartCug *m_pChartCug=nullptr;
		ChartProp *m_pProp=nullptr;
		ChartUtil *m_pUtil=nullptr;


};

