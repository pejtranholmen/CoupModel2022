// ColourPickerCBTestDlg.h : header file
//

#if !defined(AFX_COLOURPICKERCBTESTDLG_H__ADADE595_BF9A_11D3_AF3C_20114DC1015C__INCLUDED_)
#define AFX_COLOURPICKERCBTESTDLG_H__ADADE595_BF9A_11D3_AF3C_20114DC1015C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorPickerCB.h"
#include "..\Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CColourPickerDlg dialog

class CColourPickerDlg : public CDialog
{
// Construction
public:
	CColourPickerDlg(CWnd* pParent = nullptr);	// standard constructor
	CColourPickerDlg(int color);	
	COLORREF GetColour();

// Dialog Data
	//{{AFX_DATA(CColourPickerDlg)
	enum { IDD = IDD_COLOURDLG };
	CColorPickerCB	m_ctlColour;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColourPickerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	COLORREF m_crColour;
	COLORREF m_InitColour;

	// Generated message map functions
	//{{AFX_MSG(CColourPickerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOURPICKERCBTESTDLG_H__ADADE595_BF9A_11D3_AF3C_20114DC1015C__INCLUDED_)
#pragma once


