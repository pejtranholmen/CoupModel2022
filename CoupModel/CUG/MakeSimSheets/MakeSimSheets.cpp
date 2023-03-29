#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include "..\..\Util.h"
#include "MakeSimSheets.h"
#include "..\..\NewBase\FileScanner.h"
#include "..\..\MainFrm.h"

#include "..\..\NewBase\codecogs/stats/Moments/covariance.h"
#include "..\..\NewBase\codecogs/stats/Moments/correlation.h"
#include "..\..\NewBase\codecogs/stats/Moments/variance.h"
#include "..\..\NewBase\codecogs/stats/Moments/mean.h"

UINT UpdateAcceptedThreadProc( LPVOID pParam )
{
    CResiduals* pObject = (CResiduals*)pParam;
	pObject->SmartUpdateStatistics(TRUE, nullptr);

    // do something with 'pObject'

    return 0;   // thread completed successfully
}







MakeSimSheets::MakeSimSheets(void)
{
	m_SetUpDone=false;
}


MakeSimSheets::~MakeSimSheets(void)
{
}
bool MakeSimSheets::Make_RunInfoSheet()
{
	CUGCell cell; 
	unsigned int MinPreStart=0;


	Sw* pSw;

	string heading, number, test;

	string Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	string Defstring;
	string optionlist;
	string str;
	int ivalue;
	int nOutputLines;

	

	m_bDrawExcelFocus = FALSE;
	SetMultiSelectMode(TRUE);
	UseHints(TRUE);

	SetSheetNumber(0); 
	EnableUpdate(FALSE); 
	while(GetNumberRows()>0)
		DeleteRow(0);
	EnableUpdate(TRUE); 
			if(m_pSimDoc==nullptr) return FALSE;

			SetDefFont(&m_font);



				if(m_pSimDoc->IsFinishedSimulation()) {
					if(m_pSimDoc->IsMultiSimulation())
						nOutputLines=7;
					else
						nOutputLines=4;
				}
				else
					nOutputLines=0;
				
				SetNumberRows(29+nOutputLines);

				SetNumberCols(4); 
				int nc=GetNumberCols();
				for(int i=0; i<nc;i++)
					UnJoinCells(i,-1);

				for (int i1=0;i1<nc;i1++)
					for (int i2=0;i2<29;i2++)
						UnJoinCells(i1,i2);

				SetColWidth(-1,0);
				SetColWidth(0,215);
				SetColWidth(1,215);
				SetColWidth(2,215);
				SetColWidth(3,215);
				// 4rd Row
				JoinCells(0,0,3,0);
				GetCell(0,0,&cell);
				cell.SetFont(&m_largeFont);
				SetRowHeight(0,30);
				cell.SetText("General Simulation Information");
				cell.SetBackColor(m_ColorHeading);
				cell.SetReadOnly(TRUE);
				SetCell(0,0,&cell);
				if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->IsMultiSimulation()) {
						if(m_pSimDoc->MR_Get_ActualRunNumber()==1)
							str="First run "+FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_ActualRunNumber());
						else if(m_pSimDoc->MR_Get_ActualRunNumber()==m_pSimDoc->MR_Get_TotalNumberofRuns())
							str="Last run "+FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_ActualRunNumber());
						else 
							str="Run no: "+FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_ActualRunNumber());
						
						str+=" of "+FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_TotalNumberofRuns())+" Runs";
						cell.SetText(str);
						SetCell(2,0,&cell);
				}


				// 1st Row

				if(m_pSimDoc->IsFinishedSimulation()||m_pSimDoc->IsRunning()) {
					QuickSetBackColor(-1,0,m_ColorCompleted);
					if(m_pSimDoc->IsMultiSimulation()) {
						QuickSetText(0,1, "Multi Run Document");
						//JoinCells(2,1,3,1);
						GetCell(1,-1,&cell);
						cell.SetFont(&m_largeFont);
						if(m_pSimDoc->IsRunning()) {
							cell.SetText("Multi Run Ongoing");
							cell.SetBackColor(m_ColorChanged);
						}
						else {
							cell.SetText("Completed Multi Run");
							cell.SetBackColor(m_ColorCompleted);
						}
						SetCell(1,-1,&cell);
						QuickSetText(0,-1,"");
						QuickSetBackColor(0,-1,m_ColorHeading);
					}
					else {
						QuickSetText(0,1, "Single Run Document");
						GetCell(0,-1,&cell);
						cell.SetFont(&m_largeFont);
						if(m_pSimDoc->IsRunning()) {
							cell.SetText("Single Run Ongoing");
							cell.SetBackColor(m_ColorChanged);
						}
						else {
							cell.SetText("Completed Single Run");
							cell.SetBackColor(m_ColorCompleted);
						}
						SetCell(0,-1,&cell);	
						QuickSetText(1,-1,"");
						QuickSetBackColor(1,-1,m_ColorHeading);
					}

					GetCell(2,-1,&cell);
					cell.SetText("Create Copy of Document");
					cell.SetBackColor(m_ColorClick);
					cell.SetFont(&m_largeFont);
					//if(m_pSimDoc->p_ModelInfo->IsReady()) 
					//	cell.SetBackColor(m_ColorHeading);

					SetCell(2,-1,&cell);
					QuickSetText(0,1,"Created/Modified/Simulated");
					QuickSetText(1,1,m_pSimDoc->GetTimeCreated());
					QuickSetText(2,1,m_pSimDoc->GetTimeModified());
					QuickSetText(3,1, m_pSimDoc->m_DocFile2.m_DateRun);
	
				}
				else {	
					QuickSetBackColor(-1,0,m_ColorClick);
					QuickSetText(0,1,"Created/Modified");
					QuickSetText(1,1,m_pSimDoc->GetTimeCreated());
					QuickSetText(2,1,m_pSimDoc->GetTimeModified());
					QuickSetText(0,-1,"Make Single Run");
					QuickSetBackColor(0,-1,m_ColorClick);
					if(m_pSimDoc->m_MultiRun_Array.size()>0) {
						str="Make Multiple Runs";
						QuickSetText(1,-1,str);
						QuickSetBackColor(1,-1,m_ColorClick);
						if(m_pSimDoc->m_MultiRun_Array.size()>0&&m_pSimDoc->MR_Get_ActualRunNumber()<m_pSimDoc->MR_Get_TotalNumberofRuns()) {
							str="ReStart Multiple Run at run no :"+FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_ActualRunNumber());
							QuickSetText(2,-1,str);
							QuickSetBackColor(2,-1,m_ColorClick);
							JoinCells(2,-1,3,-1);
						}
						else if(m_pSimDoc->MR_Get_ActualRunNumber()==m_pSimDoc->MR_Get_TotalNumberofRuns()){
							str="Extend previous MultiRun to a new MultiRun";
							QuickSetText(2,-1,str);
							QuickSetBackColor(2,-1, m_ColorClick);
							JoinCells(2,-1,3,-1);
						}
					}
					else {
						QuickSetText(1,-1,"");
						QuickSetBackColor(1,-1,m_ColorHeading);
						QuickSetText(2,-1,"");
						QuickSetBackColor(2,-1,m_ColorHeading);

					}

					//JoinCells(1,1,3,1);
				}

				
				
				
				// 2nd Row

				QuickSetText(0,2, "Simulation Number");
				GetCell(1,2,&cell);
				cell.SetMask("000000");
				cell.SetNumber(m_pSimDoc->m_DocFile.m_SimulationRunNo);
				cell.SetAlignment(UG_ALIGNLEFT);

				cell.SetParam(101);

				SimB *pBase=(SimB*)m_pSimDoc->m_RunInfo[0].pBase;
				if(pBase->GetNumOfHistoryIndex()>0) 
					cell.SetBackColor(m_ColorHistory);
				else 
					cell.SetBackColor(RGB(40,240,240));



				SetCell(1,2,&cell);
				QuickSetText(2,2, "No of MultiRuns");
				GetCell(3,2,&cell);
				cell.SetParam(112);
				cell.SetMask("000000");
				cell.SetNumber(m_pSimDoc->MR_Get_TotalNumberofRuns());
				
				pBase=(SimB*)m_pSimDoc->m_RunInfo[1].pBase;
				if(pBase->GetNumOfHistoryIndex()>0) 
					cell.SetBackColor(m_ColorHistory);
				else 

				cell.SetBackColor(RGB(40,240,240));
				SetCell(3,2,&cell);
				
				// 3rd Row
				
				QuickSetText(0,3, "Version");
				QuickSetText(1,3, m_pSimDoc->m_DocFile2.m_ExeFileDate);
				JoinCells(1,3,3,3);
				// 4rd Row
				JoinCells(0,4,3,4);
				GetCell(0,4,&cell);
				cell.SetFont(&m_largeFont);
				SetRowHeight(4,30);
				cell.SetText("Simulation Period");
				cell.SetBackColor(m_ColorHeading);
				SetCell(0,4,&cell);
				//5th row
				//QuickSetFont(0,5,&m_largeFont);
				QuickSetText(0,5,"StartDate");
				//QuickSetFont(2,5,&m_largeFont);
				QuickSetText(2,5,"EndDate");
				
				pSw=(Sw*)m_pSimDoc->GetPtr(SWITCH,"TimeResolution");
				GetCell(1,5,&cell);
				//cell.SetFont(&m_largeFont);
				cell.SetParam(102);
				cell.SetTime(0,0,0,1,5,1980);
				DatumString=PGUtil::StringDatum(m_pSimDoc->RunOpt.longMinStart).c_str();
				cell.SetText(DatumString);
   				pBase=(SimB*)m_pSimDoc->m_RunInfo[2].pBase;
				if(pBase->GetNumOfHistoryIndex()>0) 
					cell.SetBackColor(m_ColorHistory);
				else 
					cell.SetBackColor(m_ColorEdit);
				cell.SetMask("0000-00-00 00:00");
				SetCell(1,5,&cell);
				
				GetCell(3,5,&cell);
				//cell.SetFont(&m_largeFont);
				cell.SetTime(0,0,1,11,5,1980);
				DatumString=PGUtil::StringDatum(m_pSimDoc->RunOpt.longMinEnd).c_str();
				cell.SetText(DatumString);
				cell.SetParam(103);
    			pBase=(SimB*)m_pSimDoc->m_RunInfo[3].pBase;
				if(pBase->GetNumOfHistoryIndex()>0) 
					cell.SetBackColor(m_ColorHistory);
				else 
					cell.SetBackColor(m_ColorEdit);
				cell.SetMask("0000-00-00 00:00:00");
				SetCell(3,5,&cell);
				//6th row
				//QuickSetFont(0,6,&m_largeFont);
				QuickSetText(0,6,"PrePeriodDate");
				//QuickSetFont(2,6,&m_largeFont);
				QuickSetText(2,7,"Scaling of Period");
				QuickSetText(2, 6, "PostPeriodDate");
				
				GetCell(1,6,&cell);
				//cell.SetFont(&m_largeFont);
				cell.SetTime(0,0,0,1,5,1980);
				MinPreStart=m_pSimDoc->RunOpt.longMinPreStart;//-PARAMETERS_mp_PRESIMULATIONDAYS*1440;
				cell.SetText(PGUtil::StringDatum(MinPreStart).c_str());
				cell.SetParam(104);
		       	pBase=(SimB*)m_pSimDoc->m_RunInfo[4].pBase;
				if(pBase->GetNumOfHistoryIndex()>0) 
					cell.SetBackColor(m_ColorHistory);
				else 
					cell.SetBackColor(m_ColorEdit);
				cell.SetMask("0000-00-00 00:00");

				SetCell(1,6,&cell);
				GetCell(3, 6, &cell);
				//cell.SetFont(&m_largeFont);
				cell.SetTime(0, 0, 0, 1, 5, 1980);
				auto MinPostEnd = m_pSimDoc->RunOpt.longMinPostEnd;//-PARAMETERS_mp_PRESIMULATIONDAYS*1440;
				cell.SetText(PGUtil::StringDatum(MinPostEnd).c_str());
				cell.SetParam(113);
				pBase = (SimB*)m_pSimDoc->m_RunInfo[4].pBase;
				if (pBase->GetNumOfHistoryIndex() > 0)
					cell.SetBackColor(m_ColorHistory);
				else
					cell.SetBackColor(m_ColorEdit);
				cell.SetMask("0000-00-00 00:00");
				SetCell(3, 6, &cell);



				//QuickSetFont(3,6,&m_largeFont);



				QuickSetMask(3,7,_T("00"));
				//m_pSimDoc->m_TimePeriodScaleFactor;
				QuickSetNumber(3,7,m_pSimDoc->m_DocFile.m_TimePeriodScaleFactor,false);
				GetCell(3,7,&cell);
				cell.SetParam(105);
    			pBase=(SimB*)m_pSimDoc->m_RunInfo[5].pBase;
				if(pBase->GetNumOfHistoryIndex()>0) 
					cell.SetBackColor(m_ColorHistory);
				else 
					cell.SetBackColor(m_ColorEdit);

				SetCell(3,7,&cell);

				GetCell(0,7,&cell);
				cell.SetText("Additional Settings");
				cell.SetBackColor(m_ColorHeading);
				SetRowHeight(7,30);	
				cell.SetFont(&m_largeFont);
				//JoinCells(1,7,3,7);
				SetCell(0,7,&cell);


				QuickSetText(0,8,"Input Time Resolution");



					JoinCells(2,8,3,8);
			
					
					if(pSw!=nullptr) {
						ivalue=pSw->GetIntValue();
						GetCell(1,8, &cell); 
						cell.SetText(pSw->GetOption(ivalue).c_str());
						cell.SetCellType(UGCT_DROPLIST);
						size_t n=pSw->GetNumOptions();
						for(size_t j=0;j<n;j++) {
							optionlist+=pSw->GetOption(j).c_str();
							optionlist+="\n";
						}
						cell.SetLabelText(optionlist);





					}

					cell.SetParam(106);
    				pBase=(SimB*)m_pSimDoc->m_RunInfo[6].pBase;
					if(pBase->GetNumOfHistoryIndex()>0) 
						cell.SetBackColor(m_ColorHistory);
					else 
						cell.SetBackColor(RGB(50,240,240) );
					SetCell(1,8, &cell);


					
					auto pSwNit=(Sw*)m_pSimDoc->GetPtr(SWITCH,"Nitrogen and Carbon");
					if(pSwNit->GetIntValue()>0) {
						auto pSwNitRes=(Sw*)m_pSimDoc->GetPtr(SWITCH,"NitrogenCarbonStep");
						UnJoinCells(2,8);
						UnJoinCells(3,8);
						QuickSetText(2,8,"Nitrogen Carbon TimeStep");
						ivalue=pSwNitRes->GetIntValue();
						GetCell(3,8, &cell); 
						cell.SetText(pSwNitRes->GetOption(ivalue).c_str());
						cell.SetCellType(UGCT_DROPLIST);
						size_t n=pSwNitRes->GetNumOptions();
						optionlist="";
						for(size_t j=0;j<n;j++) {
							optionlist+=pSwNitRes->GetOption(j).c_str();
							optionlist+="\n";
						}
						cell.SetLabelText(optionlist);
						if(pSwNitRes->GetNumOfHistoryIndex()>0) 
							cell.SetBackColor(m_ColorHistory);
						else 
							cell.SetBackColor(m_ColorEdit);
						SetCell(3,8, &cell);
						if(ivalue==2) {
							// Independent value of Nitrogen Balance time step
							auto pParNitStep=(Ps*)m_pSimDoc->GetPtr(PAR_SINGLE,"NC Iteration");
							auto numstep=pParNitStep->GetValue();
							QuickSetText(2,11,"NC Iteration");
							GetCell(3,11,&cell);
							cell.SetNumber(numstep);
							if(pParNitStep->IsNotOriginalValue()) 
								cell.SetBackColor(m_ColorHistory);
							else 
								cell.SetBackColor(m_ColorEdit);
							
							if(pParNitStep->IsOldValue()) 
								cell.SetBackColor(m_ColorChanged);
							
							
							
							
							SetCell(3,11,&cell);

						}




					}
					else {
						JoinCells(2,8,3,8);
					}




					QuickSetText(1,9,"Days");
					QuickSetText(2,9,"Minutes");
                    QuickSetText(0,9,"Output Interval");
					JoinCells(0,9,0,10);
					GetCell(1,10, &cell); 
					cell.SetNumber(m_pSimDoc->RunOpt.oidays);
					cell.SetParam(107);
    				pBase=(SimB*)m_pSimDoc->m_RunInfo[7].pBase;
					if(pBase->GetNumOfHistoryIndex()>0) 
						cell.SetBackColor(m_ColorHistory);
					else 
						cell.SetBackColor(m_ColorEdit);
					SetCell(1,10,&cell);
					GetCell(2,10, &cell); 
					cell.SetNumber(m_pSimDoc->RunOpt.oiminutes);
					cell.SetParam(108);
    				pBase=(SimB*)m_pSimDoc->m_RunInfo[7].pBase;
					if(pBase->GetNumOfHistoryIndex()>0) 
						cell.SetBackColor(m_ColorHistory);
					else 
						cell.SetBackColor(m_ColorEdit);
					SetCell(2,10,&cell);


					QuickSetText(0,11,"Number of Iterations per day");
					GetCell(1,11, &cell); 
					cell.SetNumber(m_pSimDoc->RunOpt.noofiter);
					cell.SetParam(109);
    				pBase=(SimB*)m_pSimDoc->m_RunInfo[8].pBase;
					if(pBase->GetNumOfHistoryIndex()>0) 
						cell.SetBackColor(m_ColorHistory);
					else 
						cell.SetBackColor(m_ColorEdit);
					SetCell(1,11,&cell);

					QuickSetText(0,12,FUtil::STD_ItoAscii(m_pSimDoc->GetNumOutPuts())+" Selected Outputs");
					JoinCells(0,12,1,12);
					GetCell(2,12,&cell);

					int icountTot=0;
					int iCountFiles=0;
					F *pValFile;
					for(size_t i=0;i<MAXSIMVAL;i++) {
						pValFile=m_pSimDoc->ValidationFilePointer(i+1);
						icountTot+=pValFile->GetNumberOfSelectedVal();
						if(pValFile->GetNumberOfSelectedVal()>0) iCountFiles++;
					}
					str=FUtil::STD_ItoAscii(icountTot);
					cell.SetText(str+ " Selected TSV Validation Variables from "+FUtil::STD_ItoAscii(iCountFiles)+" Files");
					//cell.SetBackColor(m_ColorClick);
					//cell.SetParam(300);
					SetCell(2,12,&cell);
					GetCell(3,12,&cell);
					str=FUtil::STD_ItoAscii(m_pSimDoc->m_ValidationData.GetNumSumVarVariables());
					cell.SetText(str+ " Selected SV Validation Variables");
				//	cell.SetBackColor(m_ColorClick);
				//	cell.SetParam(301);
					SetCell(3,12,&cell);
					//JoinCells(2,11,3,11);
	

					JoinCells(0,13,1,13);
					GetCell(0,13,&cell);
		

				// m_pSimDoc->

					str=FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_NumBayesPar())+"/"+FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_NumTotalPar())+ " Selected Bayesian/Total Number of Multi Run Parameters";
					cell.SetText(str);
					//cell.SetBackColor(m_ColorClick);
					cell.SetParam(400);
					SetCell(0,13,&cell);

					GetCell(2,13,&cell);
					str=FUtil::STD_ItoAscii(m_pSimDoc->m_F_Array.size())+ " Selected Input Files";
					cell.SetText(str);
					SetCell(2,13,&cell);
					JoinCells(2,13,3,13);


					QuickSetText(0,14,"Short Id Code");
					GetCell(1,14, &cell); 
					cell.SetMask("aaa");
					cell.SetText(m_pSimDoc->m_DocFile2.m_RunId.c_str());

					cell.SetParam(110);
    				pBase=(SimB*)m_pSimDoc->m_RunInfo[9].pBase;
					if(pBase->GetNumOfHistoryIndex()>0) 
						cell.SetBackColor(m_ColorHistory);
					else 
						cell.SetBackColor(m_ColorEdit);	
					SetCell(1,14,&cell);
					JoinCells(2,14,3,14);
					QuickSetText(0,15,"Comment");
					GetCell(1,15, &cell); 
					cell.SetText(m_pSimDoc->m_DocFile2.m_Comments.c_str());
					cell.SetParam(111);
    				pBase=(SimB*)m_pSimDoc->m_RunInfo[10].pBase;
					if(pBase->GetNumOfHistoryIndex()>0) 
						cell.SetBackColor(m_ColorHistory);
					else 
						cell.SetBackColor(m_ColorEdit);
					SetCell(1,15,&cell);
					JoinCells(1,15,3,15);
					QuickSetText(0,16,"Origin of Document");
					GetCell(1,16, &cell); 
					cell.SetText(m_pSimDoc->m_DocFile2.m_OriginFileName.c_str());
					SetCell(1,16,&cell);
					JoinCells(1,16,3,16);

					GetCell(0,17,&cell);
					cell.SetText("Data Base Selections");
					cell.SetBackColor(m_ColorHeading);
					cell.SetFont(&m_largeFont);
					SetCell(0,17,&cell);
					JoinCells(0,17,3,17);
					SetRowHeight(17, 30);
					QuickSetText(0,18,"Object");
					QuickSetText(1,18,"Seleted Site Name");
					QuickSetText(2,18,"From");
					QuickSetText(3,18,"Date of import");
					for(int i=0;i<10;i++) {
						str="";
						GetCell(0,19+i,&cell);
						str=m_pSimDoc->DB_GetNames(i);
						cell.SetText(str);
						cell.SetParam(500+i);
						if(m_pSimDoc->IsFinishedSimulation())
							cell.SetBackColor(m_NoColor);
						else
							cell.SetBackColor(m_ColorRClick);
						SetCell(0,19+i,&cell);
						if(m_pSimDoc->DB_GetChoice(DB_types(i)).size()>0) {
								GetCell(1,19+i,&cell);
								CString cstr=CString(m_pSimDoc->DB_GetChoice(DB_types(i)).c_str());
								//str=str.Mid(str.Find("\t")+1);
								cstr=cstr.Mid(cstr.Find(_T("\t"))+1);
								int ipos=cstr.Find(_T("\t"));
								cell.SetText(cstr.Left(ipos));
								SetCell(1,19+i,&cell);
								cstr=cstr.Mid(cstr.Find(_T("\t"))+1);
								ipos=cstr.Find(_T("\t"));
								cell.SetText(cstr.Left(ipos)+_T(" : ")+cstr.Mid(cstr.Find(_T("\t"))+1));
								SetCell(2,19+i,&cell);
								QuickSetText(3,19+i,m_pSimDoc->DB_GetImportDate(i));
							}
					}


					// File info for Finished Simulations
					if(m_pSimDoc->IsFinishedSimulation()) {
						GetCell(0,29,&cell);
						cell.SetText("Output Files from simulation");
						cell.SetBackColor(m_ColorHeading);
						cell.SetFont(&m_largeFont);
						SetCell(0,29,&cell);
						JoinCells(0,29,3,29);
						SetRowHeight(29, 30);

						QuickSetText(0,30,"Directory");
						for(int i=0; i<3;i++)
							JoinCells(1,30+i,3,30+i);


						QuickSetText(0,31,"Time Series:");
						str="";
						QuickSetText(1,30,m_pSimDoc->GetCurrentWorkDirectory());
						if(m_pSimDoc->IsOutputBinFileValid()) {
							str=m_pSimDoc->GetOutputFileName(0).c_str();
							int i=str.rfind('\\');
							int iv=str.find("_Multi");
							if(i>0) {
								str=str.substr(i+1,str.size()-i-1);							
							}
		

						}
						else if(m_pSimDoc->IsMultiSimulation()) {
							str=m_pSimDoc->GetMultiBinFileName().c_str();

						}
						else
							str="No Time serie output file generated";
	
						QuickSetText(1,31,str);
						str="";
						if(m_pSimDoc->IsOutputValidBinFileValid()) {
							F *pCF;
							for(size_t i=1;i<=MAXSIMVAL;i++) {
								test=m_pSimDoc->GetOutputFileName(i);
								size_t ipos=test.find("_COUP_");
								test=test.substr(ipos-2,18);
								pCF=m_pSimDoc->ValidationResult_F_Pointer(i-1);
								if(pCF!=nullptr) {
									if (pCF->CheckFileNameAndAssignNameToPGClass()) {
											int niv = pCF->GetNumberOfVariables();
											CPG* pg = m_pSimDoc->ValidationResult_PG_Pointer(i - 1);
											if (pg != nullptr) niv = pg->GetNumVariables();


										if (niv > 0 && pCF->m_Exist)
											str += test + ";";
									}
								}
							}
							if(str.size()>1)
								str=str.substr(0,str.size()-1);

						}
						else
							str="No Validation output file generated";


						QuickSetText(0,32,"Validation Files:");


						QuickSetText(1,32,str);


						if(m_pSimDoc->IsMultiSimulation()){
							QuickSetText(0,30,"Archive Directory");
							QuickSetText(1,30, m_pSimDoc->GetCurrentSubDirectory());
							str="";
							QuickSetText(0,33,"Residual Files:");
							if (m_pSimDoc->GetNumAll_TSV_ValVariables() == 0) {
								str = "No Residual output file - No Validation Variables";
							}
							else if(m_pSimDoc->GetResidualPointer()!=nullptr) {
								for(int i=1;i<=16;i++) {
									F* pCF=(F*)m_pSimDoc->GetResidualPointer(i);
									test=m_pSimDoc->GetXBinFileName(i);
									if(pCF!=nullptr){
										int i=test.rfind('\\');
										str+=test.substr(i+1,test.size()-i-1)+";";
									}
								}
								if(str.size()>1)
									str=str.substr(0,str.size()-1);
							}
							else {
								str="No Residual output file";
							}
							QuickSetText(1,33,str);
							QuickSetText(0,34,"Multi Run file:");

							str=m_pSimDoc->GetMBinFileName();
							QuickSetText(1,34,str);


							QuickSetText(0,35,"Attatched Simulations:");
							int ifiles=0;
							if (m_pSimDoc->IsMultiStorageBinFileValid())
								ifiles=m_pSimDoc->m_MStorage.GetNumberOfAddedFiles(string(str));
							
							if(ifiles>0) {
								GetCell(1,35, &cell); 
								str="";
								for(int i=0;i<ifiles;i++) {
									str+=m_pSimDoc->m_MStorage.GetNameAddedFiles(i);
									str+="\n";
								}
								cell.SetCellType(UGCT_DROPLIST);
								cell.SetLabelText(str);
								cell.SetText(str);
								SetCell(1,35,&cell);
								
								QuickSetText(0,35,FUtil::STD_ItoAscii(ifiles)+" Attatched Simulations:");
							}
							for(int i=0; i<3;i++)
								JoinCells(1,33+i,3,33+i);
						}


					}
	return true;
}
bool MakeSimSheets::Make_SwitchesSheet()
{

	CUGCell cell; 
	CString heading, number, test;
	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString, Process;

	Sw *pSw;
	CString str, Defstring;
	CString optionlist;
	size_t NumLines, nval;
	int LastRow, StartRow;
	int MaxCol=9;
	int colv=0;
	Doc *pDoc;
	


	EnableMenu(TRUE); 
	m_bDrawExcelFocus = FALSE;
	SetMultiSelectMode(FALSE);

	QuickSetBackColor(-1,-1,m_ColorClick);
	if(m_ShowAllSwitches)
		QuickSetText(-1,-1,_T("-"));
	else
		QuickSetText(-1,-1,_T("+"));
	
	if(m_pSimDoc->IsFinishedSimulation())
		QuickSetBackColor(-1,0,m_ColorCompleted);
	else 
		QuickSetBackColor(-1,0,m_ColorClick);

		PrevGroup=" a";
				LastRow=0;
				StartRow=0;
				SetSheetNumber(1); 
				SetDefFont(&m_font);
				int col=3;
				auto numdoc=m_pSimDoc->m_Selected_All_Documents.size();
				if(m_ParameterTableView==1) {
					UpdateDocumentLink();
					numdoc=m_pSimDoc->m_Selected_All_Documents.size();
					SetNumberCols(col+numdoc+1);
					for(int j=0;j<numdoc;j++) {
						pDoc=m_pSimDoc->m_Selected_All_Documents[j];
						QuickSetText(col,-1,FUtil::STD_ItoAscii(pDoc->m_DocFile.m_SimulationRunNo));
						col++;
					}
				}
				else {
					col=4;
					numdoc=1;
					SetNumberCols(col+1); 
					QuickSetText(2,-1, "Value");
				}
				SetColWidth(-1,20);
				SetColWidth(0,220);
				SetColWidth(1,220);
				SetColWidth(2,200);
				for(int i=3; i<col+1;i++) 
					SetColWidth(i,250);
				
				QuickSetText(0,-1, p_ModelInfo->GetGroupCategoryNames(m_ViewCategory));
				if(m_ShowProcesses)
					QuickSetText(1,-1, "Process");
				else
					QuickSetText(1,-1, "Element");
				QuickSetBackColor(0, -1, m_ColorClick);
				QuickSetBackColor(1,-1, m_ColorClick);
				QuickSetBackColor(2, -1, m_ColorClick);
				QuickSetText(2,-1, "Name");
				QuickSetText(3,-1, "Options");
	
				QuickSetText(col,-1, "Default");

				vector<Sw*> Sw_Array = GetSwitchVectorByCategory(m_ViewCategory);
				NumLines = Sw_Array.size();
				SetNumberRows(NumLines);


				JoinCells(-1,0,-1,NumLines-1);
				for(int i=0; i<NumLines; ++i)	{

					if(m_pSimDoc->IsFinishedSimulation())
						QuickSetBackColor(-1,i,m_ColorCompleted);
					else 
						QuickSetBackColor(-1,i,m_ColorClick);


					pSw=Sw_Array[i];
					nval=pSw->GetIntValue();
					size_t defnval=pSw->GetOriginalIntValue();
					Name=CString(pSw->GetName().c_str());
					if(m_ShowProcesses)
						Process=pSw->GetProcessName().c_str();
					else 
						Process=pSw->GetElementName().c_str();

					Group=pSw->GetGroup().c_str();
					if(Name==CString(m_SelName.c_str())) {
						QuickSetBackColor(1,i,m_ColorClick);

					}
					
					test=pSw->GetOption(nval).c_str();
					Defstring=pSw->GetOption(defnval).c_str();

					CString optionlist;
					size_t n=pSw->GetNumOptions();
					for(size_t j=0;j<n;j++) {
						optionlist+=pSw->GetOption(j).c_str();
						optionlist+="\n";
					}

					if(m_ParameterTableView==1) {
						for(int j=1;j<numdoc;j++) {
							pDoc=m_pSimDoc->m_Selected_All_Documents[j];
							Sw *pNew;
							int newnval;
							pNew=m_pSimDoc->GetSwPointer(((Sw*)pSw), pDoc);
							if(pNew!=nullptr) {
								newnval=pNew->GetIntValue();
								if(newnval!=nval) {
									QuickSetText(3+j, i, pNew->GetOption(newnval).c_str() );
								}
							}
						}
						QuickSetText(3+numdoc,i, Defstring);
					}
					else
						QuickSetText(4,i, Defstring);

					QuickSetText(0,i, Group);
					QuickSetBackColor(0,i,MFC_Util::GetGroupColor(p_ModelInfo->GetGroupCategoryNo(pSw->GetGroupNo())));
					QuickSetText(1,i, Process);
					QuickSetText(2,i, Name);

					GetCell(3,i, &cell); 
					cell.SetCellType(UGCT_DROPLIST); 
					cell.SetLabelText(optionlist);
					if(m_pSimDoc->IsFinishedSimulation()) {
						cell.SetReadOnly(TRUE);
					}
					else {
						cell.SetReadOnly(FALSE);
					}
					cell.SetText(test);
					if(pSw->GetNumOfHistoryIndex()>0) {
						 cell.SetBackColor(m_ColorHistory);
					}
					else if(nval!=defnval) cell.SetBackColor(m_ColorEdit );
					if(pSw->IsOldValue()) cell.SetBackColor(m_ColorChanged);
					
					SetCell(3,i, &cell);

		}
		SetSheetNumber(1);
		SetTopRow(m_CurrentTopRow);

		if (m_pSimDoc->GetCurrentSheet() == 0) {
			m_pSimDoc->m_DefinedSwitches.clear();
			for (size_t i = 0; i < m_pSimDoc->m_Sw_Array.size(); i++) {
				pSw = (Sw*)m_pSimDoc->m_Sw_Array[i];
				if (p_ModelInfo->GetGroupCategory(pSw->GetGroup()) == m_ViewCategory || m_ViewCategory == 0) {
					if (pSw->IsNotOriginalValue())
						m_pSimDoc->m_DefinedSwitches.push_back((Sw*)pSw);
				}
			}
			Make_SwitchChart();
		}

		if(m_pSimDoc->IsFinishedSimulation())
			QuickSetBackColor(-1,0,m_ColorCompleted);
		else 
			QuickSetBackColor(-1,0,m_ColorClick);

		return TRUE;

}
bool MakeSimSheets::Make_ParameterSheet()
{
	CUGCell cell; 
	CString heading, number, test;

	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	Ps* pCP;
	Doc *pDoc;
	float fvalue, DefValue,fmin,fmax;
	CString str, Defstring;
	CString optionlist, GTitle, Process;
	int NumLines;
	int LastRow, StartRow;
	int MaxCol=8;
	int colv=0;
	DefValue=fvalue = MISSING;
	BOOL Tableexpand;
	Tableexpand=FALSE;
	EnableMenu(TRUE); 
	UseHints(TRUE);
	SetMultiSelectMode(FALSE);
	CPtrArray PtrArray;
	vector <Ps*> ValidPtrArray;

	QuickSetBackColor(-1,-1,m_ColorClick);
	
	if(m_ShowAllParameters)
		QuickSetText(-1,-1,"-");
	else
		QuickSetText(-1,-1,"+");

	PrevGroup=" a";
	LastRow=0;
	StartRow=0;
	SetSheetNumber(2); 
	SetNumberCols(8+m_NumOpenDoc);
	SetDefFont(&m_font);
	SetColWidth(-1,20);
	SetColWidth(0,250);
	SetColWidth(1,180);
	SetColWidth(2,200);
	SetColWidth(8,280);
	QuickSetText(0,-1, p_ModelInfo->GetGroupCategoryNames(m_ViewCategory));
	if(m_ShowProcesses)
		QuickSetText(1,-1, "Process");
	else
		QuickSetText(1,-1, "Element");
	QuickSetBackColor(0,-1, m_ColorClick);
	QuickSetBackColor(1, -1, m_ColorClick);
	QuickSetBackColor(2, -1, m_ColorClick);
	QuickSetText(2,-1, "Name");
	QuickSetText(3,-1, "Value");
	int col=3;
	auto numdoc=m_pSimDoc->m_Selected_All_Documents.size();
	if(m_ParameterTableView==1) {
		UpdateDocumentLink();
		numdoc=m_pSimDoc->m_Selected_All_Documents.size();
		for(int j=0;j<numdoc;j++) {
			pDoc=m_pSimDoc->m_Selected_All_Documents[j];
			QuickSetText(col,-1,FUtil::STD_ItoAscii(pDoc->m_DocFile.m_SimulationRunNo));
			col++;
		}
	}
	else {
		col=4;
		numdoc=1;
	}
	QuickSetText(col,-1, "Default"); col++;
	QuickSetText(col,-1, "Min");col++;
	QuickSetText(col,-1, "Max");col++;
	QuickSetText(col,-1, "Unit");col++;
	QuickSetText(col,-1, "Function");
	if(m_ParameterTableView==1) {
		for(int j=0;j<numdoc;j++) {
				if(numdoc<=1)
						pDoc=m_pSimDoc;
				else
						pDoc=m_pSimDoc->m_Selected_All_Documents[j];
		}
	}


	ValidPtrArray=GetParameterVectorByCategory(m_ViewCategory);
	NumLines = ValidPtrArray.size();
	SetNumberRows(NumLines); 
	JoinCells(-1,0,-1,NumLines-1);
	m_ClickedRow=-1;
	for(int i=0; i<NumLines; ++i)	{
		pCP = ValidPtrArray[i];
		if (pCP != nullptr) {
			fvalue = pCP->GetValue();
			DefValue = pCP->GetOriginalValue();
			if (pCP == m_pSel) {
				m_ClickedRow = i;
			}
		}
		else
			continue;
		Name=pCP->GetName().c_str();
		Group=pCP->GetGroup().c_str();
		Unit=pCP->GetUnit().c_str();
		fmin=pCP->GetMinValue();
		fmax=pCP->GetMaxValue();
		Ps* pCP1=(Ps*)pCP;


		GTitle=pCP1->GetViewTitle().c_str();
		if(m_ShowProcesses)
			Process=pCP1->GetProcessName().c_str();
		else
			Process=pCP1->GetElementName().c_str();

		if(m_pSimDoc->IsFinishedSimulation())
			QuickSetBackColor(-1,i,m_ColorCompleted);
		else 
			QuickSetBackColor(-1,i,m_ColorClick);
		QuickSetText(0,i, Group);
		QuickSetBackColor(0,i,MFC_Util::GetGroupColor(p_ModelInfo->GetGroupCategoryNo(pCP1->GetGroupNo())));
		QuickSetText(1,i, Process);
		QuickSetText(2,i, Name);

		GetCell(3,i, &cell); 
		cell.SetNumber(fvalue);

		if(fvalue!=DefValue) cell.SetBackColor(m_ColorEdit);
		if(pCP1->GetNumOfHistoryIndex()>0) {
			cell.SetBackColor(m_ColorHistory);
		}
		else if(fvalue!=DefValue) 
			cell.SetBackColor(m_ColorEdit);
		if(fvalue<1.E-6&&fvalue>0)
			cell.SetNumberDecimals(9);
		if(pCP1->IsOldValue()) {
			cell.SetBackColor(m_ColorChanged);
		}
		SetCell(3,i, &cell);

		col=4;
		if(m_ParameterTableView==1) {
			for(int j=1;j<numdoc;j++) {
				pDoc=m_pSimDoc->m_Selected_All_Documents[j];
				float fval;
				Ps *pNew;
				pNew=m_pSimDoc->GetParPointer(pCP1, pDoc);
				if(pNew!=nullptr) {
					fval=pNew->GetValue();
					if(fval!=fvalue) 
						QuickSetNumber(col,i,fval);
				
				}
			
				col++;
			}
		}

		QuickSetNumber(col,i, DefValue); col++;
		QuickSetNumber(col,i, fmin); col++;
		QuickSetNumber(col,i, fmax); col++;
		QuickSetText(col,i, Unit); col++;
		if(GTitle!="") {

				size_t no_func;
				no_func=pCP1->GetNumOfTitles();
				if(no_func>0) {
					string txt;
					GetCell(col, i,&cell);
					cell.SetCellType(UGCT_DROPLIST); 
					CString optionlist;
					for(size_t j=0; j<no_func;j++) {
						txt=pCP1->GetViewTitle(j);
						txt+="\n";
						optionlist+=txt.c_str();






					}
					txt=pCP1->GetViewTitle(pCP1->GetSelectedFunc());
					cell.SetLabelText(optionlist);
					cell.SetText(txt.c_str());
					SetCell(col,i, &cell);
				}
				else {
					QuickSetText(col,i,GTitle);
				}

		}
		else
			QuickSetText(col, i, "");
	}
//	SortBy(0, UG_SORT_ASCENDING); 
	m_sort[2]=TRUE;
	if(m_pSimDoc->IsFinishedSimulation())
		QuickSetBackColor(-1,0,m_ColorCompleted);
	else 
		QuickSetBackColor(-1,0,m_ColorClick);
	PtrArray.RemoveAll();
	ValidPtrArray.clear();
	RedrawAll();
	return true;
}
bool MakeSimSheets::Make_HistorySheet()
{
	CUGCell cell; 
	CString heading, number, test;

	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;


	CString strvalue, Signature, ComputerName;
	CString optionlist, GTitle, Process;
	int NumLines;
	int LastRow, StartRow;
	int MaxCol=8 ;
	int colv=0;
	time_t tt;
	float fvalue;
	
	BOOL Tableexpand;

	Tableexpand=FALSE;
	EnableMenu(TRUE); 
	SetMultiSelectMode(TRUE);

	QuickSetBackColor(-1,-1,m_ColorClick);
	
	if(m_ShowHistoryList)
		QuickSetText(-1,-1,"-");
	else
		QuickSetText(-1,-1,"+");
	
	PrevGroup=" a";
				LastRow=0;
				StartRow=0;
				SetSheetNumber(11); 

				SetNumberCols(7);
				SetDefFont(&m_font);
				SetColWidth(-1,20);
				SetColWidth(0,160);
				SetColWidth(1,160);
				SetColWidth(2,200);
				SetColWidth(3,250);
				SetColWidth(4,100);
				SetColWidth(5,100);
				SetColWidth(6,180);
		 
				QuickSetText(0,-1, "Date/Time");
				QuickSetText(1,-1, "Type");
				QuickSetText(2,-1, "Group");
				QuickSetText(3,-1, "Name");
				QuickSetText(4,-1, "New Value");
				QuickSetText(5,-1, "Signature");
				QuickSetText(6,-1, "Computer");
				if(m_ShowHistoryList)
	  				NumLines=m_pSimDoc->History_GetNumber();
				else
					NumLines=0;
				SetNumberRows(NumLines); 


				JoinCells(-1,0,-1,NumLines-1);
				int irow=-1;
				size_t index;
				for(size_t i=NumLines-1; i!=string::npos; i--)
				{
					irow++;
					if (m_pSimDoc->m_ViewAll) index = i;
					else index = m_pSimDoc->m_IndexToView[i];
					SimB *pCP=(SimB*)m_pSimDoc->History_GetPointer(index);
					size_t RunInfoType=m_pSimDoc->History_GetRunInfoType(index);
					RunInfoType = m_pSimDoc->History_GetEnumType(index);
					HIST_INFO hinfo = HIST_INFO(RunInfoType);
					tt=m_pSimDoc->History_GetTime(index);
					//Type=p_ModelInfo->GetVarType(RunInfoType-1).c_str();
					size_t RunInfoIndex = m_pSimDoc->History_GetLocalIndex(index);
					if(pCP==nullptr) {
						Name=_T("");
					}
					else if(hinfo==HIST_INFO::RUN_INFO ){
						CRunInfo* pRuninfo;
						pRuninfo = static_cast<CRunInfo*>(pCP);
						if (pRuninfo != nullptr) {
							Group = ((CRunInfo*)pCP)->GetGroup().c_str();
							Name = ((CRunInfo*)pCP)->GetName().c_str();
						}
						if (Group == "General") {
							Type == "Koll";
						}
						strvalue=m_pSimDoc->History_GetString(index).c_str();
						Type = "Run info";

					}
					else if (hinfo == HIST_INFO::SWITCH_INFO) {

						Group = pCP->GetGroup().c_str();
						Name = pCP->GetName().c_str();
						strvalue = m_pSimDoc->History_GetString(index).c_str();
						Type = "Switches";
					}
					else if(RunInfoType==10){
						int n=m_pSimDoc->History_GetLocalIndex(index);
						Group=m_pSimDoc->DB_GetNames(n).c_str();
						Name=_T("");
						strvalue=m_pSimDoc->History_GetString(index).c_str();
						m_pSimDoc->History_GetLocalIndex(index);

					}
					else if (hinfo == HIST_INFO::PARSINGLE_INFO) {
						fvalue = m_pSimDoc->History_GetFloat(index);
						Group = pCP->GetGroup().c_str();
						Name = pCP->GetName().c_str();
						strvalue = MFC_Util::FtoAscii(fvalue);
						Type = "Parameters";
					}
					else if (hinfo == HIST_INFO::NUMBER_ELEMENTS) {
						Group = pCP->GetGroup().c_str();
						Name = pCP->GetName().c_str();
						strvalue = m_pSimDoc->History_GetString(index).c_str();
						Type = "Number of Elements";
					}
					else if (hinfo == HIST_INFO::MODELFILE_INFO) {
						Group = pCP->GetGroup().c_str();
						Name = pCP->GetName().c_str();
						strvalue = m_pSimDoc->History_GetString(index).c_str();
						Type = "ModelFiles";
					}
					else if(hinfo==HIST_INFO::PARTABLE_INFO){
						fvalue=m_pSimDoc->History_GetFloat(index);
						Group=pCP->GetGroup().c_str();
						Name=pCP->GetName().c_str();
						Name+=_T("(")+MFC_Util::ItoAscii(m_pSimDoc->History_GetLocalIndex(i)+1)+_T(")");
						strvalue=MFC_Util::FtoAscii(fvalue);
						Type = "Parameter Table";



						//Group=_T("");
					//	Name=((CDB*)pCP)->GetName().c_str();
						//strvalue=m_pSimDoc->History_GetString(i).c_str();

					}

					else if(RunInfoType==1) {
						if (!pCP->Is_DB()) {
							fvalue = m_pSimDoc->History_GetFloat(index);
							Group = pCP->GetGroup().c_str();
							Name = pCP->GetName().c_str();
							strvalue = MFC_Util::FtoAscii(fvalue);
							Type = "Parameters";

						}
						else {
							strvalue = m_pSimDoc->History_GetString(index).c_str();
							Type = "Data Base";
							Name = pCP->GetName().c_str();
						}
		
					}
					else if(RunInfoType==3) {
						fvalue=m_pSimDoc->History_GetFloat(index);
						Group=pCP->GetGroup().c_str();
						Name=pCP->GetName().c_str();
						Name+=_T("(")+MFC_Util::ItoAscii(m_pSimDoc->History_GetLocalIndex(i)+1)+_T(")");
						strvalue=MFC_Util::FtoAscii(fvalue);
					}

				
					Signature=m_pSimDoc->History_GetUser(index).c_str();
					ComputerName=m_pSimDoc->History_GetComputer(index).c_str();

					char buffer[24];
					struct tm *timeinfo;
					if (Group == "General"&&i==186) {

						int koll = 1;
					}

					timeinfo =localtime(&tt);
					if(timeinfo!=nullptr) {
						strftime(buffer,24, "%Y-%m-%d %H:%M:%S", timeinfo);
						QuickSetText(0,irow,buffer);
						QuickSetText(1,irow,Type);
						QuickSetText(2,irow, Group);
						QuickSetText(3,irow, Name);
						GetCell(4,irow,&cell);
						cell.SetCellType(UGCT_NORMAL);
						cell.SetText( strvalue);
						SetCell(4,irow,&cell);

						//QuickSetText(4,irow, strvalue);
						QuickSetText(5,irow, Signature);
						QuickSetText(6,irow, ComputerName);
						if(m_pSimDoc->IsFinishedSimulation())
							QuickSetBackColor(-1,irow,m_ColorCompleted);
						else 
							QuickSetBackColor(-1,irow,m_ColorClick);
					}

				}

	if(m_pSimDoc->IsFinishedSimulation())
		QuickSetBackColor(-1,0,m_ColorCompleted);
	else 
		QuickSetBackColor(-1,0,m_ColorClick);
	RedrawAll();
	return true;


}
bool MakeSimSheets::Make_ParameterTableSheet()
{
	CUGCell cell; 
	CString heading, number, test;

	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	Tab *pTab;
	float fvalue, DefValue;
	CString str, Defstring, Process;
	CString optionlist;
	int NumLines, NumTabRows;
	int LastRow, StartRow;
	int newtablines,newline ;
	int MaxCol=9;
	int colv=0;

	BOOL Tableexpand;

	Tableexpand=FALSE;

	m_bDrawExcelFocus = FALSE;
	//SetMultiSelectMode(TRUE);
	EnableMenu(TRUE); 
	SetDoubleBufferMode(TRUE); 
	QuickSetBackColor(-1,-1,m_ColorClick);


				SetSheetNumber(3); 
				SetDefFont(&m_font);

 				vector<Tab*> pTables;
				SetUniformRowHeight(TRUE); 
				SetMultiSelectMode(FALSE);
				

				newtablines=0;
				auto numdoc=m_pSimDoc->m_Selected_All_Documents.size();

				vector<Tab*> Tab_Array = GetTabVectorByCategory(m_ViewCategory);
				NumLines = m_pSimDoc->m_Tab_Array.size();
				for(int i=0; i<NumLines; ++i)	{
					pTab=Tab_Array[i];
					if(p_ModelInfo->GetGroupCategory(pTab->GetGroup())==m_ViewCategory||m_ViewCategory==0) {
						pTables.push_back(pTab);
					

						if(i==0&&m_ShowAllTables)
							QuickSetText(-1,-1,"+");
						else if(i==0)
							QuickSetText(-1,-1,"-");
						NumTabRows=pTab->GetNumOfRow();
						newtablines++;
						if(((Tab*)pTab)->GetHide()!=TRUE) {
							if(m_ParameterTableView==0) {
								int nrow=pTab->GetNumOfRow();
								newtablines+=nrow+5;
								int icol=pTab->GetNumOfCol();
								if(icol>MaxCol)
									MaxCol=icol;
							}
							else {
								int nrow=pTab->GetNumOfRow();
								int icol=pTab->GetNumOfCol();
								newtablines+=nrow*icol;
								MaxCol=numdoc+4;
							}
						}
					}
				}


				NumLines=pTables.size();
				PrevGroup=" a";
				LastRow=0;
				StartRow=0;

				SetNumberCols(MaxCol+4); // Add one index column

				SetNumberRows(newtablines);
				SetColWidth(-1,20);
				SetColWidth(0,200);
				for(int i=1;i<8;i++)
					SetColWidth(i,130);
				for(int i=8;i<MaxCol+4;i++)
					SetColWidth(i,110);

				QuickSetText(0,-1, p_ModelInfo->GetGroupCategoryNames(m_ViewCategory));
				if(m_ShowProcesses)
					QuickSetText(1,-1, "Process");
				else
					QuickSetText(1,-1, "Element");
				QuickSetBackColor(0,-1, m_ColorClick);
				QuickSetBackColor(1, -1, m_ColorClick);
				QuickSetBackColor(2, -1, m_ColorClick);
				QuickSetText(2,-1, "Table Name");
				QuickSetText(5,-1, "Number of Elements");
				QuickSetText(6,-1, "Default");

				//JoinCells(0,-1,1,-1);
				JoinCells(2,-1,4,-1);


				newline=0;
				Tableexpand=FALSE;
				m_ClickedRow=-1;
				JoinCells(-1,0,-1,newtablines-1);

				for(size_t i=0; i<pTables.size(); ++i)	{
					if(m_pSimDoc->IsFinishedSimulation())
						QuickSetBackColor(-1,newline,m_ColorCompleted);
					else 
						QuickSetBackColor(-1,newline,m_ColorClick);
					pTab=pTables[i];
					NumTabRows=pTab->GetNumOfRow();
					Name=pTab->GetName().c_str();
					Group=pTab->GetGroup().c_str();
					if(m_ShowProcesses)
						Process=pTab->GetProcessName().c_str();
					else
						Process=pTab->GetElementName().c_str();
					if(m_pSimDoc->IsFinishedSimulation()) {
					}
					else {
					}
					
					QuickSetFont(0,newline,&m_font);
					QuickSetText(0,newline, Group);
					QuickSetBackColor(0,newline,MFC_Util::GetGroupColor(p_ModelInfo->GetGroupCategoryNo(pTab->GetGroupNo())));
					QuickSetFont(1,newline,&m_font);
					QuickSetText(1,newline, Process);
					//JoinCells(0,newline,1,newline);
					
					QuickSetFont(2,newline,&m_font);
					QuickSetText(2,newline, Name);
					JoinCells(2,newline,4,newline);
					GetCell(2,newline, &cell);
					cell.SetParam(1000);  // Indicate outlining of text
					cell.SetBackColor(m_ColorClick);
					SetCell(2,newline, &cell);
					RedrawCell(2,newline); 

	
					QuickSetNumber(5,newline, NumTabRows, false);

					if(pTab->GetNumOfHistoryIndex()>0) {
						 QuickSetBackColor(5,newline, m_ColorHistory);
					}
					
					GetCell(5,newline, &cell);
					cell.SetParam(3000);
					cell.SetFont(&m_font);
					SetCell(5,newline, &cell);

					size_t NumOrigLines;
					NumOrigLines=pTab->GetNumOfOriginalRow();
					QuickSetNumber(6,newline, NumOrigLines, true);
						
					JoinCells(7,newline,MaxCol+2,newline);
					//QuickSetBitmap(7,newline, m_BMP1);
					//RedrawCell(7,newline);
					
					newline++;
					if(CT2A(Name).m_psz==m_ClickedTable){
						m_ClickedRow=newline;

					}

					if(pTab->GetHide()!=TRUE) {
						Tableexpand=TRUE;
						P *pCPt;
						int nColValid=2;
						int view=pTab->GetViewFunction();
						bool readonly;
						if(view==405||view==406) 
							readonly=TRUE;
						else
							readonly=FALSE;
						size_t nrow=pTab->GetNumOfRow();// Number of rows in table
						size_t icol=pTab->GetNumOfCol();

						if(m_ParameterTableView==0) {

							QuickSetText(2, newline,"Index");
							QuickSetBackColor(2,newline,m_ColorHeading); //
							QuickSetText(0,newline, Group);
							size_t maxrow=0;
							for(int nCol=0; nCol<icol;nCol++) {
								pCPt=pTab->GetP_Pointer(nCol);
								maxrow=pCPt->GetSize();
								if(pCPt->IsEnabled()) {
									QuickSetText(nColValid+1, newline,pCPt->GetName());
									QuickSetBackColor(nColValid+1,newline,m_ColorHeading);
									QuickSetCellTypeEx(nColValid+1,newline,UGCT_NORMALMULTILINE);
									nColValid++;
								}
					// Set String
							}
							if(maxrow>0) {
								newline++;
								for(size_t nRow=0; nRow<maxrow; nRow++){
									GetCell(2,newline,&cell);
									cell.SetNumber(nRow+1);
									cell.SetReadOnly(TRUE);
									SetCell(2,newline,&cell);
									if(m_pSimDoc->IsFinishedSimulation())
										QuickSetBackColor(-1,newline,m_ColorCompleted);
									else 
										QuickSetBackColor(-1,newline,m_ColorClick);
									nColValid=2;
									for (int nCol=0;nCol<pTab->GetNumOfCol(); nCol++){
										pCPt=pTab->GetP_Pointer(nCol);

										int view=pCPt->GetViewFunction();
										bool readonly;
										if(view==405||view==406/*||pCPt->IsReadOnly()*/) 
											readonly=TRUE;
										else
											readonly=FALSE;


										if(pCPt==nullptr) break;
										if(pCPt->IsEnabled()) {
											fvalue=pCPt->GetValue(nRow);
											QuickSetNumber(nColValid+1,newline, fvalue);

											DefValue=pCPt->GetOriginalValue(nRow);
											GetCell(nColValid+1,newline, &cell);			
											cell.SetParam(2000+nRow);
											if(pCPt->GetNumOfHistoryIndex(nRow)>0) {
												 cell.SetBackColor(m_ColorHistory);
											}
											else if(DefValue!=fvalue) 
												cell.SetBackColor(m_ColorEdit);
											if(pCPt->IsOldValue(nRow)) cell.SetBackColor(m_ColorChanged);

											if(readonly) 
												cell.SetReadOnly(TRUE);
											else
												cell.SetReadOnly(FALSE);
											SetCell(nColValid+1,newline, &cell);
											nColValid++;
										}
									}
									QuickSetText(0,newline, Group);
									newline++;						
								}
								nColValid=2;
								for (int nCol=0;nCol<pTab->GetNumOfCol(); nCol++){
										pCPt=pTab->GetP_Pointer(nCol);
										//pCPt=((Tab*)Sw)->GetCPtPointer(nCol);
										if(pCPt->IsEnabled()) {
											fvalue=pCPt->GetOriginalValue(0);
											GetCell(nColValid+1,newline,&cell);
											cell.SetNumber(fvalue);
											cell.SetReadOnly(TRUE);
											SetCell(nColValid+1,newline,&cell);

											GetCell(nColValid+1,newline+1,&cell);
											fvalue=pCPt->GetMinValue(0);
											if(fvalue>MISSING) 
												cell.SetNumber(fvalue);
											else 
												cell.SetText("");
											cell.SetReadOnly(TRUE);
											SetCell(nColValid+1,newline+1,&cell);
											GetCell(nColValid+1,newline+2,&cell);
											fvalue=pCPt->GetMaxValue(0);
											if(fvalue>MISSING) 
												cell.SetNumber(fvalue);
											else 
												cell.SetText("");
											cell.SetReadOnly(TRUE);
											SetCell(nColValid+1,newline+2,&cell);

											GetCell(nColValid+1,newline+3,&cell);
											cell.SetBackColor(m_ColorHeading);
											cell.SetText(pCPt->GetUnit().c_str());
											cell.SetReadOnly(TRUE);
											SetCell(nColValid+1,newline+3,&cell);
											nColValid++;
										}
								}
								QuickSetText(2, newline,"Default");
								QuickSetBackColor(2,newline,m_ColorHeading);
								newline++;
								QuickSetText(2, newline,"Min");
								QuickSetBackColor(2,newline,m_ColorHeading);
								newline++;
								QuickSetText(2, newline,"Max");
								QuickSetBackColor(2,newline,m_ColorHeading);
								if(m_pSimDoc->IsFinishedSimulation())
									QuickSetBackColor(-1,newline,m_ColorCompleted);
								else 
									QuickSetBackColor(-1,newline,m_ColorClick);
								newline++;	
								QuickSetText(2, newline,"Unit");
								QuickSetBackColor(2,newline,m_ColorHeading);
								if(m_pSimDoc->IsFinishedSimulation())
									QuickSetBackColor(-1,newline,m_ColorCompleted);
								else 
									QuickSetBackColor(-1,newline,m_ColorClick);

								newline++;	
								JoinCells(0, newline-nrow-4, 1, newline-1);
								if(nColValid<=MaxCol) {
									JoinCells(nColValid+1, newline-nrow-3, MaxCol+2, newline-1);
								}
							}
						}
						else {
// Make Table infor with only one parameter per line:

							Doc *pDoc;
							GetCell(nColValid,newline,&cell);
							cell.SetText("Parameter");
							cell.SetReadOnly(TRUE);
							cell.SetBackColor(m_ColorHeading);
							SetCell(nColValid,newline,&cell);
							UpdateDocumentLink();
							numdoc=m_pSimDoc->m_Selected_All_Documents.size();
							for(int j=0;j<numdoc;j++) {
								pDoc=m_pSimDoc->m_Selected_All_Documents[j];
								GetCell(nColValid+j+1,newline,&cell);
								cell.SetNumber(pDoc->m_DocFile.m_SimulationRunNo);
								cell.SetReadOnly(TRUE);
								cell.SetBackColor(m_ColorHeading);
								SetCell(nColValid+j+1,newline,&cell);
							}

							for(int j=0;j<4;j++) {
								GetCell(nColValid+j+1+numdoc,newline,&cell);
								switch (j) {
									case 0:
										cell.SetText("Default");
										break;
									case 1:
										cell.SetText("Min");
										break;
									case 2:
										cell.SetText("Max");
										break;
									case 3:
										cell.SetText("Unit");
										break;
								}
								cell.SetReadOnly(TRUE);
								cell.SetBackColor(m_ColorHeading);
								SetCell(nColValid+j+1+numdoc,newline,&cell);
							}
							newline++;

							for(int nCol=0; nCol<icol;nCol++) {
								pCPt=pTab->GetP_Pointer(nCol);
								if(pCPt->IsEnabled()) {

									float fval;
									for(int nRow=0; nRow<nrow; nRow++) {
										GetCell(nColValid,newline, &cell);
										cell.SetParam(2000+nRow);
										string st;
										st=pCPt->GetName();st+='(';
										st+=FUtil::STD_ItoAscii(nRow+1);st+=')';
										QuickSetText(nColValid, newline,st);
										fval=pCPt->GetValue(nRow);
										DefValue=pCPt->GetOriginalValue(0);




										GetCell(nColValid+1,newline, &cell);
										cell.SetParam(2000+nRow);
										cell.SetNumber(fval);

										//QuickSetNumber(nColValid+1, newline,  fval);
										SetCell(nColValid+1,newline,&cell);
										if(pCPt->GetNumOfHistoryIndex(nRow)>0) 
											cell.SetBackColor(m_ColorHistory);
										else if(fval!=DefValue) 
											cell.SetBackColor(m_ColorEdit);
										SetCell(nColValid+1,newline,&cell);

										QuickSetText(0,newline,pCPt->GetGroup().c_str());
										QuickSetText(1,newline,pCPt->GetName().c_str());


										P *pCPt1;
										float fval1;
	
										for(int j=1;j<numdoc;j++) {
											pDoc=m_pSimDoc->m_Selected_All_Documents[j];
											pCPt1=m_pSimDoc->GetParTabPointer(pCPt,pDoc);
											if(pCPt!=nullptr) {
												fval1=pCPt1->GetValue(nRow);
												if(fval1!=fval)
													QuickSetNumber(nColValid+1+j, newline, fval1);																						
											}
										}
										for(int j=0;j<3;j++) {
											if(pCPt!=nullptr) {
												switch (j) {
												case 0:
													fval1=DefValue;
													break;
												case 1:
													fval1=pCPt->GetMinValue(0);
													break;
												case 2:
													fval1=pCPt->GetMaxValue(0);
													break;

												}
												QuickSetNumber(nColValid+1+numdoc+j, newline, fval1);
											}
										}
										QuickSetText(nColValid+4+numdoc, newline, pCPt->GetUnit().c_str());
										newline++;
									}
									
								}
					// Set String
							}

						}
					}

//					}


				}


				auto JoinText = [&](size_t col) {
					unsigned int ist, iend;
					ist = 0;
					test = "AA";
					test = QuickGetText(0, 0);
					iend = ist;
					for (int i = 0; i < GetNumberRows(); i++) {
						str = QuickGetText(col, i);
						if (str != test && str != "") {
							JoinCells(col, ist, col, iend);
							ist = i;
							test = str;
						}
						else {
							iend = i;
						}
					}
					if (ist <= iend) {
						JoinCells(col, ist, col, iend);
					}
				};

				JoinText(0);
				JoinText(1);

				RedrawAll();
				pTables.clear();
				SetTopRow(m_CurrentTopRow);
				return TRUE;

}
bool MakeSimSheets::Make_ModelFileSheet()
{
	CUGCell cell; 
	CString heading, number, test;

	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	F *nodef;
	CString str, Defstring;
	CString optionlist;
	SimB *pSw;
	int NumLines;
	int MaxCol=8;
	int colv=0;
	CPtrArray CPG_Array;
	
	BOOL Tableexpand;

	Tableexpand=FALSE;
	EnableMenu(TRUE); 
	//if(m_pSimDoc->p_ModelInfo->IsRunning()) return TRUE;

				SetSheetNumber(4); 
				SetDefFont(&m_font);
				SetNumberCols(8); 
				SetColWidth(0,200);
				SetColWidth(1,150);
				SetColWidth(2,350);
				SetColWidth(3,50);
				SetColWidth(4,100);
				SetColWidth(5,100);
				SetColWidth(6,150);
				SetColWidth(7,150);
				QuickSetBackColor(-1,-1,m_ColorClick);
				if(m_ShowAllFiles)
					QuickSetText(-1,-1,"-");
				else
					QuickSetText(-1,-1,"+");
				QuickSetText(0,-1, "Module");
				QuickSetText(1,-1, "Options");
				QuickSetText(2,-1, "FileName");
				QuickSetText(3,-1, "No Of Variables");
				QuickSetText(4,-1, "No Of Records");
				QuickSetText(5,-1, "Repetitions");
				QuickSetText(6,-1, "Start Date");
				QuickSetText(7,-1, "End Date");
				CPG_Array.RemoveAll();
				m_pSimDoc->UpdateArchiveLists(7);


				NumLines=m_pSimDoc->m_F_Array.size();
				int newpglines=0;
				for(int i=0; i<NumLines; ++i)	{
					pSw=(SimB*)m_pSimDoc->m_F_Array[i];
					nodef=(F*)pSw;
					auto str=nodef->GetStrValue();
					if(nodef->GetHide()!=true) {
						if(nodef->CheckFileNameAndAssignNameToPGClass()){
							int num=m_pSimDoc->m_PlotStorage.CPG_GetNumber();
							CPG *pCPG;
							BOOL validpg=FALSE;
							pCPG=(CPG*)nodef->GetPointer();
							if(pCPG!=nullptr) {
									validpg=TRUE;
									pCPG->ReOpen();
									int  nvar=pCPG->GetNumVariables();
									newpglines+=nvar+1;
										
							}
							if(!validpg) {
								pCPG=new CPG;
								nodef->SetPointer(pCPG);
								BOOL valid=pCPG->ReadContentOfPGFileAndOpenMainPGStreamForReading(str);
								if(valid) {
									int  nvar=pCPG->GetNumVariables();
									newpglines+=nvar+1;
									pCPG->CloseFile();
								}
								else
									delete pCPG;
							}



							pCPG->CloseFile();
						}
					}

				}
				int numflexsumfile;
				numflexsumfile = m_pSimDoc->m_ValsumFlexFiles.size();
				if (numflexsumfile > 0) {
					numflexsumfile++;
				}
				size_t numcsvFile;
				numcsvFile = m_pSimDoc->m_FCSV_Array.size();
				if (numcsvFile > 0) {
					size_t newcsvlines = 1;
					for (size_t i = 0; i < numcsvFile; i++) {
						if (!m_pSimDoc->m_FCSV_Array[i]->CheckIfExistAndOpen()||m_pSimDoc->m_FCSV_Array[i]->GetHide())
							newcsvlines++;
						else
							newcsvlines += m_pSimDoc->m_FCSV_Array[i]->GetNumVariables() + 2;					
					}
					SetNumberRows(NumLines + newpglines + numflexsumfile + newcsvlines);
				}
				else
	  				SetNumberRows(NumLines+newpglines+numflexsumfile); 
				JoinCells(-1,0,-1,NumLines+newpglines-1);
				int ind=-1;
				for(int i=0; i<NumLines; ++i)	{
					pSw=(SimB*)m_pSimDoc->m_F_Array[i];
					nodef=(F*)pSw;
					string str=nodef->GetStrValue();
					Name=pSw->GetName().c_str();
					Group=pSw->GetGroup().c_str();

					if(m_pSimDoc->IsFinishedSimulation()) {
					}
					else {
					}
					ind++;
					for (size_t ic = 0; ic < 8; ic++) UnJoinCells(ic, ind);
					QuickSetText(0,ind, Group);
					QuickSetText(1,ind, Name);

					QuickSetText(2,ind, str);
					QuickSetMask(2,ind,_T(""));
					GetCell(2,ind, &cell);			
					cell.SetParam(1000);
					cell.SetBackColor(RGB(240,240,50));
					SetCell(2,ind, &cell);

					if(((F*)pSw)->CheckFileNameAndAssignNameToPGClass()){

						CPG *pCPG;
						BOOL validpg=FALSE, valid=TRUE;
						pCPG=(CPG*)nodef->GetPointer();

						if(pCPG!=nullptr) {
								validpg=TRUE;

								if(!pCPG->ReOpen()) {
									pCPG->Open(str);
									pCPG->CloseFile();


								};
								string shortname=pCPG->GetFileName(true);
								QuickSetText(2,ind, shortname,true,1000);
								int  nvar=pCPG->GetNumVariables();
									
						}
						if(!validpg) {
							validpg=TRUE;
							pCPG=new CPG;
							nodef->SetPointer(pCPG);
							valid=pCPG->ReadContentOfPGFileAndOpenMainPGStreamForReading(str);
							if(valid) {
								int  nvar=pCPG->GetNumVariables();
								newpglines+=nvar+1;
							}
							else {
								delete pCPG;
								validpg=FALSE;
							}
						}

						if(validpg) {
							auto nrec=pCPG->GetNumRecords();
							auto  nvar=pCPG->GetNumVariables();
							auto pgst=pCPG->GetLongTimeStart();
							auto pgend=pCPG->GetLongTimeEnd();
							QuickSetNumber(3,ind, nvar);
							QuickSetNumber(4,ind, nrec);
							QuickSetNumber(5,ind, pCPG->GetNumRepititions());
							GetCell(6,i,&cell);
							cell.SetTime(0,0,0,1,5,1980);
							cell.SetText(PGUtil::StringDatum(pgst).c_str());
							cell.SetMask("0000-00-00 00:00");
							SetCell(6,ind,&cell);
							cell.SetText(PGUtil::StringDatum(pgend).c_str());
							SetCell(7,ind,&cell);
							if(nodef->GetHide()!=TRUE) {
								//pCPG->GetFileDescription(str);
								ind++;
								for (size_t icol = 0; icol < 7; icol++) UnJoinCells(icol, ind);
								QuickSetText(-1,ind,"No");
								QuickSetText(0,ind,"Variable Name");
								QuickSetText(1,ind,"Unit");
								QuickSetText(2,ind,"Id");
								QuickSetText(3,ind,"Pick");
								QuickSetText(4,ind,"Chart");
								QuickSetText(5,ind,"MinValue");
								QuickSetText(6,ind,"MaxValue");
								for(int j=-1;j<7;j++)
									QuickSetBackColor(j,ind,RGB(240,240,240));
								for(int j=0;j<nvar;j++) {
										ind++;
										QuickSetNumber(-1,ind,j+1);
										GetCell(0,ind,&cell);
										cell.SetText(pCPG->GetVarName(j+1).c_str());
										cell.SetParam(2000);
										cell.SetBackColor(pCPG->GetColor(j+1));
										SetCell(0,ind,&cell);
										GetCell(1,ind,&cell);
										cell.SetText(pCPG->GetVarUnit(j+1).c_str());
										cell.SetParam(2000);
										cell.SetBackColor(pCPG->GetColor(j+1));
										SetCell(1,ind,&cell);
										GetCell(2,ind,&cell);
										cell.SetText(pCPG->GetVarId(j+1).c_str());
										cell.SetParam(2000);
										cell.SetBackColor(pCPG->GetColor(j+1));
										SetCell(2,ind,&cell);
										GetCell(3,ind,&cell);
										cell.SetCellType(UGCT_CHECKBOX);
										cell.SetParam(2100);
										cell.SetBool(pCPG->GetPicked(j+1));
										SetCell(3,ind,&cell);

										QuickSetNumber(5,ind,pCPG->GetVarMin(j+1));
										QuickSetNumber(6,ind,pCPG->GetVarMax(j+1));
								}
							}
							pCPG->CloseFile();

						}

					}



				}

				if (numflexsumfile > 0) {
					ind++;
					QuickSetText(0, ind, "Single Variable Validation File"); QuickSetBackColor(0, ind, m_ColorHeading);
					QuickSetText(2, ind, "Id Variable"); QuickSetBackColor(2, ind, m_ColorHeading);
					QuickSetText(4, ind, "No of Variables"); QuickSetBackColor(4, ind, m_ColorHeading);
					QuickSetText(5, ind, "No of Records"); QuickSetBackColor(5, ind, m_ColorHeading);
					JoinCells(0, ind, 1, ind);	JoinCells(2, ind, 3, ind);		JoinCells(5, ind, 7, ind);
					
					for (size_t i = 0; i < m_pSimDoc->m_ValsumFlexFiles.size(); i++) {
						ind++;
						GetCell(0, ind, &cell);
						cell.SetText(m_pSimDoc->m_ValsumFlexFiles[i].FileName);
						cell.SetParam(2200+i);
						cell.SetBackColor(m_ColorClick);
						SetCell(0, ind, &cell);
		
						QuickSetText(0, ind, m_pSimDoc->m_ValsumFlexFiles[i].FileName);
						GetCell(2, ind, &cell);

						cell.SetCellType(UGCT_DROPLIST);


						optionlist = "";
						for (auto it = m_pSimDoc->MapFlexId[i].begin(); it != m_pSimDoc->MapFlexId[i].end(); it++) {
							string id = (*it).first;
							optionlist += id.c_str();
							optionlist += "\n";
						}
						cell.SetParam(2200 + i);
						cell.SetLabelText(optionlist);
						cell.SetText(m_pSimDoc->m_ValsumFlexFiles[i].IDVarKey);
						cell.SetReadOnly(FALSE);
						SetCell(2, ind, &cell);

						QuickSetNumber(4, ind, m_pSimDoc->MapFlexId[i].size());
						QuickSetNumber(5, ind, m_pSimDoc->MapFlexVal[i].size());
						JoinCells(0, ind, 1, ind);
						JoinCells(2, ind, 3, ind);
						JoinCells(5, ind, 7, ind);

					}

				}
				if (numcsvFile > 0) {
					ind++;
					for (size_t icol = 0; icol < 7; icol++) UnJoinCells(icol, ind);
					QuickSetText(0, ind, "File Id"); QuickSetBackColor(0, ind, m_ColorHeading);
					QuickSetText(1, ind, "File Name"); QuickSetBackColor(1, ind, m_ColorHeading);
					QuickSetText(4, ind, "No of Columns"); QuickSetBackColor(4, ind, m_ColorHeading);
					QuickSetText(5, ind, "No of Lines"); QuickSetBackColor(5, ind, m_ColorHeading);
					JoinCells(1, ind, 3, ind);		JoinCells(5, ind, 7, ind);

					for (size_t i = 0; i < m_pSimDoc->m_FCSV_Array.size(); i++) {
						string koll= m_pSimDoc->m_FCSV_Array[i]->GetName();
						ind++;
						for (size_t icol = 0; icol < 7; icol++) UnJoinCells(icol, ind);
						GetCell(0, ind, &cell);
						cell.SetText(koll);
						cell.SetBackColor(m_NoColor);
						SetCell(0, ind, &cell);
	
						if (!m_pSimDoc->m_FCSV_Array[i]->CheckIfExistAndOpen()) {
							string str;
							if (m_pSimDoc->m_FCSV_Array[i]->GetName().find("ParCalib") == 0) {
								str = "You have Specified Key index for selection of Parameter values to be used for a MultiRun.\n";
								str += "Please specify a file name that should correspond to what you can export from the menu in MultiRun Sheet of the Document";

								MFC_Util::MessageBox(str);
								bool NoUnderScore = false;
								string FileNameParTable = MFC_Util::GetCSVFileName();
								m_pSimDoc->m_FCSV_Array[i]->SetValue(FileNameParTable);
								m_pSimDoc->m_FCSV_Array[i]->CheckFileName(m_pSimDoc);
							}
							else if(m_pSimDoc->m_FCSV_Array[i]->GetName().find("FertTable") == 0) {

								str = "You have to identify a File with the content of Parameters and Values\n";
								str += "The file should correspond to the structure of the Fert Table.";

								MFC_Util::MessageBox(str);
								bool NoUnderScore = false;
								string FileNameParTable = MFC_Util::GetCSVFileName();
								m_pSimDoc->m_FCSV_Array[i]->SetValue(FileNameParTable);
								m_pSimDoc->m_FCSV_Array[i]->CheckFileName(m_pSimDoc);

							}
						}

						koll = m_pSimDoc->m_FCSV_Array[i]->GetStrValue();
						GetCell(1, ind, &cell);
						cell.SetText(koll);
						cell.SetParam(1100 + i);
						cell.SetBackColor(m_ColorClick);
						SetCell(1, ind, &cell);
						QuickSetNumber(4, ind, m_pSimDoc->m_FCSV_Array[i]->GetNumVariables());
						QuickSetNumber(5, ind, m_pSimDoc->m_FCSV_Array[i]->GetNumLines());

						JoinCells(1, ind, 3, ind);		JoinCells(5, ind, 7, ind);

						if (!m_pSimDoc->m_FCSV_Array[i]->GetHide()) {
							ind++;

							QuickSetText(0, ind, "Variable Name"); QuickSetBackColor(0, ind, m_ColorHeading);
							QuickSetText(1, ind, "Min Value"); QuickSetBackColor(1, ind, m_ColorHeading);
							QuickSetText(2, ind, "Max Value"); QuickSetBackColor(2, ind, m_ColorHeading);
							if (m_pSimDoc->m_FCSV_Array[i]->GetNumHeadings() > 1); QuickSetText(3, ind, "Unit"); QuickSetBackColor(3, ind, m_ColorHeading);
							QuickSetBackColor(3, ind, m_ColorHeading);
							UnJoinCells(0, ind);
							UnJoinCells(1, ind);
							UnJoinCells(2, ind);
							UnJoinCells(1 + m_pSimDoc->m_FCSV_Array[i]->GetNumHeadings(), ind);
							JoinCells(2+ m_pSimDoc->m_FCSV_Array[i]->GetNumHeadings(), ind, 7, ind);
							for (size_t il = 0; il < m_pSimDoc->m_FCSV_Array[i]->GetNumVariables(); il++) {
								ind++;
								GetCell(0, ind, &cell);
								cell.SetParam(0);
								SetCell(0, ind, &cell);
								QuickSetText(0, ind, m_pSimDoc->m_FCSV_Array[i]->GetVarHeader(il)); QuickSetBackColor(0, ind, m_NoColor);
								QuickSetNumber(1, ind, m_pSimDoc->m_FCSV_Array[i]->GetVarMin(il)); QuickSetBackColor(1, ind, m_NoColor);
								QuickSetNumber(2, ind, m_pSimDoc->m_FCSV_Array[i]->GetVarMax(il)); QuickSetBackColor(2, ind, m_NoColor);
								if(m_pSimDoc->m_FCSV_Array[i]->GetNumHeadings() > 1); QuickSetText(3, ind, m_pSimDoc->m_FCSV_Array[i]->GetVarHeader_Unit(il)); QuickSetBackColor(2, ind, m_NoColor);
								QuickSetBackColor(2+ m_pSimDoc->m_FCSV_Array[i]->GetNumHeadings(), ind, m_ColorHeading);
								JoinCells(2 + m_pSimDoc->m_FCSV_Array[i]->GetNumHeadings(), ind, 7, ind);
								
							}

						}


					}




				}
		if(m_pSimDoc->IsFinishedSimulation())
			QuickSetBackColor(-1,0,m_ColorCompleted);
		else 
			QuickSetBackColor(-1,0,m_ColorClick);
	return true;
}
bool MakeSimSheets::Make_OutputSheet()
{
		CUGCell cell; 
	string heading, number, test;

	string Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	SimB *pSw;

	string str, Defstring;
	string optionlist;
	int NumLines;
	int LastRow, StartRow, LastRow2, StartRow2;;
	int newline ;
	int MaxCol=8;

	int maxrow=0;
	int MultiIndex;
	CPtrArray PtrArray;
	CUIntArray TypeArray;
	
	BOOL Tableexpand;

	Tableexpand=FALSE;
	EnableMenu(TRUE); 
	m_bDrawExcelFocus = FALSE;
	SetMultiSelectMode(TRUE);
	SetUniformRowHeight(TRUE);
	EnableUpdate(FALSE);
	while(GetNumberRows()>0)
		DeleteRow(0);

	EnableUpdate(TRUE);



	QuickSetBackColor(-1,-1,m_ColorClick);
	SetSheetNumber(5); 
	m_ShowTimeSerieTable=false; // To disable first output option
	if(m_pSimDoc->IsFinishedSimulation()&&m_pSimDoc->MR_Storage_Open()&&m_ShowTimeSerieTable) {
		ShowTimeSerieTableResults();


	}
	else {

		
				PrevGroup=" a";
				LastRow=0;
				StartRow=0;
				PrevGroup2=" a";
				LastRow2=0;
				StartRow2=0;
				SetTH_NumberRows(1);
				SetTH_Height(20); 
				SetSheetNumber(5); 
				m_pSimDoc->OutputBinFileCheck();
				BOOL validplot=m_pSimDoc->IsOutputBinFileValid();
				if(m_pSimDoc->IsFinishedSimulation()) {
					m_ShowAllOutputs=0;
					SetNumberCols(13);

					if(m_pSimDoc->IsMultiSimulation())
						SetColWidth(4,90);
					else
						SetColWidth(4,0);
					SetColWidth(5,28);
					for(int i=6;i<13;i++)
						SetColWidth(i,80);
				}
				else {
					SetNumberCols(6);
					SetColWidth(3, 200);
					SetColWidth(4, 200);
					SetColWidth(5, 150);
				}
				SetDefFont(&m_font);
				SetColWidth(0,100);
				SetColWidth(1,230);
				SetColWidth(2,170);
				SetColWidth(3, 170);

				if(m_ShowAllOutputs)
					QuickSetText(-1,-1,"-");
				else
					QuickSetText(-1,-1,"+");
				QuickSetText(0,-1, "Type");
				QuickSetBackColor(0, -1, m_ColorClick);
				QuickSetText(1,-1, p_ModelInfo->GetGroupCategoryNames(m_ViewCategory));
				QuickSetBackColor(1, -1, m_ColorClick);
				if(m_ShowSoilProfile&&m_ShowPlantVectors&&m_ShowScalar)
					QuickSetText(3,-1, "All Variables");
				else if (m_ShowSoilProfile&&m_ShowPlantVectors&&!m_ShowScalar)
					QuickSetText(3, -1, "Plant and Soil Variables");
				else if (!m_ShowSoilProfile&&!m_ShowPlantVectors && m_ShowScalar)
					QuickSetText(3, -1, "Scalar Variables");
				else if (!m_ShowSoilProfile && m_ShowPlantVectors && m_ShowScalar)
					QuickSetText(3, -1, "Scalar and Plant Variables");
				else if (m_ShowSoilProfile && !m_ShowPlantVectors && m_ShowScalar)
					QuickSetText(3, -1, "Scalar and Soil Variables");
				else if (m_ShowSoilProfile && !m_ShowPlantVectors && !m_ShowScalar)
					QuickSetText(3, -1, "Soil Variables");
				else if (!m_ShowSoilProfile && m_ShowPlantVectors && !m_ShowScalar)
					QuickSetText(3, -1, "Plant Variables");
				else
					QuickSetText(3, -1, "No Variables");
				QuickSetBackColor(2, -1, m_ColorClick);
				QuickSetBackColor(3, -1, m_ColorClick);
				QuickSetBackColor(4, -1, m_ColorClick);
				if (m_ShowProcesses) {
					if(m_Show_All_Elements)
						QuickSetText(2, -1, "Process - All");
					else
						QuickSetText(2, -1, "Process - Selected");

				}
				else {
					if (m_Show_All_Elements)
						QuickSetText(2, -1, "Element - All");
					else
						QuickSetText(2, -1, "Element - Selected");
				}

				if(m_pSimDoc->IsFinishedSimulation()) {
					QuickSetText(4,-1,"Ensemble");
					QuickSetText(5,-1, "Chart");
					QuickSetText(6,-1, "Unit");
					QuickSetText(7,-1, "Initial");
					QuickSetText(8,-1, "Final");
					QuickSetText(9,-1, "Min");
					QuickSetText(10,-1, "Max");
					QuickSetText(11,-1, "Mean");
					QuickSetText(12,-1, "Accumulated");
				    m_pSimDoc->RemoveOriginalValues("Outputs","ALL",false, m_ViewCategory);
					//if(m_pSimDoc->m_SelectedDocuments.size()>1)
					//	QuickSetBackColor(3,-1,m_ColorClick);
					//else
					//	QuickSetBackColor(3,-1,m_ColorHeading);
				}
				else {
					QuickSetText(4, -1, "MultiRun TimeSelect");
					QuickSetText(5,-1, "Unit");

				}
				if(!validplot) {
					QuickSetBackColor(2,-1,m_ColorHeading);
					QuickSetBackColor(3,-1,m_ColorHeading);

				}
				vector<SIMB> vvp;
				NumLines=m_pSimDoc->GetSize_XTGD();

				if(!m_pSimDoc->IsFinishedSimulation()) {
					m_pSimDoc->RemoveOriginalValues("Outputs","ALL",m_ShowAllOutputs, m_ViewCategory);
					m_UpdateOutputList=false;
					vvp = GetAllOutPutVectorByCategory(m_ViewCategory, m_ShowAllOutputs, true);
				}
				else if(m_pSimDoc->IsFinishedSimulation() ){
					//m_pSimDoc->RemoveOriginalValues("Outputs", "ALL", false, m_ViewCategory);
					vvp = GetAllOutPutVectorByCategory(m_ViewCategory, false);
					m_UpdateOutputList = false;
				}
				NumLines = vvp.size();

				bool multivalid;
				if(m_pSimDoc->IsFinishedSimulation())
					multivalid=m_pSimDoc->MR_Storage_Open();
				else
					multivalid=false;
				CString txt;
				size_t ii;

					SetNumberRows(NumLines);
				
					newline=-1;
					MultiIndex=0;

					SetNumberRows(NumLines);
					for(size_t ij=0; ij<vvp.size();ij++) {
					
						pSw=vvp[ij].pBase;	
						ii=vvp[ij].TabIndex;
						int typ=pSw->GetType();
						switch 	(typ){
							case 1:
							case 8:
								Type="State Variables";
								break;
							case 2:
							case 9:
								Type="Flow Variables";
								break;
							case 3:
							case 10:
								Type="Auxiliary Variables";
								break;
							case 4:
							case 11:
								Type="Driving Variables";
								break;
						}	
						OUTPUT SumValue;
						int selected_Single;
						bool selected_Multi;
						int lindex;
						OutSingle *pSingle=nullptr;
						OutVector *pVector=nullptr;
						size_t num_flags=1;
						CString unit;

						pVector=dynamic_cast<OutVector*>(pSw);
						size_t ig=pSw->GetGroupNo();
						if(pVector!=nullptr) {
							if(m_ShowAllOutputs||m_pSimDoc->GetGroupEnable(ig))
								num_flags=pVector->GetNumberOfFlags();
							else
								num_flags=pVector->GetNumberOfSelectedFlags();			
							pSingle=nullptr;
							unit=pVector->GetUnit().c_str();
						}
						else {
							num_flags=1;
							pSingle=dynamic_cast<OutSingle*>(pSw);
							if(pSingle!=nullptr)
							unit=pSingle->GetUnit().c_str();
						}
						if(pSw->Is_Vector()) {
								selected_Single=pVector->GetStoreFlag(ii);
								selected_Multi=false;
								if(selected_Single==2) selected_Multi=true;
								SumValue=((OutVector*)pSw)->GetSumList(ii);
								if(selected_Multi&&newline>=0) {
									MultiIndex++;
									QuickSetNumber(-1,newline,SumValue.PgMultiFileIndex);
									((OutVector*)pSw)->SetMultiPGFileIndex(ii, SumValue.PgMultiFileIndex);
									if(MultiIndex!=SumValue.PgMultiFileIndex&&m_pSimDoc->m_DocFile.m_FileVersionNumber<60) 
										((OutVector*)pSw)->SetMultiPGFileIndex(ii, MultiIndex);
								} 
								else {
									if(SumValue.PgMultiFileIndex!=0) 
									((OutVector*)pSw)->SetMultiPGFileIndex(ii, 0);
								}
							}
						else {
							if (pSingle == nullptr) continue;
							selected_Single=pSingle->GetStoreFlag();
							selected_Multi=false;
							SumValue=((OutSingle*)pSw)->GetSumList();
							if(selected_Single==2) {
										selected_Multi=true;
										MultiIndex++;
										QuickSetNumber(-1,newline,SumValue.PgMultiFileIndex);
										((OutSingle*)pSw)->SetMultiPGFileIndex(SumValue.PgMultiFileIndex);
										if(MultiIndex!=SumValue.PgMultiFileIndex&&m_pSimDoc->m_DocFile.m_FileVersionNumber<60) 
											((OutSingle*)pSw)->SetMultiPGFileIndex(MultiIndex);
							}
						}
						
						if(selected_Single||selected_Multi||m_ShowAllOutputs||m_pSimDoc->GetGroupEnable(pSw->GetGroupNo())) {
								newline++;

								if(m_pSimDoc->IsFinishedSimulation())
									QuickSetBackColor(-1,newline,m_ColorCompleted);
								else 
									QuickSetBackColor(-1,newline,m_ColorClick);
								test=pSw->GetName().c_str();
								if(pSw->Is_Vector()) {
									lindex=((OutVector*)pSw)->GetLocalIndex(ii);
									test+="("+FUtil::STD_ItoAscii(ii+1)+")";
								}

								QuickSetText(1,newline, pSw->GetGroup());
								QuickSetBackColor(1,newline,MFC_Util::GetGroupColor(p_ModelInfo->GetGroupCategoryNo(pSw->GetGroupNo())));

								GetCell(2, newline, &cell);
								string Process;
								if (m_ShowProcesses)
									Process = pSw->GetProcessName().c_str();
								else
									Process = pSw->GetElementName().c_str();
								cell.SetText(Process);
								SetCell(2, newline, &cell);




										
								if(!m_pSimDoc->IsFinishedSimulation()) {
									GetCell(3,newline,&cell);
									cell.SetCellType(UGCT_CHECKBOX);
									cell.SetLabelText(test);
									if (selected_Single==1|| selected_Single == 2) {
										cell.SetBool(TRUE);
									}
									else
										cell.SetBool(FALSE);
									if (selected_Single == 1) 
										cell.SetBackColor(RGB(40, 240, 240));

									
									else if(selected_Single==2) {
										cell.SetBackColor(RGB(0,100,100));
									}
									else 
										cell.SetBackColor(RGB(255,255,255));
									SetCell(3,newline,&cell);
									GetCell(4, newline, &cell);
									cell.SetCellType(UGCT_CHECKBOX);
									if (selected_Single == 2)
										cell.SetBool(TRUE);
									else
										cell.SetBool(FALSE);
									SetCell(4, newline, &cell);

								}
								else {

									GetCell(3,newline,&cell);
									cell.SetCellType(UGCT_CHECKBOX);
									cell.SetBool(FALSE);
									cell.SetLabelText(test);
									cell.SetParam(600);
									SetCell(3,newline, &cell);
								}

								QuickSetText(0,newline, Type);
								if(m_pSimDoc->IsFinishedSimulation()) {
										GetCell(4,newline, &cell);
										cell.SetCellType(UGCT_CHECKBOX);
										cell.SetBool(FALSE);
										cell.SetReadOnly(FALSE);
										if(!validplot&&!selected_Multi) {
											QuickSetBackColor(3,newline,m_ColorHeading);
											QuickSetBackColor(4,newline,m_ColorHeading);
											cell.SetReadOnly(TRUE);
										}
										else if(!m_pSimDoc->IsMultiSimulation()) {
											cell.SetCellType(UGCT_NORMAL);
											
										}
										else if(m_pSimDoc->IsMultiSimulation()&&selected_Multi) {
											QuickSetBackColor(4,newline,m_ColorHeading);
											cell.SetReadOnly(FALSE);
											bool MeanAll;
											if(pSw->Is_Vector()) {
												MeanAll=((OutVector*)pSw)->GetAcceptMeanFlag(lindex);

											}
											else {
												MeanAll=((OutSingle*)pSw)->GetAcceptMeanFlag();

											}
											if(MeanAll)
												cell.SetLabelText("All");
											else
												cell.SetLabelText("Accepted");
										
										}
										else if (m_pSimDoc->IsMultiSimulation()) {
											QuickSetBackColor(3, newline, m_ColorHeading);
											QuickSetBackColor(4, newline, m_ColorHeading);
											cell.SetReadOnly(TRUE);


										}
										SetCell(4,newline,&cell);

										QuickSetText(6,newline, unit);
										GetCell(7,newline, &cell);
										if(multivalid) {
											cell.SetCellType(UGCT_CHECKBOX);
											cell.SetBool(FALSE);
											txt=MFC_Util::FtoAscii(SumValue.Initial);
											cell.SetLabelText(txt);
										}
										else {
											cell.SetNumber(SumValue.Initial);	
										}


										cell.SetAlignment(UG_ALIGNRIGHT);
										SetCell(7,newline, &cell);
										GetCell(8,newline, &cell);
										if(multivalid) {
											cell.SetCellType(UGCT_CHECKBOX);
											cell.SetBool(FALSE);
											txt=MFC_Util::FtoAscii(SumValue.Final);
											cell.SetLabelText(txt);
										}
										else 
											cell.SetNumber(SumValue.Final);
										cell.SetAlignment(UG_ALIGNRIGHT);
										SetCell(8,newline, &cell);
										GetCell(9,newline, &cell);
										if(multivalid) {
											cell.SetCellType(UGCT_CHECKBOX);
											cell.SetBool(FALSE);
											txt=MFC_Util::FtoAscii(SumValue.Min);
											cell.SetLabelText(txt);
										}
										else
											cell.SetNumber(SumValue.Min);
										cell.SetAlignment(UG_ALIGNRIGHT);
										SetCell(9,newline, &cell);
										GetCell(10,newline, &cell);
										if(multivalid) {
											cell.SetCellType(UGCT_CHECKBOX);
											cell.SetBool(FALSE);
											txt=MFC_Util::FtoAscii(SumValue.Max);
											cell.SetLabelText(txt);
										}
										else
											cell.SetNumber(SumValue.Max);
										cell.SetAlignment(UG_ALIGNRIGHT);
										SetCell(10,newline, &cell);
										GetCell(11,newline, &cell);
										if(multivalid) {
											cell.SetCellType(UGCT_CHECKBOX);
											cell.SetBool(FALSE);
											txt=MFC_Util::FtoAscii(SumValue.Mean);
											cell.SetLabelText(txt);
										}
										else
											cell.SetNumber(SumValue.Mean);
										cell.SetAlignment(UG_ALIGNRIGHT);
										SetCell(11,newline, &cell);
										GetCell(12,newline, &cell);
										if(multivalid) {
											cell.SetCellType(UGCT_CHECKBOX);
											cell.SetBool(FALSE);
											txt=MFC_Util::FtoAscii(SumValue.Accumulated);
											cell.SetLabelText(txt);
										}
										else
											cell.SetNumber(SumValue.Accumulated);
										cell.SetAlignment(UG_ALIGNRIGHT);
										SetCell(12,newline, &cell);	
									}
									else 	
										QuickSetText(5,newline, pSw->GetUnit());

							}
				}
				JoinOutputListRows();
// Set CheckBox depending on selections	
				MultiVAL mul;
				for(size_t i=0;i<m_pSimDoc->m_SelectMultiSum.size();i++) {
								mul=m_pSimDoc->m_SelectMultiSum[i];
								GetCell(mul.col,mul.row,&cell);
								cell.SetBool(true);
								//cell.SetBackColor(m_NoColor);
								SetCell(mul.col,mul.row,&cell);
								RedrawCell(mul.col,mul.row);
				}
	}


	if(m_pSimDoc->IsFinishedSimulation())
			QuickSetBackColor(-1,0,m_ColorCompleted);
	else 
			QuickSetBackColor(-1,0,m_ColorClick);
	RedrawAll();
	if(m_CurrentTopRow>=0) SetTopRow(m_CurrentTopRow);

	TypeArray.RemoveAll();
	PtrArray.RemoveAll();
	return true;

}
bool MakeSimSheets::Make_AnnimationSheet()
{
	std::vector <SimB*> v;
	std::string str,group, name, namebase, typ, unit;
	CUGCell cell; 
	CString heading, number, test;
					  
	

	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	SimB *pOut;

	G *pGD;

	CString Defstring;
	CString optionlist;
	int LastRow, StartRow, LastRow2, StartRow2;;
	int newline ;
	int MaxCol=8 ;
	int colv=0;
	int maxrow;

	int typ_st, typ_end;
	CPtrArray PtrArray;
	CUIntArray TypeArray;
	
	
	BOOL Tableexpand;

	Tableexpand=FALSE;
	EnableMenu(TRUE); 
	m_bDrawExcelFocus = FALSE;
	SetMultiSelectMode(FALSE);
	SetUniformRowHeight(TRUE);
	EnableUpdate(FALSE);
	while(GetNumberRows()>0)
		DeleteRow(0);

	EnableUpdate(TRUE);

	QuickSetBackColor(-1,-1,m_ColorClick);

	
	PrevGroup=" a";
	LastRow=0;
	StartRow=0;
	PrevGroup2=" a";
	LastRow2=0;
	StartRow2=0;
	SetTH_NumberRows(1);
	SetTH_Height(20); 
	SetSheetNumber(12); 
	m_pSimDoc->OutputBinFileCheck();
	BOOL validplot=m_pSimDoc->IsOutputBinFileValid();
	SetNumberCols(6);
	SetDefFont(&m_font);
	SetColWidth(0,100);
	SetColWidth(1,230);
	SetColWidth(2,170);
	SetColWidth(4,80);

	if(m_ShowAllOutputs)
		QuickSetText(-1,-1,"-");
	else
		QuickSetText(-1,-1,"+");
	QuickSetText(0,-1, "Type");
	QuickSetText(1,-1, p_ModelInfo->GetGroupCategoryNames(m_ViewCategory));
	QuickSetText(2,-1, "Variable");


		QuickSetText(3,-1, "Unit");
					
		QuickSetText(4,-1, "Pick");
		QuickSetText(5,-1, "Chart");

	
	if(!validplot) {
		QuickSetBackColor(2,-1,m_ColorHeading);
		QuickSetBackColor(3,-1,m_ColorHeading);

	}
	int NumIndex;
	int NewNumLines=0;
	std::string sta;
	sta=m_SelType;
	int id=p_ModelInfo->GetTypeIndex(sta);
	if(id>3&&id<8) {
		switch (id) {
		case 4:
			typ=("State Variables");
			typ_st=0;
			break;
		case 5:
			typ=("Flow Variables");
			typ_st=1;
			break;
		case 6:
			typ=("Auxiliary Variables");
			typ_st=2;
			break;
		case 7:
			typ=("Driving Variables");
			typ_st=3;
			break;
		}
		typ_end=typ_st+1;
		std::string a;
		a=m_SelGroup;
		v=m_pSimDoc->GetPtrVector(typ,a);
	}
	else if(m_SelType=="All") {
		v=m_pSimDoc->GetPtrVector("State Variables","");
		NewNumLines+=v.size();
		v=m_pSimDoc->GetPtrVector("Flow Variables","");
		NewNumLines+=v.size();
		v=m_pSimDoc->GetPtrVector("Auxiliary Variables","");
		NewNumLines+=v.size();
		v=m_pSimDoc->GetPtrVector("Driving Variables","");
		NewNumLines+=v.size();
		typ_st=0;
		typ_end=4;
		m_SelGroup="";
	}
	else {
		typ_st=0;
		typ_end=4;
		m_SelGroup="";
	}
	NewNumLines=0;
	if(m_pSimDoc->AnnimVarList.size()==0) {
			m_pSimDoc->SetDefaultAnnimationSelection();
	}


	for(int i=typ_st; i<typ_end;i++) {
		switch (i) {
		case 0:
			typ=("State Variables");
			break;
		case 1:
			typ=("Flow Variables");
			break;
		case 2:
			typ=("Auxiliary Variables");
			break;
		case 3:
			typ=("Driving Variables");
			break;
		}
		//if(m_SelGroup!="")
		std::string a;
		a=m_SelGroup;
		int num;
		if(m_pSel==nullptr) {
			v=m_pSimDoc->GetPtrVector(typ,a);
		}
		else {
			v.clear();
			v.push_back(m_pSel);
		}
		num=v.size();
		for (int j=0; j<num; j++) {
			if(i<4) {
				pOut=v[j];
				group=pOut->GetGroup();
				namebase=pOut->GetName();
				if(pOut->Is_Vector())
					NumIndex=((OutVector*)pOut)->GetNumberOfFlags();
				else
					NumIndex=1;

			}
			else {
				pGD=(G*)v[j];
				group=pGD->GetGroup();
				namebase=pGD->GetName();
				NumIndex=pGD->GetNumberOfFlags();

			}
			NewNumLines+=NumIndex;
		}
	}

	
				

	SetNumberRows(NewNumLines); 

	maxrow=GetNumberRows();

	newline=0;
	for(int i=typ_st; i<typ_end;i++) {
		switch (i) {
		case 0:
			typ=("State Variables");
			break;
		case 1:
			typ=("Flow Variables");
			break;
		case 2:
			typ=("Auxiliary Variables");
			break;
		case 3:
			typ=("Driving Variables");
			break;
		}
		//if(m_SelGroup!="")

		int num;
		if(m_pSel==nullptr) {
			std::string a;
			a=m_SelGroup;
			v=m_pSimDoc->GetPtrVector(typ,a);
		}
		else {
			v.clear();
			v.push_back(m_pSel);
		}
		num=v.size();
		for (int j=0; j<num; j++) {
			int Ichart;
			int NoIndex=0;
			if(i<4) {
				pOut=v[j];
				group=pOut->GetGroup();
				namebase=pOut->GetName();
				if(pOut->Is_Vector())
					NumIndex=((OutVector*)pOut)->GetNumberOfFlags();
				else
					NumIndex=1;


			}
			else {
				pGD=(G*)v[j];
				group=pGD->GetGroup();
				namebase=pGD->GetName();
				NumIndex=pGD->GetNumberOfFlags();

			}
			unit=((SimB*)v[j])->GetUnit();
			size_t ival=p_ModelInfo->GetGroupCategoryNo(v[j]->GetGroupNo());
			COLORREF groupcolor=MFC_Util::GetGroupColor(int(ival));

			for (int j2=0; j2<NumIndex;j2++) {
				str="";
				name=(namebase);
				QuickSetText(0, newline, typ.c_str());
				QuickSetText(1, newline, group.c_str());
				if(pOut->Is_Vector()) {
					str=('(');
					str+=FUtil::STD_ItoAscii(j2+1);
					str+=(')');
					name+=str;
				}
				if(pOut->Is_Vector()) {
					Ichart=((OutVector*)pOut)->GetAnnimateChart(j2);
					NoIndex=((OutVector*)pOut)->GetAnnimateSequence(j2);
				}
				else {
					Ichart=((OutSingle*)pOut)->GetAnnimateChart();
					NoIndex=((OutSingle*)pOut)->GetAnnimateSequence();

				}

				string chartnumstr=FUtil::STD_ItoAscii(Ichart+1);

				if(Ichart<0)
					chartnumstr="-";
				
				QuickSetText(2,newline, name.c_str());
				QuickSetText(3,newline, unit.c_str());
				QuickSetBackColor(1,newline, groupcolor);
				GetCell(4,newline,&cell);
				cell.SetCellType(UGCT_CHECKBOX);
				if(Ichart>0)
					cell.SetBool(TRUE);	
				else
					cell.SetBool(FALSE);
				SetCell(4,newline, &cell);
				if(Ichart>0)
					QuickSetNumber(5, newline, Ichart);
				else
					QuickSetText(5,newline,"");

				newline++;



			}



		}



	}

	

	for (int i=0;i<maxrow;i++) {
		GetCell(colv, i, &cell);	
		Group=cell.GetText();
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
		Group=cell.GetText();
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

	


	QuickSetBackColor(-1,0,m_ColorClick);
	RedrawAll();
	TypeArray.RemoveAll();
	PtrArray.RemoveAll();
	return true;
}
bool MakeSimSheets::Make_ValidationSheet()
{

	CUGCell cell; 
	CString heading, number, test;

	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;

	CString str, Defstring;
	CString optionlist;
	size_t NumLines;
	int MaxCol=8 ;
	int colv=0;
	int ind, iv;
	int NUM_PERIND;
	NUM_PERIND=7;

	
	BOOL Tableexpand;

	Tableexpand=FALSE;
	EnableMenu(TRUE); 
	m_bDrawExcelFocus = TRUE;
	SetMultiSelectMode(TRUE);
	
	SetSheetNumber(6); 
	NumLines=m_pSimDoc->GetNumAll_TSV_ValVariables();

/*	if (m_pSimDoc->IsMultiSimulation()&& m_pSimDoc->IsFinishedSimulation()) {
		if (m_pSimDoc->MR_GetValidIndex_IndexInValFile_InDim1()!=string::npos) {
			//auto NumberofDim1 = m_pSimDoc->MR_GetDimCounter(0);
			auto NumberofDim1 = m_pSimDoc->m_MultiRun_Array[0].NumberOfRepeatitionsWithinDimension;
			if (NumLines < NumberofDim1) {
				for (size_t i = 0; i < NumLines; i++) {
					auto newValv=m_pSimDoc->m_Val_Array[i];
					for (size_t j = 1; j < NumberofDim1; j++) {
						newValv.ValidationFileIndex = size_t(m_pSimDoc->m_MStorage.GetMultiP(j, m_pSimDoc->MR_GetValidIndex_IndexInValFile_InDim1())+0.001)-1;
						m_pSimDoc->m_Val_Array.push_back(newValv);
					}

				}
				m_pSimDoc->m_MStorage.AllocateValid_TSV(0);


			}
			NumLines = m_pSimDoc->GetNumAll_TSV_ValVariables();
		}
	}*/
	
		SimB *pSw;
		P *pObserved=nullptr;
		pSw=m_pSimDoc->GetPtr(TABLE,"Scaling of Observed Variables");
		pObserved=(P*)m_pSimDoc->GetPtr(PAR_TABLE,"Scaling Observed");
		if(pSw!=nullptr&&NumLines>0) {
				int oldnelements=((Tab*)pSw)->GetNumOfRow();
				if (NumLines!=oldnelements)  {
	  				((Tab*)pSw)->SetNumOfRow(NumLines);

				}
		}
	int NumLinesSV=m_pSimDoc->m_ValidationData.GetNumSumVarVariables();
	int TotLines=NumLines;
	if(NumLinesSV>0&&NumLines>0)
		TotLines=TotLines+1+NumLinesSV;
	else if(NumLinesSV>0)
		TotLines=NumLinesSV;
	SetNumberRows(TotLines); 
	if (pSw != nullptr&&TotLines>0) {
		int oldnelements = ((Tab*)pSw)->GetNumOfRow();
		if (TotLines != oldnelements) {
			((Tab*)pSw)->SetNumOfRow(TotLines);

		}
	}

	if(TotLines==0&&m_pSimDoc->IsFinishedSimulation()) return FALSE;
	JoinCells(-1,0,-1,TotLines-1);
	SetDefFont(&m_font);
	QuickSetBackColor(-1,-1,m_ColorClick);
	m_pSimDoc->m_ValidationData.UpdateValOutIndex();


	if(!m_pSimDoc->IsFinishedSimulation()) {
		Val_Prior_Data(Val_Prior_Header(), NumLinesSV);
	}
	else if(!m_ShowValidRes&&!m_ShowValidCoVarRes&&!m_ShowValidationTimeSerieTable){
	
			Val_Post_Data(Val_Post_Header(),NumLinesSV );
			RedrawAll();
	}
	else if(m_pSimDoc->IsMultiSimulation()&&!m_ShowValidCoVarRes&&!m_ShowValidationTimeSerieTable) {
		if(m_pSimDoc->MR_Storage_Open()) {
			m_bDrawExcelFocus = TRUE;
			SetMultiSelectMode(TRUE);
			unsigned int RunNo,StartRunNo;
			//NumLines=m_pSimDoc->MR_Get_TotalNumberofRuns();
			NumLines=m_pSimDoc->m_MStorage.GetNumAccepted();
			if(NumLines>10000) NumLines=10000;
		//	int ncol=m_pSimDoc->ValidationFilePointer()->GetNumberOfSelectedVal();
			int ncol=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
			int NumLinesSV=m_pSimDoc->m_ValidationData.GetNumSumVarVariables();
			int ncol_extra=3;
			bool SelectMode=FALSE;
			int nextra=0;
			for(int i=0;i<ncol;i++){
					int ifile=m_pSimDoc->m_ValidationData.GetValFileNumber(i);
					if(m_pSimDoc->MR_XBin_IsOpen(ifile)) {
						nextra=6;
						ncol_extra=2+ncol;
						SelectMode=TRUE;
					}
				}
			//if(m_pSimDoc->
			//int i, ii;
			if(ncol>1)
				ncol_extra+=NUM_PERIND;
			SetNumberRows(NumLines+1+nextra);
			SetNumberCols(ncol*NUM_PERIND+ncol_extra+NumLinesSV);
			SetSH_NumberCols(1);
			SetSH_Width(30);
			SetTH_NumberRows(1);
			SetTH_Height(34);
			SetColWidth(-1,20);
			SetColWidth(0,60);
			SetColWidth(1,60);
			for(int i=2;i<ncol*6+ncol_extra;i++)
                SetColWidth(i,60);
			if(SelectMode) {
				for(int i=2;i<ncol+2;i++) {
					SetColWidth(i,30);
					SetColWidth(i,30);
				}
			}


			RunNo=m_pSimDoc->m_DocFile.m_SimulationRunNo-m_pSimDoc->MR_Get_ActualRunNumber();
			StartRunNo=RunNo;
			

			CString str;
			ind=0;
			int ipo;
			for(int i=0;i<NumLines+1+nextra;i++) {
				UnJoinCells(-1,i);
			}
			JoinCells(-1,0,-1,NumLines+nextra);


			for(int ii=0;ii<NUM_PERIND;ii++) {
				ipo=ind;
				for(int i=0;i<ncol;i++){
					ind++; 
					GetCell(ind+ncol_extra-1,-1,&cell);
					test=m_pSimDoc->m_ValidationData.GetValName(i).c_str();
					int iv=m_pSimDoc->m_ValidationData.GetValLocalIndex(i);
					if(iv>-1) test+=_T(" (")+MFC_Util::ItoAscii(iv+1)+_T(")");
					cell.SetText(test);
					cell.SetCellTypeEx(UGCT_NORMALMULTILINE);
					cell.SetBackColor(m_ColorHeading);
					SetCell(ind+ncol_extra-1,-1,&cell);
				}
				JoinCells(ipo+ncol_extra,0,ipo+ncol+ncol_extra-1,0);
				switch(ii) {
				case 0:  str="R2"; break;
				case 1:  str="A0"; break;
				case 2:  str="A1"; break;
				case 3:  str="ME"; break;
				case 4:  str="RMSE"; break;
				case 5:  str="LogLi"; break;
				case 6:  str="NSE R2"; break;
			
				}
				QuickSetText(ipo+ncol_extra,0,str);
				if(ncol>1) {
					if(ii==0) {
						QuickSetText(ncol_extra-7,-1,"Sum of All");
						JoinCells(-7+ncol_extra,-1,-1+ncol_extra,-1);
						QuickSetBackColor(6+ncol_extra-1,-1, m_ColorHeading);
					}
					QuickSetText(ii-7+ncol_extra,0,str);
					int ngrey=7;
					if(nextra==0) ngrey=1;
					for(int i=0;i<ngrey;i++)
						QuickSetBackColor(ii-7+ncol_extra,i, m_ColorHeading);
				}
				QuickSetBackColor(ipo+ncol_extra,0, m_ColorHeading);
			}
			ipo=ind;
			for(int i=0;i<NumLinesSV;i++){
				test=m_pSimDoc->m_ValidationData.GetValSumVarName(i).c_str();
				ind++; QuickSetText(ind+ncol_extra-1,-1, test);
				QuickSetBackColor(ind+ncol_extra-1,-1, m_ColorHeading);
			}
			JoinCells(ipo+ncol_extra-1,0,ipo+NumLinesSV+ncol_extra,0);
			if(NumLinesSV>0) {
				str="Mean Error (Sum Var)";
				QuickSetText(ipo+ncol_extra,0,str);
				QuickSetBackColor(ipo+ncol_extra,0, m_ColorHeading);
			}
			if(!SelectMode) {
				QuickSetText(ipo+2+ncol,-1,"Accepted");
				QuickSetBackColor(ipo+2+ncol,-1, m_ColorHeading);
			}


		
			QuickSetText(0,-1,"");
			QuickSetText(2,-1,"Pick");

			for(int i=0;i<ncol;i++) {
				GetCell(2+i,1,&cell);
				cell.SetCellTypeEx(UGCT_NORMALMULTILINE);
				test=m_pSimDoc->m_ValidationData.GetValName(i).c_str();
				int iv=m_pSimDoc->m_ValidationData.GetValLocalIndex(i);
				if(iv>-1) test+=_T("(")+MFC_Util::ItoAscii(iv+1)+_T(")");
				str=MFC_Util::ItoAscii(i+1)+_T(" - ")+test;
				int nv=str.GetLength();
				int j=7;
				while(j<nv) {
					str.Insert(j,' ');
					nv=str.GetLength();
					j=j+4;
				}
				cell.SetBackColor(m_ColorHeading);
				cell.SetText(str);
				SetCell(2+i,1,&cell);
				QuickSetBackColor(2+i,5,m_ColorHeading);
				JoinCells(2+i,1,2+i,5);
			}
			QuickSetText(0,0,"Accepted= "+FUtil::STD_ItoAscii(int(m_pSimDoc->m_MStorage.GetNumAccepted()))+"("+FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_TotalNumberofRuns())+")");
			if(m_ShowValidRes_Changed) {
				QuickSetText(0,-1,"To Be Updated");
				QuickSetBackColor(0,-1,m_ColorChanged);	
			}


			if(nextra>0) {

				QuickSetText(0,3,"Limits");
				QuickSetText(1,3,"Low");
				QuickSetText(1,4,"High");	
				QuickSetText(0,1,"All");
				QuickSetText(1,1,"Min");
				QuickSetText(1,2,"Max");

				JoinCells(0,-1,1,-1);
				JoinCells(0,0,1,0);
				JoinCells(2,-1,ncol+1,-1);

	
				/*JoinCells(1,1,ncol+1,1);
				JoinCells(0,2,ncol+1,2);
				JoinCells(1,2,ncol+1,2);
				JoinCells(1,3,ncol+1,3);
				JoinCells(1,4,ncol+1,4);*/
				//JoinCells(1,4,ncol+3,4);
				QuickSetText(0,-1,"");
				QuickSetText(2,-1,"Pick");

				for(int i=0;i<ncol;i++) {
					GetCell(2+i,1,&cell);
					cell.SetCellTypeEx(UGCT_NORMALMULTILINE);
					test=m_pSimDoc->m_ValidationData.GetValName(i).c_str();
					int iv=m_pSimDoc->m_ValidationData.GetValLocalIndex(i);
					if(iv>-1) test+=_T("(")+MFC_Util::ItoAscii(iv+1)+_T(")");
					str=MFC_Util::ItoAscii(i+1)+_T(" - ")+test;
					int nv=str.GetLength();
					int j=7;
					while(j<nv) {
						str.Insert(j,' ');
						nv=str.GetLength();
						j=j+4;
					}
					cell.SetBackColor(m_ColorHeading);
					cell.SetText(str);
					SetCell(2+i,1,&cell);
					QuickSetBackColor(2+i,5,m_ColorHeading);
					JoinCells(2+i,1,2+i,5);
				}
				QuickSetText(0,0,"Accepted= "+FUtil::STD_ItoAscii(int(m_pSimDoc->m_MStorage.GetNumAccepted()))+"("+FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_TotalNumberofRuns())+")");
				if(m_ShowValidRes_Changed) {
					QuickSetText(0,-1,"To Be Updated");
					QuickSetBackColor(0,-1,m_ColorChanged);	
				}
				else {
					QuickSetText(0,-1,"Updated");
					QuickSetBackColor(0,-1,m_ColorCompleted);
				}
					

				QuickSetText(0,6,"Measured");
				JoinCells(0,6,1,6);
				QuickSetText(0,5,"Multi #");
				QuickSetText(1,5,"Run #");
				QuickSetBackColor(0,5, m_ColorHeading);
				QuickSetBackColor(1,5, m_ColorHeading);
			}



			else {
				QuickSetText(0,-1,"Updated");
				QuickSetBackColor(0,-1,m_ColorCompleted);
			}
			



				for(int ii=0;ii<nextra;ii++) {
					QuickSetBackColor(0,1+ii, m_ColorHeading);
					QuickSetBackColor(1,1+ii, m_ColorHeading);
					ind=0;
					float range;
					BOOL locked=m_pSimDoc->m_MStorage.m_Lock;
					for(iv=0;iv<NUM_PERIND;iv++) {
				
						if(ncol>1) {
							switch(ii) {
								case 0:
									QuickSetNumber(ncol_extra+iv-NUM_PERIND,1,double(m_pSimDoc->m_MStorage.m_MeanTSV_STAT[POST_MIN][iv]));
									break;
								case 1:
									QuickSetNumber(ncol_extra + iv - NUM_PERIND, 1, double(m_pSimDoc->m_MStorage.m_MeanTSV_STAT[POST_MAX][iv]));
									break;
								case 2:
									if(!locked) {
										QuickSetNumber(ncol_extra + iv - NUM_PERIND, 3, double(m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_LOW][iv]));
										if(m_pSimDoc->m_MStorage.m_MeanTSV_STAT[PRIOR_MIN][iv]   <m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_LOW][iv])
											QuickSetBackColor(ncol_extra + iv - NUM_PERIND, 3, m_ColorChanged);		
									}
									break;
								case 3:
									if(!locked) {
										QuickSetNumber(ncol_extra + iv - NUM_PERIND, 4, double(m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_HIGH][iv]));
										if (m_pSimDoc->m_MStorage.m_MeanTSV_STAT[PRIOR_MAX][iv]   <m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_HIGH][iv])
											QuickSetBackColor(ncol_extra + iv - NUM_PERIND, 4, m_ColorChanged);
									}
									break;
							}
						}	
						
						for(int i=0; i<ncol;i++) {
							if(ii==0)
								QuickSetNumber(ncol_extra+ind,1,double(m_pSimDoc->m_MStorage.GetMinValid(ind))); 
							else if(ii==1)
								QuickSetNumber(ncol_extra+ind,2,double(m_pSimDoc->m_MStorage.GetMaxValid(ind))); 
							else if(ii==2&&!locked) {
								QuickSetNumber(ncol_extra+ind,3,double(m_pSimDoc->m_MStorage.GetValidLow(ind))); 
								if(m_pSimDoc->m_MStorage.GetMinValidAll(ind)!=m_pSimDoc->m_MStorage.GetValidLow(ind))
									QuickSetBackColor(ncol_extra+ind,3,m_ColorChanged);
							}
							else if(ii==3&&!locked) {
								QuickSetNumber(ncol_extra+ind,4,double(m_pSimDoc->m_MStorage.GetValidHigh(ind))); 
								if(m_pSimDoc->m_MStorage.GetMaxValidAll(ind)!=m_pSimDoc->m_MStorage.GetValidHigh(ind))
									QuickSetBackColor(ncol_extra+ind,4,m_ColorChanged);
							}
							else if(ii==4&&!locked) {
								range=(m_pSimDoc->m_MStorage.GetMaxValid(ind)-m_pSimDoc->m_MStorage.GetMinValid(ind))/
										(m_pSimDoc->m_MStorage.GetMaxValidAll(ind)-m_pSimDoc->m_MStorage.GetMinValidAll(ind));
								QuickSetNumber(ncol_extra+ind,5, double(range));
								QuickSetBackColor(ncol_extra+ind,ii+1, m_ColorHeading);
							}
							//if(iv<6)
							ind++;
						}
					}
					for(iv=0; iv<NumLinesSV;iv++) {
							if(ii==0)
								QuickSetNumber(ncol_extra+ind,1,double(m_pSimDoc->m_MStorage.GetMinSumValid(iv))); 
							else if(ii==1)
								QuickSetNumber(ncol_extra+ind,2,double(m_pSimDoc->m_MStorage.GetMaxSumValid(iv))); 
							else if(ii==2&&!locked) {
								QuickSetNumber(ncol_extra+ind,3,double(m_pSimDoc->m_MStorage.GetValidSumLow(iv))); 
								if(m_pSimDoc->m_MStorage.GetMinSumValid(ind)!=m_pSimDoc->m_MStorage.GetValidSumLow(iv))
									QuickSetBackColor(ncol_extra+ind,3,m_ColorChanged);
							}
							else if(ii==3&&!locked) {
								QuickSetNumber(ncol_extra+ind,4,double(m_pSimDoc->m_MStorage.GetValidSumHigh(iv))); 
								if(m_pSimDoc->m_MStorage.GetMaxSumValid(ind)!=m_pSimDoc->m_MStorage.GetValidSumHigh(iv))
									QuickSetBackColor(ncol_extra+ind,4,m_ColorChanged);
							}
							else if(ii>=4&&!locked) {
								range=(m_pSimDoc->m_MStorage.GetMaxSumValid(ind)-m_pSimDoc->m_MStorage.GetMinSumValid(ind))/
										(m_pSimDoc->m_MStorage.GetMaxSumValidAll(ind)-m_pSimDoc->m_MStorage.GetMinSumValidAll(ind));
								QuickSetNumber(ncol_extra+ind,5, double(range));
								QuickSetBackColor(ncol_extra+ind,ii+1, m_ColorHeading);
							}
							ind++;


					}

				}
				
				for(int i=2;i<ncol+2;i++) {
						BOOL testV=FALSE;
						GetCell(i,0,&cell);
						cell.SetCellType(UGCT_CHECKBOX);
						if(testV)
							cell.SetBool(TRUE);
						else
							cell.SetBool(FALSE);
						SetCell(i,0,&cell);
				}
				for(int i=2;i<ncol+2;i++) {
						BOOL testV=FALSE;
						GetCell(i,6,&cell);
						cell.SetCellType(UGCT_CHECKBOX);
						if(testV)
							cell.SetBool(TRUE);
						else
							cell.SetBool(FALSE);
						SetCell(i,6,&cell);
				}
			QuickSetText(0,6,"Measured");
			JoinCells(0,6,1,6);
			QuickSetText(0,5,"Multi #");
			QuickSetText(1,5,"Run #");
			QuickSetBackColor(0,5, m_ColorHeading);
			QuickSetBackColor(1,5, m_ColorHeading);
			
			
			QuickSetText(0,-1,"Multi #");
			QuickSetText(1,-1,"Run #");
		
			for(int ii=0;ii<NumLines;ii++) {
				auto irun=m_pSimDoc->m_MStorage.Get_AcceptedRuns(ii);
				QuickSetNumber(0,ii+1+nextra,double(irun+1));
				RunNo=StartRunNo+irun+1;
				QuickSetNumber(1,ii+1+nextra,double(RunNo));
				ind=0;
				float fvalue;
				for(iv=0;iv<NUM_PERIND;iv++) {
						if(ncol_extra>3) {
							switch(iv) {
								case 0:
									fvalue=m_pSimDoc->m_MStorage.GetMeanValid_R2(irun);
									break;
								case 1:
									fvalue=m_pSimDoc->m_MStorage.GetMeanValid_A0(irun);
									//QuickSetNumber(ncol_extra+iv-NUM_PERIND,ii+1+nextra,double(m_pSimDoc->m_MStorage.GetMeanValid_A0(irun))); 
									break;
								case 2:
									fvalue=m_pSimDoc->m_MStorage.GetMeanValid_A1(irun);
								//	QuickSetNumber(ncol_extra+iv-NUM_PERIND,ii+1+nextra,double(m_pSimDoc->m_MStorage.GetMeanValid_A1(irun))); 
									break;
								case 3:
									fvalue=m_pSimDoc->m_MStorage.GetMeanValid_AbsME(irun);
									//QuickSetNumber(ncol_extra+iv-NUM_PERIND,ii+1+nextra,double(m_pSimDoc->m_MStorage.GetMeanValid_AbsME(irun))); 
									break;
								case 4:
									fvalue=m_pSimDoc->m_MStorage.GetMeanValid_RMSE(irun);
									//QuickSetNumber(ncol_extra+iv-NUM_PERIND,ii+1+nextra,double(m_pSimDoc->m_MStorage.GetMeanValid_RMSE(irun))); 
									break;
								case 5:
									fvalue=m_pSimDoc->m_MStorage.GetSumValid_LogLi(irun);
									//QuickSetNumber(ncol_extra+iv-NUM_PERIND,ii+1+nextra,double(m_pSimDoc->m_MStorage.GetSumValid_LogLi(irun))); 
									break;
								case 6:
									fvalue=m_pSimDoc->m_MStorage.GetMeanValid_NSE(irun);
									//QuickSetNumber(ncol_extra+iv-NUM_PERIND,ii+1+nextra,double(m_pSimDoc->m_MStorage.GetMeanValid_NSE(irun))); 
									break;


							}
							if(fvalue>MISSING)
								QuickSetNumber(ncol_extra+iv-NUM_PERIND,ii+1+nextra,fvalue); 
							else
								QuickSetText(ncol_extra+iv-NUM_PERIND,ii+1+nextra,"");
						}

						for(int i=0; i<ncol;i++) {
							if(iv==NUM_PERIND-1) {
								int ind_rmse;
								float rmse, rmse_m, re_2;
								ind_rmse=(iv-2)*ncol+i;
								rmse=m_pSimDoc->m_MStorage.GetValid(irun,ind_rmse);
								rmse_m=m_pSimDoc->m_ValidationData.GetValRMSE_O(i);
								if(rmse>MISSING) {
									if(rmse_m>0) {
										re_2=1-(rmse*rmse*m_pSimDoc->m_ValidationData.GetValn(i))/(rmse_m);
										QuickSetNumber(ncol_extra+6*ncol+i,ii+1+nextra,double(re_2)); 
									}
									else
										re_2=float(MISSING);
								}
								else 
									QuickSetText(ncol_extra+6*ncol+i,ii+1+nextra,"");	
							}
							else {
								fvalue=m_pSimDoc->m_MStorage.GetValid(irun,ind);
								if(fvalue>MISSING)
									QuickSetNumber(ncol_extra+ind,ii+1+nextra,fvalue); 
								else
									QuickSetText(ncol_extra+ind,ii+1+nextra,"");
							
								if(i>0) {

									if(m_pSimDoc->m_MStorage.GetValid(irun,ind)==m_pSimDoc->m_MStorage.GetValid(irun-1,ind)) {
										float v=m_pSimDoc->m_MStorage.GetValid(irun,ind);
									}
								}
								ind++;
							}
						}
					}

				//if(ncol==0) ind++;
				for(int i=0; i<NumLinesSV;i++) {
						QuickSetNumber(ncol_extra+ind,ii+1+nextra,double(m_pSimDoc->m_MStorage.GetValidSum(irun, i*3) ) ); 
						ind++;
				}

				if(SelectMode) {

					for(int i=2;i<ncol+2;i++) {
						BOOL testV=FALSE;
						GetCell(i,ii+1+nextra,&cell);
						cell.SetCellType(UGCT_CHECKBOX);
						if(testV)
							cell.SetBool(TRUE);
						else
							cell.SetBool(FALSE);
						SetCell(i,ii+1+nextra,&cell);
					}
				}
				else if(!SelectMode)
					QuickSetNumber(ncol_extra+ind,ii+1+nextra,double(m_pSimDoc->m_MStorage.GetMultiP(irun,m_pSimDoc->MR_Get_NumTotalPar())));
			}
			
		}
	
		RedrawAll();

	}
	else if(m_ShowValidationTimeSerieTable) {
		Val_Post_TimeSerieTable(NumLines,NumLinesSV);
		

	}
	else if(m_ShowValidCoVarRes) {	
		Val_Post_CoVarRes();

	}
	if(m_pSimDoc->IsFinishedSimulation())
			QuickSetBackColor(-1,0,m_ColorCompleted);
	else 
			QuickSetBackColor(-1,0,m_ColorClick);
	return true;
}
bool MakeSimSheets::Make_RunReportSheet()
{
		CUGCell cell; 
	string heading, number, test;

	string Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	string str, Defstring;
	string optionlist;
	int NumLines;
	int MaxCol=8 ;
	int colv=0;

				int prelines=0;
				SetSheetNumber(7); 
				SetNumberCols(3); 
				SetDefFont(&m_font);
					SetColWidth(0,500);
					SetColWidth(1,100);
					SetColWidth(2,150);
			    
					QuickSetText(0,-1, "Message");
					QuickSetText(1,-1, "-");
					QuickSetText(2,-1, "-");

					prelines=0;

	
				NumLines=m_pSimDoc->m_Report_Array.size();
				SetNumberRows(NumLines+prelines); 
				for(int i=0; i<NumLines; ++i)
				{
					if(m_pSimDoc->IsFinishedSimulation())
						QuickSetBackColor(-1,i,m_ColorCompleted);
					else 
						QuickSetBackColor(-1,i,m_ColorClick);
					QuickSetText(0,i+prelines, m_pSimDoc->m_Report_Array[i]);
				}
				JoinCells(-1,0,-1,NumLines-1);
				RedrawAll();

	return true;
}
bool MakeSimSheets::Make_AttachedFileSheet()
{
	int numrows = 1;

	SetSheetNumber(13);

	if (!m_pSimDoc->IsFinishedSimulation()) {
		QuickSetText(0, -1, "");
		return true;
	}

	SetNumberRows(numrows);
	auto str = m_pSimDoc->GetMBinFileName();

	int ifiles = 0;
	if (m_pSimDoc->IsMultiStorageBinFileValid())
		ifiles = m_pSimDoc->m_MStorage.GetNumberOfAddedFiles(string(str));
	numrows = ifiles;

	SetNumberRows(numrows+1);
	SetNumberCols(3);
	SetColWidth(0, 700);
	SetColWidth(1, 150);
	SetColWidth(2, 150);
	QuickSetText(-1, -1, "#");
	string header;
	if (ifiles == 0)
		header = "No Attached files";
	else {
		header = FUtil::STD_ItoAscii(ifiles);
		header += " Attached Multiruns with in total ";
		header += FUtil::STD_ItoAscii(size_t(m_pSimDoc->m_MStorage.GetNumberOfTotalRunsWithAttachedFiles())) + " runs and ";
		header += FUtil::STD_ItoAscii(size_t(m_pSimDoc->m_MStorage.GetNumAccepted())) + " accepted";
	}
	QuickSetText(0, -1, header);
	QuickSetText(1, -1, "First RunNo");
	QuickSetText(2, -1, "Number of Runs");

	if (ifiles>0) {
		str = m_pSimDoc->GetMBinFileName();
		QuickSetText(0, 0, str);
		auto RunNo = m_pSimDoc->m_DocFile.m_SimulationRunNo - m_pSimDoc->MR_Get_ActualRunNumber();
		QuickSetNumber(1,  0, RunNo+1);
		QuickSetNumber(2, 0, m_pSimDoc->MR_Get_NumberofRuns());
		QuickSetNumber(-1, 0, 0);

		for (int i = 0; i<ifiles; i++) {
			QuickSetNumber(-1, i+1, i+1);
			str = m_pSimDoc->m_MStorage.GetNameAddedFiles(i);
			QuickSetText(0, i+1, str);
			auto firstn=m_pSimDoc->m_MStorage.GetFirstRunNoAddedRuns(i);
			auto lastn = m_pSimDoc->m_MStorage.GetNumberOfAddedRuns(i);
			QuickSetNumber(1, i+1, firstn);
			QuickSetNumber(2, i+1, lastn);
		}
	}


	
	return true;
}
bool MakeSimSheets::Make_ChartListSheet()
{
	CUGCell cell; 
	string test;
	int index, NumLines ;


	m_bDrawExcelFocus = FALSE;
	SetMultiSelectMode(FALSE);
	SetUniformRowHeight(TRUE);

	int prelines=0;
	return true;
	//if(m_pSimDoc->p_ModelInfo->IsRunning()) return TRUE;
	SetSheetNumber(8); 
	if(m_ShowChartTable) {
		if(m_ShowChartTableNo<0)
			m_ShowChartTableNo=m_pSimDoc->m_PlotStorage.GetCurrentChart();
		}
				
	if(m_ShowChartTable&&m_ShowChartTableNo>0) {

			int varnum;
			int recordnum;
			m_bDrawExcelFocus = TRUE;
			SetMultiSelectMode(TRUE);

			varnum=m_pSimDoc->m_PlotStorage.GetNumVar(m_ShowChartTableNo);
	
			SetColWidth(-1,50);
			SetColWidth(0,100);

			SetTH_NumberRows(4);
			SetTH_Height(80); 
			SetTH_RowHeight(-1,20); 
			SetTH_RowHeight(-2,20);
			SetTH_RowHeight(-3,20);
			SetTH_RowHeight(-4,20);
			SetColWidth(0,120);
			SetColWidth(1,90);
			JoinCells(0,-4,0,-1);
			QuickSetBackColor(-1,-1,m_ColorClick);
			QuickSetText(-1,-1,"-");
			QuickSetText(0,-4, "Date");
			BOOL multiRunPlot;
			vector<int> PG_indexArray;
			CPtrArray PG_pointerArray;
			//int PG_index;
			CString str;
			CPG *pPG;
			CPG *pPGTime;
			SimB *pX;
			BOOL Band, BandPG;
			BOOL MultiMode=FALSE;
			BOOL ResidualSource;
			BOOL MeanOfAll;
			unsigned int ResRunNo;
			CResiduals *pRes;

			int itype;
			int valfilno;
			int extracol=0;
			int nindex=0;
			ResidualSource=FALSE;
			BandPG=FALSE;






			for(int i=0;i<varnum;i++) {
				MeanOfAll=TRUE;
				SetColWidth(i+1,100);
				
				index=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_ShowChartTableNo, i);
				if(m_pSimDoc->m_PlotStorage.GetRepitType(index)) MultiMode=TRUE;
				int pgindex;
				pgindex=m_pSimDoc->m_PlotStorage.GetVar_Index(index);
				
				PG_indexArray.push_back(pgindex);
				multiRunPlot=m_pSimDoc->m_PlotStorage.IsMultiRunPlot(index);
				int idoc=m_pSimDoc->m_PlotStorage.GetDocIndex(index);
				int IndexVal=m_pSimDoc->m_PlotStorage.GetValVarNo(index);

				int FileNo=m_pSimDoc->m_PlotStorage.GetValFileNo(index);
				if(m_pSimDoc->MR_XBin_IsOpen(FileNo)) {
					pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(FileNo);
					ResRunNo=m_pSimDoc->m_PlotStorage.GetValRunNo(index);
					if(ResRunNo==140000) 
						extracol+=4;
				}
			}
			

			auto ens_type = m_pSimDoc->m_PlotStorage.GetEnsembleType(index);
			auto data_type = m_pSimDoc->m_PlotStorage.GetDataType(index);

			if(multiRunPlot)
					recordnum=m_pSimDoc->MR_Get_TotalNumberofRuns();
			else {
				int idoc=m_pSimDoc->m_PlotStorage.GetDocIndex(index);
				int FileNo=m_pSimDoc->m_PlotStorage.GetValFileNo(index);
				
				if(MultiMode)
					pPGTime=m_pSimDoc->m_PlotStorage.GetPGPointer(idoc,FileNo, TRUE,TRUE);
				else if(ens_type == ENSEMBLE_TYPE::ALL){
					pPGTime=m_pSimDoc->m_PlotStorage.GetPGPointer(idoc,FileNo, TRUE,FALSE,1);
					extracol+=3;
				}
				else if(ens_type == ENSEMBLE_TYPE::ACCEPTED) {
					pPGTime=m_pSimDoc->m_PlotStorage.GetPGPointer(idoc,FileNo, TRUE,FALSE,2);
					extracol+=3;
				}
				else if(ens_type == ENSEMBLE_TYPE::ACCEPTED&& data_type==DATA_TYPES::SIM_VAL) {
					string name;
					pPGTime=m_pSimDoc->m_PlotStorage.m_PG_SEL_MULTI[FileNo];
					name=pPGTime->GetFileName();
					if(name=="MyNewFile") {
						pPGTime=m_pSimDoc->m_PlotStorage.GetPGPointer(idoc,FileNo);
						name=pPGTime->GetFileName();
					}
					Band=TRUE;
					MeanOfAll=FALSE;
					if(m_pSimDoc->MR_XBin_IsOpen(FileNo)) {
						ResidualSource=TRUE;
						pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(FileNo);
					}
					if(pPGTime==nullptr) return FALSE;
					extracol+=4;
				}
				else if(data_type== DATA_TYPES::SIM_VAL){
					pPGTime=m_pSimDoc->m_PlotStorage.GetPGPointer(idoc,FileNo);
					extracol+=4;
				}
				else
					pPGTime=m_pSimDoc->m_PlotStorage.GetPGPointer(idoc,FileNo);

				if(pPGTime!=nullptr) {
					if(!pPGTime->ReOpen()) {
							string str=pPGTime->GetFileName();
							pPGTime->Open(str);
							recordnum=pPGTime->GetNumRecords();
						//	pPG->GetFileDescription();						
					}
				}
				else
					return FALSE;



			
			}
			SetNumberCols(varnum+1+extracol); 
			SetNumberRows(recordnum);



		//	CPG *pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(m_ShowChartTableNo);
			for(int i=0;i<varnum;i++) {
				MeanOfAll=TRUE;
				SetColWidth(i+1,100);
				
				index=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_ShowChartTableNo, i);
				int pgindex;
				pgindex=m_pSimDoc->m_PlotStorage.GetVar_Index(index);
				PG_indexArray.push_back(pgindex);
				multiRunPlot=m_pSimDoc->m_PlotStorage.IsMultiRunPlot(index);
				auto datatype=m_pSimDoc->m_PlotStorage.GetDataType(index);
				auto ens_type= m_pSimDoc->m_PlotStorage.GetEnsembleType(index);
				int idoc=m_pSimDoc->m_PlotStorage.GetDocIndex(index);
				int IndexVal=m_pSimDoc->m_PlotStorage.GetValVarNo(index);

				if(!multiRunPlot) {
					void *Ptr=m_pSimDoc->m_PlotStorage.GetPtr(index);

					if(datatype==DATA_TYPES::PGDATA)
						pPG=(CPG*)Ptr;
					else if(datatype== DATA_TYPES::ANYDATA) {
						pPG=(CPG*)((F*)Ptr)->GetPointer();

					}
					else if(datatype== DATA_TYPES::SIM_VAL&&ens_type==ENSEMBLE_TYPE::ALL) {
						pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(idoc,0, TRUE,FALSE,1);
						valfilno=m_pSimDoc->m_PlotStorage.GetValFileNo(index);
					}
					
					else if(datatype == DATA_TYPES::SIM_VAL&&ens_type == ENSEMBLE_TYPE::ACCEPTED) {
						pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(idoc,0, TRUE,FALSE,2);
						valfilno=m_pSimDoc->m_PlotStorage.GetValFileNo(index);
					}
					else {
						pX=(SimB*)Ptr;	
						valfilno=m_pSimDoc->m_PlotStorage.GetValFileNo(index);
						int idoc=m_pSimDoc->m_PlotStorage.GetDocIndex(index);
						pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(idoc, valfilno);

					}

					if(ens_type == ENSEMBLE_TYPE::ALL) {
						if(valfilno==0) {
							pPG=pPGTime;
							BandPG=TRUE;
						}
						else {
							pPG=m_pSimDoc->m_PlotStorage.m_PG_ALLMULTI[valfilno];
							Band=TRUE;
						}
						recordnum=pPG->GetNumRecords();
					}
					else if(ens_type == ENSEMBLE_TYPE::ACCEPTED){
						if(valfilno==0) {
							pPG=pPGTime;
							BandPG=TRUE;
						}
						else {

							string name;
							pPG=m_pSimDoc->m_PlotStorage.m_PG_SEL_MULTI[valfilno];
							name=pPG->GetFileName();
							if(name=="MyNewFile") {
								pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(idoc,valfilno);
								name=pPG->GetFileName();
							}
					
							pPG->ReOpen();
					//ResidualSource=;
					
							Band=TRUE;
							MeanOfAll=FALSE;
							if(m_pSimDoc->MR_XBin_IsOpen(valfilno)) {
								ResidualSource=TRUE;
								pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(valfilno);
							}

						}
						recordnum=pPG->GetNumRecords();
	
						if(pPG==nullptr) return FALSE;
					}
					else if(datatype==DATA_TYPES::PGDATA) {
						Band=FALSE;
				
					}
					else if(datatype== DATA_TYPES::SIM_VAL) {
						pX=(SimB*)Ptr;	
						Band=FALSE;
						int FileNo=m_pSimDoc->m_PlotStorage.GetValFileNo(index);
						pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(idoc,FileNo);
						pPG=m_pSimDoc->m_PlotStorage.m_PG_ALLMULTI[FileNo];
						if(pPG==nullptr) return FALSE;				
						pPG->ReOpen();
						if(m_pSimDoc->MR_XBin_IsOpen(FileNo)) {
							ResidualSource=TRUE;
							pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(FileNo);
							ResRunNo=m_pSimDoc->m_PlotStorage.GetValRunNo(index);
							if(ResRunNo==140000) {
								//CFileScanner ScanMeanProg(nullptr);
								//ScanMeanProg.Create(IDD_PROGRESS,nullptr);
								//ScanMeanProg.ShowWindow(SW_SHOW);
								Band=TRUE;
								MeanOfAll=FALSE;
								AfxBeginThread(UpdateAcceptedThreadProc,pRes);
								//pRes->UpdateStatistics(TRUE, &ScanMeanProg);
							}
							else if(ResRunNo==110000)
								ResRunNo=0;
						}

					}
					else {
						Band=FALSE;
					}
					if(!pPG->ReOpen()) {
						pPG->GetFileName();
						pPG->Open(pPG->GetFileName());
					//	pPG->GetFileDescription();						
					}
					PG_pointerArray.Add(pPG);

				}

				if(multiRunPlot) {
					int ind, nTabIndex;
					string unit,st;
					SimB *pPar;
					int ilocal,il;
					ilocal=m_pSimDoc->m_PlotStorage.GetLocalIndex(index);

					switch (datatype) {
						case DATA_TYPES::SIM_PAR:
						ind=0;
						
						unit="";
						for(int i=0;i<m_pSimDoc->MR_GetNumberOfDimensions();i++){
								for(int ii=0; ii<m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i+1);ii++){
									pPar=m_pSimDoc->MR_Get_pPar(i+1,ii+1);
									nTabIndex=m_pSimDoc->MR_GetTabIndex(i+1,ii+1);
									string namep=pPar->GetName();
									if(nTabIndex>=0)				
										namep+="("+FUtil::STD_ItoAscii(nTabIndex+1)+")";
									if(ind==ilocal) {
										st=namep;
										test="Parameter values";
										unit=pPar->GetUnit();
										break;

									}
									ind++;
								}
						}

						int nAddVar; nAddVar=ilocal-ind;
						if(nAddVar>=0)
							test="Simulation";
						switch (nAddVar) {
							case 0:
								st="Accepted";
								unit="-";
								break;
							case 1:
								test="Log Likelihood";
								st="Time Serie Variables";
								unit="-";
								break;
							case 2:
								test="Log Likelihood";
								st="Summary Variables";
								unit="-";
								break;
							case 3:
								test="Log Likelihood";
								st="Total";
								unit="-";
								break;
							case 4:
								test="Run Number";
								st="MAP";
								unit="-";
								break;
							case 5:
								test="Simulation Time";
								st="";
								unit="SEC";
								break;
							case 6:
								test="Simulation Time";
								st="Accumulated";
								unit="SEC";
								break;
							case 7:
								test="Mean Fraction of Total Time";
								st="Sim Period";
								unit="-";
								break;
							case 8:
								test="Mean Fraction of Total Time";
								st="Post Period";
								unit="-";
								break;
							case 9:
								test="Mean Fraction of Total Time";
								st="Inter Period";
								unit="-";
								break;
						}

						break;
					case DATA_TYPES::SIM_OUTSUM: // Output Variables
						ind=0;
						int lin;
						vector<SIMB> vvp;
						for(size_t jk=0; jk<4;jk++) {
							switch(jk) {
								case 0:
									vvp=m_pSimDoc->m_X_Array;
									break;
								case 1:
									vvp=m_pSimDoc->m_T_Array;
									break;
								case 2:
									vvp=m_pSimDoc->m_G_Array;
									break;
								case 3:
									vvp=m_pSimDoc->m_D_Array;
									break;
							}
							for(size_t ij=0; ij<vvp.size();ij++) {
								pPar=vvp[ij].pBase;
								test=pPar->GetName();
								unit=pPar->GetUnit().c_str();
								test=pPar->GetName().c_str();

								for(int ii=0;ii<((OutVector*)pPar)->GetNumberOfSelectedFlags();ii++) {
									if(pPar->Is_Vector()) {
										lin=((OutVector*)pPar)->GetSelectedLocalIndex(ii);
										int ipg=((OutVector*)pPar)->GetPgFileIndex(lin);
										if(ipg==ilocal/6) {
											il=lin;
											ii=((OutVector*)pPar)->GetNumberOfSelectedFlags();
											ij=vvp.size();
											st="";
											st+="("+FUtil::STD_ItoAscii(il+1)+")";
											break;
										}
									}
									else {
										st="";
										break;

									}
								}				
							}
						}
						switch (ilocal%6) {
							case 0:
								st+="Initial";
								break;
							case 1:
								st+="Final";
								break;
							case 2:
								st+="Min";
								break;
							case 3:
								st+="Max";
								break;
							case 4:
								st+="Mean";
								break;
							case 5:
								st+="Accumulated";
								break;
						}
						break;
					}
					QuickSetText(i+1,-4,str);
					QuickSetText(i+1,-3,test);
					QuickSetText(i+1,-1,st);
					QuickSetText(i+1,-2,unit);


				}
				else {
					int Nextra=1;
					if(Band) 
						Nextra=5;
					if(BandPG)
						Nextra=4;

					nindex++;

					int seq_ID=m_pSimDoc->m_PlotStorage.GetValRunNo(index);


					for(int j=0; j<Nextra;j++) {
						SetColWidth(nindex+j,90);
						str=_T("Var #")+MFC_Util::ItoAscii(i+1);
						if(seq_ID>0) str+=_T("Mul #(")+MFC_Util::ItoAscii(seq_ID)+_T(")");
						QuickSetText(nindex+j,-4,str);
						str=pPG->GetVarName(pgindex).c_str();
						str.TrimRight();
						
						QuickSetText(nindex+j,-3,str);
						str=pPG->GetVarUnit(pgindex).c_str();
						str.TrimRight();
						QuickSetText(nindex+j,-2,str);
						
						if(Nextra>1&&Nextra<5) {
							switch(j) {
								case 0: str="Mean"; break;
								case 1: str="Std"; break;
								case 2: str="Min"; break;
								case 3: str="Max"; break;
							}
						}
						else if(Nextra>1)
						{
							switch(j) {
								case 0: str="Measured"; break;
								case 1: str="Min Residual"; break;
								case 2: str="Max Residual"; break;
								case 3: str="Mean Residual"; break;
								case 4: str="Std Residual"; break;
							}
						}
						else if(ResRunNo<1000000) {
							str=_T("MRun :")+MFC_Util::ItoAscii(ResRunNo+1);
						}
						else {
							str=pPG->GetVarId(pgindex).c_str();
							str.TrimLeft();
							str.TrimRight();
						}
						QuickSetText(nindex+j,-1,str);
						if(j==Nextra-1&&Nextra==5) nindex+=4;
						
					}
				}

			}
