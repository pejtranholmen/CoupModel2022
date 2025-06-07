#pragma once
class SimB;
class Sw;
class F;
class CDB;

#include "../MultiRun/Residuals.h"
#include "../MultiRun/SelectedEnsembles.h"
#include "../ModelTypes/NewModelType.h"
#include "./ModelMap.h"
#include "../PG/PG.H"
#include "../DB/DB_Storage.h"
enum class SIMSUMSOURCE { MULTIFILESTAT, CSVFLEXFILE, MULTI_OUTPGFILE };
enum SUM_PERFORMANCE { SUM_R2, SUM_INTERCEPT, SUM_SLOPE, SUM_ME, SUM_RMSE, SUM_LOGLI, SUM_OBS, SUM_SIM, SUM_NSE, SUM_NOVALUE };

struct CONSTRAIN_BY {
	size_t IndepMultiPar;
	double MinRangeValue, MaxRangeValue;
};
class MRv{
public:
	size_t NumberOfRepeatitionsWithinDimension;
	size_t nCount;
};

class VALv{
public:
	size_t	ValidationFileIndex;  // index in the validation pG-file for the specific variable
	size_t	OutputType;
	std::string	Group;
	std::string	Name;
	size_t	LocalIndex;
	size_t	nNumber;
	float   R2;
	float	A0;
	float   A1;
	float   ME;
	float   RMSE;
	float   MeanSim;
	float   MeanVal;
	float	P_Error;
	float	A_Error;
	double	LogLi;
	bool	AccTest;
	size_t	ValidationFileNumber;
	bool	LogTrans;
	size_t	ValidationFileResultIndex;
	size_t	Duration;
	size_t	Monitoring;
	float   NSE;
	float	P_NewError;
	float	A_NewError;
	float	NewBoxCox_Power;
	float	NewBoxCox_Offset;
	int		PG_FilterIndex=0;
	int		PG_FilterValue=0;
	int		PG_QualityIndex=0;
	float	Quality_Threshold=0;
	std::vector<int> Sum_of_Others;
};

class VALSUMv
{
public:
	size_t	SumVarType=0; // 0 = Original, 1- Obs Scaled, 2- Sim Scaled
	enum  SIMSUMSOURCE  SimValueSource=SIMSUMSOURCE::MULTI_OUTPGFILE; // 0 = Sim Sum Values, 1 - Residuals from other Sum Variables, 2 - Multi Bin file
	bool m_xmlFileToUse{ false };
	size_t  Sim_Value_Residual_Index=0;
	simtype	OutputType=simtype::PAR;
	unsigned int  MultiBin_StartMin=0, MultiBin_EndMin=0;
	std::string	Group;
	std::string	Name;
	size_t	LocalIndex=0;
	std::vector<float>   ME;
	std::vector<float>   RMSE;
	std::vector<float> R2_Multi;
	std::vector<float> InterCept_Multi;
	std::vector<float> Slope_Multi;
	std::vector<float>	LogLi;
	std::vector<float> NSE;
	float ME_mean=0.f;
	float RMSE_mean=0.f;
	float R2_Multi_mean=1.f;
	float InterCept_Multi_mean=0.f;
	float Slope_Multi_mean=1.f;
	float NSE_mean=0.f;
	double LogLi_mean=0.;
	int  n_RegMulti=0;
	float   SimValue=MISSING;
	float   ObsValue=MISSING;
	float	P_Error =0.1f;
	float	A_Error = 0.001f;
	bool	Monitoring=false;
	bool	FlexSumVar=false;
	std::string  IDVarName;
	std::string  IDFileName;
	double  ObsScalingFactor= 1.;
	size_t	ObsScaling=0;
	std::vector<float> ObsValueVector;
	std::vector<float> SimValueVector;
 };
struct VALSUMLINK {
	std::string FileName;
	std::vector<std::string> VarName;
	std::string IDVarKey;
	size_t MultiSimNumKey;
};
struct MULTI_PARFILES {
	std::string FileName;
	std::vector<std::string> ParNames;
	std::string FileId;
};
struct DocFile2 {
	std::string m_OriginFileName;
	std::string m_ExeFileDate;
	std::string m_DateRun;
	std::string m_Comments;
	std::string m_RunId;
	std::string m_Multi_MBin_File;
	std::vector<std::string> m_Multi_SBin_FileNames;
};

