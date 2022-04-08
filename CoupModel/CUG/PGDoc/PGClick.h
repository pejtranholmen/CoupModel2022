#pragma once
#include "PGCalcActions.h"
#include "..\MakePGCharts\MakePGCharts.h"

class PGClick :
	public PGCalcActions
{
public:
	PGClick(void);

	~PGClick(void);


protected:
	void ExpandFileNameLine(int col, int row, int ParV);
	void ChangeSubChartNumber(int col, int row);
	void ClearAllChartVar(int col, int row);
	void ChangeChartVar(int col, int row);
	void ChangeChartChoice(int isheet, int col, int row, int ind, CString After, int ParV=0);
	void MultiPGChartCheck();
	void MultiPGChartDepChoice(int PGIndex, size_t Option, bool erase);

	MakePGCharts m_PGCharts;
	 
private:
	PlotInfo *m_pPlotInfo;
};

