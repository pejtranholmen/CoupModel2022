// UserSetup.cpp : implementation file
//

#include "stdafx.h"
#include "CoupModel.h"
#include ".\CUG\SimDoc\MyCug.h"

#include ".\usersetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CUserSetup dialog


CUserSetup::CUserSetup(CWnd* pParent /*=nullptr*/)
	: CDialog(CUserSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserSetup)
	CWinApp* pApp = AfxGetApp();
	((CCoupModelApp*)pApp)->m_Setup.ContinuousHelp=FALSE;


	m_UserLevel = -1;
	m_SimulationPriority = -1;
	m_ChartError = -1;
	m_ViewDisabled = FALSE;
	m_UserDirectory = "";
	m_Scale_Font_Info = "";
	m_MakeAnnimation = FALSE;
	m_ContinuousHelp = FALSE;
	m_ResidualPlot = FALSE;
	m_ErrorSignal = FALSE;
	m_DataBaseDirectory = "C:\\CoupModel\\SoilProperties\\";
	m_Signature = "";
	m_UserName = "";
	m_DetailedPlantAnnimation = FALSE;
	m_MakeTextFile = FALSE;
	m_ExcelSumFile = "CoupSummary.xls";
	m_MakeExcelTimeSerieFile = FALSE;
	m_LogLikelihoodSwitchValue=FALSE;
	m_LowResFont = 16;
	m_HighResFont = 22;

	//}}AFX_DATA_INIT

#define INFO_BUFFER_SIZE 32767


  TCHAR  infoBuf[INFO_BUFFER_SIZE];
  DWORD  bufCharCount = INFO_BUFFER_SIZE;

 
  // Get and display the name of the computer. 
  bufCharCount = INFO_BUFFER_SIZE;
  if(GetComputerName( infoBuf, &bufCharCount ) )
    m_ComputerName=CT2A(infoBuf).m_psz; 
	MFC_Util::WriteProfileString( "ComputerName", m_ComputerName);




}

void CUserSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);



	//{{AFX_DATA_MAP(CUserSetup)

	DDX_Radio(pDX, IDC_RADIO1, m_UserLevel);
	DDX_Radio(pDX, IDC_RADIO4, m_SimulationPriority);

	//DDX_Text(pDX, IDC_USER_PATH, CString(m_UserDirectory.c_str()));
	//DDV_MaxChars(pDX, CString(m_UserDirectory.c_str()), 150);

	DDX_Check(pDX, IDC_CHECK2, m_MakeAnnimation);
	DDX_Check(pDX, IDC_CHECK3, m_ContinuousHelp);

	DDX_Text(pDX, IDC_USER_PATH2, CString(m_DataBaseDirectory.c_str()));
	DDV_MaxChars(pDX, CString(m_DataBaseDirectory.c_str()), 150);
	DDX_Text(pDX, IDC_SIGNATURE, m_Signature);
	DDV_MaxChars(pDX, m_Signature, 4);
	DDX_Text(pDX, IDC_USER_NAME, m_UserName);
	DDV_MaxChars(pDX, m_UserName, 40);


	DDX_Text(pDX, IDC_HIGH_RESFONT, m_HighResFont);
	DDX_Text(pDX, IDC_LOW_RESFONT, m_LowResFont);

	//}}AFX_DATA_MAP


}


BEGIN_MESSAGE_MAP(CUserSetup, CDialog)
	//{{AFX_MSG_MAP(CUserSetup)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_DB, OnButtonBrowseDb)

	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnLvnItemchangedList1)

	ON_BN_CLICKED(IDC_CHECK10, OnBnClickedLogLikelihoodSwitch)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserSetup message handlers

void CUserSetup::OnOK() 
{
	// TODO: Add extra validation here
	CWinApp* pApp = AfxGetApp();
	UpdateData(TRUE);

   	CDialog::OnOK();


    MFC_Util::WriteProfileInt( "ContinuousHelp", m_ContinuousHelp);

    MFC_Util::WriteProfileInt( "UserLevel", m_UserLevel);

    MFC_Util::WriteProfileInt( "SimulationPriority", m_SimulationPriority);
    MFC_Util::WriteProfileInt( "ViewDisabled", m_ViewDisabled);
    MFC_Util::WriteProfileInt( "MakeAnnimation", m_MakeAnnimation);
    MFC_Util::WriteProfileInt( "DetailedPlantAnnimation", m_DetailedPlantAnnimation);


	MFC_Util::WriteProfileInt("LowResFont", m_LowResFont);
	MFC_Util::WriteProfileInt("HighResFont", m_HighResFont);

    MFC_Util::WriteProfileString( "UserDirectory", m_UserDirectory);
    MFC_Util::WriteProfileString( "DataBaseDirectory", m_DataBaseDirectory);
    MFC_Util::WriteProfileString( "UserName", string(CT2A(m_UserName).m_psz));
    MFC_Util::WriteProfileString( "Signature", string(CT2A(m_Signature).m_psz));
	MySingle::instance().GetCugPointer()->ReDefineDefaultFont();


 /*	PARAMETERS_mp_NUMOFLETTERS=m_UserDirectory.GetLength();
   	//Copy the truncated filename into the FORTRAN array 
	for(int i=0;i<PARAMETERS_mp_NUMOFLETTERS;i++)
		PARAMETERS_mp_FILENAMEF90[i]=m_UserDirectory[i];*/
	//PARAMETERS_mp_CHDIR_LOCAL();	 
	if(m_ContinuousHelp)
		((CCoupModelApp*)pApp)->m_Setup.ContinuousHelp=true;
	else
		((CCoupModelApp*)pApp)->m_Setup.ContinuousHelp=false;
    }

