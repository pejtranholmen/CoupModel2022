// Aboutbox.cpp : implementation file
//

#include "stdafx.h"
#include "CoupModel.h"
#include "Aboutbox.h"
#include ".\aboutbox.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Aboutbox dialog


Aboutbox::Aboutbox(CWnd* pParent /*=nullptr*/)
	: CDialog(Aboutbox::IDD, pParent)
{
	//{{AFX_DATA_INIT(Aboutbox)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Aboutbox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Aboutbox)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Aboutbox, CDialog)
	//{{AFX_MSG_MAP(Aboutbox)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP

	ON_UPDATE_COMMAND_UI(32870, OnUpdate32870)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Aboutbox message handlers




void Aboutbox::OnUpdate32870(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}
