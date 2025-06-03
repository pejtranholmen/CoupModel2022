#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include "..\..\Util.h"
#include "..\..\NewBase\FileScanner.h"
#include "ChartCugSheets.h"
#include "..\ChartProperties\ChartCug.h"
#include "..\..\MainFrm.h"


#include "..\ColourPickerDlg.h"


ChartCugSheets::ChartCugSheets(void)
{
	m_extra_xy_variables = 0;
}
ChartCugSheets::~ChartCugSheets(void)
{
}

void ChartCugSheets::ShowDataInGrid(bool createfile) 
{
	    bool keepprev=createfile;
		m_CreatePG=createfile;
		m_NumVarSelected=m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);
		m_NumVarBandSelected=m_TimeSeriePlot.m_ChartInfo.NumVariableBands;


		if (m_pUtil->m_MultiRunPlot || m_pProp->m_ReShape.m_NormalDist || m_pProp->m_ReShape.m_SortedSeries ) {

			m_extra_xy_variables = 0;
			


		}

		m_extra_xy_variables = 0;
		if(m_pProp->GetIndepFirst()!=DATEAXIS)
			m_extra_xy_variables = 1;
		if (m_pProp->IsSecondIndepSet())
			m_extra_xy_variables = 2;

		size_t count_ens=0;
		size_t count_not_toplot = 0;
		if (m_pUtil->m_MultiRunPlot) {
			for (int i = 0; i < m_NumVarSelected; i++) {
				for (size_t iens = 0; iens < m_TimeSeriePlot.GetNumberOfEnsembles(); iens++) {
					if (m_pSimDoc->m_MStorage.IsToBePlotted(iens))	count_ens++;
					else count_not_toplot++;
				}
			}
		}
		if (count_ens > 0) {
			count_ens++;
			m_extra_xy_variables = 0;
		}

		if (m_pProp->m_ChartType >=CHART_TYPES::FREQUENCE && m_pProp->m_ChartType >=CHART_TYPES::CUM_FREQUENCE) {
			SetNumberCols(m_NumVarSelected+2 + m_extra_xy_variables + count_ens - count_not_toplot);

		}

		SetNumberCols(m_NumVarSelected + m_extra_xy_variables+count_ens-count_not_toplot);
		AddTimeData(m_pProp->m_timeStamps,m_pProp->m_timeStamps2, 0);

		size_t nstart=0;


		size_t ratio = m_TimeSeriePlot.m_ChartInfo.NumVariables / m_TimeSeriePlot.m_ChartInfo.NumPriorVariables;


		auto AddDatatoGrid = [&](DoubleArray data, int index) {
			double v;
			int i = data.len;
			//if (m_pProp->m_Legend1.size() <= 0) return;
			if (m_MakeDataSheet) {
				SetSheetNumber(6, false);
				for (int j = 0; j < data.len; j++) {
					v = *(data.data + j);
					if (v == Chart::NoValue)
						QuickSetText(index + 1, j, "");
					else {
						QuickSetNumber(index +1, j, v);
						QuickSetAlignment(index+1 , j, UG_ALIGNRIGHT);
					}
				}
				RedrawAll();
			}
		};
		auto AddDatatoPGFile = [&](DoubleArray data, int index) {
			if (m_CreatePG) {
				m_PGExtra.AddVariableVector(data, index + 1);
				m_ToSavePGStruc = true;
			}
		};



		if (m_pUtil->m_MultiRunPlot) {
			size_t count_valid=m_extra_xy_variables;
			for (int i = 0; i < m_NumVarSelected; i++) {
				if (m_pProp->m_ChartType >=CHART_TYPES::FREQUENCE && m_pProp->m_ChartType >=CHART_TYPES::CUM_FREQUENCE) {
					AddTimeData(m_pProp->m_timeStamps, m_pProp->m_timeStamps2, i);
					if (!createfile)	AddDatatoGrid(m_TimeSeriePlot.m_dataSeriesA[i], i*((count_ens) / m_NumVarSelected + 2) + m_extra_xy_variables);
					AddDatatoPGFile(m_TimeSeriePlot.m_dataSeriesA[i], i*(count_ens / m_NumVarSelected + 1) + m_extra_xy_variables);
					if (i > 0) count_valid++;
				}
				else {
				if(!createfile)	AddDatatoGrid(m_TimeSeriePlot.m_dataSeriesA[i], i*((count_ens) / m_NumVarSelected + 1) + m_extra_xy_variables);
					AddDatatoPGFile(m_TimeSeriePlot.m_dataSeriesA[i], i*(count_ens / m_NumVarSelected + 1) + m_extra_xy_variables);

				}
					

				for (size_t iens = 0; iens < m_TimeSeriePlot.GetNumberOfEnsembles(); iens++) {
					string legend = m_TimeSeriePlot.GetEnsembleIdString(iens);
					if (m_pSimDoc->m_MStorage.IsToBePlotted(iens)) {
						count_valid++;

						if (!createfile) AddDatatoGrid(m_TimeSeriePlot.m_dataSeriesA[i+(iens+1)*m_NumVarSelected], i+ count_valid  );
						if (m_pProp->m_ChartType >= CHART_TYPES::FREQUENCE && m_pProp->m_ChartType <= CHART_TYPES::CUM_FREQUENCE)
							AddDatatoPGFile(m_TimeSeriePlot.m_dataSeriesA[i + (iens + 1)*m_NumVarSelected], count_valid);
						else
							AddDatatoPGFile(m_TimeSeriePlot.m_dataSeriesA[i + (iens + 1)*m_NumVarSelected], i + count_valid);

					}
				}
			}
		}
		else {
			for (int i = 0; i<m_NumVarSelected; i++) {
				if(m_pProp->m_ChartType != CHART_TYPES::SCATTER)
					AddDatatoGrid(m_TimeSeriePlot.m_dataSeriesA[i], i +  m_extra_xy_variables);
				else
					AddDatatoGrid(m_TimeSeriePlot.m_dataSeriesA[i], i);
				AddDatatoPGFile(m_TimeSeriePlot.m_dataSeriesA[i], i + m_extra_xy_variables);
			}



		}
		for(int i=0;i<m_NumVarBandSelected;i++) {
			//if(m_EntireDataSet) {
				AddBandData(m_TimeSeriePlot.m_dataSeriesB[i],m_TimeSeriePlot.m_dataSeriesMIN[i],
				m_TimeSeriePlot.m_dataSeriesMAX[i],m_TimeSeriePlot.m_dataSeriesP10[i],m_TimeSeriePlot.m_dataSeriesP50[i],m_TimeSeriePlot.m_dataSeriesP90[i], i);

			//}
			//else {
				//AddBandData(m_pProp->viewPortDataSeriesB[i],DoubleArray(m_pProp->viewPortDataSeriesMin[i],noOfPoints),DoubleArray(m_pProp->viewPortDataSeriesMax[i],noOfPoints),DoubleArray(m_pProp->viewPortDataSeriesP10[i],noOfPoints),DoubleArray(m_pProp->viewPortDataSeriesP50[i],noOfPoints),DoubleArray(m_pProp->viewPortDataSeriesP90[i],noOfPoints), i);
		}
		

		return;


}
void ChartCugSheets::OnUpdate()
{
	CUGCell cell; 
	string str;
	string txt;
	string optionlist;
	int cols = m_col,
		rows = m_row;
	if (m_pSimDoc == nullptr) return;
	m_NumVarSelected=m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);
	if(m_pUtil->m_FileSave.m_NoChange) return;
	m_pUtil->m_FileSave.m_NoChange=true;
	EnableMenu(true); 
	bool XLabels, ZLabels;
	XLabels=false;
	ZLabels=false;
	SetUniformRowHeight(false); 
	SetDefFont(&m_font);
	m_pMainframe=AfxGetMainWnd();
//	F* pValFile;

	m_NumUsedSubCharts=m_pProp->m_NumChartX*m_pProp->m_NumChartY;

	m_NumUsedSubCharts = min(m_TimeSeriePlot.m_ChartInfo.NumVariables, m_NumUsedSubCharts);

	if (m_NumUsedSubCharts == 0) m_NumUsedSubCharts = 1;
	//int ivar=m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);
	//m_NumUsedSubCharts=min(m_NumUsedSubCharts, ivar);

//	m_pUtil->m_MultiRunPlot=((CZoomscrolldemoDlg*)m_pDlg)->IsMultiRunPlot();

		//QuickSetText(0,-1,"Box-Cox Transformation");
	 if(m_Mode==6) {
		ShowDataInGrid();

		return;
		//MakeDataSheet(1);
		/*SetNumberRows(0);
		n1=m_Legend1.size();
		SetNumberCols(n1+1);
		SetMultiSelectMode ( true );
		QuickSetBackColor(-1,-1,m_ColorClick);	
		RedrawCell(-1,-1);*/
	//	return;

	}
	else if(m_Mode==7) {

		SetNumberRows(1);
		SetNumberCols(3);
		SetColWidth(0,200);
		SetColWidth(1,130);
		SetColWidth(2,130);
		QuickSetText(0,-1,"Box-Cox Transformation");				
		QuickSetText(1,-1,"Power coefficient");		
		QuickSetText(2,-1,"Offset value");

		GetCell(0,0,&cell);
		cell.SetCellType(UGCT_DROPLIST);
		optionlist="off\non\n";
		CString txt;
		if(m_TimeSeriePlot.m_BoxCox.use)
			txt="on";
		else
			txt="off";
		cell.SetText(txt);
		cell.SetLabelText((LPCTSTR)optionlist.c_str());
		SetCell(0,0,&cell);

		GetCell(1,0,&cell);
		cell.SetNumber(m_TimeSeriePlot.m_BoxCox.box_power);
		SetCell(1,0,&cell);
		
		GetCell(2,0,&cell);
		cell.SetNumber(m_TimeSeriePlot.m_BoxCox.box_offset);
		SetCell(2,0,&cell);


		QuickSetBackColor(-1,-1,m_ColorClick);	
		RedrawCell(-1,-1);
		return;
	}
	else if(m_Mode==10) {
		SetNumberRows(0);
		SetNumberCols(1);
		SetColWidth(0,400);
		QuickSetText(0,-1,"No information available in this mode");
		return;
	}
	else {
		GRIDCHAR in=GetRowsAndCols();
		rows=in.rows;
		cols=in.cols;

	}


	if(m_pProp->m_freeze) return;


	EnableUpdate(false);
	while(GetNumberRows()>0)
			DeleteRow(0);
	EnableUpdate(true);
		
	SetNumberRows(rows);
	SetNumberCols(cols);
	if(m_Mode==0) {
		if(m_pUtil->m_RealTimeMode) Make_Sheet0_RealTime();
		else if(m_pUtil->m_ParameterPlotMode) Make_Sheet0_ParameterPlot();
		else if(m_pUtil->m_TablePlotMode) Make_Sheet0_TablePlot();
		else Make_Sheet0_TimeSeriePlot();
	}
	else if(m_Mode==1) { //General Layout
					
		Make_Sheet1_General();

		}
	else if(m_Mode==2&&m_pSimDoc!=nullptr) {
			if(m_pUtil->m_RealTimeMode) Make_Sheet2_RealTime();
			else if(m_pUtil->m_ParameterPlotMode) Make_Sheet2_ParameterPlot();
			else {
					
				Make_Sheet2_TimeSeriePlot(m_pProp->IsRightAxisExist(), true, m_pUtil->IsZAxisExist());
			}
	}
	else if(m_Mode==3&&m_pSimDoc!=nullptr) {

			if(m_pUtil->m_RealTimeMode) {
				int NumTSV=0;
				for(int i=0;i<MAXSIMVAL;i++) {
					auto pValFile=m_pSimDoc->ValidationFilePointer(i+1);
					if(pValFile!=nullptr)
					NumTSV+=pValFile->GetNumberOfSelectedVal();
				}
				Make_Sheet3_RealTime(NumTSV);	
			}
			else if(m_pUtil->m_ParameterPlotMode) Make_Sheet3_ParameterPlot();
			else Make_Sheet3_TimeSeriePlot(); // Time Serie plot
				
	}
	else if(m_Mode==4) {
		if(m_pUtil->m_RealTimeMode) Make_Sheet4_RealTime();
		else Make_Sheet4_TimeSeriePlot(rows);
	}
	else if(m_Mode==5&&m_pSimDoc!=nullptr&&!m_pUtil->m_RealTimeMode) Make_Sheet5_TimeSeriePlot(rows);

	if(m_pUtil->m_ToBeUpdated) {
		QuickSetBackColor(-1,-1,m_ColorChanged);
	}
	else
		QuickSetBackColor(-1,-1,m_ColorClick);		

	SetTopRow(m_CurrentTopRow);
	RedrawAll();
		

}