struct InformationCriteria {
	double SSE;
	size_t n_points;
	size_t n_parameters;
	double BIC;
	double AIC;
	size_t n_aggregateNumber;
};



struct DocFile {
	bool m_Running;
	bool m_MultiRunning;
	bool m_FinishedSimulation;
	bool m_MultiStoragelocked;
	size_t m_FileVersionNumber;
	size_t m_SimulationRunNo;
	size_t m_TimePeriodScaleFactor;
	size_t ipredays;
	size_t ipostdays;
	time_t m_TimeCreated;
	time_t m_TimeModified;
	float m_SimTime;
	float m_AccSimTime;
	float m_TsVLogLi;
	float m_TsVLogLiSum;


};

 
class RUNINFO {
public:
	std::string Group;
	std::string Name;
	size_t Format;  // 0=Int, 1=Date, 2=String
	CRunInfo* pBase;
};
class Annim {
	public:

	SimB* pBase;
	size_t type;
	size_t LocalIndex;
	size_t iChart;
};
struct HistP
	{
	   SimB *pBase;
	   size_t type;
	   size_t LocalIndex;
	};
class HISTv
{
public :
	SimB* pBase;
	size_t RunInfoType;	// SimB: 0= Run Info, 1= DB object;
	size_t	LocalIndex;
	time_t	tvalue;
	float	value;
	std::string strvalue;
	std::string User;
	std::string Computer;
	size_t	RunNo;
};
class ModelBase : public ModelMap{
	
	public:
	ModelBase();	
	~ModelBase();

	DocFile m_DocFile;
	DocFile2 m_DocFile2;
	bool m_xmlFileToUse{ false };
	InformationCriteria m_InformCriteria;
	size_t m_FileVersionNumberRead;
	std::vector<MRv> m_MultiRun_Array;
	std::vector<VALv> m_Val_Array;
	std::vector<size_t> m_ValGroupIndex[NUMGROUP];
	std::vector<size_t> m_ValTypeIndex[4];

	std::vector<VALSUMv> m_ValSum_Array;
	VALSUMv GetValSumStruct(size_t index) {
		if (m_ValSum_Array.size() > index) return m_ValSum_Array[index]; else
		{
			VALSUMv out; return out;
		};
	};
	std::vector<VALSUMLINK> m_ValsumFlexFiles;
	std::vector<MULTI_PARFILES> m_MultiParFiles; // CSV files to goven change of MultiRuns during run

	std::vector<SIMB>m_X_Array;
	std::vector<SIMB>m_T_Array;
	std::vector<SIMB>m_G_Array;
	std::vector<SIMB>m_D_Array;
	std::vector<CDB*> m_DB_Array;
	std::vector<std::string> DB_Names;
	std::vector<time_t> m_DB_ImportTimeT;

	std::vector<bool> DB_Modified;
	std::vector<RUNINFO> m_RunInfo;
	std::vector<HISTv> m_History_Array;
	std::vector<HistP> m_HistoryPar_Array;
	std::vector<std::string>m_Report_Array;
	std::vector<Annim>  AnnimVarList;
	std::vector< std::vector<float>> m_ValFlexValue[4];
	size_t m_AnnimChartX, m_AnnimChartY;
	

	SimB* GetPtrFromIntType(size_t itype, std::string group, std::string name);
	SimB* GetPtr(std::string type, std::string group, std::string name);
	SimB* GetPtr(std::string type, std::string name);
	SimB* GetPtr(enum simtype, std::string name, std::string group="");
	SimB* GetPtrByName(std::string name, std::string group="");
	P* GetP(std::string name)  {return (P*)GetPtr(simtype::PAR_TABLE, name);};
	Ps* GetPs(std::string name) {return (Ps*)GetPtr(simtype::PAR_SINGLE, name);};
	Sw* GetSw(std::string name) {return (Sw*)GetPtr(simtype::SWITCH,name);};
	size_t GetSwValue(std::string name) {return ((Sw*)GetPtr(simtype::SWITCH,name))->GetIntValue();};
	size_t SetSwValue(std::string name, size_t value) {return ((Sw*)GetPtr(simtype::SWITCH,name))->SetIntValue(value);};

	void* GetNE_Ptr(size_t* val);

