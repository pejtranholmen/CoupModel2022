
#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include ".\PGEditCells.h"
#include "..\ChartProperties\ChartCug.h"
#include "..\..\PG\SelectPGTimeRange.h"

PGEditCells::PGEditCells(void)
{

}
PGEditCells::~PGEditCells(void)
{
}

bool PGEditCells::AllEditCells(int col, long row, string txt)
{
	CUGCell cell;
	int isheet = GetSheetNumber();
	GetCell(col, row, &cell);
	int ParV = cell.GetParam();
	auto NewGridFields = [&]() {
		switch (col) {
		case 1:
			m_pPGNewGrid->AdjustSize(FUtil::AtoInt(txt), m_pPGNewGrid->GetNumVariables() , m_pPGNewGrid->GetNumRepititions());
			break;
		case 2:
			m_pPGNewGrid->AdjustSize(m_pPGNewGrid->GetNumRecords(),FUtil::AtoInt(txt),m_pPGNewGrid->GetNumRepititions());
			break;
		case 3:
			m_pPGNewGrid->AdjustSize(m_pPGNewGrid->GetNumRecords(), m_pPGNewGrid->GetNumVariables(), FUtil::AtoInt(txt) );
			break;
		case 4:
			m_pPGNewGrid->SetLongTimeStart(PGUtil::MinutConv(txt, false, false));
			break;
		case 6:
			m_pPGNewGrid->SetNormalTimeInterval(FUtil::AtoInt(txt));
			break;
		};
		return Make_NewGrid_Prior();
	};

	
	if (isheet == 0 || isheet == 8) {
		if(m_pPGNewGrid!=nullptr)
			return NewGridFields();
		else if (ParV == m_ScanStartDate.param) {
			m_PGFileImport.SetStartMinuteWhenImport(PGUtil::MinutConv(txt));
			m_PGFileImport.m_CurrentFileInfo.IdentifiedStartMin = PGUtil::MinutConv(txt);
		}
		else if (ParV == m_ScanMinDiff.param) {
			m_PGFileImport.SetNormalTimeInterval(FUtil::AtoInt(txt));


		}
		else if (ParV == 12000&&col==1&&row>=5) {
			
			m_ReShaped_Actions.treatment_convert[row-5].first = txt;
			return true;
		}
		else if (ParV == 12000 && col == 2 && row >= 5) {

			m_ReShaped_Actions.treatment_convert[row - 5].second = FUtil::AtoInt(txt);
			return true;
		}
		else if (ParV == 12000 && col == 3 && row == 3) {
			int value = FUtil::AtoInt(txt);
			m_ReShaped_Actions.treatment_convert.resize(value);
			Make_ReShape_Prior(1);
			return true;
		}
		else if (ParV == m_AddScalingCoefHint.param) {
			int value = FUtil::AtoInt(txt);
			if(m_AddMix_Actions.m_AddScalingCoefVector.size()>row)
			m_AddMix_Actions.m_AddScalingCoefVector[row] = value;
			return TRUE;
		}
		else if (ParV == m_DescriptionImportedFile.param) {
			size_t index = m_PGFileImport.m_NewVarIndexSelected[col];
			size_t row_offset = 11;
			size_t DescriptorNo = row_offset-(GetNumberRows()-row);
			double value;
			switch (DescriptorNo) {
			case 0:
				m_PGFileImport.m_CurrentFileInfo.Des.base.Name[index]=txt;
				break;
			case 1:
				m_PGFileImport.m_CurrentFileInfo.Des.base.Unit[index]=txt;
				break;
			case 2:
				m_PGFileImport.m_CurrentFileInfo.Des.base.Id[index]=txt;
				break;
			case 3:
				m_PGFileImport.m_CurrentFileInfo.Des.base.Pos[index]=txt;
				break;
			case 4:
				m_PGFileImport.m_CurrentFileInfo.Des.addinfo[index].CountryCode=txt;
				break;
			case 5:
				m_PGFileImport.m_CurrentFileInfo.Des.addinfo[index].StationName=txt;
				break;
			case 6:
				value = FUtil::AtoFloat(txt);
				m_PGFileImport.m_CurrentFileInfo.Des.addinfo[index].Alt = value;
				break;
			case 7:
				value = FUtil::AtoFloat(txt);
				m_PGFileImport.m_CurrentFileInfo.Des.addinfo[index].Lat=value;
				break;
			case 8:
				value = FUtil::AtoFloat(txt);
				m_PGFileImport.m_CurrentFileInfo.Des.addinfo[index].Long=value;
				break;

			};
			return TRUE;
		}
	}
	else if (isheet == 2) {
		if (row == 2) {
			if (col == 0) {
				m_spatialPG.m_DestFileName = txt;
				m_DestFileSet[2] = true;
			}
			else if (col == 3)
				m_Latitude = FUtil::AtoFloat(txt);
			else if (col == 7)
				m_Longitude = FUtil::AtoFloat(txt);
			if (col == 3 || col == 7)
				QuickSetBackColor(col, row, m_ColorChanged);

		}
		return TRUE;
	}
	else if (isheet == 3) {
		if (row == 2) {
			if (col == 0) {
				m_spatialPG.m_DestFileName = txt;
				m_DestFileSet[3] = true;
			}
			else if (col == 1)
				m_Lat_Min = FUtil::AtoFloat(txt);
			else if (col == 2)
				m_Lat_Max = FUtil::AtoFloat(txt);
			else if (col == 3)
				m_GridLat = FUtil::AtoInt(txt);
			else if (col == 5)
				m_Long_Min = FUtil::AtoFloat(txt);
			else if (col == 6)
				m_Long_Max = FUtil::AtoFloat(txt);
			else if (col == 7)
				m_GridLong = FUtil::AtoInt(txt);
			if (col >= 0 && col<8)
				QuickSetBackColor(col, row, m_ColorChanged);
		}




		return TRUE;
	}
	else if (isheet == 4) {
		if (row == 2) {
			if (col == 0) {
				m_spatialPG.m_DestFileName = txt;
				m_DestFileSet[4] = true;
			}
			if (col >= 0 && col<8)
				QuickSetBackColor(col, row, m_ColorChanged);
		}
		if (col == 7) {

			m_MaxSubstituteDays = FUtil::AtoFloat(txt);

		}

		return TRUE;
	}
	else if (isheet == 5) {
		if (row == 2) {
			if (col == 0) {
				m_TimeAggregate.m_DestFileName = txt;
				m_DestFileSet[4] = true;
			}
			if (col >= 0 && col<8)
				QuickSetBackColor(col, row, m_ColorChanged);
		}


		return TRUE;
	}
	else if (isheet == 6) {
		if (row == 0) {
			CPG *pCPG;
			pCPG = (CPG*)m_pPGArchive->GetPGFile(m_IndexSource);
			if (col == 5 || col == 6) {
				long minut = PGUtil::MinutConv(txt);
				if (pCPG->GetLongTime(1) == string::npos) {
					if (!pCPG->ClearFile()) return FALSE;



				}
				long n = pCPG->GetRecord(minut, false);
				if (col == 5)
					m_ChangePeriod.m_StartOut = n;
				else
					m_ChangePeriod.m_EndOut = n;
			}


		}
		else if (row == 2) {
			if (col == 0) {
				m_ChangePeriod.m_DestFileName = txt;
				m_DestFileSet[4] = true;
			}
			if (col >= 0 && col<8)
				QuickSetBackColor(col, row, m_ColorChanged);
		}


		return TRUE;
	}
	else if (isheet == 9 || isheet == 10) {
		CPG *pCPG = GetCurrentPG_Pointer();

		if (ParV == m_PageRangeHint.param) {
			pCPG->SetRecordViewSize(FUtil::AtoInt(txt));
			pCPG->ResetRecordsToView();
			OnSheetSetup(isheet);
			return TRUE;

		}
		else if (ParV == m_ViewPageHint.param) {
			size_t p = FUtil::AtoInt(txt);
			pCPG->SetCurrentPeriodToView(p);
			OnSheetSetup(isheet);
			return TRUE;
		}


		if (!pCPG->m_WriteEnabled && !pCPG->GetCompleteRead()) {
			size_t actbuf = pCPG->GetPrevBuffer();
			pCPG->CloseFile();
			pCPG->ReOpen(true);
			if(actbuf!=string::npos)
				pCPG->SetActBuffer(actbuf);
			else
				pCPG->SetActBuffer(1);
		}

		QuickSetBackColor(col, row, m_ColorChanged);
		QuickSetText(col, row, txt);
		if (isheet == 10) {
			ChangeDescriptions(col, row, txt, pCPG);
			m_Changed_Description = true;
			OnSheetSetup(isheet);


		}
		else
			m_Changed_Data = true;
		return TRUE;
	}
	return FALSE;
}

void PGEditCells::ChangeDescriptions(int col, long row, string txt, CPG *pPG)
{

	size_t ivar, irep;
	ivar = row%(pPG->pg.Shape.NumVar)+1;
	irep = row /pPG->pg.Shape.NumVar+1;

	double DValue=MISSING;
	if (col > 10 && col < 14) {
		CUGCell cell;
		GetCell(col, row, &cell);
		DValue = cell.GetNumber();
	}
	switch (col) {
		case 2:	pPG->SetVarName(ivar, txt, irep); break;
		case 3:	pPG->SetVarUnit(ivar, txt, irep); break;
		case 4:	pPG->SetVarId(ivar, txt, irep); break;
		case 5:	pPG->SetVarPos(ivar, txt, irep); break;

		case 9:	pPG->SetVarCountry(ivar, txt, irep); break;
		case 10:	pPG->SetVarStation(ivar, txt, irep); break;

		case 11:	pPG->SetVarAlt(ivar, DValue, irep); break;
		case 12:	pPG->SetVarLat(ivar, DValue, irep); break;
		case 13:	pPG->SetVarLong(ivar, DValue, irep); break;
	}
}

