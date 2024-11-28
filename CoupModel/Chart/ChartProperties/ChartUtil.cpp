#include "ChartUtil.h"
#include "../../Util\MFC_Util.h"


ChartUtil::ChartUtil(void)
{
	MySingle::instance().SetChartUtilPointer(this);
	m_Displayed=false;
	m_MultiPGPlot=false;
	m_MultiRunPlot=false;
	m_AutoShiftFirstIndep = false;
	m_SelectProfileViewFrequence = 5;
	m_X_AxisAutoScaleSensitivy = 0;


	//SetAutoX_ScaleValue(m_X_AxisAutoScaleSensitivy);
	

}


ChartUtil::~ChartUtil(void)
{

}
bool ChartUtil::SetColorBackground(int color)
{
	CWinApp* m_pApp;
	m_pApp=AfxGetApp();
	int colorH;
	colorH=RGB(GetBValue(color),GetGValue(color),GetRValue(color));
	MFC_Util::WriteProfileInt( "ColorBackground", color);
	MFC_Util::WriteProfileInt( "HColorBackground", colorH);
	return true;
}

int ChartUtil::GetColorBackground()
{
	CWinApp* pApp;
	pApp=AfxGetApp();
	m_ColorBackground=0xf0f0ff;
	m_ColorBackground=MFC_Util::GetProfileIntNo( "ColorBackground", m_ColorBackground);
	return m_ColorBackground;


}
bool ChartUtil::SetColorBoxEdge(int color)
{
	CWinApp* m_pApp;
	m_pApp=AfxGetApp();
	int colorH;
	colorH=RGB(GetBValue(color),GetGValue(color),GetRValue(color));
	MFC_Util::WriteProfileInt( "ColorEdgeBackground", color);
	MFC_Util::WriteProfileInt( "HColorEdgeBackground", colorH);
	return true;
}
int ChartUtil::GetColorBoxEdge()
{
	CWinApp* pApp;
	pApp=AfxGetApp();
	m_BoxEdgeColor=0xf0f0ff;
	m_BoxEdgeColor=MFC_Util::GetProfileIntNo( "ColorEdgeBackground", m_BoxEdgeColor);
	return m_BoxEdgeColor;

}
bool ChartUtil::SetColorDist(int color)
{
	CWinApp* m_pApp;
	m_pApp=AfxGetApp();
	int colorH;
	colorH=RGB(GetBValue(color),GetGValue(color),GetRValue(color));
	MFC_Util::WriteProfileInt( "ColorDistBox", color);
	MFC_Util::WriteProfileInt( "HColorDistBox", colorH);
	return true;
}

int ChartUtil::GetEnsembleViewOption()
{
	CWinApp* pApp;
	pApp = AfxGetApp();
	m_EnsembleViewOption = MFC_Util::GetProfileIntNo("EnsembleViewOption", m_EnsembleViewOption);
	return m_EnsembleViewOption;
}
void ChartUtil::SetEnsembleViewOption(int value)
{
	CWinApp* m_pApp;
	m_pApp = AfxGetApp();
	MFC_Util::WriteProfileInt("EnsembleViewOption", value);
	m_EnsembleViewOption = value;
}

int ChartUtil::GetColorDistH()
{
	CWinApp* pApp;
	pApp=AfxGetApp();
	m_DistColor=MFC_Util::GetProfileIntNo( "HColorDistBox", m_ColorLegendBackgroundFrame);
	return m_DistColor;

}
int ChartUtil::GetColorDist()
{
	CWinApp* pApp;
	pApp=AfxGetApp();
	m_DistColor=0xf0f0ff;
	m_DistColor=MFC_Util::GetProfileIntNo( "ColorDistBox", m_DistColor);
	return m_DistColor;

}
int ChartUtil::GetColorBoxEdgeH()
{
	CWinApp* pApp;
	pApp=AfxGetApp();
	int color;
	color=MFC_Util::GetProfileIntNo( "HColorEdgeBackground", m_ColorLegendBackgroundFrame);
	return color;

}

