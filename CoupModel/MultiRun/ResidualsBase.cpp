// Residuals.cpp : implementation file
//

#include "../stdafx.h"
#include "../std.h"
#include "./ResidualsBase.h"

#ifndef COUPSTD
#include "../NewBase/FileScanner.h"
#include "../MainFrm.h"
#endif

struct RES_STAT {
	vector<float> accepted_mean;
	vector<float> accepted_min;
	vector<float> accepted_max;
	vector<float> accepted_std;
	vector<float> accepted_p10;
	vector<float> accepted_p50;
	vector<float> accepted_p90;
	vector<float> all_mean;
	vector<float> all_min;
	vector<float> all_max;
	vector<float> all_std;
};



// ResidualsBase

ResidualsBase::ResidualsBase()
{
	m_RecL=m_NumRec=m_NumVar=0;
	m_ScalingNumVarWithDim1 = 1;
	m_MeanDefined = false;

	//m_EnsembleFileRead = false;

}

ResidualsBase::~ResidualsBase()
{

}
bool ResidualsBase::EnsembleReadWrite(bool reading, size_t ensembleno) {
	 if(ensembleno==string::npos&&reading) {
		 bool validfile = true;
		for(size_t i=0; i<m_Ensembles.size(); i++) {
			auto filename = GetXStatFileName(ensembleno);
			if (!EnsembleReadWrite(reading, filename, "", i)) validfile = false;

		} 
		if (m_Ensembles.size() > 0) {
			m_MeanDefined = true;
			return validfile;
		}
		else return false;
	 }
	 else if(ensembleno<m_Ensembles.size()) {
		 string filename = GetXStatFileName(ensembleno);
		 return EnsembleReadWrite(reading, filename, "", ensembleno);
	 }
	 return false;
}
bool ResidualsBase::EnsembleReadWrite(bool reading, string EnsembleName, string NewEnsembleName, size_t ensembleno) {
	auto filename = GetXStatFileName(ensembleno);
	auto wfile = m_EnsembleResFile.ToWString(filename);
	bool good;
	m_EnsembleName = EnsembleName;
	if (reading) {

		good = m_EnsembleResFile.OpenRead(wfile);

		if (!good) {
			return false;
		}
		m_TwoFilesOpen = false;
	}
	else {

		if (FUtil::IsFileExisting(filename)) {
			m_TempOutFile = filename.substr(0, filename.find(".SXBin")) + ".XXBin";
			auto XXfile = m_EnsembleResFile.ToWString(m_TempOutFile);
			good = m_EnsembleResFile.OpenRead(wfile);
			good = m_EnsembleResFile.OpenWrite(XXfile);
			m_TwoFilesOpen = true;
		}
		else {
			m_TempOutFile.clear();
			good = m_EnsembleResFile.OpenWrite(wfile);
			m_TwoFilesOpen = false;
		}
	}
	if (!good) return false;

	m_NewEnsembleName = NewEnsembleName;

	if (ReadWrite_MeanEnsembleFile(reading, ensembleno)) {
		//m_IsUpdated = true;
		if (reading) {		
			m_EnsembleResFile.Close();

		}
		else {
			if (m_TwoFilesOpen) {
				m_EnsembleResFile.Close();
				m_EnsembleResFile.Close();
				remove(filename.c_str());
				vector< vector<size_t>> koll;
				float value = m_MeanPost.mean[0][0];
				//float value2 = m_MeanPost.mean[1][0];
				for (size_t ivar = 0; ivar < m_NumVar;ivar++) {
					vector<size_t> koll2;
					koll.push_back(koll2);
					for (size_t irec = 0; irec < m_MeanPost.mean[ivar].size(); irec++) {
						if (m_MeanPost.mean[ivar][irec] != m_MeanTest.mean[ivar][irec]) {
							koll[ivar].push_back(irec);



						}
					}
				}


				if (!rename(m_TempOutFile.data(), filename.data())) return false;
			}
			else {
				m_EnsembleResFile.Close();
			}
		}


	}
	else {
		if(reading)
			m_EnsembleResFile.Close();

		else {

			if (m_TwoFilesOpen) {
				m_EnsembleResFile.Close();
				m_EnsembleResFile.Close();
			}
			else
				m_EnsembleResFile.Close();
		}



	}
	m_EnsembleName = EnsembleName;
	if(ensembleno<m_Ensembles.size())
	m_Ensembles[ensembleno].EnsembleFileRead = true;
	return true;
}
bool ResidualsBase::ReadWrite_MeanEnsembleFile(bool reading, size_t ensemble_no) {

	bool good;
	size_t ActRecL;
	size_t num_stat = 7;
#ifndef COUPSTD
	MFC_Util mfc_util;
#endif

	
	if (reading) {
		size_t num;
		string str;
		bool hitmade = false;

		/*getline(m_EnsembleResFile.m_DocFileIn, str, ':');
		if (str != "Ensemble No ") return false;
		getline(m_EnsembleResFile.m_DocFileIn, str);
		num = atoi(str.c_str());*/
		m_EnsembleResFile.ReadTextLine(str, num);
		/*getline(m_EnsembleResFile.m_DocFileIn, str, ':');
		if (str != "Ensemble Num Records ") return false;
		getline(m_EnsembleResFile.m_DocFileIn, str);
		ActRecL = atoi(str.c_str());*/
		m_EnsembleResFile.ReadTextLine(str, ActRecL);
		if (m_MeanTest.mean.size() == 0) AlloResVector(ActRecL);


	/*	getline(m_EnsembleResFile.m_DocFileIn, str, ':');
		getline(m_EnsembleResFile.m_DocFileIn, str);*/
		size_t VarToRead;

		m_EnsembleResFile.ReadTextLine(str, VarToRead);
		vector<float> Dump;
		
		Dump.resize(ActRecL * num_stat);

		
		m_MeanPost.mean.resize(VarToRead);
		m_MeanPost.min.resize(VarToRead);
		m_MeanPost.max.resize(VarToRead);
		m_MeanPost.std.resize(VarToRead);
		m_MeanPost.p10.resize(VarToRead);
		m_MeanPost.p50.resize(VarToRead);
		m_MeanPost.p90.resize(VarToRead);

		for (size_t i = 0; i < VarToRead; i++) {

			m_MeanPost.mean[i].resize(ActRecL);
			m_MeanPost.min[i].resize(ActRecL);
			m_MeanPost.max[i].resize(ActRecL);
			m_MeanPost.std[i].resize(ActRecL);
			m_MeanPost.p10[i].resize(ActRecL);
			m_MeanPost.p50[i].resize(ActRecL);
			m_MeanPost.p90[i].resize(ActRecL);

			/*getline(m_EnsembleResFile.m_DocFileIn, str, ':');
			getline(m_EnsembleResFile.m_DocFileIn, str);*/
			size_t var;
			m_EnsembleResFile.ReadTextLine(str, var);
			

			//Dump=m_EnsembleResFile.rFloatArray2(ActRecL * num_stat, num_stat);
			size_t vectorsize = ActRecL * num_stat;
			vector<int> ints;
			m_EnsembleResFile.ReadTextLines(ActRecL, str, vectorsize, Dump, ints);
#ifndef COUPSTD
			mfc_util.SetPaneMessage("Reading Mean of Time Serie data var: "+ FUtil::STD_ItoAscii(var+1)+" Ensemble :"+ FUtil::STD_ItoAscii(ensemble_no+1));
#endif


			for (size_t i = 0; i < ActRecL; i++) {

				m_MeanPost.mean[var][i] = Dump[i * num_stat];
				m_MeanPost.min[var][i] = Dump[i * num_stat + 1];
				m_MeanPost.max[var][i] = Dump[i * num_stat + 2];
				m_MeanPost.std[var][i] = Dump[i * num_stat + 3];
				m_MeanPost.p10[var][i] = Dump[i * num_stat + 4];
				m_MeanPost.p50[var][i] = Dump[i * num_stat + 5];
				m_MeanPost.p90[var][i] = Dump[i * num_stat + 6];
			}
		}

		good = m_EnsembleResFile.IsGood();
		if (!good) return false;

		return true;
	}
	else {
			// New single file.
			size_t num = ensemble_no;
			m_EnsembleResFile.WriteTextLine("Ensemble No ",num);
			ActRecL = m_MeanPost.mean[0].size();
			m_EnsembleResFile.WriteTextLine("Ensemble Num Records ",ActRecL);

			size_t VarToWrite = m_NumVar * m_ScalingNumVarWithDim1;
			if (VarToWrite > m_MeanPost.mean.size()) VarToWrite = m_NumVar;

			vector<float> Dump;
			Dump.resize(ActRecL * num_stat);
			m_EnsembleResFile.WriteTextLine("Var in File ", VarToWrite);

			for (size_t var = 0; var < VarToWrite; var++) {
				m_EnsembleResFile.WriteTextLine("Var No ",var);
				for (size_t i = 0; i < ActRecL; i++) {
					Dump[i*num_stat] = m_MeanPost.mean[var][i];
					Dump[i * num_stat+1] = m_MeanPost.min[var][i];
					Dump[i * num_stat + 2] = m_MeanPost.max[var][i];
					Dump[i * num_stat + 3] = m_MeanPost.std[var][i];
					Dump[i * num_stat + 4] = m_MeanPost.p10[var][i];
					Dump[i * num_stat + 5] = m_MeanPost.p50[var][i];
					Dump[i * num_stat + 6] = m_MeanPost.p90[var][i];
				}
				if (num == 7&&ActRecL==8000) {
					auto koll = var;
					auto v = Dump;
				}
				m_EnsembleResFile.WriteTextLine(Dump, num_stat);
			}

			good = m_EnsembleResFile.IsGood();

			//m_Ensembles.clear();
			POS_ENSEMBLE ensemble_inf;
			ensemble_inf.pos = 0;
			ensemble_inf.name = m_EnsembleName;
			ensemble_inf.valid = false;

			//m_Ensembles.push_back(ensemble_inf);

			return good;
		}

}
string ResidualsBase::GetXStatFileName(size_t EnsembleNo) {
	if (m_pNewMap != nullptr) {
		string str;
		auto ipos = m_pNewMap->m_DocFile2.m_Multi_MBin_File.find(".MBin");
		if (ipos != string::npos) {
			str = m_pNewMap->m_DocFile2.m_Multi_MBin_File.substr(0, ipos);
			str += "_V" + FUtil::STD_ItoAscii(m_ResidualFileNo)+"_Eno"+ FUtil::STD_ItoAscii(EnsembleNo+1);
			str += ".SXBin";
			return str;
		}
	}
	return "";
}
bool ResidualsBase::AlloResVector(size_t ActRecL) {
	if (m_NumVar == 0) return false; 
	if(ActRecL==string::npos) ActRecL = m_RecL;

	m_Residuals.resize(ActRecL);
	m_Mean.resize(m_NumVar*m_RecL);
	m_MeanPrior.mean.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanPrior.min.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanPrior.max.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanPrior.std.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanPrior.p10.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanPrior.p50.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanPrior.p90.resize(m_NumVar*m_ScalingNumVarWithDim1);

	m_MeanPost.mean.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanPost.min.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanPost.max.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanPost.std.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanPost.p10.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanPost.p50.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanPost.p90.resize(m_NumVar*m_ScalingNumVarWithDim1);

	m_MeanTest.mean.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanTest.min.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanTest.max.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanTest.std.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanTest.p10.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanTest.p50.resize(m_NumVar*m_ScalingNumVarWithDim1);
	m_MeanTest.p90.resize(m_NumVar*m_ScalingNumVarWithDim1);
	
	for (size_t i = 0; i < m_NumVar*m_ScalingNumVarWithDim1; i++) {
		m_MeanPrior.mean[i].resize(ActRecL);
		m_MeanPrior.min[i].resize(ActRecL);
		m_MeanPrior.max[i].resize(ActRecL);
		m_MeanPrior.std[i].resize(ActRecL);
		m_MeanPrior.p10[i].resize(ActRecL);
		m_MeanPrior.p50[i].resize(ActRecL);
		m_MeanPrior.p90[i].resize(ActRecL);

		m_MeanPost.mean[i].resize(ActRecL);
		m_MeanPost.min[i].resize(ActRecL);
		m_MeanPost.max[i].resize(ActRecL);
		m_MeanPost.std[i].resize(ActRecL);
		m_MeanPost.p10[i].resize(ActRecL);
		m_MeanPost.p50[i].resize(ActRecL);
		m_MeanPost.p90[i].resize(ActRecL);
		
		m_MeanTest.mean[i].resize(ActRecL);
		m_MeanTest.min[i].resize(ActRecL);
		m_MeanTest.max[i].resize(ActRecL);
		m_MeanTest.std[i].resize(ActRecL);
		m_MeanTest.p10[i].resize(ActRecL);
		m_MeanTest.p50[i].resize(ActRecL);
		m_MeanTest.p90[i].resize(ActRecL);
	}
	m_SizeOfMeanVector = m_NumVar*ActRecL;
	return true;
}
std::streamsize ResidualsBase::GetFileSizeFromHandle() {
	LARGE_INTEGER size;
	if (GetFileSizeEx(m_EnsembleResFile.GetHandle(), &size))
		return static_cast<std::streamsize>(size.QuadPart);
	return 0;
}


