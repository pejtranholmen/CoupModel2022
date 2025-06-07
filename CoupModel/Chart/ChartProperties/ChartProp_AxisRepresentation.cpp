#pragma once
#include "./ChartProp_AxisRepresentation.h"



class SimB;
struct SIMB;
class CPG;

ChartProp_AxisRepresentation::ChartProp_AxisRepresentation(void)
{
	m_Indep.First= vartype::DATEAXIS;
	m_Indep.Second= vartype::NO_USE;
	m_MultiPar_dep.SecondIndepSet=false;
	m_MultiPG_dep.SecondIndepSet=false;
	m_ValidationPG_dep.resize(1);
	m_ValidationPG_dep[0].SecondIndepSet=false;
	m_MultiVal_dep.SecondIndepSet=false;
	m_MultiSum_dep.SecondIndepSet=false;
}


ChartProp_AxisRepresentation::~ChartProp_AxisRepresentation(void)
{
	
}

size_t ChartProp_AxisRepresentation::GetFirstIndepIndex(size_t index) {
	switch(m_Indep.First) {
	case vartype::PGVARVALID:
		return m_ValidationPG_dep[index].First_Index;
	case vartype::GEO_LONG:
	case vartype::PGVAR:
	case vartype::PGDAY:
	case vartype::PGYEAR:
	case vartype::PGHOUR:
		return m_MultiPG_dep.First_Index;
	case vartype::SUMVAR:
		return m_MultiSum_dep.First_Index;
	case vartype::VALVAR:
	case vartype::VALVARSUM:
		return m_MultiVal_dep.First_Index;
	case vartype::PARVAR:
		return m_MultiPar_dep.First_Index;
	case vartype::PGDEPTH:
		return m_MultiPG_dep.First_Index;

	}

	return std::string::npos;
}
size_t ChartProp_AxisRepresentation::GetSecondIndepIndex() {
	switch(m_Indep.Second) {
	case vartype::PGVARVALID:
		return m_ValidationPG_dep[0].Second_Index;
	case vartype::GEO_LAT:
	case vartype::GEO_LONG:
	case vartype::PGVAR:
		return m_MultiPG_dep.Second_Index;
	case vartype::SUMVAR:
		return m_MultiSum_dep.Second_Index;
	case vartype::VALVAR:
	case vartype::VALVARSUM:
		return m_MultiVal_dep.Second_Index;
	case vartype::PARVAR:
		return m_MultiPar_dep.Second_Index;
	}
	return std::string::npos;
}
bool ChartProp_AxisRepresentation::IsFirstIndepSet(size_t index) {
	switch(m_Indep.First) {
	case vartype::DATEAXIS:
		return true;
	case vartype::PGVARVALID:
		return m_ValidationPG_dep[index].FirstIndepSet;
	case vartype::PGDAY:
	case vartype::PGHOUR:
	case vartype::PGYEAR:
	case vartype::PGVAR:
	case vartype::GEO_LONG:
	case vartype::GEO_LAT:
		return m_MultiPG_dep.FirstIndepSet;
	case vartype::SUMVAR:
		return m_MultiSum_dep.FirstIndepSet;
	case vartype::VALVAR:
	case vartype::VALVARSUM:
		return m_MultiVal_dep.FirstIndepSet;
	case vartype::PARVAR:
		return m_MultiPar_dep.FirstIndepSet;
	case vartype::PGDEPTH:
		return m_MultiPG_dep.FirstIndepSet;
	}
	return false;
}
bool ChartProp_AxisRepresentation::IsFirstIndepSet(enum vartype vartyp, size_t index) {
	switch(vartyp) {
	case vartype::PGVARVALID:
		return m_ValidationPG_dep[index].FirstIndepSet;
	case vartype::PGDAY:
	case vartype::PGHOUR:
	case vartype::PGYEAR:
	case vartype::PGVAR:
	case vartype::GEO_LONG:
	case vartype::GEO_LAT:
		return m_MultiPG_dep.FirstIndepSet;
	case vartype::SUMVAR:
		return m_MultiSum_dep.FirstIndepSet;
	case vartype::VALVAR:
	case vartype::VALVARSUM:
		return m_MultiVal_dep.FirstIndepSet;
	case vartype::PARVAR:
		return m_MultiPar_dep.FirstIndepSet;
	case vartype::PGDEPTH:
		return m_MultiPG_dep.FirstIndepSet;
	}
	return false;
}
bool ChartProp_AxisRepresentation::IsSecondIndepSet() {
	switch(m_Indep.Second) {
	case vartype::PGVARVALID:
		return m_ValidationPG_dep[0].SecondIndepSet;
	case vartype::PGDAY:
	case vartype::PGHOUR:
	case vartype::PGYEAR:
	case vartype::PGVAR:
	case vartype::GEO_LAT:
	case vartype::GEO_LONG:
	case vartype::PGDEPTH:
		return m_MultiPG_dep.SecondIndepSet;
	case vartype::SUMVAR:
		if(m_MultiSum_dep.SecondIndepSet)
		return m_MultiSum_dep.SecondIndepSet;
	case vartype::VALVAR:
	case vartype::VALVARSUM:
		if(m_MultiVal_dep.SecondIndepSet)
		return m_MultiVal_dep.SecondIndepSet;
	case vartype::PARVAR:
		return m_MultiPar_dep.SecondIndepSet;
	case vartype::DATEAXIS:
		return true;
	}
	return false;
}
bool ChartProp_AxisRepresentation::IsSecondIndepSet(enum vartype vartyp) {
	switch(vartyp) {
	case vartype::PGVARVALID:
		return m_ValidationPG_dep[0].SecondIndepSet;
	case vartype::PGDAY:
	case vartype::PGHOUR:
	case vartype::PGYEAR:
	case vartype::PGVAR:
		return m_MultiPG_dep.SecondIndepSet;
	case vartype::SUMVAR:
		return m_MultiSum_dep.SecondIndepSet;
	case vartype::VALVAR:
		return m_MultiVal_dep.SecondIndepSet;
	case vartype::PARVAR:
		return m_MultiPar_dep.SecondIndepSet;
	}
	return false;
}
std::string ChartProp_AxisRepresentation::GetFirstIndepString(size_t index) {
	switch(m_Indep.First) {
	case vartype::PGVARVALID:
		return m_ValidationPG_dep[index].FirstStr;
	case vartype::PGVAR:
	case vartype::PGDAY:
	case vartype::PGHOUR:
	case vartype::PGYEAR:
	case vartype::GEO_LONG:
	case vartype::GEO_LAT:
		return m_MultiPG_dep.FirstStr;
	case vartype::SUMVAR:
		return m_MultiSum_dep.FirstStr;
	case vartype::VALVAR:
	case vartype::VALVARSUM:
		return m_MultiVal_dep.FirstStr;
	case vartype::PARVAR:
		return m_MultiPar_dep.FirstStr;
	}
	return "";
}
std::string ChartProp_AxisRepresentation::GetSecondIndepString() {
	switch(m_Indep.Second) {
	case vartype::PGVARVALID:
		return m_ValidationPG_dep[0].SecondStr;
	case vartype::PGVAR:
	case vartype::PGDAY:
	case vartype::PGHOUR:
	case vartype::PGYEAR:
	case vartype::GEO_LONG:
	case vartype::GEO_LAT:
	case vartype::PGDEPTH:
		return m_MultiPG_dep.SecondStr;
	case vartype::SUMVAR:
	case vartype::VALVARSUM:
		return m_MultiSum_dep.SecondStr;
	case vartype::VALVAR:
		return m_MultiVal_dep.SecondStr;
	case vartype::PARVAR:
		return m_MultiPar_dep.SecondStr;
	}
	return "";
}
void ChartProp_AxisRepresentation::SetFirstIndepIndex(enum vartype var, size_t index, size_t indep_index) {
	switch(var) {
	case vartype::PGVARVALID:
		m_ValidationPG_dep[indep_index].First_Index=index;
		m_ValidationPG_dep[indep_index].FirstIndepSet=true;
		break;
	case vartype::PGDAY:
	case vartype::PGHOUR:
	case vartype::PGYEAR:
	case vartype::PGVAR:
		m_MultiPG_dep.First_Index=index;
		m_MultiPG_dep.FirstIndepSet=true;
		break;
	case vartype::SUMVAR:
		m_MultiSum_dep.First_Index=index;
		m_MultiSum_dep.FirstIndepSet=true;
		break;
	case vartype::VALVAR:
	case vartype::VALVARSUM:
		m_MultiVal_dep.First_Index=index;
		m_MultiVal_dep.FirstIndepSet=true;
		break;
	case vartype::PARVAR:
		m_MultiPar_dep.First_Index=index;
		m_MultiPar_dep.FirstIndepSet=true;
		break;
	case vartype::NO_USE:
		m_MultiSum_dep.First_Index= std::string::npos;
		m_MultiSum_dep.FirstIndepSet=false;
		m_MultiPG_dep.FirstIndepSet=false;
		m_MultiVal_dep.FirstIndepSet=false;
		m_MultiPar_dep.FirstIndepSet=false;

	}
}
void ChartProp_AxisRepresentation::SetFirstIndepSet(enum vartype var, bool value, size_t index) {
	
	switch(var) {
	case vartype::GEO_LAT:
	case vartype::GEO_LONG:
	case vartype::GEO_ALT:
	case vartype::PGDAY:
	case vartype::PGHOUR:
	case vartype::PGYEAR:
	case vartype::DATEAXIS:
		if(value) {
			m_MultiPG_dep.FirstIndepSet=true;
			m_MultiPG_dep.FirstType=var;
			m_Indep.First=var;
		}
		else {
			m_MultiPG_dep.FirstIndepSet=false;
			m_MultiPG_dep.FirstType= vartype::NO_USE;
			m_Indep.First= vartype::NO_USE;
		}
		break;
	case vartype::PGVARVALID:
		if(value) { 
			m_ValidationPG_dep[index].FirstIndepSet=true;
			m_Indep.First= vartype::PGVARVALID;
		}
		else {
			m_ValidationPG_dep[index].FirstIndepSet=false;
			m_Indep.First= vartype::NO_USE;
			m_ValidationPG_dep[index].First_Index= std::string::npos;
		}
		break;
	case vartype::PGVAR:
		if(value) 
			m_MultiPG_dep.FirstIndepSet=true;
		else {
			m_MultiPG_dep.FirstIndepSet=false;
			m_Indep.First= vartype::NO_USE;
			m_MultiPG_dep.First_Index= std::string::npos;
		}
		break;
	case vartype::SUMVAR:
		if(value)
			m_MultiSum_dep.FirstIndepSet=true;
		else {
			m_MultiSum_dep.FirstIndepSet=false;
			m_Indep.First= vartype::NO_USE;

		}
		break;
	case vartype::VALVAR:
	case vartype::VALVARSUM:
		if (value) {
			m_MultiVal_dep.FirstIndepSet = true;
			m_Indep.First = var;
		}
		else {
			m_MultiVal_dep.FirstIndepSet=false;
			m_Indep.First= vartype::NO_USE;

		}
		break;
	case vartype::PARVAR:
		if(value)
			m_MultiPar_dep.FirstIndepSet=true;
		else {
			m_MultiVal_dep.FirstIndepSet=false;
			m_Indep.First= vartype::NO_USE;
		}
		break;
	case vartype::PGDEPTH:
		if(value) {
			m_MultiPG_dep.FirstIndepSet=true;
			m_Indep.First= vartype::PGDEPTH;
		}
		else {
			m_MultiPG_dep.FirstIndepSet=false;
			m_Indep.First= vartype::NO_USE;
		}
		break;
	case vartype::NO_USE:
		m_MultiSum_dep.First_Index= std::string::npos;
		m_MultiSum_dep.FirstIndepSet=false;
	}
}
void ChartProp_AxisRepresentation::SetSecondIndepSet(enum vartype var, bool value) {
	
	if(value==false) m_Indep.Second= vartype::NO_USE;
	else  m_Indep.Second=var;
	
	
	
	switch(var) {
	case vartype::GEO_LAT:
	case vartype::GEO_LONG:
	case vartype::GEO_ALT:
	case vartype::PGDAY:
	case vartype::PGHOUR:
	case vartype::PGYEAR:
		if(value) {
			m_MultiPG_dep.SecondIndepSet=true;
			m_MultiPG_dep.SecondType=var;
			m_Indep.Second=var;
		}
		else {
			m_MultiPG_dep.SecondIndepSet=false;
			m_MultiPG_dep.SecondType= vartype::NO_USE;
			m_Indep.Second= vartype::NO_USE;
		}
		break;
	case vartype::PGVAR:
		if(value) 
			m_MultiPG_dep.SecondIndepSet=true;
		else {
			m_MultiPG_dep.SecondIndepSet=false;
			m_Indep.Second= vartype::NO_USE;
			m_MultiPG_dep.Second_Index= std::string::npos;
		}
		break;
	case vartype::SUMVAR:
		if(value)
			m_MultiSum_dep.SecondIndepSet=true;
		else {
			m_MultiSum_dep.SecondIndepSet=false;
			m_Indep.Second= vartype::NO_USE;

		}
		break;
	case vartype::VALVAR:
	case vartype::VALVARSUM:
		if(value) 
			m_MultiVal_dep.SecondIndepSet=true;
		else {
			m_MultiVal_dep.SecondIndepSet=false;
			m_Indep.Second= vartype::NO_USE;

		}
		break;
	case vartype::PARVAR:
		if(value)
			m_MultiPar_dep.SecondIndepSet=true;
		else {
			m_MultiVal_dep.SecondIndepSet=false;
			m_Indep.Second= vartype::NO_USE;
		}
		break;
	case vartype::PGDEPTH:
		if(value) {
			m_MultiPG_dep.SecondIndepSet=true;
			m_Indep.Second= vartype::PGDEPTH;
		}
		else {
			m_MultiPG_dep.SecondIndepSet=false;
			m_Indep.Second= vartype::NO_USE;
		}

	case vartype::NO_USE:
		m_MultiSum_dep.Second_Index= std::string::npos;
		m_MultiSum_dep.SecondIndepSet=false;
	}
}

