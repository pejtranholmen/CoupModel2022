/////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived ChartCugBase
const int DEFAULT_INTERVAL  = 500;


const short ID_TIMER = 1;

#include "..\..\Chart\HotSpotDlg.h"
#include "chartdir.h"


#include <math.h>
#include <time.h>
#include <algorithm>

#include "..\..\stdafx.h"
#include "..\..\Util.h"
#include "..\..\resource.h"
#include "..\..\CoupModelDoc.h"
#include ".\ChartCugBase.h"
#include "..\ColourPickerDlg.h"
#include "..\..\Chart\ChartViewer.h"
#include "..\..\CoupModelView.h"
#include <limits>
#include "..\..\NewBase\FileScanner.h"
#include "..\..\MainFrm.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
BEGIN_MESSAGE_MAP(ChartCugBase,CUGCtrl)
	//{{AFX_MSG_MAP(ChartCug)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP

 //  ON_CONTROL(CVN_ViewPortChanged, IDC_CHART, ZoomTimeSerie::OnViewPortChanged)
   ON_WM_QUERYDRAGICON()
   ON_WM_ACTIVATE()
   ON_WM_TIMER()

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// ChartCugBase construction/destruction
ChartCugBase::ChartCugBase()
{
//	UGXPThemes::UseThemes(true);
	m_pProp=MySingle::instance().GetChartPropPointer();
	m_pUtil=MySingle::instance().GetChartUtilPointer();
	m_Mode=1;
	m_bDrawExcelFocus = false;
	m_col=1;
	m_row=1;
	m_Hpos=1;
	m_Data=false;
	m_pCTab=nullptr;
	m_ColorChanged=RGB(255,200,200);
	m_ColorHeading=RGB(230,230,230);
	m_ColorClick=RGB(240,240,50);
	m_NoColor=RGB(255,255,255);
	m_ColorClick=RGB(240,240,50);
	m_PreviousMode=-1;
	m_NewResidualFormat=false;

	//m_largeFont.CreateFont(18,0,0,0,600,0,0,0,0,0,0,0,0,_T("Arial"));
	m_extralargeFont.CreateFont(40,0,0,0,600,0,0,0,0,0,0,0,0,_T("Arial"));
	
	//m_font.CreateFont(16,0,0,0,500,0,0,0,0,0,0,0,0,_T("Arial"));
	GetDefaultFont(m_font);
	GetDefaultLargeFont(m_largeFont);
	m_minValueRight=m_maxValueRight=0.;
	m_minXValue=m_maxXValue=0.;
	m_minValue=m_maxValue=0.;


	SetMultiSelectMode ( false );
	DragDropTarget ( false );
	m_pUtil->m_MultiTable=false;
	m_pProp->m_Title="";
	m_ActNumValues = 0;
	
	CWinApp* pApp = AfxGetApp();
	m_pUtil->m_TitleSizePoint=MFC_Util::GetProfileIntNo( "TitleSize", 12);
	m_pUtil->m_LabelSizePoint=MFC_Util::GetProfileIntNo( "LabelSize", 9);
	m_pUtil->m_LegendSizePoint=MFC_Util::GetProfileIntNo( "LegendSize", 9);
	m_pUtil->m_LineWidthPoint=MFC_Util::GetProfileIntNo( "LineWidth", 2);
	m_pUtil->m_LineAxisWidthPoint=MFC_Util::GetProfileIntNo( "LineAxisWidth", 2);
	m_pUtil->m_SymbolSizePoint=MFC_Util::GetProfileIntNo( "SymbolSize", 9);
	m_pUtil->m_AxisScalePoint=MFC_Util::GetProfileIntNo( "AxisScaleSize", 9);

	m_pUtil->m_MinTickDistX=MFC_Util::GetProfileIntNo( "AxisMinTickX",60); 
	m_pUtil->m_MinTickDistY=MFC_Util::GetProfileIntNo( "AxisMinTickY",60); 
	
	m_pProp->m_NumChartX=MFC_Util::GetProfileIntNo( "NumberChartX",1);
	m_pProp->m_NumChartY=MFC_Util::GetProfileIntNo( "NumberChartY",1);
	if(m_pProp->m_NumChartX<1) m_pProp->m_NumChartX=1;
	if(m_pProp->m_NumChartY<1) m_pProp->m_NumChartY=1;

	m_pProp->m_TopMargin=MFC_Util::GetProfileIntNo( "TopMargin",60);
	m_pProp->m_BottomMargin=MFC_Util::GetProfileIntNo( "BottomMargin",60);
	m_pProp->m_GapMargin=MFC_Util::GetProfileIntNo( "GapMargin",10);
	m_pProp->m_LeftMargin=MFC_Util::GetProfileIntNo( "LeftMargin",60);
	m_pProp->m_RightMargin=MFC_Util::GetProfileIntNo( "RightMargin",60);
	m_pUtil->m_LegendPositionX=MFC_Util::GetProfileIntNo( "LegendPositionX",60);
	m_pUtil->m_LegendPositionY=MFC_Util::GetProfileIntNo( "LegendPositionY",5);
	m_pProp->m_RoundedFrame=MFC_Util::GetProfileIntNo( "Rounded frame",0);
	m_pProp->m_CommonColorForSubCharts=MFC_Util::GetProfileIntNo( "CommonColorsSubCharts",0);

	m_pProp->m_VisibleFrames=MFC_Util::GetProfileIntNo( "VisibleFrames",0);
	m_pUtil->m_ColorLegendBackground=MFC_Util::GetProfileIntNo( "HColorLegendBackground", m_pUtil->m_ColorLegendBackground);
	m_pUtil->m_ColorLegendBackgroundFrame=MFC_Util::GetProfileIntNo( "HColorLegendBackgroundFrame", m_pUtil->m_ColorLegendBackgroundFrame);

	m_pProp->m_ChartError=MFC_Util::GetProfileIntNo( "ChartError", 0);


	m_pProp->m_DisplayXSize=MFC_Util::GetProfileIntNo( "DisplaySize_cx", 10);
	m_pProp->m_DisplayYSize=MFC_Util::GetProfileIntNo( "DisplaySize_cy", 100);
	
	m_colpos=0;


	m_CurrentTopRow=0;
	m_pUtil->m_ToBeUpdated=false;
	m_pUtil->m_RemoveLegends=0;
	m_pProp->m_RemoveAxisLabel=false;
	m_pProp->m_CommonColorForSubCharts=true;
	m_pProp->m_BoxDimmer=50;
	m_pProp->m_BoxRangeDimmer=150;
	m_pProp->m_BoxDimmer=MFC_Util::GetProfileIntNo( "BoxDimmer", m_pProp->m_BoxDimmer);
	m_pProp->m_BoxRangeDimmer=MFC_Util::GetProfileIntNo( "BoxRangeDimmer", m_pProp->m_BoxRangeDimmer);
	m_pUtil->m_RejectedPoints=MFC_Util::GetProfileIntNo( "RejectedPoints", 0);
	m_pUtil->m_LegendContentStyle=MFC_Util::GetProfileIntNo( "LegendContentStyle", 1);
	m_pUtil->m_RemoveLegends=MFC_Util::GetProfileIntNo( "RemoveLegends", 0);

	m_pUtil->m_ResidualRepresentation=MFC_Util::GetProfileIntNo( "ResidualPlot", 0);
	m_pUtil->m_ErrorSignal=MFC_Util::GetProfileIntNo( "ErrorSignal", 0);
	m_pUtil->m_RemoveXAxisScale=MFC_Util::GetProfileIntNo( "RemoveXAxisScale", 1);
	m_pUtil->m_RemoveYAxisScale=MFC_Util::GetProfileIntNo( "RemoveYAxisScale", 1);
	m_pUtil->m_RemoveTitle=MFC_Util::GetProfileIntNo( "RemoveTitle", 1);
	m_pUtil->m_BoxEdgeColor=m_pUtil->GetColorBoxEdge();
	m_pUtil->SetColorBoxEdge(m_pUtil->m_BoxEdgeColor);
	int value;
	value=MFC_Util::GetProfileIntNo( "HorizontalGapPercentage", 50);
	m_pUtil->m_GapHFraction=float(value)/100;
	value=MFC_Util::GetProfileIntNo( "HorizontalGapLeftPercentage", 50);
	m_pUtil->m_GapHorLeftFraction=float(value)/100;
	value=MFC_Util::GetProfileIntNo( "VerticalTopPercentage", 50);
	m_pUtil->m_GapVertTopFraction=float(value)/100;
	m_pUtil->m_XAxisExist=false;
	m_NumberOfPoints=0;
	m_MakeDataSheet=false;
	m_PGExtra.CreateNewFile(2,2);
	m_ToSavePGStruc=false;
	m_CreatePG=false;
	m_pSimDoc=nullptr;
	m_pPar = nullptr;
	m_TabIndex = 0;
	SetDefFont(&m_font);
	
	m_AnnimateStepValue=1;
	m_AnnimateValue=2;
	m_pProp->m_ReShape.m_SortedSeries=false;
	m_pProp->m_ReShape.m_NormalDist=false;
	m_pProp->m_ReShape.m_CommonWithDay=false;
	m_pProp->m_ReShape.m_CommonDayNo=false;
	m_pProp->m_ReShape.m_CommonScale=false;
	m_pProp->m_ReShape.m_Cumulated=0;
	m_pProp->m_ReShape.m_RegLine=false;
	m_pProp->m_ReShape.m_ShowMissingValue = false;
	m_pProp->m_ReShape.m_AddLine=false;
	m_pProp->m_ReShape.m_Symbol=false;
	m_pProp->m_ReShape.m_DerivateValues=false;
	m_pProp->m_ReShape.m_CommonWithinDayRes=false;

	m_pUtil->m_ColorBackground=0xf0f0ff;
	m_pUtil->m_ColorLegendBackground=0xf0f0ff;

	m_pProp->m_SymbolType.resize(NUM_BAND*4);
	for(int i=0;i<NUM_BAND;i++) {
		m_pProp->m_SymbolType[0+i*4]=7;
		m_pProp->m_SymbolType[1+i*4]=2;
		m_pProp->m_SymbolType[2+i*4]=1;
		m_pProp->m_SymbolType[3+i*4]=6;
	}
	m_pProp->m_PointType.resize(NUM_SERIES);
	for(int i=0;i<NUM_SERIES ;i++) {
		m_pProp->m_PointType[i]=false;
		m_pProp->viewPortDataSeries[i]=nullptr;
	}
	for(int i=0;i<NUM_BAND;i++) {
		m_pProp->viewPortDataSeriesB[i]=nullptr;
		m_pProp->viewPortDataSeriesMin[i]=nullptr;
		m_pProp->viewPortDataSeriesMax[i]=nullptr;
		m_pProp->viewPortDataSeriesP10[i]=nullptr;
		m_pProp->viewPortDataSeriesP90[i]=nullptr;
		m_pProp->viewPortDataSeriesP50[i]=nullptr;
	}
	m_pProp->viewPortTimeStamps=nullptr;
	m_pProp->viewPortTimeStamps2=nullptr;

	m_pProp->m_MultiSequence=0;
	m_TimeSeriePlot.m_ChartInfo.MultiPGMode=false;
	m_ChangeAllRunNo=false;
	m_NumberofMoves=0;
	m_interval=DEFAULT_INTERVAL;
	m_AnimationRunning=false;
	m_AnnimateValue=2;
	m_TimeSeriePlot.m_Agg.AggregateRatio=1;
	m_AnnimateStepValue=1;
	m_step=10;
	m_TimeSeriePlot.m_Zoom.StartPos=0;
	m_TimeSeriePlot.m_Zoom.TimeUnit=-1;
	m_TimeSeriePlot.m_Zoom.NumberOfWindows=1;
	m_TimeSeriePlot.m_Zoom.Window=1.;
	
	m_SubChartCount=0;
	m_pProp->m_NumMultiChart=1;
	m_NumSelectedPerChart=0;
	m_pProp->m_Title="";

	m_pProp->m_CommonScale.LeftMin=1.E38;
	m_pProp->m_CommonScale.LeftMax=-1.E38;
	m_pProp->m_CommonScale.LeftInter=-1.E38;
	m_pProp->m_CommonScale.RightMin=1.E38;
	m_pProp->m_CommonScale.RightMax=-1.E38;
	m_pProp->m_CommonScale.RightInter=-1.E38;

	m_pProp->m_ReShape.m_SortedSeries=false;
	m_pProp->m_ReShape.m_NormalDist=false;

	m_TimeSeriePlot.m_BoxCox.use=false;
	m_TimeSeriePlot.m_BoxCox.box_power=1;
	m_TimeSeriePlot.m_BoxCox.box_offset=1;
	m_ZoomLevel=1;
	m_RealTime.SetChartCUGPointer(this);
	m_ParameterPlot.SetChartCUGPointer(this);
	m_pUtil->m_RealTimeMode=false;
	m_pUtil->m_ParameterPlotMode=false;
	m_pUtil->m_TablePlotMode=false;

	m_pProp->m_PrevChartNumber=-1;
	m_pMainframe =AfxGetMainWnd();
	m_NumberOfBands=0;

	m_pUtil->m_FileSave.m_MakeExtraChart=false;
	m_pUtil->m_FileSave.m_SaveExtraChart=false;
	m_pUtil->m_FileSave.m_ZoomXY=false;
	m_pUtil->m_FileSave.m_ZoomX=false;
	m_pProp->m_MultiValid.AcceptRejectMode=false;
	m_TimeSeriePlot.m_ChartInfo.MultiValMode=false;
	m_PGRepit=false;
	m_pUtil->m_FileSave.m_NoChange=false;
	m_PrevMC=nullptr;
	m_TimeSeriePlot.m_Contour.ScatterContourPoints=false;
	m_TimeSeriePlot.m_Contour.SplineContourInterPolation=false;
	m_TimeSeriePlot.m_Contour.ScaleContourChart=false;
	

	m_pProp->m_MultiValid.AddOnlyValidRuns=false;

	m_pZoomTimeSerie=MySingle::instance().GetChartZoomPointer();
	m_pChartViewer=nullptr;
	m_TimeSeriePlot.m_Zoom.Hpos=0;
	m_TimeSeriePlot.m_Zoom.ZoomLevel=1;
	m_NumberOfBands=0;
	m_NumVarBandSelected=0;
	m_pView=nullptr;
}

