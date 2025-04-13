#include "../../stdafx.h"
#include "../../std.h"
#include "../../Util.h"
#include "LoadDataToChart.h"
#include "../ChartProperties\ChartProp.h"
#include "../../NewBase\Doc.h"
#include "../PlotDesignInfo\PlotStorage.h"
#include "../../PG\PGCalc.h"

#include <time.h>


LoadDataToChart::LoadDataToChart(void)
{
	m_pChartCug = MySingle::instance().GetChartCugPointer();
	m_pProp = MySingle::instance().GetChartPropPointer();
	m_pUtil = MySingle::instance().GetChartUtilPointer();
	m_pPG_Active = nullptr;
}

LoadDataToChart::~LoadDataToChart(void)
{
}
bool LoadDataToChart::LoadTimeSerieData(bool ForcedNewChartType)
{

	if (m_pSimDoc == nullptr) return false;
	time_t t = time(0);
	struct tm ymd;
	localtime_s(&ymd, &t);
	m_Time.MaxDate = Chart::chartTime(ymd.tm_year + 1900, ymd.tm_mon, ymd.tm_mday);
	m_Time.MinDate = Chart::chartTime(ymd.tm_year + 1900 - 5, ymd.tm_mon, ymd.tm_mday);

	CPG *pPG = nullptr;
	int PGindex = 0;

	CString DocFileName;
	m_ChartInfo.NumVariableBands = 0;
	Doc *pDocAct = nullptr;


	m_pUtil->m_FileSave.m_ZoomX = false;
	vector<int> PG_indexArray;
	vector<CPG*> PG_pointerArray;
	CPG *pPG1 = nullptr;
	m_ChartInfo.ResidualSource = false;
	bool MeanOfAll = true;
	CResiduals *pRes = nullptr;
	long ResRunNo = 0;
	int MeasuredValidationNo = 0;
	//m_pProp->IsFirstIndepSet()= m_pProp->IsSecondIndepSet()=false;
	CWinApp* pApp = AfxGetApp();
	m_pUtil->m_ResidualPlot = MFC_Util::GetProfileIntNo("ResidualPlot", m_pUtil->m_ResidualPlot);



	bool first;
	first = true;
	m_ChartInfo.MultiValMode = false;
	m_ChartInfo.PGRepitMode = false;
	m_pUtil->m_FileSave.m_NoChange = false;
	PLOTv p_info;
	//for (int j=0;j<m_pSimDoc->m_PlotStorage.NumSelected();j++) {

	// Allocate first index of double array
	size_t ncurves = m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);
	int j = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, 0);
 	p_info = m_pSimDoc->m_PlotStorage.GetPlotInfo(j);
	// make sure the ChartType corresponds to the correct data Type
	auto previous_ChartType = m_pProp->m_ChartType;
	if (!ForcedNewChartType) {
		if (p_info.ChartType == CHART_TYPES::TIME_SERIE) {
			m_pProp->m_ChartType = CHART_TYPES::TIME_SERIE;
			m_pProp->m_PreviousChartType = previous_ChartType;
		}
		else if (p_info.ChartType == CHART_TYPES::CUM_FREQUENCE) {
			m_pProp->m_ChartType = CHART_TYPES::CUM_FREQUENCE;
			m_pProp->m_PreviousChartType = previous_ChartType;
		}
	}



	size_t n_factor = 2;
	n_factor = m_pSimDoc->m_MStorage.m_AcceptedRuns.size() + 1;

	if (m_pUtil->m_MultiRunPlot && !m_pProp->IsSecondIndepSet()/* && !m_pProp->IsFirstIndepSet()*/) AllocateNumberOfCurves(ncurves*n_factor);
	else AllocateNumberOfCurves(ncurves);

	if (m_pSimDoc->m_PlotStorage.IsValidIndepPGVar() && (p_info.DataType == DATA_TYPES::SIM_VAL||
		
		p_info.DataType == DATA_TYPES::MEASURED_VAL)) {
		m_pProp->SetNumberOfValidationPG_Dep(ncurves);
		AllocateNumberOf_IndepCurves(ncurves);
	}
	else if (m_pProp->GetIndepSecond() == PGDEPTH) {
		AllocateNumberOf_IndepCurves(1);
	}
	else if (m_pUtil->m_AutoShiftFirstIndep) {
		AllocateNumberOf_IndepCurves(ncurves);

	}
	else {
		AllocateNumberOf_IndepCurves(0);

	}


	size_t nbands = m_pSimDoc->m_PlotStorage.GetNumVarBands(m_pProp->m_ChartNumber);
	AllocateNumberOfBands(nbands + 1);
	size_t i_band = 0;
	for (size_t icurve = 0; icurve<ncurves; icurve++) {
		MFC_Util m;
		m.SetPaneMessage("Loading data to Chart " + FUtil::STD_ItoAscii(icurve + 1) + "(" + FUtil::STD_ItoAscii(ncurves)+")");
		size_t j = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, icurve);
		p_info = m_pSimDoc->m_PlotStorage.GetPlotInfo(j);
		m_ChartInfo.ResidualSource = false;
		m_ChartInfo.DataType = size_t(p_info.DataType);
		m_ChartInfo.RepitDataType = p_info.RepitDataType;
		

		CURVEINFO c_info = OpenCurvePG_File(p_info, j);
		c_info.MeasuredAsSource = false;
		if (p_info.DataType > DATA_TYPES::SIM_VAL && p_info.DataType < DATA_TYPES::PGDATA /*|| p_info.DataType == 99 || p_info.DataType == 98*/) {

			m_pPG_Active = nullptr;
		}
		else if (p_info.DataType == DATA_TYPES::SIM_VAL) {
			if (c_info.pPG1 == nullptr) {
				m_pUtil->m_Displayed = false;
				return false;
			}
			else
				m_pPG_Active = p_info.pPG;

			if (p_info.pPG_MeasValFile != nullptr) {
				if (p_info.pPG_MeasValFile->GetNumRepititions() > 1) m_ChartInfo.PGRepitMode = true;
				else if(p_info.RepitDataType) m_ChartInfo.PGRepitMode = true;
			}
			c_info.PGindex = m_pSimDoc->m_PlotStorage.GetVar_Index(j);
			if (!CheckandOpenPGFile(p_info)) return false;

			PG_indexArray.push_back(c_info.PGindex);
			if (p_info.pPG_MeasValFile != nullptr&&m_pSimDoc->IsMultiSimulation()) {
				m_pPG_Active = p_info.pPG_MeasValFile;
				c_info.MeasuredAsSource = true;

			}
		}
		else if (p_info.DataType == DATA_TYPES::PGDATA) {
			PG_indexArray.push_back(p_info.PGFileIndex);
			m_pPG_Active = p_info.pPG;
			c_info.pPG1= p_info.pPG;
			c_info.MeasuredAsSource = false;

		}
		else if (p_info.DataType == DATA_TYPES::SIM_T|| p_info.DataType == DATA_TYPES::SIM_G||
			p_info.DataType == DATA_TYPES::SIM_X|| p_info.DataType == DATA_TYPES::SIM_D) {
			PG_indexArray.push_back(p_info.PGFileIndex);
			m_pPG_Active = p_info.pPG;
			c_info.pPG1 = p_info.pPG;
			c_info.MeasuredAsSource = false;

		}
		else if (p_info.DataType == DATA_TYPES::MEASURED_VAL) {
			PG_indexArray.push_back(p_info.PGFileIndex);
			m_pPG_Active = p_info.pPG;
			c_info.pPG1 = p_info.pPG;
			c_info.MeasuredAsSource = true;
		}

		if (icurve == 0) {
			SetChartInfo(c_info, p_info);
			if (m_pProp->GetIndepSecond() == PGDEPTH)
				AllocateNumberOfPointsinCurves(m_ChartInfo.NumOfPoints);
			else
				AllocateNumberOfPointsinCurves(m_ChartInfo.NumOfPoints);
			if (nbands>0)
				AllocateNumberOfPointsinBands(m_ChartInfo.NumOfPoints);

		};

		// Check Determine PGindex or Variable index
		if (p_info.DataType <= DATA_TYPES(5) || p_info.DataType == DATA_TYPES(10) /*|| p_info.RepitDataType*/) {
			int koll = PG_indexArray.size();
			if (koll == 0) {
				PGindex = p_info.PGFileIndex;

			}
			else {
				PGindex = PG_indexArray[icurve];
				if (PGindex <= 0) PGindex = p_info.PGFileIndex;
			}
		}


		if (m_pPG_Active != nullptr || m_pUtil->m_MultiRunPlot) {


			//AllocateSpaceToData(icurve);
			if(!LoadSelectedVariables(j, icurve, i_band, c_info, p_info))  return false;
			TransformSelectedVariables(p_info, icurve, i_band);


			if (m_pSimDoc->m_PlotStorage.IsValidIndepPGVar() && p_info.DataType == DATA_TYPES::SIM_VAL) {
				m_pProp->m_timeStamps = DoubleArray(&TT_Indep[icurve][0], m_ChartInfo.NumOfPoints);
				m_pProp->m_IndepX[icurve] = DoubleArray(&TT_Indep[icurve][0], m_ChartInfo.NumOfPoints);

			}
			else if (m_pUtil->m_AutoShiftFirstIndep&& p_info.DataType == DATA_TYPES::SIM_SUMVAR) {
				m_pProp->m_timeStamps = DoubleArray(&TT_Indep[icurve][0], m_ChartInfo.NumOfPoints);
				m_pProp->m_IndepX[icurve] = DoubleArray(&TT_Indep[icurve][0], m_ChartInfo.NumOfPoints);

			}
			else if (icurve == 0 || (!m_pSimDoc->m_PlotStorage.IsValidIndepPGVar() && p_info.DataType == DATA_TYPES::SIM_VAL)) {

				SetTimeSerie();
			}


			if (p_info.EnsembleType > ENSEMBLE_TYPE::NO) SetBandSerie(i_band);

			SetDataSerie(icurve);
			m_pProp->m_PointType[icurve] = false;

			if (m_pUtil->m_MultiRunPlot && !m_pProp->IsSecondIndepSet()/* && !m_pProp->IsFirstIndepSet()*/) {
				int numvar = m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);
				//AllocateSpaceToData(numvar+icurve);
				AddExtraValidSerie(icurve);

			}

		}
		if (p_info.EnsembleType > ENSEMBLE_TYPE::NO) i_band++;

	}
	if (m_pProp->m_ReShape.m_StackingVariables > 0) {
			vector<double> stack;
			stack.assign(m_ChartInfo.NumOfPoints, 0.);
			if (m_pProp->m_ReShape.m_StackingVariables == 1) {
				for (size_t icurve = 0; icurve < ncurves; icurve++) {
					for (long i = 0; i < m_ChartInfo.NumOfPoints; i++) {
						stack[i] += AA[icurve][i];
						AA[icurve][i] = stack[i];
					}
				}
			}
			else {
				for (int icurve = ncurves-1; icurve >=0; icurve--) {
					for (long i = 0; i < m_ChartInfo.NumOfPoints; i++) {
						stack[i] += AA[icurve][i];
						AA[icurve][i] = stack[i];
					}
				}
			}
	}



	PG_indexArray.clear();
	PG_pointerArray.clear();


	// The initial view port is to show the entire range of data.
	if (m_pProp->m_ReShape.m_SortedSeries || m_pProp->m_ReShape.m_NormalDist) {
		m_Time.MinDuration = 1;
		m_Time.MinDate = m_pProp->m_timeStamps[0];
		// Duration is the seconds elapsed of the last timestamp from the earliest date
		m_Time.DateRange = m_pProp->m_timeStamps[m_pProp->m_timeStamps.len - 1] - m_Time.MinDate;

	}
	else {
		m_Time.CurrentDuration = m_Time.MaxDate - m_Time.MinDate;
		m_Time.MinDate = m_Time.MinDate;
		m_Time.DateRange = m_Time.CurrentDuration;
	}

	m_pProp->m_CommonScale.LeftMin = m_pProp->m_CommonScale.LeftMax = 0.;
	m_pProp->m_CommonScale.RightMin = m_pProp->m_CommonScale.RightMax = 0.;
	if (!m_pProp->isXScaleChanged(0))
		m_pProp->m_TimeScale.min = m_pProp->m_TimeScale.max = 0.;
	m_ChartInfo.NumVariableBands = i_band;

	//
	// Get the data and stores them in a memory buffer for fast scrolling / zooming. In 
	// this demo, we just use a random number generator. In practice, you may get the data
	// from a database or XML or by other means.
	//	
	if (m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber) <= 0)
		return false;
	if ((p_info.DataType<DATA_TYPES::SIM_PERFORMANCE || p_info.DataType>DATA_TYPES::SIM_SUMVAR) && pPG1 != nullptr)
		pPG1->CloseFile();
	if (p_info.DataType == DATA_TYPES::SIM_PERFORMANCE || p_info.DataType == DATA_TYPES::SIM_PAR || p_info.DataType == DATA_TYPES::SIM_SUMVAR)
		m_pProp->ResetDistScale();
	m_pUtil->m_Displayed = false;
	m_EstimateNewDist = true;

	return true;
}
CURVEINFO LoadDataToChart::OpenCurvePG_File(PLOTv p_info, int j) {
	CURVEINFO out;
	out.pPG1 = p_info.pPG;
	out.pRes = nullptr;
	out.Band = false;
	Doc *pD = m_pSimDoc->m_PlotStorage.GetDocPointer(p_info.Doc);
	Doc *pDocAct;
	if (pD != nullptr)
		pDocAct = pD;
	else
		pDocAct = m_pSimDoc;

	if (m_pSimDoc->m_SelectedDocuments.size()>p_info.Doc)
		pDocAct = m_pSimDoc->m_SelectedDocuments[p_info.Doc];
	else
		pDocAct = m_pSimDoc;
	if (m_pProp->GetIndepFirst() == PGVAR&&m_pProp->GetIndepSecond() == PGVAR) {
		out.pPG1 = p_info.pPG;
		if (out.pPG1 != nullptr)
			out.pPG1->ReOpen();

	}
	else if (!m_pUtil->m_MultiRunPlot) {
		void *Ptr = p_info.pBase;
		if (p_info.DataType == DATA_TYPES::PGDATA) {
			// Normal PG file
			out.pPG1 = p_info.pPG;
			out.pPG1->ReOpen();

		}
		else if (p_info.DataType == DATA_TYPES::ANYDATA) {
			// unknown fileType
			out.pPG1 = p_info.pPG;//((F*)Ptr)->GetPointer();
								  /*				if(out.pPG1==nullptr) {
								  string fileName=((F*)Ptr)->GetStrValue();
								  out.pPG1=new CPG;
								  out.pPG1->Open(fileName);
								  if(out.pPG1->IsOpen()){
								  ((F*)Ptr)->SetPointer(out.pPG1);
								  }
								  else	{
								  out.pPG1->CloseFile();
								  delete out.pPG1;
								  out.pPG1=nullptr;
								  }
								  }
								  else
								  out.pPG1->ReOpen();*/
		}
		else if (p_info.DataType>DATA_TYPES::PGDATA && p_info.EnsembleType>ENSEMBLE_TYPE::NO) {
			// Mean of All Variables
			string name;

			out.pPG1 = m_pSimDoc->m_PlotStorage.GetPGPointer(p_info.Doc, p_info.ValFileNo);
			if (out.pPG1 == nullptr)
				/*	if(p_info.DataType>20) {
				p_info.DataType=p_info.DataType-20;
				out.MeanOfAll=false;
				out.pPG1=m_pSimDoc->m_PlotStorage.GetPGPointer(p_info.Doc,p_info.ValFileNo);
				if(out.pPG1==nullptr)
				out.pPG1=pDocAct->m_PlotStorage.m_PG_SEL_MULTI[p_info.ValFileNo];
				}
				else {
				p_info.DataType=p_info.DataType-10;
				out.MeanOfAll=true;
				out.pPG1=m_pSimDoc->m_PlotStorage.GetPGPointer(p_info.Doc,p_info.ValFileNo);
				if(out.pPG1==nullptr)
				out.pPG1=m_pSimDoc->m_PlotStorage.m_PG_ALLMULTI[p_info.ValFileNo];
				}*/

				out.Band = true;
			if (p_info.EnsembleType==ENSEMBLE_TYPE::ACCEPTED)
				out.MeanOfAll = false;
			else
				out.MeanOfAll = true;

			if (pDocAct->MR_XBin_IsOpen(p_info.ValFileNo)) {
				m_ChartInfo.ResidualSource = true;
				out.pRes = pDocAct->GetResidualPointer(p_info.ValFileNo);
			}
			else
				out.pRes = nullptr;
		}
		else if (p_info.DataType== DATA_TYPES::SIM_VAL && !p_info.RepitDataType) {
			out.Band = false;
			int SimRunNo;
			SimRunNo = pDocAct->m_DocFile.m_SimulationRunNo;
			/*				if(pDocAct->m_FileVersionNumberRead<60)
			m_NewResidualFormat=false;
			else
			m_NewResidualFormat=true;*/

			out.pPG1 = p_info.pPG;


			if (out.pPG1 != nullptr) {
				out.pPG1->ReOpen();
				if (p_info.EnsembleType > ENSEMBLE_TYPE::NO) {
					out.Band = true;
					out.MeanOfAll = false;
					if(p_info.EnsembleType==ENSEMBLE_TYPE::ALL)
					out.MeanOfAll = true;


				}

				if (pDocAct->MR_XBin_IsOpen(p_info.ValFileNo)) {
					m_ChartInfo.ResidualSource = true;
					out.pRes = pDocAct->GetValidResidualPointer(p_info.ValFileNo);
					out.ResRunNo = p_info.Residual_RunNo;
					m_ChartInfo.MultiValMode = true;
					if (out.ResRunNo<100000) {
						int ifile = m_pSimDoc->MR_GetAddedFiles(out.ResRunNo);
						if (ifile != 0) {
							out.pRes = m_pSimDoc->GetValidResidualPointer(p_info.ValFileNo, ifile);
							out.ResRunNo = m_pSimDoc->MR_GetInternalRunNo(out.ResRunNo);
						}
					}
					if (out.ResRunNo == 140000) {
						out.Band = true;
						out.MeanOfAll = false;
					}
					else if (out.ResRunNo == 110000) {
						out.ResRunNo = 0;
					}
					else if (out.ResRunNo>120000) {
						//scaling is true
						//MeasuredValidationNo=out.ResRunNo-120000;
						out.ResRunNo = 120000;
					}

				}
			}
		}
		else if (p_info.DataType == DATA_TYPES::SIM_VAL && p_info.RepitDataType) {
		out.Band = false;
		int SimRunNo;
		SimRunNo = pDocAct->m_DocFile.m_SimulationRunNo;
		/*				if(pDocAct->m_FileVersionNumberRead<60)
		m_NewResidualFormat=false;
		else
		m_NewResidualFormat=true;*/
		if (p_info.pPG_MeasValFile != nullptr && p_info.DataType != DATA_TYPES::PGDATA) {
			out.pPG1 = p_info.pPG_MeasValFile;
		}


		if (out.pPG1 != nullptr) {
			out.pPG1->ReOpen();
			if (p_info.EnsembleType > ENSEMBLE_TYPE::NO) {
				out.Band = true;
				out.MeanOfAll = false;
				if (p_info.EnsembleType == ENSEMBLE_TYPE::ALL)
					out.MeanOfAll = true;


			}

			if (pDocAct->MR_XBin_IsOpen(p_info.ValFileNo)) {
				m_ChartInfo.ResidualSource = true;
				out.pRes = pDocAct->GetValidResidualPointer(p_info.ValFileNo);
				out.ResRunNo = p_info.Residual_RunNo;
				m_ChartInfo.MultiValMode = true;
				if (out.ResRunNo < 100000) {
					int ifile = m_pSimDoc->MR_GetAddedFiles(out.ResRunNo);
					if (ifile != 0) {
						out.pRes = m_pSimDoc->GetValidResidualPointer(p_info.ValFileNo, ifile);
						out.ResRunNo = m_pSimDoc->MR_GetInternalRunNo(out.ResRunNo);
					}
				}
				if (out.ResRunNo == 140000) {
					out.Band = true;
					out.MeanOfAll = false;
				}
				else if (out.ResRunNo == 110000) {
					out.ResRunNo = 0;
				}
				else if (out.ResRunNo > 120000) {
					//scaling is true
					//MeasuredValidationNo=out.ResRunNo-120000;
					out.ResRunNo = 120000;
				}

			}
		}
		}

		else {
			if (out.pPG1 == nullptr&&p_info.RepitDataType) {
				int simrunNo = pDocAct->m_DocFile.m_SimulationRunNo;
				long mruno = p_info.Residual_RunNo;
				simrunNo += mruno;
				if (p_info.DataType>DATA_TYPES::SIM_VAL)
					out.pPG1 = m_pSimDoc->m_PlotStorage.GetPGPointer(p_info.Doc, 0, simrunNo, true);

			}
			else {
				int simrunNo = pDocAct->m_DocFile.m_SimulationRunNo;
				out.pPG1 = p_info.pPG;
				string filename = out.pPG1->GetFileName();
				out.pPG1->CloseFile();
				if (!out.pPG1->Open(filename)) {
					out.pPG1 = nullptr;
				}
				m_ChartInfo.MultiValMode = true;
				//out.pPG1=m_pSimDoc->m_PlotStorage.GetPGPointer(p_info.Doc, 0, simrunNo, false);
			}
			if (out.pPG1 != nullptr) {
				if (!out.pPG1->ReOpen()) {

					out.pPG1->ClearFile();
					out.pPG1->ReOpen();

				}
				if (p_info.RepitDataType) { // Set RunNo
					m_ChartInfo.MultiPGMode = true;
					if (p_info.Residual_RunNo <= 0) {

						m_pSimDoc->m_PlotStorage.SetValRunNo(j, m_pProp->m_MultiSequence);
						p_info.Residual_RunNo = m_pProp->m_MultiSequence;
					}
					if (p_info.PGRepitNo == 0) p_info.PGRepitNo = p_info.Residual_RunNo;

					out.Band = false;
					if (p_info.RepitDataType) {
						int simrunNo = pDocAct->m_DocFile.m_SimulationRunNo;
						long mruno = p_info.Residual_RunNo;
						simrunNo += mruno;
						int index = m_pSimDoc->m_MStorage.GetAdd_MultiPGFileIndex(simrunNo);
						if (out.pPG1->pg.Shape.NumRepetitions > 1&& m_pSimDoc->m_MStorage.m_AddedFiles.size() == 0) {
							if (index > 0) {
								out.pPG1->SetRepititionMode(true, m_pSimDoc->m_MStorage.GetNumberOfAddedRuns(index - 1));
								long PrevRuns = m_pSimDoc->MR_Get_NumberofRuns();
								for (int jb = 1; jb < index; jb++)
									PrevRuns += m_pSimDoc->m_MStorage.GetNumberOfAddedRuns(jb - 1);
								out.pPG1->SetRepititionSequence(p_info.Residual_RunNo - PrevRuns);
							}
							else if (p_info.PGRepitNo > 0) {

								out.pPG1->SetRepititionSequence(p_info.PGRepitNo,false);

							}
							else {
								out.pPG1->SetRepititionMode(true, m_pSimDoc->MR_Get_NumberofRuns());
								out.pPG1->SetRepititionSequence(p_info.Residual_RunNo);
							}
						}

						if (p_info.EnsembleType == ENSEMBLE_TYPE::ALL) {

							out.pPG1->EstimateMultiStatistics(true);
							out.Band = true;
						}
						else if (p_info.EnsembleType == ENSEMBLE_TYPE::ACCEPTED) {
							p_info.EnsembleNo;
							bool multivalid = m_pSimDoc->MR_Storage_Open();
							if (m_pSimDoc->m_MStorage.m_AcceptedRuns.size()>0) {
									out.pPG1->EstimateMultiStatistics(false, &m_pSimDoc->m_MStorage.m_AcceptedRuns[p_info.EnsembleNo], p_info.EnsembleNo, &m_pSimDoc->m_MStorage.m_AddedFiles, &m_pSimDoc->m_MStorage.m_AddedFirstRunNo);
							}
							else {
								out.pPG1 = nullptr;

							}
							out.Band = true;
						}
					}
				}
			}
		}
	}

	else { //Multi Run plot

		out.Band = false;
	}

	return out;


}
bool LoadDataToChart::CheckandOpenPGFile(PLOTv plotinfo) {
	if (plotinfo.pPG == nullptr) return false;
	return plotinfo.pPG->Open(plotinfo.PGFileName);

}
void LoadDataToChart::CloseActivePGFile() {
	if ((m_ChartInfo.DataType<6 || m_ChartInfo.DataType>9) && m_pPG_Active != nullptr)
		m_pPG_Active->CloseFile();
}

