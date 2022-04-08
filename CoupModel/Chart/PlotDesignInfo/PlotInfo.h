#pragma once

#include "../../NewBase/ModelBase.h"
#include "../../ModelTypes/SimB.h"
#include "../../pg/pg.h"
class Doc;
class ChartProp;
// CPlotStorage command target

struct PLOTv;

class PlotInfo
{


public:
	PlotInfo();
	virtual ~PlotInfo();
	void SetDoc(Doc *pDoc);
	
	PLOTv MakeOutPutChartInfo(size_t index_select, size_t index_doc);
	PLOTv MakePGFileChartInfo(size_t index_select);
	PLOTv MakeModelFileChartInfo(size_t index_select);
	PLOTv MakeValidationChartInfo(size_t  index_select, size_t index_doc, int ResidualPlot);
	PLOTv MakeMultiSumChart(size_t index_select, size_t index_doc);
	PLOTv MakeMultiValChart(size_t index_select, size_t index_doc);
	PLOTv MakeMultiParChart(size_t index_select, size_t index_doc);
	PLOTv MakeMultiPGChart(size_t index_select);
	void  SetFirstIndep(enum vartype vtype);
	void  DisableFirstIndep();
	void  DisableFirstIndep(enum vartype vtype);
	void  DisableSecondIndep(enum vartype vtype);
	void  SetSecondIndep(enum vartype vtype) ;
	string GetDepth(SIMB base);
	string GetDepth(SimB *pbase,size_t TabIndex=0);
	private:
	Doc *m_pSimDoc;
	ChartProp *m_pProp;
	PLOTv GetDefaultPlotInfo();
	P *m_pSimDepth;
};


