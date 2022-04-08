#pragma once
#include "../../std.h"




class ChartProp_Regression
{
public:
	ChartProp_Regression(void);
	~ChartProp_Regression(void);

	bool IsRegLineEstimated(size_t index) { if (m_RegA0.size() > index&&m_RegA1.size() > index&&m_RegR.size() > index) return true; else return false; };
	void SetRegA0(size_t index, float value) ; 
	void SetRegA1(size_t index, float value) ; 
	void SetRegR(size_t index, float value) ; 
	float GetRegA0(size_t index) ;
	float GetRegA1(size_t index);
	float GetRegR(size_t index);
private:
	vector<float> m_RegA0, m_RegA1, m_RegR;
	
};

