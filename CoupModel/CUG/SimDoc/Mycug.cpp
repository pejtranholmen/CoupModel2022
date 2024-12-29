/***************************************************
****************************************************
Skeleton Class for a Derived MyCug
****************************************************
****************************************************/

/*#include "stdafx.h"
//#include "uglibsel.h"
#include "resource.h"

#include "MyCug.h"
*/
#include "..\..\stdafx.h"
#include "..\..\std.h"
#include "..\..\Util.h"

#include "..\..\CoupModel.h"

//#include "afxole.h"
#include "..\..\ModelTypes\SimB.h"
//#include "..\External.h"
#include <time.h>
#include "math.h"

#include "..\..\resource.h"
//#include "..\Chart\RealTimeDemoDlg.h"
#include "..\..\WindowNames.h"

#include "MyCug.h"
#include "..\ColourPickerDlg.h"

#include "..\..\Run\ChaParDuringRun.h"
#include "..\..\Validation\ValidationVariables.h"
#include "..\..\Validation\SelectWindowForValidationSummaryVariables.h"
#include "..\..\MultiRun\MultiRunPar.h"
#include "..\..\MultiRun\MultiRun.h"
#include "..\..\MultiRun\MultiRunDB.h"
#include "..\..\MultiRun\MultiRunTable.h"

#include "..\..\SP\SP_Retrieve.h"
#include "..\..\PG\CreatePG.h"


#include "..\..\CoupModelView.h"
#include "..\..\MainFrm.h"

#include "..\..\NewBase\HistoryList.h"
#include "..\..\PG\PG_FILEDLG.h"
#include "..\..\NewBase\FileScanner.h"

#include "..\..\PG\NewPGVariables.h"
//#include "..\NewBase\FortranMap.h"
#include "..\..\NewBase\codecogs/stats/Moments/covariance.h"
#include "..\..\NewBase\codecogs/stats/Moments/correlation.h"
#include "..\..\NewBase\codecogs/stats/Moments/variance.h"

#include "..\..\NewBase\codecogs/stats/Moments/mean.h"
#include "..\..\SoilData\PlotPF\PFCurve.h"
#include "..\..\CUG\PFCug\PFCug.h"
#include "htmlhelp.h"

extern int ViewStatus;
extern bool ReadyToSimulate;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(MyCug,CUGCtrl)
	//{{AFX_MSG_MAP(MyCug)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/***************************************************
****************************************************/



MyCug::MyCug()
{

	m_bDrawExcelFocus = FALSE;
	m_DocumentDefined=FALSE;
	m_fontsList = nullptr;
	//UGXPThemes::UseThemes(FALSE);

	MySingle::instance().SetCugPointer(this);

	string m_StartInDir, Fil;
	m_StartInDir=MFC_Util::GetProfileStringStd( "StartDirectory", m_StartInDir);
	//Fil=m_StartInDir+"\\BITMAP8.BMP";		
//	m_BMP1=AddBitmap(IDB_BITMAP2);
	m_Prior="";
	m_After="";
	m_extralargeFont.CreateFont(40,0,0,0,600,0,0,0,0,0,0,0,0,_T("Arial"));
	
	//m_font.CreateFont(16,0,0,0,500,0,0,0,0,0,0,0,0,_T("Arial"));
	GetDefaultFont(m_font);
	GetDefaultLargeFont(m_largeFont);
	m_sort[1]=TRUE; m_sort[2]=TRUE; m_sort[3]=TRUE; m_sort[4]=TRUE; m_sort[0]=TRUE;
	m_ShowAllOutputs=FALSE;
	m_ShowAllSwitches=FALSE;
	m_ShowAllParameters=FALSE;
	m_ShowAllTables=FALSE;
	m_ShowAllFiles=TRUE;
	m_ColorEdit=RGB(40,240,240);
	m_ColorHistory=RGB(100,200,240);
	m_ColorChanged=RGB(255,200,200);

	m_ColorFirstIndep=RGB(255,150,150); // Indicate Second Independent
	m_ColorSecondIndep=RGB(255,100,100); // Indicate Second Independent
	m_ColorHeading=RGB(230,230,230);
	m_ColorClick=RGB(240,240,50);
	m_NoColor=RGB(255,255,255);
	m_ColorRClick=RGB(230,230,230);
	m_ColorCompleted=RGB(80,210,80);

	m_created=FALSE;
	m_created_pathname="";

	m_CurrentChart=1;
	m_newChart=FALSE;
	m_PreviousChart=-1;

	m_ShowMultiRes=FALSE;
	m_ShowValidRes=FALSE;
	m_ShowSumRes=FALSE;
	m_ShowCoVarRes=FALSE;
	m_ShowAllEnsemblePostParList = FALSE;
	m_ShowTimeSerieTable=FALSE;
	m_ShowMultiSerieTable=FALSE;
	m_ShowHistoryList=TRUE;
	m_Display=TRUE;
	m_ShowValidRes_Changed=FALSE;
	for(int i=0;i<MAXSIMVAL;i++) {
		m_OutAllMulti_Defined[i]=FALSE;
		m_OutSelectedMulti_Defined[i]=FALSE;
	}
	m_OutMAPMulti_Defined=FALSE;
	m_NeedParUpdate=FALSE;
	m_ShowMultiTimeSerieTable=FALSE;
	SetMultiSelectMode ( TRUE );
	DragDropTarget ( TRUE );
	m_ShowChartTableNo=-1;
	m_ShowChartTable=FALSE;
	m_LinkedChartDocument=FALSE;
	m_NumOpenDoc=1;
	m_storage_exist=TRUE;
	m_CurrentTopRow=0;
	m_ViewCategory=0;
	m_ParameterTableView=0;
	m_OutPutFile_Exist=TRUE;
	m_TobePlotted=FALSE;
	m_ShowValidCoVarRes=FALSE;
	m_ShowValidationTimeSerieTable=FALSE;
	m_CurrentSubChart=1;
	m_pProp->m_NumChartX=MFC_Util::GetProfileIntNo( "NumberChartX",1);
	m_pProp->m_NumChartY=MFC_Util::GetProfileIntNo( "NumberChartY",1);
	if(m_pProp->m_NumChartX<1|| m_pProp->m_NumChartX > 100) m_pProp->m_NumChartX=1;
	if(m_pProp->m_NumChartY<1|| m_pProp->m_NumChartY > 100) m_pProp->m_NumChartY=1;
	m_pProp->m_NumSubCharts=m_pProp->m_NumChartX*m_pProp->m_NumChartY;

	m_pSel=nullptr;
	m_SelGroup="";
	m_SelType="All";
	m_SelLocalIndex=-1;
	m_SelectAllIndex=false;
	m_UpdateOutputList=true;
	m_VarNo=0;
	m_pSimDoc=nullptr;
	m_pView=nullptr;
	m_pDoc=nullptr;
	m_pFrame=nullptr;
	m_PrevMainFrame=nullptr;
}

void MyCug::NeedParUpdate() {
	m_NeedParUpdate=TRUE;
}
void MyCug::ReDefineDefaultFont()
{
	m_largeFont.DeleteObject();
	m_font.DeleteObject();
	GetDefaultFont(m_font);
	GetDefaultLargeFont(m_largeFont);
}
void MyCug::SetDocument(CCoupModelDoc *pDoc)
{
	m_pView=pDoc->m_Doc.GetModelView();
	m_pDoc=pDoc;
	m_pSimDoc=&pDoc->m_Doc;
	m_DocumentDefined=TRUE;

	m_pFrame=MySingle::instance().GetFramePointer();
	m_pFrame->m_wndPF.m_PFgrid.m_pDoc=m_pSimDoc;
	m_pFrame->m_wndSoilData.m_SoilDataGrid.m_pDoc=m_pSimDoc;
	m_pFrame->m_wndDB.m_DBBasgrid.m_pDoc=m_pSimDoc;
	m_pFrame->m_wndDBData.m_DBDataGrid.m_pDoc=m_pSimDoc;
	m_pFrame->m_wndEvalMulti.m_ValidationCug.m_pSimDoc=m_pSimDoc;
	m_pFrame->m_wndEvalMulti.m_ValidationCug.SetpCUG(this);
	m_pSimDoc->m_PlotStorage.Init(m_pSimDoc);
	if (m_pSimDoc->m_ValSum_Array.size() > 0&&m_pSimDoc->IsFinishedSimulation()) {
	//	m_pSimDoc->m_MStorage.SetMinMaxOfPerformanceIndicators();
	//	m_pSimDoc->m_MStorage.SetAllToAcceptedRuns(); // Check if you want to save previous settings
	}
	m_pFrame->m_wndEvalMulti.m_ValidationCug.OnSetup();
	m_pFrame->m_wndDBData.m_DBDataGrid.OnUpdateDataBase();
	m_pFrame->m_wndDBData.m_DBDataGrid.OnSetup();


	MySingle::instance().GetDB_HeaderPointer()->m_pSimDoc=m_pSimDoc;
	CDB *pDB=m_pSimDoc->DB_GetPointer(Soil_Properties);
	p_PFCurve = &PlotpF::GetPointer();
	if(pDB!=nullptr) {
		string soilkey=pDB->GetStringValue();
		if(soilkey.size()>0) {
			size_t newkey=p_PFCurve->GetNewKey(soilkey);
			size_t rec=p_PFCurve->GetHeaderRecord(newkey);
			p_PFCurve->SetHeader(rec);
			//m_pFrame->m_wndPF.m_PFgrid.MakeBaseSheet(false, false);
			m_pFrame->m_wndPF.m_PFgrid.OnTabSelected(0);
			m_pFrame->m_wndSoilData.m_SoilDataGrid.MakeSelectedProfileSheet();

		}
	}
	if(m_pSimDoc->GetDocumentType()>0)
	if(m_pSimDoc->IsFinishedSimulation()) 
		MakePlotWindows();



}
MyCug::~MyCug()
{
		// clean up possible fonts allocated by the user
	_tagFonts *pFont, *pTemp;
	pFont = m_fontsList;
	while ( pFont != nullptr )
	{
		pTemp = pFont->next;
		delete pFont;
		pFont = pTemp;
	}
	m_largeFont.DeleteObject();
	m_extralargeFont.DeleteObject();
	m_font.DeleteObject();

/*	if(m_pDoc!=nullptr) {
		while(m_pSimDoc->m_PlotStorage.m_PGFileArray.size()>0) {
			CPG *pPG=(CPG*)m_pSimDoc->m_PlotStorage.m_PGFileArray[0];
			delete pPG;
			m_pSimDoc->m_PlotStorage.m_PGFileArray.erase(m_pSimDoc->m_PlotStorage.m_PGFileArray.begin());
			m_pSimDoc->m_PlotStorage.m_PGFileArrayShow.erase(m_pSimDoc->m_PlotStorage.m_PGFileArrayShow.begin());

		}
	}*/
	//UGXPThemes::CleanUp();
}

// New function by PEJ

void MyCug::ReportUpdate()
{
	/*	SetSheetNumber(7); 
		int isheet=GetSheetNumber();
		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
					DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);*/
	m_pFrame=MySingle::instance().GetFramePointer();
	m_PrevMainFrame=m_pFrame;

		if(m_Display&&m_pSimDoc!=NULL) {
			MySingle::instance().GetChartCugPointer()->Init(m_pSimDoc);
			MySingle::instance().GetChartCugPointer()->RealTimeMode(TRUE);
			MySingle::instance().GetChartCugPointer()->OnSetup();
			MySingle::instance().GetChartCugPointer()->m_RealTime.InitData(m_pSimDoc);
			MySingle::instance().GetChartCugPointer()->m_RealTime.OnInit();
			m_Display=FALSE;
		}




}
void MyCug::CloseReportMode()
{

}
void MyCug::ClosePGFile()
{
	if(m_pDoc!=nullptr) {
		if(((CCoupModelDoc*)m_pDoc)->m_Doc.m_DocumentType>0)
			m_pDoc->OnCloseDocument();
	}
}
void MyCug::ResultUpdate()
{
	if(m_pSimDoc->IsMultiSimulation())
		return;
	// Update only for Single simulation or last Multi simulation
		SetSheetNumber(1); 
		int isheet=GetSheetNumber();
		EnableUpdate(FALSE); 
		m_ShowAllOutputs=FALSE;
		while(GetNumberRows()>0)
					DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);


		SetSheetNumber(2); 
		isheet=GetSheetNumber();
		EnableUpdate(FALSE); 
		m_ShowAllOutputs=FALSE;
		while(GetNumberRows()>0)
					DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);


		SetSheetNumber(3); 
		isheet=GetSheetNumber();
		EnableUpdate(FALSE); 
		m_ShowAllOutputs=FALSE;
		while(GetNumberRows()>0)
					DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);

		SetSheetNumber(4); 
		isheet=GetSheetNumber();
		EnableUpdate(FALSE); 
		m_ShowAllOutputs=FALSE;
		while(GetNumberRows()>0)
					DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);




		SetSheetNumber(5); 
		isheet=GetSheetNumber();
		EnableUpdate(FALSE); 
		m_ShowAllOutputs=FALSE;
		while(GetNumberRows()>0)
					DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);

		SetSheetNumber(6); 
		isheet=GetSheetNumber();
		EnableUpdate(FALSE); 
		m_ShowAllOutputs=FALSE;
		while(GetNumberRows()>0)
					DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);

		SetSheetNumber(7); 
		isheet=GetSheetNumber();
		EnableUpdate(FALSE); 
		m_ShowAllOutputs=FALSE;
		while(GetNumberRows()>0)
					DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);

		SetSheetNumber(8); 
		isheet=GetSheetNumber();
		EnableUpdate(FALSE); 
		m_ShowAllOutputs=FALSE;
		while(GetNumberRows()>0)
					DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);
		
		SetSheetNumber(9); 
		isheet=GetSheetNumber();
		EnableUpdate(FALSE); 
		m_ShowAllOutputs=FALSE;
		while(GetNumberRows()>0)
					DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);
		
		SetSheetNumber(10); 
		isheet=GetSheetNumber();
		EnableUpdate(FALSE); 
		m_ShowAllOutputs=FALSE;
		while(GetNumberRows()>0)
					DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);

		SetSheetNumber(0); 
		isheet=GetSheetNumber();
		EnableUpdate(FALSE); 
		m_ShowAllOutputs=FALSE;
		while(GetNumberRows()>0)
					DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);


		MakePlotWindows();

	
	
}


void MyCug::SortAndJoin(int col, int isheet)
{
	string Group, PrevGroup;
	CUGCell cell; 
	int LastRow, StartRow;

	
	if(isheet==4) return;

/*	if(isheet==3&&col==0)
		((NewMap*)m_pDoc)->Sort_TableList_Arrays(FALSE);
	else if(isheet==3&&col==2)
		m_pSimDoc->Sort_TableList_Arrays(TRUE);
	
	if(isheet==3) {
		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(3);
		return;
	}*/


	if((col<4&&isheet>0)||isheet==12) {
			PrevGroup="a";
			LastRow=0;
			StartRow=0;

			//if(m_sort[isheet])
			//	SortBy(col, UG_SORT_DESCENDING); 
			//else
			int maxrow = GetNumberRows();
			for (int i = 0; i < maxrow; i++) {
				if (isheet != 3) {
					UnJoinCells(-1, i);
					UnJoinCells(0, i);
					UnJoinCells(1, i);
				}
				if (isheet == 5) {
					UnJoinCells(1, i);
					UnJoinCells(2, i);
				}
			}
			//RedrawAll(); 
			SortBy(col, UG_SORT_ASCENDING); 
			//RedrawAll();
			//int maxrow=GetNumberRows();
			//UnJoinCells(0,maxrow-1);
			//RedrawAll(); 
			if(col<3&&isheet==3) {
// This is for the table sheet joins
				int colv1=0;
				int colv2=0;

				for (int i=0;i<maxrow;i++) {
					GetCell(3, i, &cell);
					JoinCells(colv1,i,colv2,i);
					GetCell(colv1, i, &cell);	
					Group=cell.GetStdText();
					if(PrevGroup==Group) {
						LastRow=i;
					}
					else {
						PrevGroup=Group;
						if(LastRow+1>StartRow&&LastRow>0) {
							JoinCells(colv1,StartRow,colv2,LastRow);
						}
						StartRow=i;
					}
				}
				if(LastRow+1>StartRow) {
					JoinCells(colv1,StartRow,colv2,LastRow);
				}

				colv1=1;
				LastRow=0;
				StartRow=0;
				colv2=1;

				for (int i=0;i<maxrow;i++) {
					JoinCells(colv1,i,colv2,i);
					GetCell(colv1, i, &cell);	
					Group=cell.GetStdText();
					if(PrevGroup==Group) {
						LastRow=i;
					}
					else {
						PrevGroup=Group;
						if(LastRow+1>StartRow&&LastRow>0) {
							JoinCells(colv1,StartRow,colv2,LastRow);
						}
						StartRow=i;
					}
				}
				if(LastRow+1>StartRow) {
					JoinCells(colv1,StartRow,colv2,LastRow);
				}




				
				colv1=2;
				LastRow=0;
				StartRow=0;
				colv2=4;

				for (int i=0;i<maxrow;i++) {
					JoinCells(colv1,i,colv2,i);
					GetCell(colv1, i, &cell);	
					Group=cell.GetStdText();
					if(PrevGroup==Group) {
						LastRow=i;
					}
					else {
						PrevGroup=Group;
						if(LastRow+1>StartRow&&LastRow>0) {
							JoinCells(colv1,StartRow,colv2,LastRow);
						}
						StartRow=i;
					}
				}
				if(LastRow+1>StartRow) {
					JoinCells(colv1,StartRow,colv2,LastRow);
				}
				JoinCells(-1,0,-1,maxrow-1);
				RedrawAll();
			}
			else if(col<3&&isheet!=5&&isheet!=12) {
				int colv=0;
				LastRow=0;
				StartRow=0;
				string txt;
				m_ClickedRow=-1;
				for (int i=0;i<maxrow;i++) {
					txt=QuickGetStdText(2,i);
					int ipos=txt.find('(');
					if(ipos>0)
						txt=txt.substr(0,ipos);
					if(txt==m_SelName&&m_ClickedRow<0) {
						m_ClickedRow=i;
						QuickSetBackColor(2,i,m_ColorClick);
					}
					GetCell(colv, i, &cell);	
					Group=cell.GetStdText();
					SetCell(colv,i,&cell);
					if(PrevGroup==Group) {
						LastRow=i;
					}
					else {
						PrevGroup=Group;
						if(LastRow+1>StartRow&&LastRow>0) {
							JoinCells(colv,StartRow,colv,LastRow);
						}
						StartRow=i;
					}
				}
				if(LastRow+1>StartRow) {
					JoinCells(colv,StartRow,colv,LastRow);
				}

				int colv1=1;
				LastRow=0;
				StartRow=0;
				int colv2=1;

				for (int i=0;i<maxrow;i++) {
					JoinCells(colv1,i,colv2,i);
					GetCell(colv1, i, &cell);	
					Group=cell.GetStdText();
					if(PrevGroup==Group) {
						LastRow=i;
					}
					else {
						PrevGroup=Group;
						if(LastRow+1>StartRow&&LastRow>0) {
							JoinCells(colv1,StartRow,colv2,LastRow);
						}
						StartRow=i;
					}
				}
				if(LastRow+1>StartRow) {
					JoinCells(colv1,StartRow,colv2,LastRow);
				}

				if(isheet==2) {
					int colv1=GetNumberCols()-1;
					LastRow=0;
					StartRow=0;
					colv2=colv1;

					for (int i=0;i<maxrow;i++) {
						JoinCells(colv1,i,colv2,i);
						GetCell(colv1, i, &cell);	
						Group = cell.GetStdLabelText();
							//cell.GetText();
						if(PrevGroup==Group) {
							LastRow=i;
						}
						else {
							PrevGroup=Group;
							if(LastRow+1>StartRow&&LastRow>0) {
								JoinCells(colv1,StartRow,colv2,LastRow);
							}
							StartRow=i;
						}
					}
					if(LastRow+1>StartRow) {
						JoinCells(colv1,StartRow,colv2,LastRow);
					}
				}



				JoinCells(-1,0,-1,maxrow-1);

				
				RedrawAll(); 
			}
			else if(isheet==5) {

				int colv=0;
				LastRow=0;
				StartRow=0;
				BOOL validplot=m_pSimDoc->IsOutputBinFileValid();
				for (int i=0;i<maxrow;i++) {
					GetCell(colv, i, &cell);	
					Group=cell.GetStdText();
					SetCell(colv,i,&cell);
					if(PrevGroup==Group) {
						LastRow=i;
					}
					else {
						PrevGroup=Group;
						if(LastRow+1>StartRow&&LastRow>0) {
							JoinCells(colv,StartRow,colv,LastRow);
						}
						StartRow=i;
					}
				}
				if(LastRow+1>StartRow) {
					JoinCells(colv,StartRow,colv,LastRow);
				}
				colv = 1;
				LastRow = 0;
				StartRow = 0;
				for (int i = 0; i<maxrow; i++) {
					GetCell(colv, i, &cell);
					Group = cell.GetStdText();
					SetCell(colv, i, &cell);
					if (PrevGroup == Group) {
						LastRow = i;
					}
					else {
						PrevGroup = Group;
						if (LastRow + 1>StartRow&&LastRow>0) {
							JoinCells(colv, StartRow, colv, LastRow);
						}
						StartRow = i;
					}
				}
				if (LastRow + 1>StartRow) {
					JoinCells(colv, StartRow, colv, LastRow);
				}



				colv=2;
				LastRow=0;
				StartRow=0;
				string txt, Type, Group;
				m_ClickedRow=-1;
				SimB *pSw;
				CUGCell cell1, cell2;

				for (int i=0;i<maxrow;i++) {

					GetCell(colv, i, &cell);
					Group=cell.GetStdText();
					if(m_SelName!=""&&m_ClickedRow<0) {
						if(m_pSimDoc->IsFinishedSimulation()) {
							GetCell(3,i,&cell2);
							txt=cell2.GetStdLabelText();
						}
						else
							txt=QuickGetStdText(2,i);
						int ipos=txt.find('(');
						if(ipos>0) {
							int ipose=txt.rfind(')');
							int lindex=FUtil::AtoInt(txt.substr(ipos+1,ipose-1));
							
							txt=txt.substr(0,ipos);
							 if(txt==m_SelName&&m_SelLocalIndex>=0) {
									if(m_SelLocalIndex==lindex||m_SelectAllIndex) {
										QuickSetBackColor(3,i,m_ColorClick);
										m_ClickedRow=i;
										GetCell(1,i, &cell1);
										Group=cell1.GetStdText();
										GetCell(0,i, &cell1);
										Type=cell1.GetStdText();;
										pSw=m_pSimDoc->GetPtr(Type, Group, m_SelName);
										if(pSw!=nullptr) {
											SelectOUT out;

											auto selected=pSw->GetStoreFlag(lindex-1);	

											if((validplot||selected)&&m_pSimDoc->m_SumIndicator==-1&&m_pSimDoc->IsFinishedSimulation()) {
													if(m_pSimDoc->m_SelectOut.size()==0)
														m_CurrentSubChart=1;
													else if(m_CurrentSubChart<m_pProp->m_NumSubCharts&&m_pSimDoc->m_SelectOut[m_pSimDoc->m_SelectOut.size()-1].pBase!=pSw)
														m_CurrentSubChart++;
													if(m_pSimDoc->m_NewChart) {
														out.pBase=pSw;
														out.LIndex=lindex;
														out.row=i;
														out.IType=p_ModelInfo->GetTypeIndex(Type)-3;
														out.MeanIndicator=ENSEMBLE_TYPE::NO;
														out.subchart=m_CurrentSubChart;
														m_pSimDoc->m_SelectOut.push_back(out);
													}
													QuickSetNumber(5,i,m_CurrentSubChart);

													if((m_SelLocalIndex==lindex&&m_SelectAllIndex)||!m_SelectAllIndex)
														TimeSerieChartClick(2, i, 5, pSw, m_SelLocalIndex, 0, Type);
													m_pSimDoc->m_NewChart=false;
													GetCell(3,i,&cell1);
													cell1.SetBool(TRUE);
													SetCell(3,i,&cell1);
													RedrawCell(3,i);
													if(lindex<m_SelLocalIndex) {
														m_pSimDoc->m_NewChart=true;
														m_ClickedRow=-1;
													}
											}
											else if (m_pSimDoc->m_SumIndicator>=0&&m_pSimDoc->m_NewChart) {
												col=m_pSimDoc->m_SumIndicator+6;
												GetCell(col,i,&cell1);
												cell1.SetBool(TRUE);
												SetCell(col,i ,&cell1);	
												RedrawCell(col,i);
												MultiPlotClick(5,col, i, TRUE);
												m_pSimDoc->m_NewChart=false;
												//MultiSumChart
											}
										}

									}
							 }
							 else if (txt==m_SelName)  {
									QuickSetBackColor(3,i,m_ColorClick);
									m_ClickedRow=i;
									GetCell(1,i, &cell1);
									Group=cell1.GetStdText();
									GetCell(0,i, &cell1);
									Type=cell1.GetStdText();;
									pSw=m_pSimDoc->GetPtr( Type, Group, m_SelName);
									if(pSw!=nullptr) {
										int selected;
										SelectOUT out;
										selected=pSw->GetStoreFlag(lindex-1);
											if((validplot||selected==2)&&m_pSimDoc->m_SumIndicator==-1) {
													if(m_pSimDoc->m_SelectOut.size()==0)
														m_CurrentSubChart=1;
													else if(m_CurrentSubChart<m_pProp->m_NumSubCharts)
														m_CurrentSubChart++;
													if(m_pSimDoc->m_NewChart) {
														out.pBase=pSw;
														out.LIndex=1;
														out.row=i;
														out.IType=p_ModelInfo->GetTypeIndex(Type)-3;
														out.MeanIndicator= ENSEMBLE_TYPE::NO;
														out.subchart=m_CurrentSubChart;
														m_pSimDoc->m_SelectOut.push_back(out);
													}
													TimeSerieChartClick(2, i, 5, pSw, 1, 0, Type);
													m_pSimDoc->m_NewChart=false;
													GetCell(3,i,&cell1);
													cell1.SetBool(TRUE);
													SetCell(3,i,&cell1);
													RedrawCell(3,i);
												}
											else if(m_pSimDoc->m_SumIndicator>=0&&m_pSimDoc->m_NewChart) {
													col=m_pSimDoc->m_SumIndicator+6;
													GetCell(col,i,&cell1);
													cell1.SetBool(TRUE);
													SetCell(col,i ,&cell1);	
													RedrawCell(col,i);
													MultiPlotClick(5,col, i, TRUE);
													m_pSimDoc->m_NewChart=false;
											}
									}
							 }

						}
						else if(txt==m_SelName) {
							QuickSetBackColor(3,i,m_ColorClick);
							m_ClickedRow=i;
							GetCell(1,i, &cell1);
							Group=cell1.GetStdText();
							GetCell(0,i, &cell1);
							Type=cell1.GetStdText();;
							pSw=m_pSimDoc->GetPtr( Type, Group, m_SelName);
							if(pSw!=nullptr) {
									int selected;
									SelectOUT out;
									selected=pSw->GetStoreFlag(0);

									if(m_pSimDoc->m_SumIndicator==-1&&(validplot||selected==2)&&m_pSimDoc->IsFinishedSimulation()) {
											if(m_pSimDoc->m_SelectOut.size()==0)
												m_CurrentSubChart=1;
											else if(m_CurrentSubChart<m_pProp->m_NumSubCharts)
												m_CurrentSubChart++;
											if(m_pSimDoc->m_NewChart) {
												out.pBase=pSw;
												out.LIndex=1;
												out.row=i;
												out.IType=p_ModelInfo->GetTypeIndex(Type)-3;
												out.MeanIndicator=ENSEMBLE_TYPE::NO;
												out.subchart=m_CurrentSubChart;
												m_pSimDoc->m_SelectOut.push_back(out);	
											}
											QuickSetNumber(5,i,m_CurrentSubChart);
											TimeSerieChartClick(2, i, 5, pSw, 1, 0, Type);
											m_pSimDoc->m_NewChart=false;
											GetCell(3,i,&cell1);
											cell1.SetBool(TRUE);
											SetCell(3,i,&cell1);
											RedrawCell(3,i);
									}
									else if(m_pSimDoc->m_SumIndicator>=0&&m_pSimDoc->m_NewChart) {
											int col1=m_pSimDoc->m_SumIndicator+6;
											GetCell(col1,i,&cell1);
											cell1.SetBool(TRUE);
											SetCell(col1,i ,&cell1);	
											RedrawCell(col1,i);
											MultiPlotClick(5,col1, i, TRUE);
											m_pSimDoc->m_NewChart=false;

									
									}
							}


						}
					}
					else if(m_ClickedRow<0)  {
						txt=QuickGetStdText(0,i);
						if(Group==m_SelGroup&&m_ClickedRow<0&&m_SelType==txt) {
							m_ClickedRow=i;
							GetCell(1,i, &cell1);
							Group=cell1.GetStdText();
							GetCell(0,i, &cell1);
							Type=cell1.GetStdText();;
							pSw=m_pSimDoc->GetPtr(Type, Group, m_SelName);
							if(pSw!=nullptr) {
								SelectOUT out;
								if(m_pSimDoc->m_SelectOut.size()==0)
									m_CurrentSubChart=1;
								else if(m_CurrentSubChart<m_pProp->m_NumSubCharts)
									m_CurrentSubChart++;
								if(m_pSimDoc->m_NewChart) {
									out.pBase=pSw;
									out.LIndex=-1;
									out.row=i;
									out.IType=p_ModelInfo->GetTypeIndex(Type)-3;
									out.MeanIndicator= ENSEMBLE_TYPE::NO;
									out.subchart=m_CurrentSubChart;
									m_pSimDoc->m_SelectOut.push_back(out);
								}
								QuickSetNumber(5,i,m_CurrentSubChart);
								TimeSerieChartClick(2, i, 5, pSw, -1, 0, Type);
								m_pSimDoc->m_NewChart=false;
								GetCell(2,i,&cell1);
								cell1.SetBool(TRUE);
								SetCell(2,i,&cell1);
								RedrawCell(2,i);
							}


						}
					}


					SetCell(colv,i,&cell);
					if(PrevGroup==Group) {
						LastRow=i;
					}
					else {
						PrevGroup=Group;
						if(LastRow+1>StartRow&&LastRow>0) {
							JoinCells(colv,StartRow,colv,LastRow);
						}
						StartRow=i;
					}
				}
				if(LastRow+1>StartRow) {
					JoinCells(colv,StartRow,colv,LastRow);
				}
				JoinCells(-1,0,-1,maxrow-1);
				RedrawAll(); 
			}



		RedrawAll();
		}





}

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid

