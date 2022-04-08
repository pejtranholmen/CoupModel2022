
// D:\Dev\Chart\ParameterPlotDlg.cpp : implementation file
//

#include "..\stdafx.h"
#include "..\Util.h"
#include "..\ModelTypes\SimB.h"
#include "..\ModelTypes\Func.h"

#include "..\CoupModel.h"
#include "..\CoupModelDoc.h"
#include "..\CoupModelView.h"
#include "..\MainFrm.h"


#include "HotSpotDlg.h"
#include "chartdir.h"
#include <math.h>
#include <time.h>
#include <algorithm>
#include ".\parameterplot.h"
#include <limits>
#include "..\Cug\ChartProperties\ChartCug.h"

using namespace std;



/////////////////////////////////////////////////////////////////////////////
// ParameterPlot dialog

//
// Constructor
//
ParameterPlot::ParameterPlot(CWnd* pParent /*=nullptr*/)
    : CWnd()
{
    //{{AFX_DATA_INIT(ParameterPlot)
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pProp=MySingle::instance().GetChartPropPointer();
	m_pUtil=MySingle::instance().GetChartUtilPointer();


	m_minValue=m_maxValue=0.;
	m_minXValue=m_maxXValue=0.;

	m_pCP=nullptr;
	m_pProp->m_MultiValid.AddOnlyValidRuns=TRUE;

	m_PFMeasuredPoints=FALSE;
	m_ExtraCurveCounter=-1;
	m_PreviousViewFunction=-1;
	m_Displayed=TRUE;
		CWinApp* pApp = AfxGetApp();
	m_InitDialog=FALSE;
	m_PlotPrevious=FALSE;
	m_PlotDefault=FALSE;
	m_PlotNewValue=FALSE;
	m_InitViewFunction=TRUE;

   // MFC_Util::GetProfileIntNo( "KeepPlotOnScreen", m_ContinuousHelp);

//	m_ContinuousHelp=TRUE;
	m_DisplaySize=1;
	m_cx=700;
	m_cy=400;

	m_pView=nullptr;
	m_ParNum=0;
	m_CommonScale=FALSE;
	m_TableIndex=0;

}

//
// Destructor
//
ParameterPlot::~ParameterPlot()
{
   // delete m_dataTable;
	
    delete m_ChartViewer.getChart();

	m_dataSeriesA[0];
}


bool ParameterPlot::InitData(Doc *pDoc, SimB *pSimB, Tab *pTab, size_t index, size_t FuncNo)
{
	CString str, typ, grp;



	int icount=0;
	m_Displayed=TRUE;


	if(pDoc!=nullptr) {
		m_pSimDoc=pDoc;
		m_pView=m_pSimDoc->GetModelView();
		m_pProp=MySingle::instance().GetChartPropPointer();
		m_pUtil=MySingle::instance().GetChartUtilPointer();
		int col=m_pUtil->GetColorBackground();
		m_pChartViewer=&m_pSimDoc->GetModelView()->m_ChartViewer;
		m_pPFCurve=m_pSimDoc->m_pPFCurve;
		m_pParTable=pTab;


		if(pSimB->GetType()==PAR_SINGLE) {
			m_pCP=dynamic_cast<Ps*>(pSimB);
			m_pCP->SetSelectedFunc(FuncNo);
			m_pCPt=nullptr;
			m_Func=m_pCP->GetFuncPointer();
			m_plotp=m_Func->GetPlotParam();
			m_Title=m_Func->GetName().c_str();
			m_MaxLayer=1;
			m_pParTable=nullptr;
			m_PFMeasuredPoints=false;
		}
		else {
			m_pCPt=dynamic_cast<P*>(pSimB);
			m_MaxLayer=m_pCPt->GetSize();
			m_pCP=nullptr;
			m_pCPt->SetSelectedFunc(FuncNo);
			m_Func=m_pCPt->GetFuncPointer();
			m_plotp=m_Func->GetPlotParam();
			m_Title=m_Func->GetName().c_str();
			if(m_Func->GetName()=="Measured Water Retention Curve"&&
				m_pSimDoc->DB_GetChoice(DB_types(1)).size()>0) {
				if(m_pPFCurve->IsProfile()) {

					if(m_pPFCurve->GetNumPF_Pressures()>0) m_PFMeasuredPoints=true;
					else m_PFMeasuredPoints=false;
				}
				m_Title+="\n";
				m_Title+=m_pPFCurve->m_ActProfile.OldKey.c_str();
				m_Title+=" ";
				m_Title+=m_pPFCurve->m_ActProfile.Name.c_str();

				string selected=m_pSimDoc->DB_GetChoice(DB_types(1));
				if(selected.find(m_pPFCurve->m_ActProfile.OldKey)==string::npos) {		
					m_PFMeasuredPoints=false;					
					m_Title=m_Func->GetName().c_str();

				}
			}
			else
				 m_PFMeasuredPoints=false;
			m_TableIndex=index;
			m_pParTable->ResetPlotIndex();
			m_pParTable->SetPlotIndex(index);
			m_plotp.Opt_Vec.clear();
			m_plotp.Opt_Vec.push_back(m_TableIndex);

		}
		
		m_PlotDefault= m_PlotPrevious= m_PlotNewValue=FALSE;
		

		FunctionProperties *pFuncProp;
		pFuncProp=MySingle::instance().GetFunctionPointer();
		m_Par_Dep=pFuncProp->GetParamDep(m_Func->GetName());

		m_Ps.clear();
		m_P.clear();
		for(size_t i=0; i<m_NewPlotValue.size(); i++) {
			for(size_t j=0; j<m_NewPlotValue[i].size(); j++) {
				m_NewPlotValue[i].clear();
				m_ResetPlotValue[i].clear();
				m_PreviousValue[i].clear();
			}
			m_NewPlotValue.clear();
			m_ResetPlotValue.clear();
			m_PreviousValue.clear();
		}
	
		m_NewPlotValue.resize(m_Par_Dep.P_Names.size()+m_Par_Dep.Ps_Names.size());
		m_ResetPlotValue.resize(m_Par_Dep.P_Names.size()+m_Par_Dep.Ps_Names.size());
		m_PreviousValue.resize(m_Par_Dep.P_Names.size()+m_Par_Dep.Ps_Names.size());
		double value;

		
		for(size_t i=0; i<m_Par_Dep.P_Names.size(); i++) {
			P* ptr=m_pSimDoc->GetPPointer(m_Par_Dep.P_Names[i]);
			if(ptr!=nullptr) {
				m_P.push_back(ptr);
				for(size_t layer=0; layer<ptr->GetSize(); layer++) {
						value=ptr->GetValue(layer);
						m_NewPlotValue[i].push_back(value);
						m_ResetPlotValue[i].push_back(value);
						m_PreviousValue[i].push_back(value);
				}
			}
		}
		for(size_t i=0; i<m_Par_Dep.Ps_Names.size(); i++) {	
			Ps* ptr=m_pSimDoc->GetPsPointer(m_Par_Dep.Ps_Names[i]);
			if(ptr!=nullptr) {
				m_Ps.push_back(ptr);
				value=ptr->GetValue();
				m_NewPlotValue[i].push_back(value);
				m_ResetPlotValue[i].push_back(value);
				m_PreviousValue[i].push_back(value);
			}
		}
		m_ChartType=0;
		m_minXValue = 0;
		m_maxXValue = 0;
		m_minValue = 0;
		m_maxValue = 0;
	}
	return true;
}


