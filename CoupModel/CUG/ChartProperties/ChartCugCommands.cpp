#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include "..\..\Util.h"
#include "..\..\NewBase\FileScanner.h"
#include "ChartCugCommands.h"
#include "..\..\MainFrm.h"
#include "..\..\CoupModelView.h"
#include "..\ColourPickerDlg.h"
#include "..\LegendChangeDlg.h"
#include "..\..\Chart\TimeSeriePlot.h"

const int DEFAULT_INTERVAL  = 500;
const short ID_TIMER = 1;



ChartCugCommands::ChartCugCommands(void)
{
	//m_pChartCug=MySingle::instance().GetChartCugPointer();
}
ChartCugCommands::~ChartCugCommands(void)
{
}
bool ChartCugCommands::RealTimeChangeMonOptions(int col, int row)
{
	CUGCell cell;
	bool updatedirect=true;

	GetCell(col,row,&cell);
	string After=CT2A(cell.GetText()).m_psz;
	auto optionlist=cell.GetLabelText();

	if(row==1&&m_pSimDoc->IsMultiSimulation()) {
		if(After=="Coefficient of determination")
			m_pSimDoc->SetMonitor1View(0);
		else if (After=="Intercept")
			m_pSimDoc->SetMonitor1View(1);
		else if(After=="Slope")
			m_pSimDoc->SetMonitor1View(2);
		else if(After=="Mean error")
			m_pSimDoc->SetMonitor1View(3);

	}
	else if(row==1&&!m_pSimDoc->IsMultiSimulation()) {
		if(After=="Fast")
			m_pSimDoc->m_Simulator.m_pModelInfo->SetRunDelay(0);
		else if (After=="Small Delay")
			m_pSimDoc->m_Simulator.m_pModelInfo->SetRunDelay(1);
		else if (After=="Medium Delay")
			m_pSimDoc->m_Simulator.m_pModelInfo->SetRunDelay(20);
		else if (After=="Large Delay")
			m_pSimDoc->m_Simulator.m_pModelInfo->SetRunDelay(1000);
		else if (After=="Huge Delay")
			m_pSimDoc->m_Simulator.m_pModelInfo->SetRunDelay(100000);

	}
	else if(row==3) {
		if(After=="RootMeanSquareError")
			m_pSimDoc->SetMonitor2View(0);
		else if (After=="Log Likelihood")
			m_pSimDoc->SetMonitor2View(1);
	}

	return updatedirect;

}
bool ChartCugCommands::ChangeActionOptions(int col, int row)
{
	CUGCell cell;
	bool updatedirect=true;

	GetCell(col,row,&cell);
	string After=cell.GetStdText();
	auto optionlist=cell.GetStdLabelText();
				if(col==0&&row==3) {
					m_pProp->ResetXScale();
					m_pProp->ResetYLeftScale();
					m_pProp->ResetYRightScale();
					m_pProp->ResetZScale();
					if(After=="TimeSerie") {
						m_pProp->m_ChartType= CHART_TYPES::TIME_SERIE;
						m_pProp->SetFirstIndepSet(PGDEPTH,false);
						m_pProp->SetSecondIndepSet(DATEAXIS, false);
					}
					else if(After=="Scatter Chart")
						m_pProp->m_ChartType= CHART_TYPES::SCATTER;
					else if(After=="Frequency Distribution") {
						m_pProp->m_ChartType= CHART_TYPES::FREQUENCE;
						OnUpdate();
					}
					else if(After=="Cumulative Frequency Distribution") {
						m_pProp->m_ChartType= CHART_TYPES::CUM_FREQUENCE;
						//m_pProp->m_ReShape.m_AddLine=true;
					}
					else if(After=="Contour Chart") {
						m_pProp->m_ChartType= CHART_TYPES::CONTOUR_CHART;
						if(m_pUtil->m_MultiRunPlot) {
							if(m_pProp->GetFirstIndepIndex()==m_pProp->GetSecondIndepIndex()&&m_pProp->GetSecondIndepIndex()==string::npos) 
								m_pProp->SetSecondIndepIndex(PARVAR,1);
							OnUpdate();


						}
						else {
							m_pProp->SetSecondIndepSet(PGDEPTH,true);
							m_pProp->SetFirstIndepSet(DATEAXIS, true);


						}
						//m_pProp->m_ReShape.m_AddLine=true;
					}


					m_pMainframe =AfxGetMainWnd();	
				//	((CMainFrame*)m_pMainframe)->m_wndModel.m_grid1.m_CurrentChartType=m_pProp->m_ChartType;

					m_pUtil->m_FileSave.OnReDraw(true);
				}
				else if (col == 0 && row == 5) {

					if (After == "Normal Time") {
						m_pProp->m_ChartType = CHART_TYPES::TIME_SERIE;
						m_pProp->SetFirstIndepSet(DATEAXIS, false);
					}
					else if (After == "Hour") {
						m_pProp->m_ChartType = CHART_TYPES::SCATTER;
						m_pProp->SetFirstIndepSet(PGHOUR, true);

					}
					else if (After == "Day") {
						m_pProp->m_ChartType = CHART_TYPES::SCATTER;
						m_pProp->SetFirstIndepSet(PGDAY, true);

					}
					else if (After == "Year") {
						m_pProp->m_ChartType = CHART_TYPES::SCATTER;
						m_pProp->SetFirstIndepSet(PGYEAR, true);
	
					}
					m_pProp->ResetScales();
					Make_Sheet0_TimeSeriePlot();
					m_pUtil->m_FileSave.OnReDraw(true);
					
					return true;

				}
				else if (col == 0 && row == 7) {
					if (After == "Not Used") {
						m_pProp->m_ChartType = CHART_TYPES::SCATTER;	
						m_pProp->SetSecondIndepSet(NO_USE, false);
					}
					else if (After == "Hour") {
						m_pProp->m_ChartType = CHART_TYPES::CONTOUR_CHART;
						m_pProp->SetSecondIndepSet(PGHOUR, true);
					}
					else if (After == "Day") {
						m_pProp->m_ChartType = CHART_TYPES::CONTOUR_CHART;
						m_pProp->SetSecondIndepSet(PGDAY, true);
					}
					else if (After == "Year") {
						m_pProp->m_ChartType = CHART_TYPES::CONTOUR_CHART;
						m_pProp->SetSecondIndepSet(PGYEAR, true);
					}
					m_pProp->ResetScales();
					Make_Sheet0_TimeSeriePlot();
					m_pUtil->m_FileSave.OnReDraw(true);
					return true;
				}
				else if(col==1) {

					if(row==1) {
						m_pZoomTimeSerie->SetWindowWidthValue(ChartUtil::GetZoom_ValDurOption(After));
						m_TimeSeriePlot.LoadTimeSerieData();
						m_pZoomTimeSerie->OnChangeWindowWidth();
						m_pView->m_ChartViewer.setMouseUsage(Chart::MouseUsageScroll);
						m_TimeSeriePlot.DrawTimeSeriePlot(&m_pView->m_ChartViewer);
					}

				}
				else if(col==2) {
					if(row==0) {
						if(After=="Actual Values") m_pProp->m_ReShape.m_Cumulated=0;
						else if(After=="All Cumulated") m_pProp->m_ReShape.m_Cumulated=1;
						else if(After=="Year Cumulated") m_pProp->m_ReShape.m_Cumulated=2;
						else if(After=="Day Cumulated") m_pProp->m_ReShape.m_Cumulated=3;
						m_TimeSeriePlot.LoadTimeSerieData();
						m_TimeSeriePlot.DrawTimeSeriePlot(&m_pView->m_ChartViewer);
					}
					if (row == 2) {
						if (After == "No Stacking") m_pProp->m_ReShape.m_StackingVariables = 0;
						else if (After == "Stacked forward") m_pProp->m_ReShape.m_StackingVariables = 1;
						else if (After == "Stacked backward") m_pProp->m_ReShape.m_StackingVariables = 2;
						m_TimeSeriePlot.LoadTimeSerieData();
						m_TimeSeriePlot.DrawTimeSeriePlot(&m_pView->m_ChartViewer);
					}
					else if (row == 7) {
						size_t ind = MFC_Util::AtoInt(After);
						if (After == "All") ind = 0;
						if (m_pProp->m_MultiPG_dep.FirstType == PGDAY&&m_pProp->m_MultiPG_dep.SecondType == PGYEAR) {
							m_pProp->m_HourValueRepresentation = ind;
							m_pProp->m_YearValueRepresentation = 0;
						}
						else if (m_pProp->m_MultiPG_dep.FirstType == PGDAY&&m_pProp->m_MultiPG_dep.SecondType == PGHOUR) {
							m_pProp->m_YearValueRepresentation = ind;
							m_pProp->m_HourValueRepresentation = 0;
						}
					}
				}
				else if(col==4) {  //Annimation
					if(row==2) {
						m_AnnimateStepValue=FUtil::GetOptionIndex(cell.GetStdLabelText(), After);
						OnChangeAnnimationSteps();

					}
					else if(row==3) {
						m_AnnimateValue=FUtil::GetOptionIndex(cell.GetStdLabelText(), After);
						OnChangeAnnimationSpeed();


					}

				}
				else if(col==6) {
					if(row==4) {
						m_pProp->SetFirstIndepIndex(PARVAR,FUtil::GetOptionIndex(optionlist,After));

						m_TimeSeriePlot.LoadTimeSerieData();
						m_pZoomTimeSerie->OnChangeWindowWidth();
						m_pView->m_ChartViewer.setMouseUsage(Chart::MouseUsageScroll);
						m_TimeSeriePlot.DrawTimeSeriePlot(&m_pView->m_ChartViewer);


					}



				}
				else if (col == 8) {
					if (row == 3) {
						auto iopt = FUtil::GetOptionIndex(optionlist, After);
						m_pUtil->SetAutoX_ScaleValue(iopt);
						m_pUtil->m_FileSave.OnReDraw(true);
						return true;
					}
				}
				else if(col==9) {
					if(row==1) {
						m_pProp->SetSecondIndepIndex(PARVAR,FUtil::GetOptionIndex(optionlist,After));

						m_TimeSeriePlot.LoadTimeSerieData();
						m_pZoomTimeSerie->OnChangeWindowWidth();
						m_pView->m_ChartViewer.setMouseUsage(Chart::MouseUsageScroll);
						m_TimeSeriePlot.DrawTimeSeriePlot(&m_pView->m_ChartViewer);
					}
				}
		return updatedirect;
}
bool ChartCugCommands::ChangeGeneralOptions(int col, int row)
{
	CUGCell cell;
	bool updatedirect=true;
	bool reload=false;
	int ivis;
	CWinApp* pApp = AfxGetApp();

	GetCell(col,row,&cell);
	string After=cell.GetStdText();
	auto optionlist=cell.GetStdLabelText();

				updatedirect=true;
				GetCell(col,row,&cell);
				After=cell.GetStdText();

				if(row==4) {

					switch (col) {
						case 2: // Vertical
							if(After=="Horizontal"){
								m_pProp->SetVerticalLegends(false);
								MFC_Util::WriteProfileInt("VerticalLegends" ,0 );
							}
							else {
								m_pProp->SetVerticalLegends(true);
								MFC_Util::WriteProfileInt("VerticalLegends" ,1 );
							}
							break;
						case 3:
						case 4: // legend box

							ivis=ChartUtil::GetVisibleIndex(After);
							MFC_Util::WriteProfileInt("VisibleFrames" ,ivis );
							m_pProp->SetVisibleFrames(ivis);
	
							if(col==3) 	
								QuickSetText(4,row,After);
							else 
								QuickSetText(3,row,After);
								
							if(m_pProp->GetVisibleFrames()==4){
								QuickSetBackColor(3,4,m_pUtil->GetColorLegendBackground());
								QuickSetBackColor(4,4,m_pUtil->GetColorLegendBackgroundFrame());
							}
							else {
								QuickSetBackColor(3,4,m_NoColor);
								QuickSetBackColor(4,4,m_NoColor);
							}
							RedrawCell(3,4);
							RedrawCell(4,4);
							break;
						case 5: // Vertical
							if(After=="Extended"){
								m_pUtil->SetLegendContentStyle(0);
							}
							else if(After=="Context Sensitive") {
								m_pUtil->SetLegendContentStyle(1);
							}
							else if(After=="Minimal Style") {
								m_pUtil->SetLegendContentStyle(2);
							}
							break;
						case 6:
							if(After=="All SubCharts"){
								m_pUtil->m_RemoveLegends=0;
							}
							else if(After=="Only for first SubChart") {
								m_pUtil->m_RemoveLegends=1;
							}
							else if(After=="No Legends") {
								m_pUtil->m_RemoveLegends=2;
							}
							m_pUtil->SetRemoveLegends(m_pUtil->m_RemoveLegends);
							break;
						case 8:  // Rounded frame
							if(After=="Rounded") {
								m_pProp->SetRoundedFrame(true);
							}
							else {
								m_pProp->SetRoundedFrame(false);

							}
							MFC_Util::WriteProfileInt( "Rounded frame",m_pProp->m_RoundedFrame);
							break;

						case 9:   // Independet colors subcharts
							if(After=="Common") {
								m_pProp->SetCommonColors(true);
							}
							else {
								m_pProp->SetCommonColors(false);
							}
							MFC_Util::WriteProfileInt( "CommonColorsSubCharts",m_pProp->m_CommonColorForSubCharts);
							break;

					}


					m_pUtil->m_ToBeUpdated=true;
					QuickSetBackColor(-1,-1,m_ColorChanged);
					RedrawCell(-1,-1);

			}

				else if(row==6) {
					int iopt=0;
					switch(col) {
						case 3:
							if(After=="Whisker Box style")
								iopt=0;
							else if(After=="Bands around min and max")
								iopt=1;
							else if(After=="Bands around 1 StDev")
								iopt=2;
							else if(After=="Bands around 90 % interval")
								iopt=3;
							MFC_Util::WriteProfileInt("ChartError" ,iopt);
							m_pProp->SetChartError(iopt);

							reload=true;
							break;
						case 4:
							if(After=="Yes")
								iopt=1;
							else
								iopt=0;
							
							m_pUtil->SetErrorSignal(iopt);
							break;
						case 5:
							iopt=0;
							if(After=="Yes")
								iopt=1;
							else if(After=="No")
								iopt=0;
							else if(After=="LogLikelihood")
								iopt=2;
								
							m_pUtil->SetResidualRepresentation(iopt);
							if(iopt==1) 
								ChangeToResidualCurrentPlot(false);
							else
								ChangeToResidualCurrentPlot(true);
							reload=true;
							break;

						case 7:
							if(After=="Always")
								iopt=0;
							else if(After=="Sensitive")
								iopt=1;
							else
								iopt=2;
							MFC_Util::WriteProfileInt("RemoveXAxisScale" ,iopt);
							m_pUtil->SetRemoveXAxisScale(iopt);
							break;

						case 8:
							if(After=="Always")
								iopt=0;
							else if(After=="Sensitive")
								iopt=1;
							else
								iopt=2;
							MFC_Util::WriteProfileInt("RemoveYAxisScale" ,iopt);
							m_pUtil->SetRemoveYAxisScale(iopt);
							break;
						case 9:
							if(After=="Always")
								iopt=0;
							else if(After=="Sensitive")
								iopt=1;
							else
								iopt=2;
							MFC_Util::WriteProfileInt("RemoveTitle" ,iopt);
							m_pUtil->SetRemoveTitle(iopt);
							break;

					}


					m_pUtil->m_ToBeUpdated=true;
					QuickSetBackColor(-1,-1,m_ColorChanged);
					RedrawCell(-1,-1);

				}
				if(updatedirect&&!reload)
					m_pUtil->m_FileSave.OnReDraw(false);
				else
					m_pUtil->m_FileSave.OnReDraw(true);

	
	return false;
}
bool ChartCugCommands::ChangeChartOptions(int col, int row) 
{
	CUGCell cell;
	bool updatedirect=true;

	GetCell(col,row,&cell);
	string After=cell.GetStdText();
	auto optionlist=cell.GetLabelText();
	int icurve, iopt, ArrayIndex;

	GetCell(8,row,&cell);
	icurve=int(cell.GetNumber())-1;
	GetCell(col,row,&cell);
	After=cell.GetStdText();
	switch (col) {
			case 2:
			iopt=m_pUtil->GetLineThickSizeIndex(After);
			m_pUtil->SetSpecificLineWidth(icurve,iopt);
			return true;
			break;
			case 3:
			iopt=m_pUtil->GetLineTypeIndex(After);
			m_pUtil->SetLineTypeIndex(icurve,iopt);
			return true;
			break;
			case 4:
			iopt=m_pUtil->GetSymbolIndex(After);
			m_pUtil->SetSpecificSymbol(icurve, iopt);
			m_pUtil->SetSymbolCodes();
			return true;
			break;		
			case 5:
			iopt=m_pUtil->GetSymbolSizeIndex(After);
			m_pUtil->SetSpecificSymbolSize(icurve,iopt);
			return true;
			break;
			case 7:
			if(After=="Y Left") {
				m_pProp->SetAxisLayer(row, 1);

			}
			else if(After=="Y Right") {
				m_pProp->SetAxisLayer(row, 2);

			}
			else if(After=="X axis"&&m_pSimDoc!=nullptr){
				m_pProp->SetAxisLayer(row, 3);
				int ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, row);
				int subchart=m_pSimDoc->m_PlotStorage.GetSubChart(ArrayIndex)-1;
				int prev=m_pProp->GetXVariableNumber(subchart);
				m_pProp->SetAxisLayer(prev,1);
				m_pProp->SetXVariableNumber(subchart, row);
			}
			return true;
			break;
			case 9: {
				ArrayIndex = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, row);
				enum class ENSEMBLE_TYPE itype = m_pSimDoc->m_PlotStorage.GetEnsembleType(ArrayIndex);
				enum class DATA_TYPES datatype = m_pSimDoc->m_PlotStorage.GetDataType(ArrayIndex);
				if (m_ResidualSource) {
					iopt = FUtil::GetOptionIndex(cell.GetStdLabelText(), After);
					//cell.SetNumber(iopt+1);
					if (datatype == DATA_TYPES::MEASURED_VAL|| datatype == DATA_TYPES::SIM_VAL) {
						iopt = FUtil::AtoInt(After);
						m_pSimDoc->m_PlotStorage.SetPGRepitNo(ArrayIndex, iopt);
					}
					else if (datatype > DATA_TYPES::SIM_VAL) {
						if (After == "All")
							itype = ENSEMBLE_TYPE::ALL;
						else if (After == "Accepted")
							itype = itype = ENSEMBLE_TYPE::ACCEPTED;
						else if (After == "Single Run")
							itype = ENSEMBLE_TYPE::NO;
						m_pSimDoc->m_PlotStorage.SetEnsembleType(ArrayIndex, itype);
					}
					else {
						if (After == "All") {
							itype = ENSEMBLE_TYPE::ALL;
							m_pSimDoc->m_PlotStorage.SetEnsembleType(ArrayIndex, itype);
						}
						else if (After == "Accepted") {
							itype = ENSEMBLE_TYPE::ACCEPTED;
							m_pSimDoc->m_PlotStorage.SetEnsembleType(ArrayIndex, itype);
						}
						else {
							iopt = FUtil::AtoInt(After);
							m_pSimDoc->m_PlotStorage.SetValRunNo(ArrayIndex, iopt - 1);
						}
					}

				}
				else {
					if (After == "All") {
						itype = ENSEMBLE_TYPE::ALL;
						m_pSimDoc->m_PlotStorage.SetEnsembleType(ArrayIndex, itype);
					}
					else if (After == "Accepted") {
						itype = ENSEMBLE_TYPE::ACCEPTED;
						m_pSimDoc->m_PlotStorage.SetEnsembleType(ArrayIndex, itype);
					}
					else {
						iopt = FUtil::GetOptionIndex(cell.GetStdLabelText(), After);
						iopt = FUtil::AtoInt(After);
						cell.SetNumber(iopt + 1);
						m_pSimDoc->m_PlotStorage.SetValRunNo(ArrayIndex, iopt);
						itype = ENSEMBLE_TYPE::NO;
						m_pSimDoc->m_PlotStorage.SetEnsembleType(ArrayIndex, itype);

					}
				}
				m_pProp->Reset_Selected_Or_All_BasicVector();
				m_pUtil->m_FileSave.OnReDraw(true);
				return true;
				break;
			}
			case 10:
				if(m_ResidualSource) {
					//iopt=GetOptionIndex(cell.GetLabelText(), After);
					if(After=="Value")
						iopt=3;
					else if(After=="Residual")
						iopt=2;
					else if(After=="Gaussian LogLi")
						iopt=0;
					else 
						iopt=1;
					//cell.SetNumber(iopt+1);
					int ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, row);
					m_pSimDoc->m_PlotStorage.SetLogLiVersion(ArrayIndex, iopt);

				}
				m_pProp->Reset_Selected_Or_All_BasicVector();
				m_pUtil->m_FileSave.OnReDraw(true);
				return true;
			break;
	}
	return false;

}
bool ChartCugCommands::ChangeToResidualCurrentPlot(bool residual) {
	int ArrayIndex;
	for(size_t i=0;i<m_TimeSeriePlot.m_ChartInfo.NumVariables;i++) {					
				ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, i);
				if(ArrayIndex<0) continue;
				if(residual)
					m_pSimDoc->m_PlotStorage.SetLogLiVersion(ArrayIndex,3);
				else
					m_pSimDoc->m_PlotStorage.SetLogLiVersion(ArrayIndex,2);
	}
	if(m_TimeSeriePlot.m_ChartInfo.NumVariables>0)		
		return true;
	else
		return false;
}
bool ChartCugCommands::ChangeDistibutionOptions(int col, int row)
{
	CUGCell cell;
	bool updatedirect=true;

	GetCell(col,row,&cell);
	auto After=cell.GetStdText();

	switch (col) {
		case 2:
			if(After=="Linear") 
				m_pProp->SetXDistLog(row,false);
			else 
				m_pProp->SetXDistLog(row, true);
			
			m_pProp->SetXDistChanged(row, true);
			break;

	}

	m_pUtil->m_ToBeUpdated=true;
	QuickSetBackColor(-1,-1,m_ColorChanged);
	RedrawCell(-1,-1);
	return updatedirect;

}
bool ChartCugCommands::ChangeBoxCoxOptions(int col, int row)
{
		CUGCell cell;
	bool updatedirect=true;

	GetCell(col,row,&cell);
	auto After=cell.GetStdText();
	if(After=="on") {
		m_TimeSeriePlot.m_BoxCox.use=true;
	}
	else {
		m_TimeSeriePlot.m_BoxCox.use=false;
	}
	m_pUtil->m_FileSave.OnReDraw(true);
	return updatedirect;
}

