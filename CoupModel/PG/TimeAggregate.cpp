#include "./TimeAggregate.h"
#include "../stdafx.h"
#include "../MainFrm.h"
bool TimeAggregate::CreateNewAggregatedFile(CPG *pPG, int Option, int AggregationStep)
{
	CPG *pPG_Dest;
	pPG_Dest= new CPG;

	bool fail=true;
	if(pPG==nullptr&&m_pPGSource==nullptr) return nullptr;
	else if(pPG!=nullptr)
		m_pPGSource=pPG;

	m_pPGSource->CloseFile();
	m_pPGSource->SetCompleteRead(false);
	m_pPGSource->SetCompleteRepRead(true);
	m_pPGSource->Open(m_pPGSource->GetFileName());
	m_pPGSource->ReOpen();

	auto nrep=m_pPGSource->GetNumRepititions();
	auto nvar=m_pPGSource->GetNumVariables();
	auto numrec=m_pPGSource->GetNumRecords();
	
	if(nrep==1) {
		m_pPGSource->SetActBufferSize(m_pPGSource->GetNumRecords());
	}

 


	m_pPGSource->ReadNewBuffer(0, 0);
	m_pPGSource->SetMeanReset(Option);
	int NewNumRec=0;
	float *pFloat;
	switch(Option) {
		case 0:
			NewNumRec=m_pPGSource->GetNumRecordsYear(AggregationStep);
			break;
		case 1:
			NewNumRec=m_pPGSource->GetNumRecordsMonth(AggregationStep);
			break;
		case 2:
			NewNumRec=m_pPGSource->GetNumRecordsDays(AggregationStep);
			break;
		case 3:
			NewNumRec=m_pPGSource->GetNumRecordsHour(AggregationStep);
			break;
		case 4:
			NewNumRec=m_pPGSource->GetNumRecordsMinute(10*AggregationStep);
			break;
	}

	if(NewNumRec<=0) {
		delete pPG_Dest;
		return false;
	}

	pPG_Dest->SetCompleteRead(false);
	pPG_Dest->SetCompleteRepRead(true);
	pPG_Dest->CreateNewFile(nvar, NewNumRec,nrep);
	pPG_Dest->SetInfoMode();
	for(size_t rep=0; rep<nrep; rep++) {
		string txt;
			for(size_t i=1; i<=nvar; i++) {
							txt=m_pPGSource->GetVarName(i,rep);
							pPG_Dest->SetVarName(i, txt, rep);
							txt=m_pPGSource->GetVarUnit(i, rep);
							pPG_Dest->SetVarUnit(i, txt, rep);
							txt=m_pPGSource->GetVarId(i, rep);
							pPG_Dest->SetVarId(i, txt, rep);
							txt=m_pPGSource->GetVarPos(i, rep);
							pPG_Dest->SetVarPos(i, txt, rep);
							double Lat=m_pPGSource->GetVarLat(i, rep);
							pPG_Dest->SetVarLat(i, Lat, rep);
							double Long=m_pPGSource->GetVarLong(i, rep);
							pPG_Dest->SetVarLong(i, Long, rep);
							double Alt=m_pPGSource->GetVarAlt(i, rep);
							pPG_Dest->SetVarAlt(i, Alt, rep);
							string txt=m_pPGSource->GetVarStation(i, rep);
							pPG_Dest->SetVarStation(i, txt, rep);
							txt=m_pPGSource->GetVarCountry(i, rep);
							pPG_Dest->SetVarCountry(i, txt, rep);		

				}
	}
	if(!pPG_Dest->SaveAs(m_DestFileName, true)) {
		pPG_Dest->ReOpen(true);
	}

	CWinApp* pApp = AfxGetApp();
	CCoupModelApp *pCoupModel=(CCoupModelApp*)pApp;
	CMainFrame *p_mainframe;
 
	p_mainframe=(CMainFrame*)pCoupModel->m_pMainWnd;
	if(nrep==1) {
		m_pPGSource->SetActBufferSize(m_pPGSource->GetNumRecords());
		pPG_Dest->SetActBufferSize(NewNumRec);
	}
	auto kul=pPG_Dest->pg.Var.v.size();

	for(size_t rep=0; rep<nrep; rep++) {
		m_pPGSource->ReadNewBuffer(1, rep);
		m_pPGSource->SetMeanReset(Option);
		/*if(!pPG_Dest->SetRepititionSequence(rep+1, true)) {
				delete pPG_Dest;
				return false;	


		}*/
		float rel=float(rep)/float(nrep);
		p_mainframe->UpdateProgress(0, rel);
		if(!m_pPGSource->SetRepititionSequence(rep, true)) return false;
		kul = pPG_Dest->pg.Var.v.size();
		switch(Option) {
		case 0:
			NewNumRec=m_pPGSource->GetNumRecordsYear(AggregationStep, rep);
			break;
		case 1:
			NewNumRec=m_pPGSource->GetNumRecordsMonth(AggregationStep, rep);
			break;
		case 2:
			NewNumRec=m_pPGSource->GetNumRecordsDays(AggregationStep, rep);
			break;
		case 3:
			NewNumRec=m_pPGSource->GetNumRecordsHour(AggregationStep, rep);
			break;
		case 4:
			NewNumRec=m_pPGSource->GetNumRecordsMinute(10* AggregationStep, rep);
			break;
		}
		pFloat=m_pPGSource->GetMeanPointer(Option);
		pPG_Dest->SetActBufferSize(NewNumRec);
		if(!pPG_Dest->SetPointerToAllData(pFloat)) {
			delete pPG_Dest;
			return false;
		}
		if(!pPG_Dest->GetCompleteRead()) {

			if(!pPG_Dest->WriteNewBuffer(rep)) {
				delete pPG_Dest;
				return false;				
				
			};
		}
		if(nrep==1) {
			pPG_Dest->SetCompleteRead(true);

		}

	}
	for (size_t rep = 0; rep < nrep; rep++) {
		kul = pPG_Dest->pg.Var.v.size();
		if (!pPG_Dest->ReCalculateMinMax(rep)) {
			delete pPG_Dest;
			return false;

		}
	}



//	pPG_Dest->CheckMissingNumbers();
	string txt;
	if(pPG_Dest->WritePGFileEnd(NewNumRec,nrep-1)) {


		
		txt="Calculation made and file created with : ";
		txt+=FUtil::STD_ItoAscii(pPG_Dest->GetNumRecords());
		txt+=" records and ";
		txt+=FUtil::STD_ItoAscii(pPG_Dest->GetNumVariables());
		txt+=" variables ";
		pPG_Dest->GetNumVariables();
		m_ResultSum.push_back(txt);
		m_ResultSum.push_back(pPG_Dest->GetFileName());
		txt="Click on file name to open file";
		m_ResultSum.push_back(txt);
		
	}
	else {
		txt="Calculation failed no file created";
		m_ResultSum.push_back(txt);
	}
	pPG_Dest->CloseFile();




	delete pPG_Dest;

	return true;
}
