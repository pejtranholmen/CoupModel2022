#pragma once
#include "..\simdoc\gridsimdoc.h"
enum sortby {Type, Module, Process, Name};
class CMainFrame;
class ChartCug;
struct MultiVal;
class MakeSimCharts :
	public GridSimDoc
{
public:
	MakeSimCharts(void);
	bool ManagePlotWindows(int, int, bool multisum=false);
	void UpdateCurrentChart(bool multi=false);
	bool CreateZoomPlotWindow(bool multi=false, std::string FileName="");
	void UpdatePlotStatusBar(int, BOOL clean=false);
	void TimeSerieChartClick(long col, long row, long isheet, SimB *pSw=nullptr, int Index=0, int VariableType=0, std::string Type="", int ValFileNo=1, int RepitNo=1);
	void TimeSerieChart(int isheet);
	void CleanChartsMade();
	bool CalculateMeanOffTimeSeries(long item, int ival_file, size_t ival_index= std::string::npos);
	MultiVAL GetBlankMultiValStruct();
	~MakeSimCharts(void);

	UINT m_CurrentChart;
	//int m_CurrentChartType;
	int m_PreviousChart;
	CPtrArray m_SelectedDoc, m_Charts_Array;


protected:
	vector<SIMB> GetAllOutPutVectorByCategory(int category, bool AllEnabled, bool SelectedGroup=false);
	vector<Ps*> GetParameterVectorByCategory(int category);
	vector<Sw*> GetSwitchVectorByCategory(int category);
	vector<Tab*> GetTabVectorByCategory(int category);
	bool Make_ParameterChart(SimB* pSimB, Tab *pTAB=nullptr, size_t index=0, size_t FuncNo=0);
	bool Make_TableChart(Tab* pTab, P *pCPt = nullptr);
	bool Make_SwitchChart(Sw *pSw=nullptr, std::string Group="");

	bool MultiPlotClick(int isheet, int col,  int row, bool Add=true, int Indep=0);
	bool MultiPlotVectorClick(vector<int> row_vector, size_t col= std::string::npos);

	bool MakeMultiPlot(int isheet, int col, int row);
	bool CreateChart(CHART_TYPES charttype, int chartnum, enum class DATA_TYPES dtype, std::string file="");
//	void TimeSerieChartClick(long col, long row, long isheet, CPG *pSw, int Index, int VariableType, std::string Type, int ValFileNo=1, int RepitNo=1);

	
	void MakePlotWindows();

	SimB* m_pSel;
	string m_SelType,m_SelGroup, m_SelName;
	enum sortby SelectedSort[3];


	int m_SelLocalIndex;
	bool m_SelectAllIndex;


	size_t m_CurrentSubChart;

	string m_PGFileNameAll,m_PGFileNameSelected;
	string m_ClickedTable;
	string m_created_pathname;
	
	int m_ClickedRow;
	long m_CurrentTopRow;

	int m_VarNo;

	CMainFrame *m_pFrame;

private:
	
	ChartCug *m_pChartCug;
	PlotInfo *m_pPlotInfo;
	ChartProp *m_pProp;
	vector<XYChart*> m_XY_Chart;
	MultiVAL GetMultiValStruct_Sum(int row, int col, bool *Add, int Indep=0);
	MultiVAL GetMultiValStruct_Val(int row, int col, bool *Add, int Indep=0);
	MultiVAL GetMultiValStruct_Par(int row, int col, bool *Add, int Indep=0);


	 
};

