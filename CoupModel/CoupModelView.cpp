// CoupModelView.cpp : implementation of the CCoupModelView cla
#include "stdafx.h"
#include ".\ModelTypes\SimB.h"
#include "math.h"

#include "MainFrm.h"
#include "CoupModel.h"
#include ".\CUG\SimDoc\MyCug.h"
#include ".\CUG\ChartProperties\ChartCug.h"
#include ".\CoupModelView.h"

#include ".\NewBase\MR.h"

#include "htmlhelp.h"
#ifndef COUP2013
#include ".\SQL\SQL_Exchange.h"
#endif

#include ".\Chart\ChartViewer.h"



static CCoupModelDoc* pRunDoc;
static CCoupModelDoc* pstatDoc;

extern int ViewStatus;

extern bool MultiRunning;




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCoupModelView

IMPLEMENT_DYNCREATE(CCoupModelView, CFormView)

BEGIN_MESSAGE_MAP(CCoupModelView, CFormView)
	//{{AFX_MSG_MAP(CCoupModelView)

	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)

	ON_WM_ACTIVATE()
	ON_WM_QUERYDRAGICON()
	
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_CONTROL(CVN_ViewPortChanged, IDC_CHART, OnViewPortChanged)
	

	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHART, IDC_CHART, OnChartClick)
	ON_CONTROL_RANGE(BN_DBLCLK, IDC_CHART, IDC_CHART, OnChartDoubleClick)

	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_SIM_START, OnUpdateSimStart)
	ON_COMMAND(ID_SIM_START, OnSimStart)
	ON_UPDATE_COMMAND_UI(ID_SIM_MULTI_START, OnUpdateSimMultiStart)
	ON_COMMAND(ID_SIM_MULTI_START, OnSimMultiStart)
	ON_UPDATE_COMMAND_UI(ID_SIM_DOCNEW, OnUpdateDocNew)
	ON_COMMAND(ID_SIM_DOCNEW, OnDocNew)
	ON_COMMAND(ID_PGFILE_NEW, OnPgfileNew)
	ON_COMMAND(ID_PGFILE_OPEN, OnPgfileOpen)
	ON_UPDATE_COMMAND_UI(ID_PGFILE_NEW, OnUpdatePgfileNew)
	ON_UPDATE_COMMAND_UI(ID_PGFILE_OPEN, OnUpdatePgfileOpen)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_LINK, OnLinkDocuments)
	ON_COMMAND(ID_UNLINK, OnUnLinkDocuments)
	ON_UPDATE_COMMAND_UI(ID_LINK, OnUpdateLink)
	ON_UPDATE_COMMAND_UI(ID_UNLINK, OnUpdateUnLink)
	ON_COMMAND(ID_FILE_SAVE_CHART, OnChartFileSave)
	ON_COMMAND(ID_WND_ECHART, OnChartExtra)
	ON_COMMAND(ID_SHIFT_ECHART, OnChartShift)
	ON_UPDATE_COMMAND_UI(ID_SHIFT_ECHART, OnUpdateChartShift)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_CHART, OnUpdateChartFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_ECHART, OnUpdateChartFileExtraSave)
	ON_COMMAND(ID_FILE_SAVE_ECHART, OnChartFileExtraSave)
	ON_COMMAND(ID_WND_ECHART_CLEAN, OnChartCleanExtra)
	ON_UPDATE_COMMAND_UI(ID_WND_ECHART_CLEAN, OnUpdateChartCleanExtra)
	ON_COMMAND(ID_X_PLUS, OnXPlus)
	ON_COMMAND(ID_X_MINUS, OnXMinus)
	ON_COMMAND(ID_Y_PLUS, OnYPlus)
	ON_COMMAND(ID_Y_MINUS, OnYMinus)
	ON_UPDATE_COMMAND_UI(ID_X_MINUS, OnUpdateXMinus)
	ON_UPDATE_COMMAND_UI(ID_Y_MINUS, OnUpdateYMinus)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_IN, OnUpdateZoomIn)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_OUT, OnUpdateZoomOut)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_HAND, OnUpdateZoomHand)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_FORWARD, OnUpdateZoomForward)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_BACKWARD, OnUpdateZoomBackward)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_BUTTON_HAND, OnZoomHand)
	ON_COMMAND(ID_ZOOM_FORWARD, OnZoomForward)
	ON_COMMAND(ID_ZOOM_BACKWARD, OnZoomBackward)
	ON_UPDATE_COMMAND_UI(ID_VAL_FILE, OnUpdateValFile)
	ON_UPDATE_COMMAND_UI(ID_VAL_ADDTSV, OnUpdateValAddTsv)
	ON_UPDATE_COMMAND_UI(ID_VAL_ADDSV, OnUpdateValAddSv)
	ON_UPDATE_COMMAND_UI(ID_VAL_VIEW, OnUpdateValView)
	ON_UPDATE_COMMAND_UI(ID_VAL_UNLOCK, OnUpdateValUnLock)
	ON_UPDATE_COMMAND_UI(ID_VAL_ACCEPT, OnUpdateCreatePGFile)
	ON_COMMAND(ID_VAL_FILE, OnValFile)
	ON_COMMAND(ID_VAL_ADDTSV, OnValAddTsv)
	ON_COMMAND(ID_VAL_ADDSV, OnValAddSv)
	ON_COMMAND(ID_VAL_VIEW, OnValView)
	ON_COMMAND(ID_VAL_UNLOCK, OnValUnLock)
	ON_COMMAND(ID_VAL_ACCEPT, OnCreatePGFile)
	ON_COMMAND(ID_MULTI_NEWCURVE, OnMultiNewCurve)
	ON_COMMAND(ID_MULTI_S_SHIFT, OnMultiNewRun)
	ON_COMMAND(ID_MULTI_M_SHIFT, OnMultiNewRunAll)
	ON_UPDATE_COMMAND_UI(ID_MULTI_NEWCURVE, OnUpdateMultiNewCurve)
	ON_UPDATE_COMMAND_UI(ID_MULTI_S_SHIFT, OnUpdateMultiNewRun)
	ON_UPDATE_COMMAND_UI(ID_MULTI_M_SHIFT, OnUpdateMultiNewRunAll)
