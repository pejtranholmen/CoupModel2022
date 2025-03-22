#pragma once
#include "DrawVariableLegends.h"
#include "../../Util.h"
#include "../ChartProperties\ChartProp.h"
#include "../PlotDesignInfo\PlotStorage.h"
#include "../../NewBase\Doc.h"
#include "../../Cug\ChartProperties\ChartCug.h"

DrawVariableLegends::DrawVariableLegends(void)
{
}

DrawVariableLegends::~DrawVariableLegends(void)
{
}
bool DrawVariableLegends::FindIdForVariablesToPlot(int Index_In_PlotArray, PLOTv p_info, int PGindex)
{
	int nselV;
	int ival, iv, ind;
	bool Band = false;

	int FileNo = p_info.ValFileNo;
	int SectionNo = p_info.PGSection;
	int irun;
	if (m_ChartInfo.ResidualSource)
		irun = p_info.Residual_RunNo + 1;
	else
		irun = 0;

	int ilocal = p_info.Index;
	PGDES in;

	bool hit;
	hit = false;
	vector<SIMB> vvp;
	bool SelBand = false;
	m_Label.st = "";
	/*if (itype == 18 || itype == 28) {

	}
	else if (itype>10 && itype<20) {
		m_pPG_Active = m_pSimDoc->m_PlotStorage.m_PG_ALLMULTI[FileNo];
		itype = itype - 10;


		m_pPG_Active->ReOpen();
	}
	else if (itype>20 && itype<30) {
		m_pPG_Active = m_pSimDoc->m_PlotStorage.m_PG_SEL_MULTI[FileNo];
		itype = itype - 20;

		SelBand = true;
		m_pPG_Active->ReOpen();

	}
	else if (itype>110 && itype<120) {

		SelBand = false;
		itype = itype - 10;
	}
	else if (itype>120 && itype<130) {

		SelBand = true;
		itype = itype - 20;

	}
	else if (irun == 140001) {

		SelBand = true;
	}
	*/
	if (SectionNo == 5000) {

		//	m_ChartInfo.NumVariableBands++;
	}
	int repit = 1;
	VALSUMv valsum;
	std::vector<size_t> rephits;
	//	if(p_info.MeanIndicator>0&&SectionNo<5000) m_ChartInfo.NumVariableBands++;
	switch (p_info.DataType) {
	case DATA_TYPES::PGDATA:
	case DATA_TYPES::ANYDATA:
		PGindex = ilocal;

		if (m_ChartInfo.PGRepitMode) {
			repit = m_pSimDoc->m_PlotStorage.GetPGRepitNo(Index_In_PlotArray);
			if (repit == 0) repit = 1;
			if (m_pPG_Active->Is_LatLong_FieldSensitive())
				m_Label.st = FUtil::STD_DtoAscii(m_pPG_Active->GetVarLat(p_info.PGFileIndex, repit)) + "N " + FUtil::STD_DtoAscii(m_pPG_Active->GetVarLong(p_info.PGFileIndex, repit)) + "E";
			else
				m_Label.st = "("+FUtil::STD_ItoAscii(repit)+")";
		}
		else {
			repit = 0;
			if(m_pPG_Active->Is_Id_FieldSensitive())
			m_Label.st = m_pPG_Active->GetVarId(p_info.PGFileIndex, repit) + m_pPG_Active->GetVarPos(p_info.PGFileIndex, repit);
		}

		m_Label.test = "";
		if (p_info.IndepAxis.Dependent == VAR_MIN) m_Label.test = "Minium of : ";
		else if (p_info.IndepAxis.Dependent == VAR_MEAN) m_Label.test = "Mean of : ";
		else if (p_info.IndepAxis.Dependent == VAR_MAX) m_Label.test = "Maximum of : ";
		
		m_Label.test += m_pPG_Active->GetVarName(p_info.PGFileIndex, repit);
		
		//if (repit>1) m_Label.test += m_pPG_Active->GetVarId(p_info.PGFileIndex, repit);
		
		if ((m_pProp->GetIndepFirst() == PGHOUR&&m_pProp->GetIndepSecond()== PGDAY) ||
			(m_pProp->GetIndepSecond()== PGHOUR&&m_pProp->GetIndepFirst() == PGDAY) && m_pPG_Active->GetNumberofYears()>1) {
			if (m_ChartInfo.YearRepresentation == 0) 	m_Label.test += " Mean of All Years ";
			else if (m_ChartInfo.YearRepresentation>0 && m_ChartInfo.YearRepresentation<m_pPG_Active->GetNumberofYears())
				m_Label.test += MFC_Util::ItoAscii(m_pPG_Active->GetYear(m_pPG_Active->GetYearIndex(m_ChartInfo.YearRepresentation))) + " ";
		}
		else if (((m_pProp->GetIndepFirst() == PGYEAR&&m_pProp->GetIndepSecond() == PGDAY) ||
			(m_pProp->GetIndepFirst() == PGDAY&&m_pProp->GetIndepSecond() == PGYEAR))&& m_ChartInfo.WithinDayRepresentation > 1) {		
				if (m_pProp->m_HourValueRepresentation == 0) m_Label.test += "Mean of All Hours ";
				else m_Label.test += _T("(") + MFC_Util::ItoAscii(m_pProp->m_HourValueRepresentation) + " of "
					+ MFC_Util::ItoAscii(m_ChartInfo.WithinDayRepresentation) + ") ";
		}


	
		m_Label.unit = m_pPG_Active->GetVarUnit(p_info.PGFileIndex, repit);
		FUtil::trim(m_Label.unit);
		if (m_pProp->IsFirstIndepSet()) {
			m_Label.xtitle = m_pProp->GetFirstIndepString();
		}

		if (m_pProp->IsSecondIndepSet()) {
			m_Label.ytitle1 = m_pProp->GetSecondIndepString();
		}
		break;
	case DATA_TYPES::SIM_X:
	case DATA_TYPES::SIM_T:
	case DATA_TYPES::SIM_G:
	case DATA_TYPES::SIM_D:
		m_Label.st = m_pPG_Active->GetVarId(PGindex, m_pSimDoc->m_PlotStorage.GetPGRepitNo(Index_In_PlotArray)) + m_pPG_Active->GetVarPos(PGindex);
		FUtil::trim(m_Label.st);

		m_Label.test = m_pPG_Active->GetVarName(PGindex);
		m_Label.unit = m_pPG_Active->GetVarUnit(PGindex);
		if (p_info.RepitDataType) {
			m_Label.st += " ";
			if (p_info.EnsembleType == ENSEMBLE_TYPE::NO) {
				if(m_Label.st.size()==0) m_Label.st += FUtil::STD_ItoAscii(m_pSimDoc->m_PlotStorage.GetPGRepitNo(Index_In_PlotArray) + 1) + "(" + FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_TotalNumberofRuns()) + ")";
			}
			else if (SelBand || p_info.EnsembleType == ENSEMBLE_TYPE::ACCEPTED)
				m_Label.st += "Mean of Accepted " + FUtil::STD_ItoAscii(int(m_pSimDoc->m_MStorage.GetNumAccepted())) + "(" + FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_TotalNumberofRuns()) + ")";
			else
				m_Label.st += "Mean of All (" +  FUtil::STD_ItoAscii(m_pSimDoc->MR_Get_TotalNumberofRuns()) + ")";
			FUtil::trim(m_Label.st);
		}
		break;
	case DATA_TYPES::MEASURED_VAL:
		if (p_info.index_cycle != string::npos) {
			irun = p_info.pg_start_pos + 1;
			PGindex += p_info.index_cycle;
		}
		else {
			irun = p_info.Residual_RunNo + 1;
			auto cycle_pair = m_pSimDoc->StartValueofMatchingEnsembleInDim1(p_info.EnsembleNo, p_info.pPG_MeasValFile, p_info.Index);
			if (cycle_pair.first == string::npos) irun = cycle_pair.first + 1;
		}
		rephits = m_pSimDoc->ActualRepetitionInMeasFile(p_info.EnsembleNo, p_info.pPG_MeasValFile, p_info.Index, p_info.Residual_RunNo);
		if (rephits.size() == 0) irun = 0;
		else
			irun = rephits.front();



		m_Label.st = "Measured " + p_info.pPG_MeasValFile->GetVarId(PGindex, irun);
		m_Label.test = p_info.pPG_MeasValFile->GetVarName(PGindex, irun);
		m_Label.unit = p_info.pPG_MeasValFile->GetVarUnit(PGindex, irun);

		if (p_info.pBase == nullptr) {

			m_Label.test = m_pPG_Active->GetVarName(PGindex);
			m_Label.unit = m_pPG_Active->GetVarUnit(PGindex);


		}
		else {
			m_Label.test = p_info.pBase->GetName();
			FUtil::trim(m_Label.test);
			m_Label.unit = p_info.pBase->GetUnit();
			FUtil::trim(m_Label.unit);
			m_Label.ytitle1 = p_info.pBase->GetName();
			m_Label.ytitle1 += " (";
			m_Label.ytitle1 += p_info.pBase->GetUnit();
			m_Label.unit1 = p_info.pBase->GetUnit();
			m_Label.ytitle1 += ")";
		}
		break;
	case DATA_TYPES::SIM_VAL:

		PGindex = m_pSimDoc->m_PlotStorage.GetVar_Index(Index_In_PlotArray);
		//GetNumOutPuts()+ilocal;
		if (p_info.EnsembleType == ENSEMBLE_TYPE::NO) {
			if (irun >= 1 && irun<100000) {
				if (m_pUtil->m_ResidualPlot) {
					int LogLiVersion = m_pSimDoc->m_PlotStorage.GetLogLiVersion(Index_In_PlotArray);
					if (p_info.Residual_RunNo == 0) {
						if (LogLiVersion == 2)
							m_Label.st = "Residuals MRun: " + FUtil::STD_ItoAscii(irun);
						else
							m_Label.st = "Simulated MRun: " + FUtil::STD_ItoAscii(irun);
					}
					else {
							m_Label.st = FUtil::STD_ItoAscii(irun);
					}


				}
				else
					m_Label.st = "MRun (" + FUtil::STD_ItoAscii(irun) + ")";

			}
			else if (irun == 120001 || irun == 110002)
				m_Label.st = "Measured " + m_pPG_Active->GetVarId(PGindex);
			else if (irun == 110001)
				if (m_pUtil->m_ResidualPlot == 2)
					m_Label.st = "Simulated (LogLikelihood)";
				else if (m_pUtil->m_ResidualPlot == 1)
					m_Label.st = "Simulated (Residual)";
				else
					m_Label.st = "Simulated";
			else {
				m_Label.st = "";
				m_Label.st = m_pPG_Active->GetVarId(PGindex) + m_pPG_Active->GetVarPos(PGindex);
			}
			if ((m_pProp->GetIndepFirst() == PGHOUR&&m_pProp->GetIndepSecond()== PGDAY) ||
				(m_pProp->GetIndepSecond()== PGHOUR&&m_pProp->GetIndepFirst() == PGDAY) && m_pPG_Active->GetNumberofYears()>1) {
				if (m_ChartInfo.YearRepresentation == 0) 	m_Label.test += " Mean of All Years ";
				else if (m_ChartInfo.YearRepresentation>0 && m_ChartInfo.YearRepresentation<m_pPG_Active->GetNumberofYears())
					m_Label.test += MFC_Util::ItoAscii(m_pPG_Active->GetYear(m_pPG_Active->GetYearIndex(m_ChartInfo.YearRepresentation))) + " ";
			}
			else if (((m_pProp->GetIndepFirst() == PGYEAR&&m_pProp->GetIndepSecond()== PGDAY) ||
				(m_pProp->GetIndepFirst() == PGDAY&&m_pProp->GetIndepSecond()== PGYEAR)) && m_ChartInfo.WithinDayRepresentation > 1) {
				if (m_pProp->m_HourValueRepresentation == 0) m_Label.test += "Mean of All Hours ";
				else m_Label.test += _T("(") + MFC_Util::ItoAscii(m_pProp->m_HourValueRepresentation) + " of "
					+ MFC_Util::ItoAscii(m_ChartInfo.WithinDayRepresentation) + ") ";
			}
			else if (p_info.index_cycle != string::npos) {
				//m_Label.st += "var :";
				//m_Label.st += FUtil::STD_ItoAscii(p_info.index_cycle + 1);
			}
		}
		else if (p_info.EnsembleType > ENSEMBLE_TYPE::NO) {
			int LogLiVersion = m_pSimDoc->m_PlotStorage.GetLogLiVersion(Index_In_PlotArray);

			if (SelBand) {
				if (LogLiVersion != 3)
					m_Label.st = "Mean of Residuals, Accepted ";
				else
					m_Label.st = "Mean of Accepted ";
				if(p_info.EnsembleNo>=0&& p_info.EnsembleNo< m_pSimDoc->m_MStorage.m_AcceptedIdString.size())
				m_Label.st = m_pSimDoc->m_MStorage.m_AcceptedIdString[p_info.EnsembleNo];
			}
			else {
				if (LogLiVersion != 3)
					m_Label.st = "Mean of Residuals, All Simulations";
				else
					m_Label.st = "Mean of Accepted - ";
				if (p_info.EnsembleNo>=0 && p_info.EnsembleNo < m_pSimDoc->m_MStorage.m_AcceptedIdString.size())
					m_Label.st = m_pSimDoc->m_MStorage.m_AcceptedIdString[p_info.EnsembleNo];

			}
		}
		else {
			m_Label.st = m_pPG_Active->GetVarId(PGindex) + m_pPG_Active->GetVarPos(PGindex);


		}

		if (p_info.pBase == nullptr) {

			m_Label.test = m_pPG_Active->GetVarName(PGindex);
			m_Label.unit = m_pPG_Active->GetVarUnit(PGindex);


		}
		else {
			m_Label.test = p_info.pBase->GetName();
			FUtil::trim(m_Label.test);
			m_Label.unit = p_info.pBase->GetUnit();
			FUtil::trim(m_Label.unit);
			m_Label.ytitle1 = p_info.pBase->GetName();
			m_Label.ytitle1 += " (";
			m_Label.ytitle1 += p_info.pBase->GetUnit();
			m_Label.unit1 = p_info.pBase->GetUnit();
			m_Label.ytitle1 += ")";
		}
		break;


		case DATA_TYPES::SIM_PERFORMANCE: // Validation Variables
		nselV = m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
		//if (m_pSimDoc->m_MStorage.GetCyclicNumber() > 1&& m_pSimDoc->m_MStorage.GetCyclicNumber()!=string::npos)
		//	nselV/= m_pSimDoc->m_MStorage.GetCyclicNumber();
		ival = ilocal%nselV;
		m_Label.st = MFC_Util::ItoAscii(ival + 1) + ":";

		m_Label.test = m_pSimDoc->m_ValidationData.GetValName(ival).c_str();
		if (m_pSimDoc->m_ValidationData.GetValLocalIndex(ival)>-1) m_Label.test += "(" + FUtil::STD_ItoAscii(m_pSimDoc->m_ValidationData.GetValLocalIndex(ival) + 1) + ")";
		m_Label.st += m_Label.test;



		switch (ilocal / nselV) {
		case 0:
			m_Label.test += " Coefficient of determination";
			break;
		case 1:
			m_Label.test += " Intercept";
			break;
		case 2:
			m_Label.test += " Slope";
			break;
		case 3:
			m_Label.test += " Mean Error (ME)";
			break;
		case 4:
			m_Label.test += " Root Mean Square Error (RMSE)";
			break;
		case 5:
			m_Label.test += " Log Likelihood";
			break;
		case 6:
			m_Label.test += " NSE ";
		}
		if (SectionNo>0)
			m_Label.st += ", Sub Period:" + FUtil::STD_ItoAscii(SectionNo);



		if (m_pProp->IsFirstIndepSet()) {
			m_Label.xtitle = m_pProp->GetFirstIndepString();
		}
		if (m_pProp->IsSecondIndepSet()) {
			m_Label.ytitle1 = m_pProp->GetSecondIndepString();
		}
		break;
		case DATA_TYPES::SIM_SUMVAR: //case 99: case 98:// SumVar Validation variables
		
		nselV = m_pSimDoc->m_ValidationData.GetNumSumVarVariables();
		if (p_info.Performance_Var==SUM_PERFORMANCE::SUM_OBS) {

			m_Label.st = m_pSimDoc->m_ValSum_Array[ilocal].IDVarName;
			m_Label.test = "Measured";
		}
	/*	else if (itype == 98) {
			m_Label.st = m_pSimDoc->m_ValSum_Array[ilocal].Name;


			if (m_pSimDoc->m_ValSum_Array[ilocal].SimValueSource == SIMSUMSOURCE::MULTIFILESTAT) {
				switch (m_pSimDoc->m_ValSum_Array[ilocal].SumVarType % 6) {
				case 0:
					m_Label.st += " - Initial";
					break;
				case 1:
					m_Label.st += " - Final";
					break;
				case 2:
					m_Label.st += " - Min";
					break;
				case 3:
					m_Label.st += " - Max";
					break;
				case 4:
					m_Label.st += "- Mean";
					break;
				case 5:
					m_Label.st += " - Accumulated";
					break;
				}
				m_Label.test = "Output Statistics";
				SIMB sim = m_pSimDoc->m_ValidationData.GetSimPointer_SV(ilocal);
				if (sim.pBase != nullptr) {
					m_Label.unit = sim.pBase->GetUnit();
				}

			}
			if (m_pSimDoc->m_ValSum_Array[ilocal].SimValueSource == SIMSUMSOURCE(1))
				m_Label.test = "Residual";
			if (m_pSimDoc->m_ValSum_Array[ilocal].SimValueSource == SIMSUMSOURCE(2)) {
				m_Label.unit = m_pSimDoc->m_PG_MultiOutputFile.GetVarUnit(m_pSimDoc->m_ValSum_Array[ilocal].Sim_Value_Residual_Index + 1);
				m_Label.test = "Simulated";


			}


		}*/
		else {

			m_Label.st = MFC_Util::ItoAscii(p_info.Index + 1) + ":";
			ival = p_info.Index;
			m_Label.test = m_pSimDoc->m_ValidationData.GetValSumVarName(p_info.Index).c_str();
			iv = m_pSimDoc->m_ValidationData.GetValSumVarLocalIndex(ival);
			if (iv > -1) m_Label.test += "(" + FUtil::STD_ItoAscii(iv + 1) + ")";
			m_Label.st += m_Label.test;


			switch (p_info.Performance_Var) {
			case SUM_PERFORMANCE::SUM_R2:
				m_Label.test = " R2";
				break;
			case SUM_PERFORMANCE::SUM_INTERCEPT:
				m_Label.test = "Intercept";
				break;
			case SUM_PERFORMANCE::SUM_SLOPE:
				m_Label.test = "Slope";
				break;
			case SUM_PERFORMANCE::SUM_ME:
				m_Label.test = "Error (E)";
				break;
			case SUM_PERFORMANCE::SUM_RMSE:
				m_Label.test = "Root Square Error (RSE)";
				break;
			case SUM_PERFORMANCE::SUM_LOGLI:
				m_Label.test = "Log Likelihood";
				break;
			case SUM_PERFORMANCE::SUM_NSE:
				m_Label.test = "NSE-R2";
				break;
			}
			if (m_pSimDoc->m_ValSum_Array[ival].FlexSumVar) {
				m_Label.test += " -";
				m_Label.test += m_pSimDoc->m_ValSum_Array[ival].IDVarName;
			}
		}
		if (m_pUtil->m_AutoShiftFirstIndep) {
		/*	if (itype == 98 || itype == 99) {
				if (m_pSimDoc->m_ValSum_Array[ival].FlexSumVar)
					m_Label.xtitle = m_pSimDoc->m_ValSum_Array[ival].IDVarName;

			}
			else {
				if (m_pSimDoc->m_ValSum_Array[ival].FlexSumVar)
					m_Label.xtitle = m_pSimDoc->m_ValSum_Array[ival].IDVarName;

			}*/


		}
		else if (m_pProp->IsFirstIndepSet()) {
			m_Label.xtitle = m_pProp->GetFirstIndepString();
		}
		if (m_pProp->IsSecondIndepSet()) {
			m_Label.ytitle1 = m_pProp->GetSecondIndepString();
		}

		break;
	case DATA_TYPES::SIM_PAR: // MultiRun Parameters Variables
		ind = m_pSimDoc->m_P_MR_Array.size();

		if (p_info.pBase != nullptr) {
			m_Label.st = p_info.pBase->GetName();
			m_Label.unit = p_info.pBase->GetUnit();
			m_Label.test = p_info.pBase->GetName();
			if (p_info.pBase->Is_PTable()) {
				m_Label.test += "(" + FUtil::STD_ItoAscii(p_info.TabIndex + 1) + ")";
				m_Label.st= m_Label.test;
			}

		}

		if (m_pProp->IsFirstIndepSet()) {
			m_Label.xtitle = m_pProp->GetFirstIndepString();
		}
		if (m_pProp->IsSecondIndepSet())
			m_Label.ytitle1 = m_pProp->GetSecondIndepString();
		else
			m_Label.ytitle1 = m_Label.test;

		if (m_pProp->m_ChartType == CHART_TYPES::FREQUENCE) {
			m_Label.ytitle1 = "Counts (#)";
			m_Label.xtitle = m_Label.test;
		}
		else if (m_pProp->m_ChartType == CHART_TYPES::CUM_FREQUENCE) {
			m_Label.ytitle1 = "Fraction of Total Numbers (-)";
			m_Label.xtitle = m_Label.test;
		}


		int nAddVar; nAddVar = p_info.Index - ind;

		switch (nAddVar) {
		case 0:

			break;
		case 1:
			m_Label.test = "Log Likelihood";
			m_Label.st = "Time Serie Variables";
			m_Label.unit = "-";
			break;
		case 2:
			m_Label.test = "Log Likelihood";
			m_Label.st = "Summary Variables";
			m_Label.unit = "-";
			break;
		case 3:
			m_Label.test = "Log Likelihood";
			m_Label.st = "Total";
			m_Label.unit = "-";
			break;
		case 4:
			m_Label.test = "Run Number";
			m_Label.st = "MAP";
			m_Label.unit = "-";
			break;
		case 5:
			m_Label.test = "Acc Simulation Time";
			m_Label.st = "";
			m_Label.unit = "SEC";
			break;
		case 6:
			m_Label.test = "n number points";
			m_Label.st = "";
			m_Label.unit = "-";
			break;
		case 7:
			m_Label.test = "Sum Square Errors";
			m_Label.st = "";
			m_Label.unit = "";
			break;
		case 8:
			m_Label.test = "Akaike Criterion";
			m_Label.st = "Post Period";
			m_Label.unit = "-";
			break;
		case 9:
			m_Label.test = "Bayesian Criterion";
			m_Label.st = "Inter Period";
			m_Label.unit = "-";
			break;
		}
		if (m_pProp->m_ChartType == CHART_TYPES::FREQUENCE) {
			m_Label.ytitle1 = "Counts (#)";
			m_Label.xtitle = m_Label.test;
			m_Label.xtitle += " (" + m_Label.unit + ")";
			m_Label.unit = "";
		}
		else {
			m_Label.unit1 = m_Label.unit;
		}
		break;
	case DATA_TYPES::SIM_OUTSUM: // Output Variables
		in = m_pSimDoc->GetSumOutPutDescription(ilocal);
		m_Label.st = in.Name.c_str();
		m_Label.st += in.Index;
		m_Label.unit = in.Unit;
		switch (ilocal % 6) {
		case 0:
			m_Label.st += "Initial";
			break;
		case 1:
			m_Label.st += "Final";
			break;
		case 2:
			m_Label.st += "Min";
			break;
		case 3:
			m_Label.st += "Max";
			break;
		case 4:
			m_Label.st += " Mean";
			break;
		case 5:
			m_Label.st += "Accumulated";
			break;
		}
		m_Label.test = p_info.pBase->GetName();
		m_Label.unit = p_info.pBase->GetUnit();
		if (m_pProp->IsFirstIndepSet()) {
			m_Label.xtitle = m_pProp->GetFirstIndepString();
		}
		if (m_pProp->IsSecondIndepSet())
			m_Label.ytitle1 = m_pProp->GetSecondIndepString();
		else
			m_Label.ytitle1 = m_Label.test;


		if (m_pProp->m_ChartType == CHART_TYPES::FREQUENCE) {
			m_Label.ytitle1 = "Counts (#)";
			m_Label.xtitle = m_Label.test;
			m_Label.xtitle += " (" + m_Label.unit + ")";
		}


		break;
	case DATA_TYPES::SIM_OUTSUM_Y: // Output Variables
		ind = 0;
		hit = false;

		in = m_pSimDoc->GetSumOutPutDescription(ilocal);
		m_Label.ytitle1 = in.Name;
		m_Label.ytitle1 += in.Index;
		m_Label.unit = in.Unit;
		switch (ilocal % 6) {
		case 0:
			m_Label.ytitle1 += "Initial";
			break;
		case 1:
			m_Label.ytitle1 += "Final";
			break;
		case 2:
			m_Label.ytitle1 += "Min";
			break;
		case 3:
			m_Label.ytitle1 += "Max";
			break;
		case 4:
			m_Label.ytitle1 += "Mean";
			break;
		case 5:
			m_Label.ytitle1 += "Accumulated";
			break;
		}
		break;
	case DATA_TYPES::SIM_OUTSUM_X:
		in = m_pSimDoc->GetSumOutPutDescription(ilocal);
		m_Label.xtitle = in.Name;
		m_Label.xtitle += in.Index;
		m_Label.unit = in.Unit;
		switch (ilocal % 6) {
		case 0:
			m_Label.xtitle += "Initial";
			break;
		case 1:
			m_Label.xtitle += "Final";
			break;
		case 2:
			m_Label.xtitle += "Min";
			break;
		case 3:
			m_Label.xtitle += "Max";
			break;
		case 4:
			m_Label.xtitle += "Mean";
			break;
		case 5:
			m_Label.xtitle += "Accumulated";
			break;
		}

		break;
	}
	if (SectionNo>20000 && SectionNo<21000 || (SectionNo>0 && SectionNo<1000) && m_pPG_Active != nullptr) {
		long irec = m_pPG_Active->GetIndexInSection(SectionNo % 20000);
		if (irec>0)
			m_Label.st = FUtil::STD_ItoAscii(m_pPG_Active->GetYear(irec));
	}
	else if (SectionNo % 20000 == 5000 && m_pPG_Active != nullptr) {
		m_Label.st = "Mean of all years";
		if (m_ChartInfo.PGRepitMode) {
			string txt;
			if (ilocal != 0) {
				txt = FUtil::STD_DtoAscii(m_pPG_Active->GetVarLat(ilocal, repit)) + "/";
				txt += FUtil::STD_DtoAscii(m_pPG_Active->GetVarLong(ilocal, repit));
				if (txt.size()>0) {
					m_Label.st += " - Lat/Long";
					m_Label.st += txt;
				}
				else
					m_Label.st += " - (" + FUtil::STD_ItoAscii(repit) + ")";
			}
		}



	}
	else if (SectionNo >= 6000 && m_pPG_Active != nullptr) {
		int imon = SectionNo % 6000;
		string m;
		switch (imon) {
		case 0:
			m = "Jan";
			break;
		case 1:
			m = "Feb";
			break;
		case 2:
			m = "March";
			break;
		case 3:
			m = "April";
			break;
		case 4:
			m = "May";
			break;
		case 5:
			m = "June";
			break;
		case 6:
			m = "July";
			break;
		case 7:
			m = "Aug";
			break;
		case 8:
			m = "Sep";
			break;
		case 9:
			m = "Oct";
			break;
		case 10:
			m = "Nov";
			break;
		case 11:
			m = "Dec";
			break;
		case 12:
			m = "Whole Year";
			break;

		}

		m_Label.st = m;
	}
	FUtil::trim(m_Label.unit);
	if (m_pProp->m_ChartType == CHART_TYPES::FREQUENCE)
		m_Label.ytitle1 = "Counts (#)";
	else if (m_pProp->m_ChartType == CHART_TYPES::CUM_FREQUENCE )
		m_Label.ytitle1 = "Fraction of Total Numbers (-)";
	
	if (m_pProp->m_ChartType == CHART_TYPES::FREQUENCE || m_pProp->m_ChartType == CHART_TYPES::CUM_FREQUENCE) {
		m_Label.xtitle = m_Label.test + "(" + m_Label.unit + ")";
		m_Label.unit = "";
	}


	FUtil::trim(m_Label.st);
	return true;
}
size_t DrawVariableLegends::GetNumberOfEnsembles()
{
	size_t numensembles = 1;
	if (m_pUtil->m_MultiRunPlot||m_pSimDoc->IsMultiSimulation()) {
		numensembles = m_pSimDoc->m_MStorage.m_AcceptedRuns.size();
		if (m_pSimDoc->m_ValidationData.IsSomeValidationMade())
			return numensembles;
	}
	else
		numensembles = 0;

	return 0;
}
string DrawVariableLegends::GetEnsembleIdString(size_t index)
{
	string legend;
	size_t numensembles = GetNumberOfEnsembles();
	if(m_pUtil->m_MultiRunPlot&&numensembles>0) {
		numensembles = m_pSimDoc->m_MStorage.m_AcceptedRuns.size();
		for (size_t i = 0; i < numensembles; i++) {
			string legend;
			legend = m_pSimDoc->m_MStorage.m_AcceptedIdString[index];

		}
	}
	else
		legend = "";

	return legend;
}
void DrawVariableLegends::SetLegendInfo()
{
	size_t index_start = 0;

	for (size_t ii = 0; ii < m_ChartOrigInfo.size() - 1; ++ii) {
		index_start += m_ChartOrigInfo[ii].legends.size();
	}
	size_t index_chart = m_ChartOrigInfo.size() - 1;

	if (m_pUtil->m_RemoveLegends == 2) {
		for (size_t i = 0; i < m_DataSets.size(); i++) 
			m_DataSets[i]->setDataName("");
	}
	else {
		for (size_t i = 0; i < m_DataSets.size(); i++) {
			if (m_pProp->isYLegendChanged(i + index_start)) {
				m_Label.Legendtxt = m_pProp->GetLegend(i + index_start);
				m_DataSets[i]->setDataName(m_Label.Legendtxt.c_str());
			}
			/*else if (m_pSimDoc != nullptr) {  // Simulation File
					if (!(m_pSimDoc->m_MStorage.m_AcceptedIdString.size() > index_valid)) return;
					string legend = "";

					while (m_pSimDoc->m_MStorage.m_AcceptedIdString.size() > index_valid&&!m_pSimDoc->m_MStorage.IsToBePlotted(index_valid)) {
						index_valid++;
					}

				if (m_pSimDoc->m_MStorage.m_AcceptedIdString.size()>index_valid)
					legend = m_pSimDoc->m_MStorage.m_AcceptedIdString[index_valid];
					index_valid++;

				m_DataSets[i]->setDataName(legend.c_str());
				m_pProp->SetLegend1(ivar*TotNumPrior + i, legend);

			} */
			else {// PG File
				if (m_ChartOrigInfo[index_chart].legends.size() > i) {
					auto koll= m_ChartOrigInfo[index_chart].legends[i];
					if (koll.size() > 0) {
						m_DataSets[i]->setDataName(m_ChartOrigInfo[index_chart].legends[i].c_str());
						m_pProp->SetLegend1(i + index_start, m_ChartOrigInfo[index_chart].legends[i]);
					}
				}
			}
		}
	}
}

