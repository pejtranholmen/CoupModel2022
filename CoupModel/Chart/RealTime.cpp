// realtimedemoDlg.cpp : implementation file
//



#include "../stdafx.h"
#include "../util.h"
#include "../ModelTypes\SimB.h"
#include "../NewBase\NewMap.h"
#include "../CoupModel.h"
#include "../CoupModelDoc.h"
#include "../CoupModelView.h"

//#include "./RealTimeOptionDlg.h"

//#include "./RealTimeValOptions.h"
#include <math.h>
#include "./RealTime.h"
#include "../MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static const int DataRateTimer = 1;
static const int ChartUpdateTimer = 2;
static const int DataInterval = 10000;

//extern bool Running;

// The number of samples per data series used in this demo
const int sampleSize = 240, numser =80; 
size_t m_count2;

	double m_timeStamps[sampleSize];	// The timestamps for the data series
	double m_MultiRunStamps[sampleSize];	// The timestamps for the data series

	double m_dataSeries[numser][sampleSize];	
	double m_ValidSeriesA[numser][sampleSize];
	double m_ValidSeriesB[numser][sampleSize];
	double m_ValidSeriesC[numser][sampleSize];
	double m_ValidSeriesD[numser][sampleSize];
	double m_ValidSeriesE[numser][sampleSize];
	double m_ValidSeriesF[numser][sampleSize];

	double m_dataMin[numser], m_dataMax[numser];
	// The values for the data series A
	double m_dataSeriesAReal[numser][sampleSize];	// The values for the data series B
	double m_dataSeriesBReal[numser][sampleSize];	// The values for the data series B
	double m_dataSeriesCReal[numser][sampleSize];	// The values for the data series C
	double m_dataSeriesDReal[numser][sampleSize];

/////////////////////////////////////////////////////////////////////////////
// RealTime dialog


//
// Constructor
//
#include "../Cug\ChartProperties\ChartCug.h"

RealTime::RealTime(CWnd *parent)

	:CWnd()
{
	
    //{{AFX_DATA_INIT(RealTime)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	CWinApp* pApp = AfxGetApp(); 
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_ValidId.resize(numser);
	m_ParId.resize(numser);
	m_MakeAnnimation=TRUE;
	m_MakeAnnimation=MFC_Util::GetProfileIntNo("MakeAnnimation", m_MakeAnnimation);
	
	m_DisplaySize=1;
	m_cx=900;
	m_cy=400;
	m_MultiSimDisplay=TRUE;
	m_icount=0;
	m_count2=0;
	m_NumCharts=0;
	m_NumChartX=0;
	m_NumChartY=0;
	m_LabelSizePoint = 12;


					  




	m_InitDialog=FALSE;

	m_Title.push_back("Temperature (C)");
	m_Title.push_back("Water Content (vol %)");
	m_Title.push_back("Snow Depth (m)");
	m_Title.push_back("Accumulated evaporation (mm)");
	m_NumVarPlot.clear();

	m_PlotType.push_back(0);
	m_PlotType.push_back(0);
	m_PlotType.push_back(1);
	m_PlotType.push_back(1);


	for(int i=0;i<3;i++) {
		int ii=i*10;
		m_ColorLine[0+ii]=0xff0000;
		m_ColorLine[1+ii]=0x00cc00;
		m_ColorLine[2+ii]=0x0000ff;
		m_ColorLine[3+ii]=0x008b0000;
		m_ColorLine[4+ii]=0x002f4f4f;
		m_ColorLine[5+ii]=0x0000008b;
		m_ColorLine[6+ii]=0x00ff00ff;
		m_ColorLine[7+ii]=0x0000ffff;
		m_ColorLine[8+ii]=0x007fff00;
		m_ColorLine[9+ii]=0x00ffff00;
	}
	string cstr;
	for(int i=0;i<30;i++) {
		cstr="HColorLine"+FUtil::STD_ItoAscii(i+1);
		m_ColorLine[i]=MFC_Util::GetProfileIntNo(cstr, m_ColorLine[i]);	
	}
	m_ChartCugPtr=nullptr;
	m_NumPar=0;
	m_prevtime=0.;
	m_pSimDoc=nullptr;
	m_Finnished=FALSE;
	m_pProp=MySingle::instance().GetChartPropPointer();
	m_pUtil=MySingle::instance().GetChartUtilPointer();
	m_pChartViewer=nullptr;


}


/*
void RealTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RealTime)
	DDX_Control(pDX, IDC_ChartViewer0, m_ChartViewer0);
	DDX_Control(pDX, IDC_RunPB, m_RunPB);
	DDX_Control(pDX, IDC_UpdatePeriod, m_UpdatePeriod);
	DDX_Control(pDX, IDC_UpdatePeriod2, m_DisplayPeriod);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ChartViewer2, m_ChartViewer1);
	DDX_Control(pDX, IDC_ChartViewer3, m_ChartViewer2);
	DDX_Control(pDX, IDC_ChartViewer4, m_ChartViewer3);
	DDX_Control(pDX, IDC_PROGRESS1, m_pProgress1);
	DDX_Text(pDX, IDC_RunReport, m_RunReport1);
	DDX_Text(pDX, IDC_RunReport2, m_RunReport2);
	DDX_Control(pDX, IDC_PROGRESS2, m_pProgress2);
	DDX_Text(pDX, IDC_RunReportDate, m_ActualDate);
	DDX_Control(pDX, IDC_RunReportDate, m_pDate);
	DDX_Control(pDX, IDC_RunReport, m_pRemaining);
	DDX_Control(pDX, IDC_SLIDER1, m_SimSpeedControl);
	DDX_Control(pDX, IDC_StepPB2, m_StepPB);

	DDX_Control(pDX, IDC_CHECK1, m_MultiSimView2);
	DDX_Control(pDX, IDC_BUTTON_INTERUPT, m_interupt);
}*/
/*
BEGIN_MESSAGE_MAP(RealTime, CDialog)
    //{{AFX_MSG_MAP(RealTime)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_FreezePB, OnFreezePB)
    ON_CBN_SELCHANGE(IDC_UpdatePeriod, OnSelchangeUpdatePeriod)
    ON_CONTROL(CVN_ViewPortChanged, IDC_ChartViewer0, OnViewPortChanged)
	ON_WM_SIZE()
    //}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_RunReport2, OnStnClickedRunreport2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, OnNMCustomdrawSliderSimSpeed)
	ON_BN_CLICKED(IDC_StepPB2, OnBnClickedSteppb2)
	ON_BN_CLICKED(IDC_RunPB, OnRunPB)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedMultiSimView)
	ON_BN_CLICKED(IDC_BUTTON_INTERUPT, OnBnClickedButtonInterupt)
END_MESSAGE_MAP()
*/
BEGIN_MESSAGE_MAP(RealTime, CWnd)
    ON_WM_TIMER()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// RealTime message handlers

//
// Initialization
//

BOOL RealTime::OnInit()
{
  //  CDialog::OnInitDialog();

/*	CRect rect_old,rect_new;
	GetWindowRect(rect_old);
	rect_new.bottom=m_cy+rect_old.top;
	rect_new.top=rect_old.top;
	rect_new.left=rect_old.left;
	rect_new.right=m_cx+rect_old.left;
	//rect=CRect(0,m_cy,0,m_cx);
	MoveWindow(rect_new,TRUE);
	*/
	string str;
	if(m_pSimDoc->IsMultiSimulation()) 
		str="Monintoring Multiple Simulation ("+FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_TotalNumberofRuns())+"), ";
	else
		str="Monitoring Single simulation, ";


