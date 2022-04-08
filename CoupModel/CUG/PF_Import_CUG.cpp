/////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived PF_Import_Cug

#include "../stdafx.h"
#include "../Util.h"
#include "../resource.h"
#include "../ModelTypes/NewModelType.h"
#include "PF_Import_Cug.h"
#include "../SP/SP_Retrieve.h"
#include "../SoilData/PlotPF/PFCurve.h"
//#include "../ParameterPlotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(PF_Import_Cug,CUGCtrl)
	//{{AFX_MSG_MAP(PF_Import_Cug)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// PF_Import_Cug construction/destruction
PF_Import_Cug::PF_Import_Cug()
{
//	UGXPThemes::UseThemes(true);
	m_bDrawExcelFocus = FALSE;
	m_col=1;
	m_row=1;
	m_DataBase=TRUE;
	m_CompleteList=FALSE;
	m_pCTab=nullptr;
	m_pDB2=nullptr;
	m_SelOne=FALSE;
	m_ColorChanged=RGB(255,200,200);
	m_ColorHeading=RGB(230,230,230);
	m_ColorClick=RGB(240,240,50);
	m_PrevClickRow=-1;
	p_PFCurve = &PlotpF::GetPointer();
		
	if(p_PFCurve->IsOpen())
	if(p_PFCurve->IsProfile()) {
		m_IniProf=p_PFCurve->m_ActProfile.KeyProf/100;
	    m_IniRepProf=p_PFCurve->m_ActProfile.KeyProf%100;
	}
	else
		m_IniProf=-10;

/*	if(PLOTPF_mp_FINDPROFILE()) {
	   m_IniProf=PLOTPF_mp_ACTUALPROFILE;
	   m_IniRepProf=PLOTPF_mp_ACTUALREPPROFILE;
	}
	else
		m_IniProf=-10;
*/
	SetMultiSelectMode ( TRUE );
	DragDropTarget ( TRUE );
	m_pCUG=nullptr;
	m_LatestSheet=0;
	m_CreateNewNumbers=TRUE;
	m_toprow=0;
}

