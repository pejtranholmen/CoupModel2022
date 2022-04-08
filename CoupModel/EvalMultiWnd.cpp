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

#include "stdafx.h"
#include "std.h"

#include "EvalMultiWnd.h"
#include "Resource.h"
//#include "MainFrm.h"
#include "CoupModel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

EvalMultiWnd::EvalMultiWnd()
{
}

EvalMultiWnd::~EvalMultiWnd()
{
}

BEGIN_MESSAGE_MAP(EvalMultiWnd, CDockablePane)
	ON_WM_CONTEXTMENU()	
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_EVALUATEMULTIWND, OnViewProperties)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers



int EvalMultiWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	m_Choice=0;

/*	// Create tabs window:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}


	// Create combo:
	/*const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}

	m_wndObjectCombo.AddString(_T("Chart Options"));
	m_wndObjectCombo.AddString(_T("PG File Data"));
	m_wndObjectCombo.SetCurSel(0);
	

	*/
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE;
	if(!m_ValidationCug.CreateGrid(dwStyle,CRect(0,0,0,0),this,1)) {
		TRACE0("Failed to create PG Grid \n");
		return -1;      // fail to create		;
	}

	
	
	CString strTabName;
	strTabName="Evaluate MultiRu";
	//m_wndTabs.AddTab(&m_ValidationCug, strTabName, (UINT)0);


	//InitPropList();


	// All commands will be routed via this control , not via the parent frame:
//	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void EvalMultiWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
//	m_wndObjectCombo.MoveWindow(0,0,cx,30);
	//m_PGCugData.DestroyWindow();

	SetWindowText(_T("Evaluation Multi"));
	m_ValidationCug.MoveWindow(0,0,cx,cy);



		// Tab control should cover the whole client area:
	//m_wndTabs.SetWindowPos (nullptr, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	m_ValidationCug.ShowWindow( SW_SHOW);
	AdjustLayout();
//}
}

void EvalMultiWnd::OnViewProperties()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != nullptr && pParentBar != nullptr)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}
