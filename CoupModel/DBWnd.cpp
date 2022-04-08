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

#include "DBWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

DBWnd::DBWnd()
{
	MySingle::instance().SetDBBasPointer(&m_DBBasgrid);
}

DBWnd::~DBWnd()
{
}

BEGIN_MESSAGE_MAP(DBWnd, CDockablePane)
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)

END_MESSAGE_MAP()

int DBWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	
	
//	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE ;

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	//m_grid1.SetDocument(pDoc, this);
	m_DBBasgrid.CreateGrid(dwViewStyle,CRect(0,0,0,0),this,1234);
	
	//m_PFgrid2.CreateGrid(dwViewStyle,CRect(0,0,0,0),this,1234);



	
	
	CString strTabName;
	strTabName="PF";

	//m_wndTabs.AddTab(&m_Modelgrid, strTabName, (UINT)0);
	



	return 0;
}

void DBWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	m_DBBasgrid.MoveWindow( 0, 0, cx, cy );

	// Tab control should cover the whole client area:
//	m_wndTabs.SetWindowPos (nullptr, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}
void DBWnd::OnViewOutput()
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


void DBWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	//dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void DBWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

}


