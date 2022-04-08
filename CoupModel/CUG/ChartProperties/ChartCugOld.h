// ChartCug.h : interface of the ChartCug class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once





#include "..\ugctrl.h"
#include "..\..\ModelTypes\SimB.h"
#include "..\..\CoupModelDoc.h"
#include "..\CugChartData.h"
#include "..\..\Chart\ChartViewer.h"
#include "..\..\Chart\ChartDataBuf.h"
#include "chartdir.h"

#include "..\..\Chart\RealTime.h"
#include "..\..\Chart\ParameterPlot.h"
#include "..\..\Chart\ModelView.h"
#include "..\..\Chart\TimeSeriePlot.h"
#include "..\..\Chart\TableView.h"
class ChartProp;
class ChartUtil;
class ZoomTimeSerie;




class ChartCug:public CUGCtrl
{
public:
	
	
	bool m_NewResidualFormat;
	CPG m_PGExtra;
	CPG *m_pPGFirst;
	ChartCug();
	~ChartCug();
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
	bool m_Stacked, m_DepthProfile;
	bool m_ScatterContourPoints, m_SplineContourInterPolation;
	bool m_ScaleContourChart;
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


	BOOL InitData(Doc *pDoc, int, int, string filename="", CView *view=NULL);
	void SaveToExtraChartWindow();
	void ShiftExtraChartWindow();
	void SaveToFileExtraChart();
	void RealTimeMode(bool value) {MySingle::instance().GetChartUtilPointer()->m_RealTimeMode=value;};
	void ParameterPlotMode(bool value) {MySingle::instance().GetChartUtilPointer()->m_ParameterPlotMode=value;};
	void TablePlotMode(bool value) {MySingle::instance().GetChartUtilPointer()->m_TablePlotMode=value;};
	BOOL IsMultiRunPlot() {return MySingle::instance().GetChartUtilPointer()->m_MultiRunPlot;};
	BOOL InitMultiRunPlot(Doc *pDoc, int, int, CCoupModelView *view=NULL);
	BOOL InitData(Doc *pDoc, Ps* pCP, P* pCPt=NULL, int index=-1);
	BOOL AddData(Ps*, BOOL, P* pCPt=NULL, int index=-1);
	BOOL IsNewViewFunc(Ps* );
	int	 GetChartNum();
	void DrawTextPlot(CChartViewer *viewer);
	void MakeTextPlot(CString text="");

	BOOL SetDistScale(float min, float max, BOOL log);
	BOOL OnUpdateSequence(int ArrayIndex=-1);

	void AddMainData(DoubleArray data, int index);
	void AddTimeData(DoubleArray data, DoubleArray data2);
	void AddBandData(DoubleArray datasd, DoubleArray datamin, DoubleArray datamax, DoubleArray datap10, DoubleArray datap50,DoubleArray datap90, int index);

	void OnUpdate();
	void Init(Doc *pDoc=NULL);

	void SetMultiRunPlot() {MySingle::instance().GetChartUtilPointer()->m_MultiRunPlot=TRUE;};
	BOOL GetMultiRunPlot() {return MySingle::instance().GetChartUtilPointer()->m_MultiRunPlot;};
	int GetLegendSize() {return MySingle::instance().GetChartUtilPointer()->m_LegendSizePoint;};
	
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
	void SetNumberOfPoints(int value) {m_NumberOfPoints=value;};
	void SetNumberOfBands(int value) {m_NumberOfBands=value;};
	void OnNewSeq2();
	void OnNewCurve();

 // To indicate when extra variables with only accepted have been in chart

	int m_AnnimateStepValue, m_AnnimateValue;
	int m_step;
	bool m_CommonWithDay;
	long m_EquiDistance;


	int m_NumSubCharts;
	int m_NumberOfYears;

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


	void DrawExcelFocusRect(CDC *dc,RECT *rect);

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
	

	int m_aggregateratio;
	int m_SubChartCount;

	int m_NumSelectedPerChart;

	bool m_SortedSeries;
	bool m_ResidualSource;

	
	//{{AFX_MSG(ChartCug)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code!

	
	afx_msg void OnPaint();
	afx_msg void OnActivate(UINT nstate, CWnd *pWndOther, BOOL bMinimized);

	afx_msg void OnChartViewer();
	afx_msg void OnSelchangeDuration();
	afx_msg void OnKillfocusDuration();
	afx_msg void OnTimer(ONTIMER_TYPE	/* nIDEvent	*/);







	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()



