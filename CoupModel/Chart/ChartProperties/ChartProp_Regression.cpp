#include "./ChartProp_Regression.h"


ChartProp_Regression::ChartProp_Regression(void)
{

}


ChartProp_Regression::~ChartProp_Regression(void)
{
	
}


void ChartProp_Regression::SetRegA0(size_t index, float value) { 
	while (m_RegA0.size() <= index) m_RegA0.push_back(value);
	m_RegA0[index] = value; };
void ChartProp_Regression::SetRegA1(size_t index, float value) { 
	while (m_RegA1.size() <= index) m_RegA1.push_back(value);
	m_RegA1[index] = value; };
void ChartProp_Regression::SetRegR(size_t index, float value) {
	while (m_RegR.size() <= index) m_RegR.push_back(value);	m_RegR[index] = value; };

float ChartProp_Regression::GetRegA0(size_t index) { 
	if(m_RegA0.size()>index) return m_RegA0[index];
	else return MISSING;
};
float ChartProp_Regression::GetRegA1(size_t index) { 
	if(m_RegA1.size()>index) return m_RegA1[index];
	else return MISSING;
};
float ChartProp_Regression::GetRegR(size_t index) { 
	if(m_RegR.size()> index) 
		return m_RegR[index]; 
	else return MISSING;
};
