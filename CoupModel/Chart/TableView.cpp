
#include "../stdafx.h"
#include "../std.h"
#include "../Util.h"
#include "../CoupModelView.h"
#include "chartdir.h"
#include "./ChartViewer.h"
#include "./TableView.h"
#include "../NewBase\Doc.h"
#include "./ChartProperties\ChartProp.h"
#include "./ChartProperties\ChartUtil.h"
#include "../CUG\ChartProperties\ChartCug.h"


TableView::TableView()
{
	m_pSimDoc=nullptr;
	m_pProp=MySingle::instance().GetChartPropPointer();
	m_pUtil=MySingle::instance().GetChartUtilPointer();
	m_pChartViewer=nullptr;
	m_Stacked = true;
	m_DepthProfile = false;
}
TableView::~TableView()
{

}
void TableView::InitDoc(Doc *pDoc) {
	//m_pDoc=pDoc;
	m_pSimDoc=pDoc;
	m_pView=m_pSimDoc->GetModelView();
	m_pChartViewer=&m_pView->m_ChartViewer;
}
bool TableView::MakeTablePlot(Tab *pTab, P *pCPt) {

	//m_TextPlot=txt;
	P *pCPtUpper, *pCPtLower;
	m_DepthProfile=false;
	m_Data.NumOfPoints=pTab->GetNumOfRow();
	auto group=pCPt->GetGroup();

	if(pCPt->GetName()=="Thickness Layers") {
		m_Label.ytitle1="Soil Depth (m)";
		m_pProp->m_Title="Soil Profile";
		m_Stacked=true;
	}
	else if(m_Data.NumOfPoints==1) {
			return false;
	}
	else if(group.find("Plant")!= std::string::npos) {
				return false;

	}
	else {
		m_Label.ytitle1="Soil Depth (m)";
		string umdep = "mUpper Depth";
		string lmdep = "mLower Depth";
			m_pProp->m_Title = pTab->GetName().c_str();
		if(m_pProp->m_Title.find("measured horizons")>0) {
			string udep = "Upper Depth";
			string ldep = "Lower Depth";
			pCPtUpper=m_pSimDoc->GetP(udep);
			pCPtLower=m_pSimDoc->GetP(ldep);
		}
		else {
			pCPtUpper=m_pSimDoc->GetP(umdep);
			pCPtLower=m_pSimDoc->GetP(lmdep);
		}
		m_pProp->m_Title=pCPt->GetName().c_str();
		group=pCPt->GetGroup();
		string unit;
		unit=pCPt->GetUnit();
		if(unit.size()>0) {
			m_pProp->m_Title+='(';m_pProp->m_Title+=unit;m_pProp->m_Title+=")";
		}
		m_Stacked=false;
		m_DepthProfile=true;

		if(pCPtUpper==nullptr) {
			group="Soil Hydraulic";
			pCPtUpper=(P*)m_pSimDoc->GetPtr(simtype::PAR_TABLE,umdep);	
			pCPtLower=(P*)m_pSimDoc->GetPtr(simtype::PAR_TABLE,lmdep);

		}
		if(pCPtUpper==nullptr) {

			
			MySingle::instance().GetChartCugPointer()->MakeTextPlot("Independent variable not find");
			return false;

		}
	}

	if(m_pProp->GetTitle()=="-") {
			m_pProp->SetTitle(m_pProp->m_Title);
	}
	m_pProp->m_Title=m_pProp->GetTitle();
	
	

	if(m_Stacked) {
		for(size_t i=0; i<pTab->GetNumOfRow();i++) {
			AA[i][0]=double(-1.*pCPt->GetValue(i));
			m_dataSeriesA[i] = DoubleArray(&AA[i][0], 1);
		}
	}
	else if(m_DepthProfile) {
		int count=0;
		for(size_t i=0; i<pTab->GetNumOfRow();i++) {
			AA[0][count]=double(pCPt->GetValue(i));
			TT[count]=double(pCPtUpper->GetValue(i));
			count++;
			AA[0][count]=double(pCPt->GetValue(i));
			TT[count]=double(pCPtLower->GetValue(i));
			count++;
		}
		m_Data.NumOfPoints=count;
		m_dataSeriesA[0] = DoubleArray(&AA[0][0], m_Data.NumOfPoints);
		m_dataSeriesB[0] = DoubleArray(&TT[0], m_Data.NumOfPoints);


	}
	else {
		for(size_t i=0; i<pTab->GetNumOfRow();i++)
			AA[0][i]=double(pCPt->GetValue(i));
		m_dataSeriesA[0] = DoubleArray(&AA[0][0], m_Data.NumOfPoints);

	}
	
	if (m_pChartViewer == nullptr && m_pSimDoc != nullptr)
		InitDoc(m_pSimDoc);
	if (m_pChartViewer == nullptr) return false;
	DrawTablePlot(m_pChartViewer);
	return true;

}
void TableView::DrawTablePlot(CChartViewer *viewer)
{
	if(viewer==nullptr)
		viewer=m_pChartViewer;


	XYChart *c;
	c = new  XYChart(m_pProp->m_DisplayXSize, m_pProp->m_DisplayYSize, m_pUtil->m_ColorBackground);
	int xleft=m_pProp->GetLeftMargin();
	int ytop=m_pProp->GetTopMargin();
	int ybottom=m_pProp->GetBottomMargin();
	int xright=m_pProp->GetRightMargin();
	int	yplotheight=m_pProp->m_DisplayYSize-ytop-ybottom;
	int	xplotwidth=m_pProp->m_DisplayXSize-xleft-xright;
	
	c->setPlotArea(xleft, ytop, xplotwidth, yplotheight, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);

	if(m_pProp->m_MakeChartTitle)
		c->addTitle(Chart::TopCenter,m_pProp->m_Title.c_str(),"arialbd.ttf", m_pUtil->m_TitleSizePoint);
	int	visible_legends, dim;
	visible_legends=m_pProp->GetVisibleFrames();

	if(visible_legends==0) {
		m_pUtil->m_ColorLegendBackground=Chart::Transparent;
		m_pUtil->m_ColorLegendBackgroundFrame=Chart::Transparent;	
	}
	else if(visible_legends<4) {
		dim=(4-visible_legends)*80*0x1000000;
		m_pUtil->m_ColorLegendBackground+=dim;
		m_pUtil->m_ColorLegendBackgroundFrame+=dim;	
	}
	AreaLayer *pAreaLayer=c->addAreaLayer();
	BarLayer *layer=c->addBarLayer(Chart::Stack, 0);
	if(m_Stacked) {
		for(int ivar=0;ivar<m_Data.NumOfPoints;ivar++) { 
			m_Label.st='(';
			m_Label.st+=FUtil::STD_ItoAscii(ivar+1);
			m_Label.st+=")";

			layer->addDataSet(m_dataSeriesA[ivar], m_pUtil->m_ColorLegendBackground, m_Label.st.c_str());
		}
		c->yAxis()->setTitle(m_Label.ytitle1.c_str(),"arialbd.ttf",  m_pUtil->m_LabelSizePoint);
	}
	else if(m_DepthProfile) {

		    // Swap the x and y axis to become a rotated chart
		c->swapXY();

		// Set the y axis on the top side (right + rotated = top)
		c->setYAxisOnRight();
		c->xAxis()->setReverse();
		m_Label.st="";
		int iv=m_pUtil->GetHColorLine(0);
		int m_colorbox=m_pProp->GetBoxDimmer()+iv;
		for(int ivar=0;ivar<1;ivar++) {
			pAreaLayer->addDataSet(m_dataSeriesA[ivar],  m_pUtil->m_ColorLegendBackground ,m_Label.st.c_str());
			pAreaLayer->setXData(m_dataSeriesB[ivar]);
		}
		
		c->xAxis()->setTitle(m_Label.ytitle1.c_str(),"arialbd.ttf",  m_pUtil->m_LabelSizePoint);

	}
	else {

		for(int ivar=0;ivar<1;ivar++)
			layer->addDataSet(m_dataSeriesA[ivar], m_pUtil->m_ColorLegendBackground, m_Label.st.c_str());


	}
	layer->setAggregateLabelStyle();
	layer->setDataLabelStyle();




	m_pView->MakeChart(c);
	m_XY_Chart.push_back(c);
	if(m_pUtil->m_FileSave.m_SaveFile)
		m_pUtil->m_FileSave.MakeFile();	

	ImageMapHandler *handler=viewer->getImageMapHandler();
	viewer->setImageMap(viewer->getChart()->getHTMLImageMap("clickable", "",
		"title='[{dataSetName}] {x}: {value}'"));


	m_pUtil->m_Displayed=true;
   

}
void TableView::CleanTablePlot() {

		
	while(m_XY_Chart.size()>0) {
		delete m_XY_Chart[0];
		m_XY_Chart.erase(m_XY_Chart.begin());
	}


}

