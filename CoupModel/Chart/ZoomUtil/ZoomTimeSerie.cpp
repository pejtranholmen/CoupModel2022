#include "../../std.h"
#include "../../stdafx.h"

#include "ZoomTimeSerie.h"
#include "../../Cug\ChartProperties\ChartCug.h"

ZoomTimeSerie:: ZoomTimeSerie(void)
{
	MySingle::instance().SetChartZoomPointer(this);
	m_pChartViewer=nullptr;
	m_Started=false;
	m_WindowWidthValue=0;
}
ZoomTimeSerie:: ~ZoomTimeSerie(void)
{


}
void ZoomTimeSerie::Init()
{

	m_pTimeSeriePlot=&MySingle::instance().GetChartCugPointer()->m_TimeSeriePlot;
	m_pChartViewer=nullptr;
	m_pChartCug=MySingle::instance().GetChartCugPointer();
	m_pProp=MySingle::instance().GetChartPropPointer();
	m_pUtil=MySingle::instance().GetChartUtilPointer();
	m_Started=false;
	m_WindowWidthValue=0;




}
void ZoomTimeSerie::OnChangeWindowWidth()
{
	// TODO: Add your control notification handler code here

	size_t minut;
	if(m_pUtil==nullptr) m_pUtil=MySingle::instance().GetChartUtilPointer();
	if(m_pChartViewer==nullptr) return;
	if(m_pUtil->m_MultiRunPlot) {
	    m_pChartViewer->setViewPortWidth(1.);
	  	m_pTimeSeriePlot->SetAggregateRatio(1.);
		m_pChartViewer->updateViewPort(true, true);
		OnViewPortChanged();
		return;
	}
	int year,month, day, yearend, monthend, dayend ;
	  minut=size_t(m_pProp->m_timeStamps[0]/60+2880);
	  year=PGUtil::YearFunction(minut);
	  month=PGUtil::MonthFunction(minut);
	  day=PGUtil::DayFunction(minut);

	  minut=size_t(m_pProp->m_timeStamps[m_pProp->m_timeStamps.len - 1]/60);
	  yearend=PGUtil::YearFunction(minut);
	  monthend=PGUtil::MonthFunction(minut);
	  dayend=PGUtil::DayFunction(minut);

	  m_pProp->ResetXScale(0);
	switch (m_WindowWidthValue) {

	case 0:
	  m_pChartViewer->setViewPortWidth(1.);
		break;
	case 1:
	   m_pTimeSeriePlot->SetTimeWindowRange(double(PGUtil::cminutx(year,1,1,0,0))*60-2*86400.,double(PGUtil::cminutx(yearend,12,31,24,0))*60);
	   m_pTimeSeriePlot->SetTimeWindow(365.2425*86400.);
	   m_pTimeSeriePlot->SetNumSubWindow(12);
	   m_pTimeSeriePlot->SetRelViewPortWidth(1.01);
	   m_pTimeSeriePlot->SetViewPortLeft();
	   m_pTimeSeriePlot->EstimateNumWindows();
	   m_pTimeSeriePlot->SetZoomTimeUnit(0);
		break;
	case 2:

	   m_pTimeSeriePlot->SetTimeWindowRange(double(PGUtil::cminutx(year,1,1,0,0))*60-2*86400.,double(PGUtil::cminutx(yearend,12,31,24,0))*60);
	   m_pTimeSeriePlot->SetTimeWindow(365.2425*43200.);
	   m_pTimeSeriePlot->SetNumSubWindow(6);
	   m_pTimeSeriePlot->SetRelViewPortWidth(1.01);
	   m_pTimeSeriePlot->SetViewPortLeft();
	   m_pTimeSeriePlot->EstimateNumWindows();
	   m_pTimeSeriePlot->SetZoomTimeUnit(0);

		break;
	case 3: //Month
	  m_pTimeSeriePlot->SetTimeWindowRange(double(PGUtil::cminutx(year,month,1,0,0))*60-2*86400.,double(PGUtil::cminutx(yearend,monthend,31,24,0))*60);
	   m_pTimeSeriePlot->SetTimeWindow(365.2425*7200.);
	   m_pTimeSeriePlot->SetNumSubWindow(12);
	   m_pTimeSeriePlot->SetRelViewPortWidth(1.01);
	   m_pTimeSeriePlot->SetViewPortLeft();
	   m_pTimeSeriePlot->EstimateNumWindows();
	   m_pTimeSeriePlot->SetZoomTimeUnit(1);
		break;
	case 4:  //Week

	  m_pTimeSeriePlot->SetTimeWindowRange(double(PGUtil::cminutx(year,month,1,0,0))*60-2*86400.,double(PGUtil::cminutx(yearend,monthend,31,24,0))*60);
	  m_pTimeSeriePlot->SetTimeWindow(7*86400);
	  m_pTimeSeriePlot->SetNumSubWindow(7);
	   m_pTimeSeriePlot->SetRelViewPortWidth(1.01);
	   m_pTimeSeriePlot->SetViewPortLeft();
	   m_pTimeSeriePlot->EstimateNumWindows();
	   m_pTimeSeriePlot->SetZoomTimeUnit(2);
		break;
	case 5: //Day
	  m_pTimeSeriePlot->SetTimeWindowRange(double(PGUtil::cminutx(year,month,day,0,0))*60-2*86400.,double(PGUtil::cminutx(yearend,monthend,dayend,24,0))*60);
	  m_pTimeSeriePlot->SetTimeWindow(86400);
	  m_pTimeSeriePlot->SetNumSubWindow(12);
	   m_pTimeSeriePlot->SetRelViewPortWidth(1.01);
	   m_pTimeSeriePlot->SetViewPortLeft();
	   m_pTimeSeriePlot->EstimateNumWindows();
	   m_pTimeSeriePlot->SetZoomTimeUnit(3);

		break;
	case 6: // half Day
	  m_pTimeSeriePlot->SetTimeWindowRange(double(PGUtil::cminutx(year,month,day,0,0))*60-2*86400.,double(PGUtil::cminutx(yearend,monthend,dayend,24,0))*60);
	  m_pTimeSeriePlot->SetTimeWindow(43200);
	  m_pTimeSeriePlot->SetNumSubWindow(12);
	   m_pTimeSeriePlot->SetRelViewPortWidth(1.01);
	   m_pTimeSeriePlot->SetViewPortLeft();
	   m_pTimeSeriePlot->EstimateNumWindows();
	   m_pTimeSeriePlot->SetZoomTimeUnit(3);
		break;
	case 7: //hour
	   m_pTimeSeriePlot->SetTimeWindowRange(double(PGUtil::cminutx(year,month,day,0,0))*60-2*86400.,double(PGUtil::cminutx(yearend,monthend,dayend,24,0))*60);
	   m_pTimeSeriePlot->SetTimeWindow(3600);
	   m_pTimeSeriePlot->SetNumSubWindow(12);
	   m_pTimeSeriePlot->SetRelViewPortWidth(1.01);
	   m_pTimeSeriePlot->SetViewPortLeft();
	   m_pTimeSeriePlot->EstimateNumWindows();
	   m_pTimeSeriePlot->SetZoomTimeUnit(4);
		break;
	}
	m_pTimeSeriePlot->SetAggregateRatio(1.);
	m_pChartViewer->updateViewPort(true, true);
	OnViewPortChanged();

}

