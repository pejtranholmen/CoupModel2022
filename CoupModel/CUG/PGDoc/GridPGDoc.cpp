#include "..\..\StdAfx.h"
#include "..\..\Std.h"
#include ".\GridPGDoc.h"

#include "..\..\Util.h"
#include "..\..\Chart\ChartProperties\ChartProp.h"
#include "..\..\Chart\ChartProperties\ChartUtil.h"
#include "..\SimDoc\MyCug.h"
#include "..\..\PG\PGfileArchive.h"
#include "..\..\NewBase\Doc.h"






//#include "afxole.h"

#include "..\..\NewBase\SimArchive.h"

GridPGDoc::GridPGDoc(void)
{
	m_pPGArchive=MySingle::instance().GetPGFileArchivePointer();
	m_pProp=MySingle::instance().GetChartPropPointer();
	m_pUtil=MySingle::instance().GetChartUtilPointer();
	m_pPG_ScalingSource=nullptr;
	m_ShowCalculating=false;
	m_NewResolutionSteps=0;
	m_pDoc = nullptr;
	m_pSimDoc = nullptr;
	m_col = 1;
	m_row = 1;
	m_pPG = nullptr;
	m_AddMix_Actions.m_pPGADD_Out = nullptr;
	m_AddMix_Actions.m_pPGMix_Out = nullptr;
	m_AddMix_Actions.m_pPGMerge_Out = nullptr;
	m_pPGNewGrid = nullptr;
	m_Changed_Data = false;
	m_Changed_Description = false;
	DefineButtonInfos();
	DefineListButtonInfos();
	DefineClickButtonInfos();
	DefineHintList();
	DefineComboList();
	DefineCheckBoxButtons();
	NEWPGFILE_INFO def;
	def.Param = 0;
	def.MinDiff = 1440;
	def.NumRec = 30;
	def.NumVar = 5;
	def.NumRep = 1;
	def.StartDate = "2010-07-01 12:00";
	def.FileName = "NewEmpty.bin";
	def.Assigned = false;
	m_DescriptionImportedFile.param = 3211;
	m_AggregationStepSize = 1;
	
	m_NewGridInfo = def;
}


