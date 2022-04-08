
#include "..\..\stdafx.h"
#include "..\..\std.h"
#include "MakePGCharts.h"

#include "..\..\Newbase\doc.h"
#include "../Chart/PlotDesignInfo/PlotInfo.h"
#include "../Chart/ChartProperties/ChartUtil.h"
#include "../Chart/ChartProperties/ChartProp.h"



MakePGCharts::MakePGCharts(void)
{
	m_pSimDoc=nullptr;
	m_pPlotInfo=MySingle::instance().GetPlotInfoPointer();
	m_pUtil=MySingle::instance().GetChartUtilPointer();
	m_pProp = MySingle::instance().GetChartPropPointer();
	m_CurrentChart=1;

	m_PreviousChart=-1;
}


MakePGCharts::~MakePGCharts(void)
{
}
void MakePGCharts::TimeSerieChartClick(Doc *pSimDoc)
{
	    m_pSimDoc = pSimDoc;
		m_pSimDoc->m_PlotStorage.RemoveAll();

		if(m_pSimDoc->m_SelectPGFile.size()==0) {
			
		}
		else if(m_pSimDoc->m_SelectPGFile.size()>0) {
			m_CurrentChart=m_pSimDoc->m_PlotStorage.GetCurrentChart()+1;
		}
		else {
			m_CurrentChart=m_pSimDoc->m_PlotStorage.GetCurrentChart();
		}

		int RepitNo;
		CPG *pPtr=nullptr;

		for(int i=0; i<m_pSimDoc->m_SelectPGFile.size();i++) {
				pPtr=m_pSimDoc->m_SelectPGFile[i].pPG;
				PLOTv plotv=m_pPlotInfo->MakePGFileChartInfo(i);
				//m_CurrentChartType=plotv.ChartType;

				plotv.SubChart_No = m_pSimDoc->m_SelectPGFile[i].subchart;
				if(!m_pSimDoc->m_PlotStorage.AddPlotInfo(plotv, m_CurrentChart))
					return;
		}
		if (pPtr == nullptr) return;
		string fileName=pPtr->GetFileName();

		if(m_CurrentChart!=m_PreviousChart) {
			m_VarNo=0;
			m_PreviousChart=m_CurrentChart;
		}
		if(CreateZoomPlotWindow(false, fileName)) {

			long row;
			int count=1;
			//UpdatePlotStatusBar(2);

		}

}
void MakePGCharts::MultiPGChartClick(Doc* pSimDoc)
{
	

		m_pSimDoc = pSimDoc;
		if(m_pSimDoc->m_SelectMultiPGFile.size()>0) {
			m_CurrentChart=m_pSimDoc->m_PlotStorage.GetCurrentChart()+1;
		}
		else {
			m_CurrentChart=m_pSimDoc->m_PlotStorage.GetCurrentChart();
		}
		int RepitNo;
		CPG *pPtr=nullptr;

		for(size_t i=0; i<m_pSimDoc->m_SelectMultiPGFile.size();i++) {
				pPtr=m_pSimDoc->m_SelectMultiPGFile[i].pPG;
				PLOTv plotv=m_pPlotInfo->MakeMultiPGChart(i);
			
				if(!m_pSimDoc->m_PlotStorage.AddPlotInfo(plotv, m_CurrentChart))
					return;
		}
		if (pPtr == nullptr) return;
		string fileName=pPtr->GetFileName();

		if(m_CurrentChart!=m_PreviousChart) {
			m_VarNo=0;
			m_PreviousChart=m_CurrentChart;
		}
		if(CreateZoomPlotWindow(true, fileName)) {

			long row;
			int count=1;
			//UpdatePlotStatusBar(2);

		}	
}
bool MakePGCharts::CreateZoomPlotWindow(bool multi, string FileName)
{
		bool good;

		ChartCug *pChart;
		pChart=MySingle::instance().GetChartCugPointer();
		m_pSimDoc->m_PlotStorage.Init(m_pSimDoc);
		m_pUtil->m_Displayed=false;
		return true;
		//m_pFrame->m_wndProperties.SetChoice(0);

	/*	if(multi) {											 
			good=pChart->m_TimeSeriePlot.InitMultiPGFilePlot(m_pSimDoc);
			//good=m_pZoom->InitMultiRunPlot(m_pDoc, m_CurrentChart, m_pProp->m_ChartType);
		}
		else {
			good=pChart->m_TimeSeriePlot.InitData(m_pSimDoc, m_CurrentChart, string(FileName));
			//good=m_pZoom->InitData(m_pDoc, m_CurrentChart, m_CurrentChartType,FileName);

		}
		if(good) {
				//m_pZoom->Create(IDD_ZOOMSCROLLDEMO_DIALOG, nullptr);
			//	m_pZoom->ShowWindow(SW_SHOW);
			//	pChart->RealTimeMode(false);
			//	pChart->ParameterPlotMode(false);
				pChart->Init(m_pSimDoc);
				if(pChart->m_TimeSeriePlot.MakeTimeSeriePlot()) 
					pChart->OnSetup();
				else
					return false;
				return true;

		}
		return false;	*/
}
bool MakePGCharts::CreateZoomPlotWindow(CPG *pPG)
{
			bool good;

	/*		ChartCug *pChart;
			pChart=MySingle::instance().GetChartCugPointer();
			//m_pFrame->m_wndProperties.SetChoice(0);

			m_pSimDoc->m_PlotStorage.Init(m_pSimDoc);
			m_pSimDoc->m_PlotStorage.Add(pPG, 10, 1, 0, m_pProp->m_ChartType,CT2A(m_pDoc->GetPathName()).m_psz);
			
			good=pChart->InitMultiPGFilePlot(m_pSimDoc);
				//good=m_pZoom->InitMultiRunPlot(m_pDoc, m_CurrentChart, m_CurrentChartType);

			
			if(good) {
					//m_pZoom->Create(IDD_ZOOMSCROLLDEMO_DIALOG, nullptr);
				//	m_pZoom->ShowWindow(SW_SHOW);
					pChart->RealTimeMode(FALSE);
					pChart->ParameterPlotMode(FALSE);
					if(pChart->m_TimeSeriePlot.MakeTimeSeriePlot()) 
						pChart->OnSetup();
					else
						return false;
					return true;

			}	  */
			return false;
}