// ColourPickerCBTestDlg.cpp : implementation file
//

#include "..\stdafx.h"
#include "ColorPickerCB.h"
#include "ColourPickerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColourPickerDlg dialog

CColourPickerDlg::CColourPickerDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CColourPickerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColourPickerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CColourPickerDlg::CColourPickerDlg(int colour)
	: CDialog(CColourPickerDlg::IDD, nullptr)
{
	//{{AFX_DATA_INIT(CColourPickerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_InitColour=colour;
}
void CColourPickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColourPickerDlg)
	DDX_Control(pDX, IDC_COLOUR, m_ctlColour);
	//}}AFX_DATA_MAP
//	DDX_ColorPickerCB( pDX, IDC_COLOUR, m_crColour );
             }

BEGIN_MESSAGE_MAP(CColourPickerDlg, CDialog)
	//{{AFX_MSG_MAP(CColourPickerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColourPickerDlg message handlers

BOOL CColourPickerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	m_ctlColour.InitializeDefaultColors();
	m_ctlColour.SetSelectedColorValue(m_InitColour);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CColourPickerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
		CDialog::OnSysCommand(nID, lParam);
}
void CColourPickerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CColourPickerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

COLORREF CColourPickerDlg::GetColour()
{
	return m_crColour;
}
void CColourPickerDlg::OnOK() 
{
	UpdateData();
	m_crColour=m_ctlColour.GetSelectedColorValue();
	int red=GetRValue(m_crColour);
//CString strText=m_ctlColour.GetSelectedColorName();
	//CString strText;
//	strText.Format( "Colour = RGB( 0x%X, 0x%X, 0x%X )",
//		GetRValue( m_crColour ), GetGValue( m_crColour ), GetBValue( m_crColour ) );

	//AfxMessageBox( strText );
	
	CDialog::OnOK();
}
// .\CUG\ColourPickerDlg.cpp : implementation file
//




// CColourPickerDlg dialog

