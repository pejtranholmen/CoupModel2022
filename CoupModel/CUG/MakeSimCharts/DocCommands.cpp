#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include "..\..\Util.h"
#include "..\..\NewBase\FileScanner.h"
#include "DocCommands.h"
#include "..\ChartProperties\ChartCug.h"
#include "..\..\MainFrm.h"

#include "..\..\NewBase\HistoryList.h"
#include "..\..\Run\ChaParDuringRun.h"
#include "..\..\SP\SP_Retrieve.h"
#include "..\..\MultiRun\MultiRun.h"
//#include "..\..\Validation\ValidationEvaluation.h"
#include "..\..\Validation\ValidationVariables.h"
#include "..\..\Validation\SelectWindowForValidationSummaryVariables.h"
#include "..\..\PG\CreatePG.h"
#include "..\ColourPickerDlg.h"
#include "..\..\MultiRun\MultiRunTable.h"

DocCommands::DocCommands(void)
{
	//m_pChartCug=MySingle::instance().GetChartCugPointer();
}
DocCommands::~DocCommands(void)
{
}
int DocCommands::RunInfoCommand(int col, int row, int item) {
CUGCell cell;
SimB *pBase=nullptr;
int index;
SIMB P_DB;
		GetCell(col,row, &cell); 
		long ParV=cell.GetParam();
		if(ParV<500) {
			int info_i;
			info_i=-10;
			switch(ParV) {
				case(101):
					info_i=0;
					break;
				case(102):
					info_i=2;
					break;
				case(103):
					info_i=3;
					break;
				case(104):
					info_i=4;
					break;
				case(105):
					info_i=5;
					break;
				case(106):
					info_i=6;
					break;
				case(107):
					info_i=7;
					break;
				case(108):
					info_i=7;
					break;
				case(109):
					info_i=8;
					break;
				case(110):
					info_i=9;
					break;
				case(111):
					info_i=10;
					break;
				case(112):
					info_i=1;
					break;
			}

			if(info_i>=0) {
				pBase=(SimB*)m_pSimDoc->m_RunInfo[info_i].pBase;
			}
			index=-1;

			if(item==2080) {
				CHistoryList dlg(pBase,p_ModelInfo->GetTypeIndex("RunInfo"),m_pSimDoc, index);	
				if(dlg.DoModal()==IDOK) {


				}
				return true;
			}

			P_DB.pBase = nullptr;

		}
		else {
			index=ParV-500;
			P_DB.pBase=m_pSimDoc->DB_GetPointer(DB_types(index));
			//pDB=(CDB*)pBase;
		}
		if (item == 4000) 
			m_pSimDoc->Export_MBinToCSV(false);
		else if (item == 4001) 
			m_pSimDoc->Export_MBinToCSV(true);
		else if (item == 4002|| item == 4003) {
			string outfile;
			if(item==4002)
				outfile=m_pSimDoc->Export_XLSX(false);
			else
				outfile=m_pSimDoc->Export_XLSX(true);
			string message = "Data Exported to file : " + outfile;
			MFC_Util::MessageBox(message);
		}
		else if (item == 4004 || item == 4005) {
			string outfile;
			if (item == 4004)
				outfile = m_pSimDoc->Export_XLSX(false,ExportOptions::Performance);
			else
				outfile = m_pSimDoc->Export_XLSX(true, ExportOptions::Performance);
			string message = "Data Exported to file : " + outfile;
			MFC_Util::MessageBox(message);
		}
		else if (item == 4006 || item == 4007) {
			string outfile;
			if (item == 4006)
				outfile = m_pSimDoc->Export_XLSX(false, ExportOptions::Outputs);
			else
				outfile = m_pSimDoc->Export_XLSX(true, ExportOptions::Outputs);
			string message = "Data Exported to file : " + outfile;
			MFC_Util::MessageBox(message);
		}
		else if (item == 4008 || item == 4009) {
			string outfile;
			if (item == 4008)
				outfile = m_pSimDoc->Export_XLSX(false, ExportOptions::Parameters);
			else
				outfile = m_pSimDoc->Export_XLSX(true, ExportOptions::Parameters);
			string message = "Data Exported to file : " + outfile;
			MFC_Util::MessageBox(message);
		}
		else if (item == 4020) {
			auto fileout=m_pSimDoc->Sim2XLSX(false);
			string message = "Data Saved to file : "+fileout;
			MFC_Util::MessageBox(message);
		}
		else if (item == 4022) {
			auto fileout = m_pSimDoc->Sim2XLSX(false, true);
			string message = "Data Saved to file : " + fileout;
			m_pSimDoc->WriteEntireModelToXmlFile();
			MFC_Util::MessageBox(message);
		}
		else if (item == 4021) {
			auto filein=m_pSimDoc->Sim2XLSX(true);
			string message = "Data Read from file: "+filein;
			MFC_Util::MessageBox(message);
		}
		else if (item == 4030) {
			auto fileout = m_pSimDoc->WriteEntireModelToXmlFile(doc_enabled::NOT_DEFAULT);
			string message = "Data Saved to file : " + fileout;
			MFC_Util::MessageBox(message);
		}
		else if (item == 4032) {
			auto fileout = m_pSimDoc->WriteEntireModelToXmlFile(doc_enabled::ENABLED);
			string message = "Data Saved to file : " + fileout;
			MFC_Util::MessageBox(message);
		}
		else if (item == 4033) {
			auto fileout = m_pSimDoc->WriteEntireModelToXmlFile(doc_enabled::ALL);
			string message = "Data Saved to file : " + fileout;
			MFC_Util::MessageBox(message);
		}

		else if(item==1999) {
			m_pSimDoc->SetFinishedSimulation();
			SetSheetNumber(0,FALSE);
			return true;
		}
		else if(item==3000) {
			bool test=FALSE;
			if(m_pSimDoc->GetValidResidualPointer()!=nullptr) {
						for(int i=1;i<MAXSIMVAL;i++) {
							void *pCF=m_pSimDoc->GetValidResidualPointer(i);
							if(pCF!=nullptr)
								test=TRUE;
						}
			}
			int ans;
			ans=MFC_Util::MessageBoxReturn("Old files will be overwrited, please make a copy of current files if you want to be able to undo your operation.\n Do you want to continue ?","Adding data to MBIN and making link to XBIN file",MB_YESNO);
			if(ans!=IDYES) return 0;

			BeginWaitCursor();
			m_pSimDoc->UnLockMultiStorageFile();
			m_pSimDoc->m_MStorage.AlloSplitPerformance(0); // Remove any split mode
			if(m_pSimDoc->m_MStorage.AddExistingFile(""))
				m_pSimDoc->m_MStorage.UpdateAcceptedRuns(TRUE, TRUE);
			else {
				MFC_Util::MessageBox("Addition of files failed !","Adding data to MBIN file",MB_OK);
				return -1;
			}
// Action for adding new information to MBIN or XBIN files
			int ifiles=m_pSimDoc->m_MStorage.GetNumberOfAddedFiles(m_pSimDoc->GetMBinFileName());
			CString strbase=CString(m_pSimDoc->m_MStorage.GetNameAddedFiles(ifiles-1).c_str());
			int ipos=strbase.Find(_T("_Multi_"));
			CString numbase=strbase.Mid(ipos+7,5);
			CString filename;

			if(m_pSimDoc->GetValidResidualPointer()!=nullptr) {
				/*		CFileScanner ScanMeanProg(nullptr);
						ScanMeanProg.Create(IDD_PROGRESS,nullptr);*/

						for(int i=1;i<MAXSIMVAL;i++) {
							CResiduals *pRes;
								pRes=m_pSimDoc->GetValidResidualPointer(i);
							CString test=CString(m_pSimDoc->GetXBinFileName(i).c_str());
							if(pRes!=nullptr){
								int i=test.ReverseFind('\\');
								test+=test.Mid(i+1,test.GetLength()-i-1)+_T(";");
							}
							
							if(test.GetLength()>1) {
								filename=strbase.Mid(0,ipos)+_T("\\V")+MFC_Util::ItoAscii(i)+
									_T("_Residuals_")+numbase+_T(".Xbin");
							/*	ScanMeanProg.m_Info=filename;
								ScanMeanProg.ShowWindow(SW_SHOW);
								pRes->AddExistingFile(string(filename), &ScanMeanProg);*/
							}
						}
			}
			EndWaitCursor();
			string news=FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_TotalNumberofRuns());
			MFC_Util::MessageBox("Addition of files completed All data ("+ news+") runs in current file","Adding data to MBIN  file",MB_OK);
			EnableUpdate(FALSE); 
			SetSheetNumber(0);
			EnableUpdate(FALSE); 
			while(GetNumberRows()>0){
				//for(int i=0;i<4;i++)
				//	UnJoinCells(i,0);				
				DeleteRow(0);
			}
			EnableUpdate(TRUE); 
			return 0;
		}
		else if(item==3001) {
			bool test=FALSE;
			if(m_pSimDoc->GetValidResidualPointer()!=nullptr) {
						for(int i=1;i<MAXSIMVAL;i++) {
							void *pCF=m_pSimDoc->GetValidResidualPointer(i);
							if(pCF!=nullptr)
								test=TRUE;
						}
			}
			int ans;
			ans=MFC_Util::MessageBoxReturn("Old files will be permanetly reduced, please make a copy of current files if you want to be able to undo your operation.\n Do you want to continue ?","Removing tail of data from MBIN and XBIN file",MB_YESNO);
			if(ans!=IDYES) return -1;

			BeginWaitCursor();
			m_pSimDoc->m_MStorage.OpenFile(m_pSimDoc->GetMBinFileName(), TRUE);
			m_pSimDoc->m_MStorage.ReWriteExistingFile(m_pSimDoc->MR_Get_TotalNumberofRuns());
			m_pSimDoc->UnLockMultiStorageFile();
			CString pzPathName=m_pDoc->CDocument::GetPathName();
			m_pDoc->CDocument::OnSaveDocument(pzPathName);
			// Action for adding new information to MBIN or XBIN files
			int ifiles=m_pSimDoc->m_MStorage.GetNumberOfAddedFiles();
			CString strbase=CString(m_pSimDoc->m_MStorage.GetNameAddedFiles(ifiles-1).c_str());
			int ipos=strbase.Find(_T("_Multi_"));
			CString numbase=strbase.Mid(ipos+7,5);
			CString filename;
				
			if(m_pSimDoc->GetValidResidualPointer()!=nullptr) {
						for(int i=1;i<MAXSIMVAL;i++) {
							CResiduals *pRes;
							pRes=(CResiduals*)m_pSimDoc->GetValidResidualPointer(i+1);
							CString test=CString(m_pSimDoc->GetXBinFileName(i).c_str());
							if(pRes!=nullptr){
								pRes->ReWriteExistingFile(m_pSimDoc->MR_Get_TotalNumberofRuns());
							}
						}
			}
			string news=FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_TotalNumberofRuns());
			ans=MFC_Util::MessageBoxReturn("Old files is now updated to :"+ news+ " runs","Removing tail of data from MBIN and XBIN file",MB_YESNO);
		
			EndWaitCursor();
			EnableUpdate(FALSE); 
			SetSheetNumber(0);
			EnableUpdate(FALSE); 
			while(GetNumberRows()>0){
				//for(int i=0;i<4;i++)
				//	UnJoinCells(i,0);				
				DeleteRow(0);
			}
			EnableUpdate(TRUE); 
			return 0;
		}
		else if(item==3002) {			// New MultiStorage file
			string file=m_pSimDoc->m_MStorage.GetNewFileName();
			if(file!="") {
				m_pSimDoc->SetMBinFileName(file);
			//	m_pSimDoc->SetCurrentSubDirectory(m_pSimDoc->GetCurrentSubDirectory());
				EnableUpdate(FALSE); 
				SetSheetNumber(0);
				EnableUpdate(FALSE); 
				while(GetNumberRows()>0){
					//for(int i=0;i<4;i++)
					//	UnJoinCells(i,0);				
					DeleteRow(0);
				}
				EnableUpdate(TRUE); 
				return 0;
			}

		}
		else if(item==2080) {
				CHistoryList dlg(pBase,p_ModelInfo->GetTypeIndex("DataBase"),m_pSimDoc, index);	
				if(dlg.DoModal()==IDOK) {



				}

		}
		else if(item>1500) {


				CChaParDuringRun	Dlg(P_DB,m_pSimDoc);
				Dlg.DoModal(); 				
				SetSheetNumber(9,FALSE);
				return 9;
					
		}

	return -1;
}
int DocCommands::HistoryCommand(int col, int row, int item) {
	CUGCell cell;
	SimB *pBase=nullptr;

	SIMB P_DB;

	if(item==2090) {
		m_pSimDoc->m_History_Array.clear();
		return 11;
	}
	else if(item==2097) {
		int LocalIndex = -1;
		string group, Type, name;
		GetCell(2,row,&cell);
		group=cell.GetStdText();
		GetCell(1,row,&cell);
		Type=cell.GetStdText();
		GetCell(3,row,&cell);
		name=cell.GetStdText();
		if(name.find("(")!=string::npos) {
			auto ipos=name.find("(");
			auto iposend = name.find((")"));		
			if (iposend > ipos&&iposend!=string::npos) {
				string indexstring = name.substr(ipos + 1, iposend - ipos - 1);
				LocalIndex = FUtil::AtoInt(indexstring);
			}
			name = name.substr(0, ipos);

		}

		pBase=m_pSimDoc->GetPtr(Type,group,name);
		pBase = m_pSimDoc->GetPtrByName(name);



		if(pBase!=nullptr) {
			CHistoryList dlg(pBase,size_t(p_ModelInfo->GetTypeIndex(Type)),m_pSimDoc, LocalIndex);	
			if(dlg.DoModal()==IDOK) {
			}
		}
		else {
			int i = 9;

		}

		return 11;
	}
	else if(item>=2000&&item<3000) {
		Doc* pMySimDoc;
		vector<Doc*> ReadyDocuments=FUtil::GetReadyDoc(m_pSimDoc);
		
		ReadyDocuments.push_back(m_pSimDoc);
		

		if (item == 2090&&ReadyDocuments.size()>0) {
			for (size_t i = 0; i < ReadyDocuments.size(); i++) {
				pMySimDoc = ReadyDocuments[i];
				pMySimDoc->History_RemoveAll();				
			}

		}
		if (item == 2070) {
			m_pSimDoc->m_ViewAll = true;
			MFC_Util::WriteProfileInt("HistViewAll", 1);
			EnableUpdate(TRUE);
			OnSheetSetup(11);
		}
		else if (item >= 2072 && item<=2078) {
			string ID = "Hist"; 
			string IDnum; 
			m_pSimDoc->m_ViewAll = false;
			MFC_Util::WriteProfileInt("HistViewAll", 0);
			size_t typetochange = item - 2072;
			for (size_t i = 0; i < ReadyDocuments.size(); i++) {
				IDnum = FUtil::STD_ItoAscii(typetochange);
				string Id = ID + IDnum;
				pMySimDoc = ReadyDocuments[i];
				if (pMySimDoc->m_ToView[typetochange]) {
					pMySimDoc->m_ToView[typetochange] = false;
					MFC_Util::WriteProfileInt(Id, 0);
				}
				else {
					pMySimDoc->m_ToView[typetochange] = true;
					MFC_Util::WriteProfileInt(Id, 1);
				}
				pMySimDoc->UpdateListToView();
			}
			EnableUpdate(TRUE);
			OnSheetSetup(11);
			//SetSheetNumber(11, TRUE);
			

		}


		return 11;
	}
	int info_no=item-2070;

	pBase=(SimB*)m_pSimDoc->m_RunInfo[info_no].pBase;
	CHistoryList dlg(pBase,size_t(p_ModelInfo->GetTypeIndex("RunInfo")),m_pSimDoc, -1);	
	if(dlg.DoModal()==IDOK) {
	}


	return -1;
}

