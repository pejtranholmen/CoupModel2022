#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include "GridSimDoc.h"

#include "..\..\Util.h"
#include "..\..\Chart\ChartProperties\ChartProp.h"
#include "..\..\Chart\ChartProperties\ChartUtil.h"
#include "..\SimDoc\MyCug.h"
#include "..\..\PG\PGfileArchive.h"

//#include "afxole.h"

#include "..\..\NewBase\SimArchive.h"

GridSimDoc::GridSimDoc(void)
{

	m_pSimArchive=MySingle::instance().GetSimArchivePointer();
	m_pPGArchive=MySingle::instance().GetPGFileArchivePointer();

	m_pApp = AfxGetApp();
	m_pProp=MySingle::instance().GetChartPropPointer();
	m_pUtil=MySingle::instance().GetChartUtilPointer();
	m_SelectedSoilLayers.resize(50);
	m_SelectedPlants.resize(10);
	m_ShowProcesses = false;
	if (MFC_Util::GetProfileIntNo("ShowProcesses", 1) != 0) m_ShowProcesses = true;
	m_ShowAllSoilLayers= false;
	if(MFC_Util::GetProfileIntNo("ShowAllSoilLayer", 1)!=0) m_ShowAllSoilLayers = true;
	m_ShowAllPlants = false;
	if (MFC_Util::GetProfileIntNo("ShowAllPlants", 1) != 0) m_ShowAllPlants = true;
	m_ShowOnlyNonVectorOutputs=true;
	m_ShowPlantVectors = false;
	if (MFC_Util::GetProfileIntNo("ShowPlantVectors", 1) != 0) m_ShowPlantVectors = true;
	m_ShowSoilProfile = false;
	if (MFC_Util::GetProfileIntNo("ShowSoilProfile", 1) != 0) m_ShowSoilProfile = true;
	m_ShowScalar = false;
	if (MFC_Util::GetProfileIntNo("ShowScalar", 1) != 0) m_ShowScalar = true;
	m_Show_C = false;
	if (MFC_Util::GetProfileIntNo("Show_C", 1) != 0) m_Show_C = true;
	m_Show_N = false;
	if (MFC_Util::GetProfileIntNo("Show_N", 1) != 0) m_Show_N = true;
	m_Show_P = false;
	if (MFC_Util::GetProfileIntNo("Show_P", 0) != 0) m_Show_P = true;
	m_Show_Water =false;
	if (MFC_Util::GetProfileIntNo("Show_Water", 0) != 0) m_Show_Water = true;
	m_Show_Heat = false;
	if (MFC_Util::GetProfileIntNo("Show_Heat", 0) != 0) m_Show_Heat = true;
	m_Show_SoilGas = false;
	if (MFC_Util::GetProfileIntNo("Show_SoilGas", 0) != 0) m_Show_SoilGas = true;
	m_Show_Climate = false;
	if (MFC_Util::GetProfileIntNo("Show_Climate", 0) != 0) m_Show_Climate = true;
	m_Show_All_Elements = false;
	if (MFC_Util::GetProfileIntNo("Show_All_Elements", 1) != 0) m_Show_All_Elements = true;

}


