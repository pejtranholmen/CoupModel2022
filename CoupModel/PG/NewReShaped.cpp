#include "../../StdAfx.h"
#include "../../Std.h"
#include "../../Util.h"
#include "NewReShaped.h"

NewReShaped::NewReShaped()
{
}

NewReShaped::~NewReShaped()
{
}

bool NewReShaped::CreateNewReShapedFile(CPG* pPG,  vector<string> unique_names, vector<size_t> num_repof_names, vector<vector<size_t>> VarNamePos, bool treatment, bool specialtreatment, bool parcelNo)
{
	CPG* pPG_Dest;
	pPG_Dest = new CPG;

	bool fail = true;
	if (pPG == nullptr && m_pPGSource == nullptr) return nullptr;
	else if (pPG != nullptr)
		m_pPGSource = pPG;

	m_DestFileName = m_pPGSource->GetFileName();
	int ipos = m_DestFileName.find(".bin");
	if (ipos < 0) ipos = m_DestFileName.find(".BIN");
	if (ipos < 0) ipos = m_DestFileName.find(".Bin");

	m_DestFileName = m_DestFileName.substr(0, ipos);
	m_DestFileName += "_Reshaped.bin";
	pPG_Dest->SetFileName(m_DestFileName);

	auto irec=m_pPGSource->GetNumRecords();
	auto ivar = m_pPGSource->GetNumVariables();

	// How many records with the unique names
	size_t TotalRecords;
	//for (auto ii = 0; ii < unique_names.size(); ii++) {
	TotalRecords=VarNamePos[0].size() * irec;
	
	pPG_Dest->SetCompleteRead(true);
	auto newidvar = 0;
	if (treatment) newidvar++; if (specialtreatment) newidvar++; if (parcelNo) newidvar++;
	pPG_Dest->ReAllocate(TotalRecords, unique_names.size() + newidvar);
	auto counter1 = 0;	
	string idkod;
	

	for (auto i = 0; i < unique_names.size(); i++) {		
		auto counter2 = 0;
		for (auto ir = 0; ir < irec; ir++) {
			auto minn = m_pPGSource->GetLongTime(ir + 1);
			if (ir == 0) counter2 = 0;
			for (auto j = 0; j < VarNamePos[i].size(); j++) {
				counter2++;
				auto value = m_pPGSource->GetVarValue(VarNamePos[i][j], ir + 1);
				idkod = m_pPGSource->GetVarId(VarNamePos[i][j]);
				pPG_Dest->SetLongTime(counter2, minn);
				pPG_Dest->SetVarValue(i + 1, counter2, value);
				newidvar = 0;
				if (treatment) {
					newidvar++;
					int IBeh = GetTreatment(idkod);
					pPG_Dest->SetVarValue(unique_names.size() + newidvar, counter2, float(IBeh));
				}
				if (specialtreatment) {
					newidvar++;
					int ISpec = GetSpecTreatment(idkod);
					pPG_Dest->SetVarValue(unique_names.size() + newidvar, counter2, float(ISpec));
				}
				if (parcelNo) {
					newidvar++;
					int IParc = GetParcell(idkod);
					pPG_Dest->SetVarValue(unique_names.size() + newidvar, counter2, float(IParc));
				}
			
			}
		}
		auto name = m_pPGSource->GetVarName(VarNamePos[i][0]);
		pPG_Dest->SetVarName(i + 1, name);
		newidvar = 1;
		pPG_Dest->SetVarName(unique_names.size() + 2, "ParcNo");
		if (treatment) {
			pPG_Dest->SetVarName(unique_names.size() + newidvar, "Treatment");
			newidvar++;
		}
		if (specialtreatment) {
			pPG_Dest->SetVarName(unique_names.size() + newidvar, "SpecialTreatment");
			newidvar++;

		}
		if (parcelNo) {
			pPG_Dest->SetVarName(unique_names.size() + newidvar, "ParcNo");
		}

	}

	pPG_Dest->WritePGFile();
	pPG_Dest->CloseFile();
	delete pPG_Dest;

	return false;
}

int NewReShaped::GetTreatment(string idkod) {

int IBeh = 0;


if (idkod.find("AN") == 0) IBeh = 5;
if (idkod.find("C") == 0) IBeh = 1;
if (idkod.find("F") == 0) IBeh = 2;
if (idkod.find("I") == 0) IBeh = 3;
if (idkod.find("IL") == 0) IBeh = 4;
if (IBeh == 0) {
	if (idkod.find("G(F-Mg)") == 0) IBeh = 12;
	if (idkod.find("G(F-P)") == 0) IBeh = 12;
	if (idkod.find("GC") == 0) IBeh = 11;
	if (idkod.find("Pond") == 0) IBeh = 20;
}

	return IBeh;
}