BOOL MyCug::isCreated()
{
	if(m_created)
		return TRUE;
	return FALSE;

}
void MyCug::OnReset()
{
	CUGCell cell; 
	CString heading, number, test;


	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2;

	CString str;
	int MaxCol=8;

	EnableMenu(TRUE); 
	EnableJoins(TRUE); 
	EnableUpdate(TRUE);
	m_created=TRUE;
	UseHints(TRUE);

	    SetDefFont(&m_font);


		SetDefColWidth(130);
	
		EnableUpdate(FALSE);	
		SetNumberSheets(0);
		SetTabWidth(0); 
		
		int num_sheets=GetNumberSheets();
		for (long i=0;i<=num_sheets;i++) {
			DeleteTab(i);
		}
		
		QuickSetText(0,-1,"No document loaded");
		JoinCells(0,-1,3,-1);
		//SetSheetNumber(6); 
		for(int i=0; i<num_sheets;i++) {
			//SetSheetNumber(i);
			//*DeleteRow(-1);
			while(GetNumberRows()>0)
				DeleteRow(0);
			
		}
		SetNumberSheets(0); 
		RedrawAll();
		m_pDoc=nullptr;
		m_pSimDoc=nullptr;



		EnableUpdate(TRUE); 


		int sheet=0;

	
}
void MyCug::OnSetup()
{
	CUGCell cell; 
	CString heading, number, test;


	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2;

	CString str;
	int MaxCol=8;

	EnableMenu(TRUE); 
	EnableJoins(TRUE); 
	EnableUpdate(TRUE);
	m_created=TRUE;
	UseHints(TRUE);

	    SetDefFont(&m_font);

		int num_sheets=GetNumberSheets();
		for (long i=0;i<num_sheets;i++) {
			DeleteTab(i);
		}
		SetDefColWidth(130);
		if(m_pDoc==nullptr) return;
		if (m_pSimDoc->m_DocFile.m_SimulationRunNo==string::npos)  return;	
		if(m_pDoc->m_Doc.m_DocumentType>0) return;
		if(m_pFrame==nullptr) m_pFrame=MySingle::instance().GetFramePointer();
	//	if(!m_pDoc->m_Ready) return;
		if(m_pDoc->m_Closing) return;

		//******* Add New Tabs 
		AddTab("Run Info", 0); 
		AddTab("Switches", 1); 
		AddTab("Parameters", 2); 
		AddTab("Parameter Tables", 3); 
		AddTab("Model Files", 4); 
		AddTab("Output Variables", 5); 
		AddTab("Validation", 6); 
		AddTab("Messages", 7);
		AddTab("Charts", 8);
		AddTab("Dynamic Parameters", 9);
		AddTab("MultiRun", 10);
		AddTab("History", 11);
		AddTab("Annimation Variables", 12);
		AddTab("Attached Files", 13);
		SetTabWidth(1000); 
	

	

		//SetSheetNumber(6); 
		num_sheets=GetNumberSheets();
		if(num_sheets!=14)
			SetNumberSheets(14); 
		else
			OnSheetSetup(1);
		int sheet=m_pSimDoc->GetCurrentSheet();
		if(m_pSimDoc->m_PrevSheet!=0) sheet=m_pSimDoc->m_PrevSheet;
		OnSheetSetup(sheet);
		SetCurrentTab(sheet);
					//int cx,cy;
		m_SetUpDone=true;
		if(m_PrevMainFrame!=nullptr) {

		//	MySingle::instance().GetChartCugPointer()->MakeModelPlot();
		//	MySingle::instance().GetChartCugPointer()->OnSetup();
	

			//m_pSimDoc->RemoveOriginalValues("Outputs","ALL",false);
			//CleanSelectedBySheet(4);
			//CleanSelectedBySheet(5);
			//CleanSelectedBySheet(6);
			
		//	MySingle::instance().GetChartCugPointer()->MakeModelPlot();

		}
		else {
			m_pFrame=MySingle::instance().GetFramePointer();
		//
			m_PrevMainFrame=m_pFrame;
			Make_SwitchChart();
			//CleanSelectedBySheet(4);
			//CleanSelectedBySheet(5);
			//CleanSelectedBySheet(6);
		}
		P* pP;
		pP = m_pSimDoc->GetPPointer("Thickness Layers");
		auto nl=pP->GetSize();
		auto nfreq = m_pUtil->GetProfileViewFrequence();
		m_SelectedSoilLayers.resize(nl);
		for (size_t i = 0; i < nl; i++) {
			if (i%nfreq == 0) m_SelectedSoilLayers[i] = true;
			else m_SelectedSoilLayers[i] = false;
		}
		pP = m_pSimDoc->GetPPointer("I Plant Age");
		auto np = pP->GetSize();
		nfreq = 2;
		m_SelectedPlants.resize(np);
		for (size_t i = 0; i < np; i++) {
			if (i%nfreq == 0) m_SelectedPlants[i] = true;
			else m_SelectedPlants[i] = false;
		}
	
}

/////////////////////////////////////////////////////////////////////////////
//	OnSheetSetup	
//		This notification is called for each additional sheet that the grid
//		might contain, here you can customize each sheet in the grid.
//	Params:
//		sheetNumber - index of current sheet
//	Return:
//		<none>



void MyCug::OnSheetSetup(int sheetNumber)
{
	UNREFERENCED_PARAMETER(sheetNumber);
	CUGCell cell; 
	CString heading, number, test;

	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	CString str, Defstring;
	CString optionlist;
	int MaxCol=8;
	int colv=0;

	BOOL Tableexpand;

	Tableexpand=FALSE;
	EnableMenu(TRUE); 

	if (m_pDoc == nullptr || m_pSimDoc->m_DocFile.m_SimulationRunNo == string::npos) return;
		UpdateStatusBar(sheetNumber);
		if(m_pDoc->GetDocumentType()>0&&sheetNumber!=11) {
			int num_sheets=GetNumberSheets();
			for (long i=0;i<num_sheets;i++) {
				DeleteTab(i);
			}	
			
		}
		else 
			switch(sheetNumber) {
			case(0): //Run Options
				SetLocalWindowText("SIM - Run Info", WINDOW_SIM);
				Make_RunInfoSheet();
				RedrawAll();
				break;
			case (1): //Switches
				SetLocalWindowText("SIM - Switches", WINDOW_SIM);
				Make_SwitchesSheet();
				JoinOutputListRows(2);
				break;
			case(2): // Parameters
				SetLocalWindowText("SIM - Parameters", WINDOW_SIM);
				Make_ParameterSheet();
				JoinOutputListRows(2);
				//SortAndJoin(0,2);
				break;
			case(3): // Parameters Tables
				SetLocalWindowText("SIM - Parameters Tables", WINDOW_SIM);
				if(!Make_ParameterTableSheet())
					JoinOutputListRows(2);
					RedrawAll();
				break;
			case(4): // Model Files
				SetLocalWindowText("SIM - Model Files", WINDOW_SIM);
				Make_ModelFileSheet();
				RedrawAll();
				TimeSerieChart(4);
				break;
			case(5): // Outputs
				SetLocalWindowText("SIM - Model Outputs", WINDOW_SIM);
				Make_OutputSheet();
				UpdatePlotStatusBar(sheetNumber);
				if(!m_ShowTimeSerieTable&&!m_ShowMultiSerieTable&&m_pSimDoc->IsFinishedSimulation())
					//SortAndJoin(0,5);
					if(m_pSimDoc->m_SelectOut.size()>0) {
						for(size_t i=0; i<m_pSimDoc->m_SelectOut.size();i++) {
							long row=m_pSimDoc->m_SelectOut[i].row;
							SimB *pBase;
							string grp, type;
							string Name;
							GetCell(0,row, &cell);
							type=cell.GetStdText();
							GetCell(1,row, &cell);
							grp=cell.GetStdText();
							GetCell(2,row, &cell);
							Name=cell.GetStdLabelText();

							int pos1=Name.rfind('(');
							int pos2=Name.rfind(')');
							int Index;
							if (pos2>pos1&&pos2==Name.size()-1) {
									string t=Name.substr(pos1+1,pos2-pos1-1);
									if(t!="no roots") {
										Index=FUtil::AtoInt(t);
										Name=Name.substr(0,pos1);
									}
									else
										Index=1;
				
							}
							else
								Index=1;


							pBase=(SimB*)m_pSimDoc->m_SelectOut[i].pBase;
							int in;
							
							in=m_pSimDoc->m_SelectOut[i].LIndex;
							string nam=pBase->GetName();
							string nam2=Name;
							if(nam==nam2) {

								int v=3;
							}	
							string tx;
							tx=pBase->GetGroup();
							if(tx==grp) {
								int v=3;
							}
							tx=p_ModelInfo->GetVarType(m_pSimDoc->m_SelectOut[i].IType+3);						;
							if(tx==type) {
								int v=3;
							}


							if(grp==pBase->GetGroup()&&type==p_ModelInfo->GetVarType(m_pSimDoc->m_SelectOut[i].IType+3)&&Index==m_pSimDoc->m_SelectOut[i].LIndex) {
								GetCell(2,row,&cell);
								cell.SetBool(TRUE);
								SetCell(2,row,&cell);
								RedrawCell(2,row);
								int chart=m_pSimDoc->m_SelectOut[i].subchart;
								QuickSetNumber(4,row,chart);
								RedrawCell(4,row);
							}
						}
						TimeSerieChart(5);
					}
					else if(m_pSimDoc->m_SelectMultiSum.size()>0) {
						MultiPlotClick(5,0, 0, FALSE);
					}
				break;
			case(6): // Validation
				UpdatePlotStatusBar(sheetNumber);
				SetLocalWindowText("SIM - Validation", WINDOW_SIM);
				Make_ValidationSheet();
				if(m_pSimDoc->m_SelectValTime.size()>0&&m_pSimDoc->IsFinishedSimulation()) {
					for(size_t i=0; i<m_pSimDoc->m_SelectValTime.size();i++) {
						long row=m_pSimDoc->m_SelectValTime[i].row;
						int col=m_pSimDoc->m_SelectValTime[i].col;
						GetCell(col,row,&cell);
						cell.SetBool(TRUE);
						SetCell(col,row,&cell);
						RedrawCell(col,row);
						int chart=m_pSimDoc->m_SelectValTime[i].subchart;
						QuickSetNumber(5,row,chart);
						RedrawCell(5,row);
						TimeSerieChart(6);
					}
				}
				else if(m_pSimDoc->m_SelectMultiVal.size()>0) {
					MultiPlotClick(6,0, 0, FALSE);

				}
				break;
			case 7: // Simulation report
				SetLocalWindowText("SIM - Messages", WINDOW_SIM);
				Make_RunReportSheet();
				break;
			case 8: // Simulation report
				SetLocalWindowText("SIM - Charts", WINDOW_SIM);
				Make_ChartListSheet();
				break;
			case 9: // Simulation report
				SetLocalWindowText("SIM - Dynamic Par", WINDOW_SIM);
				Make_ChaParListSheet();
				break;
			case 10: // Simulation report
				SetLocalWindowText("SIM - MultiRun", WINDOW_SIM);
				Make_MultiRunListSheet();
				if(m_pSimDoc->m_SelectMultiPar.size()>0) {
					MultiPlotClick(10,0, 0, FALSE);
				}
				UpdatePlotStatusBar(sheetNumber);
				break;
			case 11: 
				SetLocalWindowText("SIM - History", WINDOW_SIM);
				Make_HistorySheet();
				break;
			case 12: 
				SetLocalWindowText("SIM - Annimation", WINDOW_SIM);
				Make_AnnimationSheet();
				break;
			case 13:
				SetLocalWindowText("SIM - Attached Runs", WINDOW_SIM);
				Make_AttachedFileSheet();
				break;
	
		}
}


/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		is sent when a cell change action was instigated
//		( user clicked on another cell, used keyboard arrows,
//		or Goto[...] function was called ).
//	Params:
//		oldcol, oldrow - 
//		newcol, newrow - cell that is gaining focus
//	Return:
//		TRUE - to allow the move
//		FALSE - to prevent new cell from gaining focus
int MyCug::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newcol);
	UNREFERENCED_PARAMETER(newrow);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanViewMove
//		is called when current grid's view is about to be scrolled.
//	Params:
//		oldcol, oldrow - coordinates of original top-left cell
//		newcol, newrow - coordinates of top-left cell that is gaining the focus
//	Return:
//		TRUE - to allow for the scroll
//		FALSE - to prevent the view from scrolling
int MyCug::OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newcol);
	UNREFERENCED_PARAMETER(newrow);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHitBottom
