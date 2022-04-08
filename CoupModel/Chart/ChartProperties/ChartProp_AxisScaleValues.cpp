
#include "./ChartProp_AxisScaleValues.h"
#include "../../CoupModelDoc.h"

ChartProp_AxisScaleValues::ChartProp_AxisScaleValues(void)
{
	m_LegendChangeAny = false;

	m_CommonXScale = false;
	m_freeze = false;





}


ChartProp_AxisScaleValues::~ChartProp_AxisScaleValues(void)
{
	
}
bool ChartProp_AxisScaleValues::Reset_Selected_Or_All_BasicVector(size_t NumVariables, size_t NumSubCharts, bool OnlyNumElements)
{
	//m_Legend1..resize(NumSubCharts)();
	m_VerticalLegends = MFC_Util::GetProfileIntNo("VerticalLegends", 0);

if(OnlyNumElements) {
		ResetSerie1();

	
		m_XScaleSet.resize(NumSubCharts);
		m_XScaleChanged.resize(NumSubCharts);
		m_YScaleLeftChanged.resize(NumSubCharts);
		m_YScaleLeftSet.resize(NumSubCharts);
		m_YScaleRightChanged.resize(NumSubCharts);
		m_YScaleRightSet.resize(NumSubCharts);
		m_YLabel1.resize(NumSubCharts);
		m_YLabel2.resize(NumSubCharts);
		m_XLabel.resize(NumSubCharts);
		m_ZLabel.resize(NumSubCharts);
		m_YLabel1Change.resize(NumSubCharts);
		m_YLabel2Change.resize(NumSubCharts);
		m_XLabelChange.resize(NumSubCharts);
	
		m_ZLabelChange.resize(NumSubCharts);
		m_ZScaleChanged.resize(NumSubCharts);
		m_ZScaleSet.resize(NumSubCharts);


}
else {

		if(NumVariables==0) { 
			ResetSerie1(true);
			ResetSerie2(true);
		}
		m_XScaleChanged.clear();

		m_YScaleLeftChanged.clear();
		m_YScaleLeftSet.clear();
		m_YScaleRightChanged.clear();
		m_YScaleRightSet.clear();
		m_YLabel1.clear();
		m_YLabel2.clear();
		m_XLabel.clear();
		m_ZLabel.clear();
		m_YLabel1Change.clear();
		m_YLabel2Change.clear();
		m_XLabelChange.clear();
	
		m_ZLabelChange.clear();
		m_ZScaleChanged.clear();
		m_ZScaleSet.clear();


		for(size_t i=0;i<NumSubCharts;i++) {
			m_YScaleLeftChanged.push_back(false);
			m_YScaleLeftSet.push_back(false);
			m_YScaleRightChanged.push_back(false);
			m_YScaleRightSet.push_back(false);
			m_XScaleChanged.push_back(false);
			m_XScaleSet.push_back(false);
			m_YLabel1.push_back("-");
			m_YLabel2.push_back("-");
			m_XLabel.push_back("-");
			m_ZLabel.push_back("-");
			m_YLabel1Change.push_back(false);
			m_YLabel2Change.push_back(false);
			m_XLabelChange.push_back(false);
	
			m_ZLabelChange.push_back(false);
			m_ZScaleChanged.push_back(false);
			m_ZScaleSet.push_back(false);

			float pV=0;
			m_XScaleMin.push_back(pV);

			m_XScaleMax.push_back(pV);

			m_XScaleInterv.push_back(pV);

			m_ZScaleMin.push_back(pV);

			m_ZScaleMax.push_back(pV);

			m_ZScaleInterv.push_back(pV);


			m_YScaleLeftMin.push_back(pV);

			m_YScaleRightMin.push_back(pV);

			m_YScaleRightMax.push_back(pV);

			m_YScaleLeftMax.push_back(pV);

			m_YScaleLeftInterv.push_back(pV);

			m_YScaleRightInterv.push_back(pV);
		}
	}
return true;
}
bool ChartProp_AxisScaleValues::isRightYScaleSet(int index)
{
	return false;
}
void ChartProp_AxisScaleValues::SetZmin(int index, float v) {
	while (m_ZScaleMin.size() <= index) m_ZScaleMin.push_back(v);
	m_ZScaleMin[index] = v;
	m_ZScaleSet.at(index) = true;
};
void ChartProp_AxisScaleValues::SetZmax(int index, float v) {
	while (m_ZScaleMax.size() <= index) m_ZScaleMax.push_back(v);
	m_ZScaleMax[index] = v;
	m_ZScaleSet.at(index) = true;
};
void ChartProp_AxisScaleValues::SetZInterv(int index, float v) {
	while (m_ZScaleInterv.size() <= index) m_ZScaleInterv.push_back(v);
	m_ZScaleInterv[index] = v;
};
void ChartProp_AxisScaleValues::SetYLeftmin(int index, float v) {
	while (m_YScaleLeftMin.size() <= index) m_YScaleLeftMin.push_back(v);
	m_YScaleLeftMin[index] = v;
	m_YScaleLeftSet.at(index) = true;
};
void ChartProp_AxisScaleValues::SetYLeftmax(int index, float v) {
	while (m_YScaleLeftMax.size() <= index) m_YScaleLeftMax.push_back(v);
	m_YScaleLeftMax[index] = v;
	m_YScaleLeftSet.at(index) = true;
};
void ChartProp_AxisScaleValues::SetYLeftInterv(int index, float v) {
	while (m_YScaleLeftInterv.size() <= index) m_YScaleLeftInterv.push_back(v);
	m_YScaleLeftInterv[index] = v;
};
void ChartProp_AxisScaleValues::SetYRightmin(int index, float v) {
	while (m_YScaleRightMin.size() <= index) m_YScaleRightMin.push_back(v);
	m_YScaleRightMin[index] = v;
	m_YScaleRightSet.at(index) = true;
};
void ChartProp_AxisScaleValues::SetYRightmax(int index, float v) {
	while (m_YScaleRightMax.size() <= index) m_YScaleRightMax.push_back(v);
	m_YScaleRightMax[index] = v;
	m_YScaleRightSet.at(index) = true;
};
void ChartProp_AxisScaleValues::SetYRightInterv(int index, float v) {
	while (m_YScaleRightInterv.size() <= index) m_YScaleRightInterv.push_back(v);
	m_YScaleRightInterv[index] = v;
	m_YScaleRightSet.at(index) = true;
};



