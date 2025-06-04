/////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived PGFileCug - Creating new PG File structures

#include "..\stdafx.h"
#include "..\std.h"
#include "..\Util.h"
#include "..\resource.h"
#include "PGFileCug.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(PGFileCug,CUGCtrl)
	//{{AFX_MSG_MAP(PGFileCug)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// PGFileCug construction/destruction
PGFileCug::PGFileCug()
{
//	UGXPThemes::UseThemes(true);
	m_bDrawExcelFocus = FALSE;

	m_ColorChanged=RGB(255,200,200);
	m_ColorHeading=RGB(230,230,230);
	m_ColorClick=RGB(240,240,50);

	SetMultiSelectMode ( FALSE );
	DragDropTarget ( FALSE );
	m_Init=FALSE;
	m_Created=FALSE;
	m_Interpolate=FALSE;

}

PGFileCug::~PGFileCug()
{
	DeleteFiles();
	//UGXPThemes::CleanUp();
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid

void PGFileCug::OnUpdate(CPG* pPG,  int index)
{
	CUGCell cell; 

	m_PGPointers.Add(pPG);

	//pp->OnInitDialog();
	if(m_Init) {
		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSetup();
	}
}

BOOL PGFileCug::MixFiles(CPG* pPG)
{

	m_PGPointers.Add(pPG);

	BOOL EqualTime;
	EqualTime=TRUE;
// make addition of files

	long numvar, numrec, numrectest, numrecmax;
	long itime, irec_count, irec, irec_index;
	long pgst,pgend;
	int icount;

	int nrep, kollnrep;
	bool equal_repetition=true;

	numvar=0;
	irec_count=0;
	numrecmax=0;
	CUIntArray RecordtoRead, MaxRecords, Valid;

	int NumLines=m_PGPointers.GetSize();
	numrec=-10;

	for(int i=0;i<NumLines-1;i++) {
			

			CPG *pCPG;
			pCPG=(CPG*)m_PGPointers.GetAt(i);
			pCPG->ReOpen();
			long nrec=pCPG->GetNumRecords();
			int  nvar=pCPG->GetNumVariables();
			nrep=pCPG->GetNumRepititions();
			if(i==0) kollnrep=nrep;
			if(kollnrep!=nrep) {
				equal_repetition=false;

			}
			RecordtoRead.Add(1);
			MaxRecords.Add(nrec);
			Valid.Add(0);
			numrecmax+=nrec;

			if(i==0) {
				numrec=nrec;
				numrectest=nrec;
				itime=pCPG->GetLongTimeStart();
			}
			else {
				if(numrectest!=nrec)
					EqualTime=FALSE;
				if(pgst!=pCPG->GetLongTimeStart())
					EqualTime=FALSE;
				if(pgend!=pCPG->GetLongTimeEnd())
					EqualTime=FALSE;
			}
			pgst=pCPG->GetLongTimeStart();
			if(itime>pgst)
				itime=pgst;
			pgend=pCPG->GetLongTimeEnd();
			if(nvar>0)
				numvar+=nvar;

	}
	if(!EqualTime) {
		if(!m_Interpolate) {
			bool ifirst;
			bool continueread;
			continueread=TRUE;
			irec_count=-1;
			while(continueread) {
				irec_count++;
				ifirst=TRUE;
				for (int jj=0; jj<NumLines-1;jj++) //Set Not valid for all files
					Valid.SetAt(jj,0);
				for(int i=0;i<NumLines-1;i++) {
					CPG *pCPG;
					pCPG=(CPG*)m_PGPointers.GetAt(i);
					irec=RecordtoRead.GetAt(i);
					if(irec<=MaxRecords.GetAt(i)) {

						if(ifirst){
							itime=pCPG->GetLongTime(irec);
							irec_index=i;
							Valid.SetAt(i,1);
							ifirst=FALSE;
						}
						else if(pCPG->GetLongTime(irec)<itime) {
							itime=pCPG->GetLongTime(irec);
							irec_index=i;
							Valid.SetAt(i,1);
							for(int jj=0;jj<i;jj++)
								Valid.SetAt(jj,0);
						}
						else if(pCPG->GetLongTime(irec)==itime) {
							Valid.SetAt(i,1);

						}
					}
				}
				continueread=FALSE;
				for(int i=0;i<NumLines-1;i++) {
					if(Valid.GetAt(i)>0) {
						irec=RecordtoRead.GetAt(i);
						irec++;
						RecordtoRead.SetAt(i,irec);
						continueread=TRUE;
					}
				}
			}
		}
		else {
			CPG *pCPG;
			pCPG=(CPG*)m_PGPointers.GetAt(0);
			irec_count=pCPG->GetNumRecords();

		}
		numrec=irec_count;
	}
	if(numrec<0) return FALSE;
	if(!equal_repetition) nrep=1;
	//pPG->CreateNewFile(numvar,numrec);
	pPG->SaveAs(pPG->GetFileName(), TRUE);
	pPG->SetCompleteRead(FALSE);
	pPG->CreateNewFile(numvar,numrec, nrep);
	if(EqualTime) { // Same Time no intepolation needed
		int icount;
		for(int irep=0; irep<nrep; irep++) {
			for(long j=0;j<numrec;j++) {
				icount=0;
				for(int i=0;i<NumLines-1;i++) {
					CPG *pCPG;
					pCPG=(CPG*)m_PGPointers.GetAt(i);
					numvar=pCPG->GetNumVariables();
					float *pvar=(float*)pCPG->GetRecordPointer(j+1, irep+1);
					if(i==0)
						pPG->SetRecordPointer(pvar);
					else
						pPG->SetRecordPointer(pvar, icount, numvar);
					icount+=numvar;
				}
				pPG->WritePGFileRecord(j+1, irep+1);

			}
		}
	}
	else if(m_Interpolate) { //Interpolation based on first file
		int icount;
		CPG *pCPG;
		float *pvar;
		long itime_next;
		for(int i=1; i<NumLines-1;i++)
			RecordtoRead.SetAt(i,1);
		for(long j=0;j<numrec;j++) {
			icount=0;
			for (int jj=0; jj<NumLines-1;jj++) //Set Not valid for all files
				Valid.SetAt(jj,0);
			for(int i=0;i<NumLines-1;i++) {
				pCPG=(CPG*)m_PGPointers.GetAt(i);

				if(i==0) {
					numvar=pCPG->GetNumVariables();
					float *pvar=(float*)pCPG->GetRecordPointer(j+1);
					pPG->SetRecordPointer(pvar);
					itime=pCPG->GetLongTime(j+1);
				}
				else {
					numvar=pCPG->GetNumVariables();
					bool continueread=TRUE;
					long priortime;
					while(continueread) {
						irec=RecordtoRead.GetAt(i);
						itime_next=pCPG->GetLongTime(irec);
						
						if(itime_next==itime) {  // exact
							pvar=(float*)pCPG->GetRecordPointer(irec);
							pPG->SetRecordPointer(pvar,icount,numvar);
							priortime=itime_next;
							continueread=FALSE;
						}
						else if(itime_next<itime) { // prior step forward
							pPG->SetMissingValueRecordPointer(icount,numvar);
							priortime=itime_next;
							RecordtoRead.SetAt(i, irec+1); 
						}
						else {   //after - do interpolation
							pPG->SetMissingValueRecordPointer(icount,numvar);
							continueread=FALSE;
							// first take time back to make start point for interpolation
							if(irec>1) {
								float *pvarprior=(float*)pCPG->GetRecordPointer(irec-1);
								float weight;

								vector<float> vfloat; vfloat.resize(numvar + 1);

								for (int i = 0; i < numvar; i++) {
										vfloat[i+1]= *(pvarprior + i + 1);
								}
								if(itime_next>priortime)
									weight=(double(itime)-double(priortime))/(double(itime_next)-double(priortime));
								else
									weight=1.;
								pvar=(float*)pCPG->GetRecordPointer(irec);
								pPG->SetRecordPointerInterpolate(pvar,icount,numvar, weight, vfloat);
								
							}
						}
					}

				}
				pPG->SetLongTime(1,itime);
				icount+=numvar;
			}
			pPG->WritePGFileRecord(j+1);

		}
	}
	else { // Mixing without interpolation
			bool ifirst;

			CPG *pCPG;
			float *pvar;
			for(size_t irep=0; irep<nrep; irep++) {
				irec_count=0;
				for(int i=0; i<NumLines-1;i++)
					RecordtoRead.SetAt(i,1);
				while(irec_count<numrec) {
					irec_count++;
					ifirst=TRUE;
					icount=0;
					for (int jj=0; jj<NumLines-1;jj++) //Set Not valid for all files
						Valid.SetAt(jj,0);
					for(int i=0;i<NumLines-1;i++) { //Determine which is leading and which other are equal
					
						pCPG=(CPG*)m_PGPointers.GetAt(i);
						if(!pCPG->IsOpen()) pCPG->ReOpen();
						irec=RecordtoRead.GetAt(i);
						if(irec<=MaxRecords.GetAt(i)) {
							if(ifirst){
								pCPG->ReadNewBuffer(1,1);
								itime=pCPG->GetLongTime(irec);
								irec_index=i;
								ifirst=FALSE;
								Valid.SetAt(i,1);
							}
							else if(pCPG->GetLongTime(irec)==itime) {
								itime=pCPG->GetLongTime(irec);
								Valid.SetAt(i,1);
							}
							else if(pCPG->GetLongTime(irec)<itime) {
								itime=pCPG->GetLongTime(irec);
								irec_index=i;
								for (int jj=0; jj<i;jj++) 
									Valid.SetAt(jj,0);
								Valid.SetAt(i,1);					
							}				
						}
					}

					for(int i=0;i<NumLines-1;i++) { //Set the Values 
						pCPG=(CPG*)m_PGPointers.GetAt(i);
						if(!pCPG->IsOpen()) pCPG->ReOpen();
						numvar=pCPG->GetNumVariables();
					
						irec=RecordtoRead.GetAt(i);	
						pCPG->ReadNewBuffer(irec, irep+1);
						if(irec<=MaxRecords.GetAt(i)) {
							pvar=(float*)pCPG->GetRecordPointer(irec, irep+1);
						}
						else 
							pvar=(float*)pCPG->GetRecordPointer(MaxRecords.GetAt(i), irep+1);
						if(i==0)
							pPG->SetRecordPointer(pvar);

						if(Valid.GetAt(i)==0) 
							pPG->SetMissingValueRecordPointer(icount,numvar);
						else if(i>0) {
							pPG->SetRecordPointer(pvar,icount,numvar);									
						}
						icount+=numvar;
					}
					pPG->SetLongTime(1,itime);
					pPG->WritePGFileRecord(irec_count, irep+1);
					for(int i=0;i<NumLines-1;i++) {
						if(Valid.GetAt(i)>0) {
							irec=RecordtoRead.GetAt(i);
							RecordtoRead.SetAt(i,irec+1);
						}
						else {


						}
					}

				}
			}	
			
	}


	for(int irep=0; irep<nrep; irep++) {
		icount=0;
		for(int i=0;i<NumLines-1;i++) {
			CPG *pCPG;
			pCPG=(CPG*)m_PGPointers.GetAt(i);
			numvar=pCPG->GetNumVariables();
			for(int i=0;i<numvar;i++) {
				icount++;
				pPG->SetVarName(icount,pCPG->GetVarName(i+1,irep+1), irep+1);
				pPG->SetVarUnit(icount,pCPG->GetVarUnit(i+1,irep+1), irep+1);
				pPG->SetVarId(icount,pCPG->GetVarId(i+1, irep+1), irep+1);
				pPG->SetVarPos(icount,pCPG->GetVarPos(i+1, irep+1), irep+1);
				pPG->SetVarMin(icount,pCPG->GetVarMin(i+1, irep+1), irep+1);
				pPG->SetVarMax(icount,pCPG->GetVarMax(i+1, irep+1), irep+1);
				pPG->SetVarLong(icount,pCPG->GetVarLong(i+1, irep+1), irep+1);
				pPG->SetVarLat(icount,pCPG->GetVarLat(i+1, irep+1), irep+1);
				pPG->SetVarAlt(icount,pCPG->GetVarAlt(i+1, irep+1), irep+1);
				pPG->SetVarCountry(icount,pCPG->GetVarCountry(i+1, irep+1), irep+1);
			}
		}

	}
	
	//if(pPG->GetCompleteRead())
		pPG->WritePGFileHeader(numrec, nrep);
	//else
		//pPG->WritePGFile();
	//pp->OnInitDialog();

	if(m_Init) {
		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSetup();
	}
	return TRUE;

}
BOOL PGFileCug::DeleteFiles()
{
	//while(m_PGPointers.GetSize()>0) {

	for(int i=0;i<m_PGPointers.GetSize()-1;i++) {
		CPG *pCPG;
		pCPG=(CPG*)m_PGPointers.GetAt(i);
		delete pCPG;
	}
		m_PGPointers.RemoveAll();
	//}
	return true;
}
BOOL PGFileCug::AddFiles(CPG* pPG)
{
	//Addjusted to be valid for files with repetitions
	m_PGPointers.Add(pPG);

// make addition of files

	long numvar, numrec;

	numrec=0;
	numvar=10000;
	int NumLines=m_PGPointers.GetSize();
	size_t num_rep= std::string::npos;
	bool Valid_Rep = true;

	for(int i=0;i<NumLines-1;i++) {
			CPG *pCPG;
			pCPG=(CPG*)m_PGPointers.GetAt(i);
			pCPG->ReOpen();
					long nrec=pCPG->GetNumRecords();
					int  nvar=pCPG->GetNumVariables();
					long pgst=pCPG->GetLongTimeStart();
					long pgend=pCPG->GetLongTimeEnd();
					auto rep = pCPG->GetNumRepititions();
					if (num_rep != std::string::npos) {
						if (rep != num_rep)
							Valid_Rep = false;
					}
					else {
						num_rep = rep;
						Valid_Rep = true;
					}
					numrec+=nrec;
					if(nvar<numvar)
						numvar=nvar;

	}

	pPG->SaveAs(pPG->GetFileName(), true);
	pPG->CreateNewFile(numvar,numrec, num_rep);
	for (size_t repet = 0; repet < num_rep; repet++) {
	long icount=0;
	for(int i=0;i<NumLines-1;i++) {
			CPG *pCPG;
			pCPG=(CPG*)m_PGPointers.GetAt(i);
			pCPG->ReOpen();
					long nrec=pCPG->GetNumRecords();
					int  nvar=pCPG->GetNumVariables();
					long pgst=pCPG->GetLongTimeStart();
					long pgend=pCPG->GetLongTimeEnd();
					
						for (long j = 0; j < nrec; j++) {
							float *pvar = (float*)pCPG->GetRecordPointer(j + 1, repet+1);
							icount = icount + 1;
							pPG->SetRecordPointer(pvar);
							pPG->WritePGFileRecord(icount, repet+1);
						}
		}
}

	for (size_t repet = 0; repet < num_rep; repet++) {
		for (int i = 0; i < numvar; i++) {
			CPG *pCPG;
			float fmin, fmax;
			fmax = MISSING;
			fmin = abs(MISSING);
			pCPG = (CPG*)m_PGPointers.GetAt(0);
			pPG->SetVarName(i + 1, pCPG->GetVarName(i + 1), repet+1);
			pPG->SetVarUnit(i + 1, pCPG->GetVarUnit(i + 1), repet+1);
			pPG->SetVarId(i + 1, pCPG->GetVarId(i + 1), repet+1);
			pPG->SetVarPos(i + 1, pCPG->GetVarPos(i + 1), repet+1);
			if (pCPG->GetVarMin(i + 1) < fmin)
				fmin = pCPG->GetVarMin(i + 1);
			if (pCPG->GetVarMax(i + 1) > fmax)
				fmax = pCPG->GetVarMax(i + 1);
			for (int j = 1; j < NumLines - 1; j++) {
				pCPG = (CPG*)m_PGPointers.GetAt(j);
				if (pCPG->GetVarMin(i + 1) < fmin)
					fmin = pCPG->GetVarMin(i + 1);
				if (pCPG->GetVarMax(i + 1) > fmax)
					fmax = pCPG->GetVarMax(i + 1);
			}
			pPG->SetVarMin(i + 1, fmin, repet+1);
			pPG->SetVarMax(i + 1, fmax, repet+1);
		}		
	}
	//pPG->WritePGFileFrame();
	pPG->WritePGFileHeader(numrec, num_rep);
	if(m_Init) {
		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSetup();
	}
	return TRUE;

}
BOOL PGFileCug::MergeFiles(CPG* pPG)
{

	m_PGPointers.Add(pPG);

// make addition of files

	long numvar, numrec;

	numrec=0;
	numvar=10000;
	int NumLines=m_PGPointers.GetSize();

	int nrepetitions=0;

	for(int i=0;i<NumLines-1;i++) {
			CPG *pCPG;
			pCPG=(CPG*)m_PGPointers.GetAt(i);
					long nrec=pCPG->GetNumRecords();
					int  nvar=pCPG->GetNumVariables();
					auto nrep=pCPG->GetNumRepititions();
					long pgst=pCPG->GetLongTimeStart();
					long pgend=pCPG->GetLongTimeEnd();
					numrec=nrec;
					if(nvar<numvar)
						numvar=nvar;
					nrepetitions+=nrep;
	}
	pPG->SaveAs(pPG->GetFileName(), true);
	pPG->CreateNewFile(numvar,numrec, nrepetitions);
	int count_rep=1;

	for(int i=0;i<NumLines-1;i++) {
		CPG *pCPG;
	
		pCPG=(CPG*)m_PGPointers.GetAt(i);
		pCPG->ReOpen();
		long nrec=pCPG->GetNumRecords();
		int  nvar=pCPG->GetNumVariables();
		auto nrepit=pCPG->GetNumRepititions();

		long pgst=pCPG->GetLongTimeStart();
		long pgend=pCPG->GetLongTimeEnd();
		for(size_t nrep=0; nrep<nrepit; nrep++) {
			for(long j=0;j<nrec;j++) {
				float *pvar=(float*)pCPG->GetRecordPointer(j+1, nrep+1);
				pPG->SetRecordPointer(pvar);
				pPG->WritePGFileRecord(j+1, count_rep);
			}

			for(int iv=0;iv<numvar;iv++) {
				float fmin,fmax;
				fmax=MISSING;
				fmin=abs(MISSING);
				pPG->SetVarName(iv+1,pCPG->GetVarName(iv+1,nrep+1 ),count_rep);
				pPG->SetVarUnit(iv+1,pCPG->GetVarUnit(iv+1,nrep+1), count_rep);
				pPG->SetVarId(iv+1,pCPG->GetVarId(iv+1,nrep+1), count_rep);
				pPG->SetVarPos(iv+1,pCPG->GetVarPos(iv+1,nrep+1), count_rep);

				pPG->SetVarLat(iv+1,pCPG->GetVarLat(iv+1,nrep+1), count_rep);
				pPG->SetVarLong(iv+1,pCPG->GetVarLong(iv+1,nrep+1), count_rep);
				pPG->SetVarAlt(iv+1,pCPG->GetVarAlt(iv+1,nrep+1), count_rep);
				pPG->SetVarKey(iv+1,pCPG->GetVarKey(iv+1,nrep+1), count_rep);
				pPG->SetVarCountry(iv+1,pCPG->GetVarCountry(iv+1,nrep+1), count_rep);
				pPG->SetVarStation(iv+1,pCPG->GetVarStation(iv+1,nrep+1), count_rep);

				if(pCPG->GetVarMin(iv+1,nrep+1)<fmin)
					fmin=pCPG->GetVarMin(iv+1,nrep+1);
				if(pCPG->GetVarMax(iv+1),nrep+1>fmax)
					fmax=pCPG->GetVarMax(iv+1,nrep+1);
				for(int j=1;j<NumLines-1;j++) {		
					if(pCPG->GetVarMin(iv+1,nrep+1)<fmin)
						fmin=pCPG->GetVarMin(iv+1,nrep+1);
					if(pCPG->GetVarMax(iv+1,nrep+1)>fmax)
						fmax=pCPG->GetVarMax(iv+1,nrep+1);
				}
				pPG->SetVarMin(iv+1,fmin, count_rep);
				pPG->SetVarMax(iv+1,fmax, count_rep);
			}
			count_rep++;

		}

		pCPG->CloseFile();
	}
	pPG->WritePGFileHeader(numrec, nrepetitions);

	//pPG->WritePGFileFrame();

	if(m_Init) {
		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSetup();
	}
	return TRUE;

}
void PGFileCug::OnSetup()
{
	CUGCell cell; 
	CString strvalue, Signature, ComputerName;
	string Name;
	int NumLines;
	CTime tt;
	int cols = m_col,
		rows = m_row;
	m_Init=TRUE;
	

		SetNumberCols(5);
				//SetDefFont(&m_font);
		SetColWidth(-1,30);
		SetColWidth(0,270);
		SetColWidth(1,70);
		SetColWidth(2,70);
		SetColWidth(3,110);
		SetColWidth(4,110);
		QuickSetText(-1,-1, "no:");			
		QuickSetText(0,-1, "FileName");
		QuickSetText(1,-1, "Variables");
		QuickSetText(2,-1, "Records");
		QuickSetText(3,-1, "Start Date");
		QuickSetText(4,-1, "End Date");


		NumLines=m_PGPointers.GetSize();
		SetNumberRows(NumLines); 
		for(int i=0;i<NumLines;i++) {

			CPG *pCPG;
			pCPG=(CPG*)m_PGPointers.GetAt(i);
			Name=pCPG->GetFileName();
			QuickSetText(0,i, Name);
			if(m_Created&&i==NumLines-1) {
				QuickSetText(-1,i,"Out");
				QuickSetBackColor(-1,i,m_ColorHeading);

			}
			else {
				QuickSetNumber(-1,i,i+1);
//			if(m_ShowPGTable)
//				QuickSetBackColor(-1,ind,m_ColorCompleted);
//			else 
			
				QuickSetBackColor(-1,i,m_ColorClick);
			}
			pCPG->ReOpen();
			if(pCPG->IsOpen()){
					long nrec=pCPG->GetNumRecords();
					int  nvar=pCPG->GetNumVariables();
					long pgst=pCPG->GetLongTimeStart();
					long pgend=pCPG->GetLongTimeEnd();
					QuickSetNumber(1,i, nvar);
					QuickSetNumber(2,i, nrec);
					GetCell(3,i,&cell);
					cell.SetTime(0,0,0,1,5,1980);
					cell.SetText(PGUtil::StringDatum(pgst).c_str());
					cell.SetMask("0000-00-00 00:00");
					SetCell(3,i,&cell);
					cell.SetText(PGUtil::StringDatum(pgend).c_str());
					SetCell(4,i,&cell);
			}
			pCPG->CloseFile();
		

		}
		RedrawAll();
//	EnableMenu(TRUE); 

	SetVScrollMode ( UG_SCROLLTRACKING );
	SetUniformRowHeight( TRUE );
	
}

/////////////////////////////////////////////////////////////////////////////
//	OnSheetSetup	
void PGFileCug::OnSheetSetup(int sheetNumber)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the current cell in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int PGFileCug::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the top row or left column in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int PGFileCug::OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	
void PGFileCug::OnHitBottom(long numrows,long rowspast,long rowsfound)
{
}

/////////////////////////////////////////////////////////////////////////////
//	
void PGFileCug::OnHitTop(long numrows,long rowspast)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeCol
//		Sent when the user is over a separation line on the top heading
//		A return value of TRUE allows the possibiliy of a resize
int PGFileCug::OnCanSizeCol(int col)
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
void PGFileCug::OnColSizing(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSized
//		This is sent when the user finished sizing the 
//		given column (see above for more details)
void PGFileCug::OnColSized(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeRow
//		Sent when the user is over a separation line on the side heading
//		A return value of TRUE allows the possibiliy of a resize
int  PGFileCug::OnCanSizeRow(long row)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSizing
//	    Sent when the user is sizing a row The row that is being sized is given as 
//		well as the height. Plus the height can be modified 
//		at this point. This makes it easy to set min and max heights
void PGFileCug::OnRowSizing(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSized
//		This is sent when the user is finished sizing hte
//		given row ( see above for more details)
void PGFileCug::OnRowSized(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeSideHdg
//		This is sent when the user moves into position 
//		for sizing the width of the side heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int PGFileCug::OnCanSizeSideHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeTopHdg
//		This is sent when the user moves into position 
//		for sizing the height of the top heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int PGFileCug::OnCanSizeTopHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSizing
int PGFileCug::OnSideHdgSizing(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSized
int PGFileCug::OnSideHdgSized(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSized
int PGFileCug::OnTopHdgSized(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSizing
int PGFileCug::OnTopHdgSizing(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColChange
//		Sent whenever the current column changes
//		The old and the new columns are given
void PGFileCug::OnColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowChange
//		Sent whenever the current row changes
//		The old and the new rows are given
void PGFileCug::OnRowChange(long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellChange
//		Sent whenever the current cell changes rows or columns
void PGFileCug::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnLeftColChange
//		Sent whenever the left visible column in the grid changes
void PGFileCug::OnLeftColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopRowChange
//		Sent whenever the top visible row in the grid changes
void PGFileCug::OnTopRowChange(long oldrow,long newrow)
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
void PGFileCug::OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum)
{
	TRACE("PGFileCug::OnSelectionChanged called with startCol = %ld, startRow = %ld, endCol = %ld, endRow = %ld, blockNum = %ld.\n",
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
void PGFileCug::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
	CUGCell cell; 
	CString str;
	

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
void PGFileCug::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
//		Sent whenever the user double clicks the left mouse button within the grid
//	
//		'col' and 'row' are negative if the area clicked in is not a valid cell
//		'rect' the rectangle of the cell that was clicked in
//		'point' the point where the mouse was clicked
void PGFileCug::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	StartEdit();
}				 

/////////////////////////////////////////////////////////////////////////////
//	OnMouseMove
void PGFileCug::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user clicks the left mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PGFileCug::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
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
void PGFileCug::OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top heading
//	
//		'col' is negative if the area clicked in is not valid
void PGFileCug::OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_LClicked
//		Sent whenever the user clicks the left mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PGFileCug::OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
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
void PGFileCug::OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_DClicked
//		Sent whenever the user double clicks the left mouse button within the side heading
//	
//		'row' is negative if the area clicked in is not valid
void PGFileCug::OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_LClicked
//		Sent whenever the user clicks the left mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PGFileCug::OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_RClicked
//		Sent whenever the user clicks the right mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void PGFileCug::OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top corner button
void PGFileCug::OnCB_DClicked(RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyDown
//		Sent whenever the user types when the grid has focus. The keystroke can be
//		modified here as well. (See WM_KEYDOWN for more information)
void PGFileCug::OnKeyDown(UINT *vcKey,BOOL processed)
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
void PGFileCug::OnCharDown(UINT *vcKey,BOOL processed)
{
}
	
/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent everytime the grid needs to
//		draw a cell in the grid. At this point the cell
//		class has been filled with the information to be
//		used to draw the cell. The information can now be
//		changed before it is used for drawing
void PGFileCug::OnGetCell(int col,long row,CUGCell *cell)
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
void PGFileCug::OnSetCell(int col,long row,CUGCell *cell)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDataSourceNotify
//		This message is sent from a data source , message
//		depends on the data source - check the information
//		on the data source(s) being used
//		- The ID of the Data source is also returned
void PGFileCug::OnDataSourceNotify(int ID,long msg,long param)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellTypeNotify
//		This message is sent from a cell type , message
//		depends on the cell type - check the information
//		on the cell type classes
//		- The ID of the cell type is given
int PGFileCug::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditStart
//		This message is sent whenever the grid is ready to start editing a cell
//		A return of TRUE allows the editing a return of FALSE stops editing
//		Plus the properties of the CEdit class can be modified
int PGFileCug::OnEditStart(int col, long row,CWnd **edit)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditVerify
//		This is send when the editing is about to end
int PGFileCug::OnEditVerify(int col, long row,CWnd *edit,UINT *vcKey)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int PGFileCug::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag)
{
	CString str;
	edit->GetWindowText(str);
	//QuickSetText(col,row,str);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int PGFileCug::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	sections - UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//				UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
void PGFileCug::OnMenuCommand(int col,long row,int section,int item)
{
	CUGCell cell; 
	if(section==UG_GRID) {
		if(item== 1000)
			CutSelected();

		if(item==2000)
			CopySelected();

		if(item==3000) {
			Paste();
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
//	return UG_SUCCESS to allow the menu to appear
//	return 1 to not allow the menu to appear
int PGFileCug::OnMenuStart(int col,long row,int section)
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
int PGFileCug::OnHint(int col,long row,int section,CString *string)
{
	string->Format(_T("Col:%d Row:%ld"),col,row);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnVScrollHint
int PGFileCug::OnVScrollHint(long row,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHScrollHint
int PGFileCug::OnHScrollHint(int col,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OLE
#ifdef __AFXOLE_H__

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  PGFileCug::OnDragEnter(COleDataObject* pDataObject){
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  PGFileCug::OnDragOver(COleDataObject* pDataObject,int col,long row){
	//return DROPEFFECT_NONE;
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  PGFileCug::OnDragDrop(COleDataObject* pDataObject,int col,long row){
	return DROPEFFECT_COPY;
}
#endif

/////////////////////////////////////////////////////////////////////////////
//	
void PGFileCug::OnScreenDCSetup(CDC *dc,int section){
}

/////////////////////////////////////////////////////////////////////////////
//	OnSortEvaluate
//	return		-1  <
//				0	==
//				1	>
int PGFileCug::OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags)
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
void PGFileCug::OnTabSelected(int ID)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnAdjustComponentSizes
void PGFileCug::OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,
		RECT *cnrBtn,RECT *vScroll,RECT *hScroll,RECT *tabs)
{
} 

/////////////////////////////////////////////////////////////////////////////
//	OnDrawFocusRect
void PGFileCug::OnDrawFocusRect(CDC *dc,RECT *rect)
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

void PGFileCug::DrawExcelFocusRect(CDC *dc,RECT *rect)
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
COLORREF PGFileCug::OnGetDefBackColor(int section)
{
	return GetSysColor ( COLOR_APPWORKSPACE );
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetFocus
void PGFileCug::OnSetFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKillFocus
void PGFileCug::OnKillFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapStart
BOOL PGFileCug::OnColSwapStart(int col)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanColSwap
BOOL PGFileCug::OnCanColSwap(int fromCol,int toCol)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTrackingWindowMoved
void PGFileCug::OnTrackingWindowMoved(RECT *origRect,RECT *newRect)
{
}

