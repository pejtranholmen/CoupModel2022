// zoomscrolldemoDlg.cpp : implementation file
//

#include "..\stdafx.h"
#include "..\Model\SimB.h"

#include "..\CoupModel.h"
#include "..\CoupModelDoc.h"

#include "zoomscrolldemoDlg.h"
#include "HotSpotDlg.h"
#include "chartdir.h"
#include <math.h>
#include <time.h>
#include <algorithm>
#include ".\zoomscrolldemodlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

double TT[100000];
double AA[32][100000];
double BX[32][100];
double BY[32][100];


extern "C" int PARAMETERS_mp_NUMBEROFCURVES;
extern "C" int PARAMETERS_mp_ACTUALCURVE;
extern "C" int PARAMETERS_mp_SHIFTCURVE;
extern "C" int PARAMETERS_mp_DEFINEPLOT;
extern "C" int PARAMETERS_mp_NUMBERCHLEGEND;
extern "C" char PARAMETERS_mp_CHLEGEND[80];


extern "C" int PLOTPF_mp_NT;
extern "C" int PLOTPF_mp_ACTUALLEVEL;
extern "C" float PLOTPF_mp_MODPRESSURE[12];
extern "C" float PLOTPF_mp_MODTHETA[12];
extern "C" void   PLOTPF_mp_RETRIEVEPF_POINTS();
extern "C" void   PLOTPF_mp_OPENBASE();
extern "C" BOOL PLOTPF_mp_OPENDATABASE;
/////////////////////////////////////////////////////////////////////////////
// CZoomscrolldemoDlg dialog

//
// Constructor
//
CZoomscrolldemoDlg::CZoomscrolldemoDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CZoomscrolldemoDlg::IDD, pParent)
	, m_CommonScale(FALSE)
	, m_logscale(FALSE)
	, m_Cumulated(FALSE)
	, m_RegLine(false)
	, m_AddLine(FALSE)
{
    //{{AFX_DATA_INIT(CZoomscrolldemoDlg)
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_minValue=m_maxValue=0.;
	m_minXValue=m_maxXValue=0.;
	m_ColorLine[0]=0xff0000;
	m_ColorLine[1]=0x00cc00;
	m_ColorLine[2]=0x0000ff;
	m_ColorLine[3]=0x008b0000;
	m_ColorLine[4]=0x002f4f4f;
	m_ColorLine[5]=0x0000008b;
	m_ColorLine[6]=0x00ff00ff;
	m_ColorLine[7]=0x0000ffff;
	m_ColorLine[8]=0x007fff00;
	m_ColorLine[9]=0x00ffff00;
	for(int i=0;i<20;i++)
		m_PointType[i]=FALSE;
	m_MultiRunPlot=FALSE;
	m_pCP=NULL;
	m_AddOnlyValidRuns=TRUE;
	m_ZoomX=FALSE;
	m_ZoomXY=FALSE;
	m_PFMeasuredPoints=FALSE;
	m_ExtraCurveCounter=-1;

}

//
// Destructor
//
CZoomscrolldemoDlg::~CZoomscrolldemoDlg()
{
   // delete m_dataTable;
    delete m_ChartViewer.getChart();
}

void CZoomscrolldemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZoomscrolldemoDlg)
	DDX_Control(pDX, IDC_StartDate, m_StartDate);
	DDX_Control(pDX, IDC_Duration, m_Duration);
	DDX_Control(pDX, IDC_VScrollBar, m_VScrollBar);
	DDX_Control(pDX, IDC_HScrollBar, m_HScrollBar);
	DDX_Control(pDX, IDC_XZoomPB, m_XZoomPB);
	DDX_Control(pDX, IDC_PointerPB, m_PointerPB);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHARTVIEWERZOOM, m_ChartViewer);
	DDX_Check(pDX, IDC_COMMONSCALE, m_CommonScale);
	DDX_Check(pDX, IDC_LOGSCALE, m_logscale);
	DDX_Check(pDX, IDC_CUMULATE, m_Cumulated);
	DDX_Check(pDX, IDC_REG_LINE, m_RegLine);
	DDX_Check(pDX, IDC_LINE, m_AddLine);
}

BEGIN_MESSAGE_MAP(CZoomscrolldemoDlg, CDialog)
    //{{AFX_MSG_MAP(CZoomscrolldemoDlg)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_DESTROY()
	ON_WM_ACTIVATE()

//ON_MESSAGE(WM_THREADENDED, OnThreadended)
    ON_BN_CLICKED(IDC_PointerPB, OnPointerPB)
    ON_BN_CLICKED(IDC_ZoomInPB, OnZoomInPB)
    ON_BN_CLICKED(IDC_ZoomOutPB, OnZoomOutPB)
    ON_BN_CLICKED(IDC_XZoomPB, OnXZoomPB)
    ON_BN_CLICKED(IDC_XYZoomPB, OnXYZoomPB)
    ON_BN_CLICKED(IDC_CHARTVIEWERZOOM, OnChartViewer)
    ON_CONTROL(CVN_ViewPortChanged, IDC_CHARTVIEWERZOOM, OnViewPortChanged)
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
  //  ON_NOTIFY(DTN_DATETIMECHANGE, IDC_StartDate, OnDatetimechangeStartDate)
    ON_CBN_SELCHANGE(IDC_Duration, OnSelchangeDuration)
    ON_CBN_KILLFOCUS(IDC_Duration, OnKillfocusDuration)
    //}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedSaveFile)
	ON_BN_CLICKED(IDC_COMMONSCALE, OnBnClickedCommonscale)
	ON_BN_CLICKED(IDC_CUMULATE, OnBnClickedCumulate)
	ON_BN_CLICKED(IDC_LOGSCALE, OnBnClickedCommonscale)
	ON_BN_CLICKED(IDC_REG_LINE, OnBnClickedCommonscale)
	ON_BN_CLICKED(IDC_LINE, OnBnClickedCommonscale)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZoomscrolldemoDlg message handlers

//
// Initialization
//
CPG* CZoomscrolldemoDlg::GetSimFile(CCoupModelDoc *pDoc)
{
		CString dir=pDoc->GetCurrentDirectory();
		CString filename, str;
		BOOL validpg;
		str=ItoAscii(pDoc->m_SimulationRunNo);
		if(str.GetLength()>4) 
			filename=dir+"COUP_"+str+".Bin";
		else if(str.GetLength()==4)
			filename=dir+"COUP_0"+str+".Bin";	
		else if(str.GetLength()==3)
			filename=dir+"COUP_00"+str+".Bin";
		else if(str.GetLength()==2)
			filename=dir+"COUP_000"+str+".Bin";
		else if(str.GetLength()==1)
			filename=dir+"COUP_0000"+str+".Bin";
		else
			filename=dir+"COUP_"+str+".Bin";
		int num=pDoc->CPG_GetNumber();
		CPG *pCPG;
		if(num>0) {
			pCPG=pDoc->CPG_GetAt(num-1);
			if(pCPG->IsOpen()) {
				m_noOfPoints=pCPG->GetNumRecords();
				validpg=TRUE;
			}
		}
		else {
			pCPG= new CPG();
			validpg=pCPG->Open(filename);
			if(validpg) {
				pDoc->CPG_Add(pCPG);		
			}
			else
				pCPG=NULL;
		}
		return pCPG;
}
BOOL CZoomscrolldemoDlg::InitData(CCoupModelDoc *pDoc, int chartnum, int charttype, CString PGFile)
{
	CString str;
	BOOL validpg;
	CString filename;
	if(m_pDoc!=NULL&&pDoc->IsFinishedSimulation()) {
		m_pDoc=pDoc;
		m_MultiRunPlot=FALSE;
		m_pCP=NULL  ; // No parameter value plot

		m_ChartNumber=chartnum;
		m_ChartType=charttype;
		if(PGFile=="") {
		CString dir=pDoc->GetCurrentDirectory();

		str=ItoAscii(m_pDoc->m_SimulationRunNo);
		if(str.GetLength()==5) 
			filename=dir+"COUP_"+str+".Bin";
		if(str.GetLength()==4) 
			filename=dir+"COUP_0"+str+".Bin";
		else if(str.GetLength()==3)
			filename=dir+"COUP_00"+str+".Bin";
		else if(str.GetLength()==2)
			filename=dir+"COUP_000"+str+".Bin";
		else if(str.GetLength()==1)
			filename=dir+"COUP_0000"+str+".Bin";
		else
			filename=dir+"COUP_"+str+".Bin";
		}
		else
			filename=PGFile;
		int num=m_pDoc->CPG_GetNumber();
		CPG *pCPG;
		if(num>0) {
			pCPG=m_pDoc->CPG_GetAt(num-1);
			if(pCPG->IsOpen()) {
				if(pCPG->GetFileName()!=filename) {
					pCPG= new CPG();
					validpg=pCPG->Open(filename);
					if(validpg) m_pDoc->CPG_Add(pCPG);

				}
				validpg=TRUE;
			}
		}
		else {
			pCPG= new CPG();
			validpg=pCPG->Open(filename);
			if(validpg) {
				m_pDoc->CPG_Add(pCPG);		
				m_pDoc->UpdateXTGDIndexToPG(pCPG);
			}
		}
		if(validpg) {
			m_numPGPtr=m_pDoc->CPG_GetNumber()-1;
				if(m_ChartType==0) {
					// Check for two variables to make scatter plot			
						int  ilocal, itype, ichart, icharttype;
						CBase *pX;
						m_NumVarSelected=0;
						for (int j=0;j<m_pDoc->PlotV_NumSelected();j++) {
							if(ichart==m_ChartNumber) {
								m_NumVarSelected++;
							}
						}
						if(m_NumVarSelected<2) {
							MessageBox("Independent variable select. Select also dependent variables !");
							return FALSE;
						}
				}
				return TRUE;
		}
		return FALSE;		
	}
	return FALSE;
}

