/////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived ChartCug
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
#include ".\ChartCug.h"
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

BEGIN_MESSAGE_MAP(ChartCug,CUGCtrl)
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
// ChartCug construction/destruction
ChartCug::ChartCug()
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
	m_pCTab=NULL;
	m_ColorChanged=RGB(255,200,200);
	m_ColorHeading=RGB(230,230,230);
	m_ColorClick=RGB(240,240,50);
	m_NoColor=RGB(255,255,255);
	m_ColorClick=RGB(240,240,50);
	m_PreviousMode=-1;
	m_NewResidualFormat=false;

	m_largeFont.CreateFont(18,0,0,0,600,0,0,0,0,0,0,0,0,"Arial");
	m_extralargeFont.CreateFont(40,0,0,0,600,0,0,0,0,0,0,0,0,"Arial");
	
	m_font.CreateFont(16,0,0,0,500,0,0,0,0,0,0,0,0,"Arial");

	m_minValueRight=m_maxValueRight=0.;
	m_minXValue=m_maxXValue=0.;
	m_minValue=m_maxValue=0.;


	SetMultiSelectMode ( false );
	DragDropTarget ( false );
	m_pUtil->m_MultiTable=false;
	m_pProp->m_Title="";
	m_pProp->m_YLabel1.push_back("");
	m_pProp->m_YLabel2.push_back("");
	m_pProp->m_YLabel1Change.push_back(false);
	m_pProp->m_YLabel2Change.push_back(false);
	m_pProp->m_ChartNumIndex=-1;
	CWinApp* pApp = AfxGetApp();
	m_pUtil->m_TitleSizePoint=pApp->GetProfileInt("CoupModel - KTH", "TitleSize", 12);
	m_pUtil->m_LabelSizePoint=pApp->GetProfileInt("CoupModel - KTH", "LabelSize", 9);
	m_pUtil->m_LegendSizePoint=pApp->GetProfileInt("CoupModel - KTH", "LegendSize", 9);
	m_pUtil->m_LineWidthPoint=pApp->GetProfileInt("CoupModel - KTH", "LineWidth", 2);
	m_pUtil->m_LineAxisWidthPoint=pApp->GetProfileInt("CoupModel - KTH", "LineAxisWidth", 2);
	m_pUtil->m_SymbolSizePoint=pApp->GetProfileInt("CoupModel - KTH", "SymbolSize", 9);
	m_pUtil->m_AxisScalePoint=pApp->GetProfileInt("CoupModel - KTH", "AxisScaleSize", 9);

	m_pUtil->m_MinTickDistX=pApp->GetProfileInt("CoupModel - KTH", "AxisMinTickX",60); 
	m_pUtil->m_MinTickDistY=pApp->GetProfileInt("CoupModel - KTH", "AxisMinTickY",60); 
	
	m_pProp->m_NumChartX=pApp->GetProfileInt("CoupModel - KTH", "NumberChartX",1);
	m_pProp->m_NumChartY=pApp->GetProfileInt("CoupModel - KTH", "NumberChartY",1);
	if(m_pProp->m_NumChartX<1) m_pProp->m_NumChartX=1;
	if(m_pProp->m_NumChartY<1) m_pProp->m_NumChartY=1;
	m_NumSubCharts=m_pProp->m_NumChartX*m_pProp->m_NumChartY;
	m_pProp->m_TopMargin=pApp->GetProfileInt("CoupModel - KTH", "TopMargin",60);
	m_pProp->m_BottomMargin=pApp->GetProfileInt("CoupModel - KTH", "BottomMargin",60);
	m_pProp->m_GapMargin=pApp->GetProfileInt("CoupModel - KTH", "GapMargin",10);
	m_pProp->m_LeftMargin=pApp->GetProfileInt("CoupModel - KTH", "LeftMargin",60);
	m_pProp->m_RightMargin=pApp->GetProfileInt("CoupModel - KTH", "RightMargin",60);
	m_pUtil->m_LegendPositionX=pApp->GetProfileInt("CoupModel - KTH", "LegendPositionX",60);
	m_pUtil->m_LegendPositionY=pApp->GetProfileInt("CoupModel - KTH", "LegendPositionY",5);
	m_pProp->m_RoundedFrame=pApp->GetProfileInt("CoupModel - KTH", "Rounded frame",0);
	m_pProp->m_CommonColorForSubCharts=pApp->GetProfileInt("CoupModel - KTH", "CommonColorsSubCharts",0);
	m_pProp->m_VerticalLegends=pApp->GetProfileInt("CoupModel - KTH", "VerticalLegends",0);
	m_pProp->m_VisibleFrames=pApp->GetProfileInt("CoupModel - KTH", "VisibleFrames",0);
	m_pUtil->m_ColorLegendBackground=pApp->GetProfileInt("CoupModel - KTH", "HColorLegendBackground", m_pUtil->m_ColorLegendBackground);
	m_pUtil->m_ColorLegendBackgroundFrame=pApp->GetProfileInt("CoupModel - KTH", "HColorLegendBackgroundFrame", m_pUtil->m_ColorLegendBackgroundFrame);

	m_pProp->m_ChartError=pApp->GetProfileInt("CoupModel - KTH", "ChartError", 0);


	m_pProp->m_DisplayXSize=pApp->GetProfileInt("CoupModel - KTH", "DisplaySize_cx", 10);
	m_pProp->m_DisplayYSize=pApp->GetProfileInt("CoupModel - KTH", "DisplaySize_cy", 100);
	
	m_colpos=0;
	m_pProp->m_YScaleLeftChanged.push_back(false);
	m_pProp->m_YScaleLeftSet.push_back(false);
	m_pProp->m_YScaleRightChanged.push_back(false);
	m_pProp->m_YScaleRightSet.push_back(false);
	m_pProp->m_freeze=false;
	m_CurrentTopRow=0;
	m_pUtil->m_ToBeUpdated=false;
	m_pUtil->m_RemoveLegends=0;
	m_pProp->m_RemoveAxisLabel=false;
	m_pProp->m_CommonColorForSubCharts=true;
	m_pProp->m_BoxDimmer=50;
	m_pProp->m_BoxRangeDimmer=150;
	m_pProp->m_BoxDimmer=pApp->GetProfileInt("CoupModel - KTH", "BoxDimmer", m_pProp->m_BoxDimmer);
	m_pProp->m_BoxRangeDimmer=pApp->GetProfileInt("CoupModel - KTH", "BoxRangeDimmer", m_pProp->m_BoxRangeDimmer);
	m_pUtil->m_RejectedPoints=pApp->GetProfileInt("CoupModel - KTH", "RejectedPoints", 0);
	m_pUtil->m_LegendContentStyle=pApp->GetProfileInt("CoupModel - KTH", "LegendContentStyle", 1);
	m_pUtil->m_RemoveLegends=pApp->GetProfileInt("CoupModel - KTH", "RemoveLegends", 0);

	m_pUtil->m_ResidualRepresentation=pApp->GetProfileInt("CoupModel - KTH", "ResidualPlot", 0);
	m_pUtil->m_ErrorSignal=pApp->GetProfileInt("CoupModel - KTH", "ErrorSignal", 0);
	m_pUtil->m_RemoveXAxisScale=pApp->GetProfileInt("CoupModel - KTH", "RemoveXAxisScale", 1);
	m_pUtil->m_RemoveYAxisScale=pApp->GetProfileInt("CoupModel - KTH", "RemoveYAxisScale", 1);
	m_pUtil->m_RemoveTitle=pApp->GetProfileInt("CoupModel - KTH", "RemoveTitle", 1);
	m_pUtil->m_BoxEdgeColor=m_pUtil->GetColorBoxEdge();
	m_pUtil->SetColorBoxEdge(m_pUtil->m_BoxEdgeColor);
	int value;
	value=pApp->GetProfileInt("CoupModel - KTH", "HorizontalGapPercentage", 50);
	m_pUtil->m_GapHFraction=float(value)/100;
	value=pApp->GetProfileInt("CoupModel - KTH", "HorizontalGapLeftPercentage", 50);
	m_pUtil->m_GapHorLeftFraction=float(value)/100;
	value=pApp->GetProfileInt("CoupModel - KTH", "VerticalTopPercentage", 50);
	m_pUtil->m_GapVertTopFraction=float(value)/100;
	m_pUtil->m_RightAxisExist=false;
	m_pUtil->m_XAxisExist=false;
	m_NumberOfPoints=0;
	m_MakeDataSheet=false;
	m_PGExtra.CreateNewFile(2,2);
	m_ToSavePGStruc=false;
	m_CreatePG=false;
	m_pSimDoc=NULL;
	SetDefFont(&m_font);
	
	m_AnnimateStepValue=1;
	m_AnnimateValue=2;
	m_pProp->m_ReShape.m_SortedSeries=false;
	m_pProp->m_ReShape.m_NormalDist=false;
	m_pProp->m_ReShape.m_CommonWithDay=false;
	m_pProp->m_ReShape.m_CommonDayNo=false;
	m_pProp->m_ReShape.m_CommonScale=false;
	m_pProp->m_ReShape.m_LogScale=false;
	m_pProp->m_ReShape.m_Cumulated=0;
	m_pProp->m_ReShape.m_RegLine=false;
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
		m_pProp->viewPortDataSeries[i]=NULL;
	}
	for(int i=0;i<NUM_BAND;i++) {
		m_pProp->viewPortDataSeriesB[i]=NULL;
		m_pProp->viewPortDataSeriesMin[i]=NULL;
		m_pProp->viewPortDataSeriesMax[i]=NULL;
		m_pProp->viewPortDataSeriesP10[i]=NULL;
		m_pProp->viewPortDataSeriesP90[i]=NULL;
		m_pProp->viewPortDataSeriesP50[i]=NULL;
	}
	m_pProp->viewPortTimeStamps=NULL;
	m_pProp->viewPortTimeStamps2=NULL;

	m_pProp->m_MultiSequence=1;
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
	m_Stacked=false;
	m_DepthProfile=false;

	m_pUtil->m_FileSave.m_MakeExtraChart=false;
	m_pUtil->m_FileSave.m_SaveExtraChart=false;
	m_pUtil->m_FileSave.m_ZoomXY=false;
	m_pUtil->m_FileSave.m_ZoomX=false;
	m_pProp->m_MultiValid.AcceptRejectMode=false;
	m_TimeSeriePlot.m_ChartInfo.MultiValMode=false;
	m_PGRepit=false;
	m_pUtil->m_FileSave.m_NoChange=false;
	m_PrevMC=NULL;
	m_TimeSeriePlot.m_Contour.ScatterContourPoints=false;
	m_TimeSeriePlot.m_Contour.SplineContourInterPolation=false;
	m_TimeSeriePlot.m_Contour.ScaleContourChart=false;
	

	m_pProp->m_MultiValid.AddOnlyValidRuns=false;


	MySingle::instance().SetChartCugPointer(this);
	m_pZoomTimeSerie=MySingle::instance().GetChartZoomPointer();
	m_pChartViewer=nullptr;
	m_TimeSeriePlot.m_Zoom.Hpos=0;
	m_TimeSeriePlot.m_Zoom.ZoomLevel=1;
	m_NumberOfBands=0;
	m_NumVarBandSelected=0;
	m_pView=nullptr;
}

ChartCug::~ChartCug()
{
	//UGXPThemes::CleanUp();
	m_largeFont.DeleteObject();
	m_extralargeFont.DeleteObject();
	m_font.DeleteObject();

}
void ChartCug::Init(Doc *pDoc)
{	

	m_pSimDoc=pDoc;
	m_pView=m_pSimDoc->GetModelView();
	SetDefFont(&m_font);
	m_ModelView.InitDoc(pDoc);
	m_pChartViewer=&m_pView->m_ChartViewer;

//	m_MultiRunPlot=((CZoomscrolldemoDlg*)m_pDlg)->IsMultiRunPlot();

}

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid


