
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CoupModel.h"

#include "MainFrm.h"
#include "Util.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_UPDATE_COMMAND_UI(ID_STATUS_TEXT, OnUpdateBound0)
	
	ON_UPDATE_COMMAND_UI(ID_ERROR_MIN, OnUpdateBound1)
	ON_UPDATE_COMMAND_UI(ID_WARNING_MIN, OnUpdateBound2)
	ON_UPDATE_COMMAND_UI(ID_WARNING_MAX, OnUpdateBound3)
	ON_UPDATE_COMMAND_UI(ID_ERROR_MAX, OnUpdateBound4)
	ON_UPDATE_COMMAND_UI(ID_UNIT, OnUpdateUnit)


	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_STATUS_TEXT,
	ID_ERROR_MIN,
	ID_WARNING_MIN,
	ID_WARNING_MAX,
	ID_ERROR_MAX,
	ID_UNIT,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_ActDoc=nullptr;
	m_DocumentValid=false;
	MySingle::instance().SetFunctionPointer(&m_Func);
	MySingle::instance().SetPlotPFPointer(&m_PlotPF);
	
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	// set the visual manager used to draw all user interface elements
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


			// Create output window
	CString strOutputWnd=_T("Model Document");
	bNameValid = strOutputWnd.LoadString(IDS_DOCUMENT);
	ASSERT(bNameValid);
	if (!m_wndModel.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND2, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Output window\n");
		return FALSE; // failed to create
	}

	
	m_wndModel.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndModel);

		
	CString strOutputWnd2;
	strOutputWnd2="PG File";
	if (!m_wndPG.Create(strOutputWnd2, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND3, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Output window\n");
		return FALSE; // failed to create
	}
	m_wndPG.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndPG);


		// Create properties window
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}

	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);



	/*
	CString strChartWnd;
	strChartWnd="Chart View Window";
	if (!m_wndChartView.Create(strChartWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_CHARTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Chart window\n");
		return FALSE; // failed to create
	}

	m_wndChartView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndChartView);
	*/
	CString strPFWnd;
	strPFWnd="Soil Data Base";
	if (!m_wndPF.Create(strPFWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_PFWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Soil Properties window\n");
		return FALSE; // failed to create
	}

	m_wndPF.EnableDocking(CBRS_ALIGN_ANY);
	
	DockPane(&m_wndPF);
	MySingle::instance().SetPFCugPointer(&m_wndPF.m_PFgrid);

	CString strSoilDataWnd;
	strPFWnd="Soil Profile Data";
	if (!m_wndSoilData.Create(strPFWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_SOILDATAWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Soil Properties window\n");
		return FALSE; // failed to create
	}

	m_wndSoilData.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndSoilData);
	m_wndSoilData.DockToWindow(&m_wndPF,CBRS_RIGHT);
	MySingle::instance().SetSoilDataPointer(&m_wndSoilData.m_SoilDataGrid);

	m_wndPF.m_PFgrid.SetPointerToProfile(&m_wndSoilData.m_SoilDataGrid);

	CString strDBWnd;
	strDBWnd="DB Info";
	if (!m_wndDB.Create(strDBWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_DBWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Soil Properties window\n");
		return FALSE; // failed to create
	}

	m_wndDB.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndDB);
	MySingle::instance().SetDBBasPointer(&m_wndDB.m_DBBasgrid);

	CString strDBDataWnd;
	strDBDataWnd="DB Data";
	if (!m_wndDBData.Create(strDBDataWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_DBDATAWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Soil Properties window\n");
		return FALSE; // failed to create
	}

	m_wndDBData.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndDBData);
	MySingle::instance().SetDBDataPointer(&m_wndDBData.m_DBDataGrid);


	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = nullptr;
	m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	//m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);

	CString strwndEvalMulti;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndEvalMulti.Create(strwndEvalMulti, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_EVALUATEMULTIWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}


	m_wndEvalMulti.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndEvalMulti);


	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);

	 string lDataBaseDirectory;
	lDataBaseDirectory=MFC_Util::GetProfileStringStd("DataBaseDirectory",lDataBaseDirectory);
	string fil=lDataBaseDirectory;
	fil+="Functions.csv";
	m_Func.Init(fil);
	/*if(m_ActDoc!=nullptr) {
		

	}
	string dir=

	*/
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// Create class view
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Class View window\n");
		return FALSE; // failed to create
	}

	// Create file view
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create File View window\n");
		return FALSE; // failed to create
	}

	// Create output window