void ChartProp_AxisScaleValues::ResetLegend1(size_t index)
{
	while (m_Legend1Change.size() <= index) m_Legend1Change.push_back(false);
	m_Legend1Change[index] = false;
}



void ChartProp_AxisScaleValues::SetXmax(size_t index, float v) {
	if (index == string::npos) {
		for (size_t i = 0; i < m_XScaleMax.size(); i++) {
			m_XScaleMax[i] = v;
			m_XScaleSet.at(i) = true;
		}
	}
	else {
		while (m_XScaleMax.size() <= index) m_XScaleMax.push_back(v);
		m_XScaleMax[index] = v;
		m_XScaleSet.at(index) = true;
	}
};
void ChartProp_AxisScaleValues::SetXInterv(size_t index, float v) {
	if (index == string::npos) {
		for (size_t i = 0; i < m_XScaleInterv.size(); i++) {
			m_XScaleInterv[i] = v;
			m_XScaleSet.at(i) = true;
		}
	}
	else {
		while (m_XScaleInterv.size() <= index) m_XScaleInterv.push_back(v);
		m_XScaleInterv[index] = v;
	}
};
void ChartProp_AxisScaleValues::SetXmin(size_t index, float v) {
	if (index == string::npos) {
		for (size_t i = 0; i < m_XScaleMin.size(); i++) {
			m_XScaleMin[i] = v;
			m_XScaleSet.at(i) = true;
		}
	}
	else {
		while (m_XScaleMin.size() <= index) m_XScaleMin.push_back(v);
		m_XScaleMin[index] = v;
		while (m_XScaleSet.size() <= index) m_XScaleSet.push_back(true);
		m_XScaleSet.at(index) = true;
	}
};
float ChartProp_AxisScaleValues::GetXmin(int Index) { 
	while (m_XScaleMin.size() <= Index) m_XScaleMin.push_back(0.f);
	return m_XScaleMin[Index]; };