PF_Import_Cug::~PF_Import_Cug()
{
	//UGXPThemes::CleanUp();
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid



int PF_Import_Cug::GetNumberOfRows()
{
	return m_row;

}


void PF_Import_Cug::MakeBaseSheet()
{
	CUGCell cell; 
	CString str;
	CString fstring;

	int cols = m_col,
		rows = m_row;
	int Count;
	int Record;
	CString string;
	Count=0;
	SetTH_NumberRows(1); 
	SetTH_Height(25); 
	SetTH_RowHeight(-1,25);


	if(m_DataBaseString=="Kalle") {

	

		EnableMenu(TRUE); 

		SetVScrollMode ( UG_SCROLLTRACKING );
		SetUniformRowHeight( TRUE );
		return;
	}
	if(m_DataBase) {
		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE);
		int ini_prof=p_PFCurve->m_ActProfile.KeyProf/100;//PLOTPF_mp_ACTUALPROFILE;
		int ini_rep_prof=p_PFCurve->m_ActProfile.KeyProf%100;//PLOTPF_mp_ACTUALREPPROFILE;

		m_row=0;

		SetNumberCols ( 40 );


	Record=1;
	NEWHEADER header;
	vector<float> pressure;
	p_PFCurve->InitNewScan();

	while(p_PFCurve->ScanNewHeaders(&header)) {


		   auto nlayers=header.NumLayers;
			m_row+=nlayers;
			Record++;	
		}


		SetNumberRows (m_row);
		SetTH_NumberRows(2); 
		//******* Set the total height of the top heading 
		SetTH_Height(50); 

//******* Set the heights of the individual heading rows 
		SetTH_RowHeight(-1,25); 
		SetTH_RowHeight(-2,25); 
		QuickSetText(-1,-2,"Line \nNo");

		QuickSetCellTypeEx(-1,-2,UGCT_NORMALMULTILINE);
		//QuickSetCellTypeEx(0,-2,UGCT_NORMALMULTILINE);
		QuickSetAlignment(-1,-2,UG_ALIGNBOTTOM);
		QuickSetAlignment(0,-2,UG_ALIGNBOTTOM);
		QuickSetAlignment(1,-2,UG_ALIGNBOTTOM);
		QuickSetAlignment(2,-2,UG_ALIGNBOTTOM);
		QuickSetAlignment(3,-2,UG_ALIGNBOTTOM);
		JoinCells(7,-2,9,-2);
		JoinCells(10,-2,12,-2);
		JoinCells(-1,-2,-1,-1);
		JoinCells(0,-2,0,-1);
		JoinCells(1,-2,1,-1);
		JoinCells(2,-2,2,-1);
		JoinCells(3,-2,3,-1);
		JoinCells(4,-2,4,-1);
		JoinCells(5,-2,5,-1);
		JoinCells(6,-2,6,-1);
		for(int i=13;i<40;i++) {

			QuickSetCellTypeEx(i,-2,UGCT_NORMALMULTILINE);
			QuickSetAlignment(i,-2,UG_ALIGNBOTTOM);
			JoinCells(i,-2,i,-1);
		}

	



		QuickSetText(0,-2,"No1");
		QuickSetText(1,-2,"No2");
		QuickSetText(2,-2,"Name");
		QuickSetText(3,-2,"Lat");
		QuickSetText(4,-2,"Long");
		QuickSetText(5,-2,"County");
		QuickSetText(6,-2,"Country");
		QuickSetText(7,-2,"Top Soil");
		QuickSetText(7,-1,"Clay");
		QuickSetText(8,-1,"Sand");
		QuickSetText(9,-1,"Org");
		QuickSetText(10,-2,"Sub Soil");
		QuickSetText(10,-1,"Clay");
		QuickSetText(11,-1,"Sand");
		QuickSetText(12,-1,"Org");
		QuickSetText(13,-1,"Year");
		QuickSetText(14,-2,"Layer");
		QuickSetText(15,-2,"Upper");
		QuickSetText(16,-2,"Lower");
		QuickSetText(17,-2,"<0.002");
		QuickSetText(18,-2,"0.002\n-\n0.006");
		QuickSetText(19,-2,"0.006\n-\n0.02");	
		QuickSetText(20,-2,"0.02\n-\n0.06");
		QuickSetText(21,-2,"0.06\n-\n0.2");	
		QuickSetText(22,-2,"0.2\n-\n0.6");
		QuickSetText(23,-2,"0.6\n-\n2.0");	
		QuickSetText(24,-2,"loss\non\nignition");

		QuickSetText(25,-2,"Lambda \n(-)");
		QuickSetText(26,-2,"Air Entry\n(cm)");
		QuickSetText(27,-2,"Saturation\n(vol %)");
		QuickSetText(28,-2,"Wilting\npoint \n(vol %)");
		QuickSetText(29,-2,"Residual\nwater \n(vol %)");
		QuickSetText(30,-2,"Macro pore\n(vol %)");
		QuickSetText(31,-2,"Upper\nboundary\n(cm)");

		QuickSetText(32,-2,"m \n(-)");
		QuickSetText(33,-2,"n\n(cm)");
		QuickSetText(34,-2,"Alpha\n(1/cm)");

		QuickSetText(35,-2,"Matric Conductivty\n(mm/day");
		QuickSetText(36,-2,"Total Conductivity\n(mm/day)");

		QuickSetText(37,-2,"n\nTortuosity\n(-)");
		QuickSetText(38,-2,"n Relative\nsaturation\n(-)");
		QuickSetText(39,-2,"n Effective\nsaturation\n(-)");

		SetColWidth(-1,30);
		SetColWidth(0,45);
		SetColWidth(1,90);
		SetColWidth(2,90);
		for(int i=0;i<41;i++)
			SetColWidth(i, 120);

	

	//SetColWidth(0,86);
	//return;
		Record=1;
		CString string, tmpstring;

	

		Count=-1;
		p_PFCurve->InitNewScan();

		while(p_PFCurve->ScanNewHeaders(&header)) {

			string=_T("");
			for(int i=0;i<6;i++)
				string+=header.OldKey.c_str();
			
			CString String2=string.Mid(0,3);
			int prof=MFC_Util::AtoInt(String2);
			String2=string.Mid(4,2);
			int repNumber=MFC_Util::AtoInt(String2);
			int layer=1;


			//PLOTPF_mp_GETLAYER(&prof, &repNumber, &layer);
			int nvalid=0;
			vector<float> pressure;
			pressure=p_PFCurve->GetPF_Pressure(header.RecLayers);
			nvalid=pressure.size();
		/*	for(int i=0;i<PLOTPF_mp_NT;i++) {
				if(PLOTPF_mp_PPSI[i]>.05&&PLOTPF_mp_PPSI[i]<100.)
					nvalid++;

			}*/

			long nlayers=header.NumLayers;
				Count++;
				string=_T("");

				QuickSetNumber(-1,Count,Count+1);
				QuickSetNumber(0,Count, prof);
				QuickSetNumber(1,Count, repNumber);

				//QuickSetText(0,Count,string);

				string=_T("");
				for(int i=0;i<16;i++) {
					string+=header.OldKey.c_str();//PLOTPF_mp_RPLACE[i];
				}
				QuickSetText(2,Count,string);
				string=_T("");
				tmpstring=_T("");
				string+=FUtil::STD_FtoAscii(header.Lat).c_str();
				string+="/";
				string+=FUtil::STD_FtoAscii(header.Long).c_str();
				QuickSetText(3,Count,tmpstring);
				QuickSetText(4,Count,string);

	
				QuickSetText(5,Count,header.Country);

				QuickSetText(6,Count,string);

				GetCell(7,Count,&cell);	cell.SetNumber(header.Upper_Clay); SetCell(7,Count,&cell);
				GetCell(8,Count,&cell);	cell.SetNumber(header.Upper_Sand); SetCell(8,Count,&cell);
				GetCell(9,Count,&cell);	cell.SetNumber(header.Upper_Org); SetCell(9,Count,&cell);
				GetCell(10,Count,&cell);	cell.SetNumber(header.Lower_Clay); SetCell(10,Count,&cell);
				GetCell(11,Count,&cell);	cell.SetNumber(header.Lower_Sand); SetCell(11,Count,&cell);
				GetCell(12,Count,&cell);	cell.SetNumber(header.Lower_Org); SetCell(12,Count,&cell);
				QuickSetNumber(13,Count,UINT(header.Year));
				vector<float> tex;
				PFCOEF pfcoef;
				for(int i=0;i<header.NumLayers;i++) {
	
					tex=p_PFCurve->GetTexture(i);
					pfcoef=p_PFCurve->GetPF_Coef(i);
					QuickSetNumber(14,Count,i+1);
					QuickSetNumber(15,Count,pfcoef.UpperDepth);
					QuickSetNumber(16,Count,pfcoef.LowerDepth);
					for(int j=0;j<8;j++) {
						if(j==0||j==7)
							QuickSetNumber(j+17,Count,tex[j]);
						else if(j<6)
							QuickSetNumber(j+17,Count,tex[j]-tex[j-1]);
						else
							QuickSetNumber(j+17,Count,100-tex[7]-tex[5]);

					}

					QuickSetNumber(25,Count,pfcoef.Lambda);
					QuickSetNumber(26,Count,pfcoef.AirEntry);
					QuickSetNumber(27,Count,pfcoef.Saturation);
					QuickSetNumber(28,Count,pfcoef.Wilting);
					QuickSetNumber(29,Count,pfcoef.Residual);
					QuickSetNumber(30,Count,pfcoef.LowerBoundary);
					QuickSetNumber(31,Count,pfcoef.UpperBoundary);

					QuickSetNumber(32,Count,pfcoef.Gen_M);
					QuickSetNumber(33,Count,pfcoef.Gen_N);
					QuickSetNumber(34,Count,pfcoef.Gen_Alfa);

					QuickSetNumber(35,Count,pfcoef.MatConductivity);
					QuickSetNumber(36,Count,pfcoef.TotConductivity);
					QuickSetNumber(37,Count,pfcoef.Mualem_n);
					QuickSetNumber(38,Count,pfcoef.N_SrCoef);
					QuickSetNumber(39,Count,pfcoef.N_SECoef);
					Count++;

				}
				Count--;
				Record++;	
		}

		SetTopRow(m_toprow);
		RedrawAll();

	}

	EnableMenu(TRUE); 

	SetVScrollMode ( UG_SCROLLTRACKING );
	SetUniformRowHeight( TRUE );
}
void PF_Import_Cug::MakeTextureSheet()
{


}
void PF_Import_Cug::MakePFCurveSheet()
{


}
void PF_Import_Cug::MakeBoCSheet()
{
	
	


}
void PF_Import_Cug::MakeGenuchtenSheet()
{


}
void PF_Import_Cug::MakeHydraulicCondSheet()
{
	

}
void PF_Import_Cug::OnSetup()
{
	CUGCell cell; 
	CString str;
	CString fstring;


	if(m_DataBaseString=="DataSheet") {

		int num_sheets=GetNumberSheets();
		for (long i=0;i<num_sheets;i++) {
			DeleteTab(i);
		}
		SetDefColWidth(130);
		AddTab("Brook & Corey", 0); 
		AddTab("Van Genuchten", 1); 
		AddTab("Hydraulic Conductivity", 2); 
		AddTab("Texture", 3); 
		AddTab("PF Data Points", 4); 
		SetTabWidth(480); 
		SetSheetNumber(m_LatestSheet); 
		OnSheetSetup(m_LatestSheet);
		SetCurrentTab(m_LatestSheet);
		SetNumberSheets(5); 
	}
	else {
		MakeBaseSheet();
	}


}