BOOL CZoomscrolldemoDlg::InitData(CCoupModelDoc *pDoc, CP *pCP, CPt *pCPt, int index)
{
	CString str;
	BOOL validpg;
	if(m_pDoc!=NULL) {
		m_pDoc=pDoc;
		m_pCP=pCP;
		m_pCPt=pCPt;
		m_TableIndex=index;
		m_ChartType=0;
		PARAMETERS_mp_DEFINEPLOT=1; 
		PARAMETERS_mp_SHIFTCURVE=0;

		if(pCP->IsKindOf(RUNTIME_CLASS(CTab))) {
			m_Title=((CTab*)m_pCP)->GetViewTitle();
			if(((CTab*)m_pCP)->GetViewFunction()==401){

				if(!PLOTPF_mp_OPENDATABASE) m_pDoc->SP_OpenDataBase();


				PLOTPF_mp_RETRIEVEPF_POINTS();
				if(PLOTPF_mp_NT>0) 
					m_PFMeasuredPoints=TRUE;
			}
			

		}
		else 
			m_Title=m_pCP->GetViewTitle();
		if(m_Title!="") {
			PARAMETERS_mp_NUMBEROFCURVES=1;
			PARAMETERS_mp_ACTUALCURVE=1;
			m_ViewCounter=1;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CZoomscrolldemoDlg::InitMultiRunPlot(CCoupModelDoc *pDoc,int chartnum, int charttype)
{
	CString str;
	m_pDoc=pDoc;
	if(m_pDoc!=NULL) {
			// Check and open MultiRunFile
		if(m_pDoc->MR_Storage_Open()) {
			m_MultiRunPlot=TRUE;
			m_pCP=NULL;
			m_ChartNumber=chartnum;
			m_ChartType=charttype;
			if(m_ChartType==0) {
					// Check for two variables to make scatter plot			
					int  itype, ichart, icharttype;

						m_NumVarSelected=0;
						for (int j=0;j<m_pDoc->PlotV_NumSelected();j++) {
							m_noOfPoints=m_pDoc->MR_Get_TotalNumberofRuns();
							itype=m_pDoc->PlotV_GetType(j);
							ichart=m_pDoc->PlotV_GetChart(j);
							icharttype=m_pDoc->PlotV_GetChartType(j);
							if(ichart==m_ChartNumber) {
								m_NumVarSelected++;
							}
						}
						if(m_NumVarSelected<2) {
							MessageBox("Independent variable select. Select also dependent variables !");
							return FALSE;
						}
				}




			return TRUE;
		}
	}
	return FALSE;
}
BOOL CZoomscrolldemoDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // *** code automatically generated by VC++ MFC AppWizard ***
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    
    // Load icons to mouse usage buttons
    loadButtonIcon(IDC_PointerPB, IDI_PointerPB, 100, 20);  
    loadButtonIcon(IDC_ZoomInPB, IDI_ZoomInPB, 100, 20);    
    loadButtonIcon(IDC_ZoomOutPB, IDI_ZoomOutPB, 100, 20);

    // Load icons to zoom/scroll direction control buttons
    loadButtonIcon(IDC_XZoomPB, IDI_XZoomPB, 105, 20);
    loadButtonIcon(IDC_XYZoomPB, IDI_XYZoomPB, 105, 20);

    //
    // Initialize member variables
    //
    m_extBgColor = getDefaultBgColor();     // Default background color
    m_minValue = m_maxValue = 0;   	// y axes ranges
	m_minXValue = m_maxXValue =0;   // x axex ranges

    // Load the data
    loadData();
	CString string;
	if(m_pCP==NULL)
		string="CoupModel Chart :"+ItoAscii(m_ChartNumber)+", Run No :"+ItoAscii(m_pDoc->m_SimulationRunNo)+" "+m_pDoc->m_Comments;
	else
		string="CoupModel Parameter Function Chart, Run No:"+ItoAscii(m_pDoc->m_SimulationRunNo);
	SetWindowText(string);

    //
    // In this demo, we deduce the horizontal scroll range from the data.
    //
	if(m_ChartType==0) {
		 if(m_NumVarSelected<2) {
			 MessageBox("Select a minimum of 2 variables !");
			 return FALSE;
		}

    // Earliest date is the first timestamp
		 m_minDate = AA[0][0];
    // Duration is the seconds elapsed of the last timestamp from the earliest date
		 m_dateRange = AA[0][m_noOfPoints-1] - m_minDate;

	  //m_StartDate.SetRange(&startDate, &endDate);

	}
	else if(!m_MultiRunPlot) {
    // Earliest date is the first timestamp
		m_minDate = m_timeStamps[0];
		// Duration is the seconds elapsed of the last timestamp from the earliest date
		m_dateRange = m_timeStamps[m_timeStamps.len - 1] - m_minDate;

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
		m_StartDate.SetRange(&startDate, &endDate);
		 m_minDuration = 600;
		m_Duration.Clear();
		CString text;
		float days=0.5;
		for(int i=0;i<14;i++) {
			days=days*2;
			text=ItoAscii(int(days));
			if(days<m_dateRange/86400)
				m_Duration.AddString(text);
		}
		text=ItoAscii(int(m_dateRange/86400));
		m_Duration.AddString(text);



	}
	else {
			// In this demo, the maximum zoom-in is set to 10 days (1 day = 86400 seconds)
			m_minDuration = 1;

			m_minDate = m_timeStamps[0];
				// Duration is the seconds elapsed of the last timestamp from the earliest date
			m_dateRange = m_timeStamps[m_timeStamps.len - 1] - m_minDate;



			m_Duration.Clear();
			CString text;
			float days=0.5;
			for(int i=0;i<14;i++) {
				days=days*2;
				text=ItoAscii(int(days));
				if(days<m_dateRange)
					m_Duration.AddString(text);
			}
			text=ItoAscii(int(m_dateRange));
			m_Duration.AddString(text);
	}
    // Set up the ChartViewer to reflect the visible and minimum duration
    m_ChartViewer.setZoomInWidthLimit(m_minDuration / m_dateRange);
    m_ChartViewer.setViewPortWidth(m_currentDuration / m_dateRange);
    m_ChartViewer.setViewPortLeft(1 - m_ChartViewer.getViewPortWidth());

    // Initially set the mouse to drag to scroll mode in horizontal direction.
    m_PointerPB.SetCheck(1);
    m_XZoomPB.SetCheck(1);
    m_ChartViewer.setMouseUsage(Chart::MouseUsageScroll);

    // Can update chart now
    m_ChartViewer.updateViewPort(true, true);
    return TRUE;
}

// *** code automatically generated by VC++ MFC AppWizard ***
// If you add a minimize button to your dialog, you will need the code below
// to draw the icon.  For MFC applications using the document/view model,
// this is automatically done for you by the framework.
void CZoomscrolldemoDlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// *** code automatically generated by VC++ MFC AppWizard ***
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CZoomscrolldemoDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

//
// User clicks on the Pointer pushbutton
//
void CZoomscrolldemoDlg::OnPointerPB() 
{
    m_ChartViewer.setMouseUsage(Chart::MouseUsageScroll);   
}

//
// User clicks on the Zoom In pushbutton
//
void CZoomscrolldemoDlg::OnZoomInPB() 
{
    m_ChartViewer.setMouseUsage(Chart::MouseUsageZoomIn); 
	if(m_ZoomXY==FALSE)
		m_ZoomX=TRUE;
}

//
// User clicks on the Zoom Out pushbutton
//
void CZoomscrolldemoDlg::OnZoomOutPB() 
{
    m_ChartViewer.setMouseUsage(Chart::MouseUsageZoomOut);  
}

//
// User clicks on the X-Zoom pushbutton
//
void CZoomscrolldemoDlg::OnXZoomPB() 
{
    m_ChartViewer.setZoomDirection(Chart::DirectionHorizontal); 
    m_ChartViewer.setScrollDirection(Chart::DirectionHorizontal);

    // Viewport is always unzoomed as y-axis is auto-scaled
    m_ChartViewer.setViewPortTop(0);
    m_ChartViewer.setViewPortHeight(1);
        
    // Update chart to auto-scale axis
    m_ChartViewer.updateViewPort(true, true);

	m_ZoomX=TRUE;
	m_ZoomXY=FALSE;
}

//
// User clicks on the XY-Zoom pushbutton
//
void CZoomscrolldemoDlg::OnXYZoomPB() 
{
    m_ChartViewer.setZoomDirection(Chart::DirectionHorizontalVertical); 
    m_ChartViewer.setScrollDirection(Chart::DirectionHorizontalVertical);  
	m_ZoomX=FALSE;
	m_ZoomXY=TRUE;
}

//
// User selects a start date from the CDateTimeCtrl control
//
void CZoomscrolldemoDlg::OnDatetimechangeStartDate(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // Get the selected date
    CTime startDate;
    m_StartDate.GetTime(startDate);

    // Compute the new view port position based on the selected date
    m_ChartViewer.setViewPortLeft(
       (Chart::chartTime2((int)startDate.GetTime()) - m_minDate) / m_dateRange);
    m_ChartViewer.updateViewPort(true, true);

    *pResult = 0;
}

//
// User selects a duration from the Duration combo box
//
void CZoomscrolldemoDlg::OnSelchangeDuration() 
{
    // Get the selected duration
    CString text;
    m_Duration.GetLBText(m_Duration.GetCurSel(), text);

    // Validate and update the chart
    validateDuration(text);
}

//
// The Duration combo box lost focus (User may have entered a new duration.)
//
void CZoomscrolldemoDlg::OnKillfocusDuration() 
{
    // Get the duration text
    CString text;
    m_Duration.GetWindowText(text);

    // Validate and update the chart
    validateDuration(text);
}

//
// User presses "Enter" key. (User may have entered a new duration.)
//
void CZoomscrolldemoDlg::OnOK()
{
    // Same processing as OnKillfocusDuration
    OnKillfocusDuration();
}

//
// User clicks on the the horizontal scroll bar 
//
void CZoomscrolldemoDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    if (nSBCode != SB_ENDSCROLL)
    {
        // User is still scrolling

        // Set the view port based on the scroll bar
        m_ChartViewer.setViewPortLeft(moveScrollBar(nSBCode, nPos, pScrollBar));
        // Update the chart image only, but no need to update the image map.
        m_ChartViewer.updateViewPort(true, false);
    }
    else
        // Scroll bar has stoped moving. Can update image map now.
        m_ChartViewer.updateViewPort(false, true);
}       

//
// User clicks on the the vertical scroll bar 
//
void CZoomscrolldemoDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    if (nSBCode != SB_ENDSCROLL)
    {
        // User is still scrolling

        // Set the view port based on the scroll bar
        m_ChartViewer.setViewPortTop(moveScrollBar(nSBCode, nPos, pScrollBar));
        // Update the chart image only, but no need to update the image map.
        m_ChartViewer.updateViewPort(true, false);
    }
    else
        // Scroll bar has stoped moving. Can update image map now.
        m_ChartViewer.updateViewPort(false, true);
}