float ChartProp_AxisScaleValues::GetXmax(int Index) {
	while (m_XScaleMax.size() <= Index) m_XScaleMax.push_back(0.f);
	return m_XScaleMax[Index]; };
float ChartProp_AxisScaleValues::GetXInterv(int Index) { 
	while (m_XScaleInterv.size() <= Index) m_XScaleInterv.push_back(0.f);
	return m_XScaleInterv[Index]; };

float ChartProp_AxisScaleValues::GetZmin(int Index) { 
	while (m_ZScaleMin.size() <= Index) m_ZScaleMin.push_back(0.f);
	return m_ZScaleMin[Index]; };
float ChartProp_AxisScaleValues::GetZmax(int Index) { 
	while (m_ZScaleMax.size() <= Index) m_ZScaleMax.push_back(0.f);
	return m_ZScaleMax[Index]; };
float ChartProp_AxisScaleValues::GetZInterv(int Index) { 
	while (m_ZScaleInterv.size() <= Index) m_ZScaleInterv.push_back(0.f);
	return m_ZScaleInterv[Index]; };



float ChartProp_AxisScaleValues::GetYLeftmin(int Index) { 
	while (m_YScaleLeftMin.size() <= Index) m_YScaleLeftMin.push_back(0.f);
	return m_YScaleLeftMin[Index]; };
float ChartProp_AxisScaleValues::GetYLeftmax(int Index) { 
	while (m_YScaleLeftMax.size() <= Index) m_YScaleLeftMax.push_back(0.f);
	return m_YScaleLeftMax[Index]; };
float ChartProp_AxisScaleValues::GetYLeftInterv(int Index) { 
	while (m_YScaleLeftInterv.size() <= Index) m_YScaleLeftInterv.push_back(0.f);
	return m_YScaleLeftInterv[Index]; };


float ChartProp_AxisScaleValues::GetYRightmin(int Index) { 
	while (m_YScaleRightMin.size() <= Index) m_YScaleRightMin.push_back(0.f);
	return m_YScaleRightMin[Index]; };

float ChartProp_AxisScaleValues::GetYRightmax(int Index) { 
	while (m_YScaleRightMax.size() <= Index) m_YScaleRightMax.push_back(0.f);
	return m_YScaleRightMax[Index]; };
float ChartProp_AxisScaleValues::GetYRightInterv(int Index) { 
	while (m_YScaleRightInterv.size() <= Index) m_YScaleRightInterv.push_back(0.f);
	return m_YScaleRightInterv[Index]; };


bool ChartProp_AxisScaleValues::isLeftYScaleChanged(int index) {
	while (m_YScaleLeftChanged.size() <= index) m_YScaleLeftChanged.push_back(false);
	return m_YScaleLeftChanged[index]; };


bool ChartProp_AxisScaleValues::isRightYScaleChanged(int index) {
	while (m_YScaleRightChanged.size() <= index) m_YScaleRightChanged.push_back(false);
	return m_YScaleRightChanged[index]; }
