#include "../std.h"
#include "TimeSeriePlot.h"
#include "../Util.h"
#include "../NewBase\Doc.h"
#include "./ChartProperties\ChartProp.h"
#include "./ChartProperties\ChartUtil.h"
#include "../Chart\PlotDesignInfo\PlotStorage.h"
#include "../Chart\ChartViewer.h"
#include "../Cug\ChartProperties\ChartCug.h"
#include "../ChartWnd.h"
#include "../MultiRun\SelectedEnsembles.h"
#include "../CoupModelView.h"
#include "./ZoomUtil\ZoomTimeSerie.h"
#include "../PG\PGCalc.h"

#include <math.h>

#include <algorithm>

TimeSeriePlot::TimeSeriePlot(void)
{

	m_PrevMC=nullptr;
	m_pSimDoc=nullptr;
	m_Started=false;
	m_Zoom.Hpos=0;
	m_Zoom.ZoomLevel=0;
	m_Contour.SecondLog=false;
	m_ChartIsMade = false;
}
TimeSeriePlot::~TimeSeriePlot(void)
{
}


void TimeSeriePlot::Init(Doc *pDoc) {
	m_pSimDoc=pDoc;
	m_pView=m_pSimDoc->GetModelView();

}
bool TimeSeriePlot::InitData(Doc *pDoc, int chartnum,  string PGFile, CChartViewer *view)
{
	m_pSimDoc=pDoc;
	m_pUtil->SetSymbolCodes();
	m_pSimDoc->m_PlotStorage.Init(m_pSimDoc);
	if(m_pSimDoc!=nullptr&&m_pSimDoc->IsFinishedSimulation()||PGFile!="") {
		m_pChartCug=MySingle::instance().GetChartCugPointer();
		m_pUtil->m_MultiRunPlot=false;
		m_pProp->m_ChartNumber=chartnum;
		m_pView=m_pSimDoc->GetModelView();
		m_pChartViewer=&m_pView->m_ChartViewer;
		m_pProp->m_MultiValid.AddOnlyValidRuns=false;
		m_pZoomTimeSerie=MySingle::instance().GetChartZoomPointer();
		m_pZoomTimeSerie->Init();

		m_pZoomTimeSerie->SetChartViewer(m_pChartViewer);
		m_pZoomTimeSerie->SetPlotStoragePointer(&m_pSimDoc->m_PlotStorage);
		return true;
	}
	return false;
}
bool TimeSeriePlot::InitMultiPGFilePlot(Doc *pSimDoc) {
	m_pSimDoc=pSimDoc;
	m_pProp->m_ChartNumber=m_pSimDoc->m_PlotStorage.GetCurrentChart();
	if(m_pProp->GetIndepFirst()==PGDAY) {
		m_pProp->SetFirstIndepSet(PGDAY, true);
	}
	else {
		m_pProp->m_ChartType= CHART_TYPES::CONTOUR_CHART;

	}
	m_pView=m_pSimDoc->GetModelView();
	m_pChartCug=MySingle::instance().GetChartCugPointer();

	m_pChartViewer=&m_pView->m_ChartViewer;
	if(m_pProp->GetIndepFirst()==PGDAY) {
		m_pUtil->m_MultiRunPlot=false;
		m_pUtil->m_MultiPGPlot=false;
	}
	else {
		m_pUtil->m_MultiRunPlot=false;
		m_pUtil->m_MultiPGPlot=true;
	}

	//m_pProp->m_MultiIndep.MultiPGFilePlot=true;
	/*if(m_pProp->m_MultiPG_dep.FirstIndepSet)
		m_pProp->m_Indep.First=PGVAR;
	if(m_pProp->m_MultiPG_dep.SecondIndepSet)
		m_pProp->m_Indep.Second=PGVAR;*/

	return true;
}
bool TimeSeriePlot::InitMultiRunPlot(Doc *pSimDoc,int chartnum, CHART_TYPES charttype, CChartViewer *view)
{
	CString str;
	m_pSimDoc=pSimDoc;
	m_pSimDoc->m_PlotStorage.Init(m_pSimDoc);

	if(m_pSimDoc!=nullptr) {
			// Check and open MultiRunFile
		if(m_pSimDoc->MR_Storage_Open()) {
			m_pUtil->m_MultiRunPlot=true;

			if (m_pProp->IsFirstIndepSet() && !m_pProp->IsSecondIndepSet() && charttype == CHART_TYPES::CONTOUR_CHART)
				charttype = CHART_TYPES::SCATTER;
			if (m_pProp->IsFirstIndepSet() && m_pProp->IsSecondIndepSet()&&m_pProp->GetIndepSecond()!=DATEAXIS)
				charttype = CHART_TYPES::CONTOUR_CHART;


			if(m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val()>0||m_pSimDoc->m_ValidationData.GetNumSumVarVariables()>0)
				m_pProp->m_MultiValid.AddOnlyValidRuns=true;
			m_pProp->m_ChartNumber=chartnum;
			m_pProp->m_ChartType=charttype;
			m_pView=m_pSimDoc->GetModelView();
			m_pChartViewer=&m_pView->m_ChartViewer;
			m_pChartCug=MySingle::instance().GetChartCugPointer();

			m_pZoomTimeSerie=MySingle::instance().GetChartZoomPointer();
			m_pZoomTimeSerie->Init();

			m_pZoomTimeSerie->SetChartViewer(m_pChartViewer);
			m_pZoomTimeSerie->SetPlotStoragePointer(&m_pSimDoc->m_PlotStorage);
			if(m_pProp->m_ChartType== CHART_TYPES::SCATTER) {
					// Check for two variables to make scatter plot			
					m_ChartInfo.NumVariables=m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);
					m_ChartInfo.NumOfPoints=m_pSimDoc->MR_Get_TotalNumberofRuns();
	

				/*	if(m_ChartInfo.NumVariables<2&&!m_pProp->IsIndepSet()) {
							MFC_Util::MessageBox("Independent variable select. Select also dependent variables !");
							return false;
					}*/
			}

			return true;
		}
		else {



		}
		
	}
	return false;
}