GridPGDoc::~GridPGDoc(void)
{
	if (m_pPG = nullptr) delete m_pPG;
}
BOOL GridPGDoc::ReDisplay()
{

	CPG *pCPG = m_pPG;
	CUGCell cell;
	if (pCPG != nullptr) {
		pCPG->UpdateViewOrder();
		m_col = pCPG->GetNumToView() + 1;
	}
	while (GetNumberCols()<m_col) {
		int col = GetNumberCols() - 1;
		AppendCol();
		GetColDefault(0, &cell);
		cell.SetAlignment(UG_ALIGNRIGHT);
		SetColDefault(col, &cell);
		SetColWidth(col, 70);
		QuickSetText(col, -1, "Var: " + FUtil::STD_ItoAscii(col));
		AppendRow();
		QuickSetText(-1, col - 1, FUtil::STD_ItoAscii(col));
	}
	while (GetNumberCols()>m_col) {
		int col = GetNumberCols() - 1;
		DeleteCol(col);
		DeleteRow(col - 1);
	}

	return TRUE;

}
bool GridPGDoc::Make_PGFileSheet()
{
	CUGCell cell; 
	string heading, number, test;

	string Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	string str1, Defstring;
	string optionlist;
	int NumLines;
	int MaxCol=8 ;
	int colv=0;
	float v;
	CPtrArray CPG_Array;

	
	BOOL Tableexpand;

	Tableexpand=FALSE;
	EnableMenu(TRUE); 

	SetSheetNumber(0); 
	EnableUpdate(FALSE);
	while(GetNumberRows()>0)
		DeleteRow(0);

	EnableUpdate(TRUE);

		//SetDefRowHeight(22);
		SetDefFont(&m_font);
		SetNumberCols(9); 
		SetSH_NumberCols(2);
		SetSH_Width(0);
		SetColWidth(-2,0);
		SetColWidth(-1,0);
		SetTH_Height(0);
		SetTH_NumberRows(0);


		SetColWidth(0,380);
		SetColWidth(1,70);
		SetColWidth(2,180);
		SetColWidth(3,100);
		SetColWidth(4,46);
		SetColWidth(5,110);
		SetColWidth(6,110);
		SetColWidth(7,110);
		SetColWidth(8,110);
		auto SetTopHeadingLines = [&]() {
			for (size_t i = 0; i < 12; i++) { UnJoinCells(i, 0); UnJoinCells(i, 1); UnJoinCells(i, 2); };
			int TopLine = 0;
			int NextLine = 1;
			//QuickSetBackColor(NextLine,-1,m_ColorClick);
			QuickSetHeadingText(-2, NextLine, "F No");
			QuickSetHeadingText(-1, NextLine, "V No");
			QuickSetHeadingText(0, NextLine, "FileName");
			QuickSetHeadingText(1, NextLine, "Variables");
			QuickSetHeadingText(2, NextLine, "Records");

			QuickSetHeadingText(3, TopLine, "Repetitions");
			QuickSetHeadingText(3, NextLine, "Actual");
			QuickSetHeadingText(4, NextLine, "Total");
			QuickSetHeadingText(5, TopLine, "Period");
			QuickSetHeadingText(5, NextLine, "Start Date");
			QuickSetHeadingText(6, NextLine, "End Date");
			QuickSetHeadingText(7, TopLine, "Axis Representation");
			QuickSetHeadingText(7, NextLine, "X Independent");
			QuickSetHeadingText(8, NextLine, "Y Independent");
			JoinCells(3, TopLine, 4, TopLine);
			JoinCells(1, TopLine, 2, TopLine);
			JoinCells(5, TopLine, 6, TopLine);
			JoinCells(7, TopLine, 8, TopLine);
		};
	
		Doc *pDoc =(Doc*)m_pDoc;
		if (m_pDoc == nullptr) return false;
		auto SetChartTypeOptions = [&]() {
			if (pDoc != nullptr) {
				if (pDoc->m_DocumentType == 2) {
					if (m_pProp->GetFirstIndepVarType()== vartype::MULTIRUN_NUM) {
						m_pProp->SetFirstIndepVarType(vartype::DATEAXIS);
					}
				}
				else {
					m_pProp->DocumentError(pDoc);
				}
				if (m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber) > 1 || m_pUtil->m_MultiRunPlot)
					m_ChartTypeOptions.VectorNames = m_pProp->GetChartTypeOptions(false);
				else 
					m_ChartTypeOptions.VectorNames = m_pProp->GetChartTypeOptions(true);

				m_ChartTypeOptions.Text = m_pProp->GetCurrentChartTypeOption();
				SetCellClickButton(m_ChartTypeOptions);

				m_VarTransformationCombo.Text = m_VarTransformationCombo.VectorNames[m_pProp->m_ReShape.m_Cumulated];
				SetCellClickButton(m_VarTransformationCombo);
			}
		
		};




		NumLines=m_pPGArchive->GetNumPGFile();
		int newpglines=0;
		size_t i_doc=m_pPGArchive->GetIndex(m_pDoc);
		CPG *pCPG=nullptr;
		if(i_doc!= std::string::npos) {
			pCPG=(CPG*)m_pPGArchive->GetPGFile(i_doc);
			if(pCPG!=nullptr) {
				test=pCPG->GetFileName().c_str();

				
				if(FUtil::IsFileExisting(string(test))) {
					pCPG->ReOpen();
					newpglines+=pCPG->GetNumVariables()+1;
				}
				if (newpglines == 0) return false;
			}
		}

	if(pCPG!=nullptr)	{
		if (!pCPG->CheckDescriptors()) {

			return false;

		};
		NumLines=1;
		SetNumberRows(NumLines+newpglines+2);
		SetTopHeadingLines();
		SetChartTypeOptions();
		int ind=1;

	size_t i=i_doc;
		//	CPG *pCPG;
			pCPG=(CPG*)m_pPGArchive->GetPGFile(i);
			m_pProp->SetPGSource(pCPG);
			ind++;
			Name=pCPG->GetFileName(true).c_str();
			QuickSetNumber(-1,ind,i+1);
			QuickSetNumber(-2,ind,i_doc);
		
			QuickSetBackColor(-1,ind,m_ColorClick);
			QuickSetText(2,ind, str1);
			QuickSetMask(2,ind,_T(""));
			GetCell(0,ind, &cell);			
			cell.SetParam(m_FileSelectOptions.param);
			cell.SetBackColor(RGB(240,240,50));
			cell.SetText(Name);
			SetCell(0,ind, &cell);
			//JoinCells(3,ind,4,ind);

			pCPG->ReOpen();
			long nrec=pCPG->GetNumRecords();
			int  nvar=pCPG->GetNumVariables();
			long pgst=pCPG->GetLongTimeStart();
			long pgend=pCPG->GetLongTimeEnd();
			GetCell(1,ind, &cell);
			cell.SetNumber(nvar);
			cell.SetReadOnly(true);
			SetCell(1,ind, &cell);

			//QuickSetNumber(1,ind, nvar);
			QuickSetNumber(2,ind, nrec);

			auto nrep=pCPG->GetNumRepititions();
			auto actrep=pCPG->GetRepititionSequence();
			actrep = max(0, actrep);
			m_RepOptionsCombo.Text = FUtil::STD_ItoAscii(actrep+1);
			m_RepOptionsCombo.VectorNames.clear();
			for(size_t i1=0; i1<nrep; i1++) {
				m_RepOptionsCombo.VectorNames.push_back(FUtil::STD_ItoAscii(i1+1));
			}
			SetCellClickButton(m_RepOptionsCombo);

			
			QuickSetNumber(4,ind, nrep);
			if(pgst>0) {
				QuickSetText(5,ind,PGUtil::StringDatum(pgst));
				QuickSetText(6,ind,PGUtil::StringDatum(pgend));

				m_XAxisCombo.Text = "Normal Time";
				if (m_pProp->GetIndepFirst() == vartype::PGHOUR) m_XAxisCombo.Text = "Hour";
				if (m_pProp->GetIndepFirst() == vartype::PGDAY) m_XAxisCombo.Text = "Day";
				if (m_pProp->GetIndepFirst() == vartype::PGYEAR) m_XAxisCombo.Text = "Year";
				SetCellClickButton(m_XAxisCombo); //cell=7
	
				m_YAxisCombo.Text="Not Used";
				if(m_pProp->GetIndepSecond()== vartype::PGHOUR) m_YAxisCombo.Text ="Hour";
				if(m_pProp->GetIndepSecond()== vartype::PGDAY) m_YAxisCombo.Text ="Day";
				if(m_pProp->GetIndepSecond()== vartype::PGYEAR) m_YAxisCombo.Text ="Year";
				SetCellClickButton(m_YAxisCombo);// cell 8
			}
			//cell.SetText(PGUtil::StringDatum(pgend));
			//SetCell(5,ind,&cell);
			if(m_pPGArchive->m_PGFileArray[i].Visible) {
				ind++;
				QuickSetHeadingText(-1,ind,"V No");
				QuickSetHeadingText(0,ind,"Variable Name");
				QuickSetHeadingText(1,ind,"Unit");
				QuickSetHeadingText(2,ind,"Id");
				QuickSetHeadingText(3,ind,"Pick");
				QuickSetHint(3, ind, m_VarSelectBox.param);
				QuickSetHeadingText(4,ind,"Chart");
				QuickSetHeadingText(5,ind,"MinValue");
				QuickSetHeadingText(6,ind,"MaxValue");
				QuickSetHeadingText(7, ind, "Missing");
//Check if various Day value representations make sense

			pCPG->CheckEquiDistance();
			if(pCPG->GetNormalTimeInterval()<=720&&pCPG->GetNormalTimeInterval()>0) {
			
				GetCell(8,3, &cell);
				string label=m_pProp->DefineYearWithinDayCell(&cell);
				QuickSetText(7, 3, label);
				cell.SetBackColor(m_NoColor);
				cell.SetParam(m_SelectWithinDay.param);
				SetCell(8,3,&cell);
			}
			else {
				QuickSetBackColor(8,ind,RGB(240,240,240));
				m_pProp->m_DayValueRepresentation=0;
				m_pProp->m_YearValueRepresentation=0;
			}
		auto SetVarLineInfo = [&](int ind, int j, int idoc) {
			QuickSetNumber(-1, ind, j + 1);
			QuickSetNumber(-2, ind, idoc + 1);
			GetCell(0, ind, &cell);
			cell.SetText(pCPG->GetVarName(j + 1).c_str());
			cell.SetParam(m_VarSelectOptions.param);
			SetCell(0, ind, &cell);

			GetCell(1, ind, &cell);
			cell.SetText(std::string(pCPG->GetVarUnit(j + 1)).c_str());
			cell.SetParam(m_VarSelectOptions.param);
			cell.SetBackColor(pCPG->GetColor(j + 1));
			SetCell(1, ind, &cell);
			
			GetCell(2, ind, &cell);
			cell.SetText(pCPG->GetVarId(j + 1, actrep).c_str());
			cell.SetParam(m_VarSelectOptions.param);
			cell.SetBackColor(pCPG->GetColor(j + 1));
			SetCell(2, ind, &cell);


			GetCell(3, ind, &cell);
			cell.SetCellType(UGCT_CHECKBOX);
			cell.SetBool(FALSE);

			if (pCPG->GetPicked(j + 1))
				cell.SetBool(TRUE);
			cell.SetParam(m_VarSelectBox.param);

			SetCell(3, ind, &cell);
			
			GetCell(4, ind, &cell);
			cell.SetParam(m_ChartNumberSelect.param);
			SetCell(4, ind, &cell);
			
			GetCell(5, ind, &cell);
			cell.SetParam(m_VarSelectOptions.param);
			SetCell(5, ind, &cell);
			GetCell(6, ind, &cell);
			cell.SetParam(m_VarSelectOptions.param);
			SetCell(6, ind, &cell);


			v = pCPG->GetVarMin(j + 1);
			if (v<0.5E38)	QuickSetNumber(5, ind, v);
			v = pCPG->GetVarMax(j + 1);
			if (v>-1.E38)	QuickSetNumber(6, ind, v);
			auto nmiss = pCPG->GetVarMissing(j + 1);
			QuickSetNumber(7, ind, nmiss);
			GetCell(7, ind, &cell);
			cell.SetParam(m_VarSelectOptions.param);
			SetCell(7, ind, &cell);			
			
			GetCell(8, ind, &cell);
			cell.SetParam(m_VarSelectOptions.param);
			SetCell(8, ind, &cell);
		
		
		};

				for(int j=0;j<nvar;j++) {
					ind++;
					SetVarLineInfo(ind, j, i_doc);
	
				}
				pCPG->CloseFile();
					
		}
		if(m_pProp->GetIndepFirst()== vartype::PGVAR) {


			int index=m_pProp->GetFirstIndepIndex();
			GetCell(7,index+3,&cell);
			cell.SetText("X - Indep");
			cell.SetParam(m_XAxisCombo.Param);
			cell.SetBackColor(m_ColorEdit);
			SetCell(7,index+3,&cell);
		}
		if(m_pProp->GetIndepSecond()== vartype::PGVAR) {
			int index=m_pProp->GetSecondIndepIndex();
			GetCell(8,index+3,&cell);
			cell.SetText("Y - Indep");
			cell.SetParam(m_YAxisCombo.Param);
			cell.SetBackColor(m_ColorEdit);
			SetCell(8,index+3,&cell);
		}

		MySingle::instance().GetCugPointer()->SetDocument(m_pDoc);
		//	MySingle::instance().GetCugPointer()->TimeSerieChart(11);
	}	
	else
		SetNumberRows(0); 
	
	RedrawAll();


		
	return true;
}
bool GridPGDoc::Make_Description(int ifile)
{
	CUGCell cell; 
	CString heading, number, test;

	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	CString str1, Defstring;
	CString optionlist;
	int NumLines;
	int MaxCol=9 ;
	int colv=0;
	float v;
	CPtrArray CPG_Array;
	
	BOOL Tableexpand;

	Tableexpand=FALSE;
	EnableMenu(TRUE); 

	SetSheetNumber(1); 
	EnableUpdate(FALSE);
	while(GetNumberRows()>0)
		DeleteRow(0);

	EnableUpdate(TRUE);
	CPG *pCPG;

	m_pPGArchive=MySingle::instance().GetPGFileArchivePointer();
	if(ifile>0) m_IndexSource=ifile;
	if(m_IndexSource>=m_pPGArchive->GetNumPGFile())
		m_IndexSource=0;

	if(m_pPGArchive->GetNumPGFile()<=m_IndexSource) return false;

	for (size_t i = 0; i < 12; i++) {	UnJoinCells(i, 0); UnJoinCells(i, -1);};

	pCPG=(CPG*)m_pPGArchive->GetPGFile(m_IndexSource);

		NumLines=3;
		int newpglines=0;
		int n_rep_toshow=1;
		if(pCPG->GetNumRepititions()>1) {
			if(IsMultiPGRepPlot())
				newpglines=pCPG->GetNumVariables()-1;
			else {	
				newpglines=pCPG->GetNumVariables()*pCPG->GetNumRepititions();
				n_rep_toshow=pCPG->GetNumRepititions();

			}
		}

		SetNumberRows(NumLines+newpglines); 
		
		SetDefFont(&m_font);
		SetNumberCols(10); 
		SetSH_NumberCols(2);
		SetSH_Width(70);
		SetColWidth(-2,45);
		SetColWidth(-1,25);

		SetColWidth(0,380);
		SetColWidth(1,70);
		SetColWidth(2,180);
		SetColWidth(3,46);
		SetColWidth(4,46);
		SetColWidth(5,110);
		SetColWidth(6,110);
		SetColWidth(7,90);
		SetColWidth(8,90);
		QuickSetBackColor(-1,-1,m_ColorClick);
		SetTH_Height(30);

		if(m_ShowDetails)
			QuickSetText(-1,-1,"-");
		else
			QuickSetText(-1,-1,"+");
		JoinCells(-2,0,-1,0);
		QuickSetText(-2,0,"Index");
		QuickSetText(0,-1, "FileName");
		QuickSetText(1,-1, "Variables");
		QuickSetText(2,-1, "Records");
		QuickSetText(3,-1, "Repetitions");
		QuickSetText(5,-1, "Start Date");
		QuickSetText(6,-1, "End Date");
		QuickSetText(7,-1, "X Independent");
		QuickSetText(8,-1, "Y Independent");
		QuickSetText(9,-1, "Z Dependent");
	
		JoinCells(3,-1,4,-1);
		//JoinCells(7,-1,8,-1);

		int ind=0;

		Name=pCPG->GetFileName(true).c_str();
		//QuickSetNumber(-1,ind,i+1);
		//QuickSetNumber(-2,ind,i);
		
		QuickSetBackColor(-1,ind,m_ColorClick);
		QuickSetText(2,ind, str1);
		QuickSetMask(2,ind,_T(""));
			
		GetCell(0,ind, &cell);			

		int n=m_pPGArchive->GetNumPGFile();
		CPG *pPG;
		for(int j=0;j<n;j++) {
			pPG=(CPG*)m_pPGArchive->GetPGFile(j);
			optionlist+=pPG->GetFileName(true).c_str();
			optionlist+="\n";
		}
		cell.SetCellType(UGCT_DROPLIST);
		cell.SetLabelText(optionlist);
		cell.SetParam(100);
		cell.SetBackColor(RGB(240,240,50));
		cell.SetText(Name);
		SetCell(0,ind, &cell);
		JoinCells(3,ind,4,ind);
		int  nvar, nrep;	
			
		if(pCPG->IsOpen()){
			pCPG->ReOpen();
		}
		long nrec=pCPG->GetNumRecords();
		nvar=pCPG->GetNumVariables();
		long pgst=pCPG->GetLongTimeStart();
		long pgend=pCPG->GetLongTimeEnd();
		QuickSetNumber(1,ind, nvar);
		QuickSetNumber(2,ind, nrec);
		nrep=pCPG->GetNumRepititions();
		QuickSetNumber(3,ind, nrep);
		bool validrepplot=false;
		if(pgst>0) {
			QuickSetText(5,ind,PGUtil::StringDatum(pgst));
			QuickSetText(6,ind,PGUtil::StringDatum(pgend));
			GetCell(7,ind, &cell);
			cell.SetCellType(UGCT_DROPLIST);
			optionlist="Normal Time\n";
			optionlist+="Longitude\n";
			optionlist+="Latitude\n";			
			optionlist+="Altitude\n";
			cell.SetLabelText(optionlist);
			m_VarDim1Combo.Param = GetUniqueMenuNumber();
			cell.SetParam(m_VarDim1Combo.Param);
			validrepplot=false;
			Name="Normal Time";

			if(nrep>1) {
				if(m_pProp->GetIndepFirst()==vartype::GEO_LONG) Name="Longitude";
				if(m_pProp->GetIndepFirst()==vartype::GEO_LAT) Name="Latitude";
				if(m_pProp->GetIndepFirst()==vartype::GEO_ALT) Name="Altitude";
				cell.SetReadOnly(FALSE);
			}
			else {
				cell.SetReadOnly(TRUE);
			}
			cell.SetText(Name);
			SetCell(7,ind, &cell);
			GetCell(8,ind, &cell);
			cell.SetReadOnly(FALSE);
			if(Name=="Normal Time") {
				cell.SetReadOnly(TRUE);
			}
			else 
				validrepplot=true;
			cell.SetCellType(UGCT_DROPLIST);
			optionlist="Not Used\n";
			optionlist+="Longitude\n";
			optionlist+="Latitude\n";			
			optionlist+="Altitude\n";
			cell.SetLabelText(optionlist);

			Name="Not Used";
			if(nrep>1) {
				if(m_pProp->GetIndepSecond()==vartype::GEO_LONG) Name="Longitude";
				if(m_pProp->GetIndepSecond()==vartype::GEO_LAT) Name="Latitude";
				if(m_pProp->GetIndepSecond()==vartype::GEO_ALT) Name="Altitude";
				m_VarDim2Combo.Param = GetUniqueMenuNumber();
			}
		
			cell.SetParam(m_VarDim2Combo.Param);
			cell.SetText(Name);
			SetCell(8,ind, &cell);
			GetCell(9,ind, &cell);
			cell.SetReadOnly(TRUE);
			if(validrepplot) cell.SetReadOnly(FALSE);
			cell.SetCellType(UGCT_DROPLIST);
			optionlist="Not Used\n";
			optionlist+="Longitude\n";
			optionlist+="Latitude\n";			
			optionlist+="Altitude\n";
			cell.SetLabelText(optionlist);
			
			cell.SetParam(m_VarDim3Combo.Param);
			Name="Not Used";
			if(m_pProp->GetIndepDependent()==vartype::GEO_LONG) Name="Longitude";
			if(m_pProp->GetIndepDependent()==vartype::GEO_LAT) Name="Latitude";
			if(m_pProp->GetIndepDependent()==vartype::GEO_ALT) Name="Altitude";
			
			cell.SetText(Name);
			SetCell(9,ind, &cell);
		}
		

		ind++;

		QuickSetText(0,ind,"Variable Name");

		QuickSetText(-1,ind,"Var");
		QuickSetText(-2,ind,"Rep");

		if(!m_ShowDetails) {
			QuickSetText(1,ind,"Unit");
			QuickSetText(2,ind,"Id");
			QuickSetText(3,ind,"Pos");
		//	QuickSetText(4,ind,"Chart");
			QuickSetText(5,ind,"MinValue");
			QuickSetText(6,ind,"MaxValue");
			QuickSetText(7,ind,"Missings");
			QuickSetText(8,ind,"Substituted");
		}
		else {
			QuickSetText(1,ind,"Country");
			QuickSetText(2,ind,"Station");
			QuickSetText(3,ind,"Altitude");
			QuickSetText(5,ind,"Latitude");
			QuickSetText(6,ind,"Longitude");
			QuickSetText(7,ind,"");
			QuickSetText(8,ind,"");
		}
		JoinCells(3,ind,4,ind);
		int ncol=9;
		for(int i=0;i<ncol;i++)
			QuickSetBackColor(i, ind, m_ColorHeading);

		
		
		if(m_RepFirst) {
			for(int j=0;j<nvar;j++) {
				for(int i=0;i<n_rep_toshow;i++) {
					ind++;
					QuickSetNumber(-1,ind,j+1);
					QuickSetNumber(-2,ind,i+1);
					GetCell(0,ind,&cell);
					cell.SetCellType(UGCT_CHECKBOX);
					cell.SetLabelText(CString(pCPG->GetVarName(j+1,i).c_str()));
					if(pCPG->GetPickedMean(j+1+i*nvar))
						cell.SetBool(TRUE);
					else
						cell.SetBool(FALSE);
					cell.SetParam(3100);
					SetCell(0,ind,&cell);

					if(m_ShowDetails) {
						GetCell(1,ind,&cell);
						cell.SetCellType(UGCT_NORMAL);
						test=pCPG->GetVarCountry(j+1,i).c_str();
						cell.SetText(test);
						cell.SetParam(0);
						cell.SetBackColor(pCPG->GetColor(j+1));
						SetCell(1,ind,&cell);
						GetCell(2,ind,&cell);
						cell.SetText(pCPG->GetVarStation(j+1,i).c_str());
						cell.SetParam(0);
						cell.SetBackColor(pCPG->GetColor(j+1));
						SetCell(2,ind,&cell);
						JoinCells(3,ind,4,ind);
						v=pCPG->GetVarAlt(j+1,i+1);
						if(v<0.5E38)	QuickSetNumber(3,ind,v);
						v=pCPG->GetVarLat(j+1,i+1);
						if(v<0.5E38)	QuickSetNumber(5,ind,v);
						v=pCPG->GetVarLong(j+1,i+1);
						if(v>-1.E38)	QuickSetNumber(6,ind,v);

					}
					else {
						GetCell(1,ind,&cell);
						cell.SetText(std::string(pCPG->GetVarUnit(j+1,i)).c_str());
						cell.SetParam(0);
						cell.SetBackColor(pCPG->GetColor(j+1));
						SetCell(1,ind,&cell);
						GetCell(2,ind,&cell);
						cell.SetText(pCPG->GetVarId(j+1,i).c_str());
						cell.SetParam(2000);
						cell.SetBackColor(pCPG->GetColor(j+1));
						SetCell(2,ind,&cell);
						JoinCells(3,ind,4,ind);
						v=pCPG->GetVarMin(j+1,i);
						GetCell(5,ind,&cell);
						if(validrepplot) {
							cell.SetCellType(UGCT_CHECKBOX);
							cell.SetParam(3100);
							if(pCPG->GetPickedMin(j+1+i*nvar))
								cell.SetBool(TRUE);
							else
								cell.SetBool(FALSE);

							CString Label;
							Label=MFC_Util::FtoAscii(v);
							cell.SetLabelText(Label);
						}
						else {
							cell.SetCellType(UGCT_NORMAL);

							if(v<0.5E38)	cell.SetNumber(v);
						}
						SetCell(5,ind,&cell);
						v=pCPG->GetVarMax(j+1,i);
						GetCell(6,ind,&cell);
						if(validrepplot) {
							cell.SetCellType(UGCT_CHECKBOX);
							cell.SetParam(3100);
							if(pCPG->GetPickedMax(j+1+i*nvar))
								cell.SetBool(TRUE);
							else
								cell.SetBool(FALSE);
							CString Label;
							Label=MFC_Util::FtoAscii(v);
							cell.SetLabelText(Label);
						}
						else {
							cell.SetCellType(UGCT_NORMAL);
							if(v<0.5E38)	cell.SetNumber(v);
						}
						SetCell(6,ind,&cell);
						size_t jjj=pCPG->GetVarMissing(j+1,i);
						QuickSetNumber(7,ind,jjj);
						jjj=pCPG->GetVarSubstituted(j+1,i);
						QuickSetNumber(8,ind,jjj);
					}

				}
			}
		}
		else {
			for(int i=0;i<nrep;i++) {
				for(int j=0;j<nvar;j++) {
					ind++;
					QuickSetNumber(-1,ind,j+1);
					QuickSetNumber(-2,ind,i+1);
					GetCell(0,ind,&cell);
					cell.SetCellType(UGCT_CHECKBOX);
					cell.SetLabelText(pCPG->GetVarName(j+1,i).c_str());
					if(pCPG->GetPicked(j+1+i*nvar))
						cell.SetBool(TRUE);
					else
						cell.SetBool(FALSE);
					cell.SetParam(0);
					SetCell(0,ind,&cell);

					if(m_ShowDetails) {
						GetCell(1,ind,&cell);
						cell.SetText(pCPG->GetVarCountry(j+1,i).c_str());
						cell.SetParam(0);
						cell.SetBackColor(pCPG->GetColor(j+1));
						SetCell(1,ind,&cell);
						GetCell(2,ind,&cell);
						cell.SetText(pCPG->GetVarStation(j+1,i).c_str());
						cell.SetParam(2000);
						cell.SetBackColor(pCPG->GetColor(j+1));
						SetCell(2,ind,&cell);
						JoinCells(3,ind,4,ind);
						v=pCPG->GetVarAlt(j+1,i+1);
						if(v<0.5E38)	QuickSetNumber(3,ind,v);
						v=pCPG->GetVarLat(j+1,i+1);
						if(v<0.5E38)	QuickSetNumber(5,ind,v);
						v=pCPG->GetVarLong(j+1,i+1);
						if(v>-1.E38)	QuickSetNumber(6,ind,v);

					}
					else {
						GetCell(1,ind,&cell);
						cell.SetText(std::string(pCPG->GetVarUnit(j+1,i)).c_str());
						cell.SetParam(0);
						cell.SetBackColor(pCPG->GetColor(j+1));
						SetCell(1,ind,&cell);
						GetCell(2,ind,&cell);
						cell.SetText(pCPG->GetVarId(j+1,i).c_str());
						cell.SetParam(2000);
						cell.SetBackColor(pCPG->GetColor(j+1));
						SetCell(2,ind,&cell);
						JoinCells(3,ind,4,ind);
						v=pCPG->GetVarMin(j+1,i+1);
						if(v<0.5E38)	QuickSetNumber(5,ind,v);
						v=pCPG->GetVarMax(j+1,i+1);
						if(v>-1.E38)	QuickSetNumber(6,ind,v);
						size_t jjj=pCPG->GetVarMissing(j+1,i);
						QuickSetNumber(7,ind,jjj);
						jjj=pCPG->GetVarSubstituted(j+1,i);
						QuickSetNumber(8,ind,jjj);
					}


				}
			}
		}


		RedrawAll();

	return true;

}
bool GridPGDoc::Make_NewSpatialCoord(int ifile)
{
	CUGCell cell; 
	CString heading, number, test;

	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	CString Defstring;
	CString optionlist;
	int NumLines;
	int MaxCol=8 ;
	int colv=0;
	CPtrArray CPG_Array;

	double Lat_Min, Lat_Max, Long_Min, Long_Max;
	
	Lat_Min = Lat_Max = Long_Min = Long_Max = 0.;
	BOOL Tableexpand;

	Tableexpand=FALSE;
	EnableMenu(TRUE); 

	SetSheetNumber(2); 
	SetDefFont(&m_font);
	EnableUpdate(FALSE);
	while(GetNumberRows()>0)
		DeleteRow(0);

	EnableUpdate(TRUE);

	SetMultiSelectMode ( TRUE );

	CPG *pCPG;
	if(m_IndexSource>=m_pPGArchive->GetNumPGFile())
		m_IndexSource=0;


	if(m_pPGArchive->GetNumPGFile()<=m_IndexSource) return false;

	pCPG=(CPG*)m_pPGArchive->GetPGFile(m_IndexSource);

	m_pGSource=pCPG;
		SetDefFont(&m_font);
		SetNumberCols(8); 
		SetSH_NumberCols(1);
		SetSH_Width(0);
		//SetColWidth(-2,20);
		//SetColWidth(-1,30);

		SetColWidth(0,500);
		SetColWidth(1,110);
		SetColWidth(2,110);
		SetColWidth(3,55);
		SetColWidth(4,55);
		SetColWidth(5,110);
		SetColWidth(6,110);
		SetColWidth(7,110);

		SetTH_Height(30);
		QuickSetText(0,-1, "FileName");
		QuickSetText(1,-1, "Variables");
		QuickSetText(2,-1, "Records");
		QuickSetText(3,-1, "Repetitions");
		QuickSetText(5,-1, "Start Date");
		QuickSetText(6,-1, "End Date");
		QuickSetText(7,-1, "");
		JoinCells(3,-1,4,-1);


		NumLines=3;
		int newpglines=0;
	
		SetNumberRows(NumLines+newpglines); 

		int ind=0;

		Name=pCPG->GetFileName(true).c_str();
		//QuickSetNumber(-1,ind,i+1);
		//QuickSetNumber(-2,ind,i);
		
		QuickSetBackColor(-1,ind,m_ColorClick);
			
		GetCell(0,ind, &cell);			

		int n=m_pPGArchive->GetNumPGFile();
		CPG *pPG;
		for(int j=0;j<n;j++) {
			pPG=(CPG*)m_pPGArchive->GetPGFile(j);
			optionlist+=pPG->GetFileName(true).c_str();
			optionlist+="\n";
		}
		cell.SetCellType(UGCT_DROPLIST);
		cell.SetLabelText(optionlist);
		cell.SetParam(100);
		cell.SetBackColor(RGB(240,240,50));
		cell.SetText(Name);
		SetCell(0,ind, &cell);
		JoinCells(3,ind,4,ind);
		int  nvar, nrep;	
		nrep=pCPG->GetNumRepititions();	
		if(pCPG->IsOpen()){
			pCPG->ReOpen();
			long nrec=pCPG->GetNumRecords();
			nvar=pCPG->GetNumVariables();
			long pgst=pCPG->GetLongTimeStart();
			long pgend=pCPG->GetLongTimeEnd();
			QuickSetNumber(1,ind, nvar);
			QuickSetNumber(2,ind, nrec);
			nrep=pCPG->GetNumRepititions();
			QuickSetNumber(3,ind, nrep);
			if(pgst>0) {
				QuickSetText(5,ind,PGUtil::StringDatum(pgst));
				QuickSetText(6,ind,PGUtil::StringDatum(pgend));
			}
			Lat_Min=Long_Min=200.;
			Lat_Max=Long_Max=-200;

			for(int i=0; i<nrep;i++) {
				double Lat=pCPG->GetVarLat(1,i+1);
				double Long=pCPG->GetVarLong(1,i+1);
				if(Lat_Min>Lat) Lat_Min=Lat;
				if(Lat_Max<Lat) Lat_Max=Lat;
				if(Long_Min>Long) Long_Min=Long;
				if(Long_Max<Long) Long_Max=Long;
			}
			if(m_Latitude>360.&&Lat_Max>-190)
				m_Latitude=(Lat_Min+Lat_Max)/2.;
			if(m_Longitude>360.&&Long_Max>-190)
				m_Longitude=(Long_Min+Long_Max)/2.;
		}

			


		ind++;
		QuickSetText(0,ind,"Output File Name");
		QuickSetText(1,ind,"Lat Min");
		QuickSetText(2,ind,"Lat Max");
		QuickSetText(3,ind,"Lat New");
		QuickSetText(5,ind,"Long Min");
		QuickSetText(6,ind,"Long Max");
		QuickSetText(7,ind,"Long New");
		JoinCells(3,ind,4,ind);
		int ncol=8;
		for(int i=0;i<ncol;i++)
			QuickSetBackColor(i, ind, m_ColorHeading);

		m_spatialPG.SetSourceFile(pCPG);
		ind++;
		if(m_DestFileSet[2]==false) {
			m_spatialPG.m_DestFileName=pCPG->GetFileName();
			string newfilename;
			newfilename=pCPG->GetFileName();
			int ipos=newfilename.find(".bin");
			if(ipos<0) ipos=newfilename.find(".BIN");
			newfilename=newfilename.substr(0,ipos);
			newfilename.append("x.bin");
			m_spatialPG.m_DestFileName=newfilename;
		}


		if(nrep>1&&pCPG->IsAddInfoSet()&&pCPG->IsOpen()) {
			m_SpatialSourceFileReady=true;
			QuickSetText(0,ind,m_spatialPG.m_DestFileName);
			QuickSetBackColor(0,ind, m_ColorClick);

			QuickSetNumber(1,ind, Lat_Min);
			QuickSetNumber(2,ind, Lat_Max);
			UnJoinCells(3,ind);

			UnJoinCells(4,ind);
			JoinCells(3,ind,4,ind);

			GetCell(3,ind, &cell);
			cell.SetCellType(UGCT_NORMAL);
			cell.SetReadOnly(FALSE);
			cell.SetNumber(m_Latitude);
			cell.SetBackColor(m_ColorClick);
			SetCell(3,ind, &cell);
		
			GetCell(7,ind, &cell);
			cell.SetCellType(UGCT_NORMAL);
			cell.SetReadOnly(FALSE);
			cell.SetNumber(m_Longitude);
			cell.SetBackColor(m_ColorClick);
			SetCell(7,ind, &cell);
		

			QuickSetNumber(5,ind, Long_Min);
			QuickSetNumber(6,ind, Long_Max);
		}
		else {
			m_SpatialSourceFileReady=false;
			QuickSetText(0,ind,"Estimation not possible for file without coordinates");
			QuickSetBackColor(0,ind, m_ColorCompleted);
		}


		RedrawAll();


	return true;

}
bool GridPGDoc::Make_NewSpatialGrid(int ifile)
{
	CUGCell cell; 
	CString heading, number, test;

	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	CString str1, Defstring;
	CString optionlist;
	int NumLines;
	int MaxCol=8 ;
	int colv=0;
	CPtrArray CPG_Array;
	double Lat_Min, Lat_Max, Long_Min, Long_Max;
	
	BOOL Tableexpand;

	Tableexpand=FALSE;
	EnableMenu(TRUE); 

	SetSheetNumber(3); 
	EnableUpdate(FALSE);
	while(GetNumberRows()>0)
		DeleteRow(0);

	EnableUpdate(TRUE);
	CPG *pCPG;
	if(m_IndexSource>=m_pPGArchive->GetNumPGFile())
		m_IndexSource=0;
	if(m_pPGArchive->GetNumPGFile()<=m_IndexSource) return false;

	pCPG=(CPG*)m_pPGArchive->GetPGFile(m_IndexSource);
	m_pGSource=pCPG;

		SetDefFont(&m_font);
		SetNumberCols(8); 
		SetSH_NumberCols(1);
		SetSH_Width(0);
		//SetColWidth(-2,20);
		//SetColWidth(-1,30);

		SetColWidth(0,500);
		SetColWidth(1,110);
		SetColWidth(2,110);
		SetColWidth(3,55);
		SetColWidth(4,55);
		SetColWidth(5,110);
		SetColWidth(6,110);
		SetColWidth(7,110);

		SetTH_Height(30);
		QuickSetText(0,-1, "FileName");
		QuickSetText(1,-1, "Variables");
		QuickSetText(2,-1, "Records");
		QuickSetText(3,-1, "Repetitions");
		QuickSetText(5,-1, "Start Date");
		QuickSetText(6,-1, "End Date");
		QuickSetText(7,-1, "");
		JoinCells(3,-1,4,-1);


		NumLines=3;
		int newpglines=0;
	
		SetNumberRows(NumLines+newpglines); 

		int ind=0;

		Name=pCPG->GetFileName().c_str();
		//QuickSetNumber(-1,ind,i+1);
		//QuickSetNumber(-2,ind,i);
		
		QuickSetBackColor(-1,ind,m_ColorClick);
			
		GetCell(0,ind, &cell);			

		int n=m_pPGArchive->GetNumPGFile();
		CPG *pPG;
		for(int j=0;j<n;j++) {
			pPG=(CPG*)m_pPGArchive->GetPGFile(j);
			optionlist+=pPG->GetFileName().c_str();
			optionlist+="\n";
		}
		cell.SetCellType(UGCT_DROPLIST);
		cell.SetLabelText(optionlist);
		cell.SetParam(100);
		cell.SetBackColor(RGB(240,240,50));
		cell.SetText(Name);
		SetCell(0,ind, &cell);
		JoinCells(3,ind,4,ind);
		int  nvar, nrep;	
			
	
			pCPG->ReOpen();
	
			long nrec=pCPG->GetNumRecords();
			nvar=pCPG->GetNumVariables();
			long pgst=pCPG->GetLongTimeStart();
			long pgend=pCPG->GetLongTimeEnd();
			QuickSetNumber(1,ind, nvar);
			QuickSetNumber(2,ind, nrec);
			nrep=pCPG->GetNumRepititions();
			QuickSetNumber(3,ind, nrep);
			if(pgst>0) {
				QuickSetText(5,ind,PGUtil::StringDatum(pgst));
				QuickSetText(6,ind,PGUtil::StringDatum(pgend));
			}
			Lat_Min=Long_Min=200.;
			Lat_Max=Long_Max=-200.;

			for(int i=0; i<nrep;i++) {
				double Lat=pCPG->GetVarLat(1,i+1);
				double Long=pCPG->GetVarLong(1,i+1);
				if(Lat>MISSING&&Long>MISSING) {
					if(Lat_Min>Lat) Lat_Min=Lat;
					if(Lat_Max<Lat) Lat_Max=Lat;
					if(Long_Min>Long) Long_Min=Long;
					if(Long_Max<Long) Long_Max=Long;
				}
			}
			if(m_Lat_Max>360.&&Lat_Max>-190)
				m_Lat_Max=double(int(Lat_Max+0.5));
			if(m_Lat_Min>360.&&Lat_Min<190)
				m_Lat_Min=double(int(Lat_Min));
			if(m_Long_Max>360.&&Long_Max>-190)
				m_Long_Max=double(int(Long_Max+0.5));
			if(m_Long_Min>360.&&Long_Min<190)
				m_Long_Min=double(int(Long_Min));
		

		ind++;

		QuickSetText(0,ind,"Output File Name");
		string txt="Lat Min (";
		txt.append(FUtil::STD_DtoAscii(Lat_Min));
		txt.append(")");
		QuickSetText(1,ind,txt);
		
		txt="Lat Max (";
		txt.append(FUtil::STD_DtoAscii(Lat_Max));
		txt.append(")");
		QuickSetText(2,ind,txt);

		QuickSetText(3,ind,"New Grids");
		
		txt="Long Min (";
		txt.append(FUtil::STD_DtoAscii(Long_Min));
		txt.append(")");
		QuickSetText(5,ind,txt);

		txt="Long Max (";
		txt.append(FUtil::STD_DtoAscii(Long_Max));
		txt.append(")");
		QuickSetText(6,ind,txt);
		
		QuickSetText(7,ind,"New Grid");
		JoinCells(3,ind,4,ind);
		int ncol=8;
		for(int i=0;i<ncol;i++)
			QuickSetBackColor(i, ind, m_ColorHeading);

		m_spatialPG.SetSourceFile(pCPG);
		ind++;
		if(m_DestFileSet[3]==false) {
			m_spatialPG.m_DestFileName=pCPG->GetFileName();
			string newfilename;
			newfilename=pCPG->GetFileName();
			int ipos=newfilename.find(".bin");
			if(ipos<0) ipos=newfilename.find(".BIN");
			newfilename=newfilename.substr(0,ipos);
			newfilename.append("grid.bin");
			m_spatialPG.m_DestFileName=newfilename;
		}

		if(nrep>1&&pCPG->IsAddInfoSet()) {
			m_SpatialSourceFileReady=true;


		QuickSetText(0,ind,m_spatialPG.m_DestFileName);
		QuickSetBackColor(0,ind, m_ColorClick);
		QuickSetNumber(1,ind, m_Lat_Min);
		QuickSetBackColor(1,ind, m_ColorClick);
		QuickSetNumber(2,ind, m_Lat_Max);
		QuickSetBackColor(2,ind, m_ColorClick);
		UnJoinCells(3,ind);

		UnJoinCells(4,ind);
		JoinCells(3,ind,4,ind);

		GetCell(3,ind, &cell);
		cell.SetCellType(UGCT_NORMAL);
		cell.SetReadOnly(FALSE);
		cell.SetNumber(m_GridLat);
		cell.SetBackColor(m_ColorClick);
		SetCell(3,ind, &cell);
		
		GetCell(7,ind, &cell);
		cell.SetCellType(UGCT_NORMAL);
		cell.SetReadOnly(FALSE);
		cell.SetNumber(m_GridLong);
		cell.SetBackColor(m_ColorClick);
		SetCell(7,ind, &cell);

		QuickSetNumber(5,ind, m_Long_Min);
		QuickSetBackColor(5,ind, m_ColorClick);
		QuickSetNumber(6,ind, m_Long_Max);
		QuickSetBackColor(6,ind, m_ColorClick);
		}
		else {
			m_SpatialSourceFileReady=false;
			QuickSetText(0,ind,"Estimation not possible for file without coordinates");
QuickSetBackColor(0, ind, m_ColorCompleted);
		}


		RedrawAll();
		return true;
}
bool GridPGDoc::Make_DataSubstitution()
{
	CUGCell cell;
	string heading, number, test;

	string Name, Group, Type, Unit, PrevGroup, PrevGroup2, DatumString;
	string str1, Defstring;
	string optionlist;
	int NumLines;
	int MaxCol = 8;
	int colv = 0;
	CPtrArray CPG_Array;

	BOOL Tableexpand;

	Tableexpand = FALSE;
	EnableMenu(TRUE);

	SetSheetNumber(4);
	EnableUpdate(FALSE);
	while (GetNumberRows() > 0)
		DeleteRow(0);

	EnableUpdate(TRUE);
	CPG *pCPG;
	if (m_IndexSource >= m_pPGArchive->GetNumPGFile())
		m_IndexSource = 0;
	if (m_pPGArchive->GetNumPGFile() <= m_IndexSource) return false;

	pCPG = (CPG*)m_pPGArchive->GetPGFile(m_IndexSource);
	m_pGSource = pCPG;

	SetDefFont(&m_font);
	SetNumberCols(7);
	SetSH_NumberCols(1);
	SetSH_Width(0);
	//SetColWidth(-2,20);
	//SetColWidth(-1,30);



	SetTH_Height(30);
	NumLines = 3;
	int newpglines = 0;
	int reslines = 0;
	reslines = m_spatialPG.m_statsub.size();

	int ind = 0;
	if (reslines > 0 && m_ShowSubResults) {
		SetColWidth(0, 550);
		SetColWidth(1, 110);
		SetColWidth(2, 110);
		SetColWidth(3, 55);
		SetColWidth(4, 55);
		SetColWidth(5, 110);
		SetColWidth(6, 110);
		SetColWidth(7, 110);
		SetNumberCols(7);
		SetMultiSelectMode(TRUE);
		SetNumberRows(reslines + 2);
		string txt;
		txt = "Variables substituted by Linear regression made in :";
		if (m_pPG_Results != nullptr)
			txt += m_pPG_Results->GetFileName();
		QuickSetText(0, -1, txt);
		QuickSetText(1, -1, "Substituted by");
		QuickSetText(2, -1, "n Substituted");
		QuickSetText(3, -1, "R2");
		QuickSetText(5, -1, "Intercept");
		QuickSetText(6, -1, "Slope");
		string varut, varin;

		for (int i = 0; i < reslines; i++) {

			if (m_spatialPG.m_pPGSourceReg != nullptr) {
				varut = m_spatialPG.m_pPGSourceReg->GetVarName(m_spatialPG.m_statsub[i].VarSub, m_spatialPG.m_statsub[i].VarSubRep);
				varut += FUtil::STD_ItoAscii(m_spatialPG.m_statsub[i].VarSub) + ":" + FUtil::STD_ItoAscii(m_spatialPG.m_statsub[i].VarSubRep);
			}
			else
				varut = FUtil::STD_ItoAscii(m_spatialPG.m_statsub[i].VarSub) + ":" + FUtil::STD_ItoAscii(m_spatialPG.m_statsub[i].VarSubRep);
			QuickSetText(0, ind + i, varut);
			varin = FUtil::STD_ItoAscii(m_spatialPG.m_statsub[i].VarUsed) + ":" + FUtil::STD_ItoAscii(m_spatialPG.m_statsub[i].VarUsedRep);
			QuickSetText(1, ind + i, varin);
			QuickSetNumber(2, ind + i, m_spatialPG.m_statsub[i].nSub);
			QuickSetNumber(3, ind + i, m_spatialPG.m_statsub[i].R2);
			JoinCells(3, ind + i, 4, ind + i);
			QuickSetNumber(5, ind + i, m_spatialPG.m_statsub[i].A0);
			QuickSetNumber(6, ind + i, m_spatialPG.m_statsub[i].A1);
		}
		QuickSetHeadingText(0, ind + reslines, "Created PG File", true);
		QuickSetText(0, ind + reslines+1, m_pPG_Results->GetFileName(),true);
		QuickSetBackColor(0, ind + reslines + 1, m_ColorClick);




	}
	else if (m_spatialPG.m_ResRandomSub.size() > 0&&m_ShowSubResults) {
		SetColWidth(0, 750);

		SetNumberCols(1);
		SetMultiSelectMode(TRUE);
		SetNumberRows(m_spatialPG.m_ResRandomSub.size()+1);
		string txt;
		txt = "Missing Values substituded by Values for another year";

		QuickSetText(0, -1, txt);
		for (size_t i = 0; i<m_spatialPG.m_ResRandomSub.size(); i++) {
			txt = m_spatialPG.m_ResRandomSub[i];
			QuickSetText(0, i, txt);
			if (i % 2 == 1) QuickSetBackColor(0, i, m_ColorClick);
		}
		QuickSetText(0, m_spatialPG.m_ResRandomSub.size(), "Click on File name above - to open file");


     }
	 else {
		SetColWidth(0,500);
		SetColWidth(1,110);
		SetColWidth(2,110);
		SetColWidth(3,55);
		SetColWidth(4,55);
		SetColWidth(5,110);
		SetColWidth(6,110);
		SetColWidth(7,110);
			SetNumberCols(8); 
			SetMultiSelectMode ( FALSE );
			SetNumberRows(NumLines+newpglines); 
			QuickSetText(0,-1, "FileName");
			QuickSetText(1,-1, "Variables");
			QuickSetText(2,-1, "Records");
			QuickSetText(3,-1, "Repetitions");
			QuickSetText(5,-1, "Start Date");
			QuickSetText(6,-1, "End Date");
			QuickSetText(7,-1, "");
			JoinCells(3,-1,4,-1);



			Name=pCPG->GetFileName().c_str();
			//QuickSetNumber(-1,ind,i+1);
			//QuickSetNumber(-2,ind,i);
		
			QuickSetBackColor(-1,ind,m_ColorClick);
			
			GetCell(0,ind, &cell);			

			int n=m_pPGArchive->GetNumPGFile();
			CPG *pPG;
			for(int j=0;j<n;j++) {
				pPG=(CPG*)m_pPGArchive->GetPGFile(j);
				optionlist+=pPG->GetFileName().c_str();
				optionlist+="\n";
			}
			cell.SetCellType(UGCT_DROPLIST);
			cell.SetLabelText(optionlist);
			cell.SetParam(100);
			cell.SetBackColor(RGB(240,240,50));
			cell.SetText(Name);
			SetCell(0,ind, &cell);
			JoinCells(3,ind,4,ind);
			int  nvar, nrep;	
			
			if(pCPG->IsOpen()){
				pCPG->ReOpen();
				long nrec=pCPG->GetNumRecords();
				nvar=pCPG->GetNumVariables();
				long pgst=pCPG->GetLongTimeStart();
				long pgend=pCPG->GetLongTimeEnd();
				QuickSetNumber(1,ind, nvar);
				QuickSetNumber(2,ind, nrec);
				nrep=pCPG->GetNumRepititions();
				QuickSetNumber(3,ind, nrep);
				if(pgst>0) {
				QuickSetText(5,ind,PGUtil::StringDatum(pgst));
				QuickSetText(6,ind,PGUtil::StringDatum(pgend));
				}
			}

			ind++;
			QuickSetText(0,ind,"Output File Name");
			QuickSetText(1,ind,"Method for substitution");
			JoinCells(1,ind,4,ind);
		
			int ncol=8;
			for(int i=0;i<ncol;i++)
				QuickSetBackColor(i, ind, m_ColorHeading);

			m_spatialPG.SetSourceFile(pCPG);
			ind++;
			string newfilename;
			newfilename=pCPG->GetFileName();
			int ipos=newfilename.find(".bin");
			if(ipos<0) ipos=newfilename.find(".BIN");
			if (ipos<0) ipos = newfilename.find(".Bin");
			int isub=newfilename.find("sub");
			if(m_DestFileSet[4]==false) {
				m_spatialPG.m_DestFileName=pCPG->GetFileName();		
				newfilename=newfilename.substr(0,ipos);
				if(isub<0)
					newfilename.append("sub.bin");
				else {
					int icount=1;
					string checksubname;
					checksubname="sub1";
					int isubc=newfilename.find(checksubname);
					while(isubc>0) {
						checksubname="sub";
						icount++;
						checksubname+=FUtil::STD_ItoAscii(icount);
						isubc=newfilename.find(checksubname);
					}
					newfilename=newfilename.substr(0,isub);
					newfilename.append(checksubname);
					newfilename.append(".bin");
				}
				m_spatialPG.m_DestFileName=newfilename;
			
			}
			
			QuickSetText(0,ind,m_spatialPG.m_DestFileName);

			optionlist="Estimated by using best linear regression obtained to other stations\n";
			optionlist+="Linear regression using for a maximal duration of gaps\n";
			optionlist+="Artifical random selection of data from other years\n";
		
			GetCell(1,ind, &cell);	
			cell.SetCellType(UGCT_DROPLIST);
			cell.SetLabelText(optionlist);
			cell.SetParam(100);
			cell.SetBackColor(RGB(240,240,50));
			string str="Estimated by using best linear regression obtained to other stations";
			if(m_SubstituteOption==0)
				str="Estimated by using best linear regression obtained to other stations";
			else if(m_SubstituteOption==1) {
				str="Linear regression using for a maximal duration of gaps";
				QuickSetText(7,ind-1,"Maximal Gap (days)");
				QuickSetNumber(7,ind, m_MaxSubstituteDays, false);
			}
			else if(m_SubstituteOption==2)
				str="Artifical random selection of data from other years";
			cell.SetText(str.c_str());
			SetCell(1,ind, &cell);
			JoinCells(1,ind,4,ind);
			QuickSetText(5,ind, "Click to Create New File");
			QuickSetBackColor(5,ind,m_ColorClick);
			JoinCells(5,ind,6,ind);



		}








		RedrawAll();

	return true;
}
bool GridPGDoc::Make_ChangePeriodSheet()
{
	CUGCell cell; 
	CString heading, number, test;

	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	CString str1, Defstring;
	CString optionlist;
	int NumLines;
	int MaxCol=8 ;
	int colv=0;
	CPtrArray CPG_Array;
	
	BOOL Tableexpand;

	Tableexpand=FALSE;
	EnableMenu(TRUE); 

	SetSheetNumber(6); 
	EnableUpdate(FALSE);
	while(GetNumberRows()>0)
		DeleteRow(0);

	EnableUpdate(TRUE);
	CPG *pCPG;
	if(m_IndexSource>=m_pPGArchive->GetNumPGFile())
		m_IndexSource=0;
	if(m_pPGArchive->GetNumPGFile()<=m_IndexSource) return false;

	pCPG=(CPG*)m_pPGArchive->GetPGFile(m_IndexSource);
	m_pGSource=pCPG;

		SetDefFont(&m_font);
		SetNumberCols(7); 
		SetSH_NumberCols(1);
		SetSH_Width(0);
		//SetColWidth(-2,20);
		//SetColWidth(-1,30);



		SetTH_Height(30);
		NumLines=3;
		int newpglines=0;
		int reslines=0;
		reslines=m_ChangePeriod.m_ResultSum.size();

		int ind=0;
		if(reslines>0&&m_ShowSubResults) {
			SetColWidth(0,750);

			SetNumberCols(1); 
			SetMultiSelectMode ( TRUE );
			SetNumberRows(reslines+1); 
			string txt;
			txt="Period Changed";

			QuickSetText(0,-1, txt);
			for(size_t i=0; i<m_ChangePeriod.m_ResultSum.size();i++) {
				txt=m_ChangePeriod.m_ResultSum[i];
				QuickSetText(0,i, txt);
				if (i % 2 == 1) QuickSetBackColor(0, i, m_ColorClick);
			}
		}
		else {
		SetColWidth(0,500);
		SetColWidth(1,110);
		SetColWidth(2,110);
		SetColWidth(3,55);
		SetColWidth(4,55);
		SetColWidth(5,110);
		SetColWidth(6,110);
		SetColWidth(7,110);
			SetNumberCols(8); 
			SetMultiSelectMode ( FALSE );
			SetNumberRows(NumLines+newpglines); 
			QuickSetText(0,-1, "FileName");
			QuickSetText(1,-1, "Variables");
			QuickSetText(2,-1, "Records");
			QuickSetText(3,-1, "Repetitions");
			QuickSetText(5,-1, "Start Date");
			QuickSetText(6,-1, "End Date");
			QuickSetText(7,-1, "");
			JoinCells(3,-1,4,-1);



			Name=pCPG->GetFileName().c_str();
			//QuickSetNumber(-1,ind,i+1);
			//QuickSetNumber(-2,ind,i);
		
			QuickSetBackColor(-1,ind,m_ColorClick);
			
			GetCell(0,ind, &cell);			

			int n=m_pPGArchive->GetNumPGFile();
			CPG *pPG;
			for(int j=0;j<n;j++) {
				pPG=(CPG*)m_pPGArchive->GetPGFile(j);
				optionlist+=pPG->GetFileName().c_str();
				optionlist+="\n";
			}
			cell.SetCellType(UGCT_DROPLIST);
			cell.SetLabelText(optionlist);
			cell.SetParam(100);
			cell.SetBackColor(RGB(240,240,50));
			cell.SetText(Name);
			SetCell(0,ind, &cell);
			JoinCells(3,ind,4,ind);
			int  nvar, nrep;
			if(!pCPG->IsOpen()) pCPG->ReOpen();
			
			if(pCPG->IsOpen()){
				pCPG->ReOpen();
				long nrec=pCPG->GetNumRecords();
				nvar=pCPG->GetNumVariables();
				auto pgst=pCPG->GetLongTimeStart();
				auto pgend=pCPG->GetLongTimeEnd();
				QuickSetNumber(1,ind, nvar);
				QuickSetNumber(2,ind, nrec);
				nrep=pCPG->GetNumRepititions();
				QuickSetNumber(3,ind, nrep);
				if(pgst!= std::string::npos) {
				QuickSetText(5,ind,PGUtil::StringDatum(pgst),false);
				QuickSetText(6,ind,PGUtil::StringDatum(pgend),false);
				}
			}

			ind++;
			QuickSetText(0,ind,"Output File Name");
			QuickSetText(1,ind,"Method for Change of Period");
			JoinCells(1,ind,2,ind);
			if(m_ChangePeriod.m_StartOut==0) m_ChangePeriod.m_StartOut=1;
			if(m_ChangePeriod.m_EndOut==0) m_ChangePeriod.m_EndOut=pCPG->GetNumRecords();

			if(!pCPG->IsGood()) pCPG->Open(pCPG->GetFileName());
			auto pgst=pCPG->GetLongTimeStart();
			auto pgend=pCPG->GetLongTimeEnd();
			if(pgst!= std::string::npos) {
				QuickSetText(5,ind, PGUtil::StringDatum(pgst),true);
				QuickSetText(6,ind, PGUtil::StringDatum(pgend), true);
			}

			int ncol=5;
			for(int i=0;i<ncol;i++)
				QuickSetBackColor(i, ind, m_ColorHeading);

			ind++;
			string newfilename;
			newfilename=pCPG->GetFileName();
			int ipos=newfilename.find(".bin");
			if(ipos<0) ipos=newfilename.find(".BIN");
			if(ipos<0) ipos=newfilename.find(".Bin");
			int isub=newfilename.find("Per.bin");
			if(m_DestFileSet[6]==false) {
				m_ChangePeriod.m_DestFileName=pCPG->GetFileName();		
				newfilename=newfilename.substr(0,ipos);
				if(isub<0)
					newfilename.append("Per.bin");
				else {
					int icount=1;
					string checksubname;
					checksubname="Per1.";
					int isubc=newfilename.find(checksubname);
					while(isubc>0) {
						checksubname="Per.";
						icount++;
						checksubname+=FUtil::STD_ItoAscii(icount);
						isubc=newfilename.find(checksubname);
					}
					newfilename=newfilename.substr(0,isub);
					newfilename.append(checksubname);
					newfilename.append(".bin");
				}
				m_ChangePeriod.m_DestFileName=newfilename;
			
			}
			
			QuickSetText(0,ind,m_ChangePeriod.m_DestFileName, false);
			optionlist="Second half move to first half\n";
			optionlist+="New Specific dates\n";
			GetCell(1,ind, &cell);	
			cell.SetCellType(UGCT_DROPLIST);
			cell.SetLabelText(optionlist);
			cell.SetParam(100);
			cell.SetBackColor(RGB(240,240,50));
			string str="Second half move to first half";
			
			if(m_ChangePeriodOption!=0)	str="";
			if(m_ChangePeriodOption==1)	str="New Specific dates";

			cell.SetText(str.c_str());
			SetCell(1,ind, &cell);
			JoinCells(1,ind,4,ind);
			QuickSetText(5,ind, "Click to Create New File");
			QuickSetBackColor(5,ind,m_ColorClick);
			JoinCells(5,ind,6,ind);



		}








		RedrawAll();

	return true;
}
bool GridPGDoc::Make_DataAggregation()
{
	CUGCell cell; 
	CString heading, number, test;

	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	CString str1, Defstring;
	CString optionlist;
	int NumLines;
	int MaxCol=8 ;
	int colv=0;
	CPtrArray CPG_Array;
	
	BOOL Tableexpand;

	Tableexpand=FALSE;
	EnableMenu(TRUE); 

	SetSheetNumber(5); 
	EnableUpdate(FALSE);
	while(GetNumberRows()>0)
		DeleteRow(0);

	EnableUpdate(TRUE);
	CPG *pCPG;
	m_IndexSource=m_pPGArchive->GetIndex(m_pDoc);
	if(m_pPGArchive->GetNumPGFile()<=m_IndexSource) return false;

	pCPG=(CPG*)m_pPGArchive->GetPGFile(m_IndexSource);
	m_pGSource=pCPG;

		SetDefFont(&m_font);
		SetNumberCols(7); 
		SetSH_NumberCols(1);
		SetSH_Width(0);
		//SetColWidth(-2,20);
		//SetColWidth(-1,30);



		SetTH_Height(30);
		NumLines=3;
		int newpglines=0;
		int reslines=0;
		reslines=m_TimeAggregate.m_ResultSum.size();

		int ind=0;
		if(reslines>0&&m_ShowSubResults) {
			SetColWidth(0,750);

			SetNumberCols(1); 
			SetMultiSelectMode ( TRUE );
			SetNumberRows(reslines); 
			string txt;
			txt="Mean values calculated";

			QuickSetText(0,-1, txt);
			for(size_t i=0; i<m_TimeAggregate.m_ResultSum.size();i++) {
				txt=m_TimeAggregate.m_ResultSum[i];
				QuickSetText(0,i, txt);
				if(i==1) QuickSetBackColor(0,i,m_ColorClick);
			}
		}
		else {
		SetColWidth(0,500);
		SetColWidth(1,110);
		SetColWidth(2,110);
		SetColWidth(3,55);
		SetColWidth(4,55);
		SetColWidth(5,110);
		SetColWidth(6,110);
		SetColWidth(7,110);
			SetNumberCols(8); 
			SetMultiSelectMode ( FALSE );
			SetNumberRows(NumLines+newpglines); 
			QuickSetText(0,-1, "FileName");
			QuickSetText(1,-1, "Variables");
			QuickSetText(2,-1, "Records");
			QuickSetText(3,-1, "Repetitions");
			QuickSetText(5,-1, "Start Date");
			QuickSetText(6,-1, "End Date");
			QuickSetText(7,-1, "");
			JoinCells(3,-1,4,-1);



			Name=pCPG->GetFileName().c_str();
			//QuickSetNumber(-1,ind,i+1);
			//QuickSetNumber(-2,ind,i);
		
			QuickSetBackColor(-1,ind,m_ColorClick);
			
			GetCell(0,ind, &cell);			

			int n=m_pPGArchive->GetNumPGFile();
			CPG *pPG;
			for(int j=0;j<n;j++) {
				pPG=(CPG*)m_pPGArchive->GetPGFile(j);
				optionlist+=pPG->GetFileName().c_str();
				optionlist+="\n";
			}
			cell.SetCellType(UGCT_DROPLIST);
			cell.SetLabelText(optionlist);
			cell.SetParam(100);
			cell.SetBackColor(RGB(240,240,50));
			cell.SetText(Name);
			SetCell(0,ind, &cell);
			JoinCells(3,ind,4,ind);
			int  nvar, nrep;	
			
			//if(pCPG->IsOpen()){
				pCPG->ReOpen();
				long nrec=pCPG->GetNumRecords();
				nvar=pCPG->GetNumVariables();
				long pgst=pCPG->GetLongTimeStart();
				long pgend=pCPG->GetLongTimeEnd();
				QuickSetNumber(1,ind, nvar);
				QuickSetNumber(2,ind, nrec);
				nrep=pCPG->GetNumRepititions();
				QuickSetNumber(3,ind, nrep);
				if(pgst>0) {
				QuickSetText(5,ind,PGUtil::StringDatum(pgst));
				QuickSetText(6,ind,PGUtil::StringDatum(pgend));
				}
			//}

			ind++;
			QuickSetText(0,ind,"Output File Name");
			QuickSetText(1,ind,"Method for TimeAggregation");
			JoinCells(1,ind,2,ind);
			QuickSetText(3, ind, "Scaling of Aggregation Unit");
			JoinCells(3, ind, 4, ind);
			int ncol=8;
			for(int i=0;i<ncol;i++)
				QuickSetBackColor(i, ind, m_ColorHeading);

			ind++;
			string newfilename;
			newfilename=pCPG->GetFileName();
			string filebase;
			int ipos=newfilename.find(".bin");
			if(ipos<0) ipos=newfilename.find(".BIN");
			if(ipos<0) ipos=newfilename.find(".Bin");
			int isub=newfilename.find("sub");
			if(m_DestFileSet[5]==false) {
				m_TimeAggregate.m_DestFileName=pCPG->GetFileName();		
				newfilename=newfilename.substr(0,ipos);

				if(isub<0) {
					if(m_AggregateOption==0)
						newfilename.append("MeanYear");
					if(m_AggregateOption==1)
						newfilename.append("MeanMonth");
					if(m_AggregateOption==2)
						newfilename.append("MeanDay");
					if(m_AggregateOption==3)
						newfilename.append("MeanHour");
					if(m_AggregateOption==4)
						newfilename.append("Mean10Min");
				}
				filebase=newfilename;
				int icount=1;
				newfilename+=".bin";
				while (FUtil::IsFileExisting(newfilename)) {
					newfilename=filebase+FUtil::STD_ItoAscii(icount);
					newfilename+=".bin";
					icount++;
				}
				m_TimeAggregate.m_DestFileName=newfilename;
			
			}
			
			QuickSetText(0,ind,m_TimeAggregate.m_DestFileName);

			optionlist="Yearly Mean Values\n";
			optionlist+="Monthly Mean Values\n";
			optionlist+="Daily Mean Values\n";
			optionlist+="Hourly Mean Values\n";
			optionlist+="10 minutes Mean Values\n";
		
			GetCell(1,ind, &cell);	
			cell.SetCellType(UGCT_DROPLIST);
			cell.SetLabelText(optionlist);
			cell.SetParam(100);
			cell.SetBackColor(RGB(240,240,50));
			string str="Yearly Mean Values";
			if(m_AggregateOption==0)	str="Yearly Mean Value";
			else if(m_AggregateOption==1) str="Monthly Mean Value";
			else if(m_AggregateOption==2) str="Daily Mean Value";
			else if(m_AggregateOption==3) str="Hourly Mean Value";
			else if(m_AggregateOption==4) str="10 minutes Mean Value";
			cell.SetText(str.c_str());
			SetCell(1,ind, &cell);
			JoinCells(1,ind,2,ind);
			GetCell(3, ind, &cell);
			cell.SetNumber(m_AggregationStepSize);
		

			SetCell(3, ind, &cell);
			JoinCells(3, ind, 4, ind);
			QuickSetText(5,ind, "Click to Create New File");
			QuickSetBackColor(5,ind,m_ColorClick);
			JoinCells(5,ind,6,ind);



		}
		RedrawAll();

	return true;
}
bool GridPGDoc::DefineScalingFile() {
		string text;
		if(m_pPG_ScalingSource==nullptr) {
			MFC_Util::MessageBox("You should have a PG file (*.bin) which \n"
				"should be used as a scaling to expand your current PG-file.\n"
				"We assume that the current PG-file include all variables that you want to have in the output file\n"
				"The new created file should then use the scaling file to expand the variables to the duration\n"
				"of the the scaling file. The scaling file includes mean montly temperature and sum of precipitation.\n"
				"The montly values will be similar in the output file and in the scaling file","Reading scaling PG-file with Air temperature and Precipitation", MB_OK);
	

	/*		CString pszFileName ;
			LPCTSTR lpszDefExt="bin";
			LPCTSTR lpszFilter = "PG-File (*.bin)|*.bin";
			pszFileName="";
 			CFileDialog	NewFileDlg(TRUE,lpszDefExt,pszFileName, OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,lpszFilter);*/
			FILEDLG FDlg=MFC_Util::GetFileDialog("bin", "", "PG-File (*.bin)|*.bin");
			if(FDlg.opengood==IDOK) {
				string str=FDlg.filename;
				m_pPG_ScalingSource=new CPG;
				m_pPG_ScalingSource->Open(str);
				if(!m_pPG_ScalingSource->IsOpen()) {
					MFC_Util::MessageBox("File could not be open"
					,"Reading scaling text file with Air temperature and Precipitation", MB_OK);
					return FALSE;
				}
			}
			
		}
		return true;


}
bool GridPGDoc::Make_NewDataResolution()
{
	CUGCell cell; 
	CString heading, number, test;

	CString Name, Group, Type, Unit, PrevGroup,PrevGroup2, DatumString;
	CString str1, Defstring;
	CString optionlist;
	int NumLines;
	int MaxCol=8 ;
	int colv=0;
	CPtrArray CPG_Array;
	
	BOOL Tableexpand;

	Tableexpand=FALSE;
	EnableMenu(TRUE); 

	SetSheetNumber(7); 
	EnableUpdate(FALSE);
	while(GetNumberRows()>0)
		DeleteRow(0);

	EnableUpdate(TRUE);
	CPG *pCPG;
	m_IndexSource=m_pPGArchive->GetIndex(m_pDoc);
	if(m_pPGArchive->GetNumPGFile()<=m_IndexSource) return false;

	pCPG=(CPG*)m_pPGArchive->GetPGFile(m_IndexSource);
	m_pGSource=pCPG;

		CString text;

		SetDefFont(&m_font);
		SetNumberCols(7); 
		SetSH_NumberCols(1);
		SetSH_Width(0);
		//SetColWidth(-2,20);
		//SetColWidth(-1,30);



		SetTH_Height(30);
		NumLines=4;
		int newpglines=0;
		int reslines=0;
		reslines=m_ChangeResolution.m_ResultSum.size();
		int var_des_lines=0;
		if(m_pPG_ScalingSource!=nullptr&&m_pGSource!=nullptr) {
			var_des_lines=max(m_pPG_ScalingSource->GetNumVariables(),m_pGSource->GetNumVariables())+2;


		}

		int ind=0;
		if(m_ShowCalculating) {
			SetColWidth(0,750);
			SetNumberCols(1); 
			SetMultiSelectMode ( TRUE );
			SetNumberRows(2);
			auto txt="New resolution corresponding to scaling template file will be estimated";
			QuickSetText(0,-1, txt);
			txt="Wait for the operation to be completed.....";
			QuickSetText(0,0, txt);
			txt="The calculation may take long time if your task is big.";
			QuickSetText(0,1, txt);
		}
		else if(reslines>0&&m_ShowSubResults) {
			SetColWidth(0,750);
			SetNumberCols(1); 
			SetMultiSelectMode ( TRUE );
			SetNumberRows(reslines); 
			string txt;
			txt="New resolution corresponding template file was estimated";
			QuickSetText(0,-1, txt);
			for(size_t i=0; i<m_ChangeResolution.m_ResultSum.size();i++) {
				txt=m_ChangeResolution.m_ResultSum[i];
				QuickSetText(0,i, txt);
				if(i==1) QuickSetBackColor(0,i,m_ColorClick);
			}
		}
		else {
			SetColWidth(0,500);
			SetColWidth(1,110);
			SetColWidth(2,110);
			SetColWidth(3,55);
			SetColWidth(4,55);
			SetColWidth(5,110);
			SetColWidth(6,110);
			SetColWidth(7,110);
			SetNumberCols(8); 
			SetMultiSelectMode ( FALSE );
			SetNumberRows(NumLines+newpglines+var_des_lines); 
			QuickSetText(0,-1, "FileName");
			QuickSetText(1,-1, "Variables");
			QuickSetText(2,-1, "Records");
			QuickSetText(3,-1, "Repetitions");
			QuickSetText(5,-1, "Start Date");
			QuickSetText(6,-1, "End Date");
			QuickSetText(7,-1, "");
			JoinCells(3,-1,4,-1);

			Name=pCPG->GetFileName().c_str();
			//QuickSetNumber(-1,ind,i+1);
			//QuickSetNumber(-2,ind,i);
		
			QuickSetBackColor(-1,ind,m_ColorClick);
			
			GetCell(0,ind, &cell);			

			int n=m_pPGArchive->GetNumPGFile();
			CPG *pPG;
			for(int j=0;j<n;j++) {
				pPG=(CPG*)m_pPGArchive->GetPGFile(j);
				optionlist+=pPG->GetFileName().c_str();
				optionlist+="\n";
			}
			cell.SetCellType(UGCT_DROPLIST);
			cell.SetLabelText(optionlist);
			cell.SetParam(100);
			cell.SetBackColor(RGB(240,240,50));
			cell.SetText(Name);
			SetCell(0,ind, &cell);
			JoinCells(3,ind,4,ind);
			int  nvar, nrep;	
			
			if(!pCPG->IsOpen())	pCPG->ReOpen();

			long nrec=pCPG->GetNumRecords();
			nvar=pCPG->GetNumVariables();
			long pgst=pCPG->GetLongTimeStart();
			long pgend=pCPG->GetLongTimeEnd();
			QuickSetNumber(1,ind, nvar);
			QuickSetNumber(2,ind, nrec);
			nrep=pCPG->GetNumRepititions();
			QuickSetNumber(3,ind, nrep);
			if(pgst>0) {
				QuickSetText(5,ind,PGUtil::StringDatum(pgst));
				QuickSetText(6,ind,PGUtil::StringDatum(pgend));
			}
			
			ind++;
			if(m_pPG_ScalingSource!=nullptr) {
				if(!m_pPG_ScalingSource->IsOpen())	m_pPG_ScalingSource->ReOpen();
				
				nrec=m_pPG_ScalingSource->GetNumRecords();
				nvar=m_pPG_ScalingSource->GetNumVariables();
				pgst=m_pPG_ScalingSource->GetLongTimeStart();
				pgend=m_pPG_ScalingSource->GetLongTimeEnd();
				QuickSetText(0,ind,m_pPG_ScalingSource->GetFileName());
				QuickSetNumber(1,ind, nvar);
				QuickSetNumber(2,ind, nrec);
				nrep=m_pPG_ScalingSource->GetNumRepititions();
				QuickSetNumber(3,ind, nrep);
				if(pgst>0) {
					QuickSetText(5,ind,PGUtil::StringDatum(pgst));
					QuickSetText(6,ind,PGUtil::StringDatum(pgend));
				}
			}
			else {
				QuickSetText(0,ind,"Click to Define Scaling File for new period");


			}
			QuickSetBackColor(0,ind,m_ColorClick);
			ind++;

			QuickSetText(0,ind,"Output File Name");
			QuickSetText(1,ind,"Method to Estimate new Resolution");
			JoinCells(1,ind,4,ind);
		
			int ncol=8;
			for(int i=0;i<ncol;i++)
				QuickSetBackColor(i, ind, m_ColorHeading);

			ind++;
			string newfilename;
			newfilename=pCPG->GetFileName();
			string filebase;
			auto ipos=newfilename.find(".bin");
			if(ipos== std::string::npos) ipos=newfilename.find(".BIN");
			if(ipos== std::string::npos) ipos=newfilename.find(".Bin");
			auto isub=newfilename.find("Extended");
			if(ipos!= std::string::npos) {
				newfilename=newfilename.substr(0,ipos);
				newfilename+="Extended";
				newfilename+=".bin";
				m_ChangeResolution.m_DestFileName=newfilename;
			}
			else {
				m_ChangeResolution.m_DestFileName=newfilename;

			}
			optionlist="All Values Rescaled\n";
			optionlist+="Only Values Outside Orginal Period\n";
		
			GetCell(1,ind, &cell);	
			cell.SetCellType(UGCT_DROPLIST);
			cell.SetLabelText(optionlist);
			cell.SetParam(100);
			cell.SetBackColor(RGB(240,240,50));
			string str="Yearly Mean Values";
			if(m_NewResolutionOption==0)	str="All Values Rescaled";
			else if(m_NewResolutionOption==1) str="Only Values Outside Original Period";

			cell.SetText(str.c_str());
			SetCell(1,ind, &cell);
			JoinCells(1,ind,2,ind);

			GetCell(3,ind, &cell);	
			cell.SetCellType(UGCT_DROPLIST);
			optionlist="Only Original Variables\n";
			optionlist+="Add Extra Variables\n";
			cell.SetLabelText(optionlist);
			cell.SetParam(100);
			cell.SetBackColor(RGB(240,240,50));
			str="Add Extra Information";
			if(m_NewResolutionOption_Extra==1)	str="Add Extra Variables";
			else if(m_NewResolutionOption_Extra==0) str="Only Original Variables";

			cell.SetText(str.c_str());
			SetCell(3,ind, &cell);
			JoinCells(3,ind,4,ind);

			QuickSetText(0, ind, newfilename);
			if(m_pPG_ScalingSource!=nullptr) {
				if(m_NewResolutionSteps==2)
					QuickSetText(5,ind, "Click to Create New File");
				else
					QuickSetText(5,ind, "Define Matching Variables");
			}
			else {
				QuickSetText(5,ind, "Define Scaling File to extend period");
			}
			QuickSetBackColor(5,ind,m_ColorClick);
			JoinCells(5,ind,6,ind);
			if(var_des_lines>0) {
				// Make Description of Variables in File
				QuickSetText(0,ind+1,"Variables in input Files");
				QuickSetBackColor(0,ind+1,m_ColorHeading);
				JoinCells(0,ind+1,7,ind+1);
				QuickSetText(0,ind+2,"Original File");
				QuickSetBackColor(0,ind+2,m_ColorHeading);
	
				QuickSetText(1,ind+2,"Scaling File");
				QuickSetBackColor(1,ind+2,m_ColorHeading);
				JoinCells(1,ind+2,3,ind+2);
				for(size_t i=0; i<m_pGSource->GetNumVariables();i++) {
					auto txt=m_pGSource->GetVarName(i+1);
					GetCell(0,ind+3+i,&cell);
					cell.SetText(txt.c_str());
					cell.SetBackColor(m_NoColor);
					SetCell(0,ind+3+i,&cell);

				}
				if(m_ChangeResolution.m_MatchingVar.main_source_var>0) 
					QuickSetBackColor(0, ind+2+m_ChangeResolution.m_MatchingVar.main_source_var,m_ColorChanged);
				if(m_ChangeResolution.m_MatchingVar.second_source_var>0) 
					QuickSetBackColor(0, ind+2+m_ChangeResolution.m_MatchingVar.second_source_var,m_ColorCompleted);
				if(m_ChangeResolution.m_MatchingVar.main_scaling_var>0) 
					QuickSetBackColor(1, ind+2+m_ChangeResolution.m_MatchingVar.main_scaling_var,m_ColorChanged);
				if(m_ChangeResolution.m_MatchingVar.second_scaling_var>0) 
					QuickSetBackColor(1, ind+2+m_ChangeResolution.m_MatchingVar.second_scaling_var,m_ColorCompleted);
				for(size_t i=0; i<m_pPG_ScalingSource->GetNumVariables();i++) {
					auto txt=m_pPG_ScalingSource->GetVarName(i+1);
					GetCell(1,ind+3+i,&cell);
					cell.SetText(txt.c_str());
					SetCell(1,ind+3+i,&cell);
					JoinCells(1,ind+3+i,3,ind+3+i);
				}

				//QuickSetText(4,ind+2,"Define Matching Variables");
				QuickSetBackColor(4,ind+2,m_ColorHeading);
				JoinCells(4,ind+2, 7, ind+2);
				string txt;
				if(m_NewResolutionSteps==0) {
					txt="Click on Primary Matching Variables (Temp)";

				}
				else if(m_NewResolutionSteps==1) {
					txt="Click on Secondary Match Variables (Prec)";
				}
				else 
					txt="Click here to reset defined Variables";
				QuickSetText(4, ind+3, txt);
				JoinCells(4,ind+3, 7, ind+var_des_lines);
			}
		}
		RedrawAll();

	return true;
}
bool GridPGDoc::Make_New_PGFiles() {
	

	if (m_AddMix_Actions.m_pPGADD_Out != nullptr) 	return Make_Add_Prior();
	else if (m_AddMix_Actions.m_pPGReshape_Out!=nullptr) {
		//AddPGFile(m_ReShaped_Actions.m_DestFileName);
		return Make_ReShape_Prior(true);


	}
	else if (m_AddMix_Actions.m_pPGMix_Out != nullptr) return	Make_Mix_Prior();
	else if (m_AddMix_Actions.m_pPGMerge_Out != nullptr) return	Make_Merge_Prior();
	else if (m_pPGNewGrid != nullptr) return	Make_NewGrid_Prior();
	else if(m_PGFileImport.m_ScanningMade&&m_PGFileImport.m_ImportedFileVector.size()==0) return	Make_ImportFromCSV_File_Prior();
	else if (m_PGFileImport.m_ScanningMade&&m_PGFileImport.m_ImportedFileVector.size() > 0) return	Make_ImportFromCSV_File_Post();
	CUGCell cell;
	SetSH_ColWidth(-1, 0);
	for (size_t i = 0; i < 4; i++) UnJoinCells(i, -1);
	SetNewCleanSheetWithNumber(8);
	SetNumberCols(1);
	SetColWidth(0, 600);


	
	SetNumberRows(m_NewFileOptions.VectorNames.size());
	SetTH_NumberRows(1);
	QuickSetHeadingText(m_NewFileOptions.Col_start, -1, "Select Metod for creating of new file(s):");
	SetColWidth(-1, 0);
	SetSH_NumberCols(0);
	SetSH_ColWidth(-1, 0);
	SetCellClickButton(m_NewFileOptions);
	RedrawAll();
	return true;
}
bool GridPGDoc::Make_Add_Prior() {
	CUGCell cell;
	SetLocalWindowText("Adding files with the same Variables from different time periods to a single output File with entire period");
	SetNewCleanSheetWithNumber(8);
	SetNumberCols(8);
	SetPGFileColumns(8,-1,false);
	if(m_AddMix_Actions.m_pPGADD_Out==nullptr) m_AddMix_Actions.m_pPGADD_Out = new CPG;
	if (m_AddMix_Actions.m_PGPointerVector.size() == 0)  SetNumberRows(0);
	else SetNumberRows(m_AddMix_Actions.m_PGPointerVector.size()+2);

	if (m_AddMix_Actions.m_PGPointerVector.size() == 0) m_AddMorePGFilesButton.NormalState = false;
	else m_AddMorePGFilesButton.NormalState = true;

	SetCellClickButton(m_AddMorePGFilesButton);

	if (m_AddMix_Actions.m_PGPointerVector.size() > 0) {
		SetCellClickButton(m_ResetListAddButton);
		SetCellClickButton(m_CreateNewAddButton);
		SetCellClickButton(m_QuitButton);
		SetPGFileColumns(6, m_AddMix_Actions.m_PGPointerVector.size(), true);
	}
	else
		return true;
	m_AddFileList.VectorNames.clear();
	for (size_t i = 0; i < m_AddMix_Actions.m_PGPointerVector.size(); i++) {
		SetPGFileColumnsInfo(m_AddMix_Actions.m_PGPointerVector[i],1,5,i);
		m_AddFileList.VectorNames.push_back(m_AddMix_Actions.m_PGPointerVector[i]->GetFileName());
		QuickSetNumber(6, i, m_AddMix_Actions.m_PGPointerVector[i]->GetNormalTimeInterval());
		QuickSetText(7, i, FUtil::STD_ItoAscii(m_AddMix_Actions.m_AddScalingCoefVector[i]), false, m_AddScalingCoefHint.param);

	}
	m_NewOutputPGFileButton.Row = m_AddMix_Actions.m_PGPointerVector.size() + 1;
	m_NewOutputPGFileButton.pPG = m_AddMix_Actions.m_pPGADD_Out;
	SetPGFileColumnsInfoButton(m_NewOutputPGFileButton);
	SetCellClickButton(m_AddFileList);

	RedrawAll();
	return true;
}
bool GridPGDoc::Make_Merge_Prior() {
	CUGCell cell;
	SetLocalWindowText("Adding files with the same variables and the same periods (but from different sites) to a single file with all sites (repetitions)");
	SetNewCleanSheetWithNumber(8);
	SetNumberCols(6);
	SetPGFileColumns(6, -1, false);
	if (m_AddMix_Actions.m_pPGMerge_Out == nullptr) m_AddMix_Actions.m_pPGMerge_Out = new CPG;

	if (m_AddMix_Actions.m_PGPointerVector.size() == 0)  SetNumberRows(0);
	else SetNumberRows(m_AddMix_Actions.m_PGPointerVector.size() + 2);

	if (m_AddMix_Actions.m_PGPointerVector.size() == 0) m_MergeMorePGFilesButton.NormalState = false;
	m_MergeMorePGFilesButton.NormalState = true;

	SetCellClickButton(m_MergeMorePGFilesButton);

	if (m_AddMix_Actions.m_PGPointerVector.size() > 0) {
		SetCellClickButton(m_ResetListAddButton);
		SetCellClickButton(m_CreateNewMergeButton);
		SetCellClickButton(m_QuitButton);
		SetPGFileColumns(6, m_AddMix_Actions.m_PGPointerVector.size(), true);
	}
	else
		return true;
	m_AddFileList.VectorNames.clear();

	for (size_t i = 0; i < m_AddMix_Actions.m_PGPointerVector.size(); i++) {
		SetPGFileColumnsInfo(m_AddMix_Actions.m_PGPointerVector[i], 1, 5, i);
		m_AddFileList.VectorNames.push_back(m_AddMix_Actions.m_PGPointerVector[i]->GetFileName());
	}

	m_NewOutputMergePGFileButton.Row = m_AddMix_Actions.m_PGPointerVector.size() + 1;
	m_NewOutputMergePGFileButton.pPG = m_AddMix_Actions.m_pPGMerge_Out;
	SetPGFileColumnsInfoButton(m_NewOutputMergePGFileButton);
	SetCellClickButton(m_AddFileList);
	RedrawAll();
	return true;
}
bool GridPGDoc::Make_ReShape_Prior(bool update) {
	CUGCell cell;

	SetLocalWindowText("Reshaping Current file");
	if(!update) SetNewCleanSheetWithNumber(8);
	SetNumberCols(7);
	SetPGFileColumns(6, -1, false);
	SetNumberRows(6+ m_ReShaped_Actions.treatment_convert.size());

	SetPGFileColumns(6, 1, true);
	//if (m_AddMix_Actions.m_pPGReshape_Out != nullptr) return false;

	SetCellClickButton(m_CreateNewReshapeButton);
	SetCellClickButton(m_QuitButton);
	SetCellClickButton(m_UpdateReshapeButton);
	SetCellClickButton(m_ShiftOutputFormat);

	m_ReShaped_Actions.m_RepetitionOutputStyle=m_ShiftOutputFormat.NormalState;
	//SetCellClickButton(m_TranslationListButton);
	CPG* pCPG;
	m_IndexSource = m_pPGArchive->GetIndex(m_pDoc);
	if (m_pPGArchive->GetNumPGFile() <= m_IndexSource) return false;

	pCPG = (CPG*)m_pPGArchive->GetPGFile(m_IndexSource);
	m_pGSource = pCPG;
	auto name= pCPG->GetFileName();
	auto irec = pCPG->GetNumRecords();
	auto ivar = pCPG->GetNumVariables();
	auto count_names = 0;

	vector<string> unique_names;
	vector<size_t> num_repof_names;
	vector<vector<size_t>> VarNamePos;


	for (auto i = 0; i < ivar; i++) {
		string name = pCPG->GetVarName(i + 1);
		bool exist = false;
		for (auto ii = 0; ii < unique_names.size(); ii++) {
			if (name == unique_names[ii]) {
				exist = true;
				num_repof_names[ii]++;
			}
		}
		if (!exist) {
			unique_names.push_back(name);
			num_repof_names.push_back(1);
			vector<size_t> k;
			VarNamePos.push_back(k);
		}
	}

	// HOW many variables with unique names
	for (auto i = 0; i < ivar; i++) {
		string name = pCPG->GetVarName(i + 1);
		for (auto ii = 0; ii < unique_names.size(); ii++) {
			if (name == unique_names[ii]) {
				VarNamePos[ii].push_back(i + 1);
				continue;
			}
		}
	}
	string idkod;
	bool treatmentkod = false;
	bool specialtreatmentkod = false;
	bool parcelNo = false;
	map<int, size_t> hits_map,  parc_hit_map;

	for (auto i = 0; i < unique_names.size(); i++) {

		for (auto j = 0; j < VarNamePos[i].size(); j++) {
			idkod = pCPG->GetVarId(VarNamePos[i][j]);
			auto ikodhit = m_ReShaped_Actions.GetTreatment(idkod);
			hits_map[ikodhit]++;
			ikodhit = m_ReShaped_Actions.GetParcell(idkod);
			if (ikodhit > 0) parc_hit_map[ikodhit]++;			
		}
	}

	

	
	for (auto i = 0; i < unique_names.size(); i++) {

		for (auto j = 0; j < VarNamePos[i].size(); j++) {
			idkod = pCPG->GetVarId(VarNamePos[i][j]);
			auto ikodhit = m_ReShaped_Actions.GetTreatment(idkod);
			if (m_ReShaped_Actions.GetTreatment(idkod) != 0) {
				treatmentkod = true;
				if (m_ReShaped_Actions.GetSpecTreatment(idkod) != 0) specialtreatmentkod = true;
				if (m_ReShaped_Actions.GetParcell(idkod) != 0) parcelNo = true;
			}
		}
	}

	SetCodeTranslateInfo(m_TranslationListButton, m_ReShaped_Actions.treatment_convert, m_ReShaped_Actions.Parcell_Stop, m_ReShaped_Actions.Parcell_Add, hits_map, parc_hit_map);

	name = m_pGSource->GetFileName();
	name = pCPG->GetFileName();
	m_AddMix_Actions.m_pPGReshape_Out = m_ReShaped_Actions.CreateNewReShapedFile(m_pGSource, unique_names, num_repof_names, VarNamePos, treatmentkod, specialtreatmentkod, parcelNo);	

	SetPGFileColumnsInfo(m_pGSource, 0, 6, 0, false, true);
	if (m_AddMix_Actions.m_pPGReshape_Out != nullptr) {
		m_AddMix_Actions.m_pPGReshape_Out->ReOpen();
		SetPGFileColumnsInfo(m_AddMix_Actions.m_pPGReshape_Out, 0, 6, 2, false, true);
	}
	//SetPGFileColumnsInfoButton(m_NewOutputPGFileButton);
	RedrawAll();
	return true;

}
bool GridPGDoc::Make_Add_Post() {
	CUGCell cell;
	SetNewCleanSheetWithNumber(8);
	SetNumberCols(6);
	SetPGFileColumns(6, -1, true);
	SetNumberRows(1);
	SetPGFileColumnsInfo(m_AddMix_Actions.m_pPGADD_Out, 0, 6, 0,false, true);
	RedrawAll();


	return true;
}
bool GridPGDoc::Make_Mix_Prior() {
	CUGCell cell;
	SetLocalWindowText("Mixing of files with different Variables to a single output File with all Variables");
	SetNewCleanSheetWithNumber(8);

	SetNumberCols(6);
	for (size_t i = 0; i < 6; i++) UnJoinCells(i, -1);
	SetPGFileColumns(6, -1, false);
	SetColWidth(0, 400);

	auto f = [&](size_t i) {SetColWidth(i+1, 200); };
	for (size_t i = 0; i < 3; i++) f(i);
	if (m_AddMix_Actions.m_pPGMix_Out == nullptr) m_AddMix_Actions.m_pPGMix_Out = new CPG;
	if (m_AddMix_Actions.m_PGPointerVector.size() == 0)  SetNumberRows(0);
	else SetNumberRows(m_AddMix_Actions.m_PGPointerVector.size() + 2);

	if (m_AddMix_Actions.m_PGPointerVector.size() == 0) m_MixMorePGFilesButton.NormalState = false;
	else m_MixMorePGFilesButton.NormalState = true;
	SetCellClickButton(m_MixMorePGFilesButton);

	if (m_AddMix_Actions.m_PGPointerVector.size() > 0) {
		SetCellClickButton(m_MixInterpolateButton);
		m_ResetListAddButton.Col_start = 2;
		SetCellClickButton(m_ResetListAddButton);
		m_ResetListAddButton.Col_start = 1;
		SetCellClickButton(m_CreateNewMixButton);
		SetCellClickButton(m_QuitButton);
		SetPGFileColumns(6, m_AddMix_Actions.m_PGPointerVector.size(), true);
	}
	else
		return true;
	m_AddFileList.VectorNames.clear();
	for (size_t i = 0; i < m_AddMix_Actions.m_PGPointerVector.size(); i++) {
		SetPGFileColumnsInfo(m_AddMix_Actions.m_PGPointerVector[i], 1, 5, i);
		m_AddFileList.VectorNames.push_back(m_AddMix_Actions.m_PGPointerVector[i]->GetFileName());
	}

	m_NewOutputMIXPGFileButton.Row=m_AddMix_Actions.m_PGPointerVector.size() + 1;
	m_NewOutputMIXPGFileButton.pPG = m_AddMix_Actions.m_pPGMix_Out;
	SetPGFileColumnsInfoButton(m_NewOutputMIXPGFileButton);
	
	SetCellClickButton(m_AddFileList);
	RedrawAll();


	return true;
}
bool GridPGDoc::Make_Mix_Post() {
	CUGCell cell;
	SetNewCleanSheetWithNumber(8);



	return true;
}
bool GridPGDoc::Make_NewGrid_Prior() {
	CUGCell cell;
	SetLocalWindowText("Definition of a New Empty Structure based on Number of Variables and Representation of time ");
	if (m_pPGNewGrid == nullptr) m_pPGNewGrid = new CPG();
	m_pPGNewGrid->SetCompleteRepRead(true);
	auto SetDefNewPGFileInfo = [&]() {
		m_pPGNewGrid->SetNumRecords(m_NewGridInfo.NumRec);
		m_pPGNewGrid->SetLongTimeStart(PGUtil::MinutConv(m_NewGridInfo.StartDate));
		m_pPGNewGrid->SetNormalTimeInterval(m_NewGridInfo.MinDiff);
		m_pPGNewGrid->CreateNewFile( m_NewGridInfo.NumVar, m_NewGridInfo.NumRec, m_NewGridInfo.NumRep);	
		m_pPGNewGrid->SetFileName(m_NewGridInfo.FileName);
		m_NewGridInfo.Assigned = true;
	};
	SetNewCleanSheetWithNumber(8);
	SetNumberCols(7);

	if(!m_NewGridInfo.Assigned) SetDefNewPGFileInfo();
	
	if (!m_pPGNewGrid->SetAllLongTimesFromMinDiff()) return false;


	SetPGFileColumns(7, -1, true);

	SetNumberRows(1);


	m_NewOutputNewGridPGFileButton.pPG = m_pPGNewGrid;
	SetPGFileColumnsInfoButton(m_NewOutputNewGridPGFileButton,0,7);
	//SetPGFileColumnsInfo(m_pPGNewGrid, 0, 7, 0, true, true);
	SetCellClickButton(m_CreateNewEmptyPGFile);
	SetCellClickButton(m_QuitButton);
	RedrawAll();


	return true;
}
bool GridPGDoc::Make_NewGrid_Post() {
	CUGCell cell;
	SetNewCleanSheetWithNumber(8);
	SetNumberCols(6);
	SetPGFileColumns(6, -1, true);
	if (m_pPGNewGrid == nullptr) m_pPGNewGrid = new CPG();
	SetNumberRows(1);
	SetPGFileColumnsInfo(m_pPGNewGrid, 0, 6, 0, false, true);

	RedrawAll();
	return true;
}
bool GridPGDoc::Make_ImportFromCSV_File_Post() {
	SetNewCleanSheetWithNumber(8);
	for (size_t i = 0; i < 9; i++) {
		UnJoinCells(i, -2); UnJoinCells(i, -1); UnJoinCells(i, -0);
		UnJoinCells(i, 0);
	};
	SetLocalWindowText("Converted Files Ready for Use");
	m_ImportedFiles.VectorNames = m_PGFileImport.m_SuccessFullImportedFileNames;

	SetNumberRows(m_ImportedFiles.VectorNames.size());
	SetNumberCols(2);
	SetColWidth(0, 400);
	SetColWidth(-1, 20);


	QuickSetHeadingText(0, -1, "Click on File Name below to Open Created PG files");
	m_ImportedFiles.VectorNames = m_PGFileImport.m_SuccessFullImportedFileNames;
	SetCellClickButton(m_ImportedFiles);

	SetCellClickButton(m_QuitImportButton);

	SetSH_Width(20);

	RedrawAll();
		return true;
}
bool GridPGDoc::Make_ImportFromCSV_File_Prior(bool ScanningToMake, bool CreatePGFile)
{
	CUGCell cell;
	int StartLineForOutputFileInfo = 3;
	int Num_OutputInfo_lines = 13;
	SetNewCleanSheetWithNumber(8);
	SetLocalWindowText("Importing information for Text Files with CSV-Format or other options");
	bool SuccessfulNewFileImported = false;

	auto CheckImportedFiles = [&]() {if (m_PGFileImport.m_SuccessFullImportedFileNames.size() > 0) SuccessfulNewFileImported = true;};

	CheckImportedFiles();
	int col_scanning = 0;
	int col_filecreate = 0;

auto SetNumColandRows = [&]() {
		auto numrow = 0;
		int Num_basic_info_line = 3;
		int Num_ExtraFile_line = 4;

		for (size_t i = 0; i < 11; i++) { UnJoinCells(i, -2); UnJoinCells(i, -1); UnJoinCells(i, -0);
		UnJoinCells(i, 0);
		};
		if (m_PGFileImport.m_ScanningMade) {
			numrow = Num_basic_info_line + Num_OutputInfo_lines;
			for (size_t filecount = 0; filecount < m_PGFileImport.AllFileInfos.size(); filecount++) {
				if (m_PGFileImport.m_CurrentFileInfo.LinkedSites.size() > 0)
					numrow += m_PGFileImport.m_CurrentFileInfo.LinkedSites.size() ;
			}
			numrow++;
		}
	if (m_PGFileImport.m_ExtraFileInfo.FirstValidHeads.size() > 0) {
		numrow += Num_ExtraFile_line;	
		StartLineForOutputFileInfo += Num_ExtraFile_line;
	}
	
	/*if (SuccessfulNewFileImported) {
		numrow = 1 + m_PGFileImport.m_SuccessFullImportedFileNames.size();
	}*/

	SetNumberRows(numrow);
	SetRowHeight(-1, 40);
	SetTH_NumberRows(2);
	SetTH_RowHeight(-1, 20);
	SetTH_RowHeight(-2, 20);

	auto numcol = m_PGFileImport.m_CurrentFileInfo.ValidVariables.size();
	if (numcol < 2) numcol = 2;
	col_scanning = 1;
	col_filecreate = col_scanning + 1;


	JoinCells(0, 0, 2, 0);
	if(!SuccessfulNewFileImported&&m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.size() > 0&& m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.size()<12) numcol = 12;
	SetNumberCols(numcol);	
	for (size_t i = 0; i < numcol; i++) {
		if (i<10) SetColWidth(i, 150);
		else SetColWidth(i, 90);
		UnJoinCells(i, 0);
		UnJoinCells(i, -1);
	}
};
	SetNumColandRows();
	
	QuickSetHeadingText(0, -1, ""); QuickSetHeadingText(2, -1, "");
	SetCellClickButton(m_ScanFileButton);



	QuickSetText(0, 0, m_PGFileImport.m_filename);
	JoinCells(0, 0, 2, 0);
	auto ScanInfoForHeaderLine = [&]() {
		QuickSetText(3, -1, "Header Lines");
		QuickSetText(4, -1, "Total Lines");
		QuickSetText(5, -1, "Total Columns");
		QuickSetText(6, -1, "Selected Variables");

		QuickSetText(10, -1, "Selected Format");
		JoinCells(10, -1, 11, -1);
 		if (m_PGFileImport.m_CurrentFileInfo.LinkedSites.size()>1) QuickSetText(7, -1, "Number of Sites");
		QuickSetNumber(3, 0, m_PGFileImport.m_CurrentFileInfo.NumHeads);
		QuickSetNumber(4, 0, m_PGFileImport.m_CurrentFileInfo.TotRec);
		QuickSetNumber(5, 0, m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.size());
		QuickSetNumber(6, 0, m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.size() - m_PGFileImport.m_CurrentFileInfo.RedNumVar);
		if (m_PGFileImport.m_CurrentFileInfo.NumHeads == 0) {
			QuickSetText(8, -1, "Start Date");
			QuickSetText(9, -1, "Time Interval");
			QuickSetText(8, 0, PGUtil::STD_StringDatum(m_PGFileImport.m_CurrentFileInfo.IdentifiedStartMin), false, m_ScanStartDate.param);
			QuickSetText(9, 0, FUtil::STD_ItoAscii(m_PGFileImport.m_CurrentFileInfo.IdentifiedMinDiff), false, m_ScanMinDiff.param);
			QuickSetBackColor(8, 0, m_ColorEdit);
			QuickSetBackColor(9, 0, m_ColorEdit);
		}
		else {
			QuickSetText(8, -1, "");
			QuickSetText(9, -1, "");
			QuickSetText(8, 0, "");
			QuickSetText(9, 0, "");
			QuickSetBackColor(8, 0, m_NoColor);
			QuickSetBackColor(9, 0, m_NoColor);

		}
		m_ClickBoxFormatButton.NormalState = true;
		m_ClickBoxFormatButton.Text = m_IdentifiedFormatButton.VectorNames[0];
		if (m_PGFileImport.m_CurrentFileInfo.IdentifiedFormat<m_IdentifiedFormatButton.VectorNames.size())
			m_ClickBoxFormatButton.Text = m_IdentifiedFormatButton.VectorNames[m_PGFileImport.m_CurrentFileInfo.IdentifiedFormat];

		SetCellClickButton(m_ClickBoxFormatButton);
		if (m_PGFileImport.m_CurrentFileInfo.LinkedSites.size()>1) QuickSetNumber(7, 0, m_PGFileImport.m_CurrentFileInfo.LinkedSites.size());
		size_t count_timeIds = 0;
		if (m_PGFileImport.m_CurrentFileInfo.DateColumn != std::string::npos) {
			QuickSetText(0, 1, "DateColumn:" + FUtil::STD_ItoAscii(m_PGFileImport.m_CurrentFileInfo.DateColumn),true,m_ScanTimeId_Date.param);
			QuickSetBackColor(0, 1, m_ColorInfo);

			count_timeIds++;
		}
		if (m_PGFileImport.m_CurrentFileInfo.YearColumn != std::string::npos) {
			QuickSetText(count_timeIds, 1, "YearColumn:" + FUtil::STD_ItoAscii(m_PGFileImport.m_CurrentFileInfo.YearColumn),true, m_ScanTimeId_Year.param);
			QuickSetBackColor(count_timeIds, 1, m_ColorInfo);
			count_timeIds++;
		}
		if (m_PGFileImport.m_CurrentFileInfo.MonthColumn != std::string::npos) {
			QuickSetText(count_timeIds, 1, "MonthColumn:" + FUtil::STD_ItoAscii(m_PGFileImport.m_CurrentFileInfo.MonthColumn), true, m_ScanTimeId_Month.param);
			QuickSetBackColor(count_timeIds, 1, m_ColorInfo); count_timeIds++;
		}
		if (m_PGFileImport.m_CurrentFileInfo.DayNumberColumn != std::string::npos) {
			QuickSetText(count_timeIds, 1, "DayNumberColumn:" + FUtil::STD_ItoAscii(m_PGFileImport.m_CurrentFileInfo.DayNumberColumn), true, m_ScanTimeId_DayNo.param);
			QuickSetBackColor(count_timeIds, 1, m_ColorInfo); count_timeIds++;
		}
		if (m_PGFileImport.m_CurrentFileInfo.DayColumn != std::string::npos) {
			QuickSetText(count_timeIds, 1, "DayColumn:" + FUtil::STD_ItoAscii(m_PGFileImport.m_CurrentFileInfo.DayColumn), true, m_ScanTimeId_Day.param);
			QuickSetBackColor(count_timeIds, 1, m_ColorInfo); count_timeIds++;
		}
		if (m_PGFileImport.m_CurrentFileInfo.TimeColumn != std::string::npos) {
			QuickSetText(count_timeIds, 1, "TimeColumn:" + FUtil::STD_ItoAscii(m_PGFileImport.m_CurrentFileInfo.TimeColumn), true, m_ScanTimeId_Time.param);
			QuickSetBackColor(count_timeIds, 1, m_ColorInfo); count_timeIds++;
		}
		if (m_PGFileImport.m_CurrentFileInfo.HourColumn != std::string::npos) {
			QuickSetText(count_timeIds, 1, "HourColumn:" + FUtil::STD_ItoAscii(m_PGFileImport.m_CurrentFileInfo.HourColumn), true, m_ScanTimeId_Hour.param);
			QuickSetBackColor(count_timeIds, 1, m_ColorInfo); count_timeIds++;
		}
		if (m_PGFileImport.m_CurrentFileInfo.MinuteColumn != std::string::npos) {
			QuickSetText(count_timeIds, 1, "MinuteColumn:" + FUtil::STD_ItoAscii(m_PGFileImport.m_CurrentFileInfo.MinuteColumn), true, m_ScanTimeId_Minute.param);
			QuickSetBackColor(count_timeIds, 1, m_ColorInfo); count_timeIds++;
		}
		QuickSetText(count_timeIds, 1, "SiteIds:" + FUtil::STD_ItoAscii(m_PGFileImport.m_CurrentFileInfo.LinkedSites.size()), true, m_ScanSiteId.param);
		QuickSetBackColor(count_timeIds, 1, m_ColorInfo);
		if (m_PGFileImport.m_CurrentFileInfo.LinkedSites.size() > 0) {

				m_SiteInfoButton.NormalState = m_PGFileImport.m_CurrentFileInfo.EqualSizeOfSiteRecords;
				SetCellClickButton(m_SiteInfoButton, count_timeIds-1);	
				count_timeIds++;
		}
		if (m_PGFileImport.m_CurrentFileInfo.IdentifiedDelimiter == std::string::npos|| m_PGFileImport.m_CurrentFileInfo.IdentifiedDelimiter>3)
			m_VarSeparationButton.Text = "No Variable separator";
		else
			m_VarSeparationButton.Text= m_VarSeparationButton.VectorNames[m_PGFileImport.m_CurrentFileInfo.IdentifiedDelimiter];
		SetCellClickButton(m_VarSeparationButton, count_timeIds );
	
		if (m_PGFileImport.m_CurrentFileInfo.IdentifiedLineEnd == 1) m_LineSeparationButton.NormalState = true;
		else m_LineSeparationButton.NormalState = false;
		SetCellClickButton(m_LineSeparationButton, count_timeIds);

		if (m_PGFileImport.m_CurrentFileInfo.IdentifiedFormat >=m_IdentifiedFormatButton.VectorNames.size()) 
			m_IdentifiedFormatButton.NormalState = false;
		else {
			m_IdentifiedFormatButton.NormalState = true;
			m_IdentifiedFormatButton.Text = m_IdentifiedFormatButton.VectorNames[m_PGFileImport.m_CurrentFileInfo.IdentifiedFormat];
		}
		SetCellClickButton(m_IdentifiedFormatButton, count_timeIds);


		m_FilterVariableButton.VectorNames.resize(m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.size());
		for(size_t i = 0; i < m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.size(); i++) {
			m_FilterVariableButton.VectorNames[i] = m_PGFileImport.m_CurrentFileInfo.FirstValidHeads[i];
		}	

		m_FilterVariableButton.VectorNames.insert(m_FilterVariableButton.VectorNames.begin(), "No Use of Filter");
		if (m_PGFileImport.m_CurrentFileInfo.FilterColumnToUse != std::string::npos)
			m_FilterVariableButton.Text = m_FilterVariableButton.VectorNames[m_PGFileImport.m_CurrentFileInfo.FilterColumnToUse];
		else
			m_FilterVariableButton.Text = "No Use of Filter";

		SetCellClickButton(m_FilterVariableButton, count_timeIds);

		if (m_PGFileImport.m_CurrentFileInfo.FilterColumnToUse != std::string::npos) {
			m_FilterUseButton.Text = m_PGFileImport.m_CurrentFileInfo.FilterItemToUse;
			m_FilterUseButton.VectorNames.resize(m_PGFileImport.m_CurrentFileInfo.GlobalFilterItems.size());

			for (size_t i = 0; i < m_PGFileImport.m_CurrentFileInfo.GlobalFilterItems.size(); i++) {
				m_FilterUseButton.VectorNames[i] = m_PGFileImport.m_CurrentFileInfo.GlobalFilterItems[i];
			}
			SetCellClickButton(m_FilterUseButton, count_timeIds);
		}


	};

	if (m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.size() > 0) {
		m_OpenFileButton.NormalState = SuccessfulNewFileImported;
		SetCellClickButton(m_OpenFileButton);
		SetCellClickButton(m_QuitButton);

		if (m_PGFileImport.m_CurrentFileInfo.RedNumVar > m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.size() / 2)
			m_SelectVariableButton.NormalState = true;
		else
			m_SelectVariableButton.NormalState = false;

		SetCellClickButton(m_SelectVariableButton);
		if (m_PGFileImport.m_CurrentFileInfo.LinkedSites.size() > 1) {
			SetCellClickButton(m_SiteInfoFileButton);

		}	

		ScanInfoForHeaderLine();
	}

	auto SetInfoOfScannedFile = [&]() {
		SetSH_Width(130);
		QuickSetText(-1, 0, "File Name");
		QuickSetText(-1, 1, "Time/Structure");
		if(m_PGFileImport.m_CurrentFileInfo.NumHeads>0)
			QuickSetText(-1, 2, "Headings");
		else
			QuickSetText(-1, 2, "First Line Values");

		QuickSetText(-1, 3, "Extra File Name");
		QuickSetText(-1, 4, "Extra Info");
		QuickSetText(-1, 5, "Add Info");
		QuickSetText(-1, 6, "Linkage");

		m_SelectedOutputVarButton.VectorNames.resize(m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.size());
		for(size_t i = 0; i < m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.size(); i++) {
			m_SelectedOutputVarButton.VectorNames[i] = m_PGFileImport.m_CurrentFileInfo.FirstValidHeads[i];
		}
		m_SelectedOutputVarButton.VectorState = m_PGFileImport.m_CurrentFileInfo.ValidVariables;
		SetCellClickButton(m_SelectedOutputVarButton);

		if (m_PGFileImport.m_ExtraFileScanning) {
			QuickSetText(0, 3, m_PGFileImport.m_ExtraInfoFileName);
			JoinCells(0, 3, 2, 3);
			QuickSetBackColor(0, 3, m_ColorInfo);
			QuickSetText(3, 3, FUtil::STD_ItoAscii(m_PGFileImport.m_ExtraFileInfo.TotRec-1)+" Lines in Extra Info File with"+ 
				FUtil::STD_ItoAscii(m_PGFileImport.m_ExtraFileInfo.FirstValidHeads.size())+ " columns", true);
			JoinCells(3, 3, 8, 3);
			QuickSetBackColor(3, 3, m_ColorInfo);

		}

		m_IdentifiedVarSiteInfoButton.VectorNames.resize(m_PGFileImport.m_ExtraFileInfo.FirstValidHeads.size());
		for(size_t i = 0; i < m_PGFileImport.m_ExtraFileInfo.FirstValidHeads.size(); i++) {
			m_IdentifiedVarSiteInfoButton.VectorNames[i] = m_PGFileImport.m_ExtraFileInfo.FirstValidHeads[i];
		}
		SetCellClickButton(m_IdentifiedVarSiteInfoButton);

		QuickSetHeadingText(0, 5, "SiteName"); QuickSetHeadingText(1, 5, "Country"); QuickSetHeadingText(2, 5, "Latitute"); QuickSetHeadingText(3, 5, "Longitude"); QuickSetHeadingText(4, 5, "Altitude");
		if (m_PGFileImport.m_ExtraFileInfo.FirstValidHeads.size()>0)
			for (size_t i = 0; i < 5; i++) {
				switch(i) {
				 case 0: m_ComboIdButton.VectorNames.resize(m_PGFileImport.m_ExtraFileInfo.FirstValidHeads.size());
					 for(size_t i = 0; i < m_PGFileImport.m_ExtraFileInfo.FirstValidHeads.size(); i++) {
						 m_ComboIdButton.VectorNames[i] = m_PGFileImport.m_ExtraFileInfo.FirstValidHeads[i];
					 }
					 m_ComboIdButton.Text = m_PGFileImport.m_ExtraFileInfo.FirstValidHeads[m_PGFileImport.AddInfoLinkIndex[i]];
					 SetCellClickButton(m_ComboIdButton);
					 break;
				 case 1: m_ComboCountryButton.VectorNames.resize(m_PGFileImport.m_ExtraFileInfo.FirstValidHeads.size());
					 for(size_t i = 0; i < m_PGFileImport.m_ExtraFileInfo.FirstValidHeads.size(); i++) {
						 m_ComboCountryButton.VectorNames[i] = m_PGFileImport.m_ExtraFileInfo.FirstValidHeads[i];
					 }
					 m_ComboCountryButton.Text = m_PGFileImport.m_ExtraFileInfo.FirstValidHeads[m_PGFileImport.AddInfoLinkIndex[i]];
					 SetCellClickButton(m_ComboCountryButton);
					 break;
				 case 2: m_ComboLatButton.VectorNames.resize(m_PGFileImport.m_ExtraFileInfo.FirstValidHeads.size());
					 for(size_t i = 0; i < m_PGFileImport.m_ExtraFileInfo.FirstValidHeads.size(); i++) {
						 m_ComboLatButton.VectorNames[i] = m_PGFileImport.m_ExtraFileInfo.FirstValidHeads[i];
					 }
					 m_ComboLatButton.Text = m_PGFileImport.m_ExtraFileInfo.FirstValidHeads[m_PGFileImport.AddInfoLinkIndex[i]];
					 SetCellClickButton(m_ComboLatButton);
					 break;
				 case 3: m_ComboLongButton.VectorNames.resize(m_PGFileImport.m_ExtraFileInfo.FirstValidHeads.size());
					 for(size_t i = 0; i < m_PGFileImport.m_ExtraFileInfo.FirstValidHeads.size(); i++) {
						 m_ComboLongButton.VectorNames[i] = m_PGFileImport.m_ExtraFileInfo.FirstValidHeads[i];
					 }
					 m_ComboLongButton.Text = m_PGFileImport.m_ExtraFileInfo.FirstValidHeads[m_PGFileImport.AddInfoLinkIndex[i]]; 
					 SetCellClickButton(m_ComboLongButton);
					 break;
				 case 4: m_ComboAltButton.VectorNames.resize(m_PGFileImport.m_ExtraFileInfo.FirstValidHeads.size());
					 for(size_t i = 0; i < m_PGFileImport.m_ExtraFileInfo.FirstValidHeads.size(); i++) {
						 m_ComboAltButton.VectorNames[i] = m_PGFileImport.m_ExtraFileInfo.FirstValidHeads[i];
					 }	
					 m_ComboAltButton.Text = m_PGFileImport.m_ExtraFileInfo.FirstValidHeads[m_PGFileImport.AddInfoLinkIndex[i]]; 
					 SetCellClickButton(m_ComboAltButton);
					 break;
				};
			};

	};
	if (m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.size() > 0) SetInfoOfScannedFile();

	Make_OutputFileInformationLines(StartLineForOutputFileInfo,  Num_OutputInfo_lines);


	RedrawAll();
	return true;
}
void GridPGDoc::Make_OutputFileInformationLines(int startline, int linetomake)
{
	
	auto FirstLine = [&]() {
		for (size_t i = 0; i < GetNumberCols(); i++) UnJoinCells(i, startline);
		JoinCells(0, startline, GetNumberCols()-1, startline);
		QuickSetHeadingText(0, startline, "Options to add information into the new file");
		m_ApplyNameFromHeadingButton.Row = startline;
		SetCellClickButton(m_ApplyNameFromHeadingButton);
		m_ApplyUnitFromHeadingButton.Row = startline;
		SetCellClickButton(m_ApplyUnitFromHeadingButton);
		m_ApplyUnitFromUnitTypeButton.Row = startline;
		SetCellClickButton(m_ApplyUnitFromUnitTypeButton);
		m_ApplyNameFromElementTypeButton.Row = startline;
		SetCellClickButton(m_ApplyNameFromElementTypeButton);
		QuickSetText(-1, startline + 1, "Variables");
		m_NameOfHeadingLine.Row = startline + 1;
		m_NameOfHeadingLine.VectorNames.clear();
		for (size_t i = 0; i < m_PGFileImport.m_CurrentFileInfo.ValidVariables.size(); ++i)
			if (m_PGFileImport.m_CurrentFileInfo.ValidVariables[i]&& m_PGFileImport.m_CurrentFileInfo.FirstValidHeads.size()>i)
				m_NameOfHeadingLine.VectorNames.push_back(m_PGFileImport.m_CurrentFileInfo.FirstValidHeads[i]);

		SetCellClickButton(m_NameOfHeadingLine);
	};
	auto SecondLine = [&]() {
		QuickSetText(-1, startline + 2, "Type of Element");
		m_ElementTypeNewVariables.Row = startline + 2;
		size_t countvalid = 0;
		m_PGFileImport.m_NewVarIndexSelected.clear();
		for (size_t i = 0; i < m_PGFileImport.m_CurrentFileInfo.ValidVariables.size(); ++i)
			if (m_PGFileImport.m_CurrentFileInfo.ValidVariables[i]|| m_PGFileImport.m_CurrentFileInfo.CopyMode) {
				m_PGFileImport.m_NewVarIndexSelected.push_back(i);
				elements elem = m_PGFileImport.m_CurrentFileInfo.Des.addinfo[i].Element;
				if (size_t(elem) >= m_ElementTypeNewVariables.VectorNames.size())
						elem = elements::GENERAL;

				m_ElementTypeNewVariables.Text = p_ModelInfo->GetStringOfElement(elem);
				SetCellClickButton(m_ElementTypeNewVariables, countvalid);
				countvalid++;
			}
	};
	auto ThirdLine = [&]() {
		QuickSetText(-1, startline + 3, "Type of Units");
		m_UnitTypeNewVariables.Row = startline + 3;
		size_t countvalid = 0;
		for (size_t i = 0; i < m_PGFileImport.m_CurrentFileInfo.ValidVariables.size(); ++i)
			if (m_PGFileImport.m_CurrentFileInfo.ValidVariables[i]) {
				
				vector<string_view> previous = m_UnitTypeNewVariables.VectorNames;
				vector<UNIT_TYPES> types=GetUnitOptions(m_PGFileImport.m_CurrentFileInfo.Des.addinfo[i].Element);

				if (types.size() > 0) m_UnitTypeNewVariables.VectorNames =p_ModelInfo->GetSelectedStringBasedUnits(types);
				else m_UnitTypeNewVariables.VectorNames = p_ModelInfo->GetAllStringBasedUnits();
				m_UnitTypeNewVariables.Text = p_ModelInfo->GetUnitString(m_PGFileImport.m_CurrentFileInfo.Des.addinfo[i].Unit);
				SetCellClickButton(m_UnitTypeNewVariables, countvalid);
				m_UnitTypeNewVariables.VectorNames = previous;
				countvalid++;
			}
	};
	FirstLine();
	SecondLine();
	ThirdLine();

	vector<string_view> NewItems; size_t count=0;
	NewItems.push_back("Name"); NewItems.push_back("Unit"); NewItems.push_back("Id"); NewItems.push_back("Position");
	NewItems.push_back("Country"); NewItems.push_back("Station"); NewItems.push_back("Altitude"); NewItems.push_back("Latitude");
	NewItems.push_back("Longitude");
	auto SetLineofDescriptiontoSelectedOutputVariables = [&](int DescriptorNo) {
		size_t countvar = 0;
		for_each(m_PGFileImport.m_NewVarIndexSelected.begin(), m_PGFileImport.m_NewVarIndexSelected.end(), [&](int &num) {
			string Text;
			switch (DescriptorNo) {
			case 0:
				Text = m_PGFileImport.m_CurrentFileInfo.Des.base.Name[num];
				break;
			case 1:
				Text = m_PGFileImport.m_CurrentFileInfo.Des.base.Unit[num];
				break;
			case 2:
				Text = m_PGFileImport.m_CurrentFileInfo.Des.base.Id[num];
				break;
			case 3:
				Text = m_PGFileImport.m_CurrentFileInfo.Des.base.Pos[num];
				break;
			case 4:
				Text = m_PGFileImport.m_CurrentFileInfo.Des.addinfo[num].CountryCode;
				break;
			case 5:
				Text = m_PGFileImport.m_CurrentFileInfo.Des.addinfo[num].StationName;
				break;
			case 7:
				Text = FUtil::STD_DtoAscii(m_PGFileImport.m_CurrentFileInfo.Des.addinfo[num].Lat);
				break;
			case 8:
				Text = FUtil::STD_DtoAscii(m_PGFileImport.m_CurrentFileInfo.Des.addinfo[num].Long);
				break;
			case 6:
				Text = FUtil::STD_DtoAscii(m_PGFileImport.m_CurrentFileInfo.Des.addinfo[num].Alt);
				break;
			};
			QuickSetText(countvar, startline + 4 + DescriptorNo, Text,false, m_DescriptionImportedFile.param);
			countvar++;
		});
	};
	for_each (NewItems.begin(),NewItems.end(),[&]
		(const std::string&str){		
		QuickSetText(-1, startline + 4+count,str );	
		SetLineofDescriptiontoSelectedOutputVariables(count);
		count++;
	});

	size_t start = startline + 4 + count;
	
	auto SetFilesTobeCreatedLines = [&]() {
		bool DefFilterMade = false;
		size_t ExtraItems = 0;
		if (m_PGFileImport.m_CurrentFileInfo.LinkedSites.size() > 0) {
			if(m_PGFileImport.m_CurrentFileInfo.LinkedSites.front().FilterItems.size()>0) 
				DefFilterMade = true;
		}
		if (DefFilterMade) {
			LINKED_FILEINFO link;
			for_each(m_PGFileImport.m_CurrentFileInfo.LinkedSites.begin(), m_PGFileImport.m_CurrentFileInfo.LinkedSites.end(), [&](auto &link) {
				if (link.FilterItems.size() > ExtraItems) ExtraItems = link.FilterItems.size();
			});
		}

		QuickSetHeadingText(-1, start, "File No");
	
		QuickSetHeadingText(0, start, "Site Key");
		QuickSetHeadingText(1, start, "Input Records");
		size_t ExtraCol = 0;
		bool TimeDay = true;
		QuickSetHeadingText(2+ExtraCol, start, "Unique Records",true, m_ScanUniqueStep.param);
		QuickSetHeadingText(3+ExtraCol, start, "Record Step", true, m_ScanRecordStep.param);
		QuickSetHeadingText(4 + ExtraCol, start, "Common Step", true, m_ScanCommonStep.param);

		if (m_PGFileImport.m_CurrentFileInfo.LinkedSites.size() > 0) {
			QuickSetHeadingText(5 + ExtraCol, start, "Start Date");
			if (m_PGFileImport.m_CurrentFileInfo.LinkedSites.front().TimeCheck.CommonTimeStep) {

				if (m_PGFileImport.m_CurrentFileInfo.LinkedSites.front().TimeCheck.ValueStep > 1440) {
					QuickSetHeadingText(6 + ExtraCol, start, "Time Step (days)");
					TimeDay = true;
				}
				else {
					QuickSetHeadingText(6 + ExtraCol, start, "Time Step (Minutes)");
					TimeDay = false;
				}


			}
			else {
				QuickSetHeadingText(6 + ExtraCol, start, "End Date");

			};
			if (DefFilterMade) {
				QuickSetHeadingText(7, start, "Output Records");
				QuickSetHeadingText(8, start, "No Filter Items");
				for (size_t i = 0; i < ExtraItems; i++) {
					string str = "Item ("; str += FUtil::STD_ItoAscii(i + 1) + ")";
					QuickSetHeadingText(9 + i, start, str);
				}
				ExtraCol = 2 + ExtraItems;
			}
		}

		
			LINKED_FILEINFO link;
			size_t linecount=1;
			auto saveinfo = m_PGFileImport.m_CurrentFileInfo;
			for (size_t filecount = 0; filecount < m_PGFileImport.AllFileInfos.size(); filecount++) {
				m_PGFileImport.m_CurrentFileInfo = m_PGFileImport.AllFileInfos[filecount];
	
				size_t count = 1;
				for_each(m_PGFileImport.m_CurrentFileInfo.LinkedSites.begin(), m_PGFileImport.m_CurrentFileInfo.LinkedSites.end(), [&](LINKED_FILEINFO &link) {
					if (link.Ids != std::string::npos)
						QuickSetText(0, start + linecount, FUtil::STD_ItoAscii(link.Ids));
					else
						QuickSetText(0, start + linecount, m_PGFileImport.m_CurrentFileInfo.StatNo);

					QuickSetNumber(1, start + linecount, m_PGFileImport.m_CurrentFileInfo.LinkedSites[count - 1].Records);


					size_t n = link.TimeCheck.NumUniqueSteps;
					QuickSetNumber(2, start + linecount, n);
					n = link.TimeCheck.RecordStep;
					QuickSetNumber(3, start + linecount, n);
					bool valid = link.TimeCheck.CommonTimeStep;
					string Txt;
					if (valid) Txt = "Yes";
					else Txt = "No";
					QuickSetText(4, start + linecount, Txt);

					if (!valid) QuickSetBackColor(4, start + linecount, m_ColorEdit);
					else QuickSetBackColor(4, start + linecount, m_NoColor);

					QuickSetText(5, start + linecount, PGUtil::STD_StringDatum(link.TimeCheck.ValueStart));
					if (m_PGFileImport.m_CurrentFileInfo.LinkedSites.front().TimeCheck.ValueStep != std::string::npos) {
						if (TimeDay)
							QuickSetNumber(6, start + linecount, size_t(m_PGFileImport.m_CurrentFileInfo.LinkedSites.front().TimeCheck.ValueStep / 1440));
						else
							QuickSetNumber(6, start + linecount, size_t(m_PGFileImport.m_CurrentFileInfo.LinkedSites.front().TimeCheck.ValueStep));
						if (DefFilterMade) {
							QuickSetNumber(7, start + linecount, link.RecordsSelected);
							QuickSetNumber(8, start + linecount, link.FilterItems.size());
							for (size_t i = 0; i < ExtraItems; i++) {
								if (link.FilterItems.size() > i)
									QuickSetText(9 + i, start + linecount, link.FilterItems[i]);
								else
									QuickSetText(9 + i, start + linecount, "");
							}
						}
					}
					else {
						QuickSetText(6, start + linecount, PGUtil::STD_StringDatum(link.TimeCheck.ValueEnd));
					}
					QuickSetNumber(-1, start + linecount, linecount);
					count++;
					linecount++;
				});
			}
			if (m_PGFileImport.AllFileInfos.size() > 0) m_PGFileImport.m_CurrentFileInfo = saveinfo;

	};
	if(m_PGFileImport.m_CurrentFileInfo.NumHeads>0) SetFilesTobeCreatedLines();



}
int GridPGDoc::OnMenuStart(int col, long row, int section)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(section);

	CUGCell cell, cell1, cell2;
	CString Group, type, Name;


	long ParV;
	CString optionlist;

	int sheet = GetSheetNumber();
	EmptyMenu();
	if (sheet == 1) {

		int nsel;
		int nx = MySingle::instance().GetChartPropPointer()->m_NumChartX;
		int ny = MySingle::instance().GetChartPropPointer()->m_NumChartY;
		if (m_pSimDoc != nullptr)
			nsel = m_pSimDoc->m_SelectPGFile.size();
		else
			nsel = 0;
		if (nsel > 0)
			AddMenuItem(4011, "Clear the Selected (" + FUtil::STD_ItoAscii(nsel) + ") PG file variables");
		AddMenuItem(4006, "Add from (" + FUtil::STD_ItoAscii(nx) + ") to (" + FUtil::STD_ItoAscii(nx + 1) + ") horizontal charts");
		if (nx > 1)
			AddMenuItem(4007, "Reduce from (" + FUtil::STD_ItoAscii(nx) + ") to (" + FUtil::STD_ItoAscii(nx - 1) + ") horizontal charts");
		AddMenuItem(4008, "Add from (" + FUtil::STD_ItoAscii(ny) + ") to (" + FUtil::STD_ItoAscii(ny + 1) + ") vertical charts");
		if (ny > 1)
			AddMenuItem(4009, "Reduce from (" + FUtil::STD_ItoAscii(ny) + ") to (" + FUtil::STD_ItoAscii(ny - 1) + ") vertical charts");

		if (m_RepFirst)
			AddMenuItem(1010, "Show the different Variable name first");
		else
			AddMenuItem(1010, "Show the different Repetitions");


		AddMenuItem(3001, "Make Topograhic Map");

	}
	else if (sheet == 2) { // Single Coordinate

		if (m_SpatialSourceFileReady)
			AddMenuItem(3028, "Create New PG File represention new location based on interpolated values from current file");


	}
	else if (sheet == 3) { ///Grid Coordinate
		if (m_SpatialSourceFileReady)
			AddMenuItem(3029, "Create New PG File represention new locations within a rectangle based on interpolated values from current file");

	}
	else if (sheet == 4) { ///Grid Coordinate
		AddMenuItem(3027, "Make Interpolation of all variables based on regressions");
		AddMenuItem(3031, "Replace Missing data by linear interpolation");
		AddMenuItem(3032, "Replace Missing data by artifical random selection of data");
		if (m_spatialPG.m_statsub.size() > 0)
			if (!m_ShowSubResults)
				AddMenuItem(3050, "Show Results from Substitutions made");
			else {
				AddMenuItem(1997, "Copy to Clipboard");
				AddMenuItem(3050, "Do Not Show Results from Substitutions made");


			}
	}
	else if (sheet == 5) {
		if (!m_ShowSubResults)
			AddMenuItem(3050, "Show Results from previous calculation");
		else {
			AddMenuItem(1997, "Copy to Clipboard");
			AddMenuItem(3050, "Shift to menu for new calculations to be made");
			CString txt; txt = "Open File Calculated :";
			txt += m_TimeAggregate.m_DestFileName.c_str();
			AddMenuItem(3051, txt);
		}
	}
	else if (sheet == 6) {
		if (!m_ShowSubResults)
			AddMenuItem(3050, "Show Results from previous calculation");
		else {
			AddMenuItem(1997, "Copy to Clipboard");
			AddMenuItem(3050, "Shift to menu for new calculations to be made");
			CString txt; txt = "Open File Calculated :";
			txt += m_ChangePeriod.m_DestFileName.c_str();
			AddMenuItem(3051, txt);

		}
	}
	else if (sheet == 0) {
		if (section == UG_GRID) {
			GetCell(col, row, &cell);
			ParV = cell.GetParam();
			GetCell(-1, row, &cell);
			auto varindex =int(cell.GetNumber());
			if (ParV == m_VarSelectBox.param||ParV==m_VarSelectOptions.param) {
				MakePGChartMenu(col, row, varindex);
			}
			else if (ParV ==m_FileSelectOptions.param) {
				GetCell(-2, row, &cell);
				auto index = size_t(cell.GetNumber());
				if (m_pPGArchive->m_PGFileArray.size() == 0) return FALSE;
				CPG *pPG = (CPG*)m_pPGArchive->GetPGFile(index);
				string str = pPG->GetFileName(true);
				AddMenuItem(3002, "Close bin file: " + str);
				AddMenuItem(3003, "Edit bin file: " + str);
				AddMenuItem(3013, "Save bin file: " + str + " as a text (txt) file");
				AddMenuItem(3014, "Save bin file: " + str + " as an excel (xls) file");
				AddMenuItem(3015, "Create new variables and a new PG bin file");

				if (pPG->GetNumRepititions() > 1) {
					if (!pPG->IsAddInfoSet())
						AddMenuItem(3025, "Find Coordinates from NOAA Station file");
					else
						AddMenuItem(3026, "Merge stations with the same koordinates");
					AddMenuItem(3034, "Split PG with repetitions into normal PG Files");
				}
			}
			else if (ParV == 0) {
				MakePGModifyMenu(col, row, varindex);
			}

		}
		else {
			AddMenuItem(3000, "Open a PG bin file");
			AddMenuItem(3001, "Create a PG bin file");
			AddMenuItem(3101, "Create a serie of PG bin file from txt file and mix them into one single PG bin file");
			AddMenuItem(3033, "Import from Net CDF file");
			AddMenuItem(3004, "Open/Edit a PG bin file");
			AddMenuItem(3016, "Merge similar PG files to a new aggregated file");

		}
	}
	else if (sheet == 9 || sheet == 10) {
		if (section == UG_GRID) {
			GetCell(2, row, &cell);

			if ((!m_Data&&col > 1) || m_Data) {
				AddMenuItem(1000, "Cut");
				AddMenuItem(2000, "Copy");
				AddMenuItem(3000, "Paste");
			}
			if (!m_Data&&col == 0) {
				AddMenuItem(1560, "Save All Variables");
				AddMenuItem(1561, "Save All Below");
				AddMenuItem(1562, "Do not save any Variables");
				AddMenuItem(1563, "Do not save any Below");
			}
			if (!m_Data&&col == 1 && m_pPG != nullptr) {
				if (m_pPG->GetNumRepititions() > 1) {
					if (!m_pPG->GetViewOrder())
						AddMenuItem(1660, "View All repetitions prior new variables");
					else
						AddMenuItem(1660, "View All variables prior new repetitions");


				}
				string s;

				s = cell.GetStdText();
				if (!m_pPG->GetToBeViewed(size_t(row))) AddMenuItem(1661, "Enable all repetitions of" + s + " to be viewed");
				else  AddMenuItem(1661, "Disable all repetitions of " + s + " to be viewed");
				if (!m_pPG->GetToBeViewed(size_t(row))) AddMenuItem(1662, "Enable all variables to viewed");
				else  AddMenuItem(1662, "Disable all variables to be viewed");
				AddMenuItem(1663, "Update to view current selection");
			}
			else if (m_Data&&row < 3) {
				if (col == 0) {
					AddMenuItem(1500, "Move Current Date/time backward");
					AddMenuItem(1501, "Move Current Date/time forward");
					AddMenuItem(1502, "Convert Variable to Current Date/time");
					AddMenuItem(1525, "Convert Record No to Current Date/time");

					AddMenuItem(1515, "Derivate (Days) and move to mid of interval");
				}
				else if (col > 0) {
					string str;
					AddMenuItem(1700, "Delete Variable");

					if (m_pPG->GetNumToBeCalc() > 0) {
						str = FUtil::STD_ItoAscii(m_pPG->GetNumToBeCalc());
						AddMenuItem(1702, "Add New Variable(s) based on selected group of " + str + "variable(s)");
						AddMenuItem(1703, "Replace existing variable values for selected group of " + str + " variable(s)");
					}
					else {
						size_t SeqNo;
						size_t ivar = m_pPG->GetViewVariable(col - 1);
						size_t irep = m_pPG->GetViewRepetition(col - 1);
						SeqNo = ivar + m_pPG->GetNumVariables()*(irep - 1);
						if (m_pPG->GetNumRepititions() > 1) {
							str = FUtil::STD_ItoAscii(ivar) + " Repetition No:" + FUtil::STD_ItoAscii(irep);
						}
						else
							str = FUtil::STD_ItoAscii(ivar);
						AddMenuItem(1702, "Add New Variable based on Variable No:" + str);
						AddMenuItem(1703, "Replace existing variable No:" + str);

						if (m_pPG->GetNumRepititions() > 1) {
							str = FUtil::STD_ItoAscii(ivar) + " and all Repetitions";
							AddMenuItem(1704, "Add New Variable based on Variable No:" + str);
							AddMenuItem(1705, "Replace existing variable No:" + str);

						}

					}




				};
			}
		}
		else if (section == UG_SIDEHEADING && !m_Data) {
			if (row >= 3) {

				string str;
				AddMenuItem(1700, "Delete Variable");

				if (m_pPG->GetNumToBeCalc() > 0) {
					str = FUtil::STD_ItoAscii(m_pPG->GetNumToBeCalc());
					AddMenuItem(1702, "Add New Variable(s) based on selected group of " + str + "variable(s)");
					AddMenuItem(1703, "Replace existing variable values for selected group of " + str + " variable(s)");
				}
				else {
					if (m_pPG->GetNumRepititions() > 1) {
						int irep = m_pPG->GetNumVariables();
						int arep = row / irep;
						int num = row%irep;
						str = FUtil::STD_ItoAscii(num+1) + " Repetition No:" + FUtil::STD_ItoAscii(arep+1);
					}
					else
						str = FUtil::STD_ItoAscii(row + 1);
					AddMenuItem(1702, "Add New Variable based on Variable No:" + str);
					AddMenuItem(1703, "Replace existing variable No:" + str);

					if (m_pPG->GetNumRepititions() > 1) {
						int irep = m_pPG->GetNumVariables();
						int arep = row / irep;
						int num = row%irep;
						str = FUtil::STD_ItoAscii(num+1) + " and all Repetitions";
						AddMenuItem(1704, "Add New Variable based on Variable No:" + str);
						AddMenuItem(1705, "Replace existing variable No:" + str);

					}

				}



			}



			};
	}