/*

	if(m_pSimDoc->IsMultiSimulation()) {
	//	m_RunDateLabel.SetWindowText("Actual Run No");	
		GetDlgItem(IDC_CHECK1)->ModifyStyle(WS_DISABLED,0);
		GetDlgItem(IDC_BUTTON_INTERUPT)->ModifyStyle(WS_DISABLED,0);
	}
	else {
	//	m_RunDateLabel.SetWindowText("Actual Sim Date");
		GetDlgItem(IDC_CHECK1)->ModifyStyle(0,WS_DISABLED);
		GetDlgItem(IDC_BUTTON_INTERUPT)->ModifyStyle(0,WS_DISABLED);
	}
	GetDlgItem(IDC_CHECK1)->RedrawWindow();
	GetDlgItem(IDC_BUTTON_INTERUPT)->RedrawWindow();
	if(!m_InitDialog) {
		if(m_pSimDoc->IsMultiSimulation())	m_MultiSimDisplay=TRUE;
		m_gridRealTime.AttachGrid(this, IDC_GRIDSIMPLE);
		m_gridRealTime.SetMultiView(m_MultiSimDisplay);
		m_gridRealTime.OnSetup();
		m_InitDialog=TRUE;
		if(m_pSimDoc->IsMultiSimulation())
		
			m_MultiSimView2.SetCheck(TRUE);
			m_MultiSimView2.RedrawWindow();
	}


	str+=m_pSimDoc->GetCurrentSimFile();
	SetWindowText(str);
    // *** code automatically generated by VC++ MFC AppWizard ***
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
  */  
    //
    // Initialize member variables
    //
  //  m_extBgColor = getDefaultBgColor();     // Default background color
    m_threshold = 200;                      // Initial alarm threshold
	int pos;
	size_t delay;
	delay=m_pSimDoc->m_Simulator.m_pModelInfo->GetRunDelay();
	if(delay==0)
		pos=100;
	else
		pos=100-delay/10;
	//m_SimSpeedControl.SetPos(pos);

    // Clear data arrays to Chart::NoValue
	for (int i = 0; i < sampleSize; ++i){
        m_timeStamps[i] = Chart::NoValue;
		m_MultiRunStamps[i]= Chart::NoValue;
	}
	for (int j =0;j<numser;j++){
		for(int i=0;i<sampleSize;i++) {
			m_dataSeries[j][i]=Chart::NoValue;
			m_dataSeriesAReal[j][i]=Chart::NoValue;
			m_dataSeriesBReal[j][i]=Chart::NoValue;
			m_dataSeriesCReal[j][i]=Chart::NoValue;
			m_dataSeriesDReal[j][i]=Chart::NoValue;
		}
	}
	for (int j =0;j<numser;j++){
		for(int i=0;i<sampleSize;i++){
			m_ValidSeriesA[j][i]=Chart::NoValue;
			m_ValidSeriesB[j][i]=Chart::NoValue;
			m_ValidSeriesC[j][i]=Chart::NoValue;
			m_ValidSeriesD[j][i]=Chart::NoValue;
			m_ValidSeriesE[j][i]=Chart::NoValue;
			m_ValidSeriesF[j][i]=Chart::NoValue;
		}
	}


    // Set m_nextDataTime to the current time. It is used by the real time random number 
    // generator so it knows what timestamp should be used for the next data point.
    SYSTEMTIME st;
    GetLocalTime(&st);
    m_nextDataTime = Chart::chartTime(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, 
        st.wSecond) + st.wMilliseconds / 1000.0;
	double testtime, Diff;
	Diff=Chart::chartTime(1995,01,01);
	//testtime=TIMEINFO_mp_DTIME*double(86400);

    //
    // Initialize controls
    //

    // Set up the data acquisition mechanism. In this demo, we just use a timer to get a 
    // sample every 250ms.
    SetTimer( DataRateTimer, DataInterval,0);

    // The chart update rate (in ms)
//    m_UpdatePeriod.SelectString(0, _T("50"));
//	m_DisplayPeriod.SelectString(0, _T("500"));

	//m_ActualDate=StringDatum(TIMEINFO_mp_MINU).c_str();
    


    // Load icons for the Run/Freeze buttons
   // loadButtonIcon(IDC_RunPB, IDI_RunPB, 100, 20);
   // if(m_pSimDoc->IsMultiSimulation()) loadButtonIcon(IDC_FreezePB, IDI_FreezePB, 100, 20);

    // Initially set the Run mode
   // m_RunPB.SetCheck(1);
//	GetDlgItem(IDC_StepPB2)->ModifyStyle(0,WS_DISABLED);
	//GetDlgItem(IDC_StepPB2)->RedrawWindow();
	if (m_pView == nullptr) return false;
 	m_pView->SetRealTimeMode(TRUE);   
	OnRunPB();


    return TRUE;
}


void RealTime::InitData(Doc *pDoc)
{

	m_Finnished=FALSE;
	
	m_pSimDoc=pDoc;
	m_pView=m_pSimDoc->GetModelView();
	m_pChartViewer=&m_pView->m_ChartViewer;


	//if(m_pSimDoc->IsMultiSimulation()) return;
	//TIMEINFO_mp_RELRUN=0;
	//PARAMETERS_mp_RUN_RES_ID=0;
	//m_gridRealTime.Init(this,pDoc);

	CString Name, Group, Type;
	CWinApp* pApp = AfxGetApp();

	m_VarA.clear();
	m_VarB.clear();
	m_VarC.clear();
	m_VarD.clear();
	m_Finnished=FALSE;

	CString str;
	int niv;
	m_NumChartX=m_pSimDoc->m_AnnimChartX;
	m_NumChartY=m_pSimDoc->m_AnnimChartY;
	m_NumCharts=m_NumChartX*m_NumChartY;

	m_RecLastDisplayed=0;
	getData();


}

void RealTime::UpdateData()
{

	string Name, Group, Type;
	CWinApp* pApp = AfxGetApp();
	

	string str;
	size_t niv;


	size_t iv=m_pSimDoc->AnnimVarList.size();	

	while(m_VarMonitoring.size()>0) {
		m_VarMonitoring[0].clear();
		m_VarMonitoring.erase(m_VarMonitoring.begin());
	}



	for(auto ii=m_pSimDoc->AnnimVarList.begin(); ii!=m_pSimDoc->AnnimVarList.end();++ii) {
			auto ich= (*ii).iChart;
			if(m_VarMonitoring.size()<=ich) m_VarMonitoring.resize(ich+1);			
	}
	size_t count=0;
	for(auto ii=m_pSimDoc->AnnimVarList.begin(); ii!=m_pSimDoc->AnnimVarList.end();++ii) {
			auto ich=(*ii).iChart;
			m_VarMonitoring[ich-1].push_back(count);
			count++;
	}

	
	
// First Chart Find points
/*	
	int	nv=m_NumVarPlot.at(0);
	int nvv=nv;

	m_VarA.resize(nv);
	for(int i=0; i<nv;i++) {
		if(m_NameA.size()>i) {
			Name=m_NameA.at(i);
			Group=m_GroupA.at(i);
			int ityp=m_VarTypeA.at(i);
			Type=p_ModelInfo->GetVarType(ityp+4).c_str();
			SimB *pBase;
			pBase=(SimB*)m_pSimDoc->GetPtr(Type,Group,Name);
			if(pBase!=nullptr)
				m_VarA[i]= pBase;	
			else {
				nvv=nvv-1;
				m_NameA.erase(m_NameA.begin()+i);
				m_GroupA.erase(m_GroupA.begin()+i);
				m_VarTypeA.erase(m_VarTypeA.begin()+i);
				m_NumVarPlot[0]= nvv;
			}
		}
	}

	nv=m_NumVarPlot.at(1);
	m_VarB.resize(nv);
	for(int i=0; i<nv;i++) {
		if(m_NameB.size()>i) {
			Name=m_NameB.at(i);
			Group=m_GroupB.at(i);
			int ityp=m_VarTypeB.at(i);
			Type=p_ModelInfo->GetVarType(ityp+4).c_str();
			SimB *pBase;
			pBase=(SimB*)m_pSimDoc->GetPtr(Type,Group,Name);
			if(pBase!=nullptr)
				m_VarB[i]= pBase;	
		}
	}
	nv=m_NumVarPlot.at(2);
	m_VarC.resize(nv);
	for(int i=0; i<nv;i++) {
		if(m_NameC.size()>i) {
			Name=m_NameC.at(i);
			Group=m_GroupC.at(i);
			int ityp=m_VarTypeC.at(i);
			Type=p_ModelInfo->GetVarType(ityp+4).c_str();
			SimB *pBase;
			pBase=(SimB*)m_pSimDoc->GetPtr(Type,Group,Name);
			if(pBase!=nullptr)
				m_VarC[i]= pBase;	
		}
	}
	nv=m_NumVarPlot.at(3);
	m_VarD.resize(nv);
	for(int i=0; i<nv;i++) {
		if(m_NameD.size()>i) {
			Name=m_NameD.at(i);
			Group=m_GroupD.at(i);
			int ityp=m_VarTypeD.at(i);
			Type=p_ModelInfo->GetVarType(ityp+4).c_str();
			SimB *pBase;
			pBase=(SimB*)m_pSimDoc->GetPtr(Type,Group,Name);
			if(pBase!=nullptr)
				m_VarD[i]= pBase;	
		}
	}*/


	m_RecLastDisplayed=0;

}

