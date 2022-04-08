#pragma once
#include "../../std.h"
class TimeSeriePlot;
class CChartViewer;
class ChartProp;
class ChartUtil;
class CPlotStorage;


class ZoomTimeSerie 
{
	public:
		ZoomTimeSerie(void);
		~ZoomTimeSerie(void);
		void Init();
		void OnViewPortChanged();
		void OnChangeWindowWidth();
		void SetChartViewer(CChartViewer *pViewer) {m_pChartViewer=pViewer;};
		void SetPlotStoragePointer(CPlotStorage *pStorage) {m_pPlotStorage=pStorage;};
		void SetWindowWidthValue(int WidthOption) {m_WindowWidthValue=WidthOption;};
		int  GetWindowWidthValue() {return m_WindowWidthValue;};
		void OnCommonday();
		void OnCommonWithinDay();

private:
	TimeSeriePlot *m_pTimeSeriePlot;
	ChartCug *m_pChartCug;
	ChartProp *m_pProp;
	CChartViewer *m_pChartViewer;
	ChartUtil *m_pUtil;
	CPlotStorage *m_pPlotStorage;
	int m_WindowWidthValue;
	bool m_Started;
};
