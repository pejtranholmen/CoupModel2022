#pragma once
#include "UGCtrl.h"
#include "..\WindowNames.h"
#include <map>
#include <vector>
#include <string>	

enum ButtonType {INFOBUTTON, LISTBUTTON, CLICKBUTTON, COMBOBOX, INFOONLYBUTTON, CHECKBOX};

class CPG;
struct CLICK_BUTTON {
	enum ButtonType Type;
	int Param;
	int Col_start;
	int Col_end;
	int Row;
	bool NormalState;
	std::vector<std::string> VectorNames;
	std::vector<bool> VectorState;
	bool VerticalDirection;
	std::string Text;
	std::string HintText;
	std::string TextOptional;
	CPG* pPG;
};
struct HINTLIST {
	int param;
	std::string HintText;
};
struct NEWPGFILE_INFO {
	int Param;
	size_t NumRec;
	size_t NumVar;
	size_t NumRep;
	size_t MinDiff;
	std::string FileName;
	std::string StartDate;
	bool Assigned;
	std::string HintText;

};
class CPG;
class PEJUGCtrl :public CUGCtrl
{

	// Construction
public:
	PEJUGCtrl();
	// Implementation
public:
	virtual ~PEJUGCtrl();
	bool GetDefaultFont(CFont &font);
	bool GetDefaultLargeFont(CFont &font);

protected:
	void SetNewCleanSheetWithNumber(int sheet);
	//void SetCellButton(BUTTON_INFO button, size_t ShiftPos=0);
	//void SetCellListButton(LIST_BUTTON_INFO button, size_t index = std::string::npos);

	void SetCellClickButton(CLICK_BUTTON button,size_t ShiftPos = 0, size_t ListIndex=0);
	void SetPGFileColumns(size_t NumCol, long Row=-1, bool OutPutName=false, int NumTopHeads=2);
	void SetPGFileColumnsInfo(CPG *pPG, int col_start=0, int NumCol=5, long row=0, bool OnlyStruc = false, bool WithFileName=false, long Param=0);
	void SetPGFileColumnsInfoButton(CLICK_BUTTON button, size_t shiftPos=0 , int NumCol=4);
	void SetCodeTranslateInfo(CLICK_BUTTON, std::vector<std::pair<std::string, int>>&, std::vector<std::string> unique_names, std::vector<size_t> num_of_names, std::map<int,size_t> hit_map, std::map<int,size_t> parc_hit_map);


	size_t GetUniqueMenuNumber();
	void SetLocalWindowText(std::string text, WINDOW_NAMES wname=WINDOW_PG);

	void SetColWidth(int col, int width);
	void SetRowHeight(long row, int Height);
	void SetTH_Height(int height);
	void SetTH_RowHeight(int row, int height);



	
	CFont GetDefaultLargeFont();

	std::string GetHint(int ParV);

	CLICK_BUTTON GetClick_BUTTON(int ParV);
	void SetClickButtonMap(CLICK_BUTTON);
	void SetHintMap(HINTLIST h) { if(h.param==0) h.param = GetUniqueMenuNumber(); m_HintMap.insert(std::pair<int, std::string>(h.param, h.HintText)); };


	HINTLIST m_DoubleClickEdit;
	HINTLIST m_RightClickOption;
	HINTLIST m_ChartVar;
	HINTLIST m_NoUse;

	COLORREF m_ColorClick, m_ColorInfo;
	COLORREF m_COLHEAD, m_ColorHeading;

	CWinApp *m_pApp;
	std::map<int, std::string> m_HintMap;
	std::map<int, std::string> m_ClickTextMap;
	std::map<int, CLICK_BUTTON> m_ClickButtonMap;
	float m_ScaleColWidth;
	float m_ScaleRowHeight;



private:


	size_t DefinedFixedHints();
	int m_UniqueNumber, m_callCount;
	CMainFrame *m_pMainFrame;

};