void CUserSetup::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CUserSetup::OnInitDialog() 
{	
	CWinApp* pApp = AfxGetApp();

   // MFC_Util::GetProfileIntNo( "ContinuousHelp", m_ContinuousHelp);

//	m_ContinuousHelp=TRUE;
	m_MakeTextFile=FALSE;						  
	m_MakeTextFile=MFC_Util::GetProfileIntNo( "MakeTextFile", m_MakeTextFile);
	m_MakeExcelTimeSerieFile=FALSE;
	m_MakeExcelTimeSerieFile=MFC_Util::GetProfileIntNo( "MakeExcelTimeSerieFile", m_MakeExcelTimeSerieFile);
	m_ContinuousHelp=MFC_Util::GetProfileIntNo( "ContinuousHelp", m_ContinuousHelp);
	m_ErrorSignal=MFC_Util::GetProfileIntNo( "ErrorSignal", m_ErrorSignal);
	m_ResidualPlot=MFC_Util::GetProfileIntNo( "ResidualPlot", m_ResidualPlot);
   	m_UserLevel=1;
    m_UserLevel=MFC_Util::GetProfileIntNo( "UserLevel", m_UserLevel);
	m_ChartError=1;
    m_ChartError=MFC_Util::GetProfileIntNo( "ChartError", m_ChartError);

	m_LowResFont = 16;
	m_LowResFont = MFC_Util::GetProfileIntNo("LowResFont", m_LowResFont);
	m_HighResFont = 22;
	m_HighResFont = MFC_Util::GetProfileIntNo("HighResFont", m_HighResFont);

   	m_ViewDisabled=TRUE;
	m_MakeAnnimation=TRUE;
    m_ViewDisabled=MFC_Util::GetProfileIntNo( "ViewDisabled", m_ViewDisabled);
    m_MakeAnnimation=MFC_Util::GetProfileIntNo( "MakeAnnimation", m_MakeAnnimation);
	m_DetailedPlantAnnimation=FALSE;
    m_DetailedPlantAnnimation=MFC_Util::GetProfileIntNo( "DetailedPlantAnnimation", m_DetailedPlantAnnimation);
    m_ViewScaleFont=3;
	m_ViewScaleFont=MFC_Util::GetProfileIntNo( "Font-ViewScale", m_ViewScaleFont);
	m_SimulationPriority=1;
    m_SimulationPriority=MFC_Util::GetProfileIntNo( "SimulationPriority", m_SimulationPriority);

    
	m_UserDirectory=MFC_Util::GetProfileStringStd( "UserDirectory", m_UserDirectory);

	



	m_DataBaseDirectory=MFC_Util::GetProfileStringStd("DataBaseDirectory",m_DataBaseDirectory);
    
	m_UserName=MFC_Util::GetProfileString( "UserName", m_UserName);
	m_Signature=MFC_Util::GetProfileString( "Signature", m_Signature);

	m_LogLikelihoodSwitchValue=FALSE;
	m_LogLikelihoodSwitchValue=MFC_Util::GetProfileIntNo( "LogLikelihoodSwitch", m_LogLikelihoodSwitchValue);

   	CDialog::OnInitDialog();



	UpdateData(FALSE);



	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserSetup::OnButtonApply() 
{
	CWinApp* pApp = AfxGetApp();

	UpdateData(TRUE);

     MFC_Util::WriteProfileInt( "ViewDisabled", m_ViewDisabled);
    MFC_Util::WriteProfileInt( "MakeAnnimation", m_MakeAnnimation);
    MFC_Util::WriteProfileInt( "DetailedPlantAnnimation", m_DetailedPlantAnnimation);

    MFC_Util::WriteProfileString( "DataBaseDirectory", m_DataBaseDirectory);
	MFC_Util::WriteProfileString("Signature", CT2A(m_Signature).m_psz);
	MFC_Util::WriteProfileString("UserName", CT2A(m_UserName).m_psz);

	MFC_Util::WriteProfileInt("LowResFont", m_LowResFont);
	MFC_Util::WriteProfileInt("HighResFont", m_HighResFont);

    MFC_Util::WriteProfileString( "UserDirectory", m_UserDirectory);
    MFC_Util::WriteProfileString( "ExcelSumFile", m_ExcelSumFile);


	OnInitDialog();
	// TODO: Add your control notification handler code here
	
}

void CUserSetup::OnButtonBrowse() 
{
		UpdateData(TRUE);

		CWinApp* pApp = AfxGetApp();
	// TODO: Add your control notification handler code here
				CStdioFile driveFile;
				CString pszFileName ;
				LPCTSTR lpszDefExt;
				LPCTSTR lpszFilter;
				int fileDlgResult;
				lpszDefExt=_T("xls");
				lpszFilter =_T("Excel File (*.xls)|*.xls||");
				pszFileName=CString(m_ExcelSumFile.c_str());

			
				CFileDialog	DriveFileDlg(TRUE,lpszDefExt,pszFileName, OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,lpszFilter);
			
			
				fileDlgResult=DriveFileDlg.DoModal();
			
				//If the cancel button has been pushed then return
				if (fileDlgResult==IDCANCEL)
					return;
			
				//Retrieve the filename of the file that is to be opened
				pszFileName = DriveFileDlg.GetPathName();
				//If the file doesn´t exist
				if(!driveFile.Open(pszFileName, CFile::typeText | CFile ::modeRead  ))
					{
						TRACE("Can't Open file %s´\n", pszFileName); 
						//Display error message
					//	MessageBox("For some reason the file specified couldn´t be opened.","File Open Error",MB_OK|MB_ICONEXCLAMATION);
						return;
						// Actions without file name ?
					}
				driveFile.Close();
				//Change the filename to uppercase
//				pszFileName.MakeUpper();
				m_ExcelSumFile=CT2A(pszFileName).m_psz;
				MFC_Util::WriteProfileString( "ExcelSumFile", m_ExcelSumFile);
				OnInitDialog();

					
	
}

void CUserSetup::OnButtonBrowseDb() 
{

	UpdateData(TRUE);
	CWinApp* pApp = AfxGetApp();


			
		CStdioFile driveFile;
				CString pszFileName ;
				LPCTSTR lpszDefExt;
				LPCTSTR lpszFilter;
				int fileDlgResult;
				lpszDefExt=_T("dba");
				lpszFilter =_T("Data BaseExcel (*.dba)|*.dba||");
				pszFileName=CString(m_DataBaseDirectory.c_str())+_T("Pfprof.dba");
			
				CFileDialog	DriveFileDlg(TRUE,lpszDefExt,pszFileName, OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,lpszFilter);
			
			
				fileDlgResult=DriveFileDlg.DoModal();
			
				//If the cancel button has been pushed then return
				if (fileDlgResult==IDCANCEL)
					return;
			
				//Retrieve the filename of the file that is to be opened
				pszFileName = DriveFileDlg.GetPathName();
				//If the file doesn´t exist
				if(!driveFile.Open(pszFileName, CFile::typeText | CFile ::modeRead  ))
					{
						TRACE("Can't Open file %s´\n", pszFileName); 
						//Display error message
					//	MessageBox("For some reason the file specified couldn´t be opened.","File Open Error",MB_OK|MB_ICONEXCLAMATION);
						return;
						// Actions without file name ?
					}
				driveFile.Close();
				//Change the filename to uppercase
				int ipos;
				ipos=pszFileName.ReverseFind('\\');
				pszFileName.MakeUpper();
				m_DataBaseDirectory=CT2A(pszFileName.Left(ipos+1));
				/*PLOTPF_mp_NUMDIR=m_DataBaseDirectory.GetLength();
				for(int i=0;i<PLOTPF_mp_NUMDIR;i++)
					PLOTPF_mp_DATABASEDIR[i]=m_DataBaseDirectory[i];*/
				MFC_Util::WriteProfileString( "DataBaseDirectory", m_DataBaseDirectory);
		OnInitDialog();
   
}



void CUserSetup::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}



void CUserSetup::OnBnClickedLogLikelihoodSwitch()
{
	UpdateData(TRUE);
	CWinApp* pApp = AfxGetApp();
	MFC_Util::WriteProfileInt( "LogLikelihoodSwitch", m_LogLikelihoodSwitchValue);
	// TODO: Add your control notification handler code here
}
