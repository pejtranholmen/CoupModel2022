#pragma once
class NewReShaped{ 
public:
	NewReShaped();
	~NewReShaped();

	bool CreateNewReShapedFile(CPG* pPG,  vector<string> unique_names, vector<size_t> num_repof_names, vector<vector<size_t>> VarNamePos, bool treatment, bool specialtreatment, bool parcNo);

	int GetTreatment(string idkod);
	int GetParcell(string idkod);
	int GetSpecTreatment(string idkod);
	string m_DestFileName;

protected:

	CPG* m_pPGSource;

};

