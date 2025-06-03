#pragma once
#include "../stdafx.h"
#include "../std.h"
#include "FUtil.h"
struct FILEDLG { std::string filename; bool opengood; };




class MFC_Util
{
public:
	MFC_Util(void);
	~MFC_Util(void);
	static std::string GetNewSimFileName(int SimulationRunNo, std::string PrevFileName);
	static CString AddIndexString(int value) { CString str = _T("("); str += ItoAscii(value); str += _T(")"); return str; };
	static std::string AddIndexStdString(int value) { std::string str = "("; str += FUtil::STD_ItoAscii(value); str += ")"; return str; };
	static FILEDLG GetFileDialog(string filename, std::string type, std::string filtertype, BOOL OpenOnly=TRUE);
	static void WriteProfileString(string item, std::string value);
	static void WriteProfileString(string item, CString value) {WriteProfileString(item, std::string(CT2A(value).m_psz)); };
	static std::string GetProfileStringStd(string item, std::string value);
	static CString GetProfileString(string item, CString value) { return CString(GetProfileStringStd(item, CT2A(value).m_psz).c_str()); };
	static int  GetProfileIntNo(string item, int value);
	static void  WriteProfileInt(string item, int value);
	static int GetOptionIndex(CString optionlist, CString str);
	static CString	FtoAscii(float)  ;
	static CString FtoAsciiPane(float);
	static std::string StdFtoAsciiPane(float value) { return CT2A(FtoAsciiPane(value)).m_psz; };
	static CString	DtoAscii(double);
	static CString	ItoAscii(int);
	static CString	ItoAsciiHex(int);
	static float	AtoFloat(CString);
	static float	AtoFloat(string str) { return AtoFloat(CString(str.c_str())); };
	static int	AtoInt(CString str) ;
	static int	AtoInt(string str) { return AtoInt(CString(str.c_str()));};
	static double AtoDouble(CString);
	static void	StrClean(CString&);
	static float Exp(int exp);
	static unsigned long GetGroupColor(int);
	static std::vector<string> GetBinFileNames();
	static std::vector<string> GetTxtFileNames();
	static std::string GetCSVFileName();
	static std::vector<string> GetCSVFileNames();
	static std::string GetCurrentDateTime();
	static std::vector<string> GetBinTxtFileNames();
	static std::vector<string> GetSimFileNames();
	static std::vector<string> GetMBinFileNames();
	static std::vector<string> GetSBInFileNames();
	static std::string GetSBinFileName();
	static std::string GetTxtFileName();
	static std::string GetBinTxtFileName();
	static std::string GetSimFileName();
	static std::string GetBinFileName();
	static bool isHighVertResolution();
	static std::string GetWindowFileDate(string filename, bool modified=false);
	static int GetLowResFontSize(int fontsize);
	static int GetHighResFontSize(int fontsize);
	static int GetFontSize();
	static int GetBestGridRowHeight();
	static double GetColumnWidthScaleFactor();

		void SetPaneMessage(string str);

	static void MessageBox(string str) { CString koll;
	koll = CString(str.c_str());   ::MessageBox(nullptr,koll,_T("Message"),0);
	};
	static void MessageBox(string str,string str2, int Mes) {
		CString koll;
		koll = CString(str.c_str());   ::MessageBox(nullptr, koll, CString(str2.c_str()), Mes);
	};
	static int MessageBoxReturn(string str, std::string str2, int Mes) {
		CString koll;
		koll = CString(str.c_str());   return::MessageBox(nullptr, koll, CString(str2.c_str()), Mes);
	};

	static int GetColorLine(int);
	static bool SetColorLine(int line, int color);
private:
	CWinApp *m_pApp;
	CMainFrame *m_pMainFrame;

	static std::vector<string> GetFileNames(LPCTSTR lpszFilter, LPCTSTR lpszDefExt);
	static std::vector<string> GetFileNames(LPCTSTR lpszFilter, std::string DefExt);

	static std::string GetSingleFileName(LPCTSTR lpszFilter, LPCTSTR lpszDefExt);

};

