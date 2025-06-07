
#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include ".\PGButtonEvents.h"
#include "..\ChartProperties\ChartCug.h"
#include "..\..\PG\SelectPGTimeRange.h"

PGButtonEvents::PGButtonEvents(void)
{
	m_PreviousStart= m_PreviousStop=0;
}
PGButtonEvents::~PGButtonEvents(void)
{
}

void PGButtonEvents::AllSheetButtons(int sheet, int ParV, int index_col, int col)
{
	if (ParV == 0) return;
	if (sheet == 0 || sheet == 8) {
		if (ImportListButtons(ParV, index_col, col))
			ImportSheetButtons(ParV, col);

	}
	else if (sheet == 10)
		DescriptionButtons(ParV);
	else
		DataSheetButtons(ParV);
}


void PGButtonEvents::DataSheetButtons(int ParV)
{
	bool ActionMade = false;
	if (m_RepSortButton.Param == ParV) {
		if (m_RepSortButton.NormalState)
			m_pPG->SetViewOrder(false);
		else
			m_pPG->SetViewOrder(true);
		ActionMade = true;
	}
	else if (m_SelectTimeButton.Param == ParV) {
		if (m_SelectTimeButton.NormalState) {
			m_pPG->SetDateSelection(false);
			m_pPG->SetCalcStart(m_PreviousStart);
			m_pPG->SetCalcStop(m_PreviousStop);
			ActionMade = true;
		}
		else {
			m_pPG->SetDateSelection(true);
			SelectPGTimeRange Dlg;
			Dlg.Init(m_pPG);
			if (Dlg.DoModal() == IDOK);
			ActionMade = true;
			m_PreviousStart = m_pPG->GetCalcStart();
			m_PreviousStop = m_pPG->GetCalcStop();
		}
	}
	else if (m_ViewStartHint.param == ParV) {
		if (m_Changed_Data) {
			OnWriteGridDataToFile(m_pPG);

		}
		m_pPG->incrementCurrentPeriodToView();
		ActionMade = true;
	}

	if(ActionMade)	Make_DataHeader();

}