void LoadDataToChart::SetChartInfo(CURVEINFO c_info, PLOTv plotv) {

	m_ChartInfo.WithinDayRepresentation = 1;
	m_ChartInfo.YearRepresentation = 0;
	m_ChartInfo.NumExtraPointFreq = string::npos;

	if (m_pUtil->m_MultiRunPlot) {
		m_ChartInfo.NumOfPoints = m_pSimDoc->MR_Get_TotalNumberofRuns();
		m_pProp->SetPGSource(nullptr);
	}
	else if (m_pUtil->m_MultiPGPlot) {
		c_info.pPG1->ReOpen();
		m_ChartInfo.NumOfPoints = c_info.pPG1->GetNumRepititions();
		m_pProp->SetPGSource(nullptr);
	}
	else {

		auto k2 = c_info.pPG1->GetNumRepititions();
		auto k3 = c_info.pPG1->GetNumRecords();
		c_info.pPG1->ReOpen();
		

		if ((plotv.DataType==DATA_TYPES::SIM_VAL|| plotv.DataType == DATA_TYPES::MEASURED_VAL) &&plotv.RepitDataType) {
			if (m_pSimDoc->IsMultiSimulation()) {
				auto k = c_info.pPG1->GetNumRepititions();
				m_ChartInfo.NumOfPoints = c_info.pPG1->GetNumRecords(); //disregard Number of repititons 
			}
			else {
				m_pPG_Active = c_info.pPG1;
				m_ChartInfo.NumOfPoints = c_info.pPG1->GetNumRecords();

			}
			
 		}
		else if((c_info.Band&& plotv.DataType == DATA_TYPES::SIM_VAL)||c_info.MeasuredAsSource)
			m_ChartInfo.NumOfPoints = m_pPG_Active->GetNumRecords();
		else {
			m_ChartInfo.NumOfPoints = c_info.pPG1->GetNumRecords();
			m_pPG_Active = c_info.pPG1;
		}
		m_ChartInfo.pPGFirst = c_info.pPG1;
		m_ChartInfo.NumOfYears = c_info.pPG1->GetNumberofYears();
		m_pProp->SetPGSource(c_info.pPG1);
		if (c_info.pPG1->GetNormalTimeInterval() > 0) {
			if ((m_pProp->GetIndepSecond() == PGHOUR&&c_info.pPG1->GetNumberofYears() > 1 && m_pProp->GetIndepFirst() == PGDAY)
				|| (m_pProp->GetIndepSecond() == PGDAY&&c_info.pPG1->GetNumberofYears() > 1 && m_pProp->GetIndepFirst() == PGHOUR)) {
				size_t nvalues = 1440 / c_info.pPG1->GetNormalTimeInterval();
				size_t nextra = c_info.pPG1->GetRecordsInYear(1) / nvalues;
				m_ChartInfo.NumExtraPointFreq = nvalues + 1;
				m_ChartInfo.NumOfPoints = c_info.pPG1->GetRecordsInYear(1) + nextra;
				m_ChartInfo.NumOfYears = c_info.pPG1->GetNumberofYears();
				m_ChartInfo.YearRepresentation = m_pProp->m_YearValueRepresentation;
			}
			else if ((m_pProp->GetIndepSecond() == PGYEAR&&c_info.pPG1->GetNumberofYears() > 1 && m_pProp->GetIndepFirst() == PGDAY)
				|| (m_pProp->GetIndepSecond() == PGDAY&&c_info.pPG1->GetNumberofYears() > 1 && m_pProp->GetIndepFirst() == PGYEAR)) {
				size_t nvalues = 1440 / c_info.pPG1->GetNormalTimeInterval();
				if (nvalues >= 1) {
					size_t nextra = c_info.pPG1->GetNumRecords() / nvalues;
					m_ChartInfo.NumExtraPointFreq = nvalues + 1;
					m_ChartInfo.NumOfPoints = nextra;

					m_ChartInfo.WithinDayRepresentation = nvalues;
					m_ChartInfo.HourRepresentation = m_pProp->m_HourValueRepresentation;
				}
			}
		}

	}
	m_ChartInfo.NumVariables = m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);
	TT.clear();
	TT.resize(m_ChartInfo.NumOfPoints);
	TT2.clear();
	TT2.resize(m_ChartInfo.NumOfPoints);


}
void LoadDataToChart::SetDataSerie(size_t icurve) {
	// This should corresponds to the complete data serie
	// Note to be mixed up with the actual viewport
	if (m_pProp->m_ReShape.m_CommonDayNo)
		m_ChartInfo.NumOfPoints = m_pProp->m_ReShape.NumOfCommonPoints;


	for (long i = 0; i<m_ChartInfo.NumOfPoints; i++) {
		if (abs(AA[icurve][i])>1.E38)
			AA[icurve][i] = Chart::NoValue;
	}
	if (m_pProp->m_ChartType ==CHART_TYPES::CONTOUR_CHART && icurve + 1 == m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber)) {
		//vector<double> NewZ;
		//NewZ.resize(m_ChartInfo.NumOfPoints*m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber));
		AA[0].resize(m_ChartInfo.NumOfPoints*m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber));

		for (int jj = 0; jj<icurve + 1; jj++) {
			for (int pp = 0; pp < m_ChartInfo.NumOfPoints; pp++) {
				//				NewZ[pp+ jj*m_ChartInfo.NumOfPoints] = AA[jj][m_Agg.StartIndex + pp]; 
				AA[0][pp + jj*m_ChartInfo.NumOfPoints] = AA[jj][0 + pp];
			}
			if (jj>0) m_dataSeriesA[jj] = DoubleArray(&AA[jj][0], m_ChartInfo.NumOfPoints);
		}

		m_dataSeriesA[0] = DoubleArray(&AA[0][0], m_ChartInfo.NumOfPoints*(icurve + 1));
		m_dataSeriesA[icurve] = DoubleArray(&AA[icurve][0], m_ChartInfo.NumOfPoints);
		m_pProp->m_PointType[0] = false;

	}
	else if (m_pProp->IsSecondIndepSet()) {
		m_dataSeriesA[icurve] = DoubleArray(&AA[0][0], m_ChartInfo.NumOfPoints);

	}
	else {
		m_dataSeriesA[icurve] = DoubleArray(&AA[icurve][0], m_ChartInfo.NumOfPoints);
	}
}
void LoadDataToChart::SetTimeSerie() {
	// This corresponds to the entire dataset
	m_pProp->m_timeStamps = DoubleArray(&TT[0], m_ChartInfo.NumOfPoints);
	m_pProp->m_timeStamps2 = DoubleArray(TT2.data(), m_ChartInfo.NumOfPoints);
}
void LoadDataToChart::SetBandSerie(size_t i_band) {
	for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {
		if (abs(BB[i_band][i])>1.E38)
			BB[i_band][i] = Chart::NoValue;
		if (abs(BB_MIN[i_band][i])>1.E38)
			BB_MIN[i_band][i] = Chart::NoValue;
		if (abs(BB_P10[i_band][i])>1.E38)
			BB_P10[i_band][i] = Chart::NoValue;
		if (abs(BB_P50[i_band][i])>1.E38)
			BB_P50[i_band][i] = Chart::NoValue;
		if (abs(BB_P90[i_band][i])>1.E38)
			BB_P90[i_band][i] = Chart::NoValue;
		if (abs(BB_MAX[i_band][i])>1.E38)
			BB_MAX[i_band][i] = Chart::NoValue;
	}

	m_dataSeriesB[i_band] = DoubleArray(&BB[i_band][0], m_ChartInfo.NumOfPoints);
	m_dataSeriesP10[i_band] = DoubleArray(&BB_P10[i_band][0], m_ChartInfo.NumOfPoints);
	m_dataSeriesP50[i_band] = DoubleArray(&BB_P50[i_band][0], m_ChartInfo.NumOfPoints);
	m_dataSeriesP90[i_band] = DoubleArray(&BB_P90[i_band][0], m_ChartInfo.NumOfPoints);
	m_dataSeriesMIN[i_band] = DoubleArray(&BB_MIN[i_band][0], m_ChartInfo.NumOfPoints);
	m_dataSeriesMAX[i_band] = DoubleArray(&BB_MAX[i_band][0], m_ChartInfo.NumOfPoints);


}
void LoadDataToChart::AddExtraValidSerie(size_t icurve) {
	PLOTv plot;
	int j = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, icurve);
	plot = m_pSimDoc->m_PlotStorage.GetPlotInfo(j);
	Doc* pDocAct;
	int idoc = m_pSimDoc->m_PlotStorage.GetDocIndex(j);

	Doc *pD = m_pSimDoc->m_PlotStorage.GetDocPointer(idoc);
	if (pD != nullptr)
		pDocAct = pD;
	else
		pDocAct = m_pSimDoc;

	if (m_pSimDoc->m_SelectedDocuments.size()>idoc)
		pDocAct = m_pSimDoc->m_SelectedDocuments[idoc];
	else
		pDocAct = m_pSimDoc;


	float validv;

	int numvar = m_pSimDoc->m_PlotStorage.GetNumVar(m_pProp->m_ChartNumber);

	auto save_ens = m_pSimDoc->m_MStorage.m_ActualEnsemble;

	bool allensembles = true;
	size_t numensembles = 1;

	if (allensembles)
		numensembles = m_pSimDoc->m_MStorage.m_AcceptedRuns.size();



	for (size_t iens = 0; iens < numensembles; iens++) {
		int firstnum = 0;
		bool firstvalid = false;
		if (allensembles)
			m_pSimDoc->m_MStorage.m_ActualEnsemble = iens;
		else
			m_pSimDoc->m_MStorage.m_ActualEnsemble = plot.EnsembleNo;

		m_pSimDoc->m_MStorage.SetActual_AcceptedEnsemble();
		if (plot.DataType == DATA_TYPES::SIM_PAR|| plot.DataType == DATA_TYPES::SIM_OUTSUM|| plot.DataType == DATA_TYPES::SIM_PERFORMANCE) {
			auto koll = m_pSimDoc->m_MStorage.m_Valid_TSV[iens].v[E_PRIOR_MEAN][plot.Index];

			


			for (int i = 0; i < m_ChartInfo.NumOfPoints; i++) AA[numvar * (iens + 1) + icurve][i] = Chart::NoValue;
			if (m_pSimDoc->m_MStorage.m_Valid_TSV[iens].v[E_PRIOR_MEAN][plot.Index] > -0.9E38) {
				for (size_t i = 0; i < m_pSimDoc->m_MStorage.m_AcceptedRuns[iens].size(); i++) {
					auto koll1 = AA[icurve][m_pSimDoc->m_MStorage.m_AcceptedRuns[iens][i]];
					auto koll2 = m_pSimDoc->m_MStorage.m_AcceptedRuns[iens][i];
					AA[numvar * (iens + 1) + icurve][i] = AA[icurve][m_pSimDoc->m_MStorage.m_AcceptedRuns[iens][i]];
				}
			}

			
		}

		m_dataSeriesA[numvar * (iens + 1) + icurve] = DoubleArray(&AA[numvar * (iens + 1) + icurve][0], m_ChartInfo.NumOfPoints);
	}
	m_pProp->m_PointType[icurve] = true;
	m_pSimDoc->m_MStorage.m_ActualEnsemble = save_ens;
	m_pSimDoc->m_MStorage.SetActual_AcceptedEnsemble();

}
void LoadDataToChart::AdjustForResiduals(double &v, CResiduals *pRes, long ResRunNo, int IndexVal, int LogLiVersion, long index, bool MeasuredAsSource)
{
	//	if(index>0) index--;
	if (m_BoxCox.use&&m_pUtil->m_ResidualPlot == 1) {
		float meas = v;
		float modelvalue = v - pRes->GetResiduals(ResRunNo, IndexVal, index);
		meas = m_pSimDoc->m_Simulator.m_StructureModel.m_Structure.boxcox(meas, m_BoxCox.box_power, m_BoxCox.box_offset);
		modelvalue = m_pSimDoc->m_Simulator.m_StructureModel.m_Structure.boxcox(modelvalue, m_BoxCox.box_power, m_BoxCox.box_offset);
		v = double(modelvalue) - meas;
	}
	else if (m_BoxCox.use&&m_pUtil->m_ResidualPlot == 0) {
		float modelvalue = v - pRes->GetResiduals(ResRunNo, IndexVal, index);
		v = m_pSimDoc->m_Simulator.m_StructureModel.m_Structure.boxcox(modelvalue, m_BoxCox.box_power, m_BoxCox.box_offset);
	}
	else {
		if (MeasuredAsSource) {
			if (LogLiVersion == 2)
				v = pRes->GetResiduals(ResRunNo, IndexVal, index);
			else if (LogLiVersion == 3) {
				if (v > 0.1 * MISSING)
					v -= pRes->GetResiduals(ResRunNo, IndexVal, index);
			}

		}
		else {
			if (LogLiVersion == 2)
				v = -pRes->GetResiduals(ResRunNo, IndexVal, index);
			else if (LogLiVersion == 3) {
				if (v > 0.1 * MISSING)
					v -= pRes->GetResiduals(ResRunNo, IndexVal, index);

			}
			else if (LogLiVersion == 0) {
				double meas = v;
				double modelvalue = v + pRes->GetResiduals(ResRunNo, IndexVal, index);
				v = m_pSimDoc->m_Simulator.m_StructureModel.m_Structure.SumLikelihood(size_t(IndexVal), modelvalue, meas);
			}
			else if (LogLiVersion == 1) {
				v = pRes->GetLogLi(ResRunNo, IndexVal, index, false);
			}
		}
	}

}
void LoadDataToChart::AdjustForResiduals(double& v, float ResValue, long ResRunNo, int IndexVal, int LogLiVersion, long index, bool MeasuredAsSource) {
	float meas, modelvalue;
	if (index > 0) index--;
	//if(m_pUtil->m_ResidualPlot<=1) {
	meas = v;
	if (m_BoxCox.use && m_pUtil->m_ResidualPlot == 1) {
		float meas = v;
		float modelvalue = v - ResValue;
		meas = m_pSimDoc->m_Simulator.m_StructureModel.m_Structure.boxcox(meas, m_BoxCox.box_power, m_BoxCox.box_offset);
		modelvalue = m_pSimDoc->m_Simulator.m_StructureModel.m_Structure.boxcox(modelvalue, m_BoxCox.box_power, m_BoxCox.box_offset);
		v = double(modelvalue) - meas;
	}
	else if (m_BoxCox.use && m_pUtil->m_ResidualPlot == 0) {
		float modelvalue = v - ResValue;
		v = m_pSimDoc->m_Simulator.m_StructureModel.m_Structure.boxcox(modelvalue, m_BoxCox.box_power, m_BoxCox.box_offset);
	}
	else {
		if (MeasuredAsSource) {
			if (LogLiVersion == 2)
				v = ResValue;
			else if (LogLiVersion == 3) {
				if (v > 0.1 * MISSING)
					v -= ResValue;
			}

		}
		else {
			if (LogLiVersion == 2)
				v = -ResValue;
			else if (LogLiVersion == 3) {
				if (v > 0.1 * MISSING)
					v -= ResValue;

			}
			else if (LogLiVersion == 0) {
				double meas = v;
				double modelvalue = v + ResValue;
				v = m_pSimDoc->m_Simulator.m_StructureModel.m_Structure.SumLikelihood(size_t(IndexVal), modelvalue, meas);
			}
			else if (LogLiVersion == 1) {
				v = ResValue;
			}
		}
	}

}

