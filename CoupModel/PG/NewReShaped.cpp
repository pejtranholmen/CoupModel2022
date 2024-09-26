#include "../../StdAfx.h"
#include "../../Std.h"
#include "../../Util.h"
#include "NewReShaped.h"

NewReShaped::NewReShaped()
{
}

NewReShaped::~NewReShaped()
{
}

bool NewReShaped::CreateNewReShapedFile(CPG* pPG, int option)
{
	CPG* pPG_Dest;
	pPG_Dest = new CPG;

	bool fail = true;
	if (pPG == nullptr && m_pPGSource == nullptr) return nullptr;
	else if (pPG != nullptr)
		m_pPGSource = pPG;


	return false;
}
