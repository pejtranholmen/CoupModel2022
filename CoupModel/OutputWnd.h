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

#include ".\Cug\PGDoc\MyPGCug.h"


class COutputWnd : public CDockablePane
{
// Construction
public:
	COutputWnd();
	MyPGCug m_PGgrid;


// Attributes
protected:
	CMFCTabCtrl	m_wndTabs;


	//COutputList m_wndOutputBuild;
	//COutputList m_wndOutputDebug;
	//COutputList m_wndOutputFind;

protected:


// Implementation
public:
	 ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

