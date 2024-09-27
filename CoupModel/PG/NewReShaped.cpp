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

bool NewReShaped::CreateNewReShapedFile(CPG* pPG, int option)
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

	auto count_names = 0;
	vector<string> unique_names;
	vector<size_t> num_repof_names;

	
	for (auto i = 0; i < ivar; i++) {
		string name = m_pPGSource->GetVarName(i + 1);
		bool exist = false;
		for (auto ii = 0; ii < unique_names.size(); ii++) {
			if (name == unique_names[ii]) {
				exist = true;
				num_repof_names[ii]++;
			}
		}
		if (!exist) {
			unique_names.push_back(name);
			num_repof_names.push_back(1);
		}
	}
	pPG_Dest->SetCompleteRead(true);
	pPG_Dest->ReAllocate(num_repof_names[0] * irec, unique_names.size() + 4);

	auto counter1 = 0;
	
	string idkod;
	for (auto i = 0; i < unique_names.size(); i++) {
		if(i>0)  counter1+=num_repof_names[i-1];
		string name = m_pPGSource->GetVarName(counter1 + 1);
		
		auto counter2 = 0;
		for (auto ir = 0; ir < irec; ir++) {	
			if(ir>0) counter2 += num_repof_names[i];
			auto minn = m_pPGSource->GetLongTime(ir + 1);
			for (auto j = 0; j < num_repof_names[i]; j++) {
				auto value = m_pPGSource->GetVarValue(counter1+j + 1, ir + 1);
				idkod = m_pPGSource->GetVarId(counter1 + j + 1);
				pPG_Dest->SetLongTime(counter2+ j+1, minn);
				pPG_Dest->SetVarValue(i + 1, counter2+j+1, value);
				pPG_Dest->SetVarValue(unique_names.size() + 1, counter2 + j+1, float(j+1));
				auto IposA = idkod.find("A");
				auto Isplit = idkod.find(";");
				int IRuta, IBeh;
				if (IposA == string::npos) {
					auto IposB = idkod.find("B");
					if (IposB == 1) IRuta = FUtil::AtoInt(idkod.substr(0, 1));
					else if (IposB == 2) IRuta = FUtil::AtoInt(idkod.substr(0, 2));
					else IRuta = 83;
					IRuta += 17;
				}
				else {
					if (IposA == 1) IRuta = FUtil::AtoInt(idkod.substr(0, 1));
					else if (IposA == 2) IRuta = FUtil::AtoInt(idkod.substr(0, 2));

				}
				IBeh = 0;
				if (idkod.substr(Isplit).find("A") != string::npos) IBeh = 5;
				if (idkod.substr(Isplit).find("C") != string::npos) IBeh = 1;
				if (idkod.substr(Isplit).find("F") != string::npos) IBeh = 2;
				if (idkod.substr(Isplit).find("I") != string::npos) IBeh = 3;
				if (idkod.substr(Isplit).find("IL") != string::npos) IBeh = 4;

				auto ISpec = 0;
				if (idkod.substr(Isplit).find("50U") != string::npos) ISpec=5;
				if (idkod.substr(Isplit).find("30U") != string::npos) ISpec =3;
				if (idkod.substr(Isplit).find("50M") != string::npos) ISpec = 50;
				if (idkod.substr(Isplit).find("30M") != string::npos) ISpec = 30;
				if (idkod.substr(Isplit).find("60U") != string::npos) ISpec = 6;
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



				pPG_Dest->SetVarValue(unique_names.size() + 2, counter2 + j + 1, float(IRuta));
				pPG_Dest->SetVarValue(unique_names.size() + 3, counter2 + j + 1, float(IBeh));
				pPG_Dest->SetVarValue(unique_names.size() + 4, counter2 + j + 1, float(ISpec));

			}
		}
		name = m_pPGSource->GetVarName(counter1 + 1);
		pPG_Dest->SetVarName(i + 1, name);
		pPG_Dest->SetVarName(unique_names.size() + 1, "VarNo");
		pPG_Dest->SetVarName(unique_names.size() + 2, "ParcNo");
		pPG_Dest->SetVarName(unique_names.size() + 3, "Treatment");
		pPG_Dest->SetVarName(unique_names.size() + 4, "Special");
	}

	pPG_Dest->WritePGFile();
	pPG_Dest->CloseFile();
	delete pPG_Dest;

	return false;
}
