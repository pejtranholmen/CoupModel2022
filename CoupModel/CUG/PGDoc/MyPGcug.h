

#pragma once
#include ".\PGEditCells.h"
#include "..\..\CoupModelDoc.h"

class CCoupModelView;


class MyPGCug:	public PGEditCells
{

public:


	MyPGCug();
	~MyPGCug();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

	
	//{{AFX_MSG(MyPGCug)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CUGMaskedEdit m_maskedEdit;

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
public:
	void UpdateCurrentChart(bool multi=FALSE);
	void NeedParUpdate();

	int m_NumOpenDoc;
/*	CPtrArray m_SelectedPtr, m_SelectedPGPtr, m_SelectedFPtr;
	CUIntArray m_SelectedIndex, m_SelectedIType, m_SelectedPGIndex, m_SelectedFIndex;
	CUIntArray m_SelectedRow, m_SelectedPGRow, m_SelectedValRow,m_SelectedValRunNo;
	CUIntArray m_SelectedValRunNoRow, m_SelectedChartIndex;
	CUIntArray m_SelMultiValIndex,m_SelMultiSumIndex, m_SelMultiParameterIndex;
	CPtrArray m_SelectedDoc;
	*/
	void UpdateStatusBar(SimB *pBase);
	void UpdateStatusBar(CString txt);
	void UpdatePlotStatusBar(int, BOOL clean=FALSE);
	void UpdateStatusBar(int);





protected :



	BOOL m_bDrawExcelFocus;
	BOOL m_ShowMultiTimeSerieTable;
	BOOL m_ShowValidationTimeSerieTable;
	BOOL m_LinkedChartDocument;



	BOOL m_storage_exist;
	BOOL m_OutPutFile_Exist;
	BOOL m_TobePlotted;
	BOOL m_ShowValidCoVarRes;



	CPtrArray m_Charts_Array;

	BOOL m_created;
	BOOL m_TimeSerie;
	BOOL m_newChart;



	CString m_PGFileNameAll,m_PGFileNameSelected;
	CString m_ClickedTable;
	int m_ClickedRow;
	long m_CurrentTopRow;



	BOOL ShowFontDlg( CUGCell &cell, LOGFONT &selectedLogFont, COLORREF &textColor );
	CFont* CreateFont( int nHeight, int nWidth, int nEscapement, int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline, BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision, BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily, LPCTSTR lpszFacename );
	CFont* CreateFontIndirect( LOGFONT newLogFont );

	// additional edit controls
	CUGEdit m_myCUGEdit;
	int m_BMP1;

	BOOL m_DocumentDefined;
	BOOL m_Display;
	BOOL m_OutAllMulti_Defined[18];
	BOOL m_OutSelectedMulti_Defined[18];
	BOOL m_OutMAPMulti_Defined;
	BOOL CalculateMeanOffTimeSeries(long item, int ival);
	BOOL m_NeedParUpdate;
	BOOL m_ShowPGTable;
	BOOL m_ShowHistoryList;
	CCoupModelView *m_pView;
	void TimeSerieChartClick(long col, long row, long isheet, void *Sw, int Index, int VariableType, CString Type, int ValFileNo=1);

	public:

	void SetDocument(CCoupModelDoc *pDoc);
	void SetViewPointer(CCoupModelView *pView);
	void UpdateRealTimeChart();
	
	void DrawExcelFocusRect(CDC *dc,RECT *rect);
	BOOL isCreated();
	BOOL UpdateDocumentLink();
	//	New functions
	CString GetMultiOutId(int sheet);
	void SortAndJoin(int col, int isheet);
	void ResultUpdate();
	void ReportUpdate();
	void CloseReportMode();


	void DeleteChart(int);
	void UpdateChart(int);
	BOOL UpdateChart(int, int);
	void UpdateSheet(int);
	int GetColorLine(int);
	int GetMultiMethod(CString);
	int GetMultiDimensionIndex(CString);
	int GetMultiDependenceIndex(CString);
	int GetSV_Valoption(CString);
	int m_ShowChartTableNo;
	bool m_ShowChartTable;
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
	virtual void OnViewMoved( int nScrolDir, long oldPos, long newPos );
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
	virtual void OnKeyUp(UINT *vcKey,BOOL processed);
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


	// Context help 
	virtual DWORD OnGetContextHelpID( int col, long row, int section );

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
	virtual void OnKillFocus(int section, CWnd *pNewWnd);

	//column swapping
	virtual BOOL OnColSwapStart(int col);
	virtual BOOL OnCanColSwap(int fromCol,int toCol);
	virtual void OnColSwapped(int fromCol,int toCol);

	//trackig window
	virtual void OnTrackingWindowMoved(RECT *origRect,RECT *newRect);




};