void LoadDataToChart::AdjustForMeanOfResiduals(double &v,CResiduals *pRes, bool AllAccepted, int IndexVal, int LogLiVersion, long index, int TypeStat)
{
	float meas, modelvalue;
	if (index>0) index--;
	if (pRes == nullptr) return;
	if (pRes->GetNumberofMeanResiduals(IndexVal) <= index) return;
	//if(m_pUtil->m_ResidualPlot<=1) {
	meas = v;
	if (TypeStat == 0) {
		if (!AllAccepted&&m_BoxCox.use) {
			modelvalue = v + pRes->GetMeanResiduals(IndexVal, index);
			meas = m_pSimDoc->m_Simulator.m_StructureModel.m_Structure.boxcox(meas, m_BoxCox.box_power, m_BoxCox.box_offset);
			modelvalue = m_pSimDoc->m_Simulator.m_StructureModel.m_Structure.boxcox(modelvalue, m_BoxCox.box_power, m_BoxCox.box_offset);

			v = double(modelvalue) - meas;
		}
		else if (!AllAccepted) {
			auto koll= pRes->GetMeanResiduals(IndexVal, index);
			v = -pRes->GetMeanResiduals(IndexVal, index);
		}
		else if (m_BoxCox.use) {
			modelvalue = v + pRes->GetMeanAllResiduals(IndexVal, index);
			meas = m_pSimDoc->m_Simulator.m_StructureModel.m_Structure.boxcox(meas, m_BoxCox.box_power, m_BoxCox.box_offset);
			modelvalue = m_pSimDoc->m_Simulator.m_StructureModel.m_Structure.boxcox(modelvalue, m_BoxCox.box_power, m_BoxCox.box_offset);
			v = double(modelvalue) - meas;
		}
		else
			v = -pRes->GetMeanAllResiduals(IndexVal, index);

	}
	else if (TypeStat == 1) {
		if (!AllAccepted)
			v = pRes->GetStdResiduals(IndexVal, index);
		else
			v = pRes->GetStdAllResiduals(IndexVal, index);

	}
	else if (TypeStat == 3) {
		if (!AllAccepted)
			v = -pRes->GetMinResiduals(IndexVal, index);
		else
			v = -pRes->GetMinAllResiduals(IndexVal, index);

	}
	else if (TypeStat == 2) {
		if (!AllAccepted)
			v = -pRes->GetMaxResiduals(IndexVal, index);
		else
			v = -pRes->GetMaxAllResiduals(IndexVal, index);

	}
	else if (TypeStat == 5) {
		if (!AllAccepted)
			v = -pRes->GetP10Residuals(IndexVal, index);
		else
			v = -pRes->GetP10AllResiduals(IndexVal, index);

	}
	else if (TypeStat == 4) {
		if (!AllAccepted)
			v = -pRes->GetP90Residuals(IndexVal, index);
		else
			v = -pRes->GetP90AllResiduals(IndexVal, index);

	}
	else if (TypeStat == 6) {
		if (!AllAccepted)
			v = -pRes->GetP50Residuals(IndexVal, index);
		else
			v = -pRes->GetP50AllResiduals(IndexVal, index);

	}

	if (LogLiVersion == 3 && TypeStat != 1)
		v += meas;

}

void LoadDataToChart::AdjustForMeanOfResidualsBetweenYear(double &v, CResiduals *pRes, bool AllAccepted, int IndexVal, int LogLiVersion, long index, int TypeStat)
{
	float meas;
	if (index>0) index--;
	if (m_pUtil->m_ResidualPlot <= 1) {
		meas = v;
		if (TypeStat == 0) {
			if (!AllAccepted)
				v = -pRes->GetMeanResiduals(IndexVal, index);
			else
				v = -pRes->GetMeanAllResiduals(IndexVal, index);
		}
		else if (TypeStat == 1) {
			if (!AllAccepted)
				v = -pRes->GetStdResiduals(IndexVal, index);
			else
				v = -pRes->GetStdAllResiduals(IndexVal, index);

		}
		else if (TypeStat == 2) {
			if (!AllAccepted)
				v = -pRes->GetMinResiduals(IndexVal, index);
			else
				v = -pRes->GetMinAllResiduals(IndexVal, index);

		}
		else if (TypeStat == 3) {
			if (!AllAccepted)
				v = -pRes->GetMaxResiduals(IndexVal, index);
			else
				v = -pRes->GetMaxAllResiduals(IndexVal, index);

		}
		else if (TypeStat == 4) {
			if (!AllAccepted)
				v = -pRes->GetP10Residuals(IndexVal, index);
			else
				v = -pRes->GetP10AllResiduals(IndexVal, index);

		}
		else if (TypeStat == 5) {
			if (!AllAccepted)
				v = -pRes->GetP90Residuals(IndexVal, index);
			else
				v = -pRes->GetP90AllResiduals(IndexVal, index);

		}

		if (m_pUtil->m_ResidualPlot == 0 && TypeStat != 1)
			v += meas;
	}
	else if (m_pUtil->m_ResidualPlot>1 && LogLiVersion == 0) {
		float meas = v;
		float modelvalue;
		if (!AllAccepted)
			modelvalue = v - pRes->GetMeanResiduals(IndexVal, index);
		else
			modelvalue = v - pRes->GetMeanAllResiduals(IndexVal, index);

		int ival = IndexVal + 1;
		//*v=VALIDATION_mp_LOGLI_LL(&meas, &modelvalue, &ival);
		v = m_pSimDoc->m_Simulator.m_StructureModel.m_Structure.SumLikelihood(size_t(IndexVal), modelvalue, meas);
		//AA[m_ChartInfo.NumVariables-2][i]=
	}
	else if (m_pUtil->m_ResidualPlot>1 && LogLiVersion == 1) {
		//*v=pRes->GetLogLi(ResRunNo,IndexVal,index, false);
	}
}

bool LoadDataToChart::LoadSelectedVariables(int j, size_t icurve, size_t i_band, CURVEINFO c_info, PLOTv p_info)
{

	Doc *pDocAct;
	auto datatype = m_pSimDoc->m_PlotStorage.GetDataType(j);
	int IndexVal = m_pSimDoc->m_PlotStorage.GetValVarNo(j);
	int SectionNo = m_pSimDoc->m_PlotStorage.GetPGSectionNo(j);
	int PGindex = m_pSimDoc->m_PlotStorage.GetVar_Index(j);


	int LogLiVersion = m_pSimDoc->m_PlotStorage.GetLogLiVersion(j);
	int PGNumVar;

	int ilocal = m_pSimDoc->m_PlotStorage.GetLocalIndex(j);

	if (c_info.Band&&SectionNo<5000) {
		m_ChartInfo.NumVariableBands++;
		PGNumVar = m_pPG_Active->GetNumVariables();
	}

	int idoc = m_pSimDoc->m_PlotStorage.GetDocIndex(j);
	Doc *pD = m_pSimDoc->m_PlotStorage.GetDocPointer(idoc);

	if (pD != nullptr)
		pDocAct = pD;
	else
		pDocAct = m_pSimDoc;
	if (m_pSimDoc->m_SelectedDocuments.size()>idoc)
		pDocAct = m_pSimDoc->m_SelectedDocuments[idoc];
	else
		pDocAct = m_pSimDoc;



	if (m_ChartInfo.NumVariables<NUM_SERIES) {

		m_pProp->m_ReShape.EquiDistance = 0;
		// Check if the PG file is regular in case of only only selected variables
		if (m_pPG_Active != nullptr) {
			if (m_pPG_Active->GetNumMonths() == m_pPG_Active->GetNumRecords())
				m_pPG_Active->CheckEquiDistance(true);
			else
				m_pPG_Active->CheckEquiDistance();

			if (m_pPG_Active->IsEquiDistance()) {
				m_pProp->m_ReShape.EquiDistance = m_pPG_Active->GetNormalTimeInterval();
			}
		}


		if (!m_pUtil->m_MultiRunPlot && !m_pProp->IsMultiPGFilePlot() && !m_pUtil->m_MultiPGPlot) {
			//Select Data from PG File and Residual file
			if(!CurveData_SingleTime(icurve, i_band, c_info, p_info)) return false;
		}
		else if (m_pUtil->m_MultiRunPlot) {
			if(!CurveData_MultiRun(icurve, p_info)) return false;
		}
		else if (m_pUtil->m_MultiPGPlot || m_pProp->IsMultiPGFilePlot()) {
			if(!CurveData_MultiPG(icurve, p_info)) return false;
		}

		if (m_pProp->m_ReShape.m_CommonDayNo)
			m_ChartInfo.NumOfPoints = m_pProp->m_ReShape.NumOfCommonPoints;

		// Replace PG Missing with ChartDirector Missing

		for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {
			if (abs(AA[icurve][i])>0.8E36)
				AA[icurve][i] = Chart::NoValue;
		}
		if (p_info.EnsembleType > ENSEMBLE_TYPE::NO) {
			for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {
				if (abs(BB[i_band][i])>0.8E36)
					BB[i_band][i] = Chart::NoValue;
				if (abs(BB_MIN[i_band][i])>0.8E36)
					BB_MIN[i_band][i] = Chart::NoValue;
				if (abs(BB_MAX[i_band][i])>0.8E36)
					BB_MAX[i_band][i] = Chart::NoValue;
			}
			m_dataSeriesB[i_band] = DoubleArray(&BB[i_band][0], m_ChartInfo.NumOfPoints);
			m_dataSeriesMIN[i_band] = DoubleArray(&BB_MIN[i_band][0], m_ChartInfo.NumOfPoints);
			m_dataSeriesMAX[i_band] = DoubleArray(&BB_MAX[i_band][0], m_ChartInfo.NumOfPoints);
			m_dataSeriesP10[i_band] = DoubleArray(&BB_P10[i_band][0], m_ChartInfo.NumOfPoints);
			m_dataSeriesP50[i_band] = DoubleArray(&BB_P50[i_band][0], m_ChartInfo.NumOfPoints);
			m_dataSeriesP90[i_band] = DoubleArray(&BB_P90[i_band][0], m_ChartInfo.NumOfPoints);
		}

		//m_PointType[m_ChartInfo.NumVariables-1]=false;
	}
	return true;
}
bool LoadDataToChart::TransformSelectedVariables(PLOTv p_info, size_t icurve, size_t i_band)
{

	auto datatype = p_info.DataType;
	int IndexVal = 0;
	//No Transformation of data from Time serie that are not based on PG file source
	if (m_pPG_Active == nullptr) return false;

	
	if (m_pProp->m_ReShape.m_Cumulated>0 && !m_pProp->m_ReShape.m_SortedSeries && (!m_pUtil->m_ResidualPlot || m_ChartInfo.ResidualSource || datatype != DATA_TYPES::SIM_VAL)) {
		m_pProp->MakeCumTranformation(&AA[icurve]);
		if (m_pSimDoc->m_PlotStorage.IsValidIndepPGVar() && p_info.DataType == DATA_TYPES::SIM_VAL) m_pProp->MakeCumTranformation(&TT_Indep[icurve]);
		
	}
	else if (m_pProp->m_ReShape.m_DerivateValues) {
		float Curr;
		float Prev = AA[icurve][0];
		for (long i = 0; i<m_ChartInfo.NumOfPoints; i++) {
			Curr = AA[icurve][i] - Prev;
			Prev = AA[icurve][i];
			AA[icurve][i] = Curr;
			if (p_info.EnsembleType > ENSEMBLE_TYPE::NO) {
				BB_P10[i_band][i] = Chart::NoValue;
				BB_P50[i_band][i] = Chart::NoValue;
				BB_P90[i_band][i] = Chart::NoValue;
				BB_MIN[i_band][i] = Chart::NoValue;
				BB_MAX[i_band][i] = Chart::NoValue;
				BB[i_band][i] = Chart::NoValue;
			}
		}
	}


	if (m_pProp->m_ReShape.m_SortedSeries || m_pProp->m_ReShape.m_NormalDist) {
		double Acc;
		SortDArray(&AA[icurve][0], m_ChartInfo.NumOfPoints);
		double mean, std, sum, p;
		//if(m_pProp->m_ReShape.m_NormalDist) {
		heapMean(&AA[icurve][0], m_ChartInfo.NumOfPoints, &sum, &mean, &std);
		//}
		if (m_pProp->m_ReShape.m_Cumulated>0) Acc = 0.0;
		for (long i = 0; i<m_ChartInfo.NumOfPoints; i++) {
			if (m_pProp->m_ReShape.m_NormalDist) {
				p = double(i + 0.5) / double(m_ChartInfo.NumOfPoints);
				TT[i] = Normal_InvD(p, mean, std);
			}
			else
				TT[i] = double(i + 1);

			if (m_pProp->m_ReShape.m_Cumulated>0) {
				if (abs(AA[icurve][i])<1.E38)
					Acc += AA[icurve][i];

				AA[icurve][i] = Acc;
			}

		}
	}
	return true;
}

