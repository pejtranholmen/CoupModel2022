
#include "DrawContourDetails.h"
#include "../Util.h"
#include "./ChartProperties\ChartProp.h"
#include "../NewBase\Doc.h"
#include "chartdir.h"

DrawContourDetails::DrawContourDetails(void)
{

	
}


DrawContourDetails::~DrawContourDetails(void)
{
}
bool DrawContourDetails::MakeContourPlot(XYChart *c,  int ich, int ivar)
{

	DataSet *pData;
	int PGindex;
	int yrow, xcol;
	xcol=(ich%m_pProp->m_NumMultiChartX);
	yrow=(ich/m_pProp->m_NumMultiChartX%m_pProp->m_NumMultiChartY);

	int j=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, ivar);
	PLOTv p_info;
	p_info=m_pSimDoc->m_PlotStorage.GetPlotInfo(j);
	void *Ptr=m_pSimDoc->m_PlotStorage.GetPtr(j);
	auto itype=m_pSimDoc->m_PlotStorage.GetDataType(j);			
	int idoc=m_pSimDoc->m_PlotStorage.GetDocIndex(j);
	int ilocal=m_pSimDoc->m_PlotStorage.GetLocalIndex(j);
	int FileNo=	m_pSimDoc->m_PlotStorage.GetValFileNo(j);
	int SectionNo=m_pSimDoc->m_PlotStorage.GetPGSectionNo(j);
	long irun;
	if(m_ChartInfo.ResidualSource)
		irun=m_pSimDoc->m_PlotStorage.GetValRunNo(j)+1;
	else
		irun=0;
	if(itype== DATA_TYPES::PGDATA) {
			m_pPG_Active=p_info.pPG;
	}
	else if(itype== DATA_TYPES::ANYDATA) {
			m_pPG_Active=(CPG*)((F*)Ptr)->GetPointer();
	}
	else {
		if(m_pSimDoc->m_PlotStorage.GetPGPointer(idoc)!=nullptr) {
			m_pPG_Active=m_pSimDoc->m_PlotStorage.GetPGPointer(idoc);
			m_pPG_Active->ReOpen();
		}
	}

	PGindex=m_pSimDoc->m_PlotStorage.GetVar_Index(j);
	FindIdForVariablesToPlot(j, p_info, PGindex);




	if(m_pUtil->m_MultiRunPlot) {
			DataSet *pData=SetMultiSimChartData(c, ich, ivar);
			
			if (pData == nullptr) return false;
			bool logdist;
			logdist=false;
			float vmin, vmax;
			float fint;

			if(m_pProp->GetIndepSecond()!=NO_USE&&&m_Contour.ScaleContourChart||m_pProp->GetIndepSecond()!=NO_USE&&ich>0) {
				vmin=m_Contour.SecondMin;
				vmax=m_Contour.SecondMax;
				c->yAxis()->setLinearScale(m_Contour.SecondMin, m_Contour.SecondMax);
				logdist=false;
			}
			else if(m_pProp->GetIndepSecond()!=NO_USE&&!m_Contour.ScaleContourChart) {
				c->layout();
				DoubleArray YaxisTicks;
				YaxisTicks=c->yAxis()->getTicks();
	
				int ntic=YaxisTicks.len;
				vmin = c->yAxis()->getMinValue();
				vmax = c->yAxis()->getMaxValue();

				if(ntic>=2) {
					fint=YaxisTicks[1]-YaxisTicks[0];
				}
				logdist=false;

			}
			else if(m_pProp->GetSecondIndepIndex()==0) {
					vmin=0;
					vmax=float(m_pSimDoc->MR_Get_TotalNumberofRuns());
					fint=(vmax-vmin)/10;
					logdist=false;
			}
			else if(m_pProp->GetIndepSecond()==PARVAR) {
					vmin=m_pSimDoc->MR_GetMin(m_pProp->GetSecondIndepIndex()-1);
					vmax=m_pSimDoc->MR_GetMax(m_pProp->GetSecondIndepIndex()-1);
					fint=(vmax-vmin)/10;
					logdist=m_pSimDoc->MR_GetLogDist(m_pProp->GetSecondIndepIndex()-1);
			}

				
			if(m_pProp->isLeftYScaleChanged(ich)) {
					vmin=m_pProp->GetYLeftmin(ich);
					vmax=m_pProp->GetYLeftmax(ich);
					fint=m_pProp->GetYLeftInterv(ich);
					if(logdist)
						c->yAxis()->setLogScale(double(vmin), double(vmax));
					else
						c->yAxis()->setLinearScale(double(vmin), double(vmax), fint);
			}
			else if(m_pProp->IsSecondIndepSet()&&m_Contour.ScaleContourChart) {
					vmin=m_pProp->GetYLeftmin(0);
					vmax=m_pProp->GetYLeftmax(0);
					fint=m_pProp->GetYLeftInterv(0);
					m_pProp->SetYLeftmin(ich, vmin);
					m_pProp->SetYLeftmax(ich, vmax);
					m_pProp->SetYLeftInterv(ich, fint);
					if(logdist)
						c->yAxis()->setLogScale(double(vmin), double(vmax));
					else
						c->yAxis()->setLinearScale(double(vmin), double(vmax), fint);
				}

			if(m_pProp->IsSecondIndepSet()) {
					m_Label.unit1=m_Label.unit;
				}

			
			if(m_pProp->IsSecondIndepSet()) {
					if(m_pProp->isXScaleChanged(ich)) {
						vmin=m_pProp->GetXmin(ich);
						vmax=m_pProp->GetXmax(ich);
						fint=m_pProp->GetXInterv(ich);
						m_Contour.FirstMin = vmin;
						m_Contour.FirstMax = vmax;
						m_Contour.FirstInterval = fint;
					}

					string xstr;
					if(m_pProp->isXLabelChanged(ich)) {
						xstr=m_pProp->GetXLabel(ich);

					}
					else {

						xstr=m_pProp->GetFirstIndepString();
						m_pProp->SetXLabel(ich, xstr.c_str());
					}
					m_Label.xtitle=xstr;
					c->xAxis()->setTitle(xstr.c_str(), "arialbd.ttf",  m_pUtil->m_LabelSizePoint);	

			}

			pData->setDataName(m_pProp->m_Title.c_str());

			if(m_pProp->isZLabelChanged(ich)) {
				m_pProp->m_Title=m_pProp->GetZLabel(ich);
				c->addTitle(Chart::TopCenter,m_pProp->m_Title.c_str(),"arialbd.ttf", m_pUtil->m_TitleSizePoint);
				m_Label.test= m_pProp->GetZLabel(ich);
			}
			else  {
				m_pProp->m_Title=m_Label.test;


				m_pProp->m_Title+="(";
				string unit=m_Label.unit;
				FUtil::trim(unit);
				m_pProp->m_Title+=unit.c_str();
				m_pProp->m_Title+=")";
				m_pProp->SetZLabel(ich, m_pProp->m_Title);
				pData->setDataName(m_pProp->m_Title.c_str());
				//c->addTitle(Chart::TopCenter,m_Title,"arialbd.ttf", m_pUtil->m_TitleSizePoint);
			}
			
			m_pUtil->SetZAxisExist();
			double zminvalue, zmaxvalue,ztickvalue;
			m_pColorAxis[ich]->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint);

			if(m_pProp->isZScaleChanged(ich)) {
				zminvalue=m_pProp->GetZmin(ich);
				zmaxvalue=m_pProp->GetZmax(ich);
				ztickvalue=m_pProp->GetZInterv(ich);
				int ntic = int((zmaxvalue - zminvalue) / ztickvalue);
				logdist=m_pSimDoc->MR_GetLogDist(p_info.Index+1);
				m_pColorAxis[ich]->setLevels(32);
				

				if(logdist) 
					m_pColorAxis[ich]->setLogScale(zminvalue, zmaxvalue);
				else {
					m_pColorAxis[ich]->setLinearScale(zminvalue, zmaxvalue, ztickvalue);
					m_pColorAxis[ich]->setColorGradient(false);
				}
				pData->setDataName(m_pProp->m_Title.c_str());
			}
			else if(!m_Contour.ScaleContourChart) {
				c->layout();
				m_pProp->m_Title=m_Label.test;
				m_pProp->m_Title+="(";
				string unit=m_Label.unit;
				FUtil::trim(unit);
				m_pProp->m_Title+=unit.c_str();
				m_pProp->m_Title+=")";
				m_pProp->SetZLabel(ich, m_pProp->m_Title);
			//	pData->setDataName(m_pProp->m_Title.c_str());

			
				DoubleArray ZaxisTicks;
				ZaxisTicks=m_pColorAxis[ich]->getTicks();
				m_pColorAxis[ich]->setColorGradient(false);
	
				int ntic=ZaxisTicks.len;
				zminvalue = m_pColorAxis[ich]->getMinValue();
				zmaxvalue = m_pColorAxis[ich]->getMaxValue();

				if(ntic>=2) {
					ztickvalue=ZaxisTicks[1]-ZaxisTicks[0];				
					m_pProp->SetZmin(ich,zminvalue);
					m_pProp->SetZmax(ich, zmaxvalue);
					m_pProp->SetZInterv(ich, ztickvalue);
					m_pColorAxis[ich]->setLevels(32);
				}

				
				DoubleArray XaxisTicks;
				XaxisTicks = c->xAxis()->getTicks();

				ntic = XaxisTicks.len;
				if (ntic >= 2) {
					auto xtickvalue = XaxisTicks[1] - XaxisTicks[0];
					m_pProp->SetXInterv(ich, xtickvalue);
				}
				auto xminvalue = c->xAxis()->getMinValue();
				auto xmaxvalue = c->xAxis()->getMaxValue();
				m_pProp->SetXmin(ich, xminvalue);
				m_pProp->SetXmax(ich, xmaxvalue);

			  

			}
			else  {
				zminvalue=m_Contour.SecondMin;
				zmaxvalue=m_Contour.SecondMax;
				ztickvalue=(zmaxvalue-zminvalue)/11;
				m_pProp->SetZmin(ich, zminvalue);
				m_pProp->SetZmax(ich, zmaxvalue);
				m_pProp->SetZInterv(ich,ztickvalue);
				m_pColorAxis[ich]->setLinearScale(zminvalue, zmaxvalue, ztickvalue);
				m_pColorAxis[ich]->setColorGradient(false);
				m_pColorAxis[ich]->setLevels(32);
				m_pColorAxis[ich]->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint);

				//cAxis->setAxisPos(20,0,0);
			}
	

		}
	else if((m_pProp->GetIndepFirst()==PGVAR||m_pProp->GetIndepFirst()==GEO_LONG||m_pProp->GetIndepFirst()==PGDAY
		||m_pProp->GetIndepFirst()==PGHOUR||m_pProp->GetIndepFirst()==PGYEAR)&&m_pProp->IsSecondIndepSet()) {

			pData=SetPGVarContourChartData(c,  ich, ivar);

			bool logdist;
			logdist = false;
			float vmin, vmax;
			float fint;

			if (m_pProp->GetIndepSecond() != NO_USE && &m_Contour.ScaleContourChart || m_pProp->GetIndepSecond() != NO_USE && ich > 0) {
				vmin = m_Contour.SecondMin;
				vmax = m_Contour.SecondMax;
				c->yAxis()->setLinearScale(m_Contour.SecondMin, m_Contour.SecondMax);
				logdist = false;
			}
			else if (m_pProp->GetIndepSecond() != NO_USE && !m_Contour.ScaleContourChart) {
				c->layout();
				DoubleArray YaxisTicks;
				YaxisTicks = c->yAxis()->getTicks();

				int ntic = YaxisTicks.len;
				vmin = c->yAxis()->getMinValue();
				vmax = c->yAxis()->getMaxValue();

				if (ntic >= 2) {
					fint = YaxisTicks[1] - YaxisTicks[0];
				}
				logdist = false;

			}

			if (!m_pProp->isXLabelChanged(ich)) {
				if (m_pProp->GetIndepFirst() == PGHOUR)
					m_pProp->SetFirstIndepString(PGHOUR, "Hour of Day", ich);
				else if (m_pProp->GetIndepFirst() == PGDAY)
					m_pProp->SetFirstIndepString(PGDAY, "Day of Year", ich);
				else if (m_pProp->GetIndepFirst() == PGYEAR)
					m_pProp->SetFirstIndepString(PGYEAR, "Year", ich);
			}
			if (!m_pProp->isYLabel1Changed(ich)) {

				if (m_pProp->GetIndepSecond()== PGHOUR)
					m_pProp->SetSecondIndepString(PGHOUR, "Hour of Day");
				else if (m_pProp->GetIndepSecond()== PGDAY)
					m_pProp->SetSecondIndepString(PGDAY, "Day of Year");
				else if (m_pProp->GetIndepSecond()== PGYEAR)
					m_pProp->SetSecondIndepString(PGYEAR, "Year");
				else if (m_pProp->GetIndepSecond() == GEO_LAT)
					m_pProp->SetSecondIndepString(PGYEAR, "Latitude");
			}

		}
	else if(m_pProp->GetIndepSecond()==PGDEPTH) {

			pData=SetPGDepthContourChartData(c,  ich, ivar);
			if (pData == nullptr) return false;
			//m_Label.ytitle1="Soil Depth";
			//m_Label.unit1="m";
			m_pUtil->SetZAxisExist();
			m_pColorAxis[ich]->setLabelStyle("arial.ttf", m_pUtil->m_AxisScalePoint);


			if(m_pProp->isZScaleChanged(ich)) {
				double zminvalue=m_pProp->GetZmin(ich);
				double zmaxvalue=m_pProp->GetZmax(ich);
				double ztickvalue=m_pProp->GetZInterv(ich);
				m_pColorAxis[ich]->setLinearScale(zminvalue, zmaxvalue, ztickvalue);
				pData->setDataName(m_pProp->m_Title.c_str());
			}


			if (!m_Contour.ScaleContourChart) {
				m_pProp->m_Title = m_Label.test;
				m_pProp->m_Title += "(";
				string unit = m_Label.unit;
				FUtil::trim(unit);
				m_pProp->m_Title += unit.c_str();
				m_pProp->m_Title += ")";
				m_pProp->SetTitle(m_pProp->m_Title);
				m_pProp->SetSecondIndepSet(PGDEPTH, true);
				m_pProp->SetFirstIndepSet(DATEAXIS, true);
				m_pProp->SetSecondIndepString(PGDEPTH, "Depth (m)");
			}	
		}
	return true;
}

