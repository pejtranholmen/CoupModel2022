#pragma once
#include "GridPGDoc.h"
#include "..\MakePGCharts\MakePGCharts.h"

class PGCalcActions :
	public GridPGDoc
{
public:
	PGCalcActions(void);

	~PGCalcActions(void);


protected:
	void Make_PG_Calc(bool newvar, bool allrep, size_t seqno, size_t varin = 0, size_t repin = 0);
	BOOL OnNewCalculateVar();
	 
private:
	
};