GridSimDoc::~GridSimDoc(void)
{
}
void GridSimDoc::MakeRunInfo_PostMenu(int col, int row) {

	AddMenuItem(3000,"Attach additional information from another multi simulation that represent the same parameters and validations variables");
	AddMenuItem(3001,"Adjust the size of the Residual files to include only specified number of runs");
	AddMenuItem(3002,"Attach a new Multistorage file with residuals");
	AddMenuItem(3003,"Restore to Original MultiStorage file");

}
void GridSimDoc::MakeRunInfo_PriorMenu(int col, int row) {
	CUGCell cell;
	long ParV;
	GetCell(col,row, &cell);
		ParV=cell.GetParam();
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
				info_i=11;
				break;
		}
		SimB *pBase;
		if(info_i>=0) {
			pBase=(SimB*)m_pSimDoc->m_RunInfo[info_i].pBase;
			int num=pBase->GetNumOfHistoryIndex();
			if(num>0)
				AddMenuItem(2080,"View History");

		}
		else if(!m_pSimDoc->IsFinishedSimulation()) {
				AddMenuItem(1999,"Force a not completed Simulaton to be read as completed");
				
		}

		if (m_pSimDoc->IsFinishedSimulation() && m_pSimDoc->IsMultiSimulation()) {
			AddMenuItem(4000, "Export All MultiRun Results to CSV Text file");
			AddMenuItem(4001, "Export Accepted MultiRun Results to CSV Text file");
			AddMenuItem(4002, "Export All MultiRun Results to Excel file");
			AddMenuItem(4003, "Export Accepted MultiRun Results to Excel file");
			AddMenuItem(4004, "Export All Simulated & Measured Values to Excel file");

		}
		if(!m_pSimDoc->IsFinishedSimulation()) 	AddMenuItem(4021, "Read Document Settings from excel file");
		AddMenuItem(4020, "Save Document Settings to excel file");
		AddMenuItem(4022, "Save All Items in Model to excel file (including disabled)");
		AddMenuItem(4030, "Save Document Settings to XML file (non default)");
		AddMenuItem(4032, "Save All Items in Model to XML file (enabled)");
		AddMenuItem(4033, "Save All Items in Model to XML file (including disabled)");

		

}
void GridSimDoc::MakeRunInfo_DataBaseMenu(int col, int row) {
	CUGCell cell;
	string str;
				int iobject;
				GetCell(0,row,&cell);
				str=cell.GetStdText();
				iobject=1200+row-18;
				CDB* pDB;
				pDB=m_pSimDoc->DB_GetPointer(DB_types(row-19));
				int num=pDB->GetNumOfHistoryIndex(row-19);
				if(num>0) 
					AddMenuItem(2080,"View History");


}
void GridSimDoc::MakeSwitchPriorMenu(int col, int row) {
CUGCell cell1, cell;
string Name, Group, type;
	
		if(col==0) {
			GetCell(0,row,&cell);
			Group=cell.GetStdText();
			
			AddMenuItem(1001, "View all Options for ("+Group+") module");
			AddMenuItem(1003, "Hide Default Set Options for ("+Group+") module");
		}		


		GetCell(1,row, &cell1);
		cell1.GetStdText(&Name);
		GetCell(0,row, &cell1);
		cell1.GetStdText(&Group);
		type="Switches";
		SimB *pSw=m_pSimDoc->GetPtr(type, Group, Name);
		if(pSw!=nullptr) {
			if(pSw->IsOldValue()) {
				size_t oldvalue=((Sw*)pSw)->GetOldIntValue();
				string str=pSw->GetOption(oldvalue);
				AddMenuItem(2018,"Reset to previous value :"+str);
			}
			int NumHist=pSw->GetNumOfHistoryIndex();
			if(NumHist>0) {
				AddMenuItem(2080,"View History: ");
			}
		}


}
void GridSimDoc::MakeSwitchPostMenu(int col, int row) {
	CUGCell cell1, cell;
	string Name, Group, type;
	GetCell(1,row, &cell1);
	cell1.GetStdText(&Name);
	GetCell(0,row, &cell1);
	cell1.GetStdText(&Group);
	type="Switches";
	Sw *pSw=(Sw*)m_pSimDoc->GetPtr(type, Group, Name);
	if(pSw!=nullptr) {
		int NumHist=pSw->GetNumOfHistoryIndex();
		if(NumHist>0) {
			AddMenuItem(2080,"View History: ");

		}
	}
}
void GridSimDoc::MakeParameterTablePriorMenu(int col, int row) {
	CUGCell cell,cell1, cell2; 
	string Group, type, Name;
	SimB *pCP, *pSw;
	P *pCPt;
	Tab *pTab;
	long ParV;
	string optionlist;
			if(col>=2) {
				if(m_newChart) AddMenuItem(4001, "Make new plot");
			}
			if(col==2) {
				if(!m_pSimDoc->m_HideAllTableContents) 
					AddMenuItem(2050,"View All rows in All Tables");
				else
					AddMenuItem(2050,"Hide All rows inside All Tables");


			}
			GetCell(col, row, &cell);
			ParV=cell.GetParam();
			if (ParV==3000) {
					// Change of Element value
					GetCell(col-3,row, &cell1);
					cell1.GetStdText(&Name);
					GetCell(col-5,row, &cell1);
					cell1.GetStdText(&Group);
					GetCell(col,row, &cell1);
		
					type="Tables";
					
					pCP=m_pSimDoc->GetPtr(type, Group, Name);
					if(pCP!=nullptr) {
						int NumHist=pCP->GetNumOfHistoryIndex();
						if(NumHist>0) {
							AddMenuItem(2080,"View History: ");
						}
					}

					// Table pointer valid make action
						

				
				}
			else if (ParV>=2000) { //  Value within tables
					int index;
					index=ParV-2000;

					GetCell(2,row-index-2,&cell);
					cell.GetStdText(&Name);
					GetCell(0,row-index-2,&cell);
					cell.GetStdText(&Group);
					type="Tables";
			
					pSw=m_pSimDoc->GetPtr(type, Group, Name);
					pTab=(Tab*)pSw;
					GetCell(col,row-index-1,&cell);
					cell.GetStdText(&Name);
					type="Table Parameters";
	
					pCP = m_pSimDoc->GetP_Pointer(Name);

					pCPt=(P*)pCP;
					int nView=0;
					if(pCPt!=nullptr&&pTab!=nullptr) {
						nView=pCPt->GetViewFunction();
						if(!(nView==405||nView==406)) {
							AddMenuItem(2001, "Change ("+Name+"("+FUtil::STD_ItoAscii(index+1)+")) value during Simulation");		
							AddMenuItem(2002, "Set ("+Name+"("+FUtil::STD_ItoAscii(index+1)+")) Parameter for MultiRun Simulations");
							if(pCPt->IsOldValue(index)) {
								float oldvalue=pCPt->GetOldValue(index);
								AddMenuItem(2018,"Reset to previous value :"+FUtil::STD_FtoAscii(oldvalue));
							}
							int NumHist=pCP->GetNumOfHistoryIndex(index);
							if(NumHist>0) {
								AddMenuItem(2080,"View History: ");
							}

						}

		
				string GTitle=((P*)pCP)->GetViewTitle();
		
				size_t no_func;
				no_func=((P*)pCP)->GetNumOfTitles();
				for(size_t i=0; i<no_func;i++) {
					AddMenuItem(2003+i, ((P*)pCP)->GetViewTitle(i).c_str());
				}

						switch (nView) {
					
						case 2304:
							AddMenuItem(2003,"Plot Roots f(N leaf content)");
							AddMenuItem(2004,"Plot Roots f(Mass above ground)");
							AddMenuItem(2005,"Plot Roots f(Transp ratio)");
							AddMenuItem(2006,"Plot Leaf f(mass above ground)");



						//optionlist="Roots f(N leaf content)\nRoots f(Mass above ground)\nRoots f(Transp ratio)\nLeaf f(mass above ground)\n";
							break;
						case 1005:
						
							AddMenuItem(2003,"Plot PM equation, sensitivy");
							AddMenuItem(2004,"Plot Resistances");
							break;
						case 1016:
							AddMenuItem(2003,"Plot PM equation, sensitivy");
							AddMenuItem(2004,"Plot Resistances");
							break;
						case 1007:
							//	optionlist="Roughness f(LAI)\nDisplacement f(LAI)\nLohammar f(VPD)\nLohammar f(RIS)\nSurf. Resistances\n";
							//	optionlist+="Aero. Resistances\nResistances\nPM-equation, sensitivity\n";
								AddMenuItem(2003,"Roughness f(LAI)");
								AddMenuItem(2004,"Displacement f(LAI)");
								AddMenuItem(2005,"Lohammar f(VPD)");
								AddMenuItem(2006,"Lohammar f(RIS)");
								AddMenuItem(2007,"Canopy Resistances");
								AddMenuItem(2008,"Aerodynamic Resistances");
								AddMenuItem(2009,"Resistances");
								AddMenuItem(2010,"Plot PM equation, sensitivy");
						break;

//						optionlist="LAI f(leafmass)\nRootsDepth f(mass)\nHeight f(mass, age)\n";
//							break;

						case 2314:

							optionlist="CO2_Light()\nCO2_LAI()\nP_Light()\nP_LAI()\nP_PCO2()\n";
							AddMenuItem(2003,"CO2 Light");
							AddMenuItem(2004,"CO2 LAI()");
							AddMenuItem(2005,"P LAI");
							AddMenuItem(2006,"P CO2");
							break;
						default:
							if(nView>0) {
								string Title=pTab->GetViewTitle();
								AddMenuItem(2003,"Plot "+Title);
							}



							break;
							
					}
						
				}
					// Make Action if P pointer with index found
			}

}
void GridSimDoc::MakeParameterPriorMenu(int col, int row) {
	CUGCell cell1, cell;
	string Name, Group, type;
	SimB *pCP;
	int ParV;

			if (col>=2) {
					GetCell(2,row, &cell1);
					cell1.GetStdText(&Name);
					AddMenuItem(2001, "Change ("+Name+") value During Simulation");		
					AddMenuItem(2002, "Set ("+Name+") Parameter for MultiRun Simulations");
//Parameter plot preparation
					GetCell(0,row, &cell1);
					cell1.GetStdText(&Group);
					type="Parameters";
					pCP=m_pSimDoc->GetPtr(type, Group, Name);
					if(pCP!=nullptr) {
						string GTitle;												
						GTitle=((Ps*)pCP)->GetViewTitle();
						if(((Ps*)pCP)->IsOldValue()) {
							float oldvalue=((Ps*)pCP)->GetOldValue();
							AddMenuItem(2018,"Reset to previous value :"+FUtil::STD_FtoAscii(oldvalue));
						}
						if(GTitle!="") {
							AddMenuItem(2020,"Plot view function: "+GTitle);
						}
					}
			}
			if(col==0) {
				GetCell(0,row,&cell);
				Group=cell.GetStdText();
				//AddMenuItem(1000, "Add All Options");
				AddMenuItem(1001, "View all parameters for ("+Group+") module");
				//AddMenuItem(1002, "Remove All Default Set Options");
				AddMenuItem(1003, "Hide Default Set parameters for ("+Group+") module");
			}
			GetCell(col, row, &cell);
			ParV=cell.GetParam();


			if(ParV==0) {
					GetCell(2,row, &cell1);
					cell1.GetStdText(&Name);
  					GetCell(0,row, &cell1);
					cell1.GetStdText(&Group);


					type="Parameters";
					pCP=m_pSimDoc->GetPtr(type, Group, Name);
					if(pCP!=nullptr) {
						int NumHist=pCP->GetNumOfHistoryIndex();
						if(NumHist>0) {
							AddMenuItem(2080,"View History: ");
						}
					}
			}
			


}
void GridSimDoc::MakeParameterPostMenu(int col, int row) {



}

int GridSimDoc::OnMenuStart(int col,long row,int section)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(section);

	CUGCell cell,cell1, cell2; 
	string Group, type, Name;
	SimB *pCP, *pSw;
	P *pCPt;
	Tab *pTab;
	long ParV;
	CString optionlist;

	int sheet=GetSheetNumber();
	EmptyMenu();
