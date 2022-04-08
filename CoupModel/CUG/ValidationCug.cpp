 /////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived ValidationCug

#include "..\stdafx.h"
#include "..\Util.h"
#include "..\resource.h"
#include "ValidationCug.h"
#include ".\SimDoc\MyCug.h"
#include "..\PG\PG.h"
#include "..\PG\CreatePG.h"
#include "..\PG\CalculateVariable.h"
#include "..\Validation\ValidationEvaluation.h"
#include "..\NewBase\FileScanner.h"
#include "..\CoupModelView.h"


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
	DragDropTarget ( TRUE );
	m_Changed=FALSE;
	m_Updated=FALSE;
	m_ShowMean=0;
	m_pPG=NULL;
	m_pCUG=NULL;
	m_pDlg=NULL;
	m_ColorChanged=RGB(255,200,200);
	m_ColorEdit=RGB(40,240,240);
	m_ColorClick=RGB(240,240,50);
	m_ColorHeading=RGB(230,230,230);
	m_NoColor=RGB(255,255,255);
	m_font.CreateFont(14,0,0,0,500,0,0,0,0,0,0,0,0,"Arial");
	m_NewEstimations=FALSE;
	m_pNewLogLi=NULL;
	m_SelectedTimeShow=2;
	m_SelectedTimeIndCount=0;
	m_SelectedVarShow=1;
	m_SelectedVarIndCount=0;
	m_NumberNewSplitPeriods=5;



}