END_MESSAGE_MAP()

void CCoupModelView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCoupModelView)
	DDX_Control(pDX, IDC_CHART, m_ChartViewer);
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CCoupModelView construction/destruction

CCoupModelView::CCoupModelView()
	:CFormView(CCoupModelView::IDD)
{
  	CCoupModelApp* pApp = (CCoupModelApp*)AfxGetApp();
	pRunDoc=nullptr;

 	m_View_SumVarMode=FALSE;
	m_View_LeftMove=FALSE;
	m_View_RightMove=FALSE;
	m_SumType1=-1;
	m_biotic=FALSE;
	m_pageSW_Specb=0;
	m_pageSW_Specb=0; m_pageSW_Spec=0; m_pageSW_Tech=0; m_pageTab=0; m_pageTab_B=0;
	m_pageP=0; m_pageP_B=0;
	m_pageX=0; m_pageT=0; m_pageG=0; m_pageD=0;
	m_HistoryDiff=1;

	m_plantscale=0;
	m_CUGDisplayed=0;
	m_isPrinting=FALSE;
	m_OverLayPF=FALSE;
	m_UpdateGrid=TRUE;
	m_OKWriteFile=TRUE;
	m_RealTimeMode=FALSE;
	m_pChartProp=MySingle::instance().GetChartPropPointer();

	//m_grid1= new MyCug();
	//m_pDoc=nullptr;
}

CCoupModelView::~CCoupModelView()
{


}
BOOL CCoupModelView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCoupModelView drawing

void CCoupModelView::OnDraw(CDC* pDC)
{
	CCoupModelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->m_Doc.SetModelView(this);

	if(!MySingle::instance().IsActDoc(pDoc)) {
		CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
		if(pChild)	{
			if(pChild->GetActiveDocument()==pDoc) {
				pDoc->m_Doc.SetModelView(this);
				if(pDoc->m_Doc.m_DocumentType==0) {
					MySingle::instance().GetCugPointer()->SetDocument(pDoc);
					MySingle::instance().GetCugPointer()->OnSetup();
					MySingle::instance().GetCugPointer()->ShowWindow(SW_SHOW);
					MySingle::instance().GetClassViewPointer()->FillClassView();
				}
				else {
					MySingle::instance().GetPGCugPointer()->SetDocument(pDoc);
					MySingle::instance().GetPGFileArchivePointer()->SetInVisible();
					size_t i=MySingle::instance().GetPGFileArchivePointer()->GetIndex(pDoc);
					MySingle::instance().GetPGFileArchivePointer()->SetVisible(i);
					MySingle::instance().GetPGCugPointer()->OnSetup();
					MySingle::instance().GetPGCugPointer()->ShowWindow(SW_SHOW);
					//MySingle::instance().GetFileViewPointer()->FillFileView();
				}
				MySingle::instance().SetActDoc(pDoc);
			}
		}

	}
	return;

}

