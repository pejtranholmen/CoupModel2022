/////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived DB_BASCug

#include "..\..\stdafx.h"
#include "..\..\Util.h"
#include "..\..\resource.h"
#include "..\..\ModelTypes\NewModelType.h"
#include "DB_BASCug.h"
#include "..\..\SP\SP_Retrieve.h"
#include "..\ChartProperties\ChartCug.h"
#include "..\..\NewBase\Doc.h"
#include ".\SoilData.h"
#include "..\..\DB\DB_Header.h"
#include ".\DBDataCUG.h"
//#include "..\PlotPF\PlotPF\PFCurve.h"
//#include "..\ParameterPlotDlg.h"




/////////////////////////////////////////////////////////////////////////////
// DB_BASCug construction/destruction
DB_BASCug::DB_BASCug()
{
//	UGXPThemes::UseThemes(true);
	m_bDrawExcelFocus = FALSE;
	m_col=1;
	m_row=1;
	m_DataBase=TRUE;
	m_CompleteList=FALSE;
	m_pCTab=nullptr;
	m_pDB2=nullptr;
	m_SelOne=true;
	m_ColorChanged=RGB(255,200,200);
	m_ColorHeading=RGB(230,230,230);
	m_ColorClick=RGB(240,240,50);
	m_PrevClickRow=-1;
	m_PreviousRow = -1;
	m_DataImported=false;
	
	SetMultiSelectMode ( FALSE );
	DragDropTarget ( FALSE );
		//numcol=pEDIT->m_arrIdKey.GetSize();_pCUG=nullptr;
	m_LatestSheet=0;
	m_OneAction=true;
	m_pDB=MySingle::instance().GetDB_HeaderPointer();
	m_pDBDataCug=MySingle::instance().GetDBDataPointer();
	m_SortOption=0;
	m_SavedMade=false;


}

