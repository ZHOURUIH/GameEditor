#ifndef _SQLITE_DATA_READER_H_
#define _SQLITE_DATA_READER_H_

#include "ServerBase.h"

enum class SQLITE_DATATYPE : byte
{
	SQLITE_DATATYPE_INTEGER = SQLITE_INTEGER,
	SQLITE_DATATYPE_FLOAT = SQLITE_FLOAT,
	SQLITE_DATATYPE_TEXT = SQLITE_TEXT,
	SQLITE_DATATYPE_BLOB = SQLITE_BLOB,
	SQLITE_DATATYPE_NULL = SQLITE_NULL,
};

class SQLiteDataReader : public ServerBase
{
	BASE_CLASS(ServerBase);
public:
	SQLiteDataReader(sqlite3_stmt* stmt);	
	virtual ~SQLiteDataReader(); 
public:	
	bool read();// 读取一行数据,需要循环调用来读取多行数据
	void close();
	int getColumnCount();
	const char* getColumnName(int col);
	SQLITE_DATATYPE getDataType(int col);
	void getString(int col, string& str, bool toANSI = true);
	string getString(int col, bool toANSI = true);
	int getInt(int col);
	float getFloat(int col);
	const char* getBlob(int col, int& length);
protected:
	sqlite3_stmt* mStmt;
};

#endif