#include "DrawDetails.h"
#include "./ChartProperties\ChartProp.h"
#include "./PlotDesignInfo\PlotStorage.h"
#include "chartdir.h"
#include "../NewBase\Doc.h"
#include "../Chart\ChartViewer.h"
#include "../Cug\ChartProperties\ChartCug.h"


DrawDetails::DrawDetails(void)
{

	
}


DrawDetails::~DrawDetails(void)
{
}

DataSet* DrawDetails::AddDataFirstLeftPlot(int ivar,int TotNumPrior, bool accept_type, PLOTv p_info, int ivarBand,  bool FirstCurve)
{

	int SectionNo=p_info.PGSection;
	int PGindex=p_info.PGFileIndex;


						
	DataSet *pDataSet;
	pDataSet=nullptr;
	int xVar=0;
	if(m_pProp->m_ChartType ==CHART_TYPES::TIME_SERIE) {
		pDataSet=TimeSerieChart( ivar,TotNumPrior, accept_type, p_info, ivarBand,  FirstCurve);
	}
	else if(m_pProp->m_ChartType ==CHART_TYPES::FREQUENCE||m_pProp->m_ChartType ==CHART_TYPES::CUM_FREQUENCE) {
		pDataSet=TimeSerieFrequenceChart( ivar,TotNumPrior, accept_type, p_info, ivarBand, FirstCurve);
	}

	else if(m_pProp->m_ChartType ==CHART_TYPES::SCATTER||m_pProp->IsIndepSet())  {
		pDataSet=TimeSerieScatterChart( ivar,TotNumPrior, accept_type, p_info, ivarBand,  FirstCurve);
	}
	if (m_pProp->m_rightaxis) 
		m_Layers.pLineLayer2->setUseYAxis2();

	return pDataSet;

}


