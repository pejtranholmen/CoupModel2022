#pragma once

#include "..\PEJugctrl.h"
#include "..\..\ModelTypes\SimB.h"
#include "..\..\NewBase\Doc.h"
#include "..\..\SoilData\PlotPF\PFCurve.h"

class PFCug;
class CDialog;
class SP_Retrieve;
class SoilData;
class DB_Header;

class PFMenu :
	public PEJUGCtrl

{
public:
	PFMenu(void);
	~PFMenu(void);
virtual int  OnMenuStart(int col,long row,int section);






public:
	Doc *m_pDoc;
	DB_Header *m_pDB;
	CDB *m_pDB2;
	long m_col,m_row, m_toprow;
	bool m_CompleteList;
	bool m_SelOne;
	UINT *m_pProf,*m_pRepProf;
	PFCurve* p_PFCurve;

protected:

	bool m_OneAction;
	Tab* m_pCTab;
	DBDataCug *m_pDBDataCug;

	int m_LatestSheet, m_PrevClickRow;
	NEWHEADER m_NewHeader, m_ActProfile;



	bool m_DataBase;
	PFCug *m_pCUG;
	SoilData *m_pCUG_Data;

	string m_DataBaseString;
	CDialog *m_pPP_EDIT_Dlg;
	SP_Retrieve *m_pSP_Retrieve_Dlg;

private:
	
	

};