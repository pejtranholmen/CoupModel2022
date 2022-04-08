#pragma once


// PFCommands.h : interface of the PFCommands class
//
/////////////////////////////////////////////////////////////////////////////
#include ".\PFMenu.h"

struct BC_RESULT
{
	size_t Prof;
	float EstimateMin;
	float EstimateMax;
	BC_ESTIMATE bc;
};


class PFCommands:public PFMenu
{
	public:
	PFCommands();
	~PFCommands();

	//menu notifications
	virtual void OnMenuCommand(int col,long row,int section,int item);
	string EstimatePFCurveByPoints(size_t layer, bool All);
	string EstimatePFCurveByTexture(size_t layer, bool All);
	string EstimateHydraulicCondByTexture(size_t layer, bool All);
	bool EditOfProfileInfo(int row, string str);
	bool EditOfLayerInfo(int col, int row, string str);
	void UpdateLinkToProfile(); 

protected:

	SoilData *m_pSoilData;

	float m_PF_EstimateMax, m_PF_EstimateMin;
	double m_PF_ResidualFix;
	bool m_EstimationMade;
	vector<BC_RESULT> m_EstimationResults;

	void MakeChart(size_t index, SimB *pSim, Tab *pTab, size_t option=0);
	void SelectDB_Object_ToMultiRunSim(long row, int item=5101);
	void SelectDB_Object_ToChaDuringRun(long row, int item=5110);
	size_t m_SortOption;
	void SelectPFCurve_Object_ToMultiRunSim(long row, int item=5101);
	void SelectPFCurve_Object_ToChaDuringRun(long row, int item=5110);
	bool m_DataImported;
};