//		This notification allows for dynamic row loading, it will be called
//		when the grid's drawing function has hit the last row.  It allows the grid
//		to ask the datasource/developer if there are additional rows to be displayed.
//	Params:
//		numrows		- known number of rows in the grid
//		rowspast	- number of extra rows that the grid is looking for in the datasource
//		rowsfound	- number of rows actually found, usually equal to rowspast or zero.
//	Return:
//		<none>
void MyCug::OnHitBottom(long numrows,long rowspast,long rowsfound)
{
	UNREFERENCED_PARAMETER(numrows);
	UNREFERENCED_PARAMETER(rowspast);
	UNREFERENCED_PARAMETER(rowsfound);
//****** Set the Number of Rows 
//SetNumberRows(numrows+rowsfound, FALSE); 




	// used by the datasources
	if ( rowsfound > 0 )
	{
		SetNumberRows( numrows + rowsfound, FALSE );
	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnHitTop
//		Is called when the user has scrolled all the way to the top of the grid.
//	Params:
//		numrows		- known number of rows in the grid
//		rowspast	- number of extra rows that the grid is looking for in the datasource
//	Return:
//		<none>
void MyCug::OnHitTop(long numrows,long rowspast)
{
	UNREFERENCED_PARAMETER(numrows);
	UNREFERENCED_PARAMETER(rowspast);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeCol
//		is sent when the mouse was moved in the area between
//		columns on the top heading, indicating that the user
//		might want to resize a column.
//	Params:
//		col - identifies column number that will be sized
//	Return:
//		TRUE - to allow sizing
//		FALSE - to prevent sizing
int MyCug::OnCanSizeCol(int col)
{
	UNREFERENCED_PARAMETER(col);
//****** Disallow column 3 from being resized
//if (col >8){ 

//MFC_Util::MessageBox("illegal Procedure!!- Column 3 cannot be resized", "Grid Error", MB_ICONSTOP);
//	return FALSE; 

//} 

return TRUE; 

}

/////////////////////////////////////////////////////////////////////////////
//	OnColSizing
//		is sent continuously when user is sizing a column.
//		This notification is ideal to provide min/max width checks.
//	Params:
//		col - column currently sizing
//		width - current new column width
//	Return:
//		<none>
void MyCug::OnColSizing(int col,int *width)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(*width);
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSized
//		This is sent when the user finished sizing the 
//		given column.(see above for more information)
//	Params:
//		col - column sized
//		width - new column width
//	Return:
//		<none>
void MyCug::OnColSized(int col,int *width)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(*width);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeRow
//		is sent when the mouse was moved in the area between
//		rows on the side heading, indicating that the user
//		might want to resize a row.
//	Params:
//		row - identifies row number that will be sized
//	Return:
//		TRUE - to allow sizing
//		FALSE - to prevent sizing
int MyCug::OnCanSizeRow(long row)
{
	UNREFERENCED_PARAMETER(row);
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSizing
//		Sent during user sizing of a row, can provide
//		feed back on current height
//	Params:
//		row - row sizing
//		height - pointer to current new row height
//	Return:
//		<none>
void MyCug::OnRowSizing(long row,int *height)
{
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*height);
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSized
//		This is sent when the user is finished sizing the
//		given row.
//	Params:
//		row - row sized
//		height - pointer to current new row height
//	Return:
//		<none>
void MyCug::OnRowSized(long row,int *height)
{
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*height);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeSideHdg
//		Sent when the user is about to start sizing of the side heading
//	Params:
//		<none>
//	Return:
//		TRUE - to allow sizing
//		FALSE - to prevent sizing
int MyCug::OnCanSizeSideHdg()
{
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSizing
//		Sent while the heading is being sized
//	Params:
//		width - pointer to current new width of the side heading
//	Return:
//		TRUE - to accept current new size
//		FALSE - to stop sizing, the size is either too large or too small
int MyCug::OnSideHdgSizing(int *width)
{
	UNREFERENCED_PARAMETER(*width);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSized
//		Sent when the user has completed the sizing of the side heading
//	Params:
//		width - pointer to new width
//	Return:
//		TRUE - to accept new size
//		FALSE - to revert to old size
int MyCug::OnSideHdgSized(int *width)
{
	UNREFERENCED_PARAMETER(*width);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeTopHdg
//		Sent when the user is about to start sizing of the top heading
//	Params:
//		<none>
//	Return:
//		TRUE - to allow sizing
//		FALSE - to prevent sizing
int MyCug::OnCanSizeTopHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSizing
//		Sent while the top heading is being sized
//	Params:
//		height - pointer to current new height of the top heading
//	Return:
//		TRUE - to accept current new size
//		FALSE - to stop sizing, the size is either too large or too small
int MyCug::OnTopHdgSizing(int *height)
{
	UNREFERENCED_PARAMETER(*height);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSized
//		Sent when the user has completed the sizing of the top heading
//	Params:
//		height - pointer to new height
//	Return:
//		TRUE - to accept new size
//		FALSE - to revert to old size
int MyCug::OnTopHdgSized(int *height)
{
	UNREFERENCED_PARAMETER(*height);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColChange
//		Sent whenever the current column changes
//	Params:
//		oldcol - column that is loosing the focus
//		newcol - column that the user move into
//	Return:
//		<none>
void MyCug::OnColChange(int oldcol,int newcol)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(newcol);
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowChange
//		Sent whenever the current row changes
//	Params:
//		oldrow - row that is loosing the locus
//		newrow - row that user moved into
//	Return:
//		<none>
void MyCug::OnRowChange(long oldrow,long newrow)
{
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newrow);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellChange
//		Sent whenever the current cell changes
//	Params:
//		oldcol, oldrow - coordinates of cell that is loosing the focus
//		newcol, newrow - coordinates of cell that is gaining the focus
//	Return:
//		<none>
void MyCug::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(newcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newrow);
}

/////////////////////////////////////////////////////////////////////////////
//	OnLeftColChange
//		Sent whenever the left visible column in the grid changes, indicating
//		that the view was scrolled horizontally
//	Params:
//		oldcol - column that used to be on the left
//		newcol - new column that is going to be the first visible column from the left
//	Return:
//		<none>
void MyCug::OnLeftColChange(int oldcol,int newcol)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(newcol);
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopRowChange
//		Sent whenever the top visible row in the grid changes, indicating
//		that the view was scrolled vertically
//	Params:
//		oldrow - row that used to be on the top
//		newrow - new row that is going to be the first visible row from the top
//	Return:
//		<none>
void MyCug::OnTopRowChange(long oldrow,long newrow)
{
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newrow);
}

/////////////////////////////////////////////////////////////////////////////
//	OnViewMoved
//		This notification is fired after the current viewing area
//		was scrolled.
//	Params:
//		nScrolDir - UG_VSCROLL, UG_HSCROLL
//
//		if the nScrolDir == UG_VSCROLL
//			oldPos - row that used to be on the top
//			newPos - row that is now the first visible row from the top
//
//		if the nScrolDir == UG_VSCROLL
//			oldPos - column that used to be on the left
//			newPos - column that is now the first visible column from the left
//	Return:
//		<none>
void MyCug::OnViewMoved( int nScrolDir, long oldPos, long newPos )
{
	UNREFERENCED_PARAMETER(nScrolDir);
	UNREFERENCED_PARAMETER(oldPos);
	UNREFERENCED_PARAMETER(newPos);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSelectionChanged
//		The OnSelectionChanged notification is called by the multiselect
//		class when a change occurred in current selection (i.e. user clicks
//		on a new cell, drags a mouse selecting range of cells, or uses
//		CTRL/SHIFT - left click key combination to select cells.)
//	Params:
//		startCol, startRow	- coordinates of the cell starting the selection block
//		endCol, endRow		- coordinates of the cell ending the selection block
//		blockNum			- block number representing this range, this will
//							always represent total number of selection blocks.
//	Return:
//		<none>
void MyCug::OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum)
{
	UNREFERENCED_PARAMETER(startCol);
	UNREFERENCED_PARAMETER(startRow);
	UNREFERENCED_PARAMETER(endCol);
	UNREFERENCED_PARAMETER(endRow);
	UNREFERENCED_PARAMETER(blockNum);


}

/////////////////////////////////////////////////////////////////////////////
//	OnLClicked
//		Sent whenever the user clicks the left mouse button within the grid
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{



	CString str;
	CUGCell cell, cell1, cell2; 
	string Group, Name, Type;
	CCoupModelDoc *pDoc;
	int usheet;
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);

//******* Amend the cell using 'Get..Set'
	int isheet=GetSheetNumber();
    pDoc=m_pDoc;
	//Tables
/*	if(m_pSimDoc->p_ModelInfo->IsRunning()&&isheet!=12) {
		MFC_Util::MessageBox(" You can not modify document when modell is running !");
		return;

	}*/

	if (isheet==0&&!updn) {
		return;
	}
	else if(isheet==3&&updn) { 
		usheet=ParameterTableClick(col, row); 

		if(usheet>=0) OnSheetSetup(usheet);


	}
	else if(isheet==4&&updn) { // Change model Files
		usheet=ModelFileClick(col, row); 
		if(usheet>=0) OnSheetSetup(usheet);

	}			
	else if(isheet==1&&updn&&col==0) { 
		usheet=SwitchClick(col, row);
		if(usheet>=0) OnSheetSetup(usheet);
		return;
	} // Change Switches

	else if(isheet==2&&updn&&col>=0) { 
			usheet=ParameterClick(col, row);
			if(usheet>=0) OnSheetSetup(usheet);
	}
	else if(isheet==5&&m_ShowTimeSerieTable) {
		return;
	}
	else if((isheet==5&&!updn&&col==1)||(isheet==5&&col>=3&&col<=5&&!updn)||(isheet==5&&col>6&&updn)) { // Change Outputs
		if (m_pSimDoc->IsFinishedSimulation())
			usheet = OutPutVarClick(col, row);
		else
			usheet = OutPutVarSelectionClicktoStore(col, row);
		if(usheet>=0) {
			Make_OutputSheet();
			SetTopRow(m_CurrentTopRow);
		}
		
		return;
	}
	else if((isheet==12&&col>3&&col<6&&updn)) {  //Output Variables
		

	}
	
 // Output Variables

	else if(isheet==6&&m_ShowValidationTimeSerieTable)
		return;
	else if(isheet==6&&updn&&col>0&&col<=5&&!m_ShowValidRes&&!m_ShowValidCoVarRes) { 		// Validation Variables
		// to make sure that correct time serie variable is picked
		usheet=ValidationVarClick(col, row);
		if(usheet>=0) OnSheetSetup(usheet);



	}
	else if(isheet==6&&updn&&col>6&&!m_ShowValidRes&&!m_ShowValidCoVarRes) {  // Validation Variables
			usheet=ValidationVarClick(col, row);
			if(usheet>=0) OnSheetSetup(usheet);
	}
	else if(isheet==6&&updn&&m_pSimDoc->IsMultiSimulation()&&m_ShowValidRes&&!m_ShowValidCoVarRes) {
		//Click on Selection for plotting from MultiRun Validation
		int nvar=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
		if((col>=2&&row>5||col>=2&&row==0)&&col<nvar+2) {
			GetCell(0,row,&cell);
			int mrunno;
			SelectVAL val;
			val.row=col-2;
			//val.subchart=0;
			val.RunNoRow=row;
			val.subchart=m_pSimDoc->m_SelectValTime.size()+1;
			if(row==6) {
				mrunno=120000;
				val.col=1;
				val.MeanIndicator=ENSEMBLE_TYPE::NO;
				
			}
			else if(row==0) {
				val.MeanIndicator= ENSEMBLE_TYPE::ACCEPTED;
				val.col=4;
			}
			else  {
	   			val.MeanIndicator= ENSEMBLE_TYPE::NO;
				val.RunNo=int(cell.GetNumber())-1;		
				val.col=3;
			}
			
			GetCell(col,row,&cell);
			if(cell.GetBool()) {
				if(!m_TobePlotted) {
					QuickSetText(2,-1, "Click To Plot");
					QuickSetBackColor(2,-1,m_ColorChanged);
					RedrawCell(2,-1);
				}

				m_pSimDoc->m_SelectValTime.push_back(val);

				UpdatePlotStatusBar(1);
			}
			else {
				SelectVAL valold;
				for(size_t i=0; i<m_pSimDoc->m_SelectValTime.size(); i++) {
					valold=m_pSimDoc->m_SelectValTime[i];
					if(valold.RunNoRow==val.RunNoRow&&valold.col==val.col&&valold.row==val.row) {
						m_pSimDoc->m_SelectValTime.erase(m_pSimDoc->m_SelectValTime.begin()+i);
						return;
					}
				}
			}
		}

		else if(col==1&&row>5) {
			int ValFileNo=1;

			if(m_pSimDoc->m_SelectValTime.size()>0)
				TimeSerieChartClick(col, 0, isheet, (SimB*)nullptr, 0, 0, "", ValFileNo);	

		}

	}

	else if(isheet==10&&updn&&col==2&&updn) {  // MultiRun Parameters
		usheet=MultiRunClick(col, row);
		if(usheet>=0) OnSheetSetup(usheet);
	}
	else if(isheet==10&&updn&&m_ShowCoVarRes) {
		usheet=MultiRunClick(col, row);
		if(usheet>=0) OnSheetSetup(usheet);
	}
	else if (isheet == 10 && updn && col == 7) {
		usheet = MultiRunClick(col, row);
		if (usheet >= 0) OnSheetSetup(usheet);



	}
	else if(isheet==12&&updn) {
		// Change toggle Outputs on/off
		usheet=AnnimateClick(col, row);
		if(usheet>=0) OnSheetSetup(usheet);

		

	

	}
}
		
/////////////////////////////////////////////////////////////////////////////
//	OnRClicked
//		Sent whenever the user clicks the right mouse button within the grid
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
//		Sent whenever the user double clicks the left mouse button within the grid
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);

	SimB *pSw;
	string str;
	CUGCell cell, cell1, cell2; 
	string Group, Name, Type;
	CCoupModelDoc *pDoc;
	long ParV;

	int isheet=GetSheetNumber();
    pDoc=m_pDoc;
	if(isheet==5&&col==2) {  //Output Variables
		m_VarNo=0;
		
		CCoupModelDoc *pDocPointer;

		pDocPointer=m_pDoc;

		if(m_newChart)
			m_newChart=FALSE;
		else
			m_newChart=TRUE;

		if(m_CurrentChart<=pDocPointer->m_Doc.m_PlotStorage.GetCurrentChart()) {
			m_CurrentChart++;
		}
	}
	else if(isheet==4||isheet==11) { // Change model Files
		GetCell(col,row, &cell); 
		ParV=cell.GetParam();

		//cell.SetBackColor(RGB(0,255,255));
		//cell.SetText(message);
	//	SetCell(col,row,&cell); 

		if(ParV==1000) {
			GetCell(1,row, &cell1);
			Name=cell1.GetStdText();
			GetCell(0,row, &cell1);
			Group=cell1.GetStdText();
			Type="Files";
			
			pSw=m_pSimDoc->GetPtr(Type, Group, Name);
			if(pSw!=nullptr) { //Make table Visible
				if( ((F*)pSw)->GetHide()  ) 
					((F*)pSw)->SetHide(false);
				else
					((F*)pSw)->SetHide(true);
				EnableUpdate(FALSE); 
				while(GetNumberRows()>0)
						DeleteRow(0);
				EnableUpdate(TRUE); 
				OnSheetSetup(isheet);
			}
			else {
				int index=ParV-1000;
				GetCell(-2,row,&cell);
				int Index=int(cell.GetNumber());
				if(m_pSimDoc->m_PlotStorage.m_PGFileArrayShow[Index]==0){
					m_pSimDoc->m_PlotStorage.m_PGFileArrayShow[Index]=1;
				}
				else {
					m_pSimDoc->m_PlotStorage.m_PGFileArrayShow[Index]=0;
				}
				EnableUpdate(FALSE); 
				while(GetNumberRows()>0)
						DeleteRow(0);
				EnableUpdate(TRUE); 
				OnSheetSetup(isheet);



			}
		}
		else if(ParV==2000) {

			int Index=0;
			
			GetCell(-1,row,&cell);
			Index=int(cell.GetNumber());		

			if(isheet==11) {
				GetCell(-1,row-Index-1, &cell1);
				int index=int(cell1.GetNumber()-1);
				CPG *pPG=(CPG*)m_pSimDoc->m_PlotStorage.m_PGFileArray[index];

//				TimeSerieChartClick(0, row, isheet, pSw, Index, 0, Type);
					TimeSerieChartClick(0, row, isheet, (SimB*)pPG, Index, 0, Type);

			}
			else {
				GetCell(1,row-Index-1, &cell1);
				Name=cell1.GetStdText();
				GetCell(0,row-Index-1, &cell1);
				Group=cell1.GetStdText();
				Type="Files";
				
				pSw=m_pSimDoc->GetPtr(Type, Group, Name);
				if(pSw!=nullptr) { 
					TimeSerieChartClick(0, row, isheet, pSw, Index, 0, Type);
				}
			}

		}
	}			
	else {
		StartEdit();

	}
}	
			 
/////////////////////////////////////////////////////////////////////////////
//	OnMouseMove
//		is sent when the user moves mouse over the grid area.
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		point		- represents the screen point where the mouse event was detected
//		nFlags		- 
//		processed	- indicates if current event was processed by other control in the grid.
//	Return:
//		<none>
void MyCug::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user clicks the left mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
	string pzPathName, Num, newPathName;
	CUGCell cell;

	int isheet=GetSheetNumber();


	if (isheet == 0 && updn) {
		if (col == 0 && !m_pSimDoc->IsFinishedSimulation() && !m_pSimDoc->IsRunning()) {
			MakeSingleRun();
			Make_RunInfoSheet();

		}
		else if ((col == 1 || col == 2) && !m_pSimDoc->IsFinishedSimulation() && !m_pSimDoc->IsRunning()) {
			MakeMultiRuns(col);
			OnSheetSetup(0);

		}
		else if (col == 2 && (m_pSimDoc->IsFinishedSimulation() || m_pSimDoc->IsRunning())) {	// Create new document
			if (m_pSimDoc->m_ChildDocument)
				CreateNewDocFromCurrentDoc(true, false);
			else
				CreateNewDocFromCurrentDoc(false, false);
		}
		else if (col == 3 && (m_pSimDoc->IsFinishedSimulation() || m_pSimDoc->IsRunning()))	{// Create new document
			if (m_pSimDoc->m_ChildDocument)
				CreateNewDocFromCurrentDoc(false, false);
			else
				CreateNewDocFromCurrentDoc(true, false);
		}
		else if(col==1) 
			return;
	}
	else if (isheet == 6 && (col == 4||col==3)&& row== -1&&updn) {
		//Shift to a new ensemble
		if (col == 4) {
			m_pSimDoc->m_MStorage.m_ActualEnsemble++;
			if (m_pSimDoc->m_MStorage.m_ActualEnsemble == m_pSimDoc->m_MStorage.m_AcceptedIdString.size()) m_pSimDoc->m_MStorage.m_ActualEnsemble = 0;
			m_pSimDoc->m_MStorage.SetActual_AcceptedEnsemble();
			if (m_pSimDoc->m_MStorage.Check_Change_PerformanceIndicator_ToActual_Ensemble()) {
				m_pSimDoc->m_MStorage.CompleteProcessingOfInputFiles_MultiStorage();
			}
		}
		else if (col == 3) {
			auto value=m_pUtil->GetResidualRepresentation();
			value++;
			if (value == 2) value = 0;
			m_pUtil->SetResidualRepresentation(value);
		}

		
		OnSheetSetup(6);
		return;
	}
	else if(isheet==6&&m_ShowValidationTimeSerieTable) {
		SelectRange ( col, -4, col, GetNumberRows()-1 );
	}
	else if(isheet==6&&updn&&m_ShowValidRes&&!m_ShowValidCoVarRes&&!m_ShowValidationTimeSerieTable||isheet==10&&updn&&m_ShowMultiRes) {
	// no Action for Validation sheet and Multi parameter sheet
		if(col==0&&isheet==6&&m_ShowValidRes) {
			CWinApp* pApp = AfxGetApp();
			pApp->BeginWaitCursor();
			m_pSimDoc->m_MStorage.UpdateAcceptedRuns(FALSE);
			EnableUpdate(FALSE); 
			while(GetNumberRows()>0)
					DeleteRow(0);
			EnableUpdate(TRUE);
			m_ShowValidRes_Changed=FALSE;
			OnSheetSetup(6);
		
			pApp->EndWaitCursor();
			return;


		}
		else if(col==2&&isheet==6&&m_ShowValidRes) {
			int ValFileNo=1;
			if(m_pSimDoc->m_SelectValTime.size()>0)
				TimeSerieChartClick(col, 0, isheet, (SimB*)nullptr, 0, 0, "", ValFileNo);	

			return;
		}
		else if(col>0)
			SelectRange ( col, -1, col, GetNumberRows()-1 );


	}
	else if(isheet==5&&updn&&m_ShowTimeSerieTable||isheet==5&&updn&&m_ShowMultiSerieTable) {
	// no Action for Validation sheet and Multi parameter sheet
		SelectRange ( col, -4, col, GetNumberRows()-1 );

	}

	else if(isheet==6&&col==4&&updn&&!m_ShowValidCoVarRes&&!m_ShowValidationTimeSerieTable) {
		if(m_LinkedChartDocument){
			m_LinkedChartDocument=FALSE;
			SetSheetNumber(6); 
			GetCell(4,-1,&cell);
			cell.SetBackColor(m_ColorClick);
			SetCell(4,-1,&cell);
			RedrawCell(4,-1);
			m_NumOpenDoc=1;
			m_pSimDoc->m_SelectedDocuments.clear();
			//m_SelectedDoc.RemoveAll();
		}
		else if(UpdateDocumentLink()) {
			m_LinkedChartDocument=TRUE;
			SetSheetNumber(6); 
			GetCell(4,-1,&cell);
			cell.SetBackColor(m_ColorChanged);
			SetCell(4,-1,&cell);
			RedrawCell(4,-1);
			m_NumOpenDoc=m_pSimDoc->m_SelectedDocuments.size();
			
		}
	}
	else if(isheet>=1&&isheet<7&&!m_ShowValidCoVarRes&&!m_ShowValidationTimeSerieTable) {
		if (updn) {
			if (isheet == 4) return;
			if ((col == 1 && isheet != 5) || (col == 2 && isheet == 5)) {
				//if (m_ShowProcesses) m_ShowProcesses = FALSE;
				m_ShowProcesses = TRUE;
			}
			int selectedSort = col;
			if (isheet == 1 || isheet == 2 || isheet == 3) selectedSort = col + 1;
			for (size_t i = 0; i < 2; i++)
				SelectedSort[i] = SelectedSort[i + 1];
			SelectedSort[2] = enum sortby(selectedSort);
			MFC_Util::WriteProfileInt("SortOutput1", int(SelectedSort[0]));
			MFC_Util::WriteProfileInt("SortOutput2", int(SelectedSort[1]));
			MFC_Util::WriteProfileInt("SortOutput3", int(SelectedSort[2]));

			OnSheetSetup(isheet);
		}
	}
	else if(isheet==12) {
		if(updn)
			SortAndJoin(col, isheet);

	}

}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_RClicked
//		Sent whenever the user clicks the right mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top heading
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);

	SortBy(col, UG_SORT_DESCENDING); 
		
	}
/////////////////////////////////////////////////////////////////////////////
//	OnSH_LClicked
//		Sent whenever the user clicks the left mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
	if (updn) return;
	auto sheet = GetSheetNumber();
	if (sheet == 6||sheet==5||sheet==10) {
		if (col == -2)
			m_pSimDoc->m_MStorage.ChangeEnsembleNo(string::npos);
		else
			m_pSimDoc->m_MStorage.ChangeEnsembleNo(1);
		OnSheetSetup(sheet);
	}

}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_RClicked
//		Sent whenever the user clicks the right mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_DClicked
//		Sent whenever the user double clicks the left mouse button within the side heading
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_LClicked
//		Sent whenever the user clicks the left mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);

	BOOL hide=FALSE;
	

	int isheet=GetSheetNumber();
    
	if(!updn) return;
	if(isheet==1) { // Switches

		if(m_ShowAllSwitches)
			m_ShowAllSwitches=FALSE;
		else
			m_ShowAllSwitches=TRUE;

		if(m_ShowAllSwitches==FALSE)
			m_pSimDoc->RemoveOriginalValues("Switches", "ALL", false);
		else
			m_pSimDoc->RemoveOriginalValues("Switches","ALL", true);

		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
					DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);

	}
	else if(isheet==2) { // Parameters

		if(m_ShowAllParameters)
			m_ShowAllParameters=FALSE;
		else
			m_ShowAllParameters=TRUE;

		if(m_ShowAllParameters==FALSE)
			m_pSimDoc->RemoveOriginalValues("Parameters","ALL",false);
		else
			m_pSimDoc->RemoveOriginalValues("Parameters","ALL",true);
		
		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
					DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);
	}
	else if(isheet==4) {

		if(m_ShowAllFiles)
			m_ShowAllFiles=FALSE;
		else
			m_ShowAllFiles=TRUE;

		if(m_ShowAllFiles==FALSE)
			m_pSimDoc->RemoveOriginalValues("Files","ALL", false);
		else
			m_pSimDoc->RemoveOriginalValues("Files","ALL", true);

		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
					DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);
	}
	else if(isheet==3) { // Expansion of Tables

		if(m_ShowAllTables==FALSE) {
			m_pSimDoc->RemoveOriginalValues("Tables","ALL", false);
			m_ShowAllTables=TRUE;
		}
		else {
			m_pSimDoc->RemoveOriginalValues("Tables","ALL", true);
			m_ShowAllTables=FALSE;
		}

		int NumLines=m_pSimDoc->m_Tab_Array.size();
/*		for (int i=0; i<NumLines; ++i)	{
				Tab *pTab=m_pSimDoc->m_Tab_Array[i];
				if(i==0) m_HideAllTableContents=pTab->GetHide();
				if(m_HideAllTableContents) 
					pTab->SetHide(FALSE);
				else
					pTab->SetHide(TRUE);
				
		}*/

			
		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
					DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);
	}
	else if(isheet==5) { // Expansion of Variable list to include exclude Non Selected Variables
		if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiSimulation()){

			if(m_ShowMultiTimeSerieTable) {
				if(m_ShowTimeSerieTable)
					m_ShowTimeSerieTable=FALSE;
				else
					m_ShowTimeSerieTable=TRUE;	
			}
			else {
				if(m_ShowMultiSerieTable)
					m_ShowMultiSerieTable=FALSE;
				else
					m_ShowMultiSerieTable=TRUE;	
				if(m_ShowTimeSerieTable)
					m_ShowTimeSerieTable=FALSE;
				else
					m_ShowTimeSerieTable=TRUE;	
			}

		}
		else if(m_pSimDoc->IsFinishedSimulation()){
			if(m_ShowTimeSerieTable)
				m_ShowTimeSerieTable=FALSE;
			else
				m_ShowTimeSerieTable=TRUE;			
		}

		else {
			if(m_ShowAllOutputs)
				m_ShowAllOutputs=FALSE;
			else
				m_ShowAllOutputs=TRUE;

			if(m_ShowAllOutputs==FALSE)
				m_pSimDoc->RemoveOriginalValues("Outputs","ALL",false, m_ViewCategory);
			else
				m_pSimDoc->RemoveOriginalValues("Outputs","ALL",true, m_ViewCategory);




		}

		EnableUpdate(FALSE); 
		m_CurrentTopRow=GetTopRow();
		while(GetNumberRows()>0)
				DeleteRow(0);


		OnSheetSetup(isheet);
		SetTopRow(m_CurrentTopRow);

	}
	else if(isheet==6) { //Validation Data

		if(!m_pSimDoc->IsFinishedSimulation()) {
				CValidationVariables dlg(m_pDoc,1, this);
				if(dlg.DoModal()==IDOK) {
					EnableUpdate(FALSE); 
					while(GetNumberRows()>0)
							DeleteRow(0);
					EnableUpdate(TRUE); 
					OnSheetSetup(6);
					EnableUpdate(FALSE); 
					while(GetNumberRows()>0)
						DeleteRow(0);
					EnableUpdate(TRUE); 
					OnSheetSetup(0);
					OnSheetSetup(6);
					SetCurrentTab(6);
				}

			
			return;
		}
		if(m_pSimDoc->IsMultiSimulation()) {
			if(m_ShowValidCoVarRes) {
				m_ShowValidCoVarRes=FALSE;
				m_ShowValidRes=FALSE;
			}
			else if(m_ShowValidRes)
				m_ShowValidRes=FALSE;
			else
				m_ShowValidRes=TRUE;	
		}
		UpdateSheet(6);


	}
	else if(isheet==8) {
		if(m_ShowChartTable)
			m_ShowChartTable=FALSE;
		else
			m_ShowChartTable=TRUE;
		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
					DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);
	}
	else if(isheet==10) { //Multi Run Data

		if(!m_pSimDoc->IsFinishedSimulation()) return;

		if(m_ShowMultiRes)
			m_ShowMultiRes=FALSE;
		else
			m_ShowMultiRes=TRUE;

		BeginWaitCursor();		
		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		Make_MultiRunListSheet();
		EndWaitCursor();

	}



	else if(isheet==11) {
		if(m_ShowHistoryList)
			m_ShowHistoryList=FALSE;
		else {
			m_ShowHistoryList = TRUE;
			m_pSimDoc->m_ViewAll = true;

		}

		BeginWaitCursor();		
		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		Make_HistorySheet();
		EndWaitCursor();


	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_RClicked
//		Sent whenever the user clicks the right mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top corner button
//	Params:
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnCB_DClicked(RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyDown
//		Sent when grid received a WM_KEYDOWN message, usually as a result
//		of a user pressing any key on the keyboard.
//	Params:
//		vcKey		- virtual key code of the key user has pressed
//		processed	- indicates if current event was processed by other control in the grid.
//	Return:
//		<none>
void MyCug::OnKeyDown(UINT *vcKey,BOOL processed)
{
	UNREFERENCED_PARAMETER(*vcKey);
	UNREFERENCED_PARAMETER(processed);
//	GetScrollBarCtrl(SB_HORZ)->EnableScrollBar(ESB_DISABLE_BOTH);
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyUp
//		Sent when grid received a WM_KEYUP message, usually as a result
//		of a user releasing a key.
//	Params:
//		vcKey		- virtual key code of the key user has pressed
//		processed	- indicates if current event was processed by other control in the grid.
//	Return:
//		<none>
void MyCug::OnKeyUp(UINT *vcKey,BOOL processed)
{
	UNREFERENCED_PARAMETER(*vcKey);
	UNREFERENCED_PARAMETER(processed);
//	GetScrollBarCtrl(SB_HORZ)->EnableScrollBar(ESB_DISABLE_BOTH);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCharDown
//		Sent when grid received a WM_CHAR message, usually as a result
//		of a user pressing any key that represents a printable character.
//	Params:
//		vcKey		- virtual key code of the key user has pressed
//		processed	- indicates if current event was processed by other control in the grid.
//	Return:
//		<none>
void MyCug::OnCharDown(UINT *vcKey,BOOL processed)
{
	UNREFERENCED_PARAMETER(*vcKey);
	UNREFERENCED_PARAMETER(processed);
	StartEdit( *vcKey ); 
//	GetScrollBarCtrl(SB_HORZ)->EnableScrollBar(ESB_DISABLE_BOTH);
}
	
/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent every time the grid needs to
//		draw a cell in the grid. At this point the cell
//		object has been populated with all of the information
//		that will be used to draw the cell. This information
//		can now be changed before it is used for drawing.
//	Warning:
//		This notification is called for each cell that needs to be painted
//		Placing complicated calculations here will slowdown the refresh speed.
//	Params:
//		col, row	- coordinates of cell currently drawing
//		cell		- pointer to the cell object that is being drawn
//	Return:
//		<none>
void MyCug::OnGetCell(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetCell
//		This notification is sent every time a cell is set,
//		here you have a chance to make final modification
//		to the cell's content before it is saved to the data source.
//	Params:
//		col, row	- coordinates of cell currently saving
//		cell		- pointer to the cell object that is being saved
//	Return:
//		<none>
void MyCug::OnSetCell(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);
}

/////////////////////////////////////////////////////////////////////////////
//	OnDataSourceNotify
//		This message is sent from a data source and this message
//		depends on the data source - check the information
//		on the data source(s) being used
//		- The ID of the Data source is also returned
//	Params:
//		ID		- datasource ID
//		msg		- message ID to identify current process
//		param	- additional information or object that might be needed
//	Return:
//		<none>
void MyCug::OnDataSourceNotify(int ID,long msg,long param)
{
	UNREFERENCED_PARAMETER(ID);
	UNREFERENCED_PARAMETER(msg);
	UNREFERENCED_PARAMETER(param);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellTypeNotify
//		This notification is sent by the celltype and it is different from cell-type
//		to celltype and even from notification to notification.  It is usually used to
//		provide the developer with some feed back on the cell events and sometimes to
//		ask the developer if given event is to be accepted or not
//	Params:
//		ID			- celltype ID
//		col, row	- co-ordinates cell that is processing the message
//		msg			- message ID to identify current process
//		param		- additional information or object that might be needed
//	Return:
//		TRUE - to allow celltype event
//		FALSE - to disallow the celltype event
int MyCug::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	UNREFERENCED_PARAMETER(ID);
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(msg);
	UNREFERENCED_PARAMETER(param);

	SimB *pSw;
	Tab *pTab;

	CUGCell cell, cell1, cell2; 
	string Type, Group, Name, Prior, After, test;
	int iopt;



//******* Amend the cell using 'Get..Set'
	int isheet=GetSheetNumber();
	if (m_pSimDoc->m_DocumentType == 2) {
		m_pProp->DocumentError(m_pSimDoc);
		return FALSE;
	}
/*	if(m_pSimDoc->p_ModelInfo->IsRunning()) {
		if(isheet==12) return TRUE;
		MFC_Util::MessageBox(" You can not modify document when modell is running !");
		return FALSE;

	}*/

	if(m_pSimDoc->IsFinishedSimulation()){

		if(isheet==5&&(col==3|| col==4)) {
			return TRUE;
		}
		else if (isheet == 4 && col == 2&& msg == 103) {
			GetCell(col, row, &cell);
			if (cell.GetParam() >= 2200 && cell.GetParam() < 2204) {
				size_t file = cell.GetParam() - 2200;
				string txt = cell.GetStdText();
				m_pSimDoc->m_ValsumFlexFiles[file].IDVarKey = txt;

			}
			return TRUE;
		}
		else if (isheet == 4 && col == 3) {

			return FALSE;
		}
		else if(isheet==11&&col==3) {
			return FALSE;
			
		}
		else if(isheet==6&&col<=4&&m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val()>0) {
			return FALSE;

		}
		else if(isheet==0&&row>34)
			return TRUE;
		else if(isheet==10&&(col==2||m_ShowCoVarRes))
			return TRUE;

		if (!(isheet == 4 && col == 2)) {
			GetCell(col, row, &cell);
			cell.SetReadOnly(TRUE);
			SetCell(col, row, &cell);
		}
		
		
		return TRUE;
	}
	else if(m_pSimDoc->IsRunning()) {
		if(isheet==1) return FALSE;



	}

	if(isheet==1 && ID==1 && msg == 101||isheet==0 && ID==1 && msg == 101
		||isheet==10 && ID==1 && msg == 101) {
			GetCell(col,row, &cell);
			m_Prior=cell.GetStdText();

	}
	else if(isheet==1 && ID==1 && msg == 103){
			GetCell(col,row, &cell);
			m_After=cell.GetStdText();
			if(m_Prior!=m_After&&!m_pSimDoc->IsRunning()) {
				GetCell(col-1,row, &cell1);
				Name=cell1.GetStdText();
				GetCell(col-3,row, &cell1);
				Group=cell1.GetStdText();
				Type="Switches";
				
				pSw=m_pSimDoc->GetPtr(Type, Group, Name);
				size_t recalc=((Sw*)pSw)->SetStrValue(string(m_After));
				if(recalc>0) 
					m_pSimDoc->LinkedChangeToSwitches(((Sw*)pSw), recalc);
					
				if(m_ShowAllSwitches)
					m_pSimDoc->SetOriginalValues("Switches", "ALL");
				else
					m_pSimDoc->SetOriginalValues("Switches",Group);
				m_pSimDoc->TimeModified();
				m_pSimDoc->History_Add(pSw,int(HIST_INFO::SWITCH_INFO),time(nullptr),string(m_After));
					m_pFrame->m_wndClassView.FillClassView();

					long topr;
					SetSheetNumber(2, FALSE);
					topr=GetTopRow();
					m_pSimDoc->SetOriginalValues("Parameters","ALL");
					EnableUpdate(FALSE); 
					while(GetNumberRows()>0)
							DeleteRow(0);
					//EnableUpdate(TRUE); 
					OnSheetSetup(2);
					SetTopRow(topr);


					SetSheetNumber(3, FALSE);
					GetTopRow();
					m_pSimDoc->SetOriginalValues("Tables","ALL");

					EnableUpdate(FALSE); 
					while(GetNumberRows()>0)
							DeleteRow(0);
					//EnableUpdate(TRUE); 
					OnSheetSetup(3);
					SetTopRow(topr);


					SetSheetNumber(4, FALSE);
					GetTopRow();
					m_pSimDoc->SetOriginalValues("Files","ALL");
					EnableUpdate(FALSE); 
					while(GetNumberRows()>0)
							DeleteRow(0);
					//EnableUpdate(TRUE); 
					OnSheetSetup(4);
					SetTopRow(topr);

					SetSheetNumber(5, FALSE);
					GetTopRow();
					m_pSimDoc->SetOriginalValues("Outputs","ALL");

					EnableUpdate(FALSE); 
					while(GetNumberRows()>0)
							DeleteRow(0);
					//EnableUpdate(TRUE); 
					OnSheetSetup(5);
					SetTopRow(topr);
					
					SetSheetNumber(isheet, FALSE);
					topr=GetTopRow();
					EnableUpdate(FALSE); 
					while(GetNumberRows()>0)
							DeleteRow(0);
					EnableUpdate(TRUE); 
					OnSheetSetup(isheet);
					SetTopRow(topr);

	

			}

	}
	else if(isheet==0 && ID==1 && msg == 103){

			GetCell(col,row, &cell);
			m_After=cell.GetStdText();
			if(m_Prior!=m_After) {
				if(col==1) {
					Name="TimeResolution";
					Group="Technical";
					Type="Switches";
			
					pSw=m_pSimDoc->GetPtr(Type, Group, Name);
					((Sw*)pSw)->SetStrValue(string(m_After));
					m_pSimDoc->History_Add(6,string(m_After));
				}
				else if(col==3) {
					Name="NitrogenCarbonStep";
					Group="Model Structure";
					Type="Switches";		
					pSw=m_pSimDoc->GetPtr(Type, Group, Name);
					((Sw*)pSw)->SetStrValue(string(m_After));
				}
				EnableUpdate(FALSE); 
				
				while(GetNumberRows()>0)
					DeleteRow(0);
				EnableUpdate(TRUE); 
				OnSheetSetup(isheet);
			}

	}
	else if(isheet==3 && ID==1 && msg ==103) {
		if(col==7) {
			Type="Tables";
			GetCell(0,row,&cell);
			Group=cell.GetStdText();
			GetCell(2,row,&cell);
			Name=cell.GetStdText();
			GetCell(7,row,&cell);
			After=cell.GetStdText();
			
			pSw=m_pSimDoc->GetPtr(Type, Group, Name);
			if(pSw!=nullptr) {
				pTab=(Tab*)pSw;
				iopt=pTab->GetViewFunction();
				
			}


		}
	}
	else if(isheet==2 && ID==1 && msg ==103) {
		GetCell(col,row, &cell);
		m_After=cell.GetStdText();
		if(col==8) {
			Type="Parameters";
			GetCell(0,row,&cell);
			Group=cell.GetStdText();
			GetCell(2,row,&cell);
			Name=cell.GetStdText();
			GetCell(8,row,&cell);
			After=cell.GetStdText();
			string options=cell.GetStdLabelText();
			
			pSw=m_pSimDoc->GetPtr(Type, Group, Name);
			if(pSw!=nullptr) {
				Ps *pPs=dynamic_cast<Ps*>(pSw);
				size_t ivalue;
				ivalue=FUtil::GetOptionIndex(options, After);
				if(pPs!=nullptr) {
					pPs->SetSelectedFunc(ivalue);
					Make_ParameterChart(pPs,nullptr,string::npos,ivalue);
				}
				
			}


		}
	}
	else if(isheet==6 && ID==2 && msg ==53&&!m_pSimDoc->IsFinishedSimulation()) {

		BOOL Acc, LogT;
		float perr, aerr;
		int ind;
		GetCell(0,row,&cell);
		ind=int(cell.GetNumber()-1);
		if(col==6) {
			GetCell(6,row,&cell);
			BOOL status=cell.GetBool();
			if(status)
				Acc=TRUE;
			else
				Acc=FALSE;
			LogT=m_pSimDoc->m_ValidationData.GetValFileLogTrans(ind);	
		}
		else if(col==7) {
			GetCell(7,row,&cell);
			BOOL status=cell.GetBool();
			if(status)
				LogT=TRUE;
			else
				LogT=FALSE;
			Acc=m_pSimDoc->m_ValidationData.GetVal_AccTestValue(ind);
		}
		if(col<6) return false;

		perr=m_pSimDoc->m_ValidationData.GetValP_Error(ind);
		aerr=m_pSimDoc->m_ValidationData.GetValA_Error(ind);
		if(m_pSimDoc->m_ValidationData.SetValErrors(ind, perr, aerr, Acc, LogT)) {
			EnableUpdate(FALSE); 
			while(GetNumberRows()>0)
					DeleteRow(0);
			EnableUpdate(TRUE); 
			OnSheetSetup(6);
		};	
	}
	else if(isheet==6 && ID==1 && msg ==103&&!m_pSimDoc->IsFinishedSimulation()) {
		float perr, aerr, obsvalue;
		int ind;
		GetCell(0,row,&cell);
		ind=int(cell.GetNumber())-1;
		GetCell(col,row,&cell);
		int ip=int(cell.GetNumber());
		string text=cell.GetStdText();
		ip=GetSV_Valoption(text);

		perr=m_pSimDoc->m_ValidationData.GetValSumVarP_Error(ind);
		aerr=m_pSimDoc->m_ValidationData.GetValSumVarA_Error(ind);
		obsvalue=m_pSimDoc->m_ValidationData.GetValSumVarMVAL(ind);
		if(m_pSimDoc->m_ValidationData.SetValSumVarErrors(ind, ip, obsvalue, perr, aerr)) {
			EnableUpdate(FALSE); 
			while(GetNumberRows()>0)
					DeleteRow(0);
			EnableUpdate(TRUE); 
			OnSheetSetup(6);
		};	


	}
	else if(isheet==10 && ID==1 && msg == 103) {
			GetCell(-1,row,&cell);
			int ip=int(cell.GetNumber());
			GetCell(0,row,&cell);
			
			GetCell(col,row, &cell);
			m_After=cell.GetStdText();
			SimB* pPar;
			
		if(col==0) {
			size_t idim=FUtil::AtoInt(m_Prior);
			pPar=m_pSimDoc->MR_Get_pPar(idim,ip);
			test=pPar->GetName().c_str();
			int iDim=GetMultiDimensionIndex(m_After);

			auto TabIndex=m_pSimDoc->MR_GetTabIndex(idim,ip);
			string gr=m_pSimDoc->MR_GetParGroup(idim,ip);
			if(gr=="DataBase") {
				((CDB*)pPar)->MR_Set_Dim(iDim);
			}
			else if(TabIndex==string::npos) {
				((Ps*)pPar)->MR_Set_Dim(iDim);
			}
			else {
				((P*)pPar)->MR_Set_Dim(TabIndex,iDim);
			}

			long topr=GetTopRow();
			EnableUpdate(FALSE); 
			while(GetNumberRows()>0)
					DeleteRow(0);
			EnableUpdate(TRUE); 
			OnSheetSetup(10);
			SetTopRow(topr);
		}
		else if(col==2) {
			GetCell(0,row,&cell);
			int idim=int(cell.GetNumber());
			pPar=m_pSimDoc->MR_Get_pPar(idim,ip);
			string gr=m_pSimDoc->MR_GetParGroup(idim,ip);
			//test=pPar->GetName().c_str();
			//int iDataObjekt=GetMultiDataBaseIndex(m_After);

			//TabIndex=m_pSimDoc->MR_GetTabIndex(idim,ip);
			if(gr=="DataBase") {
				GetCell(2,row,&cell);
				((CDB*)pPar)->SetName(cell.GetStdText());
				//((CDB*)pPar)->MR_Set_Dim(iDim);
			}

			long topr=GetTopRow();
			EnableUpdate(FALSE); 
			while(GetNumberRows()>0)
					DeleteRow(0);
			EnableUpdate(TRUE); 
			OnSheetSetup(10);
			SetTopRow(topr);
		}
		else if(col==3) {
			GetCell(0,row,&cell);
			int idim=int(cell.GetNumber());
			pPar=m_pSimDoc->MR_Get_pPar(idim,ip);
			test=pPar->GetName().c_str();
			int imethod=GetMultiMethod(m_After);

			auto TabIndex=m_pSimDoc->MR_GetTabIndex(idim,ip);
			if(TabIndex==string::npos) {
				if((imethod==1||imethod==3)&&((Ps*)pPar)->MR_Get_Min()<=0) {
						UpdateStatusBar("Not possible to have zero or negative values for log distibutions, set new min or method !");
				}
				else
					((Ps*)pPar)->MR_Set_Method(MR_METHOD(imethod));
			}
			else {
				if((imethod==1||imethod==3)&&((P*)pPar)->MR_Get_Min(TabIndex)<=0) {
						UpdateStatusBar("Not possible to have zero or negative values for log distibutions, set new min or method !");
				}
				else
					((P*)pPar)->MR_Set_Method(TabIndex,MR_METHOD(imethod));
			}

			long topr=GetTopRow();
			EnableUpdate(FALSE); 
			while(GetNumberRows()>0)
					DeleteRow(0);
			EnableUpdate(TRUE); 
			OnSheetSetup(10);
			SetTopRow(topr);
		}
		else if (col==7) {
			GetCell(0,row,&cell);
			int idim=int(cell.GetNumber());
			pPar=m_pSimDoc->MR_Get_pPar(idim,ip);
			test=pPar->GetName().c_str();
			auto TabIndex=m_pSimDoc->MR_GetTabIndex(idim,ip);

			int iselect=GetMultiDependenceIndex(m_After);
			int icount=0;
			int DepIndex;
			SimB *pDepPar;
			for(size_t ix=0;ix<m_pSimDoc->m_MultiRun_Array.size();ix++){
	   			for(size_t iix=0; iix<m_pSimDoc->MR_Get_NumberOfParametersWithinDim(ix+1);iix++){
					if(icount==iselect) {
						if(m_pSimDoc->MR_Get_pPar(ix+1,iix+1)!=nullptr){
							pDepPar=m_pSimDoc->MR_Get_pPar(ix+1,iix+1);
							DepIndex=m_pSimDoc->MR_GetTabIndex(ix+1,iix+1);
						}
						else
							pDepPar=nullptr;
						DepIndex=m_pSimDoc->MR_GetTabIndex(ix+1,iix+1); 
						if(TabIndex==string::npos)
							if(DepIndex>=0)
								((Ps*)pPar)->Set_MR_ParDependence((Ps*)pDepPar, DepIndex);
							else
								((Ps*)pPar)->Set_MR_ParDependence((Ps*)pDepPar);
						else
							if(DepIndex>=0)
								((P*)pPar)->Set_MR_ParDependence(TabIndex, (Ps*)pDepPar, DepIndex);
							else
								((P*)pPar)->Set_MR_ParDependence(TabIndex, (Ps*)pDepPar);
					
					}
					icount++;
				}

			}


		

			long topr=GetTopRow();
			EnableUpdate(FALSE); 
			while(GetNumberRows()>0)
					DeleteRow(0);
			EnableUpdate(TRUE); 
			OnSheetSetup(10);
			SetTopRow(topr);

			}
	



		}
	else if(isheet==12 && ID==1 && msg == 103) {

			int ij=0;
			GetCell(col,row, &cell);
			m_After=cell.GetStdText();
			string list=cell.GetStdLabelText();
			if(m_Prior!=m_After) {
				int ichart=GetOptionIndex(list, m_After);
				GetCell(col-2,row, &cell1);
				Name=cell1.GetStdText();
				int pos1=Name.rfind('(');
				int pos2=Name.rfind(')');
				int Index;

				if (pos2>pos1&&pos2==Name.size()-1) {
						string t=Name.substr(pos1+1,pos2-pos1-1);
						if(t!="no roots") {
							Index=FUtil::AtoInt(t);
							Name=Name.substr(0,pos1);
						}
						else
							Index=1;
				
				}
				else
					Index=1;

				GetCell(col-3,row, &cell1);
				Group=cell1.GetStdText();
				GetCell(col-4,row, &cell1);
				Type=cell1.GetStdText();;
								
				pSw=(SimB*)m_pSimDoc->GetPtr(Type,Group,Name);

				int itype=p_ModelInfo->GetTypeIndex(Type);


				std::list<Annim> ::iterator i;
				Annim anode;
				anode.pBase=pSw;
				anode.type=itype;
				anode.LocalIndex=Index-1;

			/*	switch (ichart) {
					case -1:
						sequence=0;
						//Remove variable if exist in any of the for lists
						for(i=m_pSimDoc->AnnimVarList1.begin(); i!=m_pSimDoc->AnnimVarList1.end();++i) {
							if((*i).pBase==pSw&&(*i).type==itype&&(*i).LocalIndex==Index-1) {
								m_pSimDoc->AnnimVarList1.erase(i);
								break;
							}
						};
						for(i=m_pSimDoc->AnnimVarList2.begin(); i!=m_pSimDoc->AnnimVarList2.end();++i) {
							if((*i).pBase==pSw&&(*i).type==itype&&(*i).LocalIndex==Index-1) {
								m_pSimDoc->AnnimVarList2.erase(i);
								break;
							}
						};
						for(i=m_pSimDoc->AnnimVarList3.begin(); i!=m_pSimDoc->AnnimVarList3.end();++i) {
							if((*i).pBase==pSw&&(*i).type==itype&&(*i).LocalIndex==Index-1) {
								m_pSimDoc->AnnimVarList3.erase(i);
								break;
							}
						};
						for(i=m_pSimDoc->AnnimVarList4.begin(); i!=m_pSimDoc->AnnimVarList4.end();++i) {
							if((*i).pBase==pSw&&(*i).type==itype&&(*i).LocalIndex==Index-1) {
								m_pSimDoc->AnnimVarList4.erase(i);
								break;
							}
						};

						break;
					case 0:
						m_pSimDoc->AnnimVarList1.push_back(anode);
						sequence=m_pSimDoc->AnnimVarList1.size();
						break;
					case 1:
						m_pSimDoc->AnnimVarList2.push_back(anode);
						sequence=m_pSimDoc->AnnimVarList2.size();
						break;
					case 2:
						m_pSimDoc->AnnimVarList3.push_back(anode);
						sequence=m_pSimDoc->AnnimVarList3.size();
						break;
					case 3:
						m_pSimDoc->AnnimVarList4.push_back(anode);
						sequence=m_pSimDoc->AnnimVarList4.size();
						break;



				}
				GetCell(col+1,row, &cell);
				cell.SetNumber(sequence);
				SetCell(col+1,row,&cell);
				if(itype<6)
					((X*)pSw)->SetAnnimateChart(Index-1,ichart, sequence);
				else if(itype<8)
					((G*)pSw)->SetAnnimateChart(Index-1,ichart, sequence);


		*/
			}




		};
	




	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditStart
//		This message is sent whenever the grid is ready to start editing a cell
//	Params:
//		col, row - location of the cell that edit was requested over
//		edit -	pointer to a pointer to the edit control, allows for swap of edit control
//				if edit control is swapped permanently (for the whole grid) is it better
//				to use 'SetNewEditClass' function.
//	Return:
//		TRUE - to allow the edit to start
//		FALSE - to prevent the edit from starting
int MyCug::OnEditStart(int col, long row,CWnd **edit)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(**edit);

	/*if( col == 1 )
		(*edit)->ModifyStyle( 0, ES_UPPERCASE ); 
	else 
		(*edit)->ModifyStyle( ES_UPPERCASE, 0 );

*/
	int isheet=GetSheetNumber();

	if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiSimulation()&&isheet==0) {
				CUGCell cell;
				GetCell(col,row, &cell);
				long ParV=cell.GetParam();
				if(ParV==110||ParV==111)
					return TRUE;
				else if(ParV!=112)
					return FALSE;
	}
	else if (m_pSimDoc->IsFinishedSimulation() && m_pSimDoc->IsMultiSimulation() && isheet == 3) {
		CUGCell cell;
		GetCell(col, row, &cell);
		long ParV = cell.GetParam();
		if (ParV == 110 || ParV == 111)
			return TRUE;
		else if (ParV != 112)
			return FALSE;
	}
	else if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiSimulation()&&m_ShowMultiRes&&row>m_pSimDoc->m_MStorage.GetNumAccepted()) 
		return TRUE;
	else if((m_pSimDoc->IsFinishedSimulation()&&!m_ShowValidRes&&isheet!=6)||isheet==11) 
		return FALSE;
	else if(m_pSimDoc->IsFinishedSimulation()&&m_ShowValidRes&&isheet==6&&(row<3||row>4||col<2)&&m_pSimDoc->IsMultiSimulation())
		return FALSE;
	else if(!m_pSimDoc->IsMultiSimulation()&&m_pSimDoc->IsFinishedSimulation()&&isheet==6)
		return FALSE;
	else if(!m_pSimDoc->IsFinishedSimulation()&&isheet==6&&col==11) {
		MFC_Util::MessageBox("Please make use of parameter table : Scaling of Observed Variables, if you want to change value !");					
		return FALSE;
	}
	else if(m_pSimDoc->IsFinishedSimulation()&&isheet<=5) 
	

		return FALSE;
	else if(!m_pSimDoc->IsFinishedSimulation()&&isheet==5)
		return FALSE;
	else if(isheet==12)
		return FALSE;
	else if(m_pSimDoc->IsRunning())
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditVerify
//		This notification is sent every time the user hits a key while in edit mode.
//		It is mostly used to create custom behavior of the edit control, because it is
//		so easy to allow or disallow keys hit.
//	Params:
//		col, row	- location of the edit cell
//		edit		-	pointer to the edit control
//		vcKey		- virtual key code of the pressed key
//	Return:
//		TRUE - to accept pressed key
//		FALSE - to do not accept the key
int MyCug::OnEditVerify(int col, long row,CWnd *edit,UINT *vcKey)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*edit);
	UNREFERENCED_PARAMETER(*vcKey);

	/*if( col == 0 ){
		// numbers are not allowed in the last name field  
		if( *vcKey >= '0' && *vcKey <= '9' ) 
			return FALSE; 
	} */





	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish
//		This notification is sent when the edit is being finished
//	Params:
//		col, row	- coordinates of the edit cell
//		edit		- pointer to the edit control
//		str		- actual str that user typed in
//		cancelFlag	- indicates if the edit is being canceled
//	Return:
//		TRUE - to allow the edit to proceed
//		FALSE - to force the user back to editing of that same cell
int MyCug::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR str1,BOOL cancelFlag)
{ 
	
	SimB *pSw;
	CUGCell cell, cell1, cell2; 
	long ParV;
	long DateMinValue;
	float fvalue;
	size_t ip, idim, TabIndex, intValue;
	string Type, Group, Name, Prior, After, test;
	SimB *pPar;
	Tab *pTab;
	CWinApp* pApp = AfxGetApp();
	string DatumString;


	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*edit);
	UNREFERENCED_PARAMETER(str1);
	UNREFERENCED_PARAMETER(cancelFlag);
	if( str1 == _T("") )
	{ 
		MFC_Util::MessageBox("Sorry, but you must enter a value" );

		// force user to continue edit and enter a value
		return FALSE; 
	} 
/*	else if(m_pSimDoc->p_ModelInfo->IsRunning()) {
		MFC_FUtil::MessageBox(" You can not edit document when modell is running !");
		return FALSE;
	}*/
	else {
		int sheetNumber = GetSheetNumber(); 
		long ivv;
		string str=CT2A(str1).m_psz;

		/*if(sheetNumber!=6&&m_pSimDoc->IsFinishedSimulation()){

			else 
				return FALSE;

		}*/
		int prev_pre_period;
		int prev_start,change;
		Sw* pSwTid=(Sw*)m_pSimDoc->GetPtr(SWITCH,"TimeResolution");
		bool daily=false, hourly=false;
		if(pSwTid->GetIntValue()==1) daily=true;
		if(pSwTid->GetIntValue()==2) hourly=true;
		switch(sheetNumber) {
			case(0):
				GetCell(col,row, &cell);
				ParV=cell.GetParam();
				switch(ParV) {
					case(101):
						m_pSimDoc->m_DocFile.m_SimulationRunNo=FUtil::AtoInt(str);
						m_pSimDoc->History_Add(0,string(str));
						MFC_Util::WriteProfileInt( "SimulationRunNo",m_pSimDoc->m_DocFile.m_SimulationRunNo);
						break;
					case(102):
						FUtil::trim_datestr(str);					
						DatumString=str;
						
						
						DateMinValue=PGUtil::MinutConv(DatumString, daily, hourly);


						prev_pre_period=m_pSimDoc->RunOpt.longMinStart-m_pSimDoc->RunOpt.longMinPreStart;
						prev_start=m_pSimDoc->RunOpt.longMinStart;
						m_pSimDoc->RunOpt.longMinStart=DateMinValue;
						str=DatumString=PGUtil::StringDatum(m_pSimDoc->RunOpt.longMinStart).c_str();

						change=prev_start-m_pSimDoc->RunOpt.longMinStart;
						if(prev_pre_period==0||change>prev_pre_period) {
							m_pSimDoc->RunOpt.longMinPreStart=m_pSimDoc->RunOpt.longMinStart;
							m_pSimDoc->m_DocFile.ipredays = 0.;
							m_pSimDoc->History_Add(4,string(str));
						}
						else if (prev_pre_period > change) {
							m_pSimDoc->m_DocFile.ipredays -=change/1440.;
						}
						m_pSimDoc->History_Add(2,string(str));
						break;
					case(103):
						FUtil::trim_datestr(str);	
						DatumString=str;
						{
							auto prevminend = m_pSimDoc->RunOpt.longMinEnd;
							DateMinValue = PGUtil::MinutConv(DatumString, daily, hourly);
							m_pSimDoc->RunOpt.longMinEnd = DateMinValue;
							auto changemin = DateMinValue - prevminend;
							m_pSimDoc->RunOpt.longMinPostEnd += changemin;
						}
						str=DatumString=PGUtil::StringDatum(m_pSimDoc->RunOpt.longMinEnd).c_str();

						m_pSimDoc->History_Add(3,string(str));
						break;
					case(104):
						FUtil::trim_datestr(str);	
						DatumString=str;

						DateMinValue=PGUtil::MinutConv(DatumString, daily, hourly);
						m_pSimDoc->RunOpt.longMinPreStart=DateMinValue;
						m_pSimDoc->m_DocFile.ipredays=max(0,m_pSimDoc->RunOpt.longMinStart-m_pSimDoc->RunOpt.longMinPreStart)/1440;

						m_pSimDoc->History_Add(4,string(str));

						break;
					case(113):
						FUtil::trim_datestr(str);
						DatumString = str;
						m_pSimDoc->RunOpt.longMinPostEnd = PGUtil::MinutConv(DatumString, daily, hourly);
						m_pSimDoc->m_DocFile.ipostdays = max(0, m_pSimDoc->RunOpt.longMinPostEnd - m_pSimDoc->RunOpt.longMinEnd) / 1440;

						//m_pSimDoc->History_Add(13, string(str));

						break;
					case(105):
						m_pSimDoc->m_DocFile.m_TimePeriodScaleFactor=FUtil::AtoInt(str);
						//PARAMETERS_mp_TIMEPERIODSCALEFACTOR=FUtil::AtoInt(str);
						m_pSimDoc->History_Add(5,string(str));
						break;
					case(106):
						// Resolution switchm_pDoc->m_Doc.m_DocFile.m_SimulationRunNo=FUtil::AtoInt(str);
						break;
					case(107):
						m_pSimDoc->RunOpt.oidays=FUtil::AtoInt(str);
						ivv=m_pSimDoc->RunOpt.oidays*1440+m_pSimDoc->RunOpt.oiminutes;
						str=FUtil::STD_ItoAscii(ivv);
						m_pSimDoc->History_Add(7,string(str));
						break;
					case(108):
						m_pSimDoc->RunOpt.oiminutes=FUtil::AtoInt(str);
						ivv=m_pSimDoc->RunOpt.oidays*1440+m_pSimDoc->RunOpt.oiminutes;
						str=FUtil::STD_ItoAscii(ivv);
						m_pSimDoc->History_Add(7,string(str));
						break;
					case(109):
						m_pSimDoc->RunOpt.noofiter=FUtil::AtoInt(str);
						m_pSimDoc->History_Add(8,string(str));
						break;
					case(110):
						// m_pSimDoc->m_DocFile.m_SimulationRunNo=FUtil::AtoInt(str);
						m_pSimDoc->m_DocFile2.m_RunId=str;
						m_pSimDoc->History_Add(9,string(str));
						break;
					case(111):
						m_pSimDoc->m_DocFile2.m_Comments=str;
						m_pSimDoc->History_Add(10,string(str));
						break;
					case(112):

						m_pSimDoc->MR_SetNewNumberofTotalRuns(FUtil::AtoInt(str));
							//MR_SetRepeatitionsOfDimension(0, );
						
						
						m_pSimDoc->History_Add(1,string(str));
						break;
				}
				if(col==3&&row==11) {
					auto pParNitStep=(Ps*)m_pSimDoc->GetPtr(PAR_SINGLE,"NC Iteration");
					fvalue=FUtil::AtoFloat(str);
					pParNitStep->SetValue(fvalue);
				}
				/*EnableUpdate(FALSE); 
				while(GetNumberRows()>0)
					DeleteRow(0);
				EnableUpdate(TRUE); */
				OnSheetSetup(sheetNumber);
				return TRUE;
				break;
			case(1): // Change of Switches are made in
				break;
			case(2):// Change of parameter values
				if(col==3) {
					GetCell(col-1,row, &cell1);
					Name=cell1.GetStdText();
					GetCell(col-3,row, &cell1);
					Group=cell1.GetStdText();
					//cell1.GetNumber(&intValue);
					fvalue=FUtil::AtoFloat(str);
					Type="Parameters";
					
					pSw=m_pSimDoc->GetPtr(Type, Group, Name);
					
					if(pSw!=nullptr) {
						long topr=GetTopRow();
						ModelUtil::SetParValue(m_pSimDoc,dynamic_cast<Ps*>(pSw), fvalue);
						UpdateSheet(sheetNumber);
						SetTopRow(topr);
					}
					else {
						return FALSE;
					}
				}
				break;
			case(3) : // Change of parameter tables elements
				GetCell(col, row, &cell);
				ParV=cell.GetParam();
				if (ParV==3000) {
					// Change of Element value
					GetCell(col-3,row, &cell1);
					Name=cell1.GetStdText();
					GetCell(col-5,row, &cell1);
					Group=cell1.GetStdText();
					GetCell(col,row, &cell1);
					int intValue;
					//cell1.GetNumber(&intValue);
					intValue=FUtil::AtoInt(str);
					Type="Tables";
					
					pSw=m_pSimDoc->GetPtr(Type, Group, Name);
					if(pSw!=nullptr) {
					auto oldnelements=int(((Tab*)pSw)->GetValue());
		  			if (intValue!=oldnelements)  {
	  					((Tab*)pSw)->SetNumOfRow(intValue);
	  		  			//pSw->Apply_CE_only();
						//m_pSimDoc->ReallocateTheWholeMap();
						m_pSimDoc->TimeModified();


						pSw=m_pSimDoc->GetPtr(Type, Group, Name);
						auto itype = int(HIST_INFO::NUMBER_ELEMENTS);

						m_pSimDoc->History_Add(pSw,itype,-1,time(nullptr),string(str));
						EnableUpdate(FALSE); 
						while(GetNumberRows()>0)
							DeleteRow(0);
						EnableUpdate(TRUE); 
						OnSheetSetup(sheetNumber);
					}

				}
				}
				else if (ParV>=2000||ParV==110) { // Change of Value within tables
					int index;
					index=ParV-2000;
					if (ParV == 110) { // Special case for Criteria Table
						GetCell(2, row, &cell);
						auto str_index = cell.GetStdText();
						SetCell(2, row, &cell);
						index = FUtil::AtoInt(str_index)-1;

					}
					if(m_ParameterTableView==0) {
						GetCell(col,row-index-1,&cell);
						Name=cell.GetStdText();
						GetCell(0,row-index-2,&cell);
						Group=cell.GetStdText();

					}
					else {
						GetCell(1,row,&cell);
						Name=cell.GetStdText();
						GetCell(0,row,&cell);
						Group=cell.GetStdText();
					}
					fvalue=FUtil::AtoFloat(str);
					Type="Table Parameters";
					
					pSw=m_pSimDoc->GetPtr(PAR_TABLE, Name);
					if(pSw==nullptr) {
						Group="Plant";
						pSw=m_pSimDoc->GetPtr(Type, Group, Name);
						if(pSw==nullptr) {
							Group="Soil Profile";
							pSw=m_pSimDoc->GetPtr(Type, Group, Name);


						}
					}
					P* pP=(P*)pSw;
					if(m_ParameterTableView==0) {				
						GetCell(2,row-index-2, &cell1);
						Name=cell1.GetStdText();
						pSw=m_pSimDoc->GetPtr(TABLE, Name);
						pTab=(Tab*)pSw;
					}
					else
						pTab=nullptr;



					if(pP!=nullptr/*&&!pP->IsReadOnly()*/) {
						ModelUtil::SetParTableValue(m_pSimDoc, pP, index, fvalue);
					}

					cell.SetBackColor(RGB(80,240,240));		
						
				m_ClickedTable=Name;
				long rowsheet=row-GetTopRow();
				EnableUpdate(FALSE); 
				while(GetNumberRows()>0)
						DeleteRow(0);
				EnableUpdate(TRUE); 
				OnSheetSetup(sheetNumber);
				if(m_ClickedRow>0)
					SetTopRow(m_ClickedRow-rowsheet-1);

				}
			
			case (4) : // Change of Model files
				GetCell(col,row, &cell);
				ParV=cell.GetParam();
				if (ParV==4000) { 
					GetCell(col-1,row, &cell1);
					//GetCell(col-1,row, &cell1);
					Name=cell1.GetStdText();
					GetCell(col-2,row, &cell1);
					Group=cell1.GetStdText();
					Type="Files";
					
					pSw=m_pSimDoc->GetPtr(Type, Group, Name);
					if(pSw!=nullptr) {
						((F*)pSw)->SetValue(string(str));
						//pSw->Apply();
						if(((F*)pSw)->CheckFileNameAndAssignNameToPGClass()){
							m_pSimDoc->TimeModified();
							m_pSimDoc->History_Add(pSw,-1,time(nullptr),string(str));
							return TRUE;
							EnableUpdate(FALSE); 
							while(GetNumberRows()>0)
								DeleteRow(0);
							EnableUpdate(TRUE); 
							OnSheetSetup(sheetNumber);
						}
						return FALSE;
					}
				}

			break;
			case (6) :  // Change of Validation variables
				if(!m_pSimDoc->IsFinishedSimulation()) {
										if(col>=6||col<=16) {
						float perr, aerr, obsvalue;
						int ind, itypvalue;
						GetCell(6,row,&cell);
						int typ=cell.GetCellType();
						if(typ!=UGCT_CHECKBOX) {
							
							GetCell(0,row,&cell);
						
							ind=int(cell.GetNumber())-1;
							itypvalue=m_pSimDoc->m_ValidationData.GetValSumVarOutputType(ind);
							perr=m_pSimDoc->m_ValidationData.GetValSumVarP_Error(ind);
							aerr=m_pSimDoc->m_ValidationData.GetValSumVarA_Error(ind);
							obsvalue=m_pSimDoc->m_ValidationData.GetValSumVarMVAL(ind);

							if (col==6)	{	
								obsvalue=FUtil::AtoFloat(str);
							}
							else if(col==7) {
								perr=FUtil::AtoFloat(str);
							}
							else if( col==8) {
								aerr=FUtil::AtoFloat(str);
							}



							if(m_pSimDoc->m_ValidationData.SetValSumVarErrors(ind, itypvalue, obsvalue, perr, aerr)) {
								EnableUpdate(FALSE); 
								while(GetNumberRows()>0)
										DeleteRow(0);
								EnableUpdate(TRUE); 
								OnSheetSetup(6);
							};	

						}
						else {
							BOOL Acc, LogT;
							int iDur;
							LogT=FALSE;
							Acc=FALSE;

							GetCell(0,row,&cell);
							
							ind=int(cell.GetNumber())-1;
	

							Acc=m_pSimDoc->m_ValidationData.GetVal_AccTestValue(ind);
							perr=m_pSimDoc->m_ValidationData.GetValP_Error(ind);
							aerr=m_pSimDoc->m_ValidationData.GetValA_Error(ind);
							
							if (col==6)	{	
								if(str=="x")
									Acc=TRUE;
								else
									Acc=FALSE;
							}
							if (col==7)	{	
								if(str=="x")
									LogT=TRUE;
								else
									LogT=FALSE;
							}
							else if(col==8) {
								perr=FUtil::AtoFloat(str);
							}
							else if( col==9) {
								aerr=FUtil::AtoFloat(str);
							}
							else if( col==10) {
								iDur=FUtil::AtoInt(str);
								m_pSimDoc->m_ValidationData.SetValDuration(ind, iDur);
							}
							else if (col == 12 || col == 14) {
								auto pgindex = FUtil::AtoInt(str);
								if (col == 12) m_pSimDoc->m_Val_Array[ind].PG_FilterIndex = pgindex;
								if (col == 14) m_pSimDoc->m_Val_Array[ind].PG_QualityIndex = pgindex;
							}
							else if (col == 13) {
								auto pgvalue = FUtil::AtoInt(str);
								m_pSimDoc->m_Val_Array[ind].PG_FilterValue = pgvalue;
							}
							else if (col == 15) {
								auto pgvalue = FUtil::AtoFloat(str);
								m_pSimDoc->m_Val_Array[ind].Quality_Threshold = pgvalue;
							}



							if(m_pSimDoc->m_ValidationData.SetValErrors(ind, perr, aerr, Acc, LogT)||col==11) {
								EnableUpdate(FALSE); 
								while(GetNumberRows()>0)
										DeleteRow(0);
								EnableUpdate(TRUE); 
								OnSheetSetup(6);
							};

						}
					}
					
					return TRUE;

				}
				else if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiStorageBinFileValid()&&m_pSimDoc->IsMultiSimulation()) {
					if((row==3||row==4)&&col>1) {
							int ncol=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
							int ncolextra=0;
							if(ncol>1) ncolextra=6;
							int ncol2=m_pSimDoc->m_ValidationData.GetNumSumVarVariables();
							float fvalue=FUtil::AtoFloat(str);
							GetCell(col,row, &cell);
							float foldvalue=cell.GetNumber();
							if(col>ncol-1&&col<ncol+ncolextra+2) {
								int iv=col-ncol-2;
								if(abs(foldvalue-fvalue)>0.0001*foldvalue) {
									if(row==3) {
										m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_LOW][iv] = fvalue;
										QuickSetBackColor(col,row,m_ColorEdit);
										m_ShowValidRes_Changed=TRUE;
									}
									else if(row==4) {
										m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_HIGH][iv] = fvalue;
										QuickSetBackColor(col,row,m_ColorEdit);
										m_ShowValidRes_Changed=TRUE;
									}
									if(m_ShowValidRes_Changed) {
										QuickSetText(0,-1,"To Be Updated");
										QuickSetBackColor(0,-1,m_ColorChanged);
										RedrawCell(0,-1);
									}
									else {
										QuickSetText(0,-1,"Updated");
										QuickSetBackColor(0,-1,m_ColorCompleted);
										RedrawCell(0,-1);
									}

								}

							}
							else if(col>ncol-1&&col<7*ncol+2+ncolextra) {
								if(abs(foldvalue-fvalue)>0.0001*foldvalue) {
									if(row==3) {
										m_pSimDoc->m_MStorage.SetValidLow(col-ncol-2-ncolextra,0,fvalue);
										QuickSetBackColor(col,row,m_ColorEdit);
										m_ShowValidRes_Changed=TRUE;
									}
									else if(row==4) {
										m_pSimDoc->m_MStorage.SetValidHigh(col-ncol-2-ncolextra,0,fvalue);
										QuickSetBackColor(col,row,m_ColorEdit);
										m_ShowValidRes_Changed=TRUE;
									}
									if(m_ShowValidRes_Changed) {
										QuickSetText(0,-1,"To Be Updated");
										QuickSetBackColor(0,-1,m_ColorChanged);
										RedrawCell(0,-1);
									}
									else {
										QuickSetText(0,-1,"Updated");
										QuickSetBackColor(0,-1,m_ColorCompleted);
										RedrawCell(0,-1);
									}

								}
							}
							else if(col<ncol*7+2+ncol2+ncolextra) {
								if(abs(foldvalue-fvalue)>0.0001*foldvalue) {
									if(row==3) {

										m_pSimDoc->m_MStorage.SetValidSumLow(col-(ncol*7+2)-ncolextra,fvalue);
										QuickSetBackColor(col,row,m_ColorEdit);
										m_ShowValidRes_Changed=TRUE;
									}
									else if(row==4) {
										m_pSimDoc->m_MStorage.SetValidSumHigh(col-(ncol*7+2)-ncolextra,fvalue);
										QuickSetBackColor(col,row,m_ColorEdit);
										m_ShowValidRes_Changed=TRUE;
									}
									if(m_ShowValidRes_Changed) {
										QuickSetText(0,-1,"To Be Updated");
										QuickSetBackColor(0,-1,m_ColorChanged);
										RedrawCell(0,-1);
									}
									else {
										QuickSetText(0,-1,"Updated");
										QuickSetBackColor(0,-1,m_ColorCompleted);
										RedrawCell(0,-1);
									}

								}




							}
							
							/*m_pSimDoc->m_MStorage.UpdateAccepted();
							EnableUpdate(FALSE); 
							while(GetNumberRows()>0)
									DeleteRow(0);
							EnableUpdate(TRUE); 
							OnSheetSetup(6);*/

						


					}
				}
				break;
			case (9) :

				switch (col) {
					case (2):
					case (3):	
					GetCell(-1,row,&cell);
					ip=int(cell.GetNumber());

					string DatumString;
					if(col==2) {
						GetCell(3,row,&cell);
						fvalue=float(cell.GetNumber());
						DatumString=str;
					}
					else {
						GetCell(2,row,&cell);
						DatumString=cell.GetStdText();
						fvalue=FUtil::AtoFloat(str);
					}
					FUtil::trim_datestr(str);	
					DatumString=str;
					DateMinValue=PGUtil::MinutConv(DatumString);
					GetCell(0,row,&cell);
					Group=cell.GetStdText();
					GetCell(1,row,&cell);
					Name=cell.GetStdText();
					int ist=Name.find("(");
					int iend=Name.find(")");
					if(ist>0&&iend>ist) {
						string str=Name.substr(ist+1,iend-ist-1);
						TabIndex=FUtil::AtoInt(str);			
						TabIndex--;
						Name=Name.substr(0,ist);
						Type="Table Parameters";
					}
					else {
						TabIndex=0;
						Type="Parameters";
					}
					
					if(Group=="DataBase") {
						CDB* pDB;
						Name=cell.GetStdText();
						pDB=m_pSimDoc->DB_GetPointer(Name);
						// No action for the moment Need create
						// editfunction for pDB


					}
					else {
						pSw=m_pSimDoc->GetPtr(Type, Group, Name);
						if(pSw!=nullptr&&ip>0) {
							if(ist>0){
								if(((P*)pSw)->EditChaDateAndValue(TabIndex, DateMinValue, fvalue, ip-1)) {
									cell.SetBackColor(m_ColorEdit);
									UpdateSheet(9);
								}
							}
							else {
								if(((P*)pSw)->EditChaDateAndValue(-1, DateMinValue, fvalue, ip-1)){

									UpdateSheet(9);
								
								};
							}
						}
					}



					


					break;


				};
				break;
			case (10) : // Change of Multiple Run Parameters

				if(m_ShowMultiRes&&row>m_pSimDoc->m_MStorage.GetNumAccepted()||(row>=m_pSimDoc->MR_Get_NumBayesPar()&&m_pSimDoc->IsBayesianCalibration()&&col==2)) {
					GetCell(-1,row,&cell);
					ip=int(cell.GetNumber());
					if(ip==1000||col==2) {
						int Intvalue=FUtil::AtoInt(str);
						m_pSimDoc->SetNumBurnIn(Intvalue);
						m_pSimDoc->MR_ReCalculatePostDist(m_pSimDoc->IsBayesianCalibration());
						return TRUE;

					}
					else
						return FALSE;



					return TRUE;
				}

				GetCell(-1,row,&cell);
				ip=int(cell.GetNumber());
				GetCell(0,row,&cell);
				idim=int(cell.GetNumber());
				pPar=m_pSimDoc->MR_Get_pPar(idim,ip);
				TabIndex=m_pSimDoc->MR_GetTabIndex(idim,ip);
				test=pPar->GetName().c_str();
				GetCell(col,row,&cell);
					//cell1.GetNumber(&intValue);
				fvalue=FUtil::AtoFloat(str);
				//fvalue=edit.Get;;
				switch (col) {

					case 4:	

						if(pPar->Is_P()) {
							if((pPar->MR_Get_Method()==1||pPar->MR_Get_Method()==3)&&fvalue<=0) {
								UpdateStatusBar("Not possible to have zero or negative values for log distibutions, set new min !");
								if(((Ps*)pPar)->MR_Get_Min()<=0)
									((Ps*)pPar)->MR_Set_Min(0.001);
								fvalue=((Ps*)pPar)->MR_Get_Min();
								cell.SetNumber(fvalue);	
								SetCell(col,row,&cell);
								RedrawCell(col,row);
							}
							else
								((Ps*)pPar)->MR_Set_Min(fvalue);
						}
						else {
							if((((P*)pPar)->MR_Get_Method(TabIndex)==1||pPar->MR_Get_Method(TabIndex)==3)&&fvalue<=0) {
								UpdateStatusBar("Not possible to have zero or negative values for log distibutions, set new min !");
								if(((P*)pPar)->MR_Get_Min(TabIndex)<=0)
									((P*)pPar)->MR_Set_Min(TabIndex, 0.001);
								fvalue=((P*)pPar)->MR_Get_Min(TabIndex);
								cell.SetNumber(fvalue);
								SetCell(col,row,&cell);
								RedrawCell(col,row);

							}
							else
								((P*)pPar)->MR_Set_Min(TabIndex,fvalue);
						}
						OnTabSelected(10);
						return TRUE;
					case 5:	

						if(pPar->Is_P()) 
							((Ps*)pPar)->MR_Set_Max(fvalue);
						else
							((P*)pPar)->MR_Set_Max(TabIndex,fvalue);


					/*	if(TabIndex<=-1) {
							((Ps*)pPar)->MR_Set_Max(fvalue);
						}
						else {
							((P*)pPar)->MR_Set_Max(TabIndex,fvalue);
						}
						break;*/
					case 6:	

						if(pPar->Is_P()) 
							((Ps*)pPar)->MR_Set_Start(fvalue);
						else
							((P*)pPar)->MR_Set_Start(TabIndex,fvalue);

						if(TabIndex==string::npos) {
							((Ps*)pPar)->MR_Set_Start(fvalue);
						}
						else {
							((P*)pPar)->MR_Set_Start(TabIndex,fvalue);
						}
						break;
					case 8: //Change repitition
						intValue=FUtil::AtoInt(str);
		
						m_pSimDoc->MR_SetRepeatitionsOfDimension(idim-1, intValue);
						break;
					case 9: //Change repitition
						m_pSimDoc->MR_SetBayesianStepSize(fvalue);


				}
				EnableUpdate(FALSE); 
				while(GetNumberRows()>0)
					DeleteRow(0);
				EnableUpdate(TRUE); 
				OnSheetSetup(sheetNumber);
				
				SetSheetNumber(0,FALSE);
				EnableUpdate(FALSE); 
				while(GetNumberRows()>0)
					DeleteRow(0);
				EnableUpdate(TRUE); 
				OnSheetSetup(0);
				SetSheetNumber(10,FALSE);


				return TRUE;
		
				
			}
		}
			
		
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditContinue
//		This notification is called when the user pressed 'tab' or 'enter' keys
//		Here you have a chance to modify the destination cell
//	Params:
//		oldcol, oldrow - edit cell that is loosing edit focus
//		newcol, newrow - cell that the edit is going into, by changing their
//							values you are able to change where to edit next
//	Return:
//		TRUE - allow the edit to continue
//		FALSE - to prevent the move, the edit will be stopped
int MyCug::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(*newcol);
	UNREFERENCED_PARAMETER(*newrow);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnMenuCommand
