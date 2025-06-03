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
	m_RealTime.SetChartCUGPointer(this);
	m_ParameterPlot.SetChartCUGPointer(this);
	MySingle::instance().SetChartCugPointer(this);

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
	m_pMainframe=AfxGetMainWnd();
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
void ChartCug::OnSetup()
{
	CUGCell cell; 
	CString str;
	int cols = m_col,rows = m_row;

	if(m_pSimDoc==nullptr) return;

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

		

	if (m_pUtil->m_RealTimeMode) {
		if (m_pSimDoc->IsMultiSimulation())
			SetNumberSheets(5);
		else
			SetNumberSheets(2);

	}
	else if (m_pUtil->m_ParameterPlotMode) {
		if (m_ParameterPlot.m_TableIndex > 0)
			SetNumberSheets(4);
		else
			SetNumberSheets(3);



	}
	else if (m_pUtil->m_TablePlotMode)
		return;
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
	if(m_pSimDoc==nullptr &&sheetNumber==0) {
			m_Mode=0;
			OnUpdate();	

		RedrawAll();
		return;

	}
	else if(m_pSimDoc==nullptr) {
		m_Mode=10;
		OnUpdate();	

		RedrawAll();
		return;

	}
	m_pUtil->m_FileSave.m_NoChange=false;
	switch(sheetNumber) {
			case(0): //Remade
				m_Mode=0;
				SetLocalWindowText("Chart - Actions", WINDOW_PROP);
				OnUpdate();			
				break;
			case(1): //General
				m_Mode=1;
				SetLocalWindowText("Chart - General Layout", WINDOW_PROP);
				OnUpdate();			
				break;
			case (2): //Axis
				m_Mode=2;
				if (m_pUtil->m_ParameterPlotMode)
					SetLocalWindowText("Chart - Details", WINDOW_PROP);
				else if(m_pUtil->m_TablePlotMode)
					SetLocalWindowText("Chart - Layer to View", WINDOW_PROP);
				else
					SetLocalWindowText("Chart - General Axes", WINDOW_PROP);
				OnUpdate();
				break;
			case(3): // Variables
				m_Mode=3;
				if (m_pUtil->m_ParameterPlotMode&&m_ParameterPlot.m_TableIndex >= 0)
					SetLocalWindowText("Chart - Layers to View", WINDOW_PROP);
				else
					SetLocalWindowText("Chart - Variables", WINDOW_PROP);
				OnUpdate();
				break;
			case(4): //Distributions
				m_Mode=4;
				SetLocalWindowText("Chart - Distributions", WINDOW_PROP);
				OnUpdate();
				break;
			case(5): // Regression
				SetLocalWindowText("Chart - Regressions", WINDOW_PROP);
				m_Mode=5;
				OnUpdate();
				break;
			case(6): // Data
				m_Mode=6;
				SetLocalWindowText("Chart - Data", WINDOW_PROP);
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
				SetLocalWindowText("Chart - Actions", WINDOW_PROP);
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
	//int ArrayIndex;
	bool RightLabels;
	
	if(!updn)
		return;
	RightLabels=false;
	if(m_pProp->IsRightAxisExist())
		RightLabels=true;
	m_CurrentTopRow=GetTopRow();
	if(m_Mode==0) {
		if(m_pUtil->m_RealTimeMode) 
			ActionDuringRealTimeMode(col, row);
		else if(m_pUtil->m_ParameterPlotMode) {
				ActionDuringParameterPlotMode(col, row);
				return;


			}
		else if(m_pUtil->m_TablePlotMode) 
			ActionDuringTablePlotMode(col, row);
		else {// Normal Time serie chart mode
			ActionDuringTimeSerieMode(col, row);
		}
	}
	if(m_Mode==2&&m_pUtil->m_RealTimeMode) ParameterChangeRealTimeMode(col,row);
	else if(m_Mode==2&&m_pUtil->m_ParameterPlotMode) ChartDetailsParameterPlotMode(col, row);
	else if(m_Mode==3&&m_pUtil->m_ParameterPlotMode) LayersParameterPlotMode(col, row);

	if(m_Mode==3&&m_pSimDoc!=nullptr) {
		if(m_pUtil->m_RealTimeMode) 	ValidationRealTimeMode(col,row);
		else if(!m_pUtil->m_ParameterPlotMode) VariablesTimeSerieMode(col, row);

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
	if (updn) return;
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
		bool prevSave, prevNotify;

		prevSave=m_pUtil->m_FileSave.m_SaveFile;
		prevNotify=m_pUtil->m_FileSave.m_NotifySaveFile;
		
		m_pUtil->m_FileSave.m_SaveFile=true;
		m_pUtil->m_FileSave.m_NotifySaveFile=true;
		
		m_RealTime.OnViewPortChanged();
		
		m_pUtil->m_FileSave.m_SaveFile=prevSave;
		m_pUtil->m_FileSave.m_NotifySaveFile=prevNotify;


	}
	else {
		auto sheet=GetSheetNumber();
		if(col==8&&sheet==0) {
			bool prevSave, prevNotify;

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


			m_pProp->ChangeChartTypeForward();
			OnSheetSetup(sheet);
			if(m_pProp->GetChartTypeIndex()== CHART_TYPES::TIME_SERIE)
				m_pUtil->m_FileSave.OnReDraw(false, m_pSimDoc);
			else {
				m_pProp->ResetXScale();
				m_pProp->ResetYLeftScale();
				m_pProp->ResetYRightScale();
				m_pUtil->m_FileSave.OnReDraw(true, m_pSimDoc);
			}

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
	auto PrevTotNum=m_pProp->m_NumChartX*m_pProp->m_NumChartY;

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
			MFC_Util::WriteProfileInt( "NumberChartX",m_pProp->m_NumChartX);
			MFC_Util::WriteProfileInt( "NumberChartY",m_pProp->m_NumChartY);
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

	BOOL updatedirect;
	updatedirect=false;
	if(m_Mode==0&&ID==1&&msg==103) {
		if(m_pUtil->m_RealTimeMode) {
			if(col==1) updatedirect=RealTimeChangeMonOptions(col, row);
		}
		else {
			if(col>=0&&col<=9&&col!=5&&col!=7) updatedirect=ChangeActionOptions(col, row);
		}
	}
	else if(m_Mode==1&&ID==1&&msg==103) {
		updatedirect=ChangeGeneralOptions(col, row);

	}
	else if (m_Mode == 1 && ID == 1 && msg == 100) {
		updatedirect = ChangeGeneralOptions(col, row);

	}
	else if(m_Mode==3&&ID==1&&msg==103) {
		updatedirect=true;
		if(col>0&&col<12) {
			updatedirect=ChangeChartOptions(col, row);
			if (updatedirect) {
				m_pUtil->m_FileSave.OnReDraw(true);
			}
		}
	}
	else if(m_Mode==4&&ID==1&&msg==103) {
		if(col>1&&col<3) {
			ChangeDistibutionOptions(col,row);
		}
	}
	else if(m_Mode==7&&ID==1&&msg==103) {
		if(col==0) {
			ChangeBoxCoxOptions(col,row);
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
int ChartCug::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR std::string,BOOL cancelFlag)
{

	CString str;

	edit->GetWindowText(str);
	bool updatedirect=true;
	bool reload;

	reload=false;
	if(m_Mode==0&&m_pUtil->m_ParameterPlotMode) {
		ChangeParameterValue(col, row, str);
		return true;
		//reload=true;
	}
	else if(m_Mode==1) 		updatedirect=ChangeGeneralChartLayOut(col, row, str);
	else if(m_Mode==2&&m_pUtil->m_ParameterPlotMode)  {
		ChangeParameterValue(col, row, str);
		reload=true;
		updatedirect = false;
	}
	else if(m_Mode==2) {
		updatedirect=ChangeAxis(col, row, str);
		reload=true;
	}
	else if(m_Mode==3)	updatedirect=ChangeVariableLegends(col, row, str);		
	else if(m_Mode==4)	updatedirect=ChangeDistributionValue(col, row, str);	
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
		m_pProp->ResetXDistSet(0);

	}
		
	if(updatedirect) m_pUtil->m_FileSave.OnReDraw(reload);
	else {
		m_pUtil->m_ToBeUpdated=true;
		QuickSetBackColor(-1,-1,m_ColorChanged);
		RedrawCell(-1,-1);
	}
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

	BOOL updatedirect, reload;
	updatedirect=true;
	reload=false;



	CUGCell cell; 
	BOOL changemade;
	changemade=false;
	if(section==UG_CORNERBUTTON||(section==UG_GRID&&item>=5030&&item<=5031)) {
		if(item>=5020&&item<=5021) {
			MakeDataSheet(item-5020);
			m_CreatePG=true;
		}
		else if(item>=5030&&item<7000) {
			MakePGFileFromDataInCurrentChart();
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
		else if(item == 2401 || item==2407||(item>=4000&&item<5000)) {
			changemade=MakeAxisCommand(item);
		}
		else if(item==2402) {

			changemade=MakeVariableCommand(item, col, row);
		}
		else if (item == 2405) {
			if(!m_pProp->m_ReShape.m_CommonScale)
				m_pProp->m_ReShape.m_CommonScale = true;
			else {
				m_pProp->m_ReShape.m_CommonScale = false;
				m_pProp->m_rightaxis = true;
				if (m_pProp->m_ChartType == CHART_TYPES::CONTOUR_CHART) {
					m_TimeSeriePlot.m_Contour.ScaleContourChart = false;
					m_pProp->ResetZScale();
				}
				else
					m_pProp->ResetYLeftScale();
			}

			changemade = true;

		}
		else if (item == 2410)
			changemade;
		else if(item>=1000&&item<1032&&m_pSimDoc!=nullptr) {
		/*	m_pSimDoc->m_PlotStorage.Remove(item-1000);
			index=row;
			if(m_Mode==3&&m_pDoc!=nullptr) {
				m_Legend1.RemoveAt(index);
				m_Legend1Change.RemoveAt(index);
				if(m_pSimDoc->m_PlotStorage.GetNumVar(m_ChartNumIndex)<2)
					m_pProp->m_YLabel2.SetAt(0,"-");
				if(m_pSimDoc->m_PlotStorage.GetNumVar(m_ChartNumIndex)<1)
					m_YLabel1.SetAt(0,"-");
				changemade=true;
			}*/
		}
		else 
			changemade=MakeVariableCommand(item, col, row);

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

	CWinApp* pApp = AfxGetApp();
	m_pUtil->m_TitleSizePoint=MFC_Util::GetProfileIntNo("TitleSize", 12);
	m_pUtil->m_LabelSizePoint=MFC_Util::GetProfileIntNo( "LabelSize", 9);
	m_pUtil->m_LegendSizePoint=MFC_Util::GetProfileIntNo( "LegendSize", 9);
	m_pUtil->m_AxisScalePoint=MFC_Util::GetProfileIntNo( "AxisScaleSize", 9);

	m_pUtil->m_LineWidthPoint=MFC_Util::GetProfileIntNo( "LineWidth", 2);
	m_pUtil->m_LineAxisWidthPoint=MFC_Util::GetProfileIntNo( "LineAxisWidth", 2);
	m_pUtil->m_SymbolSizePoint=MFC_Util::GetProfileIntNo( "SymbolSize", 9);
	m_pUtil->m_MinTickDistX=MFC_Util::GetProfileIntNo( "AxisMinTickX",60); 
	m_pUtil->m_MinTickDistY=MFC_Util::GetProfileIntNo("AxisMinTickY",60); 
	if(m_pSimDoc!=nullptr&&m_Mode<2) {
		int ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, 0);
		CHART_TYPES ChartType=m_pSimDoc->m_PlotStorage.GetChartType(ArrayIndex);
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
			MakeGeneralMenu(col, row);

		}
		else if(m_Mode==2) {
			MakeAxisMenu(col, row);
		}
		else if(m_Mode==3&&m_pSimDoc!=nullptr) {
			MakeVariableMenu(col, row);
		}

		else if(m_Mode==4) {
			 if(col==1)
					AddMenuItem(2004,"Change background color of extra variable in distribution chart");
		}
		else if(m_Mode>4) {
			MakeDataMenu();

		}
	}

	if(m_pUtil->m_ToBeUpdated) {
				AddMenuItem(4099,"Update Chart to apply new setting");

			}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHint
int ChartCug::OnHint(int col,long row,int section,CString *string)
{


	if(col==-1&&row==-1&&m_pUtil->m_ToBeUpdated) {
		string->Format(_T("Single click here to update Chart with new settings"));
		return true;	
	}
	else if (col==-1) {
		if(row==1&&m_pProp->m_NumChartX>=1&&m_pProp->m_NumChartY>1) 
				string->Format(_T("Click on grid to add horizontal subcharts"));
		else if(row==3&&m_pProp->m_NumChartY>=1&&m_pProp->m_NumChartX>1)
				string->Format(_T("Click on grid to add vertical subcharts"));			
		else if(m_pProp->m_NumChartX*m_pProp->m_NumChartY>1)
				string->Format(_T("Click on yellow grid to change layout"));

		if(m_pProp->m_NumChartX*m_pProp->m_NumChartY>1)
			return true;	
		else
			return false;
	}


	if(row==-1&&m_Mode==2) {
		string->Format(_T("Right click to get options for changing chart type\nSingle Click to Change Chart Type"));

	}
	else if(row==0&&m_Mode==1) {
		switch(col) {
			case 0:
			string->Format(_T("Double click to Edit Title"));
			break;
			case 1:
			string->Format(_T("Edit Title Size"));
			break;
			case 2:
			string->Format(_T("Edit Label Size"));
			break;
			case 3:
			string->Format(_T("Edit Legend Size"));
			break;
			case 4:
			string->Format(_T("Edit Line Width"));
			break;
			case 5:
			string->Format(_T("Edit Axis Line Width"));
			break;
			case 6:
			string->Format(_T("Edit Symbol Size"));
			break;
			case 7:
			string->Format(_T("Edit Scale Size"));
			break;
			case 8:
			string->Format(_T("Edit Minimum number of points between ticks on X scale"));
			break;
			case 9:
			string->Format(_T("Edit Minimum number of points between ticks on X scale"));
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
				string->Format(_T("Double click to Edit Y axis Title"));
				break;
			case 1:
				string->Format(_T("Double click to Edit  Y axis min Value"));
				break;
			case 2:
				string->Format(_T("Double click to Edit  Y axis max Value"));
				break;
			case 3:
				string->Format(_T("Double click to Edit  Y axis grid size value"));
				break;
			case 4:
				string->Format(_T("Double click to Edit  X axis max Value"));
				break;
			default:
				return false;
		}
		return true;
	}
	else if(m_Mode==3) {

		int ArrayIndex=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, row);
		if(ArrayIndex>=0) {
			void *Ptr=m_pSimDoc->m_PlotStorage.GetPtr(ArrayIndex);
			SimB *pX;
			CPG *pPG1;
			if(Ptr!=nullptr) {
				auto	datatype=m_pSimDoc->m_PlotStorage.GetDataType(ArrayIndex);
				if(datatype== DATA_TYPES::ANYDATA||datatype== DATA_TYPES::PGDATA) {
					pPG1=(CPG*)Ptr;
					pX=nullptr;
				}
				else {
					pX=(SimB*)Ptr;	
					pPG1=nullptr;

				}
			}
			else {
				pX=nullptr;
				pPG1=nullptr;
			}

			if(pX!=nullptr&&col==1) {
				CString str=CString(pX->GetName().c_str());
				int ilocal=m_pSimDoc->m_PlotStorage.GetLocalIndex(ArrayIndex);
				auto	datatype=m_pSimDoc->m_PlotStorage.GetDataType(ArrayIndex);
				m_pProp->m_MultiSequence=m_pSimDoc->m_PlotStorage.GetValRunNo(ArrayIndex);
				if(ilocal>=0)
				str+=_T("(")+MFC_Util::ItoAscii(ilocal+1)+_T(")");
				if(m_pSimDoc->m_PlotStorage.GetRepitType(ArrayIndex) &&col==0)
					string->SetString(_T("Double click to Edit Legend or \nSingle click to select a new MultiRunNo ")+MFC_Util::ItoAscii(m_pProp->m_MultiSequence)+" ("+MFC_Util::ItoAscii(m_pSimDoc->MR_Get_TotalNumberofRuns())+") :"+str); 
				else if(col==0)
					string->SetString(_T("Double click to Edit Legend for variable (")+MFC_Util::ItoAscii(row+1)+") :"+str); 
				
				return true;
			}
			else if(pPG1!=nullptr) {
				CString str;
				CUGCell cell; 
				GetCell(0,row, &cell);	
				str=cell.GetText();
				int ilocal=m_pSimDoc->m_PlotStorage.GetLocalIndex(ArrayIndex);
				int koll=m_pSimDoc->m_PlotStorage.GetPGSectionNo(ArrayIndex);
				if(koll>0) {
					str=cell.GetText();
					if(koll>10000)
						string->SetString(_T("Single click to add variable :")+str); 
					else
						string->SetString(_T("Single click to remove variable :")+str); 

				}
				else if(col==0) {
					str=pPG1->GetVarName(ilocal).c_str();str+="/",str+=pPG1->GetVarId(ilocal).c_str();
					string->SetString(_T("Double click to Edit Legend for variable (")+MFC_Util::ItoAscii(row+1)+") :"+str); 
				}

				return true;

			}
			else if(col==0)
				string->Format(_T("Double click to edit Legend for variable no: %d"),row+1);
			else if(col==1)
				string->Format(_T("Right click to change color for variable no: %d"),row+1);
			else if(col==2)
				string->Format(_T("Click combo button to change line width for variable no: %d"),row+1);
			else if(col==3)
				string->Format(_T("Click combo button to change line style for variable no: %d"),row+1);
			else if(col==4)
				string->Format(_T("Click combo button to change symbol for variable no: %d"),row+1);
			else if(col==5)
				string->Format(_T("Click combo button to change size for variable no: %d"),row+1);
			else if(col==6)
				string->Format(_T("Single click to change Subchart for variable no: %d"),row+1);
			else if(col==7)
				string->Format(_T("Click combo button to change y axis for variable no: %d"),row+1);
			else if(col==8)
				string->Format(_T(""));
			else if(col==9)
				string->Format(_T("Single click to change Multi RunNo for variable no: %d"),row+1);

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
			if(nullptr == cell1->GetText() && nullptr == cell2->GetText())
				return 0;
			if(nullptr == cell1->GetText())
				return 1;
			if(nullptr == cell2->GetText())
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
	if(nullptr == cell1->GetText())
		return 1;
	if(nullptr == cell2->GetText())
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


BOOL ChartCug::InitData(Doc *pDoc, Ps *pCP, P *pCPt, int index)
{
	CString str;
	if(m_pSimDoc!=nullptr) {
		m_pSimDoc=pDoc;
		m_pView=m_pSimDoc->GetModelView();
		m_pCP=pCP;
		m_pCPt=pCPt;
		m_TableIndex=index;
		m_pProp->m_ChartType=CHART_TYPES::SCATTER;
		//PARAMETERS_mp_DEFINEPLOT=1; 
	//	PARAMETERS_mp_SHIFTCURVE=0;

		if(m_TableIndex>=0) {
			m_pProp->m_Title=((Tab*)m_pCP)->GetViewTitle().c_str();
			if(((Tab*)m_pCP)->GetViewFunction()==401){

			//	if(!PLOTPF_mp_OPENDATABASE) m_pSimDoc->SP_OpenDataBase();
				if(p_PFCurve->IsOpen()&&p_PFCurve->IsProfile()) {

				//PLOTPF_mp_RETRIEVEPF_POINTS();
				if(p_PFCurve->GetNumPF_Pressures()>0) 
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

BOOL ChartCug::InitData(Doc *pDoc, int chartnum, CHART_TYPES charttype, std::string PGFile, CView *view)
{
		CString str;
	CString filename;
	CString stdbinfilename;
	m_pSimDoc=pDoc;
	CCoupModelView *pView=pDoc->GetModelView();
	m_pChartViewer=&pView->m_ChartViewer;

	m_DistMin=m_DistMax=0.;
	if(m_pSimDoc!=nullptr&&m_pSimDoc->IsFinishedSimulation()||PGFile!="") {
		m_pUtil->m_MultiRunPlot=false;
		m_pCP=nullptr  ; // No parameter value plot

		m_pProp->m_ChartNumber=chartnum;
		m_pProp->m_ChartType=charttype;
		m_pView=m_pSimDoc->GetModelView();
		//if(view!=nullptr)
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
	m_pProp->m_ChartType= CHART_TYPES::CONTOUR_CHART;
	m_pView=m_pSimDoc->GetModelView();
	m_pUtil->m_MultiRunPlot=false;
	m_pUtil->m_MultiPGPlot=true;

	m_pProp->SetMultiPGFilePlot();

	return true;
}
BOOL ChartCug::InitMultiRunPlot(Doc *pSimDoc,int chartnum, CHART_TYPES charttype, CCoupModelView *view)
{
	CString str;
	m_pSimDoc=pSimDoc;

	m_DistMin=m_DistMax=0;

	if(m_pSimDoc!=nullptr) {
			// Check and open MultiRunFile
		if(m_pSimDoc->MR_Storage_Open()) {
			m_pUtil->m_MultiRunPlot=true;
			m_pUtil->m_MultiPGPlot=false;
			if(m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val()>0&&m_pSimDoc->m_ValidationData.GetNumSumVarVariables()>0)
				m_pProp->m_MultiValid.AddOnlyValidRuns=true;
			m_pCP=nullptr;
			m_pProp->m_ChartNumber=chartnum;
			m_pProp->m_ChartType=charttype;
			m_pView=m_pSimDoc->GetModelView();

			if(m_pProp->m_ChartType ==CHART_TYPES::SCATTER) {
					// Check for two variables to make scatter plot			
					int   ichart;

						m_NumVarSelected=0;
						for (size_t j=0;j<m_pSimDoc->m_PlotStorage.NumSelected();j++) {
							m_noOfPoints=m_pSimDoc->MR_Get_TotalNumberofRuns();
							auto itype=m_pSimDoc->m_PlotStorage.GetDataType(j);
							ichart=m_pSimDoc->m_PlotStorage.GetChart(j);
							
							if(ichart==m_pProp->m_ChartNumber) {
								m_NumVarSelected++;
							}
						}
						if(m_NumVarSelected<2&&m_pProp->GetFirstIndepIndex()==0) {
							MFC_Util::MessageBox("Independent variable select. Select also dependent variables !");
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
	if(m_pView==nullptr&&m_pSimDoc!=nullptr) 	m_pView=m_pSimDoc->GetModelView();
	if(m_pView==nullptr) return;
	DrawTextPlot(&m_pView->m_ChartViewer);
//	m_pProp->Reset(true);

}

void ChartCug::DrawTextPlot(CChartViewer *viewer)
{
	XYChart *c;
	c = new  XYChart(m_pProp->m_DisplayXSize, m_pProp->m_DisplayYSize, m_pUtil->m_ColorBackground);
	c->addTitle(Chart::Center,CT2A(m_TextPlot).m_psz,"arialbd.ttf", 20);
	if(m_pChartViewer!=nullptr) {

/*		if(!m_TimeSeriePlot.CleanPrevCharts()) {
			BaseChart *pChart;
			pChart=m_pChartViewer->getChart();
			if(pChart!=nullptr)
				delete m_pChartViewer->getChart();
		}*/


	}
	m_pView->MakeChart(c);
	if(m_pUtil->m_FileSave.m_SaveFile)
		m_pUtil->m_FileSave.MakeFile();	
	m_pUtil->m_Displayed=true;
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



	

// User clicks on the Pointer pushbutton
//



