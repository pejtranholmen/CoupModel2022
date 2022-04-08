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
#include "std.h"
#include "MainFrm.h"
#include "ClassView.h"
#include "Resource.h"
#include "CoupModel.h"
#include "CoupModelDoc.h"
#include "util.h"


class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CClassView;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = nullptr) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = FALSE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClassView::CClassView()
{
	m_nCurrSort = ID_SORTING_SORTBYTYPE;
	m_pDoc=nullptr;
	
	m_typeprior=_T("");
	m_groupprior=_T("");
	m_OnReDoing=false;
	MySingle::instance().SetModelPointer(this);
}

CClassView::~CClassView()
{
}

BEGIN_MESSAGE_MAP(CClassView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
//	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
	//ON_NOTIFY(TVN_SELCHANGED, 2, OnSelchangedTree)
	ON_NOTIFY(TVN_SELCHANGED, 2, OnSelchangedTree)
	ON_COMMAND(ID_SORTING_SORTBYGROUP, &CClassView::OnSortingSortbygroup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView message handlers

int CClassView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndClassView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	// Load images:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));


	if (pButton != nullptr)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// Fill in some static tree view data (dummy code, nothing magic here)
	FillClassView();

	return 0;
}

void CClassView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CClassView::FillClassView()
{	
//	SimB *pSimB;

	CString m_index, m_Col;
	CString namn;
	CString	grp,typ, status;
//	BOOL isgroup;
	BOOL IsTable, IsOutput, IsFinnished;
	//BOOL NewType;
	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = nullptr;
	tvInsert.hInsertAfter = TVI_SORT;
	tvInsert.item.mask = TVIF_TEXT;
	CString item1, item2, item3, lastItem;
	HTREEITEM hCountry, hPA, hTab;
	HTREEITEM hRoot; 
	hPA=nullptr;

/*
	HTREEITEM hRoot = m_wndClassView.InsertItem(_T("Model Components"), 0, 0);
	m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	HTREEITEM hClass = m_wndClassView.InsertItem(_T("Structure"), 1, 1, hRoot);
	m_wndClassView.InsertItem(_T("Structure"), 3, 3, hClass);
	m_wndClassView.Expand(hRoot, TVE_EXPAND);*/

	SimArchive *pAr;
	pAr=MySingle::instance().GetSimArchivePointer();
	size_t i=pAr->GetNumSimDoc();
	Doc* pMySimDoc;
	CCoupModelDoc *pMyDoc;
	for(size_t i=0; i<pAr->GetNumSimDoc(); i++) {
		pMySimDoc=pAr->GetSimDocPointer(i);
		pMyDoc=pAr->GetDocPointer(i);


	}

	
	CMainFrame *p_mainframe;
	p_mainframe = (CMainFrame*) AfxGetMainWnd();
	CCoupModelDoc *p_Doc=nullptr;
	if(p_mainframe!=nullptr&&p_mainframe->IsDocumentValid()) {
			IsFinnished=FALSE;

			CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
			if(pChild)	{
				CDocument *pDoc = pChild->GetActiveDocument();
				m_pDoc=pDoc;
				if(pDoc!=nullptr) {
					p_Doc=(CCoupModelDoc*)pDoc;
					IsFinnished=p_Doc->m_Doc.IsFinishedSimulation();
					hRoot=m_wndClassView.GetRootItem();
					while(hRoot!=nullptr) {
						m_wndClassView.DeleteItem(hRoot);
						hRoot = m_wndClassView.GetSelectedItem();
					}
				    m_wndClassView.DeleteAllItems();
				}
			}
	}
	if(p_Doc==nullptr||p_Doc->m_Doc.m_DocumentType>0) return;
	vector<SimB*> vvp;
	m_OnReDoing=TRUE;
	BOOL HasContent;
	if(m_nCurrSort==ID_SORTING_SORTBYTYPE) {

		for(size_t j=0;j<9;j++) {
			//enum simtype _typ;
		
			//vvp=p_Doc->m_Doc.GetPtrVector(p_ModelInfo->GetVarType(j),-1);
			typ=p_ModelInfo->GetVarType(j).c_str();
			if(j>3&&j<8) {
				if(j==4) {
					CString outtext;
					if(IsFinnished)
						outtext="Time series of Output Variables";
					else
						outtext="Selection of Output Variables";
					hRoot = m_wndClassView.InsertItem(TVIF_TEXT,outtext, 4, 4, 0,0,0,0,TVI_ROOT);
					m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
					HasContent=FALSE;
				}
				if(IsFinnished) {
					vector<SIMB> vsimb=p_Doc->m_Doc.GetAllSelectedOutputs();

					switch(j) {
						case 4:	vsimb=p_Doc->m_Doc.m_X_Array;break;
						case 5:	vsimb=p_Doc->m_Doc.m_T_Array;break;
						case 6:	vsimb=p_Doc->m_Doc.m_G_Array; break;
						case 7:	vsimb=p_Doc->m_Doc.m_D_Array;break;
					}

					vvp.resize(0);
					SimB* pSimB;
					if(vsimb.size()>0) {
						pSimB=vsimb[0].pBase;
						vvp.push_back(vsimb[0].pBase);
					}

					for(size_t i=1; i<vsimb.size(); i++) {
						if(pSimB!=vsimb[i].pBase) {
							pSimB=vsimb[i].pBase;
							vvp.push_back(vsimb[i].pBase);
						}
					}
				}
				else {
					vvp=p_Doc->m_Doc.GetAllPtrOutputs(j-4);
					vvp=p_ModelInfo->SortSimB_ByGroup(vvp);
				}
			}
			else {
				hRoot = m_wndClassView.InsertItem(TVIF_TEXT,typ, 5, 5, 0,0,0,0,TVI_ROOT);
				m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
				HasContent=FALSE;
				vvp=p_Doc->m_Doc.GetPtrVector(p_ModelInfo->GetVarType(j),-1);
				vvp=p_ModelInfo->SortSimB_ByGroup(vvp);
			}
			
			if(vvp.size()>0) {
					IsTable=FALSE;
					if(typ=="Tables") IsTable=TRUE;
					IsOutput=FALSE;
					if(typ=="State Variables"||typ=="Flow Variables"||typ=="Auxiliary Variables"||typ=="Driving Variables")
						IsOutput=TRUE;
					bool IsMulti=false;
					if(IsOutput) {
						IsMulti=p_Doc->m_Doc.IsMultiSimulation();
					}
					if(Make_Branch(hRoot, vvp, j, IsTable, IsOutput, IsFinnished, IsMulti))
						HasContent=TRUE;
			}
			if(j>3&&j<8) {
				if(j==7&&!HasContent)
					m_wndClassView.DeleteItem(hRoot);
			}
			else {
				if(!HasContent)
					m_wndClassView.DeleteItem(hRoot);
			}
		}
		// Add tree for Validation




			if(((CCoupModelDoc*)m_pDoc)->m_Doc.m_Val_Array.size()>0) {
				int mLoop=1;
				if(((CCoupModelDoc*)m_pDoc)->m_Doc.m_OutputValidXBinFileExist) {
					mLoop=3;

				}
				for (int im=0;im<mLoop;im++) {
					CString txt;
					switch(im) {
					case 0:
						txt="Validation Variables";
						break;
					case 1:
						txt="Mean of All Runs for Validation Variables";
						break;
					case 2:
						txt="Mean of Accepted Runs Validation Variables";
						break;
					}
					hRoot = m_wndClassView.InsertItem(TVIF_TEXT,txt, 4, 4, 0,0,0,0,TVI_ROOT);
					m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
					vector <size_t> v_int;
					VALv vst;
					((CCoupModelDoc*)m_pDoc)->m_Doc.UpdateValIndexList();
					for(size_t ityp=4;ityp<8;ityp++) {
						HasContent=FALSE;
						v_int=((CCoupModelDoc*)m_pDoc)->m_Doc.GetValIndexByType(ityp);
						if(v_int.size()>0) {
							for(size_t j=0; j<v_int.size();j++) {
								HasContent=TRUE;
								vst=((CCoupModelDoc*)m_pDoc)->m_Doc.m_Val_Array[v_int[j]];
								if(j==0){
									CString typ=CString(p_ModelInfo->GetVarType(ityp).c_str());
									hCountry = m_wndClassView.InsertItem(typ,ityp+8,0,hRoot);
								}
								int icat=p_ModelInfo->GetGroupCategory(vst.Group);
								CString kods;
								kods=":";
								item2=FUtil::STD_ItoAscii(v_int[j]+1).c_str();
								item2+=+kods;
								item2+=vst.Name.c_str();
								if(vst.LocalIndex>=0) {

									item2+="(";
									item2+=FUtil::STD_ItoAscii(vst.LocalIndex+1).c_str();
									item2+=")";
								}
								hPA = m_wndClassView.InsertItem(item2, icat+3, icat+3,  hCountry);

							}
						}	
					}

				}


			}


			// Add tree for summary variables
			if(((CCoupModelDoc*)m_pDoc)->m_Doc.IsFinishedSimulation()&&((CCoupModelDoc*)m_pDoc)->m_Doc.IsMultiSimulation()) {
					hRoot = m_wndClassView.InsertItem(TVIF_TEXT,_T("Statistics for Output Variables"), 0, 4, 0,0,0,0,TVI_ROOT);
					m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
					HasContent=FALSE;
					for(size_t isum=0; isum<6;isum++) {
						CString sumtyp;
						switch(isum) {
							case 0: sumtyp="Initial";break;
							case 1: sumtyp="Final";break;
							case 2: sumtyp="Min";break;
							case 3: sumtyp="Max";break;
							case 4: sumtyp="Mean";break;
							case 5: sumtyp="Accumulated";break;
						}
						hCountry = m_wndClassView.InsertItem(sumtyp,0,0,hRoot);

						for(size_t j=4;j<8;j++) {
							vector<SIMB> vsimb=p_Doc->m_Doc.GetAllSelectedOutputs();

							switch(j) {
								case 4:	vsimb=p_Doc->m_Doc.m_X_Array;break;
								case 5:	vsimb=p_Doc->m_Doc.m_T_Array;break;
								case 6:	vsimb=p_Doc->m_Doc.m_G_Array; break;
								case 7:	vsimb=p_Doc->m_Doc.m_D_Array;break;
							}

							vvp.resize(0);
							SimB* pSimB;
							if(vsimb.size()>0) {
								pSimB=vsimb[0].pBase;
								vvp.push_back(vsimb[0].pBase);
							}

							for(size_t i=1; i<vsimb.size(); i++) {
								if(pSimB!=vsimb[i].pBase) {
									pSimB=vsimb[i].pBase;
									vvp.push_back(vsimb[i].pBase);
								}
							}
							typ=p_ModelInfo->GetVarType(j).c_str();
							HasContent=FALSE;
							if(vvp.size()>0) {
									IsOutput=TRUE;
									if(Make_Branch(hCountry, vvp, j, IsTable, IsOutput, IsFinnished))
										HasContent=TRUE;
							}
							if(j==7&&!HasContent) {
									if(hPA!=nullptr) m_wndClassView.DeleteItem(hPA);
									m_wndClassView.DeleteItem(hCountry);
							}

						}
					}
			}
			// Add tree for completed Multi Runs
			if(((CCoupModelDoc*)m_pDoc)->m_Doc.IsFinishedSimulation()&&((CCoupModelDoc*)m_pDoc)->m_Doc.IsMultiSimulation()) {
				hRoot = m_wndClassView.InsertItem(TVIF_TEXT,_T("Performance for Validation Variables"), 4, 4, 0,0,0,0,TVI_ROOT);
				m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
				vector <size_t> v_int;
				VALv vst;
				((CCoupModelDoc*)m_pDoc)->m_Doc.UpdateValIndexList();
				for(size_t isum=0; isum<7;isum++) {
					CString sumtyp;
					switch(isum) {
						case 0: sumtyp="Coefficient of determination";break;
						case 1: sumtyp="Intercept";break;
						case 2: sumtyp="Slope";break;
						case 3: sumtyp="Mean Error";break;
						case 4: sumtyp="Root Mean Square Error";break;
						case 5: sumtyp="Log Likelihood sum";break;
						case 6: sumtyp="Modelling Efficiency (NSE)";break;
					}
					hCountry = m_wndClassView.InsertItem(sumtyp,0,0,hRoot);
					HasContent=FALSE;
					for(size_t ityp=4;ityp<8;ityp++) {
						v_int=((CCoupModelDoc*)m_pDoc)->m_Doc.GetValIndexByType(ityp);
						if(v_int.size()>0) {
							for(size_t j=0; j<v_int.size();j++) {
								HasContent=TRUE;
								vst=((CCoupModelDoc*)m_pDoc)->m_Doc.m_Val_Array[v_int[j]];
								if(j==0){
									CString typ=CString(p_ModelInfo->GetVarType(ityp).c_str());
									hPA = m_wndClassView.InsertItem(typ,ityp+8,0,hCountry);
								}
								int icat=p_ModelInfo->GetGroupCategory(vst.Group);
								item2=FUtil::STD_ItoAscii(v_int[j]+1).c_str();
								item2+=":"; item2+=vst.Name.c_str();
								if(vst.LocalIndex>=0) {
									item2+="(";
									item2+=FUtil::STD_ItoAscii(vst.LocalIndex+1).c_str();
									item2+=")";
								}
								hTab = m_wndClassView.InsertItem(item2, icat+3, icat+3,  hPA);
							}
						}	
					}
					if(!HasContent) m_wndClassView.DeleteItem(hCountry);
				}

			}
			// Add tree for completed Multi Runs
			if(((CCoupModelDoc*)m_pDoc)->m_Doc.IsFinishedSimulation()&&((CCoupModelDoc*)m_pDoc)->m_Doc.IsMultiSimulation()&&((CCoupModelDoc*)m_pDoc)->m_Doc.MR_Get_NumTotalPar()>0) {
				CString test;
				hRoot = m_wndClassView.InsertItem(TVIF_TEXT,_T("Statistics for MultiRun Parameters"), 4, 4, 0,0,0,0,TVI_ROOT);
				m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
				int icount=-1;
				for(size_t i=0;i<((CCoupModelDoc*)m_pDoc)->m_Doc.m_MultiRun_Array.size();i++){
					test="Dimension :";
					test+=FUtil::STD_ItoAscii(i+1).c_str();
					hCountry = m_wndClassView.InsertItem(test,0,0,hRoot);
					MapMR[i].clear();
		   			for(size_t ii=0; ii<((CCoupModelDoc*)m_pDoc)->m_Doc.MR_Get_NumberOfParametersWithinDim(i+1);ii++){
						icount++;
						test=((CCoupModelDoc*)m_pDoc)->m_Doc.MR_GetParGroup(i+1,ii+1).c_str();
						int icolcat=MFC_Util::GetGroupColor(p_ModelInfo->GetGroupCategory(CT2A(test).m_psz));

						test=((CCoupModelDoc*)m_pDoc)->m_Doc.MR_GetParName(i+1,ii+1).c_str();
						if(((CCoupModelDoc*)m_pDoc)->m_Doc.MR_GetTabIndex(i+1,ii+1)>-1) {
							test+=MFC_Util::AddIndexString(((CCoupModelDoc*)m_pDoc)->m_Doc.MR_GetTabIndex(i+1,ii+1)+1);
						}
						hPA = m_wndClassView.InsertItem(test,0,0,hCountry);
						MapMR[i].insert(pair<string,int> (CT2A(test).m_psz,ii));
					}
				}
			}
	}
	else if(m_nCurrSort==ID_SORTING_GROUPBYTYPE) {
		//Animation Variable display
		vector<SimB*> v;
		string typ;

		for(size_t ityp=5;ityp<9;ityp++) {

			switch (ityp) {
			case 5:
				typ=("State Variables");
				break;
			case 6:
				typ=("Flow Variables");
				break;
			case 7:
				typ=("Auxiliary Variables");
				break;
			case 8:
				typ=("Driving Variables");
				break;
			}
		
			v=((CCoupModelDoc*)m_pDoc)->m_Doc.GetPtrVector(ityp,-1);
			if(v.size()>0) {
				hRoot = m_wndClassView.InsertItem(TVIF_TEXT,CString(typ.c_str()), 1, 1, 0,0,0,0,TVI_ROOT);
				m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
				for(int i=1;i<36;i++) {
					v=((CCoupModelDoc*)m_pDoc)->m_Doc.GetPtrVector(typ, i);
					if(v.size()>0) {
						int itype=0;
						if(ityp>1)
							itype=1;
						Make_Branch(hRoot, v, itype);
					}
				}
			}
		}
		// Annimation
		//pos=1;
	}
	else {

		// Sort by Module

		bool first=true;

		for(size_t j=1;j<35;j++) {
			first=true;		
			BOOL HasContent=false;
			for(size_t t=0;t<9;t++) {
				if(t==3) t++;
				vvp=((CCoupModelDoc*)m_pDoc)->m_Doc.GetPtrVector(t,j);
				if(vvp.size()>0) {
					SimB *p;
					p=(SimB*)vvp[0];
					grp=p->GetGroup().c_str();
					int icat=p_ModelInfo->GetGroupCategory(CT2A(grp).m_psz);
					if(icat<0) icat=0;
			
					if(first) {
						hRoot = m_wndClassView.InsertItem(TVIF_TEXT,grp, 1, 1, 0,0,0,0,TVI_ROOT);
						m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
						first=false;
					}
					IsTable=FALSE;
					if(t==2) IsTable=TRUE;
					IsOutput=FALSE;
					if(t>3&&t<8) IsOutput=TRUE;

					if(Make_Branch(hRoot, vvp, t, IsTable, IsOutput,  IsFinnished))
						HasContent=true;
				
				}

			}

			if(!HasContent&&!first) {
				m_wndClassView.DeleteItem(hRoot);
			}


		}

	}
	m_OnReDoing=FALSE;
}

void CClassView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
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
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CClassView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndClassView.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CClassView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CClassView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}
	m_OnReDoing=true;
	m_nCurrSort = id;

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != nullptr)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
	FillClassView();
	m_OnReDoing=false;
}

