#pragma once

//#include ".\Base.h"
#include "./History.h"



class DB :public History
{

public:
	DB();
	bool Retrieve(std::string group, std::string key, bool NoApply=false);
	bool Read_DB_DocFile(std::string file, std::string key, bool Apply=false, bool ExportToCSV=false);
	bool Write_DB_DocFile(std::string file);
	bool Apply_DB_DocFile(std::string group, std::string key="", bool during=false);
	bool Apply_DB_DocFile(std::vector<Sw*> AppliedSw, std::vector<Ps*> AppliedPs, std::vector<Tab*> Applied_Tab, std::vector<F*> Applied_File);
	bool Export_DB_CSVFile(std::vector<Sw*> AppliedSw, std::vector<Ps*> AppliedPs, std::vector<Tab*> Applied_Tab, std::vector<F*> Applied_File);
	bool Apply_DB_Struc(std::string);
	void Actual_DB(std::string group="");

	bool ReadFrom_DB_Object(std::string ID_DB_Group, std::string key="", bool deletefile=false, bool NoApply=false, bool ExportToCSV=false);
	void SaveTo_DB_Object(std::string, std::string key);

	void SetGroupConnections(std::string Group);
	void Reset();
	bool m_ShortTimeRead;
	std::string m_Group;
	std::string m_DB_key;
	bool m_DuringRun;
	std::string m_SimFileName;
	Sw *m_pSwTimeRes;
	Sw *m_pSwPlantType;
	Sw *m_pSwLAI, *m_pSwRoot, *m_pSwGen1, *m_pSwGen2;
	Sw *m_pSw_NC;
	F *m_pFile;
	bool m_HideAllTableContents;

	std::string m_ID_DB_Group;
	std::string m_SelectedId;
	std::string m_SelectedKey;

	std::vector<Ps*>m_P_Array;
	std::vector<P*>m_Pt_Array;
	std::vector<Tab*>m_Tab_Array;
	std::vector<F*>m_F_Array;
	std::vector<Sw*>m_Sw_Array;
	std::vector<FCSV*>m_FCSV_Array;

protected:
	
	int m_Read_DB_Version;
	bool Info_DB_Switches(bool reading, int iv=0);
	std::vector<Sw*> ReadWrite_DB_Switches(bool reading, int iv = 0);
	bool Info_DB_Parameters(bool reading, int iv=0);
	std::vector<Ps*> ReadWrite_DB_Parameters(bool reading, int iv = 0);
	bool Info_DB_ParTables(bool reading, int iv=0);
	std::vector<Tab*> ReadWrite_DB_ParTables(bool reading, int iv = 0);
	bool Info_DB_Files(bool reading, int iv=0);
	std::vector<F*> ReadWrite_DB_Files(bool reading, int iv = 0);

	std::ifstream m_DocFileIn;
	std::ofstream m_DocFileOut;
	
	std::string CreateFlagString(SimB* pBase, bool GDType, int FlagValue);
	bool wString(std::string *str);
	bool wInt(int *iv);
	bool wSize_t(size_t* iv);
	bool wUint(unsigned int * iv);
	bool wbool(bool *iv);
	bool wFloat(float *f);
	bool wDouble(double *f);
	bool wTime(time_t *time);
	
	bool rString(std::string *str);
	bool rInt(int *iv);
	bool rUint(unsigned int * iv);
	bool rSize_t(size_t* iv);
	bool rBOOL(bool *iv);
	bool rbool(bool *iv);
	bool rFloat(float *f);
	bool rDouble(double *f);
	bool rTime(time_t *time);
	char m_cbuf[400];




};