return TRUE; 
}
void GridPGDoc::MakePGModifyMenu(int col, int row, int varindex) {

	CUGCell cell;
	int selected, vektor;
	string Group, Name, type;
	selected = 0;
	int nsel;
	if (m_pSimDoc != nullptr)
		nsel = m_pSimDoc->m_SelectPGFile.size();
	else
		nsel = 0;

	if (nsel>0) {
		AddMenuItem(4011, "Clear the Selected (" + FUtil::STD_ItoAscii(nsel) + ") PG file variables");

		AddMenuItem(4444, "Save the Selected (" + FUtil::STD_ItoAscii(nsel) + ")  variables to a new PG-file");


	}
	if (m_pDoc != nullptr) {
		size_t IndexSource = m_pPGArchive->GetIndex(m_pDoc);
		CPG *pPG = m_pPGArchive->GetPGFile(IndexSource);
		if (pPG != nullptr) {
			if (col == 5 || col == 6)
				AddMenuItem(7001, "ReCalculate the Min and Max values");
			else if (col == 7) {
				if(pPG->CheckMissingNumbers()) 
				AddMenuItem(7002, "Remove Records if all variables are Missing");

			}


		}
	}
}
void GridPGDoc::MakePGChartMenu(int col, int row, int index) {
		
			CUGCell cell;
			int selected, vektor;
			string Group, Name,type;
			selected=0;

				int nsel;
				if (m_pSimDoc != nullptr)
					nsel = m_pSimDoc->m_SelectPGFile.size();
				else
					nsel = 0;

					if(nsel>0) {
						AddMenuItem(4011,"Clear the Selected ("+FUtil::STD_ItoAscii(nsel)+") PG file variables");

						AddMenuItem(4444,"Save the Selected ("+FUtil::STD_ItoAscii(nsel)+")  variables to a new PG-file");
		

					}
					size_t IndexSource=m_pPGArchive->GetIndex(m_pDoc);
					CPG *pPG=m_pPGArchive->GetPGFile(IndexSource);

					string name;
					name=pPG->GetVarName(index);
					FUtil::trim(name);
					if(!m_pProp->IsFirstIndepSet()) {
							AddMenuItem(5018,"Set ("+name+") as first Independent variable");
							AddMenuItem(6010, "Set Hour of Day as first Independent variable");
							AddMenuItem(6011, "Set Day of Year as first Independent variable");
							AddMenuItem(6012, "Set Year as first Independent variable");
						}
						else {
							//size_t ivarsel=m_pProp->GetFirstIndepIndex();
							if(m_pProp->GetFirstIndepIndex()!=index)
								AddMenuItem(5018,"Select ("+name+") as New first Independent variable");
							if(m_pProp->GetFirstIndepIndex()==index)
								AddMenuItem(5019,"DeSelect ("+name+") as first Independent variable");
							if(m_pProp->GetIndepFirst()== vartype::PGDAY)  AddMenuItem(6016,"Delect Day of Year as First Independent variable");
							if(m_pProp->GetIndepFirst()== vartype::PGHOUR)  AddMenuItem(6015,"Delect Hour of Day as First Independent variable");
							if(m_pProp->GetIndepFirst()== vartype::PGYEAR)  AddMenuItem(6017,"Delect Year as First Independent variable");

							if(!m_pProp->IsSecondIndepSet()) {
								if(m_pProp->GetFirstIndepIndex()!=index)
									AddMenuItem(5028,"Set ("+name+") as Second Independent variable");
								AddMenuItem(6020, "Set Hour of Day as Second Independent variable");
								AddMenuItem(6021, "Set Day of Year as Second Independent variable");
								AddMenuItem(6022, "Set Year as Second Independent variable");
							}
							else {
								if(m_pProp->GetSecondIndepIndex()!=index&&m_pProp->GetFirstIndepIndex()!=index)
									AddMenuItem(5028,"Select ("+name+") as  New Second Independent variable");
								if(m_pProp->GetSecondIndepIndex()==index)
									AddMenuItem(5029,"DeSelect ("+name+") as Second Independent variable");

							if(m_pProp->GetIndepSecond()== vartype::PGDAY)  AddMenuItem(6026,"DeSelect Day of Year as Second Independent variable");
							if(m_pProp->GetIndepSecond()== vartype::PGHOUR)  AddMenuItem(6025,"DeSelect Hour of Day as Second Independent variable");
							if(m_pProp->GetIndepSecond()== vartype::PGYEAR)  AddMenuItem(6027,"DeSelect Year as Second Independent variable");


							}
						}
					



				AddMenuItem(4006, "Add from ("+FUtil::STD_ItoAscii(m_pProp->m_NumChartX)+") to ("+FUtil::STD_ItoAscii(m_pProp->m_NumChartX+1)+") horizontal charts");
				if(m_pProp->m_NumChartX>1)
							AddMenuItem(4007, "Reduce from ("+FUtil::STD_ItoAscii(m_pProp->m_NumChartX)+") to ("+FUtil::STD_ItoAscii(m_pProp->m_NumChartX-1)+") horizontal charts");
				AddMenuItem(4008, "Add from ("+FUtil::STD_ItoAscii(m_pProp->m_NumChartY)+") to ("+FUtil::STD_ItoAscii(m_pProp->m_NumChartY+1)+") vertical charts");
				if(m_pProp->m_NumChartY>1)
							AddMenuItem(4009, "Reduce from ("+FUtil::STD_ItoAscii(m_pProp->m_NumChartY)+") to ("+FUtil::STD_ItoAscii(m_pProp->m_NumChartY-1)+") verticaltal charts");
}
bool GridPGDoc::IsMultiPGRepPlotReady() {
	if((m_pProp->GetIndepFirst()==vartype::GEO_ALT||m_pProp->GetIndepFirst()==vartype::GEO_LAT||m_pProp->GetIndepFirst()==vartype::GEO_LONG)
		&&(m_pProp->GetIndepSecond()==vartype::GEO_ALT||m_pProp->GetIndepSecond()==vartype::GEO_LAT||m_pProp->GetIndepSecond()==vartype::GEO_LONG)
		&&(m_pProp->GetIndepDependent()==vartype::GEO_ALT||m_pProp->GetIndepDependent()==vartype::GEO_LAT||m_pProp->GetIndepDependent()==vartype::GEO_LONG)) 
		return true;
	else
		return false;
}
bool GridPGDoc::IsMultiPGRepPlot() {
	if(m_pProp->GetIndepFirst()==vartype::GEO_ALT||m_pProp->GetIndepFirst()==vartype::GEO_LAT||m_pProp->GetIndepFirst()==vartype::GEO_LONG)
		return true;
	else
		return false;
}
void GridPGDoc::Make_DataHeader()
{
	CPG *pCPG = GetCurrentPG_Pointer();
	if (pCPG == nullptr) return;
	m_Repit = pCPG->GetNumRepititions();
	bool RepModelLayout = false;
	CUGCell cell;
	SetVScrollMode(UG_SCROLLTRACKING);
	if (m_Repit > 1) {
		pCPG->SetRepititionMode(true, m_Repit);
		RepModelLayout = true;

	}
	int cols, rows;
	int LinesOfNewEmbeddedHeading = 3;
	auto AssignNumberOfColandRow = [&]() {
		pCPG->UpdateRecordsToView();
		m_col = pCPG->GetNumToView() + 1;

		m_row = pCPG->GetCalcStop() - pCPG->GetCalcStart()+1;
		if (m_row <= 1) {
			m_row = pCPG->GetNumRecords();
		}
		if (m_col == 1) {
			pCPG->UpdateViewOrder();
			m_col = pCPG->GetNumToView() + 1;
		}

		if (m_col == 0) {
			m_col = pCPG->GetNumVariables()*pCPG->GetNumRepititions() + 1;
			size_t varrep = pCPG->GetNumRepititions() - 1;
			varrep--;
			while (m_col > 1000) {
				m_col = pCPG->GetNumVariables()*varrep + 1;
				m_ShowMaxRep = varrep;
				varrep--;
			}
			m_ShowMaxRep = varrep + 1;
		}
		else
			m_ShowMaxRep = 1; // Preliminary set

		cols = m_col;
		if (m_row > pCPG->GetRecordViewSize()) rows = pCPG->GetRecordViewSize();
		else rows = m_row;
		
	};
	m_NumVar = pCPG->GetNumVariables();


	AssignNumberOfColandRow();


	if (cols < 1) return;


	if(cols>13) 
		SetNumberCols(cols);
	else
		SetNumberCols(13);

		

	SetNumberRows(rows+ LinesOfNewEmbeddedHeading);


	for (int i = 1; i<cols; i++) {
		GetColDefault(i, &cell);
		auto alin = cell.GetAlignment();
		cell.SetAlignment(UG_ALIGNRIGHT);
		SetColDefault(i, &cell);
	}


	string str;
	vector<size_t> joinv, join_varline;

		QuickSetHeadingText(0, 2, "Date");
		if (RepModelLayout)		QuickSetHeadingText(0, 1, "Date");
		else {
			UnJoinCells(0, 1); UnJoinCells(1, 1); UnJoinCells(2, 1);
			QuickSetHeadingText(0, 1, "VarNo");
		}
	

		auto GetRepString = [&](size_t i, bool Complete) {
			str = "R:" + FUtil::STD_ItoAscii(pCPG->GetViewRepetition(i));
			if (Complete) {
				string s;
				s = pCPG->GetVarId(pCPG->GetViewVariable(i), pCPG->GetViewRepetition(i)-1);
				FUtil::trim(s);
				str += "-"; str += s.c_str();
			}
			return str;
		};
		auto GetVarString = [&](size_t i) {
			str = "V:" + FUtil::STD_ItoAscii(pCPG->GetViewVariable(i));
			string s;
			s = pCPG->GetVarName(pCPG->GetViewVariable(i), pCPG->GetViewRepetition(i)-1);
			FUtil::trim(s); 
			if (RepModelLayout) {
				str += "-"; str += s.c_str();
			}
			else
				str =s;
			return str;
		};
		size_t lastrep = 0;
		auto SetJoinVector = [&](size_t i) {
			if (lastrep != pCPG->GetViewRepetition(i)) {
				joinv.push_back(i + 1);
				lastrep = pCPG->GetViewRepetition(i);
			}
		};
		size_t lastvar = 0;
		auto SetJoinVectorVariables = [&](size_t i) {
			if (lastvar != pCPG->GetViewVariable(i)) {
				join_varline.push_back(i + 1);
				lastvar = pCPG->GetViewVariable(i);
			}
		};
		auto SetColorToVariablesMarkedForCalculations = [&](size_t i) {
			if (pCPG->GetToBeCalc(i))
				QuickSetBackColor(i + 1, 2, m_ColorClick);
			else
				QuickSetBackColor(i + 1, 2, m_ColorHeading);
		};
		
		int TopRowInHeader=-1;
		if (RepModelLayout) {
			TopRowInHeader = -1;
			m_RepSortButton.NormalState = pCPG->GetViewOrder();
			m_RepSortButton.Row = TopRowInHeader;
		}
		
		m_SelectTimeButton.NormalState = pCPG->GetDateSelection();
		m_SelectTimeButton.Row = TopRowInHeader;

		auto SetRecordRange = [&](size_t shiftpos=0) {
			auto irec = pCPG->GetCalcStart();
			if (irec <1||irec>pCPG->GetNumRecords()) {
				pCPG->SetCalcStart(1);
				irec = pCPG->GetCalcStart();				
			}
			str = FUtil::STD_ItoAscii(irec);
			QuickSetHeadingText(5, 0, str, false, m_SelectStartHint.param);
			irec = pCPG->GetCalcStop();
			if (irec < 1 ||irec>pCPG->GetNumRecords()) {
				pCPG->SetCalcStop(pCPG->GetNumRecords());
				irec = pCPG->GetCalcStop();
			}
			str = FUtil::STD_ItoAscii(irec);
			QuickSetHeadingText(7, 0, str, false, m_SelectEndHint.param);
			QuickSetHeadingText(9, 0, FUtil::STD_ItoAscii(pCPG->GetRecordViewStart()), false,m_ViewStartHint.param);
			QuickSetHeadingText(10, 0, FUtil::STD_ItoAscii(pCPG->GetRecordViewSize()+ pCPG->GetRecordViewStart()-1), false);
			QuickSetHeadingText(11, 0, FUtil::STD_ItoAscii(pCPG->GetCurrentPeriodToView()), false, m_ViewPageHint.param);
			QuickSetBackColor(11, 0, m_NoColor);
			QuickSetHeadingText(12, 0, FUtil::STD_ItoAscii(pCPG->GetMaxRecordViewSize()), false, m_PageRangeHint.param);
			QuickSetBackColor(12, 0, m_NoColor);
		};

		auto SetOptionsCellsInTopHeader = [&]() {
			for(size_t i=0; i<8; i++)	UnJoinCells(i, -1);
			JoinCells(0, 0, 4, 0);
			JoinCells(5, 0, 6, 0);
			JoinCells(7, 0, 8, 0);
			JoinCells(5, -1, 6, -1);
			JoinCells(7, -1, 8, -1);
			QuickSetHeadingText(5, -1, "Selected Start");
			QuickSetHeadingText(7, -1, "Selected End");
			QuickSetHeadingText(9, -1, "View Start",true, m_ViewStartHint.param);
			if (pCPG->GetNumberofSplitPeriodsToView()>1);
				QuickSetBackColor(9, -1, m_ColorClick);
	
			QuickSetHeadingText(10, -1, "View End");
			QuickSetHeadingText(11, -1, "View Page");
			QuickSetHeadingText(12, -1, "Page Range");
			QuickSetHeadingText(0, 0, pCPG->GetFileName(true));

			
			if (RepModelLayout) {
				UnJoinCells(0, -1); UnJoinCells(1, -1); UnJoinCells(1, -1);
				SetCellClickButton(m_RepSortButton);
				SetCellClickButton(m_SelectTimeButton);
				SetRecordRange(2);
			}
			else {
				QuickSetHeadingText(0, -1, "");
				JoinCells(0, -1, 2, -1);
				SetCellClickButton(m_SelectTimeButton);
				SetRecordRange(1);

			}
			

		};
		auto ClearExtraCellsAsEmpty = [&]() {
			for (size_t i = pCPG->GetNumToView(); i < 13; i++) {
				QuickSetHeadingText(i+1,0,"");
				QuickSetHeadingText(i+1,1, "");
			}
		};


		auto SetInfoInTopHeader = [&]() {
			if (pCPG->GetViewOrder()) 
				for (size_t i = 0; i < pCPG->GetNumToView(); i++) {
					GetCell(i + 1, 1, &cell);	cell.SetAlignment(UG_ALIGNCENTER);	SetCell(i + 1, 1, &cell);
					GetCell(i + 1, 2, &cell);	cell.SetAlignment(UG_ALIGNCENTER);	SetCell(i + 1, 2, &cell);
					SetColWidth(i + 1, 90);
					if (RepModelLayout) {
						QuickSetHeadingText(i + 1, 1, GetRepString(i, true));
						SetJoinVector(i);
					}
					else {
						QuickSetHeadingText(i + 1, 1, "");
						UnJoinCells(i + 1, 1);
					}
					SetJoinVectorVariables(i);
					QuickSetHeadingText(i + 1, 2, GetVarString(i),true,m_RightClickOption.param);
					SetColorToVariablesMarkedForCalculations(i);
				}
			else 
				for (size_t i = 0; i < pCPG->GetNumToView(); i++) {
					GetCell(i + 1, 1, &cell);	cell.SetAlignment(UG_ALIGNCENTER);	SetCell(i + 1, 1, &cell);
					GetCell(i + 1, 2, &cell);	cell.SetAlignment(UG_ALIGNCENTER);	SetCell(i + 1, 2, &cell);
					SetColWidth(i + 1, 90);
					if (RepModelLayout) {
						SetJoinVectorVariables(i);
						SetJoinVector(i);
						QuickSetHeadingText(i + 1, 1, GetRepString(i, true));
					}
					else {
						UnJoinCells(i, 1); UnJoinCells(i+1, 1);
						QuickSetHeadingText(i + 1, 1, FUtil::STD_ItoAscii(pCPG->GetViewVariable(i)));
					}

					QuickSetHeadingText(i + 1, 2, GetVarString(i),true, m_RightClickOption.param);
					SetColorToVariablesMarkedForCalculations(i);
				}
			ClearExtraCellsAsEmpty();
			SetOptionsCellsInTopHeader();
			if (pCPG->GetNumToView() > 0&&RepModelLayout) {
				if (joinv[joinv.size() - 1] != pCPG->GetNumToView()) {
					joinv.push_back(pCPG->GetNumToView() + 1);
				}
				if (join_varline[join_varline.size() - 1] != pCPG->GetNumToView()) {
					join_varline.push_back(pCPG->GetNumToView() + 1);
				}
			}
		};

		SetInfoInTopHeader();
	auto SetNumberInLeftColumn = [&]() {
		size_t rec_start, rec_end;

		rec_start = pCPG->GetCalcStart() - 1;
		rec_end = pCPG->GetCalcStop();
		if (rec_end + 1 - rec_start > pCPG->GetMaxRecordViewSize()) {
			rec_start = pCPG->GetRecordViewStart() - 1;
			rec_end = rec_start + pCPG->GetRecordViewSize();
		}

	
		long irow = 3;
		for (size_t i = rec_start; i < rec_end; i++) {
			QuickSetNumber(-1, irow, i + 1);
			irow++;
		}
	};
	SetColWidth(0, 130);
	SetNumberInLeftColumn();
	auto JoinCellsInHeader = [&]() {
	
			if (join_varline.size() > 1) {
				for (int i = 0; i < join_varline.size() - 1; i++)
					JoinCells(join_varline[i], 2, join_varline[i + 1] - 1, 2);
			}
			else {
				for (size_t i = 0; i < pCPG->GetNumToView(); i++)
					JoinCells(i + 1, 1, i + 1, 1);
			}
			if (RepModelLayout) {

				JoinCells(0, 1, 0, 2);
				if (joinv.size() > 1) {
					for (int i = 0; i < joinv.size() - 1; i++)
						JoinCells(joinv[i], 1, joinv[i + 1] - 1, 1);
				}
			}
			else {
				JoinCells(0, 1, 2, 1);
			}

	};
	if(RepModelLayout)	JoinCellsInHeader();

	RedrawAll();

	m_Data = true;
	MFC_Util m;
	m.SetPaneMessage("Header for Data Grid ready - Now filling grid with data");
	OnGetFileDataFillGrid(pCPG);
	m.SetPaneMessage("Header for Data Grid ready and filled with data");

}
void GridPGDoc::Make_DescriptionHeader()
{
	CPG *pCPG = GetCurrentPG_Pointer();
	if (pCPG == nullptr) return;
	m_Repit = pCPG->GetNumRepititions();

	if (m_Repit>1)
		pCPG->SetRepititionMode(true, m_Repit);


	if (m_Repit>1 || pCPG->IsAddInfoSet())
		m_col = 16;
	else
		m_col = 9;

	int cols = m_col;
	m_row = pCPG->GetNumVariables()*pCPG->GetNumRepititions();
	auto rows = m_row;
	CUGCell cell;
	EnableUpdate(FALSE);

	m_NumVar = pCPG->GetNumVariables();
	while (GetNumberRows()>rows)
		DeleteRow(0);
	EnableUpdate(TRUE);

	SetNumberCols(cols);
	SetNumberRows(rows);

	if (m_Repit>1) {
		SetColWidth(-1, 50);
		SetSH_NumberCols(2);
		SetSH_ColWidth(-1, 30);
		SetSH_ColWidth(-2, 20);
		SetRowHeight(-1, 60);
		SetTH_NumberRows(2);
		SetTH_RowHeight(-1, 20);
		SetTH_RowHeight(-2, 40);
	}
	else {
		SetSH_NumberCols(2);
		SetColWidth(-1, 30);
		SetRowHeight(-1, 60);
		SetTH_NumberRows(2);
		SetTH_RowHeight(-1, 30);
		SetTH_RowHeight(-2, 30);

	}




	GetColDefault(0, &cell);
	cell.SetAlignment(UG_ALIGNRIGHT);
	for (int i = 0; i<cols; i++) {
		if (m_Data || i>3)
			SetColDefault(i, &cell);
	}


	SetVScrollMode(UG_SCROLLTRACKING);

	SetUniformRowHeight(TRUE);
	CString str;
	if (cols>3) {
		SetColWidth(-1, 30);
		if (m_Wide) {
			SetColWidth(2, 200);
			SetColWidth(3, 80);
			SetColWidth(4, 190);

		}
		else {
			SetColWidth(2, 155);
			SetColWidth(3, 70);
			SetColWidth(4, 155);
		}
		SetColWidth(0, 0);
		SetColWidth(1, 70);
		SetColWidth(5, 40);
		SetColWidth(6, 70);
		SetColWidth(7, 70);
		SetColWidth(8, 70);
		SetColWidth(9, 70);
		SetColWidth(10, 150);
		SetColWidth(14, 150);		SetColWidth(15, 120);
		for (int i = 0; i<3; i++)
			SetColWidth(11 + i, 80);
		QuickSetHeadingText(0, -1, "Save"); 
		QuickSetHeadingText(1, -1, "View");
		QuickSetText(2, -1, "Variable Name");
		QuickSetText(3, -1, "Unit");
		QuickSetText(4, -1, "Id");
		QuickSetText(5, -1, "Pos");
		QuickSetText(6, -1, "Min");
		QuickSetText(7, -1, "Max");
		QuickSetText(8, -1, "Missings");
		QuickSetText(9, -1, "Country");
		QuickSetText(10, -1, "Station");
		QuickSetText(11, -1, "Altitude");
		QuickSetText(12, -1, "Latitude");
		QuickSetText(13, -1, "Longitude");
		QuickSetText(14, -1, "Element");
		QuickSetText(15, -1, "Unit Type");

	}
	int buttonrow;
	if (m_Repit>1) {
		for (int i = 0; i<m_Repit; i++) {
			JoinCells(-2, i*m_NumVar, -2, (i + 1)*m_NumVar - 1);
		}

		for (int i = 0; i<rows; i++) {
			QuickSetNumber(-1, i, i%m_NumVar + 1);
			QuickSetText(-2, i, FUtil::STD_ItoAscii((i + 1) / m_NumVar + 1));
		}
		buttonrow = -2;
	}
	else {
		for (int i = 0; i<m_NumVar; i++) {
			UnJoinCells(-2, i);
			UnJoinCells(i, -2);
		}
		for (int i = 0; i<rows; i++)
			QuickSetNumber(-1, i, i + 1);
		buttonrow = -1;
	}

	m_SaveCurrentFileButton.Row = m_ApplyUnitTypeButton.Row = m_ApplyElementTypeButton.Row = -2;
	m_SaveCurrentFileButton.NormalState = true;
	if (pCPG->IsFileModified()) m_SaveCurrentFileButton.NormalState = false;
	if (m_Changed_Data || m_Changed_Description) m_SaveCurrentFileButton.NormalState = false;
	auto filename = pCPG->GetFileName();
	auto prev = m_SaveCurrentFileButton.Text;
	std::string ss = "(" + filename + ")";
	m_SaveCurrentFileButton.Text = ss;
	prev = string(m_SaveCurrentFileButton.TextOptional)+ss;
	m_SaveCurrentFileButton.TextOptional = prev;


	for (auto c = 0; c < 10; c++) for (auto r = -2; r < 0; r++) { UnJoinCells(c, r); };

	SetCellClickButton(m_SaveCurrentFileButton);
	m_SaveCurrentFileButton.Text = prev;
	m_SaveCurrentFileButton.TextOptional = prev;
	if (!m_SaveCurrentFileButton.NormalState) QuickSetBackColor(0,buttonrow, m_ColorChanged);



	SetCellClickButton(m_ApplyElementTypeButton);
	SetCellClickButton(m_ApplyUnitTypeButton);
	m_Data = false;

	MFC_Util m;
	OnGetFileDescriptionFillGrid(pCPG);
	m.SetPaneMessage("Header for Data Description ready and filled with data");

}
void GridPGDoc::OnPGHeaderUpdate(int col, long row, BOOL data, BOOL wide, long repit)
{
	m_col = col;
	if (!data&&repit>1) {
		row = row*repit;


	}
	m_row = row;
	m_Data = data;
	m_Wide = wide;
	m_Repit = repit;


	EnableUpdate(FALSE);
	while (GetNumberRows()>row)
		DeleteRow(0);
	EnableUpdate(TRUE);
	CPG *pPG;
	pPG = GetCurrentPG_Pointer();

	if (pPG != nullptr&&data) {
		if (pPG->GetNumBuffers()>1) {


		}
		else if (!(pPG->GetNumRepititions() == repit&&pPG->GetNumRecords() == row&&pPG->GetNumVariables() == col - 1))
			pPG->AdjustSize(row, col - 1, repit);
		if (m_Repit>1)
			pPG->SetRepititionMode(true, m_Repit);
		else
			pPG->SetRepititionMode(false, 0);
		Make_DataHeader();
	}
	else
		Make_DescriptionHeader();

}
CPG* GridPGDoc::GetCurrentPG_Pointer() {
	CPG* out = nullptr;
	if (m_pDoc == nullptr) return nullptr;

	size_t i_doc = m_pPGArchive->GetIndex(m_pDoc);
	out = (CPG*)m_pPGArchive->GetPGFile(i_doc);


	m_pPG = out;
	return out;
}
bool GridPGDoc::OnGetFileDataFillGrid(CPG *pCPG)
{
	long m_NumRecords;
	int m_NumVariables;

	CUGCell cell;

	EnableUpdate(FALSE);
	if (!m_Data) return false;
	if (!pCPG->IsGood() && pCPG->IsOpen()) {
		pCPG->ClearFile();
		pCPG->ReOpen();
	}
	else if (!pCPG->IsOpen()) {
		pCPG->ReOpen();
		if (!pCPG->SetRepititionSequence(0, true)) return false;
	}
		m_NumVariables = m_col - 1;
		size_t num_buff = pCPG->GetNumBuffers();
		size_t num_rep = pCPG->GetNumRepititions();
		
		if (m_Repit>1)
			m_NumVariables = m_NumVariables / m_Repit;
		m_NumRecords = m_row;


		if (!pCPG->GetCompleteRead() && !pCPG->IsGood())
			pCPG->ReOpen();

		size_t rec_start, rec_end;


			rec_start = pCPG->GetCalcStart()-1;
			rec_end = pCPG->GetCalcStop();
			if (rec_end + 1 - rec_start > pCPG->GetMaxRecordViewSize()) {
				rec_start = pCPG->GetRecordViewStart() - 1;
				rec_end = rec_start + pCPG->GetRecordViewSize();
			}


		size_t irow = 3, ivarno;
		int icol;
		for (size_t nrep = 0; nrep<m_Repit; nrep++) {
			if (m_Repit>1) {
				if (!pCPG->SetRepititionSequence(nrep, false))
					return FALSE;
				//pCPG->SetActBufferReset();
				irow = 3;
			}

			for (int ii = rec_start; ii<rec_end; ii++) {
				if (nrep == 0) {
					auto vtest = pCPG->GetVarValue(1,1, nrep);
					QuickSetText(0, irow, pCPG->GetDate(ii + 1));
				}
				for (size_t ind = 0; ind<pCPG->GetNumToView(); ind++) {
					if (pCPG->GetViewRepetition(ind) == nrep+1) {
						ivarno = pCPG->GetViewVariable(ind);
						icol = ind + 1;
						float v = pCPG->GetVarValue(ivarno, ii + 1, nrep);
						if (v>-0.8E38) {
							GetCell(icol, irow, &cell);
							int alin = cell.GetAlignment();
							cell.SetNumber(v);
							alin = cell.GetAlignment();
							//cell.SetAlignment(UG_RIGHT);
							SetCell(icol, irow, &cell);
						}
						else
							QuickSetText(icol, irow, "", false);

					}
				}
				irow++;
			}
		}



	//RedrawAll();
	EnableUpdate(TRUE);
	RedrawAll();
	return TRUE;

}
bool GridPGDoc::OnGetFileDescriptionFillGrid(CPG *pCPG)
{

	int NumVariables;

	CUGCell cell;

	EnableUpdate(FALSE);
	if (m_Data) return false;


	NumVariables = pCPG->GetNumVariables();
	m_Repit = pCPG->GetNumRepititions();
	size_t ind;
	for (int nrep = 0; nrep<m_Repit; nrep++) {
		for (int i = 0; i<NumVariables; i++) {
			ind = nrep*NumVariables + i;
			GetCell(0, ind, &cell);
			cell.SetCellType(UGCT_CHECKBOX);
			bool testV = pCPG->GetToBeSaved(i);
			if (testV)
				cell.SetBool(TRUE);
			else
				cell.SetBool(FALSE);
			SetCell(0, ind, &cell);

			GetCell(1, ind, &cell);
			cell.SetCellType(UGCT_CHECKBOX);

			testV = pCPG->GetToBeViewed(i);
			if (testV)
				cell.SetBool(TRUE);
			else
				cell.SetBool(FALSE);
			SetCell(1, ind, &cell);

			for (int colc = 2; colc < 14; colc++) {
				GetCell(colc, ind, &cell);
				cell.SetCellType(UGCT_NORMAL);
				cell.SetAlignment(UG_ALIGNLEFT);
				SetCell(colc, ind, &cell);
			}

			QuickSetText(2, ind, pCPG->GetVarName(i + 1, nrep),false);
			QuickSetText(3, ind, std::string(pCPG->GetVarUnit(i + 1, nrep)),false);
			QuickSetText(4, ind, pCPG->GetVarId(i + 1, nrep),false);
			QuickSetText(5, ind, pCPG->GetVarPos(i + 1, nrep),false);
			float v;
			v = pCPG->GetVarMin(i + 1, nrep);
			if (v<0.5E38)	QuickSetNumber(6, ind, v);
			v = pCPG->GetVarMax(i + 1, nrep);
			if (v>-0.5E37)	QuickSetNumber(7, ind, v);
			QuickSetNumber(8, ind, pCPG->GetNumberofMissing(ind + 1));
			if (pCPG->IsAddInfoSet()) {
				QuickSetText(9, ind, pCPG->GetVarCountry(i + 1, nrep),false);
				QuickSetText(10, ind, pCPG->GetVarStation(i + 1, nrep),false);
				QuickSetNumber(11, ind, pCPG->GetVarAlt(i + 1, nrep),false);
				QuickSetNumber(12, ind, pCPG->GetVarLat(i + 1, nrep),false);
				QuickSetNumber(13, ind, pCPG->GetVarLong(i + 1, nrep),false);

				m_ElementTypeVariables.Row = ind;
				elements elem = pCPG->GetVarElement(i + 1, nrep);
				if (size_t(elem) >= m_ElementTypeVariables.VectorNames.size())
							elem= elements::GENERAL;
				m_ElementTypeVariables.Text = p_ModelInfo->ElementName(size_t(elem));
				SetCellClickButton(m_ElementTypeVariables);

				m_UnitTypeVariables.Row = ind;
				UNIT_TYPES unit = pCPG->GetVarUnitType(i + 1, nrep);
				vector<string_view> previous = m_UnitTypeVariables.VectorNames;
				vector<UNIT_TYPES> types = GetUnitOptions(elem);
				if (types.size() > 0) m_UnitTypeVariables.VectorNames = p_ModelInfo->GetSelectedStringBasedUnits(types);
				else m_UnitTypeNewVariables.VectorNames = p_ModelInfo->GetAllStringBasedUnits();
				m_UnitTypeVariables.Text = p_ModelInfo->GetUnitString(unit);
				SetCellClickButton(m_UnitTypeVariables);
				m_UnitTypeVariables.VectorNames = previous;
			}
		}
	}

	EnableUpdate(TRUE);
	RedrawAll();
	return true;

}
bool GridPGDoc::OnWriteGridDataToFile(CPG *pCPG)
{	// Read Information from the grid cell and write the values into the PG File
	long m_NumRecords;
	int m_NumVariables;
	CUGCell cell;
	float v;

	//pCPG->SetNumRecords(m_row);
	m_row = GetNumberRows();
	m_col = GetNumberCols();
	if (m_row == 0 || m_col == 0) return true;

	if (m_Data) {

		auto num_Repit = pCPG->GetNumRepititions();
		m_NumVariables = (m_col - 1) / num_Repit;

		m_NumRecords = m_row;
		long size = m_row*m_NumVariables;

		m_NumRecords = m_row;
		if (m_NumRecords <= 0) return FALSE;
		size_t rec_start, rec_end;
		if (!pCPG->MakeFileToEnableWriting(true)) return false;


		bool status;
		MFC_Util m;

		for (long irep = 0; irep<num_Repit; irep++) {
			status = pCPG->SetRepititionSequence(irep, true);

			if (pCPG->GetNumBuffers()>1) {
				if (pCPG->GetPrevBuffer() == 0) {
					rec_start = 0;
					rec_end = pCPG->GetNumRecordsBufSize();
				}
				else {
					rec_start = (pCPG->GetPrevBuffer() - 1)*pCPG->GetNumRecordsBufSize();
					rec_end = pCPG->GetPrevBuffer()*pCPG->GetNumRecordsBufSize();
					if (rec_end>pCPG->GetNumRecords())
						rec_end = pCPG->GetNumRecords();
				}
			}
			else {
				rec_start = 0;
				rec_end = m_NumRecords;
			}
			size_t irow = 3;
			auto GetRecordNo = [&]() {
				GetCell(-1, irow, &cell);
				return size_t(cell.GetNumber());
			};

			for (int ii = rec_start; ii<rec_end; ii++) {
				GetCell(0, irow, &cell);
				int ich = cell.GetTextLength();
				if (ich > 0 && irep == 0 && cell.GetBackColor() == m_ColorChanged) {
					auto RecordNo = GetRecordNo();
					pCPG->SetDate(RecordNo, cell.GetStdText());
				}
				for (size_t ind = 0; ind<pCPG->GetNumToView(); ind++) {
					if (pCPG->GetViewRepetition(ind) == irep) {
						int ivarno = pCPG->GetViewVariable(ind);
						int icol = ind + 1;
						GetCell(icol, irow, &cell);
						ich = cell.GetTextLength();
						if (cell.GetBackColor() == m_ColorChanged) {

							v = cell.GetNumber();
							cell.SetBackColor(m_NoColor);
							SetCell(icol, irow, &cell);
							auto RecordNo = GetRecordNo();
							if (ich >= 0) {
								pCPG->SetVarValue(ivarno, RecordNo, v);
							}
							else if (ich<0)
								pCPG->SetVarValue(ivarno, RecordNo, MISSING);
						}
					}
				}
				irow++;
			}
			if (pCPG->GetCompleteRead()) {

				if (pCPG->WritePGFile()) {

					pCPG->SetNewReadofFile();
					m.SetPaneMessage("Result saved in Continous PG file");
					pCPG->CloseFile();

				}

			}
			else {
				if (pCPG->GetNumBuffers()>1) {
					//			Make sure last Block is saved to file

					if (!pCPG->WritePGFileRecord(rec_end)) {
						pCPG->CloseFile();
						pCPG->ReOpen(true);
						pCPG->WritePGFileRecord(rec_end);

					}
					else
						m.SetPaneMessage("Result saved in buffer PG file");

				}
				else {
					if (pCPG->GetNumRepititions()!= std::string::npos)
						status = pCPG->WriteNewBuffer(irep);

					if (!status) return false;
					m.SetPaneMessage("Result saved in first buffer of PG file");

				}
				if (irep == num_Repit) {
					pCPG->WritePGFileStartHeader(pCPG->GetNumRecords(), num_Repit);
					m.SetPaneMessage("Start Header saved in buffer of PG file");
				}

				pCPG->ReCalculateMinMax(irep);

			}

		}
		pCPG->WriteDescriptionsToStream();

	}
	m_Changed_Data = false;
	return true;

}
bool GridPGDoc::OnWriteGridDescriptionToFile(CPG *pCPG)
{	// Read Information from the grid cell and write the values into the PG File storage

	int m_NumVariables;
	CUGCell cell;
		m_row = GetNumberRows();
		m_col = GetNumberCols();
		m_NumVariables = m_row;
		if (m_NumVariables <= 0) return true;
		if (!pCPG->IsAddInfoSet())
			pCPG->SetInfoMode();
		long line = 0;
		for (int irep = 0; irep < m_Repit; irep++) {
			for (int i = 0; i < pCPG->GetNumVariables(); i++) {
				line = irep*pCPG->GetNumVariables() + i;
				GetCell(2, line, &cell);
				string str = cell.GetStdText();
				pCPG->SetVarName(i + 1, str, irep);
				GetCell(3, line, &cell);
				str = cell.GetStdText();
				pCPG->SetVarUnit(i + 1, str, irep);
				GetCell(4, line, &cell);
				str = cell.GetStdText();
				pCPG->SetVarId(i + 1, str, irep);
				GetCell(5, line, &cell);
				str = cell.GetStdText();
				pCPG->SetVarPos(i + 1, str, irep);
				if (line < m_row) {
					GetCell(9, line, &cell);
					str = cell.GetStdText();
					pCPG->SetVarCountry(i + 1, str, irep);

					GetCell(10, line, &cell);
					str = cell.GetStdText();
					pCPG->SetVarStation(i + 1, str, irep);

					GetCell(11, line, &cell);
					double f = cell.GetNumber();
					pCPG->SetVarAlt(i + 1, f, irep);

					GetCell(12, line, &cell);
					f = cell.GetNumber();
					pCPG->SetVarLat(i + 1, f, irep);

					GetCell(13, line, &cell);
					f = cell.GetNumber();
					pCPG->SetVarLong(i + 1, f, irep);
				}

			}
		}
	return true;

}
bool GridPGDoc::CheckForChanges()
{
	if (m_pPG == nullptr) return true;
	bool OK=true;
	if (m_Changed_Data) {
		if (!OnWriteGridDataToFile(m_pPG)) OK = false;
		else m_Changed_Data = false;
	}

	
	if (m_Changed_Description) {
		if (!OnWriteGridDescriptionToFile(m_pPG)) OK = false;
		else m_Changed_Description = false;

	}



	return OK;
}
void GridPGDoc::CheckStatusOfPG_Outputs(int col, long row)
{
	CUGCell cell;
	size_t index = size_t(row);
	if (col == 0) {
		GetCell(0, row, &cell);
		BOOL status;
		status = cell.GetBool();
		if (status) {
			m_pPG->SetToBeSaved(index, true);
		}
		else {
			m_pPG->SetToBeSaved(index, false);
		}
	}
	else if (col == 1) {
		GetCell(1, row, &cell);
		BOOL status;
		status = cell.GetBool();
		if (status) {
			m_pPG->SetToBeViewed(index, true);
		}
		else {
			m_pPG->SetToBeViewed(index, false);
		}
	}
	m_pPG->UpdateViewOrder();
}

