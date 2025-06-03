/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*=-=-=-=-=-                    CLASS: CSW                     -=-=-=-=-=*/
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
#include "Sw.h"
//Destructor
Sw::~Sw()
{

}
Sw::Sw(size_t *target,enum datatype data, enum elements elem, enum fysprocess fproc, enum bioprocess bproc, enum userlevel ulev)
	:SimB(SWITCH,data, elem, fproc, bproc, ulev)
{
		_ptarget=target; 
		org_value = old_value = m_recalc = 0;
		_IsOldValue = false;
	;
}

void Sw::Def(size_t ModuleNo,  std::string name, int def, std::string opt, std::string conditions, int recalc)
{
	org_value=def;
	_orgIntValue = def;
	*_ptarget=def;
	_options.clear();
	SetName(name);
	SetModuleNo(ModuleNo);
	SetConditions(conditions);
	_IsOldValue=false;
	SetIsOriginalValue();

	string		option;
	if (opt.size()<=0) return;
	
	while(opt.size()>0) {
		auto npos=opt.find('|');
		if(npos!= std::string::npos) {
			option=opt.substr(0,npos);
			opt=opt.substr(npos+1);
		}
		else {
			option=opt;
			opt="";
		}
		_options.push_back(option);
	}
	m_recalc=recalc;

}
size_t Sw::SetStrValue(string str) {

	for(size_t i=0; i<_options.size(); i++) {
		if(str==_options[i]) {
			return SetIntValue(i);
		}
	}
	return 0;
}

size_t Sw::SetDBStrValue(string str)
{
	for (size_t i = 0; i<_options.size(); i++) {
		if (str == _options[i]) {
			 SetDBIntValue(i);
			 return _DBIntValue;
		}
	}
	return 0;

}
size_t Sw::GetNumOptions() {
	return _options.size();
}