DataSet* DrawContourDetails::SetMultiSimChartData(XYChart *c,  int ich, int ivar)
{
	DataSet *pData=nullptr;
	if(m_Contour.ScatterContourPoints)
				c->addScatterLayer(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfPoints)  ,DoubleArray(m_pProp->viewPortTimeStamps2, m_ChartInfo.NumOfPoints) , "", Chart::Cross2Shape(0.2), 7, 0x000000);

	int j=m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, ivar);
	PLOTv p_info;
	p_info=m_pSimDoc->m_PlotStorage.GetPlotInfo(j);
	void *Ptr=m_pSimDoc->m_PlotStorage.GetPtr(j);
	auto itype=m_pSimDoc->m_PlotStorage.GetDataType(j);	



	if(itype<DATA_TYPES::PGDATA/*||itype==99*/) {
			if (m_pConLayer.size() <= ich)	m_pConLayer.resize(ich + 1);
			if (m_pColorAxis.size() <= ich)	m_pColorAxis.resize(ich + 1);
				int ytop=m_pProp->GetTopMargin();
				int ybottom=m_pProp->GetBottomMargin();;
				int ydiff=int(float(m_pProp->GetGapMargin())*(1.-m_pUtil->GetGapHorFraction()));
				int xdiff=int(float(m_pProp->GetGapMargin())*m_pUtil->GetGapHorFraction());
				int xdiff1, xdiff2, ydiff1, ydiff2;
				xdiff1=int(float(xdiff)*m_pUtil->GetGapHorLeftFraction());
				xdiff2=xdiff-xdiff1;
				ydiff1=int(float(ydiff)*m_pUtil->GetGapVertTopFraction());
				ydiff2=ydiff-ydiff1;

				int xleft=m_pProp->GetLeftMargin();
				int xright=m_pProp->GetRightMargin();



				m_pConLayer[ich]=c->addContourLayer(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfPoints)  ,DoubleArray(m_pProp->viewPortTimeStamps2, m_ChartInfo.NumOfPoints) ,  DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints));
	
				pData=m_pConLayer[ich]->getDataSet(0);


				pData->setDataName(m_Label.test.c_str());

				int yrow, xcol;
				xcol=(ich%m_pProp->m_NumMultiChartX);
				yrow=(ich/m_pProp->m_NumMultiChartX%m_pProp->m_NumMultiChartY);



				int xpos, ypos;
				if(yrow==0&&xcol==0) {
					xpos=xleft;
					ypos=ytop;
					
				}
				else if(yrow==0&&xcol==m_pProp->m_NumMultiChartX-1) {
					xpos=xdiff1;
					ypos=ytop;
				}
				else if(yrow==0) {
					xpos=xdiff1;
					ypos=ytop;
				}
				else if(yrow==m_pProp->m_NumMultiChartY-1&&xcol==0) {
					xpos=xleft;
					ypos=ydiff1;
				}
				else if(yrow==m_pProp->m_NumMultiChartY-1&&xcol==m_pProp->m_NumMultiChartX-1) {
					xpos=xdiff1;
					ypos=ydiff1;
				}
				else if(yrow==m_pProp->m_NumMultiChartY-1) {
					xpos=xdiff1;
					ypos=ydiff1;
				}
				else if(xcol==0) {
					xpos=xleft;
					ypos=ydiff1;
				}
				else if(xcol==m_pProp->m_NumMultiChartX-1) {
					xpos=xdiff1;
					ypos=ydiff1;
				}
				else {
					xpos=xdiff1;
					ypos=ydiff1;

				}
				m_pColorAxis[ich] = m_pConLayer[ich]->setColorAxis(xpos+m_ChartSize.xplotwidth, ypos+10, Chart::TopLeft, m_ChartSize.yplotheight-10, Chart::Right);
				if(m_Contour.SplineContourInterPolation)
					m_pConLayer[ich]->setSmoothInterpolation(true);
				else
					m_pConLayer[ich]->setSmoothInterpolation(false);		

			}
			


	return pData;
}

