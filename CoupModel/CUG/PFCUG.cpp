/////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived PFCug

#include "..\stdafx.h"
#include "..\Util.h"
#include "..\resource.h"
#include "..\ModelTypes\NewModelType.h"
#include "..\MultiRun\PP_Edit.h"
#include "PFCug.h"
#include "..\SP\SP_Retrieve.h"
#include ".\ChartCug.h"
#include "..\NewBase\Doc.h"
//#include "..\PlotPF\PlotPF\PFCurve.h"
//#include "..\ParameterPlotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(PFCug,CUGCtrl)
	//{{AFX_MSG_MAP(PFCug)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


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
	m_pCTab=NULL;
	m_pDB2=NULL;
	m_SelOne=FALSE;
	m_ColorChanged=RGB(255,200,200);
	m_ColorHeading=RGB(230,230,230);
	m_ColorClick=RGB(240,240,50);
	m_PrevClickRow=-1;
	m_DataImported=false;
	m_pPFCurve=MySingle::instance().GetPlotPFPointer();


	if(m_pPFCurve->IsOpen())
	if(m_pPFCurve->IsProfile()) {
	   m_IniProf=m_pPFCurve->m_ActProfile.KeyProf/100;
	   m_IniRepProf=m_pPFCurve->m_ActProfile.KeyProf%100;
	}
	else
		m_IniProf=-10;

	SetMultiSelectMode ( TRUE );
	DragDropTarget ( TRUE );
	m_pCUG=NULL;
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

void PFCug::OnUpdate(CDB *pDB, BOOL database, int maxrow)
{
	CUGCell cell; 
	m_pDB2=pDB;
	m_DataBase=database;
	m_row=maxrow;
	m_pCUG=NULL;
	m_SelOne=FALSE;

}
void PFCug::OnUpdate(CDB *pDB, BOOL database, UINT *pProf, UINT *pRepProf, PFCug *pCug, CDialog *pDlg, BOOL sel_one, CString str, Doc *pDoc, PFCug *pCug_Data)
{
	CUGCell cell; 
	m_pDB2=pDB;
	m_DataBase=database;
	m_pRepProf=pRepProf;
	m_pProf=pProf;
	m_pCUG=pCug;
	m_pPP_EDIT_Dlg=NULL;
	m_pSP_Retrieve_Dlg=(SP_Retrieve*)pDlg;
	m_SelOne=sel_one;
	m_DataBaseString=str;
	m_pDoc=pDoc;
	m_pCUG_Data=pCug_Data;

}
/*void PFCug::OnUpdate(CDB *pDB, BOOL database, UINT *pProf, UINT *pRepProf, PFCug *pCug, CDialog *pDlg, BOOL sel_one)
{
	CUGCell cell; 
	m_pDB2=pDB;
	m_DataBase=database;
	m_pRepProf=pRepProf;
	m_pProf=pProf;
	m_pCUG=pCug;
	m_pPP_EDIT_Dlg=NULL;
	m_pSP_Retrieve_Dlg=pDlg;
	m_SelOne=sel_one;


}*/
void PFCug::OnUpdateDataBase(BOOL database, CDialog *pEditDlg, CDB* pDB, PFCug *pCug)
{
	CUGCell cell; 
	m_pDB2=pDB;
	m_DataBase=database;
	m_pRepProf=0;
	m_pProf=0;
	m_pCUG=pCug;
	m_DataBaseString="Kalle";
	m_pPP_EDIT_Dlg=pEditDlg;
	if(database==FALSE) {
		m_row=((PP_EDIT*)pEditDlg)->m_NumToSelect;
	}





/*ableUpdate(FALSE); 
	while(GetNumberRows()>0)
		DeleteRow(0);
	EnableUpdate(TRUE); 
	OnSetup();*/

}

