// ValidationCug.h : interface of the ValidationCug class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include ".\ValidationCugButtons.h"
#include "..\..\ModelTypes\SimB.h"
#include "..\..\CoupModelDoc.h"
#include "..\..\ModelTypes\CoupTypes.h"
class CPG;
class MyCug;
class Doc;
struct MultiVAL;
class ChartUtil;
//#include "..\Chart\ZoomScrollDemoDlg.h"



class ValidationCug:public ValidationCugButtons
{
public:
	ValidationCug();
	~ValidationCug();
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

	void Init(CDialog*, CCoupModelDoc *pDoc=nullptr, BOOL picker=FALSE, ValidationCug *pCUG=nullptr, BOOL select=FALSE);
	void ResetSerie1();
	void ResetSerie2();
	BOOL AddLegend1(CString);
	BOOL AddLegend2(CString);
	void OnUpdate(int, long, BOOL);
	void SetChartNumIndex(int);
	BOOL SetColorBackground(int);
	int GetColorLine(int);
	BOOL SetTitle(int);
	void SetpCUG(void *);
	void SetpDlg(CDialog *);
	void SetpDoc(CCoupModelDoc *);
void SetpPG(CPG *);
void	Make_MainSheets(int);
void	Make_OptionSheet();
void	Make_CriteriaSheet();
void	Make_DistributionSheet(BOOL Residuals=FALSE);
void	Make_SplitPeriodSheet();
void	Make_SumVarCriteriaSheet();
void	FixZeroShiftOfPostMean(size_t ivar);	
void	ReducebyR2(size_t ivar, size_t n_left = 200, bool SV_Variable = false);
void	ReducebyRMSE(size_t ivar, size_t n_left=200, bool SV_Variable = false);
void	ReducebyNSE(size_t ivar, size_t n_left=200, bool SV_Variable = false);
void	ReducebyLogLi(size_t ivar, size_t n_left=200, bool SV_Variable = false);
void	ReducebyMeanInterval(size_t ivar, size_t n_Left=200, bool SV_Variable = false);
void	ReducebySlopeInterval(size_t ivar, size_t n_Left=200, bool SV_Variable = false);
void    ReduceUpdateStep(PERFORMANCE_VAR value);

BOOL	EstimateNewLogLi();
BOOL	MakeSortedResiduals();
BOOL	MultiPlotClick(int isheet, int col,  int row, bool Add=true, bool All=false);
bool	MakeMultiPlot(int sheet, int col, int row);
MultiVAL	GetMultiValStruct(int, int, int, bool *Add, int Indep);
BOOL	TimeSeriePlotClick(int isheet, int col,  int row, long mrunno, BOOL Add=TRUE);
BOOL	DefineNewSubPeriod();
void    NewPerformanceReady() { m_Changed = true; m_SavedCriteria2File = false; };
vector<int> m_FilterCase;

protected:
	ChartProp *m_pProp;
	
	BOOL m_Picker;
	BOOL m_Selected;
	CMainFrame *m_pMainframe;
	void DrawExcelFocusRect(CDC *dc,RECT *rect);
	ValidationCug *m_pRealTimeCug;
	F* m_pFile;
	int m_NumSelected;
	int m_PreviousClick;
	int m_ColSort;
	string Type, Group, Name;
	int m_NumLines;
	int m_NumValidVar;
	int m_TabSelected;
	float *m_pNewLogLi, *m_pErrorRel, *m_pErrorAbs, *m_pBoxCox1,*m_pBoxCox2;
	int m_LogFunc, m_BoxCoxTrans;
	BOOL m_NewEstimations;
	int m_newcolst;
	Sw *m_pSw1, *m_pSw2;
	P *m_pCPt1, *m_pCPt2, *m_pCPtError1,*m_pCPtError2;
	ChartUtil* m_pUtil;




private:
	int GetMultiValPerformIndex(int row, int col, int sheet);
	int GetMultiValPerformSplitIndex(int row, int col);
	int GetMultiValPerformSheet(int index);
	void MakeSelectionsVisible(int sheet);
	void EraseAllSelected();
	bool IsAcceptedEstimated();

	int m_TitleSizePoint,m_LabelSizePoint,m_LegendSizePoint,m_LineWidthPoint,m_SymbolSizePoint;
	int m_LineAxisWidthPoint;
	long m_col,m_row;

	Tab* m_pCTab;
	BOOL m_MultiTable;
	CFont m_font;
	Ps *m_pPar;
	
	int m_TabIndex;
	int m_ActNumValues;
	COLORREF m_ColorClick, m_ColorChanged, m_ColorHeading, m_ColorEdit, m_NoColor;

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

	
	//{{AFX_MSG(ValidationCug)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	PlotInfo *m_pPlotInfo;
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


	#ifdef __AFXOLE_H__  //OLE must be included
	virtual void OnMenuCommand(int col,long row,int section,int item);

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