//			CPG *pPG;
			



			float v;
			unsigned int minutv;
			int RunNo=m_pSimDoc->m_DocFile.m_SimulationRunNo-m_pSimDoc->MR_Get_ActualRunNumber();
			for(int irow=0;irow<recordnum;irow++) {
				if(m_pSimDoc->IsFinishedSimulation())
					QuickSetBackColor(-1,irow,m_ColorCompleted);
				else 
					QuickSetBackColor(-1,irow,m_ColorClick);

				QuickSetNumber(-1,irow,irow+1);
				if(multiRunPlot) {
					RunNo++;
					QuickSetNumber(0,irow,RunNo);
					for (int icol=1; icol<varnum+1;icol++){
						index=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_ShowChartTableNo, icol-1);
						int ilocal=m_pSimDoc->m_PlotStorage.GetLocalIndex(index);
						auto datatype=m_pSimDoc->m_PlotStorage.GetDataType(index);
						if(datatype==DATA_TYPES::SIM_PERFORMANCE)
							v=m_pSimDoc->m_MStorage.GetValid(irow,ilocal);
						else if(datatype== DATA_TYPES::SIM_PAR)
							v=m_pSimDoc->m_MStorage.GetMultiP(irow,ilocal);
						else if(datatype== DATA_TYPES::SIM_OUTSUM)
							v=m_pSimDoc->m_MStorage.GetSumV(irow,ilocal-6);
						else if(datatype== DATA_TYPES::SIM_SUMVAR)
							v=m_pSimDoc->m_MStorage.GetValidSum(irow,ilocal);
						if(v>MISSING)
							QuickSetNumber(icol,irow,v);
					}

				}
				else {
					QuickSetText(0,irow,pPGTime->GetDate(irow+1));
					minutv=pPGTime->GetLongTime(irow+1);	
					int indcol=1;
					for (int icol=1; icol<varnum+1;icol++){
						int index=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_ShowChartTableNo, icol-1);
						int IndexVal=m_pSimDoc->m_PlotStorage.GetValVarNo(index);
						int pgindex=PG_indexArray[icol-1];
						CPG *pPG1=(CPG*)PG_pointerArray.GetAt(icol-1);
						auto datatype=m_pSimDoc->m_PlotStorage.GetDataType(index);
						auto ens_type = m_pSimDoc->m_PlotStorage.GetEnsembleType(index);
						int idoc=m_pSimDoc->m_PlotStorage.GetDocIndex(index);
						float v, vmin, vmax, vmean, vstd;
						if(datatype== DATA_TYPES::SIM_VAL &&ens_type == ENSEMBLE_TYPE::NO) {
							Band=FALSE;
							int FileNo=m_pSimDoc->m_PlotStorage.GetValFileNo(index);
							pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(idoc,FileNo);
							pPG=m_pSimDoc->m_PlotStorage.m_PG_ALLMULTI[FileNo];
							if(pPG==nullptr) return FALSE;				
							pPG->ReOpen();
							if(m_pSimDoc->MR_XBin_IsOpen(FileNo)) {
								ResidualSource=TRUE;
								pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(FileNo);
								ResRunNo=m_pSimDoc->m_PlotStorage.GetValRunNo(index);
								if(ResRunNo==140000) {
									Band=TRUE;
									MeanOfAll=FALSE;
									//CFileScanner ScanMeanProg(nullptr);
								//	ScanMeanProg.Create(IDD_PROGRESS,nullptr);
									//ScanMeanProg.ShowWindow(SW_SHOW);
									//pRes->UpdateStatistics(TRUE, &ScanMeanProg);
									AfxBeginThread(UpdateAcceptedThreadProc,pRes);
									extracol+=4;
								}
								else if(ResRunNo==110000)
									ResRunNo=0;
							}

						}
						else if(ens_type==ENSEMBLE_TYPE::ACCEPTED&&datatype == DATA_TYPES::SIM_VAL) {
							MeanOfAll=FALSE;
							Band=TRUE;
						}
						else if(ens_type == ENSEMBLE_TYPE::ALL&& datatype == DATA_TYPES::SIM_VAL) {
							MeanOfAll=TRUE;
							Band=TRUE;
						}
						else if(m_pSimDoc->m_PlotStorage.GetRepitType(index)) {

							int seq=m_pSimDoc->m_PlotStorage.GetValRunNo(index);
							pPG1->SetRepititionSequence(seq);

						}




						if(pPGTime==pPG1)	
							v=pPG1->GetVarValue(pgindex,irow+1);
						if(ens_type != ENSEMBLE_TYPE::NO) {
							int PGNumVar=pPG1->GetNumVariables();
							vstd=pPG1->GetVarValue(pgindex+PGNumVar/4,irow+1);
							vmin=pPG1->GetVarValue(pgindex+(PGNumVar*2)/4,irow+1);
							vmax=pPG1->GetVarValue(pgindex+(PGNumVar*3)/4,irow+1);
						}
						if(ResidualSource) {
							if(ResRunNo<1000000&&ResRunNo>=0) {
								v=v-pRes->GetResiduals(ResRunNo,IndexVal,irow);
							}
							else if(MeanOfAll) {
								vmin=pRes->GetMinAllResiduals(IndexVal,irow);
								vmax=pRes->GetMaxAllResiduals(IndexVal,irow);
								vmean=pRes->GetMeanAllResiduals(IndexVal,irow);
								vstd=pRes->GetStdAllResiduals(IndexVal,irow);
							}
							else {
								
								vmin=pRes->GetMinResiduals(IndexVal,irow);
								vmax=pRes->GetMaxResiduals(IndexVal,irow);
								vmean=pRes->GetMeanResiduals(IndexVal,irow);
								vstd=pRes->GetStdResiduals(IndexVal,irow);	
							}
						}
						else {
							if(irow==0) pPG1->Reset_Start();
							v=pPG1->GetVarValue_MinPos(pgindex,minutv);
						}
						if(v>MISSING) {
							QuickSetNumber(indcol,irow,v);
							if(Band&&!BandPG) {
									QuickSetNumber(indcol+1,irow,vmin);
									QuickSetNumber(indcol+2,irow,vmax);
									QuickSetNumber(indcol+3,irow,vmean);
									QuickSetNumber(indcol+4,irow,vstd);
									indcol+=4;

							}
							else if(BandPG) {
									QuickSetNumber(indcol+2,irow,vmin);
									QuickSetNumber(indcol+3,irow,vmax);
									QuickSetNumber(indcol+1,irow,vstd);
									indcol+=3;

							}



						}
						indcol++;
					}

				}
			}




			PG_indexArray.clear();
			while(PG_pointerArray.GetSize()>0) {
				pPG=(CPG*)PG_pointerArray.GetAt(0);
				pPG->CloseFile();
				PG_pointerArray.RemoveAt(0);
			}
		}
		else {
// This is the main Chartlist
			SetNumberCols(6); 
			SetDefFont(&m_font);			
			SetColWidth(-1,20);
			SetColWidth(0,50);
			SetColWidth(1,50);
			SetColWidth(2,100);
			SetColWidth(3,200);
			SetColWidth(4,300);
			SetColWidth(5,400);
			QuickSetBackColor(-1,-1,m_ColorClick);
			QuickSetText(-1,-1,"+");

			QuickSetText(0,-1, "Chart #");
			QuickSetText(1,-1, "Var #");
			QuickSetText(2,-1, "Chart Type");
			QuickSetText(3,-1, "Group");
			QuickSetText(4,-1, "Variable Name");
			QuickSetText(5,-1, "Origin");



			NumLines=m_pSimDoc->m_PlotStorage.NumSelected();
			SetNumberRows(NumLines);
			SetTH_NumberRows(1);
			SetTH_Height(20); 
			int ivar, ichart;
			ichart=1;
			ivar=0;
			int iDoc=-1,iDocCount=0,iDocPrevious=-1;
			for(int i=0; i<NumLines; ++i)
			{
				if(m_pSimDoc->IsFinishedSimulation())
					QuickSetBackColor(-1,i,m_ColorCompleted);
				else 
					QuickSetBackColor(-1,i,m_ColorClick);

				GetCell(0,i,&cell);
				cell.SetNumber(m_pSimDoc->m_PlotStorage.GetChart(i));
				SetCell(0,i,&cell);
				
				
				if(ichart==m_pSimDoc->m_PlotStorage.GetChart(i)) {
					ivar++;
				}
				else {
					ichart=m_pSimDoc->m_PlotStorage.GetChart(i);
					ivar=1;
				}
				int jvarnum;
				jvarnum=m_pSimDoc->m_PlotStorage.GetNumVar(ichart);
				if(jvarnum>1&&ivar==1) 
					JoinCells(0,i,0,i+jvarnum-1);
				GetCell(1,i,&cell);


				cell.SetNumber(ivar);
				cell.SetBackColor(MFC_Util::GetColorLine(ivar));
				SetCell(1,i,&cell);
					
				GetCell(2,i,&cell);
				cell.SetNumber(int(m_pSimDoc->m_PlotStorage.GetChartType(i)));
				SetCell(2,i,&cell);	
				auto datatype=m_pSimDoc->m_PlotStorage.GetDataType(i);
				void *Ptr=m_pSimDoc->m_PlotStorage.GetPtr(i);
				SimB *pX=nullptr;
				CPG *pPG1;
				if(datatype== DATA_TYPES::ANYDATA) {
					if(Ptr!=nullptr) {
						pPG1=(CPG*)((F*)Ptr)->GetPointer();
						pX=(F*)Ptr;
					}
					else
						pPG1=nullptr;
					if(pPG1==nullptr&&Ptr!=nullptr) {
						string fileName=((F*)Ptr)->GetStrValue();
						bool is=FUtil::IsFileExisting(fileName);
						if(is) {
							pPG1=new CPG;
							pPG1->Open(fileName);
							if(pPG1->IsOpen())
								((F*)Ptr)->SetPointer(pPG1);
							else	{
								delete pPG1;
								pPG1=nullptr;
							}
						}
							
					}
				}
				else if(datatype!= DATA_TYPES::PGDATA)
					pX=(SimB*)Ptr;
				else if(Ptr!=nullptr)	{
					pPG1=(CPG*)Ptr;
					pX=nullptr;	
				}
				else {
					pX=nullptr;
					pPG1=nullptr;
				}
	
				if(pX==nullptr||(datatype>DATA_TYPES::SIM_VAL&&datatype<= DATA_TYPES::PGDATA)) {
					
					if(datatype== DATA_TYPES::SIM_PERFORMANCE) {
						int valfilno;
						valfilno=m_pSimDoc->m_PlotStorage.GetValFileNo(i);
						int idoc=m_pSimDoc->m_PlotStorage.GetDocIndex(i);
						//m_pSimDoc->ValidationFilePointer()
						int nselV=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
						int ilocal=m_pSimDoc->m_PlotStorage.GetLocalIndex(i);
						int ival;
						if(nselV>0) {
							ival=ilocal%nselV;
							test=m_pSimDoc->m_ValidationData.GetValName(ival);
							int iv=m_pSimDoc->m_ValidationData.GetValLocalIndex(ival);
							if(iv>-1) test+="("+FUtil::STD_ItoAscii(iv+1)+")";
							QuickSetText(4,i,test);
							int m_ind=ilocal/nselV;
							switch (m_ind) {
								case 0:
									test="r2";
									break;
								case 1:
									test="Intercept";
									break;
								case 2:
									test="Slope";
									break;
								case 3:
									test="ME";
									break;
								case 4:
									test="RMSE";
									break;
								case 5:
									test="Log Likelihood";
									break;
								}
						}
						else
							test="-";

						QuickSetText(3,i,"Validation : "+test);

					}
					else if(datatype== DATA_TYPES::SIM_OUTSUM) {
						QuickSetText(3,i,"Sum Variables");
						int ilocal=m_pSimDoc->m_PlotStorage.GetLocalIndex(i);


						int ind=1, il=-1;
						SimB *pPar;
						string st, unit;


						vector<SIMB> vvp;
						for(size_t jk=0; jk<4;jk++) {
							switch(jk) {
								case 0:
									vvp=m_pSimDoc->m_X_Array;
									break;
								case 1:
									vvp=m_pSimDoc->m_T_Array;
									break;
								case 2:
									vvp=m_pSimDoc->m_G_Array;
									break;
								case 3:
									vvp=m_pSimDoc->m_D_Array;
									break;
							}
						
							for(size_t ij=0; ij<vvp.size();ij++) {
								pPar=vvp[ij].pBase;
								test=pPar->GetName().c_str();
								unit=pPar->GetUnit().c_str();
								test=pPar->GetName().c_str();

								for(int ii=0;ii<((OutVector*)pPar)->GetNumberOfSelectedFlags();ii++) {
									if(pPar->Is_Vector()) {
										int lin=((OutVector*)pPar)->GetSelectedLocalIndex(ii);
										int ipg=((OutVector*)pPar)->GetPgFileIndex(lin);
										if(ipg==ilocal/6) {
											il=lin;
											ii=((OutVector*)pPar)->GetNumberOfSelectedFlags();
											ij=vvp.size();
											st="";
											st+="("+FUtil::STD_ItoAscii(il+1)+")";
											break;
										}
									}
									else {
										st="";
										break;

									}
								}	
							}
	
						switch (ilocal%6) {
							case 0:
								st+="Initial";
								break;
							case 1:
								st+="Final";
								break;
							case 2:
								st+="Min";
								break;
							case 3:
								st+="Max";
								break;
							case 4:
								st+="Mean";
								break;
							case 5:
								st+="Accumulated";
								break;
						}

						QuickSetText(4,i,st);
						}
					}
					else if(datatype== DATA_TYPES::SIM_PAR) {
						QuickSetText(3,i,"MultiRun Parameters");
						int ilocal=m_pSimDoc->m_PlotStorage.GetLocalIndex(i);
						string unit="", st;
						int ind=0;
						for(size_t i1=0;i1<m_pSimDoc->MR_GetNumberOfDimensions();i1++){
								for(size_t ii=0; ii<m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i1+1);ii++){
									SimB *pPar=m_pSimDoc->MR_Get_pPar(i1+1,ii+1);
									int nTabIndex=m_pSimDoc->MR_GetTabIndex(i1+1,ii+1);
									if (pPar == nullptr) continue;
									string namep=pPar->GetName();
									if(nTabIndex!=string::npos)				
										namep+="("+FUtil::STD_ItoAscii(nTabIndex+1)+")";
									if(ind==ilocal) {
										st=namep;
										test="Parameter values";
										unit=pPar->GetUnit();
										break;

									}
									ind++;
								}
						}
						QuickSetText(4,i, st);

					}
					else if(datatype== DATA_TYPES::PGDATA) {
						QuickSetText(3,i,"PG File");
						
						int ilocal=m_pSimDoc->m_PlotStorage.GetLocalIndex(i);
						test=pPG1->GetVarName(ilocal);
						FUtil::trim(test);
						test+="/";test+=pPG1->GetVarId(ilocal);
						QuickSetText(4,i,test);
						QuickSetText(5,i,pPG1->GetFileName());


					}
					else if(datatype== DATA_TYPES::SIM_VAL) { // Means itype==5
						int ValFileNo=m_pSimDoc->m_PlotStorage.GetValFileNo(i);
						QuickSetText(3,i,"Validation_"+FUtil::STD_ItoAscii(ValFileNo));
						int ilocal=m_pSimDoc->m_PlotStorage.GetLocalIndex(i);
						int PGindex=m_pSimDoc->m_PlotStorage.GetVar_Index(i);
						//GetNumOutPuts()+ilocal;
						CPG *pPG=nullptr;
						int idoc=m_pSimDoc->m_PlotStorage.GetDocIndex(i);
						if(m_pSimDoc->IsFinishedSimulation()) 
							pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(idoc, ValFileNo);
						else
							pPG=nullptr;

						if(pPG!=nullptr) {
							if(!pPG->IsOpen()){
								pPG->ReOpen();
							}
							test=pPG->GetVarName(PGindex).c_str();test+=pPG->GetVarId(PGindex).c_str();
							QuickSetText(4,i,test);
						}
						else {
							index=int((m_pSimDoc->m_PlotStorage.GetLocalIndex(i)+1)/2);
							QuickSetText(4,i,m_pSimDoc->m_ValidationData.GetValName(index));
						}
						if(pPG!=nullptr)
							pPG->CloseFile();
					}
				}
			}
		}
