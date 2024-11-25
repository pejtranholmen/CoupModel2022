
#include "..\..\stdafx.h"
#include "..\..\Util.h"
#include "..\..\resource.h"
#include "ValidationCugButtons.h"
#include "..\SimDoc\MyCug.h"



ValidationCugButtons::ValidationCugButtons()
{
	DefineHintList();	m_pProp = MySingle::instance().GetChartPropPointer();
}


ValidationCugButtons::~ValidationCugButtons()
{
}

/////////////////////////////////////////////////////////////////////////////
//	return UG_SUCCESS to allow the menu to appear
//	return 1 to not allow the menu to appear
int ValidationCugButtons::OnMenuStart(int col, long row, int section)
{
	int sheetNumber = GetSheetNumber();
	m_CurrentTopRow = GetTopRow();
	EmptyMenu();
	//****** Populate the Menu 
	if (section == UG_GRID && col > 1) {

		AddMenuItem(1000, "Cut");
		AddMenuItem(2000, "Copy");
		AddMenuItem(3000, "Paste");
	}
	else if (section == UG_CORNERBUTTON) {
		auto pSw=m_pSimDoc->GetSw("PreDefinedEnsembles");
		if(pSw->GetIntValue()>0)
			AddMenuItem(1065, "Reset and ReCalculated with predefined criteria");
	}
	if (section == UG_GRID && col > 9 && sheetNumber == 3) {
		int pos = sheetNumber * m_pSimDoc->GetNumAll_TSV_ValVariables();
		auto value = m_pSimDoc->m_MStorage.GetMeanValid(row + pos, 0);
		if (value < 0)
			AddMenuItem(1075, "Remove Smallest of posterior candidates until reaching zero shift of mean values");
		else
			AddMenuItem(1075, "Remove Largest of posterior candidates until reaching zero shift of mean values");


	}
	if (section == UG_GRID && col > 9 && sheetNumber == 0) {
		long iaccept = m_pSimDoc->m_MStorage.GetNumAccepted();
		if (iaccept > 0) {
			if (m_pSimDoc->m_MStorage.m_AcceptedRuns.size() > m_pSimDoc->m_MStorage.m_ActualEnsemble)
				iaccept = m_pSimDoc->m_MStorage.m_AcceptedRuns[m_pSimDoc->m_MStorage.m_ActualEnsemble].size();
			if (iaccept > 200)
				AddMenuItem(1085, "Keep the 200 best");
			if (iaccept > 10)
				AddMenuItem(1087, "Keep the 10 best");

			AddMenuItem(1088, "Keep only the single best");
			AddMenuItem(1086, "Reduce to half of current posterior ensemble");
		}

	}
	else if (section == UG_GRID && col > 9 && sheetNumber >= 2 && sheetNumber <= 6) {
		long iaccept = m_pSimDoc->m_MStorage.GetNumAccepted();
		iaccept = m_pSimDoc->m_MStorage.m_AcceptedRuns[m_pSimDoc->m_MStorage.m_ActualEnsemble].size();
		if (iaccept > 200)
			AddMenuItem(1085, "Keep the 200 best");
		if (iaccept > 10)
			AddMenuItem(1087, "Keep the 10 best");
		AddMenuItem(1086, "Reduce to half of current posterior ensemble");

	}
	if (section == UG_CORNERBUTTON) {
		if (sheetNumber >= 10) {
			if (m_SelectedTimeShow != 0) AddMenuItem(3010, "View Individual runs - one by one");
			if (m_SelectedTimeShow != 1) AddMenuItem(3011, "View Individual runs - Accepted in same list");
			if (m_SelectedTimeShow != 2) AddMenuItem(3012, "View Individual runs - All in same list");
			if (m_SelectedTimeShow != 3) AddMenuItem(3013, "View Mean of All Accepted runs - Single Indicator at a time");
			if (m_SelectedTimeShow != 4) AddMenuItem(3014, "View Mean of All Accepted runs - All Indicators in same list");
			if (m_SelectedTimeShow != 5) AddMenuItem(3015, "View Mean of All runs - Single Indicator at a time");
			if (m_SelectedTimeShow != 6) AddMenuItem(3016, "View Mean of All runs - All Indicators in same list");

		}
		return TRUE;
	}
	if (section == UG_TOPHEADING && !m_OnGoingUpdate) {
		if (m_pSimDoc->m_MStorage.m_Valid_TSV.size() > m_pSimDoc->m_MStorage.m_ActualEnsemble) {
			m_Transformation = m_pSimDoc->m_MStorage.m_Valid_TSV[m_pSimDoc->m_MStorage.m_ActualEnsemble].VarTranformed; //GetTransformationIndicator(m_pSimDoc->m_MStorage.m_ActualEnsemble);
			if (col == 0) {
				if (m_Transformation != 0)
					AddMenuItem(4019, "Estimate performance indicators using actual values");
				if (m_Transformation != 1)
					AddMenuItem(4020, "Estimate performance indicators using cumulated values");
				if (m_Transformation != 2)
					AddMenuItem(4021, "Estimate performance indicators using year cumulated values");
				if (m_Transformation != 3)
					AddMenuItem(4022, "Estimate performance indicators using day cumulated values");
				if (m_Transformation != 4)
					AddMenuItem(4023, "Estimate performance with mean of Dimension 1 ");

				if (m_pSimDoc->m_MultiRun_Array[0].NumberOfRepeatitionsWithinDimension > 1) {
					size_t next_index = 0;
					for (size_t i = 0; i < m_pSimDoc->m_MStorage.m_Valid_TSV.size(); i++) {
						if (m_pSimDoc->m_MStorage.m_Valid_TSV[i].VarTranformed > 4) {
							if (m_pSimDoc->m_MStorage.m_Valid_TSV[i].VarTranformed - 4 > next_index) next_index = m_pSimDoc->m_MStorage.m_Valid_TSV[i].VarTranformed - 4;

						}
						if (next_index >= m_pSimDoc->m_MultiRun_Array[0].NumberOfRepeatitionsWithinDimension) {
							next_index = string::npos;
							break;
						}

					}
					if (next_index != string::npos)
						AddMenuItem(4024 + int(next_index), "Assign performance based on single index within Dimension 1 ");

				}
			}
			if (col == 1) {
				if (m_Distribution_As_Independent)
					AddMenuItem(4055, " Shift to Parameters as Independent in Charts");
				else {
					AddMenuItem(4055, " Shift to show distribution of Performance in Charts");
					if (m_Add_Second_Independent) AddMenuItem(4056, "Shift to only one Independent Par");
					else AddMenuItem(4056, "Shift to 2 Independent parameters");
					
				}


			}
		}
		if (col == 0) {
			AddMenuItem(1063, "Add New Ensemble for setting of new criteria");
			AddMenuItem(1064, "Add New Ensemble for setting of new criteria - Keeping actual criteria in new ensemble");
		}
		return true;
	}
		
	if (m_pSimDoc == nullptr) return false;

	int ifile=m_pSimDoc->m_ValidationData.GetValFileNumber(0);
	if(sheetNumber>=9&&sheetNumber<=10&&m_pSimDoc->MR_XBin_IsOpen(ifile)) {
			AddMenuItem(2010,"Estimate new Likelihood values based on new settings");

	}
	if(sheetNumber>=9&&sheetNumber<=10&&m_pSimDoc->MR_XBin_IsOpen(ifile)) {
			AddMenuItem(2020,"Create a file with Sorted Residuals");

	}
	if(m_pSimDoc->m_SelectMultiVal.size()>0&&sheetNumber<7) {
		
			AddMenuItem(4001,"Clear All Charts");
	}
	if(sheetNumber==12&&m_pSimDoc->MR_XBin_IsOpen(ifile)) {
				
		if (row < 2) {
			if(m_pSimDoc->MR_CheckIndexForFile_RepMode()) 
				if(m_pSimDoc->m_MultiFileSplitMode) 
					AddMenuItem(4000, "Reset split performance indicators from Index of parameter in Dimension 1");
				else
					AddMenuItem(4000, "Estimate performance indicators for each Index of parameter in Dimension 1");
			else if (m_pSimDoc->MR_CheckIndexForFilterMode()) {
				if (m_SplitPeriods > 0)
					AddMenuItem(4000, "Reset split performance indicators");
				else
					AddMenuItem(4000, "Estimate performance indicators based on Filter Variables");

			}
			else
				AddMenuItem(4000, "Estimate performance indicators valid for equal big sub periods");
		}
		else if (m_pSimDoc->m_MStorage.m_AcceptedIdString.size()>1&&row>3) {
			AddMenuItem(4091, "Remove Ensemble");


		}

	}
/*	else if(col<2) {
		if(m_SplitPeriods>0&&sheetNumber<7)
			AddMenuItem(4001,"Make Chart with performance indicator for all sub periods");
	}*/
	else if(col>9) {
		if(m_Changed) {
			if(ifile<0) {
				AddMenuItem(1060,"Update the list of Accepted Runs");
			}
			else if(m_pSimDoc->MR_XBin_IsOpen(ifile)) {
					AddMenuItem(1060,"Update the list of Accepted Runs");
			}
		}
		if(ifile<0) {
			AddMenuItem(1061,"Reset the list of Accepted Runs for actual ensemble to all Runs");
			if (m_pSimDoc->m_MStorage.m_AcceptedRuns.size() > 1) 
				AddMenuItem(1071, "Reset the list of Accepted Runs for all ensembles to all Runs");
		}
		else if(m_pSimDoc->MR_XBin_IsOpen(ifile)) {
				AddMenuItem(1061,"Reset the list of Accepted Runs for actual ensemble to all Runs");
				if (m_pSimDoc->m_MStorage.m_AcceptedRuns.size() > 1) 
					AddMenuItem(1071, "Reset the list of Accepted Runs for all ensembles to all Runs");
				if(!m_SavedCriteria2File) AddMenuItem(1062,"Save the set Criteria and Accepted Runs to file");
		}
		
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHint
int ValidationCugButtons::OnHint(int col,long row,int section,CString *str1)
{
	CUGCell cell;
	long ParV;
	GetCell(col, row, &cell);
	ParV = cell.GetParam();
	if (ParV > 0) {
		string text = GetHint(ParV);
		if (text.size()> 0) *str1 = text.c_str();
		return TRUE;
	}



	int sheetNumber=GetSheetNumber();

	if (section == UG_CORNERBUTTON) {

		str1->Format(_T("if you have a simulation with Predefined criteria right click to redefine your criteria after change of parameters in Criteria Table "));


	}
	if (section == UG_TOPHEADING) {
		if (col == 1 && row == -2) {
		
			str1->Format(_T("Click to shift chart type"));
		}
		else if (col == 1 && row == -1) {
		    str1->Format(_T("Click in Box below to view distribution of candidates"));
	
		}
		else {
			str1->Format(_T("Single click to shift to a new performance indicators or Right click to select new performance indicators"));
		}

	}
	else if (section == UG_GRID) {
		if (col < 2 && sheetNumber < 7) {
			if ((m_SplitPeriods == 0 && col == 0) || (m_SplitPeriods >= 0 && col == 0))
				str1->Format(_T("Single Click on check box to make a chart"));



		}
		else if (col > 1 && sheetNumber < 7) {
			if (m_pSimDoc->IsMultiStorageFileLocked()) str1->Format(_T("Unlock if yout want to change criteria to accept runs"));

			else
				str1->Format(_T("Edit Low and/or high limits to make constrains for accepting runs and right click to make changes"));

		}
		else if (sheetNumber == 9) {
			str1->Format(_T("Edit options for LogLikelihood estimations and right click to make changes"));
		}
		else if (sheetNumber == 12) {
			if (row < 2)
				str1->Format(_T("Edit options for number of sub periods and right click to make changes"));
			else if (row>3 && col == 0)
				str1->Format(_T("Edit Name of Ensemble"));
		}
		else
			str1->Format(_T("Col:%d Row:%ld"), col, row);
	}
	else if (section == UG_SIDEHEADING) {
		if (sheetNumber < 9) {
			str1->Format(_T("Single click to yellow lines to estimate time serie of selected ensemble"));


		}




	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnVScrollHint
int ValidationCugButtons::OnVScrollHint(long row,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHScrollHint
int ValidationCugButtons::OnHScrollHint(int col,CString *string)
{
	return TRUE;
}

void ValidationCugButtons::DefineHintList()
{
	HINTLIST h;
	h.param = GetUniqueMenuNumber();
	h.HintText = "The Document is locked - Please Unlock the document if you want to redefine criteria for acceptance\nUnlock by using toolbar VU on top toolbar line";
	SetHintMap(h); m_SetUnLockHint = h;

	h.param = GetUniqueMenuNumber();
	h.HintText = "Set Low value to a specific threshold and \ncontinue by update to constrains to posterior ensemble of accepting runs\n (right click to update or consider other methods to constrain)";
	SetHintMap(h); m_SetLowerHint = h;

	h.param = GetUniqueMenuNumber();
	h.HintText = "Set High value to a specific threshold and \ncontinue by update to constrains to posterior ensemble of accepting runs\n (right click to update or consider other methods to constrain)";

	SetHintMap(h); m_SetHigherHint = h;




}

