
#include "ChartBase.h"
#include "../Util.h"
#include "./ChartProperties\ChartProp.h"
#include "../NewBase\Doc.h"
#include "./PlotDesignInfo\PlotStorage.h"
#include "../Cug\ChartProperties\ChartCug.h"

ChartBase::ChartBase(void)
{

}


ChartBase::~ChartBase(void)
{
}

void ChartBase::CheckIfRightAxis(enum class DATA_TYPES itype, int ivar, int curve_count) 
{
		if(itype==DATA_TYPES::SIM_VAL||itype==DATA_TYPES::MEASURED_VAL) {
			m_pProp->m_rightaxis=false;
		}
		else if(m_pProp->GetAxisLayer(ivar)<0) {
			FUtil::trim(m_Label.test);
			if(m_Label.ytitle1.find(m_Label.test)!=string::npos||m_Label.ytitle1==""||m_pProp->m_ReShape.m_CommonScale||m_pProp->m_ReShape.m_Cumulated||m_pProp->m_ReShape.m_DerivateValues)
				m_pProp->m_rightaxis=false;

			else if(m_Label.test==m_Label.ytitle2||(m_Label.ytitle2==""&&m_pProp->m_ChartType!= CHART_TYPES::TIME_SERIE&&curve_count>0)||(m_Label.ytitle2==""&&m_pProp->m_ChartType ==CHART_TYPES::SCATTER&&curve_count>1))
                m_pProp->m_rightaxis=true;
			else
				m_pProp->m_rightaxis=false;

		}
		else if(m_pProp->GetAxisLayer(ivar)==1||m_pProp->m_ReShape.m_CommonScale)
			m_pProp->m_rightaxis=false;
		else if(m_pProp->GetAxisLayer(ivar)==2&&m_pProp->m_ChartType ==CHART_TYPES::SCATTER)
			m_pProp->m_rightaxis=true;
		else
			m_pProp->m_rightaxis=true;

		if (m_pProp->GetChartTypeIndex() == CHART_TYPES::FREQUENCE || m_pProp->GetChartTypeIndex() == CHART_TYPES::CUM_FREQUENCE)
			m_pProp->m_rightaxis = false;

 }
void ChartBase::CheckAndAddLabelInfo(size_t ich, int idoc) {
				FUtil::trim(m_Label.test);
	 			size_t ipos=m_Label.ytitle1.find(m_Label.test);
				if(m_pProp->m_ChartType ==CHART_TYPES::FREQUENCE||m_pProp->m_ChartType ==CHART_TYPES::CUM_FREQUENCE) {
					ipos=m_Label.xtitle.find(m_Label.test);


				}
				if(ipos==string::npos) {
						//Label.test not in Title
					auto previous_id = m_Label.st;
					m_Label.st=m_Label.test;
					if (m_Label.st.find(previous_id) == string::npos) {
						m_Label.st += "," + previous_id;
   					}

				}
				if(m_pSimDoc->GetDocLinkedNumber()>1) {
					Doc *pDocAct;
					void *pD=m_pSimDoc->m_PlotStorage.GetDocPointer(idoc);
					if(pD!=nullptr)
						pDocAct=(Doc*)pD;
					else
						pDocAct=m_pSimDoc;

				if(m_pSimDoc->m_SelectedDocuments.size()>size_t(idoc))
					pDocAct=m_pSimDoc->m_SelectedDocuments[idoc];
				else
					pDocAct=m_pSimDoc;


					if(m_pUtil->GetLegendContentStyle()==0)
						m_Label.DocNumId=FUtil::STD_ItoAscii(pDocAct->m_DocFile.m_SimulationRunNo)+pDocAct->m_DocFile2.m_RunId;
					else if(m_pUtil->GetLegendContentStyle()==1)
						m_Label.DocNumId=FUtil::STD_ItoAscii(pDocAct->m_DocFile.m_SimulationRunNo);
					else
						m_Label.DocNumId=pDocAct->m_DocFile2.m_RunId;

					switch(m_pUtil->GetLegendContentStyle()) {
						case 0:
							m_Label.st+=" "+m_Label.DocNumId;
							break;
						case 1:
							break;
						case 2:
							m_Label.st=m_Label.DocNumId;
							break;
					}
				}	 
	 
}
void ChartBase::SetSymbolInfo(int ivar, int curve_count, bool CommonColors) {
	if(m_Symbol.isymb==0&&m_pProp->m_ChartType ==CHART_TYPES::SCATTER) {
		if(CommonColors) {
			m_pProp->SetSymbolStyle(curve_count, 1);
			m_pProp->SetSymbolSize(curve_count, 3);
		}
		else {
			m_pProp->SetSymbolStyle(ivar, 1);
			m_pProp->SetSymbolSize(ivar, 3);
		}

	}
}


void ChartBase::SetMultiSequenceInfo()
{
	if (m_pPG_Active != nullptr) {
		//SetMultiMode(true);  // Removed by PEJ to make table without extra 2 variables
		m_pProp->SetMultiSequence(m_pPG_Active->GetRepititionSequence());
		m_pProp->SetMultiRepititions(m_pSimDoc->MR_Get_TotalNumberofRuns());

	}

}

void ChartBase::SetPointertoActivePGFile(int Index_In_PlotArray, PLOTv p_info)
{
	int FileNo = p_info.ValFileNo;
	void *Ptr = m_pSimDoc->m_PlotStorage.GetPtr(Index_In_PlotArray);
	if (p_info.DataType == DATA_TYPES::PGDATA) {

		m_pPG_Active = p_info.pPG;
		if (m_pPG_Active == nullptr) {
			m_pPG_Active = p_info.pPG;
			m_pPG_Active = m_pSimDoc->m_PlotStorage.GetPGPointer(p_info.Doc);
		}
		m_pPG_Active->ReOpen();
	}
	else if (p_info.DataType == DATA_TYPES::ANYDATA) {
		m_pPG_Active = p_info.pPG;
		m_pPG_Active->ReOpen();

	}
	else if (p_info.RepitDataType) {
		//p_info.pPG;
		m_pPG_Active = m_pSimDoc->m_PlotStorage.GetPGPointer(p_info.Doc);
		if (m_pPG_Active == nullptr)
			m_pPG_Active = p_info.pPG;
	}
	else if (p_info.DataType == DATA_TYPES::SIM_VAL) {
		FileNo = p_info.ValFileNo;//m_pSimDoc->m_PlotStorage.GetValFileNo(j);
		m_pPG_Active = p_info.pPG;//m_pSimDoc->m_PlotStorage.GetPGValFilePointer(j);
		m_pPG_Active->ReOpen();
	}
	else {
		m_pPG_Active = p_info.pPG;

	}



}
