#pragma once
#include "../ModelTypes/NewModelType.h"
const size_t NUM_STAT{7};
class MR;
struct STAT_PERF;
struct MBIN_FILE_HEADER {
	unsigned long n_runs;
	unsigned long  n_param;
	unsigned long n_valid;
	unsigned long n_sumvalid;
	unsigned long n_sumvar;
	unsigned long n_validStatBase;
	bool streamgood;
};
enum STAT_ENSEMBLE { E_PRIOR_MIN, E_PRIOR_MAX, E_PRIOR_CV, E_PRIOR_MEAN, E_POST_MIN, E_POST_MAX, E_POST_CV, E_POST_MEAN, E_SELECT_LOW, E_SELECT_HIGH };
enum CRITERIA_STATE {NO_SET, ABOVE_MIN, BELOW_MAX, ABOVE_AND_BELOW};
enum PERFORMANCE_VAR { R2_STAT, AO_STAT, A1_STAT, ABSME_STAT, RMSE_STAT, LOGLI_STAT, NSE_STAT, ZERO_ADJUST };
struct DefinedCriteria {
	CRITERIA_STATE criteria;
	PERFORMANCE_VAR stat_indicator;
	float minvalue, maxvalue;
	size_t n_post;
	size_t VarIndex;
};
struct VALIDENSEMBLE {
	std::vector<float> v[10];
	std::vector<CRITERIA_STATE> criteria;
	std::vector<CRITERIA_STATE> meancriteria;
	std::vector<DistStat> ParDist;
	std::vector<float> copar;
	std::vector<DefinedCriteria> History;
	bool tobe_Plotted{ true };
	bool is_Updated;
	bool is_Copar_Updated;
	size_t VarTranformed{0};
	size_t Dim1_Par_Index{0};
	std::vector<float> MeanPerFomanceIndicators[7];
};

enum STAT_OF_PERFORMANCE {PRIOR_MIN, PRIOR_MAX, POST_MIN, POST_MAX, SELECT_LOW, SELECT_HIGH};

struct VARTRANSFORMED {
	size_t Transformed_index{0};
	size_t Dim1_index{0};
	std::vector<float> VarValidVector;
};
struct STAT_PERF;

struct STAT
{
	float mean;
	float cv;
	float min;
	float max;
};
struct VALSUM
{
	SIMB simb;
	unsigned int SumIndex;
	float ValueOpt;
	float ValueMin;
	float ValueMax;
};



class MultiStorageData 
{
public:
	MultiStorageData();
	virtual ~MultiStorageData();
	void SetCyclicNumbers(size_t CycleLength, size_t CycleStartPos = 0, size_t Dim1Value=0) { m_CyclicNumber = CycleLength; m_CyclicStartPos = CycleStartPos; m_Dim1Value = Dim1Value; };
	void UpdateCyclicNumbers(size_t ActualEnsemble, size_t Dim1_Value= std::string::npos, size_t Transform_Index= std::string::npos);

	
	size_t GetCyclicNumber() { return m_CyclicNumber; };
	bool ReadAllStat();
	bool ReadNewData(unsigned long InitRuns);
	bool ReadBaseData();
	MBIN_FILE_HEADER ReadStartData();
	bool ReadBaseDataFromAttachedFiles(std::fstream *filestream, size_t istart, size_t runs_to_read);
	MBIN_FILE_HEADER ReadStartDataFromAttachedFiles(std::fstream *filestream,size_t ifile);

	
	bool SaveBase(bool updatetest=false);
	bool SaveStat();
	bool ResetVarValidTransformed(size_t i) {
		if (m_VarValidTransformed.size() > i) { m_VarValidTransformed.clear(); return true; }
		else return false;
	}
	void ReOpenSplitFile();
	bool Export();
	bool IsFileMissing() {return m_IsMissing;};
	bool SaveRecord(unsigned long);
	bool SaveTempFile();
	bool SaveOrgFileSize(unsigned long newnumrun);
	std::string GetNewFileName();
	bool Close();
	bool Reset();
	
	bool Init(MR *pMR);
	bool InitData(std::string,unsigned long,unsigned long, unsigned long,unsigned long,unsigned long, MR *pMR);
	bool OpenFile(std::string="", bool SetFile=true);

