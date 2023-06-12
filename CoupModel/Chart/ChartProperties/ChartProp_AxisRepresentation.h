#pragma once
#include "../../std.h"
#include "../../Util.h"
#include "../../ModelTypes/SimB.h"
#include "../../NewBase/ModelMap.h"

#ifndef INDEP_AXIS_SET
struct INDEP_AXIS
{
	vartype Dependent=vartype::NO_USE;
	vartype First = vartype::NO_USE;
	vartype Second = vartype::NO_USE;
};
#define INDEP_AXIS_SET 1
#endif
class CPG;




struct MULTI_SUM_DEP
{
	bool FirstIndepSet=false;
	bool SecondIndepSet=false;
	int First_Index=0;
	int Second_Index=0;
	enum sumtype FirstType=sumtype::INITIAL;
	enum sumtype SecondType= sumtype::INITIAL;
	SIMB simbase;
	string FirstStr;
	string SecondStr;
};
struct MULTI_PAR_DEP
{
	bool FirstIndepSet;
	bool SecondIndepSet;
	int First_Index;
	int Second_Index;
	SIMB simbase;
	string FirstStr;
	string SecondStr;
};

struct MULTI_VAL_DEP
{
	bool FirstIndepSet;
	bool SecondIndepSet;
	int First_Index;
	int Second_Index;
	enum valtype FirstType;
	enum valtype SecondType;
	enum SUM_PERFORMANCE SV_PeformanceType;
	string FirstStr;
	string SecondStr;
	bool FirstIndepTimeSerieType;
	bool SecondIndepTimeSerieType;
};
struct MULTI_PG_DEP
{
	bool FirstIndepSet;
	bool SecondIndepSet;
	size_t First_Index;
	size_t Second_Index;
	enum vartype FirstType;
	enum vartype SecondType;
	enum vartype Dependent;
	string FirstStr;
	string SecondStr;
	CPG *pPG;
	bool MultiPGFilePlot;
};
struct MULTI_VALID
{
	bool AddOnlyValidRuns;
	bool AcceptRejectMode;
	bool MultiValMode;
};

struct PG_FILTER
{
	int FilterVar;
	int ValidValue;
};

class ChartProp_AxisRepresentation
{
public:
	ChartProp_AxisRepresentation(void);
	~ChartProp_AxisRepresentation(void);
	
	bool IsIndepSet() {if(m_Indep.First!=MULTIRUN_NUM) return true; else return false;};
	bool IsMultiPGFilePlot() { if(m_Indep.First==PGVAR&&m_Indep.Second==PGVAR) return true; else return false;};



	void SetMultiPGFilePlot() { 
		m_Indep.First=PGVAR;m_Indep.Second=PGVAR;};
	size_t GetFirstIndepIndex(size_t index=0);
	size_t GetSecondIndepIndex();
	void SetFirstIndepIndex(enum vartype, size_t index, size_t index2=0);
	void SetSecondIndepIndex(enum vartype, size_t index);
	void SetFirstIndepString(enum vartype, string str, size_t index=0);
	void SetSecondIndepString(enum vartype, string str);
	enum vartype GetFirstIndepVarType() {return m_Indep.First;};
	enum vartype GetSecondIndepVarType() {return m_Indep.Second;};
	void SetFirstIndepVarType(enum vartype var) {m_Indep.First=var;if(var==NO_USE) SetFirstIndepIndex(var, 0); };
	void SetSecondIndepVarType(enum vartype var) {
		m_Indep.Second=var;if(var==NO_USE) SetSecondIndepIndex(var, 0);};
	void SetDependentVarType(enum vartype var) { m_Indep.Dependent = var; };
	string GetFirstIndepString(size_t index=0);
	string GetSecondIndepString();
	bool IsFirstIndepSet(size_t index=0);
	bool IsFirstIndepSet(enum vartype var, size_t index=0);
	bool IsSecondIndepSet(enum vartype var);
	bool IsSecondIndepSet();
	void SetNumberOfValidationPG_Dep(size_t ncurves) {m_ValidationPG_dep.clear(); if(ncurves>0) m_ValidationPG_dep.resize(ncurves);} ;

	void SetFirstIndepSet(enum vartype var, bool value, size_t index=0) ;
	void SetFirstIndepValidTimeType(bool TimeType = false) {m_MultiVal_dep.FirstIndepTimeSerieType = TimeType;};
	void SetSecondIndepValidTimeType(bool TimeType = false) { m_MultiVal_dep.SecondIndepTimeSerieType = TimeType; };
	bool GetFirstIndepValidTimeType() { return m_MultiVal_dep.FirstIndepTimeSerieType; };
	void SetIndepValid_SV_Type(enum  SUM_PERFORMANCE var) { m_MultiVal_dep.SV_PeformanceType = var; };
	SUM_PERFORMANCE GetIndepValid_SV_Type() {return m_MultiVal_dep.SV_PeformanceType; };
	bool GetSecondIndepValidTimeType() { return m_MultiVal_dep.SecondIndepTimeSerieType; };
	void SetSecondIndepSet(enum vartype var, bool value) ;
	


	size_t GetIndepFirst() { return m_Indep.First; };
	size_t GetIndepSecond() { return m_Indep.Second; };
	size_t GetIndepDependent() { return m_Indep.Dependent; };


	MULTI_VALID m_MultiValid;

	MULTI_VAL_DEP m_MultiVal_dep;
	MULTI_PG_DEP m_MultiPG_dep;
	vector<MULTI_PG_DEP> m_ValidationPG_dep;
	MULTI_SUM_DEP m_MultiSum_dep;
	MULTI_PAR_DEP m_MultiPar_dep;



private:
	INDEP_AXIS m_Indep;

	
};

