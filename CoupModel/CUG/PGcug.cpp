/////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived PGCug

#include "..\stdafx.h"
#include "..\Util.h"
#include "..\resource.h"
#include "PGCug.h"
#include "..\PG\CreatePG.h"
#include "..\PG\CalculateVariable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(PGCug,CUGCtrl)
	//{{AFX_MSG_MAP(PGCug)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// PGCug construction/destruction
PGCug::PGCug()
{
//	UGXPThemes::UseThemes(true);
	m_bDrawExcelFocus = TRUE;
	m_col=1;
	m_row=1;
	m_Data=FALSE;
	SetMultiSelectMode ( TRUE );
	DragDropTarget ( TRUE );
	m_Changed=false;
	m_ShowMean=0;
	m_pPG=NULL;
	m_pCUG=NULL;
	m_pDlg=NULL;
	m_font.CreateFont(14,0,0,0,500,0,0,0,0,0,0,0,0,"Arial");
	m_ColorHeading=RGB(230,230,230);
	m_ColorClick=RGB(240,240,50);
	m_ColorSelected=RGB(255,200,200);
	m_ColorChanged=RGB(255,200,200);
	m_NoColor=RGB(255,255,255);
	m_Wide=FALSE;
	m_Repit=1;
	m_ShowMaxRep=1;


}

