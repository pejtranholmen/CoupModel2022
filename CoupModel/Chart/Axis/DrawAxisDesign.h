#include "../ChartBase.h"
#include "../Legends\DrawVariableLegends.h"

class DrawAxisDesign: public DrawVariableLegends 
{
public:

	DrawAxisDesign(void);
	~DrawAxisDesign(void);

protected:
	void FindandSet_Labels_ForBothXandY_ScatterChart();
	void FindandSet_Labels_ForBothXandY_TimeSerieChart();
	void FindandSet_Labels_ForBothXandY_DistributionChartType();
	void FindandSet_Labels_ForBothXandY_ContourChart();

	void FindandSet_Scale_ForBothXandY(bool ContourChart = false);

	void SetOffsetforTime(double SecondsOfGrid);


	void SetCommonScaleTo_Y_or_Z_Axis();
	bool CheckIfCommonScaleCanBeSet();

	void SelectAndSet_LabelsToAxis();
	void TitleAxisLabels();

	void X_Axis_Scale();
	void Y_Axis_Scale();

	void X_Axis_LabelStyle();
	void Y_Axis_LabelStyle();

	void X_Axis_GetScale();
	void YLeft_Axis_GetScale();
	void YRight_Axis_GetScale();
	void YCommon_Axis_GetScale();


	void X_Axis_SetFormatScale();
	void YLeft_Axis_SetFormatScale();
	void YRight_Axis_SetFormatScale();
	void Z_Axis_SetFormatScale();

	void X_Axis_SetChangedScale();
	void YLeft_Axis_SetChangedScale();
	void YRight_Axis_SetChangedScale();

	void X_Axis_SetScaleValueForChartPropArchive();
	void YLeft_Axis_SetScaleForChartPropArchive();
	void YRight_Axis_SetScaleForChartPropArchive();
	void Z_Axis_SetScaleForChartPropArchive();

	void Adjust_Legends_Axis_Labels();

};
