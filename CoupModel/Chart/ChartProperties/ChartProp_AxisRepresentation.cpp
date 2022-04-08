#pragma once
#include "./ChartProp_AxisRepresentation.h"



class SimB;
struct SIMB;
class CPG;

ChartProp_AxisRepresentation::ChartProp_AxisRepresentation(void)
{
	m_Indep.First=DATEAXIS;
	m_Indep.Second=NO_USE;
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
	case PGVARVALID:
		return m_ValidationPG_dep[index].First_Index;
	case GEO_LONG:
	case PGVAR:
	case PGDAY:
	case PGYEAR:
	case PGHOUR:
		return m_MultiPG_dep.First_Index;
	case SUMVAR:
		return m_MultiSum_dep.First_Index;
	case VALVAR:
	case VALVARSUM:
		return m_MultiVal_dep.First_Index;
	case PARVAR:
		return m_MultiPar_dep.First_Index;
	case PGDEPTH:
		return m_MultiPG_dep.First_Index;

	}

	return string::npos;
}
size_t ChartProp_AxisRepresentation::GetSecondIndepIndex() {
	switch(m_Indep.Second) {
	case PGVARVALID:
		return m_ValidationPG_dep[0].Second_Index;
	case GEO_LAT:
	case GEO_LONG:
	case PGVAR:
		return m_MultiPG_dep.Second_Index;
	case SUMVAR:
		return m_MultiSum_dep.Second_Index;
	case VALVAR:
	case VALVARSUM:
		return m_MultiVal_dep.Second_Index;
	case PARVAR:
		return m_MultiPar_dep.Second_Index;
	}
	return string::npos;
}
bool ChartProp_AxisRepresentation::IsFirstIndepSet(size_t index) {
	switch(m_Indep.First) {
	case DATEAXIS:
		return true;
	case PGVARVALID:
		return m_ValidationPG_dep[index].FirstIndepSet;
	case PGDAY:
	case PGHOUR:
	case PGYEAR:
	case PGVAR:
	case GEO_LONG:
	case GEO_LAT:
		return m_MultiPG_dep.FirstIndepSet;
	case SUMVAR:
		return m_MultiSum_dep.FirstIndepSet;
	case VALVAR:
	case VALVARSUM:
		return m_MultiVal_dep.FirstIndepSet;
	case PARVAR:
		return m_MultiPar_dep.FirstIndepSet;
	case PGDEPTH:
		return m_MultiPG_dep.FirstIndepSet;
	}
	return false;
}
bool ChartProp_AxisRepresentation::IsFirstIndepSet(enum vartype vartyp, size_t index) {
	switch(vartyp) {
	case PGVARVALID:
		return m_ValidationPG_dep[index].FirstIndepSet;
	case PGDAY:
	case PGHOUR:
	case PGYEAR:
	case PGVAR:
	case GEO_LONG:
	case GEO_LAT:
		return m_MultiPG_dep.FirstIndepSet;
	case SUMVAR:
		return m_MultiSum_dep.FirstIndepSet;
	case VALVAR:
	case VALVARSUM:
		return m_MultiVal_dep.FirstIndepSet;
	case PARVAR:
		return m_MultiPar_dep.FirstIndepSet;
	case PGDEPTH:
		return m_MultiPG_dep.FirstIndepSet;
	}
	return false;
}
bool ChartProp_AxisRepresentation::IsSecondIndepSet() {
	switch(m_Indep.Second) {
	case PGVARVALID:
		return m_ValidationPG_dep[0].SecondIndepSet;
	case PGDAY:
	case PGHOUR:
	case PGYEAR:
	case PGVAR:
	case GEO_LAT:
	case GEO_LONG:
	case PGDEPTH:
		return m_MultiPG_dep.SecondIndepSet;
	case SUMVAR:
		if(m_MultiSum_dep.SecondIndepSet)
		return m_MultiSum_dep.SecondIndepSet;
	case VALVAR:
	case VALVARSUM:
		if(m_MultiVal_dep.SecondIndepSet)
		return m_MultiVal_dep.SecondIndepSet;
	case PARVAR:
		return m_MultiPar_dep.SecondIndepSet;
	case DATEAXIS:
		return true;
	}
	return false;
}
bool ChartProp_AxisRepresentation::IsSecondIndepSet(enum vartype vartyp) {
	switch(vartyp) {
	case PGVARVALID:
		return m_ValidationPG_dep[0].SecondIndepSet;
	case PGDAY:
	case PGHOUR:
	case PGYEAR:
	case PGVAR:
		return m_MultiPG_dep.SecondIndepSet;
	case SUMVAR:
		return m_MultiSum_dep.SecondIndepSet;
	case VALVAR:
		return m_MultiVal_dep.SecondIndepSet;
	case PARVAR:
		return m_MultiPar_dep.SecondIndepSet;
	}
	return false;
}
string ChartProp_AxisRepresentation::GetFirstIndepString(size_t index) {
	switch(m_Indep.First) {
	case PGVARVALID:
		return m_ValidationPG_dep[index].FirstStr;
	case PGVAR:
	case PGDAY:
	case PGHOUR:
	case PGYEAR:
	case GEO_LONG:
	case GEO_LAT:
		return m_MultiPG_dep.FirstStr;
	case SUMVAR:
		return m_MultiSum_dep.FirstStr;
	case VALVAR:
	case VALVARSUM:
		return m_MultiVal_dep.FirstStr;
	case PARVAR:
		return m_MultiPar_dep.FirstStr;
	}
	return "";
}
string ChartProp_AxisRepresentation::GetSecondIndepString() {
	switch(m_Indep.Second) {
	case PGVARVALID:
		return m_ValidationPG_dep[0].SecondStr;
	case PGVAR:
	case PGDAY:
	case PGHOUR:
	case PGYEAR:
	case GEO_LONG:
	case GEO_LAT:
	case PGDEPTH:
		return m_MultiPG_dep.SecondStr;
	case SUMVAR:
	case VALVARSUM:
		return m_MultiSum_dep.SecondStr;
	case VALVAR:
		return m_MultiVal_dep.SecondStr;
	case PARVAR:
		return m_MultiPar_dep.SecondStr;
	}
	return "";
}
void ChartProp_AxisRepresentation::SetFirstIndepIndex(enum vartype var, size_t index, size_t indep_index) {
	switch(var) {
	case PGVARVALID:
		m_ValidationPG_dep[indep_index].First_Index=index;
		m_ValidationPG_dep[indep_index].FirstIndepSet=true;
		break;
	case PGDAY:
	case PGHOUR:
	case PGYEAR:
	case PGVAR:
		m_MultiPG_dep.First_Index=index;
		m_MultiPG_dep.FirstIndepSet=true;
		break;
	case SUMVAR: 
		m_MultiSum_dep.First_Index=index;
		m_MultiSum_dep.FirstIndepSet=true;
		break;
	case VALVAR:
	case VALVARSUM:
		m_MultiVal_dep.First_Index=index;
		m_MultiVal_dep.FirstIndepSet=true;
		break;
	case PARVAR:
		m_MultiPar_dep.First_Index=index;
		m_MultiPar_dep.FirstIndepSet=true;
		break;
	case NO_USE:
		m_MultiSum_dep.First_Index=string::npos;
		m_MultiSum_dep.FirstIndepSet=false;
		m_MultiPG_dep.FirstIndepSet=false;
		m_MultiVal_dep.FirstIndepSet=false;
		m_MultiPar_dep.FirstIndepSet=false;

	}
}
void ChartProp_AxisRepresentation::SetFirstIndepSet(enum vartype var, bool value, size_t index) {
	
	switch(var) {
	case GEO_LAT:
	case GEO_LONG:
	case GEO_ALT:
	case PGDAY:
	case PGHOUR:
	case PGYEAR:
	case DATEAXIS:
		if(value) {
			m_MultiPG_dep.FirstIndepSet=true;
			m_MultiPG_dep.FirstType=var;
			m_Indep.First=var;
		}
		else {
			m_MultiPG_dep.FirstIndepSet=false;
			m_MultiPG_dep.FirstType=NO_USE;
			m_Indep.First=NO_USE;
		}
		break;
	case PGVARVALID:
		if(value) { 
			m_ValidationPG_dep[index].FirstIndepSet=true;
			m_Indep.First=PGVARVALID;
		}
		else {
			m_ValidationPG_dep[index].FirstIndepSet=false;
			m_Indep.First=NO_USE;
			m_ValidationPG_dep[index].First_Index=string::npos;
		}
		break;
	case PGVAR:
		if(value) 
			m_MultiPG_dep.FirstIndepSet=true;
		else {
			m_MultiPG_dep.FirstIndepSet=false;
			m_Indep.First=NO_USE;
			m_MultiPG_dep.First_Index=string::npos;
		}
		break;
	case SUMVAR:
		if(value)
			m_MultiSum_dep.FirstIndepSet=true;
		else {
			m_MultiSum_dep.FirstIndepSet=false;
			m_Indep.First=NO_USE;

		}
		break;
	case VALVAR:
	case VALVARSUM:
		if (value) {
			m_MultiVal_dep.FirstIndepSet = true;
			m_Indep.First = var;
		}
		else {
			m_MultiVal_dep.FirstIndepSet=false;
			m_Indep.First=NO_USE;

		}
		break;
	case PARVAR:
		if(value)
			m_MultiPar_dep.FirstIndepSet=true;
		else {
			m_MultiVal_dep.FirstIndepSet=false;
			m_Indep.First=NO_USE;
		}
		break;
	case PGDEPTH:
		if(value) {
			m_MultiPG_dep.FirstIndepSet=true;
			m_Indep.First=PGDEPTH;
		}
		else {
			m_MultiPG_dep.FirstIndepSet=false;
			m_Indep.First=NO_USE;
		}
		break;
	case NO_USE:
		m_MultiSum_dep.First_Index=string::npos;
		m_MultiSum_dep.FirstIndepSet=false;
	}
}
void ChartProp_AxisRepresentation::SetSecondIndepSet(enum vartype var, bool value) {
	
	if(value==false) m_Indep.Second=NO_USE; 
	else  m_Indep.Second=var;
	
	
	
	switch(var) {
	case GEO_LAT:
	case GEO_LONG:
	case GEO_ALT:
	case PGDAY:
	case PGHOUR:
	case PGYEAR:
		if(value) {
			m_MultiPG_dep.SecondIndepSet=true;
			m_MultiPG_dep.SecondType=var;
			m_Indep.Second=var;
		}
		else {
			m_MultiPG_dep.SecondIndepSet=false;
			m_MultiPG_dep.SecondType=NO_USE;
			m_Indep.Second=NO_USE;
		}
		break;
	case PGVAR:
		if(value) 
			m_MultiPG_dep.SecondIndepSet=true;
		else {
			m_MultiPG_dep.SecondIndepSet=false;
			m_Indep.Second=NO_USE;
			m_MultiPG_dep.Second_Index=string::npos;
		}
		break;
	case SUMVAR:
		if(value)
			m_MultiSum_dep.SecondIndepSet=true;
		else {
			m_MultiSum_dep.SecondIndepSet=false;
			m_Indep.Second=NO_USE;

		}
		break;
	case VALVAR:
	case VALVARSUM:
		if(value) 
			m_MultiVal_dep.SecondIndepSet=true;
		else {
			m_MultiVal_dep.SecondIndepSet=false;
			m_Indep.Second=NO_USE;

		}
		break;
	case PARVAR:
		if(value)
			m_MultiPar_dep.SecondIndepSet=true;
		else {
			m_MultiVal_dep.SecondIndepSet=false;
			m_Indep.Second=NO_USE;
		}
		break;
	case PGDEPTH:
		if(value) {
			m_MultiPG_dep.SecondIndepSet=true;
			m_Indep.Second=PGDEPTH;
		}
		else {
			m_MultiPG_dep.SecondIndepSet=false;
			m_Indep.Second=NO_USE;
		}

	case NO_USE:
		m_MultiSum_dep.Second_Index=string::npos;
		m_MultiSum_dep.SecondIndepSet=false;
	}
}

