#pragma once

#include "./Structure.h"
#include "./Additional_BioticVariables.h"
#include "../SOIL/Soil_Frost.h"
#include "../../NewBase/ModelMap.h"	


class StructureModel	:public GlobalInfo
	{
	public:
		StructureModel(void);
		~StructureModel(void);
		bool Def(ModelInfo*, ModelMap*);
		bool Ini(bool ResetOnlyAdditional=false);
		bool IniFunctors();
		bool Dyn(bool withAdditionalVariables=true);
		bool End();
		Structure m_Structure;
		Additional_BioticVariables m_Additional;
	

		
	};

