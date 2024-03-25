/////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived CValidTimeCug

#include "..\stdafx.h"
#include "..\std.h"
#include "..\Util.h"
#include "..\NewBase\Doc.h"
#include "..\resource.h"
#include "CValidTimeCug.h"
//#include "..\Chart\realtimedemodlg.h"
#include "..\Validation\ValidationVariables.h"
#include "ColourPickerDlg.h"
#include "..\CoupModelDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CValidTimeCug,CUGCtrl)
	//{{AFX_MSG_MAP(CCValidTimeCug)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CValidTimeCug construction/destruction
CValidTimeCug::CValidTimeCug()
{
//	UGXPThemes::UseThemes(true);
	m_bDrawExcelFocus = FALSE;
	m_col=1;
	m_row=1;

	m_pCTab=nullptr;
	m_ColorChanged=RGB(255,200,200);
	m_ColorHeading=RGB(230,230,230);
	m_ColorClick=RGB(240,240,50);

	SetMultiSelectMode ( FALSE );
	DragDropTarget ( FALSE );
	m_MultiTable=FALSE;
	m_Title="-";
	m_YLabel1="-";
	m_YLabel2="-";
	m_YLabel1Change=FALSE;
	m_YLabel2Change=FALSE;
	m_ChartNumIndex=-1;

	m_TitleSizePoint=MFC_Util::GetProfileIntNo("TitleSize", 12);
	m_LabelSizePoint=MFC_Util::GetProfileIntNo("LabelSize", 9);
	m_LegendSizePoint=MFC_Util::GetProfileIntNo("LegendSize", 9);
	m_LineWidthPoint=MFC_Util::GetProfileIntNo( "LineWidth", 2);
	m_LineAxisWidthPoint=MFC_Util::GetProfileIntNo("LineAxisWidth", 2);
	m_SymbolSizePoint=MFC_Util::GetProfileIntNo( "SymbolSize", 9);
	m_colpos=0;
	m_YScaleLeftChanged=m_YScaleLeftSet=FALSE;
	m_YScaleRightChanged=m_YScaleRightSet=FALSE;
	m_PreviousClick=-1;
	m_ColSort=0;
	

}

CValidTimeCug::~CValidTimeCug()
{
	//UGXPThemes::CleanUp();
}
void CValidTimeCug::Init(CDialog *pDialog, CCoupModelDoc *pDoc, BOOL picker, CValidTimeCug *pCug, BOOL selected)
{	m_pDlg=pDialog;
	m_pDoc=pDoc;
	m_pSimDoc=&m_pDoc->m_Doc;
	m_Legend1.RemoveAll();
	m_YScaleLeftChanged=m_YScaleLeftSet=FALSE;
	m_YScaleRightChanged=m_YScaleRightSet=FALSE;
	m_Picker=picker;
	m_pRealTimeCug=pCug;
	m_Selected=selected;

}
void CValidTimeCug::SetChartNumIndex(int index)
{
	m_ChartNumIndex=index;
}
void CValidTimeCug::SetTitle(CString str)
{
	m_Title=str;
}
void CValidTimeCug::SetYLabel1(CString str)
{
	m_YLabel1=str;
}
void CValidTimeCug::SetYLabel2(CString str)
{
	m_YLabel2=str;
}
BOOL CValidTimeCug::isYLabel1Changed()
{
	return m_YLabel1Change;
}
BOOL CValidTimeCug::isYLabel2Changed()
{
	return m_YLabel2Change;
}
CString CValidTimeCug::GetYLabel1()
{
	return m_YLabel1;
}
CString CValidTimeCug::GetYLabel2()
{
	return m_YLabel2;
}
CString CValidTimeCug::GetTitle()
{
	return m_Title;
}
void CValidTimeCug::ResetSerie1()
{
	m_Legend1.RemoveAll();
	m_Legend1Change.RemoveAll();
}
void CValidTimeCug::ResetSerie2()
{
	m_Legend2.RemoveAll();
	m_Legend2Change.RemoveAll();
}
BOOL CValidTimeCug::AddLegend1(CString str)
{
	m_Legend1.Add(str);
	m_Legend1Change.Add(0);
	return TRUE;
}
BOOL CValidTimeCug::AddLegend2(CString str)
{
	m_Legend2.Add(str);
	m_Legend1Change.Add(0);
	return TRUE;
}
BOOL CValidTimeCug::isYLegendChanged(int index)
{
	if(m_Legend1.GetSize()<=index)
		return FALSE;
	if(m_Legend1Change.GetAt(index)==0)
		return FALSE;
	else
		return TRUE;

}
BOOL CValidTimeCug::isYLegend2Changed(int index)
{
	if(m_Legend2Change.GetAt(index)==0)
		return FALSE;
	else
		return TRUE;

}
int CValidTimeCug::GetNum()
{
	return m_Legend1.GetSize();
}