bool TimeSeriePlot::MakeTimeSeriePlot()
{

	m_pProp->Reset_Selected_Or_All_BasicVector(m_ChartInfo.NumVariables, m_pProp->GetNumOfChartsOnSceen(), true);

	if (LoadTimeSerieData()) {

		if (OnInit(&m_pView->m_ChartViewer)) {
			//m_ChartViewer=&m_pView->m_ChartViewer;
			if (!m_pUtil->m_Displayed)
				DrawTimeSeriePlot(&m_pView->m_ChartViewer);
			return true;
		}
		else
			return false;
	}
	else
		return false;
}
void TimeSeriePlot::DrawTimeSeriePlot(CChartViewer *viewer)
{
	m_ChartIsMade = false;
	MFC_Util m;
	m.SetPaneMessage("Starting to Prepare the Chart for Plotting");
	m_ChartOrigInfo.clear();
	
	UpdateGeneralProp();
	m_pProp->CheckChartType_Based_On_IndependentVar();
	if(!UpdateViewPortTime()) return;
	m_ChartInfo.NumVariables=m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);
	
	if(m_ChartInfo.NumVariables<=0) return;

	//m_pProp->Reset_Selected_Or_All_BasicVector(m_ChartInfo.NumVariables, m_pProp->GetNumOfChartsOnSceen(), true);

	size_t TotNumPrior;
	TotNumPrior=GetPrior_UpdateNumberOfVariables();
	//if(TotNumPrior==m_ChartInfo.NumVariables) m_pProp->Reset_Selected_Or_All_BasicVector(m_ChartInfo.NumVariables, m_pProp->GetNumOfChartsOnSceen(), false);

	
	UpdateDataDist();
	CreateViewPortData(TotNumPrior);
	AggregateData();
	MultiChart *mc;
	mc=CreateMultiCharts();
	if (mc == nullptr) return;

	if(m_pProp->m_ChartType== CHART_TYPES::CONTOUR_CHART||(m_pProp->IsFirstIndepSet()&&m_pProp->IsSecondIndepSet())) {
		m_pConLayer.resize(m_pProp->m_NumMultiChart);
		m_pColorAxis.resize(m_pProp->m_NumMultiChart);
	}
	else {
		m_pConLayer.resize(0);
		m_pColorAxis.resize(0);
	}
	bool DifferentScales=MakeAllMultiCharts(mc, TotNumPrior);

	if (m_pProp->m_ReShape.m_CommonScale&&DifferentScales||m_pProp->m_SyncAxis) {
		SetCommonScaleTo_Y_or_Z_Axis();
		DifferentScales = MakeAllMultiCharts(mc, TotNumPrior);
	}

	m_pUtil->m_FileSave.m_NoChange=false;
	m_pChartCug->OnUpdate();
	ShowChartonScreen_OptionallyCreateFile(mc);
	m.SetPaneMessage("The Chart is Ready");

}
bool TimeSeriePlot::OnUpdateSequence(int ArrayIndex)
{
	int counter, ichart, ilocal;
	CHART_TYPES icharttype;
	if(m_ChartInfo.PGRepitMode&&ArrayIndex<0&&!m_ChartInfo.MultiValMode)
		ArrayIndex=1;
	auto itype=m_pSimDoc->m_PlotStorage.GetDataType(ArrayIndex);
	if(itype==DATA_TYPES::PGDATA&&m_ChartInfo.PGRepitMode) {
		int irep=m_pSimDoc->m_PlotStorage.GetPGRepitNo(ArrayIndex);
		CPG *pPG=(CPG*)m_pSimDoc->m_PlotStorage.GetPtr(ArrayIndex);
		if(pPG==nullptr) pPG=m_pSimDoc->m_PlotStorage.GetPGValFilePointer(ArrayIndex);
		long nrep=pPG->GetNumRepititions();
		irep++;
		irep=(irep-1)%nrep+1;
		m_pSimDoc->m_PlotStorage.SetPGRepitNo(ArrayIndex, irep);
		m_pUtil->m_FileSave.OnReDraw(true, m_pSimDoc);
	}
	else if(m_ChartInfo.MultiPGMode||m_ChartInfo.MultiValMode) {
		float validv=0;
		int count=0;


		size_t incrementstep = 1;
		size_t NumAgg = 1;
		if (m_pSimDoc->m_MStorage.GetCyclicNumber() > 1) {
			incrementstep = m_pSimDoc->m_MStorage.GetCyclicNumber();
		}
		else {
			Ps* pAgg = m_pSimDoc->GetPs("Aggregate Calib Number");
			if (pAgg != nullptr) NumAgg = size_t(pAgg->GetValue());
			if (NumAgg > 1) incrementstep = NumAgg;
		}


		vector<size_t> AcceptedCandidates;


		while (count<int(m_pSimDoc->MR_Get_TotalNumberofRuns())) {
			validv = m_pSimDoc->m_MStorage.GetMultiP(count, m_pSimDoc->MR_Get_NumTotalPar());
			if (validv > .1) AcceptedCandidates.push_back(count);
			count += incrementstep;
		}
		if(AcceptedCandidates.size()> m_pProp->m_multi_ResidualSequence)
			m_pProp->m_MultiSequence = AcceptedCandidates[m_pProp->m_multi_ResidualSequence];


		int il=m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber)-1;
		bool valid=true;
		while(valid&&il>=0) {

			int j;
			if(ArrayIndex>=0&&!m_ChartInfo.MultiPGMode)
				j=ArrayIndex;
			else
				j=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, il);
			
			auto datatype=m_pSimDoc->m_PlotStorage.GetDataType(j);
			auto ens_type= m_pSimDoc->m_PlotStorage.GetEnsembleType(j);
			if(ens_type!=ENSEMBLE_TYPE::NO||datatype==DATA_TYPES::SIM_VAL||m_ChartInfo.MultiPGMode||m_ChartInfo.MultiValMode) {
				//if(m_ChartInfo.MultiPGMode&&itype!= DATA_TYPES::SIM_VAL)
			//		m_pSimDoc->m_PlotStorage.SetPGRepitNo(j, m_pProp->m_MultiSequence);
				//else 
				if (m_ChartInfo.MultiValMode) {
					size_t MaxNumberOfRepit = 1;
					if (m_pSimDoc->MR_GetValidIndex_IndexInValFile_InDim1() != string::npos) {
						MaxNumberOfRepit = m_pSimDoc->m_MultiRun_Array[0].NumberOfRepeatitionsWithinDimension;
					}

					m_pSimDoc->m_PlotStorage.SetValRunNo(j, m_pProp->m_MultiSequence);
					//m_pSimDoc->m_IsValIndexUpdated
					if (MaxNumberOfRepit > 1) {
						auto repitseq = m_pProp->m_MultiSequence % MaxNumberOfRepit+1;
						m_pSimDoc->m_PlotStorage.SetPGRepitNo(j, repitseq);

					}					
				}
				valid = false;

			}
			else
				il--;
		}


	}

	else {
				int j=m_pSimDoc->m_PlotStorage.NumSelected()-1;
				auto datatype=m_pSimDoc->m_PlotStorage.GetDataType(j);
				ichart=m_pSimDoc->m_PlotStorage.GetChart(j);
				icharttype=m_pSimDoc->m_PlotStorage.GetChartType(j);
				ilocal=m_pSimDoc->m_PlotStorage.GetLocalIndex(j);
				int iadd=0;
				if(datatype==DATA_TYPES::ANYDATA) 
					iadd=0;
				else if(datatype>DATA_TYPES::ANYDATA&&datatype<DATA_TYPES::SIM_PERFORMANCE)
					if(ilocal>=0)
						iadd=0;
					else
						iadd=1;
				else if(datatype== DATA_TYPES::SIM_PERFORMANCE)
					iadd=1;
				else if(datatype== DATA_TYPES::SIM_OUTSUM)
					iadd=6;
				else if(itype== DATA_TYPES::SIM_PAR)
					iadd=1;
				else if(itype== DATA_TYPES::SIM_SUMVAR)
					iadd=3;
								
				if(itype== DATA_TYPES::SIM_PAR) {
						m_pProp->SetDistScale(m_pSimDoc->MR_GetMin(ilocal+iadd), m_pSimDoc->MR_GetMax(ilocal+iadd), m_pSimDoc->MR_GetLogDist(ilocal+iadd));
				}
				if(!m_pSimDoc->m_PlotStorage.SetLocalIndex(j,ilocal+iadd))
								return false;
	
	}
	if(m_pSimDoc->MR_Get_TotalNumberofRuns()>1)
		return true;
	else
		return false;
}
bool TimeSeriePlot::OnInit(CChartViewer *Viewer)
{

	 //
    // Initialize member variables
    //
	m_pUtil->m_FileSave.m_NoChange=false;
    m_pProp->m_CommonScale.LeftMin = m_pProp->m_CommonScale.LeftMax = 0;   	// y axes ranges
	m_pProp->m_TimeScale.min = m_pProp->m_TimeScale.max =0;   // x axex ranges


	string str;
	//if(m_pCP==nullptr)
		str="CoupModel Chart :"+FUtil::STD_ItoAscii(m_pProp->m_ChartNumber)+", Run No :"+FUtil::STD_ItoAscii(m_pSimDoc->m_DocFile.m_SimulationRunNo)+" "+m_pSimDoc->m_DocFile2.m_Comments.c_str();
	//SetWindowText(nullptr, (LPCSTR)str.c_str());

    //
    // In this demo, we deduce the horizontal scroll range from the data.
    //
	if(m_pProp->IsFirstIndepSet()) {
		m_pProp->m_TimeScale.min=m_Time.MinDate;
		m_pProp->m_TimeScale.max=m_Time.MaxDate;

		return true;
	}
	if(m_pProp->m_ChartType ==CHART_TYPES::SCATTER) {
		 /*if(m_ChartInfo.NumVariables<2&&!m_pProp->IsMultiPGFilePlot()) {
			MFC_Util::MessageBox("Select a minimum of 2 variables !");
			 return false;
		  }*/

    // Earliest date is the first timestamp
		 m_Time.MinDate = AA[0][0];
    // Duration is the seconds elapsed of the last timestamp from the earliest date
		 m_Time.DateRange = AA[0][m_ChartInfo.NumOfPoints-1] - m_Time.MinDate;

	  //m_StartDate.SetRange(&startDate, &endDate);

	}
	else if(!m_pUtil->m_MultiRunPlot&&m_Zoom.ZoomLevel<=1) {
    // Earliest date is the first timestamp
		if(m_pProp->m_timeStamps.len>0) {
			m_Time.MinDate = m_pProp->m_timeStamps[0];
		// Duration is the seconds elapsed of the last timestamp from the earliest date
			m_Time.DateRange = m_pProp->m_timeStamps[m_pProp->m_timeStamps.len - 1] - m_Time.MinDate;
		}
		else
			return false;
/*
		// Convert the start time (in chartTime format) as an MFC CTime value
		int startYMD = Chart::getChartYMD(m_timeStamps[0]);
		int startHMS = (int)fmod(m_timeStamps[0], 86400);
		CTime startDate = CTime(startYMD / 10000, (startYMD % 10000) / 100, startYMD % 100,
			startHMS / 3600, (startHMS % 3600) / 60, startHMS % 60);

		// Convert the end time (in chartTime format) as an MFC CTime value
		int endYMD = Chart::getChartYMD(m_timeStamps[m_timeStamps.len - 1]);
		int endHMS = (int)fmod(m_timeStamps[m_timeStamps.len - 1], 86400);
		CTime endDate = CTime(endYMD / 10000, (endYMD % 10000) / 100, endYMD % 100,
			endHMS / 3600, (endHMS % 3600) / 60, endHMS % 60);
	    
		// Set the startDate and endDate to the CDateTimeCtrl control 
*/
		 m_Time.MinDuration = 600;
	}
	else if(m_Zoom.ZoomLevel==1) {
		
			m_Time.MinDuration = 1;
			if(m_pProp->m_timeStamps.len>0) {

				m_Time.MinDate = m_pProp->m_timeStamps[0];
					// Duration is the seconds elapsed of the last timestamp from the earliest date
				m_Time.DateRange = m_pProp->m_timeStamps[m_pProp->m_timeStamps.len - 1] - m_Time.MinDate;
				if(m_Time.MaxDate>m_pProp->m_timeStamps[m_pProp->m_timeStamps.len - 1]) {
					m_Time.CurrentDuration = m_Time.MaxDate - m_Time.MinDate;
					m_Time.MinDate=m_Time.MinDate;
					m_Time.DateRange=m_Time.CurrentDuration;
				}
			}
			else
				return false;
	}
	
	if(m_Zoom.ZoomLevel==1) {
    // Set up the ChartViewer to reflect the visible and minimum duration
		Viewer->setZoomInWidthLimit(m_Time.MinDuration / m_Time.DateRange);
		Viewer->setViewPortWidth(m_Time.CurrentDuration / m_Time.DateRange);
		Viewer->setViewPortLeft(1 - Viewer->getViewPortWidth());

	}
	else if(m_Zoom.ZoomLevel>1) {
		double start=double(m_Zoom.Hpos-1)/double(m_Zoom.ZoomLevel);
		Viewer->setViewPortLeft(start);
	}

    // Initially set the mouse to drag to scroll mode in horizontal direction.
   // m_PointerPB.SetCheck(1);
   // m_XZoomPB.SetCheck(1);
   // Viewer->setMouseUsage(Chart::MouseUsageScroll);

    // Can update chart now
    Viewer->updateViewPort(true, true);
    return true;
}
void*  TimeSeriePlot::CreateXYChart() 
{
	
	int ytop=m_pProp->GetTopMargin();
	int ybottom=m_pProp->GetBottomMargin();;
	m_ChartSize.ydiff=int(double(m_pProp->GetGapMargin())*(1.-m_pUtil->GetGapHorFraction()));
	m_ChartSize.xdiff=int(float(m_pProp->GetGapMargin())*m_pUtil->GetGapHorFraction());
	m_ChartSize.xdiff1=int(float(m_ChartSize.xdiff)*m_pUtil->GetGapHorLeftFraction());
	m_ChartSize.xdiff2=m_ChartSize.xdiff-m_ChartSize.xdiff1;
	m_ChartSize.ydiff1=int(float(m_ChartSize.ydiff)*m_pUtil->GetGapVertTopFraction());
	m_ChartSize.ydiff2=m_ChartSize.ydiff-m_ChartSize.ydiff1;

	int xleft=m_pProp->GetLeftMargin();
	int xright=m_pProp->GetRightMargin();

	bool vertical_legends;
	
	vertical_legends=m_pProp->GetVerticalLegends();
	//XYChart *pXY_Chart;
	bool createlegend;

	int legendposX, legendposY;
	legendposX=m_pUtil->GetLegendXPos();
	legendposY=m_pUtil->GetLegendYPos();
	createlegend=m_pUtil->MakeLegends();

	if(m_ChartSize.yrow==0&&m_ChartSize.xcol==0) {
			if(m_pProp->m_NumMultiChartX==1&&m_pProp->m_NumMultiChartY==1) {
				pXY_Chart = new  XYChart(m_ChartSize.xplotwidth+xleft+xright, m_ChartSize.yplotheight+ytop+ybottom, m_pUtil->m_ColorBackground);
				m_pProp->m_MakeChartTitle=true;
			}
			else if(m_pProp->m_NumMultiChartY==1) {
				pXY_Chart = new  XYChart(m_ChartSize.xplotwidth+xleft+m_ChartSize.xdiff2, m_ChartSize.yplotheight+ytop+ybottom, m_pUtil->m_ColorBackground);
				m_pProp->m_MakeChartTitle=true;
			}
			else if(m_pProp->m_NumMultiChartX==1){
				pXY_Chart = new  XYChart(m_ChartSize.xplotwidth+xleft+xright, m_ChartSize.yplotheight+ytop+m_ChartSize.ydiff2, m_pUtil->m_ColorBackground);
				m_pProp->m_NoXAxisScale=true;
				m_pProp->m_MakeChartTitle=true;
			}
			else {
				pXY_Chart = new  XYChart(m_ChartSize.xplotwidth+xleft+m_ChartSize.xdiff, m_ChartSize.yplotheight+ytop+m_ChartSize.ydiff2, m_pUtil->m_ColorBackground);
				m_pProp->m_NoXAxisScale=true;
			}
			pXY_Chart->setPlotArea(xleft, ytop, m_ChartSize.xplotwidth, m_ChartSize.yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend||m_pUtil->MakeSingleLegends())
				pXY_Chart->addLegend(legendposX+xleft, legendposY+ytop,vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);
		}
		else if(m_ChartSize.yrow==0&&m_ChartSize.xcol==m_pProp->m_NumMultiChartX-1) {
			if(m_pProp->m_NumMultiChartY==1)
				pXY_Chart = new  XYChart(m_ChartSize.xplotwidth+xright+m_ChartSize.xdiff1, m_ChartSize.yplotheight+ytop+ybottom, m_pUtil->m_ColorBackground);
			else {
				pXY_Chart = new  XYChart(m_ChartSize.xplotwidth+xright+m_ChartSize.xdiff1, m_ChartSize.yplotheight+ytop+m_ChartSize.ydiff2, m_pUtil->m_ColorBackground);
				m_pProp->m_NoXAxisScale=true;
			}
			m_pProp->m_MakeChartTitle=true;
			m_pProp->m_NoYAxisScale=true;
			pXY_Chart->setPlotArea(m_ChartSize.xdiff1, ytop, m_ChartSize.xplotwidth , m_ChartSize.yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend)
				pXY_Chart->addLegend(legendposX+m_ChartSize.xdiff, legendposY+ytop, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);
		}
		else if(m_ChartSize.yrow==0) {
			if(m_pProp->m_NumMultiChartY==1)
				pXY_Chart = new  XYChart(m_ChartSize.xplotwidth+m_ChartSize.xdiff, m_ChartSize.yplotheight+ytop+ybottom, m_pUtil->m_ColorBackground);
			else {
				pXY_Chart = new  XYChart(m_ChartSize.xplotwidth+m_ChartSize.xdiff, m_ChartSize.yplotheight+ytop+m_ChartSize.ydiff2, m_pUtil->m_ColorBackground);
				m_pProp->m_NoXAxisScale=true;
			}
			if(m_ChartSize.xcol==m_pProp->m_NumMultiChartX/2)
				m_pProp->m_MakeChartTitle=true;
			m_pProp->m_NoYAxisScale=true;
			pXY_Chart->setPlotArea(m_ChartSize.xdiff1, ytop, m_ChartSize.xplotwidth , m_ChartSize.yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend)
				pXY_Chart->addLegend(legendposX+m_ChartSize.xdiff1, legendposY+ytop, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);
		}
		else if(m_ChartSize.yrow==m_pProp->m_NumMultiChartY-1&&m_ChartSize.xcol==0) {
			if(m_pProp->m_NumMultiChartX==1)
				pXY_Chart = new  XYChart(m_ChartSize.xplotwidth+xleft+xright, m_ChartSize.yplotheight+ybottom+m_ChartSize.ydiff1, m_pUtil->m_ColorBackground);
			else 
				pXY_Chart = new  XYChart(m_ChartSize.xplotwidth+xleft+m_ChartSize.xdiff, m_ChartSize.yplotheight+ybottom+m_ChartSize.ydiff1, m_pUtil->m_ColorBackground);
			pXY_Chart->setPlotArea(xleft, m_ChartSize.ydiff1, m_ChartSize.xplotwidth, m_ChartSize.yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend)
				pXY_Chart->addLegend(legendposX+xleft, legendposY+m_ChartSize.ydiff1, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);

		}
		else if(m_ChartSize.yrow==m_pProp->m_NumMultiChartY-1&&m_ChartSize.xcol==m_pProp->m_NumMultiChartX-1) {
			pXY_Chart = new  XYChart(m_ChartSize.xplotwidth+xright+m_ChartSize.xdiff1, m_ChartSize.yplotheight+ybottom+m_ChartSize.ydiff1, m_pUtil->m_ColorBackground);
			m_pProp->m_NoYAxisScale=true;
			pXY_Chart->setPlotArea(m_ChartSize.xdiff1, m_ChartSize.ydiff1,m_ChartSize.xplotwidth, m_ChartSize.yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend)
				pXY_Chart->addLegend(legendposX+m_ChartSize.xdiff1, legendposY+m_ChartSize.ydiff1, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);

		}
		else if(m_ChartSize.yrow==m_pProp->m_NumMultiChartY-1) {
			pXY_Chart = new  XYChart(m_ChartSize.xplotwidth+m_ChartSize.xdiff, m_ChartSize.yplotheight+ybottom+m_ChartSize.ydiff, m_pUtil->m_ColorBackground);
			m_pProp->m_NoYAxisScale=true;
			pXY_Chart->setPlotArea(m_ChartSize.xdiff1, m_ChartSize.ydiff1,m_ChartSize.xplotwidth , m_ChartSize.yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend)
				pXY_Chart->addLegend(legendposX+m_ChartSize.xdiff1, legendposY+m_ChartSize.ydiff1, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground,m_pUtil->m_ColorLegendBackgroundFrame);

		}
		else if(m_ChartSize.xcol==0) {
			pXY_Chart = new  XYChart(m_ChartSize.xplotwidth+xleft+m_ChartSize.xdiff2, m_ChartSize.yplotheight+m_ChartSize.ydiff, m_pUtil->m_ColorBackground);
			m_pProp->m_NoXAxisScale=true;
			pXY_Chart->setPlotArea(xleft, m_ChartSize.ydiff1,m_ChartSize.xplotwidth , m_ChartSize.yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend)
				pXY_Chart->addLegend(legendposX+xleft, legendposY+m_ChartSize.ydiff1, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);

		}
		else if(m_ChartSize.xcol==m_pProp->m_NumMultiChartX-1) {
			pXY_Chart = new  XYChart(m_ChartSize.xplotwidth+xright+m_ChartSize.xdiff1, m_ChartSize.yplotheight+m_ChartSize.ydiff, m_pUtil->m_ColorBackground);
			m_pProp->m_NoXAxisScale=true;
			m_pProp->m_NoYAxisScale=true;
			pXY_Chart->setPlotArea(m_ChartSize.xdiff1, m_ChartSize.ydiff1, m_ChartSize.xplotwidth, m_ChartSize.yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend)
				pXY_Chart->addLegend(legendposX+m_ChartSize.xdiff1, legendposY+m_ChartSize.ydiff1, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);

		}
		else {
			pXY_Chart = new  XYChart(m_ChartSize.xplotwidth+m_ChartSize.xdiff, m_ChartSize.yplotheight+m_ChartSize.ydiff, m_pUtil->m_ColorBackground);
			m_pProp->m_NoXAxisScale=true;
			m_pProp->m_NoYAxisScale=true;
			pXY_Chart->setPlotArea(m_ChartSize.xdiff1, m_ChartSize.ydiff1,m_ChartSize.xplotwidth , m_ChartSize.yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend)
				pXY_Chart->addLegend(legendposX+m_ChartSize.xdiff1, legendposY+m_ChartSize.ydiff1, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);

		}

		
	return pXY_Chart; 

}

