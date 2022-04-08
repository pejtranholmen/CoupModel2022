#include "..\StdAfx.h"
#include "PEJUGCtrl.h"
#include "..\..\Util.h"
#include "..\..\MainFrm.h"




PEJUGCtrl::PEJUGCtrl()
{
	m_ColorClick = RGB(240, 240, 50);
	m_ColorInfo = RGB(173, 255, 47);
	m_COLHEAD = RGB(240, 240, 240);
	m_UniqueNumber = 12000;
	m_callCount = 0;
	m_pMainFrame= MySingle::instance().GetFramePointer();
	m_ScaleColWidth = 1.;
	m_ScaleRowHeight = 1.;

}

PEJUGCtrl::~PEJUGCtrl()
{
}
void PEJUGCtrl::SetNewCleanSheetWithNumber(int sheet)
{
	SetSheetNumber(sheet);
	EnableMenu(TRUE);
	EnableUpdate(FALSE); while (GetNumberRows()>0) DeleteRow(0);	EnableUpdate(TRUE);
}


void PEJUGCtrl::SetCellClickButton(CLICK_BUTTON button, size_t ShiftPos, size_t ListIndex)
{
	CUGCell cell;
	if (button.Type == LISTBUTTON) {
		auto SetLine = [&](size_t i) {
			JoinCells(button.Col_start, i + button.Row, button.Col_end, i + button.Row);
			GetCell(button.Col_start, i + button.Row, &cell);
			cell.SetBackColor(m_ColorClick);
			cell.SetText(button.VectorNames[i]);
			cell.SetParam(button.Param);
			SetCell(button.Col_start, i + button.Row, &cell);
			QuickSetText(-1, i + button.Row, FUtil::STD_ItoAscii(i + 1));
		};
		auto SetCol = [&](size_t i) {
			JoinCells(button.Col_start+i,  button.Row, button.Col_end+i, button.Row);
			GetCell(button.Col_start+i, button.Row, &cell);
			cell.SetBackColor(m_ColorInfo);
			cell.SetText(button.VectorNames[i]);
			cell.SetParam(button.Param);
			SetCell(button.Col_start+i, button.Row, &cell);
		};
		if(button.VerticalDirection) for (size_t i = 0; i < button.VectorNames.size(); i++)	SetLine(i);
		else for (size_t i = 0; i < button.VectorNames.size(); i++)	SetCol(i);
	}
	else if (button.Type == COMBOBOX) {
		if (button.Col_start + ShiftPos<button.Col_end + ShiftPos)
			JoinCells(button.Col_start + ShiftPos, button.Row, button.Col_end + ShiftPos, button.Row);
		GetCell(button.Col_start + ShiftPos, button.Row, &cell);
		string optionlist="";
		cell.SetCellType(UGCT_DROPLIST);
		for (size_t i = 0; i < button.VectorNames.size(); i++)
			optionlist += button.VectorNames[i] + "\n";
		cell.SetLabelText(optionlist);
		if (button.VectorNames.size() < 1) {
			cell.SetReadOnly(true);
		}
		else
			cell.SetReadOnly(false);

		cell.SetText(button.Text);
		cell.SetParam(button.Param);
		SetCell(button.Col_start + ShiftPos, button.Row, &cell);

	}
	else if (button.Type == CHECKBOX) {
		auto SetBox = [&](size_t i) {
			if (button.VerticalDirection) {
				JoinCells(button.Col_start, i + button.Row, button.Col_end, i + button.Row);
				GetCell(button.Col_start, i + button.Row, &cell);
			}
			else {
				JoinCells(button.Col_start+i, button.Row, button.Col_end+i, button.Row);
				GetCell(button.Col_start+i, button.Row, &cell);
			}
			cell.SetCellType(UGCT_CHECKBOX);
			cell.SetText(button.VectorNames[i]);
			cell.SetLabelText(button.VectorNames[i]);
			cell.SetParam(button.Param);
			if (button.VectorState.size() > i) {
				if (button.VectorState[i])
					cell.SetBool(true);
				else
					cell.SetBool(false);
			}

			if (button.VerticalDirection)
				SetCell(button.Col_start, i + button.Row, &cell);
			else
				SetCell(button.Col_start + i, button.Row, &cell);
		};
		for (size_t i = 0; i < button.VectorNames.size(); i++)	SetBox(i);

	}
	else {
		if (button.Col_start + ShiftPos<button.Col_end + ShiftPos)
			JoinCells(button.Col_start + ShiftPos, button.Row, button.Col_end + ShiftPos, button.Row);
		GetCell(button.Col_start + ShiftPos, button.Row , &cell);
		if (!button.NormalState)
				cell.SetText(button.TextOptional);
		else
			cell.SetText(button.Text);
		
		if (button.Type == INFOONLYBUTTON) {
			cell.SetBackColor(m_ColorInfo);
			cell.SetReadOnly(true);
		}
		else {
			cell.SetBackColor(m_ColorClick);
			cell.SetReadOnly(false);
		}
		cell.SetParam(button.Param);

		SetCell(button.Col_start + ShiftPos, button.Row, &cell);
	}

}