ChartCugBase::~ChartCugBase()
{
	//UGXPThemes::CleanUp();
	m_largeFont.DeleteObject();
	m_extralargeFont.DeleteObject();
	m_font.DeleteObject();

}

void ChartCugBase::OnTimer(ONTIMER_TYPE	/* nIDEvent	*/)
{
	
		double viewportStart;
		double viewportWidth;
		viewportWidth=m_pView->m_ChartViewer.getViewPortWidth();
		viewportStart=double(m_NumberofMoves)/(double(m_TimeSeriePlot.m_Zoom.NumberOfWindows)*double(m_step))*(1.-viewportWidth);
		m_pView->m_ChartViewer.setViewPortLeft(viewportStart);
		m_NumberofMoves++;

       	m_TimeSeriePlot.DrawTimeSeriePlot(&m_pView->m_ChartViewer);
		m_pView->m_ChartViewer.updateViewPort(true, true);
		if(m_pUtil->m_FileSave.m_SaveFile) {
			m_pUtil->m_FileSave.m_NotifySaveFile=false;
			m_pUtil->m_FileSave.MakeFile(m_pSimDoc);
		}

		if(m_NumberofMoves>=(long(m_TimeSeriePlot.m_Zoom.NumberOfWindows)*m_step)&&m_AnimationRunning) {
			//KillTimer(ID_TIMER);
			m_NumberofMoves=0;
		///	m_gridChart.Open();
			//OnStartAnnimation();	
		}
		else if(!m_AnimationRunning) {
			KillTimer(ID_TIMER);
			//m_gridChart.Open();
		}


}

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid

// User clicks on the Pointer pushbutton
//