void TimeSeriePlot::UpdateGeneralProp()
{

    // Note that if you only have a small amount of data (a few hundred data points), it
    // may be easier to just plot all data in any case (so the following copying code is 
    // not needed), and let ChartDirector "clip" the chart to the plot area. 
    //

	double  IntervLeft, IntervRight, IntervX;

	CWinApp* pApp = AfxGetApp();

	

	m_pProp->m_rightaxis=false;
	m_Layers.BandLayerExist=false;

	m_pUtil->m_ColorBackground=MFC_Util::GetProfileIntNo( "HColorBackground", m_pUtil->m_ColorBackground);
	m_pUtil->m_ColorLegendBackground=MFC_Util::GetProfileIntNo( "HColorLegendBackground", m_pUtil->m_ColorLegendBackground);
	m_pUtil->m_ColorLegendBackgroundFrame=MFC_Util::GetProfileIntNo( "HColorLegendBackgroundFrame", m_pUtil->m_ColorLegendBackgroundFrame);

	m_pUtil->m_TitleSizePoint=MFC_Util::GetProfileIntNo( "TitleSize", 12);
	m_pUtil->m_LabelSizePoint=MFC_Util::GetProfileIntNo( "LabelSize", 9);
	m_pUtil->m_AxisScalePoint=MFC_Util::GetProfileIntNo( "AxisScaleSize", 9);
	m_pUtil->m_LegendSizePoint=MFC_Util::GetProfileIntNo( "LegendSize", 9);
	m_pUtil->m_LineWidthPoint=MFC_Util::GetProfileIntNo( "LineWidth", 2);
	m_pUtil->m_LineAxisWidthPoint=MFC_Util::GetProfileIntNo( "LineAxisWidth", 2);
	m_pUtil->m_SymbolSizePoint=MFC_Util::GetProfileIntNo( "SymbolSize", 9);
	m_pProp->m_ChartError=MFC_Util::GetProfileIntNo( "ChartError", m_pProp->m_ChartError);
	m_pUtil->m_ErrorSignal=MFC_Util::GetProfileIntNo( "ErrorSignal", m_pUtil->m_ErrorSignal);
	m_pUtil->m_ResidualPlot=MFC_Util::GetProfileIntNo( "ResidualPlot", m_pUtil->m_ResidualPlot);

	int	visible_legends;
	visible_legends=m_pProp->GetVisibleFrames();

	if(visible_legends==0) {
		m_pUtil->m_ColorLegendBackground=Chart::Transparent;
		m_pUtil->m_ColorLegendBackgroundFrame=Chart::Transparent;	
	}
	else if(visible_legends<4) {
		int dim=(4-visible_legends)*80*0x1000000;
		m_pUtil->m_ColorLegendBackground+=dim;
		m_pUtil->m_ColorLegendBackgroundFrame+=dim;	
	}

}
bool TimeSeriePlot::UpdateViewPortTime()
{

	if((m_pProp->m_ChartType== CHART_TYPES::TIME_SERIE||(m_pProp->m_ChartType== CHART_TYPES::CONTOUR_CHART))&&m_pProp->GetFirstIndepIndex()==string::npos/*&&!m_pProp->IsFirstIndepSet()*/) {  //Time Serie 
	//	if(m_pProp->m_XScaleChanged.size()<=0) 
	//		return false;

		if(m_pProp->isXScaleChanged(0)) {
			if(m_pProp->m_ReShape.m_SortedSeries||m_pProp->m_ReShape.m_NormalDist) {
				m_Zoom.viewPortStartDate = m_Time.MinDate + (__int64)(m_pChartViewer->getViewPortLeft() * 
					m_Time.DateRange + 0.5);
				m_Zoom.viewPortEndDate = m_Zoom.viewPortStartDate + (__int64)(m_pChartViewer->getViewPortWidth() * 
					m_Time.DateRange + 0.5);
			}
			else {
				m_Zoom.viewPortStartDate=m_pProp->m_TimeScale.min;
				m_Zoom.viewPortEndDate=m_pProp->m_TimeScale.max;
			}

		}
		else {
		// Using ViewPortLeft and ViewPortWidth, get the start and end dates of the view port.
			m_Zoom.viewPortStartDate = m_Time.MinDate + (__int64)(m_pChartViewer->getViewPortLeft() * 
				m_Time.DateRange + 0.5);
			string datum;
			datum=PGUtil::StringDatumSec(m_Zoom.viewPortStartDate);
			m_Zoom.viewPortEndDate = m_Zoom.viewPortStartDate + (__int64)(m_pChartViewer->getViewPortWidth() * 
				m_Time.DateRange + 0.5);
			m_Agg.StartIndex = std::lower_bound(m_pProp->m_timeStamps.data, m_pProp->m_timeStamps.data + m_pProp->m_timeStamps.len,
			m_Zoom.viewPortStartDate) - m_pProp->m_timeStamps.data;
			datum=PGUtil::StringDatumSec(m_Zoom.viewPortEndDate);
			m_Agg.EndIndex = std::upper_bound(m_pProp->m_timeStamps.data, m_pProp->m_timeStamps.data + m_pProp->m_timeStamps.len, 
			m_Zoom.viewPortEndDate) - m_pProp->m_timeStamps.data;

		}



	    double koll;
		koll=m_Time.DateRange;
		// Get the starting index of the array using the start date
		m_Agg.StartIndex = std::lower_bound(m_pProp->m_timeStamps.data, m_pProp->m_timeStamps.data + m_pProp->m_timeStamps.len,
			m_Zoom.viewPortStartDate) - m_pProp->m_timeStamps.data;
		//if ((m_Agg.StartIndex > 0) && (m_timeStamps[m_Agg.StartIndex] != m_Zoom.viewPortStartDate)) 
		//	m_Agg.StartIndex=0;
		if (m_Agg.StartIndex == m_Agg.EndIndex) m_Agg.StartIndex = 0;
		// Get the ending index of the array using the end date
		m_Agg.EndIndex = std::upper_bound(m_pProp->m_timeStamps.data, m_pProp->m_timeStamps.data + m_pProp->m_timeStamps.len, 
			m_Zoom.viewPortEndDate) - m_pProp->m_timeStamps.data;
		if(m_Agg.AggregateRatio>1) {
			long adjuststart;
			adjuststart=int(m_Agg.StartIndex/m_Agg.AggregateRatio)*m_Agg.AggregateRatio;
			adjuststart=m_Agg.StartIndex-adjuststart;
			m_Agg.StartIndex-=adjuststart;
			m_Agg.EndIndex-=adjuststart;
		}
		// Get the length
		if(m_pProp->m_ReShape.m_CommonWithDay)  {
			m_ChartInfo.NumOfPoints=m_pProp->m_ReShape.NumOfCommonPoints;
		}
		else
			m_ChartInfo.NumOfPoints = m_Agg.EndIndex - m_Agg.StartIndex;

	}
	else if(m_pProp->m_ChartType== CHART_TYPES::SCATTER||m_pProp->GetFirstIndepIndex()!=string::npos||m_pProp->IsFirstIndepSet()) {  // Scatter plot
		m_ChartInfo.NumOfPoints=m_ChartInfo.NumOfPoints;
		m_Agg.StartIndex=0;
		m_Agg.EndIndex=m_ChartInfo.NumOfPoints-1;
	}
	else if(m_pProp->m_ChartType== CHART_TYPES::FREQUENCE ||m_pProp->m_ChartType== CHART_TYPES::CUM_FREQUENCE) {  // Frequence distributions
	/*	if(m_ChartInfo.NumOfPoints>1000)
			m_ChartInfo.NumOfPoints=101;
		else if(m_ChartInfo.NumOfPoints>200)
			m_ChartInfo.NumOfPoints=21;
		else if(m_ChartInfo.NumOfPoints>100)
			m_ChartInfo.NumOfPoints=11;
		else
			m_ChartInfo.NumOfPoints=6;*/

		m_Agg.StartIndex=0;
		m_Agg.EndIndex=m_ChartInfo.NumOfPoints-1;
	}
	else
		return false;

	if(m_pProp->viewPortTimeStamps!=nullptr) delete [] m_pProp->viewPortTimeStamps;
	if(m_pProp->viewPortTimeStamps2!=nullptr) delete [] m_pProp->viewPortTimeStamps2;
	m_pProp->viewPortTimeStamps = new double[m_ChartInfo.NumOfPoints];
	m_pProp->viewPortTimeStamps2 = new double[m_ChartInfo.NumOfPoints];

	m_Agg.ArraySizeInBytes = m_ChartInfo.NumOfPoints * sizeof(double);
	if (m_pProp->m_ChartType == CHART_TYPES::TIME_SERIE || m_pProp->m_ChartType == CHART_TYPES::CONTOUR_CHART || m_pProp->m_ChartType == CHART_TYPES::SCATTER) {
		if (m_Agg.StartIndex >= TT.size()) m_Agg.StartIndex = 0;
		memcpy(m_pProp->viewPortTimeStamps, &TT[m_Agg.StartIndex], m_Agg.ArraySizeInBytes);
		m_pProp->m_TimeScale.MinX = TT[m_Agg.StartIndex];
		m_pProp->m_TimeScale.MaxX = TT[m_Agg.StartIndex+ m_ChartInfo.NumOfPoints-1];
		if (m_pProp->m_ChartType == CHART_TYPES::CONTOUR_CHART) {
			if(TT2.size()>m_Agg.StartIndex) memcpy(m_pProp->viewPortTimeStamps2, &TT2[m_Agg.StartIndex], m_Agg.ArraySizeInBytes);

		}
	}

	return true;

}
void TimeSeriePlot::UpdateDataDist()
{

	if(!m_pProp->isXDistSet(m_ChartInfo.NumVariables-1)) {
		
			vector<size_t> IndexSub;
			
			for(size_t i=0;i<m_ChartInfo.NumVariables;i++) {
					double xmin=1.E38, xmax=-1E38;

					for(int j=0;j<m_ChartInfo.NumOfPoints;j++) {

							if(m_dataSeriesA[i][j]>xmax&&m_dataSeriesA[i][j]!=Chart::NoValue)
									xmax=m_dataSeriesA[i][j];
							if(m_dataSeriesA[i][j]<xmin&&m_dataSeriesA[i][j]!=Chart::NoValue) 
									xmin=m_dataSeriesA[i][j];
					}
					m_pProp->m_TimeScale.MinX=xmin;
					m_pProp->m_TimeScale.MaxX=xmax;
					Frequence_Set_MinMaxNumberOfStepsDistType(i);

			}
			if(!m_pProp->IsSecondIndepSet()) {
				for (m_ichart=0; m_ichart<m_pProp->m_NumMultiChart;m_ichart++) {
					double xmin=1.E38, xmax=-1E38;
					IndexSub.clear();

					for(int i=0;i<m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);i++) {
						int j=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, i);
						int isubchart0;
						int isubchart=m_pSimDoc->m_PlotStorage.GetSubChart(j);
						if(isubchart<=0)
							isubchart0=0;
						else
							isubchart0=isubchart-1;
						if(m_ichart==isubchart0) {
							IndexSub.push_back(i);
							if(xmin>m_pProp->GetXDistMin(i))
								xmin=m_pProp->GetXDistMin(i);	
							if(xmax<m_pProp->GetXDistMax(i))
								xmax=m_pProp->GetXDistMax(i);	
	
						}
					}
					for(int i=0;i<IndexSub.size();i++) {
						int ind=IndexSub.at(i);
					//	m_pProp->SetXDistMin(ind, xmin);
					//	m_pProp->SetXDistMax(ind, xmax);
					}
				}
			}
			m_pChartCug->OnUpdate();
	}

}
void TimeSeriePlot::CreateViewPortData(size_t TotNumPrior)
{
	if (m_ChartInfo.NumOfPoints == 0) return;

	size_t act_chartno=string::npos;
	size_t start_varindex= string::npos;
	size_t max_numvar=0;

	int j = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, 0 % TotNumPrior);
	auto datatype = m_pSimDoc->m_PlotStorage.GetDataType(j);
	bool commonx_scale = false;
	if (datatype == DATA_TYPES::SIM_PERFORMANCE&&(m_pProp->m_ChartType == CHART_TYPES::FREQUENCE || m_pProp->m_ChartType == CHART_TYPES::CUM_FREQUENCE)) {
		commonx_scale = true;
		m_pProp->m_TimeScale.MaxX = m_pProp->GetCommonXDistMax();
		m_pProp->m_TimeScale.MinX = m_pProp->GetCommonXDistMin();
		m_pProp->SetDistScale(m_pProp->m_TimeScale.MinX, m_pProp->m_TimeScale.MaxX, false);
	}





	for(size_t i=0;i<m_ChartInfo.NumVariables;i++) {
	
		int j=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, i%TotNumPrior);
		auto datatype=m_pSimDoc->m_PlotStorage.GetDataType(j);
		PLOTv p_info = m_pSimDoc->m_PlotStorage.GetPlotInfo(j);
		
		if(m_pProp->m_ChartType== CHART_TYPES::CONTOUR_CHART||m_pProp->IsSecondIndepSet()) {

			
			
			if(act_chartno!=p_info.SubChart_No) {
				act_chartno = p_info.SubChart_No;
				if (start_varindex == string::npos) start_varindex = 0;
				size_t numvar = 1;
				PLOTv p_info2;
				for (size_t icount = i + 1; icount < m_ChartInfo.NumVariables; icount++) {
					int jj = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, icount % TotNumPrior);
					p_info2 = m_pSimDoc->m_PlotStorage.GetPlotInfo(jj);
					if (p_info2.SubChart_No == act_chartno) numvar++;
				}
				
				if(m_pProp->viewPortDataSeries[act_chartno-1]!=nullptr) delete [] m_pProp->viewPortDataSeries[act_chartno-1];
				m_pProp->viewPortDataSeries[act_chartno-1] = new double[m_ChartInfo.NumOfPoints*numvar];

			//	for(size_t i=1; i<m_ChartInfo.NumVariables; i++)
					//for(size_t ii=0;ii<m_ChartInfo.NumOfPoints; ii++) m_dataSeriesA[0][i*m_ChartInfo.NumOfPoints+ii]=m_dataSeriesA[i][ii];
			//		memcpy(m_pProp->viewPortDataSeries[0], &AA[0][0], m_Agg.ArraySizeInBytes*m_ChartInfo.NumVariables);
				

				vector<double> NewZ;
				
				NewZ.resize(m_ChartInfo.NumOfPoints*numvar);
				for (int jj = 0; jj<numvar; jj++) {
					for (int pp = 0; pp < m_ChartInfo.NumOfPoints; pp++) {
						NewZ[pp + jj*m_ChartInfo.NumOfPoints] = AA[start_varindex+jj][m_Agg.StartIndex + pp];
					}
				}
				memcpy(m_pProp->viewPortDataSeries[act_chartno-1], &NewZ[0], m_ChartInfo.NumOfPoints*numvar*sizeof(double));
				//Adjusted for start position but not for any aggregation because of two many points..
				//m_dataSeriesA[0] = DoubleArray(m_pProp->viewPortDataSeries[0], m_ChartInfo.NumOfPoints*(m_ChartInfo.NumVariables));

			/*else {
				if (m_pProp->viewPortDataSeries[i] != nullptr) delete[] m_pProp->viewPortDataSeries[i];
				m_pProp->viewPortDataSeries[i] = new double[m_ChartInfo.NumOfPoints];
				memcpy(m_pProp->viewPortDataSeries[i], &AA[i][0], m_Agg.ArraySizeInBytes);

			} */

				start_varindex += numvar;
				if (numvar > max_numvar) max_numvar = numvar;
			}


			m_EstimateNewDist=false;
		}
		else if(m_pProp->m_ChartType!= CHART_TYPES::CONTOUR_CHART) {


			if(m_pProp->viewPortDataSeries[i]!=nullptr) delete [] m_pProp->viewPortDataSeries[i];
			m_pProp->viewPortDataSeries[i] = new double[m_ChartInfo.NumOfPoints];
			if((m_pProp->m_ChartType== CHART_TYPES::FREQUENCE||m_pProp->m_ChartType== CHART_TYPES::CUM_FREQUENCE)&&m_EstimateNewDist) {
					// Get Distribution of data Array

				if(j>=0&&datatype==DATA_TYPES::SIM_PAR) {
					int ilocal=m_pSimDoc->m_PlotStorage.GetLocalIndex(j);
					m_pProp->SetDistScale(m_pSimDoc->MR_GetMin(ilocal), m_pSimDoc->MR_GetMax(ilocal), m_pSimDoc->MR_GetLogDist(ilocal));
				}
				if (commonx_scale) {



				}
				
				Frequence_SetNumberOfPoint_LogScale(i % TotNumPrior);

					// Calculate X scale
				Frequence_SetTime();
				Frequence_EstimateDataDist(i, TotNumPrior);




			}
			else {  // Copy data Array
				memcpy(m_pProp->viewPortDataSeries[i], m_dataSeriesA[i].data + m_Agg.StartIndex, m_Agg.ArraySizeInBytes);
				m_EstimateNewDist=false;
			}
		}
			
	}
	if ((m_pProp->GetIndepFirst() == PGDAY || m_pProp->GetIndepFirst() == PGYEAR)) {
		if (m_pProp->viewPortTimeStamps != nullptr) delete[] m_pProp->viewPortTimeStamps;

		m_pProp->viewPortTimeStamps = new double[m_ChartInfo.NumOfPoints * m_ChartInfo.NumVariables];
		TT.resize(m_ChartInfo.NumOfPoints * m_ChartInfo.NumVariables);
		for (size_t i = 1; i < m_ChartInfo.NumVariables; i++)
			for (size_t ii = 0; ii < m_ChartInfo.NumOfPoints; ii++) TT[i * m_ChartInfo.NumOfPoints + ii] = TT[(i - 1) * m_ChartInfo.NumOfPoints + ii];
		memcpy(m_pProp->viewPortTimeStamps, &TT[0], m_ChartInfo.NumOfPoints * m_ChartInfo.NumVariables * sizeof(double));
	}
	else if (m_pProp->GetIndepSecond() == PGDEPTH) {
		if (m_pProp->viewPortTimeStamps != nullptr) delete[] m_pProp->viewPortTimeStamps;
		m_pProp->viewPortTimeStamps = new double[m_ChartInfo.NumOfPoints * max_numvar];
		vector<double> NewTimeStamp;

		NewTimeStamp.resize(m_ChartInfo.NumOfPoints * max_numvar);
		for (size_t ii = 0; ii < m_ChartInfo.NumOfPoints; ii++)
			NewTimeStamp[ii] = TT[m_Agg.StartIndex + ii];
		for (size_t i = 1; i < max_numvar; i++)
			for (size_t ii = 0; ii < m_ChartInfo.NumOfPoints; ii++)
				NewTimeStamp[i * m_ChartInfo.NumOfPoints + ii] = NewTimeStamp[(i - 1) * m_ChartInfo.NumOfPoints + ii];
		memcpy(m_pProp->viewPortTimeStamps, &NewTimeStamp[0], m_ChartInfo.NumOfPoints * max_numvar * sizeof(double));


	}



	for(int i=0; i<m_ChartInfo.NumVariableBands;i++) {
		if(m_pProp->viewPortDataSeriesB[i]!=nullptr) delete [] m_pProp->viewPortDataSeriesB[i];
		m_pProp->viewPortDataSeriesB[i] = new double[m_ChartInfo.NumOfPoints];
		memcpy(m_pProp->viewPortDataSeriesB[i], m_dataSeriesB[i].data + m_Agg.StartIndex, m_Agg.ArraySizeInBytes);

		if(m_pProp->viewPortDataSeriesMin[i]!=nullptr) delete [] m_pProp->viewPortDataSeriesMin[i];
		m_pProp->viewPortDataSeriesMin[i] = new double[m_ChartInfo.NumOfPoints];
		memcpy(m_pProp->viewPortDataSeriesMin[i], m_dataSeriesMIN[i].data + m_Agg.StartIndex, m_Agg.ArraySizeInBytes);
		
		
		if(m_pProp->viewPortDataSeriesMax[i]!=nullptr) delete [] m_pProp->viewPortDataSeriesMax[i];		
		m_pProp->viewPortDataSeriesMax[i] = new double[m_ChartInfo.NumOfPoints];
		memcpy(m_pProp->viewPortDataSeriesMax[i], m_dataSeriesMAX[i].data + m_Agg.StartIndex, m_Agg.ArraySizeInBytes);

		if(m_pProp->viewPortDataSeriesP10[i]!=nullptr) delete [] m_pProp->viewPortDataSeriesP10[i];
		m_pProp->viewPortDataSeriesP10[i] = new double[m_ChartInfo.NumOfPoints];
		memcpy(m_pProp->viewPortDataSeriesP10[i], m_dataSeriesP10[i].data + m_Agg.StartIndex, m_Agg.ArraySizeInBytes);

		if(m_pProp->viewPortDataSeriesP50[i]!=nullptr) delete [] m_pProp->viewPortDataSeriesP50[i];
		m_pProp->viewPortDataSeriesP50[i] = new double[m_ChartInfo.NumOfPoints];
		memcpy(m_pProp->viewPortDataSeriesP50[i], m_dataSeriesP50[i].data + m_Agg.StartIndex, m_Agg.ArraySizeInBytes);

		if(m_pProp->viewPortDataSeriesP90[i]!=nullptr) delete [] m_pProp->viewPortDataSeriesP90[i];
		m_pProp->viewPortDataSeriesP90[i] = new double[m_ChartInfo.NumOfPoints];
		memcpy(m_pProp->viewPortDataSeriesP90[i], m_dataSeriesP90[i].data + m_Agg.StartIndex, m_Agg.ArraySizeInBytes);
	}


}
void TimeSeriePlot::AggregateData() {
	

	int Resolution;
	
	Resolution = max(m_ChartSize.xplotwidth, 520);

  if (m_ChartInfo.NumOfPoints >= Resolution&&(m_pProp->m_ChartType== CHART_TYPES::TIME_SERIE/*||m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART*/)&&!m_pProp->m_ReShape.m_NormalDist&&m_pProp->GetFirstIndepIndex()==string::npos&&m_pProp->GetIndepFirst()==DATEAXIS&&m_pProp->m_AggregateTime)
    {
        //
        // Zoomable chart with high zooming ratios often need to plot many thousands of 
        // points when fully zoomed out. However, it is usually not needed to plot more
        // data points than the resolution of the chart. Plotting too many points may cause
        // the points and the lines to overlap. So rather than increasing resolution, this 
        // reduces the clarity of the chart. So it is better to aggregate the data first if
        // there are too many points.
        //
        // In our current example, the chart only has 520 pixels in width and is using a 2
        // pixel line width. So if there are more than 520 data points, we aggregate the 
        // data using the ChartDirector aggregation utility method.
        //
        // If in your real application, you do not have too many data points, you may 
        // remove the following code altogether.
        //

        // Set up an aggregator to aggregate the data based on regular sized slots
        ArrayMath m(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfPoints));
		
        m.selectRegularSpacing(m_ChartInfo.NumOfPoints / Resolution);
        
        // For the timestamps, take the first timestamp on each slot
        int aggregatedNoOfPoints = m.aggregate(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfPoints), 
            Chart::AggregateFirst).len;

		for(int i=0;i<m_ChartInfo.NumVariables;i++) {
        // For the data values, aggregate by taking the averages
			
			ArrayMath buffer(DoubleArray(m_pProp->viewPortDataSeries[i],  m_ChartInfo.NumOfPoints));
			DoubleArray maxValues = m.aggregate(buffer.result(), Chart::AggregateMax);
			m.aggregate(DoubleArray(m_pProp->viewPortDataSeries[i], m_ChartInfo.NumOfPoints), Chart::AggregateAvg);

			 for (int j = 0; j < maxValues.len; ++j)
				if (maxValues[j] < m_pProp->viewPortDataSeries[i][j])  m_pProp->viewPortDataSeries[i][j]= Chart::NoValue;


		}

		for(int i=0; i<m_ChartInfo.NumVariableBands;i++) {
				ArrayMath buffer(DoubleArray(m_pProp->viewPortDataSeriesB[i],  m_ChartInfo.NumOfPoints));
				DoubleArray maxValues = m.aggregate(buffer.result(), Chart::AggregateMax);
				m.aggregate(DoubleArray(m_pProp->viewPortDataSeriesB[i], m_ChartInfo.NumOfPoints), Chart::AggregateAvg);
				for (int j = 0; j < maxValues.len; ++j)
					if (maxValues[j] < m_pProp->viewPortDataSeriesB[i][j])  m_pProp->viewPortDataSeriesB[i][j]= Chart::NoValue;

				ArrayMath buffer2(DoubleArray(m_pProp->viewPortDataSeriesMin[i],  m_ChartInfo.NumOfPoints));
				maxValues = m.aggregate(buffer2.result(), Chart::AggregateMax);
				m.aggregate(DoubleArray(m_pProp->viewPortDataSeriesMin[i], m_ChartInfo.NumOfPoints), Chart::AggregateAvg);
				for (int j = 0; j < maxValues.len; ++j)
					if (maxValues[j] < m_pProp->viewPortDataSeriesMin[i][j])  m_pProp->viewPortDataSeriesMin[i][j]= Chart::NoValue;

				ArrayMath buffer3(DoubleArray(m_pProp->viewPortDataSeriesMax[i],  m_ChartInfo.NumOfPoints));
				maxValues = m.aggregate(buffer3.result(), Chart::AggregateMax);
				m.aggregate(DoubleArray(m_pProp->viewPortDataSeriesMax[i], m_ChartInfo.NumOfPoints), Chart::AggregateAvg);
				for (int j = 0; j < maxValues.len; ++j)
					if (maxValues[j] < m_pProp->viewPortDataSeriesMax[i][j])  m_pProp->viewPortDataSeriesMax[i][j]= Chart::NoValue;

				ArrayMath buffer4(DoubleArray(m_pProp->viewPortDataSeriesP10[i],  m_ChartInfo.NumOfPoints));
				maxValues = m.aggregate(buffer4.result(), Chart::AggregateMax);
				m.aggregate(DoubleArray(m_pProp->viewPortDataSeriesP10[i], m_ChartInfo.NumOfPoints), Chart::AggregateAvg);
				for (int j = 0; j < maxValues.len; ++j)
					if (maxValues[j] < m_pProp->viewPortDataSeriesP10[i][j])  m_pProp->viewPortDataSeriesP10[i][j]= Chart::NoValue;
				
				ArrayMath buffer5(DoubleArray(m_pProp->viewPortDataSeriesP50[i],  m_ChartInfo.NumOfPoints));
				maxValues = m.aggregate(buffer5.result(), Chart::AggregateMax);
				m.aggregate(DoubleArray(m_pProp->viewPortDataSeriesP50[i], m_ChartInfo.NumOfPoints), Chart::AggregateAvg);
				for (int j = 0; j < maxValues.len; ++j)
					if (maxValues[j] < m_pProp->viewPortDataSeriesP50[i][j])  m_pProp->viewPortDataSeriesP50[i][j]= Chart::NoValue;
				
				ArrayMath buffer6(DoubleArray(m_pProp->viewPortDataSeriesMax[i],  m_ChartInfo.NumOfPoints));
				maxValues = m.aggregate(buffer6.result(), Chart::AggregateMax);
				m.aggregate(DoubleArray(m_pProp->viewPortDataSeriesP90[i], m_ChartInfo.NumOfPoints), Chart::AggregateAvg);
				for (int j = 0; j < maxValues.len; ++j)
					if (maxValues[j] < m_pProp->viewPortDataSeriesP90[i][j])  m_pProp->viewPortDataSeriesP90[i][j]= Chart::NoValue;

		}
		m_Agg.AggregateRatio=m_ChartInfo.NumOfPoints/(aggregatedNoOfPoints-1);
        m_ChartInfo.NumOfPoints = aggregatedNoOfPoints-1;



		
		


    }
	/*if(m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART) {
		 for (int j =m_ChartInfo.NumOfPoints ; j < m_ChartInfo.NumOfPoints*m_ChartInfo.NumVariables; j++) {
				int iloop=j/m_ChartInfo.NumOfPoints;
				int jj=j%m_ChartInfo.NumOfPoints;
				m_pProp->viewPortDataSeries[0][j]=m_pProp->viewPortDataSeries[iloop][jj] ;
		}
	}*/




}
MultiChart* TimeSeriePlot::CreateMultiCharts()
{
	MultiChart *mc=nullptr;

	m_pProp->m_NumMultiChartY=m_pProp->m_NumChartY;
	m_pProp->m_NumMultiChartX=m_pProp->m_NumChartX;
	m_pProp->m_NumMultiChart=m_pProp->m_NumSubCharts;

	int ytop=m_pProp->GetTopMargin();
	int ybottom=m_pProp->GetBottomMargin();
	m_ChartSize.ydiff=int(double(m_pProp->GetGapMargin())*(1.-m_pUtil->GetGapHorFraction()));
	m_ChartSize.xdiff=int(float(m_pProp->GetGapMargin())*m_pUtil->GetGapHorFraction());
	
	m_ChartSize.xdiff1=int(float(m_ChartSize.xdiff)*m_pUtil->GetGapHorLeftFraction());
	m_ChartSize.xdiff2=m_ChartSize.xdiff-m_ChartSize.xdiff1;
	m_ChartSize.ydiff1=int(float(m_ChartSize.ydiff)*m_pUtil->GetGapVertTopFraction());
	m_ChartSize.ydiff2=m_ChartSize.ydiff-m_ChartSize.ydiff1;

	int xleft=m_pProp->GetLeftMargin();
	int xright=m_pProp->GetRightMargin();

	int numCurvesPerChart=2;
	//m_DisplayXSize=m_cx-180;
	//m_DisplayYSize=m_cy-28;
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();
	if(m_pUtil->m_FileSave.m_MakeExtraChart) {
		m_pProp->m_DisplayXSize=MySingle::instance().GetExtraChartWndPointer()->m_cx;
		m_pProp->m_DisplayYSize=MySingle::instance().GetExtraChartWndPointer()->m_cy;
	}
	else {
		m_pProp->m_DisplayXSize=m_pView->GetXViewSize();
		m_pProp->m_DisplayYSize=m_pView->GetYViewSize();
	}

	m_pProp->m_NoXAxisScale=false;
	m_pProp->m_NoYAxisScale=false;
	

	
	if(m_pProp->m_NumSubCharts>=1) {
		//mc = new MultiChart(m_DisplayXSize, m_DisplayYSize, m_ColorBackground);
		m_ChartSize.yplotheight=(m_pProp->m_DisplayYSize-ytop-ybottom-(m_pProp->m_NumMultiChartY-1)*m_ChartSize.ydiff)/m_pProp->m_NumMultiChartY;
		m_ChartSize.xplotwidth=(m_pProp->m_DisplayXSize-xleft-xright-(m_pProp->m_NumMultiChartX-1)*m_ChartSize.xdiff)/m_pProp->m_NumMultiChartX;
		if((m_pProp->m_ChartType> CHART_TYPES::TIME_SERIE&&m_pProp->m_ChartType< CHART_TYPES::CONTOUR_CHART)||m_pProp->m_ChartType== CHART_TYPES::SCATTER) {
			if(m_ChartSize.yplotheight>m_ChartSize.xplotwidth)
				m_ChartSize.yplotheight=m_ChartSize.xplotwidth;
			else
				m_ChartSize.xplotwidth=m_ChartSize.yplotheight;
		}
		if(m_PrevMC!=nullptr) {
			delete m_PrevMC;
			m_PrevMC=nullptr;
		}
		if(m_ChartSize.yplotheight<m_pProp->m_DisplayYSize&&m_ChartSize.yplotheight>0) 
			mc = new MultiChart(m_pProp->m_DisplayXSize, m_pProp->m_DisplayYSize, m_pUtil->m_ColorBackground);
		else	{
			MFC_Util::MessageBoxReturn("View Window for chart is too small\nIncrease size of window or Reduce No of Charts in Y direction", "Chart size error", MB_OK);
				return nullptr;

		}
		m_PrevMC=mc;
		bool update=false;

		while(m_PrevBaseChart.size()>0) {
			delete m_PrevBaseChart[0];
			m_PrevBaseChart.erase(m_PrevBaseChart.begin());

		}
	}
	return mc;

}
XYChart* TimeSeriePlot::CreateSingleChart() {
	//XYChart *pXY_Chart;
	m_pProp->m_NoXAxisScale=false;
	m_pProp->m_NoYAxisScale=false;

	m_pProp->m_CommonScale.LeftMin=m_pProp->m_CommonScale.LeftMax=0.;
	m_pProp->m_TimeScale.MinX=m_pProp->m_TimeScale.MaxX=0.;
	m_pProp->m_CommonScale.RightMin=m_pProp->m_CommonScale.RightMax=0.;
	//if(m_pProp->m_Dist.Max>m_pProp->m_Dist.Min)
	
	m_pProp->m_TimeScale.MinX=m_pProp->m_TimeScale.MaxX=0.;
	m_ChartSize.xcol=(m_ichart%m_pProp->m_NumMultiChartX);
	m_ChartSize.yrow=(m_ichart/m_pProp->m_NumMultiChartX%m_pProp->m_NumMultiChartY);
	pXY_Chart=(XYChart*)CreateXYChart();
	m_PrevBaseChart.push_back(pXY_Chart);

	if(m_pUtil->GetRemoveTitle()==0)
		m_pProp->m_MakeChartTitle=false;
	else if(m_pUtil->GetRemoveTitle()==2)
		m_pProp->m_MakeChartTitle=true;

	return pXY_Chart;
}
void TimeSeriePlot::SetFrame_Axis(XYChart *pXY_Chart) {

	if(m_pProp->GetRoundedFrame())
           pXY_Chart->setRoundedFrame(m_pUtil->m_ColorBackground);
    
    // Set the plotarea at (52, 60) and of size 520 x 192 pixels. Use white (ffffff) 
    // background. Enable both horizontal and vertical grids by setting their colors to 
    // grey (cccccc). Set clipping mode to clip the data lines to the plot area.
 
    pXY_Chart->setClipping();

    // Add a top title to the chart using 15 pts Times New Roman Bold Italic font, with a 
    // light blue (ccccff) background, black (000000) border, and a glass like raised effect.
  //  pXY_Chart->addTitle("Zooming and Scrolling Demonstration", "timesbi.ttf", 15
   //     )->setBackground(0xccccff, 0x0, Chart::glassEffect());


    // Set axes width to 2 pixels
    pXY_Chart->yAxis()->setWidth(m_pUtil->m_LineAxisWidthPoint);
    pXY_Chart->xAxis()->setWidth(m_pUtil->m_LineAxisWidthPoint);

	pXY_Chart->xAxis()->setTickDensity(m_pUtil->GetMinTickDistX());
	pXY_Chart->yAxis()->setTickDensity(m_pUtil->GetMinTickDistY());
	pXY_Chart->yAxis2()->setTickDensity(m_pUtil->GetMinTickDistY());
    // Add a title to the y-axis






}
void TimeSeriePlot::AddLineAreaLayer(XYChart *pXY_Chart)  {

	if(m_pProp->m_ChartType== CHART_TYPES::SCATTER||(m_pProp->GetFirstIndepIndex()!=string::npos&&m_pProp->GetSecondIndepIndex()==0)||m_pProp->IsFirstIndepSet()) {      // Scatter
		m_Layers.pLineLayer=pXY_Chart->addLineLayer();
		m_Layers.pLineLayer->setLineWidth(m_pUtil->m_LineWidthPoint);
		m_Layers.pLineLayer2 = pXY_Chart->addLineLayer();
		m_Layers.pLineLayer2->setLineWidth(m_pUtil->m_LineWidthPoint);
	}
	else if(m_pProp->m_ChartType== CHART_TYPES::TIME_SERIE) {  // Time Series
    // Add a line m_Layers.pLineLayer for the lines, using a line width of 2 pixels
		m_Layers.pLineLayer = pXY_Chart->addLineLayer();
		m_Layers.pLineLayer2=pXY_Chart->addLineLayer();
		m_Layers.pLineLayer->setLineWidth(m_pUtil->m_LineWidthPoint);
		m_Layers.pLineLayer2->setLineWidth(m_pUtil->m_LineWidthPoint);
	}
	else if(m_pProp->m_ChartType== CHART_TYPES::FREQUENCE) { // Distribution type
		m_Layers.pAreaLayer=pXY_Chart->addAreaLayer();
		m_Layers.pAreaLayer->setLineWidth(m_pUtil->m_LineWidthPoint);
	}
	else if(m_pProp->m_ChartType== CHART_TYPES::CUM_FREQUENCE) { // Distribution type
		m_Layers.pLineLayer=pXY_Chart->addLineLayer();
		m_Layers.pLineLayer->setLineWidth(m_pUtil->m_LineWidthPoint);
	}
}
bool TimeSeriePlot::MakeAllMultiCharts(MultiChart *mc, size_t TotNumPrior) {
	bool ChartFilled;
	m_pProp->SetRightAxisExist(false);
	for (m_ichart = 0; m_ichart<m_pProp->m_NumMultiChart; m_ichart++) {
		m_pProp->m_MakeChartTitle = false;
		int curve_count = -1;
	

		pXY_Chart = CreateSingleChart();
		ChartFilled = false;
		SetFrame_Axis(pXY_Chart);
		if (m_pProp->m_ChartType != CHART_TYPES::CONTOUR_CHART)	AddLineAreaLayer(pXY_Chart);

		m_CurrentChartInfo.legends.clear();
		m_CurrentChartInfo.varunit.clear();
		m_CurrentChartInfo.varname.clear();
		m_CurrentChartInfo.X_AxisTitle = m_CurrentChartInfo.title = m_CurrentChartInfo.Y_AxisTitle_right = m_CurrentChartInfo.Y_AxisTitle_Left = "";


		ChartFilled = AddData_ToChart( TotNumPrior);


		if (ChartFilled) {
			AddTitle_Axis_ToChart( mc);
			CloseActivePGFile();
		}	
		else {
			m_pProp->m_YScaleLeftSet[m_ichart] = false;
			m_pProp->m_YScaleRightSet[m_ichart] = false;
		}
	}
	bool exist = false;
	for (size_t i = 0; i < m_ChartOrigInfo.size(); i++) if (m_ChartOrigInfo[i].Y_AxisTitle_right.size() > 0) exist = true;
	m_pProp->SetRightAxisExist(exist);
	return CheckIfCommonScaleCanBeSet();
}
void TimeSeriePlot::ShowChartonScreen_OptionallyCreateFile(MultiChart *mc) {
	m_ChartIsMade = true;
	
	if(m_pUtil->m_FileSave.m_MakeExtraChart) {
		MySingle::instance().GetExtraChartWndPointer()->SetChart(mc);
	}
	else {
		BaseChart *co;
		co=m_pChartViewer->getChart();
	//	if(co!=nullptr)
	//		delete co;
	//m_ChartViewer.setChart(mc);
		m_pView->MakeChart(mc);
	}
	if(m_pUtil->m_FileSave.m_SaveFile)
		m_pUtil->m_FileSave.MakeFile(m_pSimDoc);	
	m_pUtil->m_Displayed=true;
}
size_t TimeSeriePlot::GetPrior_UpdateNumberOfVariables() {
	size_t TotNumPrior;
	size_t numensembles = 0;
	for (size_t i = 0; i < m_pSimDoc->m_MStorage.m_AcceptedRuns.size(); i++) {
		 numensembles++;

	}

	size_t nfactor = numensembles + 1;

	size_t test = m_ChartInfo.NumVariables*nfactor;
	nfactor = max(1, min(AA.size() / m_ChartInfo.NumVariables, nfactor));

	m_ChartInfo.NumVariables = m_ChartInfo.NumVariables*nfactor;
	TotNumPrior = m_ChartInfo.NumVariables / nfactor;

	m_ChartInfo.NumPriorVariables=TotNumPrior;
	 
	if(m_pProp->m_PrevChartNumber!=m_pProp->m_ChartNumber) {
		m_pProp->ClearAndResetValuesForDistributionToChartVariables(m_pProp->m_ChartNumber, m_ChartInfo.NumVariables);
		m_pProp->m_PrevChartNumber=m_pProp->m_ChartNumber;
	}
	else {
		int nvar=m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);	
		if(m_pProp->AreDistributionsFunctionNotAllocatedForTheNumber(nvar)) 
			m_pProp->ClearAndResetValuesForDistributionToChartVariables(m_pProp->m_ChartNumber, nvar);
	}

	return TotNumPrior;
}
void TimeSeriePlot::updateImageMap(CChartViewer *viewer)