//
// CChartViewer ViewPortChanged event
//
void CZoomscrolldemoDlg::OnViewPortChanged()
{
    //
    // Set up the scroll bars to reflect the current position and size of the view port
    //
    SCROLLINFO info;
    info.cbSize = sizeof(SCROLLINFO);
    info.fMask = SIF_ALL;
    info.nMin = 0;
    info.nMax = 0x1fffffff;

	if(m_ChartType==0) {
	
		drawChart(&m_ChartViewer);
        updateImageMap(&m_ChartViewer);
		return;
	}
    
    m_HScrollBar.EnableWindow(m_ChartViewer.getViewPortWidth() < 1);
    if (m_ChartViewer.getViewPortWidth() < 1)
    {
        info.nPage = (int)ceil(m_ChartViewer.getViewPortWidth() * (info.nMax - info.nMin));
        info.nPos = (int)(0.5 + m_ChartViewer.getViewPortLeft() * (info.nMax - info.nMin))
            + info.nMin;
        m_HScrollBar.SetScrollInfo(&info);
    }

    m_VScrollBar.EnableWindow(m_ChartViewer.getViewPortHeight() < 1);
    if (m_ChartViewer.getViewPortHeight() < 1)
    {
        info.nPage = (int)ceil(m_ChartViewer.getViewPortHeight() * (info.nMax - info.nMin));
        info.nPos = (int)(0.5 + m_ChartViewer.getViewPortTop() * (info.nMax - info.nMin))
            + info.nMin;
        m_VScrollBar.SetScrollInfo(&info);
    }

    //
    // Set the start date CDateTimeCtrl control and duration combo box to reflect the current
    // position and size of the view port.
    //
	

    // Compute the start date (in chartTime) and duration (in seconds) of the view port
    double currentStartDate = m_minDate + (int)(0.5 + m_ChartViewer.getViewPortLeft() 
        * m_dateRange);
    m_currentDuration = (int)(0.5 + m_ChartViewer.getViewPortWidth() * m_dateRange);

	if(!m_MultiRunPlot) {
    // Set the CDateTimeCtrl control to reflect the start date
		int startYMD = Chart::getChartYMD(currentStartDate);
		int startHMS = (int)fmod(currentStartDate, 86400);
		CTime startDate = CTime(startYMD / 10000, (startYMD % 10000) / 100, startYMD % 100,
			startHMS / 3600, (startHMS % 3600) / 60, startHMS % 60);
		m_StartDate.SetTime(&startDate);

		// Set the duration combo box to reflect the duration (in days)
		CString buffer;
		buffer.Format(_T("%d"), (int)(0.5 + m_currentDuration / 86400));
		m_Duration.SetWindowText(buffer);

	}


    //
    // Update chart and image map if necessary
    //
    if (m_ChartViewer.needUpdateChart())
        drawChart(&m_ChartViewer);
    if (m_ChartViewer.needUpdateImageMap())
        updateImageMap(&m_ChartViewer);
}

