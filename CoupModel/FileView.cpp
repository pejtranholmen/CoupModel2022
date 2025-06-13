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

#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "CoupModel.h"
#include "CoupModelDoc.h"
#include "CoupModelView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
	MySingle::instance().SetFilePointer(this);
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_NOTIFY(TVN_SELCHANGED, 4, OnSelchangedTree)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill in some static tree view data (dummy code, nothing magic here)
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView()
{
	CWinApp* pApp = AfxGetApp();
	CMultiDocTemplate* pDocTemplate;
	CCoupModelApp *pCoupModel=(CCoupModelApp*)pApp;
	CDocument *pDocument;
	
	pDocTemplate=pCoupModel->m_pDocTemplate;

	PGFileArchive *pPGArchive=MySingle::instance().GetPGFileArchivePointer();
	SimArchive *pSimArchive=MySingle::instance().GetSimArchivePointer();




	CPG *pCPG;


	m_wndFileView.DeleteAllItems();
	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("Active PG files"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	int nfile=pPGArchive->GetNumPGFile();
	HTREEITEM hPG;
	hPG=nullptr;
	for (int i=0; i<nfile;i++) {
		pCPG=pPGArchive->GetPGFile(i);
		CString str;
		str=pCPG->GetFileName().c_str();
		hPG = m_wndFileView.InsertItem(str, 2, 2, hRoot);
	}
	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	if(hPG!=nullptr)
		m_wndFileView.Expand(hPG, TVE_EXPAND);
	
	hRoot = m_wndFileView.InsertItem(_T("Completed Simulations"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	HTREEITEM hSrc = m_wndFileView.InsertItem(_T("Single Run"), 0, 0, hRoot);
	HTREEITEM hMrc = m_wndFileView.InsertItem(_T("Multi Run"), 0, 0, hRoot);
	HTREEITEM hToBe = m_wndFileView.InsertItem(_T("Simulations to be done"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	CCoupModelDoc* dg_pDoc;

	POSITION pos2= pDocTemplate->GetFirstDocPosition();
	int numdoc;
	numdoc=0;
		
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*)pCoupModel->m_pMainWnd;;	

	while(pos2&&p_mainframe->IsDocumentValid()) { 
			if(pDocument = pDocTemplate->GetNextDoc(pos2)) {
			dg_pDoc =(CCoupModelDoc*)pDocument;
			CString str=CString(dg_pDoc->m_Doc.GetCurrentSimFile().c_str());
			if(dg_pDoc->m_Doc.m_DocumentType==0) {
				if(dg_pDoc->m_Doc.IsFinishedSimulation()){
					if(dg_pDoc->m_Doc.IsMultiSimulation())
						m_wndFileView.InsertItem(str, 1, 1, hMrc);
					else
						m_wndFileView.InsertItem(str, 1, 1, hSrc);
				}
				else
					m_wndFileView.InsertItem(str, 1, 1, hToBe);
				numdoc++;
			}
			else if(dg_pDoc->m_Doc.m_DocumentType>0) {

				numdoc++;
			}
		}
	}

	SimArchive *pAr;
	pAr=MySingle::instance().GetSimArchivePointer();
	size_t i=pAr->GetNumSimDoc();
	Doc* pMySimDoc;
	CCoupModelDoc *pMyDoc;
	for(size_t i=0; i<pAr->GetNumSimDoc(); i++) {
		pMySimDoc=pAr->GetSimDocPointer(i);
		pMyDoc=pAr->GetDocPointer(i);


	}

	if(numdoc>0) {

		 CMDIChildWnd * pChild =
          ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
		 if(pChild) {
			dg_pDoc = (CCoupModelDoc*)pChild->GetActiveDocument();
			if(dg_pDoc!=nullptr) {
				MySingle::instance().GetCugPointer()->SetDocument(dg_pDoc);
				if(dg_pDoc->m_Doc.m_DocumentType==size_t(0))
					MySingle::instance().GetCugPointer()->OnSetup();
				else
					MySingle::instance().GetPGCugPointer()->OnSetup();
				p_mainframe->m_wndModel.SetFocus();
				p_mainframe->m_wndModel.ActivateTopParent();
			}
		 }
	}
	




	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hSrc, TVE_EXPAND);
	m_wndFileView.Expand(hToBe, TVE_EXPAND);
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != nullptr)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	AfxMessageBox(_T("Properties...."));

}

void CFileView::OnFileOpen()
{
	// TODO: Add your command handler code here
}

void CFileView::OnFileOpenWith()
{
	// TODO: Add your command handler code here
}

void CFileView::OnDummyCompile()
{
	// TODO: Add your command handler code here
}

void CFileView::OnEditCut()
{
	// TODO: Add your command handler code here
}

void CFileView::OnEditCopy()
{
	// TODO: Add your command handler code here
}

void CFileView::OnEditClear()
{
	// TODO: Add your command handler code here
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Locked */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

void CFileView::OnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CWinApp* pApp = AfxGetApp();


	*pResult = 0;
	HTREEITEM hItem = m_wndFileView.GetSelectedItem();
	CString m_Name=m_wndFileView.GetItemText(hItem);
	
	CCoupModelApp *pCoupModel=(CCoupModelApp*)pApp;
	CMultiDocTemplate* pDocTemplate;
	
	pDocTemplate=pCoupModel->m_pDocTemplate;


	CView *pView;
	CCoupModelDoc* dg_pDoc;
	CDocument *pDocument;

	POSITION pos2= pDocTemplate->GetFirstDocPosition();
	int numdoc;
	numdoc=0;
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();		
	

	while(pos2) { 
			if(pDocument = pDocTemplate->GetNextDoc(pos2)) {

			POSITION posd=pDocument->GetFirstViewPosition();
			pView=pDocument->GetNextView(posd);
			dg_pDoc =(CCoupModelDoc*)pDocument;
			CString str=CString(dg_pDoc->m_Doc.GetCurrentSimFile().c_str());

			if(dg_pDoc->m_Doc.m_DocumentType==2&&str==m_Name) {
				pView->GetParentFrame()->ActivateFrame();
				p_mainframe->m_wndPG.m_PGgrid.SetDocument(dg_pDoc);
				p_mainframe->m_wndPG.m_PGgrid.OnSetup();	
			}
			else if(str==m_Name){
				pView->GetParentFrame()->ActivateFrame();
				MySingle::instance().GetCugPointer()->SetDocument(dg_pDoc);
				MySingle::instance().GetCugPointer()->OnSetup();


			}

		}
	}
	

}