bool ParameterPlot::OnInit()
{
	 m_InitDialog=TRUE;
	 m_ThermalReCalc=false;
	 if(m_pView==nullptr) return false;
	 m_plotp=m_Func->GetPlotParam();
	 m_ThermalReCalc=IsThermalRelc();

	 ChartCug *pChartCug = MySingle::instance().GetChartCugPointer();
	 pChartCug->OnUpdate();
	 loadData();
	 
	if(m_PlotPrevious)
		AddData(true);
	if(m_PlotDefault)
		AddData(false);
	if(m_PlotNewValue)
		AddData(false, true);
	drawChart(&m_pView->m_ChartViewer);


    return true;
}
bool ParameterPlot::IsThermalRelc() {

	bool out=false;
	for(size_t i=0; i<m_P.size(); i++) {
		if(m_P[i]->GetViewFunction()==405) out=true;
	}

	return out;

}
void ParameterPlot::MakeSoilReCalc() {
	if(m_P.size()>0) {
		SimB *pSimB1=m_pSimDoc->GetPtr(FUNCTION,"ReCalc Soil Function");
		Func *pFunc=dynamic_cast<Func*>(pSimB1);
		if(pFunc!=nullptr)
			pFunc->MakeOperation();
		if(m_ThermalReCalc) {
			pSimB1=m_pSimDoc->GetPtr(FUNCTION,"ThermalScalingCoefEstimate");
			pFunc=dynamic_cast<Func*>(pSimB1);
			if(pFunc!=nullptr)
				pFunc->MakeOperation();
		}
	}

}




/////////////////////////////////////////////////////////////////////////////
// ParameterPlot methods

//
// Load the data
//
bool ParameterPlot::IsNewViewFunc(Ps* pCPAdd)
{
	int viewfunc;
	if(m_TableIndex>=0)
		viewfunc=((Tab*)pCPAdd)->GetViewFunction();
	else
		viewfunc=pCPAdd->GetViewFunction();
	if(viewfunc==m_PreviousViewFunction) 
		return FALSE;
	else
		return TRUE;

}

