#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include "..\..\Util.h"
#include "..\..\NewBase\FileScanner.h"
#include "PGCommands.h"
#include "..\ChartProperties\ChartCug.h"
#include "..\..\MainFrm.h"
#include "..\..\PG\PG_FILEDLG.h"
#include "..\..\PG\NewPGVariables.h"
#include "..\..\NetCDF\NetCDFTransfer.h"

#include "..\..\PG\CreatePG.h"
#include "..\ColourPickerDlg.h"
#include "..\..\MultiRun\MultiRunTable.h"
#include "..\..\PG\CalculateVariable.h"

PGCommands::PGCommands(void)
{
	m_ShowMean = 0;
	//m_pChartCug=MySingle::instance().GetChartCugPointer();
}
PGCommands::~PGCommands(void)
{
}
/////////////////////////////////////////////////////////////////////////////
//	OnMenuCommand
//		This notification is called when the user has selected a menu item
//		in the pop-up menu.
//	Params:
//		col, row - the cell coordinates of where the menu originated from
//		section - identify for which portion of the gird the menu is for.
//				  possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//						UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
//		item - ID of the menu item selected
//	Return:
//		<none>
void PGCommands::OnMenuCommand(int col,long row,int section,int item)
{

	int sheet;
	CUGCell cell,cell1, cell2; 
	long ParV;

	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(section);
	UNREFERENCED_PARAMETER(item);


	sheet=GetSheetNumber();

	
//****** The user has selected the 'Cut' option
if (item == 1996)
  CutSelected();

//****** The user has selected the 'Copy' option
if (item == 1997) {
	CopySelected();
	return;	
}

//****** The user has selected the 'Paste' option
if (item==1998) {
    Paste(); 
	return;
}

if(item==3001&&sheet==1) {
	CPG	*pCPG=(CPG*)m_pPGArchive->GetPGFile(m_IndexSource);
	if(pCPG!=nullptr) {
		m_PGCharts.CreateZoomPlotWindow(pCPG);
		return;
	}

}

if(MySingle::instance().GetCugPointer()->ManagePlotWindows(item,11)) {
		OnSheetSetup(sheet);
		return;
}
if (sheet < 9) {

	if ((item > 5010 && item < 5040) || (item > 6010 && item < 6040)) {
		if (SetNewIndependenceVar(item, col, row)) {
			OnSheetSetup(sheet);
			m_pUtil->m_FileSave.OnReDraw(true, m_pSimDoc);
			return;
		}



	}
	if (item == 1010) {
		if (m_RepFirst)
			m_RepFirst = false;
		else
			m_RepFirst = true;
		OnSheetSetup(1);
		return;
	}
	else if (item == 3001) {

		//			Create New PG File
		FileCreate();


	}

	else if (item == 3000 || item == 3004) {

		FileOpen();
	};
	if (item == 3002) {
		RemovePGFile(col, row);

	}
	else if (item == 3003 || item == 3004) {
		OpenEditPGFile(col, row, item);
		Make_PGFileSheet();
		return;

	}
	else if (item == 3006) {
		/*	GetCell(col,row, &cell);
			ParV=cell.GetParam();
			int index=ParV-1000;
			CPG *pPG=(CPG*)m_pPGArchive->m_PGFileArray[index].pFile;
			CPG_FILEDLG dlg(pPG, FALSE);
			if(dlg.DoModal()==IDOK) {


			}
			*/
	}
	else if (item == 3007) {
		/*	GetCell(col,row, &cell);
			ParV=cell.GetParam();
			int index=ParV-1000;
			CPG *pPG=(CPG*)m_pPGArchive->m_PGFileArray[index].pFile;
			CPG_FILEDLG dlg(pPG, TRUE);
			if(dlg.DoModal()==IDOK) {


			}
			*/
	}
	else if (item == 3008) {
		AddPGFile();
	}
	else if (item == 3009) {
		MixPGFilesWithoutInterpolations();

	}
	else if (item == 3010) {
		MixPGFilesWithoutInterpolations();

	}
	else if (item == 3013) {
		ExportfromPGFileText(col, row);
	}
	else if (item == 3014) {
		ExportfromPGFileExcel(col, row);
	}
	else if (item == 3015) {
		CreateNewPGFileWithNewVar(col, row);



	}
	else if (item == 3016) {
		CreateNewPGFileWithRepetition();
	}

	else if (item == 3101) {
		ImportFromTextAndMixToPGFile();


	}
	else if (item == 3025) {
		IncludeCoordinatesFromNOAA(col, row);


	}
	else if (item == 3026) {
		MergeSimilarStations(col, row);
	}
	else if (item == 3027) {
		InterpolateBasedOnRegressions(col, row);
	}
	else if (item == 3028) {
		CreateNewFileForNewPosition();

	}
	else if (item == 3029) {

		CreateNewFileForNewPosition();

	}
	else if (item == 3031) { //Linear Interpolation
		ReplaceMissingDataByLinearInterpolation();
	}
	else if (item == 3032) { //Random filling of data
		ReplaceMissingDataByRandom();
	}
	else if (item == 3033) {

		ImportFromNetCDF_File();
	}
	else if (item == 3034) {
		SplitIntoNoRepetionsFiles(col, row);



	}
	else if (item == 3050) {
		if (m_ShowSubResults)
			m_ShowSubResults = false;
		else
			m_ShowSubResults = true;
		if (sheet == 4)
			Make_DataSubstitution();
		else if (sheet == 5)
			Make_DataAggregation();
		else if (sheet == 6)
			Make_ChangePeriodSheet();
		return;

	}
	else if (item == 3051) {
		if (m_ShowSubResults)
			m_ShowSubResults = false;
		else
			m_ShowSubResults = true;

		if (sheet == 5) {
			AddPGFile(m_TimeAggregate.m_DestFileName.c_str());
			OnTabSelected(0);

		}
		else if (sheet == 6) {
			AddPGFile(m_ChangePeriod.m_DestFileName.c_str());
			OnTabSelected(0);
		}
		return;

	}
	else if (item == 4444) {
		size_t index;
		GetCell(col, row, &cell);
		ParV = cell.GetParam();
		GetCell(-2, row, &cell);
		index = int(cell.GetNumber());
		if (index > m_pPGArchive->m_PGFileArray.size())
			index = m_pPGArchive->m_PGFileArray.size();
		CPG *pPG = (CPG*)m_pPGArchive->m_PGFileArray[index - 1].pFile;
		if (ExportVariables(pPG)) {

		}
		;
		return;

	}
	else if (item == 7001||item == 7002) {
		int i = 0; //Modify PG file options
		CPG	*pCPG = (CPG*)m_pPGArchive->GetPGFile(m_IndexSource);
		if (pCPG != nullptr) {
			pCPG->ReOpen();
			string newname = pCPG->GetFileName();
			pCPG->SetRepititionSequence(0);
			if (item == 7002) {
				auto irecmiss = pCPG->RemoveMissingRecords();
				
				string nn;
				nn = FUtil::STD_ItoAscii(irecmiss);
				MFC_Util::MessageBox(nn + " Records are removed from file: " + newname);
				pCPG->SaveAs(newname);
				pCPG->CheckMissingNumbers();

			}
			else {
				if (pCPG->pg.Shape.NumRepetitions > 1) {
					for (size_t i = 0; i < pCPG->pg.Shape.NumRepetitions; i++) {
						if (pCPG->ReCalculateMinMax(i));
						else {
							pCPG->ReOpen();
							pCPG->ReCalculateMinMax(i);
						}
					}
				}
				else {
					if (pCPG->ReCalculateMinMax());
					else {
						pCPG->ReOpen();
						pCPG->ReCalculateMinMax();
					}
				}
				//pCPG->SaveAs(newname); File 
			}
		}


	}
}
else {
	if (section == UG_GRID) {

		if (item == 1000) {
			CutSelected();
			return;
			//m_Changed = true;
		}
		else if (item == 2000) {
			CopySelected();
			return;

		}
		else if (item == 3000) {
			Paste();
			if (sheet == 10) {
				m_Changed_Description = true;

				OnWriteGridDescriptionToFile(m_pPG);

			}
			else m_Changed_Data = true;
			return;
			//m_Changed= true;
		}
		else if (item == 1500) {

			m_pPG->SetVarFunctionType(0);
			m_pPG->SetVarFunction(1);
			m_pPG->SetTimeConvUnit(1);
			m_pPG->SetTimeConvBase(m_pPG->GetLongTimeStart());
			CCalculateVariable dlg;
			dlg.Init(m_pPG);
			if (dlg.DoModal() == IDOK) {
				if (OnNewCalculateVar())

					MFC_Util::MessageBox("Calculation of new Date/Time successfully done");
			}


		}
		else if (item == 1501) {
			m_pPG->SetVarFunctionType(0);
			m_pPG->SetVarFunction(2);
			m_pPG->SetTimeConvUnit(1);
			m_pPG->SetTimeConvBase(m_pPG->GetLongTimeStart());
			CCalculateVariable dlg;
			dlg.Init(m_pPG);
			if (dlg.DoModal() == IDOK) {
				if (OnNewCalculateVar())

					MFC_Util::MessageBox("Calculation of new Date/Time successfully done");
			}


		}
		else if (item == 1502) {
			m_pPG->SetVarFunctionType(0);
			m_pPG->SetVarFunction(3);
			m_pPG->SetCalcVar(1);
			m_pPG->SetTimeConvUnit(1);
			m_pPG->SetTimeConvBase(m_pPG->GetLongTimeStart());
			CCalculateVariable dlg;
			dlg.Init(m_pPG);
			if (dlg.DoModal() == IDOK) {
				if (OnNewCalculateVar())

					MFC_Util::MessageBox("Calculation of new Date/Time successfully done");
			}


		}
		else if (item == 1525) {
			m_pPG->SetVarFunctionType(0);
			m_pPG->SetVarFunction(4);
			m_pPG->SetTimeConvBase(m_pPG->GetLongTimeStart());

			if (OnNewCalculateVar()) {
				MFC_Util::MessageBox("New Dates adjusted time successfully done");
			}
		}
		else if (item == 1560) {
			CPG *pPG;
			pPG = (CPG*)m_pPG;
			pPG->SetAllToBeSaved(TRUE);
			for (int i = 0; i<pPG->GetNumVariables(); i++) {
				GetCell(0, i, &cell);
				cell.SetBool(TRUE);
				SetCell(0, i, &cell);
			}
			RedrawAll();

		}
		else if (item == 1561) {
			CPG *pPG;
			pPG = (CPG*)m_pPG;
			pPG->SetToBeSavedBelow(row, TRUE);
			for (int i = row; i<pPG->GetNumVariables(); i++) {
				GetCell(0, i, &cell);
				cell.SetBool(TRUE);
				SetCell(0, i, &cell);
			}
			RedrawAll();


		}
		else if (item == 1562) {
			CPG *pPG;
			pPG = (CPG*)m_pPG;
			pPG->SetAllToBeSaved(FALSE);
			for (int i = 0; i<pPG->GetNumVariables(); i++) {
				GetCell(0, i, &cell);
				cell.SetBool(FALSE);
				SetCell(0, i, &cell);
			}
			RedrawAll();


		}
		else if (item == 1563) {
			CPG *pPG;
			pPG = (CPG*)m_pPG;
			pPG->SetToBeSavedBelow(row, FALSE);
			for (int i = row; i<pPG->GetNumVariables(); i++) {
				GetCell(0, i, &cell);
				cell.SetBool(FALSE);
				SetCell(0, i, &cell);
			}
			RedrawAll();


		}
		else if (item == 1660) { // Output Sequence Order
			CPG *pPG;
			pPG = (CPG*)m_pPG;
			if (pPG->GetViewOrder())
				pPG->SetViewOrder(false);
			else
				pPG->SetViewOrder(true);
			Make_DataHeader();
			OnGetFileDataFillGrid(pPG);

		}
		else if (item == 1661) { // Enable/Disable viewed repetition data
			CPG *pPG;
			pPG = (CPG*)m_pPG;
			size_t index;
			bool enable = pPG->GetToBeViewed(size_t(row));
			for (size_t rep = 0; rep<pPG->GetNumRepititions(); rep++) {
				index = row%pPG->GetNumVariables() + rep*pPG->GetNumVariables();
				GetCell(1, index, &cell);
				if (enable) {
					pPG->SetToBeViewed(index, false);
					cell.SetBool(FALSE);

				}
				else {
					pPG->SetToBeViewed(index, true);
					cell.SetBool(TRUE);
				}
				SetCell(1, index, &cell);
				RedrawCell(1, index);
			}
			pPG->UpdateViewOrder();

			Make_DataHeader();
			OnGetFileDataFillGrid(pPG);
		}
		else if (item == 1662) { // Enable/Disable viewed repetition data
			CPG *pPG;
			pPG = (CPG*)m_pPG;
			size_t index = 0;
			bool enable = pPG->GetToBeViewed(size_t(row));
			for (size_t nvar = 0; nvar<pPG->GetNumVariables(); nvar++) {
				for (size_t rep = 0; rep<pPG->GetNumRepititions(); rep++) {
					GetCell(1, index, &cell);
					if (enable) {
						pPG->SetToBeViewed(index, false);
						cell.SetBool(FALSE);

					}
					else {
						pPG->SetToBeViewed(index, true);
						cell.SetBool(TRUE);
					}
					SetCell(1, index, &cell);
					RedrawCell(1, index);
					index++;
				}
			}
			pPG->UpdateViewOrder();
			Make_DataHeader();
			OnGetFileDataFillGrid(pPG);

		}
		else if (item == 1663) {
			CPG *pPG = (CPG*)m_pPG;
			pPG->UpdateViewOrder();
			Make_DataHeader();
			OnGetFileDataFillGrid(pPG);

		}
		else if (item == 1700) {
			if (m_Data) {
				CPG *pPG;
				pPG = (CPG*)m_pPG;
				pPG->RemoveVariable(col - 1);
			}
		}
		else if (item == 1701) {
			if (m_Data) {
				InsertCol(col);
				((PGCug*)m_pCUG)->InsertRow(col - 1);
			}
			else {
				InsertRow(row);
				((PGCug*)m_pCUG)->InsertCol(row + 1);
			}

		}
		else if (item == 1510) {
			m_ShowMean = 1;
			int num_sheets = GetNumberSheets();
			for (long i = 0; i<num_sheets; i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

			//******* Add New Tabs 
			AddTab("Actual Data", 0);
			AddTab("Yearly Data", 1);
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if (item == 1511) {
			m_ShowMean = 2;
			int num_sheets = GetNumberSheets();
			for (long i = 0; i<num_sheets; i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

			//******* Add New Tabs 
			AddTab("Actual Data", 0);
			AddTab("Montly Data", 1);
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if (item == 1512) {
			m_ShowMean = 3;
			int num_sheets = GetNumberSheets();
			for (long i = 0; i<num_sheets; i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

			//******* Add New Tabs 
			AddTab("Actual Data", 0);
			AddTab("Daily Data", 1);
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if (item == 1513) {
			m_ShowMean = 4;
			int num_sheets = GetNumberSheets();
			for (long i = 0; i<num_sheets; i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

			//******* Add New Tabs 
			AddTab("Actual Data", 0);
			AddTab("Hourly Data", 1);
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if (item == 1514) {
			m_ShowMean = 5;
			int num_sheets = GetNumberSheets();
			for (long i = 0; i<num_sheets; i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

			//******* Add New Tabs 
			AddTab("Actual Data", 0);
			AddTab("Minute Data", 1);
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if (item == 1515) {
			m_pPG->SetVarFunctionType(0);
			m_pPG->SetVarFunction(6);
			if (m_pPG->CalcVarTimeDerivate()) {
				MFC_Util::MessageBox("Calculation of New Derivate with adjusted time successfully done");
			}
		}
		else if (item >= 1702&&item<=1705) {
			CPG *pPG = (CPG*)m_pPG;

			bool newvar = true;
			bool allrep = false;
			size_t SeqNo;
			size_t ivar = pPG->GetViewVariable(col - 1);
			size_t irep = pPG->GetViewRepetition(col - 1);
			SeqNo = ivar + pPG->GetNumVariables()*irep;
			if (item == 1703 || item == 1705) newvar = false;
			if (item == 1704 || item == 1705) allrep = true;

			Make_PG_Calc(newvar, allrep, SeqNo);
			m_pUtil->m_FileSave.OnReDraw(true, m_pSimDoc);


		}

	}
	else if (section == UG_SIDEHEADING) {
		if (item == 1500) {

			m_pPG->SetVarFunctionType(0);
			m_pPG->SetVarFunction(1);
			m_pPG->SetTimeConvUnit(1);
			m_pPG->SetTimeConvBase(m_pPG->GetLongTimeStart());
			CCalculateVariable dlg;
			dlg.Init(m_pPG);
			if (dlg.DoModal() == IDOK) {
				if (OnNewCalculateVar())

					MFC_Util::MessageBox("Calculation of new Date/Time successfully done");
			}
		}
		else if (item == 1501) {
			m_pPG->SetVarFunctionType(0);
			m_pPG->SetVarFunction(2);
			m_pPG->SetTimeConvUnit(1);
			m_pPG->SetTimeConvBase(m_pPG->GetLongTimeStart());
			CCalculateVariable dlg;
			dlg.Init(m_pPG);
			if (dlg.DoModal() == IDOK) {
				if (OnNewCalculateVar())

					MFC_Util::MessageBox("Calculation of new Date/Time successfully done");
			}


		}
		else if (item == 1502) {
			m_pPG->SetVarFunctionType(0);
			m_pPG->SetVarFunction(3);
			m_pPG->SetCalcVar(1);
			m_pPG->SetTimeConvUnit(1);
			m_pPG->SetTimeConvBase(m_pPG->GetLongTimeStart());
			CCalculateVariable dlg;
			dlg.Init(m_pPG);
			if (dlg.DoModal() == IDOK) {
				if (OnNewCalculateVar())

					MFC_Util::MessageBox("Calculation of new Date/Time successfully done");
			}


		}
		else if (item == 1510) {
			m_ShowMean = 1;
			int num_sheets = GetNumberSheets();
			for (long i = 0; i<num_sheets; i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

			//******* Add New Tabs 
			AddTab("Actual Data", 0);
			AddTab("Yearly Data", 1);
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if (item == 1511) {
			m_ShowMean = 2;
			int num_sheets = GetNumberSheets();
			for (long i = 0; i<num_sheets; i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

			//******* Add New Tabs 
			AddTab("Actual Data", 0);
			AddTab("Montly Data", 1);
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if (item == 1512) {
			m_ShowMean = 3;
			int num_sheets = GetNumberSheets();
			for (long i = 0; i<num_sheets; i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

			//******* Add New Tabs 
			AddTab("Actual Data", 0);
			AddTab("Daily Data", 1);
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if (item == 1513) {
			m_ShowMean = 4;
			int num_sheets = GetNumberSheets();
			for (long i = 0; i<num_sheets; i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

			//******* Add New Tabs 
			AddTab("Actual Data", 0);
			AddTab("Hourly Data", 1);
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if (item == 1514) {
			m_ShowMean = 5;
			int num_sheets = GetNumberSheets();
			for (long i = 0; i<num_sheets; i++) {
				DeleteTab(i);
			}
			SetDefColWidth(130);

			//******* Add New Tabs 
			AddTab("Actual Data", 0);
			AddTab("Minute Data", 1);
			OnSheetSetup(1);
			SetCurrentTab(1);
		}
		else if (item == 1515) {
			m_pPG->SetVarFunctionType(0);
			m_pPG->SetVarFunction(6);
			if (m_pPG->CalcVarTimeDerivate()) {
				MFC_Util::MessageBox("Calculation of New Derivate with adjusted time successfully done");
			}
		}
		else if (item == 1700) {
			if (m_Data) {
				CPG *pPG;
				pPG = (CPG*)m_pPG;
				pPG->RemoveVariable(row);

			}
		}
		else if (item == 1701) {
			if (m_Data) {
				CPG *pPG;
				pPG = (CPG*)m_pPG;
				pPG->AddNewVariable(row);
			}

		}
		else if (item >= 1702) {
			CPG *pPG = (CPG*)m_pPG;
			bool newvar = true;
			bool allrep = false;
			size_t SeqNo = row + 1;
			if (item == 1703 || item == 1705) newvar = false;
			if (item == 1704 || item == 1705) allrep = true;
			Make_PG_Calc(newvar, allrep, SeqNo);

			m_pUtil->m_FileSave.OnReDraw(true, m_pSimDoc);

		}


	}
	if (item == 1700 || item == 1701 || (item >= 1702 && item<2000) || (item >= 1500 && item <= 1502)) {
		if (m_Data)
			ReDisplay();
		else if (item >= 1700)
			ReDisplay();


	}



}
			
	EnableUpdate(FALSE); 
	while(GetNumberRows()>0)
		DeleteRow(0);
	EnableUpdate(TRUE); 

	OnSheetSetup(sheet);



	
}

void PGCommands::FileCreate()
{
	CCreatePG dlg(m_pDoc, nullptr);
	int ans=dlg.DoModal();

	if(ans==IDOK) {
			CPG *pPG= new CPG;
			pPG->Open(dlg.m_PG_FileName);
			if(pPG->IsOpen()) {
				PGFile	Fil;
				Fil.pFile=pPG;
				Fil.Visible=false;
				m_pApp->OpenDocumentFile(CString(dlg.m_PG_FileName.c_str()));
			}
	}
}
void PGCommands::FileOpen()
{
	
		//Find PG file
/*			int fileDlgResult;
			CString pszFileName ;
			LPCTSTR lpszDefExt="bin";
			LPCTSTR lpszFilter;
//The next line is only neccessary as long as the PGP does not handle long filenames!
//PEJ Excluded info box
			static char BASED_CODE szFilter[] = "PG Bin file (*.BIN)|*.BIN||";
			CFileDialog	BinFileDlg(TRUE,lpszDefExt="bin",lpszFilter = "*.BIN",OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,szFilter);*/


			FILEDLG FDlg = MFC_Util::GetFileDialog("bin", "", "PG Bin file (*.BIN)|*.BIN||");
			//If the reading of the file was successful

			if (FDlg.opengood) {
				AddPGFile(FDlg.filename);
		}
}
void PGCommands::AddPGFile(string FileName )
{
				CPG *pPG= new CPG;
				if(pPG->Open(FileName)) {
					if(pPG->IsOpen()) {
						PGFile	Fil;
						Fil.pFile=pPG;
						Fil.Visible=false;
						m_pApp->OpenDocumentFile(CString(FileName.c_str()));
						//m_pPGArchive->m_PGFileArray.push_back(Fil);
					}
					pPG->CloseFile();
				};
				delete pPG;
}
bool PGCommands::ExportVariables(CPG *pCPG) {
	CPG pPG_Dest;
	vector<size_t> VarToExport;




	
	VarToExport.resize(m_pSimDoc->m_SelectPGFile.size());
	for(size_t i=0; i<VarToExport.size(); i++) 
		VarToExport[i]=m_pSimDoc->m_SelectPGFile[i].PGIndex-1;

	if(VarToExport.size()<=0) return false;

/*	int fileDlgResult;
			CString pszFileName ;
			LPCTSTR lpszDefExt="bin";
			LPCTSTR lpszFilter;
//The next line is only neccessary as long as the PGP does not handle long filenames!
//PEJ Excluded info box
			static char BASED_CODE szFilter[] = "PG Bin file (*.bin)|*.bin||";
			CFileDialog	BinFileDlg(TRUE,lpszDefExt="bin",lpszFilter = "*.bin",OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,szFilter);*/
//Show the dialog
			//fileDlgResult=BinFileDlg.DoModal();
			FILEDLG FDlg = MFC_Util::GetFileDialog("bin", "", "PG Bin file (*.bin)|*.bin||");

		string m_DestFileName;
		if (FDlg.opengood==IDOK) 
			m_DestFileName = FDlg.filename;
		else
			return false;
		
;

	//pCPG->SetCompleteRead(true);
	pCPG->ReOpen();
	pPG_Dest.SetCompleteRead(true);
	pPG_Dest.CreateNewFile(int(VarToExport.size()),pCPG->GetNumRecords());
	pPG_Dest.SetInfoMode();
	float value;
	long time, oldtime=0;
	for(int j=0; j<pCPG->GetNumRecords();j++) {
		for(size_t i=0;i<VarToExport.size(); i++) {
			if(i==0) {
			// Read and set the value of date/time
				time=pCPG->GetLongTime(j+1);
				if(time>oldtime)
					oldtime=time;
				else
					oldtime=0;
				
				pPG_Dest.SetLongTime(j+1, time );
			}	
			value=pCPG->GetVarValue(VarToExport[i]+1, j+1);
			pPG_Dest.SetVarValue(i+1,j+1, value);
		}
	}
	int varno;
	for(size_t i=0; i<VarToExport.size(); i++) {
		string txt;
		varno=VarToExport[i]+1;
		txt=pCPG->GetVarName(varno);
		pPG_Dest.SetVarName(i+1, txt);
		txt=pCPG->GetVarUnit(varno);
		pPG_Dest.SetVarUnit(i+1, txt);
		txt=pCPG->GetVarId(varno);
		pPG_Dest.SetVarId(varno,txt);		
		txt=pCPG->GetVarPos(varno);
		pPG_Dest.SetVarPos(i+1, txt);
	}
		
	pPG_Dest.CheckMissingNumbers();
	
	pPG_Dest.SaveAs(m_DestFileName,true);
	pPG_Dest.CloseFile();
	pCPG->CloseFile();

	#ifndef COUPSTD
			
				string	str="The PG-file: ";str+=m_DestFileName; str+=" created";
				//Display error message
				MFC_Util::MessageBox(str,"File Created",MB_OK);
	#endif	



	


	return true;
}
void PGCommands::OpenEditPGFile(int col, int row,int item) {
	CUGCell cell;
	int ParV;
	int index=0;
		if(item==3003) {
			GetCell(col,row, &cell); 
			ParV=cell.GetParam();
			GetCell(-2,row,&cell);
			index=int(cell.GetNumber());
		}
		else if(item==3004) {
			index=m_pPGArchive->m_PGFileArray.size();
			if(index<=0) return;
			index=m_pPGArchive->m_PGFileArray.size()-1;
		}

		CPG *pPG=(CPG*)m_pPGArchive->m_PGFileArray[index].pFile;
		string DumpFileName, filename;
		DumpFileName=pPG->GetFileName();

	
		size_t ipos=DumpFileName.rfind(".bin");

		if(ipos==string::npos)
			ipos=DumpFileName.rfind(".BIN");
		if(ipos>DumpFileName.size()-5&&ipos!=string::npos) {
			filename=DumpFileName.substr(0,ipos);
			filename+="_Copy.bin";
			FUtil::CopyFile_Std(pPG->GetFileName(), filename.data());
		}


		if(pPG->GetNumRecords()*pPG->GetNumRepititions()>50000) {
			pPG->SetCompleteRead(false);
		}
		else {
			pPG->CloseFile();	
		//	pPG->Reset();
			pPG->SetCompleteRead(true);
			pPG->Open(pPG->GetFileName());
		}

		CCreatePG PGWnd(m_pDoc, nullptr, pPG);
		if(PGWnd.DoModal()==IDOK) {
			string fil=pPG->GetFileName();
			remove(filename.data());
			pPG->CloseFile();
		}
		else {
			string fil=pPG->GetFileName();
			pPG->CloseFile();
			remove(fil.data());
			rename(filename.data(),fil.data());

		}




}
void PGCommands::AddPGFile() {
		CPG_FILEDLG dlg(nullptr,PGADD);
		if(dlg.DoModal()==IDOK) {
				CString filename=dlg.m_FileName;
				CPG *pPG= new CPG;
				pPG->Open(CT2A(filename).m_psz);
				PGFile	Fil;
				Fil.pFile=pPG;
				Fil.Visible=false;
				m_pPGArchive->m_PGFileArray.push_back(Fil);
				//m_pPGArchive->m_PGFileArray.push_back(pPG);
				//m_pDoc->m_Doc.m_PlotStorage.m_PGFileArrayShow.push_back(1);
		}



}
void PGCommands::RemovePGFile(int col, int row) {
		CUGCell cell;
		GetCell(col,row, &cell); 
		int ParV=cell.GetParam();
		GetCell(-2,row,&cell);
		size_t index=size_t(cell.GetNumber());
		index=m_pPGArchive->GetIndex(m_pDoc);

		CPG *pPG=(CPG*)m_pPGArchive->m_PGFileArray[index].pFile;
		if(pPG!=nullptr) {
			delete pPG;
			m_pPGArchive->m_PGFileArray.erase(m_pPGArchive->m_PGFileArray.begin()+index);
			
				MySingle::instance().GetCugPointer()->ClosePGFile();

		//	m_pDoc->m_Doc.m_PlotStorage.m_PGFileArrayShow.erase(m_pDoc->m_Doc.m_PlotStorage.m_PGFileArrayShow.begin()+index);					
		}
		int sheet=GetSheetNumber();
		EnableUpdate(FALSE); 
		while(GetNumberRows()>0)
				DeleteRow(0);
		EnableUpdate(TRUE); 
		OnSheetSetup(sheet);
}
bool PGCommands::CreateNewPGFileWithRepetition() {
	CPG_FILEDLG dlg(nullptr, PGMERGE);
	bool valid=FALSE;					
	if(dlg.DoModal()==IDOK) {
				CString filename=dlg.m_FileName;
				CPG *pPG= new CPG;
				pPG->Open(CT2A(filename).m_psz);
				PGFile	Fil;
				Fil.pFile=pPG;
				Fil.Visible=false;
				m_pPGArchive->m_PGFileArray.push_back(Fil);
				//m_pPGArchive->m_PGFileArray.push_back(pPG);
				//m_pDoc->m_Doc.m_PlotStorage.m_PGFileArrayShow.push_back(1);
	}
	return true;
}
void PGCommands::MixPGFilesWithoutInterpolations() {
	CPG_FILEDLG dlg(nullptr, PGMIX);
	bool valid=FALSE;					
	if(dlg.DoModal()==IDOK) {
				CString filename=dlg.m_FileName;
				CPG *pPG= new CPG;
				pPG->Open(CT2A(filename).m_psz);
				PGFile	Fil;
				Fil.pFile=pPG;
				Fil.Visible=false;
				m_pPGArchive->m_PGFileArray.push_back(Fil);
				//m_pPGArchive->m_PGFileArray.push_back(pPG);
				//m_pDoc->m_Doc.m_PlotStorage.m_PGFileArrayShow.push_back(1);
	}

}
void PGCommands::MixPGFilesWithInterpolations() {
		CPG_FILEDLG dlg(nullptr, PGMIX, TRUE);
		bool valid=FALSE;					
		if(dlg.DoModal()==IDOK) {
				CString filename=dlg.m_FileName;
				CPG *pPG= new CPG;
				pPG->Open(CT2A(filename).m_psz);
				PGFile	Fil;
				Fil.pFile=pPG;
				Fil.Visible=false;
				m_pPGArchive->m_PGFileArray.push_back(Fil);
				//m_pPGArchive->m_PGFileArray.push_back(pPG);
				//m_pDoc->m_Doc.m_PlotStorage.m_PGFileArrayShow.push_back(1);
		}
}
void PGCommands::ExportfromPGFileText(int col, int row) {
CUGCell cell;	
	GetCell(col,row, &cell); 
	int ParV=cell.GetParam();
	GetCell(-2,row,&cell);
	auto index=size_t(cell.GetNumber());

	CPG *pPG=(CPG*)m_pPGArchive->m_PGFileArray[index].pFile;
	if(pPG!=nullptr) {
		MySingle::instance().GetCugPointer()->UpdateStatusBar("Writing to file");
		string fileout=pPG->ExportToFile();
		CString out;
		if(fileout.size()>0) {
			out="Writing to text file: ";
			out+=fileout.c_str();
			out+= " Completed";
		}
		else {
			out="Writing to text file ";
			out+= " Failed";

		}
		MySingle::instance().GetCugPointer()->UpdateStatusBar(out);
	}
	

}
void PGCommands::ExportfromPGFileExcel(int col, int row) {
	CUGCell cell;	
	GetCell(col,row, &cell); 
	int ParV=cell.GetParam();
	GetCell(-2,row,&cell);
	auto index=size_t(cell.GetNumber());

	CPG *pPG=(CPG*)m_pPGArchive->m_PGFileArray[index].pFile;
	if(pPG!=nullptr) {
		MySingle::instance().GetCugPointer()->UpdateStatusBar("Writing to excel file");
		string filename=pPG->WriteExcelFile();
		CString out;
		if(filename.size()>0) {
			out="Writing to excel file: ";
			out+=filename.c_str();
			out+= " Completed";
		}
		else {
			out="Writing to excel file ";

			out+= " Failed";
		}
		MySingle::instance().GetCugPointer()->UpdateStatusBar(out);
	}
}
bool PGCommands::CreateNewPGFileWithNewVar(int col, int row) {
CUGCell cell;
	GetCell(col,row, &cell); 
		int ParV=cell.GetParam();
		GetCell(-2,row,&cell);
		size_t index=size_t(cell.GetNumber());

		CPG *pPG=(CPG*)m_pPGArchive->m_PGFileArray[index].pFile;
		pPG->CloseFile();
		pPG->Reset();
		pPG->SetCompleteRead(TRUE);
		pPG->Open(pPG->GetFileName());


		NewPGVariables dlg(pPG);
		if(dlg.DoModal()==IDOK) {
				CString filename=dlg.m_NewFileName;
				CPG *pPG= new CPG;
				pPG->Open(CT2A(filename).m_psz);
				PGFile	Fil;
				Fil.pFile=pPG;
				Fil.Visible=true;
				m_pPGArchive->m_PGFileArray.push_back(Fil);
				return true;									
				//m_pPGArchive->m_PGFileArray.push_back(pPG);
				//m_pDoc->m_Doc.m_PlotStorage.m_PGFileArrayShow.push_back(1);
		}
		return false;

}
bool PGCommands::ImportFromTextAndMixToPGFile() {
	CPG_FILEDLG dlg(nullptr, PGMIX, FALSE,TRUE, TRUE);
	if(dlg.DoModal()==IDOK) {
			CString filename=dlg.m_FileName;
			CPG *pPG= new CPG;
			pPG->Open(CT2A(filename).m_psz);
			PGFile	Fil;
			Fil.pFile=pPG;
			Fil.Visible=true;
			m_pPGArchive->m_PGFileArray.push_back(Fil);
			return true;
	}
	return false;
}
bool PGCommands::IncludeCoordinatesFromNOAA(int col, int row) {
	CUGCell cell;
	GetCell(-2,row,&cell);
		auto index=size_t(cell.GetNumber());

		CPG *pPG=(CPG*)m_pPGArchive->m_PGFileArray[index].pFile;

		string filename;

		int fileDlgResult;
	/*	CString pszFileName ;
		LPCTSTR lpszDefExt=_T("bin");
		LPCTSTR lpszFilter;
//The next line is only neccessary as long as the PGP does not handle long filenames!
//PEJ Excluded info box
		static char BASED_CODE szFilter[] = "Noaa Station File (*.TXT)|*.TXT||";
		CFileDialog	BinFileDlg(TRUE,lpszDefExt,lpszFilter,OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,szFilter);*/
//Show the dialog
		FILEDLG FDlg = MFC_Util::GetFileDialog("bin", "", "Noaa Station File (*.TXT)|*.TXT||");
		//If the reading of the file was successful

		if (FDlg.opengood==IDOK) {
			m_spatialPG.ReadStation(FDlg.filename, pPG);
			pPG->CloseFile();
			return true;
		}
		pPG->CloseFile();
		return false;	
}
bool PGCommands::MergeSimilarStations(int col, int row) {
CUGCell cell;
		GetCell(-2,row,&cell);
		auto index=size_t(cell.GetNumber());

		CPG *pPG=(CPG*)m_pPGArchive->m_PGFileArray[index].pFile;

		return m_spatialPG.MergeSimilarStation( pPG);


}
bool PGCommands::InterpolateBasedOnRegressions(int col, int row) {
	CUGCell cell;
	GetCell(-2,row,&cell);
	auto index=size_t(cell.GetNumber());
	CPG *pPG;
	if(index!=string::npos) 
		pPG=(CPG*)m_pPGArchive->m_PGFileArray[index].pFile;
	else
		pPG=m_pGSource;
		
	return m_spatialPG.CreateNewSpatialCorrelationFile(pPG);

}
bool PGCommands::CreateNewFileForNewPosition() {
	m_spatialPG.SetSourceFile(m_pGSource);
	CPG *pPGDest=m_spatialPG.CreateNewSinglePosFile(m_Latitude, m_Longitude);
	if(pPGDest!=nullptr) return true;
	else return false;

}
bool PGCommands::CreateNewFileForNewGridPosition() {
	m_spatialPG.SetSourceFile(m_pGSource);
	CPG *pPGDest=m_spatialPG.CreateNewGridFile(m_Lat_Max, m_Lat_Min,m_Long_Max, m_Long_Min, m_GridLat, m_GridLong);
	if(pPGDest!=nullptr) return true;
	else return false;
}
bool PGCommands::ReplaceMissingDataByLinearInterpolation() {
	m_spatialPG.SetSourceFile(m_pGSource);

	CPG *pPGDest=m_spatialPG.CreateNewInterPolatedFile(m_MaxSubstituteDays);
	if(pPGDest!=nullptr) return true;
	else return false;
}
bool PGCommands::ReplaceMissingDataByRandom() {
	m_spatialPG.SetSourceFile(m_pGSource);
	CPG *pPGDest=m_spatialPG.CreateNewRandomFillingFile();
	if(pPGDest!=nullptr) return true;
	else return false;
}
bool PGCommands::ImportFromNetCDF_File() {
	NetCDFTransfer transfer(2);
	return true;
}
bool PGCommands::SplitIntoNoRepetionsFiles(int col, int row){
	CUGCell cell;
		GetCell(-2,row,&cell);
		auto index=size_t(cell.GetNumber());
		CPG *pPG;
		if(index!=string::npos) 
			pPG=(CPG*)m_pPGArchive->m_PGFileArray[index].pFile;
		else
			pPG=m_pGSource;
		//NetCDFTransfer transfer;
		m_spatialPG.SetSourceFile(pPG);
		return m_spatialPG.SplitPGFile();
}
bool PGCommands::SetNewIndependenceVar(int item, int col, int row) {
 enum vartype var;
 CUGCell cell;
 var=PGVAR;
 size_t Index;
 Index=row+1;
 GetCell(-1, row, &cell);
 Index = size_t(cell.GetNumber());
 size_t pgfindex=m_pPGArchive->GetIndex(m_pDoc);
 CPG *pPG=(CPG*)m_pPGArchive->m_PGFileArray[pgfindex].pFile;
 int Irepit=1;
 SelectPG selpg;

if(item==5018||(item>=6010&&item<6020)) {
		m_pProp->m_ChartType=CHART_TYPES::SCATTER;
		string str;
		if(item==6011) {
			var=PGDAY;
			str="Day of Year";
		}
		else if(item==6010) {
			var=PGHOUR;
			str="Hour";
		}
		else if(item==6012) {
			var=PGYEAR;
			str="Year";

		}
		else if(item>=6015&&item<=6017) {
			m_pProp->m_ChartType= CHART_TYPES::TIME_SERIE;
			m_pProp->SetFirstIndepSet(enum vartype(m_pProp->GetIndepFirst()), false);
			m_pProp->SetFirstIndepVarType(NO_USE);
			return true;
		}
		m_pProp->ResetXScale();

		m_pProp->SetFirstIndepVarType(var);
		m_pProp->SetFirstIndepIndex(var, Index);
		
		if(item==5018) {	
			str=pPG->GetVarName(Index,Irepit );
			FUtil::trim(str);
			str += " ";
			string unit = pPG->GetVarUnit(Index, Irepit);
			FUtil::trim(unit);
			str+=" ("+unit+")";
		}
		else if(item>=6010) {
			m_pProp->SetFirstIndepSet(var,true);
		}
		m_pProp->SetFirstIndepString(var,str);
		if(item==5018) {
			GetCell(3,row,&cell);
			cell.SetBackColor(RGB(240,80,80));
			cell.SetReadOnly(TRUE);
			SetCell(3,row, &cell);
			RedrawCell(3,row);
			QuickSetText(4, row, "Indep 1");
			RedrawCell(4,row);
		}
	/*	if(m_pProp->m_ActualMultiSheet>0) {
			MultiPlotClick(m_pProp->m_ActualMultiSheet,col, row, false, 1);
			UpdatePlotStatusBar(m_pProp->m_ActualMultiSheet);
		}*/
		return true;
	}
else if(item==5020) {
		switch (var) {
				case PGVAR:
				while(m_pSimDoc->m_SelectPGFile.size()>0) {
						selpg=m_pSimDoc->m_SelectPGFile[0];
						GetCell(3,selpg.row,&cell);
						cell.SetBool(FALSE);
						cell.SetReadOnly(FALSE);
						cell.SetBackColor(m_NoColor);
						SetCell(3,selpg.row,&cell);
						RedrawCell(3,selpg.row);
						m_pSimDoc->m_SelectPGFile.erase(m_pSimDoc->m_SelectPGFile.begin());
				}
				break;
	/*			case VALVAR:
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
				break;*/
			}
			m_PGCharts.TimeSerieChartClick();
		/*	m_CurrentChart++;
			if(m_pProp->m_ActualMultiSheet>0) {
				MultiPlotClick(m_pProp->m_ActualMultiSheet,col, row, false, 1);
				UpdatePlotStatusBar(m_pProp->m_ActualMultiSheet);
			}*/
		return true;
	}
	else if(item==5019) {
			m_pProp->m_ChartType= CHART_TYPES::TIME_SERIE;
			m_pProp->SetFirstIndepVarType(DATEAXIS);

			m_pProp->SetFirstIndepString(var,"");
			m_pProp->SetSymbolStyle(0, 0);
			m_pProp->ResetXScale();
			GetCell(col,row,&cell);
			cell.SetBackColor(RGB(255,255,255));
			SetCell(col,row, &cell);
			RedrawCell(col,row);
			GetCell(3,row,&cell);
			cell.SetBackColor(RGB(255,255,255));
			cell.SetReadOnly(FALSE);
			SetCell(3,row, &cell);
			RedrawCell(3,row);
			QuickSetText(4, row, "");

			RedrawCell(4,row);




			//m_pProp->SetFirstIndepSet(var,false);
		return true;
	}
	else if(item==5028||(item>=6020&&item<6030)) {
		m_pProp->m_ChartType= CHART_TYPES::CONTOUR_CHART;
		string str;
		m_pProp->SetSecondIndepVarType(var);
		m_pProp->SetSecondIndepIndex(var, Index);
		if(item==5028) {
			str=pPG->GetVarName(Index,Irepit );
			FUtil::trim(str);
			str += " ";
			string unit = pPG->GetVarUnit(Index, Irepit);
			FUtil::trim(unit);
			str += " (" + unit + ")";
		}
		else if(item>=6020) {
			if(item==6021) {
				var=PGDAY;
				str="Day of Year";
			}
			else if(item==6020) {
				var=PGHOUR;
				str="Hour";
			}
			else if(item==6022) {
				var=PGYEAR;
				str="Year";

			}
			m_pProp->SetSecondIndepSet(var,true);
			m_pProp->ResetZScale();
		}
		else if(item>=6025&&item<=6027) {
			m_pProp->m_ChartType= CHART_TYPES::TIME_SERIE;
			m_pProp->SetSecondIndepSet(enum vartype(m_pProp->GetIndepSecond()), false);
			m_pProp->SetSecondIndepVarType(NO_USE);
			m_pProp->ResetXScale();
			return true;
		}

		m_pProp->SetSecondIndepString(var,str);
		if(item==5028) {
			GetCell(3,row,&cell);
			cell.SetBackColor(RGB(240,240,80));
			cell.SetReadOnly(TRUE);
			SetCell(3,row, &cell);
			RedrawCell(3,row);
			QuickSetText(4, row, "Indep 2");
			RedrawCell(4,row);
		}
		//if(m_pProp->m_ActualMultiSheet>0) {
			//MultiPlotClick(m_pProp->m_ActualMultiSheet,col, row, true,2 );
			//UpdatePlotStatusBar(m_pProp->m_ActualMultiSheet);
	//	}
		return true;

	}
	else if(item==5029) {
			m_pProp->m_ChartType= CHART_TYPES::CONTOUR_CHART;
			m_pProp->SetSecondIndepVarType(NO_USE);

			m_pProp->SetSecondIndepString(var,"");
			GetCell(col,row,&cell);
			cell.SetBackColor(RGB(255,255,255));
			SetCell(col,row, &cell);
			RedrawCell(col,row);
		//	MultiPlotClick(m_pProp->m_ActualMultiSheet,col, row, true,1 );
// Remove Selected First Independent Variable

		return true;
	}

	return false;
}