	bool AddExistingFile(std::string);
	bool ReWriteExistingFile(unsigned long);
	bool AddMultiP(unsigned long i, unsigned long, float v);
	bool AddValid(unsigned long i,unsigned long,  float v);
	bool AddValidSum(unsigned long i, unsigned long, float v);
	bool AddSumV(unsigned long i, unsigned long, float);
	bool IsOpen();
	float GetMultiP(unsigned long, unsigned long);

	void AllocateValid_TSV(size_t i);
	void AllocateValid_SUM(size_t i);
	void AllocateMeanValidVar();

	float GetMinPar(unsigned long);
	float GetMaxPar(unsigned long);
	float GetValid(unsigned long record, unsigned long IndexValid, size_t cyclestart= std::string::npos, size_t cycle_length= std::string::npos);
	float GetSplitValid(unsigned long irun , unsigned long ival, unsigned long split=0);
	float GetSplitNValue(unsigned long iv, unsigned long split=0);
	float GetValidLow(unsigned long, unsigned long split=0);

	float GetValidHigh(unsigned long, unsigned long split=0);



	
	unsigned long  UpdateAccepted(unsigned long, unsigned long);

	
	bool  GetAcceptedRunNo(unsigned long);
	void  SetAcceptedRunNo(unsigned long, bool value);
	bool RemoveAcceptedRunNo(unsigned long);

	void  ReAssignPerformanceandAcceptanceFromAggregation(size_t aggregate_num);
	void  CommonPerformanceFromAggregation(size_t aggregate_num);
	void  SetCoCorValid(unsigned long, unsigned long, float, size_t actensemble= std::string::npos);
	float GetCoCorValid(unsigned long, unsigned long, size_t actensemble = std::string::npos);
	void  SetCoCorValid_Init(unsigned long);
	bool  IsCoCorValidSet();
	void SetCoParUpdated(bool updated);

	size_t Get_ActualFilterVarValue(size_t RunNo);

	size_t   m_CoCorNumValid;
	std::vector<float> m_CoCorValid;








	float GetSumV(unsigned long, unsigned long);
	float GetSumV_Selected(unsigned long, unsigned long);
	unsigned long GetNumAccepted();
	unsigned long GetNumBayesAccepted(unsigned long num_burn_in);




	//bool IsMinUpdated, IsMaxUpdated, IsCVUpdated, IsMeanUpdated, IsCVSumUpdated, IsMeanSumUpdated, IsMeanSumUpdatedAll;
	//bool IsCVSumUpdatedAll,IsMinSumUpdatedAll, IsMaxSumUpdatedAll;

	bool SelSumV_Index_Add(unsigned long);
	void SelSumV_Index_Reset();
	unsigned long  SelSumV_Index_Get(unsigned long);
	unsigned long  SelSumV_Index_GetNum();
	bool  SelSumV_Index_SelectAll();
	unsigned long GetNumberOfAddedFiles(std::string str="");
	std::string GetNameAddedFiles(unsigned long);
	std::string GetNameBaseFile();
	unsigned long GetNumberOfAddedRuns(unsigned long);
	unsigned long GetAdd_MultiPGFileIndex(unsigned long simRunNo);
	unsigned long GetFirstRunNoAddedRuns(unsigned long);
	unsigned long GetNumberOfRuns();
	unsigned long GetNumberOfTotalRunsWithAttachedFiles(); 
	unsigned long GetSumCriteria(unsigned long);
	bool SetSumCriteria(unsigned long, unsigned long value);
	void ResetSumCriteria();
	
