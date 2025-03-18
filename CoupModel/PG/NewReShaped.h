#pragma once
class NewReShaped{ 
public:
	NewReShaped();
	~NewReShaped();

	CPG* CreateNewReShapedFile(CPG* pPG,  vector<string> unique_names, vector<size_t> num_repof_names, vector<vector<size_t>> VarNamePos, bool treatment, bool specialtreatment, bool parcNo);
	CPG* CreateReshaped(CPG* pPG);
	CPG* CreateRepitionsStructure(CPG* pPG_FirstStep, CPG* pPG_OrigSource, vector<vector<size_t>> VarNamePos);

	int GetTreatment(string idkod);
	int GetParcell(string idkod);
	int GetSpecTreatment(string idkod);
	string m_DestFileName;
	vector<pair<string, int>> treatment_convert;
	vector<size_t> P_Cutting30;
	vector<size_t> P_Cutting60;
	vector<size_t> P_Cutting100;
	vector<string> Parcell_Stop;
	vector<size_t> Parcell_Add;

	bool m_RepetitionOutputStyle{ true };

protected:
	size_t m_NewIdVar;
	CPG* m_pPGSource;
	

};

