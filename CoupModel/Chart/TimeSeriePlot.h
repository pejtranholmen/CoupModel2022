#pragma once
#include "DrawDetails.h"
class Doc;
class CCoupModelView;
class CChartViewer;

class CResiduals;
class CPG;
class ZoomTimeSerie;
enum class CHART_TYPES;


class TimeSeriePlot :public DrawDetails	{
	public:
		TimeSeriePlot(void);
		~TimeSeriePlot(void)
			;
	void Init(Doc *pDoc);

	void DrawTimeSeriePlot(CChartViewer *viewer);
	bool OnUpdateSequence(int ArrayIndex=-1);
	bool OnInit(CChartViewer *viewer);
	bool InitMultiRunPlot(Doc *pDoc, int, CHART_TYPES, CChartViewer *view=nullptr);
	bool InitData(Doc *pDoc, int, string filename="", CChartViewer *view=nullptr);
	bool InitMultiPGFilePlot(Doc *pSimDoc);

	bool MakeAllMultiCharts(MultiChart *mc, size_t TotNumPrior);
	bool MakeTimeSeriePlot();

	void *CreateXYChart(); 

	void UpdateGeneralProp();
	bool UpdateViewPortTime();
	void CreateViewPortData(size_t TotNumPrior);
	void UpdateDataDist();
	void AggregateData();
	XYChart * CreateSingleChart();
	MultiChart* CreateMultiCharts();
	void SetFrame_Axis(XYChart *c);
	void AddLineAreaLayer(XYChart *c);
	void ShowChartonScreen_OptionallyCreateFile(MultiChart *mc);
	size_t GetPrior_UpdateNumberOfVariables();
	void SetTimeWindow(double newrange);
	void SetNumSubWindow(int num);
	void SetTimeWindowRange(double mindate, double maxdate);
	void SetRelViewPortWidth(double RelFrac);
	void SetViewPortLeft();
	int  GetNumOfZoomInWindows() {return m_Zoom.NumberOfWindows;};
	void EstimateNumWindows();
	void SetZoomTimeUnit(int unit);
	void updateImageMap(CChartViewer *viewer);
	void SetAggregateRatio(double ratio) {m_Agg.AggregateRatio=int(ratio);};
	void SetZoomLevel();
	void SetCurrentTimeWindow();
	bool CleanPrevCharts() {
		bool Made;
		Made = false;
		if (m_PrevMC != nullptr) {
			delete m_PrevMC;
			m_PrevMC = nullptr;
			Made = true;
		}
		while (m_PrevBaseChart.size()>0) {
			delete m_PrevBaseChart[0];
			m_PrevBaseChart.erase(m_PrevBaseChart.begin());
			Made = true;
		}
		return Made;
	};

	private: // Member pointer
		vector< vector<double>> A, T;
		MultiChart *m_PrevMC;
		vector <XYChart*> m_PrevBaseChart;
		CCoupModelView *m_pView;
		ZoomTimeSerie *m_pZoomTimeSerie;
		bool m_Started;
		bool m_ChartIsMade;
	};

