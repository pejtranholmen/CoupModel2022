// CValidTimeCug.h : interface of the CValidTimeCug class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "ugctrl.h"

#include "..\newBase\CommonModelInfo.h"

class CCoupModelDoc;
class Doc;
class Ps;
class Tab;
class F;

class CValidTimeCug:public CUGCtrl
{
public:
	CValidTimeCug();
	~CValidTimeCug();
	BOOL m_bDrawExcelFocus;
	CString GetTitle();
	void SetTitle(CString);
	void SetYLabel1(CString);
	void SetYLabel2(CString);
	CString GetYLabel1();
	CString GetYLabel2();
	CString GetLegend(int index);
	BOOL isYLabel1Changed();
	BOOL isYLabel2Changed();
	BOOL isYLegendChanged(int index);
	BOOL isYLegend2Changed(int index);
	BOOL isLeftYScaleChanged(){return m_YScaleLeftChanged;};
	BOOL isLeftYScaleSet();
	BOOL isRightYScaleChanged(){return m_YScaleRightChanged;};
	BOOL isRightYScaleSet();
	double GetYLeftmin() {return m_YScaleLeftMin;};
	double GetYRightmin(){return m_YScaleRightMin;};
	double GetYLeftmax(){return m_YScaleLeftMax;};
	double GetYRightmax(){return m_YScaleRightMax;};
	double GetYLeftInterv(){return m_YScaleLeftInterv;};
	double GetYRightInterv(){return m_YScaleRightInterv;};
	void SetYLeftmin(float v){m_YScaleLeftMin=v;m_YScaleLeftSet=TRUE;};
	void SetYRightmin(float v){m_YScaleRightMin=v;m_YScaleRightSet=TRUE;};
	void SetYLeftmax(float v){m_YScaleLeftMax=v;m_YScaleLeftSet=TRUE;};
	void SetYRightmax(float v){m_YScaleRightMax=v;m_YScaleRightSet=TRUE;};
	void SetYLeftInterv(float v){m_YScaleLeftInterv=v;};
	void SetYRightInterv(float v){m_YScaleRightInterv=v;};
	BOOL isYLeftScaleSet() {return m_YScaleLeftSet;};
	BOOL isYRightScaleSet() {return m_YScaleRightSet;};
	int GetNum();

	void Init(CDialog*, CCoupModelDoc *pDoc=nullptr, BOOL picker=FALSE, CValidTimeCug *pCUG=nullptr, BOOL select=FALSE);
	void ResetSerie1();
	void ResetSerie2();
	BOOL AddLegend1(CString);
	BOOL AddLegend2(CString);
	void OnUpdate();
	void SetChartNumIndex(int);
	BOOL SetColorBackground(int);
	int GetColorLine(int);
	BOOL SetTitle(int);


protected:

	BOOL m_Picker;
	BOOL m_Selected;
	void DrawExcelFocusRect(CDC *dc,RECT *rect);
	CValidTimeCug *m_pRealTimeCug;
	F* m_pFile;
	int m_NumSelected;
	int m_PreviousClick;
	int m_ColSort;

private:

	int m_TitleSizePoint,m_LabelSizePoint,m_LegendSizePoint,m_LineWidthPoint,m_SymbolSizePoint;
	int m_LineAxisWidthPoint;
	long m_col,m_row;
	BOOL m_Data;
	Tab* m_pCTab;
	BOOL m_MultiTable;
	Ps *m_pPar;
	CCoupModelDoc *m_pDoc;
	Doc *m_pSimDoc;
	
	int m_TabIndex;
	int m_ActNumValues;
	COLORREF m_ColorClick, m_ColorChanged, m_ColorHeading;
	CDialog *m_pDlg;

	int *m_NumValid;
	CString m_Title;
	CString m_YLabel1, m_YLabel2;
	CStringArray m_Legend1, m_Legend2;
	CUIntArray m_Legend1Change, m_Legend2Change;
	BOOL m_YLabel1Change,m_YLabel2Change;
	int m_ChartNumIndex;
	int m_colpos;
	
	BOOL m_YScaleLeftChanged,m_YScaleLeftSet;
	BOOL m_YScaleRightChanged,m_YScaleRightSet;
	double m_YScaleLeftMin,m_YScaleLeftMax,m_YScaleLeftInterv;
	double m_YScaleRightMin,m_YScaleRightMax,m_YScaleRightInterv;
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

	
	//{{AFX_MSG(CValidTimeCug)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	//***** Over-ridable Notify Functions *****

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
	virtual int OnEditFinish(int col, long row,CWnd *edit,LPCTSTR std::string,BOOL cancelFlag);
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
