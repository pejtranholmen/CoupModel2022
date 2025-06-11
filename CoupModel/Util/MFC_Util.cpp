#include "../std.h"
#include "MFC_Util.h"
#include "../../MainFrm.h"

#ifndef MISSING
#define MISSING -1.1E38

#endif
static TCHAR buffer[900000];
CString IDCODE = _T("CoupModel");

CString COLORLINE[12], HCOLORLINE[12];


void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}




MFC_Util::MFC_Util(void)
{
	CWinApp* m_pApp = AfxGetApp();
	IDCODE = _T("CoupModel");
	CString str;
	for (size_t i = 1; i < 13; i++) {
		str.Format(_T("ColorLine%d"), i);
		COLORLINE[i - 1]=str;
		str.Format(_T("HColorLine%d"), i);
		HCOLORLINE[i - 1] = str;
	}
	m_pMainFrame = MySingle::instance().GetFramePointer();

}


MFC_Util::~MFC_Util(void)
{
}
 std::string MFC_Util::GetProfileStringStd(string item, std::string value) {

#ifndef COUPSTD
	CWinApp* pApp = AfxGetApp();
	CString Comp=pApp->GetProfileString(IDCODE, CString(item.c_str()), CString(value.c_str()));
	return CT2A(Comp).m_psz;
#endif
	return value;
}
 int MFC_Util::GetProfileIntNo(string item, int value) {
#ifndef COUPSTD
	CWinApp* pApp = AfxGetApp();
	int Comp=pApp->GetProfileInt(IDCODE,CString(item.c_str()), value);
	return Comp;
#endif
	return value;
}
 bool MFC_Util::isHighVertResolution() {

	 int horizontal = 0;
	 int vertical = 0;
	 GetDesktopResolution(horizontal, vertical);
	 if (vertical > 1000)
		 return true;
	 else
		 return false;


 }

 std::string MFC_Util::GetWindowFileDate(string filename, bool modified)
 {
	 std::string strout;
	 CWinApp *pApp;
	 pApp = AfxGetApp();
	 CFileStatus status;
	 CFile::GetStatus(CString(filename.c_str()), status);
	 CTime kk = status.m_ctime;
	 if(modified)
		kk = status.m_mtime;

	 std::string v1= CT2A(kk.Format(_T("%Y"))).m_psz;
	 v1 += "-";
	 v1+=CT2A(kk.Format(_T("%m"))).m_psz;
	 v1 += "-";
	 v1+=CT2A(kk.Format(_T("%d"))).m_psz;
	 return v1;
 }

 int MFC_Util::GetLowResFontSize(int DefSize=16)
 {
	 return GetProfileIntNo("LowResFont", DefSize);
 }
 int MFC_Util::GetHighResFontSize(int DefSize = 22)
 {
	 return GetProfileIntNo("HighResFont", DefSize);
 }

 int MFC_Util::GetFontSize() {
	 int Default;
	 if (isHighVertResolution()) {
		 Default = 22;
		 return GetProfileIntNo("HighResFont", Default);
	 }
	 else {
		 Default = 16;
		 return GetProfileIntNo("LowResFont", Default);
	 }


 }
 int MFC_Util::GetBestGridRowHeight() {
	 return GetFontSize() + 6;
 }
 double MFC_Util::GetColumnWidthScaleFactor() {
	 return double(GetFontSize()) / 16.;
 }

 void MFC_Util::SetPaneMessage(string str)
 {
	 if(m_pMainFrame!=nullptr) m_pMainFrame = MySingle::instance().GetFramePointer();
	 m_pMainFrame->UpdatePaneText(0, str);
	
 }

 std::string MFC_Util::GetNewSimFileName(int SimulationRunNo, std::string pzPathName) {
	 std::string newPathName;
	 std::string newtype;
	 newtype = ".Sim";

	 auto iposext = pzPathName.rfind('_');
	 auto iposMul = pzPathName.find("_Multi_");
	 auto iposSim = pzPathName.find(".Sim");
	 if (iposSim == std::string::npos) {
		 auto iposSim = pzPathName.find(".xml");
		 newtype = ".xml";

	 }


	 SimulationRunNo = MFC_Util::GetProfileIntNo("SimulationRunNo", SimulationRunNo);

	 std::string Num = FUtil::ItoNumAscii(SimulationRunNo);
	 if (iposMul!= std::string::npos && iposext!= std::string::npos)
		 newPathName = pzPathName.substr(0,iposMul + 1) + Num + newtype;
	 else if (iposext!= std::string::npos)
		 newPathName = pzPathName.substr(0,iposext + 1) + Num + newtype;
	 else if (iposSim!= std::string::npos)
		 newPathName = pzPathName.substr(0,iposSim) + '_' + Num + newtype;
	 else
		 newPathName = pzPathName;


	 return newPathName;
 }
 FILEDLG MFC_Util::GetFileDialog(string type, std::string defname, std::string filtertype, BOOL OpenOnly) {
	 FILEDLG output;
	 output.filename = "";
	 output.opengood = false;

	 int fileDlgResult;

	 LPCTSTR lpszDefName = CString(defname.c_str());
	 LPCTSTR lpszFilter = CString(type.c_str());


	 CFileDialog	ParFileDlg(OpenOnly, lpszFilter , lpszDefName, OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, CString(filtertype.c_str()));
	 //Show the dialog
	 fileDlgResult = ParFileDlg.DoModal();
	 //If the cancel button has been pushed then return
	 if (fileDlgResult == IDCANCEL) {
		 return output;
	 
	 }
	 else {
		 output.filename= CT2A(ParFileDlg.GetPathName()).m_psz;
		 output.opengood = true;
		 return output;
	 }
 }
 void MFC_Util::WriteProfileInt(string item, int value) {
#ifndef COUPSTD
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(IDCODE, CString(item.c_str()), value);
#endif
}
 void MFC_Util::WriteProfileString(string item, std::string value) {
#ifndef COUPSTD
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileString(IDCODE, CString(item.c_str()), CString(value.c_str()));
#endif
}
 int MFC_Util::GetOptionIndex(CString optionlist, CString str)
{
	int ipos_end, index;
	ipos_end=optionlist.Find(str);	
	if(ipos_end==0) return 0;
	optionlist=optionlist.Mid(0,ipos_end+1);
	int ipos=optionlist.Find(_T("\n"));
	if(ipos<0)
		index=-1;
	else
		index=0;
	while(ipos>0) {
		optionlist=optionlist.Mid(ipos+1);
		ipos=optionlist.Find(_T("\n"));
		index++;
	}
	return index;

}
 float MFC_Util::Exp(int exp) {
	int expvalue;
	expvalue=exp;
	float value=1;
	if (expvalue<0) while( expvalue < 0)
	{value=float(value*0.1); expvalue++;}
	if (expvalue>0) while( expvalue > 0)
	{value=value*10;expvalue--;}
	return value;
}
 int MFC_Util::AtoInt(CString str)  {
		CString v=str;
	while(v.Left(1) == " ") {
		v=v.Mid(1);}
 	v=v.SpanIncluding(_T("1234567890-+"));
 	int last=v.GetLength()-1;
	if (v.IsEmpty() || (v[last]<'0' || v[last]>'9')) return 0;
	if (v[0]=='-') return (48-(int)v[last])+10*AtoInt(v.Left(last));
	return ((int)v[last]-48)+10*AtoInt(v.Left(last));
}
 float MFC_Util::AtoFloat(CString str) {	
	int siz,decdel,expdel,tal,dec,exp=0,decnumbers;
	CString v=str,expvaluestring;
	float ret;
//	StrClean(v);
   //	v=v.SpanExcluding(" ");
	siz=v.GetLength();
	v.Replace(_T(","),_T("."));
//   sscanf(v,     "%sizf", &ret);

	while(v.Left(1)==" ")
		{
		siz=v.GetLength();
		v=v.Mid(1,siz-1);}
	v=v.SpanIncluding(_T("1234567890.-Ee+"));
	decdel=v.Find(_T("."));
	while (decdel==-1) {
		v+=".";
		decdel=v.Find(_T("."));
		}
	expdel=max(v.Find(_T("E")),v.Find(_T("e")));
	siz=v.GetLength();
	if (v.IsEmpty()) return (float)0.0;
	tal=AtoInt(v.Left(decdel));
	decnumbers=siz-decdel-1;
	if (expdel!=-1) {
		decnumbers=expdel-decdel-1;
		dec=AtoInt(v.Mid(decdel+1,decnumbers));
		expvaluestring=v.Right(siz-expdel-1);
		exp=AtoInt(expvaluestring);}
	else dec=AtoInt(v.Right(decnumbers));

	ret=float(tal);
	ret=ret*Exp(exp);
	if (v.Left(1)== "-") {
		ret=ret-(float) dec*Exp(-decnumbers)*Exp(exp);
	}
	else ret=ret+(float) dec*Exp(-decnumbers)*Exp(exp);
	return ret;

}