DB_BASCug::~DB_BASCug()
{
	//UGXPThemes::CleanUp();
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid


void DB_BASCug::OnSetup()
{
	CUGCell cell; 
	CString str;
	CString fstring;
	m_OneAction=false;
	EnableMenu(TRUE); 
	m_pDBDataCug=MySingle::instance().GetDBDataPointer();


	int num_sheets=GetNumberSheets();
	for (long i=0;i<num_sheets;i++) {
		DeleteTab(i);
	}
	SetDefColWidth(130);
	AddTab("Plant", 0); 
	AddTab("Soil Evaporation", 1); 
	AddTab("Snow", 2); 
	AddTab("Soil Frost", 3); 
	AddTab("Soil Water Flows", 4);
	AddTab("Plant Growth", 5); 
	AddTab("Site Management", 6); 
	AddTab("Soil Organic", 7);
	AddTab("Soil Mineral", 8); 
	SetTabWidth(480); 
	SetSheetNumber(m_LatestSheet); 
	//OnSheetSetup(m_LatestSheet);
	SetCurrentTab(m_LatestSheet);
	SetNumberSheets(6); 


	m_pDBDataCug->OnSetup();
}

/////////////////////////////////////////////////////////////////////////////
//	OnSheetSetup	
void DB_BASCug::OnSheetSetup(int sheetNumber)
{
		EnableMenu(TRUE); 
		SetSheetNumber(sheetNumber);

		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		m_bDrawExcelFocus = FALSE;
		m_LatestSheet=sheetNumber;
		SetMultiSelectMode ( TRUE );

//		DragDropTarget ( TRUE );

		MakeDBHeaderSheet(sheetNumber);
	
}
void DB_BASCug::MakeDBHeaderSheet(int sheet) {
	CUGCell cell; 
	CString str;
	CString fstring;

	int cols = m_col,
		rows = m_row;
	int Count;
	size_t Record;
	Count=0;
	SetTH_NumberRows(2); 
	SetTH_Height(50); 
	SetTH_RowHeight(-1,25);
	SetTH_RowHeight(-2,25);
	DB_Header *pHeader;
	pHeader=MySingle::instance().GetDB_HeaderPointer();
	pHeader->Ini_DBBase();
	SetSheetNumber(sheet);
	UseHints(TRUE);

	m_row=pHeader->GetNumOfRec(sheet);

			SetNumberCols ( 9 );

			SetNumberRows (m_row);
			JoinCells(0,-2,8,-2);
			if (m_pDB->IsDBModified(sheet)) {
				QuickSetText(0, -2, "Click to Save Changes made to data base");
				QuickSetBackColor(0, -2, m_ColorClick);
			}
			else if (m_SavedMade) {
				QuickSetText(0, -2, "All changes made are saved");
				QuickSetBackColor(0, -2, m_ColorHeading);
			}
			else {
				QuickSetText(0, -2, "No change");
				QuickSetBackColor(0, -2, m_ColorChanged);
			
			}
			QuickSetText(0,-1,"Type");
			QuickSetText(1,-1,"Name");
			QuickSetText(2,-1,"Site Name");
			QuickSetText(3,-1,"Latitude");
			QuickSetText(4, -1, "Longitude");
			QuickSetText(5,-1,"Country");
			QuickSetText(6, -1, "Created");
			QuickSetText(7,-1,"Key");
			QuickSetText(8, -1, "Compare");
			SetColWidth(-1,0);
			SetColWidth(0,145);
			SetColWidth(1,100);
			SetColWidth(2,110);
			SetColWidth(3,85);
			SetColWidth(4,85);
			SetColWidth(5,50);
			SetColWidth(6,150);
			SetColWidth(7, 70);
			SetColWidth(8, 70);
			DB_OldHeader header;
			DB_NewHeader headernew;

			vector <size_t> sortindex;
			sortindex=GetSortIndex(sheet);
			for(size_t i=0; i<m_row; i++) {
				size_t index=sortindex[i];
				header=pHeader->GetHeader(sheet, index);
				headernew = pHeader->GetNewHeader(sheet, index);
				//QuickSetNumber(0, i, double(header.Type))
				QuickSetNumber(-1,i,index);
				QuickSetText(0, i, pHeader->GetTypeString(sheet, header.Type));
				QuickSetText(1, i, header.Name);
				QuickSetText(2, i, header.SiteName);
				QuickSetText(3, i, header.Latitude);
				QuickSetText(4, i, header.Longitude);
				QuickSetText(5, i, header.Country);
				QuickSetText(6, i, headernew.ModifiedDate);
				QuickSetText(7, i, pHeader->GetKeyString(header.IdUser,header.IdNum));
				GetCell(8, i, &cell);
				cell.SetCellType(UGCT_CHECKBOX);
				if (headernew.CompareGroup)
					cell.SetBool(TRUE);
				else
					cell.SetBool(FALSE);
				if (m_pDB->GetCurrentKeyIndex() == index) {
					m_pDB->SetCurrentKeyLink(sheet, index);
					cell.SetBool(TRUE);
				}
				SetCell(8, i, &cell);
			}
			m_pDBDataCug->Make_Sheet();
			RedrawAll();
	
			CString key, strline;



}

/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the current cell in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int DB_BASCug::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the top row or left column in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int DB_BASCug::OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	
void DB_BASCug::OnHitBottom(long numrows,long rowspast,long rowsfound)
{
}

/////////////////////////////////////////////////////////////////////////////
//	
void DB_BASCug::OnHitTop(long numrows,long rowspast)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeCol
//		Sent when the user is over a separation line on the top heading
//		A return value of TRUE allows the possibiliy of a resize
int DB_BASCug::OnCanSizeCol(int col)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSizing
//		Sent when the user is sizing a column
//		The column that is being sized is given as 
//		well as the width. Plus the width can be modified 
//		at this point. This makes it easy to set min and
//		max widths
void DB_BASCug::OnColSizing(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSized
//		This is sent when the user finished sizing the 
//		given column (see above for more details)
void DB_BASCug::OnColSized(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeRow
//		Sent when the user is over a separation line on the side heading
//		A return value of TRUE allows the possibiliy of a resize
int  DB_BASCug::OnCanSizeRow(long row)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSizing
//	    Sent when the user is sizing a row The row that is being sized is given as 
//		well as the height. Plus the height can be modified 
//		at this point. This makes it easy to set min and max heights
void DB_BASCug::OnRowSizing(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSized
//		This is sent when the user is finished sizing hte
//		given row ( see above for more details)
void DB_BASCug::OnRowSized(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeSideHdg
//		This is sent when the user moves into position 
//		for sizing the width of the side heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int DB_BASCug::OnCanSizeSideHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeTopHdg
//		This is sent when the user moves into position 
//		for sizing the height of the top heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int DB_BASCug::OnCanSizeTopHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSizing
int DB_BASCug::OnSideHdgSizing(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSized
int DB_BASCug::OnSideHdgSized(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSized
int DB_BASCug::OnTopHdgSized(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSizing
int DB_BASCug::OnTopHdgSizing(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColChange
//		Sent whenever the current column changes
//		The old and the new columns are given
void DB_BASCug::OnColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowChange
//		Sent whenever the current row changes
//		The old and the new rows are given
void DB_BASCug::OnRowChange(long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellChange
//		Sent whenever the current cell changes rows or columns
void DB_BASCug::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnLeftColChange
//		Sent whenever the left visible column in the grid changes
void DB_BASCug::OnLeftColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopRowChange
//		Sent whenever the top visible row in the grid changes
void DB_BASCug::OnTopRowChange(long oldrow,long newrow)
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
void DB_BASCug::OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum)
{
	TRACE("DB_BASCug::OnSelectionChanged called with startCol = %ld, startRow = %ld, endCol = %ld, endRow = %ld, blockNum = %ld.\n",
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
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DB_BASCug::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed) {

	CUGCell cell; 
	CString str;
	if(updn) {
		size_t isheet = GetSheetNumber();
		size_t db_index = m_pDB->GetCurrentDBIndex();
		GetCell(-1, row, &cell);
		//cell.GetText();
		size_t index = size_t(cell.GetNumber());
		GetCell(8, row, &cell);

		if (col == 8) {
			GetCell(8, row, &cell);
			bool koll = cell.GetBool();
			if (koll) {
				m_pDB->SetCompareLinkNewHeader(isheet, index, true);
				m_pDB->SetCurrentKeyIndex(index);
			}
			else {
				m_pDB->SetCompareLinkNewHeader(isheet, index, false);
				vector<size_t> comp = m_pDB->GetCompareLinkVector(db_index);
				if (comp.size() > 1)
					m_pDB->SetCurrentKeyIndex(comp[comp.size() - 1]);
			}
			m_pDBDataCug->Make_Sheet();
		}
		else {
			if (col == 6 && m_pDB != nullptr) {

				CDB *pDB = m_pDoc->DB_GetPointer(DB_types(db_index));
				size_t dest = pDB->GetLastSelectedRepetition() + 1;
				if (dest > 1) {
					SelectDB_Object_ToMultiRunSim(row);
					//cstring->Format(_T("Col:%d Row:%ld",col,row);
					return;

				}
			}



			//m_pDB->SetCurrentKeyLink(isheet, index);
			m_pDBDataCug->Make_Sheet();

		}
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
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DB_BASCug::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
	
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
//		Sent whenever the user double clicks the left mouse button within the grid
//	
//		'col' and 'row' are negative if the area clicked in is not a valid cell
//		'rect' the rectangle of the cell that was clicked in
//		'point' the point where the mouse was clicked
void DB_BASCug::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	StartEdit();
}				 

/////////////////////////////////////////////////////////////////////////////
//	OnMouseMove
void DB_BASCug::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user clicks the left mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DB_BASCug::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if(row==-2) {

		if(m_pDB->WriteHeaderFile(m_pDBDataCug->GetDataBaseId())) {
			m_pDB->SetSavedDB(m_pDBDataCug->GetDataBaseId());
			m_SavedMade=true;
			OnSheetSetup(m_pDBDataCug->GetDataBaseId());
		}

	}
	else {
		m_SortOption=col;
		OnSheetSetup(m_pDBDataCug->GetDataBaseId());
	}
				//SortAndJoin(col, isheet);
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_RClicked
//		Sent whenever the user clicks the right mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DB_BASCug::OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{

}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top heading
//	
//		'col' is negative if the area clicked in is not valid
void DB_BASCug::OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_LClicked
//		Sent whenever the user clicks the left mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DB_BASCug::OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{

	size_t db_index=m_pDB->GetCurrentDBIndex();
	if(m_pDB->IsDBModified(db_index)) {
			if(m_pDB->WriteHeaderFile(db_index)) {

				m_pDB->SetSavedDB(db_index);
				MakeDBHeaderSheet(db_index);

			}


	}


}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_RClicked
//		Sent whenever the user clicks the right mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DB_BASCug::OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{

}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_DClicked1955
//		Sent whenever the user double clicks the left mouse button within the side heading
//	
//		'row' is negative if the area clicked in is not valid
void DB_BASCug::OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{

}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_LClicked
//		Sent whenever the user clicks the left mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DB_BASCug::OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_RClicked
//		Sent whenever the user clicks the right mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DB_BASCug::OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{


}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top corner button
void DB_BASCug::OnCB_DClicked(RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyDown
//		Sent whenever the user types when the grid has focus. The keystroke can be
//		modified here as well. (See WM_KEYDOWN for more information)
void DB_BASCug::OnKeyDown(UINT *vcKey,BOOL processed)
{
	if ( *vcKey == VK_SPACE )
	{
//		m_GI->m_multiSelect->FloatingBlock( 0, 0, TRUE );
	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnCharDown
//		Sent whenever the user types when the grid has focus. The keystroke can be 
//		modified here as well. (See WM_CHAR for more information)
void DB_BASCug::OnCharDown(UINT *vcKey,BOOL processed)
{
}
	
/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent everytime the grid needs to
//		draw a cell in the grid. At this point the cell
//		class has been filled with the information to be
//		used to draw the cell. The information can now be
//		changed before it is used for drawing
void DB_BASCug::OnGetCell(int col,long row,CUGCell *cell)
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
void DB_BASCug::OnSetCell(int col,long row,CUGCell *cell)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDataSourceNotify
//		This message is sent from a data source , message
//		depends on the data source - check the information
//		on the data source(s) being used
//		- The ID of the Data source is also returned
void DB_BASCug::OnDataSourceNotify(int ID,long msg,long param)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellTypeNotify
//		This message is sent from a cell type , message
//		depends on the cell type - check the information
//		on the cell type classes
//		- The ID of the cell type is given
int DB_BASCug::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	CUGCell cell;
	CString m_After;
	if(ID==1 && msg == 103) {
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditStart
//		This message is sent whenever the grid is ready to start editing a cell
//		A return of TRUE allows the editing a return of FALSE stops editing
//		Plus the properties of the CEdit class can be modified
int DB_BASCug::OnEditStart(int col, long row,CWnd **edit)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditVerify
//		This is send when the editing is about to end
int DB_BASCug::OnEditVerify(int col, long row,CWnd *edit,UINT *vcKey)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int DB_BASCug::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR st,BOOL cancelFlag)
{
	CUGCell cell; 
	CString str;
	edit->GetWindowText(str);

	if(m_DataBaseString=="DataSheet") {
		EditOfLayerInfo(col, row, CT2A(str).m_psz);
	}
	else if(m_DataBaseString=="Profile") {
		return EditOfProfileInfo(row, CT2A(str).m_psz);
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int DB_BASCug::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	sections - UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//				UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON


/////////////////////////////////////////////////////////////////////////////
//	OnHint
int DB_BASCug::OnHint(int col,long row,int section,CString *cstring)
{
	if(col==6&&m_pDB!=nullptr) {
		size_t db_index=m_pDB->GetCurrentDBIndex();
		CDB *pDB=m_pDoc->DB_GetPointer(DB_types(db_index));
		size_t dest=pDB->GetLastSelectedRepetition()+1;
		if(dest>1) {
			CUGCell cell;
			GetCell(6,row,&cell);
			string str=cell.GetStdText();
			string dest_str="("; dest_str+=FUtil::STD_ItoAscii(dest+1);dest_str+=")";
			string mes;
			mes="Click to Select properties from "+str+" for "+dest_str+" multirun repetition";
			cstring->SetString(CString(mes.c_str()));
			return TRUE;

		}
	}
	else if(col==0) 
		cstring->Format(_T("Click to show detailed info in DB Data Window"));
	else
		return FALSE;
	return TRUE;


}

/////////////////////////////////////////////////////////////////////////////
//	OnVScrollHint
int DB_BASCug::OnVScrollHint(long row,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHScrollHint
int DB_BASCug::OnHScrollHint(int col,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OLE
#ifdef __AFXOLE_H__

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  DB_BASCug::OnDragEnter(COleDataObject* pDataObject){
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  DB_BASCug::OnDragOver(COleDataObject* pDataObject,int col,long row){
	//return DROPEFFECT_NONE;
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  DB_BASCug::OnDragDrop(COleDataObject* pDataObject,int col,long row){
	return DROPEFFECT_COPY;
}
#endif

/////////////////////////////////////////////////////////////////////////////
//	
void DB_BASCug::OnScreenDCSetup(CDC *dc,int section){
}

/////////////////////////////////////////////////////////////////////////////
//	OnSortEvaluate
//	return		-1  <
//				0	==
//				1	>
int DB_BASCug::OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags)
{
	if(flags&UG_SORT_DESCENDING)
	{
		CUGCell *ptr = cell1;
		cell1 = cell2;
		cell2 = ptr;
	}

//	if(cell1->IsPropertySet(UGCELL_TEXT_SET) == FALSE)
//	{
//		if(cell2->IsPropertySet(UGCELL_TEXT_SET) == FALSE)
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
void DB_BASCug::OnTabSelected(int ID)
{
	SetFocus();
	MakeDBHeaderSheet(ID);
	m_pDBDataCug->OnUpdateDataBase();
	m_pDBDataCug->SetDataBase(ID);
	m_pDB->SetCurrentDBIndex(ID);
	m_pDBDataCug->OnSheetSetup(0);


	

}

/////////////////////////////////////////////////////////////////////////////
//	OnAdjustComponentSizes
void DB_BASCug::OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,
		RECT *cnrBtn,RECT *vScroll,RECT *hScroll,RECT *tabs)
{
} 

/////////////////////////////////////////////////////////////////////////////
//	OnDrawFocusRect
void DB_BASCug::OnDrawFocusRect(CDC *dc,RECT *rect)
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

void DB_BASCug::DrawExcelFocusRect(CDC *dc,RECT *rect)
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

/////////////////////////////////////////////////////////////////////////////
//	OnGetDefBackColor
COLORREF DB_BASCug::OnGetDefBackColor(int section)
{
	return GetSysColor ( COLOR_APPWORKSPACE );
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetFocus
void DB_BASCug::OnSetFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKillFocus
void DB_BASCug::OnKillFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapStart
BOOL DB_BASCug::OnColSwapStart(int col)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanColSwap
BOOL DB_BASCug::OnCanColSwap(int fromCol,int toCol)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTrackingWindowMoved
void DB_BASCug::OnTrackingWindowMoved(RECT *origRect,RECT *newRect)
{
}
BOOL DB_BASCug::SaveToSim()
{
	CUGCell cell; 
	string str;
	if(!m_DataBase&&m_pDB2!=nullptr) {
		for(int i=0;i<m_row;i++) {
				GetCell(0,i,&cell);
				str=cell.GetStdText();
				m_pDB2->MR_SetSelection(i,"Soil",str);

		}
		return TRUE;
	}
	return FALSE;
}


vector<size_t> DB_BASCug::GetSortIndex(int sheet) {
	DB_OldHeader header;
	DB_NewHeader headernew;
	vector<size_t> ut;
		multimap <string, size_t> sortstring;
		multimap <float, size_t> sortpos;
		multimap <size_t, size_t> sortnum;
			for(size_t i=0;i<m_pDB->GetNumOfRec(sheet);i++) {
			header=m_pDB->GetHeader(sheet, i);
			headernew = m_pDB->GetNewHeader(sheet, i);
			//while(p_PFCurve->ScanNewHeaders(&header, ExtraLocalBase)) {
				//if(KeyTest(p_PFCurve->GetProfile(i))) {
					switch (m_SortOption) {
						case 0: 
						case 7:
							sortstring.insert(pair<string,size_t>(m_pDB->GetTypeString(sheet,header.Type),i));
							break;
						case 1:
							sortstring.insert(pair<string,size_t>(header.Name,i));
							break;
						case 2:
							sortstring.insert(pair<string,size_t>(header.SiteName,i));
							break;
						case 3:
							sortstring.insert(pair<string,size_t>(header.Latitude,i));
							break;
						case 4:
							sortstring.insert(pair<string,size_t>(header.Longitude,i));
							break;
						case 5:
							sortstring.insert(pair<string, size_t>(header.Country,i));
							break;
						case 6:
						case 8:
							sortstring.insert(pair<string, size_t>(headernew.ModifiedDate, i));
							break;
					}
				//}
			}

			switch(m_SortOption) {
			case 0: case 1: 
			case 2: case 5:
			case 3: case 4:
			case 6: case 7:
			case 8:
				for(auto it=sortstring.begin();it!=sortstring.end(); it++)
					ut.push_back(it->second);
				break;

			}
			return ut;
}