GRIDCHAR ChartCugSheets::GetRowsAndCols() {
	GRIDCHAR out;
	out.rows = 0;
	out.cols = 0;
	if(m_Mode==0) {
		if(m_pUtil->m_RealTimeMode) {
			out.rows=4;
			out.cols=3;
		}
		else if(m_pUtil->m_ParameterPlotMode) {
			out.rows=(m_ParameterPlot.m_P.size()*m_ParameterPlot.m_plotp.Opt_Vec.size()+m_ParameterPlot.m_Ps.size());
			if(out.rows<4) out.rows=4;
			out.cols=6;

		}
		else if(m_pUtil->m_TablePlotMode) {
			out.rows=2;
			out.cols=6;
		}
		else {
			// Time Serie Charts
			if(m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART||m_pProp->IsSecondIndepSet()) 
				out.cols=10;
			else
				out.cols=9;

			if (m_pProp->IsFirstIndepSet())
				out.rows = 8;
			else
				out.rows = 7;
		}

	}
	else if(m_Mode==1) {

		out.rows=7;
		out.cols=10;
	}
	else if(m_Mode==2) { //Axis
		if(m_pUtil->m_RealTimeMode) {
			out.rows=m_pSimDoc->MR_Get_NumTotalPar();
			out.cols=3;

		}
		else if(m_pUtil->m_ParameterPlotMode) {
			Func *funcpointer=m_ParameterPlot.GetCurrentFuncPointer();
			PLOT_PARAM plotparam=funcpointer->GetPlotParam();
			out.rows=2+plotparam.LegendsVec.size();
			out.cols=5;

		}
		else {// Time Serie mode

			if(!m_pProp->IsSecondIndepSet())
				out.rows = 2*min(m_TimeSeriePlot.m_ChartInfo.NumPriorVariables, m_NumUsedSubCharts);
			else
				out.rows=2;



			if(m_pUtil->IsZAxisExist()) {
				out.rows+= min(m_TimeSeriePlot.m_ChartInfo.NumPriorVariables, m_NumUsedSubCharts);
			}
			
			out.cols=6;
		}
	}
	else if(m_Mode==3) {
		EnableUpdate(false);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(true); 

		if(m_pUtil->m_RealTimeMode) {
			auto NumTSV=0;
			for(int i=0;i<MAXSIMVAL;i++) {
				auto pValFile=m_pSimDoc->ValidationFilePointer(i+1);
				NumTSV+=pValFile->GetNumberOfSelectedVal();
			}
			out.rows=NumTSV;
			out.cols=3;

		}
		else if(m_pUtil->m_ParameterPlotMode) {
			if(m_ParameterPlot.m_pCPt!=nullptr)
				out.rows=m_ParameterPlot.m_pCPt->GetSize();
			else
				out.rows=0;
			out.cols=1;

		}
		else {
			//auto n1=m_pProp->m_Legend1.size();
			size_t count_not_toplot = 0;
			if (m_pUtil->m_MultiRunPlot) {
				for (int i = 0; i < m_NumVarSelected; i++) {
					for (size_t iens = 0; iens < m_TimeSeriePlot.GetNumberOfEnsembles(); iens++) {
						if (!m_pSimDoc->m_MStorage.IsToBePlotted(iens))	 count_not_toplot++;
					}
				}
			}

			out.rows = m_TimeSeriePlot.m_ChartInfo.NumVariables - count_not_toplot;


			/*if(m_pProp->m_MultiValid.AcceptRejectMode)
				out.rows=n1*2;
			else
				out.rows=n1;*/
			out.cols=9;
			auto ArrayIndex = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, 0);
			if (ArrayIndex!= std::string::npos) {
				auto koll = m_pSimDoc->m_PlotStorage.GetPGSectionNo(ArrayIndex);
				auto p_info = m_pSimDoc->m_PlotStorage.GetPlotInfo(ArrayIndex);
				if (p_info.RepitDataType) m_PGRepit = true;
			}
			if(m_pUtil->m_MultiTable||m_ResidualSource||m_PGRepit) out.cols=11;

	
		}
	}
	else if(m_Mode==4) {
		if(m_pUtil->m_RealTimeMode) {
			out.rows=10;
			out.cols=3;
		}
		else {
			size_t count_not_toplot = 0;
			if (m_pUtil->m_MultiRunPlot) {
				for (int i = 0; i < m_NumVarSelected; i++) {
					for (size_t iens = 0; iens < m_TimeSeriePlot.GetNumberOfEnsembles(); iens++) {
						if (!m_pSimDoc->m_MStorage.IsToBePlotted(iens))	 count_not_toplot++;
					}
				}
			}

			out.rows=m_TimeSeriePlot.m_ChartInfo.NumVariables-count_not_toplot;
			out.cols=6;		
		}
	}
	else if(m_Mode==5) {
		size_t count_not_toplot = 0;
		if (m_pUtil->m_MultiRunPlot) {
			for (int i = 0; i < m_NumVarSelected; i++) {
				for (size_t iens = 0; iens < m_TimeSeriePlot.GetNumberOfEnsembles(); iens++) {
					if (!m_pSimDoc->m_MStorage.IsToBePlotted(iens))	 count_not_toplot++;
				}
			}
		}

		out.rows = m_TimeSeriePlot.m_ChartInfo.NumVariables - count_not_toplot;

		out.cols=6;		
	}
	else {


	}
	return out;
}

