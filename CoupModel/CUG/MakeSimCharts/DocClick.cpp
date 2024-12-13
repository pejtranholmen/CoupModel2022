#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include "..\..\Util.h"
#include "..\..\NewBase\FileScanner.h"
#include "DocClick.h"
#include "..\ChartProperties\ChartCug.h"
#include "..\..\MainFrm.h"

#include "..\..\NewBase\HistoryList.h"
#include "..\..\Run\ChaParDuringRun.h"
#include "..\..\SP\SP_Retrieve.h"
#include "..\..\MultiRun\MultiRun.h"
#include "..\..\Validation\ValidationEvaluation.h"
#include "..\..\Validation\ValidationVariables.h"
#include "..\..\Validation\SelectWindowForValidationSummaryVariables.h"
#include "..\..\PG\CreatePG.h"
#include "..\ColourPickerDlg.h"
#include "..\..\MultiRun\MultiRunTable.h"

DocClick::DocClick(void)
{
	//m_pChartCug=MySingle::instance().GetChartCugPointer();
	m_CurrentTopRow=-1;
	m_pProp = MySingle::instance().GetChartPropPointer();
}
DocClick::~DocClick(void)
{
}
int DocClick::RunInfoClick(int col, int row) {
		
	return -1;
}
int DocClick::SwitchClick(int col, int row) {
	CUGCell cell;

	GetCell(0,row,&cell);
	string Group=cell.GetStdText();
	size_t iv = p_ModelInfo->GetGroupIndex(Group);
	if (iv == string::npos) return 1;
	if (m_pSimDoc->GetGroupEnable(iv))
		m_pSimDoc->SetGroupEnable(iv, false);
	else
		m_pSimDoc->SetGroupEnable(iv, true);
	m_pSimDoc->SetOriginalValues("Switches",Group);
	m_CurrentTopRow=GetTopRow();

	EnableUpdate(FALSE); 
	while(GetNumberRows()>0)
	DeleteRow(0);
	EnableUpdate(TRUE); 
	return 1;
}
int DocClick::ParameterClick(int col, int row) {
 CUGCell cell;
 string Name, Type, Group;
 SimB *pSw;
 if(col>=1&&row>=0) {
			GetCell(2,row, &cell);
			Name=cell.GetStdText();
			Type="Parameters";
					
					
			GetCell(0,row,&cell);
			Group=cell.GetStdText();
			pSw=m_pSimDoc->GetPtr(Type, Group, Name);
			if(pSw==nullptr) return -1;
			string GTitle;
			size_t ival=((Ps*)pSw)->GetSelectedFunc();
			GTitle=((Ps*)pSw)->GetViewTitle(ival);
			if(GTitle!="") Make_ParameterChart((Ps*)pSw,nullptr,string::npos,ival);		
 }
 else if(col==0&&row>=0) {
			GetCell(2,row, &cell);
			Name=cell.GetStdText();
			Type="Parameters";
					
					
			GetCell(0,row,&cell);
			Group=cell.GetStdText();
			pSw=m_pSimDoc->GetPtr(Type, Group, Name);
			MySingle::instance().GetCugPointer()->UpdateStatusBar(pSw, 1);

	
			m_CurrentTopRow=GetTopRow();
			size_t iv = p_ModelInfo->GetGroupIndex(Group);
			if (m_pSimDoc->GetGroupEnable(iv))
				m_pSimDoc->SetGroupEnable(iv, false);
			else
				m_pSimDoc->SetGroupEnable(iv, true);
			m_pSimDoc->SetOriginalValues("Parameters");
			EnableUpdate(FALSE); 
			while(GetNumberRows()>0)
			DeleteRow(0);
			EnableUpdate(TRUE); 
			return 2;

 }

	return -1;
}
int DocClick::ParameterTableClick(int col, int row) {
	CUGCell cell1, cell;
	string Name, Group, Type;
	P* pCP;
	Tab *pTab;
		GetCell(col,row, &cell); 
		int ParV=cell.GetParam();

		//cell.SetBackColor(RGB(0,255,255));
		//cell.SetText(message);
		SetCell(col,row,&cell); 

		if(ParV==1000) {
			GetCell(col,row, &cell1);
			Name=cell1.GetStdText();
			GetCell(col-2,row, &cell1);
			Group=cell1.GetStdText();
			Type="Tables";
			
			pTab=dynamic_cast<Tab*>(m_pSimDoc->GetPtr(Type, Group, Name));
			if(pTab!=nullptr) { //Make table Visible
				m_ClickedTable=Name;
				m_ClickedRow=row;
				if( pTab->GetHide()  ) 
					pTab->SetHide(false);
				else
					pTab->SetHide(true);
				long rowsheet=row-GetTopRow();
				EnableUpdate(FALSE); 
				while(GetNumberRows()>0)
						DeleteRow(0);
				EnableUpdate(TRUE); 
				if(m_ClickedRow>0) {
					m_CurrentTopRow=m_ClickedRow-rowsheet-1;
					SetTopRow(m_ClickedRow-rowsheet-1);
				}
				return 3;

			//	if(m_ClickedRow>0)
					//SetTopRow(m_ClickedRow-rowsheet-1);
			}
		}
		else if(ParV>=2000) {
			
			int index;
			index=ParV-2000;

				GetCell(2,row-index-2,&cell);
				Name=cell.GetStdText();
				GetCell(0,row-index-2,&cell);
				Group=cell.GetStdText();
				Type="Tables";
			
				pTab=dynamic_cast<Tab*>(m_pSimDoc->GetPtr(Type, Group, Name));

				
				GetCell(col,row-index-1,&cell);
				Name=cell.GetStdText();
				Type="Table Parameters";
				pCP=dynamic_cast<P*>(m_pSimDoc->GetPtr(Type, Group, Name));


				if(pTab!=nullptr&&pCP!=nullptr) {
						if(pCP->GetViewFunction()>0)
							Make_ParameterChart(pCP, pTab, index);
				}
		}

	return -1;

}
int DocClick::ModelFileClick(int col, int row) {
	CUGCell cell1, cell;
	string Name, Group, Type;
	SimB *pSw;
	GetCell(col,row, &cell); 
	int ParV=cell.GetParam();

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
			return 4;
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
			return 4;
		}
	}
	else if (ParV >= 1100 && ParV < 1110) {
		// This is the CSV File Pointers.
		size_t index = ParV - 1100;
		if (m_pSimDoc->m_FCSV_Array.size() > index) {


			bool value = m_pSimDoc->m_FCSV_Array[index]->GetHide();
			if (value)
				value = false;
			else
				value = true;

			m_pSimDoc->m_FCSV_Array[index]->SetHide(value);


			return 4;
		}





	}
	else if(ParV==2000) {

		int Index=0;
			
		GetCell(-1,row,&cell);
		Index=int(cell.GetNumber());		


			GetCell(1,row-Index-1, &cell1);
			Name=cell1.GetStdText();
			GetCell(0,row-Index-1, &cell1);
			Group=cell1.GetStdText();
			Type="Files";
				
			pSw=m_pSimDoc->GetPtr(Type, Group, Name);

			if(pSw!=nullptr) { 
				if(m_pSimDoc->m_SelectModelFile.size()==0) {

				}
				CPG *pCPG;
				int index;
				F* pF;
				while(m_pSimDoc->m_SelectModelFile.size()>0) {	
					//color=MFC_Util::GetColorLine(count);
					//count++;
					pF=m_pSimDoc->m_SelectModelFile[0].pF;
					pCPG=(CPG*)pF->GetPointer();
					index=m_pSimDoc->m_SelectModelFile[0].PGIndex;
					pCPG->SetPicked(index,false);
					//pCPG->SetColor(index,color);
					m_pSimDoc->m_SelectModelFile.erase(m_pSimDoc->m_SelectModelFile.begin());
					//m_SelectedFIndex.RemoveAt(0);
					//m_SelectedFPtr.RemoveAt(0);
					//m_SelectedChartPGIndex.RemoveAt(0);
				}


				if(m_pSimDoc->m_SelectModelFile.size()>0)
					TimeSerieChartClick(0, row, 4, pSw, Index, 0, Type);	
				EnableUpdate(FALSE); 
				while(GetNumberRows()>0)
					DeleteRow(0);
				EnableUpdate(TRUE);
				return 4;
			}


	}
	else if (ParV == 2100 || (col == 4)) {
		int Index = 0;

		GetCell(-1, row, &cell);
		Index = int(cell.GetNumber());


		GetCell(1, row - Index - 1, &cell1);
		Name = cell1.GetStdText();
		GetCell(0, row - Index - 1, &cell1);
		Group = cell1.GetStdText();
		Type = "Files";
		SelectPG selpg;

		pSw = m_pSimDoc->GetPtr(Type, Group, Name);
		if (pSw == nullptr) return -1;
		CPG* pCPG = (CPG*)((F*)pSw)->GetPointer();
		if (col == 3) {

			GetCell(3, row, &cell);

			if (cell.GetBool()) {
				selpg.pF = dynamic_cast<F*>(pSw);
				selpg.PGIndex = Index;
				//m_SelectedFPtr.Add(pSw);
				//m_SelectedFIndex.Add(Index);
				if (m_pSimDoc->m_SelectModelFile.size() == 1)
					m_CurrentSubChart = 1;
				else if (m_CurrentSubChart < m_pProp->m_NumSubCharts)
					m_CurrentSubChart++;
				selpg.subchart = m_CurrentSubChart;
				//m_SelectedChartPGIndex.Add(m_CurrentSubChart);
				pCPG->SetPicked(Index, TRUE);
				QuickSetNumber(4, row, m_CurrentSubChart);
				m_pSimDoc->m_SelectModelFile.push_back(selpg);

			}
			else {
				for (int i = 0; i < m_pSimDoc->m_SelectModelFile.size(); i++)
				{
					if (pSw == m_pSimDoc->m_SelectModelFile[i].pF&&Index == m_pSimDoc->m_SelectModelFile[i].PGIndex) {
						m_pSimDoc->m_SelectModelFile.erase(m_pSimDoc->m_SelectModelFile.begin() + i);
						QuickSetText(4, row, "");
						if (pCPG != nullptr)
							pCPG->SetPicked(Index, FALSE);

					}
				}
			}
			if (m_pSimDoc->m_SelectModelFile.size()>0)
				TimeSerieChartClick(0, row, 4, pSw, Index, 0, Type);
		}
		else if (col == 4) {
			int itp; int ichart;
			GetCell(4, row, &cell);
			ichart = int(cell.GetNumber());
			ichart++;
			ichart = (ichart - 1) % m_pProp->m_NumSubCharts + 1;
			QuickSetNumber(4, row, ichart);
			m_CurrentSubChart = ichart;
			itp = 0;
			while (m_pSimDoc->m_SelectModelFile[itp].pF != pSw || m_pSimDoc->m_SelectModelFile[itp].PGIndex != Index) {
				if (itp < m_pSimDoc->m_SelectModelFile.size() - 1)
					itp++;
				else
					break;
			};
			if (itp < m_pSimDoc->m_SelectModelFile.size())
				m_pSimDoc->m_SelectModelFile[itp].subchart = m_CurrentSubChart;
			if (m_pSimDoc->m_SelectModelFile.size()>0)
				TimeSerieChartClick(0, row, 4, pSw, Index, 0, Type);


		}
		RedrawCell(4, row);
		UpdatePlotStatusBar(3);
	}
	else if (ParV >= 2200 && ParV < 2204)
		return -1;

	return -1;
}
int DocClick::OutPutVarClick(int col, int row) {
	CUGCell cell, cell1;
	string Group, Name, Type;
	SimB *pSw;
	int isheet=GetSheetNumber();

	if(col==1) {
		GetCell(1,row,&cell);
		Group=cell.GetStdText();
		m_ShowAllOutputs=FALSE;
		m_pSimDoc->RemoveOriginalValues("Outputs",Group,false, m_ViewCategory);
		size_t iv=p_ModelInfo->GetGroupIndex(Group);
		if(m_pSimDoc->GetGroupEnable(iv))
			m_pSimDoc->SetGroupEnable(iv, false);
		else
			m_pSimDoc->SetGroupEnable(iv, true);
		m_CurrentTopRow=GetTopRow();

		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
		DeleteRow(0);
		EnableUpdate(TRUE); 
		return 5;
	}
	else if(col>=3&&col<=5) {
		m_CurrentTopRow=GetTopRow();
		GetCell(3,row, &cell);

		if(m_pSimDoc->IsFinishedSimulation())
			Name=cell.GetStdLabelText();
		else
			Name=cell.GetStdLabelText();
		int ParV=cell.GetParam();
		int pos1=Name.rfind('(');
		int pos2=Name.rfind(')');
		int Index;
		int selected;
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
		
		GetCell(1,row, &cell);
		Group=cell.GetStdText();
		GetCell(0,row, &cell);
		Type=cell.GetStdText();;
		

		pSw=(SimB*)m_pSimDoc->GetPtr(Type,Group,Name);
		pSw=m_pSimDoc->GetPtr(Type, Group, Name);
		if(pSw!=nullptr) {
			int ityp=p_ModelInfo->GetTypeIndex(Type);
			if(pSw->Is_Vector())
				selected=((OutVector*)pSw)->GetStoreFlag(Index-1);
			else
				selected=((OutSingle*)pSw)->GetStoreFlag();

			/*if(ityp>5 )
				((G*)pSw)->GetFlag(Index-1, &selected); 
			else
				((X*)pSw)->GetFlag(Index-1, &selected);*/
		}
		if(m_pSimDoc->IsFinishedSimulation()&&pSw!=nullptr){
			int ityp=p_ModelInfo->GetTypeIndex(Type);
			if(m_pSimDoc->IsOutputBinFileValid()||selected==2)
				if(col==3||col==4) {
						GetCell(col,row,&cell);
						SelectOUT out;
						bool wasmade=false;
						for( int i=0; i<m_pSimDoc->m_SelectOut.size();i++) {	
							out=m_pSimDoc->m_SelectOut[i];
							if(row==out.row&&out.MeanIndicator== ENSEMBLE_TYPE::NO&&col==3||row==out.row&&out.MeanIndicator>ENSEMBLE_TYPE::NO&&col==4) {
								m_pSimDoc->m_SelectOut.erase(m_pSimDoc->m_SelectOut.begin()+i);
								cell.SetBool(FALSE);
								cell.SetBackColor(m_NoColor);
								SetCell(col,row,&cell);

								RedrawCell(col,row);

	
								QuickSetNumber(5,row,0);
								QuickSetText(5,row,"");
								RedrawCell(5,row);
								wasmade=true;
							}
						}
						if(!wasmade) {
							SelectOUT out;
							out.pBase=pSw;
							out.LIndex=Index;
							out.row=row;
							if(col==3)
								out.MeanIndicator= ENSEMBLE_TYPE::NO;
							else {
							//	if(m_pSimDoc->GetNumValidValFiles()>0||)
								//	out.MeanIndicator=2;
								//else
									out.MeanIndicator= ENSEMBLE_TYPE::ACCEPTED;

							}

							cell.SetBool(TRUE);
							SetCell(col,row,&cell);
							RedrawCell(col,row);
							out.IType=ityp-3;
					
							m_pProp->UpdateNumSubcharts();
							if(m_pSimDoc->m_SelectOut.size()==0)
									m_CurrentSubChart=1;
							else if(m_CurrentSubChart<m_pProp->m_NumSubCharts&&m_pSimDoc->m_SelectOut.back().pBase!=pSw)
									m_CurrentSubChart++;
							out.subchart=m_CurrentSubChart;
							m_pSimDoc->m_SelectOut.push_back(out);
						//	m_SelectedChartIndex.Add(m_CurrentSubChart);
							QuickSetNumber(5,row,m_CurrentSubChart);
							RedrawCell(5,row);

						}
					/*	else {
							SelectOUT out;
							for( int i=0; i<m_pSimDoc->m_SelectOut.size();i++)
							{	
								out=m_pSimDoc->m_SelectOut[i];
								if(row==out.row) {
									m_pSimDoc->m_SelectOut.erase(m_pSimDoc->m_SelectOut.begin()+i);

									QuickSetNumber(4,row,0);
									QuickSetText(4,row,"");
									RedrawCell(4,row);
								}
							}
						}*/
						UpdatePlotStatusBar(0);
		

						TimeSerieChartClick(col, row, isheet, pSw, Index, 0, Type);
						return -1;
						/*SetCell(3,row,&cell);
						RedrawCell(3,row);*/


				}
				else if(col==5) {
							m_pProp->UpdateNumSubcharts();
							int ichart;
							GetCell(5,row, &cell1);
							ichart=int(cell1.GetNumber());
							if(ichart>0) {
								ichart++;
								ichart=(ichart-1)%m_pProp->m_NumSubCharts+1;
								QuickSetNumber(5,row, ichart);
								m_CurrentSubChart=ichart;
								Index=0;
								while(m_pSimDoc->m_SelectOut[Index].row!=row) {
									if(Index<m_pSimDoc->m_SelectOut.size()-1)
										Index++;
									else
										break;

								}
								if(Index<m_pSimDoc->m_SelectOut.size())
									m_pSimDoc->m_SelectOut[Index].subchart=m_CurrentSubChart;
									//m_SelectedChartIndex.SetAt(Index,m_CurrentSubChart);
								RedrawCell(5,row);
								TimeSerieChartClick(col, row, isheet, pSw, Index, 0, Type);
							}
							return -1;


				}
				else if(m_pSimDoc->m_SelectOut.size()>0) {
					long row;
				//	int count=1;
					while(m_pSimDoc->m_SelectOut.size()>0) {
						row=m_pSimDoc->m_SelectOut[0].row;
						GetCell(4,row,&cell);
						cell.SetBool(FALSE);
						SetCell(4,row,&cell);
						RedrawCell(4,row);
						QuickSetNumber(5,row,0);
						QuickSetText(5,row,"");
					//	QuickSetText(5,row,"");
					//	GetCell(2,row,&cell);
					//	color=MFC_Util::GetColorLine(count);
					//	count++;
					//	cell.SetBackColor(color);
					//	SetCell(2,row,&cell);
						RedrawCell(5,row);
						m_pSimDoc->m_SelectOut.erase(m_pSimDoc->m_SelectOut.begin());

					}

					TimeSerieChartClick(col, row, isheet, pSw, Index, 0, Type);
					return 5;
				}
	

		}
		return -1;

	}
	else if(col==6) {
		if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiSimulation()&&m_pSimDoc->IsMultiStorageBinFileValid()) {
			GetCell(col,row, &cell);
			MultiVAL outsum;
			bool wasmade=false;
			for( int i=0; i<m_pSimDoc->m_SelectMultiSum.size();i++) {	
				outsum=m_pSimDoc->m_SelectMultiSum[i];
				if(row==outsum.row&&col==outsum.col) {
					m_pSimDoc->m_SelectMultiSum.erase(m_pSimDoc->m_SelectMultiSum.begin()+i);
					cell.SetBool(FALSE);
					SetCell(col,row,&cell);
					RedrawCell(col,row);
					wasmade=true;
				}
			}

			if(!wasmade) {
				cell.SetBool(TRUE);
				SetCell(col,row,&cell);
				RedrawCell(col,row);
				MultiPlotClick(5,col, row, TRUE);
			}
			else
				MultiPlotClick(5,col, row, FALSE);

			UpdatePlotStatusBar(5);
		}
	}
	else if(col>=7) {
		pSw=nullptr;
		//int nselV=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
		if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiSimulation()) {
			GetCell(col,row, &cell);
			if(cell.GetBool()) {
				bool picked;
				picked=false;
				// Remove from list if in list
				for(size_t i=0;i<m_pSimDoc->m_SelectMultiSum.size();i++){
					if(col==m_pSimDoc->m_SelectMultiSum[i].col&&row==m_pSimDoc->m_SelectMultiSum[i].row) {
						m_pSimDoc->m_SelectMultiSum.erase(m_pSimDoc->m_SelectMultiSum.begin()+i);
						picked=true;
					}
				}

				if(!picked)
					MultiPlotClick(5,col, row, TRUE);
				else {
					cell.SetBool(FALSE);
					cell.SetBackColor(m_NoColor);
					SetCell(col,row,&cell);
					RedrawCell(col,row);
					MultiPlotClick(5,col, row, FALSE);
				}
			}
			else {
				bool picked;
				picked=false;
				// Remove from list if in list
				for(size_t i=0;i<m_pSimDoc->m_SelectMultiVal.size();i++){
					if(col==m_pSimDoc->m_SelectMultiVal[i].col&&row==m_pSimDoc->m_SelectMultiVal[i].row) {
						m_pSimDoc->m_SelectMultiVal.erase(m_pSimDoc->m_SelectMultiVal.begin()+i);
						picked=true;
						MultiPlotClick(5,col, row, FALSE);
					}
				}
				if(!picked) {
					MultiPlotClick(5,col, row, TRUE);
					cell.SetBool(TRUE);
					SetCell(col,row,&cell);
					RedrawCell(col,row);
				}
			}
			UpdatePlotStatusBar(5);
		
		}
		else {
		// Change Value Errors
		

		}




	}




	
	return -1;
}
int DocClick::OutPutVarSelectionClicktoStore(int col, int row) {
	CUGCell cell, cell1;
	string Group, Name, Type;
	SimB *pSw;
	int isheet = GetSheetNumber();

	if (col == 1) {
		GetCell(1, row, &cell);
		Group = cell.GetStdText();
		m_ShowAllOutputs = FALSE;
		m_pSimDoc->RemoveOriginalValues("Outputs", Group, true, m_ViewCategory);
		size_t iv = p_ModelInfo->GetGroupIndex(Group);
		if (m_pSimDoc->GetGroupEnable(iv))
			m_pSimDoc->SetGroupEnable(iv, false);
		else
			m_pSimDoc->SetGroupEnable(iv, true);
		m_CurrentTopRow = GetTopRow();

		EnableUpdate(FALSE);
		while (GetNumberRows() > 0)
			DeleteRow(0);
		EnableUpdate(TRUE);
		return 5;
	}
	else if (col >= 3 && col <= 5) {
		m_CurrentTopRow = GetTopRow();
		GetCell(3, row, &cell);

		if (m_pSimDoc->IsFinishedSimulation())
			Name = cell.GetStdLabelText();
		else
			Name = cell.GetStdLabelText();
		int ParV = cell.GetParam();
		int pos1 = Name.rfind('(');
		int pos2 = Name.rfind(')');
		int Index;
		int selected;
		if (pos2 > pos1&&pos2 == Name.size() - 1) {
			string t = Name.substr(pos1 + 1, pos2 - pos1 - 1);
			if (t != "no roots") {
				Index = FUtil::AtoInt(t);
				Name = Name.substr(0, pos1);
			}
			else
				Index = 1;

		}
		else
			Index = 1;

		GetCell(1, row, &cell);
		Group = cell.GetStdText();
		GetCell(0, row, &cell);
		Type = cell.GetStdText();;


		pSw = (SimB*)m_pSimDoc->GetPtr(Type, Group, Name);
		pSw = m_pSimDoc->GetPtr(Type, Group, Name);
		if (pSw != nullptr) {
			int ityp = p_ModelInfo->GetTypeIndex(Type);
			if (pSw->Is_Vector())
				selected = ((OutVector*)pSw)->GetStoreFlag(Index - 1);
			else
				selected = ((OutSingle*)pSw)->GetStoreFlag();

			/*if(ityp>5 )
				((G*)pSw)->GetFlag(Index-1, &selected);
			else
				((X*)pSw)->GetFlag(Index-1, &selected);*/
		}
		if (pSw != nullptr && (pSw->GetType() == STATE || pSw->GetType() == FLOW || pSw->GetType() == AUX || pSw->GetType() == DRIVE)) {
			int ityp = p_ModelInfo->GetTypeIndex(Type);
			size_t index = Index - 1;
			int sel = ((OutVector*)pSw)->GetStoreFlag(index);
			if (sel != 2 && col == 4) {
				((OutVector*)pSw)->SetStoreFlag(index, 2);
				//((OutVector*)pSw)->SetMultiStoreFlag(index, true);
				selected = 2;
			}
			else if (sel == 2 && col == 4) {
				((OutVector*)pSw)->SetStoreFlag(index, 1);
				//((OutVector*)pSw)->SetMultiStoreFlag(index, false);
				selected = 1;
			}
			else if (sel == 0 && col == 3) {
				//    ((OutVector*)pSw)->SetMultiStoreFlag(index, false);
				((OutVector*)pSw)->SetStoreFlag(index, 1);
				selected = 1;
			}
			else if (sel != 0 && col == 3) {
				((OutVector*)pSw)->SetStoreFlag(index, 0);
				selected = 0;
			}
			m_pSimDoc->TimeModified();
			GetCell(col, row, &cell);
			if (selected == 0)
				cell.SetBackColor(RGB(255, 255, 255));
			else if (selected == 1)
				cell.SetBackColor(RGB(40, 240, 240));
			else if (selected == 2)
				cell.SetBackColor(RGB(0, 100, 100));
			SetCell(col, row, &cell);
			RedrawCell(col, row);
			GotoCell(col, row + 1);
			return -1;
			//return 5;

		}
		else if (pSw != nullptr && (pSw->GetType() == STATE_SINGLE || pSw->GetType() == FLOW_SINGLE || pSw->GetType() == AUX_SINGLE || pSw->GetType() == DRIVE_SINGLE)) {
			int ityp = p_ModelInfo->GetTypeIndex(Type);

			int sel = ((OutSingle*)pSw)->GetStoreFlag();
			if (sel != 2 && col == 4) {
				((OutSingle*)pSw)->SetStoreFlag(2);
				selected = 2;
			}
			else if (sel == 2 && col == 4) {
				((OutSingle*)pSw)->SetStoreFlag(1);
				selected = 1;
			}
			else if (sel == 0 && col == 3) {
				((OutSingle*)pSw)->SetStoreFlag(1);
				selected = 1;
			}
			else if (sel != 0 && col == 3) {
				((OutSingle*)pSw)->SetStoreFlag(0);
				selected = 0;
			}


			m_pSimDoc->TimeModified();
			GetCell(col, row, &cell);
			if (selected == 0)
				cell.SetBackColor(RGB(255, 255, 255));
			else if (selected == 1)
				cell.SetBackColor(RGB(40, 240, 240));
			else if (selected == 2)
				cell.SetBackColor(RGB(0, 100, 100));
			SetCell(col, row, &cell);
			GotoCell(col, row + 1);
			RedrawCell(col, row);
			return -1;
		}


		return -1;

	}
	else if (col == 6) {
		if (m_pSimDoc->IsFinishedSimulation() && m_pSimDoc->IsMultiSimulation() && m_pSimDoc->IsMultiStorageBinFileValid()) {
			GetCell(col, row, &cell);
			MultiVAL outsum;
			bool wasmade = false;
			for (int i = 0; i < m_pSimDoc->m_SelectMultiSum.size(); i++) {
				outsum = m_pSimDoc->m_SelectMultiSum[i];
				if (row == outsum.row&&col == outsum.col) {
					m_pSimDoc->m_SelectMultiSum.erase(m_pSimDoc->m_SelectMultiSum.begin() + i);
					cell.SetBool(FALSE);
					SetCell(col, row, &cell);
					RedrawCell(col, row);
					wasmade = true;
				}
			}

			if (!wasmade) {
				cell.SetBool(TRUE);
				SetCell(col, row, &cell);
				RedrawCell(col, row);
				MultiPlotClick(5, col, row, TRUE);
			}
			else
				MultiPlotClick(5, col, row, FALSE);

			UpdatePlotStatusBar(5);
		}
	}
	else if (col >= 7) {
		pSw = nullptr;
		//int nselV=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
		if (m_pSimDoc->IsFinishedSimulation() && m_pSimDoc->IsMultiSimulation()) {
			GetCell(col, row, &cell);
			if (cell.GetBool()) {
				bool picked;
				picked = false;
				// Remove from list if in list
				for (size_t i = 0; i < m_pSimDoc->m_SelectMultiSum.size(); i++) {
					if (col == m_pSimDoc->m_SelectMultiSum[i].col&&row == m_pSimDoc->m_SelectMultiSum[i].row) {
						m_pSimDoc->m_SelectMultiSum.erase(m_pSimDoc->m_SelectMultiSum.begin() + i);
						picked = true;
					}
				}

				if (!picked)
					MultiPlotClick(5, col, row, TRUE);
				else {
					cell.SetBool(FALSE);
					cell.SetBackColor(m_NoColor);
					SetCell(col, row, &cell);
					RedrawCell(col, row);
					MultiPlotClick(5, col, row, FALSE);
				}
			}
			else {
				bool picked;
				picked = false;
				// Remove from list if in list
				for (size_t i = 0; i < m_pSimDoc->m_SelectMultiVal.size(); i++) {
					if (col == m_pSimDoc->m_SelectMultiVal[i].col&&row == m_pSimDoc->m_SelectMultiVal[i].row) {
						m_pSimDoc->m_SelectMultiVal.erase(m_pSimDoc->m_SelectMultiVal.begin() + i);
						picked = true;
						MultiPlotClick(5, col, row, FALSE);
					}
				}
				if (!picked) {
					MultiPlotClick(5, col, row, TRUE);
					cell.SetBool(TRUE);
					SetCell(col, row, &cell);
					RedrawCell(col, row);
				}
			}
			UpdatePlotStatusBar(5);

		}
		else {
			// Change Value Errors


		}




	}





	return -1;
}
int DocClick::ValidationVarClick(int col, int row, bool All) {
	CUGCell cell, cell1;
	string Name, Type, Group;
	Sw *pSw;
	int isheet=GetSheetNumber();
	GetCell(col,row, &cell);
	Name=cell.GetStdText();
	int pos1=Name.rfind('(');
	int pos2=Name.rfind(')');

	int ValFileNo=1;



	pSw=nullptr;

	if(row<0) return -1;

	if(col>0&&col<=5) {
		if(m_pSimDoc->IsFinishedSimulation()) {
			SelectVAL val;
			int VariableType=0;
			if(m_pSimDoc->IsOutputValidBinFileValid()){
				val.col=col; val.row=row;
				if(col==1) {   //Measured Only
						GetCell(1,row,&cell);
						val.MeanIndicator=ENSEMBLE_TYPE::NO;

						if(cell.GetBool()) {

							int iv=m_pSimDoc->m_SelectValTime.size();
						}
						 
						if(cell.GetBool()&&m_pSimDoc->m_SelectValTime.size()<=0) {
							if(m_pSimDoc->m_SelectValTime.size()==0)
									m_CurrentSubChart=1;
							else if(m_CurrentSubChart<m_pProp->m_NumSubCharts&&m_pSimDoc->m_SelectValTime.size()%2==0)
									m_CurrentSubChart++;
							val.row=row;
							val.RunNo=0;
							val.RunNoRow=row;
							val.MeanIndicator= ENSEMBLE_TYPE::NO;
							val.subchart=m_CurrentSubChart;
							m_pSimDoc->m_SelectValTime.push_back(val);
							cell.SetBool(TRUE);
							QuickSetNumber(5,row,m_CurrentSubChart);
							RedrawCell(5,row);
						}
						else {
							for( int i=0; i<m_pSimDoc->m_SelectValTime.size();i++) {
								if(row==m_pSimDoc->m_SelectValTime[i].row&&col==m_pSimDoc->m_SelectValTime[i].col) {
									m_pSimDoc->m_SelectValTime.erase(m_pSimDoc->m_SelectValTime.begin()+i);
									QuickSetNumber(5,row,0);
									QuickSetText(5,row,"");
									RedrawCell(5,row);
								}
							}
						}
						VariableType=5;
						if(m_pSimDoc->m_SelectValTime.size()>0)
							TimeSerieChartClick(col, row, isheet, pSw, 0, VariableType, "", ValFileNo);	
						UpdatePlotStatusBar(1);



				}
				else if(col==3) { // Simulated only


						val.MeanIndicator= ENSEMBLE_TYPE::NO;
						if(cell.GetBool()) {

							int iv=m_pSimDoc->m_SelectValTime.size();


						}
						 
						if(cell.GetBool()) {
							if(m_pSimDoc->m_SelectValTime.size()==0)
									m_CurrentSubChart=1;
							else if(m_CurrentSubChart<m_pProp->m_NumSubCharts&&m_pSimDoc->m_SelectValTime.size() % 2 == 0)
									m_CurrentSubChart++;
							val.RunNo=0;
							val.RunNoRow=row;
							val.subchart=m_CurrentSubChart;
							val.MeanIndicator=ENSEMBLE_TYPE::NO;
							m_pSimDoc->m_SelectValTime.push_back(val);

							//cell.SetBool(TRUE);
							//SetCell(3,row, &cell);

							QuickSetNumber(5,row,m_CurrentSubChart);
							RedrawCell(5,row);
						}
						else {
							for( int i=0; i<m_pSimDoc->m_SelectValTime.size();i++) {
								if(row==m_pSimDoc->m_SelectValTime[i].row&&col==m_pSimDoc->m_SelectValTime[i].col) {
									m_pSimDoc->m_SelectValTime.erase(m_pSimDoc->m_SelectValTime.begin()+i);
									QuickSetNumber(5,row,0);
									QuickSetText(5,row,"");
									RedrawCell(5,row);
								}
							}


						}
						if (m_pSimDoc->m_SelectValTime.size() >= 0) {
							m_pProp->m_ChartType = CHART_TYPES::TIME_SERIE;
							TimeSerieChartClick(col, row, isheet, pSw, 0, int(val.MeanIndicator), "", ValFileNo);
						}
						UpdatePlotStatusBar(1);

				
				
				}
				else if(col==2) {  //Clear all info
					long row;
					int count=1;
					int runno, runnoRow;
					COLORREF color;
					while(m_pSimDoc->m_SelectValTime.size()>0) {
						row=m_pSimDoc->m_SelectValTime[0].row;
						runno=m_pSimDoc->m_SelectValTime[0].RunNo;
						runnoRow=m_pSimDoc->m_SelectValTime[0].RunNoRow;
						if(m_ShowValidRes) {
							/*if(runno==140000) 
								GetCell(2+row,0,&cell);
							else if(runno==120000) 
								GetCell(2+row,6,&cell);
							else*/ 
								GetCell(2+row,runnoRow,&cell);
							cell.SetBool(FALSE);
							color=MFC_Util::GetColorLine(count);
							count++;
							cell.SetBackColor(color);
							/*if(runno==120000) {
								SetCell(2+row,6,&cell);
								RedrawCell(2+row,6);
							}
							else if(runno==140000) {
								SetCell(2+row,0,&cell);
								RedrawCell(2+row,0);
							}
							else {*/
								SetCell(2+row,runnoRow,&cell);
								RedrawCell(2+row,runnoRow);
							//}
						}
						else {
							GetCell(4,row,&cell);
							cell.SetBool(FALSE);
							SetCell(4,row,&cell);
							RedrawCell(4,row);
							QuickSetNumber(5,row,0);
							QuickSetText(5,row,"");
							RedrawCell(5,row);
						}
						m_pSimDoc->m_SelectValTime.erase(m_pSimDoc->m_SelectValTime.begin());
					}



					//if(m_SelectedValRow.GetSize()>0)
					//	TimeSerieChartClick(col, row, isheet, pSw, 0, 0, "", ValFileNo);	
				}
				else if(col==4&&m_pSimDoc->IsMultiSimulation()) { //Change SubChart
						GetCell(4,row,&cell);
						val.MeanIndicator= ENSEMBLE_TYPE::NO;

						if(cell.GetBool()) {

							int iv=m_pSimDoc->m_SelectValTime.size();

						}
						auto FileNo = m_pSimDoc->m_ValidationData.GetValFileNumber(row);
						CResiduals* pRes = m_pSimDoc->GetValidResidualPointer(FileNo);

						if (!pRes->m_MeanDefined) {
							MFC_Util::MessageBox("Mean value not estimated", "Right click to select action",0);
							return -1;

						}



						if(cell.GetBool()||All|| m_pSimDoc->m_SelectValTime.size() == 0) {
							if(m_pSimDoc->m_SelectValTime.size()==0)
									m_CurrentSubChart=1;
							else if(m_CurrentSubChart<m_pProp->m_NumSubCharts&&m_pSimDoc->m_SelectValTime.size() % 2 == 0)
									m_CurrentSubChart++;
							val.RunNo=110000;
							val.MeanIndicator= ENSEMBLE_TYPE::ACCEPTED;
							if(All) val.MeanIndicator= ENSEMBLE_TYPE::ALL;
							val.RunNoRow=row;
							val.col=col;
							val.subchart=m_CurrentSubChart;
							m_pSimDoc->m_SelectValTime.push_back(val);
							cell.SetBool(TRUE);
							SetCell(4,row,&cell);
							RedrawCell(4,row);
							QuickSetNumber(5,row,m_CurrentSubChart);
							RedrawCell(5,row);
						}
						else {
							for( int i=0; i<m_pSimDoc->m_SelectValTime.size();i++) {
								if(row==m_pSimDoc->m_SelectValTime[i].row) {
									QuickSetNumber(5,row,0);
									QuickSetText(5,row,"");
									RedrawCell(5,row);
									col=m_pSimDoc->m_SelectValTime[i].col;
									GetCell(col,row,&cell);
									cell.SetBool(FALSE);
									SetCell(col,row,&cell);
									RedrawCell(col,row);
									m_pSimDoc->m_SelectValTime.erase(m_pSimDoc->m_SelectValTime.begin()+i);

								}
							}
						}
						m_pProp->m_ChartType = CHART_TYPES::TIME_SERIE;
						if(All) val.MeanIndicator= ENSEMBLE_TYPE::ALL;
						TimeSerieChartClick(col, row, isheet, pSw,0, int(val.MeanIndicator),"", ValFileNo);	
						UpdatePlotStatusBar(1);
					return -1;
				}
				else if(col==5) { //Update subchart Number
					m_pProp->m_ChartType = CHART_TYPES::TIME_SERIE;
					int iv=0;
					GetCell(5,row,&cell);
					string s=cell.GetStdText();
					if(m_pSimDoc->m_SelectValTime.size()<=0||s.size()<=0) return -1;
					while (m_pSimDoc->m_SelectValTime[iv].row!=row) {
						if(iv<m_pSimDoc->m_SelectValTime.size()-1)
							iv++;
					}
					if(iv<m_pSimDoc->m_SelectValTime.size()) {
						int ichart=m_pSimDoc->m_SelectValTime[iv].subchart;
						ichart++;
						ichart=(ichart-1)%m_pProp->m_NumSubCharts+1;
						m_pSimDoc->m_SelectValTime[iv].subchart=ichart;
						m_CurrentSubChart=ichart;
						QuickSetNumber(5,row,m_CurrentSubChart);
						RedrawCell(5,row);
					}
					if(m_pSimDoc->m_SelectValTime.size()>0)
						TimeSerieChartClick(col, row, isheet, pSw, 0, 0, "", ValFileNo);	
				}
			}
		}
		else {
		// Change Value Errors
			GetCell(col,row,&cell);
			if(cell.GetParam()==601) {
					int fileDlgResult;
					string pszFileName ;
					LPCTSTR lpszDefExt=_T("bin");
					LPCTSTR lpszFilter = _T("");
		//The next line is only neccessary as long as the PGP does not handle long filenames!
		//PEJ Excluded info box
					CString szFilter = _T("PG Bin file (*.BIN)|*.BIN||");
					CFileDialog	BinFileDlg(TRUE,lpszDefExt,lpszFilter,OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,szFilter);
		//Show the dialog
					fileDlgResult=BinFileDlg.DoModal();
		//If the cancel button has been pushed then return
					if (fileDlgResult==IDOK) {
						pszFileName = CT2A(BinFileDlg.GetPathName()).m_psz;
						F *pCF=m_pSimDoc->ValidationFilePointer(ValFileNo);
						pCF->SetValue(string(pszFileName));
						pCF->Apply();
						if(pCF->CheckFileNameAndAssignNameToPGClass()){ 
							m_pSimDoc->TimeModified();
							EnableUpdate(FALSE); 
							while(GetNumberRows()>0)
								DeleteRow(0);
							EnableUpdate(TRUE); 
							OnSheetSetup(6);
						}
					}





			}

		}





	}

	else if(col>6) {
		pSw=nullptr;
		int nselV=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
		if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiSimulation()) {
			GetCell(col,row, &cell);
			
			if(cell.GetBool()) {
				bool picked;
				picked=false;
				// Remove from list if in list
				for(size_t i=0;i<m_pSimDoc->m_SelectMultiVal.size();i++){
					if(col==m_pSimDoc->m_SelectMultiVal[i].col&&row==m_pSimDoc->m_SelectMultiVal[i].row) {
						m_pSimDoc->m_SelectMultiVal.erase(m_pSimDoc->m_SelectMultiVal.begin()+i);
						picked=true;
					}
				}

				if (!picked) {
					// Avoid definition of performance that is not valid (col 7-9 and col 14-15) for SV variable
					// when not series of Performance exist
					if (row >= m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val()) {
						if (((col >= 7 && col < 10) || col == 11 || col == 14 || col == 15) && m_pSimDoc->m_MultiRun_Array.size() <= 1) {
							return false;
						}
					}
					MultiPlotClick(isheet, col, row, TRUE);
				}
				else {
					cell.SetBool(FALSE);
					cell.SetBackColor(m_NoColor);
					SetCell(col,row,&cell);
					RedrawCell(col,row);
					MultiPlotClick(isheet,col, row, FALSE);
				}
			}
			else {
				bool picked;
				picked=false;
				// Remove from list if in list
				for(size_t i=0;i<m_pSimDoc->m_SelectMultiVal.size();i++){
					if(col==m_pSimDoc->m_SelectMultiVal[i].col&&row==m_pSimDoc->m_SelectMultiVal[i].row) {
						m_pSimDoc->m_SelectMultiVal.erase(m_pSimDoc->m_SelectMultiVal.begin()+i);
						picked=true;
						MultiPlotClick(isheet,col, row, FALSE);
					}
				}
				if(!picked) {
					// Avoid definition of performance that is not valid (col 7-9 and col 14-15) for SV variable
					// when not series of Performance exist
					if (row >= m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val()) {
						if (((col >= 7 && col < 10) || col == 11 || col == 14 || col == 15) && m_pSimDoc->m_MultiRun_Array.size() <= 1) {
							return false;
						}
					}

					MultiPlotClick(isheet,col, row, TRUE);
					cell.SetBool(TRUE);
					SetCell(col,row,&cell);
					RedrawCell(col,row);
				}
			}
			UpdatePlotStatusBar(m_pProp->m_ActualMultiSheet);
		
		}
		else {
		// Change Value Errors
		

		}




	}



	return -1;
}
int DocClick::ChartsClick(int col, int row) {

	return 8;
}

