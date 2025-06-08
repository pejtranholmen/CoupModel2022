/////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived PFCug

#include "..\..\stdafx.h"
#include "..\..\Util.h"
#include "..\..\resource.h"
#include "..\..\ModelTypes\NewModelType.h"
#include "PFCug.h"
#include "..\..\SP\SP_Retrieve.h"
#include "..\ChartProperties\ChartCug.h"
#include "..\..\NewBase\Doc.h"
#include ".\SoilData.h"
//#include "..\..\SoilData\GetSoilStorage.h"
//#include "..\PlotPF\PlotPF\PFCurve.h"
//#include "..\ParameterPlotDlg.h"







/////////////////////////////////////////////////////////////////////////////
// PFCug construction/destruction
PFCug::PFCug()
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
	m_DataImported=false;
	p_PFCurve=&PlotpF::GetPointer();




	IniDataBase();

	if (p_PFCurve->IsOpen()) {
		if (p_PFCurve->IsProfile()) {
			m_IniProf = p_PFCurve->m_ActProfile.KeyProf / 100;
			m_IniRepProf = p_PFCurve->m_ActProfile.KeyProf % 100;
		}
		else
			m_IniProf = -10;
	}

	SetMultiSelectMode ( TRUE );
	DragDropTarget ( TRUE );
	m_pCUG=nullptr;
	m_LatestSheet=0;
	m_OneAction=true;
	m_PF_EstimateMin=10.;
	m_PF_EstimateMax=10000.;
	m_SortOption=0;

}

PFCug::~PFCug()
{
	//UGXPThemes::CleanUp();
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid


void PFCug::OnSetup()
{
	CUGCell cell; 
	CString str;
	CString fstring;
	m_OneAction=false;

		int num_sheets=GetNumberSheets();
		for (long i=0;i<num_sheets;i++) {
			DeleteTab(i);
		}
		SetDefColWidth(130);
		AddTab("Main Data Base", 0); 
		AddTab("Extra Data Base", 1); 
		AddTab("Selected Profile",2);

		SetTabWidth(480); 
		SetNumberSheets(3);
		SetSheetNumber(m_LatestSheet); 
		OnSheetSetup(m_LatestSheet);
		SetCurrentTab(m_LatestSheet);




}

/////////////////////////////////////////////////////////////////////////////
//	OnSheetSetup	
void PFCug::OnSheetSetup(int sheetNumber)
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

		switch(sheetNumber)		{
		case (0): 
			MakeBaseSheet(false);
			RedrawAll();
			break;
		case (1): 
			MakeBaseSheet(true,false);
			RedrawAll();
			break;
		case (2): 
			MakeBaseSheet(false,true);
			RedrawAll();
			break;
		}

}

/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the current cell in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int PFCug::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the top row or left column in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int PFCug::OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	
void PFCug::OnHitBottom(long numrows,long rowspast,long rowsfound)
{
}