// *** code automatically generated by VC++ MFC AppWizard ***
// If you add a minimize button to your dialog, you will need the code below
// to draw the icon.  For MFC applications using the document/view model,
// this is automatically done for you by the framework.


// *** code automatically generated by VC++ MFC AppWizard ***
// The system calls this to obtain the cursor to display while the user drags
// the minimized window.

//
// User clicks on the Run pushbutton
//
void RealTime::OnRunPB() 
{
    // Enable chart update timer
    CString s, sd;
	KillTimer(ChartUpdateTimer); 
	KillTimer(DataRateTimer); 
  //  m_UpdatePeriod.GetLBText(m_UpdatePeriod.GetCurSel(), s);
	//m_DisplayPeriod.GetLBText(m_DisplayPeriod.GetCurSel(), sd);

	UINT DataInt=MFC_Util::AtoInt(s);	UINT DataIntDisp=MFC_Util::AtoInt(sd);
    SetTimer(ChartUpdateTimer, DataIntDisp, 0);
	SetTimer(DataRateTimer, DataInt, 0);
//	PARAMETERS_mp_RUN_PAUSE=0;
//	PARAMETERS_mp_RUN_STEP=0;
//	GetDlgItem(IDC_StepPB2)->ModifyStyle(0,WS_DISABLED);
//	GetDlgItem(IDC_StepPB2)->RedrawWindow();

}

//
// User clicks on the Freeze pushbutton
//
void RealTime::OnFreezePB() 
{
    // Disable chart update timer

	m_pSimDoc->m_Simulator.m_pModelInfo->SetPause();
	//GetDlgItem(IDC_StepPB2)->ModifyStyle(WS_DISABLED,0);
	//GetDlgItem(IDC_StepPB2)->RedrawWindow();

}

//
// Handles timer events
//
void RealTime::OnTimer(ONTIMER_TYPE	 nIDEvent	) 
{
	BOOL newdata;
	if(!m_pSimDoc->m_Simulator.m_pModelInfo->IsPause()&&m_pSimDoc->m_Simulator.m_pModelInfo->GetRelRun()<0.99) {
		switch (nIDEvent)
		{
		case DataRateTimer:
			// Is data acquisition timer - get a new data sample
			getData();
	
			if(m_pSimDoc->IsMultiSimulation()&&m_MultiSimDisplay) {
				newdata=FALSE;
				newdata=AddData();
				if(m_MultiSimDisplay&&newdata) m_pView->m_ChartViewer.updateViewPort(true, false);   
			}
			else {
				m_MultiSimDisplay=FALSE;
				drawChart(&m_pView->m_ChartViewer);
			//	m_pView->m_ChartViewer.updateViewPort(true, false);  
			}
			break;
		case ChartUpdateTimer:
			// Is chart update timer - request chart update

			 m_pView->m_ChartViewer.updateViewPort(true, false);    

			break;
		}
	}
	else if(!m_Finnished&&!p_ModelInfo->IsRunning()) {
		CMainFrame *p_mainframe;
		p_mainframe = (CMainFrame*) AfxGetMainWnd();
		//if(pGlobalCug!=nullptr)
		//	MySingle::instance().GetCugPointer()->OnSetup();
		p_mainframe->m_wndFileView.FillFileView();
		//p_mainframe->m_wndClassView.FillClassView();

		m_Finnished=TRUE;
	}
    
    //OnTimer(nIDEvent);
}

//
// View port changed event
//
void RealTime::OnViewPortChanged()
{
    drawChart(&m_pView->m_ChartViewer);


}

//
// User changes the chart update period
//
void RealTime::OnSelchangeUpdatePeriod() 
{
    //if (m_RunPB.GetCheck())
    {
        // Call freeze then run to use the new chart update period
      //  OnFreezePB();
        OnRunPB();
    }   
}

//
// User presses the alarm threshold spin button
//
/*
void RealTime::OnDeltaposAlarmSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // Update the threshold 
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    m_threshold = pNMUpDown->iPos + pNMUpDown->iDelta;
    
    // Update chart
    m_ChartViewer0.updateViewPort(true, false);
	m_ChartViewer1.updateViewPort(true, false);
	m_ChartViewer2.updateViewPort(true, false);
	m_ChartViewer3.updateViewPort(true, false);
    *pResult = 0;
}*/

/////////////////////////////////////////////////////////////////////////////
// RealTime methods

//
// A utility to shift a new data value into a data array
//
static void shiftData(double *data, int len, double newValue)
{
    memmove(data, data + 1, sizeof(*data) * (len - 1));
    data[len - 1] = newValue;
}

//
// The data acquisition routine. In this demo, this is invoked every 250ms.
//
BOOL RealTime::AddData()
{

	int ncol=m_pSimDoc->MR_Get_NumTotalPar()+10;
	m_NumVar=ncol;
	if(m_NumVar>numser) m_NumVar=numser;

			  
	
	CString Name, Group, Type;

	if(m_MakeAnnimation) {
	int latestRecSim=m_pSimDoc->MR_Get_ActualRunNumber()-1;
	int rec;
	rec=m_RecLastDisplayed;
	if(m_RecLastDisplayed==latestRecSim) return FALSE;
	m_RecLastDisplayed=latestRecSim;
	/*if(m_pSimDoc->MR_ResetMonitoring()) {
		for (int j =0;j<numser;j++){
			for(int i=0;i<sampleSize;i++) {
				m_dataSeries[j][i]=Chart::NoValue;
			}
		}
	}*/
	if(m_pSimDoc->MR_ResetValMonitoring()) {
		for (int j =0;j<numser;j++){
			for(int i=0;i<sampleSize;i++) {
				m_ValidSeriesA[j][i]=Chart::NoValue;
				m_ValidSeriesB[j][i]=Chart::NoValue;
				m_ValidSeriesC[j][i]=Chart::NoValue;
				m_ValidSeriesD[j][i]=Chart::NoValue;
				m_ValidSeriesE[j][i]=Chart::NoValue;
				m_ValidSeriesF[j][i]=Chart::NoValue;
			}
		}
	}
	while(rec<latestRecSim) {
		
		ncol=m_pSimDoc->MR_Get_NumTotalPar()+1;
		int i, ii;	
				int ind=0;
				ind=0;
				int icount=0;
				m_NameA.clear();m_NameB.clear();m_NameC.clear();m_NameD.clear();
				for(i=0;i<m_pSimDoc->MR_GetNumberOfDimensions();i++){
						for(ii=0; ii<m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i+1);ii++){
								if(m_pSimDoc->MR_GetMethod(i+1,ii+1)==9||m_pSimDoc->MR_GetMethod(i+1,ii+1)==11||m_pSimDoc->MR_GetMethod(i+1,ii+1)<=3) {	
									if(m_pSimDoc->MR_GetMonitoring(i+1,ii+1)>0) {
										if(ind<numser) {
											m_dataMin[icount]=double(m_pSimDoc->MR_GetMin(i+1,ii+1));
											m_dataMax[icount]=double(m_pSimDoc->MR_GetMax(i+1,ii+1));
											double Value=double(m_pSimDoc->m_MStorage.GetMultiP(rec,ind));
											double NValue;
											if(m_pSimDoc->MR_GetMethod(i+1,ii+1)==11) {
												m_dataMin[icount]=log(m_dataMin[icount]);
												m_dataMax[icount]=log(m_dataMax[icount]);
												Value=log(Value);
												NValue= 1-(m_dataMax[icount]-Value)/(m_dataMax[icount]-m_dataMin[icount]);
											}
											else
												NValue= 1-(m_dataMax[icount]-Value)/(m_dataMax[icount]-m_dataMin[icount]);
											shiftData(m_dataSeries[icount], sampleSize, NValue);
											icount++;
											m_NameA.push_back(m_pSimDoc->MR_Get_CalibParName(ind).c_str());
								
										}									
									}
									
								}
								ind++;
						}
				}
				m_NumPar=icount;
				double fval;
				if(icount<numser) {
					for(i=0;i<m_pSimDoc->MR_GetNumExtraMonitor();i++) {
						int extra=m_pSimDoc->MR_GetExtraMonitor(i)-1;
						fval=m_pSimDoc->m_MStorage.GetMultiP(rec,ind+extra);
						if(fval<=float(MISSING))
							fval=Chart::NoValue;
						shiftData(m_dataSeries[icount], sampleSize,fval);
						m_NameB.push_back(m_pSimDoc->MR_GetExtraLabel(extra).c_str());
						icount++;
					}
				}
				string test;
				ncol=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
				for(i=0;i<ncol;i++){
					if(m_pSimDoc->m_ValidationData.GetValMonitoring(i)>0) {
							test=m_pSimDoc->m_ValidationData.GetValName(i).c_str();
							int iv=m_pSimDoc->m_ValidationData.GetValLocalIndex(i);
							if(iv>-1) test+="("+FUtil::STD_ItoAscii(iv+1)+")";

							m_ValidId[i]= test;
					}
						//ind++; 
				}
					int indc=0;
					int indb=0;
					int indall=0;
					for(int iv=0;iv<6;iv++) {
						indc=0;
						for(int i=0; i<ncol;i++) {
							if(m_pSimDoc->m_ValidationData.GetValMonitoring(i)>0) {
									fval=m_pSimDoc->m_MStorage.GetValid(rec,indall);
									if(fval<=float(MISSING))
										fval=Chart::NoValue;
									if(iv==0) {
										if(ind<(numser)) {
											shiftData(m_ValidSeriesA[indc], sampleSize, fval);
										}

									}
									else if(iv==1) {
										if(ind<(numser)) {
											shiftData(m_ValidSeriesB[indc], sampleSize, fval);
										}
									}
									else if(iv==2) {
										if(ind<(numser)) {
											shiftData(m_ValidSeriesC[indc], sampleSize, fval);
										}

									}
									else if(iv==3) {
										if(ind<(numser)) {
											shiftData(m_ValidSeriesD[indc], sampleSize, fval);
										}
									}
									else if(iv==4) {
										if(ind<(numser)) {
											shiftData(m_ValidSeriesE[indc], sampleSize, fval);
										}

									}
									else {
										if(ind<(numser)) {
											shiftData(m_ValidSeriesF[indc], sampleSize, fval);
										}
									}
									indc++;
									if(iv==0) {

										int FileNo=m_pSimDoc->m_ValidationData.GetValFileNumber(i);
										F *pValFile=m_pSimDoc->ValidationFilePointer(FileNo);
										int ind=m_pSimDoc->m_ValidationData.GetValFileIndex(i);

										CPG *pPG;
										BOOL valid;
										pPG=pValFile->GetPointer();
										if(pPG==nullptr) {
											pPG=new CPG;
										
											valid=pPG->ReadContentOfPGFileAndOpenMainPGStreamForReading(pValFile->GetStrValue().c_str());
											if(valid) pValFile->SetPointer(pPG);
											pPG->CloseFile();
										}


										test=pPG->GetVarName(ind+1).c_str();test+=pPG->GetVarId(ind+1).c_str();
										FUtil::trim(test);

									//m_NameC.push_back(m_pSimDoc->GetValName(i));
										m_NameC.push_back(test);
									}
							}
							if(indc<(numser*4))
								indall++;
						}
					}
			

			shiftData(m_MultiRunStamps, sampleSize, rec+1);
			rec++;
		
	
	
		}
		

	}
		
	return TRUE;
}

