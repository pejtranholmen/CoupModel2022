// MultiStorageStatData.cpp : implementation file
//

#include "../stdafx.h"
#include "../std.h"
#include "../Util.h"
#include "../NewBase/NewMap.h"
#include "MultiStorageStatData.h"


#ifndef COUPSTD
#include "../NewBase/FileScanner.h"

#endif


#include <limits>


MultiStorageStatData::MultiStorageStatData()
{
	m_IsStatOpen = false;
	m_ResetTransformedPerformance = false;
	m_ActualEnsemble = 0;
	m_IsStatCompleteRead = false;

}

MultiStorageStatData::~MultiStorageStatData()
{


}

bool MultiStorageStatData::EvaluateFile(bool reading, string filename) {
	if (m_IsStatOpen) {
		m_IsStatOpen = false;
	}
#ifndef COUPSTD
	MFC_Util mfc_util;


	if (filename.size() == 0)
		filename = GetStatFileName();



	if(reading)
		mfc_util.SetPaneMessage("Reading From Multi Statistics File:" + filename);
	else
		mfc_util.SetPaneMessage("Writing to Multi Statistic File:" + filename);
#endif
	if (!m_IsStatOpen) {
		auto wfile = m_EFile.ToWString(filename);
		bool good;
		if (reading) {
			good = m_EFile.OpenRead(wfile);
		}
		else
			good=m_EFile.OpenWrite(wfile);
		if (!good) { 
#ifndef COUPSTD
			if (reading)
				mfc_util.MessageBox("Problems when reading from : " + filename);
			else
				mfc_util.MessageBox("Problems when writing to : " + filename);
#endif
			return false;
		}
		if (m_IsStatOpen) return true;
		m_IsStatOpen = true;
	}
	m_IsStatCompleteRead = false;




#ifndef COUPSTD 
	if (reading)
		mfc_util.SetPaneMessage("Reading Accepted Runs ...");
	else
		mfc_util.SetPaneMessage("Writing Accepted Runs ...");


#endif
	if (!ReadWrite_Accepted(reading)) return false;

	if (!ReadWrite_MultiPar(reading)) return false;
	if (!ReadWrite_CoCor(reading)) return false;

#ifndef COUPSTD 
	if (reading)
		mfc_util.SetPaneMessage("Reading Criteria settings ...");
	else
		mfc_util.SetPaneMessage("Writing Criteria settings ...");
#endif

	if (!ReadWrite_Criteria(reading)) return false;


	bool error = false;
#ifndef COUPSTD 
	if (reading)
		mfc_util.SetPaneMessage("Reading TSV data ...");
	else
		mfc_util.SetPaneMessage("Writing TSV data ...");
#endif
	if (!ReadWrite_OptionalTSV_Valid(reading)) {
		error = true;
#ifndef COUPSTD
		mfc_util.SetPaneMessage("Error when Reading TSV data");
#endif
		if (reading) {
			m_EFile.Close();
			return true;
		}

	}
		


	if (!ReadWrite_OptionalSUM_Valid(reading)) return false;

#ifndef COUPSTD 
	if (reading)
		mfc_util.SetPaneMessage("Reading From Multi Statitics File "+filename+" Completed");
	else
		mfc_util.SetPaneMessage("Writing to Multi Statistic File "+filename+" Completed");
#endif



	if (reading) {
		m_IsStatCompleteRead = true;
		auto pos = m_EFile.GetFilePosition();
		auto actpos = m_MultiFile.tellg();

	}

	m_EFile.Close();


	return true;
}