void PGCug::SetpCUG(void *pCUG)
{
	m_pCUG=pCUG;

}
void PGCug::SetpDlg(CDialog *pDlg)
{
	m_pDlg=pDlg;

}
void PGCug::SetpPG(CPG *pPG)
{
	m_pPG=pPG;
}
PGCug::~PGCug()
{
	m_font.DeleteObject();
	//UGXPThemes::CleanUp();
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid

void PGCug::OnUpdate(int col, long row, BOOL data, BOOL wide, long repit)
{
	m_col=col;
	if(!data&&repit>1) {
		row=row*repit;


	}
	m_row=row;
	m_Data=data;
	m_Wide=wide;
	m_Repit=repit;

	
	EnableUpdate(FALSE); 
	while(GetNumberRows()>row)
		DeleteRow(0);
	EnableUpdate(TRUE); 
	CPG *pPG;
	pPG=(CPG*)m_pPG;

	if(pPG!=NULL&&data) {
		if(pPG->GetNumBuffers()>1) {


		}
		else if(!(pPG->GetNumRepititions()==repit&&pPG->GetNumRecords()==row&&pPG->GetNumVariables()==col-1))
			pPG->AdjustSize(row,col-1, repit);
		if(m_Repit>1)
			pPG->SetRepititionMode(true, m_Repit);
		else
			pPG->SetRepititionMode(false, 1);
		Make_DataHeader();
	  }
	else
		Make_DescriptionHeader();

}
void PGCug::UpdateGridSize(int col, long row, BOOL data, int repit)
{
	m_col=col;
	m_row=row;
	long actualrow,actualcol;
	EnableUpdate(FALSE); 
	CString str;
	CPG *pPG;
	pPG=(CPG*)m_pPG;

	if(pPG!=NULL&&data) {
		
		if(!pPG->GetCompleteRead()) {
			pPG->SetCompleteRead(true);

		}
		pPG->AdjustSize(row,col-1, repit);
		if(pPG->GetCompleteRead())
			pPG->SetActBufferSize(row);

		ReDisplay();

	}
	else if(data) {
	// Update data sheet
			while(GetNumberRows()>row) {
				actualrow=GetNumberRows()-1;
				DeleteRow(actualrow);
			}
			while(GetNumberRows()<row) {
				actualrow=GetNumberRows()-1;
				AppendRow();
				QuickSetNumber(-1,actualrow+1,actualrow+2);
			}
			while(GetNumberCols()>col) {
				actualcol=GetNumberCols()-1;
				DeleteCol(actualcol);
			}
			while(GetNumberCols()<col) {
				actualcol=GetNumberCols()-1;
				AppendCol();

				str="Var : "+MFC_Util::ItoAscii(actualcol+1);
				QuickSetText(actualcol+1,-1,str);

			}
		}	
		else if(pPG==NULL) {
	// Update description sheet
			while(GetNumberRows()>row)  {
				actualrow=GetNumberRows()-1;
				DeleteRow(actualrow);
			}
			while(GetNumberRows()<row) {
				actualrow=GetNumberRows()-1;
				AppendRow();
				QuickSetNumber(-1,actualrow+1,actualrow+2);
			}

		}
	
	EnableUpdate(TRUE); 


}
BOOL PGCug::OnReadTime(CPG *pCPG)
{

	int m_NumVariables;
	CUGCell cell;
	if(m_Data) {


		size_t rec_start, rec_end;
		if(!pCPG->IsGood())
				pCPG->ClearFile();
			if(pCPG->GetNumBuffers()>1) {
				if(pCPG->GetPrevBuffer()==0) {
					rec_start=0;
					rec_end=pCPG->GetNumRecordsBufSize();
				}
				else {
					rec_start=(pCPG->GetPrevBuffer()-1)*pCPG->GetNumRecordsBufSize();
					rec_end=rec_start+pCPG->GetCurrentRecords();
					//if(rec_end>pCPG->GetNumRecords())
					//	rec_end=pCPG->GetNumRecords();
				}
			}
			else {
				rec_start=0;
				rec_end=m_row;
			}
			size_t irow=0;

		for (int ii=rec_start;ii<rec_end;ii++) {
			QuickSetText(0,irow,pCPG->GetDate(ii+1));
			irow++;
		}
		m_Changed=true;
	}
	else {
		m_NumVariables=pCPG->GetNumVariables();

		for (int i=0;i<m_NumVariables;i++) {
				GetCell(0,i,&cell);
				cell.SetCellType(UGCT_CHECKBOX);
				bool testV=pCPG->GetToBeSaved(i);
				if(testV)
					cell.SetBool(TRUE);
				else
					cell.SetBool(FALSE);			
				SetCell(0,i,&cell);
				
				QuickSetText(1,i,pCPG->GetVarName(i+1));
				QuickSetText(2,i,pCPG->GetVarUnit(i+1));
				QuickSetText(3,i,pCPG->GetVarId(i+1));
				QuickSetText(4,i,pCPG->GetVarPos(i+1));
				//QuickSetNumber(4,i,pCPG->GetVarMin(i+1));
				//QuickSetNumber(5,i,pCPG->GetVarMax(i+1));
		}
	}
	RedrawAll();
	return TRUE;

}
bool PGCug::OnReadFileDataToGrid(CPG *pCPG)
{
	long m_NumRecords;
	int m_NumVariables;

	CUGCell cell;
	m_pPG=pCPG;
	EnableUpdate(FALSE); 
	if(!m_Data) return false;
	
	 if(m_ShowMean==0) {
			m_NumVariables=m_col-1;
			if(m_Repit>1)
				m_NumVariables=m_NumVariables/m_Repit;
			m_NumRecords=m_row;
		

		if(!pCPG->GetCompleteRead()&&!pCPG->IsGood())
				pCPG->ReOpen();

		size_t rec_start, rec_end;
		if(!pCPG->IsGood()&&pCPG->IsOpen()) {
				pCPG->ClearFile();
				pCPG->ReOpen();


		}
			if(pCPG->GetNumBuffers()>1) {
				if(pCPG->GetPrevBuffer()==0) {
					rec_start=0;
					rec_end=pCPG->GetNumRecordsBufSize();
				}
				else {
					rec_start=(pCPG->GetPrevBuffer()-1)*pCPG->GetNumRecordsBufSize();
					rec_end=rec_start+pCPG->GetCurrentRecords();
					//if(rec_end>pCPG->GetNumRecords())
					//	rec_end=pCPG->GetNumRecords();
				}
			}
			else {
				rec_start=0;
				rec_end=m_NumRecords;
			}
			size_t irow=0, ivarno;
			int icol;
			for( size_t nrep=0;nrep<m_Repit;nrep++) {
				if(m_Repit>1) {
					if(!pCPG->SetRepititionSequence(nrep+1, false))
						return FALSE;
					//pCPG->SetActBufferReset();
					irow=0;
				}

				for (int ii=rec_start;ii<rec_end;ii++) {
					if(nrep==0) QuickSetText(0,irow,pCPG->GetDate(ii+1));
					for( size_t ind=0; ind<pCPG->GetNumToView();ind++) {
						if(pCPG->GetViewRepetition(ind)==nrep+1) {
							ivarno=pCPG->GetViewVariable(ind);
							icol=ind+1;
							float v=pCPG->GetVarValue(ivarno,ii+1);
							if(v>-0.8E38){
								GetCell(icol,irow,&cell);
								int alin=cell.GetAlignment();
								cell.SetNumber(v);
								alin=cell.GetAlignment();
								//cell.SetAlignment(UG_RIGHT);
								SetCell(icol,irow,&cell);
							}
							else 
								QuickSetText(icol,irow,"");
						
						}
					}
					irow++;
				}
			}
		}
			
		else if (m_ShowMean==1) {
			//m_NumRecords=pPG->GetNumRecordsYear(1);;

			m_NumRecords=pCPG->GetNumRecordsYear(1);
			m_NumVariables=m_col-1;

			for (int ii=0;ii<m_NumRecords;ii++) {
				QuickSetText(0,ii,pCPG->GetDate(ii+1));
				for (int i=0;i<m_NumVariables;i++) {
					float v=pCPG->GetVarValue(i+1,ii+1);
					if(v>-0.8E38)
						QuickSetNumber(i+1,ii,v);
					else {
						//GetCell(i+1,ii,&cell);
						//cell.SetText("xxx");
						//cell.SetParam(2000);
						//SetCell(i+1,ii,&cell);
					}
				}		
			}
		}
		else if (m_ShowMean==2) {



		}
		else if(m_ShowMean==3) {




		}
		else if(m_ShowMean==4) {




		}
		else if(m_ShowMean==5) {




		}



	//RedrawAll();
	EnableUpdate(TRUE);
	RedrawAll();
	return TRUE;

}

bool PGCug::OnReadFileDescriptionToGrid(CPG *pCPG)
{

	int m_NumVariables;

	CUGCell cell;
	m_pPG=pCPG;
	EnableUpdate(FALSE); 
	if(m_Data) return false;
	
	m_NumVariables=pCPG->GetNumVariables();
	m_Repit=pCPG->GetNumRepititions();
	size_t ind;
		for (int nrep=0; nrep<m_Repit; nrep++) {
			for (int i=0;i<m_NumVariables;i++) {
					ind=nrep*m_NumVariables+i;
					GetCell(0,ind,&cell);
					cell.SetCellType(UGCT_CHECKBOX);
					bool testV=pCPG->GetToBeSaved(i);
					if(testV)
						cell.SetBool(TRUE);
					else
						cell.SetBool(FALSE);			
					SetCell(0,ind,&cell);

					GetCell(1,ind,&cell);
					cell.SetCellType(UGCT_CHECKBOX);
					
					testV=pCPG->GetToBeViewed(i);
					if(testV)
						cell.SetBool(TRUE);
					else
						cell.SetBool(FALSE);			
					SetCell(1,ind,&cell);


					QuickSetText(2,ind,pCPG->GetVarName(i+1, nrep+1));
					QuickSetText(3,ind,pCPG->GetVarUnit(i+1, nrep+1));
					QuickSetText(4,ind,pCPG->GetVarId(i+1, nrep+1));
					QuickSetText(5,ind,pCPG->GetVarPos(i+1, nrep+1));
					float v;
					v=pCPG->GetVarMin(i+1, nrep+1);
					if(v<0.5E38)	QuickSetNumber(6,ind,v);
					v=pCPG->GetVarMax(i+1, nrep+1);
					if(v>-0.5E37)	QuickSetNumber(7,ind,v);
					QuickSetNumber(8,ind,pCPG->GetNumberofMissing(ind+1));
					if(pCPG->IsAddInfoSet()) {
						QuickSetText(9,ind,pCPG->GetVarCountry(i+1, nrep+1));
						QuickSetText(10,ind,pCPG->GetVarStation(i+1, nrep+1));
						QuickSetNumber(11,ind,pCPG->GetVarAlt(i+1, nrep+1));
						QuickSetNumber(12,ind,pCPG->GetVarLat(i+1, nrep+1));
						QuickSetNumber(13,ind,pCPG->GetVarLong(i+1, nrep+1));
					}
			}
		}

	EnableUpdate(TRUE);
	RedrawAll();
	return true;

}

bool PGCug::OnWriteGridDataToFile(CPG *pCPG)
{	// Read Information from the grid cell and write the values into the PG File
	long m_NumRecords;
	int m_NumVariables;
	CUGCell cell;
	float v;
	m_pPG=pCPG;
	
	
		//pCPG->SetNumRecords(m_row);
	if(m_Data&&m_TabSelected==0) {
		m_row=GetNumberRows();
		m_col=GetNumberCols();
		m_Repit=m_pPG->GetNumRepititions();
		m_NumVariables=(m_col-1)/m_Repit;

		//pCPG->ReAllocate(m_row,m_col-1);
		m_NumRecords=m_row;
		long size=m_row*m_NumVariables;

		m_NumRecords=m_row;
		if(m_NumRecords<=0) return FALSE;
		size_t rec_start, rec_end;	

		if(!pCPG->m_WriteEnabled) {
			pCPG->CloseFile();
			pCPG->ReOpen(true);
		}
		


		bool status;

		for (long irep=0; irep<m_Repit;irep++) {
			status=pCPG->SetRepititionSequence(irep+1, true);
		
			if(pCPG->GetNumBuffers()>1) {
				if(pCPG->GetPrevBuffer()==0) {
					rec_start=0;
					rec_end=pCPG->GetNumRecordsBufSize();
				}
				else {
					rec_start=(pCPG->GetPrevBuffer()-1)*pCPG->GetNumRecordsBufSize();
					rec_end=pCPG->GetPrevBuffer()*pCPG->GetNumRecordsBufSize();
					if(rec_end>pCPG->GetNumRecords())
						rec_end=pCPG->GetNumRecords();
				}
			}
			else {
				rec_start=0;
				rec_end=m_NumRecords;
			}		
			size_t irow=0;
			for (int ii=rec_start;ii<rec_end;ii++) {
				GetCell(0,irow,&cell);
				int ich=cell.GetTextLength();
				if(ich>0&&irep==0)
					pCPG->SetDate(ii+1,cell.GetText());
				for(size_t ind=0;ind<pCPG->GetNumToView();ind++) {
					if(pCPG->GetViewRepetition(ind)==irep+1) {
						int ivarno=pCPG->GetViewVariable(ind);
						int icol=ind+1;
						GetCell(icol,irow,&cell);
						ich=cell.GetTextLength();
						if(cell.GetBackColor()==m_ColorChanged) {

							v=cell.GetNumber();
							cell.SetBackColor(m_NoColor);	
							SetCell(icol,irow, &cell);
							if(ich>=0) {
								v=cell.GetNumber();
								pCPG->SetVarValue(ivarno,ii+1,v);
							}
							else if(ich<0)
								pCPG->SetVarValue(ivarno,ii+1, MISSING);
						}
						/*else if(ich<0)
							pCPG->SetVarValue(ivarno,ii+1, MISSING);*/
						;
					}

				}
				irow++;
			}
			if(pCPG->GetCompleteRead()) {
				pCPG->CloseFile();
				pCPG->ReOpen(true);
				if(pCPG->WritePGFile())
					pCPG->CloseFile();

			}
			else {
				if(pCPG->GetNumBuffers()>1) {
	//			Make sure last Block is saved to file

					if(!pCPG->WritePGFileRecord(rec_end)) {
						pCPG->CloseFile();
						pCPG->ReOpen(true);
						pCPG->WritePGFileRecord(rec_end);

					}
				}
				else {
					if(pCPG->GetNumRepititions()>1)
						status=pCPG->WriteNewBuffer(irep+1);
						
					if(!status) return false;
				}
				if(irep+1==m_Repit)
				pCPG->WritePGFileStartHeader(pCPG->GetNumRecords(), m_Repit);

				pCPG->ReCalculateMinMax(irep+1);
			}
			if(m_Repit>1) {


			}
		}


	}
	else
		return false;


	return true;

}
bool PGCug::OnWriteGridDescriptionToFile(CPG *pCPG)
{	// Read Information from the grid cell and write the values into the PG File
	long m_NumRecords;
	int m_NumVariables;
	CUGCell cell;
	float v;
	m_pPG=pCPG;
	
	
		//pCPG->SetNumRecords(m_row);
	if(m_Data&&m_TabSelected==0) {
		return false;
	}
	else if(!m_Data) {
		m_row=GetNumberRows();
		m_col=GetNumberCols();
		m_NumVariables=m_row;
		if(m_NumVariables<=0) return FALSE;
		if(!pCPG->IsAddInfoSet())
			pCPG->SetInfoMode();
		long line=0;
		for (int irep=0; irep<m_Repit; irep++) {
			for (int i=0;i<pCPG->GetNumVariables();i++) {
				line=irep*pCPG->GetNumVariables()+i;
				GetCell(2,line,&cell);
				CString str=cell.GetText();
				pCPG->SetVarName(i+1,string(str), irep+1);
				GetCell(3,line,&cell);
				str=cell.GetText();
				pCPG->SetVarUnit(i+1,string(str), irep+1);
				GetCell(4,line,&cell);
				str=cell.GetText();
				pCPG->SetVarId(i+1,string(str), irep+1);
				GetCell(5,line,&cell);
				str=cell.GetText();
				pCPG->SetVarPos(i+1,string(str), irep+1);
				if(line<m_row) {
					GetCell(9,line,&cell);
					str=cell.GetText();
					pCPG->SetVarCountry(i+1,string(str), irep+1);
				
					GetCell(10,line,&cell);
					str=cell.GetText();
					pCPG->SetVarStation(i+1,string(str), irep+1);
								
					GetCell(11,line,&cell);
					double f=cell.GetNumber();
					pCPG->SetVarAlt(i+1,f, irep+1);
				
					GetCell(12,line,&cell);
					f=cell.GetNumber();
					pCPG->SetVarLat(i+1,f, irep+1);
				
					GetCell(13,line,&cell);
					f=cell.GetNumber();
					pCPG->SetVarLong(i+1,f, irep+1);
				}

			}
		}
	}

	return true;

}
void PGCug::OnSetup()
{
	EnableMenu(TRUE); 
	EnableJoins(TRUE); 
	EnableUpdate(TRUE);
	UseHints(TRUE);

	SetDefFont(&m_font);

	int num_sheets=GetNumberSheets();
	for (long i=0;i<num_sheets;i++) {
		DeleteTab(i);
	}
	SetDefColWidth(130);
	if(m_Data) {
//******* Add New Tabs 
		AddTab("Actual Data", 0); 
		AddTab("Processed Data", 1); 
		SetTabWidth(300); 
		SetNumberSheets(2); 
		OnSheetSetup(0);
		SetCurrentTab(0);
		

	}
	else
		OnSheetSetup(0);

}

/////////////////////////////////////////////////////////////////////////////
//	OnSheetSetup	
void PGCug::Make_DataHeader()
{
	CPG *pCPG=m_pPG;
	if(pCPG==NULL) return;
	m_Repit=pCPG->GetNumRepititions();

	if(m_Repit>0)
		pCPG->SetRepititionMode(true, m_Repit);

	m_col=pCPG->GetNumToView()+1;
	if(m_col==1) {
		pCPG->UpdateViewOrder();
		m_col=pCPG->GetNumToView()+1;
	}

	if(m_col==0) {
		m_col=pCPG->GetNumVariables()*pCPG->GetNumRepititions()+1;
		size_t varrep=pCPG->GetNumRepititions()-1;
		varrep--;
		while(m_col>10000) {
			m_col=pCPG->GetNumVariables()*varrep+1;
			m_ShowMaxRep=varrep;
			varrep--;
		}
		m_ShowMaxRep=varrep+1;
	}
	else
		m_ShowMaxRep=1; // Preliminary set


		

	int cols = m_col,
		rows = m_row;
	CUGCell cell;
	EnableUpdate(FALSE);

	m_NumVar=pCPG->GetNumVariables();
	while(GetNumberRows()>rows)
		DeleteRow(0);
	EnableUpdate(TRUE);
	
	SetNumberCols ( cols );
	SetNumberRows ( rows );
		if(m_Repit>1) {
			SetRowHeight(-1,40);
			SetTH_NumberRows(2);
			SetTH_RowHeight(-1,20);
			SetTH_RowHeight(-2,20);
		}
		else {
			SetTH_NumberRows(1);
			SetRowHeight(-1,20);
		}
	
	GetColDefault(0,&cell);
	
	int alin=cell.GetAlignment();
		cell.SetAlignment(UG_ALIGNLEFT);

	alin=cell.GetAlignment();
	GetColDefault(0,&cell);
	alin=cell.GetAlignment();
	cell.SetAlignment(UG_ALIGNLEFT);
	GetColDefault(0,&cell);
	alin=cell.GetAlignment();
	cell.SetAlignment(UG_ALIGNRIGHT);
	GetColDefault(0,&cell);
	alin=cell.GetAlignment();
	for(int i=0;i<cols;i++) {
		GetColDefault(i,&cell);
		alin=cell.GetAlignment();
			if(i==0) cell.SetAlignment(UG_ALIGNRIGHT);
		else
			cell.SetAlignment(UG_ALIGNRIGHT);
		SetColDefault(i,&cell);
		alin=cell.GetAlignment();
	}


	SetVScrollMode ( UG_SCROLLTRACKING );

	SetUniformRowHeight( TRUE );
	CString str;
	vector<size_t> joinv;
	if(m_Data&&cols>1) {
		SetColWidth(0,100);
		if(m_Repit>1) {
			QuickSetText(0,-2,"Date");
			QuickSetText(0,-1,"Date");
		}
		
		
		size_t lastrep=0;
		if(pCPG->GetViewOrder()) {
			for(size_t i=0;i<pCPG->GetNumToView();i++) {

				SetColWidth(i+1,70);
				if(m_Repit>1) {
					str="R:"+MFC_Util::ItoAscii(pCPG->GetViewRepetition(i));
					string s;
					s=pCPG->GetVarId(pCPG->GetViewVariable(i), pCPG->GetViewRepetition(i));
					FUtil::trim(s);
					str+="-";str+=s.c_str();
					QuickSetText(i+1,-1,str);
				}
				str="V:"+MFC_Util::ItoAscii(pCPG->GetViewVariable(i));
				string s;
				if(lastrep!=pCPG->GetViewVariable(i)) {
					joinv.push_back(i+1);
					lastrep=pCPG->GetViewVariable(i);
				}
				s=pCPG->GetVarName(pCPG->GetViewVariable(i), pCPG->GetViewRepetition(i));	FUtil::trim(s); str+="-";str+=s.c_str();
				QuickSetText(i+1,-2,str);
				if(m_pPG->GetToBeCalc(i))
					QuickSetBackColor(i+1,-1,m_ColorClick);
				else
					QuickSetBackColor(i+1,-1,m_ColorHeading);
			}
		}
		else {
			for(size_t i=0;i<pCPG->GetNumToView();i++) {
				SetColWidth(i+1,70);

				if(lastrep!=pCPG->GetViewRepetition(i)) {
					joinv.push_back(i+1);
					lastrep=pCPG->GetViewRepetition(i);
				}

				if(m_Repit>1) {
					str="R:"+MFC_Util::ItoAscii(pCPG->GetViewRepetition(i));
					QuickSetText(i+1,-2,str);
				}
				str="V:"+MFC_Util::ItoAscii(pCPG->GetViewVariable(i));
				string s; s=pCPG->GetVarName(pCPG->GetViewVariable(i), pCPG->GetViewRepetition(i));	FUtil::trim(s); str+="-"; str+=s.c_str();
				QuickSetText(i+1,-1,str);

				if(m_pPG->GetToBeCalc(i))
					QuickSetBackColor(i+1,-1,m_ColorClick);
				else
					QuickSetBackColor(i+1,-1,m_ColorHeading);
			}
		}
		if(joinv[joinv.size()-1]!=pCPG->GetNumToView()) {
			joinv.push_back(pCPG->GetNumToView()+1);
		}
	}

	size_t rec_start, rec_end;
	if(pCPG->GetNumBuffers()>1) {
		if(pCPG->GetPrevBuffer()==0) {
			rec_start=0;
			rec_end=pCPG->GetNumRecordsBufSize();
		}
		else {
			rec_start=(pCPG->GetPrevBuffer()-1)*pCPG->GetNumRecordsBufSize();
			rec_end=rec_start+pCPG->GetCurrentRecords();
			//if(rec_end>pCPG->GetNumRecords())
			//	rec_end=pCPG->GetNumRecords();
		}
	}
	else {
		rec_start=0;
		rec_end=pCPG->GetNumRecords();
	}
	long irow=0;
	for(size_t i=rec_start;i<rec_end;i++) { 
		
		/*if(m_Repit>1) {
			QuickSetNumber(-1,i,i%m_NumVar+1);
			str=ItoAscii((i+1)/m_NumVar+1);
			QuickSetText(-2, i, str);
		}
		else*/
		QuickSetNumber(-1,irow,i+1);
		irow++;
	}
	if(m_Repit>1) {
			if(joinv.size()>1) {	
				for(int i=0; i<joinv.size()-1;i++) 
					JoinCells(joinv[i],-2,joinv[i+1]-1,-2);
			}
			else {
				for(size_t i=0;i<pCPG->GetNumToView();i++) 
					JoinCells(i+1,-2,i+1,-1);
			}

	}
	else {
		for(size_t i=0;i<pCPG->GetNumToView();i++) 
			JoinCells(i+1,-2,i+1,-1);

	}

		JoinCells(0,-2,0,-1);
	

}



void PGCug::Make_DescriptionHeader()
{
	CPG *pCPG=m_pPG;
	if(pCPG==NULL) return;
	m_Repit=pCPG->GetNumRepititions();

	if(m_Repit>0)
		pCPG->SetRepititionMode(true, m_Repit);


	if(m_Repit>1||pCPG->IsAddInfoSet())
		m_col=14;
	else
		m_col=9;

	int cols = m_col,
		rows = m_row;
	CUGCell cell;
	EnableUpdate(FALSE);

	m_NumVar=pCPG->GetNumVariables();
	while(GetNumberRows()>rows)
		DeleteRow(0);
	EnableUpdate(TRUE);
	
	SetNumberCols ( cols );
	SetNumberRows ( rows );

	if(m_Repit>1) {
		SetColWidth(-1,50);
		SetSH_NumberCols(2);
		SetSH_ColWidth(-1,30);
		SetSH_ColWidth(-2,20);
	}	
	else {
		SetSH_NumberCols(1);
		SetColWidth(-1,30);
	}

	GetColDefault(0,&cell);
	cell.SetAlignment(UG_ALIGNRIGHT);
	for(int i=0;i<cols;i++) {
		if(m_Data||i>3)
		SetColDefault(i,&cell);
	}


	SetVScrollMode ( UG_SCROLLTRACKING );

	SetUniformRowHeight( TRUE );
	CString str;
	if(cols>3) {
		SetColWidth(-1,30);
		if(m_Wide) {
			SetColWidth(2,200);
			SetColWidth(3,80);
			SetColWidth(4,190);

		}
		else {
			SetColWidth(2,155);
			SetColWidth(3,40);
			SetColWidth(4,155);
		}
		SetColWidth(0,40);
		SetColWidth(1,30);
		SetColWidth(5,40);
		SetColWidth(6,70);
		SetColWidth(7,70);
		SetColWidth(8,70);
		SetColWidth(9,70);
		SetColWidth(10,150);
		for(int i=0; i<3;i++)
			SetColWidth(11+i, 80);
		QuickSetText(0,-1,"Save");
		QuickSetText(1,-1,"View");
		QuickSetText(2,-1,"Variable Name");
		QuickSetText(3,-1,"Unit");
		QuickSetText(4,-1,"Id");
		QuickSetText(5,-1,"Pos");
		QuickSetText(6,-1,"Min");
		QuickSetText(7,-1,"Max");
		QuickSetText(8,-1,"Missings");
		QuickSetText(9,-1,"Country");
		QuickSetText(10,-1,"Station");
		QuickSetText(11,-1,"Altitude");
		QuickSetText(12,-1,"Latitude");
		QuickSetText(13,-1,"Longitude");
	}

		if(m_Repit>1) {
			for(int i=0; i<m_Repit;i++) {
				JoinCells(-2,i*m_NumVar,-2,(i+1)*m_NumVar-1);
			}

			for(int i=0;i<rows;i++) { 
				QuickSetNumber(-1,i,i%m_NumVar+1);
				str=MFC_Util::ItoAscii((i+1)/m_NumVar+1);
				QuickSetText(-2, i, str);
			}

		}
		else {
			for(int i=0;i<m_NumVar;i++) {
				UnJoinCells(-2,i);
				UnJoinCells(i,-2);
			}
			for(int i=0;i<rows;i++) 
				QuickSetNumber(-1,i,i+1);
		}


}
void PGCug::Make_MeanValueSheets(int sheet)
{
	int cols;
	long rows;
	CUGCell cell;
	CPG *pPG;
	pPG=(CPG*)m_pPG;
	SetSheetNumber(1,FALSE);
	while(GetNumberRows()>0)
		DeleteRow(0);
	if(m_pPG==NULL) return;
	cols=pPG->GetNumVariables()+1;

	SetNumberCols ( cols );

	switch(m_ShowMean) {
	case 0:
		rows=0;
		break;
	case 1:
		rows=pPG->GetNumRecordsYear(1);
		break;
	case 2:
		rows=pPG->GetNumRecordsMonth(1);
		break;
	case 3:
		rows=pPG->GetNumRecordsDays(1);
		break;
	case 4:
		rows=pPG->GetNumRecordsHour(1);
		break;
	case 5:
		rows=pPG->GetNumRecordsMinute(10);
		break;
	}
	SetNumberRows ( rows );

	SetDefFont(&m_font);
	GetColDefault(0,&cell);
	cell.SetFont(&m_font);
	cell.SetAlignment(UG_ALIGNRIGHT);
	for(int i=0;i<cols;i++)
		SetColDefault(i,&cell);
	EnableMenu(TRUE); 
	SetVScrollMode ( UG_SCROLLTRACKING );
	SetUniformRowHeight( TRUE );
		QuickSetText(0,-1,"Date");
		CString str;
		SetColWidth(0,100);
		for(int i=1;i<cols;i++) {
			SetColWidth(i,70);
			str="Var : "+MFC_Util::ItoAscii(i);
			QuickSetText(i,-1,str);
		}
		switch(m_ShowMean) {
			case 0:
				rows=0;
				break;
			case 1:
				for(int ii=0;ii<rows;ii++) {
					QuickSetNumber(-1,ii,ii+1);
					QuickSetText(0,ii,pPG->GetVarDateYearsMean(ii+1,1));
					for (int i=0;i<cols-1;i++) {
							float v=pPG->GetVarValueYearsMean(i+1,ii+1,1);
							if(v>-0.8E38)
								QuickSetNumber(i+1,ii,v);
					}
				}


				break;
			case 2:
				for(int ii=0;ii<rows;ii++) {
					QuickSetNumber(-1,ii,ii+1);
					QuickSetText(0,ii,pPG->GetVarDateMonthsMean(ii+1,1));
					for (int i=0;i<cols-1;i++) {
							float v=pPG->GetVarValueMonthsMean(i+1,ii+1,1);
							if(v>-0.8E38)
								QuickSetNumber(i+1,ii,v);
					}
				}

				
				break;
			case 3:
				for(int ii=0;ii<rows;ii++) {
			QuickSetNumber(-1,ii,ii+1);
			QuickSetText(0,ii,pPG->GetVarDateDaysMean(ii+1,1));
			for (int i=0;i<cols-1;i++) {
					float v=pPG->GetVarValueDaysMean(i+1,ii+1,1);
					if(v>-0.8E38)
						QuickSetNumber(i+1,ii,v);
			}
		}


				break;
			case 4:
				for(int ii=0;ii<rows;ii++) {
					QuickSetNumber(-1,ii,ii+1);
					QuickSetText(0,ii,pPG->GetVarDateHourMean(ii+1,1));
					for (int i=0;i<cols-1;i++) {
							float v=pPG->GetVarValueHourMean(i+1,ii+1,1);
							if(v>-0.8E38)
								QuickSetNumber(i+1,ii,v);
					}
				}


				break;
			case 5:
				for(int ii=0;ii<rows;ii++) {
					QuickSetNumber(-1,ii,ii+1);
					QuickSetText(0,ii,pPG->GetVarDateMinuteMean(ii+1,1));
					for (int i=0;i<cols-1;i++) {
							float v=pPG->GetVarValueMinuteMean(i+1,ii+1,1);
							if(v>-0.8E38)
								QuickSetNumber(i+1,ii,v);
					}
				}


				break;
			}



}

void PGCug::OnSheetSetup(int sheetNumber)
{
switch(sheetNumber) {
			case(0): //Run Options
				if(m_Data) {
					Make_DataHeader();


				}
				else {
					Make_DescriptionHeader();

				}
				RedrawAll();
				break;
			case (1): //Switches
				Make_MeanValueSheets(1);
				RedrawAll(); 
				break;
}
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the current cell in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int PGCug::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the top row or left column in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int PGCug::OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	
void PGCug::OnHitBottom(long numrows,long rowspast,long rowsfound)
{
}

/////////////////////////////////////////////////////////////////////////////
//	
void PGCug::OnHitTop(long numrows,long rowspast)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeCol
//		Sent when the user is over a separation line on the top heading
//		A return value of TRUE allows the possibiliy of a resize
int PGCug::OnCanSizeCol(int col)
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
void PGCug::OnColSizing(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSized
//		This is sent when the user finished sizing the 
//		given column (see above for more details)
void PGCug::OnColSized(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeRow
//		Sent when the user is over a separation line on the side heading
//		A return value of TRUE allows the possibiliy of a resize
int  PGCug::OnCanSizeRow(long row)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSizing
//	    Sent when the user is sizing a row The row that is being sized is given as 
//		well as the height. Plus the height can be modified 
//		at this point. This makes it easy to set min and max heights
void PGCug::OnRowSizing(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSized
//		This is sent when the user is finished sizing hte
//		given row ( see above for more details)
void PGCug::OnRowSized(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeSideHdg
//		This is sent when the user moves into position 
//		for sizing the width of the side heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int PGCug::OnCanSizeSideHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeTopHdg
//		This is sent when the user moves into position 
//		for sizing the height of the top heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int PGCug::OnCanSizeTopHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSizing
int PGCug::OnSideHdgSizing(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSized
int PGCug::OnSideHdgSized(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSized
int PGCug::OnTopHdgSized(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSizing
int PGCug::OnTopHdgSizing(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColChange
//		Sent whenever the current column changes
//		The old and the new columns are given
void PGCug::OnColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowChange
//		Sent whenever the current row changes
//		The old and the new rows are given
void PGCug::OnRowChange(long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellChange
//		Sent whenever the current cell changes rows or columns
void PGCug::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnLeftColChange
//		Sent whenever the left visible column in the grid changes
void PGCug::OnLeftColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopRowChange
//		Sent whenever the top visible row in the grid changes
void PGCug::OnTopRowChange(long oldrow,long newrow)
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
				2001-10-15 12:00		  always represent total number of selection blocks.
Return:
	<none>
****************************************************/
void PGCug::OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum)
{
	TRACE("PGCug::OnSelectionChanged called with startCol = %ld, startRow = %ld, endCol = %ld, endRow = %ld, blockNum = %ld.\n",
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
void PGCug::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
	//CUGCell cell;
	/*if(updn) {

		
		if(m_pCUG!=NULL) {
			int cur=((CalcCug*)m_pCUG)->m_VarSelectCurrent;

			if(row>0&&cur>0) {
				if(((CalcCug*)m_pCUG)->m_VarValueChoice[cur-1]==0) {
					if(	((CalcCug*)m_pCUG)->m_VarSelect[cur-1]>0)
						QuickSetBackColor(-1, ((CalcCug*)m_pCUG)->m_VarSelect[cur-1]-1, m_ColorHeading);
					((CalcCug*)m_pCUG)->m_VarSelect[cur-1]=row+1;
					QuickSetBackColor(-1,row, m_ColorClick);
					((CalcCug*)m_pCUG)->OnSetup();
				}
			}
		}


	}*/
	/*
	if(!m_Data) {
		if(col==0&&updn) {
			GetCell(0,i,&cell);
			cell.SetCellType(UGCT_CHECKBOX);	
			bool testV=pCPG->GetToBeSaved(i);
			if(testV) {
				cell.SetBool(FALSE);
				pCPG->SetToBeSaved(i,FALSE);
			}
			else {
				cell.SetBool(TRUE);	
				pCPG->SetToBeSaved(i,TRUE);
			}
			SetCell(0,i,&cell);
		}
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
void PGCug::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
//		Sent whenever the user double clicks the left mouse button within the grid
//	
//		'col' and 'row' are negative if the area clicked in is not a valid cell
//		'rect' the rectangle of the cell that was clicked in
//		'point' the point where the mouse was clicked
void PGCug::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	StartEdit();
	m_Changed=true;
}				 

/////////////////////////////////////////////////////////////////////////////
//	OnMouseMove
void PGCug::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user clicks the left mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PGCug::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if ( !updn )
	{
		CUGCell cell;
		GetCell(col,row,&cell);
		if(m_pPG->GetToBeCalc(col-1)) {
			m_pPG->SetToBeCalc(col-1,FALSE);
			cell.SetBackColor(m_ColorHeading);
			ClearSelections();
		}
		else {
			m_pPG->SetToBeCalc(col-1,TRUE);
			cell.SetBackColor(m_ColorSelected);
			SetCell(col,row,&cell);
			RedrawCell(col,row);
			SelectRange ( col, 0, col, GetNumberRows()-1 );

		}
		SetCell(col,row,&cell);
		RedrawCell(col,row);
	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_RClicked
//		Sent whenever the user clicks the right mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PGCug::OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top heading
//	
//		'col' is negative if the area clicked in is not valid
void PGCug::OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{

	
	//quickSetBackColor(
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_LClicked
//		Sent whenever the user clicks the left mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PGCug::OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if ( !updn ) {


		SelectRange ( 0, row, GetNumberCols()-1, row );


	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_RClicked
//		Sent whenever the user clicks the right mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PGCug::OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_DClicked
//		Sent whenever the user double clicks the left mouse button within the side heading
//	
//		'row' is negative if the area clicked in is not valid
void PGCug::OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_LClicked
//		Sent whenever the user clicks the left mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PGCug::OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_RClicked
//		Sent whenever the user clicks the right mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PGCug::OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top corner button
void PGCug::OnCB_DClicked(RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyDown
//		Sent whenever the user types when the grid has focus. The keystroke can be
//		modified here as well. (See WM_KEYDOWN for more information)
void PGCug::OnKeyDown(UINT *vcKey,BOOL processed)
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
void PGCug::OnCharDown(UINT *vcKey,BOOL processed)
{
}
	
/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent everytime the grid needs to
//		draw a cell in the grid. At this point the cell
//		class has been filled with the information to be
//		used to draw the cell. The information can now be
//		changed before it is used for drawing
void PGCug::OnGetCell(int col,long row,CUGCell *cell)
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
void PGCug::OnSetCell(int col,long row,CUGCell *cell)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDataSourceNotify
//		This message is sent from a data source , message
//		depends on the data source - check the information
//		on the data source(s) being used
//		- The ID of the Data source is also returned
void PGCug::OnDataSourceNotify(int ID,long msg,long param)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellTypeNotify
//		This message is sent from a cell type , message
//		depends on the cell type - check the information
//		on the cell type classes
//		- The ID of the cell type is given
int PGCug::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	CUGCell cell;
	if(ID==2 && msg == 53 && m_pPG!=NULL){
		size_t index=size_t(row);
		if(col==0) {

			GetCell(0,row,&cell);
			BOOL status;		
			status=cell.GetBool();
			if(status) {
				m_pPG->SetToBeSaved(index,true);
			}
			else {
				m_pPG->SetToBeSaved(index,false);
			}
		}
		else if(col==1) {
			GetCell(1,row,&cell);
			BOOL status;		
			status=cell.GetBool();
			if(status) {
				m_pPG->SetToBeViewed(index,true);
			}
			else {
				m_pPG->SetToBeViewed(index,false);
			}


		}

	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditStart
//		This message is sent whenever the grid is ready to start editing a cell
//		A return of TRUE allows the editing a return of FALSE stops editing
//		Plus the properties of the CEdit class can be modified
int PGCug::OnEditStart(int col, long row,CWnd **edit)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditVerify
//		This is send when the editing is about to end
int PGCug::OnEditVerify(int col, long row,CWnd *edit,UINT *vcKey)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int PGCug::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag)
{
	
	CPG *pCPG=(CPG*)m_pPG;
	if(!pCPG->m_WriteEnabled&&!pCPG->GetCompleteRead()) {
		size_t actbuf=pCPG->GetPrevBuffer();
		pCPG->CloseFile();
		pCPG->ReOpen(true);
		pCPG->SetActBuffer(actbuf);
	}
	CString str;
	edit->GetWindowText(str);
	QuickSetBackColor(col,row, m_ColorChanged);
	QuickSetText(col,row,str);
	m_Changed=true;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int PGCug::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	sections - UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//				UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
void PGCug::ReNumberColRows()
{
	if(m_Data) {
		int cols=GetNumberCols();
		CString str;
		for(int i=1;i<=cols-1;i++) {
			str="Var : "+MFC_Util::ItoAscii(i);
			QuickSetText(i,-1,str);
			((PGCug*)m_pCUG)->QuickSetNumber(-1,i-1,i);
		}
	}
	else {
		int rows=GetNumberRows();
		CString str;
		for(int i=1;i<=rows;i++) {
			str="Var : "+MFC_Util::ItoAscii(i);
			((PGCug*)m_pCUG)->QuickSetText(i,-1,str);
			QuickSetNumber(-1,i-1,i);
		}

	}


}
void PGCug::OnMenuCommand(int col,long row,int section,int item)
{
	CUGCell cell;
	if(section==UG_GRID) {

		if(item== 1000) {
			CutSelected();
			m_Changed=true;
		}
		else if(item==2000)
			CopySelected();

		else if(item==3000){
			Paste();
			m_Changed=true;
		}
		else if(item==1560) {
			CPG *pPG;
			pPG=(CPG*)m_pPG;
			pPG->SetAllToBeSaved(TRUE);
			for(int i=0;i<pPG->GetNumVariables();i++) {
				GetCell(0,i,&cell);
			    cell.SetBool(TRUE);
				SetCell(0,i,&cell);
			}
			RedrawAll();
			
		}
		else if(item==1561) {
			CPG *pPG;
			pPG=(CPG*)m_pPG;
			pPG->SetToBeSavedBelow(row,TRUE);
			for(int i=row;i<pPG->GetNumVariables();i++) {
				GetCell(0,i,&cell);
			    cell.SetBool(TRUE);
				SetCell(0,i,&cell);
				}
			RedrawAll();


		}
		else if(item==1562) {
			CPG *pPG;
			pPG=(CPG*)m_pPG;
			pPG->SetAllToBeSaved(FALSE);
			for(int i=0;i<pPG->GetNumVariables();i++) {
				GetCell(0,i,&cell);
			    cell.SetBool(FALSE);
				SetCell(0,i,&cell);
			}
			RedrawAll();


		}
		else if(item==1563) {
			CPG *pPG;
			pPG=(CPG*)m_pPG;
			pPG->SetToBeSavedBelow(row,FALSE);
			for(int i=row;i<pPG->GetNumVariables();i++) {
				GetCell(0,i,&cell);
			    cell.SetBool(FALSE);
				SetCell(0,i,&cell);
			}
			RedrawAll();


		}
		else if(item==1660) { // Output Sequence Order
			CPG *pPG;
			pPG=(CPG*)m_pPG;
			if(pPG->GetViewOrder())
				pPG->SetViewOrder(false);
			else
				pPG->SetViewOrder(true);
			((PGCug*)m_pCUG)->Make_DataHeader();
			((PGCug*)m_pCUG)->OnReadFileDataToGrid(pPG);

		}
		else if(item==1661) { // Enable/Disable viewed repetition data
			CPG *pPG;
			pPG=(CPG*)m_pPG;
			size_t index;
			bool enable=pPG->GetToBeViewed(size_t(row));
			for(size_t rep=0; rep<pPG->GetNumRepititions(); rep++) {
				index=row%pPG->GetNumVariables()+rep*pPG->GetNumVariables();
				GetCell(1,index,&cell);
				if(enable) {
					pPG->SetToBeViewed(index, false);
					cell.SetBool(FALSE);

				}
				else {
					pPG->SetToBeViewed(index, true);
					cell.SetBool(TRUE);
				}
				SetCell(1,index,&cell);
				RedrawCell(1, index);
			}
			pPG->UpdateViewOrder();

			((PGCug*)m_pCUG)->Make_DataHeader();
			((PGCug*)m_pCUG)->OnReadFileDataToGrid(pPG);
		}
		else if(item==1662) { // Enable/Disable viewed repetition data
			CPG *pPG;
			pPG=(CPG*)m_pPG;
			size_t index=0;
			bool enable=pPG->GetToBeViewed(size_t(row));
			for(size_t nvar=0; nvar<pPG->GetNumVariables();nvar++) {
				for(size_t rep=0; rep<pPG->GetNumRepititions(); rep++) {
					GetCell(1,index,&cell);
					if(enable) {
						pPG->SetToBeViewed(index, false);
						cell.SetBool(FALSE);

					}
					else {
						pPG->SetToBeViewed(index, true);
						cell.SetBool(TRUE);
					}
					SetCell(1,index,&cell);
					RedrawCell(1, index);
					index++;
				}
			}
			pPG->UpdateViewOrder();
			((PGCug*)m_pCUG)->Make_DataHeader();
			((PGCug*)m_pCUG)->OnReadFileDataToGrid(pPG);

		}
		else if(item==1663) {
			CPG *pPG=(CPG*)m_pPG;
			pPG->UpdateViewOrder();
			((PGCug*)m_pCUG)->Make_DataHeader();
			((PGCug*)m_pCUG)->OnReadFileDataToGrid(pPG);

		}
		else if(item==1700) {
			if(m_Data) {
				CPG *pPG;
				pPG=(CPG*)m_pPG;
				pPG->RemoveVariable(col-1);
			}
		}
		else if(item==1701) {
			if(m_Data) {
				InsertCol(col);
				((PGCug*)m_pCUG)->InsertRow(col-1);
			}
			else {
				InsertRow(row);
				((PGCug*)m_pCUG)->InsertCol(row+1);
			}

		}
		else if(item==1702) {




		}

	}
	else if(section==UG_TOPHEADING) {
		if(item==1500) {

			m_pPG->SetVarFunctionType(0);
			m_pPG->SetVarFunction(1);
			m_pPG->SetTimeConvUnit(1);
			m_pPG->SetTimeConvBase(m_pPG->GetLongTimeStart());
			CCalculateVariable dlg;	
			dlg.Init(m_pPG);
			if(dlg.DoModal()==IDOK) {
				if(OnNewCalculateVar())
					
					MessageBox("Calculation of new Date/Time successfully done");
			}


		}
		else if(item==1501) {
			m_pPG->SetVarFunctionType(0);
			m_pPG->SetVarFunction(2);
			m_pPG->SetTimeConvUnit(1);
			m_pPG->SetTimeConvBase(m_pPG->GetLongTimeStart());
			CCalculateVariable dlg;	
			dlg.Init(m_pPG);
			if(dlg.DoModal()==IDOK) {
				if(OnNewCalculateVar())
					
					MessageBox("Calculation of new Date/Time successfully done");
			}


		}
		else if(item==1502) {
			m_pPG->SetVarFunctionType(0);
			m_pPG->SetVarFunction(3);
			m_pPG->SetCalcVar(1);
			m_pPG->SetTimeConvUnit(1);
			m_pPG->SetTimeConvBase(m_pPG->GetLongTimeStart());
			CCalculateVariable dlg;	
			dlg.Init(m_pPG);
			if(dlg.DoModal()==IDOK) {
				if(OnNewCalculateVar())
					
					MessageBox("Calculation of new Date/Time successfully done");
			}


		}
		else if(item==1510) {
			m_ShowMean=1;
			int num_sheets=GetNumberSheets();
			for (long i=0;i<num_sheets;i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

//******* Add New Tabs 
			AddTab("Actual Data", 0); 
			AddTab("Yearly Data", 1); 
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if(item==1511) {
			m_ShowMean=2;
			int num_sheets=GetNumberSheets();
			for (long i=0;i<num_sheets;i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

//******* Add New Tabs 
			AddTab("Actual Data", 0); 
			AddTab("Montly Data", 1); 
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if(item==1512) {
			m_ShowMean=3;
			int num_sheets=GetNumberSheets();
			for (long i=0;i<num_sheets;i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

//******* Add New Tabs 
			AddTab("Actual Data", 0); 
			AddTab("Daily Data", 1); 
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if(item==1513) {
			m_ShowMean=4;
			int num_sheets=GetNumberSheets();
			for (long i=0;i<num_sheets;i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

//******* Add New Tabs 
			AddTab("Actual Data", 0); 
			AddTab("Hourly Data", 1); 
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if(item==1514) {
			m_ShowMean=5;
			int num_sheets=GetNumberSheets();
			for (long i=0;i<num_sheets;i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

//******* Add New Tabs 
			AddTab("Actual Data", 0); 
			AddTab("Minute Data", 1); 
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if(item==1515) {
			m_pPG->SetVarFunctionType(0);
			m_pPG->SetVarFunction(6);
			if(m_pPG->CalcVarTimeDerivate()) {		
				MessageBox("Calculation of New Derivate with adjusted time successfully done");
			}
		}
		else if(item==1700) {
			if(m_Data) {
				CPG *pPG;
				pPG=(CPG*)m_pPG;
				pPG->RemoveVariable(col-1);

			}
		}
		else if(item==1701) {
			if(m_Data) {
				CPG *pPG;
				pPG=(CPG*)m_pPG;
				pPG->AddNewVariable(col);
			}

		}
		else if(item>=1702) {
			CPG *pPG=(CPG*)m_pPG;
			bool newvar=true;
			bool allrep=false;
			size_t SeqNo;
			size_t ivar=pPG->GetViewVariable(col-1);
			size_t irep=pPG->GetViewRepetition(col-1);
			SeqNo=ivar+pPG->GetNumVariables()*(irep-1);
			if(item==1703||item==1705) newvar=false;
			if(item==1704||item==1705) allrep=true;

			Make_PG_Calc(newvar, allrep, SeqNo);

		}
	
	}
	else if(section==UG_SIDEHEADING) {
		if(item==1500) {

			m_pPG->SetVarFunctionType(0);
			m_pPG->SetVarFunction(1);
			m_pPG->SetTimeConvUnit(1);
			m_pPG->SetTimeConvBase(m_pPG->GetLongTimeStart());
			CCalculateVariable dlg;	
			dlg.Init(m_pPG);
			if(dlg.DoModal()==IDOK) {
				if(OnNewCalculateVar())
					
					MessageBox("Calculation of new Date/Time successfully done");
			}
		}
		else if(item==1501) {
			m_pPG->SetVarFunctionType(0);
			m_pPG->SetVarFunction(2);
			m_pPG->SetTimeConvUnit(1);
			m_pPG->SetTimeConvBase(m_pPG->GetLongTimeStart());
			CCalculateVariable dlg;	
			dlg.Init(m_pPG);
			if(dlg.DoModal()==IDOK) {
				if(OnNewCalculateVar())
					
					MessageBox("Calculation of new Date/Time successfully done");
			}


		}
		else if(item==1502) {
			m_pPG->SetVarFunctionType(0);
			m_pPG->SetVarFunction(3);
			m_pPG->SetCalcVar(1);
			m_pPG->SetTimeConvUnit(1);
			m_pPG->SetTimeConvBase(m_pPG->GetLongTimeStart());
			CCalculateVariable dlg;	
			dlg.Init(m_pPG);
			if(dlg.DoModal()==IDOK) {
				if(OnNewCalculateVar())
					
					MessageBox("Calculation of new Date/Time successfully done");
			}


		}
		else if(item==1510) {
			m_ShowMean=1;
			int num_sheets=GetNumberSheets();
			for (long i=0;i<num_sheets;i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

//******* Add New Tabs 
			AddTab("Actual Data", 0); 
			AddTab("Yearly Data", 1); 
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if(item==1511) {
			m_ShowMean=2;
			int num_sheets=GetNumberSheets();
			for (long i=0;i<num_sheets;i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

//******* Add New Tabs 
			AddTab("Actual Data", 0); 
			AddTab("Montly Data", 1); 
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if(item==1512) {
			m_ShowMean=3;
			int num_sheets=GetNumberSheets();
			for (long i=0;i<num_sheets;i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

//******* Add New Tabs 
			AddTab("Actual Data", 0); 
			AddTab("Daily Data", 1); 
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if(item==1513) {
			m_ShowMean=4;
			int num_sheets=GetNumberSheets();
			for (long i=0;i<num_sheets;i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

//******* Add New Tabs 
			AddTab("Actual Data", 0); 
			AddTab("Hourly Data", 1); 
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if(item==1514) {
			m_ShowMean=5;
			int num_sheets=GetNumberSheets();
			for (long i=0;i<num_sheets;i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

//******* Add New Tabs 
			AddTab("Actual Data", 0); 
			AddTab("Minute Data", 1); 
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if(item==1515) {
			m_pPG->SetVarFunctionType(0);
			m_pPG->SetVarFunction(6);
			if(m_pPG->CalcVarTimeDerivate()) {		
				MessageBox("Calculation of New Derivate with adjusted time successfully done");
			}
		}
		else if(item==1700) {
			if(m_Data) {
				CPG *pPG;
				pPG=(CPG*)m_pPG;
				pPG->RemoveVariable(row);

			}
		}
		else if(item==1701) {
			if(m_Data) {
				CPG *pPG;
				pPG=(CPG*)m_pPG;
				pPG->AddNewVariable(row);
			}

		}
		else if(item>=1702) {
				CPG *pPG=(CPG*)m_pPG;
				bool newvar=true;
				bool allrep=false;
				size_t SeqNo=row+1;
				if(item==1703||item==1705) newvar=false;
				if(item==1704||item==1705) allrep=true;
				Make_PG_Calc(newvar, allrep, SeqNo);

		}

		
	}
	if(item==1700||item==1701||(item>=1702&&item<2000)||(item>=1500&&item<=1502)) {
			if(m_Data)
				ReDisplay();
			else if(item>=1700)
				((PGCug*)m_pCUG)->ReDisplay();

	}

}

/////////////////////////////////////////////////////////////////////////////
//	return UG_SUCCESS to allow the menu to appear
//	return 1 to not allow the menu to appear
int PGCug::OnMenuStart(int col,long row,int section)
{
	CUGCell cell;
	EmptyMenu();
			//****** Populate the Menu 
	if(section==UG_GRID) {
			GetCell(2 ,row,&cell);

			if((!m_Data&&col>1)||m_Data) {
				AddMenuItem(1000, "Cut"); 
				AddMenuItem(2000, "Copy"); 
				AddMenuItem(3000, "Paste"); 
			}
			if(!m_Data&&col==0) {
				AddMenuItem(1560, "Save All Variables");
				AddMenuItem(1561, "Save All Below");
				AddMenuItem(1562, "Do not save any Variables");		
				AddMenuItem(1563, "Do not save any Below");
			}
			if(!m_Data&&col==1&&m_pPG!=NULL) {
				if(m_pPG->GetNumRepititions()>1) {
					if(!m_pPG->GetViewOrder())
						AddMenuItem(1660, "View All repetitions prior new variables");
					else
						AddMenuItem(1660, "View All variables prior new repetitions");


				}
				CString s;

				s=cell.GetText();
				if(!m_pPG->GetToBeViewed(size_t(row))) AddMenuItem(1661, "Enable all repetitions of"+s+" to be viewed");
					else  AddMenuItem(1661, "Disable all repetitions of "+s+" to be viewed");
				if(!m_pPG->GetToBeViewed(size_t(row))) AddMenuItem(1662, "Enable all variables to viewed");
					else  AddMenuItem(1662, "Disable all variables to be viewed");
				AddMenuItem(1663, "Update to view current selection");
			}
	}
	else if(section==UG_TOPHEADING&&m_Data) {
		if(col==0) {
			AddMenuItem(1500, "Move Current Date/time backward"); 
			AddMenuItem(1501, "Move Current Date/time forward"); 
			AddMenuItem(1502, "Convert Variable to Current Date/time"); 
			AddMenuItem(1510, "Make yearly mean values");
			AddMenuItem(1511, "Make monthly mean values");
			AddMenuItem(1512, "Make daily mean values");
			AddMenuItem(1513, "Make hourly mean values");
			AddMenuItem(1514, "Make 10 minutes mean values");
			AddMenuItem(1515, "Derivate (Days) and move to mid of interval");
		}
		else if(col>0) {
			CString str;
			AddMenuItem(1700, "Delete Variable");
			
			if(m_pPG->GetNumToBeCalc()>0) {
				str=MFC_Util::ItoAscii(m_pPG->GetNumToBeCalc());
				AddMenuItem(1702, "Add New Variable(s) based on selected group of "+str+ "variable(s)");
				AddMenuItem(1703, "Replace existing variable values for selected group of "+str+" variable(s)");
			}
			else {
				size_t SeqNo;
				size_t ivar=m_pPG->GetViewVariable(col-1);
				size_t irep=m_pPG->GetViewRepetition(col-1);
				SeqNo=ivar+m_pPG->GetNumVariables()*(irep-1);
				if(m_pPG->GetNumRepititions()>1) {
					str=MFC_Util::ItoAscii(ivar)+" Repetition No:"+MFC_Util::ItoAscii(irep);
				}
				else
					str=MFC_Util::ItoAscii(ivar);
				AddMenuItem(1702, "Add New Variable based on Variable No:"+str);
				AddMenuItem(1703, "Replace existing variable No:"+str);

				if(m_pPG->GetNumRepititions()>1) {
					str=MFC_Util::ItoAscii(ivar)+" and all Repetitions";
					AddMenuItem(1704, "Add New Variable based on Variable No:"+str);
					AddMenuItem(1705, "Replace existing variable No:"+str);

				}

			}
			



		};
	}
	else if(section==UG_SIDEHEADING&&!m_Data) {
		if(row>=0) {

			CString str;
			AddMenuItem(1700, "Delete Variable");
			
			if(m_pPG->GetNumToBeCalc()>0) {
				str=MFC_Util::ItoAscii(m_pPG->GetNumToBeCalc());
				AddMenuItem(1702, "Add New Variable(s) based on selected group of "+str+ "variable(s)");
				AddMenuItem(1703, "Replace existing variable values for selected group of "+str+" variable(s)");
			}
			else {
				if(m_pPG->GetNumRepititions()>1) {
					int irep=m_pPG->GetNumVariables();
					int arep=row/irep+1;
					int num=row%irep+1;
					str=MFC_Util::ItoAscii(num)+" Repetition No:"+MFC_Util::ItoAscii(arep);
				}
				else
					str=MFC_Util::ItoAscii(row+1);
				AddMenuItem(1702, "Add New Variable based on Variable No:"+str);
				AddMenuItem(1703, "Replace existing variable No:"+str);

				if(m_pPG->GetNumRepititions()>1) {
					int irep=m_pPG->GetNumVariables();
					int arep=row/irep+1;
					int num=row%irep+1;
					str=MFC_Util::ItoAscii(num)+" and all Repetitions";
					AddMenuItem(1704, "Add New Variable based on Variable No:"+str);
					AddMenuItem(1705, "Replace existing variable No:"+str);

				}

			}







		};


	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHint
int PGCug::OnHint(int col,long row,int section,CString *string)
{
	//int isheet=GetSheetNumber()

	if(m_pCUG!=NULL) {
			int cur=((CalcCug*)m_pCUG)->m_VarSelectCurrent;
			if(cur>0) {
				string->Format("Click to select variable for calculations");
				return TRUE;
			}
			return FALSE;
		
	}
	if(section==UG_TOPHEADING) {
		string->Format("Double click to select/deselect variables for calculations");


	}
	else
		string->Format("Col:%d Row:%ld",col, row);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnVScrollHint
int PGCug::OnVScrollHint(long row,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHScrollHint
int PGCug::OnHScrollHint(int col,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OLE
#ifdef __AFXOLE_H__

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  PGCug::OnDragEnter(COleDataObject* pDataObject){
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  PGCug::OnDragOver(COleDataObject* pDataObject,int col,long row){
	//return DROPEFFECT_NONE;
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  PGCug::OnDragDrop(COleDataObject* pDataObject,int col,long row){
	return DROPEFFECT_COPY;
}
#endif

/////////////////////////////////////////////////////////////////////////////
//	
void PGCug::OnScreenDCSetup(CDC *dc,int section){
}

/////////////////////////////////////////////////////////////////////////////
//	OnSortEvaluate
//	return		-1  <
//				0	==
//				1	>
int PGCug::OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags)
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
void PGCug::OnTabSelected(int ID)
{
	SetSheetNumber(ID);
	m_TabSelected=ID;
	CPG *pPG;
	pPG=(CPG*)m_pPG;
	if(pPG==NULL) return;
	if(ID>0)
		pPG->SetDataInFocus(m_ShowMean);
	else {
		pPG->SetDataInFocus(0);
	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnAdjustComponentSizes
void PGCug::OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,
		RECT *cnrBtn,RECT *vScroll,RECT *hScroll,RECT *tabs)
{
} 

/////////////////////////////////////////////////////////////////////////////
//	OnDrawFocusRect
void PGCug::OnDrawFocusRect(CDC *dc,RECT *rect)
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

void PGCug::DrawExcelFocusRect(CDC *dc,RECT *rect)
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
COLORREF PGCug::OnGetDefBackColor(int section)
{
	return GetSysColor ( COLOR_APPWORKSPACE );
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetFocus
void PGCug::OnSetFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKillFocus
void PGCug::OnKillFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapStart
BOOL PGCug::OnColSwapStart(int col)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanColSwap
BOOL PGCug::OnCanColSwap(int fromCol,int toCol)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTrackingWindowMoved
void PGCug::OnTrackingWindowMoved(RECT *origRect,RECT *newRect)
{
}
BOOL PGCug::OnChangeTime(long minchange)
{	
	long ltime;
	CPG *pCPG=m_pPG;
	if(m_Data&&pCPG!=NULL) {
		long m_NumRecords=m_row;
		for (long ii=0;ii<m_NumRecords;ii++) {
			ltime=pCPG->GetLongTime(ii+1);
			ltime+=minchange;
			pCPG->SetLongTime(ii+1,ltime);
			QuickSetText(0,ii,pCPG->GetDate(ii+1));
		}
		RedrawAll();
		return TRUE;
	}
	return FALSE;


}
BOOL PGCug::ReDisplay()
{
	//Make_MainSheets();


	CPG *pCPG=m_pPG;
	CUGCell cell;
	if(pCPG!=NULL){
		pCPG->UpdateViewOrder();
		m_col=pCPG->GetNumToView()+1;
	}
	while(GetNumberCols()<m_col) {
		int col=GetNumberCols()-1;
		AppendCol();
		GetColDefault(0,&cell);
		cell.SetAlignment(UG_ALIGNRIGHT);
		SetColDefault(col,&cell);
		SetColWidth(col,70);
		QuickSetText(col,-1,"Var: "+MFC_Util::ItoAscii(col));
		((PGCug*)m_pCUG)->AppendRow();
		((PGCug*)m_pCUG)->QuickSetText(-1,col-1,MFC_Util::ItoAscii(col));
	}
	while(GetNumberCols()>m_col) {
		int col=GetNumberCols()-1;
		DeleteCol(col);
		((PGCug*)m_pCUG)->DeleteRow(col-1);
	}
	if(m_pDlg!=NULL) {
		((CCreatePG*)m_pDlg)->m_NumVariables=m_col-1;
		((CCreatePG*)m_pDlg)->OnBnClickedRereadpg();
	}
	//OnReadFileToGrid(m_pPG);
	//((PGCug*)m_pCUG)->OnReadFileToGrid(m_pPG);
	return TRUE;

}

BOOL PGCug::OnNewCalculateVar()
{	
	CPG *pCPG=m_pPG;

	int expression, expression_type;
	int var;
	int var_in, var_out;
	BOOL jump;
	jump=FALSE;
	bool status=true;
	if(pCPG!=NULL) {
		long m_NumRecords=m_row;
		expression=pCPG->GetVarFunction();
		var=pCPG->GetCalcVarNew();
		if(!pCPG->GetCompleteRead()) {
			pCPG->CloseFile();
			pCPG->ReOpen(true);
		}

		for( int irep=0; irep<pCPG->GetNumRepititions();irep++) {
			if(pCPG->GetNumRepititions()>1) {
				if(!pCPG->SetRepititionSequence(irep+1, true)) return FALSE;
			}
			for (int ii=0; ii<pCPG->GetNumVariables();ii++){
				expression_type=pCPG->GetVarFunctionType();
				if(expression_type==0) {
					if(expression==6) {
						if(pCPG->CalcVarTimeDerivate()) 
							OnUpdate(pCPG->GetNumVariables()+1, pCPG->GetNumRecords(),TRUE);
							return TRUE;			
					}
					if(pCPG->CalcTimeChange()) 
						return TRUE;		
				}
		
				if(pCPG->GetToBeCalc(ii+irep*pCPG->GetNumVariables())&&(!jump||m_CalcAllRepetitions)) {
					var_in=ii+1;
					var_out=ii+1;
				
					if(pCPG->GetReplaceMode()) { 
						var_out=ii+2;
						jump=TRUE;
					}
					for(size_t nbuf=1; nbuf<=pCPG->GetNumBuffers();nbuf++) {
							if(pCPG->GetNumRepititions()<=1) pCPG->SetActBuffer(nbuf);
							if(expression_type==1) {
								if(expression==1) {

									if(pCPG->CalcVarScaling(var_in, var_out)) {
							
									}
								}
								else if(expression==2) {
									if(pCPG->CalcVarMissing(var_in, var_out)) {
							
									}
								}
								else if(expression==3) {
									if(pCPG->CalcVarMissing(var_in, var_out)) {
							
									}
								}
								else if(expression==4) {
										pCPG->CalcVarMissingSubstitute(var_in, var_out,false,nbuf);
								}
								else if(expression==5) {
									if(pCPG->CalcVarMissingSubstitute(var_in, var_out)) {
							
									}
								}
								else if(expression==6) {
									if(pCPG->CalcVarSmoothing(var_in, var_out)) {
							
									}
								}
								else if(expression==7) {
									if(pCPG->CalcVarDifference(var_in, var_out)) {
							
									}
								}
			

							}
				
							else if(expression_type==2) {
					
								if(expression==1) {
									if(pCPG->CalcVarMean(var)) {
										//return TRUE;
									}
								}
								else if(expression==2) {
									if(pCPG->CalcVarSum(var)) {
										//return TRUE;
									}


								}
								else if(expression==3) {
									if(pCPG->CalcVarMultiply(var)) {
										//return TRUE;
									}


								}
								else if(expression==4) {
									if(pCPG->CalcVarDivide(var)) {
										//return TRUE;
									}


								}
								else if(expression==5) {
									pCPG->SetVarFunction(4);// To avoid linear interpolation
									if(pCPG->CalcVarMissingSubstitute(var_in, var_out, true)) {
										//return TRUE;
									}


								}					
							}
							else if(expression_type==3) {
								if(expression>=1&&expression<=4) {
									if(pCPG->CalcTimeToVar(var)) {
										//return TRUE;
									}
								}

							}
							else
								jump=FALSE;

							if(!pCPG->GetCompleteRead()) {
								if(pCPG->GetNumRepititions()>1)
									status=pCPG->WriteNewBuffer(irep+1);
								else
									status=pCPG->WriteNewBuffer(nbuf);
							
								if(!status) return FALSE;
							}
						}
						
					}

			}
		}
		if(!pCPG->GetCompleteRead()) 
			pCPG->CloseFile();

		return TRUE;
	}
	return FALSE;


}



BOOL PGCug::isMeanCalculated()
{
	if(m_TabSelected>0)
		return TRUE;
	else
		return FALSE;

}

void PGCug::Make_PG_Calc(bool newvar, bool allrep, size_t SeqNo, size_t varin, size_t repin)
{
	int expression=1;
	int count=0;
	int prevnum;
	if(m_pPG->GetNumToBeCalc()<=0) {
		m_pPG->SetCalcVar(SeqNo);
		if(!newvar) {
				if(!allrep) {
					m_pPG->SetCalcVarNew(SeqNo+1);
					m_CalcAllRepetitions=false;
				}
				else {
					m_pPG->SetCalcVarNew(SeqNo+1);
					size_t iv=(SeqNo-1)%m_pPG->GetNumVariables();
					for(size_t i=0;i<m_pPG->GetNumRepititions();i++)
						m_pPG->SetToBeCalc(i*m_pPG->GetNumVariables()+iv, true);
					m_CalcAllRepetitions=true;
				}
		}
		else {
					
			if(!allrep) {
				m_pPG->SetCalcVarNew(SeqNo);
				m_CalcAllRepetitions=false;
			}
			else {
				m_pPG->SetCalcVarNew(SeqNo);
				size_t iv=(SeqNo-1)%m_pPG->GetNumVariables();
				for(size_t i=0;i<m_pPG->GetNumRepititions();i++)
					m_pPG->SetToBeCalc(i*m_pPG->GetNumVariables()+iv, true);
				m_CalcAllRepetitions=true;
			}
		}
	}
	if(m_pPG->GetNumToBeCalc()<=0) {
		m_pPG->SetToBeCalc(SeqNo-1,true);
	}
	prevnum=m_pPG->GetNumVariables();
	m_pPG->SetInfoMode();
			
	if(m_Data) { // Write Description to file 
		if(((PGCug*)m_pCUG)->m_Changed)
			((PGCug*)m_pCUG)->OnWriteGridDescriptionToFile(m_pPG);
		if(m_Changed)
			OnWriteGridDataToFile(m_pPG);
	}
	else {
		if(m_Changed)
			OnWriteGridDescriptionToFile(m_pPG);
		if(((PGCug*)m_pCUG)->m_Changed)
			((PGCug*)m_pCUG)->OnWriteGridDataToFile(m_pPG);
	}
		
	long line=0;
	bool jump=false;
	long startrep=0;
	for( int irep=0; irep<m_pPG->GetNumRepititions(); irep++) {
		for (int ii=0; ii<prevnum;ii++){
			if(m_pPG->GetToBeCalc(line)) {
				if((m_Data&&newvar)||newvar) {
					if(irep==startrep||!jump) {
						m_pPG->AddNewVariable(ii+1+count);
						m_pPG->SetReplace(TRUE);
						m_NewVariable=TRUE;
						count++;
						startrep=irep;
						jump=true;
					}
				}
				else {
					m_pPG->SetReplace(FALSE);
					m_NewVariable=FALSE;
				}
			}
			line++;
		}
	}
	m_pPG->SetVarFunctionType(1);

	CCalculateVariable dlg;	
	dlg.Init(m_pPG);

	if(dlg.DoModal()==IDOK) {
		if(OnNewCalculateVar()) {
			BOOL jump;
			jump=FALSE;
			long line=0;
			for(int irep=0; irep<m_pPG->GetNumRepititions();irep++) {
				for (int ii=0; ii<m_pPG->GetNumVariables();ii++){
					if(m_pPG->GetToBeCalc(line)) {
						if(!jump) {
							if(m_pPG->GetNumRepititions()>1)
								MessageBox("Calculation of new var no "+MFC_Util::ItoAscii(ii+1)+" Rep no "+MFC_Util::ItoAscii(irep+1)+" successfully done");
							else
								MessageBox("Calculation of new var no "+MFC_Util::ItoAscii(ii+1)+" successfully done");

						}
						if(!newvar&&!jump) {
							jump=TRUE;
						}
					else if(!newvar)
						jump=FALSE;
									
						m_pPG->SetToBeCalc(line, FALSE);
					}
					line++;
				}
			}
			m_Changed=false;
			((PGCug*)m_pCUG)->m_Changed=false;

		}
		else {
				MessageBox("Calculation of new var  failed");
				return;
		}
	}
	else {
		if(newvar) {
			CPG *pPG;
			pPG=(CPG*)m_pPG;
			pPG->RemoveVariable(SeqNo);
		}
	}
}