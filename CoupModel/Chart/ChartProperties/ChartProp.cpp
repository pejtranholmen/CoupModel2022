
#include "./ChartProp.h"
#include "../../CUG\ugctrl.h"
#include "../../PG\PG.H"
#include "../../CoupModel.h"
#include "../../CoupModelDoc.h"
#include "../../CoupModelView.h"
#include "../../MainFrm.h"
#include "../../PG\PGCalc.h"
#include "../../MultiRun\ConStrainByInput.h"


ChartProp::ChartProp(void)
{
	MySingle::instance().SetChartPropPointer(this);
	m_ActualMultiSheet=-1;
	m_AggregateTime = true;
	m_ChartType= CHART_TYPES::TIME_SERIE;
	CHART_TYPES_OPTIONS o;
	o.typ = CHART_TYPES::SCATTER; o.typtext = "Scatter chart";
	m_ChartTypesOptions.push_back(o);
	o.typ = CHART_TYPES::TIME_SERIE; o.typtext = "Time Serie Chart";
	m_ChartTypesOptions.push_back(o);
	o.typ = CHART_TYPES::FREQUENCE; o.typtext = "Frequence distribution";
	m_ChartTypesOptions.push_back(o);
	o.typ = CHART_TYPES::CUM_FREQUENCE; o.typtext = "Cumulated distribution";
	m_ChartTypesOptions.push_back(o);
	o.typ = CHART_TYPES::CONTOUR_CHART; o.typtext = "Contour chart";
	m_ChartTypesOptions.push_back(o);
	m_multi_ResidualSequence = 0;

}


ChartProp::~ChartProp(void)
{
	
}


void ChartProp::SetMultiMode(bool value)
{
	m_MultiTable=value;

}
void ChartProp::SetMultiSequence(int value)
{
	m_MultiSequence=value;
}
void ChartProp::SetMultiRepititions(int value)
{
	m_MultiRepititions=value;
}


CHART_POS ChartProp::GetChartPos(int ich) {
	ChartUtil *m_pUtil;
	m_pUtil = MySingle::instance().GetChartUtilPointer();
	CHART_POS out;
	out.xpos = 0;
	out.ypos = 0;
	return out;
	int ytop = GetTopMargin();
	int ybottom = GetBottomMargin();;
	int ydiff = int(float(GetGapMargin())*(1. - m_pUtil->GetGapHorFraction()));
	int xdiff = int(float(GetGapMargin())*m_pUtil->GetGapHorFraction());
	int xdiff1, xdiff2, ydiff1, ydiff2;
	xdiff1 = int(float(xdiff)*m_pUtil->GetGapHorLeftFraction());
	xdiff2 = xdiff - xdiff1;
	ydiff1 = int(float(ydiff)*m_pUtil->GetGapVertTopFraction());
	ydiff2 = ydiff - ydiff1;

	int xleft = GetLeftMargin();
	int xright = GetRightMargin();


	int yrow, xcol;
	xcol = (ich%m_NumMultiChartX);
	yrow = (ich / m_NumMultiChartX%m_NumMultiChartY);



	int xpos, ypos;
	if (yrow == 0 && xcol == 0) {
		xpos = xleft;
		ypos = ytop;

	}
	else if (yrow == 0 && xcol == m_NumMultiChartX - 1) {
		xpos = xdiff1;
		ypos = ytop;
	}
	else if (yrow == 0) {
		xpos = xdiff1;
		ypos = ytop;
	}
	else if (yrow == m_NumMultiChartY - 1 && xcol == 0) {
		xpos = xleft;
		ypos = ydiff1;
	}
	else if (yrow == m_NumMultiChartY - 1 && xcol == m_NumMultiChartX - 1) {
xpos = xdiff1;
ypos = ydiff1;
	}
	else if (yrow == m_NumMultiChartY - 1) {
		xpos = xdiff1;
		ypos = ydiff1;
	}
	else if (xcol == 0) {
		xpos = xleft;
		ypos = ydiff1;
	}
	else if (xcol == m_NumMultiChartX - 1) {
		xpos = xdiff1;
		ypos = ydiff1;
	}
	else {
		xpos = xdiff1;
		ypos = ydiff1;

	}

	out.xpos = xpos;
	out.ypos = ypos;

	return out;

}

