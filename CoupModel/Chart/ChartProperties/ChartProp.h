#pragma once
#include "../../std.h"

//#ifndef COUPSTD
#include "../ChartFrames/ChartFrame.h"
#include "./ChartProp_Distributions.h"
#include "./ChartProp_Regression.h"
#include "./ChartProp_AxisScaleValues.h"
#include "./ChartProp_AxisRepresentation.h"
#include "./Chartdir.h"
//#endif
#include "../../ModelTypes/NewModelType.h"
#include "../../NewBase/ModelBase.h"

class SimB;
struct SIMB;
class CPG;
class CUGCell;


struct CHART_TYPES_OPTIONS {
	enum CHART_TYPES typ;
	std::string_view typtext;
};

struct CHART_POS
{
	int xpos;
	int ypos;
};



struct AXIS_SCALE
{
	double LeftMin;
	double LeftMax;
	double LeftInter;
	double RightMin;
	double RightMax;
	double RightInter;
};

struct RESHAPE {
	bool m_SortedSeries;
	bool m_NormalDist;
	bool m_CommonWithDay;
	bool m_CommonDayNo;
	bool m_CommonScale;
	size_t m_Cumulated;
	size_t m_StackingVariables;
	bool m_RegLine;
	bool m_ShowMissingValue;
	bool m_AddLine;
	bool m_Symbol;
	bool m_DerivateValues;
	bool m_CommonWithinDayRes;
	long EquiDistance;
	size_t NumOfCommonPoints;

};
struct COMMONSCALE {
	double min;
	double max;
	double interv;
};


struct TimeScale {
	double min;
	double max;
	double interv;
	double MinX;  // Final
	double MaxX;  // Final
	double IntervX;
};
class ChartProp:public ChartFrame, public ChartProp_Distributions, public ChartProp_Regression, public ChartProp_AxisRepresentation
	, public ChartProp_AxisScaleValues
{
public:
	ChartProp(void);
	~ChartProp(void);
	
	RESHAPE m_ReShape;
	std::vector<CONSTRAIN_BY> m_ConstrainProp;

	std::string DefineYearWithinDayCell(CUGCell *pCell);
	void UpdateNumSubcharts() { m_NumSubCharts = m_NumChartX*m_NumChartY; };
	void AddConstrainByMultiPar(SIMB simv, size_t Index);

	void SetRightAxisExist(bool value = true) {
		m_RightAxisExist = value;
	};
	bool IsRightAxisExist() {
		return m_RightAxisExist;
	};

	CHART_POS GetChartPos(int ich);
	void SetMultiMode(bool value);
	void SetMultiSequence(int value);
	void SetMultiRepititions(int value);
	void SetCommonYear(bool value);
	void SetCommonDay(bool value);
	void SetSyncAxis(bool value) {m_SyncAxis=value;};

	bool SetChartType(int chart_type);
	bool SetChartType(CHART_TYPES chart_type);
	bool SetChartType(std::string chart_type_string);
	void ResetChartType() {
		SetChartType(m_PreviousChartType);
	};
	std::vector<std::string_view> GetChartTypeOptions(bool Complete = true);
	CHART_TYPES GetChartTypeIndex() const noexcept;
	std::string_view GetCurrentChartTypeOption() { return m_ChartTypesOptions[size_t(m_ChartType)].typtext; };
	std::string_view GetChartTypeString();

	bool CheckChartType_Based_On_IndependentVar();
	void MakeCumTranformation(std::vector<double> *pVector);


	void SetPGSource(CPG *pPGSource) { m_pPGSource = pPGSource; };
	CPG * GetPGSource() { return m_pPGSource; };
	bool HasPGSource() { if (m_pPGSource == nullptr) return false; else return true; };
	void DocumentError(Doc* pDoc);

// member data below

	bool m_MakeChartTitle,m_NoXAxisScale,m_NoYAxisScale;
	
	COMMONSCALE m_CommonScaleValues;
	std::vector<COMMONSCALE> m_CommonScaleValuesVector;

	TimeScale m_TimeScale;
	bool ChangeChartTypeForward();
	

	CHART_TYPES m_ChartType, m_PreviousChartType;

	int m_MultiSequence;
	int m_multi_ResidualSequence;
	bool m_MultiTable;

	bool m_AggregateTime;
	int m_ActualMultiSheet;


	AXIS_SCALE m_CommonScale;

	bool m_SyncAxis;

	
	size_t m_DayValueRepresentation;
	size_t m_HourValueRepresentation;
	size_t m_YearValueRepresentation;


	private:
		std::vector<CHART_TYPES_OPTIONS> m_ChartTypesOptions;
		bool m_RightAxisExist;
		CPG *m_pPGSource;
	
};

