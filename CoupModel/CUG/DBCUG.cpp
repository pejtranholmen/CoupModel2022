/////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived DBCug

#include "..\stdafx.h"
#include "..\Util.h"
#include "..\resource.h"
#include "..\ModelTypes\NewModelType.h"
#include "DBCug.h"
#include "..\SP\SP_Retrieve.h"
//#include "..\ParameterPlotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(DBCug,CUGCtrl)
	//{{AFX_MSG_MAP(DBCug)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DBCug construction/destruction
DBCug::DBCug()
{
//	UGXPThemes::UseThemes(true);
	m_bDrawExcelFocus = TRUE;
	m_col=1;
	m_row=1;
	m_DataBase=TRUE;
	m_CompleteList=FALSE;
	m_pCTab=NULL;
	m_pDB2=NULL;
	m_SelOne=FALSE;
	m_ColorChanged=RGB(255,200,200);
	m_ColorDisable=RGB(180,180,180);
	m_ColorReadOnly=RGB(255,240,240);
	m_ColorReadOnlyClick=RGB(220,220,10);
	m_ColorHeading=RGB(230,230,230);
	m_ColorClick=RGB(240,240,50);


/*	if(PLOTPF_mp_FINDPROFILE()) {
	   m_IniProf=PLOTPF_mp_ACTUALPROFILE;
	   m_IniRepProf=PLOTPF_mp_ACTUALREPPROFILE;
	}
	else
		m_IniProf=-10;
*/
	SetMultiSelectMode ( TRUE );
	DragDropTarget ( TRUE );
	m_pCUG=NULL;
	m_fontsList = NULL;
	m_font.CreateFont(13,0,0,0,500,0,0,0,0,0,0,0,0,"Times");
	m_largeFont.CreateFont(14,0,0,0,600,0,0,0,0,0,0,0,0,"Times");

	
	m_fontID = AddFont("Arial", 15, 1); 
	SetDefFont(m_fontID);




}

