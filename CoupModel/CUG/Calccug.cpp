/////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived CalcCug

#include "..\stdafx.h"
#include "..\std.h"
#include "..\Util.h"
#include "..\resource.h"
#include "CalcCug.h"
#include "..\PG\CreatePG.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CalcCug,CUGCtrl)
	//{{AFX_MSG_MAP(CalcCug)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
IF_CONDITIONS CondTranslation(string str) {
	IF_CONDITIONS cond;
	cond.LeftVariable = std::string::npos;
	cond.RightVariable = std::string::npos;
	size_t pos_less = str.find('<');
	size_t pos_more = str.find('>');
	size_t pos_equal = str.find('=');
	size_t pos_left = str.find('(');
	size_t pos_right = str.find(')');
	size_t pos_left2 = str.rfind('(');
	size_t pos_right2 = str.rfind(')');
	size_t pos_min = str.find('m');
	size_t pos_day = str.find('d');
	if (str.size() == 0) return cond;
	size_t pos_cond, pos_func;
	cond.Function = NO_USE_FUNC;
	pos_func = std::string::npos;
	if (pos_min != std::string::npos) {
		cond.Function = MINUTENUMBERFUNC;
		pos_func = pos_min;
	}
	else if (pos_day != std::string::npos) {
		cond.Function = DAYNUMBERFUNC;
		pos_func = pos_day;
	}

	if (pos_less != std::string::npos&&pos_equal != std::string::npos) {
		cond.comp_method = SMALLER_EQUAL;
		pos_cond = pos_less;
	}
	else if (pos_less != std::string::npos&&pos_equal == std::string::npos) {
		cond.comp_method = SMALLER;
		pos_cond = pos_less;
	}
	else if (pos_more != std::string::npos&&pos_equal != std::string::npos) {
		cond.comp_method = GREATER_EQUAL;
		pos_cond = pos_equal;
	}
	else if (pos_more != std::string::npos&&pos_equal == std::string::npos) {
		cond.comp_method = GREATER;
		pos_cond = pos_more;
	}
	else if (pos_equal != std::string::npos) {
		cond.comp_method = EQUAL;
		pos_cond = pos_equal;
	}
	cond.LeftTimeFuncPos = false;
	if (pos_func < pos_cond) cond.LeftTimeFuncPos = true;


	if (pos_left != std::string::npos&&pos_left < pos_cond) {
		cond.LeftVariable = FUtil::AtoInt(str.substr(pos_left+1, pos_right - pos_left - 1));
		cond.LeftValue = MISSING;
	}
	else if(pos_func== std::string::npos|| pos_func >pos_cond) {
		cond.LeftValue=FUtil::AtoFloat(str.substr(0, pos_cond));	
	}
	if (pos_left2 != std::string::npos&&pos_left2 > pos_cond) {
		cond.RightVariable = FUtil::AtoInt(str.substr(pos_left2+1, pos_right2 - pos_left2 - 1));
		cond.RightValue = MISSING;
	}
	else if(pos_func == std::string::npos || pos_func <pos_cond)
		cond.RightValue = FUtil::AtoFloat(str.substr(pos_cond+1));

	return cond;
}

string CondString(IF_CONDITIONS cond) {
	string out;
	if (cond.LeftVariable != std::string::npos) {
		out = "V(" + FUtil::STD_ItoAscii(cond.LeftVariable) + ")";
	}
	else if (cond.Function > NO_USE_FUNC&&cond.LeftTimeFuncPos) {
		if (cond.Function == DAYNUMBERFUNC)
			out = 'd';
		else if (cond.Function == MINUTENUMBERFUNC)
			out = 'm';
		else
			out = "";
	}
	else
		out = FUtil::STD_FtoAscii(cond.LeftValue);
	switch (cond.comp_method) {
		case SMALLER:out += "<";break;
		case SMALLER_EQUAL:	out += "<="; break;
		case EQUAL:	out += "=";	break;
		case GREATER_EQUAL:	out += ">=";break;
		case GREATER:	out += ">";	break;
	}
	if (cond.RightVariable != std::string::npos) {
		out += "V(" + FUtil::STD_ItoAscii(cond.RightVariable) + ")";
	}
	else if (cond.Function > NO_USE_FUNC&&!cond.LeftTimeFuncPos) {
		if (cond.Function == DAYNUMBERFUNC)
			out += 'd';
		else if (cond.Function == MINUTENUMBERFUNC)
			out += 'm';
		else
			out += "";
	}
	else
		out += FUtil::STD_FtoAscii(cond.RightValue);
	return out;
};


/////////////////////////////////////////////////////////////////////////////
// CalcCug construction/destruction
void CalcCug::Init(int window,void *pPG, void *pDlg)
{


	m_expression=1;
	m_window=window;
	m_pPG=(CPG*)pPG;
	m_pCUG=(CalcCug*)pDlg;
	m_pDlg=nullptr;
	m_SelectInput=TRUE;
	m_VarSelectCurrent=0;
	DefineClickButtonInfos();
	DefineHintList();


}
void CalcCug::SetpDlg(CDialog *pdlg)
{
	m_pDlg=pdlg;
}
CalcCug::CalcCug()
{
//	UGXPThemes::UseThemes(true);
	m_bDrawExcelFocus = FALSE;
	m_col=2;
	m_row=2;
	expression=1;
	m_expression_type=1;
	NumSelVariables=1;
	m_largeFont.CreateFont(18,0,0,0,600,0,0,0,0,0,0,0,0,_T("Arial"));

	m_ColorChanged=RGB(255,200,200);
	m_ColorHeading=RGB(230,230,230);
	m_ColorClick=RGB(240,240,50);
	m_ColorSelected=RGB(255,200,200);
	m_NoColor=RGB(255,255,255);

	SetMultiSelectMode ( FALSE );
	DragDropTarget ( FALSE );
	for (int i=0;i<5;i++)
		m_VarValueChoice[i]=0;

}

