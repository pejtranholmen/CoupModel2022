#pragma once
#include ".\PGClick.h"

class PGCommands :
	public PGClick
{
public:
	PGCommands(void);
	~PGCommands(void);
		
	void FileCreate();
	void FileOpen();
		//menu notifications
virtual void OnMenuCommand(int col,long row,int section,int item);


protected:
	bool ExportVariables(CPG *pCPG);


	void AddPGFile(string FileName);
	void OpenEditPGFile(int col, int row, int item);
	void AddPGFile();
	void RemovePGFile(int col, int row);
	void MixPGFilesWithoutInterpolations();
	void MixPGFilesWithInterpolations();
	void ExportfromPGFileText(int col, int row);
	void ExportfromPGFileExcel(int col, int row);
	bool CreateNewPGFileWithNewVar(int col, int row);
	bool ImportFromTextAndMixToPGFile();
	bool IncludeCoordinatesFromNOAA(int col, int row);
	bool MergeSimilarStations(int col, int row);
	bool InterpolateBasedOnRegressions(int col, int row);
	bool CreateNewFileForNewPosition();
	bool CreateNewPGFileWithRepetition();
	bool CreateNewFileForNewGridPosition();
	bool ReplaceMissingDataByLinearInterpolation();
	bool ReplaceMissingDataByRandom();
	bool ImportFromNetCDF_File();
	bool SplitIntoNoRepetionsFiles(int col, int row);
	bool SetNewIndependenceVar(int item, int col, int row);
};