void ChartCugSheets::MakeDataSheet(int entire)
{
	//	Set
	CUGCell cell;
	SetMultiSelectMode ( true );
	//DragDropTarget ( true );
	int cols, rows;
	m_MakeDataSheet=true;
	m_NumberOfBands=m_NumVarBandSelected;

	size_t count_ens = 0;
	size_t count_not_toplot = 0;
	if (m_pUtil->m_MultiRunPlot) {
		for (int i = 0; i < m_NumVarSelected; i++) {
			for (size_t iens = 0; iens < m_TimeSeriePlot.GetNumberOfEnsembles(); iens++) {
				if (m_pSimDoc->m_MStorage.IsToBePlotted(iens))	count_ens++;
				else count_not_toplot++;
			}
		}
	}


//	m_CreatePG=false;

	
	auto n1=m_TimeSeriePlot.m_ChartInfo.NumVariables-count_not_toplot;
	if(m_pUtil->m_MultiTable) n1+=2;
	
	if(m_pProp->IsSecondIndepSet()) {
		auto n1 = m_pProp->GetNumSubCharts();
		cols=n1+2;
	}
	else {
		/*if(m_pProp->m_MultiValid.AcceptRejectMode&&!m_pProp->IsSecondIndepSet())
			cols=n1*2+1+m_NumberOfBands*6;
		else*/
			cols=n1+1+m_NumberOfBands*6;
	}
	if (m_pProp->m_ChartType >=CHART_TYPES::FREQUENCE && m_pProp->m_ChartType <= CHART_TYPES::CUM_FREQUENCE && m_pUtil->m_MultiRunPlot) cols+= m_NumVarSelected - 1;
	m_PGExtra.AdjustSize(2,cols-1);
	
	if(entire==1)
		m_EntireDataSet=true;
	else
		m_EntireDataSet=false;

	rows=m_NumberOfPoints;
	
	SetSheetNumber(6,false);
	EnableUpdate(false);
	
	while(GetNumberRows()>0)
		DeleteRow(0);
	EnableUpdate(true);
		

	SetNumberCols(cols);
	GetCell(-1,-1,&cell);
	cell.SetBackColor(m_ColorHeading);
	cell.SetText("Record No");
	SetCell(-1,-1,&cell);


	UnJoinCells(0,-1);
	QuickSetText(0,-1,"X");
	SetColWidth(0,140);
	SetColWidth(-1,90);
	size_t nstart=0;
	if(m_pProp->IsSecondIndepSet()) {
		nstart=1;
		QuickSetText(1,-1,"y");

	}
	size_t count = 0;
	for(auto i=nstart;i<n1+nstart;i++) {
		count++;
		UnJoinCells(i+1,-1);
		if (m_pProp->IsSecondIndepSet()) {
			QuickSetText(i + 1, -1, m_pProp->GetZLabel(i - 1).c_str(), false);
		}
		else {		
			if(m_pProp->m_MultiValid.AcceptRejectMode&&!m_pProp->IsSecondIndepSet()) {
				if(m_pProp->m_Legend1.size()>2*i+1) {
					QuickSetText(2*i+1,-1,m_pProp->m_Legend1.at(2*i).c_str(), false);
					QuickSetText(2*i+1+1,-1,m_pProp->m_Legend1.at(2*i+1).c_str(), false);
				}
			}
			else {

				if (m_pProp->m_Legend1.size() > i) {
					QuickSetText(count, -1, m_pProp->m_Legend1.at(i).c_str(), false);
					//m_PGExtra.SetVarName(count, m_pProp->m_Legend1.at(i).c_str());
				}
				else {
					QuickSetText(i+1,-1,"VarNo:"+FUtil::STD_ItoAscii(i+1), false);
					m_PGExtra.SetVarName(i+1,"VarNo:"+FUtil::STD_ItoAscii(i+1));
				}
				if (i%(m_pSimDoc->m_MStorage.GetNumEnsemblesToPlot()+1) == m_pSimDoc->m_MStorage.GetNumEnsemblesToPlot() && m_pProp->m_ChartType >=CHART_TYPES::FREQUENCE && m_pProp->m_ChartType <= CHART_TYPES::CUM_FREQUENCE && m_pUtil->m_MultiRunPlot) {
					count++;
					QuickSetText(count, -1, "X");
					//m_PGExtra.SetVarName(count, "X");


				}
			}

		}
/*	if(m_pProp->m_MultiValid.AcceptRejectMode&&!m_pProp->IsSecondIndepSet()) {
		for(int i=0;i<n1;i++) {
			QuickSetText(i+1+n1,-1,"VarNo:"+MFC_Util::ItoAscii(i+1)+'A');
			m_PGExtra.SetVarName(i+1+n1,"VarNo:"+FUtil::STD_ItoAscii(i+1)+'A');
			m_PGExtra.SetVarId(i+1+n1,"Mean A");
		}*/
	}
	for (int i=0;i<m_NumberOfBands;i++) {
		QuickSetText(1+n1+i*6,-1,"StD:"+FUtil::STD_ItoAscii(i+1));
		m_PGExtra.SetVarName(1+n1+i*6,"VarNo:"+FUtil::STD_ItoAscii(i+1));
		m_PGExtra.SetVarId(1+n1+i*6,"StD:"+FUtil::STD_ItoAscii(i+1));
		QuickSetText(2+n1+i*6,-1,"Min:"+FUtil::STD_ItoAscii(i+1));
		m_PGExtra.SetVarName(2+n1+i*6,"VarNo:"+FUtil::STD_ItoAscii(i+1));
		m_PGExtra.SetVarId(2+n1+i*6,"Min:"+FUtil::STD_ItoAscii(i+1));
		QuickSetText(3+n1+i*6,-1,"P5:"+FUtil::STD_ItoAscii(i+1));
		m_PGExtra.SetVarName(3+n1+i*6,"VarNo:"+FUtil::STD_ItoAscii(i+1));
		m_PGExtra.SetVarId(3+n1+i*6,"P5:"+FUtil::STD_ItoAscii(i+1));
		QuickSetText(4+n1+i*6,-1,"Median:"+FUtil::STD_ItoAscii(i+1));
		m_PGExtra.SetVarName(4+n1+i*6,"VarNo:"+FUtil::STD_ItoAscii(i+1));
		m_PGExtra.SetVarId(4+n1+i*6,"Median:"+FUtil::STD_ItoAscii(i+1));
		QuickSetText(5+n1+i*6,-1,"P95:"+FUtil::STD_ItoAscii(i+1));
		m_PGExtra.SetVarName(5+n1+i*6,"VarNo:"+FUtil::STD_ItoAscii(i+1));
		m_PGExtra.SetVarId(5+n1+i*6,"P95:"+FUtil::STD_ItoAscii(i+1));
		QuickSetText(6+n1+i*6,-1,"Max:"+FUtil::STD_ItoAscii(i+1));
		m_PGExtra.SetVarName(6+n1+i*6,"VarNo:"+FUtil::STD_ItoAscii(i+1));
		m_PGExtra.SetVarId(6+n1+i*6,"Max:"+FUtil::STD_ItoAscii(i+1));
	}
	RedrawAll();
	//OnReDraw(true);


}
void ChartCugSheets::Make_Sheet0_RealTime()
{
		CUGCell cell; 
	string str;
	string txt;
	string optionlist;
			SetSH_NumberCols(3);
			for(long rr=0;rr<4;rr++)
				for(int cc=-3;cc<0;cc++)
					UnJoinCells(cc,rr);
			SetSH_Width(90);
			SetSH_ColWidth(0,30);
			SetSH_ColWidth(1,30);
			JoinCells(-2,1,-1,1);
			JoinCells(-2,3,-1,3);
			JoinCells(-3,4,-1,4);
			SetSH_ColWidth(2,30);

			SetColWidth(0,300);
			SetColWidth(1,300);
			SetColWidth(2,200);
			GetCell(0,0,&cell);
			cell.SetCellType(UGCT_CHECKBOX);

			if(m_pSimDoc->IsMultiSimulation()) {
				QuickSetText(0,-1,"Monitoring of Multi Simulation");
				cell.SetReadOnly(false);
				cell.SetBool(m_RealTime.GetMultiSimDisplay());
				cell.SetLabelText("Display results for latest 300 runs");
				SetCell(0,0,&cell);

				GetCell(1,0,&cell);
				cell.SetText("Performance indicators chart3");
				cell.SetBackColor(m_ColorHeading);
				SetCell(1,0,&cell);

				GetCell(1,1,&cell);
				cell.SetCellType(UGCT_DROPLIST);
				optionlist="Coeficient of determination\nIntercept\nSlope\nMean error\n";
				CString txt;
				int imon1=m_pSimDoc->GetMonitor1View();
				switch (imon1) {
				case 0:
					txt="Coefficient of determination";
					break;
				case 1:
					txt="Intercept";
					break;
				case 2:
					txt="Slope";
					break;
				case 3:
					txt="Mean error";
					break;
				}
				cell.SetText(txt);
				cell.SetLabelText(optionlist.c_str());
				SetCell(1,1,&cell);

				GetCell(1,2,&cell);
				cell.SetText("Performance indicators chart4");
				cell.SetBackColor(m_ColorHeading);
				SetCell(1,2,&cell);

				GetCell(1,3,&cell);
				cell.SetCellType(UGCT_DROPLIST);
				optionlist="RootMeanSquareError\nLog Likelihood\n";
				int imon2=m_pSimDoc->GetMonitor1View();
				switch (imon2) {
				case 0:
					txt="RootMeanSquareError";
					break;
				case 1:
					txt="Log Likelihood";
					break;
				}
				cell.SetText(txt);
				cell.SetLabelText(optionlist.c_str());
				SetCell(1,3,&cell);




			}
			else {
				cell.SetReadOnly(true);
				QuickSetText(0,-1,"Monitoring of Single Run Simulation");



				GetCell(0,0,&cell);
				if(m_pSimDoc->m_Simulator.m_pModelInfo->IsPause()) {
					cell.SetBackColor(m_ColorClick);			
					cell.SetText("Single step");
				}
				else {
					cell.SetBackColor(m_ColorHeading);
					cell.SetText("");
				}
				SetCell(0,0,&cell);

				GetCell(1,0,&cell);
				cell.SetText("Simulation speed");
				cell.SetBackColor(m_ColorHeading);
				SetCell(1,0,&cell);

				GetCell(1,1,&cell);
				cell.SetCellType(UGCT_DROPLIST);
				optionlist="Fast\nSmall Delay\nMedium Delay\nLarge Delay\nHuge Delay\n";
				CString txt;
	
				if(m_pSimDoc->m_Simulator.m_pModelInfo->GetRunDelay()==0)
					txt="Fast";
				else if(m_pSimDoc->m_Simulator.m_pModelInfo->GetRunDelay()==1)
					txt="Small Delay";
				else if(m_pSimDoc->m_Simulator.m_pModelInfo->GetRunDelay()==20)
					txt="Medium Delay";
				else if(m_pSimDoc->m_Simulator.m_pModelInfo->GetRunDelay()==1000)
					txt="Large Delay";
				else if(m_pSimDoc->m_Simulator.m_pModelInfo->GetRunDelay()==100000)
					txt="Huge Delay";

				cell.SetText(txt);
				cell.SetLabelText(optionlist.c_str());
				SetCell(1,1,&cell);
			}


			QuickSetText(2,-1,"Save Current Chart To File");
			if(!m_pUtil->m_FileSave.m_SaveFile) QuickSetBackColor(2,-1, m_ColorClick);

			GetCell(0,1,&cell);
			cell.SetCellType(UGCT_CHECKBOX);

			cell.SetReadOnly(false);
			
			if(!m_pSimDoc->m_Simulator.m_pModelInfo->IsPause())
				cell.SetBool(false);
			else
				cell.SetBool(true);

			cell.SetLabelText("Pause Simulation");
			SetCell(0,1,&cell);

			GetCell(0,2,&cell);
			cell.SetCellType(UGCT_CHECKBOX);


			cell.SetBool(false);
			cell.SetLabelText("Stop Simulation");
			SetCell(0,2,&cell);

			GetCell(0,3,&cell);
			cell.SetCellType(UGCT_CHECKBOX);

			if(m_pUtil->m_RemoveLegends==0)
				cell.SetBool(true);
			else
				cell.SetBool(false);
			//cell.SetBool(m_RealTime.GetMultiSimDisplay());
			
			cell.SetLabelText("Display legends for subcharts");
			SetCell(0,3,&cell);


			QuickSetText(-1,-1,"Sub Charts");

			QuickSetText(-3,0,"X");
			QuickSetText(-2,0,"-");
			QuickSetText(-1,0,"+");

			QuickSetText(-3,2,"y");
			QuickSetText(-2,2,"-");
			QuickSetText(-1,2,"+");
			if(m_RealTime.m_NumChartX==0)
				m_RealTime.m_NumChartX=m_pSimDoc->m_AnnimChartX;
			if(m_RealTime.m_NumChartY==0)
				m_RealTime.m_NumChartY=m_pSimDoc->m_AnnimChartY;

			QuickSetNumber(-3,1,m_pProp->m_NumChartX);
			QuickSetText(-2,1,"X Charts");
			QuickSetBackColor(-1,1,m_ColorClick);
			QuickSetNumber(-3,3,m_RealTime.m_NumChartY);
			QuickSetText(-2,3,"Y Charts");

			



}
void ChartCugSheets::Make_Sheet0_ParameterPlot()
{
	CUGCell cell; 
	string str;
	string txt;
	string optionlist;
		UnJoinCells(0,-1);
			UnJoinCells(1,-1);
			UnJoinCells(2,-1);
			UnJoinCells(3,-1);
			SetColWidth(-1,0);
			SetColWidth(0,300);
			SetColWidth(1,200);
			SetColWidth(2,100);
			SetColWidth(3,100);
			SetColWidth(4,100);
			SetColWidth(5,100);

			QuickSetText(0,-1,"Parameter Function");
			QuickSetText(1,-1,"Parameter");
			QuickSetText(2,-1,"New Value");
			QuickSetText(3,-1,"Current Value");
			QuickSetText(4,-1,"Previous Value");
			QuickSetText(5,-1,"Default value");
						
			for(int i=0;i<4;i++){
				GetCell(0,i,&cell);
				cell.SetCellType(UGCT_CHECKBOX);

				if(i==0) {
					txt="Display results with new parameter values";
					cell.SetReadOnly(false);
					if(m_ParameterPlot.m_PlotNewValue)
						cell.SetBool(true);
					else
						cell.SetBool(false);
				}
				else if(i==1) {
					txt="Display results with Default value";
					cell.SetReadOnly(false);
					if(m_ParameterPlot.m_PlotDefault)
						cell.SetBool(true);
					else
						cell.SetBool(false);
				}
				else if(i==2) {
					txt="Display results with Previous value";
					cell.SetReadOnly(false);
					if(m_ParameterPlot.m_PlotPrevious)
						cell.SetBool(true);
					else
						cell.SetBool(false);
				}
				else if(i==3) {
					txt="Apply New value to sim document";
					cell.SetReadOnly(false);
					cell.SetBool(false);
				}

				cell.SetLabelText(txt.c_str());
				SetCell(0,i,&cell);
			}
			double value, newvalue, prevvalue, defvalue;
			size_t line=0;
			bool enabled;
			for(size_t i=0; i<m_ParameterPlot.m_P.size();i++) {
				prevvalue=MISSING;
				for(size_t layer=0; layer<m_ParameterPlot.m_plotp.Opt_Vec.size(); layer++) {
					m_ParameterPlot.m_TableIndex=m_ParameterPlot.m_plotp.Opt_Vec[layer];
					txt=m_ParameterPlot.m_P[i]->GetName().c_str();
					enabled=m_ParameterPlot.m_P[i]->IsEnabled();
					if(m_ParameterPlot.m_P[i]->GetViewFunction()==405) enabled=false;
					if(!enabled) {
						QuickSetBackColor(1,line,m_ColorHeading);
					}
					else
						QuickSetBackColor(1,line,m_NoColor);

					value=m_ParameterPlot.m_P[i]->GetValue(m_ParameterPlot.m_TableIndex);
					defvalue=m_ParameterPlot.m_P[i]->GetOriginalValue(m_ParameterPlot.m_TableIndex);
					newvalue=m_ParameterPlot.m_NewPlotValue[i][m_ParameterPlot.m_TableIndex];
					prevvalue=m_ParameterPlot.m_PreviousValue[i][m_ParameterPlot.m_TableIndex];

					txt+="("+FUtil::STD_ItoAscii(m_ParameterPlot.m_TableIndex+1)+")";

					QuickSetText(1,line,txt);
					GetCell(2,line,&cell);
					cell.SetNumber(newvalue);
					if(!enabled) {
						cell.SetBackColor(m_ColorHeading);
						cell.SetReadOnly(TRUE);
					}
					else cell.SetBackColor(m_NoColor);
					SetCell(2, line, &cell);	

					QuickSetNumber(3,line,value);
					if(prevvalue>-1.E37)
						QuickSetNumber(4,line,prevvalue);
					QuickSetNumber(5,line,defvalue);
					line++;
						
				}
			}
			
			for(size_t i=0; i<m_ParameterPlot.m_Ps.size();i++) {
					prevvalue=MISSING;
					txt=m_ParameterPlot.m_Ps[i]->GetName().c_str();
					enabled=m_ParameterPlot.m_Ps[i]->IsEnabled();
					value=m_ParameterPlot.m_Ps[i]->GetValue();
					defvalue=m_ParameterPlot.m_Ps[i]->GetOriginalValue();
					size_t newindex;
					newvalue = value;
					prevvalue = value;
					if (m_ParameterPlot.m_plotp.Opt_Vec.size() > 0) {
						newindex = min(m_ParameterPlot.m_NewPlotValue.size() - 1, i + m_ParameterPlot.m_P.size());
						if (m_ParameterPlot.m_NewPlotValue[newindex].size() > 0) {
							newvalue = m_ParameterPlot.m_NewPlotValue[newindex][0];
							prevvalue = m_ParameterPlot.m_PreviousValue[newindex][0];
						}
					}
					else {
						if (m_ParameterPlot.m_NewPlotValue.size() > m_ParameterPlot.m_Ps.size())
						{
							newvalue = m_ParameterPlot.m_NewPlotValue[i + m_ParameterPlot.m_P.size()][0];
							prevvalue = m_ParameterPlot.m_PreviousValue[i + m_ParameterPlot.m_P.size()][0];
						}
						else {
							newvalue = m_ParameterPlot.m_NewPlotValue[i ][0];
							prevvalue = m_ParameterPlot.m_PreviousValue[i][0];

						}
					}
					CUGCell cell;

					QuickSetText(1,line,txt);
					if(!enabled) QuickSetBackColor(1,line,m_ColorHeading);
					else QuickSetBackColor(1,line,m_NoColor);

					GetCell(2,line,&cell);
					cell.SetNumber(newvalue);
					if(!enabled) {
						cell.SetBackColor(m_ColorHeading);
						cell.SetReadOnly(TRUE);
					}
					else cell.SetBackColor(m_NoColor);
					SetCell(2, line, &cell);	
					QuickSetNumber(3,line,value);
					GetCell(4,line,&cell);
					cell.SetNumber(prevvalue);
					SetCell(4, line, &cell);
					QuickSetNumber(5,line,defvalue);

					line++;
			}

	/*		GetCell(6,0,&cell);
			cell.SetCellType(UGCT_CHECKBOX);
			cell.SetLabelText("Save All Chart on File");
			if(m_pUtil->m_FileSave.m_SaveFile)
				cell.SetBool(true);
			else
				cell.SetBool(false);
			SetCell(6,0,&cell);

			cell.SetLabelText("Notify File Save to File");
			if(m_pUtil->m_FileSave.m_NotifySaveFile)
				cell.SetBool(true);
			else
				cell.SetBool(false);
			SetCell(6,1,&cell);*/
}
void ChartCugSheets::Make_Sheet0_TablePlot()
{
	CUGCell cell; 
	string str;
	string txt;
	string optionlist;
	UnJoinCells(0,-1);
			UnJoinCells(1,-1);
			UnJoinCells(2,-1);
			UnJoinCells(3,-1);
			SetSH_Width(0);
			SetColWidth(0,0);
			SetColWidth(1,0);
			SetColWidth(2,0);
			SetColWidth(3,0);
			SetColWidth(4,0);
			SetColWidth(5,200);

			QuickSetText(0,-1,"Table Charts");
			QuickSetText(1,-1,"Parameter");
			QuickSetText(2,-1,"New Value");
			QuickSetText(3,-1,"Current Value");
			QuickSetText(4,-1,"Previous Value");
			QuickSetText(5,-1,"Save Current Chart To File");
			if(!m_pUtil->m_FileSave.m_SaveFile) QuickSetBackColor(5,-1, m_ColorClick);
			GetCell(5,0,&cell);
			cell.SetCellType(UGCT_CHECKBOX);
			cell.SetLabelText("Save All Chart on File");
			if(m_pUtil->m_FileSave.m_SaveFile)
				cell.SetBool(true);
			else
				cell.SetBool(false);
			SetCell(5,0,&cell);

			cell.SetLabelText("Notify File Save to File");
			if(m_pUtil->m_FileSave.m_NotifySaveFile)
				cell.SetBool(true);
			else
				cell.SetBool(false);
			SetCell(5,1,&cell);

}
void ChartCugSheets::Make_Sheet0_TimeSeriePlot()
{
	CUGCell cell; 
	string str;
	string txt;
	string optionlist;
			// Time Serie Mode



			SetSH_Width(0);

			for(int i=0;i<9;i++)
				SetColWidth(i,126);
			SetColWidth(1,80);
			SetColWidth(3,80);
			SetColWidth(4,80);
			SetColWidth(5,150);
			SetColWidth(6,150);
			SetColWidth(7,0);
			SetColWidth(8,160);
			if(m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART||m_pProp->IsSecondIndepSet())
				SetColWidth(9,150),

			GetCell(0,2,&cell);
			cell.SetCellType(UGCT_NORMAL);
			cell.SetText("Chart Type");
			cell.SetBackColor(m_ColorHeading);
			SetCell(0,2,&cell);
			JoinCells(0,2,1,2);

			GetCell(0,3,&cell);
			cell.SetCellType(UGCT_DROPLIST);
			if(m_pProp->m_ChartNumber>0) {
				if(m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber)>1||m_pUtil->m_MultiRunPlot)
					optionlist="TimeSerie\nScatter Chart\nFrequency Distribution\nCumulative Frequency Distribution\nContour Chart\n";
				else
					optionlist="TimeSerie\nFrequency Distribution\nCumulative Frequency Distribution\n";
				if(m_pProp->m_ChartType ==CHART_TYPES::TIME_SERIE)
					txt="Time Serie";
				else if(m_pProp->m_ChartType ==CHART_TYPES::SCATTER)
					txt="Scatter Chart";
				else if(m_pProp->m_ChartType ==CHART_TYPES::FREQUENCE)
					txt="Frequency Distribution";
				else if(m_pProp->m_ChartType ==CHART_TYPES::CUM_FREQUENCE)
					txt="Cumulative Frequency Distribution";
				else if(m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART)
					txt="Contour Chart";
				cell.SetText(txt.c_str());
				cell.SetBackColor(m_ColorClick);
				cell.SetLabelText(optionlist.c_str());
				SetCell(0,3,&cell);
			}





			QuickSetText(0,-1,"Zoom/Chart Type/Transformations");				
			JoinCells(0,-1,2,-1);
			//QuickSetText(2,-1,"Transformations");		
			QuickSetText(3,-1,"Annimation");
			JoinCells(3,-1,4,-1);
			QuickSetText(8,-1,"Save Current Chart to File");	
			QuickSetBackColor(8,-1,m_ColorClick);
			QuickSetText(6,-1,"MultiRun Selections");		
			JoinCells(6,-1,7,-1);

			QuickSetText(5,-1,"ReArrange Recalulate");

			// First Line

			GetCell(0,0,&cell);
			cell.SetCellType(UGCT_NORMAL);
			cell.SetText("Zoom In 1/"+FUtil::STD_ItoAscii(m_ZoomLevel));
			if(m_pUtil->m_FileSave.m_ZoomX)
				cell.SetBackColor(m_ColorClick);
			else
				cell.SetBackColor(m_ColorChanged);
			SetCell(0,0,&cell);

			cell.SetText("Zoom Out");
			if(m_pUtil->m_FileSave.m_ZoomX) {
				cell.SetBackColor(m_ColorHeading);
			}
			else
				cell.SetBackColor(m_ColorClick);

			SetCell(1,0,&cell);
			QuickSetText(0,1,"Fixed Zoom Level");



			GetCell(1,1,&cell);

			cell.SetCellType(UGCT_DROPLIST);

			optionlist="Full Range\nYear\nHalf Year\nMonth\nWeek\nDay\nHalf Day\nHour\n";
			int ivis=m_pZoomTimeSerie->GetWindowWidthValue();
			switch (ivis) {
				case 0:
				txt="Full Range";
				cell.SetBackColor(m_NoColor);
				break;
				case 1:
					txt="Year";
					cell.SetBackColor(m_NoColor);
				break;
				case 2:
					txt="Half Year";
					cell.SetBackColor(m_NoColor);
				break;
				case 3:
					txt="Month";
					cell.SetBackColor(m_NoColor);
				break;
				case 4:
					txt="Week";
					cell.SetBackColor(m_NoColor);
				break;
				case 5:
					txt="Day";
					cell.SetBackColor(m_NoColor);
				break;
				case 6:
					txt="Half Day";
					cell.SetBackColor(m_NoColor);
				break;
				case 7:
					txt="Hour";
					cell.SetBackColor(m_NoColor);
				break;
			}
			cell.SetText(txt.c_str());
			cell.SetLabelText(optionlist.c_str());
			SetCell(1,1,&cell);



		//	cell.SetCellType(UGCT_NORMAL);
		//	QuickSetText(0,3,"Data Attributes");
		//	QuickSetBackColor(0,3,m_ColorHeading);
			JoinCells(0,3,1,3);
			JoinCells(0, 4, 1, 4);
			JoinCells(0, 5, 1, 5);
			JoinCells(0, 6, 1, 6);
			JoinCells(0, 7, 1, 7);
			GetCell(0,4,&cell);
		
			GetCell(0, 4, &cell);
			cell.SetCellType(UGCT_NORMAL);
			cell.SetText("X Independent");
			cell.SetBackColor(m_ColorHeading);
			SetCell(0, 4, &cell);
			

			if (!m_pProp->IsMultiPGFilePlot()&& m_pProp->GetPGSource()!=nullptr) {
				GetCell(0, 5, &cell);
				cell.SetCellType(UGCT_DROPLIST);
				optionlist = "Normal Time\n";
				optionlist += "Hour\n";
				optionlist += "Day\n";
				if (m_pProp->GetPGSource()->GetNumberofYears() > 1) optionlist += "Year\n";
				cell.SetLabelText(optionlist);
				string Name = "Normal Time";
				if (m_pProp->GetIndepFirst() == PGHOUR) Name = "Hour";
				if (m_pProp->GetIndepFirst() == PGDAY) Name = "Day";
				if (m_pProp->GetIndepFirst() == PGYEAR) Name = "Year";
				cell.SetText(Name);
				SetCell(0, 5, &cell);

				if (m_pProp->IsFirstIndepSet()) {
					GetCell(0, 6, &cell);
					cell.SetCellType(UGCT_NORMAL);
					cell.SetText("Y Independent");
					cell.SetBackColor(m_ColorHeading);
					SetCell(0, 6, &cell);

					GetCell(0, 7, &cell);
					cell.SetCellType(UGCT_DROPLIST);
					optionlist = "Not Used\n";
					optionlist += "Hour\n";
					optionlist += "Day\n";
					if (m_pProp->GetPGSource()->GetNumberofYears() > 1) optionlist += "Year\n";
					cell.SetLabelText(optionlist);
					Name = "Not Used";
					if (m_pProp->GetIndepSecond()== PGHOUR) Name = "Hour";
					if (m_pProp->GetIndepSecond()== PGDAY) Name = "Day";
					if (m_pProp->GetIndepSecond()== PGYEAR) Name = "Year";


					cell.SetText(Name);
					SetCell(0, 7, &cell);
				}
				if (m_pProp->IsSecondIndepSet()) {
					CPG *pCPG = m_pProp->GetPGSource();
					if (pCPG != nullptr) {
						pCPG->CheckEquiDistance();
						if (pCPG->GetNormalTimeInterval() <= 720 && pCPG->GetNormalTimeInterval() > 0) {
							GetCell(2, 7, &cell);
							string label = m_pProp->DefineYearWithinDayCell(&cell);
							QuickSetText(2, 6, label);
							QuickSetBackColor(2, 6, m_ColorHeading);
							cell.SetBackColor(m_NoColor);
							SetCell(2, 7, &cell);
						}

					}
				}
			}

			GetCell(2,0,&cell);
			cell.SetCellType(UGCT_DROPLIST);
			optionlist="Actual Values\nAll Cumulated\nYear Cumulated\nDay Cumulated\n";
			cell.SetLabelText(optionlist.c_str());
		
			if(m_pProp->m_ReShape.m_Cumulated==0) cell.SetText("Actual Values");
			else if(m_pProp->m_ReShape.m_Cumulated==1) cell.SetText("All Cumulated"); 
			else if(m_pProp->m_ReShape.m_Cumulated==2) cell.SetText("Year Cumulated"); 
			else if(m_pProp->m_ReShape.m_Cumulated==3) cell.SetText("Day Cumulated"); 

			SetCell(2,0,&cell);

			GetCell(2,1,&cell);
			cell.SetCellType(UGCT_CHECKBOX);
			cell.SetLabelText("Derivate Values");		
			
			if(!m_pProp->m_ReShape.m_DerivateValues) {
				cell.SetBool(false);
			}
			else {
				cell.SetBool(true);
			}

			SetCell(2,1,&cell);

			GetCell(2, 2, &cell);
			cell.SetCellType(UGCT_DROPLIST);
			optionlist = "No Stacking\nStacked forward\nStacked backward\n";
			cell.SetLabelText(optionlist.c_str());

			if (m_pProp->m_ReShape.m_StackingVariables == 0) cell.SetText("No Stacking");
			else if (m_pProp->m_ReShape.m_StackingVariables == 1) cell.SetText("Stacked forward");
			else if (m_pProp->m_ReShape.m_StackingVariables == 2) cell.SetText("Stacked backward");
	

			SetCell(2, 2, &cell);


			QuickSetText(2,3,"Vertical Scale");
			QuickSetBackColor(2,3,m_ColorHeading);
			
			cell.SetLabelText("Common");
			if(!m_pProp->m_ReShape.m_CommonScale) {
				cell.SetBool(false);
			}
			else {
				cell.SetBool(true);
			}
			SetCell(2,4,&cell);
			
			cell.SetLabelText("Logaritmic");
			if(!m_pProp->IsActualDistLogScale()) {
				cell.SetBool(false);
			}
			else {
				cell.SetBool(true);

			}
			SetCell(2,5,&cell);
			 
			if(!m_AnimationRunning) {
				cell.SetLabelText("Start Annimation");
				cell.SetBackColor(m_ColorClick);
				cell.SetBool(false);
			}
			else {
				cell.SetLabelText("Stop Annimation");
				cell.SetBackColor(m_ColorChanged);
				cell.SetBool(true);
			}
			SetCell(3,0,&cell);
			JoinCells(3,0,4,0);
			GetCell(3,1,&cell);
			cell.SetCellType(UGCT_CHECKBOX);
			if(m_pUtil->m_FileSave.m_SaveFile) {
				cell.SetBool(true);
				cell.SetLabelText("Stop File Saving");
			}
			else {
				cell.SetBool(false);
				cell.SetLabelText("Start File Saving");
			}
			SetCell(3,1,&cell);
			JoinCells(3,1,4,1);

			QuickSetText(3,2,"Step");
			QuickSetText(3,3,"Speed");


			GetCell(4,2,&cell);

			cell.SetCellType(UGCT_DROPLIST);

			optionlist="Large\nNormal\nSmall\nVery Small\nSmallest Possible\n";
			switch(m_AnnimateStepValue) {
			case 0:
				txt="Large";
				break;
			case 1:
				txt="Normal";
				break;
			case 2:
				txt="Small";
				break;
			case 3:
				txt="Very Small";
				break;
			case 4:
				txt="Smallest Possible";
				break;
			}
		
			cell.SetBackColor(m_NoColor);
			cell.SetText(txt.c_str());
			cell.SetLabelText(optionlist.c_str());
			SetCell(4,2,&cell);

			GetCell(4,3,&cell);

			cell.SetCellType(UGCT_DROPLIST);

			optionlist="Very Slow\nModerate Slow\nNormal\nFast\nVery Fast\n";
			switch(m_AnnimateValue) {
			case 0:
				txt="Very SLow";
				break;
			case 1:
				txt="Slow";
				break;
			case 2:
				txt="Normal";
				break;
			case 3:
				txt="Fast";
				break;
			case 4:
				txt="Very Fast";
				break;
			}
		
			cell.SetBackColor(m_NoColor);
			cell.SetText(txt.c_str());
			cell.SetLabelText(optionlist.c_str());
			SetCell(4,3,&cell);

			if(m_ZoomLevel>2) {
				QuickSetText(3,4,"Forward"+FUtil::STD_ItoAscii(m_Hpos)+"("+FUtil::STD_ItoAscii(m_ZoomLevel)+")");
				QuickSetBackColor(3,4,m_ColorClick);

				QuickSetText(4,4,"Backward");
				QuickSetBackColor(4,4,m_ColorClick);

			}
			else {
				QuickSetText(3,4,"");
				QuickSetBackColor(3,4,m_NoColor);

				QuickSetText(4,4,"");
				QuickSetBackColor(4,4,m_NoColor);
			}


	

			cell.SetCellType(UGCT_CHECKBOX);
			cell.SetLabelText("Within year distribution");
			if(m_TimeSeriePlot.m_ChartInfo.NumOfYears<2) {
				cell.SetReadOnly(true);
				cell.SetBackColor(m_ColorHeading);
			}
			else {
				cell.SetReadOnly(false);
				cell.SetBackColor(m_NoColor);
			}

			if(m_pProp->m_ReShape.m_CommonDayNo)  {
				cell.SetBool(true);
			}
			else {
				cell.SetBool(false);
			}


			SetCell(5,0,&cell);

			cell.SetReadOnly(false);
			cell.SetBackColor(m_NoColor);
			cell.SetLabelText("Within day distribution");
			if(m_pProp->m_ReShape.m_CommonWithDay)  {
				cell.SetBool(true);
			}
			else {
				cell.SetBool(false);
			}
		
			SetCell(5,1,&cell);

			cell.SetLabelText("Normal Distribution");
			if(m_pProp->m_ReShape.m_NormalDist)  {
				cell.SetBool(true);
			}
			else {
				cell.SetBool(false);
			}
		
			SetCell(5,2,&cell);
				
			cell.SetLabelText("Sorted Series");		
			if(m_pProp->m_ReShape.m_SortedSeries)  {
				cell.SetBool(true);

			}
			else {
				cell.SetBool(false);
			}
		
			SetCell(5,3,&cell);
			cell.SetLabelText("Add Reg Line");
			if(m_pProp->m_ReShape.m_RegLine)  {
				cell.SetBool(true);
			}
			else {
				cell.SetBool(false);
			}
		
			SetCell(5,4,&cell);
	
			cell.SetLabelText("Show Missing Value");
			if (m_pProp->m_ReShape.m_ShowMissingValue) {
				cell.SetBool(true);
			}
			else {
				cell.SetBool(false);
			}

			SetCell(5, 5, &cell);


			cell.SetLabelText("Aggregate Time");
			if (m_pProp->m_AggregateTime) {
				cell.SetBool(true);
			}
			else {
				cell.SetBool(false);
			}

			SetCell(5, 6, &cell);





			cell.SetBool(false);
			cell.SetCellType(UGCT_CHECKBOX);
			//JoinCells(6,0,7,0);
			//JoinCells(6,1,7,1);
			//JoinCells(6,2,7,2);
			if(!m_TimeSeriePlot.m_ChartInfo.MultiPGMode||m_pSimDoc==nullptr) {
				cell.SetLabelText("No Multi mode file");
				cell.SetReadOnly(true);
			}
			else if(m_TimeSeriePlot.m_ChartInfo.MultiPGMode) {
				cell.SetReadOnly(false);
				cell.SetLabelText("New Curve");
			}
			SetCell(6,0, &cell);
			
			cell.SetLabelText("New Run (Last var)");			
			
			if(!m_TimeSeriePlot.m_ChartInfo.MultiPGMode||m_pSimDoc==nullptr) {
				cell.SetReadOnly(true);
			}
			else if(m_TimeSeriePlot.m_ChartInfo.MultiPGMode) {
				cell.SetReadOnly(false);
			}

			SetCell(6,1, &cell);
			
			cell.SetLabelText("New Run (All var)");
			if(!m_TimeSeriePlot.m_ChartInfo.MultiPGMode||m_pSimDoc==nullptr) {
				cell.SetReadOnly(true);
			}
			else if(m_TimeSeriePlot.m_ChartInfo.MultiPGMode) {
				cell.SetReadOnly(false);
			}
			SetCell(6,2, &cell);

			if(m_pUtil->m_MultiRunPlot&&m_pSimDoc!=nullptr||m_PGRepit) {
				size_t n=m_pSimDoc->MR_Get_NumTotalPar();
				if(n>0) {
					QuickSetText(6,3,"Independent Variable");
					GetCell(6,4,&cell);

					cell.SetCellType(UGCT_DROPLIST);

					optionlist="Run No";
					optionlist+='\n';

					SimB *pPar;
					int nTabIndex;
					string namep;
					for(size_t i=0;i<m_pSimDoc->MR_GetNumberOfDimensions();i++){
						for(size_t ii=0; ii<m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i+1);ii++){
							pPar=m_pSimDoc->MR_Get_pPar(i+1,ii+1);
							nTabIndex=m_pSimDoc->MR_GetTabIndex(i+1,ii+1);
							namep=pPar->GetName().c_str();
							if(nTabIndex>=0)				
								namep+="("+FUtil::STD_ItoAscii(nTabIndex+1)+")";
							namep+='\n';
							optionlist+=namep;
						}
					}
					enum vartype var=m_pProp->GetFirstIndepVarType();
					enum vartype var2=m_pProp->GetSecondIndepVarType();
					m_pProp->SetFirstIndepVarType(PARVAR);
					m_pProp->SetSecondIndepVarType(PARVAR);
					string selected_str=FUtil::GetOptionString(optionlist,m_pProp->GetFirstIndepIndex()+1);
					cell.SetText(selected_str.c_str());
					cell.SetLabelText(optionlist.c_str());
					SetCell(6,4,&cell);
					if(m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART||m_pProp->IsSecondIndepSet()) {
							QuickSetText(9,-1,"Contour Chart");
							if(m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART) {
								QuickSetText(9,0,"Y axis");
								string sel2=FUtil::GetOptionString(optionlist,m_pProp->GetSecondIndepIndex()+1);
								cell.SetText(sel2.c_str());
								SetCell(9,1,&cell);
							}
					}
					m_pProp->SetFirstIndepVarType(var);
					m_pProp->SetSecondIndepVarType(var2);
				}
			}
			else {
				QuickSetText(6,3,"");
				QuickSetText(6,4,"");


			};
			QuickSetText(6, 5, "Ensemble Time Charts options");
			QuickSetBackColor(6, 5, m_ColorHeading);
			GetCell(6, 6, &cell);
			cell.SetCellType(UGCT_DROPLIST);
			optionlist = "Show One Selected Ensemble \n";
			optionlist += "Show All marked Ensembles \n";
			optionlist += "Show All Ensemble \n";
			m_pUtil->m_EnsembleViewOption=m_pUtil->GetEnsembleViewOption();
			string selected_str = FUtil::GetOptionString(optionlist, m_pUtil->m_EnsembleViewOption);
			
			cell.SetText(selected_str.c_str());
			cell.SetLabelText(optionlist.c_str());

			SetCell(6, 6, &cell);




			GetCell(8,0,&cell);
			cell.SetCellType(UGCT_CHECKBOX);
			cell.SetLabelText("Save All Chart on File");
			if(m_pUtil->m_FileSave.m_SaveFile)
				cell.SetBool(true);
			else
				cell.SetBool(false);
			SetCell(8,0,&cell);

			cell.SetLabelText("Notify File Save to File");
			if(m_pUtil->m_FileSave.m_NotifySaveFile)
				cell.SetBool(true);
			else
				cell.SetBool(false);
			SetCell(8,1,&cell);


			m_pUtil->m_X_AxisAutoScaleSensitivy =  MFC_Util::GetProfileIntNo("x_AXIS_AutoScale", 0);

			QuickSetText(8, 2, "X Axis AutoScaling Options");
			GetCell(8, 3, &cell);

			cell.SetCellType(UGCT_DROPLIST);

			optionlist = "No auto Scale \n";
			optionlist += "Very Wide Space \n";
			optionlist += "Wide Space \n";
			optionlist += "Normal extra Space \n";
			optionlist += "Small Extra Space \n";
			optionlist += "Very Small Extra Space \n";

			selected_str = FUtil::GetOptionString(optionlist, m_pUtil->m_X_AxisAutoScaleSensitivy);

			cell.SetText(selected_str.c_str());
			cell.SetLabelText(optionlist.c_str());
			SetCell(8, 3, &cell);

			if(m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART||m_pProp->IsSecondIndepSet()) {
						GetCell(9,2,&cell);
						cell.SetCellType(UGCT_CHECKBOX);
						if(m_TimeSeriePlot.m_Contour.ScatterContourPoints) {
							cell.SetLabelText("Remove Scatter Points");
							cell.SetBool(true);
						}
						else {
							cell.SetLabelText("Add Scatter Points");
							cell.SetBool(false);
						}
						SetCell(9,2,&cell);		
						GetCell(9,3,&cell);
						cell.SetCellType(UGCT_CHECKBOX);
						if(m_TimeSeriePlot.m_Contour.SplineContourInterPolation) {
							cell.SetLabelText("Remove Spline Interpolation");
							cell.SetBool(true);
						}
						else {
							cell.SetLabelText("Use Spline Interpolation");
							cell.SetBool(false);
						}
						SetCell(9,3,&cell);	
						GetCell(9,4,&cell);
						cell.SetCellType(UGCT_CHECKBOX);
						if(m_TimeSeriePlot.m_Contour.ScaleContourChart) {
							cell.SetLabelText("Use Automatic scaling");
							cell.SetBool(true);
						}
						else {
							cell.SetLabelText("Use Maximal chart size");
							cell.SetBool(false);
						}
						SetCell(9,4,&cell);	



			}


			QuickSetText(-1,-1,"Sub Charts");

			QuickSetText(-3,0,"X");
			QuickSetText(-2,0,"-");
			QuickSetText(-1,0,"+");

			QuickSetText(-3,2,"y");
			QuickSetText(-2,2,"-");
			QuickSetText(-1,2,"+");
			QuickSetNumber(-3,1,m_pProp->m_NumChartX);
			QuickSetText(-2,1,"X Charts");
			QuickSetBackColor(-1,1,m_ColorClick);
			QuickSetNumber(-3,3,m_pProp->m_NumChartY);
			QuickSetText(-2,3,"Y Charts");
			QuickSetText(-3,4,"Swap x-y");
			QuickSetBackColor(-3,4,m_ColorClick);



}
void ChartCugSheets::Make_Sheet1_General()
{
	CUGCell cell; 
	string str;
	string txt;
	string optionlist;
	
		//SetLineWidth(5,60);
		SetColWidth(-1,20);
		SetRowHeight(5,60);
		for(int i=0;i<10;i++)
			SetColWidth(i,140);
			QuickSetText(0,-1,"Title");				
			QuickSetText(1,-1,"Title Font Size");		
			QuickSetText(2,-1,"Y Label Font Size");
			QuickSetText(3,-1,"Legent Font Size");				
			QuickSetText(4,-1,"Line Width");		
			QuickSetText(5,-1,"Axis Line Width");
			QuickSetText(6,-1,"Symbol Size");
			QuickSetText(7,-1,"Axis Scale Font Size");
			QuickSetText(8,-1,"Tick X size");
			QuickSetText(9,-1,"Tick Y size");



		QuickSetText(0,0,m_pProp->m_Title, false);
		QuickSetNumber(1,0,m_pUtil->m_TitleSizePoint, false);
		QuickSetNumber(2,0,m_pUtil->m_LabelSizePoint, false);
		QuickSetNumber(3,0,m_pUtil->m_LegendSizePoint, false);
		QuickSetNumber(4,0,m_pUtil->m_LineWidthPoint, false);
		QuickSetNumber(5,0,m_pUtil->m_LineAxisWidthPoint, false);

		QuickSetNumber(6,0,m_pUtil->m_SymbolSizePoint, false);
		QuickSetNumber(7,0,m_pUtil->m_AxisScalePoint, false);
		QuickSetNumber(8,0,m_pUtil->m_MinTickDistX, false);
		QuickSetNumber(9,0,m_pUtil->m_MinTickDistY, false);


		for(int i=0;i<10;i++) {
			QuickSetBackColor(i,1,m_ColorHeading);	
			QuickSetBackColor(i,3,m_ColorHeading);
			QuickSetBackColor(i,5,m_ColorHeading);

		}

		//QuickSetText(0,1,"Num ChartX");
		//QuickSetText(1,1,"Num ChartY");
		QuickSetText(2,1,"Top Margin");
		QuickSetText(3,1,"Bottom Margin");
		QuickSetText(4,1,"Left Margin");
		QuickSetText(5,1,"Right Margin");
		QuickSetText(6,1,"Gap Margin");
		QuickSetText(7,1,"Gap Horiz Percentage");
		QuickSetText(8,1,"Gap Left/Right Percentage");
		QuickSetText(9,1,"Gap Top/bottom Percentage");
		
		QuickSetText(0,1,"X Size of Chart");
		QuickSetText(1,1,"Y Size of Chart");


		QuickSetText(0,3,"X Legend Pos");
		QuickSetText(1,3,"Y Legend Pos");
		QuickSetText(2,3,"Legend Style");
		QuickSetText(3,3,"Legend Box");
		QuickSetText(4,3,"Legend Box Frame");
		QuickSetText(5,3,"Legend Content Style");
		QuickSetText(6,3,"Legend Apperance");
		QuickSetText(7,3,"Chart Background Color");
		QuickSetText(8,3,"Chart Background");
		QuickSetText(9,3,"SubChart Color");
		QuickSetCellTypeEx(0,5,UGCT_NORMALMULTILINE);
		QuickSetCellTypeEx(1,5,UGCT_NORMALMULTILINE);
		QuickSetCellTypeEx(2,5,UGCT_NORMALMULTILINE);
		QuickSetCellTypeEx(4,5,UGCT_NORMALMULTILINE);
		QuickSetCellTypeEx(5,5,UGCT_NORMALMULTILINE);
		QuickSetCellTypeEx(6,5,UGCT_NORMALMULTILINE);
		QuickSetCellTypeEx(7,5,UGCT_NORMALMULTILINE);
		QuickSetCellTypeEx(8,5,UGCT_NORMALMULTILINE);
		QuickSetCellTypeEx(9,5,UGCT_NORMALMULTILINE);
		QuickSetText(0,5,"Dimmer Whisker Box\n(0-255)");
		QuickSetText(1,5,"Dimmer Whisker Range\n(0-255)");
		QuickSetText(2,5,"Whisker Box\n Edge Color");
		QuickSetText(3,5,"Uncertainty Style");
		QuickSetText(4,5,"Error Signal\nin validation Charts");
		QuickSetText(5,5,"Residuals\nIn validation charts");
		QuickSetText(6,5,"Include all Points\nIn MultiRun charts");
		QuickSetText(7,5,"Remove X axis scale\n In MultiRun charts");
		QuickSetText(8,5,"Remove Y axis scale\n In MultiRun charts");
		QuickSetText(9,5,"Remove Top Title\n In MultiRun charts");
		QuickSetText(-1,0,"X");
		QuickSetNumber(-1,1,m_pProp->m_NumChartX, false);
		QuickSetBackColor(-1,1,m_ColorClick);
		QuickSetText(-1,2,"Y");
		QuickSetNumber(-1,3,m_pProp->m_NumChartY, false);
		QuickSetBackColor(-1,3,m_ColorClick);
		QuickSetNumber(0,2,m_pProp->m_DisplayXSize-180, false);
		QuickSetNumber(1,2,m_pProp->m_DisplayYSize-28, false);
		QuickSetNumber(2,2,m_pProp->m_TopMargin), false;
		QuickSetNumber(3,2,m_pProp->m_BottomMargin, false);
		QuickSetNumber(4,2,m_pProp->m_LeftMargin, false);
		QuickSetNumber(5,2,m_pProp->m_RightMargin, false);
		QuickSetNumber(6,2,m_pProp->m_GapMargin, false);
		QuickSetNumber(7,2,m_pUtil->m_GapHFraction*100, false);
		QuickSetNumber(8,2,m_pUtil->m_GapHorLeftFraction*100, false);
		QuickSetNumber(9,2,m_pUtil->m_GapVertTopFraction*100, false);



		QuickSetNumber(0,4,m_pUtil->m_LegendPositionX, false);
		QuickSetNumber(1,4,m_pUtil->m_LegendPositionY, false);
		
		GetCell(2,4,&cell);
		cell.SetCellType(UGCT_DROPLIST);
		optionlist="Horizontal\nVertical\n";

		if(m_pProp->GetVerticalLegends())
			txt="Vertical";
		else
			txt="Horizontal";
		cell.SetText(txt.c_str());
		cell.SetLabelText(optionlist.c_str());
		SetCell(2,4,&cell);
		
		GetCell(3,4,&cell);
		cell.SetCellType(UGCT_DROPLIST);

		optionlist="Transparent\nHigh Transparancy\nNormal Transparancy\nLimited Transparancy\nNo Tranparancy\n";
		int ivis=m_pProp->GetVisibleFrames();
		switch (ivis) {
			case 0:
			txt="Transparent";
			cell.SetBackColor(m_NoColor);
			break;
			case 1:
				txt="High Tranparancy";
				cell.SetBackColor(m_NoColor);
			break;
			case 2:
				txt="Normal Tranparancy";
				cell.SetBackColor(m_NoColor);
			break;
			case 3:
				txt="Limited Tranparancy";
				cell.SetBackColor(m_NoColor);
			break;
			case 4:
				txt="No Tranparancy";
				cell.SetBackColor(m_NoColor);
			break;
		}
		cell.SetText(txt.c_str());
		cell.SetLabelText(optionlist.c_str());
		SetCell(3,4,&cell);

		GetCell(4,4,&cell);
		cell.SetCellType(UGCT_DROPLIST);
		cell.SetText(txt.c_str());
		cell.SetLabelText(optionlist.c_str());
		SetCell(4,4,&cell);		

// Legend content
		optionlist="Extended\nContext Sensitive\nMinimal Style\n";
		ivis=m_pUtil->GetLegendContentStyle();
		switch (ivis) {
			case 0:
				txt="Extended";
					break;
			case 1:
				txt="Context Sensitive";
				break;
			case 2:
				txt="Minimal Style";
				break;
		}
		GetCell(5,4,&cell);
		cell.SetCellType(UGCT_DROPLIST);
		cell.SetText(txt.c_str());
		cell.SetLabelText(optionlist.c_str());
		SetCell(5,4,&cell);	

// Legend Repetition
		optionlist="All SubCharts\nOnly for first SubChart\nNo Legends\n";


	
		switch (m_pUtil->m_RemoveLegends) {
			case 0:
				txt="All SubCharts";
					break;
			case 1:
				txt="Only for first SubChart";
				break;
			case 2:
				txt="No Legends";
				break;
		}


		GetCell(6,4,&cell);
		cell.SetCellType(UGCT_DROPLIST);
		cell.SetText(txt.c_str());
		cell.SetLabelText(optionlist.c_str());
		SetCell(6,4,&cell);	
		
		QuickSetBackColor(7,4,m_pUtil->GetColorBackground());

		GetCell(8,4,&cell);
		cell.SetCellType(UGCT_DROPLIST);
		optionlist="Sharp\nRounded\n";
		if(m_pProp->GetRoundedFrame()) {
			txt="Rounded";
		}
		else { 
			txt="Sharp";
		}
		cell.SetText(txt.c_str());
		cell.SetLabelText(optionlist.c_str());
		SetCell(8,4,&cell);


		GetCell(9,4,&cell);
		cell.SetCellType(UGCT_DROPLIST);
		optionlist="Independent\nCommon\n";
		if(m_pProp->GetCommonColors()) {
			txt="Common";
		}
		else { 
			txt="Independent";
		}
		cell.SetText(txt.c_str());
		cell.SetLabelText(optionlist.c_str());
		SetCell(9,4,&cell);
		
		
		
		
		long iv=m_pProp->m_BoxDimmer;
		QuickSetNumber(0,6,iv, false);
		iv=m_pProp->m_BoxRangeDimmer;
		QuickSetNumber(1,6,iv, false);
		QuickSetBackColor(2,6, m_pUtil->m_BoxEdgeColor);

		GetCell(3,6,&cell);
		cell.SetCellType(UGCT_DROPLIST);
		optionlist="Whisker Box style\nBands around min and max\nBands around 1 StDev\nBands around 90 % interval\n";
		switch (m_pProp->GetChartError()) {
			case 0:
				txt="Whisker Box style";
				break;
			case 2:
				txt="Bands around 1 StDev";
				break;
			case 1:
				txt="Bands around min and max";
				break;
			case 3:
				txt="Bands around 90 % interval";
				break;
		}
		cell.SetText(txt.c_str());
		cell.SetLabelText(optionlist.c_str());
		SetCell(3,6,&cell);

		GetCell(4,6,&cell);
		cell.SetCellType(UGCT_DROPLIST);
		optionlist="Yes\nNo\n";
		switch (m_pUtil->GetErrorSignal()) {
			case 0:
				txt="No";
				break;
			case 1:
				txt="Yes";
				break;
		}
		cell.SetText(txt.c_str());
		cell.SetLabelText(optionlist.c_str());
		SetCell(4,6,&cell);
		
		GetCell(5,6,&cell);
		cell.SetCellType(UGCT_DROPLIST);
		optionlist="No\nYes\nLogLikelihood\nNew LogLikelihood\n";
		switch (m_pUtil->GetResidualRepresentation()) {
			case 0:
				txt="No";
				break;
			case 1:
				txt="Yes";
				break;
			case 2:
				txt="LogLikelihood";
				break;
			case 3:
				txt="New LogLikelihood";
				break;
		}
		cell.SetText(txt.c_str());
		cell.SetLabelText(optionlist.c_str());
		SetCell(5,6,&cell);


		GetCell(7,6,&cell);
		cell.SetCellType(UGCT_DROPLIST);
		optionlist="Always\nSensitive\nNever\n";
		switch (m_pUtil->GetRemoveXAxisScale()) {
			case 0:
				txt="Always";
				break;
			case 1:
				txt="Sensitive";
				break;
			case 2:
				txt="Never";
				break;
		}
		cell.SetText(txt.c_str());
		cell.SetLabelText(optionlist.c_str());
		SetCell(7,6,&cell);
		
		GetCell(8,6,&cell);
		cell.SetCellType(UGCT_DROPLIST);
		optionlist="Always\nSensitive\nNever\n";

		switch (m_pUtil->GetRemoveYAxisScale()) {
				case 0:
				txt="Always";
				break;
			case 1:
				txt="Sensitive";
				break;
			case 2:
				txt="Never";
				break;
		}
		cell.SetText(txt.c_str());
		cell.SetLabelText(optionlist.c_str());
		SetCell(8,6,&cell);
		
		GetCell(9,6,&cell);
		cell.SetCellType(UGCT_DROPLIST);
		optionlist="Always\nSensitive\nNever\n";

		switch (m_pUtil->GetRemoveTitle()) {
				case 0:
				txt="Always";
				break;
			case 1:
				txt="Sensitive";
				break;
			case 2:
				txt="Never";
				break;
		}
		cell.SetText(txt.c_str());
		cell.SetLabelText(optionlist.c_str());
		SetCell(9,6,&cell);

	
	
	
}//
void ChartCugSheets::Make_Sheet2_RealTime()
{
	CUGCell cell; 
	string str;

	string optionlist;
			QuickSetText(0,-1,"Module");
				QuickSetText(1,-1,"Parameter");
				QuickSetText(2,-1,"Pick");
				SetColWidth(0,120);
				SetColWidth(1,220);
				SetColWidth(2,40);
	
				int ind=0, colorline=1;
				CString txt;
				int npar=m_pSimDoc->MR_Get_NumTotalPar()+1;
				for(size_t i=0;i<m_pSimDoc->MR_GetNumberOfDimensions();i++){
						for(size_t ii=0; ii<m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i+1);ii++){
								if(m_pSimDoc->MR_GetMethod(i+1,ii+1)==9||m_pSimDoc->MR_GetMethod(i+1,ii+1)==11||m_pSimDoc->MR_GetMethod(i+1,ii+1)<=3) {	
									GetCell(0,ind,&cell);
									cell.SetText(m_pSimDoc->MR_GetParGroup(i+1,ii+1).c_str());
									cell.SetParam(i);
									SetCell(0,ind,&cell);
									GetCell(1,ind,&cell);
									cell.SetText(m_pSimDoc->MR_Get_CalibParName(ind).c_str());
									cell.SetParam(ii);
									SetCell(1,ind,&cell);
									GetCell(2,ind,&cell);
									cell.SetCellType(UGCT_CHECKBOX);
									if(m_pSimDoc->MR_GetMonitoring(i+1,ii+1)>0) {
										cell.SetBool(true);
										QuickSetBackColor(0,ind,MFC_Util::GetColorLine(colorline));
										colorline++;
									}
									else
										cell.SetBool(false);
									SetCell(2,ind,&cell);
									ind++;
								}
						}
	
				}

}
void ChartCugSheets::Make_Sheet3_ParameterPlot()
{
	CUGCell cell; 
	string str;
	string txt;
	string optionlist;
	SetColWidth(-1,0);
				for(size_t i=0; i<m_ParameterPlot.m_MaxLayer;i++) {
					GetCell(0,i,&cell);
					cell.SetCellType(UGCT_CHECKBOX);
					cell.SetBool(false);
					txt=FUtil::STD_ItoAscii(i+1);
					cell.SetLabelText(txt.c_str());
					SetCell(0,i,&cell);



				}
				if(m_ParameterPlot.m_pParTable==nullptr) return;
				size_t n=m_ParameterPlot.m_pParTable->GetNumbersToPlot();
				for(size_t i=0;i<n;i++) {
						size_t l;
						l=m_ParameterPlot.m_pParTable->GetPlotIndex(i);
						GetCell(0,l,&cell);
						cell.SetBool(true);
						SetCell(0,l,&cell);
				}

}
void ChartCugSheets::Make_Sheet2_ParameterPlot()
{
	CUGCell cell; 
	string str;
	string txt;
	string optionlist;
	Func *funcpointer=m_ParameterPlot.GetCurrentFuncPointer();
	SetColWidth(-1,0);
	SetColWidth(1,200);

	PLOT_PARAM plotparam=funcpointer->GetPlotParam();
	QuickSetText(0,-1,"Axis");
	QuickSetText(1,-1,"Legend");
	QuickSetText(2,-1,"Log");
	QuickSetText(3,-1, "Min");
	QuickSetText(4,-1,"Max");
	QuickSetText(0,0,"X-Axis");
	QuickSetText(1,0,plotparam.X_Label.c_str(), false);
	GetCell(2,0,&cell);
	cell.SetCellType(UGCT_CHECKBOX);
	if(plotparam.LogXScale)
		cell.SetBool(TRUE);
	else
		cell.SetBool(FALSE);
	SetCell(2,0,&cell);
	QuickSetNumber(3,0,plotparam.X_Min, false);
	QuickSetNumber(4,0,plotparam.X_Max, false);
	QuickSetText(0,1,"Y-Axis");
	QuickSetText(1,1,plotparam.Y_Label.c_str(), false);
	GetCell(2,1,&cell);
	cell.SetCellType(UGCT_CHECKBOX);
	if(plotparam.LogYScale)
		cell.SetBool(TRUE);
	else
		cell.SetBool(FALSE);
	SetCell(2,1,&cell);
	for(size_t i=0; i<plotparam.LegendsVec.size();i++) {
		QuickSetText(0,2+i, "Legend ("+FUtil::STD_ItoAscii(i+1)+")");
		QuickSetText(1,i+2,plotparam.LegendsVec[i].c_str(), false);
	}
	


}
void ChartCugSheets::Make_Sheet2_TimeSeriePlot(bool RightLabels, bool XLabels, bool ZLabels)
{
	// Axis
	CUGCell cell; 
	string str;
	string txt;
	string optionlist;

				int lineacc;
				SetColWidth(-1,20);
				SetColWidth(0,240);
				for(int i=1;i<6;i++)
					SetColWidth(i,106);

				string legend = m_pProp->GetChartTypeString();
				legend += "Label on axis";
				GetCell(0, -1, &cell);
				cell.SetText(legend);
				cell.SetBackColor(m_ColorClick);
				SetCell(0, -1, &cell);


			//	QuickSetText(0,-1,"Labels on axis");
				QuickSetText(1,-1,"Min Value");
				QuickSetText(2,-1,"Max Value");
				QuickSetText(3,-1,"Grid Value");
				QuickSetText(4,-1,"Axis");
				QuickSetText(5,-1,"SubChart");

				size_t ratio = m_TimeSeriePlot.m_ChartInfo.NumVariables / m_TimeSeriePlot.m_ChartInfo.NumPriorVariables;

				if (m_pUtil->m_MultiRunPlot&&m_pProp->GetFirstIndepVarType()==MULTIRUN_NUM)
						m_NumUsedSubCharts = min(m_TimeSeriePlot.m_ChartInfo.NumPriorVariables, m_NumUsedSubCharts);
				else if(m_pUtil->m_MultiRunPlot)
						m_NumUsedSubCharts = min(m_TimeSeriePlot.m_ChartInfo.NumPriorVariables, m_NumUsedSubCharts);
				else
					 m_NumUsedSubCharts=min(m_TimeSeriePlot.m_ChartInfo.NumVariables, m_NumUsedSubCharts);

				size_t numy = min(m_NumUsedSubCharts, m_TimeSeriePlot.m_ChartInfo.NumVariables);
				if(m_pProp->IsSecondIndepSet())
					numy=1;
				


				for (size_t jc=0;jc<numy;jc++) {
					if (m_pProp->m_YScaleLeftMin.size()>0) {
						QuickSetNumber(-1, jc, jc + 1);
						if (m_pProp->m_YLabel1.size() > jc) {
							string txt = m_pProp->m_YLabel1.at(jc);
							//if (!RightLabels) txt = m_pProp->GetLegend(jc);

							QuickSetText(0, jc, txt, false);
							if(m_pProp->isYLabel1Changed(jc))
								QuickSetBackColor(0, jc, m_ColorChanged);
							else
								QuickSetBackColor(0, jc, m_ColorClick);


							if (m_pProp->m_YScaleLeftMin.size() > jc) {
								QuickSetNumber(1, jc, m_pProp->m_YScaleLeftMin.at(jc), false);
								QuickSetNumber(2, jc, m_pProp->m_YScaleLeftMax.at(jc), false);
								QuickSetNumber(3, jc, m_pProp->m_YScaleLeftInterv.at(jc), false);
								if (m_pProp->isLeftYScaleChanged(int(jc)))
									for(int iv=1; iv<4; iv++) QuickSetBackColor(iv, jc, m_ColorChanged);
								else
									for (int iv = 1; iv<4; iv++) QuickSetBackColor(iv, jc, m_ColorClick);
							}
						}
						QuickSetText(4,jc,"Y Left");	
						if(m_pProp->IsSecondIndepSet())
							QuickSetText(5, jc,"");
						else
							QuickSetNumber(5,jc,jc+1);
					}
				}
				lineacc=numy;



				if(RightLabels&&!m_pProp->IsSecondIndepSet()){
					for (size_t jc=0;jc<m_NumUsedSubCharts;jc++) {
						QuickSetNumber(-1,m_NumUsedSubCharts+jc,jc+1);
						QuickSetText(0,m_NumUsedSubCharts+jc,m_pProp->m_YLabel2.at(jc));
						if(m_pProp->isYLabel2Changed(jc))
							QuickSetBackColor(0, m_NumUsedSubCharts + jc, m_ColorChanged);
						else
							QuickSetBackColor(0, m_NumUsedSubCharts + jc, m_ColorChanged);

						if (m_pProp->m_YScaleRightMin.size() > jc) {
							QuickSetNumber(1, m_NumUsedSubCharts + jc, m_pProp->m_YScaleRightMin.at(jc), false);
							QuickSetNumber(2, m_NumUsedSubCharts + jc, m_pProp->m_YScaleRightMax.at(jc), false);
							QuickSetNumber(3, m_NumUsedSubCharts + jc, m_pProp->m_YScaleRightInterv.at(jc), false);
							if (m_pProp->isRightYScaleChanged(int(jc)))
								for (int iv = 1; iv<4; iv++) QuickSetBackColor(iv, m_NumUsedSubCharts+jc, m_ColorChanged);
							else
								for (int iv = 1; iv<4; iv++) QuickSetBackColor(iv, m_NumUsedSubCharts+jc, m_ColorClick);




						}
						QuickSetText(4,m_NumUsedSubCharts+jc,"Y Right");
						QuickSetNumber(5,m_NumUsedSubCharts+jc,jc+1, false);
					}
					lineacc=2*m_NumUsedSubCharts;
				}
				if(XLabels||m_pProp->GetIndepFirst()!=DATEAXIS) /*||m_pProp->IsSecondIndepSet()*/ {

					for (size_t jc=0;jc<numy;jc++) {
						QuickSetNumber(-1,lineacc+jc,jc+1);
						if (m_pProp->m_XLabel.size() > jc) {
							QuickSetText(0, lineacc + jc, m_pProp->m_XLabel.at(jc), false);
							if(m_pProp->isXLabelChanged(jc))
								QuickSetBackColor(0, lineacc + jc, m_ColorChanged);
							else
								QuickSetBackColor(0, lineacc + jc, m_ColorClick);

						}
						if((m_pProp->m_ChartType!= CHART_TYPES::TIME_SERIE||m_pProp->m_ReShape.m_SortedSeries||m_pProp->m_ReShape.m_NormalDist)&&(!m_pProp->IsSecondIndepSet()|| m_pProp->GetFirstIndepVarType() != DATEAXIS)) {
							if (jc < m_pProp->m_XScaleMin.size()) {
								QuickSetNumber(1, lineacc + jc, m_pProp->m_XScaleMin.at(jc), false);
								QuickSetNumber(2, lineacc + jc, m_pProp->m_XScaleMax.at(jc), false);
								QuickSetNumber(3, lineacc + jc, m_pProp->m_XScaleInterv.at(jc), false);
								if (m_pProp->isXScaleChanged(int(jc)))
									for (int iv = 1; iv<4; iv++) QuickSetBackColor(iv, lineacc + jc, m_ColorChanged);
								else
									for (int iv = 1; iv<4; iv++) QuickSetBackColor(iv, lineacc + jc, m_ColorClick);

							}
						} 

						else  if(!m_pUtil->m_MultiRunPlot&&m_pProp->GetFirstIndepVarType()==DATEAXIS)  {
							size_t min=size_t(m_pProp->m_TimeScale.MinX/60+2880.);
							string txt=PGUtil::StringDatum(min);
							QuickSetText(1,lineacc+jc ,txt, false);
							min=size_t(m_pProp->m_TimeScale.MaxX/60+2880.);
							txt=PGUtil::StringDatum(min);
							QuickSetText(2,lineacc+jc ,txt, false);
							if(m_pProp->m_TimeScale.IntervX>86400)
								min=size_t(m_pProp->m_TimeScale.IntervX/60);
							else if(m_pProp->m_TimeScale.interv>60)
								min=size_t(m_pProp->m_TimeScale.IntervX/60);
							else
								min=0;
							if(min==0)
								QuickSetText(3,lineacc+jc ,"", false);
							else {
								txt=FUtil::STD_ItoAscii(min);
								QuickSetText(3,lineacc+jc,txt, false);
							}
							if (m_pProp->isXScaleChanged(int(jc)))
								for (int iv = 1; iv<4; iv++) QuickSetBackColor(iv, lineacc + jc, m_ColorChanged);
							else
								for (int iv = 1; iv<4; iv++) QuickSetBackColor(iv, lineacc + jc, m_ColorClick);



						}
						else if (m_pUtil->m_MultiRunPlot || m_pUtil->m_MultiPGPlot || m_pProp->GetSecondIndepVarType() != DATEAXIS) {
							if ((m_pProp->GetFirstIndepIndex() != std::string::npos&&m_pProp->IsSecondIndepSet() && m_pProp->GetSecondIndepVarType() == DATEAXIS)|| m_pProp->GetFirstIndepVarType()==MULTIRUN_NUM) {
								if (m_pProp->GetIndepFirst() == MULTIRUN_NUM) {
									QuickSetNumber(1, lineacc + jc, m_pProp->GetXmin(jc), false);
									QuickSetNumber(2, lineacc + jc, m_pProp->GetXmax(jc), false);
									QuickSetNumber(3, lineacc + jc, m_pProp->GetXInterv(jc), false);



								}
								else {
									QuickSetNumber(1, lineacc + jc, m_pProp->m_TimeScale.min, false);
									QuickSetNumber(2, lineacc + jc, m_pProp->m_TimeScale.max, false);
									QuickSetNumber(3, lineacc + jc, m_pProp->m_TimeScale.interv, false);
								}
							}
							else if (jc < m_pProp->m_XScaleMin.size()) {
								QuickSetNumber(1, lineacc + jc, m_pProp->m_XScaleMin.at(jc), false);
								QuickSetNumber(2, lineacc + jc, m_pProp->m_XScaleMax.at(jc), false);
								QuickSetNumber(3, lineacc + jc, m_pProp->m_XScaleInterv.at(jc), false);
							}
							if(m_pProp->m_CommonXScale&&jc>0)
								for (int iv = 1; iv<4; iv++) QuickSetBackColor(iv, lineacc + jc, m_ColorHeading);
							else if(m_pProp->isXScaleChanged(int(jc)))
								for (int iv = 1; iv<4; iv++) QuickSetBackColor(iv, lineacc + jc, m_ColorChanged);
							else
								for (int iv = 1; iv<4; iv++) QuickSetBackColor(iv, lineacc + jc, m_ColorClick);


						}
						string str="X Scale";
						if (m_pProp->m_CommonXScale&&jc>0)
							str += " Common";

						QuickSetText(4,lineacc+jc,str, false);
						if(!m_pProp->IsSecondIndepSet()) QuickSetNumber(5,lineacc+jc,jc+1, false);
						if (jc > 0) {
							for (size_t coli = 1; coli < 4; coli++) {
								GetCell(coli, lineacc + jc, &cell);
								if (!m_pProp->m_CommonXScale)
									cell.SetReadOnly(FALSE);
								else 
									cell.SetReadOnly(TRUE);
						
								SetCell(coli, lineacc + jc, &cell);
							}
						}


					}
					lineacc+=numy;
				}
				if(ZLabels&&m_pProp->m_ZLabel.size()>0) {
					for (size_t jc=0;jc<m_NumUsedSubCharts;jc++) {
						QuickSetNumber(-1,lineacc+jc,jc+1);
						QuickSetText(0,lineacc+jc,m_pProp->m_ZLabel.at(jc), false);
						if(m_pProp->isZLabelChanged(jc))
							QuickSetBackColor(0, lineacc + jc, m_ColorChanged);
						else
							QuickSetBackColor(0, lineacc + jc, m_ColorClick);


						string str = "Z Scale";
						if (m_pProp->m_CommonXScale&&jc>0)
							str += " Common";

						QuickSetText(4,lineacc+jc,str);
						if (m_pProp->m_ZScaleMin.size() > jc) {
							QuickSetNumber(1, lineacc + jc, m_pProp->m_ZScaleMin.at(jc), false);
							QuickSetNumber(2, lineacc + jc, m_pProp->m_ZScaleMax.at(jc), false);
							QuickSetNumber(3, lineacc + jc, m_pProp->m_ZScaleInterv.at(jc), false);
							if (m_pProp->isZScaleChanged(int(jc)))
								for (int iv = 1; iv<4; iv++) QuickSetBackColor(iv, lineacc + jc, m_ColorChanged);
							else
								for (int iv = 1; iv<4; iv++) QuickSetBackColor(iv, lineacc + jc, m_ColorClick);


						}
						if (jc > 0) {
							for (size_t coli = 1; coli < 4; coli++) {
								GetCell(coli, lineacc + jc, &cell);
								/*if (!m_pProp->m_CommonScale)
									cell.SetReadOnly(FALSE);
								else
									cell.SetReadOnly(TRUE);*/

								SetCell(coli, lineacc + jc, &cell);
							}
						}



						QuickSetNumber(5,lineacc+jc,jc+1, false);
					}



				}
			}