void RealTime::ResetData() {
		m_count2=0;


	/*	for (int j =0;j<numser;j++){
			for(int i=0;i<sampleSize;i++) {
				m_ValidSeriesA[j][i]=Chart::NoValue;
				m_ValidSeriesB[j][i]=Chart::NoValue;
				m_ValidSeriesC[j][i]=Chart::NoValue;
				m_ValidSeriesD[j][i]=Chart::NoValue;
				m_ValidSeriesE[j][i]=Chart::NoValue;
				m_ValidSeriesF[j][i]=Chart::NoValue;
			}
		}*/


}
void RealTime::ResetValMonitoring() {
		for (int j =0;j<numser;j++){
			for(int i=0;i<sampleSize;i++) {
				m_dataSeries[j][i]=Chart::NoValue;
			}
		}
}
void RealTime::getData()
{
    // The current time in millisecond resolution

	CMainFrame *p_mainframe;
	p_mainframe = (CMainFrame*) AfxGetMainWnd();

    SYSTEMTIME st;
    GetLocalTime(&st);
    double now = Chart::chartTime(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, 
        st.wSecond) + st.wMilliseconds / 1000.0;
	double testtime;

	//Diff=Chart::chartTime(1995,01,01);
	m_NumCharts=0;	
    testtime=m_pSimDoc->m_Simulator.m_pModelInfo->GetSecSimTime();
//	if(testtime<0) return;
	//if(testtime-m_prevtime<60.) return;
	if(m_pSimDoc->m_Simulator.m_pModelInfo->GetRelRun()<0.001) return;

	now=testtime;
	if(m_count2==0)
		m_prevtime=0;
	else
		m_prevtime=testtime;
	m_icount++;
	m_count2++;

	CString str;
	
						  
	

		
//	m_pProgress1.SetPos(int(100.*TIMEINFO_mp_RELRUN));

	if(m_pSimDoc->IsMultiSimulation()) {
/*		str="MultiRunning ";
		if(BAYESIANCALIB_mp_INDEXP>0){
			str+=FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_ActualRunNumber())+" (";			
			str+=FUtil::STD_ItoAscii(BAYESIANCALIB_mp_NUMIMPROVEDRUN)+" = accepted)";
		}
		else {
			str+=FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_ActualRunNumber())+" (";
			str+=FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_TotalNumberofRuns())+")";
		}
				
	//	m_pDate.SetWindowText(str);	

		str+=" time left: ";
		m_RunReport1=FUtil::STD_ItoAscii(GENERAL_mp_IHOUR)+":"+FUtil::STD_ItoAscii(GENERAL_mp_IMINUT)+":"+FUtil::STD_ItoAscii(GENERAL_mp_ISEC);
		str+=m_RunReport1;
		//m_pRemaining.SetWindowText(m_RunReport1);
		int Ileft=int(100.*TIMEINFO_mp_GLOBALRELRUN);*/
		
	//	m_pProgress2.SetPos(Ileft);
	}
	else {
		str="Running single simulation, time left: ";

		//m_RunReport1=FUtil::STD_ItoAscii(GENERAL_mp_IHOUR)+":"+FUtil::STD_ItoAscii(GENERAL_mp_IMINUT)+":"+FUtil::STD_ItoAscii(GENERAL_mp_ISEC);
		str+=m_RunReport1;
		str+="Actual date: ";
	

		//m_ActualDate=StringDatum(TIMEINFO_mp_MINU).c_str();
		str+=m_ActualDate;

		//m_pDate.SetWindowText(m_ActualDate);*/
	}

	//p_mainframe->UpdatePaneText(0,str);
	//std::list<Annim> ::iterator ii;
		int i1=0;
		int i2=0;
		int i3=0;
		int i4=0;
		if(m_NameA.size()<1) UpdateData();
		if(!m_MultiSimDisplay)
		for(auto ii=m_pSimDoc->AnnimVarList.begin(); ii!=m_pSimDoc->AnnimVarList.end();++ii) {
		//for(int i=0;i<m_pSimDoc->AnnimVarList1m_NumVarPlot.at(0);i++){
				double datav;
				int ityp=(*ii).type;
				SimB* pBase;
				pBase=(*ii).pBase;
				string name;
				name=pBase->GetName();

				if(pBase->Is_Vector()) {
					int ind=(*ii).LocalIndex;
					datav=pBase->GetValue(ind);
				}
				else
					datav=pBase->GetValue();
				if(datav>-1.E37) {
					int ich= (*ii).iChart;
					switch (ich) {
					case 1:
						shiftData(m_dataSeriesAReal[i1], sampleSize, datav);
						i1++;
						break;
					case 2:
						shiftData(m_dataSeriesBReal[i2], sampleSize, datav);
						i2++;
						break;
					case 3:
						shiftData(m_dataSeriesCReal[i3], sampleSize, datav);
						i3++;
						break;
					case 4:
						shiftData(m_dataSeriesDReal[i4], sampleSize, datav);
						i4++;
						break;
					}
				}


		};

		if(!m_MultiSimDisplay)
			shiftData(m_timeStamps, sampleSize, now);

		m_NumRealCharts=m_VarMonitoring.size();
/*		m_NumCharts=0;	
		if(m_NumVarPlot.size()<=m_NumCharts) m_NumVarPlot.resize(m_NumCharts+1);
	   if(i1>0) {
		   m_NumVarPlot[m_NumCharts]=i1;
		   m_NumCharts++;	
	   }
	   if(i2>0) {
		   m_NumVarPlot[m_NumCharts]=i2;
		   m_NumCharts++;	
	   }
	   if(i3>0) {
		   m_NumVarPlot[m_NumCharts]=i3;
		   m_NumCharts++;	
	   }
	   if(i4>0) {
		   m_NumVarPlot[m_NumCharts]=i4;
		   m_NumCharts++;	
	   }
	   */


}
//
// Draw the chart and display it in the given viewer
//



