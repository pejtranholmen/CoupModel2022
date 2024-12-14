

#include "../ChartProperties\ChartProp.h"
#include "../../NewBase\Doc.h"
#include "./PlotInfo.h"
#include "../../CUG\SimDoc\MyCug.h"


			
#ifndef MISSING
#define MISSING -1.1E38

#endif

PlotInfo::PlotInfo(void)
{
	MySingle::instance().SetPlotInfoPointer(this);
	m_pSimDoc=nullptr;
	m_pSimDepth=nullptr;
	m_pProp=MySingle::instance().GetChartPropPointer();
	
}


PlotInfo::~PlotInfo(void)
{
}
PLOTv PlotInfo::MakeValidationChartInfo(size_t i, size_t i_doc, int ResidualPlot) {
	PLOTv out=GetDefaultPlotInfo();

	out.Residual_RunNo=m_pSimDoc->m_SelectValTime[i].RunNo;
	out.SubChart_No=m_pSimDoc->m_SelectValTime[i].subchart;//m_SelectedChartValIndex.GetAt(ii);
	if(out.SubChart_No==0||(out.Residual_RunNo==110000&&m_pSimDoc->m_SelectedDocuments.size()>1))
		out.SubChart_No=i_doc+1;
	//if(out.SubChart_No>m_pProp->m_NumSubCharts) out.SubChart_No=m_pProp->m_NumSubCharts;
	if(out.SubChart_No>m_pProp->m_NumChartX*m_pProp->m_NumChartY) out.SubChart_No=m_pProp->m_NumChartX*m_pProp->m_NumChartY;

	int row=m_pSimDoc->m_SelectValTime[i].row;

	if(i_doc<m_pSimDoc->m_SelectedDocuments.size()) {
		out.pSimDoc=m_pSimDoc->m_SelectedDocuments[i_doc];
	}
	else
		out.pSimDoc=m_pSimDoc;

	out.ValFileNo=out.pSimDoc->m_ValidationData.GetValFileNumber(row);
	out.Index=row;
	if (out.Index >= m_pSimDoc->m_Val_Array.size()) return out;

	SIMB simb=out.pSimDoc->m_ValidationData.GetSimPointer_TSV(row);
	out.pBase=simb.pBase;
	out.TabIndex=simb.TabIndex;

		string name=out.pSimDoc->m_Val_Array[out.Index].Name;
		string group=out.pSimDoc->m_Val_Array[out.Index].Group;
		string typstr="";
		switch(out.pSimDoc->m_Val_Array[out.Index].OutputType) {
					case 1:	typstr="State Variables"; break;
					case 2:	typstr="Flow Variables"; break;
					case 3:	typstr="Auxiliary Variables"; break;
					case 4:	typstr="Driving Variables"; break;	
		}
		out.pBase=out.pSimDoc->GetPtr(typstr,group, name);
		out.TabIndex=out.pSimDoc->m_Val_Array[out.Index].LocalIndex;


	out.EnsembleType=ENSEMBLE_TYPE::NO;
	out.RepitDataType = false;
	if(out.ValFileNo>=0&&m_pSimDoc->m_SelectValTime[i].col>=3&&m_pSimDoc->m_SelectValTime[i].col<=4) {
		F *pF=out.pSimDoc->ValidationFilePointer(out.ValFileNo);
		

		if(out.pSimDoc->m_FileVersionNumberRead<41)
			out.PGFileIndex=out.Index+out.pSimDoc->GetNumOutPuts();
		else if(out.pSimDoc->m_FileVersionNumberRead<60)
			out.PGFileIndex=2*(out.pSimDoc->m_Val_Array[out.Index].ValidationFileResultIndex)-1;
		else if(out.pSimDoc->IsMultiSimulation()) {
			out.PGFileIndex=out.pSimDoc->m_Val_Array[out.Index].ValidationFileIndex+1;
			out.Residual_VarNum=out.pSimDoc->m_Val_Array[out.Index].ValidationFileResultIndex-1;
		}
		else
			out.PGFileIndex=out.pSimDoc->m_Val_Array[out.Index].ValidationFileResultIndex;
		if(out.pSimDoc->m_FileVersionNumberRead>60&&out.pSimDoc->IsMultiSimulation()) {
			out.pPG_MeasValFile=pF->GetPointer();  // The file with measure data
			if (out.pPG_MeasValFile->GetNumRepititions() > 1) out.RepitDataType = true;
			out.pPG = pF->GetPGResPointer();
			pF->CheckFileNameAndAssignNameToPGClass();
			pF->CallDataDescription();
		}
		else 
			out.pPG=pF->GetPGResPointer(); // The file with simulated data

		if(out.pPG!=nullptr) out.PGFileName=out.pPG->GetFileName();

		if(!out.pSimDoc->IsMultiSimulation()) {
			out.DataType= DATA_TYPES::SIM_VAL;
			out.EnsembleType = ENSEMBLE_TYPE::NO;

		}
		if(!out.pSimDoc->IsMultiSimulation()) {
			out.DataType= DATA_TYPES::SIM_VAL;
			out.EnsembleType = ENSEMBLE_TYPE::NO;
			F *pF = out.pSimDoc->ValidationFilePointer(out.ValFileNo);
			pF->CheckFileNameAndAssignNameToPGClass();
			pF->CallDataDescription();
			if (out.pSimDoc->m_FileVersionNumberRead>60)
				out.pPG_MeasValFile = pF->GetPointer();
			out.Residual_VarNum = out.pSimDoc->m_Val_Array[out.Index].ValidationFileIndex+1;


		}
		else if(out.pSimDoc->m_SelectValTime[i].MeanIndicator==ENSEMBLE_TYPE::ALL) {

			MyCug *pCug=MySingle::instance().GetCugPointer();
			if(!pCug->m_OutAllMulti_Defined[out.ValFileNo]) {
				pCug->BeginWaitCursor();
			//	pCug->CalculateMeanOffTimeSeries(5001, out.ValFileNo);
				pCug->EndWaitCursor();
			}
			out.EnsembleType = ENSEMBLE_TYPE::ALL;
			out.DataType = DATA_TYPES::SIM_VAL;
		}
		else if(out.pSimDoc->m_SelectValTime[i].MeanIndicator== ENSEMBLE_TYPE::ACCEPTED) {
			MyCug *pCug=MySingle::instance().GetCugPointer();
			pCug->BeginWaitCursor();
		//	pCug->CalculateMeanOffTimeSeries(5002, out.ValFileNo);
			pCug->EndWaitCursor();
			out.EnsembleType=ENSEMBLE_TYPE::ACCEPTED;
			out.DataType = DATA_TYPES::SIM_VAL;

		}
		else {
			out.DataType=DATA_TYPES::SIM_VAL;
			out.EnsembleType = ENSEMBLE_TYPE::NO;

			/*int counter = 0;
			float validv = 0;
			int count = -1;
			m_pProp->m_MultiSequence = 0;
			while (validv<0.1&&count<int(m_pSimDoc->MR_Get_TotalNumberofRuns())) {
				counter = m_pProp->m_MultiSequence + 1;
				m_pProp->m_MultiSequence = (counter - 1) % m_pSimDoc->MR_Get_TotalNumberofRuns() + 1;
				validv = m_pSimDoc->m_MStorage.GetMultiP(m_pProp->m_MultiSequence - 1, m_pSimDoc->MR_Get_NumTotalPar());
				count++;
			} */
			out.Residual_RunNo = 0;
			/*if (m_pSimDoc->m_MStorage.GetCyclicNumber() > 1) {
				out.index_cycle = m_pSimDoc->m_MStorage.GetCyclicNumber();
				out.Residual_RunNo = (m_pProp->m_MultiSequence-1)*out.index_cycle - 1;
			} */
		}
	}
	else if(out.ValFileNo>=0&&m_pSimDoc->m_SelectValTime[i].col==1) {
		F *pF=out.pSimDoc->ValidationFilePointer(out.ValFileNo);
		pF->CheckFileNameAndAssignNameToPGClass();
		pF->CallDataDescription();
		if(out.pSimDoc->m_FileVersionNumberRead>60)
			out.pPG=pF->GetPointer();
		else
			out.pPG=pF->GetPGResPointer();
		out.PGFileName=out.pPG->GetFileName();
		if(out.pSimDoc->m_FileVersionNumberRead<41)
			out.PGFileIndex=out.Index+out.pSimDoc->GetNumOutPuts();
		else if(out.pSimDoc->m_FileVersionNumberRead<60)
			out.PGFileIndex=2*(out.pSimDoc->m_Val_Array[out.Index].ValidationFileResultIndex);
		else if(out.pSimDoc->IsMultiSimulation())
			out.PGFileIndex=out.pSimDoc->m_Val_Array[out.Index].ValidationFileIndex+1;
		else
			out.PGFileIndex=out.pSimDoc->m_Val_Array[out.Index].ValidationFileIndex+1;
		if (m_pSimDoc->m_MStorage.GetCyclicNumber() > 1) out.index_cycle = row;
		out.DataType=DATA_TYPES::MEASURED_VAL;
		if (out.pPG->GetNumRepititions() > 1) out.RepitDataType = true;
		out.pPG_MeasValFile = out.pPG;

		

	}
			
	if (out.pSimDoc->IsMultiSimulation()) {
		out.pPG_MeasValFile->ReOpen();
		pair<size_t, size_t> filterinfo = GetFilterForEnsemble(m_pSimDoc->m_MStorage.m_ActualEnsemble, out.Index);
		out.PG_FilterVarIndex = filterinfo.first; out.PG_FilterVarValue = filterinfo.second;
		auto pSw = m_pSimDoc->GetSw("UseFilterParameters");
		auto pSw2 = m_pSimDoc->GetSw("PreDefinedEnsembles");
		out.EnsembleNo = m_pSimDoc->m_MStorage.m_ActualEnsemble;
		if (pSw2->GetIntValue() > 0 && pSw->GetIntValue() > 0) {
			out.RepitDataType = true;
			auto cycle=out.pPG_MeasValFile->GetCycleWithCommonTime();
			auto NumberofDim1 = m_pSimDoc->m_MultiRun_Array[0].NumberOfRepeatitionsWithinDimension;
			
			/*if (out.PG_FilterVarIndex == 0) {
				m_pSimDoc->m_MStorage.SetCyclicNumbers(1);


			}
			else {
				m_pSimDoc->m_MStorage.SetCyclicNumbers(cycle);
				size_t start_pos{ string::npos };
				for (size_t i = 0; i <cycle ; i++) {
					auto koll2=out.pPG_MeasValFile->GetVarValue(out.PG_FilterVarIndex, i + 1);
					if (size_t(out.PG_FilterVarValue) == koll2) {
						start_pos = i; break;
					}
				}
				if (start_pos != string::npos) {
					out.pg_start_pos = start_pos;

				}
				else {
					out.pg_start_pos = 0;
					out.PG_FilterVarIndex = 0;
				}


			}
			out.index_cycle = m_pSimDoc->m_MStorage.GetCyclicNumber();*/
		}

	}

	


	if(ResidualPlot==0) 
		out.LogLiVersion=3;
	else
		out.LogLiVersion=2;

	return out;
}