void GridPGDoc::DefineButtonInfos()
{
	CLICK_BUTTON i;
	i.Type = INFOONLYBUTTON;
	i.Param= GetUniqueMenuNumber();
	i.Text = "Equal Number of Records for each site";
	i.TextOptional = "Non Equal size of records for each site";
	i.HintText = "With equal number of Records a single PG File with Repetitions will be created\nOtherwise a single PG file is created for each site";
	i.Col_start = 2;
	i.Col_end = 4;
	i.Row = 1;
	SetClickButtonMap(i);
	m_SiteInfoButton = i;

	i.Type = INFOONLYBUTTON;
	i.Param = GetUniqueMenuNumber();
	i.Text = "two line delimiter \\r and \\n";
	i.TextOptional = "One line delimiter \\n";
	i.HintText = "Two delimeters are typical for windows files and one delimiter is typical for Unix files\n";
	i.Col_start =2;
	i.Col_end = 2;
	i.Row = 1;
	SetClickButtonMap(i);
	m_LineSeparationButton = i;

	i.Type = INFOONLYBUTTON;
	i.Param = GetUniqueMenuNumber();
	i.VerticalDirection = false;
	i.NormalState = true;
	i.Col_start = 1;
	i.Col_end = 1;
	i.VectorNames.resize(4);

	i.VectorNames[0] = "var separator(,)";
	i.VectorNames[1] = "var separator(;)";
	i.VectorNames[2] = "var sepatator(Tab)";
	i.VectorNames[3] = "var sepatator(Space)";
	i.TextOptional = "No Variable separtor";
	i.Row = 1;
	i.VerticalDirection = false;
	i.HintText = "Note that the same Variable sepatator must be used in the entire file\nAvoid use of (space) and comma  if they are part of the Variable definition";
	m_VarSeparationButton = i;
	SetClickButtonMap(i);

	i.Type = INFOONLYBUTTON;
	i.Param = GetUniqueMenuNumber();
	i.VerticalDirection = false;
	i.NormalState = true;
	i.Col_start = 3;
	i.Col_end = 3;
	i.VectorNames.resize(18);

	i.VectorNames[STANDARD] = "Standard";
	i.VectorNames[1] = "All 5 Time Indicators separated (YMDHM)";
	i.VectorNames[2] = "3 Time Indicators (YMD)";
	i.VectorNames[3] = "YMD as one indicator and HM as another";
	i.VectorNames[4] = "NOAA Fixed format";
	i.VectorNames[5] = "SMHI Met Station format";
	i.VectorNames[6] = "SWISS Met Stations";
	i.VectorNames[7] = "4 Time Indicators (YMDH)";
	i.VectorNames[8] = "Czech Met Stations";
	i.VectorNames[9] = "Czech Column based";
	i.VectorNames[10] = "Month Column based";
	i.VectorNames[11] = "PROFOUND";
	i.VectorNames[12] = "ID_Scan";
	i.VectorNames[13] = "SMHI_OpenData";
	i.VectorNames[14] = "SILO";
	i.VectorNames[15] = "SITES";
	i.VectorNames[16] = "OPEN METEO";
	i.VectorNames[17] = "Undefined";

	i.TextOptional = "No Variables Identified";
	i.Row = 1;
	i.VerticalDirection = false;
	i.HintText = "Note that the same Variable sepatator must be used in the entire file\nAvoid use of (space) and comma  if they are part of the Variable definition";
	m_IdentifiedFormatButton = i;
	SetClickButtonMap(i);

	m_RepSortButton.Type = INFOBUTTON;
	m_RepSortButton.Param = GetUniqueMenuNumber();
	m_RepSortButton.Text = "Repetition Based Sort";
	m_RepSortButton.TextOptional = "Repetition Merged Sort";
	m_RepSortButton.HintText = "Single click to Button to View variables in different ways";
	m_RepSortButton.Col_start = 0;
	m_RepSortButton.Col_end = 2;


	SetClickButtonMap(m_RepSortButton);

	m_SelectTimeButton.Type = INFOBUTTON;
	m_SelectTimeButton.Param = GetUniqueMenuNumber();
	m_SelectTimeButton.Text = "All Records";
	m_SelectTimeButton.TextOptional = "Selected Records";
	m_SelectTimeButton.HintText = "Single Click on Button to Select All Records or Change specific numbers in the edit fields below\nNote that the Selected records will be the records used for any type of calculations/change";
	m_SelectTimeButton.Col_start = 3;
	m_SelectTimeButton.Col_end = 4;
	SetClickButtonMap(m_SelectTimeButton);

	m_ScanFileButton.Type = INFOBUTTON;
	m_ScanFileButton.Param = GetUniqueMenuNumber();
	m_ScanFileButton.Text = "Scan CSV File";
	m_ScanFileButton.TextOptional = "Scan CSV File";
	m_ScanFileButton.HintText = "Single Click on Button to Scan an existing file";
	m_ScanFileButton.Col_start = 0;
	m_ScanFileButton.Col_end = 1;
	m_ScanFileButton.Row = -2;
	SetClickButtonMap(m_ScanFileButton);

	m_OpenFileButton.Type = INFOBUTTON;
	m_OpenFileButton.Col_start = 2;
	m_OpenFileButton.Col_end = 3;
	m_OpenFileButton.Param = GetUniqueMenuNumber();
	m_OpenFileButton.Text = "Create PG File";
	m_OpenFileButton.TextOptional = "Create PG File";
	m_OpenFileButton.NormalState = false;
	m_OpenFileButton.Row = -2;
	SetClickButtonMap(m_OpenFileButton);

	m_SelectVariableButton.Type = INFOBUTTON;
	m_SelectVariableButton.Col_start = 0;
	m_SelectVariableButton.Col_end = 1;
	m_SelectVariableButton.Param = GetUniqueMenuNumber();
	m_SelectVariableButton.Text = "Select all Variables";
	m_SelectVariableButton.TextOptional = "Deselect all Variables";
	m_SelectVariableButton.HintText = "Single Click on Button to Deselect All Records or use check box to select/deselect individual variables";
	m_SelectVariableButton.NormalState = false;
	m_SelectVariableButton.Row = -1;
	SetClickButtonMap(m_SelectVariableButton);

	m_SiteInfoFileButton.Type = INFOBUTTON;
	m_SiteInfoFileButton.Col_start = 4;
	m_SiteInfoFileButton.Col_end = 5;
	m_SiteInfoFileButton.Param = GetUniqueMenuNumber();
	m_SiteInfoFileButton.Text = "Add Site Info File";
	m_SiteInfoFileButton.TextOptional = "Add Site Info File";
	m_SiteInfoFileButton.HintText = "Single Click to Add the File to add information about the site variable. \nThe added file should contain one variable (the first) with Site Id Number. \nAdditional variables May be Longitude, Latitude, Altitude, Country.\n";	
	m_SiteInfoFileButton.NormalState = false;
	m_SiteInfoFileButton.Row = -2;
	SetClickButtonMap(m_SiteInfoFileButton);
}

