#pragma once



#include "MultiStorageStatData.h"


class SelectedAddEnsembles : public MultiStorageStatData
{
public:
	SelectedAddEnsembles();
	virtual ~SelectedAddEnsembles();
	bool SetNewPerformanceVector(size_t, std::vector<float> VarValidVector, bool SaveToFile=true);
	bool AddNewEnsemble(bool KeepPreviousCriteria=false);
	bool ChangeEnsembleNames(size_t index, std::string newname);
	bool RemoveEnsemble(size_t index);

protected:



};