void PGButtonEvents::ImportSheetButtons(int ParV, int col)
{
	 if (m_ScanFileButton.Param == ParV) {
		m_PGFileImport.m_ExtraFileScanning = false;
		m_PGFileImport.m_ScanningMade = false;
		m_PGFileImport.m_format = PROFOUND;
		m_PGFileImport.AllFileInfos.clear();
		m_PGFileImport.m_CheckedTimeMap.clear();
		vector<string> files = MFC_Util::GetCSVFileNames();
		for (size_t i = 0; i < files.size(); i++) {
			if (!m_PGFileImport.OpenInputFile(files[i])) return;
			m_PGFileImport.m_CurrentFileInfo = m_PGFileImport.ScanningInputFile();

			m_PGFileImport.AllFileInfos.push_back(m_PGFileImport.m_CurrentFileInfo);
		}
		if (m_PGFileImport.m_format == SILO) {

			vector<string> filenames;
			m_PGFileImport.m_AddFromFile = false;
			filenames = m_PGFileImport.CreatePGFileFromDefinedInputFiles();
			m_PGFileImport.m_SuccessFullImportedFileNames.push_back(filenames[0]);

		}
		else {
			m_PGFileImport.m_ScanningMade = true;
			m_PGFileImport.m_SuccessFullImportedFileNames.clear();
			Make_ImportFromCSV_File_Prior(true);
		}
		return;
	}
	else if (m_OpenFileButton.Param == ParV) {
		if (m_PGFileImport.m_ExtraFileScanning) {
			m_PGFileImport.DefineAddInfoMap();
		}
		m_PGFileImport.m_AddFromFile = false;
		auto DesFirst = m_PGFileImport.m_CurrentFileInfo.Des;
		auto RedNumVar = m_PGFileImport.m_CurrentFileInfo.RedNumVar;
		auto ValidVar = m_PGFileImport.m_CurrentFileInfo.ValidVariables;
		auto ValidVarIndex = m_PGFileImport.m_CurrentFileInfo.ValidVariableIndex;
		auto minst=m_PGFileImport.m_CurrentFileInfo.IdentifiedStartMin;
		if (m_PGFileImport.AllFileInfos.size() > 1) m_PGFileImport.m_CheckedTimeMap.clear();

		for (size_t i = 0; i < m_PGFileImport.AllFileInfos.size(); i++) {
			if(m_PGFileImport.AllFileInfos.size()>1) m_PGFileImport.m_CurrentFileInfo = m_PGFileImport.AllFileInfos[i];
			m_PGFileImport.m_CurrentFileInfo.Des = DesFirst;
			m_PGFileImport.m_CurrentFileInfo.RedNumVar = RedNumVar;
			m_PGFileImport.m_CurrentFileInfo.ValidVariables = ValidVar;
			m_PGFileImport.m_CurrentFileInfo.ValidVariableIndex = ValidVarIndex;
			m_PGFileImport.m_CurrentFileInfo.IdentifiedStartMin = minst;
			if (!m_PGFileImport.OpenInputFile(m_PGFileImport.AllFileInfos[i].InputfileName)) return;
			m_PGFileImport.m_format = m_PGFileImport.m_CurrentFileInfo.IdentifiedFormat;
			m_PGFileImport.SetStartMinuteWhenImport(m_PGFileImport.m_CurrentFileInfo.IdentifiedStartMin);
			vector<string> filenames;
			filenames = m_PGFileImport.CreatePGFileFromDefinedInputFiles();
			m_PGFileImport.m_SuccessFullImportedFileNames.push_back(filenames[0]);
		}
	
		
		
		if(m_PGFileImport.m_SuccessFullImportedFileNames.size()==0) 
			Make_ImportFromCSV_File_Prior(false);
		else
			Make_ImportFromCSV_File_Post();
	}
	else if (m_SelectVariableButton.Param== ParV) {

		for (size_t i = 0; i < m_PGFileImport.m_CurrentFileInfo.ValidVariables.size(); i++) {
			if(m_SelectVariableButton.NormalState)
				m_PGFileImport.m_CurrentFileInfo.ValidVariables[i] = true;
			else
				m_PGFileImport.m_CurrentFileInfo.ValidVariables[i] = false;
		}
		m_PGFileImport.UpdateNumberOfValidVariables();
		Make_ImportFromCSV_File_Prior(false, false);
	}
	else if (m_SelectedOutputVarButton.Param == ParV) {
		if (m_SelectedOutputVarButton.VectorState.size() <= col) return;
		if (m_SelectedOutputVarButton.VectorState[col])
			m_PGFileImport.m_CurrentFileInfo.ValidVariables[col] = false;
		else
			m_PGFileImport.m_CurrentFileInfo.ValidVariables[col] = true;

		m_PGFileImport.UpdateNumberOfValidVariables();
		Make_ImportFromCSV_File_Prior(false, false);
	}

	else if (m_SiteInfoFileButton.Param==ParV) {
		m_PGFileImport.m_ExtraFileScanning = true;
		m_PGFileImport.m_format = IDSCAN;
		m_PGFileImport.m_ExtraInfoFileName = MFC_Util::GetCSVFileName();
		m_PGFileImport.m_ExtraFileInfo = m_PGFileImport.ScanningInputFile(m_PGFileImport.m_ExtraInfoFileName);

		Make_ImportFromCSV_File_Prior(true);
	}
	else if (m_ChartTypeOptions.Param == ParV) {
	/*	if (m_pProp->ChangeChartTypeForward()) {
			OnSheetSetup(0);
			if (m_pProp->GetChartTypeIndex() == 1)
				m_pUtil->m_FileSave.OnReDraw(false, m_pSimDoc);
			else {
				m_pProp->ResetXScale();
				m_pProp->ResetYLeftScale();
				m_pProp->ResetYRightScale();
				m_pUtil->m_FileSave.OnReDraw(true, m_pSimDoc);
			}
		}*/
	}
	
	else if (m_CreateNewEmptyPGFile.Param == ParV) {
		if (m_pPGNewGrid->WriteNewPGFileAs(m_pPGNewGrid->GetFileName(), true)) {
			string filename = m_pPGNewGrid->GetFileName();
			delete m_pPGNewGrid;
			m_pPGNewGrid = nullptr;
			AddPGFile(filename);
			m_NewGridInfo.Assigned = false;
			OnSheetSetup(0);
		}
	}

	else if (m_ResetListAddButton.Param == ParV ) {
		m_AddMix_Actions.DeleteFiles();
		if(m_AddMix_Actions.m_pPGADD_Out!=nullptr)	Make_Add_Prior();
		if (m_AddMix_Actions.m_pPGMix_Out != nullptr)  Make_Mix_Prior();
		if (m_AddMix_Actions.m_pPGMerge_Out != nullptr) Make_Merge_Prior();
 	}
	else if (ParV == m_QuitImportButton.Param) {
		m_PGFileImport.m_ScanningMade = false;
		m_PGFileImport.m_ImportedFileVector.clear();
		m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.clear();
		m_PGFileImport.m_CurrentFileInfo.ValidVariables.clear();
		m_PGFileImport.m_SuccessFullImportedFileNames.clear();
		Make_New_PGFiles();
	}
	else if (ParV == m_QuitButton.Param) {
		m_PGFileImport.m_ScanningMade = false;
		m_PGFileImport.m_ImportedFileVector.clear();
		m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.clear();
		m_AddMix_Actions.m_pPGADD_Out = nullptr;
		m_AddMix_Actions.m_pPGMix_Out = nullptr;
		m_AddMix_Actions.m_pPGMerge_Out = nullptr;
		m_pPGNewGrid = nullptr;
		Make_New_PGFiles();
	}
	else if (ParV == m_NewOutputNewGridPGFileButton.Param) {
		string filename = MFC_Util::GetBinFileName();
		m_pPGNewGrid->SetFileName(filename);
		Make_NewGrid_Prior();
	}
	else if (m_NewOutputPGFileButton.Param == ParV) {
		m_AddMix_Actions.m_pPGADD_Out->SetFileName(MFC_Util::GetBinFileName());
		Make_Add_Prior();
	}
	else if (m_NewOutputMIXPGFileButton.Param == ParV) {
		m_AddMix_Actions.m_pPGMix_Out->SetFileName(MFC_Util::GetBinFileName());
		Make_Mix_Prior();
	}
	else if (m_NewOutputMergePGFileButton.Param == ParV) {
		m_AddMix_Actions.m_pPGMerge_Out->SetFileName(MFC_Util::GetBinFileName());
		Make_Merge_Prior();
	}
	else if (m_MixInterpolateButton.Param == ParV) {
		if (m_MixInterpolateButton.NormalState) m_MixInterpolateButton.NormalState = false;
		else m_MixInterpolateButton.NormalState = true;

		m_AddMix_Actions.m_Interpolate = m_MixInterpolateButton.NormalState;
		m_AddMix_Actions.SetSizeOfMixFile();
		Make_Mix_Prior();

	}
	else if (m_AddMorePGFilesButton.Param == ParV) {
		m_AddMix_Actions.AddPGFiles();
		Make_Add_Prior();
	}
	else if (m_MixMorePGFilesButton.Param == ParV) {
		m_AddMix_Actions.AddPGFiles();
		Make_Mix_Prior();
	}
	else if (m_MergeMorePGFilesButton.Param == ParV) {
		m_AddMix_Actions.AddPGFiles();
		Make_Merge_Prior();
	}
	else if (m_CreateNewAddButton.Param == ParV) {
		CPG *pPG = m_AddMix_Actions.AddFileToNewPGFile();
		if (pPG == nullptr) {
			MFC_Util m;
			m.MessageBox("Failed to create new file: ");
			Make_Add_Prior();
			return;

		}
		else {
			string filename = pPG->GetFileName();
			delete pPG;
			m_AddMix_Actions.m_pPGADD_Out = nullptr;
			m_AddMix_Actions.DeleteFiles();
			AddPGFile(filename);
			OnSheetSetup(0);
		}


	}
	else if (m_ShiftOutputFormat.Param == ParV) {
		if (m_ShiftOutputFormat.NormalState) m_ShiftOutputFormat.NormalState = false;
		else m_ShiftOutputFormat.NormalState = true;
		if (m_AddMix_Actions.m_pPGReshape_Out != nullptr) {
			delete m_AddMix_Actions.m_pPGReshape_Out;
			m_AddMix_Actions.m_pPGReshape_Out = nullptr;
		}
		Make_ReShape_Prior(true);
	}
	else if (m_CreateNewReshapeButton.Param == ParV) {
		if (m_AddMix_Actions.m_pPGReshape_Out != nullptr) {
			string filename = m_AddMix_Actions.m_pPGReshape_Out->GetFileName();
			delete m_AddMix_Actions.m_pPGReshape_Out;
			m_AddMix_Actions.m_pPGReshape_Out = nullptr;
			AddPGFile(filename);
			OnSheetSetup(0);
		}
		else {
			MFC_Util m;
			m.MessageBox("Failed to create new file: ");
			Make_Add_Prior();
			return;
		}
	}
	else if (m_UpdateReshapeButton.Param == ParV) {
		Make_ReShape_Prior(true);

	}
	else if (m_CreateNewMixButton.Param == ParV) {
		CPG *pPG = m_AddMix_Actions.MixFileToNewPGFile();
		if (pPG == nullptr) {
			MFC_Util m;
			m.MessageBox("Failed to create new file: ");
			Make_Mix_Prior();
			return;
		}
		else {
			string filename = pPG->GetFileName();
			delete pPG;
			m_AddMix_Actions.m_pPGMix_Out = nullptr;
			m_AddMix_Actions.DeleteFiles();
			AddPGFile(filename);
			OnSheetSetup(0);
		}
	}
	else if (m_CreateNewMergeButton.Param == ParV) {
		CPG *pPG = m_AddMix_Actions.MergeFileToNewPGFile();
		if (pPG == nullptr) {
			MFC_Util m;
			m.MessageBox("Failed to create new file: ");
			Make_Merge_Prior();
			return;
		}
		else {
			string filename = pPG->GetFileName();
			delete pPG;
			m_AddMix_Actions.m_pPGMerge_Out = nullptr;
			m_AddMix_Actions.DeleteFiles();
			AddPGFile(filename);
			OnSheetSetup(0);
		}
	}






}