void GridPGDoc::DefineCheckBoxButtons()
{
	CLICK_BUTTON c;
	c.Type = CHECKBOX;
	c.Param = GetUniqueMenuNumber();
	c.VerticalDirection = false;
	c.Col_start = 0;
	c.Col_end = 0;
	c.Row = 2;
	c.HintText = "Click on checkbox to select deselect Variables from output file";
	m_SelectedOutputVarButton = c;
	SetClickButtonMap(c);
}

void GridPGDoc::DefineListButtonInfos()
{
	CLICK_BUTTON l;
	m_ImportedFiles.Type = LISTBUTTON;
	m_ImportedFiles.Param = GetUniqueMenuNumber();
	m_ImportedFiles.Col_start = 0;
	m_ImportedFiles.Col_end = 1;
	m_ImportedFiles.Row = 0;
	m_ImportedFiles.VerticalDirection = true;
	SetClickButtonMap(m_ImportedFiles);

	m_NewFileOptions.Type = LISTBUTTON;
	m_NewFileOptions.VerticalDirection = true;
	m_NewFileOptions.Param = GetUniqueMenuNumber();
	m_NewFileOptions.Col_start = 0;
	m_NewFileOptions.Col_end = 2;
	m_NewFileOptions.Row = 0;
	m_NewFileOptions.HintText = "Click on Button to select Option";
	m_NewFileOptions.VectorNames.push_back("With New Clean Structure- Empty cells");
	m_NewFileOptions.VectorNames.push_back("Import from CSV/txt File");
	m_NewFileOptions.VectorNames.push_back("Adding previous Files to each other (same variables - new records)");
	m_NewFileOptions.VectorNames.push_back("Mixing previous Files to each other (new variables)");
	m_NewFileOptions.VectorNames.push_back("Merging similar PG files to a new aggregated file (= with Repetitions)");
	m_NewFileOptions.VectorNames.push_back("ReShape current PG file to repeated time with fewer variables");
	SetClickButtonMap(m_NewFileOptions);
	
	m_AddFileList.Type = LISTBUTTON;
	m_AddFileList.Param = GetUniqueMenuNumber();
	m_AddFileList.HintText = "Click on File Name to Remove File from list";
	m_AddFileList.Col_start = 0;
	m_AddFileList.Col_end = 0;
	m_AddFileList.Row = 0;
	m_AddFileList.VerticalDirection = true;
	SetClickButtonMap( m_AddFileList);

	l.Type = LISTBUTTON;
	l.Param = GetUniqueMenuNumber();
	l.VerticalDirection = false;
	l.Col_start = 0;
	l.Col_end = 0;
	l.Row = 4;
	l.HintText = "The variables in the Add Site Info may be linked to the Variable Description\n by selection in the linkage row below";
	m_IdentifiedVarSiteInfoButton=l;
	SetClickButtonMap(l);

	l.Param = GetUniqueMenuNumber();
	l.VerticalDirection = false;
	l.Col_start = 0;
	l.Col_end = 0;
	l.Row = 4;
	l.HintText = "The variables are those selected from the Heading line above";
	m_NameOfHeadingLine = l;
	SetClickButtonMap(l);

}