int NewReShaped::GetParcell(string idkod)
{
	auto Isplit = idkod.find(";");

	if (Isplit == string::npos) Isplit = 1;
	if (idkod.find("AN") == 0) Isplit = 2;
	if (idkod.find("IL") == 0) Isplit = 2;
	if (idkod.find("F30") == 0|| idkod.find("F50") == 0) Isplit = 3;
	if (idkod.find("F30M") == 0 || idkod.find("F50M") == 0) Isplit = 4;
	if (idkod.find("F-Mg") == 0) Isplit = 4;
	if (idkod.find("F-P") == 0) Isplit = 3;
	if (idkod.find("G(F-Mg)") == 0) Isplit = 7;
	if (idkod.find("G(F-P)") == 0) Isplit = 6;
	if (idkod.find("G(F-Mg)Mor") == 0) Isplit = 10;
	if (idkod.find("G(F-P)Mor") == 0) Isplit = 9;
	if (idkod.find("GC") == 0) Isplit = 2;
	if (idkod.find("CMor") == 0) Isplit = 4;
	if (idkod.find("GCMor") == 0) Isplit = 5;
	if (idkod.find("FMor") == 0) Isplit = 4;
	if (idkod.find("F50Mor") == 0) Isplit = 6;
	if (idkod.find("IL30") == 0 || idkod.find("IL50") == 0) Isplit = 4;
	if (idkod.find("IL30M") == 0|| idkod.find("IL50M") == 0) Isplit = 5;
	if (idkod.find("IH") == 0) Isplit = 2;
	if (idkod.find("ILH") == 0) Isplit = 3;
	if (idkod.find("ILHMor") == 0) Isplit = 6;
	if (idkod.find("IHMor") == 0) Isplit = 5;
	



	idkod = idkod.substr(Isplit);

	int IRuta;
	IRuta = 0;
	auto IposA = idkod.find("A");
	if(IposA == string::npos) {
		auto IposB = idkod.find("B");
		if (IposB == 1) IRuta = FUtil::AtoInt(idkod.substr(0, 1));
		else if (IposB == 2) IRuta = FUtil::AtoInt(idkod.substr(0, 2));
		else if (IposB==string::npos) return 0;
		IRuta += 17;
	}
	else {
		if (IposA == 1) IRuta = FUtil::AtoInt(idkod.substr(0, 1));
		else if (IposA == 2) IRuta = FUtil::AtoInt(idkod.substr(0, 2));
	}
	return IRuta;
}

int NewReShaped::GetSpecTreatment(string idkod)
{
	auto Isplit = idkod.find(";");
	if (Isplit == string::npos) Isplit = 0;
	auto ISpec = 0;
	if (idkod.substr(Isplit).find("50") != string::npos) ISpec = 5;
	if (idkod.substr(Isplit).find("30") != string::npos) ISpec = 3;
	if (idkod.substr(Isplit).find("50M") != string::npos) ISpec = 50;
	if (idkod.substr(Isplit).find("30M") != string::npos) ISpec = 30;
	if (idkod.substr(Isplit).find("60") != string::npos) ISpec = 6;
	if (idkod.substr(Isplit).find("60M") != string::npos) ISpec = 60;

	if (idkod.substr(Isplit).find("ILV") != string::npos) ISpec = 100;
	if (idkod.substr(Isplit).find("IV") != string::npos) ISpec = 110;
	if (idkod.substr(Isplit).find("ILVH") != string::npos) ISpec = 200;
	if (idkod.substr(Isplit).find("IVH") != string::npos) ISpec = 210;

	if (idkod.substr(Isplit).find("ILV") != string::npos) ISpec = 100;
	if (idkod.substr(Isplit).find("IV") != string::npos) ISpec = 110;
	if (idkod.substr(Isplit).find("ILVH") != string::npos) ISpec = 200;
	if (idkod.substr(Isplit).find("IVH") != string::npos) ISpec = 210;

	if (idkod.substr(Isplit).find("G") != string::npos) ISpec = 300;
	if (idkod.substr(Isplit).find("GC") != string::npos) ISpec = 310;
	if (idkod.substr(Isplit).find("GCO") != string::npos) ISpec = 320;
	return ISpec;
}
