#include "../../StdAfx.h"
#include "../../Std.h"
#include "../../Util.h"
#include "NewReShaped.h"

NewReShaped::NewReShaped()
{
	treatment_convert.push_back(pair<string, int>("C", 10));
	treatment_convert.push_back(pair<string, int>("F", 20));
	treatment_convert.push_back(pair<string, int>("I", 30));
	treatment_convert.push_back(pair<string, int>("IL", 40));
	treatment_convert.push_back(pair<string, int>("AN", 50));
	treatment_convert.push_back(pair<string, int>("G(F-Mg)", 12));
	treatment_convert.push_back(pair<string, int>("G(F-P)", 12));
	treatment_convert.push_back(pair<string, int>("GC", 11));
	treatment_convert.push_back(pair<string, int>("Pond", 20));

	Parcell_Stop.push_back("A");
	Parcell_Stop.push_back("B");
	Parcell_Add.push_back(17);



	P_Cutting30 = {3,8,12,17, 18,20, 24,28,32, };
	P_Cutting60 = { 2, 5, 26,30};
	P_Cutting100 = {11, 19,22,23,27 };
}

NewReShaped::~NewReShaped()
{
}
 CPG* NewReShaped::CreateNewReShapedFile(CPG* pPG,  vector<string> unique_names, vector<size_t> num_repof_names, vector<vector<size_t>> VarNamePos, bool treatment, bool specialtreatment, bool parcelNo)
{
	CPG* pPG_Dest;
	pPG_Dest = new CPG;

	bool fail = true;
	if (pPG == nullptr && m_pPGSource == nullptr) return nullptr;
	else if (pPG != nullptr)
		m_pPGSource = pPG;

	if (!m_pPGSource->ReOpen()) return nullptr;

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
	m_NewIdVar = 0;
	if (treatment) m_NewIdVar++; if (specialtreatment) m_NewIdVar++; if (parcelNo) m_NewIdVar++;



	pPG_Dest->CreateNewFile(unique_names.size() + m_NewIdVar, TotalRecords);
	//pPG_Dest->ReAllocate(TotalRecords, );
	auto counter1 = 0;	
	string idkod;
	
	auto counter2 = 0;
	for (auto i = 0; i < unique_names.size(); i++) {		
		counter2 = 0;
		for (auto ir = 0; ir < irec; ir++) {
			auto minn = m_pPGSource->GetLongTime(ir + 1);

			for (auto j = 0; j < VarNamePos[i].size(); j++) {
				counter2++;
				auto value = m_pPGSource->GetVarValue(VarNamePos[i][j], ir + 1);
				idkod = m_pPGSource->GetVarId(VarNamePos[i][j]);
				pPG_Dest->SetLongTime(counter2, minn);
				pPG_Dest->SetVarValue(i + 1, counter2, value);
				auto newidvar = 0;
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
		auto id = m_pPGSource->GetVarId(VarNamePos[i][0]);
		pPG_Dest->SetVarId(i + 1, id);
		auto newidvar = 1;
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
	pPG_Dest->ReCalculateMinMax();
	pPG_Dest->WritePGFile();

	auto koll = pPG_Dest->GetCycleWithCommonTime();

	
	pPG_Dest->ReOpen();

	if(m_RepetitionOutputStyle)  pPG_Dest=CreateRepitionsStructure(pPG_Dest);
	
	return pPG_Dest;
}

 CPG* NewReShaped::CreateReshaped(CPG* pPG)
// This is with aggregation of common Treatment variable
 { CPG* pPG_Dest;
 

 bool fail = true;
 if (pPG == nullptr && m_pPGSource == nullptr) return nullptr;
 else if (pPG != nullptr)
	 m_pPGSource = pPG;

 m_DestFileName = m_pPGSource->GetFileName();
 int ipos = m_DestFileName.find("_Reshaped.bin");

 m_DestFileName = m_DestFileName.substr(0, ipos);
 m_DestFileName += "_ReshapedMean.bin";


 auto irec = m_pPGSource->GetNumRecords();
 auto ivar = m_pPGSource->GetNumVariables();
 m_pPGSource->ReOpen();
 m_pPGSource->SetCompleteRead(true);
 size_t treat_var{ 0 };
 for (auto i = 0; i < ivar; i++) {
	 string varname = m_pPGSource->GetVarName(i + 1);
	 if(varname.find("Treatment")!=string::npos) {
		 treat_var = i + 1;
	 }
 }

 if (treat_var > 0) {
	 pPG_Dest = new CPG();

	 pPG_Dest->SetFileName(m_DestFileName);
	 pPG_Dest->SetInfoMode();
	 pPG_Dest->SetCompleteRead(true);
	 pPG_Dest->CreateNewFile(ivar, irec, 1);




	 size_t count_rec{0}, count{0};
	 size_t min_pre, min_act;
	 vector<float> varvalues;
	 vector<int> n_values;
	 varvalues.resize(treat_var - 1);
	 n_values.resize(treat_var - 1);
	 min_pre = m_pPGSource->GetLongTime(1);

	 float post_treat_value;
	
	 
	 int treat_num, pre_treat_num, post_treat_num{ 0 };
	 pre_treat_num = int(m_pPGSource->GetVarValue(treat_var, 1) + 0.0001);
	 if (treat_var < ivar)  post_treat_num = ivar;

	 auto estimate = [&]() {
		 count_rec++;
		 pPG_Dest->SetLongTime(count_rec, min_pre);
		 for (size_t ivar = 0; ivar < treat_var - 1; ivar++) {
			 float koll = MISSING;
			 if (n_values[ivar] > 0) {
				 koll = varvalues[ivar] / n_values[ivar];
			 }
			 pPG_Dest->SetVarValue(ivar + 1, count_rec, koll);
			 pPG_Dest->SetVarValue(treat_var, count_rec, float(pre_treat_num));
			 if(post_treat_num!=0) pPG_Dest->SetVarValue(post_treat_num, count_rec, float(post_treat_value));
			 varvalues[ivar] = 0.;
			 n_values[ivar] = 0;
		 }

	  };


	 for (size_t i = 0; i < irec; i++) {
		 min_act = m_pPGSource->GetLongTime(i + 1);
		 treat_num = int(m_pPGSource->GetVarValue(treat_var, i+ 1)+0.0001);
		 if (min_act != min_pre||treat_num!=pre_treat_num) {
			 estimate();
		 }
		 for (size_t ivar = 0; ivar < treat_var - 1; ivar++) {
				 auto koll = m_pPGSource->GetVarValue(ivar+1, i + 1);
				 if (koll > MISSING + .0001) {
					 varvalues[ivar] += koll;
					 n_values[ivar]++;
				 }
		 }
		 if(post_treat_num!=0) post_treat_value= m_pPGSource->GetVarValue(post_treat_num, i + 1);
		 min_pre = min_act;
		 pre_treat_num = treat_num;
	 }
	 estimate();


	 
	// pPG_Dest->SetNumRecords(count_rec);
	 pPG_Dest->CreateNewFile(ivar, count_rec, 1);

	 string txt;
	 for (size_t i = 1; i <= ivar; i++) {
		 txt = m_pPGSource->GetVarName(i, 1);
		 pPG_Dest->SetVarName(i, txt, 1);
		 txt = m_pPGSource->GetVarUnit(i, 1);
		 pPG_Dest->SetVarUnit(i, txt, 1);
		 txt = m_pPGSource->GetVarId(i, 1);
		 pPG_Dest->SetVarId(i, txt, 1);
		 txt = m_pPGSource->GetVarPos(i, 1);
		 pPG_Dest->SetVarPos(i, txt, 1);
		 double Lat = m_pPGSource->GetVarLat(i, 1);
		 pPG_Dest->SetVarLat(i, Lat, 1);
		 double Long = m_pPGSource->GetVarLong(i, 1);
		 pPG_Dest->SetVarLong(i, Long, 1);
		 double Alt = m_pPGSource->GetVarAlt(i, 1);
		 pPG_Dest->SetVarAlt(i, Alt, 1);
		 string txt = m_pPGSource->GetVarStation(i, 1);
		 pPG_Dest->SetVarStation(i, txt, 1);
		 txt = m_pPGSource->GetVarCountry(i, 1);
		 pPG_Dest->SetVarCountry(i, txt, 1);

	 }


	 pPG_Dest->ReCalculateMinMax();
	 if (count_rec < irec) {
		 if (pPG_Dest->SaveAs(m_DestFileName, true)) ivar = 100;
	 }

	 if (count_rec == irec) delete pPG_Dest;
	 else delete pPG_Dest;
 }

	 return nullptr;
 }

 CPG* NewReShaped::CreateRepitionsStructure(CPG* pPGSource)
 {
	 // Input File any PG-file with repeated number of variables with common time points.
	 pPGSource->CloseFile();
	 pPGSource->Open(pPGSource->GetFileName());

	 auto numcommontime = pPGSource->GetCycleWithCommonTime();
	 if (numcommontime == 1) return nullptr; // no possible restructure of file
	 auto NumRecDest = pPGSource->GetNumRecords() / numcommontime;
	 auto TotRec = pPGSource->GetNumRecords();

	 
	 CPG* pPG_Dest = new CPG;
	 pPG_Dest->CloseFile();

	 m_DestFileName = m_pPGSource->GetFileName();

	 size_t ipos = m_DestFileName.find("_Reshaped.bin");
	 if(ipos==string::npos) ipos = m_DestFileName.find(".bin");
	 if (ipos==string::npos) ipos = m_DestFileName.find(".BIN");
	 if (ipos == string::npos) ipos = m_DestFileName.find(".Bin");

	 m_DestFileName = m_DestFileName.substr(0, ipos);
	 m_DestFileName += "_WithRepetitions.bin";
	 pPG_Dest->SetFileName(m_DestFileName);


	 m_pPGSource->ReOpen();
	 m_pPGSource->SetCompleteRead(true);
	 auto ivar = pPGSource->GetNumVariables();
	 pPGSource->ReOpen();
	 pPG_Dest->SetInfoMode();
	 pPG_Dest->SetCompleteRead(true);

	 pPG_Dest->CreateNewFile(ivar, pPGSource->GetNumRecords() /numcommontime,numcommontime );
	 if (!pPG_Dest->ReOpen(true)) {
		 if (!pPG_Dest->Open(pPG_Dest->GetFileName())) {
			 auto stillproblem = 1;

		 }

	 }
	 //pPG_Dest->SaveAs(m_DestFileName, true);
	 size_t count_iout=0;
	 size_t minn;
	 for (size_t irec = 0; irec < pPGSource->GetNumRecords(); irec+=numcommontime) {
		 count_iout++;
		 for (size_t irep = 0; irep < numcommontime; irep++) {
			 pPG_Dest->SetRepititionSequence(irep, false);

			 auto kollv = pPGSource->GetRecordDataVector(irec + irep+1, 1);
			 for(size_t iivar=0; iivar<kollv.size(); iivar++) 
				  if (!pPG_Dest->WritePGFileVarValue(iivar, count_iout, kollv[iivar], irep)) { 
					 auto error8 = 0;
				 
				 };
		}
	}

	 m_pPGSource->ReOpen();
	 string txt;
	 auto test_cycle=m_pPGSource->GetCycleWithCommonTime();
	 auto ivar_test = m_pPGSource->GetNumVariables();
	 for (size_t irep = 1; irep <= numcommontime; irep++) {
		 for (size_t i = 1; i <= ivar; i++) {
			 txt = pPGSource->GetVarName(i, 1);
			 pPG_Dest->SetVarName(i, txt, irep);
			 txt = pPGSource->GetVarUnit(i, 1);
			 pPG_Dest->SetVarUnit(i, txt, irep);
			 size_t indexkol = i % (ivar-m_NewIdVar)+i/(ivar-m_NewIdVar+1);
			 if (ivar <= 2||indexkol==0) indexkol = 1;
			 if(irep>1) indexkol+=(irep - 1) * (ivar-m_NewIdVar);
			 txt = m_pPGSource->GetVarId(indexkol, 1);
			 pPG_Dest->SetVarId(i, txt, irep);
			 txt = pPGSource->GetVarPos(i, 1);
			 pPG_Dest->SetVarPos(i, txt, irep);
			 double Lat = pPGSource->GetVarLat(i, 1);
			 pPG_Dest->SetVarLat(i, Lat, irep);
			 double Long = pPGSource->GetVarLong(i, 1);
			 pPG_Dest->SetVarLong(i, Long, irep);
			 double Alt = pPGSource->GetVarAlt(i, 1);
			 pPG_Dest->SetVarAlt(i, Alt, irep);
			 string txt = pPGSource->GetVarStation(i, 1);
			 pPG_Dest->SetVarStation(i, txt, irep);
			 txt = pPGSource->GetVarCountry(i, 1);
			 pPG_Dest->SetVarCountry(i, txt, irep);
		 }

	 }
	 for(size_t irep=1; irep<=pPG_Dest->GetNumRepititions(); irep++)
		pPG_Dest->ReCalculateMinMax(irep);

	 pPG_Dest->WritePGFile();
	 




	 return pPG_Dest;
 }




int NewReShaped::GetTreatment(string idkod) {

int IBeh = 0;
for (size_t i = 0; i < treatment_convert.size(); i++) {
	if (idkod.find(treatment_convert[i].first) == 0) {
		IBeh = treatment_convert[i].second;
		auto npos=idkod.find("-");
		if (npos != string::npos) {
			auto newstr = idkod.substr(npos);
			if (newstr.find("IL")) IBeh = 60;
		}		
	}
}

	 auto parcell = GetParcell(idkod);
	 for (size_t i = 0; i < P_Cutting30.size(); i++) if(parcell==P_Cutting30[i]) IBeh += 1;
	 for (size_t i = 0; i < P_Cutting60.size(); i++) if (parcell == P_Cutting60[i]) IBeh += 2;
	 for (size_t i = 0; i < P_Cutting100.size(); i++) if (parcell == P_Cutting100[i]) IBeh += 3;


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
	auto IposA = idkod.find(Parcell_Stop[0]);
	if(IposA == string::npos) {
		auto IposB = idkod.find(Parcell_Stop[1]);
		if (IposB == 1) IRuta = FUtil::AtoInt(idkod.substr(0, 1));
		else if (IposB == 2) IRuta = FUtil::AtoInt(idkod.substr(0, 2));
		else if (IposB==string::npos) return 0;
		IRuta += Parcell_Add[0];
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
