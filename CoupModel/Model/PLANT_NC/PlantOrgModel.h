#pragma once

#include "./NC_Drivings.h"
#include "./NC_Plant.h"
#include "../../NewBase/ModelMap.h"	


class PlantOrgModel	:public GlobalInfo
	{
	public:
		PlantOrgModel(void);
		~PlantOrgModel(void);
		bool Def(ModelInfo*, ModelMap*);
		bool Ini();
		bool Dyn();
		bool End();
		NC_Plant m_NC_Plant;

		
	};