int DocClick::ChaParClick(int col, int row){

	
	
	return -1;
}
int DocClick::MultiRunClick(int col, int row) {
	CUGCell cell;
	string Name, Group, type;
	if(m_ShowCoVarRes&&m_pSimDoc->IsFinishedSimulation()) {
		return MultiCoVarParPlotClick(col, row);
	}
	else if(col==2&&m_pSimDoc->IsFinishedSimulation()) return MultiParPlotClick(col, row);
	else if(col==3&&!m_pSimDoc->IsFinishedSimulation()&&!m_ShowMultiRes) return MultiRunPreparation(col, row);
	else if(col==4&&!m_pSimDoc->IsFinishedSimulation()) return MultiRunPreparation(col, row);
	else if (col == 7 && m_pSimDoc->IsFinishedSimulation()) {
		if (m_pSimDoc->m_MStorage.m_AcceptedIdString.size() > 1) {
			m_pSimDoc->m_MStorage.m_ActualEnsemble++;
			m_pSimDoc->m_MStorage.m_ActualEnsemble = m_pSimDoc->m_MStorage.m_ActualEnsemble % m_pSimDoc->m_MStorage.m_AcceptedIdString.size();
			m_pSimDoc->m_MStorage.SetActual_AcceptedEnsemble();
			if (m_pSimDoc->m_MStorage.Check_Change_PerformanceIndicator_ToActual_Ensemble()) {
				m_pSimDoc->m_MStorage.CompleteProcessingOfInputFiles_MultiStorage();
			}
		}
		return 10;
	}
	return -1;
}
int DocClick::AnnimateClick(int col, int row) {
	CUGCell cell, cell1;
	string Name, Group, Type;
	SimB *pSw;
	if(!m_pSimDoc->IsFinishedSimulation()) {
		GetCell(2,row, &cell);
		Name=cell.GetStdText();
		int ParV=cell.GetParam();
		int pos1=Name.rfind('(');
		int pos2=Name.rfind(')');
		int Index;
		int selected;
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
		
		GetCell(1,row, &cell1);
		Group=cell1.GetStdText();
		GetCell(0,row, &cell1);
		Type=cell1.GetStdText();;
		
		pSw=m_pSimDoc->GetPtr(Type, Group, Name);
		// Change toggle Outputs on/off
		
		if(pSw!=nullptr&&(pSw->GetType()==STATE||pSw->GetType()==FLOW||pSw->GetType()==AUX||pSw->GetType()==DRIVE)) {
				int ityp=p_ModelInfo->GetTypeIndex(Type);
				size_t index=Index-1;
				bool sel=pSw->GetStoreFlag(index);
				if(sel) {
					pSw->SetStoreFlag(index, false);
					pSw->SetMultiStoreFlag(index, true);
					selected=2;
				}
				else {
					bool multi=pSw->GetMultiStoreFlag(index);
					if(multi) {
						pSw->SetMultiStoreFlag(index, false);
						selected=0;
					}
					else {
						pSw->SetStoreFlag(index, true);
						selected=1;
					}


				}
				m_pSimDoc->TimeModified();

				if(selected==0) 
					cell.SetBackColor(RGB(255,255,255));
				else if(selected==1)
					cell.SetBackColor(RGB(40,240,240));
				else if(selected==2)
					cell.SetBackColor(RGB(0,100,100));
				SetCell(col,row, &cell);
				RedrawCell(col,row);
				GotoCell(col,row+1);
				
			}
	}
	return -1;
}