bool LoadDataToChart::CurveData_MultiRun(size_t icurve, PLOTv p_info)
{
	/// Select data from MultiRunPlot
	Doc *pDocAct;
	if (m_pSimDoc->m_SelectedDocuments.size()>p_info.Doc)
		pDocAct = m_pSimDoc->m_SelectedDocuments[p_info.Doc];
	else
		pDocAct = m_pSimDoc;

	m_ChartInfo.NumOfPoints = pDocAct->MR_Get_TotalNumberofRuns();



	int ind_rmse;
	float rmse_o;
	int NumValidVar = 0;

	NumValidVar = m_pSimDoc->GetNumAll_TSV_ValVariables();

	if (p_info.DataType == DATA_TYPES::SIM_PERFORMANCE) {
		//m_pProp->SetFirstIndepVarType(MULTIRUN_NUM);
		auto koll = m_pProp->GetIndepFirst();
	}
	m_Contour.SecondMin = abs(MISSING);
	m_Contour.SecondMax = MISSING;


	m_Time.MinDate = 1.1E38;
	m_Time.MaxDate = -1.1E38;
	int index;
	float fvalue;

	ChartUtil *pUtil;
	pUtil = MySingle::instance().GetChartUtilPointer();
	if (pUtil->m_AutoShiftFirstIndep) {
		/*if (p_info.DataType == 98 || p_info.DataType == 99) {
			m_pProp->SetFirstIndepVarType(VALVARSUM);
			m_pProp->SetFirstIndepIndex(VALVARSUM, p_info.Index);
		}
		else*/ if (p_info.DataType == DATA_TYPES::SIM_SUMVAR) {
			m_pProp->SetFirstIndepVarType(VALVARSUM);
			m_pProp->SetFirstIndepIndex(VALVARSUM, p_info.Index);
		}
	}



	size_t Dim1Value = m_pSimDoc->m_MStorage.m_Valid_TSV[p_info.EnsembleNo].Dim1_Par_Index;
	if (Dim1Value == 0) 
		m_pSimDoc->m_MStorage.SetCyclicNumbers(1,0);
	else {
		auto koll = m_pSimDoc->m_MultiRun_Array[0].NumberOfRepeatitionsWithinDimension;
		m_pSimDoc->m_MStorage.SetCyclicNumbers(koll, Dim1Value - 1, Dim1Value);
	}


	VALSUMv valsum;
	switch (m_pProp->GetIndepFirst()) {
	case NO_USE:
	case MULTIRUN_NUM:
		m_Time.MinDate = double(1); m_Time.MaxDate = double(m_ChartInfo.NumOfPoints);
		for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {


				TT[i] = double(i + 1);

			//else
				//TT[i] = double(i + 1);
			if (TT[i]>m_Time.MaxDate) m_Time.MaxDate = TT[i];
			if (TT[i]<m_Time.MinDate) m_Time.MinDate = TT[i];
		}
		m_pProp->m_TimeScale.max = m_Time.MaxDate;
		m_pProp->m_TimeScale.min = m_Time.MinDate;
		//m_pProp->ResetXScale();

		break;
	case SUMVAR:
		index = m_pProp->GetFirstIndepIndex();
		index = p_info.Index;
		index = m_pProp->GetFirstIndepIndex();
		for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {

			TT[i] = double(pDocAct->m_MStorage.GetSumV(i, index - 6));
			if (TT[i]>m_Time.MaxDate) m_Time.MaxDate = TT[i];
			if (TT[i]<m_Time.MinDate) m_Time.MinDate = TT[i];
		}
		break;
	case VALVARSUM:
		index = m_pProp->GetFirstIndepIndex();

		valsum = m_pSimDoc->GetValSumStruct(index);
		if (!(p_info.Performance_Var == SUM_PERFORMANCE::SUM_OBS || (p_info.Performance_Var == SUM_PERFORMANCE::SUM_SIM)))
			m_ChartInfo.NumOfPoints = valsum.R2_Multi.size();
		
		for (int i = 0; i < m_ChartInfo.NumOfPoints; i++) {
				fvalue = valsum.ObsValueVector[i];
			if (pUtil->m_AutoShiftFirstIndep) {
				if (fvalue >= .5*MISSING) {
					TT_Indep[icurve][i] = double(valsum.ObsValueVector[i]);
				}
				else
					TT_Indep[icurve][i] = Chart::NoValue;


			}
			else {
				if (fvalue >= .5*MISSING) {
					switch (m_pProp->GetIndepValid_SV_Type()) {
					case SUM_PERFORMANCE::SUM_R2:
						TT[i] = double(valsum.R2_Multi[i]);
						break;
					case SUM_PERFORMANCE::SUM_INTERCEPT:
						TT[i] = double(valsum.InterCept_Multi[i]);
						break;
					case SUM_PERFORMANCE::SUM_SLOPE:
						TT[i] = double(valsum.Slope_Multi[i]);
						break;
					case SUM_PERFORMANCE::SUM_ME:
						TT[i] = double(valsum.SimValueVector[i] - valsum.ObsValueVector[i]);

						break;
					case SUM_PERFORMANCE::SUM_RMSE:
						TT[i] = double(valsum.RMSE[i]);
						break;
					case SUM_PERFORMANCE::SUM_OBS:
						TT[i] = double(valsum.ObsValueVector[i]);
						break;
					case SUM_PERFORMANCE::SUM_SIM:
						TT[i] = double(valsum.SimValueVector[i]);
						break;
					case SUM_PERFORMANCE::SUM_LOGLI:
						TT[i] = double(valsum.LogLi[i]);

						break;
					case SUM_PERFORMANCE::SUM_NSE:
						TT[i] = double(valsum.NSE[i]);

						break;

					}
					if (TT[i] > m_Time.MaxDate) m_Time.MaxDate = TT[i];
					if (TT[i] < m_Time.MinDate) m_Time.MinDate = TT[i];

				}
				else
					TT[i] = Chart::NoValue;
			}

		}
		break;
	case VALVAR:
		index = m_pProp->GetFirstIndepIndex();
		for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {
			if (!m_pProp->GetFirstIndepValidTimeType()) {
				//								if(p_info.DataType==9|| p_info.DataType == 99) {
				fvalue = pDocAct->m_MStorage.GetValidSumAll(i, index);
				if (fvalue > 0.9*MISSING)
					TT[i] = double(fvalue);
				else
					TT[i] = Chart::NoValue;

			}
			else {
				if (p_info.PGSection == 0) {
					if (p_info.index_cycle != string::npos)
						TT[i] = double(pDocAct->m_MStorage.GetValid(i, index));
					else
						TT[i] = double(pDocAct->m_MStorage.GetValid(i, index));
				}
				else if (p_info.PGSection > 0)
					TT[i] = double(pDocAct->m_MStorage.GetSplitValid(i, index, p_info.PGSection - 1));
			}
			if (TT[i] != Chart::NoValue) {
				if (TT[i] > m_Time.MaxDate) m_Time.MaxDate = TT[i];
				if (TT[i]<m_Time.MinDate&&TT[i]>-10.E6) m_Time.MinDate = TT[i];
			}
		}
		break;
	case PARVAR:
		m_Time.MinDate = 1.1E38;
		m_Time.MaxDate = -1.1E38;
		index = m_pProp->GetFirstIndepIndex();



		for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {


				TT[i] = double(pDocAct->m_MStorage.GetMultiP(i, index));
			

			if (TT[i]>m_Time.MaxDate) m_Time.MaxDate = TT[i];
			if (TT[i]<m_Time.MinDate) m_Time.MinDate = TT[i];
		}
		m_Contour.FirstMax = m_Time.MaxDate;
		m_Contour.FirstMin = m_Time.MinDate;
		
		//	TT[i]=double(pDocAct->m_MStorage.GetMultiP(i,m_pProp->GetFirstIndepIndex()-1));
		break;
	default:;
	
	}
	if (m_pProp->IsSecondIndepSet()) {
		size_t index = m_pProp->GetSecondIndepIndex();
		switch (m_pProp->GetIndepSecond()) {

		case MULTIRUN_NUM:
			m_Time.MinDate = double(1); m_Time.MaxDate = double(m_ChartInfo.NumOfPoints);
			for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {
	
					long irun = m_pSimDoc->m_MStorage.Get_AcceptedRuns(i);
					TT2[i] = double(irun + 1);

			//	else
			//		TT2[i] = double(i + 1);
				if (TT2[i]>m_Time.MaxDate) m_Time.MaxDate = TT2[i];
				if (TT2[i]<m_Time.MinDate) m_Time.MinDate = TT2[i];
			}
			break;
		case SUMVAR:
			index = m_pProp->GetSecondIndepIndex();
			for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {
					long irun = m_pSimDoc->m_MStorage.Get_AcceptedRuns(i);
					TT2[i] = double(pDocAct->m_MStorage.GetSumV(irun, index - 6));
//					TT2[i] = double(pDocAct->m_MStorage.GetSumV(i, index - 6));
				if (TT2[i]>m_Contour.SecondMax) m_Contour.SecondMax = TT2[i];
				if (TT2[i]<m_Contour.SecondMin) m_Contour.SecondMin = TT2[i];
			}
			break;
		case VALVARSUM:
			index = m_pProp->GetSecondIndepIndex();
			
			break;
		case VALVAR:
			index = m_pProp->GetSecondIndepIndex();
			for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {
				if (!m_pProp->GetSecondIndepValidTimeType()) {
					fvalue = pDocAct->m_MStorage.GetValidSumAll(i, index);
					if (fvalue > 0.9*MISSING) {
						TT2[i] = double(fvalue);
						if (TT2[i]>m_Time.MaxDate) m_Time.MaxDate = TT2[i];
						if (TT2[i]<m_Time.MinDate) m_Time.MinDate = TT2[i];
						if (TT2[i]>m_Contour.SecondMax) m_Contour.SecondMax = TT2[i];
						if (TT2[i]<m_Contour.SecondMin) m_Contour.SecondMin = TT2[i];

					}
					else
						TT2[i] = Chart::NoValue;

				}
				else {

						long irun = m_pSimDoc->m_MStorage.Get_AcceptedRuns(i);
						if (p_info.PGSection == 0)
							TT2[i] = double(pDocAct->m_MStorage.GetValid(irun, index));
						else if (p_info.PGSection > 0)
							TT2[i] = double(pDocAct->m_MStorage.GetSplitValid(irun, index, p_info.PGSection - 1));

					if (TT2[i]>m_Contour.SecondMax) m_Contour.SecondMax = TT2[i];
					if (TT2[i]<m_Contour.SecondMin) m_Contour.SecondMin = TT2[i];
				}

			}
			break;
		case PARVAR:
			m_Time.MinDate = 1.1E38;
			m_Time.MaxDate = -1.1E38;
			for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {

					//long irun = m_pSimDoc->m_MStorage.Get_AcceptedRuns(i);
					TT2[i] = double(pDocAct->m_MStorage.GetMultiP(i, index));

				if (TT2[i]>m_Contour.SecondMax) m_Contour.SecondMax = TT2[i];
				if (TT2[i]<m_Contour.SecondMin) m_Contour.SecondMin = TT2[i];
			}
			//	TT[i]=double(pDocAct->m_MStorage.GetMultiP(i,m_pProp->GetFirstIndepIndex()-1));
			break;
		}
	}

	auto constrain = [&]() {
		for (int i = 0; i < m_ChartInfo.NumOfPoints; i++) {
			for (int ii = 0; ii < p_info.m_ConstrainBy.size(); ii++) {
				size_t index = p_info.m_ConstrainBy[ii].IndepMultiPar;
				double min1 = p_info.m_ConstrainBy[ii].MinRangeValue;
				double max1 = p_info.m_ConstrainBy[ii].MaxRangeValue;
				double value = pDocAct->m_MStorage.GetMultiP(i, index);
				if (value <min1 || value>max1) AA[icurve][i] = Chart::NoValue;
			}
		}
	};

	if (p_info.DataType == DATA_TYPES::SIM_PERFORMANCE) {

		//if (p_info.EnsembleNo = 0) {

			//pDocAct->m_MStorage.m_ActualEnsemble = p_info.EnsembleNo;
		//	pDocAct->m_MStorage.SetActualPerformanceVector_TSV_Variables(pDocAct->m_MStorage.GetActualTransformation(p_info.EnsembleNo));
		// Changed to include all values independent of actual ensemble  
		//}
//else 
		//auto current_ens=pDocAct->m_MStorage.m_ActualEnsemble;
		vector<float> kollperformance;
		size_t n = m_ChartInfo.NumOfPoints;

		if (p_info.PGSection == 0) {

			auto stat_prior = m_pSimDoc->m_MStorage.GetValidStat(p_info.Index, false, p_info.EnsembleNo, 0);
			if (stat_prior.Mean < -0.9E38) {
				return false;
			}

			for (int i = 0; i < m_ChartInfo.NumOfPoints; i++) {
				AA[icurve][i] = double(pDocAct->m_MStorage.GetValid(i, p_info.Index, 0, 1));
				kollperformance.push_back(AA[icurve][i]);
			}
		}
		else if (p_info.PGSection>0)
			for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) AA[icurve][i] = double(pDocAct->m_MStorage.GetSplitValid(i, p_info.Index, p_info.PGSection - 1));


	}
	else if (p_info.DataType == DATA_TYPES::SIM_PAR) {
		for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {

				AA[icurve][i] = double(pDocAct->m_MStorage.GetMultiP(i, p_info.Index));


		}
		if (p_info.m_ConstrainBy.size() > 0) constrain();
		if (m_pProp->m_ChartType ==CHART_TYPES::FREQUENCE || m_pProp->m_ChartType ==CHART_TYPES::CUM_FREQUENCE) {
			m_pProp->SetDistScale(m_pSimDoc->MR_GetMin(p_info.Index), m_pSimDoc->MR_GetMax(p_info.Index), m_pSimDoc->MR_GetLogDist(p_info.Index));

		}
	}
	else if (p_info.DataType == DATA_TYPES::SIM_OUTSUM) {

		for (int i = 0; i < m_ChartInfo.NumOfPoints; i++) AA[icurve][i] = double(pDocAct->m_MStorage.GetSumV(i, p_info.Index - 6));
		if (p_info.m_ConstrainBy.size() > 0) constrain();

	}
	else if (p_info.DataType == DATA_TYPES::SIM_SUMVAR) { // SumValid

		valsum = m_pSimDoc->GetValSumStruct(p_info.Index);
		if (!(p_info.Performance_Var == SUM_PERFORMANCE::SUM_OBS || (p_info.Performance_Var == SUM_PERFORMANCE::SUM_SIM)))
			m_ChartInfo.NumOfPoints = valsum.R2_Multi.size();
		for (int i = 0; i < m_ChartInfo.NumOfPoints; i++) {
			float value = MISSING;
			switch (p_info.Performance_Var) {
				case SUM_PERFORMANCE::SUM_R2:
					value = valsum.R2_Multi[i];
					break;
				case SUM_PERFORMANCE::SUM_INTERCEPT:
					value = valsum.InterCept_Multi[i];
					break;
				case SUM_PERFORMANCE::SUM_SLOPE:
					value = valsum.Slope_Multi[i];
					break;
				case SUM_PERFORMANCE::SUM_ME:
					//value = valsum.ME[i];
					value = valsum.SimValueVector[i] - valsum.ObsValueVector[i];
					break;
				case SUM_PERFORMANCE::SUM_RMSE:
					value = valsum.RMSE[i];
					break;
				case SUM_PERFORMANCE::SUM_LOGLI:
					value = valsum.LogLi[i];
					break;
				case SUM_PERFORMANCE::SUM_OBS:
					value = valsum.ObsValueVector[i];
					break;
				case SUM_PERFORMANCE::SUM_SIM:
					value = valsum.SimValueVector[i];
					break;
				case SUM_PERFORMANCE::SUM_NSE:
					value = valsum.NSE[i];
					break;
			}
			if (value > 0.5*MISSING)
				AA[icurve][i] = double(value);
			else
				AA[icurve][i] = Chart::NoValue;
		}
	}

	return true;

}
bool LoadDataToChart::CurveData_SingleTime(size_t icurve, size_t i_band, CURVEINFO c_info, PLOTv p_info) {

	CPG *pPG1;
	int PGNumVar;
	if (c_info.Band&&p_info.PGSection<5000)
		PGNumVar = m_pPG_Active->GetNumVariables();
	PG_TIMECONTROL pg_control;

	pg_control.YearStartIndex;
	pg_control.StartIndex=1;



	if (m_pProp->m_ReShape.m_CommonDayNo || m_pProp->m_ReShape.m_CommonWithDay) {
		if (p_info.PGSection >= 5000 && p_info.PGSection<6000)
			pg_control = AdjustForWithinYearControl(p_info);

		else if (p_info.PGSection >= 6000 && p_info.PGSection<10000)
			pg_control = AdjustForWithinDayControl(p_info);
		else if (p_info.PGSection >= 10000)
			pg_control = AdjustForDisableControl();
		else if (p_info.PGSection != 0)
			pg_control = AdjustForSpecificPeriodControl(p_info);
		else {
			pg_control.YearStartIndex = 1;
			pg_control.StartIndex = 1;
		}
	}
	else {
		pg_control = AdjustForNormalTimeControl(icurve);
	}

	double Prev = Chart::NoValue;


	// Set Time start and End for first variable
	if (icurve == 0) SetTimeDataMinMax(int(p_info.DataType));

	size_t ActNoPoints = GetNumTimePoints(&pg_control, icurve, p_info);

	if (p_info.DataType == DATA_TYPES::PGDATA && m_ChartInfo.PGRepitMode)
		m_pPG_Active->SetRepititionSequence(p_info.PGRepitNo);
	else if (p_info.RepitDataType)
		m_pPG_Active->SetRepititionSequence(p_info.PGRepitNo);
	else if(p_info.DataType==DATA_TYPES::MEASURED_VAL|| p_info.DataType == DATA_TYPES::SIM_VAL && m_ChartInfo.PGRepitMode)
		m_pPG_Active->SetRepititionSequence(c_info.ResRunNo);
	if (p_info.PGSection<10000) {
		if (p_info.RepitDataType || pg_control.MeanValueBand) ReSizeBandDataVectors(i_band, ActNoPoints);
		if (icurve == 0 && (!m_pProp->m_ReShape.m_CommonDayNo && !m_pProp->m_ReShape.m_CommonWithDay)) SetTimeDataValues(ActNoPoints, pg_control);
		if (pg_control.MeanValueBand) SetMeanDataValues(ActNoPoints, pg_control, icurve, i_band, c_info);
		else if (m_pProp->m_MultiPG_dep.SecondType == PGHOUR) {
			if(!SetDataValuesForYearSelection(ActNoPoints, pg_control, icurve, p_info.PGFileIndex)) return false;
		}
		else if(!SetDataValues(ActNoPoints, pg_control, icurve, i_band, c_info)) return false;

	}
	if (m_pSimDoc->m_PlotStorage.IsValidIndepPGVar() && p_info.DataType == DATA_TYPES::SIM_VAL) {
		if (!SetIndependentMeasuredValidVar(p_info, c_info, icurve)) return false;

	}
	else if (!m_pSimDoc->m_PlotStorage.IsValidIndepPGVar() && p_info.DataType != DATA_TYPES::MEASURED_VAL)
		if (!SetIndependentSingleTime(p_info)) return false;

	return true;

}
bool LoadDataToChart::CurveData_MultiPG(size_t icurve, PLOTv p_info) {
	if (p_info.PGFileIndex == 0) p_info.PGFileIndex = 1;
	if (m_pProp->GetIndepFirst() == PGREPIT) {

		m_ChartInfo.NumOfPoints = m_pPG_Active->GetNumRepititions();
		m_pProp->SetFirstIndepVarType( PGVAR);
		m_pProp->SetSecondIndepVarType( PGVAR);
		//m_pProp->IsFirstIndepSet()=true;
		//m_pProp->IsSecondIndepSet()=true;
		for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {
			AA[icurve][i] = m_pPG_Active->GetVarAlt(p_info.PGFileIndex, i + 1);
			TT[i] = m_pPG_Active->GetVarLong(p_info.PGFileIndex, i + 1);
			TT2[i] = m_pPG_Active->GetVarLat(p_info.PGFileIndex, i + 1);
		}
	}
	if (p_info.IndepAxis.First == GEO_LONG || p_info.IndepAxis.First == GEO_LAT || p_info.IndepAxis.First == GEO_ALT
		|| p_info.IndepAxis.First == VAR_MIN || p_info.IndepAxis.First == VAR_MAX) {
		m_ChartInfo.NumOfPoints = m_pPG_Active->GetNumRepititions();
		m_pProp->SetFirstIndepVarType(p_info.IndepAxis.First);
		m_pProp->SetSecondIndepVarType(p_info.IndepAxis.Second);
		m_pProp->SetDependentVarType(p_info.IndepAxis.Dependent);
		m_Contour.FirstMax = MISSING;
		m_Contour.FirstMin = abs(MISSING);
		m_Contour.SecondMax = MISSING;
		m_Contour.SecondMin = abs(MISSING);

		for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {
			if (p_info.IndepAxis.Dependent == GEO_ALT)
				AA[icurve][i] = m_pPG_Active->GetVarAlt(p_info.PGFileIndex, i + 1);
			else if (p_info.IndepAxis.Dependent == VAR_MIN)
				AA[icurve][i] = m_pPG_Active->GetVarMin(p_info.PGFileIndex, i + 1);
			else if (p_info.IndepAxis.Dependent == VAR_MAX)
				AA[icurve][i] = m_pPG_Active->GetVarMax(p_info.PGFileIndex, i + 1);
			else if (p_info.IndepAxis.Dependent == VAR_FIRST)
				AA[icurve][i] = m_pPG_Active->GetVarValue(p_info.PGFileIndex, i + 1);
			else if (p_info.IndepAxis.Dependent == VAR_LAST)
				AA[icurve][i] = m_pPG_Active->GetVarValue(p_info.PGFileIndex, 1);
			else if (p_info.IndepAxis.Dependent == VAR_MEAN) {
				AA[icurve][i] = m_pPG_Active->GetVarMean(p_info.PGFileIndex, i + 1);
			}

			if (p_info.IndepAxis.First == GEO_LONG)
				TT[i] = m_pPG_Active->GetVarLong(p_info.PGFileIndex, i + 1);
			else if (p_info.IndepAxis.First == GEO_LAT)
				TT[i] = m_pPG_Active->GetVarLat(p_info.PGFileIndex, i + 1);
			else if (p_info.IndepAxis.First == VAR_MIN)
				TT[i] = m_pPG_Active->GetVarMin(p_info.PGFileIndex, i + 1);
			else if (p_info.IndepAxis.First == VAR_MAX)
				TT[i] = m_pPG_Active->GetVarMax(p_info.PGFileIndex, i + 1);
			else if (p_info.IndepAxis.First == GEO_ALT)
				TT[i] = m_pPG_Active->GetVarAlt(p_info.PGFileIndex, i + 1);

			if (p_info.IndepAxis.Second == GEO_LONG)
				TT2[i] = m_pPG_Active->GetVarLong(p_info.PGFileIndex, i + 1);
			else if (p_info.IndepAxis.Second == GEO_LAT)
				TT2[i] = m_pPG_Active->GetVarLat(p_info.PGFileIndex, i + 1);
			else if (p_info.IndepAxis.Second == VAR_MIN)
				TT2[i] = m_pPG_Active->GetVarMin(p_info.PGFileIndex, i + 1);
			else if (p_info.IndepAxis.Second == VAR_MAX)
				TT2[i] = m_pPG_Active->GetVarMax(p_info.PGFileIndex, i + 1);
			else if (p_info.IndepAxis.Second == GEO_ALT)
				TT2[i] = m_pPG_Active->GetVarAlt(p_info.PGFileIndex, i + 1);
			if (TT[i]>m_Contour.FirstMax) m_Contour.FirstMax = TT[i];
			if (TT[i]<m_Contour.FirstMin) m_Contour.FirstMin = TT[i];
			if (TT2[i]>m_Contour.SecondMax) m_Contour.SecondMax = TT2[i];
			if (TT2[i]<m_Contour.SecondMin) m_Contour.SecondMin = TT2[i];
		}
		string str;
		if (p_info.IndepAxis.First == GEO_LONG) str = "Longitude";
		else if (p_info.IndepAxis.First == GEO_LAT) str = "Latitude";
		else if (p_info.IndepAxis.First == VAR_MIN) { str = "Min of : "; str += m_pPG_Active->GetVarName(p_info.PGFileIndex); }
		else if (p_info.IndepAxis.First == VAR_MAX) { str = "Max of : "; str += m_pPG_Active->GetVarName(p_info.PGFileIndex); }
		else if (p_info.IndepAxis.First == GEO_ALT) str = "Altitude";
		m_pProp->SetFirstIndepSet(p_info.IndepAxis.First, true);
		m_pProp->SetFirstIndepString(p_info.IndepAxis.First, str);


		if (p_info.IndepAxis.Second == GEO_LONG) str = "Longitude";
		else if (p_info.IndepAxis.Second == GEO_LAT) str = "Latitude";
		else if (p_info.IndepAxis.Second == VAR_MIN) { str = "Min of : "; str += m_pPG_Active->GetVarName(p_info.PGFileIndex); }
		else if (p_info.IndepAxis.Second == VAR_MAX) { str = "Max of : "; str += m_pPG_Active->GetVarName(p_info.PGFileIndex); }
		else if (p_info.IndepAxis.Second == GEO_ALT) str = "Altitude";

		if (p_info.IndepAxis.Second != NO_USE) {
			m_pProp->SetSecondIndepSet(p_info.IndepAxis.Second, true);
			m_pProp->SetSecondIndepString(p_info.IndepAxis.Second, str);
		}
		else {
			m_pProp->SetSecondIndepSet(p_info.IndepAxis.Second, false);
			m_pProp->SetSecondIndepString(p_info.IndepAxis.Second, "");
			p_info.ChartType = CHART_TYPES::SCATTER;
		}

	}
	else {
		//	m_ChartInfo.NumOfPoints=m_pPG_Active->GetNumRecords();
		if (p_info.PGFileIndex>0) {
			if (m_ChartInfo.WithinDayRepresentation == 1)
				for (int i = 0; i<m_ChartInfo.NumOfPoints; i++)
					AA[icurve][i] = m_pPG_Active->GetVarValue(p_info.PGFileIndex, i + 1);
			else {
				if (m_pProp->m_HourValueRepresentation == 0) {
					for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {
						double sum = 0.;
						for (size_t ii = 0; ii<m_ChartInfo.WithinDayRepresentation; ii++) sum += m_pPG_Active->GetVarValue(p_info.PGFileIndex, i*m_ChartInfo.WithinDayRepresentation + ii + 1);
						AA[icurve][i] = sum / m_ChartInfo.WithinDayRepresentation;


					}
				}
				else {
					for (int i = 0; i<m_ChartInfo.NumOfPoints; i++)
						AA[icurve][i] = m_pPG_Active->GetVarValue(p_info.PGFileIndex, i*m_ChartInfo.WithinDayRepresentation + m_pProp->m_HourValueRepresentation - 1 + 1);
				}






			}


		}

		SetIndependentSingleTime(p_info);

	}
	return true;

}