bool PGButtonEvents::ImportListButtons(int ParV, int index, int col)
{

	if (m_NewFileOptions.Param == ParV) {

		if (index == 1) { Make_NewGrid_Prior(); }
		else if (index == 2) { Make_ImportFromCSV_File_Prior(); }
		else if (index == 3) {
			//m_AddMix_Actions.AddPGFiles();		
			Make_Add_Prior();
		}
		else if (index == 4) { Make_Mix_Prior(); }
		else if (index == 5) { Make_Merge_Prior(); }
		else if (index == 6) Make_ReShape_Prior();
	}
	else if (ParV == m_ImportedFiles.Param) {
		if (index >= 1 && m_ImportedFiles.VectorNames.size() >= index) {
			string filename = m_ImportedFiles.VectorNames[index - 1];
			AddPGFile(filename);
			OnSheetSetup(0);
		}
	}
	else if (ParV >= m_ComboIdButton.Param&&ParV<m_ComboIdButton.Param+5) {
			m_PGFileImport.AddInfoLinkIndex[ParV - m_ComboIdButton.Param] = index;
			Make_ImportFromCSV_File_Prior();
	}
	else if (ParV == m_AddFileList.Param&&index > 0 && index <= m_AddFileList.VectorNames.size()) {
		string filename = m_AddFileList.VectorNames[index - 1];
		MFC_Util m;
		auto ans = m.MessageBoxReturn("Do you Want to remove file: " + filename + " from list", "RemoveFrom Add List", IDOK);
		if (ans == IDOK) {
			m_AddMix_Actions.DeleteFile(index - 1);

		}
		Make_Add_Prior();

	}
	else if (ParV == m_ClickBoxFormatButton.Param) {
		m_PGFileImport.m_format = enum formats(index);
		m_PGFileImport.m_CurrentFileInfo.IdentifiedFormat = m_PGFileImport.m_format;
		Make_ImportFromCSV_File_Prior();
		return true;

	}
	else if (ParV == m_FilterVariableButton.Param) {
		if (index == 0) m_PGFileImport.SetFilterTestColumn(string::npos);
		else m_PGFileImport.SetFilterTestColumn(index);
		m_PGFileImport.ScanningInputFile();
		m_PGFileImport.m_ExtraFileScanning = false;
		m_PGFileImport.m_ScanningMade = false;
		m_PGFileImport.m_format = PROFOUND;
		
	
		m_PGFileImport.m_CurrentFileInfo = m_PGFileImport.ScanningInputFile();
		m_PGFileImport.m_SuccessFullImportedFileNames.clear();
		m_PGFileImport.m_ScanningMade = true;

		Make_ImportFromCSV_File_Prior();
		return true;

	}
	else if (ParV == m_FilterUseButton.Param) {
		if (index == 0) m_PGFileImport.SetFilterToUseId("");
		else m_PGFileImport.SetFilterToUseId(string(m_FilterUseButton.VectorNames[index]));
		m_PGFileImport.ScanningInputFile();
		m_PGFileImport.m_ExtraFileScanning = false;
		m_PGFileImport.m_ScanningMade = false;
		m_PGFileImport.m_format = PROFOUND;


		m_PGFileImport.m_CurrentFileInfo = m_PGFileImport.ScanningInputFile();
		m_PGFileImport.m_SuccessFullImportedFileNames.clear();
		m_PGFileImport.m_ScanningMade = true;

		Make_ImportFromCSV_File_Prior();
		return true;

	}
	else if (ParV == m_ElementTypeNewVariables.Param) {
		m_PGFileImport.m_CurrentFileInfo.Des.addinfo[m_PGFileImport.m_NewVarIndexSelected[col]].Element = elements(index);
		Make_ImportFromCSV_File_Prior();
		return true;
	}
	else if (ParV == m_UnitTypeNewVariables.Param) {
		m_PGFileImport.m_CurrentFileInfo.Des.addinfo[m_PGFileImport.m_NewVarIndexSelected[col]].Unit = UNIT_TYPES(index);
		Make_ImportFromCSV_File_Prior();
		return true;
	}

	else if (ParV == m_ApplyNameFromHeadingButton.Param) {
		if (m_ApplyNameFromHeadingButton.NormalState) m_ApplyNameFromHeadingButton.NormalState = false;
		else m_ApplyNameFromHeadingButton.NormalState = true;
		m_PGFileImport.ApplyHeaderLineAsNamesInPGFile(false, m_ApplyNameFromHeadingButton.NormalState);
		Make_ImportFromCSV_File_Prior();
		return true;
	}
	else if (ParV == m_ApplyUnitFromHeadingButton.Param) {
		if (m_ApplyUnitFromHeadingButton.NormalState) m_ApplyUnitFromHeadingButton.NormalState = false;
		else m_ApplyUnitFromHeadingButton.NormalState = true;
		m_PGFileImport.ApplyHeaderLineAsNamesInPGFile(true, m_ApplyUnitFromHeadingButton.NormalState);

		Make_ImportFromCSV_File_Prior();
		return true;
	}
	else if (ParV == m_ApplyUnitFromUnitTypeButton.Param) {
		
		if (m_ApplyUnitFromUnitTypeButton.NormalState) m_ApplyUnitFromUnitTypeButton.NormalState = false;
		else m_ApplyUnitFromUnitTypeButton.NormalState = true;
		
		ApplyUnitTypeToUnit(m_ApplyUnitFromUnitTypeButton.NormalState);
		Make_ImportFromCSV_File_Prior();
		return true;
	}
	else if (ParV == m_ApplyNameFromElementTypeButton.Param) {

		if (m_ApplyNameFromElementTypeButton.NormalState) m_ApplyNameFromElementTypeButton.NormalState = false;
		else m_ApplyNameFromElementTypeButton.NormalState = true;

		ApplyElementTypeToName(m_ApplyNameFromElementTypeButton.NormalState);
		Make_ImportFromCSV_File_Prior();
		return true;
	}

	else
		return true;


	return false;
}

