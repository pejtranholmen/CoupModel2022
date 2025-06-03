#include "CWinAPIFileHandler.h"

CWinAPIFileHandler::CWinAPIFileHandler() : m_Handle(INVALID_HANDLE_VALUE) {}

CWinAPIFileHandler::~CWinAPIFileHandler() {
    Close();
}

bool CWinAPIFileHandler::OpenRead(const std::wstring& filename) {
    Close();
    m_Handle = CreateFileW(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    return m_Handle != INVALID_HANDLE_VALUE;
}

bool CWinAPIFileHandler::OpenWrite(const std::wstring& filename) {
    Close();
    m_Handle = CreateFileW(filename.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    return m_Handle != INVALID_HANDLE_VALUE;
}

bool CWinAPIFileHandler::WriteBinary(const void* data, size_t size) {
    if (!IsOpen()) return false;
    DWORD written;
    return WriteFile(m_Handle, data, static_cast<DWORD>(size), &written, nullptr);
}

bool CWinAPIFileHandler::ReadBinary(void* data, size_t size) {
    if (!IsOpen()) return false;
    DWORD read;
    return ReadFile(m_Handle, data, static_cast<DWORD>(size), &read, nullptr);
}
std::int64_t CWinAPIFileHandler::GetFilePosition() const {
	if (!IsOpen()) return -1;
	LARGE_INTEGER li;
	if (SetFilePointerEx(m_Handle, { 0, 0 }, &li, FILE_CURRENT)) {
		return li.QuadPart;
	}
	return -1;
}
bool CWinAPIFileHandler::ReadTextLine(std::string& line) {

    size_t Dummy = 0;
    std::vector<float> floats;
    std::vector<int> ints;
    return ReadTextLine(line, Dummy, floats, ints);

}
bool CWinAPIFileHandler::ReadLineBuffer(std::string& line) {
    const size_t bufferSize = 8192;
    char buffer[bufferSize];
    DWORD bytesRead = 0;
    std::string lineBuffer;
    size_t recordIndex = 0;
    // Sök efter radslut i bufferten vi sparade från förra läsningen
    size_t pos;
    while ((pos = m_LineBuffer.find('\n')) == std::string::npos) {
        // Läs nytt block från filen
        if (!ReadFile(m_Handle, buffer, bufferSize, &bytesRead, nullptr) || bytesRead == 0) {
            // Inget mer att läsa
            if (!m_LineBuffer.empty()) {
                // returnera sista raden utan \n
                line = m_LineBuffer;
                m_LineBuffer.clear();
                return true;
            }
            return false;
        }
        m_LineBuffer.append(buffer, bytesRead);
    }
    // Plocka ut en hel rad
    line = m_LineBuffer.substr(0, pos);
    m_LineBuffer.erase(0, pos + 1);
    // Ta bort \r om den finns
    if (!line.empty() && line.back() == '\r') line.pop_back();

    return true;
}
bool CWinAPIFileHandler::ReadTextLine(std::string& line, std::string& txt) {

    auto good = ReadLineBuffer(line);
    if (good) {
        size_t pos = line.find(":");
        if (pos != std::string::npos) {
            txt = line.substr(pos + 1);
            line = line.substr(0, pos);
        }
        else {
            txt = "";
        }
    }
    return good;
}
bool CWinAPIFileHandler::ReadTextLine(std::string& line, bool &valid) {

    size_t Dummy = 0;
    std::vector<float> floats;
    std::vector<int> ints;
    bool good= ReadTextLine(line, Dummy, floats, ints);
    if(Dummy==0) valid=true;
	else valid=false;
    return good;
}
bool CWinAPIFileHandler::ReadTextLine(std::string& line, float& fvalue) {

    size_t Dummy = 0;
    std::vector<float> floats;
    std::vector<int> ints;
    bool good = ReadTextLine(line, Dummy, floats, ints);
    if (line.find(":") != std::string::npos) {
        auto pos = line.find(":");
        auto str2 = line.substr(pos + 1);
        try {
            fvalue = std::stof(str2);
        }
        catch (const std::invalid_argument&) {
			fvalue = -1.1E38f;
		}
    }
 
    return good;
}
bool CWinAPIFileHandler::ReadTextLine(std::string& line, size_t &num) {

    std::vector<float> floats;
    std::vector<int> ints;
    return ReadTextLine(line, num, floats, ints);

}
bool CWinAPIFileHandler::ReadTextLine(std::string& line, std::vector<float>& floats, std::vector<int>& ints) {

    size_t Dummy=0;
    return ReadTextLine(line, Dummy, floats, ints);
    
}
bool CWinAPIFileHandler::ReadTextLines(size_t num_rows, std::string& line, size_t& aftercol, std::vector<float>& floats, std::vector<int>& ints) {
    std::vector<float> row_floats;
    std::vector<int> row_ints;
    floats.clear();ints.clear();

    for(size_t i=0;i<num_rows;++i) {
		if(!ReadTextLine(line, aftercol, row_floats, row_ints)) return false;
        if (row_floats.size() != 0) {
            floats.insert(floats.end(), row_floats.begin(), row_floats.end());
        }
        if (row_ints.size() != 0) {
            ints.insert(ints.end(), row_ints.begin(), row_ints.end());
        }
	}
    if (floats.size() != aftercol) {

        std::string txt = "Error: Number of floats read does not match expected count.";;
		return false;   
    }
    return true;
}
bool CWinAPIFileHandler::ReadTextLines(size_t num_rows, std::string& line, size_t& aftercol, std::vector<float>& floats) {
    std::vector<float> row_floats;
    std::vector<int> row_ints;
    floats.clear(); 

    for (size_t i = 0; i < num_rows; ++i) {
        if (!ReadTextLine(line, row_floats)) return false;
        if (row_floats.size() != 0) {
            floats.insert(floats.end(), row_floats.begin(), row_floats.end());
        }

    }
    if (floats.size() != aftercol) {

        std::string txt = "Error: Number of floats read does not match expected count.";;
        return false;
    }
    return true;
}
bool CWinAPIFileHandler::ReadTextLine(std::string& line,  std::vector<float>& floats) {
    if (!IsOpen()) return false;

    const size_t bufferSize = 8192;
    char buffer[bufferSize];
    DWORD bytesRead = 0;
    std::string lineBuffer;
    size_t recordIndex = 0;
    floats.clear();

    //aftercol = std::string::npos;

    // Sök efter radslut i bufferten vi sparade från förra läsningen
    size_t pos;
    while ((pos = m_LineBuffer.find('\n')) == std::string::npos) {
        // Läs nytt block från filen
        if (!ReadFile(m_Handle, buffer, bufferSize, &bytesRead, nullptr) || bytesRead == 0) {
            // Inget mer att läsa
            if (!m_LineBuffer.empty()) {
                // returnera sista raden utan \n
                line = m_LineBuffer;
                m_LineBuffer.clear();
                return true;
            }
            return false;
        }
        m_LineBuffer.append(buffer, bytesRead);
    }
    // Plocka ut en hel rad
    line = m_LineBuffer.substr(0, pos);
    m_LineBuffer.erase(0, pos + 1);

    // Ta bort \r om den finns
    if (!line.empty() && line.back() == '\r') line.pop_back();

    if (!line.empty()) {
        if (line.find(',') != std::string::npos) {

            std::vector<std::string> tokens;
            size_t start = 0;
            size_t end = 0;


            if (line.rfind(":") != std::string::npos) {
                auto pos = line.rfind(":");
                start = pos + 1;
            }

            while ((end = line.find(',', start)) != std::string::npos) {
                std::string token = line.substr(start, end - start);
                tokens.push_back(token);
                start = end + 1;
            }
            std::string last = line.substr(start);
            tokens.push_back(last);
 
                floats.resize(tokens.size());
                for (size_t i = 0; i < tokens.size(); ++i) {
                    if(tokens[i].size() > 0) 
                        floats[i] = std::stof(tokens[i]);
                    else
                          floats[i] = -1.1E38f;
                }

        }


    }
    pos = line.rfind(":");
    if (pos != std::string::npos) {
        line = line.substr(0, pos);
    }

    return true;
}
bool CWinAPIFileHandler::ReadTextLine(std::string &line, size_t &aftercol,std::vector<float>& floats, std::vector<int>& ints) {
    if (!IsOpen()) return false;

    const size_t bufferSize = 8192;
    char buffer[bufferSize];
    DWORD bytesRead = 0;
    std::string lineBuffer;
    size_t recordIndex = 0;
    ints.clear();
    floats.clear();
    bool isFloat = false;
    //aftercol = std::string::npos;
   
    // Sök efter radslut i bufferten vi sparade från förra läsningen
    size_t pos;
    while ((pos = m_LineBuffer.find('\n')) == std::string::npos) {
        // Läs nytt block från filen
        if (!ReadFile(m_Handle, buffer, bufferSize, &bytesRead, nullptr) || bytesRead == 0) {
            // Inget mer att läsa
            if (!m_LineBuffer.empty()) {
                // returnera sista raden utan \n
                line = m_LineBuffer;
                m_LineBuffer.clear();
                return true;
            }
            return false;
        }
        m_LineBuffer.append(buffer, bytesRead);
    }
    // Plocka ut en hel rad
    line = m_LineBuffer.substr(0, pos);
    m_LineBuffer.erase(0, pos + 1);

    // Ta bort \r om den finns
    if (!line.empty() && line.back() == '\r') line.pop_back();

    if (!line.empty()) {
        if (line.find(',') != std::string::npos) {
 
            std::vector<std::string> tokens;
            size_t start = 0;
            size_t end = 0;


            if(line.rfind(":") != std::string::npos) {
				auto pos = line.rfind(":");
                start=pos+1;
			}

            while ((end = line.find(',', start)) != std::string::npos) {
                std::string token = line.substr(start, end - start);
                if (token.find('.') != std::string::npos) isFloat = true;
                tokens.push_back(token);
                start = end + 1;
            }
            std::string last = line.substr(start);
            tokens.push_back(last);
            if (isFloat) {
                // Split std::vector
  
                floats.resize(tokens.size());
                for(size_t i = 0; i < tokens.size(); ++i) {
					if(tokens[i].size()>0) {
						floats[i] = std::stof(tokens[i]);
					}
                    else
						floats[i]=-1.1E38f;
					
				}
            }
            else {
                // Split std::vector
 
                ints.resize(tokens.size());
                for (size_t i = 0; i < tokens.size(); ++i) {
                    if (tokens[i].size() > 0)
							ints[i] = std::stoi(tokens[i]);
                        else    
							ints[i] = -99999;

                }
            }
        }
        else if (line.find(":") != std::string::npos) {
            auto pos=line.rfind(":");
            auto str = line.substr(pos + 1);
            try {
                aftercol = stoul(str);
            }
			catch (const std::invalid_argument&) {
				aftercol = std::string::npos;
			}
        }

    }
    pos = line.rfind(":");
    if (pos != std::string::npos) {
	    line = line.substr(0,pos);
	}
 
    return true;
}

bool CWinAPIFileHandler::ReadTextLine(std::string& line, std::vector<float>& floats, std::vector<size_t>& sizets) {

    size_t Dummy = 0;
    return ReadTextLine(line, Dummy, floats, sizets);

}
bool CWinAPIFileHandler::ReadTextLine(std::string& line, size_t& aftercol, std::vector<float>& floats, std::vector<size_t>& sizets) {
    if (!IsOpen()) return false;

    const size_t bufferSize = 8192;
    char buffer[bufferSize];
    DWORD bytesRead = 0;
    std::string lineBuffer;
    size_t recordIndex = 0;
    sizets.clear();
    floats.clear();
   // aftercol = std::string::npos;

    // Sök efter radslut i bufferten vi sparade från förra läsningen
    size_t pos;
    while ((pos = m_LineBuffer.find('\n')) == std::string::npos) {
        // Läs nytt block från filen
        if (!ReadFile(m_Handle, buffer, bufferSize, &bytesRead, nullptr) || bytesRead == 0) {
            // Inget mer att läsa
            if (!m_LineBuffer.empty()) {
                // returnera sista raden utan \n
                line = m_LineBuffer;
                m_LineBuffer.clear();
                return true;
            }
            return false;
        }
        m_LineBuffer.append(buffer, bytesRead);
    }
    // Plocka ut en hel rad
    line = m_LineBuffer.substr(0, pos);
    m_LineBuffer.erase(0, pos + 1);

    // Ta bort \r om den finns
    if (!line.empty() && line.back() == '\r') line.pop_back();

    if (!line.empty()) {
        if (line.find(',') != std::string::npos) {

            std::vector<std::string> tokens;
            size_t start = 0;
            size_t end = 0;
            bool isFloat = false;

            while ((end = line.find(',', start)) != std::string::npos) {
                std::string token = line.substr(start, end - start);
                if (token.find('.') != std::string::npos) isFloat = true;
                tokens.push_back(token);
                start = end + 1;
            }
            std::string last = line.substr(start);
            if (last.size() > 0) tokens.push_back(last);
            if (isFloat) {
                // Split std::vector

                floats.resize(tokens.size());
                for (size_t i = 0; i < tokens.size(); ++i) {
                    if(tokens[i].size() > 0) 
                        floats[i] = std::stof(tokens[i]);
                    else 
                        floats[i] = -1.1E38f;

                }
            }
            else {
                // Split std::vector

                sizets.resize(tokens.size());
                for (size_t i = 0; i < tokens.size(); ++i) {
                    if(tokens[i].size()>0) 
                        sizets[i] = std::stoul(tokens[i]);
                    else 
                        sizets[i] = std::string::npos;
              
                }
            }
        }
        else if (line.find(":") != std::string::npos) {
            auto pos = line.find(":");
            auto str = line.substr(pos + 1);
            try {
                aftercol = stoul(str);
            }
            catch (const std::invalid_argument&) {
                aftercol = std::string::npos;
            }
        }

    }
    return true;
}

bool CWinAPIFileHandler::IsOpen() const {
    return m_Handle != INVALID_HANDLE_VALUE;
}
bool CWinAPIFileHandler::WriteTextLine(const std::vector<float>& floats, size_t num_row)
// num=number of floats per row
{
    if (!IsOpen()) return false;
    std::string line;
    bool rowbreak = true;
    DWORD written;
    if(num_row==std::string::npos) {
		rowbreak = false;
	}
    size_t count_row=1;
    if(rowbreak) line += std::to_string(count_row)+":";
    for (size_t i = 0; i < floats.size(); ++i) {
        if (floats[i] > -1.E38f) {
			line += std::to_string(floats[i]);
		}
        if (rowbreak && (i + 1) % num_row == 0) {
            line += "\r\n";
            WriteFile(m_Handle, line.c_str(), static_cast<DWORD>(line.size()), &written, nullptr);
            line.clear();
            if (i + 1 != floats.size()) {
            
                count_row++;
                line += std::to_string(count_row) + ":";
            }
        }
        else if (i < floats.size() - 1) line += ",";
    }
    if(!rowbreak)  line += "\r\n";
   
    return WriteFile(m_Handle, line.c_str(), static_cast<DWORD>(line.size()), &written, nullptr);
}

bool CWinAPIFileHandler::WriteTextLine(const std::vector<int>& ints)
{
    if (!IsOpen()) return false;
    std::string line;
    for (size_t i = 0; i < ints.size(); ++i) {
        line += std::to_string(ints[i]);
        if (i < ints.size() - 1) line += ",";
    }
    line += "\r\n";
    DWORD written;
    return WriteFile(m_Handle, line.c_str(), static_cast<DWORD>(line.size()), &written, nullptr);
}
bool CWinAPIFileHandler::WriteTextLine(const std::vector<size_t>& ints)
{
    if (!IsOpen()) return false;
    std::string line;
    for (size_t i = 0; i < ints.size(); ++i) {
        line += std::to_string(ints[i]);
        if (i < ints.size() - 1) line += ",";
    }
    line += "\r\n";
    DWORD written;
    return WriteFile(m_Handle, line.c_str(), static_cast<DWORD>(line.size()), &written, nullptr);
}
bool CWinAPIFileHandler::WriteTextLine(const std::string& line)
{
    if (!IsOpen()) return false;
    std::string out = line + "\r\n";
    DWORD written;
    return WriteFile(m_Handle, out.c_str(), static_cast<DWORD>(out.size()), &written, nullptr);
}
bool CWinAPIFileHandler::WriteTextLine(const std::string& line, const float &floatvalue)
{
    if (!IsOpen()) return false;
    std::string out = line +":"+std::to_string(floatvalue)+ "\r\n";
    DWORD written;
    return WriteFile(m_Handle, out.c_str(), static_cast<DWORD>(out.size()), &written, nullptr);
}

bool CWinAPIFileHandler::WriteTextLine(const std::string& line, const std::string&str)
{
    if (!IsOpen()) return false;
    std::string out = line + ":"+str+ "\r\n";
    DWORD written;
    return WriteFile(m_Handle, out.c_str(), static_cast<DWORD>(out.size()), &written, nullptr);
}

bool CWinAPIFileHandler::WriteTextLine(const std::string& key, const size_t &value)
{
    if (!IsOpen()) return false;
    std::string out = key + ":" + std::to_string(value) + "\r\n";
    DWORD written;
    return WriteFile(m_Handle, out.c_str(), static_cast<DWORD>(out.size()), &written, nullptr);
}
bool CWinAPIFileHandler::WriteTextLine(const std::string& key, const bool& value)
{
    if (!IsOpen()) return false;
    std::string out = key + ":" + std::to_string(value) + "\r\n";
    DWORD written;
    return WriteFile(m_Handle, out.c_str(), static_cast<DWORD>(out.size()), &written, nullptr);
}

bool CWinAPIFileHandler::WriteTextLine(const std::string& key, const size_t &value1, const size_t &value2)
{
    if (!IsOpen()) return false;
    std::string out = key + ":" + std::to_string(value1)+ "," +std::to_string(value2)+ "\r\n";
    DWORD written;
    return WriteFile(m_Handle, out.c_str(), static_cast<DWORD>(out.size()), &written, nullptr);
}

void CWinAPIFileHandler::Close() {
    if (IsOpen()) {
        CloseHandle(m_Handle);
        m_Handle = INVALID_HANDLE_VALUE;
    }
}

