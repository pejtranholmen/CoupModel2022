#pragma once
class NewReShaped{ 
public:
	NewReShaped();
	~NewReShaped();

	CPG* CreateNewReShapedFile(CPG* pPG,  vector<string> unique_names, vector<size_t> num_repof_names, vector<vector<size_t>> VarNamePos, bool treatment, bool specialtreatment, bool parcNo);
	CPG* CreateReshaped(CPG* pPG);
	CPG* CreateRepitionsStructure(CPG*);

	int GetTreatment(string idkod);
	int GetParcell(string idkod);
	int GetSpecTreatment(string idkod);
	string m_DestFileName;
	vector<pair<string, int>> treatment_convert;
	vector<string> Parcell_Stop;
	vector<size_t> Parcell_Add;
	bool m_RepetitionOutputStyle{ true };

protected:

	CPG* m_pPGSource;
	

};