bool ParameterPlot::AddData(bool Previous, bool NewValue)
{
		//if(pCPAdd!=m_pCP)
		//	return FALSE;
		//m_ViewCounter++;
	string LegendCurve;
	string NewTypeText;
	m_noOfPoints=m_plotp.Num_Of_Points;
	size_t recalc;
	
	/*if(m_pCP->Is_PTable()) {
		pTab=(Tab*)m_pCP;
	}
	else {
		viewfunc=m_pCP->GetViewFunction();		
		m_Title=m_pCP->GetViewTitle().c_str();
	}*/

	double fvalue;
	if(Previous&&!NewValue) {
		NewTypeText="Previous";
			
		for(size_t i=0; i<m_P.size(); i++) {
			for(size_t layer=0; layer<m_plotp.Opt_Vec.size(); layer++) {
				m_TableIndex=m_plotp.Opt_Vec[layer];
				m_ResetPlotValue[i][m_TableIndex]=m_P[i]->GetValue(m_TableIndex);
				fvalue= m_PreviousValue[i][m_TableIndex];
				m_P[i]->SetValue(m_TableIndex,fvalue);
			}
		}
		for(size_t i=0; i<m_Ps.size(); i++) {
			m_ResetPlotValue[i+m_P.size()*m_plotp.Opt_Vec.size()][0]=m_Ps[i]->GetValue();
			fvalue=m_PreviousValue[i][0];
			m_Ps[i]->SetValue(fvalue);
		}
	}
	else if(!NewValue) {  // Default value
		NewTypeText="Default";		
		for(size_t i=0; i<m_P.size(); i++) {
			for(size_t layer=0; layer<m_plotp.Opt_Vec.size(); layer++) {
				m_TableIndex=m_plotp.Opt_Vec[layer];
				m_ResetPlotValue[i][m_TableIndex]=m_P[i]->GetValue(m_TableIndex);
				fvalue=m_P[i]->GetOriginalValue(m_TableIndex);
				m_P[i]->SetValue(m_TableIndex,fvalue);
			}
		}
		for(size_t i=0; i<m_Ps.size(); i++) {
			m_ResetPlotValue[i+m_P.size()*m_plotp.Opt_Vec.size()][0]=m_Ps[i]->GetValue();
			auto fvalue=m_Ps[i]->GetOriginalValue();
			m_Ps[i]->SetValue(fvalue);
		}




	}
	else {  // New value
		NewTypeText="New";
		size_t ind=0;
		for(size_t i=0; i<m_P.size(); i++) {
			for(size_t layer=0; layer<m_plotp.Opt_Vec.size(); layer++) {
				m_TableIndex=m_plotp.Opt_Vec[layer];
				m_ResetPlotValue[i][m_TableIndex]=m_P[i]->GetValue(m_TableIndex);
				fvalue=m_NewPlotValue[i][m_TableIndex];
				size_t res;
				res=m_P[i]->SetValue(m_TableIndex,fvalue);
				ind++;
			}
		}
		for(size_t i=0; i<m_Ps.size(); i++) {
			m_ResetPlotValue[i+m_P.size()][0]=m_Ps[i]->GetValue();
			fvalue=m_NewPlotValue[i][0];
			recalc=m_Ps[i]->SetValue(fvalue);
			if(recalc>0) m_pSimDoc->LinkedChangeToParameters(m_Ps[i],  recalc);
			ind++;
		}
	}

	m_ViewCounter++;
	m_CurveCounter=m_NumVarSelected-1;
	//PARAMETERS_mp_DEFINEPLOT=1; 
	//PARAMETERS_mp_SHIFTCURVE=0;

	/*		for(size_t i=0; i<m_P.size(); i++) {
				m_ResetPlotValue[i]=m_P[i]->GetValue(m_TableIndex);
				fvalue=m_NewPlotValue[i];
				m_P[i]->SetValue(m_TableIndex,fvalue);
			}
			for(size_t i=0; i<m_Ps.size(); i++) {
				m_ResetPlotValue[i+m_P.size()]=m_Ps[i]->GetValue();
				fvalue=m_NewPlotValue[i+m_P.size()];
				m_Ps[i]->SetValue(fvalue);
			}
	*/


	MakeSoilReCalc();
	size_t AddNew_Curve=1;
	AddNew_Curve=m_plotp.Opt_Vec.size();
	if(AddNew_Curve<1) AddNew_Curve=1;
	size_t curveindex=0;
	for(size_t tabindex=0; tabindex<m_plotp.Tot_Num_Of_Curves/m_plotp.Num_Of_Curves; tabindex++)
	for(size_t j=0; j<m_plotp.Num_Of_Curves; j++){
			int nCurve=j+m_CurveCounter;
			curveindex=j+tabindex*m_plotp.Num_Of_Curves+1+m_CurveCounter;
			AA[curveindex].clear();
			AA[curveindex].resize(m_plotp.Num_Of_Points);

			AA[curveindex]=GetYVector(j,tabindex, true);
			m_dataSeriesA[curveindex] = DoubleArray(&AA[curveindex][0], m_plotp.Num_Of_Points);

			if(m_P.size()>0)  {
				LegendCurve=NewTypeText+" (";
				m_TableIndex=m_plotp.Opt_Vec[tabindex];
				LegendCurve+=FUtil::STD_ItoAscii(m_TableIndex+1)+")";
			}
			else
				LegendCurve=NewTypeText;
			if (m_Legends.size() >= curveindex)
				m_Legends[curveindex - 1] = LegendCurve;
			else
				m_Legends.push_back(LegendCurve);
	}
	m_NumVarSelected=curveindex+1;
	for(size_t i=0; i<m_P.size(); i++) {
		for(size_t layer=0; layer<m_plotp.Opt_Vec.size(); layer++) {
			m_TableIndex=m_plotp.Opt_Vec[layer];
			fvalue=m_ResetPlotValue[i][m_TableIndex];
			m_P[i]->SetValue(m_TableIndex,fvalue);
		}
	}
	for(size_t i=0; i<m_Ps.size(); i++) {
		fvalue=m_ResetPlotValue[i+m_P.size()][0];
		recalc=m_Ps[i]->SetValue(fvalue);
		if(recalc>0) m_pSimDoc->LinkedChangeToParameters(m_Ps[i],  recalc);
	}

	MakeSoilReCalc();

	return TRUE;
}

bool ParameterPlot::loadData()
{

	double x,x2,x3, GXmin, GXmax;
	size_t opt=0;
	if(m_pParTable!=nullptr) {
		m_plotp.Tot_Num_Of_Curves=m_pParTable->GetNumbersToPlot()*m_plotp.Num_Of_Curves;
		m_plotp.Opt_Vec.clear();
		for(size_t i=0; i<m_pParTable->GetNumbersToPlot(); i++)
			m_plotp.Opt_Vec.push_back(m_pParTable->GetPlotIndex(i));
	}
	else
		m_plotp.Tot_Num_Of_Curves=m_plotp.Num_Of_Curves;
		// Standard parameter function


		for(size_t i=0; i<m_plotp.Tot_Num_Of_Curves+1;i++) {
			AA[i].clear();
			AA[i].resize(m_plotp.Num_Of_Points);
		}
		m_noOfPoints=m_plotp.Num_Of_Points;
		GXmin=m_plotp.X_Min;
		GXmax=m_plotp.X_Max;
		m_logscale=m_plotp.LogYScale;
		if(m_plotp.X_MaxPar.size()>0) {
			P* pP=(P*)m_pSimDoc->GetPtr(PAR_TABLE,m_plotp.X_MaxPar);
			if(pP->GetValue(m_TableIndex)>m_plotp.X_Max) GXmax=pP->GetValue(m_TableIndex);

		}


		m_LabelX=m_plotp.X_Label.c_str();
		m_LabelY=m_plotp.Y_Label.c_str();

		
		
		for(size_t i=0;i<m_noOfPoints;i++) {
				x=double(i)/double(m_noOfPoints)*(GXmax-GXmin)+GXmin;
				if(m_Func->GetLogXScale())
					AA[0][i]=pow(10,x);
				else
					AA[0][i]=x;

		}
		if(m_Func->GetLogXScale()){
			GXmin=pow(10.,GXmin);
			GXmax=pow(10.,GXmax);
		}
		
		m_dataSeriesA[0] = DoubleArray(&AA[0][0], m_noOfPoints);
		x2=1.; x3=2.;
		size_t curveindex;
		for(size_t tabindex=0; tabindex<m_plotp.Tot_Num_Of_Curves/m_plotp.Num_Of_Curves; tabindex++)
		for(size_t j=0; j<m_plotp.Num_Of_Curves; j++){	
			curveindex=j+tabindex*m_plotp.Num_Of_Curves+1;
			AA[curveindex].clear();
			AA[curveindex].resize(m_plotp.Num_Of_Points);

			AA[curveindex]=GetYVector(j,tabindex, true);
			
			string str;
			if(m_plotp.LegendsVec.size()>j) {
				str=m_plotp.LegendsVec[j];
				if(str.find("\n")==0) str=str.substr(1);
			}
			else {
				str="Current";
				m_plotp.LegendsVec.push_back(str);	
			}
				

			if(m_pParTable!=nullptr) {
				str+="("+FUtil::STD_ItoAscii(m_plotp.Opt_Vec[tabindex]+1)+")";
			}

			if (m_Legends.size() > curveindex)
				m_Legends[curveindex - 1]=str;
			else
				m_Legends.push_back(str);
			m_dataSeriesA[curveindex] = DoubleArray(&AA[curveindex][0], m_noOfPoints);

			if(m_PFMeasuredPoints) {
				m_ExtraCurveCounter=0;
				vector<float> pressure, theta;
				//NEWHEADER ActProfile;
				//ActProfile=m_pPFCurve->GetProfile(m_pPFCurve->m_ActProfile.RecLayers);
				pressure=m_pPFCurve->GetPF_Pressure(m_pPFCurve->m_ActProfile.RecLayers);
				theta=m_pPFCurve->GetPF_Theta(m_TableIndex);
				for(size_t i=0; i<m_pPFCurve->GetNumPF_Pressures();i++) {
					if(pressure[i]>0)
						BY[curveindex-1][i]=double(pressure[i]);
					else
						BY[curveindex-1][i]=Chart::NoValue;
					if(theta[i]>0)
						BX[curveindex-1][i]=double(theta[i]);
					else
						BX[curveindex-1][i]=Chart::NoValue;
				}
				m_noOfExtraPoint=m_pPFCurve->GetNumPF_Pressures();
				string str;	
				str="Measured (";
				str+=FUtil::STD_ItoAscii(m_TableIndex+1).c_str();
				str+=")";
				if (m_ExtraLegends.size() > curveindex)
					m_ExtraLegends[curveindex - 1] = str;
				else
					m_ExtraLegends.push_back(str);
				m_dataSeriesB[curveindex-1] = DoubleArray(&BX[curveindex-1][0],m_noOfExtraPoint);
				m_dataSeriesC[curveindex-1] = DoubleArray(&BY[curveindex-1][0],m_noOfExtraPoint);
			}
		}
	m_NumVarSelected=m_plotp.Tot_Num_Of_Curves+1;
	m_minValue=m_maxValue=0.;
	m_minXValue=GXmin;
	m_maxXValue=GXmax;
	m_Func->SetPlotParam(m_plotp);
	return TRUE;

}


