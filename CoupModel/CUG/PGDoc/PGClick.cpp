#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include "..\..\Util.h"
#include "..\..\NewBase\FileScanner.h"
#include "PGClick.h"
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

PGClick::PGClick(void)
{
	//m_pChartCug=MySingle::instance().GetChartCugPointer();
	
}
PGClick::~PGClick(void)
{
}
void PGClick::ExpandFileNameLine(int col, int row, int ParV) {		
CUGCell cell;				
	int index=ParV-1000;
	GetCell(-2,row,&cell);
	int Index=int(cell.GetNumber());
	if(m_pPGArchive->m_PGFileArray.size()==0) {
		OnSheetSetup(0);
		return;
					
	}
	if(m_pPGArchive->m_PGFileArray[Index].Visible==0){
		m_pPGArchive->m_PGFileArray[Index].Visible=1;
	}
	else {
		m_pPGArchive->m_PGFileArray[Index].Visible=0;
	}
	EnableUpdate(FALSE); 
	while(GetNumberRows()>0)
			DeleteRow(0);
	EnableUpdate(TRUE); 
	Make_PGFileSheet();

}
void PGClick::ChangeSubChartNumber(int col, int row) {
		CUGCell cell1;	
		int ichart;
		GetCell(4,row, &cell1);
		ichart=int(cell1.GetNumber());
		ichart++;
		m_CurrentSubChart=(ichart-1)%(m_pProp->m_NumChartX*m_pProp->m_NumChartY)+1;
		QuickSetNumber(4,row, m_CurrentSubChart);
		int Index=0;
		while(m_pSimDoc->m_SelectPGFile[Index].row!=row) {
			if(Index<m_pSimDoc->m_SelectPGFile.size()-1)
				Index++;
			else
				break;

		}
		if(Index<m_pSimDoc->m_SelectPGFile.size())
			m_pSimDoc->m_SelectPGFile[Index].subchart=m_CurrentSubChart;
		RedrawCell(4,row);
		MySingle::instance().GetCugPointer()->SetDocument(m_pDoc);
		m_PGCharts.TimeSerieChartClick();
	
}
void PGClick::ClearAllChartVar(int col, int row) {
CUGCell cell, cell1; 
	int Index=0;
			
	GetCell(-1,row,&cell);
	Index=int(cell.GetNumber());		
	GetCell(-1,row-Index-1, &cell1);
	int index=int(cell1.GetNumber()-1);
	CPG *pPG=(CPG*)m_pPGArchive->GetPGFile(index);
	if(pPG!=nullptr)
		while(m_pSimDoc->m_SelectPGFile.size()>0) {
				row=m_pSimDoc->m_SelectPGFile[0].row;
				GetCell(3,row,&cell);
				cell.SetBool(FALSE);
				SetCell(3,row,&cell);
				RedrawCell(3,row);
				QuickSetNumber(4,row,0);
				QuickSetText(4,row,"");
				RedrawCell(4,row);
				m_pSimDoc->m_SelectPGFile.erase(m_pSimDoc->m_SelectPGFile.begin());
		}
}
void PGClick::ChangeChartVar(int col, int row) {
CUGCell cell, cell1; 
		int Index=0;
		GetCell(col,row, &cell); 
 		bool erase=true;
		if(cell.GetBool()) erase=false;
		int ParV=cell.GetParam();
		int RepitNo;
		CPG *pPG;
		GetCell(-1,row,&cell);
		Index=int(cell.GetNumber());	
		if(ParV==3100) {
			pPG=(CPG*)m_pPGArchive->GetPGFile(m_IndexSource);
			GetCell(-2,row,&cell);
			RepitNo=int(cell.GetNumber()-1);
			GetCell(0,row,&cell);
		}
		else {
			GetCell(-1,row-Index-1, &cell1);
			int index=int(cell1.GetNumber()-1);
			pPG=(CPG*)m_pPGArchive->GetPGFile(index);
			m_pDoc=m_pPGArchive->GetDocPointer(index);
			RepitNo=pPG->GetRepititionSequence() ;
			GetCell(3,row,&cell);
		}
		if (ParV == m_ChartNumberSelect.param) {
			ChangeSubChartNumber(col, row);
			return;
		}
		if(IsMultiPGRepPlot()) {

			size_t Option;
			if(col==0) Option=2;
			else if(col==5) Option=0;
			else Option=1;
			MultiPGChartDepChoice(Index,Option, erase );

		}
		else {
			if(col==3||col==0) {
				if(cell.GetBool()) {
					SelectPG selpg;
					selpg.pPG=pPG;
					selpg.PGIndex=Index;
					selpg.Repit=RepitNo;
					selpg.row=row;

					if(m_pSimDoc->m_SelectPGFile.size()==0)
						m_CurrentSubChart=1;
					else if(m_CurrentSubChart<m_pProp->m_NumChartX*m_pProp->m_NumChartY)
						m_CurrentSubChart++;
					selpg.subchart=m_CurrentSubChart;
					m_pSimDoc->m_SelectPGFile.push_back(selpg);
					QuickSetNumber(4,row,m_CurrentSubChart);
					pPG->SetPicked(Index,TRUE);							
					RedrawCell(4,row);
				}
				else {
					for( int i=0; i<m_pSimDoc->m_SelectPGFile.size();i++)
					{
						if(row==m_pSimDoc->m_SelectPGFile[i].row) {
							m_pSimDoc->m_SelectPGFile.erase(m_pSimDoc->m_SelectPGFile.begin()+i);
							QuickSetText(4,row,"");
							RedrawCell(4,row);
							if(pPG!=nullptr) 
								pPG->SetPicked(Index, FALSE);
						}
					}
				}
				m_PGCharts.TimeSerieChartClick();

}
		}
}
void PGClick::ChangeChartChoice(int isheet,int col, int row, int ind,CString After, int ParV) {
	if (ParV == m_VarTransformationCombo.Param) {

		m_pProp->m_ReShape.m_Cumulated=ind;
		//m_pProp->m_ReShape.m_Cumulated = m_pProp->m_ReShape.m_Cumulated%3;
		OnSheetSetup(0);
		m_pUtil->m_FileSave.OnReDraw(true, m_pSimDoc);
		//m_PGCharts.TimeSerieChartClick();
		/*m_TimeSeriePlot.LoadTimeSerieData();
		m_TimeSeriePlot.DrawTimeSeriePlot(&m_pView->m_ChartViewer);*/



	}
	if (ParV == m_ChartTypeOptions.Param) {
		m_pProp->SetChartType(ind);
		m_pProp->SetChartType(CT2A(After).m_psz);
		OnSheetSetup(0);
		if (m_pProp->GetChartTypeIndex() == CHART_TYPES::TIME_SERIE) {
			m_pProp->ResetXScale();
			m_pProp->SetFirstIndepSet(vartype::DATEAXIS,true);
			m_pUtil->m_FileSave.OnReDraw(false, m_pSimDoc);

		}
		else {
				m_pProp->ResetXScale();
				m_pProp->ResetYLeftScale();
				m_pProp->ResetYRightScale();
				m_pProp->ResetZScale();
				m_pUtil->m_FileSave.OnReDraw(true, m_pSimDoc);
		}
	}
	else if(col==1) {
			m_pGSource=(CPG*)m_pPGArchive->GetPGFile(ind);
			m_IndexSource=ind;
	}
	else if(ParV== m_RepOptionsCombo.Param) {
		m_pGSource->SetRepititionSequence(ind, true);
		MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot.m_ChartInfo.PGRepitMode=true;
		for(int il=0; il< m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);il++) {
			int j=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, il);
			if(m_pSimDoc->m_PlotStorage.SetPGRepitNo(j, ind)) ;
		}
		if(m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber)>0)
			m_pUtil->m_FileSave.OnReDraw(true, m_pSimDoc);

	}
	else if(ParV==m_XAxisCombo.Param|| ParV == m_VarDim1Combo.Param) {
		if(isheet==0) {
			string str="";
			if(ind==0) {	m_pProp->SetFirstIndepVarType(vartype::NO_USE);

			}
			if(ind==1) {
				m_pProp->SetFirstIndepVarType(vartype::PGHOUR);
				str="Hour";
			}
			if(ind==2) {
				m_pProp->SetFirstIndepVarType(vartype::PGDAY);
				str="Day of Year";
			}
			if(ind==3) {
				m_pProp->SetFirstIndepVarType(vartype::PGYEAR);
				str="Year";

			}
			if(m_pProp->GetIndepFirst()!= vartype::NO_USE) {
				m_pProp->SetFirstIndepSet(enum vartype(m_pProp->GetIndepFirst()) ,true);
				m_pProp->SetFirstIndepString(enum vartype(m_pProp->GetIndepFirst()), str);
			}
			else
				m_pProp->SetFirstIndepSet(enum vartype(m_pProp->GetIndepFirst()),false);
			OnSheetSetup(0);
			m_pUtil->m_FileSave.OnReDraw(true, m_pSimDoc);
			
		}
		else {
			if(ind==1) 	m_pProp->SetFirstIndepVarType(vartype::GEO_LONG);
			if(ind==2) 	m_pProp->SetFirstIndepVarType(vartype::GEO_LAT);
			if(ind==3) 	m_pProp->SetFirstIndepVarType(vartype::GEO_ALT);
		}
	}
	else if(ParV==m_YAxisCombo.Param||ParV == m_VarDim2Combo.Param) {
		if(isheet==0) {
			if((m_pProp->m_MultiPG_dep.FirstType== vartype::PGDAY&&m_pProp->m_MultiPG_dep.SecondType== vartype::PGYEAR)
				|| (m_pProp->m_MultiPG_dep.FirstType == vartype::PGYEAR&&m_pProp->m_MultiPG_dep.SecondType == vartype::PGDAY)) {
				m_pProp->m_HourValueRepresentation=ind;
				m_pProp->m_YearValueRepresentation=0;
			}
			else if((m_pProp->m_MultiPG_dep.FirstType== vartype::PGDAY&&m_pProp->m_MultiPG_dep.SecondType== vartype::PGHOUR)
				|| (m_pProp->m_MultiPG_dep.FirstType == vartype::PGHOUR&&m_pProp->m_MultiPG_dep.SecondType == vartype::PGDAY)) {
				m_pProp->m_YearValueRepresentation=ind;
				m_pProp->m_HourValueRepresentation=0;
			}

			string str="";
			if(ind==0) 	m_pProp->SetSecondIndepVarType(vartype::NO_USE);
			if(ind==1) 	{m_pProp->SetSecondIndepVarType(vartype::PGHOUR);str="Hour";}
			if(ind==2) {m_pProp->SetSecondIndepVarType(vartype::PGDAY);str="Day of Year";}
			if(ind==3) {m_pProp->SetSecondIndepVarType(vartype::PGYEAR);str="Year";}

			if(m_pProp->GetIndepSecond()!= vartype::NO_USE) {
				m_pProp->SetSecondIndepSet(enum vartype(m_pProp->GetIndepSecond()) ,true);
				m_pProp->SetSecondIndepString(enum vartype(m_pProp->GetIndepSecond()), str);
			}
			else
				m_pProp->SetSecondIndepSet(enum vartype(m_pProp->GetIndepSecond()) ,false);
			OnSheetSetup(0);
			m_pUtil->m_FileSave.OnReDraw(true, m_pSimDoc);

		}
		else {
			if(ind==1) 	m_pProp->SetSecondIndepVarType(vartype::GEO_LONG);
			if(ind==2) 	m_pProp->SetSecondIndepVarType(vartype::GEO_LAT);
			if(ind==3) 	m_pProp->SetSecondIndepVarType(vartype::GEO_ALT);
		}
	}
	else if (ParV == m_SelectWithinDay.param) {
		size_t ind = MFC_Util::AtoInt(After);
		if (After == "All") ind = 0;
		if (m_pProp->m_MultiPG_dep.FirstType == vartype::PGDAY&&m_pProp->m_MultiPG_dep.SecondType == vartype::PGYEAR) {
			m_pProp->m_HourValueRepresentation = ind;
			m_pProp->m_YearValueRepresentation = 0;
		}
		else if (m_pProp->m_MultiPG_dep.FirstType == vartype::PGDAY&&m_pProp->m_MultiPG_dep.SecondType == vartype::PGHOUR) {
			m_pProp->m_YearValueRepresentation = ind;
			m_pProp->m_HourValueRepresentation = 0;
		}
		OnSheetSetup(0);
		m_pUtil->m_FileSave.OnReDraw(true, m_pSimDoc);
	}





	if(col==9) {
			if(ind==1) 	m_pProp->SetDependentVarType(vartype::VAR_MEAN);
			if(ind==2) 	m_pProp->SetDependentVarType(vartype::VAR_MIN);
			if(ind==3) 	m_pProp->SetDependentVarType(vartype::VAR_MAX);
	}
		



}
void PGClick::MultiPGChartCheck() {
	if(IsMultiPGRepPlot()) {
		SelectMultiPG val;
		val.pPG=m_pGSource;
		val.PGIndex=0;
		val.FirstType=enum vartype(m_pProp->GetIndepFirst());
		val.SecondType=enum vartype(m_pProp->GetIndepSecond());
		val.DependentType=enum vartype(m_pProp->GetIndepDependent());
		if(m_pSimDoc->m_SelectMultiPGFile.size()==0)
				m_CurrentSubChart=1;
		else if(m_CurrentSubChart<m_pProp->m_NumChartX*m_pProp->m_NumChartY)
				m_CurrentSubChart++;
		val.subchart=m_CurrentSubChart;
		if(val.FirstType== vartype::NO_USE||val.SecondType== vartype::NO_USE||val.DependentType== vartype::NO_USE) return;
		m_pSimDoc->m_SelectMultiPGFile.push_back(val);
		m_PGCharts.MultiPGChartClick();
	}
	else {
		m_PGCharts.TimeSerieChartClick();
	}


}
void PGClick::MultiPGChartDepChoice(int PGIndex, size_t Option, bool erase) {
	SelectMultiPG val;

	enum vartype Type;
	Type= vartype::NO_USE;
	if(Option==0) {Type=vartype::VAR_MIN;
		m_pGSource->SetPickedMin(PGIndex, true);
	}
	else if(Option==1) {
		Type= vartype::VAR_MAX;
		m_pGSource->SetPickedMax(PGIndex, true);
	}
	else if(Option==2) {
		Type= vartype::VAR_MEAN;
		m_pGSource->SetPickedMean(PGIndex, true);

	}
	if(erase) {
		for( int i=0; i<m_pSimDoc->m_SelectMultiPGFile.size();i++) {
			if(m_pSimDoc->m_SelectMultiPGFile[i].PGIndex==PGIndex&&m_pSimDoc->m_SelectMultiPGFile[i].DependentType==Type) {
				m_pSimDoc->m_SelectMultiPGFile.erase(m_pSimDoc->m_SelectMultiPGFile.begin()+i);
				if(Option==0) m_pGSource->SetPickedMin(i, false);
				else if(Option==1) m_pGSource->SetPickedMax(i, false);
				else if(Option==2) m_pGSource->SetPickedMean(i, false);
			}
		}
	}
	else {
		val.pPG=m_pGSource;
		val.PGIndex=PGIndex;
		val.FirstType=enum vartype(m_pProp->GetIndepFirst());
		val.SecondType=enum vartype(m_pProp->GetIndepSecond());
		val.DependentType=Type;
		if(m_pSimDoc->m_SelectMultiPGFile.size()==0)
				m_CurrentSubChart=1;
		else if(m_CurrentSubChart<m_pProp->m_NumChartX*m_pProp->m_NumChartY)
				m_CurrentSubChart++;
		val.subchart=m_CurrentSubChart;


		m_pSimDoc->m_SelectMultiPGFile.push_back(val);
	}

	m_PGCharts.MultiPGChartClick();

}


