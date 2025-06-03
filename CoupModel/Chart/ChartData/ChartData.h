#pragma once
#include "chartdir.h"
#include "../../std.h"

struct CURVELABELS
{
string st;
string test;
string xtitle;
string ytitle1;
string ytitle2;
string unit;
string Legendtxt;
string unit1;
string unit2;
string IndepId;
string DocNumId;
};
struct CHART_INFO
{	string title;
	vector<string> legends;
	vector<string> varname;
	vector<string> varunit;
	string Y_AxisTitle_Left;
	string Y_AxisTitle_right;
	string X_AxisTitle;
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
		vector<CHART_INFO> m_ChartOrigInfo;
		vector<double> TT, TT2, ZZ;
		vector< std::vector<double>>  TT_Indep;

		vector< std::vector<double>>  AA, BB, BB_MIN, BB_MAX, BB_P10, BB_P50, BB_P90;
		vector< std::vector<double>> BX, BY;

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