	void OnStartAnnimation();
	void OnChangeAnnimationSpeed();
	void OnChangeAnnimationSteps();


	
public:
	
	afx_msg void OnDatetimechangeStartDate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPointerPB();

	afx_msg void OnXZoomPB();
	afx_msg void OnXYZoomPB();

	BOOL m_MakeDataSheet;

	//***** Over-ridable Notify Functions *****

	virtual void OnSetup();
	bool InitMultiPGFilePlot(Doc *pSimDoc);

	virtual void OnSheetSetup(int sheetNumber);

	//movement and sizing
	virtual int  OnCanMove(int oldcol,long oldrow,int newcol,long newrow);
	virtual int  OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow);
	virtual void OnHitBottom(long numrows,long rowspast,long rowsfound);
	virtual void OnHitTop(long numrows,long rowspast);
	
	virtual int  OnCanSizeCol(int col);
	virtual void OnColSizing(int col,int *width);
	virtual void OnColSized(int col,int *width);
	virtual int  OnCanSizeRow(long row);
	virtual void OnRowSizing(long row,int *height);
	virtual void OnRowSized(long row,int *height);

	virtual int  OnCanSizeTopHdg();
	virtual int  OnCanSizeSideHdg();
	virtual int  OnTopHdgSizing(int *height);
	virtual int  OnSideHdgSizing(int *width);
	virtual int  OnTopHdgSized(int *height);
	virtual int  OnSideHdgSized(int *width);
		
	virtual void OnColChange(int oldcol,int newcol);
	virtual void OnRowChange(long oldrow,long newrow);
	virtual void OnCellChange(int oldcol,int newcol,long oldrow,long newrow);
	virtual void OnLeftColChange(int oldcol,int newcol);
	virtual void OnTopRowChange(long oldrow,long newrow);
	virtual void OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum);

	//mouse and key strokes
	virtual void OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed);
	virtual void OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed);
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual void OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed=0);
	virtual void OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnCB_DClicked(RECT *rect,POINT *point,BOOL processed=0);
	
	virtual void OnKeyDown(UINT *vcKey,BOOL processed);
	virtual void OnCharDown(UINT *vcKey,BOOL processed);
	
	//GetCellIndirect notification
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	//SetCell notification
	virtual void OnSetCell(int col,long row,CUGCell *cell);
	
	//data source notifications
	virtual void OnDataSourceNotify(int ID,long msg,long param);

	//cell type notifications
	virtual int OnCellTypeNotify(long ID,int col,long row,long msg,long param);

	//editing
	virtual int OnEditStart(int col, long row,CWnd **edit);
	virtual int OnEditVerify(int col,long row,CWnd *edit,UINT *vcKey);
	virtual int OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);
	virtual int OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow);

	//menu notifications
	virtual void OnMenuCommand(int col,long row,int section,int item);
	virtual int  OnMenuStart(int col,long row,int section);

	//hints
	virtual int OnHint(int col,long row,int section,CString *string);
	virtual int OnVScrollHint(long row,CString *string);
	virtual int OnHScrollHint(int col,CString *string);


	#ifdef __AFXOLE_H__  //OLE must be included

	//drag and drop
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject,int col,long row);
	virtual DROPEFFECT OnDragDrop(COleDataObject* pDataObject,int col,long row);

	#endif

	//sorting
	virtual int OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags);
	
	//DC setup
	virtual void OnScreenDCSetup(CDC *dc,int section);
	
	virtual void OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,RECT *cnrBtn,
		RECT *vScroll,RECT *hScroll,RECT *tabs);

	virtual void OnTabSelected(int ID);
	
	virtual COLORREF OnGetDefBackColor(int section);

	//focus rect setup
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);
	virtual void OnSetFocus(int section);
	virtual void OnKillFocus(int section);

	//column swapping
	virtual BOOL OnColSwapStart(int col);
	virtual BOOL OnCanColSwap(int fromCol,int toCol);

	//trackig window
	virtual void OnTrackingWindowMoved(RECT *origRect,RECT *newRect);

	private :

		CCoupModelView *m_pView;
		Doc *m_pSimDoc;
		bool m_MultiPGFilePlot;
		ChartProp *m_pProp;
		ChartUtil *m_pUtil;
		ZoomTimeSerie *m_pZoomTimeSerie;
			
};
