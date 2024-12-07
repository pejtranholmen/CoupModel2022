/***************************************************
****************************************************
Skeleton Class for a Derived MyPGCug
****************************************************
****************************************************/

/*#include "stdafx.h"
//#include "uglibsel.h"
#include "resource.h"

#include "MyPGCug.h"*/
#include "..\..\stdafx.h"
#include "..\..\std.h"
#include "..\..\Util.h"
#include "..\..\CoupModel.h"
#include "..\ChartProperties\ChartCug.h"

//#include "afxole.h"
#include "..\..\ModelTypes\SimB.h"



#include "math.h"

#include "..\..\resource.h"

#include ".\MyPGCug.h"
#include "..\ColourPickerDlg.h"
#include "..\..\PG\CreatePG.h"
#include "..\..\PG\PG_FILEDLG.h"
#include "..\..\PG\NewPGVariables.h"

#include "..\SimDoc\Mycug.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(MyPGCug,CUGCtrl)
	//{{AFX_MSG_MAP(MyPGCug)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/***************************************************
****************************************************/



MyPGCug::MyPGCug()
{
	m_pDoc=nullptr;
	m_bDrawExcelFocus = FALSE;
	m_DocumentDefined=FALSE;
	if(m_pDoc!=nullptr) {

		int i=0;
	}
	m_fontsList = nullptr;
	//UGXPThemes::UseThemes(FALSE);
	m_pApp = AfxGetApp();
	CString m_StartInDir, Fil;
	m_StartInDir=MFC_Util::GetProfileString( "StartDirectory", m_StartInDir);
	//Fil=m_StartInDir+"\\BITMAP8.BMP";		
//	m_BMP1=AddBitmap(IDB_BITMAP2);
	m_Prior=_T("");
	m_After=_T("");
	//m_largeFont.CreateFont(18,0,0,0,600,0,0,0,0,0,0,0,0,_T("Arial"));
	m_extralargeFont.CreateFont(40,0,0,0,600,0,0,0,0,0,0,0,0,_T("Arial"));
	
	//m_font.CreateFont(16,0,0,0,500,0,0,0,0,0,0,0,0,_T("Arial"));

	GetDefaultFont(m_font);
	GetDefaultLargeFont(m_largeFont);
	m_sort[1]=TRUE; m_sort[2]=TRUE; m_sort[3]=TRUE; m_sort[4]=TRUE; m_sort[0]=TRUE;

	m_ColorEdit=RGB(40,240,240);
	m_ColorChanged=RGB(255,200,200);
	m_ColorHeading=RGB(230,230,230);
	m_ColorClick=RGB(240,240,50);
	m_NoColor=RGB(255,255,255);
	m_ColorRClick=RGB(230,230,230);
	m_ColorCompleted=RGB(80,210,80);

	m_created=FALSE;


	m_newChart=FALSE;

	m_Display=TRUE;

	for(int i=0;i<MAXSIMVAL;i++) {
		m_OutAllMulti_Defined[i]=FALSE;
		m_OutSelectedMulti_Defined[i]=FALSE;
	}
	m_OutMAPMulti_Defined=FALSE;
	m_NeedParUpdate=FALSE;
	m_ShowMultiTimeSerieTable=FALSE;
	SetMultiSelectMode ( TRUE );
	DragDropTarget ( TRUE );
	m_ShowChartTableNo=-1;
	m_ShowChartTable=FALSE;
	m_LinkedChartDocument=FALSE;
	m_NumOpenDoc=1;
	m_storage_exist=TRUE;
	m_CurrentTopRow=0;
	m_ViewCategory=0;
	m_OutPutFile_Exist=TRUE;
	m_TobePlotted=FALSE;
	m_ShowValidCoVarRes=FALSE;
	m_ShowValidationTimeSerieTable=FALSE;
	m_CurrentSubChart=1;
	m_RepFirst=true;
	m_ShowDetails=false;
	m_Latitude=400., m_Longitude=400.;
	m_GridLat=3; m_GridLong=2;
	m_Lat_Max=m_Lat_Min=m_Long_Max=m_Long_Min=400.;
	m_pGSource=nullptr;
	m_IndexSource=0;
	for (int i=0; i<7; i++)
		m_DestFileSet[i]=false;
	m_SpatialSourceFileReady=false;
	m_SubstituteOption=0;
	m_AggregateOption=0;
	m_MaxSubstituteDays=3.;
	m_ShowSubResults=true;
	m_pPG_Results=nullptr;
	m_ChangePeriodOption=1;
	m_NewResolutionOption=m_NewResolutionOption_Extra=0;
	MySingle::instance().SetPGCugPointer(this);
	m_pPGArchive=MySingle::instance().GetPGFileArchivePointer();
}

void MyPGCug::NeedParUpdate() {
	m_NeedParUpdate=TRUE;
}
void MyPGCug::SetDocument(CCoupModelDoc *pDoc)
{

	if (pDoc == nullptr) {
		m_pDoc = nullptr;
		m_pSimDoc = nullptr;
		m_pPG = nullptr;
		return;


	}
	m_pView=pDoc->m_Doc.GetModelView();
	m_pDoc=pDoc;
	m_pSimDoc=&pDoc->m_Doc;
	m_DocumentDefined=TRUE;
	m_IndexSource=m_pPGArchive->GetIndex(m_pDoc);
	m_PGCharts.m_pSimDoc=m_pSimDoc;
	m_PGCharts.m_pDoc=m_pDoc;
	PlotInfo *pPlotinfo=MySingle::instance().GetPlotInfoPointer();
	pPlotinfo->SetDoc(m_pSimDoc);

	if (m_pSimDoc->m_DocumentType == 2) {

		if (m_pProp->GetFirstIndepVarType() == MULTIRUN_NUM) {
			m_pProp->SetFirstIndepVarType(DATEAXIS);
		}
	}



}
void MyPGCug::SetViewPointer(CCoupModelView *pview)
{

	m_pView=pview;


}
MyPGCug::~MyPGCug()
{
		// clean up possible fonts allocated by the user
	_tagFonts *pFont, *pTemp;
	pFont = m_fontsList;
	while ( pFont != nullptr )
	{
		pTemp = pFont->next;
		delete pFont;
		pFont = pTemp;
	}
	m_largeFont.DeleteObject();
	m_extralargeFont.DeleteObject();
	m_font.DeleteObject();

	//UGXPThemes::CleanUp();
}



BOOL MyPGCug::isCreated()
{
	if(m_created)
		return TRUE;
	return FALSE;

}
void MyPGCug::OnSetup()
{
	CUGCell cell; 
	CString heading, number, test;


	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2;

	CString str1;
	int MaxCol=8;

	EnableMenu(TRUE); 
	EnableJoins(TRUE); 
	EnableUpdate(TRUE);
	m_created=TRUE;
	UseHints(TRUE);

	SetDefFont(&m_font);

	int num_sheets=GetNumberSheets();
	for (long i=0;i<=num_sheets;i++) {
			DeleteTab(i);
	}
	
	int NumTab=m_pPGArchive->GetNumPGFile();

	CString txt;
	if (NumTab == 0) {
		AddTab("Create New File", 0);
	}
	else if(NumTab>0) {
		AddTab("Main File View", 0);
		AddTab("Detailed description",1);
		AddTab("New single position",2);
		AddTab("New grid positions",3);
		AddTab("Data Substitution",4);
		AddTab("Data Aggregation",5);
		AddTab("Change Period",6);
		AddTab("New Data Resolution",7);
		AddTab("Create New File", 8);
		AddTab("Change Data", 9);
		AddTab("Change Description", 10);
	}

	SetTabWidth(1850); 

	SetDefColWidth(130);
	if(NumTab>0) 
		SetNumberSheets(11);
	else
		SetNumberSheets(1);
	
	OnSheetSetup(0);
	SetCurrentTab(0);
	
		
		


	
}

