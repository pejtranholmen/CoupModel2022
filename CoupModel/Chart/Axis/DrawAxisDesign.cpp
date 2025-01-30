#include "../ChartProperties\ChartProp.h"
#include "DrawAxisDesign.h"
#include "chartdir.h"
#include "../../Chart\ChartViewer.h"

DrawAxisDesign::DrawAxisDesign(void)
{
}

DrawAxisDesign::~DrawAxisDesign(void)
{
}

void DrawAxisDesign::FindandSet_Labels_ForBothXandY_ScatterChart()
{
	pXY_Chart->xAxis()->setTitle(m_Label.xtitle.c_str(), "arialbd.ttf", m_pUtil->m_LabelSizePoint);
	FUtil::trim(m_Label.ytitle1);
	pXY_Chart->yAxis()->setTitle(m_Label.ytitle1.c_str(), "arialbd.ttf", m_pUtil->m_LabelSizePoint);
	if (!m_pProp->IsSecondIndepSet()) {
		pXY_Chart->yAxis()->setLabelFormat("{value|G4}");
		pXY_Chart->xAxis()->setLabelFormat("{value|G4}");
		/*if(m_pProp->GetFirstIndepIndex()!=string::npos&&m_pProp->GetIndepFirst()!=PGVARVALID) {
		pXY_Chart->xAxis()->setLinearScale(m_Time.MinDate, m_Time.MaxDate);
		//m_pProp->SetAxisLayer(m_ichart,3);
		}
		else if(m_pProp->GetIndepFirst()==PGVARVALID) {
		pXY_Chart->xAxis()->setLinearScale(Chart0_Min, Chart0_Max);
		m_pProp->SetAxisLayer(m_ichart,3);
		//m_pProp->SetXmin(m_ichart,Chart0_Min);
		//m_pProp->SetXmax(m_ichart,Chart0_Max);

		}
		else  {
		pXY_Chart->xAxis()->setLinearScale(m_Time.MinDate, m_Time.MaxDate);


		};

		pXY_Chart->yAxis()->setLinearScale(Chart0_Min, Chart0_Max);*/


		string newtitle;
		if (m_pProp->GetFirstIndepIndex() != string::npos && !m_pProp->IsFirstIndepSet()) {
			newtitle = "Independent Variable :"; newtitle += m_Label.IndepId;
		}
		else
			newtitle = m_pProp->m_Title;
		if (m_pProp->m_MakeChartTitle)
			pXY_Chart->addTitle(Chart::TopCenter, newtitle.c_str(), "arialbd.ttf", m_pUtil->m_TitleSizePoint);
	}



}

void DrawAxisDesign::FindandSet_Labels_ForBothXandY_TimeSerieChart()
{
	if (m_pProp->m_MakeChartTitle) {
		pXY_Chart->addTitle(Chart::TopCenter, m_pProp->m_Title.c_str(), "arialbd.ttf", m_pUtil->m_TitleSizePoint);
	}

	pXY_Chart->yAxis()->setTitle(m_ChartOrigInfo[m_ChartOrigInfo.size()-1].Y_AxisTitle_Left.c_str(), "arialbd.ttf", m_pUtil->m_LabelSizePoint);
	pXY_Chart->yAxis()->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint);
	if (m_pProp->isXLabelChanged(m_ichart)) {
		pXY_Chart->xAxis()->setTitle(m_ChartOrigInfo[m_ChartOrigInfo.size() - 1].X_AxisTitle.c_str(), "arialbd.ttf", m_pUtil->m_LabelSizePoint);
	}
	else if (m_pUtil->m_MultiRunPlot) {
		if (m_pProp->GetIndepFirst() == NO_USE || m_pProp->GetIndepFirst() == MULTIRUN_NUM)
			pXY_Chart->xAxis()->setTitle("Run Numbers", "arialbd.ttf", m_pUtil->m_LabelSizePoint);
		else if (m_pProp->GetIndepFirst() == PARVAR) {
			string xstr = m_pProp->GetFirstIndepString();
			m_pProp->SetXLabel(m_ichart, xstr.c_str());
			m_Label.xtitle = xstr;
			pXY_Chart->xAxis()->setTitle(xstr.c_str(), "arialbd.ttf", m_pUtil->m_LabelSizePoint);
		}
	}
	else if (m_pProp->m_ReShape.m_NormalDist) {

		pXY_Chart->xAxis()->setTitle("Estimated from Normal Distribution Curve", "arialbd.ttf", m_pUtil->m_LabelSizePoint);

	}
	else if (m_pProp->m_ReShape.m_SortedSeries) {
		pXY_Chart->xAxis()->setTitle("Rank from smallest to highest", "arialbd.ttf", m_pUtil->m_LabelSizePoint);

	}
	if (m_ChartOrigInfo[m_ChartOrigInfo.size() - 1].Y_AxisTitle_right != "") {
		pXY_Chart->yAxis2()->setTitle(m_ChartOrigInfo[m_ChartOrigInfo.size() - 1].Y_AxisTitle_right.c_str(), "arialbd.ttf", m_pUtil->m_LabelSizePoint);
		//->setAlignment(Chart::Right);
		pXY_Chart->yAxis2()->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint);

	}


}

void DrawAxisDesign::FindandSet_Labels_ForBothXandY_DistributionChartType()
{
	if (m_pProp->m_MakeChartTitle)
		pXY_Chart->addTitle(Chart::TopCenter, m_pProp->m_Title.c_str(), "arialbd.ttf", m_pUtil->m_TitleSizePoint);
	pXY_Chart->xAxis()->setTitle(m_Label.xtitle.c_str(), "arialbd.ttf", m_pUtil->m_LabelSizePoint);
	pXY_Chart->yAxis()->setLabelStyle("arialbd.ttf", m_pUtil->m_AxisScalePoint);
	pXY_Chart->xAxis()->setLabelFormat("{value|G}");
	pXY_Chart->yAxis()->setTitle(m_Label.ytitle1.c_str(), "arialbd.ttf", m_pUtil->m_LabelSizePoint);

}