bool ChartCugCommands::MakeGeneralMenu(int col, int row)
{
	if((row>2&&col>2&&col<8)||(col==2&&row>4)) {
		if(col==3)
			AddMenuItem(2001,"Change Legend background color of chart");
		else if(col==4)
			AddMenuItem(2002,"Change Legend background color of chart frame");
		else if(col==7)
			AddMenuItem(2000,"Change background color of chart");
		else if(col==2)
			AddMenuItem(2003,"Change frame background color of whisker box");

	}
	else  {

		if(!m_pProp->m_RemoveAxisLabel)
			AddMenuItem(2401,"Remove Labels on axis");
		else
			AddMenuItem(2401,"Show Labels on axis");
				
		if(m_pUtil->m_RemoveLegends==0)
			AddMenuItem(2402,"Remove Repeated Legends");
		else if(m_pUtil->m_RemoveLegends==1)
			AddMenuItem(2402,"Remove All Legends");
		else
			AddMenuItem(2402,"Show Legends");
	}
	return true;

}
bool ChartCugCommands::MakeAxisMenu(int col, int row)
{
	if (!m_pProp->m_RemoveAxisLabel)
		AddMenuItem(2401, "Remove Labels on axis");
	else
		AddMenuItem(2401, "Show Labels on axis");



	if (!m_pProp->m_ReShape.m_CommonScale) {
		if (m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART)
			AddMenuItem(2405, "Make Common Scale on z axis");
		else
			AddMenuItem(2405, "Make Common Scale on y axis");

	}
	else {

		if (m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART)
			AddMenuItem(2405, "Remove the common Scale on z axis");
		else
			AddMenuItem(2405, "Remove the common Scale on y axis");
	}

	size_t Index;

	int value;


	bool reload;
	bool updatedirect;
	
	updatedirect = true;

	CUGCell cell;
	GetCell(4, row, &cell);
	string strscale = cell.GetStdText();


	if ((strscale=="Y Left" && !m_pProp->IsSecondIndepSet())
		|| (row == 0 && m_pProp->IsSecondIndepSet() && m_pProp->GetSecondIndepVarType() != PGDEPTH)) {
		updatedirect = false;
		Index = size_t(row);
		int n;
		switch (col) {
		case 0:
			if (m_pProp->isYLabel1Changed(Index))
				AddMenuItem(4100+Index, "Reset Left Label to original value");
			

			break;
		case 1:case  2: case 3:
			if (m_pProp->isLeftYScaleChanged(Index))
				AddMenuItem(4500 + Index, "Reset Left Y scale to original values for actual variable");
			break;
		}

	}

	else if (strscale=="Y Right" && !m_pProp->IsSecondIndepSet()) {
		GetCell(-1, row, &cell);
		string strvar = cell.GetStdText();
		Index = FUtil::AtoInt(strvar) - 1;
		if (size_t(Index) < m_pProp->m_YLabel2.size()) {
			switch (col) {
			case 0:
				if (m_pProp->isYLabel2Changed(Index))
					AddMenuItem(4200 + Index, "Reset Left Label to original value");
				break;
			case 1:case  2: case 3:
				if (m_pProp->isRightYScaleChanged(Index))
					AddMenuItem(4600 + Index, "Reset Right Y scale to original values for actual variable");
				break;
			}
		}
	}
	else if ((strscale=="X Scale" && !m_pProp->IsSecondIndepSet())
		|| (row == 1 && m_pProp->IsSecondIndepSet())) {
		long min;
		GetCell(-1, row, &cell);
		string strvar = cell.GetStdText();
		Index = FUtil::AtoInt(strvar) - 1;

		switch (col) {
		case 0:
			if (m_pProp->isXLabelChanged(Index))
				AddMenuItem(4300 + Index, "Reset X Label to original value");
			break;
		case 1:case  2: case 3:
			if (m_pProp->isXScaleChanged(Index))
				AddMenuItem(4700 + Index, "Reset X scale to original values for actual variable");
			if (m_pProp->m_CommonXScale)
				AddMenuItem(2407, "Make X scale individual for each chart");
			else
				AddMenuItem(2407, "Make X scale common for all subcharts");
			break;

		}
	}
	else if ((strscale == "Z Scale" && !m_pProp->IsSecondIndepSet())
		|| (row == 2 && m_pProp->IsSecondIndepSet())) {
		long min;
		GetCell(-1, row, &cell);
		string strvar = cell.GetStdText();
		Index = FUtil::AtoInt(strvar) - 1;

		switch (col) {
		case 0:
			if (m_pProp->isZLabelChanged(Index))
				AddMenuItem(4850 + Index, "Reset Z Label to original value");
			break;
		case 1:case  2: case 3:
			if (m_pProp->isZScaleChanged(Index))
				AddMenuItem(4800 + Index, "Reset Z scale to original values for actual variable");
			if (m_pProp->m_CommonXScale)
				AddMenuItem(2407, "Make Z scale individual for each chart");
			else
				AddMenuItem(2407, "Make Z scale common for all subcharts");
			break;

		}
	}
	else if (m_pProp->IsSecondIndepSet()) {
		if (m_pProp->GetSecondIndepVarType() == PGDEPTH&&row == 0) {
				return false;
		}
		GetCell(-1, row, &cell);
		string strvar = cell.GetStdText();
		Index = FUtil::AtoInt(strvar) - 1;
		//switch (col) {
			if (m_pProp->isRightYScaleChanged(Index))
				AddMenuItem(4600 + Index, "Reset Right Y scale to original values for actual variable");
		//	break;
		//}
		m_pProp->m_ToBeUpdated = true;
		updatedirect = false;

	}

	

	return true;
}