void GridPGDoc::DefineClickButtonInfos()
{
	string ClickAdd;
	ClickAdd = "Click to ";
	CLICK_BUTTON b;
	b.Type = CLICKBUTTON;
	b.Param = GetUniqueMenuNumber();
	b.Col_start = 0;
	b.Col_end = 0;
	b.Row = -2;
	SetClickButtonMap(b);



	b.Text = "Add More PG Files";
	b.Col_start = 0;
	b.HintText = "Click to Add Addition Files to List";
	b.TextOptional = " Click to Specify Files";
	b.Col_end = 0;
	b.Row = -2;
	b.Param = GetUniqueMenuNumber();
	m_AddMorePGFilesButton = b; 	SetClickButtonMap(b);

	b.Text = "Translation codes";
	b.HintText = "Change Translation codes in list below";
	b.Col_start = 0;
	b.Col_end = 1;
	b.Row = 2;
	b.Param = GetUniqueMenuNumber();
	m_TranslationListButton = b; 	SetClickButtonMap(b);

	b.Text = "Update Translation";
	b.HintText = "Click Update Translation after change of Translation codes";
	b.Col_start = 1;
	b.Col_end = 2;
	b.Row = -2;
	b.Param = GetUniqueMenuNumber();

	m_UpdateReshapeButton = b; 	SetClickButtonMap(b);
	b.Text ="Add Files";
	b.HintText = "Click to Add Addition Files to the Mixed";
	b.Param = GetUniqueMenuNumber();

	m_MixMorePGFilesButton = b; 	SetClickButtonMap(b);
	b.Param = GetUniqueMenuNumber();
	b.HintText = "Click to Add Addition Files to the List";

	m_MergeMorePGFilesButton = b; 	SetClickButtonMap(b);

	b.Param = GetUniqueMenuNumber();
	b.Text = "Interpolate";
	b.TextOptional = "No interpolation";
	b.Col_start = 1;
	b.Col_end = 1;
	b.NormalState = true;
	b.Row = -2;
	b.HintText = "Select Interpolate to keep only Records (timepoints) from first file and \nassign values to those timepoints by linear interpolation if they are not present in the other files\nSelect No Interpolation to keep all time point that are unique and\nassign missing values when they are not present in the other files\n";
	m_MixInterpolateButton = b;
	SetClickButtonMap(b);

	b.Text = "Reset List";
	b.HintText = "Click to Reset the List for selection of other files";
	b.Col_start = 1; b.Col_end = 2; b.Param = GetUniqueMenuNumber();
	b.NormalState = true;
	b.TextOptional = "";
	m_ResetListAddButton = b;
	SetClickButtonMap( b);

	b.Param = GetUniqueMenuNumber();
	b.HintText = "Click to reset setting and skip creation of a new file";
	b.Text = "Quit";
	b.Col_start = 6;
	b.Col_end = 7;
	m_QuitButton = b;
	SetClickButtonMap(b);

	b.Param = GetUniqueMenuNumber();
	b.HintText = "Click to reset setting and skip Open of files";
	b.Text = "Quit";
	b.Col_start = 1;
	b.Col_end = 1;
	b.Row = -1;
	m_QuitImportButton = b;
	SetClickButtonMap(b);


	b.Text = "Create File ";
	b.Row = -2;
	std::string str = ClickAdd + std::string(b.Text);
	b.HintText = str;
	b.Col_start = 3; b.Col_end = 4; b.Param = GetUniqueMenuNumber();
	m_CreateNewAddButton=b;
	SetClickButtonMap(b);

	b.NormalState = true;
	b.TextOptional = "Format (many lines)";
	b.Text = " Format (Repetitions)";
	str = "Click on button to shift between different output options" + string(b.Text);
	b.HintText = str;
	b.Col_start = 5, b.Col_end = 5;
	b.Param = GetUniqueMenuNumber();
	m_ShiftOutputFormat = b;

	b.NormalState = true;
	b.Text = "Create File";
		b.Row = -2;
		str = ClickAdd + string(b.Text);
		b.HintText = str;
	b.Col_start = 3; b.Col_end = 4; b.Param = GetUniqueMenuNumber();
	m_CreateNewReshapeButton = b;
	SetClickButtonMap(b);


	b.Param = GetUniqueMenuNumber();
	m_CreateNewMixButton = b;
	SetClickButtonMap(b);

	b.Param = GetUniqueMenuNumber();
	m_CreateNewMergeButton = b;
	SetClickButtonMap(b);

	b.Text = "Change Output Name";
	b.pPG = nullptr;
	str = ClickAdd + string(b.Text);
	b.HintText = str;
	b.Col_start = 0; b.Col_end = 0; b.Param = GetUniqueMenuNumber();
	m_NewOutputPGFileButton = b;
	SetClickButtonMap(b);

	b.Param= GetUniqueMenuNumber();
	m_NewOutputMIXPGFileButton = b;
	
	SetClickButtonMap(b);

	b.Param = GetUniqueMenuNumber();
	m_NewOutputMergePGFileButton = b;
	SetClickButtonMap(b);

	b.Param = GetUniqueMenuNumber();
	b.Row = 0;
	m_NewOutputNewGridPGFileButton = b;
	SetClickButtonMap(b);



	b.Text = "Create New PG File";
	str = ClickAdd + string(b.Text);
	b.HintText = str;
	b.Col_start = 0; b.Col_end = 0; b.Param = GetUniqueMenuNumber();
	b.Row = -2;
	m_CreateNewEmptyPGFile = b;
	SetClickButtonMap( b);

	b.Text = "Change File Name";
	str = ClickAdd + string(b.Text);
	b.HintText = str;

	b.Col_start = 1; b.Col_end = 3; b.Param = GetUniqueMenuNumber();
	m_ChangeNewGridFileName = b;
	SetClickButtonMap( b);

	b.Text = "Apply Names from Heading";
	b.TextOptional = "Clean Current Names";
	b.NormalState = true;
	b.HintText = "The Text from the input first header line will be applied as the name in the PG File to be created";
	b.Col_start = 3; b.Col_end = 4; b.Param = GetUniqueMenuNumber();
	m_ApplyNameFromHeadingButton = b;
	SetClickButtonMap(b);
	b.Text = "Apply Unit from Heading";
	b.TextOptional = "Clean Current Units";
	b.NormalState = true;
	b.HintText = "The End Text (following _) from the input first header line will be applied as the unit in the PG File to be created";
	b.Col_start = 5; b.Col_end = 6; b.Param = GetUniqueMenuNumber();
	m_ApplyUnitFromHeadingButton = b;
	SetClickButtonMap(b);
	b.Text = "Apply Names from Element Type";
	b.TextOptional = "Clean Current Names";
	b.NormalState = true;
	b.HintText = "The Text from the Type Element Name as the Defaults for The CoupModel will be applied if you click the button";
	b.Col_start = 7; b.Col_end = 8; b.Param = GetUniqueMenuNumber();
	m_ApplyNameFromElementTypeButton = b;
	SetClickButtonMap(b);
	b.Text = "Apply Unit from Type of Units";
	b.TextOptional = "Clean Current Units";
	b.NormalState = true;
	b.HintText = "The Text Selected by the Type of Units as selected below will be applied as the unit in the PG File to be created";
	b.Col_start = 9; b.Col_end = 10; b.Param = GetUniqueMenuNumber();
	m_ApplyUnitFromUnitTypeButton = b;
	SetClickButtonMap(b);

	b.Text = "Apply as Names";
	b.TextOptional = "Clean Current Names";
	b.NormalState = true;
	b.HintText = "The Text from the Type Element Name will be applied as Names in the description field if you click the button";
	b.Row = -1;  b.Col_start = 14; b.Col_end = 14; b.Param = GetUniqueMenuNumber();
	m_ApplyElementTypeButton = b;
	SetClickButtonMap(b);

	b.Text = "Apply as Units";
	b.TextOptional = "Clean Current Units";
	b.NormalState = true;
	b.HintText = "The Text from the Unit Types will be applied as Units in the description field if you click the button";
	b.Row = -1;  b.Col_start = 15; b.Col_end = 15; b.Param = GetUniqueMenuNumber();
	m_ApplyUnitTypeButton = b;
	SetClickButtonMap(b);

	b.Text = "Save File As:";
	b.TextOptional = "Save Modified File";
	b.NormalState = true;
	b.HintText = "Modified Edited file can be saved with the same name or with optional new names (Save as)";
	b.Row = -2;  b.Col_start = 0; b.Col_end = 8; b.Param = GetUniqueMenuNumber();
	m_SaveCurrentFileButton = b;
	SetClickButtonMap(b);



}