/////////////////////////////////////////////////////////////////////////////
//	OnSheetSetup	
//		This notification is called for each additional sheet that the grid
//		might contain, here you can customize each sheet in the grid.
//	Params:
//		sheetNumber - index of current sheet
//	Return:
//		<none>


void MyPGCug::OnSheetSetup(int sheetNumber)
{
	UNREFERENCED_PARAMETER(sheetNumber);
	CUGCell cell; 
	CString heading, number, test;

	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	CString str1, Defstring;
	CString optionlist;
	int MaxCol=8;
	int colv=0;

	EnableMenu(TRUE);
	MyCug *pCug=MySingle::instance().GetCugPointer();
	auto pCUtil = MySingle::instance().GetChartUtilPointer();
	pCUtil->SetAutoX_ScaleValue(0);
	SetMultiSelectMode(FALSE);
	DragDropTarget(FALSE);

	if(sheetNumber==0) {
		SetSheetNumber(0); 
		if (m_pPGArchive->GetNumPGFile() > 0) {
			SetLocalWindowText("PG - Main view");
			Make_PGFileSheet();

		}
		else {
			SetLocalWindowText("PG - Create New File");
			Make_New_PGFiles();
		}
	}
	else if(sheetNumber==1) {
		SetLocalWindowText("PG - Detailed View");
		Make_Description();
	}
	else if(sheetNumber==2) {
		SetLocalWindowText("PG - New specific coordinate");
		SetSheetNumber(sheetNumber); 
		Make_NewSpatialCoord(0);
	}
	else if(sheetNumber==3) {
		SetLocalWindowText("PG - New grided coordinate");
		SetSheetNumber(sheetNumber); 
		Make_NewSpatialGrid(0);
	}
	else if(sheetNumber==4) {
		SetLocalWindowText("PG - Gap Filling Procedures");
		SetSheetNumber(sheetNumber); 
		Make_DataSubstitution();
	}
	else if(sheetNumber==5) {
		SetLocalWindowText("PG - New aggregated Resolution");
		SetSheetNumber(sheetNumber); 
		Make_DataAggregation();
	}
	else if(sheetNumber==6) {
		SetLocalWindowText("PG - Create File with a Subperiod of entire period");
		SetSheetNumber(sheetNumber); 
		Make_ChangePeriodSheet();
	}
	else if(sheetNumber==7) {
		SetLocalWindowText("PG - Scaling of Values - based on Values from another file with different time resolution");
		SetSheetNumber(sheetNumber); 
		Make_NewDataResolution();
	}
	else if (sheetNumber == 8) {
		SetLocalWindowText("PG - Create new File");
		SetSheetNumber(sheetNumber);
		Make_New_PGFiles();
	}
	else if (sheetNumber == 9) {
		SetLocalWindowText("Change/ReCalculate Values of All Data");
		SetMultiSelectMode(TRUE);
		DragDropTarget(TRUE);
		SetSheetNumber(sheetNumber);
		Make_DataHeader();
	}
	else if (sheetNumber == 10) {
		SetLocalWindowText("Change The Description of Variables");
		SetMultiSelectMode(TRUE);
		DragDropTarget(TRUE);
		SetSheetNumber(sheetNumber);
		Make_DescriptionHeader();
	}
}


/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		is sent when a cell change action was instigated
//		( user clicked on another cell, used keyboard arrows,
//		or Goto[...] function was called ).
//	Params:
//		oldcol, oldrow - 
//		newcol, newrow - cell that is gaining focus
//	Return:
//		TRUE - to allow the move
//		FALSE - to prevent new cell from gaining focus
int MyPGCug::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newcol);
	UNREFERENCED_PARAMETER(newrow);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanViewMove
//		is called when current grid's view is about to be scrolled.
//	Params:
//		oldcol, oldrow - coordinates of original top-left cell
//		newcol, newrow - coordinates of top-left cell that is gaining the focus
//	Return:
//		TRUE - to allow for the scroll
//		FALSE - to prevent the view from scrolling
int MyPGCug::OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newcol);
	UNREFERENCED_PARAMETER(newrow);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHitBottom
//		This notification allows for dynamic row loading, it will be called
//		when the grid's drawing function has hit the last row.  It allows the grid
//		to ask the datasource/developer if there are additional rows to be displayed.
//	Params:
//		numrows		- known number of rows in the grid
//		rowspast	- number of extra rows that the grid is looking for in the datasource
//		rowsfound	- number of rows actually found, usually equal to rowspast or zero.
//	Return:
//		<none>
void MyPGCug::OnHitBottom(long numrows,long rowspast,long rowsfound)
{
	UNREFERENCED_PARAMETER(numrows);
	UNREFERENCED_PARAMETER(rowspast);
	UNREFERENCED_PARAMETER(rowsfound);
//****** Set the Number of Rows 
//SetNumberRows(numrows+rowsfound, FALSE); 




	// used by the datasources
	if ( rowsfound > 0 )
	{
		SetNumberRows( numrows + rowsfound, FALSE );
	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnHitTop
//		Is called when the user has scrolled all the way to the top of the grid.
//	Params:
//		numrows		- known number of rows in the grid
//		rowspast	- number of extra rows that the grid is looking for in the datasource
//	Return:
//		<none>
void MyPGCug::OnHitTop(long numrows,long rowspast)
{
	UNREFERENCED_PARAMETER(numrows);
	UNREFERENCED_PARAMETER(rowspast);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeCol
//		is sent when the mouse was moved in the area between
//		columns on the top heading, indicating that the user
//		might want to resize a column.
//	Params:
//		col - identifies column number that will be sized
//	Return:
//		TRUE - to allow sizing
//		FALSE - to prevent sizing
int MyPGCug::OnCanSizeCol(int col)
{
	UNREFERENCED_PARAMETER(col);
//****** Disallow column 3 from being resized
//if (col >8){ 

//MessageBox("illegal Procedure!!- Column 3 cannot be resized", "Grid Error", MB_ICONSTOP);
//	return FALSE; 

//} 

return TRUE; 

}

/////////////////////////////////////////////////////////////////////////////
//	OnColSizing
//		is sent continuously when user is sizing a column.
//		This notification is ideal to provide min/max width checks.
//	Params:
//		col - column currently sizing
//		width - current new column width
//	Return:
//		<none>
void MyPGCug::OnColSizing(int col,int *width)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(*width);
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSized
//		This is sent when the user finished sizing the 
//		given column.(see above for more information)
//	Params:
//		col - column sized
//		width - new column width
//	Return:
//		<none>
void MyPGCug::OnColSized(int col,int *width)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(*width);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeRow
//		is sent when the mouse was moved in the area between
//		rows on the side heading, indicating that the user
//		might want to resize a row.
//	Params:
//		row - identifies row number that will be sized
//	Return:
//		TRUE - to allow sizing
//		FALSE - to prevent sizing
int MyPGCug::OnCanSizeRow(long row)
{
	UNREFERENCED_PARAMETER(row);
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSizing
//		Sent during user sizing of a row, can provide
//		feed back on current height
//	Params:
//		row - row sizing
//		height - pointer to current new row height
//	Return:
//		<none>
void MyPGCug::OnRowSizing(long row,int *height)
{
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*height);
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSized
//		This is sent when the user is finished sizing the
//		given row.
//	Params:
//		row - row sized
//		height - pointer to current new row height
//	Return:
//		<none>
void MyPGCug::OnRowSized(long row,int *height)
{
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*height);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeSideHdg
//		Sent when the user is about to start sizing of the side heading
//	Params:
//		<none>
//	Return:
//		TRUE - to allow sizing
//		FALSE - to prevent sizing
int MyPGCug::OnCanSizeSideHdg()
{
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSizing
//		Sent while the heading is being sized
//	Params:
//		width - pointer to current new width of the side heading
//	Return:
//		TRUE - to accept current new size
//		FALSE - to stop sizing, the size is either too large or too small
int MyPGCug::OnSideHdgSizing(int *width)
{
	UNREFERENCED_PARAMETER(*width);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSized
//		Sent when the user has completed the sizing of the side heading
//	Params:
//		width - pointer to new width
//	Return:
//		TRUE - to accept new size
//		FALSE - to revert to old size
int MyPGCug::OnSideHdgSized(int *width)
{
	UNREFERENCED_PARAMETER(*width);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeTopHdg
//		Sent when the user is about to start sizing of the top heading
//	Params:
//		<none>
//	Return:
//		TRUE - to allow sizing
//		FALSE - to prevent sizing
int MyPGCug::OnCanSizeTopHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSizing
//		Sent while the top heading is being sized
//	Params:
//		height - pointer to current new height of the top heading
//	Return:
//		TRUE - to accept current new size
//		FALSE - to stop sizing, the size is either too large or too small
int MyPGCug::OnTopHdgSizing(int *height)
{
	UNREFERENCED_PARAMETER(*height);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSized
//		Sent when the user has completed the sizing of the top heading
//	Params:
//		height - pointer to new height
//	Return:
//		TRUE - to accept new size
//		FALSE - to revert to old size
int MyPGCug::OnTopHdgSized(int *height)
{
	UNREFERENCED_PARAMETER(*height);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColChange
//		Sent whenever the current column changes
//	Params:
//		oldcol - column that is loosing the focus
//		newcol - column that the user move into
//	Return:
//		<none>
void MyPGCug::OnColChange(int oldcol,int newcol)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(newcol);
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowChange
//		Sent whenever the current row changes
//	Params:
//		oldrow - row that is loosing the locus
//		newrow - row that user moved into
//	Return:
//		<none>
void MyPGCug::OnRowChange(long oldrow,long newrow)
{
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newrow);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellChange
//		Sent whenever the current cell changes
//	Params:
//		oldcol, oldrow - coordinates of cell that is loosing the focus
//		newcol, newrow - coordinates of cell that is gaining the focus
//	Return:
//		<none>
void MyPGCug::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(newcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newrow);
}

/////////////////////////////////////////////////////////////////////////////
//	OnLeftColChange
//		Sent whenever the left visible column in the grid changes, indicating
//		that the view was scrolled horizontally
//	Params:
//		oldcol - column that used to be on the left
//		newcol - new column that is going to be the first visible column from the left
//	Return:
//		<none>
void MyPGCug::OnLeftColChange(int oldcol,int newcol)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(newcol);
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopRowChange
//		Sent whenever the top visible row in the grid changes, indicating
//		that the view was scrolled vertically
//	Params:
//		oldrow - row that used to be on the top
//		newrow - new row that is going to be the first visible row from the top
//	Return:
//		<none>
void MyPGCug::OnTopRowChange(long oldrow,long newrow)
{
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newrow);
}

/////////////////////////////////////////////////////////////////////////////
//	OnViewMoved
//		This notification is fired after the current viewing area
//		was scrolled.
//	Params:
//		nScrolDir - UG_VSCROLL, UG_HSCROLL
//
//		if the nScrolDir == UG_VSCROLL
//			oldPos - row that used to be on the top
//			newPos - row that is now the first visible row from the top
//
//		if the nScrolDir == UG_VSCROLL
//			oldPos - column that used to be on the left
//			newPos - column that is now the first visible column from the left
//	Return:
//		<none>
void MyPGCug::OnViewMoved( int nScrolDir, long oldPos, long newPos )
{
	UNREFERENCED_PARAMETER(nScrolDir);
	UNREFERENCED_PARAMETER(oldPos);
	UNREFERENCED_PARAMETER(newPos);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSelectionChanged
//		The OnSelectionChanged notification is called by the multiselect
//		class when a change occurred in current selection (i.e. user clicks
//		on a new cell, drags a mouse selecting range of cells, or uses
//		CTRL/SHIFT - left click key combination to select cells.)
//	Params:
//		startCol, startRow	- coordinates of the cell starting the selection block
//		endCol, endRow		- coordinates of the cell ending the selection block
//		blockNum			- block number representing this range, this will
//							always represent total number of selection blocks.
//	Return:
//		<none>
void MyPGCug::OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum)
{
	UNREFERENCED_PARAMETER(startCol);
	UNREFERENCED_PARAMETER(startRow);
	UNREFERENCED_PARAMETER(endCol);
	UNREFERENCED_PARAMETER(endRow);
	UNREFERENCED_PARAMETER(blockNum);


}

/////////////////////////////////////////////////////////////////////////////
//	OnLClicked
//		Sent whenever the user clicks the left mouse button within the grid
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyPGCug::OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, int processed)
{


	CString str1;
	CUGCell cell, cell1, cell2;
	long ParV;
	CString Group, Name, Type;


	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);

	GetCell(col, row, &cell);
	ParV = cell.GetParam();
	auto typ = cell.GetCellType();
	if (typ == UGCT_DROPLIST) return;
	int isheet = GetSheetNumber();
	if (ParV >= m_ComboIdButton.Param && ParV<m_ComboIdButton.Param+5) {
		return;

	}
	else if (isheet == 0 && (ParV != m_VarSelectBox.param&&ParV != m_ChartNumberSelect.param) || isheet >= 8 && ParV > 0) {
		if (ParV == m_ClickBoxFormatButton.Param) return;
		if (ParV ==m_ElementTypeNewVariables.Param) return;
		if (ParV == m_UnitTypeNewVariables.Param) return;
		if (ParV == m_ElementTypeVariables.Param) return;
		if (ParV == m_UnitTypeVariables.Param) return;
		if (ParV == m_FilterVariableButton.Param) return;
		if (ParV == m_FilterUseButton.Param) return;
		if (!updn) return;
		GetCell(-1, row, &cell);
		AllSheetButtons(isheet, ParV, int(cell.GetNumber()), col);
		return;

	}
	else if (ParV == m_ChartNumberSelect.param) {
		ChangeChartVar(col, row);
	}
	else if (ParV >= 700 && ParV<705 && row == 5) {
		return;

	}

	if (updn) return;
//******* Amend the cell using 'Get..Set'
	if (m_pSimDoc == nullptr) return;
	if (m_pSimDoc->m_DocumentType != 2) {

		m_pProp->DocumentError(m_pSimDoc);

	}

	if (ParV == 1000) {
		ExpandFileNameLine(col, row, ParV);
	}
	else if ((ParV == m_VarSelectBox.param && col == 3) || ParV == 3100 && col == 0) {
		ChangeChartVar(col, row);

	}
	else if (ParV == m_ChartNumberSelect.param && col == 4)
		ChangeChartVar(col, row);
	else if (ParV == 3100 && (col == 5 || col == 6))
		ChangeChartVar(col, row);




	if(!updn&&isheet==4) {

		if (row == 2 && col == 5) {
			// Action for substitution of data
			if (m_SubstituteOption == 0) {
				CPG *pPG = (CPG*)m_pPGArchive->m_PGFileArray[m_IndexSource].pFile;
				m_pPG_Results = m_spatialPG.CreateNewSpatialCorrelationFile(pPG);
				Make_DataSubstitution();

			}
			else if (m_SubstituteOption == 1) {
				m_spatialPG.SetSourceFile(m_pGSource);
				CPG *pPGDest = m_spatialPG.CreateNewInterPolatedFile(m_MaxSubstituteDays);
				Make_DataSubstitution();

			}
			else if (m_SubstituteOption == 2) {
				m_spatialPG.SetSourceFile(m_pGSource);
				CPG *pPGDest = m_spatialPG.CreateNewRandomFillingFile();
				Make_DataSubstitution();
			}
		}
		else if (row == 1 && col == 0) {
			if (row == 1) {
				GetCell(col, row, &cell);
				string txt = cell.GetStdText();
				if (FUtil::IsFileExisting(txt))	AddPGFile(txt);
			}
			m_ShowSubResults = false;
			m_spatialPG.m_ResRandomSub.clear();




		}
		else if (col == 0 && m_spatialPG.m_statsub.size() + 1 == row) {
			if (row == m_spatialPG.m_statsub.size() + 1) {
				GetCell(col, row, &cell);
				string txt = cell.GetStdText();
				if (FUtil::IsFileExisting(txt))	AddPGFile(txt);
			}
			m_ShowSubResults = false;
			m_spatialPG.m_statsub.clear();


		}


	}
	if(!updn&&isheet==5) {
		// Action for substitution of data
	 if(row==2&&col==5) {
			CPG *pPG=(CPG*)m_pPGArchive->m_PGFileArray[m_IndexSource].pFile;
			bool res=m_TimeAggregate.CreateNewAggregatedFile(pPG, m_AggregateOption, m_AggregationStepSize);
			m_ShowSubResults=true;
			if(res)  Make_DataAggregation();
			else {
					string	str="Failed by some reason to create new file ";
				//Display error message
				MFC_Util::MessageBox(str,"Error when estimating new aggregated values",MB_OK);


			}
	 }
	 else if(col==0&&m_ShowSubResults&&m_TimeAggregate.m_ResultSum.size()>0) {
		if(row==1) { 
			GetCell(col,row,&cell);
			string txt=cell.GetStdText();
			if(FUtil::IsFileExisting(txt))	AddPGFile(txt);
		}	
		m_ShowSubResults=false;
		m_TimeAggregate.m_ResultSum.clear();
		
	 }





	}
	if(!updn&&isheet==6) {

		if (row == 2 && col == 5) {
			// Action for substitution of data

			CPG *pPG = (CPG*)m_pPGArchive->m_PGFileArray[m_IndexSource].pFile;
			m_ShowSubResults = true;
			m_ChangePeriod.m_ResultSum.clear();
			bool res = m_ChangePeriod.CreateNewFile(pPG, m_ChangePeriodOption);

			Make_ChangePeriodSheet();
		}
		else if (row == 1) {
			GetCell(col, row, &cell);
			string txt = cell.GetStdText();
			if (FUtil::IsFileExisting(txt)) {
				AddPGFile(txt);
				m_ShowSubResults = false;
				m_ChangePeriod.m_ResultSum.clear();
			}

		}




	}
	
	if(!updn&&isheet==7&&col==0&&m_ShowSubResults&&m_ChangeResolution.m_ResultSum.size()>0) {
		m_ShowCalculating=false;
		if(row==1) { 
			GetCell(col,row,&cell);
			string txt=cell.GetStdText();
			if(FUtil::IsFileExisting(txt))	AddPGFile(txt);
		}	
		m_ShowSubResults=false;
		m_ChangeResolution.m_ResultSum.clear();
	 }
	 else if(!updn&&isheet==7&&row==1&&col==0&&m_pPG_ScalingSource==nullptr) {
		// Action for substitution of data
		    DefineScalingFile();
			/*m_ShowCalculating=true;
			Make_NewDataResolution();

			CPG *pPG=(CPG*)m_pPGArchive->m_PGFileArray[m_IndexSource].pFile;
			bool res=m_ChangeResolution.CreateNewFile(pPG,m_pPG_ScalingSource, 0);
			m_ShowCalculating=false;*/
			Make_NewDataResolution();
	}
	else if(!updn&&isheet==7&&row==3&&col==5) {
		// Action for substitution of data
			m_ShowCalculating=true;
			Make_NewDataResolution();

			CPG *pPG=(CPG*)m_pPGArchive->m_PGFileArray[m_IndexSource].pFile;
			bool res=m_ChangeResolution.CreateNewFile(pPG,m_pPG_ScalingSource, m_NewResolutionOption,m_NewResolutionOption_Extra );
			m_ShowCalculating=false;
			Make_NewDataResolution();
	}
	else if(!updn&&isheet==7&&row>3&&(col==0||col==1||col==4)) {
		// Action for substitution of data
		auto numrow=GetNumberRows();
		auto numvar=m_pGSource->GetNumVariables();
		int ivar=row-(numrow-numvar-1);
		if(m_NewResolutionSteps==0) {
			if(col==1)
				m_ChangeResolution.m_MatchingVar.main_scaling_var=ivar;
			else if(col==0)
				m_ChangeResolution.m_MatchingVar.main_source_var=ivar;
			if(m_ChangeResolution.m_MatchingVar.main_scaling_var>0&&
				m_ChangeResolution.m_MatchingVar.main_source_var>0) m_NewResolutionSteps=1;
		}
		else if(m_NewResolutionSteps==1) {
			if(col==1)
				m_ChangeResolution.m_MatchingVar.second_scaling_var=ivar;
			else if(col==0)
				m_ChangeResolution.m_MatchingVar.second_source_var=ivar;
			if(m_ChangeResolution.m_MatchingVar.second_scaling_var>0&&
				m_ChangeResolution.m_MatchingVar.second_source_var>0) m_NewResolutionSteps=2;
		}
		else if(m_NewResolutionSteps==2&&col==4&&ivar>=1) {
			m_NewResolutionSteps=0;
			m_ChangeResolution.m_MatchingVar.main_scaling_var=0;
			m_ChangeResolution.m_MatchingVar.main_source_var=0;
			m_ChangeResolution.m_MatchingVar.second_scaling_var=0;
			m_ChangeResolution.m_MatchingVar.second_source_var=0;
		}

		Make_NewDataResolution();
	}
	else if(!updn||isheet>1) return;
	




	UpdatePlotStatusBar(2);




}
		
/////////////////////////////////////////////////////////////////////////////
//	OnRClicked
//		Sent whenever the user clicks the right mouse button within the grid
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyPGCug::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
//		Sent whenever the user double clicks the left mouse button within the grid
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyPGCug::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
    	StartEdit();


}	
			 
/////////////////////////////////////////////////////////////////////////////
//	OnMouseMove
//		is sent when the user moves mouse over the grid area.
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		point		- represents the screen point where the mouse event was detected
//		nFlags		- 
//		processed	- indicates if current event was processed by other control in the grid.
//	Return:
//		<none>
void MyPGCug::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user clicks the left mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyPGCug::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
	CUGCell cell;
	if (updn&&col >= 0) {

		GetCell(col, row, &cell);
		int ParV = cell.GetParam();
		if (ParV == m_VarTransformationCombo.Param) {
			int sheet = GetSheetNumber();
			string After;
			After= cell.GetStdText();
			ChangeChartChoice(sheet, -1, -1, -1, CString(After.c_str()), ParV);
		}
		else



		if (ParV > 0) AllSheetButtons(GetSheetNumber(), ParV);

	}

}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_RClicked
//		Sent whenever the user clicks the right mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyPGCug::OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top heading
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyPGCug::OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
	CUGCell cell;
	GetCell(col, row, &cell);


	SortBy(col, UG_SORT_DESCENDING); 
		
	}
/////////////////////////////////////////////////////////////////////////////
//	OnSH_LClicked
//		Sent whenever the user clicks the left mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyPGCug::OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_RClicked
//		Sent whenever the user clicks the right mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyPGCug::OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_DClicked
//		Sent whenever the user double clicks the left mouse button within the side heading
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyPGCug::OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_LClicked
//		Sent whenever the user clicks the left mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyPGCug::OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);

	BOOL hide=FALSE;
	

	int isheet=GetSheetNumber();
    
	if(!updn) return;
	if(isheet==1) {
		if(m_ShowDetails) 
			m_ShowDetails=false;
		else
			m_ShowDetails=true;
		OnSheetSetup(1);

	}
	
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_RClicked
//		Sent whenever the user clicks the right mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyPGCug::OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top corner button
//	Params:
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyPGCug::OnCB_DClicked(RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyDown
//		Sent when grid received a WM_KEYDOWN message, usually as a result
//		of a user pressing any key on the keyboard.
//	Params:
//		vcKey		- virtual key code of the key user has pressed
//		processed	- indicates if current event was processed by other control in the grid.
//	Return:
//		<none>
void MyPGCug::OnKeyDown(UINT *vcKey,BOOL processed)
{
	UNREFERENCED_PARAMETER(*vcKey);
	UNREFERENCED_PARAMETER(processed);
//	GetScrollBarCtrl(SB_HORZ)->EnableScrollBar(ESB_DISABLE_BOTH);
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyUp
//		Sent when grid received a WM_KEYUP message, usually as a result
//		of a user releasing a key.
//	Params:
//		vcKey		- virtual key code of the key user has pressed
//		processed	- indicates if current event was processed by other control in the grid.
//	Return:
//		<none>
void MyPGCug::OnKeyUp(UINT *vcKey,BOOL processed)
{
	UNREFERENCED_PARAMETER(*vcKey);
	UNREFERENCED_PARAMETER(processed);
//	GetScrollBarCtrl(SB_HORZ)->EnableScrollBar(ESB_DISABLE_BOTH);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCharDown
//		Sent when grid received a WM_CHAR message, usually as a result
//		of a user pressing any key that represents a printable character.
//	Params:
//		vcKey		- virtual key code of the key user has pressed
//		processed	- indicates if current event was processed by other control in the grid.
//	Return:
//		<none>
void MyPGCug::OnCharDown(UINT *vcKey,BOOL processed)
{
	UNREFERENCED_PARAMETER(*vcKey);
	UNREFERENCED_PARAMETER(processed);
	StartEdit( *vcKey ); 
//	GetScrollBarCtrl(SB_HORZ)->EnableScrollBar(ESB_DISABLE_BOTH);
}
	
/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent every time the grid needs to
//		draw a cell in the grid. At this point the cell
//		object has been populated with all of the information
//		that will be used to draw the cell. This information
//		can now be changed before it is used for drawing.
//	Warning:
//		This notification is called for each cell that needs to be painted
//		Placing complicated calculations here will slowdown the refresh speed.
//	Params:
//		col, row	- coordinates of cell currently drawing
//		cell		- pointer to the cell object that is being drawn
//	Return:
//		<none>
void MyPGCug::OnGetCell(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetCell
//		This notification is sent every time a cell is set,
//		here you have a chance to make final modification
//		to the cell's content before it is saved to the data source.
//	Params:
//		col, row	- coordinates of cell currently saving
//		cell		- pointer to the cell object that is being saved
//	Return:
//		<none>
void MyPGCug::OnSetCell(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);
}

/////////////////////////////////////////////////////////////////////////////
//	OnDataSourceNotify
//		This message is sent from a data source and this message
//		depends on the data source - check the information
//		on the data source(s) being used
//		- The ID of the Data source is also returned
//	Params:
//		ID		- datasource ID
//		msg		- message ID to identify current process
//		param	- additional information or object that might be needed
//	Return:
//		<none>
void MyPGCug::OnDataSourceNotify(int ID,long msg,long param)
{
	UNREFERENCED_PARAMETER(ID);
	UNREFERENCED_PARAMETER(msg);
	UNREFERENCED_PARAMETER(param);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellTypeNotify
//		This notification is sent by the celltype and it is different from cell-type
//		to celltype and even from notification to notification.  It is usually used to
//		provide the developer with some feed back on the cell events and sometimes to
//		ask the developer if given event is to be accepted or not
//	Params:
//		ID			- celltype ID
//		col, row	- co-ordinates cell that is processing the message
//		msg			- message ID to identify current process
//		param		- additional information or object that might be needed
//	Return:
//		TRUE - to allow celltype event
//		FALSE - to disallow the celltype event
int MyPGCug::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	UNREFERENCED_PARAMETER(ID);
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(msg);
	UNREFERENCED_PARAMETER(param);


	CUGCell cell, cell1, cell2; 
	CString Type, Group, Name, Prior, After, test;


//******* Amend the cell using 'Get..Set'
	int isheet=GetSheetNumber();
	if ((isheet == 0 || isheet == 8)&& msg == 103) {
		GetCell(col, row, &cell);
		int ParV = cell.GetParam();

		if ((ParV >= m_ComboIdButton.Param&&ParV<m_ComboIdButton.Param + 5)|| ParV==m_ClickBoxFormatButton.Param||
			ParV == m_ElementTypeNewVariables.Param|| ParV == m_UnitTypeNewVariables.Param||
			ParV == m_FilterVariableButton.Param || ParV == m_FilterUseButton.Param) {
			string After;
			After = cell.GetStdText();
			size_t iopt;
			if (ParV == m_UnitTypeNewVariables.Param) {
				
				iopt = size_t(p_ModelInfo->GetUnitType(After));
			}
			else {
				if (ParV == m_ElementTypeNewVariables.Param) {

					iopt = p_ModelInfo->GetElementFromString(After);
				}
				else
					iopt = FUtil::GetOptionIndex(cell.GetStdLabelText(), After);
				
			}
			ImportListButtons(ParV, iopt, col);
		}
	


	}
	else if (isheet == 10 && msg == 103) {
		GetCell(col, row, &cell);
		int ParV = cell.GetParam();
		string After;
		After = cell.GetStdText();
		size_t iopt;
		if (ParV == m_UnitTypeVariables.Param) {

			iopt = size_t(p_ModelInfo->GetUnitType(After));
		}
		if (ParV == m_ElementTypeVariables.Param) {

			iopt = p_ModelInfo->GetElementFromString(After);
		}
		else
			iopt = FUtil::GetOptionIndex(cell.GetStdLabelText(), After);
		DescriptionListButtons(ParV, iopt, col, row);
	}




	if (m_pSimDoc == nullptr) return TRUE;


	if (m_pSimDoc->m_DocumentType != 2) {

		m_pProp->DocumentError(m_pSimDoc);

	}



	GetCell(col,row,&cell);
	After=cell.GetText();
	CString optionlist=cell.GetLabelText();
	int ind=MFC_Util::GetOptionIndex(optionlist, After);
	int par = cell.GetParam();

	if(msg==103&&isheet==1) {
		ChangeChartChoice(isheet, col, row, ind, After, par);
		m_pGSource=(CPG*)m_pPGArchive->GetPGFile(m_IndexSource);
		
		Make_Description();
		MultiPGChartCheck();

	

	}
	else if(msg==103&&isheet==0) {
		GetCell(col, row, &cell);
		After = cell.GetText();
		CString optionlist = cell.GetLabelText();
		int ind = MFC_Util::GetOptionIndex(optionlist, After);
		int par = cell.GetParam();
		ChangeChartChoice(isheet, col, row, ind, After, par);
		return TRUE;
	
	}
	else if(msg==103&&isheet==0) {
		
		m_pGSource=(CPG*)m_pPGArchive->GetPGFile(m_IndexSource);
		
		Make_PGFileSheet();
		if(col!=3) 	MultiPGChartCheck();

	

	}
	else if(msg==103&&isheet==2) {
		m_pGSource=(CPG*)m_pPGArchive->GetPGFile(ind);
		m_IndexSource=ind;
		Make_NewSpatialCoord();

	}
	else if(msg==103&&isheet==3) {
		m_pGSource=(CPG*)m_pPGArchive->GetPGFile(ind);
		m_IndexSource=ind;
		Make_NewSpatialGrid();

	}
	else if(msg==103&&isheet==4) {
		if(row==0) {
			m_pGSource=(CPG*)m_pPGArchive->GetPGFile(ind);
			m_IndexSource=ind;
			Make_DataSubstitution();
		}
		else if(row==2) {
			m_SubstituteOption=ind;
			Make_DataSubstitution();
		}

	}
	else if(msg==103&&isheet==5) {
		if(row==0) {
			m_pGSource=(CPG*)m_pPGArchive->GetPGFile(ind);
			m_IndexSource=ind;
			Make_DataAggregation();
		}
		else if(row==2) {
			m_AggregateOption=ind;
			Make_DataAggregation();
		}

	}
	else if(msg==103&&isheet==6) {
		if(row==0) {
			m_pGSource=(CPG*)m_pPGArchive->GetPGFile(ind);
			m_IndexSource=ind;
			Make_ChangePeriodSheet();
		}
		else if(row==2) {
			m_ChangePeriodOption=ind;
			Make_ChangePeriodSheet();
		}

	}
	else if(msg==103&&isheet==7) {
		if(row==0) {
			m_pGSource=(CPG*)m_pPGArchive->GetPGFile(ind);
			m_IndexSource=ind;
			Make_NewDataResolution();
		}
		else if(row==3) {
			if(col==1)
				m_NewResolutionOption=ind;
			else if(col==3) 
				m_NewResolutionOption_Extra=ind;
			Make_NewDataResolution();
		}
	}
	else if (msg%50 == 3 && isheet == 10) {
		CheckStatusOfPG_Outputs(col,row);
	}
	isheet=GetSheetNumber();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditStart
//		This message is sent whenever the grid is ready to start editing a cell
//	Params:
//		col, row - location of the cell that edit was requested over
//		edit -	pointer to a pointer to the edit control, allows for swap of edit control
//				if edit control is swapped permanently (for the whole grid) is it better
//				to use 'SetNewEditClass' function.
//	Return:
//		TRUE - to allow the edit to start
//		FALSE - to prevent the edit from starting
int MyPGCug::OnEditStart(int col, long row,CWnd **edit)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(**edit);

	/*if( col == 1 )
		(*edit)->ModifyStyle( 0, ES_UPPERCASE ); 
	else 
		(*edit)->ModifyStyle( ES_UPPERCASE, 0 );

*/
	int isheet=GetSheetNumber();

	if(isheet==2) {
		return TRUE;
	}
	else if(isheet==3) {
		return TRUE;
	}
	else if(isheet==4||isheet==5||isheet==6||isheet==9 || isheet == 10) 
		return TRUE;
	else {
		CUGCell cell;
		GetCell(col, row, &cell);
		if (cell.GetParam() == 0|| cell.GetParam()==m_DoubleClickEdit.param) return TRUE;
		return TRUE;

	}


	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditVerify
//		This notification is sent every time the user hits a key while in edit mode.
//		It is mostly used to create custom behavior of the edit control, because it is
//		so easy to allow or disallow keys hit.
//	Params:
//		col, row	- location of the edit cell
//		edit		-	pointer to the edit control
//		vcKey		- virtual key code of the pressed key
//	Return:
//		TRUE - to accept pressed key
//		FALSE - to do not accept the key
int MyPGCug::OnEditVerify(int col, long row,CWnd *edit,UINT *vcKey)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*edit);
	UNREFERENCED_PARAMETER(*vcKey);

	/*if( col == 0 ){
		// numbers are not allowed in the last name field  
		if( *vcKey >= '0' && *vcKey <= '9' ) 
			return FALSE; 
	} */





	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish
//		This notification is sent when the edit is being finished
//	Params:
//		col, row	- coordinates of the edit cell
//		edit		- pointer to the edit control
//		str1		- actual str1 that user typed in
//		cancelFlag	- indicates if the edit is being canceled
//	Return:
//		TRUE - to allow the edit to proceed
//		FALSE - to force the user back to editing of that same cell
int MyPGCug::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR str1,BOOL cancelFlag)
{ 
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*edit);
	UNREFERENCED_PARAMETER(str1);
	UNREFERENCED_PARAMETER(cancelFlag);
	string txt;
	txt=CT2A(str1).m_psz;
	
	int isheet=GetSheetNumber();
	CUGCell cell;
	GetCell(col, row, &cell);

	return AllEditCells(col, row, txt);





}

/////////////////////////////////////////////////////////////////////////////
//	OnEditContinue
//		This notification is called when the user pressed 'tab' or 'enter' keys
//		Here you have a chance to modify the destination cell
//	Params:
//		oldcol, oldrow - edit cell that is loosing edit focus
//		newcol, newrow - cell that the edit is going into, by changing their
//							values you are able to change where to edit next
//	Return:
//		TRUE - allow the edit to continue
//		FALSE - to prevent the move, the edit will be stopped
int MyPGCug::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(*newcol);
	UNREFERENCED_PARAMETER(*newrow);
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
//	OnMenuStart
//		Is called when the pop up menu is about to be shown
//	Params:
//		col, row	- the cell coordinates of where the menu originated from
//		setcion		- identify for which portion of the gird the menu is for.
//					possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//						UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
//	Return:
//		TRUE - to allow menu to show
//		FALSE - to prevent the menu from poping up

/////////////////////////////////////////////////////////////////////////////
//	OnGetContextHelpID 
//		this notification is called as result of context help bing activated
//		over the UG area and should return context help ID to be displayed
//	Params:
//		col, row	- coordinates of cell which received the message
//		section		- grid section which received this message
//	Return:
//		Context help ID to be shown in the help.
DWORD MyPGCug::OnGetContextHelpID( int col, long row, int section )
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(section);

	CWinApp* pApp = AfxGetApp();
  	CString lDataBaseDirectory;
	lDataBaseDirectory=MFC_Util::GetProfileString("DataBaseDirectory",lDataBaseDirectory);
  
   	CString Dir,txt;
	Dir=lDataBaseDirectory;
	Dir+="\\Coup99.chm";


	int sheet=GetSheetNumber();

	

	txt="PGFile1";





	if(txt=="")
		txt=_T("Howtounderstandthemenusystem");

	Dir += "::/WordDocuments/";;
	Dir+= txt + _T(".htm");

	/*HWND Hwnd=::HtmlHelp(
            nullptr,
            Dir,
            HH_DISPLAY_TOPIC,
            nullptr) ;*/










	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHint
//		Is called whent the hint is about to be displayed on the main grid area,
//		here you have a chance to set the text that will be shown
//	Params:
//		col, row	- the cell coordinates of where the menu originated from
//		str1		- pointer to the str1 that will be shown
//	Return:
//		TRUE - to show the hint
//		FALSE - to prevent the hint from showing
int MyPGCug::OnHint(int col,long row,int section,CString *str1)
{
	CUGCell cell;
	long ParV;
	UNREFERENCED_PARAMETER(section);
	int isheet=GetSheetNumber();
	if(isheet==0||isheet>=8) {
		GetCell(col,row, &cell); 
		ParV=cell.GetParam();
		if (ParV > 0) {
			string text=GetHint(ParV);
			if (text.size()> 0) *str1 = text.c_str();
			return TRUE;
		}
		/*f(ParV==1000&&m_pPGArchive->m_PGFileArray.size()>0) {
			//int index=ParV-1000;
			GetCell(-2,row,&cell);
			int Index=int(cell.GetNumber());
			CPG *pPG=(CPG*)m_pPGArchive->m_PGFileArray[Index].pFile;
			*str1=pPG->GetFileName().c_str();
			return TRUE;

		}*/
	}
	
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnVScrollHint
//		Is called when the hint is about to be displayed on the vertical scroll bar,
//		here you have a chance to set the text that will be shown
//	Params:
//		row		- current top row
//		str1	- pointer to the str1 that will be shown
//	Return:
//		TRUE - to show the hint
//		FALSE - to prevent the hint from showing
int MyPGCug::OnVScrollHint(long row,CString *str1)
{
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*str1);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHScrollHint
//		Is called whent the hint is about to be displayed on the horizontal scroll bar,
//		here you have a chance to set the text that will be shown
//	Params:
//		col		- current left col
//		str1	- pointer to the str1 that will be shown
//	Return:
//		TRUE - to show the hint
//		FALSE - to prevent the hint from showing
int MyPGCug::OnHScrollHint(int col,CString *str1)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(*str1);
	return TRUE;
}

#ifdef __AFXOLE_H__
/////////////////////////////////////////////////////////////////////////////
//	OLE
//		following 3 functions are to be used with drag and drop functionality
/////////////////////////////////////////////////////////////////////////////
//	OnDragEnter
//	Params:
//		pDataObject - 
//	Return:
//		DROPEFFECT_NONE - no drag and drop
//		DROPEFFECT_COPY - allow drag and drop for copying
DROPEFFECT MyPGCug::OnDragEnter(COleDataObject* pDataObject)
{
	UNREFERENCED_PARAMETER(*pDataObject);
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	OnDragOver
//	Params:
//		col, row	-
//		pDataObject - 
//	Return:
//		DROPEFFECT_NONE - no drag and drop
//		DROPEFFECT_COPY - allow drag and drop for copying
DROPEFFECT MyPGCug::OnDragOver(COleDataObject* pDataObject,int col,long row)
{
	UNREFERENCED_PARAMETER(*pDataObject);
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	OnDragDrop
//	Params:
//		col, row	-
//		pDataObject - 
//	Return:
//		DROPEFFECT_NONE - no drag and drop
//		DROPEFFECT_COPY - allow drag and drop for copying
DROPEFFECT MyPGCug::OnDragDrop(COleDataObject* pDataObject,int col,long row)
{
	UNREFERENCED_PARAMETER(*pDataObject);
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	return DROPEFFECT_COPY;
}
#endif

/////////////////////////////////////////////////////////////////////////////
//	OnScreenDCSetup
//		Is called when each of the components are painted.
//	Params:
//		dc		- pointer to the current CDC object
//		section	- section of the grid painted.
//					possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//						UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
//	Return:
//		<none>
void MyPGCug::OnScreenDCSetup(CDC *dc,int section)
{
	UNREFERENCED_PARAMETER(*dc);
	UNREFERENCED_PARAMETER(section);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSortEvaluate
//		Sent as a result of the 'SortBy' call, this is called for each cell
//		comparison and allows for customization of the sorting routines.
//		We provide following code as example of what could be done here,
//		you might have to modify it to give your application customized sorting.
//	Params:
//		cell1, cell2	- pointers to cells that are compared
//		flags			- identifies sort direction
//	Return:
//		value less than zero to identify that the cell1 comes before cell2
//		value equal to zero to identify that the cell1 and cell2 are equal
//		value greater than zero to identify that the cell1 comes after cell2
int MyPGCug::OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags)
{
	// if one of the cells is nullptr, do not compare its text
	if ( cell1 == nullptr && cell2 == nullptr )
		return 0;
	else if ( cell1 == nullptr )
		return 1;
	else if ( cell2 == nullptr )
		return -1;

	if(flags&UG_SORT_DESCENDING)
	{
		CUGCell *ptr = cell1;
		cell1 = cell2;
		cell2 = ptr;
	}

	// initialize variables for numeric check
	double num1, num2;
	// compare the cells, with respect to the cell's datatype
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
			num1 = cell1->GetNumber();
			num2 = cell2->GetNumber();
			if(num1 < num2)
				return -1;
			if(num1 > num2)
				return 1;
			return 0;
		default:
			// if datatype is not recognized, compare cell's text
			if(nullptr == cell1->GetText())
				return 1;
			if(nullptr == cell2->GetText())
				return -1;
			return _tcscmp(cell1->GetText(),cell2->GetText());	
	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnTabSelected
//		Called when the user selects one of the tabs on the bottom of the screen
//	Params:
//		ID	- id of selected tab
//	Return:
//		<none>
void MyPGCug::OnTabSelected(int ID)
{

	if (!CheckForChanges()) return;

	if(ID==0) {
		SetSheetNumber(0);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(0);
	}
	else if(ID==1) {
		SetSheetNumber(1);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(1);
	}
	else if(ID==2) {
		SetSheetNumber(2);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(2);
	}
	else if(ID==3) {
		SetSheetNumber(3);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(3);
	}
	else if(ID==4) {
		SetSheetNumber(4);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(4);
	}
	else if(ID==5) {
		SetSheetNumber(5);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(5);
	}
	else if(ID==6) {
		SetSheetNumber(6);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(6);
	}
	else if(ID==7) {
		SetSheetNumber(7);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(7);
	}
	else if (ID == 8) {
		SetSheetNumber(8);
		EnableUpdate(FALSE);
		while (GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE);
		OnSheetSetup(8);
	}
	else if(ID>8&&ID<11) {
		SetSheetNumber(ID);
		EnableUpdate(FALSE);
		while (GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE);
		if (ID == 9) m_Data = true;
		else m_Data = false;
		OnSheetSetup(ID);


	}

	
}

/////////////////////////////////////////////////////////////////////////////
//	OnAdjustComponentSizes
//		Called when the grid components are being created,
//		sized, and arranged.  This notification provides
//		us with ability to further customize the way
//		the grid will be presented to the user.
//	Params:
//		grid, topHdg, sideHdg, cnrBtn, vScroll, hScroll, 
//		tabs	- sizes and location of each of the grid components
//	Return:
//		<none>
void MyPGCug::OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,
		RECT *cnrBtn,RECT *vScroll,RECT *hScroll,RECT *tabs)
{
	UNREFERENCED_PARAMETER(*grid);
	UNREFERENCED_PARAMETER(*topHdg);
	UNREFERENCED_PARAMETER(*sideHdg);
	UNREFERENCED_PARAMETER(*cnrBtn);
	UNREFERENCED_PARAMETER(*vScroll);
	UNREFERENCED_PARAMETER(*hScroll);
	UNREFERENCED_PARAMETER(*tabs);
} 

/////////////////////////////////////////////////////////////////////////////
//	OnDrawFocusRect
//		Called when the focus rect needs to be painted.
//	Params:
//		dc		- pointer to the current CDC object
//		rect	- rect of the cell that requires the focus rect
//	Return:
//		<none>
/*void MyPGCug::OnDrawFocusRect(CDC *dc,RECT *rect)
{
	//DrawExcelFocusRect(dc,rect);
	
	rect->bottom --;
	rect->right --;	
	dc->DrawFocusRect(rect);
}*/

/////////////////////////////////////////////////////////////////////////////
//	OnGetDefBackColor
//		Sent when the area behind the grid needs to be painted.
//	Params:
//		section - Id of the grid section that requested this color
//				  possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING, UG_GRID
//	Return:
//		RGB value representing the color of choice
COLORREF MyPGCug::OnGetDefBackColor(int section)
{
	UNREFERENCED_PARAMETER(section);
	return RGB(255,255,255);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetFocus
//		Sent when the grid is gaining focus.
//	Note:
//		The grid will loose focus when the edit is started, or drop list shown
//	Params:
//		section - Id of the grid section gaining focus, usually UG_GRID
//				  possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING, UG_GRID
//	Return:
//		<none>
//
void MyPGCug::OnSetFocus(int section)
{
	UNREFERENCED_PARAMETER(section);
}

/////////////////////////////////////////////////////////////////////////////
//	OnKillFocus
//		Sent when the grid is loosing focus.
//	Params:
//		section - Id of the grid section loosing focus, usually UG_GRID
//				  possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING, UG_GRID
//		pNewWnd	- pointer to the window that is gaining focus
//	Return:
//		<none>
void MyPGCug::OnKillFocus(int section, CWnd *pNewWnd)
{
	UNREFERENCED_PARAMETER(section);
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapStart
//		Called to inform the grid that the col swap was started on given col.
//	Params:
//		col - identifies the column
//	Return:
//		TRUE - to allow for the swap to take place
//		FALSE - to disallow the swap
BOOL MyPGCug::OnColSwapStart(int col)
{
	UNREFERENCED_PARAMETER(col);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanColSwap
//		Called when col swap is about to take place, here you can 'lock' certain
//		cols in place so that are not swappable.
//	Params:
//		fromCol - where the col originated from
//		toCol	- where the col will be located if the swap is allowed
//	Return:
//		TRUE - to allow for the swap to take place
//		FALSE - to disallow the swap
BOOL MyPGCug::OnCanColSwap(int fromCol,int toCol)
{
	UNREFERENCED_PARAMETER(fromCol);
	UNREFERENCED_PARAMETER(toCol);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapped
//		Called just after column-swap operation was completed.
//	Params:
//		fromCol - where the col originated from
//		toCol	- where the col will be located if the swap is allowed
//	Return:
//		<none>
void MyPGCug::OnColSwapped(int fromCol,int toCol)
{
	UNREFERENCED_PARAMETER(fromCol);
	UNREFERENCED_PARAMETER(toCol);
}

/////////////////////////////////////////////////////////////////////////////
//	OnTrackingWindowMoved
//		Called to notify the grid that the tracking window was moved
//	Params:
//		origRect	- from
//		newRect		- to location and size of the track window
//	Return:
//		<none>
int MyPGCug::GetColorLine(int line) 
{
	int color=0;
	switch (line-1%11)
	{
		case 0:
		color=MFC_Util::GetProfileIntNo( "ColorLine1", 0x0000ff);
		break;
		case 1:
		color=MFC_Util::GetProfileIntNo( "ColorLine2",0x00cc00 );
		break;
		case 2:
		color=MFC_Util::GetProfileIntNo( "ColorLine3", 0xff0000);
		break;
		case 3:
		color=MFC_Util::GetProfileIntNo( "ColorLine4", 0xcc0000);
		break;
		case 4:
		color=MFC_Util::GetProfileIntNo( "ColorLine5", 0x00aa00);
		break;
		case 5:
		color=MFC_Util::GetProfileIntNo( "ColorLine6", 0xA52A2A);
		break;
		case 6:
		color=MFC_Util::GetProfileIntNo( "ColorLine7", 0x8B008B);
		break;
		case 7:
		color=MFC_Util::GetProfileIntNo( "ColorLine8", 0x8B0000);
		break;
		case 8:
		color=MFC_Util::GetProfileIntNo( "ColorLine9", 0x87CEFA);
		break;
		case 9:
		color=MFC_Util::GetProfileIntNo( "ColorLine10", 0xFFFFE0);
		break;
		case 10:
		color=MFC_Util::GetProfileIntNo( "ColorLine11", 0xbbccdd);
		break;
	}
	if(color==0) 
		return RGB(240,240,240);
	else
		return color;
}

void MyPGCug::OnTrackingWindowMoved(RECT *origRect,RECT *newRect)
{
	UNREFERENCED_PARAMETER(*origRect);
	UNREFERENCED_PARAMETER(*newRect);
}

void MyPGCug::TimeSerieChartClick(long col, long row, long isheet, void *Ptr, int Index, int VariableType, CString Type, int ValFileNo)
{

	/*	CUGCell cell, cell1, cell2; 
		CString Group, Name;
		int PGindex, itype, isubchart;
		CString fileName;
		CCoupModelDoc *pDocPointer;
		int	Isubchart;*/

}

/////////////////////////////////////////////////////////////////////////////
//	OnDrawFocusRect
void MyPGCug::OnDrawFocusRect(CDC *dc,RECT *rect)
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
		rect->bottom --;
		rect->right --;
		DrawExcelFocusRect( dc, rect);
	}
}

void MyPGCug::DrawExcelFocusRect(CDC *dc,RECT *rect)
{
	BOOL innerRectOnly = FALSE;
	BOOL hideTop = FALSE;
	BOOL hideLeft = FALSE;

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
				hideLeft = TRUE;
			}
			if(startRow >= m_GI->m_numLockRows && startRow < m_GI->m_topRow)
			{
				//hide the top
				hideTop = TRUE;
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
			innerRectOnly = TRUE;
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


void MyPGCug::UpdatePlotStatusBar(int ichoice, BOOL update) 
{
	
	
}




