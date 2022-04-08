#pragma once
#include ".\ChartCugChangeValue.h"
class ChartCugCommands :
	public ChartCugChangeValue
{
public:
	ChartCugCommands(void);
	~ChartCugCommands(void);

	bool MakePGFileFromDataInCurrentChart();

protected:
	bool RealTimeChangeMonOptions(int col, int row);
	bool ChangeActionOptions(int col, int row);
	bool ChangeGeneralOptions(int col, int row);
	bool ChangeChartOptions(int col, int row);
	bool ChangeDistibutionOptions(int col, int row);
	bool ChangeBoxCoxOptions(int col, int row);
	void OnStartAnnimation();
	void OnChangeAnnimationSpeed();
	void OnChangeAnnimationSteps();
	bool ChangeToResidualCurrentPlot(bool residual);

	bool MakeGeneralMenu(int col, int row);
	bool MakeAxisMenu(int col, int row);
	bool MakeVariableMenu(int col, int row);
	bool MakeDataMenu();


	bool MakeAxisCommand(int item);
	bool MakeVariableCommand(int item, int col, int row);

	 
};