int DocCommands::SwitchCommand(int col, int row, int item) {
CUGCell cell;
			GetCell(0,row,&cell);
			string Group=cell.GetStdText();
			string Type="Switches";
			string Name;
			if(item==1000) {
				m_pSimDoc->SetOriginalValues(Type,"ALL");
				m_ShowAllSwitches=TRUE;
			}
			else if (item==1001) {
				m_pSimDoc->SetOriginalValues(Type,Group);
				m_ShowAllSwitches=TRUE;
			}
			else if (item==1002) {
				m_pSimDoc->RemoveOriginalValues(Type,"ALL");
			}
			else if (item==1003) {
				m_pSimDoc->RemoveOriginalValues(Type,Group);
			};


			if(item==2018) {

				GetCell(1,row, &cell);
				string Name=cell.GetStdText();

				Type="Switches";
				
				Sw *pSw=(Sw*)m_pSimDoc->GetPtr(Type, Group, Name);
				if(pSw!=nullptr) {
						int oldvalue=pSw->GetOldIntValue();
						pSw->SetIntValue(oldvalue);
						//pSw->EditApply();
						return 1;
				}
				

			}
			else if(item==2080) {
				GetCell(1,row,&cell);
				Name=cell.GetStdText();
				
				SimB *pCP=m_pSimDoc->GetPtr(Type, Group, Name);
				CHistoryList dlg(pCP, p_ModelInfo->GetTypeIndex(Type), m_pSimDoc);	
				if(dlg.DoModal()==IDOK) {



				}
			}

	return 1;
}
int DocCommands::ParameterCommand(int col, int row, int item) {
CUGCell cell;
			GetCell(0,row,&cell);
			string Group=cell.GetStdText();
			string Type;
			string Name;
			Type="Parameters";

			if(item==1000) {
				m_pSimDoc->SetOriginalValues(Type,"ALL");
				m_ShowAllParameters=TRUE;
			}
			else if (item==1001) {
				m_pSimDoc->SetOriginalValues(Type,Group);
				m_ShowAllParameters=TRUE;
			}
			else if (item==1002) {
				m_pSimDoc->RemoveOriginalValues(Type,"ALL");
			}
			else if (item==1003) {
				m_pSimDoc->RemoveOriginalValues(Type,Group);
			}
			else if(item==4001) { // Set chart number
				m_newChart=FALSE;;
			}
			else if(item==2001) {
				GetCell(2,row,&cell);
				Name=cell.GetStdText();
				SIMB PCP;
				PCP.pBase=m_pSimDoc->GetPtr(Type, Group, Name);
				PCP.TabIndex=-1;
				if(PCP.pBase!=nullptr) {
					CChaParDuringRun	Dlg(PCP,m_pSimDoc);
					 Dlg.DoModal(); 				
					SetSheetNumber(9,FALSE);
					return 9;
				}	
			}
			else if(item==2002) {
				GetCell(2,row,&cell);
				Name=cell.GetStdText();
				SIMB simb;
				simb.pBase=m_pSimDoc->GetPtr(Type, Group, Name);
				simb.TabIndex=-1;
				if(simb.pBase!=nullptr) {
					MultiRun Dlg(simb, m_pSimDoc);
					 Dlg.DoModal(); 				
					SetSheetNumber(10,FALSE);
					return 10;
					SetSheetNumber(2, FALSE);
				}	
			}
			else if(item==2018) {
				GetCell(2,row,&cell);
				Name=cell.GetStdText();
		
				Sw *pSw;
				pSw=(Sw*)m_pSimDoc->GetPtr(Type, Group, Name);
				size_t oldvalue=pSw->GetOldIntValue();
				pSw->SetIntValue(oldvalue);
				return 2;
			}
			else if(item==2020) {
				GetCell(2,row,&cell);
				Name=cell.GetStdText();
				SimB *pCP=m_pSimDoc->GetPtr(Type, Group, Name);
				Make_ParameterChart(pCP);
			}
			else if(item==2080) {
				GetCell(2,row,&cell);
				Name=cell.GetStdText();
				SimB *pSimB;
				pSimB=m_pSimDoc->GetPtr(Type, Group, Name);
				CHistoryList dlg(pSimB, p_ModelInfo->GetTypeIndex(Type), m_pSimDoc);	
				if(dlg.DoModal()==IDOK) {



				}
			};



	return 2;
}
int DocCommands::ParameterTableCommand(int col, int row, int item) {
	CUGCell cell;
	string Name, Group, Type;
	P* pCP;
	Tab *pTab;
				GetCell(col, row, &cell);
				long ParV=cell.GetParam();
				if (ParV==3000) {
					// Change of Element value

					GetCell(2,row,&cell);
					Name=cell.GetStdText();
					GetCell(0,row,&cell);
					Group=cell.GetStdText();
					Type="Tables";

					pCP=dynamic_cast<P*>(m_pSimDoc->GetPtr(Type, Group, Name));
					if(pCP!=nullptr) {
						CHistoryList dlg(pCP, p_ModelInfo->GetTypeIndex(Type), m_pSimDoc);	
						if(dlg.DoModal()==IDOK) {



						}
					}

				}
				else if(item==2050) {
					if(m_pSimDoc->m_HideAllTableContents) 
						m_pSimDoc->m_HideAllTableContents=false;
					else
						m_pSimDoc->m_HideAllTableContents=true;
					
					for (size_t i=0; i<m_pSimDoc->m_Tab_Array.size(); i++)	{
						Tab *pTab=m_pSimDoc->m_Tab_Array[i];
						if(m_pSimDoc->m_HideAllTableContents) 
							pTab->SetHide(FALSE);
						else
							pTab->SetHide(TRUE);
				
					}

					EnableUpdate(FALSE); 
					while(GetNumberRows()>0)
						DeleteRow(0);
					EnableUpdate(TRUE); 
					//OnSheetSetup(3);
					return 3;

				}
				else if (ParV>=2000) { // Plot Table function
					SIMB simb;
					BOOL fixedlegend=FALSE;
					int index=ParV-2000;

					GetCell(2,row-index-2,&cell);
					Name=cell.GetStdText();
					GetCell(0,row-index-2,&cell);
					Group=cell.GetStdText();
					Type="Tables";
			
					pTab=dynamic_cast<Tab*>(m_pSimDoc->GetPtr(Type, Group, Name));

					GetCell(col,row-index-1,&cell);
					Name=cell.GetStdText();
					GetCell(0,row-index-2,&cell);
					Group=cell.GetStdText();
					Type="Table Parameters";
					SimB *pSimB;
					pSimB=m_pSimDoc->GetPtr(Type, Group, Name);
					if(pSimB==nullptr) {
						Group="Plant";
						pSimB=m_pSimDoc->GetPtr(Type, Group, Name);
						if(pSimB==nullptr)
							pSimB=m_pSimDoc->GetP_Pointer(Name);
					}
					pCP=dynamic_cast<P*>(pSimB);
					simb.pBase=pCP;
					simb.TabIndex=index;
					int resetview=0;

					if(item==2001) {		// Change During Run
						if(pCP!=nullptr) {
							CChaParDuringRun	Dlg(simb, m_pSimDoc);
							Dlg.DoModal(); 				
							SetSheetNumber(9,FALSE);
							EnableUpdate(FALSE); 
							while(GetNumberRows()>0)
									DeleteRow(0);
							EnableUpdate(TRUE); 
							OnSheetSetup(9);
							SetSheetNumber(3, FALSE);
						}	
					}
					else if(item==2002) { // MultiRun
						if(pCP!=nullptr) {
							MultiRun Dlg(simb, m_pSimDoc);
							Dlg.DoModal(); 				
							SetSheetNumber(10,FALSE);
							EnableUpdate(FALSE); 
							while(GetNumberRows()>0)
									DeleteRow(0);
							EnableUpdate(TRUE); 
							OnSheetSetup(10);
							SetSheetNumber(3, FALSE);
						}	
					}
					else if(item>=2003&&item<2011) { //Make Chart first option

							if(Make_ParameterChart(pCP, pTab, index, item-2003))
								cell.SetBackColor(RGB(80,240,240));	
					}
	
					else if(item==2018) {	// Reset to previous value
						double oldvalue=pCP->GetOldValue(index);
						pCP->SetValue(index,oldvalue);
						//pCPt->EditApply(index);
						return 3;
					}
					else if(item==5001) { // Estimate New coefficients 
						if(pTab!=nullptr&&pCP!=nullptr) {
								//m_pSimDoc->SP_OpenDataBase();
							   // SP_Estimate Dlg(pTab, index+1, m_pDoc);
								//Dlg.DoModal(); 	
								SetSheetNumber(10,FALSE);
								EnableUpdate(FALSE); 
								while(GetNumberRows()>0)
									DeleteRow(0);
								EnableUpdate(TRUE); 
								OnSheetSetup(10);
								SetSheetNumber(3, FALSE);
								return 3;
						}
						
					}
					else if(item==5002) { // Retrieve from data base
						if(pTab!=nullptr&&pCP!=nullptr) {
	

	  							//SP_Retrieve dlg(TRUE, m_pDoc);
							 	CDB* pDB;
								pDB=m_pSimDoc->DB_GetPointer(DB_types(1));
								SP_Retrieve dlg(TRUE, m_pSimDoc, pDB, -99);
								if(dlg.DoModal()==IDOK) {
									int ivalue=MFC_Util::AtoInt(dlg.m_NumLayers);
									pTab->SetNumOfRow(ivalue);
									//m_pSimDoc->ReallocateTheWholeMap();
								}
								EnableUpdate(FALSE); 
								while(GetNumberRows()>0)
									DeleteRow(0);
								EnableUpdate(TRUE); 
								OnSheetSetup(3);
								SetSheetNumber(3, FALSE);
						}
						
					
						
					}
					else if(item==5003) { // Save to data base
						if(pTab!=nullptr&&pCP!=nullptr) {

							//	pDB=m_pSimDoc->DB_GetPointer(DB_types(1));
							//	SP_Retrieve dlg(TRUE, m_pDoc, pDB, -99);
								//if(name.GetLength()<=0) {
								/*	SP_Save Dlg(FALSE, m_pDoc);
									Dlg.m_NumLayers=pTab->GetValue();
									Dlg.DoModal(); */
								/*}
								else {
									SP_Save Dlg(FALSE, m_pDoc);
									Dlg.m_NumLayers=m_pCTab->GetValue();
									Dlg.DoModal(); 
								}*/
							    
						}
						
					}
					else if(item==5004) { // Save to data base
						if(pTab!=nullptr&&pCP!=nullptr) {
								//if(name.GetLength()<=0) {
								/*	SP_Save Dlg(TRUE, m_pDoc);
									if(Dlg.DoModal()==IDOK) {
										int ivalue=Dlg.m_NumLayers;
										pTab->SetNumOfRow(ivalue);
										//newprofile=TRUE;	
										//m_pSimDoc->ReallocateTheWholeMap();
										
									};*/

									EnableUpdate(FALSE); 
									while(GetNumberRows()>0)
									DeleteRow(0);
									EnableUpdate(TRUE); 
									OnSheetSetup(3);
									SetSheetNumber(3, FALSE);




								/*}
								else {
									SP_Save Dlg(FALSE, m_pDoc);
									Dlg.m_NumLayers=m_pCTab->GetValue();
									Dlg.DoModal(); 
								}*/
							    
						}
						
					}
					else if(item==2080) {
						CHistoryList dlg(pCP,p_ModelInfo->GetTypeIndex("Table Parameters"), m_pSimDoc, index);	
						if(dlg.DoModal()==IDOK) {



						}
					}
					;




			}
			GetCell(0,row,&cell);
			Group=cell.GetStdText();
			Type="Tables";
			if(item==1000) {
				m_pSimDoc->SetOriginalValues(Type,"ALL");
				m_ShowAllOutputs=TRUE;
			}
			else if (item==1001) {
				m_pSimDoc->SetOriginalValues(Type,Group);
				m_ShowAllOutputs=TRUE;
			}
			else if (item==1002) {
				m_pSimDoc->RemoveOriginalValues(Type,"ALL");
			}
			else if (item==1003) {
				m_pSimDoc->RemoveOriginalValues(Type,Group);
			};



	return -1;

}
int DocCommands::ModelFileCommand(int col, int row, int item) {
	CUGCell cell1, cell;
	string Name, Group, Type;
	SimB *pSw;
	int ParV;

				GetCell(1,row, &cell1);
				//GetCell(col-1,row, &cell1);
				Name=cell1.GetStdText();
				
				GetCell(0,row, &cell1);
				Group=cell1.GetStdText();
				Type="Files";
				
				pSw=m_pSimDoc->GetPtr(Type, Group, Name);

			if(pSw!=nullptr) {
				if(item==3000||item==3004) {
				//Find PG file
					int fileDlgResult;
					CString pszFileName ;
					LPCTSTR lpszDefExt=_T("bin");
					LPCTSTR lpszFilter;
		//The next line is only neccessary as long as the PGP does not handle long filenames!
		//PEJ Excluded info box
					CString szFilter = _T("PG Bin file (*.BIN)|*.BIN||");
					CFileDialog	BinFileDlg(TRUE,lpszDefExt,lpszFilter =_T("*.BIN"),OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,szFilter);
		//Show the dialog
					fileDlgResult=BinFileDlg.DoModal();
		//If the cancel button has been pushed then return

				if (fileDlgResult==IDOK) {
						pszFileName = BinFileDlg.GetPathName();

						CPG *pPG;
						pPG=(CPG*)((F*)pSw)->GetPointer();
						if(pPG==nullptr) {
							pPG= new CPG;
							((F*)pSw)->SetPointer(pPG);
							pPG->Open(CT2A(pszFileName).m_psz);
							pPG->CloseFile();
						}
						else  {
							pPG->CloseFile();
							pPG->Reset();
							pPG->Open(CT2A(pszFileName).m_psz);
							pPG->CloseFile();
						}


						//CCreatePG dlg(m_pDoc, pSw);
						//int ans=dlg.DoModal();
						//if(ans==IDOK) {
						((F*)pSw)->SetValue(CT2A(pszFileName.MakeUpper()).m_psz);
						//pSw->Apply();
						if(((F*)pSw)->CheckFileNameAndAssignNameToPGClass()){ 
							m_pSimDoc->TimeModified();
							m_pSimDoc->History_Add(pSw,-1,time(nullptr),CT2A(pszFileName).m_psz);
							if (m_pSimDoc->GetPtr(PGFILE, "Meteorological data") == ((F*)pSw)) {
								m_pSimDoc->SetSimPeriodFromClimateFile();


							}
						}
						//}
					}

				}


			}
			else if (item == 3010) {
				GetCell(col, row, &cell);
				ParV = cell.GetParam();
				size_t diff = ParV - 2200;
				m_pSimDoc->m_ValsumFlexFiles.erase(m_pSimDoc->m_ValsumFlexFiles.begin()+diff);


			}
			else if(item==1150) {
				if (Group == "FertTable File") {

					FCSV *pCSV = dynamic_cast<FCSV*>(m_pSimDoc->GetPtr(CSVFILE, "FertTable File"));
					if (pCSV != nullptr) {
						FILEDLG FileDlg = MFC_Util::GetFileDialog("*.csv", "", "Comma separated file (*.csv)|*.csv|");
						if (FileDlg.opengood != IDOK) {
							return false;
						}
						string filename = FileDlg.filename;
						pCSV->SetValue(filename);
						if (!pCSV->CheckFileName(m_pSimDoc))	return -1;
					}
				}
				else if (Group == "MASCUR File") {

					FCSV *pCSV = dynamic_cast<FCSV*>(m_pSimDoc->GetPtr(CSVFILE, "MASCUR File"));
					if (pCSV != nullptr) {
						FILEDLG FileDlg = MFC_Util::GetFileDialog("*.csv", "", "Comma separated file (*.csv)|*.csv|");
						if (FileDlg.opengood != IDOK) {
							return false;
						}
						string filename = FileDlg.filename;
						pCSV->SetValue(filename);
						if (pCSV->CheckFileName()) {
								if (!pGetSoil->IsOpen()) {
								pGetSoil->Init(filename);
							}
						}
						else
							return false;
					}
				}
				else if (Group.find("ParCalibrationFile") == 0) {

					FCSV *pCSV = dynamic_cast<FCSV*>(m_pSimDoc->GetPtr(CSVFILE, Group));
					if (pCSV != nullptr) {
						FILEDLG FileDlg = MFC_Util::GetFileDialog("*.csv", "", "Comma separated file (*.csv)|*.csv|");
						if (FileDlg.opengood != IDOK) {
							return false;
						}
						string filename = FileDlg.filename;
						pCSV->SetValue(filename);
						if (!pCSV->CheckFileName(m_pSimDoc))	return -1;
					}
				}
				return 4;
			}
			else if (item == 1151) {
				if (Group == "FertTable File") {
					FCSV *pCSV = dynamic_cast<FCSV*>(m_pSimDoc->GetPtr(CSVFILE, "FertTable File"));
					size_t index=0;
					while (m_pSimDoc->m_FCSV_Array.size() > index) {
						if (pCSV == m_pSimDoc->m_FCSV_Array[index]) {
							m_pSimDoc->m_FCSV_Array.erase(m_pSimDoc->m_FCSV_Array.begin() + index);
						}
						index++;

					}

				}
				else if (Group == "MASCUR File") {

					FCSV *pCSV = dynamic_cast<FCSV*>(m_pSimDoc->GetPtr(CSVFILE, "MASCUR File"));
					size_t index=0;
					while (m_pSimDoc->m_FCSV_Array.size() > index) {
						if (pCSV == m_pSimDoc->m_FCSV_Array[index]) {
							m_pSimDoc->m_FCSV_Array.erase(m_pSimDoc->m_FCSV_Array.begin() + index);
						}
						index++;

					}
	
				}
				else if (Group.find("ParCalibration")==0) {

					FCSV *pCSV = dynamic_cast<FCSV*>(m_pSimDoc->GetPtr(CSVFILE, Group));
					size_t index = 0;
					while (m_pSimDoc->m_FCSV_Array.size() > index) {
						if (pCSV == m_pSimDoc->m_FCSV_Array[index]) {
							m_pSimDoc->m_FCSV_Array.erase(m_pSimDoc->m_FCSV_Array.begin() + index);
						}
						index++;

					}

				}
				return 4;





			}
			if(item==4011) {
					ManagePlotWindows(item, 4);
					//CLEAR selected outputs
					//CPG *pCPG=m_pSimDoc->m_SelectModelFile[0].pPG;
					//pCPG->SetPicked(j+1)
				//	m_pSimDoc->m_SelectModelFile.clear();
			}


			EnableUpdate(FALSE); 
			while(GetNumberRows()>0)
				DeleteRow(0);
			EnableUpdate(TRUE); 
			OnSheetSetup(4);

			if(item>4000&&item<4006) {
	
				string Group, Name, Type;
				int Index=0;
			
				GetCell(-1,row,&cell);
				Index=int(cell.GetNumber());
				GetCell(1,row-Index-1, &cell1);
				Name=cell1.GetStdText();
				GetCell(0,row-Index-1, &cell1);
				Group=cell1.GetStdText();
				pSw=m_pSimDoc->GetPtr(Type, Group, Name);
				if(pSw!=nullptr) { 
					TimeSerieChartClick(0, row, 4, pSw, Index, 0, "Files");
				}


			}


	return -1;
}
int DocCommands::OutPutVarCommand(int col, int row, int item) {
	CUGCell cell;
	string Group, Name, Type;
	SimB *pSw;

	if (!m_pSimDoc->IsFinishedSimulation()) {
		if (item == 1080) {  // Deleselect all items
			m_pSimDoc->RemoveAllSelectedOutputs();
			return 5;
		}
		else if (item == 1081) {

			size_t selnum=m_pSimDoc->SelectAllOutputs();
			return 5;
		}
		else if (item == 1090) {   // Save to Txt File
			auto outfilename = MFC_Util::GetCSVFileName();
			fstream outfilestream;

			if (FUtil::IsFileExisting(outfilename))
				outfilestream.open(outfilename, ios::out | ios::in|ios::binary);
			else
				outfilestream.open(outfilename, ios::out | ios::binary);

			string utstr;
			utstr = "VariableType; Group; Name; OutPut Flag (1= for Single Run,2= for Multi Run)";
			string linebreak = "\r\n";
			utstr += linebreak;

			outfilestream.write((char*)&utstr[0], utstr.size());
			vector<SIMB> pvv;
			pvv = m_pSimDoc->GetAllOutputs();
			pvv = FUtil::SortSimB_ByGroup(pvv);
			size_t ivv = pvv.size();
			OutVector *pVector;
			OutSingle *pSingle;
			size_t storeflag;
			for (size_t i = 0; i < pvv.size(); i++) {
				pVector = dynamic_cast<OutVector*>(pvv[i].pBase);
				utstr.clear();
				if (pVector != nullptr) {
					auto lin = pvv[i].TabIndex;
					storeflag = pVector->GetStoreFlag(lin);
					utstr += pVector->GetTypeName();
					utstr += ";";
					utstr += pVector->GetGroup();
					utstr += ";";
					utstr += pVector->GetName();
					utstr += "(";
					utstr += FUtil::STD_ItoAscii(lin+1); utstr += ")";
					utstr += ";";
				}
				else {
					pSingle = dynamic_cast<OutSingle*>(pvv[i].pBase);
					if (pSingle != nullptr) {
						utstr += pSingle->GetTypeName();
						utstr += ";";
						utstr += pSingle->GetGroup();
						utstr += ";";
						utstr += pSingle->GetName();
						utstr += ";";
						storeflag = pSingle->GetStoreFlag();
					}
				}
				utstr += FUtil::STD_ItoAscii(storeflag);
				utstr += linebreak;
				outfilestream.write((char*)&utstr[0], utstr.size());

			}
			outfilestream.close();
		}
		else if (item == 1091) {    // Retrieve from txt File
			auto infilename = MFC_Util::GetCSVFileName();
			ifstream infilestream;

			if (FUtil::IsFileExisting(infilename))
				infilestream.open(infilename,ios::in );

			string line;
			line.resize(2000);
			infilestream.getline(&line[0], 2000);
			size_t nc;
			nc= infilestream.gcount();
			line.resize(nc);
			line.resize(2000);
			size_t count = 0;
			while (infilestream.good()) {

				infilestream.getline(&line[0], 2000);
				nc = infilestream.gcount();
				line.resize(nc);
				bool setmade=FUtil::SetOutPutFlag_FromLineInput(line, m_pSimDoc);
				if (setmade) count++;
				line.resize(2000);
			}
			infilestream.close();
			return 5;

		}
					else if(item>=1050&&item<1100) {
							GetCell(1,row,&cell);
							Group=cell.GetStdText();
							GetCell(3,row,&cell);
							Name=cell.GetStdLabelText();
							int pos1=Name.rfind('(');
							int pos2=Name.rfind(')');
							int Index;

							if (pos2>pos1&&pos2==Name.size()-1) {
								string t=Name.substr(pos1+1,pos2-pos1-1);
								Index=FUtil::AtoInt(t);
								if(Index>0)
									Name=Name.substr(0,pos1);
								else
									Index=1;
								
							}
							else
								Index=1;
							GetCell(0,row, &cell);
							string Type;
							Type=cell.GetStdText();

							SimB *pSw=m_pSimDoc->GetPtr(Type, Group, Name);
							OutVector *pVec=dynamic_cast<OutVector*>(pSw);
							if(pVec!=nullptr&&(item==1060||item==1061)) {
								if(item==1060)
									pVec->SetAllStoreFlags(1);
								if(item==1061)
									pVec->SetAllStoreFlags(0);

							}
							SimB *SwUp=pSw;
							int rowst;
							rowst=row;
							string GroupTest,NameTest,TypeTest;
							int IndexTest;
							bool up=TRUE;
							bool cont=FALSE;
							while (SwUp!=nullptr||cont) {
								if(up)
									rowst--;
								else
									rowst++;
								GetCell(1,rowst,&cell);
								GroupTest=cell.GetStdText();
								GetCell(3,rowst,&cell);
								NameTest=cell.GetStdLabelText();
								pos1=NameTest.rfind('(');
								pos2=NameTest.rfind(')');

								if (pos2>pos1&&pos2==NameTest.size()-1) {
									string t=NameTest.substr(pos1+1,pos2-pos1-1);

									IndexTest=FUtil::AtoInt(t);
									if(IndexTest>0)
										NameTest=NameTest.substr(0,pos1);
									else
										IndexTest=1;
								}
								else
									IndexTest=1;
								GetCell(0,rowst, &cell);
								TypeTest=cell.GetStdText();
								
								if((item==1050||item==1051||item==1052||item==1053)&&GroupTest==Group) {
									SwUp=m_pSimDoc->GetPtr(TypeTest, GroupTest, NameTest);
									cont=FALSE;
								}
								else if((item==1060||item==1061)&&Name==NameTest) {
									SwUp=m_pSimDoc->GetPtr(TypeTest, GroupTest, NameTest);
									cont=FALSE;
									if(IndexTest==1) {
										up=FALSE;


									}
								}
								else if(rowst<GetNumberRows()-1&&rowst>=0&&(item==1052||item==1053)){
									cont=TRUE;
								}
								else {
									SwUp=nullptr;
									cont=FALSE;
								}
								if(SwUp==nullptr&&up&&item<1052) {
									up=FALSE;
									SwUp=pSw;
								}
								else if(up&&rowst==0) {
									up=FALSE;
									rowst--;

								}
								else if(!up&&SwUp!=nullptr) {
									
									int selected;
									if(item==1050||item==1060||item==1052)
										selected=1;
									else
										selected=0;
									if(cont&&SwUp==nullptr) {


									}
									else {
										int ityp=p_ModelInfo->GetTypeIndex(Type);
										int newflagvalue=0;
										if(item%2==0) newflagvalue=1; 
										if(SwUp->Is_Vector()) {
											selected=((OutVector*)SwUp)->GetStoreFlag(IndexTest-1);
											((OutVector*)SwUp)->SetStoreFlag(IndexTest-1,newflagvalue);
											selected=newflagvalue;
										}
										else {
											selected=((OutSingle*)SwUp)->GetStoreFlag();
											((OutSingle*)SwUp)->SetStoreFlag(newflagvalue);
											selected=newflagvalue;

										}
										m_pSimDoc->TimeModified();
										
										GetCell(3,rowst, &cell);
										if(selected==0) 
											cell.SetBackColor(RGB(255,255,255));
										else 
											cell.SetBackColor(RGB(40,240,240));
										SetCell(3,rowst, &cell);
										RedrawCell(3,rowst);
									}
								}

							}
							return 5;



					}
					else if(item==1000) {
							m_pSimDoc->SetOriginalValues(Type,"ALL");
							m_ShowAllOutputs=TRUE;
							return 5;
					}
					else if (item==1001) {
							GetCell(1,row,&cell);
							Group=cell.GetStdText();

							m_ShowAllOutputs=FALSE;
							m_pSimDoc->RemoveOriginalValues("Outputs",Group,false, m_ViewCategory);
							size_t iv=p_ModelInfo->GetGroupIndex(Group);

							m_pSimDoc->SetGroupEnable(iv, true);
							m_CurrentTopRow=GetTopRow();
							return 5;



							//m_pSimDoc->SetOriginalValues(Type,Group);
							//m_ShowAllOutputs=TRUE;
					}
					else if (item==1002) {
							m_pSimDoc->RemoveOriginalValues(Type,"ALL");
							return 5;
					}
					else if (item==1003) {
							
							GetCell(1,row,&cell);
							Group=cell.GetStdText();							
							m_ShowAllOutputs=FALSE;
							m_pSimDoc->RemoveOriginalValues("Outputs",Group,false, m_ViewCategory);
							size_t iv=p_ModelInfo->GetGroupIndex(Group);

							m_pSimDoc->SetGroupEnable(iv, false);

							m_CurrentTopRow=GetTopRow();
							
							return 5;
						}
	}
	else if (m_pSimDoc->IsFinishedSimulation() && (item == 4004 || item == 4005)) {
		string outfile;
		if (item == 4004)
			outfile = m_pSimDoc->Export_XLSX(false, ExportOptions::Outputs);
		else
			outfile = m_pSimDoc->Export_XLSX(true, ExportOptions::Outputs);
		string message = "Data Exported to file : " + outfile;
		MFC_Util::MessageBox(message);
	}
	else if(col>=6) {
		SIMB simb=GetMultiSumPointer(row);
		auto Index=GetMultiSumIndex(simb,col);
		auto value=m_pSimDoc->m_MStorage.GetSumV(0,Index-6);
		VALSUM out;
		out.simb=simb;
		out.SumIndex=Index;
		out.ValueOpt=value;
		out.ValueMax=value+abs(value)*0.5;
		out.ValueMin=value-abs(value)*0.5;
		m_pSimDoc->m_MStorage.m_ValSumValues_Stored.push_back(out);

		
	}
	else {


		GetCell(col,row, &cell);
		Name=cell.GetStdLabelText();
		int ParV=cell.GetParam();
		size_t pos1=Name.rfind('(');
		size_t pos2=Name.rfind(')');
		//ReverseFind(

		int PGindex, index;
		if (pos2>pos1&&pos2==Name.size()-1) {
				string t=Name.substr(pos1+1,pos2-pos1-1);
				index=FUtil::AtoInt(t);
				if(index>0)
					Name=Name.substr(0,pos1);
				else
					index=1;
		}
		else
			index=1;
						
     	GetCell(col-1,row, &cell);
		Group=cell.GetStdText();
		GetCell(col-2,row, &cell);
		Type=cell.GetStdText();
						
		pSw=m_pSimDoc->GetPtr(Type, Group, Name);
		if(pSw==nullptr) return -1;
		int ityp=p_ModelInfo->GetTypeIndex(Type);
		if(pSw->Is_Vector())
			PGindex=((OutVector*)pSw)->GetPgFileIndex(index-1);
		else
			PGindex=((OutSingle*)pSw)->GetPgFileIndex();
		if(item==5997) {
			if(m_pSimDoc->m_PlotStorage.CPG_SetPGSimFile()) {
				CPG *pPG;
				pPG=m_pSimDoc->m_PlotStorage.CPG_GetSimFile();
				//pPG->AddAllSelections();
				pPG->AddSelection(PGindex);
				EnableUpdate(FALSE); 
				while(GetNumberRows()>0)
					DeleteRow(0);
				EnableUpdate(TRUE); 
				return 5;
			}

		}
		else if(item==5001||item==5002) {
			// 5001 All, 5002 Selected
			SelectOUT out;
			out.pBase=pSw;
			out.LIndex=index;
			out.row=row;
			out.IType=ityp-3;
			if(m_pSimDoc->m_SelectOut.size()==0)
					m_CurrentSubChart=1;
			else if(m_CurrentSubChart<m_pProp->m_NumSubCharts)
					m_CurrentSubChart++;
			out.subchart=m_CurrentSubChart;
			if(item==5001)
				out.MeanIndicator= ENSEMBLE_TYPE::ALL;
			else
				out.MeanIndicator= ENSEMBLE_TYPE::ACCEPTED;

			m_pSimDoc->m_SelectOut.push_back(out);
		    TimeSerieChartClick(col, row, 5, pSw, index, 0, Type);

		}

	}



	return -1;
}
int DocCommands::ValidationVarCommand(int col, int row, int item) {
	CUGCell cell, cell1;
	Sw *pSw;
	if(!m_pSimDoc->IsFinishedSimulation()) {
		F *pCF;
			int m_NumSelected;
			GetCell(0,row,&cell);
			int m_ItemSelected=int(cell.GetNumber())-1;

			m_NumSelected=0;
			for(size_t i=0;i<MAXSIMVAL;i++) {
				pCF=m_pSimDoc->ValidationFilePointer(i+1);
				m_NumSelected+=pCF->GetNumberOfSelectedVal();
			}

			if(item==3002) {

		//Remove File;
				GetCell(col,row, &cell1);
				int numfile=int(cell1.GetNumber());
				m_pSimDoc->m_ValidationData.RemoveValFile(numfile-1);
				EnableUpdate(FALSE); 
				while(GetNumberRows()>0)
					DeleteRow(0);
				EnableUpdate(TRUE); 
				m_pSimDoc->RemoveOriginalValues("Files","ALL", true);
				OnSheetSetup(6);

			}
			else if(item==6000) {
				// Select file and enable validation
				OnAddValFile();
			}
			else if (item == 6061) {
			
				OnAddTsvVal(true);
			}
			else if(item==6010) {
				//Disable validation
					for(size_t ifile=0; ifile<MAXSIMVAL;ifile++) {	
						pCF=m_pSimDoc->ValidationFilePointer(ifile+1);
						for(int i=0; i<pCF->GetNumberOfVariables();i++) 
							pCF->SetValNumber(i, -1);	
						pCF->SetNumberOfSelectedVal(0);
						pCF->m_Exist=FALSE;
					}
					m_pSimDoc->m_ValidationData.CleanValList();							

					pSw=dynamic_cast<Sw*>(m_pSimDoc->GetPtr(SWITCH, "ValidationMode"));
					//pSw=(SimB*)m_pDoc->GetPtr(name, Type, grp);
					pSw->SetStrValue("off");
					//pSw->Apply();
					m_pSimDoc->SetOriginalValues("Files","ALL");
				return 4;
							
			}
			else if(item==6001) {
				// New Item
					return OnAddTsvVal(false); 
			}
			else if (item == 6061) {
				// New Item
				return OnAddTsvVal();
			}
			else if(item==6002) {
				GetCell(1,row,&cell);
				int typ=cell.GetCellType();
				if(typ==UGCT_CHECKBOX) {


					m_pSimDoc->m_ValidationData.RemoveValSumVarNode(m_ItemSelected);

				}
				else {
					m_pSimDoc->m_ValidationData.RemoveValNode(m_ItemSelected);
				}
						

			}
			else if(item==6011) {
				return OnAddSvVal();
			}
			else if (item == 6020) {
				VALSUMLINK newlink=OnAddSvFlexFiles();
				if (newlink.FileName.size() > 0) {
					bool koll= m_pSimDoc->CreateMapSvFlexFile();
					m_pSimDoc->m_IsPerformanceSmartFlexibleFile_Read = false;
					if(koll) m_pSimDoc->UpdateValidationPerformance_SmartFlexVariables();


				}



			}
			else if (item == 6021) {
				return OnAddSvValFromFile();
			}


		//	UpdateSheet(4);
			//UpdateSheet(6);
			return 6;
	}
	else { //Finnished Simulation
		string Name, Group, Type;
		int index;
		vector<Doc*> ReadyDocuments=FUtil::GetReadyDoc(m_pSimDoc);
		if (m_pSimDoc->IsFinishedSimulation() && (item == 4006 || item == 4007)) {
			string outfile;
			if (item == 4006)
				outfile = m_pSimDoc->Export_XLSX(false, ExportOptions::Performance);
			else
				outfile = m_pSimDoc->Export_XLSX(true, ExportOptions::Performance);
			string message = "Data Exported to file : " + outfile;
			MFC_Util::MessageBox(message);
		}


		if(item>=2050&&item-2050<ReadyDocuments.size()) {
			Doc* pMySimDoc;
			pMySimDoc=ReadyDocuments[item-2050];
			pMySimDoc->m_Val_Array.clear();
			pMySimDoc->m_Val_Array=m_pSimDoc->m_Val_Array;
		}
		else if(m_pSimDoc->IsMultiSimulation()&&!m_ShowValidRes) {
				GetCell(3,row, &cell);
				if(m_pSimDoc->IsFinishedSimulation())
					Name=cell.GetStdLabelText();
				else
					Name=cell.GetStdLabelText();
				int ParV=cell.GetParam();
				size_t pos1=Name.rfind('(');
				size_t pos2=Name.rfind(')');
				int PGindex;
				int selected;
				selected=0;
				if (pos2>pos1&&pos2==Name.size()-1) {
						string t=Name.substr(pos1+1,pos2-pos1-1);
						index=FUtil::AtoInt(t);
						if(index>0)
							Name=Name.substr(0,pos1);
						else
							index=1;
				}
				else
					index=1;
						
     			GetCell(2,row, &cell1);
				Group=cell1.GetStdText();

						
				SimB *pSw=m_pSimDoc->GetPtrByName(Name, Group);

				if(pSw!=nullptr) {
					int ityp=p_ModelInfo->GetTypeIndex(Type);
					if(pSw->Is_Vector()) {
						PGindex=((OutVector*)pSw)->GetPgFileIndex(index-1);
						selected=((OutVector*)pSw)->GetStoreFlag(index-1); 
					}
					else {
						PGindex=((OutSingle*)pSw)->GetPgFileIndex();
						selected=((OutSingle*)pSw)->GetStoreFlag(); 
					}
				}

				//if(selected==2&&col<5) item=item+2;
				bool AllinEnsemble=false;
				if(item==5998) 
					m_pSimDoc->m_MStorage.SelSumV_Index_Reset();
				else if(item==5001||item==5003) {
				// Plot Mean
					int ival;
					ival=m_pSimDoc->m_ValidationData.GetValFileNumber(row);
					AllinEnsemble=true;

					//if(!m_OutAllMulti_Defined[ival]){
						BeginWaitCursor();
						CalculateMeanOffTimeSeries(item, ival);
							
						EndWaitCursor();
						return -1;
					//}
				}
				else if(item==5002||item==5004) {
					int ival;
					ival=m_pSimDoc->m_ValidationData.GetValFileNumber(row);
	
					BeginWaitCursor();
					CalculateMeanOffTimeSeries(item, ival);
					EndWaitCursor();
					return -1;

				}
				else if(item==1050) {
					GetCell(1,row,&cell);
					long num=int(cell.GetNumber());
					m_pSimDoc->OpenSimDocument(num, false);
				}
			
				if(item>=5001&&item<=5004) {

					return ValidationVarClick(4, row,AllinEnsemble );
					/*// Make Charts
					if(item>5002) {
						//item=item-2;
						//GetCell(-1,row,&cell);
						//index=int(cell.GetNumber());							
					}
					TimeSerieChartClick(col, row, 6, pSw, index, item-5000, Type.c_str());*/
					//return 6;
				}
				else if(item==5010) {
					// Independent Variable settings

					m_pSimDoc->m_PlotStorage.SetValidIndepPGVar(true);
					TimeSerieChartClick(col, row, 6);//, pSw, index, 0, Type.c_str());
				

				}
				else if(item==5011) {
					m_pSimDoc->m_PlotStorage.SetValidIndepPGVar(false);
					TimeSerieChartClick(col, row, 6);
					// Independent Variable settings



				}
				else if (item == 6020) {
					VALSUMLINK newlink = OnAddSvFlexFiles();
					if (newlink.FileName.size() > 0) {
						bool koll = m_pSimDoc->CreateMapSvFlexFile(m_pSimDoc->m_ValsumFlexFiles.size()-1);
						if (koll) {
							MFC_Util::MessageBoxReturn("New File is defined - Please add also variables from file", "New Flexible Summary file", MB_OK);
							OnAddSvValFromFile();
							ReEvaluateSvVal();
							return 6;

						}
					}
				}
				else if (item == 6071) {
					ChartUtil *pUtil;
					pUtil = MySingle::instance().GetChartUtilPointer();
					if (pUtil->m_AutoShiftFirstIndep)
						pUtil->m_AutoShiftFirstIndep = false;
					else
						pUtil->m_AutoShiftFirstIndep = true;

				}
				else if (item == 6040||item==6041||item==6042) {
					size_t indexstart, indexend;
					if (item == 6040||item==6042) {
						int nselV = m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
						int index = row - nselV;
						if (index < 0 && index >= m_pSimDoc->m_ValSum_Array.size()) return -1;
						indexstart = index; indexend = index + 1;

					}
					else {
						indexstart = 0;
						indexend = m_pSimDoc->m_ValSum_Array.size();

					}

					for (size_t index = indexstart; index < indexend; index++) {
						if (item = 6041) {
							m_pSimDoc->m_ValSum_Array[index].ObsScaling++;
							m_pSimDoc->m_ValSum_Array[index].ObsScaling = m_pSimDoc->m_ValSum_Array[index].ObsScaling%3;
						}
						else {
							if (m_pSimDoc->m_ValSum_Array[index].ObsScaling == 1) {
								if (item == 6040)
									m_pSimDoc->m_ValSum_Array[index].ObsScaling = 0;
								else if (item == 6042)
									m_pSimDoc->m_ValSum_Array[index].ObsScaling = 2;
							}
							else if (m_pSimDoc->m_ValSum_Array[index].ObsScaling == 2) {
								if (item == 6040)
									m_pSimDoc->m_ValSum_Array[index].ObsScaling = 1;
								else if (item == 6042)
									m_pSimDoc->m_ValSum_Array[index].ObsScaling = 0;
							}
							else {
								if (item == 6040)
									m_pSimDoc->m_ValSum_Array[index].ObsScaling = 1;
								else if (item == 6042)
									m_pSimDoc->m_ValSum_Array[index].ObsScaling = 2;
							}
						}
					}
					ReEvaluateSvVal();
					return 6;

				}
				else if (item == 6021) {
					OnAddSvValFromFile();
					ReEvaluateSvVal();
					return 6;
				}
				else if (item == 6022) {
					ReEvaluateSvVal();
					return 6;
				}


		}
		else if(m_pSimDoc->IsMultiSimulation()&&m_ShowValidRes) {

			if(item==1050) {
				GetCell(2,row,&cell);
				long num=int(cell.GetNumber());
				m_pSimDoc->OpenSimDocument(num, false);
			}

			else if(item>=1060&&item<=1061) {
					if(item==1061)
						m_pSimDoc->m_MStorage.UpdateAcceptedRuns(TRUE);
					else
						m_pSimDoc->m_MStorage.UpdateAcceptedRuns(FALSE);
					EnableUpdate(FALSE); 
					while(GetNumberRows()>0)
							DeleteRow(0);
					EnableUpdate(TRUE);
					m_ShowValidRes_Changed=FALSE;
					OnSheetSetup(6);
					return 6;
			}
			else if(item==1070) {


			}

		}
	}
	return -1;
}
int DocCommands::ChartsCommand(int col, int row, int item) {
	CUGCell cell;
			bool Changed=FALSE;
			GetCell(0, row, &cell);
			if(!m_ShowChartTable) {
			int ichart=int(cell.GetNumber());
			auto ichartType=m_pSimDoc->m_PlotStorage.GetChartType(row);
			if(item==1100) {
//Delete chart

				if(m_pSimDoc->m_PlotStorage.RemoveChart(ichart)) {
					Changed=TRUE;
					DeleteChart(ichart);
				}
			}
			else if(item==1101) {

				if(m_pSimDoc->m_PlotStorage.Remove(row)) {
					Changed=TRUE;
					UpdateChart(ichart);
				}

			}
			else if(item==1102) {
				ichartType=m_pSimDoc->m_PlotStorage.GetChartType(row);
				if(ichartType== CHART_TYPES::TIME_SERIE)
					ichartType= CHART_TYPES::SCATTER;
				else
					ichartType= CHART_TYPES::TIME_SERIE;
				if(m_pSimDoc->m_PlotStorage.SetChartType(ichart, ichartType))				{
					Changed=TRUE;
					UpdateChart(ichart, ichartType);
				}
			}
			else if(item==1104) {
				GetCell(1, row, &cell);
				int ivar=int(cell.GetNumber());
				CColourPickerDlg Dlgcol(MFC_Util::GetColorLine(ivar));
				if(Dlgcol.DoModal()==IDOK) {
					COLORREF iv=Dlgcol.GetColour();	
					MFC_Util::SetColorLine(ivar,iv);
					cell.SetBackColor(iv);
					SetCell(1,row,&cell);
					RedrawCell(1,row);
				
					
				}
				
			}
			else if(item==1105) { // Draw Chart

				if(!UpdateChart(ichart, ichartType)){

					auto idataType=m_pSimDoc->m_PlotStorage.GetDataType(row);
					BOOL koll=CreateChart(ichartType, ichart, idataType);

				}

			}
			else if(item==1106) { // Show Table
				Changed=TRUE;
				m_ShowChartTableNo=ichart;
				m_ShowChartTable=TRUE;

			}
			else if(item==4000) {
				m_pSimDoc->m_PlotStorage.RemoveAll();
				m_VarNo=0;
				Changed=TRUE;

				m_pSimDoc->m_PlotStorage.CPG_Reset();
				m_CurrentChart=1;

			}
			}
			else if(item==5997) {
				SelectRange ( 0, -4, GetNumberCols(), GetNumberRows()-1 );

			}
			if(Changed) {
// Update Chart Sheet
				EnableUpdate(FALSE); 
				while(GetNumberRows()>0)
					DeleteRow(0);
				EnableUpdate(TRUE); 
				OnSheetSetup(8);
			}

	return 8;
}
int DocCommands::ChaParCommand(int col, int row, int item){
	CUGCell cell;
	string Name, Group, Type;
	SimB *pSw;
	int index;
	GetCell(0,row,&cell);
	Group=cell.GetStdText();
	GetCell(1,row,&cell);
	Name=cell.GetStdText();
	size_t pos1=Name.rfind('(');
	size_t pos2=Name.rfind(')');
	if(Group=="DataBase") {
		SIMB pDB;
		Name=cell.GetStdText();
		pDB.pBase=m_pSimDoc->DB_GetPointer(Name);
		pDB.TabIndex=-1;
		// No action for the moment Need create
		// editfunction for pDB
		if (pDB.pBase->Is_DB()) { // Set position to change from DB Object
			CChaParDuringRun	Dlg(pDB, m_pSimDoc);
			Dlg.DoModal();
			SetSheetNumber(9, FALSE);
		}
		return 9;

	}
	else {
		if (pos2>pos1&&pos2==Name.size()-1) {
			string t=Name.substr(pos1+1,pos2-pos1-1);
						
			index=FUtil::AtoInt(t);
			if(index>0)
				Name=Name.substr(0,pos1);
			else
				index=1;
			Type="Table Parameters";
		}
		else {
			index=-1;
			Type="Parameters";
		}
						
		pSw=m_pSimDoc->GetPtr( Type, Group, Name);
		if(pSw==nullptr) {
			Group="Plant";
			pSw=m_pSimDoc->GetPtr(Type, Group, Name);
		}
		if(item==1050) {
			SIMB simb;
			simb.pBase=pSw;
			simb.TabIndex=index;

			if(pSw!=nullptr) {
				if(index==-1) {
					CChaParDuringRun	Dlg(simb,m_pSimDoc);
					Dlg.DoModal(); 				
					SetSheetNumber(9,FALSE);
				}
				else {
					CChaParDuringRun Dlg(simb, m_pSimDoc);
					Dlg.DoModal(); 				
					SetSheetNumber(9,FALSE);
				}
				return 9;

			}


		}
	}
	
	
	return -1;
}
int DocCommands::MultiRunCommand(int col, int row, int item) {
	CUGCell cell;
	string Name, Group, Type;

	if(item==1000) { // Apply start values

		SimB *pPar;
		int nTabIndex;
		float v;
		int icount;
		icount=-1;
				
		for(int i=0;i<m_pSimDoc->m_MultiRun_Array.size();i++){
		   	for(int ii=0; ii<m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i+1);ii++){
				if(m_pSimDoc->MR_GetMethod(i+1,ii+1)==9||m_pSimDoc->MR_GetMethod(i+1,ii+1)==11) {
					icount++;
					pPar=m_pSimDoc->MR_Get_pPar(i+1,ii+1);
					nTabIndex=m_pSimDoc->MR_GetTabIndex(i+1,ii+1);
					if(m_pSimDoc->MR_UseBayesianCoVar) {
							v=m_pSimDoc->MR_Get_BayesMean(icount); 
					}
					if(nTabIndex<0) {	
						if(!m_pSimDoc->MR_UseBayesianCoVar) {
							v=pPar->GetValue();
						}
						((Ps*)pPar)->MR_Set_Start(v);
						if(v<m_pSimDoc->MR_GetMin(i+1,ii+1)) ((Ps*)pPar)->MR_Set_Min(v-0.1*v);
						if(v>m_pSimDoc->MR_GetMax(i+1,ii+1)) ((Ps*)pPar)->MR_Set_Max(v+0.1*v);
					}
					else {
						if(!m_pSimDoc->MR_UseBayesianCoVar) {
							v=((P*)pPar)->GetValue(nTabIndex);
						}
						((P*)pPar)->MR_Set_Start(nTabIndex, v);
						if(v<m_pSimDoc->MR_GetMin(i+1,ii+1)) ((P*)pPar)->MR_Set_Min(nTabIndex, v-0.1*v);
						if(v>m_pSimDoc->MR_GetMax(i+1,ii+1)) ((P*)pPar)->MR_Set_Max(nTabIndex, v+0.1*v);


					}
				}
							
			}
		}





	}
	else if(item==1001) // Make use of Bayesian Calibration
	{
		if(m_pSimDoc->MR_UseBayesianCoVar)
			m_pSimDoc->MR_UseBayesianCoVar=false;
		else
			m_pSimDoc->MR_UseBayesianCoVar=true;
	}
	else if(item==1002) {
	// Remove All
		m_pSimDoc->MR_ResetAll();

	}
	else if(item==1003) {
	// Remove specific item;
		// Find Ps from row number

			GetCell(-1,row,&cell);
			int ip=int(cell.GetNumber());
			GetCell(0,row,&cell);
			int idim=int(cell.GetNumber());
					
			SimB *pPar=m_pSimDoc->MR_Get_pPar(idim,ip);
			int TabIndex=m_pSimDoc->MR_GetTabIndex(idim,ip);

			if(pPar!=nullptr) {
				if(TabIndex<0) 
					m_pSimDoc->MR_CP_Reset(pPar);
				else
					m_pSimDoc->MR_CP_Reset(pPar, TabIndex);
			}


	}
	else if(item==1004) {

		m_pSimDoc->MR_AddNewDimensiomWith11Repeatitions();

	}
	else if(item==1005) {
			GetCell(-1,row,&cell);
			int ip=int(cell.GetNumber());
			GetCell(0,row,&cell);
			int idim=int(cell.GetNumber());
			SimB *pPar=m_pSimDoc->MR_Get_pPar(idim,ip);
			int TabIndex=m_pSimDoc->MR_GetTabIndex(idim,ip);
			MultiRunTable dlg(m_pSimDoc, pPar, TabIndex, TRUE);
			if(dlg.DoModal()==IDOK){

			}

	}
	else if(item==1050) {

		GetCell(2,row,&cell);
		long num=int(cell.GetNumber());
		FUtil::WriteProfileInt("SimulationRunNo", num);


		m_pSimDoc->OpenSimDocument(num, true);
	}
	else if (item == 1055||item == 1056) {
		m_pSimDoc->m_ParToUpdate.clear();
		GetCell(0, row, &cell);
		auto iens = cell.GetIntNumber();
		auto GetSIMB = [&](int row) {

			GetCell(1, row, &cell);
			string parname = cell.GetStdText();
			SIMB_VALUE p1;
			pair<string, size_t> a = FUtil::GetIndexAndName(parname);
			parname = a.first;
			p1.TabIndex = a.second;
			SimB* pSimB = m_pSimDoc->GetPtrByName(parname);
			p1.pBase = pSimB;

			GetCell(3, row, &cell);
			auto value = cell.GetNumber();
			p1.newvalue = value;
			return p1;
		};
		SIMB_VALUE p = GetSIMB(row);
		m_pSimDoc->m_ParToUpdate.push_back(p);
		if (item == 1056) {
			size_t count = 1;
			GetCell(0, row+count, &cell);
			auto newiens = cell.GetIntNumber();
			while (newiens == iens) {
				p = GetSIMB(row + count);
				m_pSimDoc->m_ParToUpdate.push_back(p);
				count++;
				GetCell(0, row + count, &cell);
				newiens = cell.GetIntNumber();
			}
		}

		auto num = MFC_Util::GetProfileIntNo("SimulationRunNo", 100)+1;
		m_pSimDoc->m_DocFile2.m_Comments = "With mean value of Ensemble "+FUtil::STD_ItoAscii(iens)+ " from RunNo :" + FUtil::STD_ItoAscii(m_pSimDoc->m_DocFile.m_SimulationRunNo);
		m_pSimDoc->OpenSimDocument(num, true);
		
	}

	else if(item==1015||item==1025||item==1016) {
	// Change from GLUE to Bayes
		long NumLines;
		NumLines=m_pSimDoc->MR_Get_NumBayesPar();
		SimB* pPar;
		for(int jrow=0;jrow<NumLines;jrow++) {

			GetCell(-1,jrow,&cell);
			int ip=int(cell.GetNumber());
			GetCell(0,jrow,&cell);
			int idim=int(cell.GetNumber());
			pPar=m_pSimDoc->MR_Get_pPar(idim,ip);
			int imethod=m_pSimDoc->MR_GetMethod(idim,ip);
			if(item==1015) { // GLUE to Bayes
				if(imethod==2)
					imethod=9;
				else if(imethod==3)
					imethod=11;
			}
			else if(item==1016) //To Table
				imethod=4;
			
			else { // Bayes to Glue, item==1025
				if(imethod==9)
					imethod=2;
				else if(imethod==11)
					imethod=3;
			}
			auto TabIndex=m_pSimDoc->MR_GetTabIndex(idim,ip);
			if(TabIndex==string::npos) 
				((Ps*)pPar)->MR_Set_Method(MR_METHOD(imethod));
			else 
				((P*)pPar)->MR_Set_Method(TabIndex,MR_METHOD(imethod));

		}
		m_pSimDoc->MR_CheckCalibMethod();
	}
	else if (item >= 2000 && item < 2003) {
		// Save Parameter to CSV file
		size_t koll=m_pSimDoc->SaveSelectedParamtersToCSVFile(item - 2000);




	}
	else if (item >= 2005&&item<2010) {
		// Save Parameter to CSV file
		size_t koll = m_pSimDoc->ImportSelectedParamtersFromCSVFile(m_pSimDoc->m_FCSV_Array[item-2005]);




	}
	else if (item == 2010) {

		size_t koll = m_pSimDoc->ImportSelectedParamtersFromCSVFile(nullptr);







	}
	if(item>=1000&&item<1100) {
			BeginWaitCursor();
			EnableUpdate(FALSE); 
			while(GetNumberRows()>0)
					DeleteRow(0);
			EnableUpdate(TRUE); 
			OnSheetSetup(10);
			EndWaitCursor();
			return 10;
	}

	if(item>4000&&item<7000) {
		int Index;
		int nsel=m_pSimDoc->m_PlotStorage.NumSelected();
		if(nsel>0) {
			auto dtype=m_pSimDoc->m_PlotStorage.GetDataType(nsel-1);
			if(dtype<DATA_TYPES::SIM_VAL&&m_CurrentChart==m_Charts_Array.GetSize())
				m_VarNo=0;
		}
		else
			m_VarNo=0;
				
		if(!m_newChart&&m_CurrentChart!=1)
			m_CurrentChart++;


		if(m_CurrentChart==m_Charts_Array.GetSize()) {


		}



	//	m_newChart=TRUE;

		DATA_TYPES datatype=DATA_TYPES::SIM_PAR;  // IDENTIFIES Parameters multiplot
		Index=row;
		CCoupModelDoc *pDoc;
		int numvar=m_pSimDoc->m_PlotStorage.GetNumVar(m_CurrentChart);
		for(int j=0;j<((CCoupModelDoc*)m_pDoc)->m_Doc.m_SelectedDocuments.size();j++) {
			if(m_NumOpenDoc<=1)
				pDoc=m_pDoc;
			else
				pDoc=(CCoupModelDoc*)m_pSimDoc->m_SelectedDocuments[j];

			m_pSimDoc->m_PlotStorage.Add(nullptr, size_t(datatype) , Index, m_CurrentChart, m_pProp->m_ChartType,CT2A(pDoc->GetPathName()).m_psz, 0,0, 0,0,numvar+1);

		}

		SetSheetNumber(8); 
		int isheet=GetSheetNumber();
		EnableUpdate(FALSE); 
		m_ShowAllOutputs=FALSE;
		while(GetNumberRows()>0)
				DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(isheet);
		SetSheetNumber(10);

		GetCell(col,row, &cell);

	ChartCug *pChart;
	pChart=MySingle::instance().GetChartCugPointer();
				
		m_pFrame->m_wndProperties.SetChoice(0);



		if(pChart->InitMultiRunPlot(m_pSimDoc, m_CurrentChart, m_pProp->m_ChartType))
				cell.SetBackColor(RGB(80,240,240));
		}
				
	else if(m_pProp->m_ChartType ==CHART_TYPES::SCATTER)
		cell.SetBackColor(RGB(255,40,255));
	else
		cell.SetBackColor(RGB(240,0,0));
					
	SetCell(col,row, &cell);
	RedrawCell(col,row);
	return 10;
}
int DocCommands::AnnimateCommand(int col, int row, int item) {

	if(item==3200) 
			m_pSimDoc->ResetAnnimationSelection();
	else if(item==3201) 
			m_pSimDoc->SetDefaultAnnimationSelection();
	return -1;
}
int  DocCommands::MenuCornerCommand(int Col, int row, int item) {
	int sheet=GetSheetNumber();
	

	if((item==2000||item==2001)&&sheet==10) {
		if (m_ShowCoVarRes) {
			m_ShowCoVarRes = FALSE;
			if (item == 2001) {
				if (m_ShowAllEnsemblePostParList)
					m_ShowAllEnsemblePostParList = FALSE;
				else
					m_ShowAllEnsemblePostParList = TRUE;

			}
		}

		else {
			if (item == 2001) {
				if (m_ShowAllEnsemblePostParList)
					m_ShowAllEnsemblePostParList = FALSE;
				else
					m_ShowAllEnsemblePostParList = TRUE;
			}
			else
				m_ShowCoVarRes = TRUE;

		}
		return 10;

	}
	else if(item>=2050&&sheet==10) {
		Doc* pMySimDoc;
		vector<Doc*> ReadyDocuments=FUtil::GetReadyDoc(m_pSimDoc);
		if(item-2050<ReadyDocuments.size()) {
			pMySimDoc=ReadyDocuments[item-2050];
			pMySimDoc->m_MultiRun_Array.clear();
			pMySimDoc->m_P_MR_Array.clear();
			pMySimDoc->m_MultiRun_Array=m_pSimDoc->m_MultiRun_Array;
			pMySimDoc->m_P_MR_Array=m_pSimDoc->m_P_MR_Array;
			pMySimDoc->m_BayesianStepSize=m_pSimDoc->m_BayesianStepSize;
	

		}



	}
	else if(sheet==5&&item>=2500&&item<=2520) {
		CopyOutputSettings(item-2500);


	}
	else if(sheet==1&&item>=2500&&item<=2520) {
		CopySwitchSettings(item-2500);


	}
	else if(sheet==2&&item>=2500&&item<=2520) {
		CopySingleParameterSettings(item-2500);


	}
	else if(sheet==3&&item>=2500&&item<=2520) {
		CopyTableParameterSettings(item-2500);


	}
	else if(sheet==4&&item>=2500&&item<=2520) {
		CopyModelFileSettings(item-2500);


	}
	if(item==2001&&sheet==6) {
		if(m_ShowValidCoVarRes)
			m_ShowValidCoVarRes=FALSE;
		else
			m_ShowValidCoVarRes=TRUE;
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		return 6;

	}

	else if(item==2002&&sheet==6) {
			/*ValidationEvaluation Dlg(m_pDoc, this);
			int ans=Dlg.DoModal();
			if(ans==IDOK) {
				EnableUpdate(FALSE);
				while(GetNumberRows()>0)
					DeleteRow(0);
				EnableUpdate(TRUE); 
				return 6;				
			};*/
	}
	else if(item==2003&&sheet==6) {
		if(m_pSimDoc->IsMultiStorageFileLocked()) {
			m_pSimDoc->UnLockMultiStorageFile();
	/*		ValidationEvaluation Dlg(m_pDoc, this, true);
			int ans=Dlg.DoModal();
			if(ans==IDOK) {
				EnableUpdate(FALSE);
				while(GetNumberRows()>0)
					DeleteRow(0);
				EnableUpdate(TRUE); 
				return 6;				
			};*/
		}
		else
			m_pSimDoc->LockMultiStorageFile();
	}
	else if (item==6000) {

		m_pSimDoc->History_RemoveAll();
		OnTabSelected(12);
		
	}
	else if(sheet==11&&item>=2000) {
		HistoryCommand(Col, row, item);

	}
	else if(item==7010) {
			
		if(m_ShowValidationTimeSerieTable) 
			m_ShowValidationTimeSerieTable=FALSE;
		else
			m_ShowValidationTimeSerieTable=TRUE;
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		return sheet;
	}
	else if(item==7120)  {
		if(m_ParameterTableView==0) {
			m_ParameterTableView=1;
			UpdateDocumentLink();
		}
		else
			m_ParameterTableView=0;
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		
		return sheet;
	}
	else if( item>=7100&&item<7110) {
		m_ViewCategory=item-7100;
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		m_UpdateOutputList=true;
		return sheet;

	}


	
	
	return -1;
}
bool  DocCommands::MultiRunPlotCommand(int col, int row, int item, enum vartype var) {
	CUGCell cell;
	MultiVAL mul;
	int sheet;
	int Index=0;
	SIMB simb;
	int nselV=0;
	simb.pBase = nullptr;
	string str;
	VALSUMv valsum;
	if(var==SUMVAR) {
		sheet=5;
		simb=GetMultiSumPointer(row);
		Index=GetMultiSumIndex(simb, col);	

	}
	else if(var==VALVAR||var==VALVARSUM) {
		sheet=6;
		nselV=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
		if(row<nselV) {
			 Index=GetMultiValIndex(row, col);
			 simb=m_pSimDoc->m_ValidationData.GetSimPointer_TSV(row);

		}
		else {
			Index=GetMultiValSumIndex(row-nselV, col);
			valsum=m_pSimDoc->GetValSumStruct(Index);

			
			var = VALVARSUM;

			simb=m_pSimDoc->m_ValidationData.GetSimPointer_SV(row-nselV);
			if (simb.pBase == nullptr) {
				if (col == 13)
					str = valsum.IDVarName;
				else
					str = m_pSimDoc->m_ValSum_Array[row - nselV].Name;
			}

		}
	}
	else if(var==PARVAR) {
		sheet=10;
		simb=GetMultiRunParPointer(row);
		Index=GetMultiRunParIndex(row);
	}

	if(item==5018) {
		m_pProp->m_ChartType=CHART_TYPES::SCATTER;
		m_pProp->ResetXScale(string::npos);
		m_pProp->SetFirstIndepVarType(var);
		m_pProp->SetFirstIndepIndex(var, Index);
		//m_pProp->SetFirstIndepSet(var, true, Index);
		if(simb.pBase!=nullptr)
			str=simb.pBase->GetName();
		if(var==VALVAR) {
			
			m_pProp->SetFirstIndepValidTimeType(m_ValidTimeSerieType);
			switch(col) {
			case 7:
				str+=" r2";
				break;
			case 8:
				str+=" A0";
				if (simb.pBase != nullptr) str+=" ("+simb.pBase->GetUnit()+")";
				break;
			case 9:
				str+=" A1";

				break;
			case 10:
				str+=" ME";
				if (simb.pBase != nullptr) str+=" ("+simb.pBase->GetUnit()+")";
				break;
			case 11:
				str+=" RMSE";
				if (simb.pBase != nullptr) str+=" ("+simb.pBase->GetUnit()+")";
				break;
			case 14:
				str+=" LogLi";
				break;
			case 15:
				str+=" NSE";
				break;
			}
		}
		else if (var == VALVARSUM) {
			switch(col) {
			case 7:
				str = "R2";
				m_pProp->SetIndepValid_SV_Type(SUM_PERFORMANCE::SUM_R2);
				break;
			case 8:
				str = "A0";
				m_pProp->SetIndepValid_SV_Type(SUM_PERFORMANCE::SUM_INTERCEPT);
				break;
			case 9:
				str = "A1";
				m_pProp->SetIndepValid_SV_Type(SUM_PERFORMANCE::SUM_SLOPE);
				break;
			case 10:
				str = "ME";
				m_pProp->SetIndepValid_SV_Type(SUM_PERFORMANCE::SUM_ME);
				break;
			case 11:
				str = "RMSE";
				m_pProp->SetIndepValid_SV_Type(SUM_PERFORMANCE::SUM_RMSE);
				break;

			case 12:

				str = m_pSimDoc->m_ValSum_Array[Index].Name;
				str += " Simulated";
				m_pProp->SetIndepValid_SV_Type(SUM_PERFORMANCE::SUM_SIM);
				break;
			case 13:

				str = m_pSimDoc->m_ValSum_Array[Index].IDVarName;
				str += " Measured";
				m_pProp->SetIndepValid_SV_Type(SUM_PERFORMANCE::SUM_OBS);
				break;
			case 14:
				str += " LogLi";
				m_pProp->SetIndepValid_SV_Type(SUM_PERFORMANCE::SUM_LOGLI);
				break;
			case 15:
				str += " NSE";
				m_pProp->SetIndepValid_SV_Type(SUM_PERFORMANCE::SUM_NSE);
				break;

			}

		}
		else if(simb.pBase!=nullptr)
			str+=" ("+simb.pBase->GetUnit()+")";


		m_pProp->SetFirstIndepString(var,str);
		m_pProp->SetFirstIndepSet(var, true);
		GetCell(col,row,&cell);
		cell.SetBackColor(RGB(240,80,80));
		SetCell(col,row, &cell);
		RedrawCell(col,row);
		if(m_pProp->m_ActualMultiSheet>0) {
			//MultiPlotClick(m_pProp->m_ActualMultiSheet,col, row, false, 1);
			MakeMultiPlot(m_pProp->m_ActualMultiSheet,col, row);
			UpdatePlotStatusBar(m_pProp->m_ActualMultiSheet);
		}
		return true;
	}
	else if(item==5020) {
			switch (var) {
				case SUMVAR:
				while(m_pSimDoc->m_SelectMultiSum.size()>0) {
						mul=m_pSimDoc->m_SelectMultiSum[0];
						GetCell(mul.col,mul.row,&cell);
						cell.SetBool(FALSE);
						cell.SetBackColor(m_NoColor);
						SetCell(mul.col,mul.row,&cell);
						RedrawCell(mul.col,mul.row);
						m_pSimDoc->m_SelectMultiSum.erase(m_pSimDoc->m_SelectMultiSum.begin());
				}
				break;
				case VALVAR: case VALVARSUM:
				while(m_pSimDoc->m_SelectMultiVal.size()>0) {
						mul=m_pSimDoc->m_SelectMultiVal[0];
						GetCell(mul.col,mul.row,&cell);
						cell.SetBool(FALSE);
						cell.SetBackColor(m_NoColor);
						SetCell(mul.col,mul.row,&cell);
						RedrawCell(mul.col,mul.row);
						m_pSimDoc->m_SelectMultiVal.erase(m_pSimDoc->m_SelectMultiVal.begin());
				}
				break;
				case PARVAR:
				while(m_pSimDoc->m_SelectMultiPar.size()>0) {
						mul=m_pSimDoc->m_SelectMultiPar[0];
						GetCell(mul.col,mul.row,&cell);
						cell.SetBool(FALSE);
						cell.SetBackColor(m_NoColor);
						SetCell(mul.col,mul.row,&cell);
						RedrawCell(mul.col,mul.row);
						m_pSimDoc->m_SelectMultiPar.erase(m_pSimDoc->m_SelectMultiPar.begin());
				}
				break;
			}
			m_CurrentChart++;
			if(m_pProp->m_ActualMultiSheet>0) {
				MakeMultiPlot(m_pProp->m_ActualMultiSheet,col, row);
				//MultiPlotClick(m_pProp->m_ActualMultiSheet,col, row, false, 1);
				UpdatePlotStatusBar(m_pProp->m_ActualMultiSheet);
			}
		return true;
	}
	else if(item==5019) {
			m_pProp->m_ChartType= CHART_TYPES::TIME_SERIE;

			if(m_pSimDoc->IsMultiSimulation())
				m_pProp->SetFirstIndepVarType(MULTIRUN_NUM);
			else
				m_pProp->SetFirstIndepVarType(NO_USE);

			m_pProp->SetFirstIndepString(var,"");
			GetCell(col,row,&cell);
			cell.SetBackColor(RGB(255,255,255));
			SetCell(col,row, &cell);
			RedrawCell(col,row);
			m_pProp->SetFirstIndepSet(var,false);
			MakeMultiPlot(m_pProp->m_ActualMultiSheet,col, row);
			//MultiPlotClick(m_pProp->m_ActualMultiSheet,col, row, true,0);
		return true;
	}
	else if(item==5028) {
		m_pProp->m_ChartType= CHART_TYPES::CONTOUR_CHART;
		m_pProp->SetSecondIndepVarType(var);
		m_pProp->SetSecondIndepIndex(var, Index);
		if (simb.pBase == nullptr) return false;
		string str=simb.pBase->GetName();
		if(var==VALVAR) {
			switch(col) {
			case 7:
				str+=" r2";
				break;
			case 8:
				str+=" A0";
				str+=" ("+simb.pBase->GetUnit()+")";
				break;
			case 9:
				str+=" A1";

				break;
			case 10:
				str+=" ME";
				str+=" ("+simb.pBase->GetUnit()+")";
				break;
			case 11:
				str+=" RMSE";
				str+=" ("+simb.pBase->GetUnit()+")";
				break;
			case 13:
				str+=" LogLi";
				break;
			case 14:
				str+=" NSE";
				break;
			}
			m_pProp->SetSecondIndepValidTimeType(m_ValidTimeSerieType);
		}
		else if (var == VALVARSUM) {
			str = m_pSimDoc->m_ValSum_Array[Index].IDVarName;
			str += " Measured";
		}
		else 
			str+=" ("+simb.pBase->GetUnit()+")";





		m_pProp->SetSecondIndepString(var,str);
		GetCell(col,row,&cell);
		cell.SetBackColor(RGB(240,240,80));
		SetCell(col,row, &cell);
		RedrawCell(col,row);
		if(m_pProp->m_ActualMultiSheet>0) {
			MakeMultiPlot(m_pProp->m_ActualMultiSheet,col, row);
			//MultiPlotClick(m_pProp->m_ActualMultiSheet,col, row, false,2 );
			UpdatePlotStatusBar(m_pProp->m_ActualMultiSheet);
		}
		return true;

	}
	else if(item==5029) {
			m_pProp->m_ChartType=CHART_TYPES::SCATTER;	

			m_pProp->SetSecondIndepVarType(NO_USE);

			m_pProp->SetSecondIndepString(var,"");
			m_pProp->SetSecondIndepIndex(NO_USE,0);
			GetCell(col,row,&cell);
			cell.SetBackColor(RGB(255,255,255));
			SetCell(col,row, &cell);
			RedrawCell(col,row);
			MakeMultiPlot(m_pProp->m_ActualMultiSheet,col, row);
			//MultiPlotClick(m_pProp->m_ActualMultiSheet,col, row, false,1 );
// Remove Selected First Independent Variable

		return true;
	}
	else if (item == 7001) {
		MySingle::instance().GetPlotInfoPointer()->SetDoc(m_pSimDoc);

		// Plot all parameters in dimension
		// 1- clean, 2 Give command
		size_t I = 0;

		while (m_pSimDoc->m_SelectMultiPar.size() > 0) {
			m_pSimDoc->m_SelectMultiPar.clear();

		}
		GetCell(0, row, &cell);
		auto DimValue = cell.GetNumber();

		
		vector<int> row_to_plot;

		for (size_t i = 0; i < m_pSimDoc->m_P_MR_Array.size(); i++) {
			GetCell(0, long(i), &cell);
			auto ActDimValue = cell.GetNumber();
			if (ActDimValue == DimValue) {
				row_to_plot.push_back(i);
			}
		}
		MultiPlotVectorClick( row_to_plot);
		
		return true;

	}
	else if (item == 7010) {
		MySingle::instance().GetPlotInfoPointer()->SetDoc(m_pSimDoc);

		// Plot all parameters in dimension
		// 1- clean, 2 Give command
		vector<int> row_to_plot;
		for (size_t i = 0; i < m_pSimDoc->m_Val_Array.size(); i++)
				row_to_plot.push_back(i);


		while (m_pSimDoc->m_SelectMultiVal.size() > 0) {
			m_pSimDoc->m_SelectMultiVal.clear();

		}

		MultiPlotVectorClick(row_to_plot, col);

		return true;

	}

	else if (item == 5050) {
		auto pinfo = MySingle::instance().GetPlotInfoPointer();
		MySingle::instance().GetPlotInfoPointer()->SetDoc(m_pSimDoc);
		m_pProp = MySingle::instance().GetChartPropPointer();
		m_pProp->m_ConstrainProp.clear();
		if (m_pProp->m_ActualMultiSheet > 0) {
			size_t ncurves = m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);
			for (size_t jj = 0; jj < ncurves; jj++) {
				int j = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, jj);
				m_pSimDoc->m_PlotStorage.SetConstrainBy(j, m_pProp->m_ConstrainProp);
			}

			MakeMultiPlot(m_pProp->m_ActualMultiSheet, col, row);
			UpdatePlotStatusBar(m_pProp->m_ActualMultiSheet);
		}
		return true;
	}
	else if(item==5051){
		m_pProp->AddConstrainByMultiPar(simb, Index);
		if (m_pProp->m_ActualMultiSheet > 0) {
			size_t ncurves = m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);
			for (size_t jj = 0; jj < ncurves; jj++) {
				int j = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, jj);
				m_pSimDoc->m_PlotStorage.SetConstrainBy(j, m_pProp->m_ConstrainProp);
			}

			MakeMultiPlot(m_pProp->m_ActualMultiSheet, col, row);
			UpdatePlotStatusBar(m_pProp->m_ActualMultiSheet);
		}
		return true;
	}
	else if (item == 8050) {
		if (m_pSimDoc->m_MStorage.m_Valid_TSV[m_pSimDoc->m_MStorage.m_ActualEnsemble].tobe_Plotted)
			m_pSimDoc->m_MStorage.m_Valid_TSV[m_pSimDoc->m_MStorage.m_ActualEnsemble].tobe_Plotted = false;
		else
			m_pSimDoc->m_MStorage.m_Valid_TSV[m_pSimDoc->m_MStorage.m_ActualEnsemble].tobe_Plotted = true;



	}
	return false;
}
int DocCommands::ExcelExportCommand(int item, int Index, string varname) {
	ExportOptions opt=ExportOptions::Performance;
	if (item == 8014|| item == 8015) opt = ExportOptions::Outputs;
	else if (item >= 8008&&item< 8010) opt = ExportOptions::Parameters;
	else if (item == 8011)  opt = ExportOptions::ValidValues;
	else if (item <= 8005) opt = ExportOptions::Outputs;
	

	if (m_pSimDoc->IsFinishedSimulation()) {
		if (Index < 0) {
			string outfile;
			if (item % 2 == 0)
				outfile = m_pSimDoc->Export_XLSX(false, opt);
			else
				outfile = m_pSimDoc->Export_XLSX(true, opt);
			string message = "Data Exported to file : " + outfile;
			MFC_Util::MessageBox(message);
		}
		else {
			string outfile;
			if (item % 2 == 0)
				outfile = m_pSimDoc->Export_XLSX(false, opt, Index, varname);
			else
				outfile = m_pSimDoc->Export_XLSX(true, opt, Index, varname);
			string message = "Data Exported to file : " + outfile;
			MFC_Util::MessageBox(message);

		}
		return 0;
	}

	return -1;


}

