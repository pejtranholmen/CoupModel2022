#pragma once
#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include "..\..\Util.h"
#include ".\PFSheets.h"
#include "..\..\NewBase\Doc.h"


PFSheets::PFSheets()
{
	//UGXPThemes::CleanUp();
	m_pDoc=nullptr;
	m_pDB = NULL;
}


PFSheets::~PFSheets()
{
	//UGXPThemes::CleanUp();
}
void PFSheets::MakeBaseSheet(bool ExtraBase, bool NewProfile)
{
	CUGCell cell; 
	CString str;
	CString fstring;

	m_pCUG=MySingle::instance().GetPFCugPointer();
	int cols = m_col,
		rows = m_row;
	int Count;
	size_t Record;
	Count=0;
	SetTH_NumberRows(1); 
	SetTH_Height(25); 
	SetTH_RowHeight(-1,25);

	if(!ExtraBase||NewProfile) {
		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE);

		long toprow=0;
		p_PFCurve = &PlotpF::GetPointer();//&MySingle::GetPFInstance();// pftest::pPF;
		int ini_prof=p_PFCurve->m_ActProfile.KeyProf/100;//PLOTPF_mp_ACTUALPROFILE;
		int ini_rep_prof=p_PFCurve->m_ActProfile.KeyProf%100;//PLOTPF_mp_ACTUALREPPROFILE;
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
			for(size_t i=0;i<p_PFCurve->GetNumProfiles();i++) {
				auto prof = p_PFCurve->GetProfile(i);
					prof_index.push_back(i);
		
			}
			sort_index=GetSortIndex();
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
		QuickSetText(12,-2,"Year");
		QuickSetText(13,-2,"Heads");
		QuickSetText(13,-1,"(n)");
		SetColWidth(-1,30);
		SetColWidth(0,50);
		SetColWidth(1,25);
		SetColWidth(2,120);
		SetColWidth(3,45);
		SetColWidth(4,45);
		SetColWidth(5,55);
		SetColWidth(6,38);
		SetColWidth(7,38);
		SetColWidth(8,38);
		SetColWidth(9,38);
		SetColWidth(10,38);
		SetColWidth(11,38);
		SetColWidth(12,42);
		SetColWidth(13,42);
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
			m_NewHeader=p_PFCurve->GetHeader(ini_prof, ini_rep_prof);
			SetDescriptionRow(0, m_NewHeader);
		}
		else {
			Count=-1;
			for(size_t ik=0; ik<sort_index.size(); ik++) {
//			for(auto it=sortname.begin();it!=sortname.end(); it++) {
			//for(auto it=sortpos.begin();it!=sortpos.end(); it++) {
				auto i=sort_index[ik];
				int layer=1;
				header=p_PFCurve->GetProfile(i);
				pressure=p_PFCurve->GetPF_Pressure(header.RecLayers);
				int nvalid=0;
				//if(KeyTest(header)||m_CompleteList) {
					Count++;
					GetCell(0,Count,&cell);
					if(header.KeyProf==p_PFCurve->m_ActProfile.KeyProf) {
							
							toprow=Count;
							m_PrevClickRow=Count;
							cell.SetBackColor(m_ColorChanged);
					}
					else
						cell.SetBackColor(m_ColorClick);
					SetCell(0,Count,&cell);
					SetDescriptionRow(Count, header);
				//}
				Record++;	
			}


		}
		int layer=1;
		p_PFCurve->GetHeader(ini_prof, ini_rep_prof);
		//PLOTPF_mp_GETLAYER(&ini_prof, &ini_rep_prof, &layer);
 		//*m_pRepProf=ini_rep_prof;
		//*m_pProf=ini_prof;
		SetTopRow(p_PFCurve->GetTopRow());
		RedrawAll();

	}

	else if(ExtraBase) { // Only one profile to be selected
	//	MakeSelectedProfileSheet();
	}
	EnableMenu(TRUE); 

	SetVScrollMode ( UG_SCROLLTRACKING );
	SetUniformRowHeight( TRUE );
}
void PFSheets::MakeProfileSheet() {





}
void PFSheets::MakeTextureSheet()
{
		CUGCell cell; 
		long nrow=p_PFCurve->m_ActProfile.NumLayers;
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
			tex=p_PFCurve->GetTexture(i);
			pfcoef=p_PFCurve->GetPF_Coef(i);
			QuickSetNumber(-1,i,i+1);
			QuickSetNumber(0,i,pfcoef.UpperDepth);
			QuickSetNumber(1,i,pfcoef.LowerDepth);
			for(int j=0;j<8;j++) {
				GetCell(j+2,i,&cell); 
				//cell.SetNumberDecimals(1); 
				SetCell(j+2,i, &cell); 
				if(j==0||j==7)
					QuickSetNumber(j+2,i,tex[j], false);
				else if(j<6)
					QuickSetNumber(j+2,i,tex[j]-tex[j-1], false);
				else
					QuickSetNumber(j+2,i,100-tex[7]-tex[5], false);

			}


		}

}
void PFSheets::MakeSimpleTextureSheet()
{
		CUGCell cell; 
		long nrow=p_PFCurve->m_ActProfile.NumLayers+2;
		SetNumberCols ( 6 );
		SetNumberRows (nrow);
		SetTH_NumberRows(1); 
		SetColWidth(-1,25);
		SetTH_Height(60); 
		SetTH_RowHeight(-1,60); 
		
		QuickSetText(0,-1,"Upper");
		QuickSetText(1,-1,"Lower");
		QuickSetText(2,-1,"Clay");
		QuickSetText(3,-1,"Sand");
		QuickSetText(4,-1,"Organic");	
		QuickSetText(5,-1,"Porosity");	
		RedrawAll();



		for(int i=0; i<6;i++) {
			QuickSetCellTypeEx(i,-1,UGCT_NORMALMULTILINE);
			QuickSetAlignment(i,-1,UG_ALIGNBOTTOM);
			SetColWidth(i,62);
		}
		for(int i=0;i<nrow-2;i++) {
			vector<float> tex;
			PFCOEF pfcoef;
			tex=p_PFCurve->GetTexture(i);
			pfcoef=p_PFCurve->GetPF_Coef(i);
			QuickSetNumber(-1,i,i+1);
			QuickSetNumber(0,i,pfcoef.UpperDepth,false);
			QuickSetNumber(1,i,pfcoef.LowerDepth, false);
			double min_frac=(100.-tex[7])/100.;
			for(int j=0;j<4;j++) {
				GetCell(j+2,i,&cell); 
				//cell.SetNumberDecimals(1); 
				//SetCell(j+2,i, &cell); 
				if(j==0) {
					if(min_frac<=0) 
						cell.SetText("");
					else
						cell.SetNumber(tex[j]/min_frac);
				}
				else if(j==1) {
					if(min_frac<=0) 
						cell.SetText("");
					else
						cell.SetNumber((100.-tex[3]-tex[7])/min_frac);
				}
				else if(j==2) {
					if(min_frac<=0) 
						cell.SetNumber(100.);
					else
						cell.SetNumber(tex[7]/min_frac);
				}
				else 
					cell.SetNumber(pfcoef.Saturation);
				cell.SetReadOnly(false);
				SetCell(j+2,i, &cell); 

			}


		}
		JoinCells(0,nrow-2,1, nrow-2);
		JoinCells(5,nrow-2,5, nrow-1);
		QuickSetText(0,nrow-2,"Top Soil");
		JoinCells(0,nrow-1,1, nrow-1);
		QuickSetText(0,nrow-1,"Sub Soil");
		QuickSetNumber(2,nrow-2,p_PFCurve->m_ActProfile.Upper_Clay);
		QuickSetNumber(3,nrow-2,p_PFCurve->m_ActProfile.Upper_Sand);
		QuickSetNumber(4,nrow-2,p_PFCurve->m_ActProfile.Upper_Org);
		QuickSetNumber(2,nrow-1,p_PFCurve->m_ActProfile.Lower_Clay);
		QuickSetNumber(3,nrow-1,p_PFCurve->m_ActProfile.Lower_Sand);
		QuickSetNumber(4,nrow-1,p_PFCurve->m_ActProfile.Lower_Org);
}
void PFSheets::MakePFCurveSheet()
{
		CUGCell cell; 
		long nrow=p_PFCurve->m_ActProfile.NumLayers;

		vector<float> pressure;
		pressure=p_PFCurve->GetPF_Pressure(p_PFCurve->m_ActProfile.RecLayers);
		int ncol=pressure.size()+2;
		SetNumberCols (ncol);
		SetNumberRows (nrow);
		SetTH_NumberRows(2); 
		SetColWidth(-1,25);
/*		if(pressure.size()<3) {
			SetTH_Height(55); 
			SetTH_RowHeight(-1,55); 
		}
		else if(pressure.size()<5) {
			SetTH_Height(55); 
			SetTH_RowHeight(-2,35);		
		}
		else {*/
		SetTH_Height(55); 
		SetTH_RowHeight(-2,30); 
		SetTH_RowHeight(-1,25); 
		//}
		

		QuickSetText(0,-2,"Depth (m)");
		if(ncol>2) QuickSetText(2,-2,"Pressure head levels (cm water)");
		QuickSetText(0,-1,"Upper");
		QuickSetText(1,-1,"Lower");

		JoinCells(0,-2,1,-1);
		if(ncol>2) JoinCells(2,-2,ncol-1,-2);
		for(int i=0; i<ncol;i++) {
			QuickSetCellTypeEx(i,-1,UGCT_NORMALMULTILINE);
			QuickSetAlignment(i,-1,UG_ALIGNBOTTOM);
			if(i<2)
				SetColWidth(i,48);
			else {
				SetColWidth(i,48);
				QuickSetNumber(i,-1,pressure[i-2]);
			}
		}

		for(int i=0;i<nrow;i++) {
			QuickSetNumber(-1,i,i+1);
			PFCOEF pfcoef;
			pfcoef=p_PFCurve->GetPF_Coef(i);
			QuickSetNumber(0,i,pfcoef.UpperDepth);
			QuickSetNumber(1,i,pfcoef.LowerDepth);
			vector<float> pf_theta;
			pf_theta=p_PFCurve->GetPF_Theta(i);
			for(size_t j=0; j<pf_theta.size(); j++) {
				if(pf_theta[j]>0) {
						GetCell(2+j,i,&cell); 
						cell.SetNumber(pf_theta[j]);
						//cell.SetNumberDecimals(1); 
						SetCell(2+j,i, &cell);
				}
			}
		}
}
void PFSheets::MakePF_PressureSheet()
{
		CUGCell cell; 
		vector<float> pressure;
		pressure=p_PFCurve->GetPF_Pressure(p_PFCurve->m_ActProfile.RecLayers);
		int ncol=1;
		SetNumberCols (ncol);
		SetNumberRows (pressure.size());
		SetColWidth(0,155);
		SetColWidth(-1,25);

		QuickSetText(0,-1,"Pressure (cm water)");

		for(size_t i=0;i<pressure.size();i++) {
			QuickSetNumber(-1,i,i+1);
			GetCell(0,i,&cell); 
			cell.SetNumber(pressure[i]);
			SetCell(0,i, &cell);
		}

}
void PFSheets::MakeBoCSheet()
{
		CUGCell cell; 
		long nrow=p_PFCurve->m_ActProfile.NumLayers;
		SetNumberCols ( 9 );
		SetNumberRows (nrow);
		SetTH_NumberRows(2); 
		SetColWidth(-1,25);
		SetTH_Height(70); 
		SetTH_RowHeight(-1,50); 
		SetTH_RowHeight(-2,20); 
		QuickSetText(0,-2,"Depth (m)");
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
				SetColWidth(i,50);
			else
				SetColWidth(i,62);
		}
		PFCOEF pfcoef;
		for(int i=0;i<nrow;i++) {
			QuickSetNumber(-1,i,i+1);
			GetCell(2,i,&cell); 
			cell.SetNumberDecimals(3); 
			SetCell(2,i, &cell); 
			pfcoef=p_PFCurve->GetPF_Coef(i);
			QuickSetNumber(0,i,pfcoef.UpperDepth, false);
			QuickSetNumber(1,i,pfcoef.LowerDepth, false);
			QuickSetNumber(2,i,pfcoef.Lambda, false);
			QuickSetNumber(3,i,pfcoef.AirEntry, false);
			GetCell(3,i,&cell); 
			//cell.SetNumberDecimals(1); 
			SetCell(3,i, &cell); 
			QuickSetNumber(4,i,pfcoef.Saturation, false);
			GetCell(4,i,&cell); 
			//cell.SetNumberDecimals(1); 
			SetCell(4,i, &cell); 
			QuickSetNumber(5,i,pfcoef.Wilting, false);
			GetCell(5,i,&cell); 
			//cell.SetNumberDecimals(1); 
			SetCell(5,i, &cell); 
			QuickSetNumber(6,i,pfcoef.Residual, false);
			GetCell(6,i,&cell); 
			//cell.SetNumberDecimals(1); 
			SetCell(6,i, &cell); 
			//QuickSetNumber(7,i,PLOTPF_mp_[i]);
			QuickSetNumber(7,i,pfcoef.LowerBoundary, false);
			QuickSetNumber(8,i,pfcoef.UpperBoundary, false);
		}
	


}
void PFSheets::MakeGenuchtenSheet()
{
		CUGCell cell; 
		long nrow=p_PFCurve->m_ActProfile.NumLayers;
		SetNumberCols ( 5 );
		SetNumberRows (nrow);
		SetTH_NumberRows(2); 
		SetColWidth(-1,25);
		SetTH_Height(70); 
		SetTH_RowHeight(-1,50); 
		SetTH_RowHeight(-2,20); 


		QuickSetText(0,-2,"Depth (m)");
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
				SetColWidth(i,50);
			else
				SetColWidth(i,82);
		}
		PFCOEF pfcoef;
		for(int i=0;i<nrow;i++) {
			pfcoef=p_PFCurve->GetPF_Coef(i);
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
void PFSheets::MakeHydraulicCondSheet()
{
		CUGCell cell; 
		long nrow=p_PFCurve->m_ActProfile.NumLayers;
		SetNumberCols ( 7);
		SetNumberRows (nrow);
		SetTH_NumberRows(2); 
		SetColWidth(-1,25);
		SetTH_Height(80); 
		SetTH_RowHeight(-1,60); 
		SetTH_RowHeight(-2,20); 


		QuickSetText(0,-2,"Depth (m)");
		QuickSetText(2,-2,"Coefficients");
		QuickSetText(0,-1,"Upper");
		QuickSetText(1,-1,"Lower");
		QuickSetText(2,-1,"Matric Conductivty\n(mm/day");
		QuickSetText(3,-1,"Total Conductivity\n(mm/day)");

		QuickSetText(4,-1,"n\nTortuosity\n(-)");
		QuickSetText(5,-1,"n Relative\nsaturation\n(-)");
		QuickSetText(6,-1,"n Effective\nsaturation\n(-)");


		JoinCells(0,-2,1,-2);
		JoinCells(2,-2,6,-2);
		for(int i=0; i<7;i++) {
			QuickSetCellTypeEx(i,-1,UGCT_NORMALMULTILINE);
			QuickSetAlignment(i,-1,UG_ALIGNBOTTOM);
			if(i<2)
				SetColWidth(i,50);
			else
				SetColWidth(i,90);
		}
		PFCOEF pfcoef;
		for(int i=0;i<nrow;i++) {
			pfcoef=p_PFCurve->GetPF_Coef(i);
			QuickSetNumber(-1,i,i+1);
			GetCell(2,i,&cell); 
			cell.SetNumberDecimals(3); 
			SetCell(2,i, &cell); 
			QuickSetNumber(0,i,pfcoef.UpperDepth, false);
			QuickSetNumber(1,i,pfcoef.LowerDepth, false);
			QuickSetNumber(2,i,pfcoef.MatConductivity, false);
			QuickSetNumber(3,i,pfcoef.TotConductivity, false);
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
void PFSheets::MakeLinkToMultiRunTableSheet()
{
		CUGCell cell; 
		long nrow;


	if(m_pDoc==nullptr) return;
	SetMultiSelectMode ( TRUE );
	string str=m_pDoc->DB_GetNames(1);
	CDB *pDB;
	pDB=m_pDoc->DB_GetPointer(DB_types(1));
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
		QuickSetText(0, i,pDB->MR_GetKeySelection(i));

	}




}
void PFSheets::MakeEstimationSheet()
{
		CUGCell cell; 
		long nrow;
		if(m_EstimationResults.size()>0)
			nrow=m_EstimationResults.size()+2;
		else
			nrow=1;

		SetNumberCols ( 8);
		SetNumberRows (nrow);
		SetTH_NumberRows(1); 
		SetColWidth(-1,55);
		SetTH_Height(40); 


		for(size_t i=0; i<8;i++){
			SetColWidth(i,110);
			QuickSetCellTypeEx(i,-1,UGCT_NORMALMULTILINE);
		}
	
		JoinCells(0,-2,1,-2);
		JoinCells(2,-2,6,-2);
		JoinCells(0,0,4,0);
			QuickSetText(-1,-1,"Prof");
			QuickSetText(0,-1,"Layer");
			QuickSetText(1,-1,"R2");
			QuickSetText(2,-1,"n");
			QuickSetText(3,-1,"Lambda (-)");
			QuickSetText(4,-1,"Air Entry (cm)");
			QuickSetText(5,-1,"Residual (vol%)");
			QuickSetText(6,-1,"Min Pressure (cm)");
			QuickSetText(7,-1,"Max Pressure (cm)");

		QuickSetText(0,0,"Values to be used when making least square fitting");
		QuickSetBackColor(0,0, m_ColorHeading);
		PFCOEF pfcoef;
		QuickSetNumber(6,0,m_PF_EstimateMin);
		QuickSetNumber(7,0,m_PF_EstimateMax);
		
		if(m_PF_ResidualFix>0) 
			QuickSetNumber(5,0,m_PF_ResidualFix);
		else 
			QuickSetText(5,0,"");
		
		if(m_EstimationResults.size()>0) {
			JoinCells(-1,-1,-1,1);


			QuickSetText(0,1,"Results of Least Square Fitting (measured points) or use of Pedofunction (texture)");
			JoinCells(0,1,7,1);
			QuickSetBackColor(0,1, m_ColorHeading);
			GetCell(0,1,&cell);
			cell.SetReadOnly(TRUE);
			SetCell(0,1,&cell);
			BC_ESTIMATE bc;
			BC_RESULT res;
			for(int i=0;i<nrow-2;i++) {
					res=m_EstimationResults[i];
					bc=res.bc;
					string str=FUtil::STD_ItoAscii(res.Prof/100);
					str+=":";
					str+=FUtil::STD_ItoAscii(res.Prof%100);
					QuickSetText(-1,i+2, str);
					QuickSetNumber(0,i+2,bc.Layer+1);
					if(bc.R2<0) {
						QuickSetText(1,i+2,"Texture based");
						JoinCells(1,i+2,2,i+2);
					}
					else {
						QuickSetNumber(1,i+2,bc.R2);
						QuickSetNumber(2,i+2,bc.n);
					}
					QuickSetNumber(3,i+2,bc.Lambda);
					QuickSetNumber(4,i+2,bc.AirEntry);
					if(bc.Residual>0) 
						QuickSetNumber(5,i+2,bc.Residual);
					else
						QuickSetText(5,i+2,"");
					if(res.EstimateMin>0) {
						QuickSetNumber(6,i+2,res.EstimateMin);
						QuickSetNumber(7,i+2,res.EstimateMax);
					}
					else {
						QuickSetText(6,i+2,"");
						JoinCells(7,i+2,7,i+2);

					}

			}
		}

}
void PFSheets::Make_LinkToChangeDuringRunSheet() {
	CUGCell cell;

	if(m_pDoc==nullptr) return;


	string str=m_pDoc->DB_GetNames(1);
	CDB *pDB;
	pDB=m_pDoc->DB_GetPointer(DB_types(1));

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
vector<size_t> PFSheets::GetSortIndex() {
	vector<size_t> ut;
		multimap <string, size_t> sortname;
		multimap <float, size_t> sortpos;
		multimap <size_t, size_t> sortnum;
			for(size_t i=0;i<p_PFCurve->GetNumProfiles();i++) {
			//while(p_PFCurve->ScanNewHeaders(&header, ExtraLocalBase)) {
				//if(KeyTest(p_PFCurve->GetProfile(i))) {
					auto prof=p_PFCurve->GetProfile(i);
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
				//}
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

int PFSheets::GetMultiTypeIndex(string str)
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
void PFSheets::SaveTopRow()
{
	m_toprow=GetTopRow();
}
void PFSheets::SetTopRowExternt()
{
	SetTopRow(m_toprow);
	RedrawAll();
}
bool PFSheets::KeyTest(NEWHEADER header)
{
	vector<float> pressure;
	pressure=p_PFCurve->GetPF_Pressure(header.RecLayers);
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
void PFSheets::SetDescriptionRow(int Count, NEWHEADER header) 
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
	//	string=_T("");

	//	string+=header.County.c_str();
		string=header.Country.c_str();
		QuickSetText(5,Count,string);
		GetCell(6,Count,&cell);	cell.SetNumber(header.Upper_Clay); SetCell(6,Count,&cell);
		GetCell(7,Count,&cell);	cell.SetNumber(header.Upper_Sand); SetCell(7,Count,&cell);
		GetCell(8,Count,&cell);	cell.SetNumber(header.Upper_Org); SetCell(8,Count,&cell);
		GetCell(9,Count,&cell);	cell.SetNumber(header.Lower_Clay); SetCell(9,Count,&cell);
		GetCell(10,Count,&cell);	cell.SetNumber(header.Lower_Sand); SetCell(10,Count,&cell);
		GetCell(11,Count,&cell);	cell.SetNumber(header.Lower_Org); SetCell(11,Count,&cell);

		QuickSetNumber(12,Count,header.Year);
		QuickSetNumber(13,Count,p_PFCurve->GetNumPF_Pressures());

}
void PFSheets::MakeSelectedProfileSheet() {
		string str;
		int Count;
		size_t Record;
		CUGCell cell; 
		Count=0;
		SetNumberCols ( 2 );
		SetNumberRows (15);
		QuickSetText(-1,-1,"Data");
		QuickSetText(0,-1,"Selected Profile");
		QuickSetText(1,-1,"General Info");
	
		SetColWidth(-1,120);
		SetColWidth(0,140);
		SetColWidth(1,400);

		vector<string> lines=p_PFCurve->GetCommentsLines();
		for (size_t i = 0; i < lines.size(); i++) {
			if (i > 12) continue;
			QuickSetText(1, i, lines[i]);
		}

			if(!p_PFCurve->IsProfile()) {
				for(int i=0;i<9;i++) {
					QuickSetText(0,i,"");
					QuickSetText(-1,i,"");
				}

			}
			else {
				NEWHEADER test=p_PFCurve->m_ActProfile;
				QuickSetNumber(-1,Count,Count);
				str=FUtil::STD_ItoAscii(p_PFCurve->m_ActProfile.KeyProf/100);
				str+=":";
				str+=FUtil::STD_ItoAscii(p_PFCurve->m_ActProfile.KeyProf%100);
				GetCell(0,0,&cell);
				//QuickSetText(0,Count,string);
				cell.SetText(str.c_str());
				cell.SetBackColor(m_ColorClick);
				cell.SetReadOnly(TRUE);
				SetCell(0,0,&cell);
				QuickSetText(-1,0,"Profile no:");
				
	
				str=p_PFCurve->m_ActProfile.Name;
				//GetCell(1,1,&cell);
				cell.ClearAll();
				cell.SetText(str.c_str());
				cell.SetReadOnly(FALSE);
				SetCell(0,1,&cell);
				QuickSetText(-1,1,"Site",false);
			
				if(p_PFCurve->m_ActProfile.Lat>-1.E37) {
					str=FUtil::STD_FtoAscii(p_PFCurve->m_ActProfile.Lat);
					QuickSetText(-1,2,"Latitude");
					QuickSetText(0,2,str, false);
					
				}
				else {
					QuickSetText(0,2,"",false);

				}

				if(p_PFCurve->m_ActProfile.Long>-1.E37) {
					QuickSetText(-1,3,"Longitude");
					str=FUtil::STD_FtoAscii(p_PFCurve->m_ActProfile.Long);
					QuickSetText(0,3,str, false);
				}
				else {
					QuickSetText(0,3,"", false);


				}
			


				str=p_PFCurve->m_ActProfile.Country;
				QuickSetText(0,4,str, false);
				QuickSetText(-1,4,"Country");

				str = p_PFCurve->m_ActProfile.County;
				QuickSetText(0, 5, str, false);
				QuickSetText(-1, 5, "County");


				GetCell(0,6,&cell);
				cell.SetNumber(p_PFCurve->m_ActProfile.NumLayers );
				cell.SetReadOnly(false);
				SetCell(0,6,&cell);
				QuickSetText(-1,6,"Layers");
				QuickSetText(-1,7,"Year");
				GetCell(0,7,&cell);
				cell.SetText(MFC_Util::ItoAscii( p_PFCurve->m_ActProfile.Year  ));
				SetCell(0,7,&cell);
				GetCell(0,8,&cell);
				//QuickSetNumber(0,5,PLOTPF_mp_YEAR);
				QuickSetText(-1,8,"Heads");
				GetCell(0,8,&cell);
				vector<float> pressure=p_PFCurve->GetPF_Pressure(p_PFCurve->m_ActProfile.RecLayers);
				cell.SetNumber(pressure.size());
				SetCell(0,8,&cell);

				QuickSetText(-1, 9, "Created By");
				GetCell(0, 9, &cell);
				cell.SetText(p_PFCurve->m_ActProfile.CreatedBy);
				SetCell(0, 9, &cell);

				QuickSetText(-1, 10, "Date Created");
				GetCell(0, 10, &cell);
				cell.SetText(p_PFCurve->m_ActProfile.CreateDate);
				SetCell(0, 10, &cell);

				QuickSetText(-1, 11, "Modified By");
				GetCell(0, 11, &cell);
				cell.SetText(p_PFCurve->m_ActProfile.ModifiedBy);
				SetCell(0, 11, &cell);

				QuickSetText(-1, 12, "Date Modified");
				GetCell(0, 12, &cell);
				cell.SetText(p_PFCurve->m_ActProfile.ModifiedDate);
				SetCell(0, 12, &cell);



				QuickSetText(-1,13,"Data Base");
				if(m_LatestSheet==0)
					str=p_PFCurve->GetMainDataBaseName(true);
				else
					str=p_PFCurve->GetExtraDataBaseName(true);

				QuickSetText(0,13,str);



				QuickSetText(-1,14,"Current Link");
				if(m_pDoc==nullptr) {
					QuickSetText(0,14,"Link to Document not open");
				}
				else if(m_pDoc->IsFinishedSimulation()) {
					QuickSetText(0,14,"Link to completed simulation not possible");
				}
				else {
					QuickSetText(0,14,m_pDoc->GetCurrentSimFile());
				}

			}
			RedrawAll();
}
