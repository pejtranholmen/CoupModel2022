#pragma once
#include "..\makesimcharts\doccommands.h"
class MakeSimSheets :
	public DocCommands
{
public:
	MakeSimSheets(void);
	~MakeSimSheets(void);
protected:

	BOOL m_sort[5];
	bool Make_RunInfoSheet();
	bool Make_SwitchesSheet();
	bool Make_ParameterSheet();
	bool Make_ParameterTableSheet();
	bool Make_ModelFileSheet();
	bool Make_HistorySheet();
	bool Make_OutputSheet();
	bool Make_ValidationSheet();
	bool Make_AnnimationSheet();
	bool Make_RunReportSheet();
	bool Make_ChartListSheet();
	bool Make_ChaParListSheet();
	bool Make_MultiRunListSheet();
	bool Make_AttachedFileSheet();
	bool m_SetUpDone;
	void JoinOutputListRows(size_t DefColToJoin = 3);

private:
	void ShowTimeSerieTableResults();

	size_t Val_Prior_Header();
	void Val_Prior_Data(size_t NumLines, size_t NumLineSV);
	size_t  Val_Post_Header();
	void Val_Post_Data(size_t NumLines, size_t NumLineSV);
	void Val_Post_TimeSerieTable(size_t NumLines, size_t NumLineSV);
	void Val_Post_CoVarRes();
	void Multi_Post_CompleteList_Header();
	void Multi_Post_CompleteList();
	void Multi_Post_CoVarList();
	void Multi_Post_NormalList();
	void Multi_Post_SensitivityTest();
	void Multi_Post_AllEnsembleList();
	void Multi_Prior_List();

};