bool ChartUtil::SetColorLegendBackground(int color)
{
	CWinApp* m_pApp;
	m_pApp=AfxGetApp();
	int colorH;
	colorH=RGB(GetBValue(color),GetGValue(color),GetRValue(color));
	MFC_Util::WriteProfileInt( "ColorLegendBackground", color);
	MFC_Util::WriteProfileInt( "HColorLegendBackground", colorH);
	return true;
}
bool ChartUtil::SetColorLegendBackgroundFrame(int color)
{
	CWinApp* m_pApp;
	m_pApp=AfxGetApp();
	int colorH;
	colorH=RGB(GetBValue(color),GetGValue(color),GetRValue(color));
	MFC_Util::WriteProfileInt( "ColorLegendBackgroundFrame", color);
	MFC_Util::WriteProfileInt( "HColorLegendBackgroundFrame", colorH);
	return true;
}
int ChartUtil::GetColorLegendBackground()
{

	CWinApp* pApp;
	pApp=AfxGetApp();
	m_ColorLegendBackground=0xf0f0ff;
	m_ColorLegendBackground=MFC_Util::GetProfileIntNo( "ColorLegendBackground", m_ColorLegendBackground);
	return m_ColorLegendBackground;
}
int ChartUtil::GetColorLegendBackgroundFrame()
{

	CWinApp* pApp;
	pApp=AfxGetApp();
	m_ColorLegendBackgroundFrame=0xf0f0ff;
	m_ColorLegendBackgroundFrame=MFC_Util::GetProfileIntNo( "ColorLegendBackgroundFrame", m_ColorLegendBackgroundFrame);
	return m_ColorLegendBackgroundFrame;
}
void ChartUtil::SetLegendContentStyle(int value)
{
	m_LegendContentStyle=value;
	MFC_Util::WriteProfileInt( "LegendContentStyle", m_LegendContentStyle);
}
void ChartUtil::SetResidualRepresentation(int value)
{
	m_ResidualRepresentation=value;
	MFC_Util::WriteProfileInt( "ResidualPlot", m_ResidualRepresentation);
}
void ChartUtil::SetErrorSignal(int value)
{
	m_ErrorSignal=false;
	if(value==1) m_ErrorSignal=true;
	MFC_Util::WriteProfileInt( "ErrorSignal", m_ErrorSignal);
}
void ChartUtil::SetRemoveXAxisScale(int value)
{
	m_RemoveXAxisScale=value;
	MFC_Util::WriteProfileInt( "RemoveXAxisScale", m_RemoveXAxisScale);
}
void ChartUtil::SetRemoveYAxisScale(int value)
{
	m_RemoveYAxisScale=value;
	MFC_Util::WriteProfileInt( "RemoveYAxisScale", m_RemoveYAxisScale);
}
void ChartUtil::SetRemoveLegends(int value)
{
	m_RemoveLegends=value;
	MFC_Util::WriteProfileInt( "RemoveLegends", m_RemoveLegends);
}
void ChartUtil::SetRemoveTitle(int value)
{
	m_RemoveTitle=value;
	MFC_Util::WriteProfileInt( "RemoveTitle", m_RemoveTitle);
}
void ChartUtil::SetGapHorPercentage(int value)
{
	if(value>100)
		value=100;
	else if (value<0)
		value=0;
	MFC_Util::WriteProfileInt( "HorizontalGapPercentage", value);
	m_GapHFraction=float(value)/100;
}	
void ChartUtil::SetGapHorLeftPercentage(int value)
{
	if(value>100)
		value=100;
	else if (value<0)
		value=0;
	MFC_Util::WriteProfileInt( "HorizontalGapLeftPercentage", value);
	m_GapHorLeftFraction=float(value)/100;
}	
void ChartUtil::SetGapVertTopPercentage(int value)
{
	if(value>100)
		value=100;
	else if (value<0)
		value=0;
	MFC_Util::WriteProfileInt( "VerticalTopPercentage", value);
	m_GapVertTopFraction=float(value)/100;
}	