PLOTv PlotInfo::MakeOutPutChartInfo(size_t i, size_t i_doc) {
	PLOTv out=GetDefaultPlotInfo();
	out.pBase=m_pSimDoc->m_SelectOut[i].pBase;
	out.TabIndex=m_pSimDoc->m_SelectOut[i].LIndex-1;
	out.EnsembleType=m_pSimDoc->m_SelectOut[i].MeanIndicator;


	out.ChartType = m_pProp->m_ChartType;
	
	if(m_pSimDoc->m_SelectedDocuments.size()<=1)
		out.pSimDoc=m_pSimDoc;
	else if(i_doc<m_pSimDoc->m_SelectedDocuments.size()) {
		out.pSimDoc=m_pSimDoc->m_SelectedDocuments[i_doc];
		out.pBase=out.pSimDoc->GetPtr(out.pBase->GetSim(),out.pBase->GetName());
	}
	else
		out.pSimDoc=m_pSimDoc;

	//if(out.MeanIndicator==0) {
		string filename;
		if(out.pSimDoc->IsMultiSimulation()) {
			out.pPG=&out.pSimDoc->m_PG_MultiOutputFile;
			filename=out.pSimDoc->GetOutputFileName(0,false, true);
			//if(m_pProp->m_MultiSequence=)
			out.Residual_RunNo=1;
			out.PGRepitNo = out.Residual_RunNo;
		}
		else {
			out.pPG=&out.pSimDoc->m_PG_OutPutFile;
			filename=out.pSimDoc->GetOutputFileName();
		}

		out.pPG->SetFileName(filename);
		out.PGFileName=out.pPG->GetFileName();
	//}
/*	else if(out.MeanIndicator==1)
		out.pPG=m_pSimDoc->m_PlotStorage.m_PGALL_Out;
	else if(out.MeanIndicator==2) {
			out.pPG=&out.pSimDoc->m_PG_MultiOutputFile;
			filename=out.pSimDoc->GetOutputFileName(0,false, true);


		out.pPG=m_pSimDoc->m_PlotStorage.m_PGALL_Out;
	}*/


	if(out.pBase->Is_Vector()) {
			if(m_pSimDoc->IsMultiSimulation())
				out.PGFileIndex=((OutVector*)out.pBase)->GetPgFileIndex(out.TabIndex);
			else
				out.PGFileIndex=((OutVector*)out.pBase)->GetPgSingleFileIndex(out.TabIndex);
	}
	else {
			if(m_pSimDoc->IsMultiSimulation())
				out.PGFileIndex=((OutSingle*)out.pBase)->GetPgFileIndex();
			else
				out.PGFileIndex=((OutSingle*)out.pBase)->GetPgSingleFileIndex();
	}
	auto simtype = out.pBase->GetType();
	if (simtype >= 1 && simtype < 5)
		out.DataType = DATA_TYPES(out.pBase->GetType());
	else if (simtype > 7 && simtype < 12)
		out.DataType = DATA_TYPES(simtype - 7);
	else
		out.DataType = DATA_TYPES::ANYDATA;

	
	out.RepitDataType = false;
	if(out.pSimDoc->IsMultiSimulation()) out.RepitDataType=true;
	out.Index=m_pSimDoc->m_SelectOut[i].LIndex-1;

	out.SubChart_No=m_pSimDoc->m_SelectOut[i].subchart;//m_SelectedChartValIndex.GetAt(ii);
	if(out.SubChart_No==0||(out.Residual_RunNo==110000&&m_pSimDoc->m_SelectedDocuments.size()>1))
		out.SubChart_No=i_doc+1;
	//if(out.SubChart_No>m_pProp->m_NumSubCharts) out.SubChart_No=m_pProp->m_NumSubCharts;
	if(out.SubChart_No>m_pProp->m_NumChartX*m_pProp->m_NumChartY) out.SubChart_No=m_pProp->m_NumChartX*m_pProp->m_NumChartY;




	return out;
}
PLOTv PlotInfo::MakePGFileChartInfo(size_t i) {
	PLOTv out=GetDefaultPlotInfo();
	if(m_pSimDoc->m_SelectPGFile.size()<=i) return out;
	out.pPG=m_pSimDoc->m_SelectPGFile[i].pPG;
	out.PGFileName=out.pPG->GetFileName();
	out.PGFileIndex=m_pSimDoc->m_SelectPGFile[i].PGIndex;
	out.DataType =DATA_TYPES::PGDATA;
	out.PGRepitNo=m_pSimDoc->m_SelectPGFile[i].Repit;
	out.SubChart_No=m_pSimDoc->m_SelectPGFile[i].subchart;
	if(m_pProp->GetIndepFirst()!=DATEAXIS&&!m_pProp->IsSecondIndepSet())
		out.ChartType= CHART_TYPES::SCATTER;
	else if(m_pProp->IsSecondIndepSet())
		out.ChartType= CHART_TYPES::CONTOUR_CHART;
	else
		out.ChartType= CHART_TYPES::TIME_SERIE;

	if (out.pPG->GetNumRepititions() > 1) out.RepitDataType = true;
	else out.RepitDataType = false;
	
	if(m_pProp->GetIndepFirst()!=DATEAXIS)
		out.IndepAxis.First=PGVAR;
	if(m_pProp->IsSecondIndepSet())
		out.IndepAxis.Second=PGVAR;


	return out;
}
PLOTv PlotInfo::MakeModelFileChartInfo(size_t i) {

	PLOTv out=GetDefaultPlotInfo();
	F* pF=m_pSimDoc->m_SelectModelFile[i].pF;
	out.pPG=pF->GetPointer();
	out.PGFileName=out.pPG->GetFileName();
	out.PGFileIndex=m_pSimDoc->m_SelectModelFile[i].PGIndex;
	out.DataType = DATA_TYPES::ANYDATA;
	out.SubChart_No=m_pSimDoc->m_SelectModelFile[i].subchart;//m_SelectedChartValIndex.GetAt(ii);
	
	if(out.SubChart_No==0||m_pSimDoc->m_SelectedDocuments.size()>1)
		out.SubChart_No=1;
	
	//if(out.SubChart_No>m_pProp->m_NumSubCharts) out.SubChart_No=m_pProp->m_NumSubCharts;
	if(out.SubChart_No>m_pProp->m_NumChartX*m_pProp->m_NumChartY) out.SubChart_No=m_pProp->m_NumChartX*m_pProp->m_NumChartY;
	return out;
}
PLOTv PlotInfo::MakeMultiSumChart(size_t i, size_t i_doc) {
	PLOTv out=GetDefaultPlotInfo();


	out.Index=m_pSimDoc->m_SelectMultiSum[i].Index;

	out.DataType = DATA_TYPES::SIM_OUTSUM;
	out.pBase=m_pSimDoc->m_SelectMultiSum[i].pBase;
	out.TabIndex=m_pSimDoc->m_SelectMultiSum[i].LIndex-1;
	if(m_pSimDoc->m_SelectedDocuments.size()<=1)
		out.pSimDoc=m_pSimDoc;
	else if(i_doc<m_pSimDoc->m_SelectedDocuments.size()) {
		out.pSimDoc=m_pSimDoc->m_SelectedDocuments[i_doc];
		out.pBase=out.pSimDoc->GetPtr(out.pBase->GetSim(),out.pBase->GetName());
	}
	else
		out.pSimDoc=m_pSimDoc;
	if(m_pProp->IsFirstIndepSet()&&m_pProp->GetIndepFirst()!=DATEAXIS&&!m_pProp->IsSecondIndepSet())
		out.ChartType= CHART_TYPES::SCATTER;
	else if(m_pProp->IsSecondIndepSet())
		out.ChartType= CHART_TYPES::CONTOUR_CHART;
	else
		out.ChartType= CHART_TYPES::TIME_SERIE;

	out.SubChart_No=m_pSimDoc->m_SelectMultiSum[i].subchart;//m_SelectedChartValIndex.GetAt(ii);
	
	if(out.SubChart_No==0||(out.Residual_RunNo==110000&&m_pSimDoc->m_SelectedDocuments.size()>1))
		out.SubChart_No=i_doc+1;
	
	//if(out.SubChart_No>m_pProp->m_NumSubCharts) out.SubChart_No=m_pProp->m_NumSubCharts;
	if(out.SubChart_No>m_pProp->m_NumChartX*m_pProp->m_NumChartY) out.SubChart_No=m_pProp->m_NumChartX*m_pProp->m_NumChartY;
	out.EnsembleNo = m_pSimDoc->m_MStorage.m_ActualEnsemble;
	out.Doc = i_doc;
	out.m_ConstrainBy = m_pProp->m_ConstrainProp;
	return out;
}
PLOTv PlotInfo::MakeMultiValChart(size_t i, size_t i_doc) {
	PLOTv out=GetDefaultPlotInfo();

	out.Index=m_pSimDoc->m_SelectMultiVal[i].Index;
	out.index_cycle= m_pSimDoc->m_SelectMultiVal[i].index_cycle;
	out.DataType=m_pSimDoc->m_SelectMultiVal[i].type;
	out.pBase=m_pSimDoc->m_SelectMultiVal[i].pBase;
	out.TabIndex=m_pSimDoc->m_SelectMultiVal[i].LIndex-1;
	out.PGSection=m_pSimDoc->m_SelectMultiVal[i].PGSection;
	if(m_pSimDoc->m_SelectedDocuments.size()<=1)
		out.pSimDoc=m_pSimDoc;
	else if(i_doc<m_pSimDoc->m_SelectedDocuments.size()) {
		out.pSimDoc=m_pSimDoc->m_SelectedDocuments[i_doc];
		if(out.pBase!=nullptr)
			out.pBase=out.pSimDoc->GetPtr(out.pBase->GetSim(),out.pBase->GetName());
	}
	else
		out.pSimDoc=m_pSimDoc;
	if(m_pProp->IsFirstIndepSet()&&!m_pProp->IsSecondIndepSet())
		out.ChartType= CHART_TYPES::SCATTER;
	else if(m_pProp->IsSecondIndepSet())
		out.ChartType= CHART_TYPES::CONTOUR_CHART;
	else
		out.ChartType= CHART_TYPES::TIME_SERIE;
	out.Performance_Var=m_pSimDoc->m_SelectMultiVal[i].Performance_Var;;

	out.SubChart_No=m_pSimDoc->m_SelectMultiVal[i].subchart;//m_SelectedChartValIndex.GetAt(ii);
	if(out.SubChart_No==0||(out.Residual_RunNo==110000&&m_pSimDoc->m_SelectedDocuments.size()>1))
		out.SubChart_No=i_doc+1;
	//if(out.SubChart_No>m_pProp->m_NumSubCharts) out.SubChart_No=m_pProp->m_NumSubCharts;

	if(out.SubChart_No>m_pProp->m_NumChartX*m_pProp->m_NumChartY) out.SubChart_No=m_pProp->m_NumChartX*m_pProp->m_NumChartY;
	out.EnsembleNo = m_pSimDoc->m_MStorage.m_ActualEnsemble;
	out.Residual_VarNum = m_pSimDoc->m_SelectMultiVal[i].row+1;
	if (m_pSimDoc->m_SelectMultiVal[i].row < out.pSimDoc->m_Val_Array.size()) {
		out.Residual_VarNum = out.pSimDoc->m_Val_Array[m_pSimDoc->m_SelectMultiVal[i].row].ValidationFileResultIndex - 1;
	}

	out.Doc = i_doc;


	return out;
}
PLOTv PlotInfo::MakeMultiParChart(size_t i, size_t i_doc) {
	PLOTv out=GetDefaultPlotInfo();
	out.Index=m_pSimDoc->m_SelectMultiPar[i].Index;
	out.DataType = DATA_TYPES::SIM_PAR;
	out.pBase=m_pSimDoc->m_SelectMultiPar[i].pBase;
	out.TabIndex=m_pSimDoc->m_SelectMultiPar[i].LIndex-1;
	if(m_pSimDoc->m_SelectedDocuments.size()<=1)
		out.pSimDoc=m_pSimDoc;
	else if(i_doc<m_pSimDoc->m_SelectedDocuments.size()) {
		out.pSimDoc=m_pSimDoc->m_SelectedDocuments[i_doc];
		out.pBase=out.pSimDoc->GetPtr(out.pBase->GetSim(),out.pBase->GetName());
	}
	else
		out.pSimDoc=m_pSimDoc;
	if(m_pProp->IsFirstIndepSet()&&!m_pProp->IsSecondIndepSet())
		out.ChartType= CHART_TYPES::SCATTER;
	else if(m_pProp->IsSecondIndepSet())
		out.ChartType= CHART_TYPES::CONTOUR_CHART;
	else
		out.ChartType= CHART_TYPES::TIME_SERIE;

	out.SubChart_No=m_pSimDoc->m_SelectMultiPar[i].subchart;//m_SelectedChartValIndex.GetAt(ii);
	if(out.SubChart_No==0||(out.Residual_RunNo==110000&&m_pSimDoc->m_SelectedDocuments.size()>1))
		out.SubChart_No=i_doc+1;
	if(out.SubChart_No>m_pProp->m_NumChartX*m_pProp->m_NumChartY) out.SubChart_No=m_pProp->m_NumChartX*m_pProp->m_NumChartY;
	out.EnsembleNo = m_pSimDoc->m_MStorage.m_ActualEnsemble;

	out.Doc = i_doc;
	return out;
}
PLOTv PlotInfo::MakeMultiPGChart(size_t i) {
	PLOTv out=GetDefaultPlotInfo();

	out.IndepAxis.First=m_pSimDoc->m_SelectMultiPGFile[i].FirstType;
	out.IndepAxis.Second=m_pSimDoc->m_SelectMultiPGFile[i].SecondType;
	out.IndepAxis.Dependent=m_pSimDoc->m_SelectMultiPGFile[i].DependentType;
	if(out.IndepAxis.First!=NO_USE&&out.IndepAxis.Second!=NO_USE) {
		out.ChartType= CHART_TYPES::CONTOUR_CHART;


	}
	else if(out.IndepAxis.First!=NO_USE&&out.IndepAxis.Second==NO_USE) {
		out.ChartType= CHART_TYPES::SCATTER;



	}





	
	out.PGFileIndex=m_pSimDoc->m_SelectMultiPGFile[i].PGIndex;
	out.pPG=m_pSimDoc->m_SelectMultiPGFile[i].pPG;
	out.PGFileName=out.pPG->GetFileName();
	out.DataType= DATA_TYPES::PGDATA;

	out.SubChart_No=m_pSimDoc->m_SelectMultiPGFile[i].subchart;//m_SelectedChartValIndex.GetAt(ii);

	//if(out.SubChart_No>m_pProp->m_NumSubCharts) out.SubChart_No=m_pProp->m_NumSubCharts;
	if(out.SubChart_No>m_pProp->m_NumChartX*m_pProp->m_NumChartY) out.SubChart_No=m_pProp->m_NumChartX*m_pProp->m_NumChartY;
	return out;
}
pair<size_t, size_t> PlotInfo::GetFilterForEnsemble(size_t ensemble_index, size_t var_index)
{
	pair<size_t, size_t> out;
	auto pSw = m_pSimDoc->GetSw("UseFilterParameters");
	auto pSw2 = m_pSimDoc->GetSw("PreDefinedEnsembles");
	if (pSw2->GetIntValue() > 0 && pSw->GetIntValue() > 0) {
		//auto ensemble_no = m_pSimDoc->m_MStorage.m_ActualEnsemble;
		auto Dim1Value = m_pSimDoc->m_MStorage.m_Valid_TSV[ensemble_index].Dim1_Par_Index;
		if (Dim1Value > 0) {
			out.first = Dim1Value;
			for (int i1 = 0; i1 < m_pSimDoc->m_MultiRun_Array.size(); i1++) {
				for (int i2 = 0; i2 < m_pSimDoc->MR_Get_NumberOfParametersWithinDim(i1 + 1); i2++) {
					auto test = m_pSimDoc->MR_GetParName(i1 + 1, i2 + 1);
					if (test == "Filter Var Value") {
						auto indexmulti = i2 + 1;
						auto test_value_sim = size_t(m_pSimDoc->m_MStorage.GetMultiP(Dim1Value - 1, indexmulti) + 0.001);
						out.second = test_value_sim;
						break;
					}
				}
			}
			out.first = m_pSimDoc->m_Val_Array[var_index].PG_FilterIndex;
		}
		else
			out.first = 0;
	}
	else if (pSw->GetIntValue() > 0) {
		out.first = m_pSimDoc->m_Val_Array[var_index].PG_FilterIndex;
		out.second = m_pSimDoc->m_Val_Array[var_index].PG_FilterValue;
	}

	return out;
}
void PlotInfo::SetDoc(Doc *pDoc) {

	m_pSimDoc=pDoc;
	m_pProp=MySingle::instance().GetChartPropPointer();
	m_pSimDepth=pDoc->GetPPointer("DepthMidpLayers");
}
PLOTv PlotInfo::GetDefaultPlotInfo() {
	PLOTv out;
	out.pBase=nullptr;
	out.TabIndex=0;
	out.pPG=nullptr;
	out.pSimDoc=nullptr;
	out.PGFileIndex=0;
	out.Index=0;
	out.PGSection=0;
	out.Chart=1;
	out.ValFileNo=0;
	out.Validation=0;
	out.Doc=0;
	out.Residual_RunNo=0;
	out.Residual_VarNum=1;
	out.SubChart_No=0;
	out.LogLiVersion=0;
	out.PGRepitNo=1;
	out.ChartType= CHART_TYPES::SCATTER;
	out.ChartNumber=0;
	out.IndepAxis.First=NO_USE;
	out.IndepAxis.Second=NO_USE;
	out.IndepAxis.Dependent=NO_USE;
	out.EnsembleNo = 0;
	out.DataType = DATA_TYPES::ANYDATA;
	out.EnsembleType = ENSEMBLE_TYPE::NO;
	out.pPG_MeasValFile = nullptr;
	out.index_cycle = string::npos;
	return out;
}
void PlotInfo::SetFirstIndep(enum vartype vtype)	 {
	m_pProp->SetFirstIndepVarType(vtype);}

void PlotInfo::SetSecondIndep(enum vartype vtype)  {
	m_pProp->SetSecondIndepVarType(vtype);
}
void PlotInfo::DisableFirstIndep(enum vartype vtype)  {
	m_pProp->SetFirstIndepSet(vtype, false);
}
void PlotInfo::DisableSecondIndep(enum vartype vtype)  {
	m_pProp->SetSecondIndepSet(vtype, false);

}
string PlotInfo::GetDepth(SimB *pBase, size_t TabIndex) {
	if(pBase->Is_Vector()) {
		SIMB simb;
		simb.pBase=pBase;
		simb.TabIndex=TabIndex;
		return GetDepth(simb);
	}
	else
		return "";
}
string PlotInfo::GetDepth(SIMB base) {
	if(base.pBase->GetDataType()==SIMPROFILE) {
		if(m_pSimDepth!=nullptr) {
			double depth=m_pSimDepth->GetValue(base.TabIndex);
			string str;
			if(depth<9. ){
				str=FUtil::STD_DtoAscii(depth*100);
				str+=" cm";
			}
			else {
				string str=FUtil::STD_DtoAscii(depth);
				str+=" m";
			}
			return str;
		}
		else
			return "";
	}
	else 
		return "";
}
