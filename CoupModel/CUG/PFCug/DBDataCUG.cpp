/////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived DBDataCug

#include "..\..\stdafx.h"
#include "..\..\Util.h"
#include "..\..\resource.h"
#include "..\..\ModelTypes\NewModelType.h"
#include "DBDataCug.h"
#include "DB_BasCug.h"
#include "..\..\DB\DB_Header.h"
//#include "..\SP\SP_Retrieve.h"
//#include "..\ParameterPlotDlg.h"


/////////////////////////////////////////////////////////////////////////////
// DBDataCug construction/destruction
DBDataCug::DBDataCug()
{
//	UGXPThemes::UseThemes(true);
	m_bDrawExcelFocus = TRUE;
	m_col=1;
	m_row=1;
	m_DataBase=TRUE;
	m_CompleteList=FALSE;
	m_pCTab=nullptr;
	m_pDB2=nullptr;
	m_pDoc=nullptr;
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
	m_pCUG=nullptr;
	m_fontsList = nullptr;

	GetDefaultFont(m_font);
	GetDefaultLargeFont(m_largeFont);

	m_pDB=MySingle::instance().GetDB_HeaderPointer();
	m_ShowAllObjects=false;
	m_LatestSheet=0;
	m_pDB_Bas=nullptr;
	m_AllDataBaseItems = true;
}

