// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// CoupModel.h : main header file for the CoupModel application
//
#pragma once
#include "./stdafx.h"
#include "./std.h"
#include "./PG/PGFileArchive.h"
#include "./NewBase/SimArchive.h"
#include "./NewBase/CommonModelInfo.h"
#include "./Chart/ChartProperties/ChartProp.h"
#include "./Chart/PlotDesignInfo/PlotInfo.h"
#include "./Chart/ChartProperties/ChartUtil.h"
#include "./Chart/ZoomUtil/ZoomTimeSerie.h"


#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


static TCHAR BASED_CODE szIniFileSection[] = _T("Most Recent Parameter File");
static TCHAR BASED_CODE szIniFileEntry[] = _T("File");
struct Setup {
	int UserLevel;
	int SimulationPriority;
	bool ViewDisable;
	bool ContinuousHelp;
};
class CCoupModelDoc;
class CPG;
class PGFileArchive;



// CCoupModelApp:
// See CoupModel.cpp for the implementation of this class
//

class CCoupModelApp : public CWinAppEx
{
public:
	void EnsureInitialWindowSettings();
	CCoupModelApp();
	CString m_AppString;
	Setup m_Setup;
	CMultiDocTemplate* m_pDocTemplate;
	void CreateFileNew(CString);
	BOOL OnFileOpen(LPCTSTR lpszFileName, BOOL SQL);
	BOOL OnFileOpen(LPCTSTR lpszFileName);
	void CloseSimDocument();



protected:
	CString GetDocPathFromIniFile();
	void OpenSimDocument(LPCTSTR lpszFilter);
	void OnHelp();
	PGFileArchive m_pPGArchive;
	SimArchive m_pSimDocArchive;
	CommonModelInfo m_CommonModel;
	ChartProp m_ChartProp;
	ChartUtil m_ChartUtil;
	PlotInfo m_PlotInfo;
	ZoomTimeSerie m_ZoomChart;



// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	bool CheckIfFirstRun(); // Kontrollera om det är första körningen
	void RunPowerShellScript(); // Kör PowerShell-skriptet vid första start
// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	virtual void OnAppExit();

	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnMultiFileOpen();
	afx_msg void OnBaseSetup();


	DECLARE_MESSAGE_MAP()
	afx_msg void OnUsersetting();
	afx_msg void OnToolsOptions();
};

extern CCoupModelApp theApp;


