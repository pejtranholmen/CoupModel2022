#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include "..\..\Util.h"

#include "PGCAlcActions.h"
#include "..\..\PG\CalculateVariable.h"


PGCalcActions::PGCalcActions(void)
{
	
	
}
PGCalcActions::~PGCalcActions(void)
{
}
void PGCalcActions::Make_PG_Calc(bool newvar, bool allrep, size_t SeqNo, size_t varin, size_t repin)
{
	int expression = 1;
	int count = 0;
	int prevnum;
	string str;
	MFC_Util m;

	if (!m_pPG->GetCompleteRead()) {
		m_pPG->m_WriteEnabled = false;
		m_pPG->MakeFileToEnableWriting(true);
	}
	if (m_pPG->GetNumToBeCalc() <= 0) {
		m_pPG->SetCalcVar(SeqNo);
		if (!newvar) {
			if (!allrep) {
				m_pPG->SetCalcVarNew(SeqNo + 1);
				m_CalcAllRepetitions = false;
			}
			else {
				m_pPG->SetCalcVarNew(SeqNo + 1);
				size_t iv = (SeqNo - 1) % m_pPG->GetNumVariables();
				for (size_t i = 0; i < m_pPG->GetNumRepititions(); i++)
					m_pPG->SetToBeCalc(i*m_pPG->GetNumVariables() + iv, true);
				m_CalcAllRepetitions = true;
			}
		}
		else {

			if (!allrep) {
				m_pPG->SetCalcVarNew(SeqNo);
				m_CalcAllRepetitions = false;
			}
			else {
				m_pPG->SetCalcVarNew(SeqNo);
				size_t iv = (SeqNo - 1) % m_pPG->GetNumVariables();
				for (size_t i = 0; i < m_pPG->GetNumRepititions(); i++)
					m_pPG->SetToBeCalc(i*m_pPG->GetNumVariables() + iv, true);
				m_CalcAllRepetitions = true;
			}
		}
	}
	if (m_pPG->GetNumToBeCalc() <= 0) {
		m_pPG->SetToBeCalc(SeqNo - 1, true);
	}
	prevnum = m_pPG->GetNumVariables();
	m_pPG->SetInfoMode();


	if (m_Changed_Description)
		OnWriteGridDescriptionToFile(m_pPG);
	if (m_Changed_Data)
		OnWriteGridDataToFile(m_pPG);


	long line = 0;
	bool jump = false;
	long startrep = 0;
	for (int irep = 0; irep < m_pPG->GetNumRepititions(); irep++) {
		for (int ii = 0; ii < prevnum; ii++) {
			if (m_pPG->GetToBeCalc(line)) {
				if ((m_Data&&newvar) || newvar) {
					if (irep == startrep || !jump) {
						m_pPG->AddNewVariable(ii + 1 + count);
						m_pPG->SetReplace(TRUE);
						m_NewVariable = TRUE;
						count++;
						startrep = irep;
						jump = true;
					}
				}
				else {
					m_pPG->SetReplace(FALSE);
					m_NewVariable = FALSE;
				}
			}
			line++;
		}
	}
	m_pPG->SetVarFunctionType(1);

	CCalculateVariable dlg;
	dlg.Init(m_pPG);

	if (dlg.DoModal() == IDOK) {
		if (OnNewCalculateVar()) {
			long line = 0;
			for (int irep = 0; irep < m_pPG->GetNumRepititions(); irep++) {
				for (int ii = 0; ii < m_pPG->GetNumVariables(); ii++) {
					if (m_pPG->GetToBeCalc(line)) {
							if (m_pPG->GetNumRepititions() > 1) 
								m.SetPaneMessage("Calculation of new var no " + FUtil::STD_ItoAscii(ii + 1) + " Rep no " + FUtil::STD_ItoAscii(irep + 1) + " successfully done");							
							else 
								m.SetPaneMessage("Calculation of new var no " + FUtil::STD_ItoAscii(ii + 1) + " successfully done");					
						m_pPG->SetToBeCalc(line, FALSE);
					}
					line++;
				}
			}
			m_Changed_Data = false;
			m_Changed_Description = false;
		}
		else {
			MFC_Util::MessageBox("Calculation of new var  failed");
			return;
		}
		m_pPG->CloseFile();
	}
	else {
		if (newvar) {
			m_pPG->RemoveVariable(SeqNo);
		}
	}
}
BOOL PGCalcActions::OnNewCalculateVar()
{
	CPG *pCPG = m_pPG;
	MFC_Util m;
	int expression, expression_type;
	int var;
	int var_in, var_out;
	BOOL jump;
	jump = FALSE;
	bool status = true;
	if (pCPG == nullptr) return TRUE;
	long m_NumRecords = m_row;
	expression = pCPG->GetVarFunction();
	var = pCPG->GetCalcVarNew();

	auto WriteResultofActualBuftoFile = [&](int irep, int nbuf) {
		if (!pCPG->GetCompleteRead()) {
			if (pCPG->GetNumRepititions()>1)
				status = pCPG->WriteNewBuffer(irep + 1);
			else
				status = pCPG->WriteNewBuffer(nbuf);

			if (!status) {
				m.SetPaneMessage("Writing of Results to File Failed");
				return FALSE;
			}
		}
		return TRUE;
	};
	auto MakeVariableCalc = [&](int ii, int irep) {
		if (pCPG->GetToBeCalc(ii + irep*pCPG->GetNumVariables()) && (!jump || m_CalcAllRepetitions)) {
			var_in = ii + 1;
			var_out = ii + 1;
			pCPG->SetCalcVarNew(var_out);

			if (pCPG->GetReplaceMode()) {
				var_out = ii + 2;
				pCPG->SetCalcVarNew(var_out);
				jump = TRUE;
			}
			for (size_t nbuf = 1; nbuf <= pCPG->GetNumBuffers(); nbuf++) {
				if (pCPG->GetNumRepititions() > 1&&nbuf>1) pCPG->SetActBuffer(nbuf);
				if (expression_type == 1) {
					if (expression == 1) pCPG->CalcVarScaling(var_in, var_out, irep+1);
					else if (expression == 2)pCPG->CalcVarMissing(var_in, var_out);
					else if (expression == 3) pCPG->CalcVarMissing(var_in, var_out);
					else if (expression == 4) pCPG->CalcVarMissingSubstitute(var_in, var_out, false, nbuf);
					else if (expression == 5) pCPG->CalcVarMissingSubstitute(var_in, var_out);
					else if (expression == 6) pCPG->CalcVarSmoothing(var_in, var_out);
					else if (expression == 7) pCPG->CalcVarDifference(var_in, var_out);
				}
				else if (expression_type == 2) {
					if (expression == 1) pCPG->CalcVarMean(var);
					else if (expression == 2) pCPG->CalcVarSum(var);
					else if (expression == 3) pCPG->CalcVarMultiply(var);
					else if (expression == 4) pCPG->CalcVarDivide(var);
					else if (expression == 5) {
						pCPG->SetVarFunction(4);// To avoid linear interpolation
						pCPG->CalcVarMissingSubstitute(var_in, var_out, true);
					}
				}
				else if (expression_type == 3) {
					if (expression >= 1 && expression <= 4) pCPG->CalcTimeToVar(var);

				}
				else
					jump = FALSE;
				WriteResultofActualBuftoFile(irep, nbuf);
			}

		}
	};
	auto MakeTimeCalc = [&](int irep) {
		for (size_t nbuf = 1; nbuf <= pCPG->GetNumBuffers(); nbuf++) {
			if (pCPG->GetNumRepititions() <= 1) pCPG->SetActBuffer(nbuf);
			if (expression == 6) {
				if (pCPG->CalcVarTimeDerivate())
					OnPGHeaderUpdate(pCPG->GetNumVariables() + 1, pCPG->GetNumRecords(), TRUE);
			}
			else if (expression == 4) {
				pCPG->CalcTimeChange(irep + 1);
			}
			else
				if (pCPG->CalcTimeChange(irep + 1));
			WriteResultofActualBuftoFile(irep, nbuf);
		}
	};

	m.SetPaneMessage("Calculation of with Transformations Started - Results will be saved in actual file");
	auto actual_rep = pCPG->GetRepititionSequence();
	pCPG->ReadFirst_Buffer();
	for (int irep = 0; irep<pCPG->GetNumRepititions(); irep++) {
		if (irep>0) {
			if (!pCPG->ReadNewBuffer(irep+1)) {
				m.SetPaneMessage("Read of Current Data from File Failed");
				return FALSE;
			}
		}
		for (int ii = 0; ii<pCPG->GetNumVariables(); ii++) {
			expression_type = pCPG->GetVarFunctionType();
			if (expression_type == 0 && ii == 0) MakeTimeCalc(irep);
			else if (expression_type != 0)
				MakeVariableCalc(ii, irep);
		}
		pCPG->WriteNewBuffer(irep+1);
		if(irep+1==pCPG->GetNumRepititions()) pCPG->SetCompleteRead(true);
	}
	if (pCPG->GetNumRepititions()>1) pCPG->SetRepititionSequence(actual_rep, true);

	if (!pCPG->GetCompleteRead())
		pCPG->CloseFile();

	if (pCPG->WritePGFile()) {
		m.SetPaneMessage("Calculation of with Transformations Completed - Results are saved in actual file");
		m_Changed_Data = false;
		m_Changed_Description = false;
	}
	else
		m.SetPaneMessage("Failed to save file");

	return TRUE;
}