void RealTime::drawChart(CChartViewer *viewer)
{
	if(viewer==nullptr) return;


	if(m_VarMonitoring.size()==0) 	UpdateData();	

	if(m_VarMonitoring.size()==0) return;

	CWinApp* pApp = AfxGetApp();	
	int Annimation=MFC_Util::GetProfileIntNo("MakeAnnimation", 0);
	if(!Annimation) return;
	//if(m_NumCharts<=0) return;
	m_pProp->m_MakeChartTitle=FALSE;

	if(m_pView!=nullptr) {
		m_cx=m_pView->GetXViewSize();
		m_cy=m_pView->GetYViewSize();
	}

	int plotoption;
	
	LegendBox *b=nullptr;
	int ndiv=2;
	if(m_NumCharts<3)
		ndiv=1;
	int chartx_size=(m_cx-130)/m_NumChartX;
	int charty_size=(m_cy-20)/m_NumChartY;
	int yplotheight, xplotwidth;

	int ytop=m_pProp->GetTopMargin();
	int ybottom=m_pProp->GetBottomMargin();;
	int ydiff=int(double(m_pProp->GetGapMargin())*(1.-m_pUtil->GetGapHorFraction()));
	int xdiff=int(double(m_pProp->GetGapMargin())*m_pUtil->GetGapHorFraction());
	int xleft=m_pProp->GetLeftMargin();
	int xright=m_pProp->GetRightMargin();


	m_ColorBackground=MFC_Util::GetProfileIntNo("HColorBackground", m_ColorBackground);

	m_TitleSizePoint=MFC_Util::GetProfileIntNo("TitleSize", 12);



	int xdiff1, xdiff2, ydiff1, ydiff2;
	xdiff1=int(float(xdiff)*m_pUtil->GetGapHorLeftFraction());
	xdiff2=xdiff-xdiff1;
	ydiff1=int(float(ydiff)*m_pUtil->GetGapVertTopFraction());
	ydiff2=ydiff-ydiff1;

	
	if (m_pView != nullptr) {
		m_DisplayXSize = m_pView->GetXViewSize();
		m_DisplayYSize = m_pView->GetYViewSize();

		yplotheight = (m_DisplayYSize - ytop - ybottom - (m_NumChartY - 1)*ydiff) / m_NumChartY;
		xplotwidth = (m_DisplayXSize - xleft - xright - (m_NumChartX - 1)*xdiff) / m_NumChartX;

	}
	else
		return;


	MultiChart *mc;

	XYChart *pXY_Chart=nullptr;
	vector <XYChart*> chartv;
	m_NumCharts=m_NumChartX*m_NumChartY;

	mc = new MultiChart(m_DisplayXSize, m_DisplayYSize, m_ColorBackground);

	// loop for 4 subplots
	for (int ich=0; ich<m_NumCharts; ich++) {
		plotoption=ich;

		chartx_size=m_DisplayXSize/m_NumChartX;
		charty_size=m_DisplayYSize/m_NumChartY;


		int xcol=(ich%m_NumChartX);
		int yrow=(ich/m_NumChartX%m_NumChartY);
		
		if(m_ChartCugPtr==nullptr) { 
			delete mc;
			return;
		}
		pXY_Chart=(XYChart*)CreateXYChart(yrow, xcol, yplotheight,xplotwidth);

		if(m_pProp->GetRoundedFrame())
           pXY_Chart->setRoundedFrame(m_ColorBackground);

		if(m_pUtil->GetRemoveTitle()==0)
			m_pProp->m_MakeChartTitle=FALSE;
		else if(m_pUtil->GetRemoveTitle()==2)
			m_pProp->m_MakeChartTitle=TRUE;
 
    // Configure the x-axis to auto-scale with at least 75 pixels between major tick and 
    // 15  pixels between minor ticks. This shows more minor grid lines on the chart.
    pXY_Chart->xAxis()->setTickDensity(75, 15);

    // Set the axes width to 2 pixels
    pXY_Chart->xAxis()->setWidth(2);
    pXY_Chart->yAxis()->setWidth(2);
	
	double lastTime,firstTime;
    // Now we add the data to the chart. 
	if(m_MultiSimDisplay) {
		lastTime = m_MultiRunStamps[sampleSize - 1];
		firstTime=m_MultiRunStamps[0];
		int xcount=1;
		while(firstTime==Chart::NoValue&&xcount<sampleSize){
			firstTime=m_MultiRunStamps[xcount];
			xcount++;
		}
		if (lastTime != Chart::NoValue)    {
        // Set up the x-axis to show the time range in the data buffer
			if(firstTime == Chart::NoValue)
				pXY_Chart->xAxis()->setDateScale(0, lastTime);
			else
				pXY_Chart->xAxis()->setDateScale(firstTime , lastTime);
		}

	}
	else {
		if(m_count2==0) {
			for (int i = 0; i < sampleSize; ++i)
				m_timeStamps[i]= Chart::NoValue;


		}
		lastTime = m_timeStamps[sampleSize - 1];
		firstTime=m_timeStamps[0];
		if (lastTime != Chart::NoValue)	{
			// Set up the x-axis to show the time range in the data buffer
			if(firstTime!=Chart::NoValue&&firstTime>lastTime) {
				m_icount=0;
				for (int i = 0; i < sampleSize; ++i){
					m_timeStamps[i]= Chart::NoValue;
				}
				for (int j =0;j<numser;j++){
					for(int i=0;i<sampleSize;i++) {
						m_dataSeriesAReal[j][i]=Chart::NoValue;
						m_dataSeriesBReal[j][i]=Chart::NoValue;
						m_dataSeriesCReal[j][i]=Chart::NoValue;
						m_dataSeriesDReal[j][i]=Chart::NoValue;
					}
				}
				firstTime=m_timeStamps[0];
			}

			if(firstTime == Chart::NoValue) {
				if(m_count2<120) {
					delete pXY_Chart;
					delete mc;
					return;
				}
				firstTime=m_timeStamps[sampleSize-m_count2];
				pXY_Chart->xAxis()->setDateScale(firstTime, lastTime);
				/*if(m_timeStamps[200]==Chart::NoValue) {
					c->xAxis()->setDateScale(lastTime -double(86400)*TIMEINFO_mp_TPERIOD/50, lastTime);
				}
				else {
					double diff=(lastTime-m_timeStamps[200])*7;
					c->xAxis()->setDateScale(lastTime -diff, lastTime);
				}*/
			}
			else
				pXY_Chart->xAxis()->setDateScale(firstTime , lastTime);
		}
	}
        // Set the x-axis label format
	if(m_MultiSimDisplay) {


	}
	else {
		if((lastTime-firstTime)>86.4E5||lastTime-firstTime<0)
			pXY_Chart->xAxis()->setLabelFormat("{value|yyyy-m-d}");
		else if(lastTime-firstTime>86.4E4)
			pXY_Chart->xAxis()->setLabelFormat("{value|m-d}");
		else if(lastTime-firstTime>86400)
			pXY_Chart->xAxis()->setLabelFormat("{value|d-h }");
		else if(lastTime-firstTime>3600)
			pXY_Chart->xAxis()->setLabelFormat("{value|h:n}");
		else
			pXY_Chart->xAxis()->setLabelFormat("{value|h:n:s}");
	}
	

	LineLayer *layer=nullptr;
	AreaLayer *Alayer=nullptr;
	if(m_PlotType.at(plotoption)==0) {
		        // Create a line layer to plot the lines
					layer = pXY_Chart->addLineLayer();
				       // The x-coordinates are the timeStamps.
					if(m_MultiSimDisplay)
						layer->setXData(DoubleArray(m_MultiRunStamps, sampleSize));
					else
						layer->setXData(DoubleArray(m_timeStamps, sampleSize));
	}
	else {
					layer = pXY_Chart->addLineLayer();
				       // The x-coordinates are the timeStamps.
					if(m_MultiSimDisplay)
						layer->setXData(DoubleArray(m_MultiRunStamps, sampleSize));
					else
						layer->setXData(DoubleArray(m_timeStamps, sampleSize));
					Alayer = pXY_Chart->addAreaLayer();
				       // The x-coordinates are the timeStamps.
					if(m_MultiSimDisplay)
						Alayer->setXData(DoubleArray(m_MultiRunStamps, sampleSize));
					else
						Alayer->setXData(DoubleArray(m_timeStamps, sampleSize));
					Alayer->setDataCombineMethod(0);
	}

        char buffer[1024];
		int nv;
		int valcount;
		string text;
		string newtitle="";

		if(m_VarMonitoring[plotoption].size()>1)
			nv=m_VarMonitoring[plotoption].size();

		switch (plotoption) {	
			case 0:
				if(m_MultiSimDisplay)
					nv=m_NumPar;
				else 
					nv=m_VarMonitoring[plotoption].size();
				if(m_pSimDoc->AnnimVarList.size()>nv) {
					for(int i=0;i<nv;i++) {
						if(m_VarMonitoring[plotoption].size()>i)
							text=m_pSimDoc->AnnimVarList[m_VarMonitoring[plotoption][i]].pBase->GetName();
						else
							text="";

						if(m_MultiSimDisplay) {
							newtitle="Normalised parameter values";
							layer->addDataSet(DoubleArray(m_dataSeries[i], sampleSize), m_ColorLine[i],text.c_str() );
						}
						else if(m_PlotType.at(plotoption)==0)
							layer->addDataSet(DoubleArray(m_dataSeriesAReal[i], sampleSize),m_ColorLine[i],text.c_str());
						else
							Alayer->addDataSet(DoubleArray(m_dataSeriesAReal[i], sampleSize), m_ColorLine[i],text.c_str());

						if(m_pProp->m_MakeChartTitle)
							pXY_Chart->addTitle(Chart::TopCenter,newtitle.c_str(),"arialbd.ttf", m_TitleSizePoint);
					}
				}
				break;
			case 1:
				if(m_VarMonitoring[plotoption].size()>1)
					nv=m_VarMonitoring[plotoption].size();
				else
					nv=0;
				if(m_pSimDoc->AnnimVarList.size()>nv) {
					for(int i=0;i<nv;i++) {
						if(m_VarMonitoring[plotoption].size()>i)
							text=m_pSimDoc->AnnimVarList[m_VarMonitoring[plotoption][i]].pBase->GetName();
						else
							text="";

					if(m_MultiSimDisplay) {
						newtitle="Common Characteristics";
						layer->addDataSet(DoubleArray(m_dataSeries[m_NumPar+i], sampleSize), m_ColorLine[i], text.c_str() );
					}

					else if(m_PlotType.at(plotoption)==0)
						layer->addDataSet(DoubleArray(m_dataSeriesBReal[i], sampleSize),m_ColorLine[i],text.c_str());
					else
						Alayer->addDataSet(DoubleArray(m_dataSeriesBReal[i], sampleSize),m_ColorLine[i],text.c_str());
				
				
					if(m_pProp->m_MakeChartTitle)
						pXY_Chart->addTitle(Chart::TopCenter,newtitle.c_str(),"arialbd.ttf", m_TitleSizePoint);
				}
				}
				break;
			case 2:
				if(m_VarMonitoring[plotoption].size()>0)
					nv=m_VarMonitoring[plotoption].size();
				else
					nv=0;
				if(m_MultiSimDisplay){
					nv=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
					valcount=0;

				}
				if(m_pSimDoc->AnnimVarList.size()>nv) {
					for(int i=0;i<nv;i++) {
						if(m_VarMonitoring[plotoption].size()>i)
							text=m_pSimDoc->AnnimVarList[m_VarMonitoring[plotoption][i]].pBase->GetName();
						else
							text="";

						if(m_MultiSimDisplay) {
							if(m_pSimDoc->m_ValidationData.GetValMonitoring(i)>0) {
								if(m_NameC.size()>valcount) {
									switch(m_pSimDoc->GetMonitor1View()) {
										case (0):
											newtitle="R2 for regression line";
											layer->addDataSet(DoubleArray(m_ValidSeriesA[valcount], sampleSize),m_ColorLine[valcount],m_NameC.at(valcount).c_str());
											break;
										case (1):
											newtitle="Intercept for regression line";
											layer->addDataSet(DoubleArray(m_ValidSeriesB[valcount], sampleSize), m_ColorLine[valcount], m_NameC.at(valcount).c_str());
											break;
										case (2):
											newtitle="Slope for regression line";
											layer->addDataSet(DoubleArray(m_ValidSeriesC[valcount], sampleSize), m_ColorLine[valcount], m_NameC.at(valcount).c_str());
											break;
										case (3):
											newtitle="Mean Error";
											layer->addDataSet(DoubleArray(m_ValidSeriesD[valcount], sampleSize), m_ColorLine[valcount], m_NameC.at(valcount).c_str());
											break;
									}
									if(m_pProp->m_MakeChartTitle)
										pXY_Chart->addTitle(Chart::TopCenter,newtitle.c_str(),"arialbd.ttf", m_TitleSizePoint);
									valcount++;
								}
							}


						}
						else if(m_PlotType.at(plotoption)==0)
							layer->addDataSet(DoubleArray(m_dataSeriesCReal[i], sampleSize),m_ColorLine[i],text.c_str());
						else
							Alayer->addDataSet(DoubleArray(m_dataSeriesCReal[i], sampleSize),m_ColorLine[i],text.c_str());
					}
				}
				break;
			case 3:
				if(m_MultiSimDisplay){
					nv=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
					valcount=0;

				}
				if(m_pSimDoc->AnnimVarList.size()>nv) {
					for(int i=0;i<nv;i++) {
						if(m_VarMonitoring[plotoption].size()>i)
							text=m_pSimDoc->AnnimVarList[m_VarMonitoring[plotoption][i]].pBase->GetName();
						else
							text="";
						if(m_MultiSimDisplay) {
							if(m_pSimDoc->m_ValidationData.GetValMonitoring(i)>0) {
								if(m_NameC.size()>valcount) {
									switch(m_pSimDoc->GetMonitor2View()) {
										case (0):
											newtitle="Root Mean Square Error (RMSE)";
											layer->addDataSet(DoubleArray(m_ValidSeriesE[valcount], sampleSize), m_ColorLine[valcount], m_NameC.at(valcount).c_str());
											break;
										case (1):
											newtitle="Log Likelihood";
											layer->addDataSet(DoubleArray(m_ValidSeriesF[valcount], sampleSize), m_ColorLine[valcount], m_NameC.at(valcount).c_str());
											break;
									}
									if(m_pProp->m_MakeChartTitle)
										pXY_Chart->addTitle(Chart::TopCenter,newtitle.c_str(),"arialbd.ttf", m_TitleSizePoint);

									valcount++;
								}
							}

						}
						else if(m_PlotType.at(plotoption)==0)
							layer->addDataSet(DoubleArray(m_dataSeriesDReal[i], sampleSize),m_ColorLine[i],text.c_str());
						else
							Alayer->addDataSet(DoubleArray(m_dataSeriesDReal[i], sampleSize),m_ColorLine[i],text.c_str());
					}
				}
				break;          

		}

		pXY_Chart->layout();
		if(ich==0)
			mc->addChart(0,0,pXY_Chart);
		else if(yrow==0&&xcol>0) 
			mc->addChart((xcol-1)*(xplotwidth+xdiff)+(xplotwidth+xleft+xdiff2),0,pXY_Chart);
		else if(xcol==0&&yrow>0) {
			mc->addChart(0,(yrow-1)*(yplotheight+ydiff)+(yplotheight+ydiff2+ytop),pXY_Chart);
		}
		else {
			mc->addChart((xcol-1)*(xplotwidth+xdiff)+(xplotwidth+xleft+xdiff2),
				(yrow-1)*(yplotheight+ydiff)+(yplotheight+ydiff2+ytop),pXY_Chart);
		}
	
		chartv.push_back(pXY_Chart);


	}


    // Set the chart image to the WinChartViewer
	
	
//   
	viewer->setChart(mc);
	if(m_pUtil->m_FileSave.m_SaveFile)
		m_pUtil->m_FileSave.MakeFile(m_pSimDoc);

	ImageMapHandler *handler=viewer->getImageMapHandler();
	if(m_MultiSimDisplay) 
		viewer->setImageMap(viewer->getChart()->getHTMLImageMap("clickable", "",
				"title='[{dataSetName}] {x}: {value}'"));
	else
		viewer->setImageMap(viewer->getChart()->getHTMLImageMap("clickable", "",
		"title='[{dataSetName}] {x}: {value}'"));

	while(chartv.size()>0) {
		delete chartv[0];
		chartv.erase(chartv.begin()+0);

	}
    delete mc;
}

