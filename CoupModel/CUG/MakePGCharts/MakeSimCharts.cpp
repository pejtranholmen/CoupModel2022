#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include "..\..\Util.h"
#include "..\..\NewBase\FileScanner.h"
#include "MakeSimCharts.h"
#include "..\ChartCug.h"
#include "..\..\MainFrm.h"
#include "..\..\Util\PlotInfo.h"
#include "..\..\Newbase\doc.h"

UINT UpdateAcceptedThreadProc2( LPVOID pParam )
{
    CResiduals* pObject = (CResiduals*)pParam;
	pObject->UpdateStatistics(TRUE, NULL);

    // do something with 'pObject'

    return 0;   // thread completed successfully
}

MakeSimCharts::MakeSimCharts(void)
{
	m_pChartCug=MySingle::instance().GetChartCugPointer();
	m_pPlotInfo=MySingle::instance().GetPlotInfoPointer();
	m_CurrentChartType=0;
}
MakeSimCharts::~MakeSimCharts(void)
{
		int kalle=1;
}

bool MakeSimCharts::Make_ParameterChart(SimB* pSimB, Tab *pTab, size_t index, size_t FuncNo)
{
	m_bDrawExcelFocus = false;

	// extrapoint =true Make new plot if not previous plot
	//if(pCPt!=NULL){
	/*	if(((Tab*)pSw)->GetViewFunction()==401) {
			m_pSimDoc->SP_OpenDataBase();
		//	extrapoint=true;
		}
	}*/

			ChartCug *pChart;
			pChart=MySingle::instance().GetChartCugPointer();
			pChart->Init(m_pSimDoc);
			pChart->ParameterPlotMode(true);


			CleanChartsMade();
			
			if(!pChart->m_ParameterPlot.InitData(m_pSimDoc, pSimB, pTab, index, FuncNo)) return false;		
			pChart->m_ParameterPlot.OnInit();
			pChart->OnSetup();



	return true;

}
bool MakeSimCharts::Make_SwitchChart(Sw *pSw, string Group)
{
		CWinApp* pApp = AfxGetApp();

		CCoupModelApp *pCoupModel=(CCoupModelApp*)pApp;
			ChartCug *pChart;
			pChart=MySingle::instance().GetChartCugPointer();

	//	pMainFrame = (CMainFrame*) AfxGetMainWnd();
		pChart->Init(m_pSimDoc);
		if(m_pSimDoc->GetModelView()==NULL) return false;

		CleanChartsMade();
		if(pSw==NULL&&m_pSimDoc->m_DefinedSwitches.size()==0)
			pChart->MakeTextPlot("No Switches set !\nonly the heat flow equation is solved");		
		else if(pSw==NULL)
			pChart->m_ModelView.MakeModelPlot();
	/*	pChart->Init(m_pDoc);
		pChart->ParameterPlotMode(false);
		pChart->TablePlotMode(true);
		
		pChart->OnSetup();*/

	return true;

}
bool MakeSimCharts::Make_TableChart(Tab* pTab, P *pCPt)
{

		ChartCug *pChart;
		pChart=MySingle::instance().GetChartCugPointer();
		CleanChartsMade();
		pChart->Init(m_pSimDoc);
		pChart->ParameterPlotMode(false);
		pChart->TablePlotMode(true);
		pChart->m_TableView.MakeTablePlot(pTab, pCPt);		

		pChart->OnSetup();

	return true;

}
bool MakeSimCharts::ManagePlotWindows(int item, int sheet, bool multisum)
{
	CUGCell cell;
	ChartCug *pChart;
	pChart=MySingle::instance().GetChartCugPointer();
	if(item<4000||item>4011) return false;
	
	m_newChart=false;
	CString text, text1;
	if(m_newChart)
		text1="Add to previous chart mode";
	else
		text1="New chart mode";
		
			if(item==4000) {
					m_pSimDoc->m_PlotStorage.RemoveAll();
					m_VarNo=0;
				    long ivv=GetTopRow();
					EnableUpdate(false); 
					for(long i=0;i<GetNumberRows();i++){
						switch(sheet) {
							case 5:
								GetCell(2,i,&cell);
								cell.SetBackColor(m_NoColor);
								SetCell(2,i,&cell);
								break;
							default:
								;
						}

					}
			//			DeleteRow(0);
					OnSheetSetup(8);
					while(GetNumberRows()>0)
							DeleteRow(0);				
			//		OnSheetSetup(sheet);
					EnableUpdate(true); 
					SetSheetNumber(sheet);
					SetTopRow(ivv);




	
					while(m_Charts_Array.GetSize()>0) {

					}
					m_pSimDoc->m_PlotStorage.CPG_Reset();
					m_CurrentChart=1;




				}
			else if(item==4001) {
					if(m_newChart) {
						m_newChart=false;
						m_CurrentChart++;	
						text1="New chart mode";
					}
					else {
						m_newChart=true;
						text1="Add to previous chart mode";
					}
			}
			else if(item==4002) {
					m_CurrentChartType=0;
					if(m_newChart) m_CurrentChart++;
					m_newChart=false;
					text="Scatter plot mode";		
				}
			else if(item==4003) {
					m_CurrentChartType=1;
					if(m_newChart) m_CurrentChart++;
					m_newChart=false;
					text="Time Serie plot mode";					
				}
			else if(item==4004) {
					m_CurrentChartType=2;
					if(m_newChart) m_CurrentChart++;
					m_newChart=false;
					text="Distribution Frecquency mode";
				}
			else if(item==4005) {
					m_CurrentChartType=3;
					if(m_newChart) m_CurrentChart++;
					m_newChart=false;
					text="Acc Frequency  mode";
				}
			else if(item==4006) {
				m_pProp->m_NumChartX++;
			}
			else if(item==4007) {
				m_pProp->m_NumChartX--;
			}
			else if(item==4008) {
				m_pProp->m_NumChartY++;
			}
			else if(item==4009) {
				m_pProp->m_NumChartY--;
			}
			if(item>4005&&item<4010) {

				MySingle::instance().GetChartPropPointer()->SetNumChartY(m_pProp->m_NumChartY);
				MySingle::instance().GetChartPropPointer()->SetNumChartX(m_pProp->m_NumChartX);
				m_pApp->WriteProfileInt("CoupModel - KTH", "NumberChartX",m_pProp->m_NumChartX);
				m_pApp->WriteProfileInt("CoupModel - KTH", "NumberChartY",m_pProp->m_NumChartY);
				m_pProp->m_NumSubCharts=m_pProp->m_NumChartX*m_pProp->m_NumChartY;

			}

			else if(item==4011) {
				int col, row;
		
				if(sheet==4) {


					CPG *pCPG;
					int index;
					F* pF;
					while(m_pSimDoc->m_SelectModelFile.size()>0) {
						pCPG=m_pSimDoc->m_SelectModelFile[0].pPG;
						index=m_pSimDoc->m_SelectModelFile[0].PGIndex;
						pCPG->SetPicked(index,false);
						m_pSimDoc->m_SelectModelFile.erase(m_pSimDoc->m_SelectModelFile.begin());
					}


					/*while(m_SelectedFPtr.GetSize()>0) {	
						pF=(F*)m_SelectedFPtr.GetAt(0);
						pCPG=(CPG*)pF->GetPointer();
						index=m_SelectedFIndex.GetAt(0);
						pCPG->SetPicked(index,false);
						m_SelectedFIndex.RemoveAt(0);
						m_SelectedFPtr.RemoveAt(0);
						m_SelectedChartPGIndex.RemoveAt(0);
					}*/

				}
				else if(sheet==11) {
					CPG *pCPG;
					int index;
					while(m_pSimDoc->m_SelectPGFile.size()>0) {
						pCPG=m_pSimDoc->m_SelectPGFile[0].pPG;
						index=m_pSimDoc->m_SelectPGFile[0].PGIndex;
						pCPG->SetPicked(index,false);
						m_pSimDoc->m_SelectPGFile.erase(m_pSimDoc->m_SelectPGFile.begin());
					}
					if(m_pProp->IsFirstIndepSet(PGVAR)) {
						m_pProp->SetFirstIndepSet(PGVAR, false);



					}
	/*				while(m_SelectedPGPtr.GetSize()>0) {	
						pCPG=(CPG*)m_SelectedPGPtr.GetAt(0);
						index=m_SelectedPGIndex.GetAt(0);
						pCPG->SetPicked(index,false);
						m_SelectedPGPtr.RemoveAt(0);
						m_SelectedChartPGIndex.RemoveAt(0);
						m_SelectedPGRow.RemoveAt(0);
						m_SelectedPGIndex.RemoveAt(0);
						m_SelectedPG_Repit.RemoveAt(0);
					}*/

				}
				else if(sheet==5) {
					if(multisum) {
						while(m_pSimDoc->m_SelectMultiSum.size()>0) {
							col=m_pSimDoc->m_SelectMultiSum[0].col;
							row=m_pSimDoc->m_SelectMultiSum[0].row;
							GetCell(col,row,&cell);
							cell.SetBool(false);
							cell.SetBackColor(m_NoColor);
							SetCell(col,row,&cell);
							RedrawCell(col,row);
							m_pSimDoc->m_SelectMultiSum.erase(m_pSimDoc->m_SelectMultiSum.begin());
						}


					}
					else {
						SelectOUT out;
						while(m_pSimDoc->m_SelectOut.size()>0) {
							row=m_pSimDoc->m_SelectOut[0].row;
							int ico;
							if(m_pSimDoc->m_SelectOut[0].MeanIndicator==1||m_pSimDoc->m_SelectOut[0].MeanIndicator==2)
								ico=3;
							else 
								ico=2;
							GetCell(ico,row,&cell);
							cell.SetBool(false);
							SetCell(ico,row,&cell);
							RedrawCell(ico,row);
							QuickSetNumber(4,row,0);
							QuickSetText(4,row,"");
							RedrawCell(4,row);
							m_pSimDoc->m_SelectOut.erase(m_pSimDoc->m_SelectOut.begin());
						}
					}
				}
				else if(sheet==6) {
					if(multisum) {
						while(m_pSimDoc->m_SelectMultiVal.size()>0) {
							col=m_pSimDoc->m_SelectMultiVal[0].col;
							row=m_pSimDoc->m_SelectMultiVal[0].row;
							GetCell(col,row,&cell);
							cell.SetBool(false);
							SetCell(col,row,&cell);
							RedrawCell(col,row);
							m_pSimDoc->m_SelectMultiVal.erase(m_pSimDoc->m_SelectMultiVal.begin());
						}
					}
					else {
						while(m_pSimDoc->m_SelectValTime.size()>0) {
							col=m_pSimDoc->m_SelectValTime[0].col;
							row=m_pSimDoc->m_SelectValTime[0].row;
							GetCell(col,row,&cell);
							cell.SetBool(false);
							SetCell(col,row,&cell);
							RedrawCell(col,row);
							QuickSetText(5,row,"");
							m_pSimDoc->m_SelectValTime.erase(m_pSimDoc->m_SelectValTime.begin());
						}
					}

				}
				else if(sheet==10) {
					while(m_pSimDoc->m_SelectMultiPar.size()>0) {
						col=m_pSimDoc->m_SelectMultiPar[0].col;
						row=m_pSimDoc->m_SelectMultiPar[0].row;
						GetCell(col,row,&cell);
						cell.SetBool(false);
						SetCell(col,row,&cell);
						RedrawCell(col,row);
						m_pSimDoc->m_SelectMultiPar.erase(m_pSimDoc->m_SelectMultiPar.begin());
					}

				};

				pChart->MakeTextPlot("No Variables selected");



			};





			UpdatePlotStatusBar(5);
		/*	CMainFrame *p_mainframe;
			p_mainframe = (CMainFrame*) AfxGetMainWnd();
			if(item>4001&&item<4006)
				p_mainframe->UpdatePaneText(1, text);  
			p_mainframe->UpdatePaneText(6, text1); */
			Invalidate();
	return true;
}
void MakeSimCharts::MakePlotWindows()
{
	int index;
	int itype;
	CCoupModelApp* pApp = (CCoupModelApp*)AfxGetApp();
			ChartCug *pChart;
			pChart=MySingle::instance().GetChartCugPointer();


	m_CurrentChart=1;

	int DisplayPlot;
	DisplayPlot=pApp->GetProfileInt("CoupModel - KTH", "KeepPlotOnScreen", 0);
	
	if(!DisplayPlot) {
		//m_pSimDoc->m_m_PlotStorage.RemoveAll();
		m_CurrentChart=m_pSimDoc->m_PlotStorage.GetNumberofCharts()+1;
		return;
	}
	for(int j=0;j<m_pSimDoc->m_PlotStorage.GetNumberofCharts();j++) {
			index=m_pSimDoc->m_PlotStorage.GetIndex_Chart(j);
			if(index>=0) {
				m_CurrentChartType=m_pSimDoc->m_PlotStorage.GetChartType(index);
				m_CurrentChart=m_pSimDoc->m_PlotStorage.GetChart(index);
				itype=m_pSimDoc->m_PlotStorage.GetType(index);
				CString strfile;
				strfile=_T("");
					if(itype==0){

						SimB *pSw=(SimB*)m_pSimDoc->m_PlotStorage.GetPtr(j);
						if(pSw!=NULL)
							strfile=((F*)pSw)->GetStrValue().c_str();
					}
			//	CreateChart(m_CurrentChartType, m_CurrentChart, itype, strfile);
			m_pFrame->m_wndProperties.SetChoice(0);

				m_CurrentChart=m_pSimDoc->m_PlotStorage.GetChart(index);
				if((itype>5&&itype<10)||itype>10) {
						if(pChart->InitMultiRunPlot(m_pSimDoc, m_CurrentChart, m_CurrentChartType)) {
							m_CurrentChart=m_pSimDoc->m_PlotStorage.GetChart(index);
						}
				}
				else if(itype==0) {
						SimB *pSw=(SimB*)m_pSimDoc->m_PlotStorage.GetPtr(index);
	
						//					if(m_pZoom->InitData(m_pDoc, m_CurrentChart, m_CurrentChartType,((F*)pSw)->GetValue())){ 
						if(pChart->InitData(m_pSimDoc, m_CurrentChart,m_CurrentChartType,((F*)pSw)->GetStrValue().c_str())){ 
							m_CurrentChart=m_pSimDoc->m_PlotStorage.GetChart(index);	

						}
				}
				else {
					if(pChart->InitData(m_pSimDoc, m_CurrentChart,m_CurrentChartType)) {

					}

				}
			}

	}
	m_CurrentChart=int(m_pSimDoc->m_PlotStorage.GetNumberofCharts());
	if(m_CurrentChart<=0) m_CurrentChart=1;

	m_newChart=false;
}
void MakeSimCharts::UpdatePlotStatusBar(int ichoice, BOOL update) 
{

	string text, txt;
    

	switch (ichoice) {
		if(m_pSimDoc->m_PlotStorage.GetNumVar(m_CurrentChart)>0)
			txt=MFC_Util::ItoAscii(m_pSimDoc->m_PlotStorage.GetNumVar(m_CurrentChart))+"+";
		else
			txt=_T("");
		case 0:

			text="Outputs " +txt+ FUtil::STD_ItoAscii(m_pSimDoc->m_SelectOut.size());
			m_pFrame->UpdatePaneText(2, text);  
			break;
		case 1:
			text="Validation " +txt+ FUtil::STD_ItoAscii(m_pSimDoc->m_SelectValTime.size());
			m_pFrame->UpdatePaneText(2, text);  
			break;
		case 2:
			text="Input File " +txt+ FUtil::STD_ItoAscii(m_pSimDoc->m_SelectModelFile.size());
			m_pFrame->UpdatePaneText(2, text);  
			break;
		case 3:
			text="PG File " +txt+ FUtil::STD_ItoAscii(m_pSimDoc->m_SelectPGFile.size());
			m_pFrame->UpdatePaneText(2, text);  
			break;
		case 5:
			text="Multi Sum: "+FUtil::STD_ItoAscii(m_pSimDoc->m_SelectMultiSum.size());
			m_pFrame->UpdatePaneText(2, text);
			break;
		case 6:
			text="Multi Val: "+FUtil::STD_ItoAscii(m_pSimDoc->m_SelectMultiVal.size());
			m_pFrame->UpdatePaneText(2, text);  
			break;
		case 10:
			text="Multi Par: "+FUtil::STD_ItoAscii(m_pSimDoc->m_SelectMultiPar.size());
			m_pFrame->UpdatePaneText(2, text); 
			break;
		default:
			text="Selected:"+FUtil::STD_ItoAscii(m_pSimDoc->m_PlotStorage.GetNumVar(m_CurrentChart));
			m_pFrame->UpdatePaneText(2, text);  
			break;

	}




	
	string text2, text1;
	if(m_newChart)
		text1="Add to previous chart mode";
	else
		text1="New chart mode";
	switch(m_CurrentChartType) {
		case 0:

			text2="Scatter plot mode";
			break;
		case 1:
			text2="Time serie plot mode";
			break;
		case 2:
			text2="Distribution Frecquency mode";
			break;
		case 3:
			text2="Acc Frequency  mode";
			break;

	}
	m_pFrame->UpdatePaneText(1, text1);
	//m_pFrame->UpdatePaneText(6, text2);
	text2=_T("");
	if(m_pSimDoc->m_SelectedDocuments.size()>1)
		text2="Linked ("+MFC_Util::ItoAscii(m_pSimDoc->m_SelectedDocuments.size())+")";
	else
		text2="UnLinked";
	m_pFrame->UpdatePaneText(3, text2);

	if(m_pProp->m_NumSubCharts>1) {
		text2=FUtil::STD_ItoAscii(m_pProp->m_NumChartX)+"x"+FUtil::STD_ItoAscii(m_pProp->m_NumChartY)+"Charts";
	}
	else
		text2="No SubCharts";
	m_pFrame->UpdatePaneText(4, text2);
	text2=_T("");

	m_pFrame->UpdatePaneText(5, text2);
	
	
}
bool MakeSimCharts::MultiPlotClick(int sheet, int col, int row, bool Add, int Indep) 
{
		ChartCug *pChart;
		pChart=MySingle::instance().GetChartCugPointer();
		m_pSimDoc->m_PlotStorage.Init(m_pSimDoc);
		int itype, Index;
		int isheet;
		CUGCell cell, cell1;
		SimB *pSw;
		int ValFileNo, ValFileIndex;
		MultiSel pos;	
		int nselV=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
		pSw=NULL;
		ValFileNo=ValFileIndex=0;
		MultiVAL mul;
		if(!m_pSimDoc->MR_Storage_Open()) {
			m_pSimDoc->SetMultiStorageBinFileMissing();
			return false;
		}
		m_pPlotInfo->SetDoc(m_pSimDoc);


		if(Indep==0) {
			m_pProp->m_ActualMultiSheet=sheet;
			if(sheet==5) {
				if(Add)
					mul=GetMultiValStruct_Sum(row,col, &Add, Indep);
				m_CurrentChart=m_pSimDoc->m_PlotStorage.GetCurrentChart();
			//	if(m_pSimDoc->m_SelectMultiSum.size()==1)
					m_CurrentChart++;			
			}
			else if(sheet==6) {
				if(Add)
				mul=GetMultiValStruct_Val(row, col, &Add, Indep);
				m_CurrentChart=m_pSimDoc->m_PlotStorage.GetCurrentChart();
			//	if(m_pSimDoc->m_SelectMultiVal.size()==1)
					m_CurrentChart++;
			}
			else if(sheet==10) {
				if(Add)
					mul=GetMultiValStruct_Par(row, col, &Add, Indep);
				m_CurrentChart=m_pSimDoc->m_PlotStorage.GetCurrentChart();
			//	if(m_pSimDoc->m_SelectMultiPar.size()==1)
					m_CurrentChart++;
				


			}
		}
		
		PLOTv plotinfo;
		Doc *pDoc;
		m_NumOpenDoc=max(1,m_pSimDoc->m_SelectedDocuments.size());

		int ivar;
		for(auto j=0;j<m_NumOpenDoc;j++) {
			if(m_NumOpenDoc<=1)
				pDoc=m_pSimDoc;
			else
				pDoc=m_pSimDoc->m_SelectedDocuments[j];
			if(sheet==5) {
				for(int ii=0;ii<m_pSimDoc->m_SelectMultiSum.size();ii++) {
					if(pDoc->IsMultiSimulation()&&pDoc->IsFinishedSimulation()) {
						plotinfo=m_pPlotInfo->MakeMultiSumChart(ii, j);
						if(!m_pSimDoc->m_PlotStorage.AddPlotInfo(plotinfo,  m_CurrentChart)) //, m_CurrentChartType,string(m_pSimDoc->GetCurrentFile()), ValFileNo, ValFileIndex, 0,0,numvar))
							m_pSimDoc->SetMultiStorageBinFileMissing();
					}
				}
				ivar=m_pSimDoc->m_SelectMultiSum.size();
			}
			else if(sheet==6) {
				for(int ii=0;ii<m_pSimDoc->m_SelectMultiVal.size();ii++) {
					if(pDoc->IsMultiSimulation()&&pDoc->IsFinishedSimulation()) {
						plotinfo=m_pPlotInfo->MakeMultiValChart(ii, j);
						if(!m_pSimDoc->m_PlotStorage.AddPlotInfo(plotinfo,  m_CurrentChart)) //, m_CurrentChartType,string(m_pSimDoc->GetCurrentFile()), ValFileNo, ValFileIndex, 0,0,numvar))
							m_pSimDoc->SetMultiStorageBinFileMissing();
					}
				}
				if(m_pSimDoc->m_SelectMultiVal.size()>0) m_CurrentChartType=plotinfo.ChartType;
				ivar=m_pSimDoc->m_SelectMultiVal.size();
			}
			else if(sheet==10) {
				for(int ii=0;ii<m_pSimDoc->m_SelectMultiPar.size();ii++) {
					if(pDoc->IsMultiSimulation()&&pDoc->IsFinishedSimulation()) {
						plotinfo=m_pPlotInfo->MakeMultiParChart(ii, j);
						if(!m_pSimDoc->m_PlotStorage.AddPlotInfo(plotinfo,  m_CurrentChart)) //, m_CurrentChartType,string(m_pSimDoc->GetCurrentFile()), ValFileNo, ValFileIndex, 0,0,numvar))
							m_pSimDoc->SetMultiStorageBinFileMissing();
					}
				}
				ivar=m_pSimDoc->m_SelectMultiPar.size();

			}

		}

		if(ivar==0) {

				pChart->MakeTextPlot("No Variables selected");
				return false;
		}


		return MakeMultiPlot(sheet, col, row);

}
bool MakeSimCharts::MakeMultiPlot(int sheet, int col, int row) {
		ChartCug *pChart;
		pChart=MySingle::instance().GetChartCugPointer();

		int itype, Index;
		int isheet;
		int ivar;
		if(sheet==5) ivar=m_pSimDoc->m_SelectMultiSum.size();
		else if(sheet==6) ivar=m_pSimDoc->m_SelectMultiVal.size();
		else if(sheet==10) ivar=m_pSimDoc->m_SelectMultiPar.size();

		if(ivar==0) {
			pChart->MakeTextPlot("No Variables selected");
			return false;
		}


		CUGCell cell, cell1;	
			if(CreateZoomPlotWindow(true)) {
					for(int ip=0;ip<ivar;ip++) {
						if(sheet==5) {
								Index=m_pSimDoc->m_SelectMultiSum[ip].Index;
								row=m_pSimDoc->m_SelectMultiSum[ip].row;
								col=m_pSimDoc->m_SelectMultiSum[ip].col;
						}
						else if(sheet==6) {
							Index=m_pSimDoc->m_SelectMultiVal[ip].Index;
							col=m_pSimDoc->m_SelectMultiVal[ip].col;
							row=m_pSimDoc->m_SelectMultiVal[ip].row;
						}
						else {
							col=2;
							row=m_pSimDoc->m_SelectMultiPar[ip].row;
						}
						GetCell(col,row, &cell);
					//	cell.SetBool(false);
						cell.SetBackColor(RGB(80,240,240));
						SetCell(col,row, &cell);
						RedrawCell(col,row);
					} 

			}
			else if(m_CurrentChartType==0)
				cell.SetBackColor(RGB(255,40,255));
			else
				cell.SetBackColor(RGB(240,0,0));
				

			UpdatePlotStatusBar(sheet, true);

 return true;
}
void MakeSimCharts::UpdateCurrentChart(bool multi)
{
			int nsel=m_pSimDoc->m_PlotStorage.NumSelected();
			m_CurrentChart=m_pSimDoc->m_PlotStorage.GetCurrentChart();
			if(nsel>0) {
				int ityp=m_pSimDoc->m_PlotStorage.GetType(nsel-1);
				if((ityp<=5||ityp>=10)&&multi||(ityp>5&&ityp<10)&&!multi) {
					if(m_CurrentChart<=m_Charts_Array.GetSize()-1) {
						m_VarNo=0;
						m_CurrentChart++;
					}
				}				
			}
			else
				m_VarNo=0;
			if(!m_newChart)
				m_CurrentChart++;


			if(m_CurrentChart==m_Charts_Array.GetSize()) {

			}
			else if(m_CurrentChart>m_Charts_Array.GetSize()) {
			//	m_CurrentChart--;

			}

}
bool MakeSimCharts::CreateChart(int ChartType, int ChartNum, int datatype, string filename)
{
			ChartCug *pChart;
			pChart=MySingle::instance().GetChartCugPointer();
				m_CurrentChart=ChartNum;
				m_pFrame->m_wndProperties.SetChoice(0);
			


				if((datatype>5&&datatype<10)||(datatype>10&&datatype<100)) {
					if(pChart->m_TimeSeriePlot.InitMultiRunPlot(m_pSimDoc, ChartNum, ChartType)) {
							pChart->m_TimeSeriePlot.MakeTimeSeriePlot();
							return true;
						}
					}
				else if(datatype==0||datatype==10) {
						if(pChart->InitData(m_pSimDoc, ChartNum,ChartType, filename)) { 
							pChart->m_TimeSeriePlot.MakeTimeSeriePlot();
							return true;
						}
				}
				else {
					if(pChart->InitData(m_pSimDoc, ChartNum,ChartType)) {
						pChart->m_TimeSeriePlot.MakeTimeSeriePlot();
						return true;
					}

				}
				return false;
}
bool MakeSimCharts::CreateZoomPlotWindow(bool multi, string FileName)
{
			bool good;

			ChartCug *pChart;
			pChart=MySingle::instance().GetChartCugPointer();
			m_pUtil->m_Displayed=false;
			//m_pFrame->m_wndProperties.SetChoice(0);

			if(multi) {
				good=pChart->m_TimeSeriePlot.InitMultiRunPlot(m_pSimDoc, m_CurrentChart, m_CurrentChartType);
				//good=m_pZoom->InitMultiRunPlot(m_pDoc, m_CurrentChart, m_CurrentChartType);
			}
			else {
				good=pChart->m_TimeSeriePlot.InitData(m_pSimDoc, m_CurrentChart,m_CurrentChartType, string(FileName));
				//good=m_pZoom->InitData(m_pDoc, m_CurrentChart, m_CurrentChartType,FileName);

			}
			if(good) {
					//m_pZoom->Create(IDD_ZOOMSCROLLDEMO_DIALOG, NULL);
				//	m_pZoom->ShowWindow(SW_SHOW);
					pChart->RealTimeMode(false);
					pChart->ParameterPlotMode(false);
					pChart->Init(m_pSimDoc);
					if(pChart->m_TimeSeriePlot.MakeTimeSeriePlot()) 
						pChart->OnSetup();
					else
						return false;
					return true;

			}
			return false;
}
void MakeSimCharts::TimeSerieChart(int isheet) {
	if(isheet==4) {
		if(m_pSimDoc->m_SelectModelFile.size()>0) {
			SelectPG selpg=m_pSimDoc->m_SelectModelFile[m_pSimDoc->m_SelectModelFile.size()-1];
			TimeSerieChartClick(3,selpg.row, 4, selpg.pF);


		}


	}

	else if(isheet==5) {
		if(m_pSimDoc->m_SelectOut.size()>0) {
			SelectOUT selout;
			selout=m_pSimDoc->m_SelectOut[m_pSimDoc->m_SelectOut.size()-1];
			TimeSerieChartClick(2, selout.row, 5);
		}
	}
	else if(isheet==6) {
		if(m_pSimDoc->m_SelectValTime.size()>0) {
			SelectVAL selval;
			selval=m_pSimDoc->m_SelectValTime[m_pSimDoc->m_SelectValTime.size()-1];
			TimeSerieChartClick(selval.col, selval.row, 6);
		}
	}
	else if(isheet==11) {
		if(m_pSimDoc->m_SelectPGFile.size()>0) {
			SelectPG selpg;
			selpg=m_pSimDoc->m_SelectPGFile[m_pSimDoc->m_SelectPGFile.size()-1];
			TimeSerieChartClick(2, selpg.row, 11);
		}
	}


}
void MakeSimCharts::TimeSerieChartClick(long col, long row, long isheet, SimB *Ptr, int Index, int VariableType, string Type, int ValFileNo, int RepitNo)
{
		CString str;
		CUGCell cell, cell1, cell2; 
		CString Group, Name;
		int PGindex, itype;
		string fileName;
		CCoupModelApp* pApp;
		pApp = (CCoupModelApp*)AfxGetApp();
		ChartCug *pChart;
		pChart=MySingle::instance().GetChartCugPointer();
		m_pPlotInfo->SetDoc(m_pSimDoc);
		Doc *pSimDoc;
		int	Isubchart;

		int prov=m_VarNo;
		if(m_pSimDoc->m_SelectedDocuments.size()==0) {
			m_pSimDoc->m_SelectedDocuments.push_back(m_pSimDoc);
		}
		m_NumOpenDoc=max(1,m_pSimDoc->m_SelectedDocuments.size());
		bool test;
		test=true;
		if(m_pSimDoc!=NULL) 
			test=m_pSimDoc->IsFinishedSimulation();

		int ResidualPlot=pApp->GetProfileInt("CoupModel - KTH", "ResidualPlot", 0);

		if(test||isheet==4||isheet==11) {
			// Prepare action for plotting time series
			// 1) Identify output Variable 2) Send to plot class 3) Identify the variable as selected
			//m_pChart= new CFinancedemoDlg;
			
			
			UpdateCurrentChart(false);
			if(isheet==6) {

				if(m_pSimDoc->m_SelectValTime.size()>0) {				
					m_pSimDoc->m_PrevSheet=6;
					for(int j=0;j<m_NumOpenDoc;j++) {
						if(m_NumOpenDoc<=1)
							pSimDoc=m_pSimDoc;
						else
							pSimDoc=m_pSimDoc->m_SelectedDocuments[j];				

						int keeptype=0;
						//pDoc->m_Doc.m_ValidationData.Init(pDoc);
						for(int ii=0;ii<m_pSimDoc->m_SelectValTime.size();ii++) {

							PLOTv plotinfo=m_pPlotInfo->MakeValidationChartInfo(ii, j, ResidualPlot);
							if(j>0&&plotinfo.Type==35) continue;
							if(!m_pSimDoc->m_PlotStorage.AddPlotInfo(plotinfo, m_CurrentChart)){;//, m_CurrentChartType,string(pSimDoc->GetCurrentFile()), ValFileNo, ValFileIndex, mrunno,0,Isubchart, logli)) {
										m_pSimDoc->SetOutputValidBinFileMissing();
										return;
							}
						}

					}
				}
				else {
					pChart->MakeTextPlot("No Variables selected");
					return;
				}
			}
			else if(isheet==5) {
				if(m_pSimDoc->m_SelectedDocuments.size()>0&&m_pSimDoc->m_SelectOut.size()>0) {
					m_pSimDoc->m_PrevSheet=5;
					for(int j=0;j<m_NumOpenDoc;j++) {
						for(int i=0; i<m_pSimDoc->m_SelectOut.size();i++) {
							PLOTv plotinfo=m_pPlotInfo->MakeOutPutChartInfo(i, j);							
							if(m_pSimDoc->m_PlotStorage.AddPlotInfo(plotinfo, m_CurrentChart)==0)
										return;
						}
					}
	
				}
				else {
					pChart->MakeTextPlot("No Variables selected");
					return;
				}

				if(m_CurrentChart!=m_PreviousChart) {
					m_VarNo=0;
					m_PreviousChart=m_CurrentChart;
				}
				
			}
			else if(isheet==4) {
				fileName=((F*)Ptr)->GetStrValue().c_str();

				if(m_pSimDoc->m_SelectModelFile.size()<=0) return; 			

				for(int i=0; i<m_pSimDoc->m_SelectModelFile.size();i++) {
						/*Ptr=(F*)m_SelectedFPtr.GetAt(i);
						Index=m_SelectedFIndex.GetAt(i);
						Isubchart=m_SelectedChartPGIndex.GetAt(i);*/
						PLOTv plotv=m_pPlotInfo->MakeModelFileChartInfo(i);

						if(!m_pSimDoc->m_PlotStorage.AddPlotInfo(plotv, m_CurrentChart))
							return;
				}

	
				if(m_CurrentChart!=m_PreviousChart) {
					m_VarNo=0;
					m_PreviousChart=m_CurrentChart;
				}
			}
			else if(isheet==11) {
			
				if(m_pSimDoc->m_SelectPGFile.size()<=0) {
					pChart->MakeTextPlot("No Variables selected");			
					return; 			
				}
				int RepitNo;
				CPG *pPtr;
				for(int i=0; i<m_pSimDoc->m_SelectPGFile.size();i++) {
						pPtr=m_pSimDoc->m_SelectPGFile[i].pPG;
		
						PLOTv plotv=m_pPlotInfo->MakePGFileChartInfo(i);
						if(!m_pSimDoc->m_PlotStorage.AddPlotInfo(plotv, m_CurrentChart))
							return;
				}

				fileName=pPtr->GetFileName().c_str();

				if(m_CurrentChart!=m_PreviousChart) {
					m_VarNo=0;
					m_PreviousChart=m_CurrentChart;
				}


			}
			SetSheetNumber(isheet);
			
			COLORREF color;

			GetCell(col,row,&cell);
			if(CreateZoomPlotWindow(false, fileName)) {

				if(isheet==5) {
					long row;
					int count=1;
					UpdatePlotStatusBar(0);

				}
				else if(isheet==6) {
					long row;
					int count=1;
					int runno, runnoRow;
					if(m_ShowValidRes) {
						m_TobePlotted=false;
						QuickSetText(2,-1, "Pick for plotting");
						QuickSetBackColor(2,-1,m_ColorCompleted);
						RedrawCell(2,-1);
					}
					UpdatePlotStatusBar(1);
				}
				else if(isheet==11) {

					long row;
					int count=1;
					UpdatePlotStatusBar(2);

				}
				else if(isheet==4) {
					int count=1;
					int index;
					CPG *pCPG;
					F* pF;

					UpdatePlotStatusBar(3);



				}
				else {
					m_VarNo=pSimDoc->m_PlotStorage.GetNumVar(m_CurrentChart);
					color=MFC_Util::GetColorLine(m_VarNo);
					if(color==0)
						color=RGB(240,240,240);
				}
			}
			else {
				 m_pFrame->UpdatePaneText(0,"Problems when making time serie chart - File may be missing or currupt");

			}
			

			/*if(isheet!=5) {
				cell.SetBackColor(color);
				SetCell(col,row, &cell);
				RedrawCell(col,row);
			}*/




	}
}
bool MakeSimCharts::CalculateMeanOffTimeSeries(long item, int ival)
{
	CCoupModelApp* pApp;
	pApp = (CCoupModelApp*)AfxGetApp();

	string UserDirectory="";
	UserDirectory=MFC_Util::GetProfileStringStd("UserDirectory", UserDirectory);
	int numo=UserDirectory.size();
	if(UserDirectory.rfind('\\')!=numo-1) 
			UserDirectory+="\\";
	string NewFolder=m_pSimDoc->GetCurrentFile();
	int ipp=NewFolder.find('_');
	int ipm=NewFolder.find("_Multi");
	if(ipm>0) ipp=ipm;
	UserDirectory+=NewFolder.substr(ipp);
	UserDirectory+="\\";
	CFileScanner ScanMeanProg(NULL);
//	ScanMeanProg.Create(IDD_PROGRESS,NULL);
 //   ScanMeanProg.ShowWindow(SW_SHOW);

	if(item==5001||item==5003) {
		m_OutAllMulti_Defined[ival]=true;
		
		if (m_pSimDoc->MR_Storage_Open()&&m_pSimDoc->MR_XBin_IsOpen(ival)) {
			
			CResiduals *pRes=m_pSimDoc->GetValidResidualPointer(ival);
			if(!pRes->m_MeanDefined)
				AfxBeginThread(UpdateAcceptedThreadProc2,pRes);
			//if(pRes!=NULL) pRes->UpdateStatistics(false,&ScanMeanProg);
		}
	}
	else if(item==5002||item==5004) {
		m_OutSelectedMulti_Defined[ival]=true;
		if(m_pSimDoc->MR_Storage_Open()&&m_pSimDoc->MR_XBin_IsOpen(ival)) {
			CResiduals *pRes=m_pSimDoc->GetValidResidualPointer(ival);
			if(!pRes->m_MeanDefined)
				AfxBeginThread(UpdateAcceptedThreadProc2,pRes);
			//if(pRes!=NULL) pRes->UpdateStatistics(true,&ScanMeanProg);
		}
	}
	else if(item==1602) {

		m_OutMAPMulti_Defined=true;

	}
	return false;

}
void MakeSimCharts::CleanChartsMade() {
	if(!m_pChartCug->m_TimeSeriePlot.CleanPrevCharts()) ;
	m_pChartCug->m_ParameterPlot.Clean_ParameterPlot();
	m_pChartCug->m_ModelView.CleanModelPlot();
	m_pChartCug->m_TableView.CleanTablePlot();
	m_pChartCug->m_RealTime.CleanRealTimePlot();
}
MultiVAL MakeSimCharts::GetMultiValStruct_Sum(int row, int col, bool *Add, int Indep) {
	CUGCell cell, cell1;
     MultiVAL out;
	 int Index;
	 int LIndex;
		m_pSimDoc->m_PrevSheet=5;


			SIMB simb=GetMultiSumPointer(row);
			Index=GetMultiSumIndex(simb, col);


			int itype=8;  // IDENTIFIES Output Variable plot

			if(*Add) {
				out.Index=Index;
				out.row=row;
				out.col=col;
				out.Itype=itype;
				out.pBase=simb.pBase;
				out.LIndex=simb.TabIndex;
				out.subchart=m_pSimDoc->m_SelectMultiSum.size()+1;

				GetCell(out.col,out.row,&cell);
				
				if(Indep==0) {
					m_pSimDoc->m_SelectMultiSum.push_back(out);
					cell.SetBool(true);
					cell.SetBackColor(m_ColorEdit);
				}
				else if(Indep==1) {
				//	m_pSimDoc->m_SelectMultiSum_X.push_back(mul);
					cell.SetBackColor(m_ColorFirstIndep);
				}
				else if(Indep==2) {
					//m_pSimDoc->m_SelectMultiSum_Y.push_back(mul);
					cell.SetBackColor(m_ColorSecondIndep);
				}
				SetCell(out.col,out.row,&cell);
				RedrawCell(out.col,out.row);
				*Add=false;
			}
			else {
				while(m_pSimDoc->m_SelectMultiSum.size()>0) {
						out=m_pSimDoc->m_SelectMultiSum[0];
					//	if(out.col==col&&out.row==row) {
							GetCell(out.col,out.row,&cell);
							cell.SetBool(false);
							cell.SetBackColor(m_NoColor);
							SetCell(out.col,out.row,&cell);
							RedrawCell(out.col,out.row);
							m_pSimDoc->m_SelectMultiSum.erase(m_pSimDoc->m_SelectMultiSum.begin());
					//	}
				}
				*Add=true;
				out=GetBlankMultiValStruct();
			}



	return out;
}
MultiVAL MakeSimCharts::GetMultiValStruct_Val(int row, int col, bool *Add, int Indep) {
	CUGCell cell, cell1;
     MultiVAL out;
	 int nselV=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
	 int Index;
	 int itype;
		m_pSimDoc->m_PrevSheet=6;
			
			if(row==nselV&&nselV>0)
					return GetBlankMultiValStruct();
			else if(row>=nselV) {
					itype=9;
					if(col==10||col==11||col==14) 
						Index=GetMultiValSumIndex(row, col);
					else
						return GetBlankMultiValStruct();
			}
			else if(row<nselV) {
					itype=6;
					if(col>11&&col<14) 
						return GetBlankMultiValStruct();
					else
						Index=GetMultiValIndex(row, col);

					auto ValFileNo=m_pSimDoc->m_ValidationData.GetValFileNumber(row);
					auto ValFileIndex=-1;
					for(int jx=0;jx<=row;jx++) {
						if(m_pSimDoc->m_ValidationData.GetValFileNumber(row)==ValFileNo)
								ValFileIndex++;
					}					
			}
			if(*Add) {
				out.Index=Index;
				out.row=row;
				out.col=col;
				out.Itype=itype;
				out.subchart=m_pSimDoc->m_SelectMultiVal.size()+1;

				SIMB simb;
				if(row<nselV)
					simb=m_pSimDoc->m_ValidationData.GetSimPointer_TSV(row);
				else
					simb=m_pSimDoc->m_ValidationData.GetSimPointer_SV(row-nselV);

				out.pBase=simb.pBase;
				out.LIndex=simb.TabIndex;

				GetCell(out.col,out.row,&cell);
				if(Indep==0) {
					m_pSimDoc->m_SelectMultiVal.push_back(out);
					cell.SetBool(true);
					cell.SetBackColor(m_ColorEdit);
				}
				SetCell(out.col,out.row,&cell);
				RedrawCell(out.col,out.row);
				*Add=false;
			}
			else {
				while(m_pSimDoc->m_SelectMultiVal.size()>0) {
						out=m_pSimDoc->m_SelectMultiVal[0];
						GetCell(out.col,out.row,&cell);
						cell.SetBool(false);
						cell.SetBackColor(m_NoColor);
						SetCell(out.col,out.row,&cell);
						RedrawCell(out.col,out.row);
						m_pSimDoc->m_SelectMultiVal.erase(m_pSimDoc->m_SelectMultiVal.begin());
				}
				*Add=true;
				out=GetBlankMultiValStruct();
			}

			if(m_pProp->m_ChartType>1&&m_pProp->m_ChartType!=string::npos)
				m_CurrentChartType=m_pProp->m_ChartType;	

	return out;
}
MultiVAL MakeSimCharts::GetMultiValStruct_Par(int row, int col, bool *Add, int Indep) {
	CUGCell cell, cell1;
    MultiVAL out;
	m_pSimDoc->m_PrevSheet=10;
	out.Index=row;
	out.Itype=7;


	 int Index;
	 int LIndex;
		m_pSimDoc->m_PrevSheet=5;


		SIMB simb=GetMultiRunParPointer(row);
		Index=GetMultiRunParIndex(row);


			int itype=7;  // IDENTIFIES Output Variable plot

			if(*Add) {
				out.Index=Index;
				out.pBase=simb.pBase;
				out.LIndex=simb.TabIndex;
				out.Itype=itype;
				out.subchart=m_pSimDoc->m_SelectMultiPar.size()+1;
				out.row=row;
				out.col=col;
				//Index=GetMultiSumIndex(simb, col);

				m_pSimDoc->m_SelectMultiPar.push_back(out);
			}
			else {
				while(m_pSimDoc->m_SelectMultiPar.size()>0) {
						out=m_pSimDoc->m_SelectMultiPar[0];
						GetCell(out.col,out.row,&cell);
						cell.SetBool(false);
						cell.SetBackColor(m_NoColor);
						SetCell(out.col,out.row,&cell);
						RedrawCell(out.col,out.row);
						m_pSimDoc->m_SelectMultiPar.erase(m_pSimDoc->m_SelectMultiPar.begin());
				}
				*Add=true;
				out=GetBlankMultiValStruct();
			}




	if(m_pProp->m_ChartType>1)
		m_CurrentChartType=m_pProp->m_ChartType;	

	return out;
}
MultiVAL MakeSimCharts::GetBlankMultiValStruct() {
	MultiVAL out;
	out.Index=0;
	out.Itype=0;
	out.RunNo=0;
	out.subchart=0;
	out.row=0;
	out.col=0;
	return out;

}
