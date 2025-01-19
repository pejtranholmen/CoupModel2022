
// MainFrm.h : interface of the CMainFrame class
//
#pragma once
#include ".\WindowNames.h"

#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "ModelWnd.h"
#include "ChartWnd.h"
#include "PropertiesWnd.h"
#include ".\Model\FunctionProperties.h"
#include ".\SoilData\PlotPF\PFCurve.h"
#include ".\DB\DB_Header.h"
#include ".\SoilData\GetSoilStorage.h"
#include "PFWnd.h"
#include "SoilDataWnd.h"

#include "DBWnd.h"
#include "DBDataWnd.h"
#include ".\EvalMultiWnd.h"



class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
   CString m_error_min;
   CString m_warning_min;
   CString m_warning_max;
   CString m_error_max;
   CString m_unit;
   CString m_text;
   CDocument* GetMyActiveDocument();
   void SetNoDocumentValid(bool value=false);
   bool IsDocumentValid();
   FunctionProperties m_Func;
   PFCurve m_PlotPF;
   GetSoilStorage m_SoilStorage;
   DB_Header m_DB;
protected:
	CDocument* m_ActDoc;
	
	bool m_DocumentValid;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);


// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void	UpdatePaneText(int, string); 
	void	UpdateLocalWindowText(string, WINDOW_NAMES window=WINDOW_PG);
	void	UpdateProgress(int, float relfrac); 
	void	UpdateResidualProgress(int var, int numvar, float relfrac, size_t Transform = 0, size_t actual_ensemble = string::npos);
	afx_msg void OnUpdateBound0(CCmdUI *pCmdUI); // added by DSM
	afx_msg void OnUpdateBound1(CCmdUI *pCmdUI); // added by DSM
	afx_msg void OnUpdateBound2(CCmdUI *pCmdUI); // added by DSM
	afx_msg void OnUpdateBound3(CCmdUI *pCmdUI); // added by DSM
	afx_msg void OnUpdateBound4(CCmdUI *pCmdUI); // added by DSM
	afx_msg void OnUpdateUnit(CCmdUI *pCmdUI); // added by DSM

	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;


	EvalMultiWnd	  m_wndEvalMulti;
	CFileView         m_wndFileView;
	CClassView        m_wndClassView;
//	CChartWnd         m_wndChartView;
	CPropertiesWnd    m_wndProperties;
	CModelWnd		  m_wndModel;
	COutputWnd        m_wndPG;
	CPFWnd			  m_wndPF;
	SoilDataWnd		  m_wndSoilData;
	DBWnd			m_wndDB;
	DBDataWnd		m_wndDBData;
	

protected:  // control bar embedded members
/*	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;*/




// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	//afx_msg void OnViewCaptionBar();
	//afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
};


