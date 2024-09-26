#pragma once
class NewReShaped{ 
public:
	NewReShaped();
	~NewReShaped();

	bool CreateNewReShapedFile(CPG* pPG, int option);
	
	string m_DestFileName;

protected:

	CPG* m_pPGSource;

};