//
// User clicks on the CChartViewer
//
void CZoomscrolldemoDlg::OnChartViewer() 
{
    ImageMapHandler *handler = m_ChartViewer.getImageMapHandler();
    if (0 != handler)
    {
        //
        // Query the ImageMapHandler to see if the mouse is on a clickable hot spot. We 
        // consider the hot spot as clickable if its href ("path") parameter is not empty.
        //
        const char *path = handler->getValue("path");
        if ((0 != path) && (0 != *path))
        {
            // In this sample code, we just show all hot spot parameters.
            CHotSpotDlg hs;
            hs.SetData(handler);
            hs.DoModal();
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// CZoomscrolldemoDlg methods

//
// Load the data
//
BOOL CZoomscrolldemoDlg::IsNewViewFunc(CP* pCPAdd)
{
	int viewfunc;
	if(pCPAdd->IsKindOf(RUNTIME_CLASS(CTab)))
		viewfunc=((CTab*)pCPAdd)->GetViewFunction();
	else
		viewfunc=pCPAdd->GetViewFunction();
	if(viewfunc==m_PreviousViewFunction) 
		return FALSE;
	else
		return TRUE;

}

BOOL CZoomscrolldemoDlg::AddData(CP* pCPAdd, BOOL extra, CPt *pCPt, int tabindex)
{
		//if(pCPAdd!=m_pCP)
		//	return FALSE;
		//m_ViewCounter++;
	int viewfunc;
	CTab *pTab;
	if(pCPAdd->IsKindOf(RUNTIME_CLASS(CTab))) {
		pTab=(CTab*)pCPAdd;
		viewfunc=((CTab*)pCPAdd)->GetViewFunction();
		m_Title=pTab->GetViewTitle();

	}
	else {
		viewfunc=pCPAdd->GetViewFunction();		
		m_Title=m_pCP->GetViewTitle();

	}


		if(viewfunc==m_PreviousViewFunction&&!extra) {
			m_ViewCounter++;
			m_CurveCounter=m_NumVarSelected-1;
			PARAMETERS_mp_DEFINEPLOT=1; 
			PARAMETERS_mp_SHIFTCURVE=0;
		
			m_noOfPoints=m_pCP->GetChartPoints();
			float GXmin=m_pCP->GetChartMin();
			float GXmax=m_pCP->GetChartMax();
			float x, y;

			for(int j=0; j<PARAMETERS_mp_NUMBEROFCURVES; j++){
				int nCurve=j+m_CurveCounter;
				if(PARAMETERS_mp_NUMBEROFCURVES<=1)
					PARAMETERS_mp_ACTUALCURVE=tabindex+1;
				else
					PARAMETERS_mp_ACTUALCURVE=j+1;
				for(int i=0;i<m_noOfPoints;i++) {
					x=float(float(i)/m_noOfPoints*(GXmax-GXmin))+GXmin;
					if(pCPAdd->IsKindOf(RUNTIME_CLASS(CTab))) 
						y=pTab->GetViewValue(x);			
					else
						y=m_pCP->GetViewValue(x);
					AA[j+m_CurveCounter+1][i]=double(y);
				}
				m_dataSeriesA[j+m_CurveCounter+1] = DoubleArray(AA[j+m_CurveCounter+1], m_noOfPoints);
				CString str;	
				if(pCPAdd->IsKindOf(RUNTIME_CLASS(CTab))) {
					str=pCPt->GetName()+"("+ItoAscii(tabindex+1)+") : "+FtoAscii(pCPt->GetValue(tabindex));
				}
				else {
					str=m_pCP->GetName()+" : "+FtoAscii(*(float*)m_pCP->GetValue());
				}
				m_Legends.SetAtGrow(j+m_CurveCounter,str);
			
				if(m_PFMeasuredPoints&&!extra) {
					m_ExtraCurveCounter++;
					PLOTPF_mp_ACTUALLEVEL=tabindex+1;
					PLOTPF_mp_RETRIEVEPF_POINTS();
					for(int i=0; i<PLOTPF_mp_NT;i++) {
						if(PLOTPF_mp_MODPRESSURE[i]>0)
							BY[nCurve][i]=log10(double(PLOTPF_mp_MODPRESSURE[i]));
						else
							BY[nCurve][i]=Chart::NoValue;
						if(PLOTPF_mp_MODTHETA[i]>0)
							BX[nCurve][i]=double(PLOTPF_mp_MODTHETA[i]);
						else
							BX[nCurve][i]=Chart::NoValue;
					}
					m_noOfExtraPoint=PLOTPF_mp_NT;
					m_dataSeriesB[nCurve] = DoubleArray(BX[nCurve],m_noOfExtraPoint);
					m_dataSeriesC[nCurve] = DoubleArray(BY[nCurve],m_noOfExtraPoint);
				}
			}
			m_NumVarSelected=PARAMETERS_mp_NUMBEROFCURVES+m_CurveCounter+1;
			drawChart(&m_ChartViewer);
			return TRUE;
		}
		return FALSE;


}
void CZoomscrolldemoDlg::loadData()
{

	// In this demo, we allow scrolling for the last 5 years.
    time_t t = time(0);
    struct tm *ymd = localtime(&t);
    double lastDate = Chart::chartTime(ymd->tm_year + 1900, ymd->tm_mon, ymd->tm_mday);
    double firstDate = Chart::chartTime(ymd->tm_year + 1900 - 5, ymd->tm_mon, ymd->tm_mday);

	CPG *pPG;
	CTab *pTab;
	int PGindex;
	int  ilocal, itype, ichart, icharttype;
	CBase *pX;
	m_NumVarSelected=0;
	double Acc;
	float x, y;

	if(m_pCP!=NULL) {

	PARAMETERS_mp_DEFINEPLOT=1; 
	PARAMETERS_mp_SHIFTCURVE=0;
	if(m_pCP->IsKindOf(RUNTIME_CLASS(CTab))) { //Parameter Table data
		pTab=(CTab*)m_pCP;
		m_Title=pTab->GetViewTitle();
		m_noOfPoints=m_pCP->GetChartPoints();
		float GXmin=m_pCP->GetChartMin();
		float GXmax=m_pCP->GetChartMax();
		m_LabelX=pTab->GetXLabel();
		m_LabelY=pTab->GetYLabel();

		m_PreviousViewFunction=pTab->GetViewFunction();


		for(int i=0;i<m_noOfPoints;i++) {
				x=float(float(i)/m_noOfPoints*(GXmax-GXmin))+GXmin;
				AA[0][i]=double(x);
		}
		m_dataSeriesA[0] = DoubleArray(AA[0], m_noOfPoints);

		for(int j=0; j<PARAMETERS_mp_NUMBEROFCURVES; j++){
			PARAMETERS_mp_ACTUALCURVE=m_TableIndex+1;
			for(int i=0;i<m_noOfPoints;i++) {
				x=float(float(i)/m_noOfPoints*(GXmax-GXmin))+GXmin;
				y=pTab->GetViewValue(x);			
				AA[j+1][i]=double(y);
			}
			CString str=m_pCPt->GetName()+"("+ItoAscii(m_TableIndex+1)+")";
			
			
			str+=" : "+FtoAscii(m_pCPt->GetValue(m_TableIndex));
			//CString str="kalle";
			m_Legends.SetAtGrow(0,str);
			m_dataSeriesA[j+1] = DoubleArray(AA[j+1], m_noOfPoints);

	
			if(m_PFMeasuredPoints) {
				m_ExtraCurveCounter=0;
				PLOTPF_mp_ACTUALLEVEL=m_TableIndex+1;
				PLOTPF_mp_RETRIEVEPF_POINTS();
				for(int i=0; i<PLOTPF_mp_NT;i++) {
					if(PLOTPF_mp_MODPRESSURE[i]>0)
						BY[j][i]=log10(double(PLOTPF_mp_MODPRESSURE[i]));
					else
						BY[j][i]=Chart::NoValue;
					if(PLOTPF_mp_MODTHETA[i]>0)
						BX[j][i]=double(PLOTPF_mp_MODTHETA[i]);
					else
						BX[j][i]=Chart::NoValue;
				}
				m_noOfExtraPoint=PLOTPF_mp_NT;
				CString str=m_pDoc->DB_GetChoice(Soil_Properties);
			
				int ipos=str.Find("\t");
				if(ipos>=0) {
					str=str.Mid(ipos+1);
					ipos=str.Find("\t");
					str=str.Mid(ipos+1);
					ipos=str.Find("\t");
					str=str.Left(ipos);
					str.TrimRight();
				}
				m_Title+="\n"+str;
				m_dataSeriesB[j] = DoubleArray(BX[j],m_noOfExtraPoint);
				m_dataSeriesC[j] = DoubleArray(BY[j],m_noOfExtraPoint);
			}
		}


	}
	else {

		m_Title=m_pCP->GetViewTitle();
		m_noOfPoints=m_pCP->GetChartPoints();
		float GXmin=m_pCP->GetChartMin();
		float GXmax=m_pCP->GetChartMax();
		m_LabelX=m_pCP->GetXLabel();
		m_LabelY=m_pCP->GetYLabel();

		m_PreviousViewFunction=m_pCP->GetViewFunction();
		
		
		for(int i=0;i<m_noOfPoints;i++) {
				x=float(float(i)/m_noOfPoints*(GXmax-GXmin))+GXmin;
				AA[0][i]=double(x);
		}
		m_dataSeriesA[0] = DoubleArray(AA[0], m_noOfPoints);

		for(int j=0; j<PARAMETERS_mp_NUMBEROFCURVES; j++){
			PARAMETERS_mp_ACTUALCURVE=j+1;
			for(int i=0;i<m_noOfPoints;i++) {
				x=float(float(i)/m_noOfPoints*(GXmax-GXmin))+GXmin;
				y=m_pCP->GetViewValue(x);			
				AA[j+1][i]=double(y);
			}
			CString str=m_pCP->GetName()+" : "+FtoAscii(*(float*)m_pCP->GetValue());
			m_Legends.SetAtGrow(0,str);
			m_dataSeriesA[j+1] = DoubleArray(AA[j+1], m_noOfPoints);
		}
	}
	m_NumVarSelected=PARAMETERS_mp_NUMBEROFCURVES+1;
	return;
	}

	m_ZoomX=FALSE;
	if(!m_MultiRunPlot) {
		int num=m_pDoc->CPG_GetNumber();
		if(num>m_numPGPtr)
			pPG=m_pDoc->CPG_GetAt(m_numPGPtr);

		if(!pPG->IsOpen()) {
			MessageBox("Could not open PG file");
			return;
		}
	}
	
	for (int j=0;j<m_pDoc->PlotV_NumSelected();j++) {
		if(m_MultiRunPlot) {
			m_noOfPoints=m_pDoc->MR_Get_TotalNumberofRuns();
		}
		else {
			m_noOfPoints=pPG->GetNumRecords();
			pX=m_pDoc->PlotV_GetPtr(j);		
		}
		ilocal=m_pDoc->PlotV_GetLocalIndex(j);
		itype=m_pDoc->PlotV_GetType(j);
		ichart=m_pDoc->PlotV_GetChart(j);
		icharttype=m_pDoc->PlotV_GetChartType(j);
		if(ichart==m_ChartNumber) {
			m_NumVarSelected++;
			switch(itype) {
				case 0:
					PGindex=ilocal;
					break;
				case 1:
				case 2:
					PGindex=((CXT*)pX)->GetPgFileIndex(ilocal);
					break;
				case 3:
				case 4:
					PGindex=((CGD*)pX)->GetPgFileIndex(ilocal);
					break;
				case 5: //Validation section
					PGindex=m_pDoc->GetNumOutPuts()+ilocal;
					break;
				case 7:
					m_ind=ilocal;
					break;
				case 6: // Validation Multi Data
					m_ind=ilocal/6;
					break;
				case 8:
					m_ind=ilocal;
					break;
		}
		Acc=0.0;
		
		if(m_NumVarSelected==1) {
			if(m_MultiRunPlot) {
				firstDate=double(1);
				lastDate=double(m_noOfPoints);
				for(int i=0;i<m_noOfPoints;i++) {
					TT[i]=double(i+1);
					if(itype==6)
						AA[m_NumVarSelected-1][i]=double(m_pDoc->m_MStorage.GetValid(i,ilocal));
					else if(itype==7)
						AA[m_NumVarSelected-1][i]=double(m_pDoc->m_MStorage.GetMultiP(i,ilocal));
					else if(itype==8)
						AA[m_NumVarSelected-1][i]=double(m_pDoc->m_MStorage.GetSumV(i,ilocal));
						
				}
			}
			else {
				firstDate=pPG->GetDoubleTimeStart();
				lastDate=pPG->GetDoubleTimeEnd();

				for(int i=0;i<m_noOfPoints;i++) {
					TT[i]=pPG->GetDoubleTime(i+1);
					if(pPG->GetVarValue(PGindex,i+1)<-1.E38)
						AA[m_NumVarSelected-1][i]=Chart::NoValue;
					else {
						AA[m_NumVarSelected-1][i]=double(pPG->GetVarValue(PGindex, i+1));
						if(m_Cumulated&&i>0) {
							Acc=Acc+AA[m_NumVarSelected-1][i];
							AA[m_NumVarSelected-1][i]=Acc;
						}
					}

			}
			}
			m_timeStamps = DoubleArray(TT, m_noOfPoints);
			m_dataSeriesA[0] = DoubleArray(AA[m_NumVarSelected-1], m_noOfPoints);
			m_dataSeriesA[1] = DoubleArray(AA[m_NumVarSelected-1], m_noOfPoints);
			m_dataSeriesA[2] = DoubleArray(AA[m_NumVarSelected-1], m_noOfPoints);
			m_PointType[m_NumVarSelected-1]=FALSE;
		}
		else if(m_NumVarSelected>1&&m_NumVarSelected<11) {
			for(int i=0;i<m_noOfPoints;i++) {
				if(m_MultiRunPlot) {
					if(itype==6)
						AA[m_NumVarSelected-1][i]=double(m_pDoc->m_MStorage.GetValid(i,ilocal));
					else if(itype==7)
						AA[m_NumVarSelected-1][i]=double(m_pDoc->m_MStorage.GetMultiP(i,ilocal));
					else if(itype==8)
						AA[m_NumVarSelected-1][i]=double(m_pDoc->m_MStorage.GetSumV(i,ilocal));

				}
				else if(pPG->GetVarValue(PGindex,i+1)<-1.E38)
						AA[m_NumVarSelected-1][i]=Chart::NoValue;
				else {
					AA[m_NumVarSelected-1][i]=double(pPG->GetVarValue(PGindex, i+1));
					if(m_Cumulated&&i>0) {
						Acc=Acc+AA[m_NumVarSelected-1][i];
						AA[m_NumVarSelected-1][i]=Acc;
					}
				}
			}
			m_dataSeriesA[m_NumVarSelected-1] = DoubleArray(AA[m_NumVarSelected-1], m_noOfPoints);
			m_PointType[m_NumVarSelected-1]=FALSE;
	
		}

		}
	}
	float validv;
	if(m_AddOnlyValidRuns&&m_MultiRunPlot) {
		for(int ii=0;ii<m_NumVarSelected;ii++) {
			for(int i=0;i<m_noOfPoints;i++) {
				validv=m_pDoc->m_MStorage.GetMultiP(i,m_pDoc->MR_Get_NumBayesPar());
				if(validv>0.1&&i>75)
					AA[m_NumVarSelected+ii][i]=AA[ii][i];
				else
					AA[m_NumVarSelected+ii][i]=Chart::NoValue;
			}
			m_dataSeriesA[m_NumVarSelected+ii] = DoubleArray(AA[m_NumVarSelected+ii], m_noOfPoints);
			m_PointType[ii]=TRUE;
		}
	}
    // The initial view port is to show the entire range of data.
    m_currentDuration = lastDate - firstDate;
	m_minValue=m_maxValue=0.;
	m_minXValue=m_maxXValue=0.;

    
    //
    // Get the data and stores them in a memory buffer for fast scrolling / zooming. In 
    // this demo, we just use a random number generator. In practice, you may get the data
    // from a database or XML or by other means.
    //	



}

//
// Handle scroll bar events
//
double CZoomscrolldemoDlg::moveScrollBar(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    //
    // Get current scroll bar position
    //
    SCROLLINFO info;
    info.cbSize = sizeof(SCROLLINFO);
    info.fMask = SIF_ALL;
    pScrollBar->GetScrollInfo(&info);

    //
    // Compute new position based on the type of scroll bar events
    //
    int newPos = info.nPos;
    switch (nSBCode)
    {
    case SB_LEFT:
        newPos = info.nMin;
        break;
    case SB_RIGHT:
        newPos = info.nMax;
        break;
    case SB_LINELEFT:
        newPos -= (info.nPage > 10) ? info.nPage / 10 : 1;
        break;
    case SB_LINERIGHT:
        newPos += (info.nPage > 10) ? info.nPage / 10 : 1;
        break;
    case SB_PAGELEFT:
        newPos -= info.nPage;
        break;
    case SB_PAGERIGHT:
        newPos += info.nPage;
        break;
    case SB_THUMBTRACK:
        newPos = info.nTrackPos;
        break;
    }
    if (newPos < info.nMin) newPos = info.nMin;
    if (newPos > info.nMax) newPos = info.nMax;
    
    // Update the scroll bar with the new position
    pScrollBar->SetScrollPos(newPos);

    // Returns the position of the scroll bar as a ratio of its total length
    return ((double)(newPos - info.nMin)) / (info.nMax - info.nMin);
}

//
// Validate the contents of the duration combo box and update ViewPortWidth accordingly
//
void CZoomscrolldemoDlg::validateDuration(const CString &text)
{
    // Parse the duration text
    double newDuration = _tcstod(text, 0) * 86400;

    // Duration too short or not numeric?
    if (newDuration < m_minDuration) 
        newDuration = m_minDuration;

    if (newDuration != m_currentDuration)
    {
        // Set the ViewPortWidth according to the new duration
        m_currentDuration = newDuration;
        m_ChartViewer.setViewPortWidth(newDuration / m_dateRange);
        
        // Update the chart
        m_ChartViewer.updateViewPort(true, true);
    }
}

//
// Draw the chart and display it in the given viewer
//
void CZoomscrolldemoDlg::drawChart(CChartViewer *viewer)
{
    //
    // In this demo, we copy the visible part of the data to a separate buffer for chart
    // plotting. 
    //
    // Note that if you only have a small amount of data (a few hundred data points), it
    // may be easier to just plot all data in any case (so the following copying code is 
    // not needed), and let ChartDirector "clip" the chart to the plot area. 
    //
	int noOfPoints, startIndex, endIndex;
	double viewPortStartDate, viewPortEndDate, m_interv;

	CWinApp* pApp = AfxGetApp();

    



	if(m_ChartType>0) {
		// Using ViewPortLeft and ViewPortWidth, get the start and end dates of the view port.
		viewPortStartDate = m_minDate + (__int64)(viewer->getViewPortLeft() * 
			m_dateRange + 0.5);
		viewPortEndDate = viewPortStartDate + (__int64)(viewer->getViewPortWidth() * 
			m_dateRange + 0.5);
	        
		// Get the starting index of the array using the start date
		startIndex = std::lower_bound(m_timeStamps.data, m_timeStamps.data + m_timeStamps.len,
			viewPortStartDate) - m_timeStamps.data;
		if ((startIndex > 0) && (m_timeStamps[startIndex] != viewPortStartDate)) 
			--startIndex;
	    
		// Get the ending index of the array using the end date
		endIndex = std::upper_bound(m_timeStamps.data, m_timeStamps.data + m_timeStamps.len, 
			viewPortEndDate) - m_timeStamps.data;

		// Get the length
		noOfPoints = endIndex - startIndex;
	}
	else {
		noOfPoints=m_noOfPoints;
		startIndex=0;
		endIndex=m_noOfPoints-1;
	}

    // We copy the visible data from the main arrays to separate data arrays
    double* viewPortTimeStamps = new double[noOfPoints];
	double* viewPortDataSeries[32];
	int arraySizeInBytes = noOfPoints * sizeof(double);
	memcpy(viewPortTimeStamps, &TT[startIndex], arraySizeInBytes);

	CString cstr;	

	if(m_AddOnlyValidRuns&&m_MultiRunPlot) 
		m_NumVarSelected=m_NumVarSelected*2;
	for(int i=0;i<m_NumVarSelected;i++) {
		cstr="HColorLine"+ItoAscii(i+1);
		viewPortDataSeries[i] = new double[noOfPoints];
		memcpy(viewPortDataSeries[i], m_dataSeriesA[i].data + startIndex, arraySizeInBytes);
		m_ColorLine[i]=pApp->GetProfileInt("CoupModel - KTH", cstr, m_ColorLine[i]);	
	}

    if (noOfPoints >= 520&&m_ChartType>0)
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
        ArrayMath m(DoubleArray(viewPortTimeStamps, noOfPoints));
        m.selectRegularSpacing(noOfPoints / 260);
        
        // For the timestamps, take the first timestamp on each slot
        int aggregatedNoOfPoints = m.aggregate(DoubleArray(viewPortTimeStamps, noOfPoints), 
            Chart::AggregateFirst).len;
		for(int i=0;i<m_NumVarSelected;i++) {
        // For the data values, aggregate by taking the averages
			m.aggregate(DoubleArray(viewPortDataSeries[i], noOfPoints), Chart::AggregateAvg);
		}
 

        noOfPoints = aggregatedNoOfPoints-1;
    }

    //
    // Now we have obtained the data, we can plot the chart. 
    //

    ///////////////////////////////////////////////////////////////////////////////////////
    // Step 1 - Configure overall chart appearance. 
    ///////////////////////////////////////////////////////////////////////////////////////

    // Create an XYChart object 600 x 300 pixels in size, with pale blue (0xf0f0ff) 
    // background, black (000000) border, 1 pixel raised effect, and with a rounded frame.

	XYChart *c;
	if(m_ChartType==0)  {
		c = new XYChart(540, 400, 0xf0f0ff, 0, 1);
		int blue=0xffffff;
		c->setPlotArea(50, 50, 300, 300, blue, -1, -1, 0xcccccc, 0xcccccc);
		c->addLegend(362, 60, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent, 
        Chart::Transparent);

	  }
	else {
		c = new XYChart(700, 400, 0xf0f0ff, 0, 1);
		c->setPlotArea(102, 60, 520, 292, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
		c->addLegend(102, 5, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent, Chart::Transparent);
		    // Add a bottom title to the chart to show the date range of the axis, with a light blue 
    // (ccccff) background.
		if(!m_MultiRunPlot) {
			char formattedStartDate[32];
			char formattedEndDate[32];
			strcpy(formattedStartDate, c->formatValue(viewPortStartDate, "{value|mmm dd, yyyy}"));
			strcpy(formattedEndDate, c->formatValue(viewPortEndDate, "{value|mmm dd, yyyy}"));
			char buffer[2048];
			sprintf(buffer, "From <*font=arialbi.ttf*>%s<*/font*> to <*font=arialbi.ttf*>%s<*/font*>"
				" (Duration <*font=arialbi.ttf*>%d<*/font*> days)", formattedStartDate, 
				formattedEndDate, (int)((viewPortEndDate - viewPortStartDate) / 86400.0 + 0.5));
			//c->addTitle(Chart::Bottom, buffer, "ariali.ttf", 10)->setBackground(0xccccff);
			c->addLegend(102, 5, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent, 
			Chart::Transparent);
		}

	}
    c->setRoundedFrame(m_extBgColor);
    
    // Set the plotarea at (52, 60) and of size 520 x 192 pixels. Use white (ffffff) 
    // background. Enable both horizontal and vertical grids by setting their colors to 
    // grey (cccccc). Set clipping mode to clip the data lines to the plot area.
 
    c->setClipping();

    // Add a top title to the chart using 15 pts Times New Roman Bold Italic font, with a 
    // light blue (ccccff) background, black (000000) border, and a glass like raised effect.
  //  c->addTitle("Zooming and Scrolling Demonstration", "timesbi.ttf", 15
   //     )->setBackground(0xccccff, 0x0, Chart::glassEffect());


    // Set axes width to 2 pixels
    c->yAxis()->setWidth(2);
    c->xAxis()->setWidth(2);

    // Add a title to the y-axis


    ///////////////////////////////////////////////////////////////////////////////////////
    // Step 2 - Add data to chart
    ///////////////////////////////////////////////////////////////////////////////////////

    // 
    // In this example, we represent the data by lines. You may modify the code below if 
    // you want to use other representations (areas, scatter plot, etc).
    //
	Layer *layer;
	LineLayer *layer2;
	if(m_ChartType==0) {
		layer=c->addLineLayer();

	}
		else {
    // Add a line layer for the lines, using a line width of 2 pixels
			layer = c->addLineLayer();
			layer->setLineWidth(2);
		}

    
		
		// Now we add the 3 data series to a line layer, using the color red (ff0000), green
    // (00cc00) and blue (0000ff)



	CPG *pPG;
	int PGindex;
	int  ilocal, itype, ichart, ivar, iv, ival, nselV, ind, il;
	CBase *pX;
	CBase* pPar;
	CString ytitle1, ytitle2, test, unit1, unit2, IndepId, unit;

	int TotNumPrior;
	TotNumPrior=m_NumVarSelected/2;

	if(m_pCP==NULL) {
		m_NumVarSelected=0;
		CString st=_T("");
		for (int j=0;j<m_pDoc->PlotV_NumSelected();j++) {
				if(!m_MultiRunPlot){
					pPG=m_pDoc->CPG_GetAt(m_numPGPtr);
					m_noOfPoints=pPG->GetNumRecords();
				}
				pX=m_pDoc->PlotV_GetPtr(j);
				ilocal=m_pDoc->PlotV_GetLocalIndex(j);
				itype=m_pDoc->PlotV_GetType(j);
				ichart=m_pDoc->PlotV_GetChart(j);
			
				if(ichart==m_ChartNumber) {
					m_NumVarSelected++;
					switch(itype) {
					case 0:
						 PGindex=ilocal;
						 st=pPG->GetVarId(ilocal);
						 test=pPG->GetVarName(ilocal);
						break;
					case 1:
					case 2:
						PGindex=((CXT*)pX)->GetPgFileIndex(ilocal);
						 st=pPG->GetVarId(PGindex);
						 test=pPG->GetVarName(PGindex);
						break;
					case 3:
					case 4:
						PGindex=((CGD*)pX)->GetPgFileIndex(ilocal);
						 st=pPG->GetVarId(PGindex);
						 test=pPG->GetVarName(PGindex);
					break;
					case 5:
						PGindex=m_pDoc->GetNumOutPuts()+ilocal;
						st=pPG->GetVarId(PGindex);
						test=pPG->GetVarName(PGindex);
						break;
				

					case 6:
						nselV=m_pDoc->ValidationFilePointer()->GetNumberOfSelectedVal();
						ival=ilocal%nselV;
						st=ItoAscii(ival+1)+":";

						test=m_pDoc->GetValName(ival);
						iv=m_pDoc->GetValLocalIndex(ival);
						if(iv>-1) test+="("+ItoAscii(iv+1)+")";
						st+=test;


						m_ind=ilocal/nselV;
						switch (m_ind) {
							case 0:
								test="Coefficient of determination";
								break;
							case 1:
								test="Intercept";
								break;
							case 2:
								test="Slope";
								break;
							case 3:
								test="Mean Error (ME)";
								break;
							case 4:
								test="Root Mean Square Error (RMSE)";
								break;
							case 5:
								test="Log Likelihood";
								break;
						}
						break;
					case 7:
						ind=0;
						int nTabIndex;

						unit=_T("");
						for(int i=0;i<m_pDoc->MR_GetNumberOfDimensions();i++){
								for(int ii=0; ii<m_pDoc->MR_Get_NumberParDim(i+1);ii++){
									pPar=m_pDoc->MR_Get_pPar(i+1,ii+1);
									nTabIndex=m_pDoc->MR_GetTabIndex(i+1,ii+1);
									CString namep=pPar->GetName();
									if(nTabIndex>0)				
										namep+="("+ItoAscii(nTabIndex)+")";
									if(ind==ilocal) {
										st=namep;
										test="Parameter values";
										unit=pPar->GetUnit();
										break;

									}
									ind++;
								}
						}
						break;
					case 8:
						ind=0;
						for(int i=0; i<m_pDoc->m_XTGD_Array.GetSize();i++) {
							pPar=(CBase*)m_pDoc->m_XTGD_Array.GetAt(i);
							test=pPar->GetName();
                            if(pPar->IsKindOf(RUNTIME_CLASS(CGD))){
								for(int ii=0;ii<((CGD*)pPar)->GetNumberOfSelectedFlags();ii++) {
									for(int j=0;j<6;j++) {
										if(ind==ilocal) {
											il=((CGD*)pPar)->GetLocalIndex(ii);
											ii=((CGD*)pPar)->GetNumberOfSelectedFlags();
											unit=((CGD*)pPar)->GetUnit();
											i=m_pDoc->m_XTGD_Array.GetSize();
											break;
										}
										ind++;
									}
								}
							}
							else {
								for(int ii=0;ii<((CXT*)pPar)->GetNumberOfSelectedFlags();ii++) {
									for (int j=0;j<6;j++) {
										if(ind==ilocal) {
											il=((CXT*)pPar)->GetLocalIndex(ii);
											ii=((CGD*)pPar)->GetNumberOfSelectedFlags();
											unit=((CGD*)pPar)->GetUnit();
											i=m_pDoc->m_XTGD_Array.GetSize();
											break;
										}
										ind=ind+1;
									}
								}

							}					
						}
						st=_T("");
						if(ind>-1) st+="("+ItoAscii(il+1)+")";
						switch (ilocal%6) {
							case 0:
								st+="Initial";
								break;
							case 1:
								st+="Final";
								break;
							case 2:
								st+="Min";
								break;
							case 3:
								st+="Max";
								break;
							case 4:
								st+="Mean";
								break;
							case 5:
								st+="Accumulated";
								break;
						}

						break;
				}
		
				ivar=m_NumVarSelected-1;	
				if(m_NumVarSelected==1) {
	//PGindex=((CGD*)Sw)->GetPgFileIndex(Index-1);

				if(m_ChartType>0) {
				if(m_PointType[ivar]) {
						c->addScatterLayer(DoubleArray(viewPortTimeStamps,noOfPoints),DoubleArray(viewPortDataSeries[ivar+TotNumPrior],noOfPoints),
						"Accepted",Chart::DiamondSymbol,9,m_ColorLine[ivar]);

				}
				
				layer->setXData(DoubleArray(viewPortTimeStamps, noOfPoints));
				layer->addDataSet(DoubleArray(viewPortDataSeries[ivar], noOfPoints), m_ColorLine[0], st);
		
				if(!m_MultiRunPlot) {
					ytitle1=test;	
					unit1=pPG->GetVarUnit(PGindex);
				}
				else {
					
					ytitle1=test;	
					unit1=unit;
				}
				if(m_RegLine)
					c->addTrendLayer(DoubleArray(viewPortTimeStamps,noOfPoints),DoubleArray(viewPortDataSeries[ivar],noOfPoints));	
			
			}
			else {
				if(m_pCP!=NULL) {
					ytitle1=" ";


				}
				else if(m_MultiRunPlot) {
					ytitle1=" ";
					IndepId=test+st;
				}
				else {
					ytitle1=pPG->GetVarName(PGindex);
					unit1=pPG->GetVarUnit(PGindex);
					IndepId=pPG->GetVarId(PGindex);
				}
			}
		}
			else if(m_NumVarSelected>1) {
	
			if(m_ChartType==0) {
					c->addScatterLayer(DoubleArray(viewPortDataSeries[0],noOfPoints),DoubleArray(viewPortDataSeries[ivar],noOfPoints),
						st,Chart::DiamondSymbol,9,m_ColorLine[ivar]);
					if(m_AddLine) {
						layer->addDataSet(DoubleArray(viewPortDataSeries[ivar],noOfPoints),
							m_ColorLine[1],st);
						layer->setXData(DoubleArray(viewPortDataSeries[ivar],noOfPoints));
					}
					if(m_RegLine)
						c->addTrendLayer(DoubleArray(viewPortDataSeries[0],noOfPoints),DoubleArray(viewPortDataSeries[ivar],noOfPoints),
							m_ColorLine[1])->setLineWidth(3);	
					ytitle2=test;
					if(!m_MultiRunPlot) unit2=unit;
				}
			else if(test==ytitle1||unit1==unit||m_MultiRunPlot) {
				if(m_PointType[ivar]) {
						c->addScatterLayer(DoubleArray(viewPortTimeStamps,noOfPoints),DoubleArray(viewPortDataSeries[ivar+TotNumPrior],noOfPoints),
						"Accepted",Chart::DiamondSymbol,9,m_ColorLine[ivar]);

				}
				
				layer->addDataSet(DoubleArray(viewPortDataSeries[ivar], noOfPoints), m_ColorLine[ivar], st);

			}
			else if((test!=ytitle1&&test!=ytitle2)||(unit!=unit1&&unit!=unit2)) {
				layer2 = c->addLineLayer(DoubleArray(viewPortDataSeries[ivar],noOfPoints),m_ColorLine[ivar],st);
				layer2->setXData(DoubleArray(viewPortTimeStamps, noOfPoints));
				if(m_PointType[ivar]) {
						c->addScatterLayer(DoubleArray(viewPortTimeStamps,noOfPoints),DoubleArray(viewPortDataSeries[ivar+TotNumPrior],noOfPoints),
						"Accepted",Chart::DiamondSymbol,9,m_ColorLine[ivar]);
				}
				layer2->setLineWidth(2);
				layer2->setUseYAxis2();
				ytitle2=test;
				if(!m_MultiRunPlot) 
					unit2=pPG->GetVarUnit(PGindex);
				else
					unit2=unit;
			}
			else if(test==ytitle2||(unit2==unit&&m_MultiRunPlot)) {
				if(m_PointType[ivar]) {
						c->addScatterLayer(DoubleArray(viewPortTimeStamps,noOfPoints),DoubleArray(viewPortDataSeries[ivar+TotNumPrior],noOfPoints),
						"Accepted",Chart::DiamondSymbol,9,m_ColorLine[ivar]);

				}
				layer2->addDataSet(DoubleArray(viewPortDataSeries[ivar], noOfPoints), m_ColorLine[ivar], st);
			}
			else {
				if(m_PointType[ivar]) {
						c->addScatterLayer(DoubleArray(viewPortTimeStamps,noOfPoints),DoubleArray(viewPortDataSeries[ivar+TotNumPrior],noOfPoints),
						"Accepted",Chart::DiamondSymbol,9,m_ColorLine[ivar]);

				}
				layer->addDataSet(DoubleArray(viewPortDataSeries[ivar], noOfPoints), m_ColorLine[ivar], st);
			}
			}
			if(m_RegLine&&m_ChartType>0 )
					c->addTrendLayer(DoubleArray(viewPortTimeStamps,noOfPoints),DoubleArray(viewPortDataSeries[ivar],noOfPoints));	

		}

					



		
			

	}
	}
	else if(m_pCP!=NULL) {

//CString newtitle="Independent Variable :"+IndepId;
					c->addTitle(Chart::TopCenter, m_Title);
					layer->setXData(DoubleArray(viewPortDataSeries[0], noOfPoints));
					for (int i=1;i<m_NumVarSelected;i++) {
						layer->addDataSet(DoubleArray(viewPortDataSeries[i],noOfPoints),
						m_ColorLine[i-1],m_Legends.GetAt(i-1));
					}
					for (int i=0;i<m_ExtraCurveCounter+1;i++) {
						if(m_PFMeasuredPoints) {
							c->addScatterLayer(m_dataSeriesB[i],m_dataSeriesC[i],
								"Measured",Chart::DiamondSymbol,9,m_ColorLine[i]);
						}
					}
		
			
			c->xAxis()->setTitle(m_LabelX, "arialbd.ttf", 9);
			c->yAxis()->setTitle(m_LabelY, "arialbd.ttf", 9);
		}

	
	if(m_pCP==NULL) {
				ytitle1.TrimRight();
				ytitle1+=" ("+unit1.TrimRight()+")";
				if(m_ChartType==0) {
					c->xAxis()->setTitle(ytitle1, "arialbd.ttf", 9);
					ytitle2.TrimRight();
					ytitle2+=" ("+unit2.TrimRight()+")";
					c->yAxis()->setTitle(ytitle2, "arialbd.ttf", 9);
					if(m_CommonScale) 
						c->yAxis()->syncAxis(c->xAxis());
						CString newtitle="Independent Variable :"+IndepId;
					c->addTitle(Chart::TopCenter,newtitle);
				}
				else {
					c->yAxis()->setTitle(ytitle1, "arialbd.ttf", 9);
					if(ytitle2!="") {
						ytitle2.TrimRight();
						ytitle2+=" ("+unit2.TrimRight()+")";
						c->yAxis2()->setTitle(ytitle2,"arialbd.ttf", 9)->setAlignment(Chart::Right);
				if(m_CommonScale) 
					c->yAxis2()->syncAxis(c->yAxis());
			}
		}





		}





	
    ///////////////////////////////////////////////////////////////////////////////////////
    // Step 3 - Set up x-axis scale
    ///////////////////////////////////////////////////////////////////////////////////////
	if(m_MultiRunPlot&&m_ChartType==1) {
			c->xAxis()->setLinearScale(viewPortStartDate,viewPortEndDate);
			c->layout();
			m_minValue = c->yAxis()->getMinValue();
			m_maxValue = c->yAxis()->getMaxValue();
	}
	else if(m_ChartType>0) {
    // Set x-axis date scale to the view port date range. 
    c->xAxis()->setDateScale(viewPortStartDate, viewPortEndDate);

    //
    // In the current demo, the x-axis range can be from a few years to a few days. We can 
    // let ChartDirector auto-determine the date/time format. However, for more beautiful 
    // formatting, we set up several label formats to be applied at different conditions. 
    //

    // If all ticks are yearly aligned, then we use "yyyy" as the label format.
    c->xAxis()->setFormatCondition("align", 360 * 86400);
    c->xAxis()->setLabelFormat("{value|yyyy}");
    
    // If all ticks are monthly aligned, then we use "mmm yyyy" in bold font as the first 
    // label of a year, and "mmm" for other labels.
    c->xAxis()->setFormatCondition("align", 30 * 86400);
    c->xAxis()->setMultiFormat(Chart::StartOfYearFilter(), "<*font=bold*>{value|mmm yyyy}", 
        Chart::AllPassFilter(), "{value|mmm}");
    
    // If all ticks are daily algined, then we use "mmm dd<*br*>yyyy" in bold font as the 
    // first label of a year, and "mmm dd" in bold font as the first label of a month, and
    // "dd" for other labels.
    c->xAxis()->setFormatCondition("align", 86400);
    c->xAxis()->setMultiFormat(Chart::StartOfYearFilter(), 
        "<*block,halign=left*><*font=bold*>{value|mmm dd<*br*>yyyy}", 
        Chart::StartOfMonthFilter(), "<*font=bold*>{value|mmm dd}");
    c->xAxis()->setMultiFormat(Chart::AllPassFilter(), "{value|dd}");

    // For all other cases (sub-daily ticks), use "hh:nn<*br*>mmm dd" for the first label of
    // a day, and "hh:nn" for other labels.
    c->xAxis()->setFormatCondition("else");
    c->xAxis()->setMultiFormat(Chart::StartOfDayFilter(), 
        "<*font=bold*>{value|hh:nn<*br*>mmm dd}", Chart::AllPassFilter(), "{value|hh:nn}");
	}
	else {
		//c->xAxis()->setLinearScale()
		if((m_minXValue == 0) && (m_maxXValue == 0)) {
			c->layout();
			m_minXValue = c->xAxis()->getMinValue();
			m_maxXValue = c->xAxis()->getMaxValue();
			m_minValue = c->yAxis()->getMinValue();
			m_maxValue = c->yAxis()->getMaxValue();
			m_interv=0;

		}
		if(m_CommonScale) {
				double min1,max1;
				min1=min(m_minXValue,m_minValue,);
				max1=max(m_maxXValue,m_maxValue);
				m_maxValue=m_maxXValue=max1;
				m_minValue=m_minXValue=min1;
				m_interv=(max1-min1)/5.;
		}
		else
			m_interv=0;
		if(m_logscale) {
			c->xAxis()->setLogScale(m_minXValue, m_maxXValue);
				//c->XAxis()->setLogScale(, axisUpperLimit);
		}
		else if (m_interv>0) 
			c->xAxis()->setLinearScale(m_minXValue, m_maxXValue,m_interv);
		else
			c->xAxis()->setLinearScale(m_minXValue, m_maxXValue);
		


	}
    
    ///////////////////////////////////////////////////////////////////////////////////////
    // Step 4 - Set up y-axis scale
    ///////////////////////////////////////////////////////////////////////////////////////

	double axisLowerLimit, axisUpperLimit;



	if(m_ChartType==0) {
		if(((m_minValue == 0) && (m_maxValue == 0))||m_pCP!=NULL) {
			c->layout();
			m_minValue = c->yAxis()->getMinValue();
			m_maxValue = c->yAxis()->getMaxValue();
		}
		if(m_pCP==NULL) m_interv=0;
	}
	else if((m_minValue == 0) && (m_maxValue == 0)) {
			c->layout();
			m_minValue = c->yAxis()->getMinValue();
			m_maxValue = c->yAxis()->getMaxValue();
			m_interv=0;
	}

    else if (viewer->getZoomDirection() == Chart::DirectionHorizontal) 
    {
        // y-axis is auto-scaled - save the chosen y-axis scaled to support xy-zoom mode


		axisLowerLimit=m_minValue;
		axisUpperLimit=m_maxValue;
    }
    else
    {
        // xy-zoom mode - compute the actual axis scale in the view port 
        axisLowerLimit =  m_maxValue - (m_maxValue - m_minValue) * 
            (viewer->getViewPortTop() + viewer->getViewPortHeight());
        axisUpperLimit =  m_maxValue - (m_maxValue - m_minValue) * 
            viewer->getViewPortTop();
	}


	if(m_ChartType==0) {
			if(m_logscale) {
				c->yAxis()->setLogScale(m_minValue, m_maxValue);
				//c->XAxis()->setLogScale(, axisUpperLimit);
			}
			else if(m_interv>0) 
				c->yAxis()->setLinearScale(m_minValue, m_maxValue,m_interv);
			else
				c->yAxis()->setLinearScale(m_minValue, m_maxValue);


	}
	else if(m_logscale&&m_ZoomX==FALSE) {
        // *** use the following formula if you are using a log scale axis ***
			axisLowerLimit = m_maxValue * pow(m_minValue / m_maxValue, 
				viewer->getViewPortTop() + viewer->getViewPortHeight());
			axisUpperLimit = m_maxValue * pow(m_minValue / m_maxValue, 
				viewer->getViewPortTop());
			if(axisLowerLimit<=0) axisLowerLimit=1;
			c->yAxis()->setLogScale(axisLowerLimit, axisUpperLimit);
	}
	else if(m_ZoomX==FALSE)	{
        // xy-zoom mode - compute the actual axis scale in the view port 
			axisLowerLimit =  m_maxValue - (m_maxValue - m_minValue) * 
            (viewer->getViewPortTop() + viewer->getViewPortHeight());
			 axisUpperLimit =  m_maxValue - (m_maxValue - m_minValue) * 
            viewer->getViewPortTop();
        // use the zoomed-in scale
			 c->yAxis()->setLinearScale(axisLowerLimit, axisUpperLimit);
	}
    c->yAxis()->setRounding(false, false);
    

    ///////////////////////////////////////////////////////////////////////////////////////
    // Step 5 - Display the chart
    ///////////////////////////////////////////////////////////////////////////////////////

    // Set the chart image to the WinChartViewer
    delete m_ChartViewer.getChart();
    m_ChartViewer.setChart(c);

    // Free resources
    delete[] viewPortTimeStamps;
	for(int i=0; i<m_NumVarSelected;i++) {
		delete[] viewPortDataSeries[i];
	}


}

//
// Update the image map
//
void CZoomscrolldemoDlg::updateImageMap(CChartViewer *viewer)
{
    if (0 == viewer->getImageMapHandler())
    {
		if(m_MultiRunPlot) 
			viewer->setImageMap(viewer->getChart()->getHTMLImageMap("clickable", "",
			"title='[{dataSetName}] {x}: {value|2}'"));
		else
        // no existing image map - creates a new one
        viewer->setImageMap(viewer->getChart()->getHTMLImageMap("clickable", "",
			"title='[{dataSetName}] {x|mmm dd, yyyy,hh:mm}: {value|2}'"));
    }
}

/////////////////////////////////////////////////////////////////////////////
// General utilities

//
// Get the default background color
//
int CZoomscrolldemoDlg::getDefaultBgColor()
{
    LOGBRUSH LogBrush; 
    HBRUSH hBrush = (HBRUSH)SendMessage(WM_CTLCOLORDLG, (WPARAM)CClientDC(this).m_hDC, 
        (LPARAM)m_hWnd); 
    ::GetObject(hBrush, sizeof(LOGBRUSH), &LogBrush); 
    int ret = LogBrush.lbColor;
    return ((ret & 0xff) << 16) | (ret & 0xff00) | ((ret & 0xff0000) >> 16);
}

//
// Load an icon resource into a button
//
void CZoomscrolldemoDlg::loadButtonIcon(int buttonId, int iconId, int width, int height)
{
    GetDlgItem(buttonId)->SendMessage(BM_SETIMAGE, IMAGE_ICON, (LPARAM)::LoadImage(
        AfxGetResourceHandle(), MAKEINTRESOURCE(iconId), IMAGE_ICON, width, height, 
        LR_DEFAULTCOLOR));  
}

void CZoomscrolldemoDlg::OnBnClickedSaveFile()
{
	// TODO: Add your control notification handler code here


		CString dir=m_pDoc->GetCurrentDirectory();
		int num=m_pDoc->GetNumberSavedCharts();
		num++;
		CString filename, str;
		str=ItoAscii(m_pDoc->m_SimulationRunNo);
		if(str.GetLength()>3) 
			filename=dir+"COUP"+str+"_"+ItoAscii(num)+".png";
		else if(str.GetLength()==2)
			filename=dir+"COUP_0"+str+"_"+ItoAscii(num)+".png";
		else if(str.GetLength()==1)
			filename=dir+"COUP_00"+str+"_"+ItoAscii(num)+".png";
		else
			filename=dir+"COUP_"+str+"_"+ItoAscii(num)+".png";

		BaseChart *c;
	    c=m_ChartViewer.getChart();
		c->makeChart(filename);
		m_pDoc->SetNumberSavedCharts(num);
		//delete c;
}

void CZoomscrolldemoDlg::OnBnClickedCommonscale()
{
	UpdateData(TRUE);
	m_ChartViewer.updateViewPort(true, true);
	BaseChart *c;
	c=m_ChartViewer.getChart();
	drawChart(&m_ChartViewer);
	//OnViewPortChanged();
	// TODO: Add your control notification handler code here
}
void CZoomscrolldemoDlg::OnBnClickedCumulate()
{
	UpdateData(TRUE);
	loadData();
	m_ChartViewer.updateViewPort(true, true);
	drawChart(&m_ChartViewer);

	//OnViewPortChanged();
	// TODO: Add your control notification handler code here
}

void CZoomscrolldemoDlg::OnActivate(UINT nstate, CWnd *pWndOther, BOOL bMinimized)
{
    switch (nstate)
    {
    case WA_ACTIVE:
        // Is data acquisition timer - get a new data sample
     //   getData();
		loadData();
        break;
    case WA_CLICKACTIVE:
        // Is chart update timer - request chart update
       loadData();
        break;
    }
    
    CDialog::OnActivate(nstate, pWndOther, bMinimized);
}