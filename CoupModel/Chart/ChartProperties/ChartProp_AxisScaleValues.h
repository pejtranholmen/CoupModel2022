#pragma once
#include "../../std.h"




class ChartProp_AxisScaleValues
{
public:
	ChartProp_AxisScaleValues(void);
	~ChartProp_AxisScaleValues(void);

	bool Reset_Selected_Or_All_BasicVector(size_t NumVariables=0, size_t NumSubCharts=1, bool OnlyNumElements=false);
	
	bool isLeftYScaleSet(int index);
	bool isRightYScaleSet(int index);

	bool isLeftYScaleChanged(int index);

	
	bool isRightYScaleChanged(int index);

	void ResetYLeftScale(size_t index);
	void ResetYRightScale(size_t index);
	void ResetXScale(size_t index);
	void ResetZScale(size_t index);

	bool isXScaleChanged(int index);
	void SetXScaleChanged(size_t index) ;

	bool isZScaleChanged(int index);
	void SetZScaleChanged(int index) ;
	bool isZScaleSet(int index);

	void SetAllXScale_toSameValues();



	bool isXScaleSet(int index) ;
	void ResetYLeftScale() ;
	void ResetYRightScale() ;
	void ResetXScale();
	void ResetZScale() ;

	float GetXmin(int Index );
	float GetXmax(int Index);
	float GetXInterv(int Index);
	
	float GetZmin(int Index);
	float GetZmax(int Index);
	float GetZInterv(int Index);



	float GetYLeftmin(int Index ) ;
	float GetYRightmin(int Index);
	float GetYLeftmax(int Index);
	float GetYRightmax(int Index);
	float GetYLeftInterv(int Index);
	float GetYRightInterv(int Index);

	void SetXmin(size_t index, float v);
	void SetXmax(size_t index,float v);
	void SetXInterv(size_t index,float v);

	void SetZmin(int index,float v);
	void SetZmax(int index,float v);
	void SetZInterv(int index,float v);

	void ResetScales() {
		for (size_t i = 0; i < m_YScaleLeftSet.size(); i++)
			m_YScaleLeftSet[i] = false;
		for (size_t i = 0; i < m_ZScaleSet.size(); i++)
			m_ZScaleSet[i] = false;
		for (size_t i = 0; i < m_XScaleSet.size(); i++)
			m_XScaleSet[i] = false;
	};


	void SetYLeftmin(int index, float v);
	void SetYLeftmax(int index, float v);
	void SetYLeftInterv(int index, float v);
	void SetYRightmin(int index, float v);
	void SetYRightmax(int index, float v);
	void SetYRightInterv(int index, float v);


	bool isYLeftScaleSet(int index) { return m_YScaleLeftSet[index]; };

	void ResetLegend1(size_t index);

	void SetXVariableNumber(int subchart, int value);
	size_t GetXVariableNumber(int subchart);

	bool isYRightScaleSet(int index) { return m_YScaleRightSet[index]; };
	void ResetYLabel1(int index);
	void ResetYLabel2(int index);
	void ResetXLabel(int index);
	void ResetZLabel(int index);
	bool SetLegend1(int index, std::string text, bool changeset = false);

	void SetYLabel1(int index, std::string);
	void AddYUnit1(int index, std::string);
	void SetYLabel2(int index, std::string);
	std::string GetYLabel1(int index);
	std::string GetYLabel2(int index);
	std::string GetLegend(int index);
	void SetLengend1(size_t index, std::string str, bool changeset=false);
	void SetYLabel1Changed(int index) {m_YLabel1Change.at(index)=true;};
	void SetYLabel2Changed(int index) {m_YLabel2Change.at(index)=true;};
	void SetXLabelChanged(int index) {m_XLabelChange.at(index)=true;};
	void SetZLabelChanged(int index) {m_ZLabelChange.at(index)=true;};
	void SetLeftYScaleChanged(int index) {m_YScaleLeftChanged.at(index)=true;};
	void SetRightYScaleChanged(int index) {m_YScaleRightChanged.at(index)=true;};
	void ResetSerie1(bool ClearAll=false);
	void ResetSerie2(bool ClearAll=false);
	bool AddLegend1(std::string, int layer=1);
	bool AddLegend2(std::string);

	bool SetLegend2(int index, std::string txt);
	bool Open();

void SetVerticalLegends(bool value) ;
	bool GetVerticalLegends() {return m_VerticalLegends;};
	void SetXLabel(int index, std::string str);
	void SetZLabel(int index, std::string str);
	std::string GetZLabel(int index);
	bool isZLabelChanged(int index);

	bool isXLabelChanged(int index);
	std::string GetXLabel(int index);

	bool MakeLabels();
	bool SetFreeze();

	size_t GetNum();

	bool isYLabel1Changed(int index);
	bool isYLabel2Changed(int index);
	bool isYLegendChanged(int index);
	bool isYLegend2Changed(int index);

	void ChangeYLabel1(size_t index, std::string str);
	void ChangeYMin1(size_t index, float value);
	void ChangeYMax1(size_t index, float value);
	void ChangeYInterv1(size_t index, float value);
	void SetRemoveLabels(bool value) {m_RemoveAxisLabel=value;};
	std::string GetLegend(size_t index) { if (m_Legend1.size() > index) return m_Legend1[index]; return ""; };
	std::string GetLastLegend() {if (m_Legend1.size() > 0) return m_Legend1[m_Legend1.size() - 1]; else return "";	};
	bool m_RemoveAxisLabel;
	bool m_freeze;


	std::vector<float> m_YScaleRightMin, m_YScaleRightMax, m_YScaleRightInterv;
	std::vector<float> m_XScaleMin, m_XScaleMax, m_XScaleInterv;
	std::vector<float> m_ZScaleMin, m_ZScaleMax, m_ZScaleInterv;




	std::vector<float> m_YScaleLeftMin, m_YScaleLeftMax, m_YScaleLeftInterv;
	std::vector<bool> m_YScaleLeftChanged, m_YScaleLeftSet;
	std::vector<bool>  m_YScaleRightChanged, m_YScaleRightSet;
	std::vector<bool>  m_XScaleChanged, m_XScaleSet, m_ZScaleChanged, m_ZScaleSet;
	std::vector<bool>  m_YLabel1Change, m_YLabel2Change, m_XLabelChange, m_ZLabelChange;
	std::vector<size_t>  m_XVariableNumber;

	std::vector<std::string> m_YLabel1, m_YLabel2, m_XLabel, m_ZLabel;
	std::vector<std::string> m_Legend1, m_Legend2;

	std::vector<bool> m_Legend1Change, m_Legend2Change;
	bool m_LegendChangeAny, m_VerticalLegends;

	bool m_CommonXScale;



private:






	std::string m_LabelY, m_LabelX;
};

