#pragma once
#ifndef MISSING
#define MISSING -1.1E38f
#define NUM_SERIES 400
#define NUM_BAND 100
#define NUM_TIME 50
#endif
#ifndef _HAS_STD_BYTE
#define _HAS_STD_BYTE 0
#endif
#include <mutex>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>


#ifndef NOSIMMODEL
//#include "./SoilData/GetSoilStorage.h"
#include "./SoilData/Plotpf/PFCurve.h"
#include "./SoilData/PlotPF/PFStorage.h"
#include "./NewBase/CommonModelInfo.h"
#include "./Util/CoupSim_XML.h"


  


class MyCug;
class FunctionProperties;
class ChartCug;
class MyPGCug;
class PGFileArchive;
class SimArchive;
class CCoupModelDoc;
class CClassView;
class CFileView;
class CMainFrame;


class ChartProp;
class ChartUtil;
class CChartWnd;
class ZoomTimeSerie;
class PlotInfo;
class Register;
class PFCug;
class SoilData;
class DB_Header;
class DBDataCug;
class DB_BASCug;
class PFCug;

//static GetSoilStorage getsoilstorage;
//static GetSoilStorage* pGetSoil = &getsoilstorage;

static CommonModelInfo modelinfo;
static CommonModelInfo* p_ModelInfo=&modelinfo;



// SINGLETON

struct PlotpF {
	static PFCurve& GetPointer() {static PFCurve instance; return instance; };
};

class MySingle {

private:
	MySingle();
	Register* m_pRegister;
	MyCug* m_pCug;
	MyPGCug* m_pPGCug;
	ChartCug* m_pChartCug;
	FunctionProperties* m_pFunc;
	PGFileArchive* m_pPGArchive;
	SimArchive* m_pSimArchive;
	CCoupModelDoc* m_pPrevDoc;
	CClassView* m_pClassView;
	CFileView* m_pFileView;
	CMainFrame* m_pFrame;

	ChartProp* m_pChartProp;
	ChartUtil* m_pChartUtil;
	CChartWnd* m_pExtraChartWnd;
	ZoomTimeSerie* m_pZoomTimeSerie;
	PlotInfo* m_pPlotInfo;
	PFCug* m_PFCug;
	SoilData* m_SoilData;
	DB_Header* m_pDB;
	DB_BASCug* m_pDBBasCug;
	DBDataCug* m_pDBDataCug;
	//PFCurve* m_PFpointer;

public:
	
	// Not thread safe!!!!!!!!
	// Return by reference
	static MySingle& instance() {
		static MySingle mySingle;
		return mySingle;
	}
	


	void SetDBDataPointer(DBDataCug* pDBData) noexcept { m_pDBDataCug = pDBData; };
	DBDataCug* GetDBDataPointer()  noexcept { return m_pDBDataCug; };

	void SetDBBasPointer(DB_BASCug* pDBBas)  noexcept { m_pDBBasCug = pDBBas; };
	DB_BASCug* GetDBBasPointer() noexcept { return m_pDBBasCug; };


	void SetSoilDataPointer(SoilData* pSoilData)  noexcept { m_SoilData = pSoilData; };
	SoilData* GetSoilDataPointer()  noexcept { return m_SoilData; };

	void SetDB_HeaderPointer(DB_Header* pDB)  noexcept { m_pDB = pDB; };
	DB_Header* GetDB_HeaderPointer() noexcept { return m_pDB; };


	void SetPFCugPointer(PFCug* pPFCug)  noexcept { m_PFCug = pPFCug; };
	PFCug* GetPFCugPointer()  noexcept { return m_PFCug; };

	void SetFramePointer(CMainFrame* pFrame)  noexcept { m_pFrame = pFrame; };
	CMainFrame* GetFramePointer()  noexcept { return m_pFrame; };

	void SetModelPointer(CClassView* pClass)  noexcept { m_pClassView = pClass; };
	CClassView* GetClassViewPointer()  noexcept { return m_pClassView; };

	void SetFilePointer(CFileView* pFile) noexcept { m_pFileView = pFile; };
	CFileView* GetFileViewPointer() noexcept { return m_pFileView; };

	void SetCugPointer(MyCug* pCug)  noexcept { m_pCug = pCug; };
	MyCug* GetCugPointer()  noexcept { return m_pCug; };

	void SetPGCugPointer(MyPGCug* pCug) noexcept { m_pPGCug = pCug; };
	MyPGCug* GetPGCugPointer() noexcept { return m_pPGCug; };

	void SetPGFileArchivePointer(PGFileArchive* pArchive)  noexcept { m_pPGArchive = pArchive; };
	PGFileArchive* GetPGFileArchivePointer() noexcept { return m_pPGArchive; };

	void SetSimArchivePointer(SimArchive* pArchive)  noexcept { m_pSimArchive = pArchive; };
	SimArchive* GetSimArchivePointer()  noexcept { return m_pSimArchive; };

	void SetChartCugPointer(ChartCug* pCug) noexcept { m_pChartCug = pCug; };
	ChartCug* GetChartCugPointer()  noexcept { return m_pChartCug; };

	void SetFunctionPointer(FunctionProperties* pFunc)  noexcept { m_pFunc = pFunc; };
	FunctionProperties* GetFunctionPointer() noexcept { return m_pFunc; };

	void SetChartPropPointer(ChartProp* pProp)  noexcept { m_pChartProp = pProp; };
	ChartProp* GetChartPropPointer() noexcept { return m_pChartProp; };

	void SetPlotInfoPointer(PlotInfo* pProp) noexcept { m_pPlotInfo = pProp; };
	PlotInfo* GetPlotInfoPointer()  noexcept { return m_pPlotInfo; };

	void SetChartUtilPointer(ChartUtil* pUtil)  noexcept { m_pChartUtil = pUtil; };
	ChartUtil* GetChartUtilPointer()  noexcept { return m_pChartUtil; };

	void SetChartZoomPointer(ZoomTimeSerie* pZoom)  noexcept { m_pZoomTimeSerie = pZoom; };
	ZoomTimeSerie* GetChartZoomPointer()  noexcept { return m_pZoomTimeSerie; };

	void SetRegisterPointer(Register* pReg)  noexcept { m_pRegister = pReg; };
	Register* GetRegisterPointer() noexcept { return m_pRegister; };

	void SetExtraChartWndPointer(CChartWnd* pExtraChart)  noexcept { m_pExtraChartWnd = pExtraChart; };
	CChartWnd* GetExtraChartWndPointer()  noexcept { return m_pExtraChartWnd; };

	void SetActDoc(CCoupModelDoc* pDoc)  noexcept { m_pPrevDoc = pDoc; };
	bool IsActDoc(CCoupModelDoc* pDoc)  noexcept { if (pDoc == m_pPrevDoc) return true; else return false; };

	//void SetPlotPFPointer(PFCurve* pPFCurve) { m_PFpointer=pPFCurve; };

};		


#endif