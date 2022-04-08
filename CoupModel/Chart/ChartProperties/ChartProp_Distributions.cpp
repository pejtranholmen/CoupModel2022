#include "./ChartProp_Distributions.h"


ChartProp_Distributions::ChartProp_Distributions(void)
{

}


ChartProp_Distributions::~ChartProp_Distributions(void)
{
	
}
float ChartProp_Distributions::GetXDistMin(size_t Index) {
	if (Index<m_XDistMin.size()) return m_XDistMin[Index]; else return float(MISSING); 
};
float ChartProp_Distributions::GetCommonXDistMin() {
	float min = 1.E38;
	for (size_t i = 0; i < m_XDistMin.size(); i++) {
		if (min > m_XDistMin[i]) min = m_XDistMin[i];
	}
	return min;
};
float ChartProp_Distributions::GetCommonXDistMax() {
	float max = -1.E38;
	for (size_t i = 0; i < m_XDistMax.size(); i++) {
		if (max < m_XDistMax[i]) max = m_XDistMax[i];
	}
	return max;
	
};
float ChartProp_Distributions::GetXDistMax(size_t Index) { 
	if (Index<m_XDistMax.size()) return m_XDistMax[Index]; else return float(MISSING); };
bool ChartProp_Distributions::isXDistLog(size_t index) { if (index<m_XDistLog.size()) return m_XDistLog[index]; else return false; };
bool ChartProp_Distributions::isXDistSet(size_t index) { if (m_XDistSet.size()>index) return m_XDistSet[index]; else return false; }
void ChartProp_Distributions::ResetXDistSet(size_t index)
{
	while (m_XDistSet.size() <= index) m_XDistSet.push_back(false);
	m_XDistSet[index] = false;
};
int ChartProp_Distributions::GetXDistSteps(size_t index) { if (index<m_XDistSteps.size())	return m_XDistSteps[index]; else return 0; };

bool ChartProp_Distributions::isXDistChanged(size_t index) { 
	while (m_XDistChanged.size() <= index) m_XDistChanged.push_back(false);
	return m_XDistChanged[index]; };
void ChartProp_Distributions::SetXDistChanged(size_t index, bool value) { 
	while (m_XDistChanged.size() <= index) m_XDistChanged.push_back(value);
	m_XDistChanged[index] = value; };

void ChartProp_Distributions::SetXDistMin(size_t index, float v) {
	if (index<m_XDistMin.size()) {
		m_XDistMin[index] = v;
		m_XDistSet[index] = true;
	}
};
void ChartProp_Distributions::SetXDistMax(size_t index, float v) {
	if (index<m_XDistMax.size()) {
		m_XDistMax[index] = v;
		m_XDistSet[index] = true;
	}
};
bool ChartProp_Distributions::SetXDistLog(int index, bool value)
{
	if (m_XDistLog.size()>index&&index >= 0)
		m_XDistLog[index] = value;
	else
		return false;
	return false;
}
bool ChartProp_Distributions::SetXDistSteps(int index, int value)
{
	if (m_XDistSteps.size()>index&&index >= 0)
		m_XDistSteps[index] = value;
	else
		return false;
	return false;
}

void ChartProp_Distributions::ClearAndResetValuesForDistributionToChartVariables(size_t index, size_t Numvar)
{
	m_XDistMin.clear();
	m_XDistMax.clear();
	m_XDistChanged.clear();
	m_XDistLog.clear();
	m_XDistSteps.clear();
	m_XDistSet.clear();

	for (size_t i = 0; i<Numvar; i++) {
		float pv;
		pv = -float(MISSING);
		m_XDistMin.push_back(pv);
		pv = float(MISSING);
		m_XDistMax.push_back(pv);
		m_XDistChanged.push_back(false);
		m_XDistLog.push_back(false);
		m_XDistSteps.push_back(0);
		m_XDistSet.push_back(false);
	}

}