int  DocClick::MenuCornerClick(int col, int row) {
	int sheet=GetSheetNumber();
	
	return -1;
}
int DocClick::MultiCoVarParPlotClick(int col, int row) {
	CUGCell cell;
	string Name, Group, type;

		if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiSimulation()&&!m_ShowMultiRes&&m_pSimDoc->IsMultiStorageBinFileValid()&&m_ShowCoVarRes) {
			GetCell(col,row, &cell);
			string text=cell.GetStdLabelText();
			int type=cell.GetCellType();
			//if(cell.GetBool()) {
				bool picked;
				picked=false;
				for(size_t i=0;i<m_pSimDoc->m_SelectMultiPar.size();i++){
					if(col==m_pSimDoc->m_SelectMultiPar[i].col&&row==m_pSimDoc->m_SelectMultiPar[i].row) {
						m_pSimDoc->m_SelectMultiPar.erase(m_pSimDoc->m_SelectMultiPar.begin()+i); picked=true;
					}
				}
				if(!picked)
					MultiPlotClick(10,col, row, TRUE);
				else {
					cell.SetBool(FALSE);
					cell.SetBackColor(m_NoColor);
					SetCell(col,row,&cell);
					RedrawCell(col,row);
					MultiPlotClick(10,col, row, FALSE);
				}
				UpdatePlotStatusBar(10);
				return -1;
			//}
			UpdatePlotStatusBar(10);

		}


	return 10;
}
int DocClick::MultiParPlotClick(int col, int row) {
	CUGCell cell;
	string Name, Group, type;

		if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiSimulation()&&!m_ShowMultiRes&&m_pSimDoc->IsMultiStorageBinFileValid()&&!m_ShowCoVarRes) {
			GetCell(col,row, &cell);
			string text=cell.GetStdLabelText();
			int type=cell.GetCellType();
			//if(cell.GetBool()) {
				bool picked;
				picked=false;
				for(size_t i=0;i<m_pSimDoc->m_SelectMultiPar.size();i++){
					if(col==m_pSimDoc->m_SelectMultiPar[i].col&&row==m_pSimDoc->m_SelectMultiPar[i].row) {
						m_pSimDoc->m_SelectMultiPar.erase(m_pSimDoc->m_SelectMultiPar.begin()+i); picked=true;
					}
				}
				if(!picked)
					MultiPlotClick(10,col, row, TRUE);
				else {
					cell.SetBool(FALSE);
					cell.SetBackColor(m_NoColor);
					SetCell(col,row,&cell);
					RedrawCell(col,row);
					MultiPlotClick(10,col, row, FALSE);
				}
				UpdatePlotStatusBar(10);
				return -1;
			//}
			UpdatePlotStatusBar(10);

		}


	return 10;
}
int DocClick::MultiRunPreparation(int col, int row) {
	CUGCell cell;
	string Name, Group, type;
		GetCell(col,row, &cell); 
		int ParV=cell.GetParam();
		if(col==2&&!m_ShowMultiRes) {
			if(ParV==1000) {	//DataBase Records

			}
		}
		else if(col==4) {
			if(ParV==1000) {	//Table
			int i=0;

			SimB* pPar;
			string ParName;


			GetCell(-1,row,&cell);
			int ip=int(cell.GetNumber());
			GetCell(0,row,&cell);
			int idim=int(cell.GetNumber());
					
			pPar=m_pSimDoc->MR_Get_pPar(idim,ip);

			string PName;
			if(pPar!=nullptr) {
					PName=pPar->GetName();
					int TabIndex=m_pSimDoc->MR_GetTabIndex(idim,ip);
					if(TabIndex>0)
							PName+="("+FUtil::STD_ItoAscii(TabIndex+1)+")";

					MultiRunTable dlg(m_pSimDoc, pPar, TabIndex);
					if(dlg.DoModal()==IDOK){

					}
					else
						;

			}
			else
				PName="-";





		}

	}

	return -1;
}