PG_TIMECONTROL LoadDataToChart::AdjustForNormalTimeControl(size_t icurve) {

	PG_TIMECONTROL out;
	out.YearStartIndex = 0;
	out.StartIndex = 1;
	if (icurve == 0) {
		if (m_pProp->GetIndepSecond() == PGHOUR&&m_pPG_Active->GetNumberofYears()>1) {
			size_t nvalues = 1440 / m_pPG_Active->GetNormalTimeInterval();
			size_t nextra = m_pPG_Active->GetRecordsInYear(1) / nvalues;
			m_ChartInfo.NumOfPoints = m_pPG_Active->GetRecordsInYear(1) + nextra;
			if (m_ChartInfo.YearRepresentation>0)
				out.YearStartIndex = m_pPG_Active->GetYearIndex(m_ChartInfo.YearRepresentation);

		}
		else if (m_ChartInfo.YearRepresentation>0) {
			out.YearStartIndex = m_pPG_Active->GetYearIndex(m_ChartInfo.YearRepresentation);
			out.StartIndex = 1;
		}
	}
	out.MeanValueBand = false;
	out.YearBand = true;
	return out;

}
PG_TIMECONTROL LoadDataToChart::AdjustForSpecificPeriodControl(PLOTv p_info) {
	PG_TIMECONTROL out;
	out.MeanValueBand = false;
	m_ChartInfo.NumOfPoints = m_pPG_Active->GetRecordsInSection(p_info.PGSection);
	out.StartIndex = m_pPG_Active->GetIndexInSection(p_info.PGSection);
	out.YearStartIndex = m_pProp->m_ReShape.NumOfCommonPoints - m_ChartInfo.NumOfPoints;
	return out;
}
PG_TIMECONTROL LoadDataToChart::AdjustForWithinYearControl(PLOTv p_info) {
	PG_TIMECONTROL out;
	out.MeanValueBand = true;
	//int kalle=ResRunNo;
	if (p_info.DataType == DATA_TYPES::PGDATA && m_ChartInfo.PGRepitMode) {
		size_t irepit = p_info.PGRepitNo;
		if (m_pPG_Active->GetRepititionSequence() != irepit) {
			m_pPG_Active->SetRepititionSequence(irepit);
			m_pPG_Active->EstimateWithinYearClear(irepit);
		}
	}

	if (m_ChartInfo.ResidualSource)
		out.YearBand = m_pPG_Active->EstimateWithinYearStatistics(m_pSimDoc);
	else
		out.YearBand = m_pPG_Active->EstimateWithinYearStatistics(nullptr);

	double t;
	t = m_pPG_Active->GetDoubleTimeYearStart(1);
	m_ChartInfo.NumOfPoints = m_pPG_Active->GetNumberOfCommonPoints();
	out.YearStartIndex = m_pProp->m_ReShape.NumOfCommonPoints - m_ChartInfo.NumOfPoints;
	out.StartIndex = m_pPG_Active->GetIndexInSection(1);
	m_ChartInfo.NumVariableBands++;
	return out;
}
PG_TIMECONTROL LoadDataToChart::AdjustForWithinDayControl(PLOTv p_info) {
	PG_TIMECONTROL out;
	if (m_pPG_Active->EstimateWithinDayStatistics()) {
		out.YearBand = false;
		out.MeanValueBand = true;
		m_ChartInfo.NumOfPoints = m_pProp->m_ReShape.NumOfCommonPoints;
	}
	else {
		out.YearBand = false;
		out.MeanValueBand = false;



	}
	return out;

}
PG_TIMECONTROL LoadDataToChart::AdjustForDisableControl() {
	PG_TIMECONTROL out;
	out.StartIndex = 1;
	out.YearStartIndex = 0;
	out.MeanValueBand = false;
	out.YearBand = true;
	m_ChartInfo.NumOfPoints = m_pProp->m_ReShape.NumOfCommonPoints;
	return out;

}
size_t LoadDataToChart::GetNumTimePoints(PG_TIMECONTROL *pg_control, size_t icurve, PLOTv p_info) {
	size_t ActNoPoints;
	bool Regular;

	// Set Time for CommonDay Approach based on common regular intervals
	if (m_pProp->m_ReShape.m_CommonDayNo || m_pProp->m_ReShape.m_CommonWithDay) {
		pg_control->YearStartIndex = 0;
		double midint = double(m_pProp->m_ReShape.EquiDistance) * 30; // make mid point within interval (0.5*timeint)
		ActNoPoints = m_pProp->m_ReShape.NumOfCommonPoints;
		if (AA[icurve].size()<m_pProp->m_ReShape.NumOfCommonPoints) AA[icurve].resize(m_pProp->m_ReShape.NumOfCommonPoints);
		for (int i = 0; i<m_pProp->m_ReShape.NumOfCommonPoints; i++) {
			if (icurve == 0)
				TT[i] = double(i) / double(m_pProp->m_ReShape.NumOfCommonPoints)*(m_Time.MaxDate - m_Time.MinDate) + m_Time.MinDate + midint;
			AA[icurve][i] = Chart::NoValue;
		}

	}
	else if (icurve == 0) {
		// Removed conditions when cancelling of RepitionsMode 2021-07-18
	//	if (!(p_info.DataType == DATA_TYPES::PGDATA && !m_ChartInfo.PGRepitMode) && !p_info.RepitDataType&& !m_pSimDoc->IsMultiSimulation())
	//		m_ChartInfo.pPGFirst->SetRepititionMode(false);
		ActNoPoints = m_ChartInfo.NumOfPoints;
	}
	else {
		long nPoint = m_pPG_Active->GetNumRecords();
		if (nPoint == m_ChartInfo.NumOfPoints) {
			ActNoPoints = m_ChartInfo.NumOfPoints;
		}
		else {
			m_ChartInfo.pPGFirst->ReOpen();
			long ls1 = m_ChartInfo.pPGFirst->GetLongTimeStart();
			long le1 = m_ChartInfo.pPGFirst->GetLongTimeEnd();
			long ls2 = m_pPG_Active->GetLongTimeStart();
			long le2 = m_pPG_Active->GetLongTimeEnd();

			if (m_pPG_Active->GetNumRecordsMonth(1) == m_pPG_Active->GetNumRecords()) {

				m_pPG_Active->CheckEquiDistance(true);


			}
			else
				m_pPG_Active->CheckEquiDistance();
			m_ChartInfo.pPGFirst->CheckEquiDistance();
			if (m_pPG_Active->GetNormalTimeInterval() != m_ChartInfo.pPGFirst->GetNormalTimeInterval())
				Regular = false;
			else
				Regular = true;
			if (Regular) {
				if (ls2<ls1) {
					pg_control->StartIndex = m_pPG_Active->GetRecord(ls1);
					if (le2<le1) {
						long IEnd = m_ChartInfo.pPGFirst->GetRecord(le2);
						for (long i = IEnd; i<m_ChartInfo.NumOfPoints; i++)
							AA[icurve][i] = Chart::NoValue;
						ActNoPoints = IEnd;

					}
					else {
						ActNoPoints = m_ChartInfo.NumOfPoints;
					}
					pg_control->YearStartIndex = 0;
				}
				else if (ls2 >= ls1) {
					pg_control->StartIndex = 1;
					long IEnd = m_ChartInfo.pPGFirst->GetRecord(ls2) - 1;
					if (IEnd == -1)
						IEnd = m_ChartInfo.NumOfPoints;
					for (long i = 0; i<IEnd; i++)
						AA[icurve][i] = Chart::NoValue;
					pg_control->YearStartIndex = IEnd;
					ActNoPoints = m_ChartInfo.NumOfPoints - IEnd;
				};
			}
			else {
				ActNoPoints = 0;
				for (long i = 0; i<m_ChartInfo.NumOfPoints; i++)
					AA[icurve][i] = Chart::NoValue;
			}
		}
	}

	return ActNoPoints;

}
bool LoadDataToChart::SetDataValuesForYearSelection(size_t ActNoPoints, PG_TIMECONTROL pg_control, size_t icurve, int PGIndex) {
	double sum;
	size_t count;
	float value;
	size_t countextra = 0;

	for (int i = 0; i<ActNoPoints; i++) {

		if (m_ChartInfo.YearRepresentation == 0) {
			sum = 0.;
			count = 0;
			for (size_t ii = 0; ii<m_ChartInfo.NumOfYears; ii++) {
				value = m_pPG_Active->GetVarValue(PGIndex, i - countextra + pg_control.StartIndex + m_pPG_Active->GetYearIndex(ii + 1));
				if (value>-1.E38) {
					sum += value;
					count++;
				}
			}
			AA[icurve][i] = sum / count;
		}
		else
			AA[icurve][i] = m_pPG_Active->GetVarValue(PGIndex, i - countextra + pg_control.StartIndex + pg_control.YearStartIndex);
		if ((i + 1) % m_ChartInfo.NumExtraPointFreq == 0) countextra++;
	}
	return true;
}


