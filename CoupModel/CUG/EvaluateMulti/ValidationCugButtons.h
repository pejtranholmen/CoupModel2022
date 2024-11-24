#pragma once
#include "..\PEJUGCtrl.h"
class Doc;
class ChartProp;
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
	size_t m_Transformation{0}, m_Dim1_Index{0};
	long m_CurrentTopRow;

protected:

	BOOL m_Changed, m_ShowMean, m_Updated;
	BOOL m_Data;
	bool m_SavedCriteria2File;
	CPG *m_pPG;
	MyCug  *m_pCUG;
	ChartProp* m_pProp;
	bool m_Distribution_As_Independent{ true };
	size_t m_Indep_Par_Index{ 0 };


	int m_CurrentChart;
	int m_SelectedTimeShow, m_SelectedTimeIndCount;
	int m_SelectedVarShow, m_SelectedVarIndCount;
	int m_SplitPeriods;
	int m_NumberNewSplitPeriods;

	HINTLIST m_SetLowerHint, m_SetHigherHint, m_SetUnLockHint;



};

