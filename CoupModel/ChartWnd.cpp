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
//#include "std.h"

#include "ChartWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

CChartWnd::CChartWnd()
{
	m_CurrentChart=0;
	int v=m_ExtraCharts.size();
	MySingle::instance().SetExtraChartWndPointer(this);
}

CChartWnd::~CChartWnd()
{
}

BEGIN_MESSAGE_MAP(CChartWnd, CDockablePane)
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_CHARTWND, OnViewChart)
END_MESSAGE_MAP()

int CChartWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
/*	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}
	*/
	const DWORD dwStyle = WS_CHILD | WS_VISIBLE|SS_BITMAP;

	if(!m_ChartViewer.Create(_T(""),dwStyle, rectDummy, this, 22))
	{
		TRACE0("Failed to create ChartViewer window\n");
		return -1;      // fail to create;

	}


	return 0;
}
void CChartWnd::SetChart(BaseChart *pChart, bool newchart)
{
	m_ChartViewer.setChart(pChart);
	if(newchart) {
		m_ExtraCharts.push_back(pChart);
		m_CurrentChart=m_ExtraCharts.size()-1;
	}
	m_ChartViewer.updateViewPort(true, true);
}

void CChartWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	m_cx=cx;
	m_cy=cy;
	m_ChartViewer.SetWindowPos(nullptr, 0, 0, cx, cy,22);

	m_ChartViewer.ShowWindow(TRUE);
	m_ChartViewer.updateViewPort(true, true);

}
void CChartWnd::OnViewChart()
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
void CChartWnd::ShiftChart()
{
	BaseChart *pCh;
	int b=m_ExtraCharts.size();
	if(m_CurrentChart==(b-1))
		m_CurrentChart=0;
	else if(m_CurrentChart<m_ExtraCharts.size()-1)
		m_CurrentChart++;
	pCh=m_ExtraCharts[m_CurrentChart];
	SetChart(pCh, false);

}
void CChartWnd::CleanChartList()
{
	while(m_ExtraCharts.size()>0) {
		delete m_ExtraCharts[0];
		m_ExtraCharts.erase(m_ExtraCharts.begin());
	}
}



