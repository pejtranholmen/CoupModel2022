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

#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutputList window

#include ".\Chart\ChartViewer.h"


class CChartWnd : public CDockablePane
{
// Construction
public:
	CChartWnd();
	CChartViewer m_ChartViewer;	
	void SetChart(BaseChart *pChart, bool newchart=true);
	void ShiftChart();
	void CleanChartList();
	int m_cx,m_cy;
	std::vector<BaseChart*> m_ExtraCharts;
	size_t m_CurrentChart;

// Attributes
protected:
	CMFCTabCtrl	m_wndTabs;




	//COutputList m_wndOutputBuild;
	//COutputList m_wndOutputDebug;
	//COutputList m_wndOutputFind;

protected:


// Implementation
public:
	virtual ~CChartWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnViewChart();

	DECLARE_MESSAGE_MAP()
};

