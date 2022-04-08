
#include "../stdafx.h"
#include "../std.h"
#include "../Util.h"
#include "../CoupModelView.h"
#include "chartdir.h"
#include "./ChartViewer.h"
#include "./ModelView.h"
#include "../NewBase\Doc.h"
#include "./ChartProperties\ChartProp.h"
#include "./ChartProperties\ChartUtil.h"


ModelView::ModelView()
{
	m_Init=false;
	m_pSimDoc=nullptr;
	m_pProp=MySingle::instance().GetChartPropPointer();
	m_pUtil=MySingle::instance().GetChartUtilPointer();
	m_pChartViewer=nullptr;
	m_pMC=nullptr;
}
ModelView::~ModelView()
{

	CleanModelPlot();

}
void ModelView::InitDoc(Doc *pDoc) {
	//m_pDoc=pDoc;
	m_pSimDoc=pDoc;
	CCoupModelView *pView;
	pView=m_pSimDoc->GetModelView();
	m_pChartViewer=&pView->m_ChartViewer;
}
bool ModelView::GetModelData(int xplotwidth, int yplotheight, int ydiff) {
		size_t nlay;
	P *pCPtLower=m_pSimDoc->GetPPointer("mLower Depth");
	P *pCPtThickness=m_pSimDoc->GetPPointer("Thickness Layers");
	if(pCPtLower!=nullptr) {
			nlay=pCPtLower->GetSize();
			for(size_t i=0; i<nlay;i++) {
				if(i==0) {
					AA[0][i]=double(pCPtLower->GetValue(i)/2);
				}
				else {
					AA[0][i]=double((pCPtLower->GetValue(i-1)+pCPtLower->GetValue(i))/2); 
					AA[10][i]=double(pCPtLower->GetValue(i));
					AA[20][i]=double((pCPtLower->GetValue(i-1)*0.75+pCPtLower->GetValue(i)*0.25)); //y
					AA[30][i]=double((pCPtLower->GetValue(i-1)*0.25+pCPtLower->GetValue(i)*0.75)); //y
				}//y

	
				AA[1][i]=1.5;	
				AA[11][i]=0.5;	//x
				AA[21][i]=0;	//x very left
				AA[31][i]=2.0;	//x very right
				AA[41][i]=-.25;	//x very right
				AA[2][i]=xplotwidth/4;			// length
				AA[22][i]=xplotwidth/6;			// length

				double relthick=pCPtThickness->GetValue(i)/pCPtLower->GetValue(nlay-1);

				AA[12][i]=double(yplotheight)*relthick/4;			// vertical short
				AA[3][i]=90;
				AA[13][i]=180;
				AA[23][i]=270;
				AA[33][i]=360;
				//direction;
				if(i==0)
					AA[4][i]=0.;
				else
					AA[4][i]=double(pCPtLower->GetValue(i-1)); //
				AA[5][i]=double(pCPtLower->GetValue(i));//
				AA[6][i]=1; //x middle
			}

			AA[60][0]=0.5; //X Prec
			AA[61][0]=0.;    // y prec
			AA[62][0]=ydiff*0.9;  //prec length
			AA[63][0]=180.;
			m_dataSeriesA[60] = DoubleArray(&AA[60][0], 1);
			m_dataSeriesA[61] = DoubleArray(&AA[61][0], 1);
			m_dataSeriesA[62] = DoubleArray(&AA[62][0], 1);
			m_dataSeriesA[63] = DoubleArray(&AA[63][0], 1);
			AA[64][0]=1.5; //X Evap
			AA[65][0]=0.;    // y prec
			AA[66][0]=ydiff*0.9;  //evaporation length
			AA[67][0]=0.;
			m_dataSeriesA[64] = DoubleArray(&AA[64][0], 1);
			m_dataSeriesA[65] = DoubleArray(&AA[65][0], 1);
			m_dataSeriesA[66] = DoubleArray(&AA[66][0], 1);
			m_dataSeriesA[67] = DoubleArray(&AA[67][0], 1);

			AA[68][0]=0.; // Transpiration
			AA[69][0]=AA[20][nlay-1];    // y Transpiration
			AA[70][0]=yplotheight;  //evaporation length
			AA[71][0]=0.;
			m_dataSeriesA[68] = DoubleArray(&AA[68][0], 1);
			m_dataSeriesA[69] = DoubleArray(&AA[69][0], 1);
			m_dataSeriesA[70] = DoubleArray(&AA[70][0], 1);
			m_dataSeriesA[71] = DoubleArray(&AA[71][0], 1);

			m_dataSeriesA[0] = DoubleArray(&AA[0][0], nlay);
			m_dataSeriesA[10] = DoubleArray(&AA[10][0], nlay); //midpoint
			m_dataSeriesA[20] = DoubleArray(&AA[20][0], nlay); //midpoint
			m_dataSeriesA[30] = DoubleArray(&AA[30][0], nlay); //midpoint
			m_dataSeriesA[11] = DoubleArray(&AA[11][0], nlay); //left point
			m_dataSeriesA[31] = DoubleArray(&AA[31][0], nlay); //left point
			m_dataSeriesA[41] = DoubleArray(&AA[41][0], nlay); //left point
			m_dataSeriesA[1] = DoubleArray(&AA[1][0], nlay); //right point
			m_dataSeriesA[21] = DoubleArray(&AA[21][0], nlay); // very right
			m_dataSeriesA[6] = DoubleArray(&AA[6][0], nlay); // mid point
			m_dataSeriesA[2] = DoubleArray(&AA[2][0], nlay); // length
			m_dataSeriesA[12] = DoubleArray(&AA[12][0], nlay); // length
			m_dataSeriesA[22] = DoubleArray(&AA[22][0], nlay); // length
			m_dataSeriesA[3] = DoubleArray(&AA[3][0], nlay); // right
			m_dataSeriesA[13] = DoubleArray(&AA[13][0], nlay);
			m_dataSeriesA[23] = DoubleArray(&AA[23][0], nlay);
			m_dataSeriesA[33] = DoubleArray(&AA[33][0], nlay);
			m_dataSeriesA[4] = DoubleArray(&AA[4][0], nlay);
			m_dataSeriesA[5] = DoubleArray(&AA[5][0], nlay);
			return true;
	}
	else
		return false;

}
void ModelView::InitData() {
	CleanModelPlot();
	m_Init=true;
		Sw *pSw;
		m_HeatEq=true;
		m_NC=m_HBV=m_WaterEq=0;
		m_Evapo=m_PlantType=m_SaltTracer=m_SnowPack=m_SoilVapour=m_NC=m_Irrigation=m_Gw=0;
	for(size_t i=0;i<m_pSimDoc->m_DefinedSwitches.size();i++) {
		pSw=m_pSimDoc->m_DefinedSwitches[i];

		if(pSw->GetName()=="HeatEq")
			m_HeatEq=false;
		else if(pSw->GetName()=="WaterEq")
			m_WaterEq=pSw->GetIntValue();
		else if(pSw->GetName()=="Nitrogen and Carbon")
			m_NC=pSw->GetIntValue();
		else if(pSw->GetName()=="HBV Soil Module") {
			m_HBV=pSw->GetIntValue();
			if(m_HBV==2) {
				m_WaterEq=0;
			}
		}
		else if(pSw->GetName()=="Evaporation")
			m_Evapo=pSw->GetIntValue();
		else if(pSw->GetName()=="GroundWaterFlow")
			m_Gw=pSw->GetIntValue();
		else if(pSw->GetName()=="Irrigation")
			m_Irrigation=pSw->GetIntValue();
		else if(pSw->GetName()=="PlantType")
			m_PlantType=pSw->GetIntValue();
		else if(pSw->GetName()=="SaltTracer")
			m_SaltTracer=pSw->GetIntValue();
		else if(pSw->GetName()=="SnowPack")
			m_SnowPack=pSw->GetIntValue();
		else if(pSw->GetName()=="SoilVapour")
			m_SoilVapour=pSw->GetIntValue();
	};




}
void ModelView::MakePlantView(XYChart *c, int xplotwidth, int yplotheight) {
		size_t nplant;
		double MaxHeight;
		MaxHeight=0.;
		VectorLayer *vectorlayer;
		BoxWhiskerLayer *pBoxLayer;
		Sw *pSw=m_pSimDoc->GetSw("CanopyHeightInput");
		P *pCPtMaxHeight;
		if(pSw->GetIntValue()==2) {
			pCPtMaxHeight=m_pSimDoc->GetP("Max Height");
		}
		else if(pSw->GetIntValue()==0) {
			pCPtMaxHeight=m_pSimDoc->GetP("hOptimum Value");
		}
		else
			pCPtMaxHeight=m_pSimDoc->GetP("hOptimum Value");
		
		nplant=pCPtMaxHeight->GetSize();

		Ps *pP=m_pSimDoc->GetPs("ReferenceHeight");
		double RefHeight=pP->GetValue();
		double maxheight;
		maxheight=RefHeight;
		double xpos, xwidth, top, bottom;
		xpos = 0.;
		double length;
		double dir;
		for(size_t i=0; i<nplant;i++)	{
			top=double(pCPtMaxHeight->GetValue(i))+0.5;
			if(maxheight<top) maxheight=top;
		}


		double ishift=1./nplant/5;
			
		if(m_PlantType>2) 
			P *pCPtMaxCover=m_pSimDoc->GetP("Max Cover");

		for(size_t i=0; i<nplant;i++) {
			top=double(pCPtMaxHeight->GetValue(i));
			if(top>MaxHeight) MaxHeight=top;
			bottom=double(pCPtMaxHeight->GetValue(i)*0.7);
			xpos=double(i)*ishift+1.;
			xwidth=1.;
			if(m_PlantType>2) {
				P *pCPtMaxCover=m_pSimDoc->GetP("Max Cover");
				xwidth=double(pCPtMaxCover->GetValue(i));
			};
			pBoxLayer=c->addBoxLayer(DoubleArray(&top,1), DoubleArray(&bottom,1)	,0xBB00FF00,"Leaf");
			pBoxLayer->setXData(DoubleArray(&xpos,1));
			pBoxLayer->setDataWidth(int(xplotwidth*xwidth/1.5));
			bottom=0.;
			pBoxLayer=c->addBoxLayer(DoubleArray(&top,1), DoubleArray(&bottom,1)	,0xBBFFFF00,"Stem");
			xpos=xpos-1;
			pBoxLayer->setXData(DoubleArray(&xpos,1));
			pBoxLayer->setDataWidth(xplotwidth/20);
	
			dir=0;
			length=maxheight;
			xpos=xpos;
			vectorlayer=c->addVectorLayer(DoubleArray(&xpos,1),DoubleArray(&bottom,1), DoubleArray(&length,1),DoubleArray(&dir,1),2,0x00FFFF,"Transpiration");
			xpos=xpos+1.;
			length=maxheight-top;
			vectorlayer=c->addVectorLayer(DoubleArray(&xpos,1),DoubleArray(&top,1), DoubleArray(&length,1),DoubleArray(&dir,1),2,0x0000FF,"Interception");
		}					
		xpos=xpos+1;
		length=length=maxheight;
		vectorlayer=c->addVectorLayer(DoubleArray(&xpos,1),DoubleArray(&bottom,1), DoubleArray(&length,1),DoubleArray(&dir,1),2,0x0000FF,"Soil evaporation");
					
		c->xAxis()->setLinearScale(0, 2);
		c->yAxis()->setLogScale(0.1, maxheight);

};
void ModelView::MakeSnowView(XYChart *c, int xplotwidth, int yplotheight) {
		VectorLayer *vectorlayer;
		BoxWhiskerLayer *pBoxLayer;
		double xpos, xwidth, top, bottom;
		double length;
		double dir;


			top=1.;
			bottom=0.;
			xpos=1.;
			xwidth=2.;

			pBoxLayer=c->addBoxLayer(DoubleArray(&top,1), DoubleArray(&bottom,1)	,0xBB0000EE,"Snow Pack");
			pBoxLayer->setXData(DoubleArray(&xpos,1));
			pBoxLayer->setDataWidth(int(xplotwidth/1.5));


	
			dir=0;
			length=1;
			xpos=xpos;
			vectorlayer=c->addVectorLayer(DoubleArray(&xpos,1),DoubleArray(&top,1), DoubleArray(&length,1),DoubleArray(&dir,1),2,0xBB0000EE,"Snow Evaporation");
			dir=180.;
			length=0.5;
			vectorlayer=c->addVectorLayer(DoubleArray(&xpos,1),DoubleArray(&bottom,1), DoubleArray(&length,1),DoubleArray(&dir,1),0,0x0000FF,"Snow melt");
				
						
		c->xAxis()->setLinearScale(0, 2);
		c->yAxis()->setLinearScale(0, 2);


};
void ModelView::DrawModelPlot()
{
	

	int NumY=2;
	int NumX=4;

	if(m_PlantType==0)
		NumY=1;
	if(m_NC==0) {
		NumX=2;
		if(!m_HeatEq||m_WaterEq!=1)
			NumX=1;
		if(m_HBV==1)
			NumX++;
	}
	else {
		NumX=4;
		if(m_HBV==1)
			NumX++;
	}


	int ydiff=int(float(m_pProp->GetGapMargin())*(1.-m_pUtil->GetGapHorFraction()));
	int xdiff=int(float(m_pProp->GetGapMargin())*m_pUtil->GetGapHorFraction());
	int xleft=m_pProp->GetLeftMargin();
	int ytop=m_pProp->GetTopMargin();
	int ybottom=30;//GetBottomMargin();
	int xright=30;//GetRightMargin();
	int	yplotheight=m_pProp->m_DisplayYSize-ytop-ybottom;
	int	xplotwidth=m_pProp->m_DisplayXSize-xleft-xright;
	int xdiff1, xdiff2, ydiff1, ydiff2;
	xdiff1=int(xdiff*m_pUtil->GetGapHorLeftFraction());
	xdiff2=xdiff-xdiff1;
	ydiff1=int(float(ydiff)*0.9);
	ydiff2=ydiff-ydiff1;


	
	yplotheight=(m_pProp->m_DisplayYSize-ytop-ybottom-(NumY-1)*ydiff)/NumY;
	xplotwidth=(m_pProp->m_DisplayXSize-xleft-xright-(NumX-1)*xdiff)/NumX;


	if(!GetModelData(xplotwidth, yplotheight, ydiff)) return;


	bool createlegend=true;

	int legendposX, legendposY;
	legendposX=m_pUtil->GetLegendXPos();
	legendposY=yplotheight+40;
	bool vertical_legends;
	
	vertical_legends=m_pProp->GetVerticalLegends();



	XYChart *c;
	MultiChart *mc;
	mc = new  MultiChart(m_pProp->m_DisplayXSize, m_pProp->m_DisplayYSize, m_pUtil->m_ColorBackground);



    double radius[] = {5, 15};
    double angle[] = {0, 30};

    // Magnitude and direction of the vectors
    double magnitude[] = {55, 15};
    double direction[] = {0, 180};
//	c->setPlotArea(xleft, ytop, xplotwidth, yplotheight);
	m_pProp->m_Title="Model Structure";
	PlotArea *plotarea;
	VectorLayer *vectorlayer;

	double xvalue[]= {0,1,2};
	double yval[]= {1,2,3};
	double length[]={100, 50,125};
	double dir[]={90,90,90};
	DoubleArray xx,yy,ll,dd;
	xx=DoubleArray(xvalue,3);
	yy=DoubleArray(yval,3);
	ll=DoubleArray(length,3);
	dd=DoubleArray(dir,3);
//	c->addTitle(Chart::TopCenter,m_Title,"arialbd.ttf", m_TitleSizePoint);
	bool snowmade=false;
	bool heatmade=false;
	for(int xcol=0; xcol<NumX;xcol++) {
		for(int yrow=0; yrow<NumY;yrow++) {
				if(yrow==0&&xcol==0&&yrow<NumY-1) {
					//UpperLeft corner
					if(m_PlantType>=2) {
						//Explicit plant
						c = new  XYChart(xplotwidth+xleft+xdiff, yplotheight+ytop+ydiff2, m_pUtil->m_ColorBackground);
						m_pProp->m_NoXAxisScale=true;
				
						plotarea=c->setPlotArea(xleft, ytop, xplotwidth, yplotheight);
						MakePlantView(c, xplotwidth, yplotheight);
						if(createlegend||m_pUtil->MakeSingleLegends())
							c->addLegend(legendposX+xleft, legendposY+ytop,vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);

					}
					else {
						c = new  XYChart(xplotwidth+xleft+xdiff, yplotheight+ytop+ydiff2, m_pUtil->m_ColorBackground);
						m_pProp->m_NoXAxisScale=true;

					};

				}
				else if(yrow==0&&xcol==NumX-1&&yrow<NumY-1) {
					//upper right corner
					if(NumY==1)
						c = new  XYChart(xplotwidth+xright+xdiff1, yplotheight+ytop+ybottom, m_pUtil->m_ColorBackground);
	
					else {
						c = new  XYChart(xplotwidth+xright+xdiff1, yplotheight+ytop+ydiff2, m_pUtil->m_ColorBackground);
						m_pProp->m_NoXAxisScale=true;
					}

					m_pProp->m_NoYAxisScale=true;
					plotarea=c->setPlotArea(xdiff1, ytop, xplotwidth , yplotheight);
					if(m_SnowPack&&!snowmade) {
							MakeSnowView(c, xplotwidth, yplotheight);
							snowmade=true;
					}
					else if(m_NC>0) {
						MakeNC_Plant(c, xplotwidth, yplotheight);

					}
					if(createlegend)
						c->addLegend(legendposX+xdiff, legendposY+ytop, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);
				}
				else if(yrow==0) {
					//upper midle
					if(NumY==1)
						c = new  XYChart(xplotwidth+xdiff, yplotheight+ytop+ybottom, m_pUtil->m_ColorBackground);
					else {
						c = new  XYChart(xplotwidth+xdiff, yplotheight+ytop+ydiff2, m_pUtil->m_ColorBackground);
						m_pProp->m_NoXAxisScale=true;
					}
					if(m_SnowPack&&!snowmade) {
							MakeSnowView(c, xplotwidth, yplotheight);
							snowmade=true;
					}
					else if(xcol>1) {
						MakeNC_GasFlow(c, xplotwidth, yplotheight);
						
					}
					else {
						if(xcol==0)
							MakeSoilWater(c, xplotwidth, yplotheight);
						else if(m_HeatEq )
							 MakeSoilHeat(c,xplotwidth , yplotheight, heatmade);
					}


					if(xcol==NumX/2)
						m_pProp->m_MakeChartTitle=true;
					m_pProp->m_NoYAxisScale=true;
					plotarea=c->setPlotArea(xdiff1, ytop, xplotwidth , yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);




					//AreaLayer *l1;
					//l1=c->addAreaLayer();
					//l1->setDataCombineMethod(1);
	
					
					//arealayer= c->addAreaLayer(DoubleArray(radius, sizeof(radius)/sizeof(radius[0])));
					if(createlegend)
						c->addLegend(legendposX+xdiff1, legendposY+ytop, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);
				}
				else if(yrow==NumY-1&&xcol==0) {
					// Lower Left
					if(NumX==1)
						c = new  XYChart(xplotwidth+xleft+xright, yplotheight+ybottom+ydiff1, m_pUtil->m_ColorBackground);
					else 
						c = new  XYChart(xplotwidth+xleft+xdiff, yplotheight+ybottom+ydiff1, m_pUtil->m_ColorBackground);
					plotarea=c->setPlotArea(xleft, ydiff1, xplotwidth, yplotheight,0x00FFFFFF,0x00FFFFFF,0xFF000000,0xFF000000);
					MakeSoilWater(c, xplotwidth, yplotheight);
					if(createlegend)
						c->addLegend(legendposX+xleft, legendposY+ydiff1, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);

				}
				else if(yrow==NumY-1&&xcol==NumX-1) {
					//lower right

					c = new  XYChart(xplotwidth+xright+xdiff1, yplotheight+ybottom+ydiff1, m_pUtil->m_ColorBackground);
					plotarea=c->setPlotArea(xdiff1, ydiff1,xplotwidth, yplotheight,0x00FFFFFF,0x00FFFFFF,0xFF000000,0xFF000000);
					if(m_HeatEq )
							 MakeSoilHeat(c,xplotwidth , yplotheight, heatmade);
					else if(m_HBV) MakeHBV(c, xplotwidth, yplotheight);
					else if(m_NC>0) MakeSoilOrg(c, xplotwidth, yplotheight);

					if(createlegend)
						c->addLegend(legendposX+xdiff1, legendposY+ydiff1, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);

				}
				else if(yrow==NumY-1&&xcol==NumX-2) {
					//lower right

					c = new  XYChart(xplotwidth+xright+xdiff1, yplotheight+ybottom+ydiff1, m_pUtil->m_ColorBackground);
					plotarea=c->setPlotArea(xdiff1, ydiff1,xplotwidth, yplotheight,0x00FFFFFF,0x00FFFFFF,0xFF000000,0xFF000000);
					if(!m_HBV&& m_NC>0)	MakeSoilMinN(c, xplotwidth, yplotheight);
					else if(m_HBV&&m_NC) MakeSoilOrg(c, xplotwidth, yplotheight);
					else if(m_HeatEq) MakeSoilHeat(c,xplotwidth , yplotheight, heatmade);
						

					if(createlegend)
						c->addLegend(legendposX+xdiff1, legendposY+ydiff1, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);

				}
				else if(yrow==NumY-1&&xcol==NumX-3) {
					//lower midde
					c = new  XYChart(xplotwidth+xdiff, yplotheight+ybottom+ydiff, m_pUtil->m_ColorBackground);
				//	c->setWallpaper("bild.png");
					m_pProp->m_NoYAxisScale=true;
					plotarea=c->setPlotArea(xdiff1, ydiff1,xplotwidth , yplotheight,0x00FFFFFF,0x00FFFFFF,0xFF000000,0xFF000000);
					if(m_HBV&&m_NC) MakeSoilMinN(c, xplotwidth, yplotheight);
					else MakeSoilHeat(c,xplotwidth , yplotheight, heatmade);

					//plotarea->setWallpaper("bild.png");
					if(createlegend)
						c->addLegend(legendposX+xdiff1, legendposY+ydiff1, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);

				}
				else if(yrow==NumY-1&&xcol==NumX-4) {
					//lower midde
					c = new  XYChart(xplotwidth+xdiff, yplotheight+ybottom+ydiff, m_pUtil->m_ColorBackground);
				//	c->setWallpaper("bild.png");
					m_pProp->m_NoYAxisScale=true;
					plotarea=c->setPlotArea(xdiff1, ydiff1,xplotwidth , yplotheight,0x00FFFFFF,0x00FFFFFF,0xFF000000,0xFF000000);
					MakeSoilHeat(c,xplotwidth , yplotheight, heatmade);

					//plotarea->setWallpaper("bild.png");
					if(createlegend)
						c->addLegend(legendposX+xdiff1, legendposY+ydiff1, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);

				}

				else if(xcol==0) {
					c = new  XYChart(xplotwidth+xleft+xdiff2, yplotheight+ydiff, m_pUtil->m_ColorBackground);
					m_pProp->m_NoXAxisScale=true;
					plotarea=c->setPlotArea(xleft, ydiff1,xplotwidth , yplotheight);

					if(createlegend)
						c->addLegend(legendposX+xleft, legendposY+ydiff1, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);

				}
				else if(xcol==NumX-2) {
					c = new  XYChart(xplotwidth+xright+xdiff1, yplotheight+ydiff, m_pUtil->m_ColorBackground);
					m_pProp->m_NoXAxisScale=true;
					m_pProp->m_NoYAxisScale=true;
					plotarea=c->setPlotArea(xdiff1, ydiff1, xplotwidth, yplotheight);
					if(createlegend)
						c->addLegend(legendposX+xdiff1, legendposY+ydiff1, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);

				}
				else {
					c = new  XYChart(xplotwidth+xdiff, yplotheight+ydiff, m_pUtil->m_ColorBackground);
					m_pProp->m_NoXAxisScale=true;
					m_pProp->m_NoYAxisScale=true;
					plotarea=c->setPlotArea(xdiff1, ydiff1,xplotwidth , yplotheight);
					if(createlegend)
						c->addLegend(legendposX+xdiff1, legendposY+ydiff1, vertical_legends, "arialbd.ttf", m_pUtil->m_LegendSizePoint)->setBackground(m_pUtil->m_ColorLegendBackground, m_pUtil->m_ColorLegendBackgroundFrame);

				}
				//c->addTitle(Chart::TopCenter,m_Title,"arialbd.ttf", m_TitleSizePoint);
				
				if(xcol==0&&yrow==0)
					mc->addChart(0,0,c);
				else if(yrow==0&&xcol>0) 
					mc->addChart((xcol-1)*(xplotwidth+xdiff)+(xplotwidth+xleft+xdiff2),0,c);
				else if(xcol==0&&yrow>0) {
					mc->addChart(0,(yrow-1)*(yplotheight+ydiff)+(yplotheight+ydiff2+ytop),c);
				}
				else {
					mc->addChart((xcol-1)*(xplotwidth+xdiff)+(xplotwidth+xleft+xdiff2),
					(yrow-1)*(yplotheight+ydiff)+(yplotheight+ydiff2+ytop),c);
				}
				m_XY_Chart.push_back(c);
				//delete m_pChartViewer->getChart();
		}
	}

	CCoupModelView *pView;
	pView=m_pSimDoc->GetModelView();
	pView->MakeChart(mc);
	if(m_pUtil->m_FileSave.m_SaveFile)
		m_pUtil->m_FileSave.MakeFile(m_pSimDoc);	
	m_pUtil->m_Displayed=true;
	m_pProp->m_Title="";
	m_pMC=mc;
	SetImageMap();
}
void ModelView::SetImageMap() {
		ImageMapHandler *handler=m_pChartViewer->getImageMapHandler();
		m_pChartViewer->setImageMap(m_pChartViewer->getChart()->getHTMLImageMap("clickable", "",
				"title='[{dataSetName}] {x}: {value}'"));


}
void ModelView::MakeModelPlot() {

	Sw *pSw;
	m_HeatEq=true;
	m_NC=m_HBV=m_WaterEq=0;
	m_LateralInput=m_Evapo=m_PlantType=m_SaltTracer=m_SnowPack=m_SoilVapour=m_NC=m_Irrigation=m_Gw=0;


	for(size_t i=0;i<m_pSimDoc->m_DefinedSwitches.size();i++) {
		pSw=m_pSimDoc->m_DefinedSwitches[i];

		if(pSw->GetName()=="HeatEq")
			m_HeatEq=false;
		else if(pSw->GetName()=="WaterEq")
			m_WaterEq=pSw->GetIntValue();
		else if(pSw->GetName()=="Nitrogen and Carbon")
			m_NC=pSw->GetIntValue();
		else if(pSw->GetName()=="HBV Soil Module") {
			m_HBV=pSw->GetIntValue();
			if(m_HBV==2) {
				m_WaterEq=0;
			}
		}
		else if(pSw->GetName()=="Evaporation")
			m_Evapo=pSw->GetIntValue();
		else if(pSw->GetName()=="GroundWaterFlow")
			m_Gw=pSw->GetIntValue();
		else if(pSw->GetName()=="Irrigation")
			m_Irrigation=pSw->GetIntValue();
		else if(pSw->GetName()=="PlantType")
			m_PlantType=pSw->GetIntValue();
		else if(pSw->GetName()=="SaltTracer")
			m_SaltTracer=pSw->GetIntValue();
		else if(pSw->GetName()=="SnowPack")
			m_SnowPack=pSw->GetIntValue();
		else if(pSw->GetName()=="SoilVapour")
			m_SoilVapour=pSw->GetIntValue();
		else if(pSw->GetName()=="LateralInput")
			m_LateralInput=pSw->GetIntValue();
	};
	InitData();
	if(m_pChartViewer==nullptr) return;
	DrawModelPlot();

}
void ModelView::CleanModelPlot()
{
	while(m_XY_Chart.size()>0) {
		delete m_XY_Chart[0];
		m_XY_Chart.erase(m_XY_Chart.begin());
	}
	if(m_pMC!=nullptr) {
		delete m_pMC;
		m_pMC=nullptr;
	}
}
void ModelView::MakeSoilWater(XYChart *c, int xplotwidth, int yplotheight)
{
	VectorLayer *vectorlayer;
					if(m_WaterEq==1) {
						vectorlayer=c->addVectorLayer(m_dataSeriesA[60],m_dataSeriesA[61],m_dataSeriesA[62],m_dataSeriesA[63],0,0x0000FF,"Precipitation");
						vectorlayer->setArrowAlignment(8);
						vectorlayer->setArrowHead(10,20);
						vectorlayer->setLineWidth(5);

						if(m_Evapo>0&&m_PlantType>0) {
							vectorlayer=c->addVectorLayer(m_dataSeriesA[64],m_dataSeriesA[65],m_dataSeriesA[66],m_dataSeriesA[67],0,0xCC0000FF,"Evaporation");
							vectorlayer->setArrowHead(10,20);
							vectorlayer->setLineWidth(5);

							vectorlayer=c->addVectorLayer(m_dataSeriesA[21],m_dataSeriesA[20],m_dataSeriesA[2],m_dataSeriesA[23],0,0x00FF00,"Water uptake");
							vectorlayer->setArrowAlignment(8);

							vectorlayer=c->addVectorLayer(m_dataSeriesA[68],m_dataSeriesA[69],m_dataSeriesA[70],m_dataSeriesA[71],0,0x00FF00,"Transpiration");
							vectorlayer->setArrowHead(10,20);
							vectorlayer->setLineWidth(6);

						}
						else if(m_Evapo>0) {
							vectorlayer=c->addVectorLayer(m_dataSeriesA[64],m_dataSeriesA[65],m_dataSeriesA[66],m_dataSeriesA[67],0,0xCC0000FF,"Evaporation");
							vectorlayer->setArrowHead(10,20);
							vectorlayer->setLineWidth(5);


						}

						//vectorlayer=c->addVectorLayer(m_dataSeriesA[1],m_dataSeriesA[0],m_dataSeriesA[2],m_dataSeriesA[3],0,0x0000FF,"Evaporation");
						if(m_Gw>0) {
							vectorlayer=c->addVectorLayer(m_dataSeriesA[31],m_dataSeriesA[0],m_dataSeriesA[2],m_dataSeriesA[3],0,0x0000FF,"Drainage");
							vectorlayer->setArrowAlignment(8);
						}

						vectorlayer=c->addVectorLayer(m_dataSeriesA[6],m_dataSeriesA[10],m_dataSeriesA[12],m_dataSeriesA[13],0,0x0000FF,"Water Flow Upp"); // Water flow upp
						vectorlayer=c->addVectorLayer(m_dataSeriesA[6],m_dataSeriesA[10],m_dataSeriesA[12],m_dataSeriesA[33],0,0x0000FF,"Water Flow Down"); // Water Flow down

						if(m_LateralInput>0) 
							vectorlayer=c->addVectorLayer(m_dataSeriesA[41],m_dataSeriesA[30],m_dataSeriesA[22],m_dataSeriesA[3],0,0x0000FF, "Drainage Water Flow"); // Horizontal inflow
					}
					else if(m_HeatEq) {
						vectorlayer=c->addVectorLayer(m_dataSeriesA[6],m_dataSeriesA[10],m_dataSeriesA[12],m_dataSeriesA[13],0,0xFF0000,"Soil Heat Flow"); // Heat Flow
						vectorlayer=c->addVectorLayer(m_dataSeriesA[6],m_dataSeriesA[10],m_dataSeriesA[12],m_dataSeriesA[33],0,0xFF0000,"Soil Heat Flow");


					}
					else if(m_HBV==2) {



					};
					if(m_HBV!=2) {
						BoxWhiskerLayer *pBoxLayer;
						if(m_WaterEq==1)
							pBoxLayer=c->addBoxLayer(m_dataSeriesA[5], m_dataSeriesA[4],0xBB0000FF,"Water Storage");
						else
							pBoxLayer=c->addBoxLayer(m_dataSeriesA[5], m_dataSeriesA[4],0xBBFF0000,"Heat Storage");

						pBoxLayer->setDataGap(0.);
						//l1->addDataSet(1,&AA[0][i]);
						pBoxLayer->setXData(m_dataSeriesA[6]);
						pBoxLayer->setDataWidth(xplotwidth/2);
						c->xAxis()->setLinearScale(0., 2.,2.,2.);
						c->xAxis()->setLabelStyle(0,0,0xFF000000);
						c->yAxis()->setReverse();

					}

			
}
void ModelView::MakeSoilHeat(XYChart *c, int xplotwidth, int yplotheight, bool heatmade)
{
		VectorLayer *vectorlayer;
						if(m_HeatEq&&!heatmade) {
						vectorlayer=c->addVectorLayer(m_dataSeriesA[6],m_dataSeriesA[10],m_dataSeriesA[12],m_dataSeriesA[13],0,0xFF0000,"Heat Flow"); // Heat Flow
						vectorlayer=c->addVectorLayer(m_dataSeriesA[6],m_dataSeriesA[10],m_dataSeriesA[12],m_dataSeriesA[33],0,0xFF0000,"Heat Flow");

						BoxWhiskerLayer *pBoxLayer;
						pBoxLayer=c->addBoxLayer(m_dataSeriesA[5], m_dataSeriesA[4],0xBBFF0000,"Heat Storage");
						//l1->addDataSet(1,&AA[0][i]);
						pBoxLayer->setXData(m_dataSeriesA[6]);
						pBoxLayer->setDataWidth(xplotwidth/2);
						c->xAxis()->setLinearScale(0, 2,2,2);
						c->xAxis()->setLabelStyle(0,0);
						c->xAxis()->setLabelStyle(0,0,0xFF000000);
						c->yAxis()->setLabelStyle(0,0,0xFF000000);
						c->yAxis()->setReverse();

						heatmade=true;

					}
}
void ModelView::MakeSoilMinN(XYChart *c, int xplotwidth, int yplotheight)
{
			VectorLayer *vectorlayer;

			vectorlayer=c->addVectorLayer(m_dataSeriesA[6],m_dataSeriesA[10],m_dataSeriesA[12],m_dataSeriesA[13],0,0xFF0000,"Min N Infiltration"); 
			//vectorlayer=c->addVectorLayer(m_dataSeriesA[6],m_dataSeriesA[10],m_dataSeriesA[12],m_dataSeriesA[33],0,0xFF0000,"Min N Infiltration");

			BoxWhiskerLayer *pBoxLayer;
			pBoxLayer=c->addBoxLayer(m_dataSeriesA[5], m_dataSeriesA[4],0xBBFFCC00,"Mineral N");
			//l1->addDataSet(1,&AA[0][i]);
			pBoxLayer->setXData(m_dataSeriesA[6]);
			pBoxLayer->setDataWidth(xplotwidth/2);
			c->xAxis()->setLinearScale(0, 2,2,2);
			c->xAxis()->setLabelStyle(0,0);
			c->xAxis()->setLabelStyle(0,0,0xFF000000);
			c->yAxis()->setLabelStyle(0,0,0xFF000000);
			c->yAxis()->setReverse();


}
void ModelView::MakeSoilOrg(XYChart *c, int xplotwidth, int yplotheight)
{
			VectorLayer *vectorlayer;

			vectorlayer=c->addVectorLayer(m_dataSeriesA[6],m_dataSeriesA[10],m_dataSeriesA[12],m_dataSeriesA[13],0,0xFF0000,"Litter Input"); // Heat Flow
			//vectorlayer=c->addVectorLayer(m_dataSeriesA[6],m_dataSeriesA[10],m_dataSeriesA[12],m_dataSeriesA[33],0,0xFF0000,"Litter Input");

			BoxWhiskerLayer *pBoxLayer;
			pBoxLayer=c->addBoxLayer(m_dataSeriesA[5], m_dataSeriesA[4],0xBBBB0000,"Soil Org");
			//l1->addDataSet(1,&AA[0][i]);
			pBoxLayer->setXData(m_dataSeriesA[6]);
			pBoxLayer->setDataWidth(xplotwidth/2);
			c->xAxis()->setLinearScale(0, 2,2,2);
			c->xAxis()->setLabelStyle(0,0);
			c->xAxis()->setLabelStyle(0,0,0xFF000000);
			c->yAxis()->setLabelStyle(0,0,0xFF000000);
			c->yAxis()->setReverse();		
			
			vectorlayer=c->addVectorLayer(m_dataSeriesA[1],m_dataSeriesA[0],m_dataSeriesA[2],m_dataSeriesA[3],0,0xFFBBBB, "Respiration Flux"); // Horizontal inflow
			vectorlayer=c->addVectorLayer(m_dataSeriesA[11],m_dataSeriesA[20],m_dataSeriesA[22],m_dataSeriesA[3],0,0x00FF00,"Root Litter Input Flux");

						
			vectorlayer->setArrowAlignment(8);
		double xpos, xwidth, top, bottom;
		top=0.;
		P *pCPtLower=m_pSimDoc->GetPPointer("mLower Depth");
		size_t nlay=pCPtLower->GetSize();
		bottom=AA[0][nlay-1];
		pBoxLayer=c->addBoxLayer(DoubleArray(&top,1), DoubleArray(&bottom,1)	,0xBBDDFF00,"Root Biomass");
		xpos=0.;
		pBoxLayer->setXData(DoubleArray(&xpos,1));
		pBoxLayer->setDataWidth(xplotwidth/20);
		pBoxLayer=c->addBoxLayer(DoubleArray(&top,1), DoubleArray(&bottom,1)	,0xBBFFBB00,"C Mobile");
		xpos=-0.25;
		pBoxLayer->setXData(DoubleArray(&xpos,1));
		pBoxLayer->setDataWidth(xplotwidth/20);

		xpos=-0.15;
		double ypos=bottom*0.5;
		double length=0.25;
		double dir=90;
		vectorlayer=c->addVectorLayer(DoubleArray(&xpos,1),DoubleArray(&ypos,1), DoubleArray(&length,1),DoubleArray(&dir,1),2,0xAAFFAA,"C Mobile Root");
	




}
void ModelView::MakeNC_Plant(XYChart *c, int xplotwidth, int yplotheight){
		size_t nplant;
		double MaxHeight;
		MaxHeight=0.;
		VectorLayer *vectorlayer;
		BoxWhiskerLayer *pBoxLayer;
		Sw *pSw=m_pSimDoc->GetSw("CanopyHeightInput");
		P *pCPtMaxHeight=nullptr;
		if(pSw->GetIntValue()==2) {
			pCPtMaxHeight=m_pSimDoc->GetP("Max Height");
		}
		else if(pSw->GetIntValue()==0) {
			pCPtMaxHeight=m_pSimDoc->GetP("hOptimum Value");
		};
		if (pCPtMaxHeight != nullptr)
			nplant = pCPtMaxHeight->GetSize();
		else
			return;

		Ps *pP=m_pSimDoc->GetPs("ReferenceHeight");
		double RefHeight=pP->GetValue();
		double maxheight;
		maxheight=RefHeight;
		double xpos, xwidth, top, bottom;
		xpos = 0;
		double length;
		double dir;
		for(size_t i=0; i<nplant;i++)	{
			top=double(pCPtMaxHeight->GetValue(i))+0.5;
			if(maxheight<top) maxheight=top;
		}


		double ishift=1./nplant/5;
			
		if(m_PlantType>2) 
			P *pCPtMaxCover=m_pSimDoc->GetP("Max Cover");

		for(size_t i=0; i<nplant;i++) {
			top=double(pCPtMaxHeight->GetValue(i));
			if(top>MaxHeight) MaxHeight=top;
			bottom=double(pCPtMaxHeight->GetValue(i)*0.7);
			xpos=double(i)*ishift+1.;
			xwidth=1.;
			if(m_PlantType>2) {
				P *pCPtMaxCover=m_pSimDoc->GetP("Max Cover");
				xwidth=double(pCPtMaxCover->GetValue(i));
			};
			pBoxLayer=c->addBoxLayer(DoubleArray(&top,1), DoubleArray(&bottom,1)	,0xBB00FF00,"Leaf Biomass");
			pBoxLayer->setXData(DoubleArray(&xpos,1));
			pBoxLayer->setDataWidth(int(xplotwidth*xwidth/1.5));
			bottom=0.;
			pBoxLayer=c->addBoxLayer(DoubleArray(&top,1), DoubleArray(&bottom,1)	,0xBBFFFF00,"Stem Biomass");
			xpos=xpos-1;
			pBoxLayer->setXData(DoubleArray(&xpos,1));
			pBoxLayer->setDataWidth(xplotwidth/20);

			pBoxLayer=c->addBoxLayer(DoubleArray(&top,1), DoubleArray(&bottom,1)	,0xBBFFFF00,"C Mobile");
			double xxpos=-0.25;
			pBoxLayer->setXData(DoubleArray(&xxpos,1));
			pBoxLayer->setDataWidth(xplotwidth/20);

			dir=90.;
			xpos=-0.15;
			double ypos=top*0.9;
			length=4.;
			vectorlayer=c->addVectorLayer(DoubleArray(&xpos,1),DoubleArray(&ypos,1), DoubleArray(&length,1),DoubleArray(&dir,1),2,0xAAFFAA,"C Mobile Leaf");

			xpos=-0.15;
			ypos=top*0.1;
			length=0.25;
			vectorlayer=c->addVectorLayer(DoubleArray(&xpos,1),DoubleArray(&ypos,1), DoubleArray(&length,1),DoubleArray(&dir,1),2,0xAAFFAA,"C Mobile Stem");
		

	
			dir=180;
			xpos=-0.25;
			length=maxheight-top;
			vectorlayer=c->addVectorLayer(DoubleArray(&xpos,1),DoubleArray(&maxheight,1), DoubleArray(&length,1),DoubleArray(&dir,1),2,0xBBFFBB,"Photosynthesis");
			xpos+=1;
			dir=0;
			length=maxheight-top;
			vectorlayer=c->addVectorLayer(DoubleArray(&xpos,1),DoubleArray(&top,1), DoubleArray(&length,1),DoubleArray(&dir,1),2,0xBBBBFF,"LeafRespiration");
		}					
		xpos+=1.5;
		length=length=maxheight;
		vectorlayer=c->addVectorLayer(DoubleArray(&xpos,1),DoubleArray(&bottom,1), DoubleArray(&length,1),DoubleArray(&dir,1),2,0xFFBBBB,"Soil Respiration");

		pBoxLayer=c->addBoxLayer(DoubleArray(&top,1), DoubleArray(&bottom,1)	,0xBBFFBB00,"C Mobile");
		xpos=-0.25;
		pBoxLayer->setXData(DoubleArray(&xpos,1));
		pBoxLayer->setDataWidth(xplotwidth/20);




		c->xAxis()->setLinearScale(0, 2);
		c->yAxis()->setLogScale(0.1, maxheight);
}
void ModelView::MakeNC_GasFlow(XYChart *c, int xplotwidth, int yplotheight)
{

}
void ModelView::MakeHBV(XYChart *c, int xplotwidth, int yplotheight)
{
		VectorLayer *vectorlayer;
		BoxWhiskerLayer *pBoxLayer;
		double xpos, xwidth, top, bottom, ypos;
		double length;
		double dir;
		double xx[2], yy[2];


		vectorlayer=c->addVectorLayer(m_dataSeriesA[60],m_dataSeriesA[61],m_dataSeriesA[62],m_dataSeriesA[63],0,0x0000FF,"Precipitation");
		vectorlayer->setArrowAlignment(8);
		vectorlayer->setArrowHead(10,20);
		vectorlayer->setLineWidth(5);

		top=0.5;
		bottom=0.;
		xpos=0.5;
		xwidth=2.;

		pBoxLayer=c->addBoxLayer(DoubleArray(&top,1), DoubleArray(&bottom,1)	,0xDD0000EE,"Soil Water");
		pBoxLayer->setXData(DoubleArray(&xpos,1));
		pBoxLayer->setDataWidth(int(xplotwidth/2));
		xpos+=0.5;
		ypos=0.3;
		length=xplotwidth/4;
		dir=90.;
		xx[0]=1;xx[1]=1.7;
		yy[0]=yy[1]=0.3;
		vectorlayer=c->addVectorLayer(DoubleArray(&xx[0],1), DoubleArray(&yy[0],1),DoubleArray(&length,1),DoubleArray(&dir,1), 0,0x0000FF,"Evaporation");
		dir=0.;
		vectorlayer->setArrowHead(1,1);
		vectorlayer->setLineWidth(5);
		vectorlayer=c->addVectorLayer(DoubleArray(&xx[1],1), DoubleArray(&yy[1],1),DoubleArray(&length,1),DoubleArray(&dir,1), 0,0x0000FF,"Evaporation");
		vectorlayer->setArrowHead(15,20);
		vectorlayer->setLineWidth(5);				


		top=1.1;
		bottom=0.7;
		xpos=0.5;
		ypos=0.9;
		pBoxLayer=c->addBoxLayer(DoubleArray(&top,1), DoubleArray(&bottom,1)	,0xBB0000EE,"Peak GW");
		pBoxLayer->setXData(DoubleArray(&xpos,1));
		pBoxLayer->setDataWidth(int(xplotwidth/2));

		dir=90.;
		xx[0]=1;xx[1]=1.7;
		yy[0]=yy[1]=0.9;
		vectorlayer=c->addVectorLayer(DoubleArray(&xx[0],1), DoubleArray(&yy[0],1),DoubleArray(&length,1),DoubleArray(&dir,1), 0,0x0000FF,"Peak Flow");
		vectorlayer->setLineWidth(5);

		length=yplotheight/2.9;
		dir =180.;
		ypos=0.0;
		vectorlayer=c->addVectorLayer(DoubleArray(&xpos,1), DoubleArray(&ypos,1),DoubleArray(&length,1),DoubleArray(&dir,1), 0,0x0000FF,"GW Inflow");
		vectorlayer->setArrowHead(15,20);
		vectorlayer->setLineWidth(5);	


		length=yplotheight/6;
		dir =180.;
		ypos=1.1;
		vectorlayer=c->addVectorLayer(DoubleArray(&xpos,1), DoubleArray(&ypos,1),DoubleArray(&length,1),DoubleArray(&dir,1), 0,0x0000FF,"GW Redist");
		vectorlayer->setArrowHead(15,20);
		vectorlayer->setLineWidth(5);	
		
		length=yplotheight/6;
		dir =0.;
		ypos=1.3;
		vectorlayer=c->addVectorLayer(DoubleArray(&xpos,1), DoubleArray(&ypos,1),DoubleArray(&length,1),DoubleArray(&dir,1), 0,0x0000FF,"GW Redist");
		vectorlayer->setArrowHead(15,20);
		vectorlayer->setLineWidth(5);	




		top=1.7;
		bottom=1.3;

		pBoxLayer=c->addBoxLayer(DoubleArray(&top,1), DoubleArray(&bottom,1)	,0xBB0000EE,"Base GW");
		pBoxLayer->setXData(DoubleArray(&xpos,1));
		pBoxLayer->setDataWidth(int(xplotwidth/2.));

		dir=90.;
		xx[0]=1;xx[1]=1.7;
		yy[0]=yy[1]=1.5;
		length=xplotwidth/4;
		vectorlayer=c->addVectorLayer(DoubleArray(&xx[0],1), DoubleArray(&yy[0],1),DoubleArray(&length,1),DoubleArray(&dir,1), 0,0x0000FF,"Base Flow");
		vectorlayer->setLineWidth(5);


		ypos=1.2;
		vectorlayer=c->addVectorLayer(DoubleArray(&xx[1],1), DoubleArray(&ypos,1),DoubleArray(&length,1),DoubleArray(&dir,1), 0,0x0000FF,"RunOff");
		vectorlayer->setArrowHead(25,30);
		vectorlayer->setLineWidth(10);	
		dir=0.;
		length=xplotwidth/3.6;
		vectorlayer=c->addVectorLayer(DoubleArray(&xx[1],1), DoubleArray(&yy[0],1),DoubleArray(&length,1),DoubleArray(&dir,1), 0,0x0000FF,"Flow");

		vectorlayer->setArrowHead(1,1);
		vectorlayer->setLineWidth(5);


		c->xAxis()->setLinearScale(0., 2.,2.,2.);
		c->yAxis()->setLinearScale(0., 2.,2.,2.);
		c->yAxis()->setReverse();


}