void ChartCugSheets::Make_Sheet3_RealTime(int NumTSV)
{
	CUGCell cell; 
	string str;
	int FileNo;
	F *pValFile;

	string optionlist;
			QuickSetText(0,-1,"Measured");
				QuickSetText(1,-1,"Simulated");
				QuickSetText(2,-1,"Pick");
				SetColWidth(0,220);
				SetColWidth(1,220);
				SetColWidth(2,40);
				CString test;
				int colorcount=0;
				for(int i=0; i<NumTSV;i++) {
					QuickSetNumber(-1,i,i+1);
					int ind=m_pSimDoc->m_ValidationData.GetValFileIndex(i);
					FileNo=m_pSimDoc->m_ValidationData.GetValFileNumber(i);
					pValFile=m_pSimDoc->ValidationFilePointer(FileNo);
					test=pValFile->GetDataDescription(ind+1).c_str();
					QuickSetText(0,i, test);
					test=m_pSimDoc->m_ValidationData.GetValName(i).c_str();
					QuickSetText(1,i, test);
					GetCell(2,i,&cell);
					cell.SetCellType(UGCT_CHECKBOX);
					if(m_pSimDoc->m_ValidationData.GetValMonitoring(i)>0) {
						colorcount++;
						cell.SetBool(true);
						QuickSetBackColor(0,i,MFC_Util::GetColorLine(colorcount));
					}
					else
						cell.SetBool(false);
					SetCell(2,i,&cell);

				}
}
void ChartCugSheets::Make_Sheet3_TimeSeriePlot()
{
	CUGCell cell; 
	string str;
	string txt;

	string optionlist;
	int  ArrayIndex;

			SetColWidth(-1,20);
			SetColWidth(0,240);
			SetColWidth(1,50);
			for(int i=2;i<6;i++)
				SetColWidth(i,114);
			for(int i=6;i<9;i++)
				SetColWidth(i,60);

			string legend = m_pProp->GetChartTypeString();
			legend += "Legend";
			GetCell(0, -1, &cell);
			cell.SetText(legend);
			cell.SetBackColor(m_ColorClick);
			SetCell(0, -1, &cell);


			QuickSetText(1,-1,"Color");
			QuickSetText(2,-1,"Line Width");
			QuickSetText(3,-1,"Line style");
			QuickSetText(4,-1,"Symbol point");
			QuickSetText(5,-1,"Symbol size");
			QuickSetText(6,-1,"SubChart no");
			QuickSetText(7,-1,"Axis");
			QuickSetText(8,-1,"Var No");
			if(m_pUtil->m_MultiTable||m_ResidualSource) {
					SetColWidth(9,70); SetColWidth(10,100);
					QuickSetText(9,-1,"Multi No");
					QuickSetText(10,-1,"Realisation");
			}
			if(m_PGRepit) {
				SetColWidth(9,70); SetColWidth(10,0);
				QuickSetText(9,-1,"Repit No");

			}
			size_t isubchart, MultiRunNo, RepitNumber;
				

			int mLoop=1;
			size_t lrow=-1;
			if(m_pProp->m_MultiValid.AcceptRejectMode) 	mLoop=2;

			size_t NumVarPerChartRatio = 1;
			NumVarPerChartRatio = m_TimeSeriePlot.m_ChartInfo.NumVariables / m_TimeSeriePlot.m_ChartInfo.NumPriorVariables;


				//if(m_MultiTable) mn=2;
			//for(int im=0;im<mLoop;im++) {
			vector<int> curvecount(m_NumUsedSubCharts,0);

				//size_t n1=min(m_TimeSeriePlot.m_ChartInfo.NumVariables, m_pProp->m_AxisLayer.size());
				for(size_t i=0;i<m_TimeSeriePlot.m_ChartInfo.NumVariables;i++) {
						
						ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, i/NumVarPerChartRatio);
					//	if(ArrayIndex== std::string::npos&&m_TimeSeriePlot.m_ChartInfo.NumVariables==m_TimeSeriePlot.m_ChartInfo.NumPriorVariables) continue;
						lrow++;
					//	if(m_TimeSeriePlot.m_ChartInfo.NumVariables==2*m_TimeSeriePlot.m_ChartInfo.NumPriorVariables) 
					//		ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumIndex, int(i/2));

						if(ArrayIndex<0) continue;

						PLOTv p_info=m_pSimDoc->m_PlotStorage.GetPlotInfo(ArrayIndex);
						int koll=m_pSimDoc->m_PlotStorage.GetPGSectionNo(ArrayIndex);
						isubchart=m_pSimDoc->m_PlotStorage.GetSubChart(ArrayIndex);
						auto datatype=m_pSimDoc->m_PlotStorage.GetDataType(ArrayIndex);
						auto Ens_type = m_pSimDoc->m_PlotStorage.GetEnsembleType(ArrayIndex);

						string txt;
						if(curvecount.size()<=isubchart-1) 
							isubchart=curvecount.size();

						curvecount[isubchart-1]++;
						size_t curve_no;
						//if(m_TimeSeriePlot.m_ChartInfo.NumVariables==2*m_TimeSeriePlot.m_ChartInfo.NumPriorVariables)
						//	curve_no=(curvecount[isubchart-1]);
						//else
						    curve_no=curvecount[isubchart-1];

						if (Ens_type != ENSEMBLE_TYPE::NO || p_info.RepitDataType || datatype == DATA_TYPES::MEASURED_VAL) {
							RepitNumber = m_pSimDoc->m_PlotStorage.GetPGRepitNo(ArrayIndex);
							GetCell(9, lrow, &cell);
							cell.SetCellType(UGCT_DROPLIST);
							CString optionlist;
							optionlist = "";
							for (size_t ii = 0; ii < 5; ii++) {
									optionlist.Append(MFC_Util::ItoAscii(ii + 1));
									optionlist.AppendChar('\n');
								
							}
							cell.SetLabelText(optionlist);
							if (p_info.RepitDataType)
								cell.SetNumber(RepitNumber);
							SetCell(9, lrow, &cell);
						}
						else if (Ens_type != ENSEMBLE_TYPE::NO || p_info.RepitDataType){
							MultiRunNo=m_pSimDoc->m_PlotStorage.GetValRunNo(ArrayIndex);
							GetCell(9,lrow,&cell);
							cell.SetCellType(UGCT_DROPLIST);
							CString optionlist;
							optionlist="All\nAccepted\n";
							for(size_t ii=0;ii<m_pSimDoc->MR_Get_TotalNumberofRuns();ii++) {
								float validv=0;
								validv=m_pSimDoc->m_MStorage.GetMultiP(ii,m_pSimDoc->MR_Get_NumTotalPar());
								if(validv>0.5) {
									optionlist.Append(MFC_Util::ItoAscii(ii+1));
									optionlist.AppendChar('\n');
								}
							}
							cell.SetLabelText(optionlist);
							if(p_info.RepitDataType)
								cell.SetNumber(MultiRunNo);
							else if(Ens_type==ENSEMBLE_TYPE::ACCEPTED)
								cell.SetText("Accepted");
							else 
								cell.SetText("All");
							SetCell(9,lrow,&cell);
						}
						else if(datatype==DATA_TYPES::SIM_VAL&&m_pSimDoc->IsMultiSimulation()) {
							int ResRunNo=m_pSimDoc->m_PlotStorage.GetValRunNo(ArrayIndex);
							GetCell(9,lrow,&cell);
							cell.SetCellType(UGCT_DROPLIST);
							CString optionlist;
							optionlist="All\nAccepted\n";
							for(size_t ii=0;ii<m_pSimDoc->MR_Get_TotalNumberofRuns();ii++) {
								float validv=0;
								validv=m_pSimDoc->m_MStorage.GetMultiP(ii,m_pSimDoc->MR_Get_NumTotalPar());
								if(validv>0.5) {
									optionlist.Append(MFC_Util::ItoAscii(ii+1));
									optionlist.AppendChar('\n');
								}
							}
							cell.SetLabelText(optionlist);
							cell.SetNumber(ResRunNo+1);
							SetCell(9,lrow,&cell);


						}
						else if(datatype==DATA_TYPES::PGDATA) {
							CPG *pPG;
							pPG=(CPG*)m_pSimDoc->m_PlotStorage.GetPtr(ArrayIndex);
							pPG=p_info.pPG;

							long repitSeq;
							repitSeq=p_info.PGRepitNo;
							repitSeq=m_pSimDoc->m_PlotStorage.GetPGRepitNo(ArrayIndex);
							long nrep=pPG->GetNumRepititions();
							GetCell(9,lrow,&cell);
							cell.SetCellType(UGCT_DROPLIST);
							CString optionlist;
							optionlist=_T("");
							for(int ii=0;ii<nrep;ii++) {
								optionlist.Append(MFC_Util::ItoAscii(ii+1));
								optionlist.AppendChar('\n');
							}
							cell.SetLabelText(optionlist);
							cell.SetNumber(repitSeq);
							SetCell(9,lrow,&cell);



						}
						else if(datatype==DATA_TYPES::SIM_VAL&&m_pSimDoc->IsMultiSimulation()) {
							GetCell(9,lrow,&cell);
							cell.SetCellType(UGCT_DROPLIST);
							string optionlist;
							
							optionlist="All\nAccepted\nSingle Run\n";
							if(p_info.EnsembleType==ENSEMBLE_TYPE::ALL) txt="All";
							else if(p_info.EnsembleType == ENSEMBLE_TYPE::ACCEPTED) txt="Accepted";
							cell.SetText(txt.c_str());
							cell.SetLabelText(optionlist);
							SetCell(9,lrow,&cell);



						}
						else {
							QuickSetNumber(-1,lrow,i+1);
							GetCell(0,lrow,&cell);
							QuickSetText(9,lrow,"");

						}
						if(datatype == DATA_TYPES::SIM_VAL) {
							GetCell(10,lrow,&cell);
							cell.SetCellType(UGCT_DROPLIST);
							if(datatype == DATA_TYPES::SIM_VAL)
								optionlist="Value\nResidual\nGaussian LogLi\n";
							else
								optionlist="Value\nResidual\n";
							int iopt=m_pSimDoc->m_PlotStorage.GetLogLiVersion(ArrayIndex);
							if(datatype == DATA_TYPES::SIM_VAL) {
								if(iopt==3)
									txt="Value";
								else if(iopt==2)
									txt="Residual";
								else if(iopt==0)
									txt="Gaussian LogLi";
								else if(iopt==1)
									txt="Optional Logli";
							}
							else {
								if(iopt==3)
									txt="Value";
								else 
									txt="Residual";
							}
							cell.SetText(txt.c_str());
							cell.SetLabelText(optionlist);
							SetCell(10,lrow,&cell);
						}
						else {

							QuickSetText(10,lrow,"");
						}
						
						if(m_pProp->m_ReShape.m_CommonDayNo||m_pProp->m_ReShape.m_CommonWithDay) {
							cell.SetCellType(UGCT_CHECKBOX);
							if(m_pProp->m_Legend1.size()>size_t(i))
								cell.SetLabelText(m_pProp->m_Legend1.at(i));
							else
								cell.SetLabelText("");
							if(koll>10000)
								cell.SetBool(false);
							else
								cell.SetBool(true);
						}
						else {
							cell.SetCellType(UGCT_NORMAL);

							/*if(m_TimeSeriePlot.m_ChartInfo.NumVariables==2*m_TimeSeriePlot.m_ChartInfo.NumPriorVariables) {
								if(curve_no%2==1)
									cell.SetText("All");
								else 
									cell.SetText(m_pProp->m_Legend1.at(lrow).c_str());
							
							}
							else */
							if(m_pProp->m_Legend1.size()>lrow)
								cell.SetText(m_pProp->m_Legend1.at(lrow).c_str());
							//else
							//	cell.SetText("All");
						}


						if (m_pProp->isYLegendChanged(lrow)) 
							cell.SetBackColor(m_ColorChanged);
						else 
							cell.SetBackColor(m_ColorClick);
						SetCell(0,lrow,&cell);
					//	QuickSetText(0,i,m_Legend1.at(i));


						QuickSetBackColor(1,i,MFC_Util::GetColorLine(curve_no));
						GetCell(2,lrow,&cell);
						cell.SetCellType(UGCT_DROPLIST);
						optionlist="Common\nThin\nNormal\nLarge\nVery large";


						//m_pProp->m_LineWidth.at(curvecount[isubchart-1]-1)
						switch(m_pUtil->GetSpecificLineWidth(curve_no-1))
						{
						case 0:
							txt="Common";
							break;
						case 1:
							txt="Thin";
							break;
						case 2:
							txt="Normal";
							break;
						case 3:
							txt="Large";
							break;
						case 4:
							txt="Very large";
							break;
						}
						cell.SetText(txt.c_str());
						cell.SetLabelText(optionlist);
						SetCell(2,lrow,&cell);

						GetCell(3,lrow,&cell);
						cell.SetCellType(UGCT_DROPLIST);
						optionlist="Solid\nDashed Line\nDotted Line\nDot Dashed Line\n";
						optionlist+="Alternativ Dashed Line";

						switch(m_pUtil->GetLineTypeIndex(curve_no-1))
						{
						case 0:
							txt="Solid";
							break;
						case 1:
							txt="Dashed Line";
							break;
						case 2:
							txt="Dotted Line";
							break;
						case 3:
							txt="Dot Dashed Line";
							break;
						case 4:
							txt="Alternative Dashed Line";
							break;
						}
						cell.SetText(txt.c_str());
						cell.SetLabelText(optionlist);
						SetCell(3,lrow,&cell);
						GetCell(4,lrow,&cell);
						cell.SetCellType(UGCT_DROPLIST);
						optionlist="no symbol\nFilled circle\nOpen circle\nFilled square\n";
						optionlist+="Open square\nFilled diamond\nOpen diamond\nFilled triangle\nOpen triangle";

					
						switch(m_pUtil->GetSpecificSymbol(curve_no-1))
						{
						case 0:
							txt="no symbol";
							break;
						case 1:
							txt="Filled circle";
							break;
						case 2:
							txt="Open circle";
							break;
						case 3:
							txt="Filled square";
							break;
						case 4:
							txt="Open square";
							break;
						case 5:
							txt="Filled diamond";
							break;
						case 6:
							txt="Open diamond";
							break;
						case 7:
							txt="Filled triangle";
							break;
						case 8:
							txt="Open triangle";
							break;
						}
					
						cell.SetText(txt.c_str());
						cell.SetLabelText(optionlist);
						SetCell(4,lrow,&cell);					
					
						GetCell(5,lrow,&cell);
						cell.SetCellType(UGCT_DROPLIST);
						optionlist="Common\nVery small\nSmall\nNormal\nLarge\n";
						optionlist+="Very Large\n";

						if(m_pUtil->GetSpecificSymbol(curve_no-1)!=0) {



							switch(m_pUtil->GetSpecificSymbolSize(curve_no-1))
							{
							case 0:
								txt="Common";
								break;
							case 1:
								txt="Very small";
								break;
							case 2:
								txt="Small";
								break;
							case 3:
								txt="Normal";
								break;
							case 4:
								txt="Large";
								break;
							case 5:
								txt="Very large";
								break;
							case 6:
								txt="Huge";
								break;
							}
	
						}
						else 
							QuickSetText(5,lrow,"");
						cell.SetText(txt.c_str());
						cell.SetLabelText(optionlist);
						SetCell(5,lrow,&cell);	
						if(m_pSimDoc!=nullptr) {
							int subchart=m_pSimDoc->m_PlotStorage.GetSubChart(ArrayIndex);
							if(subchart>0)
								QuickSetNumber(6,lrow,subchart);
						}
					

						GetCell(7,lrow,&cell);
						cell.SetCellType(UGCT_DROPLIST);
						optionlist="Y Left\nY Right\nX axis\n";

						if(m_pProp->m_AxisLayer.size()<curve_no) {
							txt="X axis";
						}
						else if(m_pProp->m_AxisLayer.at(curve_no-1)<=1)
							txt="Y Left";
						else if(m_pProp->m_AxisLayer.at(curve_no-1)==2)
							txt="Y Right";
						else 
							txt="X axis";

						cell.SetText(txt.c_str());
						cell.SetLabelText(optionlist);
						SetCell(7,lrow,&cell);	
						QuickSetNumber(8,lrow,curve_no);

				}
			
			/*	if(m_MultiTable) {
					n1=m_Legend1.size();
					QuickSetText(0,n1,"MultiRunMode");
					QuickSetBackColor(0,n1,m_ColorHeading);
					str=MFC_Util::ItoAscii(m_MultiSequence)+"("+MFC_Util::ItoAscii(m_MultiRepititions)+")";
					QuickSetText(0,n1+1,str);
				}*/
}
void ChartCugSheets::Make_Sheet4_RealTime()
{
	CUGCell cell; 
	string str;
	string txt;
	string optionlist;
				QuickSetText(0,-1,"Measured");
				QuickSetText(1,-1,"Simulated");
				QuickSetText(2,-1,"Pick");
				SetColWidth(0,220);
				SetColWidth(1,220);
				SetColWidth(2,40);
				CString test;
				int colorcount=0;
				for(int i=0; i<10;i++) {
					QuickSetNumber(-1,i,i+1);
					QuickSetText(0,i,"Simulation");
					test=m_pSimDoc->MR_GetExtraLabel(i).c_str();
					QuickSetText(1,i,test);

					GetCell(2,i,&cell);
					cell.SetCellType(UGCT_CHECKBOX);
					cell.SetBool(false);
					SetCell(2,i,&cell);

				}
				for(size_t i=0; i<m_pSimDoc->MR_GetNumExtraMonitor();i++) {
					int index;
					index=m_pSimDoc->MR_GetExtraMonitor(i);
					GetCell(2,index,&cell);
					cell.SetParam(i+1000);
					cell.SetBool(true);
					SetCell(2,index,&cell);
				}
}
void ChartCugSheets::Make_Sheet4_TimeSeriePlot(int rows)
{
	CUGCell cell; 
	string str;
	string txt;
	string optionlist;
			SetColWidth(-1,20);
			SetColWidth(0,240);
			for(int i=1;i<6;i++)
				SetColWidth(i,114);

			string legend = m_pProp->GetChartTypeString();
			legend += "Legends";
			GetCell(0, -1, &cell);
			cell.SetText(legend);
			cell.SetBackColor(m_ColorClick);
			SetCell(0, -1, &cell);

			QuickSetText(1,-1,"Band color");
			QuickSetText(2,-1,"Log Dist");
			QuickSetText(3,-1,"Min");
			QuickSetText(4,-1,"Max");
			QuickSetText(5,-1,"Steps");
			JoinCells(1,0,1,rows-1);
			QuickSetBackColor(1,0, m_pUtil->GetColorDist());
			size_t NumVarPerChartRatio = 1;
			NumVarPerChartRatio = m_TimeSeriePlot.m_ChartInfo.NumVariables / m_TimeSeriePlot.m_ChartInfo.NumPriorVariables;



			for(size_t i=0;i<size_t(rows);i++) {
					QuickSetNumber(-1,i,i+1);
					if (i >= m_pProp->m_Legend1.size()) {
						m_pProp->SetLegend1(i, "");

					}
					QuickSetText(0,i,m_pProp->m_Legend1.at(i));
					CString txt;
					GetCell(2,i,&cell);
					cell.SetCellType(UGCT_DROPLIST);
					optionlist="Linear\nLogaritmic\n";
					if(m_pProp->isXDistLog(i))
						txt="Logaritmic";
					else
						txt="Linear";

					cell.SetText(txt);
					cell.SetLabelText(optionlist);
					SetCell(2,i,&cell);

					size_t dist_index=m_TimeSeriePlot.GetAdjustedVariableIndex(i);
						QuickSetNumber(3, i, m_pProp->GetXDistMin(dist_index));
						QuickSetNumber(4, i, m_pProp->GetXDistMax(dist_index));
						QuickSetNumber(5, i, m_pProp->GetXDistSteps(dist_index));

			}
		}