int ChartUtil::GetHColorLine(int line) 
{
	CWinApp* m_pApp;
	m_pApp=AfxGetApp();
	int color;
	color=MFC_Util::GetProfileIntNo( "HColorLine1", 0x0000ff);
	switch (line%11)
	{
		case 0:
		color=MFC_Util::GetProfileIntNo( "HColorLine1", 0x0000ff);
		break;
		case 1:
		color=MFC_Util::GetProfileIntNo( "HColorLine2",0x00cc00 );
		break;
		case 2:
		color=MFC_Util::GetProfileIntNo( "HColorLine3", 0xff0000);
		break;
		case 3:
		color=MFC_Util::GetProfileIntNo( "HColorLine4", 0xcc0000);
		break;
		case 4:
		color=MFC_Util::GetProfileIntNo( "HColorLine5", 0x00aa00);
		break;
		case 5:
		color=MFC_Util::GetProfileIntNo( "HColorLine6", 0xA52A2A);
		break;
		case 6:
		color=MFC_Util::GetProfileIntNo( "HColorLine7", 0x8B008B);
		break;
		case 7:
		color=MFC_Util::GetProfileIntNo( "HColorLine8", 0x8B0000);
		break;
		case 8:
		color=MFC_Util::GetProfileIntNo( "HColorLine9", 0x87CEFA);
		break;
		case 9:
		color=MFC_Util::GetProfileIntNo( "HColorLine10", 0xFFFFE0);
		break;
		case 10:
		color=m_pApp->GetProfileInt(_T("CoupModel"), _T("HColorLine11"), 0xbbccdd);
		break;
	}
	return color;
}
int ChartUtil::GetSymbolIndex(string str){
	int index;
	string optionlist;
	optionlist="no symbol\nFilled circle\nOpen circle\nFilled square\n";
	optionlist+="Open square\nFilled diamond\nOpen diamond\nFilled triangle\nOpen triangle\n";

	int ipos, ipos_end;
	ipos_end=optionlist.find(str);
	
	optionlist=optionlist.substr(0,ipos_end);
	ipos=optionlist.find("\n");
	index=0;
	while(ipos>0) {
		optionlist=optionlist.substr(ipos+1);
		ipos=optionlist.find("\n");
		index++;
	}
	return index;
}
int ChartUtil::GetVisibleIndex(string str){
	int index;
	string optionlist;

	optionlist="Transparent\nHigh Transparancy\nNormal Transparancy\nLimited Transparancy\nNo Tranparancy\n";
	int ipos, ipos_end;
	ipos_end=optionlist.find(str);
	
	optionlist=optionlist.substr(0,ipos_end);
	ipos=optionlist.find("\n");
	index=0;
	while(ipos>0) {
		optionlist=optionlist.substr(ipos+1);
		ipos=optionlist.find("\n");
		index++;
	}
	return index;
}
int ChartUtil::GetSymbolSizeIndex(string str) {
	int index;
	string optionlist;
	optionlist="Common\nVery small\nSmall\nNormal\nLarge\n";
	optionlist+="Very Large\nHuge\n";

	int ipos, ipos_end;
	ipos_end=optionlist.find(str);
	
	optionlist=optionlist.substr(0,ipos_end);
	ipos=optionlist.find("\n");
	index=0;
	while(ipos>0) {
		optionlist=optionlist.substr(ipos+1);
		ipos=optionlist.find("\n");
		index++;
	}
	return index;



}
int ChartUtil::GetLineTypeIndex(int lineNo)
{
 	string id = "LineType" + FUtil::STD_ItoAscii(lineNo);
	return MFC_Util::GetProfileIntNo(id, 0);
}
int ChartUtil::GetSpecificLineWidth(int lineNo)
{
	string id = "LineWidth" + FUtil::STD_ItoAscii(lineNo);
	return MFC_Util::GetProfileIntNo(id, 0);
}
int ChartUtil::GetSpecificSymbolSize(int lineNo)
{
	string id = "SymbolSize" + FUtil::STD_ItoAscii(lineNo);
	return MFC_Util::GetProfileIntNo(id, 0);
}
int ChartUtil::GetSpecificSymbol(int lineNo)
{
	string id = "SymbolType" + FUtil::STD_ItoAscii(lineNo);
	return MFC_Util::GetProfileIntNo(id, 0);
}
void ChartUtil::SetLineTypeIndex(int lineNo, int value)
{  
	string id = "LineType" + FUtil::STD_ItoAscii(lineNo);
	return MFC_Util::WriteProfileInt(id, value);
}

void ChartUtil::SetSpecificLineWidth(int lineNo, int value)
{
	string id = "LineWidth" + FUtil::STD_ItoAscii(lineNo);
	return MFC_Util::WriteProfileInt(id, value);
}

void ChartUtil::SetSpecificSymbolSize(int lineNo, int value)
{
	string id = "SymbolSize" + FUtil::STD_ItoAscii(lineNo);
	return MFC_Util::WriteProfileInt(id, value);
}