void ChartCug::OnUpdate()
{
	CUGCell cell; 
	string str;
	string txt;
	string optionlist;
	int cols = m_col,
		rows = m_row;
	int n1;
	int NumTSV;
	m_pProp=MySingle::instance().GetChartPropPointer();

	bool RightLabels;
	RightLabels=false;
	if(m_pUtil->m_FileSave.m_NoChange) return;
	m_pUtil->m_FileSave.m_NoChange=true;
	EnableMenu(true); 
	bool XLabels, ZLabels;
	XLabels=false;
	ZLabels=false;
	SetUniformRowHeight(false); 
	SetDefFont(&m_font);
	m_pMainframe=AfxGetMainWnd();
		F* pValFile;

	m_NumSubCharts=m_pProp->m_NumChartX*m_pProp->m_NumChartY;

	//int ivar=m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);
	//m_NumSubCharts=min(m_NumSubCharts, ivar);

//	m_pUtil->m_MultiRunPlot=((CZoomscrolldemoDlg*)m_pDlg)->IsMultiRunPlot();
	if(m_Mode==0) {
		if(m_pUtil->m_RealTimeMode) {
			rows=4;
			cols=3;
		}
		else if(m_pUtil->m_ParameterPlotMode) {
			rows=(m_ParameterPlot.m_P.size()*m_ParameterPlot.m_plotp.Opt_Vec.size()+m_ParameterPlot.m_Ps.size());
			if(rows<4) rows=4;
			cols=6;

		}
		else if(m_pUtil->m_TablePlotMode) {
			rows=2;
			cols=6;
		}
		else {
			// Time Serie Charts
			if(m_pProp->m_ChartType==4||m_pProp->IsSecondIndepSet()) 
				cols=10;
			else
				cols=9;
			rows=5;
		}

	}
	else if(m_Mode==1) {

		rows=7;
		cols=10;
	}
	else if(m_Mode==2) { //Axis
		if(m_pUtil->m_RealTimeMode) {
			rows=m_pSimDoc->MR_Get_NumTotalPar();
			cols=3;

		}
		else if(m_pUtil->m_ParameterPlotMode) {
			Func *funcpointer=m_ParameterPlot.GetCurrentFuncPointer();
			PLOT_PARAM plotparam=funcpointer->GetPlotParam();
			rows=2+plotparam.LegendsVec.size();
			cols=5;

		}
		else {// Time Serie mode

			if(!m_pProp->IsSecondIndepSet())
				rows=m_NumSubCharts;
			else
				rows=1;

			if(!m_pProp->IsSecondIndepSet()&&m_pUtil->IsRightAxisExist()) {
				rows=2*m_NumSubCharts;
				RightLabels=true;		
			}
			if(m_pUtil->IsXAxisExist()&&!m_pProp->IsFirstIndepSet()) {
				rows+=m_NumSubCharts;
				XLabels=true;
			}
			else if(m_pUtil->IsXAxisExist()&&m_pProp->IsFirstIndepSet()) {
				rows+=m_NumSubCharts;
				XLabels=true;
			}
			else {
				rows+=1;

			}
			if(m_pUtil->IsZAxisExist()) {
				rows+=m_NumSubCharts;
				ZLabels=true;
			}
			
			cols=6;
		}
	}
	else if(m_Mode==3) {
		EnableUpdate(false);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(true); 

		if(m_pUtil->m_RealTimeMode) {
			NumTSV=0;
			for(int i=0;i<MAXSIMVAL;i++) {
				pValFile=m_pSimDoc->ValidationFilePointer(i+1);
				NumTSV+=pValFile->GetNumberOfSelectedVal();
			}
			rows=NumTSV;
			cols=3;

		}
		else if(m_pUtil->m_ParameterPlotMode) {
			if(m_ParameterPlot.m_pCPt!=NULL)
				rows=m_ParameterPlot.m_pCPt->GetSize();
			else
				rows=0;
			cols=1;

		}
		else {
			n1=m_pProp->m_Legend1.size();
			n1=m_TimeSeriePlot.m_ChartInfo.NumVariables;
			if(m_pProp->m_MultiValid.AcceptRejectMode)
				rows=n1*2;
			else
				rows=n1;
			cols=9;
			if(m_pUtil->m_MultiTable||m_ResidualSource||m_PGRepit) cols=11;

	
		}
	}
	else if(m_Mode==4) {
		if(m_pUtil->m_RealTimeMode) {
			rows=10;
			cols=3;
		}
		else {
			rows=m_TimeSeriePlot.m_ChartInfo.NumVariables;;
			cols=6;		
		}
	}
	else if(m_Mode==5) {
		rows=m_TimeSeriePlot.m_ChartInfo.NumVariables;;
		cols=6;		
	}
	else if(m_Mode==6) {
		m_NumVarSelected=m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);
		m_NumVarBandSelected=m_TimeSeriePlot.m_ChartInfo.NumVariableBands;
		SetNumberCols(m_NumVarSelected+1);
		AddTimeData(m_pProp->m_timeStamps,m_pProp->m_timeStamps2);

		size_t nstart=0;
		if(m_pProp->IsFirstIndepSet()) nstart=1;

		for(int i=0; i<m_NumVarSelected;i++) {
			if(m_pProp->m_MultiValid.AcceptRejectMode&&!m_pProp->IsFirstIndepSet())
				AddMainData(m_TimeSeriePlot.m_dataSeriesA[i+m_NumVarSelected],i+m_NumVarSelected);
			AddMainData(m_TimeSeriePlot.m_dataSeriesA[i],i+nstart);
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
		cell.SetLabelText(optionlist.c_str());
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
		//QuickSetText(0,-1,"Box-Cox Transformation");

	
				


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
	else if(m_Mode==2&&m_pSimDoc!=NULL) {
			if(m_pUtil->m_RealTimeMode) Make_Sheet2_RealTime();
			else if(m_pUtil->m_ParameterPlotMode) Make_Sheet2_ParameterPlot();
			else Make_Sheet2_TimeSeriePlot(RightLabels, XLabels, ZLabels);
	}
	else if(m_Mode==3&&m_pSimDoc!=NULL) {
			if(m_pUtil->m_RealTimeMode)  Make_Sheet3_RealTime(NumTSV);	
			else if(m_pUtil->m_ParameterPlotMode) Make_Sheet3_ParameterPlot();
			else Make_Sheet3_TimeSeriePlot(); // Time Serie plot
				
	}
	else if(m_Mode==4) {
		if(m_pUtil->m_RealTimeMode) Make_Sheet4_RealTime();
		else Make_Sheet4_TimeSeriePlot(rows);
	}
	else if(m_Mode==5&&m_pSimDoc!=NULL&&!m_pUtil->m_RealTimeMode) Make_Sheet5_TimeSeriePlot(rows);

	if(m_pUtil->m_ToBeUpdated) {
		QuickSetBackColor(-1,-1,m_ColorChanged);
	}
	else
		QuickSetBackColor(-1,-1,m_ColorClick);		

	SetTopRow(m_CurrentTopRow);
	RedrawAll();
		

}
void ChartCug::OnSetup()
{
	CUGCell cell; 
	CString str;
	int cols = m_col,rows = m_row;

	if((m_PreviousMode==0&&m_pUtil->m_RealTimeMode)||(m_PreviousMode==1&&m_pUtil->m_ParameterPlotMode)
		||(m_PreviousMode==2&&m_pUtil->m_TablePlotMode)||(m_PreviousMode==3&&!m_pUtil->m_RealTimeMode&&!m_pUtil->m_ParameterPlotMode&&!m_pUtil->m_TablePlotMode))
		return;


		
	int num_sheets=GetNumberSheets();
	for (long i=0;i<num_sheets;i++) {
		DeleteTab(i);
	}
	SetMultiSelectMode ( true );

	EnableMenu(true); 
	UseHints(true);
	EnableUpdate(true);
	if(m_pUtil->m_RealTimeMode) {
		AddTab("Actions", 0); 
		AddTab("General Layout", 1); 
		if(m_pSimDoc->IsMultiSimulation()) {
			AddTab("Normalised Parameters", 2); 
			AddTab("Validation Variables", 3); 
			AddTab("Additional Variables", 4); 
		}
		m_PreviousMode=0;
	}
	else if(m_pUtil->m_ParameterPlotMode) {
		AddTab("Actions", 0); 
		AddTab("General Layout", 1); 
		AddTab("Change Chart Details", 2); 
		if(m_ParameterPlot.m_TableIndex>=0)
			AddTab("Layers to view",3);

		m_PreviousMode=1;
	}
	else if(m_pUtil->m_TablePlotMode) {
		AddTab("Actions", 0); 
		AddTab("General Layout", 1); 
		if(m_ParameterPlot.m_TableIndex>=0)
			AddTab("Layers to view",2);
		m_PreviousMode=2;

	}
	else {
		AddTab("Actions", 0); 
		AddTab("General Layout", 1); 
		AddTab("Axis", 2); 
		AddTab("Variables", 3); 
		AddTab("Distributions", 4);
		AddTab("Regression", 5);
		AddTab("The Data", 6);
		AddTab("Box Cox Transformation", 7);
		m_PreviousMode=3;
	}

	SetTabWidth(800); 


			


	//	int index = AddDataSource(&m_data);
	//	SetDefDataSource(index);
	//	SetGridUsingDataSource(index);
	//SetVScrollMode ( UG_SCROLLTRACKING );
	//SetHScrollMode ( UG_SCROLLTRACKING );

		

		if(m_pUtil->m_RealTimeMode) {
			if(m_pSimDoc->IsMultiSimulation())
				SetNumberSheets(5);
			else
				SetNumberSheets(2);

		}
		else if(m_pUtil->m_ParameterPlotMode) {
			if(m_ParameterPlot.m_TableIndex>0)
				SetNumberSheets(4);
			else
				SetNumberSheets(3);



		}
		else 
			SetNumberSheets(8);


		SetSheetNumber(0); 
  		OnSheetSetup(0);
		SetCurrentTab(0);

		return;
	
}

/////////////////////////////////////////////////////////////////////////////
//	OnSheetSetup	
void ChartCug::OnSheetSetup(int sheetNumber)
{
	//m_Mode is the current sheetNumber
	if(m_pSimDoc==NULL &&sheetNumber==0) {
			m_Mode=0;
			OnUpdate();	

		RedrawAll();
		return;

	}
	else if(m_pSimDoc==NULL) {
		m_Mode=10;
		OnUpdate();	

		RedrawAll();
		return;

	}
	m_pUtil->m_FileSave.m_NoChange=false;
	switch(sheetNumber) {
			case(0): //Remade
				m_Mode=0;
				OnUpdate();			
				break;
			case(1): //General
				m_Mode=1;
				OnUpdate();			
				break;
			case (2): //Axis
				m_Mode=2;
				OnUpdate();
				break;
			case(3): // Variables
				m_Mode=3;
				OnUpdate();
				break;
			case(4): //Distributions
				m_Mode=4;
				OnUpdate();
				break;
			case(5): // Regression
				m_Mode=5;
				OnUpdate();
				break;
			case(6): // Data
				m_Mode=6;
				SetSheetNumber(6,true);
				long r;
				r=GetNumberRows();
				int co;
				co=GetNumberCols();
				RedrawAll();
			    OnUpdate();			
				break;
			case(7): // Data
				m_Mode=7;
				OnUpdate();			
				break;
	}
	RedrawAll();





	
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the current cell in the grid is about to move
//		A return of true allows the move, a return of
//		false stops the move
int ChartCug::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return true;
}
/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the top row or left column in the grid is about to move
//		A return of true allows the move, a return of
//		false stops the move
int ChartCug::OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	
void ChartCug::OnHitBottom(long numrows,long rowspast,long rowsfound)
{
}

/////////////////////////////////////////////////////////////////////////////
//	
void ChartCug::OnHitTop(long numrows,long rowspast)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeCol
//		Sent when the user is over a separation line on the top heading
//		A return value of true allows the possibiliy of a resize
int ChartCug::OnCanSizeCol(int col)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSizing
//		Sent when the user is sizing a column
//		The column that is being sized is given as 
//		well as the width. Plus the width can be modified 
//		at this point. This makes it easy to set min and
//		max widths
void ChartCug::OnColSizing(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSized
//		This is sent when the user finished sizing the 
//		given column (see above for more details)
void ChartCug::OnColSized(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeRow
//		Sent when the user is over a separation line on the side heading
//		A return value of true allows the possibiliy of a resize
int  ChartCug::OnCanSizeRow(long row)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSizing
//	    Sent when the user is sizing a row The row that is being sized is given as 
//		well as the height. Plus the height can be modified 
//		at this point. This makes it easy to set min and max heights
void ChartCug::OnRowSizing(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSized
//		This is sent when the user is finished sizing hte
//		given row ( see above for more details)
void ChartCug::OnRowSized(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeSideHdg
//		This is sent when the user moves into position 
//		for sizing the width of the side heading
//		return true to allow the sizing
//		or false to not allow it
int ChartCug::OnCanSizeSideHdg()
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeTopHdg
//		This is sent when the user moves into position 
//		for sizing the height of the top heading
//		return true to allow the sizing
//		or false to not allow it
int ChartCug::OnCanSizeTopHdg()
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSizing
int ChartCug::OnSideHdgSizing(int *width)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSized
int ChartCug::OnSideHdgSized(int *width)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSized
int ChartCug::OnTopHdgSized(int *height)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSizing
int ChartCug::OnTopHdgSizing(int *height)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColChange
//		Sent whenever the current column changes
//		The old and the new columns are given
void ChartCug::OnColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowChange
//		Sent whenever the current row changes
//		The old and the new rows are given
void ChartCug::OnRowChange(long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellChange
//		Sent whenever the current cell changes rows or columns
void ChartCug::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnLeftColChange
//		Sent whenever the left visible column in the grid changes
void ChartCug::OnLeftColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopRowChange
//		Sent whenever the top visible row in the grid changes
void ChartCug::OnTopRowChange(long oldrow,long newrow)
{
}

/***************************************************
OnSelectionChanged
	The OnSelectionChanged notification is called by the multiselect
	class when a change occured in current selection (ie user clicks
	on a new cell, drags a mouse selecting range of cells, or uses
	CTRL/SHIFT - left click key combination to select cells.)
Params:
	startCol, startRow	- coordinates of the cell starting the selection block
	endCol, endRow		- coordinates of the cell ending the selection block
	blockNum			- block number representing this range, this will
						  always represent total number of selection blocks.
Return:
	<none>
****************************************************/
void ChartCug::OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum)
{
	TRACE("ChartCug::OnSelectionChanged called with startCol = %ld, startRow = %ld, endCol = %ld, endRow = %ld, blockNum = %ld.\n",
				startCol, startRow, endCol,	endRow, blockNum );
}

/////////////////////////////////////////////////////////////////////////////
//	OnLClicked
//		Sent whenever the user clicks the left mouse button within the grid
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' and 'row' are negative if the area clicked in is not a valid cell
//		'rect' the rectangle of the cell that was clicked in
//		'point' the point where the mouse was clicked
//		'updn'  true if the button is down false if the button just when up
void ChartCug::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
	CUGCell cell; 
	CString str, txt;
	int ArrayIndex;
	BOOL RightLabels;
	
	if(!updn)
		return;
	RightLabels=false;
	if(m_pUtil->IsRightAxisExist())
		RightLabels=true;
	int n1=m_pProp->m_Legend1.size();
	m_CurrentTopRow=GetTopRow();
	if(m_Mode==0) {
		if(m_pUtil->m_RealTimeMode) {
			GetCell(col,row,&cell);
			switch(row) {
			case 0:
				if(col==0&&m_pSimDoc->IsMultiSimulation()) {
					m_RealTime.InitData(m_pSimDoc);
					m_RealTime.SetMultiSimDisplay(cell.GetBool());


				}
				else if(col==0&&!m_pSimDoc->IsMultiSimulation()) {
					m_pSimDoc->m_Simulator.SetGoSim();
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
					m_pSimDoc->m_Simulator.SetPause();
					//PARAMETERS_mp_RUN_PAUSE=1;
				else {
					m_pSimDoc->m_Simulator.SetGoSim();
//					PARAMETERS_mp_RUN_PAUSE=0;
//					PARAMETERS_mp_RUN_STEP=0;
				}
				RedrawCell(0,1);
				OnUpdate();
				break;
			case 2:
				if(cell.GetBool()) {
					m_pSimDoc->Set_MultiStopped();
					MessageBox("Please wait while multirun simulation will be closed ! ");
				}
				break;
			case 3:
				if(cell.GetBool())
					m_pUtil->SetRemoveLegends(0);
				else
					m_pUtil->SetRemoveLegends(2);
			}

		}
		else if(m_pUtil->m_ParameterPlotMode) {
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
		else if(m_pUtil->m_TablePlotMode) {
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
		else {
			// Normal Time serie chart mode
			int i=0;
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
						m_pProp->m_ReShape.m_LogScale=true;
					}
					else {
						m_pProp->m_ReShape.m_LogScale=false;
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
				else if(row==3)
					m_pUtil->m_FileSave.SaveToFileExtraChart();
				else if(row==4)
					m_pUtil->m_FileSave.ShiftExtraChartWindow();
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


	}
	if(m_Mode==2&&m_pUtil->m_RealTimeMode) {
		GetCell(col,row,&cell);
		SimB* pPar;
		if(col==2) {
			BOOL v=cell.GetBool();
			GetCell(0,row,&cell);
			int idim=cell.GetParam();
			GetCell(1,row,&cell);
			int ip=cell.GetParam();
			if(idim>=0&&ip>=0) {
				//m_pSimDoc->MR_SetResetValMonitoring();
				//m_pSimDoc->MR_SetResetMonitoring();
				m_RealTime.ResetValMonitoring();
				pPar=m_pSimDoc->MR_Get_pPar(idim+1,ip+1);
				if(pPar!=NULL) {
					m_RealTime.InitData(m_pSimDoc);
					int TabIndex=m_pSimDoc->MR_GetTabIndex(idim+1,ip+1);
					if(TabIndex>=0) {
						((P*)pPar)->MR_Set_Monitoring(TabIndex, int(v));
					}
					else
						((Ps*)pPar)->MR_Set_Monitoring(int(v));
				}

			}
			OnUpdate();
		}


	}
	else if(m_Mode==2&&m_pUtil->m_ParameterPlotMode) {
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
	else if(m_Mode==3&&m_pUtil->m_ParameterPlotMode) {
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

	if(m_Mode==3&&m_pSimDoc!=NULL) {
		if(m_pUtil->m_RealTimeMode) {
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
		else if(!m_pUtil->m_ParameterPlotMode) {



			ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumIndex, row);

			int koll=m_pSimDoc->m_PlotStorage.GetPGSectionNo(ArrayIndex);

			int itype=m_pSimDoc->m_PlotStorage.GetType(ArrayIndex);
			if(itype<0) return;
			if(koll==0&&col==0&&itype<100) return;
			int value, index;
			index=row;
			if(col==0&&itype<100||m_pProp->m_ReShape.m_CommonDayNo&&col==0||m_pProp->m_ReShape.m_CommonWithDay&&col==0){

				GetCell(0,row,&cell);
				if(cell.GetBool()) {
					m_pSimDoc->m_PlotStorage.EnableSectionNo(ArrayIndex, true);
				}
				else {
						m_pSimDoc->m_PlotStorage.EnableSectionNo(ArrayIndex, true);
				}
				m_pUtil->m_FileSave.OnReDraw(true);
	
			}
			else if((itype==5||itype>100)&&m_pSimDoc!=NULL&&col==8) {
					//m_pProp->m_MultiSequence++;
					//((CZoomscrolldemoDlg*)m_pDlg)->OnUpdateSequence();
					if(m_TimeSeriePlot.m_ChartInfo.MultiPGMode)
						m_pProp->m_MultiSequence=m_pSimDoc->m_PlotStorage.GetValRunNo(ArrayIndex);
					else if(m_TimeSeriePlot.m_ChartInfo.MultiValMode)
						m_pProp->m_MultiSequence=m_pSimDoc->m_PlotStorage.GetValRunNo(ArrayIndex)+1;
					m_TimeSeriePlot.OnUpdateSequence(ArrayIndex);
				//	m_pProp->m_MultiSequence++;Puh88win
				//	m_pProp->m_MultiSequence=(m_pProp->m_MultiSequence-1)%m_pSimDoc->MR_Get_TotalNumberofRuns()+1;
				//	m_pSimDoc->m_PlotStorage.SetValRunNo(ArrayIndex, m_pProp->m_MultiSequence);
					m_pUtil->m_FileSave.OnReDraw(true);

			}
			else if(itype==10&&m_PGRepit) {

				int irep=m_pSimDoc->m_PlotStorage.GetPGRepitNo(ArrayIndex);
				CPG *pPG=(CPG*)m_pSimDoc->m_PlotStorage.GetPtr(ArrayIndex);
				long nrep=pPG->GetNumRepititions();
				irep++;
				irep=(irep-1)%nrep+1;
				m_pSimDoc->m_PlotStorage.SetPGRepitNo(ArrayIndex, irep);
				m_pUtil->m_FileSave.OnReDraw(true);
			}
			else if(col==6&&m_pSimDoc!=NULL) {
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

	}
	if(col>=0&&col<7) {

	if(!m_pUtil->m_ToBeUpdated) {
		m_pUtil->m_ToBeUpdated=true;
		QuickSetBackColor(-1,-1,m_ColorChanged);
	}
	else
		QuickSetBackColor(-1,-1,m_ColorClick);



	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnRClicked
//		Sent whenever the user clicks the right mouse button within the grid
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' and 'row' are negative if the area clicked in is not a valid cell
//		'rect' the rectangle of the cell that was clicked in
//		'point' the point where the mouse was clicked
//		'updn'  true if the button is down false if the button just when up
void ChartCug::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
//		Sent whenever the user double clicks the left mouse button within the grid
//	
//		'col' and 'row' are negative if the area clicked in is not a valid cell
//		'rect' the rectangle of the cell that was clicked in
//		'point' the point where the mouse was clicked
void ChartCug::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	StartEdit();
}				 

/////////////////////////////////////////////////////////////////////////////
//	OnMouseMove
void ChartCug::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user clicks the left mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  true if the button is down false if the button just when up
void ChartCug::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{

	CUGCell cell; 
	CString str, txt;
	if(m_pUtil->m_ParameterPlotMode) {
		bool prevSave, prevNotify;

		prevSave=m_pUtil->m_FileSave.m_SaveFile;
		prevNotify=m_pUtil->m_FileSave.m_NotifySaveFile;
		
		m_pUtil->m_FileSave.m_SaveFile=true;
		m_pUtil->m_FileSave.m_NotifySaveFile=true;
		
		m_ParameterPlot.OnInit();
		
		m_pUtil->m_FileSave.m_SaveFile=prevSave;
		m_pUtil->m_FileSave.m_NotifySaveFile=prevNotify;

	}
	else if(m_pUtil->m_TablePlotMode) {
		bool prevSave, prevNotify;

		prevSave=m_pUtil->m_FileSave.m_SaveFile;
		prevNotify=m_pUtil->m_FileSave.m_NotifySaveFile;
		
		m_pUtil->m_FileSave.m_SaveFile=true;
		m_pUtil->m_FileSave.m_NotifySaveFile=true;
		m_TableView.DrawTablePlot(&m_pView->m_ChartViewer);

		m_pUtil->m_FileSave.m_SaveFile=prevSave;
		m_pUtil->m_FileSave.m_NotifySaveFile=prevNotify;


	}
	else if(m_pUtil->m_RealTimeMode)  {
		BOOL prevSave, prevNotify;

		prevSave=m_pUtil->m_FileSave.m_SaveFile;
		prevNotify=m_pUtil->m_FileSave.m_NotifySaveFile;
		
		m_pUtil->m_FileSave.m_SaveFile=true;
		m_pUtil->m_FileSave.m_NotifySaveFile=true;
		
		m_RealTime.OnViewPortChanged();
		
		m_pUtil->m_FileSave.m_SaveFile=prevSave;
		m_pUtil->m_FileSave.m_NotifySaveFile=prevNotify;


	}

	else {
		if(col==8) {
			BOOL prevSave, prevNotify;

			prevSave=m_pUtil->m_FileSave.m_SaveFile;
			prevNotify=m_pUtil->m_FileSave.m_NotifySaveFile;
		
			m_pUtil->m_FileSave.m_SaveFile=true;
			m_pUtil->m_FileSave.m_NotifySaveFile=true;
		
			m_TimeSeriePlot.MakeTimeSeriePlot();
		
			m_pUtil->m_FileSave.m_SaveFile=prevSave;
			m_pUtil->m_FileSave.m_NotifySaveFile=prevNotify;



		};
		if(row==-1&&col==0) {
			GetCell(0,-1,&cell);
			txt=cell.GetText();




		}


	}

}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_RClicked
//		Sent whenever the user clicks the right mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  true if the button is down false if the button just when up
void ChartCug::OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top heading
//	
//		'col' is negative if the area clicked in is not valid
void ChartCug::OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_LClicked
//		Sent whenever the user clicks the left mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  true if the button is down false if the button just when up
void ChartCug::OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	BOOL Hit;
	Hit=false;
	int keep;

	int PrevTotNum;

	PrevTotNum=m_pProp->m_NumChartX*m_pProp->m_NumChartY;

	if ( !updn &&m_pUtil->m_RealTimeMode) {
		if(row==0) {
			if(col==-2&&m_RealTime.m_NumChartX>1)
				m_RealTime.m_NumChartX--;
			else if(col==-1&&m_RealTime.m_NumChartX<2)
				m_RealTime.m_NumChartX++;
			m_pSimDoc->m_AnnimChartX=m_RealTime.m_NumChartX;
		}
		else if(row==2) {
			if(col==-2&&m_RealTime.m_NumChartY>1)
				m_RealTime.m_NumChartY--;
			else if(col==-1&&m_RealTime.m_NumChartY<2)
				m_RealTime.m_NumChartY++;
			m_pSimDoc->m_AnnimChartY=m_RealTime.m_NumChartY;
		}
		OnUpdate();

	}
	else if(!updn) {

		if(row==0) {
			if(col==-2&&m_pProp->m_NumChartX>1)
				m_pProp->m_NumChartX--;
			else if(col==-1&&m_pProp->m_NumChartX<10)
				m_pProp->m_NumChartX++;

			Hit=true;

		}
		else if(row==2) {
			if(col==-2&&m_pProp->m_NumChartY>1)
				m_pProp->m_NumChartY--;
			else if(col==-1&&m_pProp->m_NumChartY<10)
				m_pProp->m_NumChartY++;

			Hit=true;

		}

		else if (row==4) {
			if(m_pProp->m_NumChartX>=1&&m_pProp->m_NumChartY>1) {
				if(m_pProp->m_NumChartY==2) {
					m_pProp->m_NumChartY=1;
					m_pProp->m_NumChartX=m_pProp->m_NumChartX*2;
				}
				else if(m_pProp->m_NumChartX==1&&m_pProp->m_NumChartY/2>1) {
					m_pProp->m_NumChartY=m_pProp->m_NumChartY/2;
					m_pProp->m_NumChartX=2;

				}
				else if(m_pProp->m_NumChartY>2) {
					keep=m_pProp->m_NumChartX;
					m_pProp->m_NumChartX=m_pProp->m_NumChartY;
					m_pProp->m_NumChartY=keep;
				}
				Hit=true;
			}


		}
		else if(row==3) {

			if(m_pProp->m_NumChartY>=1&&m_pProp->m_NumChartX>1) {
				if(m_pProp->m_NumChartX==2) {
					m_pProp->m_NumChartX=1;
					m_pProp->m_NumChartY=m_pProp->m_NumChartY*2;
				}
				else if(m_pProp->m_NumChartY==1&&m_pProp->m_NumChartX/2>1) {
					m_pProp->m_NumChartX=m_pProp->m_NumChartX/2;
					m_pProp->m_NumChartY=2;
				}
				else if(m_pProp->m_NumChartX>2) {
					keep=m_pProp->m_NumChartX;
					m_pProp->m_NumChartX=m_pProp->m_NumChartY;
					m_pProp->m_NumChartY=keep;
				}
				Hit=true;
			}
		



		}
		if(Hit) {
			m_pMainframe =AfxGetMainWnd();	
		//	((CMainFrame*)m_pMainframe)->m_wndModel.m_grid1.UpdateChartInfo();
			CWinApp* pApp = AfxGetApp();
			pApp->WriteProfileInt("CoupModel - KTH", "NumberChartX",m_pProp->m_NumChartX);
			pApp->WriteProfileInt("CoupModel - KTH", "NumberChartY",m_pProp->m_NumChartY);
			if(m_pProp->m_NumChartX*m_pProp->m_NumChartY>PrevTotNum)
				m_TimeSeriePlot.MakeTimeSeriePlot();
			else
				m_pUtil->m_FileSave.OnReDraw(false);
			OnUpdate();
			m_pUtil->m_ToBeUpdated=false;
		}

	}
		//SelectRange ( 0, row, GetNumberCols()-1, row );
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_RClicked
//		Sent whenever the user clicks the right mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  true if the button is down false if the button just when up
void ChartCug::OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_DClicked
//		Sent whenever the user double clicks the left mouse button within the side heading
//	
//		'row' is negative if the area clicked in is not valid
void ChartCug::OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_LClicked
//		Sent whenever the user clicks the left mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  true if the button is down false if the button just when up
void ChartCug::OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{

	if(m_pUtil->m_ToBeUpdated) {
			m_pUtil->m_FileSave.OnReDraw(true);
			m_pUtil->m_ToBeUpdated=false;
			QuickSetBackColor(-1,-1,m_ColorClick);
			RedrawCell(-1,-1);
	}

}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_RClicked
//		Sent whenever the user clicks the right mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  true if the button is down false if the button just when up
void ChartCug::OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top corner button
void ChartCug::OnCB_DClicked(RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyDown
//		Sent whenever the user types when the grid has focus. The keystroke can be
//		modified here as well. (See WM_KEYDOWN for more information)
void ChartCug::OnKeyDown(UINT *vcKey,BOOL processed)
{
	if ( *vcKey == VK_SPACE )
	{
//		m_GI->m_multiSelect->FloatingBlock( 0, 0, true );
	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnCharDown
//		Sent whenever the user types when the grid has focus. The keystroke can be 
//		modified here as well. (See WM_CHAR for more information)
void ChartCug::OnCharDown(UINT *vcKey,BOOL processed)
{
}
	
/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent everytime the grid needs to
//		draw a cell in the grid. At this point the cell
//		class has been filled with the information to be
//		used to draw the cell. The information can now be
//		changed before it is used for drawing
void ChartCug::OnGetCell(int col,long row,CUGCell *cell)
{
	/*cell->SetNumberDecimals ( 0 );

	 if ( col == -1 && row >= 0 )
		cell->SetNumber ( row );
	else if ( row == -1 && col >= 0 )
		cell->SetNumber ( col );*/
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetCell
//		This message is sent everytime the a cell is about to change.
void ChartCug::OnSetCell(int col,long row,CUGCell *cell)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDataSourceNotify
//		This message is sent from a data source , message
//		depends on the data source - check the information
//		on the data source(s) being used
//		- The ID of the Data source is also returned
void ChartCug::OnDataSourceNotify(int ID,long msg,long param)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellTypeNotify
//		This message is sent from a cell type , message
//		depends on the cell type - check the information
//		on the cell type classes
//		- The ID of the cell type is given
int ChartCug::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	CUGCell cell; 
	string Type, Prior,After, optionlist;
	int iopt, ivis, itype, ArrayIndex;
	CWinApp* pApp = AfxGetApp();
	BOOL updatedirect, reload=false;
	updatedirect=false;
	if(m_Mode==0&&ID==1&&msg==103) {
//
			updatedirect=true;
			GetCell(col,row,&cell);
			After=cell.GetText();
			optionlist=cell.GetLabelText();
			if(m_pUtil->m_RealTimeMode) {
				if(col==1) {
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
							m_pSimDoc->m_Simulator.SetRunDelay(0);
						else if (After=="Small Delay")
							m_pSimDoc->m_Simulator.SetRunDelay(1);
						else if (After=="Medium Delay")
							m_pSimDoc->m_Simulator.SetRunDelay(20);
						else if (After=="Large Delay")
							m_pSimDoc->m_Simulator.SetRunDelay(1000);
						else if (After=="Huge Delay")
							m_pSimDoc->m_Simulator.SetRunDelay(100000);

					}
					else if(row==3) {
						if(After=="RootMeanSquareError")
							m_pSimDoc->SetMonitor2View(0);
						else if (After=="Log Likelihood")
							m_pSimDoc->SetMonitor2View(1);




					}


				}


			}
			else {
				if(col==0&&row==3) {
					m_pProp->ResetXScale();
					m_pProp->ResetYLeftScale();
					m_pProp->ResetYRightScale();
					m_pProp->ResetZScale();
					if(After=="TimeSerie") {
						m_pProp->m_ChartType=1;
						m_pProp->SetFirstIndepSet(PGDEPTH,false);
						m_pProp->SetSecondIndepSet(DATEAXIS, false);
					}
					else if(After=="Scatter Chart")
						m_pProp->m_ChartType=0;
					else if(After=="Frequency Distribution") {
						m_pProp->m_ChartType=2;
						OnUpdate();
					}
					else if(After=="Cumulative Frequency Distribution") {
						m_pProp->m_ChartType=3;
						//m_pProp->m_ReShape.m_AddLine=true;
					}
					else if(After=="Contour Chart") {
						m_pProp->m_ChartType=4;
						if(m_pUtil->m_MultiRunPlot) {
							if(m_pProp->GetFirstIndepIndex()==m_pProp->GetSecondIndepIndex()&&m_pProp->GetSecondIndepIndex()==string::npos) 
								m_pProp->SetSecondIndepIndex(PARVAR,1);
							OnUpdate();


						}
						else {
							m_pProp->SetFirstIndepSet(PGDEPTH,true);
							m_pProp->SetSecondIndepSet(DATEAXIS, true);


						}
						//m_pProp->m_ReShape.m_AddLine=true;
					}


					m_pMainframe =AfxGetMainWnd();	
				//	((CMainFrame*)m_pMainframe)->m_wndModel.m_grid1.m_CurrentChartType=m_pProp->m_ChartType;

					m_pUtil->m_FileSave.OnReDraw(true);
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
				}
				else if(col==4) {  //Annimation
					if(row==2) {
						m_AnnimateStepValue=FUtil::GetOptionIndex(string(cell.GetLabelText()), After);
						OnChangeAnnimationSteps();

					}
					else if(row==3) {
						m_AnnimateValue=FUtil::GetOptionIndex(string(cell.GetLabelText()), After);
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
				else if(col==9) {
					if(row==1) {
						m_pProp->SetSecondIndepIndex(PARVAR,FUtil::GetOptionIndex(optionlist,After));

						m_TimeSeriePlot.LoadTimeSerieData();
						m_pZoomTimeSerie->OnChangeWindowWidth();
						m_pView->m_ChartViewer.setMouseUsage(Chart::MouseUsageScroll);
						m_TimeSeriePlot.DrawTimeSeriePlot(&m_pView->m_ChartViewer);
					}



				}
			}





	}
	else if(m_Mode==1&&ID==1&&msg==103) {
			updatedirect=true;
				GetCell(col,row,&cell);
				After=cell.GetText();

				if(row==4) {

					switch (col) {
						case 2: // Vertical
							if(After=="Horizontal"){
								m_pProp->SetVerticalLegends(false);
								pApp->WriteProfileInt("CoupModel - KTH","VerticalLegends" ,0 );
							}
							else {
								m_pProp->SetVerticalLegends(true);
								pApp->WriteProfileInt("CoupModel - KTH","VerticalLegends" ,1 );
							}
							break;
						case 3:
						case 4: // legend box

							ivis=ChartUtil::GetVisibleIndex(After);
							pApp->WriteProfileInt("CoupModel - KTH","VisibleFrames" ,ivis );
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
							pApp->WriteProfileInt("CoupModel - KTH", "Rounded frame",m_pProp->m_RoundedFrame);
							break;

						case 9:   // Independet colors subcharts
							if(After=="Common") {
								m_pProp->SetCommonColors(true);
							}
							else {
								m_pProp->SetCommonColors(false);
							}
							pApp->WriteProfileInt("CoupModel - KTH", "CommonColorsSubCharts",m_pProp->m_CommonColorForSubCharts);
							break;

					}


					m_pUtil->m_ToBeUpdated=true;
					QuickSetBackColor(-1,-1,m_ColorChanged);
					RedrawCell(-1,-1);

			}

				else if(row==6) {
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
							pApp->WriteProfileInt("CoupModel - KTH","ChartError" ,iopt);
							m_pProp->SetChartError(iopt);

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
							break;
						case 6:
							if(After=="Yes")
								iopt=0;
							else
								iopt=1;
							pApp->WriteProfileInt("CoupModel - KTH","RejectedPoints" ,iopt);
							m_pUtil->SetRejectedPoints(iopt);
							break;
						case 7:
							if(After=="Always")
								iopt=0;
							else if(After=="Sensitive")
								iopt=1;
							else
								iopt=2;
							pApp->WriteProfileInt("CoupModel - KTH","RemoveXAxisScale" ,iopt);
							m_pUtil->SetRemoveXAxisScale(iopt);
							break;

						case 8:
							if(After=="Always")
								iopt=0;
							else if(After=="Sensitive")
								iopt=1;
							else
								iopt=2;
							pApp->WriteProfileInt("CoupModel - KTH","RemoveYAxisScale" ,iopt);
							m_pUtil->SetRemoveYAxisScale(iopt);
							break;
						case 9:
							if(After=="Always")
								iopt=0;
							else if(After=="Sensitive")
								iopt=1;
							else
								iopt=2;
							pApp->WriteProfileInt("CoupModel - KTH","RemoveTitle" ,iopt);
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

	}
	else if(m_Mode==3&&ID==1&&msg==103) {
		updatedirect=true;
		if(col>0&&col<12) {
				int icurve;
				GetCell(8,row,&cell);
				icurve=cell.GetNumber()-1;
				GetCell(col,row,&cell);
				After=cell.GetText();
				switch (col) {
					case 2:
					iopt=m_pUtil->GetLineThickSizeIndex(After);
					m_pProp->SetLineWidth(icurve,iopt);
					break;
					case 3:
					iopt=m_pUtil->GetLineTypeIndex(After);
					m_pProp->SetDashModeLine(icurve,iopt);

					break;
					case 4:
					iopt=m_pUtil->GetSymbolIndex(After);
					m_pProp->SetSymbolStyle(icurve, iopt);

					break;		
					case 5:
					iopt=m_pUtil->GetSymbolSizeIndex(After);
					m_pProp->SetSymbolSize(icurve,iopt);

					break;
					case 7:
					if(After=="Y Left") {
						m_pProp->SetAxisLayer(row, 1);

					}
					else if(After=="Y Right") {
						m_pProp->SetAxisLayer(row, 2);

					}
					else if(After=="X axis"&&m_pSimDoc!=NULL){
						m_pProp->SetAxisLayer(row, 3);
						int ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumIndex, row);
						int subchart=m_pSimDoc->m_PlotStorage.GetSubChart(ArrayIndex)-1;
						int prev=m_pProp->GetXVariableNumber(subchart);
						m_pProp->SetAxisLayer(prev,1);
						m_pProp->SetXVariableNumber(subchart, row);
					}

					break;
					case 9:
						ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumIndex, row);
						itype=m_pSimDoc->m_PlotStorage.GetType(ArrayIndex);
						if(m_ResidualSource) {
							iopt=FUtil::GetOptionIndex(string(cell.GetLabelText()), After);
							//cell.SetNumber(iopt+1);
							if(itype>5) {
								if(After=="All")
									itype=15;
								else if(After=="Accepted")
									itype=25;
								else if(After=="Single Run")
									itype=5;
								m_pSimDoc->m_PlotStorage.SetChartType(ArrayIndex,itype);
							}
							else {
								if(After=="All") {
									itype=15;
									m_pSimDoc->m_PlotStorage.SetChartType(ArrayIndex,itype);
								}
								else if(After=="Accepted") {
									itype=25;
									m_pSimDoc->m_PlotStorage.SetChartType(ArrayIndex,itype);
								}
								else {
									iopt=FUtil::AtoInt(After);
									m_pSimDoc->m_PlotStorage.SetValRunNo(ArrayIndex, iopt-1);
								}
							}

						}
						else {
								if(After=="All") {
									if(itype>100&&itype<110)
										itype+=10;
									else if(itype>120&&itype<130)
										itype-=10;

									m_pSimDoc->m_PlotStorage.SetChartType(ArrayIndex,itype);
								}
								else if(After=="Accepted") {
									if(itype>100&&itype<110)
										itype+=20;
									else if(itype>110&&itype<120)
										itype+=10;

									m_pSimDoc->m_PlotStorage.SetChartType(ArrayIndex,itype);
								}
								else {
									iopt=FUtil::GetOptionIndex(string(cell.GetLabelText()), After);
									iopt=FUtil::AtoInt(After);
									cell.SetNumber(iopt+1);
									m_pSimDoc->m_PlotStorage.SetValRunNo(ArrayIndex, iopt);
									if(itype>120)
										itype-=20;
									else if(itype>110)
										itype-=10;
									m_pSimDoc->m_PlotStorage.SetChartType(ArrayIndex,itype);

								}
						}
						m_pProp->Reset();
						m_pUtil->m_FileSave.OnReDraw(true);
						return true;
					break;
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
							int ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumIndex, row);
							m_pSimDoc->m_PlotStorage.SetLogLiVersion(ArrayIndex, iopt);

						}
						m_pProp->Reset();
						m_pUtil->m_FileSave.OnReDraw(true);
						return true;
					break;

				}
				//if(updatedirect)
					m_pUtil->m_FileSave.OnReDraw(false);
				/*else {
					m_pUtil->m_ToBeUpdated=true;
					QuickSetBackColor(-1,-1,m_ColorChanged);
					RedrawCell(-1,-1);
				}*/

		}


	}

	else if(m_Mode==4&&ID==1&&msg==103) {

			if(col>1&&col<3) {
				GetCell(col,row,&cell);
				After=cell.GetText();
				switch (col) {
					case 2:
						if(After=="Linear") {
							iopt=0;
						}
						else 
							iopt=1;
						m_pProp->SetXDistLog(row,iopt);
						m_pProp->SetXDistChanged(row, true);
						break;

				}

				m_pUtil->m_ToBeUpdated=true;
				QuickSetBackColor(-1,-1,m_ColorChanged);
				RedrawCell(-1,-1);

		}


	}

	else if(m_Mode==7&&ID==1&&msg==103) {
			if(col==0) {
				GetCell(col,row,&cell);
				After=cell.GetText();
				if(After=="on") {
					m_TimeSeriePlot.m_BoxCox.use=true;
				}
				else {
					m_TimeSeriePlot.m_BoxCox.use=false;
				}
				m_pUtil->m_FileSave.OnReDraw(reload);

			}

	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditStart
//		This message is sent whenever the grid is ready to start editing a cell
//		A return of true allows the editing a return of false stops editing
//		Plus the properties of the CEdit class can be modified
int ChartCug::OnEditStart(int col, long row,CWnd **edit)
{
	int start;
	start=3+m_NumSubCharts;
	if(m_pProp->m_YLabel2.at(0)!="-")
		start=3+2*m_NumSubCharts;
	int n1=m_pProp->m_Legend1.size();
	if(m_Mode==0&&m_pUtil->m_ParameterPlotMode&&col==2)
		return true;
	else if(m_Mode==1) {
		if(row%2==0) return true;

	}
	else if(m_Mode==2) {
		return true;
	}
	else if(m_Mode==3&&col==0) {
		return true;
	}
	else if(m_Mode==4||m_Mode==7) {
		return true;
	}




	return false;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditVerify
//		This is send when the editing is about to end
int ChartCug::OnEditVerify(int col, long row,CWnd *edit,UINT *vcKey){
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int ChartCug::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag)
{

	CString str;
	float pV;
	BOOL RightLabels, XLabels;
	int Index;
	int value;
	RightLabels=false;
	XLabels=false;
	CWinApp* pApp = AfxGetApp();
	edit->GetWindowText(str);
	int n1=m_pProp->m_Legend1.size();

	BOOL updatedirect;
	BOOL reload=false;
	updatedirect=false;

	if(m_Mode==0&&m_pUtil->m_ParameterPlotMode) {
		float value;
		if(col==2) {
			size_t layer, i;
			if(m_ParameterPlot.m_plotp.Opt_Vec.size()>0) {
				auto ll=row%m_ParameterPlot.m_plotp.Opt_Vec.size();
				layer=m_ParameterPlot.m_plotp.Opt_Vec[ll];
				i=row/m_ParameterPlot.m_plotp.Opt_Vec.size();
			}
			else {
				layer=0;
				i=row;
			}
			value=MFC_Util::AtoFloat(str);
			m_ParameterPlot.m_PreviousValue[i][layer]=m_ParameterPlot.m_NewPlotValue[i][layer];
			if(m_ParameterPlot.m_PreviousValue[i][layer]==m_ParameterPlot.m_NewPlotValue[i][layer]) {
				QuickSetNumber(4, row, m_ParameterPlot.m_PreviousValue[i][layer]);
				QuickSetBackColor(4, row, m_ColorChanged);
			}

			m_ParameterPlot.m_NewPlotValue[i][layer]=value;
			QuickSetBackColor(col, row, m_ColorChanged);
			if(m_ParameterPlot.m_PlotNewValue)
				 m_ParameterPlot.OnInit();
			m_pUtil->m_FileSave.m_NoChange=false;
		}
	}

	else if(m_Mode==1) { // General Layout
		updatedirect=true;
		if(row==0) {
		switch(col) {
			case 0:
				m_pProp->m_Title=str;
			break;
			case 1:
				m_pUtil->m_TitleSizePoint=MFC_Util::AtoInt(str);
				pApp->WriteProfileInt("CoupModel - KTH", "TitleSize",m_pUtil->m_TitleSizePoint );
			break;
			case 2:
				m_pUtil->m_LabelSizePoint=MFC_Util::AtoInt(str);
				pApp->WriteProfileInt("CoupModel - KTH", "LabelSize", m_pUtil->m_LabelSizePoint);
			break;
			case 3:
				m_pUtil->m_LegendSizePoint=MFC_Util::AtoInt(str);
				pApp->WriteProfileInt("CoupModel - KTH", "LegendSize",m_pUtil->m_LegendSizePoint);
			break;
			case 4:
				m_pUtil->m_LineWidthPoint=MFC_Util::AtoInt(str);
				pApp->WriteProfileInt("CoupModel - KTH", "LineWidth", m_pUtil->m_LineWidthPoint);
			break;
			case 5:
				m_pUtil->m_LineAxisWidthPoint=MFC_Util::AtoInt(str);
				pApp->WriteProfileInt("CoupModel - KTH", "LineAxisWidth",m_pUtil->m_LineAxisWidthPoint);
			break;
			case 6:
				m_pUtil->m_SymbolSizePoint=MFC_Util::AtoInt(str);
				pApp->WriteProfileInt("CoupModel - KTH", "SymbolSize", m_pUtil->m_SymbolSizePoint);
			break;
			case 7:
				m_pUtil->m_AxisScalePoint=MFC_Util::AtoInt(str);
				pApp->WriteProfileInt("CoupModel - KTH", "AxisScaleSize", m_pUtil->m_AxisScalePoint);
			break;
			case 8:
				m_pUtil->m_MinTickDistX=MFC_Util::AtoInt(str);
				pApp->WriteProfileInt("CoupModel - KTH", "AxisMinTickX", m_pUtil->m_MinTickDistX);
				
			break;
			case 9:
				m_pUtil->m_MinTickDistY=MFC_Util::AtoInt(str);
				pApp->WriteProfileInt("CoupModel - KTH", "AxisMinTickY", m_pUtil->m_MinTickDistY);
			break;
		}
		}
		else if(row==2) {

			
			switch(col) {
				case 0:
					m_pProp->m_cx=MFC_Util::AtoInt(str)+180;
					pApp->WriteProfileInt("CoupModel - KTH", "DisplaySize_cx", m_pProp->m_cx);
//					((CZoomscrolldemoDlg*)m_pDlg)->OnInitDialog();
					updatedirect=false;
					break;
				case 1:
					m_pProp->m_cy=MFC_Util::AtoInt(str)+28;
					pApp->WriteProfileInt("CoupModel - KTH", "DisplaySize_cy", m_pProp->m_cy);
	//				((CZoomscrolldemoDlg*)m_pDlg)->OnInitDialog();
					updatedirect=false;
					break;
				case 2:
					m_pProp->m_TopMargin=MFC_Util::AtoInt(str);
					pApp->WriteProfileInt("CoupModel - KTH", "TopMargin",m_pProp->m_TopMargin);
					
					break;
				case 3:
					m_pProp->m_BottomMargin=MFC_Util::AtoInt(str);
					pApp->WriteProfileInt("CoupModel - KTH", "BottomMargin",m_pProp->m_BottomMargin);
					break;
				case 4:
					m_pProp->m_LeftMargin=MFC_Util::AtoInt(str);
					pApp->WriteProfileInt("CoupModel - KTH", "LeftMargin",m_pProp->m_LeftMargin);
					break;
				case 5:
					m_pProp->m_RightMargin=MFC_Util::AtoInt(str);
					pApp->WriteProfileInt("CoupModel - KTH", "RightMargin",m_pProp->m_RightMargin);
					break;
				case 6:
					m_pProp->m_GapMargin=MFC_Util::AtoInt(str);
					pApp->WriteProfileInt("CoupModel - KTH", "GapMargin",m_pProp->m_GapMargin);
					break;
				case 7:
					value=MFC_Util::AtoInt(str);
					m_pUtil->SetGapHorPercentage(value);
					break;
				case 8:
					value=MFC_Util::AtoInt(str);
					m_pUtil->SetGapHorLeftPercentage(value);
					break;
				case 9:
					value=MFC_Util::AtoInt(str);
					m_pUtil->SetGapVertTopPercentage(value);
					break;
				}

	
		}
		else if (row==4) {


			switch(col) {	
				case 0:
					m_pUtil->m_LegendPositionX=MFC_Util::AtoInt(str);
					pApp->WriteProfileInt("CoupModel - KTH", "LegendPositionX",m_pUtil->m_LegendPositionX);
					break;
				case 1:
					m_pUtil->m_LegendPositionY=MFC_Util::AtoInt(str);
					pApp->WriteProfileInt("CoupModel - KTH", "LegendPositionY",m_pUtil->m_LegendPositionY);
					break;
			}

		}
		else if (row==6) {


			switch(col) {	
				case 0:
					m_pProp->m_BoxDimmer=MFC_Util::AtoInt(str);
					pApp->WriteProfileInt("CoupModel - KTH", "BoxDimmer",m_pProp->m_BoxDimmer);
					break;
				case 1:
					m_pProp->m_BoxRangeDimmer=MFC_Util::AtoInt(str);
					pApp->WriteProfileInt("CoupModel - KTH", "BoxRangeDimmer",m_pProp->m_BoxRangeDimmer);
					break;
			}

		}





	}
	else if(m_Mode==2&&m_pUtil->m_ParameterPlotMode) {
		Func *funcpointer=m_ParameterPlot.GetCurrentFuncPointer();
		PLOT_PARAM plotparam=funcpointer->GetPlotParam();
		if(row<=1) {
			switch(col) {
		case 1:
			if(row==0) plotparam.X_Label=str;
			else if(row==1) plotparam.Y_Label=str;
			break;
		case 3: if(row==0) plotparam.X_Min=MFC_Util::AtoFloat(str);	break;
		case 4: if(row==0) plotparam.X_Max=MFC_Util::AtoFloat(str);	break;



		}
		}
		else {
			plotparam.LegendsVec[row-2]=str;


		}

		funcpointer->SetPlotParam(plotparam);
		m_ParameterPlot.OnInit();
	}
	else if(m_Mode==2) { //Axis
		if(m_pUtil->IsRightAxisExist()) {
			RightLabels=true;
		}
		if(m_pUtil->IsXAxisExist())
			XLabels=true;
		updatedirect=true;		

		
		if((row>=0&&m_pProp->m_YScaleLeftChanged.size()>row&&row<m_pProp->m_YScaleLeftMax.size()&&!m_pProp->IsSecondIndepSet())
			||(row==0&&m_pProp->IsSecondIndepSet())) {
				Index=row;
				int n;
				switch(col) {
					case 0:
						m_pProp->m_YLabel1[Index]=str;
						m_pProp->m_YLabel1Change[Index]=true;
						break;
					case 1:
						m_pProp->m_YScaleLeftChanged[Index]=true;
						pV=MFC_Util::AtoFloat(str);
						m_pProp->m_YScaleLeftMin[Index]=pV;

						break;
					case 2:
						n=m_pProp->m_YScaleLeftMax.size();
						if(Index<n) {
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_YScaleLeftMax[Index]=pV;
							m_pProp->m_YScaleLeftChanged[Index]=true;

						}
						break;
					case 3:
						pV=MFC_Util::AtoFloat(str);
						m_pProp->m_YScaleLeftInterv[Index]=pV;
						m_pProp->m_YScaleLeftChanged[Index]=true;
						break;


		
				}

		}
		else if(row>=m_NumSubCharts&&RightLabels&&!m_pProp->IsSecondIndepSet()) {
				Index=row-(m_NumSubCharts);
				if(Index<m_pProp->m_YLabel2.size()) {
					switch(col) {
						case 0:
							m_pProp->m_YLabel2[Index]=str;
							m_pProp->m_YLabel2Change[Index]=true;
							break;
						case 1:
							m_pProp->m_YScaleRightChanged[Index]=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_YScaleRightMin[Index]=pV;
							break;
						case 2:
							m_pProp->m_YScaleRightChanged[Index]=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->SetYRightmax(Index,pV);
							break;
						case 3:
							pV=m_pProp->m_YScaleRightInterv.at(Index);
							m_pProp->m_YScaleRightInterv[Index]=pV;
							m_pProp->m_YScaleRightChanged[Index]=true;
							break;
					}
				}

		}
		else if((row>=m_NumSubCharts&&XLabels&&row-m_NumSubCharts<m_pProp->m_XScaleMin.size()&&!m_pProp->IsSecondIndepSet())
			||(row==1&&m_pProp->IsSecondIndepSet())) {
				long min;
				if(RightLabels)
					Index=row-(2*m_NumSubCharts);
				else if(!m_pProp->IsSecondIndepSet())
					Index=row-(m_NumSubCharts);
				else
					Index=0;
				switch(col) {
					case 0:
						m_pProp->m_XLabel[Index]=str;
						m_pProp->m_XLabelChange[Index]=true;
						break;
					case 1:

						m_pProp->m_XScaleChanged[Index]=true;
						if(m_pProp->m_ChartType!=1||m_pProp->m_ReShape.m_NormalDist||m_pProp->m_ReShape.m_SortedSeries) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_XScaleMin[Index]=pV;
						}
						else if(m_pUtil->m_MultiRunPlot) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_XScaleMin[Index]=pV;

						}
						else			
						{
							min=PGUtil::MinutConv(str.GetString())-2880;
							m_pProp->m_TimeScale.min=min*double(60);
						}
						break;
					case 2:

						m_pProp->m_XScaleChanged[Index]=true;
						if(m_pProp->m_ChartType!=1||m_pProp->m_ReShape.m_NormalDist||m_pProp->m_ReShape.m_SortedSeries) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_XScaleMax[Index]=pV;
						}
						else if(m_pUtil->m_MultiRunPlot) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_XScaleMax[Index]=pV;
						
						}
						else {
							min=PGUtil::MinutConv(str.GetString())-2880;
							m_pProp->m_TimeScale.max=min*60.;
						}
						break;
					case 3:
						m_pProp->m_XScaleChanged[Index]=true;
						if(m_pProp->m_ChartType!=1||m_pProp->m_ReShape.m_NormalDist||m_pProp->m_ReShape.m_SortedSeries) {
							reload=true;
				
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_XScaleInterv[Index]= pV;
						}
						else if(m_pUtil->m_MultiRunPlot) {
							reload=true;
	
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_XScaleInterv[Index]= pV;

						}
						else
							m_pProp->m_TimeScale.interv=MFC_Util::AtoFloat(str)*double(60);

						break;
				}
				
		}
		else if(m_pProp->IsSecondIndepSet()) {

				
				Index=row-2;
				switch(col) {
					case 0:
						m_pProp->m_ZLabel[Index]=str;
						m_pProp->m_ZLabelChange[Index]=true;
						break;
					case 1:

						m_pProp->m_ZScaleChanged[Index]=true;
						if(m_pProp->m_ChartType!=1||m_pProp->m_ReShape.m_NormalDist||m_pProp->m_ReShape.m_SortedSeries) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_ZScaleMin[Index]=pV;
						}
						else if(m_pUtil->m_MultiRunPlot) {
							reload=true;
							pV=m_pProp->m_ZScaleMin.at(Index);
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_ZScaleMin[Index]=pV;

						}

						break;
					case 2:

						m_pProp->m_ZScaleChanged[Index]=true;
						if(m_pProp->m_ChartType!=1||m_pProp->m_ReShape.m_NormalDist||m_pProp->m_ReShape.m_SortedSeries) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_ZScaleMax[Index]=pV;
						}
						else if(m_pUtil->m_MultiRunPlot) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_ZScaleMax[Index]=pV;
						
						}

						break;
					case 3:
						m_pProp->m_ZScaleChanged[Index]=true;
						if(m_pProp->m_ChartType!=1||m_pProp->m_ReShape.m_NormalDist||m_pProp->m_ReShape.m_SortedSeries) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_ZScaleInterv[Index]=pV;
						}
						else if(m_pUtil->m_MultiRunPlot) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_ZScaleInterv[Index]=pV;


						}
						break;
				}

		}
	}
	else if(m_Mode==3) {
				
		if(row>=0&&row<n1) {
			updatedirect=true;
			m_pProp->m_Legend1[row]= str;
			m_pProp->m_Legend1Change[row]=true;
		}
		else 
			return false;

	}


	else if(m_Mode==4) {
		updatedirect=false;
		if(col==3) {
			pV=m_pProp->m_XDistMin.at(row);
			pV=MFC_Util::AtoFloat(str);
			m_pProp->m_XDistChanged[row]=true;

		}
		else if(col==4) {
			pV=MFC_Util::AtoFloat(str);
			m_pProp->m_XDistChanged[row]=true;
		}
		else if(col==5) {
			int num=MFC_Util::AtoInt(str);
			for (int il=0;il<n1;il++) {
				QuickSetNumber(col,il,num);
				RedrawCell(col, il);
				m_pProp->SetXDistSteps(il, num);
				m_pProp->m_XDistChanged[il]=true;
			}
		}
		else 
			return false;

	}

	else if(m_Mode==7) {
		float value;
		if(col==1) {
			value=MFC_Util::AtoFloat(str);
			m_TimeSeriePlot.m_BoxCox.box_power=value;
		}
		else if(col==2) {
			value=MFC_Util::AtoFloat(str);
			m_TimeSeriePlot.m_BoxCox.box_offset=value;
		}
		updatedirect=true;
		reload=true;
		m_pProp->m_XDistSet[0]= false;

	}
		if(updatedirect)
				m_pUtil->m_FileSave.OnReDraw(reload);
		else {
	//QuickSetText(col,row,str);
			m_pUtil->m_ToBeUpdated=true;
			QuickSetBackColor(-1,-1,m_ColorChanged);
			RedrawCell(-1,-1);
		}

	//((CZoomscrolldemoDlg*)m_pDlg)->OnReDraw();
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int ChartCug::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	sections - UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//				UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
void ChartCug::OnMenuCommand(int col,long row,int section,int item)
{
	CWinApp* pApp = AfxGetApp();
	int  index;
	BOOL RightLabels;
	BOOL updatedirect, reload;
	updatedirect=true;
	reload=false;

	if(m_pUtil->IsRightAxisExist()) {
		RightLabels=true;
	}

	CUGCell cell; 
	BOOL changemade;
	changemade=false;
	if(section==UG_CORNERBUTTON||(section==UG_GRID&&item>=5030&&item<=5031)) {
		if(item>=5020&&item<=5021) {
			MakeDataSheet(item-5020);
			m_CreatePG=true;
		}
		else if(item>=5030&&item<7000) {
			CString pszFileName ;
			LPCTSTR lpszDefExt="bin";
			static char BASED_CODE szFilter[] = "PG Bin file (*.BIN)|*.BIN||";
			CFileDialog	BinFileDlg(false,lpszDefExt="bin","NewFileName.BIN",OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,szFilter);
			if(BinFileDlg.DoModal()) {
				pszFileName = BinFileDlg.GetPathName();
				m_PGExtra.ReCalculateMinMax();
				for(int i=0; i<m_PGExtra.GetNumVariables();i++) {
					//m_PGExtra.SetVarName(i+1,m_Legend1.at(i));
					//m_PGExtra.SetVarUnit(i+1,);
				//	m_PGExtra.SetVarId(i+1,);
				}
				if(m_PGExtra.SaveAs(string(pszFileName), true)) {
					m_ToSavePGStruc=false;
					m_CreatePG=false;
				}
				m_PGExtra.CloseFile();
				::MessageBox(NULL, "PG file created"+pszFileName,"File Created",MB_OK);
			}
		}
		else {
			m_Mode=item-7000;
			OnUpdate();
		}
	}
	else  {

		if(item== 1000)
			CutSelected();
		else if(item==2000)
			CopySelected();
		else if(item==3000) 
			Paste();
		





		if(item>7000&&item<7004) {
			m_Mode=item-7000;
			OnUpdate();
		}
		else if(item==2401) {
			if(!m_pProp->m_RemoveAxisLabel) {
				m_pProp->m_RemoveAxisLabel=true;
             }
			else
				m_pProp->m_RemoveAxisLabel=false;
			changemade=true;
		}
		else if(item==2402) {
			if(m_pUtil->m_RemoveLegends==0) {
				m_pUtil->m_RemoveLegends=1;
			}
			else if(m_pUtil->m_RemoveLegends==1)
				m_pUtil->m_RemoveLegends=2;
			else
				m_pUtil->m_RemoveLegends=0;	
			changemade=true;
		}
		else if(item>=1000&&item<1032&&m_pSimDoc!=NULL) {
		/*	m_pSimDoc->m_PlotStorage.Remove(item-1000);
			index=row;
			if(m_Mode==3&&m_pDoc!=NULL) {
				m_Legend1.RemoveAt(index);
				m_Legend1Change.RemoveAt(index);
				if(m_pSimDoc->m_PlotStorage.GetNumVar(m_ChartNumIndex)<2)
					m_pProp->m_YLabel2.SetAt(0,"-");
				if(m_pSimDoc->m_PlotStorage.GetNumVar(m_ChartNumIndex)<1)
					m_YLabel1.SetAt(0,"-");
				changemade=true;
			}*/
		}
		else if(item==1500) {
			index=row;
			if(m_Mode==3) {
			if(m_pProp->m_PointType[index]) 
				m_pProp->m_PointType[index]=false;		
			else 
				m_pProp->m_PointType[index]=true;
			changemade=true;
			}
		}
		else if(item==2000) {
			if(m_Mode==1) {
				int colb=AfxGetApp()->GetProfileInt("CoupModel - KTH", "ColorBackground", 0xf0f0ff);;
				CColourPickerDlg Dlgcol(colb);
				if(Dlgcol.DoModal()==IDOK) {
						COLORREF iv=Dlgcol.GetColour();	
						m_pUtil->SetColorBackground(iv);
				}
				QuickSetBackColor(7,4,m_pUtil->GetColorBackground());
				RedrawCell(7,4);
				changemade=true;
			}
			else if(m_Mode==3) {
				GetCell(8,row,&cell);
				int ic=cell.GetNumber();
				CColourPickerDlg Dlgcol(MFC_Util::GetColorLine(ic));
				if(Dlgcol.DoModal()==IDOK) {
					COLORREF iv=Dlgcol.GetColour();	
					MFC_Util::SetColorLine(ic,iv);
				}
				QuickSetBackColor(1,row,MFC_Util::GetColorLine(ic));
				RedrawCell(1,row);
				changemade=true;
			}
		}
		else if(item==2001) {
			if(m_Mode==1) {
				int colb=AfxGetApp()->GetProfileInt("CoupModel - KTH", "ColorLegendBackground", 0xf0f0ff);;
				CColourPickerDlg Dlgcol(colb);
				if(Dlgcol.DoModal()==IDOK) {
						COLORREF iv=Dlgcol.GetColour();	
						m_pUtil->SetColorLegendBackground(iv);
				}
				changemade=true;
				QuickSetBackColor(3,4,m_pUtil->GetColorLegendBackground());
				RedrawCell(3,4);
			}

		}
		else if(item==2002) {
			if(m_Mode==1) {
				int colb=AfxGetApp()->GetProfileInt("CoupModel - KTH", "ColorLegendBackgroundFrame", 0xf0f0ff);;
				CColourPickerDlg Dlgcol(colb);
				if(Dlgcol.DoModal()==IDOK) {
						COLORREF iv=Dlgcol.GetColour();	
						m_pUtil->SetColorLegendBackgroundFrame(iv);
				}
				changemade=true;
				QuickSetBackColor(4,4,m_pUtil->GetColorLegendBackgroundFrame());
				RedrawCell(4,4);
			}
		}
		else if(item==2003) {
			if(m_Mode==1) {
				int colb=m_pUtil->GetColorBoxEdge();
				CColourPickerDlg Dlgcol(colb);
				if(Dlgcol.DoModal()==IDOK) {
						COLORREF iv=Dlgcol.GetColour();	
						m_pUtil->SetColorBoxEdge(iv);
				}
				changemade=true;
				QuickSetBackColor(2,6,m_pUtil->GetColorBoxEdge());
				RedrawCell(2,6);
			}
		}
		else if(item==2004) {
			if(m_Mode==4) {
				int colb=m_pUtil->GetColorDist();
				CColourPickerDlg Dlgcol(colb);
				if(Dlgcol.DoModal()==IDOK) {
						COLORREF iv=Dlgcol.GetColour();	
						m_pUtil->SetColorDist(iv);
				}
				changemade=true;
				QuickSetBackColor(col,row,m_pUtil->GetColorDist());
				RedrawCell(1,row);
			}
		}
		else if(item==2410) {
			int j=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, row);
			if(m_pSimDoc->m_PlotStorage.Remove(j)) {
				m_pProp->Reset();
				m_pUtil->m_FileSave.OnReDraw(true);
			}
		}
		else if(item==2411) {
			// Add new Curve with new realisation
			m_pSimDoc->m_PlotStorage.AddCurve();
			if(m_PGRepit) {

				int il=m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber)-1;
				int j=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, il);	
				int irep=m_pSimDoc->m_PlotStorage.GetPGRepitNo(j);
				CPG *pPG=(CPG*)m_pSimDoc->m_PlotStorage.GetPtr(j);
				long nrep=pPG->GetNumRepititions();
				irep++;
				irep=(irep-1)%nrep+1;
				m_pSimDoc->m_PlotStorage.SetPGRepitNo(j, irep);
				m_pUtil->m_FileSave.OnReDraw(true);
			}
			else if(m_ResidualSource) {
				int il=m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber)-1;
				int j=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, il);	
				int LogLi=m_pSimDoc->m_PlotStorage.GetLogLiVersion(j);
				if(LogLi==3)
					LogLi=2;
				else if(LogLi==2)
					LogLi=3;
				else if(LogLi==0)
					LogLi=2;
				int itype=m_pSimDoc->m_PlotStorage.GetType(j);
				if(itype!=35) {
					m_pSimDoc->m_PlotStorage.SetLogLiVersion(j, LogLi);
				}
				m_pUtil->m_FileSave.OnReDraw(true);

			}

		}
		else if(item==2412) {
			int il=m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber)-1;
			m_pSimDoc->m_PlotStorage.AddCurve(0,il);
			if(m_ResidualSource) {
				il=m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber)-1;
				int j=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, il);	
				int ResRunNo=m_pSimDoc->m_PlotStorage.GetValRunNo(j);
				ResRunNo=ResRunNo%m_pSimDoc->MR_Get_TotalNumberofRuns()+1;
				if(il>2) {
					int jj=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, il-1);	
					if(ResRunNo<=m_pSimDoc->m_PlotStorage.GetValRunNo(jj))
						ResRunNo=m_pSimDoc->m_PlotStorage.GetValRunNo(jj)+1;
				}


				int itype=m_pSimDoc->m_PlotStorage.GetType(j);
				if(itype!=35) {

					m_pSimDoc->m_PlotStorage.SetValRunNo(j, ResRunNo);
				}
				m_pUtil->m_FileSave.OnReDraw(true);

			}


			// Add new curve with new run no


		}
		else if(item>=3000&&item<=4000) {
		int ans=GotoCell(item-2999,0);
			m_colpos=item-2999;

			RedrawAll();
		}
		else if(item>4000&&item<5000) {
			int itype=item-4001;
			if(itype<10&&m_pSimDoc!=NULL) {
				m_pSimDoc->m_PlotStorage.SetChartType(m_pProp->m_ChartNumIndex, itype);
				if(m_pProp->SetChartType(itype)){
					changemade=true;
					reload=true;
				}
			}
			else if(itype<20) {
				GotoCell(itype-10,2);
				RedrawCell(itype-10,2);
			}
			else if(itype==20) {
				if(m_pProp->m_RoundedFrame)
					m_pProp->m_RoundedFrame=false;
				else
					m_pProp->m_RoundedFrame=true;
				pApp->WriteProfileInt("CoupModel - KTH", "Rounded frame",m_pProp->m_RoundedFrame);
		
			}
			else if(itype==21) {
				if(m_pProp->m_CommonColorForSubCharts)
					m_pProp->m_CommonColorForSubCharts=false;
				else
					m_pProp->m_CommonColorForSubCharts=true;
				pApp->WriteProfileInt("CoupModel - KTH", "CommonColorsSubCharts",m_pProp->m_CommonColorForSubCharts);
		
			}
			else if(item==4100) {
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
		else if(item>=5000&&item<=5100&&m_pSimDoc!=NULL){
			int ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumIndex, 0);
			if(item==5000)
				m_pSimDoc->m_PlotStorage.RemoveAllSectionCurves();
			m_pSimDoc->m_PlotStorage.AddCurve(5000);
			if(item==5000&&m_pSimDoc!=NULL) 
				m_pSimDoc->m_PlotStorage.Remove(ArrayIndex);
			changemade=true;

		}
	}
	if(changemade) {
		if(updatedirect&&!reload&&!m_pUtil->m_ParameterPlotMode)
				m_pUtil->m_FileSave.OnReDraw(true);
		else if(m_pUtil->m_ParameterPlotMode)
			m_ParameterPlot.OnInit();
		else if(updatedirect)
				m_pUtil->m_FileSave.OnReDraw(true);
		if(!m_pUtil->m_ToBeUpdated) {
			m_pUtil->m_ToBeUpdated=true;
		//	QuickSetBackColor(-1,-1,m_ColorChanged);
		//	RedrawCell(-1,-1);
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
//	return UG_SUCCESS to allow the menu to appear
//	return 1 to not allow the menu to appear
int ChartCug::OnMenuStart(int col,long row,int section)
{
	int  index;
	CString str;
	BOOL RightLabels, XLabels;

	if(m_pUtil->IsRightAxisExist()) {
		RightLabels=true;
	}
	if(m_pUtil->IsXAxisExist()) {
		XLabels=true;
	}


	CWinApp* pApp = AfxGetApp();
	m_pUtil->m_TitleSizePoint=pApp->GetProfileInt("CoupModel - KTH", "TitleSize", 12);
	m_pUtil->m_LabelSizePoint=pApp->GetProfileInt("CoupModel - KTH", "LabelSize", 9);
	m_pUtil->m_LegendSizePoint=pApp->GetProfileInt("CoupModel - KTH", "LegendSize", 9);
	m_pUtil->m_AxisScalePoint=pApp->GetProfileInt("CoupModel - KTH", "AxisScaleSize", 9);

	m_pUtil->m_LineWidthPoint=pApp->GetProfileInt("CoupModel - KTH", "LineWidth", 2);
	m_pUtil->m_LineAxisWidthPoint=pApp->GetProfileInt("CoupModel - KTH", "LineAxisWidth", 2);
	m_pUtil->m_SymbolSizePoint=pApp->GetProfileInt("CoupModel - KTH", "SymbolSize", 9);
	m_pUtil->m_MinTickDistX=pApp->GetProfileInt("CoupModel - KTH", "AxisMinTickX",60); 
	m_pUtil->m_MinTickDistY=pApp->GetProfileInt("CoupModel - KTH", "AxisMinTickY",60); 
	if(m_pSimDoc!=NULL) {
		int ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumIndex, 0);
		int ChartType=m_pSimDoc->m_PlotStorage.GetChartType(ArrayIndex);
	}

	EmptyMenu();

	//****** Populate the Menu 
	if(section==UG_CORNERBUTTON) {
		if(m_Mode==6) {
			
			return true;

		}

	}
	else if(section==UG_GRID) {


		if(m_Mode==1) {

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
		}
		else if(m_Mode==2) {
				if(!m_pProp->m_RemoveAxisLabel)
					AddMenuItem(2401,"Remove Labels on axis");
				else
					AddMenuItem(2401,"Show Labels on axis");

			
			
		}
		else if(m_Mode==3&&m_pSimDoc!=NULL) {
				index=row;
				if(m_pUtil->m_RemoveLegends==0)
					AddMenuItem(2402,"Remove Repeated Legends");
				else if(m_pUtil->m_RemoveLegends==1)
					AddMenuItem(2402,"Remove All Legends");
				else
					AddMenuItem(2402,"Show Legends");
				int ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumIndex, index);
				if(ArrayIndex>=0) {
					void *Ptr=m_pSimDoc->m_PlotStorage.GetPtr(ArrayIndex);
					int itype=m_pSimDoc->m_PlotStorage.GetType(ArrayIndex);
					SimB *pX=NULL;
					CPG *pPG1=NULL;
					if(itype==10) 
						pPG1=(CPG*)Ptr;
					else if(itype==0) 
						pPG1=(CPG*)((F*)Ptr)->GetPointer();
					else {
						pX=(SimB*)Ptr;	
						int idoc=m_pSimDoc->m_PlotStorage.GetDocIndex(ArrayIndex);
						pPG1=m_pSimDoc->m_PlotStorage.GetPGPointer(idoc);
					}
					AddMenuItem(2410,"Remove Curve from selection");
					itype=m_pSimDoc->m_PlotStorage.GetType(ArrayIndex);
					if(itype==10&&m_PGRepit) {
						AddMenuItem(2411,"Add New Curve with Repitition of variable");
					}
					else if(itype!=35&&itype%10==5) {
						AddMenuItem(2411,"Add New Curve with New Realisation of simulation");
						AddMenuItem(2412,"Add New Curve with New Multi RunNo");
					}

				}
				str=MFC_Util::ItoAscii(index+1);
				AddMenuItem(2000,"Change Color var no: "+str);
					

				if(m_pProp->m_ReShape.m_CommonDayNo) {
					AddMenuItem(5000,"Substitute all curves with Mean value of years");
					AddMenuItem(5001,"Add Mean value of years");
				}

		}

		else if(m_Mode==4) {
			 if(col==1)
					AddMenuItem(2004,"Change background color of extra variable in distribution chart");
		}
		else if(m_Mode>4) {
			AddMenuItem(1000, "Cut"); 
			AddMenuItem(2000, "Copy"); 
			AddMenuItem(3000, "Paste"); 
			if(m_ToSavePGStruc) {
				AddMenuItem(5030,"Save As PG File");
			}
		}
	}

	if(m_pUtil->m_ToBeUpdated) {
				AddMenuItem(4100,"Update Chart to apply new setting");

			}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHint
int ChartCug::OnHint(int col,long row,int section,CString *string)
{

	BOOL RightLabels;
	int start;
	start=3+m_NumSubCharts;
	CString txt;
	BOOL Hit;
	Hit=false;
	if(m_pUtil->IsRightAxisExist()) {
		start=3+2*m_NumSubCharts;
		RightLabels=true;
	}
	if(col==-1&&row==-1&&m_pUtil->m_ToBeUpdated) {
		string->Format("Single click here to update Chart with new settings");
		return true;	
	}
	else if (col==-1) {
		if(row==1&&m_pProp->m_NumChartX>=1&&m_pProp->m_NumChartY>1) 
				string->Format("Click on grid to add horizontal subcharts");
		else if(row==3&&m_pProp->m_NumChartY>=1&&m_pProp->m_NumChartX>1)
				string->Format("Click on grid to add vertical subcharts");			
		else if(m_pProp->m_NumChartX*m_pProp->m_NumChartY>1)
				string->Format("Click on yellow grid to change layout");

		if(m_pProp->m_NumChartX*m_pProp->m_NumChartY>1)
			return true;	
		else
			return false;
	}


	if(row==-1) {
		string->Format("Right click to get options for changing chart type");

	}
	else if(row==0&&m_Mode==1) {
		switch(col) {
			case 0:
			string->Format("Double click to Edit Title");
			break;
			case 1:
			string->Format("Edit Title Size");
			break;
			case 2:
			string->Format("Edit Label Size");
			break;
			case 3:
			string->Format("Edit Legend Size");
			break;
			case 4:
			string->Format("Edit Line Width");
			break;
			case 5:
			string->Format("Edit Axis Line Width");
			break;
			case 6:
			string->Format("Edit Symbol Size");
			break;
			case 7:
			string->Format("Edit Scale Size");
			break;
			case 8:
			string->Format("Edit Minimum number of points between ticks on X scale");
			break;
			case 9:
			string->Format("Edit Minimum number of points between ticks on X scale");
		break;
			break;
			default:
				return false;
		}
		return true;

	}
	else if(m_Mode==2) {
		switch(col) {
			case 0:
				string->Format("Double click to Edit Y axis Title");
				break;
			case 1:
				string->Format("Double click to Edit  Y axis min Value");
				break;
			case 2:
				string->Format("Double click to Edit  Y axis max Value");
				break;
			case 3:
				string->Format("Double click to Edit  Y axis grid size value");
				break;
			default:
				return false;
		}
		return true;
	}
	else if(m_Mode==3) {

		int ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumIndex, row);
		if(ArrayIndex>=0) {
			void *Ptr=m_pSimDoc->m_PlotStorage.GetPtr(ArrayIndex);
			SimB *pX;
			CPG *pPG1;
			if(Ptr!=NULL) {
				int	itype=m_pSimDoc->m_PlotStorage.GetType(ArrayIndex);
				if(itype==0||itype==10) {
					pPG1=(CPG*)Ptr;
					pX=NULL;
				}
				else {
					pX=(SimB*)Ptr;	
					pPG1=NULL;

				}
			}
			else {
				pX=NULL;
				pPG1=NULL;
			}

			if(pX!=NULL&&col==1) {
				CString str=pX->GetName().c_str();
				int ilocal=m_pSimDoc->m_PlotStorage.GetLocalIndex(ArrayIndex);
				int	itype=m_pSimDoc->m_PlotStorage.GetType(ArrayIndex);
				m_pProp->m_MultiSequence=m_pSimDoc->m_PlotStorage.GetValRunNo(ArrayIndex);
				if(ilocal>=0)
				str+="("+MFC_Util::ItoAscii(ilocal+1)+")";
				if(itype>100&&col==0) 
					string->SetString("Double click to Edit Legend or \nSingle click to select a new MultiRunNo "+MFC_Util::ItoAscii(m_pProp->m_MultiSequence)+" ("+MFC_Util::ItoAscii(m_pSimDoc->MR_Get_TotalNumberofRuns())+") :"+str); 
				else if(col==0)
					string->SetString("Double click to Edit Legend for variable ("+MFC_Util::ItoAscii(row+1)+") :"+str); 
				
				return true;
			}
			else if(pPG1!=NULL) {
				CString str;
				CUGCell cell; 
				GetCell(0,row, &cell);	
				str=cell.GetText();
				int ilocal=m_pSimDoc->m_PlotStorage.GetLocalIndex(ArrayIndex);
				int koll=m_pSimDoc->m_PlotStorage.GetPGSectionNo(ArrayIndex);
				if(koll>0) {
					str=cell.GetText();
					if(koll>10000)
						string->SetString("Single click to add variable :"+str); 
					else
						string->SetString("Single click to remove variable :"+str); 

				}
				else if(col==0) {
					str=pPG1->GetVarName(ilocal).c_str();str+="/",str+=pPG1->GetVarId(ilocal).c_str();
					string->SetString("Double click to Edit Legend for variable ("+MFC_Util::ItoAscii(row+1)+") :"+str); 
				}

				return true;

			}
			else if(col==0)
				string->Format("Double click to edit Legend for variable no: %d",row+1);
			else if(col==1)
				string->Format("Right click to change color for variable no: %d",row+1);
			else if(col==2)
				string->Format("Click combo button to change line width for variable no: %d",row+1);
			else if(col==3)
				string->Format("Click combo button to change line style for variable no: %d",row+1);
			else if(col==4)
				string->Format("Click combo button to change symbol for variable no: %d",row+1);
			else if(col==5)
				string->Format("Click combo button to change size for variable no: %d",row+1);
			else if(col==6)
				string->Format("Single click to change Subchart for variable no: %d",row+1);
			else if(col==7)
				string->Format("Click combo button to change y axis for variable no: %d",row+1);
			else if(col==8)
				string->Format("");
			else if(col==9)
				string->Format("Single click to change Multi RunNo for variable no: %d",row+1);

		}
	}
	else
		return false;
	
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OnVScrollHint
int ChartCug::OnVScrollHint(long row,CString *string)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHScrollHint
int ChartCug::OnHScrollHint(int col,CString *string)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OLE
#ifdef __AFXOLE_H__

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  ChartCug::OnDragEnter(COleDataObject* pDataObject){
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  ChartCug::OnDragOver(COleDataObject* pDataObject,int col,long row){
	//return DROPEFFECT_NONE;
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  ChartCug::OnDragDrop(COleDataObject* pDataObject,int col,long row){
	return DROPEFFECT_COPY;
}
#endif

/////////////////////////////////////////////////////////////////////////////
//	
void ChartCug::OnScreenDCSetup(CDC *dc,int section){
}

/////////////////////////////////////////////////////////////////////////////
//	OnSortEvaluate
//	return		-1  <
//				0	==
//				1	>
int ChartCug::OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags)
{
	if(flags&UG_SORT_DESCENDING)
	{
		CUGCell *ptr = cell1;
		cell1 = cell2;
		cell2 = ptr;
	}

//	if(cell1->IsPropertySet(UGCELL_TEXT_SET) == false)
//	{
//		if(cell2->IsPropertySet(UGCELL_TEXT_SET) == false)
//			return 0;
//		return -1;
//	}
	switch(cell1->GetDataType())
	{
		case UGCELLDATA_STRING:
			if(NULL == cell1->GetText() && NULL == cell2->GetText())
				return 0;
			if(NULL == cell1->GetText())
				return 1;
			if(NULL == cell2->GetText())
				return -1;
			return _tcscmp(cell1->GetText(),cell2->GetText());	
		case UGCELLDATA_NUMBER:
		case UGCELLDATA_BOOL:
		case UGCELLDATA_CURRENCY:
				double n1 = cell1->GetNumber();
				double n2 = cell2->GetNumber();
				if(n1 < n2)
					return -1;
				if(n1 > n2)
					return 1;
				return 0;
	}
	if(NULL == cell1->GetText())
		return 1;
	if(NULL == cell2->GetText())
		return -1;
	return _tcscmp(cell1->GetText(),cell2->GetText());	
}

/////////////////////////////////////////////////////////////////////////////
//	OnTabSelected
void ChartCug::OnTabSelected(int ID)
{
		SetSheetNumber(ID);
		EnableUpdate(false);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(true); 
		OnSheetSetup(ID);
}

/////////////////////////////////////////////////////////////////////////////
//	OnAdjustComponentSizes
void ChartCug::OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,
		RECT *cnrBtn,RECT *vScroll,RECT *hScroll,RECT *tabs)
{
} 

/////////////////////////////////////////////////////////////////////////////
//	OnDrawFocusRect
void ChartCug::OnDrawFocusRect(CDC *dc,RECT *rect)
{
	//DrawExcelFocusRect(dc,rect);
	
	if ( !m_bDrawExcelFocus )
	{
		rect->bottom --;
		rect->right --;
		dc->DrawFocusRect(rect);
	}
	else
	{
		DrawExcelFocusRect( dc, rect);
	}
}

void ChartCug::DrawExcelFocusRect(CDC *dc,RECT *rect)
{
	BOOL innerRectOnly = false;
	BOOL hideTop = false;
	BOOL hideLeft = false;

	CPen pen,wPen;

	int dcID = dc->SaveDC();

	//find the rect to draw - may be a range of cells
	//if multi-select is on
	if(m_GI->m_multiSelectFlag&3)
	{
		if(m_GI->m_multiSelect->GetNumberBlocks() < 2)
		{
			int startCol,endCol;
			long startRow,endRow;
			m_GI->m_multiSelect->GetCurrentBlock(&startCol,&startRow,&endCol,&endRow);

			if( startRow < m_GI->m_topRow && endRow < m_GI->m_topRow )
				return;

			if( startRow > m_GI->m_bottomRow && endRow > m_GI->m_bottomRow )
				return;

			if( startCol < m_GI->m_leftCol && endCol < m_GI->m_leftCol )
				return;

			if( startCol > m_GI->m_rightCol && endCol > m_GI->m_rightCol )
				return;
			
			if(startCol != endCol || startRow != endRow)
			{
				GetRangeRect(startCol,startRow,endCol,endRow,rect);
			}
			if(startCol >= m_GI->m_numLockCols && startCol < m_GI->m_leftCol)
			{
				//hide the left
				hideLeft = true;
			}
			if(startRow >= m_GI->m_numLockRows && startRow < m_GI->m_topRow)
			{
				//hide the top
				hideTop = true;
			}
			if(rect->bottom <= rect->top)
				GetRangeRect(startCol,startRow,endCol,endRow,rect);
			if(rect->right <= rect->left)
				GetRangeRect(startCol,startRow,endCol,endRow,rect);
			
			//add draw hints for the grid so it will clear up the
			//focus rect next time
			m_CUGGrid->m_drawHint.AddHint(startCol-1,startRow-1,endCol+1,endRow+1);
		}
		else if(m_GI->m_multiSelect->GetNumberBlocks() >= 2)
		{
			innerRectOnly = true;
		}
	}
	else
	{
		//add draw hints for the grid so it will clear up the
		//focus rect next time
		m_CUGGrid->m_drawHint.AddHint(m_GI->m_currentCol-1,m_GI->m_currentRow-1,
			m_GI->m_currentCol+1,m_GI->m_currentRow+1);
	}

	int top = rect->top;
	int left = rect->left;


	//inner black rect
	dc->SelectObject(GetStockObject(BLACK_PEN));
	if(rect->top == 0)
		rect->top ++;
	if(rect->left == 0)
		rect->left ++;
	if(!hideTop)
	{
		dc->MoveTo(rect->left,rect->top);
		dc->LineTo(rect->right-1,rect->top);
	}
	else
	{
		dc->MoveTo(rect->right-1,rect->top);
	}
	dc->LineTo(rect->right-1,rect->bottom-1);
	dc->LineTo(rect->left,rect->bottom-1);
	if(!hideLeft)
	{
		dc->LineTo(rect->left,rect->top-1);
	}
	rect->top = top;
	rect->left = left;


	//inner white rect
	dc->SelectObject(GetStockObject(WHITE_PEN));
	if(rect->top == 0)
		rect->top ++;
	if(rect->left == 0)
		rect->left ++;
	if(!hideTop)
	{
		dc->MoveTo(rect->left+1,rect->top+1);
		dc->LineTo(rect->right-2,rect->top+1);
	}
	else
	{
		dc->MoveTo(rect->right-2,rect->top+1);
	}
	dc->LineTo(rect->right-2,rect->bottom-2);
	dc->LineTo(rect->left+1,rect->bottom-2);
	if(!hideLeft)
	{
		dc->LineTo(rect->left+1,rect->top);
	}
	rect->top = top;
	rect->left = left;

	//draw the rest if innerRectOnly is false
	if(!innerRectOnly)
	{
		dc->SelectObject(GetStockObject(BLACK_PEN));

		//outer rect
		if(rect->top == 0)
			rect->top +=2;
		if(rect->left == 0)
			rect->left +=2;
		if(hideLeft)
			rect->left++;
		if(!hideTop)
		{
			dc->MoveTo(rect->left-2,rect->top-2);
			dc->LineTo(rect->right+1,rect->top-2);			
		}
		else
		{
			rect->top++;
			dc->MoveTo(rect->right+1,rect->top-2);
		}
		
		dc->LineTo(rect->right+1,rect->bottom+1);
		dc->LineTo(rect->left-2,rect->bottom+1);
		
		if(!hideLeft)
		{
			dc->LineTo(rect->left-2,rect->top -3);
		}

		rect->top = top;
		rect->left = left;


		//bottom right square
		dc->MoveTo(rect->right-2,rect->bottom-2);
		dc->LineTo(rect->right,rect->bottom-2);
		dc->MoveTo(rect->right+2,rect->bottom-2);
		dc->LineTo(rect->right+2,rect->bottom+1);
		dc->MoveTo(rect->right+2,rect->bottom+1);
		dc->LineTo(rect->right+2,rect->bottom+2);
		dc->LineTo(rect->right,rect->bottom+2);
		dc->MoveTo(rect->right-2,rect->bottom+2);
		dc->LineTo(rect->right,rect->bottom+2);

		wPen.CreatePen(PS_SOLID,1,RGB(255,255,255));
		dc->SelectObject(wPen);
		dc->MoveTo(rect->right+3,rect->bottom-3);
		dc->LineTo(rect->right-3,rect->bottom-3);
		dc->LineTo(rect->right-3,rect->bottom+3);
	

		pen.CreatePen(PS_SOLID,1,RGB(120,120,120));
		dc->SelectObject(pen);


		//dark gray middle rect
		if(rect->top >0 && !hideTop)
		{
			dc->MoveTo(rect->left-2,rect->top-1);
			dc->LineTo(rect->right+2,rect->top-1);
		}
		else
			rect->top +=2;

		dc->MoveTo(rect->right,rect->top-1);
		dc->LineTo(rect->right,rect->bottom+3);
		if(rect->left >0 && !hideLeft)
		{
			dc->MoveTo(rect->left-1,rect->bottom+1);
			dc->LineTo(rect->left-1,rect->top-2);
		}
		else
			rect->left +=3;
		dc->MoveTo(rect->right+2,rect->bottom);
		dc->LineTo(rect->left-3,rect->bottom);

	}

	rect->top = top;
	rect->left = left;

	dc->RestoreDC(dcID);

	pen.DeleteObject();
	wPen.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
//	OnGetDefBackColor
COLORREF ChartCug::OnGetDefBackColor(int section)
{
	return GetSysColor ( COLOR_APPWORKSPACE );
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetFocus
void ChartCug::OnSetFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKillFocus
void ChartCug::OnKillFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapStart
BOOL ChartCug::OnColSwapStart(int col)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanColSwap
BOOL ChartCug::OnCanColSwap(int fromCol,int toCol)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTrackingWindowMoved
void ChartCug::OnTrackingWindowMoved(RECT *origRect,RECT *newRect)
{
}


void ChartCug::AddMainData(DoubleArray data, int index) 
{
	double v;
	int i=data.len;
	if(m_pProp->m_Legend1.size()<=0) return;
	if(m_MakeDataSheet) {
		SetSheetNumber(6,false);
		for (int j=0;j<data.len;j++) {
			v=*(data.data+j);
			if(v>1.E38)
				QuickSetText(1+index,j,"");
			else {
				QuickSetNumber(1+index,j,v);
				QuickSetAlignment(1+index,j,UG_ALIGNRIGHT);
			}
		}
		RedrawAll();

		if(m_CreatePG) {
			m_PGExtra.AddVariableVector(data, index);
			m_ToSavePGStruc=true;
		}
	}
}
void ChartCug::AddTimeData(DoubleArray data, DoubleArray data_y) 
{
	double v;
	CUGCell cell;
	int i=data.len;
	m_MakeDataSheet=true;

	if(m_MakeDataSheet) {

		if(m_pProp->m_Legend1.size()<=0) return;
		MakeDataSheet(0);
		SetSheetNumber(6,false);
		SetNumberRows(data.len);
		for (int j=0;j<data.len;j++) {
			GetCell(0,j,&cell);
			if(m_pUtil->m_MultiRunPlot||m_pProp->m_ReShape.m_NormalDist||m_pProp->m_ReShape.m_SortedSeries||m_pProp->m_ChartType!=1) {
				v=*(data.data+j);
				cell.SetNumber(v);
			}
			else {
				v=*(data.data+j);
				v=v/60+2880;
				long min=v;
				string txt=PGUtil::StringDatum(min);
				cell.SetText(txt.c_str());
			}
		
			
			SetCell(0,j,&cell);
			if(m_pProp->IsSecondIndepSet()) 
				QuickSetNumber(1,j,*(data_y.data+j));



			//QuickSetNumber(0,j,v);
			QuickSetNumber(-1,j,j+1);
		}
		RedrawAll();

		if(m_CreatePG) {
			m_PGExtra.SetCompleteRead(true);
			m_PGExtra.AdjustSize(data.len, m_PGExtra.GetNumVariables());
			m_PGExtra.AddTimeVector(data);
		}
	}

}
void ChartCug::AddBandData(DoubleArray datasd,DoubleArray datamin,DoubleArray datamax,DoubleArray datap10, DoubleArray datap50, DoubleArray datap90, int index) 
{
	double v;


	int n1=m_pProp->m_Legend1.size();

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
			m_PGExtra.AddVariableVector(datasd, n1+index*6);
			m_PGExtra.AddVariableVector(datamin, 1+n1+index*6);	
			m_PGExtra.AddVariableVector(datap10, 2+n1+index*6);
			m_PGExtra.AddVariableVector(datap50, 3+n1+index*6);
			m_PGExtra.AddVariableVector(datap90, 4+n1+index*6);
			m_PGExtra.AddVariableVector(datamax, 5+n1+index*6);
			m_ToSavePGStruc=true;
		}
	}

}
void ChartCug::MakeDataSheet(int entire)
{
	//	Set
	CUGCell cell;
	SetMultiSelectMode ( true );
	//DragDropTarget ( true );
	int cols, rows;
	m_MakeDataSheet=true;
	m_NumberOfBands=m_NumVarBandSelected;
	m_CreatePG=false;

	int n1=m_pProp->m_Legend1.size();
	
	n1=m_TimeSeriePlot.m_ChartInfo.NumVariables;
	if(m_pUtil->m_MultiTable) n1+=2;
	
	if(m_pProp->IsSecondIndepSet()) {
		cols=n1+2;
	}
	else {
		if(m_pProp->m_MultiValid.AcceptRejectMode&&!m_pProp->IsSecondIndepSet())
			cols=n1*2+1+m_NumberOfBands*6;
		else
			cols=n1+1+m_NumberOfBands*6;
	}
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
	int nstart=0;
	if(m_pProp->IsSecondIndepSet()) {
		nstart=1;
		QuickSetText(1,-1,"y");

	}

	for(int i=nstart;i<n1+nstart;i++) {
		UnJoinCells(i+1,-1);
		if(i>=m_pProp->m_ZLabel.size()) continue;
		if(m_pProp->IsSecondIndepSet()) QuickSetText(i+1,-1,m_pProp->GetZLabel(i-1).c_str());
		else QuickSetText(i+1,-1,"VarNo:"+MFC_Util::ItoAscii(i+1));
		m_PGExtra.SetVarName(i+1,"VarNo:"+FUtil::STD_ItoAscii(i+1));
		m_PGExtra.SetVarId(i+1,"Mean");
	}
	if(m_pProp->m_MultiValid.AcceptRejectMode&&!m_pProp->IsSecondIndepSet()) {
		for(int i=0;i<n1;i++) {
			QuickSetText(i+1+n1,-1,"VarNo:"+MFC_Util::ItoAscii(i+1)+'A');
			m_PGExtra.SetVarName(i+1+n1,"VarNo:"+FUtil::STD_ItoAscii(i+1)+'A');
			m_PGExtra.SetVarId(i+1+n1,"Mean A");
		}
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
BOOL ChartCug::InitData(Doc *pDoc, Ps *pCP, P *pCPt, int index)
{
	CString str;
	if(m_pSimDoc!=NULL) {
		m_pSimDoc=pDoc;
		m_pView=m_pSimDoc->GetModelView();
		m_pCP=pCP;
		m_pCPt=pCPt;
		m_TableIndex=index;
		m_pProp->m_ChartType=0;
		//PARAMETERS_mp_DEFINEPLOT=1; 
	//	PARAMETERS_mp_SHIFTCURVE=0;

		if(m_TableIndex>=0) {
			m_pProp->m_Title=((Tab*)m_pCP)->GetViewTitle().c_str();
			if(((Tab*)m_pCP)->GetViewFunction()==401){

			//	if(!PLOTPF_mp_OPENDATABASE) m_pSimDoc->SP_OpenDataBase();
				if(m_pSimDoc->m_pPFCurve->IsOpen()&&m_pSimDoc->m_pPFCurve->IsProfile()) {

				//PLOTPF_mp_RETRIEVEPF_POINTS();
				if(m_pSimDoc->m_pPFCurve->GetNumPF_Pressures()>0) 
					m_PFMeasuredPoints=true;
				}
			}
			

		}
		else 
			m_pProp->m_Title=m_pCP->GetViewTitle().c_str();
		if(m_pProp->m_Title!="") {
			//PARAMETERS_mp_NUMBEROFCURVES=1;
			//PARAMETERS_mp_ACTUALCURVE=1;
			m_ViewCounter=1;
			return true;
		}
	}
	return false;
}

BOOL ChartCug::InitData(Doc *pDoc, int chartnum, int charttype, string PGFile, CView *view)
{
		CString str;
	CString filename;
	CString stdbinfilename;
	m_pSimDoc=pDoc;
	CCoupModelView *pView=pDoc->GetModelView();
	m_pChartViewer=&pView->m_ChartViewer;

	m_DistMin=m_DistMax=0.;
	if(m_pSimDoc!=NULL&&m_pSimDoc->IsFinishedSimulation()||PGFile!="") {
		m_pUtil->m_MultiRunPlot=false;
		m_pCP=NULL  ; // No parameter value plot

		m_pProp->m_ChartNumber=chartnum;
		m_pProp->m_ChartType=charttype;
		m_pView=m_pSimDoc->GetModelView();
		//if(view!=NULL)
			//pView=(CCoupModelView*)view;

		//if(m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val()>0||m_pSimDoc->m_ValidationData.GetNumSumVarVariables()>0)
		m_pProp->m_MultiValid.AddOnlyValidRuns=false;


		return true;
	}

	return false;
}
bool ChartCug::InitMultiPGFilePlot(Doc *pSimDoc) {
	m_pSimDoc=pSimDoc;
	m_pProp->m_ChartNumber=m_pSimDoc->m_PlotStorage.GetCurrentChart();
	m_pProp->m_ChartType=4;
	m_pView=m_pSimDoc->GetModelView();
	m_pUtil->m_MultiRunPlot=false;
	m_pUtil->m_MultiPGPlot=true;

	m_pProp->SetMultiPGFilePlot();

	return true;
}
BOOL ChartCug::InitMultiRunPlot(Doc *pSimDoc,int chartnum, int charttype, CCoupModelView *view)
{
	CString str;
	m_pSimDoc=pSimDoc;

	m_DistMin=m_DistMax=0;

	if(m_pSimDoc!=NULL) {
			// Check and open MultiRunFile
		if(m_pSimDoc->MR_Storage_Open()) {
			m_pUtil->m_MultiRunPlot=true;
			m_pUtil->m_MultiPGPlot=false;
			if(m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val()>0&&m_pSimDoc->m_ValidationData.GetNumSumVarVariables()>0)
				m_pProp->m_MultiValid.AddOnlyValidRuns=true;
			m_pCP=NULL;
			m_pProp->m_ChartNumber=chartnum;
			m_pProp->m_ChartType=charttype;
			m_pView=m_pSimDoc->GetModelView();

			if(m_pProp->m_ChartType==0) {
					// Check for two variables to make scatter plot			
					int  itype, ichart, icharttype;

						m_NumVarSelected=0;
						for (int j=0;j<m_pSimDoc->m_PlotStorage.NumSelected();j++) {
							m_noOfPoints=m_pSimDoc->MR_Get_TotalNumberofRuns();
							itype=m_pSimDoc->m_PlotStorage.GetType(j);
							ichart=m_pSimDoc->m_PlotStorage.GetChart(j);
							icharttype=m_pSimDoc->m_PlotStorage.GetChartType(j);
							if(ichart==m_pProp->m_ChartNumber) {
								m_NumVarSelected++;
							}
						}
						if(m_NumVarSelected<2&&m_pProp->GetFirstIndepIndex()==0) {
							MessageBox("Independent variable select. Select also dependent variables !");
							return false;
						}
				}

			return true;
		}
	}
	return false;
}

void ChartCug::MakeTextPlot(CString txt) {

	m_TextPlot=txt;
	if(m_pView==NULL) 	m_pView=m_pSimDoc->GetModelView();
	if(m_pView==NULL) return;
	DrawTextPlot(&m_pView->m_ChartViewer);

}

void ChartCug::DrawTextPlot(CChartViewer *viewer)
{
	XYChart *c;
	c = new  XYChart(m_pProp->m_DisplayXSize, m_pProp->m_DisplayYSize, m_pUtil->m_ColorBackground);
	c->addTitle(Chart::Center,m_TextPlot,"arialbd.ttf", 20);
	if(m_pChartViewer!=NULL) {

/*		if(!m_TimeSeriePlot.CleanPrevCharts()) {
			BaseChart *pChart;
			pChart=m_pChartViewer->getChart();
			if(pChart!=NULL)
				delete m_pChartViewer->getChart();
		}*/


	}
	m_pView->MakeChart(c);
	if(m_pUtil->m_FileSave.m_SaveFile)
		m_pUtil->m_FileSave.MakeFile();	
	m_pUtil->m_Displayed=true;
}


void ChartCug::OnTimer(ONTIMER_TYPE	/* nIDEvent	*/)
{
	
		double viewportStart;
		double viewportWidth;
		viewportWidth=m_pView->m_ChartViewer.getViewPortWidth();
		viewportStart=double(m_NumberofMoves)/(m_TimeSeriePlot.m_Zoom.NumberOfWindows*m_step)*(1.-viewportWidth);
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
void ChartCug::OnStartAnnimation()
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
		SetTimer(ID_TIMER, m_interval, NULL);
		if(m_AnimationRunning) {
			m_AnimationRunning=false;
		}
		else {
			m_AnimationRunning=true;
		}
		

}

void ChartCug::OnChangeAnnimationSpeed()
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
		SetTimer(ID_TIMER, m_interval, NULL);
	}
	// TODO: Add your control notification handler code here
}
void ChartCug::OnNewSeq2()
{
	//if(OnUpdateSequence()) {
	int runno;
		for(int il=0;il<m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);il++) {
				int j=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, il);
				int itype=m_pSimDoc->m_PlotStorage.GetType(j);
				if(itype>100||(itype==5&&m_TimeSeriePlot.m_ChartInfo.MultiValMode)) {
					float validv=0;
					int count=0;
					if(m_TimeSeriePlot.m_ChartInfo.MultiPGMode)
						runno=m_pSimDoc->m_PlotStorage.GetValRunNo(j);
					else if (m_TimeSeriePlot.m_ChartInfo.MultiValMode)
						runno=m_pSimDoc->m_PlotStorage.GetValRunNo(j)+1;

					while(validv<0.5&&count<m_pSimDoc->MR_Get_TotalNumberofRuns()) {
						runno++;
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

void ChartCug::OnNewCurve()
{
	// TODO: Add your control notification handler code here
	
	int il=m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber)-1;	
	int j=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, il);
	int itype=m_pSimDoc->m_PlotStorage.GetType(j);
	if(m_TimeSeriePlot.m_ChartInfo.MultiValMode) {
		if(itype!=5&&il>0)
			il=1;
		else
			il=0;
	}
	
	if(m_pSimDoc->m_PlotStorage.AddCurve(0,il)&&(m_TimeSeriePlot.m_ChartInfo.MultiPGMode||m_TimeSeriePlot.m_ChartInfo.MultiValMode||(itype==0&&m_PGRepit))){
		m_pProp->m_PrevChartNumber=-1;
		if(m_TimeSeriePlot.OnUpdateSequence())
			m_TimeSeriePlot.MakeTimeSeriePlot();
	}
	else {
		if(m_TimeSeriePlot.OnUpdateSequence())
			m_pUtil->m_FileSave.OnReDraw(true);

	}
}

void ChartCug::OnActivate(UINT nstate, CWnd *pWndOther, BOOL bMinimized)
{
    switch (nstate)
    {
    case WA_ACTIVE:
        // Is data acquisition timer - get a new data sample
     //   getData();
		if(m_pUtil->m_Displayed) m_TimeSeriePlot.LoadTimeSerieData();
        break;
    case WA_CLICKACTIVE:
        // Is chart update timer - request chart update
       m_TimeSeriePlot.LoadTimeSerieData();
        break;
    }
    
    //CDialog::OnActivate(nstate, pWndOther, bMinimized);
}

HCURSOR ChartCug::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}
void ChartCug::OnChangeAnnimationSteps()
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



void ChartCug::Make_Sheet0_RealTime()
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
				if(m_pSimDoc->m_Simulator.IsPause()) {
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
	
				if(m_pSimDoc->m_Simulator.GetRunDelay()==0)
					txt="Fast";
				else if(m_pSimDoc->m_Simulator.GetRunDelay()==1)
					txt="Small Delay";
				else if(m_pSimDoc->m_Simulator.GetRunDelay()==20)
					txt="Medium Delay";
				else if(m_pSimDoc->m_Simulator.GetRunDelay()==1000)
					txt="Large Delay";
				else if(m_pSimDoc->m_Simulator.GetRunDelay()==100000)
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
			
			if(!m_pSimDoc->m_Simulator.IsPause()) 
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
void ChartCug::Make_Sheet0_ParameterPlot()
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
			float value, newvalue, prevvalue, defvalue;
			size_t line=0;
			bool enabled;
			for(size_t i=0; i<m_ParameterPlot.m_P.size();i++) {
				prevvalue=-1.1E38;
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

					txt+="("+MFC_Util::ItoAscii(m_ParameterPlot.m_TableIndex+1)+")";

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
					prevvalue=-1.1E38;
					txt=m_ParameterPlot.m_Ps[i]->GetName().c_str();
					enabled=m_ParameterPlot.m_Ps[i]->IsEnabled();
					value=m_ParameterPlot.m_Ps[i]->GetValue();
					defvalue=m_ParameterPlot.m_Ps[i]->GetOriginalValue();
					size_t newindex;
					newindex=min(m_ParameterPlot.m_NewPlotValue.size()-1,i+m_ParameterPlot.m_P.size());
					if(m_ParameterPlot.m_NewPlotValue[newindex].size()>0) {
						newvalue=m_ParameterPlot.m_NewPlotValue[newindex][0];
						prevvalue=m_ParameterPlot.m_PreviousValue[newindex][0];	
					}
					else {
						newvalue=value;
						prevvalue=value;
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
void ChartCug::Make_Sheet0_TablePlot()
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
void ChartCug::Make_Sheet0_TimeSeriePlot()
{
	CUGCell cell; 
	string str;
	string txt;
	string optionlist;
			// Time Serie Mode


			SetSH_NumberCols(3);
			for(long rr=0;rr<5;rr++)
				for(int cc=-3;cc<0;cc++)
					UnJoinCells(cc,rr);
			SetSH_Width(90);
			SetSH_ColWidth(0,30);
			SetSH_ColWidth(1,30);
			JoinCells(-2,1,-1,1);
			JoinCells(-2,3,-1,3);
			JoinCells(-3,4,-1,4);
			SetSH_ColWidth(2,30);
			for(int i=0;i<9;i++)
				SetColWidth(i,126);
			SetColWidth(1,80);
			SetColWidth(3,80);
			SetColWidth(4,80);
			SetColWidth(5,150);
			SetColWidth(6,150);
			SetColWidth(7,0);
			SetColWidth(8,160);
			if(m_pProp->m_ChartType==4||m_pProp->IsSecondIndepSet())
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
				if(m_pProp->m_ChartType==1)
					txt="Time Serie";
				else if(m_pProp->m_ChartType==0)
					txt="Scatter Chart";
				else if(m_pProp->m_ChartType==2)
					txt="Frequency Distribution";
				else if(m_pProp->m_ChartType==3)
					txt="Cumulative Frequency Distribution";
				else if(m_pProp->m_ChartType==4)
					txt="Contour Chart";
				cell.SetText(txt.c_str());
				cell.SetBackColor(m_ColorClick);
				cell.SetLabelText(optionlist.c_str());
				SetCell(0,3,&cell);
			}



			QuickSetText(0,-1,"Zoom");				
			JoinCells(0,-1,1,-1);
			QuickSetText(2,-1,"Transformations");		
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
			cell.SetText("Zoom In 1/"+MFC_Util::ItoAscii(m_ZoomLevel));
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
			GetCell(0,4,&cell);
		

			cell.SetCellType(UGCT_CHECKBOX);
			
			cell.SetLabelText("Symbols");
			if(!m_pProp->m_ReShape.m_Symbol) {
				cell.SetBool(false);
			}
			else {
				cell.SetBool(true);
			}

			SetCell(0,4,&cell);
			cell.SetLabelText("Lines");
			
			if(!m_pProp->m_ReShape.m_AddLine) {
				cell.SetBool(false);
			}
			else {
				cell.SetBool(true);
			}

			SetCell(1,4,&cell);

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

			QuickSetText(2,2,"Vertical Scale");
			QuickSetBackColor(2,2,m_ColorHeading);
			
			cell.SetLabelText("Common");
			if(!m_pProp->m_ReShape.m_CommonScale) {
				cell.SetBool(false);
			}
			else {
				cell.SetBool(true);
			}
			SetCell(2,3,&cell);
			
			cell.SetLabelText("Logaritmic");
			if(!m_pProp->m_ReShape.m_LogScale) {
				cell.SetBool(false);
			}
			else {
				cell.SetBool(true);

			}
			SetCell(2,4,&cell);
			 
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
				QuickSetText(3,4,"Forward"+MFC_Util::ItoAscii(m_Hpos)+'('+MFC_Util::ItoAscii(m_ZoomLevel)+')');
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
			cell.SetBool(false);
			cell.SetCellType(UGCT_CHECKBOX);
			//JoinCells(6,0,7,0);
			//JoinCells(6,1,7,1);
			//JoinCells(6,2,7,2);
			if(!m_TimeSeriePlot.m_ChartInfo.MultiPGMode||m_pSimDoc==NULL) {
				cell.SetLabelText("No Multi mode file");
				cell.SetReadOnly(true);
			}
			else if(m_TimeSeriePlot.m_ChartInfo.MultiPGMode) {
				cell.SetReadOnly(false);
				cell.SetLabelText("New Curve");
			}
			SetCell(6,0, &cell);
			
			cell.SetLabelText("New Run (Last var)");			
			
			if(!m_TimeSeriePlot.m_ChartInfo.MultiPGMode||m_pSimDoc==NULL) {
				cell.SetReadOnly(true);
			}
			else if(m_TimeSeriePlot.m_ChartInfo.MultiPGMode) {
				cell.SetReadOnly(false);
			}

			SetCell(6,1, &cell);
			
			cell.SetLabelText("New Run (All var)");
			if(!m_TimeSeriePlot.m_ChartInfo.MultiPGMode||m_pSimDoc==NULL) {
				cell.SetReadOnly(true);
			}
			else if(m_TimeSeriePlot.m_ChartInfo.MultiPGMode) {
				cell.SetReadOnly(false);
			}
			SetCell(6,2, &cell);

			if(m_pUtil->m_MultiRunPlot&&m_pSimDoc!=NULL) {
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
					for(int i=0;i<m_pSimDoc->MR_GetNumberOfDimensions();i++){
						for(int ii=0; ii<m_pSimDoc->MR_Get_NumberParDim(i+1);ii++){
							pPar=m_pSimDoc->MR_Get_pPar(i+1,ii+1);
							nTabIndex=m_pSimDoc->MR_GetTabIndex(i+1,ii+1);
							namep=pPar->GetName().c_str();
							if(nTabIndex>=0)				
								namep+="("+MFC_Util::ItoAscii(nTabIndex+1)+")";
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
					if(m_pProp->m_ChartType==4||m_pProp->IsSecondIndepSet()) {
							QuickSetText(9,-1,"Contour Chart");
							if(m_pProp->m_ChartType==4) {
								QuickSetText(9,0,"Y axis");
								string sel2=FUtil::GetOptionString(optionlist,m_pProp->GetSecondIndepIndex()+1);
								cell.SetText(sel2.c_str());
								SetCell(9,1,&cell);
							}
					}
					m_pProp->SetFirstIndepVarType(var);
					m_pProp->SetSecondIndepVarType(var);
				}
			}
			else {
				QuickSetText(6,3,"");
				QuickSetText(6,4,"");


			};




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

			cell.SetCellType(UGCT_NORMAL);
			cell.SetBackColor(m_ColorClick);
			cell.SetText("Save to Extra Chart Window");
			SetCell(8,2,&cell);
			cell.SetText("Save to File Extra Chart");
			SetCell(8,3,&cell);			
			if(m_pSimDoc!=NULL) {
				if(((CMainFrame*)m_pMainframe)->m_wndChartView.m_ExtraCharts.size()>0) {
					cell.SetText("Shift between saved Charts");
					SetCell(8,4,&cell);
				}
				else
					QuickSetText(8,4,"");
			}
			if(m_pProp->m_ChartType==4||m_pProp->IsSecondIndepSet()) {
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
void ChartCug::Make_Sheet1_General()
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



		QuickSetText(0,0,m_pProp->m_Title);
		QuickSetNumber(1,0,m_pUtil->m_TitleSizePoint);
		QuickSetNumber(2,0,m_pUtil->m_LabelSizePoint);
		QuickSetNumber(3,0,m_pUtil->m_LegendSizePoint);
		QuickSetNumber(4,0,m_pUtil->m_LineWidthPoint);
		QuickSetNumber(5,0,m_pUtil->m_LineAxisWidthPoint);

		QuickSetNumber(6,0,m_pUtil->m_SymbolSizePoint);
		QuickSetNumber(7,0,m_pUtil->m_AxisScalePoint);
		QuickSetNumber(8,0,m_pUtil->m_MinTickDistX);
		QuickSetNumber(9,0,m_pUtil->m_MinTickDistY);


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
		QuickSetNumber(-1,1,m_pProp->m_NumChartX);
		QuickSetBackColor(-1,1,m_ColorClick);
		QuickSetText(-1,2,"Y");
		QuickSetNumber(-1,3,m_pProp->m_NumChartY);
		QuickSetBackColor(-1,3,m_ColorClick);
		QuickSetNumber(0,2,m_pProp->m_DisplayXSize-180);
		QuickSetNumber(1,2,m_pProp->m_DisplayYSize-28);
		QuickSetNumber(2,2,m_pProp->m_TopMargin);
		QuickSetNumber(3,2,m_pProp->m_BottomMargin);
		QuickSetNumber(4,2,m_pProp->m_LeftMargin);
		QuickSetNumber(5,2,m_pProp->m_RightMargin);
		QuickSetNumber(6,2,m_pProp->m_GapMargin);
		QuickSetNumber(7,2,m_pUtil->m_GapHFraction*100);
		QuickSetNumber(8,2,m_pUtil->m_GapHorLeftFraction*100);
		QuickSetNumber(9,2,m_pUtil->m_GapVertTopFraction*100);



		QuickSetNumber(0,4,m_pUtil->m_LegendPositionX);
		QuickSetNumber(1,4,m_pUtil->m_LegendPositionY);
		
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
		QuickSetNumber(0,6,iv);
		iv=m_pProp->m_BoxRangeDimmer;
		QuickSetNumber(1,6,iv);
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
void ChartCug::Make_Sheet2_RealTime()
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
						for(size_t ii=0; ii<m_pSimDoc->MR_Get_NumberParDim(i+1);ii++){
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
void ChartCug::Make_Sheet3_ParameterPlot()
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
					txt=MFC_Util::ItoAscii(i+1);
					cell.SetLabelText(txt.c_str());
					SetCell(0,i,&cell);



				}
				if(m_ParameterPlot.m_pParTable==NULL) return;
				size_t n=m_ParameterPlot.m_pParTable->GetNumbersToPlot();
				for(size_t i=0;i<n;i++) {
						size_t l;
						l=m_ParameterPlot.m_pParTable->GetPlotIndex(i);
						GetCell(0,l,&cell);
						cell.SetBool(true);
						SetCell(0,l,&cell);
				}

}
void ChartCug::Make_Sheet2_ParameterPlot()
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
	QuickSetText(1,0,plotparam.X_Label.c_str());
	GetCell(2,0,&cell);
	cell.SetCellType(UGCT_CHECKBOX);
	if(plotparam.LogXScale)
		cell.SetBool(TRUE);
	else
		cell.SetBool(FALSE);
	SetCell(2,0,&cell);
	QuickSetNumber(3,0,plotparam.X_Min);
	QuickSetNumber(4,0,plotparam.X_Max);
	QuickSetText(0,1,"Y-Axis");
	QuickSetText(1,1,plotparam.Y_Label.c_str());
	GetCell(2,1,&cell);
	cell.SetCellType(UGCT_CHECKBOX);
	if(plotparam.LogYScale)
		cell.SetBool(TRUE);
	else
		cell.SetBool(FALSE);
	SetCell(2,1,&cell);
	for(size_t i=0; i<plotparam.LegendsVec.size();i++) {
		QuickSetText(0,2+i, "Legend ("+MFC_Util::ItoAscii(i+1)+")");
		QuickSetText(1,i+2,plotparam.LegendsVec[i].c_str());
	}
	


}
void ChartCug::Make_Sheet2_TimeSeriePlot(bool RightLabels, bool XLabels, bool ZLabels)
{
	CUGCell cell; 
	string str;
	string txt;
	string optionlist;

				int lineacc;
				SetColWidth(-1,20);
				SetColWidth(0,240);
				for(int i=1;i<6;i++)
					SetColWidth(i,106);

				QuickSetText(0,-1,"Labels on axis");
				QuickSetText(1,-1,"Min Value");
				QuickSetText(2,-1,"Max Value");
				QuickSetText(3,-1,"Grid Value");
				QuickSetText(4,-1,"Axis");
				QuickSetText(5,-1,"SubChart");


				 m_NumSubCharts=min(m_TimeSeriePlot.m_ChartInfo.NumVariables, m_NumSubCharts);

				size_t numy=m_NumSubCharts;
				if(m_pProp->IsSecondIndepSet())
					numy=1;
				


				for (size_t jc=0;jc<numy;jc++) {
					if(m_pProp->m_YScaleLeftMin.size()>0) {
						QuickSetNumber(-1,jc,jc+1);
						string txt=m_pProp->m_YLabel1.at(jc);
						QuickSetText(0,jc,txt);
						QuickSetNumber(1,jc,m_pProp->m_YScaleLeftMin.at(jc));
						QuickSetNumber(2,jc,m_pProp->m_YScaleLeftMax.at(jc));
						QuickSetNumber(3,jc,m_pProp->m_YScaleLeftInterv.at(jc));
						QuickSetText(4,jc,"Y Left");	
						if(m_pProp->IsSecondIndepSet())
							QuickSetText(5, jc,"");
						else
							QuickSetNumber(5,jc,jc+1);
					}
				}
				lineacc=numy;



				if(RightLabels&&!m_pProp->IsSecondIndepSet()){
					for (int jc=0;jc<m_NumSubCharts;jc++) {
						QuickSetNumber(-1,m_NumSubCharts+jc,jc+1);
						QuickSetText(0,m_NumSubCharts+jc,m_pProp->m_YLabel2.at(jc));
						QuickSetNumber(1,m_NumSubCharts+jc ,m_pProp->m_YScaleRightMin.at(jc));
						QuickSetNumber(2,m_NumSubCharts+jc ,m_pProp->m_YScaleRightMax.at(jc));
						QuickSetNumber(3,m_NumSubCharts+jc ,m_pProp->m_YScaleRightInterv.at(jc));
						QuickSetText(4,m_NumSubCharts+jc,"Y Right");
						QuickSetNumber(5,m_NumSubCharts+jc,jc+1);
					}
					lineacc=2*m_NumSubCharts;
				}
				if(XLabels/*||m_pProp->IsSecondIndepSet()*/) {

					for (int jc=0;jc<numy;jc++) {
						QuickSetNumber(-1,lineacc+jc,jc+1);
						QuickSetText(0,lineacc+jc,m_pProp->m_XLabel.at(jc));
						if((m_pProp->m_ChartType!=1||m_pProp->m_ReShape.m_SortedSeries||m_pProp->m_ReShape.m_NormalDist)&&!m_pProp->IsSecondIndepSet()) {
							QuickSetNumber(1,lineacc+jc ,m_pProp->m_XScaleMin.at(jc));
							QuickSetNumber(2,lineacc+jc ,m_pProp->m_XScaleMax.at(jc));
							QuickSetNumber(3,lineacc+jc ,m_pProp->m_XScaleInterv.at(jc));
						} 
						else if(m_pUtil->m_MultiRunPlot||m_pUtil->m_MultiPGPlot||m_pProp->GetSecondIndepVarType()!=DATEAXIS) {
							if(m_pProp->GetFirstIndepIndex()!=string::npos&&m_pProp->IsFirstIndepSet()&&m_pProp->GetSecondIndepVarType()==DATEAXIS) {
								QuickSetNumber(1,lineacc+jc ,m_pProp->m_TimeScale.min);
								QuickSetNumber(2,lineacc+jc ,m_pProp->m_TimeScale.max);
								QuickSetNumber(3,lineacc+jc ,m_pProp->m_TimeScale.interv);
							}
							else {
								QuickSetNumber(1,lineacc+jc ,m_pProp->m_XScaleMin.at(jc));
								QuickSetNumber(2,lineacc+jc ,m_pProp->m_XScaleMax.at(jc));
								QuickSetNumber(3,lineacc+jc ,m_pProp->m_XScaleInterv.at(jc));
							}

						}
						else  if(!m_pUtil->m_MultiRunPlot||m_pProp->GetSecondIndepVarType()==DATEAXIS)  {
							long min=m_pProp->m_TimeScale.min/60+2880.;
							string txt=PGUtil::StringDatum(min);
							QuickSetText(1,lineacc+jc ,txt);
							min=m_pProp->m_TimeScale.max/60+2880.;
							txt=PGUtil::StringDatum(min);
							QuickSetText(2,lineacc+jc ,txt);
							if(m_pProp->m_TimeScale.interv>86400)
								min=m_pProp->m_TimeScale.interv/60;
							else if(m_pProp->m_TimeScale.interv>60)
								min=m_pProp->m_TimeScale.interv/60;
							else
								min=0;
							if(min==0)
								QuickSetText(3,lineacc+jc ,"");
							else {
								txt=MFC_Util::ItoAscii(min);
								QuickSetText(3,lineacc+jc,txt);
							}


						}
						QuickSetText(4,lineacc+jc,"X Scale");
						if(!m_pProp->IsSecondIndepSet()) QuickSetNumber(5,lineacc+jc,jc+1);
					}
					lineacc+=numy;
				}
				if(ZLabels&&m_pProp->m_ZLabel.size()>0) {
					for (int jc=0;jc<m_NumSubCharts;jc++) {
						QuickSetNumber(-1,lineacc+jc,jc+1);
						QuickSetText(0,lineacc+jc,m_pProp->m_ZLabel.at(jc));

						QuickSetText(4,lineacc+jc,"Z Scale");

						QuickSetNumber(1,lineacc+jc ,m_pProp->m_ZScaleMin.at(jc));
						QuickSetNumber(2,lineacc+jc ,m_pProp->m_ZScaleMax.at(jc));
						QuickSetNumber(3,lineacc+jc ,m_pProp->m_ZScaleInterv.at(jc));



						QuickSetNumber(5,lineacc+jc,jc+1);
					}



				}
			}
void ChartCug::Make_Sheet3_RealTime(int NumTSV)
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
void ChartCug::Make_Sheet3_TimeSeriePlot()
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
			QuickSetText(0,-1,"Legends");
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
			int isubchart, MultiRunNo, itype;
				

			int mLoop=1;
			long lrow=-1;
			if(m_pProp->m_MultiValid.AcceptRejectMode) {
				mLoop=2;


			}
				//if(m_MultiTable) mn=2;
			for(int im=0;im<mLoop;im++) {
				vector<int> curvecount(m_NumSubCharts,0);

				size_t n1=min(m_TimeSeriePlot.m_ChartInfo.NumVariables, m_pProp->m_AxisLayer.size());
				for(int i=0;i<n1;i++) {
						lrow++;
						ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumIndex, i);
						PLOTv p_info=m_pSimDoc->m_PlotStorage.GetPlotInfo(ArrayIndex);
						int koll=m_pSimDoc->m_PlotStorage.GetPGSectionNo(ArrayIndex);
						isubchart=m_pSimDoc->m_PlotStorage.GetSubChart(ArrayIndex);
						itype=m_pSimDoc->m_PlotStorage.GetType(ArrayIndex);
						if(itype>100) {
							MultiRunNo=m_pSimDoc->m_PlotStorage.GetValRunNo(ArrayIndex);
							GetCell(9,lrow,&cell);
							cell.SetCellType(UGCT_DROPLIST);
							CString optionlist;
							optionlist="All\nAccepted\n";
							for(int ii=0;ii<m_pSimDoc->MR_Get_TotalNumberofRuns();ii++) {
								float validv=0;
								validv=m_pSimDoc->m_MStorage.GetMultiP(ii,m_pSimDoc->MR_Get_NumTotalPar());
								if(validv>0.5) {
									optionlist.Append(MFC_Util::ItoAscii(ii+1));
									optionlist.AppendChar('\n');
								}
							}
							cell.SetLabelText(optionlist);
							if(itype<110)
								cell.SetNumber(MultiRunNo);
							else if(itype>120)
								cell.SetText("Accepted");
							else 
								cell.SetText("All");
							SetCell(9,lrow,&cell);
						}
						else if(itype!=35&&itype==5&&m_pSimDoc->IsMultiSimulation()) {
							int ResRunNo=m_pSimDoc->m_PlotStorage.GetValRunNo(ArrayIndex);
							GetCell(9,lrow,&cell);
							cell.SetCellType(UGCT_DROPLIST);
							CString optionlist;
							optionlist="All\nAccepted\n";
							for(int ii=0;ii<m_pSimDoc->MR_Get_TotalNumberofRuns();ii++) {
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
						else if(itype==10) {
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
						else if(itype%5==0&&itype<30&&m_pSimDoc->IsMultiSimulation()) {
							GetCell(9,lrow,&cell);
							cell.SetCellType(UGCT_DROPLIST);
							string optionlist;
							
							optionlist="All\nAccepted\nSingle Run\n";
							if(itype==15) txt="All";
							else if(itype==25) txt="Accepted";
							cell.SetText(txt.c_str());
							cell.SetLabelText(optionlist.c_str());
							SetCell(9,lrow,&cell);



						}
						else {
							QuickSetNumber(-1,lrow,i+1);
							GetCell(0,lrow,&cell);
							QuickSetText(9,lrow,"");

						}
						if(itype%5==0&&itype>0&&itype<30) {
							GetCell(10,lrow,&cell);
							cell.SetCellType(UGCT_DROPLIST);
							if(itype==5)
								optionlist="Value\nResidual\nGaussian LogLi\n";
							else
								optionlist="Value\nResidual\n";
							int iopt=m_pSimDoc->m_PlotStorage.GetLogLiVersion(ArrayIndex);
							if(itype==5) {
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
							cell.SetLabelText(optionlist.c_str());
							SetCell(10,lrow,&cell);
						}
						else {

							QuickSetText(10,lrow,"");
						}
						
						if(m_pProp->m_ReShape.m_CommonDayNo||m_pProp->m_ReShape.m_CommonWithDay) {
							cell.SetCellType(UGCT_CHECKBOX);
							cell.SetLabelText(m_pProp->m_Legend1.at(i).c_str());
							if(koll>10000)
								cell.SetBool(false);
							else
								cell.SetBool(true);
						}
						else {
							cell.SetCellType(UGCT_NORMAL);
							if(im==0&&m_pProp->m_Legend1.size()>i)
								cell.SetText(m_pProp->m_Legend1.at(i).c_str());
							else
								cell.SetText("All");
						}



						SetCell(0,lrow,&cell);
					//	QuickSetText(0,i,m_Legend1.at(i));
						CString txt;
						if(curvecount.size()<=isubchart-1) 
							isubchart=curvecount.size();
						
						
						curvecount[isubchart-1]++;
						QuickSetBackColor(1,i,MFC_Util::GetColorLine(curvecount[isubchart-1]));

						GetCell(2,lrow,&cell);
						cell.SetCellType(UGCT_DROPLIST);
						optionlist="Common\nThin\nNormal\nLarge\nVery large";


						//m_pProp->m_LineWidth.at(curvecount[isubchart-1]-1)
						switch(m_pProp->GetLineWidth(curvecount[isubchart-1]-1))
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
						cell.SetText(txt);
						cell.SetLabelText(optionlist.c_str());
						SetCell(2,lrow,&cell);

						GetCell(3,lrow,&cell);
						cell.SetCellType(UGCT_DROPLIST);
						optionlist="Solid\nDashed Line\nDotted Line\nDot Dashed Line\n";
						optionlist+="Alternativ Dashed Line";

						switch(m_pProp->GetDashModeLine(curvecount[isubchart-1]-1))
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
						cell.SetText(txt);
						cell.SetLabelText(optionlist.c_str());
						SetCell(3,lrow,&cell);
						GetCell(4,lrow,&cell);
						cell.SetCellType(UGCT_DROPLIST);
						optionlist="no symbol\nFilled circle\nOpen circle\nFilled square\n";
						optionlist+="Open square\nFilled diamond\nOpen diamond\nFilled triangle\nOpen triangle";

					
						switch(m_pProp->GetSymbolStyle(curvecount[isubchart-1]-1))
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
					
						cell.SetText(txt);
						cell.SetLabelText(optionlist.c_str());
						SetCell(4,lrow,&cell);					
					
						GetCell(5,lrow,&cell);
						cell.SetCellType(UGCT_DROPLIST);
						optionlist="Common\nVery small\nSmall\nNormal\nLarge\n";
						optionlist+="Very Large\n";

						if(m_pProp->GetSymbolStyle(curvecount[isubchart-1]-1)!=0) {



							switch(m_pProp->GetSymbolSize(curvecount[isubchart-1]-1))
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
						cell.SetText(txt);
						cell.SetLabelText(optionlist.c_str());
						SetCell(5,lrow,&cell);	
						if(m_pSimDoc!=NULL) {
							int subchart=m_pSimDoc->m_PlotStorage.GetSubChart(ArrayIndex);
							if(subchart>0)
								QuickSetNumber(6,lrow,subchart);
						}
					

						GetCell(7,lrow,&cell);
						cell.SetCellType(UGCT_DROPLIST);
						optionlist="Y Left\nY Right\nX axis\n";


						if(m_pProp->m_AxisLayer.at(i)<=1)
							txt="Y Left";
						else if(m_pProp->m_AxisLayer.at(i)==2)
							txt="Y Right";
						else 
							txt="X axis";

						cell.SetText(txt);
						cell.SetLabelText(optionlist.c_str());
						SetCell(7,lrow,&cell);	
						QuickSetNumber(8,lrow,curvecount[isubchart-1]);

				}
			}
			/*	if(m_MultiTable) {
					n1=m_Legend1.size();
					QuickSetText(0,n1,"MultiRunMode");
					QuickSetBackColor(0,n1,m_ColorHeading);
					str=MFC_Util::ItoAscii(m_MultiSequence)+"("+MFC_Util::ItoAscii(m_MultiRepititions)+")";
					QuickSetText(0,n1+1,str);
				}*/
}
void ChartCug::Make_Sheet4_RealTime()
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
				for(int i=0; i<m_pSimDoc->MR_GetNumExtraMonitor();i++) {
					int index;
					index=m_pSimDoc->MR_GetExtraMonitor(i);
					GetCell(2,index,&cell);
					cell.SetParam(i+1000);
					cell.SetBool(true);
					SetCell(2,index,&cell);
				}
}
void ChartCug::Make_Sheet4_TimeSeriePlot(int rows)
{
	CUGCell cell; 
	string str;
	string txt;
	string optionlist;
			SetColWidth(-1,20);
			SetColWidth(0,240);
			for(int i=1;i<6;i++)
				SetColWidth(i,114);
			QuickSetText(0,-1,"Legends");
			QuickSetText(1,-1,"Band color");
			QuickSetText(2,-1,"Log Dist");
			QuickSetText(3,-1,"Min");
			QuickSetText(4,-1,"Max");
			QuickSetText(5,-1,"Steps");
			JoinCells(1,0,1,rows-1);
			QuickSetBackColor(1,0, m_pUtil->GetColorDist());

			for(int i=0;i<rows;i++) {
					QuickSetNumber(-1,i,i+1);
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
					cell.SetLabelText(optionlist.c_str());
					SetCell(2,i,&cell);
					QuickSetNumber(3,i, m_pProp->GetXDistMin(i));
					QuickSetNumber(4,i, m_pProp->GetXDistMax(i));
					QuickSetNumber(5,i, m_pProp->GetXDistSteps(i));
			}
		}
void ChartCug::Make_Sheet5_TimeSeriePlot(int rows) {
		int  ArrayIndex;
			SetColWidth(-1,20);
			SetColWidth(0,240);
			for(int i=1;i<6;i++)
				SetColWidth(i,114);
			QuickSetText(2,-1,"Intercept");
			QuickSetText(3,-1,"Slope");
			QuickSetText(4,-1,"R");

			if(rows>m_pProp->m_Legend1.size()) rows=m_pProp->m_Legend1.size();
			for(int i=0;i<rows;i++) {
					ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumIndex, i);
					int koll=m_pSimDoc->m_PlotStorage.GetPGSectionNo(ArrayIndex);
					if(koll>0) {
						if(koll<10000) {
							QuickSetBackColor(0,i,MFC_Util::GetColorLine(i+1)); 
						}


					}
					QuickSetNumber(-1,i,i+1);
					QuickSetText(0,i,m_pProp->m_Legend1.at(i));
					QuickSetBackColor(1,i,MFC_Util::GetColorLine(i+1));
					if((m_pProp->GetRegA0(i)>-0.9E38&&m_pProp->m_ChartType==0&&i>0)||(m_pProp->m_ChartType==1&&m_pProp->GetRegA0(i)>-0.9E38)) {
						QuickSetNumber(2,i, m_pProp->GetRegA0(i));
						QuickSetNumber(3,i, m_pProp->GetRegA1(i));
						QuickSetNumber(4,i, m_pProp->GetRegR(i));
					}
					

			}
}
	

// User clicks on the Pointer pushbutton
//



