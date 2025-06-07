#pragma once

#ifndef _HAS_STD_BYTE
#define _HAS_STD_BYTE 0
#endif
#include "SimBConst.h"

#ifndef MISSING
#define MISSING -1.1E38f
#define NUM_SERIES 200
#define NUM_BAND 16
#define NUM_TIME 50
#endif
#include <vector>
#include <string>
#include <string_view>
#include <map>
#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>

struct OUTPUT_SUMMARY_RESULTS
{
	double Initial;
	double Min;
	double Max;
	double Acc;
	double AccTotal;
	bool YBin;
	bool NC_Type;
};
struct VALID_TEST_RESULTS
{
	size_t count;
	size_t duration;
	size_t nc_shift;
	double acc_sim;
	size_t acc_sim_count;
	double acc_sim_prior;
	size_t acc_sim_prior_count;
	bool hitdone;
	bool delayed_hit;
	bool postmean;
	bool lastrecord;
	bool lasthit;
	bool firsthit;
	size_t nextrecord;
	unsigned int minute_post;
	unsigned int minute_post2;
	unsigned int minute_prior;
	double value_post;
	double value_prior;
};





enum class DATA_TYPES {
	ANYDATA, SIM_X, SIM_T, SIM_G, SIM_D, SIM_VAL,
	SIM_PERFORMANCE, SIM_PAR, SIM_OUTSUM, SIM_SUMVAR, PGDATA, MEASURED_VAL, SIM_OUTSUM_X, SIM_OUTSUM_Y, SIM_ACCEPTED_PERFORMANCE
};

enum class sumtype { INITIAL, FINAL, MINVALUE, MAXVALUE, MEANVALUE, ACCVALUE };
enum class valtype { R2VALUE, INTERCEPT, SLOPE, MERROR, RMSE, LOGLIKE, NSE };
enum class vartype {NO_USE,vartype,DATEAXIS,MULTIRUN_NUM,PGVAR,SUMVAR,VALVAR,PARVAR,PGREPIT,PGYEAR,PGMONTH,PGDAY,PGHOUR,PGDEPTH,PGVARVALID,GEO_LAT,GEO_LONG,GEO_ALT,VAR_MIN,VAR_MAX,VAR_MISSING,VAR_FIRST,VAR_LAST,VAR_MEAN,VALVARSUM};

enum class CHART_TYPES { SCATTER, TIME_SERIE, FREQUENCE, CUM_FREQUENCE, CONTOUR_CHART };
enum class TimeResolution { WITHIN_DAY, DAILY_MEAN, HOURLY, TEN_MIN };
enum class SIMB_COMP{NOVALUE, LE, LT, GE, GT, EQ, NEQ, ON, OFF};
enum class HIST_INFO {RUN_INFO, DATA_BASE_INFO, SWITCH_INFO, PARSINGLE_INFO, PARTABLE_INFO,NUMBER_ELEMENTS, MODELFILE_INFO, OUTPUT_SINGLE, OUTPUT_VECTOR};
struct OUTPUT {size_t LocalIndex; float Initial; float Final; float Min; float Max; float Mean; float Accumulated; size_t ValidFlagIndex; float OptInitial; size_t PgFileIndex; size_t PgMultiFileIndex;int StoreFlag;}	;
constexpr std::string_view StringId[] = {"Ps","X","T","G","D","Sw","F", "Ps", "Xs","Ts", "Gs", "Ds", "P", "Table","DB","Func","CSV"};
constexpr std::string_view Sw_StringOptions[] = { "O1","O2","O3", "O4", "O5", "O6", "O7", "O8","O9","O10","O11", "O12", "O13", "O14", "O15", "O16" , "O17", "O18" };
enum class simtype {PAR, STATE, FLOW, AUX, DRIVE, SWITCH, PGFILE, PAR_SINGLE, STATE_SINGLE, FLOW_SINGLE,AUX_SINGLE, DRIVE_SINGLE, PAR_TABLE, TABLE, DB, FUNCTION, CSVFILE, NUM_ELEMENTS_VECTOR};
enum class datatype{SINGLE,SIMPROFILE,MPROFILE,SINGLEPLANT,PLANT,MYCON,OTHER,WATERPIPE,HEATBOX,ATMBOUNDARY,SURFACEPOOL,WATERPOND, 
	IRRIGATIONCONTAINER,ROADSURFACE,SOILSURFACE,SOILBOTTOM,OBSERVED,TOTPROFILE,ALL_SOIL_LAYERS,ALL_SOIL_BOUNDARIES,ALLPLANTS,ACCUMULATOR,FILEDEFINEDPARAMETERS};