void DrawAxisDesign::FindandSet_Labels_ForBothXandY_ContourChart()
{
	//if (m_pProp->m_ZLabel.size()>m_ichart)

	

	
	if (m_pProp->isZLabelChanged(m_ichart))
		m_ChartOrigInfo[m_ichart].Y_AxisTitle_Left=m_pProp->GetZLabel(m_ichart);

	auto test_title = m_pProp->GetTitle();
	if (test_title.size() == 0&& m_ChartOrigInfo.size()> m_ichart) test_title = m_ChartOrigInfo[m_ichart].Y_AxisTitle_Left;
	pXY_Chart->addTitle(Chart::TopCenter, test_title.c_str(), "arialbd.ttf", m_pUtil->m_TitleSizePoint);
	
	if (!m_pProp->isXLabelChanged(m_ichart)) {
		m_Label.xtitle = m_pProp->GetFirstIndepString();
		m_pProp->SetXLabel(m_ichart, m_Label.xtitle);
	}
	else {
		m_Label.xtitle = m_pProp->GetXLabel(m_ichart);
	}



	pXY_Chart->xAxis()->setTitle(m_Label.xtitle.c_str(), "arialbd.ttf", m_pUtil->m_LabelSizePoint);
	FUtil::trim(m_Label.ytitle1);


	if (!m_pProp->isYLabel1Changed(m_ichart)) {
		m_Label.ytitle1 = m_pProp->GetSecondIndepString();
		m_pProp->SetYLabel1(m_ichart, m_Label.ytitle1);
	}
	else {
		m_Label.ytitle1 = m_pProp->GetYLabel1(m_ichart);
	}


	auto ystr = m_pProp->GetSecondIndepString();
	m_pProp->SetYLabel1(m_ichart, ystr);
	//m_Label.ytitle1=xstr;


	pXY_Chart->yAxis()->setTitle(m_Label.ytitle1.c_str(), "arialbd.ttf", m_pUtil->m_LabelSizePoint);

	if (!m_pProp->isZLabelChanged(m_ichart)&& m_ChartOrigInfo.size()>m_ichart) {
		m_pProp->SetZLabel(m_ichart, m_ChartOrigInfo[m_ichart].varname[0]);
		m_pProp->AddLegend1(m_ChartOrigInfo[m_ichart].varname[0]);
	}
	else {



	}

	m_pUtil->SetZAxisExist();
	//pXY_Chart->xAxis()->setLinearScale(m_Contour.FirstMin, m_Contour.FirstMax);
	//	pXY_Chart->yAxis()->setLinearScale(m_Contour.SecondMin, m_Contour.SecondMax);

}

