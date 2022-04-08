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

// CoupModel.cpp : Defines the class behaviors for the application.
//


#include "stdafx.h"
#include "std.h"
#include ".\util.h"
#include "afxwinappex.h"
#include "afxdialogex.h"

#include "MainFrm.h"

#include "ChildFrm.h"
#include "CoupModel.h"
#include "CoupModelDoc.h"
#include ".\NewBase\ModelBase.h"
#include ".\CUG\SimDoc\Mycug.h"
#include "CoupModelView.h"
#include "Aboutbox.h"
#include "UserSetup.h"
#include "Welcome.h"
#include "HtmlHelp.h"
#include ".\PG\PG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//#include "External.h"



#include "Userenv.h"
#pragma comment(lib, "userenv.lib")

CString GetUserHomeDir()
{
   TCHAR szHomeDirBuf[MAX_PATH] = { 0 };
   
   HANDLE hToken = 0;
   VERIFY( OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken ));
   
   DWORD BufSize = MAX_PATH;
   VERIFY( GetUserProfileDirectory( hToken, szHomeDirBuf, &BufSize ));
   
   CloseHandle( hToken );
   return CString( szHomeDirBuf );
}

static char buffer[90000];
static TCHAR tbuffer[90000];



// CCoupModelApp

BEGIN_MESSAGE_MAP(CCoupModelApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CCoupModelApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CCoupModelApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_APP_EXIT, &CCoupModelApp::OnAppExit)
	ON_COMMAND(ID_PGFILE_NEW, &CCoupModelView::OnPgfileNew)
	ON_COMMAND(ID_PGFILE_OPEN,&CCoupModelView::OnPgfileOpen)

	// Standard print setup command
	//ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_USERSETTING, &CCoupModelApp::OnBaseSetup)

	ON_COMMAND(ID_TOOLS_OPTIONS, &CCoupModelApp::OnToolsOptions)
END_MESSAGE_MAP()


// CCoupModelApp construction

CCoupModelApp::CCoupModelApp()
{
	EnableHtmlHelp();
	AfxOleInit();
	m_bHiColorIcons = TRUE;

	// support Restart Manager
	//	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_NO_AUTOSAVE;
		 //AFX_RESTART_MANAGER_REOPEN_PREVIOUS_FILES
		m_dwRestartManagerSupportFlags =0x00;//AFX_RESTART_MANAGER_SUPPORT_NO_AUTOSAVE	;
	//m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("PEJ.CoupModel.Sim.5.4.4.0"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

}

// The one and only CCoupModelApp object

CCoupModelApp theApp;



// CCoupModelApp initialization

BOOL CCoupModelApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	m_AppString=_T("CoupModel");
/*
	ViewStatus=-1;
	Running=FALSE;
	MultiRunning=FALSE;
	RunningStatus=0;
	ReadyToSimulate=FALSE;*/
	m_Setup.ContinuousHelp=false;


	CWinAppEx::InitInstance();
	MFC_Util::MFC_Util();

	

	//AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;



	// Initialize OLE libraries
	//if (!AfxOleInit())
	//{
		//AfxMessageBox(IDP_OLE_INIT_FAILED);