DataSet* DrawContourDetails::SetPGVarContourChartData(XYChart *c,  int ich, int ivar) {
	DataSet *pData=nullptr;

	if(m_Contour.ScatterContourPoints)
				c->addScatterLayer(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfPoints)  ,DoubleArray(m_pProp->viewPortTimeStamps2, m_ChartInfo.NumOfPoints) , "", Chart::Cross2Shape(0.2), 7, 0x000000);
	m_pConLayer[ich]=c->addContourLayer(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfPoints)  ,DoubleArray(m_pProp->viewPortTimeStamps2, m_ChartInfo.NumOfPoints) ,  DoubleArray(m_pProp->viewPortDataSeries[ivar],m_ChartInfo.NumOfPoints));
	
	pData=m_pConLayer[ich]->getDataSet(0);

		int yrow, xcol;
		xcol=(ich%m_pProp->m_NumMultiChartX);
		yrow=(ich/m_pProp->m_NumMultiChartX%m_pProp->m_NumMultiChartY);

		int ytop=m_pProp->GetTopMargin();
				int ybottom=m_pProp->GetBottomMargin();;
				int ydiff=int(double(m_pProp->GetGapMargin())*(1.-m_pUtil->GetGapHorFraction()));
				int xdiff=int(double(m_pProp->GetGapMargin())*m_pUtil->GetGapHorFraction());
				int xdiff1, xdiff2, ydiff1, ydiff2;
				xdiff1=int(float(xdiff)*m_pUtil->GetGapHorLeftFraction());
				xdiff2=xdiff-xdiff1;
				ydiff1=int(float(ydiff)*m_pUtil->GetGapVertTopFraction());
				ydiff2=ydiff-ydiff1;

				int xleft=m_pProp->GetLeftMargin();
				int xright=m_pProp->GetRightMargin();			
				int xpos, ypos;
				if(yrow==0&&xcol==0) {
					xpos=xleft;
					ypos=ytop;
					
				}
				else if(yrow==0&&xcol==m_pProp->m_NumMultiChartX-1) {
					xpos=xdiff1;
					ypos=ytop;
				}
				else if(yrow==0) {
					xpos=xdiff1;
					ypos=ytop;
				}
				else if(yrow==m_pProp->m_NumMultiChartY-1&&xcol==0) {
					xpos=xleft;
					ypos=ydiff1;
				}
				else if(yrow==m_pProp->m_NumMultiChartY-1&&xcol==m_pProp->m_NumMultiChartX-1) {
					xpos=xdiff1;
					ypos=ydiff1;
				}
				else if(yrow==m_pProp->m_NumMultiChartY-1) {
					xpos=xdiff1;
					ypos=ydiff1;
				}
				else if(xcol==0) {
					xpos=xleft;
					ypos=ydiff1;
				}
				else if(xcol==m_pProp->m_NumMultiChartX-1) {
					xpos=xdiff1;
					ypos=ydiff1;
				}
				else {
					xpos=xdiff1;
					ypos=ydiff1;

				}
				m_pColorAxis[ich] = m_pConLayer[ich]->setColorAxis(xpos+m_ChartSize.xplotwidth, ypos+10, Chart::TopLeft, m_ChartSize.yplotheight-10, Chart::Right);
				if(m_Contour.SplineContourInterPolation)
					m_pConLayer[ich]->setSmoothInterpolation(true);
				else
					m_pConLayer[ich]->setSmoothInterpolation(false);	



	return pData;
}
DataSet* DrawContourDetails::SetPGDepthContourChartData(XYChart *c, int ich, int ivar) {
	DataSet *pData;

	auto ny = m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);
	SimB *pSimB = m_pSimDoc->GetPtrByName("Compartment Sizes", "Soil Profile");
	size_t nl=ny;
	if (pSimB!=nullptr) {

		 nl=((Tab*)pSimB)->GetNumOfRow();


	}
	vector<size_t> num_curves;
	num_curves = m_pSimDoc->m_PlotStorage.GetNumVarForEachSubChart(m_pProp->m_ChartNumber);
		
	vector<double> datay;
	datay.resize(num_curves[ich]);
	double ymin = 1.E38, ymax = -1.E38;
	int PGindex;
	m_pProp->SetFirstIndepVarType(DATEAXIS);

		TT2.clear();
		TT2.resize(num_curves[ich] * m_ChartInfo.NumOfPoints);
		size_t icurve_prior = 0;
		for (size_t i = 0; i < ich; i++)
			icurve_prior+= num_curves[i];

		for (size_t i_curve = 0; i_curve < num_curves[ich]; i_curve++) {
			datay.resize(num_curves[ich]);
			int jj = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, i_curve+icurve_prior);

			PLOTv p_info = m_pSimDoc->m_PlotStorage.GetPlotInfo(jj);
			auto itype = m_pSimDoc->m_PlotStorage.GetDataType(jj);
			int idoc = m_pSimDoc->m_PlotStorage.GetDocIndex(jj);
			if (itype == DATA_TYPES::PGDATA && p_info.pPG != nullptr) {
				m_pPG_Active = p_info.pPG;
			}
			else if (itype == DATA_TYPES::ANYDATA && p_info.pPG != nullptr) {
				m_pPG_Active = p_info.pPG; ;
			}
			else if (p_info.pPG != nullptr)
				m_pPG_Active = p_info.pPG;
			else {
				if (m_pSimDoc->m_PlotStorage.GetPGPointer(idoc) != nullptr) {
					m_pPG_Active = m_pSimDoc->m_PlotStorage.GetPGPointer(idoc);
				}
			}

			if (m_pProp->GetIndepSecond() == PGDEPTH) {
				PGindex = p_info.PGFileIndex;
				float v = -0.01 * FUtil::AtoFloat(m_pPG_Active->GetVarPos(PGindex));

				if (v < 1.E36)
					datay[i_curve] = v;
				else
					datay[i_curve] = 1.;
				if (v < ymin) ymin = v;
				if (v > ymax) ymax = v;
				m_Contour.SecondMin = ymin;
				m_Contour.SecondMax = ymax;

			}
			else {
				ymin = m_Time.MinDate;
				ymax = m_Time.MaxDate;


			}

			for (size_t ip = 0; ip < m_ChartInfo.NumOfPoints; ip++) {
				TT2[ip + m_ChartInfo.NumOfPoints * i_curve] = datay[i_curve];
			}

		}
		if (ymin == ymax) {
			MFC_Util::MessageBox("No Depth information in Variable\n ");
			m_pProp->SetSecondIndepVarType(NO_USE);
			return nullptr;
		}
		else {
			m_pProp->SetYLeftmin(0, ymin);
			m_pProp->SetYLeftmax(0, ymax);
		}



		m_pConLayer[ich] = c->addContourLayer(DoubleArray(m_pProp->viewPortTimeStamps, m_ChartInfo.NumOfPoints * num_curves[ich]), DoubleArray(&TT2[0], m_ChartInfo.NumOfPoints * num_curves[ich]), DoubleArray(m_pProp->viewPortDataSeries[ich], m_ChartInfo.NumOfPoints * num_curves[ich]));
	
		int j = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, 0);
		PGindex = m_pSimDoc->m_PlotStorage.GetVar_Index(j);
		string unit = m_pPG_Active->GetVarUnit(PGindex);
		FUtil::trim(unit);

		m_pProp->m_Title = m_Label.st;//m_pPG_Active->GetVarName(PGindex).c_str();
		if (m_pProp->GetTitle() == "-") {
			m_pProp->SetTitle(m_pProp->m_Title);
		}
		//m_pProp->m_Title=m_pProp->GetTitle();
	//	c->addTitle(Chart::TopCenter,m_pProp->m_Title.c_str(),"arialbd.ttf", m_pUtil->m_TitleSizePoint);

		if (m_pConLayer.size() <= ich)	m_pConLayer.resize(ich + 1);
		if (m_pColorAxis.size() <= ich)	m_pColorAxis.resize(ich + 1);
		if (m_ChartInfo.NumOfPoints == 0) return nullptr;				


		m_pColorAxis[ich] = m_pConLayer[ich]->setColorAxis(m_ChartSize.xplotwidth+m_pProp->GetLeftMargin()+10, m_pProp->GetTopMargin(), Chart::TopLeft, m_ChartSize.yplotheight, Chart::Right);


	/*for (size_t i = 0; i < m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber); i++) {
		int j = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, i);
		void *Ptr = m_pSimDoc->m_PlotStorage.GetPtr(j);
		auto itype = m_pSimDoc->m_PlotStorage.GetDataType(j);
		int idoc = m_pSimDoc->m_PlotStorage.GetDocIndex(j);
		PLOTv p_info = m_pSimDoc->m_PlotStorage.GetPlotInfo(j);

		if (itype == DATA_TYPES::PGDATA&& p_info.pPG != nullptr) {
			m_pPG_Active = p_info.pPG;
		}
		else if (itype == DATA_TYPES::ANYDATA&& p_info.pPG != nullptr) {
			m_pPG_Active = p_info.pPG; ;
		}
		else if (p_info.pPG != nullptr)
			m_pPG_Active = p_info.pPG;
		else {
			if (m_pSimDoc->m_PlotStorage.GetPGPointer(idoc) != nullptr) {
				m_pPG_Active = m_pSimDoc->m_PlotStorage.GetPGPointer(idoc);
			}
		}
		if (m_pProp->GetIndepSecond()== PGDEPTH) {
			PGindex = p_info.PGFileIndex;
			float v = -0.01*FUtil::AtoFloat(m_pPG_Active->GetVarPos(PGindex));

			if (v < 1.E36)
				datay[i] = v;
			else
				datay[i] = 1.;
			if (v < ymin) ymin = v;
			if (v > ymax) ymax = v;
			m_Contour.SecondMin = ymin;
			m_Contour.SecondMax = ymax;

		}
		else {
			ymin = m_Time.MinDate;
			ymax = m_Time.MaxDate;


		}
	}
	if (ymin == ymax) {
		MFC_Util::MessageBox("No Depth information in Variable\n ");
		m_pProp->SetSecondIndepVarType(NO_USE);
		return nullptr;


	}
	else {
		m_pProp->SetYLeftmin(0, ymin);
		m_pProp->SetYLeftmax(0, ymax);
	}
	TT2.clear();
	TT2.resize(ny*m_ChartInfo.NumOfPoints);
	for (size_t i = 0; i < ny; i++)
		for (size_t ii = 0; ii < m_ChartInfo.NumOfPoints; ii++)
			TT2[i*m_ChartInfo.NumOfPoints + ii] = datay[i];
	/*ZZ.clear();
	ZZ.resize(ny * m_ChartInfo.NumOfPoints);
	for (size_t i = 0; i < ny; i++)
		for (size_t ii = 0; ii < m_ChartInfo.NumOfPoints; ii++)
			ZZ[i * m_ChartInfo.NumOfPoints + ii] = *(m_pProp->viewPortDataSeries[ivar]+ii);
	


	int j = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, 0);
	PGindex = m_pSimDoc->m_PlotStorage.GetVar_Index(j);
	string unit = m_pPG_Active->GetVarUnit(PGindex);
	FUtil::trim(unit);

	m_pProp->m_Title = m_Label.st;//m_pPG_Active->GetVarName(PGindex).c_str();
	if (m_pProp->GetTitle() == "-") {
		m_pProp->SetTitle(m_pProp->m_Title);
	}
	//m_pProp->m_Title=m_pProp->GetTitle();
//	c->addTitle(Chart::TopCenter,m_pProp->m_Title.c_str(),"arialbd.ttf", m_pUtil->m_TitleSizePoint);

	if (m_pConLayer.size() <= ich)	m_pConLayer.resize(ich + 1);
	if (m_pColorAxis.size() <= ich)	m_pColorAxis.resize(ich + 1);
	if (m_ChartInfo.NumOfPoints == 0) return nullptr;					*/


//	m_pColorAxis[ich] = m_pConLayer[ich]->setColorAxis(m_ChartSize.xplotwidth+m_pProp->GetLeftMargin()+10, m_pProp->GetTopMargin(), Chart::TopLeft, m_ChartSize.yplotheight, Chart::Right);
	c->layoutAxes();
	pData=m_pConLayer[ich]->getDataSet(0);

		
			return pData;


}
