#pragma once
class MakeSimCharts;
class MakeDocInfoSheet
{
public:
	MakeDocInfoSheet(MakeSimCharts& simchart);
	~MakeDocInfoSheet(void);
public:
	MakeSimCharts& m_pUG;
	void MakeSheet();
};

