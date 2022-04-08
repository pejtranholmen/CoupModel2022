#pragma once
class NumFrames
{
public:
	NumFrames(void);
	~NumFrames(void);
	size_t m_NumMultiChart;
	size_t m_NumMultiChartX;
	size_t m_NumMultiChartY;
	size_t m_NumSubCharts,m_NumChartX,m_NumChartY;
	void SetDisplaySize(int cx, int cy) {m_DisplayXSize=cx; m_DisplayYSize=cy;};
	int m_DisplayXSize, m_DisplayYSize;

};

