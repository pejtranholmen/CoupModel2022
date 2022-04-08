#pragma once
#include "DrawContourDetails.h"

struct PLOTv;
class Doc;
class CChartViewer;


class DrawDetails: public DrawContourDetails
{
	public:
	DrawDetails(void);
	~DrawDetails(void);
	DataSet* AddDataFirstLeftPlot( int ivar,int TotNumPrior, bool accept_type, PLOTv pinfo, int ivarBand,  bool FirstCurve);
	DataSet* AddDataPlot(int ivar,int TotNumPrior, bool accept_type, PLOTv pinfo, int ivarBand,  bool FirstCurve);

	DataSet* TimeSerieChart(int ivar,int TotNumPrior, bool accept_type, PLOTv pinfo, int ivarBand,  bool FirstCurve);
	DataSet* TimeSerieFrequenceChart(int ivar,int TotNumPrior, bool accept_type, PLOTv pinfo, int ivarBand,  bool FirstCurve);
	DataSet* TimeSerieScatterChart( int ivar,int TotNumPrior, bool accept_type, PLOTv pinfo, int ivarBand,  bool FirstCurve);
	//DataSet* TimeSerieFrequenceChart(XYChart *c, int ivar,int TotNumPrior, bool accept_type, PLOTv pinfo, int ivarBand, int ich, bool FirstCurve);
	bool AddData_ToChart( size_t TotNumPrior);
	void AddTitle_Axis_ToChart(MultiChart *mc);
	void AddChartToMulti(MultiChart *mc);
	






};