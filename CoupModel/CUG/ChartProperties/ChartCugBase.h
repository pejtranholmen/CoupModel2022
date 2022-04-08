// ChartCugBase.h : interface of the ChartCugBase class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once





#include "..\PEJugctrl.h"
#include "..\..\ModelTypes\SimB.h"
#include "..\..\CoupModelDoc.h"
#include "..\CugChartData.h"
#include "..\..\Chart\ChartViewer.h"
#include "chartdir.h"

#include "..\..\Chart\RealTime.h"
#include "..\..\Chart\ParameterPlot.h"
#include "..\..\Chart\ModelView.h"
#include "..\..\Chart\TimeSeriePlot.h"
#include "..\..\Chart\TableView.h"
class ChartProp;
class ChartUtil;
class ZoomTimeSerie;
struct GRIDCHAR {
	size_t rows;
	size_t cols;
};




class ChartCugBase:public PEJUGCtrl
{
public:
	
	
	bool m_NewResidualFormat;
	CPG m_PGExtra;
	CPG *m_pPGFirst;
	ChartCugBase();
	~ChartCugBase();



	RealTime m_RealTime;
	ParameterPlot m_ParameterPlot;
	ModelView m_ModelView;
	TimeSeriePlot m_TimeSeriePlot;
	TableView m_TableView;
	CWnd *m_pMainframe;
	BOOL m_bDrawExcelFocus;
	int m_PreviousMode;
	bool m_PGRepit;
	BOOL  m_ChangeAllRunNo;

	bool m_NoChange;
	MultiChart *m_PrevMC;
	vector <XYChart*> m_PrevBaseChart;

	bool m_boxcox;
	float m_box_power, m_box_offset;
	bool m_EntireDataSet;
	bool m_CommonDayNo;
	bool m_DerivateValues;
	bool m_ScatterContourPoints, m_SplineContourInterPolation;

	CChartViewer *m_pChartViewer;
	int m_ZoomLevel;
	int m_Hpos;
	CString m_TextPlot;
	vector <Sw*>m_SwitchSet;
	bool m_HeatEq;
	int m_WaterEq, m_NC,m_HBV,m_Evapo,m_Gw, m_Irrigation, m_PlantType, m_SaltTracer, m_SnowPack, m_SoilVapour, m_LateralInput;

	int m_IndepMulti_Index, m_IndepMulti2_Index;;

	BOOL m_ScaleSet,m_LogScale;
	BOOL m_NormalDist;
	float m_DistMin, m_DistMax;

 // To indicate when extra variables with only accepted have been in chart

	int m_AnnimateStepValue, m_AnnimateValue;
	int m_step;
	bool m_CommonWithDay;
	long m_EquiDistance;


	size_t m_NumUsedSubCharts;
	size_t m_NumberOfYears;

	BOOL m_AnimationRunning;


	float m_SecondMin,m_SecondMax;

protected:

	//
	BOOL m_Started;


	string m_Legendtxt, m_unit1, m_IndepId, m_ytitle2, m_unit2, m_DocNumId;
	int  m_color_ivar, m_colorbox, m_color_range, m_color_edge, m_color_dist, m_isymb;
	LineLayer *m_LineLayer;
	TrendLayer *m_pTrendLayer;
	double m_firstDate, m_lastDate;
	double m_MaxIndep2, m_MinIndep2;
	XYChart *m_c;
	LineLayer *m_layer2;
	LineLayer *m_BandLayer;
	AreaLayer *m_pAreaLayer;
	BoxWhiskerLayer *m_errLayer;
	BoxWhiskerLayer *m_errLayer2;
	ScatterLayer *m_ScatterLayer;
	bool m_BandLayerExist;


	CFont  m_defFont;
	CFont  m_smallFont;
	CFont  m_largeFont;
	CFont m_extralargeFont;
	CFont m_font;

	CStringArray m_Legends;
	long m_noOfPoints, m_noOfExtraPoint;
	long m_noOfCommonPoints;
	BOOL m_PFMeasuredPoints;

	int m_NumVarSelected;
	int m_NumVarBandSelected;
	int m_ExtraCurveCounter;
	Ps *m_pCP;	// pointer to parameter/table object
	P *m_pCPt;	// pointer to parameter table object
	int	m_TableIndex;	// index of Pt to be plotted

	int m_PreviousViewFunction;
	int m_CurveCounter;
	int m_ViewCounter;
	int m_ind;
	
	int m_extBgColor;			// The default background color

	double m_minDate;			// The earliest date (in chartTime unit)
	double m_maxDate;			// The Latest date (in chartTime unit)
	double m_dateRange;			// The overall duration (in seconds)
	double m_maxValue;			// The maximum overall y scale
	double m_minValue;			// The minimum overall y scale
	double m_maxValueRight;			// The maximum overall y scale
	double m_minValueRight;			// The minimum overall y scale
	
	double m_maxXValue;			// The maximum overall x scale
	double m_minXValue;			// The minimum overall x scale
	double m_XRange;			// The overall duration i x scale
	double m_interv1,m_interv2;

	double m_currentDuration;	// The visible duration of the view port (in seconds)
	double m_minDuration;		// The maximum zoom-in limit (in seconds)

	double m_currentXRange;	// The visible duration of the view port ()
	double m_minXRange;		// The maximum zoom-in limit ()

	BOOL m_Band, m_NewDist;
	string m_st, m_test, m_unit;
	HICON m_hIcon;
	CCoupModelDoc* m_pDoc;
	CPG *m_pPG;
	int m_numPGPtr;
	string m_ytitle1, m_xtitle;

	long m_NumberofMoves;
	short m_interval;


	int m_NumberOfPoints;
	int m_NumberOfBands;

	CugChartData m_data;

	long m_col,m_row;
	BOOL m_Data;
	Tab* m_pCTab;

	Ps *m_pPar;
	int m_TabIndex;
	int m_ActNumValues;
	COLORREF m_ColorClick, m_ColorChanged, m_ColorHeading,	m_NoColor;
	int m_Mode;

	CDialog *m_pDlg;
	int *m_NumValid;
	int m_colpos;
	int m_CurrentTopRow;
	bool m_CreatePG;
	bool m_ToSavePGStruc;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL
	afx_msg void OnTimer(ONTIMER_TYPE	/* nIDEvent	*/);
	DECLARE_MESSAGE_MAP()
	int m_aggregateratio;
	int m_SubChartCount;

	int m_NumSelectedPerChart;

	bool m_SortedSeries;
	bool m_ResidualSource;

public:

	
	BOOL m_MakeDataSheet;

	protected :

		CCoupModelView *m_pView;
		Doc *m_pSimDoc;
		bool m_MultiPGFilePlot;
		ChartProp *m_pProp;
		ChartUtil *m_pUtil;
		ZoomTimeSerie *m_pZoomTimeSerie;
			
};
