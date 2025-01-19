// MultiStorageData.cpp : implementation file
//

#include "../stdafx.h"
#include "../std.h"
#include "../Util.h"
#include "../NewBase/NewMap.h"
#include "MultiStorageData.h"

#ifndef COUPSTD
#include "../NewBase/FileScanner.h"

#endif


#include <limits>

// MultiStorageData

MultiStorageData::MultiStorageData()
{
	m_IsOpen=false;
	m_IsReady=false;
	m_IsMissing=false;
	m_TotalNumAccepted=0;
	m_TotalNumBayesAccepted=0;
	m_CyclicNumber = 1;
	m_CyclicStartPos=0;

	m_OrigRuns=0;
	m_OrigFirstRunNo=0;
	m_Lock=true;
	m_NumExtraValid=0;
	m_NumSplitPeriod=0;
	m_IsSplitOpen=false;
	m_VarSplit.clear();
	m_AcceptedUpdated=false;
	m_pNewMap = nullptr;
	m_CumulatedPerformance=0;
	m_AcceptedRuns.resize(0);
	
	m_MeanSV_STAT.resize(NUM_STAT); // First Dim STAT of Performance
	m_MeanTSV_STAT.resize(NUM_STAT);
	m_VarTSV_Mean.resize(NUM_STAT);
	m_VarSV_Mean.resize(NUM_STAT);
	for (size_t i = 0; i < m_MeanSV_STAT.size(); i++) {
		m_MeanSV_STAT[i].resize(NUM_STAT); // Second Dim type of Performance
		m_MeanTSV_STAT[i].resize(NUM_STAT);
	
		m_VarTSV_Mean[i].resize(NUM_STAT);
		m_VarSV_Mean[i].resize(NUM_STAT);
	}


	m_MeanTSV_All.resize(NUM_STAT);
	m_MeanTSV_Accepted.resize(NUM_STAT);
	m_MeanSV_All.resize(NUM_STAT);
	


	
/*	m_MeanTSV_STAT[SELECT_LOW].resize(7); m_ValidHigh.resize(7);
	m_ValidMin.resize(7); m_ValidMax.resize(7);  m_ValidMinAll.resize(7); m_ValidMaxAll.resize(7);

	m_ValidSumLow.resize(3); m_ValidSumHigh.resize(3);		// Represent mean of SV variables
	m_ValidSumMin.resize(3); m_ValidSumMax.resize(3);;			// index 0-2, reprsent the various performance indicators	
	m_ValidSumMinAll.resize(3); m_ValidSumMaxAll.resize(3);;*/
	m_CoCorNumValid = 0;
	VALIDENSEMBLE base;
	m_Valid_TSV.clear();
	m_Valid_TSV.push_back(base);

	//m_Valid_TSV.resize(1);
	m_Valid_SV.resize(1);
	m_Valid_SV_Mean.resize(1);
	m_TotalNumberRuns = string::npos;
}

MultiStorageData::~MultiStorageData()
{


	if(m_IsOpen) m_MultiFile.close();
	if(m_IsSplitOpen) m_SplitPeriodFile.close();
}
void MultiStorageData::UpdateCyclicNumbers(size_t ActualEnsemble, size_t Dim1_Value, size_t Transform_index)
{
		if(Dim1_Value!=string::npos)
			m_Valid_TSV[ActualEnsemble].Dim1_Par_Index = Dim1_Value;
		else
			Dim1_Value = m_Valid_TSV[ActualEnsemble].Dim1_Par_Index;
		if (Transform_index != string::npos)
			m_Valid_TSV[ActualEnsemble].VarTranformed = Transform_index;
		else
			Transform_index= m_Valid_TSV[ActualEnsemble].VarTranformed;

		if (Dim1_Value == 0) {
			if(Transform_index!=4)
				SetCyclicNumbers(1, 0);
			else {
				auto koll = m_pNewMap->m_MultiRun_Array[0].NumberOfRepeatitionsWithinDimension;
				SetCyclicNumbers(koll, koll-1, Dim1_Value);
			}
		}
		else {
			auto koll = m_pNewMap->m_MultiRun_Array[0].NumberOfRepeatitionsWithinDimension;
			SetCyclicNumbers(koll, Dim1_Value - 1, Dim1_Value);
		}
}
bool MultiStorageData::Reset()
{

	m_TotalNumberRuns=string::npos;
	return true;
}




string MultiStorageData::GetSplitFileName()
{
string str;
unsigned long ipos=m_FileName.find(".MBin");
	if(ipos>0) {
		str=m_FileName.substr(0,ipos);
		str.append(".ZBin");

		return str;
	}
	return "";
}
string MultiStorageData::GetStatFileName(bool NewFile)
{
	string str;
	size_t ipos = m_FileName.find(".MBin");
	if (ipos!=string::npos) {
		if (NewFile) {
			MFC_Util m;
			auto vec_names=m.GetSBInFileNames();
		}
		else {
			str = m_FileName.substr(0, ipos);
			str.append(".SBin");
		}
		return str;
	}
	return "";
}
unsigned long MultiStorageData::GetNumSplitPeriods()
{
	return m_NumSplitPeriod;
}
void MultiStorageData::ReOpenSplitFile() {
	if(!m_IsSplitOpen||!m_SplitPeriodFile.good()) {
		string fil=GetSplitFileName();
		if(FUtil::IsFileExisting(fil)) {
			if(!m_SplitPeriodFile.good()) {
				m_SplitPeriodFile.close();
				m_SplitPeriodFile.clear();

			}
			m_SplitPeriodFile.open(fil, ios::binary|ios::in|ios::out);
		}
		else {
			m_SplitPeriodFile.open(fil, ios::binary|ios::out);
			m_SplitPeriodFile.write((char*)&m_NumSplitPeriod,4);
			m_SplitPeriodFile.close();
			m_SplitPeriodFile.open(fil, ios::binary);
			if(m_SplitPeriodFile.fail()) {
				m_SplitPeriodFile.close();
				m_SplitPeriodFile.open(fil, ios::binary|ios::out);
				m_SplitPeriodFile.close();

				m_SplitPeriodFile.open(fil, ios::binary|ios::out|ios::in);

			}


		}

		if(m_SplitPeriodFile.is_open())
			m_IsSplitOpen=true;
		else {
			m_IsSplitOpen=false;
		}
	}



}
void MultiStorageData::AlloSplitPerformance(unsigned long splitperiods)
{
	m_NumSplitPeriod=splitperiods;

	unsigned long ival=m_NumValid/m_ValidStatBase;
	if(ival==0) return;  // No Validation Variables in MultiStorageData File
	m_SplitRec=(m_NumValid+2*m_NumValid/m_ValidStatBase)/ival;
	if (m_pNewMap->m_MultiFileSplitMode)
		m_SplitRecRuns = m_TotalNumberRuns * m_SplitRec;
	else
		m_SplitRecRuns = m_TotalNumberRuns * m_SplitRec * m_NumSplitPeriod;

	m_VarSplit.resize((unsigned long)(m_SplitRecRuns));
	//*unsigned long(splitperiods)*m_TotalNumberRuns;
	//m_VarValidSplit.resize(m_SplitRec/4];
	if(!m_IsSplitOpen||!m_SplitPeriodFile.good()) {
		string fil=GetSplitFileName();
		if(FUtil::IsFileExisting(fil)) {
			if(!m_SplitPeriodFile.good()) {
				m_SplitPeriodFile.close();
				m_SplitPeriodFile.clear();

			}
			m_SplitPeriodFile.open(fil, ios::binary|ios::in|ios::out);
		}
		else {
			m_SplitPeriodFile.open(fil, ios::binary|ios::out);
			m_SplitPeriodFile.write((char*)&m_NumSplitPeriod,4);
			m_SplitPeriodFile.close();
			m_SplitPeriodFile.open(fil, ios::binary);
			if(m_SplitPeriodFile.fail()) {
				m_SplitPeriodFile.close();
				m_SplitPeriodFile.open(fil, ios::binary|ios::out);
				m_SplitPeriodFile.close();

				m_SplitPeriodFile.open(fil, ios::binary|ios::out|ios::in);

			}


		}

		if(m_SplitPeriodFile.is_open())
			m_IsSplitOpen=true;
		else {
			m_IsSplitOpen=false;
		}
	}
	if(m_IsSplitOpen) {
		bool str=m_SplitPeriodFile.good();
		m_SplitPeriodFile.seekp(0,ios::beg);
		m_SplitPeriodFile.write((char*)&m_NumSplitPeriod,4);
		str=m_SplitPeriodFile.good();
		if(splitperiods==0) {
			string fil=GetSplitFileName();
			m_SplitPeriodFile.close();
			remove(fil.data());
			m_IsSplitOpen=false;

		}

	}

}
void MultiStorageData::AlloMinMaxUpdated(size_t NumOfEnsembles) 
{
	m_pNewMap->m_MStorage.IsMinUpdated.resize(NumOfEnsembles);
	m_pNewMap->m_MStorage.IsMaxUpdated.resize(NumOfEnsembles);
	m_pNewMap->m_MStorage.IsCVUpdated.resize(NumOfEnsembles); m_pNewMap->m_MStorage.IsMeanUpdated.resize(NumOfEnsembles);
	m_pNewMap->m_MStorage.IsCVSumUpdated.resize(NumOfEnsembles); m_pNewMap->m_MStorage.IsMeanSumUpdated.resize(NumOfEnsembles); m_pNewMap->m_MStorage.IsMeanSumUpdatedAll.resize(NumOfEnsembles);
	m_pNewMap->m_MStorage.IsCVSumUpdatedAll.resize(NumOfEnsembles); m_pNewMap->m_MStorage.IsMinSumUpdatedAll.resize(NumOfEnsembles); m_pNewMap->m_MStorage.IsMaxSumUpdatedAll.resize(NumOfEnsembles);



}
void MultiStorageData::SetPerformance(unsigned long irun,  unsigned long ind_split, STAT_PERF split)
{
	unsigned long ival=m_NumValid/m_ValidStatBase;
	unsigned long j=(unsigned long)(m_SplitRec)*ind_split+(unsigned long)(m_SplitRec)*m_NumSplitPeriod*irun;

	m_VarSplit[j]=split.r2;//VALIDATION_mp_REG_R2;
	m_VarSplit[j+1]=split.intercept;// VALIDATION_mp_REG_A0;
	m_VarSplit[j+2]=split.slope; //VALIDATION_mp_REG_A1;
	m_VarSplit[j+3]=split.me;// VALIDATION_mp_R_MEAN;
	m_VarSplit[j+4]=split.rmse;// VALIDATION_mp_R_RMS;
	m_VarSplit[j+5]=split.logli;//VALIDATION_mp_SUM_LOGLI;
	m_VarSplit[j+6]=split.nse; // VALIDATION_mp_NSE;
	m_VarSplit[j+7]=float(split.n); // VALIDATION_mp_N_VALUE;
	m_VarSplit[j+8]=split.meanobs;// VALIDATION_mp_MEAN_OBS;*/
	if(split.r2>0.99999) ival=1;

}
void MultiStorageData::SetNewPerformance(unsigned long irun, unsigned long ivar,  STAT_PERF newperf)
{
	//m_VarValid[irun*m_NumValid]=
	size_t ival = m_NumValid / m_ValidStatBase;
	unsigned long index=ivar%ival;

	AddValid(irun,index,newperf.r2);
	AddValid(irun,1*ival+index,newperf.intercept);
	AddValid(irun,2*ival+index,newperf.slope);
	AddValid(irun,3*ival+index,newperf.me);
	AddValid(irun,4*ival+index,newperf.rmse);
	AddValid(irun,5*ival+index,newperf.logli);
	AddValid(irun,6*ival+index, newperf.nse);

}
bool MultiStorageData::SaveSplitData(unsigned long index)
{
	std::streampos pos, pos2;
	pos=m_SplitRecRuns*index*4+4;
	bool str=m_SplitPeriodFile.good();
	m_SplitPeriodFile.seekp(pos, ios::beg);
	
	str=m_SplitPeriodFile.good();
	m_SplitPeriodFile.write((char*)&m_VarSplit[0], m_SplitRecRuns*4);
	str=m_SplitPeriodFile.good();
	//m_SplitPeriodFile.write((char*)*&m_VarValidSplit, m_SplitRec);

	return str;

}

