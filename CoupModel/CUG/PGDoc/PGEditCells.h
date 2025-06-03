#include ".\PGButtonEvents.h"

class PGEditCells:	public PGButtonEvents
{

public:

	PGEditCells();
	~PGEditCells();


protected:
	bool AllEditCells(int col, long row,  std::string txt);

	void ChangeDescriptions(int col, long row, std::string txt, CPG *pPG);



};