/*	if(m_pSimDoc->p_ModelInfo->IsRunning()&&sheet!=13) {
		MessageBox(" You can not modify document when modell is running !");
		return FALSE;
	}*/
	if(section == UG_GRID){
//***** Clear all menu items 
	EmptyMenu();

	if(m_pSimDoc->IsFinishedSimulation()&&sheet<11)
		AddMenuItem(1997, "Copy to clipboard");
	else if(sheet<11&&sheet>3&&sheet!=9) {
		AddMenuItem(1997, "Copy to clipboard");
		//AddMenuItem(1998, "Paste from clipboard"); 
	}
	string str;

	switch (sheet) {
		case(0)	:		
			if(row>18&&row<=28&&col==0) 
				MakeRunInfo_DataBaseMenu(col, row);
			else if (row>32&&m_pSimDoc->IsFinishedSimulation()) 
				MakeRunInfo_PostMenu(col, row);
			else 
				MakeRunInfo_PriorMenu(col, row);
			break;
		case (1):
			 if (col>=1&&!m_pSimDoc->IsFinishedSimulation()) 
				MakeSwitchPriorMenu(col, row);
			else if (col>=1&&m_pSimDoc->IsFinishedSimulation()) 
				MakeSwitchPostMenu(col, row);
			break;
		case (2):
			MakeParameterPriorMenu(col, row);
			break;
		case (3):
			MakeParameterTablePriorMenu(col, row);
			break;
		case (4):
			GetCell(col,row, &cell); 
			ParV=cell.GetParam();

			if(ParV==2000) {
				MakeChartMenu(col,row);
			}
			else if (ParV >= 2200&&ParV<2204) {
				str = "Remove file :";
				str+=m_pSimDoc->m_ValsumFlexFiles[ParV - 2200].FileName;
				AddMenuItem(3010, str.c_str());


			}
			else if (ParV >= 1100 && ParV <= 1200) {
				AddMenuItem(1150, "Select New File");
				AddMenuItem(1151, "Remove Selected Csv File");

			}
			else {
				GetCell(1,row, &cell1);
				cell1.GetStdText(&Name);
				GetCell(0,row, &cell1);
				cell1.GetStdText(&Group);
				type="Files";
			
				pSw=m_pSimDoc->GetPtr(type, Group, Name);

				if(pSw!=nullptr) {
						int NumHist=pSw->GetNumOfHistoryIndex();
						if(NumHist>0) {
							AddMenuItem(2080,"View History: ");
						}
				}
				AddMenuItem(3000, "Select PG File");
				AddMenuItem(3001, "Create New PG File");
				if(Group=="Validation") {
					if(((F*)pSw)->GetNumberOfSelectedVal()>0)
						AddMenuItem(3002,"Remove used Validation File");
					else
						AddMenuItem(3002,"Remove not used Validation File");					
				
				}
			}
			break;

		case (5):
			if(!m_pSimDoc->IsFinishedSimulation()) 
				MakeSelectOutputMenu(col, row);		
			else 
				MakeChartMenu(col, row, sheet);			
			break;
		case (6):
			if(!m_pSimDoc->IsFinishedSimulation()) 
				MakeValidationPriorMenu(col, row);
			else if(!m_ShowValidationTimeSerieTable&&!m_ShowValidRes&&m_pSimDoc->IsFinishedSimulation()) 
				MakeChartMenu(col, row, sheet);
			else if (!m_ShowValidationTimeSerieTable&&m_pSimDoc->IsFinishedSimulation()) {
				MakeValidationPostMenu(col, row);
				break;
		case(8) :
			if (m_ShowChartTable) {
				AddMenuItem(5997, "Select All Outputs");
				return TRUE;
			}
				if (GetNumberRows() <= 0)
					return FALSE;
				GetCell(0, row, &cell);
				str = cell.GetStdText();
				AddMenuItem(4000, "Remove all Charts");
				AddMenuItem(1100, "Delete Chart");
				AddMenuItem(1101, "Delete Variable from Chart");
				AddMenuItem(1102, "Toggle Chart Type");
				AddMenuItem(1104, "Toggle Color");
				if (m_pSimDoc->IsFinishedSimulation()) {
					AddMenuItem(1105, "Draw Chart no: " + str);
					AddMenuItem(1106, "Show Table for Chart no: " + str);
				}
				break;
		case (9) :
			if (!m_pSimDoc->IsFinishedSimulation()) {
				GetCell(0, row, &cell);
				string grp = cell.GetStdText();
				GetCell(1, row, &cell);
				string Name = cell.GetStdText();

				string txt = "Edit " + Name;
				AddMenuItem(1050, txt);


			}
				 return TRUE;
				 break;
		case 10:
			if (m_pSimDoc->IsMultiSimulation() && m_pSimDoc->IsFinishedSimulation() && m_ShowMultiRes) {
				if (col == 2) {
					GetCell(2, row, &cell);
					string txt = cell.GetStdText();

					AddMenuItem(1050, "Open Sim Document: " + txt);

				}
				else if (col == 1||col==0||col==2) {
					GetCell(0, row, &cell);
					if (cell.GetParam() >= 1000) {
						string txt = FUtil::STD_ItoAscii(size_t(cell.GetParam() - 999));
						AddMenuItem(1055, "Open a new sim Document with Mean Value of the specific Calibrated Parameter for Ensemble : " + txt);
						if(m_pSimDoc->MR_Get_NumBayesPar()>1) 
							AddMenuItem(1056, "Open a new sim Document with Mean Value of All Calibrated Parameter for Ensemble : " + txt);
					}
					GetCell(2, row, &cell);
					if (cell.GetParam() >= 1000) {

					}



				}
			}
			else if (m_pSimDoc->IsFinishedSimulation())
				MakeChartMenu(col, row, sheet);
			else if (!m_pSimDoc->IsFinishedSimulation()) {
				MakeMultiRunPriorMenu(col, row);

			}


			break;
		case 11:
			MakeHistoryMenu(col, row);
			break;

		case (12) :

			AddMenuItem(3200, "Remove all Selected Annimation Variables");
			AddMenuItem(3201, "Reset Selected Annimation Variables to default choice");

			break;
		case (13) :
			if (row > 0) {
				  AddMenuItem(3000, "Remove all Attached Files");
				  AddMenuItem(3001, "Remove Selected Attached Files");
			}
			AddMenuItem(3002, "Attach additions MultiRuns (same format)");

			break;
	      }


		}
	}
	else if(section==UG_CORNERBUTTON) {
		EmptyMenu();
		if(sheet==6) {
			MakeValidationMenu(col, row);



		}

		else if(sheet==10&&m_pSimDoc->IsFinishedSimulation()) {

			MakeMultiRunMenu(0,0);

		}
		else if (sheet==11) {

			MakeHistoryMenu(0,0);


		}
		else if(sheet>0&&sheet<4||sheet==5) {
				if(sheet!=5) {
					if(m_ParameterTableView==0)
						AddMenuItem(7120, "View Linked Documents");
					else
						AddMenuItem(7120, "View Documents individually");


				}
				if(m_ViewCategory!=0) AddMenuItem(7100,"View all Categories");
			for(int i=1;i<9;i++) {
				string str="View : ";
				str+=p_ModelInfo->GetGroupCategoryNames(i);
				if(m_ViewCategory!=i) AddMenuItem(7100+i,str);
			}
			if(sheet==5) {
				Doc* pMySimDoc;
				vector<Doc*> ReadyDocuments=FUtil::GetReadyDoc(m_pSimDoc);

				if(ReadyDocuments.size()>0) {
					string text;
					for(size_t i=0; i<ReadyDocuments.size();i++) {
						text=FUtil::STD_ItoAscii(ReadyDocuments[i]->m_DocFile.m_SimulationRunNo);
						AddMenuItem(2500+i,"Move Output Setting to to docuement: "+text);

					}
				}
			}


		}

	}
	else if(section==UG_SIDEHEADING) {
		EmptyMenu();
		if(m_pSimDoc->IsFinishedSimulation()) {
				AddMenuItem(7000,"Create Copy of document");
		}
		else {
				AddMenuItem(7001,"Start a single run simulation");
			
				if(m_pSimDoc->m_MultiRun_Array.size()>0)
					AddMenuItem(7002,"Start a sequence of multi run simulation");

				if(m_pSimDoc->m_MultiRun_Array.size()>0&&m_pSimDoc->MR_Get_ActualRunNumber()<m_pSimDoc->MR_Get_TotalNumberofRuns()&&m_pSimDoc->MR_Get_ActualRunNumber()>1) {
					AddMenuItem(7003,"ReStart a sequence of multi run run simulation");
				}
				else if(m_pSimDoc->m_MultiRun_Array.size()>0) {
					AddMenuItem(7003,"Extend previous multi run simulation to a new multirun");
				}



		}
		if(sheet==11) {
				AddMenuItem(3000,"Open a PG bin file");
				AddMenuItem(3001,"Create a PG bin file");
				AddMenuItem(3015,"Create new variables and a new PG bin file");
		}
	}
	else if(section==UG_TOPHEADING) {
	auto Process_ElementView = [&](int sheetview) {
		string txt = "";
		if (sheetview == 1) txt = "Switches";
		else if (sheetview == 2) txt = "Parameters";
		else if (sheetview == 3) txt = "Parameter Tables";
		else if (sheetview == 5) txt = "Output Variables";
		if (m_ShowProcesses) AddMenuItem(5300, "View "+txt+" by Element");
		else AddMenuItem(5300, "View "+txt+" by Process");
		if (m_Show_All_Elements) {
			AddMenuItem(5308, "Select Elements to View");


		}
		else {
			AddMenuItem(5308, "View All Elements");
			if (m_Show_C) AddMenuItem(5301, "Do not View Carbon Elements");
			else AddMenuItem(5301, "View Carbon Elements");
			if (m_Show_N) AddMenuItem(5302, "Do not View Nitrogen Elements");
			else AddMenuItem(5302, "View Nitrogen Elements");
			if (m_Show_P) AddMenuItem(5303, "Do not View Phosphorus Elements");
			else AddMenuItem(5303, "View Phosphorus Elements");
			if (m_Show_Water) AddMenuItem(5304, "Do not View Water Elements");
			else AddMenuItem(5304, "View Water Elements");
			if (m_Show_Heat) AddMenuItem(5305, "Do not View Heat Elements");
			else AddMenuItem(5305, "View Heat Elements");
			if (m_Show_SoilGas) AddMenuItem(5306, "Do not View Soil Gas Elements");
			else AddMenuItem(5306, "View Soil Gas Elements");
			if (m_Show_Climate) AddMenuItem(5307, "Do not View Climate Related Elements");
			else AddMenuItem(5307, "View Climate Related Elements");
		}


	};
		if(sheet==1||sheet==2||sheet==3){
			if (col == 1) Process_ElementView(sheet);
		}
		else if(sheet==5) {
			if (col == 2) Process_ElementView(sheet);
			else if (col == 3) {
				if (m_ShowScalar) AddMenuItem(5331, "Do not view Scalar Outputs");
				else AddMenuItem(5331, "View Scalar Output");

				if (m_ShowSoilProfile) {
					AddMenuItem(5311, "Do not View Soil Profile");
					if (m_ShowAllSoilLayers)
						AddMenuItem(5310, "View Selected Soil Layers");
					else
						AddMenuItem(5310, "View All Soil Layers");
				}
				else
					AddMenuItem(5311, "View Soil Profile");
				if (m_ShowPlantVectors) {
					AddMenuItem(5321, "Do not View Plant Vectors");
					if (m_ShowAllPlants)
						AddMenuItem(5320, "View Selected Plants");
					else
						AddMenuItem(5320, "View All Plants");
				}
				else
					AddMenuItem(5321, " View Plant Vectors");
			}
			else if(col==4){
				if(m_LinkedChartDocument)
					AddMenuItem(3400,"Un link connection to other open documents");
				else if(UpdateDocumentLink()) {
						string NumStr;
						NumStr=FUtil::STD_ItoAscii(m_NumOpenDoc);
						AddMenuItem(3401,"Make connection to all open documents ("+NumStr+")");
				}			
			}


		}
		else if(sheet==11) MakeHistoryMenu();
	
	}