string ChartProp::DefineYearWithinDayCell(CUGCell* pCell) {

	pCell->SetCellType(UGCT_DROPLIST);
	string optionlist;
	string label;

	size_t numvalues = 1440 / m_pPGSource->GetNormalTimeInterval();
	if (m_MultiPG_dep.FirstType == PGDAY&&m_MultiPG_dep.SecondType == PGYEAR) {
		label = "Within Day Value";
		optionlist = "All\n";
		for (size_t i = 0; i < numvalues; i++) {
			optionlist += FUtil::STD_ItoAscii(i + 1) + '\n';
		}
		optionlist += "Mean\n";
		string hourrepstring;
		if (m_HourValueRepresentation == 0) hourrepstring = "All";
		else if (m_HourValueRepresentation <= numvalues) hourrepstring = FUtil::STD_ItoAscii(m_HourValueRepresentation);
		else if (m_HourValueRepresentation == numvalues + 1) hourrepstring = "Mean";
		pCell->SetText(hourrepstring);
	}
	else if (m_MultiPG_dep.FirstType == PGDAY&&m_MultiPG_dep.SecondType == PGHOUR) {
		label = "Year Value";
		size_t numvalues = m_pPGSource->GetNumberofYears();
		optionlist = "Mean\n";
		for (size_t i = 0; i < numvalues; i++) {
			auto index = m_pPGSource->GetYearIndex(i + 1);
			auto year = m_pPGSource->GetYear(index);
			optionlist += FUtil::STD_ItoAscii(year) + '\n';
		}
		string yearrepstring;
		if (m_YearValueRepresentation == 0) yearrepstring = "Mean";
		else if (m_YearValueRepresentation <= numvalues) yearrepstring = FUtil::STD_ItoAscii(m_pPGSource->GetYear(m_pPGSource->GetYearIndex(m_YearValueRepresentation)));
		pCell->SetText(yearrepstring);
	}
	pCell->SetLabelText(optionlist);

	return label;

}

void ChartProp::AddConstrainByMultiPar(SIMB simv, size_t Index)
{
	CONSTRAIN_BY add;
	add.IndepMultiPar = Index;
	float mi,ma;
	string str;
	str = "Constrain outputs based on : ";
	str += simv.pBase->GetName();
	if (simv.pBase->Is_Vector()) {
		mi = ((P*)simv.pBase)->MR_Get_Min(simv.TabIndex);
		ma= ((P*)simv.pBase)->MR_Get_Max(simv.TabIndex);
	}
	else {
		mi = ((Ps*)simv.pBase)->MR_Get_Min();
		ma = ((Ps*)simv.pBase)->MR_Get_Max();
	}
	ConstrainByInput dlg(mi, ma, str);
	if (dlg.DoModal() == IDOK) {
		add.MinRangeValue = dlg.m_NewMin;
		add.MaxRangeValue = dlg.m_NewMax;
		m_ConstrainProp.push_back(add);
	}

}

