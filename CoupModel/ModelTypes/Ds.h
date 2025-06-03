#pragma once
#include "OutSingle.h"	
class Ds :public OutSingle {

public:
	Ds(double *target, enum datatype data, enum elements elem, enum fysprocess fproc, enum bioprocess bproc, enum userlevel ulev);

	~Ds();

	void Def(int module_no, std::string varname, double initv, std::string unit, std::string conditions);  

protected:


};