// Draw the chart and display it in the given viewer
//
void ParameterPlot::drawChart(CChartViewer *viewer)
{
	Clean_ParameterPlot();
    //
    // In this demo, we copy the visible part of the data to a separate buffer for chart
    // plotting. 
    //
    // Note that if you only have a small amount of data (a few hundred data points), it
    // may be easier to just plot all data in any case (so the following copying code is 
    // not needed), and let ChartDirector "clip" the chart to the plot area. 
    //
	int noOfPoints, startIndex, endIndex;
	double m_interv=0.;
	CWinApp* pApp = AfxGetApp();
 
	char* labels[121];

	// Scatter plot
		noOfPoints=m_plotp.Num_Of_Points;
		startIndex=0;
		endIndex=m_noOfPoints-1;
	

    // We copy the visible data from the main arrays to separate data arrays
    double* viewPortTimeStamps = new double[noOfPoints];
	double* viewPortDataSeries[32];
	int arraySizeInBytes = noOfPoints * sizeof(double);
	if(m_ChartType!=2)
		memcpy(viewPortTimeStamps, &TT[startIndex], arraySizeInBytes);

	string cstr;	

	if(m_pProp->m_MultiValid.AddOnlyValidRuns&&m_pUtil->m_MultiRunPlot) 
		m_NumVarSelected=m_NumVarSelected*2;
	
	for(int i=0;i<m_NumVarSelected;i++) {
		cstr="HColorLine"+FUtil::STD_ItoAscii(i+1);
		viewPortDataSeries[i] = new double[noOfPoints];
		memcpy(viewPortDataSeries[i], m_dataSeriesA[i].data + startIndex, arraySizeInBytes);
	}
	
	double* viewPortDataSeriesB[6];
	double* viewPortDataSeriesMin[6];
	double* viewPortDataSeriesMax[6];
	for(int i=0; i<m_NumVarBandSelected;i++) {
		viewPortDataSeriesB[i] = new double[noOfPoints];
		memcpy(viewPortDataSeriesB[i], m_dataSeriesB[i].data + startIndex, arraySizeInBytes);
		viewPortDataSeriesMin[i] = new double[noOfPoints];
		memcpy(viewPortDataSeriesMin[i], m_dataSeriesMIN[i].data + startIndex, arraySizeInBytes);
		viewPortDataSeriesMax[i] = new double[noOfPoints];
		memcpy(viewPortDataSeriesMax[i], m_dataSeriesMAX[i].data + startIndex, arraySizeInBytes);
	}





    if (noOfPoints >= 300&&m_ChartType==1)
    {


        // Set up an aggregator to aggregate the data based on regular sized slots
        ArrayMath m(DoubleArray(viewPortTimeStamps, noOfPoints));
		
        m.selectRegularSpacing(noOfPoints / 260);
        
        // For the timestamps, take the first timestamp on each slot
        int aggregatedNoOfPoints = m.aggregate(DoubleArray(viewPortTimeStamps, noOfPoints), 
            Chart::AggregateFirst).len;

		for(int i=0;i<m_NumVarSelected;i++) {
        // For the data values, aggregate by taking the averages
			
			ArrayMath buffer(DoubleArray(viewPortDataSeries[i],  noOfPoints));
			DoubleArray maxValues = m.aggregate(buffer.result(), Chart::AggregateMax);
			m.aggregate(DoubleArray(viewPortDataSeries[i], noOfPoints), Chart::AggregateAvg);

			 for (int j = 0; j < maxValues.len; ++j)
				if (maxValues[j] < viewPortDataSeries[i][j])  viewPortDataSeries[i][j]= Chart::NoValue;


		}

			for(int i=0; i<m_NumVarBandSelected;i++) {
				ArrayMath buffer(DoubleArray(viewPortDataSeriesB[i],  noOfPoints));
				DoubleArray maxValues = m.aggregate(buffer.result(), Chart::AggregateMax);
				m.aggregate(DoubleArray(viewPortDataSeriesB[i], noOfPoints), Chart::AggregateAvg);
				for (int j = 0; j < maxValues.len; ++j)
					if (maxValues[j] < viewPortDataSeriesB[i][j])  viewPortDataSeriesB[i][j]= Chart::NoValue;

				ArrayMath buffer2(DoubleArray(viewPortDataSeriesMin[i],  noOfPoints));
				maxValues = m.aggregate(buffer2.result(), Chart::AggregateMax);
				m.aggregate(DoubleArray(viewPortDataSeriesMin[i], noOfPoints), Chart::AggregateAvg);
				for (int j = 0; j < maxValues.len; ++j)
					if (maxValues[j] < viewPortDataSeriesMin[i][j])  viewPortDataSeriesMin[i][j]= Chart::NoValue;

				ArrayMath buffer3(DoubleArray(viewPortDataSeriesMax[i],  noOfPoints));
				maxValues = m.aggregate(buffer3.result(), Chart::AggregateMax);
				m.aggregate(DoubleArray(viewPortDataSeriesMax[i], noOfPoints), Chart::AggregateAvg);
				for (int j = 0; j < maxValues.len; ++j)
					if (maxValues[j] < viewPortDataSeriesMax[i][j])  viewPortDataSeriesMax[i][j]= Chart::NoValue;
		}
 
        noOfPoints = aggregatedNoOfPoints-1;

    }

    //
    // Now we have obtained the data, we can plot the chart. 
    //

    ///////////////////////////////////////////////////////////////////////////////////////
    // Step 1 - Configure overall chart appearance. 
    ///////////////////////////////////////////////////////////////////////////////////////

    // Create an XYChart object 600 x 300 pixels in size, with pale blue (0xf0f0ff) 
    // background, black (000000) border, 1 pixel raised effect, and with a rounded frame.

	XYChart *c;

	if(m_pView!=nullptr) {
		m_cx=m_pView->GetXViewSize();
		m_cy=m_pView->GetYViewSize();
	}
	if(m_cx>m_cy+130)
		m_cx=m_cy+130;

	int xleft = m_pProp->GetLeftMargin();

	int ytop = m_pProp->GetTopMargin();
	int ybottom = m_pProp->GetBottomMargin();
	int xright = m_pProp->GetRightMargin();

	m_DisplayXSize=m_cx;
	m_DisplayYSize=m_cy;


		c = new XYChart(m_DisplayXSize, m_DisplayYSize, m_pUtil->m_ColorBackground, 0, 1);
		int blue=0xffffff;
		c->setPlotArea(xleft, ytop, m_DisplayYSize-xleft-xright, m_DisplayYSize-ytop-ybottom, blue, -1, -1, 0xcccccc, 0xcccccc);
		c->addLegend(m_DisplayYSize, 60, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent, 
        Chart::Transparent);


    c->setRoundedFrame(m_extBgColor);
    
    // Set the plotarea at (52, 60) and of size 520 x 192 pixels. Use white (ffffff) 
    // background. Enable both horizontal and vertical grids by setting their colors to 
    // grey (cccccc). Set clipping mode to clip the data lines to the plot area.
 
    c->setClipping();

    // Add a top title to the chart using 15 pts Times New Roman Bold Italic font, with a 
    // light blue (ccccff) background, black (000000) border, and a glass like raised effect.
  //  c->addTitle("Zooming and Scrolling Demonstration", "timesbi.ttf", 15
   //     )->setBackground(0xccccff, 0x0, Chart::glassEffect());


    // Set axes width to 2 pixels
    c->yAxis()->setWidth(2);
    c->xAxis()->setWidth(2);

    // Add a title to the y-axis


    ///////////////////////////////////////////////////////////////////////////////////////
    // Step 2 - Add data to chart
    ///////////////////////////////////////////////////////////////////////////////////////

    // 
    // In this example, we represent the data by lines. You may modify the code below if 
    // you want to use other representations (areas, scatter plot, etc).
    //
	Layer *layer;
	   // Scatter
	layer=c->addLineLayer();
	layer->setLineWidth(2);

	CString ytitle1, ytitle2, test, unit1, unit2, IndepId, unit;


	int TotNumPrior;

	TotNumPrior=m_NumVarSelected/2;
	int NumPrior=m_NumVarSelected;

	

//CString newtitle="Independent Variable :"+IndepId;
					c->addTitle(Chart::TopCenter, m_Title.c_str());
					if(m_NumVarSelected>0)
						layer->setXData(DoubleArray(viewPortDataSeries[0], noOfPoints));
					for (int i=1;i<m_NumVarSelected;i++) {
						layer->addDataSet(DoubleArray(viewPortDataSeries[i],noOfPoints),
						m_pUtil->GetHColorLine(i-1),m_Legends[i-1].c_str());
					}
					for (int i=0;i<m_ExtraCurveCounter+1;i++) {
						if(m_PFMeasuredPoints) {
							c->addScatterLayer(m_dataSeriesB[i],m_dataSeriesC[i],
								m_ExtraLegends[i].c_str(),Chart::DiamondSymbol,9, m_pUtil->GetHColorLine(i-1)  );
						}
					}
		
			
			c->xAxis()->setTitle(m_LabelX.c_str(), "arialbd.ttf", 9);
			c->yAxis()->setTitle(m_LabelY.c_str(), "arialbd.ttf", 9);
	

	

	
    ///////////////////////////////////////////////////////////////////////////////////////
    // Step 3 - Set up x-axis scale
    ///////////////////////////////////////////////////////////////////////////////////////
	if(m_ChartType==2||m_ChartType==3) {
		//c->xAxis()->setLinearScale(m_minXValue, m_maxXValue);

		if(m_ChartType==3)
			c->yAxis()->setLinearScale(0,1,.2);

		c->layout();
		//m_minXValue = c->xAxis()->getMinValue();
	//	m_maxXValue = c->xAxis()->getMaxValue();
		m_minValue = c->yAxis()->getMinValue();
		m_maxValue = c->yAxis()->getMaxValue();
	}
	else {
		//c->xAxis()->setLinearScale()
		if((m_minXValue == 0) && (m_maxXValue == 0)) {
			c->layout();
			m_minXValue = c->xAxis()->getMinValue();
			m_maxXValue = c->xAxis()->getMaxValue();
			m_minValue = c->yAxis()->getMinValue();
			m_maxValue = c->yAxis()->getMaxValue();
			m_interv=0;

		}
		if(m_CommonScale) {
				double min1,max1;
				min1=min(m_minXValue,m_minValue);
				max1=max(m_maxXValue,m_maxValue);
				m_maxValue=m_maxXValue=max1;
				m_minValue=m_minXValue=min1;
				m_interv=(max1-min1)/5.;
		}
		else
			m_interv=0;
		
		//c->xAxis()->setLogScale(, axisUpperLimit);
		if(!m_Func->GetLogXScale()) {
			if (m_interv>0) 
				c->xAxis()->setLinearScale(m_minXValue, m_maxXValue,m_interv);
			else
				c->xAxis()->setLinearScale(m_minXValue, m_maxXValue);
		}
		else 
			c->xAxis()->setLogScale();


	}

    
    ///////////////////////////////////////////////////////////////////////////////////////
    // Step 4 - Set up y-axis scale
    ///////////////////////////////////////////////////////////////////////////////////////

	double axisLowerLimit, axisUpperLimit;



	if(m_ChartType==0) {
		if(((m_minValue == 0) && (m_maxValue == 0))||m_pCP!=nullptr) {
			if(m_logscale)
					c->yAxis()->setLogScale();
			c->layout();
			m_minValue = c->yAxis()->getMinValue();
			m_maxValue = c->yAxis()->getMaxValue();
		}
		if(m_pCP==nullptr) m_interv=0;
	}
	else if(m_ChartType==2||m_ChartType==3) {
		// The labels for the area chart
		char str[200];
		for( int i=0;i<noOfPoints;i++) {
			
			auto f=double(i)/double((noOfPoints-1))*(m_maxXValue-m_minXValue)+m_minXValue;
			if(f>=0)
				sprintf_s(str,"%2G",f);
			else
				sprintf_s(str,"\\%2G",f);

			labels[i]=str;
		}
		c->xAxis()->setLabels(StringArray(labels, noOfPoints));
		int minstep=min(noOfPoints/10,noOfPoints/4);
		c->xAxis()->setLabelStep(minstep);
	}
	else if((m_minValue == 0) && (m_maxValue == 0)) {
			c->layout();
			m_minValue = c->yAxis()->getMinValue();
			m_maxValue = c->yAxis()->getMaxValue();
			m_interv=0;
	}

    else if (viewer->getZoomDirection() == Chart::DirectionHorizontal) 
    {
        // y-axis is auto-scaled - save the chosen y-axis scaled to support xy-zoom mode


		axisLowerLimit=m_minValue;
		axisUpperLimit=m_maxValue;
    }
    else
    {
        // xy-zoom mode - compute the actual axis scale in the view port 
        axisLowerLimit =  m_maxValue - (m_maxValue - m_minValue) * 
            (viewer->getViewPortTop() + viewer->getViewPortHeight());
        axisUpperLimit =  m_maxValue - (m_maxValue - m_minValue) * 
            viewer->getViewPortTop();
	}


	if(m_ChartType==0||m_ChartType==2||m_ChartType==3) {
			if(m_ChartType==3) {

			}
			else if(m_logscale) {
				c->yAxis()->setLogScale(m_minValue, m_maxValue);
				//c->XAxis()->setLogScale(, axisUpperLimit);
			}
			else if(m_interv>0) 
				c->yAxis()->setLinearScale(m_minValue, m_maxValue,m_interv);
			else
				c->yAxis()->setLinearScale(m_minValue, m_maxValue);
			c->layout();

	}
	else if(m_logscale&&m_ZoomX==FALSE) {
        // *** use the following formula if you are using a log scale axis ***
			axisLowerLimit = m_maxValue * pow(m_minValue / m_maxValue, 
				viewer->getViewPortTop() + viewer->getViewPortHeight());
			axisUpperLimit = m_maxValue * pow(m_minValue / m_maxValue, 
				viewer->getViewPortTop());
			if(axisLowerLimit<=0) axisLowerLimit=1;
			c->yAxis()->setLogScale(axisLowerLimit, axisUpperLimit);
	}
	else if(m_ZoomX==FALSE)	{
        // xy-zoom mode - compute the actual axis scale in the view port 
			axisLowerLimit =  m_maxValue - (m_maxValue - m_minValue) * 
            (viewer->getViewPortTop() + viewer->getViewPortHeight());
			 axisUpperLimit =  m_maxValue - (m_maxValue - m_minValue) * 
            viewer->getViewPortTop();
        // use the zoomed-in scale
			 c->yAxis()->setLinearScale(axisLowerLimit, axisUpperLimit);
	}
    c->yAxis()->setRounding(false, false);
    

    ///////////////////////////////////////////////////////////////////////////////////////
    // Step 5 - Display the chart
    ///////////////////////////////////////////////////////////////////////////////////////

    // Set the chart image to the WinChartViewer
   // delete m_ChartViewer.getChart();
    
	viewer->setChart(c);
	if(m_pUtil->m_FileSave.m_SaveFile){
		m_pUtil->m_FileSave.MakeFile(m_pSimDoc);
	}
	m_pUtil->m_Displayed=true;
	m_XY_Chart.push_back(c);
    // Free resources

    delete[] viewPortTimeStamps;
	for(int i=0; i<m_NumVarSelected;i++) {
		delete[] viewPortDataSeries[i];
	
	}
	for(int i=0;i<m_NumVarBandSelected;i++) {
		delete [] viewPortDataSeriesB[i];
		delete [] viewPortDataSeriesMin[i];
		delete [] viewPortDataSeriesMax[i];
	}
	if(m_ChartType==2||m_ChartType==3) {
		for (int i=0;i<noOfPoints;i++) {
			char *str=labels[i];
			delete [] str;
		}
	}
}

