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
#include ".\CUG\CreatePG\PGCug.h"
#include ".\CUG\\ChartProperties\ChartCug.h"




class CPropertiesWnd : public CDockablePane
{
// Construction
public:
	CPropertiesWnd();
	afx_msg void OnViewProperties();
	void SetChoice(UINT value) {m_Choice =value;};


// Attributes
public:
	PGCug m_PGCugData;
	ChartCug m_ChartCug;

protected:
	CMFCTabCtrl	m_wndTabs;

//	CFont m_fntPropList;
	CComboBox m_wndObjectCombo;
	UINT m_Choice;

	//CPropertiesToolBar m_wndToolBar;
//	CMFCPropertyGridCtrl m_wndPropList;

// Implementation
public:
	virtual ~CPropertiesWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	

	DECLARE_MESSAGE_MAP()


};