CString CValidTimeCug::GetLegend(int index)
{
	if(m_Legend1.GetSize()>index)
		return m_Legend1.GetAt(index);
	else
		return _T("");
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid


void CValidTimeCug::OnUpdate()
{
	CUGCell cell; 
	CString str;
	int cols = m_col,
		rows = m_row;

	int startlegend;


	if(m_Picker) {








	}
	else {


			
		int n1=m_Legend1.GetSize();



		startlegend=4;
		cols=7;
		if(m_YLabel1!="-"){
			rows++;
		}
		if(m_YLabel2!="-") {
			startlegend=5;

		}
		while(GetNumberRows()>0)
			DeleteRow(0);
		rows=n1+startlegend;		
		SetNumberRows(rows);
		SetNumberCols(cols);
		SetColWidth(-1,0);
		for(int i=0;i<7;i++)
			SetColWidth(i,106);
		
		QuickSetText(0,0,m_Title);
		QuickSetNumber(1,0,m_TitleSizePoint);
		QuickSetNumber(2,0,m_LabelSizePoint);
		QuickSetNumber(3,0,m_LegendSizePoint);
		QuickSetNumber(4,0,m_LineWidthPoint);
		QuickSetNumber(5,0,m_LineAxisWidthPoint);

		QuickSetNumber(6,0,m_SymbolSizePoint);
		
		QuickSetText(0,1,"Y Labels");
		QuickSetText(1,1,"Min Value");
		QuickSetText(2,1,"Max Value");
		
		QuickSetBackColor(0,1,m_ColorHeading);
		QuickSetBackColor(1,1,m_ColorHeading);
		QuickSetBackColor(2,1,m_ColorHeading);

		QuickSetText(0,2,m_YLabel1);
		QuickSetNumber(1,2,m_YScaleLeftMin);
		QuickSetNumber(2,2,m_YScaleLeftMax);


		if(startlegend==5){
			QuickSetText(0,3,m_YLabel2);
			QuickSetNumber(1,3,m_YScaleRightMin);
			QuickSetNumber(2,3,m_YScaleRightMax);
		}

		QuickSetText(0,startlegend-1,"Legends");
		QuickSetBackColor(0,startlegend-1,m_ColorHeading);
		
		for(int i=0;i<n1;i++) {
				QuickSetText(0,i+startlegend,m_Legend1.GetAt(i));
		}
	}
	RedrawAll();
		

}
void CValidTimeCug::OnSetup()
{
	CUGCell cell; 
	string str, astr, strdef;
	int cols = m_col,rows = m_row;
	int row;

	CWinApp* pApp = AfxGetApp();
	int NumLines;
	int LastRow, StartRow;
	size_t newtablines,newline ;
	int MaxCol=8 ;
	int colv=0;
	int maxrow, isuper;
	string Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString, test;
	SimB *pSw;

	int niv, count;
	count=0, isuper=0;	
	long ivv=GetTopRow();

	SetUniformRowHeight(TRUE);

	if(m_Picker) {
		// Description of Sim Variables
		SetNumberCols ( 4 );
		QuickSetText(1,-1,"Group");
		QuickSetText(0,-1,"Type");				
		QuickSetText(2,-1,"Name");
		QuickSetText(3,-1,"Select No");
		SetColWidth(-1,0);
		SetColWidth(0,80);SetColWidth(1,138);SetColWidth(3,80);
		if (MFC_Util::isHighVertResolution())
			SetColWidth(2, 300);
		else
			SetColWidth(2, 160);


		if(m_Selected) {
			SetNumberRows(1);
			if(((CValidationVariables*)m_pDlg)->m_ReadySim) {
				QuickSetText(0,0,((CValidationVariables*)m_pDlg)->m_SelType);
				QuickSetText(1,0,((CValidationVariables*)m_pDlg)->m_SelGroup);
				QuickSetText(2,0,((CValidationVariables*)m_pDlg)->m_SelName);
				QuickSetNumber(3,0,((CValidationVariables*)m_pDlg)->m_ItemSelected+1);
			}
			else {
				QuickSetText(0,0," ");
				QuickSetText(1,0," ");
				QuickSetText(2,0," ");
				QuickSetText(3,0," ");


			}

			if(((CValidationVariables*)m_pDlg)->m_ReadySim) {
				QuickSetBackColor(0,0, m_ColorChanged);
				QuickSetBackColor(1,0, m_ColorChanged);
				QuickSetBackColor(2,0, m_ColorChanged);
				QuickSetBackColor(3,0, m_ColorChanged);
			}
			else {
				QuickSetBackColor(0,0, m_ColorHeading);
				QuickSetBackColor(1,0, m_ColorHeading);
				QuickSetBackColor(2,0, m_ColorHeading);
				QuickSetBackColor(3,0, m_ColorHeading);

			}


		}
		else {


		m_pSimDoc->SetOriginalValues("Outputs","ALL");

		newtablines=m_pSimDoc->GetSize_XTGD_Total();
		int extra=newtablines;
		newline=-1;
		LastRow=0;
		int vflagindex;
		vector<SIMB> vvp;
		for( int isuper=0; isuper<1;isuper++) {
				//for(size_t jk=0; jk<4;jk++) {
				vvp=m_pSimDoc->GetAllOutputs();
				SetNumberRows(vvp.size());
				for (size_t i = 0; i < vvp.size(); i++) {
					pSw = vvp[i].pBase;
					Type = pSw->GetTypeName();
					int selected;
					int lindex;
					string Text;

					if (pSw->Is_Vector()) {
						selected = ((OutVector*)pSw)->GetStoreFlag(vvp[i].TabIndex);
						vflagindex = ((OutVector*)pSw)->GetValidFlagIndex(vvp[i].TabIndex);
					}
					else {
						selected = ((OutSingle*)pSw)->GetStoreFlag();
						vflagindex = ((OutSingle*)pSw)->GetValidFlagIndex();
					}


					newline++;
					test = pSw->GetName();
					if (test == "Soil Respiration") {



						int hej;
						hej = 2;
						hej++;

					}
					lindex = vvp[i].TabIndex;
					if (pSw->GetType() < 6) {
						test += MFC_Util::AddIndexStdString(1 + vvp[i].TabIndex);

					}
						QuickSetText(1,newline, pSw->GetGroup());
						GetCell(2,newline,&cell);
						cell.SetText(test);
						cell.SetParam(600+isuper);
						SetCell(2,newline, &cell);

						QuickSetText(0,newline, Type);
								
						if(vflagindex>0) {
							if(vflagindex>m_pSimDoc->GetNumAll_TSV_ValVariables()) {
								if(pSw->Is_Vector())
									((OutVector*)pSw)->SetValidFlagIndex(vvp[i].TabIndex, 0);
								else 
									((OutSingle*)pSw)->SetValidFlagIndex(0);
							}
							else {
								auto Text=MFC_Util::ItoAscii(vflagindex)+"*";
								QuickSetText(3,newline,Text);
								QuickSetBackColor(3,newline, RGB(255-10*vflagindex,10*vflagindex, 255-5*vflagindex));
								QuickSetBackColor(2,newline, RGB(255-10*vflagindex,10*vflagindex, 255-5*vflagindex));
							}
						}
			}
			RedrawAll();
			maxrow=GetNumberRows();
		
		}
		if(m_ColSort>0) {
			SortBy(m_ColSort,UG_SORT_ASCENDING); 

		}
		StartRow=0;
		for (int i=0;i<maxrow;i++) {
				GetCell(colv, i, &cell);	
				Group=cell.GetStdText();
				SetCell(colv,i,&cell);
				if(PrevGroup==Group) {
					LastRow=i;
				}
				else {
					PrevGroup=Group;
					if(LastRow+1>StartRow&&LastRow>0) {
						JoinCells(colv,StartRow,colv,LastRow);
					}
					StartRow=i;
				}
			}
			if(LastRow+1>StartRow) {
				JoinCells(colv,StartRow,colv,LastRow);
			}
			colv=1;
			LastRow=0;
			StartRow=0;
			for (int i=0;i<maxrow;i++) {
				GetCell(colv, i, &cell);	
				Group=cell.GetStdText();
				SetCell(colv,i,&cell);
				if(PrevGroup==Group) {
					LastRow=i;
				}
				else {
					PrevGroup=Group;
					if(LastRow+1>StartRow&&LastRow>0) {
						JoinCells(colv,StartRow,colv,LastRow);
					}
					StartRow=i;
				}
			}
			if(LastRow+1>StartRow) {
				JoinCells(colv,StartRow,colv,LastRow);
			}
			int ivv=JoinCells(-1,0,-1,maxrow-1);
		}
	}
	else {
		// Description of Validation file contents
		F *pCF;
		//m_pFile=m_pSimDoc->ValidationFilePointer();
	//	m_NumSelected=m_pFile->GetNumberOfSelectedVal();
	//	m_pFile->SetNumberOfSelectedVal(m_NumSelected);
				
		EnableUpdate(FALSE); 		
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		int NumValFiles=0;
		count=0;
		for(size_t i=0;i<MAXSIMVAL;i++) {
			pCF=m_pSimDoc->ValidationFilePointer(i+1);
			CPG *pPG=pCF->GetPointer();
			niv=pPG->GetNumVariables();
		//	niv=pCF->GetNumberOfVariables();
			if(niv>0&&pCF->m_Exist) {
				NumValFiles++;
				count+=niv;
			}
		}

		NumLines=count+NumValFiles;
		SetNumberCols ( 4 );
		
		
		QuickSetText(0,-1,"File No");	
		SetColWidth(-1,0);
		SetColWidth(0,50);SetColWidth(1,50);
		SetColWidth(2,190);	SetColWidth(3,60);	
		if (MFC_Util::isHighVertResolution())
			SetColWidth(2, 300);

		QuickSetText(1,-1,"Var No");		
		QuickSetText(2,-1,"Name");	
		QuickSetText(3,-1,"Select No");		


		row=0;

		if(m_Selected) {
			SetNumberRows (1);

			if(((CValidationVariables*)m_pDlg)->m_ReadyObs) {
				QuickSetNumber(0,0,((CValidationVariables*)m_pDlg)->m_FileNo);
				QuickSetNumber(1,0,((CValidationVariables*)m_pDlg)->m_ValVarList+1);
				QuickSetText(2,0,((CValidationVariables*)m_pDlg)->m_ValDesc);
				QuickSetNumber(3,0,((CValidationVariables*)m_pDlg)->m_ItemSelected+1);
			}
			else {
				QuickSetText(0,0," ");
				QuickSetText(1,0," ");
				QuickSetText(2,0," ");
				QuickSetText(3,0," ");
			}
			if(((CValidationVariables*)m_pDlg)->m_ReadyObs) {
				QuickSetBackColor(0,0, m_ColorChanged);
				QuickSetBackColor(1,0, m_ColorChanged);
				QuickSetBackColor(2,0, m_ColorChanged);
				QuickSetBackColor(3,0, m_ColorChanged);
			}
			else {
				QuickSetBackColor(0,0, m_ColorHeading);
				QuickSetBackColor(1,0, m_ColorHeading);
				QuickSetBackColor(2,0, m_ColorHeading);
				QuickSetBackColor(3,0, m_ColorHeading);

			}
		}
		else {
			SetNumberRows (NumLines);
			for(size_t i=0;i<NumValFiles;i++) {
				pCF=m_pSimDoc->ValidationFilePointer(i+1);
				CPG *pPG=pCF->GetPointer();

				niv=pPG->GetNumVariables();
				CString title;
				QuickSetText(0,row,MFC_Util::ItoAscii(i+1));
				title=pCF->GetStrValue().c_str();
				QuickSetText(1,row,title);
				QuickSetBackColor(2,row,m_ColorClick);
				JoinCells(1,row,3,row);
				row++;
				for(int j=0;j<niv;j++) {
					int num=pCF->GetValNumber(j);
					if(num>0) {
						QuickSetNumber(3,row, num);
						QuickSetBackColor(0,row, RGB(255-10*num,10*num, 255-5*num));
						QuickSetBackColor(1,row, RGB(255-10*num,10*num, 255-5*num));
						QuickSetBackColor(3,row, RGB(255-10*num,10*num, 255-5*num));
					}	
					QuickSetNumber(0,row,i+1);
					QuickSetNumber(1,row, j+1);
					if(pPG!=nullptr) {
						string tit=pPG->GetVarName(j+1)+pPG->GetVarId(j+1);
						QuickSetText(2,row,tit);
					}
					else
						QuickSetText(2,row,"-");



					row++;
				}

			}
		}

	}


		RedrawAll();		


		/*int index = AddDataSource(&m_data);
		SetDefDataSource(index);
		SetGridUsingDataSource(index);*/
		SetVScrollMode ( UG_SCROLLTRACKING );
		SetHScrollMode ( UG_SCROLLTRACKING );
		SetUniformRowHeight( TRUE );
		EnableMenu(TRUE); 
		UseHints(TRUE);
		if(ivv>0) SetTopRow(ivv);
		return;
	
}

/////////////////////////////////////////////////////////////////////////////
//	OnSheetSetup	
void CValidTimeCug::OnSheetSetup(int sheetNumber)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the current cell in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int CValidTimeCug::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the top row or left column in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int CValidTimeCug::OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	
void CValidTimeCug::OnHitBottom(long numrows,long rowspast,long rowsfound)
{
}

/////////////////////////////////////////////////////////////////////////////
//	
void CValidTimeCug::OnHitTop(long numrows,long rowspast)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeCol
//		Sent when the user is over a separation line on the top heading
//		A return value of TRUE allows the possibiliy of a resize
int CValidTimeCug::OnCanSizeCol(int col)
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
void CValidTimeCug::OnColSizing(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSized
//		This is sent when the user finished sizing the 
//		given column (see above for more details)
void CValidTimeCug::OnColSized(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeRow
//		Sent when the user is over a separation line on the side heading
//		A return value of TRUE allows the possibiliy of a resize
int  CValidTimeCug::OnCanSizeRow(long row)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSizing
//	    Sent when the user is sizing a row The row that is being sized is given as 
//		well as the height. Plus the height can be modified 
//		at this point. This makes it easy to set min and max heights
void CValidTimeCug::OnRowSizing(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSized
//		This is sent when the user is finished sizing hte
//		given row ( see above for more details)
void CValidTimeCug::OnRowSized(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeSideHdg
//		This is sent when the user moves into position 
//		for sizing the width of the side heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int CValidTimeCug::OnCanSizeSideHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeTopHdg
//		This is sent when the user moves into position 
//		for sizing the height of the top heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int CValidTimeCug::OnCanSizeTopHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSizing
int CValidTimeCug::OnSideHdgSizing(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSized
int CValidTimeCug::OnSideHdgSized(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSized
int CValidTimeCug::OnTopHdgSized(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSizing
int CValidTimeCug::OnTopHdgSizing(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColChange
//		Sent whenever the current column changes
//		The old and the new columns are given
void CValidTimeCug::OnColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowChange
//		Sent whenever the current row changes
//		The old and the new rows are given
void CValidTimeCug::OnRowChange(long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellChange
//		Sent whenever the current cell changes rows or columns
void CValidTimeCug::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnLeftColChange
//		Sent whenever the left visible column in the grid changes
void CValidTimeCug::OnLeftColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopRowChange
//		Sent whenever the top visible row in the grid changes
void CValidTimeCug::OnTopRowChange(long oldrow,long newrow)
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
void CValidTimeCug::OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum)
{
	TRACE("CValidTimeCug::OnSelectionChanged called with startCol = %ld, startRow = %ld, endCol = %ld, endRow = %ld, blockNum = %ld.\n",
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
void CValidTimeCug::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{

	if(m_Selected) return;
	CString str;
		CUGCell cell; 
	int fno, vno;
	string Group, Type, Name;
	if(m_Picker) {
		GetCell(1,row, &cell);
		Group=cell.GetStdText();
		GetCell(0,row, &cell);
		Type=cell.GetStdText();
		GetCell(2,row, &cell);
		Name=cell.GetStdText();
		((CValidationVariables*)m_pDlg)->m_SelType=Type;
		((CValidationVariables*)m_pDlg)->m_SelGroup=Group;
		((CValidationVariables*)m_pDlg)->m_SelName=Name;
		((CValidationVariables*)m_pDlg)->m_ReadySim=TRUE;

		
		if(Type=="State Variables")
			((CValidationVariables*)m_pDlg)->m_SelTypeInt=1;
		else if(Type=="Flow Variables")
			((CValidationVariables*)m_pDlg)->m_SelTypeInt=2;
		else if(Type=="Auxiliary Variables")
			((CValidationVariables*)m_pDlg)->m_SelTypeInt=3;
		else if(Type=="Driving Variables")
			((CValidationVariables*)m_pDlg)->m_SelTypeInt=4;

		m_pRealTimeCug->OnSetup();
		if(m_PreviousClick>=0) {
			QuickSetBackColor(2,m_PreviousClick, m_ColorHeading);
			QuickSetBackColor(3,m_PreviousClick, m_ColorHeading);
		}
		QuickSetBackColor(2,row, m_ColorChanged);
		QuickSetBackColor(3,row, m_ColorChanged);
		m_PreviousClick=row;
	}
	else {
		GetCell(0,row, &cell);
		fno=MFC_Util::AtoInt(cell.GetStdText());
		((CValidationVariables*)m_pDlg)->m_FileNo=fno;
		GetCell(1,row, &cell);
		vno=MFC_Util::AtoInt(cell.GetStdText())-int(1);
		((CValidationVariables*)m_pDlg)->m_ValVarList=vno;
		if (((CValidationVariables*)m_pDlg)->m_OnlyMeasured&&m_PreviousClick!=row) {
			((CValidationVariables*)m_pDlg)->m_GroupSelIndex.push_back(vno);
		}
		GetCell(2,row, &cell);
		((CValidationVariables*)m_pDlg)->m_ValDesc=cell.GetStdText();
		((CValidationVariables*)m_pDlg)->m_ReadyObs=TRUE;
		m_pRealTimeCug->OnSetup();
		if (((CValidationVariables*)m_pDlg)->m_OnlyMeasured)
		if(m_PreviousClick>=0&&!((CValidationVariables*)m_pDlg)->m_OnlyMeasured) {
			QuickSetBackColor(0,m_PreviousClick , m_ColorHeading);
			QuickSetBackColor(1,m_PreviousClick, m_ColorHeading);
			QuickSetBackColor(2,m_PreviousClick, m_ColorHeading);
			QuickSetBackColor(3,m_PreviousClick, m_ColorHeading);
		}
		QuickSetBackColor(0,row, m_ColorChanged);
		QuickSetBackColor(1,row, m_ColorChanged);
		QuickSetBackColor(2,row, m_ColorChanged);
		QuickSetBackColor(3,row, m_ColorChanged);
		m_PreviousClick=row;
		

	}
	RedrawAll();
	((CValidationVariables*)m_pDlg)->OnInitDialog();
	



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
void CValidTimeCug::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
//		Sent whenever the user double clicks the left mouse button within the grid
//	
//		'col' and 'row' are negative if the area clicked in is not a valid cell
//		'rect' the rectangle of the cell that was clicked in
//		'point' the point where the mouse was clicked
void CValidTimeCug::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	CUGCell cell; 
	int fno, vno;
	string Group, Type, Name;

	if(m_Selected) return;
	if(m_Picker) {
		GetCell(1,row, &cell);
		Group=cell.GetStdText();
		GetCell(0,row, &cell);
		Type=cell.GetStdText();
		GetCell(2,row, &cell);
		Name=cell.GetStdText();
		((CValidationVariables*)m_pDlg)->m_SelType=Type;
		((CValidationVariables*)m_pDlg)->m_SelGroup=Group;
		((CValidationVariables*)m_pDlg)->m_SelName=Name;
		((CValidationVariables*)m_pDlg)->m_ReadySim=TRUE;

		
		if(Type=="State Variables")
			((CValidationVariables*)m_pDlg)->m_SelTypeInt=1;
		else if(Type=="Flow Variables")
			((CValidationVariables*)m_pDlg)->m_SelTypeInt=2;
		else if(Type=="Auxiliary Variables")
			((CValidationVariables*)m_pDlg)->m_SelTypeInt=3;
		else if(Type=="Driving Variables")
			((CValidationVariables*)m_pDlg)->m_SelTypeInt=4;

		m_pRealTimeCug->OnSetup();
		if(m_PreviousClick>=0) {
			QuickSetBackColor(2,m_PreviousClick, m_ColorHeading);
			QuickSetBackColor(3,m_PreviousClick, m_ColorHeading);
		}
		QuickSetBackColor(2,row, m_ColorChanged);
		QuickSetBackColor(3,row, m_ColorChanged);
		m_PreviousClick=row;
	}
	else {
		GetCell(0,row, &cell);
		fno=MFC_Util::AtoInt(cell.GetStdText());
		((CValidationVariables*)m_pDlg)->m_FileNo=fno;
		GetCell(1,row, &cell);
		vno=MFC_Util::AtoInt(cell.GetText());
		((CValidationVariables*)m_pDlg)->m_ValVarList=vno;
		GetCell(2,row, &cell);
		((CValidationVariables*)m_pDlg)->m_ValDesc=cell.GetStdText();
		((CValidationVariables*)m_pDlg)->m_ReadyObs=TRUE;
		m_pRealTimeCug->OnSetup();
		if(m_PreviousClick>=0) {
			QuickSetBackColor(0,m_PreviousClick , m_ColorHeading);
			QuickSetBackColor(1,m_PreviousClick, m_ColorHeading);
			QuickSetBackColor(2,m_PreviousClick, m_ColorHeading);
			QuickSetBackColor(3,m_PreviousClick, m_ColorHeading);
		}
		QuickSetBackColor(0,row, m_ColorChanged);
		QuickSetBackColor(1,row, m_ColorChanged);
		QuickSetBackColor(2,row, m_ColorChanged);
		QuickSetBackColor(3,row, m_ColorChanged);
		m_PreviousClick=row;
		

	}
	((CValidationVariables*)m_pDlg)->OnInitDialog();
}				 

/////////////////////////////////////////////////////////////////////////////
//	OnMouseMove
void CValidTimeCug::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user clicks the left mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void CValidTimeCug::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if ( !updn )
	{
		SelectRange ( col, 0, col, GetNumberRows()-1 );
	}
	else {
			string Group, PrevGroup;
			CUGCell cell; 
			int LastRow, StartRow;
			PrevGroup="a";
			LastRow=0;
			StartRow=0;


			int maxrow=GetNumberRows();
			for (int i=0; i<maxrow;i++) {
					UnJoinCells(0,i);
					UnJoinCells(1,i);
					UnJoinCells(2,i);
			}
			SortBy(col, UG_SORT_ASCENDING); 
			m_ColSort=col;

			int colv=0;
			LastRow=0;
			StartRow=0;

				for (int i=0;i<maxrow;i++) {
					GetCell(colv, i, &cell);	
					Group=cell.GetStdText();
					SetCell(colv,i,&cell);
					if(PrevGroup==Group) {
						LastRow=i;
					}
					else {
						PrevGroup=Group;
						if(LastRow+1>StartRow&&LastRow>0) {
							JoinCells(colv,StartRow,colv,LastRow);
						}
						StartRow=i;
					}
				}
				if(LastRow+1>StartRow) {
					JoinCells(colv,StartRow,colv,LastRow);
				}
				colv=1;
				LastRow=0;
				StartRow=0;
				for (int i=0;i<maxrow;i++) {
					GetCell(colv, i, &cell);	
					Group=cell.GetStdText();
					SetCell(colv,i,&cell);
					if(PrevGroup==Group) {
						LastRow=i;
					}
					else {
						PrevGroup=Group;
						if(LastRow+1>StartRow&&LastRow>0) {
							JoinCells(colv,StartRow,colv,LastRow);
						}
						StartRow=i;
					}
				}
				if(LastRow+1>StartRow) {
					JoinCells(colv,StartRow,colv,LastRow);
				}
				JoinCells(-1,0,-1,maxrow-1);
				RedrawAll(); 


	}

}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_RClicked
//		Sent whenever the user clicks the right mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void CValidTimeCug::OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top heading
//	
//		'col' is negative if the area clicked in is not valid
void CValidTimeCug::OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_LClicked
//		Sent whenever the user clicks the left mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void CValidTimeCug::OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
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
void CValidTimeCug::OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_DClicked
//		Sent whenever the user double clicks the left mouse button within the side heading
//	
//		'row' is negative if the area clicked in is not valid
void CValidTimeCug::OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_LClicked
//		Sent whenever the user clicks the left mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void CValidTimeCug::OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_RClicked
//		Sent whenever the user clicks the right mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void CValidTimeCug::OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top corner button
void CValidTimeCug::OnCB_DClicked(RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyDown
//		Sent whenever the user types when the grid has focus. The keystroke can be
//		modified here as well. (See WM_KEYDOWN for more information)
void CValidTimeCug::OnKeyDown(UINT *vcKey,BOOL processed)
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
void CValidTimeCug::OnCharDown(UINT *vcKey,BOOL processed)
{
}
	
/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent everytime the grid needs to
//		draw a cell in the grid. At this point the cell
//		class has been filled with the information to be
//		used to draw the cell. The information can now be
//		changed before it is used for drawing
void CValidTimeCug::OnGetCell(int col,long row,CUGCell *cell)
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
void CValidTimeCug::OnSetCell(int col,long row,CUGCell *cell)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDataSourceNotify
//		This message is sent from a data source , message
//		depends on the data source - check the information
//		on the data source(s) being used
//		- The ID of the Data source is also returned
void CValidTimeCug::OnDataSourceNotify(int ID,long msg,long param)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellTypeNotify
//		This message is sent from a cell type , message
//		depends on the cell type - check the information
//		on the cell type classes
//		- The ID of the cell type is given
int CValidTimeCug::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditStart
//		This message is sent whenever the grid is ready to start editing a cell
//		A return of TRUE allows the editing a return of FALSE stops editing
//		Plus the properties of the CEdit class can be modified
int CValidTimeCug::OnEditStart(int col, long row,CWnd **edit)
{
	int start;
	start=4;
	if(m_YLabel2!="-")
		start=5;
	
	if(row==0) {
		return TRUE;
	}
	else if(row==2) {
			switch(col) {
				case 0:
					return TRUE;
					break;
				case 1:
					return TRUE;
					break;
				case 2:
					return TRUE;
					break;
			}

	}
	else if(row==3&&start==5) {
			switch(col) {
				case 0:
					return TRUE;
					break;
				case 1:
					return TRUE;
					break;
				case 2:
					return TRUE;
					break;
			}

	}
	else if(row>=start) {
		return TRUE;
	}
	else 
		return FALSE;





	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditVerify
//		This is send when the editing is about to end
int CValidTimeCug::OnEditVerify(int col, long row,CWnd *edit,UINT *vcKey)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int CValidTimeCug::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR stringx,BOOL cancelFlag)
{
	CString str;
	CWinApp* pApp = AfxGetApp();
	edit->GetWindowText(str);

	CUGCell cell; 
	CString  strdef;
	string astr;



	if(!m_Picker) {

		GetCell(0,row, &cell);
		int ichart=int(cell.GetNumber());
			switch(ichart-1) {
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


			MFC_Util::WriteProfileString("Title_"+astr, string(CT2A(str)));
			//((CRealtimedemoDlg*)m_pDlg)->SetChartTitle(ichart-1, str);
	
	}


	


	QuickSetText(col,row,str);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int CValidTimeCug::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	sections - UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//				UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
void CValidTimeCug::OnMenuCommand(int col,long row,int section,int item)
{
	

}

/////////////////////////////////////////////////////////////////////////////
//	return UG_SUCCESS to allow the menu to appear
//	return 1 to not allow the menu to appear
int CValidTimeCug::OnMenuStart(int col,long row,int section)
{


	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHint
int CValidTimeCug::OnHint(int col,long row,int section,CString *string)
{

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnVScrollHint
int CValidTimeCug::OnVScrollHint(long row,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHScrollHint
int CValidTimeCug::OnHScrollHint(int col,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OLE
#ifdef __AFXOLE_H__

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  CValidTimeCug::OnDragEnter(COleDataObject* pDataObject){
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  CValidTimeCug::OnDragOver(COleDataObject* pDataObject,int col,long row){
	//return DROPEFFECT_NONE;
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  CValidTimeCug::OnDragDrop(COleDataObject* pDataObject,int col,long row){
	return DROPEFFECT_COPY;
}
#endif

/////////////////////////////////////////////////////////////////////////////
//	
void CValidTimeCug::OnScreenDCSetup(CDC *dc,int section){
}

/////////////////////////////////////////////////////////////////////////////
//	OnSortEvaluate
//	return		-1  <
//				0	==
//				1	>
int CValidTimeCug::OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags)
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
void CValidTimeCug::OnTabSelected(int ID)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnAdjustComponentSizes
void CValidTimeCug::OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,
		RECT *cnrBtn,RECT *vScroll,RECT *hScroll,RECT *tabs)
{
} 

/////////////////////////////////////////////////////////////////////////////
//	OnDrawFocusRect
void CValidTimeCug::OnDrawFocusRect(CDC *dc,RECT *rect)
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

void CValidTimeCug::DrawExcelFocusRect(CDC *dc,RECT *rect)
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
COLORREF CValidTimeCug::OnGetDefBackColor(int section)
{
	return GetSysColor ( COLOR_APPWORKSPACE );
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetFocus
void CValidTimeCug::OnSetFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKillFocus
void CValidTimeCug::OnKillFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapStart
BOOL CValidTimeCug::OnColSwapStart(int col)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanColSwap
BOOL CValidTimeCug::OnCanColSwap(int fromCol,int toCol)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTrackingWindowMoved
void CValidTimeCug::OnTrackingWindowMoved(RECT *origRect,RECT *newRect)
{
}
BOOL CValidTimeCug::SetColorBackground(int color)
{
	int colorH;
	colorH=RGB(GetBValue(color),GetGValue(color),GetRValue(color));
	MFC_Util::WriteProfileInt( "ColorBackground", color);
	MFC_Util::WriteProfileInt( "HColorBackground", colorH);
	return TRUE;
}







BOOL CValidTimeCug::SetTitle(int) {

	return TRUE;
}