void ChartProp_AxisScaleValues::ResetYLeftScale(size_t index)
{
	while (m_YScaleLeftChanged.size() <= index) m_YScaleLeftChanged.push_back(false);
	m_YScaleLeftChanged[index] = false;
	while (m_YScaleLeftSet.size() <= index) m_YScaleLeftSet.push_back(false);
	m_YScaleLeftSet[index] = false;
}
void ChartProp_AxisScaleValues::ResetYRightScale(size_t index)
{
	while (m_YScaleRightChanged.size() <= index) m_YScaleRightChanged.push_back(false);
	m_YScaleRightChanged[index] = false;

}
void ChartProp_AxisScaleValues::ResetXScale(size_t index)
{
	if (index == string::npos) {
		for (size_t i = 0; i < m_XScaleChanged.size(); i++) {
			m_XScaleChanged[i] = false;
			m_XScaleSet[i] = false;
		}
	}
	else {
		while (m_XScaleChanged.size() <= index) m_XScaleChanged.push_back(false);
		m_XScaleChanged[index] = false;
		while (m_XScaleSet.size() <= index) m_XScaleSet.push_back(false);
		m_XScaleSet[index] = false;
	}

}

void ChartProp_AxisScaleValues::ResetZScale(size_t index)
{
	while (m_ZScaleChanged.size() <= index) m_ZScaleChanged.push_back(false);
	m_ZScaleChanged[index] = false;
}
;



bool ChartProp_AxisScaleValues::isXScaleChanged(int index) {
	while (m_XScaleChanged.size() <= index) m_XScaleChanged.push_back(false);
	return m_XScaleChanged[index];
};
void ChartProp_AxisScaleValues::SetXScaleChanged(size_t index) {
	if (index == string::npos) {
		for (size_t i = 0; i < m_XScaleChanged.size(); i++) {
			m_XScaleChanged[i] = true;
		}

	}
	else {
		while (m_XScaleChanged.size() <= index) m_XScaleChanged.push_back(true);
		m_XScaleChanged.at(index) = true;
	}
}
;

bool ChartProp_AxisScaleValues::isZScaleChanged(int index) {
	while (m_ZScaleChanged.size() <= index) m_ZScaleChanged.push_back(false);
	return m_ZScaleChanged[index]; };
void ChartProp_AxisScaleValues::SetZScaleChanged(int index) {
	while (m_ZScaleChanged.size() <= index) m_ZScaleChanged.push_back(true);
	m_ZScaleChanged.at(index) = true; };
bool ChartProp_AxisScaleValues::isZScaleSet(int index) {
	return m_ZScaleSet[index]; }
void ChartProp_AxisScaleValues::SetAllXScale_toSameValues()
{
	for (size_t i = 1; i < m_XScaleMin.size(); i++) {
		m_XScaleMin[i] = m_XScaleMin[0];
		m_XScaleMax[i] = m_XScaleMax[0];
		m_XScaleInterv[i] = m_XScaleInterv[0];
	}
}




bool ChartProp_AxisScaleValues::isXScaleSet(int index) {
	while (m_XScaleSet.size() <= index) m_XScaleSet.push_back(false);
		return m_XScaleSet[index]; };
	
void ChartProp_AxisScaleValues::ResetYLeftScale() {
	for (size_t i = 0; i<m_YScaleLeftSet.size(); i++)
		m_YScaleLeftSet[i] = false;
	for (size_t i = 0; i<m_YScaleLeftChanged.size(); i++)
		m_YScaleLeftChanged[i] = false;
};
void ChartProp_AxisScaleValues::ResetYRightScale() {
	for (size_t i = 0; i<m_YScaleRightSet.size(); i++)
		m_YScaleRightSet[i] = false;
};
void ChartProp_AxisScaleValues::ResetXScale() {
	for (size_t i = 0; i<m_XScaleSet.size(); i++)
		m_XScaleSet[i] = false;
	for (size_t i = 0; i<m_XScaleChanged.size(); i++)
		m_XScaleChanged[i] = false;

};
void ChartProp_AxisScaleValues::ResetZScale() {
	for (size_t i = 0; i<m_ZScaleSet.size(); i++)
		m_ZScaleSet[i] = false;
};

