#pragma once


// PFCommands.h : interface of the PFCommands class
//
/////////////////////////////////////////////////////////////////////////////


#include ".\PFButtons.h"




class PFSheets:public PFButtons
{
	public:
	PFSheets();
	~PFSheets();

	void MakeBaseSheet(bool Extra=false, bool NewProf=false);
	void MakeSelectedProfileSheet();
	void MakeProfileSheet();
	void MakeTextureSheet();
	void MakePFCurveSheet();
	void MakePF_PressureSheet();
	void MakeSimpleTextureSheet();
	void MakeBoCSheet();
	void MakeGenuchtenSheet();
	void MakeHydraulicCondSheet();
	void MakeCommentsSheet();
	void MakeEstimationSheet();
	void MakeLinkToMultiRunTableSheet();

	void Make_LinkToChangeDuringRunSheet();
	void Set_Changed_LinkToChangeDuringRun_Document();


	
	BOOL m_bDrawExcelFocus;
	UINT	m_ProfMin;
	UINT	m_TopSandMin;
	UINT	m_SubSandMin;
	UINT	m_SubSandMax;
	UINT	m_SubClayMin;
	UINT	m_SubClayMax;
	UINT	m_ProfMax;
	string	m_CountyMin;
	string	m_CountyMax;
	UINT	m_TopSandMax;
	UINT	m_TopClayMin;
	UINT	m_TopClayMax;
	UINT	m_TopOrgMin;
	UINT	m_TopOrgMax;
	UINT	m_SubOrgMin;
	UINT	m_SubOrgMax;
	string	m_PFpoints;
	UINT	m_YearMin;
	UINT	m_YearMax;
	unsigned int m_Min_NumHeads;
	void SaveTopRow();
	void SetTopRowExternt();


protected:
	vector<size_t> GetSortIndex();

	void SetDescriptionRow(int Count, NEWHEADER header);
	bool KeyTest(NEWHEADER header);
	int GetMultiTypeIndex(string);
	NEWHEADER m_ActHeader;


	COLORREF m_ColorClick, m_ColorChanged, m_ColorHeading;
	int m_IniProf, m_IniRepProf;

};