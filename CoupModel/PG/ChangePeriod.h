#pragma once
#include "../std.h"
#include "PG.H"


class ChangePeriod
{
public:
	ChangePeriod(void) {m_StartOut=0; m_EndOut=0;};
	~ChangePeriod(void) {;};
	bool CreateNewFile(CPG *pPG, int option);
	std::string m_DestFileName;
	std::vector<std::string> m_ResultSum;

protected:


	CPG *m_pPGSource;

public:
	long m_StartOut, m_EndOut;

}
;
