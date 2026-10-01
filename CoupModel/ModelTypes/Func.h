#pragma once

#include "./SimB.h"
#include "../PG/PG.H"
#include "./FunctFactory.h"
class WaterUptake_Functions;
class SimModule;
class FunctionProperties;
class ModelMap;
struct HYDRAULIC_COEF;
struct CONDUCTIVITY_COMMON_PAR;



class Func :public SimB {

public:
	//Func(double (WaterUptake_Functions::*fpoint)(double), WaterUptake_Functions*, enum datatype data, enum elements elem, enum fysprocess fproc, enum bioprocess bproc, enum userlevel ulev);
	Func(enum datatype data, enum elements elem, enum fysprocess fproc, enum bioprocess bproc, enum userlevel ulev);
	Func(definitions def);
	~Func(void);
	void Def(int ModuleNo,size_t opt, size_t ftype,SimModule *mClass,std::string name, FunctionProperties* pFunc, ModelMap *pModelMap);
	void Def(FunctorProp prop, SimModule *mClass, std::string name, FunctionProperties* pFunc, ModelMap *pModelMap);
	void Def(std::function<double(double)> *pFunctor, std::string name, FunctionProperties* pFunc, ModelMap *pModelMap);
	void Def(FunctorProp prop, std::string name, std::vector<Ps*> Ps_Vector,  FunctionProperties* pFunctions, ModelMap* pModelMap);
	void Def(FunctorProp prop, std::string name, std::vector<Ps*> Ps_Vector, std::vector<P*> P_Vector, FunctionProperties* pFunctions, ModelMap* pModelMap);
	void Def(FunctorProp prop, std::string name, std::vector<P*> P_Vector, FunctionProperties* pFunctions, ModelMap* pModelMap);
	void Def(FunctorProp prop, std::string name, HYDRAULIC_COEF *pHyd, std::vector<P*> P_Vector, FunctionProperties* pFunctions, ModelMap* pModelMap);
	void SetPlotParam(PLOT_PARAM param) {m_PlotParam=param;};
	void ResizeDoubleParVector(std::vector<double> *pDouble, size_t NewSize);

	bool   MakeOperation();
	double GetValue(double x, bool check=false);
	double GetValue(double x, double y);
	double GetValue(double x, double y, double z);
	double GetValue(double x, double y, double z, double zz);
	double GetValue(size_t)  noexcept;
	double GetValue(size_t, double x);
	double GetValue(size_t, double x, double y);
	double GetValue(size_t, double x, double y, double z, bool check=false);
	double GetValue(size_t, double x, double y, double z, double zz);
	
	size_t GetFuncType() const {return m_FuncType;};
	std::function<double(bool)> Get_boolBlank_Functor();
	std::function<std::vector<double>()> GetVector_Blank_Functor();
	std::function<std::vector<double>(bool)> GetVector_boolBlank_Functor();
	std::function<double(double)> Get_D1_Functor();
	std::function<double(double,double)> Get_D2_Functor();
	std::function<double(double,double, double)> Get_D3_Functor();
	std::function<double(double,double, double, double)> Get_D4_Functor();
	std::function<double(double, double, double, double, double)> Get_D5_Functor();
	std::function<double(bool, size_t)> Get_boolBlank_TableFunctor();
	std::function<double(bool, size_t, double, double, double)> Get_boolD3_TableFunctor();
	std::function<double(bool, size_t, double, double, double, double)> Get_boolD4_TableFunctor();
	std::function<double(size_t, double)> Get_D1_TableFunctor();
	std::function<double(size_t, double, double)> Get_D2_TableFunctor();
	std::function<double(size_t, double, double, double)> Get_D3_TableFunctor();
	std::function<double(size_t, double, double, double, double)> Get_D4_TableFunctor();
	std::function<double(size_t, size_t, double, double, double)> Get_T_D3_TableFunctor();

	std::function<void(std::vector<double>*)>  GetVectorPointer_D1_Functor();
	std::function<void(std::vector<double>*, std::vector<double>*)>  GetVectorPointer_D2_Functor();
	std::function<void(std::vector<double>*, std::vector<double>*, std::vector<double>*)>  GetVectorPointer_D3_Functor();
	std::function<void(std::vector<double>*, std::vector<double>*, std::vector<double>*, std::vector<double>*)> GetVectorPointer_D4_Functor();
	std::function<void(std::vector<double>*, std::vector<double>*, std::vector<double>*, std::vector<double>*, std::vector<double>*)> GetVectorPointer_D5_Functor();
	std::function<OrgTurnOver(double, double, double)> Get_OrgTurnOverFunctor();
	std::function<OrgTurnOver(double, double, double, double, double)> Get_D5_OrgTurnOverFunctor();
	std::function < std::pair<double, double>( double, double)> Get_Pair_D2Functor();
	std::function < std::pair<double, double>(size_t, double, double)> Get_Pair_D2_TableFunctor();
	std::function < std::pair<double, double>(double, double, double)> Get_Pair_D3Functor();
	std::function < std::pair<double, double>(size_t, double, double, double)> Get_Pair_D3_TableFunctor();



    enum MyArgType	GetArgumentTypes() const;
	bool IsTableType() const { return m_TableType; };



	PLOT_PARAM GetPlotParam() const {return m_PlotParam;};
	Hydraulic* Get_PointerHydraulic() { return m_FunctorProp.pHyd; };
	MyFunc Get_FunctorName() { return m_FunctorProp.FuncName; };

	double GetXMin() {return m_PlotParam.X_Min;};
	double GetXMax() {return m_PlotParam.X_Max;};
	std::string GetXLabel() {return m_PlotParam.X_Label;};
	std::string GetYLabel() {return m_PlotParam.Y_Label;};
	bool   GetLogScale() {return m_PlotParam.LogYScale;};
	bool   GetLogXScale() {return m_PlotParam.LogXScale;};
    void   SetFunctor(std::function<bool()> functor) { m_pBoolFunctor = functor; };
	void   SetFunctor(std::function<bool(bool)> functor) { m_D1Functor= functor; };
	bool   MakeBool();
	bool m_FunctorFunctions;
	std::vector<std::string> GetParDep_Ps_Names() const { std::vector<std::string> out; 
		for (size_t i = 0; i < m_Ps_Vector.size(); i++) out.push_back(((SimB*)m_Ps_Vector[i])->GetName());
		return out;
	};
	std::vector<std::string> GetParDep_P_Names()const  {
		std::vector<std::string> out;
		for (size_t i = 0; i<m_P_vector.size(); i++) out.push_back(((SimB*)m_P_vector[i])->GetName());
		return out;
	};
	double Get(double x);
	double Get(size_t, double x);
	
protected:
	void UpdateP_Par();
	void UpdatePs_Par();
	size_t m_FuncType;
	size_t m_Opt;
	size_t m_Index;
	SimModule *m_Class;
	PLOT_PARAM m_PlotParam;
	std::function<bool()> m_pBoolFunctor;
	std::function<double(double)> m_D1Functor;
	std::function<double(double)> *m_pD1Functor;

	std::function<double(double)> *m_pFunctor;
	std::vector<Ps*> m_Ps_Vector;
	std::vector<P*> m_P_vector;
	std::vector<double> m_FunctorPs_Par;
	std::vector< std::vector<double>> m_FunctorP_Par;
	FunctorProp m_FunctorProp;
	bool m_TableType;
	HYDRAULIC_COEF *m_pHydraulic_Coef;
	CONDUCTIVITY_COMMON_PAR *m_pCondCommonPar;



};
