#pragma once
#include ".\ChartCugBase.h"
class ChartCugSheets :
	public ChartCugBase
{
public:
	ChartCugSheets(void);

	~ChartCugSheets(void);
	
	GRIDCHAR GetRowsAndCols();
	void OnUpdate();
	void MakeDataSheet(int);
	void Make_Sheet0_RealTime();
	void Make_Sheet1_General();
	void Make_Sheet2_RealTime();
	void Make_Sheet4_RealTime();
	void Make_Sheet3_RealTime(int NumTSV);
	void Make_Sheet0_ParameterPlot();
	void Make_Sheet2_ParameterPlot();
	void Make_Sheet3_ParameterPlot();
	void Make_Sheet0_TablePlot();
	void Make_Sheet0_TimeSeriePlot();
	void Make_Sheet3_TimeSeriePlot();
	void Make_Sheet4_TimeSeriePlot(int rows);
	void Make_Sheet5_TimeSeriePlot(int rows);
	void Make_Sheet2_TimeSeriePlot(bool RightLabels, bool XLabels, bool ZLabels);

	void ShowDataInGrid(bool CreateFile=false);

	void AddMainData(DoubleArray data, int index);
	void AddTimeData(DoubleArray data, DoubleArray data2, size_t index_x=string::npos);
	void AddBandData(DoubleArray datasd, DoubleArray datamin, DoubleArray datamax, DoubleArray datap10, DoubleArray datap50,DoubleArray datap90, int index);


protected:
	size_t m_extra_xy_variables;

	 
};