/////////////////////////////////////////////////////////////////////////////
// CCoupModelView printing

BOOL CCoupModelView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCoupModelView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CCoupModelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int pageHeight= pDC->GetDeviceCaps(VERTRES);
	int logPixelsY= pDC->GetDeviceCaps(LOGPIXELSY);
	int numpages=abs(m_position)/800+1;
	pInfo->SetMaxPage(numpages);
	// TODO: add extra initialization before printing
}

void CCoupModelView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
}
void CCoupModelView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CSize sizeTotal;

		CRect winRect;
	GetWindowRect(&winRect);
	// TODO: calculate the total size of this view
	//sizeTotal.cx = sizeTotal.cy = 100;


}
/////////////////////////////////////////////////////////////////////////////
// CCoupModelView message handlers

/*void CCoupModelView::OnDestroy() 
{
	CChartViewer::OnDestroy();
}*/

void CCoupModelView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	m_pChartProp->SetDisplaySize(cx, cy);
	m_cx=cx;
	m_cy=cy;

}

void CCoupModelView::writeStatus(CString &message) {
  CMainFrame *p_parent;
  p_parent = (CMainFrame*) GetParentFrame();
  p_parent->SetMessageText(message);
  Invalidate();

}

/////////////////////////////////////////////////////////////////////////////
// CCoupModelView diagnostics


/////////////////////////////////////////////////////////////////////////////
// CCoupModelView diagnostics
CCoupModelDoc* CCoupModelView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCoupModelDoc)));
	return (CCoupModelDoc*)m_pDocument;
}


#ifdef _DEBUG

void CCoupModelView::AssertValid() const
{
	CView::AssertValid();
}
void CCoupModelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

