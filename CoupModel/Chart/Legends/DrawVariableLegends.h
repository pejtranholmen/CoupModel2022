#pragma once
#include "../ChartBase.h"
struct PLOTv;

class DrawVariableLegends : public ChartBase
{
public:
	DrawVariableLegends(void);
	~DrawVariableLegends(void);

	bool FindIdForVariablesToPlot(int j, PLOTv p_info, int PGindex);
	size_t GetNumberOfEnsembles();
	string GetEnsembleIdString(size_t i);


protected:
	void SetLegendInfo();
	
	bool SetLineWidth(DataSet *pDataSet);
	bool SetLineWidth(int index, DataSet *pDataSet);

	bool SetLineColors(int index, DataSet *pDataSet);
	bool SetSymbols(int index, DataSet *pDataSet);







};