void ChartProp_AxisScaleValues::ResetYLabel1(int index) {
	while (m_YLabel1Change.size() <= index) m_YLabel1Change.push_back(false);
	m_YLabel1Change[index] = false;
};
void ChartProp_AxisScaleValues::ResetYLabel2(int index) {
	while (m_YLabel2Change.size() <= index) m_YLabel2Change.push_back(false);
	m_YLabel2Change[index] = false;
};
void ChartProp_AxisScaleValues::ResetXLabel(int index) {
	while (m_XLabelChange.size() <= index) m_XLabelChange.push_back(false);
	m_XLabelChange[index] = false;
};
void ChartProp_AxisScaleValues::ResetZLabel(int index) {
	while (m_ZLabelChange.size() <= index) m_ZLabelChange.push_back(false);
	m_ZLabelChange[index] = false;
};
bool ChartProp_AxisScaleValues::SetLegend1(int index, string txt, bool changeset)
{

	while (m_Legend1.size() <= index) m_Legend1.push_back(txt);
	while (m_Legend1Change.size() <= index) m_Legend1Change.push_back(false);
	m_Legend1[index] = txt;
	if (changeset) {
		m_Legend1Change[index] = true;
		m_LegendChangeAny = true;
	}
	return m_Legend1Change[index];
}
void ChartProp_AxisScaleValues::SetXVariableNumber(int subchart, int value)
{
	while (m_XVariableNumber.size() <= subchart) m_XVariableNumber.push_back(value);
	m_XVariableNumber[subchart]=value;
}
size_t ChartProp_AxisScaleValues::GetXVariableNumber(int subchart) 
{
	if(m_XVariableNumber.size()<=subchart)
		return string::npos;
	else
		return m_XVariableNumber.at(subchart);
}
void ChartProp_AxisScaleValues::SetXLabel(int index, string str)
{
	m_XLabel[index]=str;
}
void ChartProp_AxisScaleValues::SetZLabel(int index, string str)
{

	m_ZLabel[index]=str;
}
void ChartProp_AxisScaleValues::SetYLabel1(int index, string str)
{
	m_YLabel1[index]=str;
}
void ChartProp_AxisScaleValues::AddYUnit1(int index, string str)
{
	string txt;
	txt=m_YLabel1.at(index);
	txt+=" ("+str+")";
	m_YLabel1[index]= txt;
}
void ChartProp_AxisScaleValues::SetYLabel2(int index, string str)
{
	m_YLabel2[index]=str;
}
bool ChartProp_AxisScaleValues::isYLabel1Changed(int index)
{
	while (m_YLabel1Change.size() <= index) m_YLabel1Change.push_back(false);
		return m_YLabel1Change.at(index);
}
bool ChartProp_AxisScaleValues::isYLabel2Changed(int index)
{
	while (m_YLabel2Change.size() <= index) m_YLabel2Change.push_back(false);
		return m_YLabel2Change.at(index);
}
bool ChartProp_AxisScaleValues::isXLabelChanged(int index)
{
	while (m_XLabelChange.size() <= index) m_XLabelChange.push_back(false);
		return m_XLabelChange.at(index);

}
bool ChartProp_AxisScaleValues::isZLabelChanged(int index)
{
	while (m_ZLabelChange.size() <= index) m_ZLabelChange.push_back(false);
		return m_ZLabelChange.at(index);
}
string ChartProp_AxisScaleValues::GetYLabel1(int index)
{
	return m_YLabel1.at(index);
}
string ChartProp_AxisScaleValues::GetYLabel2(int index)
{
	return m_YLabel2.at(index);
}
string ChartProp_AxisScaleValues::GetXLabel(int index)
{
	return m_XLabel.at(index);
}
string ChartProp_AxisScaleValues::GetZLabel(int index)
{
	if (index >= m_ZLabel.size()) return "";
	return m_ZLabel.at(index);
}

