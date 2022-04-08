// Welcome.cpp : implementation file
//

#include "stdafx.h"
#include "CoupModel.h"
#include "Welcome.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWelcome dialog


CWelcome::CWelcome(CWnd* pParent /*=nullptr*/)
	: CDialog(CWelcome::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWelcome)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWelcome::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWelcome)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWelcome, CDialog)
	//{{AFX_MSG_MAP(CWelcome)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWelcome message handlers
