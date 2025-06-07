// ColourPickerCBTestDlg.cpp : implementation file
//

#include "..\stdafx.h"
#include "LegendChangeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LegendChangeDlg dialog

LegendChangeDlg::LegendChangeDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(LegendChangeDlg::IDD, pParent)
{

}
LegendChangeDlg::LegendChangeDlg(std::string  prevtext)
	: CDialog(LegendChangeDlg::IDD, nullptr)
{
	m_newtext = prevtext;

}
void LegendChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

    }

BEGIN_MESSAGE_MAP(LegendChangeDlg, CDialog)
	//{{AFX_MSG_MAP(LegendChangeDlg)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LegendChangeDlg message handlers

BOOL LegendChangeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_EDIT1)->SetWindowText(CString(m_newtext.c_str()));
	GetDlgItem(IDC_EDIT1)->RedrawWindow();

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.



// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.



void LegendChangeDlg::OnOK() 
{
	UpdateData(TRUE);
	CString text;
	GetDlgItem(IDC_EDIT1)->GetWindowText(text);
	m_newtext= CT2A(text).m_psz;
	
	CDialog::OnOK();
}
// .\CUG\LegendChangeDlg.cpp : implementation file
//




// LegendChangeDlg dialog