enum class elements {
	WATER, HEAT, CARBON, NITROGEN, PHOSPHORUS, RADIATION, SNOW, SALT, METHANE, VAPOUR, GENERAL, TEMPERATURE, WATERPOTENTIAL, ATMOSPHERE,
	AIR, PRECIPITATION, CLIMATE, ORGANIC, DOC, DON, DOP, MINERAL_N, MINERAL_P, AMMONIUM, NITRATE, MYCORRHIZA, DENITRIFIERS, NITRIFIERS, ANAROBIC_GAS, OXYGEN, MANURE, PLANT_SHAPE, DISTANCE, SOILCO2, TIME, AREA, PLANT_STRESS, WIND,
	GLOBALRADIATION, LONGWAVEINCOMINGRADIATION, HUMIDITY, GHG, PLANT_BIOMASS , SOILGAS_NO, SOILGAS_N2O , COS
};
enum class fysprocess{WATERUPTAKE, EVAPOTRANSPIRATION, TRANSPIRATION, SOILEVAPORATION,POTTRANSPIRATION, INTERCEPTION, TURBULENCE, SURFACERUNOFF,
	RESISTANCE,SNOW_MELTING,
	REDISTRIBUTION,STORAGE,TRANSPORT, INFILTRATION, DRAINAGE, PERCOLATION, RUNOFF,NOFPROC,INITF, SHRINKING, FYS_SHAPE, 
	EXTRACTION, HEATPIPE, HEATSOURCE,LOWERBOUNDARY,UPPERBOUNDARY,WATERSOURCE,SALTSOURCE,
	EBAL_PARTITIONING,HEAT_CONDUCTION, HEAT_CONVECTION, TECH_REGULATOR, EMISSION,ADSORPTION,REFLECTION, MASSBALANCE, SENSOR, GEOPOS, FROST_BOUNDARY, HYSTERESIS,IRRIGATION, BUDGET,
	GEOMETRIC, SCATTERING, SOIL_FREEZING, DIFFUSION};
enum class bioprocess{DECOMPOSITION,MINERALISATION, NITRIFICATION, DENITRIFICATION, RESPIRATION, PRODUCTION,OXIDATION, REALLOCATION, PHENOLOGY, SOIL_GAS_DYNAMICS,
	PHOTOSYNTHESIS, LITTERFALL,NOBPROC, INITB, INIT_P,FORCING_BIOLOGY, ORGANICTRANSFORMATION, RATIO,CONCENTRATION, PLANTORGUPTAKE, PLANT_NUPTAKE, PLANT_PUPTAKE, SORPTION, HARVEST,
	GRAZING, MYCON_NUPTAKE, SOILMIXING, EMERGENCE, PLANT_MANAGEMENT, NFIXATION, LIVING, NON_LIVING, GROWTH, WOODEATING, WEATHERING, METHANE_OXIDATION, METHANE_PRODUCTION, METHANE_TRANSPORT,
	SOIL_GAS_EQUILIBRIUM, COS_TRANSPORT
};
enum class userlevel{SIMPLE, NORMAL, DETAILED};
enum class MR_METHOD{LINEAR_CHANGE, LOG_CHANGE, STOCHASTIC_LIN_CHANGE, STOCHASTIC_LOG_CHANGE, TABLE_VAL, DB_RECORD, SAME_AS, REL_SAME_AS, OP_REL_SAME_AS, BAYESIAN_CAL, CONNECTED, BAYESIAN_CAL_LOG, NELDER_MEAD};