{
    if (0 == viewer->getImageMapHandler()&&!m_pUtil->m_MakeExtraChart)
    {
		if(m_pProp->m_ChartType== CHART_TYPES::TIME_SERIE) {
			if(!m_ChartIsMade) return;
			if (m_pUtil->m_MultiRunPlot) {
				const char* pChart = viewer->getChart()->getHTMLImageMap("clickable", "",
					"title='[{dataSetName}] {x}: {value|2}'");
				//if(pChart)
				//viewer->setImageMap(pChart);
			}
			else {

				// no existing image map - creates a new one
				viewer->setImageMap(viewer->getChart()->getHTMLImageMap("clickable", "",
					"title='[{dataSetName}] {x|mmm dd, yyyy,hh:mm}: {value|2}'"));


			}
		}
    }
	else if(0==m_pChartViewer->getImageMapHandler()) {
		if(m_pProp->m_ChartType== CHART_TYPES::TIME_SERIE) {
			if(!m_ChartIsMade) return;
			if(viewer->getMouseUsage()==Chart::MouseUsageScroll) return;
			if(m_pUtil->m_MultiRunPlot) 
				viewer->setImageMap(viewer->getChart()->getHTMLImageMap("clickable", "",
				"title='[{dataSetName}] {x}: {value|2}'"));
			else 
			// no existing image map - creates a new one
			 m_pChartViewer->setImageMap(m_pChartViewer->getChart()->getHTMLImageMap("clickable", "",
				"title='[{dataSetName}] {x|mmm dd, yyyy,hh:mm}: {value|2}'"));
		}




	}
}
void TimeSeriePlot::SetTimeWindowRange(double mindate, double maxdate) {

	m_Time.MinDate=mindate;
	m_Time.MaxDate=maxdate;
	m_Time.DateRange=m_Time.MaxDate-m_Time.MinDate;
}
void TimeSeriePlot::SetTimeWindow(double range) {
	m_Zoom.Window=range/m_Time.DateRange;
}
void TimeSeriePlot::SetNumSubWindow(int num) {
	m_Zoom.SubWindow=m_Zoom.Window/num;
}
void TimeSeriePlot::SetRelViewPortWidth(double RelFrac) {
	m_pChartViewer->setViewPortWidth(RelFrac*m_Zoom.Window);
}
void TimeSeriePlot::SetViewPortLeft() {
	m_pChartViewer->setViewPortLeft(double(m_Zoom.StartPos*m_Zoom.Window));
}
void TimeSeriePlot::EstimateNumWindows() {
	m_Zoom.NumberOfWindows=int(1./(m_Zoom.Window+0.0000001));
}
void TimeSeriePlot::SetZoomTimeUnit(int unit) {
	m_Zoom.TimeUnit=unit;}