void ChartProp_AxisRepresentation::SetSecondIndepIndex(enum vartype var, size_t index) {
	switch(var) {
	case vartype::GEO_LAT:
	case vartype::GEO_LONG:
	case vartype::GEO_ALT:
	case vartype::PGDAY:
	case vartype::PGHOUR:
	case vartype::PGYEAR:
		m_MultiPG_dep.Second_Index=index;
		m_MultiPG_dep.SecondIndepSet=true;
		m_MultiPG_dep.SecondType=var;
		break;

	case vartype::PGVARVALID:
		m_ValidationPG_dep[0].Second_Index=index;  //preliminary only one second index
		m_ValidationPG_dep[0].SecondIndepSet=true;
		break;
	case vartype::PGVAR:
		m_MultiPG_dep.Second_Index=index;
		m_MultiPG_dep.SecondIndepSet=true;
		break;
	case vartype::SUMVAR:
		m_MultiSum_dep.Second_Index=index;
		m_MultiSum_dep.SecondIndepSet=true;
		break;
	case vartype::VALVAR:
	case vartype::VALVARSUM:
		m_MultiVal_dep.SecondIndepSet=true;
		m_MultiVal_dep.Second_Index=index;
		break;
	case vartype::PARVAR:
		m_MultiPar_dep.SecondIndepSet=true;
		m_MultiPar_dep.Second_Index=index;
		break;
	case vartype::NO_USE:

		m_MultiSum_dep.Second_Index= std::string::npos;
		m_MultiSum_dep.SecondIndepSet=false;
		m_MultiVal_dep.SecondIndepSet=false;
		m_MultiPar_dep.SecondIndepSet=false;
		m_MultiPG_dep.SecondIndepSet=false;
	//	m_ValidationPG_dep[0].SecondIndepSet=false;

	}
}
void ChartProp_AxisRepresentation::SetFirstIndepString(enum vartype var, std::string str, size_t index) {
	switch(var) {
	case vartype::PGVARVALID:
		m_ValidationPG_dep[index].FirstStr=str;
		break;
	case vartype::PGDAY:
	case vartype::PGHOUR:
	case vartype::PGYEAR:
	case vartype::PGVAR:
	case vartype::GEO_LAT:
	case vartype::GEO_LONG:
	case vartype::GEO_ALT:
    
		m_MultiPG_dep.FirstStr=str;
		break;
	case vartype::SUMVAR:
		m_MultiSum_dep.FirstStr=str;
		break;
	case vartype::VALVAR:
	case vartype::VALVARSUM:
		m_MultiVal_dep.FirstStr=str;
		break;
	case vartype::PARVAR:
		m_MultiPar_dep.FirstStr=str;
		break;
	}
}
void ChartProp_AxisRepresentation::SetSecondIndepString(enum vartype var, std::string str) {
	switch(var) {
	case vartype::PGVARVALID:
		m_ValidationPG_dep[0].SecondStr=str;
		break;
	case vartype::PGVAR:
	case vartype::PGDAY:
	case vartype::PGHOUR:
	case vartype::PGYEAR:
	case vartype::GEO_LAT:
	case vartype::GEO_LONG:
	case vartype::GEO_ALT:
	case vartype::PGDEPTH:
		m_MultiPG_dep.SecondStr=str;
		break;
	case vartype::SUMVAR:
	case vartype::VALVARSUM:
		m_MultiSum_dep.SecondStr=str;
		break;
	case vartype::VALVAR:
		m_MultiVal_dep.SecondStr=str;
		break;
	case vartype::PARVAR:
		m_MultiPar_dep.SecondStr=str;
		break;
	}
}
