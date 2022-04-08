
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// DBCug.h : interface of the DBCug class
//
/////////////////////////////////////////////////////////////////////////////
#include "PFSheets.h"
#include "..\..\ModelTypes\Simb.h"
class m_pDoc;
class DB_BASCug;

class DBDataCug:public PFSheets
{
public:
	DBDataCug();
	~DBDataCug();
	CDB *m_pDB2;
	Doc *m_pDoc;
	BOOL m_bDrawExcelFocus;
	string m_Prior,m_After;

	unsigned int m_Min_NumHeads;
// font functions
	struct _tagFonts
	{
		int fontIndex;
		CFont font;
		_tagFonts *next;

		_tagFonts()
		{
			fontIndex = -1;
			next = nullptr;
		}
	} *m_fontsList;




protected:
	void DrawExcelFocusRect(CDC *dc,RECT *rect);

private:
	int m_DataBaseId;
	BOOL m_DataBase;
	Tab* m_pCTab;
	//DBCug *m_pCUG;
	CFont m_font, m_largeFont;
	string m_DataBaseString;
	DB_BASCug *m_pDB_Bas;
	CDialog *m_pSP_Retrieve_Dlg;
	int m_fontID;


	COLORREF m_ColorClick, m_ColorChanged, m_ColorHeading, m_ColorDisable;
	COLORREF m_ColorReadOnly,	m_ColorReadOnlyClick;

	Sw *m_pSwTimeRes;
	Sw *m_pSwPlantType;
	bool m_ShowAllObjects;
	size_t m_LatestSheet;
	
	string GetDataBaseName() {
		if(m_DataBaseId>0)
			return m_pDoc->DB_GetNames(m_DataBaseId+1);
		else
			return m_pDoc->DB_GetNames(m_DataBaseId);
	};



public:
	//***** Over-ridable Notify Functions *****

	long m_col,m_row;
	long m_toprow;
	BOOL m_CompleteList;
	BOOL m_SelOne;
	bool m_AllDataBaseItems;



	virtual void OnSetup();
	void SetDataBase(int ID) {m_DataBaseId=ID;};
	int GetDataBaseId() { return m_DataBaseId;};
	BOOL OnAdjustLeft(int col);
	void OnUpdateDataBase();
	virtual void OnSheetSetup(int sheetNumber);
	void Make_Sheet();
	void Make_GeneralInfoSheet();
	void Make_SwitchesSheet();
	void Make_ParameterSheet();
	void Make_ParameterTableSheet();
	void Make_TableElementSheet();
	void Make_ModelFileSheet();
	void Make_LinkToDocumentSheet();
	void Make_LinkToChangeDuringRunSheet();
	void Set_Changed_MR_LinkToDocument();
	void Set_Changed_LinkToChangeDuringRun_Document();

	//movement and sizing
	virtual int  OnCanMove(int oldcol,long oldrow,int newcol,long newrow);
	virtual int  OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow);
	virtual void OnHitBottom(long numrows,long rowspast,long rowsfound);
	virtual void OnHitTop(long numrows,long rowspast);
	
	virtual int  OnCanSizeCol(int col);
	virtual void OnColSizing(int col,int *width);
	virtual void OnColSized(int col,int *width);
	virtual int  OnCanSizeRow(long row);
	virtual void OnRowSizing(long row,int *height);
	virtual void OnRowSized(long row,int *height);

	virtual int  OnCanSizeTopHdg();
	virtual int  OnCanSizeSideHdg();
	virtual int  OnTopHdgSizing(int *height);
	virtual int  OnSideHdgSizing(int *width);
	virtual int  OnTopHdgSized(int *height);
	virtual int  OnSideHdgSized(int *width);
		
	virtual void OnColChange(int oldcol,int newcol);
	virtual void OnRowChange(long oldrow,long newrow);
	virtual void OnCellChange(int oldcol,int newcol,long oldrow,long newrow);
	virtual void OnLeftColChange(int oldcol,int newcol);
	virtual void OnTopRowChange(long oldrow,long newrow);
	virtual void OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum);

	//mouse and key strokes
	virtual void OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed);
	virtual void OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed);
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual void OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed=0);
	virtual void OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed=0);
	virtual void OnCB_DClicked(RECT *rect,POINT *point,BOOL processed=0);
	
	virtual void OnKeyDown(UINT *vcKey,BOOL processed);
	virtual void OnCharDown(UINT *vcKey,BOOL processed);
	
	//GetCellIndirect notification
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	//SetCell notification
	virtual void OnSetCell(int col,long row,CUGCell *cell);
	
	//data source notifications
	virtual void OnDataSourceNotify(int ID,long msg,long param);

	//cell type notifications
	virtual int OnCellTypeNotify(long ID,int col,long row,long msg,long param);

	//editing
	virtual int OnEditStart(int col, long row,CWnd **edit);
	virtual int OnEditVerify(int col,long row,CWnd *edit,UINT *vcKey);
	virtual int OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);
	virtual int OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow);

	//menu notifications
	virtual void OnMenuCommand(int col,long row,int section,int item);
	virtual int  OnMenuStart(int col,long row,int section);

	//hints
	virtual int OnHint(int col,long row,int section,CString *string);
	virtual int OnVScrollHint(long row,CString *string);
	virtual int OnHScrollHint(int col,CString *string);


	#ifdef __AFXOLE_H__  //OLE must be included

	//drag and drop
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject,int col,long row);
	virtual DROPEFFECT OnDragDrop(COleDataObject* pDataObject,int col,long row);

	#endif

	//sorting
	virtual int OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags);
	
	//DC setup
	virtual void OnScreenDCSetup(CDC *dc,int section);
	
	virtual void OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,RECT *cnrBtn,
		RECT *vScroll,RECT *hScroll,RECT *tabs);

	virtual void OnTabSelected(int ID);
	
	virtual COLORREF OnGetDefBackColor(int section);

	//focus rect setup
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);
	virtual void OnSetFocus(int section);
	virtual void OnKillFocus(int section);

	//column swapping
	virtual BOOL OnColSwapStart(int col);
	virtual BOOL OnCanColSwap(int fromCol,int toCol);

	//trackig window
	virtual void OnTrackingWindowMoved(RECT *origRect,RECT *newRect);
};