bool MultiStorageData::OpenFile(string filename, bool SetFileName)
{

	if (m_IsOpen || m_IsMissing) return false;

	if (filename.size() == 0) {
		m_IsReady = false;
		if (m_pNewMap != nullptr)
			filename = m_pNewMap->m_DocFile2.m_Multi_MBin_File;
		else
			filename = m_FileName;
	}
	if (m_IsReady) return true;
	m_MultiFile.open(filename, ios::binary | ios::in);
	if (!m_MultiFile.is_open())
	{
		filename = FUtil::check_open_newfile(&m_MultiFile, filename);
		m_pNewMap->m_DocFile2.m_Multi_MBin_File = filename;
	}
	if (filename == "") return false;
	m_IsOpen = true;
	if (SetFileName)
		m_FileName = filename;

	bool check;
	check = true;

	auto koll=m_pNewMap->m_ResidualFileVector.size();
	if (FUtil::check_if_empty_file(&m_MultiFile, filename)) return false;

	m_MBinHeader = ReadStartData();
	m_NumValid = m_MBinHeader.n_valid;
	m_NumSumValid = m_MBinHeader.n_sumvalid;
	if (!m_New7_Stat) 	m_NumExtraValid = m_MBinHeader.n_valid / 6;
	else {
		m_NumExtraValid = 0;
		m_ValidStatBase = m_MBinHeader.n_validStatBase;
	}

	m_NumMulti = m_MBinHeader.n_param;
	m_NumSum = m_MBinHeader.n_sumvar;
	unsigned long m_InitRuns = m_TotalNumberRuns = m_MBinHeader.n_runs;
	m_NumValidAll = m_NumValid + m_NumExtraValid;
	if (m_NumValidAll == 0&&m_NumSum==0) return true;
	if (FUtil::IsFileExisting(GetStatFileName())) {
		if(!m_pNewMap->m_MStorage.EvaluateFile(true)) return false;
		if (m_pNewMap->m_MStorage.m_IsStatCompleteRead) {

			

		}
		
		if(m_pNewMap->MR_Get_TotalNumberofAddedRuns(0) < m_MBinHeader.n_runs) {
			m_MBinHeader.n_runs = m_pNewMap->MR_Get_TotalNumberofAddedRuns(0);
		}
	}
	if (m_pNewMap != nullptr) {
		if (m_NumValid != m_pNewMap->m_Val_Array.size() * m_ValidStatBase) {
			m_NumValid = m_pNewMap->m_Val_Array.size() * m_ValidStatBase;
			m_NumExtraValid = 0;
			m_NumValidAll = m_NumValid + m_NumExtraValid;
		}
	}

	
		m_TotalNumberRuns=GetNumberOfTotalRunsWithAttachedFiles();
		AllocateSpaceFromBaseVariables(m_NumValidAll);
#ifndef COUPSTD
		MFC_Util mfcutil;
		mfcutil.SetPaneMessage(" Reading from MBin File:" + filename);
#endif

	if (m_pNewMap->m_FileVersionNumberRead<=65&&!FUtil::IsFileExisting(GetStatFileName())) {
		m_TotalNumberRuns = m_MBinHeader.n_runs;
		if (!FUtil::IsFileExisting(GetStatFileName())) {

			if(!ReadAllStat()) return false;

		}
		m_IsSplitOpen = OpenAndCheckIfSplitPeriod();

	}
	else {



		ReadBaseData();
		m_IsSplitOpen = OpenAndCheckIfSplitPeriod();
		size_t index_start = 0;

		index_start = m_pNewMap->MR_Get_NumberofRuns();
		fstream multifilestream;
		for (size_t i = 0; i < GetNumberOfAddedFiles(); i++) {

			string filename = GetNameAddedFiles(i);
#ifndef COUPSTD
			mfcutil.SetPaneMessage(" Reading from Attached MBin File:" + filename);
#endif
			multifilestream.open(filename, ios::binary | ios::in);
			ReadStartDataFromAttachedFiles(&multifilestream, i);
			if (ReadBaseDataFromAttachedFiles(&multifilestream, index_start, GetNumberOfAddedRuns(i)))
				index_start += GetNumberOfAddedRuns(i);
			else {

				//Error reading from file

			}
			multifilestream.close();
		}

	}
	if (!m_pNewMap->m_MStorage.m_IsStatCompleteRead) {
		VarValidAllo();
		if (!FUtil::IsFileExisting(GetStatFileName())) 	m_pNewMap->m_MStorage.SetAllToAcceptedRuns();
	

		if (!m_pNewMap->m_MStorage.CompleteProcessingOfInputFiles_MultiStorage()) return false;

		if (m_pNewMap->m_MStorage.m_IsStatOpen)
			m_pNewMap->m_MStorage.SetPriorDistributions();
	}

	m_IsOpen=false;
	m_IsReady=true;
	m_MultiFile.close();
	if (!FUtil::IsFileExisting(GetStatFileName())) {

		if (!m_pNewMap->m_MStorage.EvaluateFile(false)) return false;

	}
	return true;
}

