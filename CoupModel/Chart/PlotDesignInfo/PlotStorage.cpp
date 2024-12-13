// PlotStorage.cpp : implementation file
//
#include "../../ModelTypes/SimB.h"
#include "../../stdafx.h"
#include "../../Util.h"
#ifndef COUPSTD
#include "../../Chart/ChartProperties/ChartProp.h"
#include "../../CoupModel.h"
#include "../../CoupModelDoc.h"
#else 
#include "../../NewBase/SimArchive.h"
#include "../../NewBase/Doc.h"
#endif
#include "./PlotStorage.h"
#include "../ChartProperties/ChartProp.h"


// CPlotStoragePt

CPlotStorage::CPlotStorage()
{

	m_OutputBinFileExist=true;
	m_OutputValidBinFileExist=true;
	m_MultiStorageBinFileExist=true;
	m_NumberSavedCharts=0;
		
	for (size_t i=0;i<MAXPGDOC;i++)
		for (size_t ii=0;ii<MAXSIMVAL;ii++) {
			pg_docindex[i][ii]=-1;
		}

	for(size_t i=0; i<MAXSIMVAL;i++) {
		m_NumberSelectedValidationVar[i]=0;
		m_PG_ALLMULTI[i]= new CPG;
		m_PG_SEL_MULTI[i]= new CPG;
	}
	m_PG= new CPG;
	m_PGVal=new CPG;
	m_PGALL_Out =new CPG;
	m_PGSEL_Out= new CPG;
	m_pCPG=NULL;
	m_CPG_Active=false;
	m_PG_StartMin=-1;
	m_PG_EndMin=-1;
	m_PlotArray.clear();
	m_pProp=MySingle::instance().GetChartPropPointer();
	m_ValidIndepPGVar=false;
}

CPlotStorage::~CPlotStorage()
{

	CPG_Reset();
	for(size_t i=0;i<MAXSIMVAL;i++) {
		if(m_PG_ALLMULTI[i]!=NULL) delete m_PG_ALLMULTI[i];
		if(m_PG_SEL_MULTI[i]!=NULL) delete m_PG_SEL_MULTI[i];
	}
	while(m_PGFileArray.size()>0) {
			CPG *pPG=(CPG*)m_PGFileArray[0];
			delete pPG;
			m_PGFileArray.erase(m_PGFileArray.begin());
			m_PGFileArrayShow.erase(m_PGFileArrayShow.begin());
	}
	if(m_PGSEL_Out!=NULL) delete m_PGSEL_Out;
	if(m_PGALL_Out!=NULL) delete m_PGALL_Out;
	if(m_PGVal!=NULL) delete m_PGVal;
	if(m_PG!=NULL) delete m_PG;
	
}
PLOTv CPlotStorage::GetPlotInfo(size_t index) {
	if(m_PlotArray.size()>index) return m_PlotArray[index];
	else {
		PLOTv blank;
		blank.pBase=NULL;
		blank.pPG=NULL;
		blank.pSimDoc=NULL;
		return blank;
	}
}


// CPlotStorage member functions

void CPlotStorage::Update(size_t RunNo, size_t NumberRead)
{
	m_SimulationRunNo=RunNo;
	m_FileVersionNumberRead=NumberRead;
}

bool CPlotStorage::Init(Doc *pDoc)
{

	m_pSimDoc=pDoc;
	m_SimulationRunNo=m_pSimDoc->m_DocFile.m_SimulationRunNo;
	m_FileVersionNumberRead=m_pSimDoc->m_FileVersionNumberRead;
	size_t v=m_PlotArray.size();
	return true;
}
bool CPlotStorage::Add(void *pXG, size_t type, size_t index, size_t chartnum, CHART_TYPES charttyp, string docname, size_t ValFileNo, size_t vnum, size_t runno, size_t section_no, size_t sub_chart, size_t logli_ver, size_t repit_no)
{
	size_t iDoc=-1;
	bool MainDoc=true;
	size_t SimRunNo;
	PLOTv plot;
	CPG *pPG=NULL;

	if(docname=="") {
		docname=m_pSimDoc->GetCurrentSimFile();
		//docname=GetOutputFileName(ValFileNo);
	}
	if(chartnum==0) {
			chartnum=GetCurrentChart()+1;
	}

	for (size_t i=0;i<m_XTGD_DocNameArray.size();i++) {
		if(m_XTGD_DocNameArray[i]==docname){
			iDoc=i;
			break;
		}
	}
	if(m_pSimDoc->GetCurrentSimFile()==docname.c_str()) {
		SimRunNo=m_pSimDoc->m_DocFile.m_SimulationRunNo;
		auto iposext=docname.find(".S");
		if(iposext>=0&&iposext!=string::npos) {
			string txt=docname.substr(iposext-5,5);
			size_t SimRunNoFile=FUtil::AtoInt(txt);
			if(SimRunNoFile!=SimRunNo&&SimRunNoFile>0) {
				m_pSimDoc->m_DocFile.m_SimulationRunNo=SimRunNoFile;
				SimRunNo=SimRunNoFile;
			}
		}


	}
	else if(docname.size()>0) {
		auto iposext=docname.find(".S");
		string txt=docname.substr(iposext-5,5);
		SimRunNo=FUtil::AtoInt(txt);
	}
	else
		SimRunNo=0;
	if(iDoc<0) {
		m_XTGD_DocNameArray.push_back(docname);
		iDoc=size_t(m_XTGD_DocNameArray.size()-1);
	}
	if(type>=6&&type<10) {
		if(!m_pSimDoc->MR_Storage_Open()) {
			m_pSimDoc->SetMultiStorageBinFileMissing();
			return false;
		}
	}

	

	

	plot.EnsembleType = ENSEMBLE_TYPE::NO;


	plot.pBase=(SimB*)pXG;
	plot.pPG=pPG;

	//plot.PGFileIndex=GetVar_Index(m_PlotArray.size());
	plot.Doc=iDoc;
	//m_XTGD_Doc_PlotArray.Add(iDoc);
	plot.ValFileNo=ValFileNo;
	//m_XTGD_ValFileNo_Array.Add(ValFileNo);

	// Array of pointer to Document
	plot.Residual_VarNum=vnum;
	//m_Residual_VarNum.Add(vnum);
	plot.Residual_RunNo=runno;
	//m_Residual_RunNo.Add(runno);
	plot.PGSection=section_no;
	//m_PGSection_No.Add(section_no);
	plot.Type=type;
	//m_XTGD_Type_PlotArray.Add(type);	// Array of indicators Variable type
	plot.Index=index;
	//m_XTGD_Index_PlotArray.Add(index);	// Array of local indices for variables
	plot.Chart=chartnum;
	//m_XTGD_Chart_PlotArray.Add(chartnum);	// Array of Chart no indicators

#ifndef COUPSTD	
	plot.ChartType=charttyp;
#else
	plot.ChartType= CHART_TYPES::TIME_SERIE;
#endif

	plot.SubChart_No=sub_chart;
	//m_SubChart_No.Add(sub_chart);
	plot.LogLiVersion=logli_ver;
	//m_LogLiVersion.Add(logli_ver);
	plot.PGRepitNo=repit_no;
	m_PlotArray.push_back(plot);
	
	return true;
}
size_t CPlotStorage::AddPlotInfo(PLOTv add, size_t chartnum) {
	if(chartnum==0) chartnum=GetCurrentChart()+1;
	add.Chart=size_t(chartnum);
	m_PlotArray.push_back(add);
	return m_PlotArray.size();
}