bool MultiStorageStatData::ReadWrite_Accepted(bool reading) {

	string str;
	bool good=m_EFile.IsGood();
	vector<float> floats;
	vector<int> ints;
	vector<size_t> sizets;
	size_t Aftercol;
	if(reading) {

		//while (m_EFile.ReadTextLine(str,Aftercol,floats, ints)) {
		//	auto koll = str;
		//}

		if (good) {
			size_t ifiles;
			m_EFile.ReadTextLine(str, ifiles, floats, ints); // Number of files
			m_pNewMap->ResidualFile_SetNumAttachedFiles(ifiles);
			m_AddedFiles.clear();
			m_AddedFiles.resize(0);
			m_AddedNumRuns.resize(0);
			m_AddedFirstRunNo.resize(0);
			if (ifiles > 0) m_EFile.ReadTextLine(str);
			size_t num, numensembles, num_accepted;
			for (unsigned long i = 0; i < ifiles; i++) {
				m_EFile.ReadTextLine( str, num, floats, ints); 
				{ 
					bool exist = FUtil::IsFileExisting(str);
					if (!exist) {
						string UserDirectory = "";
						UserDirectory = FUtil::GetProfileStringStd("UserDirectory", UserDirectory);
						int ipos = str.find("\\");
						while (ipos >= 0) {
							str = str.substr(ipos + 1);
							ipos = str.find("\\");
						}
						string testname = UserDirectory + str;
						str= testname;
					}



					m_AddedFiles.push_back(str);

				}

				m_AddedNumRuns.push_back(ints[0]);
				m_AddedFirstRunNo.push_back(ints[1]);
			}

			m_EFile.ReadTextLine(str,numensembles);
			good=m_EFile.ReadTextLine(str, m_ActualEnsemble);
			if (good) {
				if (m_AcceptedRuns.size() < numensembles) {
					m_AcceptedIdString.resize(numensembles);
					m_AcceptedRuns.resize(numensembles);
				}
				for (size_t i = 0; i < numensembles; i++) {
					m_EFile.ReadTextLine(str);
					str=str.substr(str.find(":")+1);
					 m_AcceptedIdString[i] = str;
					// num_accepted = stoi(str);
					 m_EFile.ReadTextLine(str, num_accepted);
					if (good) {
						m_AcceptedRuns[i].resize(num_accepted);
						if (num_accepted > 0) {
							m_EFile.ReadTextLine(str, num_accepted, floats, sizets);
							m_AcceptedRuns[i] = sizets;
						}
					}
					else
						m_AcceptedRuns[i].resize(0);
				}
			}
			if(!good) {
				m_AcceptedIdString.resize(0);
				m_AcceptedRuns.resize(0);
			}
			else {
				m_TotalNumAccepted = m_AcceptedRuns[m_ActualEnsemble].size();


			}
		}

		return m_EFile.IsGood();
	}
	else {
		//Writing to fil
		unsigned int ifiles = m_AddedFiles.size();

		m_EFile.WriteTextLine("Number of Files added to Basic Run ", ifiles);

		unsigned int num, num_first;
		if(ifiles>0) {
			m_EFile.WriteTextLine("File name :  Added Runs , First Added ");
		}
		for (unsigned int i = 0; i < ifiles; i++) {
            string addfile = GetNameAddedFiles(i);
			num = GetNumberOfAddedRuns(i);
			num_first = GetFirstRunNoAddedRuns(i);
			m_EFile.WriteTextLine(addfile, num , num_first);
		}
	
		size_t NumEnsembles = m_AcceptedRuns.size();
		NumEnsembles = max(size_t(1), NumEnsembles);

		m_EFile.WriteTextLine("Number of Ensembles ", NumEnsembles);
		m_EFile.WriteTextLine("Actual Ensemble ",m_ActualEnsemble);

		size_t NumCheck = m_AcceptedIdString.size();
		if (NumCheck != NumEnsembles) {
			m_AcceptedIdString.resize(NumEnsembles);
			m_AcceptedRuns.resize(NumEnsembles);
		}
		for (size_t i = 0; i < NumEnsembles; i++) {
			if (m_AcceptedIdString[i].size() == 0) {
				m_AcceptedIdString[i] = "E(" + FUtil::STD_ItoAscii(i + 1) + ")";
			}
			m_EFile.WriteTextLine("Ensemble Id ", m_AcceptedIdString[i]);

			size_t num = m_AcceptedRuns[i].size();
			if (m_TotalNumberRuns == string::npos) {
				m_TotalNumberRuns = m_pNewMap->MR_Get_NumberofRuns();
			}
			if (num == 0&&m_TotalNumberRuns>0) {
				m_AcceptedRuns[i].resize(m_TotalNumberRuns);
				for (size_t ii = 0; ii < m_TotalNumberRuns; ii++) {
					m_AcceptedRuns[i][ii] = ii;
				}
				num=m_AcceptedRuns[i].size();
				m_TotalNumAccepted = num;
			}

			if (num > 0) {
				m_EFile.WriteTextLine("Accepted Runs ", num);
				m_EFile.WriteTextLine(m_AcceptedRuns[i]);

			}
		}


		return m_EFile.IsGood();
	}
	return true;

};
bool MultiStorageStatData::ReadWrite_MultiPar(bool reading) {
	string str;
	bool good = m_EFile.IsGood();
	vector<float> floats;
	vector<int> ints;
	vector<size_t> sizets;
	size_t Aftercol;

	if (reading) {
		if (m_MBinHeader.n_param > 0) {
			if (m_ParMin.size() != m_MBinHeader.n_param) {
				m_ParMin.resize(m_MBinHeader.n_param);
				m_ParMax.resize(m_MBinHeader.n_param);
				m_ParValidMin.resize(m_MBinHeader.n_param);
				m_ParValidMax.resize(m_MBinHeader.n_param);
			}
			m_EFile.ReadTextLine(str, Aftercol);
			m_EFile.ReadTextLine(str, Aftercol, m_ParMin, ints);


			m_EFile.ReadTextLine(str, Aftercol);
			m_EFile.ReadTextLine(str, Aftercol, m_ParMax, ints);

			m_EFile.ReadTextLine(str, Aftercol);
			m_EFile.ReadTextLine(str, Aftercol, m_ParValidMin, ints);


			m_EFile.ReadTextLine(str, Aftercol);
			m_EFile.ReadTextLine(str, Aftercol, m_ParValidMax, ints);

			if (unsigned long(m_ParMin.size()) != m_MBinHeader.n_param) good = false;
			if (unsigned long(m_ParMax.size()) != m_MBinHeader.n_param) good = false;
			if (unsigned long(m_ParValidMin.size()) != m_MBinHeader.n_param) good = false;
			if (unsigned long(m_ParValidMax.size()) != m_MBinHeader.n_param) good = false;

		}
		if(good) good=m_EFile.IsGood();
	}
	else {
		// Writing
		if (m_ParMin.size() != m_MBinHeader.n_param) {
			m_ParMin.resize(m_MBinHeader.n_param);
			m_ParMax.resize(m_MBinHeader.n_param);
			m_ParValidMin.resize(m_MBinHeader.n_param);
			m_ParValidMax.resize(m_MBinHeader.n_param);
		}
		size_t n_param = m_MBinHeader.n_param;
		m_EFile.WriteTextLine("Parameter Min Values ", n_param); // Number of files
		good=m_EFile.WriteTextLine(m_ParMin);
		m_EFile.WriteTextLine("Parameter Max Values ", n_param);
		good=m_EFile.WriteTextLine(m_ParMax);
		m_EFile.WriteTextLine("Valid Min Values ", n_param);
		good=m_EFile.WriteTextLine(m_ParValidMin);
		m_EFile.WriteTextLine("Valid Max Values ", n_param);
		good=m_EFile.WriteTextLine(m_ParValidMax);
	}

	return true;
}
bool MultiStorageStatData::ReadWrite_CoCor(bool reading) {
	bool good=true;
	/*
	if (reading) {
		string str;
		getline(m_EFile.m_DocFileIn, str, ':');
		getline(m_EFile.m_DocFileIn, str);
		m_CoCorNumValid = stoi(str);
		m_CoCorValid.resize(m_CoCorNumValid*m_CoCorNumValid);
		auto num = m_CoCorValid.size();
		if(num>0) m_CoCorValid = m_EFile.rFloatArray2(num, m_CoCorNumValid);
		return m_EFile.IsFileInGood();
	}
	else {
		// Writing
		m_EFile.WriteTextLine( << "Parameter CoCorrelation : " << m_CoCorNumValid << endl;
		auto num = m_CoCorValid.size();
		if(num>0) good = m_EFile.wFloatArray2(m_CoCorValid, num, m_CoCorNumValid);

		return m_EFile.IsFileOutGood();
	}
	*/
	return good;
}
bool MultiStorageStatData::ReadWrite_Criteria(bool reading) {
	size_t num;
	size_t num_indicators = 7;
	bool good = m_EFile.IsGood();
	vector<float> floats;
	vector<int> ints;
	vector<size_t> sizets;
	size_t Aftercol;
	if (reading) {
		string str;
		VarValidAllo();
		if (m_NumValidAll != 0) {
				m_EFile.ReadTextLine(str, num);
			for (size_t i = 0; i < num; i++) {
				size_t num2;	
				m_EFile.ReadTextLine(str, num2);
				if (num2 > 0) {
					m_EFile.ReadTextLine(str, num_indicators);
					m_EFile.ReadTextLine(str, num_indicators, m_Valid_TSV_Mean[i].v[E_SELECT_LOW], ints);
					m_EFile.ReadTextLine(str, num_indicators);
					m_EFile.ReadTextLine(str, num_indicators, m_Valid_TSV_Mean[i].v[E_SELECT_HIGH], ints);
					m_EFile.ReadTextLine(str, num_indicators);
					m_EFile.ReadTextLine(str, num_indicators, m_Valid_TSV_Mean[i].v[E_POST_MIN], ints);
					m_EFile.ReadTextLine(str, num_indicators);
					m_EFile.ReadTextLine(str, num_indicators, m_Valid_TSV_Mean[i].v[E_POST_MAX], ints);
					m_EFile.ReadTextLine(str, num_indicators);
					m_EFile.ReadTextLine(str, num_indicators, m_Valid_TSV_Mean[i].v[E_POST_MEAN], ints);
				}
			}
			m_EFile.ReadTextLine(str, num);
			size_t num_all, kollens;
			for (size_t i = 0; i < num; i++) {

				
					m_EFile.ReadTextLine(str, kollens);
					m_EFile.ReadTextLine(str, num_all);
					if (num_all == m_NumValidAll) { m_EFile.ReadTextLines(num_indicators, str, num_all, m_Valid_TSV[i].v[E_SELECT_LOW], ints); m_EFile.ReadTextLine(str, num_all); };

					if (num_all == m_NumValidAll) {m_EFile.ReadTextLines(num_indicators, str, num_all, m_Valid_TSV[i].v[E_SELECT_HIGH], ints); m_EFile.ReadTextLine(str, num_all);};
					if (num_all == m_NumValidAll) {m_EFile.ReadTextLines(num_indicators, str, num_all, m_Valid_TSV[i].v[E_POST_MIN], ints); m_EFile.ReadTextLine(str, num_all);};
					if (num_all == m_NumValidAll) {m_EFile.ReadTextLines(num_indicators, str, num_all, m_Valid_TSV[i].v[E_POST_MAX], ints); m_EFile.ReadTextLine(str, num_all);};
					if (num_all == m_NumValidAll) {m_EFile.ReadTextLines(num_indicators, str, num_all, m_Valid_TSV[i].v[E_POST_MEAN], ints); m_EFile.ReadTextLine(str, num_all);};
					if (num_all == m_NumValidAll) {m_EFile.ReadTextLines(num_indicators, str, num_all, m_Valid_TSV[i].v[E_POST_CV], ints); m_EFile.ReadTextLine(str, num_all);};
					if (num_all == m_NumValidAll) {m_EFile.ReadTextLines(num_indicators, str, num_all, m_Valid_TSV[i].v[E_PRIOR_MIN], ints); m_EFile.ReadTextLine(str, num_all);};
					if (num_all == m_NumValidAll) {m_EFile.ReadTextLines(num_indicators, str, num_all, m_Valid_TSV[i].v[E_PRIOR_MAX], ints); m_EFile.ReadTextLine(str, num_all);};
					if (num_all == m_NumValidAll) {m_EFile.ReadTextLines(num_indicators, str, num_all, m_Valid_TSV[i].v[E_PRIOR_MEAN], ints); m_EFile.ReadTextLine(str, num_all);};
					if (num_all == m_NumValidAll) {m_EFile.ReadTextLines(num_indicators, str, num_all, m_Valid_TSV[i].v[E_PRIOR_CV], ints);};

					m_EFile.ReadTextLine(str, m_Valid_TSV[i].tobe_Plotted);
					m_EFile.ReadTextLine(str, m_Valid_TSV[i].is_Updated);
					m_EFile.ReadTextLine(str, m_Valid_TSV[i].VarTranformed);
					m_EFile.ReadTextLine(str, m_Valid_TSV[i].Dim1_Par_Index);

				//History of Criteria Setting
				size_t numhistory;
				m_EFile.ReadTextLine(str, numhistory);
				if (numhistory < 10000) {
					m_Valid_TSV[i].History.resize(numhistory);

					for (size_t jj = 0; jj < numhistory; jj++) {
						size_t item1;
						size_t item2;
						m_EFile.ReadTextLine(str, item1); m_Valid_TSV[i].History[jj].criteria=CRITERIA_STATE(item1); // CRITERIA_STATE(item1);
						m_EFile.ReadTextLine(str, item2); m_Valid_TSV[i].History[jj].stat_indicator = PERFORMANCE_VAR(item2);
						m_EFile.ReadTextLine(str, m_Valid_TSV[i].History[jj].n_post);
						m_EFile.ReadTextLine(str, m_Valid_TSV[i].History[jj].VarIndex);
						m_EFile.ReadTextLine(str, m_Valid_TSV[i].History[jj].minvalue);
						m_EFile.ReadTextLine(str, m_Valid_TSV[i].History[jj].maxvalue);
					}
				}
				m_pNewMap->m_MStorage.IsMinUpdated[i] = true;
				m_pNewMap->m_MStorage.IsMaxUpdated[i] = true;
				m_pNewMap->m_MStorage.IsCVUpdated[i] = true;
				m_pNewMap->m_MStorage.IsMeanUpdated[i] = true;
			}

		}
		/*if (m_pNewMap->m_ValSum_Array.size() > 0) {
			m_EFile.rsize_t(&num);
			if (num != m_Valid_SV.size()) m_Valid_SV.resize(num);
			for (size_t i = 0; i < m_Valid_SV.size(); i++) {
				m_EFile.rsize_t(&num);
				if (num == 0) {
					m_Valid_SV.resize(num);
					continue;
				}
				AllocateValid_SUM(i);
				auto values = m_EFile.rsize_tArray(num);
				m_Valid_TSV[i].criteria.resize(values.size());
				for (size_t j = 0; j < values.size(); j++) m_Valid_SV[i].criteria[j] = CRITERIA_STATE(values[j]);

				m_Valid_SV[i].v[E_SELECT_LOW] = m_EFile.rFloatArray2(num);
				m_Valid_SV[i].v[E_SELECT_HIGH] = m_EFile.rFloatArray2(num);
				m_Valid_SV[i].v[E_POST_MIN] = m_EFile.rFloatArray2(num);
				m_Valid_SV[i].v[E_POST_MAX] = m_EFile.rFloatArray2(num);
				m_Valid_SV[i].v[E_POST_MEAN] = m_EFile.rFloatArray2(num);
				m_Valid_SV[i].v[E_POST_CV] = m_EFile.rFloatArray2(num);
				m_EFile.rbool(&m_Valid_SV[i].tobe_Plotted);
				m_EFile.rbool(&m_Valid_SV[i].is_Updated);
				m_EFile.rsize_t(&m_Valid_SV[i].VarTranformed);
				m_EFile.rsize_t(&num);
				m_pNewMap->m_MStorage.m_SV_StatMemory.AcceptedRuns = true;

				if (m_Valid_SV[i].criteria.size() > 7 && num == 7) {
					m_Valid_SV_Mean[i].criteria.resize(num);

					auto values = m_EFile.rsize_tArray(m_NumValidAll);
					m_Valid_SV_Mean[i].criteria.resize(values.size());
					for (size_t j = 0; j < values.size(); j++) m_Valid_SV_Mean[i].criteria[j] = CRITERIA_STATE(values[j]);
					m_Valid_SV_Mean[i].v[E_SELECT_LOW] = m_EFile.rFloatArray2(num);
					m_Valid_SV_Mean[i].v[E_SELECT_HIGH] = m_EFile.rFloatArray2(num);
					m_Valid_SV_Mean[i].v[E_POST_MIN] = m_EFile.rFloatArray2(num);
					m_Valid_SV_Mean[i].v[E_POST_MAX] = m_EFile.rFloatArray2(num);
					m_Valid_SV_Mean[i].v[E_POST_MEAN] = m_EFile.rFloatArray2(num);
					m_Valid_SV_Mean[i].v[E_POST_CV] = m_EFile.rFloatArray2(num);
				}
			}

			// Prior statistics for all runs similar for all ensembles
			if (m_Valid_SV.size() > 0) {
				size_t SV_VarNum = m_Valid_SV[0].criteria.size();
				m_EFile.rsize_t(&num);
				if (m_EFile.IsFileInGood() && SV_VarNum == num) {
					m_Valid_SV[0].v[E_PRIOR_MIN] = m_EFile.rFloatArray2(num);
					m_Valid_SV[0].v[E_PRIOR_MAX] = m_EFile.rFloatArray2(num);
					m_Valid_SV[0].v[E_PRIOR_MEAN] = m_EFile.rFloatArray2(num);
					m_Valid_SV[0].v[E_PRIOR_CV] = m_EFile.rFloatArray2(num);
					for (size_t i = 1; i < m_Valid_SV.size(); i++) {
						m_Valid_SV[i].v[E_PRIOR_MIN] = m_Valid_SV[0].v[E_PRIOR_MIN];
						m_Valid_SV[i].v[E_PRIOR_MAX] = m_Valid_SV[0].v[E_PRIOR_MAX];
						m_Valid_SV[i].v[E_PRIOR_MEAN] = m_Valid_SV[0].v[E_PRIOR_MEAN];
						m_Valid_SV[i].v[E_PRIOR_CV] = m_Valid_SV[0].v[E_PRIOR_CV];
					}
					m_pNewMap->m_MStorage.m_SV_StatMemory.AllRuns = true;
				}
			}

			if (m_pNewMap->m_ValSum_Array.size() > 1) {
				m_EFile.rsize_t(&num);
				if (num == 7) {
					m_Valid_SV_Mean[0].v[E_PRIOR_MIN] = m_EFile.rFloatArray2(num);
					m_Valid_SV_Mean[0].v[E_PRIOR_MAX] = m_EFile.rFloatArray2(num);
					m_Valid_SV_Mean[0].v[E_PRIOR_MEAN] = m_EFile.rFloatArray2(num);
					m_Valid_SV_Mean[0].v[E_PRIOR_CV] = m_EFile.rFloatArray2(num);



					for (size_t i = 1; i < m_Valid_SV.size(); i++) {
						m_Valid_SV_Mean[i].v[E_PRIOR_MIN] = m_Valid_SV_Mean[0].v[E_PRIOR_MIN];
						m_Valid_SV_Mean[i].v[E_PRIOR_MAX] = m_Valid_SV_Mean[0].v[E_PRIOR_MAX];
						m_Valid_SV_Mean[i].v[E_PRIOR_MEAN] = m_Valid_SV_Mean[0].v[E_PRIOR_MEAN];
						m_Valid_SV_Mean[i].v[E_PRIOR_CV] = m_Valid_SV_Mean[0].v[E_PRIOR_CV];
					}
					m_pNewMap->m_MStorage.m_SV_StatMemory.AllRuns = true;
				}
			}
		}*/

		return m_EFile.IsGood();
	}
	else {

		unsigned int num;
		if (m_NumValidAll != 0) {
			num = m_Valid_TSV_Mean.size();
			m_EFile.WriteTextLine("Number of Ensembles ", num);
			for (size_t i = 0; i < num; i++) {
				if (good && m_NumValidAll > 0) {
					m_EFile.WriteTextLine("Ensemble No  ", i + 1);
					m_EFile.WriteTextLine("Low Mean Criteria ", num_indicators);
					m_EFile.WriteTextLine(m_Valid_TSV_Mean[i].v[E_SELECT_LOW]);
					m_EFile.WriteTextLine("High Mean Criteria ", num_indicators);
					m_EFile.WriteTextLine(m_Valid_TSV_Mean[i].v[E_SELECT_HIGH]);
					m_EFile.WriteTextLine("Posterior Min Mean Criteria ", num_indicators);
					m_EFile.WriteTextLine(m_Valid_TSV_Mean[i].v[E_POST_MIN]);
					m_EFile.WriteTextLine("Posterior High Mean Criteria ", num_indicators);
					m_EFile.WriteTextLine(m_Valid_TSV_Mean[i].v[E_POST_MAX]);
					m_EFile.WriteTextLine("Posterior mean of Mean Value", num_indicators);
					m_EFile.WriteTextLine(m_Valid_TSV_Mean[i].v[E_POST_MEAN]);
				}

			}

			num = m_Valid_TSV.size();
			m_EFile.WriteTextLine("Number of Ensembles ", num);
			size_t row_size = m_NumValidAll / num_indicators;
			size_t NumValidAll = m_NumValidAll;
			for (size_t i = 0; i < num; i++) {
				if (good&&m_NumValidAll > 0) {
					m_EFile.WriteTextLine("Ensemble No  ", i + 1);
					m_EFile.WriteTextLine("Low Criteria ", NumValidAll);
					m_EFile.WriteTextLine(m_Valid_TSV[i].v[E_SELECT_LOW],  row_size); m_EFile.WriteTextLine("High Criteria ",NumValidAll);
					m_EFile.WriteTextLine(m_Valid_TSV[i].v[E_SELECT_HIGH], row_size); m_EFile.WriteTextLine( "Posterior Min of Value ",NumValidAll);
					m_EFile.WriteTextLine(m_Valid_TSV[i].v[E_POST_MIN],row_size); m_EFile.WriteTextLine( "Posterior Max of Value ",NumValidAll);
					m_EFile.WriteTextLine(m_Valid_TSV[i].v[E_POST_MAX],row_size); m_EFile.WriteTextLine("Posterior Mean of Value ",NumValidAll);
					m_EFile.WriteTextLine(m_Valid_TSV[i].v[E_POST_MEAN],row_size); m_EFile.WriteTextLine("Posterior CV of Value ",NumValidAll);
					m_EFile.WriteTextLine(m_Valid_TSV[i].v[E_POST_CV],row_size); m_EFile.WriteTextLine("Prior Min of Value ",NumValidAll);
					m_EFile.WriteTextLine(m_Valid_TSV[i].v[E_PRIOR_MIN],row_size); m_EFile.WriteTextLine("Prior Max of Value ",NumValidAll);
					m_EFile.WriteTextLine(m_Valid_TSV[i].v[E_PRIOR_MAX],row_size); m_EFile.WriteTextLine("Prior Mean of Value ",NumValidAll);
					m_EFile.WriteTextLine(m_Valid_TSV[i].v[E_PRIOR_MEAN],row_size);	m_EFile.WriteTextLine("Prior CV Value ",NumValidAll);
					m_EFile.WriteTextLine(m_Valid_TSV[i].v[E_PRIOR_CV],row_size);
				}
				m_EFile.WriteTextLine("Enable for plotting  ", m_Valid_TSV[i].tobe_Plotted);
				m_EFile.WriteTextLine("Is Updated ",m_Valid_TSV[i].is_Updated);
				m_EFile.WriteTextLine("Transformed Variables Indicator ", m_Valid_TSV[i].VarTranformed);
				m_EFile.WriteTextLine("Dimension 1 Index  ", m_Valid_TSV[i].Dim1_Par_Index);
				//History of Criteria Setting
				size_t numhistory = m_Valid_TSV[i].History.size();
				m_EFile.WriteTextLine("Number of History  ", numhistory);
				for (size_t jj = 0; jj < numhistory; jj++) {
					bool f_valid = m_Valid_TSV[i].History[jj].criteria;
					m_EFile.WriteTextLine( "Criteria ", f_valid);
					size_t f_ind= m_Valid_TSV[i].History[jj].stat_indicator;
					m_EFile.WriteTextLine( "Statistical Performance Indicator : ", f_ind);
					m_EFile.WriteTextLine( "Remaining runs : ", m_Valid_TSV[i].History[jj].n_post);
					m_EFile.WriteTextLine( "Var Index : ", m_Valid_TSV[i].History[jj].VarIndex);
					m_EFile.WriteTextLine( "Min value  ", m_Valid_TSV[i].History[jj].minvalue);
					m_EFile.WriteTextLine(  "Max value  ", m_Valid_TSV[i].History[jj].maxvalue);
				}
				

			}
		}/*
		num = m_Valid_SV.size();
		if (m_pNewMap->m_ValSum_Array.size() > 0) {
			good = m_EFile.wInt(&num);
			for (size_t i = 0; i < num; i++) {
				size_t SV_VarNum = m_Valid_SV[i].criteria.size();
				m_EFile.wSize_t(&SV_VarNum);
				size_t numval = 7;
				if (m_EFile.IsFileOutGood() && SV_VarNum > 0) {

					vector<size_t> values;
					values.resize(m_Valid_SV[i].criteria.size());
					for (size_t j = 0; j < values.size(); j++) values[j] = size_t(m_Valid_SV[i].criteria[j]);
					m_EFile.wsize_tArray(values, m_NumValidAll);
					//m_EFile.wsize_tArray(&m_Valid_SV[i].criteria, SV_VarNum);
					m_EFile.wFloatArray2(m_Valid_SV[i].v[E_SELECT_LOW]);
					m_EFile.wFloatArray2(m_Valid_SV[i].v[E_SELECT_HIGH]);
					m_EFile.wFloatArray2(m_Valid_SV[i].v[E_POST_MIN]);
					m_EFile.wFloatArray2(m_Valid_SV[i].v[E_POST_MAX]);
					m_EFile.wFloatArray2(m_Valid_SV[i].v[E_POST_MEAN]);
					m_EFile.wFloatArray2(m_Valid_SV[i].v[E_POST_CV]);
					m_EFile.wbool(&m_Valid_SV[i].tobe_Plotted);
					m_EFile.wbool(&m_Valid_SV[i].is_Updated);
					m_EFile.wSize_t(&m_Valid_SV[i].VarTranformed);
					m_EFile.wSize_t(&numval);
				}
				if (SV_VarNum > 7) {
					if (m_EFile.IsFileOutGood()) {

						vector<size_t> values;
						values.resize(m_Valid_SV_Mean[i].criteria.size());
						for (size_t j = 0; j < values.size(); j++) values[j] = size_t(m_Valid_SV_Mean[i].criteria[j]);
						m_EFile.wsize_tArray(values, m_NumValidAll);

						//m_EFile.wsize_tArray(&m_Valid_SV_Mean[i].criteria, numval);
						m_EFile.wFloatArray2(m_Valid_SV_Mean[i].v[E_SELECT_LOW]);
						m_EFile.wFloatArray2(m_Valid_SV_Mean[i].v[E_SELECT_HIGH]);
						m_EFile.wFloatArray2(m_Valid_SV_Mean[i].v[E_POST_MIN]);
						m_EFile.wFloatArray2(m_Valid_SV_Mean[i].v[E_POST_MAX]);
						m_EFile.wFloatArray2(m_Valid_SV_Mean[i].v[E_POST_MEAN]);
						m_EFile.wFloatArray2(m_Valid_SV_Mean[i].v[E_POST_CV]);
					}
				}
			}

			// Prior statistics for all runs similar for all ensembles
			size_t SV_VarNum = m_Valid_SV[0].criteria.size();
			m_EFile.wSize_t(&SV_VarNum);
			if (m_EFile.IsFileOutGood() && SV_VarNum > 0) {
				m_EFile.wFloatArray2(m_Valid_SV[0].v[E_PRIOR_MIN]);
				m_EFile.wFloatArray2(m_Valid_SV[0].v[E_PRIOR_MAX]);
				m_EFile.wFloatArray2(m_Valid_SV[0].v[E_PRIOR_MEAN]);
				m_EFile.wFloatArray2(m_Valid_SV[0].v[E_PRIOR_CV]);
			}
			size_t  ivar = m_pNewMap->m_ValSum_Array.size();
			if (ivar > 1) {
				size_t numval = 7;
				m_EFile.wSize_t(&numval);
				if (m_EFile.IsFileOutGood()) {
					m_EFile.wFloatArray2(m_Valid_SV_Mean[0].v[E_PRIOR_MIN]);
					m_EFile.wFloatArray2(m_Valid_SV_Mean[0].v[E_PRIOR_MAX]);
					m_EFile.wFloatArray2(m_Valid_SV_Mean[0].v[E_PRIOR_MEAN]);
					m_EFile.wFloatArray2(m_Valid_SV_Mean[0].v[E_PRIOR_CV]);
				}
			}
		}

		// Writing

		return m_EFile.IsFileOutGood();

		*/
	}
return true;
}
bool MultiStorageStatData::ReadWrite_OptionalTSV_Valid(bool reading) {
	size_t num;
	size_t num_indicators = 7;
	bool good = m_EFile.IsGood();
	vector<float> floats;
	vector<int> ints;
	vector<size_t> sizets;
	size_t Aftercol;

#ifndef COUPSTD
	MFC_Util mfc_util;
#endif
	if(m_NumValidAll>0)
		
		if (reading) {
			string str;

			m_EFile.ReadTextLine(str, m_CumulatedPerformance);
			if (m_CumulatedPerformance > 100) return false;
			m_EFile.ReadTextLine(str, num);
			VARTRANSFORMED add;
			size_t koll;
			size_t numrow;
			m_Ensembles.resize(num);
			for(size_t i = 0; i < num; i++) {
				m_Ensembles[i].valid = true;
				m_Ensembles[i].EnsembleFileRead = true;	
				m_EFile.ReadTextLine(str, koll);
				m_EFile.ReadTextLine(str, add.Transformed_index);
				m_EFile.ReadTextLine(str, add.Dim1_index);
				size_t numvector;
				m_EFile.ReadTextLine(str, numvector);
				numrow=numvector/m_NumValidAll;
#ifndef COUPSTD
				auto act = FUtil::STD_ItoAscii(i + 1);
				auto tot = FUtil::STD_ItoAscii(num);
				mfc_util.SetPaneMessage("Reading Ensembles " + act + "(" + tot + ")");

#endif

				m_EFile.ReadTextLines(numrow, str, numvector, add.VarValidVector,ints);
				m_VarValidTransformed.push_back(add);
				MapOfStoredTSV.insert(pair<size_t,size_t>(add.Transformed_index, m_VarValidTransformed.size() - 1));

			}
			return m_EFile.IsGood();
		}
		else {
			m_EFile.WriteTextLine("Cumulated Performance ",m_CumulatedPerformance);
				size_t num;
			num = m_VarValidTransformed.size();
	
			//m_EFile.wSize_t(&num);
			if (num == 0 && m_CumulatedPerformance == 0&& m_VarValid.size()>0) {
				//Original Performance indicators to archive
				VARTRANSFORMED vartrans;
				vartrans.Transformed_index = 0;
				vartrans.VarValidVector = m_VarValid;
				m_VarValidTransformed.push_back(vartrans);
				MapOfStoredTSV.insert(pair<size_t, size_t>(vartrans.Transformed_index, 0));
			}
			num = m_VarValidTransformed.size();
			m_EFile.WriteTextLine("Number of Transformed Ensembles ", num);
//         Check if VarValidVector has same size as m_VarValid

			for (size_t i = 0; i < m_VarValidTransformed.size(); i++) {

#ifndef COUPSTD
				auto act=FUtil::STD_ItoAscii(i + 1);
				auto tot=FUtil::STD_ItoAscii(num);
				mfc_util.SetPaneMessage("Writing Ensembles " + act + "("+tot + ")");

#endif

				m_EFile.WriteTextLine("Transformed Ensemble ",i+1);
				m_EFile.WriteTextLine("Transform Index ",m_VarValidTransformed[i].Transformed_index);
				m_EFile.WriteTextLine("Dim1 Index ",m_VarValidTransformed[i].Dim1_index);
				num = m_VarValidTransformed[i].VarValidVector.size();
				m_EFile.WriteTextLine("VarValid Entire Vector ",num);
				if (!m_EFile.WriteTextLine(m_VarValidTransformed[i].VarValidVector, m_NumValidAll)) {
					TRACE("MultiStorage  Error in writing VarValid Entire Vector &d \n", i);
				};
			}

			return m_EFile.IsGood();
		}
	return m_EFile.IsGood();

	return true;
}
bool MultiStorageStatData::ReadWrite_OptionalSUM_Valid(bool reading) {
	/*
	bool good;
	size_t totnumruns;
	if (reading) {
		good = m_EFile.rsize_t(&totnumruns);
		if(good)
			for (size_t i = 0; i < m_pNewMap->m_ValSum_Array.size(); i++) {
				auto valsum = m_pNewMap->GetValSumStruct(i);
				valsum.ME.resize(totnumruns);
				valsum.R2_Multi.resize(totnumruns);
				valsum.InterCept_Multi.resize(totnumruns);
				valsum.Slope_Multi.resize(totnumruns);
				valsum.RMSE.resize(totnumruns);
				valsum.NSE.resize(totnumruns);
				valsum.LogLi.resize(totnumruns);
				valsum.SimValueVector.resize(totnumruns);
				valsum.ObsValueVector.resize(totnumruns);

				valsum.ME = m_EFile.rFloatArray2(totnumruns);
				valsum.R2_Multi = m_EFile.rFloatArray2(totnumruns);
				valsum.InterCept_Multi = m_EFile.rFloatArray2(totnumruns);
				valsum.Slope_Multi = m_EFile.rFloatArray2(totnumruns);
				valsum.RMSE = m_EFile.rFloatArray2(totnumruns);
				valsum.NSE = m_EFile.rFloatArray2(totnumruns);
				valsum.LogLi = m_EFile.rFloatArray2(totnumruns);
				valsum.SimValueVector = m_EFile.rFloatArray2(totnumruns);
				valsum.ObsValueVector = m_EFile.rFloatArray2(totnumruns);
				m_EFile.rInt(&valsum.n_RegMulti);
				m_pNewMap->SetValSum(i, valsum);
			}
			m_pNewMap->CalculateMeanPerformanceAndSetValues();
			m_pNewMap->m_IsPerformanceSmartFlexibleFile_Read = true;

			if (m_pNewMap->m_ValSum_Array.size() > 1) {

				for (size_t ivar = 0; ivar < 7; ivar++) {
					if (m_Valid_SV_Mean[0].MeanPerFomanceIndicators[ivar].size() != totnumruns) m_Valid_SV_Mean[0].MeanPerFomanceIndicators[ivar].resize(totnumruns);
					m_Valid_SV_Mean[0].MeanPerFomanceIndicators[ivar] = m_EFile.rFloatArray2(totnumruns);
				}
				for (size_t i = 1; i < m_Valid_SV_Mean.size(); i++) {
					for (size_t ivar = 0; ivar < 7; ivar++) {
						if (m_Valid_SV_Mean[i].MeanPerFomanceIndicators[ivar].size() != totnumruns) m_Valid_SV_Mean[i].MeanPerFomanceIndicators[ivar].resize(totnumruns);
						m_Valid_SV_Mean[i].MeanPerFomanceIndicators[ivar] = m_Valid_SV_Mean[0].MeanPerFomanceIndicators[ivar];
					}
				}

				m_pNewMap->m_Are_MeanValues_SV_VariablesEstimated = true;
			}


		return true;// m_EFile.IsFileInGood();
	}
	else {
		if (m_pNewMap->m_ValSum_Array.size() > 0) {
			auto valsum = m_pNewMap->GetValSumStruct(0);
			totnumruns = valsum.ME.size();			
			while(totnumruns != m_TotalNumberRuns) {
				m_pNewMap->m_IsPerformanceSmartFlexibleFile_Read = false;
				m_pNewMap->m_Are_MeanValues_SV_VariablesEstimated =false;
				m_pNewMap->CalculatePerformanceAndSetValues();
				m_pNewMap->m_Are_MeanValues_SV_VariablesEstimated = false;
				m_pNewMap->CalculateSumMeanValidValues_SV_Variables();

				valsum = m_pNewMap->GetValSumStruct(0);
				totnumruns = valsum.ME.size();


			}
			m_EFile.wSize_t(&totnumruns);
			for (size_t i = 0; i < m_pNewMap->m_ValSum_Array.size(); i++) {
				auto valsum = m_pNewMap->GetValSumStruct(i);
				m_EFile.wFloatArray2(valsum.ME);
				m_EFile.wFloatArray2(valsum.R2_Multi);
				m_EFile.wFloatArray2(valsum.InterCept_Multi);
				m_EFile.wFloatArray2(valsum.Slope_Multi);
				m_EFile.wFloatArray2(valsum.RMSE);
				m_EFile.wFloatArray2(valsum.NSE);
				m_EFile.wFloatArray2(valsum.LogLi);
				m_EFile.wFloatArray2(valsum.SimValueVector);
				m_EFile.wFloatArray2(valsum.ObsValueVector);

				m_EFile.wInt(&valsum.n_RegMulti);
			}
			if (m_pNewMap->m_ValSum_Array.size() > 1) {
				for (size_t ivar = 0; ivar < 7; ivar++)
					m_EFile.wFloatArray2(m_Valid_SV_Mean[0].MeanPerFomanceIndicators[ivar]);
				
			}

		}


		return m_EFile.IsFileOutGood();
	}*/
	return true;
}
bool MultiStorageStatData::ReadWrite_Parameters(bool reading) {
	if (reading) {


	}
	else {


		return m_EFile.IsGood();
	}
	return m_EFile.IsGood();

}
bool MultiStorageStatData::CompleteProcessingOfInputFiles_MultiStorage() {
	AllocateMeanValidVar();

	SetActualPerformanceVector_TSV_Variables();
	if (m_TotalNumAccepted <= 0 || m_TotalNumAccepted > m_TotalNumberRuns) {
		m_TotalNumAccepted = m_TotalNumberRuns;
		//All are accepted if not defined
		m_Accepted.resize(m_TotalNumberRuns);
		for (unsigned long i = 0; i < m_TotalNumberRuns; i++)
			SetAcceptedRunNo(i, true);
		if (m_AcceptedRuns.size() == 0) m_AcceptedRuns.resize(1);
		
		m_AcceptedRuns[m_AcceptedRuns.size() - 1].resize(m_TotalNumberRuns);

		for (unsigned long i = 0; i < m_TotalNumberRuns; i++) {
			m_Accepted[i] = i;
			m_AcceptedRuns[m_AcceptedRuns.size() - 1][i] = i;
		}
		

	}
	else
		SetActual_AcceptedEnsemble();
	
	if(m_NumValidVar>0) CalculateMeanValidValues();

	if (m_pNewMap->GetNumberofSumVariables() > 0&&!m_pNewMap->m_IsPerformanceSmartFlexibleFile_Read) {
		while (!m_pNewMap->CalculateSumMeanValidValues_SV_Variables()) {
			EvaluateFile(true);
			m_pNewMap->UpdateValidationPerformance_SmartFlexVariables();
				
		};

		m_pNewMap->m_MStorage.Update_SV_VarStat(false);
	}

	if(m_NumValidVar>0) UpdateMeanValidValues();  // TSV Variables
	//UpdateMeanValidValues(true);  //SV Variables

	m_pNewMap->m_MStorage.Set_Update_ValidVar();
	m_pNewMap->MR_ReCalculatePostDist(m_pNewMap->IsBayesianCalibration());

	return true;
}
bool MultiStorageStatData::Reset_AcceptedRuns(size_t actens)
{
	m_Accepted.resize(m_TotalNumberRuns/m_CyclicNumber);
	for (unsigned long i = 0; i < m_TotalNumberRuns / m_CyclicNumber; i++) {
		size_t valid_runno = i * m_CyclicNumber + m_CyclicStartPos;
		SetAcceptedRunNo(valid_runno, true);
	}
	if (m_AcceptedRuns.size()<=actens) return false;

	m_AcceptedRuns[actens].resize(m_TotalNumberRuns/m_CyclicNumber);

	for (unsigned long i = 0; i < m_TotalNumberRuns/m_CyclicNumber; i++) {
		size_t valid_runno = i * m_CyclicNumber + m_CyclicStartPos;
		m_Accepted[i] = valid_runno;
		m_AcceptedRuns[actens][i] = valid_runno;
	}


	return false;
}
void MultiStorageStatData::UpdateMeanValidValues(bool SV_Variables)
{
	if (SV_Variables) {
		if (m_pNewMap->GetNumberofSumVariables() <= 0) return;
		if (!m_pNewMap->Are_SV_Values_Defined()) return;
		if (m_Valid_SV_Mean[m_ActualEnsemble].MeanPerFomanceIndicators[0].size() == 0) {
			m_pNewMap->m_Are_MeanValues_SV_VariablesEstimated = false;
			if (!m_pNewMap->CalculateSumMeanValidValues_SV_Variables()) return;
		};

		
		


	}
	else if (m_NumValidVar <= 0) return;

	SimpleDistStat stat;
	size_t firstensemble = 0;
#ifndef COUPSTD
	MFC_Util mfc_util;
#endif

		

	auto fix= [&](bool all, size_t iv) {
#ifndef COUPSTD
		if(all)
			mfc_util.SetPaneMessage("Estimating Statistics for Mean of all SV  Performance Indicators - Val Var " + FUtil::STD_ItoAscii(iv));
		else
			mfc_util.SetPaneMessage("Estimating Statistics for Mean of accepted TSV  Performance Indicators"+ FUtil::STD_ItoAscii(iv));
#endif
		float vmin = 1.1E38f;
		float vmax = float(MISSING);
		float fval;
		vector<float> sample;
		size_t numruns;
		if (all) numruns = m_TotalNumberRuns;
		else numruns = m_TotalNumAccepted;

		sample.clear();
		for (unsigned long i = 0; i<numruns; i++) {
			size_t irun;
			if (all) irun = i;
			else {
				if (m_AcceptedRuns[m_ActualEnsemble].size() > i)
					irun = m_AcceptedRuns[m_ActualEnsemble][i];
				else
					continue;
			}
			if (SV_Variables) 
				fval = m_Valid_SV_Mean[firstensemble].MeanPerFomanceIndicators[iv][irun];
			else
				fval = m_MeanTSV_All[iv][irun];
			sample.push_back(fval);
			if (fval>MISSING) {
				if (fval<vmin)
					vmin = fval;
				if (fval>vmax)
					vmax = fval;
			}
		}

		size_t icount;
		SStat(&sample[0], sample.size(), &icount, &stat);
		if (SV_Variables) {
			if (icount > 1) {
				if (all) {
					m_Valid_SV_Mean[firstensemble].v[E_PRIOR_MEAN][iv] = stat.Mean;
					m_Valid_SV_Mean[firstensemble].v[E_PRIOR_CV][iv] = stat.Std;
					m_Valid_SV_Mean[firstensemble].v[E_PRIOR_MIN][iv] = stat.Min;
					m_Valid_SV_Mean[firstensemble].v[E_PRIOR_MAX][iv] = stat.Max;
					m_Valid_SV_Mean[firstensemble].v[E_SELECT_LOW][iv] = stat.Min;
					m_Valid_SV_Mean[firstensemble].v[E_SELECT_HIGH][iv] = stat.Max;


				}
				else {
					m_Valid_SV_Mean[firstensemble].v[E_POST_MEAN][iv] = stat.Mean;
					m_Valid_SV_Mean[firstensemble].v[E_POST_CV][iv] = stat.Std;
					m_Valid_SV_Mean[firstensemble].v[E_POST_MIN][iv] = stat.Min;
					m_Valid_SV_Mean[firstensemble].v[E_POST_MAX][iv] = stat.Max;
				}
			}
		}
		else {
			if (all) {
				m_MeanTSV_STAT[PRIOR_MIN][iv] = vmin;
				m_MeanTSV_STAT[PRIOR_MAX][iv] = vmax;
			}
			else {
				m_MeanTSV_STAT[POST_MIN][iv] = vmin;
				m_MeanTSV_STAT[POST_MAX][iv] = vmax;
				m_MeanTSV_Accepted[iv] = sample;
			}
		}
	};

	for (unsigned long iv = 0; iv < 7; iv++) {
		fix(false, iv);
	}
	
	for (unsigned long iv = 0; iv < 7; iv++) {
		fix(true, iv);

	}
	
#ifndef COUPSTD
	if (SV_Variables)
		mfc_util.SetPaneMessage("Estimating Statistics for Mean of all SV  Performance Indicators - Completed");
	else
		mfc_util.SetPaneMessage("Estimating Statistics for Mean of accepted TSV  Performance Indicators - Completed");
#endif


}


