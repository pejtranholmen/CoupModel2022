#pragma once
#include "..\PEJUGCtrl.h"
#include ".\PGMaps.h"
#include "..\..\ModelTypes\SimB.h"
#include "..\..\NewBase\Doc.h"
#include "..\..\PG\FileImport\PGFileImport.h"

class Doc;
class CommonModelInfo;
class SimArchive;
class ChartProp;
class PGFileArchive;

#include "..\..\PG\NoaaCoord.h"
#include "..\..\PG\TimeAggregate.h"
#include "..\..\PG\ChangePeriod.h"
#include "..\..\PG\ChangeToNewResolution.h"
#include "..\..\PG\NewAddMixPGFiles.h"
#include "..\..\PG\NewReShaped.h"

class GridPGDoc :
	public PEJUGCtrl, public PGMaps
{
public:
	GridPGDoc(void);
	~GridPGDoc(void);
	void OnPGHeaderUpdate(int, long, BOOL, BOOL wide = FALSE, long repit = 1);
	BOOL ReDisplay();
	PGFileImport m_PGFileImport;
	bool Make_ImportFromCSV_File_Prior(bool ScanningtoMake = false, bool CreatePGFile = false);

protected:
	CString m_Prior, m_After;
	BOOL m_sort[5];

	COLORREF m_ColorEdit;
	COLORREF m_ColorChanged;
	COLORREF m_ColorHeading;
	COLORREF m_ColorClick;
	COLORREF m_NoColor;
	COLORREF m_ColorRClick;
	COLORREF m_ColorCompleted;

	CFont  m_defFont;
	CFont  m_smallFont;
	CFont  m_largeFont;
	CFont m_extralargeFont;
	CFont m_font;


	virtual int  OnMenuStart(int col,long row,int section);

	bool Make_PGFileSheet();
	bool Make_Description(int ifile=0);
	bool Make_NewSpatialCoord(int ifile=0);
	bool Make_NewSpatialGrid(int ifile=0);
	bool Make_DataSubstitution();
	bool Make_ChangePeriodSheet();
	bool Make_DataAggregation();
	bool Make_NewDataResolution();

	bool Make_New_PGFiles();
	bool Make_Merge_Prior();
	bool Make_ReShape_Prior(bool Update = false);
	bool Make_Add_Prior();
	bool Make_Add_Post();
	bool Make_Mix_Prior();
	bool Make_Mix_Post();
	bool Make_NewGrid_Prior();
	bool Make_NewGrid_Post();

	bool Make_ImportFromCSV_File_Post();


	void Make_OutputFileInformationLines(int startline, int linetomake);

	void MakePGChartMenu(int col, int row, int index=0);
	void MakePGModifyMenu(int col, int row, int index=0);
	bool DefineScalingFile();

	bool IsMultiPGRepPlot();
	bool IsMultiPGRepPlotReady();
	void Make_DataHeader();
	void Make_DescriptionHeader();

	CPG* GetCurrentPG_Pointer();
	bool OnWriteGridDataToFile(CPG *pCPG);
	bool OnWriteGridDescriptionToFile(CPG *pCPG);
	bool OnGetFileDataFillGrid(CPG *pCPG);
	bool OnGetFileDescriptionFillGrid(CPG *pCPG);


	bool CheckForChanges();
	void CheckStatusOfPG_Outputs(int col, long row);


	BOOL m_NewVariable;
	bool m_CalcAllRepetitions;
	void DefineButtonInfos();
	void DefineListButtonInfos();
	void DefineClickButtonInfos();
	void DefineHintList();
	void DefineComboList();
	void DefineCheckBoxButtons();

	PGFileArchive *m_pPGArchive;
	Doc* m_pSimDoc;
	CCoupModelDoc *m_pDoc;
	ChartProp *m_pProp;
	ChartUtil *m_pUtil;
	void *m_pCUG;
	int m_ShowMean;

	int m_CurrentSubChart;


	int m_IndexSource;
	int m_ViewCategory;
	bool m_RepFirst, m_ShowDetails;
	double m_Latitude, m_Longitude, m_Lat_Max, m_Lat_Min, m_Long_Max, m_Long_Min;
	size_t m_GridLat, m_GridLong;
	CPG *m_pGSource, *m_pPG_Results, *m_pPG_ScalingSource;

	NoaaCoord m_spatialPG;
	TimeAggregate m_TimeAggregate;
	ChangePeriod m_ChangePeriod;
	ChangeToNewResolution m_ChangeResolution;

	NewAddMixPGFiles m_AddMix_Actions;
	NewReShaped m_ReShaped_Actions;

	bool	m_DestFileSet[7];
	bool	m_SpatialSourceFileReady;
	int		m_SubstituteOption;
	int		m_AggregateOption;
	int		m_AggregationStepSize;
	int		m_NewResolutionOption, m_NewResolutionOption_Extra;
	int		m_NewResolutionSteps;
	int		m_ChangePeriodOption;
	float	m_MaxSubstituteDays;
	bool	m_ShowSubResults;
	bool	m_ShowCalculating;
	int		m_col;
	long	m_row;
	bool    m_Wide, m_Data;
	size_t	m_Repit;
	int m_NumVar;
	size_t m_ShowMaxRep;
	bool m_Changed;
	bool m_Changed_Data;
	bool m_Changed_Description;
	CPG *m_pPG;

	CPG *m_pPGNewGrid;
	CLICK_BUTTON m_RepSortButton;
	CLICK_BUTTON m_SelectTimeButton;
	CLICK_BUTTON m_ScanFileButton;
	CLICK_BUTTON m_OpenFileButton;
	CLICK_BUTTON m_SelectVariableButton;
	CLICK_BUTTON m_SiteInfoFileButton;
	CLICK_BUTTON m_ImportedFiles;
	CLICK_BUTTON m_NewFileOptions;
	CLICK_BUTTON m_NewOutputNewGridPGFileButton;

	CLICK_BUTTON m_ChartTypeOptions;



//   Import File Buttons
	CLICK_BUTTON m_SiteInfoButton;
	CLICK_BUTTON m_QuitImportButton;
	CLICK_BUTTON m_SelectedOutputVarButton;
	CLICK_BUTTON m_IdentifiedVarSiteInfoButton;
	CLICK_BUTTON m_ComboIdButton, m_ComboCountryButton, m_ComboLatButton;
	CLICK_BUTTON m_ComboLongButton, m_ComboAltButton;
	CLICK_BUTTON m_VarSeparationButton, m_LineSeparationButton;
	CLICK_BUTTON m_IdentifiedFormatButton, m_ClickBoxFormatButton;
	CLICK_BUTTON m_FilterVariableButton, m_FilterUseButton;


// OutputFileInformationButtons
	CLICK_BUTTON m_ApplyNameFromHeadingButton;
	CLICK_BUTTON m_ApplyNameFromElementTypeButton;
	CLICK_BUTTON m_ApplyUnitFromHeadingButton;
	CLICK_BUTTON m_ApplyUnitFromUnitTypeButton;
	CLICK_BUTTON m_NameOfHeadingLine;
	CLICK_BUTTON m_ElementTypeNewVariables;
	CLICK_BUTTON m_UnitTypeNewVariables;

	// PG DescriptionButtons

	CLICK_BUTTON m_ElementTypeVariables;
	CLICK_BUTTON m_UnitTypeVariables;
	CLICK_BUTTON m_ApplyElementTypeButton;
	CLICK_BUTTON m_ApplyUnitTypeButton;

	CLICK_BUTTON m_SaveCurrentFileButton;



	// Add Buttons
	CLICK_BUTTON m_AddMorePGFilesButton;
	CLICK_BUTTON m_CreateNewAddButton;
	CLICK_BUTTON m_ResetListAddButton;
	CLICK_BUTTON m_NewOutputPGFileButton;
	CLICK_BUTTON m_QuitButton;
	CLICK_BUTTON m_AddFileList;
	

	// Reshape Buttons
	CLICK_BUTTON m_TranslationListButton;
	CLICK_BUTTON m_CreateNewReshapeButton;
	CLICK_BUTTON m_UpdateReshapeButton;
	CLICK_BUTTON m_ShiftOutputFormat;

	//Mix
	CLICK_BUTTON m_MixMorePGFilesButton;
	CLICK_BUTTON m_CreateNewMixButton;
	CLICK_BUTTON m_NewOutputMIXPGFileButton;
	CLICK_BUTTON m_MixInterpolateButton;

	// Merge
	CLICK_BUTTON m_MergeMorePGFilesButton;
	CLICK_BUTTON m_CreateNewMergeButton;
	CLICK_BUTTON m_NewOutputMergePGFileButton;

	// New Grid
	NEWPGFILE_INFO m_NewGridInfo;
	CLICK_BUTTON m_CreateNewEmptyPGFile;
	CLICK_BUTTON m_ChangeNewGridFileName;

	//
	HINTLIST m_VarSelectBox, m_VarSelectOptions, m_FileSelectOptions;
	HINTLIST m_ChartNumberSelect;
	HINTLIST m_ScanTimeId_Date, m_ScanTimeId_Year, m_ScanTimeId_Month, m_ScanTimeId_DayNo, m_ScanTimeId_Day, m_ScanSiteId;
	HINTLIST m_ScanTimeId_Time, m_ScanTimeId_Hour, m_ScanTimeId_Minute, m_AddScalingCoefHint;

	HINTLIST m_ScanStartDate, m_ScanMinDiff;
	HINTLIST m_DescriptionImportedFile;

	HINTLIST m_ScanUniqueStep, m_ScanRecordStep, m_ScanCommonStep;

	HINTLIST m_ViewStartHint, m_ViewPageHint, m_PageRangeHint;
	HINTLIST m_SelectStartHint, m_SelectEndHint;
	HINTLIST m_SelectWithinDay;
	// DataSheetCommands




	// DROPLIST
	CLICK_BUTTON m_RepOptionsCombo;
	CLICK_BUTTON m_XAxisCombo;
	CLICK_BUTTON m_YAxisCombo;
	CLICK_BUTTON m_VarTransformationCombo;
	CLICK_BUTTON m_VarDim1Combo, m_VarDim2Combo, m_VarDim3Combo;

	
};

