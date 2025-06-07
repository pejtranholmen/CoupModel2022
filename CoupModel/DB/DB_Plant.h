
#pragma once


// DB_Plant.h : header file
//
class Sw;
/////////////////////////////////////////////////////////////////////////////
// DB_Plant

class DB_Plant 
{


// Construction
public:
	DB_Plant(){};
	DB_Plant(std::string pszCaption, bool, void *pDoc) ;
	DB_Plant(std::string pszCaption, void *pDoc, std::string key="");

// Attributes
public:

// Operations
public:



// Implementation
public:
	void Init(std::string pszCaption, bool, void *pDoc);
	void Init(std::string pszCaption,   std::string key="");
	virtual ~DB_Plant();
	void Actual_DB();
	void DB_Actual();
	bool DB_Struc_Actual(std::string Name);
	bool ReadWrite(std::string filename, bool writing=true);
	bool m_ReadOnly;

	bool m_DuringRun;


protected:

	void *m_pDoc;
	std::string m_Group; 
	std::string mm_name;
	std::string m_DB_key;
	Sw *m_pSwTimeRes;
	Sw *m_pSwPlantType;

};