bool PGButtonEvents::DescriptionListButtons(int ParV, int index_choice, int col, int row)
{
	CPG *pPG = GetCurrentPG_Pointer();
	if (pPG != nullptr) {
		size_t ivar, irep;
		ivar = row % (pPG->pg.Shape.NumVar) + 1;
		irep = row / pPG->pg.Shape.NumVar + 1;

		if (ParV == m_UnitTypeVariables.Param) {
			UNIT_TYPES unit = UNIT_TYPES(index_choice);
			pPG->SetVarUnitType(ivar, irep, unit);
			return true;
		}
		if (ParV == m_ElementTypeVariables.Param) {
			elements elem = elements(index_choice);
			pPG->SetVarElementType(ivar, irep, elem);
			vector<UNIT_TYPES> types = GetUnitOptions(elem);
			if (types.size() > 0) {
				pPG->SetVarUnitType(ivar, irep, types[0]);
			}
			OnGetFileDescriptionFillGrid(pPG);
			return true;
		}

	}
	return false;
}

void PGButtonEvents::DescriptionButtons(int ParV)
{
	MFC_Util m;
	CPG *pPG = GetCurrentPG_Pointer();
	if (pPG != nullptr) {
		if (ParV == m_ApplyUnitTypeButton.Param) {
			for (size_t irep = 0; irep < pPG->GetNumRepititions(); irep++)
				for (size_t ivar = 1; ivar <= pPG->GetNumVariables(); ivar++) {
					string str;
					UNIT_TYPES unit=pPG->GetVarUnitType(ivar, irep);

					if (unit < UNIT_TYPES::NO_UNIT&&unit >= UNIT_TYPES(0)) {
						pPG->SetVarUnit(ivar, string(p_ModelInfo->GetUnitString(unit)), irep);
						pPG->pg.status.FileModified = true;
					}
				}

		}
		else if(ParV==m_ApplyElementTypeButton.Param) {
			for (size_t irep = 0; irep < pPG->GetNumRepititions(); irep++)
				for (size_t ivar = 1; ivar <= pPG->GetNumVariables(); ivar++) {
					string str;
					elements elem = pPG->GetVarElement(ivar, irep);
					if (elem != elements::GENERAL&&elem >= elements(0) && size_t(elem) < p_ModelInfo->GetNumberOfElements()) {
						pPG->SetVarName(ivar, string(p_ModelInfo->ElementName(size_t(elem))), irep);
						m_Changed_Description = true;
						pPG->pg.status.FileModified = true;
					}

				}
		}
		else if (ParV == m_SaveCurrentFileButton.Param) {

			if (m_SaveCurrentFileButton.NormalState) {
				string newname = MFC_Util::GetBinFileName();
				if (pPG->WriteNewPGFileAs(newname)) {
					pPG->SetFileName(newname);
					string file = pPG->GetFileName();
					m.SetPaneMessage("Succesfully saved file as:" + file);
					MFC_Util::MessageBox("Succesfully saved file as" + file);
					m_Changed_Data = false;
					m_Changed_Description = false;
				}

				else
					m.SetPaneMessage("Failed to save file");
			}
			else {
				if (pPG->WritePGFile()) {
					string file = pPG->GetFileName();
					m.SetPaneMessage("Succesfully saved file as:" + file);
					MFC_Util::MessageBox("Succesfully saved file as" + file);
					m_Changed_Data = false;
					m_Changed_Description = false;
				}

				else
					m.SetPaneMessage("Failed to save file");


			}
		}
		OnSheetSetup(10);
	};
}

void PGButtonEvents::ApplyUnitTypeToUnit(bool CleanInstead)
{
	size_t count = 0;

	for_each(m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.begin(), m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.end(), [&](string &str) {
		if (CleanInstead)
			m_PGFileImport.m_CurrentFileInfo.Des.base.Unit[count] = "";
		else
			m_PGFileImport.m_CurrentFileInfo.Des.base.Unit[count] = p_ModelInfo->GetUnitString(m_PGFileImport.m_CurrentFileInfo.Des.addinfo[count].Unit);
		count++;
	});
}
void PGButtonEvents::ApplyElementTypeToName(bool CleanInstead)
{
	size_t count = 0;

	for_each(m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.begin(), m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.end(), [&](string &str) {
		if (CleanInstead)
			m_PGFileImport.m_CurrentFileInfo.Des.base.Name[count] = "";
		else
			m_PGFileImport.m_CurrentFileInfo.Des.base.Name[count] = p_ModelInfo->ElementName(size_t(m_PGFileImport.m_CurrentFileInfo.Des.addinfo[count].Element));
		count++;
	});
}