void GridPGDoc::DefineHintList()
{
	HINTLIST h;
	h.param = GetUniqueMenuNumber();
	h.HintText = "The cell can be edited here or after the creation of a new PG File is made)";
	SetHintMap(h); m_DescriptionImportedFile = h;

	h.param= GetUniqueMenuNumber();
	h.HintText = "A Date column including complete information for date/time (Code = Date)";
	SetHintMap(h); m_ScanTimeId_Date = h;
	
	h.param = GetUniqueMenuNumber();
	h.HintText = "The Start Date/time can be modified if not specified in the input file";
	SetHintMap(h); m_ScanStartDate = h;

	h.param = GetUniqueMenuNumber();
	h.HintText = "The time interval between records can be modified if not specified in the input file";
	SetHintMap(h); m_ScanMinDiff = h;

	h.param = GetUniqueMenuNumber();
	h.HintText = "A Year column including the year (Code = Year)";
	SetHintMap(h); m_ScanTimeId_Year = h;

	h.param = GetUniqueMenuNumber();
	h.HintText = "A Month column including the Month number of Year (Code = Month)";
	SetHintMap(h); m_ScanTimeId_Month = h;

	h.param = GetUniqueMenuNumber();
	h.HintText = "A DayNo column including the DayNo of Year (Code = DayNo)";
	SetHintMap(h); m_ScanTimeId_DayNo = h;
	
	h.param = GetUniqueMenuNumber();
	h.HintText = "A Day column including the Day of the Month (Code = Day)";
	SetHintMap(h); m_ScanTimeId_Day = h;

	h.param = GetUniqueMenuNumber();
	h.HintText = "A Time column including the Hour and Minute of Day (Code = Time)\n";
	h.HintText+= " Format should be HH:MM ";
	SetHintMap(h); m_ScanTimeId_Time = h;

	h.param = GetUniqueMenuNumber();
	h.HintText = "A Hour column including the Hour of Day (Code = Hour)\n";
	h.HintText += " Format should be an integer number";
	SetHintMap(h); m_ScanTimeId_Hour = h;

	h.param = GetUniqueMenuNumber();
	h.HintText = "A Minute column including the Minutes of an Hour (Code = Minutes)\n";
	h.HintText += " Format should be an integer number";
	SetHintMap(h); m_ScanTimeId_Minute = h;



	h.param = GetUniqueMenuNumber();
	h.HintText = "A Site column including a unique site number (Code = site_id)";
	SetHintMap(h); m_ScanSiteId = h;

	h.param = GetUniqueMenuNumber();
	h.HintText = "Click on Button if you want to shift to next page";
	SetHintMap(h); m_ViewStartHint = h;

	h.param = GetUniqueMenuNumber();
	h.HintText = "Edit field  if you want to jump to a specific new page";
	SetHintMap(h); m_ViewPageHint = h;
	h.param = GetUniqueMenuNumber();
	h.HintText = "Edit field if you want to change the number of rows in the grids below\nNote - the maximum number depends on memory and number of total cells ";
	SetHintMap(h); m_PageRangeHint = h;
	h.param = GetUniqueMenuNumber();
	h.HintText = "Edit field if you want to change the start Record to be considered for operations to be made\nNote - The view start may be a higher number\n";
	h.HintText	+="Also note that the selection can be made based on date by click on the Selected Records button";   
	SetHintMap(h); m_SelectStartHint = h;
	h.param = GetUniqueMenuNumber();
	h.HintText = "Edit field if you want to change the end Record to be considered for operations to be made\nNote - the maximum number depends on memory and number of total cells ";
	SetHintMap(h); m_SelectEndHint = h;

	h.param = GetUniqueMenuNumber();
	h.HintText = "Then number of different time points in your input file";
	SetHintMap(h); m_ScanUniqueStep = h;

	h.param = GetUniqueMenuNumber();
	h.HintText = "Normal Step should be 1 providing that all time are unique and not replicated\n"+
	h.HintText = "A Value larger than 1 indicates that you have dublicate time points in records following each other";
	SetHintMap(h); m_ScanRecordStep = h;

	h.param = GetUniqueMenuNumber();
	h.HintText = "The Total Number of Records should reflect exactly the number of step size and unique number of time points\n"+
	h.HintText +="A possible reason is error in the input file in the time specification - Check your input file ?";
	SetHintMap(h); m_ScanCommonStep = h;

	h.param = GetUniqueMenuNumber();
	h.HintText = "For Various Contour plots when you have more then 2 possible independent variables you may need to select\n";
	h.HintText += "The representation of the 3 possible independent as which hour within day when you have days and years on the other axes\n";
	h.HintText += "or which year when you have days and hours on the other axes\n";
	h.HintText += "Click this button to select you choice of representation";
	SetHintMap(h); m_SelectWithinDay = h;

	h.param = GetUniqueMenuNumber();
	h.HintText ="Enter a posivite number if you want the content of this file to be repeated forward\n" ;
	h.HintText += "Enter a negative number if you want the content to be repeated backwards\n";
	h.HintText += "0 - indicates that only the actual time points will be included as they are.";
	SetHintMap(h);	m_AddScalingCoefHint = h;

	m_VarSelectBox.param = GetUniqueMenuNumber();
	m_VarSelectBox.HintText ="Click on Check box\n or Right Click to select deselect variables";
	SetHintMap(m_VarSelectBox);
	m_VarSelectOptions.param = GetUniqueMenuNumber();
	m_VarSelectOptions.HintText ="Right Click to find options for variables";
	SetHintMap(m_VarSelectOptions);
	m_FileSelectOptions.param = GetUniqueMenuNumber();
	m_FileSelectOptions.HintText = "Right Click to find options for File";
	SetHintMap(m_FileSelectOptions);
	m_ChartNumberSelect.param = GetUniqueMenuNumber();
	m_ChartNumberSelect.HintText = "Single Click to move variable to next chart number";
	SetHintMap(m_ChartNumberSelect);


}

