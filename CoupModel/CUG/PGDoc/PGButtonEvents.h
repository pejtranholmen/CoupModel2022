#include ".\PGCommands.h"

class PGButtonEvents:	public PGCommands
{

public:

	PGButtonEvents();
	~PGButtonEvents();


	void AllSheetButtons(int sheet, int parv, int index_col=-1, int col=0);
	void DataSheetButtons(int parv);
	void ImportSheetButtons(int parv, int index_col=0);
	bool ImportListButtons(int parv, int index_col, int col=0);
	bool DescriptionListButtons(int parv, int index_choice, int col, int row);
	void DescriptionButtons(int parv );
	void ApplyUnitTypeToUnit(bool CleanInstead=false);
	void ApplyElementTypeToName(bool CleanInstead = false);
private:
	size_t m_PreviousStart;
	size_t m_PreviousStop;

};