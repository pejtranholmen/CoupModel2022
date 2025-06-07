#pragma once
#include "./ChartData\LoadDataToChart.h"


struct CHARTLAYERS
{
	LineLayer *pLineLayer;
	LineLayer *pLineLayer2;
	LineLayer *pBandLayer;
	TrendLayer *pTrendLayer;
	bool BandLayerExist;
	XYChart *c;

	AreaLayer *pAreaLayer;
	BoxWhiskerLayer *pErrLayer;
	BoxWhiskerLayer *pErrLayer2;
	ScatterLayer *pScatterLayer;
};
struct CHARTCOLORS
{
	int ivar;
	int box;
	int range;
	int edge;
	int dist;
};
struct CHARTSYMBOLS
{
	int isymb;

};


struct SUBCHARTSIZE
{
	int yrow;
	int xcol;
	int xdiff;
	int xdiff2;
	int xdiff1;
	int ydiff;
	int ydiff1;
	int ydiff2;
	int yplotheight;
	int xplotwidth;
};
struct ZOOM
{
	int Hpos;
	int StartPos;
	int ZoomLevel;
	double SubWindow;
	double Window;
	int NumberOfWindows;
	int TimeUnit;
	double viewPortStartDate;
	double viewPortEndDate;
};
struct PLOTv;

class CChartViewer;


class ChartBase: public LoadDataToChart
{
	public:
	ChartBase(void);
	~ChartBase(void);

	void CheckIfRightAxis(enum class DATA_TYPES itype, int ivar, int curve_count);
	void CheckAndAddLabelInfo(size_t ich, int idoc);
	void SetSymbolInfo(int ivar, int curve_count, bool CommonColors);

	
public:
	ZOOM m_Zoom;
	
	CURVELABELS m_Label;
protected:

	void SetMultiSequenceInfo();
	void SetPointertoActivePGFile(int Index_In_PlotArray, PLOTv p_info);

	std::vector<ColorAxis*> m_pColorAxis;
	std::vector<DataSet*> m_DataSets;
	
	CHARTLAYERS m_Layers;

	CHARTCOLORS m_Color;
	CHARTSYMBOLS m_Symbol;
		

	SUBCHARTSIZE m_ChartSize;

	CChartViewer *m_pChartViewer;

	XYChart *pXY_Chart;  // Pointer to XY Chart
	size_t m_ichart; //Index for chart Number



};
