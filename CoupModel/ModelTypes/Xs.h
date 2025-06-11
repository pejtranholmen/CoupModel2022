#pragma once
#include "OutSingle.h"	
class Xs :public OutSingle {

public:
	Xs(double *target, enum datatype data, enum elements elem, enum fysprocess fproc, enum bioprocess bproc, enum userlevel ulev);

	~Xs();

	void Def(size_t module_no, std::string varname, double initv, std::string_view unit, std::string conditions);  

protected:


};