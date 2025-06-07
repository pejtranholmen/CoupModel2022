#pragma once
#include "../../std.h"
#include "../../Util.h"
#ifndef COUPSTD
#include <ChartDir.h>
#endif
#include "NumFrames.h"
class ChartFrame: public NumFrames
{
public:
	ChartFrame(void);
	~ChartFrame(void);

	std::string GetTitle();
	void SetTitle(std::string);
	


	int GetSymbolStyleCode(int line);
	int GetSymbolStyleOpen(int line);


	int	 GetAxisLayer(int index);
	bool SetAxisLayer(int index, int value);


	bool SetDashModeLine(int, int);
	int  GetDashModeLine(int);
	bool SetLineWidth(int,int);
	int	 GetLineWidth(int);
	bool SetSymbolStyle(int,int);
	int	 GetSymbolStyle(int);
	bool SetSymbolSize(int,int);
	int	 GetSymbolSize(int);


	int GetNumChartX();
	void SetNumChartX(int value);
	int GetNumChartY();
	void SetNumChartY(int value) ;
	size_t GetNumOfChartsOnSceen() {	return m_NumChartX*m_NumChartY;	};

	int GetGapMargin(); 
	void SetGapMargin(int value);
	int GetLeftMargin();
	void SetLeftMargin(int value) ;
	int GetRightMargin();
	void SetRightMargin(int value) ;
	int GetTopMargin();
	void SetTopMargin(int value);
	int GetBottomMargin();
	void SetBottomMargin(int value);
	bool GetCommonColors();
	void SetCommonColors(bool value) {m_CommonColorForSubCharts=value;};

	bool GetRoundedFrame();
	void SetRoundedFrame(bool value) {m_RoundedFrame=value;};

	int GetNumSubCharts();


	void SetVisibleFrames(int value) ;
	int GetVisibleFrames() {return m_VisibleFrames;};

	long GetBoxDimmer() {return m_BoxDimmer*0x1000000;};
	void SetBoxDimmer(long value){ m_BoxDimmer=value;};
	long GetBoxRangeDimmer() {return m_BoxRangeDimmer*0x1000000;}; 
	void SetBoxRangeDimmer(long value) {m_BoxRangeDimmer=value;};

	long GetBoxGapScaling() { return m_BoxGapScaling;};
	void SetChartError(int value);
	int GetChartError() {return m_ChartError;};

	int GetNumVariable() {return m_NumVariable;};

	

/// Member variables

	std::vector<size_t> m_LineWidth, m_DashStyle;
	std::vector<size_t> m_SymbolStyle, m_SymbolSize;
	std::vector<int> m_PointType, m_SymbolType;
#ifndef COUPSTD
	DoubleArray m_IndepX[NUM_SERIES];
	DoubleArray m_timeStamps, m_timeStamps2;
#endif
	std::vector<double> m_ViewPortData_A[NUM_SERIES];
	std::vector<double> m_ViewPortTime;
    double* viewPortTimeStamps, *viewPortTimeStamps2;
	double* viewPortDataSeries[NUM_SERIES];
	double* viewPortDataSeriesB[NUM_BAND];
	double* viewPortDataSeriesMin[NUM_BAND];
	double* viewPortDataSeriesMax[NUM_BAND];
	double* viewPortDataSeriesP10[NUM_BAND];
	double* viewPortDataSeriesP50[NUM_BAND];
	double* viewPortDataSeriesP90[NUM_BAND];


	std::vector<size_t> m_AxisLayer;

	
	bool m_CommonColorForSubCharts;
	bool m_MultiMode;

	int m_MultiRepititions;
	bool m_ToBeUpdated;
	long m_BoxDimmer;

	long m_BoxRangeDimmer;
	long m_BoxGapScaling;
	int m_ChartError;
	bool m_rightaxis;
	int  m_LeftMargin,m_RightMargin, m_GapMargin, m_TopMargin,m_BottomMargin;
	bool m_VisibleFrames;
	bool m_RoundedFrame;

	size_t m_NumVariable;

	std::string m_Title;
	
	int m_cx, m_cy;
	int m_ChartNumber, m_PrevChartNumber;

	



};

