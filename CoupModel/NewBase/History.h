#pragma once

#include "./ModelFiles.h"


class History :public ModelFiles {
	
public:
	History();
	bool History_Add(SimB *,int Type, int LocalIndex, time_t t, float value, std::string User="");
	bool History_Add(SimB *,int Type, int LocalIndex, time_t t, std::string str, std::string User="");
	bool History_Add(SimB* pSimB, int LocalIndex, time_t t, float value, std::string User="");
	bool History_Add(SimB* pSimB, int LocalIndex, time_t t, double value, std::string User = "") {return History_Add( pSimB, LocalIndex, t, float(value), User); };
	bool History_Add(SimB *, int LocalIndex, time_t t, std::string str, std::string User="");
	bool History_Add(size_t RunInfoIndex, std::string str);
	void History_Add(std::string, std::string, std::string, std::string, int, time_t, std::string, std::string);
	void History_NewAdd(size_t RunInfoType, std::string, std::string, std::string, std::string, int, time_t, std::string, std::string);


	size_t History_GetNumber();
	bool History_RemoveAll();
	bool History_Remove(size_t index);
    SimB* History_GetPointer(size_t index);
	time_t History_GetTime(size_t index);
	size_t History_GetRunInfoType(size_t index);
	size_t History_GetEnumType(size_t index);
	std::string History_GetUser(size_t index);
	std::string History_GetComputer(size_t index);
	std::string History_GetString(size_t index);
	float History_GetFloat(size_t index);
	size_t History_GetLocalIndex(size_t index);
	size_t History_GetRunNo(size_t index);
	size_t UpdateListToView();
	bool   m_ToView[7],	m_ViewAll;
	std::vector<size_t> m_IndexToView;

protected:

	
};