void DocCommands::OnAddValFile()
{
		SimB *pSw;
		F *pCF;
		int fileDlgResult;
		size_t inumvalid=0;
		CString pszFileName, Group,Type,Name ;
		LPCTSTR lpszDefExt=_T("bin");
		LPCTSTR lpszFilter;
//The next line is only neccessary as long as the PGP does not handle long filenames!
//PEJ Excluded info box
		CString szFilter = _T("PG Bin file (*.BIN)|*.BIN||");
		CFileDialog	BinFileDlg(TRUE,lpszDefExt,_T("*.BIN"),OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,szFilter);
//Show the dialog
		fileDlgResult=BinFileDlg.DoModal();
//If the cancel button has been pushed then return
		if (fileDlgResult==IDOK) {
			pszFileName = BinFileDlg.GetPathName();
			//if(pCF->m_Exist) {
			// Create new pointer to new Validation File
				Group="Validation";
				Type="Switches";
				Name="ValidationMode";
									
				pSw=m_pSimDoc->GetPtr(SWITCH,"ValidationMode");
			
				inumvalid=((Sw*)pSw)->GetIntValue();
				//inumvalid=max(inumvalid,1);
				pCF=m_pSimDoc->ValidationFilePointer(inumvalid+1);
//
//								}
			pCF->SetValue(CT2A(pszFileName).m_psz);
			pCF->Apply();
			if(pCF->CheckFileNameAndAssignNameToPGClass()){ 
				pCF->ResetValNumbers();
				m_pSimDoc->TimeModified();
				EnableUpdate(FALSE); 
				while(GetNumberRows()>0)
					DeleteRow(0);
				EnableUpdate(TRUE); 
				OnSheetSetup(6);
				pCF->m_Exist=TRUE;
				pCF->SetEnable(true);

									
				SimB* pSw;
				pSw=m_pSimDoc->GetPtr(SWITCH, "ValidationMode");
				((Sw*)pSw)->SetIntValue(inumvalid+1);
				m_pSimDoc->ResidualFile_AddFile(1);
				//pSw->Apply();
				m_pSimDoc->SetOriginalValues("Files","ALL");
				m_pSimDoc->m_NumValidValFiles=-1;
				MySingle::instance().GetCugPointer()->OnSheetSetup(4);

			}
	}


}
VALSUMLINK DocCommands::OnAddSvFlexFiles()
{
	SimB *pSw;
	F *pCF;
	int fileDlgResult;
	VALSUMLINK sumlink;
	sumlink.FileName = "";

	LPCTSTR lpszDefExt = _T("csv");
	LPCTSTR lpszFilter;
	//The next line is only neccessary as long as the PGP does not handle long filenames!
	//PEJ Excluded info box
	CString szFilter = _T("Comma delimited Text file (*.csv)|*.csv||");
	CFileDialog	BinFileDlg(TRUE, lpszDefExt, _T("*.csv"), OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, szFilter);
	//Show the dialog
	fileDlgResult = BinFileDlg.DoModal();
	//If the cancel button has been pushed then return
	if (fileDlgResult == IDOK) {
		string s=CT2A(BinFileDlg.GetPathName()).m_psz;
		sumlink.FileName= s;
		m_pSimDoc->m_ValsumFlexFiles.push_back(sumlink);
	}
	return sumlink;
}
int DocCommands::OnAddTsvVal(bool Aggregated)
{
	F *pCF;
	int	NumSelected=m_pSimDoc->GetNumAll_TSV_ValVariables();


		CValidationVariables dlg(m_pDoc, NumSelected, Aggregated, this);
		if (dlg.DoModal() == IDOK) {
			Tab* pTab = (Tab*)m_pSimDoc->GetPtr(TABLE, "Scaling of Observed Variables");
			if (pTab != nullptr) {
				F* pFile;
				int NumSelected = 0;
				NumSelected = m_pSimDoc->GetNumAll_TSV_ValVariables();

				int oldnelements = pTab->GetNumOfRow();
				if (NumSelected != oldnelements) {
					pTab->SetNumOfRow(NumSelected);
					m_pSimDoc->m_ValidationData.UpdateValOutIndex();
				}
			}
		}
		SetSheetNumber(6);
		return 6;
}
int DocCommands::OnAddSvVal()
{
	SelectWindowForValidationSummaryVariables dlg(m_pDoc, this);
	dlg.m_VariableFromFile = false;
	if(dlg.DoModal()==IDOK) {


	}
	SetSheetNumber(6);
	return 6;
}
int DocCommands::OnAddSvValFromFile()
{
	SelectWindowForValidationSummaryVariables dlg(m_pDoc, this);
	dlg.m_VariableFromFile = true;
	if (dlg.DoModal() == IDOK) {
		
	}
	SetSheetNumber(6);
	return 6;
}
void DocCommands::OnValidationEval()
{
		int ans;
		if((m_pSimDoc->m_MStorage.GetNumAccepted()>0&&m_pSimDoc->m_MStorage.GetNumAccepted()<m_pSimDoc->MR_Get_TotalNumberofRuns())||m_pSimDoc->m_MStorage.IsCoCorValidSet()) {
		//	ValidationEvaluation Dlg(m_pDoc, this);
		//	ans=Dlg.DoModal();
		}
		else {
			m_pSimDoc->m_MStorage.UpdateAcceptedRuns(true, true);
		//	ValidationEvaluation Dlg(m_pDoc, this, true);
		//	ans=Dlg.DoModal();
		}

}
void DocCommands::ReEvaluateSvVal() {
	m_pSimDoc->m_IsPerformanceSmartFlexibleFile_Read = false;

	m_pSimDoc->UpdateValidationPerformance_SmartFlexVariables();
	m_pSimDoc->m_Are_MeanValues_SV_VariablesEstimated = false;
	m_pSimDoc->CalculateSumMeanValidValues_SV_Variables();
	m_pSimDoc->m_MStorage.m_SV_StatMemory.AllRuns = false;
	m_pSimDoc->m_MStorage.Update_SV_VarStat(false);
	m_pSimDoc->m_MStorage.Update_TSV_VarStat(false);
	m_pSimDoc->m_MStorage.UpdateAcceptedRuns(true, false);
	m_pSimDoc->m_MStorage.EvaluateFile(false);



}
void DocCommands::OnValidationEvalView()
{
/*	ValidationEvaluation Dlg(m_pDoc, this);
	int ans=Dlg.DoModal();
	if(ans==IDOK) {
		SetSheetNumber(6);
		EnableUpdate(FALSE);
		while(GetNumberRows()>0)
			DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(6);				

	};*/
}
void DocCommands::DeleteChart(int index) {


}
void DocCommands::UpdateChart(int index) {


}
bool DocCommands::UpdateChart(int index, CHART_TYPES chartType) {

	BOOL createNew;
	int chartnum;
	int arrayindex;
	int numcharts=m_Charts_Array.GetSize();
	createNew=TRUE;
	if(numcharts<1) {
		createNew=TRUE;
		chartnum=1;

		//CreateChart(chartType, index, itype, filename);
	//	return FALSE;
	}
	else {
		for( int i=0;i<m_pSimDoc->m_PlotStorage.GetNumberofCharts();i++) {
			arrayindex=m_pSimDoc->m_PlotStorage.GetIndex_Chart(index);
			if(m_pSimDoc->m_PlotStorage.GetNumberofCharts()==i-1) {
			}
		}
	}
	if(createNew) {
		enum class DATA_TYPES dtype;
		arrayindex=m_pSimDoc->m_PlotStorage.GetIndex_Chart(index);
		if(arrayindex<0) {
			chartnum=1;
			dtype=DATA_TYPES::ANYDATA;
		}
		else  {
			chartnum=m_pSimDoc->m_PlotStorage.GetChart(arrayindex);
			dtype=m_pSimDoc->m_PlotStorage.GetDataType(arrayindex);
		}
		CreateChart(chartType, chartnum, dtype, "");
	}
	return TRUE;

}
int DocCommands::CopyOutputSettings(int idoc) {
	Doc* pMySimDoc;
		vector<Doc*> ReadyDocuments=FUtil::GetReadyDoc(m_pSimDoc);

		if(idoc<ReadyDocuments.size()) {
			pMySimDoc=ReadyDocuments[idoc];
// Scan all outputs in current Document and make the same settings in the destination document
			pMySimDoc->m_X_Array.clear();	pMySimDoc->m_T_Array.clear();	pMySimDoc->m_G_Array.clear();	pMySimDoc->m_D_Array.clear();
			pMySimDoc->m_X_Array=m_pSimDoc->m_X_Array;
			pMySimDoc->m_T_Array=m_pSimDoc->m_T_Array;
			pMySimDoc->m_G_Array=m_pSimDoc->m_G_Array;
			pMySimDoc->m_D_Array=m_pSimDoc->m_D_Array;

		}
		return 5;
}
int DocCommands::CopySwitchSettings(int idoc) {
	Doc* pMySimDoc;
		vector<Doc*> ReadyDocuments=FUtil::GetReadyDoc(m_pSimDoc);

		if(idoc<ReadyDocuments.size()) {
			pMySimDoc=ReadyDocuments[idoc];
// Scan all outputs in current Document and make the same settings in the destination document
			SetCurrentSwitchesToDefault(pMySimDoc);
			//Set Switches to the values in the Imported Sw_Array
			CopySwitchValues(pMySimDoc);
		}
		return 1;
}
int DocCommands::CopySingleParameterSettings(int idoc) {
	Doc* pMySimDoc;
		vector<Doc*> ReadyDocuments=FUtil::GetReadyDoc(m_pSimDoc);

		if(idoc<ReadyDocuments.size()) {
			pMySimDoc=ReadyDocuments[idoc];
// Scan all outputs in current Document and make the same settings in the destination document
			SetCurrentSingleParToDefault(pMySimDoc);
			pMySimDoc->m_P_Array.clear();
			pMySimDoc->m_P_Array=m_pSimDoc->m_P_Array;
			CopySingleParValues(pMySimDoc);

		}
		return 2;
}
int DocCommands::CopyTableParameterSettings(int idoc) {
	Doc* pMySimDoc;
		vector<Doc*> ReadyDocuments=FUtil::GetReadyDoc(m_pSimDoc);

		if(idoc<ReadyDocuments.size()) {
			pMySimDoc=ReadyDocuments[idoc];
// Scan all outputs in current Document and make the same settings in the destination document
			SetCurrentTableParToDefault(pMySimDoc);
			CopyTableParValues(pMySimDoc);
		}
		return 3;
}
int DocCommands::CopyModelFileSettings(int idoc) {
	Doc* pMySimDoc;
		vector<Doc*> ReadyDocuments=FUtil::GetReadyDoc(m_pSimDoc);

		if(idoc<ReadyDocuments.size()) {
			pMySimDoc=ReadyDocuments[idoc];
// Scan all outputs in current Document and make the same settings in the destination document
			SetCurrentModelFileToDefault(pMySimDoc);
			CopyModelFileValues(pMySimDoc);

		}
		return 4;
}
void DocCommands::SetCurrentSwitchesToDefault(Doc *pDestinationDoc) {

		auto iv=pDestinationDoc->m_Sw_Array.size();
		for(size_t i=0;i<iv;i++) {
			auto pSw=pDestinationDoc->m_Sw_Array[i];
			int ivalue=pSw->GetIntValue();
			auto idef=pSw->GetOriginalIntValue();
			pSw->SetIntValue(idef);
		}

}
void DocCommands::SetCurrentSingleParToDefault(Doc *pDestinationDoc) {

		auto iv=pDestinationDoc->m_P_Array.size();
		for(size_t i=0;i<iv;i++) {
			auto pSw=pDestinationDoc->m_P_Array[i];
			int ivalue=pSw->GetIntValue();
			auto idef=pSw->GetOriginalValue();
			pSw->SetValue(idef);
		}


}
void DocCommands::SetCurrentTableParToDefault(Doc *pDestinationDoc) {
	auto iv=pDestinationDoc->m_Tab_Array.size();
	for(int i=0;i<iv;i++) {
		auto pTab=(Tab*)pDestinationDoc->m_Tab_Array[i];
		int v=pTab->GetNumOfCol();
		P *pP;
		double fvalue;
		for(size_t nCol=0; nCol<pTab->GetNumOfCol(); nCol++) {
				pP=pTab->GetP_Pointer(nCol);
				for(size_t nRow=0; nRow<pTab->GetNumOfRow(); nRow++) {
					fvalue=pP->GetOriginalValue(nRow);
					pP->SetValue(nRow,fvalue);
				}					
		}
		pTab->SetNumOfRow(pTab->GetNumOfOriginalRow());
	}
}
void DocCommands::SetCurrentModelFileToDefault(Doc *pDestinationDoc) {
		auto iv=pDestinationDoc->m_F_Array.size();
		for(size_t i=0;i<iv;i++) {
			auto pF=(F*)pDestinationDoc->m_F_Array[i];
			string str=pF->GetOriginalStrValue();
			pF->SetValue(str);
		}
}
void DocCommands::CopySwitchValues(Doc *pDestinationDoc) {
		pDestinationDoc->m_Sw_Array.clear();
		auto iv=m_pSimDoc->m_Sw_Array.size();
		for(size_t i=0;i<iv;i++) {
			auto pSw=m_pSimDoc->m_Sw_Array[i];
			auto ivalue=pSw->GetIntValue();
			auto pSwDest=(Sw*)pDestinationDoc->GetPtr("Switches",pSw->GetGroup(),pSw->GetName());			
			pSwDest->SetIntValue(ivalue);
			pDestinationDoc->m_Sw_Array.push_back(pSwDest);
		}


}
void DocCommands::CopySingleParValues(Doc *pDestinationDoc) {
		pDestinationDoc->m_P_Array.clear();
		auto iv=m_pSimDoc->m_P_Array.size();
		for(size_t i=0;i<iv;i++) {
			auto pPs=m_pSimDoc->m_P_Array[i];
			auto value=pPs->GetValue();
			auto pPDest=pDestinationDoc->GetPsPointer(pPs->GetName());			
			pPDest->SetValue(value);
			pDestinationDoc->m_P_Array.push_back(pPDest);
		}



}
void DocCommands::CopyTableParValues(Doc *pDestinationDoc) {
		pDestinationDoc->m_Tab_Array.clear();


		auto iv=m_pSimDoc->m_Tab_Array.size();

		for(int i=0;i<iv;i++) {
			auto pTab=(Tab*)m_pSimDoc->m_Tab_Array[i];

			Tab* pDestTab;
			pDestTab=(Tab*)pDestinationDoc->GetPtr("Tables",pTab->GetGroup(),pTab->GetName());

			
			auto rows=pTab->GetNumOfRow();
			pDestTab->SetNumOfRow(rows);
			P *pP, *pDest;
			double fvalue;

			for(size_t nCol=0; nCol<pTab->GetNumOfCol(); nCol++) {
					pP=pTab->GetP_Pointer(nCol);
					pDest=pDestinationDoc->GetPPointer(pP->GetName());
					for(size_t nRow=0; nRow<pTab->GetNumOfRow(); nRow++) {
						fvalue=pP->GetValue(nRow);
						pDest->SetValue(nRow,fvalue);
					}
						
			}
			pDestinationDoc->m_Tab_Array.push_back(pDestTab);
		}

}
void DocCommands::CopyModelFileValues(Doc *pDestinationDoc) {
		pDestinationDoc->m_F_Array.clear();
		auto iv=m_pSimDoc->m_F_Array.size();
		for(size_t i=0;i<iv;i++) {
			auto pF=m_pSimDoc->m_F_Array[i];
			string value=pF->GetStrValue();
			auto pFDest=(F*)pDestinationDoc->GetPtr("Files",pF->GetGroup(), pF->GetName());			
			pFDest->SetValue(value);
			pDestinationDoc->m_F_Array.push_back(pFDest);
		}
}
int DocCommands::AttachedFileCommand(int col, int row, int item) {
	CUGCell cell1, cell;
	string Name, Group, Type;

	GetCell(1, row, &cell1);
	//GetCell(col-1,row, &cell1);
	Name = cell1.GetStdText();

	GetCell(0, row, &cell1);
	Group = cell1.GetStdText();
	Type = "Files";
	CMainFrame *p_mainframe = MySingle::instance().GetFramePointer();
	if (item == 3000) {
		// Remove all 

		m_pSimDoc->m_MStorage.RemoveAddedFiles();
		m_pSimDoc->m_MStorage.EvaluateFile(false);
		m_pSimDoc->m_MStorage.m_SV_StatMemory.AcceptedRuns = false;
		m_pSimDoc->m_MStorage.m_SV_StatMemory.AllRuns = false;
		m_pSimDoc->m_MStorage.ResetAndReOpenFile();
		p_mainframe->m_wndEvalMulti.m_ValidationCug.OnSheetSetup(0);

	}
	else if (item == 3001) {
		if (row > 0) {
			m_pSimDoc->m_MStorage.RemoveAddedFile(size_t(row - 1));
			m_pSimDoc->m_MStorage.m_SV_StatMemory.AcceptedRuns = false;
			m_pSimDoc->m_MStorage.m_SV_StatMemory.AllRuns = false;
			m_pSimDoc->m_MStorage.EvaluateFile(false);
			m_pSimDoc->m_MStorage.ResetAndReOpenFile();
			p_mainframe->m_wndEvalMulti.m_ValidationCug.OnSheetSetup(0);
		}

	}
	else if (item == 3002) {

		if (!m_pSimDoc->m_MStorage.AttachNewFiles()) {
			return 13;
		}
		
		m_pSimDoc->m_MStorage.m_SV_StatMemory.AcceptedRuns = false;
		m_pSimDoc->m_MStorage.EvaluateFile(false);
		m_pSimDoc->m_MStorage.ResetAndReOpenFile();
		p_mainframe->m_wndEvalMulti.m_ValidationCug.OnSheetSetup(0);


	}
	MFC_Util mfc_util;
	mfc_util.SetPaneMessage("New Documents are now attached to the main Document - In total " + FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_TotalNumberofRuns()) + " Runs are accepted - Please specify criteria to constrain to posterior ensembles" );

	return 13;
}