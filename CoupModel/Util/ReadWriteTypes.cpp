#include "../std.h"
#include "../Util/ReadWriteTypes.h"
#include "FUtil.h"


#include "../std.h"
#include "../Util/ReadWriteTypes.h"
#include "FUtil.h"


ReadWriteTypes::ReadWriteTypes(void)
{
	m_FileVersionNumberRead = 51;
	m_ShortTimeRead = true;
}


ReadWriteTypes::~ReadWriteTypes(void)
{
}

bool ReadWriteTypes::OpenDocFileIn(string filename)
{
	m_DocFileIn.open(filename);
	if (!m_DocFileIn.good())
	{
		m_DocFileIn.clear();
		m_DocFileIn.open(filename);
	}

	m_FileName = filename;

	return m_DocFileIn.good();
}

bool ReadWriteTypes::OpenDocFileout(string filename)
{
	if (FUtil::IsFileExisting(filename))
		m_DocFileOut.open(filename, ios_base::in|ios_base::out);
	else
		m_DocFileOut.open(filename, ios_base::out);

	size_t n = 0;
	SetPutPosition(&n);
	m_FileName = filename;
	return m_DocFileOut.good();
}

void ReadWriteTypes::CloseFileIn()
{
	m_DocFileIn.close();
}

void ReadWriteTypes::CloseFileOut()
{
	m_DocFileOut.close();
}

bool ReadWriteTypes::wString(string* str)
{
	size_t il = str->size();
	if (il > 0)
		m_DocFileOut << *str << endl;

	return true;
}

bool ReadWriteTypes::rString(string* str)
{
	size_t il;
	string s;
	m_DocFileIn >> s;


	if (il > 0)
	{
		char* buffer = new char[il];
		m_DocFileIn.getline(buffer, il + 1);
		*str = buffer;
		delete[] buffer;
	}

	return true;
}

bool ReadWriteTypes::rInt(int* iv)
{
	m_DocFileIn >> *iv;
	return true;
}

bool ReadWriteTypes::rsize_t(size_t* iv)
{
	m_DocFileIn >> *iv;
	return m_DocFileIn.good();
}

bool ReadWriteTypes::ruint(unsigned int* iv)
{
	m_DocFileIn >> *iv;
	return m_DocFileIn.good();
}

bool ReadWriteTypes::SetGetPosition(size_t* iv)
{
	m_DocFileIn.seekg(*iv);
	return true;
}

bool ReadWriteTypes::SetPutPosition(size_t* iv)
{
	m_DocFileOut.seekp(*iv);
	if (m_DocFileOut.tellg() == *iv)
		return true;
	return false;
}

bool ReadWriteTypes::rBOOL(bool* iv)
{
	int iBOOL;
	m_DocFileIn >> iBOOL;
	*iv = (iBOOL != 0);
	return true;
}

bool ReadWriteTypes::rbool(bool* iv)
{
	m_DocFileIn >> *iv;
	return true;
}

bool ReadWriteTypes::rLong(long* iv)
{
	m_DocFileIn >> *iv;
	return true;
}

bool ReadWriteTypes::ruLong(unsigned long* iv)
{
	m_DocFileIn >> *iv;
	return true;
}

bool ReadWriteTypes::rFloat(float* f)
{
	m_DocFileIn >> *f;
	return true;
}

bool ReadWriteTypes::rFloatArray(float* f, size_t n)
{
	for (size_t i = 0; i < n; i++)
		m_DocFileIn >> f[i];
	return m_DocFileIn.good();
}

bool ReadWriteTypes::ruLongArray(unsigned long* v, unsigned long n)
{
	for (unsigned long i = 0; i < n; i++)
		m_DocFileIn >> v[i];
	return m_DocFileIn.good();
}

vector<size_t> ReadWriteTypes::rsize_tArray(size_t n,  size_t line_break)
{
	string str, str_line;
	if(line_break<n)
		getline(m_DocFileIn, str, ',');

	vector<size_t> v; v.resize(n);
	for (size_t i = 0; i < n; i++)
	{
		if ((i + 1) % line_break == 0) {
			if (i < n - 1) {
				getline(m_DocFileIn, str);	getline(m_DocFileIn, str_line, ',');
			}
		}
		else
			getline(m_DocFileIn, str, ',');
		auto value = atoi(str.c_str());
		v[i]=value;
	}
	return v;
}