bool MultiStorageData::ReWriteExistingFile(unsigned long NewRuns)
{
	if(NewRuns>0) m_TotalNumberRuns=NewRuns;
	if (m_TotalNumberRuns == string::npos) return false;
	//if (m_pNewMap->m_ValidationData.GetNumSumVarVariables()*3 != m_NumSumValid) {

		m_NumSumValid = m_pNewMap->m_ValidationData.GetNumSumVarVariables() * 7;

		m_VarValidSum.resize(m_NumSumValid*m_TotalNumberRuns);
		for (unsigned long i = 0; i<m_NumSumValid*m_TotalNumberRuns; i++)
			m_VarValidSum[i] = 0.0;

	//}
	m_HeaderEnd=-1;
	
	SaveBase();
	SaveStat();
	return true;
}
string MultiStorageData::GetNewFileName()
{
	string filename="";
#ifndef COUPSTD
	/*	string pszFileName;
		LPCTSTR lpszDefExt="bin", lpszFilter;
		lpszFilter = "Multi Storage MBin File (*.MBin)|*.MBin||";
		CFileDialog	OpenFileDlg(true,lpszDefExt,pszFileName.c_str(), OFN_NOCHANGEDIR,lpszFilter);*/
		FILEDLG FDlg = MFC_Util::GetFileDialog("bin", "", "Multi Storage MBin File (*.MBin)|*.MBin||");
		if(FDlg.opengood) 
			filename=FDlg.filename;
		else
			return "";

		if(IsOpen())
			Close();
		return filename;
#endif
return filename;

}
bool MultiStorageData::AddExistingFile(string filename)
{


	return true;
}
bool MultiStorageData::Export()
{





	return false;

}
bool MultiStorageData::SaveOrgFileSize(unsigned long newnumrun)
{
	// This also means that the attachments are lost to other residual files
	m_TotalNumberRuns=newnumrun;
	return SaveBase(false);

}
bool MultiStorageData::SaveStat()
{	
	
	return false;
}
bool MultiStorageData::SaveRecord(unsigned long irec)
{
	if(!m_IsOpen){
		m_MultiFile.open(m_FileName, ios::binary|ios::out|ios::in );
		if(!m_MultiFile.is_open())
		{
			return false;
		}
	}
	std::streampos lOffset = 20;
	if(m_New7_Stat) 
	std::streampos pos, lActual;

	if (m_New7_Stat)
		lOffset = irec * m_NumMulti * 4 + 24;
	else
		lOffset=irec*m_NumMulti*4+20;

	m_MultiFile.seekp( lOffset, ios::cur );
	m_MultiFile.write((char*)&m_VarMulti[irec*m_NumMulti],4*m_NumMulti);
	
	lOffset=(m_TotalNumberRuns-irec-2)*m_NumMulti+irec*m_NumValid;
	m_MultiFile.seekp( lOffset, ios::cur );
	m_MultiFile.write((char*)&m_VarValid[irec*m_NumValid],4*m_NumValid);
	
	lOffset=(m_TotalNumberRuns-irec-2)*m_NumValid+irec*m_NumSumValid;
	m_MultiFile.write((char*)&m_VarValidSum[irec*m_NumSumValid],4*m_NumSumValid);
	
	lOffset=(m_TotalNumberRuns-irec-2)*m_NumSumValid+irec*m_NumSum;
	m_MultiFile.write((char*)&m_VarSum[irec*m_NumSum],4*m_NumSum);

	m_MultiFile.close();
	m_IsOpen=false;
	return true;

}
bool MultiStorageData::SaveBase(bool update)
{	
	if(!m_IsOpen) {
		m_MultiFile.open(m_FileName, ios::binary|ios::out|ios::in );
		if(!m_MultiFile.is_open())
		{
			m_MultiFile.close();
			m_MultiFile.clear();
			m_MultiFile.open(m_FileName, ios::binary|ios::out|ios::in );
			if(!m_MultiFile.is_open())
				return false;
		}
		m_IsOpen=true;
	}

	std::streampos pos;
	pos=m_MultiFile.tellp();
	m_MultiFile.write((char*)&m_TotalNumberRuns,4);
	m_MultiFile.write((char*)&m_NumMulti,4);
	m_MultiFile.write((char*)&m_NumValid,4);
	m_MultiFile.write((char*)&m_NumSumValid,4);
	m_MultiFile.write((char*)&m_NumSum,4);
	m_MultiFile.write((char*)&m_ValidStatBase, 4);
	pos=m_MultiFile.tellp();
	unsigned long RelPscan=25;

	pos=m_MultiFile.tellp();
	if(m_VarMulti.size()==0) {
		return false;
	}
	m_MultiFile.write((char*)&m_VarMulti[0],4*m_NumMulti*m_TotalNumberRuns);
	if(m_VarValid.size()>0) m_MultiFile.write((char*)&m_VarValid[0],4*m_NumValid*m_TotalNumberRuns);
	if(m_VarValidSum.size()>0)	m_MultiFile.write((char*)&m_VarValidSum[0],4*m_NumSumValid*m_TotalNumberRuns);
	if (m_VarSum.size()>0) m_MultiFile.write((char*)&m_VarSum[0],4*m_NumSum*m_TotalNumberRuns);
	pos=m_MultiFile.tellp();


// Adding Statistics to Storage File
	pos=m_MultiFile.tellp();
	m_HeaderEnd=pos;

	m_IsOpen=false;
	pos=m_MultiFile.tellp();
	m_MultiFile.close();
	return true;
}
bool MultiStorageData::SaveTempFile()
{	
	//CFile m_MultiTempFile;
	m_MultiFile.open(m_FileName,ios::binary|ios::out );
	if(!m_MultiFile.is_open())
	{
		return false;
	}
	
	m_MultiFile.write((char*)&m_TotalNumberRuns,4);
	m_MultiFile.write((char*)&m_NumMulti,4);
	m_MultiFile.write((char*)&m_NumValid,4);
	m_MultiFile.write((char*)&m_NumSumValid,4);
	m_MultiFile.write((char*)&m_NumSum,4);
	if(m_New7_Stat) m_MultiFile.write((char*)&m_ValidStatBase, 4);

	m_MultiFile.write((char*)&m_VarMulti[0],4*m_NumMulti*m_TotalNumberRuns);
	m_MultiFile.write((char*)&m_VarValid[0],4*m_NumValid*m_TotalNumberRuns);
	m_MultiFile.write((char*)&m_VarValidSum[0],4*m_NumSumValid*m_TotalNumberRuns);
	m_MultiFile.write((char*)&m_VarSum[0],4*m_NumSum*m_TotalNumberRuns);

	return true;
}
bool MultiStorageData::Close()
{
	if(m_IsOpen)
		m_MultiFile.close();
	return true;
}
bool MultiStorageData::Init(MR *pMap)
{
	m_pNewMap=pMap;

	return true;
}
bool MultiStorageData::InitData(string filename, unsigned long numrec, unsigned long nump, unsigned long numv,unsigned long numvsum, unsigned long numsum, MR *pMap)
{
	m_TotalNumberRuns=numrec;
	m_NumMulti=nump+10; // Changed NumMulti from +5 to +10 Variables March 17
	m_NumValid=numv*m_ValidStatBase;
	m_NumSumValid=numvsum*3;
	m_NumSum=numsum*6;
	m_pNewMap=pMap;
	m_IsMissing = false;
	m_Lock = false;
	if(m_FileName==filename)
		return false;
	m_FileName=filename;

	m_IsOpen=false;
	m_IsReady=false;
	m_MultiFile.open(m_FileName,ios::binary|ios::out );
	if(!m_MultiFile.is_open())
	{
		m_MultiFile.close();
		m_MultiFile.open(m_FileName,ios::binary|ios::out );
	};
	m_MultiFile.close();
	m_NumExtraValid = 0;
	m_NumValidAll = m_NumValid;
	if(m_VarMulti.size()==0) {
		m_VarMulti.assign(m_NumMulti*m_TotalNumberRuns,0.);
		m_VarSum.assign(m_NumSum*m_TotalNumberRuns,0.);
		m_VarValid.assign(m_NumValidAll*m_TotalNumberRuns, 0.);
		m_VarValidSum.assign(m_NumSumValid*m_TotalNumberRuns,0.);


		m_VarValidSumHigh.resize(m_NumSumValid);
		m_VarValidSumLow.resize(m_NumSumValid);
		m_VarValidMinSum.resize(m_NumSumValid);
		m_VarValidMaxSum.resize(m_NumSumValid);
		m_VarValidMinSumAll.resize(m_NumSumValid);
		m_VarValidMaxSumAll.resize(m_NumSumValid);
		m_VarValidSumCriteria.resize(m_NumSumValid);
		m_VarValidSumCriteria.resize(6);

		m_Accepted.resize(m_TotalNumberRuns);
		m_ParValidMin.resize(m_NumMulti);
		m_ParValidMax.resize(m_NumMulti);
		m_ParMin.resize(m_NumMulti);
		m_ParMax.resize(m_NumMulti);
	}
	m_AcceptedIdString.clear();
	m_AcceptedIdString.push_back("Ensemble (1)");
	
	m_MBinHeader.n_param = m_NumMulti;
	m_MBinHeader.n_valid = m_NumValid;
	m_MBinHeader.n_sumvalid = m_NumSumValid;
	m_MBinHeader.n_sumvar = m_NumSum;
	m_MBinHeader.n_runs = numrec;


	VarValidAllo();
	m_MBinHeader.n_param = m_NumMulti;
	m_MBinHeader.n_valid = m_NumValid;
	m_MBinHeader.n_sumvalid= m_NumSumValid;
	m_MBinHeader.n_sumvar = m_NumSum;
	m_MBinHeader.n_runs = numrec;

//	m_PostSumVar.resize(1);
//	m_PriorSumVar.resize(1);

	
	return true;
}
bool MultiStorageData::AddMultiP(unsigned long rec, unsigned long index, float fv)
{
	m_VarMulti[index+m_NumMulti*rec]= fv;
	return true;

}
bool MultiStorageData::AddValid(unsigned long rec, unsigned long index, float fv)
{
		m_VarValid[index+(m_NumValid+m_NumExtraValid)*rec]= fv;

	return true;
}
bool MultiStorageData::AddValidSum(unsigned long rec, unsigned long index, float fv)
{

	m_VarValidSum[index+m_NumSumValid*rec]= fv;
	return true;
}
bool MultiStorageData::AddSumV(unsigned long rec, unsigned long index, float fv)
{
	if(index<0)
		return false;
	if(m_VarSum.size()>0)
		m_VarSum[index+m_NumSum*rec]= fv;
	else {
		auto k = this;
	
	
	}
	return true;
}
float MultiStorageData::GetMultiP(unsigned long rec, unsigned long ip)
{
	if(rec*m_NumMulti+ip>=m_VarMulti.size()) return MISSING;
	float v=m_VarMulti[m_NumMulti*rec+ip];
	if(v==-numeric_limits<double>::infinity( ))
		return MISSING;
	else
		return v;
}

float MultiStorageData::GetSplitNValue(unsigned long iv, unsigned long ind_split)
{
	unsigned long ip=iv;
	std::streampos pos=4+ip*m_SplitRecRuns*4;
	//LONGLONG pos=m_SplitRecRuns*ind_split+4;
	if(pos!=m_SplitPos) {
		m_SplitPeriodFile.seekg(pos, ios::beg);
		m_SplitPos=m_SplitPeriodFile.tellg();
		m_SplitPeriodFile.read((char*)&m_VarSplit[0], m_SplitRecRuns*4);
	}
	unsigned long j=(unsigned long)(m_SplitRec)*ind_split;
	float v=m_VarSplit[j+7];
	//float v=m_VarSplit[ip];
	if(v==-numeric_limits<double>::infinity( ))
		return MISSING;
	else
		return v;
}
float MultiStorageData::GetSplitValid(unsigned long irun, unsigned long ip, unsigned long ind_split )
{
	if(irun<0||irun>1000000) return MISSING;
	unsigned long varno=ip%m_NumValidVar;
	unsigned long varid=ip/m_NumValidVar;
	std::streampos pos=4+varno*m_SplitRecRuns*4;
	if(pos!=m_SplitPos) {
		if(!m_SplitPeriodFile.good()) {
			ReOpenSplitFile();
		}
		m_SplitPeriodFile.seekg(pos, ios::beg);
		m_SplitPos=m_SplitPeriodFile.tellg();
		if(m_SplitPos==pos)
			m_SplitPeriodFile.read((char*)&m_VarSplit[0], m_SplitRecRuns*4);
		else
			return MISSING;
	}
	unsigned long j=(unsigned long)(m_SplitRec)*ind_split+(unsigned long)(m_SplitRec)*m_NumSplitPeriod*irun;
	float v=m_VarSplit[j+varid];
	if(v==-numeric_limits<double>::infinity( ))
		return MISSING;
	else
		return v;
}
float MultiStorageData::GetValid(unsigned long rec, unsigned long ip, size_t i_cycle_start, size_t i_cycle)
{
	if(i_cycle_start!=string::npos) {
		m_CyclicStartPos = i_cycle_start;
	}
	if(i_cycle!=string::npos) {
		m_CyclicNumber = i_cycle;
	}
	if (m_CyclicNumber > 0) {
		if (rec % m_CyclicNumber != m_CyclicStartPos)
			return MISSING;
	}
	if (rec*(m_NumValid+m_NumExtraValid)+ip>=m_VarValid.size()) return MISSING;
	
	auto index=ip+(m_NumValid+m_NumExtraValid)*rec;
	float v=m_VarValid[index];
	if(v==-numeric_limits<double>::infinity( ))
		return MISSING;
	else
		return v;
}




void MultiStorageData::SetAcceptedRunNo(unsigned long index, bool value)
{
	if (m_VarMulti.size() == 0) return;
	unsigned long apos=m_NumMulti-10;
	if(value)
		m_VarMulti[index*m_NumMulti+apos]=1.;
	else
		m_VarMulti[index*m_NumMulti+apos]=0.;
}

bool MultiStorageData::RemoveAcceptedRunNo(unsigned long index)
{



	return false;
}