bool ResidualsBase::OpenResidualFile(bool reading) {


	if (reading) {
		if (!m_ResidualFile.OpenDocFileIn(m_FileName)) return false;
		m_FileSize = m_ResidualFile.GetStdInputFileSize();
		if (m_FileSize == 0 || m_FileSize == string::npos) {
			return false;


		}
		Residuals_ReadWrite_Header(true);
		m_Residuals.resize(m_RecL);
		if(m_pNewMap->m_FileVersionNumberRead < 65)
			AlloResVector();
	}
	else
		if (!m_ResidualFile.OpenDocFileout(m_FileName)) return false;
	
	return true;
}
bool ResidualsBase::Residuals_ReadWrite_Header(bool reading) {
	if (reading) {
		if (!m_ResidualFile.rsize_t(&m_NumRec)) return false;
		if (!m_ResidualFile.rsize_t(&m_RecL)) return false;
		if (!m_ResidualFile.rsize_t(&m_NumVar)) return false;
		m_MaxNumRec = size_t(m_FileSize / (m_RecL*m_NumVar * 4));
		if (m_pNewMap != nullptr) {
			if (m_pNewMap->m_MultiRun_Array[0].NumberOfRepeatitionsWithinDimension < m_NumRec) {
				if (m_pNewMap->m_MultiRun_Array.size() == 1) m_NumRec = m_pNewMap->m_MultiRun_Array[0].NumberOfRepeatitionsWithinDimension * m_NumVar;
				else m_NumRec = 0;
				for (size_t i = 1; i < m_pNewMap->m_MultiRun_Array.size(); i++) {
					m_NumRec += m_pNewMap->m_MultiRun_Array[i].NumberOfRepeatitionsWithinDimension*m_NumVar;
				}
			}
		}
		if (m_MaxNumRec<m_NumRec / m_NumVar) {
#ifndef COUPSTD
			if (m_pNewMap != nullptr) {
				m_pNewMap->m_ErrorCount++;
				if (m_pNewMap->m_ErrorCount<10) {
					string str;
					str = "For some reason the file \n("; str += m_FileName; str += ")\n contains only :" + FUtil::STD_ItoAscii(m_MaxNumRec) + " Runs";
					str += "\nAdjust the Number of Multi Runs to correspond with that number";
					str += "\nExpected Number was: ";
					str += FUtil::STD_ItoAscii(m_NumRec / m_NumVar);
					MFC_Util::MessageBox(str.c_str(), "Lacking data in XBin", MB_OK);
				}
			}
#endif		

		}
	}
	else {
		if (!m_ResidualFile.wSize_t(&m_NumRec)) return false;
		if (!m_ResidualFile.wSize_t(&m_RecL)) return false;
		if (!m_ResidualFile.wSize_t(&m_NumVar)) return false;
	}
	return true;
}
bool ResidualsBase::CloseResidualFile(bool reading) {

	if (reading)
		m_ResidualFile.CloseFileIn();
	else
		m_ResidualFile.CloseFileOut();

	return true;
}
bool ResidualsBase::IsOpen(bool reading) {
	if (reading) {
		if (m_ResidualFile.IsFileInGood() && m_IsOpen)
			return true;
		else
			return false;
	}
	else {
		if (m_ResidualFile.IsFileOutGood() && m_IsOpen)
			return true;
		else
			return false;
	}
}
bool ResidualsBase::SetNewEnsembleByNumber(size_t EnsembleNo) {
	if (EnsembleNo < m_pNewMap->m_MStorage.m_AcceptedRuns.size())
		return EnsembleReadWrite(true, EnsembleNo);
	else
		return false;
}
bool ResidualsBase::ShiftEnsembleName(string oldname, string newname) {
	bool out=false;


	for (size_t i = 0; i < m_Ensembles.size(); i++) {
		if (m_Ensembles[i].name == oldname) {
			m_Ensembles[i].name = newname;
			out = true;
		}
	}
	if (out) return EnsembleReadWrite(false, oldname, newname);

	return out;
}
bool ResidualsBase::EnableEstimatesMade(size_t enableno) {
	if (enableno < m_Ensembles.size()) {
		bool made = true;
		for (size_t i = 0; i < m_Ensembles.size(); i++)
			if (!m_Ensembles[enableno].EnsembleFileRead) made = false;
		return made;


	}
	else if (m_pNewMap->m_MStorage.m_AcceptedRuns.size() >= enableno) {
		m_Ensembles.resize(m_pNewMap->m_MStorage.m_AcceptedRuns.size());
		return false;


	}
	else return false;
};
