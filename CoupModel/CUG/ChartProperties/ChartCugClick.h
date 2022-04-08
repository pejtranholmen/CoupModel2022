#pragma once
#include ".\chartCugCommands.h"
class ChartCugClick :
	public ChartCugCommands
{
public:
	ChartCugClick(void);

	~ChartCugClick(void);

	void OnNewSeq2();
	void OnNewCurve();
protected:

	void ActionDuringRealTimeMode(size_t col, size_t row);
	void ActionDuringParameterPlotMode(size_t col, size_t row);
	void ActionDuringTablePlotMode(size_t col, size_t row);
	void ActionDuringTimeSerieMode(size_t col, size_t row);

	void ParameterChangeRealTimeMode(size_t col, size_t row);
	void ValidationRealTimeMode(size_t col, size_t row);
	void ChartDetailsParameterPlotMode(size_t col, size_t row);
	void LayersParameterPlotMode(size_t col, size_t row);

	void VariablesTimeSerieMode(size_t col, size_t row);



};