bool MultiStorageStatData::SetActualPerformanceVector_TSV_Variables(size_t ActualCumulatedPerformance, size_t dim1_index) {
	size_t newindex;
	if (MapOfStoredTSV.empty()) 
		return false;
	else {
		
		if (ActualCumulatedPerformance != string::npos) {
			auto it = MapOfStoredTSV.find(ActualCumulatedPerformance);
			if (it != MapOfStoredTSV.end()) 
				newindex = (*it).second;
			else
				return false;
			m_CumulatedPerformance = ActualCumulatedPerformance;
		}
		else {
			auto it = MapOfStoredTSV.find(m_CumulatedPerformance);
			if (it != MapOfStoredTSV.end()) {
				newindex = (*it).second;
			}
			else
				return false;
		}
	}
	if (newindex < m_VarValidTransformed.size() &&( m_VarValid.size() / 7 == m_VarValidTransformed[newindex].VarValidVector.size() / 6|| m_VarValid.size()== m_VarValidTransformed[newindex].VarValidVector.size())) {
		for (size_t i = 0; i < m_VarValidTransformed[newindex].VarValidVector.size();i++)
			m_VarValid[i] = m_VarValidTransformed[newindex].VarValidVector[i];
		m_Valid_TSV[m_ActualEnsemble].VarTranformed = m_CumulatedPerformance;
		if (dim1_index != string::npos) m_Valid_TSV[m_ActualEnsemble].Dim1_Par_Index = dim1_index;
	}
	else
		return false; //Keep m_VarValid as the original values

	return true;
}
size_t MultiStorageStatData::GetActualTransformation(size_t EnsembleNo)
{
	if (m_Valid_TSV.size() > EnsembleNo) {
		return m_Valid_TSV[EnsembleNo].VarTranformed;
	}
	else
		return 0;
}
bool MultiStorageStatData::SetActual_AcceptedEnsemble() {

	for (unsigned long i = 0; i<m_TotalNumberRuns; i++) 
		SetAcceptedRunNo(i, false);

	m_Accepted.resize(0);

	for (size_t i = 0; i <m_AcceptedRuns[m_ActualEnsemble].size(); i++) {
		size_t runno = m_AcceptedRuns[m_ActualEnsemble][i];
		if (runno < m_TotalNumberRuns) {
			SetAcceptedRunNo(runno, true);
		}
	}
	if (m_AcceptedIdString.size() < m_AcceptedRuns.size())
		m_AcceptedIdString.resize(m_AcceptedRuns.size());



	return true;
}
bool MultiStorageStatData::Check_Change_PerformanceIndicator_ToActual_Ensemble() {
	if (m_CumulatedPerformance != m_Valid_TSV[m_ActualEnsemble].VarTranformed) {
		m_CumulatedPerformance = m_Valid_TSV[m_ActualEnsemble].VarTranformed;
		return SetActualPerformanceVector_TSV_Variables();
	}
	return false;
}