bool ReadWriteTypes::wFloatArray(float* f, size_t n)
{
	for (size_t i = 0; i < n; i++)
		m_DocFileOut << f[i] << " ";
	m_DocFileOut << endl;
	return m_DocFileOut.good();
}

bool ReadWriteTypes::wFloatArray2(vector<float> v, size_t n, size_t line_break)
{
	size_t line_count = 1;
	m_DocFileOut << line_count << ',';
	for (size_t i = 0; i < n; i++) {
		if (abs(v[i]) < 1.E37) {
			if (abs(v[i]) < 1.E-20)
				m_DocFileOut << 0.;
			else
				m_DocFileOut << v[i];
		}
		if ((i + 1) % line_break == 0) {
			line_count++;
			if (i < n - 1) {
				m_DocFileOut << endl << line_count << ',';
			}
		}
		else m_DocFileOut << ",";
	}
	m_DocFileOut << endl;
	return m_DocFileOut.good();
}

vector<float> ReadWriteTypes::rFloatArray2(size_t n, size_t line_break)
{
	vector<float> v;	v.resize(n);
	string str, str_line;
	getline(m_DocFileIn, str, ',');
	for (size_t i = 0; i < n; i++)
	{
		if ((i + 1) % line_break == 0) {
			if (i < n - 1) {
				getline(m_DocFileIn, str);	getline(m_DocFileIn, str_line, ',');
			}
		}
		else
			getline(m_DocFileIn,str,',');
		auto value=atof(str.c_str());
		v[i]=value;
	}
	return v;
}

bool ReadWriteTypes::wuLongArray(unsigned long* v, size_t n)
{
	for (size_t i = 0; i < n; i++)
		m_DocFileOut << v[i] << " ";
	m_DocFileOut << endl;
	return m_DocFileOut.good();
}

bool ReadWriteTypes::wsize_tArray(vector<size_t>* v, size_t n, size_t line_break)
{
	size_t line_count = 1;
	if(line_break<n)
		m_DocFileOut << line_count<<',';
	for (size_t i = 0; i < n; i++) {
		m_DocFileOut << v->at(i);
		if (i + 1 % line_break == 0) {
			line_count++;
			if(i<n-1) {
				m_DocFileOut << endl << line_count << ',';			
			}
		}
		else m_DocFileOut << ",";
	}
	m_DocFileOut << endl;
	return m_DocFileOut.good();
}

bool ReadWriteTypes::rDouble(double* f)
{
	m_DocFileIn >> *f;
	return true;
}

bool ReadWriteTypes::wInt(int* iv)
{
	m_DocFileOut << *iv << endl;
	return true;
}

bool ReadWriteTypes::wInt(unsigned int* iv)
{
	m_DocFileOut << *iv << endl;
	return m_DocFileOut.good();
}

bool ReadWriteTypes::wSize_t(size_t* iv)
{
	m_DocFileOut << *iv << endl;
	return m_DocFileOut.good();
}

bool ReadWriteTypes::wPosition()
{
	auto pos = m_DocFileOut.tellp();
	size_t ipos = size_t(pos);
	m_DocFileOut << ipos << endl;
	return m_DocFileOut.good();
}

bool ReadWriteTypes::wbool(bool* iv)
{
	m_DocFileOut << *iv << endl;
	return true;
}

bool ReadWriteTypes::wLong(long* iv)
{
	m_DocFileOut << *iv << endl;
	return true;
}

bool ReadWriteTypes::wFloat(float* f)
{
	m_DocFileOut << *f << endl;
	return true;
}

bool ReadWriteTypes::wDouble(double* f)
{
	m_DocFileOut << *f << endl;
	return true;
}

bool ReadWriteTypes::wTime(time_t* time)
{
	m_DocFileOut << *time << endl;
	return true;
}

bool ReadWriteTypes::rTime(time_t* time)
{
	m_DocFileIn >> *time;
	return true;
}

size_t ReadWriteTypes::GetCurrentInPosition()
{
	auto actpos = m_DocFileIn.tellg();
	return size_t(actpos);
}

size_t ReadWriteTypes::GetInputFileSize()
{
	m_DocFileIn.seekg(0, ios::end);
	auto actpos = m_DocFileIn.tellg();
	m_DocFileIn.seekg(0, ios::beg);
	return size_t(actpos);
}

std::streamsize ReadWriteTypes::GetStdInputFileSize()
{
	m_DocFileIn.seekg(0, ios::end);
	std::streamsize actpos = m_DocFileIn.tellg();
	m_DocFileIn.seekg(0, ios::beg);
	return actpos;
}