bool CPlotStorage::Add(void *pXG, size_t type, size_t index, size_t chartnum, CHART_TYPES charttyp, size_t iDoc, size_t ValFileNo, size_t vnum, size_t runno, size_t section_no, size_t sub_chart, size_t logli_ver, size_t repit_no)
{
	PLOTv plot;
	plot.pBase=(SimB*)pXG;
	plot.Doc=iDoc;
	plot.ValFileNo=ValFileNo;
	plot.Residual_VarNum=vnum;
	plot.Residual_RunNo=runno;
	plot.PGSection=section_no;
	plot.Type=type;
	plot.Index=index;
	plot.Chart=chartnum;
	plot.ChartType=charttyp;
	plot.SubChart_No=sub_chart;
	plot.LogLiVersion=logli_ver;
	plot.PGRepitNo=repit_no;
	m_PlotArray.push_back(plot);	
	return true;
}
bool CPlotStorage::AddCurve(size_t add_section, size_t backshift)
{
	auto j=m_PlotArray.size()-1-backshift;
	if(j==string::npos||m_PlotArray.size()<=j) return false;
	PLOTv plot;
	plot=m_PlotArray[j];
	if(add_section==5000) 
		plot.EnsembleType=ENSEMBLE_TYPE::ALL;
	else
		plot.EnsembleType = ENSEMBLE_TYPE::NO;

	if(add_section!=0)
		plot.PGSection=add_section;

	
	m_PlotArray.push_back(plot);
	return true;
}
bool CPlotStorage::Remove(SimB *pXG)
{
	SimB *p;
	for(size_t i=0;i<m_PlotArray.size();i++) {
		p=(SimB*)m_PlotArray[i].pBase;
		if(p==pXG) {
			m_PlotArray.erase(m_PlotArray.begin()+i);
			return true;
		}
	}
	return false;
}


size_t CPlotStorage::GetValFileNo(size_t index) 
{
	if(index<m_PlotArray.size()) {
		return m_PlotArray[index].ValFileNo;
	}
	else
		return 1;

}
size_t CPlotStorage::GetDocIndex(size_t index) 
{
	if(index<m_PlotArray.size()) {
		return m_PlotArray[index].Doc;
	}
	else
		return 0;

}
bool CPlotStorage::Remove(size_t index)
{	if(index<m_PlotArray.size()) {
			m_PlotArray.erase(m_PlotArray.begin()+index);
			return true;
		}
	return false;
}

bool CPlotStorage::RemoveChart(size_t index)
{ bool removed=false;
	for(size_t i=0;i<m_PlotArray.size();i++) {
		  if(m_PlotArray[i].ChartNumber==index) {
				m_PlotArray.erase(m_PlotArray.begin()+i);
				removed=true;
				break;
		  }	
	
	}
	return removed;
}
bool CPlotStorage::SetEnsembleType(size_t index, ENSEMBLE_TYPE ensemtype)
{
	bool changed;
	changed = false;
	if (index<m_PlotArray.size()) {
		m_PlotArray[index].EnsembleType = ensemtype;
		changed = true;
	}
	return changed;
	
}
bool CPlotStorage::SetDataType(size_t index, enum DATA_TYPES type)
{	bool changed;
	changed=false;
	if(index<m_PlotArray.size()){
		 m_PlotArray[index].DataType=type;
				changed=true;
	}
	return changed;
}
bool CPlotStorage::SetChartType(size_t index, enum CHART_TYPES chart_type)
{
	bool changed;
	changed = false;
	if (index<m_PlotArray.size()) {
		m_PlotArray[index].ChartType = chart_type;
		changed = true;
	}
	return changed;
}
bool CPlotStorage::RemoveAll()
{	m_PlotArray.clear();
	m_XTGD_DocNameArray.clear();
	return true;
}

bool CPlotStorage::SetConstrainBy(size_t index, vector<CONSTRAIN_BY> con)
{
	bool changed;
	changed = false;
	if (index < m_PlotArray.size()) {
		m_PlotArray[index].m_ConstrainBy = con;
		changed = true;
	}
	return changed;
}