void ChartProp_AxisRepresentation::SetSecondIndepIndex(enum vartype var, size_t index) {
	switch(var) {
	case GEO_LAT:
	case GEO_LONG:
	case GEO_ALT:
	case PGDAY:
	case PGHOUR:
	case PGYEAR:
		m_MultiPG_dep.Second_Index=index;
		m_MultiPG_dep.SecondIndepSet=true;
		m_MultiPG_dep.SecondType=var;
		break;

	case PGVARVALID:
		m_ValidationPG_dep[0].Second_Index=index;  //preliminary only one second index
		m_ValidationPG_dep[0].SecondIndepSet=true;
		break;
	case PGVAR:
		m_MultiPG_dep.Second_Index=index;
		m_MultiPG_dep.SecondIndepSet=true;
		break;
	case SUMVAR:
		m_MultiSum_dep.Second_Index=index;
		m_MultiSum_dep.SecondIndepSet=true;
		break;
	case VALVAR:
	case VALVARSUM:
		m_MultiVal_dep.SecondIndepSet=true;
		m_MultiVal_dep.Second_Index=index;
		break;
	case PARVAR:
		m_MultiPar_dep.SecondIndepSet=true;
		m_MultiPar_dep.Second_Index=index;
		break;
	case NO_USE:

		m_MultiSum_dep.Second_Index=string::npos;
		m_MultiSum_dep.SecondIndepSet=false;
		m_MultiVal_dep.SecondIndepSet=false;
		m_MultiPar_dep.SecondIndepSet=false;
		m_MultiPG_dep.SecondIndepSet=false;
	//	m_ValidationPG_dep[0].SecondIndepSet=false;

	}
}
void ChartProp_AxisRepresentation::SetFirstIndepString(enum vartype var, string str, size_t index) {
	switch(var) {
	case PGVARVALID:
		m_ValidationPG_dep[index].FirstStr=str;
		break;
	case PGDAY:
	case PGHOUR:
	case PGYEAR:
	case PGVAR:
	case GEO_LAT:
	case GEO_LONG:
	case GEO_ALT:
    
		m_MultiPG_dep.FirstStr=str;
		break;
	case SUMVAR:
		m_MultiSum_dep.FirstStr=str;
		break;
	case VALVAR:
	case VALVARSUM:
		m_MultiVal_dep.FirstStr=str;
		break;
	case PARVAR:
		m_MultiPar_dep.FirstStr=str;
		break;
	}
}
void ChartProp_AxisRepresentation::SetSecondIndepString(enum vartype var, string str) {
	switch(var) {
	case PGVARVALID:
		m_ValidationPG_dep[0].SecondStr=str;
		break;
	case PGVAR:
	case PGDAY:
	case PGHOUR:
	case PGYEAR:
	case GEO_LAT:
	case GEO_LONG:
	case GEO_ALT:
	case PGDEPTH:
		m_MultiPG_dep.SecondStr=str;
		break;
	case SUMVAR:
	case VALVARSUM:
		m_MultiSum_dep.SecondStr=str;
		break;
	case VALVAR:
		m_MultiVal_dep.SecondStr=str;
		break;
	case PARVAR:
		m_MultiPar_dep.SecondStr=str;
		break;
	}
}
