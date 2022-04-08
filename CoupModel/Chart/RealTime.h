// realtimedemoDlg.h : header file
//



#pragma once


#include "ChartViewer.h"

#include "../Resource.h"


#include <afxmt.h>
#include "afxwin.h"
#include "afxcmn.h"

class CCoupModelView;
class CCoupModelDoc;
class CommonModelInfo;
class Doc;
class CChartViewer;
class ChartProp;
class ChartUtil;

/////////////////////////////////////////////////////////////////////////////
// CRealtimedemoDlg dialog

class RealTime : public CWnd
{
public:
// Construction
	RealTime(CWnd *parent=nullptr);	// standard constructor


	void InitData(Doc *pDoc);
	void ResetValMonitoring();
	void ResetData();
	virtual BOOL OnInit();
	void SetChartCUGPointer(void *p) {m_ChartCugPtr =p;};
	void UpdateData();
	BOOL AddChartVar(int chartno, SimB* Sw, string type, int index);
	BOOL RemoveChartVar(int chartno,int varno);
	BOOL SetChartTitle(int chartno, string text);
	BOOL SetChartType(int chartno, int);
	void SetMultiSimDisplay(BOOL value) {m_MultiSimDisplay=value;};
	BOOL GetMultiSimDisplay() {return m_MultiSimDisplay;};
	int	 GetChartType(int);
	void OnViewPortChanged();
	void *CreateXYChart(int yrow, int xcol, int, int); 
	void CleanRealTimePlot();

	void *m_ChartCugPtr;
	int m_NumCharts;
	int m_NumChartX,m_NumChartY;
	BOOL m_Finnished;

// Dialog Data
	//{{AFX_DATA(CRealtimedemoDlg)
	//enum { IDD = IDD_REALTIMEDEMO_DIALOG };

	//CButton	m_RunPB;

	//CComboBox	m_UpdatePeriod;
//	CComboBox	m_DisplayPeriod;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealtimedemoDlg)
	protected:
//	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//BOOL m_pProp->m_MakeChartTitle,m_pProp->m_NoXAxisScale,m_pProp->m_NoYAxisScale;
	int m_TitleSizePoint,m_LabelSizePoint,m_LegendSizePoint,m_LineWidthPoint,m_SymbolSizePoint;
	int m_LineAxisWidthPoint, m_AxisScalePoint;


	MultiChart *m_pChart;
	HICON m_hIcon;
	size_t m_icount;
	size_t icount;
	double m_prevtime;
	ChartProp *m_pProp;
	CChartViewer *m_pChartViewer;
	ChartUtil *m_pUtil;
	

	BOOL m_Nitrogen, m_Growth;
	BOOL m_MakeAnnimation;
	BOOL m_InitDialog;

	size_t m_NumRealCharts;
	vector< vector<size_t>> m_VarMonitoring;


	vector<string> m_Title, m_GroupA, m_GroupB, m_GroupC, m_GroupD;
	vector<string> m_NameA, m_NameB, m_NameC, m_NameD;
	vector<size_t> m_NumVarPlot, m_PlotType, m_IndexA, m_IndexB, m_IndexC, m_IndexD;
	vector<size_t> m_VarTypeA, m_VarTypeB, m_VarTypeC, m_VarTypeD ;
	vector<SimB*> m_VarA, m_VarB, m_VarC, m_VarD;
	// Generated message map functions
	//{{AFX_MSG(RealTime)

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(ONTIMER_TYPE nIDEvent);
	afx_msg void OnRunPB();
	afx_msg void OnFreezePB();
	afx_msg void OnSelchangeUpdatePeriod();
	afx_msg void OnDeltaposAlarmSpin(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:


	int m_DisplaySize;
	int m_DisplayXSize;
	int m_DisplayYSize;
	int m_DisplayXPlotSize;
	int m_DisplayYPlotSize;
	int m_cx,m_cy;
	BOOL m_MultiSimDisplay;
	int m_NumPar;



	double m_nextDataTime;	// Used by the random number generator to generate real time data.
	int m_threshold;		// The current alarm threshold

	int m_NumVar;
	int m_RecLastDisplayed;

	// Shift new data values into the real time data series 
	void getData();
	
	// Draw chart
	void drawChart(CChartViewer *viewer);
	void drawMultiRunChart(CChartViewer *viewer, int);

	CCoupModelView *m_pView;
	Doc *m_pSimDoc;


public:
	BOOL AddData();
	void OnReDraw();
	void ClickedSaveFile();
	CChartViewer m_ChartViewer1;
	CChartViewer m_ChartViewer2;
	CChartViewer m_ChartViewer3;
	COLORREF m_ColorLine[32];
	COLORREF m_ColorBackground;
	int m_SymbolType[32];
	BOOL m_PointType[32];


	//CProgressCtrl m_pProgress1;
	CString m_RunReport1;
	CString m_RunReport2;
	vector<string> m_ParId, m_ValidId;
	//CProgressCtrl m_pProgress2;
	CString m_ActualDate;
	CStatic m_pDate;
	CStatic m_pRemaining;
	CChartViewer m_ChartViewer0;
	afx_msg void OnStnClickedRunreport2();
	//CSliderCtrl m_SimSpeedControl;
	//afx_msg void OnNMCustomdrawSliderSimSpeed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSteppb2();
	//CButton m_StepPB;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedMultiSimView();
	//CButton m_MultiSimView2;
	afx_msg void OnBnClickedButtonInterupt();
	//CButton m_interupt;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

