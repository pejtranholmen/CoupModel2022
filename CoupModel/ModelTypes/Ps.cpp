#include "../std.h"
#include "Ps.h"


Ps::Ps(double *target,definitions def) :Par(def)
{
	_ptarget=target;
	_recalc_option=0;
}

Ps::Ps(double *target,enum datatype data, enum elements elem, enum fysprocess fproc, enum bioprocess bproc, enum userlevel ulev)
	:Par(data, elem, fproc, bproc, ulev)
{
	_ptarget=target;
	SetSimType(simtype::PAR_SINGLE);
	if(fproc==fysprocess::LOWERBOUNDARY)
		_Process="Lower Boundary";
	else
		_Process="";

}
void Ps::Def(size_t ModuleNo, std::string name, double init, std::string unit, std::string conditions, 
			double errormin, double errormax, double warningmin, double warningmax,
			int recalc, int view)
{
SetViewFunction(0);
SetModuleNo(ModuleNo);
SetConditions(conditions);
SetReCalcOption(recalc);
SetUnit(unit);
SetOriginalValue(init);
SetValue(init);

SetIsOriginalValue();
SetName(name);
SetMinValue(errormin); SetMaxValue(errormax); SetWarnMinValue(warningmin); SetWarnMaxValue(warningmax); 
_IsOldValue=false;
_ViewTitle="";
SetSelectedFunc(0);
_mr.Min=_mr.Max=MISSING;
_DBvalue = MISSING;
_mr.MR_Dependent.pBase=nullptr;

}

Ps::~Ps(void)
{
}
					   
