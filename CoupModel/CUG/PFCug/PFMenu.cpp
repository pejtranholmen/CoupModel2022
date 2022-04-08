#pragma once
#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include "..\..\Util.h"
#include ".\PFMenu.h"
#include "..\..\DB\DB_Header.h"


PFMenu::PFMenu()
{
	//UGXPThemes::CleanUp();
}


PFMenu::~PFMenu()
{
	//UGXPThemes::CleanUp();
}
int PFMenu::OnMenuStart(int col,long row,int section)
{
	p_PFCurve=&PlotpF::GetPointer();
	int isheet = GetSheetNumber();
			//****** Populate the Menu 
	if(section==UG_GRID) {
			EmptyMenu();

			
			if(m_DataBaseString=="DataSheet") {
				if(isheet==8){
					AddMenuItem(2100, "Estimate pF curve by using measured retention points");
					AddMenuItem(3100, "Estimate pF curve by using texture (Sand, Clay and Porosity)");
				}
				else if(isheet==3) {
					AddMenuItem(2600, "Estimate Hydraulic conductivity by using texture (Sand, Clay and Porosity)");

				}
				AddMenuItem(2000, "Copy"); 
				AddMenuItem(3000, "Paste"); 
			}
			else if(m_DataBase&&m_DataBaseString!="DataSheet") {
				string str;
				CUGCell cell; 
	
					
				
				if(m_pDoc!=nullptr&&m_pDB!=nullptr) {

					GetCell(7,row,&cell);
					str=cell.GetStdText();
					CDB *pDB;
					size_t db_index=m_pDB->GetCurrentDBIndex();
					string strID, strIDKey;
					pDB=m_pDoc->DB_GetPointer(DB_types(db_index));
					size_t dest=pDB->GetLastSelectedRepetition()+1;
					string dest_str="("; dest_str+=FUtil::STD_ItoAscii(dest+1);dest_str+=")";
					//m_pDoc->MR_GetNumOfRepeatitionsOfDim(N_Dim-1)
					size_t N_Dim=m_pDoc->MR_GetNumberOfDimensions();
					size_t numrep=m_pDoc->MR_GetNumOfRepeatitionsOfDim(N_Dim-1);
					string numrep_str="("; numrep_str+=FUtil::STD_ItoAscii(numrep);numrep_str+=")";

					if(!m_pDoc->IsFinishedSimulation()) {
						AddMenuItem(5020, "Transfer properties from "+str+" to Sim Document ("+ FUtil::FileNameOnly(m_pDoc->GetCurrentSimFile()) +")");
						AddMenuItem(5100, "Select properties from "+str+" for first multirun repetition");
						if(dest<numrep)
							AddMenuItem(5101, "Select properties from "+str+" for "+dest_str.c_str()+" multirun repetition");
						AddMenuItem(5102, "Select properties from "+str+" for all "+numrep_str.c_str()+" multirun repetition");
						AddMenuItem(5110, "Select properties from "+str+" for changes within simulation period");

					}

					AddMenuItem(5030,"Create New Data Base Object based on results from : "+FUtil::FileNameOnly(m_pDoc->GetCurrentSimFile()));
					AddMenuItem(5040,"Delete Data Base Object : "+str);
					AddMenuItem(5050, "Export a Data Base Object : " + str+" to a CSV-file (*.csv)");
					AddMenuItem(5060, "Create a Data Base Object from a CSV-file");
				}
				else {
					CDB *pDB;
					pDB=m_pDoc->DB_GetPointer(DB_types(1));
					size_t dest=pDB->GetLastSelectedRepetition()+1;
					string dest_str="("; dest_str+=FUtil::STD_ItoAscii(dest+1);dest_str+=")";
					//m_pDoc->MR_GetNumOfRepeatitionsOfDim(N_Dim-1)
					size_t N_Dim=m_pDoc->MR_GetNumberOfDimensions();
					size_t numrep=m_pDoc->MR_GetNumOfRepeatitionsOfDim(N_Dim-1);
					string numrep_str="("; numrep_str+=FUtil::STD_ItoAscii(numrep);numrep_str+=")";		
	
					AddMenuItem(5010, "Add New Profile to data base");
					AddMenuItem(5011, "Import data from an existing CSV-file");
					GetCell(0,row,&cell);
					str=cell.GetStdText();
					str+=":";
					GetCell(1,row,&cell);
					str+=cell.GetStdText();
					AddMenuItem(5000, "Delete Profile "+str); 
					AddMenuItem(5200, "Export Info to CSV-file");

					if(!m_pDoc->IsFinishedSimulation()) {
						AddMenuItem(5020, "Transfer properties from "+str+" to Sim Document");
						AddMenuItem(5100, "Select properties from profile "+str+" for first multirun repetition");
						if(dest<numrep)
							AddMenuItem(5101, "Select properties from profile "+str+" for "+dest_str.c_str()+" multirun repetition");
						AddMenuItem(5102, "Select properties from profile "+str+" for all "+numrep_str.c_str()+" multirun repetition");

					}
					
					AddMenuItem(5110, "Select properties from profile "+str+" for changes within simulation period");


				}
			}
			else {
				AddMenuItem(2000, "Copy"); 
				AddMenuItem(3000, "Paste"); 
			}
	}
	else if(section==UG_SIDEHEADING) {
			EmptyMenu();
			if(m_DataBaseString=="DataSheet") {
				string strlay;
				strlay=FUtil::STD_ItoAscii(row+1);
				//int isheet=GetSheetNumber();
				if(isheet==6)
					AddMenuItem(2100, "Estimate pF curve by using measured retention points for layer ("+strlay+ ")");
				else if(isheet==5||isheet==4)
					AddMenuItem(3100, "Estimate pF curve by using pedofunction measured texture for layer ("+strlay+ ")");
			}
			else {
				AddMenuItem(5010, "Add New Profile to data base");



			}

	}
	else if(section==UG_CORNERBUTTON) {

		EmptyMenu();

		if(m_DataBaseString=="DataSheet") {
			AddMenuItem(2100, "Estimate pF curve by using measured retention points between for all soil layers");
		}
		else {
			AddMenuItem(6000, "Create New data base using an existing (csv file)");
			AddMenuItem(6010, "Open an existing database (dbn or dba files");
			AddMenuItem(6100, "Export current data base to a csv file");



		}

	}
	else if(section==UG_TOPHEADING) {
		if(m_DataBaseString!="DataSheet")
			AddMenuItem(5010, "Add New Profile to data base");

	}
	return TRUE;
}
