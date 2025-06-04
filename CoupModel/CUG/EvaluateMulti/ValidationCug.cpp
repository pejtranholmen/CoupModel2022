 /////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived ValidationCug
#include "..\..\stdafx.h"
#include "..\..\Util.h"
#include "..\..\resource.h"
#include "ValidationCug.h"
#include "..\SimDoc\MyCug.h"
#include "..\..\PG\PGCalc.h"
#include "..\..\PG\CreatePG.h"
#include "..\..\PG\CalculateVariable.h"
#include "..\..\Validation\ValidationEvaluation.h"
#include "..\..\NewBase\FileScanner.h"
#include "..\..\CoupModelView.h"
#include "..\ChartProperties\ChartCug.h"
#include "..\..\MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(ValidationCug,CUGCtrl)
	//{{AFX_MSG_MAP(ValidationCug)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

UINT UpdateValidationVar( LPVOID pParam )
{
    ValidationCug* pObject = (ValidationCug*)pParam;
	pObject->m_OnGoingUpdate = true;
	//pObject->EstimateNewPerformance(pObject->m_CumulatedPerformance);
	pObject->m_pSimDoc->EstimateNewTransformedPerformance(pObject->m_Transformation, pObject->m_Dim1_Index);

	pObject->m_pSimDoc->m_MStorage.UpdateAcceptedRuns(true, false);
	pObject->NewPerformanceReady();
	pObject->m_pSimDoc->MR_ReCalculatePostDist(false);
	pObject->m_OnGoingUpdate = false;
    return 0;   // thread completed successfully
}
UINT UpdateNewSubPeriods( LPVOID pParam )
{
    ValidationCug* pObject = (ValidationCug*)pParam;
	pObject->DefineNewSubPeriod();

    // do something with 'pObject'

    return 0;   // thread completed successfully
}






/////////////////////////////////////////////////////////////////////////////
// ValidationCug construction/destruction
ValidationCug::ValidationCug()
{
//	UGXPThemes::UseThemes(true);
	m_bDrawExcelFocus = TRUE;
	m_col=1;
	m_row=1;
	m_Data=FALSE;
	SetMultiSelectMode ( TRUE );
	//DragDropTarget ( TRUE );
	m_Changed=FALSE;
	m_Updated=FALSE;
	m_ShowMean=0;
	m_pPG=nullptr;
	m_pCUG=nullptr;
	m_ColorChanged=RGB(255,200,200);
	m_ColorEdit=RGB(40,240,240);
	m_ColorClick=RGB(240,240,50);
	m_ColorHeading=RGB(230,230,230);
	m_NoColor=RGB(255,255,255);
	//m_font.CreateFont(14,0,0,0,500,0,0,0,0,0,0,0,0,_T("Arial"));
	GetDefaultFont(m_font);
	m_NewEstimations=FALSE;
	m_pNewLogLi=nullptr;
	m_SelectedTimeShow=2;
	m_SelectedTimeIndCount=0;
	m_SelectedVarShow=1;
	m_SelectedVarIndCount=0;
	m_NumberNewSplitPeriods=5;
	m_pSimDoc=nullptr;
	m_pPlotInfo=nullptr;
	m_SavedCriteria2File=false;
	m_OnGoingUpdate=false;
	m_pUtil = MySingle::instance().GetChartUtilPointer();


}

void ValidationCug::SetpCUG(void *pCUG)
{
	m_pCUG=MySingle::instance().GetCugPointer();
	m_pProp=MySingle::instance().GetChartPropPointer();
	m_pPlotInfo=MySingle::instance().GetPlotInfoPointer();
}

void ValidationCug::SetpPG(CPG *pPG)
{
	m_pPG=pPG;
}
void ValidationCug::SetpDoc(CCoupModelDoc *pDoc)
{
	m_pSimDoc=&pDoc->m_Doc;
	m_pPlotInfo=MySingle::instance().GetPlotInfoPointer();
	m_pProp=MySingle::instance().GetChartPropPointer();

	m_pCUG=MySingle::instance().GetCugPointer();
}
ValidationCug::~ValidationCug()
{
	if(m_pNewLogLi!=nullptr) {
		delete [] m_pNewLogLi;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid

void ValidationCug::OnUpdate(int col, long row, BOOL data)
{
	m_col=col;
	m_row=row;
	m_Data=data;
	EnableUpdate(FALSE); 
	while(GetNumberRows()>0)
		DeleteRow(0);
	EnableUpdate(TRUE); 

	OnSetup();

}

void ValidationCug::OnSetup()
{
	EnableMenu(TRUE); 
	EnableJoins(TRUE); 
	EnableUpdate(TRUE);
	UseHints(TRUE);

	SetDefFont(&m_font);

	

		SetDefColWidth(10);
		if(m_pSimDoc==nullptr) return;
		if(!m_pSimDoc->IsMultiSimulation()||!m_pSimDoc->IsFinishedSimulation()||m_pSimDoc->m_MStorage.IsFileMissing()||!m_pSimDoc->m_MStorage.m_IsStatOpen) {
			return;
		}
		int num_sheets = GetNumberSheets();
		for (long i = 0; i<num_sheets; i++) {
			DeleteTab(i);
		}
		m_row=m_pSimDoc->m_Val_Array.size()+m_pSimDoc->m_ValSum_Array.size();
//******* Add New Tabs 
		AddTab("Coefficient of Determination", 0); 
		AddTab("Intercept", 1); 
		AddTab("Slope", 2); 
		AddTab("Mean Error", 3); 
		AddTab("Root Mean Square Error", 4); 
		AddTab("LogLikelihood", 5); 
		AddTab("Nash–Sutcliffe R2", 6); 
		AddTab("Criteria on Output Stat",7);
		AddTab("Defined criterias", 8); 
		AddTab("Loglikelihood options", 9); 
		AddTab("Loglikelihood statistics", 10);
		AddTab("Residuals statistics", 11);	
		AddTab("Ensembles - Split", 12);
		SetTabWidth(1000); 

		SetNumberSheets(13); 
		OnSheetSetup(0);
		SetCurrentTab(0);
}


bool ValidationCug::IsAcceptedEstimated() {
	auto stat_post = m_pSimDoc->m_MStorage.GetValidStat(0, true, m_pSimDoc->m_MStorage.m_ActualEnsemble, 0);

	if(stat_post.Max>stat_post.Min) return true;
	return m_pSimDoc->m_MStorage.m_AcceptedUpdated;

}
/////////////////////////////////////////////////////////////////////////////
//	OnSheetSetup	
void ValidationCug::Make_MainSheets(int SheetNumber)
{


	CUGCell cell;
	auto checkens = m_pSimDoc->m_MStorage.m_ActualEnsemble;
//
	if (!m_pSimDoc->m_MStorage.IsMaxUpdated[checkens] || !m_pSimDoc->m_MStorage.IsMinUpdated[checkens])
		m_pSimDoc->m_MStorage.SetMinMaxOfPerformanceIndicators();

	m_SplitPeriods = m_pSimDoc->m_MStorage.GetNumSplitPeriods();

	int NumLinesSV = m_pSimDoc->m_ValidationData.GetNumSumVarVariables();
	m_col = 14;
	int cols = m_col,
		rows = m_row;
	if (m_pSimDoc->m_MStorage.IsFileMissing() || (m_row < 1 && NumLinesSV < 1)) return;

	//	if(!IsAcceptedEstimated()) m_pSimDoc->m_MStorage.UpdateAcceptedRuns(true, true);

	SetSheetNumber(SheetNumber);
	string txt;
	string short_perf_name;

	switch (SheetNumber) {
	case 0:
		short_perf_name = "R2 -";
		txt = "Coefficients of Determination for the regression line";

		break;
	case 1:
		short_perf_name = "A0 -";
		txt = "Intercept for linear regression line with simulated values as independent variable";
		break;
	case 2:
		short_perf_name = "A1 -";
		txt = "Slope for linear regression line with simulated values as independent variable";
		break;
	case 3:
		short_perf_name = "ME -";
		txt = "Mean Error between simulated values and measured variables";
		break;
	case 4:
		short_perf_name = "RMSE - ";
		txt = "Root Mean Square Error between simulated values and measured variables";
		break;
	case 5:
		short_perf_name = "LogLi - ";
		txt = "Log Likelihood values";
		break;
	case 6:
		short_perf_name = "NSE - ";
		txt = "Nash Sutcliffe Coefficient of Determination (Modelling efficiency)";
		break;


	}
	
	//	m_pDlg->SetWindowText(txt);
	//	m_pDlg->OnInitDialog();
	SetDefFont(&m_font);

	EnableUpdate(FALSE);
	while (GetNumberRows() > 0)
		DeleteRow(0);
	EnableMenu(TRUE);
	EnableUpdate(TRUE);
	SetNumberCols(cols);
	SetSH_Width(90);
	SetSH_NumberCols(2);
	if (NumLinesSV > 1)
		SetNumberRows(rows + 3);
	else
		SetNumberRows(rows);
	GetColDefault(0, &cell);
	cell.SetAlignment(UG_ALIGNRIGHT);
	for (int i = 0; i < cols; i++) {
		if (m_Data || i > 3)
			SetColDefault(i, &cell);
		if (i == 1) SetColWidth(i, 90);
	}
	SetMultiSelectMode ( TRUE );

	SetVScrollMode(UG_SCROLLTRACKING);

	SetUniformRowHeight(TRUE);
	GetCell(-1, -1, &cell);
	if (m_pSimDoc->m_MStorage.IsCVUpdated[checkens])
		cell.SetBackColor(m_ColorClick);
	else
		cell.SetBackColor(m_NoColor);

	string str;
	if (m_pSimDoc->m_MStorage.m_AcceptedIdString.size() > 0)
		str = m_pSimDoc->m_MStorage.m_AcceptedIdString[checkens]+ "-"+FUtil::STD_ItoAscii(m_pSimDoc->m_MStorage.m_AcceptedIdString.size());
	else
		str = "";

	cell.SetText(str);
	if(!m_Distribution_As_Independent) 	m_pProp->SetTitle(str);
	else m_pProp->SetTitle("");
	SetCell(-1, -1, &cell);




	GetCell(1, -2, &cell);
	cell.SetCellTypeEx(UGCT_NORMALMULTILINE);
	SetCell(1, -2, &cell);
	SetTH_NumberRows(2);
	SetTH_Height(50);
	SetTH_RowHeight(-2, 25);
	SetTH_RowHeight(-1, 25);

	SetColWidth(-1, 30);
	SetColWidth(1, 100);
	SetColWidth(0, 200);
	SetColWidth(2, 40);
	for (int i = 3; i < cols; i++)
		SetColWidth(i, 62);

	m_Transformation = m_pSimDoc->m_MStorage.m_CumulatedPerformance;

	auto act_ensemble = m_pSimDoc->m_MStorage.m_ActualEnsemble;
	auto tests = m_pSimDoc->m_MStorage.m_Valid_TSV[act_ensemble].VarTranformed;

	QuickSetText(0,-2, m_pSimDoc->m_MStorage.GetTransformationIndicator(m_pSimDoc->m_MStorage.m_ActualEnsemble));


	QuickSetBackColor(0, -2, m_ColorClick);
	QuickSetText(0, -1, "Variable Name");

	auto koll=m_pSimDoc->m_MStorage.m_Valid_TSV.size();

	long iaccept = m_pSimDoc->m_MStorage.GetNumAccepted();
	long iacceptall = m_pSimDoc->m_MStorage.GetNumberOfRuns();
	if (iacceptall <= 0) return;

	iaccept = m_pSimDoc->m_MStorage.m_AcceptedRuns[m_pSimDoc->m_MStorage.m_ActualEnsemble].size();


	QuickSetText(3, -2, short_perf_name+"Distribution Prior Selection n=" + FUtil::STD_ItoAscii(iacceptall));
	QuickSetText(7, -2, short_perf_name+"Distribution Post Selection n=" + FUtil::STD_ItoAscii(iaccept));
	if (iaccept != iacceptall) QuickSetBackColor(5, -2, m_ColorEdit);
	QuickSetText(12, -2, "Limits for Selection");

	
	
	if (m_Distribution_As_Independent) {
		
		GetCell(1, -2, &cell);
		cell.SetBackColor(m_ColorClick);
		//if (m_pProp->m_ChartType == CHART_TYPES::TIME_SERIE) txt = "serie";
		//else if (m_pProp->m_ChartType == CHART_TYPES::FREQUENCE) txt = "frequence";
		//else if (m_pProp->m_ChartType == CHART_TYPES::CUM_FREQUENCE) ;
		txt = "cumulated";
		cell.SetText(txt);
		SetCell(1, -2, &cell);
	}
	else {
		GetCell(1, -2, &cell);
		cell.SetBackColor(m_ColorClick);
		if (m_Add_Second_Independent) txt = "2 Independent Par";
		else {
			txt = "1 Independent Par";
			m_pProp->SetSecondIndepVarType(NO_USE);
		}
		cell.SetText(txt);
		SetCell(1, -2, &cell);
	}
	
	//QuickSetText(1, -2, "select");
	
	QuickSetText(1, -1, "Plot");
	QuickSetText(2, -2, "n");
	QuickSetText(3, -1, "Mean");
	QuickSetText(4, -1, "CV");
	QuickSetText(5, -1, "Min");
	QuickSetText(6, -1, "Max");
	QuickSetText(7, -1, "Mean");
	QuickSetText(8, -1, "CV");
	QuickSetText(9, -1, "Min");
	QuickSetText(10, -1, "Max");
	QuickSetText(11, -1, "Range");
	QuickSetText(12, -1, "Low");
	QuickSetText(13, -1, "High");
	//JoinCells(0,-2,0,-1);
	//JoinCells(1, -2, 1, -1);
	JoinCells(2, -2, 2, -1);
	JoinCells(3, -2, 6, -2);
	JoinCells(7, -2, 11, -2);
	JoinCells(12, -2, 13, -2);

	int NumLines = 0;
	F* pValFile;

	
	m_NumValidVar = m_pSimDoc->GetNumAll_TSV_ValVariables();
	if (m_pSimDoc->m_MStorage.GetCyclicNumber() > 1&& m_NumValidVar> m_pSimDoc->m_MStorage.GetCyclicNumber())
	//m_NumValidVar/= m_pSimDoc->m_MStorage.GetCyclicNumber();
	m_pSimDoc->m_MStorage.UpdateCyclicNumbers(m_pSimDoc->m_MStorage.m_ActualEnsemble);


	Type = "Tables";
	Group = "Technical";
	Name = "Scaling of Observed Variables";
	SimB *Sw;
	P *pCPt = nullptr;

	Sw = m_pSimDoc->GetPtr(Type, "Technical", Name);
	if (Sw != nullptr&&NumLines > 0) {
		int oldnelements = ((Tab*)Sw)->GetNumOfRow();
		if (NumLines != oldnelements) {
			((Tab*)Sw)->SetNumOfRow(NumLines);
			//Sw->Apply_CE_only();
			//m_pSimDoc->ReallocateTheWholeMap();
		}
		pCPt = ((Tab*)Sw)->GetP_Pointer(0);
	}


	int TotLines = m_NumValidVar;
	if (m_SplitPeriods > 0) {
		TotLines = m_NumValidVar*(m_SplitPeriods + 1);
	}
	if (NumLinesSV > 0 && m_NumValidVar > 0)
		TotLines = TotLines + 1 + NumLinesSV;
	else if (NumLinesSV > 0)
		TotLines = NumLinesSV+1;
	if (NumLinesSV > 1 && m_NumValidVar > 1)
		SetNumberRows(TotLines + 2);
	else if (NumLinesSV > 1 || m_NumValidVar > 1)
		SetNumberRows(TotLines + 1);
	else
		SetNumberRows(TotLines);

	m_NumLines = TotLines;



	string test;
	int FileNo, ind, index_start;
	index_start = SheetNumber*m_NumValidVar;
	double allmax, allmin, selmax, selmin, allcv, selcv;
	SimpleStatistics stat_all, stat_post;
	int criteria;
	int line;
	if (m_NumValidVar > 0) {
		stat_all= m_pSimDoc->m_MStorage.GetValidStat(0, false);
		if (stat_all.Max == stat_all.Min) {
			m_pSimDoc->m_MStorage.SetPriorDistributions(); //Dumm
		}
	}
	SetTopRow(m_CurrentTopRow);
line=-1;
if (!m_pSimDoc->m_MStorage.Check_Adjust_ActualEnsemble()) {

	MFC_Util::MessageBox(" The Actual Ensemble was adjusted to the first because of to few Ensembles in current Document");


};

for(int i=0;i<m_NumValidVar;i++) {
		test=FUtil::STD_ItoAscii(i+1);
		FileNo=m_pSimDoc->m_ValidationData.GetValFileNumber(i);
		CResiduals *pRes = m_pSimDoc->GetValidResidualPointer(FileNo);



		pValFile=m_pSimDoc->ValidationFilePointer(FileNo);
		ind=m_pSimDoc->m_ValidationData.GetValFileIndex(i);
		CPG *pPG;
		BOOL valid;
		pPG=(CPG*)pValFile->GetPointer();
		if(pPG==nullptr) {
			pPG=new CPG;
			//CPG_Array.Add(pCPG);
			valid=pPG->ReadContentOfPGFileAndOpenMainPGStreamForReading(pValFile->GetStrValue().c_str());
			if(valid) pValFile->SetPointer(pPG);
			pPG->CloseFile();
		}
		test=pValFile->GetDataDescription(ind);
		if (test == "-") {
			pValFile->CallDataDescription();
			test = pValFile->GetDataDescription(ind);

		}

		for (int split_ind=0; split_ind<=m_SplitPeriods;split_ind++) {
			line++;
			if (pRes != nullptr) {
				if (!pRes->m_MeanDefined) 	QuickSetBackColor(-1, line, m_ColorClick);
				else QuickSetBackColor(-1, line, m_NoColor);
			}
			auto vname = FUtil::STD_ItoAscii(i + 1) + ":" + test;
			QuickSetText(0,line, vname);
			criteria=m_pSimDoc->m_MStorage.GetCriteria(i+index_start, split_ind);
			stat_all= m_pSimDoc->m_MStorage.GetValidStat(i+index_start, false, m_pSimDoc->m_MStorage.m_ActualEnsemble, split_ind);
			stat_post = m_pSimDoc->m_MStorage.GetValidStat(i + index_start, true, m_pSimDoc->m_MStorage.m_ActualEnsemble, split_ind);

			selmax=double(stat_all.Max);	
			selmin=double(stat_all.Min);
			selcv=double(stat_all.CV);
			if(split_ind==0) {

					if (act_ensemble > 0) {
						QuickSetText(-2, line, "<");
						QuickSetBackColor(-2, line, m_ColorClick);
					}
					else {
						QuickSetText(-2, line, "");
						QuickSetBackColor(-2, line, m_NoColor);
					}
					QuickSetText(-1, line, ">");
					QuickSetBackColor(-1, line, m_ColorClick);
					
				if (abs(stat_all.Mean) < 1.E38)		
					QuickSetNumber(2,line,m_pSimDoc->m_ValidationData.GetValn(i));
				else
					QuickSetNumber(2, line, 0);
				GetCell(1,line,&cell);
				cell.SetCellType(UGCT_CHECKBOX);
				cell.SetText("");
				cell.SetReadOnly(TRUE);
				if (abs(stat_all.Mean) < 1.E38)
					cell.SetReadOnly(FALSE);
				cell.SetBool(FALSE);	
				SetCell(1,line,&cell);
			}
			else {
				//QuickSetNumber(1,line, split_ind);
				if (abs(stat_all.Mean) < 1.E38)
					QuickSetNumber(2,line,m_pSimDoc->m_MStorage.GetSplitNValue(i,split_ind-1));
				else
					QuickSetText(2, line, "0");
				GetCell(1,line,&cell);
				cell.SetCellType(UGCT_CHECKBOX);
				cell.SetBool(FALSE);
				string str=FUtil::STD_ItoAscii(split_ind);
				cell.SetLabelText(str);
				SetCell(1,line,&cell);
			}
			if(split_ind>0&&split_ind==m_SplitPeriods) {
				JoinCells(0,line-m_SplitPeriods,0,line);
				JoinCells(-1,line-m_SplitPeriods,-1,line);
			}
			if(abs(stat_all.Mean)<1.E38) QuickSetNumber(3,line,stat_all.Mean);	else QuickSetText(3, line, "");
			if(abs(stat_all.CV)<1.E38)	 QuickSetNumber(4,line,stat_all.CV); else QuickSetText(4, line, "");
			if(abs(stat_all.Min)<0.5E38) QuickSetNumber(5,line,stat_all.Min); else QuickSetText(5, line, "");
			if(abs(stat_all.Max)<0.5E38) QuickSetNumber(6,line,stat_all.Max); else QuickSetText(6, line, "");
	
			if(abs(stat_post.Mean)<0.5E38)	QuickSetNumber(7,line,stat_post.Mean); 	else QuickSetText(7, line, "");
			if(abs(stat_post.CV)<0.5E38)	QuickSetNumber(8,line,stat_post.CV);	else QuickSetText(8, line, "");
			if(abs(stat_post.Min)<0.5E38)	QuickSetNumber(9,line,stat_post.Min); 	else QuickSetText(9, line, "");
			if(abs(stat_post.Max)<0.5E38)	QuickSetNumber(10,line,stat_post.Max); 	else QuickSetText(10, line, "");
			double rangef;
			if(stat_all.Max-stat_all.Min>0&&abs(stat_all.Max)<1.E38&&abs(stat_all.Min)<0.5e38) {
				rangef=(selmax-selmin)/(stat_all.Max-stat_all.Min);	QuickSetNumber(11,line,rangef);
				if (rangef == 0) {

					if (stat_post.Max > stat_all.Max) {
						m_pSimDoc->m_MStorage.SetValidHigh(i + index_start, 0, stat_post.Max);
					
					}
					if (m_pSimDoc->m_MStorage.GetValidLow(i + index_start, 0) < stat_all.Min) {
						m_pSimDoc->m_MStorage.SetValidLow(i + index_start,0,stat_all.Min);

					}
				}
			}
			else if(abs(stat_all.Mean)>0.5E38) 
				QuickSetText(11, line, "");
			else
				QuickSetNumber(11, line, 1.0);

			if(criteria==10) {


			}
			else {
				GetCell(12,line, &cell);
				cell.SetParam(split_ind);
				float value;
				value=m_pSimDoc->m_MStorage.GetValidLow(i+index_start, split_ind);
				if(abs(value)<0.5E38)
					cell.SetNumber(value);
				else
					cell.SetText("");
				cell.SetBackColor(m_NoColor);
				if (m_pSimDoc->IsMultiStorageFileLocked()) {
					cell.SetReadOnly(TRUE);
					//cell.SetParam(m_SetUnLockHint.param);
				}
				else {
					cell.SetReadOnly(FALSE);
					//cell.SetParam(m_SetLowerHint.param);

				}
				if(criteria==1||criteria==3)cell.SetBackColor(m_ColorEdit);
				else cell.SetBackColor(m_NoColor);
				SetCell(12, line, &cell);
				GetCell(13,line, &cell);
				cell.SetParam(split_ind);
				value=m_pSimDoc->m_MStorage.GetValidHigh(i+index_start, split_ind);
				if (abs(value) < 0.5E38)
					cell.SetNumber(value);
				else
					cell.SetText("");


				//else
					//cell.SetText("");
				cell.SetBackColor(m_NoColor);
				if (m_pSimDoc->IsMultiStorageFileLocked()) {
					cell.SetReadOnly(TRUE);
					cell.SetParam(m_SetUnLockHint.param);
				}
				else {
					cell.SetReadOnly(FALSE);
					//cell.SetParam(m_SetLowerHint.param);


				}
				if(criteria==1||criteria==3) cell.SetBackColor(m_ColorEdit);
				else cell.SetBackColor(m_NoColor);
				SetCell(13, line, &cell);

			}
		}

}


if(m_NumValidVar>1) {
	double limit_low, limit_high;
	CString VarText;
	VarText="Mean of all variables";
	line++;
	criteria=m_pSimDoc->m_MStorage.GetMeanCriteria(SheetNumber);
	PERFORMANCE_VAR pervar=PERFORMANCE_VAR(SheetNumber);
	switch (SheetNumber) {
		case 5:
		case 6:
		VarText="Sum of all variables";
		break;
	}
	selmin = double(m_pSimDoc->m_MStorage.m_MeanTSV_STAT[POST_MIN][pervar]);



	selmax = double(m_pSimDoc->m_MStorage.m_MeanTSV_STAT[POST_MAX][pervar]);
	if (selmax <= selmin) m_pSimDoc->m_MStorage.m_MeanTSV_STAT[POST_MAX][pervar] = m_pSimDoc->m_MStorage.m_MeanTSV_STAT[POST_MIN][pervar] + 1;
	limit_low = double(m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_LOW][pervar]);
	limit_high = double(m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_HIGH][pervar]);
	if (limit_high <= limit_low) {
		m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_LOW][pervar] = -1000.;
		m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_HIGH][pervar] = 1000.;
		limit_low = double(m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_LOW][pervar]);
		limit_high = double(m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_HIGH][pervar]);

	}



		QuickSetText(7,line,VarText); 

		QuickSetNumber(9,line,selmin); 
		QuickSetNumber(10,line,selmax); 


	if(criteria==10) {
		QuickSetText(12,line, "BC");
		QuickSetText(13,line, "BC");
	}
	else {
		QuickSetNumber(12,line,limit_low); 
		if(criteria==1||criteria==3) QuickSetBackColor(12,line, m_ColorEdit);
		if(criteria!=10) QuickSetNumber(13,line,limit_high); 
		if(criteria==2||criteria==3) QuickSetBackColor(13,line, m_ColorEdit);
	}

	//JoinCells(6,line,7,line);

	if (!m_Distribution_As_Independent) {
		for(size_t i=0; i<8;i++) UnJoinCells(i, line);
		QuickSetText(0, line, "First Indep Par");
		JoinCells(7, line, 8, line);
		JoinCells(1, line, 2, line);
		if (m_Add_Second_Independent) {
			JoinCells(3, line, 4, line); QuickSetText(3, line, "Second Index Par");
			JoinCells(5, line, 6, line);
		}
		else {
			JoinCells(3, line, 6, line);
		}

		
		auto NoRow = GetNumberRows();
		NoRow=NoRow - 1;

		GetCell(1, line, &cell);
		cell.SetCellType(UGCT_DROPLIST);

		string optionlist ;

		SimB* pPar;
		int nTabIndex;
		string namep;

		//exclude first dimension if more than one

		size_t num_in_firstdim = 0;
		size_t dimstart = 0;
		if (m_pSimDoc->MR_GetNumberOfDimensions() > 1) {
			dimstart = 1;
			num_in_firstdim=m_pSimDoc->MR_Get_NumberOfParametersWithinDim(1);
		}

		for (size_t i = dimstart; i < m_pSimDoc->MR_GetNumberOfDimensions(); i++) {
			for (size_t ii = 0; ii < m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i + 1); ii++) {
				pPar = m_pSimDoc->MR_Get_pPar(i + 1, ii + 1);
				nTabIndex = m_pSimDoc->MR_GetTabIndex(i + 1, ii + 1);
				namep = pPar->GetName().c_str();
				if (nTabIndex >= 0)
					namep += "(" + FUtil::STD_ItoAscii(nTabIndex + 1) + ")";
				namep += '\n';
				optionlist += namep;
			}
		}
		enum vartype var = m_pProp->GetFirstIndepVarType();


		int index_sel = 0;
		index_sel = m_pProp->GetFirstIndepIndex();
		if (m_pProp->GetFirstIndepIndex() >= num_in_firstdim)
			index_sel -= num_in_firstdim;

		string selected_str = FUtil::GetOptionString(optionlist, index_sel);
		cell.SetText(selected_str.c_str());
		cell.SetLabelText(optionlist.c_str());
		cell.SetParam(100);
		cell.SetBackColor(m_ColorClick);

		SetCell(1, line, &cell);
		if (m_Add_Second_Independent) {
			if (m_pProp->GetIndepFirst() > 0) {
				GetCell(5, line, &cell);
				cell.SetCellType(UGCT_DROPLIST);
				var = m_pProp->GetSecondIndepVarType();
				index_sel = m_pProp->GetSecondIndepIndex();
				if (m_pProp->GetFirstIndepIndex() >= num_in_firstdim)
					index_sel -= num_in_firstdim;
				string selected_str = FUtil::GetOptionString(optionlist, index_sel);
				cell.SetText(selected_str.c_str());
				cell.SetLabelText(optionlist.c_str());
				cell.SetParam(300);
				cell.SetBackColor(m_ColorClick);
				SetCell(5, line, &cell);
			}
		}
		//QuickSetText(0, line, "koll");
	}




}
if (NumLinesSV > 0)
{
	line++;
	QuickSetText(0, line, "Single Variable Type");
	JoinCells(0, line, 13, line);
	QuickSetBackColor(0, line, m_ColorHeading);

	float value;
	for (int i = 0; i < NumLinesSV; i++) {

		line++;
		test = m_pSimDoc->m_ValidationData.GetValSumVarName(i).c_str();
		test += " - ";
		test += m_pSimDoc->m_ValidationData.GetStringValSumVarOutputType(i).c_str();
		QuickSetNumber(-1, line, i + 1);
		QuickSetText(0, line, test);
		JoinCells(0, line, 2, line);
		int index;
		float vmin, vmax, range, vmin1, vmax1;
		PERFORMANCE_VAR pervar;

		index = SheetNumber *m_pSimDoc->GetNumberofSumVariables() + i;

		size_t ActualEnsemble = m_pSimDoc->m_MStorage.m_ActualEnsemble;

		if (m_pSimDoc->m_MStorage.m_Valid_SV[ActualEnsemble].v[E_PRIOR_MEAN].size() == 0) {
			if (ActualEnsemble > 0) ActualEnsemble = 0;
			if (m_pSimDoc->m_MStorage.m_Valid_SV[ActualEnsemble].v[E_PRIOR_MEAN].size() == 0) return;
		}
		pervar = PERFORMANCE_VAR(SheetNumber);

		value = m_pSimDoc->m_MStorage.m_Valid_SV[ActualEnsemble].v[E_PRIOR_MEAN][index];
		QuickSetNumber(3, line, value);
		value = m_pSimDoc->m_MStorage.m_Valid_SV[ActualEnsemble].v[E_PRIOR_CV][index];
		QuickSetNumber(4, line, value);
		value = m_pSimDoc->m_MStorage.m_Valid_SV[ActualEnsemble].v[E_PRIOR_MIN][index];
		QuickSetNumber(5, line, value);
		value = m_pSimDoc->m_MStorage.m_Valid_SV[ActualEnsemble].v[E_PRIOR_MAX][index];
		QuickSetNumber(6, line, value);
		value = m_pSimDoc->m_MStorage.m_Valid_SV[ActualEnsemble].v[E_POST_MEAN][index];
		float f_norm = FUtil::GetNormalValue(value, m_pSimDoc->m_MStorage.m_Valid_SV[ActualEnsemble].v[E_PRIOR_MIN][index], m_pSimDoc->m_MStorage.m_Valid_SV[ActualEnsemble].v[E_PRIOR_MAX][index]);
		if (f_norm<0.4 || f_norm>0.6) {
			rgb_color rgb = FUtil::Get_RGB(f_norm);
			COLORREF refcolor;
			refcolor = RGB(rgb.red, rgb.green, rgb.blue);
			QuickSetBackColor(7, line, refcolor);
		}
		else {
			QuickSetBackColor(7, line, m_NoColor);
		}
		QuickSetNumber(7, line, f_norm);
		value = m_pSimDoc->m_MStorage.m_Valid_SV[ActualEnsemble].v[E_POST_CV][index];
		QuickSetNumber(8, line, value);
		value = m_pSimDoc->m_MStorage.m_Valid_SV[ActualEnsemble].v[E_POST_MIN][index];
		vmin1 = value;
		QuickSetNumber(9, line, value);
		value = m_pSimDoc->m_MStorage.m_Valid_SV[ActualEnsemble].v[E_POST_MAX][index];

		QuickSetNumber(10, line, value);
		vmax1 = value;
		if (vmax1 > vmin1) {
			range = (vmax1 - vmin1) / (m_pSimDoc->m_MStorage.m_Valid_SV[ActualEnsemble].v[E_PRIOR_MAX][index] - m_pSimDoc->m_MStorage.m_Valid_SV[ActualEnsemble].v[E_PRIOR_MIN][index]);
			QuickSetNumber(11, line, range);
		}
		criteria = m_pSimDoc->m_MStorage.m_Valid_SV[ActualEnsemble].criteria[index];
		value = m_pSimDoc->m_MStorage.m_Valid_SV[ActualEnsemble].v[E_SELECT_LOW][index];
		QuickSetNumber(12, line, value);
		if (line == 0) {
			auto koll = value;
			auto koll2 = criteria;

		}
		if (criteria == 1 || criteria == 3) QuickSetBackColor(12, line, m_ColorEdit);
		else QuickSetBackColor(12, line, m_NoColor);

		value = m_pSimDoc->m_MStorage.m_Valid_SV[ActualEnsemble].v[E_SELECT_HIGH][index];
		QuickSetNumber(13, line, value);
		if (criteria == 2 || criteria == 3) QuickSetBackColor(13, line, m_ColorEdit);
		else QuickSetBackColor(13, line, m_NoColor);



	}
	if (NumLinesSV > 1) {
		double limit_low, limit_high;

		line++;
		size_t ActualEnsemble = m_pSimDoc->m_MStorage.m_ActualEnsemble;
		PERFORMANCE_VAR pervar = PERFORMANCE_VAR(SheetNumber);

		criteria = m_pSimDoc->m_MStorage.m_Valid_SV_Mean[ActualEnsemble].criteria[pervar];

		CString VarText;
		VarText = "Mean of SV variables";
		if(SheetNumber) VarText = "Sum of all variables";			

		QuickSetNumber(3, line, m_pSimDoc->m_MStorage.m_Valid_SV_Mean[ActualEnsemble].v[E_PRIOR_MEAN][pervar]);
		QuickSetNumber(4, line, m_pSimDoc->m_MStorage.m_Valid_SV_Mean[ActualEnsemble].v[E_PRIOR_CV][pervar]);
		QuickSetNumber(5, line, m_pSimDoc->m_MStorage.m_Valid_SV_Mean[ActualEnsemble].v[E_PRIOR_MIN][pervar]);
		QuickSetNumber(6, line, m_pSimDoc->m_MStorage.m_Valid_SV_Mean[ActualEnsemble].v[E_PRIOR_MAX][pervar]);
	
		if (SheetNumber >= 0 && SheetNumber < 7) {
			QuickSetText(0, line, VarText);

			float f_norm = FUtil::GetNormalValue(m_pSimDoc->m_MStorage.m_Valid_SV_Mean[ActualEnsemble].v[E_POST_MEAN][pervar],
				m_pSimDoc->m_MStorage.m_Valid_SV_Mean[ActualEnsemble].v[E_PRIOR_MIN][pervar], 
				m_pSimDoc->m_MStorage.m_Valid_SV_Mean[ActualEnsemble].v[E_PRIOR_MAX][pervar]);
			if (f_norm<0.4 || f_norm>0.6) {
				rgb_color rgb = FUtil::Get_RGB(f_norm);
				COLORREF refcolor;
				refcolor = RGB(rgb.red, rgb.green, rgb.blue);
				QuickSetBackColor(7, line, refcolor);
			}
			else {
				QuickSetBackColor(7, line, m_NoColor);
			}
			QuickSetNumber(7, line, f_norm);

			QuickSetNumber(8, line, m_pSimDoc->m_MStorage.m_Valid_SV_Mean[ActualEnsemble].v[E_POST_CV][pervar]);
			QuickSetNumber(9, line, double(m_pSimDoc->m_MStorage.m_Valid_SV_Mean[ActualEnsemble].v[E_POST_MIN][pervar]));
			QuickSetNumber(10, line, double(m_pSimDoc->m_MStorage.m_Valid_SV_Mean[ActualEnsemble].v[E_POST_MAX][pervar]));

			if (criteria == 10) {
				QuickSetText(12, line, "BC");
				QuickSetText(13, line, "BC");
			}
			else {
				QuickSetNumber(12, line, m_pSimDoc->m_MStorage.m_Valid_SV_Mean[ActualEnsemble].v[E_SELECT_LOW][pervar]);
				if (criteria == 1 || criteria == 3) QuickSetBackColor(12, line, m_ColorEdit);
				else QuickSetBackColor(12, line, m_NoColor);
				if (criteria != 10) QuickSetNumber(13, line, m_pSimDoc->m_MStorage.m_Valid_SV_Mean[ActualEnsemble].v[E_SELECT_HIGH][pervar]);
				if (criteria == 2 || criteria == 3) QuickSetBackColor(13, line, m_ColorEdit);
				else QuickSetBackColor(13, line, m_NoColor);
			}

			//JoinCells(0, line, 2, line);
		}
	}

}