CalcCug::~CalcCug()
{
	//UGXPThemes::CleanUp();
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid



void CalcCug::OnSetup()
{
	UseHints(TRUE);
	EnableMenu(TRUE);

	if(m_window!=2) {
		EnableUpdate(FALSE);
		while(GetNumberRows())
			DeleteRow(0);
		EnableUpdate(TRUE);
	}

	SetUniformRowHeight(TRUE);
	
	CUGCell cell; 
	CString str;
	float coef;
	int isel;
	CString optionlist;
	NumSelVariables=m_pPG->GetNumOfSelected();
	m_expression_type=m_pPG->GetVarFunctionType();
	m_pPG->ReadFirst_Buffer();
	m_pPG->SetApplyConditions(!m_CondButton.NormalState);
	int cols = m_col,
		rows = m_row+NumSelVariables;
	if(m_window==0) {
		EnableMenu(TRUE);
		m_pPG->GetDateSelection();
		if(m_expression_type==0) {
			if(expression==3)
				rows=5;
			else
				rows=5;

		}
		else if(m_expression_type==1){
			if(expression==1)
				rows=6;
			else if(expression==2||expression==3||expression==6)
				rows=5;
			else if(expression==4)
				rows=4;
			else
				rows=2;
			if(!m_CondButton.NormalState)
				rows += m_pPG->GetNumOfIfConditions()+2;
		}
		else if(m_expression_type==2)
			rows=m_row+m_pPG->GetNumOfSelected()+1;
		else if(m_expression_type==3) {
			rows=2;
			cols=2;
	
		}
		else if(m_expression_type==6) {
			rows=2;
			cols=2;
	
		}
		SetNumberCols ( cols );
		if(m_pPG->GetDateSelection())
			rows+=3;
		SetNumberRows ( rows);
		SetRowHeight(-1, 60);
		SetTH_NumberRows(2);
		SetTH_RowHeight(-1, 30);
		SetTH_RowHeight(-2, 30);
		QuickSetText(0,-1,"Calculation Function");
		SetColWidth(0,400);
		SetColWidth(1,220);
		SetColWidth(-1,0);
		

		
		GetCell(0,0, &cell); 
		if(m_expression_type>0) {
			cell.SetCellType(UGCT_DROPLIST);
			str="2";
			optionlist="New Values based on one Variable\n";
			optionlist+="New Values based on Different Variables\n";
			optionlist+="New Values based on Date/Time\n";

			switch (m_expression_type) {
				case 1:
					cell.SetText("New Values based on one Variable");
					break;
				case 2:
					cell.SetText("New Values based on Different Variables");
					break;
				case 3:
					cell.SetText("New Values based on Date/Time");
					break;
			}
			m_pPG->SetVarFunctionType(m_expression_type);
			cell.SetLabelText(optionlist);
			
			cell.SetBackColor(RGB(50,240,240) );
		}
		else {
			cell.SetCellType(UGCT_NORMAL);
			cell.SetText("New Date/Time Variable");
		}
		
		SetCell(0,0, &cell);
		if(m_expression_type==0) {
			QuickSetText(1,-1,"");
			GetCell(0,1, &cell); 
			cell.SetCellType(UGCT_DROPLIST);
			expression=m_pPG->GetVarFunction();

			optionlist="Move Current Date/time backward\n";
			optionlist+="Move Current Date/time forward\n";
			optionlist+="Convert Variable to Current Date/time\n";
				switch (expression) {
				case 1:
					cell.SetText("Move Current Date/time backward");
					break;
				case 2:
					cell.SetText("Move Current Date/time forward");
					break;
				case 3:
					cell.SetText("Convert Variable to Current Date/time");
					str=MFC_Util::ItoAscii(m_pPG->GetCalcVar());			
					QuickSetText(1,0,_T("Input Var: ")+str);
					break;
			}
			m_pPG->SetVarFunction(expression);
			cell.SetLabelText(optionlist);
	
			cell.SetBackColor(RGB(50,240,240) );
			SetCell(0,1, &cell);
			QuickSetText(1,1,"Output Var: Date/time");
			if(expression<=2) {
				QuickSetText(0,2,"Coefficients for change");
				QuickSetBackColor(0,2,m_ColorHeading);
				JoinCells(0,2,1,2);
				QuickSetText(0,3,"Number of Days:");
				QuickSetText(0,4,"Number of Minutes");
				for (int i=0;i<2;i++) {
					isel=m_pPG->GetSelection(i);
					if(isel<0) {
						switch(i) {
							case 0:
								coef=0.;
								break;
							default:
								coef=1.;		
						}
						m_pPG->AddSelection(0, coef);
					}
					coef=m_pPG->GetSelectionWeight(i);
					QuickSetNumber(1,3+i,coef,false);
				}
			}
			if(expression==3) {
				QuickSetText(0,2,"Units and base date/time options for conversion");
				QuickSetBackColor(0,2,m_ColorHeading);
				JoinCells(0,2,1,2);
				GetCell(0,3, &cell); 
				cell.SetCellType(UGCT_DROPLIST);
				int expressiont=m_pPG->GetTimeConvUnit();

				optionlist="Time as Years\n";
				optionlist+="Time as Days\n";
				optionlist+="Time as Hours\n";
				optionlist+="Time as Minutes\n";
				switch (expressiont) {
				case 1:
					cell.SetText("Time as Years");
					break;
				case 2:
					cell.SetText("Time as Days");
					break;
				case 3:
					cell.SetText("Time as Hours");
					break;
				case 4:
					cell.SetText("Time as Minutes");
					break;
				}

				cell.SetLabelText(optionlist);
	
				cell.SetBackColor(RGB(50,240,240) );
				SetCell(0,3, &cell);
				QuickSetText(0,4,"Start Date :");
				string str1=PGUtil::StringDatum(m_pPG->GetTimeConvBase()).c_str();			
				QuickSetText(1,4,str1,false);
			}


		}
		else if(m_expression_type==1) {
			GetCell(1,0, &cell);
			cell.SetAlignment(UG_ALIGNLEFT);
			SetCell(1,0, &cell);

			SetCellClickButton(m_CondButton);
			if (m_pPG->GetNumOfIfConditions() > 0) m_ResetCondButton.NormalState = false;
			SetCellClickButton(m_ResetCondButton);

			GetCell(0,1, &cell); 
			int num=m_pPG->GetNumToBeCalc();
			bool first;
			first=true;
			BOOL jump;
			BOOL replace;
			jump=FALSE;
			replace=m_pPG->GetReplaceMode();
			if(replace)
				num=num/2;
			CString str_out;
			CString s1;
			long line=0;
			for(size_t irep=0; irep<m_pPG->GetNumRepititions();irep++) {
				for(size_t ii=0;ii<m_pPG->GetNumVariables();ii++) {

					if(m_pPG->GetToBeCalc(line)&&!jump) {
						if(m_pPG->GetNumRepititions()>1) {
							s1=MFC_Util::ItoAscii(ii+1)+"-"+MFC_Util::ItoAscii(irep+1);
						}
						else {
							s1=MFC_Util::ItoAscii(line+1);
						}
						
						if(first) {
							str=_T("(")+MFC_Util::ItoAscii(num)+") :"+s1;
							first=false;
							if(replace)
								str_out=_T("(")+MFC_Util::ItoAscii(num)+") :"+MFC_Util::ItoAscii(line+2);
							else
								str_out=str;
						}
						else {
							str+=_T(",")+s1;
							if(replace)
								str_out+=_T(",")+MFC_Util::ItoAscii(line+2);
							else
								str_out=str;
						}
						if(replace) {
							jump=TRUE;
						}
					}
					else if(jump)
						jump=FALSE;
					line++;
				}
			}

			//str=MFC_Util::ItoAscii(m_pPG->GetCalcVar());
			QuickSetText(1,0,_T("Input Var: ")+str);
			str=MFC_Util::ItoAscii(m_pPG->GetCalcVarNew());
			QuickSetText(1,1,_T("Output Var: ")+str_out);
			
			cell.SetCellType(UGCT_DROPLIST);
			optionlist="Scaling of existing values\n";
			optionlist+="Introducing missing values by defining valid range\n";
			optionlist+="Introducing missing values by defining non valid range\n";
			optionlist+="Introducing missing values by simple smoothing\n";
			optionlist+="Substitution of missing values by fixed value\n";
			optionlist+="Substitution of missing values by linear interpolation\n";
			optionlist+="Difference between previous and current value\n";
			auto DefineConditionLines = [&](size_t StartLine) {
				if (!m_CondButton.NormalState) {
					QuickSetHeadingText(0, StartLine, "Conditions");
					QuickSetHeadingText(1, StartLine, "Script");
					vector<IF_CONDITIONS> cond;
					cond = m_pPG->GetIfConditions();
					auto condno = [](size_t i) {
						string out;
						out = "Condition:(";
						out += FUtil::STD_ItoAscii(i + 1);
						out += ")";
						return out;
					};


					for (size_t i = 0; i < cond.size(); i++) {
						QuickSetNumber(-1, StartLine+1 + i, i);
						QuickSetText(0, StartLine + 1 + i, condno(i), true, m_ResetCondHint.param);
						QuickSetText(1, StartLine + 1 + i, CondString(cond[i]), false, m_ResetCondHint.param);

					}
					QuickSetText(0, StartLine + 1 + cond.size(), condno(cond.size()), m_ResetCondHint.param);
					QuickSetText(1, StartLine + 1 + cond.size(), "", false, m_NewCondHint.param);
				}

			};

			switch (expression) {
				case 1:
					cell.SetText("Scaling of existing values");	
					QuickSetText(0,2,"Coefficient");
					QuickSetBackColor(0,2,m_ColorHeading);
					QuickSetText(1,2,"Value");
					QuickSetBackColor(1,2,m_ColorHeading);
					QuickSetText(0,3,"Intercept (A0)");
					QuickSetText(0,4,"First order Slope (A1)");
					QuickSetText(0,5,"Second order (A2)");

					for (int i=0;i<3;i++) {
						isel=m_pPG->GetSelection(i);
						if(isel<0) {
							switch(i) {
								case 0:
								case 2:
									coef=0.;
									break;
								default:
									coef=1.;		
							}
							m_pPG->AddSelection(0, coef);
						}
						coef=m_pPG->GetSelectionWeight(i);
						string txt = FUtil::STD_FtoAscii(coef);
						QuickSetText(1,3+i,txt,false);
					}
					DefineConditionLines(6);

				
					break;
				case 2:
					cell.SetText("Introducing missing values by defining valid range");
					QuickSetText(0,2,"Limits");
					QuickSetBackColor(0,2,m_ColorHeading);
					QuickSetText(1,2,"Value");
					QuickSetBackColor(1,2,m_ColorHeading);
					QuickSetText(0,3,"Lower boundary");
					QuickSetText(0,4,"Upper boundary");
					for (int i=0;i<2;i++) {
						isel=m_pPG->GetSelection(i);
						if(isel<0) {
							switch(i) {
								case 0:
								case 2:
									coef=0.;
									break;
								default:
									coef=1.;		
							}
							m_pPG->AddSelection(0, coef);
						}
						coef=m_pPG->GetSelectionWeight(i);
						QuickSetNumber(1,3+i,coef,false);
					}
					DefineConditionLines(5);
					QuickSetText(1,-1,"Input Var:");
					QuickSetNumber(1,0,m_pPG->GetCalcVar());
					break;
				case 3:
					cell.SetText("Introducing missing values by defining non valid range");
					QuickSetText(0,2,"Limits");
					QuickSetBackColor(0,2,m_ColorHeading);
					QuickSetText(1,2,"Value");
					QuickSetBackColor(1,2,m_ColorHeading);
					QuickSetText(0,3,"Lower boundary");
					QuickSetText(0,4,"Upper boundary");
					for (int i=0;i<2;i++) {
						isel=m_pPG->GetSelection(i);
						if(isel<0) {
							switch(i) {
								case 0:
								case 2:
									coef=0.;
									break;
								default:
									coef=1.;		
							}
							m_pPG->AddSelection(0, coef);
						}
						coef=m_pPG->GetSelectionWeight(i);
						QuickSetNumber(1,3+i,coef, false);
					}
					DefineConditionLines(5);
					QuickSetText(1,-1,"Input Var:");
					QuickSetNumber(1,0,m_pPG->GetCalcVar());
					break;
				case 4:
					cell.SetText("Substitution of missing values by fixed value");
					QuickSetText(1,2,"Value");
					QuickSetBackColor(1,2,m_ColorHeading);
					QuickSetBackColor(0,2,m_ColorHeading);
					QuickSetBackColor(0,3,m_ColorHeading);										
					isel=m_pPG->GetSelection(0);
					if(isel<0) {
						coef=0.;
						m_pPG->AddSelection(0, coef);
					}
					coef=m_pPG->GetSelectionWeight(0);
					QuickSetNumber(1,3,coef, false);
					QuickSetText(1,-1,"Input Var:");
					QuickSetNumber(1,0,m_pPG->GetCalcVar());
					DefineConditionLines(4);
					break;
				case 5:
					cell.SetText("Substitution of missing values by linear interpolation");
					QuickSetText(1,-1,"Input Var:");
					QuickSetNumber(1,0,m_pPG->GetCalcVar());
					break;
				case 6:  //
					cell.SetText("Introducing missing values by simple smoothing");
					QuickSetText(0,2,"Limits");
					QuickSetBackColor(0,2,m_ColorHeading);
					QuickSetText(1,2,"Value");
					QuickSetBackColor(1,2,m_ColorHeading);
					QuickSetText(0,3,"Lower boundary of increase");
					QuickSetText(0,4,"Upper boundary of decrease");
					for (int i=0;i<2;i++) {
						isel=m_pPG->GetSelection(i);
						if(isel<0) {
							switch(i) {
								case 0:
								case 2:
									coef=0.1f;
									break;
								default:
									coef=-0.05f;		
							}
							m_pPG->AddSelection(0, coef);
						}
						coef=m_pPG->GetSelectionWeight(i);
						QuickSetNumber(1,3+i,coef,false);
					}
					break;
				case 7:
					cell.SetText("Difference between previous and current value");
					QuickSetText(1,-1,"Input Var:");
					QuickSetNumber(1,0,m_pPG->GetCalcVar());
					break;
			}


			m_pPG->SetVarFunction(expression);
			cell.SetLabelText(optionlist);
			
			cell.SetBackColor(RGB(50,240,240) );
			
			SetCell(0,1, &cell);
		}
		else if(m_expression_type==2) {
			GetCell(1,0, &cell);
			cell.SetAlignment(UG_ALIGNCENTER);
			SetCell(1,0, &cell);
			QuickSetText(1,-1,"Number of Variables");
			GetCell(0,1, &cell); 
			cell.SetCellType(UGCT_DROPLIST);

			int num = m_pPG->GetNumToBeCalc();
			bool first;
			first = true;
			bool jump;
			bool replace;
			jump = false;
			replace = m_pPG->GetReplaceMode();
			
			if (replace)
				num = num / 2;
			CString str_out;
			CString s1;
			long line = 0;
			for (size_t irep = 0; irep<m_pPG->GetNumRepititions(); irep++) {
				for (size_t ii = 0; ii<m_pPG->GetNumVariables(); ii++) {

					if (m_pPG->GetToBeCalc(line) && !jump) {
						if (m_pPG->GetNumRepititions()>1) {
							s1 = MFC_Util::ItoAscii(ii + 1) + "-" + MFC_Util::ItoAscii(irep + 1);
						}
						else {
							s1 = MFC_Util::ItoAscii(line + 1);
						}

						if (first) {
							str = _T("(") + MFC_Util::ItoAscii(num) + ") :" + s1;
							first = false;
							if (replace)
								str_out = _T("(") + MFC_Util::ItoAscii(num) + ") :" + MFC_Util::ItoAscii(line + 2);
							else
								str_out = str;
						}
						else {
							str += _T(",") + s1;
							if (replace)
								str_out += _T(",") + MFC_Util::ItoAscii(line + 2);
							else
								str_out = str;
						}
						if (replace) {
							jump = TRUE;
						}
					}
					else if (jump)
						jump = FALSE;
					line++;
				}
			}






			str=MFC_Util::ItoAscii(m_pPG->GetCalcVarNew());
			QuickSetText(1,1,_T("Output Var: ")+str_out);

			optionlist="Mean Value of Different Variables\n";
			optionlist+="Weighted sum of Different Variables\n";
			optionlist+="Multiplication of Different Variables\n";
			optionlist+="Division of Different Variables\n";
			optionlist+="Substitute missing Values by one Selected Variable\n";
			switch (expression) {
				case 1:
					cell.SetText("Mean Value of Different Variables");
					break;
				case 2:
					cell.SetText("Weighted sum of Different Variables");
					break;
				case 3:
					cell.SetText("Multiplication of Different Variables");
					break;
				case 4:
					cell.SetText("Division of Different Variables");
					break;
				case 5:
					cell.SetText("Substitute missing Values by one Selected Variable");
					break;
			}
			m_pPG->SetVarFunction(expression);
			cell.SetLabelText(optionlist);
			
			cell.SetBackColor(RGB(50,240,240) );
			
			SetCell(0,1, &cell);
			GetCell(1,0, &cell); 
			cell.SetAlignment(UG_ALIGNCENTER);
			SetCell(1,0, &cell);
			QuickSetNumber(1,0,NumSelVariables);
			QuickSetText(0,2,"Variable No:");
			if(expression==2) {
				QuickSetText(1,2,"Weight");
				QuickSetBackColor(1,2,m_ColorHeading);

			}
			QuickSetBackColor(0,2,m_ColorHeading);
			for(int i=0;i<NumSelVariables;i++) {
			   QuickSetNumber(-1,i+3,i+1);
			   QuickSetNumber(0,i+3,m_pPG->GetSelection(i));
			   if(expression==2) {
				    QuickSetNumber(1,i+3,m_pPG->GetSelectionWeight(i),false);
			}
		}





		}
		else if(m_expression_type==3) {
			QuickSetText(1,-1,"");
			GetCell(0,1, &cell); 
			cell.SetCellType(UGCT_DROPLIST);

			optionlist="Variable values as daynumber of year\n";
			optionlist+="Variable values as month of year\n";
			optionlist+="Variable values as minute of day\n";
			optionlist+="Variable as days since start of serie\n";
			switch (expression) {
				case 1:
					cell.SetText("Variable values as daynumber of year");
					break;
				case 2:
					cell.SetText("Variable values as month of year");
					break;
				case 3:
					cell.SetText("Variable values as minute of day");
					break;
				case 4:
					cell.SetText("Variable as days since start of serie");
					break;
			}
			m_pPG->SetVarFunction(expression);
			cell.SetLabelText(optionlist);
			
			cell.SetBackColor(RGB(50,240,240) );
			
			SetCell(0,1, &cell);

			string str1=FUtil::STD_ItoAscii(m_pPG->GetCalcVarNew());
			QuickSetText(1,1,"Output Var: "+str1,false);
		}
		if(m_pPG->GetDateSelection()) {
				int ist=GetNumberRows()-3;
				QuickSetText(0,ist,"Calculations Restricted to a range of dates");
				JoinCells(0,ist,1,ist);
				QuickSetBackColor(0,ist,m_ColorHeading);
				QuickSetText(0,ist+1,"First Date:");
				QuickSetText(0,ist+2,"Last Date:");
				QuickSetText(1,ist+1,m_pPG->GetDate(m_pPG->GetCalcStart()));
				if(m_pPG->GetCalcStart()>1)
					QuickSetBackColor(1,ist+1, m_ColorChanged);
				QuickSetText(1,ist+2,m_pPG->GetDate(m_pPG->GetCalcStop()));
				if(m_pPG->GetCalcStop()<m_pPG->GetNumRecords())
					QuickSetBackColor(1,ist+2, m_ColorChanged);
		}
		
	}
	else if(m_window==1) {
		EnableMenu(TRUE); 
		m_row=m_pPG->GetNumVariables();
		m_expression_type=m_pPG->GetVarFunctionType();
		int repit=m_pPG->GetNumRepititions();

		if(repit>1) {
			SetColWidth(-1,50);
			SetSH_NumberCols(2);
			SetSH_ColWidth(-1,30);
			SetSH_ColWidth(-2,20);
		}
		else {
			SetSH_NumberCols(1);
			SetColWidth(-1,30);
		}
		SetNumberCols(2);
		SetColWidth(0, 130);
		SetColWidth(1, 110);

		JoinCells(0,-1,1,-1);
		QuickSetBackColor(0,-1,m_ColorClick);
		if(m_SelectInput&&m_expression_type==1||m_expression_type==0)
			QuickSetText(0,-1,"Select Input variable");
		else if(m_SelectInput&&m_expression_type==2)
			QuickSetText(0,-1,"Select Input variables");
		else
			QuickSetText(0,-1,"Select Output variable");
		SetNumberRows (m_row*repit);
		long line=0;
		for( int irep=0; irep<repit;irep++) {
			if(repit>1) {
				QuickSetNumber(-2,line, irep+1);
				JoinCells(-2, line,-2, line+m_row-1);
			}
			for(int i=0; i<m_row;i++) {
				QuickSetNumber(-1,line, i+1);
				QuickSetText(0,line,m_pPG->GetVarName(i+1, irep+1));
				QuickSetText(1,line,m_pPG->GetVarId(i+1, irep+1));
				if(m_pPG->GetToBeCalc(line)&&m_expression_type==1){
					QuickSetBackColor(0,line,m_ColorSelected);
					QuickSetBackColor(1,line,m_ColorSelected);
				}
				line++;
			}
		}

		


	}
	else if(m_window==2) {
		SetNumberCols(1);
		SetColWidth(0,215);
		SetRowHeight(-1, 120);
		SetColWidth(-1, 90);
		SetTH_NumberRows(4);
		SetTH_RowHeight(-1, 30);
		SetTH_RowHeight(-2, 30);
		SetTH_RowHeight(-3, 30);
		SetTH_RowHeight(-4, 30);
	
		m_DateSelection = m_pPG->GetDateSelection();
		
	
		EnableMenu(TRUE); 
		if(m_DateSelection) {
			if(GetNumberRows()==0) {
				m_row=m_pPG->GetNumRecords();
				QuickSetText(0, -2, "Seletect by click on line");
				QuickSetText(-1, -1, "Record No");
				SetNumberRows(m_row);
				QuickSetBackColor(0,-1,m_ColorClick);

				if(m_StartSelect)
					QuickSetText(0,-1,"Set Start");
				else
					QuickSetText(0,-1,"Set Stop");
				m_pPG->ReadFirst_Buffer();
				for(long i=0;i<m_row;i++) {
					QuickSetNumber(-1,i, i+1);
					QuickSetText(0,i,m_pPG->GetDate(i+1));
				}
			}
			else {

				if(m_StartSelect)
					QuickSetText(0,-1,"Set Start");
				else
					QuickSetText(0,-1,"Set Stop");

				m_row=GetNumberRows();
				long selstop=m_pPG->GetCalcStop();
			}
			long selstart=m_pPG->GetCalcStart();
			long selstop=m_pPG->GetCalcStop();

			if (selstop <= selstart) {
				selstart = 1;
				selstop = m_row;

			}
			QuickSetText(0, -4, "Start:" + FUtil::STD_ItoAscii(selstart));
			QuickSetText(0, -3, "End:" + FUtil::STD_ItoAscii(selstop));
			for(long i=0;i<m_row;i++) {
				if(i+1>=selstart&&i+1<=selstop)
					QuickSetBackColor(0,i,m_ColorSelected);
			}

		}
		else {
			QuickSetText(0,-1,"Date Selection");
			EnableUpdate(FALSE);
			while(GetNumberRows()>0)
				DeleteRow(0);
			EnableUpdate(TRUE);
		}


	}
	else if(m_window==4) {
		EnableMenu(TRUE); 
		m_row=m_NumParCalcFunc;
		SetNumberCols(2);
		if (m_VarSelectCurrent > 0) {
			SetColWidth(1, 150);
			SetNumberCols(3);

		}
		SetTH_Height(0);
		SetColWidth(0,240);
		SetRowHeight(0, 40);
		SetNumberRows(m_row+1);
		JoinCells(0,0,1,0);
		QuickSetText(0,0, m_CalcFuncName);
		QuickSetFont(0,0,&m_largeFont);
		for(long i=0;i<m_row;i++) {
			QuickSetNumber(-1,i+1, i+1);
			if(m_VarSelectCurrent>0) {
				QuickSetText(0,i+1, m_CalcFuncPar[i]);
				GetCell(1,i+1, &cell);
				cell.SetCellType(UGCT_DROPLIST);
				optionlist="as Variable\n";
				optionlist+="as Value\n";
				switch (m_VarValueChoice[i]) {
					case 0:
						cell.SetText("as Variable");
						QuickSetNumber(2,i+1, m_VarSelect[i], false);
						if(m_VarSelectCurrent>0&&m_VarSelectCurrent==i+1)
							QuickSetBackColor(-1,m_VarSelectCurrent, m_ColorSelected);
						break;
					case 1:
						cell.SetText("as Value");
						QuickSetNumber(2,i+1, m_ParValue[i],false);
						break;
				}
				cell.SetLabelText(optionlist);
				cell.SetReadOnly(false);
			
				cell.SetBackColor(RGB(50,240,240) );
				SetCell(1,i+1, &cell);

				QuickSetBackColor(1,i+1,m_ColorClick);
				

			}
			else {
				QuickSetText(0,i+1, m_CalcFuncPar[i]);
				QuickSetNumber(1,i+1, m_ParValue[i]);
				QuickSetBackColor(1,i+1,RGB(255,255,255));
			}
		}

	}

	RedrawAll();

//	

	SetVScrollMode ( UG_SCROLLTRACKING );
	SetUniformRowHeight( TRUE );
	
}

/////////////////////////////////////////////////////////////////////////////
//	OnSheetSetup	
void CalcCug::OnSheetSetup(int sheetNumber)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the current cell in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int CalcCug::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		Sent when the top row or left column in the grid is about to move
//		A return of TRUE allows the move, a return of
//		FALSE stops the move
int CalcCug::OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	
void CalcCug::OnHitBottom(long numrows,long rowspast,long rowsfound)
{
}

/////////////////////////////////////////////////////////////////////////////
//	
void CalcCug::OnHitTop(long numrows,long rowspast)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeCol
//		Sent when the user is over a separation line on the top heading
//		A return value of TRUE allows the possibiliy of a resize
int CalcCug::OnCanSizeCol(int col)
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
void CalcCug::OnColSizing(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSized
//		This is sent when the user finished sizing the 
//		given column (see above for more details)
void CalcCug::OnColSized(int col,int *width)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeRow
//		Sent when the user is over a separation line on the side heading
//		A return value of TRUE allows the possibiliy of a resize
int  CalcCug::OnCanSizeRow(long row)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSizing
//	    Sent when the user is sizing a row The row that is being sized is given as 
//		well as the height. Plus the height can be modified 
//		at this point. This makes it easy to set min and max heights
void CalcCug::OnRowSizing(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSized
//		This is sent when the user is finished sizing hte
//		given row ( see above for more details)
void CalcCug::OnRowSized(long row,int *height)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeSideHdg
//		This is sent when the user moves into position 
//		for sizing the width of the side heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int CalcCug::OnCanSizeSideHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeTopHdg
//		This is sent when the user moves into position 
//		for sizing the height of the top heading
//		return TRUE to allow the sizing
//		or FALSE to not allow it
int CalcCug::OnCanSizeTopHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSizing
int CalcCug::OnSideHdgSizing(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSized
int CalcCug::OnSideHdgSized(int *width)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSized
int CalcCug::OnTopHdgSized(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSizing
int CalcCug::OnTopHdgSizing(int *height)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColChange
//		Sent whenever the current column changes
//		The old and the new columns are given
void CalcCug::OnColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowChange
//		Sent whenever the current row changes
//		The old and the new rows are given
void CalcCug::OnRowChange(long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellChange
//		Sent whenever the current cell changes rows or columns
void CalcCug::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnLeftColChange
//		Sent whenever the left visible column in the grid changes
void CalcCug::OnLeftColChange(int oldcol,int newcol)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopRowChange
//		Sent whenever the top visible row in the grid changes
void CalcCug::OnTopRowChange(long oldrow,long newrow)
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
						  always represent total number of selection blocks.
Return:
	<none>
****************************************************/
void CalcCug::OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum)
{
	TRACE("CalcCug::OnSelectionChanged called with startCol = %ld, startRow = %ld, endCol = %ld, endRow = %ld, blockNum = %ld.\n",
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
void CalcCug::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
	CUGCell cell; 
	CString str;
	if(m_window==1&&!updn ) {
		if(m_pPG->GetReplaceMode()&&m_expression_type==1) return;
		m_expression_type=m_pPG->GetVarFunctionType();
		if(m_expression_type==0) {
			m_pPG->SetCalcVar(row+1);
			OnSetup();
		}
		else if(m_expression_type==1) {
			if(m_SelectInput) {
				if(m_pPG->GetToBeCalc(row)) {
					m_pPG->SetToBeCalc(row,FALSE);
				}
				else {
					m_pPG->SetToBeCalc(row,TRUE);
				}	
				OnSetup();
			}
			
				//m_pPG->SetCalcVarNew(row+1);
			
		}
		else if(m_expression_type==2) {
			if(m_SelectInput){
				m_pPG->AddSelection(row+1,1.);
				QuickSetBackColor(0,row,m_ColorClick);
			}
			//else
				//m_pPG->SetCalcVarNew(row+1);
	
			RedrawAll();
		}
		else if(m_expression_type==3) {
			//m_pPG->SetCalcVarNew(row+1);
			RedrawAll();
			OnSetup();
		}
		m_pCUG->OnSetup();
	}
	else if(m_window==2&&!updn) {
		QuickSetBackColor(0,row+1,m_ColorSelected);
		if(m_StartSelect)  {
			for(int i=0; i<row+1;i++)
				QuickSetBackColor(0,i,m_NoColor);			
			m_pPG->SetCalcStart(row+1);

		}
		else { 
			m_pPG->SetCalcStop(row+1);
			for(int i=row+1; i<m_row; i++)
				QuickSetBackColor(0,i,m_NoColor);

		}
		if(m_StartSelect)
			m_StartSelect=FALSE;
		else
			m_StartSelect=TRUE;
		OnSetup();
		m_pCUG->OnSetup();
	}



	else if(m_window==4&&!updn) {
		if(row>0) {
			if(	m_VarValueChoice[row-1]==0) {
				if(m_VarSelectCurrent>0)
					QuickSetBackColor(-1,m_VarSelectCurrent, m_ColorHeading);
				m_VarSelectCurrent=row;
				QuickSetBackColor(-1,row, m_ColorSelected);
			}
			else if(m_VarValueChoice[row-1]==1) {
				;
			}
		}

	}
//if ( !updn )
	/*{
		int stCol, edCol;
		long stRow, edRow;

		m_GI->m_multiSelect->GetTotalRange( &stCol, &stRow, &edCol, &edRow );

		if ( stCol == edCol && stRow == edRow )
			StartEdit();
	}*/

/*	if ( !updn )
	{
		int stCol, edCol;
		long stRow, edRow;

		m_GI->m_multiSelect->EnumNextBlock ( &stCol, &stRow, &edCol, &edRow );
		TRACE ( "%d, %d - %d, %d\n", stCol, stRow, edCol, edRow );
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
void CalcCug::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
//		Sent whenever the user double clicks the left mouse button within the grid
//	
//		'col' and 'row' are negative if the area clicked in is not a valid cell
//		'rect' the rectangle of the cell that was clicked in
//		'point' the point where the mouse was clicked
void CalcCug::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	StartEdit();
}				 

/////////////////////////////////////////////////////////////////////////////
//	OnMouseMove
void CalcCug::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user clicks the left mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void CalcCug::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	CUGCell cell;
	int isheet = GetSheetNumber();
	GetCell(col, row, &cell);
	int ParV = cell.GetParam();
	if (ParV == m_CondButton.Param&& !updn) {
		m_pPG->SetApplyConditions(m_CondButton.NormalState);
		if (m_CondButton.NormalState)
			m_CondButton.NormalState = false;
		else
			m_CondButton.NormalState = true;
		OnSetup();
	}
	else if (ParV == m_ResetCondButton.Param && !updn) {
		if (!m_ResetCondButton.NormalState)
			m_pPG->ResetIfVector();
		OnSetup();
	}

	m_expression_type=m_pPG->GetVarFunctionType();
	if(!updn&&m_window==1&&m_expression_type==1) {
		if(m_SelectInput)
			m_SelectInput=FALSE;
		else
			m_SelectInput=TRUE;
		OnSetup();
	}
	else if(!updn&&m_window==1&&m_expression_type==2) {
		if(m_SelectInput)
			m_SelectInput=FALSE;
		else
			m_SelectInput=TRUE;
		OnSetup();
	}
	else if(!updn&&m_window==2) {
		if (!m_DateSelection) {
			m_pPG->SetDateSelection(true);
			m_StartSelect = true;
		}
		else {
			if (m_StartSelect)
				m_StartSelect = FALSE;
			else
				m_StartSelect = TRUE;
		}
		OnSetup();
	}

	if ( !updn )
	{
		SelectRange ( col, 0, col, GetNumberRows()-1 );
	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_RClicked
//		Sent whenever the user clicks the right mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'col' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void CalcCug::OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top heading
//	
//		'col' is negative if the area clicked in is not valid
void CalcCug::OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_LClicked
//		Sent whenever the user clicks the left mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void CalcCug::OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if ( !updn )
		SelectRange ( 0, row, GetNumberCols()-1, row );
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_RClicked
//		Sent whenever the user clicks the right mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	
//		'row' is negative if the area clicked in is not valid
//		'updn'  TRUE if the button is down FALSE if the button just when up
void CalcCug::OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_DClicked
//		Sent whenever the user double clicks the left mouse button within the side heading
//	
//		'row' is negative if the area clicked in is not valid
void CalcCug::OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_LClicked
//		Sent whenever the user clicks the left mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void CalcCug::OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
	if(m_window==2&&!updn) {
		if(m_DateSelection) {
			m_DateSelection=FALSE;
			m_pPG->SetDateSelection(FALSE);
			m_pPG->SetCalcStart(1);
			m_pPG->SetCalcStop(m_pPG->GetNumRecords());
		}
		else {
			m_DateSelection=TRUE;
			m_pPG->SetCalcStart(1);
			m_pPG->SetCalcStop(m_pPG->GetNumRecords());
			m_pPG->SetDateSelection(TRUE);	
		}
		OnSetup();
		m_pCUG->OnSetup();

	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_RClicked
//		Sent whenever the user clicks the right mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	
//		'updn'  TRUE if the button is down FALSE if the button just when up
void CalcCug::OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top corner button
void CalcCug::OnCB_DClicked(RECT *rect,POINT *point,BOOL processed)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyDown
//		Sent whenever the user types when the grid has focus. The keystroke can be
//		modified here as well. (See WM_KEYDOWN for more information)
void CalcCug::OnKeyDown(UINT *vcKey,BOOL processed)
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
void CalcCug::OnCharDown(UINT *vcKey,BOOL processed)
{
}
	
/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent everytime the grid needs to
//		draw a cell in the grid. At this point the cell
//		class has been filled with the information to be
//		used to draw the cell. The information can now be
//		changed before it is used for drawing
void CalcCug::OnGetCell(int col,long row,CUGCell *cell)
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
void CalcCug::OnSetCell(int col,long row,CUGCell *cell)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnDataSourceNotify
//		This message is sent from a data source , message
//		depends on the data source - check the information
//		on the data source(s) being used
//		- The ID of the Data source is also returned
void CalcCug::OnDataSourceNotify(int ID,long msg,long param)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellTypeNotify
//		This message is sent from a cell type , message
//		depends on the cell type - check the information
//		on the cell type classes
//		- The ID of the cell type is given
int CalcCug::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	
	CUGCell cell;
	CString test;


//******* Amend the cell using 'Get..Set'


	if(ID==1 && msg == 101) {
			GetCell(col,row, &cell);
			m_Prior=cell.GetText();

	}
	else if(ID==1 && msg == 103){
			GetCell(col,row, &cell);
			m_After=cell.GetText();
			if(m_window==4) {
				if(col==1&&row>0) {
							if(m_After=="as Variable")
								m_VarValueChoice[row-1]=0;
							else {
								m_VarValueChoice[row-1]=1;
								if(m_VarSelectCurrent==row)
									 m_VarSelectCurrent=0;
							}

				}
				
			}
			else if(row==0) {
				
				if(m_After=="New Values based on one Variable") {
					m_expression_type=1;
				}
				else if (m_After=="New Values based on Different Variables") {
					m_expression_type=2;
				}
				else if (m_After=="New Values based on Date/Time") {
					m_expression_type=3;
				}
				if(m_Prior!=m_After) {
					m_pPG->SetVarFunctionType(m_expression_type);
					m_pPG->ResetSelections();
					m_pCUG->OnSetup();

				}


			}
			else if(row==1) {
			if(m_Prior!=m_After) {
				if(m_After=="Mean Value of Different Variables"||m_After=="Scaling of existing values"
					||m_After=="Variable values as daynumber of year"||m_After=="Move Current Date/time backward") {
					expression=1;

				}
				else if (m_After=="Weighted sum of Different Variables"||m_After=="Introducing missing values by defining valid range"
					||m_After=="Variable values as month of year"||m_After=="Move Current Date/time forward") {
					expression=2;

				}
				else if (m_After=="Multiplication of Different Variables"||m_After=="Introducing missing values by defining non valid range"
					||m_After=="Variable values as minute of day"||m_After=="Convert Variable to Current Date/time") {
					expression=3;

				}
				else if (m_After=="Division of Different Variables"||m_After=="Substitution of missing values by fixed value"
					||m_After=="Variable as days since start of serie") {
					expression=4;

				}
				else if(m_After=="Substitution of missing values by linear interpolation"||m_After=="Substitute missing Values by one Selected Variable"){
					expression=5;
				}
				else if(m_After=="Introducing missing values by simple smoothing"){
					expression=6;
					m_pPG->SetSelectionWeight(0,0.3f);
					m_pPG->SetSelectionWeight(1,-0.1f);
				}
				else if(m_After=="Difference between previous and current value"){
					expression=7;
				}
				m_pPG->SetVarFunction(expression);


			}
			}
			else if(row==3) {
			if(m_Prior!=m_After) {
				if(m_After=="Time as Years") {
					expression=1;

				}
				else if (m_After=="Time as Days") {
					expression=2;

				}
				else if (m_After=="Time as Hours") {
					expression=3;

				}
				else if (m_After=="Time as Minutes") {
					expression=4;

				}

				m_pPG->SetTimeConvUnit(expression);


			}

		}
		OnSetup();
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditStart
//		This message is sent whenever the grid is ready to start editing a cell
//		A return of TRUE allows the editing a return of FALSE stops editing
//		Plus the properties of the CEdit class can be modified
int CalcCug::OnEditStart(int col, long row,CWnd **edit)
{


		if(m_window==4&&m_VarSelect&&row>0) {
					if(m_VarValueChoice[row-1]==0)
						return FALSE;
		}
		
		return TRUE;

}

/////////////////////////////////////////////////////////////////////////////
//	OnEditVerify
//		This is send when the editing is about to end
int CalcCug::OnEditVerify(int col, long row,CWnd *edit,UINT *vcKey)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int CalcCug::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR st,BOOL cancelFlag)
{
	CString str;
	edit->GetWindowText(str);

	CUGCell cell;
	int isheet = GetSheetNumber();
	GetCell(col, row, &cell);
	int ParV = cell.GetParam();





	if(m_window==4) {
		if(m_VarSelectCurrent>0) {
			if(row>0) {
				m_VarSelectCurrent=row;
				if(m_VarValueChoice[m_VarSelectCurrent-1]==0)
					return FALSE;
				else if(col==2) {
					float v=MFC_Util::AtoFloat(str);
					m_ParValue[m_VarSelectCurrent-1]=v;
				}


			}


		}
		else {
			if(col==1&&row>0) {
				float v=MFC_Util::AtoFloat(str);
				m_ParValue[row-1]=v;
			}
		}


	}
	else {
		int expression=m_pPG->GetVarFunction();
		if (ParV == m_NewCondHint.param) {
			string str1 = CT2A(str).m_psz;
			if (str1.size() > 3) {
				auto cond = CondTranslation(str1);
				m_pPG->SetIfConditions(cond);
				OnSetup();
			}
			return TRUE;
		}


		if(col==1&&(m_expression_type==2||m_expression_type==1)) {
			if (row < 6) {
				float v = MFC_Util::AtoFloat(str);
				m_pPG->SetSelectionWeight(row - 3, v);
			}
			else if (row > 6) {
				GetCell(-1, row, &cell);
				size_t index = size_t(cell.GetNumber()+1.e-7);
				bool Error = true;
				if (index != std::string::npos&&index<m_pPG->GetNumOfIfConditions()) {
					Error = false;
					string str1 = CT2A(str).m_psz;
					auto cond = CondTranslation(str1);
					if (m_pPG->SetIfConditions(index, cond))
						OnSetup();
					else
						Error = true;

				}
				if (Error) MFC_Util::MessageBox("Error when trying to modify conditions for calculations");
				return TRUE;

			}
		}
		else if(col==1&&(m_expression_type==0&&expression==3)&&row==4) {
			string s=CT2A(str).m_psz;
			long min=PGUtil::MinutConv(s);
			m_pPG->SetTimeConvBase(min);
			OnSetup();

		}
		else if(col==1&&m_expression_type==0&&expression<=2) {
			float v=MFC_Util::AtoFloat(str);
			m_pPG->SetSelectionWeight(row-3, v);
		}
	}
	//QuickSetText(col,row,str);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish this is send when editing is finished
int CalcCug::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	sections - UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//				UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
void CalcCug::OnMenuCommand(int col,long row,int section,int item)
{
	CUGCell cell; 
	if(m_window==2) {
		if(item==1000){
			m_DateSelection=FALSE;
			m_pPG->SetDateSelection(FALSE);
			m_pPG->SetCalcStart(1);
			m_pPG->SetCalcStop(m_pPG->GetNumRecords());
		}
		else if(item==2000) {
			m_DateSelection=TRUE;
			m_pPG->SetCalcStart(1);
			m_pPG->SetCalcStop(m_pPG->GetNumRecords());
			m_pPG->SetDateSelection(TRUE);		
		}
		OnSetup();
		m_pCUG->OnSetup();
	}
	else if(m_window==1) {


		if(item==1000) {
			m_pPG->ResetSelections();
		}
		OnSetup();
		m_pCUG->OnSetup();

	}
	if (item == m_ResetCondHint.param) {
		m_pPG->ResetIfVector();
		OnSetup();

	}


}

/////////////////////////////////////////////////////////////////////////////
//	return UG_SUCCESS to allow the menu to appear
//	return 1 to not allow the menu to appear
int CalcCug::OnMenuStart(int col,long row,int section)
{	CUGCell cell;
	EmptyMenu();
	GetCell(col, row, &cell); 
	auto ParV = cell.GetParam();
	if (ParV ==m_ResetCondHint.param) {
		AddMenuItem(m_ResetCondHint.param, "Reset the Current Conditions");
		return TRUE;
	}
			//****** Populate the Menu 
	if(section==UG_GRID) {
		
			AddMenuItem(1000, "Cut"); 
			AddMenuItem(2000, "Copy"); 
			AddMenuItem(3000, "Paste"); 
	}
	if(section==UG_CORNERBUTTON) {
		m_expression_type=m_pPG->GetVarFunctionType();
		if(m_window==1){
			if(m_expression_type==2) {
				EmptyMenu();
				if(m_pPG->GetNumOfSelected()>0)
					AddMenuItem(1000, "Reset Variable Selections"); 				
			}
			else
				return FALSE;
		}
		else if(m_window==2){
			EmptyMenu();
			if(m_DateSelection)
				AddMenuItem(1000, "Reset Date Selections"); 
			else
				AddMenuItem(2000, "Make Date Selection"); 
		}

	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHint
int CalcCug::OnHint(int col,long row,int section,CString *str1)
{
	CUGCell cell;
	long ParV;
	UNREFERENCED_PARAMETER(section);
	int isheet = GetSheetNumber();

	GetCell(col, row, &cell);
	ParV = cell.GetParam();
	if (ParV > 0) {
		std::string txt = GetHint(ParV);
		if (txt.size() > 0) *str1 = txt.c_str();
		return TRUE;
	}





	if(m_VarSelectCurrent>0) {
		if(row>0) {
			if(row>0&&col==1)
				str1->Format(_T("Click to shift between single value and variable value inputs"));
			else if(row>0&&m_VarValueChoice[row-1]==0)	
				str1->Format(_T("Click on row in the description of input file variable to select input variable %d "),m_VarSelectCurrent );
			if(row>0&&m_VarValueChoice[row-1]==1&&col==2)
				str1->Format(_T("Double click to edit value"));
			return TRUE;
		}
		return FALSE;
	}
	else if(m_window==4) {
		if(col==1&&row>0) {
			str1->Format(_T("Double click to edit value"));
			return TRUE;
		}
		return FALSE;

	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnVScrollHint
int CalcCug::OnVScrollHint(long row,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHScrollHint
int CalcCug::OnHScrollHint(int col,CString *string)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OLE
#ifdef __AFXOLE_H__

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  CalcCug::OnDragEnter(COleDataObject* pDataObject){
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  CalcCug::OnDragOver(COleDataObject* pDataObject,int col,long row){
	//return DROPEFFECT_NONE;
	return DROPEFFECT_COPY;
}

/////////////////////////////////////////////////////////////////////////////
//	
DROPEFFECT  CalcCug::OnDragDrop(COleDataObject* pDataObject,int col,long row){
	return DROPEFFECT_COPY;
}
#endif

/////////////////////////////////////////////////////////////////////////////
//	
void CalcCug::OnScreenDCSetup(CDC *dc,int section){
}

/////////////////////////////////////////////////////////////////////////////
//	OnSortEvaluate
//	return		-1  <
//				0	==
//				1	>
int CalcCug::OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags)
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
void CalcCug::OnTabSelected(int ID)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnAdjustComponentSizes
void CalcCug::OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,
		RECT *cnrBtn,RECT *vScroll,RECT *hScroll,RECT *tabs)
{
} 

/////////////////////////////////////////////////////////////////////////////
//	OnDrawFocusRect
void CalcCug::OnDrawFocusRect(CDC *dc,RECT *rect)
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

void CalcCug::DrawExcelFocusRect(CDC *dc,RECT *rect)
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

void CalcCug::DefineClickButtonInfos()
{
	CLICK_BUTTON i;
	i.Type = INFOBUTTON;
	i.Param = GetUniqueMenuNumber();
	i.Text = "Enable Conditions";
	i.TextOptional = "Disable Conditions";
	i.HintText = "By Adding Conditions you will constrain your calculations to the conditions\n"
	"that will be satisfied\n"
	"v(1)<=V(3) are valid code\n"
	"V(1)>2.0 are valid code to compare with a static value\n"
	"m>200 are valid code to the minute of day function with a static value\n"
	"d>120 are valid code to the day of year function with a static value\n"
	"The 5 Valid operators are: < <= = >= > \n"
	"Note that more than one conditions will be added type\n";

	i.Col_start = 1;
	i.Col_end = 1;
	i.Row = -2;
	SetClickButtonMap(i);
	m_CondButton = i;

	i.Param = GetUniqueMenuNumber();
	i.Text = "No Conditons Set";
	i.TextOptional = "Reset Defined Conditions";
	i.HintText = "By Adding Conditions you will constrain your calculations to the conditions\n"
	"Set Conditions will be saved until you reset those"
	"Reset Conditions to start with new conditions"
	"You can also edit current conditions when they are visible after you Enable Conditions";

	i.Col_start = 0;
	i.Col_end = 0;
	i.Row = -2;
	SetClickButtonMap(i);
	m_ResetCondButton = i;

}

void CalcCug::DefineHintList()
{

	HINTLIST h;
	h.param = GetUniqueMenuNumber();
	h.HintText = "Edit this cell to Add a new condition - Note that you can Values by 3 methods:\n"
	"1 -Variables V(#) where # is the Variable Number\n"
	"2 -Time Function of Minutes of Day or Day of Year, codes are m or d for respectively function\n"
	"3 -A static value as specified on the line";


	SetHintMap(h); m_NewCondHint = h;
	h.param = GetUniqueMenuNumber();
	h.HintText = "Righ Click to get a Menu for Removing of Defined Conditions";
	SetHintMap(h); m_ResetCondHint = h;


}

/////////////////////////////////////////////////////////////////////////////
//	OnGetDefBackColor
COLORREF CalcCug::OnGetDefBackColor(int section)
{
	return GetSysColor ( COLOR_APPWORKSPACE );
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetFocus
void CalcCug::OnSetFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnKillFocus
void CalcCug::OnKillFocus(int section)
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapStart
BOOL CalcCug::OnColSwapStart(int col)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanColSwap
BOOL CalcCug::OnCanColSwap(int fromCol,int toCol)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTrackingWindowMoved
void CalcCug::OnTrackingWindowMoved(RECT *origRect,RECT *newRect)
{
}