void CCoupModelView::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
    CCoupModelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CWinApp* pApp = AfxGetApp();
	pCmdUI->Enable(TRUE);
}
void CCoupModelView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnPrepareDC(pDC, pInfo);
	SIZE ViewSize;
	pDC->SetMapMode(MM_ANISOTROPIC);
	ViewSize=pDC->GetWindowExt();

    
	logFonta.lfHeight = 20*m_intScaleFont/20+8;
    logFonta.lfWidth = 0;
    logFonta.lfEscapement = 0;
    logFonta.lfOrientation = 0;
    logFonta.lfWeight = FW_NORMAL;
    logFonta.lfItalic = 0;
    logFonta.lfUnderline = 0;
    logFonta.lfStrikeOut = 0;
    logFonta.lfCharSet = ANSI_CHARSET;
    logFonta.lfOutPrecision = OUT_DEFAULT_PRECIS;
    logFonta.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    logFonta.lfQuality = PROOF_QUALITY;
    logFonta.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(CT2A(logFonta.lfFaceName), "MS Sans Serif");

	logFont_i.lfHeight = 20*m_intScaleFont/20+8;
    logFont_i.lfWidth = 0;
    logFont_i.lfEscapement = 0;
    logFont_i.lfOrientation = 0;
    logFont_i.lfWeight = 600;
    logFont_i.lfItalic = TRUE;
    logFont_i.lfUnderline = 0;
    logFont_i.lfStrikeOut = 0;
    logFont_i.lfCharSet = ANSI_CHARSET;
    logFont_i.lfOutPrecision = OUT_DEFAULT_PRECIS;
    logFont_i.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    logFont_i.lfQuality = PROOF_QUALITY;
    logFont_i.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(CT2A(logFont_i.lfFaceName), "MS Sans Serif");



    logFont.lfHeight = 24*m_intScaleFont/20+10;
    logFont.lfWidth = 0;
    logFont.lfEscapement = 0;
    logFont.lfOrientation = 0;
    logFont.lfWeight = FW_BOLD;
    logFont.lfItalic = 0;
    logFont.lfUnderline = 0;
    logFont.lfStrikeOut = 0;
    logFont.lfCharSet = ANSI_CHARSET;
    logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
    logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    logFont.lfQuality = PROOF_QUALITY;
    logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	strcpy(CT2A(logFont.lfFaceName), "MS Sans Serif");


	if(pDC->IsPrinting()){
		int pageHeight=pDC->GetDeviceCaps(VERTRES);
		int pageWidth=pDC->GetDeviceCaps(HORZRES);
		int originY=pageHeight*(pInfo->m_nCurPage-1);
		pDC->SetWindowExt(1600,1000);
		pDC->SetViewportExt(pageWidth, -pageHeight);
		pDC->SetViewportOrg(200,-originY);
		m_IsPrinting=TRUE;
		m_pageheight=1000;
		m_margtop=50;
		m_Margbottom=30;
	}
	else {
		pDC->SetWindowExt(1400,400);
		int pageHeight=pDC->GetDeviceCaps(VERTRES);
		int pageWidth=pDC->GetDeviceCaps(HORZRES);
		pDC->SetViewportExt(pageWidth, -pageHeight);
		m_IsPrinting=FALSE;
		m_margtop=0;
	
	}
	ViewSize=pDC->GetWindowExt();
	
}
void CCoupModelView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	CCoupModelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CString pzPathName=pDoc->CDocument::GetPathName();
   	CFont* oldFont;
    CFont* fonta = new CFont();

    fonta->CreateFontIndirect(&logFonta);
	oldFont = pDC->SelectObject(fonta);

	
	CString  page;
	page.Format(_T("Page %u"), pInfo->m_nCurPage);

	CTime t = CTime::GetCurrentTime();
	CString s = t.Format( "%A, %B %d, %Y. %H:%M    " );

	pDC->TextOut(20, (pInfo->m_nCurPage-1)*-1000-950, pzPathName+pDoc->m_Doc.m_DocFile2.m_ExeFileDate.c_str()+s);
	pDC->TextOut(1450,(pInfo->m_nCurPage-1)*-1000-950, page);	
//	CScrollView::OnPrint(pDC, pInfo);

	pDC->SelectObject(oldFont);
	delete fonta;
}
void CCoupModelView::MakeChart(BaseChart *pChart)
{
	// Get the size of the window
	if(&m_ChartViewer==nullptr) return;
		CCoupModelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->m_Doc.SetModelView(this);
	if(pChart!=nullptr) 
			m_ChartViewer.setChart(pChart);
	// Show the charts
	m_ChartViewer.ShowWindow(TRUE);

	
}
void CCoupModelView::OnChartClick(UINT nID)
{
	// In this demo program, all ChartViewers are handled by this handler. We need to 
	// retrieve the ChartViewer that generates the message and get its ImageMapHandler.
	int chartViewerIndex = nID - IDC_CHART;
	ImageMapHandler *handler = m_ChartViewer.getImageMapHandler();
	
	if (0 != handler)
	{
		// Query the ImageMapHandler to see if the mouse is on a clickable hot spot. We 
		// consider the hot spot as clickable if its href ("path") parameter is not empty.
		const char *path = handler->getValue("label");
		if ((0 != path) && (0 != *path))
		{
			// In this sample code, we just show all hot spot parameters.
			//CHotSpotDlg hs;
			//hs.SetData(handler);
			//hs.DoModal();
		}
	}
}
void CCoupModelView::OnChartDoubleClick(UINT nID)
{
	// In this demo program, all ChartViewers are handled by this handler. We need to 
	// retrieve the ChartViewer that generates the message and get its ImageMapHandler.
	int chartViewerIndex = nID - IDC_CHART;
	ImageMapHandler *handler = m_ChartViewer.getImageMapHandler();
	
	if (0 != handler)
	{
		// Query the ImageMapHandler to see if the mouse is on a clickable hot spot. We 
		// consider the hot spot as clickable if its href ("path") parameter is not empty.
		const char *path = handler->getValue("label");
		if ((0 != path) && (0 != *path))
		{
			// In this sample code, we just show all hot spot parameters.
			//CHotSpotDlg hs;
			//hs.SetData(handler);
			//hs.DoModal();
		}
	}
}
void CCoupModelView::OnViewPortChanged()
{
	CCoupModelDoc* pDoc = GetDocument();
	if(m_RealTimeMode&&!pDoc->m_Doc.IsFinishedSimulation())
		MySingle::instance().GetChartCugPointer()->m_RealTime.OnViewPortChanged();
	else if(!m_RealTimeMode) {
		auto pZoom=MySingle::instance().GetChartZoomPointer();
		pZoom->OnViewPortChanged();

	}
		
}
void CCoupModelView::OnUpdateSimStart(CCmdUI* pCmdUI)
{
	BOOL val=FALSE;
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		if(!pDoc->m_Doc.IsFinishedSimulation()&&pDoc->m_Doc.m_DocumentType==0)
			val=TRUE;
		pCmdUI->Enable(val);
	}

}
void CCoupModelView::OnSimStart()
{
	MySingle::instance().GetCugPointer()->MakeSingleRun();

}