//		return FALSE;
	//}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control	
	 AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
    LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)


	CString UserDirectory=GetUserHomeDir();
	int pos=UserDirectory.ReverseFind('\\');
	CString UserName=UserDirectory.Mid(pos+1);
	UserName=MFC_Util::GetProfileString( "UserName", UserName);
	MFC_Util::WriteProfileString("UserName", UserName);


	UserDirectory.Append(_T("\\Documents\\CoupModel\\Samples\\"));
	UserDirectory=MFC_Util::GetProfileString("UserDirectory", UserDirectory);
	MFC_Util::WriteProfileString( "UserDirectory", UserDirectory);

	CString DataBaseDirectory=GetUserHomeDir();
	DataBaseDirectory.Append(_T("\\Documents\\CoupModel\\SoilProperties\\"));
	DataBaseDirectory=MFC_Util::GetProfileString( "DataBaseDirectory", DataBaseDirectory);

	MFC_Util::WriteProfileString("DataBaseDirectory", DataBaseDirectory);
	int ContHelp=0;
	ContHelp=MFC_Util::GetProfileIntNo("ContinuousHelp", ContHelp);
	if(ContHelp!=0) m_Setup.ContinuousHelp=true;


	InitContextMenuManager();
	InitShellManager();
	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	//CMultiDocTemplate* pDocTemplate;
	m_pDocTemplate = new CMultiDocTemplate(IDR_CoupModelTYPE,
		RUNTIME_CLASS(CCoupModelDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CCoupModelView));
	if (!m_pDocTemplate)
		return FALSE;
	AddDocTemplate(m_pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	MySingle::instance().SetFramePointer(pMainFrame);
		// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	
	// m_pMainWnd->DragAcceptFiles();

	// Parse command line for standard shell commands, DDE, file open

	
	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	

	int m_SimulationRunNo=1;
	m_SimulationRunNo=GetProfileInt(m_AppString, _T("SimulationRunNo"), m_SimulationRunNo);
	if(m_SimulationRunNo==1) {
			CWelcome dlg;
			dlg.DoModal();
	}

	// create some flags needed to take commandline input
		int Rflag = 0;
		int Qflag = 0;
		int Iflag = 0;
		m_CommonModel.SetCommandLineMode(false);
	
	CString strCmdLine;
	if (m_lpCmdLine[0] == '\0')
	{
    // Open file name saved in private INI file.
		CString strDocPath = GetDocPathFromIniFile();
		int numdoc=0;
		CString fileopen;
		numdoc=GetProfileInt(m_AppString,_T("OpenFile"), numdoc);

		for(int i=0;i<numdoc;i++) {
		fileopen=_T("OpenFile")+MFC_Util::ItoAscii(i+1);
			strDocPath=GetProfileString(m_AppString, fileopen, strDocPath);
			if(FUtil::IsFileExisting(CT2A(strDocPath).m_psz) )
				OpenDocumentFile(strDocPath);
		}




	}
	else
	{
	// Read command line input to string
		strCmdLine = m_lpCmdLine;
		CString strDocPath;

		// search for flags in Command line string
		if(strCmdLine.Find(_T("/"),0)>-1)
		{
			m_CommonModel.SetCommandLineMode(true);
			// /F = flag proceeding file name
			if(strCmdLine.Find(_T("/F"),0)>-1)
			{
				// find index of /F sign
				int iStrPos = strCmdLine.Find(_T("/F"),0);
								
				// look for other flasg after /F
				int iStrPos2 = strCmdLine.Find(_T("/"),iStrPos+2);

				// if no flags, then read to the end of CommandLine
				if(iStrPos2 == -1)
				{
					iStrPos2 = strCmdLine.GetLength();
				}
			
				// if no text behind '/F', then the filname is read from private INI file
				if(iStrPos+2<iStrPos2)
				{
					strDocPath = strCmdLine.Mid(iStrPos+2,iStrPos2-iStrPos-2);
				}
				else
				{
					strDocPath = GetDocPathFromIniFile();
				}
			}
			// if /F is omitted, the filename is assumed to be at the end of command line
			else
			{
				// find index of last flag in commandline
				int iStrPos = strCmdLine.ReverseFind('/');
				if(iStrPos == -1)
				{
					iStrPos = -2;
				}

				int	iStrPos2 = strCmdLine.GetLength();
			
				if(iStrPos+2<iStrPos2)
				{
					strDocPath = strCmdLine.Mid(iStrPos+2,iStrPos2-(iStrPos+2));
				}
				else
				{
					strDocPath = GetDocPathFromIniFile();
				}
			}
		}
		else
		{
			strDocPath = strCmdLine;
			m_CommonModel.SetCommandLineMode(false);
		}
		if (!strDocPath.IsEmpty())
		{
			while (strDocPath[0]==' '){
				strDocPath = strDocPath.Mid(1,strDocPath.GetLength());}
			while (strDocPath[strDocPath.GetLength()-1]==' '){
				strDocPath = strDocPath.Mid(0,strDocPath.GetLength()-1);}

			OpenDocumentFile(strDocPath);
			
		}
		}

	// David: Ok, commandline arguments have been read and converted to integer flags, and the document is opened
	// Now, how to use these inputs? Implementing a new, which may for instance perform
	// simulation without starting the MFC application (thus skipping the lines below)?
	if(m_CommonModel.GetCommandLineMode())	{
		POSITION pos;
		//::MultiRunning=TRUE;
		pos = m_pDocTemplate->GetFirstDocPosition();
		CCoupModelDoc* dg_pDoc =(CCoupModelDoc*)m_pDocTemplate->GetNextDoc(pos);
		pos = dg_pDoc->GetFirstViewPosition();
		
		if(!dg_pDoc->m_Doc.OnCommandLine(CT2A(strCmdLine).m_psz))
		{
			return FALSE;
		}
		if(dg_pDoc->m_Doc.m_CommandLine_Quit) // quit application
		{
			exit(0);
		}
	}





	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.

	//if(!ProcessShellCommand(cmdInfo))
		//return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	pMainFrame->m_wndFileView.FillFileView();

	return TRUE;
}

int CCoupModelApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}
void CCoupModelApp::OnAppExit()
{

	int numdoc=0;
	string fileopen;
	string filename;
	Doc* pDoc;
	size_t numpg=MySingle::instance().GetPGFileArchivePointer()->GetNumPGFile();
	SimArchive *pArchive=MySingle::instance().GetSimArchivePointer();

	numdoc=pArchive->GetNumSimDoc();
	MFC_Util::WriteProfileInt("OpenFile", numdoc+numpg);

	for(size_t i=0;i<numpg;i++) {
		CPG *pPG=MySingle::instance().GetPGFileArchivePointer()->GetPGFile(i);
		filename=pPG->GetFileName();
		if (filename.find(".") == string::npos) filename += ".Bin";
		pDoc = pArchive->GetSimDocPointer(i);
		fileopen = "OpenFile" + FUtil::STD_ItoAscii(i + 1);
		MFC_Util::WriteProfileString(fileopen, filename);
	}
	size_t icount = numpg;

	for(int i=0;i<numdoc;i++) {
		//pDoc =(CCoupModelDoc*)m_pDocTemplate->GetNextDoc(pos);
		//filename=pDoc->GetPathName();
		pDoc=pArchive->GetSimDocPointer(i);
		fileopen = "OpenFile" + FUtil::STD_ItoAscii(icount + 1); icount++;
		filename=pDoc->GetCurrentSimFile().c_str();
		MFC_Util::WriteProfileString( fileopen, filename);

	}

	CWinApp::OnAppExit();
}

// CCoupModelApp message handlers


// CAboutDlg dialog used for App About


// CCoupModelApp customization load/save methods

void CCoupModelApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
//	auto res=CleanState();

}

void CCoupModelApp::LoadCustomState()
{
}

void CCoupModelApp::OnFileNew()
{
	// By default, the framework does not create an actual file for
	// a new document until the user does a File Save As.  But CoupModel
	// requires a file as soon as the user does a File New, because
	// CoupModel updates the file on a per transaction basis.  Upon File
	// New, we prompt the user with a File Open dialog, in which the
	// user specifies a new file (or if she changes her mind, she
	// can specify an existing check book file).  We call the same
	// CWinApp::DoPromptFileName that CWinApp::OnFileOpen calls.
	// But we replace the OFN_FILEMUSTEXIST flag with
	// OFN_CREATEPROMPT.
	//    CString CWinsoGetDocPathFromIniFile()
	//ReadyToSimulate=FALSE;
	
	
	CString UserDirectory;
	UserDirectory=GetProfileString(m_AppString,_T("UserDirectory"), UserDirectory);
	
	CString strNewFileName;
	//strNewFileName=UserDirectory;
	//CStdioFile NewFileDlg;
	CString pszFileName ;
	LPCTSTR lpszDefExt=_T("Sim"), lpszFilter;
	lpszFilter = _T("Sim File (*.sim)|*.Sim||");
 	CFileDialog	NewFileDlg(TRUE,lpszDefExt,pszFileName, OFN_NOCHANGEDIR,lpszFilter);
	
	NewFileDlg.m_ofn.lpstrFile=&tbuffer[0];
	NewFileDlg.m_ofn.nMaxFile=1000;
	NewFileDlg.m_ofn.lpstrInitialDir=UserDirectory;

	int fileDlgResult=NewFileDlg.DoModal();

//if (!(DoPromptFileName(strNewFileName, IDS_NEW_DOCUMENT,
//			OFN_HIDEREADONLY | OFN_CREATEPROMPT, FALSE, nullptr)))
//	return;
// If file doesn't already exist, then create it.

	CString NewPathName;
	NewPathName=NewFileDlg.GetPathName();
	
	int SimulationRunNo=1;

	SimulationRunNo=GetProfileInt(m_AppString,_T("SimulationRunNo"), SimulationRunNo);
    


	int iposext=NewPathName.Find(_T(".Sim"));
	CString Num;
	Num.Format(_T("%05u"),SimulationRunNo);
	if (iposext==-1) {
		NewPathName+=_T("_")+Num+_T(".Sim");			
	}
	else if(NewPathName.Mid(iposext-4,1)=="_") {
		NewPathName=NewPathName.Mid(0,NewPathName.GetLength()-4);
		NewPathName+=_T("_")+Num+_T(".Sim");
	}
	else {
		NewPathName=NewPathName.Mid(0,iposext)+_T("_")+Num+_T(".Sim");
	}


	strNewFileName=NewPathName;


	CFile file;
	CFileStatus status;
	if (!file.GetStatus(strNewFileName, status))
	{
		if (!file.Open(strNewFileName, CFile::modeCreate))
		{
			TCHAR szFileTitle[64];
			CString strMessage;
			::GetFileTitle(strNewFileName, szFileTitle, sizeof(szFileTitle)/sizeof(TCHAR));
			AfxFormatString1(strMessage, IDS_CANNOT_OPEN_DOCUMENT, szFileTitle);
			AfxMessageBox(strMessage);
			return;
		}
		file.Close();
	}

	// Open the file now that it has been created.
	OpenDocumentFile(strNewFileName);
//	ReadyToSimulate=TRUE;
//	ViewStatus=-1;
	}