if(SheetNumber<7) MakeSelectionsVisible(SheetNumber);


RedrawAll();


}
void ValidationCug::Make_CriteriaSheet()
{

	int cols = 8,rows;
	CUGCell cell;
	SetSheetNumber(8);
//	m_pDlg->SetWindowText("Criterias used to select accepted runs");
//	m_pDlg->OnInitDialog();

	SetDefFont(&m_font);
	//SetMultiSelectMode ( TRUE );


	
	EnableUpdate(FALSE); 
	while(GetNumberRows()>0)
		DeleteRow(0);
	EnableUpdate(TRUE);
	EnableMenu(TRUE); 
	
	SetNumberCols ( cols );
	int icount=0;

	int NumLinesSV=m_pSimDoc->m_ValidationData.GetNumSumVarVariables();	
	if(m_pSimDoc->m_MStorage.IsFileMissing()||m_pSimDoc->GetNumAll_TSV_ValVariables()+
		m_pSimDoc->GetNumberofSumVariables()<1) return;
	
		auto save_ens = m_pSimDoc->m_MStorage.m_ActualEnsemble;
		for (int split_ind = 0; split_ind <= m_SplitPeriods; split_ind++) {
			for (size_t iens = 0; iens < m_pSimDoc->m_MStorage.m_AcceptedRuns.size(); iens++) {
				m_pSimDoc->m_MStorage.m_ActualEnsemble = iens;
				icount += m_pSimDoc->m_MStorage.m_Valid_TSV[iens].History.size();
				//if (m_pSimDoc->m_MStorage.GetCriteria(i, split_ind)>0 && m_pSimDoc->m_MStorage.GetCriteria(i, split_ind) < 10)
				//	icount++;
			}
		}
		m_pSimDoc->m_MStorage.m_ActualEnsemble = save_ens;

	for(int i=0;i<7;i++) {
		if(m_pSimDoc->m_MStorage.GetMeanCriteria(i)>0&&m_pSimDoc->m_MStorage.GetMeanCriteria(i)<10)
			icount++;
	}
	
	
	for(int i=0;i<NumLinesSV*7;i++) {
		if(m_pSimDoc->m_MStorage.GetSumCriteria(i)>0&&m_pSimDoc->m_MStorage.GetSumCriteria(i)<10)
			icount++;
	}
	if (m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].criteria.size() < 7) m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].criteria.resize(7);
	for(int i=0;i<7;i++) {
		if(m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].criteria[i]>0&&
			m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].criteria[i]<10)
			icount++;
	}

	rows=icount;

	SetNumberRows ( rows );
	SetTH_NumberRows(1);
	for(int i=0; i<rows;i++)
		for (int j=0; j<cols;j++)
			UnJoinCells(j,i);

	for (int j=0; j<cols;j++) {
		//UnJoinCells(j,-2);
		UnJoinCells(j,-1);
	}
	SetColWidth(-1,30);
	SetColWidth(0,80);
	SetColWidth(1, 80);
	SetColWidth(2,200);
	for(int i=3;i<cols;i++) 
	    SetColWidth(i,100);
	SetColWidth(4, 140);
	GetColDefault(0,&cell);
	cell.SetAlignment(UG_ALIGNRIGHT);
	QuickSetText(-1,-1,"No");
	QuickSetText(0, -1, "Ensemble No");
	QuickSetText(1,-1,"Varialbe No");
	QuickSetText(2,-1,"Variable Name");
	QuickSetText(3,-1,"Sub Period");
	QuickSetText(4,-1,"Performance index");
	QuickSetText(5,-1,"Minimum");
	QuickSetText(6,-1,"Maximum");
	QuickSetText(7, -1, "n Left");

    int index;
	icount=0;
	int criteria;
	int FileNo, ind;

	F* pValFile;
	string PerformanceVar, test;
	int MeanCriteria;
	//for(int var=0;var<7;var++) {
		

		save_ens = m_pSimDoc->m_MStorage.m_ActualEnsemble;
		for (int split_ind=0; split_ind<=m_SplitPeriods;split_ind++) {
			for (size_t iens = 0; iens < m_pSimDoc->m_MStorage.m_AcceptedRuns.size(); iens++) {
				m_pSimDoc->m_MStorage.m_ActualEnsemble = iens;

				for (size_t histindex = 0; histindex < m_pSimDoc->m_MStorage.m_Valid_TSV[iens].History.size(); histindex++) {
					DefinedCriteria def = m_pSimDoc->m_MStorage.m_Valid_TSV[iens].History[histindex];
					size_t local_var_in_list = def.VarIndex%m_pSimDoc->m_Val_Array.size();
					FileNo = m_pSimDoc->m_ValidationData.GetValFileNumber(local_var_in_list);
					pValFile = m_pSimDoc->ValidationFilePointer(FileNo);

					if(def.VarIndex ==m_pSimDoc->m_Val_Array.size()) MeanCriteria = m_pSimDoc->m_MStorage.GetMeanCriteria(def.VarIndex);
					switch (def.stat_indicator) {
							case PERFORMANCE_VAR::R2_STAT: PerformanceVar = "R2"; break;
							case PERFORMANCE_VAR::AO_STAT: PerformanceVar = "Intercept"; break;
							case PERFORMANCE_VAR::A1_STAT: PerformanceVar = "Slope"; break;
							case PERFORMANCE_VAR::ABSME_STAT: PerformanceVar = "Mean Error"; break;
							case PERFORMANCE_VAR::RMSE_STAT: PerformanceVar = "RMSE"; break;
							case PERFORMANCE_VAR::LOGLI_STAT: PerformanceVar = "Log Like"; break;
							case PERFORMANCE_VAR::NSE_STAT: PerformanceVar = "NSE"; break;
					};
					CPG* pPG;
					BOOL valid;
					pPG = (CPG*)pValFile->GetPointer();
					if (pPG == nullptr) {
						pPG = new CPG;
						//CPG_Array.Add(pCPG);
						valid = pPG->ReadContentOfPGFileAndOpenMainPGStreamForReading(pValFile->GetStrValue().c_str());
						if (valid) pValFile->SetPointer(pPG);
						pPG->CloseFile();
					}
					ind = m_pSimDoc->m_ValidationData.GetValFileIndex(local_var_in_list);
					test = pValFile->GetDataDescription(ind).c_str();
					QuickSetNumber(0, icount, iens + 1);
					if (iens == save_ens)
						QuickSetBackColor(0, icount, m_ColorChanged);
					else
						QuickSetBackColor(0, icount, m_NoColor);

					QuickSetNumber(1, icount, local_var_in_list + 1);
					QuickSetText(2, icount, test);

					test = PerformanceVar;
					if (m_pSimDoc->m_MStorage.m_Valid_TSV[iens].VarTranformed != 0) {
						test += " - ";
						test += m_pSimDoc->m_MStorage.GetTransformationIndicator(iens);
					}
					QuickSetText(4, icount, test);
					if (def.stat_indicator == ABSME_STAT || def.stat_indicator == A1_STAT) {
						def.criteria = ABOVE_AND_BELOW;

					}
					switch (def.criteria) {
						case ABOVE_MIN:	QuickSetNumber(5, icount, def.minvalue);break;
						case BELOW_MAX:	QuickSetNumber(6, icount, def.maxvalue); break;
						case ABOVE_AND_BELOW:	QuickSetNumber(5, icount, def.minvalue); 
							QuickSetNumber(6, icount, def.maxvalue);
							break;
					}

					QuickSetNumber(7, icount, def.n_post);
					icount++;
					QuickSetNumber(-1, icount - 1, icount);
				}

				/*for (int i = 0; i < m_NumValidVar; i++) {
					index = i + var*m_NumValidVar;

					criteria = m_pSimDoc->m_MStorage.GetCriteria(index, split_ind);
					if (criteria > 0) {
						FileNo = m_pSimDoc->m_ValidationData.GetValFileNumber(i);
						pValFile = m_pSimDoc->ValidationFilePointer(FileNo);
						ind = m_pSimDoc->m_ValidationData.GetValFileIndex(i);
						CPG *pPG;
						BOOL valid;
						pPG = (CPG*)pValFile->GetPointer();
						if (pPG == nullptr) {
							pPG = new CPG;
							//CPG_Array.Add(pCPG);
							valid = pPG->ReadContentOfPGFileAndOpenMainPGStreamForReading(pValFile->GetStrValue().c_str());
							if (valid) pValFile->SetPointer(pPG);
							pPG->CloseFile();
						}
						test = pValFile->GetDataDescription(ind).c_str();
						QuickSetNumber(0, icount, iens + 1);
						if (iens == save_ens)
							QuickSetBackColor(0,icount,m_ColorChanged);
						else
							QuickSetBackColor(0, icount, m_NoColor);

						QuickSetNumber(1, icount, i + 1	);
						QuickSetText(2, icount, test);
						test = PerformanceVar;
						if (m_pSimDoc->m_MStorage.m_Valid_TSV[iens].VarTranformed != 0) {
							test += " - ";
							test+= m_pSimDoc->m_MStorage.GetTransformationIndicator(iens);
						}
						QuickSetText(4, icount, test);
						if (split_ind == 0)
							QuickSetText(3, icount, "");
						else
							QuickSetNumber(3, icount, split_ind);

						if (criteria == 1)
							QuickSetNumber(5, icount, double(m_pSimDoc->m_MStorage.GetValidLow(index, split_ind)));
						else if (criteria == 2)
							QuickSetNumber(6, icount, double(m_pSimDoc->m_MStorage.GetValidHigh(index, split_ind)));
						else if (criteria == 3) {
							QuickSetNumber(5, icount, double(m_pSimDoc->m_MStorage.GetValidLow(index, split_ind)));
							QuickSetNumber(6, icount, double(m_pSimDoc->m_MStorage.GetValidHigh(index, split_ind)));
						}
						icount++;
						QuickSetNumber(-1, icount - 1, icount);
					}
				}*/
	}
	m_pSimDoc->m_MStorage.m_ActualEnsemble = save_ens;
	/*double MeanLow, MeanHigh; Need to be part of histindex;
	if(MeanCriteria>0) {
		test="Mean of all Variables";
		//if(def.VarIndex==5) test = "Sum of all Variables";


		MeanLow=double(m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_LOW][var]);
		MeanHigh=double(m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_HIGH][var]);
				


		if(MeanCriteria==1)
			QuickSetNumber(5,icount,MeanLow);
		else if(MeanCriteria==2)
			QuickSetNumber(6,icount,MeanHigh);
		else if(MeanCriteria==3) {
			QuickSetNumber(5,icount,MeanLow);
			QuickSetNumber(6,icount,MeanHigh);
		}
		QuickSetText(1,icount,test);
		QuickSetText(4,icount,PerformanceVar);
		icount++;
		QuickSetNumber(-1, icount-1, icount);
	}*/


}


for(int var=0;var<7;var++) {
		MeanCriteria= m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].criteria[var];
		switch( var) {
		case 0:
			PerformanceVar = "R2";
			break;
		case 1:
			PerformanceVar = "InterCept";
			break;
		case 2:
			PerformanceVar = "Slope";
			break;
			case 3:
				PerformanceVar="ME";
				break;
			case 4:
				PerformanceVar="RMSE";
				break;
			case 5:
				PerformanceVar="LogLi";
				break;
			case 6:
				PerformanceVar = "NSE R2";
				break;

		}

		for(int i=0;i<NumLinesSV;i++) {
			int index;
			size_t SV_index = var*m_pSimDoc->GetNumberofSumVariables() + i;
			criteria=m_pSimDoc->m_MStorage.GetSumCriteria(SV_index);
			if(criteria>0) {
				test=m_pSimDoc->m_ValidationData.GetValSumVarName(i);
				test+=" - ";
				test+=m_pSimDoc->m_ValidationData.GetStringValSumVarOutputType(i);
				QuickSetNumber(0, icount, m_pSimDoc->m_MStorage.m_ActualEnsemble+1);

				QuickSetNumber(1,icount, i+1);
				QuickSetText(2,icount, test);
				QuickSetText(4,icount, PerformanceVar);
				QuickSetText(3,icount,"");

				
				if(criteria==1) 
					QuickSetNumber(5,icount,double(m_pSimDoc->m_MStorage.m_Valid_SV[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_LOW][SV_index]));
				else if (criteria==2)
					QuickSetNumber(6,icount,double(m_pSimDoc->m_MStorage.m_Valid_SV[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_HIGH][SV_index]));
				else if (criteria==3) {
					QuickSetNumber(5,icount,double(m_pSimDoc->m_MStorage.m_Valid_SV[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_LOW][SV_index]));
					QuickSetNumber(6,icount,double(m_pSimDoc->m_MStorage.m_Valid_SV[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_HIGH][SV_index]));
				}
				icount++;
				QuickSetNumber(-1, icount-1, icount);
			}
		}
	double MeanLow, MeanHigh;
	if(MeanCriteria>0) {
		test="Mean of all SV Variables";
		MeanLow=double(m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[SELECT_LOW][var]);
		MeanHigh=double(m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[SELECT_HIGH][var]);

		if(MeanCriteria==1)
			QuickSetNumber(5,icount,MeanLow);
		else if(MeanCriteria==2)
			QuickSetNumber(6,icount,MeanHigh);
		else if(MeanCriteria==3) {
			QuickSetNumber(5,icount,MeanLow);
			QuickSetNumber(6,icount,MeanHigh);
		}
		QuickSetNumber(0, icount, m_pSimDoc->m_MStorage.m_ActualEnsemble + 1);
		QuickSetText(2,icount,test);
		QuickSetText(4,icount,PerformanceVar);
		icount++;
		QuickSetNumber(-1, icount-1, icount);
	}


	}



}