size_t CPlotStorage::NumSelected()
{	return m_PlotArray.size();
}
bool CPlotStorage::IsMultiRunPlot(size_t index)
{
	auto itype=GetDataType(index);
	if(itype>=DATA_TYPES::SIM_PERFORMANCE&&itype<DATA_TYPES::PGDATA)
		return true;
	else
		return false;

}
size_t CPlotStorage::GetNumRecords(size_t indexchart)
{	
	
	size_t idoc;
	size_t records;
	for(size_t i=0;i<m_PlotArray.size();i++) {
		if(m_PlotArray[i].Index==indexchart) {
			auto itype=GetDataType(i);
			idoc=GetDocIndex(i);
			if (itype >= DATA_TYPES::SIM_PERFORMANCE&&itype<DATA_TYPES::PGDATA) {
					records=m_pSimDoc->MR_Get_TotalNumberofRuns();
				}
			else if(itype==DATA_TYPES::PGDATA){
					if(m_PG==NULL){
						return -1;
					}
					else if(!m_PG->IsOpen()) {
					//	MessageBox("Could not open PG file");
						return -1;
					}					
					else	
						records=m_PG->GetNumRecords();
				}
			else {
#ifndef COUPSTD
				CPG *pPG=GetPGPointer(idoc);
				if(pPG!=NULL) { 
					records=pPG->GetNumRecords();
				}
				else
#endif
					records=-1;
	
			}
			return records;
		}
	}
	return -1;
}


