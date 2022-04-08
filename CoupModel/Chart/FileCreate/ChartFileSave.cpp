#include "../../stdafx.h"
#include "ChartFileSave.h"
#include "../../NewBase\Doc.h"
#include "../../Util.h"
#include "../../CUG\ChartProperties\ChartCug.h"
#include "../../ChartWnd.h"

ChartFileSave::ChartFileSave(void)
{
	m_ZoomX=true;
	m_ZoomXY=false;
	m_SaveFile=false;
	m_MakeExtraChart=false;
	m_SaveExtraChart=false;
	m_NoChange=false;

}


ChartFileSave::~ChartFileSave(void)
{
}

void ChartFileSave::MakeFile(Doc *pSimDoc)
{
	ChartUtil *pUtil;
	pUtil=MySingle::instance().GetChartUtilPointer();
	ChartCug *pChartCug;
	pChartCug=MySingle::instance().GetChartCugPointer();
	if(pUtil->m_ParameterPlotMode) {
		bool prevSave, prevNotify;

		prevSave=m_SaveFile;
		prevNotify=m_NotifySaveFile;
		
		m_SaveFile=true;
		m_NotifySaveFile=true;
		
		//pChartCug->m_ParameterPlot.OnInit();
		
		m_SaveFile=prevSave;
		m_NotifySaveFile=prevNotify;

	}
	else if(pUtil->m_TablePlotMode) {
		bool prevSave, prevNotify;

		prevSave=m_SaveFile;
		prevNotify=m_NotifySaveFile;
		
		m_SaveFile=true;
		m_NotifySaveFile=true;
		//pChartCug->m_TableView.DrawTablePlot();

		m_SaveFile=prevSave;
		m_NotifySaveFile=prevNotify;


	}
	else if(pUtil->m_RealTimeMode)  {
		bool prevSave, prevNotify;

		prevSave=m_SaveFile;
		prevNotify=m_NotifySaveFile;
		
		m_SaveFile=true;
		m_NotifySaveFile=true;
		
		pChartCug->m_RealTime.OnViewPortChanged();
		
		m_SaveFile=prevSave;
		m_NotifySaveFile=prevNotify;


	}

//	else {


		string dir;//=m_pSimDoc->GetCurrentWorkDirectory();

		dir=MFC_Util::GetProfileStringStd("UserDirectory", dir);

		long num=pSimDoc->m_PlotStorage.GetNumberSavedCharts();
		num++;
		string filename, str;
		if(pSimDoc->m_DocumentType==0) 
			str=FUtil::STD_ItoAscii(pSimDoc->m_DocFile.m_SimulationRunNo);
		else {
			str=pSimDoc->GetCurrentSimFile();
			int pos=str.rfind('.');
			if(pos>0)
				str=str.substr(0,pos);
		}
		bool Exist=true;	
		BaseChart *c;

		while(Exist) {
			if(pSimDoc->m_DocumentType>0) {
				filename=str+"_"+FUtil::STD_ItoAscii(num)+".png";
			}
			else if(str.size()>3) 
				filename=dir+"COUP"+str+"_"+FUtil::STD_ItoAscii(num)+".png";
			else if(str.size()==2)
				filename=dir+"COUP_0"+str+"_"+FUtil::STD_ItoAscii(num)+".png";
			else if(str.size()==1)
				filename=dir+"COUP_00"+str+"_"+FUtil::STD_ItoAscii(num)+".png";
			else
				filename=dir+"COUP_"+str+"_"+FUtil::STD_ItoAscii(num)+".png";
			if(FUtil::IsFileExisting(filename)){		
					num++;
			}
			else 
					Exist=false;
		}
		CChartViewer *pViewer;
		pViewer=pChartCug->m_pChartViewer;
		if(m_MakeExtraChart) {
			CChartWnd *pChartWnd=MySingle::instance().GetExtraChartWndPointer();
			c=pChartWnd->m_ChartViewer.getChart();
		}
		else {
			c=pViewer->getChart();

		}

		c->makeChart(filename.c_str());
		if(m_NotifySaveFile)
			MFC_Util::MessageBox( filename.c_str(), "PNG File Created",0);
		pSimDoc->m_PlotStorage.SetNumberSavedCharts(num);


	//}




}
void ChartFileSave::SaveToExtraChartWindow()
{
		m_MakeExtraChart=true;
		OnReDraw(true);
		m_MakeExtraChart=false;
}
void ChartFileSave::SaveToFileExtraChart(Doc *pSimDoc)
{
		m_MakeExtraChart=true;
			MakeFile(pSimDoc);
		m_MakeExtraChart=false;
}
void ChartFileSave::OnReDraw(bool reload, Doc *pSimDoc)
{
	ChartUtil *pUtil;
	pUtil=MySingle::instance().GetChartUtilPointer();
	ChartCug *pChartCug;
	pChartCug=MySingle::instance().GetChartCugPointer();
	CChartViewer *pViewer;
	pViewer=pChartCug->m_pChartViewer;

	if(pUtil->m_TablePlotMode) {
		pChartCug->m_TableView.DrawTablePlot(pViewer);
	}
	else if(reload) {
		if(pChartCug->m_TimeSeriePlot.LoadTimeSerieData()&&(!pUtil->m_RealTimeMode
			&&!pUtil->m_ParameterPlotMode&&!pUtil->m_TablePlotMode)) {
		//OnInit(&m_pView->m_ChartViewer);
			//pChartCug->m_TimeSeriePlot.LoadTimeSerieData();
			pChartCug->m_TimeSeriePlot.DrawTimeSeriePlot(pViewer);
			
		}
		else
			pChartCug->MakeTextPlot("No Variables selected");
		m_NoChange=false;
		pChartCug->OnUpdate();
	}
	else if(MySingle::instance().GetChartPropPointer()->viewPortTimeStamps!=nullptr)
	   pViewer->updateViewPort(true, true);
	//

}
void ChartFileSave::OnPointerPB(CChartViewer *viewer) 
{
    viewer->setMouseUsage(Chart::MouseUsageScroll);   
}

