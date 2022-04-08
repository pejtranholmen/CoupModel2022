
#pragma once

#include "./ChartData\ChartData.h"
#include "../ModelTypes\CoupTypes.h"




class Doc;
class CCoupModelView;
class CChartViewer;
class ChartProp;
class ChartUtil;


class TableView: public ChartData 
{
	public:
	TableView();
	~TableView();
	void InitDoc(Doc *pDoc);
	void DrawTablePlot(CChartViewer *viewer=nullptr);
	bool MakeTablePlot(Tab *pTab, P *pCPt);
	void CleanTablePlot();

	bool m_Stacked, m_DepthProfile;
	Doc *m_pSimDoc;
	ChartProp *m_pProp;
	CChartViewer *m_pChartViewer;
	CCoupModelView *m_pView;
	ChartUtil *m_pUtil;
	CURVELABELS m_Label;

private:
	vector<XYChart*> m_XY_Chart;


};

