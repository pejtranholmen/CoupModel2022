#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include "..\..\Util.h"

#include "..\..\NewBase\FileScanner.h"
#include "ChartCugClick.h"
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
#include "..\..\Chart\ChartViewer.h"
#include "..\..\CoupModelView.h"

ChartCugClick::ChartCugClick(void)
{
	//m_pChartCug=MySingle::instance().GetChartCugPointer();
	m_CurrentTopRow=-1;
}
ChartCugClick::~ChartCugClick(void)
{
}
void ChartCugClick::ActionDuringRealTimeMode(size_t col, size_t row)
{
	CUGCell cell; 
	CString str, txt;
	size_t n1=m_pProp->m_Legend1.size();
	m_CurrentTopRow=GetTopRow();

			GetCell(col,row,&cell);
			switch(row) {
			case 0:
				if(col==0&&m_pSimDoc->IsMultiSimulation()) {
					m_RealTime.InitData(m_pSimDoc);
					m_RealTime.SetMultiSimDisplay(cell.GetBool());


				}
				else if(col==0&&!m_pSimDoc->IsMultiSimulation()) {
					m_pSimDoc->m_Simulator.m_pModelInfo->SetGoSim();
						//PARAMETERS_mp_RUN_STEP=1;
						//PARAMETERS_mp_RUN_PAUSE=0;
	
				}
				else if(col==1) {
					if(cell.GetBool())
						m_pUtil->SetRemoveLegends(0);
					else
						m_pUtil->SetRemoveLegends(2);


				}
				break;
			case 1:
				if(col==1) return;
				if(cell.GetBool())
					m_pSimDoc->m_Simulator.m_pModelInfo->SetPause();
					//PARAMETERS_mp_RUN_PAUSE=1;
				else {
					m_pSimDoc->m_Simulator.m_pModelInfo->SetGoSim();
//					PARAMETERS_mp_RUN_PAUSE=0;
//					PARAMETERS_mp_RUN_STEP=0;
				}
				RedrawCell(0,1);
				OnUpdate();
				break;
			case 2:
				if(cell.GetBool()) {
					m_pSimDoc->Set_MultiStopped();
					MFC_Util::MessageBox("Please wait while multirun simulation will be closed ! ");
				}
				break;
			case 3:
				if(cell.GetBool())
					m_pUtil->SetRemoveLegends(0);
				else
					m_pUtil->SetRemoveLegends(2);
			}

}
void ChartCugClick::ActionDuringParameterPlotMode(size_t col, size_t row)
{
	CUGCell cell; 
	CString str, txt;
			GetCell(col,row,&cell);
			switch(row) {
				case 0:
				if(col==0) {
					if(cell.GetBool()) {
						m_ParameterPlot.m_PlotNewValue=true;
						m_ParameterPlot.OnInit();
					}
					else {
						m_ParameterPlot.m_PlotNewValue=false;
						m_ParameterPlot.OnInit();
					}
					OnUpdate();
				}
				else if(col==5) {
					if(cell.GetBool()) {
						m_pUtil->m_FileSave.m_SaveFile=true;
					}
					else {
						m_pUtil->m_FileSave.m_SaveFile=false;
					}

				}
				break;
				case 1:
				if(col==0) {
					if(cell.GetBool()) {
						m_ParameterPlot.m_PlotDefault=true;
						m_ParameterPlot.OnInit();
					}
					else {
						m_ParameterPlot.m_PlotDefault=false;
						m_ParameterPlot.OnInit();
					}
					OnUpdate();
				}
				else if(col==5) {
					if(cell.GetBool()) {
						m_pUtil->m_FileSave.m_NotifySaveFile=true;
					}
					else {
						m_pUtil->m_FileSave.m_NotifySaveFile=false;
					}

				}
				break;
				case 2:
				if(col==0) {
					if(cell.GetBool()) {
						m_ParameterPlot.m_PlotPrevious=true;
						m_ParameterPlot.OnInit();
					}
					else {
						m_ParameterPlot.m_PlotPrevious=false;
						m_ParameterPlot.OnInit();
					}
					OnUpdate();
				}
				break;
				case 3:
				if(col==0) {
					if(cell.GetBool()) {
						m_ParameterPlot.ApplyNewvalue();
						OnUpdate();
					}
				}
				break;
			}


}
void ChartCugClick::ActionDuringTablePlotMode(size_t col, size_t row)
{
	CUGCell cell; 
	CString str, txt;
				GetCell(col,row,&cell);
			switch(row) {
				case 0:
				if(col==5) {
					if(cell.GetBool()) {
						m_pUtil->m_FileSave.m_SaveFile=true;
					}
					else {
						m_pUtil->m_FileSave.m_SaveFile=false;
					}

				}
				break;
				case 1:
				if(col==5) {
					if(cell.GetBool()) {
						m_pUtil->m_FileSave.m_NotifySaveFile=true;
					}
					else {
						m_pUtil->m_FileSave.m_NotifySaveFile=false;
					}

				}
				break;
			}

}
void ChartCugClick::ActionDuringTimeSerieMode(size_t col, size_t row) {
	CUGCell cell; 
	CString str, txt;
			size_t i=0;
			if(col==0) {
				GetCell(col,row,&cell);
				if(row==3) {
					return;


				}

				else if(row==0) {
				
					GetCell(0,0,&cell);
					cell.SetText("Zoom In");
					if(m_pUtil->m_FileSave.m_ZoomX) {
						cell.SetBackColor(m_ColorClick);

					}
					else {
						if(m_pUtil->m_FileSave.m_ZoomXY==false)
							m_pUtil->m_FileSave.m_ZoomX=true;
						m_pUtil->m_FileSave.OnZoomInPB(&m_pView->m_ChartViewer);
						cell.SetBackColor(m_ColorChanged);


					}
				}
				else if(row==4)  {
					if(cell.GetBool()) {
						m_pProp->m_ReShape.m_Symbol=true;
					}
					else {
						m_pProp->m_ReShape.m_Symbol=false;
	
					}


				}

				SetCell(col,row,&cell);
				
				
				m_pUtil->m_FileSave.OnReDraw(true);
				return;



			}
			else if(col==1) {
				GetCell(col,row,&cell);
				if(row==0) {
				
					GetCell(1,0,&cell);
					if(m_ZoomLevel>1) {
						if(m_pUtil->m_FileSave.m_ZoomXY==false)
							m_pUtil->m_FileSave.m_ZoomX=true;
						m_pUtil->m_FileSave.OnZoomOutPB(&m_pView->m_ChartViewer);
						cell.SetBackColor(m_ColorChanged);


					}
				}
				else if(row==4) {
					if(cell.GetBool()) {
						m_pProp->m_ReShape.m_AddLine=true;
					}
					else {
						m_pProp->m_ReShape.m_AddLine=false;
					}

					SetCell(1,4,&cell);
				}


				SetCell(col,row,&cell);
				m_pUtil->m_FileSave.OnReDraw(true);
				return;


			}
			else if(col==2) {
				GetCell(col,row,&cell);
				/*if(row==0) {
					if(cell.GetBool()) {
						m_pProp->m_ReShape.m_Cumulated=true;
					}
					else {
						m_pProp->m_ReShape.m_Cumulated=false;
					}
					m_pProp->ResetXScale();
					m_pProp->m_PrevChartNumber=-1;
				}
				else */
				if(row==1) {
					if(cell.GetBool()) {
						m_pProp->m_ReShape.m_DerivateValues=true;
					}
					else {
						m_pProp->m_ReShape.m_DerivateValues=false;
					}
					m_pProp->m_PrevChartNumber=-1;



				}
				else if(row==3) {
					if(cell.GetBool()) {
						m_pProp->m_ReShape.m_CommonScale=true;
					}
					else {
						m_pProp->m_ReShape.m_CommonScale=false;
					}


				}
				else if(row==4) {
					if(cell.GetBool()) {
						m_pProp->SetDistributionLogScale(true);
					}
					else {
						m_pProp->SetDistributionLogScale(false);
					}
				}

	
				SetCell(col,row,&cell);
				m_pUtil->m_FileSave.OnReDraw(true);
				return;

			}
			else if(col==3) {
				GetCell(col,row,&cell);
				if(row==0) {
					OnStartAnnimation();
			
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
					RedrawCell(3,0);

					if(m_AnimationRunning) {
						GetCell(3,1,&cell);
			
						if(m_pUtil->m_FileSave.m_SaveFile) {
							cell.SetBool(true);
							cell.SetBackColor(m_ColorChanged);
							cell.SetLabelText("Stop File Saving");
						}
						else {
							cell.SetBool(false);
							cell.SetBackColor(m_ColorClick);
							cell.SetLabelText("Start File Saving");
						}	
						SetCell(3,1,&cell);
						RedrawCell(3,1);

					}
					return;
				
				}
				else if(row==1) {
		
					if(cell.GetBool()) {
						m_pUtil->m_FileSave.m_SaveFile=true;
						cell.SetLabelText("Stop File Saving");
						cell.SetBackColor(m_ColorChanged);
					}
					else {
						m_pUtil->m_FileSave.m_SaveFile=false;
						cell.SetLabelText("Start File Saving");
						cell.SetBackColor(m_ColorClick);
					}

					SetCell(3,1,&cell);
				
				}
				else if(row==4) {
					if(m_Hpos<m_ZoomLevel) {
						m_Hpos++;
						m_TimeSeriePlot.OnInit(&m_pView->m_ChartViewer);
						m_TimeSeriePlot.DrawTimeSeriePlot(&m_pView->m_ChartViewer);
					}

				}

			}
			else if(col==4) {
				if(row==4) {
					if(m_Hpos>1) {
						m_Hpos--;
						m_TimeSeriePlot.OnInit(&m_pView->m_ChartViewer);
					    m_TimeSeriePlot.DrawTimeSeriePlot(&m_pView->m_ChartViewer);
					}
				}
			}
			else if(col==5) {
				GetCell(col,row,&cell);
				if(row==0) {
					m_pProp->ResetSerie1(true);
					if(!cell.GetBool()) {
						m_pProp->m_ReShape.m_CommonDayNo=false;

					}
					else {
						m_pProp->m_ReShape.m_CommonDayNo=true;
						
					}
					m_pZoomTimeSerie->OnCommonday();
					m_pUtil->m_FileSave.OnReDraw(true);

				}
				else if(row==1) {
					m_pProp->ResetSerie1();
					if(!cell.GetBool()) {
						m_pProp->m_ReShape.m_CommonWithDay=false;

					}
					else {
						m_pProp->m_ReShape.m_CommonWithDay=true;
						//OnCommonWithinDay();
					}
					m_pZoomTimeSerie->OnCommonWithinDay();
					m_pUtil->m_FileSave.OnReDraw(true);


				}
				else if(row==2) {
					if(!cell.GetBool()) {
						m_pProp->m_ReShape.m_NormalDist=false;
						m_pProp->m_ReShape.m_SortedSeries=false;

					}
					else {
						m_pProp->m_ReShape.m_NormalDist=true;
						m_pProp->m_ReShape.m_SortedSeries=true;

					}
					m_pProp->m_XScaleChanged[0]=false;
					m_TimeSeriePlot.LoadTimeSerieData();
					m_TimeSeriePlot.OnInit(&m_pView->m_ChartViewer);
					//m_pView->m_ChartViewer.updateViewPort(true, true);
				//	BaseChart *c;
				//	c=m_pView->m_ChartViewer.getChart();
					m_TimeSeriePlot.DrawTimeSeriePlot(&m_pView->m_ChartViewer);





				}
				else if(row==3) {
					m_pProp->m_ReShape.m_NormalDist=false;
					if(!cell.GetBool())
						m_pProp->m_ReShape.m_SortedSeries=false;
					else
						m_pProp->m_ReShape.m_SortedSeries=true;

					m_pProp->m_XScaleChanged[0]=false;
					m_TimeSeriePlot.LoadTimeSerieData();
					m_TimeSeriePlot.OnInit(&m_pView->m_ChartViewer);
			//		m_pView->m_ChartViewer.updateViewPort(true, true);

					m_TimeSeriePlot.DrawTimeSeriePlot(&m_pView->m_ChartViewer);



				}
				else if(row==4) {

					if(!cell.GetBool())  {
						m_pProp->m_ReShape.m_RegLine=false;
					}
					else {
						m_pProp->m_ReShape.m_RegLine=true;

					}
					m_pUtil->m_FileSave.OnReDraw(true);
				}
				else if (row == 5) {

					if (!cell.GetBool()) {
						m_pProp->m_ReShape.m_ShowMissingValue = false;
					}
					else {
						m_pProp->m_ReShape.m_ShowMissingValue = true;

					}
					m_pUtil->m_FileSave.OnReDraw(true);
				}
				else if (row == 6) {
					if (cell.GetBool())
						m_pProp->m_AggregateTime = true;
					else
						m_pProp->m_AggregateTime = false;
					m_pUtil->m_FileSave.OnReDraw(true);
				}

				SetCell(col,row,&cell);
	
				return;		
			}
			else if(col==6) {
				GetCell(8,row,&cell);
				if(m_TimeSeriePlot.m_ChartInfo.MultiPGMode) {
					switch (row) {
					case 0:
						OnNewCurve();
						break;
					case 1:
						cell.SetBool(false);
						m_TimeSeriePlot.OnUpdateSequence();
						m_pUtil->m_FileSave.OnReDraw(true);
						

						break;
					case 2:
						cell.SetBool(false);
						OnNewSeq2();


						break;
					}
					RedrawCell(col,row);
				}




			}
			else if(col==8) {
				GetCell(8,row,&cell);
				if(row==0) {
					if(cell.GetBool()) {
						m_pUtil->m_FileSave.m_SaveFile=true;
					}
					else {
						m_pUtil->m_FileSave.m_SaveFile=false;
					}

				}
				else if(row==1) {
					if(cell.GetBool()) {
						m_pUtil->m_FileSave.m_NotifySaveFile=true;
					}
					else {
						m_pUtil->m_FileSave.m_NotifySaveFile=false;
					}

				}
				else if(row==2) 
					m_pUtil->m_FileSave.SaveToExtraChartWindow();
				else if (row == 3) {
					GetCell(8, row, &cell);

				}
					
			}
			else if(col==9) {
				GetCell(9,row,&cell);
				if(row==2) {
					if(cell.GetBool()) {
						m_TimeSeriePlot.m_Contour.ScatterContourPoints=true;
						cell.SetLabelText("Remove Scatter Points");
						cell.SetBackColor(m_ColorClick);
					}
					else {
						m_TimeSeriePlot.m_Contour.ScatterContourPoints=false;
						cell.SetLabelText("Add Scatter Points");
						cell.SetBackColor(m_ColorChanged);

					}
					SetCell(9,2,&cell);
					m_pUtil->m_FileSave.OnReDraw(true);
				}
				else if(row==3) {
					if(cell.GetBool()) {
						m_TimeSeriePlot.m_Contour.SplineContourInterPolation=true;
						cell.SetLabelText("Remove Spline Interpolation");
						cell.SetBackColor(m_ColorClick);
					}
					else {
						m_TimeSeriePlot.m_Contour.SplineContourInterPolation=false;
						cell.SetLabelText("Use Spline Interpolation");
						cell.SetBackColor(m_ColorChanged);

					}
					SetCell(9,3,&cell);
					m_pUtil->m_FileSave.OnReDraw(true);
				}
				else if(row==4) {
					if(cell.GetBool()) {
						m_TimeSeriePlot.m_Contour.ScaleContourChart=true;
						cell.SetLabelText("Use Automatic Scaling");
						cell.SetBackColor(m_ColorClick);
					}
					else {
						m_TimeSeriePlot.m_Contour.ScaleContourChart=false;
						cell.SetLabelText("Use Maximal Chart Size");
						cell.SetBackColor(m_ColorChanged);

					}
					SetCell(9,4,&cell);
					m_pUtil->m_FileSave.OnReDraw(true);
				}


			}

}
void ChartCugClick::OnNewSeq2()
{
	//if(OnUpdateSequence()) {
	size_t runno;

	size_t incrementstep = 1;
	size_t NumAgg = 1;
	Ps* pAgg = m_pSimDoc->GetPs("Aggregate Calib Number");
	if (pAgg != nullptr) NumAgg = size_t(pAgg->GetValue());
	if (NumAgg > 1) incrementstep = NumAgg;


		for(size_t il=0;il<m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);il++) {
				auto j=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, il);
				auto itype=m_pSimDoc->m_PlotStorage.GetDataType(j);
				if((itype==DATA_TYPES::SIM_VAL&&m_TimeSeriePlot.m_ChartInfo.MultiValMode)) {
					float validv=0;
					size_t count=0;
					if(m_TimeSeriePlot.m_ChartInfo.MultiPGMode)
						runno=m_pSimDoc->m_PlotStorage.GetValRunNo(j);
					else if (m_TimeSeriePlot.m_ChartInfo.MultiValMode)
						runno=m_pSimDoc->m_PlotStorage.GetValRunNo(j)+1;

					while(validv<0.5&&count<m_pSimDoc->MR_Get_TotalNumberofRuns()) {
						runno+=incrementstep;
						m_pProp->m_MultiSequence=(runno-1)%m_pSimDoc->MR_Get_TotalNumberofRuns()+1;	
						count++;
						validv=m_pSimDoc->m_MStorage.GetMultiP(m_pProp->m_MultiSequence-1,m_pSimDoc->MR_Get_NumTotalPar());
					}
					if(m_TimeSeriePlot.m_ChartInfo.MultiPGMode)
						m_pSimDoc->m_PlotStorage.SetValRunNo(j, m_pProp->m_MultiSequence);
					else if (m_TimeSeriePlot.m_ChartInfo.MultiValMode)
						m_pSimDoc->m_PlotStorage.SetValRunNo(j, m_pProp->m_MultiSequence-1);
				}
		}
		m_pUtil->m_FileSave.OnReDraw(true);
	//}
	// TODO: Add your control notification handler code here
}
void ChartCugClick::ParameterChangeRealTimeMode(size_t col, size_t row)
{
	CUGCell cell; 
	CString str, txt;
			GetCell(col,row,&cell);
		SimB* pPar;
		if(col==2) {
			BOOL v=cell.GetBool();
			GetCell(0,row,&cell);
			size_t idim=cell.GetParam();
			GetCell(1,row,&cell);
			size_t ip=cell.GetParam();
			if(idim>=0&&ip>=0) {
				//m_pSimDoc->MR_SetResetValMonitoring();
				//m_pSimDoc->MR_SetResetMonitoring();
				m_RealTime.ResetValMonitoring();
				pPar=m_pSimDoc->MR_Get_pPar(idim+1,ip+1);
				if(pPar!=nullptr) {
					m_RealTime.InitData(m_pSimDoc);
					size_t TabIndex=m_pSimDoc->MR_GetTabIndex(idim+1,ip+1);
					if(TabIndex>=0) {
						((P*)pPar)->MR_Set_Monitoring(TabIndex, size_t(v));
					}
					else
						((Ps*)pPar)->MR_Set_Monitoring(size_t(v));
				}

			}
			OnUpdate();
		}


}
void ChartCugClick::ValidationRealTimeMode(size_t col, size_t row)
{
	CUGCell cell; 
	CString str, txt;
			if(col==2) {
				GetCell(col, row, &cell);
				BOOL v=cell.GetBool();
				if(v) {
					m_pSimDoc->m_ValidationData.SetValMonitoring(row,1);
				}
				else {
					m_pSimDoc->m_ValidationData.SetValMonitoring(row,0);

				}
				OnUpdate();
			}
}
void ChartCugClick::ChartDetailsParameterPlotMode(size_t col, size_t row)
{
		CUGCell cell; 
		GetCell(col,row,&cell);
		if(col==2) {
			Func *funcpointer=m_ParameterPlot.GetCurrentFuncPointer();
			PLOT_PARAM plotparam=funcpointer->GetPlotParam();
			 if(row==0) {
				 if(cell.GetBool())
					plotparam.LogXScale=true;
				 else
					 plotparam.LogXScale=false;
			 }
			 else if(row==1) {
				 if(cell.GetBool())
					plotparam.LogYScale=true;
				 else
					 plotparam.LogYScale=false;
			 }
			funcpointer->SetPlotParam(plotparam);
			m_ParameterPlot.OnInit();
			m_pUtil->m_FileSave.m_NoChange=false;
			OnUpdate();
		}

}
void ChartCugClick::LayersParameterPlotMode(size_t col, size_t row)
{
		CUGCell cell; 
		GetCell(col,row,&cell);
		if(col==0) {
			m_ParameterPlot.m_pParTable->ResetPlotIndex();
			bool first;
			first=true;
			for(size_t i=0; i<m_ParameterPlot.m_MaxLayer;i++) {
					GetCell(0,i,&cell);
					if(cell.GetBool()) {
						m_ParameterPlot.m_pParTable->SetPlotIndex(i);
						if(first) {
							m_ParameterPlot.m_TableIndex=i;
							first=false;
						}
					}
			}
			OnUpdate();
			m_ParameterPlot.loadData();
			m_ParameterPlot.OnInit();
			m_pUtil->m_FileSave.m_NoChange=false;
			OnUpdate();
		}

}
void ChartCugClick::OnNewCurve()
{
	// TODO: Add your control notification handler code here
	
	size_t il=m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber)-1;	
	size_t j=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, il);
	auto plot = m_pSimDoc->m_PlotStorage.GetPlotInfo(j);
	auto itype=m_pSimDoc->m_PlotStorage.GetDataType(j);
	size_t actual_chart =0;
	if(m_TimeSeriePlot.m_ChartInfo.MultiValMode) {
		il = 0;
		m_pProp->m_MultiSequence++;
		m_pProp->m_multi_ResidualSequence++;
	}
	
	if(m_pSimDoc->m_PlotStorage.AddCurve(0,il)&&(m_TimeSeriePlot.m_ChartInfo.MultiPGMode||m_TimeSeriePlot.m_ChartInfo.MultiValMode||(itype==DATA_TYPES::PGDATA&&m_PGRepit))){
		m_pProp->m_PrevChartNumber=-1;
		if(m_TimeSeriePlot.OnUpdateSequence())
			m_TimeSeriePlot.MakeTimeSeriePlot();
	}
	else {
		if(m_TimeSeriePlot.OnUpdateSequence())
			m_pUtil->m_FileSave.OnReDraw(true);

	}
}
void ChartCugClick::VariablesTimeSerieMode(size_t col, size_t row) {
	CUGCell cell; 
	CString str, txt;
	auto ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, row);
	if (ArrayIndex == string::npos) return;
			auto koll=m_pSimDoc->m_PlotStorage.GetPGSectionNo(ArrayIndex);
			auto p_info= m_pSimDoc->m_PlotStorage.GetPlotInfo(ArrayIndex);

			auto datatype=m_pSimDoc->m_PlotStorage.GetDataType(ArrayIndex);

			//if(itype<0) return;
			if(koll==0&&col==0) return;
			size_t value, index;
			index=row;
			if(col==0&&!p_info.RepitDataType||m_pProp->m_ReShape.m_CommonDayNo&&col==0||m_pProp->m_ReShape.m_CommonWithDay&&col==0){

				GetCell(0,row,&cell);
				if(cell.GetBool()) {
					m_pSimDoc->m_PlotStorage.EnableSectionNo(ArrayIndex, true);
				}
				else {
					m_pSimDoc->m_PlotStorage.EnableSectionNo(ArrayIndex, true);
				}
				m_pUtil->m_FileSave.OnReDraw(true);
	
			}
			else if((datatype==DATA_TYPES::SIM_VAL|| p_info.RepitDataType)&&m_pSimDoc!=nullptr&&col==8) {
					//m_pProp->m_MultiSequence++;
					//((CZoomscrolldemoDlg*)m_pDlg)->OnUpdateSequence();
					if(m_TimeSeriePlot.m_ChartInfo.MultiPGMode)
						m_pProp->m_MultiSequence=m_pSimDoc->m_PlotStorage.GetValRunNo(ArrayIndex);
					else if(m_TimeSeriePlot.m_ChartInfo.MultiValMode)
						m_pProp->m_MultiSequence=m_pSimDoc->m_PlotStorage.GetValRunNo(ArrayIndex)+1;
					m_TimeSeriePlot.OnUpdateSequence(ArrayIndex);
					m_pUtil->m_FileSave.OnReDraw(true);

			}
			else if(datatype == DATA_TYPES::PGDATA&&m_PGRepit) {
				auto irep=p_info.PGRepitNo;
				CPG *pPG=p_info.pPG;
				auto nrep=pPG->GetNumRepititions();
				irep++;
				irep=(irep-1)%nrep+1;
				m_pSimDoc->m_PlotStorage.SetPGRepitNo(ArrayIndex, irep);
				m_pUtil->m_FileSave.OnReDraw(true);
			}
			else if(col==6&&m_pSimDoc!=nullptr) {
				value=m_pSimDoc->m_PlotStorage.GetSubChart(ArrayIndex);
				if(value<m_pProp->m_NumChartX*m_pProp->m_NumChartY)
					value++;
				else
					value=1;
				m_pSimDoc->m_PlotStorage.SetSubChart(ArrayIndex, value);
				QuickSetNumber(col,row,value);
				RedrawCell(col,row);
				
				CMainFrame *p_mainframe = (CMainFrame*) AfxGetMainWnd();
			//	MySingle::instance().GetCugPointer()->UpdateChartInfo();

				m_pUtil->m_FileSave.OnReDraw(true);
			}

}