	std::string GetSplitFileName();
	std::string GetStatFileName(bool NewName=false);
	void AlloSplitPerformance(unsigned long splitperiods);
	void AlloMinMaxUpdated(size_t NumOfEnsembles);
	void SetPerformance(unsigned long irun,  unsigned long ind_split, STAT_PERF stat);
	void SetNewPerformance(unsigned long irun, unsigned long ivar, STAT_PERF stat);
	unsigned long GetNumSplitPeriods();
	void VarValidAllo();
	bool AllocateSpaceFromBaseVariables( size_t numVal);
	bool SaveSplitData(unsigned long ind_val);
	bool OpenAndCheckIfSplitPeriod();
	bool ReadAcceptedRunInfo();
	size_t Get_And_UpdateTotalNumberofRuns();
	void SetToBePlotted(size_t i, bool value=true) { if (m_Valid_TSV.size() > i) m_Valid_TSV[i].tobe_Plotted = value; };
	bool IsToBePlotted(size_t i) { if (m_Valid_TSV.size() <= i) return false; return m_Valid_TSV[i].tobe_Plotted; };
	bool UpdateParDist(std::vector<DistStat> ParDist, size_t ensemble= std::string::npos);
	DistStat GetParDist(size_t index, size_t ensemble = std::string::npos);
	bool  GetMultiPGFileForCorrectRunNo(size_t runno, CPG *pPG);
	size_t ChangeEnsembleNo(size_t change);
	std::string GetTransformationIndicator(size_t i) {
		std::string txt;
		if (m_Valid_TSV.size() > i) {
			switch (m_Valid_TSV[i].VarTranformed%10) {
				case 0:	txt = "Normal"; break;
				case 1:	txt = "Cumulated"; break;
				case 2:	txt = "Year Cumulated"; break;
				case 3: txt = "Day Cumulated"; break;
				case 4: txt = "Mean Dimension 1"; break;
			}
			if (m_Valid_TSV[i].VarTranformed > 4) {
				txt+= " - ("+FUtil::STD_ItoAscii(m_Valid_TSV[i].Dim1_Par_Index)+")";
			}
		}
		return txt;
	};

	size_t ReduceCriteria(PERFORMANCE_VAR, size_t VarIndex=0, size_t ThresholdType=0, size_t Threshold=0);
	void ReduceUpdateStep(PERFORMANCE_VAR);
	size_t GetNumberofEnsembles() { return m_Valid_TSV.size(); };
	std::vector<size_t> GetEnsemblesToPlot() {
		std::vector<size_t> out;
		for (size_t i = 0; i < m_Valid_TSV.size(); i++) {
			if(m_Valid_TSV[i].tobe_Plotted)
				out.push_back(i);
		}
		return out;
	};
	size_t GetNumEnsemblesToPlot() {
		size_t num = 0;
		for (size_t i = 0; i < m_Valid_TSV.size(); i++) {
			if (m_Valid_TSV[i].tobe_Plotted)
				num++;
		}
		return num;
	};
	unsigned long m_NumMulti;
	unsigned long m_NumMultiAdd;

public:
	bool m_SplitFileOpen;
	bool m_Lock;
	bool m_New7_Stat;
	std::vector< std::vector<size_t> > m_AcceptedRuns;
	std::vector<std::string> m_AcceptedIdString;

	bool m_AcceptedUpdated;
	bool CalculateMeanValidValues(bool SV_Variables=false);

	std::vector<float> m_ValidSumLow,m_ValidSumHigh;			// Represent mean of SV variables
	std::vector<float> m_ValidSumMin,m_ValidSumMax;			// index 0-2, reprsent the various performance indicators	
	std::vector<float> m_ValidSumMinAll,m_ValidSumMaxAll;

	std::vector< std::vector<float>> m_MeanSV_STAT, m_MeanTSV_STAT;
	std::vector< std::vector<float>> m_MeanSV_All, m_MeanSV_Accepted;
	std::vector< std::vector<float>> m_MeanTSV_All, m_MeanTSV_Accepted;



	// 0-Mean, 1- RMSE, 2-LogLi
	std::fstream m_MultiFile, m_AddFile, m_SplitPeriodFile;
	//fstream m_EvaluationFile;
	bool m_IsSplitOpen;
	size_t m_CumulatedPerformance;
	std::map<size_t, size_t> MapOfStoredTSV; // key cumulated value, value är index in m_VarValidTransformed;
	size_t m_ActualEnsemble;
	unsigned long m_NumSum, m_NumValid, m_NumSumValid, m_TotalNumAccepted, m_TotalNumBayesAccepted;
	unsigned long m_TotalNumberRuns;
	unsigned long m_ValidStatBase{ 7 };

	std::vector<VALIDENSEMBLE> m_Valid_TSV, m_Valid_SV;
	std::vector<VALIDENSEMBLE> m_Valid_TSV_Mean, m_Valid_SV_Mean;

