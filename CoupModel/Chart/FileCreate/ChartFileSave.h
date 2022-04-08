#pragma once
class Doc;
class CChartViewer;
class ChartFileSave
{
public:
	ChartFileSave(void);
	~ChartFileSave(void);
	void MakeFile(Doc *pDoc=nullptr);
	void OnReDraw(bool reload, Doc *pDoc=nullptr);
	void OnXYZoomPB(CChartViewer *pViewer);
	void OnZoomOutPB(CChartViewer *pViewer);
	void OnZoomInPB(CChartViewer *pViewer);
	void OnXZoomPB(CChartViewer *pViewer);
	void OnPointerPB(CChartViewer *pViewer);
	void SaveToExtraChartWindow();
	void SaveToFileExtraChart(Doc *pDoc=nullptr);
	void ShiftExtraChartWindow();
	bool m_NotifySaveFile;
	bool m_MakeExtraChart;
	bool m_SaveExtraChart;
	bool m_SaveFile;
	bool m_NoChange;
	bool m_ZoomX;
	bool m_ZoomXY;
};

