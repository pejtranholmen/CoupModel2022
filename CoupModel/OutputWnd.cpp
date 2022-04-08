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

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	
	// Create tabs window:
	//if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	//{
	//	TRACE0("Failed to create output tab window\n");
	//	return -1;      // fail to create
	//}
	

	//const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE ;
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;


	//m_grid1.SetDocument(pDoc, this);
//	m_Modelgrid.CreateGrid(dwStyle,CRect(0,0,0,0),this,52);
//	m_PGgrid.CreateGrid(dwStyle,CRect(0,0,0,0),&m_wndTabs,2);

	m_PGgrid.CreateGrid(dwViewStyle,CRect(0,0,0,0),this,2);

	
	
	//CString strTabName;
	/*strTabName="Model";
	m_wndTabs.AddTab(&m_Modelgrid, strTabName, (UINT)0);*/
	//strTabName="PG File";
	//m_wndTabs.AddTab(&m_PGgrid, strTabName, (UINT)0);
	



	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	m_PGgrid.MoveWindow( 0, 0, cx, cy );


	// Tab control should cover the whole client area:
	//m_wndTabs.SetWindowPos (nullptr, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}
void COutputWnd::OnViewOutput()
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



