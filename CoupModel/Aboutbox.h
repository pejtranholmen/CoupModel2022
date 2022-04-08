


#pragma once

// Aboutbox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Aboutbox dialog

class Aboutbox : public CDialog
{
// Construction
public:
	Aboutbox(CWnd* pParent = nullptr);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Aboutbox)
	enum { IDD = IDD_ABOUTBOX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Aboutbox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Aboutbox)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnUpdate32870(CCmdUI *pCmdUI);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

