// zoomscrolldemoDlg.h : header file
//

#if !defined(AFX_ZOOMSCROLLDEMODLG_H__1D289C1E_2C9E_43C6_B6F4_CDE438051F6C__INCLUDED_)
#define AFX_ZOOMSCROLLDEMODLG_H__1D289C1E_2C9E_43C6_B6F4_CDE438051F6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartViewer.h"
#include <afxmt.h>
#include "afxdtctl.h"
#include "..\Resource.h"
#include "..\CoupModelDoc.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CZoomscrolldemoDlg dialog

//const int sampleSize = 240;

class CZoomscrolldemoDlg : public CDialog
{
public:
// Construction
	CZoomscrolldemoDlg(CWnd* pParent = NULL);	// standard constructor
	~CZoomscrolldemoDlg();


// Dialog Data
	//{{AFX_DATA(CZoomscrolldemoDlg)
	enum { IDD = IDD_ZOOMSCROLLDEMO_DIALOG };
	CDateTimeCtrl	m_StartDate;
	CComboBox	m_Duration;
	CScrollBar	m_VScrollBar;
	CScrollBar	m_HScrollBar;
	CButton	m_XZoomPB;
	CButton	m_PointerPB;
	CChartViewer m_ChartViewer;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZoomscrolldemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL InitData(CCoupModelDoc *pDoc, int, int, CString filename="");
	BOOL InitMultiRunPlot(CCoupModelDoc *pDoc, int, int);
	BOOL InitData(CCoupModelDoc *pDoc, CP* pCP, CPt* pCPt=NULL, int index=-1);
	BOOL AddData(CP*, BOOL, CPt* pCPt=NULL, int index=-1);
	BOOL IsNewViewFunc(CP* );
	CPG* GetSimFile(CCoupModelDoc *pDoc);
protected:
	HICON m_hIcon;
	CCoupModelDoc* m_pDoc;
	int m_numPGPtr;
	// Generated message map functions
	//{{AFX_MSG(CZoomscrolldemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnActivate(UINT nstate, CWnd *pWndOther, BOOL bMinimized);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPointerPB();
	afx_msg void OnZoomInPB();
	afx_msg void OnZoomOutPB();
	afx_msg void OnXZoomPB();
	afx_msg void OnXYZoomPB();
	afx_msg void OnChartViewer();
	afx_msg void OnViewPortChanged();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDatetimechangeStartDate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDuration();
	afx_msg void OnKillfocusDuration();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()

	// Handles pressing the "Enter" key
	virtual void OnOK();

private:
	//
	// Data arrays for the scrollable / zoomable chart.
	// - In this demo, we just use a RanTable object to generate random data for the chart.
	//

	DoubleArray m_timeStamps;
	DoubleArray m_dataSeriesA[32];
	DoubleArray m_dataSeriesB[32];
	DoubleArray m_dataSeriesC[32];
	CStringArray m_Legends;
	long m_noOfPoints, m_noOfExtraPoint;
	BOOL m_PFMeasuredPoints;
	int m_ChartNumber;
	int m_ChartType;
	int m_NumVarSelected;
	int m_ExtraCurveCounter;
	CP *m_pCP;	// pointer to parameter/table object
	CPt *m_pCPt;	// pointer to parameter table object
	int	m_TableIndex;	// index of Pt to be plotted
	CString m_Title;
	CString m_LabelY, m_LabelX;
	int m_PreviousViewFunction;
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

	// Load data into data arrays
	void loadData();
	
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
public:

	afx_msg void OnBnClickedSaveFile();
	BOOL m_CommonScale;
	BOOL m_logscale;
	BOOL m_Cumulated;
	BOOL m_AddOnlyValidRuns;
	BOOL m_ZoomX;
	BOOL m_ZoomXY;
	afx_msg void OnBnClickedCommonscale();
	afx_msg void OnBnClickedCumulate();
	BOOL m_RegLine;
	BOOL m_AddLine;
	COLORREF m_ColorLine[10];
	BOOL m_PointType[20];
	BOOL m_MultiRunPlot;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZOOMSCROLLDEMODLG_H__1D289C1E_2C9E_43C6_B6F4_CDE438051F6C__INCLUDED_)