//REMAIN
/*				else if() {
					test=((F*)pX)->GetValue();
					index=m_pSimDoc->m_PlotStorage.GetLocalIndex(i);
					if(pPG1!=nullptr) {
						test=pPG1->GetVarName(index);

						QuickSetText(4,i,test);
						QuickSetText(3,i,"Model Files");
					}
				}
				else {
					QuickSetText(3,i,pX->GetGroup());
					test=pX->GetName().c_str();
					index=m_pSimDoc->m_PlotStorage.GetLocalIndex(i);
					if(index>=0)
						test+="("+FUtil::STD_ItoAscii(index+1)+")";
					QuickSetText(4,i, test);
				}

				// DocName
				if(type!=0)
					iDoc=m_pSimDoc->m_PlotStorage.GetDocIndex(i);
				if(iDoc==iDocPrevious&&type!=10&&type!=nullptr) {
					iDocCount++;
				}
				else {
					if(iDocCount>1) 
						JoinCells(5,i-iDocCount,5,i-1);
					iDocCount=1;
					iDocPrevious=iDoc;
				}
				
				CString str;
				if(type==0&&pX!=nullptr) {
					test=((F*)pX)->GetStrValue().c_str();
					str=test;
				}
				else if(iDoc>=0)
					str=m_pSimDoc->m_PlotStorage.GetDocFileName(iDoc).c_str();
				else
					str=_T("");
				if(type!=10)
					QuickSetText(5,i,str);

			}


			JoinCells(-1,0,-1,NumLines);
		}
	RedrawAll();*/
	return true;
}
bool MakeSimSheets::Make_ChaParListSheet()
{
	CUGCell cell; 
	CString test, Group, Name, Dat, str;
	float fvalue;
	SimB* pSw;
	size_t TabIndex;
	unsigned int NumLines, ii, icount, StartRow;
	int itype;

				int prelines=0;
				SetSheetNumber(9); 
				SetNumberCols(5); 
				SetDefFont(&m_font);
				SetColWidth(-1,20);
				SetColWidth(0,150);
				SetColWidth(1,150);
				SetColWidth(2,150);
				SetColWidth(3,100);
			    SetColWidth(4,100);
				
				QuickSetText(0,-1, "Module");
				QuickSetText(1,-1, "Parameter/Database");
				QuickSetText(2,-1, "Date for Change");
				QuickSetText(3,-1, "Value");
				QuickSetText(4,-1, "Key");
   				NumLines=m_pSimDoc->m_P_CH_Array.size();
				icount=0;
				for(int i=0; i<NumLines; ++i) {
					pSw=(Ps*)m_pSimDoc->m_P_CH_Array[i].pBase;
					TabIndex=m_pSimDoc->m_P_CH_Array[i].TabIndex;
					itype=m_pSimDoc->m_P_CH_Array[i].Type;
					if(itype==2) {
						for(ii=0;ii<((CDB*)pSw)->GetNumChaDates();ii++){
							icount++;
						}
					}
					else {
						icount++;
						if(itype==1) {
							for(ii=0;ii<((P*)pSw)->GetNumChaDates(TabIndex);ii++) 
								icount++;
						}
						else if(itype==0) {
							for(ii=0;ii<((Ps*)pSw)->GetNumChaDates();ii++) 
								icount++;

						}
					}
				}
				SetNumberRows(icount);
				icount=-1;

				for(int i=0; i<NumLines; ++i){
					pSw=m_pSimDoc->m_P_CH_Array[i].pBase;
					TabIndex=m_pSimDoc->m_P_CH_Array[i].TabIndex;
					itype=m_pSimDoc->m_P_CH_Array[i].Type;
				//	Group=pSw->GetGroup().c_str(

					if(itype==2) {
						Group="DataBase";
						Name=((CDB*)pSw)->GetName().c_str();
						StartRow=icount+1;
						for(ii=0;ii<((CDB*)pSw)->GetNumChaDates();ii++){
							Dat=PGUtil::StringDatum(((CDB*)pSw)->GetChaDate(ii)).c_str();
							test=((CDB*)pSw)->GetChaParValue(ii).c_str();
							str=((CDB*)pSw)->GetChaParKey(ii).c_str();
							icount++;
							if(m_pSimDoc->IsFinishedSimulation())
								QuickSetBackColor(-1,icount,m_ColorCompleted);
							else 
								QuickSetBackColor(-1,icount,m_ColorClick);
							QuickSetNumber(-1,icount, ii+1);
							QuickSetText(0,icount, Group);
							QuickSetText(1,icount, Name);
							QuickSetText(2,icount, Dat);
							QuickSetText(3,icount, test);
							QuickSetText(4,icount, str);
						}
						JoinCells(0,StartRow,0,icount);
						JoinCells(1,StartRow,1,icount);
					}
					else {
						Group=pSw->GetGroup().c_str();
						Name=pSw->GetName().c_str();

						size_t inum;
						if(itype==1){
							Name+=_T("(")+MFC_Util::ItoAscii(TabIndex+1)+_T(")");
							fvalue=((P*)pSw)->GetValue(TabIndex);
							inum=((P*)pSw)->GetNumChaDates(TabIndex);
						}
						else if(itype==0) {
							fvalue=((Ps*)pSw)->GetValue();
							inum=((Ps*)pSw)->GetNumChaDates();
						}
						else {
							fvalue = 0.f;
							inum = 0;
						}
						test=MFC_Util::FtoAscii(fvalue);
						icount++;
						StartRow=icount;
						Dat=PGUtil::StringDatum(m_pSimDoc->RunOpt.longMinStart).c_str();
							if(m_pSimDoc->IsFinishedSimulation())
								QuickSetBackColor(-1,icount,m_ColorCompleted);
							else 
								QuickSetBackColor(-1,icount,m_ColorClick);

							QuickSetText(0,icount, Group);
							QuickSetText(1,icount, Name);
							QuickSetText(2,icount, Dat);
							QuickSetText(3,icount, test);
							Dat=PGUtil::StringDatum(m_pSimDoc->RunOpt.longMinStart).c_str();


						for(size_t ii=0;ii<inum;ii++){
							if(itype==1) {
								Dat=PGUtil::StringDatum(((P*)pSw)->GetChaDate(TabIndex,ii)).c_str();
								fvalue=((P*)pSw)->GetChaParValue(TabIndex,ii);
							}
							else if(itype==0) {
								Dat=PGUtil::StringDatum(((Ps*)pSw)->GetChaDate(ii)).c_str();
								fvalue=((Ps*)pSw)->GetChaParValue(ii);

							}
							test=MFC_Util::FtoAscii(fvalue);
							icount++;
							if(m_pSimDoc->IsFinishedSimulation())
								QuickSetBackColor(-1,icount,m_ColorCompleted);
							else 
								QuickSetBackColor(-1,icount,m_ColorClick);
							QuickSetNumber(-1,icount, ii+1);
							QuickSetText(0,icount, Group);
							QuickSetText(1,icount, Name);
							QuickSetText(2,icount, Dat);
							QuickSetText(3,icount, test);
						}

					JoinCells(0,StartRow,0,icount);
					JoinCells(1,StartRow,1,icount);
					}
				}
				RedrawAll();
	return true;
}
bool MakeSimSheets::Make_MultiRunListSheet()
{
	CUGCell cell; 
	CString test, Group, Name, Dat, str;

	unsigned int NumLines;
	bool ParameterCalib;
	if(m_pSimDoc==nullptr) return false;
	SetSheetNumber(10);
	m_bDrawExcelFocus = TRUE;
	SetMultiSelectMode(TRUE);
	SetSH_NumberCols(1);
	SetSH_Width(30);
	SetTH_NumberRows(1);
	SetTH_Height(30);

	if(m_pSimDoc->IsMultiSimulation()&&m_pSimDoc->IsFinishedSimulation()&&m_ShowMultiRes) {
		//List of standard result of a ready MultiDocument
		QuickSetBackColor(-1,-1,m_ColorClick);
		QuickSetText(-1,-1,"-");

		if(m_pSimDoc->MR_Storage_Open()) {
			Multi_Post_CompleteList_Header();
			Multi_Post_CompleteList();
			RedrawAll();
		}
	}
	else if(m_pSimDoc->MR_Get_NumTotalPar()>1&&m_pSimDoc->IsFinishedSimulation()&&m_ShowCoVarRes) {
		// CoVariance of parameters List from Ready MultiRuns
		//QuickSetBackColor(-1,-1,m_ColorClick);
		//QuickSetText(-1,-1,"-");
		Multi_Post_CoVarList();
		RedrawAll();
	}
	else if(m_pSimDoc->IsMultiSimulation()&&m_pSimDoc->IsFinishedSimulation()&&!m_ShowCoVarRes) {
		if (m_ShowAllEnsemblePostParList)
			Multi_Post_AllEnsembleList();
		else {
			if(m_pSimDoc->GetNumAll_TSV_ValVariables()>0)
				Multi_Post_NormalList();
			else
				Multi_Post_SensitivityTest();
		}
	}
	else {
		Multi_Prior_List();
		RedrawAll();

	}
	return true;
}
size_t MakeSimSheets::Val_Post_Header() {

				bool multivalid=false;
				bool multimeanvalid=true;
				if(m_pSimDoc->IsMultiSimulation())
					multivalid=m_pSimDoc->MR_Storage_Open();

				CString txt;
				m_bDrawExcelFocus = TRUE;
				SetMultiSelectMode(TRUE);
				SetSH_NumberCols(1);
				SetSH_Width(30);
				SetTH_NumberRows(1);
				SetTH_Height(30);
				SetNumberCols(16); 
				SetColWidth(-1,20);

				SetColWidth(0,40);
				SetColWidth(1,160);
				SetColWidth(2,160);
				SetColWidth(3,130);
				if(multivalid)
					SetColWidth(4,130);
				else
					SetColWidth(4,0);
				
				SetColWidth(5,60);
				SetColWidth(6,120);
				UnJoinCells(0,-1);
				UnJoinCells(1,-1);
				for(int i=7;i<16;i++)
					SetColWidth(i,80);
				QuickSetText(0,-1, "#");
				QuickSetText(1,-1, "Measured");
				QuickSetText(2,-1, "Group");
				QuickSetText(3,-1, "Simulated");
				QuickSetText(4,-1, "Ensemble");
				QuickSetText(5,-1, "Chart");
				QuickSetText(6,-1, "n");
				QuickSetText(7,-1, "r2");
				QuickSetText(8,-1, "Intercept");
				QuickSetText(9,-1, "slope");
				QuickSetText(10,-1, "ME");
				QuickSetText(11,-1, "RMSE");
				QuickSetText(12,-1, "M Simulated");
				QuickSetText(13,-1, "M Mesaured");
				QuickSetText(14,-1, "L Likelihood");
				QuickSetText(15,-1, "NSE r2");
				m_pSimDoc->OutputBinFileCheck();
				BOOL validplot=m_pSimDoc->IsOutputValidBinFileValid();
				if(!validplot) {
					QuickSetBackColor(3,-1,m_ColorHeading);
					QuickSetBackColor(4,-1,m_ColorHeading);
				}
				if(!multivalid) multimeanvalid=false;
				if(!m_pSimDoc->m_OutputValidXBinFileExist) multimeanvalid=false;		
			return size_t(GetNumberRows());



}
void MakeSimSheets::Val_Post_Data(size_t NumLines, size_t NumLinesSV) {
	string test;
	CString txt;
	float fvalue;
	CUGCell cell;

	bool multimeanvalid=true;
	bool multivalid;
	if(m_pSimDoc->IsFinishedSimulation())
		multivalid=m_pSimDoc->MR_Storage_Open();
	else
		multivalid=false;


	if(!multivalid) multimeanvalid=false;
	if(!m_pSimDoc->m_OutputValidXBinFileExist) multimeanvalid=false;	

	BOOL validplot=m_pSimDoc->IsOutputValidBinFileValid();

	int numtsv;
	if (NumLinesSV > 0) {
		numtsv = NumLines - NumLinesSV - 1;
		numtsv = max(0, numtsv);
	}
	else
		numtsv=NumLines;
	
	VALSUMv sumv;
	bool validMBinFile = false;

	bool DynamicIndexValFile = false;
	size_t DynamicIndexPos = string::npos;


	if (m_pSimDoc->IsMultiSimulation()) {
		bool make_cyclic_sampling = false;
		auto pP = m_pSimDoc->GetPsPointer("Aggregate Calib Number");
		if (pP != nullptr) {
			if (pP->GetValue() > 1.1) make_cyclic_sampling = true;
		}
		if (m_pSimDoc->MR_GetValidIndex_IndexInValFile_InDim1() != string::npos&&make_cyclic_sampling) {
			//auto NumberofDim1 = m_pSimDoc->MR_GetDimCounter(0);
			auto NumberofDim1 = m_pSimDoc->m_MultiRun_Array[0].NumberOfRepeatitionsWithinDimension;
			DynamicIndexPos= m_pSimDoc->MR_GetValidIndex_IndexInValFile_InDim1();
			DynamicIndexValFile = true;
			m_pSimDoc->m_MStorage.SetCyclicNumber(NumberofDim1);
			auto FileNo = m_pSimDoc->m_ValidationData.GetValFileNumber(0);
			CResiduals *pRes = m_pSimDoc->GetValidResidualPointer(FileNo);
			if (pRes != nullptr) {
				pRes->SetResidualBaseFileNumVarScaling(NumberofDim1);
				pRes->AlloResVector();
			}
			else {
				MFC_Util::MessageBoxReturn("Missing OutPut File - No viewing of Validation data possible", "File Missing", MB_OK);


			}

		}
	}








	for(int i=0;i<numtsv;i++) {
					test=FUtil::STD_ItoAscii(i+1);
					QuickSetText(0, i, test);
					sumv = m_pSimDoc->GetValSumStruct(i);
					int ind=m_pSimDoc->m_ValidationData.GetValFileIndex(i);
					//if(DynamicIndexValFile) ind= m_pSimDoc->m_MStorage.GetMultiP(i, DynamicIndexPos)-1;
					size_t FileNo=m_pSimDoc->m_ValidationData.GetValFileNumber(i);
					auto pValFile=m_pSimDoc->ValidationFilePointer(FileNo);
					pValFile->CallDataDescription();
					test=pValFile->GetDataDescription(ind).c_str();
					GetCell(1,i, &cell);
					cell.SetCellType(UGCT_CHECKBOX);
					cell.SetBool(FALSE);
					cell.SetLabelText(test.c_str());
					SetCell(1,i,&cell);

					test=m_pSimDoc->m_ValidationData.GetValGroup(i).c_str();
					QuickSetText(2,i, test);
					QuickSetBackColor(2,i,MFC_Util::GetGroupColor(p_ModelInfo->GetGroupCategory(string(test))));
					test=m_pSimDoc->m_ValidationData.GetValName(i).c_str();
					int iv=m_pSimDoc->m_ValidationData.GetValLocalIndex(i);
					if(iv>-1) test+="("+FUtil::STD_ItoAscii(iv+1)+")";
					GetCell(3,i, &cell);
					cell.SetCellType(UGCT_CHECKBOX);
					cell.SetBool(FALSE);

					cell.SetLabelText(test.c_str());
					SetCell(3,i,&cell);
					GetCell(4,i, &cell);
					if (multimeanvalid) {
						cell.SetCellType(UGCT_CHECKBOX);
						cell.SetBool(FALSE);
						if (!validMBinFile) {
							if (m_pSimDoc->m_MStorage.m_AcceptedIdString.size() == 0) {
								m_pSimDoc->m_MStorage.OpenFile(m_pSimDoc->m_DocFile2.m_Multi_MBin_File);
								if (!m_pSimDoc->m_MStorage.EvaluateFile(true)) {

									if (!m_pSimDoc->m_MStorage.EvaluateFile(false)) return;
									if (!m_pSimDoc->m_MStorage.CompleteProcessingOfInputFiles_MultiStorage()) return;

								}
								m_pSimDoc->m_MStorage.UpdateAcceptedRuns(true, true);
						}
							validMBinFile = true;
						}
						auto str=m_pSimDoc->m_MStorage.m_AcceptedIdString[m_pSimDoc->m_MStorage.m_ActualEnsemble];
						cell.SetLabelText(str);

						CResiduals *pRes = m_pSimDoc->GetValidResidualPointer(FileNo);
						if (pRes != nullptr) {

							if (!pRes->EnableEstimatesMade()) {
								if (pRes->EnsembleReadWrite(true, string::npos)) {
									cell.SetBackColor(m_ColorClick);
									cell.SetReadOnly(FALSE);
								}
								else {

									cell.SetReadOnly(FALSE);

								}
							}
							else if (pRes->m_MeanDefined) {
								cell.SetBackColor(m_ColorClick);
								cell.SetReadOnly(FALSE);
							}
							else {
								cell.SetReadOnly(TRUE);
								cell.SetBackColor(m_NoColor);
							//	MyCug *pCug = MySingle::instance().GetCugPointer();
							//	if (!pRes->m_MeanDefined&&!pRes->m_Calculating) {
							//		pCug->CalculateMeanOffTimeSeries(5002, FileNo);
							//	}
								
							}


						}
						cell.SetParam(2100);
					}
					else
						cell.SetCellType(UGCT_NORMAL);
					SetCell(4,i,&cell);
					if(!validplot) {
						QuickSetBackColor(1,i,m_ColorHeading);
						QuickSetBackColor(3,i,m_ColorHeading);
					}

					/*BOOL testV=m_pSimDoc->GetVal_AccTestValue(i);
					if(testV)
						QuickSetText(5,i,"x");
					else
						QuickSetText(5,i,"-");*/

					GetCell(6,i,&cell);	

					if (m_pSimDoc->m_ValSum_Array.size() > 0) {
						auto n = sumv.n_RegMulti;
						string str;
						if (n > 0) {
							str = FUtil::STD_ItoAscii(n);
							if (m_pSimDoc->m_ValSum_Array[i].FlexSumVar&&m_pSimDoc->m_ValSum_Array[i].ObsScaling) {
								str += " Values are Scaled to match";
								cell.SetText(str);


								if (m_pSimDoc->m_ValSum_Array[i].FlexSumVar&&m_pSimDoc->m_ValSum_Array[i].ObsScaling) {
									cell.SetText("Values are Scaled to macth");

								}
								else if (m_pSimDoc->m_ValSum_Array[i].FlexSumVar) {
									cell.SetText("Original Measured Values");

								}
								else if (m_pSimDoc->m_ValSum_Array[i].FlexSumVar) {
									cell.SetText("Original Measured Values");

								}

							}
						}
						else
							cell.SetText("1");
					}
					else {
							cell.SetNumber(m_pSimDoc->m_ValidationData.GetValn(i));

					}





					SetCell(6,i,&cell);


					GetCell(7,i,&cell);
					fvalue=m_pSimDoc->m_ValidationData.GetValR2(i);
					if(fvalue>-0.9e38)
						txt=MFC_Util::FtoAscii(fvalue);
					else
						txt=_T("");
					if(multivalid) {
						cell.SetCellType(UGCT_CHECKBOX);
						cell.SetBool(FALSE);
						cell.SetLabelText(txt);
					}
					else {
						cell.SetCellType(UGCT_NORMAL);
						cell.SetText(txt);
					}
					if(i>0) {
						if(m_pSimDoc->m_ValidationData.GetValR2(i-1)==m_pSimDoc->m_ValidationData.GetValR2(i)) {
							float vf=m_pSimDoc->m_ValidationData.GetValR2(i-1);
						}
					}
					SetCell(7,i,&cell);	


					GetCell(8,i,&cell);	
					fvalue=m_pSimDoc->m_ValidationData.GetValA0(i);
					if(fvalue>-0.9e38)
						txt=MFC_Util::FtoAscii(fvalue);
					else
						txt=_T("");
					if(multivalid) {
						cell.SetCellType(UGCT_CHECKBOX);
						cell.SetBool(FALSE);
						cell.SetLabelText(txt);
					}
					else {
						cell.SetCellType(UGCT_NORMAL); 
						cell.SetText(txt);
					}
					SetCell(8,i,&cell);	

					GetCell(9,i,&cell);	
					fvalue=m_pSimDoc->m_ValidationData.GetValA1(i);
					if(fvalue>-0.9e38)
						txt=MFC_Util::FtoAscii(fvalue);
					else
						txt=_T("");
					if(multivalid) {
						cell.SetCellType(UGCT_CHECKBOX);
						cell.SetBool(FALSE);
						cell.SetLabelText(txt);
					}
					else {
						cell.SetCellType(UGCT_NORMAL);
						cell.SetText(txt);
					}
					SetCell(9,i,&cell);	

					GetCell(10,i,&cell);
					fvalue=m_pSimDoc->m_ValidationData.GetValME(i);
					if(fvalue>-0.9e38)
						txt=MFC_Util::FtoAscii(fvalue);
					else
						txt=_T("");

					if(multivalid) {
						cell.SetCellType(UGCT_CHECKBOX);
						cell.SetBool(FALSE);
						cell.SetLabelText(txt);
					}
					else {
						cell.SetCellType(UGCT_NORMAL);
						cell.SetText(txt);
					}
					SetCell(10,i,&cell);	

					GetCell(11,i,&cell);
					fvalue=m_pSimDoc->m_ValidationData.GetValRMSE(i);
					if(fvalue>-0.9e38)
						txt=MFC_Util::FtoAscii(fvalue);
					else
						txt=_T("");
					if(multivalid) {
						cell.SetCellType(UGCT_CHECKBOX);
						cell.SetBool(FALSE);
						cell.SetLabelText(txt);
					}
					else {
						cell.SetCellType(UGCT_NORMAL); 
						cell.SetText(txt);
					}
					SetCell(11,i,&cell);			

					GetCell(12,i,&cell);
					fvalue=m_pSimDoc->m_ValidationData.GetValMSIM(i);
					if(fvalue>-0.9e38)
						txt=MFC_Util::FtoAscii(fvalue);
					else
						txt=_T("");
					cell.SetText(txt);
					SetCell(12,i,&cell);	

					GetCell(13,i,&cell);
					fvalue=m_pSimDoc->m_ValidationData.GetValMVAL(i);
					if(fvalue>-0.9e38)
						txt=MFC_Util::FtoAscii(fvalue);
					else
						txt=_T("");
					cell.SetText(txt);
					SetCell(13,i,&cell);	
					
					GetCell(14,i,&cell);
					fvalue=m_pSimDoc->m_ValidationData.GetValLogLi(i);
					if(fvalue>-0.9e38)
						txt=MFC_Util::FtoAscii(fvalue);
					else
						txt=_T("");
					
					if(multivalid) {
						cell.SetCellType(UGCT_CHECKBOX);
						cell.SetBool(FALSE);
						cell.SetLabelText(txt);
					}
					else {
						cell.SetCellType(UGCT_NORMAL);
						cell.SetText(txt);
					}
					SetCell(14,i,&cell);	

					GetCell(15,i,&cell);	
					fvalue=m_pSimDoc->m_ValidationData.GetValNSE(i);
					if(fvalue>-0.9e38)
						txt=MFC_Util::FtoAscii(fvalue);
					else
						txt=_T("");
					if(multivalid) {
						cell.SetCellType(UGCT_CHECKBOX);
						cell.SetBool(FALSE);
						cell.SetLabelText(txt);
					}
					else {
						cell.SetCellType(UGCT_NORMAL);
						cell.SetText(txt);
					}
					SetCell(15,i,&cell);
			}
	int ind;
	if(numtsv>0&&NumLinesSV>0) 
		ind=numtsv;
	else
		ind=-1;		
	if(NumLinesSV>0) {
		QuickSetText(0,ind, "#");
		QuickSetText(1,ind, "Singe Variable Type");
		QuickSetText(2,ind, "Group");
		QuickSetText(3,ind, "Simulated");
		QuickSetText(4, ind, "Obs Var");
		QuickSetText(6,ind, "n");
		QuickSetText(10,ind, "E");
		QuickSetText(11,ind, "RSE");
		QuickSetText(12,ind, "Simulated");
		QuickSetText(13,ind, "Measured");
		QuickSetText(14,ind, "L Likelihood");
						
		for(int i=0;i<15;i++)
			QuickSetBackColor(i,ind,m_ColorHeading);
		ind++;
		for (int i = 0; i < NumLinesSV; i++) {
			sumv = m_pSimDoc->GetValSumStruct(i);
			QuickSetText(0, ind + i, FUtil::STD_ItoAscii(i + 1));
			if (sumv.SimValueSource == SIMSUMSOURCE::MULTIFILESTAT) {
				UnJoinCells(2, ind + i);
				UnJoinCells(3, ind + i);
				QuickSetText(1, ind + i, m_pSimDoc->m_ValidationData.GetStringValSumVarOutputType(i));
				test = m_pSimDoc->m_ValidationData.GetValSumVarGroup(i).c_str();
				QuickSetText(2, ind + i, test);
				QuickSetBackColor(2, ind + i, MFC_Util::GetGroupColor(p_ModelInfo->GetGroupCategory(string(test))));
				QuickSetText(3, ind + i, m_pSimDoc->m_ValidationData.GetValSumVarName(i));
			}
			else if (sumv.SimValueSource == SIMSUMSOURCE::CSVFLEXFILE) {
				string str;
				str = "Residuals (";
				str += FUtil::STD_ItoAscii(sumv.Sim_Value_Residual_Index+1);
				str += ")";
				QuickSetText(1, ind + i, str);
				JoinCells(2, ind + i, 3, ind + i);
				str = sumv.Name;
				QuickSetText(2, ind + i, str);

			}
			else if (sumv.SimValueSource == SIMSUMSOURCE::MULTI_OUTPGFILE) {
				string str;
				str = "MultiSim (";
				str += FUtil::STD_ItoAscii(sumv.Sim_Value_Residual_Index + 1);
				str += ")";
				QuickSetText(1, ind + i, str);
				UnJoinCells(2, ind + i);
				UnJoinCells(3, ind + i);
				str = sumv.Name;
				QuickSetText(2, ind + i, str);
				auto min = sumv.MultiBin_StartMin;
				str=PGUtil::cdatum(min);
				QuickSetText(3, ind + i, str);

			}
					
					if (multivalid&&m_pSimDoc->m_ValSum_Array[i].FlexSumVar) {
						QuickSetText(4, ind+i, m_pSimDoc->m_ValSum_Array[i].IDVarName);
					}
					

					GetCell(6, ind + i, &cell);

					if (m_pSimDoc->m_ValSum_Array[i].FlexSumVar&&m_pSimDoc->m_ValSum_Array[i].ObsScaling==1) {
						cell.SetText(MFC_Util::ItoAscii(sumv.n_RegMulti)+" Meas Scaled");
					}
					else if (m_pSimDoc->m_ValSum_Array[i].FlexSumVar&&m_pSimDoc->m_ValSum_Array[i].ObsScaling == 2) {
						cell.SetText(MFC_Util::ItoAscii(sumv.n_RegMulti) + " Sim Scaled");
					}
					else if (m_pSimDoc->m_ValSum_Array[i].FlexSumVar) {
						cell.SetText(MFC_Util::ItoAscii(sumv.n_RegMulti) + " Original");

					}
					else {
						cell.SetNumber(m_pSimDoc->m_ValidationData.GetValn(i));

					}


					SetCell(6, ind + i, &cell);
					if (sumv.R2_Multi.size() > 0) {
						GetCell(7, ind + i, &cell);
						if (m_pSimDoc->m_MultiRun_Array.size() > 1) {
							cell.SetCellType(UGCT_CHECKBOX);
							cell.SetBool(FALSE);
							cell.SetLabelText(MFC_Util::FtoAscii(sumv.R2_Multi_mean));
						}
						else {
							cell.SetCellType(UGCT_NORMAL);
							cell.SetBool(FALSE);
							cell.SetText(MFC_Util::FtoAscii(sumv.R2_Multi_mean));

						}
						SetCell(7, ind + i, &cell);
						
						GetCell(8, ind + i, &cell);
						if (m_pSimDoc->m_MultiRun_Array.size() > 1) {
							cell.SetCellType(UGCT_CHECKBOX);
							cell.SetBool(FALSE);
							cell.SetLabelText(MFC_Util::FtoAscii(sumv.InterCept_Multi_mean));
						}
						else {
							cell.SetCellType(UGCT_NORMAL);
							cell.SetBool(FALSE);
							cell.SetText(MFC_Util::FtoAscii(sumv.InterCept_Multi_mean));


						}
						SetCell(8, ind + i, &cell);
						
						GetCell(9, ind + i, &cell); 
						if (m_pSimDoc->m_MultiRun_Array.size() > 1) {
							cell.SetCellType(UGCT_CHECKBOX);
							cell.SetBool(FALSE);
							cell.SetLabelText(MFC_Util::FtoAscii(sumv.Slope_Multi_mean)); SetCell(9, ind + i, &cell);
						}
						else {
							cell.SetCellType(UGCT_NORMAL);
							cell.SetBool(FALSE);
							cell.SetText(MFC_Util::FtoAscii(sumv.Slope_Multi_mean));
						}
						SetCell(9, ind + i, &cell);
					}

					//JoinCells(6, ind+i, 9, ind+i);

					GetCell(10,ind+i,&cell);
					if(multivalid) {
						cell.SetCellType(UGCT_CHECKBOX);
						cell.SetBool(FALSE);
						txt=MFC_Util::FtoAscii(sumv.ME_mean);
						cell.SetLabelText(txt);
					}
					else {
						cell.SetCellType(UGCT_NORMAL); 
						cell.SetNumber(m_pSimDoc->m_ValidationData.GetValSumVarME(i));
					}
					SetCell(10,ind+i,&cell);

					GetCell(11,ind+i,&cell);
					if(multivalid&&m_pSimDoc->m_MultiRun_Array.size() > 1) {
						cell.SetCellType(UGCT_CHECKBOX);
						cell.SetBool(FALSE);
						cell.SetLabelText(MFC_Util::FtoAscii(sumv.RMSE_mean));
					}
					else {
						cell.SetCellType(UGCT_NORMAL); 
						cell.SetNumber(m_pSimDoc->m_ValidationData.GetValSumVarRMSE(i));
					}
					SetCell(11,ind+i,&cell);

					//QuickSetNumber(12,ind+i,m_pSimDoc->m_ValSumValues_Stored[i].SimValue);
					
					GetCell(12, ind + i, &cell);
					if (multivalid&&m_pSimDoc->m_ValSum_Array[i].FlexSumVar) {
						cell.SetCellType(UGCT_CHECKBOX);
						cell.SetBool(FALSE);
						cell.SetLabelText(MFC_Util::FtoAscii(sumv.SimValue));
					}
					else {
						cell.SetCellType(UGCT_NORMAL);
						cell.SetNumber(m_pSimDoc->m_ValSum_Array[i].SimValue);
					}
					SetCell(12, ind + i, &cell);
					
					
					
					
					GetCell(13, ind + i, &cell);
					if (multivalid&&m_pSimDoc->m_ValSum_Array[i].FlexSumVar) {
						cell.SetCellType(UGCT_CHECKBOX);
						cell.SetBool(FALSE);
						cell.SetLabelText(MFC_Util::FtoAscii(sumv.ObsValue));
					}
					else {
						cell.SetCellType(UGCT_NORMAL);
						cell.SetNumber(m_pSimDoc->m_ValidationData.GetValSumVarMVAL(i));
					}

					SetCell(13, ind + i, &cell);

					GetCell(14,ind+i,&cell);
					if(multivalid&&m_pSimDoc->m_MultiRun_Array.size() > 1) {
						cell.SetCellType(UGCT_CHECKBOX);
						cell.SetBool(FALSE);
					
						cell.SetLabelText(MFC_Util::FtoAscii(sumv.LogLi_mean));
					}
					else {
						cell.SetCellType(UGCT_NORMAL); 
						cell.SetNumber(m_pSimDoc->m_ValidationData.GetValSumVarLogLi(i));
					}
					SetCell(14, ind + i, &cell);


					GetCell(15, ind + i, &cell);
					if (multivalid&&m_pSimDoc->m_ValSum_Array[i].FlexSumVar&&m_pSimDoc->m_MultiRun_Array.size() > 1) {
						cell.SetCellType(UGCT_CHECKBOX);
						cell.SetBool(FALSE);
						cell.SetLabelText(MFC_Util::FtoAscii(sumv.NSE_mean));
					}


					SetCell(15,ind+i,&cell);


		}


	}

// Set CheckBox depending on selections	
	MultiVAL mul;
		for(size_t i=0;i<m_pSimDoc->m_SelectMultiVal.size();i++) {
						mul=m_pSimDoc->m_SelectMultiVal[i];
						GetCell(mul.col,mul.row,&cell);
						cell.SetBool(true);
						//cell.SetBackColor(m_NoColor);
						SetCell(mul.col,mul.row,&cell);
						RedrawCell(mul.col,mul.row);
		}


}