bool ChartCugCommands::MakeVariableMenu(int col, int row)
{
			auto index=row;
			if(m_pUtil->m_RemoveLegends==0)
				AddMenuItem(2402,"Remove Repeated Legends");
			else if(m_pUtil->m_RemoveLegends==1)
				AddMenuItem(2402,"Remove All Legends");
			else
				AddMenuItem(2402,"Show Legends");

			

			int ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, index);
			if(ArrayIndex>=0) {
				void *Ptr=m_pSimDoc->m_PlotStorage.GetPtr(ArrayIndex);
				auto itype=m_pSimDoc->m_PlotStorage.GetDataType(ArrayIndex);
				SimB *pX=nullptr;
				CPG *pPG1=nullptr;
				if(itype==DATA_TYPES::PGDATA) 
					pPG1=(CPG*)Ptr;
				else if(itype== DATA_TYPES::ANYDATA)
					pPG1=(CPG*)((F*)Ptr)->GetPointer();
				else {
					pX=(SimB*)Ptr;	
					int idoc=m_pSimDoc->m_PlotStorage.GetDocIndex(ArrayIndex);
					pPG1=m_pSimDoc->m_PlotStorage.GetPGPointer(idoc);
				}
				
				itype=m_pSimDoc->m_PlotStorage.GetDataType(ArrayIndex);
				if(itype==DATA_TYPES::PGDATA&&m_PGRepit) {
					AddMenuItem(2411,"Add New Curve with Repitition of variable");
				}
				else if(itype==DATA_TYPES::SIM_VAL) {
					AddMenuItem(2411,"Add New Curve with New Realisation of simulation");
					AddMenuItem(2412,"Add New Curve with New Multi RunNo");
				}

			}
	
			auto str=FUtil::STD_ItoAscii(index+1);
			if (m_pProp->isYLegendChanged(index)) AddMenuItem(2403, "Reset Legend Name");
			AddMenuItem(2000,"Change Color var no: "+str);
			AddMenuItem(2005, "Change Legend var no: " + str);

			if(m_pProp->m_ReShape.m_CommonDayNo) {
				AddMenuItem(5000,"Substitute all curves with Mean value of years");
				AddMenuItem(5001,"Add Mean value of years");
				AddMenuItem(5002, "Toggle all selections of variables");
			}
			else if (m_pProp->m_ReShape.m_CommonWithDay) {
				AddMenuItem(5002, "Toggle all selections of variables");
				AddMenuItem(5003, "Enable all selections of variables");
				AddMenuItem(5004, "Disable all selections of variables");
			}
			else if (ArrayIndex >= 0) {
				AddMenuItem(2410, "Remove Curve from selection");

			}
			if (col >=2&&col<=5) {
				string uniq;
				if (col == 2) uniq = "Line Width";
				else if (col == 3) uniq = "Line Pattern";
				else if (col == 4) uniq = "Symbol";
				else if (col == 5) uniq = "Symbol Size";
  				AddMenuItem(2015+col, "Save Current "+uniq+ " Scheeme");
				AddMenuItem(2025+col, "Retrieve "+uniq+ " Scheeme");
				AddMenuItem(2035+col, "Reset all "+uniq + " Settings");

			}

			return true;

}
bool ChartCugCommands::MakeDataMenu()
{
	AddMenuItem(1000, "Cut"); 
	AddMenuItem(2000, "Copy"); 
	AddMenuItem(3000, "Paste"); 
	//if(m_ToSavePGStruc) {
		AddMenuItem(5030,"Save As PG File");
	//}
	return true;

}
bool ChartCugCommands::MakePGFileFromDataInCurrentChart() 
{
	CString pszFileName ;
	LPCTSTR lpszDefExt=_T("bin");
//	static char BASED_CODE szFilter[] = "PG Bin file (*.BIN)|*.BIN||";
	CString szFilter = _T("PG Bin file (*.BIN)|*.BIN||)");

	pszFileName=_T("Chart_")+MFC_Util::ItoAscii(m_pProp->m_ChartNumber)+_T(".bin");

	CFileDialog	BinFileDlg(FALSE,lpszDefExt,pszFileName,OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,szFilter);
	if(BinFileDlg.DoModal()) {
		ShowDataInGrid(true);
		pszFileName = BinFileDlg.GetPathName();
		m_PGExtra.ReCalculateMinMax();

		size_t count_ens = 0;
		size_t count_not_toplot = 0;
		if (m_pUtil->m_MultiRunPlot) {
			for (int i = 0; i < m_NumVarSelected; i++) {
				for (size_t iens = 0; iens < m_TimeSeriePlot.GetNumberOfEnsembles(); iens++) {
					if (m_pSimDoc->m_MStorage.IsToBePlotted(iens)&&i==0)	count_ens++;
					else count_not_toplot++;
				}
			}
		}




		for(size_t i=0; i<m_PGExtra.GetNumVariables();i++) {
			if (m_pProp->m_Legend1.size() > i) {
				m_PGExtra.SetVarId(i + 1 + m_extra_xy_variables, m_pProp->m_Legend1.at(i));
				m_PGExtra.SetVarName(i + 1 + m_extra_xy_variables, m_pProp->m_Legend1.at(i));
			}

			
		//	m_PGExtra.SetVarUnit(i+1,m_pProp->GetU_);
		//	m_PGExtra.SetVarId(i+1,);
		}

		if (m_extra_xy_variables > 0) {
			m_PGExtra.SetVarName(1, "X Variable");
			if(m_pProp->GetIndepFirst()!=DATEAXIS) m_PGExtra.SetVarId(1, m_pProp->GetFirstIndepString());
			if (m_extra_xy_variables > 1) 
				m_PGExtra.SetVarName(2, "Y Variable");
			if (m_pProp->IsSecondIndepSet()) m_PGExtra.SetVarId(2, m_pProp->GetSecondIndepString());
		}
		for(int i=0; i<m_NumVarSelected;i++) {

			
			int j=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, i);
			PLOTv p_info=m_pSimDoc->m_PlotStorage.GetPlotInfo(j);
			int PGindex=m_pSimDoc->m_PlotStorage.GetVar_Index(j);
			
			//auto pChartCug=MySingle::instance().GetChartCugPointer();
			m_TimeSeriePlot.FindIdForVariablesToPlot(j, p_info, PGindex);
			auto var=m_TimeSeriePlot.m_Label.test;
			for (size_t iens = 0; iens <= max(count_ens,1); iens++) {
				var = m_TimeSeriePlot.m_Label.test;
				m_PGExtra.SetVarName(i*max(count_ens+1,1)+iens +1+ m_extra_xy_variables, var.c_str());
				var = m_TimeSeriePlot.m_Label.unit;
				m_PGExtra.SetVarUnit(i*max(count_ens+1,1)+iens+1+ m_extra_xy_variables, var.c_str());
			}
		}

		for(int i=0;i<m_NumVarBandSelected;i++) {
			int j=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, i);
			PLOTv p_info=m_pSimDoc->m_PlotStorage.GetPlotInfo(j);
			int PGindex=m_pSimDoc->m_PlotStorage.GetVar_Index(j);
			m_TimeSeriePlot.FindIdForVariablesToPlot(j, p_info, PGindex);
			auto var=m_TimeSeriePlot.m_Label.test;
			for(size_t jj=0; jj<6;jj++) 
				m_PGExtra.SetVarName(i*6+m_NumVarSelected+1+jj,var.c_str());
			var=m_TimeSeriePlot.m_Label.unit;
			for(size_t jj=0; jj<6;jj++) 
				m_PGExtra.SetVarUnit(i*6+m_NumVarSelected+1+jj,var.c_str());
		}

		auto maxvar = m_extra_xy_variables + m_NumVarSelected+m_NumVarBandSelected*6;
		if(maxvar<m_PGExtra.GetNumVariables()) 	m_PGExtra.AdjustSize(m_PGExtra.GetNumRecords(), maxvar);
		m_PGExtra.ReCalculateMinMax();


		if(m_PGExtra.SaveAs(CT2A(pszFileName).m_psz, true)) {
			m_ToSavePGStruc=false;
			m_CreatePG=false;
		}
		m_PGExtra.CloseFile();
		string str = "PG file created:\n";
		str+=CT2A(pszFileName).m_psz;
		str += "\nDo you want to Open the new file in new Window ?";
		auto yes = MFC_Util::MessageBoxReturn(str, "File Created ", MB_YESNO);
		if (yes==6) {
			CPG *pPG = new CPG;
			pPG->Open(CT2A(pszFileName).m_psz);
			if (pPG->IsOpen()) {
				PGFile	Fil;
				Fil.pFile = pPG;
				Fil.Visible = false;
				m_pApp = AfxGetApp();
				m_pApp->OpenDocumentFile(pszFileName);
				pPG->CloseFile();
			}
			delete pPG;
		}

		return true;
	}
	return false;
}
bool ChartCugCommands::MakeAxisCommand(int item)
{
	if (item == 2401) {
		if (!m_pProp->m_RemoveAxisLabel) {
			m_pProp->m_RemoveAxisLabel = true;
		}
		else
			m_pProp->m_RemoveAxisLabel = false;
		return true;
	}
	else if (item == 2407) {
		if (m_pProp->m_CommonXScale) {
			m_pProp->m_CommonXScale = false;	
			m_pProp->ResetXScale(string::npos);
		}
		else {
			m_pProp->m_CommonXScale = true;
			m_pProp->ResetXScale(string::npos);
			m_pProp->SetAllXScale_toSameValues();
		}
		return true;
	}
	else if(item< 4100) 
		return true;
	else if(item<4200) {
		size_t Index = item - 4100;
		m_pProp->ResetYLabel1(Index);

	}


	else if(item<4300) {
		size_t Index = item - 4200;
		m_pProp->ResetYLabel2(Index);

	}

	else if(item<4400) {
		size_t Index = item - 4300;
		m_pProp->ResetXLabel(Index);

	}
	else if (item<4500) {
		size_t Index = item - 4400;
		m_pProp->ResetXLabel(Index);

	}
	else if (item < 4600) {
		size_t Index = item - 4500;
		m_pProp->ResetYLeftScale(Index);

	}
	else if (item < 4700) {
		size_t Index = item - 4600;
		m_pProp->ResetYRightScale(Index);

	}
	else if (item < 4800) {
		size_t Index = item - 4700;
		m_pProp->ResetXScale(Index);



	}
	else if (item < 4850) {
		size_t Index = item - 4800;
		m_pProp->ResetZScale(Index);


	}
	else if (item < 4900) {
		size_t Index = item - 4850;
		m_pProp->ResetZLabel(Index);


	}



	return true;
}
bool ChartCugCommands::MakeVariableCommand(int item, int col, int row)
{
	bool changemade = false;
	bool reload;
	int index;
	CUGCell cell;
	CWinApp* pApp = AfxGetApp();
	if (item == 5030) {

		return MakePGFileFromDataInCurrentChart();

	}
	else if (item == 2402) {
		if (m_pUtil->m_RemoveLegends == 0) {
			m_pUtil->m_RemoveLegends = 1;
		}
		else if (m_pUtil->m_RemoveLegends == 1)
			m_pUtil->m_RemoveLegends = 2;
		else
			m_pUtil->m_RemoveLegends = 0;
		changemade = true;
	}
	else if (item == 2403) {

		m_pProp->ResetLegend1(row);
		changemade = true;

	}
	else if (item == 1500) {
		index = row;
		if (m_Mode == 3) {
			if (m_pProp->m_PointType[index])
				m_pProp->m_PointType[index] = false;
			else
				m_pProp->m_PointType[index] = true;
			changemade = true;
		}
	}
	else if (item == 2000) {
		if (m_Mode == 1) {
			int colb = MFC_Util::GetProfileIntNo("ColorBackground", 0xf0f0ff);;
			CColourPickerDlg Dlgcol(colb);
			if (Dlgcol.DoModal() == IDOK) {
				COLORREF iv = Dlgcol.GetColour();
				m_pUtil->SetColorBackground(iv);
			}
			QuickSetBackColor(7, 4, m_pUtil->GetColorBackground());
			RedrawCell(7, 4);
			changemade = true;
		}
		else if (m_Mode == 3) {
			GetCell(8, row, &cell);
			auto ic = int(cell.GetNumber());
			CColourPickerDlg Dlgcol(MFC_Util::GetColorLine(ic));
			if (Dlgcol.DoModal() == IDOK) {
				COLORREF iv = Dlgcol.GetColour();
				MFC_Util::SetColorLine(ic, iv);
			}
			QuickSetBackColor(1, row, MFC_Util::GetColorLine(ic));
			RedrawCell(1, row);
			changemade = true;
		}
	}
	else if (item == 2001) {
		if (m_Mode == 1) {
			int colb = MFC_Util::GetProfileIntNo("ColorLegendBackground", 0xf0f0ff);;
			CColourPickerDlg Dlgcol(colb);
			if (Dlgcol.DoModal() == IDOK) {
				COLORREF iv = Dlgcol.GetColour();
				m_pUtil->SetColorLegendBackground(iv);
			}
			changemade = true;
			QuickSetBackColor(3, 4, m_pUtil->GetColorLegendBackground());
			RedrawCell(3, 4);
		}

	}
	else if (item == 2002) {
		if (m_Mode == 1) {
			int colb = MFC_Util::GetProfileIntNo("ColorLegendBackgroundFrame", 0xf0f0ff);;
			CColourPickerDlg Dlgcol(colb);
			if (Dlgcol.DoModal() == IDOK) {
				COLORREF iv = Dlgcol.GetColour();
				m_pUtil->SetColorLegendBackgroundFrame(iv);
			}
			changemade = true;
			QuickSetBackColor(4, 4, m_pUtil->GetColorLegendBackgroundFrame());
			RedrawCell(4, 4);
		}
	}
	else if (item == 2003) {
		if (m_Mode == 1) {
			int colb = m_pUtil->GetColorBoxEdge();
			CColourPickerDlg Dlgcol(colb);
			if (Dlgcol.DoModal() == IDOK) {
				COLORREF iv = Dlgcol.GetColour();
				m_pUtil->SetColorBoxEdge(iv);
			}
			changemade = true;
			QuickSetBackColor(2, 6, m_pUtil->GetColorBoxEdge());
			RedrawCell(2, 6);
		}
	}
	else if (item == 2004) {
		if (m_Mode == 4) {
			int colb = m_pUtil->GetColorDist();
			CColourPickerDlg Dlgcol(colb);
			if (Dlgcol.DoModal() == IDOK) {
				COLORREF iv = Dlgcol.GetColour();
				m_pUtil->SetColorDist(iv);
			}
			changemade = true;
			QuickSetBackColor(col, row, m_pUtil->GetColorDist());
			RedrawCell(1, row);
		}
	}
	else if (item == 2005) {
		if (m_Mode == 3) {
			CString c_str;
			c_str = "a";
			GetCell(col, row, &cell);
			string oldtext;
			oldtext = cell.GetStdLabelText();
			if (UGCT_CHECKBOX == cell.GetCellType())
				oldtext = cell.GetStdLabelText();
			else
				oldtext = cell.GetStdText();
			LegendChangeDlg dlg(oldtext);
			if (dlg.DoModal() == IDOK) {

				c_str = dlg.m_newtext.c_str();

			}
			ChangeVariableLegends(col, row, c_str);
			cell.SetLabelText(c_str);
			SetCell(col, row, &cell);

			RedrawCell(col, row);
			changemade = true;
		}
	}
	else if (item == 2410) {
		int j = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, row);
		if (m_pSimDoc->m_PlotStorage.Remove(j)) {
			m_pProp->Reset_Selected_Or_All_BasicVector();
			m_pUtil->m_FileSave.OnReDraw(true);
		}
	}
	else if (item == 2411) {
		// Add new Curve with new realisation
		m_pSimDoc->m_PlotStorage.AddCurve();
		if (m_PGRepit) {

			int il = m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber) - 1;
			int j = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, il);
			int irep = m_pSimDoc->m_PlotStorage.GetPGRepitNo(j);
			CPG* pPG = (CPG*)m_pSimDoc->m_PlotStorage.GetPtr(j);
			long nrep = pPG->GetNumRepititions();
			irep++;
			irep = (irep - 1) % nrep + 1;
			m_pSimDoc->m_PlotStorage.SetPGRepitNo(j, irep);
			m_pUtil->m_FileSave.OnReDraw(true);
		}
		else if (m_ResidualSource) {
			int il = m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber) - 1;
			int j = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, il);
			int LogLi = m_pSimDoc->m_PlotStorage.GetLogLiVersion(j);
			if (LogLi == 3)
				LogLi = 2;
			else if (LogLi == 2)
				LogLi = 3;
			else if (LogLi == 0)
				LogLi = 2;
			auto itype = m_pSimDoc->m_PlotStorage.GetDataType(j);
			if (itype != DATA_TYPES::SIM_VAL) {
				m_pSimDoc->m_PlotStorage.SetLogLiVersion(j, LogLi);
			}
			m_pUtil->m_FileSave.OnReDraw(true);

		}

	}
	else if (item == 2412) {
		int il = m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber) - 1;
		m_pSimDoc->m_PlotStorage.AddCurve(0, il);
		if (m_ResidualSource) {
			il = m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber) - 1;
			int j = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, il);
			auto ResRunNo = m_pSimDoc->m_PlotStorage.GetValRunNo(j);
			ResRunNo = ResRunNo % m_pSimDoc->MR_Get_TotalNumberofRuns() + 1;
			if (il > 2) {
				int jj = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, il - 1);
				if (ResRunNo <= m_pSimDoc->m_PlotStorage.GetValRunNo(jj))
					ResRunNo = m_pSimDoc->m_PlotStorage.GetValRunNo(jj) + 1;
			}
			auto itype = m_pSimDoc->m_PlotStorage.GetDataType(j);
			if (itype != DATA_TYPES::SIM_VAL) {

				m_pSimDoc->m_PlotStorage.SetValRunNo(j, ResRunNo);
			}
			m_pUtil->m_FileSave.OnReDraw(true);

		}

		}
		else if (item >= 2017 && item <= 2040) {
			if (item < 2021) { CHART_OPTION value = CHART_OPTION(item - 2017); m_pUtil->SaveCurrentLinePatternScheme(value); return true; }
			else if (item < 2031) { CHART_OPTION value = CHART_OPTION(item - 2027); m_pUtil->RetrieveSavedLinePatternScheme(value); return true; }
			else if (item < 2041) { CHART_OPTION value = CHART_OPTION(item - 2037); m_pUtil->ResetLinePatternScheme(value);	return true; }
		}
		else if(item>=3000&&item<=4000) {
			int ans=GotoCell(item-2999,0);
			m_colpos=item-2999;

			RedrawAll();
		}
		else if(item>4000&&item<5000) {
			enum class DATA_TYPES itype=DATA_TYPES(item-4001);
			if(itype<DATA_TYPES::PGDATA&&m_pSimDoc!=nullptr) {
				m_pSimDoc->m_PlotStorage.SetDataType(m_pProp->m_ChartNumber, itype);
				if(m_pProp->SetChartType(size_t(itype))){
					changemade=true;
					reload=true;
				}
			}
			else if(item<4021) {
				GotoCell(item-4011,2);
				RedrawCell(item-4011,2);
			}
			else if(item==4021) {
				if(m_pProp->m_RoundedFrame)
					m_pProp->m_RoundedFrame=false;
				else
					m_pProp->m_RoundedFrame=true;
				MFC_Util::WriteProfileInt( "Rounded frame",m_pProp->m_RoundedFrame);
		
			}
			else if(item==4022) {
				if(m_pProp->m_CommonColorForSubCharts)
					m_pProp->m_CommonColorForSubCharts=false;
				else
					m_pProp->m_CommonColorForSubCharts=true;
				MFC_Util::WriteProfileInt( "CommonColorsSubCharts",m_pProp->m_CommonColorForSubCharts);
		
			}
			else if(item==4099) {
				if(m_pUtil->m_ToBeUpdated) {
					if(	m_pUtil->m_ParameterPlotMode) {
						m_ParameterPlot.OnInit();


					}
					else {
						m_pUtil->m_FileSave.OnReDraw(true);
						
					}
					m_pUtil->m_ToBeUpdated=false;
					QuickSetBackColor(-1,-1,m_ColorClick);
				}

			}
			else if(item==4200) {

				GotoCell(9,2);
				RedrawCell(9,2);
			}
			else if(item==4201) {

				GotoCell(10,2);
				RedrawCell(10,2);
			}
			

		}
		else if(item>=5000&&item<=5100&&m_pSimDoc!=nullptr){
			if (item == 5002) {
				m_pSimDoc->m_PlotStorage.ToggleAllSectionNo();
			}
			if (item == 5003) {
				m_pSimDoc->m_PlotStorage.EnableAllSectionNo(true);
			}
			if (item == 5004) {
				m_pSimDoc->m_PlotStorage.EnableAllSectionNo(false);
			}
			else if (m_pProp->m_ReShape.m_CommonWithDay) {
				int ArrayIndex = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, 0);
				if (item == 5000)
					m_pSimDoc->m_PlotStorage.RemoveAllSectionCurves();
				m_pSimDoc->m_PlotStorage.AddCurve(5000);
				if (item == 5000 && m_pSimDoc != nullptr)
					m_pSimDoc->m_PlotStorage.Remove(ArrayIndex);
			}
			else if (m_pProp->m_ReShape.m_CommonDayNo) {
				int ArrayIndex = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, 0);
				if (item == 5000)
					m_pSimDoc->m_PlotStorage.RemoveAllSectionCurves();
				m_pSimDoc->m_PlotStorage.AddCurve(5000);
				if (item == 5000 && m_pSimDoc != nullptr)
					m_pSimDoc->m_PlotStorage.Remove(ArrayIndex);


			}
			changemade=true;

		}
	return changemade;
}
void ChartCugCommands::OnChangeAnnimationSteps()
{

	switch (m_AnnimateStepValue) {

	case 0:
		m_step=1;
		break;
	case 1:
		m_step=10;
		break;
	case 2:
		m_step=50;
		break;
	case 3:
		m_step=100;
		break;
	case 4:
		m_step=500;
		break;
	}
	// TODO: Add your control notification handler code here
}
void ChartCugCommands::OnStartAnnimation()
{
	    if(m_pView->m_ChartViewer.getViewPortWidth()>=0.99) {

			m_pView->m_ChartViewer.setViewPortWidth(0.2);
			m_TimeSeriePlot.m_Zoom.NumberOfWindows=5;
			m_TimeSeriePlot.m_Zoom.SubWindow=10;
			m_TimeSeriePlot.m_Zoom.StartPos=0;
			m_TimeSeriePlot.m_Agg.AggregateRatio=1;
			m_pView->m_ChartViewer.updateViewPort(true, true);
			m_pZoomTimeSerie->OnViewPortChanged();

		}
		m_pUtil->m_FileSave.m_NotifySaveFile=false;
		SetTimer(ID_TIMER, m_interval, nullptr);
		if(m_AnimationRunning) {
			m_AnimationRunning=false;
		}
		else {
			m_AnimationRunning=true;
		}
		

}

void ChartCugCommands::OnChangeAnnimationSpeed()
{

	switch (m_AnnimateValue) {
	case 0:
		m_interval=5000;
		break;
	case 1:
		m_interval=1000;
		break;
	case 2:
		m_interval=500;
		break;
	case 3:
		m_interval=100;
		break;
	case 4:
		m_interval=10;
		break;
	}
	if(m_AnimationRunning) {
		KillTimer(ID_TIMER);
		SetTimer(ID_TIMER, m_interval, nullptr);
	}
	// TODO: Add your control notification handler code here
}
