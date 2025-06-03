#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include "..\..\Util.h"
#include "..\..\NewBase\FileScanner.h"
#include "ChartCugChangeValue.h"
#include "..\..\MainFrm.h"



ChartCugChangeValue::ChartCugChangeValue(void)
{
	//m_pChartCug=MySingle::instance().GetChartCugPointer();
}
ChartCugChangeValue::~ChartCugChangeValue(void)
{
}
void ChartCugChangeValue::ChangeParameterValue(int col, int row, CString str) 
{
if(m_Mode==0) {
	float value;
		if(col==2) {
			size_t layer, i;
			if(m_ParameterPlot.m_plotp.Opt_Vec.size()>0) {
				auto ll=row%m_ParameterPlot.m_plotp.Opt_Vec.size();
				layer=m_ParameterPlot.m_plotp.Opt_Vec[ll];
				i=row/m_ParameterPlot.m_plotp.Opt_Vec.size();
			}
			else {
				layer=0;
				i=row;
			}
			if (m_ParameterPlot.m_NewPlotValue.size() > i) {
				value = MFC_Util::AtoFloat(str);
				m_ParameterPlot.m_PreviousValue[i][layer] = m_ParameterPlot.m_NewPlotValue[i][layer];
				if (m_ParameterPlot.m_PreviousValue[i][layer] == m_ParameterPlot.m_NewPlotValue[i][layer]) {
					QuickSetNumber(4, row, m_ParameterPlot.m_PreviousValue[i][layer]);
					QuickSetBackColor(4, row, m_ColorChanged);
				}

				if(m_ParameterPlot.m_Ps.size()>0) 
					m_ParameterPlot.m_NewPlotValue[i+ m_ParameterPlot.m_P.size()][layer] = value;
				else
					m_ParameterPlot.m_NewPlotValue[i][layer] = value;

				QuickSetBackColor(col, row, m_ColorChanged);
				if (m_ParameterPlot.m_PlotNewValue)
					m_ParameterPlot.OnInit();
				m_pUtil->m_FileSave.m_NoChange = false;
			}
		}
	}
	else if(m_Mode==2) {

		Func *funcpointer=m_ParameterPlot.GetCurrentFuncPointer();
		PLOT_PARAM plotparam=funcpointer->GetPlotParam();
		if(row<=1) {
			switch(col) {
			case 1:
				if(row==0) plotparam.X_Label=CT2A(str).m_psz;
				else if(row==1) plotparam.Y_Label=CT2A(str).m_psz;
				break;
			case 3: if(row==0) plotparam.X_Min=MFC_Util::AtoFloat(str);	break;
			case 4: if(row==0) plotparam.X_Max=MFC_Util::AtoFloat(str);	break;



			}
		}
		else {
			plotparam.LegendsVec[row-2]=CT2A(str).m_psz;


		}

		funcpointer->SetPlotParam(plotparam);
		m_ParameterPlot.OnInit();



	}


}
bool ChartCugChangeValue::ChangeGeneralChartLayOut(int col, int row, CString str)
{
	bool updatedirect=true;
	float value;
	CWinApp* pApp = AfxGetApp();
	if(row==0) {
		switch(col) {
			case 0:
				m_pProp->m_Title=CT2A(str).m_psz;
			break;
			case 1:
				m_pUtil->m_TitleSizePoint=MFC_Util::AtoInt(str);
				MFC_Util::WriteProfileInt("TitleSize",m_pUtil->m_TitleSizePoint );
			break;
			case 2:
				m_pUtil->m_LabelSizePoint=MFC_Util::AtoInt(str);
				MFC_Util::WriteProfileInt( "LabelSize", m_pUtil->m_LabelSizePoint);
			break;
			case 3:
				m_pUtil->m_LegendSizePoint=MFC_Util::AtoInt(str);
				MFC_Util::WriteProfileInt( "LegendSize",m_pUtil->m_LegendSizePoint);
			break;
			case 4:
				m_pUtil->m_LineWidthPoint=MFC_Util::AtoInt(str);
				MFC_Util::WriteProfileInt("LineWidth", m_pUtil->m_LineWidthPoint);
			break;
			case 5:
				m_pUtil->m_LineAxisWidthPoint=MFC_Util::AtoInt(str);
				MFC_Util::WriteProfileInt( "LineAxisWidth",m_pUtil->m_LineAxisWidthPoint);
			break;
			case 6:
				m_pUtil->m_SymbolSizePoint=MFC_Util::AtoInt(str);
				MFC_Util::WriteProfileInt( "SymbolSize", m_pUtil->m_SymbolSizePoint);
			break;
			case 7:
				m_pUtil->m_AxisScalePoint=MFC_Util::AtoInt(str);
				MFC_Util::WriteProfileInt( "AxisScaleSize", m_pUtil->m_AxisScalePoint);
			break;
			case 8:
				m_pUtil->m_MinTickDistX=MFC_Util::AtoInt(str);
				MFC_Util::WriteProfileInt( "AxisMinTickX", m_pUtil->m_MinTickDistX);
				
			break;
			case 9:
				m_pUtil->m_MinTickDistY=MFC_Util::AtoInt(str);
				MFC_Util::WriteProfileInt( "AxisMinTickY", m_pUtil->m_MinTickDistY);
			break;
		}
		}
		else if(row==2) {

			
			switch(col) {
				case 0:
					m_pProp->m_cx=MFC_Util::AtoInt(str)+180;
					MFC_Util::WriteProfileInt( "DisplaySize_cx", m_pProp->m_cx);
//					((CZoomscrolldemoDlg*)m_pDlg)->OnInitDialog();
					updatedirect=false;
					break;
				case 1:
					m_pProp->m_cy=MFC_Util::AtoInt(str)+28;
					MFC_Util::WriteProfileInt( "DisplaySize_cy", m_pProp->m_cy);
	//				((CZoomscrolldemoDlg*)m_pDlg)->OnInitDialog();
					updatedirect=false;
					break;
				case 2:
					m_pProp->m_TopMargin=MFC_Util::AtoInt(str);
					MFC_Util::WriteProfileInt( "TopMargin",m_pProp->m_TopMargin);
					
					break;
				case 3:
					m_pProp->m_BottomMargin=MFC_Util::AtoInt(str);
					MFC_Util::WriteProfileInt( "BottomMargin",m_pProp->m_BottomMargin);
					break;
				case 4:
					m_pProp->m_LeftMargin=MFC_Util::AtoInt(str);
					MFC_Util::WriteProfileInt( "LeftMargin",m_pProp->m_LeftMargin);
					break;
				case 5:
					m_pProp->m_RightMargin=MFC_Util::AtoInt(str);
					MFC_Util::WriteProfileInt( "RightMargin",m_pProp->m_RightMargin);
					break;
				case 6:
					m_pProp->m_GapMargin=MFC_Util::AtoInt(str);
					MFC_Util::WriteProfileInt( "GapMargin",m_pProp->m_GapMargin);
					break;
				case 7:
					m_pUtil->SetGapHorPercentage(MFC_Util::AtoInt(str));
					break;
				case 8:
					m_pUtil->SetGapHorLeftPercentage(MFC_Util::AtoInt(str));
					break;
				case 9:
					m_pUtil->SetGapVertTopPercentage(MFC_Util::AtoInt(str));
					break;
				}

	
		}
		else if (row==4) {


			switch(col) {	
				case 0:
					m_pUtil->m_LegendPositionX=MFC_Util::AtoInt(str);
					MFC_Util::WriteProfileInt( "LegendPositionX",m_pUtil->m_LegendPositionX);
					break;
				case 1:
					m_pUtil->m_LegendPositionY=MFC_Util::AtoInt(str);
					MFC_Util::WriteProfileInt( "LegendPositionY",m_pUtil->m_LegendPositionY);
					break;
			}

		}
		else if (row==6) {


			switch(col) {	
				case 0:
					m_pProp->m_BoxDimmer=MFC_Util::AtoInt(str);
					MFC_Util::WriteProfileInt("BoxDimmer",m_pProp->m_BoxDimmer);
					break;
				case 1:
					m_pProp->m_BoxRangeDimmer=MFC_Util::AtoInt(str);
					MFC_Util::WriteProfileInt("BoxRangeDimmer",m_pProp->m_BoxRangeDimmer);
					break;
			}

		}


		return updatedirect;
}
bool ChartCugChangeValue::ChangeAxis(int col, int row, CString str)
{
	float pV;

	size_t Index;
	int value;


	bool reload, updatedirect;

		updatedirect=false;	
		CUGCell cell;

		GetCell(4, row, &cell);
		string strtype = cell.GetStdText();
		


		
		if(row>=0&&row<m_NumUsedSubCharts&strtype=="Y Left") {
			updatedirect = false;
				Index=size_t(row);
				int n;
				switch(col) {
					case 0:
					//	m_pProp->m_YLabel1[Index]=CT2A(str).m_psz;
						m_pProp->m_YLabel1Change[Index]=true;
						m_pProp->m_ToBeUpdated = true;
						if (m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART) 
							m_pProp->ChangeYLabel1(string::npos, CT2A(str).m_psz);
						else
							m_pProp->ChangeYLabel1(Index, CT2A(str).m_psz);

						
						break;
					case 1:
						pV = MFC_Util::AtoFloat(str);
						if (m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART)
							m_pProp->ChangeYMin1(string::npos, pV);
						else
							m_pProp->ChangeYMin1(Index, pV);
						m_pProp->m_ToBeUpdated = true;
						break;
					case 2:
						n=m_pProp->m_YScaleLeftMax.size();
						if(Index<n) {
							pV=MFC_Util::AtoFloat(str);
							if (m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART)
								m_pProp->ChangeYMax1(string::npos, pV);
							else
								m_pProp->ChangeYMax1(Index, pV);
							m_pProp->m_ToBeUpdated = true;

						}
						break;
					case 3:
						pV=MFC_Util::AtoFloat(str);
						if (m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART)
							m_pProp->ChangeYInterv1(string::npos, pV);
						else
							m_pProp->ChangeYInterv1(Index, pV);

						m_pProp->m_ToBeUpdated = true;
						break;


		
				}

		}
		else if(strtype=="Y Right"&&!m_pProp->IsSecondIndepSet()) {
			GetCell(-1, row, &cell);
			string strvar = cell.GetStdText();
			Index = FUtil::AtoInt(strvar) - 1;
				if(size_t(Index)<m_pProp->m_YLabel2.size()) {
					switch(col) {
						case 0:
							m_pProp->m_YLabel2[Index]=CT2A(str).m_psz;
							m_pProp->m_YLabel2Change[Index]=true;
							break;
						case 1:
							m_pProp->m_YScaleRightChanged[Index]=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_YScaleRightMin[Index]=pV;
							break;
						case 2:
							m_pProp->m_YScaleRightChanged[Index]=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->SetYRightmax(Index,pV);
							break;
						case 3:
							pV=m_pProp->m_YScaleRightInterv.at(Index);
							m_pProp->m_YScaleRightInterv[Index]=pV;
							m_pProp->m_YScaleRightChanged[Index]=true;
							break;
					}
				}

		}
		else if((row>=m_NumUsedSubCharts&&strtype=="X Scale"&&!m_pProp->IsSecondIndepSet())
			||(row==1&&m_pProp->IsSecondIndepSet())) {
				long min;
				GetCell(-1, row, &cell);
				string strvar = cell.GetStdText();
				Index = FUtil::AtoInt(strvar) - 1;

				switch(col) {
					case 0:
						m_pProp->m_XLabel[Index]=CT2A(str).m_psz;
						m_pProp->m_XLabelChange[Index]=true;
						break;
					case 1:
						if (m_pProp->m_CommonXScale) 
							 m_pProp->SetXScaleChanged(string::npos);
						else
							m_pProp->m_XScaleChanged[Index]=true;
						if(m_pProp->m_ChartType!= CHART_TYPES::TIME_SERIE||m_pProp->m_ReShape.m_NormalDist||m_pProp->m_ReShape.m_SortedSeries) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							if (m_pProp->m_CommonXScale) 
								m_pProp->SetXmin(string::npos, pV);
							else
								m_pProp->SetXmin(Index,pV);							
						}
						else if(m_pUtil->m_MultiRunPlot) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_XScaleMin[Index]=pV;
							m_pProp->m_TimeScale.min = pV;
	
							if (m_pProp->m_CommonXScale)
								m_pProp->SetXmin(string::npos, pV);
							else 
								m_pProp->SetXmin(Index, pV);

							m_pProp->SetXScaleChanged(Index);
						//	if (m_pProp->m_XScaleInterv[Index] == 0)
								m_pProp->SetXInterv(Index,m_pProp->m_TimeScale.interv);
						//	if (m_pProp->m_XScaleMax[Index] == 0)
								m_pProp->SetXmax(Index, m_pProp->m_TimeScale.max);


						}
						else if (m_pProp->GetFirstIndepVarType() != DATEAXIS) {
							pV = MFC_Util::AtoFloat(str);
							if (m_pProp->m_CommonXScale)
								m_pProp->SetXmin(string::npos, pV);
							else
								m_pProp->SetXmin(Index, pV);
						}
						else			
						{
							m_pProp->SetXScaleChanged(Index);
							min=PGUtil::MinutConv(CT2A(str).m_psz)-2880;
							m_pProp->m_TimeScale.min=min*double(60);
						}
						break;
					case 2:

						if (m_pProp->m_CommonXScale)
							m_pProp->SetXScaleChanged(string::npos);
						else if (Index < m_pProp->m_XScaleChanged.size()) {
							for_each(m_pProp->m_XScaleChanged.begin(), m_pProp->m_XScaleChanged.end(), [](auto &Value) {
								Value = true; }
							);

		//					m_pProp->m_XScaleChanged[Index] = true;
						}
						if(m_pProp->m_ChartType!= CHART_TYPES::TIME_SERIE||m_pProp->m_ReShape.m_NormalDist||m_pProp->m_ReShape.m_SortedSeries) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_XScaleMax[Index]=pV;
							if (m_pProp->m_CommonXScale)
								m_pProp->SetXmax(string::npos, pV);
							else {
								m_pProp->SetXmax(string::npos, pV);
								//for(size_t iall=0; iall<m_pProp->m_XScaleMax.size(); iall++) m_pProp->m_XScaleMax[iall] = pV;

								//for_each(m_pProp->m_XScaleMax.begin(), m_pProp->m_XScaleMax.end(), [&](auto &value) {
								//value= pV; });



							}


						}
						else if(m_pUtil->m_MultiRunPlot) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							if (m_pProp->GetIndepFirst() == MULTIRUN_NUM) {
								m_pProp->SetXmax(Index, pV);
								if (m_pProp->m_CommonXScale)
									m_pProp->SetXmax(string::npos, pV);
								else
									m_pProp->SetXmax(Index, pV);


							}
							else {
								m_pProp->m_XScaleMax[Index] = pV;
								if (m_pProp->m_CommonXScale)
									m_pProp->SetXmax(string::npos, pV);
								else
									m_pProp->m_XScaleMax[Index] = pV;
								m_pProp->m_TimeScale.max = pV;
								if (m_pProp->m_XScaleInterv[Index] == 0)
									m_pProp->m_XScaleInterv[Index] = m_pProp->m_TimeScale.interv;
								if (m_pProp->m_XScaleMin[Index] == 0)
									m_pProp->m_XScaleMin[Index] = m_pProp->m_TimeScale.min;
							}
						}
						else if (m_pProp->GetFirstIndepVarType() != DATEAXIS) {
							pV = MFC_Util::AtoFloat(str);
							if (m_pProp->m_CommonXScale)
								m_pProp->SetXmax(string::npos, pV);
							else
								m_pProp->m_XScaleMax[Index] = pV;



						}
						else {
							min=PGUtil::MinutConv(CT2A(str).m_psz)-2880;
							m_pProp->m_TimeScale.max=min*60.;
						}
						break;
					case 3:

						if (m_pProp->m_CommonXScale)
							m_pProp->SetXScaleChanged(string::npos);
						else
							m_pProp->m_XScaleChanged[Index] = true;
						if(m_pProp->m_ChartType!= CHART_TYPES::TIME_SERIE||m_pProp->m_ReShape.m_NormalDist||m_pProp->m_ReShape.m_SortedSeries) {
							reload=true;
				
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_XScaleInterv[Index]= pV;

							if (m_pProp->m_CommonXScale)
								m_pProp->SetXInterv(string::npos, pV);

						}
						else if(m_pUtil->m_MultiRunPlot) {
							reload=true;
	
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_XScaleInterv[Index]= pV;
							if (m_pProp->m_CommonXScale)
								m_pProp->SetXInterv(string::npos, pV);
							m_pProp->m_TimeScale.interv = pV;
							if (m_pProp->m_XScaleMax[Index] == 0)
								m_pProp->m_XScaleMax[Index] = m_pProp->m_TimeScale.max;
							if (m_pProp->m_XScaleMin[Index] == 0)
								m_pProp->m_XScaleMin[Index] = m_pProp->m_TimeScale.min;


						}
						else if (m_pProp->GetFirstIndepVarType() != DATEAXIS) {
							pV = MFC_Util::AtoFloat(str);
							if (m_pProp->m_CommonXScale)
								m_pProp->SetXInterv(string::npos, pV);
							else
								m_pProp->SetXInterv(Index, pV);
						}
						else
							m_pProp->m_TimeScale.interv = MFC_Util::AtoFloat(str)*double(60);

						break;
				}
				m_pProp->m_ToBeUpdated = true;
				updatedirect = false;
				
		}
		else if(m_pProp->IsSecondIndepSet()) {
			if (m_pProp->GetSecondIndepVarType() == PGDEPTH&&row == 0) {
				switch (col) {
				case 0:
					m_pProp->SetSecondIndepString(PGDEPTH, CT2A(str).m_psz);
					m_pProp->SetYLabel1Changed(0);
				case 1:
					m_pProp->m_YScaleLeftChanged[0] = true;
					pV = MFC_Util::AtoFloat(str);
					m_pProp->m_YScaleLeftMin[0] = pV;
					break;
				case 2:
						pV = MFC_Util::AtoFloat(str);
						m_pProp->m_YScaleLeftMax[0] = pV;
						m_pProp->m_YScaleLeftChanged[0] = true;
					break;
				case 3:
					pV = MFC_Util::AtoFloat(str);
					m_pProp->m_YScaleLeftInterv[0] = pV;
					m_pProp->m_YScaleLeftChanged[0] = true;
					break;
				}
				return false;
			}

			GetCell(-1, row, &cell);
			string strvar = cell.GetStdText();
			Index = FUtil::AtoInt(strvar) - 1;
			if (Index == std::string::npos) return false;

			switch(col) {
					case 0:
						m_pProp->m_ZLabel[Index]=CT2A(str).m_psz;
						m_pProp->m_ZLabelChange[Index]=true;
						break;
					case 1:

						m_pProp->m_ZScaleChanged[Index]=true;
						if(m_pProp->m_ChartType!= CHART_TYPES::TIME_SERIE ||m_pProp->m_ReShape.m_NormalDist||m_pProp->m_ReShape.m_SortedSeries) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_ZScaleMin[Index]=pV;
						}
						else if(m_pUtil->m_MultiRunPlot) {
							reload=true;
							pV=m_pProp->m_ZScaleMin.at(Index);
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_ZScaleMin[Index]=pV;

						}

						break;
					case 2:

						m_pProp->m_ZScaleChanged[Index]=true;
						if(m_pProp->m_ChartType!= CHART_TYPES::TIME_SERIE||m_pProp->m_ReShape.m_NormalDist||m_pProp->m_ReShape.m_SortedSeries) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_ZScaleMax[Index]=pV;
						}
						else if(m_pUtil->m_MultiRunPlot) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_ZScaleMax[Index]=pV;
						
						}

						break;
					case 3:
						m_pProp->m_ZScaleChanged[Index]=true;
						if(m_pProp->m_ChartType!= CHART_TYPES::TIME_SERIE ||m_pProp->m_ReShape.m_NormalDist||m_pProp->m_ReShape.m_SortedSeries) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_ZScaleInterv[Index]=pV;
						}
						else if(m_pUtil->m_MultiRunPlot) {
							reload=true;
							pV=MFC_Util::AtoFloat(str);
							m_pProp->m_ZScaleInterv[Index]=pV;


						}
						break;
				}
			m_pProp->m_ToBeUpdated = true;
			updatedirect = false;
			
		}


		if (m_pProp->m_ChartType == CHART_TYPES::CONTOUR_CHART && m_pProp->m_ToBeUpdated) {
			m_TimeSeriePlot.m_Contour.ScaleContourChart = true;
		}


	return updatedirect;
}
bool ChartCugChangeValue:: ChangeVariableLegends(int col, int row, CString str)
{
	while (m_pProp->m_Legend1.size()<= row) m_pProp->m_Legend1.push_back(CT2A(str).m_psz);
	while (m_pProp->m_Legend1Change.size()<= row) m_pProp->m_Legend1Change.push_back(true);
		
	m_pProp->m_Legend1[row]= CT2A(str).m_psz;
	m_pProp->m_Legend1Change[row]=true;
	m_pProp->m_LegendChangeAny = true;
	return true;
}
bool ChartCugChangeValue::ChangeDistributionValue(int col, int row, CString str)
{
		bool updatedirect=true;	
		if(col==3) {
			m_pProp->SetXDistMin(row,MFC_Util::AtoFloat(str));
			

		}
		else if(col==4) {
			m_pProp->SetXDistMax(row, MFC_Util::AtoFloat(str));
			m_pProp->SetXDistChanged(row, true);
		}
		else if(col==5) {
			int num=MFC_Util::AtoInt(str);
			auto n1=m_pProp->m_Legend1.size();
			for (int il=0;il<n1;il++) {
				QuickSetNumber(col,il,num);
				RedrawCell(col, il);
				m_pProp->SetXDistSteps(il, num);
				m_pProp->SetXDistChanged(il, true);
			}
		}
		else 
			return false;
		return updatedirect;
}