//		This notification is called when the user has selected a menu item
//		in the pop-up menu.
//	Params:
//		col, row - the cell coordinates of where the menu originated from
//		section - identify for which portion of the gird the menu is for.
//				  possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//						UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
//		item - ID of the menu item selected
//	Return:
//		<none>
void MyCug::OnMenuCommand(int col,long row,int section,int item)
{
	CUGCell cell,cell1, cell2; 
	string Group, type, Name;
	int sheet;
	SimB *pSw=nullptr;

	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(section);
	UNREFERENCED_PARAMETER(item);


	sheet=GetSheetNumber();
if (section == UG_GRID||(sheet==6&&item<7000)&&item>2003){ 
	
//****** The user has selected the 'Cut' option
if (item == 1996)
  CutSelected();

//****** The user has selected the 'Copy' option
if (item == 1997) {
	CopySelected();
	return;	
}

//****** The user has selected the 'Paste' option
if (item==1998) {
    Paste(); 
	return;
}
if ((sheet == 5 || sheet == 6 || sheet == 10) && item > 8003 && item < 8012) {
		int ans=ExcelExportCommand(item);
		if (ans == 0) return;	
	}
	else if ((sheet == 5) && (item==8014||item==8015)) {
		SIMB simb = GetMultiSumPointer(row);
		string varname = simb.pBase->GetName();
		int Index = GetMultiSumIndex(simb, col);
		if (simb.pBase->Is_Vector()) {
			varname += "_";
			varname += FUtil::STD_ItoAscii(simb.TabIndex + 1);

		}
		if (Index % 6 == 0) varname += "_Ini";
		else if (Index % 6 == 1) varname += "_Final";
		else if (Index % 6 == 2) varname += "_Min";
		else if (Index % 6 == 3) varname += "_Max";
		else if (Index % 6 == 4) varname += "_Mean";
		else if (Index % 6 == 5) varname += "_Acc";

		int ans = ExcelExportCommand(item, Index, varname);
		if (ans == 0) return;
	}
	else if(col>5&&sheet==5) {

		if(ManagePlotWindows(item, sheet,true))
			return;

		if(MultiRunPlotCommand(col, row, item, SUMVAR)) return;

	}
	else if(col>5&&sheet==6) {

		if(ManagePlotWindows(item, sheet,true))
			return;

		if(MultiRunPlotCommand(col, row, item, VALVAR)) return;

	}
	else if((sheet==5||sheet==6)&&item<4020){
		if(ManagePlotWindows(item, sheet)) 
			return;
	}
	else if (sheet==10) { 
		
		if (MultiRunPlotCommand(col, row, item, PARVAR)) {
			if(item==5050||item==5051) UpdateSheet(10);
			return;
		}
		
		if(ManagePlotWindows(item, sheet)) 
			return;

	}
	else if(item==3400) {

				m_LinkedChartDocument=FALSE;
				m_NumOpenDoc=1;
				m_pSimDoc->m_SelectedDocuments.clear();
				if(m_pSimDoc->m_PrevSheet==5) {
					if(m_pSimDoc->m_SelectOut.size()>0) {
						pSw=(SimB*)m_pSimDoc->m_SelectOut[m_pSimDoc->m_SelectOut.size()-1].pBase;
						TimeSerieChartClick(col, row, sheet, pSw, 0, 0, "");
					}
					else if(m_pSimDoc->m_SelectMultiSum.size()>0) {
							MultiPlotClick(5,col, row, FALSE);
					}

				}
				else if(m_pSimDoc->m_PrevSheet==6) {
					if(m_pSimDoc->m_SelectValTime.size()>0)
						TimeSerieChartClick(col, row, sheet, (SimB*)nullptr, 0, 1, "", 1);
					else if(m_pSimDoc->m_SelectMultiVal.size()>0) {
							MultiPlotClick(6,col, row, FALSE);
					}

				}
				else if(m_pSimDoc->m_PrevSheet==10) {
					if(m_pSimDoc->m_SelectMultiPar.size()>0) {
						MultiPlotClick(10,col, row, FALSE);
					}

				}
				//m_SelectedDoc.RemoveAll();
				UpdatePlotStatusBar(5);


	}
	else if(item==3401) {
				m_LinkedChartDocument=TRUE;
				m_NumOpenDoc=m_pSimDoc->m_SelectedDocuments.size();
				if(m_pSimDoc->m_PrevSheet==5) {
					if(m_pSimDoc->m_SelectOut.size()>0) {
						pSw=(SimB*)m_pSimDoc->m_SelectOut[m_pSimDoc->m_SelectOut.size()-1].pBase;
						TimeSerieChartClick(col, row, sheet, pSw, 0, 0, "");
					}
					else if(m_pSimDoc->m_SelectMultiSum.size()>0) {
						MultiPlotClick(5,col, row, FALSE);
					}
					UpdatePlotStatusBar(5);

				}
				else if(m_pSimDoc->m_PrevSheet==6) {
					if(m_pSimDoc->m_SelectValTime.size()>0)
						TimeSerieChartClick(col, row, sheet, (SimB*)nullptr, 0, 1, "", 1);
					else if(m_pSimDoc->m_SelectMultiVal.size()>0) {
						MultiPlotClick(6,col, row, FALSE);
					}
					UpdatePlotStatusBar(6);
				}
				else if(m_pSimDoc->m_PrevSheet==10) {
					if(m_pSimDoc->m_SelectMultiPar.size()>0) {
						MultiPlotClick(10,col, row, FALSE);
					}
					UpdatePlotStatusBar(10);
				}
					



	}



	int iusheet;
	switch (sheet) {
	
	case(0) :
		iusheet=RunInfoCommand(col, row, item);
		if(iusheet>=0) UpdateSheet(iusheet);

		break;

	case(1): //Switches
		iusheet=SwitchCommand(col, row, item);
		if(iusheet>=0) UpdateSheet(iusheet);
		break;
	case (2): // Parameters
		iusheet=ParameterCommand(col, row, item);
		if(iusheet>=0) UpdateSheet(iusheet);
		break;
	case (3): // Parameter Tables
		iusheet=ParameterTableCommand(col, row, item);
		if(iusheet>=0) UpdateSheet(iusheet);
		break;


	case (4): // Model Files
		iusheet=ModelFileCommand(col, row, item);
		if(iusheet>=0) UpdateSheet(iusheet);
		break;

	case (5): // Outputs
		iusheet=OutPutVarCommand(col, row, item);
		if(iusheet>=0) UpdateSheet(iusheet);
		break;

	case (6):
		iusheet=ValidationVarCommand(col, row, item);
		if(iusheet>=0) UpdateSheet(iusheet);
		break;

	case (8):
		iusheet=ChartsCommand(col, row, item);
		if(iusheet>=0) UpdateSheet(iusheet);
		break;
	case (9):
		iusheet=ChaParCommand(col, row, item);
		if(iusheet>=0) UpdateSheet(iusheet);
		break;


	case (10):// Multiple Run Options
		iusheet=MultiRunCommand(col, row, item);
		if(iusheet>=0) UpdateSheet(iusheet);
		break;

	case 11:

		HistoryCommand(col, row, item);
		break;
	case (12):
		iusheet=AnnimateCommand(col, row, item);
		if(iusheet>=0) UpdateSheet(iusheet);
		break;
	case (13) :
		iusheet = AttachedFileCommand(col, row, item);
		if (iusheet >= 0) UpdateSheet(iusheet);
		break;

	}


	if(item>=1000&&item<1050) {

	EnableUpdate(FALSE); 
	while(GetNumberRows()>0)
			DeleteRow(0);
	EnableUpdate(TRUE); 
	OnSheetSetup(sheet);
	return;

	return;



}
}
else if(section==UG_CORNERBUTTON){
	int usheet=MenuCornerCommand(col,row,item);
	if(usheet>=0) OnSheetSetup(usheet);


}
else if(section==UG_SIDEHEADING) {

	string pzPathName, Num, newPathName;
	if(item==7000) 
		CreateNewDocFromCurrentDoc();
	else if(item==7001) 
		MakeSingleRun();
	else if(item==7002) 
		MakeMultiRuns(1);
	else if(item==7003)
			MakeMultiRuns(2);	


}
else if(section==UG_TOPHEADING) {
	if (item >= 5300&&item<5340) {

		if (item == 5300) {
			if (m_ShowProcesses) {
				m_ShowProcesses = false;
				MFC_Util::WriteProfileInt("ShowProcesses", 0);
			}
			else {
				m_ShowProcesses = true;
				MFC_Util::WriteProfileInt("ShowProcesses", 1);

			}
		}
		if (item == 5301) {
			if (m_Show_C) {
				m_Show_C = false;
				MFC_Util::WriteProfileInt("Show_C", 0);
			}
			else {
				m_Show_C = true;
				MFC_Util::WriteProfileInt("Show_C", 1);
			}
		}
		if (item == 5302) {
			if (m_Show_N) {
				m_Show_N = false;
				MFC_Util::WriteProfileInt("Show_N", 0);
			}
			else {
				MFC_Util::WriteProfileInt("Show_N", 1);
				m_Show_N = true;


			}
		}
		if (item == 5303) {
			if (m_Show_P) {
				m_Show_P = false;
				MFC_Util::WriteProfileInt("Show_P", 0);
			}
			else {
				MFC_Util::WriteProfileInt("Show_P", 1);
				m_Show_P = true;


			}
		}
		if (item == 5304) {
			if (m_Show_Water) {
				m_Show_Water = false;
				MFC_Util::WriteProfileInt("Show_Water", 0);
			}
			else {
				MFC_Util::WriteProfileInt("Show_Water", 1);
				m_Show_Water = true;

			}
		}
		if (item == 5305) {
			if (m_Show_Heat) {
				m_Show_Heat = false;
				MFC_Util::WriteProfileInt("Show_Heat", 0);
			}
			else {
				MFC_Util::WriteProfileInt("Show_Heat", 1);
				m_Show_Heat = true;
			}
		}
		if (item == 5306) {
			if (m_Show_SoilGas) {
				MFC_Util::WriteProfileInt("Show_SoilGas", 0);
				m_Show_SoilGas = false;
			}
			else {
				MFC_Util::WriteProfileInt("Show_SoilGas", 1);
				m_Show_SoilGas = true;
			}
		}
		if (item == 5307) {
			if (m_Show_Climate) {
				MFC_Util::WriteProfileInt("Show_Climate", 0);
				m_Show_Climate = false;
			}
			else {
				MFC_Util::WriteProfileInt("Show_Climate", 1);
				m_Show_Climate = true;
			}
		}
		if (item == 5308) {
			if (m_Show_All_Elements) {
				MFC_Util::WriteProfileInt("Show_All_Elements", 0);
				m_Show_All_Elements = false;
			}
			else {
				MFC_Util::WriteProfileInt("Show_All_Elements", 1);
				m_Show_All_Elements = true;
			}
		}
		else if (item == 5310) {
			if (m_ShowAllSoilLayers) {
				m_ShowAllSoilLayers = false;
				MFC_Util::WriteProfileInt("ShowAllSoilLayer", 0);
			}
			else {
				m_ShowAllSoilLayers = true;
				MFC_Util::WriteProfileInt("ShowAllSoilLayer", 1);
			}
			m_ShowOnlyNonVectorOutputs = false;
		}
		else if (item == 5311) {
			if (m_ShowSoilProfile) {
				m_ShowSoilProfile = false;
				MFC_Util::WriteProfileInt("ShowSoilProfile", 0);
			}
			else {
				m_ShowSoilProfile = true;
				MFC_Util::WriteProfileInt("ShowSoilProfile", 1);
			}
		}
		else if (item == 5320) {
			if (m_ShowAllPlants) {
				m_ShowAllPlants = false;
				MFC_Util::WriteProfileInt("ShowAllPlants", 0);
			}
			else {
				m_ShowAllPlants = true;
				MFC_Util::WriteProfileInt("ShowAllPlants", 1);
			}
			m_ShowOnlyNonVectorOutputs = false;
		}
		else if (item == 5321) {
			if (m_ShowPlantVectors) {
				MFC_Util::WriteProfileInt("ShowPlantVectors", 0);
				m_ShowPlantVectors = false;
			}
			else {
				MFC_Util::WriteProfileInt("ShowPlantVectors", 1);
				m_ShowPlantVectors = true;

			}
		}
		else if (item == 5330) {
			if (m_ShowOnlyNonVectorOutputs) m_ShowOnlyNonVectorOutputs = false;
			else m_ShowOnlyNonVectorOutputs = true;
		}
		else if (item == 5331) {
			if (m_ShowScalar) {
				MFC_Util::WriteProfileInt("ShowScalar", 0);
				m_ShowScalar = false;
			}
			else {
				MFC_Util::WriteProfileInt("ShowScalar", 1);
				m_ShowScalar = true;

			}
		}
		
		OnSheetSetup(5);
	
	}

	else if(item==3400) {
		UnLinkDocuments();

				//m_SelectedDoc.RemoveAll();
	}
	else if(item==3401) {
		LinkDocuments();

	}
	else if(item>=2000&&item<=2090)
		HistoryCommand(col, row, item);
	}


}