bool ChartProp::CheckChartType_Based_On_IndependentVar()
{
	if (GetIndepFirst() != DATEAXIS&&GetIndepFirst() != NO_USE&&GetIndepFirst() != MULTIRUN_NUM&&
		GetIndepSecond() != DATEAXIS&&GetIndepSecond() != NO_USE)
	{
		return SetChartType(4);
	}
	else 	if (IsFirstIndepSet() && !IsSecondIndepSet() && m_ChartType ==CHART_TYPES::CONTOUR_CHART)
		return SetChartType(0);
	else
		return false;
}
void ChartProp::MakeCumTranformation(vector<double>* pVector)
{
	auto Acc = 0.;
	double AccCorrection = 1;
	size_t StepInterval;
	if (m_pPGSource->IsEquiDistance()) {
		auto dist = m_pPGSource->GetEquiDistance();
		AccCorrection = 1. / (1440 / dist);
		StepInterval= 1440 / dist;
	}
	

	vector<size_t> resetindex;
	if (m_ReShape.m_Cumulated == 2) resetindex = ((PGCalc*)m_pPGSource)->GetYearStartIndex();
	else if (m_ReShape.m_Cumulated == 3) resetindex = ((PGCalc*)m_pPGSource)->GetDayStartIndex();


	if ((GetIndepSecond() == PGHOUR&&m_pPGSource->GetNumberofYears() > 1 && GetIndepFirst() == PGDAY)
		|| (GetIndepSecond() == PGDAY&&m_pPGSource->GetNumberofYears() > 1 && GetIndepFirst() == PGHOUR) ||
		(GetIndepSecond() == PGYEAR&&m_pPGSource->GetNumberofYears() > 1 && GetIndepFirst() == PGDAY)
			|| (GetIndepSecond() == PGDAY&&m_pPGSource->GetNumberofYears() > 1 && GetIndepFirst() == PGYEAR)) {
		if (StepInterval > 1)
			for (size_t i = 1; i < resetindex.size(); i++)
				resetindex[i] = resetindex[i] / StepInterval + 1;
		AccCorrection = 1;
	}


	size_t first = 0;
	for (long i = 0; i < pVector->size(); i++) {
		if (m_ReShape.m_Cumulated >= 2&& resetindex.size()>first) {
			if (i + 1 == resetindex[first]) {
				Acc = 0.0;
				first++;
				first = min(resetindex.size() - 1, first);
			}
		}
		if (abs(pVector->at(i)) < 1.E38) {
			Acc += pVector->at(i)*AccCorrection;
			pVector->at(i) = Acc;
		}

	}
}
void ChartProp::DocumentError(Doc * pDoc)
{
	// Find another Document and open that view.
	size_t Action = 1;

	CWinApp* pApp = AfxGetApp();

	CCoupModelApp *pCoupModel = (CCoupModelApp*)pApp;
	CMultiDocTemplate* pDocTemplate;

	pDocTemplate = pCoupModel->m_pDocTemplate;


	CView *pView;
	CCoupModelDoc* dg_pDoc;
	CDocument *pDocument;

	POSITION pos2 = pDocTemplate->GetFirstDocPosition();
	int numdoc;
	numdoc = 0;
	CMainFrame *p_mainframe;
	p_mainframe = (CMainFrame*)AfxGetMainWnd();



	if (pDocument = pDocTemplate->GetNextDoc(pos2)) {

		POSITION posd = pDocument->GetFirstViewPosition();
		pView = pDocument->GetNextView(posd);
		dg_pDoc = (CCoupModelDoc*)pDocument;
		CString str = CString(dg_pDoc->m_Doc.GetCurrentSimFile().c_str());

		if (dg_pDoc->m_Doc.m_DocumentType == 2 && pDoc->m_DocumentType == 2) {
			if (pView != nullptr) pView->GetParentFrame()->ActivateFrame();
			p_mainframe->m_wndPG.m_PGgrid.SetDocument(dg_pDoc);
			p_mainframe->m_wndPG.m_PGgrid.OnSetup();
		}
		else if (dg_pDoc->m_Doc.m_DocumentType == 0 && pDoc->m_DocumentType == 0) {
			if(pView!=nullptr) pView->GetParentFrame()->ActivateFrame();
			MySingle::instance().GetCugPointer()->SetDocument(dg_pDoc);
			MySingle::instance().GetCugPointer()->OnSetup();


		}

	}


















}
bool ChartProp::ChangeChartTypeForward()
{
	
	m_ChartType=CHART_TYPES(int(int(m_ChartType)+1)%4);
	return true;
	
}
string ChartProp::GetChartTypeString()
{
	string str;
	switch (m_ChartType) {
	case 0:
		str = "Scatter:";
		break;
	case 1:
		str = "Time Serie:";
		break;
	case 2:
		str = "Distribution:";
		break;
	case 3:
		str = "Cumulative Dist:";
		break;
	case 4:
		str = "Contour:";
		break;
	}
	return str;
}
vector<string> ChartProp::GetChartTypeOptions(bool Complete)
{
	vector<string> out;
	if (Complete) 
		for (size_t i = 0; i < 5; i++)	out.push_back(m_ChartTypesOptions[i].typtext);
	else
		for (size_t i = 1; i < 4; i++)	out.push_back(m_ChartTypesOptions[i].typtext);
	return out;
}
CHART_TYPES ChartProp::GetChartTypeIndex()
{
	return m_ChartType;
}
bool ChartProp::SetChartType(int chart_type) {
	if (chart_type >= 0 && chart_type <= 4) {
		m_ChartType = CHART_TYPES(chart_type);
		return true;
	}
	else
		return false;
}
bool ChartProp::SetChartType(CHART_TYPES chart_type) {
	if (chart_type >= CHART_TYPES::SCATTER && chart_type <= CHART_TYPES::CONTOUR_CHART) {
		m_ChartType = chart_type;
		return true;
	}
	else
		return false;
}

bool ChartProp::SetChartType(string chart_type_string)
{
	size_t ChartTypeIndex=0;
	while (m_ChartTypesOptions[ChartTypeIndex].typtext != chart_type_string&&
		ChartTypeIndex<m_ChartTypesOptions.size()-1) {
		ChartTypeIndex++;
	}
	if (ChartTypeIndex < m_ChartTypesOptions.size()) {
		m_ChartType = CHART_TYPES(ChartTypeIndex);
		return true;
	}
	return false;
}
