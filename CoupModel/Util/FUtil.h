#pragma once
#include <cmath>
#include <algorithm>
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


namespace PedoFunc {

	inline double AirEntry(double SAND, double CLAY, double PORO) noexcept {
		double C[13] = { 5.3396738, 0.1845038, -2.48394546, -0.00213853,
			-0.04356349, -0.61745089, 0.00143598, -0.00855375,
			-0.00001282, 0.00895359, -0.00072472, 0.0000054, 0.50028060 };
		double A = C[0] + C[1] * CLAY + C[2] * PORO + C[3] * CLAY * CLAY + C[4] * SAND * PORO
			+ C[5] * PORO * CLAY + C[6] * SAND * SAND * PORO * PORO + C[7] * CLAY * CLAY * PORO * PORO
			+ C[8] * SAND * SAND * CLAY + C[9] * CLAY * CLAY * PORO
			+ C[10] * SAND * SAND * PORO + C[11] * CLAY * CLAY * SAND + C[12] * PORO * PORO * CLAY;
		return std::exp(A);
	}

	inline double Lambda(double SAND, double CLAY, double PORO) noexcept {
		double C[12] = { -0.7842831, 0.0177544, -1.062468, -0.00005304,
			-0.00273493, 1.11134946, -0.03088295, 0.00026587,
			-0.00610522, -0.00000235, 0.00798746, -0.00674491 };
		double A = C[0] + C[1] * SAND + C[2] * PORO + C[3] * SAND * SAND + C[4] * CLAY * CLAY
			+ C[5] * PORO * PORO + C[6] * SAND * PORO + C[7] * SAND * SAND * PORO * PORO
			+ C[8] * CLAY * CLAY * PORO * PORO + C[9] * SAND * SAND * CLAY
			+ C[10] * CLAY * CLAY * PORO + C[11] * CLAY * PORO * PORO;
		return std::exp(A);
	}

	inline double Residual(double SAND, double CLAY, double PORO) noexcept {
		double C[9] = { -0.0182482 , 0.00087269, 0.00513488, 0.02939286,
			-0.00015395, -0.0010827, -0.00018233, 0.00030703, -0.0023584 };
		double A = C[0];
		A += C[1] * SAND;
		A += C[2] * CLAY;
		A += C[3] * PORO;
		A += C[4] * CLAY * CLAY;
		A += C[5] * SAND * PORO;
		A += C[6] * CLAY * CLAY * PORO * PORO;
		A += C[7] * CLAY * CLAY * PORO;
		A += C[8] * CLAY * PORO * PORO;
		A *= 100;
		if (A > 100.0) A = 100.0; 
		else if(A<0.0) A = 0.0; 
		return A;
	}

	inline double HydraulicCond(double SAND, double CLAY, double PORO) noexcept {
		double C[13] = { 19.52348, -8.96847, -0.028212, 0.00018107,
			-0.0094125, -8.395215, 0.077718, -0.00298,
			-0.019492, 0.0000173, 0.02733, 0.001434, -0.0000035 };
		double A = PORO * C[0];
		A += C[1] + C[2] * CLAY + C[3] * SAND * SAND + C[4] * CLAY * CLAY + C[5] * PORO * PORO
			+ C[6] * SAND * PORO + C[7] * SAND * SAND * PORO * PORO + C[8] * CLAY * CLAY * PORO * PORO
			+ C[9] * SAND * SAND * CLAY + C[10] * CLAY * CLAY * PORO + C[11] * SAND * SAND * PORO
			+ C[12] * CLAY * CLAY * SAND;
		return 240.0 * std::exp(A);
	}

	inline double WiltingPoint(double CLAY) noexcept {
		return 0.45 * CLAY + 2.0;
	}

} // namespace PedoFunc


class FUtil