return TRUE; 
}
void GridSimDoc::MakeHistoryMenu(int col, int row) {

	if(col<0&&row<0) {
		if (!m_pSimDoc ->m_ViewAll) AddMenuItem(2070,"View All Changes");
			if (!m_pSimDoc->m_ToView[0]) 	AddMenuItem(2072, "View Run History");
			else 	AddMenuItem(2072, "Hide Run History");
			if (!m_pSimDoc->m_ToView[1]) AddMenuItem(2073, "View Data Base");
			else AddMenuItem(2073, "Hide Data Base");
			if (!m_pSimDoc->m_ToView[2]) AddMenuItem(2074, "View Switches");
			else AddMenuItem(2074, "Hide Switches");
			if (!m_pSimDoc->m_ToView[3]) AddMenuItem(2075, "View Parameters");
			else AddMenuItem(2075, "Hide Parameters");
			if (!m_pSimDoc->m_ToView[4]) AddMenuItem(2076, "View Element Numbers");
			else    AddMenuItem(2076, "Hide Element Numbers");
			if (!m_pSimDoc->m_ToView[5]) AddMenuItem(2077, "View Parameter Tables");
			else AddMenuItem(2077, "Hide Parameter Tables");
			if (!m_pSimDoc->m_ToView[6]) AddMenuItem(2078, "View Model Files");
			else AddMenuItem(2078, "Hide Model Files");
		AddMenuItem(2090,"Clean History List");
	}
	else if(col>=2&&col<=3) {
		CUGCell cell; 
		string Group, type, Name;
		GetCell(1, row, &cell);
		type = cell.GetStdText();
		GetCell(2, row, &cell);

		Group = cell.GetStdText();

		AddMenuItem(1997, "Copy to clipboard");
		if (type != "Run info") {
 			GetCell(3, row, &cell);
			string str;
			str = cell.GetStdText();
			AddMenuItem(2097, "View all Changes of: " + str);

		}


	}
	else if(col==0&&row==0) {
		// Check if another open document is not completed and can recieve the history list
		Doc* pMySimDoc;
		vector<Doc*> ReadyDocuments=FUtil::GetReadyDoc(m_pSimDoc);

		if(ReadyDocuments.size()>0) {
			string text;
			for(size_t i=0; i<ReadyDocuments.size();i++) {
				text=FUtil::STD_ItoAscii(ReadyDocuments[i]->m_DocFile.m_SimulationRunNo);
				AddMenuItem(2000+i,"Move history List to docuement: "+text);

			}
		}
	}
	else {
		AddMenuItem(1997, "Copy to clipboard");

	}
	



}
void GridSimDoc::MakeValidationMenu(int col, int row) {

	if(m_pSimDoc->IsMultiSimulation()) {

		if(m_pSimDoc->IsMultiStorageFileLocked()) {
			AddMenuItem(2002,"View statistics for accepted runs");
			AddMenuItem(2003,"Unlock Multi Storage file to redefine accepted runs");
			if(m_ShowValidationTimeSerieTable)
				AddMenuItem(7010,"Do not view Time Serie Table for each run");
			else
				AddMenuItem(7010,"View Time Serie Table for each run");
		}
		else {
			if(m_ShowValidCoVarRes)
				AddMenuItem(2001,"Show Table of Validation Result");
			else
				AddMenuItem(2001,"Show Co Correlation between parameters and validation");
			AddMenuItem(2002,"Specify new criterias to define accepted runs");
			AddMenuItem(2003,"Lock Multi Storage file to save accepted runs");
			if(m_ShowValidationTimeSerieTable)
				AddMenuItem(7010,"Do not view Time Serie Table for each run");
			AddMenuItem(7010,"View Time Serie Table for each run");
		}
	}
	Doc* pMySimDoc;
	vector<Doc*> ReadyDocuments=FUtil::GetReadyDoc(m_pSimDoc);
	if(ReadyDocuments.size()>0) {
		if(m_pSimDoc->m_Val_Array.size()>0) {
			string text;
			for(size_t i=0; i<ReadyDocuments.size();i++) {
				text=FUtil::STD_ItoAscii(ReadyDocuments[i]->m_DocFile.m_SimulationRunNo);
				AddMenuItem(2050+i,"Move All Validation Options to docuement: "+text);

			}

		}


	}
	
}
void GridSimDoc::MakeMultiRunMenu(int col, int row) {


	if(col==0&&row==0) {
		if(m_pSimDoc->IsMultiSimulation()) {
			if (m_ShowCoVarRes) {
				AddMenuItem(2000, "Show Posterior Table of Parameters");
			}
			else {
				AddMenuItem(2000, "Show Co Correlation between parameters");
			}
			if(!m_ShowAllEnsemblePostParList) 
				AddMenuItem(2001, "Show Normal List with Posterior statistics with All Ensembles");
		}

		// Check if another open document is not completed and can recieve the history list
		Doc* pMySimDoc;
		vector<Doc*> ReadyDocuments=FUtil::GetReadyDoc(m_pSimDoc);

		if(ReadyDocuments.size()>0) {
			string text;
			for(size_t i=0; i<ReadyDocuments.size();i++) {
				text=FUtil::STD_ItoAscii(ReadyDocuments[i]->m_DocFile.m_SimulationRunNo);
				AddMenuItem(2050+i,"Move Multi Run Options to docuement: "+text);

			}
		}

	}
	



}