bool DrawVariableLegends::SetLineWidth(DataSet * pDataSet)
{
	if (pDataSet != nullptr) {

		pDataSet->setLineWidth(m_pUtil->m_LineWidthPoint + 1);

		return true;
	}
	else
		return false;
}

bool DrawVariableLegends::SetLineWidth(int index, DataSet * pDataSet)
{
	if (pDataSet != nullptr) {
		int iwid = m_pUtil->GetSpecificLineWidth(index);
		if(iwid>0 )	pDataSet->setLineWidth(iwid*3);
		return true;
	}
	else
		return false;
}

bool DrawVariableLegends::SetLineColors(int index, DataSet * pDataSet)
{
	if (pDataSet != nullptr) {

		m_Color.ivar = m_pUtil->GetHColorLine(index);

		if (m_pUtil->GetLineTypeIndex(index) > 0) {
			switch (m_pUtil->GetLineTypeIndex(index)) {
			case 1:
				m_Color.ivar = pXY_Chart->dashLineColor(m_Color.ivar, DashLine);
				break;
			case 2:
				m_Color.ivar = pXY_Chart->dashLineColor(m_Color.ivar, DotLine);
				break;
			case 3:
				m_Color.ivar = pXY_Chart->dashLineColor(m_Color.ivar, DotDashLine);
				break;
			case 4:
				m_Color.ivar = pXY_Chart->dashLineColor(m_Color.ivar, AltDashLine);
				break;
			}

			pDataSet->setDataColor(m_Color.ivar);

		}
		return true;
	}
	else 
		return false;
}

bool DrawVariableLegends::SetSymbols(int index, DataSet * pDataSet)
{
	if (pDataSet != nullptr) {

		pDataSet->setDataSymbol(m_pUtil->m_SymbolCode[index%11], m_pUtil->GetSpecificSymbolSize(index) * 3, m_pUtil->m_SymbolOpenCode[index%11]);
		return true;
	}
	else
		return false;
}
