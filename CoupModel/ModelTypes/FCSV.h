#pragma once
#include "../std.h"
#include "./SimB.h"
#include <fstream>
#include <iostream>


class ModelBase;
class FCSV :public SimB {

public:
	FCSV(std::ifstream *, enum datatype data, enum elements elem, enum fysprocess fproc, enum bioprocess bproc, enum userlevel ulev);
	FCSV(definitions def);
	~FCSV(void);
	void Def(size_t ModuleNo, std::string name, std::string value, std::string conditions, size_t numheaders);

	bool CheckFileName(ModelBase *pModelBase=nullptr);
	bool CheckIfExistAndOpen() { if (m_Exist&&m_headervar.size() > 0) return true; else return false; };

	bool SetValue(std::string newvalue) {m_FileName=newvalue; return true;};
	bool SetDBValue(std::string str) {dbstrvalue=str; return true;};

	bool GetHide() {return m_hide;};
	void SetHide(bool value) {m_hide=value;};
	std::string GetStrValue() const {return m_FileName;};
	std::string GetDBStrValue() {return dbstrvalue;};
	void SetDBStrValue(std::string str) {dbstrvalue=str;};
			// resets tmpvalues to values
	virtual void	SetOriginalValue();		// Sets the original value to the current value
	std::string   GetOriginalStrValue();
	std::ifstream*	GetPointer() { return m_fstream; };
	void	SetPointer(std::ifstream *Ptr) { m_fstream = Ptr; };

	void CheckSizeOfFile();
	bool CheckMinMaxValues();
	size_t AssignSIMB_PointersFromHeader();
	std::string GetVarHeader(size_t index) { if (m_headervar.size() > index) return m_headervar[index]; else return ""; };
	std::string GetVarHeader_Unit(size_t index) { if (m_headervar_unit.size() > index) return m_headervar_unit[index]; else return ""; };


	double GetVarMin(size_t index) { if (m_VarMin.size() > index) return m_VarMin[index]; else return double(MISSING); };
	double GetVarMax(size_t index) { if (m_VarMax.size() > index) return m_VarMax[index]; else return double(MISSING); };
	size_t GetNumLines() { return m_LineNo; };
	size_t GetNumVariables() { return m_VarNo; };
	size_t GetNumHeadings() { return m_Headers; };
	void   SetNumHeadings(size_t numheadings) { m_Headers = numheadings; };
	size_t AssignKey_WithValues();
	std::vector<double> GetParValue(size_t keyindex);
	std::vector<std::string> GetCDB_Value(size_t keyindex);
	std::vector<double> GetMinVector() { return m_VarMin; };
	std::vector<double> GetMaxVector() { return m_VarMax; };
	std::vector<SIMB> GetSIMB_vector();
	std::vector<size_t> GetKeyVector();
	bool WithTableValues() { if (m_LineNo > 0 && m_Complete) return true;
		else return false;
	};

	bool m_Complete;
protected:
	std::ifstream *m_fstream;
	
	
	std::string 	extension;
	std::string		tmpvalue, originalValue, dbstrvalue;
	void *m_Ptr;
	std::string	m_FileName;
	int 		rec, frec, view, optionals;
	bool		m_hide;

private :
	ModelBase *m_pModelBase;
	bool m_Exist;
	size_t m_LineNo, m_VarNo;
	std::vector<std::string> m_headervar, m_headervar_unit;
	std::vector<double> m_VarMin;
	std::vector<double> m_VarMax;
	std::vector<SIMB> m_ParameterPointers;
	size_t m_Headers;
	std::map <size_t,size_t>KeyIndex;
	std::vector< std::vector<double>> m_Values;
	std::vector<  std::vector<std::string>> m_KeyValues;
};
