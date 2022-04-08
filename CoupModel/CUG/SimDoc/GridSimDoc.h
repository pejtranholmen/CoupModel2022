#pragma once
#include "..\PEJugctrl.h"
#include "..\..\ModelTypes\SimB.h"
#include "..\..\NewBase\Doc.h"

class Doc;
class CommonModelInfo;
class SimArchive;
class ChartPro;
class GridSimDoc :
	public PEJUGCtrl
{
public:
	GridSimDoc(void);
	~GridSimDoc(void);

	bool m_newChart;
	int m_NumOpenDoc;

	bool UpdateDocumentLink();

	//int m_NumChartX,m_NumChartY, m_NumSubCharts;

	CPtrArray m_SelectedPGPtr, m_SelectedFPtr;
	CUIntArray  m_SelectedPGIndex, m_SelectedFIndex, m_SelectedPG_Repit;
	CUIntArray m_SelectedRow, m_SelectedPGRow;
	CUIntArray  m_SelectedChartIndex, m_SelectedChartValIndex,m_SelectedChartPGIndex;
	CUIntArray m_SelMultiValIndex,m_SelMultiSumIndex, m_SelMultiParameterIndex;
	bool m_OutAllMulti_Defined[MAXSIMVAL+1];
	bool m_OutSelectedMulti_Defined[MAXSIMVAL+1];
	void MakeChartMenu(int col, int row, int sheet=11);

protected:

	virtual int  OnMenuStart(int col,long row,int section);
	void MakeRunInfo_PriorMenu(int col, int row);
	void MakeRunInfo_PostMenu(int col, int row);
	void MakeRunInfo_DataBaseMenu(int col, int row);
	void MakeSwitchPriorMenu(int col, int row);
	void MakeSwitchPostMenu(int col, int row);
	void MakeParameterPriorMenu(int col, int row);
	void MakeParameterTablePriorMenu(int col, int row);
	void MakeParameterPostMenu(int col, int row);

	void MakeValidationPriorMenu(int col, int row);
	void MakeValidationPostMenu(int col, int row);
	void MakeHistoryMenu(int col=-1, int row=-1);
	void MakeMultiRunMenu(int col=-1, int row=-1);
	void MakeValidationMenu(int col=-1, int row=-1);
	void MakeSelectOutputMenu(int col, int row);
	void MakeMultiRunPriorMenu(int col, int row);
	PLOTv MakeOutPutChartInfo(size_t index_select, size_t index_doc);
	SIMB GetMultiSumPointer(int row);
	SIMB GetMultiRunParPointer(int row);
	int GetMultiRunParIndex(int row);
	int GetMultiSumIndex(SIMB simb, int col);
	int GetMultiValIndex(int row, int col);
	int GetMultiValSumIndex(int row, int col);
	SUM_PERFORMANCE GetMultiValSumType(int row, int col);
	Doc *m_pSimDoc;

	SimArchive *m_pSimArchive;	
	CWinApp* m_pApp;
	ChartProp *m_pProp;
	ChartUtil *m_pUtil;
	CCoupModelDoc *m_pDoc;


	int m_ViewCategory;
	int m_ParameterTableView;


	int m_ShowChartTableNo;
	bool m_ShowChartTable;
	BOOL m_ShowAllOutputs;
	BOOL m_ShowAllSwitches;
	BOOL m_ShowProcesses;
	BOOL m_ShowAllSoilLayers;
	BOOL m_ShowAllPlants;
	BOOL m_ShowSoilProfile;
	BOOL m_ShowPlantVectors;
	BOOL m_ShowScalar;
	BOOL m_ShowOnlyNonVectorOutputs;
	BOOL m_Show_C, m_Show_N, m_Show_P, m_Show_Water, m_Show_Heat;
	BOOL m_Show_SoilGas, m_Show_Climate;
	BOOL m_Show_All_Elements;
	BOOL m_Show_OnlySelected_Elements;
	vector<bool> m_SelectedSoilLayers;
	vector<bool> m_SelectedPlants;
	BOOL m_ShowAllParameters;
	BOOL m_ShowAllTables;
	BOOL m_ShowAllFiles;
	BOOL m_ShowMultiRes;
	BOOL m_ShowValidRes;
	BOOL m_ShowValidRes_Changed;
	BOOL m_ShowSumRes;
	BOOL m_ShowCoVarRes;
	BOOL m_ShowAllEnsemblePostParList;
	BOOL m_ShowTimeSerieTable;
	BOOL m_ShowMultiSerieTable;

	BOOL m_ShowMultiTimeSerieTable;
	BOOL m_ShowValidationTimeSerieTable;
	BOOL m_LinkedChartDocument;
	bool m_UpdateOutputList;

	bool m_ValidTimeSerieType;

	BOOL m_storage_exist;
	BOOL m_OutPutFile_Exist;
	BOOL m_TobePlotted;
	BOOL m_ShowValidCoVarRes;

	COLORREF m_ColorEdit;
	COLORREF m_ColorHistory;
	COLORREF m_ColorChanged;
	COLORREF m_ColorHeading;
	COLORREF m_ColorClick;
	COLORREF m_NoColor;
	COLORREF m_ColorRClick;
	COLORREF m_ColorCompleted;
	COLORREF m_ColorFirstIndep;
	COLORREF m_ColorSecondIndep;
	CFont  m_defFont;
	CFont  m_smallFont;
	CFont  m_largeFont;
	CFont m_extralargeFont;
	CFont m_font;



	BOOL m_DocumentDefined;
	BOOL m_Display;

	BOOL m_OutMAPMulti_Defined;
	
	BOOL m_NeedParUpdate;
	BOOL m_ShowPGTable;
	BOOL m_ShowHistoryList;
	BOOL m_bDrawExcelFocus;
	private:
	PGFileArchive *m_pPGArchive;
	
};