bool LoadDataToChart::SetTimeDataValues(size_t ActNoPoints, PG_TIMECONTROL pg_control) {
	double sum;
	if (!m_pPG_Active->ReOpen()) return false;
	for (int i = 0; i<ActNoPoints; i++)

		if (m_ChartInfo.WithinDayRepresentation>1) {
			if (m_pProp->m_HourValueRepresentation == 0) {
				sum = 0.;
				for (size_t ii = 0; ii<m_ChartInfo.WithinDayRepresentation; ii++) sum += m_pPG_Active->GetDoubleTime(long(i*m_ChartInfo.WithinDayRepresentation + pg_control.StartIndex + ii));
				TT[i] = sum / m_ChartInfo.WithinDayRepresentation;
			}
			else
				TT[i] = m_pPG_Active->GetDoubleTime(long(i*m_ChartInfo.WithinDayRepresentation + pg_control.StartIndex + m_pProp->m_HourValueRepresentation - 1));
		}
		else   if(i<m_pPG_Active->GetNumRecords())
			TT[i] = m_pPG_Active->GetDoubleTime(long(i + pg_control.StartIndex));
		else {
			TT[i] = TT[i % m_pPG_Active->GetNumRecords()];


		}
	return true;
}
bool LoadDataToChart::SetDataValues(size_t ActNoPoints, PG_TIMECONTROL pg_control, size_t icurve, size_t i_band, CURVEINFO c_info) {

	int j = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, icurve);
	PLOTv p_info = m_pSimDoc->m_PlotStorage.GetPlotInfo(j);
	int PGNumVar;
	auto kollens = p_info.EnsembleNo;
	int PGindex = p_info.PGFileIndex;
	PGNumVar = m_pPG_Active->GetNumVariables();
	auto datatype = m_pSimDoc->m_PlotStorage.GetDataType(j);
	int Residual_VarNum;
	//if(p_info.index_cycle!=string::npos) Removed conditions forcing Residual to only first and One Validation variable
	//	 Residual_VarNum = 0;
	//else
		Residual_VarNum = p_info.Residual_VarNum;
	int LogLiVersion = m_pSimDoc->m_PlotStorage.GetLogLiVersion(j);

	if (PGindex<0) {
		for (size_t i = 0; i<ActNoPoints; i++) SetDataValueMissing(icurve, pg_control, i);
		return false;
	}
	//Check number of points

	size_t koll = m_pPG_Active->GetNumRecords()*m_pPG_Active->GetNumRepititions();
	if (koll<ActNoPoints) {
		for (size_t i = 0; i<ActNoPoints; i++) SetDataValueMissing(icurve, pg_control, i);
		return false;


	}
	if (p_info.pPG_MeasValFile != NULL && m_pUtil->m_ResidualPlot == 1|| p_info.DataType == DATA_TYPES::SIM_VAL) {
		p_info.pPG_MeasValFile->SetCompleteRead(true);
		p_info.pPG_MeasValFile->CloseFile();
		p_info.pPG_MeasValFile->Open(p_info.pPG_MeasValFile->GetFileName());
		p_info.pPG_MeasValFile->ReOpen();

	}
	size_t save_ensemble_no;

	if (m_ChartInfo.MultiValMode&& m_pSimDoc->m_MStorage.m_AcceptedRuns.size()>0&&datatype==DATA_TYPES::SIM_VAL) {
		size_t iens = m_pSimDoc->m_MStorage.m_ActualEnsemble;
		//m_pSimDoc->m_MStorage.m_AcceptedRuns[iens][iaccept] + startvalue;
		auto AcceptedCandidates = m_pSimDoc->m_MStorage.m_AcceptedRuns[iens];
		vector<size_t> NewList;
		size_t accept_seq{ 0 };
		if (p_info.pPG_MeasValFile->GetNumRepititions() > 1) {
			for (size_t i = 0; i < AcceptedCandidates.size(); i++) {
				if (m_pSimDoc->ValidDim1VersusRunNo(iens, AcceptedCandidates[i]))
					NewList.push_back(AcceptedCandidates[i]);
			}
			AcceptedCandidates = NewList;
		}


		if (AcceptedCandidates.size() > p_info.Residual_Sequence) {
			size_t runno = AcceptedCandidates[p_info.Residual_Sequence];
			p_info.Residual_RunNo = runno;
			m_pSimDoc->m_PlotStorage.SetValRunNo(j,runno, p_info.Residual_Sequence);
			c_info.ResRunNo = runno;
		}
		else {
			MFC_Util::MessageBoxW("No more Accepted runs");
			return false;
		}


	}


	CResiduals *pResAct = c_info.pRes;
	int ResRunNoAct = c_info.ResRunNo;
	if (pg_control.StartIndex <= 0) return false;
	kollens = p_info.EnsembleNo;

	if (p_info.EnsembleType>ENSEMBLE_TYPE::NO && pResAct != nullptr) {

		save_ensemble_no = m_pSimDoc->m_MStorage.m_ActualEnsemble;
		// Set the correct ensemble using number....
		if (!pResAct->SetNewEnsembleByNumber(p_info.EnsembleNo)) {
			// Error when shiftning to new ensemble
			int i = 0;
			MFC_Util::MessageBoxW("Error No mean value estimated");
			return false;
		}



	}

	auto kollrep = m_pPG_Active->GetRepititionSequence();
	size_t NumAgg = 1;
	Ps* pAgg = m_pSimDoc->GetPs("Aggregate Calib Number");
	if (pAgg != nullptr) NumAgg=size_t(pAgg->GetValue());
	Ps* pValIndex;
	int ValIndex_In_MultiParList=-1;
	vector<float> SumVal;
	vector<int> NumValid;

	int VAL_PGindex = PGindex;
	size_t act_rep = 0;
	vector<size_t> RepHits;

	
	if (p_info.RepitDataType)
		act_rep = p_info.PGRepitNo;
	else
		act_rep = 0;

	bool Mean_Of_Repititions = false;
	vector<double> Mean_Of_Measured_Value, Simulated_SingleRunValue;

	if (p_info.pPG_MeasValFile != nullptr&&m_pSimDoc->IsMultiSimulation()) {
		if (p_info.pPG_MeasValFile->GetNumRepititions() > 1) {
			auto rephits = m_pSimDoc->ActualRepetitionInMeasFile(p_info.EnsembleNo, p_info.pPG_MeasValFile, p_info.Index, p_info.Residual_RunNo);
			RepHits = rephits;
			if (rephits.size() > 0) act_rep = rephits.front();
			Mean_Of_Repititions = true;
		}
		else {
			auto pSw = m_pSimDoc->GetSw("PreDefinedEnsembles");
			if (pSw > 0) {
				auto cycle_pair = m_pSimDoc->StartValueofMatchingEnsembleInDim1(p_info.EnsembleNo, p_info.pPG_MeasValFile, p_info.Index);
				if (cycle_pair.first == string::npos) act_rep = cycle_pair.first + 1;
			}
		}
	}
	else if(p_info.pPG_MeasValFile !=nullptr) { // Single Run Case
	
			if (p_info.pPG_MeasValFile->GetNumRepititions() > 1) {
				auto rephits = m_pSimDoc->ActualRepetitionInMeasFile(string::npos, p_info.pPG_MeasValFile, p_info.Index);
				RepHits = rephits;
				if (rephits.size() > 0) act_rep = rephits.front();
				Mean_Of_Repititions = true;
			}
			else {
				act_rep = 0;
				Mean_Of_Repititions = false;
			}	
	}

	vector<float> points;

	//for (size_t index_ValAgg = 0; index_ValAgg < NumAgg; index_ValAgg++) {

		//if (NumAgg > 1&& m_ChartInfo.ResidualSource) {
		//	if(ValIndex_In_MultiParList>=0)	PGindex=size_t(m_pSimDoc->m_MStorage.GetMultiP(c_info.ResRunNo, ValIndex_In_MultiParList));

		//}
		bool allrep = false;
		size_t NumRepit = m_pPG_Active->GetNumRepititions();
		if (NumRepit <= 1) allrep = false;
		else if (p_info.RepitDataType&& p_info.DataType == DATA_TYPES::SIM_VAL) allrep = true;
		else if (p_info.RepitDataType && p_info.DataType == DATA_TYPES::MEASURED_VAL) allrep = true;
		if (act_rep >= 0) {
			allrep = false;
			m_pPG_Active->SetRepititionSequence(act_rep, true);
			if(p_info.pPG_MeasValFile!=nullptr)
				p_info.pPG_MeasValFile->SetRepititionSequence(act_rep, true);
		}
		
		size_t act_rec;
		m_pPG_Active->SetRepititionSequence(act_rep);
		size_t last_rep = 0;
		size_t count = 0;
		koll = 0;

		size_t inc_step = 1;
		size_t start_pos = 0;
		if (p_info.index_cycle != string::npos) {
			if (p_info.index_cycle > 1) inc_step = p_info.index_cycle;
			start_pos = p_info.pg_start_pos % inc_step;
		}
	
		if (inc_step > 1) {
			for (size_t i = 0; i < ActNoPoints; i++) {
				AA[icurve][i] = MISSING;
				if (p_info.EnsembleType > ENSEMBLE_TYPE::NO) {
					SetDataBandValueMissing(i_band, pg_control, i);
				}

			}
	
		}
		size_t const NumStatPoints=7;
		vector<float> ActiveResiduals, Mean_Of_ResidualsValue, Mean_Of_Mean_ResidualsValue[NumStatPoints];
		size_t Start_SubSet_Residuals = 0;
		if (c_info.pRes != nullptr) {
			if (Mean_Of_Repititions) {
				Mean_Of_ResidualsValue.resize(ActNoPoints);
				if (p_info.EnsembleType > ENSEMBLE_TYPE::NO) {
					for(size_t i=0;i<NumStatPoints;i++)	Mean_Of_Mean_ResidualsValue[i].resize(ActNoPoints);
				}
				
					vector< vector<float>> ValidValue, ValidMeanValue[NumStatPoints];
					ValidValue.resize(ActNoPoints);
					for (size_t i_stat = 0; i_stat < NumStatPoints; i_stat++) 	ValidMeanValue[i_stat].resize(ActNoPoints);

					for (size_t irepit = 0; irepit < RepHits.size(); irepit++) {
						ActiveResiduals = m_pSimDoc->GetResidualVectorForAllRuns(c_info.ResRunNo, p_info.Index, p_info.Residual_VarNum, ActNoPoints, RepHits[irepit]);
						if (p_info.EnsembleType > ENSEMBLE_TYPE::NO) {
							for (size_t i_stat = 0; i_stat < 7; i_stat++) {
								Mean_Of_Mean_ResidualsValue[i_stat] = m_pSimDoc->GetMeanResidualVectorForAllRuns(p_info.Index,p_info.Residual_VarNum, ActNoPoints, RepHits[irepit], true, STAT_INDICATOR(i_stat));
							}
						}

						for (size_t i = 0; i < ActNoPoints; i++){
							if (ActiveResiduals[i] > -1.E38) {
								ValidValue[i].push_back(-ActiveResiduals[i]);
								if (p_info.EnsembleType > ENSEMBLE_TYPE::NO) {
									for (size_t i_stat = 0; i_stat < NumStatPoints; i_stat++) {
										ValidMeanValue[i_stat][i].push_back(Mean_Of_Mean_ResidualsValue[i_stat][i]);
									}
								}
							}
						}
					}
					for (size_t irec = 0; irec < ActNoPoints; ++irec) {
						if (ValidValue[irec].size() > 0) {
							size_t ncheck;
							DistStat stat;
							vector<float> farray;
							farray= ValidValue[irec];
							StatUtil::SortArray(&farray[0], farray.size());
							MeanStat(&farray[0], farray.size(), &ncheck, &stat);
							Mean_Of_ResidualsValue[irec] = stat.Mean;
							if (p_info.EnsembleType > ENSEMBLE_TYPE::NO) {
								for (size_t i_stat = 0; i_stat < NumStatPoints; i_stat++) {
									farray = ValidMeanValue[i_stat][irec];
									StatUtil::SortArray(&farray[0], farray.size());
									MeanStat(&farray[0], farray.size(), &ncheck, &stat);
									Mean_Of_Mean_ResidualsValue[i_stat][irec] = stat.Mean;
								}

							}

						}
						else {
							Mean_Of_ResidualsValue[irec] = MISSING;
							if (p_info.EnsembleType > ENSEMBLE_TYPE::NO) {
								for (size_t i_stat = 0; i_stat < NumStatPoints; i_stat++) {
									Mean_Of_Mean_ResidualsValue[i_stat][irec] = MISSING;
								}
							}

						}
					}
					ActiveResiduals=Mean_Of_ResidualsValue;

			}
			
			Start_SubSet_Residuals = ActNoPoints * (act_rep);
		}
		else {
			if (!m_pSimDoc->IsMultiSimulation()&& p_info.DataType == DATA_TYPES::SIM_VAL) {
				Simulated_SingleRunValue.resize(ActNoPoints);
				
				for (size_t i = 0; i < ActNoPoints; i++) {
					Simulated_SingleRunValue[i]= p_info.pPG->GetVarValue(p_info.PGFileIndex,i+1);
				}


			}
			else 
			ActiveResiduals.assign(ActNoPoints, MISSING);
		}
		

		if (Mean_Of_Repititions) {
			Mean_Of_Measured_Value.resize(ActNoPoints);
			for (size_t irec = 0; irec < ActNoPoints; ++irec) {
				vector<float> ValidValue;
				for (size_t irepit = 0; irepit < RepHits.size(); irepit++) {
					auto value = p_info.pPG_MeasValFile->GetVarValue(PGindex, irec + 1, RepHits[irepit]);
					if (value > -1.E38) ValidValue.push_back(value);
				}
				if (ValidValue.size() > 0) {
					size_t ncheck;
					DistStat stat;
					MeanStat(&ValidValue[0], ValidValue.size(), &ncheck, &stat);
					Mean_Of_Measured_Value[irec] = stat.Mean;
				}
				else
					Mean_Of_Measured_Value[irec] = MISSING;
				if (TT_Indep.size() > icurve) {
					TT_Indep[icurve][irec] = Mean_Of_Measured_Value[irec];
				}
			}
		}
		else {
			for (size_t irec = 0; irec < ActNoPoints; ++irec) {
				if (TT_Indep.size() > icurve) {
					TT_Indep[icurve][irec] = p_info.pPG_MeasValFile->GetVarValue(PGindex, irec + 1);
				}
			}


		};



		for (size_t i = start_pos; i < ActNoPoints; i+=inc_step) {
			act_rec = i;
			if (allrep) {

				//act_rep = i / m_pPG_Active->GetNumRecords() + 1;
				//act_rep = 2; // Test to be replace by RunNo control
				m_pPG_Active->SetRepititionSequence(act_rep);
				act_rec = i % m_pPG_Active->GetNumRecords();

			}
			else if (m_pProp->m_ConstrainProp.size() > 0&& m_ChartInfo.ResidualSource && NumRepit > 1) {
				for (size_t rep = m_pProp->m_ConstrainProp[0].MinRangeValue; rep <= m_pProp->m_ConstrainProp[0].MaxRangeValue; rep++) {
					m_pPG_Active->SetRepititionSequence(rep);
					if (m_pPG_Active->GetVarValue(PGindex, act_rec + pg_control.StartIndex) > -1.E38) break;
				}
			}
			else if(!m_ChartInfo.ResidualSource&& p_info.DataType == DATA_TYPES::SIM_VAL){
				AA[icurve][i] =Simulated_SingleRunValue[i];
				continue;
	
			}


			


			if (m_pPG_Active->GetVarValue(PGindex, act_rec + pg_control.StartIndex, act_rep) < -1.E38&&!Mean_Of_Repititions) {
				SetDataValueMissing(icurve, pg_control, i);
				if (p_info.EnsembleType > ENSEMBLE_TYPE::NO) SetDataBandValueMissing(i_band, pg_control, i);
			}
			else {
				if (p_info.EnsembleType == ENSEMBLE_TYPE::ALL && !m_ChartInfo.ResidualSource) {
					SetAllRangeValue(icurve, pg_control, i, PGindex, p_info.EnsembleNo);
					SetAllRangeBandValue(i_band, pg_control, i, PGindex, p_info.EnsembleNo);
				}
				else if (p_info.EnsembleType == ENSEMBLE_TYPE::ACCEPTED && !m_ChartInfo.ResidualSource) {
					SetAcceptedRangeValue(icurve, pg_control, act_rec, PGindex, p_info.EnsembleNo);
					SetAcceptedRangeBandValue(i_band, pg_control, act_rec, PGindex, p_info.EnsembleNo);
				}
				else {
					act_rec = i+ pg_control.StartIndex-1;
					if(Mean_Of_Repititions)
						AA[icurve][i] = Mean_Of_Measured_Value[i];
					else
						AA[icurve][i + pg_control.YearStartIndex] = GetDataFromPGFile(pg_control, i , PGindex,act_rep);
					if (p_info.EnsembleType > ENSEMBLE_TYPE::NO) {
						if (m_ChartInfo.ResidualSource) {
							if (abs(AA[icurve][i]) < 1.E38) {
								BB_MIN[i_band][i + pg_control.YearStartIndex] = AA[icurve][i + pg_control.YearStartIndex];
								BB_MAX[i_band][i + pg_control.YearStartIndex] = AA[icurve][i + pg_control.YearStartIndex];
								BB_P10[i_band][i + pg_control.YearStartIndex] = AA[icurve][i + pg_control.YearStartIndex];
								BB_P90[i_band][i + pg_control.YearStartIndex] = AA[icurve][i + pg_control.YearStartIndex];
								BB_P50[i_band][i + pg_control.YearStartIndex] = AA[icurve][i + pg_control.YearStartIndex];
								BB[i_band][i + pg_control.YearStartIndex] = AA[icurve][i + pg_control.YearStartIndex];
								if (m_pUtil->m_ResidualPlot == 0&&Mean_Of_Repititions) {
									AA[icurve][i + pg_control.YearStartIndex]= Mean_Of_Measured_Value[i]- Mean_Of_Mean_ResidualsValue[size_t(STAT_INDICATOR::MEAN_VALUE)][i]; 
									BB[i_band][i + pg_control.YearStartIndex] = AA[icurve][i + pg_control.YearStartIndex];
									BB_MIN[i_band][i + pg_control.YearStartIndex] = Mean_Of_Measured_Value[i] - Mean_Of_Mean_ResidualsValue[size_t(STAT_INDICATOR::MIN_VALUE)][i];
									BB_MAX[i_band][i + pg_control.YearStartIndex] = Mean_Of_Measured_Value[i] - Mean_Of_Mean_ResidualsValue[size_t(STAT_INDICATOR::MAX_VALUE)][i];
									BB_P10[i_band][i + pg_control.YearStartIndex] = Mean_Of_Measured_Value[i] - Mean_Of_Mean_ResidualsValue[size_t(STAT_INDICATOR::P10_VALUE)][i];
									BB_P90[i_band][i + pg_control.YearStartIndex] = Mean_Of_Measured_Value[i] - Mean_Of_Mean_ResidualsValue[size_t(STAT_INDICATOR::P90_VALUE)][i];
									BB_P50[i_band][i + pg_control.YearStartIndex] = Mean_Of_Measured_Value[i] - Mean_Of_Mean_ResidualsValue[size_t(STAT_INDICATOR::P50_VALUE)][i];

								}
								else if (m_pUtil->m_ResidualPlot == 1&&Mean_Of_Repititions) {
									AA[icurve][i + pg_control.YearStartIndex] = Mean_Of_Mean_ResidualsValue[size_t(STAT_INDICATOR::MEAN_VALUE)][i];
									BB[i_band][i + pg_control.YearStartIndex] = AA[icurve][i + pg_control.YearStartIndex];
									BB_MIN[i_band][i + pg_control.YearStartIndex] =Mean_Of_Mean_ResidualsValue[size_t(STAT_INDICATOR::MIN_VALUE)][i];
									BB_MAX[i_band][i + pg_control.YearStartIndex] =Mean_Of_Mean_ResidualsValue[size_t(STAT_INDICATOR::MAX_VALUE)][i];
									BB_P10[i_band][i + pg_control.YearStartIndex] = Mean_Of_Mean_ResidualsValue[size_t(STAT_INDICATOR::P10_VALUE)][i];
									BB_P90[i_band][i + pg_control.YearStartIndex] = Mean_Of_Mean_ResidualsValue[size_t(STAT_INDICATOR::P90_VALUE)][i];
									BB_P50[i_band][i + pg_control.YearStartIndex] = Mean_Of_Mean_ResidualsValue[size_t(STAT_INDICATOR::P50_VALUE)][i];
								}
								else {
									AdjustForMeanOfResiduals(AA[icurve][i + pg_control.YearStartIndex], c_info.pRes, c_info.MeanOfAll, Residual_VarNum, LogLiVersion, i + pg_control.StartIndex + Start_SubSet_Residuals, 0);
									AdjustForMeanOfResiduals(BB[i_band][i + pg_control.YearStartIndex], c_info.pRes, c_info.MeanOfAll, Residual_VarNum, LogLiVersion, i + pg_control.StartIndex + Start_SubSet_Residuals, 1);
									AdjustForMeanOfResiduals(BB_MIN[i_band][i + pg_control.YearStartIndex], c_info.pRes, c_info.MeanOfAll, Residual_VarNum, LogLiVersion, i + pg_control.StartIndex + Start_SubSet_Residuals, 2);
									AdjustForMeanOfResiduals(BB_MAX[i_band][i + pg_control.YearStartIndex], c_info.pRes, c_info.MeanOfAll, Residual_VarNum, LogLiVersion, i + pg_control.StartIndex + Start_SubSet_Residuals, 3);
									AdjustForMeanOfResiduals(BB_P10[i_band][i + pg_control.YearStartIndex], c_info.pRes, c_info.MeanOfAll, Residual_VarNum, LogLiVersion, i + pg_control.StartIndex + Start_SubSet_Residuals, 4);
									AdjustForMeanOfResiduals(BB_P90[i_band][i + pg_control.YearStartIndex], c_info.pRes, c_info.MeanOfAll, Residual_VarNum, LogLiVersion, i + pg_control.StartIndex + Start_SubSet_Residuals, 5);
									AdjustForMeanOfResiduals(BB_P50[i_band][i + pg_control.YearStartIndex], c_info.pRes, c_info.MeanOfAll, Residual_VarNum, LogLiVersion, i + pg_control.StartIndex + Start_SubSet_Residuals, 6);
								}
							}
							else {
								SetDataBandValueMissing(i_band, pg_control, i);
							
							}
						}
						else {

							if (p_info.EnsembleType == ENSEMBLE_TYPE::ALL) {
								SetAllRangeValue(icurve, pg_control, i, PGindex, p_info.EnsembleNo);
								SetAllRangeBandValue(i_band, pg_control, i, PGindex, p_info.EnsembleNo);
							}
							else if (p_info.EnsembleType == ENSEMBLE_TYPE::ACCEPTED) {
								SetAcceptedRangeValue(icurve, pg_control, i, PGindex, p_info.EnsembleNo);
								SetAcceptedRangeBandValue(i_band, pg_control, i, PGindex, p_info.EnsembleNo);
							}
						}
					}
					else if (m_ChartInfo.ResidualSource && c_info.ResRunNo != 120000 && c_info.ResRunNo <= 100000 && datatype != DATA_TYPES::MEASURED_VAL) {
						if (allrep) {
							auto resno = c_info.ResRunNo + act_rep - 1;
							AdjustForResiduals(AA[icurve][i + pg_control.YearStartIndex], c_info.pRes, resno, Residual_VarNum, LogLiVersion, act_rec + pg_control.StartIndex - 1, c_info.MeasuredAsSource);
						}
						else if(Mean_Of_Repititions){
							AdjustForResiduals(AA[icurve][i + pg_control.YearStartIndex], ActiveResiduals[i + pg_control.YearStartIndex], c_info.ResRunNo, Residual_VarNum, LogLiVersion, act_rec + pg_control.StartIndex - 1, c_info.MeasuredAsSource);

						}
						else {
							AdjustForResiduals(AA[icurve][i + pg_control.YearStartIndex], c_info.pRes, c_info.ResRunNo, Residual_VarNum, LogLiVersion, act_rec + pg_control.StartIndex - 1, c_info.MeasuredAsSource);
						}
					}
					else if (!m_ChartInfo.ResidualSource&&datatype == DATA_TYPES::SIM_VAL) {
						if (m_pUtil->m_ResidualPlot == 1) {
							auto Meas = ActiveResiduals[i + pg_control.StartIndex + pg_control.YearStartIndex];// p_info.pPG_MeasValFile->GetVarValue(p_info.Residual_VarNum, i + pg_control.StartIndex + pg_control.YearStartIndex);
							AA[icurve][i + pg_control.YearStartIndex] -= Meas;
						}
						else {
							koll += i;

						}
					}



				}

//			}
		}
	}
	
	if (p_info.EnsembleType>ENSEMBLE_TYPE::NO && pResAct != nullptr) {
		save_ensemble_no = m_pSimDoc->m_MStorage.m_ActualEnsemble;
		// Set the correct ensemble using number....
		pResAct->SetNewEnsembleByNumber(save_ensemble_no);



	}
	return true;




}
void LoadDataToChart::SetMeanDataValues(size_t ActNoPoints, PG_TIMECONTROL pg_control, size_t icurve, size_t i_band, CURVEINFO c_info) {

	int j = m_pSimDoc->m_PlotStorage.GetArrayIndex(m_pProp->m_ChartNumber, icurve);
	int PGindex = m_pSimDoc->m_PlotStorage.GetVar_Index(j);
	int Residual_VarNum = m_pSimDoc->m_PlotStorage.GetValVarNo(j);
	int Residual_RunNo = m_pSimDoc->m_PlotStorage.GetValRunNo(j);
	int LogLiVersion = m_pSimDoc->m_PlotStorage.GetLogLiVersion(j);
	int PGSection = m_pSimDoc->m_PlotStorage.GetPGSectionNo(j);

	PLOTv p_info = m_pSimDoc->m_PlotStorage.GetPlotInfo(j);
	PGindex = p_info.PGFileIndex;

	if (Residual_RunNo == 110001 && PGindex>1 && pg_control.YearBand&&pg_control.MeanValueBand) PGindex--;

	for (int i = 0; i<ActNoPoints; i++) {
		float fvalue, fv2 = float(MISSING);
		if (pg_control.YearBand) {

			fvalue = m_pPG_Active->GetVarValue_WithinYearMean(PGindex, i + pg_control.StartIndex);

			fv2 = m_pPG_Active->GetVarValue_WithinYearSD(PGindex, i + pg_control.StartIndex);
			if (fv2>-1.E38)
				BB[i_band][i + pg_control.YearStartIndex] = double(fv2);
			else
				BB[i_band][i + pg_control.YearStartIndex] = Chart::NoValue;

			fv2 = m_pPG_Active->GetVarValue_WithinYearMin(PGindex, i + pg_control.StartIndex);
			if (fv2>-1.E38)
				BB_MIN[i_band][i + pg_control.YearStartIndex] = double(fv2);
			else
				BB_MIN[i_band][i + pg_control.YearStartIndex] = Chart::NoValue;

			fv2 = m_pPG_Active->GetVarValue_WithinYearMax(PGindex, i + pg_control.StartIndex);
			if (fv2>-1.E38)
				BB_MAX[i_band][i + pg_control.YearStartIndex] = double(fv2);
			else
				BB_MAX[i_band][i + pg_control.YearStartIndex] = Chart::NoValue;
			fv2 = m_pPG_Active->GetVarValue_WithinYearP5(PGindex, i + pg_control.StartIndex);
			if (fv2>-1.E38)
				BB_P10[i_band][i + pg_control.YearStartIndex] = double(fv2);
			else
				BB_P10[i_band][i + pg_control.YearStartIndex] = Chart::NoValue;
			fv2 = m_pPG_Active->GetVarValue_WithinYearP50(PGindex, i + pg_control.StartIndex);
			if (fv2>-1.E38)
				BB_P50[i_band][i + pg_control.YearStartIndex] = double(fv2);
			else
				BB_P50[i_band][i + pg_control.YearStartIndex] = Chart::NoValue;
			fv2 = m_pPG_Active->GetVarValue_WithinYearP95(PGindex, i + pg_control.StartIndex);
			if (fv2>-1.E38)
				BB_P90[i_band][i + pg_control.YearStartIndex] = double(fv2);
			else
				BB_P90[i_band][i + pg_control.YearStartIndex] = Chart::NoValue;

			// Section to account for information from Residual File

		}
		else {
			fvalue = m_pPG_Active->GetVarValue_WithinDayMean(PGindex, i + 1, PGSection - 6000);
		}
		if (fvalue<-.9E38) {
			AA[icurve][i + pg_control.YearStartIndex] = Chart::NoValue;
			if (m_ChartInfo.NumVariableBands>0) {
				BB_P10[i_band][i + pg_control.YearStartIndex] = Chart::NoValue;
				BB_P90[i_band][i + pg_control.YearStartIndex] = Chart::NoValue;
			}
		}
		else {
			AA[icurve][i + pg_control.YearStartIndex] = double(fvalue);
		}

		if (m_ChartInfo.ResidualSource && !pg_control.MeanValueBand) {
			AdjustForMeanOfResidualsBetweenYear(AA[icurve][i + pg_control.YearStartIndex], c_info.pRes, true, Residual_VarNum, LogLiVersion, i + pg_control.StartIndex, 0);
			AdjustForMeanOfResidualsBetweenYear(BB[i_band][i + pg_control.YearStartIndex], c_info.pRes, true, Residual_VarNum, LogLiVersion, i + pg_control.StartIndex, 1);
			AdjustForMeanOfResidualsBetweenYear(BB_MIN[i_band][i + pg_control.YearStartIndex], c_info.pRes, true, Residual_VarNum, LogLiVersion, i + pg_control.StartIndex, 2);
			AdjustForMeanOfResidualsBetweenYear(BB_MAX[i_band][i + pg_control.YearStartIndex], c_info.pRes, true, Residual_VarNum, LogLiVersion, i + pg_control.StartIndex, 3);
			AdjustForMeanOfResidualsBetweenYear(BB_P10[i_band][i + pg_control.YearStartIndex], c_info.pRes, true, Residual_VarNum, LogLiVersion, i + pg_control.StartIndex, 4);
			AdjustForMeanOfResidualsBetweenYear(BB_P90[i_band][i + pg_control.YearStartIndex], c_info.pRes, true, Residual_VarNum, LogLiVersion, i + pg_control.StartIndex, 5);
		}

	}


}
void LoadDataToChart::SetDataValueMissing(size_t icurve, PG_TIMECONTROL pg_control, size_t i) {
	AA[icurve][i + pg_control.YearStartIndex] = Chart::NoValue;
}
void LoadDataToChart::SetDataBandValueMissing(size_t i_band, PG_TIMECONTROL pg_control, size_t i) {
	BB[i_band][i + pg_control.YearStartIndex] = Chart::NoValue;
	BB_MIN[i_band][i + pg_control.YearStartIndex] = Chart::NoValue;
	BB_MAX[i_band][i + pg_control.YearStartIndex] = Chart::NoValue;
	BB_P10[i_band][i] = Chart::NoValue;
	BB_P50[i_band][i] = Chart::NoValue;
	BB_P90[i_band][i] = Chart::NoValue;
}
void LoadDataToChart::SetAllRangeValue(size_t icurve, PG_TIMECONTROL pg_control, size_t i, int PGindex, size_t EnsembleNo) {
	AA[icurve][i + pg_control.YearStartIndex] = double(m_pPG_Active->GetVarValue_Ensemble(DistPos::Mean, PGindex, i + pg_control.StartIndex, true));
}
void LoadDataToChart::SetAllRangeBandValue(size_t i_band, PG_TIMECONTROL pg_control, size_t i, int PGindex, size_t EnsembleNo) {
	bool all = true;
	BB[i_band][i + pg_control.YearStartIndex] = double(m_pPG_Active->GetVarValue_Ensemble(DistPos::Std, PGindex, i + pg_control.StartIndex, all, EnsembleNo));
	BB_MIN[i_band][i + pg_control.YearStartIndex] = double(m_pPG_Active->GetVarValue_Ensemble(DistPos::Min, PGindex, i + pg_control.StartIndex, all, EnsembleNo));
	BB_MAX[i_band][i + pg_control.YearStartIndex] = double(m_pPG_Active->GetVarValue_Ensemble(DistPos::Max, PGindex, i + pg_control.StartIndex, all, EnsembleNo));
	BB_P10[i_band][i + pg_control.YearStartIndex] = double(m_pPG_Active->GetVarValue_Ensemble(DistPos::P5, PGindex, i + pg_control.StartIndex, all, EnsembleNo));
	BB_P90[i_band][i + pg_control.YearStartIndex] = double(m_pPG_Active->GetVarValue_Ensemble(DistPos::P95,PGindex, i + pg_control.StartIndex, all, EnsembleNo));
	BB_P50[i_band][i + pg_control.YearStartIndex] = double(m_pPG_Active->GetVarValue_Ensemble(DistPos::P50,PGindex, i + pg_control.StartIndex, all, EnsembleNo));
}
void LoadDataToChart::SetAcceptedRangeValue(size_t icurve, PG_TIMECONTROL pg_control, size_t i, int PGindex, size_t EnsembleNo) {
	AA[icurve][i + pg_control.YearStartIndex] = double(m_pPG_Active->GetVarValue_Ensemble(DistPos::Mean, PGindex, i + pg_control.StartIndex, false, EnsembleNo));
}
void LoadDataToChart::SetAcceptedRangeBandValue(size_t i_band, PG_TIMECONTROL pg_control, size_t i, int PGindex, size_t EnsembleNo) {
	bool all = false;
	BB[i_band][i + pg_control.YearStartIndex] = double(m_pPG_Active->GetVarValue_Ensemble(DistPos::Std, PGindex, i + pg_control.StartIndex, all, EnsembleNo));
	BB_MIN[i_band][i + pg_control.YearStartIndex] = double(m_pPG_Active->GetVarValue_Ensemble(DistPos::Min, PGindex, i + pg_control.StartIndex, all, EnsembleNo));
	BB_MAX[i_band][i + pg_control.YearStartIndex] = double(m_pPG_Active->GetVarValue_Ensemble(DistPos::Max, PGindex, i + pg_control.StartIndex, all, EnsembleNo));
	BB_P10[i_band][i + pg_control.YearStartIndex] = double(m_pPG_Active->GetVarValue_Ensemble(DistPos::P5, PGindex, i + pg_control.StartIndex, all, EnsembleNo));
	BB_P90[i_band][i + pg_control.YearStartIndex] = double(m_pPG_Active->GetVarValue_Ensemble(DistPos::P95, PGindex, i + pg_control.StartIndex, all, EnsembleNo));
	BB_P50[i_band][i + pg_control.YearStartIndex] = double(m_pPG_Active->GetVarValue_Ensemble(DistPos::P50, PGindex, i + pg_control.StartIndex, all, EnsembleNo));
}
void LoadDataToChart::SetDataValue(size_t icurve, PG_TIMECONTROL pg_control, size_t i, int PGindex) {
	double sum;
	if (m_ChartInfo.WithinDayRepresentation == 1) {
		AA[icurve][i + pg_control.YearStartIndex] = double(m_pPG_Active->GetVarValue(PGindex, i + pg_control.StartIndex));
		return;
	}
	else {
		if (m_pProp->m_HourValueRepresentation == 0) {
			sum = 0.;
			for (size_t ii = 0; ii<m_ChartInfo.WithinDayRepresentation; ii++) sum += m_pPG_Active->GetVarValue(PGindex, i*m_ChartInfo.WithinDayRepresentation + pg_control.StartIndex + ii);
			AA[icurve][i + pg_control.YearStartIndex] = sum / m_ChartInfo.WithinDayRepresentation;
		}
		else
			AA[icurve][i + pg_control.YearStartIndex] = m_pPG_Active->GetVarValue(PGindex, i*m_ChartInfo.WithinDayRepresentation + pg_control.StartIndex + m_pProp->m_HourValueRepresentation - 1);

	}
}
double LoadDataToChart::GetDataFromPGFile(PG_TIMECONTROL pg_control, size_t i, int PGindex, size_t act_rep) {
	if (m_ChartInfo.WithinDayRepresentation == 1 && m_pProp->GetIndepSecond() != PGHOUR)
		return double(m_pPG_Active->GetVarValue(PGindex, i + pg_control.StartIndex,act_rep));
	else if (m_pProp->GetIndepSecond() == PGHOUR&&m_pPG_Active->GetNumberofYears()>1) {

		return double(m_pPG_Active->GetVarValue(PGindex, i + pg_control.StartIndex + pg_control.YearStartIndex, act_rep));




	}

	else if (m_pProp->m_HourValueRepresentation == 0) {
		double sum = 0.;
		for (size_t ii = 0; ii<m_ChartInfo.WithinDayRepresentation; ii++) sum += m_pPG_Active->GetVarValue(PGindex, i*m_ChartInfo.WithinDayRepresentation + pg_control.StartIndex + ii, act_rep);
		return sum / m_ChartInfo.WithinDayRepresentation;
	}
	else
		return m_pPG_Active->GetVarValue(PGindex, i*m_ChartInfo.WithinDayRepresentation + pg_control.StartIndex + m_pProp->m_HourValueRepresentation - 1, act_rep);


}

