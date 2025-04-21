#pragma once
#include <WinSock2.h>
#include <windows.h>
#include "../std.h"
#include "../Util/ReadWriteTypes.h"

// ResidualsBase command target
class NewMap;

struct RESIDUAL_MEAN{
	vector<vector <float>> mean;
	vector<vector <float>> min;
	vector<vector <float>> max;
	vector<vector <float>> std;
	vector<vector <float>> p10;
	vector<vector <float>> p50;
	vector<vector <float>> p90;
};
struct POS_ENSEMBLE {
	bool valid;
	size_t pos;
	string name;
	bool EnsembleFileRead;
};

class ResidualsBase 
{
public:
	ResidualsBase();
	virtual ~ResidualsBase();
	bool EnsembleReadWrite(bool reading, string EnsembleName, string NewEnsembleName="", size_t ensembleno=string::npos);
	bool EnsembleReadWrite(bool reading, size_t ensembleno);

	bool ReadWrite_MeanEnsembleFile(bool reading, size_t ensembleno=string::npos);
	bool ReadWrite_PriorStat();
	bool ReadWrite_Ensemble(string EnsembleName);
	bool OpenResidualFile(bool reading=false);
	bool CloseResidualFile(bool reading);
	bool Residuals_ReadWrite_Header(bool reading);
	bool SetNewEnsembleByNumber(size_t ensembleno);
	bool AlloResVector(size_t ActRecl=string::npos);
	string GetXStatFileName(size_t ensembleno=string::npos);
	bool IsOpen(bool reading=true);
	bool SetOneValidationVariableIndex(size_t index) { m_ValidationFileIndex = index; if (m_ValidationFileIndex < 1000) return true; else return false; };
	bool EnableEstimatesMade(size_t enableno=0);
	bool m_IsUpdated, m_IsUpdated_Extra;
	bool ShiftEnsembleName(string oldname, string newname);
	void SetResidualBaseFileNumVarScaling(size_t NumVarScaling) { m_ScalingNumVarWithDim1 = NumVarScaling; };
	bool m_MeanDefined;
	size_t m_ValidationFileIndex{ string::npos };
	std::streamsize ResidualsBase::GetFileSizeFromHandle();


protected:
	NewMap *m_pNewMap;
	vector<float> m_Mean;
	RESIDUAL_MEAN m_MeanPrior;
	RESIDUAL_MEAN m_MeanPost;
	RESIDUAL_MEAN m_MeanTest;
	vector<POS_ENSEMBLE> m_Ensembles;
	vector<float> m_Residuals;
	size_t m_RecL, m_NumRec, m_NumVar, m_ScalingNumVarWithDim1;
	string m_FileName, m_ExtraFileName, m_SortedFileName;
	bool m_IsOpen, m_NewPrior;
	bool m_EnsembleFileRead;
	int m_ResidualFileNo;
	string m_NewEnsembleName;
	

private :
	ReadWriteTypes m_EnsembleResFile,  m_ExtraFile, m_SortedFile;

	class WinAPIFileHandler {
	public:
		bool OpenWrite(const std::wstring& filename) {
			hFile = CreateFileW(filename.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
			return hFile != INVALID_HANDLE_VALUE;
		}

		bool OpenRead(const std::wstring& filename) {
			hFile = CreateFileW(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
			return hFile != INVALID_HANDLE_VALUE;
		}

		bool WriteBinary(const void* data, size_t size) {
			DWORD written;
			return WriteFile(hFile, data, static_cast<DWORD>(size), &written, nullptr);
		}

		bool ReadBinary(void* data, size_t size) {
			DWORD read;
			return ReadFile(hFile, data, static_cast<DWORD>(size), &read, nullptr);
		}

		void Close() {
			if (hFile != INVALID_HANDLE_VALUE) {
				CloseHandle(hFile);
				hFile = INVALID_HANDLE_VALUE;
			}
		}

		bool IsOpen() const {
			return hFile != INVALID_HANDLE_VALUE;
		}
		HANDLE GetHandle() const {
			return hFile;
		}



	private:
		HANDLE hFile = INVALID_HANDLE_VALUE;
	};

	WinAPIFileHandler m_ResidualFile;

	bool meanValid;
	size_t m_MaxNumRec;
	std::streamsize m_FileSize;
	map<string, size_t> m_MapofEnsemblePositions;
	std::streampos m_PosToRead, m_PosToWrite;
	string m_EnsembleName;
	string m_TempOutFile;
	bool m_TwoFilesOpen;
	size_t m_SizeOfMeanVector;



};