	std::vector<SimB*> GetPtrVector(std::string type, std::string group, bool All=false);
	std::vector<SimB*> GetPtrVector(enum simtype,  bool All=false);
	std::vector<SimB*> GetPtrVector(enum simtype, size_t GroupNo,bool All);
	std::vector<SimB*> GetAllPtr(enum simtype);

	std::vector<SimB*> GetPtrVector(std::string type, size_t igroup=-1);
	std::vector<SimB*> GetPtrVector(size_t itype=100, size_t igroup=-1);
	std::vector<size_t> GetEnabledGroupNumbers(bool all=false);

	std::vector<SIMB> GetAllSelectedOutputs(bool MultiFlag=false);
	std::vector<SIMB> GetAllSelectedOutputsSortedByName(bool MultiFlag = false);
	std::vector<std::string> GetAllSelectedNameList(bool MultiFlag = false);
	void RemoveAllSelectedOutputs();
	size_t SelectAllOutputs();

	std::vector<SIMB> GetWShedOutputs();
	std::vector<size_t> GetPressureHeadOutputIndex(std::vector<SIMB> allout);
	std::vector<SIMB> GetAllOutputs(size_t choice= std::string::npos, bool allout=false);
	std::vector<SimB*> GetAllPtrOutputs(size_t choice= std::string::npos);
	std::vector<SIMB> GetALLXOutputs() {return GetAllOutputs(0);};
	std::vector<SIMB> GetALLTOutputs() {return GetAllOutputs(1);};
	std::vector<SIMB> GetALLGutputs() {return GetAllOutputs(2);};
	std::vector<SIMB> GetALLDOutputs() {return GetAllOutputs(3);};
	void SetEnabledCategories(std::vector<int> categories) { m_EnabledCategories = categories; };
	std::vector<int> GetEnabledCategories() { return m_EnabledCategories; };
	std::vector <bool>m_GroupEnabled;
	std::vector<int> m_EnabledCategories;
	void SetUpdatedGroupNumbers(std::vector<size_t> groups) {
		m_UpdatedGroupNoVector = groups;
		UpdateEnabledElementvector();
	};
	std::vector<size_t> GetUpdatedGroupNumbers() { return m_UpdatedGroupNoVector; };
	void UpdateEnabledElementvector();
	void UpdateEnabledProcvectors();

	void SetUpdatedElements(std::vector<elements> currentElements) {
		m_UpdatedElementsVector = currentElements;
		UpdateEnabledProcvectors();
	};
	void SetUpdatedFysProcVector(std::vector<fysprocess> currentfysproc) {
		m_UpdatedFysProcVector = currentfysproc;
	};
	void SetUpdatedBioProcVector(std::vector<bioprocess> currentbioproc) {
		m_UpdatedBioProcVector = currentbioproc;
	};



	std::vector<elements> GetUpdatedElementsVector() { return m_UpdatedElementsVector;};
	std::vector<elements> GetEnabledElementsVector() { return m_EnabledElementsVector;};
	std::vector<fysprocess> GetUpdatedPhysProcVector() { return m_UpdatedFysProcVector; };
	std::vector<fysprocess> GetEnabledPhysProcVector() { return m_EnabledFysProcVector; };
	std::vector<bioprocess> GetUpdatedBioProcVector() { return m_UpdatedBioProcVector; };
	std::vector<bioprocess> GetEnabledBioProcVector() { return m_EnabledBioProcVector; };


	bool IsFinishedSimulation();
	void SetFinishedSimulation();
	void SetNewSimulation();
	bool IsMultiSimulation();
	size_t		MR_GetNumberOfRepeationsForThisDimension(size_t index);
	std::string GetXBinFileName(size_t ValidIndex=0, size_t addfile=0);


	void SetMultiStorageBinFileMissing();
	bool IsMultiStorageBinFileValid();
	bool IsBayesianCalibration() {return m_IsMakingBayesianCalib;};

	void SetMultiSimulation();
	void SetRunning(bool value) {m_DocFile.m_Running=value;};
	bool IsRunning() {return m_DocFile.m_Running;};
	void SetSingleSimulation();
	void SetSimTime(double usedtime) {
		if(m_DocFile.m_SimTime<0) {
				m_DocFile.m_SimTime=float(usedtime);
				m_DocFile.m_AccSimTime=float(usedtime);
		}
		else {
			float prev=m_DocFile.m_AccSimTime;
			m_DocFile.m_AccSimTime=float(usedtime);
			m_DocFile.m_SimTime=m_DocFile.m_AccSimTime-prev;
		};};
	bool SetSimPeriodFromClimateFile();
	bool SetOutputFileNameToOutputFile(std::string filename);
	std::string GetWShedFileName_In();
	std::string GetWShedFileName_Out();
	std::string GetWShedFileName(size_t inum);