//
// Update the image map
//
void ParameterPlot::updateImageMap(CChartViewer *viewer)
{
    if (0 == viewer->getImageMapHandler())
    {
		if(m_ChartType==1) {
			if(m_pUtil->m_MultiRunPlot) 
				viewer->setImageMap(viewer->getChart()->getHTMLImageMap("clickable", "",
				"title='[{dataSetName}] {x}: {value|2}'"));
			else 
			// no existing image map - creates a new one
			viewer->setImageMap(viewer->getChart()->getHTMLImageMap("clickable", "",
				"title='[{dataSetName}] {x|mmm dd, yyyy,hh:mm}: {value|2}'"));
		}
    }
}

/////////////////////////////////////////////////////////////////////////////
// General utilities

//
// Get the default background color
//
int ParameterPlot::getDefaultBgColor()
{
    LOGBRUSH LogBrush; 
    HBRUSH hBrush = (HBRUSH)SendMessage(WM_CTLCOLORDLG, (WPARAM)CClientDC(this).m_hDC, 
        (LPARAM)m_hWnd); 
    ::GetObject(hBrush, sizeof(LOGBRUSH), &LogBrush); 
    int ret = LogBrush.lbColor;
    return ((ret & 0xff) << 16) | (ret & 0xff00) | ((ret & 0xff0000) >> 16);
}