void TimeSeriePlot::SetZoomLevel() {

	if(m_Zoom.NumberOfWindows>1)
		m_Zoom.ZoomLevel=m_Zoom.NumberOfWindows;
	else
		m_Zoom.ZoomLevel=int(1./m_pChartViewer->getViewPortWidth());
	m_Zoom.Hpos=int(m_pChartViewer->getViewPortLeft()*double(m_Zoom.ZoomLevel)+1.000001);
	if(m_Zoom.Hpos>m_Zoom.ZoomLevel) m_Zoom.Hpos=m_Zoom.ZoomLevel;



}
void TimeSeriePlot::SetCurrentTimeWindow() {
	
    // Compute the start date (in chartTime) and duration (in seconds) of the view port
    m_Zoom.viewPortStartDate = m_Time.MinDate + (int)(0.5 + (m_pChartViewer->getViewPortLeft()*m_pProp->GetNumChartX()) 
        * m_Time.DateRange);
    m_Time.CurrentDuration = (int)(0.5 + m_pView->m_ChartViewer.getViewPortWidth()*m_pProp->GetNumChartX() * m_Time.DateRange);
	string datum=PGUtil::StringDatumSec(m_Zoom.viewPortStartDate);

	m_Zoom.Window=m_Time.CurrentDuration;
	m_Zoom.SubWindow=m_Zoom.Window/24.;

}