// Sample format for latitide and longitudes
// double lat1=45.54243333333333,lat2=45.53722222,long1=-122.96045277777778,long2=-122.9630556;
// Below is the main code
        //#include <cmath>
 unsigned long MFC_Util::GetGroupColor(int num)
{
	if(num==1)				return RGB(230,248,255);
	if(num==2)				return RGB(150,255,150);
	if(num==3)				return RGB(255,200,150);
	if(num==4)				return RGB(100,200,255);
	if(num==5)				return RGB(100,205,100);
	if(num==6)				return RGB(205,205,100);
	if(num==7)				return RGB(255,255,255);
	if(num==8)				return RGB(245,245,245);
	return RGB(255,255,255);
}
 CString MFC_Util::FtoAscii(float num) {	

CString str;

	if(num<-1.E38) return _T("MISSING");
  	if(num>0&&num<1.e-3||num>1.e3)
		str.Format(_T("%9.3e"),num);
	else if(num<0&&num>=-1.e-2||num<0&&num<-1.e3)
		str.Format(_T("%9.3e"),num);
	else if(num>MISSING) 
	    str.Format(_T("%G"),num);
	else
		return _T("MISSING");
	int expdel=str.Find(_T("e+00"));
	if(expdel>0) {
		str=str.Mid(0,expdel+2)+str.Mid(expdel+4,1);
	}
	expdel=str.Find(_T("e-00"));
	if(expdel>0) {
		str=str.Mid(0,expdel+2)+str.Mid(expdel+4,1);
	}

	return str;

}


 CString MFC_Util::FtoAsciiPane(float num) {	
	CString str;
	int decdel;
	if (num==0) return _T(" 0.");
    if (num<= -1.e38) return _T("-");
	else str.Format(_T("%6.3g"),num);
	decdel=str.Find(_T("e+0"));
	while (decdel>0) {
		str=str.Mid(0,decdel+2)+str.Mid(decdel+3,str.GetLength()-decdel-2);
		decdel=str.Find(_T("e+0"));
	}
   	return str;

}

 CString MFC_Util::DtoAscii(double num)  {	
	CString str;
	if (num==0.0) return _T(" 0.0");
  	if(num>0&&num<1.e-3||num>1.e3)
		str.Format(_T("%9.2e"),num);
	else if(num<0&&num>=-1.e-2||num<0&&num<-1.e3)
		str.Format(_T("%9.2e"),num);
	else
		str.Format(_T("%9.4f"),num);

	while (str.Find(_T("0"))==str.GetLength()-1)
  		str=str.Left(str.GetLength()-1);
	return str;
}



 CString MFC_Util::ItoAscii(int tal) {
	int x=tal,ax;
	CString tmp;
	if (x<0) ax=-x;	else ax=x;
	tmp=(char)(ax%10+48);
	if (ax<10) {
		if (x<0) tmp=_T("-") + tmp;
		return tmp;}
	return 	ItoAscii(x/10)+tmp;
}


 CString MFC_Util::ItoAsciiHex(int tal) {
  CString str;
  str.Format(_T("%x"),tal);
  return _T("0x")+str;
}

 void MFC_Util::StrClean(CString &v) {
	CString tmp,tmpch;
	CString noll=_T("0");
	CString nio=_T("9");
	for (int i=0;i<v.GetLength();i++) {
		tmpch=v.GetAt(i);
		if ((tmpch>=noll &&  tmpch<=nio) 
			|| tmpch== "-" || tmpch=="E"
			|| tmpch==".")
			tmp+=tmpch;
	}
	v=tmp;
}
 int MFC_Util::GetColorLine(int line) 
{
	int color;
	CWinApp* m_pApp = AfxGetApp();
	size_t i=(line-1)%11;
	switch (i)
	{
		case 0:
		color=m_pApp->GetProfileInt(IDCODE, _T("ColorLine1"), 0x0000ff);
		break;
		case 1:
		color=m_pApp->GetProfileInt(IDCODE, _T("ColorLine2"),0x00cc00 );
		break;
		case 2:
		color=m_pApp->GetProfileInt(IDCODE, _T("ColorLine3"), 0xff0000);
		break;
		case 3:
		color=m_pApp->GetProfileInt(IDCODE, _T("ColorLine4"), 0xcc0000);
		break;
		case 4:
		color=m_pApp->GetProfileInt(IDCODE, _T("ColorLine5"), 0x00aa00);
		break;
		case 5:
		color=m_pApp->GetProfileInt(IDCODE, _T("ColorLine6"), 0xA52A2A);
		break;
		case 6:
		color=m_pApp->GetProfileInt(IDCODE, _T("ColorLine7"), 0x8B008B);
		break;
		case 7:
		color=m_pApp->GetProfileInt(IDCODE, _T("ColorLine8"), 0x8B0000);
		break;
		case 8:
		color=m_pApp->GetProfileInt(IDCODE, _T("ColorLine9"), 0x87CEFA);
		break;
		case 9:
		color=m_pApp->GetProfileInt(IDCODE, _T("ColorLine10"), 0xFFFFE0);
		break;
		case 10:
		color=m_pApp->GetProfileInt(IDCODE, _T("ColorLine11"), 0xbbccdd);
		break;
		default:
		color=m_pApp->GetProfileInt(IDCODE, _T("ColorLine1"), 0x0000ff);
	}
	if(color==0) 
		return RGB(240,240,240);
	else
		return color;
}
bool MFC_Util::SetColorLine(int line, int color)
{
CWinApp* m_pApp = AfxGetApp();
int colorH;
colorH=RGB(GetBValue(color),GetGValue(color),GetRValue(color));
if (line < 12) {
	m_pApp->WriteProfileInt(IDCODE, COLORLINE[line - 1], color);
	m_pApp->WriteProfileInt(IDCODE, HCOLORLINE[line - 1], colorH);
	return TRUE;
}
else {
	m_pApp->WriteProfileInt(IDCODE, COLORLINE[(line-1)%11], color);
	m_pApp->WriteProfileInt(IDCODE, HCOLORLINE[(line-1)%11], colorH);


	return FALSE;
}	

}
vector<string> MFC_Util::GetFileNames(LPCTSTR lpszFilter, std::string DefExt) {


	return GetFileNames(lpszFilter, CA2T(DefExt.c_str()));


}
vector<string> MFC_Util::GetFileNames(LPCTSTR lpszFilter, LPCTSTR lpszDefExt) {
	CString pszFileName;
	int fileDlgResult;
	vector<string> out;


	CFileDialog	DriveFileDlg(TRUE, lpszDefExt, pszFileName, OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT, lpszFilter);
	DriveFileDlg.m_ofn.lpstrFile = &buffer[0];
	DriveFileDlg.m_ofn.nMaxFile = 120000;
	//DriveFileDlg.m_ofn.lpstrInitialDir=UserDirectory;

	fileDlgResult=DriveFileDlg.DoModal();
	if (fileDlgResult==IDCANCEL)
		return out;

	POSITION pos=DriveFileDlg.GetStartPosition( );
	while(pos!=nullptr) {
		out.push_back(CT2A(DriveFileDlg.GetNextPathName(pos)).m_psz);
	}

	return out;
}
string MFC_Util::GetSingleFileName(LPCTSTR lpszFilter,LPCTSTR lpszDefExt) {
	CString pszFileName ;
	pszFileName="";
	//pszFileName+=lpszDefExt;
	int fileDlgResult;
	CFileDialog	DriveFileDlg(TRUE,lpszDefExt,pszFileName, OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,lpszFilter);

	fileDlgResult=DriveFileDlg.DoModal();
	if (fileDlgResult==IDCANCEL)
		return "";
	return CT2A(DriveFileDlg.GetPathName()).m_psz;
}
vector<string> MFC_Util::GetBinTxtFileNames() {
	vector<string> out;
	LPCTSTR lpszFilter;
	lpszFilter = _T("PG File (*.bin)|*.Bin|PG File Text Input (*.Txt)|*.Txt|");
	out=GetFileNames(lpszFilter,"Bin|Txt");
	return out;
}
vector<string> MFC_Util::GetMBinFileNames() {
	vector<string> out;
	LPCTSTR lpszFilter, lpszType;
	lpszFilter = _T("Muli Storage Sim Files (*.MBin)|*.MBin");
	lpszType = _T("MBin Files (*.MBin)|*.MBin|");
	out = GetFileNames(lpszFilter, lpszType);
	return out;
}
vector<string> MFC_Util::GetSBInFileNames()
{
	vector<string> out;
	LPCTSTR lpszFilter;
	lpszFilter = _T("SBin Files (*.SBin)");
	out = GetFileNames(lpszFilter, "SBin");
	return out;

}
string MFC_Util::GetSBinFileName()
{
	LPCTSTR lpszFilter;
	lpszFilter = _T("SBin Files (*.SBin)|*.SBin");
	return GetSingleFileName(lpszFilter, _T("SBin"));
}
vector<string> MFC_Util::GetSimFileNames() {
	vector<string> out;
	LPCTSTR lpszFilter;
	lpszFilter = _T("Sim Files (*.sim)|*.Sim|Sim Files(*.xlm)");
	out=GetFileNames(lpszFilter,"Sim|xlm");
	return out;
}
vector<string> MFC_Util::GetBinFileNames() {
	vector<string> out;
	LPCTSTR lpszFilter;
	lpszFilter = _T("PG File (*.bin)|*.Bin|");
	out=GetFileNames(lpszFilter,"Bin");
	return out;
}
vector<string> MFC_Util::GetTxtFileNames() {
	vector<string> out;
	LPCTSTR lpszFilter;
	lpszFilter = _T("Txt (*.txt)|*.txt|");
	out=GetFileNames(lpszFilter,"txt");
	return out;
}
string MFC_Util::GetCSVFileName() {

	LPCTSTR lpszFilter;
	lpszFilter = _T("Comma separated (*.csv)|*.csv|PG File Text Input (*.Txt)|*.txt|");
	return GetSingleFileName(lpszFilter,_T("csv|txt"));

}
vector<string> MFC_Util::GetCSVFileNames() {

	LPCTSTR lpszFilter;
	lpszFilter = _T("Comma separated (*.csv)|*.csv|PG File Text Input (*.Txt)|*.txt|");
	return GetFileNames(lpszFilter, _T("*.csv|*.txt"));

}
string MFC_Util::GetCurrentDateTime() {
	CTime tt = CTime::GetCurrentTime();
	CString cstr = tt.Format("%Y-%m-%d %H:%M");
	return CT2A(cstr).m_psz;
}

string MFC_Util::GetBinTxtFileName() {
	LPCTSTR lpszFilter;
	lpszFilter = _T("PG File (*.bin)|*.Bin|PG File Text Input (*.Txt)|*.Txt|");
	return GetSingleFileName(lpszFilter,_T("Bin|Txt"));
}
string MFC_Util::GetSimFileName() {

	LPCTSTR lpszFilter;
	lpszFilter = _T("Sim Files (*.sim)|*.Sim|");
	return GetSingleFileName(lpszFilter,_T("Sim"));

}
string MFC_Util::GetBinFileName() {
	LPCTSTR lpszFilter;
	lpszFilter = _T("PG File (*.bin)|*.Bin|");
	return GetSingleFileName(lpszFilter,_T("Bin"));
}
string MFC_Util::GetTxtFileName() {
	LPCTSTR lpszFilter;
	lpszFilter = _T("Txt (*.txt)|*.txt|");
	return GetSingleFileName(lpszFilter,_T("txt"));
}