/////////////////////////////////////////////////////////////////////////////
//	
void PFCug::OnHitTop(long numrows,long rowspast)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeCol
//		Sent when the user is over a separation line on the top heading
//		A return value of TRUE allows the possibiliy of a resize
int PFCug::OnCanSizeCol(int col)
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
void PFCug::OnColSizing(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSized
//		This is sent when the user finished sizing the 
//		given column (see above for more details)
void PFCug::OnColSized(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeRow
//		Sent when the user is over a separation line on the side heading
//		A return value of TRUE allows the possibiliy of a resize
int  PFCug::OnCanSizeRow(long row)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSizing
//	    Sent when the user is sizing a row The row that is being sized is given as 
//		well as the height. Plus the height can be modified 
//		at this point. This makes it easy to set min and max heights
void PFCug::OnRowSizing(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSized
//		This is sent when the user is finished sizing hte
//		given row ( see above for more details)
void PFCug::OnRowSized(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeSideHdg
//		This is sent when the user moves into position 
//		for sizing the width of the side heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int PFCug::OnCanSizeSideHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeTopHdg
//		This is sent when the user moves into position 
//		for sizing the height of the top heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int PFCug::OnCanSizeTopHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSizing
int PFCug::OnSideHdgSizing(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSized
int PFCug::OnSideHdgSized(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSized
int PFCug::OnTopHdgSized(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSizing
int PFCug::OnTopHdgSizing(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColChange
//		Sent whenever the current column changes
//		The old and the new columns are given
void PFCug::OnColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowChange
//		Sent whenever the current row changes
//		The old and the new rows are given
void PFCug::OnRowChange(long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellChange
//		Sent whenever the current cell changes rows or columns
void PFCug::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnLeftColChange
//		Sent whenever the left visible column in the grid changes
void PFCug::OnLeftColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopRowChange
//		Sent whenever the top visible row in the grid changes
void PFCug::OnTopRowChange(long oldrow,long newrow)
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
void PFCug::OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum)
{
	TRACE("PFCug::OnSelectionChanged called with startCol = %ld, startRow = %ld, endCol = %ld, endRow = %ld, blockNum = %ld.\n",
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
void PFCug::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
	CUGCell cell; 
	string str;
	//StartDragDrop();

	GetCell(0,row,&cell);
	if(m_DataBaseString=="DataSheet") return;

		if(cell.GetBackColor()==m_ColorClick) {
			if(m_PrevClickRow>=0) {
				QuickSetBackColor(0,m_PrevClickRow,m_ColorClick);
				QuickSetBackColor(1,m_PrevClickRow,m_ColorClick);
			}
			RedrawCell(0,m_PrevClickRow);
			RedrawCell(1,m_PrevClickRow);
			m_PrevClickRow=row;
			cell.SetBackColor(m_ColorChanged);	
		}
		//else if(cell.GetBackColor()==m_ColorChanged) {
		//	cell.SetBackColor(m_ColorClick);	
		//}
		SetCell(0,row,&cell);
		RedrawCell(0,row); 
		str=cell.GetStdText();
		int prof=FUtil::AtoInt(str);
		
		GetCell(1,row,&cell);
		cell.SetBackColor(m_ColorChanged);
		str=cell.GetStdText();
		SetCell(1,row,&cell);
		RedrawCell(1,row);
		int repNumber=FUtil::AtoInt(str);
		if(m_DataBaseString=="Kalle"&&m_DataBase) {
			if(m_pDB2!=nullptr) {
				if(m_pDB2->MR_GetNumberOfSelections()>=m_pCUG->m_row) 
					return;
			}
			if(col!=5) return;
			GetCell(5,row,&cell);

			if(cell.GetBackColor()==m_ColorClick) {

				cell.SetBackColor(m_ColorChanged);	
			}
				//else if(cell.GetBackColor()==m_ColorChanged) {
			//	cell.SetBackColor(m_ColorClick);	
			//}
			SetCell(5,row,&cell);
			RedrawCell(5,row); 


			if(m_DataBase) {
				GetCell(5,row,&cell);
				str=cell.GetStdText();

				if(m_pDB2==nullptr) {

				}
				else if(m_pCUG!=nullptr) {
					if(m_pDB2->MR_GetNumberOfSelections()<m_pCUG->m_row) {
						m_pDB2->MR_AddSelection(m_pDB2->GetName(),str);
						if(m_pCUG!=nullptr)
							m_pCUG->OnSetup();
					}
				}
			}

			return;
		}
		if(m_DataBaseString=="Kalle") return; // Kalle = Called from PP_EDIT
		if(m_SelOne) {
			//CString std::string=_T("");
			//for(int i=0;i<6;i++)
			//	string+=PLOTPF_mp_DKEY[i];
			//((SP_Retrieve*)m_pSP_Retrieve_Dlg)->OnEnChangeEditComments();

			int layer=1;
			
			//*m_pRepProf=repNumber;
			//*m_pProf=prof;

			GetCell(1,row,&cell);
			repNumber=int(cell.GetNumber());
			p_PFCurve->SetTopRow(GetTopRow());
			
			//PLOTPF_mp_GETLAYER(&prof, &repNumber, &layer);
			if(prof>0&&repNumber>0) {
				if(p_PFCurve->SetHeader(prof, repNumber)) {
				NEWHEADER test=p_PFCurve->m_ActProfile;
		
				};
				m_DataImported=false;
				UpdateLinkToProfile();
				//m_pCUG->OnSetup();
				//m_pSoilData->OnSetup();
				//((SP_Retrieve*)m_pSP_Retrieve_Dlg)->SetComments("");
			//	pEDIT->m_Details="Kalle";

			}

		}
		else if(m_pCUG!=nullptr) {
			if(m_pDB2!=nullptr&&m_pDB2->MR_GetNumberOfSelections()<m_pCUG->m_row) {
				m_pDB2->MR_AddSelection("Soil",str);
				if(m_pCUG!=nullptr)
					m_pCUG->OnSetup();
			}
		}
		

//if ( !updn )
	/*{
		int stCol, edCol;
		long stRow, edRow;

		m_GI->m_multiSelect->GetTotalRange( &stCol, &stRow, &edCol, &edRow );

		if ( stCol == edCol && stRow == edRow )
			StartEdit();
	}*/

/*	if ( !updn )
	{
		int stCol, edCol;
		long stRow, edRow;

		m_GI->m_multiSelect->EnumNextBlock ( &stCol, &stRow, &edCol, &edRow );
		TRACE ( "%d, %d - %d, %d\n", stCol, stRow, edCol, edRow );
	}*/
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
void PFCug::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
	
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
//		Sent whenever the user double clicks the left mouse button within the grid
//	
//		'col' and 'row' are negative if the area clicked in is not a valid cell
//		'rect' the rectangle of the cell that was clicked in
//		'point' the point where the mouse was clicked
void PFCug::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	StartEdit();
}				 

/////////////////////////////////////////////////////////////////////////////
//	OnMouseMove
void PFCug::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user clicks the left mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PFCug::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{

	if(m_DataBaseString=="Kalle"&&m_DataBase&&col==0) {
		if ( !updn )
		{
			SelectRange ( col, 0, col, GetNumberRows()-1 );

		}
		if(updn)
			SortBy(col, UG_SORT_ASCENDING); 
	}
	m_SortOption=col;
	MakeBaseSheet();
				//SortAndJoin(col, isheet);
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_RClicked
//		Sent whenever the user clicks the right mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PFCug::OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{

}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top heading
//	
//		'col' is negative if the area clicked in is not valid
void PFCug::OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_LClicked
//		Sent whenever the user clicks the left mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PFCug::OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if(m_DataBaseString!="DataSheet") return;
	if ( !updn ) {
		SelectRange ( 0, row, GetNumberCols()-1, row );

	}
	SimB *pSimB;
	Tab *pTab;
	int sheet=GetSheetNumber();
	switch (sheet) {
		case 0:
			case 1:
			pSimB=m_pDoc->GetPtr(simtype::PAR_TABLE, string("Air Entry"));
			pTab=(Tab*)m_pDoc->GetPtr(simtype::TABLE, string("Brooks-Corey, water retention, measured horizons"));
			MakeChart(row, pSimB, 0);


		break;
		case 2:
			pSimB=m_pDoc->GetPtr(simtype::PAR_TABLE, string("Total Conductivity"));
			pTab=(Tab*)m_pDoc->GetPtr(simtype::TABLE, string("Hydraulic conductivity, measured horizons"));
			MakeChart(row, pSimB, pTab, 0);

		break;




	}



}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_RClicked
//		Sent whenever the user clicks the right mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PFCug::OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{

}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_DClicked1955
//		Sent whenever the user double clicks the left mouse button within the side heading
//	
//		'row' is negative if the area clicked in is not valid
void PFCug::OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{

}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_LClicked
//		Sent whenever the user clicks the left mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PFCug::OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_RClicked
//		Sent whenever the user clicks the right mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PFCug::OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{

}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top corner button
void PFCug::OnCB_DClicked(RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyDown
//		Sent whenever the user types when the grid has focus. The keystroke can be
//		modified here as well. (See WM_KEYDOWN for more information)
void PFCug::OnKeyDown(UINT *vcKey,BOOL processed)
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
void PFCug::OnCharDown(UINT *vcKey,BOOL processed)
{
}
	
/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent everytime the grid needs to
//		draw a cell in the grid. At this point the cell
//		class has been filled with the information to be
//		used to draw the cell. The information can now be
//		changed before it is used for drawing
void PFCug::OnGetCell(int col,long row,CUGCell *cell)
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
void PFCug::OnSetCell(int col,long row,CUGCell *cell)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDataSourceNotify
//		This message is sent from a data source , message
//		depends on the data source - check the information
//		on the data source(s) being used
//		- The ID of the Data source is also returned
void PFCug::OnDataSourceNotify(int ID,long msg,long param)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellTypeNotify
//		This message is sent from a cell type , message
//		depends on the cell type - check the information
//		on the cell type classes
//		- The ID of the cell type is given
int PFCug::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	CUGCell cell;
	CString m_After;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditStart
//		This message is sent whenever the grid is ready to start editing a cell
//		A return of TRUE allows the editing a return of FALSE stops editing
//		Plus the properties of the CEdit class can be modified
int PFCug::OnEditStart(int col, long row,CWnd **edit)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditVerify
//		This is send when the editing is about to end
int PFCug::OnEditVerify(int col, long row,CWnd *edit,UINT *vcKey)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int PFCug::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR st,BOOL cancelFlag)
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
int PFCug::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	sections - UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//				UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON


/////////////////////////////////////////////////////////////////////////////
//	OnHint
int PFCug::OnHint(int col,long row,int section,CString *str1)
{
	CUGCell cell;
	long ParV;
	UNREFERENCED_PARAMETER(section);
	int isheet = GetSheetNumber();
	if (isheet == 0 || isheet >= 8) {
		GetCell(col, row, &cell);
		ParV = cell.GetParam();
		if (ParV > 0) {
			string text = GetHint(ParV);
			if (text.size() > 0) *str1 = text.c_str();
			return TRUE;
		}
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnVScrollHint
int PFCug::OnVScrollHint(long row,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHScrollHint
int PFCug::OnHScrollHint(int col,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OLE
#ifdef __AFXOLE_H__

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  PFCug::OnDragEnter(COleDataObject* pDataObject){
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  PFCug::OnDragOver(COleDataObject* pDataObject,int col,long row){
	//return DROPEFFECT_NONE;
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  PFCug::OnDragDrop(COleDataObject* pDataObject,int col,long row){
	return DROPEFFECT_COPY;
}
#endif

/////////////////////////////////////////////////////////////////////////////
//	
void PFCug::OnScreenDCSetup(CDC *dc,int section){
}

/////////////////////////////////////////////////////////////////////////////
//	OnSortEvaluate
//	return		-1  <
//				0	==
//				1	>
int PFCug::OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags)
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
void PFCug::OnTabSelected(int ID)
{
	if(!m_DataBase&&!m_OneAction) {
		/*m_pCUG->OneAction(true);
		m_pCUG->OnTabSelected(ID);
		m_pCUG->OneAction(false);*/

		if(GetNumberSheets()>ID)
			OnSheetSetup(ID);

	}
	else if(m_DataBaseString!="DataSheet"&&m_DataBase&&!m_OneAction) {
/*		m_pCUG->OneAction(true);
		m_pCUG->OnTabSelected(ID);
		m_pCUG->OneAction(false);*/
		if(GetNumberSheets()>ID)
			OnSheetSetup(ID);


	}
	else {
		//if(!m_OneAction)
		//	OnSheetSetup(ID);
	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnAdjustComponentSizes
void PFCug::OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,
		RECT *cnrBtn,RECT *vScroll,RECT *hScroll,RECT *tabs)
{
} 

/////////////////////////////////////////////////////////////////////////////
//	OnDrawFocusRect
void PFCug::OnDrawFocusRect(CDC *dc,RECT *rect)
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

void PFCug::DrawExcelFocusRect(CDC *dc,RECT *rect)
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
COLORREF PFCug::OnGetDefBackColor(int section)
{
	return GetSysColor ( COLOR_APPWORKSPACE );
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetFocus
void PFCug::OnSetFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKillFocus
void PFCug::OnKillFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapStart
BOOL PFCug::OnColSwapStart(int col)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanColSwap
BOOL PFCug::OnCanColSwap(int fromCol,int toCol)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTrackingWindowMoved
void PFCug::OnTrackingWindowMoved(RECT *origRect,RECT *newRect)
{
}
BOOL PFCug::SaveToSim()
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
bool PFCug::IniDataBase() {

	if(!p_PFCurve->IsOpen()) {
	  std::string directory;
	  directory=MFC_Util::GetProfileStringStd("DataBaseDirectory", directory);
	 
	 if(!p_PFCurve->OpenMainFile(directory+"PfProf")) {
		 return false;
	 }
	 p_PFCurve->SetDataBaseDirectory(directory);
	 auto koll = p_PFCurve->GetDataBaseDirectory();
	 }
	if(p_PFCurve->NewFormatFile()) {
		auto koll = p_PFCurve->GetDataBaseDirectory();
		p_PFCurve->ReadHeaderFile();
		
		m_ActHeader=p_PFCurve->GetHeader(0);
		p_PFCurve->SetHeader(0);
		return true;
		//m_ActHeader=p_PFCurve->GetHeader(p_PFCurve->GetInsertRecord(0));
	}
	else if(!p_PFCurve->NewFormatFile()) {
		p_PFCurve->OpenEditMainDataBase();
		p_PFCurve->ReadHeaderFile();
		p_PFCurve->SaveNewFormat();
		p_PFCurve->CloseMainDataBase();
		 std::string directory;
		directory=MFC_Util::GetProfileStringStd("DataBaseDirectory", directory);
		p_PFCurve->OpenMainFile(directory+"PfProf");
		if(size_t(p_PFCurve->GetInsertRecord(0))== std::string::npos) return false;
		m_ActHeader=p_PFCurve->GetHeader(p_PFCurve->GetInsertRecord(0));
	}
	if(m_ActHeader.RecHeader>0) {
	   m_IniProf=m_ActHeader.KeyProf/100;
	   m_IniRepProf=m_ActHeader.KeyProf%100;
	   return true;
	}
	else
		m_IniProf=-10;

	return false;

}