void LoadDataToChart::SetTimeDataMinMax(int Type) {
	if (m_pProp->m_ReShape.m_CommonDayNo) {
		m_Time.MinDate = m_pPG_Active->GetDoubleTimeYearStart(1);
		m_Time.MaxDate = m_pPG_Active->GetDoubleTimeYearEnd(1);
		if (!m_pProp->isXScaleSet(0) && m_pProp->GetFirstIndepIndex() == string::npos&&Type != 18) {
			m_pProp->m_TimeScale.min = m_Time.MinDate;
			m_pProp->m_TimeScale.max = m_Time.MaxDate;
		}
	}
	else if (m_pProp->m_ReShape.m_CommonWithDay) {
		m_Time.MinDate = m_pPG_Active->GetDoubleTimeYearStart(1);
		m_Time.MaxDate = m_Time.MinDate + 86400.;
		if (!m_pProp->isXScaleSet(0) && m_pProp->GetFirstIndepIndex() == string::npos && !m_pProp->IsFirstIndepSet()) {
			m_pProp->m_TimeScale.min = m_Time.MinDate;
			m_pProp->m_TimeScale.max = m_Time.MaxDate;
		}
	}
	else if (m_pProp->GetIndepFirst() == DATEAXIS || (m_pProp->GetIndepFirst() == NO_USE)) {
		m_Time.MinDate = m_pPG_Active->GetDoubleTimeStart();
		m_Time.MaxDate = m_pPG_Active->GetDoubleTimeEnd();
	}


}
void LoadDataToChart::ReSizeBandDataVectors(size_t i_band, size_t ActNoPoints) {

	if (BB[i_band].size()<ActNoPoints) BB[i_band].resize(ActNoPoints);
	if (BB_MIN[i_band].size()<ActNoPoints) BB_MIN[i_band].resize(ActNoPoints);
	if (BB_MAX[i_band].size()<ActNoPoints) BB_MAX[i_band].resize(ActNoPoints);
	if (BB_P10[i_band].size()<ActNoPoints) BB_P10[i_band].resize(ActNoPoints);
	if (BB_P90[i_band].size()<ActNoPoints) BB_P90[i_band].resize(ActNoPoints);
	if (BB_P50[i_band].size()<ActNoPoints) BB_P50[i_band].resize(ActNoPoints);
}