void ValidationCug::SetpCUG(void *pCUG)
{
	m_pCUG=MySingle::instance().GetCugPointer();
}
void ValidationCug::SetpDlg(CDialog *pDlg)
{
	m_pDlg=pDlg;

}
void ValidationCug::SetpPG(CPG *pPG)
{
	m_pPG=pPG;
}
void ValidationCug::SetpDoc(CCoupModelDoc *pDoc)
{
	m_pSimDoc=&pDoc->m_Doc;

	m_pCUG=MySingle::instance().GetCugPointer();
}
ValidationCug::~ValidationCug()
{
	if(m_pNewLogLi!=NULL) {
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

	int num_sheets=GetNumberSheets();
	for (long i=0;i<num_sheets;i++) {
		DeleteTab(i);
	}
		SetDefColWidth(10);

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
		AddTab("Split", 12);
		SetTabWidth(760); 
		SetNumberSheets(12); 
		OnSheetSetup(0);
		SetCurrentTab(0);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSheetSetup	
void ValidationCug::Make_MainSheets(int SheetNumber)
{


	CUGCell cell;
	m_SplitPeriods=m_pSimDoc->m_MStorage.GetNumSplitPeriods();

	int NumLinesSV=m_pSimDoc->m_ValidationData.GetNumSumVarVariables();
	m_col=14;
	int cols = m_col,
	rows = m_row;

	SetSheetNumber(SheetNumber);
	CString txt;
	switch (SheetNumber) {
		case 0:
			txt="Coefficients of Determination for the regression line";
			break;
		case 1:
			txt="Intercept for linear regression line with simulated values as independent variable";
			break;
		case 2:
			txt="Slope for linear regression line with simulated values as independent variable";
			break;
		case 3:
			txt="Mean Error between simulated values and measured variables";
			break;
		case 4:
			txt="Root Mean Square Error between simulated values and measured variables";
			break;
		case 5:
			txt="Log Likelihood values";
			break;
		case 6:
			txt="Nash Sutcliffe Coefficient of Determination (Modelling efficiency)";
			break;


	}
	m_pDlg->SetWindowText(txt);
	m_pDlg->OnInitDialog();
	SetDefFont(&m_font);

	EnableUpdate(FALSE); 
	while(GetNumberRows()>0)
		DeleteRow(0);
	EnableMenu(TRUE);
	EnableUpdate(TRUE); 
	SetNumberCols ( cols );
	if(NumLinesSV>1)
		SetNumberRows(rows+1);
	else
		SetNumberRows(rows);
	GetColDefault(0,&cell);
	cell.SetAlignment(UG_ALIGNRIGHT);
	for(int i=0;i<cols;i++) {
		if(m_Data||i>3)
		SetColDefault(i,&cell);
	}
	SetMultiSelectMode ( TRUE );

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
		SetColWidth(0,200);
		for(int i=1;i<3;i++) 
			SetColWidth(i,40);
		for(int i=3;i<cols;i++) 
			SetColWidth(i,62);
	
		QuickSetText(0,-2,"Variable Name");


		long iaccept=m_pSimDoc->m_MStorage.GetNumAccepted();
		long iacceptall=m_pSimDoc->m_MStorage.GetNumberOfRuns();


		QuickSetText(3,-2,"Distribution Prior Selection n="+MFC_Util::ItoAscii(iacceptall));
		QuickSetText(7,-2,"Distribution Post Selection n="+MFC_Util::ItoAscii(iaccept));
		if(iaccept!=iacceptall) QuickSetBackColor(5,-2, m_ColorEdit);
		QuickSetText(12,-2,"Limits for Selection");
		QuickSetText(1,-2,"Sub\nPeriod");
		QuickSetText(2,-2,"n");
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

	int NumLines=0;
	F* pValFile;


	NumLines=m_pSimDoc->GetNumAllValVariables();

	m_NumValidVar=NumLines;
	
	m_NumValidVar=m_pSimDoc->GetNumAllValVariables();

		Type="Tables";
		Group="Technical";
		Name="Scaling of Observed Variables";
		SimB *Sw;
		P *pCPt=NULL;

		Sw=m_pSimDoc->GetPtr(string(Type),"Technical", string(Name));
		if(Sw!=NULL&&NumLines>0) {
				int oldnelements=((Tab*)Sw)->GetNumOfRow();
				if (NumLines!=oldnelements)  {
	  				((Tab*)Sw)->SetNumOfRow(NumLines);
	  				//Sw->Apply_CE_only();
					//m_pSimDoc->ReallocateTheWholeMap();
				}
				pCPt=((Tab*)Sw)->GetP_Pointer(0);
		}


	int TotLines=m_NumValidVar;
	if(m_SplitPeriods>0) {
		TotLines=m_NumValidVar*(m_SplitPeriods+1);		
	}
	if(NumLinesSV>0&&m_NumValidVar>0)
		TotLines=TotLines+1+NumLinesSV;
	else if(NumLinesSV>0)
		TotLines=NumLinesSV;
	if(NumLinesSV>1&&m_NumValidVar>1)
		SetNumberRows(TotLines+2);
	else if (NumLinesSV>1||m_NumValidVar>1)
		SetNumberRows(TotLines+1);
	else
		SetNumberRows(TotLines);

	m_NumLines=TotLines;



CString test;
int FileNo, ind, index_start;
index_start=SheetNumber*NumLines;
double allmax, allmin, selmax, selmin, allcv,selcv;
int criteria;
int line;
line=-1;
for(int i=0;i<m_NumValidVar;i++) {
		test=MFC_Util::ItoAscii(i+1);
		FileNo=m_pSimDoc->m_ValidationData.GetValFileNumber(i);
		pValFile=m_pSimDoc->ValidationFilePointer(FileNo);
		ind=m_pSimDoc->m_ValidationData.GetValFileIndex(i);
		CPG *pPG;
		BOOL valid;
		pPG=(CPG*)pValFile->GetPointer();
		if(pPG==NULL) {
			pPG=new CPG;
			//CPG_Array.Add(pCPG);
			valid=pPG->GetFileDescription(pValFile->GetStrValue().c_str());
			if(valid) pValFile->SetPointer(pPG);
			pPG->CloseFile();
		}
		test=pValFile->GetDataDescription(ind).c_str();

		for (int split_ind=0; split_ind<=m_SplitPeriods;split_ind++) {
			line++;
			QuickSetText(0,line, test);
			criteria=m_pSimDoc->m_MStorage.GetCriteria(i+index_start, split_ind);
			allmax=double(m_pSimDoc->m_MStorage.GetMaxValidAll(i+index_start, split_ind));
			allmin=double(m_pSimDoc->m_MStorage.GetMinValidAll(i+index_start, split_ind));
			allcv=double(m_pSimDoc->m_MStorage.GetCV_ValidAll(i+index_start, split_ind));
			selmax=double(m_pSimDoc->m_MStorage.GetMaxValid(i+index_start, split_ind));
			selmin=double(m_pSimDoc->m_MStorage.GetMinValid(i+index_start, split_ind));
			selcv=double(m_pSimDoc->m_MStorage.GetCV_Valid(i+index_start, split_ind));
			if(split_ind==0) {
				QuickSetNumber(-1,line, i+1);
				QuickSetNumber(2,line,m_pSimDoc->m_ValidationData.GetValn(i));
			}
			else {
				QuickSetNumber(1,line, split_ind);
				QuickSetNumber(2,line,m_pSimDoc->m_MStorage.GetSplitNValue(i,split_ind-1));
			}
			if(split_ind>0&&split_ind==m_SplitPeriods) {
				JoinCells(0,line-m_SplitPeriods,0,line);
				JoinCells(-1,line-m_SplitPeriods,-1,line);
				
				
			}
			QuickSetNumber(3,line,double(m_pSimDoc->m_MStorage.GetMeanValidAll(i+index_start, split_ind)));
			QuickSetNumber(4,line,allcv); 
			QuickSetNumber(5,line,allmin); 
			QuickSetNumber(6,line,allmax);
			QuickSetNumber(7,line,double(m_pSimDoc->m_MStorage.GetMeanValid(i+index_start, split_ind))); 
			QuickSetNumber(8,line,selcv);
			QuickSetNumber(9,line,selmin); 
			QuickSetNumber(10,line,selmax); 
			double rangef;
			if(allmax-allmin>0) {
				rangef=(selmax-selmin)/(allmax-allmin);
				QuickSetNumber(11,line,rangef);
			}
			else
				QuickSetNumber(11,line,1.0);

			if(criteria==10) {


			}
			else {
				GetCell(12,line, &cell);
				cell.SetParam(split_ind);
				cell.SetNumber(double(m_pSimDoc->m_MStorage.GetValidLow(i+index_start, split_ind)));
				cell.SetBackColor(m_NoColor);
				if(criteria==1||criteria==3)cell.SetBackColor(m_ColorEdit);
				SetCell(12, line, &cell);
				GetCell(13,line, &cell);
				cell.SetParam(split_ind);
				cell.SetNumber(double(m_pSimDoc->m_MStorage.GetValidHigh(i+index_start, split_ind)));
				cell.SetBackColor(m_NoColor);
				if(criteria==1||criteria==3)cell.SetBackColor(m_ColorEdit);
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
	switch (SheetNumber) {
	
		case 0:
		//allmax=double(m_pSimDoc->m_MStorage.GetMaxMeanValidAll(i+index_start));
		//allmin=double(m_pSimDoc->m_MStorage.GetMinValidValidAll(i+index_start));
	
		selmax=double(m_pSimDoc->m_MStorage.GetMaxMeanValid_R2());
		selmin=double(m_pSimDoc->m_MStorage.GetMinMeanValid_R2());
		limit_low=double(m_pSimDoc->m_MStorage.GetMeanValidLow_R2());
		limit_high=double(m_pSimDoc->m_MStorage.GetMeanValidHigh_R2());
		break;
		case 1:
		//allmax=double(m_pSimDoc->m_MStorage.GetMaxMeanValidAll(i+index_start));
		//allmin=double(m_pSimDoc->m_MStorage.GetMinValidValidAll(i+index_start));
		selmax=double(m_pSimDoc->m_MStorage.GetMaxMeanValid_A0());
		selmin=double(m_pSimDoc->m_MStorage.GetMinMeanValid_A0());
		limit_low=double(m_pSimDoc->m_MStorage.GetMeanValidLow_A0());
		limit_high=double(m_pSimDoc->m_MStorage.GetMeanValidHigh_A0());

		break;
		case 2:
		//allmax=double(m_pSimDoc->m_MStorage.GetMaxMeanValidAll(i+index_start));
		//allmin=double(m_pSimDoc->m_MStorage.GetMinValidValidAll(i+index_start));
		selmax=double(m_pSimDoc->m_MStorage.GetMaxMeanValid_A1());
		selmin=double(m_pSimDoc->m_MStorage.GetMinMeanValid_A1());
		limit_low=double(m_pSimDoc->m_MStorage.GetMeanValidLow_A1());
		limit_high=double(m_pSimDoc->m_MStorage.GetMeanValidHigh_A1());					


		break;
		case 3:
		//allmax=double(m_pSimDoc->m_MStorage.GetMaxMeanValidAll(i+index_start));
		//allmin=double(m_pSimDoc->m_MStorage.GetMinValidValidAll(i+index_start));
		selmax=double(m_pSimDoc->m_MStorage.GetMaxMeanValid_AbsME());
		selmin=double(m_pSimDoc->m_MStorage.GetMinMeanValid_AbsME());
		limit_low=double(m_pSimDoc->m_MStorage.GetMeanValidLow_AbsME());
		limit_high=double(m_pSimDoc->m_MStorage.GetMeanValidHigh_AbsME());

		break;
		case 4:
		//allmax=double(m_pSimDoc->m_MStorage.GetMaxMeanValidAll(i+index_start));
		//allmin=double(m_pSimDoc->m_MStorage.GetMinValidValidAll(i+index_start));
		selmax=double(m_pSimDoc->m_MStorage.GetMaxMeanValid_RMSE());
		selmin=double(m_pSimDoc->m_MStorage.GetMinMeanValid_RMSE());
		limit_low=double(m_pSimDoc->m_MStorage.GetMeanValidLow_RMSE());
		limit_high=double(m_pSimDoc->m_MStorage.GetMeanValidHigh_RMSE());					


		break;
		case 5:
		//allmax=double(m_pSimDoc->m_MStorage.GetMaxMeanValidAll(i+index_start));
		//allmin=double(m_pSimDoc->m_MStorage.GetMinValidValidAll(i+index_start));
		selmax=double(m_pSimDoc->m_MStorage.GetMaxSumValid_LogLi());
		selmin=double(m_pSimDoc->m_MStorage.GetMinSumValid_LogLi());
		limit_low=double(m_pSimDoc->m_MStorage.GetSumValidLow_LogLi());
		limit_high=double(m_pSimDoc->m_MStorage.GetSumValidHigh_LogLi());			
		VarText="Sum of all variables";


		break;
		case 6:
		//allmax=double(m_pSimDoc->m_MStorage.GetMaxMeanValidAll(i+index_start));
		//allmin=double(m_pSimDoc->m_MStorage.GetMinValidValidAll(i+index_start));
		selmax=double(m_pSimDoc->m_MStorage.GetMaxMeanValid_NSE());
		selmin=double(m_pSimDoc->m_MStorage.GetMinMeanValid_NSE());
		limit_low=double(m_pSimDoc->m_MStorage.GetMeanValidLow_NSE());
		limit_high=double(m_pSimDoc->m_MStorage.GetMeanValidHigh_NSE());			
		VarText="Sum of all variables";


		break;
	}

	//QuickSetNumber(1,i,double(m_pSimDoc->m_MStorage.GetMeanValidAll(i+index_start)));
	//QuickSetNumber(3,i,allmin); 
	//QuickSetNumber(4,i,allmax);
	//QuickSetNumber(5,i,double(m_pSimDoc->m_MStorage.GetMeanValid(i+index_start))); 
		QuickSetText(0,line,VarText); 

		QuickSetNumber(9,line,selmin); 
		QuickSetNumber(10,line,selmax); 
	/*double rangef;
	if(allmax-allmin>0) {
		rangef=(selmax-selmin)/(allmax-allmin);
		QuickSetNumber(9,NumLines,rangef);
	}
	else
		QuickSetNumber(9,NumLines,1.0);*/

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

	JoinCells(0,line,8,line);
}
if(NumLinesSV>0) {
	line++;
	QuickSetText(0,line, "Single Variable Type");
	JoinCells(0,line,13, line);
	QuickSetBackColor(0,line,m_ColorHeading);

float value;
for(int i=0; i<NumLinesSV;i++) {
	
	line++;
	test=m_pSimDoc->m_ValidationData.GetValSumVarName(i).c_str();
	test+=" - ";
	test+=m_pSimDoc->m_ValidationData.GetStringValSumVarOutputType(i).c_str();
	QuickSetNumber(-1,line, i+1);
	QuickSetText(0,line, test);
	JoinCells(0,line,2,line);
	int index;
	float vmin, vmax, range, vmin1,vmax1;
	switch(SheetNumber) {
		case 3:
		case 4:
		case 5:
			index=SheetNumber%3+i*3;
			value=m_pSimDoc->m_MStorage.GetMeanSumValidAll(index);
			QuickSetNumber(3, line, value);
			value=m_pSimDoc->m_MStorage.GetCVSumValidAll(index);
			QuickSetNumber(4, line, value);
		 	vmin=m_pSimDoc->m_MStorage.GetMinSumValidAll(index);
			QuickSetNumber(5, line, vmin);
			vmax=m_pSimDoc->m_MStorage.GetMaxSumValidAll(index);
			QuickSetNumber(6, line, vmax);
			value=m_pSimDoc->m_MStorage.GetMeanSumValid(index);
			QuickSetNumber(7, line, value);
			value=m_pSimDoc->m_MStorage.GetCVSumValid(index);
			QuickSetNumber(8, line, value);
			value=m_pSimDoc->m_MStorage.GetMinSumValid(index);
			QuickSetNumber(9, line, value);
			vmin1=value;
			value=m_pSimDoc->m_MStorage.GetMaxSumValid(index);
			QuickSetNumber(10, line, value);
			vmax1=value;
			if(vmax1>vmin1) {
				range=(vmax1-vmin1)/(vmax-vmin);
				QuickSetNumber(11, line, range);
			}
			value=m_pSimDoc->m_MStorage.GetValidSumLow(index);
				GetCell(12,line, &cell);
				cell.SetNumber(double(value));
				cell.SetBackColor(m_NoColor);
				if(value>vmin)cell.SetBackColor(m_ColorEdit);
				SetCell(12, line, &cell);
			value=m_pSimDoc->m_MStorage.GetValidSumHigh(index);
				GetCell(13,line, &cell);
				cell.SetNumber(double(value));
				cell.SetBackColor(m_NoColor);
				if(value<vmax)cell.SetBackColor(m_ColorEdit);
				SetCell(13, line, &cell);
				break;

		default:;



	}
}
if(NumLinesSV>1) {
	double limit_low, limit_high;
	CString VarText;
	VarText="Sum of SV variables";
	line++;
	criteria=m_pSimDoc->m_MStorage.GetMeanSumCriteria(SheetNumber);
	switch (SheetNumber) {

		case 3:
		//allmax=double(m_pSimDoc->m_MStorage.GetMaxMeanValidAll(i+index_start));
		//allmin=double(m_pSimDoc->m_MStorage.GetMinValidValidAll(i+index_start));
		selmax=double(m_pSimDoc->m_MStorage.m_ValidSumMax[0]);
		selmin=double(m_pSimDoc->m_MStorage.m_ValidSumMin[0]);
		limit_low=double(m_pSimDoc->m_MStorage.m_ValidSumLow[0]);
		limit_high=double(m_pSimDoc->m_MStorage.m_ValidSumHigh[0]);

		break;
		case 4:
		selmax=double(m_pSimDoc->m_MStorage.m_ValidSumMax[1]);
		selmin=double(m_pSimDoc->m_MStorage.m_ValidSumMin[1]);
		limit_low=double(m_pSimDoc->m_MStorage.m_ValidSumLow[1]);
		limit_high=double(m_pSimDoc->m_MStorage.m_ValidSumHigh[1]);
			


		break;
		case 5:
		//allmax=double(m_pSimDoc->m_MStorage.GetMaxMeanValidAll(i+index_start));
		//allmin=double(m_pSimDoc->m_MStorage.GetMinValidValidAll(i+index_start));
		selmax=double(m_pSimDoc->m_MStorage.m_ValidSumMax[2]);
		selmin=double(m_pSimDoc->m_MStorage.m_ValidSumMin[2]);
		limit_low=double(m_pSimDoc->m_MStorage.m_ValidSumLow[2]);
		limit_high=double(m_pSimDoc->m_MStorage.m_ValidSumHigh[2]);			
		VarText="Sum of all variables";


		break;
		default:;

	}

	//QuickSetNumber(1,i,double(m_pSimDoc->m_MStorage.GetMeanValidAll(i+index_start)));
	//QuickSetNumber(3,i,allmin); 
	//QuickSetNumber(4,i,allmax);
	//QuickSetNumber(5,i,double(m_pSimDoc->m_MStorage.GetMeanValid(i+index_start))); 
	if(SheetNumber>2&&SheetNumber<6) {	
		QuickSetText(0,line,VarText); 

		QuickSetNumber(9,line,selmin); 
		QuickSetNumber(10,line,selmax);

	/*double rangef;
	if(allmax-allmin>0) {
		rangef=(selmax-selmin)/(allmax-allmin);
		QuickSetNumber(9,NumLines,rangef);
	}
	else
		QuickSetNumber(9,NumLines,1.0);*/

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

		JoinCells(0,line,8,line);
	}
}




	//value=m_pSimDoc->m_MStorage.GetMinSumValid(i);
		//value=m_pSimDoc->m_MStorage.GetMinSumValid(i);
//	QuickSetText(1,ind+i,m_pDoc->m_Doc.m_ValidationData.GetStringValSumVarOutputType(i));
		




}


RedrawAll();


}
void ValidationCug::Make_CriteriaSheet()
{

	int cols = 6,rows;
	CUGCell cell;
	SetSheetNumber(8);
	m_pDlg->SetWindowText("Criterias used to select accepted runs");
	m_pDlg->OnInitDialog();

	SetDefFont(&m_font);
	SetMultiSelectMode ( TRUE );


	
	EnableUpdate(FALSE); 
	while(GetNumberRows()>0)
		DeleteRow(0);
	EnableUpdate(TRUE);
	EnableMenu(TRUE); 
	
	SetNumberCols ( cols );
	int icount=0;
	for(int i=0;i<m_NumValidVar*7;i++) {

		for (int split_ind=0; split_ind<=m_SplitPeriods;split_ind++) {
		if(m_pSimDoc->m_MStorage.GetCriteria(i, split_ind)>0&&m_pSimDoc->m_MStorage.GetCriteria(i, split_ind)<10)
			icount++;
		}
	}
	for(int i=0;i<7;i++) {
		if(m_pSimDoc->m_MStorage.GetMeanCriteria(i)>0&&m_pSimDoc->m_MStorage.GetMeanCriteria(i)<10)
			icount++;
	}
	
	int NumLinesSV=m_pSimDoc->m_ValidationData.GetNumSumVarVariables();	
	for(int i=0;i<NumLinesSV*3;i++) {
		if(m_pSimDoc->m_MStorage.GetSumCriteria(i)>0&&m_pSimDoc->m_MStorage.GetSumCriteria(i)<10)
			icount++;
	}

	for(int i=0;i<3;i++) {
		if(m_pSimDoc->m_MStorage.GetMeanSumCriteria(i)>0&&m_pSimDoc->m_MStorage.GetMeanSumCriteria(i)<10)
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
	SetColWidth(1,250);
	for(int i=2;i<cols;i++) 
	    SetColWidth(i,100);
	GetColDefault(0,&cell);
	cell.SetAlignment(UG_ALIGNRIGHT);
	QuickSetText(-1,-1,"No");
	QuickSetText(0,-1,"Varialbe No");
	QuickSetText(1,-1,"Variable Name");
	QuickSetText(2,-1,"Sub Period");
	QuickSetText(3,-1,"Performance index");
	QuickSetText(4,-1,"Minimum");
	QuickSetText(5,-1,"Maximum");

    int index;
	icount=0;
	int criteria;
	int FileNo, ind;

	F* pValFile;
	CString PerformanceVar, test;
	int MeanCriteria;
	for(int var=0;var<7;var++) {
		MeanCriteria=m_pSimDoc->m_MStorage.GetMeanCriteria(var);
		switch( var) {
			case 0:
				PerformanceVar="R2";

				break;
			case 1:
				PerformanceVar="A0";
				break;
			case 2:
				PerformanceVar="A1";
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
				PerformanceVar="NSE R2";
				break;

		}

		for (int split_ind=0; split_ind<=m_SplitPeriods;split_ind++) {
			for(int i=0;i<m_NumValidVar;i++) {
			index=i+var*m_NumValidVar;
			criteria=m_pSimDoc->m_MStorage.GetCriteria(index, split_ind);
			if(criteria>0) {
				FileNo=m_pSimDoc->m_ValidationData.GetValFileNumber(i);
				pValFile=m_pSimDoc->ValidationFilePointer(FileNo);
				ind=m_pSimDoc->m_ValidationData.GetValFileIndex(i);
				CPG *pPG;
				BOOL valid;
				pPG=(CPG*)pValFile->GetPointer();
				if(pPG==NULL) {
					pPG=new CPG;
					//CPG_Array.Add(pCPG);
					valid=pPG->GetFileDescription(pValFile->GetStrValue().c_str());
					if(valid) pValFile->SetPointer(pPG);
					pPG->CloseFile();
				}
				test=pValFile->GetDataDescription(ind+1).c_str();
				QuickSetNumber(0,icount, i+1);
				QuickSetText(1,icount, test);
				QuickSetText(3,icount, PerformanceVar);
				if(split_ind==0)
					QuickSetText(2,icount,"");
				else
					QuickSetNumber(2,icount,split_ind);
				
				if(criteria==1) 
					QuickSetNumber(4,icount,double(m_pSimDoc->m_MStorage.GetValidLow(index, split_ind))); 
				else if (criteria==2)
					QuickSetNumber(5,icount,double(m_pSimDoc->m_MStorage.GetValidHigh(index, split_ind))); 
				else if (criteria==3) {
					QuickSetNumber(4,icount,double(m_pSimDoc->m_MStorage.GetValidLow(index, split_ind))); 
					QuickSetNumber(5,icount,double(m_pSimDoc->m_MStorage.GetValidHigh(index, split_ind))); 
				}
				icount++;
				QuickSetNumber(-1, icount-1, icount);
			}
		}
	}
	double MeanLow, MeanHigh;
	if(MeanCriteria>0) {
		test="Mean of all Variables";
		switch(var) {
			case 0:
				MeanLow=double(m_pSimDoc->m_MStorage.GetMeanValidLow_R2());
				MeanHigh=double(m_pSimDoc->m_MStorage.GetMeanValidHigh_R2());
				break;
			case 1:
				MeanLow=double(m_pSimDoc->m_MStorage.GetMeanValidLow_A0());
				MeanHigh=double(m_pSimDoc->m_MStorage.GetMeanValidHigh_A0());
				break;
			case 2:
				MeanLow=double(m_pSimDoc->m_MStorage.GetMeanValidLow_A1());
				MeanHigh=double(m_pSimDoc->m_MStorage.GetMeanValidHigh_A1());
				break;
			case 3:
				MeanLow=double(m_pSimDoc->m_MStorage.GetMeanValidLow_AbsME());
				MeanHigh=double(m_pSimDoc->m_MStorage.GetMeanValidHigh_AbsME());
				break;
			case 4:
				MeanLow=double(m_pSimDoc->m_MStorage.GetMeanValidLow_RMSE());
				MeanHigh=double(m_pSimDoc->m_MStorage.GetMeanValidHigh_RMSE());
				break;
			case 5:
				MeanLow=double(m_pSimDoc->m_MStorage.GetSumValidLow_LogLi());
				MeanHigh=double(m_pSimDoc->m_MStorage.GetSumValidHigh_LogLi());
				test="Sum of all Variables";
				break;
			case 6:
				MeanLow=double(m_pSimDoc->m_MStorage.GetMeanValidLow_NSE());
				MeanHigh=double(m_pSimDoc->m_MStorage.GetMeanValidHigh_NSE());
				break;
		}
		if(MeanCriteria==1)
			QuickSetNumber(4,icount,MeanLow);
		else if(MeanCriteria==2)
			QuickSetNumber(5,icount,MeanHigh);
		else if(MeanCriteria==3) {
			QuickSetNumber(4,icount,MeanLow);
			QuickSetNumber(5,icount,MeanHigh);
		}
		QuickSetText(1,icount,test);
		QuickSetText(3,icount,PerformanceVar);
		icount++;
		QuickSetNumber(-1, icount-1, icount);
	}


}


for(int var=0;var<3;var++) {
		MeanCriteria=m_pSimDoc->m_MStorage.GetMeanSumCriteria(var);
		switch( var) {
			case 0:
				PerformanceVar="ME";
				break;
			case 1:
				PerformanceVar="RMSE";
				break;
			case 2:
				PerformanceVar="LogLi";
				break;

		}

		for(int i=0;i<NumLinesSV;i++) {
			int index;
			index=i*3+var;
			criteria=m_pSimDoc->m_MStorage.GetSumCriteria(index);
			if(criteria>0) {
				test=_T("");//pValFile->GetDataDescription(ind+1).c_str();
				test=m_pSimDoc->m_ValidationData.GetValSumVarName(i).c_str();
				test+=" - ";
				test+=m_pSimDoc->m_ValidationData.GetStringValSumVarOutputType(i).c_str();

				QuickSetNumber(0,icount, i+1);
				QuickSetText(1,icount, test);
				QuickSetText(3,icount, PerformanceVar);
				QuickSetText(2,icount,"");

				
				if(criteria==1) 
					QuickSetNumber(4,icount,double(m_pSimDoc->m_MStorage.GetValidSumLow(index))); 
				else if (criteria==2)
					QuickSetNumber(5,icount,double(m_pSimDoc->m_MStorage.GetValidSumHigh(index))); 
				else if (criteria==3) {
					QuickSetNumber(4,icount,double(m_pSimDoc->m_MStorage.GetValidSumLow(index))); 
					QuickSetNumber(5,icount,double(m_pSimDoc->m_MStorage.GetValidSumHigh(index))); 
				}
				icount++;
				QuickSetNumber(-1, icount-1, icount);
			}
		}
	double MeanLow, MeanHigh;
	if(MeanCriteria>0) {
		test="Mean of all SV Variables";
		switch(var) {
			case 0:
				MeanLow=double(m_pSimDoc->m_MStorage.m_ValidSumLow[0]);
				MeanHigh=double(m_pSimDoc->m_MStorage.m_ValidSumHigh[0]);
				break;
			case 1:
				MeanLow=double(m_pSimDoc->m_MStorage.m_ValidSumLow[1]);
				MeanHigh=double(m_pSimDoc->m_MStorage.m_ValidSumHigh[1]);
				test="Sum of all  SV Variables";
				break;
			case 2:
				MeanLow=double(m_pSimDoc->m_MStorage.m_ValidSumLow[2]);
				MeanHigh=double(m_pSimDoc->m_MStorage.m_ValidSumHigh[2]);
				test="Sum of all  SV Variables";
				break;
		}
		if(MeanCriteria==1)
			QuickSetNumber(4,icount,MeanLow);
		else if(MeanCriteria==2)
			QuickSetNumber(5,icount,MeanHigh);
		else if(MeanCriteria==3) {
			QuickSetNumber(4,icount,MeanLow);
			QuickSetNumber(5,icount,MeanHigh);
		}
		QuickSetText(1,icount,test);
		QuickSetText(3,icount,PerformanceVar);
		icount++;
		QuickSetNumber(-1, icount-1, icount);
	}


	}



}

void ValidationCug::Make_SplitPeriodSheet()
{

	int cols = 2,rows=2;
	CUGCell cell;
	SetSheetNumber(12);
	m_pDlg->SetWindowText("Split the simulations period into sub periods");
	m_pDlg->OnInitDialog();

	SetDefFont(&m_font);
	SetMultiSelectMode ( TRUE );


	
	EnableUpdate(FALSE); 
	while(GetNumberRows()>0)
		DeleteRow(0);
	EnableUpdate(TRUE);
	EnableMenu(TRUE); 
	
	SetNumberCols ( cols );
	
	SetNumberRows ( rows );
	SetColWidth(0,250);
	SetColWidth(1,100);
	SetColWidth(-1,0);
	QuickSetText(0,-1,"Item");
	QuickSetText(1,-1,"Number");
	QuickSetText(0,0,"Current Number of Sub Periods");
	QuickSetText(0,1,"New Number of Sub Periods");
	QuickSetNumber(1,0,m_SplitPeriods);
	QuickSetNumber(1,1,m_NumberNewSplitPeriods);
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
	m_pDlg->SetWindowText("Assumptions and parameters used in log Likelihood values");
	m_pDlg->OnInitDialog();



	SimB *pSw;
	P *pCPt=NULL;

	
	int nval, n;
	CString txt;
	CString optionlist;

	Type="Switches";
	Group="Technical";
	Name="Likelihood Function";
	pSw=m_pSimDoc->GetPtr(string(Type), string(Group), string(Name));
	m_pSw1=(Sw*)pSw;
	Name="Box-Cox Tranformation";	
	pSw=m_pSimDoc->GetPtr(string(Type), string(Group), string(Name));
	m_pSw2=(Sw*)pSw;

	nval=m_pSw2->GetValue();
	if(nval==0) 
		cols=8;
	else
		cols=10;

	m_newcolst=cols-4;
	
	txt=m_pSw2->GetOption(nval).c_str();




	SetNumberCols ( cols );

	int NumLines=0;
	F* pValFile;

	m_NumValidVar=m_pSimDoc->GetNumAllValVariables();
	NumLines=m_pSimDoc->GetNumAllValVariables();
	int NumLinesSV=m_pSimDoc->m_ValidationData.GetNumSumVarVariables();
	int TotLines=m_NumValidVar;


	if(NumLinesSV>0&&m_NumValidVar>0)
		TotLines=TotLines+1+NumLinesSV;
	else if(NumLinesSV>0)
		TotLines=NumLinesSV;
	
	SetNumberRows(TotLines+3);

	if(m_pNewLogLi==NULL) {
		long numrec;
		numrec=0;

			for(int i=0;i<MAXSIMVAL;i++) {
				pValFile=m_pSimDoc->ValidationFilePointer(i+1);
				if(pValFile->GetNumberOfSelectedVal()>0) {
					CPG *pPG;
					pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(0, i+1, TRUE);
					if(pPG!=NULL) {
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
	SetMultiSelectMode ( TRUE );

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

	pSw=m_pSimDoc->GetPtr(string(Group),string(Type), string(Name));
	if(pSw!=NULL&&NumLines>0) {
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
	pSw=m_pSimDoc->GetPtr(string(Type), string(Group), string(Name));
	m_pSw1=(Sw*)pSw;
	
	nval=m_pSw1->GetValue();

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
	pSw=m_pSimDoc->GetPtr(string(Type), string(Group), string(Name));
	m_pSw2=(Sw*)pSw;

	nval=m_pSw2->GetValue();
	
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
	pSw=m_pSimDoc->GetPtr(string(Type), string(Group), string(Name));
	m_pCPt1=(P*)pSw;

	Name="Box Cox offset";
	pSw=m_pSimDoc->GetPtr(string(Type), string(Group), string(Name));
	m_pCPt2=(P*)pSw;

	Name="Abs Error";
	pSw=m_pSimDoc->GetPtr(string(Type), string(Group), string(Name));
	m_pCPtError2=(P*)pSw;


	Name="Rel Error";
	pSw=m_pSimDoc->GetPtr(string(Type), string(Group), string(Name));
	m_pCPtError1=(P*)pSw;

	for(int i=0;i<TotLines;i++) 
		QuickSetNumber(-1,i,i+1);

	CString test;
	int FileNo, ind;

	for(int i=0;i<NumLines;i++) {
			test=MFC_Util::ItoAscii(i+1);
			FileNo=m_pSimDoc->m_ValidationData.GetValFileNumber(i);
			pValFile=m_pSimDoc->ValidationFilePointer(FileNo);
			ind=m_pSimDoc->m_ValidationData.GetValFileIndex(i);
			CPG *pPG;
			BOOL valid;
			pPG=(CPG*)pValFile->GetPointer();
			if(pPG==NULL) {
				pPG=new CPG;
				//CPG_Array.Add(pCPG);
				valid=pPG->GetFileDescription(pValFile->GetStrValue().c_str());
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
		m_pDlg->SetWindowText("Residuals between simulated and observed Variables");
		m_pDlg->OnInitDialog();
	}
	else {
		SetSheetNumber(10);
		m_pDlg->SetWindowText("Log Likelihood values based on new settings");
		m_pDlg->OnInitDialog();
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
		if(pRes!=NULL) {
			if(pRes->IsExtraLogLiFile())
				FileOpen=TRUE;
		}
	}
	NumLines=m_pSimDoc->GetNumAllValVariables();
	int NumAllVariables=NumLines;


	int NumLinesSV=m_pSimDoc->m_ValidationData.GetNumSumVarVariables();
	
	int TotLines=NumLines;
	if(NumLinesSV>0&&NumLines>0)
		TotLines=TotLines+1+NumLinesSV;
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
	SetMultiSelectMode ( TRUE );

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

	CString domain;
	

	QuickSetText(0,-1,"Variable Name");
	QuickSetText(1,-2,"n");
	QuickSetText(2,-2,"Mean");
	QuickSetText(3,-2,"St.Dev");
	QuickSetText(4,-2,"Min");
	QuickSetText(5,-2,"Max");
	GetCell(-1,-1,&cell);
	cell.SetCellTypeEx(UGCT_NORMALMULTILINE);
	cell.SetBackColor(m_ColorClick);
	CString str;
	if(m_SelectedTimeShow==0) {
		str="Individual\nAccepted Run\n";
		str+=MFC_Util::ItoAscii(m_SelectedTimeIndCount%iaccept+1);
		str+="( "+MFC_Util::ItoAscii(iaccept)+")";
	}
	else if(m_SelectedTimeShow==1)
		str="Accepted\n("+MFC_Util::ItoAscii(iaccept)+")";
	else if(m_SelectedTimeShow==2)
		str="All Runs\n("+MFC_Util::ItoAscii(iacceptall)+")";
	else if(m_SelectedTimeShow<5)
		str="Accepted\n("+MFC_Util::ItoAscii(iaccept)+")";
	else if(m_SelectedTimeShow<7)
		str="All Runs\n("+MFC_Util::ItoAscii(iacceptall)+")";
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

	CString test;
	int FileNo, ind;
	
	long Id_run;
	long irow;
	irow=-1;
	int Id_var;
	if(m_SelectedVarShow==0) {
		test="One Variable "+MFC_Util::ItoAscii(m_SelectedVarIndCount%NumAllVariables+1)+" ("+MFC_Util::ItoAscii(NumAllVariables)+")";
		QuickSetBackColor(0,-2,m_ColorClick);
	}
	else
		test="All Variables ("+MFC_Util::ItoAscii(NumAllVariables)+")";
	QuickSetText(0,-2,test);
	for(int i=0;i<NumLines;i++) {
		Id_var=i;
		if(m_SelectedVarShow==0)
			Id_var=m_SelectedVarIndCount%NumAllVariables;
		test=MFC_Util::ItoAscii(Id_var+1);
		QuickSetText(-2,irow+1,test);
		FileNo=m_pSimDoc->m_ValidationData.GetValFileNumber(Id_var);
		pValFile=m_pSimDoc->ValidationFilePointer(FileNo);
		pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(FileNo);
		ind=m_pSimDoc->m_ValidationData.GetValFileIndex(Id_var);
		CPG *pPG;
		BOOL valid;
		pPG=(CPG*)pValFile->GetPointer();
		if(pPG==NULL) {
			pPG=new CPG;
			//CPG_Array.Add(pCPG);
			valid=pPG->GetFileDescription(pValFile->GetStrValue().c_str());
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
				domain+=MFC_Util::ItoAscii(Id_run+1);
			}
			else if(m_SelectedTimeShow<7) {
				domain=_T("");
				if(m_SelectedTimeShow>4)
					domain=_T("");
				CString Add;
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
				if(sheetNumber<7)
					Make_MainSheets(sheetNumber);
				else if(sheetNumber==7)
					Make_SumVarCriteriaSheet();
				else if(sheetNumber==8)
					Make_CriteriaSheet();
				else if(sheetNumber==9)
					Make_OptionSheet();
				else if(sheetNumber==10) {
					Make_DistributionSheet(FALSE);
				}
				else if(sheetNumber==11)
					Make_DistributionSheet(TRUE);
				else if(sheetNumber==12)
					Make_SplitPeriodSheet();
				
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
	m_pDlg->SetWindowText("Criteria on Statistic of Summary of Output Variables");
	m_pDlg->OnInitDialog();

	
	int nval, n;
	CString txt;
	CString optionlist;



	SetNumberCols ( 14 );

	auto NumLines=m_pSimDoc->m_MStorage.m_ValSum_Array.size();


	
	SetNumberRows(NumLines);
	GetColDefault(0,&cell);
	cell.SetAlignment(UG_ALIGNRIGHT);
	for(int i=0;i<cols;i++) {
		if(m_Data||i>3)
		SetColDefault(i,&cell);
	}
	SetMultiSelectMode ( TRUE );

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


		QuickSetText(3,-2,"Distribution Prior Selection n="+MFC_Util::ItoAscii(iacceptall));
		QuickSetText(7,-2,"Distribution Post Selection n="+MFC_Util::ItoAscii(iaccept));
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
		if(m_pSimDoc->m_MStorage.m_PriorSumVar.size()!=m_pSimDoc->m_MStorage.m_ValSum_Array.size()) {
			m_pSimDoc->m_MStorage.m_PriorSumVar.resize(NumLines);
			m_pSimDoc->m_MStorage.m_PostSumVar.resize(NumLines);
			m_pSimDoc->m_MStorage.UpdateSumVarStat(false);
			

		}

	VALSUM valsum;
	for(int i=0;i<NumLines;i++) {
		valsum=m_pSimDoc->m_MStorage.m_ValSum_Array[i];
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
			QuickSetNumber(12,i,valsum.ValueMin);
			QuickSetNumber(13,i,valsum.ValueMax);

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
	TRACE("ValidationCug::OnSelectionChanged called with startCol = %ld, startRow = %ld, endCol = %ld, endRow = %ld, blockNum = %ld.\n",
				startCol, startRow, endCol,	endRow, blockNum );
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
	if(col==0||col==1) {
		MultiPlotClick(isheet, col, row, TRUE);

	}
	//CUGCell cell;
	/*
	if(!m_Data) {
		if(col==0&&updn) {
			GetCell(0,i,&cell);
			cell.SetCellType(UGCT_CHECKBOX);	
			bool testV=pCPG->GetToBeSaved(i);
			if(testV) {
				cell.SetBool(FALSE);
				pCPG->SetToBeSaved(i,FALSE);
			}
			else {
				cell.SetBool(TRUE);	
				pCPG->SetToBeSaved(i,TRUE);
			}
			SetCell(0,i,&cell);
		}
	}*/
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
	int isheet=GetSheetNumber();
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
	int isheet=GetSheetNumber();
	if(isheet>=9&&(m_SelectedTimeShow==0||m_SelectedTimeShow==3||m_SelectedTimeShow==5)) {
		m_SelectedTimeIndCount++;
		OnSheetSetup(isheet);
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
	if ( !updn ) return;
	int isheet=GetSheetNumber();
	if(isheet>=9) {
		m_SelectedTimeIndCount++;
		OnSheetSetup(isheet);
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
	CString m_After;
	m_After=_T("");
	int prior;


	if(ID==1) {
		if(msg==103) {
			GetCell(col,row, &cell);
			m_After=cell.GetText();
		}
		if(row==m_NumLines+1) {
			prior=m_pSw1->GetIntValue();
			if(m_After!="") {
				m_pSw1->SetStrValue(string(m_After));
				m_LogFunc=m_pSw1->GetValue();
				m_pSimDoc->m_ValidationData.SetValNewLogLiOptions(m_LogFunc, m_BoxCoxTrans);
				m_pSw1->SetIntValue(prior);
			}

		}
		else if(row==m_NumLines+2) {
			prior=m_pSw2->GetIntValue();
			if(m_After!="") {
				m_pSw2->SetStrValue(string(m_After));
				m_BoxCoxTrans=m_pSw2->GetValue();
				m_pSimDoc->m_ValidationData.SetValNewLogLiOptions(m_LogFunc, m_BoxCoxTrans);
				m_pSw2->SetIntValue(prior);
			}


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
	if(SheetNumber==9) {
		float value;
		value=MFC_Util::AtoFloat(str);
		if(col==m_newcolst) {
			m_pSimDoc->m_ValidationData.SetValP_ErrorNew(row, value);
		}
		else if(col==m_newcolst+1) {
			m_pSimDoc->m_ValidationData.SetValA_ErrorNew(row, value);
		}
		else if(col==m_newcolst+2) {
			m_pSimDoc->m_ValidationData.SetValBoxCox_PowerNew(row, value);
		}
		else if(col==m_newcolst+3) {
			m_pSimDoc->m_ValidationData.SetValBoxCox_OffsetNew(row, value);
		}


	}
	if(SheetNumber==13) {
		if(col==1&&row==1) {
			m_NumberNewSplitPeriods=MFC_Util::AtoInt(str);
			if(m_NumberNewSplitPeriods!=5) QuickSetBackColor(1,1,m_ColorChanged);

		}

	}
	QuickSetText(col,row,str);
	if(col>9) {
		float value;
	    value=MFC_Util::AtoFloat(str);

	  int index_start=SheetNumber*m_NumValidVar;
	  if(row==m_NumValidVar*(m_SplitPeriods+1)) {
		  int criteria=m_pSimDoc->m_MStorage.GetMeanCriteria(SheetNumber);
		  if(col==12) {
			  switch(SheetNumber) {
				case 0:
					m_pSimDoc->m_MStorage.SetMeanValidLow_R2(value);
					break;
				case 1:
					m_pSimDoc->m_MStorage.SetMeanValidLow_A0(value);
					break;
				case 2:
					m_pSimDoc->m_MStorage.SetMeanValidLow_A1(value);
					break;
				case 3:
					m_pSimDoc->m_MStorage.SetMeanValidLow_AbsME(value);
					break;
				case 4:
					m_pSimDoc->m_MStorage.SetMeanValidLow_RMSE(value);
					break;
				case 5:
					m_pSimDoc->m_MStorage.SetSumValidLow_LogLi(value);
					break;
				case 6:
					m_pSimDoc->m_MStorage.SetMeanValidLow_NSE(value);
					break;
				}
				if(criteria==0)
					m_pSimDoc->m_MStorage.SetMeanCriteria(SheetNumber, 1);
				else
					m_pSimDoc->m_MStorage.SetMeanCriteria(SheetNumber, 3);
		  }


		  else if( col==13) {
			  switch(SheetNumber) {
				case 0:
					m_pSimDoc->m_MStorage.SetMeanValidHigh_R2(value);
					break;
				case 1:
					m_pSimDoc->m_MStorage.SetMeanValidHigh_A0(value);
					break;
				case 2:
					m_pSimDoc->m_MStorage.SetMeanValidHigh_A1(value);
					break;
				case 3:
					m_pSimDoc->m_MStorage.SetMeanValidHigh_AbsME(value);
					break;
				case 4:
					m_pSimDoc->m_MStorage.SetMeanValidHigh_RMSE(value);
					break;
				case 5:
					m_pSimDoc->m_MStorage.SetSumValidHigh_LogLi(value);
					break;
				case 6:
					m_pSimDoc->m_MStorage.SetMeanValidHigh_NSE(value);
					break;
				}
				if(criteria==0)
					m_pSimDoc->m_MStorage.SetMeanCriteria(SheetNumber, 2);
				else
					m_pSimDoc->m_MStorage.SetMeanCriteria(SheetNumber, 3);
		  }


		  QuickSetBackColor(col,row, m_ColorChanged);		  
		  m_Changed=TRUE;
	  }
	  else if(row>m_NumValidVar*(m_SplitPeriods+1)) {
		  int index=row-m_NumValidVar*(m_SplitPeriods+1)-2;
		  if(m_NumValidVar==0) index++;
		  int NumLinesSV=m_pSimDoc->m_ValidationData.GetNumSumVarVariables();
		  index=SheetNumber%3+index*3;
		  int criteria=m_pSimDoc->m_MStorage.GetMeanSumCriteria(SheetNumber%3);
		  if(col==12) {
			  if(index<NumLinesSV*3) {
				criteria=m_pSimDoc->m_MStorage.GetSumCriteria(index);	
				m_pSimDoc->m_MStorage.SetValidSumLow(index, value);
				if(criteria==0)
					m_pSimDoc->m_MStorage.SetSumCriteria(index, 1);
				else
					m_pSimDoc->m_MStorage.SetSumCriteria(index, 3);

			  }
			  else {
				  if(SheetNumber==3)
					m_pSimDoc->m_MStorage.SetMeanValidLow_Sum_AbsME(value);
				  else if(SheetNumber==4)
					m_pSimDoc->m_MStorage.SetMeanValidLow_Sum_RMSE(value);
				  else if(SheetNumber==5)
					m_pSimDoc->m_MStorage.SetSumValidLow_Sum_LogLi(value);
				if(criteria==0)
					m_pSimDoc->m_MStorage.SetMeanSumCriteria(SheetNumber%3, 1);
				else
					m_pSimDoc->m_MStorage.SetMeanSumCriteria(SheetNumber%3, 3);
			  }
			 

		  }
		  else if(col==13) {
			  if(index<NumLinesSV*3) {
				criteria=m_pSimDoc->m_MStorage.GetSumCriteria(index);	
				m_pSimDoc->m_MStorage.SetValidSumHigh(index, value);
			  	if(criteria==0)
					m_pSimDoc->m_MStorage.SetSumCriteria(index, 2);
				else
					m_pSimDoc->m_MStorage.SetSumCriteria(index, 3);
			  }
			  else {
				  if(SheetNumber==3)
					m_pSimDoc->m_MStorage.SetMeanValidHigh_Sum_AbsME(value);
				  else if(SheetNumber==4)
					m_pSimDoc->m_MStorage.SetMeanValidHigh_Sum_RMSE(value);
				  else if(SheetNumber==5)
					m_pSimDoc->m_MStorage.SetSumValidHigh_Sum_LogLi(value);


				if(criteria==0)
					m_pSimDoc->m_MStorage.SetMeanSumCriteria(SheetNumber%3, 2);
				else
					m_pSimDoc->m_MStorage.SetMeanSumCriteria(SheetNumber%3, 3);
			  }


		  }
		  QuickSetBackColor(col,row, m_ColorChanged);
		  m_Changed=TRUE;

	  }
	  else {
		GetCell(col, row, &cell);
		int ind_split=cell.GetParam();
		int iv;
		iv=row/(m_SplitPeriods+1);
		if(col==12) {
			if(SheetNumber==7) {
				m_pSimDoc->m_MStorage.m_ValSum_Array[row].ValueMin=value;


			}
			else {

				m_pSimDoc->m_MStorage.SetValidLow(iv+index_start, ind_split, value);
				if(m_pSimDoc->m_MStorage.GetCriteria(iv+index_start, ind_split)==0)
					m_pSimDoc->m_MStorage.SetCriteria(iv+index_start, 1, ind_split);
				else
					m_pSimDoc->m_MStorage.SetCriteria(iv+index_start, 3, ind_split);
			}

		}
		else if(col==13) {
			if(SheetNumber==7) {
				m_pSimDoc->m_MStorage.m_ValSum_Array[row].ValueMax=value;
			}
			else {
				m_pSimDoc->m_MStorage.SetValidHigh(iv+index_start, ind_split, value);
				if(m_pSimDoc->m_MStorage.GetCriteria(iv+index_start, ind_split)==0)
					m_pSimDoc->m_MStorage.SetCriteria(iv+index_start, 2, ind_split);
				else
					m_pSimDoc->m_MStorage.SetCriteria(iv+index_start, 3, ind_split);
			}

		}
		QuickSetBackColor(col,row, m_ColorChanged);
		m_Changed=TRUE;
	  }
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
	if(section==UG_GRID) {

		if(item== 1000) {
			CutSelected();
		}
		else if(item==2000)
			CopySelected();

		else if(item==3000){
			Paste();
		}
		else if(item==1061) {
			m_pSimDoc->m_MStorage.UpdateAcceptedRuns(TRUE, FALSE);
			((ValidationEvaluation*)m_pDlg)->m_Changed=TRUE;
			m_pSimDoc->MR_ReCalculatePostDist(false);
		}
		else if(item==1060) {
			m_pSimDoc->m_MStorage.UpdateAcceptedRuns(FALSE, FALSE);
			((ValidationEvaluation*)m_pDlg)->m_Changed=TRUE;
			m_pSimDoc->MR_ReCalculatePostDist(false);

			for(int i=0;i<MAXSIMVAL;i++) {
				CResiduals *pRes;
				pRes=m_pSimDoc->GetValidResidualPointer(i+1);
				if(pRes!=NULL) {
					pRes->ToBeUpdated();
					pRes->m_MeanDefined=false;
				}
			}







		}
		else if(item==2010) {

			EstimateNewLogLi();

		}
		else if(item==2020) {
			MakeSortedResiduals();

		}
		else if(item==4000) {
			DefineNewSubPeriod();
		}
		else if(item==4001) {
			MultiPlotClick(isheet, col, row, TRUE, TRUE); 
		
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
}


/////////////////////////////////////////////////////////////////////////////
//	return UG_SUCCESS to allow the menu to appear
//	return 1 to not allow the menu to appear
int ValidationCug::OnMenuStart(int col,long row,int section)
{
	int sheetNumber=GetSheetNumber();
	EmptyMenu();
			//****** Populate the Menu 
	if(section==UG_GRID&&col>1) {
		
			AddMenuItem(1000, "Cut"); 
			AddMenuItem(2000, "Copy"); 
			AddMenuItem(3000, "Paste"); 
	}
	if(section==UG_CORNERBUTTON	) {
		if(sheetNumber>=10) {
			if(m_SelectedTimeShow!=0) AddMenuItem(3010,"View Individual runs - one by one");
			if(m_SelectedTimeShow!=1) AddMenuItem(3011,"View Individual runs - Accepted in same list");
			if(m_SelectedTimeShow!=2) AddMenuItem(3012,"View Individual runs - All in same list");
			if(m_SelectedTimeShow!=3) AddMenuItem(3013,"View Mean of All Accepted runs - Single Indicator at a time");
			if(m_SelectedTimeShow!=4) AddMenuItem(3014,"View Mean of All Accepted runs - All Indicators in same list");
			if(m_SelectedTimeShow!=5) AddMenuItem(3015,"View Mean of All runs - Single Indicator at a time");
			if(m_SelectedTimeShow!=6) AddMenuItem(3016,"View Mean of All runs - All Indicators in same list");

		}
		return TRUE;



	}
	else if(section==UG_TOPHEADING) {
			if(m_SelectedVarShow!=0) AddMenuItem(3020,"View Variables - one by one");
			if(m_SelectedVarShow!=1) AddMenuItem(3021,"View All Variables in same list");
			return TRUE;
	}

	int ifile=m_pSimDoc->m_ValidationData.GetValFileNumber(0);
	if(sheetNumber>=9&&sheetNumber<=10&&m_pSimDoc->MR_XBin_IsOpen(ifile)) {
			AddMenuItem(2010,"Estimate new Likelihood values based on new settings");

	}
	if(sheetNumber>=9&&sheetNumber<=10&&m_pSimDoc->MR_XBin_IsOpen(ifile)) {
			AddMenuItem(2020,"Create a file with Sorted Residuals");

	}
	if(sheetNumber==13&&m_pSimDoc->MR_XBin_IsOpen(ifile)) 
				AddMenuItem(4000,"Estimate performance indicators valid for equal big sub periods");
	else if(col<2) {
		if(m_SplitPeriods>0&&sheetNumber<7)
			AddMenuItem(4001,"Make Chart with performance indicator for all sub periods");
	}
	else {
		if(m_Changed) {
			if(ifile<0) {
				AddMenuItem(1060,"Update the list of Accepted Runs");
			}
			else if(m_pSimDoc->MR_XBin_IsOpen(ifile)) {
					AddMenuItem(1060,"Update the list of Accepted Runs");
			}
		}
		if(ifile<0) {
			AddMenuItem(1061,"Reset the list of Accepted Runs to all Runs");
		}
		else if(m_pSimDoc->MR_XBin_IsOpen(ifile)) {
				AddMenuItem(1061,"Reset the list of Accepted Runs to all Runs");
		}
		
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHint
int ValidationCug::OnHint(int col,long row,int section,CString *string)
{
	int sheetNumber=GetSheetNumber();
	if(col<2&&sheetNumber<7) {
		if((m_SplitPeriods==0&&col==0)||(m_SplitPeriods>=0&&col==0)) 
			string->Format("Single Click on Variable name to make a chart");
		else if(m_SplitPeriods==0)
			string->Format("Single Click Split tab to define sub periods");
		else
			string->Format("Single click on sub period to make a chart representing that sub period");
		

	}
	else if(col>1&&sheetNumber<7) {
		string->Format("Edit Low and/or high limits to make constrains for accepting runs and right click to make changes");

	}
	else if(sheetNumber==9) {
		string->Format("Edit options for LogLikelihood estimations and right click to make changes");
	}
	else if(sheetNumber==12) {
		string->Format("Edit options for number of sub periods and right click to make changes");
	}
	else
		string->Format("Col:%d Row:%ld",col, row);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnVScrollHint
int ValidationCug::OnVScrollHint(long row,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHScrollHint
int ValidationCug::OnHScrollHint(int col,CString *string)
{
	return TRUE;
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
			if(NULL == cell1->GetText() && NULL == cell2->GetText())
				return 0;
			if(NULL == cell1->GetText())
				return 1;
			if(NULL == cell2->GetText())
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
	if(NULL == cell1->GetText())
		return 1;
	if(NULL == cell2->GetText())
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
					if(pPG==NULL) {
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
	float accaccsum, accsum;
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
/*		CFileScanner ScanMeanProg(NULL);
		ScanMeanProg.Create(IDD_PROGRESS,NULL);
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

/*				
				RelPscan=float(j)/float(numrec)*smallsegment+run_ratio*100;
				if(RelPscan%4==1&&j%1000==1) ScanMeanProg.Update();
				*/




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
		Sw=NULL;

		int ivar=1;
		int numvar;
		int ValFileNo;
		int Id_var;

	

		m_pCUG->UpdateCurrentChart(FALSE);
		
		int numvarprev=m_pSimDoc->m_PlotStorage.GetNumVar(m_pCUG->m_CurrentChart);


		CWinApp* pApp = AfxGetApp();
		int value=2;
		int ResidualPlot=pApp->GetProfileInt("CoupModel - KTH", "ResidualPlot", value);
		if(ResidualPlot<=2) {
			if(sheet==9)
				pApp->WriteProfileInt("CoupModel - KTH", "ResidualPlot", 2);
			else
				pApp->WriteProfileInt("CoupModel - KTH", "ResidualPlot", 1);
		}
		int loglistyle;
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
					
						m_pSimDoc->m_PlotStorage.Add(Sw, itype, Index, m_pCUG->m_CurrentChart, m_pCUG->m_CurrentChartType,m_pSimDoc->GetCurrentFile(), ValFileNo, ValFileIndex, mrunno,0,numvar+1, 0);
						loglistyle=1;
					
					}



					if(!m_pSimDoc->m_PlotStorage.Add(Sw, itype, Index, m_pCUG->m_CurrentChart, m_pCUG->m_CurrentChartType,m_pSimDoc->GetCurrentFile(), ValFileNo, ValFileIndex, mrunno,0,numvar+1, loglistyle))
							m_pSimDoc->SetMultiStorageBinFileMissing();

					}

		

	
		if(m_pCUG->CreateZoomPlotWindow(FALSE)) {
			//SetCell(col,row, &cell);
			m_pCUG->UpdatePlotStatusBar(sheet, TRUE);
			return TRUE;
		}
		return FALSE;

}
BOOL ValidationCug::MultiPlotClick(int sheet, int col, int row, BOOL Add, BOOL All) 
{
		int itype, Index;
		CUGCell cell, cell1;
		SimB *Sw;
		int iv;
		int nselV=m_pSimDoc->m_ValidationData.GetNumberSelectedTSV_Val();
		iv=row;
		int ind_split=0;
		if(m_SplitPeriods>0) {
			nselV=m_NumValidVar*(m_SplitPeriods+1);
			iv=row/(m_SplitPeriods+1);
			GetCell(1, row, &cell);
			ind_split=cell.GetNumber();
		}
		Sw=NULL;
		

		
		int ivar=1;
		int numvar;
		int ValFileIndex, ValFileNo;

		CCoupModelDoc *pDoc;

			if(row==nselV)
					return FALSE;
			else if(row>nselV) {
					itype=9;
					int nvar=row-nselV;
					if(col==10) 
						Index=(nvar-1)*3;
					else if(col==11)
						Index=(nvar-1)*3+1;
					else if(col==14) 
						Index=(nvar-1)*3+2;
					else
						return FALSE;
				}
			else if(row<nselV) {


					ValFileNo=m_pSimDoc->m_ValidationData.GetValFileNumber(iv);
					ValFileIndex=-1;
					for(int jx=0;jx<=iv;jx++) {
						if(m_pSimDoc->m_ValidationData.GetValFileNumber(iv)==ValFileNo)
								ValFileIndex++;
					}

					itype=6;
					if(sheet<7)
						Index=iv+sheet*m_NumValidVar;
					else
						return FALSE;

			}

		m_pCUG->UpdateCurrentChart(TRUE);
		
		int numvarprev=m_pSimDoc->m_PlotStorage.GetNumVar(m_pCUG->m_CurrentChart);


		for(int ip=0; ip<ivar; ip++) {
					numvar=numvarprev+ip;
					/*if(sheet==5) 
						Index=m_SelMultiSumIndex.GetAt(ip);
					else if(sheet==6)
						Index=m_SelMultiValIndex.GetAt(ip);
					else
						Index=m_SelMultiParameterIndex.GetAt(ip);
*/
					if(m_pCUG->m_NumOpenDoc>0) {
						for(int j=0;j<m_pCUG->m_NumOpenDoc;j++) {
							if(m_pCUG->m_NumOpenDoc<=1)
								;
							else
								m_pCUG->m_SelectedDoc.GetAt(j);
							if(All) {
								for(int i=1; i<=m_SplitPeriods;i++)
									if(!m_pSimDoc->m_PlotStorage.Add(Sw, itype, Index, m_pCUG->m_CurrentChart, m_pCUG->m_CurrentChartType,m_pSimDoc->GetCurrentFile(), ValFileNo, ValFileIndex, 0,i,numvar+1))
										m_pSimDoc->SetMultiStorageBinFileMissing();
								}
							else if(!m_pSimDoc->m_PlotStorage.Add(Sw, itype, Index, m_pCUG->m_CurrentChart, m_pCUG->m_CurrentChartType,m_pSimDoc->GetCurrentFile(), ValFileNo, ValFileIndex, 0,ind_split,numvar+1))
								m_pSimDoc->SetMultiStorageBinFileMissing();

						}
					}
					else {
						if(All) {
							for(int i=1; i<=m_SplitPeriods;i++)
								if(!m_pSimDoc->m_PlotStorage.Add(Sw, itype, Index, m_pCUG->m_CurrentChart, m_pCUG->m_CurrentChartType,m_pSimDoc->GetCurrentFile(), ValFileNo, ValFileIndex, 0,i,numvar+1))
									m_pSimDoc->SetMultiStorageBinFileMissing();
						}
						else if(!m_pSimDoc->m_PlotStorage.Add(Sw, itype, Index, m_pCUG->m_CurrentChart, m_pCUG->m_CurrentChartType,m_pSimDoc->GetCurrentFile(), ValFileNo, ValFileIndex, 0,ind_split,numvar+1))
							m_pSimDoc->SetMultiStorageBinFileMissing();

					}

			}

	
		if(m_pCUG->CreateZoomPlotWindow(TRUE)) {
			GetCell(col,row, &cell);
			SetCell(col,row, &cell);
			m_pCUG->UpdatePlotStatusBar(sheet, TRUE);
			return TRUE;
		}
		return FALSE;

}
BOOL ValidationCug::DefineNewSubPeriod() 
{
	float accaccsum, accsum;
	double mcorr;
	int index, countvar;
	F* pValFile;
	CResiduals *pRes;
	float meas;
	long start_min, end_min;
	long SplitPeriods=m_NumberNewSplitPeriods;

	CPG *pPG;


	m_pSimDoc->m_MStorage.AlloSplitPerformance(SplitPeriods);

	if(SplitPeriods<=0) {
		MessageBox(NULL,"Reset to no split periods mode !",MB_OK);
		OnSheetSetup(0);
		return TRUE;
	}

	if(!m_pSimDoc->m_MStorage.m_IsSplitOpen||!m_pSimDoc->m_MStorage.m_SplitPeriodFile.good()) {

		MessageBox(NULL,"Problems with definition of file to store results!",MB_OK);
		OnSheetSetup(0);
		return TRUE;

	}

	//VALIDATION_mp_MAXNUMBERVALIDATIONVARIABLES=m_NumValidVar;
	//VALIDATION_mp_ALLO();

	/*	CFileScanner ScanMeanProg(NULL);
		ScanMeanProg.Create(IDD_PROGRESS,NULL);
		ScanMeanProg.ShowWindow(SW_SHOW);
		ScanMeanProg.m_Info="Calculating new performance indicators";*/
	long previous=-1;

	vector<size_t> recordsa;
	vector<CResiduals*> ResidualPtr;
	vector<CPG*> PGPtr;
	vector<size_t> Valid;
	Valid.resize(MAXSIMVAL);
	PGPtr.resize(MAXSIMVAL);
	CString str=m_pSimDoc->GetMBinFileName().c_str();
	int addedfiles=m_pSimDoc->m_MStorage.GetNumberOfAddedFiles(string(str));
	recordsa.resize(MAXSIMVAL*(SplitPeriods+1));
	ResidualPtr.resize(MAXSIMVAL*(addedfiles+1));
	countvar=0;

	for(int i=0;i<MAXSIMVAL;i++) {
				pValFile=m_pSimDoc->ValidationFilePointer(i+1);
				Valid[i]=0;
				if(pValFile->GetNumberOfSelectedVal()>0) {
					Valid[i]=1;
					countvar+=pValFile->GetNumberOfSelectedVal();
				
					pPG=m_pSimDoc->m_PlotStorage.GetPGPointer(0, i+1,0, TRUE);
					PGPtr[i]=pPG;
					//pPG=(CPG*)pValFile->GetPointer();
					if(pPG==NULL) {
							string name=m_pSimDoc->GetOutputFileName(i+1);
							pPG=new CPG;
							pPG->Open(name);
							pPG->CloseFile();
					}

					PGPtr[i]=pPG;
					pPG->ReOpen();
	
					long numrec=pPG->GetNumRecords();
					for (int j=0;j<=SplitPeriods;j++) {
						int indexrec=i*(SplitPeriods+1)+j;
						recordsa[indexrec]=numrec/SplitPeriods*j;
					}

					for(int j=0;j<=addedfiles;j++) {
						int index=(addedfiles+1)*i+j;

						if(j!=0) {
							pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(i+1, j);
							ResidualPtr[index]=pRes;
							if(!pRes->IsOpen()) {
								int f=0;

							}
						}
						else {
							pRes=(CResiduals*)m_pSimDoc->GetResidualPointer(i+1);
							ResidualPtr[index]=pRes;
							if(!pRes->IsOpen()) {
								int f=0;

							}
						}
					}
				}
	}



	accaccsum=0;
	index=-1;
	
	
	int countvar2=0;
	for(int i=0;i<MAXSIMVAL;i++) {

		if(Valid[i]>0) {
			pPG=PGPtr[i];
			long numrec=pPG->GetNumRecords();
			long numvar=pPG->GetNumVariables();
			start_min=pPG->GetLongTimeStart();
			end_min=pPG->GetLongTimeEnd();
			long nrec;

			//VALIDATION_mp_NUMVALID_COUNT=numvar/2;
			pValFile=m_pSimDoc->ValidationFilePointer(i+1);

			
			for (int jj=0;jj<numvar/2;jj++) {
				int indexval;
				indexval=jj+1;

				int index_vallist;

				index_vallist=pValFile->GetValNumber(jj);

				accsum=0;
				nrec=0;
				index++;	
				mcorr=0.0;
/*				RelPscan=float(jj+countvar2)/float(countvar)*100;

				ScanMeanProg.ShowWindow(SW_SHOW);
				ScanMeanProg.Update();*/
				std::vector <float> meas_v,res_v, sim_v ;
				meas_v.resize(numrec);
				meas_v=pPG->GetVarVector(jj*2+2);
				res_v.resize(numrec);

				for (long irun=0;irun<m_pSimDoc->MR_Get_TotalNumberofRuns(); irun++) {


					long irun_int;
					int ifile;
					ifile=m_pSimDoc->MR_GetAddedFiles(irun);
					int index=(addedfiles+1)*i+ifile;
					pRes=(CResiduals*)ResidualPtr[index];
					if(ifile!=0) {
							irun_int=m_pSimDoc->MR_GetInternalRunNo(irun);
						}
					else {
						irun_int=irun;
					}
					res_v=pRes->GetResidualVector(irun_int, jj);
					sim_v.resize(numrec);
					for(long ii=0;ii<numrec;ii++)
						sim_v[ii]=meas_v[ii]-res_v[ii];
					
					STAT_PERF split;
					vector<double> sim, obs;
					for(int index_split=0; index_split<SplitPeriods;index_split++) {
						int indexrec=i*(SplitPeriods+1)+index_split;
						//VALIDATION_mp_POSTPREPARE();
						sim.clear(); obs.clear();
						for(unsigned long j=recordsa[indexrec];j<recordsa[indexrec+1];j++) {
							meas=meas_v[j];
							if(abs(meas)<.9E38) {
								sim.push_back(sim_v[j]);
								obs.push_back(meas);
								//VALIDATION_mp_POSTCHECK(&sim_v[j],&meas,&indexval);								
							}

						}
						split=StatUtil::Get_StatComp(&sim, &obs);
						m_pSimDoc->m_MStorage.SetPerformance(irun,  index_split, split);
					}

			


				}
				if(!m_pSimDoc->m_MStorage.SaveSplitData(index_vallist)) {
							MessageBox(NULL,"Problems when storing data",MB_OK);	
							OnSheetSetup(0);
							return FALSE;
				}			
	
			}
			countvar2+=numvar/2;

		}
		pPG->CloseFile();
		
	}
		
	//VALIDATION_mp_DEALLO();
//	ScanMeanProg.m_Info="Updating Accepted Runs and calculating statistics";
	m_pSimDoc->m_MStorage.VarValidAllo();
	m_pSimDoc->m_MStorage.UpdateAcceptedRuns(TRUE);
	
	if(SplitPeriods>0) {
		MessageBox(NULL,"Performance indicators estimated",MB_OK);
		OnSheetSetup(0);

	}

	return TRUE;
}