void ValidationCug::Make_SplitPeriodSheet()
{

	int cols = 5,rows=2;
	CUGCell cell;
	SetSheetNumber(12);
	//m_pDlg->SetWindowText("Split the simulations period into sub periods");
//	m_pDlg->OnInitDialog();

	SetDefFont(&m_font);
	//SetMultiSelectMode ( TRUE );


	
	EnableUpdate(FALSE); 
	while(GetNumberRows()>0)
		DeleteRow(0);
	EnableUpdate(TRUE);
	EnableMenu(TRUE); 
	
	SetNumberCols ( cols );
	
	SetNumberRows ( rows+2+m_pSimDoc->m_MStorage.m_AcceptedRuns.size() );
	SetColWidth(0,250);
	SetColWidth(1,160);
	SetColWidth(2, 80);
	SetColWidth(3, 100);
	SetColWidth(4, 180);
	SetColWidth(-1,0);
	QuickSetText(0,-1,"Item");
	QuickSetText(1,-1,"Number");
	QuickSetText(0,0,"Current Number of Sub Periods");
	QuickSetText(0,1,"New Number of Sub Periods");
	QuickSetNumber(1,0,m_SplitPeriods);
	QuickSetNumber(1,1,m_NumberNewSplitPeriods,false);
	QuickSetText(0, rows, "Actual Ensemble");
	QuickSetBackColor(0, rows, m_ColorHeading);

	string optionlist;
	
	QuickSetText(2, 0, "Ensemble Time Charts options");
	QuickSetBackColor(2, 0, m_ColorHeading);
	GetCell(2, 1, &cell);
	cell.SetCellType(UGCT_DROPLIST);
	optionlist = "Show One Selected Ensemble \n";
	optionlist += "Show All marked Ensembles \n";
	optionlist += "Show All Ensemble \n";
	m_pUtil->m_EnsembleViewOption = m_pUtil->GetEnsembleViewOption();
	string selected_str = FUtil::GetOptionString(optionlist, m_pUtil->m_EnsembleViewOption);

	cell.SetText(selected_str.c_str());
	cell.SetLabelText(optionlist.c_str());

	SetCell(2, 1, &cell);

	GetCell(2, 2, &cell);
	string txt="Update Chart";
	cell.SetText(txt.c_str());
	cell.SetBackColor(m_ColorClick);
	SetCell(2, 2, &cell);


	JoinCells(2, 0, 3, 0);
	JoinCells(2, 1, 3, 1);
	JoinCells(2, 2, 3, 2);
	GetCell(1, rows, &cell);
	cell.SetCellType(UGCT_DROPLIST);
	optionlist = "";

	for (size_t i = 0; i < m_pSimDoc->m_MStorage.m_AcceptedIdString.size(); i++) 
		optionlist += m_pSimDoc->m_MStorage.m_AcceptedIdString[i] + '\n';
	cell.SetLabelText(optionlist.c_str());
	if(m_pSimDoc->m_MStorage.m_AcceptedIdString.size()>m_pSimDoc->m_MStorage.m_ActualEnsemble)
		cell.SetText(m_pSimDoc->m_MStorage.m_AcceptedIdString[m_pSimDoc->m_MStorage.m_ActualEnsemble]);
	SetCell(1, rows, &cell);
	QuickSetText(0, rows + 1, "Name");
	QuickSetText(1, rows + 1, "No");
	QuickSetText(2, rows + 1, "Number of Accepted");
	QuickSetText(3, rows + 1, "To be Plotted");
	QuickSetText(4, rows + 1, "Performance Indicators");
	QuickSetBackColor(0, rows+1, m_ColorHeading);
	QuickSetBackColor(1, rows + 1, m_ColorHeading);
	QuickSetBackColor(2, rows + 1, m_ColorHeading);
	QuickSetBackColor(3, rows + 1, m_ColorHeading);
	QuickSetBackColor(4, rows + 1, m_ColorHeading);


	for (size_t i = 0; i < m_pSimDoc->m_MStorage.m_AcceptedIdString.size(); i++) {
		if(i== m_pSimDoc->m_MStorage.m_ActualEnsemble)
			QuickSetBackColor(0, rows+2+i , m_ColorChanged);
		else
			QuickSetBackColor(0, rows + 2 + i, m_NoColor);

		QuickSetText(0, rows + 2+i, m_pSimDoc->m_MStorage.m_AcceptedIdString[i], false);
		QuickSetNumber(1, rows + 2+ i, i+1);
		QuickSetNumber(2, rows + 2+ i, m_pSimDoc->m_MStorage.m_AcceptedRuns[i].size());

		GetCell(3, rows + 2 + i, &cell);
		cell.SetCellType(UGCT_CHECKBOX);
		if (m_pSimDoc->m_MStorage.IsToBePlotted(i))
			cell.SetBool(true);
		else
			cell.SetBool(false);
		SetCell(3, rows + 2 + i, &cell);
		QuickSetText(4, rows + 2 + i, m_pSimDoc->m_MStorage.GetTransformationIndicator(i) );
	}

	if(m_NumberNewSplitPeriods==5) QuickSetBackColor(1,1,m_ColorEdit);
	
}

void ValidationCug::Make_OptionSheet()
{

	m_col=6;
	int cols = m_col,
	rows = m_row;
	CUGCell cell;

	SetDefFont(&m_font);

	EnableUpdate(FALSE); 
	while(GetNumberRows()>0)
		DeleteRow(0);
	EnableMenu(TRUE);
	EnableUpdate(TRUE); 

	SetSheetNumber(9);
	//m_pDlg->SetWindowText("Assumptions and parameters used in log Likelihood values");
	//m_pDlg->OnInitDialog();



	SimB *pSw;
	P *pCPt=nullptr;

	
	int nval, n;
	CString txt;
	CString optionlist;

	Type="Switches";
	Group="Technical";
	Name="Likelihood Function";
	pSw=m_pSimDoc->GetPtr(Type, Group, Name);
	m_pSw1=(Sw*)pSw;
	Name="Box-Cox Tranformation";	
	pSw=m_pSimDoc->GetPtr(Type, Group, Name);
	m_pSw2=(Sw*)pSw;

	nval=m_pSw2->GetIntValue();
	if(nval==0) 
		cols=8;
	else
		cols=10;

	m_newcolst=cols-4;
	
	txt=m_pSw2->GetOption(nval).c_str();




	SetNumberCols ( cols );

	int NumLines=0;
	F* pValFile;

	m_NumValidVar=m_pSimDoc->GetNumAll_TSV_ValVariables();
	NumLines=m_pSimDoc->GetNumAll_TSV_ValVariables();
	int NumLinesSV=m_pSimDoc->m_ValidationData.GetNumSumVarVariables();
	int TotLines=m_NumValidVar;


	if(NumLinesSV>0&&m_NumValidVar>0)
		TotLines=TotLines+1+NumLinesSV;
	else if(NumLinesSV>0)
		TotLines=NumLinesSV;
	
	SetNumberRows(TotLines+3);

	if(m_pNewLogLi==nullptr) {
		long numrec;
		numrec=0;

			for(int i=0;i<MAXSIMVAL;i++) {
				pValFile=m_pSimDoc->ValidationFilePointer(i+1);
				if(pValFile->GetNumberOfSelectedVal()>0) {
					CPG *pPG;
					pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(0, i+1, TRUE);
					if(pPG!=nullptr) {
						pPG->ReOpen();

						if(pPG->GetNumRecords()>numrec)
							numrec=pPG->GetNumRecords();
						pPG->CloseFile();
					}
				}
			}

		
		m_pNewLogLi=new float[numrec];

	}

	GetColDefault(0,&cell);
	cell.SetAlignment(UG_ALIGNRIGHT);
	for(int i=0;i<cols;i++) {
		SetColDefault(i,&cell);
	}
//	SetMultiSelectMode ( TRUE );

	SetVScrollMode ( UG_SCROLLTRACKING );

	SetUniformRowHeight( TRUE );


	SetTH_NumberRows(3); 




	for(int i=0; i<(TotLines+3);i++)
		for (int j=0; j<cols;j++)
			UnJoinCells(j,i);

	for (int j=0; j<cols;j++) {
		//UnJoinCells(j,-3);
		//UnJoinCells(j,-2);
		UnJoinCells(j,-1);
	}


	SetTH_Height(75); 
	SetTH_RowHeight(-3,25);
	SetTH_RowHeight(-2,25);
	SetTH_RowHeight(-1,25);

	SetColWidth(-1,30);
	SetColWidth(0,250);
	for(int i=1;i<cols;i++) 
		SetColWidth(i,62);

	QuickSetText(0,-3,"Variable Name");
	QuickSetText(1,-3,"n");
	QuickSetText(2,-3,"Original Value");
	QuickSetText(m_newcolst,-3,"New Value");



	long iaccept=m_pSimDoc->m_MStorage.GetNumAccepted();
	long iacceptall=m_pSimDoc->m_MStorage.GetNumberOfRuns();


	QuickSetText(2,-2,"Error Parameters");
	QuickSetText(2,-1,"Relative");
	QuickSetText(3,-1,"Absolute");

	QuickSetText(m_newcolst,-2,"Error Parameters");
//	QuickSetText(newcolst,-1,"Relative");
//	QuickSetText(newcolst+1,-1,"Absolute");

	QuickSetText(m_newcolst,-1,"Relative");
	QuickSetText(m_newcolst+1,-1,"Absolute");	

	QuickSetText(m_newcolst+2,-2,"Box-Cox Parameters");
	QuickSetText(m_newcolst+2,-1,"power");
	QuickSetText(m_newcolst+3,-1,"offset");




	JoinCells(0,-3,0,-1);
	JoinCells(1,-3,1,-1);
	JoinCells(2,-2,3,-2);
	JoinCells(m_newcolst,-2,m_newcolst+1,-2);
	JoinCells(m_newcolst+2,-2,m_newcolst+3,-2);
	JoinCells(2,-3,m_newcolst-1,-3);
	JoinCells(m_newcolst,-3,m_newcolst+3,-3);

	if(m_newcolst>4) {
		JoinCells(4,-2,5,-2);
		QuickSetText(4,-2,"Box-Cox Parameters");
		QuickSetText(4,-1,"power");
		QuickSetText(5,-1,"offset");
	}
	else {

	}





	Type="Tables";
	Group="Technical";
	Name="Scaling of Observed Variables";

	pSw=m_pSimDoc->GetPtr(Group,Type, Name);
	if(pSw!=nullptr&&NumLines>0) {
			int oldnelements=((Tab*)pSw)->GetNumOfRow();
			if (NumLines!=oldnelements)  {
	  			((Tab*)pSw)->SetNumOfRow(NumLines);
	  			//Sw->Apply_CE_only();
				//m_pSimDoc->ReallocateTheWholeMap();
			}
			pCPt=((Tab*)pSw)->GetP_Pointer(0);
	}



	m_NumLines=NumLines;


	
	JoinCells(1, m_NumValidVar,m_newcolst-1, m_NumValidVar);
	JoinCells(m_newcolst, NumLines,m_newcolst+3, NumLines);
	
	
	
	JoinCells(1, NumLines+1,m_newcolst-1, NumLines+1);
	JoinCells(m_newcolst,NumLines+1,m_newcolst+3, NumLines+1);
	
	JoinCells(1, NumLines+2,m_newcolst-1, NumLines+2);
	JoinCells(m_newcolst,NumLines+2,m_newcolst+3, NumLines+2);
	
	QuickSetText(0,NumLines,"Assumptions");
	QuickSetText(1,NumLines,"Original Value");
	QuickSetText(m_newcolst,NumLines,"New Value");
	QuickSetText(0,NumLines+1,"Likelihoodfunction");
	QuickSetText(0,NumLines+2,"Box-Cox Transformation");
	
	QuickSetBackColor(0, NumLines, m_ColorHeading);
	QuickSetBackColor(1, NumLines, m_ColorHeading);
	QuickSetBackColor(m_newcolst, NumLines, m_ColorHeading);

	Type="Switches";
	Group="Technical";
	Name="Likelihood Function";
	pSw=m_pSimDoc->GetPtr(Type, Group, Name);
	m_pSw1=(Sw*)pSw;
	
	nval=m_pSw1->GetIntValue();

	m_LogFunc=m_pSimDoc->m_ValidationData.GetValFuncNew();

	txt=m_pSw1->GetOption(nval).c_str();
	n=m_pSw1->GetNumOptions();
	for(int j=0;j<n;j++) {
		optionlist+=m_pSw1->GetOption(j).c_str();
		optionlist+="\n";
	}
	GetCell(1,NumLines+1, &cell); 
	cell.SetCellType(UGCT_DROPLIST); 
	cell.SetLabelText(optionlist);
	cell.SetText(txt);
	cell.SetReadOnly(TRUE);
	SetCell(1,NumLines+1,&cell);
	
	
	
	GetCell(m_newcolst,NumLines+1, &cell); 
	cell.SetCellType(UGCT_DROPLIST); 
	cell.SetLabelText(optionlist);
	txt=m_pSw1->GetOption(m_LogFunc).c_str();
	cell.SetText(txt);
	cell.SetReadOnly(FALSE);
	SetCell(m_newcolst,NumLines+1,&cell);

	Name="Box-Cox Tranformation";	
	pSw=m_pSimDoc->GetPtr(Type, Group, Name);
	m_pSw2=(Sw*)pSw;

	nval=m_pSw2->GetIntValue();
	
	txt=m_pSw2->GetOption(nval).c_str();
	n=m_pSw2->GetNumOptions();
	optionlist=_T("");
	for(int j=0;j<n;j++) {
		optionlist+=m_pSw2->GetOption(j).c_str();
		optionlist+="\n";
	}
	
	GetCell(1,NumLines+2, &cell); 
	cell.SetCellType(UGCT_DROPLIST); 
	cell.SetLabelText(optionlist);
	cell.SetText(txt);
	cell.SetReadOnly(TRUE);
	SetCell(1,NumLines+2,&cell);
	
	m_BoxCoxTrans=m_pSimDoc->m_ValidationData.GetValBoxCoxNew();

	txt=m_pSw2->GetOption(m_BoxCoxTrans).c_str();
	GetCell(m_newcolst,NumLines+2, &cell); 
	cell.SetCellType(UGCT_DROPLIST); 
	cell.SetLabelText(optionlist);
	cell.SetText(txt);
	cell.SetReadOnly(FALSE);
	SetCell(m_newcolst,NumLines+2,&cell);



	Type="Table Parameters";
	Name="Box Cox power";
	pSw=m_pSimDoc->GetPtr(Type, Group, Name);
	m_pCPt1=(P*)pSw;

	Name="Box Cox offset";
	pSw=m_pSimDoc->GetPtr(Type, Group, Name);
	m_pCPt2=(P*)pSw;

	Name="Abs Error";
	pSw=m_pSimDoc->GetPtr(Type, Group, Name);
	m_pCPtError2=(P*)pSw;


	Name="Rel Error";
	pSw=m_pSimDoc->GetPtr(Type, Group, Name);
	m_pCPtError1=(P*)pSw;

	for(int i=0;i<TotLines;i++) 
		QuickSetNumber(-1,i,i+1);

	string test;
	int FileNo, ind;

	for(int i=0;i<NumLines;i++) {
			test=FUtil::STD_ItoAscii(i+1);
			FileNo=m_pSimDoc->m_ValidationData.GetValFileNumber(i);
			pValFile=m_pSimDoc->ValidationFilePointer(FileNo);
			ind=m_pSimDoc->m_ValidationData.GetValFileIndex(i);
			CPG *pPG;
			BOOL valid;
			pPG=(CPG*)pValFile->GetPointer();
			if(pPG==nullptr) {
				pPG=new CPG;
				//CPG_Array.Add(pCPG);
				valid=pPG->ReadContentOfPGFileAndOpenMainPGStreamForReading(pValFile->GetStrValue().c_str());
				if(valid) pValFile->SetPointer(pPG);
				pPG->CloseFile();
			}
			test=pValFile->GetDataDescription(ind+1).c_str();
			GetCell(0,i,&cell);	
			cell.SetText(test);
			cell.SetReadOnly(TRUE);
			SetCell(0,i,&cell);

			float value, value1, value2;
			GetCell(1,i,&cell);	
			//value=((CCoupModelDoc*)m_pDoc)->GetValn(i);
			cell.SetNumber(m_pSimDoc->m_ValidationData.GetValn(i));
			cell.SetReadOnly(TRUE);

			SetCell(1,i,&cell);	
	
			GetCell(2,i,&cell);	
			value=m_pCPtError1->GetValue(i);
			cell.SetNumber(value);
			cell.SetReadOnly(TRUE);
			SetCell(2,i,&cell);
			
				
			GetCell(m_newcolst,i,&cell);
			float test=m_pSimDoc->m_ValidationData.GetValP_ErrorNew(i);
			if(test<-1.1e8) {
				m_pSimDoc->m_ValidationData.SetValP_ErrorNew(i, m_pCPtError1->GetValue(i) );
			}

			cell.SetNumber(m_pSimDoc->m_ValidationData.GetValP_ErrorNew(i));
			SetCell(m_newcolst,i,&cell);


			
			GetCell(3,i,&cell);	
			value=m_pCPtError2->GetValue(i);
			//value=((CCoupModelDoc*)m_pDoc)->GetValA_Error(i);
			cell.SetNumber(value);
			cell.SetReadOnly(TRUE);
			SetCell(3,i,&cell);	
			
			
			GetCell(m_newcolst+1,i,&cell);

			test=m_pSimDoc->m_ValidationData.GetValA_ErrorNew(i);
			if(test<-1.1e8) {
				m_pSimDoc->m_ValidationData.SetValA_ErrorNew(i, m_pCPtError2->GetValue(i) );
			}



			cell.SetNumber(m_pSimDoc->m_ValidationData.GetValA_ErrorNew(i));
			SetCell(m_newcolst+1,i,&cell);	
			
			if(m_newcolst>4) {
				GetCell(4,i,&cell);	
				value1=m_pCPt1->GetValue(i);
				cell.SetNumber(value1);
				cell.SetReadOnly(TRUE);
				SetCell(4,i,&cell);	

				GetCell(5,i,&cell);	
				value2=m_pCPt2->GetValue(i);
				cell.SetNumber(value2);
				cell.SetReadOnly(TRUE);
				SetCell(5,i,&cell);	
			}
			else {
				GetCell(m_newcolst+2,i,&cell);	
				value1=m_pCPt1->GetValue(i);
				cell.SetNumber(value1);
				SetCell(m_newcolst+2,i,&cell);	

				GetCell(m_newcolst+3,i,&cell);	
				value2=m_pCPt2->GetValue(i);
				cell.SetNumber(value2);
				SetCell(m_newcolst+3,i,&cell);	
			}

			GetCell(m_newcolst+2,i,&cell);	
			test=m_pSimDoc->m_ValidationData.GetValBoxCox_PowerNew(i);
			if(test<-1.1e8) {
				m_pSimDoc->m_ValidationData.SetValBoxCox_PowerNew(i, m_pCPt1->GetValue(i) );
			}

			cell.SetNumber(m_pSimDoc->m_ValidationData.GetValBoxCox_PowerNew(i));
			SetCell(m_newcolst+2,i,&cell);	

			GetCell(m_newcolst+3,i,&cell);	

			test=m_pSimDoc->m_ValidationData.GetValBoxCox_OffsetNew(i);
			if(test<-1.1e8) {
				m_pSimDoc->m_ValidationData.SetValBoxCox_OffsetNew(i, m_pCPt2->GetValue(i) );
			}

			cell.SetNumber(m_pSimDoc->m_ValidationData.GetValBoxCox_OffsetNew(i));
			SetCell(m_newcolst+3,i,&cell);


	}

	RedrawAll();


}
void ValidationCug::Make_DistributionSheet(BOOL Residuals)
{

	
	int cols = 15,
	rows = m_row;
	CUGCell cell;
	BOOL FileOpen;

	SetDefFont(&m_font);

	EnableUpdate(FALSE); 
	while(GetNumberRows()>0)
		DeleteRow(0);
	EnableMenu(TRUE);
	EnableUpdate(TRUE); 

	if(Residuals) {
		SetSheetNumber(11);
	//	m_pDlg->SetWindowText("Residuals between simulated and observed Variables");
	//	m_pDlg->OnInitDialog();
	}
	else {
		SetSheetNumber(10);
	//	m_pDlg->SetWindowText("Log Likelihood values based on new settings");
		//m_pDlg->OnInitDialog();
	}



	
	SetNumberCols ( cols );

	int NumLines=0;
	F* pValFile;
	CResiduals *pRes;
	FileOpen=FALSE;
	for(int i=0;i<MAXSIMVAL;i++) {
		pValFile=m_pSimDoc->ValidationFilePointer(i+1);
		NumLines+=pValFile->GetNumberOfSelectedVal();
		pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(i+1);
		if(pRes!=nullptr) {
			if(pRes->IsExtraLogLiFile())
				FileOpen=TRUE;
		}
	}
	NumLines=m_pSimDoc->GetNumAll_TSV_ValVariables();
	int NumAllVariables=NumLines;


	int NumLinesSV=m_pSimDoc->m_ValidationData.GetNumSumVarVariables();
	
	int TotLines=NumLines;
	if(NumLinesSV>0&&NumLines>0)
		TotLines=TotLines+2+NumLinesSV;
	else if(NumLinesSV>0)
		TotLines=NumLinesSV;
	
	if(!FileOpen) return;
	long RunsToShow=1;
	long iaccept=m_pSimDoc->m_MStorage.GetNumAccepted();
	long iacceptall=m_pSimDoc->m_MStorage.GetNumberOfRuns();

	if(m_SelectedTimeShow==1)
		RunsToShow=iaccept;
	else if(m_SelectedTimeShow==2)
		RunsToShow=iacceptall;
	else if(m_SelectedTimeShow%2==0)
		RunsToShow=7;

	if(m_SelectedVarShow==0)
		NumLines=1;
	TotLines=NumLines*RunsToShow;
	SetNumberRows(TotLines);

	

	GetColDefault(0,&cell);
	cell.SetAlignment(UG_ALIGNRIGHT);
	for(int i=0;i<cols;i++) {
		SetColDefault(i,&cell);
	}
//	SetMultiSelectMode ( TRUE );

	SetVScrollMode ( UG_SCROLLTRACKING );

	SetUniformRowHeight( TRUE );


	SetTH_NumberRows(2); 

	SetTH_Height(50); 

	SetTH_RowHeight(-2,25);
	SetTH_RowHeight(-1,25);
	UnJoinCells(0,-2);
	UnJoinCells(0,-1);
	for(int i=1;i<6;i++) {
		JoinCells(i,-2,i,-1);
	}
	SetSH_NumberCols(2);
	SetSH_ColWidth(-2,0);
	SetSH_ColWidth(-1,90);
	SetSH_Width(90);
		
	JoinCells(6,-2,8,-2);
	JoinCells(9,-2,11,-2);
	JoinCells(12,-2,14,-2);


	//SetColWidth(-1,70);
	SetColWidth(0,170);
	SetColWidth(1,40);
	for(int i=2;i<cols;i++) 
		SetColWidth(i,60);

	string domain;
	

	QuickSetText(0,-1,"Variable Name");
	QuickSetText(1,-2,"n");
	QuickSetText(2,-2,"Mean");
	QuickSetText(3,-2,"St.Dev");
	QuickSetText(4,-2,"Min");
	QuickSetText(5,-2,"Max");
	GetCell(-1,-1,&cell);
	cell.SetCellTypeEx(UGCT_NORMALMULTILINE);
	cell.SetBackColor(m_ColorClick);
	string str;
	if(m_SelectedTimeShow==0) {
		str="Individual\nAccepted Run\n";
		str+=FUtil::STD_ItoAscii(m_SelectedTimeIndCount%iaccept+1);
		str+="( "+FUtil::STD_ItoAscii(iaccept)+")";
	}
	else if(m_SelectedTimeShow==1)
		str="Accepted\n("+FUtil::STD_ItoAscii(iaccept)+")";
	else if(m_SelectedTimeShow==2)
		str="All Runs\n("+FUtil::STD_ItoAscii(iacceptall)+")";
	else if(m_SelectedTimeShow<5)
		str="Accepted\n("+FUtil::STD_ItoAscii(iaccept)+")";
	else if(m_SelectedTimeShow<7)
		str="All Runs\n("+FUtil::STD_ItoAscii(iacceptall)+")";
	cell.SetText(str);
	
	SetCell(-1,-1,&cell);


	QuickSetText(6,-2,"Empirical Quantiles");
	QuickSetText(9,-2,"Normal Dist Quantiles");
	QuickSetText(12,-2,"Difference (Emp-Nor)");

	for(int i=0;i<3;i++) {
		QuickSetText(6+i*3,-1,"5");
		QuickSetText(7+i*3,-1,"50");
		QuickSetText(8+i*3,-1,"95");
	}

	string test;
	int FileNo, ind;
	
	long Id_run;
	long irow;
	irow=-1;
	int Id_var;
	if(m_SelectedVarShow==0) {
		test="One Variable "+FUtil::STD_ItoAscii(m_SelectedVarIndCount%NumAllVariables+1)+" ("+FUtil::STD_ItoAscii(NumAllVariables)+")";
		QuickSetBackColor(0,-2,m_ColorClick);
	}
	else
		test="All Variables ("+FUtil::STD_ItoAscii(NumAllVariables)+")";
	QuickSetText(0,-2,test);
	for(int i=0;i<NumLines;i++) {
		Id_var=i;
		if(m_SelectedVarShow==0)
			Id_var=m_SelectedVarIndCount%NumAllVariables;
		test=FUtil::STD_ItoAscii(Id_var+1);
		QuickSetText(-2,irow+1,test);
		FileNo=m_pSimDoc->m_ValidationData.GetValFileNumber(Id_var);
		pValFile=m_pSimDoc->ValidationFilePointer(FileNo);
		pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(FileNo);
		ind=m_pSimDoc->m_ValidationData.GetValFileIndex(Id_var);
		CPG *pPG;
		BOOL valid;
		pPG=(CPG*)pValFile->GetPointer();
		if(pPG==nullptr) {
			pPG=new CPG;
			//CPG_Array.Add(pCPG);
			valid=pPG->ReadContentOfPGFileAndOpenMainPGStreamForReading(pValFile->GetStrValue().c_str());
			if(valid) pValFile->SetPointer(pPG);
			pPG->CloseFile();
		}
		test=pValFile->GetDataDescription(ind).c_str();
		GetCell(0,irow+1,&cell);	
		cell.SetText(test);
		cell.SetReadOnly(TRUE);
		SetCell(0,irow+1,&cell);

		JoinCells(0,irow+1,0,irow+RunsToShow);
		JoinCells(1,irow+1,1,irow+RunsToShow);
		
		for(long jruns=0; jruns<RunsToShow; jruns++) {

			if(m_SelectedTimeShow==1) {
				Id_run=m_pSimDoc->m_MStorage.Get_AcceptedRuns(jruns);
			}
			else if(m_SelectedTimeShow==2) {
				Id_run=jruns;
			}
			else if(m_SelectedTimeShow==0) {
				Id_run=m_pSimDoc->m_MStorage.Get_AcceptedRuns(m_SelectedTimeIndCount%iaccept);
			}
			else if(m_SelectedTimeShow%2==0)
				m_SelectedTimeIndCount=jruns;

			int ifile=m_pSimDoc->MR_GetAddedFiles(Id_run);
			if(ifile!=0) 
			{
				pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(FileNo, ifile);
				Id_run=m_pSimDoc->MR_GetInternalRunNo(Id_run);
			}

			GetCell(-1,irow+1,&cell);
			if(m_SelectedTimeShow<=2) {
				domain="Run No :";
				domain+=FUtil::STD_ItoAscii(Id_run+1);
			}
			else if(m_SelectedTimeShow<7) {
				domain="";
				if(m_SelectedTimeShow>4)
					domain="";
				string Add;
				switch(m_SelectedTimeIndCount%7) {
					case 0:
						Add="Mean";
						break;
					case 1:
						Add="StD";
						break;
					case 2:
						Add="Min";
						break;
					case 3:
						Add="Max";
						break;
					case 4:
						Add="Q5";
						break;
					case 5:
						Add="Q50";
						break;
					case 6:
						Add="Q95";
						break;
				}
				domain+=Add;
			}
			cell.SetText(domain);
			cell.SetCellTypeEx(UGCT_NORMALMULTILINE);
			cell.SetParam(Id_var);
			SetCell(-1,irow+1  ,&cell);

			irow++;
	

			float value, value1, value2, valuem;
			GetCell(1,irow,&cell);	
			cell.SetNumber(m_pSimDoc->m_ValidationData.GetValn(Id_var));
			cell.SetReadOnly(TRUE);

			SetCell(1,irow,&cell);	

			GetCell(2,irow,&cell);	
			if(m_SelectedTimeShow<3) 
				valuem=pRes->GetLogLiMean(Id_run,ind, Residuals);
			else if(m_SelectedTimeShow<5&&!Residuals)
				valuem=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,0, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<5)
				valuem=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,0, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<7&&!Residuals)
				valuem=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,0, m_SelectedTimeIndCount%7, TRUE);
			else if(m_SelectedTimeShow<7)
				valuem=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,0, m_SelectedTimeIndCount%7, TRUE);

			cell.SetNumber(valuem);
			SetCell(2,irow,&cell);



			GetCell(3,irow,&cell);	
			if(m_SelectedTimeShow<3) 
				value=pRes->GetLogLiCV(Id_run,ind, Residuals);
			else if(m_SelectedTimeShow<5&&!Residuals)
				value=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,1, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<5)
				value=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,1, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<7&&!Residuals)
				value=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,1, m_SelectedTimeIndCount%7, TRUE);
			else if(m_SelectedTimeShow<7)
				value=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,1, m_SelectedTimeIndCount%7, TRUE);

			cell.SetNumber(value);
			SetCell(3,irow,&cell);
				

			GetCell(4,irow,&cell);	
			if(m_SelectedTimeShow<3) 
				value=pRes->GetLogLiPMIN(Id_run,ind, Residuals);
			else if(m_SelectedTimeShow<5&&!Residuals)
				value=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,2, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<5)
				value=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,2, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<7&&!Residuals)
				value=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,2, m_SelectedTimeIndCount%7, TRUE);
			else if(m_SelectedTimeShow<7)
				value=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,2, m_SelectedTimeIndCount%7, TRUE);

			cell.SetNumber(value);
			SetCell(4,irow,&cell);


			GetCell(5,irow,&cell);	
			if(m_SelectedTimeShow<3) 
				value=pRes->GetLogLiPMAX(Id_run,ind, Residuals);
			else if(m_SelectedTimeShow<5&&!Residuals)
				value=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,3, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<5)
				value=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,3, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<7&&!Residuals)
				value=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,3, m_SelectedTimeIndCount%7, TRUE);
			else if(m_SelectedTimeShow<7)
				value=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,3, m_SelectedTimeIndCount%7, TRUE);

			cell.SetNumber(value);
			SetCell(5,irow,&cell);

			GetCell(6,irow,&cell);	
			if(m_SelectedTimeShow<3) 
				value1=pRes->GetLogLiP5(Id_run,ind, Residuals);
			else if(m_SelectedTimeShow<5&&!Residuals)
				value1=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,4, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<5)
				value1=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,4, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<7&&!Residuals)
				value1=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,4, m_SelectedTimeIndCount%7, TRUE);
			else if(m_SelectedTimeShow<7)
				value1=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,4, m_SelectedTimeIndCount%7, TRUE);

			cell.SetNumber(value1);
			SetCell(6,irow,&cell);



			GetCell(7,irow,&cell);	
			if(m_SelectedTimeShow<3) 
				valuem=pRes->GetLogLiP50(Id_run,ind, Residuals);
			else if(m_SelectedTimeShow<5&&!Residuals)
				valuem=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,5, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<5)
				valuem=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,5, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<7&&!Residuals)
				valuem=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,5, m_SelectedTimeIndCount%7, TRUE);
			else if(m_SelectedTimeShow<7)
				valuem=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,5, m_SelectedTimeIndCount%7, TRUE);
			cell.SetNumber(valuem);
			SetCell(7,irow,&cell);

			GetCell(8,irow,&cell);	
			if(m_SelectedTimeShow<3) 
				value2=pRes->GetLogLiP95(Id_run,ind, Residuals);
			else if(m_SelectedTimeShow<5&&!Residuals)
				value2=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,6, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<5)
				value2=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,6, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<7&&!Residuals)
				value2=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,6, m_SelectedTimeIndCount%7, TRUE);
			else if(m_SelectedTimeShow<7)
				value2=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,6, m_SelectedTimeIndCount%7, TRUE);			

			cell.SetNumber(value2);
			SetCell(8,irow,&cell);

			GetCell(9,irow,&cell);	
			if(m_SelectedTimeShow<3)
				value=pRes->GetZ_ScoreLogLiP5(Id_run,ind, Residuals);
			else if(m_SelectedTimeShow<5&&!Residuals)
				value=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,7, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<5)
				value=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,7, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<7&&!Residuals)
				value=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,7, m_SelectedTimeIndCount%7, TRUE);
			else if(m_SelectedTimeShow<7)
				value=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,7, m_SelectedTimeIndCount%7, TRUE);			
			cell.SetNumber(value);
			SetCell(9,irow,&cell);
			
			GetCell(12,irow,&cell);	
			if(m_SelectedTimeShow<3)
				value1-=value;
			else if(m_SelectedTimeShow<5&&!Residuals)
				value1=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,10, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<5)
				value1=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,10, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<7&&!Residuals)
				value1=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,10, m_SelectedTimeIndCount%7, TRUE);
			else if(m_SelectedTimeShow<7)
				value1=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,10, m_SelectedTimeIndCount%7, TRUE);	
			cell.SetNumber(value1);
			SetCell(12,irow,&cell);
			
			GetCell(10,irow,&cell);
			if(m_SelectedTimeShow<3)
				value=pRes->GetLogLiMean(Id_run,ind, Residuals);
			else if(m_SelectedTimeShow<5&&!Residuals)
				value=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,8, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<5)
				value=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,8, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<7&&!Residuals)
				value=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,8, m_SelectedTimeIndCount%7, TRUE);
			else if(m_SelectedTimeShow<7)
				value=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,8, m_SelectedTimeIndCount%7, TRUE);	
			cell.SetNumber(value);
			SetCell(10,irow,&cell);

			
			GetCell(13,irow,&cell);	
			if(m_SelectedTimeShow<3) 
				valuem-=value;
			else if(m_SelectedTimeShow<5&&!Residuals)
				valuem=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,11, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<5)
				valuem=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,11, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<7&&!Residuals)
				valuem=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,11, m_SelectedTimeIndCount%7, TRUE);
			else if(m_SelectedTimeShow<7)
				valuem=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,11, m_SelectedTimeIndCount%7, TRUE);	
			cell.SetNumber(valuem);
			SetCell(13,irow,&cell);

			GetCell(11,irow,&cell);	
			if(m_SelectedTimeShow<3) 
				value=pRes->GetZ_ScoreLogLiP95(Id_run,ind, Residuals);
			else if(m_SelectedTimeShow<5&&!Residuals)
				value=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,9, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<5)
				value=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,9, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<7&&!Residuals)
				value=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,9, m_SelectedTimeIndCount%7, TRUE);
			else if(m_SelectedTimeShow<7)
				value=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,9, m_SelectedTimeIndCount%7, TRUE);	
			cell.SetNumber(value);
			SetCell(11,irow,&cell);
					
			GetCell(14,irow,&cell);	
			if(m_SelectedTimeShow<3)
				value2-=value;
			else if(m_SelectedTimeShow<5&&!Residuals)
				value2=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,12, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<5)
				value2=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,12, m_SelectedTimeIndCount%7, FALSE);
			else if(m_SelectedTimeShow<7&&!Residuals)
				value2=m_pSimDoc->m_MStorage.GetValidDynLogLi(Id_var,12, m_SelectedTimeIndCount%7, TRUE);
			else if(m_SelectedTimeShow<7)
				value2=m_pSimDoc->m_MStorage.GetValidDynResiduals(Id_var,12, m_SelectedTimeIndCount%7, TRUE);	
			cell.SetNumber(value2);

			SetCell(14,irow,&cell);

		
		}
	}
	RedrawAll();


}
void ValidationCug::OnSheetSetup(int sheetNumber)