void DrawAxisDesign::FindandSet_Scale_ForBothXandY(bool ContourChart) {
	
	if (ContourChart) {
		if (!m_pProp->isXScaleChanged(m_ichart)|| !m_pProp->isLeftYScaleChanged(m_ichart)) {
			if (m_pProp->GetIndepFirst() == DATEAXIS) {
				/*if(m_pProp->m_TimeScale.MinX< m_pProp->m_TimeScale.MaxX)
					pXY_Chart->xAxis()->setLinearScale(m_pProp->m_TimeScale.MinX, m_pProp->m_TimeScale.MaxX, m_pProp->m_TimeScale.IntervX);
				else if	(m_Contour.FirstMax> m_Contour.FirstMin) 
					pXY_Chart->xAxis()->setLinearScale(m_Contour.FirstMin, m_Contour.FirstMax, m_Contour.FirstInterval);
					*/



			}
			else {
				pXY_Chart->xAxis()->setLinearScale(m_Contour.FirstMin, m_Contour.FirstMax, m_Contour.FirstInterval);
				pXY_Chart->yAxis()->setLinearScale(m_Contour.SecondMin, m_Contour.SecondMax, m_Contour.SecondInterval);
			}
		}
		else
		{
			pXY_Chart->xAxis()->setLinearScale(m_pProp->GetXmin(0), m_pProp->GetXmax(0), m_pProp->GetXInterv(0));
			pXY_Chart->yAxis()->setLinearScale(m_pProp->GetYLeftmin(0), m_pProp->GetYLeftmax(0), m_pProp->GetYLeftInterv(0));
		}

	}
	else {
		float Chart0_Min, Chart0_Max;
		Chart0_Min = m_pProp->GetXDistMin(m_ichart);
		Chart0_Max = m_pProp->GetXDistMax(m_ichart);
		double cmin = min(Chart0_Min, m_Zoom.viewPortStartDate);
		double cmax = max(Chart0_Max, m_Zoom.viewPortEndDate);

	}


}
void DrawAxisDesign::SetOffsetforTime(double SecondsOfGrid)
{
	
	if (SecondsOfGrid >= 86390.&&SecondsOfGrid<86402) // day give have a day of offset
		pXY_Chart->xAxis()->setLabelOffset(43200);
	else if (SecondsOfGrid>28 * 86400.&&SecondsOfGrid<32 * 86400)  // a month give have a month as offset
		pXY_Chart->xAxis()->setLabelOffset(15.2 * 86400);
	else if (SecondsOfGrid>364. * 86400 && SecondsOfGrid<367 * 86400) // a year give have a year as offset
		pXY_Chart->xAxis()->setLabelOffset(182.6*86400.);
}
void DrawAxisDesign::SetCommonScaleTo_Y_or_Z_Axis()
{

	for (m_ichart = 0; m_ichart < m_pProp->m_NumMultiChart; m_ichart++) {
		if (m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART) {
			if (!m_pProp->isZScaleChanged(m_ichart)) {
				m_pProp->SetZScaleChanged(m_ichart);
				m_pProp->SetZmin(m_ichart, m_pProp->m_CommonScaleValues.min);
				m_pProp->SetZmax(m_ichart, m_pProp->m_CommonScaleValues.max);
				m_pProp->SetZInterv(m_ichart, m_pProp->m_CommonScaleValues.interv);
			}
		}
		else {
			if (!m_pProp->isLeftYScaleChanged(m_ichart)) {
				// m_pProp->SetLeftYScaleChanged(m_ichart);
				m_pProp->SetYLeftmin(m_ichart, m_pProp->m_CommonScaleValues.min);
				m_pProp->SetYLeftmax(m_ichart, m_pProp->m_CommonScaleValues.max);
				m_pProp->SetYLeftInterv(m_ichart,m_pProp->m_CommonScaleValues.interv);
				if (m_pProp->m_ChartType == CHART_TYPES::SCATTER&& m_pProp->m_SyncAxis) {
					m_pProp->SetXScaleChanged(m_ichart);
					m_pProp->SetXmin(m_ichart, m_pProp->m_CommonScaleValues.min);
					m_pProp->SetXmax(m_ichart, m_pProp->m_CommonScaleValues.max);
					m_pProp->SetXInterv(m_ichart, m_pProp->m_CommonScaleValues.interv);

				}
			}
		}
	}


}
bool DrawAxisDesign::CheckIfCommonScaleCanBeSet()
{
	double min_value = -MISSING;
	double max_value = MISSING;
	double interv_value = MISSING;
	bool test = false;
	for (m_ichart = 0; m_ichart < m_pProp->m_NumMultiChart; m_ichart++) {
		if (m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART) {
			if (m_pProp->GetZmin(m_ichart) < min_value)  min_value = m_pProp->GetZmin(m_ichart);
			if (m_pProp->GetZmax(m_ichart) > max_value) max_value = m_pProp->GetZmax(m_ichart);
			if (m_pProp->GetZInterv(m_ichart) > interv_value) interv_value = m_pProp->GetZInterv(m_ichart);
		}
		else {
			if (m_pProp->GetYLeftmin(m_ichart) < min_value)  min_value = m_pProp->GetYLeftmin(m_ichart);
			if (m_pProp->GetYLeftmax(m_ichart) > max_value) max_value = m_pProp->GetYLeftmax(m_ichart);
			if (m_pProp->GetYLeftInterv(m_ichart) > interv_value) interv_value = m_pProp->GetYLeftInterv(m_ichart);
			if (m_pProp->m_ChartType == CHART_TYPES::SCATTER&& m_pProp->m_SyncAxis) {
				if (m_pProp->GetXmin(m_ichart) < min_value)  min_value = m_pProp->GetXmin(m_ichart);
				if (m_pProp->GetXmax(m_ichart) > max_value) max_value = m_pProp->GetXmax(m_ichart);
				if (m_pProp->GetXInterv(m_ichart) > interv_value) interv_value = m_pProp->GetXInterv(m_ichart);
			}
		}
	}
	for (m_ichart = 0; m_ichart < m_pProp->m_NumMultiChart; m_ichart++) {
		if (m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART) {
			if (m_pProp->GetZmin(m_ichart) != min_value)  test = true;
			if (m_pProp->GetZmax(m_ichart) != max_value) test = true;
			if (m_pProp->GetZInterv(m_ichart) != interv_value) test = true;
		}
		else {
			if (m_pProp->GetYLeftmin(m_ichart) != min_value)  test = true;
			if (m_pProp->GetYLeftmax(m_ichart) != max_value) test = true;
			if (m_pProp->GetYLeftInterv(m_ichart) != interv_value) test = true;
		}
	}
	m_pProp->m_CommonScaleValues.min = min_value;
	m_pProp->m_CommonScaleValues.max = max_value;
	m_pProp->m_CommonScaleValues.interv = interv_value;

	return test;




}
void DrawAxisDesign::X_Axis_Scale() {
		// Set the values on the x-scale
	if ((m_pUtil->m_MultiRunPlot&&m_ChartInfo.DataType != 8)&&(m_pProp->m_ChartType ==CHART_TYPES::TIME_SERIE || m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART) && (m_pProp->GetFirstIndepIndex() == string::npos && !m_pProp->IsFirstIndepSet())) {
		pXY_Chart->xAxis()->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint);
		if (m_pProp->m_ReShape.m_SortedSeries&&m_pProp->m_ReShape.m_NormalDist) {
			FindandSet_Scale_ForBothXandY(false);
		}
		else if (m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART) {
			FindandSet_Scale_ForBothXandY(true);
		}
		else if (m_Zoom.viewPortEndDate > m_Zoom.viewPortStartDate) {
			//Normalt time serie plot


		}
		if (m_ChartInfo.NumVariableBands > 0 && !m_pProp->m_ReShape.m_SortedSeries&&m_Layers.pErrLayer!=nullptr) {
			// Set the line width to 2 pixels
			m_Layers.pErrLayer->setLineWidth(m_pUtil->m_LineWidthPoint);
			// Set the error zone to occupy half the space between the symbols
			m_Layers.pErrLayer->setDataGap(1.0);
			m_Layers.pErrLayer->setDataWidth(2);
		}

		else if (!m_pProp->isXScaleChanged(m_ichart)) {
		pXY_Chart->layout();

		m_pProp->m_CommonScale.LeftMin = pXY_Chart->yAxis()->getMinValue();
		m_pProp->m_CommonScale.LeftMax = pXY_Chart->yAxis()->getMaxValue();
		}
	}
	else if ((m_pProp->m_ChartType ==CHART_TYPES::FREQUENCE || m_pProp->m_ChartType ==CHART_TYPES::CUM_FREQUENCE) && (DATA_TYPES(m_ChartInfo.DataType) == DATA_TYPES::SIM_PERFORMANCE|| m_ChartInfo.DataType == 8)) {
		//pXY_Chart->xAxis()->setLogScale(m_pProp->m_TimeScale.MinX, m_pProp->m_TimeScale.MaxX);
		pXY_Chart->xAxis()->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint);
		pXY_Chart->yAxis()->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint);
		if (m_pProp->m_ChartType ==CHART_TYPES::CUM_FREQUENCE) {
			pXY_Chart->yAxis()->setLinearScale(0, 1, .2);
			if (m_pProp->IsActualDistLogScale())
				pXY_Chart->xAxis()->setLogScale(m_pProp->m_TimeScale.MinX, m_pProp->m_TimeScale.MaxX);
			else
				pXY_Chart->xAxis()->setLinearScale(m_pProp->m_TimeScale.MinX, m_pProp->m_TimeScale.MaxX);
		}
		else {
			//if (m_pProp->IsDistScaleValidSet()) {
				if (m_pProp->IsActualDistLogScale())
					pXY_Chart->xAxis()->setLogScale(m_pProp->GetXDistMin(m_ichart), m_pProp->GetXDistMax(m_ichart));
				else {
					pXY_Chart->xAxis()->setRounding(false, false);
				

					pXY_Chart->xAxis()->setLinearScale(m_pProp->GetXDistMin(m_ichart), m_pProp->GetXDistMax(m_ichart));
				}
			//}

		}
		pXY_Chart->xAxis()->setLabelFormat("{value|G}");

		//m_pProp->m_TimeScale.MinX = pXY_Chart->xAxis()->getMinValue();
		//	m_pProp->m_TimeScale.MaxX = pXY_Chart->xAxis()->getMaxValue();
		//m_pProp->m_CommonScale.LeftMin = pXY_Chart->yAxis()->getMinValue();
		//	m_pProp->m_CommonScale.LeftMax = pXY_Chart->yAxis()->getMaxValue();
	}

	else if (m_pProp->m_ChartType ==CHART_TYPES::CUM_FREQUENCE) {
		pXY_Chart->yAxis()->setLinearScale(0, 1.1, .2);
		pXY_Chart->xAxis()->setLinearScale(m_pProp->m_TimeScale.MinX, m_pProp->m_TimeScale.MaxX);
	}
	else if (m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART) {
		FindandSet_Scale_ForBothXandY(true);
 	}
	else if (m_pProp->IsFirstIndepSet()) {

		if (m_pProp->m_ChartType ==CHART_TYPES::SCATTER) {
			pXY_Chart->xAxis()->setAutoScale(0.05, 0.95, 0.99);
		}





	}
	

}
void DrawAxisDesign::Y_Axis_Scale() {
	TextBox *pTextBox;


	pTextBox = pXY_Chart->yAxis()->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint, m_pUtil->m_ColorBackground);
	if (m_pProp->m_ChartType ==CHART_TYPES::SCATTER) {

	}
	else if ((m_pProp->m_ChartType ==CHART_TYPES::FREQUENCE || m_pProp->m_ChartType ==CHART_TYPES::CUM_FREQUENCE) && m_ChartInfo.DataType == 7) {
		DoubleArray YaxisTicks, Yaxis2Ticks;
		int nticks;
		nticks = YaxisTicks.len;
		if (nticks>0) {
			CString txt;
			const char *ptxt;
			StringArray labela;



			ptxt = pXY_Chart->yAxis()->getLabel(YaxisTicks[nticks - 1]);
			int il = sizeof(ptxt);
			labela = StringArray(&ptxt, il);
			//				pXY_Chart->yAxis()->setLabels(YaxisTicks[nticks-1],&ptxt);
			//pTextBox=pXY_Chart->yAxis()->setLabels(YaxisTicks[nticks-1],&ptxt);
			//m_pProp->m_CommonScale.LeftMin = pXY_Chart->yAxis()->getMinValue();
			//	m_pProp->m_CommonScale.LeftMax = pXY_Chart->yAxis()->getMaxValue();

		}
		bool logscale=false;
		if (m_pProp->IsActualDistLogScale()) logscale = true;
		if (logscale) pXY_Chart->xAxis()->setLogScale();
		else pXY_Chart->xAxis()->setLogScale(false);

		// The labels for the area chart


		//m_pProp->m_TimeScale.MinX = pXY_Chart->xAxis()->getMinValue();
		//m_pProp->m_TimeScale.MaxX = pXY_Chart->xAxis()->getMaxValue();
		/*
		if(m_pProp->IsActualDistLogScale()) {
		for( int i=0;i<m_ChartInfo.NumOfPoints;i++) {
		char *str =new char[200];
		labels[i]=str;
		}

		}

		else {
		for( int i=0;i<m_ChartInfo.NumOfPoints;i++) {
		char *str =new char[200];

		float f=float(i)/float((m_ChartInfo.NumOfPoints-1))*(m_pProp->m_TimeScale.MaxX-m_pProp->m_TimeScale.MinX)+m_pProp->m_TimeScale.MinX;
		if(f>=0)
		sprintf(str,"%2G",f);
		else
		sprintf(str,"\\%2G",f);

		labels[i]=str;
		}
		pXY_Chart->xAxis()->setLabels(StringArray(labels, m_ChartInfo.NumOfPoints));
		int minstep;
		if(abs(NoSteps)>50) NoSteps=10;
		if(NoSteps%2==1)
		minstep=min(m_ChartInfo.NumOfPoints/NoSteps,m_ChartInfo.NumOfPoints/5);
		else
		minstep=min(m_ChartInfo.NumOfPoints/NoSteps,m_ChartInfo.NumOfPoints/4);
		pXY_Chart->xAxis()->setLabelStep(minstep);

		}*/
	}
	else if ((m_pProp->m_CommonScale.LeftMin == 0) && (m_pProp->m_CommonScale.LeftMax == 0)) {
		m_pProp->m_CommonScale.LeftInter = 0;
	}


	else if (m_pChartViewer->getZoomDirection() == Chart::DirectionHorizontal)
	{
		// y-axis is auto-scaled - save the chosen y-axis scaled to support xy-zoom mode


		auto axisLowerLimit = m_pProp->m_CommonScale.LeftMin;
		auto axisUpperLimit = m_pProp->m_CommonScale.LeftMax;
	}
	else
	{
		// xy-zoom mode - compute the actual axis scale in the view port 
		auto axisLowerLimit = m_pProp->m_CommonScale.LeftMax - (m_pProp->m_CommonScale.LeftMax - m_pProp->m_CommonScale.LeftMin) *
			(m_pChartViewer->getViewPortTop() + m_pChartViewer->getViewPortHeight());
		auto axisUpperLimit = m_pProp->m_CommonScale.LeftMax - (m_pProp->m_CommonScale.LeftMax - m_pProp->m_CommonScale.LeftMin) *
			m_pChartViewer->getViewPortTop();
	}

}
void DrawAxisDesign::X_Axis_LabelStyle() {
	if ((m_pProp->m_NoXAxisScale&&m_pUtil->GetRemoveXAxisScale() == 1) || m_pUtil->GetRemoveXAxisScale() == 0) {
		TextBox *pTextBox;
		pTextBox = pXY_Chart->xAxis()->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint, m_pUtil->m_ColorBackground);
		//pTextBox->setFontColor(m_ColorBackground);
	}
	else
		pXY_Chart->xAxis()->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint);


}
void DrawAxisDesign::Y_Axis_LabelStyle() {
	if ((m_pProp->m_NoYAxisScale&&m_pUtil->GetRemoveYAxisScale() == 1) || m_pUtil->GetRemoveYAxisScale() == 0) {
		TextBox *pTextBox;
		pTextBox = pXY_Chart->yAxis()->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint, m_pUtil->m_ColorBackground);
		//pTextBox->setFontColor(m_pUtil->m_ColorBackground);
	}
	else {
		pXY_Chart->yAxis()->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint);
		pXY_Chart->yAxis2()->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint);
	}
}
void DrawAxisDesign::X_Axis_SetFormatScale() {
	DoubleArray XaxisTicks;

	// In the current demo, the x-axis range can be from a few years to a few days. We can 
	// let ChartDirector auto-determine the date/time format. However, for more beautiful 
	// formatting, we set up several label formats to be applied at different conditions. 
	//
	if (m_pProp->m_ChartType ==CHART_TYPES::TIME_SERIE && m_pProp->GetFirstIndepVarType() == DATEAXIS) {
		pXY_Chart->xAxis()->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint);
		// If all ticks are yearly aligned, then we use "yyyy" as the label format.
		pXY_Chart->xAxis()->setFormatCondition("align", 360 * 86400);
		if (m_pProp->m_ReShape.m_CommonDayNo)
			pXY_Chart->xAxis()->setLabelFormat("{value|mmm}");
		else if (m_pProp->m_ReShape.m_CommonWithDay)
			pXY_Chart->xAxis()->setLabelFormat("{value|hh}");
		else
			pXY_Chart->xAxis()->setLabelFormat("{value|yyyy}");

		// If all ticks are monthly aligned, then we use "mmm yyyy" in bold font as the first 
		// label of a year, and "mmm" for other labels.
		pXY_Chart->xAxis()->setFormatCondition("align", 30 * 86400);
		if (m_pProp->m_ReShape.m_CommonDayNo)
			pXY_Chart->xAxis()->setMultiFormat(Chart::StartOfYearFilter(), "<*font=bold*>{value|mmm}",
				Chart::AllPassFilter(), "{value|mmm}");
		else
			pXY_Chart->xAxis()->setMultiFormat(Chart::StartOfYearFilter(), "<*font=bold*>{value|mmm yyyy}",
				Chart::AllPassFilter(), "{value|mmm}");

		// If all ticks are daily algined, then we use "mmm dd<*br*>yyyy" in bold font as the 
		// first label of a year, and "mmm dd" in bold font as the first label of a month, and
		// "dd" for other labels.
		pXY_Chart->xAxis()->setFormatCondition("align", 86400);

		pXY_Chart->xAxis()->setMultiFormat(Chart::StartOfYearFilter(),
			"<*block,halign=left*><*font=bold*>{value|mmm dd<*br*>yyyy}",
			Chart::StartOfMonthFilter(), "<*font=bold*>{value|mmm dd}");

		pXY_Chart->xAxis()->setMultiFormat(Chart::AllPassFilter(), "{value|dd}");

		// For all other cases (sub-daily ticks), use "hh:nn<*br*>mmm dd" for the first label of
		// a day, and "hh:nn" for other labels.
		pXY_Chart->xAxis()->setFormatCondition("else");
		if (m_pProp->m_ReShape.m_CommonWithDay)
			pXY_Chart->xAxis()->setMultiFormat(Chart::StartOfDayFilter(),
				"<*font=bold*>{value|hh:nn<*br*>}", Chart::AllPassFilter(), "{value|hh:nn}");
		else
			pXY_Chart->xAxis()->setMultiFormat(Chart::StartOfDayFilter(),
				"<*font=bold*>{value|hh:nn<*br*>mmm dd}", Chart::AllPassFilter(), "{value|hh:nn}");



	}
	else if (m_pProp->m_ChartType == CHART_TYPES::CONTOUR_CHART && m_pProp->GetFirstIndepVarType() == DATEAXIS) {
		pXY_Chart->xAxis()->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint);
			// If all ticks are yearly aligned, then we use "yyyy" as the label format.
			pXY_Chart->xAxis()->setFormatCondition("align", 360 * 86400);
			if (m_pProp->m_ReShape.m_CommonDayNo)
				pXY_Chart->xAxis()->setLabelFormat("{value|mmm}");
			else if (m_pProp->m_ReShape.m_CommonWithDay)
				pXY_Chart->xAxis()->setLabelFormat("{value|hh}");
			else
				pXY_Chart->xAxis()->setLabelFormat("{value|yyyy}");

	



	}


	if (!m_pProp->isXScaleChanged(m_ichart) && !m_pProp->m_ReShape.m_CommonScale && (m_pProp->m_ChartType ==CHART_TYPES::SCATTER || m_pProp->GetFirstIndepIndex() != string::npos || m_pProp->IsFirstIndepSet())) {
		if (m_pProp->IsFirstIndepSet() && !m_pProp->isXScaleChanged(0) && m_pProp->m_ChartType !=CHART_TYPES::CONTOUR_CHART)
			pXY_Chart->xAxis()->setAutoScale(m_pUtil->m_X_AxisAutoScaleValue, m_pUtil->m_X_AxisAutoScaleValue,0.);
		/*else  if (m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART && !m_Contour.ScaleContourChart)
			pXY_Chart->xAxis()->setAutoScale();
		else
			pXY_Chart->xAxis()->setAutoScale();*/
	}
}
void DrawAxisDesign::YRight_Axis_SetFormatScale() {
	pXY_Chart->yAxis2()->setLabelFormat("{value|G2}");
	pXY_Chart->yAxis2()->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint);
	
	if (!m_pProp->isRightYScaleChanged(m_ichart) && !m_pProp->m_ReShape.m_CommonScale)
		pXY_Chart->yAxis2()->setAutoScale(0.005, 0.005, 0.2);
}
void DrawAxisDesign::Z_Axis_SetFormatScale()
{

	m_pColorAxis[m_ichart]->setLabelFormat("{value|G2}");
	m_pColorAxis[m_ichart]->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint);
	if (!m_pProp->isZScaleChanged(m_ichart) && !m_pProp->m_ReShape.m_CommonScale && !m_Contour.ScaleContourChart)
		m_pColorAxis[m_ichart]->setAutoScale(0.05, 0.05, 0.2);
	else if (!m_pProp->m_ReShape.m_CommonScale) {
		m_pColorAxis[m_ichart]->setLinearScale(m_pProp->GetZmin(m_ichart), m_pProp->GetZmax(m_ichart), m_pProp->GetZInterv(m_ichart));
	}
	else if (m_pProp->m_ReShape.m_CommonScale) {
		m_pColorAxis[m_ichart]->setLinearScale(m_pProp->m_CommonScaleValues.min, m_pProp->m_CommonScaleValues.max, m_pProp->m_CommonScaleValues.interv);

	}

}
void DrawAxisDesign::YLeft_Axis_SetFormatScale() {

	auto YaxisTicks = pXY_Chart->yAxis()->getTicks();
	if (m_pProp->m_ChartType != CHART_TYPES::CUM_FREQUENCE)
		pXY_Chart->yAxis()->setRounding(true, true);
	pXY_Chart->yAxis()->setLabelFormat("{value|G4}");
	pXY_Chart->yAxis()->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint);


	if (!m_pProp->isLeftYScaleChanged(m_ichart) && !m_pProp->m_ReShape.m_CommonScale && m_pProp->m_ChartType != CHART_TYPES::CUM_FREQUENCE && m_pProp->m_ChartType != CHART_TYPES::CONTOUR_CHART)
		pXY_Chart->yAxis()->setAutoScale(0.05, 0.05, 0.2);
	else if (m_pProp->m_ReShape.m_CommonScale) {
		pXY_Chart->yAxis()->setLinearScale(m_pProp->m_CommonScaleValues.min, m_pProp->m_CommonScaleValues.max, m_pProp->m_CommonScaleValues.interv);
	}


}
void DrawAxisDesign::YLeft_Axis_SetScaleForChartPropArchive() {
	auto YaxisTicks = pXY_Chart->yAxis()->getTicks();
	int n = YaxisTicks.len;
	if (n >= 2) {
		auto ymax = YaxisTicks[n - 1];
		auto yinterv = YaxisTicks[1] - YaxisTicks[0];

		m_pProp->SetYLeftmin(m_ichart, YaxisTicks[0]);
			m_pProp->SetYLeftmax(m_ichart, ymax);
			m_pProp->SetYLeftInterv(m_ichart, yinterv);
	}
}
void DrawAxisDesign::YRight_Axis_SetScaleForChartPropArchive() {
	auto YaxisTicks = pXY_Chart->yAxis2()->getTicks();
	int n = YaxisTicks.len;
	if (n >= 2) {
		m_pProp->SetYRightmin(m_ichart, YaxisTicks[0]);
		m_pProp->SetYRightmax(m_ichart, YaxisTicks[n - 1]);
		m_pProp->SetYRightInterv(m_ichart, YaxisTicks[1] - YaxisTicks[0]);
	}
}
void DrawAxisDesign::Z_Axis_SetScaleForChartPropArchive() {
	if (m_pColorAxis.size() <= m_ichart) {
		m_pColorAxis.resize(m_ichart + 1);
		return;
	}
	if (m_pProp->m_ChartType !=CHART_TYPES::CONTOUR_CHART) return;
	if (m_pColorAxis[0] == nullptr) return;
	auto ZaxisTicks = m_pColorAxis[m_ichart]->getTicks();
	if (!m_pProp->isZScaleChanged(m_ichart)) {
		int ntic = ZaxisTicks.len;
		auto zminvalue = m_pColorAxis[m_ichart]->getMinValue();
		auto zmaxvalue = m_pColorAxis[m_ichart]->getMaxValue();
		if (ntic >= 2) {
			auto ztickvalue = ZaxisTicks[1] - ZaxisTicks[0];
			m_pProp->SetZmin(m_ichart, zminvalue);
			m_pProp->SetZmax(m_ichart, zmaxvalue);
			m_pProp->SetZInterv(m_ichart, ztickvalue);
		}
	}


}
void DrawAxisDesign::Adjust_Legends_Axis_Labels()
{
	string title_left, title_right;


	//for (size_t i = 0; i < m_ChartOrigInfo.size(); i++) {
		size_t i = m_ChartOrigInfo.size() - 1;
		auto subcharts=m_pProp->GetNumSubCharts();
		m_ChartOrigInfo[i].title = "";
// Check if the Legends are the same for all variables
		string koll="";  bool common = true;
		for (size_t index = 0; index < m_ChartOrigInfo[i].legends.size(); index++) {
			if (index == 00) koll = m_ChartOrigInfo[i].legends[index];
			else if (koll != m_ChartOrigInfo[i].legends[index]) common = false;
		}
		if (common) {
			if(i==0||m_pProp->GetNumSubCharts()>=i) m_ChartOrigInfo[i].title += koll;  // Add only to first chart heading
			for (size_t index = 0; index < m_ChartOrigInfo[i].legends.size(); index++)
				m_ChartOrigInfo[i].legends[index] = "";

		}
		auto test_title=m_pProp->GetTitle();
		if(test_title.size()==0||test_title!= m_ChartOrigInfo[i].title)
			m_pProp->SetTitle(m_ChartOrigInfo[i].title);

		common = true;
		for (size_t index = 0; index < m_ChartOrigInfo[i].legends.size(); index++) {
			if (index == 00) koll = m_ChartOrigInfo[i].varname[index];
			else if (koll == m_ChartOrigInfo[i].varname[index]) common = false;
		}
		bool titlevariable_substituted = false;
		if (common && m_ChartOrigInfo[i].legends.size() > 2) {
			m_ChartOrigInfo[i].Y_AxisTitle_Left = "Variable ()";
			titlevariable_substituted = true;
		}
		else if (common && m_ChartOrigInfo[i].legends.size() > 1) {
			titlevariable_substituted = true;
		}
		else {
			title_left = m_ChartOrigInfo[i].Y_AxisTitle_Left;
		}

		title_right = m_ChartOrigInfo[i].Y_AxisTitle_right;
	
		for (size_t index = 0; index < m_ChartOrigInfo[i].legends.size(); index++) {
			if (index == 0) {
   				if(titlevariable_substituted) m_ChartOrigInfo[i].legends[index]= m_ChartOrigInfo[i].varname[index];
			}
			else if (title_right.size() > 0 && index == 1) {
				m_ChartOrigInfo[i].legends[index] = m_ChartOrigInfo[i].varname[index] +" "+ m_ChartOrigInfo[i].legends[index];

			}
			else {
				if(title_left.find(m_ChartOrigInfo[i].varname[index])==string::npos)
				m_ChartOrigInfo[i].legends[index] = m_ChartOrigInfo[i].varname[index] + " " + m_ChartOrigInfo[i].legends[index];
			};
		 }

}
void DrawAxisDesign::X_Axis_SetScaleValueForChartPropArchive() {


	//Sets the value of the X-scale to the ChartProp Arhive with the point m_pProp
	// based on the ticks that are obtained from the chart pXY_Chart.

	auto XaxisTicks = pXY_Chart->xAxis()->getTicks();
	int ntic = XaxisTicks.len;


	if (ntic > 1 && m_pProp->m_ChartType != CHART_TYPES::SCATTER && (m_pProp->GetFirstIndepIndex() == string::npos || m_pProp->GetIndepFirst() == DATEAXIS) && !(m_pUtil->m_MultiRunPlot || m_pUtil->m_MultiPGPlot)) {
		// axis with date/time
		// find good label offset depending on the lengths of the interval between the ticks
		SetOffsetforTime(XaxisTicks[1] - XaxisTicks[0]);
	}
	if (ntic > 1) {
		m_pProp->SetXmin(m_ichart, XaxisTicks[0]);
		m_pProp->SetXmax(m_ichart, XaxisTicks[ntic - 1]);
		m_pProp->SetXInterv(m_ichart, XaxisTicks[1] - XaxisTicks[0]);
		if (m_pProp->m_ChartType == CHART_TYPES::CONTOUR_CHART && m_pProp->GetIndepFirst() == DATEAXIS) {
			m_pProp->m_TimeScale.MinX = XaxisTicks[0];
			m_pProp->m_TimeScale.MaxX = XaxisTicks[ntic - 1];
			m_pProp->m_TimeScale.IntervX = XaxisTicks[1] - XaxisTicks[0];

		}
		else if (m_pProp->m_ChartType == CHART_TYPES::CONTOUR_CHART) {
			m_Contour.FirstMin = XaxisTicks[0];
			m_Contour.FirstMax = XaxisTicks[ntic - 1];

		}
	}


	if (!m_pProp->isXScaleSet(m_ichart) && (m_pProp->m_ChartType == CHART_TYPES::FREQUENCE || m_pProp->m_ChartType == CHART_TYPES::CUM_FREQUENCE || m_pProp->m_ReShape.m_NormalDist || m_pProp->m_ReShape.m_SortedSeries)) {

		//if(m_pProp->m_ChartType!=4) {
		m_pProp->m_TimeScale.MinX = pXY_Chart->xAxis()->getMinValue();
		m_pProp->m_TimeScale.MaxX = pXY_Chart->xAxis()->getMaxValue();
		m_pProp->SetXmin(m_ichart, m_pProp->m_TimeScale.MinX);

		m_pProp->SetXmax(m_ichart, m_pProp->m_TimeScale.MaxX);
		float v;
		if (ntic >= 2) {
			v = XaxisTicks[1] - XaxisTicks[0];
			m_pProp->SetXInterv(m_ichart, v);
		}
	}
	else if (!m_pProp->isXScaleSet(m_ichart) && m_pProp->m_ChartType == CHART_TYPES::SCATTER) {
		float v;
		if (ntic >= 2) {
			v = XaxisTicks[1] - XaxisTicks[0];
			m_pProp->SetXInterv(m_ichart, v);
		}
		//pXY_Chart->xAxis()->setLinearScale(m, m_pProp->m_TimeScale.MaxX);
	}
	else if (m_pProp->m_ChartType == CHART_TYPES::TIME_SERIE) {
		m_pProp->m_TimeScale.MinX = pXY_Chart->xAxis()->getMinValue();
		m_pProp->m_TimeScale.MaxX = pXY_Chart->xAxis()->getMaxValue();
		float v;
		if (ntic >= 2) {
			m_pProp->m_TimeScale.interv = XaxisTicks[1] - XaxisTicks[0];
		}
		//	m_pProp->SetXmin(m_ichart, m_pProp->m_TimeScale.MinX);
		//	m_pProp->SetXmax(m_ichart, m_pProp->m_TimeScale.MaxX);
		//	m_pProp->SetXInterv(m_ichart, m_pProp->m_TimeScale.interv);


	}
	else if (!m_pProp->isXScaleSet(m_ichart) && m_pProp->m_ChartType == CHART_TYPES::CONTOUR_CHART&&m_pProp->GetIndepFirst() == DATEAXIS) {
		m_pProp->m_TimeScale.MinX = pXY_Chart->xAxis()->getMinValue();
		m_pProp->m_TimeScale.MaxX = pXY_Chart->xAxis()->getMaxValue();
		m_pProp->SetXmin(m_ichart, m_pProp->m_TimeScale.MinX);
		m_pProp->SetXmax(m_ichart, m_pProp->m_TimeScale.MaxX);

		if (ntic >= 2) {
			m_Contour.FirstInterval = XaxisTicks[1] - XaxisTicks[0];
			m_pProp->SetXInterv(m_ichart, m_Contour.FirstInterval);
		}
		else {
			if (m_pProp->m_TimeScale.MaxX - m_pProp->m_TimeScale.MinX > 1440. * 60.)
				m_pProp->m_TimeScale.IntervX = (m_pProp->m_TimeScale.MaxX - m_pProp->m_TimeScale.MinX) / 12.;
			else
				m_pProp->m_TimeScale.IntervX = (m_pProp->m_TimeScale.MaxX - m_pProp->m_TimeScale.MinX) / 10.;
		}
	}
	else if(m_pProp->m_ChartType!=CHART_TYPES::SCATTER) {
		m_pProp->SetXmin(m_ichart, m_Contour.FirstMin);
		m_pProp->SetXmax(m_ichart, m_Contour.FirstMax);
		if (ntic >= 2)	m_Contour.FirstInterval = XaxisTicks[1] - XaxisTicks[0];
		m_pProp->SetXInterv(m_ichart, m_Contour.FirstInterval);


	}


}
void DrawAxisDesign::X_Axis_GetScale() {
		if (m_pProp->IsFirstIndepSet()) {
			m_pProp->m_TimeScale.MinX = m_pProp->GetXmin(0);
			m_pProp->m_TimeScale.MaxX = m_pProp->GetXmax(0);
			m_pProp->m_TimeScale.IntervX = m_pProp->GetXInterv(0);
		}
		else {
			m_pProp->m_TimeScale.MinX = m_pProp->GetXmin(m_ichart);
			m_pProp->m_TimeScale.MaxX = m_pProp->GetXmax(m_ichart);
			m_pProp->m_TimeScale.IntervX = m_pProp->GetXInterv(m_ichart);
		}





/*	if (m_pProp->m_ChartType ==CHART_TYPES::TIME_SERIE && !m_pProp->m_ReShape.m_SortedSeries && !m_pProp->m_ReShape.m_NormalDist&&m_pProp->GetFirstIndepIndex() == string::npos && !m_pProp->IsFirstIndepSet()) {
		m_pProp->m_TimeScale.MinX = m_pProp->m_TimeScale.min;
		m_pProp->m_TimeScale.MaxX = m_pProp->m_TimeScale.max;
		m_pProp->m_TimeScale.IntervX = m_pProp->m_TimeScale.interv;

	}
	else {
		if (m_pProp->GetIndepFirst() == DATEAXIS) {
			/*	m_pProp->m_TimeScale.min = m_pProp->GetXmin(0);
			m_pProp->m_TimeScale.max = m_pProp->GetXmax(0);
			m_pProp->m_TimeScale.interv= m_pProp->GetXInterv(0);


		}
		else if (m_pProp->IsFirstIndepSet()) {
			m_pProp->m_TimeScale.MinX = m_pProp->GetXmin(0);
			m_pProp->m_TimeScale.MaxX = m_pProp->GetXmax(0);
			m_pProp->m_TimeScale.IntervX = m_pProp->GetXInterv(0);
		}
		else {
			m_pProp->m_TimeScale.MinX = m_pProp->GetXmin(m_ichart);
			m_pProp->m_TimeScale.MaxX = m_pProp->GetXmax(m_ichart);
			m_pProp->m_TimeScale.IntervX = m_pProp->GetXInterv(m_ichart);
		}
	}*/
}
void DrawAxisDesign::YLeft_Axis_GetScale() {
	m_pProp->m_CommonScale.LeftMin = m_pProp->GetYLeftmin(m_ichart);
	m_pProp->m_CommonScale.LeftMax = m_pProp->GetYLeftmax(m_ichart);
	m_pProp->m_CommonScale.LeftInter = m_pProp->GetYLeftInterv(m_ichart);
}
void DrawAxisDesign::YRight_Axis_GetScale() {
	m_pProp->m_CommonScale.RightMin = m_pProp->GetYRightmin(m_ichart);
	m_pProp->m_CommonScale.RightMax = m_pProp->GetYRightmax(m_ichart);
	m_pProp->m_CommonScale.RightInter = m_pProp->GetYRightInterv(m_ichart);

}
void DrawAxisDesign::YCommon_Axis_GetScale() {
	m_pProp->m_CommonScale.LeftMin = min(m_pProp->m_CommonScale.LeftMin, m_pProp->m_CommonScale.RightMin);
	m_pProp->m_CommonScale.LeftMax = max(m_pProp->m_CommonScale.LeftMax, m_pProp->m_CommonScale.RightMax);
	if ((m_pProp->m_CommonScale.LeftMax - m_pProp->m_CommonScale.LeftMin)>(m_pProp->m_CommonScale.RightMax - m_pProp->m_CommonScale.RightMin))
		m_pProp->m_CommonScale.LeftInter = m_pProp->m_CommonScale.LeftInter;
	else
		m_pProp->m_CommonScale.LeftInter = m_pProp->m_CommonScale.RightInter;

}
void DrawAxisDesign::X_Axis_SetChangedScale() {
	//pXY_Chart->XAxis()->setLogScale(, axisUpperLimit);
	if (m_pUtil->m_MultiRunPlot) {
		pXY_Chart->xAxis()->setLinearScale(m_pProp->GetXmin(m_ichart), m_pProp->GetXmax(m_ichart), m_pProp->GetXInterv(m_ichart));

	}
	else if (m_pProp->m_TimeScale.IntervX > 0)
		pXY_Chart->xAxis()->setLinearScale(m_pProp->m_TimeScale.MinX, m_pProp->m_TimeScale.MaxX, m_pProp->m_TimeScale.IntervX);
	else if (m_pProp->m_TimeScale.interv > 0)
		pXY_Chart->xAxis()->setLinearScale(m_pProp->m_TimeScale.min, m_pProp->m_TimeScale.max, m_pProp->m_TimeScale.interv);
	else {
		m_pProp->m_TimeScale.IntervX = (m_pProp->m_TimeScale.MaxX - m_pProp->m_TimeScale.MinX) / 5.;
		pXY_Chart->xAxis()->setAutoScale(0.05, 0.95, 0.99);
		pXY_Chart->xAxis()->setLinearScale(m_pProp->m_TimeScale.MinX, m_pProp->m_TimeScale.MaxX);

	}

	double xmin = pXY_Chart->xAxis()->getMinValue();
}
void DrawAxisDesign::YLeft_Axis_SetChangedScale() {
	if (m_pProp->m_ChartType ==CHART_TYPES::CUM_FREQUENCE) {

	}
	else if (m_pProp->IsActualDistLogScale()) {
		pXY_Chart->yAxis()->setLogScale(m_pProp->m_CommonScale.LeftMin, m_pProp->m_CommonScale.LeftMax);
		//pXY_Chart->XAxis()->setLogScale(, axisUpperLimit);
	}
	else if (m_pProp->m_CommonScale.LeftInter>0)
		pXY_Chart->yAxis()->setLinearScale(m_pProp->m_CommonScale.LeftMin, m_pProp->m_CommonScale.LeftMax, m_pProp->m_CommonScale.LeftInter);
	else {

		pXY_Chart->yAxis()->setLinearScale(m_pProp->m_CommonScale.LeftMin, m_pProp->m_CommonScale.LeftMax);


	}


}
void DrawAxisDesign::YRight_Axis_SetChangedScale() {
	if (m_pProp->IsActualDistLogScale()) {
		pXY_Chart->yAxis2()->setLogScale(m_pProp->m_CommonScale.RightMin, m_pProp->m_CommonScale.RightMax);
		//pXY_Chart->XAxis()->setLogScale(, axisUpperLimit);
	}
	else if (m_pProp->m_CommonScale.RightInter>0)
		pXY_Chart->yAxis2()->setLinearScale(m_pProp->m_CommonScale.RightMin, m_pProp->m_CommonScale.RightMax, m_pProp->m_CommonScale.RightInter);
	else
		pXY_Chart->yAxis2()->setLinearScale(m_pProp->m_CommonScale.RightMin, m_pProp->m_CommonScale.RightMax);
}
void DrawAxisDesign::SelectAndSet_LabelsToAxis()
{
	if (m_pProp->isYLabel1Changed(m_ichart))
		m_Label.ytitle1 = m_pProp->GetYLabel1(m_ichart);
	else
		m_pProp->SetYLabel1(m_ichart, m_Label.ytitle1.c_str());

	if (m_pProp->isYLabel2Changed(m_ichart))
		m_Label.ytitle2 = m_pProp->GetYLabel2(m_ichart);
	else {
		m_pProp->SetYLabel2(m_ichart, m_Label.ytitle2.c_str());
	}
	if (m_pProp->isXLabelChanged(m_ichart))
		m_Label.xtitle = m_pProp->GetXLabel(m_ichart);
	else {
		m_pProp->SetXLabel(m_ichart, m_Label.xtitle.c_str());
	}

	m_Label.test = "kul";



}
void DrawAxisDesign::TitleAxisLabels() {
	//float Chart0_Min, Chart0_Max;
	//	Chart0_Min=m_pProp->GetXDistMin(m_ichart);
	//	Chart0_Max=m_pProp->GetXDistMax(m_ichart);

	if (m_pProp->MakeLabels()) {
		if (m_pProp->m_ChartType ==CHART_TYPES::SCATTER || (m_pProp->m_ChartType !=CHART_TYPES::CONTOUR_CHART && m_pProp->GetFirstIndepIndex() != string::npos&&m_pProp->GetIndepFirst() != PARVAR)) {
			FindandSet_Labels_ForBothXandY_ScatterChart();
		}
		else if (m_pProp->m_ChartType ==CHART_TYPES::TIME_SERIE) {
			FindandSet_Labels_ForBothXandY_TimeSerieChart();
		}
		else if (m_pProp->m_ChartType ==CHART_TYPES::FREQUENCE || m_pProp->m_ChartType ==CHART_TYPES::CUM_FREQUENCE) {
			FindandSet_Labels_ForBothXandY_DistributionChartType();
		}
		else if (m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART) {
			FindandSet_Labels_ForBothXandY_ContourChart();
		}
	}
	else {
		if (m_pProp->m_ChartType ==CHART_TYPES::SCATTER) {
			auto xVar = m_pProp->GetXVariableNumber(m_ichart);
			m_pProp->SetXVariableNumber(m_ichart, xVar);
			pXY_Chart->yAxis()->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint);
			pXY_Chart->yAxis()->setLabelFormat("{value|E4.1}");
			
			string newtitle = "Independent Variable :"; newtitle += m_Label.IndepId;
			if (m_pProp->m_MakeChartTitle)
				pXY_Chart->addTitle(Chart::TopCenter, newtitle.c_str(), "arialbd.ttf", m_pUtil->m_TitleSizePoint);
		}
		else if (m_pProp->m_ChartType ==CHART_TYPES::TIME_SERIE && m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART) {
			if (m_pProp->m_MakeChartTitle)
				pXY_Chart->addTitle(Chart::TopCenter, m_pProp->m_Title.c_str(), "arialbd.ttf", m_pUtil->m_TitleSizePoint);
			else
				pXY_Chart->addTitle(Chart::TopCenter, "", "arialbd.ttf", m_pUtil->m_TitleSizePoint);
		}
		else if (m_pProp->m_ChartType ==CHART_TYPES::FREQUENCE || m_pProp->m_ChartType ==CHART_TYPES::CUM_FREQUENCE) {
			if (m_pProp->m_MakeChartTitle)
				pXY_Chart->addTitle(Chart::TopCenter, m_pProp->m_Title.c_str(), "arialbd.ttf", m_pUtil->m_TitleSizePoint);
			pXY_Chart->yAxis()->setLabelStyle("arialbd.ttf", m_pUtil->m_AxisScalePoint);

		}
	}

}