/////////////////////////////////////////////////////////////////////////////
// General utilities

//
// Get the default background color
//



void RealTime::OnStnClickedRunreport2()
{
	// TODO: Add your control notification handler code here
}
/*
void RealTime::OnNMCustomdrawSliderSimSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	int value=m_SimSpeedControl.GetPos();
	PARAMETERS_mp_RUN_DELAY=(100-value)*10;
	*pResult = 0;
}
*/
void RealTime::OnBnClickedSteppb2()
{
	//PARAMETERS_mp_RUN_STEP=1;
	m_pSimDoc->m_Simulator.m_pModelInfo->SetGoSim();
//	PARAMETERS_mp_RUN_PAUSE=0;
//	m_StepPB.SetCheck(0);
//	GetDlgItem(IDC_StepPB2)->RedrawWindow();
	getData();
	OnViewPortChanged();

	// TODO: Add your control notification handler code here
}
void RealTime::OnReDraw() {



}


void RealTime::OnBnClickedButton1()
{
	//m_gridRealTime.SetMultiView(m_MultiSimDisplay);
	/*if(!m_MultiSimDisplay) {
		CRealTimeOptionDlg dlg(m_pDoc, this,m_MultiSimDisplay);
		if(dlg.DoModal()==IDOK);
	}
	else {
		CRealTimeValOptions dlg(m_pDoc, this,m_MultiSimDisplay);
		if(dlg.DoModal()==IDOK);
	}*/
	//m_gridRealTime.OnSetup();


}
BOOL RealTime::AddChartVar(int chartno, SimB *pBase, string Type, int index)
{
	string Name, Group;
	bool response;
	response=false;

//	if(pBase->IsKindOf(RUNTIME_CLASS(X))
		Group=pBase->GetGroup();
		Name=pBase->GetName();

		int ityp;
		if(Type=="State Variables")
			ityp=0;
		else if(Type=="Flow Variables")
			ityp=1;
		else if(Type=="Auxiliary Variables")
			ityp=2;
		else
			ityp=3;

	switch(chartno) {
		
		case 1:
			if(m_NumVarPlot.at(0)<5) {
				int numvar=m_NumVarPlot.at(0);
				numvar++;
				m_NumVarPlot[0]=numvar;
				m_NameA.push_back(Name);
				m_GroupA.push_back(Group);
				m_VarTypeA.push_back(ityp);
				m_IndexA.push_back(index);
				if(numvar==1) {
					auto s=pBase->GetName();s+=" (";s+=pBase->GetUnit();s+=")";
					SetChartTitle(chartno-1,s);
		
				}
				response=TRUE;
			}

			break;
		case 2:
			if(m_NumVarPlot.at(1)<5) {
				int numvar=m_NumVarPlot.at(1);
				numvar++;
				m_NumVarPlot[1]=numvar;
				m_NameB.push_back(Name);
				m_GroupB.push_back(Group);
				m_VarTypeB.push_back(ityp);
				m_IndexB.push_back(index);
				if(numvar==1) {
					auto s=pBase->GetName();s+=" (";s+=pBase->GetUnit();s+=")";
					SetChartTitle(chartno-1,s);
								}
				response=TRUE;
			}

			break;
		case 3:
			if(m_NumVarPlot.at(2)<5) {
				int numvar=m_NumVarPlot.at(2);
				numvar++;
				m_NumVarPlot[2]=numvar;
				m_NameC.push_back(Name);
				m_GroupC.push_back(Group);
				m_VarTypeC.push_back(ityp);
				m_IndexC.push_back(index);
				if(numvar==1) {
					auto s=pBase->GetName();s+=" (";s+=pBase->GetUnit();s+=")";
					SetChartTitle(chartno-1,s);
				}
				response=TRUE;
			}

			break;
		case 4:
			if(m_NumVarPlot.at(3)<5) {
				int numvar=m_NumVarPlot.at(3);
				numvar++;
				m_NumVarPlot[3]=numvar;
				m_NameD.push_back(Name);
				m_GroupD.push_back(Group);
				m_VarTypeD.push_back(ityp);
				m_IndexD.push_back(index);
				if(numvar==1) {
					auto s=pBase->GetName();s+=" (";s+=pBase->GetUnit();s+=")";
					SetChartTitle(chartno-1,s);
					}
				response=TRUE;
			}

			break;



	}
	if(response)
		UpdateData();

	return response;

}