/*	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndModel.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Output window\n");
		return FALSE; // failed to create
	}
	*/
	// Create properties window
/*	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}*/

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	//HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	//CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	//m_wndOutput.UpdateFonts();
}
BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void CMainFrame::OnUpdateBound1(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
  pCmdUI->SetText(m_error_min);
}
void CMainFrame::OnUpdateBound0(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
  pCmdUI->SetText(m_text);

}

void CMainFrame::OnUpdateBound2(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
  pCmdUI->SetText(m_warning_min);
}

void CMainFrame::OnUpdateBound3(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
  pCmdUI->SetText(m_warning_max);
}

void CMainFrame::OnUpdateBound4(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
  m_wndStatusBar.SetPaneText(5, m_error_max);  // alternate method
}

void CMainFrame::OnUpdateUnit(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
  m_wndStatusBar.SetPaneText(6, m_unit); 
}

// OTHER
void CMainFrame::UpdateProgress(int pane_number, float relfrac) {
	CString text;
	text="Calculating : ";

	text+=FUtil::STD_FtoAscii(100*relfrac).c_str();
	text+=" %";
	if (pane_number > 0) {
	
		switch(pane_number) {
			case 1:
				m_error_min=text;
				break;
			case 2:
				m_warning_min=text;
				break;
			case 3:
				m_warning_max=text;
				break;
			case 4:
				m_error_max=text;
				break;
			case 5:
				m_unit=text;
				break;
			case 6:
				m_text=text;
				break;
		}

   
	}
	else if(pane_number==0) {
			
		  SetMessageText(text);

	}


};

void CMainFrame::UpdatePaneText(int pane_number, string text) {
	if (pane_number > 0) {
	
		switch(pane_number) {
			case 1:
				m_error_min=text.c_str();
				break;
			case 2:
				m_warning_min=text.c_str();
				break;
			case 3:
				m_warning_max=text.c_str();
				break;
			case 4:
				m_error_max=text.c_str();
				break;
			case 5:
				m_unit=text.c_str();
				break;
			case 6:
				m_text=text.c_str();
				break;
		}

   
	}
	else if(pane_number==0) {

		  SetMessageText(CString(text.c_str()));

	}



}
void CMainFrame::UpdateLocalWindowText(string text, WINDOW_NAMES wname)
{
	switch (wname) {

	case WINDOW_PG:
		m_wndPG.SetWindowTextW(CString(text.c_str()));
		break;
	case WINDOW_SIM:
		m_wndModel.SetWindowTextW(CString(text.c_str()));
		break;
	case WINDOW_SOILDB:
		m_wndPF.SetWindowTextW(CString(text.c_str()));
		break;
	case WINDOW_SOIL:
		m_wndSoilData.SetWindowTextW(CString(text.c_str()));
		break;
	case WINDOW_PROP:
		m_wndProperties.SetWindowTextW(CString(text.c_str()));
		break;
	case WINDOW_EVAL:
		m_wndEvalMulti.SetWindowTextW(CString(text.c_str()));
		break;
	case WINDOW_DB:
		m_wndDB.SetWindowTextW(CString(text.c_str()));
		break;
	case WINDOW_DBDATA:
		m_wndDBData.SetWindowTextW(CString(text.c_str()));
		break;

	}

}
;
void CMainFrame::UpdateResidualProgress(int var, int numvar, float relfrac, size_t transform) {

	CString transformtext;
	transformtext = "";
	switch (transform) {
	case 0:
		transformtext = "Actual values ";
		break;
	case 1:
		transformtext = "Cumulated values ";
		break;
	case 2:
		transformtext = "Year Cumulated values ";
		break;
	case 3:
		transformtext = "Day Cumulated values ";
		break;
	}

	CString text;
	text="Calculating var ";
	text+=FUtil::STD_ItoAscii(var).c_str();
	text+="(";
	text+=FUtil::STD_ItoAscii(numvar).c_str();
	text += ") for transformation ";
	text += transformtext;
	text+=FUtil::STD_FtoAscii(100*relfrac).c_str();
	text+=" % done";

	SetMessageText(text);

}
void CMainFrame::SetNoDocumentValid(bool value)
{
	m_DocumentValid=value;

};
 bool CMainFrame::IsDocumentValid()
 {

	 return m_DocumentValid;
 };