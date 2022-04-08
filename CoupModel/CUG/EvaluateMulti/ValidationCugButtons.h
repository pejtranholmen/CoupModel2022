#pragma once
#include "..\PEJUGCtrl.h"
class Doc;
class ValidationCugButtons :
	public PEJUGCtrl
{
public:
	ValidationCugButtons();
	~ValidationCugButtons();
	//menu notifications
	virtual int  OnMenuStart(int col,long row,int section);

	//hints
	virtual int OnHint(int col,long row,int section,CString *string);
	virtual int OnVScrollHint(long row,CString *string);
	virtual int OnHScrollHint(int col,CString *string);

	void DefineHintList();

	bool m_OnGoingUpdate;
	Doc *m_pSimDoc;
	size_t m_CumulatedPerformance;

protected:

	BOOL m_Changed, m_ShowMean, m_Updated;
	BOOL m_Data;
	bool m_SavedCriteria2File;
	CPG *m_pPG;
	MyCug  *m_pCUG;


	int m_CurrentChart;
	int m_SelectedTimeShow, m_SelectedTimeIndCount;
	int m_SelectedVarShow, m_SelectedVarIndCount;
	int m_SplitPeriods;
	int m_NumberNewSplitPeriods;

	HINTLIST m_SetLowerHint, m_SetHigherHint, m_SetUnLockHint;



};