void MultiStorageData::ReAssignPerformanceandAcceptanceFromAggregation(size_t aggregate_num)
{
	if (aggregate_num <= 1) return;
	for (unsigned long i = 0; i < m_TotalNumberRuns; i+=aggregate_num) {
		float Accepted = GetMultiP(i+aggregate_num-1, m_NumMulti - 10);
		float LogLikelihood = GetMultiP(i+aggregate_num-1, m_NumMulti - 7);
			for (size_t ii = 0; ii < aggregate_num-1; ii++) {
				AddMultiP(i + ii, m_NumMulti-10, Accepted);
				AddMultiP(i + ii, m_NumMulti-7, LogLikelihood);
			}
	}
}
void MultiStorageData::CommonPerformanceFromAggregation(size_t aggregate_num)
{
	if (aggregate_num <= 1) return;

	vector<float>  SumPerform;
	SumPerform.resize( m_NumValid);
	for (unsigned long i = 0; i < m_TotalNumberRuns; i+=aggregate_num) {
			for (size_t i3 = 0; i3 < m_NumValid; i3++) SumPerform[i3] = 0.;
			for (size_t ii = 0; ii < aggregate_num; ii++) {
				for (size_t i3 = 0; i3 < m_NumValid; i3++) {
					SumPerform[i3] += GetValid(i + ii, i3);
				}
			}
			for (size_t ii = 0; ii < aggregate_num; ii++) {
				for (size_t i3 = 0; i3 < m_NumValid; i3++) {
					AddValid(i + ii, i3, SumPerform[i3]/aggregate_num);
				}
			}
	}
}

bool MultiStorageData::GetAcceptedRunNo(unsigned long index)
{
	float v=GetMultiP(index, m_NumMulti-10);
	if(v<=0.)
		return false;
	else
		return true;
}

float MultiStorageData::GetValidHigh(unsigned long ip, unsigned long ind_split)
{
	if (m_Valid_TSV.size() <= m_ActualEnsemble) return 0.;
	if (m_Valid_TSV[m_ActualEnsemble].v[E_SELECT_HIGH].size() <= ip) return MISSING;
	float v=m_Valid_TSV[m_ActualEnsemble].v[E_SELECT_HIGH][ip+ind_split*(m_NumValid+m_NumExtraValid)];
	if (v == -numeric_limits<double>::infinity()) {
		m_Valid_TSV[m_ActualEnsemble].v[E_SELECT_HIGH][ip + ind_split * (m_NumValid + m_NumExtraValid)] = 1.1E38;
		return -MISSING;
	}
	else
		return v;
}
float MultiStorageData::GetValidLow(unsigned long ip, unsigned long ind_split)
{
	if (m_Valid_TSV.size() <= m_ActualEnsemble) return 0.;
	if (m_Valid_TSV[m_ActualEnsemble].v[E_SELECT_LOW].size() <= ip) return MISSING;
	float v = m_Valid_TSV[m_ActualEnsemble].v[E_SELECT_LOW][ip + ind_split*(m_NumValid + m_NumExtraValid)];
	if (v == -numeric_limits<double>::infinity()) {
		m_Valid_TSV[m_ActualEnsemble].v[E_SELECT_LOW][ip + ind_split * (m_NumValid + m_NumExtraValid)] = MISSING;
		return MISSING;

	}
	else
		return v;
}


float MultiStorageData::GetSumV(unsigned long rec, unsigned long ip)
{
	return m_VarSum[ip+m_NumSum*rec];
}
float MultiStorageData::GetSumV_Selected(unsigned long rec, unsigned long ip)
{
	unsigned long ipp=SelSumV_Index_Get(ip);
	if(ipp>=0)
		return m_VarSum[ipp+m_NumSum*rec];
	else
		return MISSING;
}
unsigned long MultiStorageData::GetNumAccepted() 
{
	if ((m_TotalNumAccepted == 0 || m_TotalNumAccepted>m_TotalNumberRuns)&&m_TotalNumberRuns!=(unsigned long)(string::npos)) {
		unsigned long icount = 0;

		for (unsigned long i = 0; i < m_TotalNumberRuns; i++)
			if (GetMultiP(i, m_NumMulti - 10)>0)
				icount++;
		m_TotalNumAccepted = icount;
	}
	else if (m_TotalNumAccepted == string::npos)
		m_TotalNumAccepted = 0;

	return m_TotalNumAccepted;
}
unsigned long MultiStorageData::GetNumBayesAccepted(unsigned long num_burn_in) 
{
	if ((m_TotalNumBayesAccepted == 0 || m_TotalNumBayesAccepted > m_TotalNumberRuns)&&m_TotalNumberRuns != string::npos) {
		unsigned long icount = 0;

		for (unsigned long i = num_burn_in; i < m_TotalNumberRuns; i++)
			if (GetMultiP(i, m_NumMulti - 10)>0)
				icount++;
		m_TotalNumBayesAccepted = icount;
	}
	else if (m_TotalNumBayesAccepted == string::npos)
		m_TotalNumBayesAccepted = 0;
	return m_TotalNumBayesAccepted;
}

float MultiStorageData::GetMinPar(unsigned long ip)
{
	if (m_ParMin.size() <= ip) return 1.1E38;
	if(!m_pNewMap->m_MStorage.IsMinUpdated[m_pNewMap->m_MStorage.m_ActualEnsemble]) {
		float v=1.1E38f;
		for(unsigned long i=0;i<m_TotalNumberRuns;i++) {
			if(GetMultiP(i,ip)<v)
				v=GetMultiP(i,ip);
		}
		m_ParMin[ip]=v;
		return v;
	}
	else 
		return m_ParMin[ip];

}
float MultiStorageData::GetMaxPar(unsigned long ip)
{
	if (m_ParMax.size() <= ip) return -1.1E38;
	if(!m_pNewMap->m_MStorage.IsMinUpdated[m_pNewMap->m_MStorage.m_ActualEnsemble]) {
		float v=float(MISSING);
		for(unsigned long i=0;i<m_TotalNumberRuns;i++) {
			if(GetMultiP(i,ip)>v)
				v=GetMultiP(i,ip);
		}
		m_ParMax[ip]=v;
		return v;
	}
	else
		return m_ParMax[ip];

}



unsigned long MultiStorageData::GetSumCriteria(unsigned long index)
{
	if(m_VarValidSumCriteria.size()<=index) return false;
	return m_VarValidSumCriteria[index];
}
bool MultiStorageData::SetSumCriteria(unsigned long index, unsigned long value)
{
	if(m_VarValidSumCriteria.size()<=index||index>=m_NumSumValid) return false;
	m_VarValidSumCriteria[index]=value;
	return true;
}
void MultiStorageData::ResetSumCriteria()
{
     for(unsigned long i=0;i<m_NumSumValid;i++)
		 m_VarValidSumCriteria[i]=0;
}