{
	SetSheetNumber(sheetNumber);
	if (sheetNumber < 7) {
		if (sheetNumber == 0)
			SetLocalWindowText("Eval-Coefficients of Determination", WINDOW_EVAL);
		else if (sheetNumber == 1)
			SetLocalWindowText("Eval-Intercept", WINDOW_EVAL);
		else if (sheetNumber == 2)
			SetLocalWindowText("Eval-Slope", WINDOW_EVAL);
		else if (sheetNumber == 3)
			SetLocalWindowText("Eval-Mean Error", WINDOW_EVAL);
		else if (sheetNumber == 4)
			SetLocalWindowText("Eval Log Likelihood", WINDOW_EVAL);
		else if (sheetNumber == 5)
			SetLocalWindowText("Eval Nash-Sutcliffe R2", WINDOW_EVAL);
		else if (sheetNumber == 6)
			SetLocalWindowText("Eval - Criteria Outputs", WINDOW_EVAL);
		Make_MainSheets(sheetNumber);
		}
	else if (sheetNumber == 7) {
		SetLocalWindowText("Eval - Criteria on Output Stat", WINDOW_EVAL);
		Make_SumVarCriteriaSheet();

	}
	else if (sheetNumber == 8) {
		SetLocalWindowText("Eval - Defined Criteria", WINDOW_EVAL);
		Make_CriteriaSheet();
	}
	else if (sheetNumber == 9) {
		SetLocalWindowText("Eval - Log Likelihood Options", WINDOW_EVAL);

		Make_OptionSheet();
	}
				else if(sheetNumber==10) {
					SetLocalWindowText("Eval - Log Likelihood statistics", WINDOW_EVAL);

					Make_DistributionSheet(FALSE);
				}
				else if (sheetNumber == 11) {
					SetLocalWindowText("Eval - Residual Statistics", WINDOW_EVAL);
					Make_DistributionSheet(TRUE);
				}
				else if (sheetNumber == 12) {
					SetLocalWindowText("Eval - Ensembles", WINDOW_EVAL);
					Make_SplitPeriodSheet();
				}
				
				RedrawAll();	
}
void ValidationCug::Make_SumVarCriteriaSheet()
{


	int cols = 5;
	int rows = 0;
	CUGCell cell;

	SetDefFont(&m_font);

	EnableUpdate(FALSE); 
	while(GetNumberRows()>0)
		DeleteRow(0);
	EnableMenu(TRUE);
	EnableUpdate(TRUE); 

	SetSheetNumber(7);
	//m_pDlg->SetWindowText("Criteria on Statistic of Summary of Output Variables");
//	m_pDlg->OnInitDialog();

	
	int nval, n;
	CString txt;
	CString optionlist;



	SetNumberCols ( 14 );

	auto NumLines=m_pSimDoc->m_MStorage.m_ValSumValues_Stored.size();


	
	SetNumberRows(NumLines);
	GetColDefault(0,&cell);
	cell.SetAlignment(UG_ALIGNRIGHT);
	for(int i=0;i<cols;i++) {
		if(m_Data||i>3)
		SetColDefault(i,&cell);
	}
//	SetMultiSelectMode ( TRUE );

	SetVScrollMode ( UG_SCROLLTRACKING );

	SetUniformRowHeight( TRUE );

	GetCell(1,-2,&cell);
	cell.SetCellTypeEx(UGCT_NORMALMULTILINE);
	SetCell(1,-2,&cell);
		SetTH_NumberRows(2); 
		SetTH_Height(50); 
		SetTH_RowHeight(-2,25);
		SetTH_RowHeight(-1,25);

		SetColWidth(-1,30);
		SetColWidth(0,100);
		SetColWidth(1,120);
		SetColWidth(2,60);
		for(int i=3;i<cols;i++) 
			SetColWidth(i,62);
	
		QuickSetText(0,-2,"Group");
		QuickSetText(1,-2,"Variable Name");
		QuickSetText(2,-2,"Stat");

		long iaccept=m_pSimDoc->m_MStorage.GetNumAccepted();
		long iacceptall=m_pSimDoc->m_MStorage.GetNumberOfRuns();


		QuickSetText(3,-2,"Distribution Prior Selection n="+FUtil::STD_ItoAscii(iacceptall));
		QuickSetText(7,-2,"Distribution Post Selection n="+FUtil::STD_ItoAscii(iaccept));
		if(iaccept!=iacceptall) QuickSetBackColor(5,-2, m_ColorEdit);
		QuickSetText(12,-2,"Limits for Selection");

		QuickSetText(3,-1,"Mean");
		QuickSetText(4,-1,"CV");
		QuickSetText(5,-1,"Min");
		QuickSetText(6,-1,"Max");
		QuickSetText(7,-1,"Mean");
		QuickSetText(8,-1,"CV");
		QuickSetText(9,-1,"Min");
		QuickSetText(10,-1,"Max");
		QuickSetText(11,-1,"Range");
		QuickSetText(12,-1,"Low");
		QuickSetText(13,-1,"High");
		JoinCells(0,-2,0,-1);
		JoinCells(1,-2,1,-1);
		JoinCells(2,-2,2,-1);
		JoinCells(3,-2,6,-2);
		JoinCells(7,-2,11,-2);
		JoinCells(12,-2,13,-2);

		if(m_pSimDoc->m_MStorage.m_PriorSumVar.size()!=m_pSimDoc->m_MStorage.m_ValSumValues_Stored.size()) {
			m_pSimDoc->m_MStorage.m_PriorSumVar.resize(NumLines);
			m_pSimDoc->m_MStorage.m_PostSumVar.resize(NumLines);
			m_pSimDoc->m_MStorage.Update_SV_VarStat(false);
			

		}

	VALSUM valsum;
	for(int i=0;i<NumLines;i++) {
		valsum=m_pSimDoc->m_MStorage.m_ValSumValues_Stored[i];
			auto group=valsum.simb.pBase->GetGroup();
			auto name=valsum.simb.pBase->GetName();
			if(valsum.simb.TabIndex>0) {
				name+="(";
				name+=FUtil::STD_ItoAscii(valsum.simb.TabIndex);
				name+=")";
			}
			auto index=valsum.SumIndex;
			QuickSetText(0,i,group.c_str());
			QuickSetText(1,i,name.c_str());
			switch(index%6) {
			case 0: txt="Initial"; break;
			case 1: txt="Final"; break;
			case 2: txt="Min"; break;
			case 3: txt="Max"; break;
			case 4: txt="Mean"; break;
			case 5: txt="Accumulated"; break;
			}
			QuickSetText(2,i, txt);
			QuickSetNumber(3,i,m_pSimDoc->m_MStorage.m_PriorSumVar[i].mean);
			QuickSetNumber(4,i,m_pSimDoc->m_MStorage.m_PriorSumVar[i].cv);
			QuickSetNumber(5,i,m_pSimDoc->m_MStorage.m_PriorSumVar[i].min);
			QuickSetNumber(6,i,m_pSimDoc->m_MStorage.m_PriorSumVar[i].max);
			QuickSetNumber(7,i,m_pSimDoc->m_MStorage.m_PostSumVar[i].mean);
			QuickSetNumber(8,i,m_pSimDoc->m_MStorage.m_PostSumVar[i].cv);
			QuickSetNumber(9,i,m_pSimDoc->m_MStorage.m_PostSumVar[i].min);
			QuickSetNumber(10,i,m_pSimDoc->m_MStorage.m_PostSumVar[i].max);
			float range;
			if(m_pSimDoc->m_MStorage.m_PriorSumVar[i].max>m_pSimDoc->m_MStorage.m_PriorSumVar[i].min) 
				range=(m_pSimDoc->m_MStorage.m_PostSumVar[i].max-m_pSimDoc->m_MStorage.m_PostSumVar[i].min)/
				(m_pSimDoc->m_MStorage.m_PriorSumVar[i].max-m_pSimDoc->m_MStorage.m_PriorSumVar[i].min);
			else
				range=1;
			QuickSetNumber(11,i, range);
			QuickSetNumber(12,i,valsum.ValueMin,false);
			QuickSetNumber(13,i,valsum.ValueMax,false);

	}
	
	RedrawAll();


}

/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the current cell in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int ValidationCug::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the top row or left column in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int ValidationCug::OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	
void ValidationCug::OnHitBottom(long numrows,long rowspast,long rowsfound)
{
}