	std::vector<VALSUM> m_ValSumValues_Stored;
	std::vector<STAT> m_PriorSumVar, m_PostSumVar;
	std::vector<size_t> m_AddedFirstRunNo;
	std::vector<std::string> m_AddedFiles;
	
protected:
	size_t m_CyclicNumber, m_CyclicStartPos, m_Dim1Value;
	
	unsigned long m_NumValidVar;
	unsigned long m_runs;
	unsigned long m_OrigRuns, m_OrigFirstRunNo;
	
	unsigned long m_NumSplitPeriod;
	unsigned long m_runsAdd;
	unsigned long m_NumSumAdd, m_NumValidAdd, m_NumSumValidAdd;
	unsigned long m_NumExtraValid; // To represent NSE and may be additional new index of performance
	unsigned long m_NumValidAll;  // The number of TSV validation performance indicators

	MBIN_FILE_HEADER m_MBinHeader;
	std::streampos m_HeaderStart, m_HeaderEnd;

	std::vector<float> m_ValidLow, m_ValidHigh;
	std::vector<float> m_ValidMin, m_ValidMax,m_ValidMinAll, m_ValidMaxAll;

//	float m_ValidLow[7],m_ValidHigh[7];	
	// Represent mean of TSV variables
	//float m_ValidMin[7],m_ValidMax[7];			// index 0-6, reprsent the various performance indicators	

	std::streampos m_SplitRec,m_SplitRecRuns, m_SplitPos;

	bool m_IsOpen, m_IsReady, m_IsMissing;

	//CFloatArray m_VarSum[4000], m_VarMulti[100], m_VarValid[100], m_VarValidSum[50];
	//CUIntArray m_RunNo, m_Accepted;
	std::string m_FileName;
	std::vector<size_t> m_AddedNumRuns;

	char filestr;
	std::vector<unsigned long> m_RunNo, m_Accepted;


	std::vector< VARTRANSFORMED > m_VarValidTransformed;// , m_VarSumValidAdded;
	std::vector<float> m_VarValidSplit;
	std::vector <float> m_VarSplit;

	std::vector<float> m_VarSumStart, m_VarMultiStart, m_VarValidStart, m_VarValidSumStart;


	std::vector<float> m_VarValidMin,m_VarValidMax;
	std::vector<float> m_VarValidMean,m_VarValidMeanAll;
	std::vector<float> m_VarValidCV,m_VarValidCVAll;
	std::vector<float> m_VarValidMinSum,m_VarValidMaxSum;
	std::vector<float> m_VarValidMinAll,m_VarValidMaxAll;


	std::vector<float> m_VarValidMeanSum,m_VarValidMeanSumAll;
	std::vector<float> m_VarValidMinSumAll,m_VarValidMaxSumAll;
	std::vector<float> m_VarValidCVSum,m_VarValidCVSumAll;


	std::vector<float> m_ParValidMin,m_ParValidMax, m_ParMin,m_ParMax;;


	std::vector<float> m_VarValidMean_R2,m_VarValidMean_A0, m_VarValidMean_A1;
	std::vector<float> m_VarValidMean_AbsME,m_VarValidMean_RMSE;  // Mean of TSV Variables
	std::vector<float> m_VarValidMean_NSE, m_VarValidSum_LogLi;

	

	std::vector<size_t> m_VarValidCriteria, m_VarValidSumCriteria;;
	std::vector<size_t> m_VarValidMeanCriteria;
	std::vector<size_t> m_VarValidMeanSumCriteria;
	std::vector<float> m_ValidDynLogLi,m_ValidDynResiduals;

	std::vector<unsigned long> m_VarSumSel, m_VarMultiSel, m_VarValidSel, m_VarValidSumSel;
	public:
		std::vector< std::vector<float>> m_VarTSV_Mean, m_VarSV_Mean;
		MR *m_pNewMap;
		std::vector<float> m_VarSum, m_VarMulti, m_VarValid, m_VarValidSum;
		std::vector<float> m_VarValidHigh, m_VarValidSumLow, m_VarValidSumHigh;

		std::vector<float> m_VarValidSumMean_AbsME, m_VarValidSumMean_RMSE;  // Mean of Sum Variables

};