/////////////////////////////////////////////////////////////////////////////
//	OnMenuStart
//		Is called when the pop up menu is about to be shown
//	Params:
//		col, row	- the cell coordinates of where the menu originated from
//		setcion		- identify for which portion of the gird the menu is for.
//					possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//						UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
//	Return:
//		TRUE - to allow menu to show
//		FALSE - to prevent the menu from poping up



/////////////////////////////////////////////////////////////////////////////
//	OnGetContextHelpID 
//		this notification is called as result of context help bing activated
//		over the UG area and should return context help ID to be displayed
//	Params:
//		col, row	- coordinates of cell which received the message
//		section		- grid section which received this message
//	Return:
//		Context help ID to be shown in the help.

void MyCug::OnGetHelp(string Name, string Group, int Type) 
{
	CWinApp* pApp = AfxGetApp();

	FUtil::trim_space(Name);
  	string lDataBaseDirectory;
	lDataBaseDirectory=MFC_Util::GetProfileStringStd("DataBaseDirectory",lDataBaseDirectory);
    CString txt;
	
	m_HelpDir=lDataBaseDirectory;
	m_HelpDir+="\\Coup99.chm";
	m_HelpDir+="::/WordDocuments/"+Name+".htm";



	if(!((CCoupModelApp*)pApp)->m_Setup.ContinuousHelp)
		return ;
	GetHelp();
}

