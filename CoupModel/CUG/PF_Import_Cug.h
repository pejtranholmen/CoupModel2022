
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// PF_Import_Cug.h : interface of the PF_Import_Cug class
//
/////////////////////////////////////////////////////////////////////////////
#include "ugctrl.h"
#include "..\ModelTypes\SimB.h"

class PFCurve;
class CDB;
class Tab;
struct NEWHEADER;

class PF_Import_Cug:public CUGCtrl
{
public:
	PF_Import_Cug();
	~PF_Import_Cug();
	CDB *m_pDB2;
	BOOL m_bDrawExcelFocus;
	UINT	m_ProfMin;
	UINT	m_TopSandMin;
	UINT	m_SubSandMin;
	UINT	m_SubSandMax;
	UINT	m_SubClayMin;
	UINT	m_SubClayMax;
	UINT	m_ProfMax;
	string	m_CountyMin;
	string	m_CountyMax;
	UINT	m_TopSandMax;
	UINT	m_TopClayMin;
	UINT	m_TopClayMax;
	UINT	m_TopOrgMin;
	UINT	m_TopOrgMax;
	UINT	m_SubOrgMin;
	UINT	m_SubOrgMax;
	string	m_PFpoints;
	UINT	m_YearMin;
	UINT	m_YearMax;
	unsigned int m_Min_NumHeads;

protected:
	void DrawExcelFocusRect(CDC *dc,RECT *rect);
	bool KeyTest(NEWHEADER header);
	int m_IniProf, m_IniRepProf;

private:
	
	BOOL m_DataBase,m_CreateNewNumbers;
	Tab* m_pCTab;
	PF_Import_Cug *m_pCUG, *m_pCUG_Data;
	string m_DataBaseString;
	CDialog *m_pPP_EDIT_Dlg;
	CDialog *m_pSP_Retrieve_Dlg;
	CCoupModelDoc *m_pDoc;
	PFCurve* p_PFCurve;

	int m_LatestSheet, m_PrevClickRow;




	COLORREF m_ColorClick, m_ColorChanged, m_ColorHeading;
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

	
	//{{AFX_MSG(PF_Import_Cug)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	//***** Over-ridable Notify Functions *****
	void OnUpdateDataBase(BOOL, CDialog *pDialog=nullptr, CDB *pDB=nullptr, PF_Import_Cug *pCug=nullptr);

//
	void MakeBaseSheet();
	void MakeTextureSheet();
	void MakePFCurveSheet();
	void MakeBoCSheet();
	void MakeGenuchtenSheet();
	void MakeHydraulicCondSheet();
	void MakeCommentsSheet();
	int GetMultiTypeIndex(string);
	void SaveTopRow();
	void SetTopRowExternt();
	void NewRowNumber(int);
	int GetNumberOfRows();
	int Import();
	BOOL SaveProfile();

	BOOL SaveToSim();
	long m_col,m_row, m_toprow;
	BOOL m_CompleteList;
	BOOL m_SelOne;
	UINT *m_pProf,*m_pRepProf;


	virtual void OnSetup();

	virtual void OnSheetSetup(int sheetNumber);

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
