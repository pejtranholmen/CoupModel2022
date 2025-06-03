#pragma once
#include "OutSingle.h"	
class Ts :public OutSingle {

public:
	Ts(double *target, enum datatype data, enum elements elem, enum fysprocess fproc, enum bioprocess bproc, enum userlevel ulev);

	~Ts();

	void Def(size_t module_no, std::string varname, double initv, std::string unit, std::string conditions);  

protected:


};