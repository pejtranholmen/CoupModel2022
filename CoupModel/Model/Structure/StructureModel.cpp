#include "StructureModel.h"



StructureModel::StructureModel(void)
{
}
bool StructureModel::Def(ModelInfo* modelptr, ModelMap* mapptr)
{	
  //	m_Structure.SetPtr2Module(this, modelptr, mapptr);
//	m_Structure.Def();
	//m_Additional.SetPtr2Module(this, modelptr, mapptr);
	//m_Additional.Def();
	return true;
}
bool StructureModel::Ini(bool OnlyResetAdditional)
{
	if (OnlyResetAdditional) {
		m_Additional.Ini();
	}
	else {
		if (!m_Structure.Ini()) return false;
		m_Additional.Ini();
	}
	return true;
}
bool StructureModel::IniFunctors()
{
	if (!m_Structure.IniFunctors()) return false;

	//m_Additional.IniFunctors();
	return true;
}
bool StructureModel::Dyn(bool WithAdditionals)
{
	 if(!m_Structure.Dyn()) return false;
	 if (WithAdditionals)  if (!m_Additional.Dyn()) return false;
	 return true;
}

bool StructureModel::End()
{
	m_Structure.End();
	return true;
}

StructureModel::~StructureModel(void)
{
}