enum class UNIT_TYPES {
	MEGA_HEATFLOW_UNIT,
	HEATFLOW_UNIT, HEATSOURCE_UNIT, HEATSTORE_UNIT, MEGA_HEATSTORE_VOL_UNIT,
	TEMP_UNIT, TEMPSUM_UNIT, HEATCOND_UNIT, MASS_UNIT, CARBON_MASS_UNIT, CARBON_MASSFLOW_UNIT, NITROGEN_MASS_UNIT, NITROGEN_MASSFLOW_UNIT, MASSFLOW_UNIT, WIND_SPEED_UNIT,
	WATERFLOW_UNIT, VOLPERCENTAGE_UNIT, PRESSUREHEAD_UNIT, AREA_UNIT, MASSDENSITY_UNIT, KILO_MASSDENSITY_UNIT, INVERSE_MASS_UNIT,
	mMASS_UNIT, mMASSFLOW_UNIT, WATERMASS_UNIT, PRESSURE_UNIT, LENGTH_UNIT, LENGTH_PER_TIME_UNIT, AERODYNAMIC_RESISTANCE_UNIT,
	MIKRO_MOL_SEC_RATE, AREA_PER_TIME_UNIT, AREA_PER_SEC_TIME_UNIT, CM_AREA_PER_SEC_TIME_UNIT, GAS_MASS_UNIT, PPM_CONC_UNIT, YEAR_AGE_UNIT, PERCENTAGE, TIME_UNIT, ENERGY_MOLE,
	mMASS_LITER_UNIT, mikro_MASS_LITER_UNIT, NO_UNIT, RATECOEF_UNIT, INVERSE_WATERFLOW_UNIT, INVERSE_SQUARED_FLUX_PER_VOLUME, MASS_RATIO_UNIT, VOLUME_RATIO_UNIT,
	MIKRO_MASS_RATIO, MIKRO_MASS_FLOW_UNIT, MIKRO_MASS_RATE, TIME_MINUTE_UNIT, ANGLE_UNIT, THERMAL_CONDUCTIVY_UNIT, INVERSE_AREA_UNIT, DAYOFYEAR_UNIT,
	INVERSE_TEMP_UNIT, WATERCONTENT_UNIT

};
struct DEPTH_type {
	size_t NumLay=0;
	std::vector<double> Dist;
};

enum class ENSEMBLE_TYPE { NO, ALL, ACCEPTED };




class SimB;
class CommonModelInfo;
struct SIMB {SimB* pBase=nullptr;
	size_t TabIndex= std::string::npos;
};
struct SIMB_VALUE {
	SimB* pBase = nullptr;
	size_t TabIndex = std::string::npos;
	double newvalue= MISSING;
};
struct MR_PAR{
	int Dimension=0;
	double Min=0.;
	double Max=0.;
	double Start=0.;
	double RelChange=0.;
	enum MR_METHOD Method=MR_METHOD::LINEAR_CHANGE;
	SIMB MR_Dependent;
};
struct PLOT_PARAM {
	size_t Tot_Num_Of_Curves=0; // Correspnds to Num_Of_Curves*Num_Of_Opt 
	size_t Num_Of_Curves=0;
	size_t Num_Of_Points=0;
	bool LogXScale=false;
	bool LogYScale=false;
	int X_ScalingCoef = 0;
	int Y_ScalingCoef = 0;
	double X_Min=0.;
	double X_Max=0.;
	std::string X_MaxPar;
	std::string X_Label;
	std::string Y_Label;
	size_t Num_Of_X=0;
	size_t Num_Of_Opt=0;
	std::vector<std::string> LegendsVec;
	std::vector<size_t> Opt_Vec;
	std::vector<float> X2_Vec;
	std::vector<float> X3_Vec;
	std::vector<float> X4_Vec;
	std::vector<float> X5_Vec;
};
struct PLOT_PARAM_DEP {
	std::vector<std::string> Ps_Names;
	std::vector<std::string> P_Names;
};
enum class DB_types
{
	Plant_Hydrology,
	Soil_Properties,
	Soil_Evaporation,
	Snow,
	Soil_Freezing,
	Soil_Water_Flows,
	Plant_Growth,
	Soil_Manage,
	Soil_Organic,
	Soil_InOrganic
};

class CE; class Sw; class NewMap;
struct Condition {
	Sw*	variable;
	enum  SIMB_COMP op;
	int		value;
};

struct definitions {enum class datatype data; enum class elements elem; enum class fysprocess fproc; enum class bioprocess bproc; enum class userlevel ulev;; enum class UNIT_TYPES unittype;};

