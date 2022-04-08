// ChartCug.h : interface of the ChartCug class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once





#include ".\ChartCugClick.h"
#include "..\..\ModelTypes\SimB.h"
#include "..\..\CoupModelDoc.h"
#include "..\CugChartData.h"
#include "..\..\Chart\ChartViewer.h"
#include "chartdir.h"

#include "..\..\Chart\RealTime.h"
#include "..\..\Chart\ParameterPlot.h"
#include "..\..\Chart\ModelView.h"
#include "..\..\Chart\TimeSeriePlot.h"
#include "..\..\Chart\TableView.h"
class ChartProp;
class ChartUtil;
class ZoomTimeSerie;
class PGCurve;





class ChartCug:public ChartCugClick
{
public:
	
	

	ChartCug();
	~ChartCug();



	BOOL InitData(Doc *pDoc, int, CHART_TYPES, string filename="", CView *view=nullptr);
	void SaveToExtraChartWindow();
	void ShiftExtraChartWindow();
	void SaveToFileExtraChart();
	void RealTimeMode(bool value) {MySingle::instance().GetChartUtilPointer()->m_RealTimeMode=value;};
	void ParameterPlotMode(bool value) {MySingle::instance().GetChartUtilPointer()->m_ParameterPlotMode=value;};
	void TablePlotMode(bool value) {MySingle::instance().GetChartUtilPointer()->m_TablePlotMode=value;};
	BOOL IsMultiRunPlot() {return MySingle::instance().GetChartUtilPointer()->m_MultiRunPlot;};
	BOOL InitMultiRunPlot(Doc *pDoc, int, CHART_TYPES, CCoupModelView *view=nullptr);
	BOOL InitData(Doc *pDoc, Ps* pCP, P* pCPt=nullptr, int index=-1);
	BOOL AddData(Ps*, BOOL, P* pCPt=nullptr, int index=-1);
	BOOL IsNewViewFunc(Ps* );
	int	 GetChartNum();
	void DrawTextPlot(CChartViewer *viewer);
	void MakeTextPlot(CString text=_T(""));
	void MakeTextPlot(string txt = "") { MakeTextPlot(CString(txt.c_str())); };

	BOOL SetDistScale(float min, float max, BOOL log);
	BOOL OnUpdateSequence(int ArrayIndex=-1);


	void Init(Doc *pDoc=nullptr);

	void SetMultiRunPlot() {MySingle::instance().GetChartUtilPointer()->m_MultiRunPlot=TRUE;};
	BOOL GetMultiRunPlot() {return MySingle::instance().GetChartUtilPointer()->m_MultiRunPlot;};
	int GetLegendSize() {return MySingle::instance().GetChartUtilPointer()->m_LegendSizePoint;};
	

	void SetNumberOfPoints(int value) {m_NumberOfPoints=value;};
	void SetNumberOfBands(int value) {m_NumberOfBands=value;};

protected:


	void DrawExcelFocusRect(CDC *dc,RECT *rect);



	
	//{{AFX_MSG(ChartCug)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code!

	
	afx_msg void OnPaint();
	afx_msg void OnActivate(UINT nstate, CWnd *pWndOther, BOOL bMinimized);

	afx_msg void OnChartViewer();
	afx_msg void OnSelchangeDuration();
	afx_msg void OnKillfocusDuration();



	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()






	
public:
	
	afx_msg void OnDatetimechangeStartDate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPointerPB();

	afx_msg void OnXZoomPB();
	afx_msg void OnXYZoomPB();



	//***** Over-ridable Notify Functions *****

	virtual void OnSetup();
	bool InitMultiPGFilePlot(Doc *pSimDoc);

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
	PFCurve* p_PFCurve;


			
};
