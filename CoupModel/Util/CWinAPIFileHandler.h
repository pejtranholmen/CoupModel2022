#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <codecvt>

class CWinAPIFileHandler {
public:
    CWinAPIFileHandler();
    ~CWinAPIFileHandler();

    bool OpenRead(const std::wstring& filename);
    bool OpenWrite(const std::wstring& filename);
    bool WriteBinary(const void* data, size_t size);
    bool ReadBinary(void* data, size_t size);
    bool ReadLineBuffer(std::string& line);
    bool ReadTextLine(std::string& line);
    bool ReadTextLine(std::string& line, std::string& txt);
    bool ReadTextLine(std::string& line, bool& boolaftercol);
    bool ReadTextLine(std::string& line, float& floataftercol);
    bool ReadTextLine(std::string& line, size_t& intaftercol);
    bool ReadTextLine(std::string &line, size_t &intaftercol, std::vector<float>& floats, std::vector<int>& ints);
    bool ReadTextLines(size_t rows, std::string& line, size_t& intaftercol, std::vector<float>& floats, std::vector<int>& ints);
    bool ReadTextLines(size_t rows, std::string& line, size_t& intaftercol, std::vector<float>& floats);
    bool ReadTextLine(std::string& line, std::vector<float>& floats, std::vector<int>& ints);
    bool ReadTextLine(std::string& line, size_t& intaftercol, std::vector<float>& floats, std::vector<size_t>& ints);
    bool ReadTextLine(std::string& line, std::vector<float>& floats, std::vector<size_t>& ints);
    bool ReadTextLine(std::string& line, std::vector<float>& floats);
    std::int64_t GetFilePosition() const;
    bool SetFilePosition(std::int64_t position) {
		if (!IsOpen()) return false;
		LARGE_INTEGER li;
		li.QuadPart = position;
		return SetFilePointerEx(m_Handle, li, nullptr, FILE_BEGIN);
	}
    bool IsOpen() const;
    bool IsGood() const { return IsOpen(); }
    void Close();
    bool WriteTextLine(const std::vector<float>& floats, size_t numrow=std::string::npos);
    bool WriteTextLine(const std::vector<int>& ints);
    bool WriteTextLine(const std::vector<size_t>& ints);
    bool WriteTextLine(const std::string& line);
    bool WriteTextLine(const std::string& line, const std::string &str);
    bool WriteTextLine(const std::string& key, const float &value);
    bool WriteTextLine(const std::string& key, const size_t &value);
    bool WriteTextLine(const std::string& key, const bool &value);
    bool WriteTextLine(const std::string& key, const size_t &value1, const size_t &value2);


    std::wstring ToWString(const std::string& str) {
        return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(str);
    };
    HANDLE GetHandle() const { return m_Handle; };

private:
    HANDLE m_Handle;
    std::string m_LineBuffer;

};
