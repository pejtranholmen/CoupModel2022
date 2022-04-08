
#include "../../std.h"
#include "ChartFrame.h"


ChartFrame::ChartFrame(void)
{
	
}


ChartFrame::~ChartFrame(void)
{
}

void ChartFrame::SetTitle(string str)
{
	m_Title=str;
}
int ChartFrame::GetAxisLayer(int index)
{
	if(index>=0&&index<m_AxisLayer.size())
		return m_AxisLayer.at(index);
	else
		return -1;
}
bool ChartFrame::SetAxisLayer(int index, int value)
{
	if(index>=0&&index<m_AxisLayer.size()){
		m_AxisLayer[index]= value;
		return true;	
	}
	else
		return false;
}


bool ChartFrame::SetDashModeLine(int line, int value)
{
	if(m_DashStyle.size()>line) {
		m_DashStyle[line]= value;
		return true;
	}
	else if (m_DashStyle.size() == size_t(line)) {
		m_DashStyle.push_back(value);
	}
	else if (m_DashStyle.size() < line) {
		while (m_DashStyle.size() < line)
			m_DashStyle.push_back(0);
		m_DashStyle.push_back(value);
	}
	return true;
}
int ChartFrame::GetDashModeLine(int line)
{
	if(m_DashStyle.size()>line)
		return m_DashStyle.at(line);
	return 0;
}
bool ChartFrame::SetLineWidth(int line, int value)
{
	if(m_LineWidth.size()>line) {
		m_LineWidth[line]=value;
		return true;
	}
	else {
		while (m_LineWidth.size() < line)
			m_LineWidth.push_back(0);
		m_LineWidth.push_back(value);
	}
	return true;
}
int ChartFrame::GetLineWidth(int line)
{
	if(line<m_LineWidth.size())
		return m_LineWidth.at(line);
	else
		return 0;
}
bool ChartFrame::SetSymbolStyle(int line, int value)
{
	if(m_SymbolStyle.size()>line) {
		m_SymbolStyle[line]=value;
		return true;
	}
	else if(m_SymbolStyle.size()==line) {
		m_SymbolStyle.push_back(value);
		return true;
	}
	else {
		while (m_SymbolStyle.size() <= line) {
			m_SymbolStyle.push_back(value);
		}
		return true;

	}
	return false;
}
int ChartFrame::GetSymbolStyle(int line)
{
	if(line<m_SymbolStyle.size()) 
		return m_SymbolStyle.at(line);
	else
		return 0;
}
int ChartFrame::GetSymbolStyleCode(int line)
{
	if(line<m_SymbolStyle.size()) {
		switch (m_SymbolStyle.at(line)) {
				case 0:
					return 0;
				case 1:
				case 2:
					return 7;
				case 3:
				case 4:
					return 1;
				case 5:
				case 6:
					return 2;
				case 7:
				case 8:
					return 3;


		}
		return 0;
	}
	else
		return 0;
}
int ChartFrame::GetSymbolStyleOpen(int line)
{
	if(line<m_SymbolStyle.size()) {
		if(m_SymbolStyle.at(line)%2==0)
			return 0;//RGB(GetBValue(m_NoColor),GetGValue(m_NoColor),GetRValue(m_NoColor));
		else
			return -1;

	}
	else
		return -1;
}
bool ChartFrame::SetSymbolSize(int line, int value)
{
	if(m_SymbolSize.size()>line) {
		m_SymbolSize[line]=value;
		return true;
	}
	else if(m_SymbolSize.size()==line) 
		m_SymbolSize.push_back(value);
	else {
		while (m_SymbolSize.size() <= line) {
			m_SymbolSize.push_back(value);
		}
		return false;
	}
	return true;
}
int ChartFrame::GetSymbolSize(int line)
{
	if(line<m_SymbolSize.size())
		return m_SymbolSize.at(line);
	else
		return 0;
}

int ChartFrame::GetNumChartX()
{
	return m_NumChartX;
}
int ChartFrame::GetNumChartY()
{
	return m_NumChartY;
}
int ChartFrame::GetGapMargin()
{
	return m_GapMargin;
}
int ChartFrame::GetLeftMargin()
{
	return m_LeftMargin;
}
int ChartFrame::GetTopMargin()
{
	return m_TopMargin;
}
int ChartFrame::GetRightMargin()
{
	return m_RightMargin;
}
int ChartFrame::GetBottomMargin()
{
	return m_BottomMargin;
}




bool ChartFrame::GetRoundedFrame()
{
	return m_RoundedFrame;
}
bool ChartFrame::GetCommonColors()
{
	return m_CommonColorForSubCharts;
}


int ChartFrame::GetNumSubCharts()
{
	return m_NumSubCharts;
}



void ChartFrame::SetRightMargin(int value)
{
	m_RightMargin=value;
}

void ChartFrame:: SetNumChartX(int value) 
{m_NumChartX=value;
}
void ChartFrame::SetNumChartY(int value)
{
		m_NumChartY=value;
}
void ChartFrame:: SetGapMargin(int value) 
{m_GapMargin=value;}

void ChartFrame::SetLeftMargin(int value) 
	{m_LeftMargin=value;}

void ChartFrame::SetTopMargin(int value) 
	{m_TopMargin=value;}
void ChartFrame::SetBottomMargin(int value) 
	{m_BottomMargin=value;}


void ChartFrame::SetVisibleFrames(int value)
	{m_VisibleFrames=value;}
	
void ChartFrame::SetChartError(int value)
{
	m_ChartError=value;
}
string ChartFrame::GetTitle()
{
	return m_Title;
}