/////////////////////////////////////////////////////////////////////////////
//	
void ValidationCug::OnHitTop(long numrows,long rowspast)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeCol
//		Sent when the user is over a separation line on the top heading
//		A return value of TRUE allows the possibiliy of a resize
int ValidationCug::OnCanSizeCol(int col)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSizing
//		Sent when the user is sizing a column
//		The column that is being sized is given as 
//		well as the width. Plus the width can be modified 
//		at this point. This makes it easy to set min and
//		max widths
void ValidationCug::OnColSizing(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSized
//		This is sent when the user finished sizing the 
//		given column (see above for more details)
void ValidationCug::OnColSized(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeRow
//		Sent when the user is over a separation line on the side heading
//		A return value of TRUE allows the possibiliy of a resize
int  ValidationCug::OnCanSizeRow(long row)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSizing
//	    Sent when the user is sizing a row The row that is being sized is given as 
//		well as the height. Plus the height can be modified 
//		at this point. This makes it easy to set min and max heights
void ValidationCug::OnRowSizing(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSized
//		This is sent when the user is finished sizing hte
//		given row ( see above for more details)
void ValidationCug::OnRowSized(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeSideHdg
//		This is sent when the user moves into position 
//		for sizing the width of the side heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int ValidationCug::OnCanSizeSideHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeTopHdg
//		This is sent when the user moves into position 
//		for sizing the height of the top heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int ValidationCug::OnCanSizeTopHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSizing
int ValidationCug::OnSideHdgSizing(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSized
int ValidationCug::OnSideHdgSized(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSized
int ValidationCug::OnTopHdgSized(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSizing
int ValidationCug::OnTopHdgSizing(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColChange
//		Sent whenever the current column changes
//		The old and the new columns are given
void ValidationCug::OnColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowChange
//		Sent whenever the current row changes
//		The old and the new rows are given
void ValidationCug::OnRowChange(long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellChange
//		Sent whenever the current cell changes rows or columns
void ValidationCug::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnLeftColChange
//		Sent whenever the left visible column in the grid changes
void ValidationCug::OnLeftColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopRowChange
//		Sent whenever the top visible row in the grid changes
void ValidationCug::OnTopRowChange(long oldrow,long newrow)
{
}

/***************************************************
OnSelectionChanged
	The OnSelectionChanged notification is called by the multiselect
	class when a change occured in current selection (ie user clicks
	on a new cell, drags a mouse selecting range of cells, or uses
	CTRL/SHIFT - left click key combination to select cells.)
Params:
	startCol, startRow	- coordinates of the cell starting the selection block
	endCol, endRow		- coordinates of the cell ending the selection block
	blockNum			- block number representing this range, this will
				2001-10-15 12:00		  always represent total number of selection blocks.
Return:
	<none>
****************************************************/
void ValidationCug::OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum)
{
	/*TRACE("ValidationCug::OnSelectionChanged called with startCol = %ld, startRow = %ld, endCol = %ld, endRow = %ld, blockNum = %ld.\n",
				startCol, startRow, endCol,	endRow, blockNum );*/
}

/////////////////////////////////////////////////////////////////////////////
//	OnLClicked
//		Sent whenever the user clicks the left mouse button within the grid
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' and 'row' are negative if the area clicked in is not a valid cell
//		'rect' the rectangle of the cell that was clicked in
//		'point' the point where the mouse was clicked
//		'updn'  TRUE if the button is down FALSE if the button just when up
void ValidationCug::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
	int isheet=GetSheetNumber();
	CUGCell cell;

	if (col == 1 && updn) {
		if (row < GetNumberRows()) {
			if (m_Distribution_As_Independent) {
				m_pProp->m_ChartType = CHART_TYPES::TIME_SERIE;
				m_pProp->SetFirstIndepSet(PARVAR, false);
				m_pProp->SetFirstIndepVarType(MULTIRUN_NUM);
				MultiPlotClick(isheet, col, row, true);
			}
			else {

				m_pProp->SetFirstIndepVarType(PARVAR);
				m_pProp->m_ChartType = CHART_TYPES::SCATTER;
				m_pProp->ResetXScale(string::npos);
				m_pProp->SetFirstIndepSet(PARVAR, true);
				MultiPlotClick(isheet, col, row, true);
			}
			
			
		}
	}
	if (col == 2 && row == 2 && updn) {
		auto charts = m_pSimDoc->m_PlotStorage.m_PlotArray.size();
		if (charts > 0) {
			m_pCUG->CreateZoomPlotWindow(true);
		}
	}


}

/////////////////////////////////////////////////////////////////////////////
//	OnRClicked
//		Sent whenever the user clicks the right mouse button within the grid
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' and 'row' are negative if the area clicked in is not a valid cell
//		'rect' the rectangle of the cell that was clicked in
//		'point' the point where the mouse was clicked
//		'updn'  TRUE if the button is down FALSE if the button just when up
void ValidationCug::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
//		Sent whenever the user double clicks the left mouse button within the grid
//	
//		'col' and 'row' are negative if the area clicked in is not a valid cell
//		'rect' the rectangle of the cell that was clicked in
//		'point' the point where the mouse was clicked
void ValidationCug::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	StartEdit();

}				 

/////////////////////////////////////////////////////////////////////////////
//	OnMouseMove
void ValidationCug::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user clicks the left mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void ValidationCug::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	int isheet = GetSheetNumber();
	CUGCell cell;
	if (updn && row == -2 && col == 1) {
		if (m_Distribution_As_Independent) {
			m_pProp->SetFirstIndepSet(PARVAR, false);
			//if (m_pProp->m_ChartType == CHART_TYPES::FREQUENCE) m_pProp->m_ChartType = CHART_TYPES::CUM_FREQUENCE;
			//else if (m_pProp->m_ChartType == CHART_TYPES::TIME_SERIE) m_pProp->m_ChartType = CHART_TYPES::FREQUENCE;
			//else if (m_pProp->m_ChartType == CHART_TYPES::CUM_FREQUENCE) m_pProp->m_ChartType = CHART_TYPES::TIME_SERIE;
			//else if (m_pProp->m_ChartType == CHART_TYPES::SCATTER) m_pProp->m_ChartType = CHART_TYPES::CUM_FREQUENCE;
			if (m_pProp->m_ChartType == CHART_TYPES::TIME_SERIE) m_pProp->SetFirstIndepVarType(NO_USE);
			auto ivar = m_pSimDoc->m_SelectMultiVal.size();
			if (ivar > 0) m_pCUG->CreateZoomPlotWindow(true);
		}
		else {
			/*if (col == 1) {
				GetCell(col, row, &cell);
				auto m_After = cell.GetStdText();
				auto optionlist = cell.GetStdLabelText();
				auto test = FUtil::GetOptionIndex(optionlist, m_After);
				m_pProp->SetFirstIndepIndex(PARVAR, FUtil::GetOptionIndex(optionlist, m_After));







				m_Indep_Par_Index==test;
				//m_Indep_Par_Index = m_Indep_Par_Index % 10; //Change to correct upper limit
				m_pProp->SetFirstIndepVarType(PARVAR);
				m_pProp->m_ChartType = CHART_TYPES::SCATTER;
				m_pProp->ResetXScale(string::npos);
				m_pProp->SetFirstIndepIndex(PARVAR, m_Indep_Par_Index);
				m_pProp->SetFirstIndepString(PARVAR, "kool");
				m_pProp->SetFirstIndepSet(PARVAR, true);











			}*/
		}
		OnSheetSetup(isheet);

		/*
		MyCug* pCug;
		pCug = MySingle::instance().GetCugPointer();
		int isheet = pCug->GetSheetNumber();
		if (isheet == 6 || isheet == 10)
			pCug->OnSheetSetup(isheet);
		else
			pCug->OnSheetSetup(6);*/
	}

	if ( updn &&col==0&&isheet>=9)
	{
		m_SelectedVarIndCount++;
		OnSheetSetup(isheet);
		//SelectRange ( col, 0, col, GetNumberRows()-1 );
	}

}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_RClicked
//		Sent whenever the user clicks the right mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void ValidationCug::OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if (!updn) {






	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top heading
//	
//		'col' is negative if the area clicked in is not valid
void ValidationCug::OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{



}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_LClicked
//		Sent whenever the user clicks the left mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void ValidationCug::OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if ( !updn ) return;
	MyCug *pCug = MySingle::instance().GetCugPointer();
	int isheet=GetSheetNumber();
	if(isheet>=9&&(m_SelectedTimeShow==0||m_SelectedTimeShow==3||m_SelectedTimeShow==5)) {
		m_SelectedTimeIndCount++;
		OnSheetSetup(isheet);
	}
	else if (isheet < 9) {
		if (col == -2||col==-1) {
			if (m_pSimDoc->m_MStorage.m_AcceptedIdString.size() > 1) {
				if(col==-2)
					m_pSimDoc->m_MStorage.ChangeEnsembleNo(string::npos);
				else
					m_pSimDoc->m_MStorage.ChangeEnsembleNo(1);
				m_pSimDoc->m_MStorage.SetActual_AcceptedEnsemble();
			//	if (m_pSimDoc->m_MStorage.Check_Change_PerformanceIndicator_ToActual_Ensemble()) {
			//		m_pSimDoc->m_MStorage.CompleteProcessingOfInputFiles_MultiStorage();
			//	}
			}
			//auto charts = m_pSimDoc->m_PlotStorage.m_PlotArray.size();
			/*if (charts > 0) {
				auto pinfo = m_pSimDoc->m_PlotStorage.GetPlotInfo(charts - 1);
				pinfo.EnsembleNo = m_pSimDoc->m_MStorage.m_ActualEnsemble;
				m_pSimDoc->m_PlotStorage.m_PlotArray.erase(m_pSimDoc->m_PlotStorage.m_PlotArray.begin());
				m_pSimDoc->m_PlotStorage.m_PlotArray.push_back(pinfo);
				auto ivar = m_pSimDoc->m_SelectMultiVal.size();
				if (ivar > 0) m_pCUG->CreateZoomPlotWindow(true);
			}*/
			MultiPlotClick(isheet, 0, 0, false);

			OnSheetSetup(isheet);

			return;
		}
		m_CurrentTopRow = GetTopRow();
		auto FileNo = m_pSimDoc->m_ValidationData.GetValFileNumber(row);
		CResiduals *pRes = m_pSimDoc->GetValidResidualPointer(FileNo);

		if (pRes!=nullptr&&!pRes->m_MeanDefined) {

			BeginWaitCursor();

			pCug->CalculateMeanOffTimeSeries(5002, FileNo);
			EndWaitCursor();

		}

	}
	else if(m_SelectedTimeShow==1) {
		long id_run;
		id_run=m_pSimDoc->m_MStorage.Get_AcceptedRuns(row);
		TimeSeriePlotClick(isheet, col, row, id_run);


	}
	else if(m_SelectedTimeShow==2) {
		long id_run;
		id_run=row;
		TimeSeriePlotClick(isheet, col, row, id_run);


	}
	
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_RClicked
//		Sent whenever the user clicks the right mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void ValidationCug::OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_DClicked
//		Sent whenever the user double clicks the left mouse button within the side heading
//	
//		'row' is negative if the area clicked in is not valid
void ValidationCug::OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_LClicked
//		Sent whenever the user clicks the left mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void ValidationCug::OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
	if (!updn) {
		int isheet = GetSheetNumber();
		if (m_pSimDoc->m_MStorage.m_AcceptedIdString.size() > 0) {
			if (m_pSimDoc->m_MStorage.m_AcceptedIdString.size() > 1) {
				m_pSimDoc->m_MStorage.ChangeEnsembleNo(1);
				m_pSimDoc->m_MStorage.SetActual_AcceptedEnsemble();
				if (m_pSimDoc->m_MStorage.Check_Change_PerformanceIndicator_ToActual_Ensemble()) {
					m_pSimDoc->m_MStorage.CompleteProcessingOfInputFiles_MultiStorage();
				}
			}
			auto charts=m_pSimDoc->m_PlotStorage.m_PlotArray.size();
			if (charts > 0) {
				auto pinfo = m_pSimDoc->m_PlotStorage.GetPlotInfo(charts - 1);
				pinfo.EnsembleNo = m_pSimDoc->m_MStorage.m_ActualEnsemble;
				m_pSimDoc->m_PlotStorage.m_PlotArray.erase(m_pSimDoc->m_PlotStorage.m_PlotArray.begin());
				m_pSimDoc->m_PlotStorage.m_PlotArray.push_back(pinfo);
				auto ivar = m_pSimDoc->m_SelectMultiVal.size();
				if (ivar > 0) m_pCUG->CreateZoomPlotWindow(true);
			}
			OnSheetSetup(isheet);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
//	OnCB_RClicked
//		Sent whenever the user clicks the right mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void ValidationCug::OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{


}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top corner button
void ValidationCug::OnCB_DClicked(RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyDown
//		Sent whenever the user types when the grid has focus. The keystroke can be
//		modified here as well. (See WM_KEYDOWN for more information)
void ValidationCug::OnKeyDown(UINT *vcKey,BOOL processed)
{
	if ( *vcKey == VK_SPACE )
	{
//		m_GI->m_multiSelect->FloatingBlock( 0, 0, TRUE );
	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnCharDown
//		Sent whenever the user types when the grid has focus. The keystroke can be 
//		modified here as well. (See WM_CHAR for more information)
void ValidationCug::OnCharDown(UINT *vcKey,BOOL processed)
{
}
	
/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent everytime the grid needs to
//		draw a cell in the grid. At this point the cell
//		class has been filled with the information to be
//		used to draw the cell. The information can now be
//		changed before it is used for drawing
void ValidationCug::OnGetCell(int col,long row,CUGCell *cell)
{
	/*cell->SetNumberDecimals ( 0 );

	 if ( col == -1 && row >= 0 )
		cell->SetNumber ( row );
	else if ( row == -1 && col >= 0 )
		cell->SetNumber ( col );*/
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetCell
//		This message is sent everytime the a cell is about to change.
void ValidationCug::OnSetCell(int col,long row,CUGCell *cell)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDataSourceNotify
//		This message is sent from a data source , message
//		depends on the data source - check the information
//		on the data source(s) being used
//		- The ID of the Data source is also returned
void ValidationCug::OnDataSourceNotify(int ID,long msg,long param)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellTypeNotify
//		This message is sent from a cell type , message
//		depends on the cell type - check the information
//		on the cell type classes
//		- The ID of the cell type is given
int ValidationCug::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	CUGCell cell;
	string m_After, optionlist;
	m_After="";
	int prior;
	int sheetNumber = GetSheetNumber();

	//m_CurrentTopRow = GetTopRow();

	if(ID==1) {
		if(msg==103||msg==100) {
			GetCell(col,row, &cell);
			auto koll=cell.GetCellType();
			auto koll2 = cell.GetParam();
			m_After=cell.GetStdText();
			optionlist = cell.GetStdLabelText();
			auto optionlist2 = cell.GetLabelText();
		}
		if (col == 1||col==5&&sheetNumber<7) {
			auto test = FUtil::GetOptionIndex(optionlist, m_After);
			auto row_of_par = test;
			size_t mr_par_index{ 0 };
			if (m_pSimDoc->MR_GetNumberOfDimensions() > 1) {
				mr_par_index+=m_pSimDoc->MR_Get_NumberOfParametersWithinDim(1);
			}
			mr_par_index +=test ;
			if (cell.GetParam() == 100) {
				m_pProp->SetFirstIndepIndex(PARVAR, mr_par_index);
				m_pProp->SetFirstIndepString(PARVAR, m_After);
				m_pProp->SetFirstIndepVarType(PARVAR);
			}
			else if (cell.GetParam() == 300) {
				m_pProp->SetSecondIndepIndex(PARVAR, mr_par_index);
				m_pProp->SetSecondIndepString(PARVAR, m_After);
				m_pProp->SetSecondIndepVarType(PARVAR);
			}

			MultiPlotClick(sheetNumber, col, row_of_par, false);
			OnSheetSetup(0);

		}
		if (col == 2 && sheetNumber == 12) {
			auto iopt = FUtil::GetOptionIndex(optionlist, m_After);
			m_pUtil->SetEnsembleViewOption(iopt);
			OnSheetSetup(12);

		}
		else if(row==m_NumLines+1) {
			prior=m_pSw1->GetIntValue();
			if(m_After!="") {
				m_pSw1->SetStrValue(m_After);
				m_LogFunc=m_pSw1->GetIntValue();
				m_pSimDoc->m_ValidationData.SetValNewLogLiOptions(m_LogFunc, m_BoxCoxTrans);
				m_pSw1->SetIntValue(prior);
			}

		}
		else if(row==m_NumLines+2) {
			prior=m_pSw2->GetIntValue();
			if(m_After!="") {
				m_pSw2->SetStrValue(m_After);
				m_BoxCoxTrans=m_pSw2->GetIntValue();
				m_pSimDoc->m_ValidationData.SetValNewLogLiOptions(m_LogFunc, m_BoxCoxTrans);
				m_pSw2->SetIntValue(prior);
			}


		}
		else if(row==2) {
			// Change of Option of Actual Ensemble;
			size_t index=FUtil::GetOptionIndex(optionlist, m_After);
			m_pSimDoc->m_MStorage.m_ActualEnsemble = index;
			m_pSimDoc->m_MStorage.SetActual_AcceptedEnsemble();
			if (m_pSimDoc->m_MStorage.Check_Change_PerformanceIndicator_ToActual_Ensemble()) {
				m_pSimDoc->m_MStorage.CompleteProcessingOfInputFiles_MultiStorage();
			}
			OnSheetSetup(12);
		}


	}
	else if (ID == 2) { // Check box type

		if (col == 3 && row > 3) {
		size_t iens = row - 4;
		GetCell(col, row, &cell);
		if (cell.GetBool())
			m_pSimDoc->m_MStorage.SetToBePlotted(iens, true);
		else
			m_pSimDoc->m_MStorage.SetToBePlotted(iens, false);

		// ReDraw Chart
		//ChartUtil *pUtil;
		//pUtil=MySingle::instance().GetChartUtilPointer();
		//pUtil->m_FileSave.OnReDraw(true);
	}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditStart
//		This message is sent whenever the grid is ready to start editing a cell
//		A return of TRUE allows the editing a return of FALSE stops editing
//		Plus the properties of the CEdit class can be modified
int ValidationCug::OnEditStart(int col, long row,CWnd **edit)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditVerify
//		This is send when the editing is about to end
int ValidationCug::OnEditVerify(int col, long row,CWnd *edit,UINT *vcKey)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int ValidationCug::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag)
{
	CString str;
	CUGCell cell;
	edit->GetWindowText(str);
	int SheetNumber=GetSheetNumber();
	if (SheetNumber == 9) {
		float value;
		value = MFC_Util::AtoFloat(str);
		if (col == m_newcolst) {
			m_pSimDoc->m_ValidationData.SetValP_ErrorNew(row, value);
		}
		else if (col == m_newcolst + 1) {
			m_pSimDoc->m_ValidationData.SetValA_ErrorNew(row, value);
		}
		else if (col == m_newcolst + 2) {
			m_pSimDoc->m_ValidationData.SetValBoxCox_PowerNew(row, value);
		}
		else if (col == m_newcolst + 3) {
			m_pSimDoc->m_ValidationData.SetValBoxCox_OffsetNew(row, value);
		}
	}
	if(SheetNumber==12) {
		if(col==1&&row==1) {
			auto old=m_NumberNewSplitPeriods;
			m_NumberNewSplitPeriods=MFC_Util::AtoInt(str);
			if(m_NumberNewSplitPeriods!=5) QuickSetBackColor(1,1,m_ColorChanged);
			if(m_NumberNewSplitPeriods!=old) {
				DefineNewSubPeriod();
				return TRUE;
			}
		}
		else if (row > 2 && col == 0) {
			size_t index = row - 4;
			m_pSimDoc->m_MStorage.ChangeEnsembleNames(index, CT2A(str).m_psz);
			m_pSimDoc->m_MStorage.m_AcceptedIdString[index] = CT2A(str).m_psz;
		}

	}
	QuickSetText(col,row,str);
	if(col>9) {
		auto Set_SV_Criteria = [&](float value) {
			size_t  ivar = row - m_NumValidVar*(m_SplitPeriods + 1) - 2;
			if (m_NumValidVar == 0) ivar++;
			int NumLinesSV = m_pSimDoc->m_ValidationData.GetNumSumVarVariables();
			size_t SV_index = SheetNumber*m_pSimDoc->GetNumberofSumVariables() + ivar;
			int criteria = m_pSimDoc->m_MStorage.GetMeanSumCriteria(ivar);
			if (col == 12) {
				if (SV_index<NumLinesSV * 7&&ivar<NumLinesSV) {
					criteria = m_pSimDoc->m_MStorage.GetSumCriteria(SV_index);
					m_pSimDoc->m_MStorage.m_Valid_SV[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_LOW][SV_index] = value;
					if (criteria == 0)
						m_pSimDoc->m_MStorage.SetSumCriteria(SV_index, CRITERIA_STATE::ABOVE_MIN);
					else
						m_pSimDoc->m_MStorage.SetSumCriteria(SV_index, CRITERIA_STATE::ABOVE_AND_BELOW);
				}
				else {
					m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_LOW][SheetNumber] = value;
					if (m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].criteria[SheetNumber] == 0)
						m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].criteria[SheetNumber] = CRITERIA_STATE::BELOW_MAX;
					else
						m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].criteria[SheetNumber] = CRITERIA_STATE::ABOVE_AND_BELOW;

				}
			}
			else if (col == 13) {
				if (SV_index<NumLinesSV * 7) {

					m_pSimDoc->m_MStorage.m_Valid_SV[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_HIGH][SV_index] = value;
					//m_pSimDoc->m_MStorage.SetValidSumHigh(index, value);
					if (criteria == 0)
						m_pSimDoc->m_MStorage.SetSumCriteria(SV_index, CRITERIA_STATE::BELOW_MAX);
					else
						m_pSimDoc->m_MStorage.SetSumCriteria(SV_index, CRITERIA_STATE::ABOVE_AND_BELOW);
				}
				else {
					m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_HIGH][SheetNumber] = value;
					if (m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].criteria[SheetNumber] == 0)
						m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].criteria[SheetNumber] = CRITERIA_STATE::BELOW_MAX;
					else
						m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].criteria[SheetNumber] = CRITERIA_STATE::ABOVE_AND_BELOW;
				}
			}
			QuickSetBackColor(col, row, m_ColorChanged);
			m_Changed = TRUE;


		};
		auto Set_TSV_Criteria = [&](float value) {
			GetCell(col, row, &cell);
			int ind_split = cell.GetParam();
			int iv;
			iv = row / (m_SplitPeriods + 1);

			int index_start = SheetNumber*m_NumValidVar;
			if (col == 12) {
				if (SheetNumber == 7) {
			        m_pSimDoc->m_MStorage.m_ValSumValues_Stored[row].ValueMin = value;


				}
				else {

					m_pSimDoc->m_MStorage.SetValidLow(iv + index_start, ind_split, value);
					if (m_pSimDoc->m_MStorage.GetCriteria(iv + index_start, ind_split) == 0)
						m_pSimDoc->m_MStorage.SetCriteria(iv + index_start, CRITERIA_STATE::ABOVE_MIN, ind_split);
					else
						m_pSimDoc->m_MStorage.SetCriteria(iv + index_start, CRITERIA_STATE::ABOVE_AND_BELOW, ind_split);
				}

			}
			else if (col == 13) {
				if (SheetNumber == 7) {
					m_pSimDoc->m_MStorage.m_ValSumValues_Stored[row].ValueMax = value;
				}
				else {
					m_pSimDoc->m_MStorage.SetValidHigh(iv + index_start, ind_split, value);
					if (m_pSimDoc->m_MStorage.GetCriteria(iv + index_start, ind_split) == 0)
						m_pSimDoc->m_MStorage.SetCriteria(iv + index_start, CRITERIA_STATE::BELOW_MAX, ind_split);
					else
						m_pSimDoc->m_MStorage.SetCriteria(iv + index_start, CRITERIA_STATE::ABOVE_AND_BELOW, ind_split);
				}

			}
			QuickSetBackColor(col, row, m_ColorChanged);
			m_Changed = TRUE;
		};
		auto Set_TSV_Mean_Criteria = [&](float value) {
			int criteria = m_pSimDoc->m_MStorage.GetMeanCriteria(SheetNumber);
			if (col == 12) {
				m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_LOW][SheetNumber] = value;
				if (criteria == 0)
					m_pSimDoc->m_MStorage.SetMeanCriteria(SheetNumber, CRITERIA_STATE::ABOVE_MIN);
				else
					m_pSimDoc->m_MStorage.SetMeanCriteria(SheetNumber, CRITERIA_STATE::ABOVE_AND_BELOW);
			}
			else if (col == 13) {
				m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_HIGH][SheetNumber] = value;

				if (criteria == 0)
					m_pSimDoc->m_MStorage.SetMeanCriteria(SheetNumber, CRITERIA_STATE::BELOW_MAX);
				else
					m_pSimDoc->m_MStorage.SetMeanCriteria(SheetNumber, CRITERIA_STATE::ABOVE_AND_BELOW);
			}


			QuickSetBackColor(col, row, m_ColorChanged);
			m_Changed = TRUE;

		};

		float value;
	    value=MFC_Util::AtoFloat(str);

	  if(row==m_NumValidVar*(m_SplitPeriods+1)) 
		  Set_TSV_Mean_Criteria(value);
	  else if(row>m_NumValidVar*(m_SplitPeriods+1))
		  Set_SV_Criteria(value);
	  else 
		  Set_TSV_Criteria(value);
	
	}


	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int ValidationCug::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	sections - UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//				UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON

void ValidationCug::OnMenuCommand(int col,long row,int section,int item)
{
	CUGCell cell;
	int isheet=GetSheetNumber();

	auto Reset_All = [&]() {
		m_pSimDoc->m_MStorage.UpdateAcceptedRuns(true, false);
		m_Changed = TRUE;
		m_SavedCriteria2File = false;
		m_pSimDoc->MR_ReCalculatePostDist(false);
		for (int i = 0; i < m_pSimDoc->m_ResidualFileVector.size(); i++) {
			CResiduals* pRes;
			pRes = m_pSimDoc->GetValidResidualPointer(i + 1);
			if (pRes != nullptr) {
				pRes->ToBeUpdated();
				pRes->m_MeanDefined = false;
			}
		}

		m_pSimDoc->m_MStorage.EvaluateFile(false);
	};
	if (item == 4055) {
		if (m_Distribution_As_Independent)
			m_Distribution_As_Independent = false;
		else
			m_Distribution_As_Independent = true;
		OnSheetSetup(isheet);
		return;
	}
	if (item == 4056) {
		if (m_Add_Second_Independent)
			m_Add_Second_Independent = false;
		else
			m_Add_Second_Independent = true;
		OnSheetSetup(isheet);
		return;
	}
	if (item == 1165) {
		//Save Criteriea
		string fileName=MFC_Util::GetSBinFileName();

		m_pSimDoc->m_MStorage.EvaluateFile(false, fileName);
	}
	else if (item == 1175) {
		//Read File with new criteria
		string fileName = MFC_Util::GetSBinFileName();
		m_pSimDoc->m_MStorage.EvaluateFile(true, fileName);

	}
	else if (item == 1065) {
		if(m_pSimDoc->PreDefined_Criteria_Process());
		OnSheetSetup(isheet);
	}
	else if (item == 1066) {
		if (m_pSimDoc->PreDefined_Criteria_Process(true));
		OnSheetSetup(isheet);
	}
	else if (item == 1063) {
			m_pSimDoc->m_MStorage.AddNewEnsemble(false);
			OnSheetSetup(isheet);
	}
	else if (item == 1064) {
			m_pSimDoc->m_MStorage.AddNewEnsemble(true);
			OnSheetSetup(isheet);
	}
	else if (item == 1061||item==1071) {

		if (item == 1071) {
			auto saveens = m_pSimDoc->m_MStorage.m_ActualEnsemble;
			for (size_t iens = 0; iens < m_pSimDoc->m_MStorage.m_AcceptedRuns.size(); iens++) {
				m_pSimDoc->m_MStorage.m_ActualEnsemble = iens;
				Reset_All();
			}
			m_pSimDoc->m_MStorage.m_ActualEnsemble = saveens;
		}
		else 
			Reset_All();

		
		MyCug *pCug;
		pCug = MySingle::instance().GetCugPointer();
		int isheet = pCug->GetSheetNumber();
		if (isheet == 6 || isheet == 10)
			pCug->OnSheetSetup(isheet);

	}
	else if (item == 1060) {
		//Update list to current criteria
		m_pSimDoc->m_MStorage.m_SV_StatMemory.AcceptedRuns = false;
		m_pSimDoc->m_MStorage.UpdateAcceptedRuns(false, false);
		m_Changed = TRUE;
		m_SavedCriteria2File = false;
		m_pSimDoc->MR_ReCalculatePostDist(false);

		for (int i = 0; i<MAXSIMVAL; i++) {
			CResiduals *pRes;
			pRes = m_pSimDoc->GetValidResidualPointer(i + 1);
			if (pRes != nullptr) {
				pRes->ToBeUpdated();
				pRes->m_MeanDefined = false;
			}
		}
		m_pSimDoc->m_MStorage.EvaluateFile(false);
		/*	ChartUtil *m_pUtil;
		m_pUtil=MySingle::instance().GetChartUtilPointer();
		m_pUtil->m_FileSave.OnReDraw(true, m_pSimDoc);*/

		MyCug *pCug;
		pCug = MySingle::instance().GetCugPointer();
		int isheet = pCug->GetSheetNumber();
		if (isheet == 6 || isheet == 10)
			pCug->OnSheetSetup(isheet);
		else
			pCug->OnSheetSetup(6);




	}
	if(section==UG_GRID) {
		if(item== 1000) {
			CutSelected();
		}
		else if(item==2000)
			CopySelected();

		else if(item==3000){
			Paste();
		}
		else if(item==1075) {
			FixZeroShiftOfPostMean(size_t(row));
			m_pSimDoc->m_MStorage.EvaluateFile(false);
			MyCug *pCug;
			pCug = MySingle::instance().GetCugPointer();
			int isheet = pCug->GetSheetNumber();
			pCug->OnSheetSetup(isheet);
		}
		else if(item==1085||item==1087||item==1088) {
			bool SV_Variable = false;
			if (row > m_NumValidVar*(m_SplitPeriods + 1)) {
				size_t  ivar = row - m_NumValidVar*(m_SplitPeriods + 1) - 2;
				if (m_NumValidVar == 0) ivar++;
				SV_Variable = true;
				row = ivar;
			}
			else {
				SV_Variable = false;
			}

			size_t Threshold = 200;
			if (item == 1087) Threshold = 10;
			else if (item == 1088) Threshold = 3;

			size_t VarIndex = row;
			size_t ThresholdType = 0;
			size_t left;
			if(isheet==0) 
				left = m_pSimDoc->m_MStorage.ReduceCriteria(R2_STAT, VarIndex, ThresholdType, Threshold);
				//ReducebyR2(size_t(row), Threshold, SV_Variable);
			else if(isheet==1)
				left = m_pSimDoc->m_MStorage.ReduceCriteria(AO_STAT, VarIndex, ThresholdType, Threshold);
				//ReducebyR2(size_t(row), Threshold, SV_Variable
			else if(isheet==2)
				left = m_pSimDoc->m_MStorage.ReduceCriteria(A1_STAT, VarIndex, ThresholdType, Threshold);
				//ReducebySlopeInterval(size_t(row), Threshold, SV_Variable);
			else if(isheet==3)
				left = m_pSimDoc->m_MStorage.ReduceCriteria(ABSME_STAT, VarIndex, ThresholdType, Threshold);
				//ReducebyMeanInterval(size_t(row), Threshold, SV_Variable);
			else if(isheet==4)
				left = m_pSimDoc->m_MStorage.ReduceCriteria(RMSE_STAT, VarIndex, ThresholdType, Threshold);
				//ReducebyRMSE(size_t(row), Threshold, SV_Variable);
			else if(isheet==5)
				left = m_pSimDoc->m_MStorage.ReduceCriteria(LOGLI_STAT, VarIndex, ThresholdType, Threshold);
				//ReducebyLogLi(size_t(row), Threshold, SV_Variable);
			else if(isheet==6)
				left = m_pSimDoc->m_MStorage.ReduceCriteria(NSE_STAT, VarIndex, ThresholdType, Threshold);
			//	ReducebyNSE(size_t(row), Threshold, SV_Variable);

			//if (ZeroAdjust > 0.00001) left = m_MStorage.ReduceCriteria(ZERO_ADJUST, VarIndex);
			m_pSimDoc->m_MStorage.IsMaxUpdated[m_pSimDoc->m_MStorage.m_ActualEnsemble] = m_pSimDoc->m_MStorage.IsMinUpdated[m_pSimDoc->m_MStorage.m_ActualEnsemble] = false;
			auto MaxNum = m_pSimDoc->GetNumAll_TSV_ValVariables();
			size_t ip = MaxNum * isheet + VarIndex;
			auto dist_pair = m_pSimDoc->m_MStorage.Get_ValidPerformanceIndicatorPairVector(true, ip, 0);

			m_pSimDoc->m_MStorage.SetMinMaxOfPerformanceIndicators(false);


			for (int i = 0; i < 6; i++)
				Make_MainSheets(i);
			Make_CriteriaSheet();
			Make_OptionSheet();
			OnSheetSetup(isheet);
			SetSheetNumber(isheet);

			m_pSimDoc->m_MStorage.EvaluateFile(false);
			MyCug *pCug;
			pCug = MySingle::instance().GetCugPointer();
			int isheet = pCug->GetSheetNumber();
			if (isheet == 6 || isheet == 10)
				pCug->OnSheetSetup(isheet);
			else
				pCug->OnSheetSetup(6);

			OnSheetSetup(isheet);

		}
		else if(item==1086) {
			bool SV_Variable = false;

			if (row > m_NumValidVar*(m_SplitPeriods + 1)) {
				size_t  ivar = row - m_NumValidVar*(m_SplitPeriods + 1) - 2;
				if (m_NumValidVar == 0) ivar++;
				SV_Variable = true;
				row = ivar;
			}
			else {
				SV_Variable = false;

			}

			
			auto Threshold = m_pSimDoc->m_MStorage.m_AcceptedRuns[m_pSimDoc->m_MStorage.m_ActualEnsemble].size()/2;
			size_t VarIndex = row;
			size_t ThresholdType = 0;
			size_t left;
			if (isheet == 0)
				left = m_pSimDoc->m_MStorage.ReduceCriteria(R2_STAT, VarIndex, ThresholdType, Threshold);
			//ReducebyR2(size_t(row), Threshold, SV_Variable);
			else if (isheet == 1)
				left = m_pSimDoc->m_MStorage.ReduceCriteria(AO_STAT, VarIndex, ThresholdType, Threshold);
			//ReducebyR2(size_t(row), Threshold, SV_Variable
			else if (isheet == 2)
				left = m_pSimDoc->m_MStorage.ReduceCriteria(A1_STAT, VarIndex, ThresholdType, Threshold);
			//ReducebySlopeInterval(size_t(row), Threshold, SV_Variable);
			else if (isheet == 3)
				left = m_pSimDoc->m_MStorage.ReduceCriteria(ABSME_STAT, VarIndex, ThresholdType, Threshold);
			//ReducebyMeanInterval(size_t(row), Threshold, SV_Variable);
			else if (isheet == 4)
				left = m_pSimDoc->m_MStorage.ReduceCriteria(RMSE_STAT, VarIndex, ThresholdType, Threshold);
			//ReducebyRMSE(size_t(row), Threshold, SV_Variable);
			else if (isheet == 5)
				left = m_pSimDoc->m_MStorage.ReduceCriteria(LOGLI_STAT, VarIndex, ThresholdType, Threshold);
			//ReducebyLogLi(size_t(row), Threshold, SV_Variable);
			else if (isheet == 6)
				left = m_pSimDoc->m_MStorage.ReduceCriteria(NSE_STAT, VarIndex, ThresholdType, Threshold);
			//	ReducebyNSE(size_t(row), Threshold, SV_Variable);

			//if (ZeroAdjust > 0.00001) left = m_MStorage.ReduceCriteria(ZERO_ADJUST, VarIndex);
			m_pSimDoc->m_MStorage.IsMaxUpdated[m_pSimDoc->m_MStorage.m_ActualEnsemble] = m_pSimDoc->m_MStorage.IsMinUpdated[m_pSimDoc->m_MStorage.m_ActualEnsemble] = false;
			auto MaxNum = m_pSimDoc->GetNumAll_TSV_ValVariables();
			size_t ip = MaxNum * isheet + VarIndex;
			auto dist_pair = m_pSimDoc->m_MStorage.Get_ValidPerformanceIndicatorPairVector(true, ip, 0);

			m_pSimDoc->m_MStorage.SetMinMaxOfPerformanceIndicators(false);

			MyCug *pCug;
			pCug = MySingle::instance().GetCugPointer();
			int isheet = pCug->GetSheetNumber();
			if (isheet == 6 || isheet == 10)
				pCug->OnSheetSetup(isheet);

			OnSheetSetup(isheet);
			m_pSimDoc->m_MStorage.EvaluateFile(false);

		}
		else if(item==1062) {
			m_pSimDoc->m_MStorage.UpdateAcceptedRuns(false, true);
			m_SavedCriteria2File=true;
		}
		else if(item==2010) {

			EstimateNewLogLi();

		}
		else if(item==2020) {
			MakeSortedResiduals();

		}
		else if(item==4000) {
			DefineNewSubPeriod();
			m_SavedCriteria2File=false;
		}
		else if (item == 4091) {

			size_t index = row - 4;
			if(m_pSimDoc->m_MStorage.RemoveEnsemble(index))	OnSheetSetup(isheet);

		}
		else if (item >= 4019 && item < 4024) {
			m_Transformation = item - 4019;
			// Check if already estimated
			m_pSimDoc->m_MStorage.m_CumulatedPerformance = m_Transformation;
			if (!m_pSimDoc->m_MStorage.SetActualPerformanceVector_TSV_Variables())
				AfxBeginThread(UpdateValidationVar, this);
		}

		else if (item >= 4024&&item<4100) {
			m_Transformation = 0;
			m_Dim1_Index = item - 4024 + 1;
			m_SavedCriteria2File = false;
			AfxBeginThread(UpdateValidationVar, this);
		}
		else if(item==4001) {
			EraseAllSelected(); 
			OnSheetSetup(isheet);
			auto pChart=MySingle::instance().GetChartCugPointer();
			pChart->MakeTextPlot("No Variables selected");

		}

		if(item>=1060&&item<=1061) {
			m_Changed=FALSE;
			m_Updated=TRUE;

			
			for(int i=0;i<6;i++)
				Make_MainSheets(i);
			Make_CriteriaSheet();
			Make_OptionSheet();
			OnSheetSetup(isheet);
			SetSheetNumber(isheet);


		}

		
	}
	else if(section==UG_CORNERBUTTON&&isheet>=9) {
		if(item>=3010)
			m_SelectedTimeShow=item-3010;

		OnSheetSetup(isheet);
	}
	else if(section==UG_TOPHEADING&&isheet>=9) {
		if(item>=3020)
			m_SelectedVarShow=item-3020;

		OnSheetSetup(isheet);
	}
	else if(section==UG_TOPHEADING) {
		if(item>=4019&&item<4024) {
			m_Transformation=item-4019;
			// Check if already estimated
			m_pSimDoc->m_MStorage.m_CumulatedPerformance = m_Transformation;
			if (!m_pSimDoc->m_MStorage.SetActualPerformanceVector_TSV_Variables())
				AfxBeginThread(UpdateValidationVar,this);
		}

		else if (item >= 4024) {
			m_Transformation = 0;
			m_Dim1_Index = item - 4024 + 1;
			m_SavedCriteria2File = false;
			AfxBeginThread(UpdateValidationVar, this);
		}
		else if(item==1062) {
			m_pSimDoc->m_MStorage.UpdateAcceptedRuns(false, true);
		//	m_pSimDoc->m_MStorage.Close();
		//	m_pSimDoc->m_MStorage.Save(true);
			m_SavedCriteria2File=true;
		}
		OnSheetSetup(isheet);
	}
}

/////////////////////////////////////////////////////////////////////////////
//	OLE
#ifdef __AFXOLE_H__

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  ValidationCug::OnDragEnter(COleDataObject* pDataObject){
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  ValidationCug::OnDragOver(COleDataObject* pDataObject,int col,long row){
	//return DROPEFFECT_NONE;
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  ValidationCug::OnDragDrop(COleDataObject* pDataObject,int col,long row){
	return DROPEFFECT_COPY;
}
#endif

/////////////////////////////////////////////////////////////////////////////
//	
void ValidationCug::OnScreenDCSetup(CDC *dc,int section){
}

/////////////////////////////////////////////////////////////////////////////
//	OnSortEvaluate
//	return		-1  <
//				0	==
//				1	>
int ValidationCug::OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags)
{
	if(flags&UG_SORT_DESCENDING)
	{
		CUGCell *ptr = cell1;
		cell1 = cell2;
		cell2 = ptr;
	}

//	if(cell1->IsPropertySet(UGCELL_TEXT_SET) == FALSE)
//	{
//		if(cell2->IsPropertySet(UGCELL_TEXT_SET) == FALSE)
//			return 0;
//		return -1;
//	}
	switch(cell1->GetDataType())
	{
		case UGCELLDATA_STRING:
			if(nullptr == cell1->GetText() && nullptr == cell2->GetText())
				return 0;
			if(nullptr == cell1->GetText())
				return 1;
			if(nullptr == cell2->GetText())
				return -1;
			return _tcscmp(cell1->GetText(),cell2->GetText());	
		case UGCELLDATA_NUMBER:
		case UGCELLDATA_BOOL:
		case UGCELLDATA_CURRENCY:
				double n1 = cell1->GetNumber();
				double n2 = cell2->GetNumber();
				if(n1 < n2)
					return -1;
				if(n1 > n2)
					return 1;
				return 0;
	}
	if(nullptr == cell1->GetText())
		return 1;
	if(nullptr == cell2->GetText())
		return -1;
	return _tcscmp(cell1->GetText(),cell2->GetText());	
}

/////////////////////////////////////////////////////////////////////////////
//	OnTabSelected
void ValidationCug::OnTabSelected(int ID)
{	
	int isheet=GetSheetNumber();
	OnSheetSetup(ID);
}

/////////////////////////////////////////////////////////////////////////////
//	OnAdjustComponentSizes
void ValidationCug::OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,
		RECT *cnrBtn,RECT *vScroll,RECT *hScroll,RECT *tabs)
{
} 

/////////////////////////////////////////////////////////////////////////////
//	OnDrawFocusRect
void ValidationCug::OnDrawFocusRect(CDC *dc,RECT *rect)
{
	//DrawExcelFocusRect(dc,rect);
	
	if ( !m_bDrawExcelFocus )
	{
		rect->bottom --;
		rect->right --;
		dc->DrawFocusRect(rect);
	}
	else
	{
		DrawExcelFocusRect( dc, rect);
	}
}

void ValidationCug::DrawExcelFocusRect(CDC *dc,RECT *rect)
{
	BOOL innerRectOnly = FALSE;
	BOOL hideTop = FALSE;
	BOOL hideLeft = FALSE;

	CPen pen,wPen;

	int dcID = dc->SaveDC();

	//find the rect to draw - may be a range of cells
	//if multi-select is on
	if(m_GI->m_multiSelectFlag&3)
	{
		if(m_GI->m_multiSelect->GetNumberBlocks() < 2)
		{
			int startCol,endCol;
			long startRow,endRow;
			m_GI->m_multiSelect->GetCurrentBlock(&startCol,&startRow,&endCol,&endRow);

			if( startRow < m_GI->m_topRow && endRow < m_GI->m_topRow )
				return;

			if( startRow > m_GI->m_bottomRow && endRow > m_GI->m_bottomRow )
				return;

			if( startCol < m_GI->m_leftCol && endCol < m_GI->m_leftCol )
				return;

			if( startCol > m_GI->m_rightCol && endCol > m_GI->m_rightCol )
				return;
			
			if(startCol != endCol || startRow != endRow)
			{
				GetRangeRect(startCol,startRow,endCol,endRow,rect);
			}
			if(startCol >= m_GI->m_numLockCols && startCol < m_GI->m_leftCol)
			{
				//hide the left
				hideLeft = TRUE;
			}
			if(startRow >= m_GI->m_numLockRows && startRow < m_GI->m_topRow)
			{
				//hide the top
				hideTop = TRUE;
			}
			if(rect->bottom <= rect->top)
				GetRangeRect(startCol,startRow,endCol,endRow,rect);
			if(rect->right <= rect->left)
				GetRangeRect(startCol,startRow,endCol,endRow,rect);
			
			//add draw hints for the grid so it will clear up the
			//focus rect next time
			m_CUGGrid->m_drawHint.AddHint(startCol-1,startRow-1,endCol+1,endRow+1);
		}
		else if(m_GI->m_multiSelect->GetNumberBlocks() >= 2)
		{
			innerRectOnly = TRUE;
		}
	}
	else
	{
		//add draw hints for the grid so it will clear up the
		//focus rect next time
		m_CUGGrid->m_drawHint.AddHint(m_GI->m_currentCol-1,m_GI->m_currentRow-1,
			m_GI->m_currentCol+1,m_GI->m_currentRow+1);
	}

	int top = rect->top;
	int left = rect->left;


	//inner black rect
	dc->SelectObject(GetStockObject(BLACK_PEN));
	if(rect->top == 0)
		rect->top ++;
	if(rect->left == 0)
		rect->left ++;
	if(!hideTop)
	{
		dc->MoveTo(rect->left,rect->top);
		dc->LineTo(rect->right-1,rect->top);
	}
	else
	{
		dc->MoveTo(rect->right-1,rect->top);
	}
	dc->LineTo(rect->right-1,rect->bottom-1);
	dc->LineTo(rect->left,rect->bottom-1);
	if(!hideLeft)
	{
		dc->LineTo(rect->left,rect->top-1);
	}
	rect->top = top;
	rect->left = left;


	//inner white rect
	dc->SelectObject(GetStockObject(WHITE_PEN));
	if(rect->top == 0)
		rect->top ++;
	if(rect->left == 0)
		rect->left ++;
	if(!hideTop)
	{
		dc->MoveTo(rect->left+1,rect->top+1);
		dc->LineTo(rect->right-2,rect->top+1);
	}
	else
	{
		dc->MoveTo(rect->right-2,rect->top+1);
	}
	dc->LineTo(rect->right-2,rect->bottom-2);
	dc->LineTo(rect->left+1,rect->bottom-2);
	if(!hideLeft)
	{
		dc->LineTo(rect->left+1,rect->top);
	}
	rect->top = top;
	rect->left = left;

	//draw the rest if innerRectOnly is false
	if(!innerRectOnly)
	{
		dc->SelectObject(GetStockObject(BLACK_PEN));

		//outer rect
		if(rect->top == 0)
			rect->top +=2;
		if(rect->left == 0)
			rect->left +=2;
		if(hideLeft)
			rect->left++;
		if(!hideTop)
		{
			dc->MoveTo(rect->left-2,rect->top-2);
			dc->LineTo(rect->right+1,rect->top-2);			
		}
		else
		{
			rect->top++;
			dc->MoveTo(rect->right+1,rect->top-2);
		}
		
		dc->LineTo(rect->right+1,rect->bottom+1);
		dc->LineTo(rect->left-2,rect->bottom+1);
		
		if(!hideLeft)
		{
			dc->LineTo(rect->left-2,rect->top -3);
		}

		rect->top = top;
		rect->left = left;


		//bottom right square
		dc->MoveTo(rect->right-2,rect->bottom-2);
		dc->LineTo(rect->right,rect->bottom-2);
		dc->MoveTo(rect->right+2,rect->bottom-2);
		dc->LineTo(rect->right+2,rect->bottom+1);
		dc->MoveTo(rect->right+2,rect->bottom+1);
		dc->LineTo(rect->right+2,rect->bottom+2);
		dc->LineTo(rect->right,rect->bottom+2);
		dc->MoveTo(rect->right-2,rect->bottom+2);
		dc->LineTo(rect->right,rect->bottom+2);

		wPen.CreatePen(PS_SOLID,1,RGB(255,255,255));
		dc->SelectObject(wPen);
		dc->MoveTo(rect->right+3,rect->bottom-3);
		dc->LineTo(rect->right-3,rect->bottom-3);
		dc->LineTo(rect->right-3,rect->bottom+3);
	

		pen.CreatePen(PS_SOLID,1,RGB(120,120,120));
		dc->SelectObject(pen);


		//dark gray middle rect
		if(rect->top >0 && !hideTop)
		{
			dc->MoveTo(rect->left-2,rect->top-1);
			dc->LineTo(rect->right+2,rect->top-1);
		}
		else
			rect->top +=2;

		dc->MoveTo(rect->right,rect->top-1);
		dc->LineTo(rect->right,rect->bottom+3);
		if(rect->left >0 && !hideLeft)
		{
			dc->MoveTo(rect->left-1,rect->bottom+1);
			dc->LineTo(rect->left-1,rect->top-2);
		}
		else
			rect->left +=3;
		dc->MoveTo(rect->right+2,rect->bottom);
		dc->LineTo(rect->left-3,rect->bottom);

	}

	rect->top = top;
	rect->left = left;

	dc->RestoreDC(dcID);

	pen.DeleteObject();
	wPen.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
//	OnGetDefBackColor
COLORREF ValidationCug::OnGetDefBackColor(int section)
{
	return GetSysColor ( COLOR_APPWORKSPACE );
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetFocus
void ValidationCug::OnSetFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKillFocus
void ValidationCug::OnKillFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapStart
BOOL ValidationCug::OnColSwapStart(int col)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanColSwap
BOOL ValidationCug::OnCanColSwap(int fromCol,int toCol)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTrackingWindowMoved
void ValidationCug::OnTrackingWindowMoved(RECT *origRect,RECT *newRect)
{
}
BOOL ValidationCug::MakeSortedResiduals()
{
	float accaccsum, accsum;
	double mcorr;
	int index, countvar;
	F* pValFile;
	CResiduals *pRes;
	float meas,  resvalue, sim;

	for(int i=0;i<MAXSIMVAL;i++) {
			pValFile=m_pSimDoc->ValidationFilePointer(i+1);
			if(pValFile->GetNumberOfSelectedVal()>0) {
				pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(i+1);
				pRes->ResetSortedResidualFile();
			}
	}



	
	for (long irun=0;irun<m_pSimDoc->MR_Get_TotalNumberofRuns(); irun++) {
			accaccsum=0;
			index=-1;
			countvar=0;
			for(int i=0;i<MAXSIMVAL;i++) {
				pValFile=m_pSimDoc->ValidationFilePointer(i+1);
				if(pValFile->GetNumberOfSelectedVal()>0) {
					CPG *pPG;
					pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(0, i+1, TRUE);
					if(pPG==nullptr) {
							pPG=new CPG;
					}
					pPG->ReOpen();

					pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(i+1);

					int ifile=m_pSimDoc->MR_GetAddedFiles(irun);
					long irun_int=irun;
					if(ifile!=0)  {
						pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(i+1, ifile);
						irun_int=m_pSimDoc->MR_GetInternalRunNo(irun);
					}
						

					pRes->InitSortedResidualFile();
					long numrec=pPG->GetNumRecords();
					long numvar=pPG->GetNumVariables();
					long nrec;
					accsum = 0;
						
						
					for (int jj=0;jj<numvar/2;jj++) {
						
						accsum=0;
						nrec=0;
						index++;	

						mcorr=0.0;
						for(long j=0;j<numrec;j++) {
							meas=pPG->GetVarValue(jj*2+2,j+1);
							if(abs(meas)<1.E38) {
								nrec++;
								resvalue=pRes->GetResiduals(irun_int, jj, j);
								//sim=meas-resvalue;

								*(m_pNewLogLi+j)=-resvalue;
							}
							else
								*(m_pNewLogLi+j)=float(MISSING);
						}
						StatUtil::SortArray(m_pNewLogLi, numrec);
						pRes->AddSortedResiduals(m_pNewLogLi, nrec);

						
					}

					accaccsum+=accsum;
					countvar++;
				}
			}
	}	
	
	for(int i=0;i<MAXSIMVAL;i++) {
			pValFile=m_pSimDoc->ValidationFilePointer(i+1);
			if(pValFile->GetNumberOfSelectedVal()>0) {
					pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(i+1);
					//pRes->ExitAddLogLiFile();
				
			}
	}
	return TRUE;
}




BOOL ValidationCug::EstimateNewLogLi()
{
	float accaccsum, accsum=0;
	double mcorr;
	int index, countvar;
	F* pValFile;
	CResiduals *pRes;
	float meas, meascorr, resvalue, sim;
	double lvalue;


	for(int i=0;i<MAXSIMVAL;i++) {
			pValFile=m_pSimDoc->ValidationFilePointer(i+1);
			if(pValFile->GetNumberOfSelectedVal()>0) {
				pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(i+1);
				pRes->ResetExtraLogLiFile();
			}
	}

// Apply coefficients to the Model domain

	m_pSw1->SetIntValue(m_LogFunc);
	//m_pSw1->EditApply();
	m_pSw2->SetIntValue(m_BoxCoxTrans);
	//m_pSw2->EditApply();

	for(int i=0;i<m_NumLines;i++) {
		m_pCPtError1->SetValue(i, m_pSimDoc->m_ValidationData.GetValP_ErrorNew(i) );	
		//m_pCPtError2->EditApply(i);
		m_pCPtError2->SetValue(i,m_pSimDoc->m_ValidationData.GetValA_ErrorNew(i) );
		//m_pCPtError2->EditApply(i);
		m_pCPt1->SetValue(i, m_pSimDoc->m_ValidationData.GetValBoxCox_PowerNew(i)   );
		//m_pCPt1->EditApply(i);
		m_pCPt2->SetValue(i, m_pSimDoc->m_ValidationData.GetValBoxCox_OffsetNew(i)	);
	//	m_pCPt2->EditApply(i);
	}
/*		CFileScanner ScanMeanProg(nullptr);
		ScanMeanProg.Create(IDD_PROGRESS,nullptr);
		ScanMeanProg.ShowWindow(SW_SHOW);
		ScanMeanProg.m_Info="New values of Log likelihoods will be calculated";
		*/
	// Make Calculations
	for (long irun=0;irun<m_pSimDoc->MR_Get_TotalNumberofRuns(); irun++) {
			accaccsum=0;
			index=-1;
			countvar=0;
			for(int i=0;i<MAXSIMVAL;i++) {
				pValFile=m_pSimDoc->ValidationFilePointer(i+1);
				if(pValFile->GetNumberOfSelectedVal()>0) {
					CPG *pPG;
					pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(0, i+1, TRUE);
					//pPG=(CPG*)pValFile->GetPointer();
					
					pPG->ReOpen();
					pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(i+1);
					pRes->InitExtraLogLiFile();
					long numrec=pPG->GetNumRecords();
					long numvar=pPG->GetNumVariables();
					long nrec;
					int varindex;
					float segment=100./float(m_pSimDoc->MR_Get_TotalNumberofRuns());
					float smallsegment=100./float(numvar)/float(m_pSimDoc->MR_Get_TotalNumberofRuns());
					float run_ratio=float(irun)/float(m_pSimDoc->MR_Get_TotalNumberofRuns());
						
					
					for (int jj=0;jj<numvar/2;jj++) {
						
						accsum=0;
						nrec=0;
						index++;	

						mcorr=0.0;
						for(long j=0;j<numrec;j++) {
							meas=pPG->GetVarValue(jj*2+2,j+1);
							if(abs(meas)<1.E38) {
								nrec++;
								resvalue=pRes->GetResiduals(irun, jj, j);
								if(mcorr!=0.0) 
									meascorr=double(meas)*pow(double(10.),mcorr);
								else
									meascorr=meas;
								sim=meas-resvalue;
								varindex=jj+1;
							//	lvalue=VALIDATION_mp_LOGLI_LL(&sim, &meascorr, &varindex);
								lvalue=m_pSimDoc->m_Simulator.m_StructureModel.m_Structure.SumLikelihood(size_t(jj), sim, meas);
								*(m_pNewLogLi+j)=lvalue;
							}
							else
								*(m_pNewLogLi+j)=float(MISSING);
						}
						pRes->AddLogLi(m_pNewLogLi, nrec);
						
					}

					accaccsum+=accsum;
					countvar++;
				}
			}
	}	

// Apply coefficients to the Model domain
	m_LogFunc=m_pSw1->GetOldIntValue();
	m_pSw1->SetIntValue(m_LogFunc);
	//m_pSw1->Apply();
	
	m_BoxCoxTrans=m_pSw1->GetOldIntValue();
	m_pSw2->SetIntValue(m_BoxCoxTrans);
	//m_pSw2->Apply();

	float v;
	for(int i=0;i<m_NumLines;i++) {
		v=m_pCPtError1->GetOldValue(i);
		if(v>MISSING) {
			m_pCPtError1->SetValue(i, v );	
			//m_pCPtError1->Apply(i);
		}
		v=m_pCPtError2->GetOldValue(i);
		if(v>MISSING) {	
			m_pCPtError2->SetValue(i,v );
			//m_pCPtError2->Apply(i);
		}
		
		v=m_pCPt1->GetOldValue(i);
		if(v>MISSING) {
			m_pCPt1->SetValue(i, v);
			//m_pCPt1->Apply(i);
		}

		v=m_pCPt2->GetOldValue(i);
		if(v>MISSING) {
			m_pCPt2->SetValue(i, v	);
			//m_pCPt2->Apply(i);
		}
	}

	//NewEstimations=TRUE;
	
	for(int i=0;i<MAXSIMVAL;i++) {
			pValFile=m_pSimDoc->ValidationFilePointer(i+1);
			if(pValFile->GetNumberOfSelectedVal()>0) {
					pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(i+1);

					pRes->ForcedUpdateofIndicators();
				//	pRes->UpdateStatisticsOfIndicators(FALSE,&ScanMeanProg);
				
			}
	}

	return TRUE;
}
BOOL ValidationCug::TimeSeriePlotClick(int sheet, int col, int row, long mrunno,  BOOL Add) 
{
		int itype, Index;

		CUGCell cell, cell1;
		SimB *Sw;

		int nselV=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
		Sw=nullptr;

		int ivar=1;
		int numvar;
		int ValFileNo;
		int Id_var;

	

		m_pCUG->UpdateCurrentChart(FALSE);
		
		int numvarprev=m_pSimDoc->m_PlotStorage.GetNumVar(m_pCUG->m_CurrentChart);


		CWinApp* pApp = AfxGetApp();
		int value=2;
		int ResidualPlot=MFC_Util::GetProfileIntNo( "ResidualPlot", value);
		if(ResidualPlot<=2) {
			if(sheet==9)
				MFC_Util::WriteProfileInt( "ResidualPlot", 2);
			else
				MFC_Util::WriteProfileInt( "ResidualPlot", 1);
		}
		int loglistyle=0;
		for(int ip=0; ip<ivar; ip++) {
					numvar=numvarprev+ip;
					itype=5;
					Index=0;
					GetCell(-1,row,&cell);
					Id_var=cell.GetParam();
					ValFileNo=m_pSimDoc->m_ValidationData.GetValFileNumber(Id_var);
					int ValFileIndex;
					ValFileIndex=-1;
					for(int jx=0;jx<=Id_var;jx++) {
						if(m_pSimDoc->m_ValidationData.GetValFileNumber(Id_var)==ValFileNo)
								ValFileIndex++;
					}
					
					if(sheet==9) {
					
						m_pSimDoc->m_PlotStorage.Add(Sw, itype, Index, m_pCUG->m_CurrentChart, m_pProp->m_ChartType,m_pSimDoc->GetCurrentSimFile(), ValFileNo, ValFileIndex, mrunno,0,numvar+1, 0);
						loglistyle=1;
					
					}


					m_pProp->SetFirstIndepVarType(MULTIRUN_NUM);
					if(!m_pSimDoc->m_PlotStorage.Add(Sw, itype, Index, m_pCUG->m_CurrentChart, m_pProp->m_ChartType,m_pSimDoc->GetCurrentSimFile(), ValFileNo, ValFileIndex, mrunno,0,numvar+1, loglistyle))
							m_pSimDoc->SetMultiStorageBinFileMissing();

					}

		

	
		if(m_pCUG->CreateZoomPlotWindow(FALSE)) {
			//SetCell(col,row, &cell);
			m_pCUG->UpdatePlotStatusBar(sheet, TRUE);
			return TRUE;
		}
		return FALSE;

}
BOOL ValidationCug::MultiPlotClick(int sheet, int col, int row, bool Add, bool All) 
{
		int  Index=1;
		CUGCell cell, cell1;
		SimB *Sw;
		int iv;
		int nselV=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
		iv=row;
		int ind_split=0;
		if(sheet>8) return FALSE;
		m_pPlotInfo->SetDoc(m_pSimDoc);

		Sw=nullptr;

		int ivar=1;
		int numvar;
		int ValFileIndex, ValFileNo;

		CCoupModelDoc *pDoc;
		MultiVAL mul;

		m_pCUG->UpdateCurrentChart(TRUE);
		
		numvar=m_pSimDoc->m_PlotStorage.GetNumVar(m_pCUG->m_CurrentChart);

		PLOTv plotinfo;	
		GetCell(col, row, &cell);
		if(!cell.GetBool()) Add=false;
		if(m_pSimDoc->m_SelectMultiVal.size()==0) m_pCUG->m_CurrentChart++;
		mul=GetMultiValStruct(row, col, sheet, &Add,0);

	/*	if(All) {
			for (int i = 1; i <= m_SplitPeriods; i++) {
				if (!m_pSimDoc->m_PlotStorage.Add(Sw, mul.Itype, mul.Index, m_pCUG->m_CurrentChart, m_pCUG->m_CurrentChartType, m_pSimDoc->GetCurrentSimFile(), ValFileNo, ValFileIndex, 0, i, numvar + 1))
					m_pSimDoc->SetMultiStorageBinFileMissing();
			}
		}*/
				
		for(int ii=0;ii<m_pSimDoc->m_SelectMultiVal.size();ii++) {
			if(m_pSimDoc->IsMultiSimulation()&&m_pSimDoc->IsFinishedSimulation()) {
				plotinfo=m_pPlotInfo->MakeMultiValChart(ii, 0);
				if(!m_pSimDoc->m_PlotStorage.AddPlotInfo(plotinfo,  m_pCUG->m_CurrentChart)) //, m_CurrentChartType,string(m_pSimDoc->GetCurrentSimFile()), ValFileNo, ValFileIndex, 0,0,numvar))
					m_pSimDoc->SetMultiStorageBinFileMissing();
			}
		}
		if(m_pSimDoc->m_SelectMultiVal.size()>0) 
		ivar=m_pSimDoc->m_SelectMultiVal.size();

/*		if(m_pCUG->CreateZoomPlotWindow(TRUE)) {
			GetCell(col,row, &cell);
			SetCell(col,row, &cell);
			m_pCUG->UpdatePlotStatusBar(sheet, TRUE);
			return TRUE;
		}*/
		return MakeMultiPlot(sheet, col, row);


}
int ValidationCug::GetMultiValPerformIndex(int row,int col, int sheet) {
	int index;
	CUGCell cell;
	int iv=row;
	if(m_SplitPeriods>0) {
		auto nselV=m_NumValidVar*(m_SplitPeriods+1);
		iv=row/(m_SplitPeriods+1);
		GetCell(1, row, &cell);
	}
	index=iv+sheet*m_NumValidVar;

	return index;
}
int ValidationCug::GetMultiValPerformSheet(int index) {
	if (m_NumValidVar != 0)
		return index / m_NumValidVar;
	else
		return index;
}
void ValidationCug::MakeSelectionsVisible(int Sheet) {
		CUGCell cell;
		for(size_t i=0; i<m_pSimDoc->m_SelectMultiVal.size();i++) {
				if(GetMultiValPerformSheet(m_pSimDoc->m_SelectMultiVal[i].Index)==Sheet) {
					auto col=1;
					auto row=m_pSimDoc->m_SelectMultiVal[i].row;
					GetCell(col,row,&cell);
					cell.SetBackColor(m_ColorEdit);
					cell.SetBool(TRUE);
					SetCell(col,row,&cell);
					RedrawCell(col,row);
				}
		}			
}
int ValidationCug::GetMultiValPerformSplitIndex(int row,int col) {
	int ind_split;
	CUGCell cell;
	ind_split=0;
	if(m_SplitPeriods>0) {
			auto nselV=m_NumValidVar*(m_SplitPeriods+1);
			auto iv=row/(m_SplitPeriods+1);
			GetCell(1, row, &cell);
			if(row%(m_SplitPeriods+1)>0) {
				auto txt=cell.GetLabelText();
				ind_split=MFC_Util::AtoInt(txt);
				if(ind_split<1||ind_split>m_SplitPeriods) ind_split=0;
			}
	}
	return ind_split;
}
MultiVAL ValidationCug::GetMultiValStruct(int row, int col,int sheet, bool *Add, int Indep) {
	MultiVAL out;
	CUGCell cell, cell1;
     
	 int nselV=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
	 int Index=0;
	 
		m_pSimDoc->m_PrevSheet=6;
			

			if(row<nselV*m_SplitPeriods||m_SplitPeriods==0) {
					

					Index=GetMultiValPerformIndex(row, col, sheet);
					out.PGSection=GetMultiValPerformSplitIndex(row,col);

					auto ValFileNo=m_pSimDoc->m_ValidationData.GetValFileNumber(row);
					auto ValFileIndex=-1;
					for(int jx=0;jx<=row;jx++) {
						if(m_pSimDoc->m_ValidationData.GetValFileNumber(row)==ValFileNo)
								ValFileIndex++;
					}					
			}
			else 
				out=m_pCUG->GetBlankMultiValStruct();
			if(*Add) {
				out.Index=Index;
				if (sheet == 6) sheet = 8;
				out.Performance_Var = SUM_PERFORMANCE(sheet);
				out.index_cycle = 1;
				out.row=row;
				out.col=col;
				out.type=DATA_TYPES::SIM_PERFORMANCE;
				out.subchart=m_pSimDoc->m_SelectMultiVal.size()+1;

				SIMB simb;
				if(row<nselV)
					simb=m_pSimDoc->m_ValidationData.GetSimPointer_TSV(row);
				else
					simb=m_pSimDoc->m_ValidationData.GetSimPointer_SV(row-nselV);

				out.pBase=simb.pBase;
				out.LIndex=simb.TabIndex;

				GetCell(out.col,out.row,&cell);
				if(Indep==0) {
					m_pSimDoc->m_SelectMultiVal.push_back(out);
					cell.SetBool(true);
					cell.SetBackColor(m_ColorEdit);
				}
				SetCell(out.col,out.row,&cell);
				RedrawCell(out.col,out.row);
				*Add=false;
			}
			else {
				for(size_t i=0; i<m_pSimDoc->m_SelectMultiVal.size();i++) {
					if(m_pSimDoc->m_SelectMultiVal[i].row==row) {
							m_pSimDoc->m_SelectMultiVal.erase(m_pSimDoc->m_SelectMultiVal.begin()+i);
							GetCell(col,row,&cell);
							if(Indep==0) {
								cell.SetBackColor(m_NoColor);
							}
							SetCell(col,row,&cell);
							RedrawCell(col,row);
					}
				}


				*Add=true;
				out=m_pCUG->GetBlankMultiValStruct();
			}

	return out;
}

void ValidationCug::EraseAllSelected() {
	while(m_pSimDoc->m_SelectMultiVal.size()>0) {
		m_pSimDoc->m_SelectMultiVal.erase(m_pSimDoc->m_SelectMultiVal.begin());
	}
}
bool ValidationCug::MakeMultiPlot(int sheet, int col, int row) {
		ChartCug *pChart;
		pChart=MySingle::instance().GetChartCugPointer();

		int Index;
		int isheet;
		int ivar;
		
		ivar=m_pSimDoc->m_SelectMultiVal.size();

		if(ivar==0) {

			pChart->MakeTextPlot("No Variables selected");
			return false;
		}


		CUGCell cell, cell1;	
		if(m_pCUG->CreateZoomPlotWindow(true)) {
					for(int ip=0;ip<ivar;ip++) {
							Index=m_pSimDoc->m_SelectMultiVal[ip].Index;
							col=m_pSimDoc->m_SelectMultiVal[ip].col;
							row=m_pSimDoc->m_SelectMultiVal[ip].row;
						}
						/*else {
							col=2;
							row=m_pSimDoc->m_SelectMultiPar[ip].row;
						}*/
						GetCell(col,row, &cell);
					//	cell.SetBool(false);
						cell.SetBackColor(RGB(80,240,240));
						SetCell(col,row, &cell);
						RedrawCell(col,row);
			} 
			else if(m_pProp->m_ChartType ==CHART_TYPES::SCATTER)
				cell.SetBackColor(RGB(255,40,255));
			else
				cell.SetBackColor(RGB(240,0,0));
				

        m_pCUG->UpdatePlotStatusBar(sheet, true);

 return true;
}
BOOL ValidationCug::DefineNewSubPeriod() 
{
	float accaccsum, accsum;
	double mcorr;
	int index, countvar;
	F* pValFile;
	CResiduals *pRes;
	float meas;
	long start_min, end_min, SplitPeriods;
	size_t filter_index;
	
	if (m_pSimDoc->MR_CheckIndexForFile_RepMode()) {	
		if (m_pSimDoc->m_MultiFileSplitMode) {
			SplitPeriods = 0;
			m_pSimDoc->m_MultiFileSplitMode = false;
		}
		else {
			m_pSimDoc->m_MultiFileSplitMode = true;
			SplitPeriods = m_pSimDoc->MR_GetNumOfRepeatitionsOfDim(0);
		}
	}
	else {
		auto filterindex = [&]() {
			for (auto valv : m_pSimDoc->m_Val_Array) {
				auto test_index = valv.Name.find("Filter_Id");
				if (test_index != std::string::npos) return test_index;
			}
			return std::string::npos;
		};
		auto testindex = filterindex();
		if (m_SplitPeriods > 0) {
			m_FilterCase.clear();
			SplitPeriods = 0;
			m_NumberNewSplitPeriods = 0;
		}
		else {
			if (testindex != std::string::npos) {
				auto FileNo = m_pSimDoc->m_ValidationData.GetValFileNumber(testindex);
				filter_index = testindex;
				pValFile = m_pSimDoc->ValidationFilePointer(FileNo);
				auto ind = m_pSimDoc->m_ValidationData.GetValFileIndex(0);
				CPG* pPG;
				pPG = (CPG*)pValFile->GetPointer();
				if (pPG->ReOpen()) {
					for (size_t irec = 0; irec < pPG->GetNumRecords(); irec++) {
						int test = int(pPG->GetVarValue(ind + 1, irec + 1) + 0.001);
						bool koll = false;
						for (auto val : m_FilterCase) {
							if (test == val) {
								koll = true;
								break;
							}
						}
						if (!koll) m_FilterCase.push_back(test);
					}
					SplitPeriods = m_FilterCase.size();
					m_NumberNewSplitPeriods = SplitPeriods;
				}
				else
					return FALSE;



			}
			else
				SplitPeriods = m_NumberNewSplitPeriods;
		}
	}
	CPG *pPG;

	EraseAllSelected();
	m_pSimDoc->m_MStorage.AlloSplitPerformance(SplitPeriods);

	if(SplitPeriods<=0) {
		MFC_Util::MessageBox("Reset to no split periods mode !");
		OnSheetSetup(0);
		return TRUE;
	}

	if(!m_pSimDoc->m_MStorage.m_IsSplitOpen||!m_pSimDoc->m_MStorage.m_SplitPeriodFile.good()) {

		MFC_Util::MessageBox("Problems with definition of file to store results!");
		OnSheetSetup(0);
		return TRUE;
	}

	long previous=-1;

	vector<size_t> recordsa;
	vector<CResiduals*> ResidualPtr;
	vector<CPG*> PGPtr;
	vector<size_t> Valid;
	Valid.resize(MAXSIMVAL);
	PGPtr.resize(MAXSIMVAL);
	vector<size_t> ResidualIndex;
	string str=m_pSimDoc->GetMBinFileName();
	int addedfiles=m_pSimDoc->m_MStorage.GetNumberOfAddedFiles(string(str));
	recordsa.resize(MAXSIMVAL*(SplitPeriods+1));
	ResidualPtr.resize(MAXSIMVAL*(addedfiles+1));
	ResidualIndex.resize(MAXSIMVAL);
	countvar=0;

	

	for(int i=0;i<MAXSIMVAL;i++) {
				pValFile=m_pSimDoc->ValidationFilePointer(i+1);
				Valid[i]=0;
				ResidualIndex[i]=0;
				if(pValFile->GetNumberOfSelectedVal()>0) {
					Valid[i]=1;
					countvar+=pValFile->GetNumberOfSelectedVal();
				
					pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(0, i+1,0, TRUE);
					PGPtr[i]=pPG;
					//pPG=(CPG*)pValFile->GetPointer();
					if(pPG==nullptr) {
							string name=m_pSimDoc->GetOutputFileName(i+1);
							pPG=new CPG;
							pPG->Open(name);
							pPG->CloseFile();
					}

					PGPtr[i]=pPG;
					pPG->SetCompleteRead(true);
					pPG->ReOpen();

					auto num_repitions = pPG->GetNumRepititions();
	
					long numrec=pPG->GetNumRecords()*num_repitions;
					if (m_FilterCase.size()>0) {
						


					}
					else {
						for (int j = 0; j <= SplitPeriods; j++) {
							int indexrec = i * (SplitPeriods + 1) + j;
							recordsa[indexrec] = numrec / SplitPeriods * j;
						}
						for (int j = 0; j <= addedfiles; j++) {
							int index = (addedfiles + 1) * i + j;

							if (j != 0) {
								pRes = (CResiduals*)m_pSimDoc->GetResidualPointer(i + 1, j);
								ResidualPtr[index] = pRes;
								if (!pRes->IsOpen()) {
									int f = 0;

								}
							}
							else {
								pRes = (CResiduals*)m_pSimDoc->GetResidualPointer(i + 1);
								ResidualPtr[index] = pRes;
								if (!pRes->IsOpen()) {
									int f = 0;

								}
							}
						}
					}
				}
	}
	if (m_FilterCase.size() > 0) SplitPeriods = m_FilterCase.size();
	accaccsum=0;
	index=-1;
	
	for (int i = 0; i < m_NumValidVar; i++) {
		auto test = FUtil::STD_ItoAscii(i + 1);
		auto FileNo = m_pSimDoc->m_ValidationData.GetValFileNumber(i);
		pValFile = m_pSimDoc->ValidationFilePointer(FileNo);
		auto ind = m_pSimDoc->m_ValidationData.GetValFileIndex(i);
		CPG* pPG;
		BOOL valid;
		pPG = (CPG*)pValFile->GetPointer();
		pPG->SetCompleteRead(true);
		pPG->ReOpen();
		if (pPG == nullptr) {


			pPG = new CPG;
			//CPG_Array.Add(pCPG);
			pPG->SetCompleteRead(true);
			valid = pPG->ReadContentOfPGFileAndOpenMainPGStreamForReading(pValFile->GetStrValue().c_str());
			if (valid) pValFile->SetPointer(pPG);
			pPG->CloseFile();
		}
		test = pValFile->GetDataDescription(ind).c_str();
		long numrec = pPG->GetNumRecords();
		long numvar = pPG->GetNumVariables();
		auto start_min = pPG->GetLongTimeStart();
		auto end_min = pPG->GetLongTimeEnd();
		auto num_repitions = pPG->GetNumRepititions();
		size_t numpar_calib = 0;
		if (num_repitions > 1) { // if repitionsfile check type of simulation
			if (m_pSimDoc->IsMultiSimulation()) { //if multisimulation check value of parameter Val
				//"Index for selection of File parameter in multirun options				
				size_t index_split_par;
				for (int i = 0; i < m_pSimDoc->MR_GetNumberOfDimensions(); i++) {
					for (int ii = 0; ii < m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i + 1); ii++) {
						auto pPar = m_pSimDoc->MR_Get_pPar(i + 1, ii + 1);
						auto nTabIndex = m_pSimDoc->MR_GetTabIndex(i + 1, ii + 1);
						string namep = pPar->GetName();
						if (namep.find("Index for selection of File") != std::string::npos) {
							index_split_par = numpar_calib;
						}
						numpar_calib++;
					}
				}
			}
		}
		long nrec;
		accsum = 0;
		nrec = 0;
		index++;
		mcorr = 0.0;
		std::vector <float> meas_v, res_v, sim_v, filter_v;
		if (numpar_calib != 0) {
			pPG->SetRepititionMode(true);
			if (SplitPeriods == num_repitions) {
				meas_v.resize(numrec);
				res_v.resize(numrec);
				sim_v.resize(numrec);
				int NumRuns_Repitions = m_pSimDoc->m_MStorage.GetNumberOfRuns() / num_repitions;
					int irep;
					for (size_t irun = 0; irun < m_pSimDoc->m_MStorage.GetNumberOfRuns(); irun++) {
						long irun_int;
						int ifile = m_pSimDoc->MR_GetAddedFiles(irun);
						auto pRes = (CResiduals*)m_pSimDoc->GetResidualPointer(FileNo);
						if (ifile > 0) pRes = (CResiduals*)m_pSimDoc->GetResidualPointer(FileNo, ifile);
						if (ifile != 0) {
							irun_int = m_pSimDoc->MR_GetInternalRunNo(irun);
						}
						else {
							irun_int = irun;
						}
						
						irep = irun % num_repitions;
						pPG->SetRepititionSequence(irep, false);
						meas_v = pPG->GetVarVector(ind + 1);

						auto kollsimindex = ResidualIndex[FileNo - 1]+i;

						res_v = pRes->GetResidualVector(irun_int, kollsimindex);

						STAT_PERF split;
						double AccSim = 0., AccMeas = 0.;
						vector<double> sim, obs;
						for (long ii = 0; ii < numrec; ii++) {
							sim_v[ii] = meas_v[ii] - res_v[ii];
							auto meas = meas_v[ii];
							if (abs(meas) < .9E38 && abs(res_v[ii]) < 0.9E38) {
								sim.push_back(sim_v[ii]);
								obs.push_back(meas);
							}
						}
						split = StatUtil::Get_StatComp(&sim, &obs);
						split.logli = m_pSimDoc->m_Simulator.m_StructureModel.m_Structure.SumLikelihood(size_t(i), &sim, &obs);
						auto irun_store = irun / num_repitions;
						m_pSimDoc->m_MStorage.SetPerformance(irun_store, irep, split);
					}
			}
			else {
				string message = "Split periods failure check number of repitions in Input file :";
				MFC_Util::MessageBox(message);

			}
			m_pSimDoc->m_MStorage.VarValidAllo();
			m_pSimDoc->m_MStorage.ReOpenSplitFile();
			m_pSimDoc->m_MStorage.SaveSplitData(unsigned long(i));
		}
		else if (m_FilterCase.size() > 0) {
			res_v.resize(numrec);
			meas_v.resize(numrec);
			filter_v.resize(numrec);
			
			meas_v = pPG->GetVarVector(ind + 1);
			filter_v = pPG->GetVarVector(filter_index + 1);
			for (long irun = 0; irun < m_pSimDoc->m_MStorage.GetNumberOfRuns(); irun++) {
			long irun_int;
			int ifile = m_pSimDoc->MR_GetAddedFiles(irun);
			auto pRes = (CResiduals*)m_pSimDoc->GetResidualPointer(FileNo);
			if (ifile > 0) pRes = (CResiduals*)m_pSimDoc->GetResidualPointer(FileNo, ifile);
			if (ifile != 0) {
				irun_int = m_pSimDoc->MR_GetInternalRunNo(irun);
			}
			else {
				irun_int = irun;
			}
			res_v = pRes->GetResidualVector(irun_int, ResidualIndex[FileNo - 1]);
				sim_v.resize(numrec);
				for (long ii = 0; ii < numrec; ii++)
					sim_v[ii] = meas_v[ii] - res_v[ii];

				vector<double> sim, obs;
				STAT_PERF split;
				for (int index_split = 0; index_split < SplitPeriods; index_split++) {
					sim.clear(); obs.clear();

					for (long ii = 0; ii < numrec; ii++) {
						sim_v[ii] = meas_v[ii] - res_v[ii];
						auto meas = meas_v[ii];
						if (int(filter_v[ii]) == m_FilterCase[index_split]) {
							if (abs(meas) < .9E38 && abs(res_v[ii]) < 0.9E38) {
								sim.push_back(sim_v[ii]);
								obs.push_back(meas);
							}
						}
					}
					split = StatUtil::Get_StatComp(&sim, &obs);
					split.logli = m_pSimDoc->m_Simulator.m_StructureModel.m_Structure.SumLikelihood(size_t(i), &sim, &obs);
					m_pSimDoc->m_MStorage.SetPerformance(irun, index_split, split);
				}





			}

			m_pSimDoc->m_MStorage.VarValidAllo();
			m_pSimDoc->m_MStorage.ReOpenSplitFile();
			m_pSimDoc->m_MStorage.SaveSplitData(unsigned long(i));

		}
		else { // split by equal long subperiods
			res_v.resize(numrec);
			meas_v.resize(numrec);
			meas_v = pPG->GetVarVector(ind + 1);
			for (long irun = 0; irun < m_pSimDoc->m_MStorage.GetNumberOfRuns(); irun++) {


				long irun_int;
				int ifile = m_pSimDoc->MR_GetAddedFiles(irun);
				auto pRes = (CResiduals*)m_pSimDoc->GetResidualPointer(FileNo);
				if (ifile > 0) pRes = (CResiduals*)m_pSimDoc->GetResidualPointer(FileNo, ifile);
				if (ifile != 0) {
					irun_int = m_pSimDoc->MR_GetInternalRunNo(irun);
				}
				else {
					irun_int = irun;
				}
				res_v = pRes->GetResidualVector(irun_int, ResidualIndex[FileNo - 1]);
				sim_v.resize(numrec * num_repitions);
				for (long ii = 0; ii < numrec * num_repitions; ii++)
					sim_v[ii] = meas_v[ii] - res_v[ii];
				STAT_PERF split;
				double AccSim = 0., AccMeas = 0.;
				vector<double> sim, obs;
				for (int index_split = 0; index_split < SplitPeriods; index_split++) {
					int indexrec = i * (SplitPeriods + 1) + index_split;
					//VALIDATION_mp_POSTPREPARE();
					sim.clear(); obs.clear();
					//for(unsigned long j=0;j<numrec;j++)
					for (size_t j = recordsa[indexrec]; j < recordsa[indexrec + 1]; j++) {
						auto meas = meas_v[j];
						if (abs(meas) < .9E38 && abs(res_v[j]) < 0.9E38) {
							sim.push_back(sim_v[j]);
							obs.push_back(meas);
						}
					}
					split = StatUtil::Get_StatComp(&sim, &obs);
					split.logli = m_pSimDoc->m_Simulator.m_StructureModel.m_Structure.SumLikelihood(size_t(i), &sim, &obs);
					m_pSimDoc->m_MStorage.SetPerformance(irun, indexrec, split);
				}
				ResidualIndex[FileNo - 1]++;
			}

		}
	}


	if(SplitPeriods>0) {

		m_pSimDoc->m_MStorage.UpdateAcceptedRuns(TRUE);
		m_pSimDoc->m_MStorage.Set_Update_ValidVar();
		m_pSimDoc->m_MStorage.SetAllToAcceptedRuns();

		MFC_Util::MessageBox("Performance indicators estimated");
		OnSheetSetup(0);
	}
	return TRUE;
}
void ValidationCug::FixZeroShiftOfPostMean(size_t ivar) {


	vector<float> dist;
	int ip=3*m_pSimDoc->GetNumAll_TSV_ValVariables()+ivar;
	dist = m_pSimDoc->m_MStorage.GetPost_TSV_ValidPerformanceIndicatorVector(ip, 0);
	int icount=dist.size();
	StatUtil::SortArray(&dist[0],icount );
	float meanvalue;
	meanvalue=StatUtil::mean(dist);
	if(meanvalue>0) {
		while(meanvalue>0&&dist.size()>3) {
			dist.erase(dist.end()-1);
			meanvalue=StatUtil::mean(dist);
		}
	}
	else {
		while(meanvalue<0&&dist.size()>3) {
			dist.erase(dist.begin());
			meanvalue=StatUtil::mean(dist);
		}
	}
	if(dist.size()>2) {
		m_pSimDoc->m_MStorage.SetCriteriaHistory(dist.size() - dist.size()/2, A1_STAT);
			/*			m_pSimDoc->m_MStorage.SetValidLow(iv+index_start, ind_split, value);
				if(m_pSimDoc->m_MStorage.GetCriteria(iv+index_start, ind_split)==0)
					m_pSimDoc->m_MStorage.SetCriteria(iv+index_start, 1, ind_split);*/
		float newmin=dist[0]-abs(dist[0])/1000.;
		float newmax=dist[dist.size()-1]+abs(dist[dist.size()-1])/1000.;
		m_pSimDoc->m_MStorage.SetValidLow(ip,0, newmin);
		 //int criteria=m_pSimDoc->m_MStorage.GetMeanCriteria(3);
		//m_pSimDoc->m_MStorage.SetValidLow(ip,0, newmin);
		if(m_pSimDoc->m_MStorage.GetCriteria(ip,0)==0)
			m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_MIN);
		else
			m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_AND_BELOW);

		//m_pSimDoc->m_MStorage.SetMeanValidHigh_AbsME(newmax);
		m_pSimDoc->m_MStorage.SetValidHigh(ip,0, newmax);
		if(m_pSimDoc->m_MStorage.GetCriteria(ip,0)==0)
			m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::BELOW_MAX);
		else
			m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_AND_BELOW);


			m_pSimDoc->m_MStorage.UpdateAcceptedRuns(false, false);
			m_Changed=TRUE;
			m_SavedCriteria2File=false;
			m_pSimDoc->MR_ReCalculatePostDist(false);

			for(int i=0;i<MAXSIMVAL;i++) {
				CResiduals *pRes;
				pRes=m_pSimDoc->GetValidResidualPointer(i+1);
				if(pRes!=nullptr) {
					pRes->ToBeUpdated();
					pRes->m_MeanDefined=false;
				}
			}

			m_Changed=FALSE;
			m_Updated=TRUE;

			
			for(int i=0;i<6;i++)
				Make_MainSheets(i);
			Make_CriteriaSheet();
			Make_OptionSheet();
			OnSheetSetup(3);
			SetSheetNumber(3);
	}

	return;
}
void ValidationCug::ReducebyR2(size_t ivar, size_t n_left, bool SV_Variable) {

	MFC_Util m;
	m.SetPaneMessage("Wait when posterior ensemble is reduced to " + FUtil::STD_ItoAscii(n_left) + " runs");
	vector<float> dist;
	int ip=ivar;
	bool MeanOfMany = false;
	if (ivar == m_pSimDoc->GetNumAll_TSV_ValVariables()) MeanOfMany = true;

	if (!SV_Variable) {
		if (MeanOfMany) {
			dist = m_pSimDoc->m_MStorage.m_MeanTSV_Accepted[R2_STAT];
		}
		else {
			dist = m_pSimDoc->m_MStorage.GetPost_TSV_ValidPerformanceIndicatorVector(ip, 0);
			ip = ivar;
		}
	}
	else {
		dist = m_pSimDoc->m_MStorage.GetPost_SV_ValidPerformanceIndicatorVector(ivar, R2_STAT);
		ip = ivar;
	}


	int icount=dist.size();
	if (icount == 0) return;
	float lastvalid=0.f;
	StatUtil::SortArray(&dist[0],icount );
	size_t n_toremove = dist.size() - n_left-1;
	dist.erase(dist.begin(),dist.begin() + n_toremove);
	lastvalid = dist[0];

	if (dist.size() > n_left - 1) {
			
			if (!SV_Variable) {
				auto no = m_pSimDoc->m_MStorage.SetCriteriaHistory(dist.size(), R2_STAT);
				if (MeanOfMany) {
					m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_LOW][LOGLI_STAT] = lastvalid;
					m_pSimDoc->m_MStorage.SetMeanCriteria(LOGLI_STAT, CRITERIA_STATE::ABOVE_MIN);
				}
				else {
					m_pSimDoc->m_MStorage.SetValidLow(ip, 0, lastvalid);
					if (m_pSimDoc->m_MStorage.GetCriteria(ip, 0) == 0)
						m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_MIN);
					else
						m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_AND_BELOW);
				}
			}
			else {
				if (ivar == m_pSimDoc->m_ValSum_Array.size()) {
					m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_LOW][R2_STAT] = lastvalid;
					m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].criteria[R2_STAT] = CRITERIA_STATE::ABOVE_MIN;

				}
				else {
					m_pSimDoc->m_MStorage.m_Valid_SV[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_LOW][ivar] = lastvalid;
					if (m_pSimDoc->m_MStorage.GetSumCriteria(ivar) == 0)
						m_pSimDoc->m_MStorage.SetSumCriteria(ivar, CRITERIA_STATE::ABOVE_MIN);
					else
						m_pSimDoc->m_MStorage.SetSumCriteria(ivar, CRITERIA_STATE::ABOVE_AND_BELOW);
				}
			}

			ReduceUpdateStep(R2_STAT);
	
	}
	return;
}
void ValidationCug::ReducebyRMSE(size_t ivar, size_t n_left, bool SV_Variable) {
	vector<float> dist;
	int ip;
	bool MeanOfMany = false;
	if (ivar == m_pSimDoc->GetNumAll_TSV_ValVariables()) MeanOfMany = true;

	if (!SV_Variable) {
		if (MeanOfMany) {
			dist = m_pSimDoc->m_MStorage.m_MeanTSV_Accepted[RMSE_STAT];
		}
		else {
			ip = ivar + m_pSimDoc->GetNumAll_TSV_ValVariables() * RMSE_STAT;
			dist = m_pSimDoc->m_MStorage.GetPost_TSV_ValidPerformanceIndicatorVector(ip, 0);
		}

	}
	else {
		dist = m_pSimDoc->m_MStorage.GetPost_SV_ValidPerformanceIndicatorVector(ivar, RMSE_STAT);
		ip = ivar + m_pSimDoc->m_ValSum_Array.size() *RMSE_STAT;
	}

	int icount=dist.size();
	float lastvalid=0.f;
	StatUtil::SortArray(&dist[0],icount );
	while(dist.size()>n_left-1) {
			lastvalid=dist[dist.size()-1];
			dist.erase(dist.end()-1);
	}
	if(dist.size()>=n_left-1) {
		if ((!SV_Variable)) {
			auto no = m_pSimDoc->m_MStorage.SetCriteriaHistory(dist.size(),RMSE_STAT);
			if (MeanOfMany) {
				m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_HIGH][RMSE_STAT] = lastvalid;
				m_pSimDoc->m_MStorage.SetMeanCriteria(RMSE_STAT, CRITERIA_STATE::BELOW_MAX);
			}
			else {
				m_pSimDoc->m_MStorage.SetValidHigh(ip, 0, lastvalid);
				if (m_pSimDoc->m_MStorage.GetCriteria(ip, 0) == 0)
					m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::BELOW_MAX);
				else
					m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_AND_BELOW);
			}
		}
		else {
			if (ivar == m_pSimDoc->m_ValSum_Array.size()) {
				m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_HIGH][RMSE_STAT] = lastvalid;
				m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].criteria[RMSE_STAT] = CRITERIA_STATE::ABOVE_AND_BELOW;

			}
			else {
				m_pSimDoc->m_MStorage.m_Valid_SV[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_HIGH][ip] = lastvalid;
				m_pSimDoc->m_MStorage.SetSumCriteria(ip, CRITERIA_STATE::ABOVE_AND_BELOW);
			}
		}

		ReduceUpdateStep(RMSE_STAT);

	}
	return;
}
void ValidationCug::ReducebyNSE(size_t ivar, size_t n_left, bool SV_Variable) {


	vector<float> dist;
	int ip;
	bool MeanOfMany = false;
	if (ivar == m_pSimDoc->GetNumAll_TSV_ValVariables()) MeanOfMany = true;
	if (!SV_Variable) {
		if (MeanOfMany) {
			dist = m_pSimDoc->m_MStorage.m_MeanTSV_Accepted[NSE_STAT];
		}
		else {
			ip = ivar + m_pSimDoc->GetNumAll_TSV_ValVariables() * NSE_STAT;
			dist = m_pSimDoc->m_MStorage.GetPost_TSV_ValidPerformanceIndicatorVector(ip, 0);
		}
	}
	else {
		dist = m_pSimDoc->m_MStorage.GetPost_SV_ValidPerformanceIndicatorVector(ivar, NSE_STAT);
		ip = ivar + m_pSimDoc->m_ValSum_Array.size() * NSE_STAT;
	}

	int icount=dist.size();
	float lastvalid=0.f;
	StatUtil::SortArray(&dist[0],icount );
		while(dist.size()>n_left-1) {
			lastvalid=dist[0];
			dist.erase(dist.begin());
		}
	if(dist.size()>n_left-2) {
		if (!SV_Variable) {
			m_pSimDoc->m_MStorage.SetCriteriaHistory(dist.size() - n_left, NSE_STAT);
			if (MeanOfMany) {
				m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_LOW][NSE_STAT] = lastvalid;
				m_pSimDoc->m_MStorage.SetMeanCriteria(NSE_STAT, CRITERIA_STATE::ABOVE_MIN);
			}
			else {

				m_pSimDoc->m_MStorage.SetValidLow(ip, 0, lastvalid);
				if (m_pSimDoc->m_MStorage.GetCriteria(ip, 0) == 0)
					m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_MIN);
				else
					m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_AND_BELOW);


			}
		}
		else {
			if (ivar == m_pSimDoc->m_ValSum_Array.size()) {
				m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_LOW][NSE_STAT] = lastvalid;
				m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].criteria[NSE_STAT] = CRITERIA_STATE::ABOVE_MIN;
			}
			else {
				m_pSimDoc->m_MStorage.m_Valid_SV[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_LOW][ip] = lastvalid;
				m_pSimDoc->m_MStorage.SetSumCriteria(ip, CRITERIA_STATE::ABOVE_MIN);
			}
		}
		ReduceUpdateStep(NSE_STAT);
	}
	return;
}
void ValidationCug::ReducebyLogLi(size_t ivar, size_t n_left, bool SV_Variable) {

	vector<float> dist;
	int ip;
	bool MeanOfMany = false;
	if (ivar == m_pSimDoc->GetNumAll_TSV_ValVariables()) MeanOfMany = true;


	if (!SV_Variable){
		if (MeanOfMany) {
			dist = m_pSimDoc->m_MStorage.m_MeanTSV_Accepted[LOGLI_STAT];
		}
		else {
			ip = ivar + m_pSimDoc->GetNumAll_TSV_ValVariables() * LOGLI_STAT;
			dist = m_pSimDoc->m_MStorage.GetPost_TSV_ValidPerformanceIndicatorVector(ip, 0);
		}
	}
	else {
		ip = ivar + m_pSimDoc->m_ValSum_Array.size() * LOGLI_STAT;
		dist = m_pSimDoc->m_MStorage.GetPost_SV_ValidPerformanceIndicatorVector(ivar, LOGLI_STAT);
	}

	//dist=m_pSimDoc->m_MStorage.GetPost_TSV_ValidPerformanceIndicatorVector(ip,0);
	int icount=dist.size();
	float lastvalid=0.f;
	StatUtil::SortArray(&dist[0],icount );
	while(dist.size()>n_left-1) {
			lastvalid=dist[0];
			dist.erase(dist.begin());
	}



	if(dist.size()>n_left-2) {
		if (!SV_Variable) {
			m_pSimDoc->m_MStorage.SetCriteriaHistory(dist.size() - n_left, LOGLI_STAT);
			if (MeanOfMany) {
				m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_LOW][LOGLI_STAT] = lastvalid;
				m_pSimDoc->m_MStorage.SetMeanCriteria(LOGLI_STAT,CRITERIA_STATE::ABOVE_MIN);
			}
			else {
				m_pSimDoc->m_MStorage.SetValidLow(ip, 0, lastvalid);
					if (m_pSimDoc->m_MStorage.GetCriteria(ip, 0) == 0)
						m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_MIN);
					else
						m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_AND_BELOW);
			}
		}
		else {
			if (ivar == m_pSimDoc->m_ValSum_Array.size()) {
				m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_LOW][LOGLI_STAT] = lastvalid;
				m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].criteria[LOGLI_STAT] = CRITERIA_STATE::ABOVE_MIN;

			}
			else {
				m_pSimDoc->m_MStorage.m_Valid_SV[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_LOW][ip] = lastvalid;
				if (m_pSimDoc->m_MStorage.GetSumCriteria(ip) == 0)
					m_pSimDoc->m_MStorage.SetSumCriteria(ip, CRITERIA_STATE::BELOW_MAX);
				else
					m_pSimDoc->m_MStorage.SetSumCriteria(ip, CRITERIA_STATE::ABOVE_AND_BELOW);
			}

		}
		
		ReduceUpdateStep(LOGLI_STAT);

	}
	return;
}

