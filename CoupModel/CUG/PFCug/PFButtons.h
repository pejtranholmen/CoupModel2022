#pragma once
#include ".\PFCommands.h"

class PFButtons : public PFCommands
{

public:

	PFButtons();
	~PFButtons();
	void DefineButtonInfos();
	void DefineListButtonInfos();
	void DefineClickButtonInfos();
	void DefineHintList();
	void DefineComboList();
	void DefineCheckBoxButtons();
private:

};