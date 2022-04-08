#pragma once


#include "..\Resource.h"

/////////////////////////////////////////////////////////////////////////////
// LegendChangeDlg dialog

class LegendChangeDlg : public CDialog
{
// Construction
public:
	LegendChangeDlg(CWnd* pParent = nullptr);	// standard constructor
	LegendChangeDlg(string prevtext);	
	string m_newtext;

// Dialog Data
	//{{AFX_DATA(LegendChangeDlg)
	enum { IDD = IDD_CHANGE_VARNAME};

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LegendChangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(LegendChangeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