	std::string GetMBinFileName();
	std::string GetCurrentSimFile();
	std::string GetCurrentWorkDirectory();
	F* OutputFilePointer();
	std::string m_AppString;
	size_t	MR_Get_TotalNumberofRuns();
	size_t	MR_Get_NumberofRuns();
	size_t	MR_Get_ActualRunNumber();
	size_t		MR_GetDimCounter(size_t);
	size_t		MR_GetNumberOfDimensions();
	std::string GetMultiSubDir();
	
	size_t GetNumOutPuts();
	void Clear_XTGD();
	size_t GetSize_XTGD();
	size_t GetSize_XTGD_Total();
	std::vector<SIMB> GetOutVector(size_t itype);
	std::vector<SIMB> GetAllOutPutVectorByCategory(int category);
	void CleanValFlags();

	size_t m_NumberSelectedValidationVar[16];

	void DB_SetChoice(DB_types, std::string value);
	std::string DB_GetChoice(DB_types);
	std::string DB_GetNames(size_t, bool AllIndex=true);
	CDB* DB_GetPointer(DB_types);
	CDB* DB_GetPointer(std::string);
	void DB_SetImportTimeT(size_t index, time_t time);
	time_t DB_GetImportTimeT(size_t index);
	void SetRunReportArray(std::string);

	float GetActualObsValueFromValSumFlex(std::string IDFileName, std::string  IDVarName, size_t runno);
	void CheckFileNameLinkedto_SV_Variables();
	std::string GetExistingDocFileName(std::string FileName);


	bool m_Protect;
	bool m_ChildDocument;
	bool m_SavedSum;
	bool m_SavedTimeSerie;
	size_t m_LinkedDocNumber;
	bool m_HasBeenRunning;

	bool m_OutputValidXBinFileExist;
	bool m_OutputBinFileExist, m_OutputValidBinFileExist;
	
	bool m_IsMakingBayesianCalib; bool m_IsMakingNelderMeadCalib;
	
	size_t m_CountError;
	size_t m_MultiRun_TotalNumber;
	size_t m_NumOutPuts;
	size_t m_UserLevel;
	std::string m_DocFileName;
	std::string m_UserDirectory;
	std::string m_CurrentFile, m_CurrentDirectory, m_StartDirectory, m_CurrentSubDirectory;
	std::string m_DataBaseDirectory, m_PreviousMultiMBinFile;
	std::string m_Multi_XBin_File, m_Multi_Bin_FileName;

	Sw* m_pValidIndexSet;

	SelectedEnsembles m_MStorage;

	bool m_ReReadMStorageFile;

	CPG m_PG_OutPutFile, m_PG_MultiOutputFile, m_PG_WShed_In, m_PG_WShed_Out;

	size_t GetNumAll_TSV_ValVariables();
	size_t m_ErrorCount;

	std::vector<std::string> m_arrGroups;
	std::vector<std::string> m_PlantTypes;
	std::string m_DB_choices[10];
	DB_OldHeader m_DB_header[10];
	PFCurve* p_PFCurve;
	std::string GetTimeCreated();
	std::string GetTimeModified();
protected:
	bool m_MultiStorageBinFileExist;
	
	std::string m_CurrentSubDirectoryFile;
	std::vector<SIMB> SelectedOutVector(std::vector<SimB*> vp);
	std::vector<SIMB> CompleteOutVector(std::vector<SimB*> vp);

	std::vector<std::string> m_FailedNames;

	std::string CreateFlagString(SimB* currentParameter, size_t FlagValue=1);

private:
	std::vector<size_t> m_UpdatedGroupNoVector;
	std::vector<elements> m_UpdatedElementsVector;
	std::vector<elements> m_EnabledElementsVector;
	std::vector<fysprocess> m_UpdatedFysProcVector;
	std::vector<fysprocess> m_EnabledFysProcVector;
	std::vector<bioprocess> m_EnabledBioProcVector;
	std::vector<bioprocess> m_UpdatedBioProcVector;
};