void GridPGDoc::DefineComboList()
{
	CLICK_BUTTON c;
	c.Type = COMBOBOX;
	c.Param= GetUniqueMenuNumber();
	c.Row = 2;
	c.Col_start = 3;
	c.Col_end = 3;
	c.HintText = "Click to Select the Repetition Number to be represented";
	m_RepOptionsCombo = c;
	SetClickButtonMap(c);
	
	c.Param = GetUniqueMenuNumber();
	c.Row = 0;
	c.Col_start = 10;
	c.Col_end = 11;
	c.VectorNames = m_IdentifiedFormatButton.VectorNames;
	c.HintText = "Click to Select a format if not identified correctly by the scanning";
	m_ClickBoxFormatButton = c;
	SetClickButtonMap(c);
	
	c.Param = GetUniqueMenuNumber();
	c.VerticalDirection = false;
	c.NormalState = true;
	c.Col_start = 0;
	c.Col_end = 0;
	c.VectorNames.clear();


	c.VectorNames=p_ModelInfo->GetSortedListofElements();
	m_ElementTypeNewVariables = c;
	SetClickButtonMap(c);

	c.Param = GetUniqueMenuNumber();
	c.Col_start = 14; c.Col_end = 14;
	m_ElementTypeVariables = c;
	SetClickButtonMap(c);
	
	c.Param = GetUniqueMenuNumber();
	c.VerticalDirection = false;
	c.NormalState = true;
	c.Col_start = 0;
	c.Col_end = 0;
	c.VectorNames.clear();

	for (size_t ii = 0; ii < p_ModelInfo->GetNumUnits(); ++ii)
		c.VectorNames.push_back(p_ModelInfo->GetUnitString(UNIT_TYPES(ii)));
	m_UnitTypeNewVariables = c;
	SetClickButtonMap(c);
	
	
	c.Param = GetUniqueMenuNumber();
	c.Col_start = 15; c.Col_end = 15;
	m_UnitTypeVariables = c;
	SetClickButtonMap(c);


	c.Row = 2;
	c.Param=GetUniqueMenuNumber();
	c.Col_start = 7;
	c.Col_end = 7;
	c.VectorNames.resize(4);
	c.VectorNames[0] = "Normal Time";
	c.VectorNames[1] = "Hour";
	c.VectorNames[2] = "Day";
	c.VectorNames[3] = "Year";
	c.HintText = "Click to selected the x-axis of chart";
	c.Text = "Normal Time";
	m_XAxisCombo = c;
	SetClickButtonMap(c);

	c.Param = GetUniqueMenuNumber();
	c.VectorNames[0] = "Not Used";
	c.Col_start = 8;
	c.Col_end = 8;
	c.Text = "Not Used";
	c.HintText = "Click to selected the y-axis of isoline surface chart";
	m_YAxisCombo = c;
	SetClickButtonMap(c);

	c.Param = GetUniqueMenuNumber();
	c.VectorNames.resize(5);
	c.VectorNames[0] = "Scatter chart";
	c.VectorNames[1] = "Time Serie Chart"; 
	c.VectorNames[2] = "Frequence distibution";
	c.VectorNames[3] = "Cumulated distribution";
	c.VectorNames[4] = "Contour Chart";	
	c.Col_start = 0;
	c.Col_end = 0;
	c.Row = 0;
	c.HintText = "Click to Select Chart Type";
	m_ChartTypeOptions = c;
	SetClickButtonMap(m_ChartTypeOptions);
	
	c.Param = GetUniqueMenuNumber();
	c.VectorNames.resize(4);
	c.VectorNames[0] = "Actual Values";
	c.VectorNames[1] = "All Cumulated";
	c.VectorNames[2] = "Year Cumulated";
	c.VectorNames[3] = "Day Cumulated";
	c.Col_start = 1;
	c.Col_end = 1;
	c.Row = 0;
	c.HintText = "Click to Make Simple Transformation of Time Serie Data";
	m_VarTransformationCombo = c;
	SetClickButtonMap(c);

	c.Param = GetUniqueMenuNumber();
	c.Col_start = 0;
	c.Col_end = 0;
	c.Row = 6;
	c.HintText = "Select the Variable that will be the used to define the name of the site";

	m_ComboIdButton = c;
	SetClickButtonMap(c);
	
	c.Param = GetUniqueMenuNumber();
	c.Col_start = 1;
	c.Col_end = 1;
	c.HintText = "Select the Variable that will be the used to define the Country";
	m_ComboCountryButton = c;
	SetClickButtonMap(c);

	c.Param = GetUniqueMenuNumber();
	c.Col_start = 2;
	c.Col_end = 2;
	c.HintText = "Select the Variable that will be the used to define the Latitude";
	m_ComboLatButton = c;
	SetClickButtonMap(c);

	c.Param = GetUniqueMenuNumber();
	c.Col_start = 3;
	c.Col_end = 3;
	c.HintText = "Select the Variable that will be the used to define the Longitude";
	m_ComboLongButton = c;
	SetClickButtonMap(c);
	c.Param = GetUniqueMenuNumber();
	c.Col_start = 4;
	c.Col_end = 4;
	c.HintText = "Select the Variable that will be the used to define the Altitude";
	m_ComboAltButton = c;
	SetClickButtonMap(c);

	c.Param = GetUniqueMenuNumber();
	c.Col_start = 4;
	c.Col_end = 4;
	c.HintText = "Select the Variable that will be the used to define the Altitude";
	m_ComboAltButton = c;
	SetClickButtonMap(c);

	c.Param = GetUniqueMenuNumber();
	c.Row = 1;
	c.Col_start = 4;
	c.Col_end = 4;
	c.HintText = "Select the Variable that will be the used as a filter to make the Output as a function of the values of that Variable";
	m_FilterVariableButton = c;
	SetClickButtonMap(c);
	
	c.Param = GetUniqueMenuNumber();
	c.Col_start = 5;
	c.Col_end = 5;
	c.HintText = "Select the Conditions for what to select to make an output of the other variables";
	m_FilterUseButton = c;
	SetClickButtonMap(c);




	
}