void GridSimDoc::MakeValidationPostMenu(int col, int row) {
	CUGCell cell;
	int ifile=m_pSimDoc->m_ValidationData.GetValFileNumber(1);
	if (m_pSimDoc->IsBayesianCalibration()) {
		MFC_Util mfc_util;
		mfc_util.SetPaneMessage("A Bayesian Calibration can not be reset");
	}
	if(m_pSimDoc->MR_XBin_IsOpen(ifile)&&m_ShowValidRes_Changed) {
		AddMenuItem(1060,"Update the list of Accepted Runs");
	}
	if(m_pSimDoc->MR_XBin_IsOpen(ifile)) {
		AddMenuItem(1061,"Reset the list of Accepted Runs to all Runs");
	}
	int nrow=2;
	if(m_pSimDoc->MR_XBin_IsOpen(ifile)) nrow=4;
	if(row>nrow&&col<2) {
		GetCell(1,row,&cell);
		string txt=cell.GetStdText();
		AddMenuItem(1050,"Open Sim Document: "+txt);
	}	
	string t;
	t = FUtil::STD_ItoAscii(m_pSimDoc->m_ValsumFlexFiles.size());
	t += " Single Variable Validation files are valid. ";
	t += "Add New File";
	AddMenuItem(6020, t);

	ChartUtil *pUtil;
	pUtil = MySingle::instance().GetChartUtilPointer();
	if (pUtil->m_AutoShiftFirstIndep)
		t = "Set first indep variable to common Variable";
	else
		t = "Shift to new first indep for each Sum Variable to plot";

	AddMenuItem(6071, t);
	if (m_pSimDoc->m_ValsumFlexFiles.size() > 0) {
		t = "Add Variable Validation from File (";
		t += m_pSimDoc->m_ValsumFlexFiles.back().FileName;
		t += ")";
		AddMenuItem(6021, t);
	}


}
void GridSimDoc::MakeValidationPriorMenu(int col, int row) {
			CUGCell cell;
			string t;
				int NumValFiles=0;
				int icountTot=0;
				F *pValFile;
				NumValFiles=m_pSimDoc->GetNumValidValFiles();
				for(size_t i=0;i<MAXSIMVAL;i++) {
					pValFile=m_pSimDoc->ValidationFilePointer(i+1);
					icountTot+=pValFile->GetNumberOfSelectedVal();
				}
				t=FUtil::STD_ItoAscii(NumValFiles);
				t+=" Validation files are valid. ";
				t+="Add New File";

				AddMenuItem(6000,t);
				if(col==1) {
					GetCell(1,row,&cell);
					t="Remove Validation File (";
					t+=cell.GetStdText();
					t+=") and All Related Validation Variables";
					AddMenuItem(3002,t);
				}
				else if(NumValFiles>0||icountTot>0) {
					GetCell(0,row,&cell);
					t="Remove Validation Variable (";
					t+=cell.GetStdText();
					t+=")";
					AddMenuItem(6002,t);
					AddMenuItem(6010,"Disable Time Serie Validation (TSV)");
					AddMenuItem(6001,"Add New TSV Validation Variable");
					AddMenuItem(6061, "Add New Grouped TSV validation Variable");
				}
				AddMenuItem(6011,"Add New Single Value Validation (SV)");
				t = FUtil::STD_ItoAscii(m_pSimDoc->m_ValsumFlexFiles.size());
				t += " Single Variable Validation files are valid. ";
				t += "Add New File";

				AddMenuItem(6020, t);
				if (m_pSimDoc->m_ValsumFlexFiles.size() > 0) {
					t = "Add Variable Validation from File (";
					t += m_pSimDoc->m_ValsumFlexFiles.back().FileName;
					t += ")";
					AddMenuItem(6021, t);
				}

}
void GridSimDoc::MakeSelectOutputMenu(int col, int row) {
			CUGCell cell;
			int Index;
			int selected, vektor;
			string Group, Name,type;
			selected=0;
			AddMenuItem(1080, "Deselect all items");
			AddMenuItem(1081, "Select all items");
			AddMenuItem(1090, "Save Current Selection to csv file");
			AddMenuItem(1091, "Retrieve Selections from csv file");
			if (row < 0) return;

			if(col<2) {
					GetCell(1,row,&cell);
					Group=cell.GetStdText();
					size_t iv=p_ModelInfo->GetGroupIndex(Group);
					if(m_pSimDoc->GetGroupEnable(iv))
						AddMenuItem(1003, "Hide non set output variables for ("+Group+") module");
					else
						AddMenuItem(1001, "View all output variables for ("+Group+") module");
					
			}
			else if(col==3) {
					GetCell(1,row,&cell);
					Group=cell.GetStdText();
					GetCell(3,row,&cell);
					if(m_pSimDoc->IsFinishedSimulation())
						Name=cell.GetStdLabelText();
					else
						Name=cell.GetStdLabelText();
					GetCell(0,row, &cell);
					cell.GetStdText(&type);
					int pos1=Name.rfind('(');
					int pos2=Name.rfind(')');
					if (pos2>pos1&&pos2==Name.size()-1) {
						string t=Name.substr(pos1+1,pos2-pos1-1);
						Index=FUtil::AtoInt(t);
						if(Index>0) {
							Name=Name.substr(0,pos1);
							vektor=TRUE;
						}
						else {
							Index=1;
							vektor=FALSE;
						}
					}
					else {
						Index=1;
						vektor=FALSE;
					}

					//SimB *pSw = m_pSimDoc->GetPtr(Type, Group, Name);
					//OutVector *pVec = dynamic_cast<OutVector*>(pSw);

					SimB *pSw = m_pSimDoc->GetPtrByName(Name, Group);
					if (pSw != nullptr) {
						if (pSw->Is_Vector()) 
							selected = ((OutVector*)pSw)->GetStoreFlag(Index - 1);
						else 
							selected = ((OutSingle*)pSw)->GetStoreFlag();
					}

					if(selected==0) {
						AddMenuItem(1050,"Set all enabled output variables for ("+Group+") module ("+type+") Type");
						AddMenuItem(1052,"Set all enabled output variables for ("+Group+") module");
					}
					else {
						AddMenuItem(1051,"DeSelect all enabled output variables for ("+Group+") module ("+type+") Type");
						AddMenuItem(1053,"DeSelect all enabled output variables for ("+Group+") module ");
					}
					if(vektor) {
						if(selected==0)
							AddMenuItem(1060,"Set all enabled output variables for ("+Name+") variable");
						else
							AddMenuItem(1061,"Deselect all enabled output variables for ("+Name+") variable");
					}

				}

}
void GridSimDoc::MakeChartMenu(int col, int row, int sheet)
{				
			CUGCell cell;
			int Index;
			int selected, vektor;
			string Group, Name,type;
			selected=0;

			if(col==3&&sheet!=10&&sheet!=11&&sheet!=6) {
				GetCell(1,row,&cell);
				Group=cell.GetStdText();
				GetCell(3,row,&cell);
				if(m_pSimDoc->IsFinishedSimulation())
					Name=cell.GetStdLabelText();
				else
					Name=cell.GetStdText();
				GetCell(0,row, &cell);
				cell.GetStdText(&type);
				int pos1=Name.rfind('(');
				int pos2=Name.rfind(')');
				if (pos2>pos1&&pos2==Name.size()-1) {
					string t=Name.substr(pos1+1,pos2-pos1-1);
					Index=FUtil::AtoInt(t);
					if(Index>0) {
						Name=Name.substr(0,pos1);
						vektor=TRUE;
					}
					else {
						Index=1;
						vektor=FALSE;
					}
				}
				else {
					Index=1;
					vektor=FALSE;
				}	

					SimB *pSim=m_pSimDoc->GetPtr(type,Group, Name);
					OutVector *pVec=dynamic_cast<OutVector*>(pSim);
					if(pVec!=nullptr) selected=pVec->GetStoreFlag(Index-1);
					else selected=((OutSingle*)pSim)->GetStoreFlag();

			};

				int nsel;
				if(sheet==4) {
					nsel=m_pSimDoc->m_SelectModelFile.size();
					if(nsel>0)
							AddMenuItem(4011,"Clear the Selected ("+FUtil::STD_ItoAscii(nsel)+") PG file variables");
				}

				else if(sheet==5) {

					if(col==3&&m_pSimDoc->IsMultiSimulation()&&(m_pSimDoc->IsOutputBinFileValid()||selected==2)) {
						AddMenuItem(5001,"Plot Mean of All MultiRuns");
						//AddMenuItem(5002,"Estimate Mean of Accepted Runs");
					//AddMenuItem(5050,"Plot time serie of a specific Run");
					}
					if (m_pSimDoc->IsMultiSimulation() && (m_pSimDoc->IsOutputBinFileValid())) {
						AddMenuItem(8004, "Export Output to excel file for all runs");
						AddMenuItem(8005, "Export Output to excel file for accepted runs");

					}

					if(col>=7&&m_pSimDoc->IsMultiSimulation()) {
						
						SIMB simb=GetMultiSumPointer(row);
						Index=GetMultiSumIndex(simb,col);
						string name;
						if (simb.pBase != nullptr) {
							name = simb.pBase->GetName();

							if (simb.pBase->Is_Vector()) name += "(" + FUtil::STD_ItoAscii(simb.TabIndex) + ")";
						}
						else {
							name = "";
						}


						if(m_pSimDoc->m_SelectMultiSum.size()>0) {
							string str;
							str="Remove (";
							str.append(FUtil::STD_ItoAscii(m_pSimDoc->m_SelectMultiSum.size()));
							str.append(") Selected Variables");
							AddMenuItem(5020,str);
						}
						if(!m_pProp->IsFirstIndepSet())
							AddMenuItem(5018,"Set ("+name+") as first Independent variable");
						else {
							AddMenuItem(5018,"Select ("+name+") as New first Independent variable");
							AddMenuItem(5019,"DeSelect first Independent variable");
							if(!m_pProp->IsSecondIndepSet()) {
								AddMenuItem(5028,"Set ("+name+") as Second Independent variable");
							}
							else {
								AddMenuItem(5028,"Select ("+name+") as  New Second Independent variable");
								AddMenuItem(5029,"DeSelect Second Independent variable");
							}
						}
						switch(col) {
							case 7:	name+=" - Initial";break; 
							case 8:	name+=" - Final";break; 
							case 9:	name+=" - Min";break; 
							case 10:	name+=" - Max";break; 
							case 11: name+=" - Mean";break; 
							case 12: name+=" - Accumulated";break; 
						}
						AddMenuItem(2001,"Select ("+name+") to add criteria to accepted multi run simulations"); 
						AddMenuItem(8014, "Export all values of (" + name + ") to excel file");
						AddMenuItem(8015, "Export accepted values of (" + name + ") to excel file");


					}
					else {
						nsel=m_pSimDoc->m_SelectOut.size();
						if(nsel>0) 
							AddMenuItem(4011,"Clear the Selected ("+FUtil::STD_ItoAscii(nsel)+") Output variables");					
					}
				}
				else if(sheet==6) {

					int icountTot=0;
					F *pValFile;
					for(size_t i=0;i<MAXSIMVAL;i++) {
						pValFile=m_pSimDoc->ValidationFilePointer(i+1);
						icountTot+=pValFile->GetNumberOfSelectedVal();
					}

					if (m_pSimDoc->IsMultiSimulation() && (m_pSimDoc->IsOutputBinFileValid())) {
						AddMenuItem(8006, "Export Performance indicators to excel file for all runs");
						AddMenuItem(8007, "Export Performance indicators to excel file for accepted runs");
						AddMenuItem(8011, "Export Simulated & Measured Values to excel file for accepted runs");

					}

				
					if(col>=1&&col<=3&&row<=icountTot&&!m_ShowValidRes) {
						AddMenuItem(5001,"Plot Mean of All MultiRuns");
						//AddMenuItem(5002,"Plot Mean of Accepted Runs");
						if(!m_pSimDoc->m_PlotStorage.IsValidIndepPGVar())
							AddMenuItem(5010,"Set Measured Variables as independent Variable to all charts");
						else
							AddMenuItem(5011,"Set Time as independent Variable to all charts");
				
					}

					if(col>=6&&m_pSimDoc->IsMultiSimulation()) {
						int nselV=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
						string perf;
						switch (col) {
						case 7: perf = "r2"; break; case 8: perf = "Intercept"; break;
						case 9: perf = "Slope"; break;
						case 10: perf = "ME"; break;
						case 11: perf = "RMSE"; break;
						case 14: perf = "LogLi"; break;
						case 15: perf = "NSE"; break;
						}
						SIMB simb;
						simb.pBase = nullptr;
						string name;
						if(row<nselV&&row>=0) {
							simb=m_pSimDoc->m_ValidationData.GetSimPointer_TSV(row);
							Index=GetMultiValIndex(row, col);
							name = simb.pBase->GetName();

							AddMenuItem(7010, "Select All " + FUtil::STD_ItoAscii(nselV) + " Time Serie Performance (" + perf + ") indicators for plotting");

						}
						else if(row>=nselV) {
							AddMenuItem(7020, "Select All " + FUtil::STD_ItoAscii(m_pSimDoc->m_ValSum_Array.size()) + " Single Variable Performance (" + perf + ") indicators for plotting");
							string t;
							ChartUtil *pUtil;
							pUtil = MySingle::instance().GetChartUtilPointer();
							if (pUtil->m_AutoShiftFirstIndep)
								t = "Set first indep variable to common Variable";
							else
								t = "Shift to new first indep for each Sum Variable to plot";

							AddMenuItem(6071, t);
							simb=m_pSimDoc->m_ValidationData.GetSimPointer_SV(row-nselV);
							if (simb.pBase == nullptr) {
								name = m_pSimDoc->m_ValSum_Array[row - nselV].Name;

							}
							else {
								Index = GetMultiValSumIndex(row - nselV, col);
								name = simb.pBase->GetName();
							}
							if (m_pSimDoc->m_ValSum_Array[row - nselV].FlexSumVar) {
								if (col == 13)
									name = m_pSimDoc->m_ValSum_Array[row - nselV].IDVarName;


							}
							if (m_pSimDoc->m_ValSum_Array[row - nselV].ObsScaling == 1) {
								AddMenuItem(6040, "Re Scaling of Measured to Original Values");
								AddMenuItem(6042, "Re Scaling of Simulated to Measured Values");
							}
							else if (m_pSimDoc->m_ValSum_Array[row - nselV].ObsScaling == 2) {
								AddMenuItem(6040, "Scaling of Measured to Simulated");
								AddMenuItem(6042, "Re Scaling of Simulated to original Values");
							}
							else {
								AddMenuItem(6040, "Scaling of Measured to match simulated meanvalue");
								AddMenuItem(6042, "Scaling of Simulated to measured meanvalue");
							}

							AddMenuItem(6041, "Toggle scaling options for all Validation variables");
						}
						if(simb.pBase!=nullptr)
							if(simb.pBase->Is_Vector()) name+="("+FUtil::STD_ItoAscii(simb.TabIndex)+")";
						switch (col) {
							case 7:
								name+=" - r2";
								break;
							case 8:
								name+=" - A0";
								break;
							case 9:
								name+=" - A1";
								break;
							case 10:
								name+=" - ME";
								break;
							case 11:
								name+=" - RMSE";
								break;
							case 13:
								name += " -Measured";
								break;
							case 14:
								name+=" - LogLi";
								break;
							case 15:
								name+=" - NSE";
								break;

						}



						if(m_pSimDoc->m_SelectMultiVal.size()>0) {
							string str;
							str="Remove (";
							str.append(FUtil::STD_ItoAscii(m_pSimDoc->m_SelectMultiVal.size()));
							str.append(") Selected Variables");
							AddMenuItem(5020,str);
						}
						if(!m_pProp->IsFirstIndepSet())
							AddMenuItem(5018,"Set ("+name+") as first Independent variable");
						else {
							AddMenuItem(5018,"Select ("+name+") as New first Independent variable");
							AddMenuItem(5019,"DeSelect first Independent variable");
							if(!m_pProp->IsSecondIndepSet()) {
								AddMenuItem(5028,"Set ("+name+") as Second Independent variable");
							}
							else {
								AddMenuItem(5028,"Select ("+name+") as  New Second Independent variable");
								AddMenuItem(5029,"DeSelect Second Independent variable");
							}
						}

						nsel=m_pSimDoc->m_SelectMultiVal.size();
						if(nsel>0) 
							AddMenuItem(4011,"Clear the Selected ("+FUtil::STD_ItoAscii(nsel)+") Performance Indicators");

					}
					else {
						nsel=m_pSimDoc->m_SelectValTime.size();
						if(nsel>0) 
								AddMenuItem(4011,"Clear the Selected ("+FUtil::STD_ItoAscii(nsel)+") validation variables");
					}
					if (m_pSimDoc->IsMultiSimulation()) {
						AddMenuItem(5002, "Estimate Mean value for Ensemble");

						string t = FUtil::STD_ItoAscii(m_pSimDoc->m_ValsumFlexFiles.size());
						t += " Single Variable Validation files are valid. ";
						t += "Add New Flexible File";
						AddMenuItem(6020, t);
						if (m_pSimDoc->m_ValsumFlexFiles.size() > 0) {
							t = "Add Variable Validation from File (";
							t += m_pSimDoc->m_ValsumFlexFiles.back().FileName;
							t += ")";
							AddMenuItem(6021, t);
							AddMenuItem(6022, "ReEstimate Performance for Single Value Validation Variables");
						}
					}
				}
				else if(sheet==10) {
					if(m_ShowCoVarRes) {


					}
					else {
						nsel=m_pSimDoc->m_SelectMultiPar.size();
						bool NoParExport = false;
						if (col == 2 || col == 7) NoParExport=true;
						if (m_pSimDoc->IsMultiSimulation() && (m_pSimDoc->IsOutputBinFileValid())&&!NoParExport) {
							AddMenuItem(8008, "Export Parameter values to excel file for all runs");
							AddMenuItem(8009, "Export Parameter values to excel file for accepted runs");

						}
						if(col==2&&m_pSimDoc->IsMultiSimulation()) {
						
							SIMB simb=GetMultiRunParPointer(row);
							Index=GetMultiRunParIndex(row);

							GetCell(0, row, &cell);
							size_t idim = cell.GetIntNumber();
							size_t parindim= m_pSimDoc->MR_Get_NumberOfParametersWithinDim(idim);
							if (parindim > 1) {

								AddMenuItem(7001, "Select all " + FUtil::STD_ItoAscii(parindim) + " Parameters in this Dimension");

							}

	



							string name;
							if(simb.pBase==nullptr) {
								name="";

							}
							else {
								name=simb.pBase->GetName();

								if(simb.pBase->Is_Vector()) name+="("+FUtil::STD_ItoAscii(simb.TabIndex)+")";
							}

							if(m_pSimDoc->m_SelectMultiPar.size()>0) {
								string str;
								str="Remove (";
								str.append(FUtil::STD_ItoAscii(m_pSimDoc->m_SelectMultiPar.size()));
								str.append(") Selected Variables");
								AddMenuItem(5020,str);
							}
							if(!m_pProp->IsFirstIndepSet())
								AddMenuItem(5018,"Set ("+name+") as first Independent variable");
							else {
								AddMenuItem(5018,"Select ("+name+") as New first Independent variable");
								AddMenuItem(5019,"DeSelect first Independent variable");
								if(!m_pProp->IsSecondIndepSet()) {
									AddMenuItem(5028,"Set ("+name+") as Second Independent variable");
								}
								else {
									AddMenuItem(5028,"Select ("+name+") as  New Second Independent variable");
									AddMenuItem(5029,"DeSelect Second Independent variable");
								}
							}
							
							if (m_pProp->m_ConstrainProp.size()>0) {
								string txt = FUtil::STD_ItoAscii(m_pProp->m_ConstrainProp.size());
								string txt2= FUtil::STD_ItoAscii(m_pProp->m_ConstrainProp.size()+1);
								AddMenuItem(5050, "Remove the "+txt+" constrains based on parameters");
								AddMenuItem(5051, "Add Constrain no: "+txt2+" based on " + name + " parameter");

							}
							else AddMenuItem(5051, "Add Constrain based on " + name + " parameter");

						}
						else if (col == 7) {
							if (m_pSimDoc->m_MStorage.m_AcceptedIdString.size() > 1) {
								string  ens = FUtil::STD_ItoAscii(m_pSimDoc->m_MStorage.m_ActualEnsemble + 1);
								if (m_pSimDoc->m_MStorage.m_Valid_TSV[m_pSimDoc->m_MStorage.m_ActualEnsemble].tobe_Plotted)
									AddMenuItem(8050, "Exclude " +ens+ " Ensemble from Plotting");
								else
									AddMenuItem(8050, "Include " +ens+ " Ensemble to Plotting");
							}
						}


						if(nsel>0) 
							AddMenuItem(4011,"Clear the Selected ("+FUtil::STD_ItoAscii(nsel)+") statistics for multi run parameters");	
					}
				}


				else if(sheet==11) {
					nsel=m_pSimDoc->m_SelectPGFile.size();
					size_t IndexSource=m_pPGArchive->GetIndex(m_pDoc);
					CPG *pPG=m_pPGArchive->GetPGFile(IndexSource);
					if (pPG != nullptr) {
						string name;
						name = pPG->GetVarName(row - 1);
						if (!m_pProp->IsFirstIndepSet())
							AddMenuItem(5018, "Set (" + name + ") as first Independent variable");
						else {
							AddMenuItem(5018, "Select (" + name + ") as New first Independent variable");
							AddMenuItem(5019, "DeSelect first Independent variable");
							if (!m_pProp->IsSecondIndepSet()) {
								AddMenuItem(5028, "Set (" + name + ") as Second Independent variable");
							}
							else {
								AddMenuItem(5028, "Select (" + name + ") as  New Second Independent variable");
								AddMenuItem(5029, "DeSelect Second Independent variable");
							}
						}

					}

				}
				/*
				AddMenuItem(4006, "Add from ("+FUtil::STD_ItoAscii(m_pProp->m_NumChartX)+") to ("+FUtil::STD_ItoAscii(m_pProp->m_NumChartX+1)+") horizontal charts");
				if(m_pProp->m_NumChartX>1)
							AddMenuItem(4007, "Reduce from ("+FUtil::STD_ItoAscii(m_pProp->m_NumChartX)+") to ("+FUtil::STD_ItoAscii(m_pProp->m_NumChartX-1)+") horizontal charts");
				AddMenuItem(4008, "Add from ("+FUtil::STD_ItoAscii(m_pProp->m_NumChartY)+") to ("+FUtil::STD_ItoAscii(m_pProp->m_NumChartY+1)+") vertical charts");
				if(m_pProp->m_NumChartY>1)
							AddMenuItem(4009, "Reduce from ("+FUtil::STD_ItoAscii(m_pProp->m_NumChartY)+") to ("+FUtil::STD_ItoAscii(m_pProp->m_NumChartY-1)+") verticaltal charts");
				*/
				if(m_pSimDoc->m_SelectedDocuments.size()>1)
					AddMenuItem(3400,"Un link connection to other open documents");
				else if(UpdateDocumentLink()) {
					string NumStr;
					m_NumOpenDoc=m_pSimDoc->m_SelectedDocuments.size();
					NumStr=FUtil::STD_ItoAscii(m_pSimDoc->m_SelectedDocuments.size());
					AddMenuItem(3401,"Make connection to all open documents ("+NumStr+")");
					m_pSimDoc->m_SelectedDocuments.clear();

				}
}
bool GridSimDoc::UpdateDocumentLink() {
		
	Doc* dg_pDoc;
	m_pSimDoc->m_SelectedDocuments.clear();
	
	m_pSimDoc->m_SelectedDocuments.push_back(m_pSimDoc);
	m_pSimDoc->m_Selected_All_Documents.clear();
	m_pSimDoc->m_Selected_All_Documents.push_back(m_pSimDoc);


		for(size_t i=0; i<m_pSimArchive->GetNumSimDoc(); i++) {	
				dg_pDoc =m_pSimArchive->GetSimDocPointer(i);
				if(dg_pDoc->IsFinishedSimulation()){
					if(dg_pDoc!=m_pSimDoc/*&&!dg_pDoc->IsMultiSimulation()*/) {
						m_pSimDoc->m_SelectedDocuments.push_back(dg_pDoc);
						m_pSimDoc->m_Selected_All_Documents.push_back(dg_pDoc);
					}			
					else if(dg_pDoc!=m_pSimDoc)
						m_pSimDoc->m_Selected_All_Documents.push_back(dg_pDoc);
				}
		}

	if(m_pSimDoc->m_SelectedDocuments.size()>1) {
		m_pSimDoc->SetDocLinkedNumber(m_pSimDoc->m_SelectedDocuments.size());
		m_NumOpenDoc=m_pSimDoc->m_SelectedDocuments.size();	
		return TRUE;
	
	}
	m_NumOpenDoc=1;
	m_pSimDoc->SetDocLinkedNumber(m_pSimDoc->m_SelectedDocuments.size());
	return FALSE;

}
PLOTv GridSimDoc::MakeOutPutChartInfo(size_t i, size_t i_doc) {
	PLOTv out;
	out.pBase=m_pSimDoc->m_SelectOut[i].pBase;
	out.TabIndex=m_pSimDoc->m_SelectOut[i].LIndex-1;
	out.EnsembleType=m_pSimDoc->m_SelectOut[i].MeanIndicator;
	if(out.EnsembleType==ENSEMBLE_TYPE::NO) {
		out.pPG=&m_pSimDoc->m_PG_OutPutFile;
		string filename=m_pSimDoc->GetOutputFileName();
		out.pPG->SetFileName(filename);

		out.PGFileName=out.pPG->GetFileName();
	}
	else if(out.EnsembleType==ENSEMBLE_TYPE::ALL)
		out.pPG=m_pSimDoc->m_PlotStorage.m_PGALL_Out;
	else if(out.EnsembleType== ENSEMBLE_TYPE::ACCEPTED)
		out.pPG=m_pSimDoc->m_PlotStorage.m_PGALL_Out;
	if(m_pSimDoc->m_SelectedDocuments.size()<=1)
		out.pSimDoc=m_pSimDoc;
	else if(i_doc<m_pSimDoc->m_SelectedDocuments.size()) {
		out.pSimDoc=m_pSimDoc->m_SelectedDocuments[i_doc];
		out.pBase=out.pSimDoc->GetPtr(out.pBase->GetSim(),out.pBase->GetName());
	}
	else
		out.pSimDoc=m_pSimDoc;
	if(out.pBase->Is_Vector()) {
			if(m_pSimDoc->IsMultiSimulation())
				out.PGFileIndex=((OutVector*)out.pBase)->GetPgFileIndex(out.TabIndex);
			else
				out.PGFileIndex=((OutVector*)out.pBase)->GetPgSingleFileIndex(out.TabIndex);
	}
	else {
			if(m_pSimDoc->IsMultiSimulation())
				out.PGFileIndex=((OutSingle*)out.pBase)->GetPgFileIndex();
			else
				out.PGFileIndex=((OutSingle*)out.pBase)->GetPgSingleFileIndex();
	}
	out.Type=out.pBase->GetType();
	out.Index=m_pSimDoc->m_SelectOut[i].LIndex-1;
	out.PGSection=0;
	return out;
}
SIMB GridSimDoc::GetMultiSumPointer(int row) {
	CUGCell cell, cell1;
	SIMB out;
	 int LIndex;
			string Name, Group, Type;
			GetCell(3,row, &cell);
			if(m_pSimDoc->IsFinishedSimulation())
				Name=cell.GetStdLabelText();
			else
				Name=cell.GetStdText();
			int ParV=cell.GetParam();
			auto pos1=Name.rfind('(');
			auto pos2=Name.rfind(')');
			int PGindex;


			if (pos2>pos1&&pos2==Name.size()-1) {
					string t=Name.substr(pos1+1,pos2-pos1-1);
					LIndex=MFC_Util::AtoInt(t);
					if(LIndex>0)
						Name=Name.substr(0,pos1);
					else
						LIndex=1;
			}
			else
				LIndex=0;

			GetCell(1,row, &cell1);
			cell1.GetStdText(&Group);
			GetCell(0,row, &cell1);
			cell1.GetStdText(&Type);
			
			out.pBase=m_pSimDoc->GetPtr(Type, Group, Name);
			out.TabIndex=LIndex;


	return out;

}
SIMB GridSimDoc::GetMultiRunParPointer(int row) {
	CUGCell cell, cell1;
	SIMB out;
	string Name, Group, Type;
	 int LIndex;
	 int DimValue;
	 int DimCount;
	 GetCell(-1,row,&cell);
	 DimCount=int(cell.GetNumber());

	 GetCell(0,row,&cell);
	 DimValue=int(cell.GetNumber());
	 GetCell(1,row, &cell);
	 Group=cell.GetStdText();
	 if(row<m_pSimDoc->m_P_MR_Array.size())
		out=m_pSimDoc->m_P_MR_Array[row];
	 else {
		out.pBase=nullptr;
		out.TabIndex=0;
	 }
	return out;

}
int GridSimDoc::GetMultiRunParIndex(int row) {


	return row;
}
int GridSimDoc::GetMultiSumIndex(SIMB simb, int col) {
		if(simb.pBase->Is_Vector())
			return ((OutVector*)simb.pBase)->GetSumVarIndex(simb.TabIndex-1, col);
			
		else 
			return ((OutSingle*)simb.pBase)->GetSumVarIndex(col);

}