CPG* CPlotStorage::GetPGValFilePointer(size_t indexdoc, size_t ValidIndex, size_t simRunNo, bool ObsFile) {
	CPG *pPG;
	size_t indexpg;


// Create a link to a PGValidRes File
			bool FirstTry, SecondTry;
			string filename;
			if(indexdoc>=0) 
				filename=GetDocFileName(indexdoc).c_str();



			auto iposext=filename.find(".S");
			auto ipos=filename.rfind('\\');
						

			string pgfilename;

			FirstTry=true;
			SecondTry=true;
			string str;
			if(simRunNo==0) simRunNo=m_pSimDoc->m_DocFile.m_SimulationRunNo;

			if(simRunNo>0) {

				if(m_pSimDoc->m_FileVersionNumberRead<60||!ObsFile) {
					pgfilename=m_pSimDoc->GetOutputFileName(ValidIndex).c_str();
								   //    GetOutputFileName
					//str.Format("%05u",simRunNo);
					str=FUtil::ItoNumAscii(simRunNo);
					pgfilename=filename.substr(0,ipos+1)+"V"+FUtil::STD_ItoAscii(ValidIndex)+"_Coup_"+str+".Bin";
				}
				else {
					F *pF=m_pSimDoc->ValidationFilePointer(ValidIndex);
					pF->CheckFileNameAndAssignNameToPGClass();
				    pF->CallDataDescription();
					return pF->GetPointer();
				}

			}
			else {
				str=filename.substr(iposext-5,5);
				pgfilename=filename.substr(0,ipos+1)+"V"+FUtil::STD_ItoAscii(ValidIndex)+"_Coup_"+str+".Bin";
			}


			while(!FUtil::IsFileExisting(string(pgfilename))&&SecondTry) {
				auto iposmul=filename.find("_Multi");
				if(iposmul!=string::npos) {
					if(simRunNo==0) {
						string str;
						str=FUtil::ItoNumAscii(simRunNo);
						pgfilename=filename.substr(0,iposmul)+"\\V"+FUtil::STD_ItoAscii(ValidIndex)+"_Coup_"+str+".Bin";
					}
					else if(simRunNo>0) {
						string str;
						str=FUtil::ItoNumAscii(simRunNo);
						pgfilename=filename.substr(0,iposmul)+"\\V"+FUtil::STD_ItoAscii(ValidIndex)+"_Coup_"+str+".Bin";

					}
					else
						pgfilename=filename.substr(iposmul)+"\\V"+FUtil::STD_ItoAscii(ValidIndex)+"_Coup_"+filename.substr(iposext-5,5)+".Bin";
				}
				if(FirstTry)
					FirstTry=false;
				else if(SecondTry)
					SecondTry=false;	
			}

			

		
			for(size_t i=0; i<m_FailedNames.size();i++) {
				if(m_FailedNames[i].c_str()==pgfilename)
						return NULL;
			}			
			if(SecondTry!=true) {
				m_FailedNames.push_back(string(pgfilename));
				return NULL;



			}


			pPG=new CPG;

			pPG->Open(string(pgfilename),false);
			if(pPG->IsOpen()){
				pPG->CloseFile();
				if(indexdoc>=0) {
					indexpg=m_CPGVal_Array.size();
					pg_docindex[indexdoc][ValidIndex]=indexpg;
					m_CPGVal_Array.push_back(pPG);

				}
				return pPG;
			}
			else {
				delete pPG;
				return NULL;
			}

}
#ifndef COUPSTD
CPG* CPlotStorage::GetPGPointer(size_t indexdoc, size_t ValidIndex, size_t simRunNo, bool MultiModeFile, size_t MeanPGType)
{	
	CPG *pPG;
	bool pgfind=true;
	bool pgopen=true;
	bool pgset=false;
	pPG=NULL;
	size_t indexpg;
	
	if(indexdoc<CPG_GetNumber()&&ValidIndex==0&&MeanPGType==0){
		pPG=CPG_GetAt(indexdoc);
		if(pPG==NULL) {
			string filename=GetDocFileName(indexdoc);
		//	CWinApp* pApp = AfxGetApp();
		//	CCoupModelApp *pCoupModel=(CCoupModelApp*)pApp;
			for(size_t i=0; i<m_FailedNames.size();i++) {
				if(m_FailedNames[i]==filename)
						return NULL;
			}			
		
			/* Removed since it shift the pointer to the Document
			if(!pCoupModel->OnFileOpen(filename.c_str())){
				return NULL;
				
			};*/
			pgfind=true;

		}
		else {
			pgfind=false;
			pgset=true;
		}
	}
	else if(ValidIndex>0) {
		if(indexdoc>19) {
#ifndef COUPSTD
			MFC_Util::MessageBox("Too many documents open, Clean Chart List !","Error when open file",MB_OK | MB_ICONERROR);
#endif
		
			return pPG;
		}
		if(indexdoc>=0) 
			indexpg=pg_docindex[indexdoc][ValidIndex];
		else
			indexpg=-1;
		if(indexpg==-1) {
// Create a link to a PGValidRes File
			bool FirstTry, SecondTry;
			string filename;
			if(indexdoc>=0) 
				filename=GetDocFileName(indexdoc).c_str();



			size_t iposext=filename.find(".S");
			size_t ipos=filename.rfind('\\');
						

			string pgfilename;

			FirstTry=true;
			SecondTry=true;
			string str;
			if(simRunNo==0) simRunNo=m_pSimDoc->m_DocFile.m_SimulationRunNo;

			if(simRunNo>0) {

				if(m_pSimDoc->m_FileVersionNumberRead<60) {
					pgfilename=m_pSimDoc->GetOutputFileName(ValidIndex).c_str();
								   //    GetOutputFileName
					str=FUtil::ItoNumAscii(simRunNo);
					pgfilename=filename.substr(0,ipos+1)+"V"+FUtil::STD_ItoAscii(ValidIndex)+"_Coup_"+str+".Bin";
				}
				else {
					F *pF=m_pSimDoc->ValidationFilePointer(ValidIndex);
					pF->CheckFileNameAndAssignNameToPGClass();
				    pF->CallDataDescription();
					return pF->GetPointer();
				}

			}
			else {
				str=filename.substr(iposext-5,5);
				pgfilename=filename.substr(0,ipos+1)+"V"+FUtil::STD_ItoAscii(ValidIndex)+"_Coup_"+str+".Bin";
			}


			while(!FUtil::IsFileExisting(string(pgfilename))&&SecondTry) {
				size_t iposmul;
				iposmul=filename.find("_Multi");
				if(iposmul>0) {
					if(simRunNo==0) {
						string str;
						str=FUtil::ItoNumAscii(m_pSimDoc->m_DocFile.m_SimulationRunNo);
						pgfilename=filename.substr(0,iposmul)+"\\V"+FUtil::STD_ItoAscii(ValidIndex)+"_Coup_"+str+".Bin";
					}
					else if(simRunNo>0) {
						string str;
						str=FUtil::ItoNumAscii(simRunNo);
						pgfilename=filename.substr(0,iposmul)+"\\V"+FUtil::STD_ItoAscii(ValidIndex)+"_Coup_"+str+".Bin";

					}
					else
						pgfilename=filename.substr(iposmul)+"\\V"+FUtil::STD_ItoAscii(ValidIndex)+"_Coup_"+filename.substr(iposext-5,5)+".Bin";
				}
				if(FirstTry)
					FirstTry=false;
				else if(SecondTry)
					SecondTry=false;	
			}

			

		
			for(size_t i=0; i<m_FailedNames.size();i++) {
				if(m_FailedNames[i].c_str()==pgfilename)
						return NULL;
			}			
			if(SecondTry!=true) {
				m_FailedNames.push_back(string(pgfilename));
				return NULL;



			}


			pPG=new CPG;

			pPG->Open(string(pgfilename),false);
			if(pPG->IsOpen()){
				pPG->CloseFile();
				if(indexdoc>=0) {
					if(ValidIndex==0&&MultiModeFile) {



					}
					else {
						indexpg=m_CPGVal_Array.size();
						pg_docindex[indexdoc][ValidIndex]=indexpg;
						m_CPGVal_Array.push_back(pPG);
					}
				}
				return pPG;
			}
			else {
				delete pPG;
				return NULL;
			}
		}
		else {

			if(m_CPGVal_Array.size()>indexpg) {
				pPG=(CPG*)m_CPGVal_Array[indexpg];
            	return pPG;
			}
		}
	}


	
	if(MeanPGType>0) pgfind=true;
	
	while (pgfind) {
		string filename=GetDocFileName(indexdoc);
		CDocument *pDocument;
		CMultiDocTemplate* pDocTemplate;
		CWinApp* pApp = AfxGetApp();
		CCoupModelApp *pCoupModel=(CCoupModelApp*)pApp;
	
		pDocTemplate=pCoupModel->m_pDocTemplate;
		POSITION pos2= pDocTemplate->GetFirstDocPosition();

		CCoupModelDoc* dg_pDoc;
		//=(CPlotStorage*)pDocTemplate->GetNextDoc(pos2);
		size_t numdoc=0;

		while(pos2) { 
		if(pDocument = pDocTemplate->GetNextDoc(pos2)) {
				//m_pDocArray.Add((CPlotStorage*)pDocument);
			dg_pDoc =(CCoupModelDoc*)pDocument;
			if(dg_pDoc->GetPathName()==filename.c_str()){
					string pgFilename;
					if(MeanPGType==1) {
						pPG=dg_pDoc->m_Doc.m_PlotStorage.m_PGALL_Out;
						pgFilename=dg_pDoc->m_Doc.GetOutputFileName(ValidIndex, false, MultiModeFile, MeanPGType);
					}
					else if(MeanPGType==2) {
						pPG=dg_pDoc->m_Doc.m_PlotStorage.m_PGSEL_Out;
						pgFilename=dg_pDoc->m_Doc.GetOutputFileName(ValidIndex, false, MultiModeFile, MeanPGType);
					}
					else {
						pPG=dg_pDoc->m_Doc.m_PlotStorage.m_PG;
						if(MultiModeFile) {
							size_t index=dg_pDoc->m_Doc.m_MStorage.GetAdd_MultiPGFileIndex(simRunNo);
							pgFilename=dg_pDoc->m_Doc.GetOutputFileName(ValidIndex, false, MultiModeFile,  MeanPGType, index);
							CPG_SetTypeFile(indexdoc,true);
						}
						else {
							pgFilename=dg_pDoc->m_Doc.GetOutputFileName(ValidIndex, false, MultiModeFile,  MeanPGType);
							//pgFilename = pPG->GetFileName();
						}


					}
					
					if(!pPG->IsOpen()) {
							if(pgFilename=="") {
								m_OutputBinFileExist=false;
								return NULL;

							}
							for(size_t i=0; i<m_FailedNames.size();i++) {
								if(m_FailedNames[i]==pgFilename.c_str()) {
									return NULL;
								}
							}
							if(!pPG->Open(pgFilename.c_str(),false)) { 
								string name;
								name=MFC_Util::GetProfileStringStd("Failed_Name", name);
								m_FailedNames.push_back(name);
								return NULL;
							}
					}
					pPG->CloseFile();
				pgopen=false;
				pgfind=false;
			}
			numdoc++;
		}

	}

		pgfind=false;
	}
	if(!pgset&&ValidIndex==0&&pPG!=NULL&&MeanPGType==0) {
		if(!CPG_SetAt(indexdoc, pPG, MultiModeFile))
			return NULL;
			
	}
	return pPG;
}
Doc* CPlotStorage::GetDocPointer(size_t indexdoc)
{
	Doc *pDoc;
	pDoc=NULL;

	MySingle::instance().GetSimArchivePointer()->GetSimDocPointer(indexdoc);
	string filename=GetDocFileName(indexdoc);
	CDocument *pDocument;
	CMultiDocTemplate* pDocTemplate;
	CWinApp* pApp = AfxGetApp();
	CCoupModelApp *pCoupModel=(CCoupModelApp*)pApp;
	
	pDocTemplate=pCoupModel->m_pDocTemplate;
	POSITION pos2= pDocTemplate->GetFirstDocPosition();

	CCoupModelDoc* dg_pDoc;
		//=(CPlotStorage*)pDocTemplate->GetNextDoc(pos2);
	size_t numdoc=0;

	while(pos2) { 
		if(pDocument = pDocTemplate->GetNextDoc(pos2)) {
				//m_pDocArray.Add((CPlotStorage*)pDocument);
			dg_pDoc =(CCoupModelDoc*)pDocument;
			if(dg_pDoc->GetPathName()==filename.c_str()){
					pDoc=&dg_pDoc->m_Doc;
			}
		}
	}


	return pDoc;

}
#endif
size_t CPlotStorage::GetNumVar(size_t indexchart)
{	
	size_t ivar=0;
	for(size_t i=0;i<m_PlotArray.size();i++) {
		if(m_PlotArray[i].Chart==indexchart) {
				ivar++;
		  }	
	}
	return min(size_t(100),ivar);
}
vector<size_t> CPlotStorage::GetNumVarForEachSubChart(size_t indexchart) {
	size_t index;
	vector<size_t> numcurves;
	for (size_t i = 0; i < m_PlotArray.size(); i++) {
		if (m_PlotArray[i].Chart == indexchart) {
			index = m_PlotArray[i].SubChart_No - 1;
			if (index <= numcurves.size()) numcurves.resize(index + 1);
			numcurves[index]++;
		}
	}
	return numcurves;
}
size_t CPlotStorage::GetNumVarBands(size_t indexchart)
{	
	size_t ivar=0;
	for(size_t i=0;i<m_PlotArray.size();i++) {
		if(m_PlotArray[i].Chart==indexchart) {
			if(m_PlotArray[i].EnsembleType>ENSEMBLE_TYPE::NO)	ivar++;
		  }	
	}
	return min(size_t(100),ivar);
}
size_t CPlotStorage::GetArrayIndex(size_t indexchart, size_t indexVar)
{	
	size_t ivar=0;
	for(size_t i=0;i<m_PlotArray.size();i++) {
		  if(m_PlotArray[i].Chart==indexchart) {
				if(ivar==indexVar) {
					return i;
				}
				ivar++;
		  }	
	}
	return string::npos;
}