bool MultiStorageData::IsOpen()
{
	return m_IsOpen;
}
bool MultiStorageData::SelSumV_Index_Add(unsigned long index) 
{
	m_VarSumSel.push_back(index);
	return true;
}
bool MultiStorageData::SelSumV_Index_SelectAll() 
{
	m_VarSumSel.clear();
	for(unsigned long i=0;i<m_NumSum;i++)
		m_VarSumSel.push_back(i);
	return true;
}
void MultiStorageData::SelSumV_Index_Reset() 
{
	m_VarSumSel.clear();
}
unsigned long MultiStorageData::SelSumV_Index_Get(unsigned long index) 
{
	if(index>=m_VarSumSel.size())
		return string::npos;
	else
		return m_VarSumSel[index];
}
unsigned long MultiStorageData::SelSumV_Index_GetNum() 
{
	return m_VarSumSel.size();
}
void MultiStorageData::SetCoCorValid_Init(unsigned long Nindex)
{
	m_CoCorValid.resize(Nindex*Nindex);
	m_CoCorNumValid=Nindex;
}
void MultiStorageData::SetCoCorValid(unsigned long i1, unsigned long i2, float v, size_t act_ensemble){
if (act_ensemble != string::npos) {
	if (i1*m_CoCorNumValid + i2 >= m_Valid_TSV[act_ensemble].copar.size())
	m_Valid_TSV[act_ensemble].copar[i1*m_CoCorNumValid + i2] = v;

}
else {
	if (i1*m_CoCorNumValid + i2 <= m_CoCorValid.size())
		m_CoCorValid[i1*m_CoCorNumValid + i2] = v;
}
}
float MultiStorageData::GetCoCorValid(unsigned long i1, unsigned long i2, size_t act_ensemble)
{
	if (act_ensemble != string::npos) {
		if (i1*m_CoCorNumValid + i2 >= m_Valid_TSV[act_ensemble].copar.size())
			return m_Valid_TSV[act_ensemble].copar[i1*m_CoCorNumValid + i2];
		else
			return MISSING;

	}
	else {
		if (m_CoCorValid.size() <= i1*m_CoCorNumValid + i2) return MISSING;
		return m_CoCorValid[i1*m_CoCorNumValid + i2];
	}

}
bool MultiStorageData::IsCoCorValidSet()
{
	if(m_Valid_TSV[m_ActualEnsemble].copar.size()>0&&!m_Valid_TSV[m_ActualEnsemble].is_Copar_Updated)
		return false;
	else
		return true;
}
void MultiStorageData::SetCoParUpdated(bool updated) {
	m_Valid_TSV[m_ActualEnsemble].is_Copar_Updated = updated;
}
size_t MultiStorageData::Get_ActualFilterVarValue(size_t RunNo)
{

	for (int ii = 0; ii < m_pNewMap->MR_Get_NumberOfParametersWithinDim(1); ii++) {
		auto pValIndex = (Ps*)m_pNewMap->MR_Get_pPar(1, ii + 1);
		string namep = pValIndex->GetName();
		if (namep == "Filter Var Value")
			return size_t(m_pNewMap->m_MStorage.GetMultiP(RunNo, ii+1)+0.0001);
	}
	return string::npos;
}
unsigned long MultiStorageData::GetNumberOfAddedFiles(string str)
{
	if(m_IsOpen)
		return m_AddedFiles.size();
	else if(str!="") {
		OpenFile(str,true);
		return	m_AddedFiles.size();
	}
	return 0;
}
string MultiStorageData::GetNameAddedFiles(unsigned long index)
{
	if(index!=string::npos&&index<m_AddedFiles.size())
		return m_AddedFiles[index];
	else
		return "";

}
string MultiStorageData::GetNameBaseFile()
{
	return m_FileName;

}
unsigned long MultiStorageData::GetNumberOfRuns()
{
	return m_TotalNumberRuns;
}
unsigned long MultiStorageData::GetNumberOfTotalRunsWithAttachedFiles() {
	m_TotalNumberRuns = m_MBinHeader.n_runs;
	for (size_t i = 0; i < m_AddedFiles.size(); i++)
		m_TotalNumberRuns += GetNumberOfAddedRuns(i);
	return m_TotalNumberRuns;
}
unsigned long MultiStorageData::GetNumberOfAddedRuns(unsigned long index)
{
	if(index<m_AddedFiles.size())
		return m_AddedNumRuns[index];
	else
		return 0;
}
unsigned long MultiStorageData::GetFirstRunNoAddedRuns(unsigned long index)
{
	if(index<m_AddedFiles.size())
		return m_AddedFirstRunNo[index];
	else
		return 0;
}
unsigned long MultiStorageData::GetAdd_MultiPGFileIndex(unsigned long simRunNo)
{
	unsigned long index=string::npos;
	unsigned long count;
	count=m_OrigFirstRunNo;
	for(unsigned long i=0; i<m_AddedFiles.size();i++) {
		if(simRunNo>=count+GetNumberOfAddedRuns(i)) {
			index=i;
		}
	}
	return index;
}
void MultiStorageData::AllocateValid_TSV(size_t i) {
	if (m_NumValidAdd != string::npos && m_MBinHeader.n_param != string::npos) {
		size_t num_indicators = 7;

		m_Valid_TSV[i].v[E_PRIOR_MIN].resize(m_NumValidAll);
		m_Valid_TSV[i].v[E_PRIOR_MAX].resize(m_NumValidAll);
		m_Valid_TSV[i].v[E_PRIOR_MEAN].resize(m_NumValidAll);
		m_Valid_TSV[i].v[E_PRIOR_CV].resize(m_NumValidAll);
		m_Valid_TSV[i].v[E_POST_MIN].resize(m_NumValidAll);
		m_Valid_TSV[i].v[E_POST_MAX].resize(m_NumValidAll);
		m_Valid_TSV[i].v[E_POST_MEAN].resize(m_NumValidAll);
		m_Valid_TSV[i].v[E_POST_CV].resize(m_NumValidAll);
		m_Valid_TSV[i].v[E_SELECT_LOW].resize(m_NumValidAll);
		m_Valid_TSV[i].v[E_SELECT_HIGH].resize(m_NumValidAll);
		m_Valid_TSV[i].criteria.resize(m_NumValidAll);
		m_Valid_TSV[i].meancriteria.resize(num_indicators);
		m_Valid_TSV[i].tobe_Plotted = true;
		m_Valid_TSV[i].is_Copar_Updated = false;
		m_Valid_TSV[i].copar.resize(m_MBinHeader.n_param * m_MBinHeader.n_param);
		m_Valid_TSV[i].ParDist.resize(m_MBinHeader.n_param);


		m_Valid_TSV_Mean[i].v[E_PRIOR_MIN].resize(num_indicators);
		m_Valid_TSV_Mean[i].v[E_PRIOR_MAX].resize(num_indicators);
		m_Valid_TSV_Mean[i].v[E_PRIOR_MEAN].resize(num_indicators);
		m_Valid_TSV_Mean[i].v[E_PRIOR_CV].resize(num_indicators);
		m_Valid_TSV_Mean[i].v[E_POST_MIN].resize(num_indicators);
		m_Valid_TSV_Mean[i].v[E_POST_MAX].resize(num_indicators);
		m_Valid_TSV_Mean[i].v[E_POST_MEAN].resize(num_indicators);
		m_Valid_TSV_Mean[i].v[E_POST_CV].resize(num_indicators);
		m_Valid_TSV_Mean[i].v[E_SELECT_LOW].resize(num_indicators);
		m_Valid_TSV_Mean[i].v[E_SELECT_HIGH].resize(num_indicators);

	}
}
void MultiStorageData::AllocateValid_SUM(size_t i) {
	size_t num_perf = size_t(NSE_STAT+1);
	size_t newNumSumValid = m_pNewMap->GetNumberofSumVariables() *num_perf;
	m_Valid_SV[i].v[E_PRIOR_MIN].resize(newNumSumValid);
	m_Valid_SV[i].v[E_PRIOR_MAX].resize(newNumSumValid);
	m_Valid_SV[i].v[E_PRIOR_MEAN].resize(newNumSumValid);
	m_Valid_SV[i].v[E_PRIOR_CV].resize(newNumSumValid);
	m_Valid_SV[i].v[E_POST_MIN].resize(newNumSumValid);
	m_Valid_SV[i].v[E_POST_MAX].resize(newNumSumValid);
	m_Valid_SV[i].v[E_POST_MEAN].resize(newNumSumValid);
	m_Valid_SV[i].v[E_POST_CV].resize(newNumSumValid);
	m_Valid_SV[i].v[E_SELECT_LOW].resize(newNumSumValid);
	m_Valid_SV[i].v[E_SELECT_HIGH].resize(newNumSumValid);
	m_Valid_SV[i].criteria.resize(newNumSumValid);
	m_Valid_SV[i].meancriteria.resize(num_perf);

	m_Valid_SV_Mean[i].v[E_PRIOR_MIN].resize(num_perf);
	m_Valid_SV_Mean[i].v[E_PRIOR_MAX].resize(num_perf);
	m_Valid_SV_Mean[i].v[E_PRIOR_MEAN].resize(num_perf);
	m_Valid_SV_Mean[i].v[E_PRIOR_CV].resize(num_perf);
	m_Valid_SV_Mean[i].v[E_POST_MIN].resize(num_perf);
	m_Valid_SV_Mean[i].v[E_POST_MAX].resize(num_perf);
	m_Valid_SV_Mean[i].v[E_POST_MEAN].resize(num_perf);
	m_Valid_SV_Mean[i].v[E_POST_CV].resize(num_perf);
	m_Valid_SV_Mean[i].v[E_SELECT_LOW].resize(num_perf);
	m_Valid_SV_Mean[i].v[E_SELECT_HIGH].resize(num_perf);
	m_Valid_SV_Mean[i].criteria.resize(num_perf);






}

void MultiStorageData::VarValidAllo()
{

	m_NumValidAll=m_NumValid+m_NumExtraValid;
	if(m_NumSplitPeriod>0) {
		m_NumValidAll+=m_NumValidAll*m_NumSplitPeriod;
	}

	if (m_ParMin.size() != m_MBinHeader.n_param) {
		m_ParMin.resize(m_MBinHeader.n_param);
		m_ParMax.resize(m_MBinHeader.n_param);
		m_ParValidMin.resize(m_MBinHeader.n_param);
		m_ParValidMax.resize(m_MBinHeader.n_param);
	}


	if (m_AcceptedRuns.size() == 0) m_AcceptedRuns.resize(1);
	if (m_Valid_TSV.size() < m_AcceptedRuns.size()) {
		m_Valid_TSV.resize(m_AcceptedRuns.size());
		m_pNewMap->m_MStorage.AlloMinMaxUpdated(m_AcceptedRuns.size());
	}
	if (m_Valid_TSV_Mean.size() < m_AcceptedRuns.size()) {
		m_Valid_TSV_Mean.resize(m_AcceptedRuns.size());

	}

	for(size_t i=0; i< m_AcceptedRuns.size(); i++)	AllocateValid_TSV(i);
	if (m_Valid_SV.size() < m_AcceptedRuns.size()) m_Valid_SV.resize(m_AcceptedRuns.size());
	if (m_Valid_SV_Mean.size() < m_AcceptedRuns.size()) m_Valid_SV_Mean.resize(m_AcceptedRuns.size());
	for (size_t i = 0; i< m_AcceptedRuns.size(); i++) AllocateValid_SUM(i);

}

