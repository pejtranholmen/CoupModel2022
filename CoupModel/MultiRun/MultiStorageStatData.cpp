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
		bool good;
		if (reading)
			good=m_EvaluationFile.OpenDocFileIn(filename);
		else
			good=m_EvaluationFile.OpenDocFileout(filename);
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



	if (!ReadWrite_Accepted(reading)) return false;
#ifndef COUPSTD 
	if (reading)
		mfc_util.SetPaneMessage("Reading Accepted Run Completed");
	else
		mfc_util.SetPaneMessage("Writing Accepted Runs Completed");
#endif
	if (!ReadWrite_MultiPar(reading)) return false;
	if (!ReadWrite_CoCor(reading)) return false;



	if (!ReadWrite_Criteria(reading)) return false;
#ifndef COUPSTD 
	if (reading)
		mfc_util.SetPaneMessage("Reading Criteria settings Completed");
	else
		mfc_util.SetPaneMessage("Writing Criteria settings Completed");
#endif

	bool error = false;

	if (!ReadWrite_OptionalTSV_Valid(reading)) {
		error = true;
#ifndef COUPSTD
		mfc_util.SetPaneMessage("Error when Reading TSV data");
#endif
		if (reading) {
			m_EvaluationFile.CloseFileIn();
			return true;
		}

	}
		
#ifndef COUPSTD 
	if (reading) 
		mfc_util.SetPaneMessage("Reading TSV data Completed");
	else
		mfc_util.SetPaneMessage("Writing TSV data Completed");
#endif



	if (!ReadWrite_OptionalSUM_Valid(reading)) return false;

#ifndef COUPSTD 
	if (reading)
		mfc_util.SetPaneMessage("Reading From Multi Statitics File "+filename+" Completed");
	else
		mfc_util.SetPaneMessage("Writing to Multi Statistic File "+filename+" Completed");
#endif



	if (reading) {
		m_IsStatCompleteRead = true;
		auto pos = m_EvaluationFile.GetCurrentInPosition();
		auto actpos = m_MultiFile.tellg();

	}

	if (reading) {
		m_EvaluationFile.CloseFileIn();
	

	}
	else {
		m_EvaluationFile.CloseFileOut();
	
	}

	return true;
}