void CCoupModelView::OnUpdateSimMultiStart(CCmdUI* pCmdUI)
{
	BOOL val=FALSE;
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		if(!pDoc->m_Doc.IsFinishedSimulation()&&pDoc->m_Doc.m_MultiRun_Array.size()>0)
			val=TRUE;
		pCmdUI->Enable(val);
	}

}
void CCoupModelView::OnSimMultiStart()
{

	MySingle::instance().GetCugPointer()->MakeMultiRuns(1);

}
void CCoupModelView::OnUpdateDocNew(CCmdUI* pCmdUI)
{
	BOOL val=FALSE;
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		if(pDoc->m_Doc.IsFinishedSimulation())
			val=TRUE;
		pCmdUI->Enable(val);
	}

}
void CCoupModelView::OnDocNew()
{
	MySingle::instance().GetCugPointer()->CreateNewDocFromCurrentDoc();
}
void CCoupModelView::OnPgfileNew()
{
	CMainFrame *p_mainframe;

	size_t numfile0=MySingle::instance().GetPGFileArchivePointer()->GetNumPGFile();
	MyPGCug *pPGCug=MySingle::instance().GetPGCugPointer();
	p_mainframe = (CMainFrame*) AfxGetMainWnd();
	p_mainframe->m_wndPG.ShowWindow(SW_SHOW);

	if (numfile0 > 0) {
		pPGCug->OnSheetSetup(8);
	}
	else {
		pPGCug->OnSetup();
		pPGCug->OnSheetSetup(0);
	}

}


void CCoupModelView::OnPgfileOpen()
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	p_mainframe->m_wndPG.ShowWindow(SW_SHOW);
	p_mainframe->m_wndPG.m_PGgrid.FileOpen();
	p_mainframe->m_wndPG.m_PGgrid.OnSetup();
	p_mainframe->m_wndFileView.FillFileView();
}
void CCoupModelView::OnUpdatePgfileNew(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();

	BOOL val=TRUE;
	pCmdUI->Enable(val);

}


