#pragma once
#include ".\docclick.h"
class DocCommands :
	public DocClick
{
public:
	DocCommands(void);

	~DocCommands(void);
	int CopyOutputSettings(int idoc);
	int CopySwitchSettings(int idoc);
	int CopySingleParameterSettings(int idoc);
	int CopyTableParameterSettings(int idoc);
	int CopyModelFileSettings(int idoc);
	void SetCurrentSwitchesToDefault(Doc* pDestinationDoc);
	void SetCurrentSingleParToDefault(Doc* pDestinationDoc);
	void SetCurrentTableParToDefault(Doc* pDestinationDoc);
	void SetCurrentModelFileToDefault(Doc* pDestinationDoc);
	void CopySwitchValues(Doc* pDestinationDoc);
	void CopySingleParValues(Doc* pDestinationDoc);
	void CopyTableParValues(Doc* pDestinationDoc);
	void CopyModelFileValues(Doc* pDestinationDoc);


	int HistoryCommand(int Col, int row, int item);
	int RunInfoCommand(int Col, int row, int item);
	int SwitchCommand(int Col, int row, int item);
	int ParameterCommand(int Col, int row, int item);
	int ParameterTableCommand(int Col, int row, int item);
	int ModelFileCommand(int Col, int row, int item);
	int OutPutVarCommand(int Col, int row, int item);
	int ValidationVarCommand(int Col, int row, int item);
	int ChartsCommand(int Col, int row, int item);
	int ChaParCommand(int Col, int row, int item);
	int MultiRunCommand(int Col, int row, int item);
	int AnnimateCommand(int Col, int row, int item);
	int MenuCornerCommand(int Col, int row, int item);
	int AttachedFileCommand(int Col, int row, int item);
	int ExcelExportCommand(int item, int Index=-1, string name="");


	bool MultiRunPlotCommand(int Col, int row, int item, enum vartype);

	void OnAddValFile();
	int OnAddTsvVal(bool Aggregated=false);
	int OnAddSvVal();
	int OnAddSvValFromFile();
	VALSUMLINK OnAddSvFlexFiles();
	void ReEvaluateSvVal();

	void OnValidationEvalView();
	void OnValidationEval();

	void DeleteChart(int);
	void UpdateChart(int);
	bool UpdateChart(int, CHART_TYPES);


protected:


	 
};

