#pragma once
#include ".\makesimcharts.h"
class DocClick :
	public MakeSimCharts
{
public:
	DocClick(void);

	~DocClick(void);
	int RunInfoClick(int Col, int row);
	int SwitchClick(int Col, int row);
	int ParameterClick(int Col, int row);
	int ParameterTableClick(int Col, int row);
	int ModelFileClick(int Col, int row);
	int OutPutVarClick(int Col, int row);
	int OutPutVarSelectionClicktoStore(int Col, int row);
	int ValidationVarClick(int Col, int row, bool All=false);
	int ChartsClick(int Col, int row);
	int ChaParClick(int Col, int row);
	int MultiRunClick(int Col, int row);
	int AnnimateClick(int Col, int row);
	int MenuCornerClick(int Col, int row);
	int MultiParPlotClick(int Col, int row);
	int MultiCoVarParPlotClick(int Col, int row);
	int MultiRunPreparation(int Col, int row);
	int MultiRunMade(int Col, int row);

protected:
	ChartProp *m_pProp;


	 
};

