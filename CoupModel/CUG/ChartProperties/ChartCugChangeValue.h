#pragma once
#include ".\ChartCugSheets.h"
class ChartCugChangeValue :
	public ChartCugSheets
{
public:
	ChartCugChangeValue(void);
	~ChartCugChangeValue(void);
protected:
	void ChangeParameterValue(int col, int row, CString str);
	bool ChangeGeneralChartLayOut(int col, int row, CString str);
	bool ChangeAxis(int col, int row, CString str); 
	bool ChangeVariableLegends(int col, int row, CString str); 
	bool ChangeDistributionValue(int col, int row, CString str); 
};