bool MultiStorageData::AllocateSpaceFromBaseVariables(size_t NumValid) {

	m_VarMulti.resize(m_TotalNumberRuns*m_MBinHeader.n_param);
	m_VarSum.resize(m_TotalNumberRuns*m_MBinHeader.n_sumvar);
	m_VarValid.resize(m_TotalNumberRuns*(NumValid));
	m_VarValidSum.resize(m_TotalNumberRuns*m_MBinHeader.n_sumvalid);
	return true;
}
bool MultiStorageData::CalculateMeanValidValues(bool SV_Variables)
{
	if (SV_Variables) {
		return m_pNewMap->CalculateSumMeanValidValues_SV_Variables();
		
	}
	if (m_NumValidVar == 0) return false;
	unsigned long NumValidVar = m_NumValid / 7;
	float dsum[7];
	unsigned long count;
	float fvalue;
	for (unsigned long i = 0; i < m_TotalNumberRuns; i++) {
		for (unsigned long jx = 0; jx < 7; jx++) {
			if (m_MeanTSV_All[jx].size() < m_TotalNumberRuns) m_MeanTSV_All[jx].resize(m_TotalNumberRuns);
			dsum[jx] = 0.;
			count = 0;

				for (unsigned long j = 0; j < NumValidVar; j++) {
					fvalue = m_VarValid[j + jx*NumValidVar + i*m_NumValid];
					if (fvalue > MISSING && !(jx == 2 && fvalue == 0)) {
						count++;
						if (jx == 3)
							dsum[jx] += fabs(fvalue);
						else if (jx == 6) {
							auto rmse = m_VarValid[j + 4 * NumValidVar + i*m_NumValid];
							dsum[jx] += 1 - rmse*rmse*m_pNewMap->m_ValidationData.GetValn(j) / m_pNewMap->m_ValidationData.GetValRMSE_O(j);
						}
						else
							dsum[jx] += fvalue;
					}
				}
				if (count == NumValidVar) {
					if (jx == 5)
						m_MeanTSV_All[jx][i] = dsum[jx];
					else
						m_MeanTSV_All[jx][i] = dsum[jx] / count;
				}
				else {
					m_MeanTSV_All[jx][i] = float(MISSING);
			}
		}
	}
	return	true;
}
void MultiStorageData::AllocateMeanValidVar() {

	if (m_NumValid == 7) {
		m_NumValidVar = m_NumValid/m_ValidStatBase ;
		m_ValidDynLogLi.resize(m_NumValidVar * 13 * 7 * 2);
		m_ValidDynResiduals.resize(m_NumValidVar * 13 * 7 * 2);
	}
	else if (m_NumValid > 6) {
		m_NumValidVar = m_NumValid/m_ValidStatBase ;
		//m_VarTSV_Mean
	//	m_Valid_TSV_Mean.v[E_PRIOR_MIN].resize(m_TotalNumberRuns);

		for (size_t i = 0; i < m_MeanTSV_All.size(); i++) {
			m_MeanTSV_All[i].resize(m_TotalNumberRuns);
			m_MeanSV_All[i].resize(m_TotalNumberRuns);

		}

		
		m_ValidDynLogLi.resize(m_NumValidVar * 13 * 7 * 2);
		m_ValidDynResiduals.resize(m_NumValidVar * 13 * 7 * 2);


		CalculateMeanValidValues();
		CalculateMeanValidValues(true);

	
	}
	else {
		m_NumValidVar = 0;
		m_VarValidSumMean_AbsME.resize(m_TotalNumberRuns);
		m_VarValidSumMean_RMSE.resize(m_TotalNumberRuns);
		for (size_t i = 0; i < 7; i++) m_MeanSV_All[i].resize(m_TotalNumberRuns);
		CalculateMeanValidValues(true);
		/*	CalculateSumMeanValidValues_SV_Variables();
		if (m_NumSumValid>3 && m_Lock)
		UpdateMeanSumValidValues();*/
	}

	for (size_t i = 0; i < 7; i++) m_MeanSV_All[i].resize(m_TotalNumberRuns);
	CalculateMeanValidValues(true);

}
bool MultiStorageData::ReadAllStat(){

	bool check;
	check = true;
	
	std::streampos actpos, nbyte;
	bool readmore;
	m_Lock = false;


	m_VarMulti.resize(m_NumMulti*m_TotalNumberRuns);
	m_VarSum.resize(m_NumSum*m_TotalNumberRuns);
	m_VarValid.resize((m_NumValid + m_NumExtraValid)*m_TotalNumberRuns);
	m_VarValidSum.resize(m_NumSumValid*m_TotalNumberRuns);
	VarValidAllo();

	m_VarValidSumHigh.resize(m_NumSumValid);
	m_VarValidSumLow.resize(m_NumSumValid);

	m_VarValidMaxSum.resize(m_NumSumValid);
	m_VarValidMinSum.resize(m_NumSumValid);
	m_VarValidMeanSum.resize(m_NumSumValid);
	m_VarValidCVSum.resize(m_NumSumValid);

	m_VarValidMaxSumAll.resize(m_NumSumValid);
	m_VarValidMeanSumAll.resize(m_NumSumValid);
	m_VarValidCVSumAll.resize(m_NumSumValid);
	m_VarValidMinSumAll.resize(m_NumSumValid);
	m_VarValidSumCriteria.resize(m_NumSumValid);



	m_VarValidMeanCriteria.resize(7);
	m_VarValidMeanSumCriteria.resize(3);

	m_ParValidMin .resize(m_NumMulti);
	m_ParValidMax .resize(m_NumMulti);
	m_ParMin .resize(m_NumMulti);
	m_ParMax .resize(m_NumMulti);

	m_Accepted.resize(m_TotalNumberRuns);
	unsigned long nb = sizeof(float)*m_NumMulti*m_TotalNumberRuns;

	m_HeaderStart = m_MultiFile.tellg();
	auto prior = m_HeaderStart;
	m_MultiFile.read((char*)&m_VarMulti[0], nb);
	actpos = m_MultiFile.tellg();
	nbyte = m_MultiFile.gcount();
	check = m_MultiFile.good();
	m_MultiFile.clear();
	if (nb + prior != actpos)
		m_MultiFile.seekg(nb + prior, ios::beg);

	prior = m_MultiFile.tellg();
	m_MultiFile.clear();
	m_MultiFile.seekg(prior);

	nb = sizeof(float)*m_NumValid*m_TotalNumberRuns;
	if(nb>0) m_MultiFile.read((char*)&m_VarValid[0], nb);

	prior = m_MultiFile.tellg();
	nb = sizeof(float)*m_NumSumValid*m_TotalNumberRuns;
	if(nb>0) m_MultiFile.read((char*)&m_VarValidSum[0], nb);
	actpos = m_MultiFile.tellg();
	nbyte = m_MultiFile.gcount();
	check = m_MultiFile.good();
	m_MultiFile.clear();

	if (nb + prior != actpos)
		m_MultiFile.seekg(nb + prior, ios::beg);



	nb = sizeof(float)*m_NumSum*m_TotalNumberRuns;
	m_MultiFile.read((char*)&m_VarSum[0], nb);


	m_HeaderStart = m_MultiFile.tellg();
	string str;

	readmore = true;
	AllocateMeanValidVar();

	m_HeaderStart = m_MultiFile.tellg();
	if (m_HeaderStart <= 0) {

		return false;

	};
	
	if (!m_MultiFile.good()) return false;

	if (!m_Lock) {
		if (readmore) {
			if (m_NumValid > 0) {
				m_MultiFile.read((char*)&m_Valid_TSV[0].v[E_SELECT_LOW][0], 4 * (m_NumValidAll));
				m_MultiFile.read((char*)&m_Valid_TSV[0].v[E_SELECT_HIGH][0], 4 * (m_NumValidAll));
				m_MultiFile.read((char*)&m_Valid_TSV[0].v[E_POST_MIN][0], 4 * (m_NumValidAll));
				m_MultiFile.read((char*)&m_Valid_TSV[0].v[E_POST_MAX][0], 4 * (m_NumValidAll));
			}
			if (m_NumSumValid > 0) {
				m_MultiFile.read((char*)&m_VarValidSumLow[0], 4 * m_NumSumValid);
				m_MultiFile.read((char*)&m_VarValidSumHigh[0], 4 * m_NumSumValid);
				m_MultiFile.read((char*)&m_VarValidMinSum[0], 4 * m_NumSumValid);
				m_MultiFile.read((char*)&m_VarValidMaxSum[0], 4 * m_NumSumValid);
				m_MultiFile.read((char*)&m_Valid_SV[0].v[E_SELECT_LOW][0], 4 * (m_NumSumValid));
				m_MultiFile.read((char*)&m_Valid_SV[0].v[E_SELECT_HIGH][0], 4 * (m_NumSumValid));
				m_MultiFile.read((char*)&m_Valid_SV[0].v[E_POST_MIN][0], 4 * (m_NumSumValid));
				m_MultiFile.read((char*)&m_Valid_SV[0].v[E_POST_MAX][0], 4 * (m_NumSumValid));
			}
			if (!m_MultiFile.good()) return false;
			ReadAcceptedRunInfo();



		}
		else {
			for (unsigned long i = 0; i < m_TotalNumberRuns; i++)
				SetAcceptedRunNo(i, true);
			    m_pNewMap->m_MStorage.IsMinUpdated.assign(m_pNewMap->m_MStorage.IsMinUpdated.size(),false);
				m_pNewMap->m_MStorage.IsMaxUpdated.assign(m_pNewMap->m_MStorage.IsMaxUpdated.size(), false);
				for (unsigned long split = 0; split <m_NumSplitPeriod; split++) {
						for (unsigned long i = 0; i < m_NumValid + m_NumExtraValid; i++) {
							size_t index = i + split*(m_NumValid + m_NumExtraValid);
							m_Valid_TSV[0].v[E_SELECT_LOW][index] = m_Valid_TSV[0].v[E_PRIOR_MIN][index];
							m_Valid_TSV[0].v[E_SELECT_HIGH][index] = m_Valid_TSV[0].v[E_PRIOR_MAX][index];
							//UpdateAccepted(i, -1);
						}
				}
						for (unsigned long i = 0; i < m_NumSumValid; i++) {
						//	SetValidSumLow(i, GetMinSumValid(i));
						//	SetValidSumHigh(i, GetMaxSumValid(i));
						//	UpdateAccepted(-1, i);
						}
		}
		
	}
	return true;
}
bool MultiStorageData::ReadNewData(unsigned long InitRuns) {
	
	m_TotalNumberRuns = InitRuns;
	return true;
}
bool MultiStorageData::ReadBaseData() {
	
	unsigned long nb = sizeof(float)*m_MBinHeader.n_param*m_MBinHeader.n_runs;
	auto koll = m_MultiFile.tellg();
	if (m_MBinHeader.n_param>0)		m_MultiFile.read((char*)&m_VarMulti[0], nb);
	if (m_MBinHeader.n_valid>0)		m_MultiFile.read((char*)&m_VarValid[0], 4 * m_MBinHeader.n_valid*m_MBinHeader.n_runs);
	koll = m_MultiFile.tellg();
	if(m_MBinHeader.n_sumvalid>0)	m_MultiFile.read((char*)&m_VarValidSum[0], 4 * m_MBinHeader.n_sumvalid*m_MBinHeader.n_runs);
	koll = m_MultiFile.tellg();
	if (m_MBinHeader.n_sumvar>0)	m_MultiFile.read((char*)&m_VarSum[0], 4 * m_MBinHeader.n_sumvar*m_MBinHeader.n_runs);
	koll = m_MultiFile.tellg();

	return true;
}
bool MultiStorageData::ReadBaseDataFromAttachedFiles(fstream *File, size_t index_start, size_t runs_to_read) {

	unsigned long nb = sizeof(float)*m_NumMulti*runs_to_read;
	File->read((char*)&m_VarMulti[index_start*m_NumMulti], nb);
	if(m_NumValid>0) File->read((char*)&m_VarValid[index_start*m_NumValid], 4 * m_NumValid*runs_to_read);
	if (m_NumSumValid>0) File->read((char*)&m_VarValidSum[index_start*m_NumSumValid], 4 * m_NumSumValid*runs_to_read);
	if (m_NumSum>0) File->read((char*)&m_VarSum[index_start*m_NumSum], 4 * m_NumSum*runs_to_read);

	return true;
}
MBIN_FILE_HEADER MultiStorageData::ReadStartData() {
	MBIN_FILE_HEADER out;
	
	out.streamgood = true;
	unsigned long m_InitRuns = m_TotalNumberRuns;

	std::streampos actpos;
	char* pf;
	pf = new char[24];
	m_MultiFile.seekg(0, ios::beg);
	actpos = m_MultiFile.tellg();
	out.streamgood = m_MultiFile.good();
	if (m_pNewMap->m_FileVersionNumberRead > 70) {
		m_New7_Stat = true;
		m_MultiFile.read(pf, 24);
	}
	else {
		m_New7_Stat = false;
		out.n_validStatBase = 6;
		m_MultiFile.read(pf, 20);
	}

	actpos = m_MultiFile.tellg();
	auto nbyte = m_MultiFile.gcount();
	out.streamgood = m_MultiFile.good();

	out.n_runs = *(unsigned long*)pf;
	out.n_param = *(unsigned long*)(pf + 4);
	out.n_valid = *(unsigned long*)(pf + 8);
	out.n_sumvalid = *(unsigned long*)(pf + 12);
	out.n_sumvar = *(unsigned long*)(pf + 16);
	if (m_New7_Stat)
		out.n_validStatBase = *(unsigned long*)(pf + 20);


	delete[]pf;

	return out;
}
MBIN_FILE_HEADER MultiStorageData::ReadStartDataFromAttachedFiles(fstream *File, size_t ifile) {
	MBIN_FILE_HEADER out;
	out.streamgood = true;
	unsigned long m_InitRuns = m_TotalNumberRuns;

	std::streampos actpos;
	char *pf;

	
	pf = new char[24];
	File->seekg(0, ios::beg);
	actpos = File->tellg();
	out.streamgood = File->good();
	if (m_pNewMap->m_FileVersionNumberRead > 70) {
		m_New7_Stat = true;
		File->read(pf, 24);
	}
	else {
		m_New7_Stat = false;
		out.n_validStatBase = 6;
	}
	

	actpos = File->tellg();
	auto nbyte = File->gcount();
	out.streamgood = File->good();


	out.n_runs = *(unsigned long*)pf;
	out.n_param = *(unsigned long*)(pf + 4);
	out.n_valid = *(unsigned long*)(pf + 8);
	out.n_sumvalid = *(unsigned long*)(pf + 12);
	out.n_sumvar = *(unsigned long*)(pf + 16);
	if(m_New7_Stat)
	out.n_validStatBase= *(unsigned long*)(pf + 20);


	delete[]pf;

	return out;
}
bool MultiStorageData::OpenAndCheckIfSplitPeriod() {
	m_IsSplitOpen = false;
	string filenamesub = GetSplitFileName();
	if (FUtil::IsFileExisting(filenamesub)) {
		m_NumSplitPeriod = 0;
		m_SplitPeriodFile.open(filenamesub, ios::binary | ios::in | ios::out);
		if (m_SplitPeriodFile.good()) {
			std::streampos tot;
			m_SplitPeriodFile.seekg(0, ios::end);
			tot = m_SplitPeriodFile.tellg();
			m_SplitPeriodFile.seekg(0, ios::beg);
			if (tot>4)
				m_SplitPeriodFile.read((char*)&m_NumSplitPeriod, 4);
			else
				m_NumSplitPeriod = 0;
			unsigned long ival;
			if(m_New7_Stat)
				ival = m_NumValid / 7;
			else
				ival = m_NumValid/m_ValidStatBase ;


			m_SplitRec = (m_NumValid + 2 * m_NumValid/m_ValidStatBase  + m_NumExtraValid) / ival;
			//m_SplitRecRuns = m_TotalNumberRuns*m_SplitRec*m_NumSplitPeriod;

			if (m_pNewMap->MR_CheckIndexForFile_RepMode()&& m_NumSplitPeriod>0)
				m_pNewMap->m_MultiFileSplitMode = true;

			if (m_pNewMap->m_MultiFileSplitMode)
				m_SplitRecRuns = m_TotalNumberRuns * m_SplitRec;
			else
				m_SplitRecRuns = m_TotalNumberRuns * m_SplitRec * m_NumSplitPeriod;




			m_VarSplit.resize((unsigned long)(m_SplitRecRuns));
			if (m_SplitPeriodFile.good())
				m_IsSplitOpen = true;

			if (m_NumSplitPeriod>0) {
				m_NumValidAll += m_NumValidAll*m_NumSplitPeriod;
			}
		}
	};
	return m_IsSplitOpen;
}

