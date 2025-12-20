#pragma once

class ReadWriteTypes
{
public:
	ReadWriteTypes(void);
	~ReadWriteTypes(void);
	bool OpenDocFileIn(std::string filename, bool binary_type=false);
	bool OpenDocFileout(std::string filename);
	bool IsFileInGood() noexcept {return m_DocFileIn.good();};
	bool IsFileOutGood() noexcept  {return m_DocFileOut.good();};
	bool SetGetPosition(size_t iv);
	bool IsBinaryFile() noexcept { return m_binary; };
	bool wInt(int *iv);
	bool wSize_t(size_t *iv);
	bool wInt(unsigned int *iv);
#ifdef COUPSTD
    //bool wInt(size_t *iv);
#endif
	bool wPosition();
	bool SetGetPosition(size_t *pos);
	bool SetPutPosition(size_t *pos);
	std::size_t GetCurrentInPosition();
	std::size_t GetInputFileSize();
	bool wbool(bool *iv);
	bool wLong(long *iv);
	bool wFloat(float *f);
	bool wDouble(double *f);
	bool wTime(time_t *time);
	bool wString(std::string *str);
	bool rString(std::string *str);
	bool rInt(int *iv);
	bool rsize_t(size_t *iv);
#ifdef COUPSTD
	//bool rsize_t(unsigned int* iv);
#endif
	bool ruLong(unsigned long *iv);
	bool rBOOL(bool *iv);
	bool rbool(bool *iv);
	bool rLong(long *iv);
	bool ruint(unsigned int* iv);
	bool rFloat(float *f);
	bool rDouble(double *f);
	bool rTime(time_t *time);
	std::string GetFileName() { return m_FileName; };
	void CloseFileIn();
	void CloseFileOut();
	bool rFloatArray(float *f, size_t num);
	bool wFloatArray(const std::vector<float> &f);
	bool wFloatArray(float *f, size_t num);
    std::vector<float> rFloatArray2( size_t num, size_t linebreak_cycle = std::string::npos) ;
	bool wFloatArray2(const std::vector<float> &f, size_t num, size_t linebreak_cycle= std::string::npos) ;
	bool wFloatArray2(const std::vector<float> &f, size_t linebreak_cycle = std::string::npos) ;

	bool ruLongArray(unsigned long *v,unsigned long num);
	bool wuLongArray(const std::vector<unsigned long> &v, size_t num);

	bool wsize_tArray(const std::vector<size_t>  &v, size_t num= std::string::npos, size_t linebreak_cycle = std::string::npos);
	std::vector<size_t> rsize_tArray(size_t num, size_t linebreak_cycle = std::string::npos);
	std::streamsize GetStdInputFileSize();
	std::fstream m_DocFileIn;
	std::fstream m_DocFileOut;
protected:
	std::string m_FileName;
private :

	char m_cbuf[4000];
	int m_FileVersionNumberRead;
	bool m_ShortTimeRead;
	bool m_binary;
};