void CCoupModelView::OnUpdatePgfileOpen(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();

	BOOL val=TRUE;
	pCmdUI->Enable(val);


}
void CCoupModelView::OnHelp()
{

	MySingle::instance().GetCugPointer()->GetHelp();

}
void CCoupModelView::OnLinkDocuments()
{

	MySingle::instance().GetCugPointer()->UpdateDocumentLink();
	MySingle::instance().GetCugPointer()->LinkDocuments();

}
void CCoupModelView::OnUnLinkDocuments()
{
	MySingle::instance().GetCugPointer()->UnLinkDocuments();
	
}
void CCoupModelView::OnUpdateLink(CCmdUI* pCmdUI)
{
	BOOL val=FALSE;
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		if(pDoc->m_Doc.IsFinishedSimulation()&&pDoc->m_Doc.m_SelectedDocuments.size()<=1)
			val=TRUE;
		pCmdUI->Enable(val);
	}

}
void CCoupModelView::OnUpdateUnLink(CCmdUI* pCmdUI)
{
	BOOL val=FALSE;
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		if(pDoc->m_Doc.IsFinishedSimulation()&&pDoc->m_Doc.m_SelectedDocuments.size()>1)
			val=TRUE;
		pCmdUI->Enable(val);
	}

}
void CCoupModelView::OnChartFileSave()
{
	CCoupModelDoc* pDoc = GetDocument();
	MySingle::instance().GetChartUtilPointer()->m_FileSave.MakeFile(&pDoc->m_Doc);
}
void CCoupModelView::OnChartFileExtraSave()
{
	CCoupModelDoc* pDoc = GetDocument();
	MySingle::instance().GetChartUtilPointer()->m_FileSave.SaveToFileExtraChart(&pDoc->m_Doc);
}
void CCoupModelView::OnChartExtra()
{
	MySingle::instance().GetChartUtilPointer()->m_FileSave.SaveToExtraChartWindow();
}
void CCoupModelView::OnChartCleanExtra()
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	//p_mainframe->m_wndChartView.CleanChartList();
}
void CCoupModelView::OnChartShift()
{
	MySingle::instance().GetChartUtilPointer()->m_FileSave.ShiftExtraChartWindow();
}
void CCoupModelView::OnUpdateChartFileSave(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	BOOL val=FALSE;
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		val=TRUE;
		pCmdUI->Enable(val);
	}
}
void CCoupModelView::OnUpdateChartFileExtraSave(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	BOOL val=FALSE;
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		//if(p_mainframe->m_wndChartView.m_ExtraCharts.size()>0)
	//		val=TRUE;
		//pCmdUI->Enable(val);
	}
}
void CCoupModelView::OnUpdateChartExtra(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	BOOL val=FALSE;
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		val=TRUE;
		pCmdUI->Enable(val);
	}
}
void CCoupModelView::OnUpdateChartShift(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	BOOL val=FALSE;
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		val=FALSE;
	//	if(p_mainframe->m_wndChartView.m_ExtraCharts.size()>1)
	//		val=TRUE;
	//	pCmdUI->Enable(val);
	}
}
void CCoupModelView::OnUpdateChartCleanExtra(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	BOOL val=FALSE;
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
	//	if(p_mainframe->m_wndChartView.m_ExtraCharts.size()>1)
	//		val=TRUE;
	//	pCmdUI->Enable(val);
	}
}
void CCoupModelView::OnUpdateXMinus(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	BOOL val=FALSE;
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		if(MySingle::instance().GetChartPropPointer()->m_NumChartX>1)
			val=TRUE;
		pCmdUI->Enable(val);
	}
}
void CCoupModelView::OnUpdateYMinus(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	BOOL val=FALSE;
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		if(MySingle::instance().GetChartPropPointer()->m_NumChartY>1)
			val=TRUE;
		pCmdUI->Enable(val);
	}
}
void CCoupModelView::OnXPlus()
{
	MySingle::instance().GetCugPointer()->ManagePlotWindows(4006,0);
	if(!MySingle::instance().GetChartUtilPointer()->m_TablePlotMode)
		MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.MakeTimeSeriePlot();
}
void CCoupModelView::OnXMinus()
{

	MySingle::instance().GetCugPointer()->ManagePlotWindows(4007,0);
	if(!MySingle::instance().GetChartUtilPointer()->m_TablePlotMode)
	MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.MakeTimeSeriePlot();

}
void CCoupModelView::OnYPlus()
{
	MySingle::instance().GetCugPointer()->ManagePlotWindows(4008,0);
	if(!MySingle::instance().GetChartUtilPointer()->m_TablePlotMode)
		MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.MakeTimeSeriePlot();

}
void CCoupModelView::OnYMinus()
{

	MySingle::instance().GetCugPointer()->ManagePlotWindows(4009,0);
	MySingle::instance().GetChartUtilPointer()->m_FileSave.OnReDraw(TRUE);


}
void CCoupModelView::OnUpdateZoomIn(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	BOOL val=FALSE;
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		if(m_ChartViewer.getMouseUsage()!=Chart::MouseUsageZoomIn&&(MySingle::instance().GetChartPropPointer()->m_ChartType== CHART_TYPES::TIME_SERIE ||MySingle::instance().GetChartPropPointer()->m_ChartType ==CHART_TYPES::CONTOUR_CHART))
			val=TRUE;
		pCmdUI->Enable(val);
	}
}
void CCoupModelView::OnUpdateZoomOut(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();

	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		BOOL val=FALSE;
		if(m_ChartViewer.getViewPortWidth()<1)
			val=TRUE;
		pCmdUI->Enable(val);
	}
}
void CCoupModelView::OnUpdateZoomHand(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();

	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		BOOL val=FALSE;
		if(m_ChartViewer.getViewPortWidth()<1&&(m_ChartViewer.getMouseUsage()==Chart::MouseUsageZoomIn||m_ChartViewer.getMouseUsage()==Chart::MouseUsageZoomOut))
			val=TRUE;
		pCmdUI->Enable(val);
	}
}
void CCoupModelView::OnUpdateZoomForward(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		BOOL val=FALSE;
		ChartCug *pCug;
		pCug=MySingle::instance().GetChartCugPointer();
		if(pCug==nullptr) return;
		if(pCug->m_TimeSeriePlot.m_Zoom.ZoomLevel>2)
			val=TRUE;
		pCmdUI->Enable(val);
	}
}
void CCoupModelView::OnUpdateZoomBackward(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		BOOL val=FALSE;
		ChartCug *pCug;
		pCug=MySingle::instance().GetChartCugPointer();
		if(pCug==nullptr) return;
		if(pCug->m_TimeSeriePlot.m_Zoom.Hpos>1)
			val=TRUE;
		pCmdUI->Enable(val);
	}
}
void CCoupModelView::OnZoomIn()
{
	m_RealTimeMode=false;
	MySingle::instance().GetChartUtilPointer()->m_FileSave.OnZoomInPB(&m_ChartViewer);
}
void CCoupModelView::OnZoomOut()
{
	MySingle::instance().GetChartUtilPointer()->m_FileSave.OnZoomOutPB(&m_ChartViewer);
}
void CCoupModelView::OnZoomHand()
{
	MySingle::instance().GetChartUtilPointer()->m_FileSave.OnPointerPB(&m_ChartViewer);
}
void CCoupModelView::OnZoomForward()
{
	MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.m_Zoom.Hpos++;
	MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.OnInit(&m_ChartViewer);
	MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.DrawTimeSeriePlot(&m_ChartViewer);
}
void CCoupModelView::OnZoomBackward()
{
	MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.m_Zoom.Hpos--;
	MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.OnInit(&m_ChartViewer);
	MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.DrawTimeSeriePlot(&m_ChartViewer);
}
void CCoupModelView::OnValFile()
{
	MySingle::instance().GetCugPointer()->OnAddValFile();
}