void ZoomTimeSerie::OnViewPortChanged()

{
	if(m_pChartViewer==nullptr) return;
    //
    // Set up the scroll bars to reflect the current position and size of the view port
    //
	CUGCell cell; 
    SCROLLINFO info;
    info.cbSize = sizeof(SCROLLINFO);
    info.fMask = SIF_ALL;
    info.nMin = 0;
    info.nMax = 0x1fffffff;

	if(m_pProp->m_ChartType== CHART_TYPES::TIME_SERIE) {
		m_pTimeSeriePlot->DrawTimeSeriePlot(m_pChartViewer);
        m_pTimeSeriePlot->updateImageMap(m_pChartViewer);
		if(!m_Started)
			return;		
	}
	else if(m_pProp->m_ChartType== CHART_TYPES::FREQUENCE||m_pProp->m_ChartType== CHART_TYPES::CUM_FREQUENCE) {
	
		//drawChart(&m_ChartViewer);
        //updateImageMap(&m_ChartViewer);
		if(!m_Started)
			return;
}
    m_Started=false;

   if (m_pChartViewer->getViewPortWidth() < 1)
    {
		info.nPage = (int)ceil(m_pChartViewer->getViewPortWidth()* (info.nMax - info.nMin));
        info.nPos = (int)(0.5 + m_pChartViewer->getViewPortLeft()* (info.nMax - info.nMin))
            + info.nMin;
		m_pTimeSeriePlot->SetZoomLevel();
  
    }

    if (m_pChartViewer->getViewPortHeight() < 1)
    {
        info.nPage = (int)ceil(m_pChartViewer->getViewPortHeight() * (info.nMax - info.nMin));
        info.nPos = (int)(0.5 + m_pChartViewer->getViewPortTop() * (info.nMax - info.nMin))
            + info.nMin;
  
    }
	
	m_pTimeSeriePlot->SetCurrentTimeWindow();
    //
    // Set the start date CDateTimeCtrl control and duration combo box to reflect the current
    // position and size of the view port.
    //
	if(!m_pUtil->m_MultiRunPlot) {
    // Set the CDateTimeCtrl control to reflect the start date
		//int startYMD = Chart::getChartYMD(currentStartDate);
		//int startHMS = (int)fmod(currentStartDate, 86400);
		//CTime startDate = CTime(startYMD / 10000, (startYMD % 10000) / 100, startYMD % 100,
		//	startHMS / 3600, (startHMS % 3600) / 60, startHMS % 60);
		//m_StartDate.SetTime(&startDate);

		// Set the duration combo box to reflect the duration (in days)
		//CString buffer;
	//	buffer.Format(_T("%d"), (int)(0.5 + m_currentDuration / 86400));
	//	m_Duration.SetWindowText(buffer);

	}


    //
    // Update chart and image map if necessary
    //
    if (m_pChartViewer->needUpdateChart())
        	m_pTimeSeriePlot->DrawTimeSeriePlot(m_pChartViewer);
    if (m_pChartViewer->needUpdateImageMap())
			m_pTimeSeriePlot->updateImageMap(m_pChartViewer);
}