bool MultiStorageData::ReadAcceptedRunInfo() {
	unsigned long nbyte;
	string str;
	char buf[1000];
	if (!m_MultiFile.good()) return false;
	m_MultiFile.read((char*)&m_TotalNumAccepted, 4);

	if (m_TotalNumAccepted <= 0 || m_TotalNumAccepted>m_TotalNumberRuns) {
		m_TotalNumAccepted = m_TotalNumberRuns;
		for (unsigned long i = 0; i<m_TotalNumberRuns; i++)
			SetAcceptedRunNo(i, true);

	}
	else {
		vector<unsigned long> accepted;
		accepted.resize(m_TotalNumberRuns);
		m_MultiFile.read((char*)&accepted[0], 4 * m_TotalNumberRuns);

		m_AcceptedRuns[m_AcceptedRuns.size()-1].clear();
		for (unsigned long i = 0; i<m_TotalNumberRuns; i++) {
			if (GetAcceptedRunNo(i)) {
				m_AcceptedRuns[m_AcceptedRuns.size() - 1].push_back(i);
			}
		}


		if (m_NumMulti>0) {
			m_MultiFile.read((char*)&m_ParMin[0], 4 * m_NumMulti);
			m_MultiFile.read((char*)&m_ParMax[0], 4 * m_NumMulti);
			m_MultiFile.read((char*)&m_ParValidMin[0], 4 * m_NumMulti);
			m_MultiFile.read((char*)&m_ParValidMax[0], 4 * m_NumMulti);
			nbyte = (unsigned long)(m_MultiFile.gcount());
			if (nbyte == 0) {
				m_pNewMap->m_MStorage.IsMinUpdated[0] = false;
				for (unsigned long i = 0; i<m_NumMulti; i++) {
					GetMinPar(i);
					GetMaxPar(i);
				//	GetMinValidPar(i);
				//	GetMaxValidPar(i);
				}
				m_pNewMap->m_MStorage.IsMinUpdated[0] = true;



			}
			else {
				// Read CoCorMatrix
				m_MultiFile.read((char*)&m_CoCorNumValid, 4);
				if (m_MultiFile.good()) {
					if (m_CoCorNumValid<0 || m_CoCorNumValid>10000)
						m_CoCorNumValid = 0;
					m_CoCorValid.resize(m_CoCorNumValid*m_CoCorNumValid);
					if(m_CoCorNumValid>0) m_MultiFile.read((char*)&m_CoCorValid[0], 4 * m_CoCorNumValid*m_CoCorNumValid);
					if (!m_MultiFile.good()) {
						m_CoCorNumValid = 0;
						m_CoCorValid.resize(0);
					}
				}
				else
					m_CoCorNumValid = 0;
				// Read Attatched File
				unsigned long ifiles;
				m_MultiFile.read((char*)&ifiles, 4);
				unsigned long ich;
				unsigned long iruns, ifirstno;
				if (ifiles>10 || ifiles<0) {
					ifiles = 0;
					m_IsOpen = true;
					m_IsReady = true;
					nbyte = 0;


				}


				if (m_MultiFile.good()) {
					if (m_AddedFiles.size()>0) {
						m_AddedFiles.clear();
					}
					m_AddedNumRuns.resize(ifiles);
					m_AddedFirstRunNo.resize(ifiles);
					m_MultiFile.read((char*)&m_OrigRuns, 4);
					m_MultiFile.read((char*)&m_OrigFirstRunNo, 4);
					for (unsigned long i = 0; i<ifiles; i++) {
						m_MultiFile.read((char*)&ich, 4);
						m_MultiFile.read((char*)&buf, ich);
						str = "";
						for (unsigned long ii = 0; ii<ich; ii++)
							str += buf[ii];
						m_AddedFiles.push_back(str);
						m_MultiFile.read((char*)&iruns, 4);
						m_AddedNumRuns[i] = iruns;
						m_MultiFile.read((char*)&ifirstno, 4);
						m_AddedFirstRunNo[i] = ifirstno;
					}
					// Section for Reading Low and High Limits to Mean Indicators
					bool InitMeanLimits = false;
					bool InitMeanSumLimits = false;
					bool NewFormat = true;
					if (m_pNewMap != nullptr) {
						if (m_pNewMap->m_FileVersionNumberRead <= 48) NewFormat = false;

					}
					if (NewFormat) {
						m_MultiFile.read((char*)&m_MeanTSV_STAT[SELECT_LOW][0], 28);
						if (!m_MultiFile.good()) InitMeanLimits = true;
						m_MultiFile.read((char*)&m_ValidHigh[0], 28);
						if (!m_MultiFile.good()) InitMeanLimits = true;
						if (m_NumValidVar>0) {
							m_MultiFile.read((char*)&m_ValidDynLogLi[0], m_NumValidVar * 13 * 7 * 2 * 4);
							m_MultiFile.read((char*)&m_ValidDynResiduals[0], m_NumValidVar * 13 * 7 * 2 * 4);
						}
						m_MultiFile.read((char*)&m_ValidSumLow[0], 12);
						m_MultiFile.read((char*)&m_ValidSumHigh[0], 12);
						if (!m_MultiFile.good()) InitMeanSumLimits = true;


					}
					else {
						m_MultiFile.read((char*)&m_MeanTSV_STAT[SELECT_LOW][0], 24);
						if (!m_MultiFile.good()) InitMeanLimits = true;
						m_MultiFile.read((char*)&m_ValidHigh[0], 24);
						if (!m_MultiFile.good()) InitMeanLimits = true;


					}

					// Update Criterias
					/*for (unsigned long split_ind = 0; split_ind <= m_NumSplitPeriod; split_ind++) {
						for (unsigned long i = 0; i<m_NumValid + m_NumExtraValid; i++) {
							SetCriteria(i, 0, split_ind);

						}
					}*/

					// Update Mean and CV for accepted


				}
				else {
					unsigned long ipos = m_FileName.find(".MBin");
					unsigned long ifirst;
					if (ipos>0) {
						str = m_FileName.substr(ipos - 5, ipos - 1);
						ifirst = FUtil::AtoInt(str);
					}
					else
						ifirst = 0;
					m_OrigFirstRunNo = ifirst;
					m_OrigRuns = m_TotalNumberRuns;
				}



			}
		}
	}



	return true;
}
size_t MultiStorageData::Get_And_UpdateTotalNumberofRuns() {

	m_TotalNumberRuns = m_MBinHeader.n_runs;
	if (m_AddedFiles.size() > 0) m_TotalNumberRuns += GetNumberOfAddedRuns(0);
	for (size_t i = 1; i < m_AddedFiles.size(); i++) {
		m_AddedFirstRunNo[i] = m_AddedFirstRunNo[i - 1] + GetNumberOfAddedRuns(i - 1);
		m_TotalNumberRuns += GetNumberOfAddedRuns(i);
	}
	return m_TotalNumberRuns;
}
bool MultiStorageData::UpdateParDist(vector<DistStat> ParDist, size_t ensemble_index) {
	if (ensemble_index == string::npos) ensemble_index = m_ActualEnsemble;
	if (m_Valid_TSV.size() <= ensemble_index) return false;
	if (ParDist.size() == m_Valid_TSV[ensemble_index].ParDist.size())
		m_Valid_TSV[ensemble_index].ParDist = ParDist;
	else if (ParDist.size() < m_Valid_TSV[ensemble_index].ParDist.size()) {
		for (size_t i = 0; i <ParDist.size(); i++) {
			m_Valid_TSV[ensemble_index].ParDist[i] = ParDist[i];
		}

	}
	else
		return false;

	return true;



}
DistStat MultiStorageData::GetParDist(size_t index, size_t ensemble) {
	if (ensemble == string::npos)
		return m_Valid_TSV[m_ActualEnsemble].ParDist[index];
	else
		return m_Valid_TSV[ensemble].ParDist[index];
}

