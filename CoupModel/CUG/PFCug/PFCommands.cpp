#pragma once

#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include "..\..\Util.h"
#include ".\PFCommands.h"
#include "..\..\SP\SP_Retrieve.h"
#include "..\..\NewBase\Doc.h"
#include "..\ChartProperties\ChartCug.h"
#include "SoilData.h"
#include "..\..\DB\DB_Header.h"
#include "DBDataCug.h"
#include ".\SoilData.h"

PFCommands::PFCommands()
{
	m_PF_ResidualFix=MISSING;
	m_SortOption=0;
	//UGXPThemes::CleanUp();
}
PFCommands::~PFCommands()
{
	//UGXPThemes::CleanUp();
}
void PFCommands::OnMenuCommand(int col,long row,int section,int item)
{
	CUGCell cell;
	if((item==5010||item==5011)&&m_DataBaseString!="DataSheet"&&p_PFCurve!=nullptr) {
		
			p_PFCurve->CloseMainDataBase();


			p_PFCurve->OpenMainFile(p_PFCurve->GetMainDataBaseName(false),true);
			if(item==5011) {
				//string inputfilename = MFC_Util::GetCSVFileName();
				if (p_PFCurve->Export_Import_ActualProfile_CSVFile(string::npos, false)) {

	
					MySingle::instance().GetPFCugPointer()->MakeBaseSheet();
					MySingle::instance().GetSoilDataPointer()->MakeBaseSheet();
					string sitekey;
					string str = FUtil::STD_ItoAscii(p_PFCurve->m_ActProfile.KeyProf / 100);
					str += ":";
					str += FUtil::STD_ItoAscii(p_PFCurve->m_ActProfile.KeyProf % 100);
					sitekey = str;


					string message;
					message = "A Profile was imported from (" + sitekey + ")",
					MFC_Util::MessageBox(message, "File Information", MB_OK);



					;
				}
				else {
					MFC_Util::MessageBox("Something went wrong when importing file- Please check input file !", "File Information", MB_OK);
				}
			// Create new profile.... by reading from file
			}
			else {
				if (p_PFCurve->Set_Header(p_PFCurve->GetNewHeader())) {
					p_PFCurve->UpdateHeaderFile();
					p_PFCurve->OpenEditMainDataBase();
					if (!p_PFCurve->SaveNewFormat());
					MySingle::instance().GetPFCugPointer()->MakeBaseSheet();
					MySingle::instance().GetSoilDataPointer()->MakeBaseSheet();
	
					//m_Prof_ToSim.OnSheetSetup(2);
					//m_SoilPropCug.OnSheetSetup(2);
					string sitekey;
					string str = FUtil::STD_ItoAscii(p_PFCurve->m_ActProfile.KeyProf / 100);
					str += ":";
					str += FUtil::STD_ItoAscii(p_PFCurve->m_ActProfile.KeyProf % 100);
					sitekey = str;


					string message;
					message = "New Profile was created (" + sitekey + " MySite) - Edit description field\n Make sure that all data are added !\n";
					message += "Some of the default values maybe misleading.\nNote for example that the suggested profile has 3 layers";
					MFC_Util::MessageBox(message, "File Information", MB_OK);
				}
				else {
					MFC_Util::MessageBox("New Profile could not be created !", "File Information", MB_OK);
				}
			}
		return;
	}
	else if(item==5020) {
	
		if(m_pDB!=nullptr) {
			size_t db_index=m_pDB->GetCurrentDBIndex(true);
			GetCell(-1,row,&cell);
			m_pDB->SetCurrentKeyLink(db_index, size_t(cell.GetNumber()));
			size_t adjust=0;
			if(db_index>0) adjust++;
			string db_id=m_pDoc->DB_GetNames(db_index+adjust);
			size_t rec_to_show=m_pDB->GetCurrentKeyIndex();

			DB_OldHeader header=m_pDB->GetHeader(db_index, rec_to_show);
			string testkey=m_pDB->GetKeyString(header.IdUser,header.IdNum);     

			if(m_pDoc->ReadFrom_DB_Object(db_id, testkey)) { 
				SIMB P_DB;
				P_DB.pBase=m_pDoc->DB_GetPointer(DB_types(db_index));					
				m_pDoc->DB_SetImportTimeT(db_index + adjust, time(nullptr));
				m_pDoc->m_DB_header[db_index + adjust] = header;
				m_pDoc->DB_SetChoice(DB_types(db_index+adjust), header.Name);

				CDB *pDB;
				pDB=(CDB*)P_DB.pBase;
				pDB->SetName(m_pDoc->DB_GetNames(db_index+adjust));
				pDB->SetValue(testkey);
				m_pDoc->History_Add(P_DB.pBase,db_index,time(nullptr),testkey);
			}
		}
		else if( p_PFCurve!=nullptr) {
			GetCell(0,row,&cell);
			auto prof=int(cell.GetNumber());
			GetCell(1,row,&cell);
			auto repNumber=int(cell.GetNumber());

			if(p_PFCurve->SetHeader(prof, repNumber)) {	

				m_pDoc->GetProfileFromPlotPF(false,true);
				string str;
				DB_OldHeader header;
				NEWHEADER pfheader=p_PFCurve->GetHeader(prof, repNumber);
				header.Latitude = FUtil::STD_FtoAscii(pfheader.Lat);
				header.Longitude = FUtil::STD_FtoAscii(pfheader.Long);
				header.keyId = p_PFCurve->m_ActProfile.OldKey;
				header.SiteName = p_PFCurve->m_ActProfile.Name;
				header.Country = pfheader.Country;
				m_pDoc->m_DB_header[1] = header;
				str=p_PFCurve->m_ActProfile.OldKey+"-"+p_PFCurve->m_ActProfile.Name;
				m_pDoc->DB_SetChoice(Soil_Properties, str);
				SIMB P_DB;
				P_DB.pBase=m_pDoc->DB_GetPointer(DB_types(1));
				m_pDoc->DB_SetImportTimeT(1, time(nullptr));

				CDB *pDB;
				pDB=(CDB*)P_DB.pBase;
				pDB->SetName("Soil Properties");
				pDB->SetValue(str);
				m_pDoc->History_Add(P_DB.pBase,1,time(nullptr),str);
			}
		}
	}
	else if(item==5030) {
		if(p_PFCurve==nullptr&&m_pDB!=nullptr) {
			size_t db_index=m_pDB->GetCurrentDBIndex(true);

			GetCell(-1,row,&cell);
			size_t index=m_pDB->CreateNewHeader(db_index);
			m_pDB->WriteHeaderFile(db_index);
			m_pDB->SetCurrentKeyLink(db_index, index);
			DB_OldHeader header=m_pDB->GetHeader(db_index, index);
			m_pDoc->SaveTo_DB_Object(m_pDoc->DB_GetNames(db_index,false),m_pDB->GetKeyString(header.IdUser,header.IdNum));
			m_pDB->SetDBModified(db_index);
			m_pDB->SetCurrentKeyLink(db_index, m_pDB->GetNumOfRec(db_index)-1);
			OnSheetSetup(db_index);
			MFC_Util::MessageBoxReturn("A new object has been created - Add and Edit additional information", "Message", 0);

		}
	}
	else if(item==5040) {
		if(p_PFCurve==nullptr&&m_pDB!=nullptr) {
			size_t db_index=m_pDB->GetCurrentDBIndex(true);
			GetCell(-1,row,&cell);
			auto index=size_t(cell.GetNumber());
			size_t oldrec=m_pDB->GetNumOfRec(db_index);
			if(index<=m_pDB->GetNumOriginal(db_index)) {
				auto ans = MFC_Util::MessageBoxReturn("Your Selected object belongs to original items - Do you really want to removed that from database", "Message", 1);
				if (ans != 1) {

					return;

				}
			
			
			
			}
			size_t numrec=m_pDB->DeleteRecord(db_index, index);
			if(oldrec!=numrec) {
				m_pDB->SetDBModified(db_index);
			}
			OnSheetSetup(db_index);
			MFC_Util::MessageBoxReturn("Your Selected object will be deleted and removed from database", "Message", 0);
		}
	}
	else if(item>=5100&&item<5110) {
		if(p_PFCurve==nullptr)
			SelectDB_Object_ToMultiRunSim(row, item);
		else if(p_PFCurve!=nullptr) {
			SelectPFCurve_Object_ToMultiRunSim(row, item);


		}

	}
	else if (item == 5050) {

		if (p_PFCurve == nullptr&&m_pDB != nullptr) {
			size_t db_index = m_pDB->GetCurrentDBIndex(true);
			GetCell(-1, row, &cell);
			m_pDB->SetCurrentKeyLink(db_index, size_t(cell.GetNumber()));
			size_t adjust = 0;
			if (db_index>0) adjust++;
			string db_id = m_pDoc->DB_GetNames(db_index + adjust);
			size_t rec_to_show = m_pDB->GetCurrentKeyIndex();

			DB_OldHeader header = m_pDB->GetHeader(db_index, rec_to_show);
			string testkey = m_pDB->GetKeyString(header.IdUser, header.IdNum);

			if (m_pDoc->ReadFrom_DB_Object(db_id, testkey, false, true, true)) {

				MFC_Util::MessageBoxReturn("Object Exported to : "+testkey+".Csv", "Export of object", 0);

			}
		}
		else {
			GetCell(0, row, &cell);
			auto prof = int(cell.GetNumber());
			GetCell(1, row, &cell);
			auto repNumber = int(cell.GetNumber());
			if (p_PFCurve->SetHeader(prof, repNumber)) {

				m_pDoc->GetProfileFromPlotPF(false, true);
				string str;
				str = p_PFCurve->m_ActProfile.OldKey + " " + p_PFCurve->m_ActProfile.Name;
				m_pDoc->DB_SetChoice(Soil_Properties, str);
				SIMB P_DB;
				P_DB.pBase = m_pDoc->DB_GetPointer(DB_types(1));
				m_pDoc->DB_ImportDate(1);
				m_pDoc->TimeModified();

				CDB *pDB;
				pDB = (CDB*)P_DB.pBase;
				pDB->SetName("Soil Properties");
				pDB->SetValue(str);
				m_pDoc->History_Add(P_DB.pBase, 1, time(nullptr), str);
			}
		}
	}
	else if (item == 5060) {
		// Import from a CSV file and create/change corrent DB object in database

		string inputfilename = MFC_Util::GetCSVFileName();
		CDB* pCDB = m_pDB->CreateNewDBObject_FromCSVFile(inputfilename);
		if (pCDB != nullptr) {
			m_pDB->WriteHeaderFile(pCDB->Get_DB_Index());

			DB_OldHeader header = m_pDB->GetHeader(pCDB->Get_DB_Index(), m_pDB->GetCurrentKeyIndex());
			m_pDB->SetDBModified(pCDB->Get_DB_Index());

			OnSheetSetup(pCDB->Get_DB_Index());
			MFC_Util::MessageBoxReturn("Succesful import of data from "+inputfilename+". New object ("+m_pDB->GetKeyString() +") is created", "Message", 0);
		}
		else
			MFC_Util::MessageBoxReturn("Failed to import data from "+inputfilename+" and create new object in database", "Message", 0);

	}
	else if(item==5110) {
		if(p_PFCurve==nullptr)
			SelectDB_Object_ToChaDuringRun(row, item);
		else if(p_PFCurve!=nullptr) {
			SelectPFCurve_Object_ToChaDuringRun(row, item);
		}

		// Select for within changes within runs
		//CChaParDuringRun	Dlg(P_DB,m_pSimDoc);
		//Dlg.DoModal(); 			


	}
	else if (item == 5200) {
		string str;
		GetCell(0, row, &cell);
		size_t keyno;
		str = cell.GetStdText();
		keyno = 100 * FUtil::AtoInt(str);
		str += ":";
		GetCell(1, row, &cell);
		str += cell.GetStdText();
		keyno += FUtil::AtoInt(cell.GetStdText());
		p_PFCurve->Export_Import_ActualProfile_CSVFile(keyno);



	}
	if(section==UG_GRID) {
		 if(m_DataBase&&item==5000) {

			CString String;
			m_pCUG->SaveTopRow();
					
			GetCell(0,row,&cell);
			auto prof=int(cell.GetNumber());
			GetCell(1,row,&cell);
			auto repnum=int(cell.GetNumber());
			size_t rec=p_PFCurve->GetHeaderRecord(p_PFCurve->GetKey(prof,repnum));
			p_PFCurve->SetHeader(rec);



			




			//rec_head=p_PFCurve->GetActHeaderRecord();

			string str=FUtil::STD_ItoAscii(p_PFCurve->m_ActProfile.KeyProf/100);
			str+=":";
			str+=FUtil::STD_ItoAscii(p_PFCurve->m_ActProfile.KeyProf%100);


			if(p_PFCurve->DeleteActHeader()) {
				MFC_Util::MessageBox("Profile ("+str+") deleted !","File Information",MB_OK);
				if(rec>0) {
					p_PFCurve->SetHeader(rec-1);	
					UpdateLinkToProfile();



				}
			}
			else 
				MFC_Util::MessageBox("Profile ("+ str+" could not be deleted !","File Information",MB_OK);
			
			//p_PFCurve->UpdateHeaderFile();
			p_PFCurve->OpenEditMainDataBase();
			if(!p_PFCurve->SaveNewFormat());


			m_pCUG->MakeBaseSheet();
			m_pCUG->SetTopRow(m_toprow);

			return;
/*
			if(p_PFCurve->DeleteProfile(row,false)) {
				int sheet;
				sheet=GetSheetNumber();
				OnSheetSetup(sheet);
				return;
			};
			*/
		}
		if(item==2100||item==3100||item==2600) {
			CUGCell cell;
			GetCell(0,row,&cell);
			int layer;
			if(GetSheetNumber()==8) layer=int(cell.GetNumber())-1;
			else layer=row;

			if(!p_PFCurve->IsMainWriteEnabled()) p_PFCurve->OpenEditMainDataBase();
			if(item==2100) 
				auto results=EstimatePFCurveByPoints(layer, false);
			else if(item==2600) 
				auto results=EstimateHydraulicCondByTexture(row, false);
			else
				auto results=EstimatePFCurveByTexture(row, false);
			SimB *pSimB;
			if(m_pDoc!=nullptr&&item==2600) {
				m_pDoc->GetProfileFromPlotPF(false,false);
				pSimB=m_pDoc->GetPtr(PAR_TABLE,"Total Conductivity");
				Tab *pTab=(Tab*)m_pDoc->GetPtr(TABLE,"Hydraulic conductivity, measured horizons");
				MakeChart(layer, pSimB,pTab, 0);
				MySingle::instance().GetSoilDataPointer()->OnSheetSetup(3);
			}
			else if(item==2100||item==3100) {
				pSimB=m_pDoc->GetPtr(PAR_TABLE,"Air Entry");
				Tab *pTab=nullptr;//(Tab*)m_pDoc->GetPtr(TABLE,"Hydraulic conductivity, measured horizons");
				MakeChart(row, pSimB,pTab, 0);
				MySingle::instance().GetSoilDataPointer()->OnSheetSetup(8);
			}
			

			return;

		}

		if(item== 1000)
			CutSelected();

		if(item==2000)
			CopySelected();

		if(item==3000) {
			// Paste information to sheet
			CUGCell cell;
			Paste();
			if(m_DataBaseString=="DataSheet") {
				int isheet=GetSheetNumber();
				long nrow=GetNumberRows();
				double value;
				PFCOEF pfcoef;
				pfcoef=p_PFCurve->GetPF_Coef(row);
				vector<float> texture,theta, pressure;
				pressure=p_PFCurve->GetPF_Pressure(p_PFCurve->m_ActProfile.RecLayers);
				texture.resize(8);
				theta.resize(pressure.size());
				theta=p_PFCurve->GetPF_Theta(row);
				texture=p_PFCurve->GetTexture(row);



				if(isheet==0) {
					for(long j=0;j<nrow;j++) {
							pfcoef=p_PFCurve->GetPF_Coef(j);
							//PLOTPF_mp_ACTUALLEVEL=j+1;
							GetCell(0,j,&cell);
							value=cell.GetNumber();
							pfcoef.UpperDepth=value;
							GetCell(1,j,&cell);	value=cell.GetNumber();
							pfcoef.LowerDepth=value;
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
							p_PFCurve->SetPF_Coef(j, pfcoef);
					}
				}
				else if(isheet==1) { //Genuchten
					for(long j=0;j<nrow;j++) {
							pfcoef=p_PFCurve->GetPF_Coef(j);
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
							p_PFCurve->SetPF_Coef(j, pfcoef);
					}

				}
				else if(isheet==2) {// Hydraulic
					for(long j=0;j<nrow;j++) {
							pfcoef=p_PFCurve->GetPF_Coef(j);
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
							p_PFCurve->SetPF_Coef(j, pfcoef);
					}
				}
				else if(isheet==3) {// Texture
						for(long j=0;j<nrow;j++) {
							pfcoef=p_PFCurve->GetPF_Coef(j);
							GetCell(0,j,&cell);
							value=cell.GetNumber();
							pfcoef.UpperDepth=value;
							GetCell(1,j,&cell);	value=cell.GetNumber();
							pfcoef.LowerDepth=value;
							float acc=0;
					
							texture=p_PFCurve->GetTexture(j);
							for (int jj=0;jj<7;jj++) {
								GetCell(2+jj,j,&cell);	value=cell.GetNumber();
								acc+=value;
								texture[jj]=acc;
							}
							GetCell(9,j,&cell);	value=cell.GetNumber();
							texture[7]=value;
							p_PFCurve->SetPF_Coef(j, pfcoef);
							p_PFCurve->SetPF_Texture(j, texture);
						}

				}
				else if(isheet==4) { // PF Points
				
						pressure=p_PFCurve->GetPF_Pressure(p_PFCurve->m_ActProfile.RecLayers);
						for(long j=0;j<nrow;j++) {
							pfcoef=p_PFCurve->GetPF_Coef(j);
							theta=p_PFCurve->GetPF_Theta(j);
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
						p_PFCurve->SetPF_Pressure(pressure);
				}
				else if(isheet==6) {
					if(p_PFCurve==nullptr) {

						int i=0;


					}


				}
				else if(isheet==9&&m_pDB2==nullptr&&item==3000) {
	
					CDB *pDB;	
					CUGCell cell;

					pDB=m_pDoc->DB_GetPointer("Soil Properties");
					size_t NRep=GetNumberRows();
					for(size_t i=0; i<NRep;i++) {
						string key_str=QuickGetStdText(0,i);
						pDB->MR_SetSelection(i, "Soil Properties", key_str);

					}
				}

				//PLOTPF_mp_SAVEDATABASE();

			}





		}

		if(item==2000||item==3000) {

			if(m_pDB2!=nullptr&&m_DataBase!=TRUE) {
				string str;
				CUGCell cell; 

				m_pDB2->MR_ResetSelection();
				for(int i=0;i<m_row;i++) {
					GetCell(0,i,&cell);
					str=cell.GetStdText();
					//QuickSetNumber(-1,i,i+1);
					m_pDB2->MR_AddSelection("Soil",str);
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
			if(!p_PFCurve->IsMainWriteEnabled()) p_PFCurve->OpenEditMainDataBase();
			string results=EstimatePFCurveByPoints(row, false);
			m_pDoc->GetProfileFromPlotPF(false,false);
			SimB *pSimB;
			pSimB=m_pDoc->GetPtr(PAR_TABLE,"Air Entry");
			Tab *pTab=(Tab*)m_pDoc->GetPtr(TABLE,"Hydraulic conductivity, measured horizons");
			MakeChart(row, pSimB,pTab, 0);
			MySingle::instance().GetSoilDataPointer()->OnSheetSetup(8);
			//m_pSP_Retrieve_Dlg->OnAddComments(results);
		}
		else if(item==3100) {
	// Estimate pF Curve
			if(!p_PFCurve->IsMainWriteEnabled()) p_PFCurve->OpenEditMainDataBase();
			string results=EstimatePFCurveByTexture(row, false);
			m_pDoc->GetProfileFromPlotPF(false,false);
			SimB *pSimB;
			pSimB=m_pDoc->GetPtr(PAR_TABLE,"Air Entry");
			Tab *pTab=(Tab*)m_pDoc->GetPtr(TABLE,"Hydraulic conductivity, measured horizons");
			MakeChart(row, pSimB,pTab, 0);
			MySingle::instance().GetSoilDataPointer()->OnSheetSetup(8);
		}


	}
	else if(section==UG_CORNERBUTTON) {
		if (item == 6000) {
			// Create new Data base
			FILEDLG FDLG = MFC_Util::GetFileDialog("csv", "", "Comma separated file (*.csv)|*.csv|");
			string filename;
			if (FDLG.opengood)
				filename = FDLG.filename;
			else
				filename = "";

			if (pGetSoil->OpenMainFile(filename, true)) {
				pGetSoil->CreateSimpleDataBase();
				pGetSoil->IniNewDataBase();
				pGetSoil->ReadNewHeaderFile();

				SOILHEADER header = pGetSoil->GetSoilHeaderByNum(0);
				vector<float> values;
				if (header.NumLayers != string::npos)
					for (size_t layer = 0; layer<header.NumLayers; layer++)
						values = pGetSoil->GetSoilData(header.KeyProf, header.NumProf, layer);


			}

			return;


		}
		else if (item == 6010) {
			// Open and existing data base
			string filename;
			// 		CFileDialog	NewFileDlg(TRUE,lpszDefExt,pszFileName, OFN_NOCHANGEDIR,lpszFilter);
			FILEDLG FDLG = MFC_Util::GetFileDialog("dbn|dba", "", "Data Base File (*.dbn)|*.dbn|Old Data Base File (*.dba)|*.dba|");
			//string filename;

			if (FDLG.opengood) {
				filename = FDLG.filename;
				size_t pos = filename.find(".dba");
				if (pos == string::npos) pos = filename.find(".DBA");
				if (pos != string::npos)
					filename = filename.substr(0, pos);
				else {
					pos = filename.find(".dbn");
					if (pos == string::npos) pos = filename.find(".DBN");

					if (pos != string::npos)
						filename = filename.substr(0, pos);
				}


				if (m_LatestSheet == 0) {//Main DataBase 
					p_PFCurve->CloseMainDataBase();
					p_PFCurve->OpenMainFile(filename, p_PFCurve->IsMainWriteEnabled());
					p_PFCurve->ReadHeaderFile();
					p_PFCurve->SaveNewFormat();
					p_PFCurve->CloseMainDataBase();
					p_PFCurve->OpenMainFile(filename, p_PFCurve->IsMainWriteEnabled());

				}
				else
					p_PFCurve->OpenExtraFile(filename, p_PFCurve->IsExtraWriteEnabled());

				OnSetup();

			}



		}
		else if (item == 6100) {
			//Create a Export File CSV
			p_PFCurve->ExportMainDataBasetoCSVFile();


		}

		else if(item==2100) {
			// Estimate pF Curve
			string results;
			for(size_t i=0; i<p_PFCurve->m_ActProfile.NumLayers;i++)
				results=EstimatePFCurveByPoints(i, false);

		}
	}
}
string PFCommands::EstimatePFCurveByPoints(size_t Layer, bool All) {
	string str="";
	vector<double> Theta, Psi;
	vector<float> pressure, theta;
	size_t NumEstimated=0;
	pressure=p_PFCurve->GetPF_Pressure(p_PFCurve->m_ActProfile.RecLayers);
	PFCOEF pfcoef;
	pfcoef=p_PFCurve->GetPF_Coef(Layer);
	theta=p_PFCurve->GetPF_Theta(Layer);
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

		BC_ESTIMATE bc;
		if(m_PF_ResidualFix>0)
			bc=p_PFCurve->Estimate_BC(&Theta, &Psi,pfcoef.Saturation, m_PF_ResidualFix);
		else
			bc=p_PFCurve->Estimate_BC(&Theta, &Psi,pfcoef.Saturation, -1.);
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
		bc.Layer=Layer+1;
		BC_RESULT res;
		res.Prof=p_PFCurve->m_ActProfile.KeyProf;
		res.bc=bc;
		res.EstimateMin=m_PF_EstimateMin;
		res.EstimateMax=m_PF_EstimateMax;
		m_EstimationResults.push_back(res);
		
		p_PFCurve->SetPF_Coef(Layer, pfcoef);
		m_pDoc->m_Simulator.m_SoilModel.m_SoilFrost.ReCalc();

	}
	else {
		str="No Estimation made";

	}
	return str;
}
string PFCommands::EstimatePFCurveByTexture(size_t Layer, bool All) {
	string str="";
	PFCOEF pfcoef;
	pfcoef=p_PFCurve->GetPF_Coef(Layer);
	vector<float> texture=p_PFCurve->GetTexture(Layer);
	double min_frac=(100.-texture[7])/100.;
	BC_ESTIMATE bc;
	if(min_frac>0) {
		double sand=(100.-texture[3]-texture[7])/min_frac;
		double clay=texture[0]/min_frac;
		bc=p_PFCurve->Estimate_BC_Texture(sand,clay,pfcoef.Saturation/100);
	}
	else
		return "No Mineral content. Not possible to estimate PF curve by texture";
	pfcoef.AirEntry=bc.AirEntry;
	pfcoef.Residual=bc.Residual;
	pfcoef.Lambda=bc.Lambda;
	BC_RESULT res;
	res.Prof=p_PFCurve->m_ActProfile.KeyProf;
	res.EstimateMax=MISSING;
	res.EstimateMin=MISSING;
	bc.n=0;
	bc.Layer=Layer;
	bc.R2=MISSING;
	res.bc=bc;
	m_EstimationResults.push_back(res);
	p_PFCurve->SetPF_Coef(Layer, pfcoef);
	str="New coefficients estimated Using pedofunction estimated";

	return str;
}
string PFCommands::EstimateHydraulicCondByTexture(size_t Layer, bool All) {
	string str="";
	PFCOEF pfcoef;
	pfcoef=p_PFCurve->GetPF_Coef(Layer);

	vector<float> texture=p_PFCurve->GetTexture(Layer);
	double min_frac=(100.-texture[7])/100.;
	BC_ESTIMATE bc;
	
	if(min_frac>0) {
		double sand=(100.-texture[3]-texture[7])/min_frac;
		double clay=texture[0]/min_frac;
		double cond=FUtil::PedoFunc_HydraulicCond(sand, clay, pfcoef.Saturation/100);
		pfcoef.TotConductivity=cond;
		pfcoef.MatConductivity=cond;
	}
	p_PFCurve->SetPF_Coef(Layer, pfcoef);
	str="New hydraulic conductivty estimated Using pedofunction estimated";

	return str;
}
bool PFCommands::EditOfProfileInfo(int row, string str) {

	size_t nc;
	size_t nkol;
	string newstring;
	bool ChangePositionLevelFile;
	ChangePositionLevelFile = false;
	vector<float> Pressure;
		switch (row) {

	case 1:
		p_PFCurve->m_ActProfile.Name = str;
		break;
	case 2:
		p_PFCurve->m_ActProfile.Lat = FUtil::AtoFloat(str);

		break;
	case 3:
		p_PFCurve->m_ActProfile.Long = FUtil::AtoFloat(str);

		break;
	case 4:
		p_PFCurve->m_ActProfile.Country = str;
		break;
	case 5:
		p_PFCurve->m_ActProfile.County = str;
		break;
	case 6:
		p_PFCurve->ExpandToNewLayerActHeader(FUtil::AtoInt(str));
		p_PFCurve->m_ActProfile.NumLayers = FUtil::AtoInt(str);
		ChangePositionLevelFile = true;
		//m_pCUG_Data->OnSetup();
// Make change of database struture !!
		break;
	case 7:
		p_PFCurve->m_ActProfile.Year = FUtil::AtoInt(str);
		ChangePositionLevelFile = true;
		//m_pCUG_Data->OnSetup();
		break;
	case 8:

		Pressure = p_PFCurve->GetPF_Pressure(p_PFCurve->m_ActProfile.RecLayers);
		Pressure.resize(FUtil::AtoInt(str));
		p_PFCurve->SetPF_Pressure(Pressure);
		Pressure = p_PFCurve->GetPF_Pressure(p_PFCurve->m_ActProfile.RecLayers);
		break;
	case 9:
		p_PFCurve->m_ActProfile.CreatedBy = str;
		break;
	case 10:
		p_PFCurve->m_ActProfile.CreateDate = str;
		break;
	}
	
	p_PFCurve->m_ActProfile.ModifiedDate = MFC_Util::GetCurrentDateTime();
	p_PFCurve->m_ActProfile.ModifiedBy = FUtil::GetWindowUser();

	if(!p_PFCurve->UpdateActualProfile()) {
			p_PFCurve->UpdateHeaderFile();

	}
		
		p_PFCurve->OpenEditMainDataBase();
		if(!p_PFCurve->SaveNewFormat()) return false;
		//m_pCUG->MakeBaseSheet();
		if(ChangePositionLevelFile&&m_pCUG_Data!=nullptr) {

			m_pCUG_Data->OnSetup();
			
		}
			//m_pCUG->OnSetup();
		return true;


}
bool PFCommands::EditOfLayerInfo(int col, int row, string str) {
	CUGCell cell;
	int isheet=GetSheetNumber();
		int prof1=p_PFCurve->m_ActProfile.KeyProf/100;
		int prof2=p_PFCurve->m_ActProfile.KeyProf%100;
		PFCOEF pfcoef;
		pfcoef=p_PFCurve->GetPF_Coef(row);
		vector<float> texture,theta, pressure;
		pressure=p_PFCurve->GetPF_Pressure(p_PFCurve->m_ActProfile.RecLayers);
		texture.resize(8);
		theta.resize(pressure.size());
		theta=p_PFCurve->GetPF_Theta(row);
		texture=p_PFCurve->GetTexture(row);
		if(isheet==0) {
			if(col==0)	return EditOfProfileInfo(row, str);
			else if(col==1) {
				vector<string>lines = p_PFCurve->GetCommentsLines();
				if (row < lines.size()) {
					lines[row] = str;
				}
				else
					lines.push_back(str);
				str.resize(0);
				for (size_t i = 0; i < lines.size(); i++)
					str += lines[i]+'\n';
				str = str.substr(0, str.size() - 1);

				if(p_PFCurve->SetComments(str)>0)
					return true;
				else
					return false;
			}
			else return false;
		}
		else if(isheet==1) {
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
		else if(isheet==2) { //Genuchten
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
		else if(isheet==3) {// Hydraulic
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
				case 5:
					pfcoef.N_SrCoef=FUtil::AtoFloat(str);
					break;
				case 6:
					pfcoef.N_SECoef=FUtil::AtoFloat(str);
					break;
			}
		}
		else if(isheet==4) {// Texture
			switch (col) {
				case 0:
					pfcoef.UpperDepth=FUtil::AtoFloat(str);
					break;
				case 1:
					pfcoef.LowerDepth=FUtil::AtoFloat(str);
					break;

				case 2: {
					vector<float> prev_v;
					for (size_t i = 0; i < 8; i++) prev_v.push_back(texture[i]);
					texture[0] = FUtil::AtoFloat(str);
					for (size_t ii = 1; ii < 6; ii++) {
						double k = texture[ii-1];
						double diff = (prev_v[ii] - prev_v[ii-1])/(100.-prev_v[0]-texture[7]);
						texture[ii] = diff*(100 - k-texture[7])+k;
					}
					break;
				}
				case 9:
					texture[col-2]=FUtil::AtoFloat(str);
					break;
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8: {
					float v;
					v = 0;

					for (int j = 0; j < col - 2; j++) {
						GetCell(j + 2, row, &cell);
						v += cell.GetNumber();
					}
					double prev = texture[col - 2];
					texture[col - 2] = v + FUtil::AtoFloat(str);
					double masskoll = 100. - texture[5] - texture[7];
					if (masskoll < 0) texture[col - 2] += masskoll;
					double diff = texture[col - 2] - prev;
					double delta = diff / (9. - double(col));
					for (size_t i = 1; i < 9 - col; i++) {
						texture[col - 2 + i] += diff - delta*i;
					}
				}
					/*

					if(col==7||col==5) {
						GetCell(2+7,row,&cell);
						float v7=cell.GetNumber();
						GetCell(2+5,row,&cell);
						float v5=cell.GetNumber();
						texture[6]=100-v7-v5;
					}*/
					break;
			}


		}
		else if(isheet==5) {
			// Pedo Info
			if(row>=p_PFCurve->m_ActProfile.NumLayers) {

			}
			else {
				if(col>=2) {
					 if(col==2) {
						 //Clay
						double prev = texture[0];
						texture[col - 2] = FUtil::AtoFloat(str);
						double diff = texture[0] - prev;
						for (size_t i = 1; i < 6; i++) { texture[i] += (1. - float(i) / 5.)*diff; }
					}
					 else if(col==3) { //Sand
						 double prev_sand = 100 - texture[3] - texture[7];
						 double min_frac=(100.-texture[7])/100.;
						 texture[3]=min(100-FUtil::AtoFloat(str)*min_frac-texture[7], 100-texture[7]);
						 double diff = 100-texture[3]-texture[7];
						 
						 texture[4] = texture[3]+diff /3.;
						 texture[5] = texture[4]+diff/3.;
						 texture[1] =texture[0]+(texture[3]-texture[0])/3.;
						 texture[2] = texture[1] +(texture[3] - texture[0]) / 3.;
					}
					 else if (col == 4) {
						 texture[7] = FUtil::AtoFloat(str);
					 }
				}
			}
		}
		else if(isheet==6) { // PF Points
			if(col==0) {
				pressure[row]=FUtil::AtoFloat(str);
			}
			p_PFCurve->SetPF_Pressure(pressure);
			return true;
		}
		else if(isheet==7) { // PF Points
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
			p_PFCurve->SetPF_Theta(row,theta);
			return true;
		}
		else if(isheet==8&&row==0) { // Comments
			if(col==6)
				m_PF_EstimateMin=FUtil::AtoFloat(str);
			else if(col==7)
				m_PF_EstimateMax=FUtil::AtoFloat(str);
			else if(col==5)
				m_PF_ResidualFix=FUtil::AtoFloat(str);
			
			//p_PFCurve->SetComments(string(str));		
		}
		else if(isheet==10&&col==0) {
			CDB *pDB=m_pDoc->DB_GetPointer(DB_types(1));
			size_t NumSelected=pDB->GetNumChaDates();
			if(row==NumSelected) {
				long nextdate=PGUtil::MinutConv(string(str));
				if(m_pDoc!=nullptr)
					m_pDoc->SetNextChaParDate(nextdate);
			}



		}

		if (isheet == 4 || isheet == 5) {
			p_PFCurve->SetPF_Texture(row, texture);
			OnSheetSetup(isheet);
			return true;


		}
		if(!p_PFCurve->SetPF_Coef(row, pfcoef)) return true;
		else
			return false;
		//PLOTPF_mp_SAVEDATABASE();


	//return true;
}
void PFCommands::MakeChart(size_t index, SimB *pSimB, Tab* pTab, size_t FuncNo) {
	ChartCug *pChartCug;
	pChartCug=MySingle::instance().GetChartCugPointer();
	if(!m_DataImported) {
		m_pDoc->GetProfileFromPlotPF( false, false);
		m_DataImported=true;
	}
	pChartCug->Init(m_pDoc);
	pChartCug->ParameterPlotMode(TRUE);
	if(pTab==nullptr)
		pTab=dynamic_cast<Tab*>(m_pDoc->GetPtr(TABLE,"Brooks-Corey, water retention, measured horizons"));
	if(pTab!=nullptr) {
		pChartCug->m_ParameterPlot.InitData(m_pDoc, pSimB, pTab, index, FuncNo);
		pChartCug->m_ParameterPlot.OnInit();
		pChartCug->OnSetup();
	}
}
void PFCommands::SelectDB_Object_ToMultiRunSim(long row, int item) {
		CDB *pDB;	
		CUGCell cell;
		size_t db_index=m_pDB->GetCurrentDBIndex();
		GetCell(-1,row,&cell);
		//cell.GetText();
		auto index=size_t(cell.GetNumber());
		m_pDB->SetCurrentKeyLink(db_index, index);
		string strID, strIDKey;
		pDB=m_pDoc->DB_GetPointer(DB_types(db_index));
		size_t keyindex=m_pDB->GetCurrentKeyIndex();
		DB_OldHeader header=m_pDB->GetHeader(db_index, keyindex);
		strIDKey=m_pDB->GetKeyString(header.IdUser, header.IdNum);
		strID=pDB->GetName();		   
		pDB->MR_Set_Method(MR_METHOD(5));
		size_t N_Dim;
		N_Dim=m_pDoc->MR_GetNumberOfDimensions();
		if(N_Dim==0) {
			m_pDoc->MR_AddNewDimensiomWith11Repeatitions();
		}
		N_Dim=m_pDoc->MR_GetNumberOfDimensions();
		size_t NumSelected=pDB->MR_GetNumberOfSelections();
		

		if(item==5100) {
			if (NumSelected==0)
				pDB->MR_AddSelection(strID, strIDKey);
			else
				pDB->MR_SetSelection(0,strID,strIDKey);

		}
		else if(item==5101) {
			size_t dest = pDB->GetLastSelectedRepetition() + 1;
			if (NumSelected<=dest )
				pDB->MR_AddSelection(strID, strIDKey);
			else
				pDB->MR_SetSelection(dest,strID,strIDKey);

		}
		else if(item==5102) {
			pDB->MR_ResetSelection();
			for(size_t i3=0;i3<m_pDoc->MR_GetNumOfRepeatitionsOfDim(N_Dim-1);i3++) 
					pDB->MR_AddSelection(strID, strIDKey);
	
		}

	   pDB->MR_Set_Dim(N_Dim);
	   pDB->MR_Set_Method(MR_METHOD(5));
	   m_pDoc->MR_CP_Set((Ps*)pDB,-1,true);
	   m_pDBDataCug->Make_Sheet();
}
void PFCommands::SelectPFCurve_Object_ToMultiRunSim(long row, int item) {
		CDB *pDB;	
		CUGCell cell;
		GetCell(0, row, &cell);
		size_t prof = size_t(cell.GetNumber()+1.E-7);
		GetCell(1, row, &cell);
		size_t rep = size_t(cell.GetNumber()+1.E-7);
		string key;
		key = FUtil::STD_ItoAscii(prof);
		key += ":";
		key += FUtil::STD_ItoAscii(rep);
	
		pDB=m_pDoc->DB_GetPointer("Soil Properties");
		
		ModelUtil::SetMRSoilPropConnectionsProfNumberOnly(m_pDoc);
		
		//string key=FUtil::STD_ItoAscii(p_PFCurve->m_ActProfile.KeyProf/100);
	//	key+=":";
		//key+=FUtil::STD_ItoAscii(p_PFCurve->m_ActProfile.KeyProf%100);
		string strID="Soil Properties";
		
		   
		pDB->MR_Set_Method(MR_METHOD(5));
		size_t N_Dim;
		N_Dim=m_pDoc->MR_GetNumberOfDimensions();
		if(N_Dim==0) {
			m_pDoc->MR_AddNewDimensiomWith11Repeatitions();
		}
		N_Dim=m_pDoc->MR_GetNumberOfDimensions();
		size_t NumSelected=pDB->MR_GetNumberOfSelections();
		if(item==5100) {
			if(NumSelected>=1)
				pDB->MR_SetSelection(0,strID,key);
			else
				pDB->MR_AddSelection(strID, key);


		}
		else if(item==5101) {

			size_t dest=pDB->GetLastSelectedRepetition()+1;
			if(NumSelected>dest)
				pDB->MR_SetSelection(dest,strID,key);
			else
				pDB->MR_AddSelection(strID,  key);

		}
		else if(item==5102) {
			pDB->MR_ResetSelection();
			for(size_t i3=0;i3<m_pDoc->MR_GetNumOfRepeatitionsOfDim(N_Dim-1);i3++) 
					pDB->MR_AddSelection(strID, key);
	
		}

	   pDB->MR_Set_Dim(N_Dim);
	   pDB->MR_Set_Method(MR_METHOD(5));
	   m_pDoc->MR_CP_Set((Ps*)pDB,-1,true);
	   MySingle::instance().GetSoilDataPointer()->OnSheetSetup(9);


	
}
void PFCommands::SelectDB_Object_ToChaDuringRun(long row, int item) {
		CDB *pDB;	
		CUGCell cell;
		size_t db_index=m_pDB->GetCurrentDBIndex();
		GetCell(-1,row,&cell);
		//cell.GetText();
		auto index=int(cell.GetNumber());
		m_pDB->SetCurrentKeyLink(db_index, index);
		string strID, strIDKey;
		pDB=m_pDoc->DB_GetPointer(DB_types(db_index));
		
		size_t keyindex=m_pDB->GetCurrentKeyIndex();
		DB_OldHeader header=m_pDB->GetHeader(db_index, keyindex);
		strIDKey=m_pDB->GetKeyString(header.IdUser, header.IdNum);
		strID=pDB->GetName();		   

		size_t NumSelected=pDB->GetNumChaDates();
		long newdate=m_pDoc->GetNextChaParDate();
		pDB->SetChaDateAndValues(newdate,strID,strIDKey);

	   m_pDoc->Set_P_CH((SimB*)pDB,2,0);
	   m_pDBDataCug->Make_Sheet();
}
void PFCommands::SelectPFCurve_Object_ToChaDuringRun(long row, int item) {
		CDB *pDB;	
		CUGCell cell;

		pDB=m_pDoc->DB_GetPointer("Soil Properties");

		string key=FUtil::STD_ItoAscii(p_PFCurve->m_ActProfile.KeyProf/100);
		key+=":";
		key+=FUtil::STD_ItoAscii(p_PFCurve->m_ActProfile.KeyProf%100);
		string strID="Soil Properties";


		long newdate=m_pDoc->GetNextChaParDate();
		pDB->SetChaDateAndValues(newdate,strID,key);

	   m_pDoc->Set_P_CH((SimB*)pDB,2,0);



		   

	 MySingle::instance().GetSoilDataPointer()->OnSheetSetup(10);


	
}
/////////////////////////////////////////////////////////////////////////////
//	return UG_SUCCESS to allow the menu to appear
//	return 1 to not allow the menu to appear
void PFCommands::UpdateLinkToProfile() {
	if(m_pSoilData!=nullptr) m_pSoilData->OnSetup();

}