void CCoupModelView::OnValAddTsv()
{
	MySingle::instance().GetCugPointer()->OnAddTsvVal();
}
void CCoupModelView::OnValAddSv()
{
    MySingle::instance().GetCugPointer()->OnAddSvVal();
}
void CCoupModelView::OnValView()

{
	CCoupModelDoc* pDoc = GetDocument();
	pDoc->m_Doc.m_PlotStorage.Init(&pDoc->m_Doc);
	MySingle::instance().GetCugPointer()->OnValidationEvalView();
}
void CCoupModelView::OnValUnLock()
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	p_mainframe=MySingle::instance().GetFramePointer();
	CCoupModelDoc* pDoc = GetDocument();
	pDoc->m_Doc.m_PlotStorage.Init(&pDoc->m_Doc);
	pDoc->m_Doc.UnLockMultiStorageFile();
	p_mainframe->m_wndEvalMulti.m_ValidationCug.OnSetup();
}
void CCoupModelView::OnCreatePGFile()
{
	CCoupModelDoc* pDoc = GetDocument();
	pDoc->m_Doc.m_PlotStorage.Init(&pDoc->m_Doc);
	MySingle::instance().GetChartCugPointer()->MakePGFileFromDataInCurrentChart();
//	MySingle::instance().GetCugPointer()->OnValidationEval();



}
void CCoupModelView::OnUpdateValFile(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		BOOL val=FALSE;
		if(!pDoc->m_Doc.IsFinishedSimulation()&&pDoc->m_Doc.GetNumValidValFiles()<MAXSIMVAL&&pDoc->m_Doc.m_DocumentType==0)
			val=TRUE;
		pCmdUI->Enable(val);
	}
}