void ChartProp_AxisScaleValues::ResetSerie1(bool ResetAll)
{
	if(m_Legend1Change.size()>0&&m_LegendChangeAny==true&&!ResetAll) {
		
	}
	else {
		m_Legend1.clear();
		m_Legend1Change.clear();
	}
}
void ChartProp_AxisScaleValues::ResetSerie2(bool ResetAll)
{
	if(m_Legend2Change.size()>0&&m_Legend2Change[0]==true&&!ResetAll) {}
		
	else {
		m_Legend2.clear();
		m_Legend2Change.clear();
	}
}
bool ChartProp_AxisScaleValues::AddLegend1(string str, int layer)
{
	m_Legend1.push_back(str);
	m_Legend1Change.push_back(0);


	return true;
}
bool ChartProp_AxisScaleValues::AddLegend2(string str)
{
	m_Legend2.push_back(str);
	m_Legend1Change.push_back(false);
	return true;
}
bool ChartProp_AxisScaleValues::isYLegendChanged(int index)
{
	while (m_Legend1Change.size() <= index) m_Legend1Change.push_back(false);
		return m_Legend1Change.at(index);
}



bool ChartProp_AxisScaleValues::isYLegend2Changed(int index)
{
	if(m_Legend2Change.at(index)==0)
		return false;
	else
		return true;

}
size_t ChartProp_AxisScaleValues::GetNum()
{
	return m_Legend1.size();
}

string ChartProp_AxisScaleValues::GetLegend(int index)
{
	if(m_Legend1.size()>index)
		return m_Legend1.at(index);
	else
		return "";
}


bool ChartProp_AxisScaleValues::SetLegend2(int index, string txt)
{
	if(m_Legend2.size()>index&&index>=0) {
		m_Legend2[index]= txt;
		m_Legend2Change[index]=true;
		return true;
	}
	else
		return false;
}
void ChartProp_AxisScaleValues::SetVerticalLegends(bool value)
	{m_VerticalLegends=value;}

bool ChartProp_AxisScaleValues::SetFreeze()
{
	m_YScaleLeftChanged[0]=true;
	m_freeze=true;
	return true;
}
void ChartProp_AxisScaleValues::ChangeYLabel1(size_t Index, string str) {
	if (Index == string::npos) {
		for (size_t i = 0; i < m_YLabel1.size(); i++) {
			m_YLabel1[i] = str;
			m_YLabel1Change[i] = true;
		}
	}
	else {
		m_YLabel1[Index] = str;
		m_YLabel1Change[Index]=true;
	}
}
void ChartProp_AxisScaleValues::ChangeYMin1(size_t Index, float value) {
	if (Index == string::npos) {
		for (size_t i = 0; i <min(m_YScaleLeftMin.size(), m_YScaleLeftChanged.size()); i++) {
			m_YScaleLeftMin[i] = value;
			m_YScaleLeftChanged[i] = true;
		}
	}
	else if(Index<min(m_YScaleLeftMin.size(), m_YScaleLeftChanged.size())) {
		m_YScaleLeftMin[Index] = value;
		m_YScaleLeftChanged[Index] = true;
	}
}
void ChartProp_AxisScaleValues::ChangeYMax1(size_t Index, float value) {
	if (Index == string::npos) {
		for (size_t i = 0; i <min(m_YScaleLeftMax.size(), m_YScaleLeftChanged.size()); i++) {
			m_YScaleLeftMax[i] = value;
			m_YScaleLeftChanged[i] = true;
		}
	}
	else if (Index<m_YScaleLeftMax.size()) {
		m_YScaleLeftMax[Index] = value;
		m_YScaleLeftChanged[Index] = true;
	}
}
void ChartProp_AxisScaleValues::ChangeYInterv1(size_t Index, float value) {
	if (Index == string::npos) {
		for (size_t i = 0; i < min(m_YScaleLeftInterv.size(), m_YScaleLeftChanged.size()); i++) {
			m_YScaleLeftInterv[i] = value;
			m_YScaleLeftChanged[i] = true;
		}
	}
	else {
		m_YScaleLeftInterv[Index] = value;
		m_YScaleLeftChanged[Index] = true;
	}
}
bool ChartProp_AxisScaleValues::Open()
{

	m_freeze = false;
	return true;
}
bool ChartProp_AxisScaleValues::MakeLabels()
{
	if(m_RemoveAxisLabel)
		return false;
	else
		return true;
}