//
// Load an icon resource into a button
//
void ParameterPlot::loadButtonIcon(int buttonId, int iconId, int width, int height)
{
    GetDlgItem(buttonId)->SendMessage(BM_SETIMAGE, IMAGE_ICON, (LPARAM)::LoadImage(
        AfxGetResourceHandle(), MAKEINTRESOURCE(iconId), IMAGE_ICON, width, height, 
        LR_DEFAULTCOLOR));  
}



/*
void ParameterPlot::OnClose()
{
	if(m_TableIndex>=0&&m_ResetViewFunction>100) 
		((Tab*)m_pCP)->SetViewFunction(m_ResetViewFunction);

	CDialog::OnClose();

}*/




/*
void ParameterPlot::OnBnClickedAddDefault()
{
	// TODO: Add your control notification handler code here
	//Add values with default values
	m_PlotDefault=TRUE;
	OnInitDialog();
}

void ParameterPlot::OnBnClickedAddPrevious()
{
	// TODO: Add your control notification handler code here
	m_PlotPrevious=TRUE;
	OnInitDialog();
}

void ParameterPlot::OnBnClickedNewvalue()
{
	// TODO: Add your control notification handler code here
	m_PlotNewValue=TRUE;
	OnInitDialog();
}

void ParameterPlot::OnBnClickedApplyDefault()
{
		float fvalue;

		CMainFrame *p_mainframe;
		p_mainframe = (CMainFrame*) AfxGetMainWnd();

		if(m_TableIndex>=0) {
			fvalue=m_pCPt->GetOriginalValue(m_TableIndex);
			m_pCPt->SetValue(m_TableIndex,fvalue);
			m_pCPt->EditApply(m_TableIndex);
			if(m_ParNum>1) {
				fvalue=m_pCPt2->GetOriginalValue(m_TableIndex);
				m_pCPt2->SetValue(m_TableIndex,fvalue);
				m_pCPt2->EditApply(m_TableIndex);
			}
			if(m_ParNum>2) {
				fvalue=m_pCPt3->GetOriginalValue(m_TableIndex);
				m_pCPt3->SetValue(m_TableIndex,fvalue);
				m_pCPt3->EditApply(m_TableIndex);
			}
			if(m_ParNum>3) {
				fvalue=m_pCPt4->GetOriginalValue(m_TableIndex);
				m_pCPt4->SetValue(m_TableIndex,fvalue);
				m_pCPt4->EditApply(m_TableIndex);
			}
			MySingle::instance().GetCugPointer()->UpdateSheet(3);
			if(((Tab*)m_pCP)->GetViewFunction()==401)
				HYDRAULIC_mp_HYDRAULIC_RECALC();
		}
		else {
			fvalue=*(float*)m_pCP->GetOriginalValue();
			m_pCP->SetValue(fvalue);
			m_pCP->EditApply();
			if(m_ParNum>1) {
				fvalue=*(float*)m_pCP2->GetOriginalValue();
				m_pCP2->SetValue(fvalue);
				m_pCP2->EditApply();
			}
			if(m_ParNum>2) {
				fvalue=*(float*)m_pCP3->GetOriginalValue();
				m_pCP3->SetValue(fvalue);
				m_pCP3->EditApply();
			}
			if(m_ParNum>3) {
				fvalue=*(float*)m_pCP4->GetOriginalValue();
				m_pCP4->SetValue(fvalue);
				m_pCP4->EditApply();
			}
		}
		OnInitDialog();
	// TODO: Add your control notification handler code here
}

void ParameterPlot::OnBnClickedApplyPrevious()
{
	float fvalue;
	CMainFrame *p_mainframe;
    p_mainframe = (CMainFrame*) AfxGetMainWnd();

		if(m_TableIndex>=0) {
			if(m_pCPt->IsOldValue(m_TableIndex)) {
				fvalue=m_pCPt->GetOldValue(m_TableIndex);
				m_pCPt->SetValue(m_TableIndex,fvalue);
				m_pCPt->EditApply(m_TableIndex);
			}
			if(m_ParNum>1) {
				if(m_pCPt2->IsOldValue(m_TableIndex)) {
					fvalue=m_pCPt2->GetOldValue(m_TableIndex);
					m_pCPt2->SetValue(m_TableIndex,fvalue);
					m_pCPt2->EditApply(m_TableIndex);
				}
			}
			if(m_ParNum>2) {
				if(m_pCPt3->IsOldValue(m_TableIndex)) {
					fvalue=m_pCPt3->GetOldValue(m_TableIndex);
					m_pCPt3->SetValue(m_TableIndex,fvalue);
					m_pCPt3->EditApply(m_TableIndex);
				}
			}
			if(m_ParNum>3) {
				if(m_pCPt4->IsOldValue(m_TableIndex)) {
					fvalue=m_pCPt4->GetOldValue(m_TableIndex);
					m_pCPt4->SetValue(m_TableIndex,fvalue);
					m_pCPt4->EditApply(m_TableIndex);
				}
			}
			MySingle::instance().GetCugPointer()->UpdateSheet(3);
			if(((Tab*)m_pCP)->GetViewFunction()==401)
				HYDRAULIC_mp_HYDRAULIC_RECALC();
		}
		else {
			fvalue=m_pCP->GetOldValue();
			m_pCP->SetValue(fvalue);
			m_pCP->EditApply();
			if(m_ParNum>1) {
				fvalue=m_pCP2->GetOldValue();
				m_pCP2->SetValue(fvalue);
				m_pCP2->EditApply();
			}
			if(m_ParNum>2) {
				fvalue=m_pCP3->GetOldValue();
				m_pCP3->SetValue(fvalue);
				m_pCP3->EditApply();
			}
			if(m_ParNum>3) {
				fvalue=m_pCP4->GetOldValue();
				m_pCP4->SetValue(fvalue);
				m_pCP4->EditApply();
			}
		}
		OnInitDialog();
	// TODO: Add your control notification handler code here
}
*/
void ParameterPlot::ApplyNewvalue()
{
	for(size_t i=0; i<m_P.size(); i++) {
		for(size_t layer=0; layer<m_plotp.Opt_Vec.size(); layer++) {
				m_TableIndex=m_plotp.Opt_Vec[layer];
				if(m_TableIndex>=0) {
					m_P[i]->SetValue(m_TableIndex, m_NewPlotValue[i][m_TableIndex]);
				}
		}
	}
	for(size_t i=0; i<m_Ps.size(); i++) {
			auto fvalue=m_NewPlotValue[i+m_P.size()*m_plotp.Opt_Vec.size()][0];
			m_Ps[i]->SetValue(fvalue);
	}

	if(m_pCP!=nullptr) {
		if(((Tab*)m_pCP)->GetViewFunction()==401) {
			SimB *pSimB1=m_pSimDoc->GetPtr(FUNCTION,"ReCalc Soil Function");
			Func *pFunc=dynamic_cast<Func*>(pSimB1);
			if(pFunc!=nullptr)
				pFunc->MakeOperation();
		}
	}
	OnInit();
}