{

public:
	FUtil(void);
	~FUtil(void);
	static std::string check_open_newfile(std::fstream *filesteam, std::string filename);
	static std::string check_open_newcsvfile(std::fstream *filesteam, std::string filename);
	static bool check_if_empty_file(std::fstream *filesteam, std::string filename);
	static void WriteProfileStringStd(std::string item, std::string value);
	static std::string GetProfileStringStd(std::string item, std::string value);
	static int  GetProfileIntNo(std::string item, int value=1);
	static void  WriteProfileInt(std::string item, int value);
	static void trim(std::string& str);
	static void trim_datestr(std::string& str);
	static void trim_space(std::string&str);
	static void trim_index(std::string &str);
	static void trim_xmlstring(std::string& str);
	static float AtoFloat(std::string str);
	static std::string ItoNumAscii(int);
	static std::string ItoNum2Ascii(int);
	static std::string	STD_FtoAscii(float v) ;
	static std::string GetString(char* buf);
	static std::string STD_FtoAsciiPane(float v);
	static std::string	STD_DtoAscii(double v) ;
	static std::string	STD_ItoAscii(int v) ;
	static std::string	STD_ItoAscii(size_t value) { return STD_ItoAscii(int(value)); };
	//static std::string	STD_ItoAsciiHex(int v);
	static int	AtoInt(std::string str);
	static int GetOptionIndex(std::string optionlist, std::string str);
	static std::string GetOptionString(std::string optionlist, int Index);
	static bool IsFileExisting(std::string str);
	static bool CopyFile_Std(std::string infile,std::string outfile);

	static float Exp(int) ;
	static double Decimal(int) ;
	static int GetPositionSumInitialFromOutSheet();

	static std::vector<Doc*> GetReadyDoc(Doc* CurrentDoc);
	static std::vector<SIMB> SortSimB_ByGroup(std::vector<SIMB> v_in);
	static std::vector<SIMB> SortSimB_ByName(std::vector<SIMB> v_in);
	static std::vector<SIMB> SortSimB_ByProcess(std::vector<SIMB> v_in);
	static std::vector<SIMB> SortSimB_ByElement(std::vector<SIMB> v_in);
	static std::vector<SIMB> SortSimB_ByType(std::vector<SIMB> v_in);
	static std::vector<Ps*> SortSimB_ByGroup(std::vector<Ps*> v_in);
	static std::vector<Ps*> SortSimB_ByName(std::vector<Ps*> v_in);
	static std::vector<Ps*> SortSimB_ByProcess(std::vector<Ps*> v_in);
	static std::vector<Ps*> SortSimB_ByElement(std::vector<Ps*> v_in);
	static std::vector<Sw*> SortSimB_ByGroup(std::vector<Sw*> v_in);
	static std::vector<Sw*> SortSimB_ByName(std::vector<Sw*> v_in);
	static std::vector<Sw*> SortSimB_ByProcess(std::vector<Sw*> v_in);
	static std::vector<Sw*> SortSimB_ByElement(std::vector<Sw*> v_in);
	static std::vector<Tab*> SortSimB_ByGroup(std::vector<Tab*> v_in);
	static std::vector<Tab*> SortSimB_ByName(std::vector<Tab*> v_in);
	static std::vector<Tab*> SortSimB_ByProcess(std::vector<Tab*> v_in);
	static std::vector<Tab*> SortSimB_ByElement(std::vector<Tab*> v_in);

	static SIMB GetPointerforNumberinList(std::vector<SIMB> v_in, size_t num);
	static bool SetOutPutFlag_FromLineInput(std::string line, Doc* pSimDoc);


	static std::vector<float> GetFloatVectorFromString(std::string str, size_t num);
	static std::vector<std::string> GetStringVectorFromStringLine(std::string str, size_t num=0);
	static char GetDelimChar();
	static void SetDelimChar(char delim);

	static std::string FileNameOnly(std::string urlname);
	static std::string DirectoryNameOnly(std::string urlname);
	static float GetNormalValue(float value, float minvalue, float maxvalue);
	static std::string GetWindowUser();
	static std::string GetCurrentDateTime();
	static std::pair<std::string, size_t> GetIndexAndName(std::string str);

	static rgb_color Get_RGB(float value);
	static bool IsValidNumber(std::string str);
	static std::string NameRevision2018(std::string str);


private:
	


};