void ChartCugSheets::Make_Sheet5_TimeSeriePlot(int rows) {
		int  ArrayIndex;
			SetColWidth(-1,20);
			SetColWidth(0,240);
			for(int i=1;i<6;i++)
				SetColWidth(i,114);
			string legend = m_pProp->GetChartTypeString();
			legend += "Legends";
			CUGCell cell;
			GetCell(0, -1, &cell);
			cell.SetText(legend);
			cell.SetBackColor(m_ColorClick);
			SetCell(0, -1, &cell);



			QuickSetText(2,-1,"Intercept");
			QuickSetText(3,-1,"Slope");
			QuickSetText(4,-1,"R");

			if(size_t(rows)>m_pProp->m_Legend1.size()) rows=m_pProp->m_Legend1.size();
			size_t NumVarPerChartRatio = 1;
			NumVarPerChartRatio = m_TimeSeriePlot.m_ChartInfo.NumVariables / m_TimeSeriePlot.m_ChartInfo.NumPriorVariables;
			vector<int> curvecount(m_NumUsedSubCharts, 0);
			for (size_t i = 0; i<m_TimeSeriePlot.m_ChartInfo.NumVariables; i++) {

				ArrayIndex = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, i / NumVarPerChartRatio);
				if (ArrayIndex<0) continue;

				PLOTv p_info = m_pSimDoc->m_PlotStorage.GetPlotInfo(ArrayIndex);
				size_t isubchart = m_pSimDoc->m_PlotStorage.GetSubChart(ArrayIndex);


				if (curvecount.size() <= isubchart - 1)
					isubchart = curvecount.size();
				curvecount[isubchart - 1]++;
				size_t curve_no;
				curve_no = curvecount[isubchart - 1];



					int koll=m_pSimDoc->m_PlotStorage.GetPGSectionNo(ArrayIndex);
					if(koll>0) {
						if(koll<10000) {
							QuickSetBackColor(0,i,MFC_Util::GetColorLine(i+1)); 
						}
					}
					QuickSetNumber(-1,i,i+1);
					if (m_pProp->m_Legend1.size() > i) {
						QuickSetText(0, i, m_pProp->m_Legend1.at(i));
						QuickSetBackColor(1, i, MFC_Util::GetColorLine(curve_no));
						size_t dist_index = m_TimeSeriePlot.GetAdjustedVariableIndex(i);
						if (m_pProp->IsRegLineEstimated(dist_index)) {
							//if ((m_pProp->GetRegA0(i) > -0.9E38&&m_pProp->m_ChartType == 0) || (m_pProp->m_ChartType ==CHART_TYPES::TIME_SERIE && m_pProp->GetRegA0(i) > -0.9E38)) {
							QuickSetNumber(2, i, m_pProp->GetRegA0(dist_index));
							QuickSetNumber(3, i, m_pProp->GetRegA1(dist_index));
							QuickSetNumber(4, i, m_pProp->GetRegR(dist_index));
							//}
						}
					}

			}
}