DWORD MyCug::OnGetContextHelpID( int col, long row, int section )
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(section);

	CWinApp* pApp = AfxGetApp();

  	string lDataBaseDirectory;
	lDataBaseDirectory=MFC_Util::GetProfileStringStd("DataBaseDirectory",lDataBaseDirectory);
  
   	string txt;
	m_HelpDir=lDataBaseDirectory;
	m_HelpDir+="\\Coup99.chm";


	int sheet=GetSheetNumber();

	if(sheet==0) {
		txt="RunOptions";
	}
	else if(sheet==1) {
		if(col==1) {
			txt=QuickGetStdText(1,row);
		}
		else {
			txt=QuickGetStdText(0,row);
		}
		while(txt.find(' ')!=string::npos)
			txt.erase(txt.begin()+txt.find(' '));
		if(txt=="Plant")
			txt="DescriptionofPlant";
		else if(txt=="ExternalNinputs")
			txt="Externalinputs";
		else if(txt=="Soilevaporation")
			txt="Evaporationfromthesoilsurface";
		else if(txt=="SnowPack")
			txt="SnowDynamics";
		else if(txt=="Radiationproperties")
			txt="Radiationprocesses";
		else if(txt=="SoilHydraulic")
			txt="Soilhydraulicproperties";
		else if(txt=="SoilThermal")
			txt="SoilThermalProperties";
		else if(txt=="Commonabioticresponses")
			txt="Commonabioticfunctions";
		else if(txt=="AdditionalVariables")
			txt="Additionalabioticvariables";
		else if(txt=="SoilmineralNprocesses")
			txt="MineralNprocesses";
		else if(txt=="Soilheatflows")
			txt="SoilHeatFlow";


	}
	else if(sheet==2) {

		if(col==2) {
			txt=QuickGetStdText(2,row);
		}
		else {
			txt=QuickGetStdText(0,row);
		}
		while(txt.find(' ')!=string::npos)
			txt.erase(txt.begin()+txt.find(' '));
		if(txt=="Plant")
			txt="DescriptionofPlant";
		else if(txt=="ExternalNinputs")
			txt="Externalinputs";
		else if(txt=="Soilevaporation")
			txt="Evaporationfromthesoilsurface";
		else if(txt=="SnowPack")
			txt="SnowDynamics";
		else if(txt=="Radiationproperties")
			txt="Radiationprocesses";
		else if(txt=="SoilHydraulic")
			txt="Soilhydraulicproperties";
		else if(txt=="SoilThermal")
			txt="SoilThermalProperties";
		else if(txt=="Commonabioticresponses")
			txt="Commonabioticfunctions";
		else if(txt=="AdditionalVariables")
			txt="Additionalabioticvariables";
		else if(txt=="SoilmineralNprocesses")
			txt="MineralNprocesses";
		else if(txt=="Soilheatflows")
			txt="SoilHeatFlow";

	}
	else if(sheet==3) {
		if(col==1) {
			txt=QuickGetStdText(1,row);
		}
		else {
			txt=QuickGetStdText(0,row);
		}
		while(txt.find(' ')!=string::npos)
			txt.erase(txt.begin()+txt.find(' '));
		if(txt=="Plant")
			txt="DescriptionofPlant";
		else if(txt=="ExternalNinputs")
			txt="Externalinputs";
		else if(txt=="Soilevaporation")
			txt="Evaporationfromthesoilsurface";
		else if(txt=="SnowPack")
			txt="SnowDynamics";
		else if(txt=="Radiationproperties")
			txt="Radiationprocesses";
		else if(txt=="SoilHydraulic")
			txt="Soilhydraulicproperties";
		else if(txt=="SoilThermal")
			txt="SoilThermalProperties";
		else if(txt=="Commonabioticresponses")
			txt="Commonabioticfunctions";
		else if(txt=="AdditionalVariables")
			txt="Additionalabioticvariables";
		else if(txt=="SoilmineralNprocesses")
			txt="MineralNprocesses";
		else if(txt=="Soilheatflows")
			txt="SoilHeatFlow";

	}
	else if(sheet==4) {
		if(col==1) {
			txt=QuickGetStdText(1,row);
		}
		else {
			txt=QuickGetStdText(0,row);
		}
		while(txt.find(' ')!=string::npos)
			txt.erase(txt.begin()+txt.find(' '));

	}
	else if(sheet==5) {

		if(col==2) {
			txt=QuickGetStdText(2,row);
		}
		else {
			txt=QuickGetStdText(1,row);
		}
		while(txt.find(' ')!=string::npos)
			txt.erase(txt.begin()+txt.find(' '));

		int pos1,pos2;
		pos2=txt.rfind(')');
		if(txt.size()==pos2+1) {
			pos1=txt.rfind('(');
				if(pos1>0&&pos2>pos1) 
					txt=txt.substr(0,pos1);
		}
		if(txt=="Plant")
			txt="DescriptionofPlant";
		else if(txt=="ExternalNinputs")
			txt="Externalinputs";
		else if(txt=="Soilevaporation")
			txt="Evaporationfromthesoilsurface";
		else if(txt=="SnowPack")
			txt="SnowDynamics";
		else if(txt=="Radiationproperties")
			txt="Radiationprocesses";
		else if(txt=="SoilHydraulic")
			txt="Soilhydraulicproperties";
		else if(txt=="SoilThermal")
			txt="SoilThermalProperties";
		else if(txt=="Commonabioticresponses")
			txt="Commonabioticfunctions";
		else if(txt=="AdditionalVariables")
			txt="Additionalabioticvariables";
		else if(txt=="SoilmineralNprocesses")
			txt="MineralNprocesses";
		else if(txt=="Soilheatflows")
			txt="SoilHeatFlow";

	}
	else if(sheet==6) {

		txt="validation1";




	}
	else if(sheet==7) {
	txt="Messages1";
	}
	else if(sheet==8) {
		txt="Charts1";
	}
	else if(sheet==9) {
		txt="DynamicParameters1";
	}
	else if(sheet==10) {
		txt="MultiRun1";
	}
	else if(sheet==11) {
		txt="PGFile1";
	}
	else if(sheet==12) {
		txt="History1";
	}
	if(txt=="")
		txt="Howtounderstandthemenusystem";

	m_HelpDir+="::/WordDocuments/";
	m_HelpDir+=txt.c_str();
	m_HelpDir+=".htm";



	if(!((CCoupModelApp*)pApp)->m_Setup.ContinuousHelp)
		return 0;

	GetHelp();

	return 0;
}
void MyCug::GetHelp()
{
	string cstr=m_HelpDir;
	HWND hwnd=::HtmlHelp(nullptr,
            CString(cstr.c_str()),
            HH_DISPLAY_TOPIC,
            0) ;

}
/////////////////////////////////////////////////////////////////////////////
//	OnHint
//		Is called whent the hint is about to be displayed on the main grid area,
//		here you have a chance to set the text that will be shown
//	Params:
//		col, row	- the cell coordinates of where the menu originated from
//		str		- pointer to the str that will be shown
//	Return:
//		TRUE - to show the hint
//		FALSE - to prevent the hint from showing
int MyCug::OnHint(int col,long row,int section,CString *str)
{
	UNREFERENCED_PARAMETER(section);

	SimB *pSw;
	P *pCPt;
	CUGCell cell, cell1; 
	string Type, Group, Name;
	bool selected;
	int Index;
	CWinApp* pApp = AfxGetApp();
	if(m_pSimDoc==nullptr) return FALSE;



	long ParV;
	CString NumStr, NumChartStr;
	int sheet=GetSheetNumber();
	if(section==UG_GRID) {

		OnGetContextHelpID(col, row,section);
		switch (sheet) {
		case 0:
			if(m_pSimDoc->IsFinishedSimulation()) 
				str->Format(_T("A document with completed simulation could be edited after making a new copy")) ;
			else if(row>18) {
				if(row==20) 
					str->Format(_T("Open window (Soil Data Base) and right click at soil profiles to get options for using the data base\n")
					_T("Note that also the (Soil Profile Data) window should be open to view linked information"));	

				else if(row<29)
					str->Format(_T("Open window (DB info), select a tab to open a data base and right click inside grid to get options for using the data base\n")
					_T("Note that also the (DB Data) window should be open to view linked information"));	
			}
			else if(row<18)
				str->Format(_T("Colored fields are possible to edit"));		
			else
				return FALSE;
			break;

		case 1:
			if(m_pSimDoc->IsFinishedSimulation()) 
				str->Format(_T( "A document with completed simulation could be edited after making a new copy")) ;
			else {
					GetCell(1,row, &cell1);
					Name=cell1.GetStdText();
					GetCell(0,row, &cell1);
					Group=cell1.GetStdText();
					Type="Switches";
					
					pSw=m_pSimDoc->GetPtr(Type, Group, Name);
					if(pSw!=nullptr) UpdateStatusBar(pSw ,0);

				if(col==0) {
					str->Format(_T("Click on Module field to enable all options"));		
				}
				else if(col==1) {
					str->Format(_T( "Press F1 to see explantions for the different options"));	
				}
				else 
					return FALSE;
			}
			break;
		case 2:
			if(m_pSimDoc->IsFinishedSimulation()) 
				str->Format(_T( "A document with completed simulation could be edited after making a new copy"));
			else {
					GetCell(2,row, &cell1);
					Name=cell1.GetStdText();
					GetCell(0,row, &cell1);
					Group=cell1.GetStdText();
					Type="Parameters";
					
					pSw=m_pSimDoc->GetPtr(Type, Group, Name);
					UpdateStatusBar(pSw, 1);
				if(col==0) {
					str->Format(_T("Click on upper left corner to view/hide parameters"));		
				}
				else if(col==2) {
					str->Format(_T("Press F1 to see explantions for the parameters or right click to get options"));	
				}
				else if(col==3) {
					str->Format(_T("Double click to change value of parameters"));	
				}				
				else if(col>3) {
					UpdateStatusBar("Kalle");

				}
					return FALSE;
			}
			break;





		case 3:
			if(m_pSimDoc->IsFinishedSimulation()) 
				str->Format(_T("A document with completed simulation could be edited after making a new copy")) ;
			else {
				if(col==0) {
					str->Format( _T("Click on upper left button to enable all parameter tables"));		
				}
				else if(col==2) {
					str->Format(_T("Click on Table name to view all parameters in table"));	
				}	
				else if(col==5) {
					str->Format(_T( "Double Click to edit the number of rows in the table"));	
				}
				else {

					GetCell(col, row, &cell);
					ParV=cell.GetParam();
					if(ParV>=2000) {
						int index;
						index=ParV-2000;
						GetCell(col,row-index-1,&cell);
						Name=cell.GetStdText();
						GetCell(0,row-index-2,&cell);
						Group=cell.GetStdText();
						Type="Table Parameters";
						
						pSw=m_pSimDoc->GetPtr(Type, Group, Name);


						pCPt=(P*)pSw;
						if(pCPt!=nullptr) {
							UpdateStatusBar(pCPt, 2);
						}
					/*	GetCell(2,row-index-2, &cell1);
						Name=cell1.GetStdText();
						Type="Tables";
						GetCell(col,row, &cell1);
						Type="Tables";
						pSw=m_pDoc->GetPtr(Name, type, Group);
						pTab=(Tab*)pSw;*/

					}
					else
						return FALSE;

				}
			}
			break;
		case 4:

				
			if(m_pSimDoc->IsFinishedSimulation()) {
				GetCell(col,row, &cell); 
				ParV=cell.GetParam();
				if(ParV==2000) {
					CString st1;
					if(m_pProp->m_ChartType ==CHART_TYPES::TIME_SERIE)
						st1="Time Serie";
					else if(m_pProp->m_ChartType ==CHART_TYPES::SCATTER)
						st1="Scatter type";
					else if(m_pProp->m_ChartType ==CHART_TYPES::FREQUENCE)
						st1="Frequence distribution";
					else if(m_pProp->m_ChartType ==CHART_TYPES::CUM_FREQUENCE)
						st1="Cumulative distribution";

					str->SetString(_T("Click Clean Chart from selected variables : "+st1));
				
				}
				else if(ParV==1000) 
					str->Format(_T( "Click on yellow name fields to view/hide variables in file"));
				else 
					str->Format(_T( "A document with completed simulation could be edited after making a new copy")) ;
			}
			else {
				GetCell(col,row, &cell); 
				ParV=cell.GetParam();
				if(ParV==2000) {
					CString st1;
					NumChartStr.Format(_T("%u"), m_CurrentChart);
					if(m_pProp->m_ChartType ==CHART_TYPES::TIME_SERIE)
						st1="Time Serie";
					else if(m_pProp->m_ChartType ==CHART_TYPES::SCATTER)
						st1="Scatter type";
					else if(m_pProp->m_ChartType ==CHART_TYPES::FREQUENCE)
						st1="Frequence distribution";
					else if(m_pProp->m_ChartType ==CHART_TYPES::CUM_FREQUENCE)
						st1="Cumulative distribution";
					NumStr.Format(_T("%1u"), m_VarNo);
	

				}
				else if(ParV==1000) 
					str->Format(_T("Click on yellow name fields to view/hide variables in file"));
				else if(col==0) {
					str->Format(_T("Click on upper left button to enable all model input files"));		
				}
				else 
					return FALSE;
			}		
		
			break;
		
		
		
		
		case 5:
				if (!m_pSimDoc->IsFinishedSimulation() && col == 1) {
					str->SetString(_T("Single click on Module Name \nto View/Hide All Variables in Module"));
					return TRUE;

				}
				else if(col==3) {
					if(m_pSimDoc->IsFinishedSimulation()) {
						string st1;
						NumChartStr.Format(_T("%u"), m_CurrentChart);
						if(m_pProp->m_ChartType ==CHART_TYPES::TIME_SERIE)
							st1="Time Serie";
						else if(m_pProp->m_ChartType ==CHART_TYPES::SCATTER)
							st1="Scatter type";
						else if(m_pProp->m_ChartType ==CHART_TYPES::FREQUENCE)
							st1="Frequence distribution";
						else if(m_pProp->m_ChartType ==CHART_TYPES::CUM_FREQUENCE)
							st1="Cumulative distribution";
						
						NumStr.Format(_T("%1u"), m_VarNo);
			

					
						if(m_pSimDoc->IsOutputBinFileValid()) {
							str->SetString(_T("Pick variable to make chart or right click to see more options"));
							return TRUE;
						}					
						else if(m_pSimDoc->IsMultiSimulation()) {

							GetCell(1,row,&cell);
							Group=cell.GetStdText();
							GetCell(3,row,&cell);
							Name=cell.GetStdText();
							GetCell(0,row, &cell);
			
							Type=cell.GetStdText();;

							int pos1=Name.rfind('(');
							int pos2=Name.rfind(')');

							if (pos2>pos1&&pos2==Name.size()-1) {
								string t=Name.substr(pos1+1,pos2-pos1-1);
								Index=FUtil::AtoInt(t);
								if(Index>0)
									Name=Name.substr(0,pos1);
								else
									Index=1;
							}
							else {
								Index=1;
							}	
							
							pSw=m_pSimDoc->GetPtr(Type, Group, Name);
							if(pSw==nullptr) return FALSE;
							selected=pSw->GetMultiStoreFlag(Index-1);
							/*if(GetTypeIndex(Type)>5 )
								((G*)pSw)->GetFlag(Index-1, &selected); 
							else
								((X*)pSw)->GetFlag(Index-1, &selected); 

								*/
							if(selected) {
								str->SetString(_T("Right click to select option for  making chart !"));		
					

							}
							else 
								str->SetString(_T("No Output PG Bin File exist - No charts can be made !"));		
					

							return TRUE;
						}

					}
					else {

							GetCell(1,row,&cell);
							Group=cell.GetStdText();
							GetCell(3,row,&cell);
							Name=cell.GetStdText();
							GetCell(0,row, &cell);
			
							Type=cell.GetStdText();;

							int pos1=Name.rfind('(');
							int pos2=Name.rfind(')');

							if (pos2>pos1&&pos2==Name.size()-1) {
								string t=Name.substr(pos1+1,pos2-pos1-1);
								Index=FUtil::AtoInt(t);
								if(Index>0)
									Name=Name.substr(0,pos1);
								else
									Index=1;
							}
							else {
								Index=1;
							}	
							
							pSw=m_pSimDoc->GetPtr(Type, Group, Name);
							if(pSw==nullptr) return FALSE;
							selected=pSw->GetStoreFlag(Index-1);
							/*if(GetTypeIndex(Type)>5 )
								((G*)pSw)->GetFlag(Index-1, &selected); 
							else
								((X*)pSw)->GetFlag(Index-1, &selected); */

						if(m_pSimDoc->m_MultiRun_Array.size()>0) {	
							CString txt;

							
							if(selected==0) 
								str->SetString(_T("Click to select as single run output variable or for multi run statitics"));	
							else if(selected==1)
								str->SetString(_T("Click to select as multi run time serie outputs variable "));
							else if(int(selected)==2)
								str->SetString(_T("Click to deselect as outputs variable "));


						}
						else {
						 if(selected==0) 
							str->SetString(_T("Click to select as single run outputs "));	
						 else if(selected==1)
							str->SetString(_T("Click to deselect as single run outputs "));
						}
						return TRUE;
					}


				
				}
				else if(col==5&&m_pSimDoc->IsFinishedSimulation()) {
					string s;
					GetCell(5,row,&cell);
					s=cell.GetStdText();
					if(s.size()>0) {
						str->SetString(_T("Single click on Chart Number to move variable to next Chart"));
						return TRUE;
					}
					return FALSE;
				}
				else if(col>6&&col<13&&m_pSimDoc->IsMultiSimulation()&&m_pSimDoc->IsFinishedSimulation()) {
					CString txt=CString(GetMultiOutId(sheet).c_str());
					if(m_pSimDoc->IsMultiStorageBinFileValid())
						str->SetString(_T("Click on grid to select/plot, "+txt+" from multiruns"));	
					else
						str->SetString(_T("No Multi Storage MBin File exist - No charts can be made !"));
					
					return TRUE;

				}
				return FALSE;
			case 6:
				if(m_ShowValidCoVarRes||m_ShowValidRes)
					return FALSE;
				else if(col==2||col==3) {
					if(m_pSimDoc->IsFinishedSimulation()) {
						if(!m_pSimDoc->IsOutputValidBinFileValid()) 
							str->SetString(_T("No Output PG Bin File exist - No charts can be made !"));		
						return TRUE;
					}
				}
				else if(col==5&&m_pSimDoc->IsFinishedSimulation()) {
					string s;
					GetCell(5,row,&cell);
					s=cell.GetStdText();
					if(s.size()>0) {
						str->SetString(_T("Single click on Chart Number to move variable to next Chart"));
						return TRUE;
					}
					return FALSE;
				}

				else if(col>6&&col<11||col==13) {
					if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiSimulation()) {
		
						if(m_pSimDoc->IsMultiStorageBinFileValid()) {
							CString txt=CString(GetMultiOutId(sheet).c_str());
							str->SetString(_T("Click on grid to select "+txt+" from multiruns"));	
						}
						else
							str->SetString(_T("No Multi Storage MBin File exist - No charts can be made !"));	
				
						return TRUE;
					}

				}

				return FALSE;
			case 7:
				return FALSE;
			case 8:
				return FALSE;
			case 9:
				if(!m_pSimDoc->IsFinishedSimulation()) {
						if(col<3)
							str->SetString(_T("Right click to see options"));
						else if(col==7)
							return FALSE;
						else if(col>2) 
							str->SetString(_T("Double click to modify values"));
						else
							return FALSE;
						return TRUE;

				}
				return FALSE;

			case 10:
		

				GetCell(col,row, &cell); 
				ParV=cell.GetParam();
				if(ParV==2000) {
					CString st1;
					NumChartStr.Format(_T("%u"), m_CurrentChart);
					if(m_pProp->m_ChartType ==CHART_TYPES::TIME_SERIE)
						st1="Time Serie";
					else if(m_pProp->m_ChartType ==CHART_TYPES::SCATTER)
						st1="Scatter type";
					else if(m_pProp->m_ChartType ==CHART_TYPES::FREQUENCE)
						st1="Frequence distribution";
					else if(m_pProp->m_ChartType ==CHART_TYPES::CUM_FREQUENCE)
						st1="Cumulative distribution";
					NumStr.Format(_T("%1u"), m_VarNo);

					if(!m_newChart)
						str->SetString(_T("Click to plot: (")+NumChartStr+_T(") as a new chart : ")+st1);
					else
						str->SetString(_T("Single Click to add to previous chart (")+NumChartStr+_T(") with (")+NumStr+_T(") variables : ")+st1);
	
				}
				else if(ParV==1000) 
					str->Format(_T("Click on yellow name fields to view/hide variables in file"));
				else if(col==2) str->SetString(_T("Right Click to see options for plotting parameter values"));
				else if (col == 7) str->SetString(_T("Right Click to see options for Ensembles "));
				else  
					str->SetString(_T("Right Click to view options for PG-files"));
	
				return TRUE;

			case 11:
				
				str->SetString(_T("Right Click to Get List of Specific Items by selection from name column"));
				return TRUE;

		}
	}
	else if(section==UG_CORNERBUTTON) {

		switch (sheet) {
			case 1:
				if(m_ShowAllSwitches)
					str->SetString(_T("Click This button to hide switches that are set to default values or Right Click to select new domain"));	
				else
					str->SetString(_T("Click This button to view all switches to be visible or Right Click to select new domain"));
				return TRUE;
			case 2:
				if(m_ShowAllParameters)
					str->SetString(_T("Click This button to hide Parameters that are set to default values- or Right Click to select new domain"));	
				else
					str->SetString(_T("Click This button to view all Parameters -or Right Click to select new domain"));	
				return TRUE;
			case 3:
				if(m_ShowAllTables)
					str->SetString(_T("Click This button to hide Parameters that are within tables or Right Click to select new domain"));	
				else
					str->SetString(_T("Click This button to view all Parameters -or Right Click to select new domain"));	
				return TRUE;
			case 4:
				return FALSE;
			case 5:
				if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->MR_Storage_Open()&&m_pSimDoc->m_SelectMultiSum.size()>0)
					str->SetString(_T("Click This button to view Selected Multi run variables as prior-posterior distributions"));	
				else if(m_pSimDoc->IsFinishedSimulation())
					str->SetString(_T("Right Click to select new catergory to view"));	
				else 
					str->SetString(_T("Single Click to View all variables that are enabled as possible outputs"));
				
				return TRUE;
			case 6:
				if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiSimulation()&&!m_ShowValidRes)
					str->SetString(_T("Click This button to view Multi run tables from all runs"));	
				else if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiSimulation())
					str->SetString(_T("Click This button to view Multi run results from current run"));		
				else
					return FALSE;

				return TRUE;
			case 7:
				return FALSE;
			case 8:
				return FALSE;
			case 9:
				return FALSE;
			case 10:
				if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiSimulation()&&!m_ShowCoVarRes&&!m_ShowMultiRes)
					str->SetString(_T("Click This button to view Multi run tables from all runs  - or Right Click to make new choice"));	
				else if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiSimulation()&&m_ShowCoVarRes&&!m_ShowMultiRes)
					str->SetString(_T("Click This button to view Multi run tables from all runs  - or Right Click to make new choice"));	
				else if (m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiSimulation()&&m_ShowMultiRes&&!m_ShowCoVarRes )
					str->SetString(_T("Click This button to view simple Multi run table  - or Right Click to make new choice"));				
				else if (m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiSimulation()&&!m_ShowCoVarRes&&m_ShowMultiRes )
					str->SetString(_T("Click This button to View co variance between multiRun parameters - or Right Click to make new choice"));				
				else if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiSimulation()&&m_ShowCoVarRes&&m_ShowMultiRes)
					str->SetString(_T("Click This button to view co variance between multiRun parameters  - or Right Click to make new choice"));	
				else

					return FALSE;
				return TRUE;
			case 11:
				str->SetString(_T("Right Click to view options for PG-files"));
				return TRUE;
			case 12:
				return FALSE;


		}

	}
	else if(section==UG_TOPHEADING) {

		switch (sheet) {
			case 0:
				if(m_pDoc==nullptr) 
					return FALSE;
				if(col==0&&!m_pSimDoc->IsFinishedSimulation())
					str->SetString(_T("Click To make a single run of model"));	
				else if(col==1&&!m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->m_MultiRun_Array.size()>0)
					str->SetString(_T("Click to start of sequence of multi run simulations"));
				else if(col==2&&m_pSimDoc->IsFinishedSimulation()) {
					//if(!ReadyToSimulate)
						str->SetString(_T("Click to make a copy of simulation"));
					//else
					//	str->SetString(_T("Another document is prepared for simulation - no copy of document can currently be made"));
				}
				else if(col==2&&!m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->m_MultiRun_Array.size()>0&&m_pSimDoc->MR_Get_ActualRunNumber()<m_pSimDoc->MR_Get_TotalNumberofRuns())
					str->SetString(_T("Click to ReStart a sequence of multi run run simulation"));
				else if(col==2)
					str->SetString(_T("Click to Extend prevous multi run simulation to a new multirun"));
				else
					return FALSE;
				break;
			case 1:
				if(col==0)
					str->SetString(_T("Click This button Sort all Items by Modules"));	
				else if(col==1) {
					if (m_ShowProcesses)
						str->SetString(_T("Click to Sort\nRight click to Selected elements to view"));
					else
						str->SetString(_T("Click to Sort\nRight click to show Processes or to make selection"));
				}
				else if(col==2)
					str->SetString(_T("Click This button Sort all Items by Switch Name"));
				else
					return FALSE;
				break;
			case 2:
				if(col==0)
					str->SetString(_T("Click This button Sort all Parameters by Modules"));	
				else if(col==1) {
					if (m_ShowProcesses)
						str->SetString(_T("Click to Sort\nRight click to Selected elements to view"));
					else
						str->SetString(_T("Click to Sort\nRight click to show Processes or to make selection"));

				}
				else if(col==2)
					str->SetString(_T("Click This button Sort all Parameters by Parameter Names"));	
				else
					return FALSE;
				break;
			case 3:
				if(col==0)
					str->SetString(_T("Click This button Sort all Tables by Modules"));	
				else if(col==1) {
					if (m_ShowProcesses)
						str->SetString(_T("Click to Sort\nRight click to Selected elements to view"));
					else
						str->SetString(_T("Click to Sort\nRight click to show Processes or to make selection"));
				}
				else if(col==2)
					str->SetString(_T("Click This button Sort all Tables by Table Names"));	
				else
					return FALSE;
				break;
			case 4:
				return FALSE;
			case 5:
				if (col == 0)
					str->SetString(_T("Click to Sort all Output Variables by Variable Type"));
				else if (col == 1)
					str->SetString(_T("Click to Sort all Output Variables by Modules"));
				else if (col == 2) {
					if (m_ShowProcesses)
						str->SetString(_T("Click to Sort\nRight click to Selected elements to view"));
					else
						str->SetString(_T("Click to Sort\nRight click to show Processes or to make selection"));
				}
				else if (col == 3)
					str->SetString(_T("Click to Sort all Output Variables by Name\nRight click to select Variables to View"));
				return TRUE;
			case 6:
				if (col == 4 && row == -1) {
					if (m_pSimDoc->IsFinishedSimulation() && m_pSimDoc->IsMultiSimulation() && m_pSimDoc->m_MStorage.m_AcceptedIdString.size() > 0) {
						str->SetString(_T("Click on header to increment to the next ensemble"));
						return TRUE;
					}
				}
				else if (col == 3 && row == -1) {
					if (m_pSimDoc->IsFinishedSimulation()) {
						str->SetString(_T("Click on header to shift between Simulated/Residuals/Log Likelihood "));
						return TRUE;
					}
				}
				return FALSE;
			case 7:
				return FALSE;
			case 8:
				return FALSE;
			case 9:
				return FALSE;
			case 10:
				return FALSE;
			case 11:
				str->SetString(_T("Right Click to view options for History list"));
				return TRUE;
		}
		return TRUE;
	}


	else if(section==UG_SIDEHEADING) {
		switch (sheet) {
			case 0:
					return FALSE;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 8:
			case 9:
			case 10:
			case 12:
				str->SetString(_T("Right Click to see options"));	
				break;
			case 7:
			case 11:
				return FALSE;
		}
		



	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnVScrollHint
//		Is called when the hint is about to be displayed on the vertical scroll bar,
//		here you have a chance to set the text that will be shown
//	Params:
//		row		- current top row
//		str	- pointer to the str that will be shown
//	Return:
//		TRUE - to show the hint
//		FALSE - to prevent the hint from showing
int MyCug::OnVScrollHint(long row,CString *str)
{
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*str);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHScrollHint
//		Is called whent the hint is about to be displayed on the horizontal scroll bar,
//		here you have a chance to set the text that will be shown
//	Params:
//		col		- current left col
//		str	- pointer to the str that will be shown
//	Return:
//		TRUE - to show the hint
//		FALSE - to prevent the hint from showing
int MyCug::OnHScrollHint(int col,CString *str)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(*str);
	return TRUE;
}

