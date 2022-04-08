#pragma once
class Doc;
class CPG;
class MakePGCharts
{
public:
	MakePGCharts(void);
	~MakePGCharts(void);
	void TimeSerieChartClick();
	void MultiPGChartClick();
	bool CreateZoomPlotWindow(bool multi=false, string FileName="");
	bool CreateZoomPlotWindow(CPG *pPG);
	Doc *m_pSimDoc;
	CCoupModelDoc *m_pDoc;
	ChartCug *m_pChartCug;
	PlotInfo *m_pPlotInfo;
	ChartUtil *m_pUtil;
	ChartProp *m_pProp;

	size_t m_CurrentChart;
//	int m_CurrentChartType;
	int m_VarNo;
	int m_PreviousChart;
};