void CClassView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CClassView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("Add member function..."));
}

void CClassView::OnClassAddMemberVariable()
{
	// TODO: Add your command handler code here
}

void CClassView::OnClassDefinition()
{
	// TODO: Add your command handler code here
}

void CClassView::OnClassProperties()
{
	// TODO: Add your command handler code here
}

void CClassView::OnNewFolder()
{
//	AfxMessageBox(_T("New Folder..."));
}

void CClassView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndClassView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CClassView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	HTREEITEM hItem = m_wndClassView.GetSelectedItem();

	m_wndClassView.SetFocus();
}

void CClassView::OnChangeVisualStyle()
{
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

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

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndClassView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Locked */);
}
void CClassView::OnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hParent0, hParent1, hParent2, hParent3;

	hParent0=hParent1=hParent2=hParent3=nullptr;

	*pResult = 0;

	BOOL IndexSelected;
	SimB *pBase=nullptr;P *pPt=nullptr;
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();


	HTREEITEM hItem = m_wndClassView.GetSelectedItem();
	CString m_Name=m_wndClassView.GetItemText(hItem);
	CString m_PtName;
	CString m_SumTyp;
	m_LocalIndex=-1;
	IndexSelected=FALSE;
	bool SelectAllIndex=false;
	int ipos_co;
	ipos_co=m_Name.Find(':');
	if(ipos_co>0&&ipos_co<5) {
		int ipo;
		ipo=m_Name.Find('A');
		if(ipo>0&&ipo<ipos_co) ipos_co=ipo;
		ipo=m_Name.Find('a');
		if(ipo>0&&ipo<ipos_co) ipos_co=ipo;
		CString num=m_Name.Left(ipos_co);
		int row=MFC_Util::AtoInt(num)-1;
		hParent0=m_wndClassView.GetParentItem(hItem);
		hParent1=m_wndClassView.GetParentItem(hParent0);
		CString Indicator=m_wndClassView.GetItemText(hParent1);
		int iper;
		iper=-1;
		if(Indicator=="Coefficient of determination")
			iper=0;
		else if(Indicator=="Intercept")
			iper=1;
		else if(Indicator=="Slope")
			iper=2;
		else if(Indicator=="Mean Error")
			iper=3;
		else if(Indicator=="Root Mean Square Error")
			iper=4;
		else if(Indicator=="Log Likelihood sum")
			iper=5;
		else if(Indicator=="Modelling Efficiency (NSE)")
			iper=6;
		else if(Indicator.Find(_T("Mean of All"))>=0)
			iper=15;
		else if(Indicator.Find(_T("Mean of Accepted"))>=0)
			iper=25;
		p_mainframe->m_wndModel.ShowWindow(SW_SHOW);
		MySingle::instance().GetCugPointer()->UpdateSelectedVal(row, iper);
		return;
	}

	if(m_Name.Find('(')==0) {
		int end=m_Name.ReverseFind(')');
		if(end>0) {
			m_LocalIndex=MFC_Util::AtoInt(m_Name.Mid(1,end-1));
			hParent0=m_wndClassView.GetParentItem(hItem);
			IndexSelected=TRUE;
		}
		m_Name=m_wndClassView.GetItemText(hParent0);
		int st=m_Name.Find('(');
		if(st>0) {
			m_Name=m_Name.Left(st);

		}

	}
	else {
		hParent0=m_wndClassView.GetParentItem(hItem);
	}
	if(hParent0) {
		hParent1=m_wndClassView.GetParentItem(hParent0);
	}
	CString dim;
	dim=m_wndClassView.GetItemText(hParent0);
	int posdim;
	posdim=dim.Find(_T("Dimension"));
	if(posdim==0) {
		m_Name=m_wndClassView.GetItemText(hItem);
		map<string, int>::iterator itm;
		CString idim;
		int l=dim.GetLength();
		idim=dim.Mid(11,l-11);
		int iidim;
		iidim=MFC_Util::AtoInt(idim);
		if(iidim>0) {
			itm=MapMR[iidim-1].find(CT2A(m_Name).m_psz);
			int icount;
			icount=itm->second;
			p_mainframe->m_wndModel.ShowWindow(SW_SHOWNORMAL);
			MySingle::instance().GetCugPointer()->UpdateSelected_MR(iidim, icount);
			MySingle::instance().GetCugPointer()->SelectTab(10);
			return;
		}

	}
	if(hParent1) {
		hParent2=m_wndClassView.GetParentItem(hParent1);
	}
	if(hParent2) {
		hParent3=m_wndClassView.GetParentItem(hParent2);
	}
	if(hParent3) {
		if(IndexSelected) {
			m_type=m_wndClassView.GetItemText(hParent2);
			if(m_type=="State Variables"||m_type=="Flow Variables"||m_type=="Auxiliary Variables"||m_type=="Driving Variables") {
				m_group=m_wndClassView.GetItemText(hParent1);
				m_SumTyp=m_wndClassView.GetItemText(hParent3);
			}
			else if(m_type!="Time series of Output Variables"&&m_type!="Selection of Output Variables") {
				m_type=m_wndClassView.GetItemText(hParent3);
				m_group=m_wndClassView.GetItemText(hParent2);
				int st=m_Name.Find('(');
				if(st>0) {
					m_Name=m_Name.Left(st);
				}
				m_PtName=m_Name;
				m_Name=m_wndClassView.GetItemText(hParent1);
			}
			else 
				m_group=m_wndClassView.GetItemText(hParent1);
		}
		else {
			m_type=m_wndClassView.GetItemText(hParent3);
			if(m_type=="Statistics for Output Variables") {
				m_type=m_wndClassView.GetItemText(hParent1);
				m_SumTyp=m_wndClassView.GetItemText(hParent2);
				m_group=m_wndClassView.GetItemText(hParent0);
			}
			else if(m_type=="Time series of Output Variables"||m_type=="Selection of Output Variables") {
				m_type=m_wndClassView.GetItemText(hParent2);
				m_group=m_wndClassView.GetItemText(hParent1);

			}
		}
	}
	else if(hParent2) {
		m_type=m_wndClassView.GetItemText(hParent2);
		m_group=m_wndClassView.GetItemText(hParent1);
		if(m_type=="Time series of Output Variables"||m_type=="Selection of Output Variables") {
			m_type=m_wndClassView.GetItemText(hParent1);
			m_group=m_wndClassView.GetItemText(hParent0);
		}
		else if(m_type=="Tables") {
			int st=m_Name.Find('(');
			if(st>0) {
				m_Name=m_Name.Left(st);
			}
			m_PtName=m_Name;
			m_Name=m_wndClassView.GetItemText(hParent0);
			st=m_Name.Find('(');
			if(st>0) {
				m_Name=m_Name.Left(st);
			}
		}
	}
	else if(hParent1) {
		m_type=m_wndClassView.GetItemText(hParent1);
		m_group=m_wndClassView.GetItemText(hParent0);
		if(m_type=="Output Variables") {
			m_type=m_wndClassView.GetItemText(hParent0);
			m_group=m_Name;
		}
	}
	else if(hParent0) {
		m_type=m_wndClassView.GetItemText(hParent0);
		m_group=m_Name;
	}
	else {
		m_type=m_Name;
		m_group="ALL";
	}
	if(m_nCurrSort==ID_SORTING_GROUPBYTYPE) {
	

	}
	else if(m_nCurrSort!=ID_SORTING_SORTBYTYPE) {
		// Swap between type and group
		CString str=m_type;
		m_type=m_group;
		m_group=str;

	}

	int iselect_sheet;
	iselect_sheet=5;
	if(m_type=="Switches")
		iselect_sheet=1;
	else if(m_type=="Parameters")
		iselect_sheet=2;
	else if(m_type=="Tables")
		iselect_sheet=3;
	else if(m_type=="Files")
		iselect_sheet=4;
	else if(m_nCurrSort==ID_SORTING_GROUPBYTYPE)
		iselect_sheet=13;





	if(m_pDoc!=nullptr&&!m_OnReDoing&&p_mainframe->IsDocumentValid()) {
		
		
		if(m_Name.Find('(')>0) {
			int end=m_Name.ReverseFind(')');
			int st=m_Name.Find('(');
			if(end>st&&st>0) {
				m_LocalIndex=MFC_Util::AtoInt(m_Name.Mid(st+1,end-1));
				SelectAllIndex=true;
				m_Name=m_Name.Left(st);
			}
		}
		

		pBase=((CCoupModelDoc*)m_pDoc)->GetPtr(m_Name, m_type, m_group);
		if(m_type=="Tables") {
			pPt=(P*)((CCoupModelDoc*)m_pDoc)->GetPtr(m_PtName,_T("Table Parameters"),m_group);
		}
		// Categories
		int icat=0;
		if(pBase!=nullptr) {
			icat=p_ModelInfo->GetGroupCategory(pBase->GetGroup());
			MySingle::instance().GetCugPointer()->SetViewCategory(icat);
		}
		else if(m_group!="") {
			int icat=p_ModelInfo->GetGroupCategory(CT2A(m_group).m_psz);
			MySingle::instance().GetCugPointer()->SetViewCategory(icat);
		}
		else
			MySingle::instance().GetCugPointer()->SetViewCategory(0);
		// Tables
		if(iselect_sheet==3&&pBase!=nullptr) {
			bool k=((Tab*)pBase)->GetHide();
			((Tab*)pBase)->SetHide(FALSE);
		}
		// Output Variables
		if(((CCoupModelDoc*)m_pDoc)->m_Doc.IsFinishedSimulation()&&iselect_sheet==5) {
			MySingle::instance().GetCugPointer()->SetShowAllOutputs(FALSE);
			if(m_typeprior!=""&&m_groupprior!="")
				((CCoupModelDoc*)m_pDoc)->m_Doc.RemoveOriginalValues(CT2A(m_typeprior).m_psz,CT2A(m_groupprior).m_psz,false, icat);
			else
				((CCoupModelDoc*)m_pDoc)->m_Doc.RemoveOriginalValues(CT2A(m_typeprior).m_psz,CT2A(m_groupprior).m_psz,true, icat);
			
			if(m_type!=""&&m_group!="")
				((CCoupModelDoc*)m_pDoc)->m_Doc.RemoveOriginalValues(CT2A(m_type).m_psz,CT2A(m_group).m_psz,true, icat);


		}



		else { // Not output variables below
			MySingle::instance().GetCugPointer()->SetShowAllOutputs(FALSE);
			if(iselect_sheet==5)
				MySingle::instance().GetCugPointer()->SetShowAllOutputs(TRUE);
			if(m_typeprior!=""&&m_groupprior!="")
				((CCoupModelDoc*)m_pDoc)->m_Doc.RemoveOriginalValues(CT2A(m_typeprior).m_psz,CT2A(m_groupprior).m_psz,true);
			if(m_type!=""&&m_group!="")
				((CCoupModelDoc*)m_pDoc)->m_Doc.RemoveOriginalValues(CT2A(m_type).m_psz,CT2A(m_group).m_psz,true);
			
			
			
		}
		CWnd *pWnd;

		auto test=p_mainframe->m_wndModel.ShowWindow(SW_SHOWNORMAL);
		test=p_mainframe->m_wndModel.m_grid1.ShowWindow(SW_SHOWNORMAL);
		pWnd=p_mainframe->m_wndModel.m_grid1.SetFocus();
		MySingle::instance().GetCugPointer()->UpdateSelected(pBase, CT2A(m_type).m_psz, CT2A(m_group).m_psz,  m_LocalIndex, pPt, CT2A(m_SumTyp).m_psz, SelectAllIndex);
		MySingle::instance().GetCugPointer()->SelectTab(iselect_sheet);
		m_typeprior=m_type;
		m_groupprior=m_group;

	}


}
BOOL CClassView::Make_Branch(HTREEITEM hRoot,vector<SimB*> v, size_t ityp, BOOL IsTable, BOOL IsOutput, BOOL IsFinnished, bool IsMulti) {

	SimB *pSimB;
	CString m_index, m_Col;

	CString namn;
	CString		grp,typ, status;
	BOOL isgroup, isvector;

	BOOL NewType;
	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = nullptr;
	tvInsert.hInsertAfter = TVI_SORT;
	tvInsert.item.mask = TVIF_TEXT;
	CString item1, item2, item3, lastItem;
	HTREEITEM hCountry, hPA, hPB, hTab;

	pSimB=(SimB*)v[0];
	size_t i=0;	
	isgroup=FALSE;
	isvector=FALSE;
	hCountry=FALSE;

	while(i<v.size())	{
					pSimB=(SimB*)v[i];

					OutVector *pVector;
		//			OutSingle *pSingle;
					pVector=dynamic_cast<OutVector*>(v[i]);


					if(IsOutput&&IsFinnished) {
						BOOL set=FALSE;
					/*	if(ityp>=6)
							set=((G*)pSimB)->IsFlagsSet(IsMulti);
						else */
					/*	if(pVector!=nullptr) 						
							set=pVector->IsFlagsSet(IsMulti);
						else {
							pSingle=dynamic_cast<OutSingle*>(v[i]);
							set=pSingle->IsFlagSet();
						}
						if(set)
							isgroup=TRUE;*/
						isgroup=TRUE;
					}
					else if(pSimB->IsEnabled())
							isgroup=TRUE;
					i++;
	}
	if(isgroup) {
			i=0;
			NewType=true;
			while(i<v.size())	{
					pSimB=(SimB*)v[i];
					OutVector *pVector;
					OutSingle *pSingle;
					pVector=dynamic_cast<OutVector*>(v[i]);
					pSingle=dynamic_cast<OutSingle*>(v[i]);

					BOOL set=TRUE;
					if(IsOutput&&IsFinnished&&!IsMulti)
						set=TRUE;
					else if(IsOutput&&IsFinnished) {
						set=TRUE;
						if(pVector!=nullptr)
							set=pVector->IsFlagsSet(IsMulti);
						else {
							pSingle=dynamic_cast<OutSingle*>(v[i]);
							set=pSingle->IsFlagSet();
						}

					};
					if(pSimB->IsEnabled()&&set){
						int numrow;
						if(m_nCurrSort==ID_SORTING_SORTBYTYPE||m_nCurrSort==ID_SORTING_GROUPBYTYPE)
							namn=pSimB->GetGroup().c_str();
						else
							namn=p_ModelInfo->GetVarType(ityp).c_str();

						item2=pSimB->GetName().c_str();
						int icat=p_ModelInfo->GetGroupCategory(pSimB->GetGroup());
						//if(m_nCurrSort!=ID_SORTING_SORTBYTYPE)
							//icat=0;
							//icat=pSimB->GetIntType();
						if(IsOutput) {
							if(IsFinnished) {
								isvector=FALSE;
								if(pVector!=nullptr) 
									numrow=pVector->GetNumberOfSelectedFlags(IsMulti);
								else
									numrow=pSingle->GetNumberOfSelectedFlags(IsMulti);
								if(numrow>1) {
									item2+=MFC_Util::AddIndexString(numrow);
									isvector=TRUE;
								}
								else if(numrow==1) {
									int rows;
									if(pVector!=nullptr) 
										rows=pVector->GetNumberOfSelectedFlags(IsMulti);
									else
										rows=pSingle->GetNumberOfSelectedFlags(IsMulti);
									if(rows>1)
										isvector=TRUE;
								}


							}
							else {
								isvector=FALSE;
								if(pVector!=nullptr) 
									numrow=pVector->GetNumberOfFlags();
								else
									numrow=1;
								if(numrow>1) {
									isvector=TRUE;
									item2+=MFC_Util::AddIndexString(numrow);
								}
							}
							if(NewType) {
								CString typ=CString(p_ModelInfo->GetVarType(ityp).c_str());
								hCountry = m_wndClassView.InsertItem(typ,ityp+8,0,hRoot);
							}

						}
						if(namn==lastItem&&!NewType) {
							if((IsOutput&&m_nCurrSort==ID_SORTING_SORTBYTYPE)||(IsOutput&&m_nCurrSort==ID_SORTING_GROUPBYTYPE))
								hPB = m_wndClassView.InsertItem(item2, 3, 3,  hPA);
							else
								hPA = m_wndClassView.InsertItem(item2, 3, 3,  hCountry);
						}
						else {
							if(IsOutput) {	
								if(IsFinnished&&set||!IsFinnished) {
									if(m_nCurrSort==ID_SORTING_SORTBYTYPE) {
										hPA = m_wndClassView.InsertItem(namn,icat+3,icat+3,hCountry);
										hPB = m_wndClassView.InsertItem(item2, 3, 3,  hPA);
									
									}
									else if(m_nCurrSort==ID_SORTING_GROUPBYTYPE) {
										hPA = m_wndClassView.InsertItem(namn,icat+3,icat+3,hCountry);
										hPB = m_wndClassView.InsertItem(item2, 3, 3,  hPA);
									
									}
									else {
										hPA = m_wndClassView.InsertItem(item2,3,3,hCountry);
									}
									
								}
							}
							else {
								if(m_nCurrSort==ID_SORTING_SORTBYTYPE||(m_nCurrSort==ID_SORTING_GROUPBYTYPE)) {
									if(ityp==8)
										hCountry = m_wndClassView.InsertItem(namn,2,2,hRoot);
									else
										hCountry = m_wndClassView.InsertItem(namn,icat+3,icat+3,hRoot);
									hPA = m_wndClassView.InsertItem(item2, 3, 3,  hCountry);
								}
								else {
									if(ityp==8)
										hCountry = m_wndClassView.InsertItem(namn,2,2,hRoot);
									else
										hCountry = m_wndClassView.InsertItem(namn,icat+3,icat+3,hRoot);
									hPA = m_wndClassView.InsertItem(item2,3,3,hCountry);

								}
							}



							lastItem=namn;
							NewType=FALSE;
						}
						if(IsTable) {
 							int numrow=((Tab*)pSimB)->GetNumOfRow();// Number of rows in table

							for(size_t nCol=0; nCol<((Tab*)pSimB)->GetNumOfCol();nCol++) {
								CString Item3=CString(((Tab*)pSimB)->GetTitle(nCol).c_str());
								if(numrow>1) Item3+=MFC_Util::AddIndexString(numrow);
								//m_wndClassView.InsertItem(Item3,2,2, hPA);
								hTab = m_wndClassView.InsertItem(Item3, 1, 1,  hPA);
								if(numrow>1) {
									CString IndexVal;
									for (int ll=0; ll< numrow;ll++) {
										IndexVal=MFC_Util::AddIndexString(ll+1);
										m_wndClassView.InsertItem(IndexVal,3,3, hTab);
									}
								}
							}	
						}
						else if(IsOutput&&isvector) {
								CString Item3;
								int row=1;
								for(int i=1;i<=numrow;i++) {
									if(IsFinnished) {
										if(pVector!=nullptr) 
											row=pVector->GetSelectedLocalIndex(i-1,IsMulti)+1;
										else if(ityp>3&&ityp<8) 
											row=1;
										Item3=MFC_Util::AddIndexString(row);
									}
									else
										Item3= MFC_Util::AddIndexString(i); 
									if(IsOutput&&m_nCurrSort==ID_SORTING_SORTBYTYPE||(m_nCurrSort==ID_SORTING_GROUPBYTYPE))
										m_wndClassView.InsertItem(Item3, 3,3, hPB );
									else
										m_wndClassView.InsertItem(Item3, 3,3, hPA );
							}					
						}
					}
					i++;
			}
	}
	if(hCountry)
		return TRUE;
	else
		return FALSE;

}

void CClassView::OnSortingSortbygroup()
{
	// TODO: Add your command handler code here
}
