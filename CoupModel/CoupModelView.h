// CoupModelView.h : interface of the CCoupModelView class
//
/////////////////////////////////////////////////////////////////////////////


#pragma once


#include "math.h"
#include "memory.h"
#include ".\Resource.h"
#include ".\Chart\ChartViewer.h"
class ChartProp;


				
class CCoupModelView : public CFormView
{
protected: // create from serialization only
	CCoupModelView();
	DECLARE_DYNCREATE(CCoupModelView);
 
// Attributes
public:
	enum	{IDD=IDD_FORMVIEW};
	CCoupModelDoc* GetDocument();

	CChartViewer m_ChartViewer;
//	MyCug m_grid1;
protected:
	
	int m_printPage;
	BOOL m_isPrinting;
	CRect m_drawRect;
	int m_cx, m_cy;
	BOOL m_OKWriteFile;
// Operations
public:
 // Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoupModelView)
	public:
	virtual void DoDataExchange(CDataExchange *pDX);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = nullptr);
	void OnPgfileNew();
	void OnPgfileOpen();



	//virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = nullptr);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct

	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

	//}}AFX_VIRTUAL

// Implementation
public:

	bool m_RealTimeMode;
	void SetRealTimeMode(bool realtimemode) { m_RealTimeMode=realtimemode;};
	void writeStatus(CString &);

	void MakeChart(BaseChart *pChart=nullptr);
	int GetXViewSize() {return m_cx;};
	int GetYViewSize() {return m_cy;};
	BOOL m_OverLayPF;
	BOOL m_UpdateGrid;

	CString m_legendtmp;

	long m_longSimTime;
	virtual ~CCoupModelView();

	BOOL m_View_LeftMove,m_View_RightMove,m_View_SumVarMode;
	int View_VariableNumber,View_OverLayVariableNumber, View_NumXPoints;
	float m_ViewYRange;
	int m_SumType1,m_SumGroup1,m_SumName1,m_SumIndex1;
	int m_SumType2,m_SumGroup2,m_SumName2,m_SumIndex2;

	int m_CUGDisplayed;
private:
	ChartProp *m_pChartProp;

protected:

	void OnOpenRapport();
	void CreateTreeView();
	BOOL m_HistoryDiff;
	int m_pageSW_Specb, m_pageSW_Spec, m_pageSW_Tech, m_pageTab, m_pageTab_B,m_pageP, m_pageP_B;
	int m_pageX, m_pageT, m_pageG, m_pageD,m_pageX_B, m_pageT_B, m_pageG_B, m_pageD_B ;
	BOOL m_biotic;

	CUIntArray	m_SetValues;
	CUIntArray	m_SetDocValues;
	CUIntArray  m_DocRunNumbers;
	CString m_PF_Label;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   
// Generated message map functions
protected:
	

	LOGFONT logFont;
	LOGFONT logFonta;
	LOGFONT logFont_i;
	int m_pageheight;
	BOOL m_IsPrinting;
	int m_nextpageposition;
	int m_margtop;
	int m_Margbottom;
	int m_position;
	int m_intScaleY;
	int m_intScaleX;
	int m_intScaleFont;
	void mProgbar(CDC* pDc, float);
	int m_NumLines;
	int m_plantscale;


	//{{AFX_MSG(CCoupModelView)

	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


	afx_msg void OnRunOptions();
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);


	afx_msg void OnUpdateViewPage(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnDestroy();




	//}}AFX_MSG
	afx_msg LRESULT OnHotHit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPropertyChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnChartClick(UINT nID);
	afx_msg void OnChartDoubleClick(UINT nID);
	afx_msg void OnViewPortChanged();
	void OnUpdateSimStart(CCmdUI* pCmdUI);
	void OnSimStart();
	void OnUpdateSimMultiStart(CCmdUI* pCmdUI);
	void OnSimMultiStart();
	void OnUpdateDocNew(CCmdUI* pCmdUI);

	void OnDocNew();
	void OnHelp();
	void OnLinkDocuments();
	void OnUnLinkDocuments();
	void OnUpdateLink(CCmdUI* pCmdUI);
	void OnUpdateUnLink(CCmdUI* pCmdUI);
	void OnChartFileSave();
	void OnChartFileExtraSave();
	void OnChartExtra();
	void OnChartCleanExtra();
	void OnChartShift();
	void OnUpdatePgfileNew(CCmdUI* pCmdUI);
	void OnUpdatePgfileOpen(CCmdUI* pCmdUI);
	void OnUpdateChartFileSave(CCmdUI* pCmdUI);
	void OnUpdateChartFileExtraSave(CCmdUI* pCmdUI);
	void OnUpdateChartExtra(CCmdUI* pCmdUI);
	void OnUpdateChartShift(CCmdUI* pCmdUI);
	void OnUpdateChartCleanExtra(CCmdUI* pCmdUI);
	void OnUpdateXMinus(CCmdUI* pCmdUI);
	void OnUpdateYMinus(CCmdUI* pCmdUI);
	void OnXPlus();
	void OnXMinus();
	void OnYPlus();
	void OnYMinus();
	void OnZoomIn();
	void OnZoomOut();
	void OnZoomHand();
	void OnZoomForward();
	void OnZoomBackward();
	void OnUpdateZoomIn(CCmdUI* pCmdUI);
	void OnUpdateZoomOut(CCmdUI* pCmdUI);
	void OnUpdateZoomHand(CCmdUI* pCmdUI);
	void OnUpdateZoomForward(CCmdUI* pCmdUI);
	void OnUpdateZoomBackward(CCmdUI* pCmdUI);
	void OnUpdateValFile(CCmdUI* pCmdUI);
	void OnUpdateValView(CCmdUI* pCmdUI);
	void OnUpdateValAddTsv(CCmdUI* pCmdUI);
	void OnUpdateValAddSv(CCmdUI* pCmdUI);
	void OnUpdateValUnLock(CCmdUI* pCmdUI);
	void OnUpdateCreatePGFile(CCmdUI* pCmdUI);
	void OnValFile();
	void OnValView();
	void OnValAddTsv();
	void OnValAddSv();
	void OnValUnLock();
	void OnCreatePGFile();
	void OnMultiNewCurve();
	void OnMultiNewRun();
	void OnMultiNewRunAll();
	void OnUpdateMultiNewCurve(CCmdUI* pCmdUI);
	void OnUpdateMultiNewRun(CCmdUI* pCmdUI);
	void OnUpdateMultiNewRunAll(CCmdUI* pCmdUI);	
	DECLARE_MESSAGE_MAP()
public:



};