void MakeSimSheets::Val_Post_TimeSerieTable(size_t NumLines, size_t NumLinesSV) {
	string test;
	string txt;
	CUGCell cell;

	bool multivalid=false;
	bool multimeanvalid=true;
	if(!multivalid) multimeanvalid=false;
	if(!m_pSimDoc->m_OutputValidXBinFileExist) multimeanvalid=false;	

	BOOL validplot=m_pSimDoc->IsOutputValidBinFileValid();

		int ncol;
		unsigned int numrec_all;
		CResiduals *pRes;
		BOOL LikelihoodValue;
		CWinApp* pApp = AfxGetApp();
		LikelihoodValue=0;
		LikelihoodValue=MFC_Util::GetProfileIntNo( "LogLikelihoodSwitch", LikelihoodValue);
		if(m_pSimDoc->MR_Storage_Open()) {
			m_bDrawExcelFocus = TRUE;
			SetMultiSelectMode(TRUE);
			double resvalue;
			//NumLines=m_pSimDoc->MR_Get_TotalNumberofRuns();
			NumLines=m_pSimDoc->m_MStorage.GetNumAccepted();
			if(NumLines>30000) NumLines=30000;
			SetNumberRows(10+1);
			ncol=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
			double accsum, accaccsum;
			CFile file;
			file.Open(_T("Dump.csv"),CFile::modeWrite|CFile::modeCreate);
			char buf[100000];
			string strpr, strhead1, strhead2;

			ncol=1;
			strhead1="";
			strhead2="";
			
				for(size_t i=0;i<MAXSIMVAL;i++) {
					auto pValFile=m_pSimDoc->ValidationFilePointer(i+1);
					if(pValFile->GetNumberOfSelectedVal()>0) {
						CPG *pPG;
						pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(0, i+1, TRUE);
						//pPG=(CPG*)pValFile->GetPointer();
						if(pPG==nullptr) {
								pPG=new CPG;
								//CPG_Array.Add(pCPG);
						
						}
						pPG->ReOpen();
						pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(i+1);
						auto numrec=pPG->GetNumRecords();
						auto numvar=pPG->GetNumVariables();
						if(numrec>0) ncol+=numvar/2+1;
						for (size_t jj=0;jj<numvar/2;jj++) {

							for(size_t j=0;j<numrec;j++) {
								float meas=pPG->GetVarValue(jj*2+2,j+1);
								if(abs(meas)<1.E38) ncol++;

							}

						}
					}
				}

			SetNumberCols(ncol);
			numrec_all=0;
			float  meas, err, mrel, mabs;
			double sim, LOG_PI, reli, logli;
			double mcorr, meascorr;
			LOG_PI=0.5*log(2.*3.14159);


			unsigned int index;
			int countvar;
			countvar=0;
			strpr="";
			strhead1="run;";
			strhead2="#;";
			/*for (unsigned int ii=0;ii<NumLines; ii++) {
				strpr=_T("");
				ncol=-1;
				accaccsum=0;
				index=-1;
				unsigned int irun=m_pSimDoc->m_MStorage.Get_AcceptedRuns(ii);
				strpr=MFC_Util::ItoAscii(irun+1)+";";
				for(int i=0;i<8;i++) {
					pValFile=m_pSimDoc->ValidationFilePointer(i);
					if(pValFile->GetNumberOfSelectedVal()>0) {
						CPG *pPG;
						BOOL valid;
						pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(-1, i+1, TRUE);
						//pPG=(CPG*)pValFile->GetPointer();
						if(pPG==nullptr) {
								pPG=new CPG;
								//CPG_Array.Add(pCPG);

						}
						pPG->ReOpen();
						pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(i+1);
						unsigned int numrec=pPG->GetNumRecords();
						unsigned int numvar=pPG->GetNumVariables();
						unsigned int nrec;
						
						
						for (unsigned int jj=0;jj<numvar/2;jj++) {
							
							accsum=0;
							nrec=0;
							index++;	
							mrel=m_pSimDoc->GetValP_Error(index);
							mabs=m_pSimDoc->GetValA_Error(index);
							mcorr=0.0;
							
						
								if(index>=0&&index<3)
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,17));
								if(index>=3&&index<6)
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,16));
								if(index==6) 
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,16));
								else if(index==7)
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,17));
								else if(index==8)
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,18));
								else if(index==9)
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,19));


							
							for(unsigned int j=0;j<numrec;j++) {
	
								meas=pPG->GetVarValue(jj*2+2,j+1);
								if(abs(meas)<1.E38) {
								ncol++;	
								nrec++;
								if(irun==0) {
									QuickSetText(ncol+1,0,pPG->GetDate(j+1));
									QuickSetText(ncol+1,-1,pPG->GetVarName(jj*2+2));
									strhead2+=pPG->GetDate(j+1)+";";	
									strhead1+=pPG->GetVarName(j+1)+";";
								}
								resvalue=pRes->GetResiduals(irun, jj, j);
								if(mcorr!=0.0) 
									meascorr=double(meas)*pow(double(10.),mcorr);
								else
									meascorr=meas;

								sim=meas-resvalue;
								if(abs(meas)*mrel<mabs&&mabs>0) 
									err=mabs;	
								else if(abs(meas)>0.)
									err=mrel*abs(meas);
								else if(abs(sim-meas)>0.)
									err=mrel*abs(sim);
								else
									err=1;


								reli=(sim-meascorr)/err;
								if(abs(reli)<1.E-10)
									logli=-LOG_PI-log(2*err);
								else
									logli=log(1-exp(-0.5*reli*reli))-log(reli*reli)-LOG_PI-log(err);
								accsum+=logli;
								if(LikelihoodValue) {
									QuickSetNumber(ncol+1, ii+1, logli);
									strpr+=MFC_Util::FtoAscii(logli)+";";
								}
								else {
									QuickSetNumber(ncol+1, ii+1, resvalue);
									strpr+=MFC_Util::FtoAscii(resvalue)+";";

								}

							}

							
							}
							ncol++;
							accaccsum+=accsum;
							if(irun==0) {
								QuickSetText(ncol+1,-1,"LogLikelihood");
								QuickSetText(ncol+1,0,"Sum");
								strhead2+="Sum;";	
								strhead1+="LogLikelihood;";
							}
							QuickSetNumber(ncol+1,ii+1, accsum);
							strpr+=MFC_Util::FtoAscii(accsum)+";";

							QuickSetNumber(0,ii+1, irun+1);
							countvar++;
						}
					}


				}	
				ncol++;
				QuickSetText(ncol+1,-1,"LogLikelihood");
				QuickSetText(ncol+1,0,"Global Sum");
				QuickSetNumber(ncol+1,ii+1, accaccsum);
				if(irun==0) {

					strhead2+="Global Sum\r\n";	
					strhead1+="LogLikelihood\r\n";
					int nc=strhead1.GetLength();
					for(int jj=0;jj<nc;jj++)
						buf[jj]=strhead1.GetAt(jj);
					file.Write(&buf,nc);
					nc=strhead2.GetLength();
					for(int jj=0;jj<nc;jj++)
						buf[jj]=strhead2.GetAt(jj);
					file.Write(&buf,nc);

				}
				strpr+=MFC_Util::FtoAscii(accaccsum)+"\r\n";

				int nc=strpr.GetLength();
				for(int jj=0;jj<nc;jj++)
					buf[jj]=strpr.GetAt(jj);
				file.Write(&buf,nc);

			}
			/*
			for (unsigned int ii=0;ii<NumLines; ii++) {
				strpr=_T("");
				ncol=-1;
				accaccsum=0;
				index=-1;
				unsigned int irun=m_pSimDoc->m_MStorage.Get_AcceptedRuns(ii);
				strpr=FUtil::STD_ItoAscii(irun+1)+";";
				for(int i=0;i<8;i++) {
					pValFile=m_pSimDoc->ValidationFilePointer(i);
					if(pValFile->GetNumberOfSelectedVal()>0) {
						CPG *pPG;
						BOOL valid;
						pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(-1, i+1, TRUE);
						//pPG=(CPG*)pValFile->GetPointer();
						if(pPG==nullptr) {
								pPG=new CPG;
								//CPG_Array.Add(pCPG);

						}
						pPG->ReOpen();
						pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(i+1);
						unsigned int numrec=pPG->GetNumRecords();
						unsigned int numvar=pPG->GetNumVariables();
						unsigned int nrec;
						
						
						for (unsigned int jj=0;jj<numvar/2;jj++) {
							
							accsum=0;
							nrec=0;
							index++;	
							mrel=m_pSimDoc->GetValP_Error(index);
							mabs=m_pSimDoc->GetValA_Error(index);
							mcorr=0.0;
							
						
								if(index>=0&&index<3)
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,17));
								if(index>=3&&index<6)
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,16));
								if(index==6) 
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,16));
								else if(index==7)
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,17));
								else if(index==8)
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,18));
								else if(index==9)
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,19));


							
							for(unsigned int j=0;j<numrec;j++) {
	
								meas=pPG->GetVarValue(jj*2+2,j+1);
								if(abs(meas)<1.E38) {
								ncol++;	
								nrec++;
								if(irun==0) {
									QuickSetText(ncol+1,0,pPG->GetDate(j+1));
									QuickSetText(ncol+1,-1,pPG->GetVarName(jj*2+2));
									strhead2+=pPG->GetDate(j+1)+";";	
									strhead1+=pPG->GetVarName(j+1)+";";
								}
								resvalue=pRes->GetResiduals(irun, jj, j);
								if(mcorr!=0.0) 
									meascorr=double(meas)*pow(double(10.),mcorr);
								else
									meascorr=meas;

								sim=meas-resvalue;
								if(abs(meas)*mrel<mabs&&mabs>0) 
									err=mabs;	
								else if(abs(meas)>0.)
									err=mrel*abs(meas);
								else if(abs(sim-meas)>0.)
									err=mrel*abs(sim);
								else
									err=1;


								reli=(sim-meascorr)/err;
								if(abs(reli)<1.E-10)
									logli=-LOG_PI-log(2*err);
								else
									logli=log(1-exp(-0.5*reli*reli))-log(reli*reli)-LOG_PI-log(err);
								accsum+=logli;
								if(LikelihoodValue) {
									QuickSetNumber(ncol+1, ii+1, logli);
									strpr+=MFC_Util::FtoAscii(logli)+";";
								}
								else {
									QuickSetNumber(ncol+1, ii+1, resvalue);
									strpr+=MFC_Util::FtoAscii(resvalue)+";";

								}

							}

							
							}
							ncol++;
							accaccsum+=accsum;
							if(irun==0) {
								QuickSetText(ncol+1,-1,"LogLikelihood");
								QuickSetText(ncol+1,0,"Sum");
								strhead2+="Sum;";	
								strhead1+="LogLikelihood;";
							}
							QuickSetNumber(ncol+1,ii+1, accsum);
							strpr+=MFC_Util::FtoAscii(accsum)+";";

							QuickSetNumber(0,ii+1, irun+1);
							countvar++;
						}
					}


				}	
				ncol++;
				QuickSetText(ncol+1,-1,"LogLikelihood");
				QuickSetText(ncol+1,0,"Global Sum");
				QuickSetNumber(ncol+1,ii+1, accaccsum);
				if(irun==0) {

					strhead2+="Global Sum\r\n";	
					strhead1+="LogLikelihood\r\n";
					int nc=strhead1.GetLength();
					for(int jj=0;jj<nc;jj++)
						buf[jj]=strhead1.GetAt(jj);
					file.Write(&buf,nc);
					nc=strhead2.GetLength();
					for(int jj=0;jj<nc;jj++)
						buf[jj]=strhead2.GetAt(jj);
					file.Write(&buf,nc);

				}
				strpr+=MFC_Util::FtoAscii(accaccsum)+"\r\n";

				int nc=strpr.GetLength();
				for(int jj=0;jj<nc;jj++)
					buf[jj]=strpr.GetAt(jj);
				file.Write(&buf,nc);

			}
			*/
			for (size_t ii=0;ii<NumLines; ii++) {
				strpr="";
				ncol=-1;
				accaccsum=0;
				index=-1;
				auto irun=m_pSimDoc->m_MStorage.Get_AcceptedRuns(ii);
				strpr=FUtil::STD_ItoAscii(irun+1)+";";
				for(size_t i=0;i<MAXSIMVAL;i++) {
					auto pValFile=m_pSimDoc->ValidationFilePointer(i+1);
					if(pValFile->GetNumberOfSelectedVal()>0) {
						CPG *pPG;
						pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(0, i+1, TRUE);
						//pPG=(CPG*)pValFile->GetPointer();
						if(pPG==nullptr) {
								pPG=new CPG;
								//CPG_Array.Add(pCPG);

						}
						pPG->ReOpen();
						pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(i+1);
						auto numrec=pPG->GetNumRecords();
						auto numvar=pPG->GetNumVariables();
						auto nrec =0;
						
						
						for (unsigned int jj=0;jj<numvar/2;jj++) {
							
							accsum=0;
							nrec=0;
							index++;	
							mrel=m_pSimDoc->m_ValidationData.GetValP_Error(index);
							mabs=m_pSimDoc->m_ValidationData.GetValA_Error(index);
							mcorr=0.0;
							
						
								if(index>=0&&index<3)
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,17));
								if(index>=3&&index<6)
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,16));
								if(index==6) 
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,16));
								else if(index==7)
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,17));
								else if(index==8)
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,18));
								else if(index==9)
									mcorr=double(m_pSimDoc->m_MStorage.GetMultiP(irun,19));


							
							for(auto j=0;j<numrec;j++) {
	
								meas=pPG->GetVarValue(jj*2+2,j+1);
								if(abs(meas)<1.E38) {
								ncol++;	
								nrec++;
								if(irun==0) {
									//QuickSetText(ncol+1,0,pPG->GetDate(j+1));
									//QuickSetText(ncol+1,-1,pPG->GetVarName(jj*2+2));
									strhead2+=pPG->GetDate(j+1);strhead2+=";";	
									strhead1+=pPG->GetVarName(j+1);strhead1+=";";
								}
								resvalue=pRes->GetResiduals(irun, jj, j);
								if(mcorr!=0.0) 
									meascorr=double(meas)*pow(double(10.),mcorr);
								else
									meascorr=meas;

								sim=meas-resvalue;
								if(abs(meas)*mrel<mabs&&mabs>0) 
									err=mabs;	
								else if(abs(meas)>0.)
									err=mrel*abs(meas);
								else if(abs(sim-meas)>0.)
									err=mrel*abs(sim);
								else
									err=1;


								reli=(sim-meascorr)/err;
								if(abs(reli)<1.E-10)
									logli=-LOG_PI-log(2*err);
								else
									logli=log(1-exp(-0.5*reli*reli))-log(reli*reli)-LOG_PI-log(err);
								accsum+=logli;
								if(LikelihoodValue) {
									//QuickSetNumber(ncol+1, ii+1, logli);
									strpr+=FUtil::STD_FtoAscii(logli)+";";
								}
								else {
									//QuickSetNumber(ncol+1, ii+1, resvalue);
									strpr+=FUtil::STD_FtoAscii(resvalue)+";";

								}

							}

							
							}
							ncol++;
							accaccsum+=accsum;
							if(irun==0) {
								//QuickSetText(ncol+1,-1,"LogLikelihood");
								//QuickSetText(ncol+1,0,"Sum");
								strhead2+="Sum;";	
								strhead1+="LogLikelihood;";
							}
							//QuickSetNumber(ncol+1,ii+1, accsum);
							strpr+=FUtil::STD_FtoAscii(accsum)+";";

							//QuickSetNumber(0,ii+1, irun+1);
							countvar++;
						}
					}


				}	
				ncol++;
				//QuickSetText(ncol+1,-1,"LogLikelihood");
				//QuickSetText(ncol+1,0,"Global Sum");
				//QuickSetNumber(ncol+1,ii+1, accaccsum);
				if(irun==0) {

					strhead2+="Global Sum\r\n";	
					strhead1+="LogLikelihood\r\n";
					int nc=strhead1.size();
					for(int jj=0;jj<nc;jj++)
						buf[jj]=strhead1[jj];
					file.Write(&buf,nc);
					nc=strhead2.size();
					for(int jj=0;jj<nc;jj++)
						buf[jj]=strhead2[jj];
					file.Write(&buf,nc);

				}
				strpr+=FUtil::STD_FtoAscii(accaccsum)+"\r\n";

				int nc=strpr.size();
				for(int jj=0;jj<nc;jj++)
					buf[jj]=strpr[jj];
				file.Write(&buf,nc);

			}
			file.Close();
		}
}
void MakeSimSheets::Val_Post_CoVarRes() {
CUGCell cell;


	if (!m_pSimDoc->m_MStorage.IsCoCorValidSet())
		m_pSimDoc->UpdateCoVar();



		int numpar=m_pSimDoc->MR_Get_NumBayesPar();
		if(numpar==0) 
			numpar=m_pSimDoc->MR_Get_NumTotalPar();
		
		int ncount=numpar+m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val()*6;
		int numval=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
		SetSheetNumber(6); 
		SetSH_NumberCols(2);
		SetSH_Width(350);
		SetTH_NumberRows(2);
		SetTH_Height(60);
		SetSH_ColWidth(-2,200);
		SetSH_ColWidth(-1,150);
		SetNumberCols(ncount); 
		SetNumberRows(ncount-numpar); 
		//SetColWidth(-2,200);
		//SetColWidth(-1,150);
		QuickSetText(0,-1,"Module");
		QuickSetText(1,-1,"Parameter");
		for(int i=0;i<ncount;i++) {
			QuickSetText(i,-1,"");
			SetColWidth(i, 80);
			UnJoinCells(i,-1);
			UnJoinCells(i,-2);
			if(i-numpar>=0) {
				UnJoinCells(-1,i-numpar);
				UnJoinCells(-2,i-numpar);
			}
		}
		



		int ind=0;
		string str;
		for(int i=numpar;i<ncount;i+=numval) {			
					JoinCells(i,-2,i+numval-1,-2);
					JoinCells(-2,i-numpar,-2,i-numpar+numval-1);
					switch(ind) {
					case 0:  str="R2"; break;
					case 1:  str="A0"; break;
					case 2:  str="A1"; break;
					case 3:  str="ME"; break;
					case 4:  str="RMSE"; break;
					case 5:  str="LogLi"; break;			
					}
					QuickSetText(i,-2,str);
					QuickSetText(-2,i-numpar,str);
					ind++;

		}
		for(int i=numpar;i<ncount;i++) {
			for(int ii=0;ii<ncount;ii++) {
				if(ii<numpar) {
					string txt=m_pSimDoc->MR_Get_BayesGroup(ii);
					QuickSetText(ii,-1,m_pSimDoc->MR_Get_CalibParName(ii));
					QuickSetText(ii,-2,txt);
					QuickSetBackColor(ii,-1,m_ColorHeading);
					QuickSetBackColor(ii,-2,MFC_Util::GetGroupColor(p_ModelInfo->GetGroupCategory(txt)));
				}
				else if(i==ii) {
					int iv2;
					string str2;
					int numval=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();


					iv2=(i-numpar)%numval;
					str2=m_pSimDoc->m_ValidationData.GetValName(iv2);
					int ivv=m_pSimDoc->m_ValidationData.GetValLocalIndex(iv2);
					if(ivv>-1) str2+="("+FUtil::STD_ItoAscii(ivv+1)+")";
					QuickSetText(-1, i-numpar, str2);
					QuickSetText(i,-1, str2);
					QuickSetBackColor(-1,i-numpar, m_ColorHeading);
				}

				GetCell(ii,i-numpar,&cell);
				cell.SetCellType(UGCT_NORMAL);
				if(i>ii) {
						float v=m_pSimDoc->m_MStorage.GetCoCorValid(ii,i);
						cell.SetNumber(v);
						if(v<-0.8) cell.SetBackColor(RGB(100,100,240));
						else if(v<-0.4&&v>=-0.8) cell.SetBackColor(RGB(30,240,240));
						else if(v>0.4&&v<=0.8) cell.SetBackColor(RGB(240,240,150));
						else if(v>0.8) cell.SetBackColor(RGB(240,100,100));
						else
							cell.SetBackColor(m_NoColor);

				}
				else {
						cell.SetText("");
						cell.SetBackColor(m_NoColor);
				}
				SetCell(ii,i-numpar,&cell);
			}

		}
		QuickSetText(-1,-1, "Co Correlation matrix");
		RedrawAll();

}
size_t MakeSimSheets::Val_Prior_Header() {

		SetNumberCols(12); 
		SetColWidth(0,40);
		SetColWidth(1,40);
		SetColWidth(2,40);
		SetColWidth(3,220);
		SetColWidth(4,150);
		SetColWidth(5,150);
		SetColWidth(6,100);
		SetColWidth(7,100);
		SetColWidth(11,150);
		QuickSetText(0,-1, "#");
		QuickSetText(1,-1, "File");
		QuickSetText(2,-1, "Pos");
		QuickSetText(3,-1, "Measured");
		QuickSetText(4,-1, "Group");
		QuickSetText(5,-1, "Simulated Variable");
		QuickSetText(6,-1, "Accumulated");
		QuickSetText(7,-1, "Log Trans");
		QuickSetText(8,-1, "Rel Error");
		QuickSetText(9,-1, "Abs Error");
		QuickSetText(10,-1, "Duration (min)");
		QuickSetText(11,-1, "Log Scaling of Obs");

		return GetNumberRows();
}
void MakeSimSheets::Val_Prior_Data(size_t NumLines, size_t NumLinesSV) {
	string test;
	CString txt;
	CUGCell cell;
	int ind;
	SimB *pSw;
	P *pObserved = nullptr;
	pSw = m_pSimDoc->GetPtr(TABLE, "Scaling of Observed Variables");
	pObserved=(P*)m_pSimDoc->GetPtr(PAR_TABLE,"Scaling Observed");

	int numtsv;
	if(NumLinesSV>0)
		numtsv=NumLines-NumLinesSV-1;
	else
		numtsv=NumLines;
	


	for(int i=0;i<numtsv;i++) {
			test=FUtil::STD_ItoAscii(i+1);
			QuickSetText(0, i, test);
			size_t FileNo=m_pSimDoc->m_ValidationData.GetValFileNumber(i);
			QuickSetText(1, i,FUtil::STD_ItoAscii(FileNo));
			auto pValFile=m_pSimDoc->ValidationFilePointer(FileNo);
			int ind=m_pSimDoc->m_ValidationData.GetValFileIndex(i);
			QuickSetText(2, i,FUtil::STD_ItoAscii(ind+1));

			CPG *pPG;
			BOOL valid;
			pPG=(CPG*)pValFile->GetPointer();
			if(pPG==nullptr) {
				pPG=new CPG;
				//CPG_Array.Add(pCPG);
				valid=pPG->ReadContentOfPGFileAndOpenMainPGStreamForReading(pValFile->GetStrValue().c_str());
				if(valid) pValFile->SetPointer(pPG);
				pPG->CloseFile();
			}
						
			test=pPG->GetVarName(ind+1).c_str();test+=pPG->GetVarId(ind+1).c_str();
			QuickSetText(3,i, test);
			test=m_pSimDoc->m_ValidationData.GetValGroup(i).c_str();
			QuickSetText(4,i, test);
			QuickSetBackColor(4,i,MFC_Util::GetGroupColor(p_ModelInfo->GetGroupCategory(string(test))));

			test=m_pSimDoc->m_ValidationData.GetValName(i).c_str();
			auto iv=m_pSimDoc->m_ValidationData.GetValLocalIndex(i);
			if(iv>-1) test+="("+FUtil::STD_ItoAscii(iv+1)+")";
			QuickSetText(5, i, test);
						
			BOOL testV=m_pSimDoc->m_ValidationData.GetVal_AccTestValue(i);
			GetCell(6,i,&cell);
			cell.SetCellType(UGCT_CHECKBOX);
			if(testV)
				cell.SetBool(TRUE);
				//QuickSetText(4,i,"x");
			else
				cell.SetBool(FALSE);
				//QuickSetText(4,i,"-");
			SetCell(6,i,&cell);

			GetCell(7,i,&cell);
						
			testV=m_pSimDoc->m_ValidationData.GetValFileLogTrans(i);
			cell.SetCellType(UGCT_CHECKBOX);
			if(testV)
				cell.SetBool(TRUE);
				//QuickSetText(4,i,"x");
			else
				cell.SetBool(FALSE);
				//QuickSetText(4,i,"-");
			SetCell(7,i,&cell);


			GetCell(8,i,&cell);
			if(m_pSimDoc->m_ValidationData.GetValP_Error(i)<0) {
				m_pSimDoc->m_ValidationData.SetValErrors(i,0.15f,0.1f);
			}
			cell.SetNumber(m_pSimDoc->m_ValidationData.GetValP_Error(i));
			SetCell(8,i,&cell);
			GetCell(9,i,&cell);	
			cell.SetNumber(m_pSimDoc->m_ValidationData.GetValA_Error(i));
			SetCell(9,i,&cell);					
			GetCell(10,i,&cell);	
			cell.SetNumber(m_pSimDoc->m_ValidationData.GetValDuration(i));
			SetCell(10,i,&cell);
			if(pObserved!=nullptr)
				cell.SetNumber(pObserved->GetValue(i));
			SetCell(11,i,&cell);
	
		}
			
	if(numtsv>0&&NumLinesSV>0) 
		ind=numtsv;
	else
		ind=-1;
					
	if(NumLinesSV>0) {
			CString optionlist,optiontext;
			optionlist="Initial\nFinal\nMinimum\nMaximum\nMean\nAccumulated\n";
			QuickSetText(0,ind, "#");
			QuickSetText(3,ind, "Single Variable Type");
			QuickSetText(4,ind, "Group");
			QuickSetText(5,ind, "Simulated Variale");
			QuickSetText(6,ind, "Observed Value");
			QuickSetText(7,ind, "Rel Error");
			QuickSetText(8,ind, "Abs Error");
			JoinCells(8,ind,10,ind);
			for(int i=0;i<9;i++)
				QuickSetBackColor(i,ind,m_ColorHeading);
			ind++;
			for(int i=0;i<NumLinesSV;i++) {
				QuickSetText(0,ind+i,FUtil::STD_ItoAscii(i+1));
				GetCell(3,ind+i,&cell);
				cell.SetCellType(UGCT_DROPLIST);
				cell.SetLabelText(optionlist);
				optiontext=m_pSimDoc->m_ValidationData.GetStringValSumVarOutputType(i).c_str();
				cell.SetText(optiontext);
				SetCell(3,ind+i,&cell);



				//QuickSetText(1,ind+i,m_pSimDoc->GetStringValSumVarOutputType(i));
				QuickSetText(4,ind+i,m_pSimDoc->m_ValidationData.GetValSumVarGroup(i));
				QuickSetText(5,ind+i,m_pSimDoc->m_ValidationData.GetValSumVarName(i));

				QuickSetNumber(6,ind+i,m_pSimDoc->m_ValidationData.GetValSumVarMVAL(i));
				QuickSetNumber(7,ind+i,m_pSimDoc->m_ValidationData.GetValSumVarP_Error(i));
				QuickSetNumber(8,ind+i,m_pSimDoc->m_ValidationData.GetValSumVarA_Error(i));
				JoinCells(0,ind+i,2,ind+i);
				JoinCells(8,ind+i,10,ind+i);
			}
	}
	RedrawAll();


}
void MakeSimSheets::Multi_Post_CompleteList_Header() {
		SetColWidth(0,50);
		SetColWidth(1,150);
		SetColWidth(2,150);
		SetColWidth(3,200);
		SetColWidth(4,80);
		SetColWidth(5,80);
		SetColWidth(6,80);
							
		auto NumLines=m_pSimDoc->m_MStorage.GetNumAccepted();
		int ncol;
		if(m_pSimDoc->m_DocFile.m_FileVersionNumber>40)
			ncol=m_pSimDoc->MR_Get_NumTotalPar()+10;
		else
			ncol=m_pSimDoc->MR_Get_NumTotalPar()+5;

		if(m_pSimDoc->IsBayesianCalibration()) 		
			SetNumberRows(NumLines+14);
		else
			SetNumberRows(NumLines+5);
		SetNumberCols(ncol+2);
			SetColWidth(0,40);
			SetColWidth(1,60);
		for(int i=0;i<ncol;i++)
				SetColWidth(2+i,80);

	
		QuickSetText(0,-1,"Multi #");
		QuickSetText(1,-1,"Run #");
	
}
void MakeSimSheets::Multi_Post_CompleteList() {
CUGCell cell;
CString str;
			auto NumLines=GetNumberRows()-5;
			auto ncol=GetNumberCols();
			auto RunNo=m_pSimDoc->m_DocFile.m_SimulationRunNo-m_pSimDoc->MR_Get_ActualRunNumber();

			int ind=0;
			int nTabIndex;
			SimB* pPar;
			for(int i=0;i<m_pSimDoc->MR_GetNumberOfDimensions();i++){
					for(int ii=0; ii<m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i+1);ii++){
						pPar=m_pSimDoc->MR_Get_pPar(i+1,ii+1);
						nTabIndex=m_pSimDoc->MR_GetTabIndex(i+1,ii+1);
						string namep=pPar->GetName();
						if(nTabIndex>=0)				
							namep+="("+FUtil::STD_ItoAscii(nTabIndex+1)+")";
						ind++;
						QuickSetText(1+ind,-1,namep);
					}
			}
			ind++;
			ind++;	QuickSetText(ind,-1,"Accepted");
			ind++;	QuickSetText(ind,-1,"TSV LogLi");
			ind++;	QuickSetText(ind,-1,"Sum LogLi");
			ind++;	QuickSetText(ind,-1,"Tot LogLi");
			ind++;	QuickSetText(ind,-1,"MAP RunNo");
			ind++;	QuickSetText(ind,-1,"Acc Sim Time");
			ind++;	QuickSetText(ind,-1,"n");
			ind++;	QuickSetText(ind,-1,"SSE");
			ind++;	QuickSetText(ind,-1,"AIC");
			ind++;	QuickSetText(ind,-1,"BIC");
		
			int ncount=m_pSimDoc->MR_Get_NumBayesPar();
			for(int ii=0;ii<NumLines;ii++) {
				auto irun=m_pSimDoc->m_MStorage.Get_AcceptedRuns(ii);
				if (irun == string::npos) irun = ii;
				GetCell(0,ii, &cell);
				str.Format(_T("%u"),irun+1);
				cell.SetText(str);
				cell.SetReadOnly(TRUE);
				SetCell(0,ii,&cell);
				//QuickSetNumber(0,ii,double(ii+1));
				if(!m_pSimDoc->IsBayesianCalibration())
					auto irun_no=m_pSimDoc->m_MStorage.GetMultiRunNo(ii)-1;
				QuickSetNumber(1,ii,irun+1+RunNo);

				for(int i=0; i<m_pSimDoc->m_MStorage.m_NumMulti;i++) {
						GetCell(2+i,ii,&cell);

						cell.SetNumber(double(m_pSimDoc->m_MStorage.GetMultiP(irun,i)));
						SetCell(2+i,ii,&cell);
	
				}
			}
			JoinCells(2+m_pSimDoc->MR_Get_NumBayesPar(),NumLines,ncol+1,NumLines+4);
			QuickSetText(2+m_pSimDoc->MR_Get_NumBayesPar(), NumLines,"Accepted Ensemble");
			for(int ii=0;ii<5;ii++) {
				switch(ii) {
					case 0: str="Mean";
						break;
					case 1: str="St.D";
						break;
					case 2: str="CV";
						break;
					case 3: str="Min";
						break;
					case 4: str="Max";
						break;
				}

				QuickSetText(0,NumLines+ii, str);
				JoinCells(0,NumLines+ii,1,NumLines+ii);
				int imethod;
				//Check if mean value is calculated
				if(!m_pSimDoc->m_MStorage.m_Lock&&ncount>0) {
					if(!m_pSimDoc->m_MStorage.IsCoCorValidSet()) 
						m_pSimDoc->UpdateCoVar();
				}

				for(int i=0; i<ncol-10;i++) {
					DistStat dist=m_pSimDoc->GetPostDist(i);
					float v, vmean;
					switch(ii) {
					case 0:
						imethod=m_pSimDoc->MR_Get_BayesMethod(i);	
						if(imethod==11&&m_pSimDoc->m_MStorage.m_Lock)
							v=dist.Mean;//exp(double(m_pSimDoc->MR_Get_BayesMean(i)));
						else
							v=dist.Mean;//m_pSimDoc->MR_Get_BayesMean(i);
						break;
					case 1: 
						imethod=m_pSimDoc->MR_Get_BayesMethod(i);
						if(imethod==11&&m_pSimDoc->m_MStorage.m_Lock) {
							v=dist.Std;//exp(sqrt(m_pSimDoc->MR_Get_BayesCoCor(i,i)));
						}
						else {
							v=dist.Std;//sqrt(m_pSimDoc->MR_Get_BayesCoCor(i,i));
						}
						break;
					case 2: 
						imethod=m_pSimDoc->MR_Get_BayesMethod(i);
						if(imethod==11&&m_pSimDoc->m_MStorage.m_Lock) {
							vmean=exp(double(m_pSimDoc->MR_Get_BayesMean(i)));
							v=exp(double(m_pSimDoc->MR_Get_BayesMean(i)-m_pSimDoc->MR_Get_BayesCoCor(i,i)));
							v=(vmean-v)/vmean;
							if(abs(vmean)>0) v=dist.Std/abs(dist.Mean);// v=sqrt(v)/abs(vmean);
						}
						else {
							vmean=m_pSimDoc->MR_Get_BayesMean(i);
							v=m_pSimDoc->MR_Get_BayesCoCor(i,i);
							if(abs(dist.Mean)>0) v=dist.Std/abs(dist.Mean);
						}
						
						break;
					case 3: v=dist.Min;//m_pSimDoc->m_MStorage.GetMinValidPar(i);
						break;
					case 4: v=dist.Max;//m_pSimDoc->m_MStorage.GetMaxValidPar(i);
						break;
					}
					auto noparindex = m_pSimDoc->MR_GetNoTableParIndex(i);
					GetCell(2+noparindex,ii+NumLines,&cell);
					cell.SetNumber(double(v));
					SetCell(2+noparindex,ii+NumLines,&cell);
						//QuickSetNumber(2+i,ii,double(m_pSimDoc->m_MStorage.GetMultiP(ii,i))); 	
				}
			}
			if(m_pSimDoc->IsBayesianCalibration()) {
				JoinCells(2+m_pSimDoc->MR_Get_NumBayesPar(),NumLines+5,4+m_pSimDoc->MR_Get_NumBayesPar(),NumLines+5);
				QuickSetText(2+m_pSimDoc->MR_Get_NumBayesPar(), NumLines+5,"Length of Burn in Period");
				QuickSetNumber(5+m_pSimDoc->MR_Get_NumBayesPar(), NumLines+5,m_pSimDoc->GetNumBurnIn());
				QuickSetNumber(-1, NumLines+5, 1000); // Indicator of BurnInperiod 
				JoinCells(5+m_pSimDoc->MR_Get_NumBayesPar(),NumLines+5,ncol+1, NumLines+5);
				JoinCells(2+m_pSimDoc->MR_Get_NumBayesPar(),NumLines+6,ncol+1,NumLines+13);
				QuickSetText(2+m_pSimDoc->MR_Get_NumBayesPar(), NumLines+6,"Bayesian Ensemble");
				
				
				for(size_t i=0;i<m_pSimDoc->MR_Get_NumBayesPar();i++) {
					DistStat dist;
					dist=m_pSimDoc->GetPostDist(i);
					if(i==0) {
						for(int ii=0;ii<9;ii++) {
							switch(ii) {
								case 0: str="Mean";	break;
								case 1: str="St.D";	break;
								case 2: str="CV";	break;
								case 3: str="Min";	break;
								case 4: str="Max";	break;
								case 5: str="P 5";	break;
								case 6: str="P 50";	break;
								case 7: str="P 95";	break;
								case 8: str="n";	break;
							}
							QuickSetText(0,NumLines+ii+5, str);
							JoinCells(0,NumLines+ii+5,1,NumLines+ii+5);
						}
					}

							// Display the result of the statistics dist
					QuickSetNumber(i+2, NumLines+0+5,dist.Mean);
					QuickSetNumber(i+2, NumLines+1+5,dist.Std);
					if(dist.Mean!=0) QuickSetNumber(i+2, NumLines+2+5,dist.Std/dist.Mean);
					QuickSetNumber(i+2, NumLines+3+5,dist.Min);
					QuickSetNumber(i+2, NumLines+4+5,dist.Max);
					QuickSetNumber(i+2, NumLines+5+5,dist.P5);
					QuickSetNumber(i+2, NumLines+6+5,dist.P50);
					QuickSetNumber(i+2, NumLines+7+5,dist.P95);
					QuickSetNumber(i+2, NumLines+8+5,dist.NumData);
				}

			}
	
}

