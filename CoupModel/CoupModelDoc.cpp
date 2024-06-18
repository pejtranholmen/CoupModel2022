// CoupModelDoc.cpp : implementation of the CCoupModelDoc class
//  d

#define CRTDBG_MAP_ALLOC
#include ".\stdafx.h"
#include "std.h"
#include "Util.h"
#include ".\MainFrm.h"
#include "CoupModel.h"
#include "CoupModelDoc.h"
#include "CoupModelView.h"
#include ".\PG\FileImport\PGFileImport.h"
#include ".\PG\PGfileArchive.h"

/*extern "C" char  PARAMETERS_mp_WORKDIR[255];
extern "C" int   PARAMETERS_mp_NUMCWORKDIR;
extern "C" int   PLOTPF_mp_NUMDIR;
extern "C" char  PARAMETERS_mp_UNITPATH;
extern "C" bool  PLOTPF_mp_CHECKDATABASE();
extern "C" char  PLOTPF_mp_DATABASEDIR[255];
extern "C" void SOIL_MAIN_mp_SOIL_INIT_FLAGS();*/


static BOOL ContinueRead=FALSE;
BOOL ReadResultsFromFile=TRUE;
//extern bool ReadyToSimulate;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static BOOL MultiRunChange;
BOOL AutoDelete;


extern int countinfo, countvalidation,countvalsum,count_x,count_t,count_g,count_d,countmulti;


/////////////////////////////////////////////////////////////////////////////
// CCoupModelDoc
IMPLEMENT_DYNCREATE(CCoupModelDoc, CDocument)

BEGIN_MESSAGE_MAP(CCoupModelDoc, CDocument)
	//{{AFX_MSG_MAP(CCoupModelDoc)
	ON_COMMAND(ID_FILE_CLOSE, OnCloseDocument)
	
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoupModelDoc construction/destruction

CCoupModelDoc::CCoupModelDoc()
{
//   	CBasePtrArray *arr;
	
	m_pApp=(CCoupModelApp*)AfxGetApp();
	AutoDelete=FALSE;
	m_Closing=FALSE;
	m_Ready=FALSE;
	m_Doc.m_DocumentType=0;
	m_ToBeChild = false;

}
CCoupModelDoc::~CCoupModelDoc()
{
	int i=0;
	//AutoDelete=TRUE;
  	
}