void CCoupModelApp::OnFileOpen()
{
	LPCTSTR lpszFilter;
	lpszFilter = _T("Sim Files (*.sim)|*.Sim|PG File (*.Bin)|*.Bin|PG File Text Input (*.Txt)|*.Txt||");
	OpenSimDocument(lpszFilter);
}
void CCoupModelApp::CloseSimDocument()
{
	CDocument *pDocument;
	POSITION pos2= m_pDocTemplate->GetFirstDocPosition();

	CCoupModelDoc* dg_pDoc =(CCoupModelDoc*)m_pDocTemplate->GetNextDoc(pos2);
	int numdoc=0;

	while(pos2) { 
	   if(pDocument = m_pDocTemplate->GetNextDoc(pos2)) {
				//m_pDocArray.Add((CCoupModelDoc*)pDocument);
				numdoc++;
	   }
	}
	


}
BOOL CCoupModelApp::OnFileOpen(LPCTSTR lpszFile, BOOL SQL)
{

	LPCTSTR lpszFilter;
	lpszFilter = _T("Sim Files (*.sim)|*.Sim||");
	CStdioFile driveFile;
	CString pszFileName ;
	LPCTSTR lpszDefExt;
	int fileDlgResult;


	lpszDefExt=_T("sim");
	CString UserDirectory;
	UserDirectory=MFC_Util::GetProfileString("UserDirectory", UserDirectory);

 	CFileDialog	DriveFileDlg(TRUE,lpszDefExt,pszFileName, OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR|OFN_ALLOWMULTISELECT,lpszFilter);
	DriveFileDlg.m_ofn.lpstrFile=&tbuffer[0];
	DriveFileDlg.m_ofn.nMaxFile=90000;
	DriveFileDlg.m_ofn.lpstrInitialDir=UserDirectory;

	fileDlgResult=DriveFileDlg.DoModal();
	if (fileDlgResult==IDCANCEL) {
		OpenDocumentFile(lpszFile);
		return FALSE;
	}


	POSITION pos=DriveFileDlg.GetStartPosition( );
	CString kalle;
	bool Shifted=FALSE;
	//BAYESIANCALIB_mp_ACCEPTEDRUN=TRUE;
	while(pos!=nullptr) {
		kalle=DriveFileDlg.GetNextPathName(pos);
		OpenDocumentFile(kalle);
		POSITION pos2= m_pDocTemplate->GetFirstDocPosition();
		CCoupModelDoc* dg_pDoc =(CCoupModelDoc*)m_pDocTemplate->GetNextDoc(pos2);
#ifndef WIN64

			if(!m_CommonModel.IsRunning()) {
				m_CommonModel.SetRunning(true);
				Shifted=TRUE;
			}


#else
			dg_pDoc->m_Doc.m_Link.WriteExport();
			if(!Running) {
				Running=TRUE;
				Shifted=TRUE;
			}


#endif
		dg_pDoc->OnCloseDocument();
	//	PARAMETERS_mp_PARTY_OVER();
	}
	if(Shifted) 
		m_CommonModel.SetRunning(false);
	OpenDocumentFile(lpszFile);
	return FALSE;

}