void ZoomTimeSerie::OnCommonday()
{
	long numsections;
	if(m_pProp->m_ReShape.m_CommonWithDay) {
		m_pProp->m_ReShape.m_CommonWithDay=false;
	}



	CPG *pPG;
	SimB *pX;
	int NoCurvesPrior;

	NoCurvesPrior=m_pPlotStorage->GetNumVar(m_pProp->m_ChartNumber);

	if(m_pProp->m_ReShape.m_CommonDayNo) {
	
		for(int i=0;i<NoCurvesPrior;i++) {


			int j=m_pPlotStorage->GetArrayIndex(m_pProp->m_ChartNumber, i);
			auto datatype=m_pPlotStorage->GetDataType(j);
			int idoc=m_pPlotStorage->GetDocIndex(j);
			void *Ptr=m_pPlotStorage->GetPtr(j);
			PLOTv p_info=m_pPlotStorage->GetPlotInfo(j);

			if(datatype==DATA_TYPES::PGDATA) {
				pPG=p_info.pPG;
			}
			else if(datatype==DATA_TYPES::ANYDATA) {
				pPG=(CPG*)((F*)Ptr)->GetPointer();
			}
			else if(datatype==DATA_TYPES::SIM_VAL) {

				pX=(SimB*)Ptr;	
				int FileNo=m_pPlotStorage->GetValFileNo(j);
				pPG=m_pPlotStorage->GetPGPointer(idoc,FileNo);
			//	pPG=m_pPlotStorage->m_PG_ALLMULTI[FileNo];
			

				}
			else {
				pPG=m_pPlotStorage->GetPGPointer(idoc);

			}
			if(pPG==nullptr) pPG=p_info.pPG;
			if(pPG==nullptr)break;
			pPG->ReOpen();
			pPG->EstimateWithinYearClear(0);


			int PGIndex=m_pPlotStorage->GetVar_Index(j);
			pPG->SetYearSectionStyle(true);
			numsections=pPG->GetNumberofSections();
			if(pPG->GetNumRecordsMonth(1)==pPG->GetNumRecords())
					pPG->CheckEquiDistance(true);						
			else
					pPG->CheckEquiDistance();
			if(pPG->IsEquiDistance())  {
				m_pProp->m_ReShape.NumOfCommonPoints=pPG->GetNumberOfCommonPoints();
				m_pProp->m_TimeScale.min=pPG->GetDoubleTimeYearStart(1);
			}
			else
				m_pProp->m_ReShape.NumOfCommonPoints=0;
			
			if(numsections<4&&numsections!=string::npos)
				m_pPlotStorage->SetPGSectionNo(j,1);
			else
				m_pPlotStorage->SetPGSectionNo(j,20001);
			numsections=min(100,numsections);

			int backshift;
			int enable=20000;
			if(numsections<4)
				enable=0;
			if(NoCurvesPrior<=1||numsections<30) {
				for(int ii=1; ii<numsections;ii++) {
					if(NoCurvesPrior==1)
						backshift=0;
					else {
						if(i==0)
							backshift=NoCurvesPrior-1;
						else 
							backshift=numsections*i+NoCurvesPrior-i-1;
					}
					if(ii>1) backshift=0;
					m_pPlotStorage->AddCurve(ii+1+enable, backshift);
				}
			}

			if(numsections<4)
				m_pPlotStorage->AddCurve(25000);
			else
				m_pPlotStorage->AddCurve(5000);



			}	
		

		m_pTimeSeriePlot->m_Agg.AggregateRatio=1;
	}
	else {
	//Remove curves with sections ID's from chart
			m_pPlotStorage->RemoveAllSectionCurves();
	}


	m_pProp->Reset_Selected_Or_All_BasicVector();
	m_pProp->m_PrevChartNumber=-1;
	m_pUtil->m_FileSave.OnReDraw(true);
	/*
	if(m_pProp->m_ReShape.m_CommonDayNo)
		GetDlgItem(IDC_BUTTON_NEWCURVE)->ModifyStyle(WS_DISABLED,0);
	else
		GetDlgItem(IDC_BUTTON_NEWCURVE)->ModifyStyle(0,WS_DISABLED);

	GetDlgItem(IDC_BUTTON_NEWCURVE)->RedrawWindow();*/
	// TODO: Add your control notification handler code here
}

