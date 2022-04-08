#include "ChartData.h"


ChartData::ChartData(void)
{
	m_Data.NumOfPoints=NUM_TIME;
	m_Data.NumVariables=NUM_SERIES;
	TT.resize(NUM_TIME);
	TT2.resize(NUM_TIME);
	AA.resize(NUM_SERIES);
	BX.resize(NUM_SERIES);
	BY.resize(NUM_SERIES);
	for(size_t i=0; i<NUM_SERIES; i++) {
		AA[i].resize(NUM_TIME);
	}
	BB.resize(NUM_BAND);	
	BB_MIN.resize(NUM_BAND);
	BB_MAX.resize(NUM_BAND);
	BB_P10.resize(NUM_BAND);
	BB_P50.resize(NUM_BAND);
	BB_P90.resize(NUM_BAND);
	for(size_t i=0; i<NUM_BAND; i++) {
		BB[i].resize(NUM_TIME);
		BB_MIN[i].resize(NUM_TIME);
		BB_MAX[i].resize(NUM_TIME);
		BB_P10[i].resize(NUM_TIME);
		BB_P50[i].resize(NUM_TIME);
		BB_P90[i].resize(NUM_TIME);
	}
	for(size_t i=0; i<NUM_SERIES;i++) {
		BX[i].resize(NUM_SERIES);
		BY[i].resize(NUM_SERIES);
	}

	
}


ChartData::~ChartData(void)
{
}