/////////////////////////////////////////////////////////////////////////////
//	OnSheetSetup	
void PF_Import_Cug::OnSheetSetup(int sheetNumber)
{
	EnableMenu(TRUE); 
	SetSheetNumber(sheetNumber);
	if(m_DataBaseString=="DataSheet") {
				EnableUpdate(FALSE); 
				while(GetNumberRows()>0)
					DeleteRow(0);
				EnableUpdate(TRUE); 
				m_bDrawExcelFocus = FALSE;
				m_LatestSheet=sheetNumber;
				SetMultiSelectMode ( TRUE );
		//		DragDropTarget ( TRUE );
				switch(sheetNumber)		{
				case (0): 
					MakeBoCSheet();
					RedrawAll();
					break;
				case (1): 
					MakeGenuchtenSheet();
					RedrawAll();
					break;
				case (2): 
					MakeHydraulicCondSheet();
					RedrawAll();
					break;
				case (3): // 
					MakeTextureSheet();
					RedrawAll(); 
					break;
				case (4): // 
					MakePFCurveSheet();
					RedrawAll(); 
					break;
				}
	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the current cell in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int PF_Import_Cug::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the top row or left column in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int PF_Import_Cug::OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	
void PF_Import_Cug::OnHitBottom(long numrows,long rowspast,long rowsfound)
{
}

/////////////////////////////////////////////////////////////////////////////
//	
void PF_Import_Cug::OnHitTop(long numrows,long rowspast)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeCol
//		Sent when the user is over a separation line on the top heading
//		A return value of TRUE allows the possibiliy of a resize
int PF_Import_Cug::OnCanSizeCol(int col)
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
void PF_Import_Cug::OnColSizing(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSized
//		This is sent when the user finished sizing the 
//		given column (see above for more details)
void PF_Import_Cug::OnColSized(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeRow
//		Sent when the user is over a separation line on the side heading
//		A return value of TRUE allows the possibiliy of a resize
int  PF_Import_Cug::OnCanSizeRow(long row)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSizing
//	    Sent when the user is sizing a row The row that is being sized is given as 
//		well as the height. Plus the height can be modified 
//		at this point. This makes it easy to set min and max heights
void PF_Import_Cug::OnRowSizing(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSized
//		This is sent when the user is finished sizing hte
//		given row ( see above for more details)
void PF_Import_Cug::OnRowSized(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeSideHdg
//		This is sent when the user moves into position 
//		for sizing the width of the side heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int PF_Import_Cug::OnCanSizeSideHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeTopHdg
//		This is sent when the user moves into position 
//		for sizing the height of the top heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int PF_Import_Cug::OnCanSizeTopHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSizing
int PF_Import_Cug::OnSideHdgSizing(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSized
int PF_Import_Cug::OnSideHdgSized(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSized
int PF_Import_Cug::OnTopHdgSized(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSizing
int PF_Import_Cug::OnTopHdgSizing(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColChange
//		Sent whenever the current column changes
//		The old and the new columns are given
void PF_Import_Cug::OnColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowChange
//		Sent whenever the current row changes
//		The old and the new rows are given
void PF_Import_Cug::OnRowChange(long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellChange
//		Sent whenever the current cell changes rows or columns
void PF_Import_Cug::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnLeftColChange
//		Sent whenever the left visible column in the grid changes
void PF_Import_Cug::OnLeftColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopRowChange
//		Sent whenever the top visible row in the grid changes
void PF_Import_Cug::OnTopRowChange(long oldrow,long newrow)
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
void PF_Import_Cug::OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum)
{
	TRACE("PF_Import_Cug::OnSelectionChanged called with startCol = %ld, startRow = %ld, endCol = %ld, endRow = %ld, blockNum = %ld.\n",
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
void PF_Import_Cug::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
	CUGCell cell; 
	CString str;
	StartDragDrop();

	GetCell(0,row,&cell);
	if(m_DataBaseString=="DataSheet") return;

		if(cell.GetBackColor()==m_ColorClick) {
			if(m_PrevClickRow>=0) QuickSetBackColor(0,m_PrevClickRow,m_ColorClick);
			RedrawCell(0,m_PrevClickRow);
			m_PrevClickRow=row;
			cell.SetBackColor(m_ColorChanged);	
		}
		//else if(cell.GetBackColor()==m_ColorChanged) {
		//	cell.SetBackColor(m_ColorClick);	
		//}
		SetCell(0,row,&cell);
		RedrawCell(0,row); 
		str=cell.GetText();
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



			return;
		}
		if(m_DataBaseString=="Kalle") return; // Kalle = Called from PP_EDIT


	
		

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
void PF_Import_Cug::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
//		Sent whenever the user double clicks the left mouse button within the grid
//	
//		'col' and 'row' are negative if the area clicked in is not a valid cell
//		'rect' the rectangle of the cell that was clicked in
//		'point' the point where the mouse was clicked
void PF_Import_Cug::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	StartEdit();
}				 

/////////////////////////////////////////////////////////////////////////////
//	OnMouseMove
void PF_Import_Cug::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user clicks the left mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PF_Import_Cug::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{

	if(m_DataBaseString=="Kalle"&&m_DataBase&&col==0) {
		if ( !updn )
		{
			SelectRange ( col, 0, col, GetNumberRows()-1 );

		}
		if(updn)
			SortBy(col, UG_SORT_ASCENDING); 
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
void PF_Import_Cug::OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{

}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top heading
//	
//		'col' is negative if the area clicked in is not valid
void PF_Import_Cug::OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_LClicked
//		Sent whenever the user clicks the left mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PF_Import_Cug::OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
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
void PF_Import_Cug::OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_DClicked1955
//		Sent whenever the user double clicks the left mouse button within the side heading
//	
//		'row' is negative if the area clicked in is not valid
void PF_Import_Cug::OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_LClicked
//		Sent whenever the user clicks the left mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PF_Import_Cug::OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_RClicked
//		Sent whenever the user clicks the right mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PF_Import_Cug::OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top corner button
void PF_Import_Cug::OnCB_DClicked(RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyDown
//		Sent whenever the user types when the grid has focus. The keystroke can be
//		modified here as well. (See WM_KEYDOWN for more information)
void PF_Import_Cug::OnKeyDown(UINT *vcKey,BOOL processed)
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
void PF_Import_Cug::OnCharDown(UINT *vcKey,BOOL processed)
{
}
	
/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent everytime the grid needs to
//		draw a cell in the grid. At this point the cell
//		class has been filled with the information to be
//		used to draw the cell. The information can now be
//		changed before it is used for drawing
void PF_Import_Cug::OnGetCell(int col,long row,CUGCell *cell)
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
void PF_Import_Cug::OnSetCell(int col,long row,CUGCell *cell)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDataSourceNotify
//		This message is sent from a data source , message
//		depends on the data source - check the information
//		on the data source(s) being used
//		- The ID of the Data source is also returned
void PF_Import_Cug::OnDataSourceNotify(int ID,long msg,long param)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellTypeNotify
//		This message is sent from a cell type , message
//		depends on the cell type - check the information
//		on the cell type classes
//		- The ID of the cell type is given
int PF_Import_Cug::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
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
int PF_Import_Cug::OnEditStart(int col, long row,CWnd **edit)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditVerify
//		This is send when the editing is about to end
int PF_Import_Cug::OnEditVerify(int col, long row,CWnd *edit,UINT *vcKey)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int PF_Import_Cug::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR st,BOOL cancelFlag)
{
	CUGCell cell; 
	CString str;
	edit->GetWindowText(str);
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int PF_Import_Cug::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	sections - UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//				UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
void PF_Import_Cug::OnMenuCommand(int col,long row,int section,int item)
{


}

/////////////////////////////////////////////////////////////////////////////
//	return UG_SUCCESS to allow the menu to appear
//	return 1 to not allow the menu to appear
int PF_Import_Cug::OnMenuStart(int col,long row,int section)
{
			//****** Populate the Menu 
	if(section==UG_GRID) {
			EmptyMenu();
			if(m_DataBaseString=="DataSheet") {

				AddMenuItem(2000, "Copy"); 
				AddMenuItem(3000, "Paste"); 
				AddMenuItem(5000, "Specify number of soil layers to import");
			}
			else if(col==5&&m_DataBase) {
/*				CString KeyMove;
				CUGCell cell; 
				GetCell(col,row,&cell);
				KeyMove=cell.GetText();
				int rec=cell.GetParam();
				AddMenuItem(1310, "Create copy of "+ KeyMove);
				if(rec<((PP_EDIT*)m_pPP_EDIT_Dlg)->m_NumOriginalRecords) {
					AddMenuItem(1300, "Move "+ KeyMove+ " to write/delete enabled position");
				}
				else {
					AddMenuItem(1300, "Move "+ KeyMove+ " to write protected position");
					AddMenuItem(1301, "Delete "+ KeyMove+ " record");
				}*/
			}
			else {
				AddMenuItem(2000, "Copy"); 
				AddMenuItem(3000, "Paste"); 
				AddMenuItem(5000, "Specify number of soil layers to import");
				AddMenuItem(5001, "Import All data to database");
			}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHint
int PF_Import_Cug::OnHint(int col,long row,int section,CString *string)
{
	string->Format(_T("Col:%d Row:%ld"),col,row);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnVScrollHint
int PF_Import_Cug::OnVScrollHint(long row,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHScrollHint
int PF_Import_Cug::OnHScrollHint(int col,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OLE
#ifdef __AFXOLE_H__

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  PF_Import_Cug::OnDragEnter(COleDataObject* pDataObject){
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  PF_Import_Cug::OnDragOver(COleDataObject* pDataObject,int col,long row){
	//return DROPEFFECT_NONE;
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  PF_Import_Cug::OnDragDrop(COleDataObject* pDataObject,int col,long row){
	return DROPEFFECT_COPY;
}
#endif

/////////////////////////////////////////////////////////////////////////////
//	
void PF_Import_Cug::OnScreenDCSetup(CDC *dc,int section){
}

/////////////////////////////////////////////////////////////////////////////
//	OnSortEvaluate
//	return		-1  <
//				0	==
//				1	>
int PF_Import_Cug::OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags)
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
void PF_Import_Cug::OnTabSelected(int ID)
{
	OnSheetSetup(ID);
}

/////////////////////////////////////////////////////////////////////////////
//	OnAdjustComponentSizes
void PF_Import_Cug::OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,
		RECT *cnrBtn,RECT *vScroll,RECT *hScroll,RECT *tabs)
{
} 

/////////////////////////////////////////////////////////////////////////////
//	OnDrawFocusRect
void PF_Import_Cug::OnDrawFocusRect(CDC *dc,RECT *rect)
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

void PF_Import_Cug::DrawExcelFocusRect(CDC *dc,RECT *rect)
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
COLORREF PF_Import_Cug::OnGetDefBackColor(int section)
{
	return GetSysColor ( COLOR_APPWORKSPACE );
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetFocus
void PF_Import_Cug::OnSetFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKillFocus
void PF_Import_Cug::OnKillFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapStart
BOOL PF_Import_Cug::OnColSwapStart(int col)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanColSwap
BOOL PF_Import_Cug::OnCanColSwap(int fromCol,int toCol)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTrackingWindowMoved
void PF_Import_Cug::OnTrackingWindowMoved(RECT *origRect,RECT *newRect)
{
}
BOOL PF_Import_Cug::SaveToSim()
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
int PF_Import_Cug::GetMultiTypeIndex(string str)
{
	int index;
	string optionlist;
	int icounto=0;	
	optionlist="";




	int ipos, ipos_end;
	ipos_end=optionlist.find(str);
	
	optionlist=optionlist.substr(0,ipos_end);
	ipos=optionlist.find("\n");
	index=0;
	while(ipos>0) {
		optionlist=optionlist.substr(ipos+1);
		ipos=optionlist.find("\n");
		index++;
	}
	return index+1;

}
void PF_Import_Cug::SaveTopRow()
{
	m_toprow=GetTopRow();
}
void PF_Import_Cug::SetTopRowExternt()
{
	SetTopRow(m_toprow);
	RedrawAll();
}
void PF_Import_Cug::NewRowNumber(int value)
{
	m_row=value;
	EnableUpdate(FALSE); 
	while(GetNumberRows()>m_row)
		DeleteRow(m_row);
	EnableUpdate(TRUE);
	if(GetNumberRows()<m_row) {
		SetNumberCols(40);
		SetNumberRows(m_row);
		RedrawAll();
	}
}
int PF_Import_Cug::Import()
{
	CUGCell cell;
	string txt1, txt2, txt;
	size_t	iv1, iv2,iv, nc, ll ;
	float value,value1, value2;
	size_t NumSaved;
	NumSaved=0;
	m_row=GetNumberRows();
	BOOL newprofile, notsaved;
	notsaved=FALSE;
	m_CreateNewNumbers=FALSE;
	PFCOEF pfcoef;
	NEWHEADER header;
	vector<float> texture,theta, pressure;
	texture.resize(8);

	for(int i=0; i<m_row;i++) {
			GetCell(14,i,&cell);
			ll=size_t(cell.GetNumber());
					if(ll==1) 
						newprofile=TRUE;
					else
						newprofile=FALSE;
					if(newprofile&&notsaved) {
						if(SaveProfile())
							NumSaved++;
						notsaved=FALSE;
					}
					if(m_CreateNewNumbers) {
						header.KeyProf=710+(NumSaved+1);						
					}
					
			for (int j=0;j<40;j++) {
			GetCell(j,i,&cell);
			switch(j) {
				case 0:
					iv1=size_t(cell.GetNumber());				
					if(newprofile&&!m_CreateNewNumbers)
						header.KeyProf=iv1*100;
					break;
				case 1:
					notsaved=TRUE;
					iv2=size_t(cell.GetNumber());
					if(newprofile&&!m_CreateNewNumbers) 
						header.KeyProf+=iv2;
					else if(newprofile)
						header.KeyProf+=NumSaved+1-(NumSaved+1)/100*100;
					break;
				case 2:
					txt=cell.GetStdText();
					if(newprofile) {
						
						nc=txt.size();
						header.Name=txt;

					}
					break;
					
				case 3:
					txt1=cell.GetStdText();
					break;
				case 4:
					txt2=cell.GetStdText();
					if(newprofile) {

					}

					break;
				case 5:
					txt1=cell.GetStdText();

					break;
				case 6:
					txt2=cell.GetStdText();
					if(newprofile) {

					}
					break;
				case 7:
					value=cell.GetNumber();
					if(newprofile) {
						header.Upper_Clay=value;

					}
					break;
				case 8:
					value=cell.GetNumber();
					if(newprofile) {
						header.Upper_Sand=value;

					}
					break;
				case 9:
					value=cell.GetNumber();
					if(newprofile) {
						header.Upper_Org=value;

					}
					break;
				case 10:
					value=cell.GetNumber();
					if(newprofile) {
						header.Lower_Clay=value;

					}
					break;
				case 11:
					value=cell.GetNumber();
					if(newprofile) {
						header.Lower_Sand=value;

					}
					break;
				case 12:
					value=cell.GetNumber();
					if(newprofile) {
						header.Lower_Org=value;

					}
					break;
				case 13:
					iv=size_t(cell.GetNumber());
					//PLOTPF_mp_NT=0;
					if(newprofile&&!m_CreateNewNumbers) {
						header.Year=iv;
					}
					else 
						header.Year=2013;
					break;
				case 14:
					iv1=size_t(cell.GetNumber());
					//PLOTPF_mp_ACTUALLEVEL=iv1;
					ll=iv1-1;
					
					break;
				case 15:
					value1=cell.GetNumber();
					pfcoef.UpperDepth=value1;
					break;
				case 16:
					value2=cell.GetNumber();
					pfcoef.LowerDepth=value2;
					break;
				case 17: 
					value=cell.GetNumber();
					texture[0]=value;
					break;
				case 24:
					value=cell.GetNumber();
					texture[7]=value;
					break;
				case 18:case 19:case 20:case 21:case 22:case 23:
					value=cell.GetNumber();
					texture[j-17]=texture[j-18]+value;
					if(j==22||j==20){
						GetCell(24,i,&cell);
						float v7=cell.GetNumber();
						GetCell(22,i,&cell);
						float v5=cell.GetNumber();					
						texture[6]=100-v7-v5;
					}
					break;
				case 25:
					value=cell.GetNumber();
					pfcoef.Lambda=value;
					break;
				case 26:
					value=cell.GetNumber();
					pfcoef.AirEntry=value;
					break;
				case 27:
					value=cell.GetNumber();
					pfcoef.Saturation=value;
					break;
				case 28:
					value=cell.GetNumber();
					pfcoef.Wilting=value;
					break;
				case 29:
					value=cell.GetNumber();
					pfcoef.Residual=value;
					break;
				case 30:
					value=cell.GetNumber();
					pfcoef.LowerBoundary=value;
					break;
				case 31:
					value=cell.GetNumber();
					pfcoef.UpperBoundary=value;
					break;
				case 32:
					value=cell.GetNumber();
					pfcoef.Gen_M=value;
					break;
				case 33:
					value=cell.GetNumber();
					pfcoef.Gen_N=value;
					break;
				case 34:
					value=cell.GetNumber();
					pfcoef.Gen_Alfa=value;
					break;
				case 35:
					value=cell.GetNumber();
					pfcoef.MatConductivity=value;
					break;
				case 36:
					value=cell.GetNumber();
					pfcoef.TotConductivity=value;
					break;
				case 37:
					value=cell.GetNumber();
					pfcoef.Mualem_n=value;
					break;
				case 38:
					value=cell.GetNumber();
					pfcoef.N_SrCoef=value;
					break;
				case 39:
					value=cell.GetNumber();
					pfcoef.N_SECoef=value;
					break;
			}


		}
		//PLOTPF_mp_SAVEDATABASE();
	}
	if(notsaved) {
		if(SaveProfile())
			NumSaved++;
	}

	RedrawAll();
	return NumSaved;
}
BOOL PF_Import_Cug::SaveProfile()
{
	/*PLOTPF_mp_ACTUALNUMLEVELS=PLOTPF_mp_ACTUALLEVEL;
	PLOTPF_mp_SAVEDATABASE();
	PLOTPF_mp_FINDPROFILE;*/
	return FALSE;// PLOTPF_mp_SAVED;
}

