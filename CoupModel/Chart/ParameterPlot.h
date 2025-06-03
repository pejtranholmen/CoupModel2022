
//
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


// CParameterPlotDialog dialog
#pragma once

#include "ChartViewer.h"
#include "./ChartData\ChartData.h"
#include <afxmt.h>
#include "afxdtctl.h"

#include "afxwin.h"
#include "afxcmn.h"

/////////////////////////////////////////////////////////////////////////////
// ParameterPlot dialog

//const int sampleSize = 240;
class Func;
class PFCurve;
class Doc;
class ChartProp;
class ChartViewer;
class ChartUtil;

class ParameterPlot : public ChartData, public CWnd
{
public:
// Construction
	ParameterPlot(CWnd* pParent = nullptr);	// standard constructor
	~ParameterPlot();


// Dialog Data
	//{{AFX_DATA(ParameterPlot)
	CDateTimeCtrl	m_StartDate;
	CComboBox	m_Duration;
	CScrollBar	m_VScrollBar;
	CScrollBar	m_HScrollBar;
	CButton	m_XZoomPB;
	CButton	m_PointerPB;
	CChartViewer m_ChartViewer;

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ParameterPlot)
	protected:

	//}}AFX_VIRTUAL

// Implementation
public:
	bool InitData(Doc *pDoc, SimB* pSimB, Tab *pTab=nullptr, size_t index=0, size_t FuncNo=0);
	bool AddData(bool, bool NewValue=FALSE);
	vector<double> GetYVector(size_t curve,size_t tabindex=0, bool test=false);
	bool IsNewViewFunc(Ps* );
	bool OnInit();
	void ApplyNewvalue();
	Ps *m_pCP,*m_pCP1, *m_pCP2,*m_pCP3, *m_pCP4;	// pointer to parameter/table object
	P *m_pCPt,*m_pCPt1, *m_pCPt2, *m_pCPt3, *m_pCPt4;	// pointer to parameter table object
	size_t	m_TableIndex;	// index of Pt to be plotted
	int m_ParNum;
	bool m_PlotDefault, m_PlotPrevious, m_PlotNewValue;
	void SetChartCUGPointer(void *p) {m_ChartCugPtr =p;};
	void SetOriginalViewFunction(int);
	Func *GetCurrentFuncPointer() {return m_Func;};
	void *m_ChartCugPtr;
	// Load data into data arrays
	bool loadData();
	bool m_CommonScale;
	bool m_logscale;
	bool m_Cumulated;
	bool m_ZoomX;
	bool m_ZoomXY;
	size_t m_MaxLayer;

	//float m_NewPlotValue, m_NewPlotValue2, m_NewPlotValue3, m_NewPlotValue4;
	vector< std::vector<double>> m_NewPlotValue, m_ResetPlotValue, m_PreviousValue;
	vector<P*> m_P;
	vector<Ps*> m_Ps;
	Tab* m_pParTable;
	PLOT_PARAM m_plotp;
	void Clean_ParameterPlot();
	PFCurve* p_PFCurve;

protected:
	HICON m_hIcon;
	bool m_FixedLegend;

	
	Func *m_Func;
	PLOT_PARAM_DEP m_Par_Dep;

	int m_numPGPtr;
	
	//ChartCug m_grid;
	// Generated message map functions
	//{{AFX_MSG(ParameterPlot)


	//}}AFX_MSG
	
//	DECLARE_MESSAGE_MAP()



private:
	vector<XYChart*> m_XY_Chart;
	CCoupModelView *m_pView;
	Doc *m_pSimDoc;
	ChartProp *m_pProp;
	CChartViewer *m_pChartViewer;
	ChartUtil *m_pUtil;
	bool m_ThermalReCalc;
	bool IsThermalRelc();
	void MakeSoilReCalc();
	//
	// Data arrays for the scrollable / zoomable chart.
	// - In this demo, we just use a RanTable object to generate random data for the chart.
	//
	bool m_Displayed;
	DoubleArray m_timeStamps;
	DoubleArray m_dataSeriesA[32];
	DoubleArray m_dataSeriesB[32];
	DoubleArray m_dataSeriesMIN[32];
	DoubleArray m_dataSeriesMAX[32];
	DoubleArray m_dataSeriesC[32];
	vector<string> m_Legends, m_ExtraLegends;
	size_t m_noOfPoints, m_noOfExtraPoint;
	bool m_PFMeasuredPoints;
	int m_ChartNumber;
	CHART_TYPES m_ChartType;
	int m_NumVarSelected;
	int m_NumVarBandSelected;
	int m_ExtraCurveCounter;
	string m_Title;
	string m_LabelY, m_LabelX;
	int m_PreviousViewFunction;
	bool m_InitViewFunction;
	int m_ResetViewFunction;
	int m_CurveCounter;
	int m_ViewCounter;
	int m_ind;

	
	int m_extBgColor;			// The default background color

	double m_minDate;			// The earliest date (in chartTime unit)
	double m_dateRange;			// The overall duration (in seconds)
	double m_maxValue;			// The maximum overall y scale
	double m_minValue;			// The minimum overall y scale
	double m_maxXValue;			// The maximum overall x scale
	double m_minXValue;			// The minimum overall x scale
	double m_XRange;			// The overall duration i x scale

	double m_currentDuration;	// The visible duration of the view port (in seconds)
	double m_minDuration;		// The maximum zoom-in limit (in seconds)

	double m_currentXRange;	// The visible duration of the view port ()
	double m_minXRange;		// The maximum zoom-in limit ()
	int m_DisplaySize;
	int m_DisplayXSize;
	int m_DisplayYSize;
	int m_DisplayXPlotSize;
	int m_DisplayYPlotSize;
	int m_cx,m_cy;
	bool m_InitDialog;

	// Draw chart
	void drawChart(CChartViewer *viewer);
	// Update image map 
	void updateImageMap(CChartViewer *viewer);

	// Moves the scroll bar when the user clicks on it
	double moveScrollBar(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// Validate the duration entered by the user
	void validateDuration(const CString &text);

	// utility to get default background color
	int getDefaultBgColor();
	// utility to load icon resource to a button
	void loadButtonIcon(int buttonId, int iconId, int width, int height);

};


