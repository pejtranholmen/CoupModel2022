#pragma once
#include "chartdir.h"
#include "../../std.h"

struct CURVELABELS
{
std::string st;
std::string test;
std::string xtitle;
std::string ytitle1;
std::string ytitle2;
std::string unit;
std::string Legendtxt;
std::string unit1;
std::string unit2;
std::string IndepId;
std::string DocNumId;
};
struct CHART_INFO
{	std::string title;
	std::vector<std::string> legends;
	std::vector<std::string> varname;
	std::vector<std::string> varunit;
	std::string Y_AxisTitle_Left;
	std::string Y_AxisTitle_right;
	std::string X_AxisTitle;
};

struct DATA_INFO {
	size_t NumVariables;
	size_t NumVariableBands;
	size_t NumOfPoints;
};




class ChartData
{
	public:
	ChartData(void);
	~ChartData(void);
	DATA_INFO m_Data;
	DoubleArray m_dataSeriesA[NUM_SERIES];
	DoubleArray m_dataSeriesB[NUM_SERIES];
	DoubleArray m_dataSeriesP10[NUM_SERIES];
	DoubleArray m_dataSeriesP50[NUM_SERIES];
	DoubleArray m_dataSeriesP90[NUM_SERIES];
	DoubleArray m_dataSeriesMIN[NUM_SERIES];
	DoubleArray m_dataSeriesMAX[NUM_SERIES];
	DoubleArray m_dataSeriesC[NUM_SERIES];


	protected:
		void AllocateNumberOfBands(size_t nbands) {
			BB.clear();
			BB.resize(nbands);
			BB_MIN.clear();
			BB_MIN.resize(nbands);
			BB_MAX.clear();
			BB_MAX.resize(nbands);
			BB_P10.clear();
			BB_P10.resize(nbands);
			BB_P50.clear();
			BB_P50.resize(nbands);
			BB_P90.clear();
			BB_P90.resize(nbands);
		};
		void AllocateNumberOfPointsinBands(size_t npoints) {
			for(size_t i=0; i<BB.size(); i++) {
				BB[i].clear();
				BB[i].resize(npoints);
				BB_MIN[i].clear();
				BB_MIN[i].resize(npoints);
				BB_MAX[i].clear();
				BB_MAX[i].resize(npoints);
				BB_P10[i].clear();
				BB_P10[i].resize(npoints);
				BB_P50[i].clear();
				BB_P50[i].resize(npoints);
				BB_P90[i].clear();
				BB_P90[i].resize(npoints);
			}			
		};
		void AllocateNumberOfCurves(size_t ncurves) {
			AA.clear();
			AA.resize(ncurves);
		};
		void AllocateNumberOf_IndepCurves(size_t ncurves) {
			TT_Indep.clear();
			if(ncurves>0) TT_Indep.resize(ncurves);
		};
		void AllocateNumberOfPointsinCurves(size_t npoints){
			for(size_t i=0; i<AA.size(); i++) {
				AA[i].clear();
				AA[i].resize(npoints);
			}
			for(size_t i=0; i<TT_Indep.size(); i++) {
				TT_Indep[i].clear();
				TT_Indep[i].resize(npoints);
			}
		};
		CHART_INFO m_CurrentChartInfo;
		std::vector<CHART_INFO> m_ChartOrigInfo;
		std::vector<double> TT, TT2, ZZ;
		std::vector< std::vector<double>>  TT_Indep;

		std::vector< std::vector<double>>  AA, BB, BB_MIN, BB_MAX, BB_P10, BB_P50, BB_P90;
		std::vector< std::vector<double>> BX, BY;

/*static double TT[NUM_TIME];
static double TT2[NUM_TIME];
static double AA[NUM_SERIES][NUM_TIME];
static double BB[NUM_BAND][NUM_TIME];
static double BB_MIN[NUM_BAND][NUM_TIME];
static double BB_MAX[NUM_BAND][NUM_TIME];
static double BB_P10[NUM_BAND][NUM_TIME];
static double BB_P50[NUM_BAND][NUM_TIME];
static double BB_P90[NUM_BAND][NUM_TIME];
static double BX[100][100];
static double BY[100][100];*/

};