void ChartUtil::SetSpecificSymbol(int lineNo, int value)
{
	string id = "SymbolType" + FUtil::STD_ItoAscii(lineNo);
	return MFC_Util::WriteProfileInt(id, value);
}
void ChartUtil::SetSymbolCodes() {
	for (size_t i = 0; i < 10; i++) {
		auto value = GetSpecificSymbol(i);
		switch (CHART_SYMBOLS(value)) {
			case CHART_SYMBOLS::NO_SYMBOL:
				m_SymbolCode[i] = 0;
				m_SymbolOpenCode[i] = -1;
				break;
			case CHART_SYMBOLS::FILLED_CIRCLE:
				m_SymbolCode[i] = 7;  m_SymbolOpenCode[i] = -1;	break;
			case CHART_SYMBOLS::OPEN_CIRCLE:
				m_SymbolCode[i] = 7;  m_SymbolOpenCode[i] = 1;	break;
			case CHART_SYMBOLS::FILLED_SQUARE:
				m_SymbolCode[i] = 1;  m_SymbolOpenCode[i] = -1;	break;
			case CHART_SYMBOLS::OPEN_SQUARE:
				m_SymbolCode[i] = 1;  m_SymbolOpenCode[i] = 0;	break;
			case CHART_SYMBOLS::FILLED_DIAMOND:
				m_SymbolCode[i] = 2;  m_SymbolOpenCode[i] = -1;	break;
			case CHART_SYMBOLS::OPEN_DIAMOND:
				m_SymbolCode[i] = 2;  m_SymbolOpenCode[i] = 0;	break;
			case CHART_SYMBOLS::FILLED_RECTANGLE:
				m_SymbolCode[i] = 3;  m_SymbolOpenCode[i] = -1;	break;
			case CHART_SYMBOLS::OPEN_RECTANGLE:
				m_SymbolCode[i] = 3;  m_SymbolOpenCode[i] = 0;	break;
		}
	 }
}




int ChartUtil::GetLineTypeIndex(string str) {
	int index;
	string optionlist;
	optionlist="Solid\nDashed Line\nDotted Line\nDot Dashed Line\n";
	optionlist+="Alternativ Dashed Line\n";

	int ipos, ipos_end;
	ipos_end=optionlist.find(str);
	
	optionlist=optionlist.substr(0,ipos_end);
	ipos=optionlist.find("\n");
	index=0;
	while(ipos>0) {
		optionlist=optionlist.substr(ipos+1);
		ipos=optionlist.find("\n");
		index++;
	}
	return index;

}
int ChartUtil::GetLineThickSizeIndex(string str) {
	int index;
	string optionlist;
	optionlist="Common\nThin\nNormal\nLarge\nVery large";

	int ipos, ipos_end;
	ipos_end=optionlist.find(str);
	
	optionlist=optionlist.substr(0,ipos_end);
	ipos=optionlist.find("\n");
	index=0;
	while(ipos>0) {
		optionlist=optionlist.substr(ipos+1);
		ipos=optionlist.find("\n");
		index++;
	}
	if(index>0) {
		return index;
	}
	return index;
}
int ChartUtil::GetZoom_ValDurOption(string str)
{
	int index;
	string optionlist;
	optionlist="Full Range\nYear\nHalf Year\nMonth\nWeek\nDay\nHalf Day\nHour\n";

	int ipos, ipos_end;
	ipos_end=optionlist.find(str);
	
	optionlist=optionlist.substr(0,ipos_end);
	ipos=optionlist.find("\n");
	index=0;
	while(ipos>0) {
		optionlist=optionlist.substr(ipos+1);
		ipos=optionlist.find("\n");
		index++;
	}
	return index;

}
int ChartUtil::GetLegendXPos()
{
return m_LegendPositionX;
}
int ChartUtil::GetLegendYPos()
{
	return m_LegendPositionY;
}
int ChartUtil::GetMinTickDistX()
{
	return m_MinTickDistX;
}
int ChartUtil::GetMinTickDistY()
{
	return m_MinTickDistY;
}
void ChartUtil::SetMinTickDistY(int value )
{m_MinTickDistY=value;
}
void ChartUtil::SetProfileViewFrequence(int Frequence)
{
	MFC_Util::WriteProfileInt("ProfileViewFrequence", Frequence);
}
size_t ChartUtil::GetProfileViewFrequence()
{
	int value = 5;
	value = MFC_Util::GetProfileIntNo("ProfileViewFrequence", value);
	return size_t(value);
}
void ChartUtil::SetMinTickDistX(int value )
{
	m_MinTickDistX=value;
}
float ChartUtil::GetScaleInterval(float max, float min) {
	float out;
	out=(max-min)/12.f;	
	float error = 0.1;
	
	if (out > 1.) {
		error = out - int(out);
		if (error > 0.25 && error < 0.75) out = int(out) + 0.5;
		else if (error <= 0.25) out = int(out);
		else out = int(out + 1);
	}
	else if (out > 0.1) {
		error = out - int(out);
		if (error > 0.25 && error < 0.75) out = int(out) + 1.0;
		else out = int(1);
	}
	else
		out = (max - min) / 10.f;



/*

	if (out - int(out) > error) out = (max - min) / 9.f;
		else error = out - int(out);
	if (out - int(out) > error) out = (max - min) / 8.f; else error = out - int(out);
	if(out-int(out)>error) out=(max-min)/7.f;
	if(out-int(out)>error) out=(max-min)/6.f;*/
	
	return out;
}