DBCug::~DBCug()
{
	_tagFonts *pFont, *pTemp;
	pFont = m_fontsList;
	while ( pFont != NULL )
	{
		pTemp = pFont->next;
		delete pFont;
		pFont = pTemp;
	}
	//UGXPThemes::CleanUp();
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid

void DBCug::OnUpdateDataBase(CDialog *pEditDlg, CDB *pDB)
{
	NewMap *p_fmap;




	m_pPP_EDIT_Dlg=pEditDlg;
	m_pDB2=pDB;
	//m_pDoc=((PP_EDIT*)m_pPP_EDIT_Dlg)->m_pDoc;


	m_pSwTimeRes=(Sw*)m_pDoc->GetPtr(SWITCH,"TimeResolution");
	m_pSwPlantType=(Sw*)m_pDoc->GetPtr(SWITCH,"PlantType");





}
BOOL DBCug::OnAdjustLeft(int col)
{
	SetLeftCol(col);
	RedrawAll();
	return TRUE;

}
void DBCug::OnSetup()
{
	CUGCell cell; 
	CString str, Group, Type, Name;


	int Sw_Counter, P_Counter, Pt_Counter;
	Sw_Counter=P_Counter=Pt_Counter=0;
	EnableMenu(TRUE); 

	//	SetDefFont(&m_font);

		int num_sheets=GetNumberSheets();
		for (long i=0;i<num_sheets;i++) {
			DeleteTab(i);
		}
		SetDefColWidth(130);
		AddTab("Switches", 0); 
		AddTab("Parameters", 1); 
		AddTab("Table Elements", 2); 
		AddTab("Parameter Tables", 3); 
		AddTab("Model Files", 4); 
		SetTabWidth(400); 
		SetSheetNumber(0); 
		OnSheetSetup(0);
		SetCurrentTab(0);
		SetNumberSheets(5); 


	
	
}

/////////////////////////////////////////////////////////////////////////////
//	OnSheetSetup	
void DBCug::OnSheetSetup(int sheetNumber)
{

	UNREFERENCED_PARAMETER(sheetNumber);
	CUGCell cell; 
	CString heading, number, test;

	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	CString string, Defstring;
	CString optionlist;
	int MaxCol=8;
	int colv=0;

	BOOL Tableexpand;

	Tableexpand=FALSE;
	EnableMenu(TRUE); 
			SetSheetNumber(sheetNumber);

			EnableUpdate(FALSE); 
			while(GetNumberRows()>0)
				DeleteRow(0);
			EnableUpdate(TRUE); 
			switch(sheetNumber)		{
			case (0): //Switches
				Make_SwitchesSheet();
				RedrawAll();
				break;
			case (1): // Parameters
				Make_ParameterSheet();
				RedrawAll(); 
				break;
			case (2): // Parameters
				Make_TableElementSheet();
				RedrawAll(); 
				break;
			case(3): // Parameters Tables
				Make_ParameterTableSheet();
				break;
			case(4): // Model Files
				Make_ModelFileSheet();
				RedrawAll();
				break;
			}


}
void DBCug::Make_SwitchesSheet()
{
	CUGCell cell; 
	CString str, Group, Type, Name;

	SimB *pSw;

	int Sw_Counter, P_Counter, Pt_Counter, Counter;
	Sw_Counter=P_Counter=Pt_Counter=0;
	EnableMenu(TRUE); 

	SetSheetNumber(0);
	SetDefColWidth(130);
	SetDefFont(m_fontID);
	BOOL readonly;


	for(int i=0;i<m_pDoc->m_arrGroups.size();i++) {
				Group=pEDIT->m_arrGroups.GetAt(i);
				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";
				Type="Switches";
				vector<SimB*> vv;
				vv=m_pDoc->GetPtrVector(string(Type),string(Group));
				Sw_Counter+=vv.size();
		}	
		
	
	   	SetSH_NumberCols(3);
		SetNumberCols (pEDIT->m_arrIdKey.GetSize());
		SetNumberRows (Sw_Counter+2);
		SetSH_Width(390);
		SetSH_ColWidth(-3,90);
		SetSH_ColWidth(-2,150);
		SetSH_ColWidth(-1,150);

		Counter=0;

		pSw=m_pSwTimeRes;	
		str=m_pSwTimeRes->GetName().c_str();
		QuickSetFont(-2,Counter,&m_largeFont);
		QuickSetText(-2,Counter,str);

		QuickSetText(-3,Counter,"Hidden");
		QuickSetAlignment(-2,Counter, UG_ALIGNLEFT);
		int i1=pSw->GetValue();
		str=((Sw*)pSw)->GetOption(i1).c_str();
		QuickSetAlignment(-1,Counter, UG_ALIGNLEFT);
		QuickSetText(-1,Counter, str);
				
		Counter++;
		pSw=m_pSwPlantType;	
		str=pSw->GetName().c_str();;
		QuickSetFont(-2,Counter,&m_largeFont);
		QuickSetText(-2,Counter,str);
		QuickSetText(-3,Counter,"Model Structure");
		QuickSetAlignment(-2,Counter, UG_ALIGNLEFT);
		i1=pSw->GetValue();
		str=((Sw*)pSw)->GetOption(i1).c_str();
		QuickSetAlignment(-1,Counter, UG_ALIGNLEFT);
		QuickSetText(-1,Counter, str);
		Counter++;




		for(int i=0;i<pEDIT->m_arrGroups.GetSize();i++) {
				Group=pEDIT->m_arrGroups.GetAt(i);
				Type="Switches";
				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";

				QuickSetText(-1,-1,"Module  |    Name	    |   Current Value");
				vector<SimB*> vv;
				vv=m_pDoc->GetPtrVector(string(Type),string(Group));
				if(vv.size()>0){
					size_t num=vv.size();
					QuickSetCellTypeEx(-3,Counter,UGCT_NORMALMULTILINE);
					QuickSetText(-3,Counter,Group);
					QuickSetFont(-3,Counter,&m_largeFont);
					QuickSetAlignment(-3,Counter, UG_ALIGNLEFT);
					JoinCells(-3,Counter,-3,Counter+num-1);
					for(size_t ii=0;ii<vv.size();ii++) {
						pSw=(SimB*)vv[ii];
						str=pSw->GetName().c_str();
						QuickSetFont(-2,Counter,&m_largeFont);
						QuickSetText(-2,Counter,str);
						QuickSetText(-3,Counter,Group);
						QuickSetAlignment(-2,Counter, UG_ALIGNLEFT);
						int i1=pSw->GetValue();
						str=((Sw*)pSw)->GetOption(i1).c_str();
						QuickSetAlignment(-1,Counter, UG_ALIGNLEFT);
						QuickSetText(-1,Counter, str);
						if(!pSw->IsEnabled())
							QuickSetBackColor(-1, Counter, m_ColorDisable);
						Counter++;
					}
				}

		}	

		string group= pEDIT->m_ID_DB_Group;
		for(int ix=0;ix<pEDIT->m_arrIdKey.GetSize();ix++) {
				CString testkey;
				testkey=pEDIT->m_arrIdKey[ix];
				if(testkey.Find("NN3")>=0) {

					int i=0;

				}
				QuickSetText(ix,-1, testkey);
				QuickSetFont(ix,-1,&m_largeFont);
				
				
				pEDIT->m_ArrayIndex=ix;
				m_pDoc->Reset();
				m_pDoc->Retrieve(group, string(testkey), true);
				if(ix<pEDIT->m_NumOriginalRecords) {
					readonly=TRUE;
					QuickSetBackColor(ix,-1, m_ColorChanged);
				}
				else {
					readonly=FALSE;
					QuickSetBackColor(ix,-1, m_ColorHeading);

				}

				
				Counter=0;
				CString optionlist;	
				size_t i1, n;

				pSw=m_pSwTimeRes;
				while(pSw!=NULL) {
					GetCell(ix,Counter, &cell); 
					cell.SetCellType(UGCT_DROPLIST); 
					optionlist=_T("");
					n=((Sw*)pSw)->GetNumOptions();
					for(int j=0;j<n;j++) {
						optionlist+=((Sw*)pSw)->GetOption(j).c_str();
						optionlist+="\n";
					}
					cell.SetLabelText(optionlist);
					i1=pSw->GetDBIntValue();
					if(i1!=pSw->GetOriginalValue()) {
						str=((Sw*)pSw)->GetOption(i1).c_str();
						cell.SetText(str);
						cell.SetBackColor(m_ColorClick);
						if(readonly)
							cell.SetBackColor(m_ColorReadOnlyClick);
					}
					else {
						str=((Sw*)pSw)->GetOption(i1).c_str();
						cell.SetText( str);
						cell.SetBackColor(m_ColorHeading);
						if(readonly)
							cell.SetBackColor(m_ColorReadOnly);
					}
					SetCell(ix,Counter,&cell);
					Counter++;
					if(Counter==1)
						pSw=m_pSwPlantType;
								//Sw=m_pSwPlantType;	
					else
						pSw=NULL;
				}
				vector<SimB*> vv;
				
				for(int i=0;i<pEDIT->m_arrGroups.GetSize();i++) {
					Group=pEDIT->m_arrGroups.GetAt(i);
					if(Group=="Multiple Plant") Group="Plant";
					else if(Group=="Snow") Group="SnowPack";
					Type="Switches";
					vv=m_pDoc->GetPtrVector(string(Type),string(Group));
	
						for(size_t ii=0;ii<vv.size();ii++) {
							pSw=(SimB*)vv[ii];
							GetCell(ix,Counter, &cell); 
							cell.SetCellType(UGCT_DROPLIST); 
							optionlist=_T("");
							n=((Sw*)pSw)->GetNumOptions();
							for(int j=0;j<n;j++) {
								optionlist+=((Sw*)pSw)->GetOption(j).c_str();
								optionlist+="\n";
							}
							cell.SetLabelText(optionlist);

							i1=pSw->GetDBIntValue();
							if(i1!=pSw->GetOriginalValue()) {
								str=((Sw*)pSw)->GetOption(i1).c_str();
								cell.SetText(str);
								cell.SetBackColor(m_ColorClick);
								if(readonly)
									cell.SetBackColor(m_ColorReadOnlyClick);
							}
							else {
								str=((Sw*)pSw)->GetOption(i1).c_str();
								cell.SetText( str);
								cell.SetBackColor(m_ColorHeading);
								if(readonly)
									cell.SetBackColor(m_ColorReadOnly);
							}
							if(!pSw->IsDBEnabled())
								cell.SetBackColor(m_ColorDisable);

							SetCell(ix,Counter,&cell);
							Counter++;
						}
				}


		}
				
		CString test;
	/*	int colst, colend, numcol;
		numcol=pEDIT->m_arrIdKey.GetSize();
		for (int i=0;i<Sw_Counter;i++) {
				colst=0;
				colend=colst+1;
				str=QuickGetText(colst,i);
				while(colend<numcol) {
				 if(QuickGetText(colend,i)==str) {
	
				 }
				 else {
					if(colend-1>colst)
						JoinCells(colst,i,colend-1,i);
					colst=colend;
					str=QuickGetText(colst,i);
				 }
				 colend++;
				}
				if(colend-1>colst)
					JoinCells(colst,i,colend-1,i);

		}
*/
		OnAdjustLeft(pEDIT->m_SelectedIndex);
	SetVScrollMode ( UG_SCROLLTRACKING );
	SetUniformRowHeight( TRUE );
}
void DBCug::Make_ParameterSheet()
{
	CUGCell cell; 
	CString str, Group, Type, Name;

	SimB *pSw;
	int Sw_Counter, P_Counter, Pt_Counter, Counter;
	Sw_Counter=P_Counter=Pt_Counter=0;

	EnableMenu(TRUE); 
	
	PP_EDIT *pEDIT;
	pEDIT=(PP_EDIT*)m_pPP_EDIT_Dlg;
	SetSheetNumber(1);
	SetDefColWidth(75);
	SetDefFont(m_fontID);
	BOOL readonly;
	vector<SimB*> vv;
  //int i=str.ReverseFind(':');

  //while(if<m_arr_Idnum.GetSize) {
//	if(m_arr_Idnum[i]==
//
  //}
 // m_ArrayIndex=m_SelectedIndex;
		for(int i=0;i<pEDIT->m_arrGroups.GetSize();i++) {
				Group=pEDIT->m_arrGroups.GetAt(i);
				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";
				Type="Parameters";
				vv=m_pDoc->GetPtrVector(string(Type),string(Group), true);
				P_Counter+=vv.size();
		}	
		SetSH_NumberCols(3);
		SetNumberCols (pEDIT->m_arrIdKey.GetSize());
		SetNumberRows (P_Counter);
		SetSH_Width(315);
		SetSH_ColWidth(-3,90);
		SetSH_ColWidth(-2,150);
		SetSH_ColWidth(-1,75);
		QuickSetFont(-1,-1,&m_largeFont);
		QuickSetText(-1,-1,"Module  |    Name	    |   Current Value");
		Counter=0;
		for(int i=0;i<pEDIT->m_arrGroups.GetSize();i++) {
				Group=pEDIT->m_arrGroups.GetAt(i);
				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";
				Type="Parameters";
				vv=m_pDoc->GetPtrVector(string(Type),string(Group), true);
				if(vv.size()>0){
					int num=vv.size();
					QuickSetCellTypeEx(-3,Counter,UGCT_NORMALMULTILINE);
					QuickSetText(-3,Counter,Group);
					QuickSetFont(-3,Counter,&m_largeFont);
					QuickSetAlignment(-3,Counter, UG_ALIGNLEFT);
					JoinCells(-3,Counter,-3,Counter+num-1);
					for(size_t ii=0;ii<vv.size();ii++) {
						pSw=(SimB*)vv[ii];
						str=pSw->GetName().c_str();
						QuickSetText(-3,Counter,Group);
						QuickSetText(-2,Counter, str);
						QuickSetFont(-2,Counter,&m_largeFont);
						QuickSetAlignment(-2,Counter, UG_ALIGNLEFT);
						float val=pSw->GetValue();
						QuickSetAlignment(-1,Counter, UG_ALIGNLEFT);
						QuickSetFont(-1,Counter,&m_largeFont);
						QuickSetNumber(-1,Counter, val);
						if(!pSw->IsEnabled())
							QuickSetBackColor(-1, Counter, m_ColorDisable);
						Counter++;
					}
				}
		}	


		for(int ix=0;ix<pEDIT->m_arrIdKey.GetSize();ix++) {
				CString testkey;
				testkey=pEDIT->m_arrIdKey[ix];
				QuickSetText(ix,-1, testkey);
				QuickSetFont(ix,-1,&m_largeFont);
				pEDIT->m_ArrayIndex=ix;
				
				if(ix<pEDIT->m_NumOriginalRecords||pEDIT->m_TotalReadOnly) 
					pEDIT->m_ReadOnly=TRUE;
				else {
					pEDIT->m_ReadOnly=FALSE;
				}
				m_pDoc->Reset();
				string group;
				group=string(pEDIT->m_ID_DB_Group);
				m_pDoc->Retrieve(group, string(testkey), true);
				if(ix<pEDIT->m_NumOriginalRecords)
					readonly=TRUE;
				else
					readonly=FALSE;
			//	SetDefFont(&m_font);
				Counter=0;
				for(int i=0;i<pEDIT->m_arrGroups.GetSize();i++) {
					Group=pEDIT->m_arrGroups.GetAt(i);
					if(Group=="Multiple Plant") Group="Plant";
					else if(Group=="Snow") Group="SnowPack";
					Type="Parameters";
					vv=m_pDoc->GetPtrVector(string(Type),string(Group), true);
					if(vv.size()>0){
						for(size_t ii=0;ii<vv.size();ii++) {
							pSw=(SimB*)vv[ii];
							float val=pSw->GetDBValue();
							if(val!=pSw->GetOriginalValue()) {
								QuickSetNumber(ix,Counter, val);
								QuickSetBackColor(ix,Counter,m_ColorClick);
								if(readonly)
									QuickSetBackColor(ix,Counter,m_ColorReadOnlyClick);
							}
							else {
								QuickSetNumber(ix,Counter, val);
								QuickSetBackColor(ix,Counter,m_ColorHeading);
								if(readonly)
									QuickSetBackColor(ix,Counter,m_ColorReadOnly);
							}
							if(!pSw->IsDBEnabled())
								QuickSetBackColor(ix,Counter, m_ColorDisable);
							Counter++;
						}
					}

				}

		}
		CString test;
		int  numcol;
		numcol=pEDIT->m_arrIdKey.GetSize();
	/*	for (int i=0;i<P_Counter;i++) {
				colst=0;
				colend=colst+1;
				str=QuickGetText(colst,i);
				while(colend<numcol) {
				 if(QuickGetText(colend,i)==str) {
	
				 }
				 else {
					if(colend-1>colst)
						JoinCells(colst,i,colend-1,i);
					colst=colend;
					str=QuickGetText(colst,i);
				 }
				 colend++;
				}
				if(colend-1>colst)
					JoinCells(colst,i,colend-1,i);

		}

*/


					
	OnAdjustLeft(pEDIT->m_SelectedIndex);

	SetVScrollMode ( UG_SCROLLTRACKING );
	SetUniformRowHeight( TRUE );
}
void DBCug::Make_TableElementSheet()
{
	CUGCell cell; 
	CString str, Group, Type, Name;

	SimB *pSw;
	int Sw_Counter, P_Counter, Pt_Counter, Counter;
	Sw_Counter=P_Counter=Pt_Counter=0;
	vector<SimB*> vv;

	EnableMenu(TRUE); 
	
	PP_EDIT *pEDIT;
	pEDIT=(PP_EDIT*)m_pPP_EDIT_Dlg;
	SetSheetNumber(2);
	SetDefColWidth(40);
	SetDefFont(m_fontID);
	BOOL readonly;

  //int i=str.ReverseFind(':');

  //while(if<m_arr_Idnum.GetSize) {
//	if(m_arr_Idnum[i]==
//
  //}
 // m_ArrayIndex=m_SelectedIndex;
		for(int i=0;i<pEDIT->m_arrGroups.GetSize();i++) {
				Group=pEDIT->m_arrGroups.GetAt(i);
				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";
				Type="Tables";
				vv=m_pDoc->GetPtrVector(string(Type),string(Group));
				P_Counter+=vv.size();
		}	
		SetSH_NumberCols(3);
		SetNumberCols (pEDIT->m_arrIdKey.GetSize());
		SetNumberRows (P_Counter);
		SetSH_Width(380);
		SetSH_ColWidth(-3,90);
		SetSH_ColWidth(-2,250);
		SetSH_ColWidth(-1,40);
		QuickSetFont(-1,-1,&m_largeFont);
		QuickSetText(-1,-1,"Module  |    Name	    |   Current Value");
		Counter=0;
		for(int i=0;i<pEDIT->m_arrGroups.GetSize();i++) {
				Group=pEDIT->m_arrGroups.GetAt(i);
				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";
				Type="Tables";
				vv=m_pDoc->GetPtrVector(string(Type),string(Group));
				if(vv.size()>0){
					int num=vv.size();
					QuickSetCellTypeEx(-3,Counter,UGCT_NORMALMULTILINE);
					QuickSetText(-3,Counter,Group);
					QuickSetFont(-3,Counter,&m_largeFont);
					QuickSetAlignment(-3,Counter, UG_ALIGNLEFT);
					JoinCells(-3,Counter,-3,Counter+num-1);
					for(size_t ii=0;ii<vv.size();ii++) {
						SetColWidth(Counter,40);
						pSw=(SimB*)vv[ii];
						str=pSw->GetName().c_str();
						QuickSetText(-3,Counter,Group);
						QuickSetText(-2,Counter, str);
						QuickSetFont(-2,Counter,&m_largeFont);
						QuickSetAlignment(-2,Counter, UG_ALIGNLEFT);
						int val=((Tab*)pSw)->GetValue();
						QuickSetAlignment(-1,Counter, UG_ALIGNLEFT);
						QuickSetFont(-1,Counter,&m_largeFont);
						QuickSetNumber(-1,Counter, val);
						if(!pSw->IsEnabled())
							QuickSetBackColor(-1, Counter, m_ColorDisable);
						Counter++;
					}
				}
		}	

		string group= pEDIT->m_ID_DB_Group;
		for(int ix=0;ix<pEDIT->m_arrIdKey.GetSize();ix++) {
				CString testkey;
				testkey=pEDIT->m_arrIdKey[ix];
				QuickSetText(ix,-1, testkey);
				QuickSetFont(ix,-1,&m_largeFont);
				pEDIT->m_ArrayIndex=ix;
				
				if(ix<pEDIT->m_NumOriginalRecords||pEDIT->m_TotalReadOnly) 
					pEDIT->m_ReadOnly=TRUE;
				else {
					pEDIT->m_ReadOnly=FALSE;
				}
				m_pDoc->Reset();
				m_pDoc->Retrieve(group, string(testkey), true);
				if(ix<pEDIT->m_NumOriginalRecords)
					readonly=TRUE;
				else
					readonly=FALSE;
			//	SetDefFont(&m_font);
				Counter=0;
				for(int i=0;i<pEDIT->m_arrGroups.GetSize();i++) {
					Group=pEDIT->m_arrGroups.GetAt(i);
					if(Group=="Multiple Plant") Group="Plant";
					else if(Group=="Snow") Group="SnowPack";
					Type="Tables";
					vv=m_pDoc->GetPtrVector(string(Type),string(Group));
					if(vv.size()>0){
						for(size_t ii=0;ii<vv.size();ii++) {
							pSw=(SimB*)vv[ii];
							int val=((Tab*)pSw)->GetDBValue();
							if(val!=((Tab*)pSw)->GetOriginalValue()) {
								QuickSetNumber(ix,Counter, val);
								QuickSetBackColor(ix,Counter,m_ColorClick);
								if(readonly)
									QuickSetBackColor(ix,Counter,m_ColorReadOnlyClick);
							}
							else {
								QuickSetNumber(ix,Counter, val);
								QuickSetBackColor(ix,Counter,m_ColorHeading);
								if(readonly)
									QuickSetBackColor(ix,Counter,m_ColorReadOnly);
							}
							if(!pSw->IsDBEnabled())
								QuickSetBackColor(ix,Counter, m_ColorDisable);
							Counter++;
						}
					}

				}

		}
		CString test;
		int numcol;
		numcol=pEDIT->m_arrIdKey.GetSize();
	/*	for (int i=0;i<P_Counter;i++) {
				colst=0;
				colend=colst+1;
				str=QuickGetText(colst,i);
				while(colend<numcol) {
				 if(QuickGetText(colend,i)==str) {
	
				 }
				 else {
					if(colend-1>colst)
						JoinCells(colst,i,colend-1,i);
					colst=colend;
					str=QuickGetText(colst,i);
				 }
				 colend++;
				}
				if(colend-1>colst)
					JoinCells(colst,i,colend-1,i);

		}

*/


					
	OnAdjustLeft(pEDIT->m_SelectedIndex);

	SetVScrollMode ( UG_SCROLLTRACKING );
	SetUniformRowHeight( TRUE );
}
void DBCug::Make_ParameterTableSheet()
{
	CUGCell cell; 
	CString str, Group, Type, Name;


	SimB *pSw;

	int Sw_Counter, P_Counter, Pt_Counter, Counter;
	Sw_Counter=P_Counter=Pt_Counter=0;
	//SetDefFont(&m_font);
	EnableMenu(TRUE); 
	
	PP_EDIT *pEDIT;
	pEDIT=(PP_EDIT*)m_pPP_EDIT_Dlg;
	SetSheetNumber(3);
	SetDefColWidth(75);
	SetDefFont(m_fontID);
	BOOL readonly;
	vector<SimB*> vv;


	for(int i=0;i<pEDIT->m_arrGroups.GetSize();i++) {
			Group=pEDIT->m_arrGroups.GetAt(i);
			if(Group=="Multiple Plant") Group="Plant";
			else if(Group=="Snow") Group="SnowPack";
			Type="Table Parameters";
			vv=m_pDoc->GetPtrVector(string(Type),string(Group));
			if(vv.size()>0){
					for(size_t ii=0;ii<vv.size();ii++) {
						pSw=(SimB*)vv[ii];
						int num=((P*)pSw)->GetSize();
						P_Counter=P_Counter+num;
					}
				}
		}	
		SetSH_NumberCols(3);
		SetNumberCols (pEDIT->m_arrIdKey.GetSize());
		SetNumberRows (P_Counter);
		SetSH_Width(315);
		SetSH_ColWidth(-3,90);
		SetSH_ColWidth(-2,150);
		SetSH_ColWidth(-1,75);
		QuickSetText(-1,-1,"Module  |    Name	    |   Current Value");
		QuickSetFont(-1,-1,&m_largeFont);
		Counter=0;
		for(int i=0;i<pEDIT->m_arrGroups.GetSize();i++) {
				Group=pEDIT->m_arrGroups.GetAt(i);
				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";
				Type="Table Parameters";
				vv=m_pDoc->GetPtrVector(string(Type),string(Group));				
				if(vv.size()>0){
					int num=vv.size();
					QuickSetCellTypeEx(-3,Counter,UGCT_NORMALMULTILINE);
					QuickSetText(-3,Counter,Group);
					QuickSetFont(-3,Counter,&m_largeFont);
					QuickSetAlignment(-3,Counter, UG_ALIGNLEFT);
					int GrpSt=Counter;
					
					for(size_t ii=0;ii<vv.size();ii++) {
						pSw=(SimB*)vv[ii];
						str=pSw->GetName().c_str();
						CString strn;
						int numv=((P*)pSw)->GetSize();
						for(int i3=0;i3<numv;i3++) {
							QuickSetText(-3,Counter,Group);
							strn=str+"("+MFC_Util::ItoAscii(i3+1)+")";
							QuickSetText(-2,Counter,strn);
							QuickSetFont(-2,Counter,&m_largeFont);
							QuickSetAlignment(-2,Counter, UG_ALIGNLEFT);
							float val=((P*)pSw)->GetValue(i3);
							QuickSetAlignment(-1,Counter, UG_ALIGNLEFT);
							QuickSetNumber(-1,Counter, val);
							if(!pSw->IsEnabled())
								QuickSetBackColor(-1, Counter, m_ColorDisable);
							QuickSetFont(-1,Counter,&m_largeFont);
							Counter++;
						}
					}
					JoinCells(-3,GrpSt,-3,Counter-1);
				}

		}
	string group= pEDIT->m_ID_DB_Group;
	for(int ix=0;ix<pEDIT->m_arrIdKey.GetSize();ix++) {
				CString testkey;
				testkey=pEDIT->m_arrIdKey[ix];
				QuickSetText(ix,-1, testkey);
				QuickSetFont(ix,-1,&m_largeFont);
				pEDIT->m_ArrayIndex=ix;
				
				if(pEDIT->m_ArrayIndex<pEDIT->m_NumOriginalRecords||pEDIT->m_TotalReadOnly) 
					pEDIT->m_ReadOnly=TRUE;
				else {
					pEDIT->m_ReadOnly=FALSE;
				}
				m_pDoc->Reset();
				m_pDoc->Retrieve(group, string(testkey), true);
				//pEDIT->Reset();
				//pEDIT->Retrieve();
				if(ix<pEDIT->m_NumOriginalRecords)
					readonly=TRUE;
				else
					readonly=FALSE;
				
				Counter=0;
				for(int i=0;i<pEDIT->m_arrGroups.GetSize();i++) {
					Group=pEDIT->m_arrGroups.GetAt(i);
					if(Group=="Multiple Plant") Group="Plant";
					else if(Group=="Snow") Group="SnowPack";
					Type="Table Parameters";
					vv=m_pDoc->GetPtrVector(string(Type),string(Group));
					if(vv.size()>0){
						P *pCPt;
						for(size_t ii=0;ii<vv.size();ii++) {
							pSw=(SimB*)vv[ii];
							pCPt=(P*)pSw;
							int numn=pCPt->GetSize();
							for (int i3=0; i3<numn;i3++) {
								float val=pCPt->GetDBValue(i3);
								if(val!=pCPt->GetOriginalValue(i3)) {
									QuickSetNumber(ix,Counter, val);
									QuickSetBackColor(ix,Counter,m_ColorClick);
									if(readonly)
										QuickSetBackColor(ix,Counter,m_ColorReadOnlyClick);
								}
								else {
									QuickSetNumber(ix,Counter, val);
									QuickSetBackColor(ix,Counter,m_ColorHeading);
									if(readonly)
										QuickSetBackColor(ix,Counter,m_ColorReadOnly);
								}
								if(!pSw->IsDBEnabled())
									QuickSetBackColor(ix,Counter, m_ColorDisable);
								Counter++;
							}

						}
					}

				}

		}


		CString test;
		int  numcol;
		numcol=pEDIT->m_arrIdKey.GetSize();
	/*	for (int i=0;i<P_Counter;i++) {
				colst=0;
				colend=colst+1;
				str=QuickGetText(colst,i);
				while(colend<numcol) {
				 if(QuickGetText(colend,i)==str) {
	
				 }
				 else {
					if(colend-1>colst)
						JoinCells(colst,i,colend-1,i);
					colst=colend;
					str=QuickGetText(colst,i);
				 }
				 colend++;
				}
				if(colend-1>colst)
					JoinCells(colst,i,colend-1,i);

		}
	*/
		OnAdjustLeft(pEDIT->m_SelectedIndex);
		RedrawAll();
}
void DBCug::Make_ModelFileSheet()
{
	CUGCell cell; 
	CString str, Group, Type, Name;
	SimB *pSw;
	BOOL readonly;
	vector<SimB*> vv;

	int Sw_Counter, P_Counter, Pt_Counter, Counter;
	Sw_Counter=P_Counter=Pt_Counter=0;
	//SetDefFont(&m_font);
	EnableMenu(TRUE); 
	
	PP_EDIT *pEDIT;
		pEDIT=(PP_EDIT*)m_pPP_EDIT_Dlg;
		SetSheetNumber(4);
		SetDefColWidth(160);
		SetDefFont(m_fontID);
  //int i=str.ReverseFind(':');

  //while(if<m_arr_Idnum.GetSize) {
//	if(m_arr_Idnum[i]==
//
  //}
 // m_ArrayIndex=m_SelectedIndex;
		for(int i=0;i<pEDIT->m_arrGroups.GetSize();i++) {
				Group=pEDIT->m_arrGroups.GetAt(i);
				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";
				Type="Files";
				vv=m_pDoc->GetPtrVector(string(Type),string(Group));
				P_Counter+=vv.size();
		}	
		SetSH_NumberCols(2);
		SetNumberCols (pEDIT->m_arrIdKey.GetSize());
		SetNumberRows (P_Counter);
		SetSH_Width(240);
		SetSH_ColWidth(-2,90);
		SetSH_ColWidth(-1,150);

		Counter=0;
		for(int i=0;i<pEDIT->m_arrGroups.GetSize();i++) {
				Group=pEDIT->m_arrGroups.GetAt(i);
				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";
				Type="Files";
				vv=m_pDoc->GetPtrVector(string(Type),string(Group));
				if(vv.size()>0){
					int num=vv.size();
					QuickSetCellTypeEx(-2,Counter,UGCT_NORMALMULTILINE);
					QuickSetText(-2,Counter,Group);
					QuickSetFont(-2,Counter,&m_largeFont);
					QuickSetAlignment(-2,Counter, UG_ALIGNLEFT);
					int GrpSt=Counter;
					
					for(size_t ii=0;ii<vv.size();ii++) {
						QuickSetText(-2,Counter,Group);
						pSw=(SimB*)vv[ii];
						str=pSw->GetName().c_str();
						QuickSetText(-1,Counter,str);
						QuickSetFont(-1,Counter,&m_largeFont);
						QuickSetAlignment(-1,Counter, UG_ALIGNLEFT);
						Counter++;
					}
					JoinCells(-2,GrpSt,-2,Counter-1);
				}

		}
		string group= pEDIT->m_ID_DB_Group;
	for(int ix=0;ix<pEDIT->m_arrIdKey.GetSize();ix++) {
				CString testkey;
				testkey=pEDIT->m_arrIdKey[ix];
				QuickSetText(ix,-1, testkey);
				QuickSetFont(ix,-1,&m_largeFont);
				pEDIT->m_ArrayIndex=ix;
				
				if(pEDIT->m_ArrayIndex<pEDIT->m_NumOriginalRecords||pEDIT->m_TotalReadOnly) 
					pEDIT->m_ReadOnly=TRUE;
				else {
					pEDIT->m_ReadOnly=FALSE;
				}

				m_pDoc->Reset();
				m_pDoc->Retrieve(group, string(testkey), true);
				if(ix<pEDIT->m_NumOriginalRecords)
					readonly=TRUE;
				else
					readonly=FALSE;
				
				Counter=0;
				for(int i=0;i<pEDIT->m_arrGroups.GetSize();i++) {
					Group=pEDIT->m_arrGroups.GetAt(i);
					if(Group=="Multiple Plant") Group="Plant";
					else if(Group=="Snow") Group="SnowPack";
					Type="Files";
					vv=m_pDoc->GetPtrVector(string(Type),string(Group));
					if(vv.size()>0){
						for(size_t ii=0;ii<vv.size();ii++) {
							pSw=(SimB*)vv[ii];
								CString val;
								val=((F*)pSw)->GetDBStrValue().c_str();
								CString org=((F*)pSw)->GetOriginalStrValue().c_str();
								//if(val!=pSw->GetOriginalValue()) {
								QuickSetText(ix,Counter, val);
								if(org==val) { 
									QuickSetBackColor(ix,Counter,m_ColorHeading);
									if(readonly)
										QuickSetBackColor(ix,Counter,m_ColorReadOnly);
								}
								else {
									QuickSetBackColor(ix,Counter,m_ColorClick);
									if(readonly)
										QuickSetBackColor(ix,Counter,m_ColorReadOnlyClick);
								}
								if(!pSw->IsDBEnabled())
									QuickSetBackColor(ix,Counter, m_ColorDisable);
								Counter++;
							}

						}
					}

				}

	


		CString test;
		int numcol;
		numcol=pEDIT->m_arrIdKey.GetSize();
		int koll=GetNumberCols();
	/*	for (int i=0;i<P_Counter;i++) {
				colst=0;
				colend=colst+1;
				QuickGetText(colst,i, &str);
				while(colend<numcol) {
				 CString koll;
				 QuickGetText(colend,i, &koll);
				 if(koll==str) {
	
				 }
				 else {
					if(colend-1>colst)
						JoinCells(colst,i,colend-1,i);
					colst=colend;
					str=QuickGetText(colst,i);
				 }
				 colend++;
				}
				if(colend-1>colst)
					JoinCells(colst,i,colend-1,i);

		}
*/
				OnAdjustLeft(pEDIT->m_SelectedIndex);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the current cell in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int DBCug::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the top row or left column in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int DBCug::OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	
void DBCug::OnHitBottom(long numrows,long rowspast,long rowsfound)
{
}

/////////////////////////////////////////////////////////////////////////////
//	
void DBCug::OnHitTop(long numrows,long rowspast)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeCol
//		Sent when the user is over a separation line on the top heading
//		A return value of TRUE allows the possibiliy of a resize
int DBCug::OnCanSizeCol(int col)
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
void DBCug::OnColSizing(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSized
//		This is sent when the user finished sizing the 
//		given column (see above for more details)
void DBCug::OnColSized(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeRow
//		Sent when the user is over a separation line on the side heading
//		A return value of TRUE allows the possibiliy of a resize
int  DBCug::OnCanSizeRow(long row)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSizing
//	    Sent when the user is sizing a row The row that is being sized is given as 
//		well as the height. Plus the height can be modified 
//		at this point. This makes it easy to set min and max heights
void DBCug::OnRowSizing(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSized
//		This is sent when the user is finished sizing hte
//		given row ( see above for more details)
void DBCug::OnRowSized(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeSideHdg
//		This is sent when the user moves into position 
//		for sizing the width of the side heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int DBCug::OnCanSizeSideHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeTopHdg
//		This is sent when the user moves into position 
//		for sizing the height of the top heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int DBCug::OnCanSizeTopHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSizing
int DBCug::OnSideHdgSizing(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSized
int DBCug::OnSideHdgSized(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSized
int DBCug::OnTopHdgSized(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSizing
int DBCug::OnTopHdgSizing(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColChange
//		Sent whenever the current column changes
//		The old and the new columns are given
void DBCug::OnColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowChange
//		Sent whenever the current row changes
//		The old and the new rows are given
void DBCug::OnRowChange(long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellChange
//		Sent whenever the current cell changes rows or columns
void DBCug::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnLeftColChange
//		Sent whenever the left visible column in the grid changes
void DBCug::OnLeftColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopRowChange
//		Sent whenever the top visible row in the grid changes
void DBCug::OnTopRowChange(long oldrow,long newrow)
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
void DBCug::OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum)
{
	TRACE("DBCug::OnSelectionChanged called with startCol = %ld, startRow = %ld, endCol = %ld, endRow = %ld, blockNum = %ld.\n",
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
void DBCug::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
	CUGCell cell; 
	CString str;

	PP_EDIT *pEDIT;
	pEDIT=(PP_EDIT*)m_pPP_EDIT_Dlg;
	if(updn) {
		pEDIT->m_ArrayIndex=col;
		pEDIT->m_SelectedIndex=col;
		pEDIT->m_SelectedKey=pEDIT->m_arrIdKey[col];
		pEDIT->OnInitDialog();
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
void DBCug::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
//		Sent whenever the user double clicks the left mouse button within the grid
//	
//		'col' and 'row' are negative if the area clicked in is not a valid cell
//		'rect' the rectangle of the cell that was clicked in
//		'point' the point where the mouse was clicked
void DBCug::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	StartEdit();
}				 

/////////////////////////////////////////////////////////////////////////////
//	OnMouseMove
void DBCug::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user clicks the left mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DBCug::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	/*if ( !updn )
	{
		SelectRange ( col, 0, col, GetNumberRows()-1 );

	}
	if(updn)
		SortBy(col, UG_SORT_ASCENDING); 
	*/			//SortAndJoin(col, isheet);
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_RClicked
//		Sent whenever the user clicks the right mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DBCug::OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top heading
//	
//		'col' is negative if the area clicked in is not valid
void DBCug::OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_LClicked
//		Sent whenever the user clicks the left mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DBCug::OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
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
void DBCug::OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_DClicked
//		Sent whenever the user double clicks the left mouse button within the side heading
//	
//		'row' is negative if the area clicked in is not valid
void DBCug::OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_LClicked
//		Sent whenever the user clicks the left mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DBCug::OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_RClicked
//		Sent whenever the user clicks the right mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DBCug::OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top corner button
void DBCug::OnCB_DClicked(RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyDown
//		Sent whenever the user types when the grid has focus. The keystroke can be
//		modified here as well. (See WM_KEYDOWN for more information)
void DBCug::OnKeyDown(UINT *vcKey,BOOL processed)
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
void DBCug::OnCharDown(UINT *vcKey,BOOL processed)
{
}
	
/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent everytime the grid needs to
//		draw a cell in the grid. At this point the cell
//		class has been filled with the information to be
//		used to draw the cell. The information can now be
//		changed before it is used for drawing
void DBCug::OnGetCell(int col,long row,CUGCell *cell)
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
void DBCug::OnSetCell(int col,long row,CUGCell *cell)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDataSourceNotify
//		This message is sent from a data source , message
//		depends on the data source - check the information
//		on the data source(s) being used
//		- The ID of the Data source is also returned
void DBCug::OnDataSourceNotify(int ID,long msg,long param)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellTypeNotify
//		This message is sent from a cell type , message
//		depends on the cell type - check the information
//		on the cell type classes
//		- The ID of the cell type is given
int DBCug::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	NewMap *p_fmap;
	SimB *pSw;
	CUGCell cell, cell1;
	string Type, Group, Name, Prior, After, test;
	PP_EDIT *pEDIT;
	pEDIT=(PP_EDIT*)m_pPP_EDIT_Dlg;

	if(col<pEDIT->m_NumOriginalRecords||pEDIT->m_TotalReadOnly)
		return FALSE;

	string group= pEDIT->m_ID_DB_Group;
	if(ID==1 && msg == 101) {
		GetCell(col,row, &cell);
		m_Prior=cell.GetText();
	}

	else if(ID==1 && msg == 103&&col>=pEDIT->m_NumOriginalRecords){
			int iold;

			iold=pEDIT->m_ArrayIndex;

			pEDIT->m_ArrayIndex=col;
			string testkey;
			testkey=pEDIT->m_arrIdKey[col];
				
		//	pEDIT->Reset();
		//	pEDIT->Retrieve();
				m_pDoc->Reset();
				m_pDoc->Retrieve(group, testkey, true);

			GetCell(col,row, &cell);
			m_After=cell.GetText();
			if(m_Prior!=m_After) {
				GetCell(-2,row, &cell1);
				cell1.GetStdText(&Name);
				GetCell(-3,row, &cell1);
				cell1.GetStdText(&Group);
				Type="Switches";

				pSw=m_pDoc->GetPtr(Type, Group, Name);
				((Sw*)pSw)->SetStrValue(string(m_After));
				//((Sw*)pSw)->ApplyDBValue();
			}

			pEDIT->Save();
			OnSheetSetup(0);

			pEDIT->m_ArrayIndex=iold;
			//pEDIT->Reset();
			//pEDIT->Retrieve();
				testkey=pEDIT->m_arrIdKey[iold];
				m_pDoc->Reset();
				m_pDoc->Retrieve(group, testkey, true);

	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditStart
//		This message is sent whenever the grid is ready to start editing a cell
//		A return of TRUE allows the editing a return of FALSE stops editing
//		Plus the properties of the CEdit class can be modified
int DBCug::OnEditStart(int col, long row,CWnd **edit)
{
	PP_EDIT *pEDIT;
	pEDIT=(PP_EDIT*)m_pPP_EDIT_Dlg;
	if(col<pEDIT->m_NumOriginalRecords||pEDIT->m_TotalReadOnly)
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditVerify
//		This is send when the editing is about to end
int DBCug::OnEditVerify(int col, long row,CWnd *edit,UINT *vcKey)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int DBCug::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR str1,BOOL cancelFlag)
{
	CString str;
	edit->GetWindowText(str);
	NewMap *p_fmap;
	SimB *pSw;
	CUGCell cell, cell1;
	string Type, Group, Name, Prior, After, test;
	int isheet=GetSheetNumber();
	int iold;
	PP_EDIT *pEDIT;
	pEDIT=(PP_EDIT*)m_pPP_EDIT_Dlg;

	iold=pEDIT->m_ArrayIndex;

	if(col<pEDIT->m_NumOriginalRecords||pEDIT->m_TotalReadOnly)
		return TRUE;
	pEDIT->m_ArrayIndex=col;
				
	//pEDIT->Reset();
	//pEDIT->Retrieve();
	string group= pEDIT->m_ID_DB_Group;
	string testkey;
	testkey=pEDIT->m_arrIdKey[col];
	m_pDoc->Reset();
	m_pDoc->Retrieve(group, string(testkey), true);

	if(isheet==1) {
			GetCell(-2,row, &cell1);
			cell1.GetStdText(&Name);
			GetCell(-3,row, &cell1);
			cell1.GetStdText(&Group);
			Type="Parameters";

			float value=MFC_Util::AtoFloat(str);
			pSw=m_pDoc->GetPtr(Type, Group, Name);
			if(pSw!=NULL) {
				((Ps*)pSw)->SetDBValue(value);
				//((Ps*)Sw)->ApplyDBValue();
				PP_EDIT *pEDIT;
				pEDIT=(PP_EDIT*)m_pPP_EDIT_Dlg;
				pEDIT->Save();
				OnSheetSetup(isheet);
			}
	}
	else if(isheet==2) {
			GetCell(-2,row, &cell1);
			cell1.GetStdText(&Name);
			GetCell(-3,row, &cell1);
			cell1.GetStdText(&Group);
			Type="Tables";
			size_t value=MFC_Util::AtoFloat(str);
			pSw=m_pDoc->GetPtr(Type, Group, Name);
			if(pSw!=NULL) {
				((Tab*)pSw)->SetDB_Row(value);
				//((Tab*)pSw)->ApplyDBValue();
				PP_EDIT *pEDIT;
				pEDIT=(PP_EDIT*)m_pPP_EDIT_Dlg;
				pEDIT->Save();
				OnSheetSetup(isheet);
			}
	}
	else if(isheet==3) {
			GetCell(-2,row, &cell1);
			cell1.GetStdText(&Name);
			int TabIndex;
			int ist=Name.find("(");
			int iend=Name.find(")");
			if(ist>0&&iend>ist) {
					string str=Name.substr(ist+1,iend-ist-1);
					TabIndex=FUtil::AtoInt(str);			
					TabIndex--;
					Name=Name.substr(0,ist);
					Type="Table Parameters";
			}

			GetCell(-3,row, &cell1);
			cell1.GetStdText(&Group);
			float value=MFC_Util::AtoFloat(str);
			pSw=m_pDoc->GetPtr(Type, Group, Name);
			if(pSw!=NULL) {
				((P*)pSw)->SetDBValue(TabIndex,value);
				//((P*)pSw)->ApplyDBValue(TabIndex);

				PP_EDIT *pEDIT;
				pEDIT=(PP_EDIT*)m_pPP_EDIT_Dlg;
				pEDIT->Save();
				OnSheetSetup(isheet);

			}

	}
	else if(isheet==4) {
			GetCell(-1,row, &cell1);
			cell1.GetStdText(&Name);
			GetCell(-2,row, &cell1);
			cell1.GetStdText(&Group);
			Type="Files";
			float value=MFC_Util::AtoFloat(str);
			pSw=m_pDoc->GetPtr(Type, Group, Name);
			if(pSw!=NULL) {
				((F*)pSw)->SetDBValue(string(str));
				PP_EDIT *pEDIT;
				pEDIT=(PP_EDIT*)m_pPP_EDIT_Dlg;
				pEDIT->Save();
				OnSheetSetup(isheet);

			}

	}
	group= pEDIT->m_ID_DB_Group;
	pEDIT->m_ArrayIndex=iold;
	//pEDIT->Reset();
	//pEDIT->Retrieve();
	testkey=pEDIT->m_arrIdKey[iold];
	m_pDoc->Reset();
	m_pDoc->Retrieve(group, testkey, true);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int DBCug::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
//	OnHint
int DBCug::OnHint(int col,long row,int section,CString *string)
{
	string->Format("Col:%d Row:%ld",col,row);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnVScrollHint
int DBCug::OnVScrollHint(long row,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHScrollHint
int DBCug::OnHScrollHint(int col,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OLE
#ifdef __AFXOLE_H__

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  DBCug::OnDragEnter(COleDataObject* pDataObject){
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  DBCug::OnDragOver(COleDataObject* pDataObject,int col,long row){
	//return DROPEFFECT_NONE;
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  DBCug::OnDragDrop(COleDataObject* pDataObject,int col,long row){
	return DROPEFFECT_COPY;
}
#endif

/////////////////////////////////////////////////////////////////////////////
//	
void DBCug::OnScreenDCSetup(CDC *dc,int section){
}

/////////////////////////////////////////////////////////////////////////////
//	OnSortEvaluate
//	return		-1  <
//				0	==
//				1	>
int DBCug::OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags)
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
void DBCug::OnTabSelected(int ID)
{
	SetSheetNumber(ID) ;
	OnSheetSetup(ID);
}

/////////////////////////////////////////////////////////////////////////////
//	OnAdjustComponentSizes
void DBCug::OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,
		RECT *cnrBtn,RECT *vScroll,RECT *hScroll,RECT *tabs)
{
} 

/////////////////////////////////////////////////////////////////////////////
//	OnDrawFocusRect
void DBCug::OnDrawFocusRect(CDC *dc,RECT *rect)
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

void DBCug::DrawExcelFocusRect(CDC *dc,RECT *rect)
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
COLORREF DBCug::OnGetDefBackColor(int section)
{
	return GetSysColor ( COLOR_APPWORKSPACE );
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetFocus
void DBCug::OnSetFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKillFocus
void DBCug::OnKillFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapStart
BOOL DBCug::OnColSwapStart(int col)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanColSwap
BOOL DBCug::OnCanColSwap(int fromCol,int toCol)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTrackingWindowMoved
void DBCug::OnTrackingWindowMoved(RECT *origRect,RECT *newRect)
{
}


