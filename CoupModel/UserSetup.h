#pragma once

// UserSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserSetup dialog

#include ".\ModelTypes\SimB.h"
#include "afxwin.h"

class CUserSetup : public CDialog
{
// Construction
public:
	CUserSetup(CWnd* pParent = nullptr);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserSetup)
	enum { IDD = IDD_USER_SETUP };
	CSliderCtrl	m_Scale_Font;
	int		m_UserLevel;
	int		m_SimulationPriority;
	int		m_ChartError;
	int		m_LogLikelihoodSwitchValue;
	BOOL	m_ViewDisabled;
	string	m_UserDirectory;
	string	m_Scale_Font_Info;
	BOOL	m_MakeAnnimation;
	BOOL	m_ContinuousHelp;
	BOOL	m_ResidualPlot;
	BOOL	m_ErrorSignal;
	string	m_DataBaseDirectory;
	CString	m_Signature;
	CString	m_UserName;
	string	m_ComputerName;
	BOOL	m_DetailedPlantAnnimation;
	BOOL	m_MakeTextFile;
	string	m_ExcelSumFile;
	BOOL	m_MakeExcelTimeSerieFile;
	int		m_LowResFont;
	int		m_HighResFont;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_ViewScaleFont;

	// Generated message map functions
	//{{AFX_MSG(CUserSetup)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonApply();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonBrowseDb();
	afx_msg void OnButtonCheckSQL();
	afx_msg void OnButtonSqlCreate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_Residual_Data;
	afx_msg void OnBnClickedLogLikelihoodSwitch();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.