void PEJUGCtrl::SetPGFileColumns(size_t NumCol, long row, bool OutPutName, int NumTopHead)
{
	if (row == -1) {
		if (NumTopHead==1) {
			SetRowHeight(-1, 30);
			SetTH_NumberRows(1);
			SetTH_Height(30);
			SetTH_RowHeight(-1, 30);
		}
		else if (NumTopHead==2) {
			SetRowHeight(-1, 60);
			SetTH_NumberRows(2);
			SetTH_Height(60);
			SetTH_RowHeight(-2, 30);
			SetTH_RowHeight(-1, 30);

		}

		SetColWidth(0, 300);
		SetColWidth(1, 70);
		SetColWidth(2, 70);
		SetColWidth(3, 70);
		if (NumCol > 4) {
			SetColWidth(4, 140);
			SetColWidth(5, 140);
			if (NumCol > 6)
				SetColWidth(6, 170);
			if(NumCol>7)
				SetColWidth(7, 170);
		}
	}
	if(OutPutName)
		QuickSetHeadingText(0, row, "Output File Name");
	else
		QuickSetHeadingText(0, row, "Input File Names");
	QuickSetHeadingText(1, row, "Records");
	QuickSetHeadingText(2, row, "Variables");
	QuickSetHeadingText(3, row, "Repetitions");
	if (NumCol > 4) {
		QuickSetHeadingText(4, row, "Start Date");
		QuickSetHeadingText(5, row, "End Date");
		if(NumCol>6)
			QuickSetHeadingText(6, row, "Time Interval(min)");
		if(NumCol>7)
			QuickSetHeadingText(7, row, "Scaling of File");
	}
}

void PEJUGCtrl::SetPGFileColumnsInfo(CPG *pPG, int col_start,int NumCol, long row, bool OnlyStruc, bool WithFileName, long Param)
{
	CUGCell cell;

	if (WithFileName) {
		GetCellIndirect(col_start, row, &cell);
		if (NumCol == 7)
			cell.SetParam(Param);
		else
			cell.SetParam(Param);
		cell.SetText(pPG->GetFileName());
		cell.SetBackColor(m_ColorClick);
		SetCell(col_start, row, &cell);
		col_start++;
	}
	QuickSetNumber(-1, row, row + 1);

	auto ParEdit = m_DoubleClickEdit.param;
	if (NumCol != 7) {
		ParEdit = 0;

	}

	GetCellIndirect(col_start, row, &cell); cell.SetNumber(pPG->GetNumRecords()); cell.SetParam(ParEdit); SetCell(col_start, row, &cell);
	GetCellIndirect(col_start+1, row, &cell); cell.SetNumber(pPG->GetNumVariables()); cell.SetParam(ParEdit); SetCell(col_start+1, row, &cell);
	GetCellIndirect(col_start+2, row, &cell); cell.SetNumber(pPG->GetNumRepititions()); cell.SetParam(ParEdit); SetCell(col_start+2, row, &cell);
	if (!OnlyStruc) {
		GetCellIndirect(col_start + 3, row, &cell); cell.SetText(pPG->GetStartDate()); cell.SetParam(ParEdit); SetCell(col_start + 3, row, &cell);
		QuickSetText(col_start + 4, row, pPG->GetEndDate());
	}
	if (NumCol == 7) {
		GetCellIndirect(6, row, &cell); cell.SetNumber(pPG->GetNormalTimeInterval()); cell.SetParam(ParEdit); SetCell(6, row, &cell);

	}

}