BOOL CCoupModelDoc::AddPGFile(string file)
{ 
	if(file.find(".Sim")>0) {
		m_Doc.m_DocumentType=0;
	}
	else if(file.find(".Txt")>0||file.find(".txt")>0||file.find(".TXT")>0)
		m_Doc.m_DocumentType=1;
	else
		m_Doc.m_DocumentType=2;

	if(m_Doc.m_DocumentType>0) {

			
			if(m_Doc.m_DocumentType==1) {
				PGFileImport *pPG= new PGFileImport;
				pPG->SetCompleteRead(TRUE);
				pPG->ImportFromFile(file);
				file=pPG->GetFileName();
				CDocument::SetPathName(CString(file.c_str()));
				pPG->SaveAs(file);
				pPG->SetCompleteRead(FALSE);
				pPG->CloseFile();
				delete pPG;
			}
			CPG *pPG= new CPG;
			pPG->Open(file);
			if(pPG->IsOpen()) {
				CMainFrame *p_mainframe;
				p_mainframe = (CMainFrame*) AfxGetMainWnd();
				p_mainframe->m_wndFileView.FillFileView();
			//	m_PlotStorage.m_PGFileArray.Add(pPG);
			//	m_PlotStorage.m_PGFileArrayShow.Add(0);
			}

		return TRUE;
	}

	return FALSE;
}
BOOL CCoupModelDoc::OnOpenDocument(LPCTSTR pzPathName)
{ 
	CWinApp* pApp = AfxGetApp();
	CString strApp=((CCoupModelApp*)pApp)->m_AppString;

 	string file=CT2A(pzPathName).m_psz;
	int ifilt;
	ifilt=file.find(".Sim");
	if (ifilt<0)ifilt = file.find(".sim");
	if(ifilt<0) ifilt=file.find(".SIM");
	if (ifilt < 0) ifilt = file.find(".xml");
	if (ifilt == string::npos) ifilt = file.find(".MBin");
	if (ifilt == string::npos) ifilt = file.find(".SBin");
	m_Doc.m_DocumentType=2;
	if(ifilt>0) {
		m_Doc.m_DocumentType=0;
	}
	else {
		ifilt=file.find(".Txt");
		if(ifilt>0)
			m_Doc.m_DocumentType=1;
		ifilt=file.find(".txt");
		if(ifilt>0)
			m_Doc.m_DocumentType=1;		
		ifilt=file.find(".TXT");
		if(ifilt>0)
			m_Doc.m_DocumentType=1;
		ifilt = file.find(".csv");
		if (ifilt>0)
			m_Doc.m_DocumentType = 1;
		ifilt = file.find(".Csv");
		if (ifilt>0)
			m_Doc.m_DocumentType = 1;
		ifilt = file.find(".CSV");
		if (ifilt>0)
			m_Doc.m_DocumentType = 1;



	}

	m_Doc.m_DocFile.m_SimulationRunNo=1;
	m_Doc.m_DocFile.m_SimulationRunNo=pApp->GetProfileInt(strApp, _T("SimulationRunNo"), m_Doc.m_DocFile.m_SimulationRunNo);

	m_Doc.m_CurrentFile=CT2A(pzPathName).m_psz;	
    int iv=	m_Doc.m_CurrentFile.rfind('\\');
	if(iv>1) m_Doc.m_CurrentDirectory=m_Doc.m_CurrentFile.substr(0,iv+1);

	//init.InitGlobalPointer(&F);	//	DSM error

	

	CMainFrame *p_mainframe;
	p_mainframe = (CMainFrame*) AfxGetMainWnd();

	size_t num=MySingle::instance().GetPGFileArchivePointer()->GetNumPGFile();

	if(m_Doc.m_DocumentType>0) {
			
			if(m_Doc.m_DocumentType==1) {
				p_mainframe->m_wndFileView.FillFileView();
				p_mainframe->m_wndPG.m_PGgrid.SetDocument(nullptr);
				p_mainframe->m_wndPG.m_PGgrid.m_PGFileImport.m_ExtraFileScanning = false;
				p_mainframe->m_wndPG.m_PGgrid.m_PGFileImport.m_format = STANDARD;
				p_mainframe->m_wndPG.m_PGgrid.m_PGFileImport.m_ScanningMade = false;
				p_mainframe->m_wndPG.m_PGgrid.m_PGFileImport.OpenInputFile(file);
				p_mainframe->m_wndPG.m_PGgrid.m_PGFileImport.m_ScanningMade = true;
				p_mainframe->m_wndPG.m_PGgrid.m_PGFileImport.m_CurrentFileInfo = p_mainframe->m_wndPG.m_PGgrid.m_PGFileImport.ScanningInputFile();
				p_mainframe->m_wndPG.m_PGgrid.m_PGFileImport.m_SuccessFullImportedFileNames.clear();

				if (!p_mainframe->m_wndPG.m_PGgrid.m_PGFileImport.m_CurrentFileInfo.CopyMode) {
					p_mainframe->m_wndPG.m_PGgrid.Make_ImportFromCSV_File_Prior(true);
					p_mainframe->m_wndPG.m_PGgrid.OnSheetSetup(8);
				}
				else {
					p_mainframe->m_wndPG.m_PGgrid.Make_ImportFromCSV_File_Prior(true);
					//p_mainframe->m_wndPG.m_PGgrid.OnSheetSetup(8);

					p_mainframe->m_wndPG.m_PGgrid.m_PGFileImport.m_format = p_mainframe->m_wndPG.m_PGgrid.m_PGFileImport.m_CurrentFileInfo.IdentifiedFormat;

					auto newfile=p_mainframe->m_wndPG.m_PGgrid.m_PGFileImport.CreatePGFileFromDefinedInputFiles();
				    if (newfile.size() > 0) {
						CPG *pPG = new CPG;
						pPG->Open(newfile[0]);
						if (pPG->IsOpen()) {

							PGFile	Fil;
							Fil.pFile = pPG;
							Fil.Visible = false;
							Fil.pDoc = this;
							MySingle::instance().GetPGFileArchivePointer()->AddPGFile(Fil);
							//m_pApp->m_PGFileArray.push_back(Fil);
							num = MySingle::instance().GetPGFileArchivePointer()->GetNumPGFile();
							p_mainframe->SetNoDocumentValid(true);
							p_mainframe->m_wndFileView.FillFileView();
							num = MySingle::instance().GetPGFileArchivePointer()->GetNumPGFile();
						}
						else
							return FALSE;
						num = MySingle::instance().GetPGFileArchivePointer()->GetNumPGFile();
						CString kalle;
						kalle = pzPathName;
						int n = kalle.Find(_T("."));
						if (n < 0) kalle += ".Bin";
						pzPathName = kalle;
						UpdateIniFileWithDocPath(pzPathName);
						p_mainframe->m_wndPG.SetFocus();
						p_mainframe->m_wndPG.ActivateTopParent();
					}

	
				}

				p_mainframe->m_wndPG.SetFocus();
				p_mainframe->m_wndPG.ActivateTopParent();

			}
			else {
				CPG *pPG = new CPG;
				pPG->Open(file);
				if (pPG->IsOpen()) {
					//	m_Doc.m_PlotStorage.m_PGFileArray.push_back(pPG);



					//	m_Doc.m_PlotStorage.m_PGFileArrayShow.push_back(0);
					PGFile	Fil;
					Fil.pFile = pPG;
					Fil.Visible = false;
					Fil.pDoc = this;
					MySingle::instance().GetPGFileArchivePointer()->AddPGFile(Fil);
					//m_pApp->m_PGFileArray.push_back(Fil);
					num = MySingle::instance().GetPGFileArchivePointer()->GetNumPGFile();
					p_mainframe->SetNoDocumentValid(true);
					p_mainframe->m_wndFileView.FillFileView();
					num = MySingle::instance().GetPGFileArchivePointer()->GetNumPGFile();
				}
				else
					return FALSE;
				num = MySingle::instance().GetPGFileArchivePointer()->GetNumPGFile();
				CString kalle;
				kalle = pzPathName;
				int n = kalle.Find(_T("."));
				if (n < 0) kalle += ".Bin";
				pzPathName = kalle;
				UpdateIniFileWithDocPath(pzPathName);
				p_mainframe->m_wndPG.SetFocus();
				p_mainframe->m_wndPG.ActivateTopParent();

			}


		return TRUE;
	}
	else {
		SimObject Fil;
		Fil.pDoc=this;
		Fil.DocType=m_Doc.m_DocumentType;
		Fil.Running=false;
		Fil.pSimDoc=&m_Doc;
		MySingle::instance().GetSimArchivePointer()->AddSimDoc(Fil);

	/*	PVOID OldValue = nullptr;
		Wow64DisableWow64FsRedirection(&OldValue);
		auto filenamex = LPCSTR(file.c_str());
		auto verb = LPCSTR("open");
		auto exe = LPCSTR("C:\\program files(x86)\\PEJ\\CoupModel\\upload_xmlfile.exe");
		HWND  hwnd{ nullptr };


		auto koll=ShellExecuteA(hwnd, verb, filenamex  , exe, NULL, SW_HIDE);

		int i = 1;
			
		CreateProcess(NULL,   // No module name (use command line)
			TEXT("upload_xmlfile.exe"),        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi);           // Pointer to PROCESS_INFORMATION structure
		
		auto kolll = GetLastError();
		printf("CreateProcess failed (%d).\n", GetLastError());
		

		// Wait until child process exits.
		WaitForSingleObject(pi.hProcess, INFINITE);
		*/
		// Close process and thread handles. 
		//CloseHandle(pi.hProcess);
		//CloseHandle(pi.hThread);





	}
	num=MySingle::instance().GetPGFileArchivePointer()->GetNumPGFile();
    m_Doc.m_UserLevel=pApp->GetProfileInt(strApp, _T("UserLevel"), m_Doc.m_UserLevel);

	m_Doc.m_UserDirectory=MFC_Util::GetProfileStringStd("UserDirectory", m_Doc.m_UserDirectory);
	string message;	
	if(m_Doc.m_CurrentDirectory!=m_Doc.m_UserDirectory&&p_ModelInfo->IsStartMode()) {

		message="You have  changed user directory by selection of new document";
		p_mainframe->UpdatePaneText(0,message);
		//MessageBox(nullptr, LPCTSTR(message),_T("Change of User Default directory"),MB_OK|MB_ICONEXCLAMATION);
		m_Doc.m_UserDirectory=m_Doc.m_CurrentDirectory;
		MFC_Util::WriteProfileString("UserDirectory", m_Doc.m_UserDirectory.c_str());

	}

	string m_DataBaseDirectory="\\CoupModel\\SoilProperties\\";

	m_DataBaseDirectory=MFC_Util::GetProfileStringStd("DataBaseDirectory", m_DataBaseDirectory);


	bool NewDocumentPath = false;
		CString kalle;
		kalle=pzPathName;
		int n=kalle.Find(_T(".Sim."));
		if(n>0) {
			kalle.Delete(n+4,4);
		}
		n = kalle.Find(_T(".SBin"));
		if (n > 0) {
			kalle.Delete(n + 1, 4);
			kalle += "xml";
			NewDocumentPath = true;
		}
		n = kalle.Find(_T(".MBin"));
		if (n > 0) {
			kalle.Delete(n + 1, 4);
			kalle += "xml";
			NewDocumentPath = true;
		}
		pzPathName=kalle;

	//SOIL_MAIN_mp_SOIL_INIT_FLAGS();
		if (NewDocumentPath) {
			CCoupModelApp* pApp;
			pApp = (CCoupModelApp*)AfxGetApp();
			pApp->m_pDocTemplate->OpenDocumentFile(pzPathName);
			return FALSE;
		}

	
	if(m_Doc.ReadDocFile(CT2A(pzPathName).m_psz)) {
		CDocument::SetPathName(pzPathName);
		CMainFrame *p_mainframe;
		p_mainframe = (CMainFrame*) AfxGetMainWnd();
		p_mainframe->SetNoDocumentValid(true);

	}
	else	{
		message="Could not read data from file (";
		message+=CT2A(pzPathName).m_psz;
		message+=')';"\nDo you want to create blanke file with that name ?";
		int answer;	
		answer=MFC_Util::MessageBoxReturn(message,"Problems to define new Sim file)",MB_YESNO|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if(m_Doc.IsMultiSimulation()) {
		CString SubDir;
		SubDir=m_Doc.GetMultiSubDir().c_str();
		pApp->WriteProfileString(strApp, _T("UserSubDirectory"), SubDir);
	}
	UpdateIniFileWithDocPath(pzPathName);
	ReadResultsFromFile=TRUE;
	//if(!IsFinishedSimulation()&&!::ReadyToSimulate) ::ReadyToSimulate=TRUE;
	
	//_CrtMemCheckpoint( &m_MemStart);
	//SaveHelpID();
	((CCoupModelApp*)pApp)->CloseSimDocument();

	p_mainframe->m_wndFileView.FillFileView();
	p_mainframe->m_wndModel.SetFocus();
	p_mainframe->m_wndModel.ActivateTopParent();

	m_Ready=TRUE;
	return TRUE;
}
SimB* CCoupModelDoc::GetPtr(CString Name, CString Type, CString Group)
{

	return (SimB*)m_Doc.GetPtr(CT2A(Type).m_psz, CT2A(Group).m_psz, CT2A(Name).m_psz);

}
BOOL CCoupModelDoc::OnSaveDocument(LPCTSTR pzPathName)
{
  CString PathName;
  PathName=pzPathName;
   int iposext=PathName.Find(_T(".Sim"));
   if(iposext<0)
	iposext=PathName.Find(_T(".SIM"));
   int iposbin=PathName.Find(_T(".Bin"));
   if(iposbin<0)
	iposbin=PathName.Find(_T(".BIN"));
   if (iposbin<0)
	   iposbin = PathName.Find(_T(".bin"));
   if(iposext<0&&iposbin<0) {
	   iposext=PathName.ReverseFind('.');
	   if(iposext<0)
		   PathName+=_T(".xml");
	   else
		   PathName=PathName.Mid(0,iposext)+_T(".xml");
   }

   if (m_Doc.m_DocumentType == 2 || iposbin > 0) {
	   CPG* pPG = (CPG*)MySingle::instance().GetPGFileArchivePointer()->GetPGFile();

	   size_t i_doc = MySingle::instance().GetPGFileArchivePointer()->GetIndex(this);
	   pPG = (CPG*)MySingle::instance().GetPGFileArchivePointer()->GetPGFile(i_doc);
	   if (pPG != nullptr) {
		   if (CT2A(PathName).m_psz == pPG->GetFileName()) {
			   if (pPG->GetCompleteRead()) pPG->WritePGFile();
			  
				pPG->CloseFile();
	
		   }
		   else {
			   if (pPG->GetCompleteRead()) {
				   pPG->SetFileName(CT2A(PathName).m_psz);
				   pPG->WritePGFile();
			   }
			   pPG->CloseFile();
			   pPG->WriteNewPGFileAs(CT2A(PathName).m_psz, true);
			   pPG->CloseFile();
		   }

	   }
	   UpdateIniFileWithDocPath(PathName);
	   return TRUE;

   }

   else if(iposbin>0) 
		return TRUE; // No saving ov Bin File documents


	 m_Doc.SetPathName(CT2A(PathName).m_psz);
   if (!m_Doc.WriteDocFile(m_ToBeChild)) return FALSE;

	
		
		return TRUE;
   
}
BOOL CCoupModelDoc::SaveAndOpenDocument()
{	CCoupModelApp* pApp = (CCoupModelApp*)AfxGetApp();
	CString strApp=pApp->m_AppString;
	CString Num;
	CString pzPathName=CDocument::GetPathName();
	CString newPathName ;

	if (!CDocument::OnSaveDocument(pzPathName)) return FALSE;
  	int PreviousRunNo=m_Doc.m_DocFile.m_SimulationRunNo;
	m_Doc.m_DocFile.m_SimulationRunNo=pApp->GetProfileInt(strApp, _T("SimulationRunNo"), m_Doc.m_DocFile.m_SimulationRunNo);
	m_Doc.m_DocFile.m_SimulationRunNo=(m_Doc.m_DocFile.m_SimulationRunNo-1)%99999+1;
	pApp->WriteProfileInt(strApp, _T("SimulationRunNo"),m_Doc.m_DocFile.m_SimulationRunNo);
	m_Doc.m_DocFile.m_SimulationRunNo=pApp->GetProfileInt(strApp, _T("SimulationRunNo"), m_Doc.m_DocFile.m_SimulationRunNo);
	if(m_Doc.m_DocFile.m_SimulationRunNo==PreviousRunNo) {
		m_Doc.m_DocFile.m_SimulationRunNo++;
		pApp->WriteProfileInt(strApp, _T("SimulationRunNo"), m_Doc.m_DocFile.m_SimulationRunNo);
	}
	int iposext=pzPathName.Find(_T(".Sim"));
	Num.Format(_T("%05u"),m_Doc.m_DocFile.m_SimulationRunNo);
   	if (iposext==-1||!(pzPathName.Mid(iposext-4,1)=="_")) {
	 newPathName=pzPathName.Mid(0,pzPathName.GetLength()-4);
	 newPathName+=_T("_")+Num+_T(".Sim");
	}
	else {
		newPathName=pzPathName.Mid(0,iposext-3)+Num+_T(".Sim");
	}
	SetModifiedFlag(TRUE);
	if (!CDocument::OnSaveDocument(newPathName)) return FALSE;
	m_Doc.m_DocFile.m_SimulationRunNo=PreviousRunNo;
	SetModifiedFlag(TRUE);
	if (!CDocument::OnSaveDocument(pzPathName)) return FALSE;
//	CDocument::OnCloseDocument();
	OnCloseDocument();
    CFile file;
	CFileStatus status;
	if (!file.GetStatus(newPathName, status))
	{
		if (!file.Open(newPathName, CFile::modeCreate))
		{
			TCHAR szFileTitle[64];
			CString strMessage;
			::GetFileTitle(newPathName, szFileTitle, sizeof(szFileTitle)/sizeof(TCHAR));
			AfxFormatString1(strMessage, IDS_CANNOT_OPEN_DOCUMENT, szFileTitle);
			AfxMessageBox(strMessage);
			return FALSE;
		}
		file.Close();
	}
	ReadResultsFromFile=FALSE;
	pApp->m_pDocTemplate->OpenDocumentFile(newPathName);
	SetModifiedFlag(TRUE);
 	CDocument::UpdateAllViews(nullptr);
	CDocument::OnOpenDocument(newPathName);
	UpdateIniFileWithDocPath(newPathName);
	m_Doc.m_CurrentFile=CT2A(pzPathName).m_psz;
	m_Doc.m_HasBeenRunning=FALSE;
	pzPathName=CDocument::GetPathName();

	return TRUE;
}
BOOL CCoupModelDoc::OnNewDocument() {
	CDocument::OnNewDocument();
	CWinApp* pApp = AfxGetApp();
	CString strApp=((CCoupModelApp*)pApp)->m_AppString;
	CString pzPathName;
	pzPathName=GetTitle();
 	string file=CT2A(pzPathName).m_psz;

	m_Doc.m_DocumentType=0;
	m_Doc.m_DocFile.m_SimulationRunNo=1;
	m_Doc.m_DocFile.m_SimulationRunNo=MFC_Util::GetProfileIntNo("SimulationRunNo", m_Doc.m_DocFile.m_SimulationRunNo);

	int iposext=pzPathName.Find(_T(".Sim"));
	int iposst=pzPathName.Find(_T("CoupModel1"));
	if(iposst==0)
		pzPathName="SimpleSim";
	CString Num;
	Num.Format(_T("%05u"),m_Doc.m_DocFile.m_SimulationRunNo);
	if (iposext==-1) {
		pzPathName+=_T("_")+Num;			
	}
	CDocument::SetTitle(pzPathName);
	CMainFrame *p_mainframe;
	p_mainframe = (CMainFrame*) AfxGetMainWnd();
	p_mainframe->SetNoDocumentValid(true);


	//CDocument::SetPathName();
	for (size_t i = 0; i < m_Doc.m_ResidualFileVector.size(); i++) m_Doc.m_ResidualFileVector[i][0]->InitDoc(&m_Doc, i + 1);

	m_Doc.Init_BlankDocument();


	return TRUE;
}

void CCoupModelDoc::UpdateIniFileWithDocPath(LPCTSTR lpszPathName)
{	CString str;
    CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileString(szIniFileSection, szIniFileEntry, lpszPathName);
	str=pApp->GetProfileString(szIniFileSection, szIniFileEntry);
}

/////////////////////////////////////////////////////////////////////////////
// CCoupModelDoc diagnostics

#ifdef _DEBUG
void CCoupModelDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCoupModelDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


void CCoupModelDoc::OnCloseDocument() 
{
	//if(!IsFinishedSimulation()&&::ReadyToSimulate) ::ReadyToSimulate=FALSE;
	//m_bAutoDelete=FALSE;

	AutoDelete=FALSE;
	//m_bAutoDelete=FALSE;
	m_Closing=TRUE;
	//if(!m_Doc.IsFinishedSimulation())
	//	ReadyToSimulate=false;
	if(m_Doc.m_DocumentType>0) {

		size_t num=MySingle::instance().GetPGFileArchivePointer()->GetNumPGFile();
		for(size_t i=0;i<num;i++) {
			CPG *pPG=MySingle::instance().GetPGFileArchivePointer()->GetPGFile(i);
			if (pPG->GetFileName().c_str() == CDocument::GetPathName()) {
				if (pPG->IsFileModified()) {
					int ans;
					string message;
					message = "Changed Made are saved automatically ! However, you can return to your previous file if you do not want changes made?\n";
					message+="by responding with Yes you will return to the previous file";					
					ans = MFC_Util::MessageBoxReturn(message, "Saving of PG File", MB_YESNO);
					if (ans == IDYES) {
						if (pPG->ReturnToBackUpCopy(true)) {
							MFC_Util::MessageBox("PG File Saved with content corresponding to prior start of modifications");
						}
					}
					else
						pPG->ReturnToBackUpCopy(false);

				}

				MySingle::instance().GetPGFileArchivePointer()->ErasePGFile(i);
				num=MySingle::instance().GetPGFileArchivePointer()->GetNumPGFile();
				delete pPG;
			}
		}
		
	}
	else {
		SimArchive *pArchive=MySingle::instance().GetSimArchivePointer();
		for(size_t i=0; i<pArchive->GetNumSimDoc();i++) {
			CCoupModelDoc* pDoc=pArchive->GetDocPointer(i);
			if(this==pDoc) 
				pArchive->EraseSimDoc(i);

		}
		for(size_t i=0; i<pArchive->GetNumSimDoc();i++) {
			CCoupModelDoc* pDoc=pArchive->GetDocPointer(i);
			Doc *pSimDoc=pArchive->GetSimDocPointer(i);
			pSimDoc->CheckSelectedDocumentsValidity();

		}
	}

	SimArchive *pArchive=MySingle::instance().GetSimArchivePointer();
	//if(pArchive->GetNumSimDoc()>0)


	if(m_Doc.m_DocumentType==0) {
		if(!AutoDelete) {
			m_bAutoDelete=FALSE;
		}
		CDocument::OnCloseDocument( );
		CMainFrame *p_mainframe;
	    p_mainframe = (CMainFrame*) AfxGetMainWnd();
		p_mainframe =MySingle::instance().GetFramePointer();
		p_mainframe->m_wndFileView.FillFileView();
		p_mainframe->m_wndClassView.FillClassView();
	}
	else {
		CDocument::OnCloseDocument( );
		CMainFrame *p_mainframe;
	    p_mainframe = (CMainFrame*) AfxGetMainWnd();
		p_mainframe =MySingle::instance().GetFramePointer();
		p_mainframe->m_wndFileView.FillFileView();
		p_mainframe->m_wndPG.m_PGgrid.SetDocument(nullptr);
		p_mainframe->m_wndPG.m_PGgrid.OnSheetSetup(0);


	}
//	if(this!=nullptr) m_bAutoDelete=FALSE;
	//CString pzPathName=CDocument::GetPathName();

//	 m_Doc.SetPathName(string(pzPathName));
	//if (!m_Doc.WriteDocFile());
	//Fort.Delete_Map();
	//Fort.Delete_CEPM();

}