int GridSimDoc::GetMultiValSumIndex(int row, int col) {
	int nselV=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
	int nvar;
	if(nselV==0)
		nvar=row+1;
	else
		nvar=row-nselV;
	return nvar - 1;	
}
SUM_PERFORMANCE GridSimDoc::GetMultiValSumType(int row, int col) {

	switch (col) {
	case 7:
		return SUM_PERFORMANCE::SUM_R2;
	case 8:
		return SUM_PERFORMANCE::SUM_INTERCEPT;
	case 9:
		return SUM_PERFORMANCE::SUM_SLOPE;
	case 10:
		return SUM_PERFORMANCE::SUM_ME;
	case 11:
		return SUM_PERFORMANCE::SUM_RMSE;
	case 12:
		return SUM_PERFORMANCE::SUM_SIM;
	case 13:
		return SUM_PERFORMANCE::SUM_OBS;
	case 14:
		return SUM_PERFORMANCE::SUM_LOGLI;
	case 15:
		return SUM_PERFORMANCE::SUM_NSE;
	}
	return SUM_PERFORMANCE::SUM_NOVALUE;

}
int GridSimDoc::GetMultiValIndex(int row, int col) {
	
	int nselV=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
/*	if (m_pSimDoc->m_MStorage.GetCyclicNumber() > 1)
		nselV/= m_pSimDoc->m_MStorage.GetCyclicNumber();*/

	if(col<12)
		return row+(col-7)*nselV;
	else if(col==14) 
		return row+5*nselV;
	else if(col==15)
		return row+6*nselV;
	return -1;


}