size_t	CPlotStorage::GetValVarNo(size_t index)
{
	return m_PlotArray[index].Residual_VarNum;
}
size_t	CPlotStorage::GetValRunNo(size_t index)
{
	return m_PlotArray[index].Residual_RunNo;
}
bool CPlotStorage::SetValRunNo(size_t index, size_t value_runno, size_t sequence_no)
{
	if(m_PlotArray.size()>index) {
		m_PlotArray[index].Residual_RunNo=value_runno;
		m_PlotArray[index].Residual_Sequence = sequence_no;
		return true;
	}
	else
		return false;
}
void* CPlotStorage::GetPtr(size_t index)
{	return (void*)m_PlotArray[index].pBase;
}


string CPlotStorage::GetDocFileName(size_t index)
{
	if(m_XTGD_DocNameArray.size()>index)
		return m_XTGD_DocNameArray[index];
	else	{
		string str;
#ifndef COUPSTD
		CDocument *pDocument;
		CMultiDocTemplate* pDocTemplate;
		CWinApp* pApp = AfxGetApp();
		CCoupModelApp *pCoupModel=(CCoupModelApp*)pApp;
	
		pDocTemplate=pCoupModel->m_pDocTemplate;
		POSITION pos2= pDocTemplate->GetFirstDocPosition();
		pDocument = pDocTemplate->GetNextDoc(pos2);
		CCoupModelDoc *dg_pDoc =(CCoupModelDoc*)pDocument;
		str=CT2A(dg_pDoc->GetPathName()).m_psz;
#else
		str="";
#endif
		return str;
	}
}

size_t CPlotStorage::GetLocalIndex(size_t index)
{	
	return m_PlotArray[index].Index;
}
bool CPlotStorage::SetLocalIndex(size_t index, size_t value)
{	
	if((m_PlotArray.size())>index) {
		m_PlotArray[index].Index=value;
		return true;
	}
	return false;
}