void ChartUtil::SaveCurrentLinePatternScheme(CHART_OPTION value) {
	string pattern;
	switch (value) {
	case CHART_OPTION::LINESTYLE:
		for (int i = 0; i < 9; i++)
			pattern += FUtil::STD_ItoAscii(GetLineTypeIndex(i)) + ";";
		pattern += FUtil::STD_ItoAscii(GetLineTypeIndex(9));
		MFC_Util::WriteProfileString("LinePattern_1", pattern);
		break;
	case CHART_OPTION::LINEWIDTH:
		for (int i = 0; i < 9; i++)
			pattern += FUtil::STD_ItoAscii(GetSpecificLineWidth(i)) + ";";
		pattern += FUtil::STD_ItoAscii(GetSpecificLineWidth(9));
		MFC_Util::WriteProfileString("LinePattern_0", pattern);
		break;
	case CHART_OPTION::SYMBOLSTYLE:
		for (int i = 0; i < 9; i++)
			pattern += FUtil::STD_ItoAscii(GetSpecificSymbol(i)) + ";";
		pattern += FUtil::STD_ItoAscii(GetSpecificSymbol(9));
		MFC_Util::WriteProfileString("LinePattern_2", pattern);
		break;
	case CHART_OPTION::SYMBOLSIZE:
		for (int i = 0; i < 9; i++)
			pattern += FUtil::STD_ItoAscii(GetSpecificSymbolSize(i)) + ";";
		pattern += FUtil::STD_ItoAscii(GetSpecificSymbolSize(9));
		MFC_Util::WriteProfileString("LinePattern_3", pattern);
		break;
	}
}

void ChartUtil::RetrieveSavedLinePatternScheme(CHART_OPTION value)
{
	string pattern;
	size_t ikoll=size_t(value);
	string add=FUtil::STD_ItoAscii(ikoll);
	pattern = MFC_Util::GetProfileStringStd("LinePattern_"+add, "0;0;0;0;0;0;0;0;0;0");

	vector<string> split = FUtil::GetStringVectorFromStringLine(pattern);

	switch (value) {
	case CHART_OPTION::LINESTYLE:
		for (size_t i = 0; i < 10; i++) SetLineTypeIndex(i, FUtil::AtoInt(split[i]));
		break;
	case CHART_OPTION::LINEWIDTH:
		for (size_t i = 0; i < 10; i++) SetSpecificLineWidth(i, FUtil::AtoInt(split[i]));
		break;
	case CHART_OPTION::SYMBOLSTYLE:
		for (size_t i = 0; i < 10; i++) SetSpecificSymbol(i, FUtil::AtoInt(split[i]));
		break;
	case CHART_OPTION::SYMBOLSIZE:
		for (size_t i = 0; i < 10; i++) SetSpecificSymbolSize(i, FUtil::AtoInt(split[i]));
		break;
	}
}

void ChartUtil::ResetLinePatternScheme(CHART_OPTION value)
{
	switch (value) {
		case CHART_OPTION::LINESTYLE:
			for (size_t i = 0; i < 10; i++) SetLineTypeIndex(i, 0);
			break;
		case CHART_OPTION::LINEWIDTH:
			for (size_t i = 0; i < 10; i++) SetSpecificLineWidth(i, 0);
			break;
		case CHART_OPTION::SYMBOLSTYLE:
			for (size_t i = 0; i < 10; i++) SetSpecificSymbol(i, 0);
			SetSymbolCodes();
			break;
		case CHART_OPTION::SYMBOLSIZE  :
			for (size_t i = 0; i < 10; i++) SetSpecificSymbolSize(i, 0);
			break;
	}
}
void ChartUtil::SetAutoX_ScaleValue(int value) {

	if (m_X_AxisAutoScaleSensitivy > 0)
		m_X_AxisAutoScaleValue = pow(10., -m_X_AxisAutoScaleSensitivy);
	else
		m_X_AxisAutoScaleValue = 0.;
	MFC_Util::WriteProfileInt("x_AXIS_AutoScale", value);


}