void GridSimDoc::MakeMultiRunPriorMenu(int col, int row) {
	CUGCell cell;
	long ParV;
	if(GetNumberRows()>0) {
		GetCell(-1,row,&cell);
		int ip=int(cell.GetNumber());
		GetCell(0,row,&cell);
		int idim=int(cell.GetNumber());
					
		if(col==6)
		AddMenuItem(1000,"Set all start values to actual values in current sim document");

		if(m_pSimDoc->MR_GetMethod(idim,ip)==4)
			AddMenuItem(1005,"Edit/show all Tables");
		else if (m_pSimDoc->MR_GetMethod(idim,ip)==2||m_pSimDoc->MR_GetMethod(idim,ip)==3) {
			AddMenuItem(1015,"Change from General GLUE to Bayesian Calibration");
			AddMenuItem(1016,"Change from General GLUE to Table method");
		}
		else if (m_pSimDoc->MR_GetMethod(idim,ip)==9||m_pSimDoc->MR_GetMethod(idim,ip)==11) {
			AddMenuItem(1025,"Change from Bayesian Calibration to GLUE");
		}

		SimB *pPar=m_pSimDoc->MR_Get_pPar(idim,ip);

		string PName;
		if(pPar!=nullptr) {
			PName=pPar->GetName();
			int TabIndex=m_pSimDoc->MR_GetTabIndex(idim,ip);
			if(TabIndex>=0)
				PName+="("+FUtil::STD_ItoAscii(TabIndex+1)+")";
		}
		else
			PName="-";

		if(m_pSimDoc->MR_UseBayesianCoVar)
			AddMenuItem(1001,"Make New Calibration");
		else
			AddMenuItem(1001,"Make Use of Calibrated Values");
		AddMenuItem(1002,"Remove All Items from MultiRun list");
	
		AddMenuItem(1003,"Remove ("+PName+ " )from MultiRun list");
		AddMenuItem(1004,"Add New Dimension");
		AddMenuItem(2000, "Export CSV File with All Enabled Parameters");
		AddMenuItem(2001, "Export CSV File with All not Default Parameters");
		AddMenuItem(2002, "Export CSV File with selected Parameters");
		for (size_t i = 0; i < m_pSimDoc->m_FCSV_Array.size(); i++)
		{
			if (m_pSimDoc->m_FCSV_Array[i]->m_Complete) {
				string strfile;
				strfile = m_pSimDoc->m_FCSV_Array[i]->GetStrValue();
				AddMenuItem(2005 + i, "Incorporate values from CSV File: " + strfile + " in current selections of Parameters");
			};
		};
		AddMenuItem(2010, "Incorporate values from CSV File");

	}



}