enum  ENSEMBLE_TYPE CPlotStorage::GetEnsembleType(size_t index)
{	
	if(index<m_PlotArray.size())
		return m_PlotArray[index].EnsembleType;
	else
		return ENSEMBLE_TYPE::NO;
}
enum  DATA_TYPES CPlotStorage::GetDataType(size_t index)
{
	if (index<m_PlotArray.size())
		return m_PlotArray[index].DataType;
	else
		return DATA_TYPES::ANYDATA;

}
bool CPlotStorage::GetRepitType(size_t index)
{
	if (index<m_PlotArray.size())
		return m_PlotArray[index].RepitDataType;
	else
		return false;
}
size_t CPlotStorage::GetChart(size_t index)
{	
	if(index<m_PlotArray.size())
		return m_PlotArray[index].Chart;
	else
		return 0;
}
size_t CPlotStorage::GetCurrentChart()
{
	auto index=m_PlotArray.size()-1;
	if(index==string::npos)
		return 0;
	else
		return m_PlotArray[index].Chart;
}
CHART_TYPES CPlotStorage::GetChartType(size_t index)
{	if(index>=0&&index!=string::npos)
		return m_PlotArray[index].ChartType;
	else
		return CHART_TYPES::TIME_SERIE;
}
size_t CPlotStorage::GetNumberofCharts()
{
	size_t index=m_PlotArray.size();
	if(index==0)
		return 0;
	else
		return m_PlotArray[index-1].ChartNumber;
}
size_t CPlotStorage::GetIndex_Chart(size_t iv)
{
	bool test;
	test=true;
	size_t num;
	size_t index=string::npos;
	for (size_t j=0;j<m_PlotArray.size();j++) {
		test=true;
		num=m_PlotArray[j].ChartNumber;
		for (size_t j2=0;j2<m_PlotArray.size();j2++) {
			if(iv==m_PlotArray[j2].ChartNumber){
				return j;
			}
			else if(num==m_PlotArray[j2].ChartNumber)
				test=false;
		}
/*REMAIN*		if(test) {
			m_XTGD_ChartNumbers.Add(m_XTGD_Chart_PlotArray.GetAt(j));
			j--;

		}*/
	}
	return size_t(index);
}
size_t CPlotStorage::GetVar_Index(size_t index)
{  // Returns the index from Pg file or M_Storage to
	// exctracted for  plotting
	size_t varindex;
	auto ilocal=GetLocalIndex(index);
	auto itype=GetDataType(index);
	SimB *Ptr=(SimB*)GetPtr(index);
	
#ifndef COUPSTD

	PLOTv p_info=m_pSimDoc->m_PlotStorage.GetPlotInfo(index);


	SimB *pX=NULL;
	if(Ptr!=NULL) {
			CPG *pPG1=(CPG*)Ptr;
			pX=(SimB*)Ptr;		
	}
	Doc* pDocAct;
	auto idoc=m_pSimDoc->m_PlotStorage.GetDocIndex(index);
	Doc *pD=MySingle::instance().GetSimArchivePointer()->GetSimDocPointer(idoc);
	//Doc *pD=m_pSimDoc->m_PlotStorage.GetDocPointer(idoc);
	if(pD!=NULL)
		pDocAct=pD;
	else
		pDocAct=m_pSimDoc;
		
	varindex = 1;
	switch(itype) {
	case DATA_TYPES::ANYDATA:
	case DATA_TYPES::PGDATA:
				varindex = p_info.PGFileIndex;
				if(varindex==0&&ilocal!=0)	varindex=ilocal;
				break;

			case DATA_TYPES::SIM_X:
			case DATA_TYPES::SIM_T:
			case DATA_TYPES::SIM_G:
			case DATA_TYPES::SIM_D:

				if(pX==NULL)
					varindex=ilocal;
				else if(pX->Is_Vector()) {
					if(m_pSimDoc->IsMultiSimulation())
						varindex=((OutVector*)pX)->GetPgFileIndex(ilocal);
					else
						varindex=((OutVector*)pX)->GetPgSingleFileIndex(ilocal);
				}
				else {
					if(m_pSimDoc->IsMultiSimulation())
						varindex=((OutSingle*)pX)->GetPgFileIndex();
					else
						varindex=((OutSingle*)pX)->GetPgSingleFileIndex();
				}
				break;
			case DATA_TYPES::SIM_VAL: //Validation section
			case DATA_TYPES::MEASURED_VAL:
				varindex=p_info.PGFileIndex;
				break;
			case DATA_TYPES::SIM_PERFORMANCE:
				// Validation Multi Data
				
					varindex=ilocal/6;
					break;
			case DATA_TYPES::SIM_PAR:
					varindex=ilocal;
					break;
			case DATA_TYPES::SIM_OUTSUM:

					varindex=ilocal-6;
					break;
			case DATA_TYPES::SIM_SUMVAR:
		// Validation Multi Data
					varindex=ilocal/3;
					break;
		}
#else
	varindex = 0;
#endif
		return varindex;

}

string CPlotStorage::GetActualVarName()
{
return m_PG_Name;
}
string CPlotStorage::GetActualVarUnit()
{
return m_PG_Unit;
}

void CPlotStorage::SetNumberSavedCharts(size_t iv)
{
	m_NumberSavedCharts=iv;
}
size_t CPlotStorage::GetNumberSavedCharts()
{
	return m_NumberSavedCharts;
}
size_t CPlotStorage::GetNumberofDocuments()
{
	return	m_XTGD_DocNameArray.size();
}
bool CPlotStorage::CPG_Add(CPG *p)
{
	if(p!=NULL) {
	  m_CPG_Array.push_back(p);
	  m_CPG_MultiType.push_back(0);
	  return true;
	}
	else
		return false;
}
bool CPlotStorage::CPG_Reset()
{	CPG *pCPG;
	bool test;
	m_CPG_Active=false;
	bool removed=false;
	test=false;
	if(m_CPG_Array.size()>0){
		while(m_CPG_Array.size()>0){
			pCPG=(CPG*)m_CPG_Array[0];
			test=false;
			for(size_t i=0;i<MAXSIMVAL;i++) {
				if(m_PG_ALLMULTI[i]==pCPG) test=true;
				if(m_PG_SEL_MULTI[i]==pCPG) test=true;
			}
			//pCPG->Reset();
			if(pCPG!=m_PG&&!test){
				delete pCPG;
				pCPG=NULL;
			}
			m_CPG_Array.erase(m_CPG_Array.begin()+0);
			m_CPG_MultiType.erase(m_CPG_MultiType.begin()+0);
			removed=true;
		}
	}
	if(m_CPGVal_Array.size()>0){
		while(m_CPGVal_Array.size()>0){
			pCPG=(CPG*)m_CPGVal_Array[0];
			
			test=false;
			for(size_t i=0;i<MAXSIMVAL;i++) {
				if(m_PG_ALLMULTI[i]==pCPG) test=true;
				if(m_PG_SEL_MULTI[i]==pCPG) test=true;
			}
			//pCPG->Reset();
			if(pCPG!=m_PG&&!test) {
				delete pCPG;
				pCPG=NULL;
			}
			m_CPGVal_Array.erase(m_CPGVal_Array.begin());
		}
	}
	if (m_pCPG!=NULL) {
		m_pCPG->Reset();
		delete m_pCPG;
		m_pCPG=NULL;
		removed=true;
	}
	
	if(removed) return true;
	return false;
}
bool CPlotStorage::CPG_RemoveAt(size_t index)
{
	if(m_CPG_Array.size()>index){
		delete m_CPG_Array[index];
		m_CPG_Array.erase(m_CPG_Array.begin()+index);
		m_CPG_MultiType.erase(m_CPG_MultiType.begin()+index);
		return true;
	}
	else
		return false;
}
bool CPlotStorage::CPG_SetAt(size_t index, CPG *p, bool MultiFile)
{
	if(m_CPG_Array.size()>index){
		delete m_CPG_Array[index];
		m_CPG_Array[index]=p;
		m_CPG_MultiType[index]=MultiFile;
		return true;
	}
	else if(p!=NULL){

		m_CPG_Array.resize(index+1);
		m_CPG_MultiType.resize(index+1);
		m_CPG_Array[index]= p;
		m_CPG_MultiType[index]=MultiFile;
		return true;
	}
	else
		return false;
}
CPG* CPlotStorage::CPG_GetAt(size_t index)
{
	if(m_CPG_Array.size()>index)
		return (CPG*)m_CPG_Array[index];
	else
		return NULL;
}
CPG* CPlotStorage::CPG_GetSimFile()
{
	return m_pCPG;
}
size_t CPlotStorage::CPG_GetNumber()
{
	return m_CPG_Array.size();
}