void ValidationCug::ReducebyMeanInterval(size_t ivar, size_t n_left, bool SV_Variable) {
	int ip;
	bool MeanOfMany = false;
	if (ivar == m_pSimDoc->GetNumAll_TSV_ValVariables()) MeanOfMany = true;
	vector<float> distin, distout;

	if (!SV_Variable) {

		if (MeanOfMany) {
			distin = m_pSimDoc->m_MStorage.m_MeanTSV_Accepted[ABSME_STAT];
		}
		else {
			ip = ivar + m_pSimDoc->GetNumAll_TSV_ValVariables() * ABSME_STAT;
			distin = m_pSimDoc->m_MStorage.GetPost_TSV_ValidPerformanceIndicatorVector(ip, 0);
		}
	}
	else {
		ip = ivar + m_pSimDoc->m_ValSum_Array.size() * ABSME_STAT;
		distin = m_pSimDoc->m_MStorage.GetPost_SV_ValidPerformanceIndicatorVector(ivar, ABSME_STAT);
	}

	
	int icount=distin.size();
	float lastvalid;
	StatUtil::SortArray(&distin[0],icount );

	float meanvaluein, meanvalueout;
	meanvaluein=StatUtil::mean(distin);
	size_t npos=0;
	for(size_t i=0; i<n_left; i++) 
		distout.push_back(distin[i+npos]);

	meanvalueout=StatUtil::mean(distout);

	while(meanvalueout>0&&npos>0) {
		distout.erase(distout.end()-1);
		npos--;
		distout.emplace(distout.begin(), distin[npos]);
		meanvalueout=StatUtil::mean(distout);
	}
	while(meanvalueout<0&&npos<distin.size()-n_left-1) {
		distout.erase(distout.begin());
		npos++;
		distout.push_back(distin[npos+n_left-1]);
		meanvalueout=StatUtil::mean(distout);
	}


	if(distout.size()>n_left-2) {
		m_pSimDoc->m_MStorage.SetCriteriaHistory(distout.size(), ABSME_STAT);
		if (!SV_Variable) {
			if (MeanOfMany) {
				m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_LOW][A1_STAT] = distout[0];
				m_pSimDoc->m_MStorage.SetMeanCriteria(A1_STAT, CRITERIA_STATE::ABOVE_AND_BELOW);
				m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_HIGH][A1_STAT] = distout[distout.size() - 1];
			}
			else {

				m_pSimDoc->m_MStorage.SetValidLow(ip, 0, distout[0]);
				if (m_pSimDoc->m_MStorage.GetCriteria(ip, 0) == 0)
					m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_MIN);
				else
					m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_AND_BELOW);

				m_pSimDoc->m_MStorage.SetValidHigh(ip, 0, distout[distout.size() - 1]);
				if (m_pSimDoc->m_MStorage.GetCriteria(ip, 0) == 0)
					m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::BELOW_MAX);
				else
					m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_AND_BELOW);
			}
		}
		else {
			if (ivar == m_pSimDoc->m_ValSum_Array.size()) {
				m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_LOW][ABSME_STAT] = distout[0];
				m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_HIGH][ABSME_STAT] = distout[distout.size() - 1];
				m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].criteria[ABSME_STAT] = CRITERIA_STATE::ABOVE_AND_BELOW;

			}
			else {
				m_pSimDoc->m_MStorage.m_Valid_SV[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_LOW][ip] = distout[0];
				m_pSimDoc->m_MStorage.m_Valid_SV[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_HIGH][ip] = distout[distout.size() - 1];
				m_pSimDoc->m_MStorage.SetSumCriteria(ip, CRITERIA_STATE::ABOVE_AND_BELOW);
			}

		}
		ReduceUpdateStep(ABSME_STAT);
	}
	return;
}

