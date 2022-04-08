
#pragma once
#include "./ChartData\ChartData.h"

class Doc;
class CCoupModelDoc;
class CChartViewer;
class ChartProp;
class ChartUtil;



class ModelView : public ChartData
{
	public:
	ModelView();
	~ModelView();
	void DrawModelPlot();
	void MakePlantView(XYChart *c, int xplotwidth, int yplotheight);
	void MakeSnowView(XYChart *c, int xplotwidth, int yplotheight);
	void MakeNC_Plant(XYChart *c, int xplotwidth, int yplotheight);
	void MakeSoilWater(XYChart *c, int xplotwidth, int yplotheight);
	void MakeSoilHeat(XYChart *c, int xplotwidth, int yplotheight, bool heatmade);
	void MakeSoilMinN(XYChart *c, int xplotwidth, int yplotheight);
	void MakeSoilOrg(XYChart *c, int xplotwidth, int yplotheight);
	void MakeHBV(XYChart *c, int xplotwidth, int yplotheight);
	void MakeNC_GasFlow(XYChart *c, int xplotwidth, int yplotheight);
	void InitDoc(Doc *pDoc);
	bool GetModelData(int xplotwidth, int yplotheight, int ydiff);
	void InitData();
	bool m_Init;
	void MakeModelPlot();
	void CleanModelPlot();
	void SetImageMap();

	Doc *m_pSimDoc;
	ChartProp *m_pProp;
	CChartViewer *m_pChartViewer;
	ChartUtil *m_pUtil;
	vector<XYChart *> m_XY_Chart;
	BaseChart *m_pMC;

	bool m_HeatEq;
	int m_LateralInput, m_WaterEq, m_NC,m_HBV,m_Evapo,m_Gw, m_Irrigation, m_PlantType, m_SaltTracer, m_SnowPack, m_SoilVapour;


};