#ifdef __AFXOLE_H__
/////////////////////////////////////////////////////////////////////////////
//	OLE
//		following 3 functions are to be used with drag and drop functionality
/////////////////////////////////////////////////////////////////////////////
//	OnDragEnter
//	Params:
//		pDataObject - 
//	Return:
//		DROPEFFECT_NONE - no drag and drop
//		DROPEFFECT_COPY - allow drag and drop for copying
DROPEFFECT MyCug::OnDragEnter(COleDataObject* pDataObject)
{
	UNREFERENCED_PARAMETER(*pDataObject);
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	OnDragOver
//	Params:
//		col, row	-
//		pDataObject - 
//	Return:
//		DROPEFFECT_NONE - no drag and drop
//		DROPEFFECT_COPY - allow drag and drop for copying
DROPEFFECT MyCug::OnDragOver(COleDataObject* pDataObject,int col,long row)
{
	UNREFERENCED_PARAMETER(*pDataObject);
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	OnDragDrop
//	Params:
//		col, row	-
//		pDataObject - 
//	Return:
//		DROPEFFECT_NONE - no drag and drop
//		DROPEFFECT_COPY - allow drag and drop for copying
DROPEFFECT MyCug::OnDragDrop(COleDataObject* pDataObject,int col,long row)
{
	UNREFERENCED_PARAMETER(*pDataObject);
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	return DROPEFFECT_COPY;
}
#endif

/////////////////////////////////////////////////////////////////////////////
//	OnScreenDCSetup
//		Is called when each of the components are painted.
//	Params:
//		dc		- pointer to the current CDC object
//		section	- section of the grid painted.
//					possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//						UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
//	Return:
//		<none>
void MyCug::OnScreenDCSetup(CDC *dc,int section)
{
	UNREFERENCED_PARAMETER(*dc);
	UNREFERENCED_PARAMETER(section);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSortEvaluate
//		Sent as a result of the 'SortBy' call, this is called for each cell
//		comparison and allows for customization of the sorting routines.
//		We provide following code as example of what could be done here,
//		you might have to modify it to give your application customized sorting.
//	Params:
//		cell1, cell2	- pointers to cells that are compared
//		flags			- identifies sort direction
//	Return:
//		value less than zero to identify that the cell1 comes before cell2
//		value equal to zero to identify that the cell1 and cell2 are equal
//		value greater than zero to identify that the cell1 comes after cell2
int MyCug::OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags)
{
	// if one of the cells is nullptr, do not compare its text
	if ( cell1 == nullptr && cell2 == nullptr )
		return 0;
	else if ( cell1 == nullptr )
		return 1;
	else if ( cell2 == nullptr )
		return -1;

	if(flags&UG_SORT_DESCENDING)
	{
		CUGCell *ptr = cell1;
		cell1 = cell2;
		cell2 = ptr;
	}

	// initialize variables for numeric check
	double num1, num2;
	// compare the cells, with respect to the cell's datatype
	switch(cell1->GetDataType())
	{
		case UGCELLDATA_STRING:
			if(""== cell1->GetStdText() && ""== cell2->GetStdText())
				return 0;
			if("" == cell1->GetStdText())
				return 1;
			if("" == cell2->GetStdText())
				return -1;
			return _tcscmp(cell1->GetText(),cell2->GetText());	
		case UGCELLDATA_BOOL:
			if("" == cell1->GetStdLabelText() && "" == cell2->GetStdLabelText())
				return 0;
			if("" == cell1->GetStdLabelText())
				return 1;
			if("" == cell2->GetStdLabelText())
				return -1;
			return _tcscmp(cell1->GetLabelText(),cell2->GetLabelText());	

		case UGCELLDATA_NUMBER:
		case UGCELLDATA_CURRENCY:
			num1 = cell1->GetNumber();
			num2 = cell2->GetNumber();
			if(num1 < num2)
				return -1;
			if(num1 > num2)
				return 1;
			return 0;
		default:
			// if datatype is not recognized, compare cell's text
			if("" == cell1->GetStdText())
				return 1;
			if("" == cell2->GetStdText())
				return -1;
			return _tcscmp(cell1->GetText(),cell2->GetText());	
	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnTabSelected
//		Called when the user selects one of the tabs on the bottom of the screen
//	Params:
//		ID	- id of selected tab
//	Return:
//		<none>

void MyCug::SelectTab(int ID)
{
	SetCurrentTab(ID);
}
void MyCug::OnTabSelected(int ID)
{
	UNREFERENCED_PARAMETER(ID);
	int Iv=GetSheetNumber();
	UpdateStatusBar(ID);
	if(ID==10)
		m_bDrawExcelFocus=TRUE;
	else
		m_bDrawExcelFocus=FALSE;
	if(ID==12) {
		SetSheetNumber(12);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(12);
	}
	else if(ID==0) {
		SetSheetNumber(0);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 

		OnSheetSetup(0);
	}
	else if(ID==1) {
		SetSheetNumber(1);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 

		OnSheetSetup(1);
	}
	else if(ID==2) {
		SetSheetNumber(2);
		long topr=GetTopRow();
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(2);
		if(m_ClickedRow>=0)
			SetTopRow(m_ClickedRow);

	}
	else if(ID==3) {
		SetSheetNumber(3);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(3);
	}
	else if(ID==4) {
		SetSheetNumber(4);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(4);
	}
	else if(ID==5) {
		SetSheetNumber(5);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(5);
		if(m_ClickedRow>=0)
			//GotoRow(m_ClickedRow);
			SetTopRow(m_ClickedRow);
	}
	else if(ID==6) {
		SetSheetNumber(6);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(6);
		m_pSimDoc->SetCurrentSheet(6);
	}
	else if(ID==7) {
		SetSheetNumber(7);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(7);
	}
	else if(ID==8) {
		SetSheetNumber(8);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(8);
	}
	else if(ID==9) {
		SetSheetNumber(9);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(9);
	}
	else if(ID==10) {
		SetSheetNumber(10);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(10);
	}
	else if(ID==11) {
		SetSheetNumber(11);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(11);
	}
	if(ID==12) {
		SetSheetNumber(12);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(12);
	}
	if (ID == 13) {
		SetSheetNumber(13);
		EnableUpdate(FALSE);
		while (GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE);
		OnSheetSetup(13);
	}
	else
		return;
	SetSheetNumber(ID) ; 
}

/////////////////////////////////////////////////////////////////////////////
//	OnAdjustComponentSizes
//		Called when the grid components are being created,
//		sized, and arranged.  This notification provides
//		us with ability to further customize the way
//		the grid will be presented to the user.
//	Params:
//		grid, topHdg, sideHdg, cnrBtn, vScroll, hScroll, 
//		tabs	- sizes and location of each of the grid components
//	Return:
//		<none>
void MyCug::OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,
		RECT *cnrBtn,RECT *vScroll,RECT *hScroll,RECT *tabs)
{
	UNREFERENCED_PARAMETER(*grid);
	UNREFERENCED_PARAMETER(*topHdg);
	UNREFERENCED_PARAMETER(*sideHdg);
	UNREFERENCED_PARAMETER(*cnrBtn);
	UNREFERENCED_PARAMETER(*vScroll);
	UNREFERENCED_PARAMETER(*hScroll);
	UNREFERENCED_PARAMETER(*tabs);
} 

/////////////////////////////////////////////////////////////////////////////
//	OnDrawFocusRect
//		Called when the focus rect needs to be painted.
//	Params:
//		dc		- pointer to the current CDC object
//		rect	- rect of the cell that requires the focus rect
//	Return:
//		<none>
/*void MyCug::OnDrawFocusRect(CDC *dc,RECT *rect)
{
	//DrawExcelFocusRect(dc,rect);
	
	rect->bottom --;
	rect->right --;	
	dc->DrawFocusRect(rect);
}*/

/////////////////////////////////////////////////////////////////////////////
//	OnGetDefBackColor
//		Sent when the area behind the grid needs to be painted.
//	Params:
//		section - Id of the grid section that requested this color
//				  possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING, UG_GRID
//	Return:
//		RGB value representing the color of choice
COLORREF MyCug::OnGetDefBackColor(int section)
{
	UNREFERENCED_PARAMETER(section);
	return RGB(255,255,255);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetFocus
//		Sent when the grid is gaining focus.
//	Note:
//		The grid will loose focus when the edit is started, or drop list shown
//	Params:
//		section - Id of the grid section gaining focus, usually UG_GRID
//				  possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING, UG_GRID
//	Return:
//		<none>
//
void MyCug::OnSetFocus(int section)
{
	UNREFERENCED_PARAMETER(section);
}

/////////////////////////////////////////////////////////////////////////////
//	OnKillFocus
//		Sent when the grid is loosing focus.
//	Params:
//		section - Id of the grid section loosing focus, usually UG_GRID
//				  possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING, UG_GRID
//		pNewWnd	- pointer to the window that is gaining focus
//	Return:
//		<none>
void MyCug::OnKillFocus(int section, CWnd *pNewWnd)
{
	UNREFERENCED_PARAMETER(section);
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapStart
//		Called to inform the grid that the col swap was started on given col.
//	Params:
//		col - identifies the column
//	Return:
//		TRUE - to allow for the swap to take place
//		FALSE - to disallow the swap
BOOL MyCug::OnColSwapStart(int col)
{
	UNREFERENCED_PARAMETER(col);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanColSwap
//		Called when col swap is about to take place, here you can 'lock' certain
//		cols in place so that are not swappable.
//	Params:
//		fromCol - where the col originated from
//		toCol	- where the col will be located if the swap is allowed
//	Return:
//		TRUE - to allow for the swap to take place
//		FALSE - to disallow the swap
BOOL MyCug::OnCanColSwap(int fromCol,int toCol)
{
	UNREFERENCED_PARAMETER(fromCol);
	UNREFERENCED_PARAMETER(toCol);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapped
//		Called just after column-swap operation was completed.
//	Params:
//		fromCol - where the col originated from
//		toCol	- where the col will be located if the swap is allowed
//	Return:
//		<none>
void MyCug::OnColSwapped(int fromCol,int toCol)
{
	UNREFERENCED_PARAMETER(fromCol);
	UNREFERENCED_PARAMETER(toCol);
}

/////////////////////////////////////////////////////////////////////////////
//	OnTrackingWindowMoved
//		Called to notify the grid that the tracking window was moved
//	Params:
//		origRect	- from
//		newRect		- to location and size of the track window
//	Return:
//		<none>




void MyCug::OnTrackingWindowMoved(RECT *origRect,RECT *newRect)
{
	UNREFERENCED_PARAMETER(*origRect);
	UNREFERENCED_PARAMETER(*newRect);
}
int MyCug::GetMultiMethod(string str)
{
	int index;
	string optionlist;
	optionlist="Linear\nLogarithmic\nStochastic Linear\nStochastic Log\n";
	optionlist+="Table\nSame As\nRelative Change As\nRelative Opposite Change As\n";
	if(m_pSimDoc->MR_UseBayesianCoVar)
		optionlist+="Apply Bayesian results\n";
	else
		optionlist+="Bayesian Calibration\n";

	optionlist+="Connected\n";
	if(!m_pSimDoc->MR_UseBayesianCoVar)
		optionlist+="Bayesian Calibration using log distribution\n";
	int ipos, ipos_end;
	ipos_end=optionlist.find(str);
	
	optionlist=optionlist.substr(0,ipos_end);
	ipos=optionlist.find("\n");
	index=0;
	while(ipos>0) {
		optionlist=optionlist.substr(ipos+1);
		ipos=optionlist.find("\n");
		index++;
	}
	if(index>4)
		index++;
	return index;

}
int MyCug::GetMultiDimensionIndex(string str)
{
	int index;
	string optionlist;
	int icounto=0;	
	optionlist="";
	for(int i=0;i<m_pSimDoc->MR_GetNumberOfDimensions();i++) {
			optionlist+=FUtil::STD_ItoAscii(i+1)+"\n";
	}

	int ipos, ipos_end;
	ipos_end=optionlist.find(str);
	
	optionlist=optionlist.substr(0,ipos_end);
	ipos=optionlist.find("\n");
	index=0;
	while(ipos>0) {
		optionlist=optionlist.substr(ipos+1);
		ipos=optionlist.find("\n");
		index++;
	}
	return index+1;

}
int MyCug::GetOptionIndex(string optionlist, string str)
{
	int ipos_end, index;
	ipos_end=optionlist.find(str);	
	optionlist=optionlist.substr(0,ipos_end);
	int ipos=optionlist.find("\n");
	index=-1;
	while(ipos>0) {
		optionlist=optionlist.substr(ipos+1);
		ipos=optionlist.find("\n");
		index++;
	}
	return index;

}
int MyCug::GetMultiDependenceIndex(string str)
{
	int index;
	string optionlist;
	int icounto=0;	
	optionlist="";
	for(int ix=0;ix<m_pSimDoc->m_MultiRun_Array.size();ix++){
	   			for(int iix=0; iix<m_pSimDoc->MR_Get_NumberOfParametersWithinDim(ix+1);iix++){
								icounto++;
								string name;
								name=m_pSimDoc->MR_GetParName(ix+1,iix+1).c_str();
								if(m_pSimDoc->MR_GetTabIndex(ix+1,iix+1)!=string::npos) {
									name+="("+FUtil::STD_ItoAscii(m_pSimDoc->MR_GetTabIndex(ix+1,iix+1)+1)+")";
								}

								optionlist+=name+"\n";

					}
								
	}

	int ipos, ipos_end;
	ipos_end=optionlist.find(str);
	
	optionlist=optionlist.substr(0,ipos_end);
	ipos=optionlist.find("\n");
	index=0;
	while(ipos>0) {
		optionlist=optionlist.substr(ipos+1);
		ipos=optionlist.find("\n");
		index++;
	}
	return index;

}


int MyCug::GetSV_Valoption(string str)
{
	int index;
	string optionlist;
	optionlist="Initial\nFinal\nMinimum\nMaximum\nMean\nAccumulated\n";

	int ipos, ipos_end;
	ipos_end=optionlist.find(str);
	
	optionlist=optionlist.substr(0,ipos_end);
	ipos=optionlist.find("\n");
	index=0;
	while(ipos>0) {
		optionlist=optionlist.substr(ipos+1);
		ipos=optionlist.find("\n");
		index++;
	}
	return index;

}

void MyCug::UpdateRealTimeChart()
{
		if(m_PrevMainFrame!=nullptr) {
			MySingle::instance().GetChartCugPointer()->m_RealTime.ResetData();
		}
}

string  MyCug::GetMultiOutId(int sheet)
{
		int all, current;
		current=1;
		if(sheet==5) 
			current=m_pSimDoc->m_SelectMultiSum.size()+1;
		if(sheet==6) 
			current=m_pSimDoc->m_SelectMultiVal.size()+1;
		else if(sheet==10) 
			current=m_pSimDoc->m_SelectMultiPar.size()+1;
		all=m_pProp->m_NumChartX*m_pProp->m_NumChartY;
		return FUtil::STD_ItoAscii(current)+" ("+FUtil::STD_ItoAscii(all)+")";
}

void MyCug::UpdateSheet(int sheet) {

	SetSheetNumber(sheet);
	long topr=GetTopRow();
	EnableUpdate(FALSE); 
	//int ncol=GetNumberCols();
	while(GetNumberCols()>0)
		DeleteCol(0);
	while(GetNumberRows()>0) {		
			DeleteRow(0);
	}
	OnSheetSetup(0);
	while(GetNumberRows()>0)
			DeleteRow(0);
	EnableUpdate(TRUE); 
	OnSheetSetup(0);
	OnSheetSetup(sheet);
	if(m_ClickedRow>=0) {
		SetTopRow(m_ClickedRow);
	}
	else
		SetTopRow(topr);
	SetCurrentTab(sheet);

}
void MyCug::FocusReportSheet() 
{
	OnSheetSetup(7);


}

void MyCug::UpdateStatusBar(int isheet)
{

	string text;
	switch(isheet) {
	case 0:
		text="General Run Info Sheet";
		break;
	case 1:
		text="Your choice of model structure";
		if(m_ShowAllSwitches)
			text+=" (coloured items are modified)";
		else
			text+=" - Only those that are different from default values";	
		break;
	case 2:
		text="";
		if(m_ShowAllParameters)
			text+="All Parameters (coloured items are modified)";
		else
			text+="Parameters that are different from default values";	
		break;
	case 3:
		if(m_ShowAllTables)
			text="Parameters as vectors within tables";
		else
			text="Parameter tables";
		break;
	case 4:
		text="Model input file";
		break;
	case 5:
		break;
	case 6:
		text = "Validation Informaton Sheet";
		break;
	case 10:
		text="MultiRun Sheet";
		break;

	}
	if(m_pSimDoc->IsFinishedSimulation())  {
		text+=" Completed ";
		if(m_pSimDoc->IsMultiSimulation())
			text+="Multi Simulation";
		else
			text+="Single Simulation";
	}
	else
		text+=" for design of new simulation";

	if(text.size()>0) {
	 m_pFrame->UpdatePaneText(0, text);  
	
	}

	Invalidate();

}
void MyCug::UpdateStatusBar(CString text)
{

    m_pFrame->UpdatePaneText(0, CT2A(text).m_psz);  
	Invalidate();

}
void MyCug::UpdateStatusBar(SimB *nod, int itype) {
	if (nod==nullptr) return;

    string text;
	string kalle, lotta;
	double value;
	float fvalue;
	Ps* p_par;
	P* pt_par;
	size_t j;
	int ivalue;


    TRACE("CPPage1::UpdateStatusBar continued\n");

	switch (itype) {

	case 1:	
	  TRACE("CPPage1::UpdateStatusBar continued (1) in Ps (if)\n");
	  p_par=(Ps*)nod;
  	  value=p_par->GetMinValue();
	  text = MFC_Util::StdFtoAsciiPane(value);
	  m_pFrame->UpdatePaneText(1, text);  

      value=p_par->GetWarnMinValue();
	  text = MFC_Util::StdFtoAsciiPane(value);
	  m_pFrame->UpdatePaneText(2, text);
      value=p_par->GetWarnMaxValue();
	  text = MFC_Util::StdFtoAsciiPane(value);
	  m_pFrame->UpdatePaneText(3, text);   
      value=p_par->GetMaxValue();
	  text = MFC_Util::StdFtoAsciiPane(value);
	  m_pFrame->UpdatePaneText(4, text); 
	  text = nod->GetUnit().c_str();
	  m_pFrame->UpdatePaneText(5, text);

	  kalle=nod->GetName().c_str();
	  m_pFrame->UpdatePaneText(6,kalle);
	  j=kalle.find(" ");
	  while (j!=string::npos) {
		lotta+=kalle.substr(0,j);
	    kalle=kalle.substr(j+1);
		j=kalle.find(" ");
	  }
	  lotta+=kalle;
	  if(lotta.find(",")!=string::npos)
			lotta.replace(lotta.find(","),1,".");
	  m_pSimDoc->m_HelpName=lotta;
	  break;
	case 2:

		pt_par=(P*)nod;
		kalle=pt_par->GetName().c_str();
		//text = MFC_Util::FtoAsciiPane(value);
		 m_pFrame->UpdatePaneText(0, kalle);  
      // repeat for four other panes
		//p_par->GetWarnMinValue(value);
	  //text = MFC_Util::FtoAsciiPane(value);
	 
		break;
	case 3:
    
	 TRACE("CPPage1::UpdateStatusBar continued in CE-Tab (else)\n");

      fvalue=nod->GetMinValue();
	  ivalue=int(fvalue);
	  if (ivalue != 0) text = FUtil::STD_ItoAscii(ivalue);
      else if ((ivalue > 1.E5) || (ivalue < -1.E5)) text = "large";
	  else text = "---"; // update first CString variable
      m_pFrame->UpdatePaneText(1, text);
	  // repeat for three other panes
      fvalue=nod->GetWarnMinValue();
	  ivalue=int(fvalue);
	  if (ivalue != 0) text = "---";
	  else text = FUtil::STD_ItoAscii(ivalue);
      // update second CString variable
      m_pFrame->UpdatePaneText(2, text);
	  fvalue=nod->GetWarnMaxValue();
	  ivalue=int(fvalue);
      if ((ivalue != 0) ) //|| (ivalue > (nod->GetValue()))) 
		  text = FUtil::STD_ItoAscii(ivalue);
      else text = "---"; // update third CString variable
      m_pFrame->UpdatePaneText(3, text);   
      fvalue=nod->GetMaxValue();
	  ivalue=int(fvalue);
	  if (ivalue != 0) text = FUtil::STD_ItoAscii(ivalue);
      else if ((ivalue > 1.E5) || (ivalue < -1.E5)) text = "large";
	  else text = "---"; // update fourth CString variable
      m_pFrame->UpdatePaneText(4, text);
	  text = nod->GetUnit().c_str();
	  m_pFrame->UpdatePaneText(5, text);
	  kalle=nod->GetName().c_str();
	  j=kalle.find(" ");
	  while (j!=string::npos) {
		lotta+=kalle.substr(0,j);
	    kalle=kalle.substr(j+1);
		j=kalle.find(" ");
	  }
	  lotta+=kalle;
	  if(lotta.find(",")!=string::npos)
		lotta.replace(lotta.find(","),1,".");
	  m_pSimDoc->m_HelpName=lotta;

	break;

	default:
	  kalle=nod->GetName().c_str();
	  lotta="";
	  j=kalle.find(" ");
	  while (j!=string::npos) {
		lotta+=kalle.substr(0,j);
	    kalle=kalle.substr(j+1);
		j=kalle.find(" ");
	  }
	  lotta+=kalle;
	  if(lotta.find(",")!=string::npos)
		lotta.replace(lotta.find(","),1,".");
	  m_pSimDoc->m_HelpName=lotta;



  };

}


/////////////////////////////////////////////////////////////////////////////
//	OnDrawFocusRect
void MyCug::OnDrawFocusRect(CDC *dc,RECT *rect)
{
	//DrawExcelFocusRect(dc,rect);
	
	if ( !m_bDrawExcelFocus )
	{
		rect->bottom --;
		rect->right --;
		dc->DrawFocusRect(rect);
	}
	else
	{
		rect->bottom --;
		rect->right --;
		DrawExcelFocusRect( dc, rect);
	}
}

void MyCug::DrawExcelFocusRect(CDC *dc,RECT *rect)
{
	BOOL innerRectOnly = FALSE;
	BOOL hideTop = FALSE;
	BOOL hideLeft = FALSE;

	CPen pen,wPen;

	int dcID = dc->SaveDC();

	//find the rect to draw - may be a range of cells
	//if multi-select is on
	if(m_GI->m_multiSelectFlag&3)
	{
		if(m_GI->m_multiSelect->GetNumberBlocks() < 2)
		{
			int startCol,endCol;
			long startRow,endRow;
			m_GI->m_multiSelect->GetCurrentBlock(&startCol,&startRow,&endCol,&endRow);

			if( startRow < m_GI->m_topRow && endRow < m_GI->m_topRow )
				return;

			if( startRow > m_GI->m_bottomRow && endRow > m_GI->m_bottomRow )
				return;

			if( startCol < m_GI->m_leftCol && endCol < m_GI->m_leftCol )
				return;

			if( startCol > m_GI->m_rightCol && endCol > m_GI->m_rightCol )
				return;
			
			if(startCol != endCol || startRow != endRow)
			{
				GetRangeRect(startCol,startRow,endCol,endRow,rect);
			}
			if(startCol >= m_GI->m_numLockCols && startCol < m_GI->m_leftCol)
			{
				//hide the left
				hideLeft = TRUE;
			}
			if(startRow >= m_GI->m_numLockRows && startRow < m_GI->m_topRow)
			{
				//hide the top
				hideTop = TRUE;
			}
			if(rect->bottom <= rect->top)
				GetRangeRect(startCol,startRow,endCol,endRow,rect);
			if(rect->right <= rect->left)
				GetRangeRect(startCol,startRow,endCol,endRow,rect);
			
			//add draw hints for the grid so it will clear up the
			//focus rect next time
			m_CUGGrid->m_drawHint.AddHint(startCol-1,startRow-1,endCol+1,endRow+1);
		}
		else if(m_GI->m_multiSelect->GetNumberBlocks() >= 2)
		{
			innerRectOnly = TRUE;
		}
	}
	else
	{
		//add draw hints for the grid so it will clear up the
		//focus rect next time
		m_CUGGrid->m_drawHint.AddHint(m_GI->m_currentCol-1,m_GI->m_currentRow-1,
			m_GI->m_currentCol+1,m_GI->m_currentRow+1);
	}

	int top = rect->top;
	int left = rect->left;


	//inner black rect
	dc->SelectObject(GetStockObject(BLACK_PEN));
	if(rect->top == 0)
		rect->top ++;
	if(rect->left == 0)
		rect->left ++;
	if(!hideTop)
	{
		dc->MoveTo(rect->left,rect->top);
		dc->LineTo(rect->right-1,rect->top);
	}
	else
	{
		dc->MoveTo(rect->right-1,rect->top);
	}
	dc->LineTo(rect->right-1,rect->bottom-1);
	dc->LineTo(rect->left,rect->bottom-1);
	if(!hideLeft)
	{
		dc->LineTo(rect->left,rect->top-1);
	}
	rect->top = top;
	rect->left = left;


	//inner white rect
	dc->SelectObject(GetStockObject(WHITE_PEN));
	if(rect->top == 0)
		rect->top ++;
	if(rect->left == 0)
		rect->left ++;
	if(!hideTop)
	{
		dc->MoveTo(rect->left+1,rect->top+1);
		dc->LineTo(rect->right-2,rect->top+1);
	}
	else
	{
		dc->MoveTo(rect->right-2,rect->top+1);
	}
	dc->LineTo(rect->right-2,rect->bottom-2);
	dc->LineTo(rect->left+1,rect->bottom-2);
	if(!hideLeft)
	{
		dc->LineTo(rect->left+1,rect->top);
	}
	rect->top = top;
	rect->left = left;

	//draw the rest if innerRectOnly is false
	if(!innerRectOnly)
	{
		dc->SelectObject(GetStockObject(BLACK_PEN));

		//outer rect
		if(rect->top == 0)
			rect->top +=2;
		if(rect->left == 0)
			rect->left +=2;
		if(hideLeft)
			rect->left++;
		if(!hideTop)
		{
			dc->MoveTo(rect->left-2,rect->top-2);
			dc->LineTo(rect->right+1,rect->top-2);			
		}
		else
		{
			rect->top++;
			dc->MoveTo(rect->right+1,rect->top-2);
		}
		
		dc->LineTo(rect->right+1,rect->bottom+1);
		dc->LineTo(rect->left-2,rect->bottom+1);
		
		if(!hideLeft)
		{
			dc->LineTo(rect->left-2,rect->top -3);
		}

		rect->top = top;
		rect->left = left;


		//bottom right square
		dc->MoveTo(rect->right-2,rect->bottom-2);
		dc->LineTo(rect->right,rect->bottom-2);
		dc->MoveTo(rect->right+2,rect->bottom-2);
		dc->LineTo(rect->right+2,rect->bottom+1);
		dc->MoveTo(rect->right+2,rect->bottom+1);
		dc->LineTo(rect->right+2,rect->bottom+2);
		dc->LineTo(rect->right,rect->bottom+2);
		dc->MoveTo(rect->right-2,rect->bottom+2);
		dc->LineTo(rect->right,rect->bottom+2);

		wPen.CreatePen(PS_SOLID,1,RGB(255,255,255));
		dc->SelectObject(wPen);
		dc->MoveTo(rect->right+3,rect->bottom-3);
		dc->LineTo(rect->right-3,rect->bottom-3);
		dc->LineTo(rect->right-3,rect->bottom+3);
	

		pen.CreatePen(PS_SOLID,1,RGB(120,120,120));
		dc->SelectObject(pen);


		//dark gray middle rect
		if(rect->top >0 && !hideTop)
		{
			dc->MoveTo(rect->left-2,rect->top-1);
			dc->LineTo(rect->right+2,rect->top-1);
		}
		else
			rect->top +=2;

		dc->MoveTo(rect->right,rect->top-1);
		dc->LineTo(rect->right,rect->bottom+3);
		if(rect->left >0 && !hideLeft)
		{
			dc->MoveTo(rect->left-1,rect->bottom+1);
			dc->LineTo(rect->left-1,rect->top-2);
		}
		else
			rect->left +=3;
		dc->MoveTo(rect->right+2,rect->bottom);
		dc->LineTo(rect->left-3,rect->bottom);

	}

	rect->top = top;
	rect->left = left;

	dc->RestoreDC(dcID);

	pen.DeleteObject();
	wPen.DeleteObject();
}
BOOL MyCug::ResetAcceptance()
{

	return TRUE;
}

void MyCug::SetViewCategory(int value)
{
	if(value>0&&value<9) 
		 m_ViewCategory=value;
	else
		m_ViewCategory=0;
}
void MyCug::SetShowAllOutputs(BOOL value)
{
	if(value)
		m_ShowAllOutputs=TRUE;
	else
		m_ShowAllOutputs=FALSE;
}
void MyCug::UpdateSelectedVal(int row, int indicator)
{
	CUGCell cell;
	int ValFileNo=1;
	if(m_pSimDoc->IsFinishedSimulation()) {
		int current=GetSheetNumber();
		if(current!=6)
		SelectTab(6);
		if(indicator==-1||indicator==15||indicator==25) {
			GetCell(1,row,&cell);
			//if(!cell.GetBool()){
				int col, irow;
				size_t koll, count=0, deleted=0;
				koll=m_pSimDoc->m_SelectValTime.size();
				vector<SelectVAL>::iterator it;
				if(m_CurrentSubChart>=m_pProp->m_NumSubCharts) {
					while(count<koll) {

						col=m_pSimDoc->m_SelectValTime[count-deleted].col;
						irow=m_pSimDoc->m_SelectValTime[count-deleted].row;
						GetCell(col,irow,&cell);
						cell.SetBool(FALSE);
						SetCell(col,irow,&cell);
						RedrawCell(col,irow);
						QuickSetText(5,irow,"");
						if(col==1||irow!=row) {
							it=m_pSimDoc->m_SelectValTime.begin();
							it+=count-deleted;
							m_pSimDoc->m_SelectValTime.erase(it);
							deleted++;
						}
						count++;
					}
				}
					SelectVAL val;
					if(m_pSimDoc->m_SelectValTime.size()==0)
							m_CurrentSubChart=1;
					else if(m_CurrentSubChart<m_pProp->m_NumSubCharts)
							m_CurrentSubChart++;
					if(indicator==-1) {
						GetCell(3,row,&cell);
						cell.SetBool(TRUE);
						SetCell(3,row,&cell);	
						RedrawCell(3,row);
						val.col=3;
						col=3;
						val.MeanIndicator= ENSEMBLE_TYPE::NO;
					}
					else {
						GetCell(4,row,&cell);
						cell.SetBool(TRUE);
						if(indicator==15) {
							cell.SetLabelText("All");
							val.MeanIndicator= ENSEMBLE_TYPE::ALL;
						}
						else {
							cell.SetLabelText("Accepted");
							val.MeanIndicator= ENSEMBLE_TYPE::ACCEPTED;
						};
						SetCell(4,row,&cell);	
						RedrawCell(4,row);

						val.col=4;
						col=4;

					}
					val.row=row;

					val.RunNo=0;
					val.subchart=m_CurrentSubChart;
					m_pSimDoc->m_SelectValTime.push_back(val);
					GetCell(1,row,&cell);
	
					cell.SetBool(TRUE);
					SetCell(1,row,&cell);
					RedrawCell(1,row);
					val.col=1;
					val.MeanIndicator= ENSEMBLE_TYPE::NO;
					m_pSimDoc->m_SelectValTime.push_back(val);

					QuickSetNumber(5,row,m_CurrentSubChart);
					RedrawCell(5,row);
	
					//if(m_SelectedValRow.GetSize()>0)
						//TimeSerieChartClick(1, row, 6, nullptr, 0, VariableType, "", ValFileNo);

					
					if(m_pSimDoc->m_SelectValTime.size()>0) {
						SimB *pS=nullptr; 
						TimeSerieChartClick(4, row, 6, pS, 0, 0, "", ValFileNo);
					}



		}
		else {
//				When selected performance indicators
			int nselV=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
			int col, irow;
			if(m_CurrentSubChart>=m_pProp->m_NumSubCharts) {
				while(m_pSimDoc->m_SelectMultiVal.size()>0) {
					col=m_pSimDoc->m_SelectMultiVal[0].col;
					irow=m_pSimDoc->m_SelectMultiVal[0].row;
					GetCell(col,irow,&cell);
					cell.SetBool(FALSE);
					SetCell(col,irow,&cell);
					m_pSimDoc->m_SelectMultiVal.erase(m_pSimDoc->m_SelectMultiVal.begin());
				}
			}

			if(m_pSimDoc->m_SelectMultiVal.size()==0)
					m_CurrentSubChart=1;
			else if(m_CurrentSubChart<m_pProp->m_NumSubCharts)
					m_CurrentSubChart++;
			col=indicator+7;
			if(indicator>4) col+=2;


			GetCell(col,row,&cell);
			cell.SetBool(TRUE);
			SetCell(col,row,&cell);	
			RedrawCell(col,row);
			MultiPlotClick(6,col, row, true);
		}
	}
	else {
		SelectTab(6);

	}


}
void MyCug::UpdateSelected(SimB* pBase, string type,string group, int localindex, P* pCPt, string SumTyp, bool SelectAll)
{
	m_pSel=pBase;
	m_SelType=type;
	m_SelectAllIndex=SelectAll;
	if(group=="ALL")
		m_SelGroup="";
	else
		m_SelGroup=group;
	if(pBase!=nullptr) {
		m_SelName=pBase->GetName();
	}
	else
		m_SelName="";
	if(m_pDoc==nullptr) return;
	if(SumTyp!="") {
		if(SumTyp=="Initial")
			m_pSimDoc->m_SumIndicator=0;
		else if(SumTyp=="Final")
			m_pSimDoc->m_SumIndicator=1;
		else if(SumTyp=="Min")
			m_pSimDoc->m_SumIndicator=2;
		else if(SumTyp=="Max")
			m_pSimDoc->m_SumIndicator=3;
		else if(SumTyp=="Mean")
			m_pSimDoc->m_SumIndicator=4;
		else if(SumTyp=="Accumulated")
			m_pSimDoc->m_SumIndicator=5;
	}
	else
		m_pSimDoc->m_SumIndicator=-1;


	m_SelLocalIndex=localindex;
	if(type=="Parameters"&&localindex==-1&&pBase!=nullptr) {
		string GTitle;												
		GTitle=((Ps*)pBase)->GetViewTitle();
		if(GTitle!="")  Make_ParameterChart((Ps*)pBase,FALSE);

	}
	if(type=="Tables"&&localindex==-1&&pBase!=nullptr) {
		CString GTitle;												
		GTitle=((Tab*)pBase)->GetViewTitle().c_str();
		//if(GTitle!="")  Make_ParameterChart((Tab*)pBase,FALSE);

	}
	if(type=="Tables"&&pCPt!=nullptr&&pBase!=nullptr) {

		Make_TableChart((Tab*)pBase,pCPt);
		OnGetHelp(pCPt);

	}
	else if(pBase!=nullptr) {
		if(type=="State Variables"||type=="Flow Variables"||type=="Auxiliary Variables"||type=="Driving Variables") {
				if(m_pSimDoc->m_SelectOut.size()>=m_pProp->m_NumSubCharts*m_pProp->m_NumVariable&&m_pSimDoc->m_SumIndicator==-1)
					m_pSimDoc->m_SelectOut.clear();
				m_pSimDoc->m_NewChart=true;
				if (m_SelectAllIndex) {
					auto save=m_pProp->m_ChartType;
					//m_pProp->ResetSerie1(true);
				

				}
				if (!m_pSimDoc->IsFinishedSimulation()) {
					OutSingle *pSingle = nullptr;
					OutVector *pVector = nullptr;
					pVector = dynamic_cast<OutVector*>(m_pSel);
					if (pVector != NULL) {
						auto selected_Single = pVector->GetStoreFlag(localindex-1);
						if (selected_Single == 1) pVector->SetStoreFlag(localindex - 1,0);
						else if (selected_Single == 0) pVector->SetStoreFlag(localindex - 1, 1);
					}
					else {
						pSingle = dynamic_cast<OutSingle*>(m_pSel);
						auto selected_Single = pSingle->GetStoreFlag();
						if (selected_Single == 1) pSingle->SetStoreFlag(0);
						else if (selected_Single == 0) pSingle->SetStoreFlag(1);
					}
				}



		}
		OnGetHelp(pBase);

	}
}
void MyCug::UpdateSelected_MR(int idim, int irow_in)
{
	CUGCell cell;
	//MultiVAL mul;
	SetSheetNumber(10);
	int col=2;
	long row=irow_in;

	GetCell(col,row, &cell);
	if(!cell.GetBool()) {
		MultiPlotClick(10,col, row, TRUE);
	}
	else {
		// Remove from list if in list
		for(size_t i=0;m_pSimDoc->m_SelectMultiPar.size();i++){
			if(col==m_pSimDoc->m_SelectMultiPar[i].col&&row==m_pSimDoc->m_SelectMultiPar[i].row)
				m_pSimDoc->m_SelectMultiPar.erase(m_pSimDoc->m_SelectMultiPar.begin()+i);
		}
	}
	UpdatePlotStatusBar(10);
}
void MyCug::OnGetHelp(SimB *pBase)
{
	
	OnGetHelp(pBase->GetName().c_str(), pBase->GetGroup().c_str(), pBase->GetType());
	
}
void MyCug::UpdateChartInfo()
{


	//	m_NumChartX=MySingle::instance().GetChartPropPointer()->m_NumChartX;
	//	m_NumChartY=MySingle::instance().GetChartPropPointer()->m_NumChartY;
	//	m_NumSubCharts=m_NumChartX*m_NumChartY;
		UpdatePlotStatusBar(0,TRUE);

}
void MyCug::CleanSelectedBySheet(int sheet) {
		CUGCell cell;
			int col, row;
			if(sheet==4||sheet==11) {

					CPG *pCPG;
					int index;
					F* pF;
					while(m_pSimDoc->m_SelectModelFile.size()>0) {	
						pF=(F*)m_SelectedFPtr.GetAt(0);
						pCPG=(CPG*)pF->GetPointer();
						index=m_SelectedFIndex.GetAt(0);
						pCPG->SetPicked(index,false);
						m_SelectedFIndex.RemoveAt(0);
						m_SelectedFPtr.RemoveAt(0);
						m_SelectedChartPGIndex.RemoveAt(0);
					}


				}
				else if(sheet==5) {

					int ico;
					while(m_pSimDoc->m_SelectOut.size()>0) {
						row=m_pSimDoc->m_SelectOut[0].row;
						if(m_pSimDoc->m_SelectOut[0].MeanIndicator==ENSEMBLE_TYPE::NO)
							ico=2;
						else
							ico=3;
						GetCell(ico,row,&cell);
						cell.SetBool(FALSE);
						SetCell(ico,row,&cell);
						RedrawCell(ico,row);
						QuickSetNumber(4,row,0);
						QuickSetText(4,row,"");
						RedrawCell(4,row);
						m_pSimDoc->m_SelectOut.erase(m_pSimDoc->m_SelectOut.begin());
					}




				}
				else if(sheet==6) {
					while(m_pSimDoc->m_SelectValTime.size()>0) {
						col=m_pSimDoc->m_SelectValTime[0].col;
						row=m_pSimDoc->m_SelectValTime[0].row;
						GetCell(col,row,&cell);
						cell.SetBool(FALSE);
						SetCell(col,row,&cell);
						RedrawCell(col,row);
						QuickSetText(5,row,"");
						m_pSimDoc->m_SelectValTime.erase(m_pSimDoc->m_SelectValTime.begin());
					}
	

				};



	
}
void MyCug::CreateNewDocFromCurrentDoc(bool ChildDocument, bool KeepParentDoc)
{
	CCoupModelApp* pApp;
	string pzPathName, Num, newPathName, newMBinPatHName;
	int ipos_sim, iposext,iposMul;
		// Create new document
	//if(ReadyToSimulate) return;

	
			pApp = (CCoupModelApp*)AfxGetApp();
			pzPathName=CT2A(m_pDoc->CDocument::GetPathName()).m_psz;
					iposMul=pzPathName.find("_Multi");
								
					iposext=pzPathName.rfind('_');


				//	if (!m_pDoc->CDocument::OnSaveDocument(CString(pzPathName.c_str()))) return;

  					int PreviousRunNo=m_pSimDoc->m_DocFile.m_SimulationRunNo;
					m_pSimDoc->m_DocFile.m_SimulationRunNo=MFC_Util::GetProfileIntNo( "SimulationRunNo", m_pSimDoc->m_DocFile.m_SimulationRunNo);
					m_pSimDoc->m_DocFile.m_SimulationRunNo=(m_pSimDoc->m_DocFile.m_SimulationRunNo-1)%999999+1;
					MFC_Util::WriteProfileInt( "SimulationRunNo",m_pSimDoc->m_DocFile.m_SimulationRunNo+1); //Allways increment with 1
					m_pSimDoc->m_DocFile.m_SimulationRunNo=MFC_Util::GetProfileIntNo( "SimulationRunNo", m_pSimDoc->m_DocFile.m_SimulationRunNo);
					
					if(m_pSimDoc->m_DocFile.m_SimulationRunNo==PreviousRunNo) {
						m_pSimDoc->m_DocFile.m_SimulationRunNo++;
						MFC_Util::WriteProfileInt( "SimulationRunNo", m_pSimDoc->m_DocFile.m_SimulationRunNo);
					}
					Num=FUtil::ItoNumAscii(m_pSimDoc->m_DocFile.m_SimulationRunNo);


					string filetype = ".Sim";

					if (m_pSimDoc->m_xmlFileToUse) filetype = ".xml";

					ipos_sim=pzPathName.find(".S");

					if (m_pSimDoc->m_xmlFileToUse) {
						filetype = ".xml";
						ipos_sim = pzPathName.find(".xml");
						if(ipos_sim==string::npos) ipos_sim = pzPathName.find(".S");
					}
					if(iposext>0)
						newPathName=pzPathName.substr(0,iposext+1)+Num+filetype;
					else if(ipos_sim>0)
						newPathName=pzPathName.substr(0,ipos_sim)+"_"+Num+filetype;
					else {
							int ipos=pzPathName.size();
							newPathName=pzPathName.substr(0,ipos)+"_"+Num+filetype;
					}

					string saveMBinFileName = m_pSimDoc->m_DocFile2.m_Multi_MBin_File;
					ipos_sim = newPathName.rfind(".Sim");
					if(ipos_sim==string::npos) ipos_sim = pzPathName.rfind(".xml");
					if (ipos_sim != string::npos)
						m_pSimDoc->m_DocFile2.m_Multi_MBin_File = newPathName.substr(0, ipos_sim) + ".MBin";
					m_pDoc->SetModifiedFlag(FALSE);
					if(p_ModelInfo->IsMultiRunning()) 
						m_pSimDoc->MR_ResetDimCounter();
					
					string OldOriginFileName=m_pSimDoc->m_DocFile2.m_OriginFileName;
					if(!KeepParentDoc)	m_pSimDoc->m_DocFile2.m_OriginFileName=pzPathName;
					m_pSimDoc->SetNewSimulation();
					m_pSimDoc->m_Protect=FALSE;
					m_pSimDoc->m_DocFile.m_MultiStoragelocked=true;
					//m_pSimDoc->m_FileVersionNumberRead=0;
					m_pSimDoc->TimeCreated();
					m_pSimDoc->SetTimeModified(0);
					//m_pSimDoc->ClearRunReportArray();
					string str;
					str = FUtil::ItoNumAscii(m_pSimDoc->m_DocFile.m_SimulationRunNo);
					m_pSimDoc->History_Add(13,str);
					m_pDoc->SetToBeChild(ChildDocument);

					if (!m_pDoc->OnSaveDocument(CString(newPathName.c_str()))) return;
				m_pSimDoc->SetFinishedSimulation();
				if (newPathName != pzPathName) {
					m_pSimDoc->m_DocFile.m_SimulationRunNo = PreviousRunNo;
				}
					
					m_pDoc->SetModifiedFlag(FALSE);
					m_pSimDoc->m_CurrentFile=string(pzPathName);
					m_pSimDoc->m_DocFile2.m_OriginFileName=string(OldOriginFileName);

					//::ReadResultsFromFile=FALSE;
					m_created_pathname=newPathName;
					pApp->m_pDocTemplate->OpenDocumentFile(CString(newPathName.c_str()));
					m_UpdateOutputList = true;
					//ReadyToSimulate=true;

}
void MyCug::MakeSingleRun()
{
	SetSheetNumber(0);
	CCoupModelApp* pApp;
	string pzPathName, Num, newPathName;
			p_ModelInfo->SetMultiRunning(false);
			pApp = (CCoupModelApp*)AfxGetApp();
			if(m_pDoc==nullptr) {
				MFC_Util::MessageBox("No Document linked to the simulation, No simulation possible !");
				return;

			}
			m_pSimDoc->SetSingleSimulation();
			//MFC_FUtil::MessageBox(" You can not modify document when modell is running !");
			pzPathName=m_pSimDoc->GetCurrentSimFile();
			if(pzPathName=="") {
				MFC_Util::MessageBox("No Document linked to the simulation, No simulation possible !");
				return;

			}
			if(m_pSimDoc->RunOpt.longMinStart>m_pSimDoc->RunOpt.longMinEnd) {

				MFC_Util::MessageBox("Start Time is after the End Time - Please check RunInfo and Dates for simulations, No simulation possible !");
				return;
			}

			if(pzPathName==m_created_pathname)
				remove(pzPathName.c_str());

			newPathName=MFC_Util::GetNewSimFileName(1, pzPathName);

			m_pDoc->CDocument::SetPathName(CString(newPathName.c_str()),FALSE);   
			m_pSimDoc->SetCurrentFileName(newPathName);

			if (!m_pSimDoc->WriteDocFile(m_pSimDoc->m_ChildDocument)) return;
			m_pSimDoc->OnSimStart();

			QuickSetText(0,-1,"Model Running !");
			QuickSetBackColor(0,-1,m_ColorChanged);
			RedrawCell(0,-1);
			m_pSimDoc->History_Add(11,"Single Run");
			m_CurrentChart=1;
			m_Display=TRUE;
			ReportUpdate();
			pApp->CloseSimDocument();
			Make_RunInfoSheet();
}
void MyCug::MakeMultiRuns(int col)
{
	SetSheetNumber(0);
	CCoupModelApp* pApp;
	string pzPathName, Num, newPathName;
	int ipos_sim, iposext,iposMul;
	//MultiRunning=TRUE;
	p_ModelInfo->SetMultiRunning(true);
	m_pSimDoc->SetMultiSimulation();
	pApp = (CCoupModelApp*)AfxGetApp();

	pzPathName=m_pSimDoc->GetCurrentSimFile();
	if(pzPathName=="") {
		MFC_Util::MessageBox("No Document linked to the simulation, No simulation possible !");
		return;

	}
	if(pzPathName==m_created_pathname)
		remove(pzPathName.c_str());
	m_Display=TRUE;
		//if (!m_pSimDoc->CDocument::OnSaveDocument(pzPathName)) return;
				
		iposext=pzPathName.rfind('_');
		iposMul=pzPathName.find("_Multi");

		int SimulationRunNo=MFC_Util::GetProfileIntNo( "SimulationRunNo", m_pSimDoc->m_DocFile.m_SimulationRunNo);
		SimulationRunNo=(SimulationRunNo-1)%999999+1;
		MFC_Util::WriteProfileInt( "SimulationRunNo",SimulationRunNo);
		Num=FUtil::ItoNumAscii(SimulationRunNo);
		if(iposMul>0&&iposext>0) {
			if(m_pSimDoc->m_xmlFileToUse)
				newPathName = pzPathName.substr(0, iposMul + 7) + Num + ".xml";
			else
				newPathName=pzPathName.substr(0,iposMul+7)+Num+".Sim";
		}
		else if (iposext > 0) {
			if (m_pSimDoc->m_xmlFileToUse)
				newPathName = pzPathName.substr(0, iposext + 1) + "Multi_" + Num + ".xml";
			else
				newPathName = pzPathName.substr(0, iposext + 1) + "Multi_" + Num + ".Sim";

		}
		else {
			ipos_sim=pzPathName.find(".S");
			if(ipos_sim>0)
				newPathName=pzPathName.substr(0,ipos_sim)+"_Multi_"+Num+".Sim";
			else {
				int ipos=pzPathName.size();
				if (m_pSimDoc->m_xmlFileToUse)
					newPathName = pzPathName.substr(0, ipos) + "_Multi_" + Num + ".xml";
				else
					newPathName=pzPathName.substr(0,ipos)+"_Multi_"+Num+".Sim";
			}
		}
		m_pSimDoc->m_DocFile.m_SimulationRunNo=SimulationRunNo;
		m_pDoc->SetModifiedFlag(TRUE);

		if(col==1){
			m_pSimDoc->MR_ResetDimCounter();
		}
		else if(col==2) {
			if(m_pSimDoc->MR_Get_ActualRunNumber()==m_pSimDoc->MR_Get_TotalNumberofRuns()) {
				for(int i=0;i<m_pSimDoc->MR_GetNumberOfDimensions();i++){
					int n=m_pSimDoc->m_MultiRun_Array[i].NumberOfRepeatitionsWithinDimension;
					m_pSimDoc->MR_SetRepeatitionsOfDimension(i,n*2);
				}					
			}
			m_pSimDoc->Set_ReReadMStorageFile();

		}
		m_pDoc->CDocument::SetPathName(CString(newPathName.c_str()), FALSE);
		//if (!m_pSimDoc->CDocument::OnSaveDocument(newPathName)) return;
			   
		m_pSimDoc->SetCurrentFileName(newPathName);
	
	//	pApp->m_pDocTemplate->OpenDocumentFile(newPathName);

		ReportUpdate();
		if (!m_pSimDoc->WriteDocFile()) return;
		/*QuickSetText(0,-1,"Model Running !");
		QuickSetBackColor(0,-1,m_ColorChanged);
		RedrawCell(0,-1);
		QuickSetText(1,-1,"Model Running !");
		QuickSetBackColor(1,-1,m_ColorChanged);
		RedrawCell(1,-1);*/ 


		m_pSimDoc->History_Add(11,"Multi");
		m_pSimDoc->OnSimMultiStart();
		pApp->CloseSimDocument();
		Make_RunInfoSheet();
}
void MyCug::UnLinkDocuments()
{
	CUGCell cell;
	SimB *pSw;
	int col, row;
	m_LinkedChartDocument=FALSE;
	m_NumOpenDoc=1;
	m_pSimDoc->m_SelectedDocuments.clear();
	if(m_pSimDoc->m_PrevSheet==5) {
		if(m_pSimDoc->m_SelectOut.size()>0) {
			pSw=(SimB*)m_pSimDoc->m_SelectOut[m_pSimDoc->m_SelectOut.size()-1].pBase;
			row=m_pSimDoc->m_SelectOut[m_pSimDoc->m_SelectOut.size()-1].row;
			TimeSerieChartClick(0, row, 5, pSw, 0, 0, "");
		}
		else if(m_pSimDoc->m_SelectMultiSum.size()>0) {
			row=m_pSimDoc->m_SelectMultiSum[m_pSimDoc->m_SelectMultiSum.size()-1].row;
			col=m_pSimDoc->m_SelectMultiSum[m_pSimDoc->m_SelectMultiSum.size()-1].col;
			MultiPlotClick(5,col, row, FALSE);
		}
		SetSheetNumber(5);
		GetCell(3,-1,&cell);
		cell.SetBackColor(m_ColorClick);
		SetCell(3,-1,&cell);
		RedrawCell(3,-1);
		UpdatePlotStatusBar(5);

	}
	else if(m_pSimDoc->m_PrevSheet==6) {
		if(m_pSimDoc->m_SelectValTime.size()>0) {
			row=m_pSimDoc->m_SelectValTime[m_pSimDoc->m_SelectValTime.size()-1].row;
			col=m_pSimDoc->m_SelectValTime[m_pSimDoc->m_SelectValTime.size()-1].col;
			TimeSerieChartClick(col, row, 6, (SimB*)nullptr, 0, 1, "", 1);
		}
		else if(m_pSimDoc->m_SelectMultiVal.size()>0) {
			row=m_pSimDoc->m_SelectMultiVal[m_pSimDoc->m_SelectMultiVal.size()-1].row;
			col=m_pSimDoc->m_SelectMultiVal[m_pSimDoc->m_SelectMultiVal.size()-1].col;
			MultiPlotClick(6, col,row, FALSE);
			UpdatePlotStatusBar(6);

		}
	}
	else if(m_pSimDoc->m_PrevSheet==10) {
			if(m_pSimDoc->m_SelectMultiPar.size()>0) {
				row=m_pSimDoc->m_SelectMultiPar[m_pSimDoc->m_SelectMultiPar.size()-1].row;
				col=m_pSimDoc->m_SelectMultiPar[m_pSimDoc->m_SelectMultiPar.size()-1].col;
				MultiPlotClick(10,col, row, FALSE);
			}
		UpdatePlotStatusBar(10);
	


	}
}
void MyCug::LinkDocuments()
{
	SimB *pSw;
	int col, row;
	CUGCell cell;
	m_LinkedChartDocument=TRUE;
	m_NumOpenDoc=m_pSimDoc->m_SelectedDocuments.size();
	if(m_pSimDoc->m_PrevSheet==5) {
		SetSheetNumber(5);
		if(m_pSimDoc->m_SelectOut.size()>0) {
			pSw=(SimB*)m_pSimDoc->m_SelectOut[m_pSimDoc->m_SelectOut.size()-1].pBase;
			row=m_pSimDoc->m_SelectOut[m_pSimDoc->m_SelectOut.size()-1].row;
			TimeSerieChartClick(3, row, 5, pSw, 0, 0, "");
			SetSheetNumber(5); 
			GetCell(3,-1,&cell);
			cell.SetBackColor(m_ColorChanged);
			SetCell(3,-1,&cell);
			RedrawCell(3,-1);
		}
		else if(m_pSimDoc->m_SelectMultiSum.size()>0) {
			row=m_pSimDoc->m_SelectMultiSum[m_pSimDoc->m_SelectMultiSum.size()-1].row;
			col=m_pSimDoc->m_SelectMultiSum[m_pSimDoc->m_SelectMultiSum.size()-1].col;
			MultiPlotClick(5,col, row, FALSE);
		}
		UpdatePlotStatusBar(5);
	}
	else if(m_pSimDoc->m_PrevSheet==6) {
		if(m_pSimDoc->m_SelectValTime.size()>0) {
			row=m_pSimDoc->m_SelectValTime[m_pSimDoc->m_SelectValTime.size()-1].row;
			col=m_pSimDoc->m_SelectValTime[m_pSimDoc->m_SelectValTime.size()-1].col;
			TimeSerieChartClick(col, row, 6, (SimB*)nullptr, 0, 0, "", 0);
		}
		else if(m_pSimDoc->m_SelectMultiVal.size()>0) {
			row=m_pSimDoc->m_SelectMultiVal[m_pSimDoc->m_SelectMultiVal.size()-1].row;
			col=m_pSimDoc->m_SelectMultiVal[m_pSimDoc->m_SelectMultiVal.size()-1].col;

			MultiPlotClick(6,col, row, FALSE);

		}
		UpdatePlotStatusBar(6);
	}
	else if(m_pSimDoc->m_PrevSheet==10) {
		if(m_pSimDoc->m_SelectMultiPar.size()>0) {
			row=m_pSimDoc->m_SelectMultiPar[m_pSimDoc->m_SelectMultiPar.size()-1].row;
			col=m_pSimDoc->m_SelectMultiPar[m_pSimDoc->m_SelectMultiPar.size()-1].col;
			MultiPlotClick(10,col, row, FALSE);
		}
		UpdatePlotStatusBar(10);
	}
		

}