void ValidationCug::ReducebySlopeInterval(size_t ivar, size_t n_left, bool SV_Variable) {


	int ip;
	bool MeanOfMany = false;
	if (ivar == m_pSimDoc->GetNumAll_TSV_ValVariables()) MeanOfMany = true;

	vector<float> distin, distout;	
	float meanvaluein, meanvalueout;

	if (!SV_Variable) {
		if (MeanOfMany) {
			distin = m_pSimDoc->m_MStorage.m_MeanTSV_Accepted[A1_STAT];
		}
		else {
			ip = ivar + m_pSimDoc->GetNumAll_TSV_ValVariables() * A1_STAT;
			distin = m_pSimDoc->m_MStorage.GetPost_TSV_ValidPerformanceIndicatorVector(ip, 0);
		}
	}
	else {

		ip = ivar + m_pSimDoc->m_ValSum_Array.size() * 2;
		distin = m_pSimDoc->m_MStorage.GetPost_SV_ValidPerformanceIndicatorVector(ivar, A1_STAT);

	}

	int icount=distin.size();
	StatUtil::SortArray(&distin[0],icount );

	meanvaluein=StatUtil::mean(distin);
	size_t npos=0;
	for(size_t i=0; i<n_left; i++) 
		distout.push_back(distin[i+npos]);

	meanvalueout=StatUtil::mean(distout);

	while(meanvalueout>1&&npos>0) {
		distout.erase(distout.end()-1);
		npos--;
		distout.emplace(distout.begin(), distin[npos]);
		meanvalueout=StatUtil::mean(distout);
	}
	while(meanvalueout<1&&npos<distin.size()-n_left-1) {
		distout.erase(distout.begin());
		npos++;
		distout.push_back(distin[npos+n_left-1]);
		meanvalueout=StatUtil::mean(distout);
	}



	if(distout.size()>n_left-2) {
		m_pSimDoc->m_MStorage.SetCriteriaHistory(distout.size()-n_left, ABSME_STAT);
		if (!SV_Variable) {
			if (MeanOfMany) {
				m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_LOW][A1_STAT] = distout[0];
				m_pSimDoc->m_MStorage.SetMeanCriteria(A1_STAT, CRITERIA_STATE::ABOVE_AND_BELOW);
				m_pSimDoc->m_MStorage.m_MeanTSV_STAT[SELECT_HIGH][A1_STAT] = distout[distout.size()-1];
			}
			else {
				m_pSimDoc->m_MStorage.SetValidLow(ip, 0, distout[0]);
				if (m_pSimDoc->m_MStorage.GetCriteria(ip, 0) == 0)
					m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_MIN);
				else
					m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_AND_BELOW);

				m_pSimDoc->m_MStorage.SetValidHigh(ip, 0, distout[distout.size() - 1]);
				if (m_pSimDoc->m_MStorage.GetCriteria(ip, 0) == 0)
					m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_MIN);
				else
					m_pSimDoc->m_MStorage.SetCriteria(ip, CRITERIA_STATE::ABOVE_AND_BELOW);
			}
		}
		else {
			if (ivar == m_pSimDoc->m_ValSum_Array.size()) {
				m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_LOW][A1_STAT] = distout[0];
				m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_HIGH][A1_STAT] = distout[distout.size() - 1];
				m_pSimDoc->m_MStorage.m_Valid_SV_Mean[m_pSimDoc->m_MStorage.m_ActualEnsemble].criteria[A1_STAT] = CRITERIA_STATE::ABOVE_AND_BELOW;

			}
			else {
				m_pSimDoc->m_MStorage.m_Valid_SV[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_LOW][ip] = distout[0];
				m_pSimDoc->m_MStorage.m_Valid_SV[m_pSimDoc->m_MStorage.m_ActualEnsemble].v[E_SELECT_HIGH][ip] = distout[distout.size() - 1];
				m_pSimDoc->m_MStorage.SetSumCriteria(ip, CRITERIA_STATE::ABOVE_AND_BELOW);
			}
		}
		ReduceUpdateStep(A1_STAT);
	}
	return;
}

void ValidationCug::ReduceUpdateStep(PERFORMANCE_VAR value)
{
	m_pSimDoc->m_MStorage.ReduceUpdateStep(value);


}