void CCoupModelView::OnUpdateValAddTsv(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		BOOL val=FALSE;
		if(!pDoc->m_Doc.IsFinishedSimulation()&&pDoc->m_Doc.GetNumValidValFiles()>0)
			val=TRUE;
		pCmdUI->Enable(val);
	}
}
void CCoupModelView::OnUpdateValAddSv(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		BOOL val=FALSE;
		if(!pDoc->m_Doc.IsFinishedSimulation()&&pDoc->m_Doc.m_DocumentType==0)
			val=TRUE;
		pCmdUI->Enable(val);
	}
}
void CCoupModelView::OnUpdateValView(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		BOOL val=FALSE;
		//if(pDoc->m_Doc.IsFinishedSimulation()&&pDoc->m_Doc.IsMultiSimulation()&&pDoc->m_Doc.IsMultiStorageFileLocked())
		//	val=TRUE;
		pCmdUI->Enable(val);
	}
}
void CCoupModelView::OnUpdateValUnLock(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		BOOL val=FALSE;
		if(pDoc->m_Doc.IsFinishedSimulation()&&pDoc->m_Doc.IsMultiSimulation()&&pDoc->m_Doc.IsMultiStorageFileLocked())
			val=TRUE;
		pCmdUI->Enable(val);
	}
}
void CCoupModelView::OnUpdateCreatePGFile(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		BOOL val=FALSE;
		auto m_pProp=MySingle::instance().GetChartPropPointer();
		//m_NumVarSelected=m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);
		if(pDoc->m_Doc.m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber)>0)
			val=TRUE;
		pCmdUI->Enable(val);
	}
}
void CCoupModelView::OnMultiNewCurve()
{
	MySingle::instance().GetChartCugPointer()->OnNewCurve();
}
void CCoupModelView::OnMultiNewRun()
{
	MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.OnUpdateSequence();
	MySingle::instance().GetChartUtilPointer()->m_FileSave.OnReDraw(true);
	
}
void CCoupModelView::OnMultiNewRunAll()
{
	MySingle::instance().GetChartCugPointer()->OnNewSeq2();

}
void CCoupModelView::OnUpdateMultiNewCurve(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		BOOL val=FALSE;
		MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.m_ChartInfo.MultiPGMode;
		if(pDoc->m_Doc.IsFinishedSimulation()&&pDoc->m_Doc.IsMultiSimulation()) {
			if(MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.m_ChartInfo.MultiPGMode||MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.m_ChartInfo.MultiValMode)
				val=TRUE;
		}
		else if(MySingle::instance().GetChartCugPointer()->m_PGRepit)
			val=TRUE;
		pCmdUI->Enable(val);
	}
	else {
		BOOL val=FALSE;
		if(MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.m_ChartInfo.MultiPGMode)
			val=TRUE;
		pCmdUI->Enable(val);
	}

}
void CCoupModelView::OnUpdateMultiNewRun(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();

	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		BOOL val=FALSE;
		if(pDoc->m_Doc.IsFinishedSimulation()&&pDoc->m_Doc.IsMultiSimulation()) {
			if(MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.m_ChartInfo.MultiPGMode||MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.m_ChartInfo.MultiValMode)
			val=TRUE;
		}
		else if(MySingle::instance().GetChartCugPointer()->m_PGRepit)
			val=TRUE;
		else if(MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.m_ChartInfo.PGRepitMode)
			val=TRUE;
		pCmdUI->Enable(val);

	}
	else {
		BOOL val=FALSE;
		if(MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.m_ChartInfo.MultiPGMode)
			val=TRUE;
		pCmdUI->Enable(val);
	}
	
}
void CCoupModelView::OnUpdateMultiNewRunAll(CCmdUI* pCmdUI)
{
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();

	CCoupModelDoc* pDoc = GetDocument();
	if(pDoc!=nullptr) {
		BOOL val=FALSE;
		if(pDoc->m_Doc.IsFinishedSimulation()&&pDoc->m_Doc.IsMultiSimulation()) {
			if(MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.m_ChartInfo.MultiPGMode||MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.m_ChartInfo.MultiValMode)
			val=TRUE;
		}
		else if(MySingle::instance().GetChartCugPointer()->m_PGRepit)
			val=TRUE;
		pCmdUI->Enable(val);
	}
	else {
		BOOL val=FALSE;
		if(MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.m_ChartInfo.MultiPGMode)
			val=TRUE;
		pCmdUI->Enable(val);
	}

}