vector<double> ParameterPlot::GetYVector(size_t j,size_t tabindex, bool test) {
	double x2, x3, x4, y;
	vector<double> out;
	size_t opt;

	if(m_plotp.X2_Vec.size()>j) x2=m_plotp.X2_Vec[j]; else x2=0;
	if(m_plotp.X3_Vec.size()>j) x3=m_plotp.X3_Vec[j]; else x3=0;
	if(m_plotp.X4_Vec.size()>j) x4=m_plotp.X4_Vec[j]; else x4=0;
	if(m_plotp.Opt_Vec.size()>tabindex) opt=m_plotp.Opt_Vec[tabindex]; else opt=0;
	if(test) {
		//Singel initial call
		switch(m_Func->GetFuncType()%100) {
			case 0:
				y=m_Func->GetValue(AA[0][0], test);		
				break;
			case 1:
				y=m_Func->GetValue(AA[0][0],x2);		
				break;
			case 2:
				y=m_Func->GetValue(AA[0][0], x2, x3);		
				break;
			case 3:
				y=m_Func->GetValue(AA[0][0], x2, x3, x4);		
				break;
			case 10:
				y=m_Func->GetValue(opt,AA[0][0]);		
				break;
			case 11:
				y=m_Func->GetValue(opt,AA[0][0], x2);		
				break;
			case 12:
				y=m_Func->GetValue(opt, AA[0][0], x2, x3, test);		
				break;
			case 13:
				y=m_Func->GetValue(opt, AA[0][0], x2, x3, x4);		
				break;
			default:
				y=0;

		}
	}
	if (m_Func->m_FunctorFunctions) {
		enum class MyArgType Arg;
		Arg = m_Func->GetArgumentTypes();
		bool TableType = m_Func->IsTableType();
		if (!TableType) {
			switch (Arg) {
			case MyArgType::D1: {	
				auto f_D1 = m_Func->Get_D1_Functor();
				for (size_t i = 0; i < m_plotp.Num_Of_Points; i++) { y = f_D1(AA[0][i]);	if (y < -1.e38)	out.push_back(Chart::NoValue);	else out.push_back(y); }};	break;
			case MyArgType::D2: {	
				auto f_D2 = m_Func->Get_D2_Functor();
				for (size_t i = 0; i < m_plotp.Num_Of_Points; i++) { y = f_D2(AA[0][i], AA[1][i]);	if (y < -1.e38)	out.push_back(Chart::NoValue);	else out.push_back(y); }};	break;
			case MyArgType::D3: {	
				auto f_D3 = m_Func->Get_D3_Functor();
				for (size_t i = 0; i < m_plotp.Num_Of_Points; i++) { y = f_D3(AA[0][i], AA[1][i], AA[2][i]);	if (y < -1.e38)	out.push_back(Chart::NoValue);	else out.push_back(y); }};	break;
			case MyArgType::D4: {
				auto f_D4 = m_Func->Get_D4_Functor();
				for (size_t i = 0; i < m_plotp.Num_Of_Points; i++) { y = f_D4(AA[0][i], AA[1][i], AA[2][i], AA[3][i]);	if (y < -1.e38)	out.push_back(Chart::NoValue);	else out.push_back(y); }};	break;
			}
		}
		else {

		}

	}
	else {
		for (size_t i = 0; i < m_plotp.Num_Of_Points; i++) {

			switch (m_Func->GetFuncType() % 100) {
			case 0:
				y = m_Func->GetValue(AA[0][i]);
				break;
			case 1:
				y = m_Func->GetValue(AA[0][i], x2);
				break;
			case 2:
				y = m_Func->GetValue(AA[0][i], x2, x3);
				break;
			case 3:
				y = m_Func->GetValue(AA[0][i], x2, x3, x4);
				break;
			case 10:
				y = m_Func->GetValue(opt, AA[0][i]);
				break;
			case 11:
				y = m_Func->GetValue(opt, AA[0][i], x2);
				break;
			case 12:
				y = m_Func->GetValue(opt, AA[0][i], x2, x3);
				break;
			case 13:
				y = m_Func->GetValue(opt, AA[0][i], x2, x3, x4);
				break;
			default:
				y = 0;

			}
			if (y < -1.e38)
				out.push_back(Chart::NoValue);
			else
				out.push_back(y);
		}
	}
	return out;
}
void ParameterPlot::Clean_ParameterPlot()
{
	while(m_XY_Chart.size()>0) {
		delete m_XY_Chart[0];
		m_XY_Chart.erase(m_XY_Chart.begin());
	}



}