bool LoadDataToChart::SetIndependentMeasuredValidVar(PLOTv p_info, CURVEINFO c_info, size_t icurve) {

	F *pF = p_info.pSimDoc->ValidationFilePointer(p_info.ValFileNo);
	pF->CheckFileNameAndAssignNameToPGClass();
	pF->CallDataDescription();
	CPG *pPG_Measured;
	if (p_info.pSimDoc->m_FileVersionNumberRead > 60)
		pPG_Measured = pF->GetPointer();
	else
		return false;
	int PGFileMeasuredIndex = p_info.pSimDoc->m_Val_Array[p_info.Index].ValidationFileIndex + 1;


	m_Time.MinDate = double(pPG_Measured->GetVarMin(PGFileMeasuredIndex)); m_Time.MaxDate = double(pPG_Measured->GetVarMax(PGFileMeasuredIndex));
	double val;
	pPG_Measured->ReOpen();
	pPG_Measured->CheckAndEstimateRepetions();
	pPG_Measured->SetRepititionMode(true);
	// Check number of repitions

	auto numrep=pPG_Measured->GetNumRepititions();


	for (int i = 0; i < m_ChartInfo.NumOfPoints; i++) {
		auto rec = i%pPG_Measured->GetNumRecords();
		auto irep = i / pPG_Measured->GetNumRecords();
		if (pPG_Measured->SetRepititionSequence(irep)) {
			val = pPG_Measured->GetVarValue(PGFileMeasuredIndex, rec + 1);
			if (val == MISSING) val = Chart::NoValue;
		}
		else
			val = Chart::NoValue;



		auto val= TT_Indep[icurve][i];

		if(val== MISSING) val = Chart::NoValue;
		TT_Indep[icurve][i] = val;


		//TT_Indep[icurve][i] = val;
	}
		//pPG_Measured->GetVarValue(PGFileMeasuredIndex, i + 1);

	m_pProp->SetFirstIndepIndex(PGVARVALID, PGFileMeasuredIndex, icurve);
	m_pProp->SetChartType(0);
	m_pProp->SetFirstIndepSet(PGVARVALID, true, icurve);
	string str;
	str = pPG_Measured->GetVarName(PGFileMeasuredIndex);
	FUtil::trim(str);
	string stru;
	stru = pPG_Measured->GetVarUnit(PGFileMeasuredIndex);
	FUtil::trim(stru);
	str += " (" + stru + ")";
	m_pProp->SetFirstIndepString(PGVARVALID, str, icurve);

	return true;
}
bool LoadDataToChart::SetIndependentSingleTime(PLOTv p_info) {
	bool check;
	check = false;
	int index;
	size_t minut;
	size_t countextra;

	index = m_pProp->GetFirstIndepIndex();
	auto koll = m_pProp->GetIndepFirst();


	switch (m_pProp->GetIndepFirst()) {

	case NO_USE:
	case DATEAXIS:
		m_Time.MinDate = TT[0];
		m_Time.MaxDate = TT[m_ChartInfo.NumOfPoints - 1];
		check = true;
		m_Contour.FirstMin = m_Time.MinDate;
		m_Contour.FirstMax = m_Time.MaxDate;

		break;

	case PGVAR:
		m_Time.MinDate = double(m_pPG_Active->GetVarMin(index)); m_Time.MaxDate = double(m_pPG_Active->GetVarMax(index));

		for (int i = 0; i < m_ChartInfo.NumOfPoints; i++) {
			TT[i] = m_pPG_Active->GetVarValue(index, i*m_ChartInfo.WithinDayRepresentation + 1);
			if (TT[i] <= 0.9*MISSING) TT[i] = Chart::NoValue;
		}
		if (m_pProp->m_ReShape.m_Cumulated > 0) {
			m_pProp->MakeCumTranformation(&TT);
			m_Time.MinDate = TT[0];
			m_Time.MaxDate = TT[m_ChartInfo.NumOfPoints-1];
		}

		m_Contour.FirstMin = m_Time.MinDate;
		m_Contour.FirstMax = m_Time.MaxDate;
		m_Contour.FirstInterval = m_pUtil->GetScaleInterval(m_Contour.FirstMax, m_Contour.FirstMin);


		check = true;
		break;
	case PGVARVALID:
		// Return to time serie mode
		m_pProp->SetChartType(1);
		m_pProp->SetFirstIndepSet(PGVARVALID, false);
		check = true;
		break;
	case PGDEPTH:
		check = true;
		break;
	case PGDAY:
		check = true;
		m_Time.MinDate = 366;
		m_Time.MaxDate = 1;
		countextra = 0;
		for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {
			//if((i+1)%m_ChartInfo.NumExtraPointFreq==0) countextra++;
			minut = m_pPG_Active->GetLongTime(i*m_ChartInfo.WithinDayRepresentation + 1 - countextra);
			TT[i] = int(PGUtil::DayNumFunction(minut));

			if (TT[i]>m_Time.MaxDate) m_Time.MaxDate = TT[i];
			if (TT[i]<m_Time.MinDate) m_Time.MinDate = TT[i];
		}
		m_Contour.FirstMin = m_Time.MinDate;
		m_Contour.FirstMax = m_Time.MaxDate;
		m_Contour.FirstInterval = 30.5;

		break;
	case PGHOUR:
		check = true;
		m_Time.MinDate = 0;
		m_Time.MaxDate = 24;
		m_Contour.FirstMin = 0;
		m_Contour.FirstMax = 24;
		m_Contour.FirstInterval = 2.;

		for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {
			minut = m_pPG_Active->GetLongTime(i*m_ChartInfo.WithinDayRepresentation + 1);
			TT[i] = minut % 1440 / 60;
		}
		break;
	case PGYEAR:
		check = true;
		m_Time.MinDate = 5000;
		m_Time.MaxDate = 1;
		for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {
			minut = m_pPG_Active->GetLongTime(i*m_ChartInfo.WithinDayRepresentation + 1);
			TT[i] = PGUtil::YearFunction(minut);
			if (TT[i]>m_Time.MaxDate) m_Time.MaxDate = TT[i];
			if (TT[i]<m_Time.MinDate) m_Time.MinDate = TT[i];
		}
		m_Contour.FirstMin = m_Time.MinDate;
		m_Contour.FirstMax = m_Time.MaxDate;
		m_Contour.FirstInterval = m_pUtil->GetScaleInterval(m_Contour.FirstMax, m_Contour.FirstMin);



		break;




	}
	if (m_pProp->IsSecondIndepSet()) {
		index = m_pProp->GetSecondIndepIndex();
		switch (m_pProp->GetIndepSecond()) {

		case PGVAR:
			m_Time.MinDate = double(m_pPG_Active->GetVarMin(index)); m_Time.MaxDate = double(m_pPG_Active->GetVarMax(index));
			m_Contour.SecondMax = MISSING;
			m_Contour.SecondMin = abs(MISSING);

			for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {
				TT2[i] = m_pPG_Active->GetVarValue(index*m_ChartInfo.WithinDayRepresentation, i + 1);
				if (TT2[i] >= .8*MISSING) {
					if (TT2[i] > m_Contour.SecondMax) 	m_Contour.SecondMax = TT2[i];
					if (TT2[i] < m_Contour.SecondMin) 	m_Contour.SecondMin = TT2[i];
				}
				else
					TT2[i] = Chart::NoValue;
			}
			m_Contour.SecondInterval = m_pUtil->GetScaleInterval(m_Contour.SecondMax, m_Contour.SecondMin);

			break;
		case PGDAY:
			m_Time.MinDate = 1;
			m_Contour.SecondMax = MISSING;
			m_Contour.SecondMin = abs(MISSING);
			for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {
				minut = m_pPG_Active->GetLongTime(i*m_ChartInfo.WithinDayRepresentation + 1);
				TT2[i] = PGUtil::DayNumFunction(minut);
				if (TT2[i]>m_Contour.SecondMax) 	m_Contour.SecondMax = TT2[i];
				if (TT2[i]<m_Contour.SecondMin) 	m_Contour.SecondMin = TT2[i];
			}
			m_Contour.SecondInterval = 30.5;
			break;
		case PGHOUR:
			m_Time.MinDate = 0;
			m_Time.MaxDate = 24;
			m_Contour.SecondMax = 0.;
			m_Contour.SecondMin = 24.;
			countextra = 0;
			for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {
				if ((i + 1) % m_ChartInfo.NumExtraPointFreq == 0) {
					countextra++;
					minut = m_pPG_Active->GetLongTime(i*m_ChartInfo.WithinDayRepresentation + 1 - countextra);
					minut += m_pPG_Active->GetNormalTimeInterval();
					TT2[i] = double(m_ChartInfo.WithinDayRepresentation);
				}
				else {
					minut = m_pPG_Active->GetLongTime(i*m_ChartInfo.WithinDayRepresentation + 1 - countextra);
					TT2[i] = minut % 1440 / m_pPG_Active->GetNormalTimeInterval();
				}
				if (TT2[i]<m_Contour.SecondMin) m_Contour.SecondMin = TT2[i];
				if (TT2[i]>m_Contour.SecondMax) m_Contour.SecondMax = TT2[i];
			}
			m_Contour.SecondInterval = m_pUtil->GetScaleInterval(m_Contour.SecondMax, m_Contour.SecondMin);

			break;
		case PGYEAR:
			m_Time.MinDate = 5000;
			m_Time.MaxDate = 1;
			m_Contour.SecondMax = 1;
			m_Contour.SecondMin = 5000;
			for (int i = 0; i<m_ChartInfo.NumOfPoints; i++) {
				minut = m_pPG_Active->GetLongTime(i*m_ChartInfo.WithinDayRepresentation + 1);
				TT2[i] = PGUtil::YearFunction(minut);
				if (TT2[i]>m_Time.MaxDate) m_Time.MaxDate = TT2[i];
				if (TT2[i]<m_Time.MinDate) m_Time.MinDate = TT2[i];
				if (TT2[i]>m_Contour.SecondMax) m_Contour.SecondMax = TT2[i];
				if (TT2[i]<m_Contour.SecondMin) m_Contour.SecondMin = TT2[i];

			}
			m_Contour.SecondInterval = m_pUtil->GetScaleInterval(m_Contour.SecondMax, m_Contour.SecondMin);

			break;
		case PGDEPTH:

			auto value = FUtil::AtoFloat(m_pPG_Active->GetVarPos(index));
			if (value > m_Contour.SecondMax) 	m_Contour.SecondMax = value;
			if (value < m_Contour.SecondMin) 	m_Contour.SecondMin = value;

			if (FUtil::AtoFloat(m_pPG_Active->GetVarPos(index))>MISSING)
				check = true;
			else
				check = false;
			break;
		}
	}
	return check;
}