void PEJUGCtrl::SetPGFileColumnsInfoButton(CLICK_BUTTON button, size_t shiftPos, int NumCol)
{
	CUGCell cell;
	GetCellIndirect(button.Col_start, button.Row, &cell);
	cell.SetParam(button.Param);
	if (button.pPG != nullptr) cell.SetText(button.pPG->GetFileName());
	cell.SetBackColor(m_ColorClick);
	SetCell(button.Col_start, button.Row, &cell);
	auto ParEdit = m_DoubleClickEdit.param;
	bool ReadOnly = false;
	if (NumCol != 7) {
		ParEdit = 0;
		ReadOnly = true;

	}



	GetCellIndirect(button.Col_start+1, button.Row, &cell); cell.SetNumber(button.pPG->GetNumRecords()); cell.SetParam(ParEdit); cell.SetReadOnly(ReadOnly); SetCell(button.Col_start+1, button.Row, &cell);
	GetCellIndirect(button.Col_start+2, button.Row, &cell); cell.SetNumber(button.pPG->GetNumVariables()); cell.SetParam(ParEdit); cell.SetReadOnly(ReadOnly); SetCell(button.Col_start + 2, button.Row, &cell);
	GetCellIndirect(button.Col_start+3, button.Row, &cell); cell.SetNumber(button.pPG->GetNumRepititions()); cell.SetParam(ParEdit); cell.SetReadOnly(ReadOnly); SetCell(button.Col_start + 3, button.Row, &cell);


	if (NumCol>4) {
		GetCellIndirect(button.Col_start + 4, button.Row, &cell); cell.SetText(button.pPG->GetStartDate()); cell.SetParam(ParEdit); cell.SetReadOnly(ReadOnly); SetCell(button.Col_start + 4, button.Row, &cell);
		QuickSetText(button.Col_start + 5, button.Row, button.pPG->GetEndDate());
	}

	if (NumCol == 7) {
		GetCellIndirect(6, button.Row, &cell); cell.SetNumber(button.pPG->GetNormalTimeInterval()); cell.SetParam(ParEdit); cell.SetReadOnly(ReadOnly); SetCell(6, button.Row, &cell);

	}


}

size_t PEJUGCtrl::GetUniqueMenuNumber()
{
	if (m_callCount == 0) m_callCount = DefinedFixedHints();
	m_callCount++;
	return m_UniqueNumber+m_callCount;
}

void PEJUGCtrl::SetLocalWindowText(string text, WINDOW_NAMES wname)
{
	if(m_pMainFrame==nullptr) m_pMainFrame = MySingle::instance().GetFramePointer();
	if (m_pMainFrame != nullptr) {
		m_pMainFrame->UpdateLocalWindowText(text, wname);


	}
}
bool PEJUGCtrl::GetDefaultFont(CFont &font)
{
	if (&font!=nullptr ) {
		int fontsize = MFC_Util::GetFontSize();
		font.CreateFont(fontsize, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, _T("Arial"));
		m_ScaleColWidth = float(fontsize)/16.*1.2;
		m_ScaleRowHeight = float(fontsize)/16.;
		SetDefFont(&font);
		SetDefRowHeight(fontsize + 8);
		return true;
	}
	return false;

}
bool PEJUGCtrl::GetDefaultLargeFont(CFont &font)
{
	if (&font != nullptr) {
		int fontsize = int(float(MFC_Util::GetFontSize())*1.3);
		font.CreateFont(fontsize, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, _T("Arial"));
		return true;
	}
	return false;

}
void PEJUGCtrl::SetColWidth(int col, int width)
{
	int new_width;
	new_width = int(m_ScaleColWidth*float(width));
	CUGCtrl::SetColWidth(col, new_width);
}
void PEJUGCtrl::SetRowHeight(long row, int Height)
{
	int NewHeight;
	NewHeight = int(m_ScaleRowHeight*float(Height));

	CUGCtrl::SetRowHeight(row, NewHeight);
}
void PEJUGCtrl::SetTH_Height(int Height)
{
	int NewHeight;
	NewHeight = int(m_ScaleRowHeight*float(Height));
	CUGCtrl::SetTH_Height(NewHeight);
}
void PEJUGCtrl::SetTH_RowHeight(int row, int Height)
{
	int NewHeight;
	NewHeight = int(m_ScaleRowHeight*float(Height));
	CUGCtrl::SetTH_RowHeight(row, NewHeight);
}
string PEJUGCtrl::GetHint(int ParV)
{
	auto it = m_ClickButtonMap.find(ParV);
	if (it != m_ClickButtonMap.end()) {
		return (*it).second.HintText;
	}
	else {
		auto iter = m_HintMap.find(ParV);
		if (iter != m_HintMap.end()) {
			return (*iter).second;
		}
	}
	return "";
}

CLICK_BUTTON PEJUGCtrl::GetClick_BUTTON(int ParV)
{
	CLICK_BUTTON b;
	auto it = m_ClickButtonMap.find(ParV);
	if (it != m_ClickButtonMap.end()) {
		b = (*it).second;
	}
	return b;
}
void PEJUGCtrl::SetClickButtonMap(CLICK_BUTTON c)
{
	m_ClickButtonMap.insert(pair<int, CLICK_BUTTON>(c.Param, c));

}





size_t PEJUGCtrl::DefinedFixedHints()
{
	m_DoubleClickEdit.param = m_UniqueNumber;
	m_DoubleClickEdit.HintText = "Double Click to Edit";
	m_HintMap.insert(pair<int, string>(m_DoubleClickEdit.param, m_DoubleClickEdit.HintText));
	m_RightClickOption.param = m_UniqueNumber+1;
	m_RightClickOption.HintText = "Right Click to Select options";
	m_HintMap.insert(pair<int, string>(m_RightClickOption.param, m_RightClickOption.HintText));

	return 3;

}