bool CPlotStorage::CPGVal_Add(CPG *p)
{
	if(p!=NULL) {
	  m_CPGVal_Array.push_back(p);
	  return true;
	}
	else
		return false;
}
bool CPlotStorage::CPGVal_Reset()
{	CPG *pCPG;
	m_CPGVal_Active=false;
	bool removed=false;
	if(m_CPGVal_Array.size()>0){
		while(m_CPGVal_Array.size()>0){
			pCPG=(CPG*)m_CPGVal_Array[0];
			//pCPG->Reset();
			if(pCPG!=m_PG)
				delete pCPG;
			m_CPG_Array.erase(m_CPG_Array.begin());
			m_CPG_MultiType.erase(m_CPG_MultiType.begin());
			removed=true;
		}
	}
	if (m_pCPG!=NULL) {
		m_pCPG->Reset();
		delete m_pCPG;
		m_pCPG=NULL;
		removed=true;
	}
	
	if(removed) return true;
	return false;
}
bool CPlotStorage::CPGVal_RemoveAt(size_t index)
{
	if(m_CPGVal_Array.size()>index){
		delete m_CPGVal_Array[index];
		m_CPGVal_Array.erase(m_CPGVal_Array.begin()+index);
		return true;
	}
	else
		return false;
}
bool CPlotStorage::CPGVal_SetAt(size_t index, CPG *p)
{
	if(m_CPGVal_Array.size()>index){
		delete m_CPGVal_Array[index];
		m_CPGVal_Array.erase(m_CPGVal_Array.begin()+index);
		m_CPGVal_Array[index]= p;
		return true;
	}
	else if(p!=NULL){
		m_CPGVal_Array[index]= p;
		return true;
	}
	else
		return false;
}
CPG* CPlotStorage::CPGVal_GetAt(size_t index)
{
	if(m_CPGVal_Array.size()>index)
		return (CPG*)m_CPGVal_Array[index];
	else
		return NULL;
}
CPG* CPlotStorage::CPGVal_GetSimFile()
{
	return m_pCPG;
}
size_t CPlotStorage::CPGVal_GetNumber()
{
	return m_CPGVal_Array.size();
}


bool CPlotStorage::CPG_SetPGSimFile()
{
	if(m_pCPG!=NULL) {
		return true;
	}

		string dir=m_pSimDoc->GetCurrentWorkDirectory();
		string filename, str;
		bool validpg;
		str=FUtil::STD_ItoAscii(m_pSimDoc->m_DocFile.m_SimulationRunNo);
		if(str.size()>4) 
			filename=dir+"COUP_"+str+".Bin";
		else if(str.size()==4)
			filename=dir+"COUP_0"+str+".Bin";	
		else if(str.size()==3)
			filename=dir+"COUP_00"+str+".Bin";
		else if(str.size()==2)
			filename=dir+"COUP_000"+str+".Bin";
		else if(str.size()==1)
			filename=dir+"COUP_0000"+str+".Bin";
		else
			filename=dir+"COUP_"+str+".Bin";
		auto num=CPG_GetNumber();
		CPG *pCPG;


		pCPG= new CPG();
		validpg=pCPG->Open(filename.c_str());
		if(validpg) {
			m_pCPG=pCPG;
			return true;
		}
		else {
			m_pCPG=NULL;
			delete pCPG;
			return false;
		}

}


bool CPlotStorage::CPGVal_SetPGSimFile()
{
	if(m_pCPG!=NULL) {
		return true;
	}
		string dir=m_pSimDoc->GetCurrentWorkDirectory();
		string filename, str;
		bool validpg;
		str=FUtil::STD_ItoAscii(m_pSimDoc->m_DocFile.m_SimulationRunNo);
		if(str.size()>4) 
			filename=dir+"COUP_"+str+".Bin";
		else if(str.size()==4)
			filename=dir+"COUP_0"+str+".Bin";	
		else if(str.size()==3)
			filename=dir+"COUP_00"+str+".Bin";
		else if(str.size()==2)
			filename=dir+"COUP_000"+str+".Bin";
		else if(str.size()==1)
			filename=dir+"COUP_0000"+str+".Bin";
		else
			filename=dir+"COUP_"+str+".Bin";
		auto num=CPG_GetNumber();
		CPG *pCPG;


		pCPG= new CPG();
		validpg=pCPG->Open(filename.c_str());
		if(validpg) {
			m_pCPG=pCPG;
			return true;
		}
		else {
			m_pCPG=NULL;
			delete pCPG;
			return false;
		}

}
void CPlotStorage::SelectedDoc_Add(void* pDoc)
{
   m_SelectedDocArray.push_back(pDoc);
}

