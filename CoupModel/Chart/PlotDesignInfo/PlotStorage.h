#pragma once

#include "../../NewBase/ModelBase.h"
#include "../../ModelTypes/SimB.h"
#include "../../PG/PG.H"
#ifndef COUPSTD
#include "../ChartProperties/ChartProp.h"
#endif
//enum vartype {NO_USE, DATEAXIS, MULTIRUN_NUM, PGVAR, SUMVAR, VALVAR, PARVAR, PGREPIT, vartype::PGYEAR, PGMONTH, vartype::PGDAY, vartype::PGHOUR, vartype::PGDEPTH, vartype::PGVARVALID, GEO_LAT, GEO_LONG, GEO_ALT, vartype::VAR_MIN, vartype::VAR_MAX, VAR_MISSING, VAR_FIRST, VAR_LAST, vartype::VAR_MEAN, VALVARSUM};

#ifndef INDEP_AXIS_SET

struct INDEP_AXIS
{
	enum vartype Dependent;
	enum vartype First;
	enum vartype Second;
};
#define INDEP_AXIS_SET 1
#endif

class Doc;
class ChartProp;
//enum CHART_TYPES;



// CPlotStorage command target
enum sumdatatypes { };


struct PLOTv
{
	SimB *pBase=nullptr;
	size_t TabIndex=0;
	CPG *pPG=nullptr;
	CPG *pPG_MeasValFile = nullptr;
	Doc *pSimDoc = nullptr;
	std::string PGFileName;
	std::string PGDocName;
	size_t DayValueRepresentation=0;// 0 - all, 1-n -specific modulus of day, n+1, mean of day
	simtype Type=simtype::PAR;	/// Data Types_previous
	bool RepitDataType=false;
    enum DATA_TYPES DataType=DATA_TYPES::ANYDATA; // New enum DataType to replace Type
	size_t Index=0;
	size_t index_cycle=0;
	size_t Chart=0;
	enum  SUM_PERFORMANCE Performance_Var=SUM_PERFORMANCE::SUM_R2;
	size_t ChartNumber=0;
	CHART_TYPES ChartType;
    enum ENSEMBLE_TYPE EnsembleType;
	size_t Validation=0;
	size_t ValFileNo=0;
	size_t PGFileIndex=0;
	size_t Doc=0;
	size_t Residual_VarNum=0;
	size_t Residual_RunNo=0;
	size_t Residual_Sequence = 0;
	size_t PGSection=0;
	size_t SubChart_No=0;
	size_t LogLiVersion=0;  // 0 Gaussian 1 Non Gaussian 2 forced Residual Only
    size_t PGRepitNo=0;	
	size_t EnsembleNo=0;// 3 forced  Measured Only
	INDEP_AXIS IndepAxis;
	int PG_FilterVarIndex=0;
	int PG_FilterVarValue=0;
	std::vector<CONSTRAIN_BY> m_ConstrainBy;
	size_t pg_start_pos{0};
};

class CPlotStorage
{
protected:
	size_t pg_docindex[MAXPGDOC][MAXSIMVAL];
	size_t m_SimulationRunNo;
	size_t m_FileVersionNumberRead;
	std::string m_PG_Unit,m_PG_Name;
	size_t m_NumberSavedCharts;
	size_t m_NumberSelectedValidationVar[MAXSIMVAL];
	
	bool m_ReReadMStorageFile;
	bool m_OutputBinFileExist, m_OutputValidBinFileExist,m_MultiStorageBinFileExist;
	ChartProp *m_pProp;

public:
	CPlotStorage();
	virtual ~CPlotStorage();
	bool Init(Doc *pDoc);
	void Update(size_t SimmNo, size_t NumberRead);
	size_t AddPlotInfo(PLOTv plotinfo,size_t chartnum);
	bool Add(void *pXG, size_t type, size_t index, size_t chartnum, CHART_TYPES charttype, std::string DocName="", size_t ValFileNo=0, size_t vnum=0, size_t runno=0, size_t section=0, size_t subchart=0, size_t logli_ver=0, size_t repit_no=1);
	bool Add(void *pXG, size_t type, size_t index, size_t chartnum, CHART_TYPES charttype, size_t iDoc, size_t ValFileNo=0, size_t vnum=0, size_t runno=0, size_t section=0, size_t subchart=0, size_t logli_ver=0, size_t repit_no=1);
	bool AddCurve(size_t add_section=0, size_t backshift=0);

	PLOTv GetPlotInfo(size_t index);
	bool Remove(SimB *Sw);
	bool Remove(size_t index);
	size_t  RemoveAllSectionCurves();
	bool RemoveChart(size_t index);
    bool SetEnsembleType(size_t, enum ENSEMBLE_TYPE ensemtype);
    bool SetDataType(size_t, enum DATA_TYPES datatype);
	bool SetChartType(size_t, CHART_TYPES Chart_type);
    enum ENSEMBLE_TYPE GetEnsembleType(size_t index);
    enum DATA_TYPES GetDataType(size_t index);
	bool GetRepitType(size_t index);
	bool RemoveAll();
	bool SetConstrainBy(size_t index, std::vector<CONSTRAIN_BY> con);
	size_t  NumSelected();
	//SimB* GetPtr(size_t index);
	void* GetPtr(size_t index);