void ZoomTimeSerie::OnCommonWithinDay()
{
	// TODO: Add your control notification handler code here
	long numsections;


	if(m_pProp->m_ReShape.m_CommonDayNo) {
		m_pProp->m_ReShape.m_CommonDayNo=false;
		m_pPlotStorage->RemoveAllSectionCurves();
	}

	m_pChartCug->UpdateData(true);
	CPG *pPG;
	int NoCurvesPrior;
	NoCurvesPrior=m_pPlotStorage->GetNumVar(m_pProp->m_ChartNumber);
	m_pProp->ResetXScale(0);

	if(m_pProp->m_ReShape.m_CommonWithDay) {

		for(int i=0;i<NoCurvesPrior;i++) {

			int j=m_pPlotStorage->GetArrayIndex(m_pProp->m_ChartNumber, i);
			auto datatype=m_pPlotStorage->GetDataType(j);
			int idoc=m_pPlotStorage->GetDocIndex(j);
			void *Ptr=m_pPlotStorage->GetPtr(j);
			int PGIndex=m_pPlotStorage->GetVar_Index(j);
			int FileNo=m_pPlotStorage->GetValFileNo(j);
			PLOTv p_info=m_pPlotStorage->GetPlotInfo(j);


			if(datatype == DATA_TYPES::PGDATA) {
				pPG=p_info.pPG;
				pPG->ReOpen();
			}
			else if(datatype == DATA_TYPES::SIM_VAL) {
				pPG=m_pPlotStorage->GetPGPointer(idoc,FileNo);
				pPG->ReOpen();
			}
			else if(datatype == DATA_TYPES::ANYDATA) {
				pPG=(CPG*)((F*)Ptr)->GetPointer();
				pPG->ReOpen();
			}
			else {
				pPG=m_pPlotStorage->GetPGPointer(idoc);

				if(pPG==nullptr) pPG=p_info.pPG;
				pPG->ReOpen();
			}
		

			if(pPG==nullptr) return;

			pPG->SetYearSectionStyle(false);
			numsections=pPG->GetNumberofSections();
			if(pPG->GetNumRecordsMonth(1)==pPG->GetNumRecords())
					pPG->CheckEquiDistance(true);						
			else
					pPG->CheckEquiDistance();
			if(pPG->IsEquiDistance()) {
				m_pProp->m_ReShape.NumOfCommonPoints=pPG->GetNumberOfCommonPoints();
				m_pProp->m_TimeScale.min=pPG->GetDoubleTimeYearStart(1);
			}
			m_pPlotStorage->SetPGSectionNo(j,6000);
			int backshift;
			int enable=6000;
			if(NoCurvesPrior==1)
				backshift=0;
			else {
				if(i==0)
					backshift=NoCurvesPrior-1;
				else 
					backshift=12*i+NoCurvesPrior-i-1;

			}
			for(int ii=1; ii<13;ii++) {
				if(ii>1) backshift=0;
				if(ii<12)
					enable=6000;
				else	
					enable=6000;
				m_pPlotStorage->AddCurve(ii+enable, backshift);
			}
		}	
		m_pTimeSeriePlot->m_Agg.AggregateRatio=1;
		m_pUtil->m_FileSave.OnReDraw(true);
	}
	else {
	//Remove curves with sections ID's from chart
			m_pPlotStorage->RemoveAllSectionCurves();
			m_pTimeSeriePlot->MakeTimeSeriePlot();
	}




	/*if(m_pProp->m_ReShape.m_CommonWithinDayRes)
		GetDlgItem(IDC_BUTTON_NEWCURVE)->ModifyStyle(WS_DISABLED,0);

	else
		GetDlgItem(IDC_BUTTON_NEWCURVE)->ModifyStyle(0,WS_DISABLED);

	GetDlgItem(IDC_BUTTON_NEWCURVE)->RedrawWindow();*/
}
