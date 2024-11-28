#include "DistributionCurves.h"


class CResiduals;
struct TIMEDATA
{
	double MinDate;
	double MaxDate;
	double DateRange;
	double CurrentDuration;
	double MinDuration;
};

struct CURVEINFO
{
	bool Band;
	CPG *pPG1;
	int PGindex;
	CResiduals *pRes;
	bool MeanOfAll;
	int ResRunNo;
	bool MeasuredAsSource;
};
struct BOXCOX
{
	bool use=false;
	float box_power=1.;
	float box_offset=0.;
};



struct AGGREGATING
{
	size_t StartIndex=0;
	size_t EndIndex=0;
	size_t ArraySizeInBytes=4;
	int AggregateRatio=1;

};
struct CONTOURCHART
{
	bool ScatterContourPoints=false;
	bool SplineContourInterPolation=false;
	bool ScaleContourChart=false;
	double SecondMin=0.;
	double SecondMax=0.;
	double SecondInterval=0.;
	bool  SecondLog=false;
	double FirstMin=0.;
	double FirstMax=0.;
	double FirstInterval=0.;
	bool FirstLog=false;

};
struct PG_TIMECONTROL
{
	bool MeanValueBand;
	bool YearBand;
	long YearStartIndex;
	long StartIndex;
};

class Doc;
struct PLOTv;

class LoadDataToChart :public DistributionCurves
{
	public:

		LoadDataToChart(void);
		~LoadDataToChart(void);

		bool LoadTimeSerieData();
		CONTOURCHART m_Contour;
		BOXCOX m_BoxCox;
		AGGREGATING m_Agg;

	protected:
		TIMEDATA m_Time;
		Doc *m_pSimDoc;
		CPG *m_pPG_Active;
		void AdjustForResiduals(double &v, CResiduals *pRes, long ResRunNo, int IndexVal, int LogLiVersion, long index, bool MeasuredAsSource=false);
		void AdjustForMeanOfResiduals(double &v, CResiduals *pRes, bool Accepted, int IndexVal, int LogLiVersion, long index, int TypeStat);
		void AdjustForMeanOfResidualsBetweenYear(double &v, CResiduals *pRes, bool Accepted, int IndexVal, int LogLiVersion, long index, int TypeStat);
		bool TransformSelectedVariables(PLOTv p_info, size_t icurve, size_t i_band);
		bool LoadSelectedVariables(int j, size_t ich, size_t i_band, CURVEINFO c_info, PLOTv info);

		bool CurveData_MultiRun(size_t icurve, PLOTv p_info);
		bool CurveData_SingleTime(size_t icurve, size_t i_band, CURVEINFO c_info, PLOTv p_info);
		bool CurveData_MultiPG(size_t icurve, PLOTv p_info);

		CURVEINFO OpenCurvePG_File(PLOTv p_info, int j = 0);
		bool CheckandOpenPGFile(PLOTv plotinfo);

		void CloseActivePGFile();

		void SetChartInfo(CURVEINFO c_info, PLOTv);
		void AllocateSpaceToData(size_t icurve);
		void AddExtraValidSerie(size_t icurve);
		void SetDataSerie(size_t icurve);
		void SetTimeSerie();
		void SetBandSerie(size_t i_band);
		bool m_EstimateNewDist;

	private:

		PG_TIMECONTROL AdjustForNormalTimeControl(size_t icurve);
		PG_TIMECONTROL AdjustForWithinYearControl(PLOTv p_info);
		PG_TIMECONTROL AdjustForWithinDayControl(PLOTv p_info);
		PG_TIMECONTROL AdjustForDisableControl();
		PG_TIMECONTROL AdjustForSpecificPeriodControl(PLOTv p_info);

		bool SetIndependentSingleTime(PLOTv p_info);
		bool SetIndependentMeasuredValidVar(PLOTv p_info, CURVEINFO c_info, size_t icurve);

		size_t GetNumTimePoints(PG_TIMECONTROL *ptimecontrol, size_t icurve, PLOTv p_info);
		bool SetTimeDataValues(size_t ActNoPoints, PG_TIMECONTROL pg_control);
		bool SetDataValuesForYearSelection(size_t ActNoPoints, PG_TIMECONTROL pg_control, size_t icurve, int PGIndex);

		void SetTimeDataMinMax(int Type);

		void ReSizeBandDataVectors(size_t iband, size_t ActNoPoints);

		bool SetDataValues(size_t ActNoPoints, PG_TIMECONTROL pg_control, size_t icurve, size_t i_band, CURVEINFO c_info);
		void SetMeanDataValues(size_t ActNoPoints, PG_TIMECONTROL pg_control, size_t icurve, size_t i_band, CURVEINFO c_info);


		void SetAllRangeValue(size_t icurve, PG_TIMECONTROL pg_control, size_t i, int PGindex, size_t EnsembleNo=0);
		void SetAllRangeBandValue(size_t icurve, PG_TIMECONTROL pg_control, size_t i, int PGindex, size_t EnsembleNo = 0);
		void SetAcceptedRangeValue(size_t icurve, PG_TIMECONTROL pg_control, size_t i, int PGindex, size_t EnsembleNo = 0);
		void SetAcceptedRangeBandValue(size_t icurve, PG_TIMECONTROL pg_control, size_t i, int PGindex, size_t EnsembleNo = 0);

		void SetDataValue(size_t icurve, PG_TIMECONTROL pg_control, size_t index, int PGindex);

		void SetDataValueMissing(size_t icurve, PG_TIMECONTROL pg_control, size_t index);
		void SetDataBandValueMissing(size_t i_band, PG_TIMECONTROL pg_control, size_t index);
		double GetDataFromPGFile(PG_TIMECONTROL pg_control, size_t index, int PGindex);



};