void MakeSimSheets::Multi_Post_CoVarList() {
	CUGCell cell;
	CString str;
		int ncount=m_pSimDoc->MR_Get_NumTotalPar();
		SetSheetNumber(10); 
		SetSH_NumberCols(2);
		SetSH_Width(350);
		SetTH_NumberRows(2);
		SetTH_Height(60);
		SetSH_ColWidth(-2,200);
		SetSH_ColWidth(-1,150);
		SetNumberCols(ncount); 
		SetNumberRows(ncount+3); 
		//SetColWidth(-2,200);
		//SetColWidth(-1,150);
		QuickSetText(0,-1,"Module");
		QuickSetText(1,-1,"Parameter");
		for(int i=0;i<ncount;i++)
			QuickSetText(i,-1,"");
		
		int icount=0;
		int icount_bayes=0;
		bool ParameterCalib;
		for(int i=0;i<m_pSimDoc->m_MultiRun_Array.size();i++){
		   	for(int ii=0; ii<m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i+1);ii++){

				if(m_pSimDoc->MR_GetMethod(i+1,ii+1)==9||m_pSimDoc->MR_GetMethod(i+1,ii+1)==11||m_pSimDoc->MR_GetMethod(i+1,ii+1)<=5){
					icount_bayes++;
					ParameterCalib=true;
				}
				else
					ParameterCalib=false;



				SetColWidth(icount_bayes-1, 100);
				QuickSetText(icount_bayes-1,-1,"");
				QuickSetText(-1,icount_bayes+1,"");
				icount++;
			}	
		}

				//Check if mean value is calculated
		if(!m_pSimDoc->m_MStorage.m_Lock&&ncount>0) {
			if(!m_pSimDoc->m_MStorage.IsCoCorValidSet()) 
				m_pSimDoc->UpdateCoVar();
		}
		else if (m_pSimDoc->m_MStorage.m_Lock&&ncount > 0 && !m_pSimDoc->m_MStorage.IsCoCorValidSet()) {
			if (MFC_Util::MessageBoxReturn("Multi Run Document is Lock and no Statistic is Available \nDo you want to Unlock document and calculated statistic (Y/N) :", "Multi Run Statistics", MB_YESNO)) {
				m_pSimDoc->UnLockMultiStorageFile();
				m_pSimDoc->UpdateCoVar();
			}
			QuickSetText(0, 0, "No Information is ready");
			return;

		}
	
		for(int i=0;i<ncount;i++) {
			QuickSetText(-1,i,m_pSimDoc->MR_Get_CalibParName(i));
			QuickSetText(i,-1,m_pSimDoc->MR_Get_CalibParName(i));
			QuickSetText(-2,i,m_pSimDoc->MR_Get_BayesGroup(i));
			QuickSetText(i,-2,m_pSimDoc->MR_Get_BayesGroup(i));
				for(int ii=0;ii<ncount;ii++) {
					GetCell(i,ii,&cell);
					cell.SetBackColor(m_NoColor);
					if(i>ii) {
						cell.SetCellType(UGCT_NORMAL);
						cell.SetBool(FALSE);
						float v=m_pSimDoc->MR_Get_BayesCoCor(ii,i);
						string txt=FUtil::STD_FtoAscii(v);
						cell.SetText(txt.c_str());
						//QuickSetNumber(i,ii,v);
						if(v<-0.8) cell.SetBackColor(RGB(100,100,240));
						if(v<-0.4&&v>=-0.8) cell.SetBackColor(RGB(30,240,240));
						if(v>0.4&&v<=0.8) cell.SetBackColor(RGB(240,240,150));
						if(v>0.8) cell.SetBackColor(RGB(240,100,100));

					}
					else {
						cell.SetCellType(UGCT_NORMAL);
						cell.SetText("");


					}
					SetCell(i,ii,&cell);
				}
			QuickSetText(-1,ncount,"Mean Value");
			QuickSetText(-1,ncount+1,"St.Dev Value");
			QuickSetText(-1,ncount+2,"CV Value");
			QuickSetCellType(i, ncount, UGCT_NORMAL);
			QuickSetCellType(i, ncount+1, UGCT_NORMAL);
			QuickSetCellType(i, ncount+2, UGCT_NORMAL);

			if(m_pSimDoc->MR_Get_BayesMethod(i)==11&&m_pSimDoc->m_MStorage.m_Lock) {
				float fmean=exp(double(m_pSimDoc->MR_Get_BayesMean(i)));

				QuickSetNumber(i, ncount, fmean);
				float fcv=exp(double(sqrt(m_pSimDoc->MR_Get_BayesCoCor(i,i))));
				QuickSetNumber(i, ncount+1, fcv );
				fcv=exp(double(m_pSimDoc->MR_Get_BayesMean(i)-sqrt(m_pSimDoc->MR_Get_BayesCoCor(i,i))));
				fcv=(fmean-fcv)/fmean;


				QuickSetNumber(i, ncount+2, fcv );
			}
			else {



				QuickSetNumber(i, ncount, m_pSimDoc->MR_Get_BayesMean(i));
				QuickSetNumber(i, ncount+1, sqrt(m_pSimDoc->MR_Get_BayesCoCor(i,i)));
				if(m_pSimDoc->MR_Get_BayesMean(i)!=0.) 
					QuickSetNumber(i, ncount+2, sqrt(m_pSimDoc->MR_Get_BayesCoCor(i,i))/m_pSimDoc->MR_Get_BayesMean(i) );
			}
			
		}



}
void MakeSimSheets::Multi_Post_NormalList() {
	CString test;
	CUGCell cell;
	int NumberOfCols = 17;
	// This page corresponds to the post distribution of parameters from MultiRun
	QuickSetBackColor(-1, -1, m_ColorClick);
	QuickSetText(-1, -1, "-");
	int prelines = 0;
	SetSheetNumber(10);
	SetNumberCols(NumberOfCols);
	SetDefFont(&m_font);
	SetColWidth(-1, 20);
	SetColWidth(0, 50);
	SetColWidth(1, 150);
	SetColWidth(2, 150);
	SetColWidth(3, 200);
	for (int in = 4; in < 8; in++)
		SetColWidth(in, 0);
	for (int in = 8; in < NumberOfCols; in++)
		SetColWidth(in, 100);



	for (int i = 0; i < NumberOfCols; i++)
		UnJoinCells(i, 0);

	QuickSetText(0, -1, "Dim");
	QuickSetText(1, -1, "Module");
	QuickSetText(2, -1, "Name");
	QuickSetText(3, -1, "Method");
	QuickSetText(4, -1, "Min");
	QuickSetText(5, -1, "Max");
	QuickSetText(6, -1, "Start");
	QuickSetText(7, -1, "Dep");
	QuickSetText(8, -1, "n Accepted");
	QuickSetText(9, -1, "Prior Mean");
	QuickSetText(10, -1, "Post Mean");
	QuickSetText(11, -1, "Post St.D");
	QuickSetText(12, -1, "Post CV");
	QuickSetText(13, -1, "Range Ratio");
	QuickSetText(14, -1, "5 Percentile");
	QuickSetText(15, -1, "50 Percentile");
	QuickSetText(16, -1, "95 Percentile");
	int icount = 0;


	CFileException e;
	BOOL stor_exist;
	if (m_storage_exist) {
		TRY{
			if (m_pSimDoc->MR_Storage_Open())
				stor_exist = TRUE;
			else
				stor_exist = FALSE;
		}
			CATCH_ALL(e) {
			stor_exist = FALSE;
		}
		END_CATCH_ALL;
		m_storage_exist = stor_exist;
		//Check if mean value is calculated
		if (!m_pSimDoc->m_MStorage.m_Lock) {
			if (!m_pSimDoc->m_MStorage.IsCoCorValidSet())
				m_pSimDoc->UpdateCoVar();
		}



	}

	for (int i = 0; i < m_pSimDoc->m_MultiRun_Array.size(); i++) {
		for (int ii = 0; ii < m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i + 1); ii++) {
			QuickSetNumber(0, icount, i + 1);
			icount++;
		}
	}
	int nAddVar;
	if (m_pSimDoc->m_DocFile.m_FileVersionNumber > 40)
		nAddVar = 10;
	else
		nAddVar = 0;
	SetNumberRows(icount + nAddVar);
	icount = -1;
	int icount_bayes = -1;
	bool ParameterCalib;
	DistStat dist;
	for (int i = 0; i < m_pSimDoc->m_MultiRun_Array.size(); i++) {
		for (int ii = 0; ii < m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i + 1); ii++) {
			COLORREF scale_color;

			icount++;
			if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) != 4) {
				icount_bayes++;
				ParameterCalib = true;
				dist = m_pSimDoc->GetPostDist(icount_bayes);
			}
			else
				ParameterCalib = false;

			QuickSetNumber(0, icount, i + 1);
			QuickSetNumber(-1, icount, ii + 1);
			if (m_pSimDoc->IsFinishedSimulation())
				QuickSetBackColor(-1, icount, m_ColorCompleted);
			else
				QuickSetBackColor(-1, icount, m_ColorClick);

			for (int ifn = 1; ifn < NumberOfCols; ifn++) {
				scale_color = m_NoColor;
				// SubItem 1
				QuickSetCellType(ifn, icount, UGCT_NORMAL);
				if (ifn == 1) {
					test = m_pSimDoc->MR_GetParGroup(i + 1, ii + 1).c_str();
					QuickSetBackColor(1, icount, MFC_Util::GetGroupColor(p_ModelInfo->GetGroupCategory(CT2A(test).m_psz)));

				}
				else if (ifn == 2) {
					test = m_pSimDoc->MR_GetParName(i + 1, ii + 1).c_str();
					if (m_pSimDoc->MR_GetTabIndex(i + 1, ii + 1) != string::npos) {
						test += _T("(") + MFC_Util::ItoAscii(m_pSimDoc->MR_GetTabIndex(i + 1, ii + 1) + 1) + _T(")");
					}
				}
				else if (ifn == 3) {
					GetCell(ifn, icount, &cell);
					cell.SetText(m_pSimDoc->MR_GetMethodName(i + 1, ii + 1).c_str());
					cell.SetCellType(UGCT_DROPLIST);
					CString optionlist;
					optionlist = "Linear\nLogarithmic\nStochastic Linear\nStochastic Log\n";
					optionlist += "Table\nDatabase Record\nSame As\nRelative Change As\nRelative Opposite Change As\n";
					if (m_pSimDoc->MR_UseBayesianCoVar)
						optionlist += "Apply Bayesian results\n";
					else
						optionlist += "Bayesian Calibration\n";
					optionlist += "Connected\n";
					if (!m_pSimDoc->MR_UseBayesianCoVar)
						optionlist += "Bayesian Calibration using log distribution\n";
					cell.SetLabelText(optionlist);
					if (m_pSimDoc->IsFinishedSimulation())
						cell.SetReadOnly(TRUE);
					else
						cell.SetReadOnly(FALSE);
					SetCell(ifn, icount, &cell);
				}
				else if (ifn == 4) {
					if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 4) {
						SimB *pPar;
						int index;
						float fmin, fmin1, fact;
						index = m_pSimDoc->MR_GetTabIndex(i + 1, ii + 1);
						pPar = m_pSimDoc->MR_Get_pPar(i + 1, ii + 1);
						fmin = pPar->GetMinValue();
						fmin1 = m_pSimDoc->MR_GetMin(i + 1, ii + 1);
						if (fmin <= fmin1) {
							fmin = 1.E38f;
							for (int j = 0; j < m_pSimDoc->MR_Get_TotalNumberofRuns(); j++) {
								auto irun = m_pSimDoc->m_MStorage.GetMultiRunNo(j);
								fact = m_pSimDoc->m_MStorage.GetMultiP(j, ii);
								if (fact < fmin) fmin = fact;
							}
							if (index < 0)
								((Ps*)pPar)->MR_Set_Min(fmin);
							else
								((P*)pPar)->MR_Set_Min(index, fmin);


						};
						test = MFC_Util::FtoAscii(m_pSimDoc->MR_GetMin(i + 1, ii + 1));

					}
					else if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) < 9 && m_pSimDoc->MR_GetMethod(i + 1, ii + 1) > 3)
						test = "-";
					else
						test = MFC_Util::FtoAscii(m_pSimDoc->MR_GetMin(i + 1, ii + 1));
				}
				else if (ifn == 5) {
					if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 4) {
						test = "-";
						SimB *pPar;
						size_t index;
						float fmax, fmax1, fmin, fact;

						pPar = m_pSimDoc->MR_Get_pPar(i + 1, ii + 1);
						P* pP;
						pP = dynamic_cast<P*>(pPar);
						if (pP != nullptr)
							index = m_pSimDoc->MR_GetTabIndex(i + 1, ii + 1);
						fmax1 = m_pSimDoc->MR_GetMax(i + 1, ii + 1);
						if (fmax1 < -0.9E37) {
							fmax = MISSING;
							fmin = 1.E38f;
							for (int j = 0; j < m_pSimDoc->MR_Get_TotalNumberofRuns(); j++) {
								auto irun = m_pSimDoc->m_MStorage.GetMultiRunNo(j);
								fact = m_pSimDoc->m_MStorage.GetMultiP(j, ii);
								if (fact > fmax) fmax = fact;
								if (fact < fmin) fmin = fact;
							}

							if (pP == nullptr) {
								((Ps*)pPar)->MR_Set_Max(fmax);
								((Ps*)pPar)->MR_Set_Min(fmin);
							}
							else {
								((P*)pPar)->MR_Set_Max(index, fmax);
								((P*)pPar)->MR_Set_Min(index, fmin);
							}


						};
						test = MFC_Util::FtoAscii(m_pSimDoc->MR_GetMax(i + 1, ii + 1));

					}
					else if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) < 9 && m_pSimDoc->MR_GetMethod(i + 1, ii + 1) > 3)
						test = "-";
					else
						test = MFC_Util::FtoAscii(m_pSimDoc->MR_GetMax(i + 1, ii + 1));
				}
				else if (ifn == 6) {
					if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) != 9)
						test = "-";
					else if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 5) {
						test = "-";

					}
					else
						test = MFC_Util::FtoAscii(m_pSimDoc->MR_GetStart(i + 1, ii + 1));
				}
				else if (ifn == 7) {
					if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) >= 6 && m_pSimDoc->MR_GetMethod(i + 1, ii + 1) < 9) {
						SimB *pPar, *pParDep;
						size_t index, indexdep;
						index = m_pSimDoc->MR_GetTabIndex(i + 1, ii + 1);
						pPar = m_pSimDoc->MR_Get_pPar(i + 1, ii + 1);
						if (pPar != nullptr) {
							if (index == string::npos) {
								pParDep = ((Ps*)pPar)->Get_MR_ParDependence();
								indexdep = ((Ps*)pPar)->Get_MR_ParDependenceIndex();
							}
							else {
								pParDep = ((P*)pPar)->Get_MR_ParDependence(index);
								indexdep = ((P*)pPar)->Get_MR_ParDependenceIndex(index);
							}
							if (pParDep != nullptr) {
								test = pParDep->GetName().c_str();
								if (indexdep != string::npos)
									test += _T("(") + MFC_Util::ItoAscii(indexdep + 1) + _T(")");
							}
							else
								test = "-";
						}

					}
					else
						test = "-";

				}
				else if (ifn == 8) {
					// Number of accepted run		
					if (m_storage_exist)
						test = MFC_Util::ItoAscii(m_pSimDoc->m_MStorage.GetNumAccepted());
					else
						test = _T("");
				}
				else if (ifn == 9) {	//Prior Mean	
					if (ParameterCalib && !(m_pSimDoc->MR_GetMethod(i + 1, ii + 1) >= 6 && m_pSimDoc->MR_GetMethod(i + 1, ii + 1) < 9)) {
						float fmean = (m_pSimDoc->MR_GetMin(i + 1, ii + 1) + m_pSimDoc->MR_GetMax(i + 1, ii + 1)) / 2.;
						test = MFC_Util::FtoAscii(fmean);
					}
					else
						test = _T("");

				}
				else if (ifn == 10) {	//Post Mean	
					if (ParameterCalib) {
						test = MFC_Util::FtoAscii(dist.Mean);

						if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 11 && m_pSimDoc->m_MStorage.m_Lock) {
							float fmean = exp(double(m_pSimDoc->MR_Get_BayesMean(icount_bayes)));
							test = MFC_Util::FtoAscii(fmean);
						}
						test = MFC_Util::FtoAscii(dist.Mean);
						float f_norm = FUtil::GetNormalValue(dist.Mean, m_pSimDoc->MR_GetMin(i + 1, ii + 1), m_pSimDoc->MR_GetMax(i + 1, ii + 1));
						if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 11 && m_pSimDoc->m_MStorage.m_Lock) {
							float fmean = exp(double(m_pSimDoc->MR_Get_BayesMean(icount_bayes)));
							float f_norm = FUtil::GetNormalValue(fmean, m_pSimDoc->MR_GetMin(i + 1, ii + 1), m_pSimDoc->MR_GetMax(i + 1, ii + 1));
							test = MFC_Util::FtoAscii(fmean);
						}

						if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 11 || m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 2
							|| m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 9 || m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 3) {
							if (f_norm<0.45 || f_norm>0.55) {
								rgb_color rgb = FUtil::Get_RGB(f_norm);
								scale_color = RGB(rgb.red, rgb.green, rgb.blue);
							}
						}
						else
							scale_color = m_NoColor;

					}
					else
						test = _T("");
				}
				else if (ifn == 11) {	//Post StDev	
					if (ParameterCalib) {

						if (dist.Mean != 0) {
							float fcv = dist.Std;//sqrt(m_pSimDoc->MR_Get_BayesCoCor(icount_bayes,icount_bayes));
							test = MFC_Util::FtoAscii(fcv);
						}
						if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 11 && m_pSimDoc->m_MStorage.m_Lock) {
							float fcv = exp(sqrt((m_pSimDoc->MR_Get_BayesCoCor(icount_bayes, icount_bayes))));
							test = MFC_Util::FtoAscii(fcv);
						}
					}
					else
						test = _T("");
				}
				else if (ifn == 12) {	//Post CV	
					if (ParameterCalib) {

						if (dist.Mean != 0) {
							float fcv = dist.Std / abs(dist.Mean);//sqrt(m_pSimDoc->MR_Get_BayesCoCor(icount_bayes,icount_bayes))/m_pSimDoc->MR_Get_BayesMean(icount_bayes);
							test = MFC_Util::FtoAscii(fcv);
						}
						if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 11 && m_pSimDoc->m_MStorage.m_Lock) {
							float fmean = exp(double(m_pSimDoc->MR_Get_BayesMean(icount_bayes)));
							float fcv = exp(double(m_pSimDoc->MR_Get_BayesCoCor(icount_bayes, icount_bayes)));
							fcv = exp(double(m_pSimDoc->MR_Get_BayesMean(icount_bayes) - m_pSimDoc->MR_Get_BayesCoCor(icount_bayes, icount_bayes)));
							fcv = (fmean - fcv) / fmean;
							test = MFC_Util::FtoAscii(fcv);
						}
					}
					else
						test = _T("");
				}
				else if (ifn == 13) {	//Post distribution
					if (ParameterCalib && (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) <= 3 || m_pSimDoc->MR_GetMethod(i + 1, ii + 1) >= 9)) {
						float v;
						v = (dist.Max - dist.Min) /
							(m_pSimDoc->MR_GetMax(i + 1, ii + 1) - m_pSimDoc->MR_GetMin(i + 1, ii + 1));
						test = MFC_Util::FtoAscii(v);
					}
					else
						test = _T("");
				}
				else if (ifn == 14) {	//Post distribution
					if (ParameterCalib && (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) <= 3 || m_pSimDoc->MR_GetMethod(i + 1, ii + 1) >= 9)) {
						test = MFC_Util::FtoAscii(dist.P5);
					}
					else
						test = _T("");
				}
				else if (ifn == 15) {	//Post distribution
					if (ParameterCalib && (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) <= 3 || m_pSimDoc->MR_GetMethod(i + 1, ii + 1) >= 9)) {
						test = MFC_Util::FtoAscii(dist.P50);
					}
					else
						test = _T("");
				}
				else if (ifn == 16) {	//Post distribution
					if (ParameterCalib && (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) <= 3 || m_pSimDoc->MR_GetMethod(i + 1, ii + 1) >= 9)) {
						test = MFC_Util::FtoAscii(dist.P95);
					}
					else
						test = _T("");
				}
				if (ifn != 3) {
					GetCell(ifn, icount, &cell);

					if (m_pSimDoc->IsFinishedSimulation()) {
						if (ifn == 2) {
							cell.SetCellType(UGCT_CHECKBOX);
							cell.SetLabelText(test);
							cell.SetBool(FALSE);
						}
						else {
							cell.SetReadOnly(TRUE);
							cell.SetText(test);


						}
					}
					else {
						cell.SetText(test);
						if (ifn == 2)
							cell.SetCellType(UGCT_NORMAL);
						cell.SetReadOnly(FALSE);

					}
					cell.SetBackColor(scale_color);

					SetCell(ifn, icount, &cell);

				}
			}
		}

	}
	for (int ii = 0; ii < 10; ii++) {
		icount++;
		QuickSetNumber(0, icount, 1);
		QuickSetNumber(-1, icount, ii + 1);
		if (m_pSimDoc->IsFinishedSimulation())
			QuickSetBackColor(-1, icount, m_ColorCompleted);
		else
			QuickSetBackColor(-1, icount, m_ColorClick);

		QuickSetText(1, icount, "Simulation");
		switch (ii) {
		case 0:
			test = "Accepted";
			break;
		case 1:
			test = "TSV LogLi";
			break;
		case 2:
			test = "Sum LogLi";
			break;
		case 3:
			test = "Tot LogLi";
			break;
		case 4:
			test = "MAP RunNo";
			break;
		case 5:
			test = "Acc Sim Time";
			break;
		case 6:
			test = "n";
			break;
		case 7:
			test = "SSE";
			break;
		case 8:
			test = "AIC";
			break;
		case 9:
			test = "BIC";
			break;
		}
		GetCell(2, icount, &cell);
		cell.SetCellType(UGCT_CHECKBOX);
		cell.SetLabelText(test);
		cell.SetBool(FALSE);
		SetCell(2, icount, &cell);
	}
	RedrawAll();

	MultiVAL mul;
	for (size_t i = 0; i < m_pSimDoc->m_SelectMultiPar.size(); i++) {
		mul = m_pSimDoc->m_SelectMultiPar[i];
		GetCell(mul.col, mul.row, &cell);
		cell.SetBool(true);
		//cell.SetBackColor(m_NoColor);
		SetCell(mul.col, mul.row, &cell);
		RedrawCell(mul.col, mul.row);
	}




}
void MakeSimSheets::Multi_Post_SensitivityTest() {
	CString test;
	CUGCell cell;
	int NumberOfCols=10;
	// This page corresponds to the post distribution of parameters from MultiRun
		QuickSetBackColor(-1,-1,m_ColorClick);
				QuickSetText(-1,-1,"-");
				int prelines=0;
				SetSheetNumber(10); 
				SetNumberCols(NumberOfCols); 
				SetDefFont(&m_font);
				SetColWidth(-1,20);
				SetColWidth(0,50);
				SetColWidth(1,150);
				SetColWidth(2,150);
				SetColWidth(3,200);

				for(int in=4;in<NumberOfCols;in++)
					SetColWidth(in,100);



				for(int i=0;i<NumberOfCols;i++) 
					UnJoinCells(i,0);
	
				QuickSetText(0,-1, "Dim");
				QuickSetText(1,-1, "Module");
				QuickSetText(2,-1, "Name");
				QuickSetText(3,-1, "Method");
				QuickSetText(4,-1, "Org Min");
				QuickSetText(5,-1, "Org Max");
				QuickSetText(6, -1, "Org n");
				QuickSetText(7,-1, "New Min");
				QuickSetText(8,-1, "New Max");
				QuickSetText(9,-1, "New n ");

				int icount=0;

				
				CFileException e;
				BOOL stor_exist;


		for(int i=0;i<m_pSimDoc->m_MultiRun_Array.size();i++){
		   	for(int ii=0; ii<m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i+1);ii++){		
				QuickSetNumber(0,icount,i+1);
				icount++;
			}
		}

		SetNumberRows(icount);	
		icount=-1;
		int icount_bayes=-1;
		bool ParameterCalib;
		DistStat dist;

		size_t orignum = 1;
		for (int i = 0; i < m_pSimDoc->m_MultiRun_Array.size(); i++) {
			orignum *= m_pSimDoc->m_MultiRun_Array[i].NumberOfRepeatitionsWithinDimension;

		}

		for (int i = 0; i < m_pSimDoc->m_MultiRun_Array.size(); i++) {
			for (int ii = 0; ii < m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i + 1); ii++) {
				COLORREF scale_color;

				icount++;
				if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) != 5) {
					icount_bayes++;
					ParameterCalib = true;
					dist = m_pSimDoc->GetPostDist(icount_bayes);
				}
				else
					ParameterCalib = false;

				QuickSetNumber(0, icount, i + 1);
				QuickSetNumber(-1, icount, ii + 1);
				if (m_pSimDoc->IsFinishedSimulation())
					QuickSetBackColor(-1, icount, m_ColorCompleted);
				else
					QuickSetBackColor(-1, icount, m_ColorClick);

				for (int ifn = 1; ifn < NumberOfCols; ifn++) {
					scale_color = m_NoColor;
					// SubItem 1
					QuickSetCellType(ifn, icount, UGCT_NORMAL);
					if (ifn == 1) {
						test = m_pSimDoc->MR_GetParGroup(i + 1, ii + 1).c_str();
						QuickSetBackColor(1, icount, MFC_Util::GetGroupColor(p_ModelInfo->GetGroupCategory(CT2A(test).m_psz)));

					}
					else if (ifn == 2) {
						test = m_pSimDoc->MR_GetParName(i + 1, ii + 1).c_str();
						if (m_pSimDoc->MR_GetTabIndex(i + 1, ii + 1) != string::npos) {
							test += _T("(") + MFC_Util::ItoAscii(m_pSimDoc->MR_GetTabIndex(i + 1, ii + 1) + 1) + _T(")");
						}
					}
					else if (ifn == 3) {
						GetCell(ifn, icount, &cell);
						cell.SetText(m_pSimDoc->MR_GetMethodName(i + 1, ii + 1).c_str());
						cell.SetCellType(UGCT_DROPLIST);
						CString optionlist;
						optionlist = "Linear\nLogarithmic\nStochastic Linear\nStochastic Log\n";
						optionlist += "Table\nDatabase Record\nSame As\nRelative Change As\nRelative Opposite Change As\n";
						if (m_pSimDoc->MR_UseBayesianCoVar)
							optionlist += "Apply Bayesian results\n";
						else
							optionlist += "Bayesian Calibration\n";
						optionlist += "Connected\n";
						if (!m_pSimDoc->MR_UseBayesianCoVar)
							optionlist += "Bayesian Calibration using log distribution\n";
						cell.SetLabelText(optionlist);
						if (m_pSimDoc->IsFinishedSimulation())
							cell.SetReadOnly(TRUE);
						else
							cell.SetReadOnly(FALSE);
						SetCell(ifn, icount, &cell);
					}
					else if (ifn == 4) {
						if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 4) {
							SimB *pPar;
							int index;
							float fmin, fmin1, fact;
							index = m_pSimDoc->MR_GetTabIndex(i + 1, ii + 1);
							pPar = m_pSimDoc->MR_Get_pPar(i + 1, ii + 1);
							fmin = pPar->GetMinValue();
							fmin1 = m_pSimDoc->MR_GetMin(i + 1, ii + 1);
							if (fmin <= fmin1) {
								fmin = 1.E38f;
								for (int j = 0; j < m_pSimDoc->MR_Get_TotalNumberofRuns(); j++) {
									auto irun = m_pSimDoc->m_MStorage.GetMultiRunNo(j);
									fact = m_pSimDoc->m_MStorage.GetMultiP(j, ii);
									if (fact < fmin) fmin = fact;
								}
								if (index < 0)
									((Ps*)pPar)->MR_Set_Min(fmin);
								else
									((P*)pPar)->MR_Set_Min(index, fmin);


							};
							test = MFC_Util::FtoAscii(m_pSimDoc->MR_GetMin(i + 1, ii + 1));

						}
						else if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) < 9 && m_pSimDoc->MR_GetMethod(i + 1, ii + 1) > 3)
							test = "-";
						else
							test = MFC_Util::FtoAscii(m_pSimDoc->MR_GetMin(i + 1, ii + 1));
					}
					else if (ifn == 5) {
						if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 4) {
							test = "-";
							SimB *pPar;
							size_t index;
							float fmax, fmax1, fmin, fact;

							pPar = m_pSimDoc->MR_Get_pPar(i + 1, ii + 1);
							P* pP;
							pP = dynamic_cast<P*>(pPar);
							if (pP != nullptr)
								index = m_pSimDoc->MR_GetTabIndex(i + 1, ii + 1);
							fmax1 = m_pSimDoc->MR_GetMax(i + 1, ii + 1);
							if (fmax1 < -0.9E37) {
								fmax = MISSING;
								fmin = 1.E38f;
								for (int j = 0; j < m_pSimDoc->MR_Get_TotalNumberofRuns(); j++) {
									auto irun = m_pSimDoc->m_MStorage.GetMultiRunNo(j);
									fact = m_pSimDoc->m_MStorage.GetMultiP(j, ii);
									if (fact > fmax) fmax = fact;
									if (fact < fmin) fmin = fact;
								}

								if (pP == nullptr) {
									((Ps*)pPar)->MR_Set_Max(fmax);
									((Ps*)pPar)->MR_Set_Min(fmin);
								}
								else {
									((P*)pPar)->MR_Set_Max(index, fmax);
									((P*)pPar)->MR_Set_Min(index, fmin);
								}


							};
							test = MFC_Util::FtoAscii(m_pSimDoc->MR_GetMax(i + 1, ii + 1));

						}
						else if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) < 9 && m_pSimDoc->MR_GetMethod(i + 1, ii + 1) > 3)
							test = "-";
						else
							test = MFC_Util::FtoAscii(m_pSimDoc->MR_GetMax(i + 1, ii + 1));
					}
					else if (ifn == 6) {
					test = MFC_Util::ItoAscii(m_pSimDoc->m_MultiRun_Array[i].NumberOfRepeatitionsWithinDimension);
					}
					else if (ifn == 7) {
						test = "-";
					}
					else if (ifn == 8) {
					}
					else if (ifn == 9) {
						test = MFC_Util::ItoAscii(orignum);
					}
					if (ifn != 3) {
						GetCell(ifn, icount, &cell);

						if (m_pSimDoc->IsFinishedSimulation()) {
							if (ifn == 2) {
								cell.SetCellType(UGCT_CHECKBOX);
								cell.SetLabelText(test);
								cell.SetBool(FALSE);
							}
							else {
								cell.SetReadOnly(TRUE);
								cell.SetText(test);
							}
						}
						else {
							cell.SetText(test);
							if (ifn == 2)
								cell.SetCellType(UGCT_NORMAL);
							cell.SetReadOnly(FALSE);

						}
						cell.SetBackColor(scale_color);

						SetCell(ifn, icount, &cell);

					}
				}
			}
		}
		RedrawAll();

		MultiVAL mul;
		for(size_t i=0;i<m_pSimDoc->m_SelectMultiPar.size();i++) {
						mul=m_pSimDoc->m_SelectMultiPar[i];
						GetCell(mul.col,mul.row,&cell);
						cell.SetBool(true);
						//cell.SetBackColor(m_NoColor);
						SetCell(mul.col,mul.row,&cell);
						RedrawCell(mul.col,mul.row);
		}
		if (m_pProp->m_ConstrainProp.size() > 0) {




			auto reject = [&](size_t iv) {
				size_t rejected = 0;
				bool done = false;
					for (size_t k = 0; k < orignum; k++) {
						done = false;
						for (size_t i = 0; i < iv+1; i++) {
							if (done) continue;
							auto index = m_pProp->m_ConstrainProp[i].IndepMultiPar;
							auto value = m_pSimDoc->m_MStorage.GetMultiP(k, index);
							if (value< m_pProp->m_ConstrainProp[i].MinRangeValue || value > m_pProp->m_ConstrainProp[i].MaxRangeValue) {
								done = true;
							}
						}
						if (done) rejected++;
					}
					return rejected;
			};

			for (size_t i = 0; i < m_pProp->m_ConstrainProp.size(); i++) {
				auto row=m_pProp->m_ConstrainProp[i].IndepMultiPar;

				GetCell(7, row, &cell);
				cell.SetText(FUtil::STD_FtoAscii(m_pProp->m_ConstrainProp[i].MinRangeValue));
				SetCell(7, row, &cell);
				RedrawCell(7,row);
				GetCell(8, row, &cell);
				cell.SetText(FUtil::STD_FtoAscii(m_pProp->m_ConstrainProp[i].MaxRangeValue));
				SetCell(8, row, &cell);
				RedrawCell(8, row);

				auto nrejected = reject(i);
				GetCell(9, row, &cell);
				cell.SetText(FUtil::STD_ItoAscii(orignum-nrejected));
				SetCell(9, row, &cell);
				RedrawCell(9, row);
				

			}

		}
}
void MakeSimSheets::Multi_Post_AllEnsembleList() {
	CString test;
	CUGCell cell;
	int NumberOfCols = 18;
	// This page corresponds to the post distribution of parameters from MultiRun
	QuickSetBackColor(-1, -1, m_ColorClick);
	QuickSetText(-1, -1, "-");
	int prelines = 0;
	SetSheetNumber(10);
	SetNumberCols(NumberOfCols);
	SetDefFont(&m_font);
	SetColWidth(-1, 20);
	SetColWidth(0, 50);
	SetColWidth(1, 150);
	SetColWidth(2, 150);
	SetColWidth(3, 200);
	for (int in = 4; in<8; in++)
		SetColWidth(in, 0);
	for (int in = 8; in<NumberOfCols; in++)
		SetColWidth(in, 100);



	for (int i = 0; i<NumberOfCols; i++)
		UnJoinCells(i, 0);

	QuickSetText(0, -1, "Dim");
	QuickSetText(1, -1, "Module");
	QuickSetText(2, -1, "Name");
	QuickSetText(3, -1, "Method");
	QuickSetText(4, -1, "Min");
	QuickSetText(5, -1, "Max");
	QuickSetText(6, -1, "Start");
	QuickSetText(7, -1, "Dep");
	QuickSetText(8, -1, "n Accepted");
	QuickSetText(9, -1, "Prior Mean");
	QuickSetText(10, -1, "Post Mean");
	QuickSetText(11, -1, "Post St.D");
	QuickSetText(12, -1, "Post CV");
	QuickSetText(13, -1, "Range Ratio");
	QuickSetText(14, -1, "5 Percentile");
	QuickSetText(15, -1, "50 Percentile");
	QuickSetText(16, -1, "95 Percentile");
	QuickSetText(17, -1, "Ensemble");
	int icount = 0;


	CFileException e;
	BOOL stor_exist;
	if (m_storage_exist) {
		TRY{
			if (m_pSimDoc->MR_Storage_Open())
			stor_exist = TRUE;
			else
				stor_exist = FALSE;
		}
			CATCH_ALL(e) {
			stor_exist = FALSE;
		}
		END_CATCH_ALL;
		m_storage_exist = stor_exist;
		//Check if mean value is calculated
		if (!m_pSimDoc->m_MStorage.m_Lock) {
			if (!m_pSimDoc->m_MStorage.IsCoCorValidSet())
				m_pSimDoc->UpdateCoVar();
		}



	}

	for (int i = 0; i<m_pSimDoc->m_MultiRun_Array.size(); i++) {
		for (int ii = 0; ii<m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i + 1); ii++) {
			QuickSetNumber(0, icount, i + 1);
			icount++;
		}
	}
	int nAddVar;
	if (m_pSimDoc->m_DocFile.m_FileVersionNumber>40)
		nAddVar = 10;
	else
		nAddVar = 0;

	size_t num_ensembles = m_pSimDoc->m_MStorage.m_AcceptedRuns.size();
	SetNumberRows(icount*num_ensembles + nAddVar);
	icount = -1;
	int icount_bayes = -1;
	bool ParameterCalib;
	DistStat dist;
	for (int i = 0; i<m_pSimDoc->m_MultiRun_Array.size(); i++) {
		for (int ii = 0; ii<m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i + 1); ii++) {
			for (size_t ensemble_no = 0; ensemble_no < num_ensembles; ensemble_no++) {
				icount++;
				if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) != 5) {
					if(ensemble_no==0) icount_bayes++;
					ParameterCalib = true;
					dist = m_pSimDoc->GetPostDist(icount_bayes);
				}
				else
					ParameterCalib = false;

				QuickSetNumber(0, icount, i + 1);
				QuickSetNumber(-1, icount, ii + 1);
				if (m_pSimDoc->IsFinishedSimulation())
					QuickSetBackColor(-1, icount, m_ColorCompleted);
				else
					QuickSetBackColor(-1, icount, m_ColorClick);

				dist = m_pSimDoc->GetPostDist(icount_bayes, ensemble_no);
				COLORREF scale_color;
				for (int ifn = 1; ifn < NumberOfCols; ifn++) {
						// SubItem 1
						scale_color = m_NoColor;
						QuickSetCellType(ifn, icount, UGCT_NORMAL);
						if (ifn == 1) {
							test = m_pSimDoc->MR_GetParGroup(i + 1, ii + 1).c_str();
							QuickSetBackColor(1, icount, MFC_Util::GetGroupColor(p_ModelInfo->GetGroupCategory(CT2A(test).m_psz)));

						}
						else if (ifn == 2) {
							test = m_pSimDoc->MR_GetParName(i + 1, ii + 1).c_str();
							if (m_pSimDoc->MR_GetTabIndex(i + 1, ii + 1) != string::npos) {
								test += _T("(") + MFC_Util::ItoAscii(m_pSimDoc->MR_GetTabIndex(i + 1, ii + 1) + 1) + _T(")");
							}
						}
						else if (ifn == 3) {
							GetCell(ifn, icount, &cell);
							cell.SetText(m_pSimDoc->MR_GetMethodName(i + 1, ii + 1).c_str());
							cell.SetCellType(UGCT_DROPLIST);
							CString optionlist;
							optionlist = "Linear\nLogarithmic\nStochastic Linear\nStochastic Log\n";
							optionlist += "Table\nDatabase Record\nSame As\nRelative Change As\nRelative Opposite Change As\n";
							if (m_pSimDoc->MR_UseBayesianCoVar)
								optionlist += "Apply Bayesian results\n";
							else
								optionlist += "Bayesian Calibration\n";
							optionlist += "Connected\n";
							if (!m_pSimDoc->MR_UseBayesianCoVar)
								optionlist += "Bayesian Calibration using log distribution\n";
							cell.SetLabelText(optionlist);
							if (m_pSimDoc->IsFinishedSimulation())
								cell.SetReadOnly(TRUE);
							else
								cell.SetReadOnly(FALSE);
							SetCell(ifn, icount, &cell);
						}
						else if (ifn == 4) {
							if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 4) {
								SimB *pPar;
								int index;
								float fmin, fmin1, fact;
								index = m_pSimDoc->MR_GetTabIndex(i + 1, ii + 1);
								pPar = m_pSimDoc->MR_Get_pPar(i + 1, ii + 1);
								fmin = pPar->GetMinValue();
								fmin1 = m_pSimDoc->MR_GetMin(i + 1, ii + 1);
								if (fmin <= fmin1) {
									fmin = 1.E38f;
									for (int j = 0; j < m_pSimDoc->MR_Get_TotalNumberofRuns(); j++) {
										auto irun = m_pSimDoc->m_MStorage.GetMultiRunNo(j);
										fact = m_pSimDoc->m_MStorage.GetMultiP(j, ii);
										if (fact < fmin) fmin = fact;
									}
									if (index < 0)
										((Ps*)pPar)->MR_Set_Min(fmin);
									else
										((P*)pPar)->MR_Set_Min(index, fmin);


								};
								test = MFC_Util::FtoAscii(m_pSimDoc->MR_GetMin(i + 1, ii + 1));

							}
							else if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) < 9 && m_pSimDoc->MR_GetMethod(i + 1, ii + 1) > 3)
								test = "-";
							else
								test = MFC_Util::FtoAscii(m_pSimDoc->MR_GetMin(i + 1, ii + 1));
						}
						else if (ifn == 5) {
							if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 4) {
								test = "-";
								SimB *pPar;
								size_t index;
								float fmax, fmax1, fmin, fact;

								pPar = m_pSimDoc->MR_Get_pPar(i + 1, ii + 1);
								P* pP;
								pP = dynamic_cast<P*>(pPar);
								if (pP != nullptr)
									index = m_pSimDoc->MR_GetTabIndex(i + 1, ii + 1);
								fmax1 = m_pSimDoc->MR_GetMax(i + 1, ii + 1);
								if (fmax1 < -0.9E37) {
									fmax = MISSING;
									fmin = 1.E38f;
									for (int j = 0; j < m_pSimDoc->MR_Get_TotalNumberofRuns(); j++) {
										auto irun = m_pSimDoc->m_MStorage.GetMultiRunNo(j);
										fact = m_pSimDoc->m_MStorage.GetMultiP(j, ii);
										if (fact > fmax) fmax = fact;
										if (fact < fmin) fmin = fact;
									}

									if (pP == nullptr) {
										((Ps*)pPar)->MR_Set_Max(fmax);
										((Ps*)pPar)->MR_Set_Min(fmin);
									}
									else {
										((P*)pPar)->MR_Set_Max(index, fmax);
										((P*)pPar)->MR_Set_Min(index, fmin);
									}


								};
								test = MFC_Util::FtoAscii(m_pSimDoc->MR_GetMax(i + 1, ii + 1));

							}
							else if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) < 9 && m_pSimDoc->MR_GetMethod(i + 1, ii + 1) > 3)
								test = "-";
							else
								test = MFC_Util::FtoAscii(m_pSimDoc->MR_GetMax(i + 1, ii + 1));
						}
						else if (ifn == 6) {
							if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) != 9)
								test = "-";
							else if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 5) {
								test = "-";

							}
							else
								test = MFC_Util::FtoAscii(m_pSimDoc->MR_GetStart(i + 1, ii + 1));
						}
						else if (ifn == 7) {
							if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) >= 6 && m_pSimDoc->MR_GetMethod(i + 1, ii + 1) < 9) {
								SimB *pPar, *pParDep;
								size_t index, indexdep;
								index = m_pSimDoc->MR_GetTabIndex(i + 1, ii + 1);
								pPar = m_pSimDoc->MR_Get_pPar(i + 1, ii + 1);
								if (pPar != nullptr) {
									if (index == string::npos) {
										pParDep = ((Ps*)pPar)->Get_MR_ParDependence();
										indexdep = ((Ps*)pPar)->Get_MR_ParDependenceIndex();
									}
									else {
										pParDep = ((P*)pPar)->Get_MR_ParDependence(index);
										indexdep = ((P*)pPar)->Get_MR_ParDependenceIndex(index);
									}
									if (pParDep != nullptr) {
										test = pParDep->GetName().c_str();
										if (indexdep != string::npos)
											test += _T("(") + MFC_Util::ItoAscii(indexdep + 1) + _T(")");
									}
									else
										test = "-";
								}

							}
							else
								test = "-";

						}
						else if (ifn == 8) {
							// Number of accepted run		
							if (m_storage_exist)
								test = MFC_Util::ItoAscii(m_pSimDoc->m_MStorage.GetNumAccepted());
							else
								test = _T("");
						}
						else if (ifn == 9) {	//Prior Mean	
							if (ParameterCalib&&!(m_pSimDoc->MR_GetMethod(i + 1, ii + 1) >= 6 && m_pSimDoc->MR_GetMethod(i + 1, ii + 1) < 9)) {
								float fmean = (m_pSimDoc->MR_GetMin(i + 1, ii + 1) + m_pSimDoc->MR_GetMax(i + 1, ii + 1)) / 2.;
								test = MFC_Util::FtoAscii(fmean);
							}
							else
								test = _T("");

						}
						else if (ifn == 10) {	//Post Mean	
							if (ParameterCalib) {
								test = MFC_Util::FtoAscii(dist.Mean);
								float f_norm = FUtil::GetNormalValue(dist.Mean, m_pSimDoc->MR_GetMin(i + 1, ii + 1), m_pSimDoc->MR_GetMax(i + 1, ii + 1));
								if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 11 && m_pSimDoc->m_MStorage.m_Lock) {
									float fmean = exp(double(m_pSimDoc->MR_Get_BayesMean(icount_bayes)));
									float f_norm = FUtil::GetNormalValue(fmean, m_pSimDoc->MR_GetMin(i + 1, ii + 1), m_pSimDoc->MR_GetMax(i + 1, ii + 1));							
									test = MFC_Util::FtoAscii(fmean);
								}
								if (f_norm<0.45 || f_norm>0.55) {
									rgb_color rgb = FUtil::Get_RGB(f_norm);
									scale_color = RGB(rgb.red, rgb.green, rgb.blue);
								}
								
							}
							else
								test = _T("");
						}
						else if (ifn == 11) {	//Post StDev	
							if (ParameterCalib) {

								if (dist.Mean != 0) {
									float fcv = dist.Std;//sqrt(m_pSimDoc->MR_Get_BayesCoCor(icount_bayes,icount_bayes));
									test = MFC_Util::FtoAscii(fcv);
								}
								if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 11 && m_pSimDoc->m_MStorage.m_Lock) {
									float fcv = exp(sqrt((m_pSimDoc->MR_Get_BayesCoCor(icount_bayes, icount_bayes))));
									test = MFC_Util::FtoAscii(fcv);
								}
							}
							else
								test = _T("");
						}
						else if (ifn == 12) {	//Post CV	
							if (ParameterCalib) {

								if (dist.Mean != 0) {
									float fcv = dist.Std / abs(dist.Mean);//sqrt(m_pSimDoc->MR_Get_BayesCoCor(icount_bayes,icount_bayes))/m_pSimDoc->MR_Get_BayesMean(icount_bayes);
									test = MFC_Util::FtoAscii(fcv);
								}
								if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 11 && m_pSimDoc->m_MStorage.m_Lock) {
									float fmean = exp(double(m_pSimDoc->MR_Get_BayesMean(icount_bayes)));
									float fcv = exp(double(m_pSimDoc->MR_Get_BayesCoCor(icount_bayes, icount_bayes)));
									fcv = exp(double(m_pSimDoc->MR_Get_BayesMean(icount_bayes) - m_pSimDoc->MR_Get_BayesCoCor(icount_bayes, icount_bayes)));
									fcv = (fmean - fcv) / fmean;
									test = MFC_Util::FtoAscii(fcv);
								}
							}
							else
								test = _T("");
						}
						else if (ifn == 13) {	//Post distribution
							if (ParameterCalib && (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) <= 3 || m_pSimDoc->MR_GetMethod(i + 1, ii + 1) >= 9)) {
								float v;
								v = (dist.Max - dist.Min) /
									(m_pSimDoc->MR_GetMax(i + 1, ii + 1) - m_pSimDoc->MR_GetMin(i + 1, ii + 1));
								test = MFC_Util::FtoAscii(v);
							}
							else
								test = _T("");
						}
						else if (ifn == 14) {	//Post distribution
							if (ParameterCalib && (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) <= 3 || m_pSimDoc->MR_GetMethod(i + 1, ii + 1) >= 9)) {
								test = MFC_Util::FtoAscii(dist.P5);
							}
							else
								test = _T("");
						}
						else if (ifn == 15) {	//Post distribution
							if (ParameterCalib && (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) <= 3 || m_pSimDoc->MR_GetMethod(i + 1, ii + 1) >= 9)) {
								test = MFC_Util::FtoAscii(dist.P50);
							}
							else
								test = _T("");
						}
						else if (ifn == 16) {	//Post distribution
							if (ParameterCalib && (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) <= 3 || m_pSimDoc->MR_GetMethod(i + 1, ii + 1) >= 9)) {
								test = MFC_Util::FtoAscii(dist.P95);
							}
							else
								test = _T("");
						}
						if (ifn != 3) {
							GetCell(ifn, icount, &cell);

							if (m_pSimDoc->IsFinishedSimulation()) {
								if (ifn == 2) {
									cell.SetCellType(UGCT_CHECKBOX);
									cell.SetLabelText(test);
									cell.SetBool(FALSE);
								}
								else {
									cell.SetReadOnly(TRUE);
									cell.SetText(test);

								}
							}
							else {
								cell.SetText(test);
								if (ifn == 2)
									cell.SetCellType(UGCT_NORMAL);
								cell.SetReadOnly(FALSE);

							}
							cell.SetBackColor(scale_color);

							SetCell(ifn, icount, &cell);

						}
					}
				QuickSetNumber(17, icount, ensemble_no + 1);
			}
		}
		for (int ii = 0; ii<10; ii++) {
			icount++;
			QuickSetNumber(0, icount, i + 1);
			QuickSetNumber(-1, icount, ii + 1);
			if (m_pSimDoc->IsFinishedSimulation())
				QuickSetBackColor(-1, icount, m_ColorCompleted);
			else
				QuickSetBackColor(-1, icount, m_ColorClick);

			QuickSetText(1, icount, "Simulation");
			switch (ii) {
			case 0:
				test = "Accepted";
				break;
			case 1:
				test = "TSV LogLi";
				break;
			case 2:
				test = "Sum LogLi";
				break;
			case 3:
				test = "Tot LogLi";
				break;
			case 4:
				test = "MAP RunNo";
				break;
			case 5:
				test = "Acc Sim Time";
				break;
			case 6:
				test = "n";
				break;
			case 7:
				test = "SSE";
				break;
			case 8:
				test = "AIC";
				break;
			case 9:
				test = "BIC";
				break;
			}
			GetCell(2, icount, &cell);
			cell.SetCellType(UGCT_CHECKBOX);
			cell.SetLabelText(test);
			cell.SetBool(FALSE);
			SetCell(2, icount, &cell);
		}
		RedrawAll();
	}


	MultiVAL mul;
	for (size_t i = 0; i<m_pSimDoc->m_SelectMultiPar.size(); i++) {
		mul = m_pSimDoc->m_SelectMultiPar[i];
		GetCell(mul.col, mul.row, &cell);
		cell.SetBool(true);
		//cell.SetBackColor(m_NoColor);
		SetCell(mul.col, mul.row, &cell);
		RedrawCell(mul.col, mul.row);
	}




}
void MakeSimSheets::Multi_Prior_List() {
CUGCell cell;
CString test;
		// Prior Simulation to be done
		QuickSetBackColor(-1,-1,m_ColorClick);
		QuickSetText(-1,-1,"");

		// The page below corresponds to prior simulation
				int prelines=0;
				SetSheetNumber(10); 
				SetNumberCols(10); 
				SetDefFont(&m_font);
				SetColWidth(-1,20);
				

				SetColWidth(0,50);
				SetColWidth(1,150);
				SetColWidth(2,150);
				SetColWidth(3,150);
			    SetColWidth(4,80);
				SetColWidth(5,80);
			    SetColWidth(6,80);
				SetColWidth(7,150);
				SetColWidth(8,50);
				SetColWidth(9,50);
			
				for(int i=0;i<9;i++) 
					UnJoinCells(i,0);
	
				QuickSetText(0,-1, "Dim");
				QuickSetText(1,-1, "Module");
				QuickSetText(2,-1, "Name");
				QuickSetText(3,-1, "Method");
				QuickSetText(4,-1, "Min");
				QuickSetText(5,-1, "Max");
				QuickSetText(6,-1, "Start");
				QuickSetText(7,-1, "Dependence");
				QuickSetText(8,-1, "n");
				QuickSetText(9,-1, "Step");
		int icount=0;
		for(int i=0;i<m_pSimDoc->m_MultiRun_Array.size();i++){
		   	for(int ii=0; ii<m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i+1);ii++){		
			//	QuickSetNumber(0,icount,i+1);
				icount++;
			}
		}

		if(m_pSimDoc->IsBayesianCalibration())
			SetNumberRows(icount+2);
		else
			SetNumberRows(icount);		
		icount=-1;
		CString dimoptionlist;
		for(int i=0;i<m_pSimDoc->m_MultiRun_Array.size();i++){
			dimoptionlist+=MFC_Util::ItoAscii(i+1)+_T("\n");
		}
		for(int i=0;i<m_pSimDoc->m_MultiRun_Array.size();i++){
		   	for(int ii=0; ii<m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i+1);ii++){
				icount++;
				//QuickSetNumber(0,icount,i+1);
				QuickSetNumber(-1,icount,ii+1);
				if(m_pSimDoc->IsFinishedSimulation())
					QuickSetBackColor(-1,icount,m_ColorCompleted);
				else 
					QuickSetBackColor(-1,icount,m_ColorClick);
				BOOL clickcell=FALSE;
				for(int ifn=0; ifn<10;ifn++) {
		// SubItem 1
					if(ifn==0) {
						GetCell(ifn,icount, &cell); 

						cell.SetText(FUtil::STD_ItoAscii(i+1));
						cell.SetCellType(UGCT_DROPLIST);
						cell.SetLabelText(dimoptionlist);
						if(m_pSimDoc->IsFinishedSimulation())
							cell.SetReadOnly(TRUE);
						else
							cell.SetReadOnly(FALSE);
						SetCell(ifn, icount,&cell);

					}
					else if(ifn==1) {
						auto tst=m_pSimDoc->MR_GetParGroup(i+1,ii+1);
						test = CString(tst.c_str());
						QuickSetBackColor(1,icount,MFC_Util::GetGroupColor(p_ModelInfo->GetGroupCategory(tst)));

					}
					else if(ifn==2) {
						GetCell(ifn,icount,&cell);
						SimB *pPar=m_pSimDoc->MR_Get_pPar(i+1,ii+1);
						if(m_pSimDoc->MR_GetParGroup(i+1,ii+1)=="DataBase"){
							auto tst=((CDB*)pPar)->GetName();
							cell.SetText(tst);
							CString optionlistBase;
							for(int ix=0;ix<10;ix++) 
								optionlistBase+=m_pSimDoc->DB_GetNames(ix).c_str();
								optionlistBase+="\n";
							cell.SetCellType(UGCT_DROPLIST);
							cell.SetLabelText(optionlistBase);
							if(m_pSimDoc->IsFinishedSimulation())
								cell.SetReadOnly(TRUE);
							else
								cell.SetReadOnly(FALSE);
							SetCell(ifn, icount,&cell);
						}
						else {
							auto tst=pPar->GetName();
						//test=m_pSimDoc->MR_GetParName(i+1,ii+1);
							if(m_pSimDoc->MR_GetTabIndex(i+1,ii+1)!=string::npos) 
								tst+="("+FUtil::STD_ItoAscii(m_pSimDoc->MR_GetTabIndex(i+1,ii+1)+1)+")";
							cell.SetText(tst);
							if(m_pSimDoc->IsFinishedSimulation())
								cell.SetReadOnly(TRUE);
							else
								cell.SetReadOnly(FALSE);
							SetCell(ifn,icount,&cell);
						}
					}
					else if(ifn==3) {
						if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 5) {
							SimB *pPar = m_pSimDoc->MR_Get_pPar(i + 1, ii + 1);
							if (pPar->Is_DB()) {
								CString optionlistBase;
								size_t nrep = m_pSimDoc->MR_GetNumberOfRepeationsForThisDimension(i);
								for (size_t ix = 0; ix< nrep; ix++) {
									optionlistBase += ((CDB*)pPar)->MR_GetKeySelection(ix).c_str();
									optionlistBase += "\n";
								}
								auto tst= ((CDB*)pPar)->MR_GetKeySelection(((CDB*)pPar)->GetLastSelectedRepetition());
								cell.SetText(tst);
								cell.SetCellType(UGCT_DROPLIST);
								cell.SetLabelText(optionlistBase);
							}
							else {
								test = "Selected Records";
								clickcell = TRUE;
								JoinCells(3, icount, 7, icount);
								GetCell(ifn, icount, &cell);
								cell.SetText(test);
								if (m_pSimDoc->IsFinishedSimulation())
									cell.SetReadOnly(TRUE);
								else
									cell.SetReadOnly(FALSE);
								if (clickcell) {
									cell.SetBackColor(m_ColorClick);
									clickcell = FALSE;
									cell.SetParam(1000);
								}
							}
							SetCell(ifn,icount,&cell);
						}
						else {
							GetCell(ifn,icount, &cell); 
							cell.SetText(m_pSimDoc->MR_GetMethodName(i+1,ii+1).c_str());
							cell.SetCellType(UGCT_DROPLIST);
							CString optionlist;
							optionlist="Linear\nLogarithmic\nStochastic Linear\nStochastic Log\n";
							optionlist+="Table\nSame As\nRelative Change As\nRelative Opposite Change As\n";
							if(m_pSimDoc->MR_UseBayesianCoVar)
									optionlist+="Apply Bayesian results\n";
								else
									optionlist+="Bayesian Calibration\n";
							optionlist+="Connected\n";
							if (!m_pSimDoc->MR_UseBayesianCoVar) {
								optionlist += "Bayesian Calibration using log distribution\n";
								optionlist += "Nelder Mead Optimization\n";

							}
							cell.SetLabelText(optionlist);
							if(m_pSimDoc->IsFinishedSimulation())
								cell.SetReadOnly(TRUE);
							else
								cell.SetReadOnly(FALSE);
							SetCell(ifn, icount,&cell);
						}
					}
					else if(ifn==4) {
						if(m_pSimDoc->MR_GetMethod(i+1,ii+1)==1||m_pSimDoc->MR_GetMethod(i+1,ii+1)==3||m_pSimDoc->MR_GetMethod(i+1,ii+1)==11 || m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == NELDER_MEAD) {
							float fval=m_pSimDoc->MR_GetMin(i+1,ii+1);
							if(fval<=0&&m_SetUpDone)  {
								MFC_Util::MessageBox("Your seetings with a log distribution can not be combined with a value that is <=0\n A new value is set to be 0.001 - Please modify to a valid new value!","Error in Value for parameter range",MB_OK);
								SimB *pPar=m_pSimDoc->MR_Get_pPar(i+1,ii+1);
								int TabInd=m_pSimDoc->MR_GetTabIndex(i+1,ii+1);
								if(TabInd<0)
									((Ps*)pPar)->MR_Set_Min(0.001);
								else
									((P*)pPar)->MR_Set_Min(TabInd, 0.001);
								fval=0.001f;
							}
							test=MFC_Util::FtoAscii(fval);

						}
						else if(m_pSimDoc->MR_GetMethod(i+1,ii+1)>6&&m_pSimDoc->MR_GetMethod(i+1,ii+1)<9)
							test=MFC_Util::FtoAscii(m_pSimDoc->MR_GetMin(i+1,ii+1));
						else if (m_pSimDoc->MR_GetMethod(i + 1, ii + 1) < 9 && m_pSimDoc->MR_GetMethod(i + 1, ii + 1) > 3) {

							 SimB *pPar = m_pSimDoc->MR_Get_pPar(i + 1, ii + 1);
							 if (pPar->Is_DB()) {
								 test = "Data Base Link";
								 JoinCells(4, icount, 7, icount);

							 }
							 else
								 test = "-";
							 
						}
						else
							test=MFC_Util::FtoAscii(m_pSimDoc->MR_GetMin(i+1,ii+1));

						if(m_pSimDoc->MR_GetMin(i+1,ii+1)>=m_pSimDoc->MR_GetMax(i+1,ii+1)&&m_SetUpDone) {
							string caption;
							SimB *pPar=m_pSimDoc->MR_Get_pPar(i+1,ii+1);
							caption=pPar->GetName();
							if(m_pSimDoc->MR_GetMethod(i+1,ii+1)>6&&m_pSimDoc->MR_GetMethod(i+1,ii+1)<9||(m_pSimDoc->MR_GetMethod(i+1,ii+1)>0&&m_pSimDoc->MR_GetMethod(i+1,ii+1)<=3)) {
								MFC_Util::MessageBox("Your seetings with a min value larger than the max value may cause errors!\n - Please modify to a valid new value!","Error in Value for parameter range for -"+caption,MB_OK);

							}


						}

						if(m_pSimDoc->MR_GetMethod(i+1,ii+1)==4) {

							test="Edit/Copy/Paste Values";
							clickcell=TRUE;
							JoinCells(4,icount,7,icount);

						}
						else if(m_pSimDoc->MR_GetMethod(i+1,ii+1)==10) {
							test="Connected to Database";
							JoinCells(4,icount,7,icount);
						}
						else if(m_pSimDoc->MR_GetMethod(i+1,ii+1)==6) {
							JoinCells(3,icount,6,icount);
						}
						else if(m_pSimDoc->MR_GetMethod(i+1,ii+1)>6&&m_pSimDoc->MR_GetMethod(i+1,ii+1)<9) {
							JoinCells(5,icount,6,icount);
						}
						else if(m_pSimDoc->MR_GetMethod(i+1,ii+1)!=9&&m_pSimDoc->MR_GetMethod(i+1,ii+1)!=11 && m_pSimDoc->MR_GetMethod(i + 1, ii + 1) != 5
							&& m_pSimDoc->MR_GetMethod(i + 1, ii + 1) != 12) {
							JoinCells(5,icount,7,icount);
						}
					}
					else if(ifn==5) {
						if(m_pSimDoc->MR_GetMethod(i+1,ii+1)>6&&m_pSimDoc->MR_GetMethod(i+1,ii+1)<9)
							test=MFC_Util::FtoAscii(m_pSimDoc->MR_GetMax(i+1,ii+1));
						else if(m_pSimDoc->MR_GetMethod(i+1,ii+1)<9&&m_pSimDoc->MR_GetMethod(i+1,ii+1)>3)
							test="-";
						else
							test=MFC_Util::FtoAscii(m_pSimDoc->MR_GetMax(i+1,ii+1));
					}
					else if(ifn==6) {
						if(m_pSimDoc->MR_GetMethod(i+1,ii+1)<9)
							test="-";
						else if(m_pSimDoc->MR_GetMethod(i+1,ii+1)==9 || m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == 11 || m_pSimDoc->MR_GetMethod(i + 1, ii + 1) == NELDER_MEAD)
							test=MFC_Util::FtoAscii(m_pSimDoc->MR_GetStart(i+1,ii+1));
					}
					else if(ifn==7) {
						if(m_pSimDoc->MR_GetMethod(i+1,ii+1)>=6&&m_pSimDoc->MR_GetMethod(i+1,ii+1)<9) {
							SimB *pPar, *pParDep;
							size_t index, indexdep;
							index=m_pSimDoc->MR_GetTabIndex(i+1,ii+1);
							pPar=m_pSimDoc->MR_Get_pPar(i+1,ii+1);
							if(pPar!=nullptr) {
								if(index==string::npos) {
									pParDep=((Ps*)pPar)->Get_MR_ParDependence();
									if(pParDep!=nullptr)
										indexdep=((Ps*)pPar)->Get_MR_ParDependenceIndex();
								}
								else {
									pParDep=((P*)pPar)->Get_MR_ParDependence(index);
									if(pParDep!=nullptr)
										indexdep=((P*)pPar)->Get_MR_ParDependenceIndex(index);
								}
								if(pParDep!=nullptr) {
									test=CString(pParDep->GetName().c_str());
									if(indexdep!=string::npos)
										test+=_T("(")+MFC_Util::ItoAscii(indexdep+1)+_T(")");
								}
								else
									test=_T("-");


								GetCell(ifn,icount, &cell); 
								cell.SetText(test);



								cell.SetCellType(UGCT_DROPLIST);
								CString optionlist;

								int icounto=0;	
								optionlist=_T("");
								for(int ix=0;ix<m_pSimDoc->m_MultiRun_Array.size();ix++){
	   								for(int iix=0; iix<m_pSimDoc->MR_Get_NumberOfParametersWithinDim(ix+1);iix++){
										icounto++;
										CString name;
										name=m_pSimDoc->MR_GetParName(ix+1,iix+1).c_str();
										if(m_pSimDoc->MR_GetTabIndex(ix+1,iix+1)!=string::npos) {
											name+=_T("(")+MFC_Util::ItoAscii(m_pSimDoc->MR_GetTabIndex(ix+1,iix+1)+1)+_T(")");
										}
										/*	if(pkoll==m_pSimDoc->MR_Get_pPar(i+1,ii+1)){
											if(m_TabIndex<0) 
												pindexkoll=	m_pPar->Get_MR_ParDependenceIndex();
											else
												pindexkoll=	m_pPar->Get_MR_ParDependenceIndex(_TabIndex);
											if(pindexkoll==m_pSimDoc->MR_GetTabIndex(i+1,ii+1))
												m_ParDependentIndex=icounto-1;
											
										*/
										optionlist+=name+_T("\n");

										}
										
									}
								

								cell.SetLabelText(optionlist);
								if(m_pSimDoc->IsFinishedSimulation())
									cell.SetReadOnly(TRUE);
								else
									cell.SetReadOnly(FALSE);
								SetCell(ifn, icount,&cell);

							}
	
						}
						else
							test="-";

					}
					else if(ifn==8) {			
						test=MFC_Util::ItoAscii(m_pSimDoc->m_MultiRun_Array[i].NumberOfRepeatitionsWithinDimension);
					}
					else if(ifn==9) {	// Bayesian step size		
						if(m_pSimDoc->MR_GetMethod(i+1,ii+1)==9||m_pSimDoc->MR_GetMethod(i+1,ii+1)==11) 
							test=MFC_Util::FtoAscii(m_pSimDoc->MR_GetBayesianStepSize());
						else
							test=_T("");
					}
					if(ifn!=3&&ifn!=7&&ifn!=0&&ifn!=2) {
						GetCell(ifn,icount,&cell);
						cell.SetText(test);
						if(m_pSimDoc->IsFinishedSimulation())
							cell.SetReadOnly(TRUE);
						else
							cell.SetReadOnly(FALSE);
						if(clickcell) {
							cell.SetBackColor(m_ColorClick);
							clickcell=FALSE;
							cell.SetParam(1000);
						}
						SetCell(ifn,icount,&cell);
						
					}
				}
			}

		}
		if(m_pSimDoc->IsBayesianCalibration()) {
			icount++;
			JoinCells(0,icount,9,icount);
			QuickSetText(0,icount, "MC MC information");
			QuickSetBackColor(0, icount,m_ColorHeading);
			QuickSetText(0,icount+1,"Length of Burn in period");
			JoinCells(0,icount+1,1,icount+1);
			QuickSetNumber(2,icount+1,m_pSimDoc->GetNumBurnIn());
			JoinCells(3,icount+1,9,icount+1);

		}




}
void MakeSimSheets::ShowTimeSerieTableResults() {
	if (m_pSimDoc->m_SelectedDocuments.size() == 0) {
		m_pSimDoc->m_SelectedDocuments.push_back(m_pSimDoc);
	}
	m_NumOpenDoc = max(1, m_pSimDoc->m_SelectedDocuments.size());


	if (m_pSimDoc->m_SelectedDocuments.size()>0 && m_pSimDoc->m_SelectMultiSum.size()>0) {
		m_pSimDoc->m_PrevSheet = 5;
		Doc *pDoc;

		auto NumLines = m_pSimDoc->m_SelectMultiSum.size();
		int nac = m_pSimDoc->m_MStorage.GetNumAccepted();
		int iacceptall = m_pSimDoc->m_MStorage.GetNumberOfRuns();
		auto MaxCol = 11;
		SetTH_NumberRows(2);
		SetTH_Height(36);
		SetNumberCols(MaxCol);
		SetNumberRows(NumLines);
		QuickSetText(2, -2, "Distribution Prior Selection n=" + FUtil::STD_ItoAscii(iacceptall));
		QuickSetText(6, -2, "Distribution Post Selection n=" + FUtil::STD_ItoAscii(nac));
		QuickSetText(0, -2, "Variable");
		QuickSetText(2, -1, "Mean");
		QuickSetText(3, -1, "CV");
		QuickSetText(4, -1, "Min");
		QuickSetText(5, -1, "Max");
		QuickSetText(6, -1, "Mean");
		QuickSetText(7, -1, "CV");
		QuickSetText(8, -1, "Min");
		QuickSetText(9, -1, "Max");
		QuickSetText(10, -1, "Range");

		JoinCells(0, -2, 0, -1);
		JoinCells(1, -2, 1, -1);
		JoinCells(2, -2, 5, -2);
		JoinCells(6, -2, 10, -2);
		SetColWidth(0, 160);
		SetColWidth(1, 100);
		for (int j = 2; j<10; j++) SetColWidth(j, 80);

		for (int j = 0; j<m_NumOpenDoc; j++) {
			if (m_NumOpenDoc <= 1)
				pDoc = m_pSimDoc;
			else
				pDoc = m_pSimDoc->m_SelectedDocuments[j];//(CCoupModelDoc*)m_SelectedDoc.GetAt(j);
			for (int i = 0; i<m_pSimDoc->m_SelectMultiSum.size(); i++) {
				auto datatype = m_pSimDoc->m_SelectMultiSum[i].type;
				//SimB *Ptr=(SimB*)m_pSimDoc->m_SelectMultiSum[i].pBase;
				int Index = m_pSimDoc->m_SelectMultiSum[i].Index;


				//Index=Index*6;
				float	value;
				value = pDoc->m_MStorage.GetSumV(0, Index - 6);
				value = pDoc->m_MStorage.GetSumV(0, Index - 1);
				value = pDoc->m_MStorage.GetSumV(0, Index - 5);

				vector <float> pvalue;
				nac = iacceptall;
				pvalue.resize(nac);
				float min1 = 1.1E38f, max1 = -1.1E38f, std, mean, range, cv;
				for (int ii = 0; ii<nac; ii++) {
					pvalue[ii] = pDoc->m_MStorage.GetSumV(ii, Index - 6);
					if (pvalue[ii]>max1) max1 = pvalue[ii];
					if (pvalue[ii]<min1) min1 = pvalue[ii];
				}
				mean = Stats::Moments::mean(nac, &pvalue[0]);
				std = sqrt(Stats::Moments::variance(nac, &pvalue[0], false));
				QuickSetNumber(2, i, mean);
				if (mean != 0.) {
					cv = std / mean;
					QuickSetNumber(3, i, cv);
				}
				QuickSetNumber(4, i, min1);
				QuickSetNumber(5, i, max1);
				range = max1 - min1;
				int nac = pDoc->m_MStorage.GetNumAccepted();
				pvalue.resize(nac);
				min1 = 1.1E38f; max1 = MISSING;
				for (int ii = 0; ii<nac; ii++) {
					auto irun = m_pSimDoc->m_MStorage.Get_AcceptedRuns(ii);

					pvalue[ii] = pDoc->m_MStorage.GetSumV(irun, Index - 6);
					if (pvalue[ii]>max1) max1 = pvalue[ii];
					if (pvalue[ii]<min1) min1 = pvalue[ii];


				}
				mean = Stats::Moments::mean(nac, &pvalue[0]);
				std = sqrt(Stats::Moments::variance(nac, &pvalue[0], false));
				QuickSetNumber(6, i, mean);
				if (mean != 0.)
					cv = std / mean;
				QuickSetNumber(7, i, cv);
				QuickSetNumber(8, i, min1);
				QuickSetNumber(9, i, max1);
				if (range>0.)
					QuickSetNumber(10, i, (max1 - min1) / range);
				else
					QuickSetText(10, i, "");

				vector<SIMB> vvp;
				int ind = 0;
				int il, lin;
				SimB* pPar;
				bool hit = false;
				string VarName, st;

				for (size_t jk = 0; jk<4; jk++) {
					switch (jk) {
					case 0:
						vvp = m_pSimDoc->m_X_Array;
						break;
					case 1:
						vvp = m_pSimDoc->m_T_Array;
						break;
					case 2:
						vvp = m_pSimDoc->m_G_Array;
						break;
					case 3:
						vvp = m_pSimDoc->m_D_Array;
						break;
					}


					for (size_t i2 = 0; i2<vvp.size(); i2++) {

						pPar = (SimB*)vvp[i2].pBase;
						//VarName=pPar->GetName().c_str();
						if (pPar->Is_Vector()) {

							for (int ii = 0; ii<((OutVector*)pPar)->GetNumberOfSelectedFlags(); ii++) {
								lin = ((OutVector*)pPar)->GetSelectedLocalIndex(ii);
								int ipg = ((OutVector*)pPar)->GetPgFileIndex(lin);
								il = 0;
								if (ipg == Index / 6) {
									il = lin;
									ii = ((OutVector*)pPar)->GetNumberOfSelectedFlags();
									VarName = pPar->GetName().c_str();
									i2 = vvp.size();
									hit = true;
									break;
								}
							}
						}
						else {
							int ipg = ((OutSingle*)pPar)->GetPgFileIndex();
							lin = -1;
							if (ipg == Index / 6) {
								VarName = pPar->GetName().c_str();
								i2 = vvp.size();
								hit = true;
								break;

							}
						}

					}

					if (hit) break;
				}

				if (lin>-1) VarName += "(" + FUtil::STD_ItoAscii(il + 1) + ")";
				st = "";
				switch (Index % 6) {
				case 0:
					st += "Initial";
					break;
				case 1:
					st += "Final";
					break;
				case 2:
					st += "Min";
					break;
				case 3:
					st += "Max";
					break;
				case 4:
					st += "Mean";
					break;
				case 5:
					st += "Accumulated";
					break;
				}

				QuickSetText(0, i, VarName);
				QuickSetText(1, i, st);

				string Type;

				if (datatype == DATA_TYPES::ANYDATA || m_pSimDoc != pDoc) {
					auto dtype = m_pSimDoc->m_SelectMultiSum[i].type;
					switch (dtype) {
					case DATA_TYPES::SIM_X: Type = "State Variables";	break;
					case DATA_TYPES::SIM_T: Type = "Flow Variables";	break;
					case DATA_TYPES::SIM_G: Type = "Auxiliary Variables";	break;
					case DATA_TYPES::SIM_D: Type = "Driving Variables";	break;
					}
				}



			}


		}




	}




}
void MakeSimSheets::JoinOutputListRows(size_t NumColToJoin) {
	CUGCell cell;
	auto maxrow = GetNumberRows();
	int colv = 0;
	string Group, PrevGroup;
	int LastRow, StartRow;
	LastRow = StartRow= 0;

	for (int i = 0; i < maxrow; i++) {
			UnJoinCells(0, i);
			UnJoinCells(1, i);
	}



	for (int i = 0; i<maxrow; i++) {
		GetCell(colv, i, &cell);
		Group = cell.GetStdText();
		SetCell(colv, i, &cell);
		if (PrevGroup == Group) {
			LastRow = i;
		}
		else {
			PrevGroup = Group;
			if (LastRow + 1>StartRow&&LastRow>0) {
				JoinCells(colv, StartRow, colv, LastRow);
			}
			StartRow = i;
		}
	}
	if (LastRow + 1>StartRow) {
		JoinCells(colv, StartRow, colv, LastRow);
	}
	colv = 1;
	if (NumColToJoin <= colv) return;
	LastRow = 0;
	StartRow = 0;
	for (int i = 0; i<maxrow; i++) {
		GetCell(colv, i, &cell);
		Group = cell.GetStdText();
		SetCell(colv, i, &cell);
		if (PrevGroup == Group) {
			LastRow = i;
		}
		else {
			PrevGroup = Group;
			if (LastRow + 1>StartRow&&LastRow>0) {
				JoinCells(colv, StartRow, colv, LastRow);
			}
			StartRow = i;
		}
	}
	if (LastRow + 1>StartRow) {
		JoinCells(colv, StartRow, colv, LastRow);
	}

	colv = 2;
	if (NumColToJoin <= colv) return;
	LastRow = 0;
	StartRow = 0;
	for (int i = 0; i<maxrow; i++) {
		GetCell(colv, i, &cell);
		Group = cell.GetStdText();
		SetCell(colv, i, &cell);
		if (PrevGroup == Group) {
			LastRow = i;
		}
		else {
			PrevGroup = Group;
			if (LastRow + 1>StartRow&&LastRow>0) {
				JoinCells(colv, StartRow, colv, LastRow);
			}
			StartRow = i;
		}
	}
	if (LastRow + 1>StartRow) {
		JoinCells(colv, StartRow, colv, LastRow);
	}



	int ivv = JoinCells(-1, 0, -1, maxrow - 1);

	if (maxrow == 0) 	m_pFrame->UpdatePaneText(0, "No Output Variables in Current Document !");
	else {
		string txt = FUtil::STD_ItoAscii(maxrow) + " Output Variables in Current Document";
		m_pFrame->UpdatePaneText(0, txt);


	}


}