void PFCug::MakeBaseSheet(bool ExtraBase, bool NewProfile)
{
	CUGCell cell; 
	CString str;
	CString fstring;

	int cols = m_col,
		rows = m_row;
	int Count;
	size_t Record;
	Count=0;
	SetTH_NumberRows(1); 
	SetTH_Height(25); 
	SetTH_RowHeight(-1,25);


	if(m_DataBaseString=="Kalle") {
		if(m_DataBase) {
			m_pDoc=((PP_EDIT*)m_pPP_EDIT_Dlg)->m_pDoc;
			SetNumberCols ( 6 );
			m_row=((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrPlantType.GetSize();
			SetNumberRows (m_row);
			QuickSetText(0,-1,"Type");
			QuickSetText(1,-1,"Name");
			QuickSetText(2,-1,"Site Name");
			QuickSetText(3,-1,"Coordinates");
			QuickSetText(4,-1,"Country");
			QuickSetText(5,-1,"Key");
			SetColWidth(-1,0);
			SetColWidth(0,145);
			SetColWidth(1,100);
			SetColWidth(2,110);
			SetColWidth(3,85);
			SetColWidth(4,50);
			SetColWidth(5,50);
			CString key, strline;
			for(int i=0; i<m_row;i++) {
					QuickSetNumber(-1,i,i);
		
			//	if(UINT(((PP_EDIT*)m_pPP_EDIT_Dlg)->m_PlantTypes.GetSize())) {
						if(((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrPlantType[i]<UINT(((PP_EDIT*)m_pPP_EDIT_Dlg)->m_PlantTypes.GetSize()))
							strline=((PP_EDIT*)m_pPP_EDIT_Dlg)->m_PlantTypes[((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrPlantType[i]];
						else
							strline=_T("");

					CString optionlist;
					int n=((PP_EDIT*)m_pPP_EDIT_Dlg)->m_PlantTypes.GetSize();
					for(int j=0;j<n;j++) {
						optionlist+=((PP_EDIT*)m_pPP_EDIT_Dlg)->m_PlantTypes.GetAt(j)+"\n";
					}

					GetCell(0,i, &cell); 
					cell.SetCellType(UGCT_DROPLIST); 
					cell.SetLabelText(optionlist);
					//if(((PP_EDIT*)m_pPP_EDIT_Dlg)->m_ArrayIndex==i)
					//	cell.SetBackColor(m_ColorChanged);

					cell.SetText(strline);
					SetCell(0,i, &cell);

					strline=((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrPlantName[i];
					QuickSetText(1,i,strline);
			
					strline=((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrSiteName[i];
					QuickSetText(2,i,strline);

					strline=((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrLatitude[i]+"/"+((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrLongitude[i];
					QuickSetText(3,i,strline);
					strline=((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrCountry[i];
					QuickSetText(4,i,strline);
					key=((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrIdUser[i]+MFC_Util::ItoAscii(int(((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrIdNum[i]));
					GetCell(5,i,&cell);
					cell.SetParam(i);
					cell.SetText(key);
					if(i==((PP_EDIT*)m_pPP_EDIT_Dlg)->m_ArrayIndex)
						fstring=key;
					if(i<((PP_EDIT*)m_pPP_EDIT_Dlg)->m_NumOriginalRecords)
						cell.SetBackColor(m_ColorChanged);
					else
						cell.SetBackColor(m_ColorClick);
					SetCell(5,i,&cell);
					SetTopRow(((PP_EDIT*)m_pPP_EDIT_Dlg)->m_ArrayIndex);
			//	}
			}
			SortBy(0, UG_SORT_ASCENDING); 
			RedrawAll();

	
			int col=5;
			long row=0;

			FindFirst( &fstring, &col, &row, UG_FIND_PARTIAL);
			SetTopRow(row); 
		}
		else {
			CString string;
			SetNumberCols ( 1 );
			SetNumberRows (m_row);
			int isel;
			if(m_pDB2==NULL) {

				if(((PP_EDIT*)m_pPP_EDIT_Dlg)->m_SelectedKey>"")
					isel=1;
				else
					isel=0;
			}
			else
				isel=m_pDB2->MR_GetNumberOfSelections();
			
			string ="Add Record "+MFC_Util::ItoAscii(isel+1)+" of "+MFC_Util::ItoAscii(m_row)+"\nclick on database list";
			
			if(isel==m_row)
				string ="Edit Key to Record";
			QuickSetText(-1,-1,"Simulation");
			QuickSetText(0,-1,string);
			QuickSetCellTypeEx(0,-1,UGCT_NORMALMULTILINE);
			SetTH_RowHeight(-1,40);
			SetColWidth(-1,70);
			SetColWidth(0,74);


			for(int i=0;i<m_row;i++) {
				QuickSetNumber(-1,i,i+1);
				if(m_pDB2!=NULL)
					if(m_pDB2->MR_GetNumberOfSelections()>i)
						string=m_pDB2->MR_GetKeySelection(i).c_str();
					else
						string=_T("");
				else {
					string=_T("");
					CString key=((PP_EDIT*)m_pPP_EDIT_Dlg)->m_SelectedKey;
					if(key!=string)
						string=key;

					//key=((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrIdUser[i]+MFC_Util::ItoAscii(int(((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrIdNum[i]));

				}
				QuickSetText(0,i,string);
			}
			RedrawAll();



		}
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

		long toprow=0;
		int ini_prof=m_pPFCurve->m_ActProfile.KeyProf/100;//PLOTPF_mp_ACTUALPROFILE;
		int ini_rep_prof=m_pPFCurve->m_ActProfile.KeyProf%100;//PLOTPF_mp_ACTUALREPPROFILE;
		//PLOTPF_mp_SORTDATABASE();
		Record=1;
		NEWHEADER header;
		header.County.resize(2);
		header.Country.resize(2);
		vector<float> pressure;
		bool ExtraLocalBase;
		if(m_LatestSheet==0)
			ExtraLocalBase=false;
		else
			ExtraLocalBase=true;

		vector<size_t> prof_index;
		vector<size_t> sort_index;
		
		if(NewProfile) {

			m_row=1;


		}
		else {

			//m_pPFCurve->InitNewScan(ExtraLocalBase);
			for(size_t i=0;i<m_pPFCurve->GetNumProfiles();i++) {
			//while(m_pPFCurve->ScanNewHeaders(&header, ExtraLocalBase)) {
				if(KeyTest(m_pPFCurve->GetProfile(i))) {
					auto prof=m_pPFCurve->GetProfile(i);
					prof_index.push_back(i);
				}


			}
			sort_index=GetSortIndex();
			//m_pPFCurve->InitNewScan(ExtraLocalBase);
			m_row=prof_index.size();
		}
		SetNumberCols ( 14 );
		SetNumberRows (m_row);
		SetTH_NumberRows(2); 
		//******* Set the total height of the top heading 
		SetTH_Height(50); 

//******* Set the heights of the individual heading rows 
		SetTH_RowHeight(-1,25); 
		SetTH_RowHeight(-2,25); 
		QuickSetText(-1,-2,"Line \nNo");
		QuickSetText(0,-2,"Profile");
		QuickSetText(1,-2,"Profile");
		UnJoinCells(3,-2);
		UnJoinCells(3,-1);
		UnJoinCells(4,-2);
		UnJoinCells(4,-1);
		UnJoinCells(5,-2);
		UnJoinCells(5,-1);
		QuickSetCellTypeEx(-1,-2,UGCT_NORMALMULTILINE);
		//QuickSetCellTypeEx(0,-2,UGCT_NORMALMULTILINE);
		QuickSetAlignment(-1,-2,UG_ALIGNBOTTOM);
		QuickSetAlignment(0,-2,UG_ALIGNBOTTOM);
		QuickSetAlignment(1,-2,UG_ALIGNBOTTOM);
		QuickSetAlignment(2,-2,UG_ALIGNBOTTOM);
		QuickSetAlignment(3,-2,UG_ALIGNBOTTOM);
		QuickSetAlignment(4,-2,UG_ALIGNBOTTOM);
		QuickSetAlignment(5,-2,UG_ALIGNBOTTOM);
		QuickSetText(2,-2,"Name");
		QuickSetText(3,-2,"Lat");
		QuickSetText(4,-2,"Long");
		QuickSetText(5,-2,"Country");
		QuickSetText(6,-2,"TopSoil");
		QuickSetText(9,-2,"SubSoil");
		QuickSetText(9,-1,"Clay");
		QuickSetText(10,-1,"Sand");
		QuickSetText(11,-1,"Org");
		QuickSetText(6,-1,"Clay");
		QuickSetText(7,-1,"Sand");
		QuickSetText(8,-1,"Org");
		QuickSetText(12,-1,"Year");
		QuickSetText(13,-2,"Heads");
		QuickSetText(13,-1,"(n)");
		SetColWidth(-1,30);
		SetColWidth(0,25);
		SetColWidth(1,20);
		SetColWidth(2,90);
		SetColWidth(3,45);
		SetColWidth(4,45);
		SetColWidth(5,55);
		SetColWidth(6,38);
		SetColWidth(7,38);
		SetColWidth(8,38);
		SetColWidth(9,38);
		SetColWidth(10,38);
		SetColWidth(11,38);
		SetColWidth(12,38);
		SetColWidth(13,38);
		JoinCells(6,-2,8,-2);
		JoinCells(9,-2,11,-2);
		JoinCells(-1,-2,-1,-1);
		JoinCells(0,-2,0,-1);
		JoinCells(1,-2,1,-1);
		JoinCells(2,-2,2,-1);
		JoinCells(3,-2,3,-1);
		JoinCells(4,-2,4,-1);
		JoinCells(5,-2,5,-1);
		JoinCells(12,-2,12,-1);
	

	//SetColWidth(0,86);
	//return;
		Record=1;
		CString string, tmpstring;
		if(NewProfile) {
			m_NewHeader=m_pPFCurve->GetHeader(ini_prof, ini_rep_prof);
			SetDescriptionRow(0, m_NewHeader);
		}
		else {
			Count=-1;
			for(size_t ik=0; ik<sort_index.size(); ik++) {
//			for(auto it=sortname.begin();it!=sortname.end(); it++) {
			//for(auto it=sortpos.begin();it!=sortpos.end(); it++) {
				auto i=sort_index[ik];
				int layer=1;
				header=m_pPFCurve->GetProfile(i);
				pressure=m_pPFCurve->GetPF_Pressure(header.RecLayers);
				int nvalid=0;
				if(KeyTest(header)||m_CompleteList) {
					Count++;
					GetCell(0,Count,&cell);
					if(ini_prof==header.KeyProf/100&&ini_rep_prof==header.KeyProf%100) {
							
							toprow=Count;
							m_PrevClickRow=Count;
							cell.SetBackColor(m_ColorChanged);
					}
					else
						cell.SetBackColor(m_ColorClick);
					SetCell(0,Count,&cell);
					SetDescriptionRow(Count, header);
				}
				Record++;	
			}


		}
		int layer=1;
		m_pPFCurve->GetHeader(ini_prof, ini_rep_prof);
		//PLOTPF_mp_GETLAYER(&ini_prof, &ini_rep_prof, &layer);
 		*m_pRepProf=ini_rep_prof;
		*m_pProf=ini_prof;
		SetTopRow(m_pPFCurve->GetTopRow());
		RedrawAll();

	}
	else if(m_pDB2!=NULL&&m_row>0) {
		CString string;
		SetNumberCols ( 1 );
		SetNumberRows (m_row);
		int isel=m_pDB2->MR_GetNumberOfSelections();
		string ="Add Profile "+MFC_Util::ItoAscii(isel+1)+" of "+MFC_Util::ItoAscii(m_row)+"\nclick on database list";
		if(isel==m_row)
			string ="Edit Profile No xxx:yy";
		QuickSetText(-1,-1,"Simulation");
		QuickSetText(0,-1,string);
		QuickSetCellTypeEx(0,-1,UGCT_NORMALMULTILINE);
		SetTH_RowHeight(-1,40);
		SetColWidth(-1,80);
		SetColWidth(0,140);


		for(int i=0;i<m_row;i++) {
				QuickSetNumber(-1,i,i+1);
				string=m_pDB2->MR_GetKeySelection(i).c_str();
				QuickSetText(0,i,string);
		}
		RedrawAll();


	}
	else { // Only one profile to be selected
		string str;
		SetNumberCols ( 2 );
		SetNumberRows (10);
		QuickSetText(-1,-1,"");
		QuickSetText(1,-1,"Selected Profile");
	
		SetColWidth(-1,0);
		SetColWidth(0,100);
		SetColWidth(1,140);

			if(!m_pPFCurve->IsProfile()) {
				for(int i=0;i<9;i++) {
					QuickSetText(1,i,"");
					QuickSetText(0,i,"");
					QuickSetBackColor(0,0,m_ColorHeading);
				}

			}
			else {
				NEWHEADER test=m_pPFCurve->m_ActProfile;
				QuickSetNumber(-1,Count,Count);
				str=FUtil::STD_ItoAscii(m_pPFCurve->m_ActProfile.KeyProf/100);
				str+=":";
				str+=FUtil::STD_ItoAscii(m_pPFCurve->m_ActProfile.KeyProf%100);
				GetCell(1,0,&cell);
				//QuickSetText(0,Count,string);
				cell.SetText(str.c_str());
				cell.SetBackColor(m_ColorClick);
				cell.SetReadOnly(TRUE);
				SetCell(1,0,&cell);
				QuickSetText(0,0,"Profile no:");
				
	
				str=m_pPFCurve->m_ActProfile.Name;
	
				QuickSetText(1,1,str.c_str());
				QuickSetText(0,1,"Site");
			
				if(m_pPFCurve->m_ActProfile.Lat>-1.E37) {
					str=FUtil::STD_FtoAscii(m_pPFCurve->m_ActProfile.Lat);
					QuickSetText(0,2,"Latitude");
					QuickSetText(1,2,str);
					
				}
				else {
					QuickSetText(1,2,"");
					QuickSetText(1,2,"");

				}

				if(m_pPFCurve->m_ActProfile.Long>-1.E37) {
					QuickSetText(0,3,"Longitude");
					str=FUtil::STD_FtoAscii(m_pPFCurve->m_ActProfile.Long);
					QuickSetText(1,3,str);
				}
				else {
					QuickSetText(1,3,"");
					QuickSetText(1,3,"");

				}
			
				str=m_pPFCurve->m_ActProfile.County;
				str+=":";
				str+=m_pPFCurve->m_ActProfile.Country;
				QuickSetText(1,4,str);
				QuickSetText(0,4,"Country");
				GetCell(1,5,&cell);
				cell.SetNumber(m_pPFCurve->m_ActProfile.NumLayers );
				//cell.SetReadOnly(TRUE);
				SetCell(1,5,&cell);
				QuickSetText(0,5,"Layers");
				QuickSetText(0,6,"Year");
				GetCell(1,6,&cell);
				cell.SetText(MFC_Util::ItoAscii( m_pPFCurve->m_ActProfile.Year  ));
				SetCell(1,6,&cell);
				GetCell(1,7,&cell);
				//QuickSetNumber(0,5,PLOTPF_mp_YEAR);
				QuickSetText(0,7,"Heads");
				GetCell(1,7,&cell);
				vector<float> pressure=m_pPFCurve->GetPF_Pressure(m_pPFCurve->m_ActProfile.RecLayers);
				cell.SetNumber(pressure.size());
				SetCell(1,7,&cell);
				QuickSetText(0,8,"Data Base");
				if(m_LatestSheet==0)
					str=m_pPFCurve->GetMainDataBaseName(true);
				else
					str=m_pPFCurve->GetExtraDataBaseName(true);

				QuickSetText(1,8,str);
				for(size_t i=0; i<9; i++) QuickSetBackColor(0,i,m_ColorHeading);

			}
			RedrawAll();

	}
	EnableMenu(TRUE); 

	SetVScrollMode ( UG_SCROLLTRACKING );
	SetUniformRowHeight( TRUE );
}
void PFCug::MakeTextureSheet()
{
		CUGCell cell; 
		long nrow=m_pPFCurve->m_ActProfile.NumLayers;
		SetNumberCols ( 10 );
		SetNumberRows (nrow);
		SetTH_NumberRows(1); 
		SetColWidth(-1,25);
		SetTH_Height(60); 
		SetTH_RowHeight(-1,60); 
		
		QuickSetText(0,-1,"Upper");
		QuickSetText(1,-1,"Lower");
		QuickSetText(2,-1,"<0.002");
		QuickSetText(3,-1,"0.002\n-\n0.006");
		QuickSetText(4,-1,"0.006\n-\n0.02");	
		QuickSetText(5,-1,"0.02\n-\n0.06");
		QuickSetText(6,-1,"0.06\n-\n0.2");	
		QuickSetText(7,-1,"0.2\n-\n0.6");
		QuickSetText(8,-1,"0.6\n-\n2.0");	
		QuickSetText(9,-1,"loss\non\nignition");

		for(int i=0; i<10;i++) {
			QuickSetCellTypeEx(i,-1,UGCT_NORMALMULTILINE);
			QuickSetAlignment(i,-1,UG_ALIGNBOTTOM);
			SetColWidth(i,52);
		}
		for(int i=0;i<nrow;i++) {
			vector<float> tex;
			PFCOEF pfcoef;
			tex=m_pPFCurve->GetTexture(i);
			pfcoef=m_pPFCurve->GetPF_Coef(i);
			QuickSetNumber(-1,i,i+1);
			QuickSetNumber(0,i,pfcoef.UpperDepth);
			QuickSetNumber(1,i,pfcoef.LowerDepth);
			for(int j=0;j<8;j++) {
				GetCell(j+2,i,&cell); 
				//cell.SetNumberDecimals(1); 
				SetCell(j+2,i, &cell); 
				if(j==0||j==7)
					QuickSetNumber(j+2,i,tex[j]);
				else if(j<6)
					QuickSetNumber(j+2,i,tex[j]-tex[j-1]);
				else
					QuickSetNumber(j+2,i,100-tex[7]-tex[5]);

			}


		}

}
void PFCug::MakePFCurveSheet()
{
		CUGCell cell; 
		long nrow=m_pPFCurve->m_ActProfile.NumLayers;

		vector<float> pressure;
		pressure=m_pPFCurve->GetPF_Pressure(m_pPFCurve->m_ActProfile.RecLayers);
		int ncol=pressure.size()+2;
		SetNumberCols (ncol);
		SetNumberRows (nrow+1);
		SetTH_NumberRows(1); 
		SetColWidth(-1,25);
		if(pressure.size()<3) {
			SetTH_Height(55); 
			SetTH_RowHeight(-1,55); 
		}
		else if(pressure.size()<5) {
			SetTH_Height(45); 
			SetTH_RowHeight(-1,45);		
		}
		else {
			SetTH_Height(25); 
			SetTH_RowHeight(-1,25); 
		}
		

		QuickSetText(0,-1,"Depth (m)");
		if(ncol>2) QuickSetText(2,-1,"Pressure head levels (m water)");
		QuickSetText(-1,0,"");
		QuickSetText(0,0,"Upper");
		QuickSetText(1,0,"Lower");
		QuickSetBackColor(0,0,m_ColorHeading);
		QuickSetBackColor(1,0,m_ColorHeading);
		JoinCells(0,-1,1,-1);
		if(ncol>2) JoinCells(2,-1,ncol-1,-1);
		for(int i=0; i<ncol;i++) {
			QuickSetCellTypeEx(i,-1,UGCT_NORMALMULTILINE);
			QuickSetAlignment(i,-1,UG_ALIGNBOTTOM);
			if(i<2)
				SetColWidth(i,48);
			else {
				SetColWidth(i,48);
				QuickSetBackColor(i,0,m_ColorHeading);
				QuickSetNumber(i,0,pressure[i-2]);
			}
		}



		for(int i=0;i<nrow+1;i++) {
			QuickSetNumber(-1,i+1,i+1);
			GetCell(0,i,&cell); 
			//cell.SetNumberDecimals(2); 
			SetCell(0,i, &cell);
			GetCell(1,i,&cell); 
			//cell.SetNumberDecimals(2); 
			SetCell(1,i, &cell);
			PFCOEF pfcoef;
			
			
			if(i>0) {
				pfcoef=m_pPFCurve->GetPF_Coef(i-1);
				QuickSetNumber(0,i,pfcoef.UpperDepth);
				QuickSetNumber(1,i,pfcoef.LowerDepth);
				vector<float> pf_theta;
				for(int j=0;j<pressure.size();j++) {
					pf_theta=m_pPFCurve->GetPF_Theta(i-1);
					if(pf_theta[j]>0) {
						GetCell(2+j,i,&cell); 
						cell.SetNumber(pf_theta[j]);
						//cell.SetNumberDecimals(1); 
						SetCell(2+j,i, &cell);
					}
				}
			}
		}

}
void PFCug::MakeBoCSheet()
{
		CUGCell cell; 
		long nrow=m_pPFCurve->m_ActProfile.NumLayers;
		SetNumberCols ( 9 );
		SetNumberRows (nrow);
		SetTH_NumberRows(2); 
		SetColWidth(-1,25);
		SetTH_Height(70); 
		SetTH_RowHeight(-1,50); 
		SetTH_RowHeight(-2,20); 
		QuickSetText(0,-2,"Depth");
		QuickSetText(2,-2,"Coefficients");
		QuickSetText(0,-1,"Upper");
		QuickSetText(1,-1,"Lower");
		QuickSetText(2,-1,"Lambda \n(-)");
		QuickSetText(3,-1,"Air Entry\n(cm)");
		QuickSetText(4,-1,"Saturation\n(vol %)");
		QuickSetText(5,-1,"Wilting\npoint \n(vol %)");
		QuickSetText(6,-1,"Residual\nwater \n(vol %)");
		QuickSetText(7,-1,"Macro pore\n(vol %)");
		QuickSetText(8,-1,"Upper\nboundary\n(cm)");

		JoinCells(0,-2,1,-2);
		JoinCells(2,-2,8,-2);
		for(int i=0; i<9;i++) {
			QuickSetCellTypeEx(i,-1,UGCT_NORMALMULTILINE);
			QuickSetAlignment(i,-1,UG_ALIGNBOTTOM);
			if(i<2)
				SetColWidth(i,40);
			else
				SetColWidth(i,62);
		}
		PFCOEF pfcoef;
		for(int i=0;i<nrow;i++) {
			QuickSetNumber(-1,i,i+1);
			GetCell(2,i,&cell); 
			cell.SetNumberDecimals(3); 
			SetCell(2,i, &cell); 
			pfcoef=m_pPFCurve->GetPF_Coef(i);
			QuickSetNumber(0,i,pfcoef.UpperDepth);
			QuickSetNumber(1,i,pfcoef.LowerDepth);
			QuickSetNumber(2,i,pfcoef.Lambda);
			QuickSetNumber(3,i,pfcoef.AirEntry);
			GetCell(3,i,&cell); 
			//cell.SetNumberDecimals(1); 
			SetCell(3,i, &cell); 
			QuickSetNumber(4,i,pfcoef.Saturation);
			GetCell(4,i,&cell); 
			//cell.SetNumberDecimals(1); 
			SetCell(4,i, &cell); 
			QuickSetNumber(5,i,pfcoef.Wilting);
			GetCell(5,i,&cell); 
			//cell.SetNumberDecimals(1); 
			SetCell(5,i, &cell); 
			QuickSetNumber(6,i,pfcoef.Residual);
			GetCell(6,i,&cell); 
			//cell.SetNumberDecimals(1); 
			SetCell(6,i, &cell); 
			//QuickSetNumber(7,i,PLOTPF_mp_[i]);
			QuickSetNumber(7,i,pfcoef.LowerBoundary);
			QuickSetNumber(8,i,pfcoef.UpperBoundary);
		}
	


}
void PFCug::MakeGenuchtenSheet()
{
		CUGCell cell; 
		long nrow=m_pPFCurve->m_ActProfile.NumLayers;
		SetNumberCols ( 5 );
		SetNumberRows (nrow);
		SetTH_NumberRows(2); 
		SetColWidth(-1,25);
		SetTH_Height(70); 
		SetTH_RowHeight(-1,50); 
		SetTH_RowHeight(-2,20); 


		QuickSetText(0,-2,"Depth");
		QuickSetText(2,-2,"Coefficients");
		QuickSetText(0,-1,"Upper");
		QuickSetText(1,-1,"Lower");
		QuickSetText(2,-1,"m \n(-)");
		QuickSetText(3,-1,"n\n(cm)");
		QuickSetText(4,-1,"Alpha\n(1/cm)");

		JoinCells(0,-2,1,-2);
		JoinCells(2,-2,4,-2);
		for(int i=0; i<5;i++) {
			QuickSetCellTypeEx(i,-1,UGCT_NORMALMULTILINE);
			QuickSetAlignment(i,-1,UG_ALIGNBOTTOM);
			if(i<2)
				SetColWidth(i,40);
			else
				SetColWidth(i,82);
		}
		PFCOEF pfcoef;
		for(int i=0;i<nrow;i++) {
			pfcoef=m_pPFCurve->GetPF_Coef(i);
			QuickSetNumber(-1,i,i+1);
			GetCell(2,i,&cell); 
			cell.SetNumberDecimals(3); 
			SetCell(2,i, &cell); 

			QuickSetNumber(0,i,pfcoef.UpperDepth);
			QuickSetNumber(1,i,pfcoef.LowerDepth);
			QuickSetNumber(2,i,pfcoef.Gen_M);
			GetCell(2,i,&cell); 
			cell.SetNumberDecimals(3); 
			SetCell(2,i, &cell);
			QuickSetNumber(3,i,pfcoef.Gen_N);
			GetCell(3,i,&cell); 
			cell.SetNumberDecimals(3); 
			SetCell(3,i, &cell); 
			QuickSetNumber(4,i,pfcoef.Gen_Alfa);
			GetCell(4,i,&cell); 
			cell.SetNumberDecimals(3); 
			SetCell(4,i, &cell); 

		}
	


}
void PFCug::MakeHydraulicCondSheet()
{
		CUGCell cell; 
		long nrow=m_pPFCurve->m_ActProfile.NumLayers;
		SetNumberCols ( 7);
		SetNumberRows (nrow);
		SetTH_NumberRows(2); 
		SetColWidth(-1,25);
		SetTH_Height(80); 
		SetTH_RowHeight(-1,60); 
		SetTH_RowHeight(-2,20); 


		QuickSetText(0,-2,"Depth");
		QuickSetText(2,-2,"Coefficients");
		QuickSetText(0,-1,"Upper");
		QuickSetText(1,-1,"Lower");
		QuickSetText(2,-1,"Matric Conductivty\n(mm/day");
		QuickSetText(3,-1,"Total Conductivity\n(mm/day)");

		QuickSetText(4,-1,"n\nTortuosity\n(-)");
		QuickSetText(5,-1,"n Relative\nsaturation\n(-)");
		QuickSetText(6,-1,"n Effective\nsaturation\n(-)");


		JoinCells(0,-2,1,-2);
		JoinCells(2,-2,5,-2);
		for(int i=0; i<7;i++) {
			QuickSetCellTypeEx(i,-1,UGCT_NORMALMULTILINE);
			QuickSetAlignment(i,-1,UG_ALIGNBOTTOM);
			if(i<2)
				SetColWidth(i,40);
			else
				SetColWidth(i,90);
		}
		PFCOEF pfcoef;
		for(int i=0;i<nrow;i++) {
			pfcoef=m_pPFCurve->GetPF_Coef(i);
			QuickSetNumber(-1,i,i+1);
			GetCell(2,i,&cell); 
			cell.SetNumberDecimals(3); 
			SetCell(2,i, &cell); 
			QuickSetNumber(0,i,pfcoef.UpperDepth);
			QuickSetNumber(1,i,pfcoef.LowerDepth);
			QuickSetNumber(2,i,pfcoef.MatConductivity);
			QuickSetNumber(3,i,pfcoef.TotConductivity);
			QuickSetNumber(4,i,pfcoef.Mualem_n);
			GetCell(2,i,&cell); 
			/*	if(PLOTPF_mp_BSATC[i]<1)
					cell.SetNumberDecimals(3); 
				else if(PLOTPF_mp_BSATC[i]<1000)
					cell.SetNumberDecimals(1); 
				else
					//cell.SetNumberDecimals(0);*/
			SetCell(2,i, &cell);
			QuickSetNumber(5,i,pfcoef.N_SrCoef);
			GetCell(3,i,&cell); 
			/*if(PLOTPF_mp_BBSAT[i]<1)
				cell.SetNumberDecimals(3); 
			else if(PLOTPF_mp_BBSAT[i]<1000)
				cell.SetNumberDecimals(1); 
			else
				cell.SetNumberDecimals(0);

*/
			SetCell(3,i, &cell); 
			QuickSetNumber(6,i,pfcoef.N_SECoef);
			GetCell(4,i,&cell); 
			//cell.SetNumberDecimals(1); 
			SetCell(4,i, &cell); 
			GetCell(5,i,&cell); 
			//cell.SetNumberDecimals(1); 
			SetCell(5,i, &cell);
			GetCell(6,i,&cell); 
			//cell.SetNumberDecimals(1); 
			SetCell(6,i, &cell);
		}

}
void PFCug::OnSetup()
{
	CUGCell cell; 
	CString str;
	CString fstring;
	m_OneAction=false;

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
		//OnSheetSetup(m_LatestSheet);
		SetCurrentTab(m_LatestSheet);
		SetNumberSheets(5); 
	}
	else if(m_DataBase) {
		int num_sheets=GetNumberSheets();
		for (long i=0;i<num_sheets;i++) {
			DeleteTab(i);
		}
		SetDefColWidth(130);
		AddTab("Main Data Base", 0); 
		AddTab("Extra Data Base", 1); 
		AddTab("New Profile",2);

		SetTabWidth(480); 
		SetSheetNumber(m_LatestSheet); 
		OnSheetSetup(m_LatestSheet);
		SetCurrentTab(m_LatestSheet);
		SetNumberSheets(3);

	}
	else {
		MakeBaseSheet();

	}


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
	if(m_DataBaseString=="DataSheet") {
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
	else {
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
	CString str;
	StartDragDrop();

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
		str=cell.GetText();
		int prof=FUtil::AtoInt(string(str));
		
		GetCell(1,row,&cell);
		cell.SetBackColor(m_ColorChanged);
		str=cell.GetText();
		SetCell(1,row,&cell);
		RedrawCell(1,row);
		int repNumber=FUtil::AtoInt(string(str));
		if(m_DataBaseString=="Kalle"&&m_DataBase) {
			if(m_pDB2!=NULL) {
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
				str=cell.GetText();
				PP_EDIT *pEDIT;
				pEDIT=(PP_EDIT*)m_pPP_EDIT_Dlg;
  //int i=str.ReverseFind(':');
  //str=str.Mid(i+1);
				pEDIT->m_SelectedKey=str;
				
				pEDIT->m_SelectedKeyDisplay=pEDIT->m_SelectedKey;
  //while(if<m_arr_Idnum.GetSize) {
//	if(m_arr_Idnum[i]==
//
  //}
 // m_ArrayIndex=m_SelectedIndex;
				string	group= pEDIT->m_ID_DB_Group;
				for(int i=0;i<pEDIT->m_arrIdKey.GetSize();i++) {
					CString testkey;
					testkey=pEDIT->m_arrIdKey[i];
					if(testkey==pEDIT->m_SelectedKey) pEDIT->m_ArrayIndex=i;
				 }
				pEDIT->m_SelectedIndex=pEDIT->m_ArrayIndex;
				if(pEDIT->m_ArrayIndex<pEDIT->m_NumOriginalRecords||pEDIT->m_TotalReadOnly) 
					pEDIT->m_ReadOnly=TRUE;
				else {
					pEDIT->m_ReadOnly=FALSE;
				}
				//pEDIT->Retrieve();
				m_pDoc->Retrieve(group, string(pEDIT->m_SelectedKey), true);
				pEDIT->OnInitDialog();
				if(m_pDB2==NULL) {
				//	pEDIT->OnInitDialog();
				}
				else if(m_pCUG!=NULL) {
					if(m_pDB2->MR_GetNumberOfSelections()<m_pCUG->m_row) {
						m_pDB2->MR_AddSelection(m_pDB2->GetName(),string(str));
						if(m_pCUG!=NULL)
							m_pCUG->OnSetup();
					}
				}
			}

			return;
		}
		if(m_DataBaseString=="Kalle") return; // Kalle = Called from PP_EDIT
		if(m_SelOne) {
			//CString string=_T("");
			//for(int i=0;i<6;i++)
			//	string+=PLOTPF_mp_DKEY[i];
			((SP_Retrieve*)m_pSP_Retrieve_Dlg)->OnEnChangeEditComments();

			int layer=1;
			
			*m_pRepProf=repNumber;
			*m_pProf=prof;

			GetCell(1,row,&cell);
			repNumber=cell.GetNumber();
			m_pPFCurve->SetTopRow(GetTopRow());
			
			//PLOTPF_mp_GETLAYER(&prof, &repNumber, &layer);
			if(m_pCUG!=NULL&&prof>0&&repNumber>0) {
				if(m_pPFCurve->SetHeader(prof, repNumber)) {
				NEWHEADER test=m_pPFCurve->m_ActProfile;
		
				};
				m_DataImported=false;
				m_pCUG->OnSetup();
				m_pCUG_Data->OnSetup();
				((SP_Retrieve*)m_pSP_Retrieve_Dlg)->SetComments("");
			//	pEDIT->m_Details="Kalle";

			}

		}
		else if(m_pCUG!=NULL) {
			if(m_pDB2!=NULL&&m_pDB2->MR_GetNumberOfSelections()<m_pCUG->m_row) {
				m_pDB2->MR_AddSelection("Soil",string(str));
				if(m_pCUG!=NULL)
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
			pSimB=m_pDoc->GetPtr(PAR_TABLE,"Air Entry");
			pTab=(Tab*)m_pDoc->GetPtr(TABLE,"Brooks-Corey, water retention, measured horizons");
			MakeChart(row, pSimB, 0);


		break;
		case 2:
			pSimB=m_pDoc->GetPtr(PAR_TABLE,"Total Conductivity");
			pTab=(Tab*)m_pDoc->GetPtr(TABLE,"Hydraulic conductivity, measured horizons");
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
	if(m_DataBaseString!="DataSheet") {
		CString pszFileName ;
		LPCTSTR lpszDefExt="dbn|dba", lpszFilter;
		lpszFilter = "Data Base File (*.dbn)|*.dbn|Old Data Base File (*.dba)|*.dba|";
		string filename;
 		CFileDialog	NewFileDlg(TRUE,lpszDefExt,pszFileName, OFN_NOCHANGEDIR,lpszFilter);
		if(NewFileDlg.DoModal()==IDOK) {
			filename=string(NewFileDlg.GetPathName());
			size_t pos=filename.find(".dba");
			if(pos==string::npos) pos=filename.find(".DBA");
			if(pos!=string::npos)
				filename=filename.substr(0,pos);
			else {
				pos=filename.find(".dbn");
				if(pos==string::npos) pos=filename.find(".DBN");
	
				if(pos!=string::npos)
					filename=filename.substr(0,pos);
			}

			
			if(m_LatestSheet==0) {//Main DataBase 
				m_pPFCurve->CloseMainDataBase();
				m_pPFCurve->OpenMainFile(filename, m_pPFCurve->IsMainWriteEnabled());
				m_pPFCurve->ReadHeaderFile();
				m_pPFCurve->SaveNewFormat();
				m_pPFCurve->CloseMainDataBase();
				m_pPFCurve->OpenMainFile(filename, m_pPFCurve->IsMainWriteEnabled());




			}
			else
				m_pPFCurve->OpenExtraFile(filename, m_pPFCurve->IsExtraWriteEnabled());

			OnSetup();

		}
	}

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
	if(ID==1 && msg == 103) {
			if(((PP_EDIT*)m_pPP_EDIT_Dlg)->m_ReadOnly) return TRUE;
			GetCell(0,row,&cell);
			m_After=cell.GetText();
			int iDim=GetMultiTypeIndex(m_After);
			int index=((PP_EDIT*)m_pPP_EDIT_Dlg)->m_ArrayIndex;
			GetCell(-1,row,&cell);
			int koll=cell.GetNumber();
			((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrPlantType[koll]=iDim-1;
	}
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
		EditOfLayerInfo(col, row, string(str));
	}
	else if(m_pPP_EDIT_Dlg!=NULL) {
		int Index;
		CUGCell cell;
		GetCell(-1,row,&cell);
		Index=cell.GetNumber();
		int Koll=((PP_EDIT*)m_pPP_EDIT_Dlg)->m_ArrayIndex;
		if(Koll!=Index||((PP_EDIT*)m_pPP_EDIT_Dlg)->m_ReadOnly) return TRUE;
		QuickSetText(col,row,str);
		if(col==1){
			((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrPlantName.SetAt(Index, str);
		}
		else if(col==2) {
			((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrSiteName.SetAt(Index, str);
		}
		else if(col==3) {
			((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrLatitude.SetAt(Index, str);
			((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrLongitude.SetAt(Index, str);
		}
		else if(col==4) {
			((PP_EDIT*)m_pPP_EDIT_Dlg)->m_arrCountry.SetAt(Index, str); 
		}
	}
	else if(m_DataBaseString=="Profile") {
		return EditOfProfileInfo(row, string(str));
	}
	return true;
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
void PFCug::OnMenuCommand(int col,long row,int section,int item)
{
	if(section==UG_GRID) {
		if(col==5&&m_DataBase&&item>=1300) {

			CString KeyMove;
			CUGCell cell; 
			GetCell(col,row,&cell);
			KeyMove=cell.GetText();
			int rec=cell.GetParam();
			if(item==1310) {
				((PP_EDIT*)m_pPP_EDIT_Dlg)->CopyKey(rec);
			}
			else {
				if(rec<((PP_EDIT*)m_pPP_EDIT_Dlg)->m_NumOriginalRecords)
					((PP_EDIT*)m_pPP_EDIT_Dlg)->MoveSystemKey(rec,TRUE);
				else {
					if(item==1300)
						((PP_EDIT*)m_pPP_EDIT_Dlg)->MoveSystemKey(rec,FALSE);
					else {
						((PP_EDIT*)m_pPP_EDIT_Dlg)->m_ArrayIndex=rec;
						((PP_EDIT*)m_pPP_EDIT_Dlg)->OnButtonDelete();
					}

				}
			}


		}
		else if(m_DataBase&&item==5000) {
			if(m_pPFCurve->DeleteProfile(row,false)) {
				int sheet;
				sheet=GetSheetNumber();
				OnSheetSetup(sheet);
				return;
			};
		}
		if(item== 1000)
			CutSelected();

		if(item==2000)
			CopySelected();

		if(item==3000) {
			CUGCell cell;
			Paste();
			if(m_DataBaseString=="DataSheet") {
		int isheet=GetSheetNumber();
		long nrow=GetNumberRows();
		double value;
		int nvalue;
		PFCOEF pfcoef;
		pfcoef=m_pPFCurve->GetPF_Coef(row);
		vector<float> texture,theta, pressure;
		pressure=m_pPFCurve->GetPF_Pressure(m_pPFCurve->m_ActProfile.RecLayers);
		texture.resize(8);
		theta.resize(pressure.size());
		theta=m_pPFCurve->GetPF_Theta(row);
		texture=m_pPFCurve->GetTexture(row);



		if(isheet==0) {
			for(long j=0;j<nrow;j++) {
					pfcoef=m_pPFCurve->GetPF_Coef(j);
					//PLOTPF_mp_ACTUALLEVEL=j+1;
					GetCell(0,j,&cell);
					value=cell.GetNumber();
					pfcoef.UpperDepth=value;
					GetCell(1,j,&cell);	value=cell.GetNumber();
					pfcoef.LowerDepth==value;
					GetCell(2,j,&cell);	value=cell.GetNumber();					
					pfcoef.Lambda=value;
					GetCell(3,j,&cell);	value=cell.GetNumber();
					pfcoef.AirEntry=value;
					GetCell(4,j,&cell);	value=cell.GetNumber();
					pfcoef.Saturation=value;
					GetCell(5,j,&cell);	value=cell.GetNumber();
					pfcoef.Wilting=value;
					GetCell(6,j,&cell);	value=cell.GetNumber();
					pfcoef.Residual=value;
					GetCell(7,j,&cell);	value=cell.GetNumber();
					pfcoef.LowerBoundary=value;
					GetCell(8,j,&cell);	value=cell.GetNumber();
					pfcoef.UpperBoundary=value;
					m_pPFCurve->SetPF_Coef(j, pfcoef);
			}
		}
		else if(isheet==1) { //Genuchten
			for(long j=0;j<nrow;j++) {
					pfcoef=m_pPFCurve->GetPF_Coef(j);
					GetCell(0,j,&cell);
					value=cell.GetNumber();
					pfcoef.UpperDepth=value;
					GetCell(1,j,&cell);	value=cell.GetNumber();
					pfcoef.LowerDepth=value;
					GetCell(2,j,&cell);	value=cell.GetNumber();					
					pfcoef.Gen_M=value;
					GetCell(3,j,&cell);	value=cell.GetNumber();
					pfcoef.Gen_N=value;
					GetCell(4,j,&cell);	value=cell.GetNumber();
					pfcoef.Gen_Alfa=value;
					m_pPFCurve->SetPF_Coef(j, pfcoef);
			}

		}
		else if(isheet==2) {// Hydraulic
			for(long j=0;j<nrow;j++) {
					pfcoef=m_pPFCurve->GetPF_Coef(j);
					GetCell(0,j,&cell);
					value=cell.GetNumber();
					pfcoef.UpperDepth=value;
					GetCell(1,j,&cell);	value=cell.GetNumber();
					pfcoef.LowerDepth=value;
					GetCell(2,j,&cell);	value=cell.GetNumber();					
					pfcoef.MatConductivity=value;
					GetCell(3,j,&cell);	value=cell.GetNumber();
					pfcoef.TotConductivity=value;
					GetCell(4,j,&cell);	value=cell.GetNumber();
					pfcoef.Mualem_n=value;
					m_pPFCurve->SetPF_Coef(j, pfcoef);
			}
		}
		else if(isheet==3) {// Texture
				for(long j=0;j<nrow;j++) {
					pfcoef=m_pPFCurve->GetPF_Coef(j);
					GetCell(0,j,&cell);
					value=cell.GetNumber();
					pfcoef.UpperDepth=value;
					GetCell(1,j,&cell);	value=cell.GetNumber();
					pfcoef.LowerDepth=value;
					float acc=0;
					
					texture=m_pPFCurve->GetTexture(j);
					for (int jj=0;jj<7;jj++) {
						GetCell(2+jj,j,&cell);	value=cell.GetNumber();
						acc+=value;
						texture[jj]=acc;
					}
					GetCell(9,j,&cell);	value=cell.GetNumber();
					texture[7]=value;
					m_pPFCurve->SetPF_Coef(j, pfcoef);
					m_pPFCurve->SetPF_Texture(j, texture);
				}

		}
		else if(isheet==4) { // PF Points
				
				pressure=m_pPFCurve->GetPF_Pressure(m_pPFCurve->m_ActProfile.RecLayers);
				for(long j=0;j<nrow;j++) {
					pfcoef=m_pPFCurve->GetPF_Coef(j);
					theta=m_pPFCurve->GetPF_Theta(j);
					if(j>0) {
						GetCell(0,j,&cell);
						value=cell.GetNumber();
						pfcoef.UpperDepth=value;
						GetCell(1,j,&cell);	value=cell.GetNumber();
						pfcoef.LowerDepth=value;
						for(int jj=0;jj<pressure.size();jj++) {
							GetCell(jj+2,j,&cell);
							float v=cell.GetNumber();
							if(v>0)
								theta[jj]=v;
						}
					}
					else {
						if(col>1) {
							GetCell(col,j,&cell);value=cell.GetNumber();
							pressure[col-2]=value;
						}

					}
				}
				m_pPFCurve->SetPF_Pressure(pressure);
		}

		//PLOTPF_mp_SAVEDATABASE();

	}





		}

		if(item==2000||item==3000) {

			if(m_pDB2!=NULL&&m_DataBase!=TRUE) {
				CString str;
				CUGCell cell; 

				m_pDB2->MR_ResetSelection();
				for(int i=0;i<m_row;i++) {
					GetCell(0,i,&cell);
					str=cell.GetText();
					//QuickSetNumber(-1,i,i+1);
					m_pDB2->MR_AddSelection("Soil",string(str));
					//QuickSetText(0,i,string);
				}
				OnSetup();
				//RedrawAll(();



			}




		}

	}
	else if(section==UG_SIDEHEADING) {

		if(item==2100) {
			// Estimate pF Curve
			string results=EstimatePFCurveByPoints(row, false);
			SimB *pSimB;
			pSimB=m_pDoc->GetPtr(PAR_TABLE,"Air Entry");
			Tab *pTab=(Tab*)m_pDoc->GetPtr(TABLE,"Hydraulic conductivity, measured horizons");
			MakeChart(row, pSimB,pTab, 0);
			m_pSP_Retrieve_Dlg->OnAddComments(results);
		}


	}
	else if(section==UG_CORNERBUTTON) {

		if(item==2100) {
			// Estimate pF Curve
			string results;
			for(size_t i=0; i<m_pPFCurve->m_ActProfile.NumLayers;i++)
				results=EstimatePFCurveByPoints(i, false);

		}


	}

}

/////////////////////////////////////////////////////////////////////////////
//	return UG_SUCCESS to allow the menu to appear
//	return 1 to not allow the menu to appear
int PFCug::OnMenuStart(int col,long row,int section)
{
			//****** Populate the Menu 
	if(section==UG_GRID) {
			EmptyMenu();
			if(m_DataBaseString=="DataSheet") {

				AddMenuItem(2000, "Copy"); 
				AddMenuItem(3000, "Paste"); 
			}
			else if(col==5&&m_DataBase) {
				CString KeyMove;
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
				}
			}
			else if(m_DataBase) {
				CString str;
				CUGCell cell; 
				GetCell(0,row,&cell);
				str=cell.GetText();
				str+=":";
				GetCell(1,row,&cell);
				str+=cell.GetText();
				AddMenuItem(5000, "Delete Profile "+str); 
			}
			else {
				AddMenuItem(2000, "Copy"); 
				AddMenuItem(3000, "Paste"); 
			}
	}
	else if(section==UG_SIDEHEADING) {
			EmptyMenu();
			if(m_DataBaseString=="DataSheet") {
				AddMenuItem(2100, "Estimate pF curve by using retion points between ");
			}

	}
	else if(section==UG_CORNERBUTTON) {
		EmptyMenu();
		if(m_DataBaseString=="DataSheet") {
			AddMenuItem(2100, "Estimate pF curve by using retion points between for all soil layers");
		}

	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHint
int PFCug::OnHint(int col,long row,int section,CString *string)
{
	string->Format("Col:%d Row:%ld",col,row);
	return TRUE;
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
void PFCug::OnTabSelected(int ID)
{
	if(!m_DataBase&&!m_OneAction) {
		m_pCUG->OneAction(true);
		m_pCUG->OnTabSelected(ID);
		m_pCUG->OneAction(false);

		if(GetNumberSheets()>ID)
			OnSheetSetup(ID);

	}
	else if(m_DataBaseString!="DataSheet"&&m_DataBase&&!m_OneAction) {
		m_pCUG->OneAction(true);
		m_pCUG->OnTabSelected(ID);
		m_pCUG->OneAction(false);
		if(GetNumberSheets()>ID)
			OnSheetSetup(ID);


	}
	else {
		if(!m_OneAction)
			OnSheetSetup(ID);
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
	CString str;
	if(!m_DataBase&&m_pDB2!=NULL) {
		for(int i=0;i<m_row;i++) {
				GetCell(0,i,&cell);
				str=cell.GetText();
				m_pDB2->MR_SetSelection(i,"Soil",string(str));

		}
		return TRUE;
	}
	return FALSE;
}
int PFCug::GetMultiTypeIndex(CString str)
{
	int index;
	CString optionlist;
	int icounto=0;	
	optionlist=_T("");

	int n=((PP_EDIT*)m_pPP_EDIT_Dlg)->m_PlantTypes.GetSize();
	for(int j=0;j<n;j++) {
				optionlist+=((PP_EDIT*)m_pPP_EDIT_Dlg)->m_PlantTypes.GetAt(j)+"\n";
	}



	int ipos, ipos_end;
	ipos_end=optionlist.Find(str);
	
	optionlist=optionlist.Mid(0,ipos_end);
	ipos=optionlist.Find("\n");
	index=0;
	while(ipos>0) {
		optionlist=optionlist.Mid(ipos+1);
		ipos=optionlist.Find("\n");
		index++;
	}
	return index+1;

}
void PFCug::SaveTopRow()
{
	m_toprow=GetTopRow();
}
void PFCug::SetTopRowExternt()
{
	SetTopRow(m_toprow);
	RedrawAll();
}
bool PFCug::KeyTest(NEWHEADER header)
{
	vector<float> pressure;
	pressure=m_pPFCurve->GetPF_Pressure(header.RecLayers);
	size_t numpf_head=pressure.size();
	UINT prof=header.KeyProf/100;
	
	if((prof>=m_ProfMin&&prof<=m_ProfMax&&
				header.Upper_Org>=m_TopOrgMin&&header.Upper_Org<=m_TopOrgMax&&
				header.Upper_Sand>=m_TopSandMin&&header.Upper_Sand<=m_TopSandMax&&
				header.Upper_Clay>=m_TopClayMin&&header.Upper_Clay<=m_TopClayMax&&
			header.Lower_Clay>=m_SubClayMin&&header.Lower_Clay <=m_SubClayMax&&
			header.Lower_Sand>=m_SubSandMin&&header.Lower_Sand<=m_SubSandMax&&
			header.Lower_Org>=m_SubOrgMin&&header.Lower_Org<=m_SubOrgMax&&
			numpf_head>=m_Min_NumHeads&&
		//	   PLOTPF_mp_COUNTY[0]>=m_CountyMin&&PLOTPF_mp_COUNTY[0]<=m_CountyMax&&
			header.Year>=m_YearMin&&header.Year<=m_YearMax)||m_CompleteList)
			return true;
	else
			return false;
}
void PFCug::SetDescriptionRow(int Count, NEWHEADER header) 
{
		CUGCell cell; 
		CString string;
		QuickSetNumber(-1,Count,Count+1);
		GetCell(0,Count,&cell);
		cell.SetNumber(header.KeyProf/100);
		cell.SetReadOnly(TRUE);
		SetCell(0,Count,&cell);
		cell.SetNumber(header.KeyProf%100);
		SetCell(1,Count,&cell);		
		QuickSetText(2,Count,header.Name.c_str());
		if(header.Lat>-0.8E38)	QuickSetNumber(3,Count,header.Lat);
			else QuickSetText(3, Count, "");
		if(header.Long>-0.8E38) QuickSetNumber(4,Count,header.Long);
			else QuickSetText(4, Count, "");
		string=_T("");

		string+=header.County.c_str();
		string+=header.Country.c_str();
		QuickSetText(5,Count,string);
		GetCell(6,Count,&cell);	cell.SetNumber(header.Upper_Clay); SetCell(6,Count,&cell);
		GetCell(7,Count,&cell);	cell.SetNumber(header.Upper_Sand); SetCell(7,Count,&cell);
		GetCell(8,Count,&cell);	cell.SetNumber(header.Upper_Org); SetCell(8,Count,&cell);
		GetCell(9,Count,&cell);	cell.SetNumber(header.Lower_Clay); SetCell(9,Count,&cell);
		GetCell(10,Count,&cell);	cell.SetNumber(header.Lower_Sand); SetCell(10,Count,&cell);
		GetCell(11,Count,&cell);	cell.SetNumber(header.Lower_Org); SetCell(11,Count,&cell);

		QuickSetNumber(12,Count,header.Year);
		QuickSetNumber(13,Count,m_pPFCurve->GetNumPF_Pressures());

}
void PFCug::MakeChart(size_t index, SimB *pSimB, Tab* pTab, size_t FuncNo) {
	ChartCug *pChartCug;
	pChartCug=MySingle::instance().GetChartCugPointer();
	if(!m_DataImported) {
		m_pDoc->GetProfileFromPlotPF(m_pPFCurve->m_ActProfile, false, false);
		m_DataImported=true;
	}
	pChartCug->Init(m_pDoc);
	pChartCug->ParameterPlotMode(TRUE);
	pTab=dynamic_cast<Tab*>(m_pDoc->GetPtr(TABLE,"Brooks-Corey, water retention, measured horizons"));
	if(pTab!=NULL) {

		pChartCug->m_ParameterPlot.InitData(m_pDoc, pSimB, pTab, index, FuncNo);
		pChartCug->m_ParameterPlot.OnInit();
		pChartCug->OnSetup();
	}
}
string PFCug::EstimatePFCurveByPoints(size_t Layer, bool All) {
	string str="";
	vector<double> Theta, Psi;
	vector<float> pressure, theta;
	size_t NumEstimated=0;
	pressure=m_pPFCurve->GetPF_Pressure(m_pPFCurve->m_ActProfile.RecLayers);
	PFCOEF pfcoef;
	pfcoef=m_pPFCurve->GetPF_Coef(Layer);
	theta=m_pPFCurve->GetPF_Theta(Layer);
	for(int i=0;i<pressure.size();i++) {
		if(pressure[i]>=m_PF_EstimateMin&&
			pressure[i]<=m_PF_EstimateMax&&
			theta[i]>0.001) {
				Theta.push_back(theta[i]);
				Psi.push_back(pressure[i]);
				if(Psi[NumEstimated]>14900.&&Psi[NumEstimated]<15100)
					pfcoef.Wilting=Theta[NumEstimated];
				NumEstimated++;
			}
	}
	if(NumEstimated>1) {
		BC_ESTIMATE bc=m_pPFCurve->Estimate_BC(&Theta, &Psi,pfcoef.Saturation, pfcoef.Residual);
		//int i=HYDRAULIC_mp_ESTIMATEBC(&Theta, &Psi, &m_NumEstimated, &m_Porosity, &m_Residual);
		pfcoef.Lambda=bc.Lambda;//HYDRAULIC_mp_LAMBDA_EST;
		pfcoef.AirEntry=bc.AirEntry;//HYDRAULIC_mp_AIRENTRY_EST;
		if(bc.Residual>0&&NumEstimated>2)
			pfcoef.Residual=bc.Residual;//HYDRAULIC_mp_RESIDUAL_EST;
		str="New coefficients estimated Using ";
		str+=FUtil::STD_ItoAscii(NumEstimated);
		str+=" points, layer ";
		str+=FUtil::STD_ItoAscii(Layer+1);
		str+=" r2=";
		str+=FUtil::STD_FtoAscii(bc.R2);
		m_pPFCurve->SetPF_Coef(Layer, pfcoef);
		m_pDoc->m_Simulator.m_SoilModel.m_SoilFrost.ReCalc();

	}
	else {
		str="No Estimation made";

	}
	return str;
}
string PFCug::EstimatePFCurveByTexture(size_t layer, bool All) {
	string str="";
	return str;
}

bool PFCug::EditOfProfileInfo(int row, string str) {

	size_t nc;
	size_t nkol;
	string newstring;
	bool ChangePositionLevelFile;
	
	ChangePositionLevelFile=false;
	switch(row) {

		case 1:
			m_pPFCurve->m_ActProfile.Name=str;
		/*	nc=str.GetLength();
			nc=min(16,nc);
			for(int i=0;i<nc;i++)
				PLOTPF_mp_RPLACE[i]=str.GetAt(i);
			for(int i=nc;i<16;i++)
				PLOTPF_mp_RPLACE[i]=' ';
			PLOTPF_mp_SAVEDATABASE();*/
			break;
		case 2:
			m_pPFCurve->m_ActProfile.Lat=FUtil::AtoFloat(str);

			break;
		case 3:
			m_pPFCurve->m_ActProfile.Long=FUtil::AtoFloat(str);

			break;
		case 4:
			nc=str.size();
			//nc=min(6,nc);
			newstring=string(str);
			nkol=newstring.find(":");
			if(nkol==string::npos)
				m_pPFCurve->m_ActProfile.Country=newstring;
			else {
				m_pPFCurve->m_ActProfile.Country=newstring.substr(0,nkol);
				m_pPFCurve->m_ActProfile.County=newstring.substr(nkol+1);
			}
			break;
		case 5:
			m_pPFCurve->ExpandToNewLayerActHeader(FUtil::AtoInt(str));
			m_pPFCurve->m_ActProfile.NumLayers=FUtil::AtoInt(str);
			ChangePositionLevelFile=true;
			//m_pCUG_Data->OnSetup();
// Make change of database struture !!
			break;
		case 6:
			m_pPFCurve->m_ActProfile.Year=FUtil::AtoInt(str);
			ChangePositionLevelFile=true;
			//m_pCUG_Data->OnSetup();
			break;
		case 7:
			m_pPFCurve->m_ActProfile.RecLayers=FUtil::AtoInt(str);
		//	PLOTPF_mp_NT=
		//	PLOTPF_mp_SAVEDATABASE();
		//	m_pSP_Retrieve_Dlg->OnInitDialog();
			break;
		}
		if(!m_pPFCurve->UpdateActualProfile()) {
			m_pPFCurve->UpdateHeaderFile();

		};
		m_pPFCurve->OpenEditMainDataBase();
		if(!m_pPFCurve->SaveNewFormat()) return false;
		m_pCUG->MakeBaseSheet();
		if(ChangePositionLevelFile&&m_pCUG_Data!=NULL) {

			m_pCUG_Data->OnSetup();
			
		}
			//m_pCUG->OnSetup();
		return true;


}
bool PFCug::EditOfLayerInfo(int col, int row, string str) {
	CUGCell cell;
	int isheet=GetSheetNumber();
		int prof1=m_pPFCurve->m_ActProfile.KeyProf/100;
		int prof2=m_pPFCurve->m_ActProfile.KeyProf%100;
		PFCOEF pfcoef;
		pfcoef=m_pPFCurve->GetPF_Coef(row);
		vector<float> texture,theta, pressure;
		pressure=m_pPFCurve->GetPF_Pressure(m_pPFCurve->m_ActProfile.RecLayers);
		texture.resize(8);
		theta.resize(pressure.size());
		theta=m_pPFCurve->GetPF_Theta(row);
		texture=m_pPFCurve->GetTexture(row);

		if(isheet==0) {
			switch (col) {
				case 0:
					pfcoef.UpperDepth=FUtil::AtoFloat(str);
					break;
				case 1:
					pfcoef.LowerDepth=FUtil::AtoFloat(str);
					break;
				case 2:
					pfcoef.Lambda=FUtil::AtoFloat(str);
					break;
				case 3:
					pfcoef.AirEntry=FUtil::AtoFloat(str);
					break;
				case 4:
					pfcoef.Saturation=FUtil::AtoFloat(str);
				break;
				case 5:
					pfcoef.Wilting=FUtil::AtoFloat(str);
					break;
				case 6:
					pfcoef.Residual=FUtil::AtoFloat(str);
					break;
				case 7:
					pfcoef.LowerBoundary=FUtil::AtoFloat(str);
					break;
				case 8:
					pfcoef.UpperBoundary=FUtil::AtoFloat(str);
					break;
			}
		}
		else if(isheet==1) { //Genuchten
			switch (col) {
				case 0:
					pfcoef.UpperDepth=FUtil::AtoFloat(str);
					break;
				case 1:
					pfcoef.LowerDepth=FUtil::AtoFloat(str);
					break;
				case 2:
					pfcoef.Gen_M=FUtil::AtoFloat(str);
					break;
				case 3:
					pfcoef.Gen_N=FUtil::AtoFloat(str);
					break;
				case 4:
					pfcoef.Gen_Alfa=FUtil::AtoFloat(str);
					break;
			}

		}
		else if(isheet==2) {// Hydraulic
			switch (col) {
				case 0:
					pfcoef.UpperDepth=FUtil::AtoFloat(str);
					break;
				case 1:
					pfcoef.LowerDepth=FUtil::AtoFloat(str);
					break;
				case 2:
					pfcoef.MatConductivity=FUtil::AtoFloat(str);
					break;
				case 3:
					pfcoef.TotConductivity=FUtil::AtoFloat(str);
					break;
				case 4:
					pfcoef.Mualem_n=FUtil::AtoFloat(str);
					break;
			}
		}
		else if(isheet==3) {// Texture
			switch (col) {
				case 0:
					pfcoef.UpperDepth=FUtil::AtoFloat(str);
					break;
				case 1:
					pfcoef.LowerDepth=FUtil::AtoFloat(str);
					break;

				case 2:
				case 9:
					texture[col-2]=FUtil::AtoFloat(str);
					break;
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
					float v;
					v=0;
					for(int j=0;j<col-2;j++) {
						GetCell(j+2,row,&cell);
						v+=cell.GetNumber();
					}
					texture[col-2]=v+FUtil::AtoFloat(str);
					if(col==7||col==5) {
						GetCell(2+7,row,&cell);
						float v7=cell.GetNumber();
						GetCell(2+5,row,&cell);
						float v5=cell.GetNumber();
						texture[6]=100-v7-v5;
					}
					break;
			}


		}
		else if(isheet==4) { // PF Points
			if(row==0) {
				pressure[col-2]=FUtil::AtoFloat(str);
			}
			else {
				if(col==0)
					pfcoef.UpperDepth=FUtil::AtoFloat(str);
				else if(col==1)
					pfcoef.LowerDepth=FUtil::AtoFloat(str);
				else if(col>1) {
					GetCell(col,row,&cell);
					float v=FUtil::AtoFloat(str);
					if(v>0)
						theta[col-2]=v;
					else
						theta[col-2]=0.;
				}
			}
			m_pPFCurve->SetPF_Theta(row,theta);
			m_pPFCurve->SetPF_Pressure(pressure);

		}
		else if(isheet==5) { // Comments
			m_pPFCurve->SetComments(string(str));		
		}

		return m_pPFCurve->SetPF_Coef(row, pfcoef);
		//PLOTPF_mp_SAVEDATABASE();


	//return true;
}
vector<size_t> PFCug::GetSortIndex() {
	vector<size_t> ut;
		multimap <string, size_t> sortname;
		multimap <float, size_t> sortpos;
		multimap <size_t, size_t> sortnum;
			for(size_t i=0;i<m_pPFCurve->GetNumProfiles();i++) {
			//while(m_pPFCurve->ScanNewHeaders(&header, ExtraLocalBase)) {
				if(KeyTest(m_pPFCurve->GetProfile(i))) {
					auto prof=m_pPFCurve->GetProfile(i);
					switch (m_SortOption) {
						case 0: case 1:
							sortnum.insert(pair<size_t,size_t>(prof.KeyProf,i));
							break;
						case 2:
							sortname.insert(pair<string,size_t>(prof.Name,i));
							break;
						case 3:
							sortpos.insert(pair<float, size_t>(prof.Lat,i));
							break;
						case 4:
							sortpos.insert(pair<float, size_t>(prof.Long,i));
							break;
						case 5:
							sortname.insert(pair<string, size_t>(prof.Country,i));
							break;
						case 6:
							sortpos.insert(pair<float, size_t>(prof.Upper_Clay,i));
							break;
						case 7:
							sortpos.insert(pair<float, size_t>(prof.Upper_Sand,i));
							break;
						case 8:
							sortpos.insert(pair<float, size_t>(prof.Upper_Org,i));
							break;
						case 9:
							sortpos.insert(pair<float, size_t>(prof.Lower_Clay,i));
							break;
						case 10:
							sortpos.insert(pair<float, size_t>(prof.Lower_Sand,i));
							break;
						case 11:
							sortpos.insert(pair<float, size_t>(prof.Lower_Org,i));
							break;
						case 12:
							sortnum.insert(pair<size_t, size_t>(prof.Year,i));
							break;

					}
				}
			}

			switch(m_SortOption) {
			case 0: case 1: case 12:
				for(auto it=sortnum.begin();it!=sortnum.end(); it++)
					ut.push_back(it->second);
				break;
			case 2: case 5:
				for(auto it=sortname.begin();it!=sortname.end(); it++)
					ut.push_back(it->second);
				break;
			case 3: case 4: case 6: case 7: case 8: case 9: case 10: case 11:
				for(auto it=sortpos.begin();it!=sortpos.end(); it++)
					ut.push_back(it->second);
				break;
			}
			return ut;
}