bool MultiStorageData::GetMultiPGFileForCorrectRunNo(size_t runno, CPG *pPG)
{
	bool NewOpen = false;

	auto FirstRunNo = m_pNewMap->m_DocFile.m_SimulationRunNo - m_pNewMap->MR_Get_ActualRunNumber();

	size_t index = 0;
	if (runno < m_pNewMap->MR_Get_NumberofRuns()) {

		return false;
	}
	else {
		while ((FirstRunNo + runno+1)>=(GetFirstRunNoAddedRuns(index)+ m_pNewMap->MR_Get_NumberofRuns()) &&index < m_AddedFiles.size()) {
			index++;
		}
		string filename=m_AddedFiles[index];
		size_t lp = filename.find("_Multi_");
		size_t lext= filename.rfind(".MBin");
		string numname;
		if (lp != string::npos) {
			numname = filename.substr(lp + 7, lext - lp - 7);		
		}	
		string dir=m_pNewMap->GetMultiSubDir();
		string newfilename = dir+"Multi_Coup_";
		newfilename += numname;
		newfilename += ".Bin";
		if (pPG->GetFileName() != newfilename) {
			pPG->CloseFile();
			if (pPG->Open(newfilename))
				return true;
			else
				return false;
		}
		return false;
	}
}
size_t MultiStorageData::ChangeEnsembleNo(size_t change)
{
	if (m_AcceptedIdString.size() > 1) {
		if(change==string::npos)
			m_ActualEnsemble--;
		else
		    m_ActualEnsemble ++;
		m_ActualEnsemble = m_ActualEnsemble % m_AcceptedIdString.size();
	}
	return m_ActualEnsemble;
}
size_t MultiStorageData::ReduceCriteria(PERFORMANCE_VAR Indicator, size_t VarIndex, size_t ThresholdType, size_t Threshold) {
#ifndef COUPSTD
	MFC_Util m;

	m.SetPaneMessage("Wait when posterior ensemble is reduced to " + FUtil::STD_ItoAscii(Threshold) + " runs");
#endif
	vector<float> dist, distout;
	bool RelativeThreshold{ false };
	if (ThresholdType > 0) RelativeThreshold = true; // Theshold = reduced percentage

	bool MeanOfMany = false;
	size_t MaxNum = m_pNewMap->GetNumAll_TSV_ValVariables();
	int ip;
	if (Indicator == ZERO_ADJUST) {
		ip = VarIndex+ABSME_STAT*MaxNum;
		Threshold = 10000;
	}
	else
		ip = VarIndex + Indicator*MaxNum ;

	if (VarIndex == MaxNum) MeanOfMany = true;
	if (ip == string::npos) return false;

	vector<pair<float, size_t>> dist_pair;
	if (MeanOfMany) {
		dist = m_pNewMap->m_MStorage.m_MeanTSV_Accepted[Indicator];
	}
	else {
		dist_pair = m_pNewMap->m_MStorage.GetPost_TSV_ValidPerformanceIndicatorPairVector(ip, 0);
	}
	int icount = dist_pair.size();
	if (icount <= 1) return icount;
	if (RelativeThreshold)
		Threshold = Threshold * icount / 100;
	if (Threshold <= 0) return icount;


	if (m_pNewMap->m_MStorage.m_ActualEnsemble == 2 && ip == 99) {

		auto koll = dist_pair.size();
	}
	float last_valid = 0.f, first_valid;
	//StatUtil::SortArray(&dist[0], icount);
		auto min_value = dist_pair.front().first;
		auto max_value = dist_pair.back().first;
		size_t n_toremove = 0;
		if (dist_pair.size() <= Threshold) {
			n_toremove = 0; Threshold = 1;}
		else if (Indicator != ABSME_STAT) n_toremove = dist_pair.size() - Threshold - 1;

		if (Indicator == ABSME_STAT || Indicator == A1_STAT) {
			float meanvaluein, meanvalueout;
			meanvaluein = StatUtil::mean(dist);
			size_t npos = 0;
			float goal = 0;
			if (Indicator == A1_STAT) goal = 1;
			size_t i = 0;
			while (dist[i] < goal && i < dist.size() - 1) {
				i++;
			};
			npos = i;
			if (i == 0) { //No candidates below goal
				for (size_t i = 0; i < Threshold; i++)	distout.push_back(dist[i]);
			}
			else {
				distout.push_back(dist[npos]);
			}
			meanvalueout = StatUtil::mean(distout);
			size_t nposlow, nposhigh;
			nposlow = nposhigh = i;
			bool endlow, endhigh, endloop;
			endlow = endhigh = false;
			endloop = false;

			while ((distout.size() < Threshold && !endloop)) {
				while (meanvalueout > goal && distout.size() < Threshold && !endlow&&nposlow>0) {
					nposlow--;
					if (nposlow >= 0)distout.push_back(dist[nposlow]);
					else endlow = true;
					if (distout.size() >= Threshold) {
						endlow = true;
					}
					meanvalueout = StatUtil::mean(distout);

				}
				while ((meanvalueout < goal && distout.size() < Threshold && !endhigh) && !endloop) {
					nposhigh++;
					auto prevmeanvalueout = meanvalueout;
					if (nposhigh < dist.size()) distout.push_back(dist[nposhigh]);
					else {
						endhigh = true;

						if (distout.size() < Threshold)
							distout.push_back(dist[nposhigh - 1]);
					}
					meanvalueout = StatUtil::mean(distout);
					if (prevmeanvalueout == meanvalueout) endloop = true;
				}
				if (endlow && endhigh) endloop = true;
			}
			StatUtil::SortArray(&distout[0], distout.size());
			dist = distout;
			first_valid = distout.front();
			last_valid = distout.back();
			if (first_valid < last_valid) {


			}
			else if (first_valid == last_valid) {
				last_valid = last_valid + abs(last_valid) / 1.E4;
			}
		}
		else if (Indicator == R2_STAT || Indicator == NSE_STAT || Indicator == LOGLI_STAT) {
			auto pos = dist_pair.size() - Threshold;
			if (pos >= 0)
				last_valid = dist_pair[pos].first;
			else
				last_valid = dist_pair.back().first + 0.0001;

		}
		else if (Indicator == RMSE_STAT) {
			last_valid = dist[Threshold - 1];

		}
		else if (Indicator == ZERO_ADJUST) {
			float meanvalue;
			meanvalue = StatUtil::mean(dist);
			if (meanvalue > 0) {
				while (meanvalue > 0 && dist.size() > 3) {
					dist.erase(dist.end() - 1);
					meanvalue = StatUtil::mean(dist);
				}
			}
			else {
				while (meanvalue < 0 && dist.size()>3) {
					dist.erase(dist.begin());
					meanvalue = StatUtil::mean(dist);
				}
			}
			Indicator = ABSME_STAT;
			first_valid = dist.front();
			last_valid = dist.back();
			Threshold = 1;
			n_toremove = 0;

		}


		if (dist_pair.size() > Threshold - 1) {
			if (MeanOfMany) {
				m_pNewMap->m_MStorage.m_MeanTSV_STAT[SELECT_LOW][LOGLI_STAT] = last_valid;
				m_pNewMap->m_MStorage.SetMeanCriteria(LOGLI_STAT, CRITERIA_STATE::ABOVE_MIN);
			}
			else {
				m_pNewMap->m_MStorage.SetCriteriaHistory(dist_pair.size() - n_toremove, Indicator);
				if (Indicator == R2_STAT || Indicator == NSE_STAT || Indicator == LOGLI_STAT) {
					//	m_pNewMap->m_MStorage.SetValidLow(ip, 0, min_value);
					m_pNewMap->m_MStorage.SetValidHigh(ip, 0, max_value);
					m_pNewMap->m_MStorage.SetValidLow(ip, 0, last_valid);
					if (m_pNewMap->m_MStorage.GetCriteria(ip, 0) == CRITERIA_STATE::NO_SET)
						m_pNewMap->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_MIN);
					else
						m_pNewMap->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_MIN);
				}
				else if (Indicator == RMSE_STAT) {
					//m_pNewMap->m_MStorage.SetValidLow(ip, 0, min_value);
					//m_pNewMap->m_MStorage.SetValidHigh(ip, 0, max_value);

					m_pNewMap->m_MStorage.SetValidHigh(ip, 0, last_valid);
					if (m_pNewMap->m_MStorage.GetCriteria(ip, 0) == CRITERIA_STATE::NO_SET)
						m_pNewMap->m_MStorage.SetCriteria(ip, CRITERIA_STATE::BELOW_MAX);
					else
						m_pNewMap->m_MStorage.SetCriteria(ip, CRITERIA_STATE::BELOW_MAX);
				}
				else if (Indicator == ABSME_STAT || Indicator == A1_STAT) {
					m_pNewMap->m_MStorage.SetValidLow(ip, 0, min_value);
					m_pNewMap->m_MStorage.SetValidHigh(ip, 0, max_value);


					m_pNewMap->m_MStorage.SetValidLow(ip, 0, first_valid);
					if (m_pNewMap->m_MStorage.GetCriteria(ip, 0) == CRITERIA_STATE::NO_SET)
						m_pNewMap->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_MIN);
					else
						m_pNewMap->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_AND_BELOW);

					m_pNewMap->m_MStorage.SetValidHigh(ip, 0, last_valid);
					m_pNewMap->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_AND_BELOW);
				}
				else if (Indicator == ZERO_ADJUST) {
					m_pNewMap->m_MStorage.SetValidLow(ip, 0, min_value);
					m_pNewMap->m_MStorage.SetValidHigh(ip, 0, max_value);
					m_pNewMap->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_AND_BELOW);

				}
			}
			auto pos = dist_pair.size() - Threshold;

			// Remove from Accepted Runs and Set RunNo to be false:

			for (size_t i = 0; i <= n_toremove; i++) {
				auto runno = dist_pair.front().second;
				m_pNewMap->m_MStorage.SetAcceptedRunNo(runno, false);
				dist_pair.erase(dist_pair.begin());
			}
			auto dist_pairprev = m_pNewMap->m_MStorage.GetPost_TSV_ValidPerformanceIndicatorPairVector(ip, 0);

			m_TotalNumAccepted = m_pNewMap->m_MStorage.FindAcceptedRunsFromFlagInPar();
			if (ip == 99 && m_pNewMap->m_MStorage.m_ActualEnsemble == 2) {

				auto koll = dist_pair.size();
			}
			ReduceUpdateStep(Indicator);

		}
		else {
			return string::npos;
		}
	return dist_pair.size();

}
void MultiStorageData::ReduceUpdateStep(PERFORMANCE_VAR value)
{
	m_pNewMap->m_MStorage.m_SV_StatMemory.AcceptedRuns = false;
	//m_pNewMap->m_MStorage.UpdateAcceptedRuns(false, false);
	//m_Changed = TRUE;
	//m_SavedCriteria2File = false;
	m_pNewMap->MR_ReCalculatePostDist(false);

	for (int i = 0; i < MAXSIMVAL; i++) {
		CResiduals* pRes;
		pRes = m_pNewMap->GetValidResidualPointer(i + 1);
		if (pRes != nullptr) {
			pRes->ToBeUpdated();
			pRes->m_MeanDefined = false;
		}
	}

	//m_Changed = FALSE;
	//m_Updated = TRUE;
}