	size_t GetValVarNo(size_t index);
	size_t GetValRunNo(size_t index);
	bool SetValRunNo(size_t index, size_t value_runo, size_t sequence_no=0);
	size_t GetPGSectionNo(size_t index);
	bool SetPGSectionNo(size_t index, size_t value);
	size_t GetPGRepitNo(size_t index);
	bool SetPGRepitNo(size_t index, size_t value);
	size_t GetLocalIndex(size_t index);
	bool SetLocalIndex(size_t index, size_t value);
	size_t GetSubChart(size_t index);
	bool SetSubChart(size_t index, size_t value);
	size_t GetLogLiVersion(size_t index);
	bool SetLogLiVersion(size_t index, size_t value);

	size_t GetChart(size_t index);
	size_t GetCurrentChart();
	SUM_PERFORMANCE GetPerformance_type(size_t index);
	CHART_TYPES GetChartType(size_t index);
	size_t GetNumberofCharts();
	size_t GetNumberofDocuments();
	size_t GetIndex_Chart(size_t iv);
	size_t GetArrayIndex(size_t indexchart, size_t indexvar);
	size_t GetNumVar(size_t indexchart);
	std::vector<size_t> GetNumVarForEachSubChart(size_t indexchart);
	size_t GetNumVarBands(size_t indexchart);
	size_t GetValFileNo(size_t index);
	size_t GetNumRecords(size_t indexchart);
	size_t GetDocIndex(size_t index);
	
	CPG *GetPGValFilePointer(size_t index);
	CPG *GetPGValFilePointer(size_t indexdoc, size_t ValidIndex, size_t SimRunNo, bool obsfile=false);
//	CPG *GetPGPointer(size_t indexdoc);
	std::string GetDocFileName(size_t index);
	
#ifndef COUPSTD	
	CPG *GetPGPointer(size_t indexdoc, size_t ValidIndex=0, size_t SimRunNo=0, bool MultiModeFile=false, size_t MeanPGType=0);	
	Doc *GetDocPointer(size_t index);
#endif


	size_t GetVar_Index(size_t index);
	bool IsMultiRunPlot(size_t index);
	void SetNumberSavedCharts(size_t);
	size_t GetNumberSavedCharts();
	std::string GetMultiSubDir();
	std::string GetActualVarName();
	std::string GetActualVarUnit();
	bool EnableSectionNo(size_t index, bool value);
	void EnableAllSectionNo(bool value);
	void ToggleAllSectionNo();

	size_t SelectedDoc_GetNum();
	void* SelectedDoc_GetAt(size_t index);
	void SelectedDoc_Reset();
	void SelectedDoc_Add(void*);

	void *m_pDoc;
	CPG *m_PG, *m_pCPG, *m_PGALL_Out, *m_PGSEL_Out;
	CPG *m_PG_ALLMULTI[MAXSIMVAL], *m_PG_SEL_MULTI[MAXSIMVAL], *m_PGVal;
	bool m_CPG_Active, m_CPGVal_Active;
	long m_PG_StartMin, m_PG_EndMin;

	std::vector<void*> m_SelectedDocArray;
	std::vector<CPG*> m_PGFileArray;
	std::vector<size_t> m_PGFileArrayShow;

	std::string GetTimeCreated();
	std::string GetTimeModified();
	void TimeModified();
	void TimeCreated();
	void SetTimeModified(time_t);
	void SetTimeCreated(time_t);
	void CPG_SetIncreasePlotPeriod(long stvalue, long endvalue);
	void CPG_SetDecreasePlotPeriod(long stvalue, long endvalue);
	void CPG_SetPlotPeriod(long stvalue, long endvalue);
	void CPG_SetStartMinEstimate(long value);
	void CPG_SetStartMin(long value);
	void CPG_SetEndMin(long value);
	long CPG_GetStartMin();
	long CPG_GetEndMin();

	bool CPG_Add(CPG *p);
	bool CPG_Reset();
	bool CPG_SetAt(size_t index,CPG *p, bool MultiFile=false);
	bool CPG_RemoveAt(size_t index);
	CPG* CPG_GetAt(size_t index);
	CPG* CPG_GetSimFile();
	size_t  CPG_GetNumber();
	bool CPGVal_Add(CPG *p);
	bool CPGVal_Reset();
	bool CPGVal_SetAt(size_t index,CPG *p);
	bool CPGVal_RemoveAt(size_t index);
	CPG* CPGVal_GetAt(size_t index);
	CPG* CPGVal_GetSimFile();
	size_t  CPGVal_GetNumber();
	bool CPG_SetPGSimFile();
	bool CPGVal_SetPGSimFile();
	bool CPG_SetTypeFile(size_t index, bool);
	size_t CPG_GetTypeFile(size_t index);


	std::vector<CPG*> m_CPG_Array, m_CPGVal_Array, m_CPGMulti_Array;
	std::vector<size_t> m_CPG_MultiType;

	std::vector<SIMB>m_X_Array;
	std::vector<SIMB>m_T_Array;
	std::vector<SIMB>m_G_Array;
	std::vector<SIMB>m_D_Array;
	std::vector<PLOTv> m_PlotArray;
	std::vector<std::string> m_XTGD_DocNameArray;
	std::vector<std::string> m_FailedNames;
	void SetValidIndepPGVar(bool value) {m_ValidIndepPGVar=value;};
	bool IsValidIndepPGVar() {return m_ValidIndepPGVar;};


	private:
	Doc *m_pSimDoc;
	bool m_ValidIndepPGVar;
};


