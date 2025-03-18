#pragma once
#include "../../std.h"
#include "../ChartFrames/ChartFrame.h"
#ifndef COUPSTD
#include "./Chartdir.h"
#endif
#include "../../ModelTypes/NewModelType.h"


struct DISTRIBUTIONS
{
	double Min;
	double Max;
	bool LogScale;
};

class ChartProp_Distributions
{
public:
	ChartProp_Distributions(void);
	~ChartProp_Distributions(void);

	bool SetDistScale(float minv, float maxv, bool logv)
	{
		m_Dist.Min = minv;
		m_Dist.Max = maxv;
		m_Dist.LogScale = logv;
		m_ScaleSet = true;
		return true;
	};
	void ResetDistScale() {
		m_Dist.Min = 0.;
		m_Dist.Max = 0.;
		m_ScaleSet = false;
	}
	bool IsActualDistLogScale() { return m_Dist.LogScale; };
	void SetActualDistLogScale(bool value) { m_Dist.LogScale = value; };
	bool IsDistScaleValidSet() { 
		if (m_Dist.Max > m_Dist.Min&&m_ScaleSet) return true; else return false; };
	double GetActualDistMin() { return m_Dist.Min; };
	double GetActualDistMax() { return m_Dist.Max; };


	float GetXDistMin(size_t Index ) ;
	float GetXDistMax(size_t Index ) ;
	float GetCommonXDistMin();
	float GetCommonXDistMax();
	bool isXDistLog(size_t index) ;
	
	bool isXDistSet(size_t index) ;
	void ResetXDistSet(size_t index);

	int GetXDistSteps(size_t index) ;

	bool isXDistChanged(size_t index) ;
	void SetXDistChanged(size_t index, bool value) ;

	void SetXDistMin(size_t index, float v) ;
	void SetXDistMax(size_t index, float v) ;
	bool SetXDistLog(int index, bool value); 
	bool SetXDistSteps(int index, size_t value);
	void ClearAndResetValuesForDistributionToChartVariables(size_t chartnumindex, size_t NumVar = 0);
	bool AreDistributionsFunctionNotAllocatedForTheNumber(size_t NumVariables) { if (m_XDistMax.size() < NumVariables) return true; return  false; };

	void SetDistributionLogScale(bool value) { m_Dist.LogScale = value; };


private:
	
	DISTRIBUTIONS m_Dist;

	bool m_ScaleSet;


	vector<size_t> m_XDistSteps, m_XDistColor;

	vector<bool>  m_XDistSet, m_XDistLog, m_XDistChanged;
	vector<float> m_XDistMin, m_XDistMax;

	
};