class SimB
	{
	public:
		SimB(void);
		SimB(enum simtype,enum datatype, enum elements,enum fysprocess fproc, enum bioprocess bproc, enum userlevel);
		SimB(enum simtype,enum datatype, enum elements, enum userlevel);
		SimB(enum simtype,enum datatype, enum elements);
		SimB(enum simtype, definitions);
		SimB(enum simtype);
		virtual ~SimB(void);
		void SetEnable(bool value) noexcept {_enabled=value;};
		void SetDBEnable(bool value) noexcept {_DBenabled=value;};
		void SetChildChange(bool value) noexcept { m_ChildChange = value; };
		bool IsChildChanged() noexcept { return m_ChildChange; };
		void SetModuleNo(size_t value) noexcept {_ModuleNo=value;};
		void SetName(std::string str) {_name=str;};
		void SetConditions(std::string str)  {cond=str;};
		void SetUnit(std::string str)  {_unit=str;};
		//void SetDefValue(double value) { _orgFValue=value;};
		virtual bool IsEnabled() noexcept {if(_enabled) return false; return true;};
		bool IsDBEnabled() noexcept {return _DBenabled;};
		bool Is_PTable() noexcept {if(_stype==simtype::PAR_TABLE) return true; return false;};
		bool Is_DB() noexcept {if(_stype==simtype::DB) return true; return false;};
		bool Is_P() noexcept {if(_stype==simtype::PAR_SINGLE) return true; return false;};
		bool Is_Vector() noexcept {if(_stype<=simtype::DRIVE) return true; return false;};
		simtype GetType() noexcept {return _stype;};
		std::string GetTypeName() const;
		int  GetUserLevel() const noexcept {return int(_def.ulev);};
		size_t  GetGroupNo() const noexcept {return _ModuleNo;};
		virtual bool IsOldValue() noexcept {return false;};
		virtual int GetIntValue() noexcept {return -1;};
		virtual std::string GetOption(size_t ) {return "";};
		virtual std::string GetName() {return _name;};
		virtual void SetDBIntValue(size_t value) noexcept {_DBIntValue=value;};
		virtual size_t GetDBIntValue() noexcept {return _DBIntValue;};
		virtual int MR_Get_Dim() noexcept {return -1;};
		virtual int MR_Get_Dim(size_t ) noexcept {return -1;};
		virtual MR_METHOD MR_Get_Method() noexcept {return MR_METHOD::STOCHASTIC_LIN_CHANGE;};
		virtual MR_METHOD MR_Get_Method(size_t ) noexcept {return  MR_METHOD::STOCHASTIC_LIN_CHANGE;};
		virtual double GetMinValue() noexcept {return MISSING;};
		virtual double GetMinValue(size_t ) noexcept {return MISSING;};
		virtual double GetWarnMinValue() noexcept {return MISSING;};
		virtual double GetWarnMinValue(size_t ) noexcept {return MISSING;};
		virtual double GetWarnMaxValue() noexcept  {return -MISSING;};
		virtual double GetWarnMaxValue(size_t ) noexcept {return -MISSING;};
		virtual double GetMaxValue() noexcept {return -MISSING;};
		virtual double GetMaxValue(size_t ) noexcept {return -MISSING;};
		virtual double GetValue() noexcept {return MISSING;};
		virtual double GetValue(size_t ) noexcept {return MISSING;};
		virtual double GetDBValue() noexcept {return MISSING;};
		virtual double GetDBValue(size_t) noexcept {return MISSING;};
		virtual size_t GetPgFileIndex(size_t )noexcept  {return 0;};

		virtual size_t GetValidFlagIndex(size_t ) const noexcept {return std::string::npos;};
		virtual size_t GetValidFlagIndex() const noexcept {return std::string::npos;};

		virtual void SetStoreFlag(bool) noexcept {};
		virtual void SetMultiStoreFlag(bool) noexcept {};
		virtual void SetStoreFlag(size_t , bool) noexcept {};
		virtual void SetMultiStoreFlag(size_t, bool) noexcept {};
		virtual bool GetStoreFlag(size_t ) noexcept {return false;};
		virtual bool GetMultiStoreFlag(size_t ) noexcept {return false;};
		virtual size_t GetNumberOfSelectedFlags(bool ) const  noexcept {return 0;};
		virtual size_t GetNumberOfFlags(bool ) const  noexcept {return 0;};
		virtual int GetOriginalIntValue() {return _orgIntValue;};
		void SetGroupNo(size_t value)  noexcept { _ModuleNo = value; };

		virtual double GetOriginalValue()  noexcept {return MISSING;};
		
		virtual double GetOriginalValue(size_t index)  noexcept {if(index<_orgFVector.size()) return _orgFVector[index]; return MISSING;};
		size_t GetOriginalSize()  noexcept {return _orgFVector.size();};
		//int GetGroupCategory() {return ::GetGroupCategory(GetGroup());};
		size_t GetFysProcess()  noexcept {return size_t(_def.fproc);};
		size_t GetBioProcess()  noexcept {return size_t(_def.bproc);};
		std::string GetFysProcessName() const ;
		std::string GetBioProcessName() const;

		enum elements GetElement()   noexcept { return _def.elem; };
		enum datatype GetDataType() noexcept {return _def.data;};

		std::string GetDepName(size_t) const;
		virtual std::string GetUnit() const {return _unit;};		
		UNIT_TYPES GetUnitType() const  noexcept { return _def.unittype; };
		std::string GetDepUnit(size_t) const;	   			// Get name of dependent variables 
		std::string GetGroup();
		std::string_view GetProcessName();
		std::string_view GetElementName();

		enum simtype GetSim()   noexcept {return _stype;};
		void SetSimType(enum simtype value)  noexcept {_stype=value;};
		bool	IsTrue(size_t) const;	
		virtual void	Notice();
		virtual void	SetIsOriginalValue()  noexcept {m_IsNotOriginal=false;};
		virtual void	SetNotOriginalValue()  noexcept {m_IsNotOriginal=true;};			// Sets the original value to the current value
		virtual void SetOriginalValue(double )  noexcept {;};
		void SetOriginalValue(std::vector<double> value)  noexcept {_orgFVector.resize(value.size()); for(size_t i=0; i<value.size();i++) {_orgFVector[i]=value[i];};};

		virtual bool	IsNotOriginalValue() const  noexcept {return m_IsNotOriginal;};
		void SetBackOriginalValue()  noexcept {m_IsNotOriginal=false;};

		std::string GetCondition(size_t) const;
		void	AddToNoticeList(SimB*); 
		void	BuildDependencies(NewMap*);

		void AddHistoryIndex(size_t value) {m_HistoryIndex[0].push_back(value); };
		void AddHistoryIndex(size_t index, size_t value) { if(index<100) m_HistoryIndex[index].push_back(value);};
		size_t GetHistoryIndex(size_t index)  noexcept {	if(index<100) return m_HistoryIndex[0][index];	return 0; };
		size_t GetHistoryIndex(size_t index1, size_t index2) noexcept {	if(index2<m_HistoryIndex[index1].size()) return m_HistoryIndex[index1][index2]; return 0; };
		size_t GetNumOfHistoryIndex()  noexcept {	return m_HistoryIndex[0].size();};
		size_t GetNumOfHistoryIndex(size_t index1)  noexcept { if(index1<100)	return m_HistoryIndex[index1].size(); return 0;};
		void SetReadOnly(bool value)  noexcept { m_ReadOnly = value; };
		bool IsReadOnly()  noexcept { return m_ReadOnly; };


		virtual bool SetAnnimateChart(int , int )  noexcept {return false;};
		virtual bool SetAnnimateChart(size_t , int , int)  noexcept {return false;};
		CommonModelInfo* getCommonModelPointer() noexcept { return m_pCommonModelInfo; };
		void setCommonModelPointer(CommonModelInfo* p) noexcept { m_pCommonModelInfo = p; };
		std::vector<Condition> GetArrayConditions() { return conditions_Array; };
	protected:

		SimB*	GetDepPtr(size_t) const;
		void	SendNotice() const;	
		NewMap* pfMap = nullptr;
		std::string _unit;
		size_t _DBIntValue = 0;
		int _orgIntValue = 0;
		bool m_ChildChange = false;
		CommonModelInfo* m_pCommonModelInfo = nullptr;
		std::vector<double> _orgFVector;
	private:
		bool m_IsNotOriginal = false;
		bool m_ReadOnly = false;
		bool _enabled = false;
		bool _DBenabled = true;
		std::string _name;
		definitions _def{
			datatype::SINGLE,
			elements::WATER,
			fysprocess::STORAGE,
			bioprocess::NOBPROC,
			userlevel::NORMAL
		};
		enum simtype _stype = simtype::FUNCTION;
		size_t _ModuleNo = std::string::npos;
		std::string cond;
		std::vector<Condition> conditions_Array;
		std::vector<SimB*> noticelist_Array;
		std::vector<size_t> m_HistoryIndex[100];

		void Decode(std::string);		 	// Decodes condition std::string
		SIMB_COMP OpDecode(std::string,bool*) const;		// Decodes operators
		int	ValDecode(std::string) const;	  	// Decodes std::string to integer number
	};