BOOL RealTime::RemoveChartVar(int chartno, int ivarno)
{
	int numvar;
	SimB *pBase;
	switch(chartno) {
		
		case 1:
				numvar=m_NumVarPlot.at(0);
				numvar--;
				m_NumVarPlot[0]=numvar;
				m_NameA.erase(m_NameA.begin()+ivarno-1);
				m_GroupA.erase(m_GroupA.begin()+ ivarno-1);
				m_VarTypeA.erase(m_VarTypeA.begin()+ivarno-1);
				m_IndexA.erase(m_IndexA.begin()+ivarno-1);
				if(numvar==1) {
					pBase=(SimB*)m_VarA.at(0);
					auto s=pBase->GetName();s+=" (";s+=pBase->GetUnit();s+=")";
					SetChartTitle(chartno-1,s);
				}
			break;
		case 2:
				numvar=m_NumVarPlot.at(1);
				numvar--;
				m_NumVarPlot[1]=numvar;
				m_NameB.erase(m_NameB.begin()+ivarno-1);
				m_GroupB.erase(m_GroupB.begin()+ ivarno-1);
				m_VarTypeB.erase(m_VarTypeB.begin()+ivarno-1);
				m_IndexB.erase(m_IndexB.begin()+ivarno-1);
				if(numvar==1) {
					pBase=(SimB*)m_VarB.at(0);
					auto s=pBase->GetName();s+=" (";s+=pBase->GetUnit();s+=")";
					SetChartTitle(chartno-1,s);
				}

			break;
		case 3:
				numvar=m_NumVarPlot.at(2);
				numvar--;
				m_NumVarPlot[2]=numvar;
				m_NameC.erase(m_NameC.begin()+ivarno-1);
				m_GroupC.erase(m_GroupC.begin()+ ivarno-1);
				m_VarTypeC.erase(m_VarTypeC.begin()+ivarno-1);
				m_IndexC.erase(m_IndexC.begin()+ivarno-1);
				if(numvar==1) {
					pBase=(SimB*)m_VarC.at(0);
					auto s=pBase->GetName();s+=" (";s+=pBase->GetUnit();s+=")";
					SetChartTitle(chartno-1,s);

				}


			break;
		case 4:
				numvar=m_NumVarPlot.at(3);
				numvar--;
				m_NumVarPlot[3]=numvar;
				m_NameD.erase(m_NameD.begin()+ivarno-1);
				m_GroupD.erase(m_GroupD.begin()+ ivarno-1);
				m_VarTypeD.erase(m_VarTypeD.begin()+ivarno-1);
				m_IndexD.erase(m_IndexD.begin()+ivarno-1);
				if(numvar==1) {
					pBase=(SimB*)m_VarD.at(0);
					auto s=pBase->GetName();s+=" (";s+=pBase->GetUnit();s+=")";
					SetChartTitle(chartno-1,s);
				}

			break;



	}

		UpdateData();

	return TRUE;

}

BOOL RealTime::SetChartTitle(int chartno, string text)
{
	CWinApp* pApp = AfxGetApp();
	string astr;
	m_Title[chartno]=text;
		switch(chartno) {
				case 0:
					astr="A";
					break;
				case 1:
					astr="B";
					break;
				case 2:
					astr="C";
					break;
				case 3:
					astr="D";
					break;
				default:
					return FALSE;
		}
		MFC_Util::WriteProfileString("Title_"+astr, text);

	return TRUE;

}
int RealTime::GetChartType(int chartno)
{
	return m_PlotType.at(chartno-1);
}
BOOL RealTime::SetChartType(int chartno, int ityp)
{
	m_PlotType[chartno-1]=ityp;
	return TRUE;
}
void RealTime::OnBnClickedMultiSimView()
{
	// TODO: Add your control notification handler code here
//	m_MultiSimDisplay=m_MultiSimView2.GetCheck();
	//UpdateData();

//	m_gridRealTime.SetMultiView(m_MultiSimDisplay);
	//m_gridRealTime.OnSetup();
}