BOOL CCoupModelApp::OnFileOpen(LPCTSTR lpszFile)
{
	CDocument* Doc;
 	Doc=nullptr;
	m_CommonModel.SetStartMode(false);
	Doc=OpenDocumentFile(lpszFile);
	if(Doc==nullptr)
		return FALSE;
	else
		return TRUE;
}

void CCoupModelApp::OnMultiFileOpen()
{
	LPCTSTR lpszFilter;
	lpszFilter = _T("Sim Files (*Multi*.sim)|*Multi*.Sim||");
	OpenSimDocument(lpszFilter);
}
void CCoupModelApp::OnHelp()
{
  	CString lDataBaseDirectory;
	lDataBaseDirectory=MFC_Util::GetProfileString("DataBaseDirectory",lDataBaseDirectory);
  
   	CString Dir;
	Dir=_T("ms-its:")+lDataBaseDirectory;
	Dir+="\\COUP99.chm";
	Dir+="::/WordDocuments/howtoreadthisdocument.htm";
	 
}
void CCoupModelApp::OnAppAbout()
{
	Aboutbox dlg(nullptr);
	dlg.DoModal();
}


void CCoupModelApp::OpenSimDocument(LPCTSTR lpszFilter)
{
	CStdioFile driveFile;
	CString pszFileName ;
	LPCTSTR lpszDefExt;
//	LPCTSTR lpstrInitialDir;
	int fileDlgResult;
	int filecount;
	CDocument *pDoc=nullptr;

	lpszDefExt=_T("sim|xlm");
	CString UserDirectory;
	UserDirectory=MFC_Util::GetProfileString("UserDirectory", UserDirectory);

 	CFileDialog	DriveFileDlg(TRUE,lpszDefExt,pszFileName, OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR|OFN_ALLOWMULTISELECT,lpszFilter);
	DriveFileDlg.m_ofn.lpstrFile=&tbuffer[0];
	DriveFileDlg.m_ofn.nMaxFile=1000;
	DriveFileDlg.m_ofn.lpstrInitialDir=UserDirectory;

	fileDlgResult=DriveFileDlg.DoModal();
	if (fileDlgResult==IDCANCEL)
		return;
	filecount=0;
	CString kalle;
	POSITION pos=DriveFileDlg.GetStartPosition( );
	while(pos!=nullptr) {
		filecount++;
		kalle=DriveFileDlg.GetNextPathName(pos);
	}

	pos=DriveFileDlg.GetStartPosition( );
	
	bool Shifted=FALSE;

	while(pos!=nullptr) {
		kalle=DriveFileDlg.GetNextPathName(pos);
		if(pDoc==nullptr) {
			pDoc=OpenDocumentFile(kalle);

		}
		else {
			((CCoupModelDoc*)pDoc)->AddPGFile(CT2A(kalle).m_psz);

		}
		if(!m_CommonModel.IsRunning()){
			m_CommonModel.SetRunning(true);
			Shifted=TRUE;
		}
	}
	if(Shifted)
		m_CommonModel.SetRunning(false);



}
void CCoupModelApp::SaveCustomState()
{
}

// CCoupModelApp message handlers

void CCoupModelApp::CreateFileNew(CString strNewFileName)
{
	// If file doesn't already exist, then create it.
	CFile file;
	CFileStatus status;
	if (!file.GetStatus(strNewFileName, status))
	{
		if (!file.Open(strNewFileName, CFile::modeCreate))
		{
			TCHAR szFileTitle[64];
			CString strMessage;
			::GetFileTitle(strNewFileName, szFileTitle, sizeof(szFileTitle)/sizeof(TCHAR));
			AfxFormatString1(strMessage, IDS_CANNOT_OPEN_DOCUMENT, szFileTitle);
			AfxMessageBox(strMessage);
			return;
		}
		file.Close();
	}
	OpenDocumentFile(strNewFileName);
	m_CommonModel.SetReady(true);
}
void CCoupModelApp::OnBaseSetup()
{
	CUserSetup dlg;
	if(dlg.DoModal()){
	};
}

CString CCoupModelApp::GetDocPathFromIniFile()
{	 
	CWinApp* pApp = AfxGetApp();
	return pApp->GetProfileString(szIniFileSection, szIniFileEntry);
}


void CCoupModelApp::OnUsersetting()
{
	int i=0;
	// TODO: Add your command handler code here
}





void CCoupModelApp::OnToolsOptions()
{
	CUserSetup dlg;
	if(dlg.DoModal()){
	};
	
}


