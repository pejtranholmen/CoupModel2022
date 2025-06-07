#pragma once
//#include "../../stdafx.h"
#ifndef _HAS_STD_BYTE
#define _HAS_STD_BYTE 0
#endif
#include "../../std.h"

#include "../FileCreate/ChartFileSave.h"
enum class CHART_OPTION{LINEWIDTH, LINESTYLE, SYMBOLSTYLE, SYMBOLSIZE };
enum class CHART_SYMBOLS{NO_SYMBOL, FILLED_CIRCLE, OPEN_CIRCLE,FILLED_SQUARE, OPEN_SQUARE,
FILLED_DIAMOND, OPEN_DIAMOND, FILLED_RECTANGLE, OPEN_RECTANGLE};


class ChartZoom;
class ChartUtil
	{
	public:
		ChartUtil(void);
		~ChartUtil(void);
	ChartFileSave m_FileSave; 

	bool SetColorBackground(int);
	bool SetColorLegendBackground(int);
	bool SetColorLegendBackgroundFrame(int);
	int  GetColorLegendBackground();
	int  GetColorLegendBackgroundFrame();
	int  GetColorBackground();
	int GetHColorLine(int);
	int GetColorBoxEdge();
	int GetColorBoxEdgeH();
	bool SetColorBoxEdge(int color) ;
	bool SetColorDist(int color);
	int GetColorDist();
	int GetColorDistH();
	void SetLegendContentStyle(int value) ;
	int GetLegendContentStyle() {return m_LegendContentStyle;};
	void SetResidualRepresentation(int value) ;
	int GetResidualRepresentation() {return m_ResidualRepresentation;};
	void SetErrorSignal(int value) ;
	int GetErrorSignal() {return m_ErrorSignal;};

	void SetRemoveXAxisScale(int value) ;
	int GetRemoveXAxisScale() {return m_RemoveXAxisScale;};
	void SetRemoveYAxisScale(int value) ;
	int GetRemoveYAxisScale() {return m_RemoveYAxisScale;};
	void SetGapHorPercentage(int value);
	void SetGapVertTopPercentage(int value);
	void SetGapHorLeftPercentage(int value);
	void SetRemoveLegends(int value);
	void SetRemoveTitle(int value);
	int GetRemoveTitle() {return m_RemoveTitle;};

	float GetScaleInterval(float max, float min);
	float GetGapHorFraction() noexcept {return m_GapHFraction;};
	float GetGapVertTopFraction() noexcept {return m_GapVertTopFraction;};
	float GetGapHorLeftFraction() noexcept {return m_GapHorLeftFraction;};
	
	void SaveCurrentLinePatternScheme(CHART_OPTION);
	void RetrieveSavedLinePatternScheme(CHART_OPTION);
	void ResetLinePatternScheme(CHART_OPTION);


	
	void SetXAxisExist(){m_XAxisExist=true;};
	bool IsXAxisExist() {return m_XAxisExist;};

	void SetZAxisExist(){m_ZAxisExist=true;};
	bool IsZAxisExist() {return m_ZAxisExist;};

	int GetLineTypeIndex(int lineNo);
	int GetSpecificLineWidth(int lineNo);
	int GetSpecificSymbolSize(int lineNo);
	int GetSpecificSymbol(int lineNo);
	int GetSymbolCode(int lineNo);
	int GetSymbolOpenCode(int lineNo);

	void SetLineTypeIndex(int lineNo, int value);
	void SetSpecificLineWidth(int lineNo, int value);
	void SetSpecificSymbolSize(int lineNo, int value);
	void SetSpecificSymbol(int lineNo, int value);



	static int GetSymbolIndex(std::string str);
	static int GetSymbolSizeIndex(std::string str);
	static int GetLineTypeIndex(std::string str);
	static int GetLineThickSizeIndex(std::string str);
	static int GetVisibleIndex(std::string txt);
	static int GetZoom_ValDurOption(std::string);

	int GetLegendXPos();
	void SetLegendXPos(int value){ MySingle::instance().GetChartUtilPointer()->m_LegendPositionX=value;};
	int GetLegendYPos();
	void SetLegendYPos(int value) { MySingle::instance().GetChartUtilPointer()->m_LegendPositionY=value;};

	int GetMinTickDistX();
	void SetMinTickDistX(int value);
	int GetMinTickDistY();
	void SetMinTickDistY(int value);
	bool MakeLegends() {if(m_RemoveLegends==0) return true; else return false;};
	bool MakeSingleLegends() {if(m_RemoveLegends==1) return true; else return false;};

	void SetProfileViewFrequence(int Frequence=5);
	size_t GetProfileViewFrequence();
	void SetSymbolCodes();

	void SetAutoX_ScaleValue(int value);

	int GetEnsembleViewOption();
	void SetEnsembleViewOption(int value);

	int m_BoxEdgeColor;
	int m_DistColor;
	int m_ColorBackground, m_ColorLegendBackground, m_ColorLegendBackgroundFrame;
	int m_RemoveTitle;
	int m_LegendContentStyle;
	int m_ResidualRepresentation;
	int m_RemoveXAxisScale, m_RemoveYAxisScale;
	int m_RemoveLegends;

	int m_RejectedPoints;
	int m_TitleSizePoint,m_LabelSizePoint,m_LegendSizePoint,m_LineWidthPoint,m_SymbolSizePoint;
	int m_LineAxisWidthPoint, m_AxisScalePoint;
	int m_LegendPositionX, m_LegendPositionY;
	int m_MinTickDistX,m_MinTickDistY;

	size_t m_SelectProfileViewFrequence;

	bool m_RealTimeMode;
	bool m_ParameterPlotMode;
	bool m_TablePlotMode;
	bool m_AutoShiftFirstIndep;
	bool m_MakeExtraChart;
	bool m_SaveExtraChart;
	bool m_MultiRunPlot;
	bool m_MultiPGPlot;
	bool m_MultiTable;
	int m_ResidualPlot;

	bool  m_ErrorSignal;

	bool m_Displayed;
	bool m_ToBeUpdated;
	float m_GapHFraction,m_GapVertTopFraction,m_GapHorLeftFraction;
	bool m_XAxisExist, m_ZAxisExist;

	int m_X_AxisAutoScaleSensitivy;
	int m_EnsembleViewOption{0};
	double m_X_AxisAutoScaleValue;

	int m_SymbolCode[10];
	int m_SymbolOpenCode[10];
	
	private:




};