DataSet* DrawDetails::AddDataPlot( int ivar,int TotNumPrior, bool accept_type, PLOTv p_info, int ivarBand,  bool FirstCurve)
{
	int SectionNo=p_info.PGSection;
	int PGindex=p_info.PGFileIndex;
	int xVar;
	DataSet *pDataSet;
		if(m_pProp->m_ChartType ==CHART_TYPES::SCATTER||m_pProp->GetFirstIndepIndex()!=string::npos) {
				if(m_pProp->GetFirstIndepIndex()!=string::npos) {
					if(m_pProp->GetIndepFirst()==PGVARVALID) {
						m_Layers.pScatterLayer=pXY_Chart->addScatterLayer(m_pProp->m_IndepX[ivar],DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints),
							m_Label.st.c_str(),m_Symbol.isymb,m_pUtil->m_SymbolSizePoint,m_Color.ivar);
						pDataSet=m_Layers.pScatterLayer->getDataSet(0);

						if(m_pProp->m_ReShape.m_RegLine)
							m_Layers.pTrendLayer=pXY_Chart->addTrendLayer(m_pProp->m_IndepX[ivar],DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints),
								m_Color.ivar);
					}
					else {
						m_Layers.pScatterLayer=pXY_Chart->addScatterLayer(m_pProp->m_timeStamps,DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints),
							m_Label.st.c_str(),m_Symbol.isymb,m_pUtil->m_SymbolSizePoint,m_Color.ivar);
						pDataSet=m_Layers.pScatterLayer->getDataSet(0);

						if(m_pProp->m_ReShape.m_RegLine)
							m_Layers.pTrendLayer=pXY_Chart->addTrendLayer(m_pProp->m_timeStamps,DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints),
								m_Color.ivar);
					}
					m_DataSets.push_back(pDataSet);

				}
				else {
					xVar=m_pProp->GetXVariableNumber(m_ichart);
					if(xVar<0)
						xVar=0;
					if(xVar!=ivar) {
						m_Layers.pScatterLayer=pXY_Chart->addScatterLayer(DoubleArray(m_pProp->viewPortDataSeries[xVar],m_ChartInfo.NumOfPoints),DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints),
							m_Label.st.c_str(),m_Symbol.isymb,m_pUtil->m_SymbolSizePoint,m_Color.ivar);
						pDataSet=m_Layers.pScatterLayer->getDataSet(0);
						m_DataSets.push_back(pDataSet);
						if(m_pProp->m_ReShape.m_RegLine)
							m_Layers.pTrendLayer=pXY_Chart->addTrendLayer(DoubleArray(m_pProp->viewPortDataSeries[xVar],m_ChartInfo.NumOfPoints),DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints),
								m_Color.ivar);	
						m_Label.ytitle2=m_Label.test;
						if(!m_pUtil->m_MultiRunPlot) m_Label.unit2=m_Label.unit;
					}
					else {
						m_Label.xtitle=m_Label.test+ "("+m_Label.unit+")";
						m_Label.IndepId=m_Label.st;
						m_pProp->SetXLabelChanged(0);
		
					}
				}
		}
		else if(m_pProp->m_ChartType ==CHART_TYPES::FREQUENCE||m_pProp->m_ChartType ==CHART_TYPES::CUM_FREQUENCE) {
					
			if(m_pProp->m_ChartType ==CHART_TYPES::FREQUENCE) {
				if(m_pUtil->m_MultiRunPlot) {
					m_Layers.pAreaLayer=pXY_Chart->addAreaLayer();
					pDataSet=m_Layers.pAreaLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), m_Color.dist,m_Label.st.c_str());
					m_DataSets.push_back(pDataSet);
					m_Label.Legendtxt=m_Label.st+" Accepted";
					m_Layers.pAreaLayer->setXData(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfPoints));
					m_Layers.pAreaLayer->setLineWidth(m_pUtil->m_LineWidthPoint);
					m_DataSets.push_back(pDataSet);
				}
				else
					m_Label.Legendtxt=m_Label.st;
				if(m_pProp->m_PointType[ivar]&&m_pUtil->m_MultiRunPlot) {
					for (size_t i = 0; i < GetNumberOfEnsembles(); i++) {
						string legend=GetEnsembleIdString(i);
						pDataSet = m_Layers.pAreaLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar + TotNumPrior*(i + 1)], m_ChartInfo.NumOfPoints), m_Color.box, legend.c_str());
						m_DataSets.push_back(pDataSet);
						m_Layers.pAreaLayer->setXData(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfPoints));
						m_Layers.pAreaLayer->setLineWidth(m_pUtil->m_LineWidthPoint);
					}

				}


			}
			else {
				if(m_pUtil->m_MultiRunPlot) {
					pDataSet=m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), m_Color.ivar, m_Label.st.c_str());
					m_DataSets.push_back(pDataSet);
					m_Label.Legendtxt=m_Label.st+" Accepted";
				}
				else
					m_Label.Legendtxt=m_Label.st;
							
				if(m_pProp->m_PointType[ivar]&&m_pUtil->m_MultiRunPlot)
					pDataSet=m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar+TotNumPrior], m_ChartInfo.NumOfPoints), m_Color.ivar, m_Label.Legendtxt.c_str());
					m_DataSets.push_back(pDataSet);

			}


		}
		else if((m_Label.test==m_Label.ytitle1||m_pUtil->m_MultiRunPlot)&&!m_pProp->m_rightaxis) {
			// Adding variables to the Left scale

			if(m_pUtil->m_MultiRunPlot&&accept_type)
				m_Label.Legendtxt=m_Label.st+" Accepted";
			else
				m_Label.Legendtxt=m_Label.st;

			//AddLegend1(m_Label.st);
			if(m_pProp->m_PointType[ivar]&&m_pUtil->m_MultiRunPlot) {
					size_t numensembles = 1;
					numensembles = m_pSimDoc->m_MStorage.m_AcceptedRuns.size();
					for (size_t i = 0; i < numensembles; i++) {
						string legend;
						legend = m_pSimDoc->m_MStorage.m_AcceptedIdString[i];
						pDataSet = m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar + TotNumPrior*(i+1)], m_ChartInfo.NumOfPoints),
							m_Color.ivar, legend.c_str());
					}
			}
			if(SectionNo<10000) {
				if(m_pProp->m_ReShape.m_Symbol||m_pProp->m_PointType[ivar]&&(m_pUtil->m_MultiRunPlot&&accept_type)) {
					pDataSet=m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), m_Color.ivar, "All");	
					m_DataSets.push_back(pDataSet);
					pDataSet->setDataSymbol(m_pProp->m_SymbolType[ivar], m_pUtil->m_SymbolSizePoint);
				}
					//m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), m_Color.ivar, m_Label.st)->setDataSymbol(m_SymbolType[ivar], m_pUtil->m_SymbolSizePoint);	
				else if(!m_pUtil->m_MultiRunPlot) {
					pDataSet=m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), m_Color.ivar, m_Label.st.c_str());
					m_DataSets.push_back(pDataSet);
					if(m_pUtil->m_ErrorSignal&&p_info.EnsembleType==ENSEMBLE_TYPE::NO   &&m_Layers.BandLayerExist&&!m_pUtil->m_ResidualPlot) {
							;
							pXY_Chart->addInterLineLayer( m_Layers.pBandLayer->getLine(0),m_Layers.pLineLayer->getLine(1), m_pProp->GetBoxRangeDimmer()+0xff0000,Chart::Transparent);
							pXY_Chart->addInterLineLayer( m_Layers.pBandLayer->getLine(1),m_Layers.pLineLayer->getLine(1), Chart::Transparent, m_pProp->GetBoxRangeDimmer()+0x0000ff);
						}

				}
			}

			if(p_info.EnsembleType > ENSEMBLE_TYPE::NO &&!m_pProp->m_ReShape.m_SortedSeries)  {
				if(m_pProp->m_ChartError>0) {
					m_Layers.pBandLayer = pXY_Chart->addLineLayer();
					

//ArrayMath(DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints )).add(DoubleArray(m_pProp->viewPortDataSeriesB[ivarBand], m_ChartInfo.NumOfPoints))
					if(m_pProp->m_ChartError==3) {
						m_Layers.pBandLayer->addDataSet(ArrayMath(DoubleArray(m_pProp->viewPortDataSeriesP90[ivarBand],m_ChartInfo.NumOfPoints ))
							,m_Color.ivar);	
						m_Layers.pBandLayer->addDataSet(ArrayMath(DoubleArray(m_pProp->viewPortDataSeriesP10[ivarBand],m_ChartInfo.NumOfPoints ))
							,m_Color.ivar, "90 % Interval");	

					}
					else if(m_pProp->m_ChartError==2) {
						m_Layers.pBandLayer->addDataSet(ArrayMath(DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints ))
						.sub(DoubleArray(m_pProp->viewPortDataSeriesB[ivarBand], m_ChartInfo.NumOfPoints)),m_Color.ivar);	
						m_Layers.pBandLayer->addDataSet(ArrayMath(DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints ))
						.add(DoubleArray(m_pProp->viewPortDataSeriesB[ivarBand], m_ChartInfo.NumOfPoints)),m_Color.ivar, "Mean +-StDev");	

					}
					else {

						m_Layers.pBandLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeriesMax[ivarBand],m_ChartInfo.NumOfPoints )
						,m_Color.ivar, "Simulated (min-max)");	
						m_Layers.pBandLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeriesMin[ivarBand],m_ChartInfo.NumOfPoints ),m_Color.ivar);	
					}

					m_Layers.pBandLayer->setXData(DoubleArray(m_pProp->viewPortTimeStamps,m_ChartInfo.NumOfPoints));
					pXY_Chart->addInterLineLayer(m_Layers.pBandLayer->getLine(0), m_Layers.pBandLayer->getLine(1), m_Color.box,m_Color.box);

				}
				else {
				
					m_Layers.pErrLayer = pXY_Chart->addBoxWhiskerLayer(ArrayMath(DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints )).add(DoubleArray(
					m_pProp->viewPortDataSeriesB[ivarBand], m_ChartInfo.NumOfPoints)),	
					ArrayMath(DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints )).sub(DoubleArray(
					m_pProp->viewPortDataSeriesB[ivarBand], m_ChartInfo.NumOfPoints)),
					DoubleArray(m_pProp->viewPortDataSeriesMax[ivarBand],m_ChartInfo.NumOfPoints), 
					DoubleArray(m_pProp->viewPortDataSeriesMin[ivarBand],m_ChartInfo.NumOfPoints),
					DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints),
					m_Color.box,m_Color.range, m_Color.edge ); 
					m_Layers.pErrLayer->setXData(DoubleArray(m_pProp->viewPortTimeStamps,m_ChartInfo.NumOfPoints));
				}
			}

		}
		else if(!m_pProp->m_rightaxis) {
			// Adding Variables to the left scale when not matching either of the two scales
			if(FirstCurve) {
				m_Layers.pLineLayer->setXData(DoubleArray(m_pProp->viewPortTimeStamps,m_ChartInfo.NumOfPoints));
				FirstCurve=false;
			}
			if(m_Label.test!=m_Label.ytitle1) {
				m_Label.st=m_Label.test+m_Label.st;
			}
			m_Label.st=m_Label.st;

						
			if(m_pProp->m_ReShape.m_Symbol) {
				pDataSet=m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), m_Color.ivar, m_Label.st.c_str());	
				pDataSet->setDataSymbol(m_pProp->m_SymbolType[ivar], m_pUtil->m_SymbolSizePoint);
			}
			else
				pDataSet=m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), m_Color.ivar, m_Label.st.c_str());


			m_DataSets.push_back(pDataSet);
			if(m_pProp->m_PointType[ivar]) {
					pXY_Chart->addScatterLayer(DoubleArray(m_pProp->viewPortTimeStamps,m_ChartInfo.NumOfPoints),DoubleArray(m_pProp->viewPortDataSeries[ivar+TotNumPrior],m_ChartInfo.NumOfPoints),
					"Accepted",Chart::DiamondSymbol,m_pUtil->m_SymbolSizePoint,m_Color.ivar);
			}
			if(p_info.EnsembleType>ENSEMBLE_TYPE::NO&&!m_pProp->m_ReShape.m_SortedSeries)  {
				int ivv;
				ivv=0xEF000000+m_Color.ivar;
				m_Layers.pErrLayer2 = pXY_Chart->addBoxWhiskerLayer(ArrayMath(DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints )).add(DoubleArray(
				m_pProp->viewPortDataSeriesB[ivarBand], m_ChartInfo.NumOfPoints)),	
				ArrayMath(DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints )).sub(DoubleArray(
				m_pProp->viewPortDataSeriesB[ivarBand], m_ChartInfo.NumOfPoints)),
				DoubleArray(m_pProp->viewPortDataSeriesMax[ivarBand],m_ChartInfo.NumOfPoints), 
				DoubleArray(m_pProp->viewPortDataSeriesMin[ivarBand],m_ChartInfo.NumOfPoints),
				DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints),
				m_Color.box,m_Color.range, m_Color.edge ); 
				m_Layers.pErrLayer2->setXData(DoubleArray(m_pProp->viewPortTimeStamps,m_ChartInfo.NumOfPoints));
				m_Layers.pErrLayer2->setLineWidth(m_pUtil->m_LineWidthPoint);

			}

		}
		else if(m_pProp->m_rightaxis||(m_Label.unit2==m_Label.unit&&m_pUtil->m_MultiRunPlot)) {
			//m_Label.st=m_Label.test+m_Label.st;//Layer2 
			//AddLegend2(m_Label.st);

			if(m_Label.test!=m_Label.ytitle2) {
				m_Label.st=m_Label.test+m_Label.st;
			}

			m_Layers.pLineLayer2->setXData(DoubleArray(m_pProp->viewPortTimeStamps,m_ChartInfo.NumOfPoints));

			if(m_pProp->m_PointType[ivar]&&m_pUtil->m_MultiRunPlot) {
					pXY_Chart->addScatterLayer(DoubleArray(m_pProp->viewPortTimeStamps,m_ChartInfo.NumOfPoints),DoubleArray(m_pProp->viewPortDataSeries[ivar+TotNumPrior],m_ChartInfo.NumOfPoints),
					"Accepted",Chart::DiamondSymbol,m_pUtil->m_SymbolSizePoint,m_Color.ivar);

			}
			if(m_pProp->m_ReShape.m_Symbol||m_pProp->m_PointType[ivar]) {
				pDataSet=m_Layers.pLineLayer2->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), m_Color.ivar, m_Label.st.c_str());	
				pDataSet->setDataSymbol(m_pProp->m_SymbolType[ivar], m_pUtil->m_SymbolSizePoint);
			}
			else
				pDataSet=m_Layers.pLineLayer2->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), m_Color.ivar, m_Label.st.c_str());
			m_DataSets.push_back(pDataSet);

			if(p_info.EnsembleType > ENSEMBLE_TYPE::NO &&!m_pProp->m_ReShape.m_SortedSeries)  {

				m_Layers.pErrLayer2 = pXY_Chart->addBoxWhiskerLayer(ArrayMath(DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints )).add(DoubleArray(
				m_pProp->viewPortDataSeriesB[ivarBand], m_ChartInfo.NumOfPoints)),	
				ArrayMath(DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints )).sub(DoubleArray(
				m_pProp->viewPortDataSeriesB[ivarBand], m_ChartInfo.NumOfPoints)),
				DoubleArray(m_pProp->viewPortDataSeriesMax[ivarBand],m_ChartInfo.NumOfPoints), 
				DoubleArray(m_pProp->viewPortDataSeriesMin[ivarBand],m_ChartInfo.NumOfPoints),
				DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints),
				m_Color.box,m_Color.range, m_Color.edge ); 
				m_Layers.pErrLayer2->setXData(DoubleArray(m_pProp->viewPortTimeStamps,m_ChartInfo.NumOfPoints));
				m_Layers.pErrLayer2->setUseYAxis2();
			}
			m_Layers.pLineLayer2->setUseYAxis2();
			if(m_Label.ytitle2=="") {
				m_Label.ytitle2=m_Label.test;
				m_Label.unit2=m_Label.unit;

			}
		}
		else {
			// Variable that are not fit to left or right scale are put to left scale
			if(m_Label.test!=m_Label.ytitle1) {
				m_Label.st=m_Label.test+m_Label.st;
			}
			
						
			m_Label.Legendtxt=m_Label.st;

			if(m_pProp->m_PointType[ivar]&&m_pUtil->m_MultiRunPlot&&accept_type) {
					pXY_Chart->addScatterLayer(DoubleArray(m_pProp->viewPortTimeStamps,m_ChartInfo.NumOfPoints),DoubleArray(m_pProp->viewPortDataSeries[ivar+TotNumPrior],m_ChartInfo.NumOfPoints),
					m_Label.Legendtxt.c_str(),Chart::DiamondSymbol,m_pUtil->m_SymbolSizePoint,MFC_Util::GetColorLine(ivar));

			}
			if(m_pProp->m_ReShape.m_Symbol||m_pProp->m_PointType[ivar]) {
				pDataSet=m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), MFC_Util::GetColorLine(ivar), m_Label.st.c_str());	
				pDataSet->setDataSymbol(m_pProp->m_SymbolType[ivar], m_pUtil->m_SymbolSizePoint);
			}
			else
				pDataSet=m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), MFC_Util::GetColorLine(ivar), m_Label.st.c_str());
						
			if(p_info.EnsembleType > ENSEMBLE_TYPE::NO &&!m_pProp->m_ReShape.m_SortedSeries)  {
				m_Layers.pErrLayer2 = pXY_Chart->addBoxWhiskerLayer(ArrayMath(DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints )).add(DoubleArray(
				m_pProp->viewPortDataSeriesB[ivarBand], m_ChartInfo.NumOfPoints)),	
				ArrayMath(DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints )).sub(DoubleArray(
				m_pProp->viewPortDataSeriesB[ivarBand], m_ChartInfo.NumOfPoints)),
				DoubleArray(m_pProp->viewPortDataSeriesMax[ivarBand],m_ChartInfo.NumOfPoints), 
				DoubleArray(m_pProp->viewPortDataSeriesMin[ivarBand],m_ChartInfo.NumOfPoints),
				DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints),
				m_Color.box, m_Color.range, m_Color.edge ); 
				m_Layers.pErrLayer2->setXData(DoubleArray(m_pProp->viewPortTimeStamps,m_ChartInfo.NumOfPoints));
			}



		}
		return pDataSet;
}
DataSet* DrawDetails::TimeSerieChart( int ivar,int TotNumPrior, bool accept_type, PLOTv p_info, int ivarBand,  bool FirstCurve) {
	DataSet* pDataSet;
	if (m_pProp->m_rightaxis || (m_Label.unit2 == m_Label.unit && m_pUtil->m_MultiRunPlot)) {
		//m_Label.st=m_Label.test+m_Label.st;//Layer2 
		//AddLegend2(m_Label.st);

		if (m_Label.test != m_Label.ytitle2) {
			if(m_Label.st.find(m_Label.test)==string::npos)
			m_Label.st = m_Label.test + m_Label.st;
		}

		m_Layers.pLineLayer2->setXData(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfPoints));
		if (p_info.PGSection < 10000) {
			if ((m_pProp->m_ReShape.m_Symbol || m_pProp->m_PointType[ivar]) && m_pUtil->m_MultiRunPlot && accept_type) {
				pDataSet = m_Layers.pLineLayer2->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), m_Color.ivar, "All");
				pDataSet->setDataSymbol(m_pProp->m_SymbolType[ivar], m_pUtil->m_SymbolSizePoint);
			}
			else if (m_pProp->m_ReShape.m_Symbol || m_pProp->m_PointType[ivar] && !m_pUtil->m_MultiRunPlot) {
				pDataSet = m_Layers.pLineLayer2->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), m_Color.ivar, m_Label.st.c_str());
				pDataSet->setDataSymbol(m_pProp->m_SymbolType[ivar], m_pUtil->m_SymbolSizePoint);
			}
			else if (!m_pUtil->m_MultiRunPlot) {
				pDataSet = m_Layers.pLineLayer2->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), m_Color.ivar, m_Label.st.c_str());
			}
			else {
				pDataSet = m_Layers.pLineLayer2->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), m_Color.ivar, m_Label.st.c_str());
			}
			m_DataSets.push_back(pDataSet);
		}
		else {
			pDataSet = nullptr;
		}
		if (p_info.DataType == DATA_TYPES::SIM_VAL) {


		}
		else if (!m_pUtil->m_MultiRunPlot) {
			m_Label.ytitle2 = m_Label.test;
			m_Label.unit2 = m_Label.unit;   //m_pPG->GetVarUnit(PGindex);
		}
		else if (!m_pProp->IsFirstIndepSet()) {
			m_Label.ytitle2 = m_Label.test;
			m_Label.unit2 = m_Label.unit;
			m_Label.xtitle = "Run Numbers";


		}

	}
	else {

		m_Layers.pLineLayer->setXData(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfPoints));
		if (p_info.PGSection < 10000) {
			if ((m_pProp->m_ReShape.m_Symbol || m_pProp->m_PointType[ivar]) && m_pUtil->m_MultiRunPlot && accept_type) {
				pDataSet = m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), m_Color.ivar, "All");
				pDataSet->setDataSymbol(m_pProp->m_SymbolType[ivar], m_pUtil->m_SymbolSizePoint);
			}
			else if (m_pProp->m_ReShape.m_Symbol || m_pProp->m_PointType[ivar] && !m_pUtil->m_MultiRunPlot) {
				pDataSet = m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), m_Color.ivar, m_Label.st.c_str());
				pDataSet->setDataSymbol(m_pProp->m_SymbolType[ivar], m_pUtil->m_SymbolSizePoint);
			}
			else if (!m_pUtil->m_MultiRunPlot) {
				pDataSet = m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), m_Color.ivar, m_Label.st.c_str());
			}
			else {
				pDataSet = m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints), m_Color.ivar, m_Label.st.c_str());
			}
			m_DataSets.push_back(pDataSet);
		}
		else {
			pDataSet = nullptr;
		}

		if (p_info.DataType == DATA_TYPES::SIM_VAL) {


		}
		else if (!m_pUtil->m_MultiRunPlot&&ivar==0) {
			m_Label.ytitle1 = m_Label.test;
			m_Label.unit1 = m_Label.unit;   
		}
		else if (!m_pProp->IsFirstIndepSet()) {
			m_Label.ytitle1 = m_Label.test;
			m_Label.unit1 = m_Label.unit;
			m_Label.xtitle = "Run Numbers";


		}
	}
	
	// Set X 
	m_Layers.pLineLayer->setXData(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfPoints));
	
	if (m_pProp->m_PointType[ivar] && m_pUtil->m_MultiRunPlot) {
		if (m_pUtil->m_MultiRunPlot&&accept_type) {
			size_t numensembles = 1;
			numensembles = m_pSimDoc->m_MStorage.m_AcceptedRuns.size();
			size_t count_to_plot = 0;
			for (size_t i = 0; i < numensembles; i++) {
				string legend;
				if ((m_pSimDoc->m_MStorage.IsToBePlotted(i) && m_pUtil->GetEnsembleViewOption() == 1) || i == m_pSimDoc->m_MStorage.m_ActualEnsemble) {
						count_to_plot++;
						auto color = m_pUtil->GetHColorLine(count_to_plot);
						legend = m_pSimDoc->m_MStorage.m_AcceptedIdString[i];
						pDataSet = m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar + TotNumPrior*(i + 1)], m_ChartInfo.NumOfPoints),
							color, legend.c_str());
						m_DataSets.push_back(pDataSet);
					}
			}
		}
	}



						



		if (m_pProp->m_ReShape.m_ShowMissingValue) {
			double *pMissing;
			pMissing = new double [m_ChartInfo.NumOfPoints];
			double Max;
			
			Max = MISSING;
			for (size_t i = 0; i<m_ChartInfo.NumOfPoints; i++)
				if (m_pProp->viewPortDataSeries[ivar][i]>Max&&m_pProp->viewPortDataSeries[ivar][i] != Chart::NoValue)
					Max = m_pProp->viewPortDataSeries[ivar][i];
			for (size_t i = 0; i<m_ChartInfo.NumOfPoints; i++) {
				if (m_pProp->viewPortDataSeries[ivar][i] == Chart::NoValue) *(pMissing + i) = Max;
				else *(pMissing + i) = 0;

			}
			m_Layers.pAreaLayer = pXY_Chart->addAreaLayer(DoubleArray(pMissing, m_ChartInfo.NumOfPoints), m_pProp->GetBoxDimmer()+ m_Color.ivar, m_Label.st.c_str());
			m_Layers.pAreaLayer->setXData(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfPoints));
			delete [] pMissing;
		


		}


		if(m_pProp->m_ReShape.m_RegLine)
			m_Layers.pTrendLayer=pXY_Chart->addTrendLayer(DoubleArray(m_pProp->viewPortTimeStamps,m_ChartInfo.NumOfPoints),DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints));	
		if(p_info.EnsembleType > ENSEMBLE_TYPE::NO &&!m_pProp->m_ReShape.m_SortedSeries&&p_info.PGSection<20000)  {

		//	m_Color.ivar=m_ColorLine[ivarBand];
			if(m_pProp->m_ChartError>0) {
				m_Layers.pBandLayer = pXY_Chart->addLineLayer();
				m_Color.box=m_pProp->GetBoxDimmer()+m_Color.ivar;
				string labelmulti;
				labelmulti=FUtil::STD_ItoAscii(p_info.EnsembleNo+1);
				
	
//ArrayMath(DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints )).add(DoubleArray(m_pProp->viewPortDataSeriesB[ivarBand], m_ChartInfo.NumOfPoints))
				if(m_pProp->m_ChartError==2) {
					labelmulti = "Mean +- StD";
					m_Layers.pBandLayer->addDataSet(ArrayMath(DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints ))
						.sub(DoubleArray(m_pProp->viewPortDataSeriesB[ivarBand], m_ChartInfo.NumOfPoints)),m_Color.ivar);	
					m_Layers.pBandLayer->addDataSet(ArrayMath(DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints ))
						.add(DoubleArray(m_pProp->viewPortDataSeriesB[ivarBand], m_ChartInfo.NumOfPoints)),m_Color.ivar, labelmulti.c_str());
									
				}
				if(m_pProp->m_ChartError==3) {
					m_Layers.pBandLayer->addDataSet(ArrayMath(DoubleArray(m_pProp->viewPortDataSeriesP90[ivarBand],m_ChartInfo.NumOfPoints ))
						,m_Color.ivar);	
					m_Layers.pBandLayer->addDataSet(ArrayMath(DoubleArray(m_pProp->viewPortDataSeriesP10[ivarBand],m_ChartInfo.NumOfPoints ))
						,m_Color.ivar, "90 % Interval");	
									
				}
				else {
					labelmulti = "";
					m_Layers.pBandLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeriesMax[ivarBand],m_ChartInfo.NumOfPoints )
					,m_Color.ivar, labelmulti.c_str());	
					m_Layers.pBandLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeriesMin[ivarBand],m_ChartInfo.NumOfPoints ),m_Color.ivar);	


				}

				m_Layers.pBandLayer->setXData(DoubleArray(m_pProp->viewPortTimeStamps,m_ChartInfo.NumOfPoints));
				m_Layers.BandLayerExist=true;
				pXY_Chart->addInterLineLayer(m_Layers.pBandLayer->getLine(0), m_Layers.pBandLayer->getLine(1), m_Color.box,m_Color.box);
			}
			else {

					//ivv=m_Color.ivar;
				m_Layers.pErrLayer = pXY_Chart->addBoxWhiskerLayer(ArrayMath(DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints )).add(DoubleArray(
				m_pProp->viewPortDataSeriesB[ivarBand], m_ChartInfo.NumOfPoints)),	
				ArrayMath(DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints )).sub(DoubleArray(
				m_pProp->viewPortDataSeriesB[ivarBand], m_ChartInfo.NumOfPoints)),
				DoubleArray(m_pProp->viewPortDataSeriesMax[ivarBand],m_ChartInfo.NumOfPoints), 
				DoubleArray(m_pProp->viewPortDataSeriesMin[ivarBand],m_ChartInfo.NumOfPoints),
				DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints),
				m_Color.box ,m_Color.range, m_Color.edge ); 

				m_Layers.pErrLayer->setXData(DoubleArray(m_pProp->viewPortTimeStamps,m_ChartInfo.NumOfPoints));
			}
	}

	return pDataSet;

}
DataSet* DrawDetails::TimeSerieFrequenceChart( int ivar,int TotNumPrior, bool accept_type, PLOTv p_info, int ivarBand,  bool FirstCurve) {
	DataSet* pDataSet=nullptr;
	size_t numensembles = 1;
	string legend;
		if(m_pProp->m_ChartType ==CHART_TYPES::FREQUENCE) {
			// Bar Representation
			if(m_pProp->m_PointType[ivar]&&m_pUtil->m_MultiRunPlot||m_ChartInfo.NumVariables>1&&m_pUtil->m_MultiRunPlot) {
				if(accept_type) {

						m_Layers.pAreaLayer = pXY_Chart->addAreaLayer();
						legend = "All";

						pDataSet = m_Layers.pAreaLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfDistPoints), m_Color.dist, legend.c_str());

						m_Layers.pAreaLayer->setXData(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfDistPoints));
						m_Layers.pAreaLayer->setLineWidth(m_pUtil->m_LineWidthPoint);
						m_DataSets.push_back(pDataSet);
					}

				}
				if(m_pProp->m_PointType[ivar]) {
						numensembles = 0;
						if (accept_type) 
							numensembles = m_pSimDoc->m_MStorage.m_AcceptedRuns.size();
						size_t count_to_plot = 0;
						for (size_t i = 0; i < numensembles; i++) {
							if ((m_pSimDoc->m_MStorage.IsToBePlotted(i) && m_pUtil->GetEnsembleViewOption() == 1) || i == m_pSimDoc->m_MStorage.m_ActualEnsemble) {
								count_to_plot++;
								string legend;

								legend = m_pSimDoc->m_MStorage.m_AcceptedIdString[i];

								m_Layers.pAreaLayer = pXY_Chart->addAreaLayer();

								pDataSet = m_Layers.pAreaLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar + TotNumPrior*(i + 1)], m_ChartInfo.NumOfDistPoints), m_Color.box, legend.c_str());
								m_Layers.pAreaLayer->setXData(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfDistPoints));
								m_Layers.pAreaLayer->setLineWidth(m_pUtil->m_LineWidthPoint);
								m_DataSets.push_back(pDataSet);
							}
					}
				}

			else {
				m_Layers.pAreaLayer=pXY_Chart->addAreaLayer();
				legend = m_pProp->GetLastLegend();
				pDataSet=m_Layers.pAreaLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfDistPoints), m_Color.box,legend.c_str());
				m_DataSets.push_back(pDataSet);
				m_Label.Legendtxt=m_Label.st+" Accepted";
				m_Layers.pAreaLayer->setXData(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfDistPoints));
				m_Layers.pAreaLayer->setLineWidth(m_pUtil->m_LineWidthPoint);
			}

		}

		else {
			// Cumulative line representation
			if(m_pUtil->m_MultiRunPlot&&accept_type) {
				
				legend = "All";
				//	auto legend = m_pProp->m_Legend1[m_pProp->m_Legend1.size() - 2];
				pDataSet = m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfDistPoints), m_pUtil->GetHColorLine(0), legend.c_str());
				m_DataSets.push_back(pDataSet);
			}
			else
				m_Label.Legendtxt=m_Label.st;

			//auto legend=m_pProp->m_Legend1[m_pProp->m_Legend1.size()-1];
			if (m_pProp->m_PointType[ivar] && m_pUtil->m_MultiRunPlot) {


				numensembles = m_pSimDoc->m_MStorage.m_AcceptedRuns.size();
				size_t count_to_plot = 0;
				for (size_t i = 0; i < numensembles; i++) {
					if ((m_pSimDoc->m_MStorage.IsToBePlotted(i) && m_pUtil->GetEnsembleViewOption() == 1) || i == m_pSimDoc->m_MStorage.m_ActualEnsemble) {
						count_to_plot++;

						legend = m_pSimDoc->m_MStorage.m_AcceptedIdString[i];

						auto color = m_pUtil->GetHColorLine(count_to_plot);
						pDataSet = m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar + TotNumPrior*(i + 1)], m_ChartInfo.NumOfDistPoints), color, legend.c_str());
						m_DataSets.push_back(pDataSet);
					}
				}



			}
			else {
				pDataSet = m_Layers.pLineLayer->addDataSet(DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfDistPoints), m_Color.ivar, legend.c_str());
				m_DataSets.push_back(pDataSet);

			}
			m_Layers.pLineLayer->setXData(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfPoints));
	
		}
		//m_Label.ytitle1=m_Label.test;	
		if(!m_pUtil->m_MultiRunPlot) 
			m_Label.unit1=m_pPG_Active->GetVarUnit(p_info.PGFileIndex).c_str();
		else
			m_Label.unit1=m_Label.unit;
	return pDataSet;
}
DataSet* DrawDetails::TimeSerieScatterChart( int ivar,int TotNumPrior, bool accept_type, PLOTv p_info, int ivarBand,  bool FirstCurve) {
	DataSet* pDataSet=nullptr;
	int xVar=0;
	size_t numensembles = 1;
	string legend;
	if(m_pProp->GetFirstIndepIndex()==string::npos||m_pProp->IsFirstIndepSet()||(xVar==ivar&&m_pUtil->m_MultiRunPlot)) {
			if(m_pProp->GetIndepFirst()==PGVARVALID||(m_pProp->GetIndepFirst() == VALVARSUM&&m_pUtil->m_AutoShiftFirstIndep)) {
				m_Layers.pScatterLayer=pXY_Chart->addScatterLayer(m_pProp->m_IndepX[ivar],DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints),
					m_Label.st.c_str(),m_Symbol.isymb,m_pUtil->m_SymbolSizePoint,m_Color.ivar);
				pDataSet=m_Layers.pScatterLayer->getDataSet(0);

				if(m_pProp->m_ReShape.m_RegLine)
					m_Layers.pTrendLayer=pXY_Chart->addTrendLayer(m_pProp->m_IndepX[ivar],DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints),
						m_Color.ivar);
				
				size_t count_to_plot = 0;
				
				for (size_t i = 0; i < GetNumberOfEnsembles(); i++) {
					if ((m_pSimDoc->m_MStorage.IsToBePlotted(i) && m_pUtil->GetEnsembleViewOption() == 1) || i == m_pSimDoc->m_MStorage.m_ActualEnsemble) {
						count_to_plot++;
						m_Color.ivar = m_pUtil->GetHColorLine(count_to_plot);;
						string legend = GetEnsembleIdString(i);

						m_Layers.pScatterLayer = pXY_Chart->addScatterLayer(m_pProp->m_IndepX[ivar], DoubleArray(m_pProp->viewPortDataSeries[ivar + TotNumPrior*(i + 1)], m_ChartInfo.NumOfPoints),
							m_Label.st.c_str(), m_Symbol.isymb, m_pUtil->m_SymbolSizePoint, m_Color.ivar);
						pDataSet = m_Layers.pScatterLayer->getDataSet(0);

						if (m_pProp->m_ReShape.m_RegLine)
							m_Layers.pTrendLayer = pXY_Chart->addTrendLayer(m_pProp->m_IndepX[ivar], DoubleArray(m_pProp->viewPortDataSeries[ivar + TotNumPrior*(i + 1)], m_ChartInfo.NumOfPoints),
								m_Color.ivar);

						m_DataSets.push_back(pDataSet);
					}

				}


			}
			else {
				m_Layers.pScatterLayer=pXY_Chart->addScatterLayer(m_pProp->m_timeStamps ,DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints),
					m_Label.st.c_str(),m_Symbol.isymb,m_pUtil->m_SymbolSizePoint,m_Color.ivar);
				pDataSet=m_Layers.pScatterLayer->getDataSet(0);
				size_t count_to_plot = 0;
				//for (size_t i = 0; i < GetNumberOfEnsembles(); i++) {

					//if (m_pSimDoc->m_MStorage.IsToBePlotted(i)) {
						
						string legend = GetEnsembleIdString(p_info.EnsembleNo);
						if(p_info.EnsembleNo==0) 
						count_to_plot++;
						m_Color.ivar = m_pUtil->GetHColorLine(count_to_plot);
						m_Symbol.isymb = count_to_plot;

						if (p_info.EnsembleNo == 0) {
							m_Layers.pScatterLayer = pXY_Chart->addScatterLayer(m_pProp->m_timeStamps, DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints),
								legend.c_str(), m_Symbol.isymb, m_pUtil->m_SymbolSizePoint, m_Color.ivar);

						}
						else {
							m_Layers.pScatterLayer = pXY_Chart->addScatterLayer(m_pProp->m_timeStamps, DoubleArray(m_pProp->viewPortDataSeries[ivar + TotNumPrior * (p_info.EnsembleNo + 1)], m_ChartInfo.NumOfPoints),
								legend.c_str(), m_Symbol.isymb, m_pUtil->m_SymbolSizePoint, m_Color.ivar);
						
						}
						
						pDataSet = m_Layers.pScatterLayer->getDataSet(0);

						if (m_pProp->m_ReShape.m_RegLine)
							m_Layers.pTrendLayer = pXY_Chart->addTrendLayer(m_pProp->m_timeStamps, DoubleArray(m_pProp->viewPortDataSeries[ivar], m_ChartInfo.NumOfPoints),
								m_Color.ivar);

						m_DataSets.push_back(pDataSet);
					//}

				//}

				if(m_pProp->m_ReShape.m_RegLine)
					m_Layers.pTrendLayer=pXY_Chart->addTrendLayer(m_pProp->m_timeStamps ,DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints),
						m_Color.ivar);

			}
			m_DataSets.push_back(pDataSet);
			

				m_Label.ytitle1=m_Label.test;
				if(m_Label.ytitle1.find(m_Label.unit)==string::npos) 
					m_Label.ytitle1 = m_Label.test+ " ("+ m_Label.unit+")";
				m_Label.unit1=m_Label.unit;
				if((m_pProp->GetFirstIndepIndex()!=string::npos&&!m_pUtil->m_AutoShiftFirstIndep )||!m_pProp->IsFirstIndepSet()) {
					m_Label.xtitle=m_pProp->GetFirstIndepString(ivar);
				}

	}
	else {
		if(xVar!=ivar) {
				if(m_pUtil->m_MultiRunPlot) {
					m_Label.ytitle1=" ";
					m_Label.IndepId=m_Label.test+m_Label.st;
				}
				else {
					m_Label.ytitle1=m_Label.test;
					m_Label.unit1=m_Label.unit;
				}
				if(p_info.DataType== DATA_TYPES::SIM_VAL) m_pProp->m_ReShape.m_RegLine=true;
				m_pProp->SetAxisLayer(xVar,3);
				m_Layers.pScatterLayer=pXY_Chart->addScatterLayer(DoubleArray(m_pProp->viewPortDataSeries[xVar],m_ChartInfo.NumOfPoints),DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints),
						m_Label.st.c_str(),m_Symbol.isymb,m_pUtil->m_SymbolSizePoint,m_Color.ivar);
				pDataSet=m_Layers.pScatterLayer->getDataSet(0);
				if(m_pProp->m_ReShape.m_RegLine)
					m_Layers.pTrendLayer=pXY_Chart->addTrendLayer(DoubleArray(m_pProp->viewPortDataSeries[xVar],m_ChartInfo.NumOfPoints),DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints),
							m_Color.ivar);
				m_DataSets.push_back(pDataSet);


				size_t count_to_plot = 0;
					for (size_t i = 0; i < GetNumberOfEnsembles(); i++) {
						if ((m_pSimDoc->m_MStorage.IsToBePlotted(i) && m_pUtil->GetEnsembleViewOption() == 1) || i == m_pSimDoc->m_MStorage.m_ActualEnsemble) {
							count_to_plot++;
							string legend = GetEnsembleIdString(i);
							m_Color.ivar = m_pUtil->GetHColorLine(count_to_plot);
							m_Layers.pScatterLayer = pXY_Chart->addScatterLayer(DoubleArray(m_pProp->viewPortDataSeries[xVar], m_ChartInfo.NumOfPoints), DoubleArray(m_pProp->viewPortDataSeries[ivar + TotNumPrior*(i + 1)], m_ChartInfo.NumOfPoints),
								m_Label.st.c_str(), m_Symbol.isymb, m_pUtil->m_SymbolSizePoint, m_Color.ivar);
							pDataSet = m_Layers.pScatterLayer->getDataSet(0);

							if (m_pProp->m_ReShape.m_RegLine)
								m_Layers.pTrendLayer = pXY_Chart->addTrendLayer(DoubleArray(m_pProp->viewPortDataSeries[xVar], m_ChartInfo.NumOfPoints), DoubleArray(m_pProp->viewPortDataSeries[ivar + TotNumPrior*(i + 1)], m_ChartInfo.NumOfPoints),
									m_Color.ivar);

							m_DataSets.push_back(pDataSet);
						}

					}
		}
		else {
				m_Label.xtitle=m_Label.test+ "("+m_Label.unit+")";
				m_Label.IndepId=m_Label.st;
				m_pProp->SetXLabelChanged(m_ichart);

		}
	}


	return pDataSet;
}
bool DrawDetails::AddData_ToChart(size_t TotNumPrior) {
	int curve_count=-1;
	
	float Chart0_Min, Chart0_Max;
	
	Chart0_Min=1.E38f; 
	Chart0_Max=MISSING;
	bool ChartFilled=false;
	bool CommonColors;
	CommonColors=m_pProp->GetCommonColors();
	size_t i_band=0;
	int ivar=m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);


	m_DataSets.clear();
	m_Label.ytitle2 = "";
	m_Label.ytitle1 = "";


	m_pProp->CheckChartType_Based_On_IndependentVar();
	size_t count_to_plot = 0;
	for(int var_index=0;var_index<m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);var_index++) {
			int j=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, var_index);
			PLOTv p_info = m_pSimDoc->m_PlotStorage.GetPlotInfo(j);
			int ichart=m_pSimDoc->m_PlotStorage.GetChart(j);
  			if(p_info.DataType==DATA_TYPES::MEASURED_VAL&&m_pSimDoc->m_PlotStorage.IsValidIndepPGVar()) continue;
  			DataSet *pDataSet=nullptr;
			if(m_pProp->m_ChartType >=CHART_TYPES::FREQUENCE&&m_pProp->m_ChartType >=CHART_TYPES::CUM_FREQUENCE&&p_info.DataType== DATA_TYPES::SIM_PAR&&m_ichart==p_info.SubChart_No-1) {				
					m_pProp->SetDistScale(float(m_pSimDoc->MR_GetMin(p_info.Index)), float(m_pSimDoc->MR_GetMax(p_info.Index)), m_pSimDoc->MR_GetLogDist(p_info.Index));
					Frequence_SetTime();

					m_pProp->m_timeStamps=DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfDistPoints);
					m_dataSeriesA[var_index]=DoubleArray(m_pProp->viewPortDataSeries[var_index],m_ChartInfo.NumOfDistPoints);
					size_t iens = 0;

					for (size_t i = 0; i < m_pSimDoc->m_MStorage.m_AcceptedRuns.size(); i++) {
						if ((m_pSimDoc->m_MStorage.IsToBePlotted(i)&& m_pUtil->GetEnsembleViewOption()==1)||i== m_pSimDoc->m_MStorage.m_ActualEnsemble) {
							iens++;
							m_dataSeriesA[iens*ivar+var_index] = DoubleArray(m_pProp->viewPortDataSeries[iens*ivar+var_index], m_ChartInfo.NumOfDistPoints);
						}
					}
																																		 			}
			else if(m_pProp->m_ChartType >=CHART_TYPES::FREQUENCE&&m_pProp->m_ChartType <=CHART_TYPES::CUM_FREQUENCE&&m_ichart==p_info.SubChart_No-1) {
					m_pProp->m_TimeScale.MinX=m_pProp->GetXDistMin(var_index%TotNumPrior);
					m_pProp->m_TimeScale.MaxX=m_pProp->GetCommonXDistMax();
					m_pProp->m_TimeScale.MinX =m_pProp->GetCommonXDistMin();
				//	m_pProp->m_ReShape.m_LogScale=m_pProp->isXDistLog(var_index%TotNumPrior);
					m_pProp->SetDistScale(m_pProp->m_TimeScale.MinX,m_pProp->m_TimeScale.MaxX, false);

					Frequence_SetTime();
					m_pProp->m_timeStamps=DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfDistPoints);
					if (var_index == 0) m_dataSeriesA[var_index*ivar] = DoubleArray(m_pProp->viewPortDataSeries[var_index], m_ChartInfo.NumOfDistPoints);
					size_t iens = 0;
					for (size_t i = 0; i < m_pSimDoc->m_MStorage.m_AcceptedRuns.size(); i++) {
						if ((m_pSimDoc->m_MStorage.IsToBePlotted(i) && m_pUtil->GetEnsembleViewOption() == 1) || i == m_pSimDoc->m_MStorage.m_ActualEnsemble) {
							iens++;
							m_dataSeriesA[iens*ivar + var_index] = DoubleArray(m_pProp->viewPortDataSeries[iens*ivar + var_index], m_ChartInfo.NumOfDistPoints);
						}
					}

			}
			int PGindex=m_pSimDoc->m_PlotStorage.GetVar_Index(j);
			int FileNo=	p_info.ValFileNo;
			int SectionNo=p_info.PGSection;// m_pSimDoc->m_PlotStorage.GetPGSectionNo(j);

			if(m_ichart==p_info.SubChart_No-1) {
				ChartFilled=true;
				// To Open PG file to make variable description accessible
				if(!m_pUtil->m_MultiRunPlot){
					SetPointertoActivePGFile(j, p_info);
				}				
   				
				// To Create code of variable Identification (m_Label.st, m_Label.test, m_Label.unit)
				FindIdForVariablesToPlot(j, p_info, PGindex);
				if (p_info.RepitDataType) SetMultiSequenceInfo();
				FUtil::trim(m_Label.test);
				m_CurrentChartInfo.legends.push_back(m_Label.st);
				m_CurrentChartInfo.varname.push_back(m_Label.test);
				m_CurrentChartInfo.varunit.push_back(m_Label.unit);
				m_CurrentChartInfo.X_AxisTitle = m_Label.xtitle;

				curve_count++;

				// To determine if right axis Should be used

				CheckIfRightAxis(p_info.DataType, ivar, curve_count);

				if (curve_count==0) {
					m_Label.ytitle1 = m_Label.test+ " ("+m_Label.unit+")";
					m_CurrentChartInfo.Y_AxisTitle_Left = m_Label.ytitle1;
				}
				else if(m_pProp->m_rightaxis&&m_Label.ytitle2==""){ 
					m_Label.ytitle2 = m_Label.test + " (" + m_Label.unit + ")";
					m_CurrentChartInfo.Y_AxisTitle_right = m_Label.ytitle2;
 				}

				if(m_pProp->m_ChartType<=CHART_TYPES::TIME_SERIE||(m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART&&m_pProp->GetFirstIndepVarType()==DATEAXIS)&&!m_pSimDoc->IsMultiSimulation()) {
					if(m_pProp->GetXDistMin(ivar)<Chart0_Min)
						Chart0_Min=m_pProp->GetXDistMin(ivar);
					if(m_pProp->GetXDistMax(ivar)>Chart0_Max)
						Chart0_Max=m_pProp->GetXDistMax(ivar);
				}

				// To determine the colors for variables	


				if(CommonColors)				
					m_Symbol.isymb=m_pProp->GetSymbolStyle(curve_count);
				else
					m_Symbol.isymb=m_pProp->GetSymbolStyle(var_index);


				if(CommonColors)
					m_Color.ivar=m_pUtil->GetHColorLine(curve_count);
				else
					m_Color.ivar=m_pUtil->GetHColorLine(var_index);

				m_Color.box=m_pProp->GetBoxDimmer()+m_Color.ivar;
				m_Color.range=m_pProp->GetBoxRangeDimmer()+m_Color.ivar;
				m_Color.edge=m_pProp->GetBoxRangeDimmer()+m_pUtil->GetColorBoxEdgeH();
				m_Color.dist=m_pProp->GetBoxDimmer()+m_pUtil->GetColorDistH();
				
				// To make fine tuning of the ID code for variables

				CheckAndAddLabelInfo(m_ichart, p_info.Doc);
				SetSymbolInfo(ivar, curve_count, CommonColors);

				bool accept_type=m_pUtil->m_MultiRunPlot;
				
				if(m_pProp->IsFirstIndepSet()&&m_pProp->IsSecondIndepSet()) {
					p_info.DataType=DATA_TYPES::PGDATA;
					if (!MakeContourPlot(pXY_Chart, m_ichart, var_index)) 
						return false;
					else {
						m_ChartOrigInfo.push_back(m_CurrentChartInfo);
						Adjust_Legends_Axis_Labels();
						SetLegendInfo();
						return true;
					}
				}
				//Create Charts for left and right axis layers
				else  {
					bool FirstChart = false;
					if (m_ichart == 0 || !m_pProp->m_rightaxis) {
						FirstChart = true;
						pDataSet = AddDataFirstLeftPlot(var_index, TotNumPrior, accept_type, p_info, i_band, FirstChart);
					}
					else
						  pDataSet=AddDataPlot(var_index,TotNumPrior, accept_type, p_info, i_band,  false);
				}
				if (pDataSet != nullptr) {

					if ((m_pProp->m_ChartType != CHART_TYPES::TIME_SERIE || m_pProp->m_ReShape.m_AddLine) && m_pProp->GetFirstIndepIndex() == string::npos && !m_pProp->IsFirstIndepSet())
						SetLineWidth(pDataSet);

					if (CommonColors)
						SetLineColors(curve_count, pDataSet);
					else
						SetLineColors(var_index, pDataSet);

					if (CommonColors)
						SetLineWidth(curve_count, pDataSet);
					else
						SetLineWidth(var_index, pDataSet);

					if (m_pProp->m_ReShape.m_RegLine) {

						if ((m_pProp->m_ChartType ==CHART_TYPES::SCATTER) || (m_pProp->m_ChartType ==CHART_TYPES::TIME_SERIE)) {

							m_pProp->SetRegA0(var_index, m_Layers.pTrendLayer->getIntercept());
							if (m_pProp->m_ChartType ==CHART_TYPES::TIME_SERIE)
								m_pProp->SetRegA1(var_index, m_Layers.pTrendLayer->getSlope() * 86400);
							else
								m_pProp->SetRegA1(var_index, m_Layers.pTrendLayer->getSlope());
							m_pProp->SetRegR(var_index, m_Layers.pTrendLayer->getCorrelation());
						}
					}
					//if (m_Symbol.isymb == 1 && m_pProp->m_ChartType != 0) {
					//	m_pProp->SetSymbolStyle(curve_count, 0);
					//	m_pProp->SetSymbolStyle(var_index, 0);
					//}


					//if ((m_Symbol.isymb > 0 || m_pProp->m_ChartType ==CHART_TYPES::SCATTER || m_pProp->GetFirstIndepIndex() != string::npos || m_pProp->IsFirstIndepSet()) && pDataSet != nullptr) {
						if (CommonColors)
							SetSymbols(curve_count, pDataSet);
						else
							SetSymbols(var_index, pDataSet);
					//}

					// Add Trend Line
					if (m_pProp->m_ReShape.m_RegLine&&m_pProp->m_ChartType > CHART_TYPES::SCATTER)
						m_Layers.pTrendLayer = pXY_Chart->addTrendLayer(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfPoints), DoubleArray(m_pProp->viewPortDataSeries[var_index], m_ChartInfo.NumOfPoints));

				}
			}  // End of Loop for valid SubChart

			if(p_info.EnsembleType > ENSEMBLE_TYPE::NO) i_band++;

		}	// End of Loop for all subCharts
		if (m_DataSets.size() > 0&&ChartFilled) {
			m_ChartOrigInfo.push_back(m_CurrentChartInfo);
			Adjust_Legends_Axis_Labels();
			SetLegendInfo();
		}


	return ChartFilled;

}
void DrawDetails::AddTitle_Axis_ToChart( MultiChart *mc) {
	
// Set Determine Labels on axis
	if(m_pProp->MakeLabels()) {
		SelectAndSet_LabelsToAxis();  
		TitleAxisLabels();
	}
	

		///////////////////////////////////////////////////////////////////////////////////////
		// Step 3 - Set up x-axis scale
		///////////////////////////////////////////////////////////////////////////////////////



		X_Axis_Scale();
		X_Axis_LabelStyle();

		///////////////////////////////////////////////////////////////////////////////////////
		// Step 4 - Set up y-axis scale
		///////////////////////////////////////////////////////////////////////////////////////

		if (m_pProp->isXScaleChanged(m_ichart)) 	X_Axis_GetScale();
		if (m_pProp->isLeftYScaleChanged(m_ichart)) YLeft_Axis_GetScale();
		if (m_pProp->isRightYScaleChanged(m_ichart)) 	YRight_Axis_GetScale();
		if (m_pProp->m_ReShape.m_CommonScale)  YRight_Axis_GetScale();
	
		Y_Axis_Scale();

		if (m_pProp->m_ChartType != CHART_TYPES::CONTOUR_CHART) {
			if (m_pProp->isXScaleChanged(m_ichart)) X_Axis_SetChangedScale();
			if (m_pProp->isLeftYScaleChanged(m_ichart))	YLeft_Axis_SetChangedScale();
			if (m_pProp->isRightYScaleChanged(m_ichart)) YRight_Axis_SetChangedScale();
		}

		// Step 4b Set Format on scales and Apply AutoScaling 

		X_Axis_SetFormatScale();
		YLeft_Axis_SetFormatScale();
		YRight_Axis_SetFormatScale();
		if (m_pProp->m_ChartType == CHART_TYPES::CONTOUR_CHART)		Z_Axis_SetFormatScale();


		pXY_Chart->layout();

		X_Axis_SetScaleValueForChartPropArchive();
		YLeft_Axis_SetScaleForChartPropArchive();
		YRight_Axis_SetScaleForChartPropArchive();
		if (m_pProp->m_ChartType == CHART_TYPES::CONTOUR_CHART) Z_Axis_SetScaleForChartPropArchive();

		Y_Axis_LabelStyle();


	// Step Update Cug with recent info
	m_pChartCug->OnUpdate();
	
	AddChartToMulti( mc);






}
void DrawDetails::AddChartToMulti(MultiChart *mc) {
	int xleft=m_pProp->GetLeftMargin();
	int xright=m_pProp->GetRightMargin();
		int ytop=m_pProp->GetTopMargin();

	if(m_ichart==0)
		mc->addChart(0,0,pXY_Chart);
	else if(m_ChartSize.yrow==0&&m_ChartSize.xcol>0) 
		mc->addChart((m_ChartSize.xcol-1)*(m_ChartSize.xplotwidth+m_ChartSize.xdiff)+(m_ChartSize.xplotwidth+xleft+m_ChartSize.xdiff2),0,pXY_Chart);
	else if(m_ChartSize.xcol==0&&m_ChartSize.yrow>0) {
		mc->addChart(0,(m_ChartSize.yrow-1)*(m_ChartSize.yplotheight+m_ChartSize.ydiff)+(m_ChartSize.yplotheight+m_ChartSize.ydiff2+ytop),pXY_Chart);
	}
	else {
		mc->addChart((m_ChartSize.xcol-1)*(m_ChartSize.xplotwidth+m_ChartSize.xdiff)+(m_ChartSize.xplotwidth+xleft+m_ChartSize.xdiff2),
			(m_ChartSize.yrow-1)*(m_ChartSize.yplotheight+m_ChartSize.ydiff)+(m_ChartSize.yplotheight+m_ChartSize.ydiff2+ytop),pXY_Chart);
	}
}