//
// User clicks on the Zoom In pushbutton
//
void ChartFileSave::OnZoomInPB(CChartViewer *viewer) 
{
    viewer->setMouseUsage(Chart::MouseUsageZoomIn);

	MySingle::instance().GetChartPropPointer()->ResetXScale(0);
	
	if(m_ZoomXY==false)
		m_ZoomX=true;
	viewer->updateViewPort(true, true);
	OnReDraw(true);
}

//
// User clicks on the Zoom Out pushbutton
//
void ChartFileSave::OnZoomOutPB(CChartViewer *viewer) 
{
    viewer->setMouseUsage(Chart::MouseUsageZoomOut);  
	
	MySingle::instance().GetChartPropPointer()->ResetXScale(0);

	if(m_ZoomXY==false)
		m_ZoomX=true;
	viewer->updateViewPort(true, true);


}

//
// User clicks on the X-Zoom pushbutton
//
void ChartFileSave::OnXZoomPB(CChartViewer *viewer) 
{
    viewer->setZoomDirection(Chart::DirectionHorizontal); 
    viewer->setScrollDirection(Chart::DirectionHorizontal);

    // Viewport is always unzoomed as y-axis is auto-scaled
    viewer->setViewPortTop(0);
    viewer->setViewPortHeight(1);
        
    // Update chart to auto-scale axis
    viewer->updateViewPort(true, true);

	m_ZoomX=true;
	m_ZoomXY=false;
}

//
// User clicks on the XY-Zoom pushbutton
//
void ChartFileSave::OnXYZoomPB(CChartViewer *viewer) 
{
    viewer->setZoomDirection(Chart::DirectionHorizontalVertical); 
    viewer->setScrollDirection(Chart::DirectionHorizontalVertical);  
	m_ZoomX=false;
	m_ZoomXY=true;
}
void ChartFileSave::ShiftExtraChartWindow()
{
	MySingle::instance().GetExtraChartWndPointer()->ShiftChart();
}