void ChartCugSheets::AddMainData(DoubleArray data, int index) 
{
	double v;
	int i=data.len;
	if(m_pProp->m_Legend1.size()<=0) return;
	if(m_MakeDataSheet) {
		SetSheetNumber(6,false);
		for (int j=0;j<data.len;j++) {
			v=*(data.data+j);
			if(v==Chart::NoValue)
				QuickSetText(index+1,j,"");
			else {
				QuickSetNumber(index+1,j,v);
				QuickSetAlignment(index+1,j,UG_ALIGNRIGHT);
			}
		}
		RedrawAll();

		if(m_CreatePG) {
			m_PGExtra.AddVariableVector(data, index+1);
			m_ToSavePGStruc=true;
		}
	}
}
void ChartCugSheets::AddTimeData(DoubleArray data, DoubleArray data_y, size_t x_var) 
{
	double v;
	DoubleArray data_x;
	CUGCell cell;
	int i=data.len;
	m_MakeDataSheet=true;
	

	if(m_MakeDataSheet) {

		//if(m_pProp->m_Legend1.size()<=0) return;

		if (!(m_pProp->m_ChartType >=CHART_TYPES::FREQUENCE && m_pProp->m_ChartType >=CHART_TYPES::CUM_FREQUENCE)) x_var = std::string::npos;

		if (x_var == 0 || x_var == std::string::npos) {
			MakeDataSheet(0);
			SetSheetNumber(6, false);
			SetNumberRows(data.len);
		}

		for (int j=0;j<data.len;j++) {
			GetCell(0,j,&cell);
			if(m_pUtil->m_MultiRunPlot||m_pProp->m_ReShape.m_NormalDist||m_pProp->m_ReShape.m_SortedSeries||m_pProp->m_ChartType!= CHART_TYPES::TIME_SERIE) {
				if(x_var== std::string::npos)
					v=*(data.data+j);
				else {
					double minv = m_pProp->GetXDistMin(m_NumVarSelected - 1);
					double maxv = m_pProp->GetXDistMax(m_NumVarSelected - 1);
					v = *(data.data + j)-minv;
					float vnew=v/(maxv-minv);
					vnew = m_pProp->GetXDistMin(x_var) + vnew*(m_pProp->GetXDistMax(x_var) - m_pProp->GetXDistMin(x_var));
					v = vnew;
				}
				if (v == Chart::NoValue)
					cell.SetText("");
				else
					cell.SetNumber(v);
			}
			else {
				v=*(data.data+j);
				v=v/60+2880;
				auto min=size_t(v);
				string txt=PGUtil::StringDatum(min);
				cell.SetText(txt.c_str());
			}
		
			if (x_var != std::string::npos) {
				int col = x_var*(m_TimeSeriePlot.GetNumberOfEnsembles() + 2);
				SetCell(col, j, &cell);

			}
			else
				SetCell(0,j,&cell);
			if (m_pProp->IsSecondIndepSet()) {
				if (*(data_y.data + j) == Chart::NoValue)
					QuickSetText(1, j, "");
				else
					QuickSetNumber(1, j, *(data_y.data + j));

			}



			//QuickSetNumber(0,j,v);
			QuickSetNumber(-1,j,j+1);
		}
		RedrawAll();

		if(m_CreatePG) {
			m_PGExtra.SetCompleteRead(true);
			m_PGExtra.AdjustSize(data.len, m_PGExtra.GetNumVariables()+ m_extra_xy_variables);
			if (m_pUtil->m_MultiRunPlot || m_pProp->m_ReShape.m_NormalDist || m_pProp->m_ReShape.m_SortedSeries || m_pProp->m_ChartType !=CHART_TYPES::TIME_SERIE||
				m_pProp->GetIndepFirst()!=DATEAXIS) {
				m_PGExtra.AddDummySequenceTimeVector();
				m_PGExtra.AddVariableVector(data, 1);
				if(m_pProp->IsSecondIndepSet())
					m_PGExtra.AddVariableVector(data_y, 2);
			}
			else 
				m_PGExtra.AddTimeVector(data);
		}
	}

}
void ChartCugSheets::AddBandData(DoubleArray datasd,DoubleArray datamin,DoubleArray datamax,DoubleArray datap10, DoubleArray datap50, DoubleArray datap90, int index) 
{
	double v;


	int n1=m_pProp->m_Legend1.size();
	n1=m_NumVarSelected;

	int i=datasd.len;

	if(m_MakeDataSheet) {
		SetSheetNumber(6,false);
		for (int j=0;j<datasd.len;j++) {
			v=*(datasd.data+j);
			if(Chart::NoValue!=v) {
				QuickSetNumber(1+n1+index*6,j,v);
				QuickSetAlignment(1+n1+index*6,j,UG_ALIGNRIGHT);
				v=*(datamin.data+j);
				QuickSetNumber(2+n1+index*6,j,v);
				QuickSetAlignment(2+n1+index*6,j,UG_ALIGNRIGHT);
				v=*(datap10.data+j);
				QuickSetNumber(3+n1+index*6,j,v);
				QuickSetAlignment(3+n1+index*6,j,UG_ALIGNRIGHT);
				v=*(datap50.data+j);
				if(Chart::NoValue!=v) {
					QuickSetNumber(4+n1+index*6,j,v);
					QuickSetAlignment(4+n1+index*6,j,UG_ALIGNRIGHT);
				}
				v=*(datap90.data+j);
				QuickSetNumber(5+n1+index*6,j,v);
				QuickSetAlignment(5+n1+index*6,j,UG_ALIGNRIGHT);
				v=*(datamax.data+j);
				QuickSetNumber(6+n1+index*6,j,v);
				QuickSetAlignment(6+n1+index*6,j,UG_ALIGNRIGHT);
			}
		}
		RedrawAll();
		if(m_CreatePG) {
			m_PGExtra.AddVariableVector(datasd, 1+n1+index*6);
			m_PGExtra.AddVariableVector(datamin, 2+n1+index*6);	
			m_PGExtra.AddVariableVector(datap10, 3+n1+index*6);
			m_PGExtra.AddVariableVector(datap50, 4+n1+index*6);
			m_PGExtra.AddVariableVector(datap90, 5+n1+index*6);
			m_PGExtra.AddVariableVector(datamax, 6+n1+index*6);
			m_ToSavePGStruc=true;
		}
	}

}
