#pragma once

class Doc;
struct SIMB;
class Ps;
class Sw;
class Tab;
struct rgb_color
{
	int red;
	int green;
	int blue;
};



struct DistStat
{
	float Mean;
	float Std;
	float Min;
	float Max;
	float P5;
	float P50;
	float P95;
	size_t NumData;
};


class FUtil

{

public:
	FUtil(void);
	~FUtil(void);
	static std::string check_open_newfile(fstream *filesteam, std::string filename);
	static std::string check_open_newcsvfile(fstream *filesteam, std::string filename);
	static bool check_if_empty_file(fstream *filesteam, std::string filename);
	static void WriteProfileStringStd(string item, std::string value);
	static std::string GetProfileStringStd(string item, std::string value);
	static int  GetProfileIntNo(string item, int value=1);
	static void  WriteProfileInt(string item, int value);
	static void trim(string& str);
	static void trim_datestr(string& str);
	static void trim_space(string&str);
	static void trim_index(string &str);
	static void trim_xmlstring(string& str);
	static float AtoFloat(string str);
	static std::string ItoNumAscii(int);
	static std::string ItoNum2Ascii(int);
	static std::string	STD_FtoAscii(float v) ;
	static std::string GetString(char* buf);
	static std::string STD_FtoAsciiPane(float v);
	static std::string	STD_DtoAscii(double v) ;
	static std::string	STD_ItoAscii(int v) ;
	static std::string	STD_ItoAscii(size_t value) { return STD_ItoAscii(int(value)); };
	//static std::string	STD_ItoAsciiHex(int v);
	static int	AtoInt(string str);
	static int GetOptionIndex(string optionlist, std::string str);
	static std::string GetOptionString(string optionlist, int Index);
	static bool IsFileExisting(string str);
	static bool CopyFile_Std(string infile,string outfile);

	static float Exp(int) ;
	static double Decimal(int) ;
	static double PedoFunc_AirEntry(double SAND, double CLAY, double PORO);
	static double PedoFunc_Lambda(double SAND, double CLAY, double PORO);
	static double PedoFunc_Residual(double SAND, double CLAY, double PORO);
	static double PedoFunc_HydraulicCond(double SAND, double CLAY, double PORO);
	static double PedoFunc_WiltingPoint(double CLAY);
	static int GetPositionSumInitialFromOutSheet();

	static std::vector<Doc*> GetReadyDoc(Doc* CurrentDoc);
	static std::vector<SIMB> SortSimB_ByGroup(vector<SIMB> v_in);
	static std::vector<SIMB> SortSimB_ByName(vector<SIMB> v_in);
	static std::vector<SIMB> SortSimB_ByProcess(vector<SIMB> v_in);
	static std::vector<SIMB> SortSimB_ByElement(vector<SIMB> v_in);
	static std::vector<SIMB> SortSimB_ByType(vector<SIMB> v_in);
	static std::vector<Ps*> SortSimB_ByGroup(vector<Ps*> v_in);
	static std::vector<Ps*> SortSimB_ByName(vector<Ps*> v_in);
	static std::vector<Ps*> SortSimB_ByProcess(vector<Ps*> v_in);
	static std::vector<Ps*> SortSimB_ByElement(vector<Ps*> v_in);
	static std::vector<Sw*> SortSimB_ByGroup(vector<Sw*> v_in);
	static std::vector<Sw*> SortSimB_ByName(vector<Sw*> v_in);
	static std::vector<Sw*> SortSimB_ByProcess(vector<Sw*> v_in);
	static std::vector<Sw*> SortSimB_ByElement(vector<Sw*> v_in);
	static std::vector<Tab*> SortSimB_ByGroup(vector<Tab*> v_in);
	static std::vector<Tab*> SortSimB_ByName(vector<Tab*> v_in);
	static std::vector<Tab*> SortSimB_ByProcess(vector<Tab*> v_in);
	static std::vector<Tab*> SortSimB_ByElement(vector<Tab*> v_in);

	static SIMB GetPointerforNumberinList(vector<SIMB> v_in, size_t num);
	static bool SetOutPutFlag_FromLineInput(string line, Doc* pSimDoc);


	static std::vector<float> GetFloatVectorFromString(string str, size_t num);
	static std::vector<string> GetStringVectorFromStringLine(string str, size_t num=0);
	static char GetDelimChar();
	static void SetDelimChar(char delim);

	static std::string FileNameOnly(string urlname);
	static std::string DirectoryNameOnly(string urlname);
	static float GetNormalValue(float value, float minvalue, float maxvalue);
	static std::string GetWindowUser();
	static std::string GetCurrentDateTime();
	static pair<string, size_t> GetIndexAndName(string str);

	static rgb_color Get_RGB(float value);
	static bool IsValidNumber(string str);
	static std::string NameRevision2018(string str);


private:
	


};

