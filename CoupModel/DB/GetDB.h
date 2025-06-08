#pragma once
#include "DB_Storage.h"
#include "../Util/ReadWriteTypes.h"
class CDB;
class GetDB :
	public DB_Storage
{
public:
	GetDB(void);
	~GetDB(void);
	bool Ini_DBBase();
	std::string GetDBSimFileDate(std::string group, std::string key, bool modified);

	bool ReadOpenHeaderFile(size_t index, bool add=false, bool newformat=false);
	bool WriteHeaderFile(size_t index);
	size_t GetNumOfRec(size_t index=0);
	DB_OldHeader GetHeader(size_t ityp, size_t rec);
	DB_NewHeader GetNewHeader(size_t ityp, size_t rec);
	std::string	GetTypeString(size_t itype, size_t Type);
	std::string	GetKeyString(std::string ID, size_t IdNum);
	std::string GetKeyString();
	size_t CreateNewHeader(size_t db_index);
	CDB* CreateNewDBObject_FromCSVFile(std::string FileName);
private :

	ReadWriteTypes m_Rw;
};

