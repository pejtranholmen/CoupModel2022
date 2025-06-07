#pragma once
#include "../Chart\Axis\DrawAxisDesign.h"


struct PLOTv;
class Doc;
class CChartViewer;


class DrawContourDetails: public DrawAxisDesign
{
	public:
	DrawContourDetails(void);
	~DrawContourDetails(void);
	
	bool MakeContourPlot(XYChart *c, int ich, int ivar);

	DataSet* SetMultiSimChartData(XYChart *c,  int ich, int ivar);
	DataSet* SetPGVarContourChartData(XYChart *c,  int ich, int ivar);
	DataSet* SetPGDepthContourChartData(XYChart *c,  int ich, int ivar);
protected:
	std::vector<ContourLayer*> m_pConLayer;

};
