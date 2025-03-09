
#include "DistributionCurves.h"
#include "../../Util.h"
#include "../ChartProperties\ChartProp.h"

DistributionCurves::DistributionCurves(void)
{

	m_pChartCug=MySingle::instance().GetChartCugPointer();
	m_pProp=MySingle::instance().GetChartPropPointer();
	m_pUtil=MySingle::instance().GetChartUtilPointer();
	m_ChartInfo.NumVariables = 0;
	
}


DistributionCurves::~DistributionCurves(void)
{
}
size_t DistributionCurves::GetAdjustedVariableIndex(size_t org_index)
{
	if (m_ChartInfo.NumVariables != m_ChartInfo.NumPriorVariables&&m_ChartInfo.NumPriorVariables!=0) {
		size_t NumVarPerChartRatio = m_ChartInfo.NumVariables /m_ChartInfo.NumPriorVariables;
		return org_index/NumVarPerChartRatio + (org_index%NumVarPerChartRatio)* m_ChartInfo.NumPriorVariables;
	}
	return org_index;
}
void DistributionCurves::Frequence_SetTime()
{
	if (m_pProp == nullptr) {

		m_pProp = MySingle::instance().GetChartPropPointer();


	}

	auto koll = m_ChartInfo;
		if (m_pProp->IsDistScaleValidSet()) {
			m_pProp->m_TimeScale.MinX = m_pProp->GetActualDistMin();
			m_pProp->m_TimeScale.MaxX = m_pProp->GetActualDistMax();
			if(m_pProp->IsActualDistLogScale()) {
				double maxl=log(m_pProp->m_TimeScale.MaxX);
				double minl=log(m_pProp->m_TimeScale.MinX);
				double halfdist=(maxl-minl)/(m_ChartInfo.NumOfDistPoints-1);
				double Ratio;
				for (int i=0;i<m_ChartInfo.NumOfDistPoints;i++) {
					Ratio=double(i)/double(m_ChartInfo.NumOfDistPoints-1);
					m_pProp->viewPortTimeStamps[i]=exp(Ratio*maxl+(1-Ratio)*minl);
				}
			}
			else {
				double dist=2*(m_pProp->m_TimeScale.MaxX-m_pProp->m_TimeScale.MinX)/m_ChartInfo.NumOfDistPoints;
				for (size_t i=0;i<m_ChartInfo.NumOfDistPoints/2;i++) {
					m_pProp->viewPortTimeStamps[i*2]=m_pProp->m_TimeScale.MinX+dist*i;
					m_pProp->viewPortTimeStamps[i*2+1]=m_pProp->m_TimeScale.MinX+dist*i+dist;
				}
			}

		}

}
void DistributionCurves::Frequence_EstimateDataDist(size_t icurve, size_t TotNumPrior) 
{
		// Calculate Distributions
		for(int j=0;j<m_ChartInfo.NumOfPoints;j++)
			m_pProp->viewPortDataSeries[icurve][j]=0.;
		size_t istep=0;
		auto koll = m_dataSeriesA[icurve][0];

		auto koll1 = m_pProp->viewPortDataSeries[icurve][istep * 2];
		auto koll2 = m_pProp->viewPortDataSeries[icurve][istep * 2 + 1];
		auto max = m_pProp->m_TimeScale.MaxX;
		auto min = m_pProp->m_TimeScale.MinX;
		size_t count=0;





		size_t numsteps=m_pProp->GetXDistSteps(icurve%TotNumPrior);
		for(int j=0;j<m_ChartInfo.NumOfPoints;j++) {
			if(m_dataSeriesA[icurve][j]!=Chart::NoValue&&abs(m_dataSeriesA[icurve][j])<1.E38) {
				double relf;
				double ll;
				ll=log(m_dataSeriesA[icurve][j]);
				if(m_pProp->IsActualDistLogScale())
					relf=(ll-log(m_pProp->m_TimeScale.MinX))/(log(m_pProp->m_TimeScale.MaxX)-log(m_pProp->m_TimeScale.MinX));
				else
					relf=(m_dataSeriesA[icurve][j]-m_pProp->m_TimeScale.MinX)/(m_pProp->m_TimeScale.MaxX-m_pProp->m_TimeScale.MinX);
				relf=min(0.99999999,relf); relf=max(0,relf);
				istep=size_t(relf*numsteps);

				m_pProp->viewPortDataSeries[icurve][istep*2]++;
				m_pProp->viewPortDataSeries[icurve][istep*2+1]++;
			}
			else {
				koll = m_dataSeriesA[icurve][j];

				koll1=m_pProp->viewPortDataSeries[icurve][istep * 2];
				koll2=m_pProp->viewPortDataSeries[icurve][istep * 2 + 1];
				auto koll4 = 0;
				count++;

			}
		}
		if(m_pProp->m_ChartType ==CHART_TYPES::CUM_FREQUENCE) {
			for(int j=1;j<numsteps;j++) {
				m_pProp->viewPortDataSeries[icurve][j*2]+=m_pProp->viewPortDataSeries[icurve][(j-1)*2];
				m_pProp->viewPortDataSeries[icurve][j*2+1]=m_pProp->viewPortDataSeries[icurve][j*2];
			}
			for(int j=0;j<numsteps*2;j++) 
				m_pProp->viewPortDataSeries[icurve][j]=m_pProp->viewPortDataSeries[icurve][j]/m_pProp->viewPortDataSeries[icurve][(numsteps-1)*2+1];				

		}
		m_ChartInfo.NumOfDistPoints=numsteps*2;

}
void DistributionCurves::Frequence_Set_MinMaxNumberOfStepsDistType(size_t icurve) {
	
	m_pProp->SetXDistMin(icurve, m_pProp->m_TimeScale.MinX);	
	m_pProp->SetXDistMax(icurve, m_pProp->m_TimeScale.MaxX);

	int point_set;
	if(m_ChartInfo.NumOfPoints>=1000)
		point_set=100;
	else if(m_ChartInfo.NumOfPoints>=200)
		point_set=20;
	else if(m_ChartInfo.NumOfPoints>=100)
		point_set=10;
	else
		point_set=5;
	if (m_pProp->m_ChartType == CHART_TYPES::CUM_FREQUENCE) point_set = 200;


	m_pProp->SetXDistSteps(icurve,point_set);
	m_pProp->SetXDistLog(icurve, false);

}
void DistributionCurves::Frequence_SetNumberOfPoint_LogScale(size_t icurve) {

	if (m_pProp->IsDistScaleValidSet()) {
		m_pProp->m_TimeScale.MinX=m_pProp->GetActualDistMin();
		m_pProp->m_TimeScale.MaxX=m_pProp->GetActualDistMax();	
		m_pProp->SetActualDistLogScale(m_pProp->IsActualDistLogScale());
	}
	else {
		m_pProp->m_TimeScale.MinX=m_pProp->GetXDistMin(icurve);
		m_pProp->m_TimeScale.MaxX=m_pProp->GetXDistMax(icurve);
		m_pProp->SetActualDistLogScale(m_pProp->isXDistLog(icurve));
	}
	m_ChartInfo.NumOfDistPoints=m_pProp->GetXDistSteps(icurve)*2;
}
