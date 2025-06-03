/////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived SimpleCug

#include "..\stdafx.h"
#include "..\Util.h"
#include "..\resource.h"
#include "SimpleCug.h"
#include "..\MultiRun\MultiRunTable.h"
//#include "..\ParameterPlotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(SimpleCug,CUGCtrl)
	//{{AFX_MSG_MAP(SimpleCug)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// SimpleCug construction/destruction
SimpleCug::SimpleCug()
{
//	UGXPThemes::UseThemes(true);
	m_bDrawExcelFocus = TRUE;
	m_col=1;
	m_row=1;
	m_Data=FALSE;
	m_pCTab=nullptr;
	m_ColorChanged=RGB(255,200,200);
	m_ColorHeading=RGB(230,230,230);
	m_ColorClick=RGB(240,240,50);

	SetMultiSelectMode ( TRUE );
	DragDropTarget ( TRUE );
	m_MultiTable=FALSE;
	m_toprow=0;
	m_ShowAll=FALSE;
}

SimpleCug::~SimpleCug()
{
	//UGXPThemes::CleanUp();
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid

void SimpleCug::OnUpdate(int col, int row, BOOL data, Tab * pTab, CDialog *pPlot)
{
	CUGCell cell; 
	m_col=col;
	m_row=row;
	m_Data=data;
	m_pCTab=pTab;
	CDialog *pp=pPlot;
	//pp->OnInitDialog();
	if(pTab!=nullptr)
		m_row=pTab->GetNumOfRow();


/*ableUpdate(FALSE); 
	while(GetNumberRows()>0)
		DeleteRow(0);
	EnableUpdate(TRUE); 
	OnSetup();*/

}
void SimpleCug::SetMultiTable(int row, SimB *pPar, int tabindex, Doc *pDoc , CDialog *pDlg, int *numvalid, BOOL showall)
{
	CUGCell cell; 
	m_col=1;
	m_row=row;
	m_Data=FALSE;
	m_pCTab=nullptr;
	m_MultiTable=TRUE;
	m_pDoc=pDoc;
	m_TabIndex=tabindex;
	m_pType=1;
	if(tabindex<0) m_pType=0;
	m_SIMB.pBase=pPar;
	m_SIMB.TabIndex=tabindex;
	SIMB PAR;
	P* p=dynamic_cast<P*>(m_SIMB.pBase);
	if(p==nullptr) {
		Ps* pPs=dynamic_cast<Ps*>(m_SIMB.pBase);///n_lines=m_spCP->MR_GetSizeTable();	
		m_ActNumValues=pPs->MR_GetSizeTable();
	}
	else 
		m_ActNumValues=p->MR_GetSizeTable(m_TabIndex);
	m_pDlg=pDlg;
	m_NumValid=numvalid;
	if(showall) {
		int ind=0;
		m_PtrArray.clear();
		//m_TabIndexArray.RemoveAll();
		for(size_t i=0;i<m_pDoc->MR_GetNumberOfDimensions();i++){
					for(size_t ii=0; ii<m_pDoc->MR_Get_NumberOfParametersWithinDim(i+1);ii++){
						PAR=m_pDoc->MR_Get_PAR(i+1,ii+1);
						if(m_pDoc->MR_GetMethod(i+1,ii+1)==4) {
							m_PtrArray.push_back(PAR);
							ind++;
						}
					}
			}
		m_col=ind;
		m_ShowAll=TRUE;
		m_MultiTable=TRUE;
	}

}
BOOL SimpleCug::OnRead()
{

	CUGCell cell;

	return TRUE;

}

BOOL SimpleCug::OnWrite()
{

	return TRUE;

}
void SimpleCug::OnSetup()
{
	CUGCell cell; 
	string str;

	SIMB PAR;
	int mainindex;
	int cols = m_col,
		rows = m_row;

	SetUniformRowHeight(TRUE);

	EnableUpdate(FALSE); 
	while(GetNumberRows()>0)
			DeleteRow(0);
	EnableUpdate(TRUE);

	if(!m_ShowAll) {
		m_PtrArray.clear();
		PAR=m_SIMB;
		m_PtrArray.push_back(PAR);

	}

	SetNumberCols ( cols );
	SetNumberRows ( rows );
	SetColWidth(-1,0);
	if(m_MultiTable) {
	EnableMenu(TRUE); 
	for(int icol=0;icol<cols;icol++){
        SetColWidth(-1,40);
		SetColWidth(icol,86);
		PAR=m_PtrArray[icol];
		
		//TabIndex=m_TabIndexArray.GetAt(icol);
		str=PAR.pBase->GetName();
		if(PAR.pBase->Is_PTable())
			str+="("+FUtil::STD_ItoAscii(PAR.TabIndex+1)+")";
		QuickSetText(icol,-1,str);
		if((PAR.pBase==m_SIMB.pBase&&PAR.pBase->Is_PTable())||(PAR.pBase==m_SIMB.pBase&&PAR.pBase->Is_P())) QuickSetBackColor(icol,-1,m_ColorChanged);


		if(PAR.pBase->Is_P()) 
			m_ActNumValues=((Ps*)PAR.pBase)->MR_GetSizeTable();
		else 
			m_ActNumValues=((P*)PAR.pBase)->MR_GetSizeTable(PAR.TabIndex);

		if(m_ActNumValues<m_row) {
			for(int i=m_ActNumValues;i<m_row;i++) {
				if(PAR.pBase->Is_P()) 
					((Ps*)PAR.pBase)->MR_Append_TableValue(MISSING);
				else 
					((P*)PAR.pBase)->MR_Append_TableValue(PAR.TabIndex,MISSING);
			}
		}

		for(int i=0;i<m_row;i++)
			QuickSetNumber(-1,i,i+1);
			int ivalid=0;
			for(int i=0;i<m_ActNumValues;i++) {
				double v;
				Ps* pPs;
				P* pP;
				if(PAR.pBase->Is_P()) {
					pPs=((Ps*)PAR.pBase);
					v=((Ps*)PAR.pBase)->MR_Get_TabValue(i);
					pPs->MR_Set_TabValue(i,v);
					v=pPs->MR_Get_TabValue(i);
				}
				else {
					pP=((P*)PAR.pBase);
					v=pP->MR_Get_TabValue(PAR.TabIndex, i);
					pP->MR_Set_TabValue(PAR.TabIndex,i,v);
					v=pP->MR_Get_TabValue(PAR.TabIndex, i);
				}
				if(v<-1.E38)
					QuickSetText(icol,i,"",false);
				else {
					QuickSetNumber(icol,i,v,false);
					ivalid++;
				}
			}
			*m_NumValid=ivalid;
	}

	((MultiRunTable*)m_pDlg)->Update();
		SetVScrollMode ( UG_SCROLLTRACKING );
		SetUniformRowHeight( TRUE );
		return;
	}
	if(m_pCTab==nullptr) return;
	mainindex=m_pCTab->GetMainIndex();
	for (int i=0; i<rows;i++) {
		GetCell(0,i,&cell);
		if(i==mainindex) {
			cell.SetBackColor(RGB(255,100,100));
			str="("+FUtil::STD_ItoAscii(i+1)+") Main Index";
			cell.SetText(str);
		}
		else {
			cell.SetBackColor(m_ColorClick);
			str="("+FUtil::STD_ItoAscii(i+1)+") Not Selected";
			cell.SetText(str);
			for (size_t j=0; j<m_pCTab->GetNumbersToPlot();j++) {
				int selected=m_pCTab->GetPlotIndex(j);
				if(selected==i) {
					cell.SetBackColor(m_ColorChanged);
					str="("+FUtil::STD_ItoAscii(i+1)+") Selected";
					cell.SetText(str);
				}
			}
		}
		SetCell(0,i,&cell);
	}






//	EnableMenu(TRUE); 

	SetVScrollMode ( UG_SCROLLTRACKING );
	SetUniformRowHeight( TRUE );
	
}

/////////////////////////////////////////////////////////////////////////////
//	OnSheetSetup	
void SimpleCug::OnSheetSetup(int sheetNumber)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the current cell in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int SimpleCug::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the top row or left column in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int SimpleCug::OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	
void SimpleCug::OnHitBottom(long numrows,long rowspast,long rowsfound)
{
}

/////////////////////////////////////////////////////////////////////////////
//	
void SimpleCug::OnHitTop(long numrows,long rowspast)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeCol
//		Sent when the user is over a separation line on the top heading
//		A return value of TRUE allows the possibiliy of a resize
int SimpleCug::OnCanSizeCol(int col)
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
void SimpleCug::OnColSizing(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSized
//		This is sent when the user finished sizing the 
//		given column (see above for more details)
void SimpleCug::OnColSized(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeRow
//		Sent when the user is over a separation line on the side heading
//		A return value of TRUE allows the possibiliy of a resize
int  SimpleCug::OnCanSizeRow(long row)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSizing
//	    Sent when the user is sizing a row The row that is being sized is given as 
//		well as the height. Plus the height can be modified 
//		at this point. This makes it easy to set min and max heights
void SimpleCug::OnRowSizing(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSized
//		This is sent when the user is finished sizing hte
//		given row ( see above for more details)
void SimpleCug::OnRowSized(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeSideHdg
//		This is sent when the user moves into position 
//		for sizing the width of the side heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int SimpleCug::OnCanSizeSideHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeTopHdg
//		This is sent when the user moves into position 
//		for sizing the height of the top heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int SimpleCug::OnCanSizeTopHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSizing
int SimpleCug::OnSideHdgSizing(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSized
int SimpleCug::OnSideHdgSized(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSized
int SimpleCug::OnTopHdgSized(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSizing
int SimpleCug::OnTopHdgSizing(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColChange
//		Sent whenever the current column changes
//		The old and the new columns are given
void SimpleCug::OnColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowChange
//		Sent whenever the current row changes
//		The old and the new rows are given
void SimpleCug::OnRowChange(long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellChange
//		Sent whenever the current cell changes rows or columns
void SimpleCug::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnLeftColChange
//		Sent whenever the left visible column in the grid changes
void SimpleCug::OnLeftColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopRowChange
//		Sent whenever the top visible row in the grid changes
void SimpleCug::OnTopRowChange(long oldrow,long newrow)
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
void SimpleCug::OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum)
{
	TRACE("SimpleCug::OnSelectionChanged called with startCol = %ld, startRow = %ld, endCol = %ld, endRow = %ld, blockNum = %ld.\n",
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
void SimpleCug::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
	CUGCell cell; 
	string str;
	

	if(m_MultiTable) {



	}
	else {
		StartDragDrop();
		GetCell(0,row,&cell);
		if(cell.GetBackColor()==m_ColorClick) {
			cell.SetBackColor(m_ColorChanged);
			str="("+FUtil::STD_ItoAscii(row+1)+") Selected";
			cell.SetText(str);
		
		}
		else if(cell.GetBackColor()==m_ColorChanged) {
			cell.SetBackColor(m_ColorClick);
			str="("+FUtil::STD_ItoAscii(row+1)+") Not Selected";
			cell.SetText(str);			
		}
		SetCell(0,row,&cell);
		RedrawCell(0,row); 
		int nl=m_pCTab->GetNumbersToPlot();
		m_pCTab->ResetPlotIndex();
		int mainindex=m_pCTab->GetMainIndex();
		m_pCTab->SetPlotIndex(mainindex);
		nl=m_pCTab->GetNumbersToPlot();
		for (int i=0; i<m_row;i++) {
			GetCell(0,i,&cell);
			if(cell.GetBackColor()==m_ColorChanged)
				m_pCTab->SetPlotIndex(i);
		
		}
		nl=m_pCTab->GetNumbersToPlot();
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
void SimpleCug::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
//		Sent whenever the user double clicks the left mouse button within the grid
//	
//		'col' and 'row' are negative if the area clicked in is not a valid cell
//		'rect' the rectangle of the cell that was clicked in
//		'point' the point where the mouse was clicked
void SimpleCug::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	StartEdit();
}				 

/////////////////////////////////////////////////////////////////////////////
//	OnMouseMove
void SimpleCug::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user clicks the left mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void SimpleCug::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if ( !updn )
	{
		SelectRange ( col, 0, col, GetNumberRows()-1 );
	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_RClicked
//		Sent whenever the user clicks the right mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void SimpleCug::OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top heading
//	
//		'col' is negative if the area clicked in is not valid
void SimpleCug::OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_LClicked
//		Sent whenever the user clicks the left mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void SimpleCug::OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if ( !updn )
		SelectRange ( 0, row, GetNumberCols()-1, row );
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_RClicked
//		Sent whenever the user clicks the right mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void SimpleCug::OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_DClicked
//		Sent whenever the user double clicks the left mouse button within the side heading
//	
//		'row' is negative if the area clicked in is not valid
void SimpleCug::OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_LClicked
//		Sent whenever the user clicks the left mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void SimpleCug::OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_RClicked
//		Sent whenever the user clicks the right mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void SimpleCug::OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top corner button
void SimpleCug::OnCB_DClicked(RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyDown
//		Sent whenever the user types when the grid has focus. The keystroke can be
//		modified here as well. (See WM_KEYDOWN for more information)
void SimpleCug::OnKeyDown(UINT *vcKey,BOOL processed)
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
void SimpleCug::OnCharDown(UINT *vcKey,BOOL processed)
{
}
	
/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent everytime the grid needs to
//		draw a cell in the grid. At this point the cell
//		class has been filled with the information to be
//		used to draw the cell. The information can now be
//		changed before it is used for drawing
void SimpleCug::OnGetCell(int col,long row,CUGCell *cell)
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
void SimpleCug::OnSetCell(int col,long row,CUGCell *cell)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDataSourceNotify
//		This message is sent from a data source , message
//		depends on the data source - check the information
//		on the data source(s) being used
//		- The ID of the Data source is also returned
void SimpleCug::OnDataSourceNotify(int ID,long msg,long param)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellTypeNotify
//		This message is sent from a cell type , message
//		depends on the cell type - check the information
//		on the cell type classes
//		- The ID of the cell type is given
int SimpleCug::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditStart
//		This message is sent whenever the grid is ready to start editing a cell
//		A return of TRUE allows the editing a return of FALSE stops editing
//		Plus the properties of the CEdit class can be modified
int SimpleCug::OnEditStart(int col, long row,CWnd **edit)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditVerify
//		This is send when the editing is about to end
int SimpleCug::OnEditVerify(int col, long row,CWnd *edit,UINT *vcKey)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int SimpleCug::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR std::string,BOOL cancelFlag)
{
	CString str;
	CUGCell cell; 
	edit->GetWindowText(str);
	//QuickSetText(col,row,str);
	if(m_MultiTable) {

				double v=double(MFC_Util::AtoFloat(str));
				if(!m_ShowAll) {
					if(m_pType==0)
						((Ps*)m_SIMB.pBase)->MR_Set_TabValue(row,v);
					else
						((P*)m_SIMB.pBase)->MR_Set_TabValue(m_TabIndex, row, v);
					
				}
				else {
					SIMB PAR;
					PAR=m_PtrArray[col];
					//TabIndex=m_TabIndexArray.GetAt(col);
					if(PAR.pBase->Is_P()) {
						((Ps*)PAR.pBase)->MR_Set_TabValue(row,v);
						v=((Ps*)PAR.pBase)->MR_Get_TabValue(row);
					}
					else {
						((P*)PAR.pBase)->MR_Set_TabValue(PAR.TabIndex, row, v);
						v=((P*)PAR.pBase)->MR_Get_TabValue(PAR.TabIndex, row);
					}
					GetCell(col,row,&cell);	
					cell.SetNumber(v);
					SetCell(col,row,&cell);
					RedrawCell(col,row);
				}

	}


	//OnSetup();
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int SimpleCug::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	sections - UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//				UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
void SimpleCug::OnMenuCommand(int col,long row,int section,int item)
{
	CUGCell cell; 
	if(section==UG_GRID) {
		if(item== 1000)
			CutSelected();

		if(item==2000)
			CopySelected();

		if(item==3000) {
			Paste();
			SIMB PAR;
			for(int icol=0;icol<m_col;icol++) {
				PAR=m_PtrArray[icol];
				
				//TabIndex=m_TabIndexArray.GetAt(icol);

				if(PAR.pBase->Is_P())
					((Ps*)PAR.pBase)->MR_Reset_Table();
				else
					((P*)PAR.pBase)->MR_Reset_Table(PAR.TabIndex);

				for(int i=0;i<m_row;i++) {
					GetCell(icol,i,&cell);
					auto v=cell.GetNumber();
					if(PAR.pBase->Is_P())
						((Ps*)PAR.pBase)->MR_Append_TableValue(v);
					else
						((P*)PAR.pBase)->MR_Append_TableValue(PAR.TabIndex,v);
				}
			}
			OnSetup();
			RedrawAll();
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
//	return UG_SUCCESS to allow the menu to appear
//	return 1 to not allow the menu to appear
int SimpleCug::OnMenuStart(int col,long row,int section)
{
			//****** Populate the Menu 
	if(section==UG_GRID) {
			EmptyMenu();
			AddMenuItem(1000, "Cut"); 
			AddMenuItem(2000, "Copy"); 
			AddMenuItem(3000, "Paste"); 
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHint
int SimpleCug::OnHint(int col,long row,int section,CString *string)
{
	string->Format(_T("Col:%d Row:%ld"),col,row);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnVScrollHint
int SimpleCug::OnVScrollHint(long row,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHScrollHint
int SimpleCug::OnHScrollHint(int col,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OLE
#ifdef __AFXOLE_H__

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  SimpleCug::OnDragEnter(COleDataObject* pDataObject){
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  SimpleCug::OnDragOver(COleDataObject* pDataObject,int col,long row){
	//return DROPEFFECT_NONE;
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  SimpleCug::OnDragDrop(COleDataObject* pDataObject,int col,long row){
	return DROPEFFECT_COPY;
}
#endif

/////////////////////////////////////////////////////////////////////////////
//	
void SimpleCug::OnScreenDCSetup(CDC *dc,int section){
}

/////////////////////////////////////////////////////////////////////////////
//	OnSortEvaluate
//	return		-1  <
//				0	==
//				1	>
int SimpleCug::OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags)
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
void SimpleCug::OnTabSelected(int ID)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnAdjustComponentSizes
void SimpleCug::OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,
		RECT *cnrBtn,RECT *vScroll,RECT *hScroll,RECT *tabs)
{
} 

/////////////////////////////////////////////////////////////////////////////
//	OnDrawFocusRect
void SimpleCug::OnDrawFocusRect(CDC *dc,RECT *rect)
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

void SimpleCug::DrawExcelFocusRect(CDC *dc,RECT *rect)
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
COLORREF SimpleCug::OnGetDefBackColor(int section)
{
	return GetSysColor ( COLOR_APPWORKSPACE );
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetFocus
void SimpleCug::OnSetFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKillFocus
void SimpleCug::OnKillFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapStart
BOOL SimpleCug::OnColSwapStart(int col)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanColSwap
BOOL SimpleCug::OnCanColSwap(int fromCol,int toCol)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTrackingWindowMoved
void SimpleCug::OnTrackingWindowMoved(RECT *origRect,RECT *newRect)
{
}