bool MultiStorageStatData::ReadWrite_Accepted(bool reading) {

	string str;
	bool good;
	//if (!m_EvaluationFile.good()) return false;
	if (reading) {
		good = true;
		unsigned int ifiles;
		auto getnext_str= [&]() {
			getline(m_EvaluationFile.m_DocFileIn, str, ':');
			getline(m_EvaluationFile.m_DocFileIn, str);
		};

		//getline(m_EvaluationFile.m_DocFileIn, str,':');
		//getline(m_EvaluationFile.m_DocFileIn, str);
		getline(m_EvaluationFile.m_DocFileIn, str, ':');
		if(str.find("Number of Files added to Basic Run")==string::npos) return false;
		getline(m_EvaluationFile.m_DocFileIn, str);
		ifiles = stoi(str);


		//good=m_EvaluationFile.ruint(&ifiles);
		if (good) {
			m_pNewMap->ResidualFile_SetNumAttachedFiles(ifiles);
			m_AddedFiles.clear();
			m_AddedFiles.resize(0);
			m_AddedNumRuns.resize(0);
			m_AddedFirstRunNo.resize(0);
			if (ifiles > 0) getline(m_EvaluationFile.m_DocFileIn, str);
			size_t num, numensembles, num_accepted;
			for (unsigned long i = 0; i < ifiles; i++) {
				getline(m_EvaluationFile.m_DocFileIn, str, ',');
					m_AddedFiles.push_back(str);
				getline(m_EvaluationFile.m_DocFileIn, str, ','); num = stoi(str);
					m_AddedNumRuns.push_back(num);
				getline(m_EvaluationFile.m_DocFileIn, str);	num = stoi(str);
					m_AddedFirstRunNo.push_back(num); 
			}
			getnext_str(); numensembles = stoi(str);
			getnext_str(); m_ActualEnsemble = stoi(str);


			if (good) {
				if (m_AcceptedRuns.size() < numensembles) {
					m_AcceptedIdString.resize(numensembles);
					m_AcceptedRuns.resize(numensembles);
				}
				for (size_t i = 0; i < numensembles; i++) {
					getnext_str(); m_AcceptedIdString[i] = str;
					getnext_str(); num_accepted = stoi(str);

					if (good) {
						m_AcceptedRuns[i].resize(num_accepted);
						if (num_accepted > 0) {
							m_AcceptedRuns[i] = m_EvaluationFile.rsize_tArray(num_accepted);
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

		return m_EvaluationFile.IsFileInGood();
	}
	else {
		//Writing to fil
		str = "Number of Files added to Basic Run : ";
		//m_EvaluationFile.wString(&str);
		unsigned int ifiles = m_AddedFiles.size();

		m_EvaluationFile.m_DocFileOut << str<<ifiles<<endl;
		//m_EvaluationFile.wInt(&ifiles);
		unsigned int num, num_first;
		if(ifiles>0) {
			m_EvaluationFile.m_DocFileOut << "File name : , Added Runs , First Added " << endl;		
		}
		for (unsigned int i = 0; i < ifiles; i++) {
            string addfile = GetNameAddedFiles(i);
			num = GetNumberOfAddedRuns(i);
			num_first = GetFirstRunNoAddedRuns(i);
			m_EvaluationFile.m_DocFileOut << addfile <<',' << num <<','<< num_first << endl;
		}
	
		size_t NumEnsembles = m_AcceptedRuns.size();
		NumEnsembles = max(size_t(1), NumEnsembles);

		m_EvaluationFile.m_DocFileOut << "Number of Ensembles : " << NumEnsembles<<endl;
		m_EvaluationFile.m_DocFileOut << "Actual Ensemble : "<< m_ActualEnsemble<<endl;

		size_t NumCheck = m_AcceptedIdString.size();
		if (NumCheck != NumEnsembles) {
			m_AcceptedIdString.resize(NumEnsembles);
			m_AcceptedRuns.resize(NumEnsembles);
		}
		for (size_t i = 0; i < NumEnsembles; i++) {
			if (m_AcceptedIdString[i].size() == 0) {
				m_AcceptedIdString[i] = "E(" + FUtil::STD_ItoAscii(i + 1) + ")";
			}
			m_EvaluationFile.m_DocFileOut << "Ensemble Id : "<< m_AcceptedIdString[i] <<endl;

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
				m_EvaluationFile.m_DocFileOut << "Accepted Runs : " << num << endl;
				good = m_EvaluationFile.wsize_tArray(&m_AcceptedRuns[i], num);

			}
		}


		return m_EvaluationFile.IsFileOutGood();
	}
	return true;
}
bool MultiStorageStatData::ReadWrite_MultiPar(bool reading) {
	bool good;
	string str;
	auto getnext_str = [&]() {
		getline(m_EvaluationFile.m_DocFileIn, str, ':');
		getline(m_EvaluationFile.m_DocFileIn, str);
		};
	if (reading) {
		if (m_MBinHeader.n_param > 0) {
			if (m_ParMin.size() != m_MBinHeader.n_param) {
				m_ParMin.resize(m_MBinHeader.n_param);
				m_ParMax.resize(m_MBinHeader.n_param);
				m_ParValidMin.resize(m_MBinHeader.n_param);
				m_ParValidMax.resize(m_MBinHeader.n_param);
			}
			getnext_str();
			m_ParMin =m_EvaluationFile.rFloatArray2( m_MBinHeader.n_param);
			getnext_str();
			m_ParMax =m_EvaluationFile.rFloatArray2( m_MBinHeader.n_param);
			getnext_str();
			m_ParValidMin =m_EvaluationFile.rFloatArray2( m_MBinHeader.n_param);
			getnext_str();
			m_ParValidMax =m_EvaluationFile.rFloatArray2(m_MBinHeader.n_param);

		}
		good=m_EvaluationFile.IsFileInGood();
	}
	else {
		// Writing
		if (m_ParMin.size() != m_MBinHeader.n_param) {
			m_ParMin.resize(m_MBinHeader.n_param);
			m_ParMax.resize(m_MBinHeader.n_param);
			m_ParValidMin.resize(m_MBinHeader.n_param);
			m_ParValidMax.resize(m_MBinHeader.n_param);
		}
		m_EvaluationFile.m_DocFileOut << "Parameter Min Values : " << m_MBinHeader.n_param << endl;
		good=m_EvaluationFile.wFloatArray2(m_ParMin,m_MBinHeader.n_param);
		m_EvaluationFile.m_DocFileOut << "Parameter Max Values : " << m_MBinHeader.n_param << endl;
		good=m_EvaluationFile.wFloatArray2(m_ParMax,  m_MBinHeader.n_param);
		m_EvaluationFile.m_DocFileOut << "Valid Min Values : " << m_MBinHeader.n_param << endl;
		good=m_EvaluationFile.wFloatArray2(m_ParValidMin, m_MBinHeader.n_param);
		m_EvaluationFile.m_DocFileOut << "Valid Max Values : " << m_MBinHeader.n_param << endl;
		good=m_EvaluationFile.wFloatArray2(m_ParValidMax, m_MBinHeader.n_param);
	}

	return good;
}
bool MultiStorageStatData::ReadWrite_CoCor(bool reading) {
	bool good;
	if (reading) {
		string str;
		getline(m_EvaluationFile.m_DocFileIn, str, ':');
		getline(m_EvaluationFile.m_DocFileIn, str);
		m_CoCorNumValid = stoi(str);
		m_CoCorValid.resize(m_CoCorNumValid*m_CoCorNumValid);
		auto num = m_CoCorValid.size();
		if(num>0) m_CoCorValid = m_EvaluationFile.rFloatArray2(num, m_CoCorNumValid);
		return m_EvaluationFile.IsFileInGood();
	}
	else {
		// Writing
		m_EvaluationFile.m_DocFileOut << "Parameter CoCorrelation : " << m_CoCorNumValid << endl;
		auto num = m_CoCorValid.size();
		if(num>0) good = m_EvaluationFile.wFloatArray2(m_CoCorValid, num, m_CoCorNumValid);

		return m_EvaluationFile.IsFileOutGood();
	}
	
}
bool MultiStorageStatData::ReadWrite_Criteria(bool reading) {
	bool good=true;
	size_t num;
	size_t num_indicators = 7;
	if (reading) {
		string str;
		auto getnext_str = [&]() {
			getline(m_EvaluationFile.m_DocFileIn, str, ':');
			getline(m_EvaluationFile.m_DocFileIn, str);
		};

		VarValidAllo();
		if (m_NumValidAll != 0) {
			getnext_str(); num = stoi(str);
			for (size_t i = 0; i < num; i++) {
				getnext_str();
				size_t num2;
				getnext_str(); num2 = stoi(str);
				if (num2 > 0) {
					m_Valid_TSV_Mean[i].v[E_SELECT_LOW] = m_EvaluationFile.rFloatArray2(num2);
					getnext_str(); num2 = stoi(str);
					m_Valid_TSV_Mean[i].v[E_SELECT_HIGH] = m_EvaluationFile.rFloatArray2(num2);
					getnext_str(); num2 = stoi(str);
					m_Valid_TSV_Mean[i].v[E_POST_MIN] = m_EvaluationFile.rFloatArray2(num2);
					getnext_str(); num2 = stoi(str);
					m_Valid_TSV_Mean[i].v[E_POST_MAX] = m_EvaluationFile.rFloatArray2(num2);
					getnext_str(); num2 = stoi(str);
					m_Valid_TSV_Mean[i].v[E_POST_MEAN] = m_EvaluationFile.rFloatArray2(num2);
				}
			}
			getnext_str(); num = stoi(str);
			size_t num_all;
			for (size_t i = 0; i < num; i++) {

					getnext_str(); 
					getnext_str(); num_all = stoi(str);	if(num_all== m_NumValidAll)
						m_Valid_TSV[i].v[E_SELECT_LOW] = m_EvaluationFile.rFloatArray2(num_all, m_NumValidAll / num_indicators);
					getnext_str(); num_all = stoi(str);	if (num_all == m_NumValidAll)
						m_Valid_TSV[i].v[E_SELECT_HIGH] = m_EvaluationFile.rFloatArray2(num_all, m_NumValidAll / num_indicators);
					getnext_str(); num_all = stoi(str);	if (num_all == m_NumValidAll)
						m_Valid_TSV[i].v[E_POST_MIN] = m_EvaluationFile.rFloatArray2(num_all, m_NumValidAll / num_indicators);
					getnext_str(); num_all = stoi(str);	if (num_all == m_NumValidAll)
						m_Valid_TSV[i].v[E_POST_MAX] = m_EvaluationFile.rFloatArray2(num_all, m_NumValidAll / num_indicators);
					getnext_str(); num_all = stoi(str);	if (num_all == m_NumValidAll)
						m_Valid_TSV[i].v[E_POST_MEAN] = m_EvaluationFile.rFloatArray2(num_all, m_NumValidAll / num_indicators);
					getnext_str(); num_all = stoi(str);	if (num_all == m_NumValidAll)
						m_Valid_TSV[i].v[E_POST_CV] = m_EvaluationFile.rFloatArray2(num_all, m_NumValidAll / num_indicators);
					getnext_str(); num_all = stoi(str);	if (num_all == m_NumValidAll)
						m_Valid_TSV[i].v[E_PRIOR_MIN] = m_EvaluationFile.rFloatArray2(num_all, m_NumValidAll / num_indicators);
					getnext_str(); num_all = stoi(str);	if (num_all == m_NumValidAll)
						m_Valid_TSV[i].v[E_PRIOR_MAX] = m_EvaluationFile.rFloatArray2(num_all, m_NumValidAll / num_indicators);
					getnext_str(); num_all = stoi(str);	if (num_all == m_NumValidAll)
						m_Valid_TSV[i].v[E_PRIOR_MEAN] = m_EvaluationFile.rFloatArray2(num_all, m_NumValidAll / num_indicators);
					getnext_str(); num_all = stoi(str);	if (num_all == m_NumValidAll)
						m_Valid_TSV[i].v[E_PRIOR_CV] = m_EvaluationFile.rFloatArray2(num_all, m_NumValidAll / num_indicators);

					getnext_str(); m_Valid_TSV[i].tobe_Plotted = stoi(str);
					getnext_str(); m_Valid_TSV[i].is_Updated = stoi(str);
					getnext_str(); m_Valid_TSV[i].VarTranformed = stoi(str);
					getnext_str(); m_Valid_TSV[i].Dim1_Par_Index = stoi(str);

				//History of Criteria Setting
				size_t numhistory;
				getnext_str(); numhistory = stoi(str);
				if (numhistory < 10000) {
					m_Valid_TSV[i].History.resize(numhistory);

					for (size_t jj = 0; jj < numhistory; jj++) {
						int item1;
						int item2;
						getnext_str(); item1 = stoi(str);
						m_Valid_TSV[i].History[jj].criteria = CRITERIA_STATE(item1);
						getnext_str(); item2 = stoi(str);
						m_Valid_TSV[i].History[jj].stat_indicator = PERFORMANCE_VAR(item2);
						getnext_str(); m_Valid_TSV[i].History[jj].n_post = stoi(str);
						getnext_str(); m_Valid_TSV[i].History[jj].VarIndex = stoi(str);
						getnext_str(); m_Valid_TSV[i].History[jj].minvalue = stof(str);
						getnext_str(); m_Valid_TSV[i].History[jj].maxvalue = stof(str);
					}
				}

				m_pNewMap->m_MStorage.IsMinUpdated[i] = true;
				m_pNewMap->m_MStorage.IsMaxUpdated[i] = true;

			}

		}
		if (m_pNewMap->m_ValSum_Array.size() > 0) {
			m_EvaluationFile.rsize_t(&num);
			if (num != m_Valid_SV.size()) m_Valid_SV.resize(num);
			for (size_t i = 0; i < m_Valid_SV.size(); i++) {
				m_EvaluationFile.rsize_t(&num);
				if (num == 0) {
					m_Valid_SV.resize(num);
					continue;
				}
				AllocateValid_SUM(i);
				auto values = m_EvaluationFile.rsize_tArray(num);
				m_Valid_TSV[i].criteria.resize(values.size());
				for (size_t j = 0; j < values.size(); j++) m_Valid_SV[i].criteria[j] = CRITERIA_STATE(values[j]);

				m_Valid_SV[i].v[E_SELECT_LOW] = m_EvaluationFile.rFloatArray2(num);
				m_Valid_SV[i].v[E_SELECT_HIGH] = m_EvaluationFile.rFloatArray2(num);
				m_Valid_SV[i].v[E_POST_MIN] = m_EvaluationFile.rFloatArray2(num);
				m_Valid_SV[i].v[E_POST_MAX] = m_EvaluationFile.rFloatArray2(num);
				m_Valid_SV[i].v[E_POST_MEAN] = m_EvaluationFile.rFloatArray2(num);
				m_Valid_SV[i].v[E_POST_CV] = m_EvaluationFile.rFloatArray2(num);
				m_EvaluationFile.rbool(&m_Valid_SV[i].tobe_Plotted);
				m_EvaluationFile.rbool(&m_Valid_SV[i].is_Updated);
				m_EvaluationFile.rsize_t(&m_Valid_SV[i].VarTranformed);
				m_EvaluationFile.rsize_t(&num);
				m_pNewMap->m_MStorage.m_SV_StatMemory.AcceptedRuns = true;

				if (m_Valid_SV[i].criteria.size() > 7 && num == 7) {
					m_Valid_SV_Mean[i].criteria.resize(num);

					auto values = m_EvaluationFile.rsize_tArray(m_NumValidAll);
					m_Valid_SV_Mean[i].criteria.resize(values.size());
					for (size_t j = 0; j < values.size(); j++) m_Valid_SV_Mean[i].criteria[j] = CRITERIA_STATE(values[j]);
					m_Valid_SV_Mean[i].v[E_SELECT_LOW] = m_EvaluationFile.rFloatArray2(num);
					m_Valid_SV_Mean[i].v[E_SELECT_HIGH] = m_EvaluationFile.rFloatArray2(num);
					m_Valid_SV_Mean[i].v[E_POST_MIN] = m_EvaluationFile.rFloatArray2(num);
					m_Valid_SV_Mean[i].v[E_POST_MAX] = m_EvaluationFile.rFloatArray2(num);
					m_Valid_SV_Mean[i].v[E_POST_MEAN] = m_EvaluationFile.rFloatArray2(num);
					m_Valid_SV_Mean[i].v[E_POST_CV] = m_EvaluationFile.rFloatArray2(num);
				}
			}

			// Prior statistics for all runs similar for all ensembles
			if (m_Valid_SV.size() > 0) {
				size_t SV_VarNum = m_Valid_SV[0].criteria.size();
				m_EvaluationFile.rsize_t(&num);
				if (m_EvaluationFile.IsFileInGood() && SV_VarNum == num) {
					m_Valid_SV[0].v[E_PRIOR_MIN] = m_EvaluationFile.rFloatArray2(num);
					m_Valid_SV[0].v[E_PRIOR_MAX] = m_EvaluationFile.rFloatArray2(num);
					m_Valid_SV[0].v[E_PRIOR_MEAN] = m_EvaluationFile.rFloatArray2(num);
					m_Valid_SV[0].v[E_PRIOR_CV] = m_EvaluationFile.rFloatArray2(num);
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
				m_EvaluationFile.rsize_t(&num);
				if (num == 7) {
					m_Valid_SV_Mean[0].v[E_PRIOR_MIN] = m_EvaluationFile.rFloatArray2(num);
					m_Valid_SV_Mean[0].v[E_PRIOR_MAX] = m_EvaluationFile.rFloatArray2(num);
					m_Valid_SV_Mean[0].v[E_PRIOR_MEAN] = m_EvaluationFile.rFloatArray2(num);
					m_Valid_SV_Mean[0].v[E_PRIOR_CV] = m_EvaluationFile.rFloatArray2(num);



					for (size_t i = 1; i < m_Valid_SV.size(); i++) {
						m_Valid_SV_Mean[i].v[E_PRIOR_MIN] = m_Valid_SV_Mean[0].v[E_PRIOR_MIN];
						m_Valid_SV_Mean[i].v[E_PRIOR_MAX] = m_Valid_SV_Mean[0].v[E_PRIOR_MAX];
						m_Valid_SV_Mean[i].v[E_PRIOR_MEAN] = m_Valid_SV_Mean[0].v[E_PRIOR_MEAN];
						m_Valid_SV_Mean[i].v[E_PRIOR_CV] = m_Valid_SV_Mean[0].v[E_PRIOR_CV];
					}
					m_pNewMap->m_MStorage.m_SV_StatMemory.AllRuns = true;
				}
			}
		}

		return m_EvaluationFile.IsFileInGood();
	}
	else {

		unsigned int num;
		if (m_NumValidAll != 0) {
			num = m_Valid_TSV_Mean.size();
			m_EvaluationFile.m_DocFileOut << "Number of Ensembles : " <<  num<< endl;
			for (size_t i = 0; i < num; i++) {
				if (good && m_NumValidAll > 0) {
					m_EvaluationFile.m_DocFileOut << "Ensemble No : " << i+1 << endl;
					auto num2=m_Valid_TSV_Mean[i].v[E_SELECT_LOW].size();
					m_EvaluationFile.m_DocFileOut << "Low Mean Criteria : " << num2 << endl;
					m_EvaluationFile.wFloatArray2(m_Valid_TSV_Mean[i].v[E_SELECT_LOW], num_indicators);
					m_EvaluationFile.m_DocFileOut << "High Mean Criteria : " << num2 << endl;
					m_EvaluationFile.wFloatArray2(m_Valid_TSV_Mean[i].v[E_SELECT_HIGH], num_indicators);
					m_EvaluationFile.m_DocFileOut << "Posterior Min of Mean Value : " << num2 << endl;
					m_EvaluationFile.wFloatArray2(m_Valid_TSV_Mean[i].v[E_POST_MIN], num_indicators);
					m_EvaluationFile.m_DocFileOut << "Posterior Max of Mean Value : " << num2 << endl;
					m_EvaluationFile.wFloatArray2(m_Valid_TSV_Mean[i].v[E_POST_MAX], num_indicators);
					m_EvaluationFile.m_DocFileOut << "Posterior mean of Mean Value : " << num2 << endl;
					m_EvaluationFile.wFloatArray2(m_Valid_TSV_Mean[i].v[E_POST_MEAN], num_indicators);
				}

			}

			num = m_Valid_TSV.size();
			// Posterior statistics for all variables and all accepted Ensembles
			m_EvaluationFile.m_DocFileOut << "Number of Ensembles : " << num << endl;

			for (size_t i = 0; i < num; i++) {
				size_t koll = m_NumValidAll;
				good = m_EvaluationFile.wSize_t(&koll);
				if (good&&m_NumValidAll > 0) {
					m_EvaluationFile.m_DocFileOut << "Ensemble No : " << i + 1 << endl;
					m_EvaluationFile.m_DocFileOut << "Low Criteria : " << m_NumValidAll << endl;
					m_EvaluationFile.wFloatArray2(m_Valid_TSV[i].v[E_SELECT_LOW], m_NumValidAll, m_NumValidAll/num_indicators);
					m_EvaluationFile.m_DocFileOut << "High Criteria : " << m_NumValidAll << endl;
					m_EvaluationFile.wFloatArray2(m_Valid_TSV[i].v[E_SELECT_HIGH], m_NumValidAll, m_NumValidAll / num_indicators);
					m_EvaluationFile.m_DocFileOut << "Posterior Min of Value : " << m_NumValidAll << endl;
					m_EvaluationFile.wFloatArray2(m_Valid_TSV[i].v[E_POST_MIN], m_NumValidAll, m_NumValidAll / num_indicators);
					m_EvaluationFile.m_DocFileOut << "Posterior Max of Value : " << m_NumValidAll << endl;
					m_EvaluationFile.wFloatArray2(m_Valid_TSV[i].v[E_POST_MAX], m_NumValidAll, m_NumValidAll / num_indicators);
					m_EvaluationFile.m_DocFileOut << "Posterior Mean of Value : " << m_NumValidAll << endl;
					m_EvaluationFile.wFloatArray2(m_Valid_TSV[i].v[E_POST_MEAN], m_NumValidAll, m_NumValidAll / num_indicators);
					m_EvaluationFile.m_DocFileOut << "Posterior CV of Value : " << m_NumValidAll << endl;
					m_EvaluationFile.wFloatArray2(m_Valid_TSV[i].v[E_POST_CV], m_NumValidAll, m_NumValidAll / num_indicators);
					m_EvaluationFile.m_DocFileOut << "Prior Min of Value : " << m_NumValidAll << endl;
					m_EvaluationFile.wFloatArray2(m_Valid_TSV[i].v[E_PRIOR_MIN], m_NumValidAll, m_NumValidAll / num_indicators);
					m_EvaluationFile.m_DocFileOut << "Prior Max of Value : " << m_NumValidAll << endl;
					m_EvaluationFile.wFloatArray2(m_Valid_TSV[i].v[E_PRIOR_MAX], m_NumValidAll, m_NumValidAll / num_indicators);
					m_EvaluationFile.m_DocFileOut << "Prior Mean of Value : " << m_NumValidAll << endl;
					m_EvaluationFile.wFloatArray2(m_Valid_TSV[i].v[E_PRIOR_MEAN], m_NumValidAll, m_NumValidAll / num_indicators);
					m_EvaluationFile.m_DocFileOut << "Prior CV Value : " << m_NumValidAll << endl;
					m_EvaluationFile.wFloatArray2(m_Valid_TSV[i].v[E_PRIOR_CV], m_NumValidAll, m_NumValidAll / num_indicators);
				}
				m_EvaluationFile.m_DocFileOut << "Enable for plotting : " << m_Valid_TSV[i].tobe_Plotted << endl;
				m_EvaluationFile.m_DocFileOut << "Is Updated : " << m_Valid_TSV[i].is_Updated << endl;
				m_EvaluationFile.m_DocFileOut << "Transformed Variables Indicator : " << m_Valid_TSV[i].VarTranformed << endl;
				m_EvaluationFile.m_DocFileOut << "Dimension 1 Index : " << m_Valid_TSV[i].Dim1_Par_Index << endl;
			/*	m_EvaluationFile.wbool(&m_Valid_TSV[i].tobe_Plotted);
				m_EvaluationFile.wbool(&m_Valid_TSV[i].is_Updated);
				m_EvaluationFile.wSize_t(&m_Valid_TSV[i].VarTranformed);
				m_EvaluationFile.wSize_t(&m_Valid_TSV[i].Dim1_Par_Index);*/

				//History of Criteria Setting
				size_t numhistory = m_Valid_TSV[i].History.size();
				m_EvaluationFile.m_DocFileOut << "Number of History : " << numhistory << endl;
				for (size_t jj = 0; jj < numhistory; jj++) {
					m_EvaluationFile.m_DocFileOut << "Criteria : " << m_Valid_TSV[i].History[jj].criteria << endl;
					m_EvaluationFile.m_DocFileOut << "Statistical Performance Indicator : " << m_Valid_TSV[i].History[jj].stat_indicator << endl;
					m_EvaluationFile.m_DocFileOut << "Remaining runs : " << m_Valid_TSV[i].History[jj].n_post << endl;
					m_EvaluationFile.m_DocFileOut << "Var Index : " << m_Valid_TSV[i].History[jj].VarIndex << endl;
					m_EvaluationFile.m_DocFileOut << "Min value : " << m_Valid_TSV[i].History[jj].minvalue << endl;
					m_EvaluationFile.m_DocFileOut << "Max value : " << m_Valid_TSV[i].History[jj].maxvalue << endl;
				}
				

			}
		}
		num = m_Valid_SV.size();
		if (m_pNewMap->m_ValSum_Array.size() > 0) {
			good = m_EvaluationFile.wInt(&num);
			for (size_t i = 0; i < num; i++) {
				size_t SV_VarNum = m_Valid_SV[i].criteria.size();
				m_EvaluationFile.wSize_t(&SV_VarNum);
				size_t numval = 7;
				if (m_EvaluationFile.IsFileOutGood() && SV_VarNum > 0) {

					vector<size_t> values;
					values.resize(m_Valid_SV[i].criteria.size());
					for (size_t j = 0; j < values.size(); j++) values[j] = size_t(m_Valid_SV[i].criteria[j]);
					m_EvaluationFile.wsize_tArray(&values, m_NumValidAll);
					//m_EvaluationFile.wsize_tArray(&m_Valid_SV[i].criteria, SV_VarNum);
					m_EvaluationFile.wFloatArray2(m_Valid_SV[i].v[E_SELECT_LOW], SV_VarNum);
					m_EvaluationFile.wFloatArray2(m_Valid_SV[i].v[E_SELECT_HIGH], SV_VarNum);
					m_EvaluationFile.wFloatArray2(m_Valid_SV[i].v[E_POST_MIN], SV_VarNum);
					m_EvaluationFile.wFloatArray2(m_Valid_SV[i].v[E_POST_MAX], SV_VarNum);
					m_EvaluationFile.wFloatArray2(m_Valid_SV[i].v[E_POST_MEAN], SV_VarNum);
					m_EvaluationFile.wFloatArray2(m_Valid_SV[i].v[E_POST_CV], SV_VarNum);
					m_EvaluationFile.wbool(&m_Valid_SV[i].tobe_Plotted);
					m_EvaluationFile.wbool(&m_Valid_SV[i].is_Updated);
					m_EvaluationFile.wSize_t(&m_Valid_SV[i].VarTranformed);
					m_EvaluationFile.wSize_t(&numval);
				}
				if (SV_VarNum > 7) {
					if (m_EvaluationFile.IsFileOutGood()) {

						vector<size_t> values;
						values.resize(m_Valid_SV_Mean[i].criteria.size());
						for (size_t j = 0; j < values.size(); j++) values[j] = size_t(m_Valid_SV_Mean[i].criteria[j]);
						m_EvaluationFile.wsize_tArray(&values, m_NumValidAll);

						//m_EvaluationFile.wsize_tArray(&m_Valid_SV_Mean[i].criteria, numval);
						m_EvaluationFile.wFloatArray2(m_Valid_SV_Mean[i].v[E_SELECT_LOW], numval);
						m_EvaluationFile.wFloatArray2(m_Valid_SV_Mean[i].v[E_SELECT_HIGH], numval);
						m_EvaluationFile.wFloatArray2(m_Valid_SV_Mean[i].v[E_POST_MIN], numval);
						m_EvaluationFile.wFloatArray2(m_Valid_SV_Mean[i].v[E_POST_MAX], numval);
						m_EvaluationFile.wFloatArray2(m_Valid_SV_Mean[i].v[E_POST_MEAN], numval);
						m_EvaluationFile.wFloatArray2(m_Valid_SV_Mean[i].v[E_POST_CV], numval);
					}
				}
			}

			// Prior statistics for all runs similar for all ensembles
			size_t SV_VarNum = m_Valid_SV[0].criteria.size();
			m_EvaluationFile.wSize_t(&SV_VarNum);
			if (m_EvaluationFile.IsFileOutGood() && SV_VarNum > 0) {
				m_EvaluationFile.wFloatArray2(m_Valid_SV[0].v[E_PRIOR_MIN], SV_VarNum);
				m_EvaluationFile.wFloatArray2(m_Valid_SV[0].v[E_PRIOR_MAX], SV_VarNum);
				m_EvaluationFile.wFloatArray2(m_Valid_SV[0].v[E_PRIOR_MEAN], SV_VarNum);
				m_EvaluationFile.wFloatArray2(m_Valid_SV[0].v[E_PRIOR_CV], SV_VarNum);
			}
			size_t  ivar = m_pNewMap->m_ValSum_Array.size();
			if (ivar > 1) {
				size_t numval = 7;
				m_EvaluationFile.wSize_t(&numval);
				if (m_EvaluationFile.IsFileOutGood()) {
					m_EvaluationFile.wFloatArray2(m_Valid_SV_Mean[0].v[E_PRIOR_MIN], numval);
					m_EvaluationFile.wFloatArray2(m_Valid_SV_Mean[0].v[E_PRIOR_MAX], numval);
					m_EvaluationFile.wFloatArray2(m_Valid_SV_Mean[0].v[E_PRIOR_MEAN], numval);
					m_EvaluationFile.wFloatArray2(m_Valid_SV_Mean[0].v[E_PRIOR_CV], numval);
				}
			}
		}

		// Writing

		return m_EvaluationFile.IsFileOutGood();


	}
}
bool MultiStorageStatData::ReadWrite_OptionalTSV_Valid(bool reading) {
	if(m_NumValidAll>0)
		
		if (reading) {
			string str;
			auto getnext_str = [&]() {
				getline(m_EvaluationFile.m_DocFileIn, str, ':');
				getline(m_EvaluationFile.m_DocFileIn, str);
			};



			getnext_str(); m_CumulatedPerformance = stoi(str);
			//m_EvaluationFile.rsize_t(&m_CumulatedPerformance);
			size_t num;
			//m_VarValidTransformed.resize(0);
			if (m_CumulatedPerformance > 100) return false;

			getnext_str(); num = stoi(str);
			//if(!m_EvaluationFile.rsize_t(&num)) return false;
			VARTRANSFORMED add;
			for(size_t i = 0; i < num; i++) {
				getnext_str(); auto ikoll= stoi(str);
				getnext_str(); add.Transformed_index = stoi(str);
				//if(!m_EvaluationFile.rsize_t(&add.Transformed_index)) return false;
				size_t numvector;
				//if (m_New7_Stat&&!m_EvaluationFile.rsize_t(&add.Dim1_index)) return false;
				getnext_str(); add.Dim1_index= stoi(str);
				if (!m_EvaluationFile.IsFileInGood()) return false;

				getnext_str(); numvector = stoi(str);
				//if(!m_EvaluationFile.rsize_t(&numvector)) return false;
				if (numvector == string::npos||numvector>5000000) return false;
				add.VarValidVector.resize(numvector);

				add.VarValidVector=m_EvaluationFile.rFloatArray2(numvector, m_NumValidAll);
				m_VarValidTransformed.push_back(add);
				MapOfStoredTSV.insert(pair<size_t,size_t>(add.Transformed_index, m_VarValidTransformed.size() - 1));
			}
			return m_EvaluationFile.IsFileInGood();
		}
		else {
			m_EvaluationFile.m_DocFileOut << "Cumulated Performance : " <<m_CumulatedPerformance << endl;
			//m_EvaluationFile.wSize_t(&m_CumulatedPerformance);
			size_t num;
			num = m_VarValidTransformed.size();
			m_EvaluationFile.m_DocFileOut << "Number of Transformed Ensembles : " << num << endl;
			//m_EvaluationFile.wSize_t(&num);
			if (num == 0 && m_CumulatedPerformance == 0&& m_VarValid.size()>0) {
				//Original Performance indicators to archive
				VARTRANSFORMED vartrans;
				vartrans.Transformed_index = 0;
				vartrans.VarValidVector = m_VarValid;
				m_VarValidTransformed.push_back(vartrans);
				MapOfStoredTSV.insert(pair<size_t, size_t>(vartrans.Transformed_index, 0));
			}
//         Check if VarValidVector has same size as m_VarValid

			for (size_t i = 0; i < m_VarValidTransformed.size(); i++) {
				m_EvaluationFile.m_DocFileOut << "Transformed Ensemble : " << i+1 << endl;
				m_EvaluationFile.m_DocFileOut << "Transform Index : " << m_VarValidTransformed[i].Transformed_index << endl;
				m_EvaluationFile.m_DocFileOut << "Dim1 Index : " << m_VarValidTransformed[i].Dim1_index << endl;
				num = m_VarValidTransformed[i].VarValidVector.size();
				m_EvaluationFile.m_DocFileOut << "VarValid Entire Vector : " << num<< endl;
				
				m_EvaluationFile.wFloatArray2(m_VarValidTransformed[i].VarValidVector, num, m_NumValidAll);
			}

			return m_EvaluationFile.IsFileOutGood();
		}
	return m_EvaluationFile.IsFileInGood();

}
bool MultiStorageStatData::ReadWrite_OptionalSUM_Valid(bool reading) {

	bool good;
	size_t totnumruns;
	if (reading) {
		good = m_EvaluationFile.rsize_t(&totnumruns);
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

				valsum.ME = m_EvaluationFile.rFloatArray2(totnumruns);
				valsum.R2_Multi = m_EvaluationFile.rFloatArray2(totnumruns);
				valsum.InterCept_Multi = m_EvaluationFile.rFloatArray2(totnumruns);
				valsum.Slope_Multi = m_EvaluationFile.rFloatArray2(totnumruns);
				valsum.RMSE = m_EvaluationFile.rFloatArray2(totnumruns);
				valsum.NSE = m_EvaluationFile.rFloatArray2(totnumruns);
				valsum.LogLi = m_EvaluationFile.rFloatArray2(totnumruns);
				valsum.SimValueVector = m_EvaluationFile.rFloatArray2(totnumruns);
				valsum.ObsValueVector = m_EvaluationFile.rFloatArray2(totnumruns);
				m_EvaluationFile.rInt(&valsum.n_RegMulti);
				m_pNewMap->SetValSum(i, valsum);
			}
			m_pNewMap->CalculateMeanPerformanceAndSetValues();
			m_pNewMap->m_IsPerformanceSmartFlexibleFile_Read = true;

			if (m_pNewMap->m_ValSum_Array.size() > 1) {

				for (size_t ivar = 0; ivar < 7; ivar++) {
					if (m_Valid_SV_Mean[0].MeanPerFomanceIndicators[ivar].size() != totnumruns) m_Valid_SV_Mean[0].MeanPerFomanceIndicators[ivar].resize(totnumruns);
					m_Valid_SV_Mean[0].MeanPerFomanceIndicators[ivar] = m_EvaluationFile.rFloatArray2(totnumruns);
				}
				for (size_t i = 1; i < m_Valid_SV_Mean.size(); i++) {
					for (size_t ivar = 0; ivar < 7; ivar++) {
						if (m_Valid_SV_Mean[i].MeanPerFomanceIndicators[ivar].size() != totnumruns) m_Valid_SV_Mean[i].MeanPerFomanceIndicators[ivar].resize(totnumruns);
						m_Valid_SV_Mean[i].MeanPerFomanceIndicators[ivar] = m_Valid_SV_Mean[0].MeanPerFomanceIndicators[ivar];
					}
				}

				m_pNewMap->m_Are_MeanValues_SV_VariablesEstimated = true;
			}


		return true;// m_EvaluationFile.IsFileInGood();
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
			m_EvaluationFile.wSize_t(&totnumruns);
			for (size_t i = 0; i < m_pNewMap->m_ValSum_Array.size(); i++) {
				auto valsum = m_pNewMap->GetValSumStruct(i);
				m_EvaluationFile.wFloatArray2(valsum.ME, totnumruns);
				m_EvaluationFile.wFloatArray2(valsum.R2_Multi, totnumruns);
				m_EvaluationFile.wFloatArray2(valsum.InterCept_Multi, totnumruns);
				m_EvaluationFile.wFloatArray2(valsum.Slope_Multi, totnumruns);
				m_EvaluationFile.wFloatArray2(valsum.RMSE, totnumruns);
				m_EvaluationFile.wFloatArray2(valsum.NSE, totnumruns);
				m_EvaluationFile.wFloatArray2(valsum.LogLi, totnumruns);
				m_EvaluationFile.wFloatArray2(valsum.SimValueVector, totnumruns);
				m_EvaluationFile.wFloatArray2(valsum.ObsValueVector, totnumruns);

				m_EvaluationFile.wInt(&valsum.n_RegMulti);
			}
			if (m_pNewMap->m_ValSum_Array.size() > 1) {
				for (size_t ivar = 0; ivar < 7; ivar++)
					m_EvaluationFile.wFloatArray2(m_Valid_SV_Mean[0].MeanPerFomanceIndicators[ivar], totnumruns);
				
			}

		}


		return m_EvaluationFile.IsFileOutGood();
	}
}
bool MultiStorageStatData::ReadWrite_Parameters(bool reading) {
	if (reading) {


	}
	else {


		return m_EvaluationFile.IsFileOutGood();
	}
	return m_EvaluationFile.IsFileInGood();

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