void RealTime::OnBnClickedButtonInterupt()
{
		m_pSimDoc->Set_MultiStopped();
		//MessageBox("Please wait while multirun simulation will be closed ! ");

}
void*  RealTime::CreateXYChart(int yrow, int xcol, int yplotheight, int xplotwidth) 
{
	
	int ytop=m_pProp->GetTopMargin();
	int ybottom=m_pProp->GetBottomMargin();;
	int ydiff=int(float(m_pProp->GetGapMargin())*(1.-m_pUtil->GetGapHorFraction()));
	int xdiff=int(float(m_pProp->GetGapMargin())*m_pUtil->GetGapHorFraction());
	int xdiff1, xdiff2, ydiff1, ydiff2;
	xdiff1=int(float(xdiff)*m_pUtil->GetGapHorLeftFraction());
	xdiff2=xdiff-xdiff1;
	ydiff1=int(float(ydiff)*m_pUtil->GetGapVertTopFraction());
	ydiff2=ydiff-ydiff1;

	int xleft=m_pProp->GetLeftMargin();
	int xright=m_pProp->GetRightMargin();

	BOOL vertical_legends;
	
	vertical_legends=m_pProp->GetVerticalLegends();
	XYChart *c;
		BOOL createlegend;

		int legendposX, legendposY;
		legendposX=m_pUtil->GetLegendXPos();
		legendposY=m_pUtil->GetLegendYPos();

		createlegend=m_pUtil->MakeLegends();
	int ColorLegendBackground, ColorLegendBackgroundFrame, LegendSizePoint;
	ColorLegendBackground=m_pUtil->GetColorLegendBackground();
	ColorLegendBackgroundFrame=m_pUtil->GetColorLegendBackgroundFrame();
	LegendSizePoint=m_pUtil->m_LegendSizePoint;

	int	visible_legends;
	visible_legends=m_pProp->GetVisibleFrames();

	if(visible_legends==0) {
		ColorLegendBackground=Chart::Transparent;
		ColorLegendBackgroundFrame=Chart::Transparent;	
	}
	else if(visible_legends<4) {
		int dim=(4-visible_legends)*80*0x1000000;
		ColorLegendBackground+=dim;
		ColorLegendBackgroundFrame+=dim;	
	}



	if(yrow==0&&xcol==0) {
			if(m_NumChartX==1&&m_NumChartY==1) {
				c = new  XYChart(xplotwidth+xleft+xright, yplotheight+ytop+ybottom, m_ColorBackground);
				m_pProp->m_MakeChartTitle=TRUE;
			}
			else if(m_NumChartY==1) {
				c = new  XYChart(xplotwidth+xleft+xdiff2, yplotheight+ytop+ybottom, m_ColorBackground);
				m_pProp->m_MakeChartTitle=TRUE;
			}
			else if(m_NumChartX==1){
				c = new  XYChart(xplotwidth+xleft+xright, yplotheight+ytop+ydiff2, m_ColorBackground);
				m_pProp->m_NoXAxisScale=TRUE;
				m_pProp->m_MakeChartTitle=TRUE;
			}
			else {
				c = new  XYChart(xplotwidth+xleft+xdiff, yplotheight+ytop+ydiff2, m_ColorBackground);
				m_pProp->m_NoXAxisScale=TRUE;
			}
			c->setPlotArea(xleft, ytop, xplotwidth, yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend||m_pUtil->MakeSingleLegends())
				c->addLegend(legendposX+xleft, legendposY+ytop,vertical_legends, "arialbd.ttf", LegendSizePoint)->setBackground(ColorLegendBackground, ColorLegendBackgroundFrame);
		}
		else if(yrow==0&&xcol==m_NumChartX-1) {
			if(m_NumChartY==1)
				c = new  XYChart(xplotwidth+xright+xdiff1, yplotheight+ytop+ybottom, m_ColorBackground);
			else {
				c = new  XYChart(xplotwidth+xright+xdiff1, yplotheight+ytop+ydiff2, m_ColorBackground);
				m_pProp->m_NoXAxisScale=TRUE;
			}
			m_pProp->m_NoYAxisScale=TRUE;
			c->setPlotArea(xdiff1, ytop, xplotwidth , yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend)
				c->addLegend(legendposX+xdiff, legendposY+ytop, vertical_legends, "arialbd.ttf", LegendSizePoint)->setBackground(ColorLegendBackground, ColorLegendBackgroundFrame);
		}
		else if(yrow==0) {
			if(m_NumChartY==1)
				c = new  XYChart(xplotwidth+xdiff, yplotheight+ytop+ybottom, m_ColorBackground);
			else {
				c = new  XYChart(xplotwidth+xdiff, yplotheight+ytop+ydiff2, m_ColorBackground);
				m_pProp->m_NoXAxisScale=TRUE;
			}
			if(xcol==m_NumChartX/2)
				m_pProp->m_MakeChartTitle=TRUE;
			m_pProp->m_NoYAxisScale=TRUE;
			c->setPlotArea(xdiff1, ytop, xplotwidth , yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend)
				c->addLegend(legendposX+xdiff1, legendposY+ytop, vertical_legends, "arialbd.ttf", LegendSizePoint)->setBackground(ColorLegendBackground, ColorLegendBackgroundFrame);
		}
		else if(yrow==m_NumChartY-1&&xcol==0) {
			if(m_NumChartX==1)
				c = new  XYChart(xplotwidth+xleft+xright, yplotheight+ybottom+ydiff1, m_ColorBackground);
			else 
				c = new  XYChart(xplotwidth+xleft+xdiff, yplotheight+ybottom+ydiff1, m_ColorBackground);
			c->setPlotArea(xleft, ydiff1, xplotwidth, yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend)
				c->addLegend(legendposX+xleft, legendposY+ydiff1, vertical_legends, "arialbd.ttf", LegendSizePoint)->setBackground(ColorLegendBackground, ColorLegendBackgroundFrame);

		}
		else if(yrow==m_NumChartY-1&&xcol==m_NumChartX-1) {
			c = new  XYChart(xplotwidth+xright+xdiff1, yplotheight+ybottom+ydiff1, m_ColorBackground);
			m_pProp->m_NoYAxisScale=TRUE;
			c->setPlotArea(xdiff1, ydiff1,xplotwidth, yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend)
				c->addLegend(legendposX+xdiff1, legendposY+ydiff1, vertical_legends, "arialbd.ttf", LegendSizePoint)->setBackground(ColorLegendBackground, ColorLegendBackgroundFrame);

		}
		else if(yrow==m_NumChartY-1) {
			c = new  XYChart(xplotwidth+xdiff, yplotheight+ybottom+ydiff, m_ColorBackground);
			m_pProp->m_NoYAxisScale=TRUE;
			c->setPlotArea(xdiff1, ydiff1,xplotwidth , yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend)
				c->addLegend(legendposX+xdiff1, legendposY+ydiff1, vertical_legends, "arialbd.ttf", LegendSizePoint)->setBackground(ColorLegendBackground, ColorLegendBackgroundFrame);

		}
		else if(xcol==0) {
			c = new  XYChart(xplotwidth+xleft+xdiff2, yplotheight+ydiff, m_ColorBackground);
			m_pProp->m_NoXAxisScale=TRUE;
			c->setPlotArea(xleft, ydiff1,xplotwidth , yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend)
				c->addLegend(legendposX+xleft, legendposY+ydiff1, vertical_legends, "arialbd.ttf", LegendSizePoint)->setBackground(ColorLegendBackground, ColorLegendBackgroundFrame);

		}
		else if(xcol==m_NumChartX-1) {
			c = new  XYChart(xplotwidth+xright+xdiff1, yplotheight+ydiff, m_ColorBackground);
			m_pProp->m_NoXAxisScale=TRUE;
			m_pProp->m_NoYAxisScale=TRUE;
			c->setPlotArea(xdiff1, ydiff1, xplotwidth, yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend)
				c->addLegend(legendposX+xdiff1, legendposY+ydiff1, vertical_legends, "arialbd.ttf", LegendSizePoint)->setBackground(ColorLegendBackground, ColorLegendBackgroundFrame);

		}
		else {
			c = new  XYChart(xplotwidth+xdiff, yplotheight+ydiff, m_ColorBackground);
			m_pProp->m_NoXAxisScale=TRUE;
			m_pProp->m_NoYAxisScale=TRUE;
			c->setPlotArea(xdiff1, ydiff1,xplotwidth , yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
			if(createlegend)
				c->addLegend(legendposX+xdiff1, legendposY+ydiff1, vertical_legends, "arialbd.ttf", LegendSizePoint)->setBackground(ColorLegendBackground, ColorLegendBackgroundFrame);

		}

		
	return c; 

}
void RealTime::CleanRealTimePlot() {



}
