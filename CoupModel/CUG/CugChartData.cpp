/***************************************************
  Ultimate Grid version 7.2
  Copyright © The Ultimate Toolbox 2007, All Rights Reserved

  Data Source Base Class


  Standard Return Values: UG_SUCCESS - success (0)
						  UG_NA		 - not available (-1)
						  1 and up   - error codes
****************************************************/
#include "..\stdafx.h"
#include "..\std.h"
#include "..\CUG\UGCtrl.h"
#include "CugChartData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
Constructor
	Init all data here
****************************************************/
CugChartData::CugChartData(TCHAR delim, TCHAR quote, LPCTSTR quoted){
		m_rows=-1;
		m_cols=0;
		m_ctrl=nullptr;
		m_delim=delim;
		m_quote=quote;
		m_quoted=quoted;
		m_dirty = FALSE;
}
/***************************************************
Destructor
	Perform clean up here
****************************************************/
CugChartData::~CugChartData(){
	Close();
}

/***************************************************
********** opening and closing *********************
****************************************************/


/***************************************************
****************************************************/
int CugChartData::Open(LPCTSTR name,LPCTSTR option){

	Close();


	// open the given delimitted file and load into memory
	TRY	{
		// do a wait cursor
		CWaitCursor wait;

		CStdioFile file(name, CFile::modeRead);
		CString buff;
		CString temp;
		BOOL ret=TRUE;
		m_rows=-1;
		m_cols=0;
		int col;
		int pos;
		int qpos;

		CUGCell cell;
		// loop through the entire file reading in data
		while(ret){
			m_rows++;
			ret = file.ReadString(buff);
			col = 0;

			while((pos=buff.Find(m_delim))!=-1){
				if(buff[0]==m_quote){
					// this is quoted text -- have to figure out 
					// where it stops...
					temp=_T("");
					buff=buff.Mid(1);
					while((qpos=buff.Find(m_quote))!=-1){
						if(buff[qpos+1]==m_quote){
							// this is a double quote
							temp +=buff.Left(qpos+1);
							buff = buff.Mid(qpos+2);
						}
						else{
							// this quote should match the beginning quote
							ASSERT(buff[qpos+1]==m_delim  || qpos+1==buff.GetLength());
							temp += buff.Left(qpos);
							buff = buff.Mid(qpos+2);
							break;
						}
					}
				}
				else {
					temp=buff.Left(pos);
					buff=buff.Mid(pos+1);
				}
				cell.SetText(temp);
				SetCell(col, m_rows, &cell);
				col++;
			}
			cell.SetText(buff);
			SetCell(col, m_rows, &cell);
			// the number of columns will be the maximum cols found
			// in any row
			col++;
			m_cols=max(m_cols,col);
		}
		// if we are currently attached to the grid, reset the number
		// of rows and cols
		if(m_ctrl!=nullptr){
			m_ctrl->SetNumberRows(m_rows);
			m_ctrl->SetNumberCols(m_cols);
		}
	}
	CATCH(CFileException,e)	{
		return UG_ERROR;
	}
	END_CATCH;

	m_fileName=name;
	SetDirty(FALSE);
	
	return UG_SUCCESS;
}
/***************************************************
****************************************************/
int CugChartData::Close(){
	if(GetDirty() && m_ctrl!=nullptr){
		m_ctrl->OnDataSourceNotify(/*ID?*/0, UG_DELIM_DS_CLOSEDIRTY, 0);
	}
	return UG_NA;
}
/***************************************************
****************************************************/
int CugChartData::Save(){
	// save with the cached filename.
	return SaveAs(m_fileName,nullptr);
}
/***************************************************
****************************************************/
int CugChartData::SaveAs(LPCTSTR name,LPCTSTR option){
	if(*name==_T('\0') || m_ctrl==nullptr)
		return UG_ERROR;

	// save out the information in delimited format
	TRY	{
		CStdioFile file(name, CFile::modeWrite|CFile::modeCreate);
		CString buff;
		CString text;

		int cols = m_ctrl->GetNumberCols();
		long rows = m_ctrl->GetNumberRows();
		int pos;
		CString temp;
		CUGCell cell;
		for(int i=0;i<rows;i++){
			buff = _T("");
			for(int j=0;j<cols;j++)	{
				GetCell(j,i,&cell);
				cell.GetText(&text);
				// double any quote chars
				if(text.Find(m_quote)!=-1){
					temp=_T("");
					while((pos=text.Find(m_quote))!=-1){
						temp+=text.Left(pos+1);
						temp+=m_quote;
						text=text.Mid(++pos);
					}
					temp+=text;
					text=temp;
				}
				// if one of the quoted chars is in the string 
				//    quote the string
				if(text.FindOneOf(m_quoted)!=-1){
					buff += m_quote;
					buff +=text;
					buff += m_quote;
					}
				else
					buff +=text;
				// add a delimiter
				buff +=m_delim;
			}
			// erase the last delimiter
			buff.SetAt(buff.GetLength()-1, '\n');
			file.WriteString(buff);
		}
	}
	CATCH(CFileException,e)	{
		return UG_ERROR;
	}
	END_CATCH;

	m_fileName=name;
	SetDirty(FALSE);
	return UG_SUCCESS;
}

long CugChartData::GetNumRows(){
	return m_rows;
}
/***************************************************
****************************************************/
int CugChartData::GetNumCols(){
	return m_cols;
}


/***************************************************
****************************************************/
int CugChartData::SetCell(int col,long row,CUGCell *cell){
	SetDirty();
	return CUGMem::SetCell(col,row,cell);
}