DBDataCug::~DBDataCug()
{
	_tagFonts *pFont, *pTemp;
	pFont = m_fontsList;
	while ( pFont != nullptr )
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

void DBDataCug::OnUpdateDataBase()
{

	if(m_pDoc!=nullptr) {
		m_pSwTimeRes=(Sw*)m_pDoc->GetPtr(SWITCH,"TimeResolution");
		m_pSwPlantType=(Sw*)m_pDoc->GetPtr(SWITCH,"PlantType");
	}





}
BOOL DBDataCug::OnAdjustLeft(int col)
{
	SetLeftCol(col);
	RedrawAll();
	return TRUE;

}
void DBDataCug::OnSetup()
{
	CUGCell cell; 
	CString str, Group, Type, Name;


	int Sw_Counter, P_Counter, Pt_Counter;
	Sw_Counter=P_Counter=Pt_Counter=0;
	if(m_pDoc==nullptr)  return;
	m_pDB_Bas=MySingle::instance().GetDBBasPointer();
	EnableMenu(TRUE); 

	//	SetDefFont(&m_font);

		int num_sheets=GetNumberSheets();
		if (num_sheets == 8) return;
		for (long i=0;i<num_sheets;i++) {
			DeleteTab(i);
		}
		num_sheets = GetNumberSheets();
		SetNumberSheets(1);
		SetDefColWidth(130);
		AddTab("General Info", 0); 
		AddTab("Switches", 1); 
		AddTab("Parameters", 2); 
		AddTab("Table Elements", 3); 
		AddTab("Parameter Tables", 4); 
		AddTab("Model Files", 5); 
		AddTab("Link To Table in Multi Run Document", 6); 
		AddTab("Link To Change During Run", 7); 
		SetTabWidth(400); 
		SetNumberSheets(8); 
		SetSheetNumber(0); 
		OnSheetSetup(0);
		SetCurrentTab(0);


	
	
}

/////////////////////////////////////////////////////////////////////////////
//	OnSheetSetup	
void DBDataCug::OnSheetSetup(int sheetNumber)
{

	UNREFERENCED_PARAMETER(sheetNumber);
	CUGCell cell; 
	CString heading, number, test;

	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	CString string, Defstring;
	CString optionlist;
	int MaxCol=8;
	int colv=0;
	if (m_pDoc == nullptr) return;
	BOOL Tableexpand;

	Tableexpand=FALSE;
	EnableMenu(TRUE); 
	SetMultiSelectMode ( TRUE );
	//DragDropTarget ( TRUE );
			SetSheetNumber(sheetNumber);

			EnableUpdate(FALSE); 
			while(GetNumberRows()>0)
				DeleteRow(0);
			EnableUpdate(TRUE); 
			switch(sheetNumber)		{
			case (0): //Switches
				Make_GeneralInfoSheet();
				m_LatestSheet=0;
				RedrawAll();
				break;
			case (1): //Switches
				Make_SwitchesSheet();
				m_LatestSheet=1;
				RedrawAll();
				break;
			case (2): // Parameters
				Make_ParameterSheet();
				m_LatestSheet=2;
				RedrawAll(); 
				break;
			case (3): // Parameters
				Make_TableElementSheet();
				m_LatestSheet=3;
				RedrawAll(); 
				break;
			case(4): // Parameters Tables
				Make_ParameterTableSheet();
				m_LatestSheet=4;
				break;
			case(5): // Model Files
				Make_ModelFileSheet();
				m_LatestSheet=5;
				RedrawAll();
				break;
			case(6): // Link To Document Files
				Make_LinkToDocumentSheet();
				m_LatestSheet=6;
				RedrawAll();
				break;
			case(7): // Link To Document Files
				Make_LinkToChangeDuringRunSheet();
				m_LatestSheet=7;
				RedrawAll();
				break;
			}


}
void DBDataCug::Make_LinkToDocumentSheet() {
	CUGCell cell;

	if(m_pDoc==nullptr) return;

	size_t db_index=m_pDB->GetCurrentDBIndex();
	int adjust=0;
	if(m_DataBaseId>0) adjust=1;
	string str=m_pDoc->DB_GetNames(m_DataBaseId+adjust);
	CDB *pDB;
	pDB=m_pDoc->DB_GetPointer(DB_types(db_index+adjust));
	size_t N_Dim=pDB->MR_Get_Dim();
	size_t NRep;
	size_t NSelected;

	if(N_Dim!=string::npos) {
		NRep=m_pDoc->MR_GetNumOfRepeatitionsOfDim(N_Dim-1);
		NSelected=pDB->MR_GetNumberOfSelections();
	}
	else {
		NRep=0;
		NSelected=0;
	}

	SetNumberCols(1);
	SetNumberRows(NRep);
	SetColWidth(-1, 120);
	SetColWidth(0,200);
	QuickSetText(-1,-1,"Repetition");
	str+=" Key";
	QuickSetText(0,-1,str.c_str());

	for(size_t i=0; i<NRep;i++) {
		QuickSetText(-1,i, FUtil::STD_ItoAscii(i+1));
		size_t dest=pDB->GetLastSelectedRepetition()+1;
		if(i==dest) QuickSetHBackColor(-1,i,m_ColorChanged);
		QuickSetText(0, i,pDB->MR_GetKeySelection(i));

	}

}
void DBDataCug::Make_LinkToChangeDuringRunSheet() {
	CUGCell cell;

	if(m_pDoc==nullptr) return;

	size_t db_index=m_pDB->GetCurrentDBIndex();
	int adjust=0;
	if(m_DataBaseId>0) adjust=1;
	string str=m_pDoc->DB_GetNames(m_DataBaseId+adjust);
	CDB *pDB;
	pDB=m_pDoc->DB_GetPointer(DB_types(db_index+adjust));

	size_t NumChaValues;


	NumChaValues=pDB->GetNumChaDates();

	SetNumberCols(2);
	SetNumberRows(NumChaValues+1);
	SetColWidth(-1, 120);
	SetColWidth(0,200);
	SetColWidth(1,200);
	QuickSetText(-1,-1,"");
	QuickSetText(0,-1,"Date To Change Value");
	str+=" Key";
	QuickSetText(1,-1,str.c_str());

	for(size_t i=0; i<NumChaValues;i++) {
		QuickSetText(-1,i, FUtil::STD_ItoAscii(i+1));
		size_t dest=pDB->GetLastSelectedRepetition()+1;
		if(i==dest) QuickSetHBackColor(-1,i,m_ColorChanged);
		QuickSetText(0, i, PGUtil::StringDatum(pDB->GetChaDate(i)));
		QuickSetText(1, i, pDB->GetChaParKey(i));

	}
	QuickSetText(-1,NumChaValues,"Next");

	long nextchadate=m_pDoc->GetNextChaParDate();
	QuickSetText(0,NumChaValues,PGUtil::StringDatum(nextchadate));


}
void DBDataCug::Make_GeneralInfoSheet() {
	CUGCell cell;

	if(m_pDoc==nullptr) return;


	SetNumberCols(2);
	SetNumberRows(12);
	SetColWidth(-1, 120);
	SetColWidth(0,180);
	SetColWidth(1, 500);
	
	int adjust=0;
	if(m_DataBaseId>0) adjust=1;
	string str=m_pDoc->DB_GetNames(m_DataBaseId+adjust);
	QuickSetText(-1,-1,str);
	QuickSetText(0,-1, "Info");
	QuickSetText(1,-1, "Details");
	QuickSetText(-1,0,"Type");
	QuickSetText(-1,1,"Name");
	QuickSetText(-1,2,"Site Name");
	QuickSetText(-1,3,"Latitude");
	QuickSetText(-1,4,"Longitude");
	QuickSetText(-1,5,"Country");
	QuickSetText(-1, 6, "Version");
	QuickSetText(-1, 7, "Date Created");
	QuickSetText(-1, 8, "Created by");
	QuickSetText(-1, 9, "Date Modified");
	QuickSetText(-1, 10, "Modified by");
	QuickSetText(-1,11,"Key");
	size_t Key=m_pDB->GetCurrentKeyIndex();
	//vector<size_t> compindex;
	//compindex = m_pDB->GetCompareLinkVector(m_DataBaseId);
	//if(compindex.size())>0)
	//	Key = compindex[compindex.size() - 1];

	DB_OldHeader header;
	DB_NewHeader headernew;
	header=m_pDB->GetHeader(m_DataBaseId, Key);
	headernew = m_pDB->GetNewHeader(m_DataBaseId, Key);
	GetCell(0,0,&cell);
	cell.SetCellType(UGCT_DROPLIST); 
	string optionlist;
	for(size_t j=0;j<m_pDB->m_PlantType[m_pDB->m_PlantTypeIndex[m_DataBaseId]].size();j++) {
		optionlist+=m_pDB->m_PlantType[m_pDB->m_PlantTypeIndex[m_DataBaseId]][j];
		optionlist+="\n";
	}
	cell.SetLabelText(optionlist.c_str());
	size_t typeindex=m_pDB->m_PlantTypeIndex[m_DataBaseId];
	if(typeindex<6) {
		if(m_pDB->m_PlantType[typeindex].size()>header.Type)
			str=m_pDB->m_PlantType[typeindex][header.Type];
		else
			str= m_pDB->m_PlantType[typeindex][m_pDB->m_PlantType[typeindex].size()-1];
	}

	cell.SetText(str.c_str());
	SetCell(0,0,&cell);
	QuickSetText(0,1,header.Name);
	QuickSetText(0,2,header.SiteName);
	QuickSetNumber(0,3,FUtil::AtoFloat(header.Latitude));
	QuickSetNumber(0,4,FUtil::AtoFloat(header.Longitude));
	QuickSetText(0,5,header.Country);
	QuickSetText(0, 6, headernew.ModelVersion);
	QuickSetText(0, 7, headernew.CreateDate);
	QuickSetText(0, 8, headernew.CreatedBy);
	QuickSetText(0, 9, headernew.ModifiedDate);
	QuickSetText(0, 10, headernew.ModifiedBy);

	QuickSetText(0,11,m_pDB->GetKeyString(header.IdUser, header.IdNum));
	GetCell(0,6, &cell);
	cell.SetReadOnly(TRUE);
	SetCell(0,6,&cell);
	for(int i=0; i<10;i++)	{
		UnJoinCells(1,i);
		UnJoinCells(0,i);

	}
	JoinCells(1,0,1,11);
	RedrawAll();
	GetCell(1,0,&cell);
	//cell.ClearAll();
	//QuickSetCellTypeEx(-3,Counter,UGCT_NORMALMULTILINE);
	cell.SetCellTypeEx(UGCT_NORMALMULTILINE);
	cell.SetAlignment(UG_ALIGNLEFT);
	string cstr=header.Details;
	cell.SetText(cstr);
	SetCell(1,0,&cell);

	RedrawAll();


}
void DBDataCug::Make_SwitchesSheet()
{
	CUGCell cell; 
	string str, Group, Type, Name;

	SimB *pSw;

	int Sw_Counter, P_Counter, Pt_Counter, Counter;
	Sw_Counter=P_Counter=Pt_Counter=0;
	EnableMenu(TRUE); 
	SetSheetNumber(1);
	SetDefColWidth(130);
	SetDefFont(m_fontID);
	BOOL readonly;


	for(size_t i=0;i<m_pDB->GetNumGroups(m_DataBaseId);i++) {
				Group=m_pDB->GetGroupString(m_DataBaseId, i).c_str();
				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";
				Type="Switches";
				vector<SimB*> vv;
				vv=m_pDoc->GetPtrVector(Type,Group,m_AllDataBaseItems);
				Sw_Counter+=vv.size();
		}	
	   	SetSH_NumberCols(3);
		vector<size_t> compindex;
		compindex = m_pDB->GetCompareLinkVector(m_DataBaseId);
		if(m_ShowAllObjects)
			SetNumberCols (m_pDB->GetNumOfRec(m_DataBaseId));
		else
			SetNumberCols(compindex.size());
		SetNumberRows (Sw_Counter+2);
		SetSH_Width(390);
		SetSH_ColWidth(-3,90);
		SetSH_ColWidth(-2,150);
		SetSH_ColWidth(-1,150);
		for(size_t i=0; i<GetNumberCols(); i++)
			SetColWidth(i, 140);

		Counter=0;

		pSw=m_pSwTimeRes;	
		str=m_pSwTimeRes->GetName().c_str();
		QuickSetFont(-2,Counter,&m_largeFont);
		QuickSetText(-2,Counter,str);

		QuickSetText(-3,Counter,"Technical");
		QuickSetAlignment(-2,Counter, UG_ALIGNLEFT);
		int i1=((Sw*)pSw)->GetIntValue();
		str=((Sw*)pSw)->GetOption(i1).c_str();
		QuickSetAlignment(-1,Counter, UG_ALIGNLEFT);
		QuickSetText(-1,Counter, str);
				
		Counter++;
		pSw=m_pSwPlantType;	
		str=pSw->GetName();;
		QuickSetFont(-2,Counter,&m_largeFont);
		QuickSetText(-2,Counter,str);
		QuickSetText(-3,Counter,"Model Structure");
		QuickSetAlignment(-2,Counter, UG_ALIGNLEFT);
		i1=((Sw*)pSw)->GetIntValue();
		str=((Sw*)pSw)->GetOption(i1).c_str();
		QuickSetAlignment(-1,Counter, UG_ALIGNLEFT);
		QuickSetText(-1,Counter, str);
		Counter++;




		for(size_t i=0;i<m_pDB->GetNumGroups(m_DataBaseId);i++) {
				Group=m_pDB->GetGroupString(m_DataBaseId, i).c_str();
				Type="Switches";
				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";

				QuickSetText(-1,-1,"Module  |    Name	    |   Current Value");
				vector<SimB*> vv;
				vv=m_pDoc->GetPtrVector(Type,Group, m_AllDataBaseItems);
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
						int i1=((Sw*)pSw)->GetIntValue();
						str=((Sw*)pSw)->GetOption(i1).c_str();
						QuickSetAlignment(-1,Counter, UG_ALIGNLEFT);
						QuickSetText(-1,Counter, str);
						if(!pSw->IsEnabled())
							QuickSetBackColor(-1, Counter, m_ColorDisable);
						Counter++;
					}
				}

		}	

		string group= m_pDB->GetGroupString(m_DataBaseId, 0).c_str();//pEDIT->m_ID_DB_Group;

		size_t NumRecordsToShow;
		if(m_ShowAllObjects) NumRecordsToShow=m_pDB->GetNumOfRec(m_DataBaseId);
		else NumRecordsToShow = compindex.size();
		for(size_t ix=0;ix<NumRecordsToShow;ix++) { 
				string testkey;
				DB_OldHeader header;
				size_t rec_to_show;
				if(m_ShowAllObjects) rec_to_show=ix;
				else rec_to_show= compindex[ix];
				header=m_pDB->GetHeader(m_DataBaseId, rec_to_show); 
				testkey=m_pDB->GetKeyString(header.IdUser,header.IdNum);                   //pEDIT->m_arrIdKey[ix];
				if(header.IdNum>0&& header.IdUser!="") {
					QuickSetText(ix,-1, testkey);
					QuickSetFont(ix,-1,&m_largeFont);
					//pEDIT->m_ArrayIndex=ix;
					m_pDoc->Reset();
					m_pDoc->Retrieve(GetDataBaseName(), string(testkey), true);
					if(ix<m_pDB->GetNumOriginal(m_DataBaseId)){//;m_NumOriginalRecords) {
						readonly=TRUE;
						QuickSetBackColor(ix,-1, m_ColorChanged);
					}
					else {
						readonly=FALSE;
						QuickSetBackColor(ix,-1, m_ColorHeading);
					}
				}
				else
					readonly = TRUE;
				
				Counter=0;
				CString optionlist;	
				size_t i1, n;

				pSw=m_pSwTimeRes;
				while(pSw!=nullptr) {
					GetCell(ix,Counter, &cell); 
					cell.SetCellType(UGCT_DROPLIST); 
					optionlist=_T("");
					n=((Sw*)pSw)->GetNumOptions();
					for(int j=0;j<n;j++) {
						optionlist+=((Sw*)pSw)->GetOption(j).c_str();
						optionlist+="\n";
					}
					cell.SetLabelText(optionlist);
					i1=((Sw*)pSw)->GetDBIntValue();
					if(i1!=pSw->GetOriginalIntValue()) {
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
					UnJoinCells(ix, Counter);
					SetCell(ix,Counter,&cell);
					Counter++;
					if(Counter==1)
						pSw=m_pSwPlantType;
								//Sw=m_pSwPlantType;	
					else
						pSw=nullptr;
				}
				vector<SimB*> vv;
				
				for(size_t i=0;i<m_pDB->GetNumGroups(m_DataBaseId);i++) {
					Group=m_pDB->GetGroupString(m_DataBaseId, i).c_str();
					//Group=pEDIT->m_arrGroups.GetAt(i);
					if(Group=="Multiple Plant") Group="Plant";
					else if(Group=="Snow") Group="SnowPack";
					Type="Switches";
					vv=m_pDoc->GetPtrVector(Type,Group, m_AllDataBaseItems);
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
							if (i1 == string::npos) {
								cell.SetText("-");
								cell.SetBackColor(m_ColorHeading);
							}						
							else if(i1!=pSw->GetOriginalIntValue()) {
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
			
		int colst, colend, numcol;
		numcol=GetNumberCols();//pEDIT->m_arrIdKey.GetSize();
		for (int i=0;i<Sw_Counter;i++) {
				colst=0;
				colend=colst+1;
				for (size_t ic = 0; ic<numcol; ic++)
					UnJoinCells(ic, i);
				str=QuickGetStdText(colst,i);
				while(colend<numcol) {
					 if(QuickGetStdText(colend,i)==str) {
	
					 }
					 else {
						if(colend-1>colst)
							JoinCells(colst,i,colend-1,i);
						colst=colend;
						str=QuickGetStdText(colst,i);
					 }
					 colend++;
				}
				if(colend-1>colst)
					JoinCells(colst,i,colend-1,i);

		}
		RedrawAll();
	
	OnAdjustLeft(m_pDB->GetCurrentKeyIndex());
	SetVScrollMode ( UG_SCROLLTRACKING );
	SetUniformRowHeight( TRUE );
}
void DBDataCug::Make_ParameterSheet()
{
	CUGCell cell; 
	string str, Group, Type, Name;

	SimB *pSw;
	Ps *pPs;
	int Sw_Counter, P_Counter, Pt_Counter, Counter;
	Sw_Counter=P_Counter=Pt_Counter=0;

	EnableMenu(TRUE); 
	

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

		for(size_t i=0;i<m_pDB->GetNumGroups(m_DataBaseId);i++) {
				Group=m_pDB->GetGroupString(m_DataBaseId, i).c_str();
				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";
				Type="Parameters";
				vv=m_pDoc->GetPtrVector(Type,Group, m_AllDataBaseItems);
				P_Counter+=vv.size();
		}	
		SetSH_NumberCols(3);
		vector<size_t> compindex;
		compindex = m_pDB->GetCompareLinkVector(m_DataBaseId);
		if(m_ShowAllObjects)
			SetNumberCols (m_pDB->GetNumOfRec(m_DataBaseId));
		else
			SetNumberCols (compindex.size());
		SetNumberRows (P_Counter);
		SetSH_Width(315);
		SetSH_ColWidth(-3,90);
		SetSH_ColWidth(-2,150);
		SetSH_ColWidth(-1,75);
		for (size_t i = 0; i<GetNumberCols(); i++)
			SetColWidth(i, 100);


		QuickSetFont(-1,-1,&m_largeFont);
		string strdoc =FUtil::DirectoryNameOnly( m_pDoc->GetCurrentSimFile());
		QuickSetText(-1,-1,"Module |  Name  | Current Value ("+strdoc+")");
		Counter=0;

		m_pDoc->SetGroupConnections(GetDataBaseName());
		for(size_t i=0;i<m_pDB->GetNumGroups(m_DataBaseId);i++) {
				Group=m_pDB->GetGroupString(m_DataBaseId, i).c_str();
				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";
				Type="Parameters";
				vv=m_pDoc->GetPtrVector(Type,Group, m_AllDataBaseItems);
				if(vv.size()>0){
					int num=vv.size();
					QuickSetCellTypeEx(-3,Counter,UGCT_NORMALMULTILINE);
					QuickSetText(-3,Counter,Group);
					QuickSetFont(-3,Counter,&m_largeFont);
					QuickSetAlignment(-3,Counter, UG_ALIGNLEFT);
					JoinCells(-3,Counter,-3,Counter+num-1);
					for(size_t ii=0;ii<vv.size();ii++) {
						pSw=(SimB*)vv[ii];
						pPs=dynamic_cast<Ps*>(vv[ii]);
						str=pSw->GetName().c_str();
						QuickSetText(-3,Counter,Group);
						QuickSetText(-2,Counter, str);
						QuickSetFont(-2,Counter,&m_largeFont);
						QuickSetAlignment(-2,Counter, UG_ALIGNLEFT);
						float val=pSw->GetValue();
						val=pPs->GetValue();
						QuickSetAlignment(-1,Counter, UG_ALIGNLEFT);
						QuickSetFont(-1,Counter,&m_largeFont);
						QuickSetNumber(-1,Counter, val);
						if(!pSw->IsEnabled())
							QuickSetBackColor(-1, Counter, m_ColorDisable);
						Counter++;
					}
				}
		}	


		//for(int ix=0;ix<pEDIT->m_arrIdKey.GetSize();ix++) {
		size_t NumRecordsToShow;
		if(m_ShowAllObjects) NumRecordsToShow=m_pDB->GetNumOfRec(m_DataBaseId);
		else NumRecordsToShow=compindex.size();
			for(size_t ix=0;ix<NumRecordsToShow;ix++) {
				string testkey;
				DB_OldHeader header;
				size_t rec_to_show;
				if(m_ShowAllObjects) rec_to_show=ix;
				else rec_to_show=compindex[ix];
				header=m_pDB->GetHeader(m_DataBaseId, rec_to_show);
				testkey=m_pDB->GetKeyString(header.IdUser,header.IdNum);     
				QuickSetText(ix,-1, testkey);
				QuickSetFont(ix,-1,&m_largeFont);
			//	pEDIT->m_ArrayIndex=ix;
				//if(ix<m_pDB->GetNumOriginal(m_DataBaseId)){//;m_NumOriginalRecords) {
				//	readonly=TRUE;
				//	QuickSetBackColor(ix,-1, m_ColorChanged);
				//}
				//else {
					readonly=FALSE;
					QuickSetBackColor(ix,-1, m_ColorHeading);

				//}				

				m_pDoc->Reset();

				if (m_pDoc->Retrieve(GetDataBaseName(), string(testkey), true)) {
					Counter = 0;
					for (size_t i = 0; i < m_pDB->GetNumGroups(m_DataBaseId); i++) {
						Group = m_pDB->GetGroupString(m_DataBaseId, i).c_str();


						if (Group == "Multiple Plant") Group = "Plant";
						else if (Group == "Snow") Group = "SnowPack";
						Type = "Parameters";
						vv = m_pDoc->GetPtrVector(Type, Group, m_AllDataBaseItems);
						if (vv.size() > 0) {
							for (size_t ii = 0; ii < vv.size(); ii++) {
								pPs = dynamic_cast<Ps*>(vv[ii]);
								float val = pPs->GetDBValue();
								GetCell(ix, Counter, &cell);
								cell.SetCellType(UGCT_NORMAL);
								if (val != pPs->GetOriginalValue() && val > 0.9*MISSING) {
									cell.SetNumber(val);
									cell.SetBackColor(m_ColorClick);
									if (readonly)
										cell.SetBackColor(m_ColorReadOnlyClick);
								}
								else if (val < 0.9*MISSING) {
									cell.SetText("-");
									cell.SetBackColor(m_ColorHeading);



								}
								else {
									cell.SetNumber(val);
									cell.SetBackColor(m_ColorHeading);
									if (readonly)
										cell.SetBackColor(m_ColorReadOnly);
								}
								if (!pPs->IsDBEnabled())
									cell.SetBackColor(m_ColorDisable);
								SetCell(ix, Counter, &cell);
								Counter++;
							}
						}

					}
				}
		}

		CString test;
		int  numcol;
		int colst, colend;
		numcol= GetNumberCols();
		for (int i=0;i<P_Counter;i++) {
				
				colst=0;
				colend=colst+1;
				for(size_t ic=0;ic<numcol; ic++)
					UnJoinCells(ic, i);
				str=QuickGetStdText(colst,i);
				while(colend<numcol) {
				 if(QuickGetStdText(colend,i)==str) {
	
				 }
				 else {
					if(colend-1>colst)
						JoinCells(colst,i,colend-1,i);
					colst=colend;
					str=QuickGetStdText(colst,i);
				 }
				 colend++;
				}
				if(colend-1>colst)
					JoinCells(colst,i,colend-1,i);

		}
		RedrawAll();

	OnAdjustLeft(m_pDB->GetCurrentKeyIndex());
	SetVScrollMode ( UG_SCROLLTRACKING );
	SetUniformRowHeight( TRUE );
}
void DBDataCug::Make_TableElementSheet()
{
	CUGCell cell; 
	string str, Group, Type, Name;


	Tab *pTab;
	int Sw_Counter, P_Counter, Pt_Counter, Counter;
	Sw_Counter=P_Counter=Pt_Counter=0;
	vector<SimB*> vv;

	EnableMenu(TRUE); 
	

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
	for(size_t i=0;i<m_pDB->GetNumGroups(m_DataBaseId);i++) {
				Group=m_pDB->GetGroupString(m_DataBaseId, i).c_str();
				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";
				Type="Tables";
				vv=m_pDoc->GetPtrVector(Type,Group, m_AllDataBaseItems);
				P_Counter+=vv.size();
		}	
		SetSH_NumberCols(3);
		vector<size_t> compindex;
		compindex = m_pDB->GetCompareLinkVector(m_DataBaseId);
		if(m_ShowAllObjects)
			SetNumberCols (m_pDB->GetNumOfRec(m_DataBaseId));
		else
			SetNumberCols (compindex.size());

		SetNumberRows (P_Counter);
		SetSH_Width(380);
		SetSH_ColWidth(-3,90);
		SetSH_ColWidth(-2,250);
		SetSH_ColWidth(-1,40);
		QuickSetFont(-1,-1,&m_largeFont);
		QuickSetText(-1,-1,"Module  |    Name	    |   Current Value");
		Counter=0;

		for(size_t i=0;i<m_pDB->GetNumGroups(m_DataBaseId);i++) {
				Group=m_pDB->GetGroupString(m_DataBaseId, i).c_str();
				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";
				Type="Tables";
				vv=m_pDoc->GetPtrVector(Type,Group, m_AllDataBaseItems);
				if(vv.size()>0){
					int num=vv.size();
					QuickSetCellTypeEx(-3,Counter,UGCT_NORMALMULTILINE);
					QuickSetText(-3,Counter,Group);
					QuickSetFont(-3,Counter,&m_largeFont);
					QuickSetAlignment(-3,Counter, UG_ALIGNLEFT);
					JoinCells(-3,Counter,-3,Counter+num-1);
					for(size_t ii=0;ii<vv.size();ii++) {
						SetColWidth(Counter,40);
						pTab=dynamic_cast<Tab*>(vv[ii]);
						str=pTab->GetName().c_str();
						QuickSetText(-3,Counter,Group);
						QuickSetText(-2,Counter, str);
						QuickSetFont(-2,Counter,&m_largeFont);
						QuickSetAlignment(-2,Counter, UG_ALIGNLEFT);
						int val;//=((Tab*)pSw)->GetValue();
						val=pTab->GetNumOfRow();
						QuickSetAlignment(-1,Counter, UG_ALIGNLEFT);
						QuickSetFont(-1,Counter,&m_largeFont);
						QuickSetNumber(-1,Counter, val);
						if(!pTab->IsEnabled())
							QuickSetBackColor(-1, Counter, m_ColorDisable);
						Counter++;
					}
				}
		}	

		size_t NumRecordsToShow;
		if(m_ShowAllObjects) NumRecordsToShow=m_pDB->GetNumOfRec(m_DataBaseId);
		else NumRecordsToShow= compindex.size();
		for(size_t ix=0;ix<NumRecordsToShow;ix++) { 
				string testkey;
				DB_OldHeader header;
				size_t rec_to_show;
				if(m_ShowAllObjects) rec_to_show=ix;
				else rec_to_show= compindex[ix];
				header=m_pDB->GetHeader(m_DataBaseId, rec_to_show);
				testkey=m_pDB->GetKeyString(header.IdUser,header.IdNum); 
				QuickSetText(ix,-1, testkey);
				QuickSetFont(ix,-1,&m_largeFont);


				m_pDoc->Reset();
				m_pDoc->Retrieve(GetDataBaseName(), string(testkey), true);

				if(ix<m_pDB->GetNumOriginal(m_DataBaseId)){//;m_NumOriginalRecords) {
					readonly=TRUE;
					QuickSetBackColor(ix,-1, m_ColorChanged);
				}
				else {
					readonly=FALSE;
					QuickSetBackColor(ix,-1, m_ColorHeading);

				}
			//	SetDefFont(&m_font);
				Counter=0;
				for(size_t i=0;i<m_pDB->GetNumGroups(m_DataBaseId);i++) {
					Group=m_pDB->GetGroupString(m_DataBaseId, i).c_str();
					if(Group=="Multiple Plant") Group="Plant";
					else if(Group=="Snow") Group="SnowPack";
					Type="Tables";
					vv=m_pDoc->GetPtrVector(Type,Group, m_AllDataBaseItems);
					if(vv.size()>0){
						for(size_t ii=0;ii<vv.size();ii++) {
							//pSw=(SimB*)vv[ii];
							pTab=dynamic_cast<Tab*>(vv[ii]);
							int val;//=((Tab*)pSw)->GetDBValue();
							val=pTab->GetDB_Row();
							if(val!=pTab->GetNumOfOriginalRow()) {
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
							if(!pTab->IsDBEnabled())
								QuickSetBackColor(ix,Counter, m_ColorDisable);
							Counter++;
						}
					}

				}

		}
		CString test;
		int numcol;
		numcol=m_pDB->GetNumOfRec(m_DataBaseId);
		int colst, colend;
		numcol = GetNumberCols();
		for (int i = 0; i<P_Counter; i++) {

			colst = 0;
			colend = colst + 1;
			for (size_t ic = 0; ic<numcol; ic++)
				UnJoinCells(ic, i);
			str = QuickGetStdText(colst, i);
			while (colend<numcol) {
				if (QuickGetStdText(colend, i) == str) {

				}
				else {
					if (colend - 1>colst)
						JoinCells(colst, i, colend - 1, i);
					colst = colend;
					str = QuickGetStdText(colst, i);
				}
				colend++;
			}
			if (colend - 1>colst)
				JoinCells(colst, i, colend - 1, i);

		}
		RedrawAll();

	/*	for (int i=0;i<P_Counter;i++) {
				colst=0;
				colend=colst+1;
				str=QuickGetStdText(colst,i);
				while(colend<numcol) {
				 if(QuickGetStdText(colend,i)==str) {
	
				 }
				 else {
					if(colend-1>colst)
						JoinCells(colst,i,colend-1,i);
					colst=colend;
					str=QuickGetStdText(colst,i);
				 }
				 colend++;
				}
				if(colend-1>colst)
					JoinCells(colst,i,colend-1,i);

		}


*/

					
	OnAdjustLeft(m_pDB->GetCurrentKeyIndex());

	SetVScrollMode ( UG_SCROLLTRACKING );
	SetUniformRowHeight( TRUE );
}
void DBDataCug::Make_ParameterTableSheet()
{
	CUGCell cell; 
	string str, Group, Type, Name;


	SimB *pSw;
	P *pP;

	int Sw_Counter, P_Counter, Pt_Counter, Counter;
	Sw_Counter=P_Counter=Pt_Counter=0;
	//SetDefFont(&m_font);
	EnableMenu(TRUE); 


	SetSheetNumber(3);
	SetDefColWidth(75);
	SetDefFont(m_fontID);
	BOOL readonly;
	vector<SimB*> vv;



	for(size_t i=0;i<m_pDB->GetNumGroups(m_DataBaseId);i++) {
			Group=m_pDB->GetGroupString(m_DataBaseId, i).c_str();
			if(Group=="Multiple Plant") Group="Plant";
			else if(Group=="Snow") Group="SnowPack";
			Type="Table Parameters";
			vv=m_pDoc->GetPtrVector(Type,Group, m_AllDataBaseItems);
			if(vv.size()>0){
					for(size_t ii=0;ii<vv.size();ii++) {
						pSw=(SimB*)vv[ii];
						int num=((P*)pSw)->GetSize();
						P_Counter=P_Counter+num;
					}
				}
		}	
		SetSH_NumberCols(3);
		vector<size_t> compindex;
		compindex = m_pDB->GetCompareLinkVector(m_DataBaseId);
		if(m_ShowAllObjects)
			SetNumberCols (m_pDB->GetNumOfRec(m_DataBaseId));
		else
			SetNumberCols (compindex.size());
		SetNumberRows (P_Counter);
		SetSH_Width(315);
		SetSH_ColWidth(-3,90);
		SetSH_ColWidth(-2,150);
		SetSH_ColWidth(-1,75);
		QuickSetText(-1,-1,"Module  |    Name	    |   Current Value");
		QuickSetFont(-1,-1,&m_largeFont);
		Counter=0;

		for(size_t i=0;i<m_pDB->GetNumGroups(m_DataBaseId);i++) {
				Group=m_pDB->GetGroupString(m_DataBaseId, i).c_str();

				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";
				Type="Table Parameters";
				vv=m_pDoc->GetPtrVector(Type,Group, m_AllDataBaseItems);
				if(vv.size()>0){
					int num=vv.size();
					QuickSetCellTypeEx(-3,Counter,UGCT_NORMALMULTILINE);
					QuickSetText(-3,Counter,Group);
					QuickSetFont(-3,Counter,&m_largeFont);
					QuickSetAlignment(-3,Counter, UG_ALIGNLEFT);
					int GrpSt=Counter;
					
					for(size_t ii=0;ii<vv.size();ii++) {
						pP=dynamic_cast<P*>(vv[ii]);
						str=pP->GetName().c_str();
						string strn;
						int numv=pP->GetSize();
						for(int i3=0;i3<numv;i3++) {
							QuickSetText(-3,Counter,Group);
							strn=str+"("+FUtil::STD_ItoAscii(i3+1)+")";
							QuickSetText(-2,Counter,strn);
							QuickSetFont(-2,Counter,&m_largeFont);
							QuickSetAlignment(-2,Counter, UG_ALIGNLEFT);
							float val=pP->GetValue(i3);
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
	//string group= pEDIT->m_ID_DB_Group;
		size_t NumRecordsToShow;
		if(m_ShowAllObjects) NumRecordsToShow=m_pDB->GetNumOfRec(m_DataBaseId);
		else NumRecordsToShow= compindex.size();
		for(size_t ix=0;ix<NumRecordsToShow;ix++) { 
				string testkey;
				DB_OldHeader header;
				size_t rec_to_show;
				if(m_ShowAllObjects) rec_to_show=ix;
				else rec_to_show= compindex[ix];
				header=m_pDB->GetHeader(m_DataBaseId, rec_to_show);
				testkey=m_pDB->GetKeyString(header.IdUser,header.IdNum);     
				QuickSetText(ix,-1, testkey);
				QuickSetFont(ix,-1,&m_largeFont);
				if(ix<m_pDB->GetNumOriginal(m_DataBaseId)){//;m_NumOriginalRecords) {
					readonly=TRUE;
					QuickSetBackColor(ix,-1, m_ColorChanged);
				}
				else {
					readonly=FALSE;
					QuickSetBackColor(ix,-1, m_ColorHeading);

				}
				m_pDoc->Reset();
				m_pDoc->Retrieve(GetDataBaseName(), string(testkey), true);
				//pEDIT->Reset();
				//pEDIT->Retrieve();
			
				Counter=0;
				for(size_t i=0;i<m_pDB->GetNumGroups(m_DataBaseId);i++) {
					Group=m_pDB->GetGroupString(m_DataBaseId, i).c_str();	

					if(Group=="Multiple Plant") Group="Plant";
					else if(Group=="Snow") Group="SnowPack";
					Type="Table Parameters";
					vv=m_pDoc->GetPtrVector(Type,Group, m_AllDataBaseItems);
					if(vv.size()>0){

						for(size_t ii=0;ii<vv.size();ii++) {
							pP=dynamic_cast<P*>(vv[ii]);
							int numn=pP->GetSize();
							for (int i3=0; i3<numn;i3++) {
								float val=pP->GetDBValue(i3);
								if(val!=pP->GetOriginalValue(i3)) {
									if(val<=0.9*MISSING) {
										QuickSetText(ix,Counter, "-");
										QuickSetBackColor(ix,Counter,m_ColorHeading);
									}
									else {
										QuickSetNumber(ix,Counter, val);
										QuickSetBackColor(ix,Counter,m_ColorClick);
									}
									if(readonly)
										QuickSetBackColor(ix,Counter,m_ColorReadOnlyClick);
								}
								else {
									if(val<=0.9*MISSING) {
										QuickSetText(ix,Counter, "-");
										QuickSetBackColor(ix,Counter,m_ColorClick);
									}
									else {
										QuickSetNumber(ix,Counter, val);
										QuickSetBackColor(ix,Counter,m_ColorHeading);
									}
									if(readonly)
										QuickSetBackColor(ix,Counter,m_ColorReadOnly);
								}
								if(!pP->IsDBEnabled())
									QuickSetBackColor(ix,Counter, m_ColorDisable);
								Counter++;
							}

						}
					}

				}

		}
		CString test;
		int  numcol;
		int colst, colend;
		numcol = GetNumberCols();
		for (int i = 0; i<P_Counter; i++) {

			colst = 0;
			colend = colst + 1;
			for (size_t ic = 0; ic<numcol; ic++)
				UnJoinCells(ic, i);
			str = QuickGetStdText(colst, i);
			while (colend<numcol) {
				if (QuickGetStdText(colend, i) == str) {

				}
				else {
					if (colend - 1>colst)
						JoinCells(colst, i, colend - 1, i);
					colst = colend;
					str = QuickGetStdText(colst, i);
				}
				colend++;
			}
			if (colend - 1>colst)
				JoinCells(colst, i, colend - 1, i);

		}
		RedrawAll();


		OnAdjustLeft(m_pDB->GetCurrentKeyIndex());
		RedrawAll();
}
void DBDataCug::Make_ModelFileSheet()
{
	CUGCell cell; 
	string str, Group, Type, Name;
	SimB *pSw;
	F* pF;
	BOOL readonly;
	vector<SimB*> vv;

	int Sw_Counter, P_Counter, Pt_Counter, Counter;
	Sw_Counter=P_Counter=Pt_Counter=0;
	//SetDefFont(&m_font);
	EnableMenu(TRUE); 


		SetSheetNumber(4);
		SetDefColWidth(160);
		SetDefFont(m_fontID);
  //int i=str.ReverseFind(':');

  //while(if<m_arr_Idnum.GetSize) {
//	if(m_arr_Idnum[i]==
//
  //}
 // m_ArrayIndex=m_SelectedIndex;
	
	for(size_t i=0;i<m_pDB->GetNumGroups(m_DataBaseId);i++) {
				Group=m_pDB->GetGroupString(m_DataBaseId, i).c_str();
				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";
				Type="Files";
				vv=m_pDoc->GetPtrVector(Type,Group, m_AllDataBaseItems);
				P_Counter+=vv.size();
		}	
		SetSH_NumberCols(2);
		vector<size_t> compindex;
		compindex = m_pDB->GetCompareLinkVector(m_DataBaseId);
		if(m_ShowAllObjects)
			SetNumberCols (m_pDB->GetNumOfRec(m_DataBaseId));
		else
			SetNumberCols (compindex.size());
		SetNumberRows (P_Counter);
		SetSH_Width(240);
		SetSH_ColWidth(-2,90);
		SetSH_ColWidth(-1,150);

		Counter=0;
		for(size_t i=0;i<m_pDB->GetNumGroups(m_DataBaseId);i++) {
				Group=m_pDB->GetGroupString(m_DataBaseId, i).c_str();
				if(Group=="Multiple Plant") Group="Plant";
				else if(Group=="Snow") Group="SnowPack";
				Type="Files";
				vv=m_pDoc->GetPtrVector(Type,Group, m_AllDataBaseItems);
				if(vv.size()>0){
					int num=vv.size();
					QuickSetCellTypeEx(-2,Counter,UGCT_NORMALMULTILINE);
					QuickSetText(-2,Counter,Group);
					QuickSetFont(-2,Counter,&m_largeFont);
					QuickSetAlignment(-2,Counter, UG_ALIGNLEFT);
					int GrpSt=Counter;
					
					for(size_t ii=0;ii<vv.size();ii++) {
						QuickSetText(-2,Counter,Group);
						//pSw=(SimB*)vv[ii];
						pF=dynamic_cast<F*>(vv[ii]);
						str=pF->GetName().c_str();
						QuickSetText(-1,Counter,str);
						QuickSetFont(-1,Counter,&m_largeFont);
						QuickSetAlignment(-1,Counter, UG_ALIGNLEFT);
						Counter++;
					}
					JoinCells(-2,GrpSt,-2,Counter-1);
				}

		}
		size_t NumRecordsToShow;
		if (m_ShowAllObjects) NumRecordsToShow = m_pDB->GetNumOfRec(m_DataBaseId);
		else NumRecordsToShow = compindex.size();

		for(int ix=0;ix<NumRecordsToShow;ix++) {
				string testkey;
				DB_OldHeader header;
				size_t rec_to_show;
				if (m_ShowAllObjects) rec_to_show = ix;
				else rec_to_show = compindex[ix];
				header=m_pDB->GetHeader(m_DataBaseId, rec_to_show); 
				testkey=m_pDB->GetKeyString(header.IdUser,header.IdNum); 
				QuickSetText(ix,-1, testkey);
				QuickSetFont(ix,-1,&m_largeFont);


				m_pDoc->Reset();
				m_pDoc->Retrieve(GetDataBaseName(), string(testkey), true);
				if(ix<m_pDB->GetNumOriginal(m_DataBaseId)){//;m_NumOriginalRecords) {
					readonly=TRUE;
					QuickSetBackColor(ix,-1, m_ColorChanged);
				}
				else {
					readonly=FALSE;
					QuickSetBackColor(ix,-1, m_ColorHeading);

				}
				
				Counter=0;
				for(size_t i=0;i<m_pDB->GetNumGroups(m_DataBaseId);i++) {
					Group=m_pDB->GetGroupString(m_DataBaseId, i);
					if(Group=="Multiple Plant") Group="Plant";
					else if(Group=="Snow") Group="SnowPack";
					Type="Files";
					vv=m_pDoc->GetPtrVector(Type,Group, m_AllDataBaseItems);
					if(vv.size()>0){
						for(size_t ii=0;ii<vv.size();ii++) {
							pF=dynamic_cast<F*>(vv[ii]);
								string val;
								val=pF->GetDBStrValue().c_str();
								string org=pF->GetOriginalStrValue();
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
								if(!pF->IsDBEnabled())
									QuickSetBackColor(ix,Counter, m_ColorDisable);
								Counter++;
							}

						}
					}

				}

	


		CString test;
		int numcol;
		numcol=m_pDB->GetNumOfRec(m_DataBaseId);
		int koll=GetNumberCols();
	/*	for (int i=0;i<P_Counter;i++) {
				colst=0;
				colend=colst+1;
				QuickGetStdText(colst,i, &str);
				while(colend<numcol) {
				 CString koll;
				 QuickGetStdText(colend,i, &koll);
				 if(koll==str) {
	
				 }
				 else {
					if(colend-1>colst)
						JoinCells(colst,i,colend-1,i);
					colst=colend;
					str=QuickGetStdText(colst,i);
				 }
				 colend++;
				}
				if(colend-1>colst)
					JoinCells(colst,i,colend-1,i);

		}*/
		OnAdjustLeft(m_pDB->GetCurrentKeyIndex());
		
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the current cell in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int DBDataCug::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the top row or left column in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int DBDataCug::OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	
void DBDataCug::OnHitBottom(long numrows,long rowspast,long rowsfound)
{
}

/////////////////////////////////////////////////////////////////////////////
//	
void DBDataCug::OnHitTop(long numrows,long rowspast)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeCol
//		Sent when the user is over a separation line on the top heading
//		A return value of TRUE allows the possibiliy of a resize
int DBDataCug::OnCanSizeCol(int col)
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
void DBDataCug::OnColSizing(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSized
//		This is sent when the user finished sizing the 
//		given column (see above for more details)
void DBDataCug::OnColSized(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeRow
//		Sent when the user is over a separation line on the side heading
//		A return value of TRUE allows the possibiliy of a resize
int  DBDataCug::OnCanSizeRow(long row)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSizing
//	    Sent when the user is sizing a row The row that is being sized is given as 
//		well as the height. Plus the height can be modified 
//		at this point. This makes it easy to set min and max heights
void DBDataCug::OnRowSizing(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSized
//		This is sent when the user is finished sizing hte
//		given row ( see above for more details)
void DBDataCug::OnRowSized(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeSideHdg
//		This is sent when the user moves into position 
//		for sizing the width of the side heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int DBDataCug::OnCanSizeSideHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeTopHdg
//		This is sent when the user moves into position 
//		for sizing the height of the top heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int DBDataCug::OnCanSizeTopHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSizing
int DBDataCug::OnSideHdgSizing(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSized
int DBDataCug::OnSideHdgSized(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSized
int DBDataCug::OnTopHdgSized(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSizing
int DBDataCug::OnTopHdgSizing(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColChange
//		Sent whenever the current column changes
//		The old and the new columns are given
void DBDataCug::OnColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowChange
//		Sent whenever the current row changes
//		The old and the new rows are given
void DBDataCug::OnRowChange(long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellChange
//		Sent whenever the current cell changes rows or columns
void DBDataCug::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnLeftColChange
//		Sent whenever the left visible column in the grid changes
void DBDataCug::OnLeftColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopRowChange
//		Sent whenever the top visible row in the grid changes
void DBDataCug::OnTopRowChange(long oldrow,long newrow)
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
void DBDataCug::OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum)
{
	TRACE("DBDataCug::OnSelectionChanged called with startCol = %ld, startRow = %ld, endCol = %ld, endRow = %ld, blockNum = %ld.\n",
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
void DBDataCug::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
	CUGCell cell; 
	CString str;
	if(updn&&col==-1) {


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
void DBDataCug::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
//		Sent whenever the user double clicks the left mouse button within the grid
//	
//		'col' and 'row' are negative if the area clicked in is not a valid cell
//		'rect' the rectangle of the cell that was clicked in
//		'point' the point where the mouse was clicked
void DBDataCug::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	StartEdit();
}				 

/////////////////////////////////////////////////////////////////////////////
//	OnMouseMove
void DBDataCug::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user clicks the left mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DBDataCug::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if( !updn ) {
		if (m_AllDataBaseItems) m_AllDataBaseItems = false;
		else m_AllDataBaseItems = true;


		OnSheetSetup(m_LatestSheet);
			

	}

}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_RClicked
//		Sent whenever the user clicks the right mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DBDataCug::OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top heading
//	
//		'col' is negative if the area clicked in is not valid
void DBDataCug::OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_LClicked
//		Sent whenever the user clicks the left mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DBDataCug::OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
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
void DBDataCug::OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_DClicked
//		Sent whenever the user double clicks the left mouse button within the side heading
//	
//		'row' is negative if the area clicked in is not valid
void DBDataCug::OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_LClicked
//		Sent whenever the user clicks the left mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DBDataCug::OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_RClicked
//		Sent whenever the user clicks the right mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void DBDataCug::OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top corner button
void DBDataCug::OnCB_DClicked(RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyDown
//		Sent whenever the user types when the grid has focus. The keystroke can be
//		modified here as well. (See WM_KEYDOWN for more information)
void DBDataCug::OnKeyDown(UINT *vcKey,BOOL processed)
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
void DBDataCug::OnCharDown(UINT *vcKey,BOOL processed)
{
}
	
/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent everytime the grid needs to
//		draw a cell in the grid. At this point the cell
//		class has been filled with the information to be
//		used to draw the cell. The information can now be
//		changed before it is used for drawing
void DBDataCug::OnGetCell(int col,long row,CUGCell *cell)
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
void DBDataCug::OnSetCell(int col,long row,CUGCell *cell)
{
	if(GetSheetNumber()==6&&col==0) {
		string text=cell->GetStdText();


	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnDataSourceNotify
//		This message is sent from a data source , message
//		depends on the data source - check the information
//		on the data source(s) being used
//		- The ID of the Data source is also returned
void DBDataCug::OnDataSourceNotify(int ID,long msg,long param)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellTypeNotify
//		This message is sent from a cell type , message
//		depends on the cell type - check the information
//		on the cell type classes
//		- The ID of the cell type is given
int DBDataCug::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	NewMap *p_fmap;
	SimB *pSw;
	CUGCell cell, cell1;
	string Type, Group, Name, Prior, After, test;
	size_t db_index=m_pDB->GetCurrentDBIndex(true);
	size_t rec_index=m_pDB->GetCurrentKeyIndex();

	if(rec_index<m_pDB->GetNumOriginal(db_index))
		return TRUE;
	string databasename;


	databasename=m_pDoc->DB_GetNames(db_index,false);

	DB_OldHeader header;
	header=m_pDB->GetHeader(db_index, rec_index);

	if(ID==1 && msg == 101) {
		GetCell(col,row, &cell);
		m_Prior=cell.GetStdText();
	}

	else if(ID==1 && msg == 103){
			int iold;
			int isheet=GetSheetNumber();

			if(isheet==0) {
				GetCell(col,row, &cell);
				After=cell.GetStdText();
				size_t option=FUtil::GetOptionIndex(cell.GetStdLabelText(),After);
				header.Type=option;
				if(m_pDB->SetHeader(db_index, rec_index, header)) {
					m_pDB->SetDBModified(db_index);
					DB_NewHeader headernew;
					headernew = m_pDB->GetNewHeader(db_index, rec_index);
					headernew.ModifiedDate = MFC_Util::GetCurrentDateTime();
					headernew.ModifiedBy = FUtil::GetWindowUser();
					m_pDB->SetNewHeader(db_index, rec_index, headernew);


					m_pDB_Bas->MakeDBHeaderSheet(db_index);

				}

			}
			else {
				//Sparade ändringar av switch values


				//	pEDIT->Reset();
				//	pEDIT->Retrieve();
			//	m_pDoc->Reset();
			//	m_pDoc->Retrieve(group, testkey, true);

				GetCell(col, row, &cell);
				m_After = cell.GetStdText();
				if (m_Prior != m_After) {
					GetCell(-2, row, &cell1);
					cell1.GetStdText(&Name);
					GetCell(-3, row, &cell1);
					cell1.GetStdText(&Group);
					Type = "Switches";

					pSw = m_pDoc->GetPtr(Type, Group, Name);
					((Sw*)pSw)->SetStrValue(string(m_After));
					//((Sw*)pSw)->ApplyDBValue();
				}

				//pEDIT->Save();
				OnSheetSetup(0);

				//	pEDIT->m_ArrayIndex=iold;
				//pEDIT->Reset();
				//pEDIT->Retrieve();
					//testkey=pEDIT->m_arrIdKey[iold];
				m_pDoc->Reset();
				//m_pDoc->Retrieve(group, testkey, true);
			}

	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditStart
//		This message is sent whenever the grid is ready to start editing a cell
//		A return of TRUE allows the editing a return of FALSE stops editing
//		Plus the properties of the CEdit class can be modified
int DBDataCug::OnEditStart(int col, long row,CWnd **edit)
{

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditVerify
//		This is send when the editing is about to end
int DBDataCug::OnEditVerify(int col, long row,CWnd *edit,UINT *vcKey)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int DBDataCug::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR str1,BOOL cancelFlag)
{
	string str;
	CString c_str;
	edit->GetWindowText(c_str);
	str = CT2A(c_str).m_psz;
	NewMap *p_fmap;
	SimB *pSw;
	CUGCell cell, cell1;
	string Type, Group, Name, Prior, After, test;
	int isheet=GetSheetNumber();
	int iold;

	size_t db_index=m_pDB->GetCurrentDBIndex(true); //To retrieve value 0-9
	size_t rec_index=m_pDB->GetCurrentKeyIndex();

	if (rec_index < m_pDB->GetNumOriginal(db_index) && isheet < 6) {
		MFC_Util::MessageBox("Original Data Base Object - Can not be changed");
		return TRUE;

	}
	DB_OldHeader header;
	header=m_pDB->GetHeader(db_index, rec_index);
	DB_NewHeader headernew;
	headernew = m_pDB->GetNewHeader(db_index, rec_index);

				
	//pEDIT->Reset();
	//pEDIT->Retrieve();
	string databasename;

	databasename=m_pDoc->DB_GetNames(db_index, false);
	string testkey;
	testkey=m_pDB->GetKeyString(header.IdUser, header.IdNum);
	m_pDoc->Reset();
	m_pDoc->Retrieve(databasename, testkey, true);
	if(isheet==0) {
		if(col==1&&row==0) {
			header.Details=string(str);

		}
		else if(col==0) {
			GetCell(col,row, &cell);
			int option;
			switch(row) {
				case 0:
//type		
					option=FUtil::GetOptionIndex(cell.GetStdLabelText(),str);
					header.Type=option;
					break;
				case 1:
					header.Name=str;
					break;
		
				case 2:
					header.SiteName=str;
					break;
				case 3:
					header.Latitude=str;
					break;
				case 4:
					header.Longitude=str;
					break;
				case 5:
					header.Country=str;
					break;
				case 6:
					if(headernew.CreateDate.size()==0)
						headernew.CreateDate = str;
					break;
				case 7:
					if (headernew.ModelVersion.size() == 0)
						headernew.ModelVersion = str;
					break;
				case 8:
					break;


			}
		
		}
	}
	else if(isheet==1) {
			GetCell(-2,row, &cell1);
			cell1.GetStdText(&Name);
			GetCell(-3,row, &cell1);
			cell1.GetStdText(&Group);
			Type="Parameters";

			float value=MFC_Util::AtoFloat(str);
			pSw=m_pDoc->GetPtr(Type, Group, Name);
			if(pSw!=nullptr) {
				((Ps*)pSw)->SetDBValue(value);

			}
	}
	else if(isheet==2) {
			GetCell(-2,row, &cell1);
			cell1.GetStdText(&Name);
			GetCell(-3,row, &cell1);
			cell1.GetStdText(&Group);
			Type="Tables";
			size_t value=size_t(MFC_Util::AtoFloat(str));
			pSw=m_pDoc->GetPtr(Type, Group, Name);

	}
	else if(isheet==3) {
			GetCell(-2,row, &cell1);
			cell1.GetStdText(&Name);
			int TabIndex=0;
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
			if(pSw!=nullptr) {
				((P*)pSw)->SetDBValue(TabIndex,value);
				//((P*)pSw)->ApplyDBValue(TabIndex);

		/*		PP_EDIT *pEDIT;
				pEDIT=(PP_EDIT*)m_pPP_EDIT_Dlg;
				pEDIT->Save();
				OnSheetSetup(isheet);*/

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
			if(pSw!=nullptr) {
				((F*)pSw)->SetDBValue(string(str));
	/*			PP_EDIT *pEDIT;
				pEDIT=(PP_EDIT*)m_pPP_EDIT_Dlg;
				pEDIT->Save();
				OnSheetSetup(isheet);*/

			}	  
				

	}
	else if(isheet==6) {
		
		CDB *pDB=m_pDoc->DB_GetPointer(DB_types(db_index));
		pDB->MR_SetSelection(row, databasename,str);

	}
	else if(isheet==7) {
		if(col==0) {
			CDB *pDB=m_pDoc->DB_GetPointer(DB_types(db_index));
			size_t NumSelected=pDB->GetNumChaDates();
			if(row==NumSelected) {
				long nextdate=PGUtil::MinutConv(str);
				if(m_pDoc!=nullptr)
					m_pDoc->SetNextChaParDate(nextdate);
			}


		}



	}

	if (m_pDB->SetHeader(db_index, rec_index, header)) {
		headernew.ModifiedDate = MFC_Util::GetCurrentDateTime();
		headernew.ModifiedBy = FUtil::GetWindowUser();
		m_pDB->SetNewHeader(db_index, rec_index, headernew);
		m_pDB->SetDBModified(db_index);
		m_pDB_Bas->MakeDBHeaderSheet(db_index);

	}
	if(isheet>1) {
		//Save DB-value to db object Files
		m_pDoc->Reset();
		m_pDoc->Retrieve(databasename, testkey, true);

	}


	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int DBDataCug::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	sections - UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//				UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
void DBDataCug::OnMenuCommand(int col,long row,int section,int item)
{
	if(section==UG_GRID||section==UG_SIDEHEADING || section == UG_TOPHEADING) {
		int sheetnumber;
		sheetnumber=GetSheetNumber();
		if(item==4000||item==4001) {
			if (item == 4000) {
				if (m_ShowAllObjects) m_ShowAllObjects = false;
				else m_ShowAllObjects = true;
			}
			else {
				if (m_AllDataBaseItems) m_AllDataBaseItems = false;
				else m_AllDataBaseItems = true;
			}
			OnSheetSetup(m_LatestSheet);				
		}

		if(item== 1000)
			CutSelected();

		if(item==2000)
			CopySelected();

		if(item==3000) {
			Paste();
			if(sheetnumber==6) {
				Set_Changed_MR_LinkToDocument();
			}
			else if(sheetnumber==7) 
				Set_Changed_LinkToChangeDuringRun_Document();

		}

	}
}


/////////////////////////////////////////////////////////////////////////////
//	return UG_SUCCESS to allow the menu to appear
//	return 1 to not allow the menu to appear
int DBDataCug::OnMenuStart(int col,long row,int section)
{
			//****** Populate the Menu 
	if(section==UG_GRID || section == UG_SIDEHEADING || section == UG_TOPHEADING) {
			EmptyMenu();
			if(GetSheetNumber()<6) {
				if(m_ShowAllObjects) AddMenuItem(4000,"Show only one Selected object (col) in Sheets");
				else AddMenuItem(4000,"Show all available objects (col) in sheets");
				if (m_AllDataBaseItems) AddMenuItem(4001, "Show Only enabled items (rows) in Sheets");
				else AddMenuItem(4001, "Show All items (rows) in sheets");
			}
			AddMenuItem(1000, "Cut"); 
			AddMenuItem(2000, "Copy"); 
			AddMenuItem(3000, "Paste"); 
	}
	else
		EmptyMenu();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHint
int DBDataCug::OnHint(int col,long row,int section,CString *string)
{
	string->Format(_T("Col:%d Row:%ld"),col,row);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnVScrollHint
int DBDataCug::OnVScrollHint(long row,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHScrollHint
int DBDataCug::OnHScrollHint(int col,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OLE
#ifdef __AFXOLE_H__

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  DBDataCug::OnDragEnter(COleDataObject* pDataObject){
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  DBDataCug::OnDragOver(COleDataObject* pDataObject,int col,long row){
	//return DROPEFFECT_NONE;
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  DBDataCug::OnDragDrop(COleDataObject* pDataObject,int col,long row){
	return DROPEFFECT_COPY;
}
#endif

/////////////////////////////////////////////////////////////////////////////
//	
void DBDataCug::OnScreenDCSetup(CDC *dc,int section){
}

/////////////////////////////////////////////////////////////////////////////
//	OnSortEvaluate
//	return		-1  <
//				0	==
//				1	>
int DBDataCug::OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags)
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
void DBDataCug::OnTabSelected(int ID)
{
	//SetSheetNumber(ID, TRUE) ;
	OnSheetSetup(ID);
}

/////////////////////////////////////////////////////////////////////////////
//	OnAdjustComponentSizes
void DBDataCug::OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,
		RECT *cnrBtn,RECT *vScroll,RECT *hScroll,RECT *tabs)
{
} 

/////////////////////////////////////////////////////////////////////////////
//	OnDrawFocusRect
void DBDataCug::OnDrawFocusRect(CDC *dc,RECT *rect)
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

void DBDataCug::DrawExcelFocusRect(CDC *dc,RECT *rect)
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
COLORREF DBDataCug::OnGetDefBackColor(int section)
{
	return GetSysColor ( COLOR_APPWORKSPACE );
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetFocus
void DBDataCug::OnSetFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKillFocus
void DBDataCug::OnKillFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapStart
BOOL DBDataCug::OnColSwapStart(int col)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanColSwap
BOOL DBDataCug::OnCanColSwap(int fromCol,int toCol)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTrackingWindowMoved
void DBDataCug::OnTrackingWindowMoved(RECT *origRect,RECT *newRect)
{
}
void DBDataCug::Make_Sheet()
{
	OnSheetSetup(m_LatestSheet);

}
void DBDataCug::Set_Changed_MR_LinkToDocument() {
	CUGCell cell;

	if(m_pDoc==nullptr) return;

	size_t db_index=m_pDB->GetCurrentDBIndex();
	int adjust=0;
	if(m_DataBaseId>0) adjust=1;
	string str=m_pDoc->DB_GetNames(m_DataBaseId+adjust);
	CDB *pDB;
	pDB=m_pDoc->DB_GetPointer(DB_types(db_index));
	size_t NRep=GetNumberRows();
	for(size_t i=0; i<NRep;i++) {
		string key_str=QuickGetStdText(0,i);
		pDB->MR_SetSelection(i, str, key_str);

	}

}
void DBDataCug::Set_Changed_LinkToChangeDuringRun_Document() {
	CUGCell cell;

	if(m_pDoc==nullptr) return;

	size_t db_index=m_pDB->GetCurrentDBIndex();
	int adjust=0;
	if(m_DataBaseId>0) adjust=1;
	string str=m_pDoc->DB_GetNames(m_DataBaseId+adjust);
	CDB *pDB;
	pDB=m_pDoc->DB_GetPointer(DB_types(db_index));
	size_t NRep=GetNumberRows();
	for(size_t i=0; i<NRep;i++) {
		string key_str=QuickGetStdText(0,i);
		pDB->MR_SetSelection(i, str, key_str);

	}

}