void CPlotStorage::SelectedDoc_Reset()
{
   m_SelectedDocArray.clear();
}

void* CPlotStorage::SelectedDoc_GetAt(size_t index)
{
	if(index<m_SelectedDocArray.size())
		return 	m_SelectedDocArray[index];
	else
		return NULL;

}

size_t CPlotStorage::SelectedDoc_GetNum()
{
	return m_SelectedDocArray.size();
}
bool CPlotStorage::CPG_SetTypeFile(size_t index, bool value)
{
	if (index == string::npos) return false;
	while(m_CPG_MultiType.size()<index+1)
		m_CPG_MultiType.push_back(false);
	m_CPG_MultiType[index]=value;	
	return true;
}
size_t CPlotStorage::CPG_GetTypeFile(size_t index)
{
	if (index == string::npos) return string::npos;
	if(m_CPG_MultiType.size()>index)
		return m_CPG_MultiType[index];
	else
		return string::npos;
}
size_t CPlotStorage::GetPGRepitNo(size_t index) 
{
	size_t koll;
	if(index>=0&&index<m_PlotArray.size()) {
		koll=m_PlotArray[index].PGRepitNo;
		return koll;
	}
	else
		return -1;

}
bool CPlotStorage::SetPGRepitNo(size_t index, size_t value)
{
	if(m_PlotArray.size()>index) {
		m_PlotArray[index].PGRepitNo=value;
		return true;
	}
	else
		return false;
}
size_t CPlotStorage::GetPGSectionNo(size_t index) 
{
	size_t koll;
	if(index!=string::npos&&index<m_PlotArray.size()) {
		koll=m_PlotArray[index].PGSection;
		return koll;
	}
	else
		return string::npos;

}
bool CPlotStorage::SetPGSectionNo(size_t index, size_t value)
{
	if(m_PlotArray.size()>index) {
		m_PlotArray[index].PGSection=value;
		return true;
	}
	else
		return false;
}
void CPlotStorage::ToggleAllSectionNo()
{
	bool value = true;
	for (size_t index = 0; index < m_PlotArray.size(); index++) {
		auto ivalue = m_PlotArray[index].PGSection;
		if (ivalue > 20000 && ivalue < 20100)
			ivalue = ivalue - 20000;
		else if (ivalue == 25000)
			ivalue = 5000;
		else if (ivalue == 5000)
			ivalue = 25000;
		else if (ivalue > 0 && ivalue < 100 && value)
			ivalue = ivalue + 20000;
		else if (ivalue < 10000)
			ivalue = ivalue + 6000;
		else if (ivalue > 10000 )
			ivalue = ivalue - 6000;

		m_PlotArray[index].PGSection = ivalue;
	}

}
void CPlotStorage::EnableAllSectionNo( bool value)
{
	for (size_t index = 0; index < m_PlotArray.size(); index++) {
		auto ivalue = m_PlotArray[index].PGSection;
		if (ivalue > 20000 && ivalue < 20100)
			ivalue = ivalue - 20000;
		else if (ivalue == 25000)
			ivalue = 5000;
		else if (ivalue == 5000)
			ivalue = 25000;
		else if (ivalue > 0 && ivalue < 100 && value)
			ivalue = ivalue + 20000;
		else if (ivalue < 10000&& !value)
			ivalue = ivalue + 6000;
		else if (ivalue > 10000 && value)
			ivalue = ivalue - 6000;

		m_PlotArray[index].PGSection = ivalue;
	}
		
}
bool CPlotStorage::EnableSectionNo(size_t index, bool value)
{
	if(m_PlotArray.size()>index) {
		auto ivalue=m_PlotArray[index].PGSection;
		if(ivalue>20000&&ivalue<20100)
			ivalue=ivalue-20000;
		else if(ivalue==25000) 
			ivalue=5000;
		else if(ivalue==5000)
			ivalue=25000;
		else if(ivalue>0&&ivalue<100&&value)
			ivalue=ivalue+20000;
		else if(ivalue<10000&&value)
			ivalue=ivalue+6000;
		else if(ivalue>10000&&value)
			ivalue=ivalue-6000;

		m_PlotArray[index].PGSection=ivalue;
		return true;
	}
	else
		return false;


}
size_t CPlotStorage::RemoveAllSectionCurves()
{ size_t count;
	count=0;
	auto i=m_PlotArray.size()-1;
	while(m_PlotArray.size()>i&&i!=string::npos) {
		if (m_PlotArray[i].PGSection>1&&m_PlotArray[i].PGSection<5000) {
			Remove(i);
			count++;
		}
		else if(m_PlotArray[i].PGSection==12000||m_PlotArray[i].PGSection==6000) 
			m_PlotArray[i].PGSection=0;
		else if (m_PlotArray[i].PGSection>6000) {
			Remove(i);
			count++;
		}
		else if(m_PlotArray[i].PGSection==1||m_PlotArray[i].PGSection==5000)
			m_PlotArray[i].PGSection=0;

		i--;
	}
	return count;

}
size_t CPlotStorage::GetSubChart(size_t index) 
{
	size_t koll;
	if(index<m_PlotArray.size()) {
		koll=m_PlotArray[index].SubChart_No;
		return koll;
	}
	else
		return string::npos;

}
bool CPlotStorage::SetSubChart(size_t index, size_t value)
{
	if(m_PlotArray.size()>index) {
		m_PlotArray[index].SubChart_No=value;
		return true;
	}
	else
		return false;
}
size_t CPlotStorage::GetLogLiVersion(size_t index) 
{
	size_t koll;
	if(m_PlotArray.size()>index) {
		koll=m_PlotArray[index].LogLiVersion;
		return koll;
	}
	else
		return string::npos;

}
bool CPlotStorage::SetLogLiVersion(size_t index, size_t value)
{
	if(m_PlotArray.size()>index) {
		m_PlotArray[index].LogLiVersion= value;
		return true;
	}
	else
		return false;
}
CPG* CPlotStorage::GetPGValFilePointer(size_t index) 
{
	if(m_PlotArray.size()>index) {
		return m_PlotArray[index].pPG;
	}
	else
		return NULL;

}
